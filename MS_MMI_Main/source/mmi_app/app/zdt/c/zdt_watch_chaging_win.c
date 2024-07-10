#ifndef _MMIZDT_CHARGINGWINTAB_C_
#define _MMIZDT_CHARGINGWINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
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
#include "zdt_common.h"
#include "zdt_app.h"
#include "mmiidle_export.h"
#include "mmi_position.h"
#include "ldo_drvapi.h"
#include "img_dec_interface.h"
#include "guiownerdraw.h"
#include "graphics_draw.h"
#include "mmiparse_export.h"

#ifdef ENG_SUPPORT
#include "mmieng_export.h"
#endif

#ifdef XYSDK_SUPPORT
#include "libxmly_api.h"
#endif

#include "zdt_win_export.h"

//充电动画用gif
#define CHARGING_ANIM_GIF

static uint8 my_charging_tips_timer = 0;

static void timer_charging_tips_callback(uint32 param)
{
    GUILABEL_SetVisible(MMIZDT_CHARGE_TIPS_CTRL_ID,FALSE,TRUE);
}

static void timer_charging_tips_create(void)
{
    my_charging_tips_timer = MMK_CreateTimerCallback(1000,timer_charging_tips_callback,PNULL,FALSE);
}


LOCAL MMI_RESULT_E  HandleZDT_ChargingWinMsg( MMI_WIN_ID_T win_id,  MMI_MESSAGE_ID_E msg_id, DPARAM param);

WINDOW_TABLE( MMIZDT_CHARGING_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_ChargingWinMsg),    
    WIN_ID( MMIZDT_CHARGING_WIN_ID ),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIZDT_CHARGE_TIPS_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};


#define ENG_TEST_VERSION 

LOCAL uint8 charging_image_index = 0;
LOCAL uint8 charging_image_tips_index = 0;
LOCAL BOOLEAN test_mark = FALSE; //for test use
LOCAL uint16 click_count = 0 ;  //for test use
LOCAL uint8 charging_timer_id = 0;
LOCAL uint32 anim_total_count = 0;
LOCAL uint32 charging_win_open_ms = 0;
static BOOLEAN charging_anim_flag = TRUE;
extern PUBLIC BOOLEAN MMIAPIGET_ZdtWatchPowerWinDisplay(void);

#if 0
//充电gif动画
LOCAL void MMIZDT_CreateGiFAnim()
{
    GUIANIM_CTRL_INFO_T             ctrl_info = {0};
    GUIANIM_DATA_INFO_T             data_info = {0};
    GUIANIM_DISPLAY_INFO_T          display_info = {0};
    GUIANIM_INIT_DATA_T             anim_init = {0};
    MMI_CONTROL_CREATE_T            anim_ctrl = {0};
    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id =MMIZDT_CHARGING_ANIM_CTRL_ID;
    anim_init.both_rect = MMITHEME_GetFullScreenBothRect();   
    data_info.img_id = IMG_DIGIT_CLOCK2_BG;     
    //creat anim
    anim_ctrl.ctrl_id           = MMIIDLE_ANIM_CTRL_ID;
    anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
    anim_ctrl.init_data_ptr     = &anim_init;
    anim_ctrl.parent_win_handle = MMK_ConvertIdToHandle(MMIZDT_CHARGING_WIN_ID);
    MMK_CreateControl(&anim_ctrl);

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = FALSE;
    display_info.is_bg_buf      = TRUE;
    display_info.bg.bg_type     = GUI_BG_COLOR;
    display_info.bg.color       = MMI_WINDOW_BACKGROUND_COLOR;
    display_info.is_update = TRUE;
    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
}

LOCAL GUI_LCD_DEV_INFO  s_charging_layer_handle = {0,UILAYER_NULL_HANDLE};
//gif 动画上显示信息 需要画在图层上
LOCAL void Create_Charging_PaintLayer(MMI_WIN_ID_T  win_id)
{

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_charging_layer_handle))    
    {
        uint16   layer_width = 0;
        uint16   layer_height = 0;
        UILAYER_CREATE_T    create_info = {0};
        UILAYER_APPEND_BLT_T    append_layer = {0};
        //get tips layer width height
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&layer_width,&layer_height);    

        //creat layer
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = layer_width;
        create_info.height = layer_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(&create_info, &s_charging_layer_handle);
      
        append_layer.lcd_dev_info = s_charging_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_HIGH;
        UILAYER_AppendBltLayer(&append_layer);
    }
    else
    {
        if (UILAYER_IsLayerActive(&s_charging_layer_handle))
        {
            UILAYER_Clear(&s_charging_layer_handle);
        }
    }
}

#endif

LOCAL void MMIZDT_DrawBatteryText(GUI_LCD_DEV_INFO  lcd_dev_info, BOOLEAN full)
{
    GUI_RECT_T battery_text_rect = {0, 205, 180, 239};
    MMI_STRING_T battery_string = {0};
    GUISTR_STATE_T battery_text_state = GUISTR_STATE_ALIGN;
    GUISTR_STYLE_T battery_text_style = {0};
    if(full)
    {
        battery_text_style.align = ALIGN_HVMIDDLE;
        battery_text_style.font_color = MMI_WHITE_COLOR;
        battery_text_style.font = SONG_FONT_26;
        battery_text_rect.right= 239;
        MMIRES_GetText(BATTERY_TXT_CHARGING_FULL,win_id,&battery_string);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &battery_text_rect,
            &battery_text_rect,
            &battery_string,
            &battery_text_style,
            battery_text_state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
    else
    {
        char battery_char[5] = {0};
        wchar battery_wchar[10] = {0};
        GUI_RECT_T battery_percent_rect = {100, 209, 239, 239};
        uint8 battery = 0;
        battery = MMIAPIPHONE_GetBatteryPercent();
        MMIRES_GetText(BATTERY_TXT_CHARGING_NOW,win_id,&battery_string);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &battery_text_rect,
            &battery_text_rect,
            &battery_string,
            &battery_text_style,
            battery_text_state,
            GUISTR_TEXT_DIR_AUTO
            );

        battery_text_style.font = SONG_FONT_30;
        sprintf(battery_char, "%d%s", battery, "%");
        battery_string.wstr_ptr = MMIAPICOM_StrToWstr((const uint8 *)battery_char, battery_wchar);
        battery_string.wstr_len = MMIAPICOM_Wstrlen(battery_string.wstr_ptr);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &battery_percent_rect,
            &battery_percent_rect,
            &battery_string,
            &battery_text_style,
            battery_text_state,
            GUISTR_TEXT_DIR_AUTO
            );
    }

}

LOCAL void MMIZDT_DrawChargingImage(GUI_LCD_DEV_INFO  lcd_dev_info)
{
    GUI_RECT_T anim_rect = {0,141, (MMI_MAINSCREEN_WIDTH -1), 141+78};
    GUI_RECT_T animol_rect = {24, 35, 216, 200};
    GUI_RECT_T battery_rect = {24, 110, 216, 200};
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect(win_id);
    MMI_IMAGE_ID_T img_id_list[9] = 
    {
        IMAGE_ZDT_CHARGING_ANIM1,
        IMAGE_ZDT_CHARGING_ANIM2,
        IMAGE_ZDT_CHARGING_ANIM3,
        IMAGE_ZDT_CHARGING_ANIM4,
        IMAGE_ZDT_CHARGING_ANIM5,
        IMAGE_ZDT_CHARGING_ANIM6,
        IMAGE_ZDT_CHARGING_ANIM7,
    };


    //充满了
    if(ZDT_GetIsBatFull() || ZDT_IsChargeDone())
    {
        charging_image_index = 9; 
        charging_image_tips_index = 8;
        GUIRES_DisplayImg(PNULL,&full_rect,PNULL,win_id,IMAGE_ZDT_CHARGING_FULL,&lcd_dev_info);
        MMIZDT_DrawBatteryText(lcd_dev_info,TRUE);

    }
    else
    {    
        GUIRES_DisplayImg(PNULL,&full_rect,PNULL, win_id,img_id_list[charging_image_tips_index],&lcd_dev_info);
        GUIRES_DisplayImg(PNULL,&full_rect,PNULL, win_id,IMAGE_ZDT_CHARGING_BG,&lcd_dev_info);
        MMIZDT_DrawBatteryText(lcd_dev_info,FALSE);

    }
}

LOCAL void MMIZDT_StartChargingTimer()
{
    if(charging_timer_id != 0)
    {
        MMK_StopTimer(charging_timer_id);
    }
    charging_timer_id =  MMK_CreateWinTimer(MMIZDT_CHARGING_WIN_ID, 1000,TRUE);

}

LOCAL void MMIZDT_StopChargingTimer()
{
    if(charging_timer_id != 0)
    {
        MMK_StopTimer(charging_timer_id);
    }
}

PUBLIC void MMIZDT_PowerOffCloseCharging()
{
    MMIZDT_StopChargingTimer();
}

PUBLIC BOOLEAN MMIZDT_IsInChargingWin()
{
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId() ;

    return win_id == MMIZDT_CHARGING_WIN_ID;
}

PUBLIC BOOLEAN MMIZDT_IsOpenChargingWin()
{
    return MMK_IsOpenWin(MMIZDT_CHARGING_WIN_ID);
}

PUBLIC BOOLEAN MMIZDT_ShouldStayInChargingWin()
{
    BOOLEAN isCharging = ZDT_GetIsCharge() ;
    BOOLEAN isInChargingWin = MMIZDT_IsInChargingWin() ;
    return  isCharging && isInChargingWin;
}

PUBLIC void MMIZDT_CheckOpenChargingWin()
{
    //is_in_factorywin = TRUE;
    if(is_in_factorywin||is_in_Agingfactorywin || cancel_charge_anim_func())
    {
        return ;
    }
    if(ZDT_GetIsCharge() && !test_mark && STARTUP_NORMAL == MMIAPIPHONE_GetStartUpCondition() && !MMICC_IsInActiveCallWin())
    {
        if(MMK_GetFocusWinId() != MMIZDT_CHARGING_WIN_ID)
        {
            if(MMK_IsOpenWin(MMIZDT_CHARGING_WIN_ID))
            {
                //MMK_CloseWin(MMIZDT_CHARGING_WIN_ID);
            }
            else
            {
                uint32 cur_ms = MMIAPICOM_GetFfsTime();
                if(z_abs(cur_ms - charging_win_open_ms) > 300)
                {
                    charging_win_open_ms = cur_ms;
                    #ifdef CAMERA_SUPPORT
                    MMIDC_CloseCameraWin();
                    #endif
                    MMK_CreateWin((uint32*)MMIZDT_CHARGING_WIN_TAB,PNULL);
                }
  
            }
        }
    }
}


PUBLIC void MMIZDT_OpenChargingWin()
{
    //is_in_factorywin = TRUE;
    if(is_in_factorywin||is_in_Agingfactorywin || cancel_charge_anim_func())
    {
        return ;
    }
   if(MMK_GetFocusWinId() != MMIZDT_CHARGING_WIN_ID && !test_mark)
   {

        //MMK_CreateWin((uint32*)MMIZDT_CHARGING_WIN_TAB,PNULL);
        uint32 cur_ms = MMIAPICOM_GetFfsTime();
        if(z_abs(cur_ms - charging_win_open_ms) > 300)
        {
            charging_win_open_ms = cur_ms;
            #ifdef CAMERA_SUPPORT
            MMIDC_CloseCameraWin();
            #endif
            MMK_CreateWin((uint32*)MMIZDT_CHARGING_WIN_TAB,PNULL);
        }

   }
}

PUBLIC void MMK_CloseChargingWin()
{
    if(MMK_IsOpenWin(MMIZDT_CHARGING_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_CHARGING_WIN_ID);
    }
}
/************************************************************************/
static uint8 getcharginganimnum(uint8 param)
{

    if(MMIAPIPHONE_GetBatteryPercent() > 0 && MMIAPIPHONE_GetBatteryPercent() <= 12 && param <= 0)
        return 0;
    else if(MMIAPIPHONE_GetBatteryPercent() > 12 && MMIAPIPHONE_GetBatteryPercent() <= 24 && param <= 1)
        return 1;
    else if(MMIAPIPHONE_GetBatteryPercent() > 24 && MMIAPIPHONE_GetBatteryPercent() <= 36 && param <= 2)
        return 2;
    else if(MMIAPIPHONE_GetBatteryPercent() > 36 && MMIAPIPHONE_GetBatteryPercent() <= 48 && param <= 3)
        return 3;
    else if(MMIAPIPHONE_GetBatteryPercent() > 48 && MMIAPIPHONE_GetBatteryPercent() <= 60 && param <= 4)
        return 4;
    else if(MMIAPIPHONE_GetBatteryPercent() > 60 && MMIAPIPHONE_GetBatteryPercent() <= 72 && param <= 5)
        return 5;
    else if(MMIAPIPHONE_GetBatteryPercent() > 72 && MMIAPIPHONE_GetBatteryPercent() <= 84 && param <= 6)
        return 6;
    else if(MMIAPIPHONE_GetBatteryPercent() > 84 && MMIAPIPHONE_GetBatteryPercent() <= 99 && param <= 7)
        return 7;
    else if(MMIAPIPHONE_GetBatteryPercent() == 100)
        return 8;
    else 
        return 9;
}
/************************************************************************/
extern BOOLEAN MMIVIDEO_IsOpenVideoWin(void);

extern BOOLEAN MMISOS_IsOpenSosWin(void);

LOCAL MMI_RESULT_E  HandleZDT_ChargingWinMsg(
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
    GUISTR_INFO_T       text_info = {0};
    GUI_RECT_T lowbattery_title_left_rect = {20,100,220,140};
    GUI_BG_T lowbattery_charge;
    lowbattery_charge.bg_type = GUI_BG_COLOR;
    lowbattery_charge.shape = GUI_SHAPE_ROUNDED_RECT;
    lowbattery_charge.is_screen_img = FALSE;
    lowbattery_charge.color = MMI_WHITE_COLOR;
    
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            GUILABEL_SetRect(MMIZDT_CHARGE_TIPS_CTRL_ID, &lowbattery_title_left_rect, FALSE); 
            GUILABEL_SetBg(MMIZDT_CHARGE_TIPS_CTRL_ID,&lowbattery_charge); 
            GUILABEL_SetFont(MMIZDT_CHARGE_TIPS_CTRL_ID, SONG_FONT_20, MMI_BLACK_COLOR);
            GUILABEL_SetTextById(MMIZDT_CHARGE_TIPS_CTRL_ID,TXT_CHARGE_PROHIBIT_TIPS,FALSE);
            GUILABEL_SetVisible(MMIZDT_CHARGE_TIPS_CTRL_ID,FALSE,TRUE);
            if(MMIIDLE_GetIdleWinInfo()->is_charge && charging_anim_flag)
            {
                charging_anim_flag = FALSE;
                charging_image_index = 0;
                charging_image_tips_index = getcharginganimnum(0);
            }
            else
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
            }
            
            anim_total_count = 0;
            MMIZDT_StartChargingTimer();
            click_count = 0;//for test
            //MMIDEFAULT_StartLcdBackLightTimer(); //yangyu add for bug
            break;
        case MSG_GET_FOCUS:
            if(MMIIDLE_GetIdleWinInfo()->is_charge && charging_anim_flag)
            {
                charging_anim_flag = FALSE;
                charging_image_index = 0;
                charging_image_tips_index = getcharginganimnum(0);
            }
            anim_total_count = 0;
            MMIZDT_StartChargingTimer();
            //MMIDEFAULT_StartLcdBackLightTimer(); //yangyu add for bug
            break;
    case MSG_FULL_PAINT:
    {

        if(!ZDT_IsRealCharingVol())
        {
            MMK_SendMsg(win_id, MSG_CLOSE_WINDOW, NULL);
            break;
        }
        
        GUI_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
        
        MMIZDT_DrawChargingImage(lcd_dev_info);
        {
            MMI_MESSAGE_PTR_T mmi_msg_ptr;
            if (MMK_GetMSGQueue(&mmi_msg_ptr)) 
            {
                MMK_DispatchMSGQueue(mmi_msg_ptr);
                MMK_FreeMSG(mmi_msg_ptr);
            }
        }
    }
        break;
        
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
            GUILABEL_SetVisible(MMIZDT_CHARGE_TIPS_CTRL_ID,TRUE,TRUE);
            timer_charging_tips_create();
            #ifdef ENG_TEST_VERSION
            if(point.x > 180 && point.y > 160)
            {
              click_count += 1;
            }
            if(click_count >= 9)
            {
                //edit by fys 2022/06/10
              //test_mark = TRUE;
              //MMK_CloseWin(win_id);
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

        case MSG_CTL_MIDSK:
            break;
        case MSG_TIMER:
        {
            if (*(uint8*)param == charging_timer_id && MMIAPIGET_ZdtWatchPowerWinDisplay())
            {
                charging_image_index++;
                if(charging_image_index >= 10)
                {
                    charging_image_index = 0;
                }
                charging_image_tips_index++;
                if(charging_image_tips_index > 8)
                {
                    charging_image_tips_index = 0;
                }
                if(getcharginganimnum(charging_image_tips_index) != 9)
                    charging_image_tips_index = getcharginganimnum(charging_image_tips_index);
                anim_total_count++;
                SCI_TRACE_LOW("yyu anim_total_count=%d",anim_total_count);
                if(anim_total_count == 1)
                {
                    SCI_TRACE_LOW("yyu MMIDEFAULT_StartLcdBackLightTimer");
                   // MMIDEFAULT_StartLcdBackLightTimer();
                }
                if(anim_total_count >= 20)
                {
                    SCI_TRACE_LOW("yyu MMIDEFAULT_StartLcdBackLightTimer2");
                    anim_total_count = 0;
                    //MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                    
                }
            }
        }
        
        break;
        case MSG_BACKLIGHT_TURN_ON:
        case MSG_BACKLIGHT_TURN_OFF:
            anim_total_count = 0;
        break;
        //case MSG_LOSE_FOCUS:
        case MSG_CLOSE_WINDOW:
        { 
            extern BOOLEAN power_on_flag;
            if(!MMIIDLE_GetIdleWinInfo()->is_charge && !charging_anim_flag)
            {
                charging_anim_flag = TRUE;
            }
            MMIZDT_StopChargingTimer();
            anim_total_count = 0;
            if(MMIAPIPHONE_GetBatteryPercent() <= LOWBATTERY_MODE_MAX_NUM && !power_on_flag)
            {
                MMIAPILOWBATTERY_CloseWin();
                MMIAPILOWBATTERY_CreateLowBatteryWin();
            }
        }
        
        break;
        case MSG_KEYUP_RED:
            MMIDEFAULT_TurnOffBackLight();//yangyu add    
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
