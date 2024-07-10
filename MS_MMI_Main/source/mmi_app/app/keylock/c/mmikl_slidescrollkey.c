/*****************************************************************************
** File Name:      mmikl_slidescrollkey.c                                    *
** Author:         jian.ma                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe slide scrollkey function    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create
******************************************************************************/
#ifdef MMI_KEY_LOCK_SUPPORT
#ifndef _MMIKL_SLIDESCROLLKEY_C_

#include "mmi_app_keylock_trc.h"
#ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmikl_slidescrollkey.h"
#include "mmk_type.h"
#include "guianim.h"
#include "mmitheme_pos.h"
#include "mmi_id.h"
#include "guiscrollkey.h"
#include "mmi_image.h"
#include "mmi_position.h"
#include "mmi_keylock_position.h"
#include "chg_drvapi.h"
#include "mmi_text.h"
#include "mmicc_text.h"
#include "mn_type.h"
#include "mmikl_export.h"
#include "guires.h"
#include "guictrl_api.h"
#include "mmi_textfun.h"
#include "mmicc_export.h"
#include "mmisms_export.h"
#include "mmialarm_export.h"
#include "mmiidle_export.h"
#include "guistatusbar.h"
#include "mmi_appmsg.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmi_wallpaper_export.h"
#include "mmk_tp.h"
#include "mmikl_effectmain.h"
#include "mmikl_internal.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

#define KEYLOCK_MAX_DATA_STRING_SIZE      50
#define KEYLOCK_DATE_WEEK_STRING_SIZE     16
#define KEYLOCK_SPACE_BETWEEN_DATE_WEEK   3

typedef enum
{
    KL_CHARGING_INFO,
    KL_MISS_CALL_INFO,
    KL_UNREAD_MESSAGE_INFO,
    KL_UNDO_ENENT_INFO,
    KL_DYN_INFO_MAX
}MMI_KL_DYN_INFO_E;

typedef uint32 MMIKL_INFO_NUM;

typedef struct
{
    MMI_STRING_T    mmikl_info;
    MMIKL_INFO_NUM  mmikl_num;
}MMIKL_INFO_CONTENT_T;



/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
  
/**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  Display clock callback function of effect type2.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayClockCallbackFunEffect2 (GUIANIM_OWNER_DRAW_T *draw_ptr);

/*****************************************************************************/
//  Description :  Create the control of effect2.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateKeyLockControlEffect2(MMI_WIN_ID_T win_id)
{
    GUIANIM_DATA_INFO_T         data_info = {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
    GUIANIM_CTRL_INFO_T         control_info = {0};

    uint16       image_width = 0, image_height = 0;
    GUI_RECT_T   rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T both_rect = {0};

#ifdef GUIF_SCROLLKEY
    GUISCROLLKEY_SetConfirmAnim(win_id, MMICOMMON_SCROLLKEY_CTRL_ID, RIGHTSCROLL_BUTTON, TRUE);
#endif
   
    
    //set timer owerdraw bg
    data_info.img_id = IMAGE_IDLE_KEYLOCK_TIME_TRANSPARENT_BG;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIKL_CLOCK_ANIM_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_LEFT_VMIDDLE;
    GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
    GUIANIM_SetOwnerDraw(MMIKL_CLOCK_ANIM_ID, DisplayClockCallbackFunEffect2);

    /* set rect */
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_IDLE_KEYLOCK_TIME_TRANSPARENT_BG, MMIKL_CLOCK_DISP_WIN_ID);
    rect.top = MMI_STATUSBAR_HEIGHT + MMIKL_TOP_OFFSET_FROM_STATUS_BAR;
    rect.bottom = rect.top + image_height;

    both_rect.h_rect = rect;
    both_rect.v_rect = rect;
    GUIAPICTRL_SetBothRect(MMIKL_CLOCK_ANIM_ID, &both_rect);
    //GUIAPICTRL_SetRect(MMIKL_CLOCK_ANIM_ID, &rect);
    //GUIAPICTRL_SetDisplayRect(MMIKL_CLOCK_ANIM_ID, &rect, FALSE);
}

LOCAL void SetInfoEffect2(MMIKL_INFO_CONTENT_T *info_area_data, GUIANIM_DATA_INFO_T *info_area_icon)
{
    uint16       image_width = 0, image_height = 0;
    GUI_RECT_T   both_rect = MMITHEME_GetClientRect();
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();

    //clock
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_IDLE_KEYLOCK_TIME_TRANSPARENT_BG, MMIKL_CLOCK_DISP_WIN_ID);
    both_rect.top = MMI_STATUSBAR_HEIGHT + MMIKL_TOP_OFFSET_FROM_STATUS_BAR;
    both_rect.bottom = both_rect.top + image_height;
    //GUIAPICTRL_SetRect(MMIKL_CLOCK_ANIM_ID, &both_rect);

    //date & week
    both_rect.top = both_rect.bottom;
    both_rect.bottom = both_rect.top + MMIKL_DATE_WEEK_LABEL_HEIGHT;

    //battery
    MMI_GetLabelTextByLang(TXT_BTRY_CAP_LABEL_TEXT, &info_area_data[KL_CHARGING_INFO].mmikl_info);
    info_area_data[KL_CHARGING_INFO].mmikl_num = p_chgmng_info->bat_remain_cap;
    info_area_icon[KL_CHARGING_INFO].img_id = IMAGE_IDLE_KEYLOCK_CHARGE;    

    //call
    MMI_GetLabelTextByLang(TXT_COMMON_MISS_CALLS, &info_area_data[KL_MISS_CALL_INFO].mmikl_info);
    info_area_data[KL_MISS_CALL_INFO].mmikl_num = MMIAPICC_GetMissedCallNumber();
    info_area_icon[KL_MISS_CALL_INFO].img_id = IMAGE_IDLE_KEYLOCK_CALL_EFFECT2;

    //message
    /* Modify by michael on 3/8/2012 for NEWMS00177382 : Add new string accorrding the num of unread msg*/
    info_area_data[KL_UNREAD_MESSAGE_INFO].mmikl_num = MMIAPISMS_GetAllUnreadMsgCount()
    + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX)
    ;

    if( 1 == info_area_data[KL_UNREAD_MESSAGE_INFO].mmikl_num )
    {
        MMI_GetLabelTextByLang(TXT_KL_UNREAD_MESSAGES, &info_area_data[KL_UNREAD_MESSAGE_INFO].mmikl_info); 
    }
    else if(info_area_data[KL_UNREAD_MESSAGE_INFO].mmikl_num > 1)
    {
        MMI_GetLabelTextByLang(TXT_KL_UNREAD_MESSAGES2, &info_area_data[KL_UNREAD_MESSAGE_INFO].mmikl_info); 
    }	

    /************************Modify over*************************/
    info_area_icon[KL_UNREAD_MESSAGE_INFO].img_id = IMAGE_IDLE_KEYLOCK_MESSAGE_EFFECT2;

    //event
    MMI_GetLabelTextByLang(TXT_KL_EVENT, &info_area_data[KL_UNDO_ENENT_INFO].mmikl_info);
    info_area_data[KL_UNDO_ENENT_INFO].mmikl_num = MMIAPIALM_GetTotalTimesOfArrivedEvent();
    info_area_icon[KL_UNDO_ENENT_INFO].img_id = IMAGE_IDLE_KEYLOCK_HINT_EFFECT2;
}

/*****************************************************************************/
//  Description :  Update key lock screen of effect2.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void UpdateKeyLockScreenEffect2(MMI_WIN_ID_T win_id)
{
    GUIANIM_DATA_INFO_T    info_area_icon[KL_DYN_INFO_MAX] = {0};   
    MMIKL_INFO_CONTENT_T   info_area_data[KL_DYN_INFO_MAX] = {0};
    GUI_LCD_DEV_INFO lcd_dev = {0};
    GUI_RECT_T   rect = MMITHEME_GetClientRect();

    uint16  index = 0, image_width = 0, image_height = 0;
    uint8 str_len = 0;
    wchar count_wstr[KEYLOCK_COUNT_STRING_SIZE] = {0}, data_wstr[KEYLOCK_MAX_DATA_STRING_SIZE] = {0};
    char count_str[KEYLOCK_COUNT_STRING_SIZE] = {0};

    uint16 line_height = 0;
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;
    GUI_POINT_T pos = {0};
    //clock
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_IDLE_KEYLOCK_TIME_TRANSPARENT_BG, win_id);
    rect.top = MMI_STATUSBAR_HEIGHT + MMIKL_TOP_OFFSET_FROM_STATUS_BAR;
    rect.bottom = rect.top + image_height;
    
    text_style.effect = FONT_EFFECT_REVERSE_EDGE; 
    text_style.angle = ANGLE_0;
    text_style.align = ALIGN_LEFT;
    text_style.font = MMI_DEFAULT_NORMAL_FONT;
    text_style.font_color = MMI_WHITE_COLOR;
    
    SetInfoEffect2(info_area_data, info_area_icon);

    GUIRES_GetImgWidthHeight(&image_width, &image_height, info_area_icon[0].img_id, win_id);
    line_height = image_height;
    
    rect.top = rect.bottom;
    rect.bottom += line_height;
    MMIKL_DisplayDateAndDay(&lcd_dev, rect, MMI_DEFAULT_NORMAL_FONT, ALIGN_VMIDDLE, MMI_WHITE_COLOR, FONT_EFFECT_REVERSE_EDGE);
    
    rect.left = image_width+4;
    for(index = 0; index < KL_DYN_INFO_MAX; index++)
    {
        if(KL_CHARGING_INFO == index)
        {
            if(!MMIAPIIDLE_IsChargeConnected())
            {
                continue;
            }
        }
        else if( 0 == info_area_data[index].mmikl_num)
        {
            continue;
        }
        
        rect.top = rect.bottom;
        rect.bottom += line_height;
        
        if(IMAGE_NULL != info_area_icon[index].img_id)
        {
            pos.x = 0;
            pos.y = rect.top;
            GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, info_area_icon[index].img_id, &lcd_dev);
        }

        if(info_area_data[index].mmikl_info.wstr_len > 0 && (PNULL != info_area_data[index].mmikl_info.wstr_ptr))
        {
            SCI_MEMSET(data_wstr, 0, KEYLOCK_MAX_DATA_STRING_SIZE);
            MMIAPICOM_Wstrncpy(data_wstr, info_area_data[index].mmikl_info.wstr_ptr, info_area_data[index].mmikl_info.wstr_len);

            if(KL_CHARGING_INFO == index)
            {
                sprintf(count_str, "(%d%%)", info_area_data[index].mmikl_num);
            }
            else
            {
                sprintf(count_str, "(%d)", info_area_data[index].mmikl_num);
            }
            str_len = strlen(count_str);
            MMI_STRNTOWSTR(count_wstr, KEYLOCK_COUNT_STRING_SIZE, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE, str_len);
            MMIAPICOM_Wstrcat(data_wstr, count_wstr);

            info_area_data[index].mmikl_info.wstr_ptr = data_wstr;
            info_area_data[index].mmikl_info.wstr_len = info_area_data[index].mmikl_info.wstr_len + str_len;                

            GUISTR_DrawTextToLCDInRect(
                &lcd_dev,
                &rect,
                &rect,
                (const MMI_STRING_T *)&info_area_data[index].mmikl_info,
                &text_style,
                text_state,
                0
                );
        }
    }
}


/*****************************************************************************/
//  Description :  Display clock callback function of effect type2.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayClockCallbackFunEffect2 (GUIANIM_OWNER_DRAW_T *draw_ptr)
{
    GUI_POINT_T point = {0};
    SCI_TIME_T time = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMISET_TIME_DISPLAY_TYPE_E time_display_type = MMIAPISET_GetTimeDisplayType();
    BOOLEAN b_am = TRUE;
    uint16 image_width = 0,image_height = 0;
    uint16 time_total_width = 0;

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN; 
    TM_GetSysTime(&time);

    //12小时制
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        GET_12STYLE_HOUR(time.hour, b_am);
    }

    //计算时间总的宽度，为了居中显示
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT2_DOT, MMIKL_CLOCK_DISP_WIN_ID);
    time_total_width = time_total_width  + image_width;//colon
    
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT2_AM,MMIKL_CLOCK_DISP_WIN_ID);
        time_total_width = time_total_width  + image_width;//am.pm
    }
    
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT2_DIGIT0,MMIKL_CLOCK_DISP_WIN_ID);
    time_total_width = time_total_width + image_width * 4 ;//有四个数字
    
    point.y = MAX(draw_ptr->display_y,draw_ptr->display_rect.top);
    point.x = MAX(draw_ptr->display_x,draw_ptr->display_rect.left);
        
    //HOUR
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_EFFECT2_DIGIT0 + time.hour/10,&lcd_dev_info);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_EFFECT2_DIGIT0 + time.hour%10,&lcd_dev_info);

    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_EFFECT2_DOT,&lcd_dev_info);

    //COLON IMAGE WIDTH
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT2_DOT,MMIKL_CLOCK_DISP_WIN_ID);
    //MIN
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_EFFECT2_DIGIT0 + time.min/10,&lcd_dev_info);

    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT2_DIGIT0,MMIKL_CLOCK_DISP_WIN_ID);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_EFFECT2_DIGIT0 + time.min%10,&lcd_dev_info);
    //AM.PM
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        point.x = point.x + image_width;
        if(b_am )
        {
            GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_EFFECT2_AM,&lcd_dev_info);
        }
        else
        {
            GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_KL_EFFECT2_PM,&lcd_dev_info);
        }
    }
}



/*****************************************************************************/
//  Description :  Handle win msg of effect type2 key lock win.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note: Slide right arc bar unlock style
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleSlideScrollKeyWinMsg(
                                                MMI_WIN_ID_T    win_id, 
                                                uint16          msg_id, 
                                                DPARAM          param
                                                )                    
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    //static BOOLEAN          s_is_timer_on       = FALSE;


    //SCI_TRACE_LOW:"MMIKL_HandleSlideScrollKeyWinMsg: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_SLIDESCROLLKEY_373_112_2_18_2_31_9_11,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        CreateKeyLockControlEffect2(win_id);
        GUIWIN_CreateStbDyn(MMIKL_CLOCK_DISP_WIN_ID);
        GUIWIN_SetStbState( MMIKL_CLOCK_DISP_WIN_ID, GUISTATUSBAR_STATE_SCROLLMSG_PAUSE, TRUE);
        break;
    case MSG_FULL_PAINT:
        UpdateKeyLockScreenEffect2(win_id);
        break;

#ifdef GUIF_SCROLLKEY
    case MSG_APP_LSKOK:
        MMK_PostMsg(win_id, MSG_KEY_LOCK_CLOSE_WIN, NULL, 0);
        break;
    
    case MSG_APP_RSKOK:
    {
        uint8 active_mode = 0;
        if (MMIKL_IsPreviewState())
        {
            break;
        }
        switch (MMIAPIENVSET_GetActiveModeId())
        {
        case MMIENVSET_STANDARD_MODE:  // 普通环境
        case MMIENVSET_INSIDE_MODE:      // 车内环境
        case MMIENVSET_NOISY_MODE:       // 户外环境
            active_mode = MMIENVSET_MEETING_MODE;
            MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_VIBRA, PNULL);
            GUIWIN_SetScrollkeytButtonIconId(win_id, IMAGE_IDLE_KEYLOCK_R_VIBRATE_ON, TXT_KL_SOUND_ON, GUISCROLLKEY_SKIN_YELLOW_ID, RIGHTSCROLL_BUTTON, TRUE);
            break;
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT 
        case MMIENVSET_EARPHONE_MODE:
        case MMIENVSET_BLUETOOTH_MODE:
            if (MMIAPIENVSET_GetMuteState())
            {
                active_mode = MMIENVSET_STANDARD_MODE;
                MMIAPIENVSET_SetMuteState(FALSE);
                GUIWIN_SetScrollkeytButtonIconId(win_id, IMAGE_IDLE_KEYLOCK_R_SOUND_ON, TXT_KL_SOUND_OFF, GUISCROLLKEY_SKIN_GRAY_ID, RIGHTSCROLL_BUTTON, TRUE);
            }
            else
            {
                active_mode = MMIENVSET_MEETING_MODE;
                MMIAPIENVSET_SetMuteState(TRUE);
                MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_VIBRA, PNULL);
                GUIWIN_SetScrollkeytButtonIconId(win_id, IMAGE_IDLE_KEYLOCK_R_VIBRATE_ON, TXT_KL_SOUND_ON, GUISCROLLKEY_SKIN_YELLOW_ID, RIGHTSCROLL_BUTTON, TRUE);
            }
            break;
#endif
        case MMIENVSET_SILENT_MODE:      // 安静环境               
        case MMIENVSET_MEETING_MODE:     // 会议环境
            active_mode = MMIENVSET_STANDARD_MODE;
            GUIWIN_SetScrollkeytButtonIconId(win_id, IMAGE_IDLE_KEYLOCK_R_SOUND_ON, TXT_KL_SOUND_OFF, GUISCROLLKEY_SKIN_GRAY_ID, RIGHTSCROLL_BUTTON, TRUE);
            break;

        default:
            //SCI_TRACE_LOW:"IdleWin_HandleMsg MSG_KEYLONG_HASH!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_SLIDESCROLLKEY_437_112_2_18_2_31_9_12,(uint8*)"");
            break;
        }

        //if(MMIAPIENVSET_GetActiveModeId()<= MMIENVSET_NOISY_MODE)
        {
            MMIAPIENVSET_SetActiveModeForIdle(active_mode);
        }
        break;
    }
#endif

  #if 0
    case MSG_KEYDOWN_RED:

        if(MMIDEFAULT_IsBacklightOn()&&MMIAPISET_GetRedKeyLock())
        {
#ifdef LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT        
            MMISEPC_CloseAllLight();
#else
            MMIDEFAULT_CloseAllLight();
#endif

        }
        
        //reset to locked state
        if (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus())
        {
            //stop timer
            MMIKL_StopUnlockConfirmTimer();
            MMIKL_SetWinStatus(KL_LOCKED);
            GUIWIN_SetSoftkeyTextId(MMIKL_CLOCK_DISP_WIN_ID, TXT_IDLE_UNLOCK, TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
        }
        break;
        
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYDOWN_HASH:
        if (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus())
        {
            //stop timer
            MMIKL_StopUnlockConfirmTimer();
            MMIKL_SetWinStatus(KL_LOCKED);
            GUIWIN_SetSoftkeyTextId(MMIKL_CLOCK_DISP_WIN_ID, TXT_IDLE_UNLOCK, TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
        }
        
        break;

    //键盘锁下允许拨打紧急拨号
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_0:
        if (!MMIAPICC_OpenEmergencyWin(msg_id))
        {
            if (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus())
            {
                //stop timer
                MMIKL_StopUnlockConfirmTimer();
                MMIKL_SetWinStatus(KL_LOCKED);
                GUIWIN_SetSoftkeyTextId(MMIKL_CLOCK_DISP_WIN_ID, TXT_IDLE_UNLOCK, TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
            }
        }
        break;
        
    case MSG_KEYLONG_RED://锁键盘界面不能关机
        if (MMIKL_IsPreviewState())
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
#endif
    case MSG_LOSE_FOCUS:
        //MAIN_SetStbTimeVisible(TRUE);
        //stop timer
        if (MMIKL_StopUnlockConfirmTimer())
        {
            MMIKL_SetWinStatus(KL_LOCKED);
        }

        GUIWIN_SetSoftkeyTextId(MMIKL_CLOCK_DISP_WIN_ID, TXT_IDLE_UNLOCK, TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
        break;

    case MSG_GET_FOCUS:
        //MAIN_SetStbTimeVisible(FALSE);
#if defined MMI_WALLPAPER_SUPPORT
        #ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
        MMIAPIIDLE_SetInitWallpaper_KL(FALSE, MMIKL_CLOCK_DISP_WIN_ID, MMIKL_MAIN_ANIM_BG_ID,TRUE );
        #else
        MMIAPIIDLE_SetInitWallpaper(FALSE, MMIKL_CLOCK_DISP_WIN_ID, MMIKL_MAIN_ANIM_BG_ID);
        #endif
#endif
        if(MMIKL_GetWinStatus() == KL_UNLOCKED)
        {
            //s_is_press_down = FALSE;
            MMK_CloseWin(win_id);
        }
        break;
      
    case MSG_APP_OK:
  //  case MSG_APP_MENU:
        #if 0
        if ((MMIDEFAULT_IsBacklightOn()) && 
            (MMIKL_GetWinStatus()== KL_LOCKED))
        {
            MMIKL_SetWinStatus(KL_UNLOCKED_ONCE);
            //s_is_press_down = FALSE;
            //start key lock timer
            MMIKL_StartUnlockConfirmTimer(win_id);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)TXT_COMMON_OK, TRUE);
            MMIDEFAULT_TurnOnBackLight();
        }       
        else if (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus())
        {
            //stop timer
            MMIKL_StopUnlockConfirmTimer();
            MMIKL_SetWinStatus(KL_LOCKED);
            GUIWIN_SetSoftkeyTextId(MMIKL_CLOCK_DISP_WIN_ID, TXT_IDLE_UNLOCK, TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);

        }
        else 
        #endif
        if (MMIKL_IsPreviewState())
        {
            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT2);
            MMK_CloseWin(win_id);
        }
        else
        {
             //MMIDEFAULT_TurnOnBackLight();
             recode = MMI_RESULT_FALSE;	
        }
        break;

    case MSG_APP_CANCEL:
        #if 0
        if ((MMIDEFAULT_IsBacklightOn()) && 
            (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus()))
        {
            MMIKL_SetWinStatus(KL_UNLOCKED);
            //检查s_st_mmi_kl_event，判断是否有未读SMS、MMS、MISSED CALL发生
            //如果有，需要调用这3个模块的函数，来弹出各自的提示窗口。
            MMK_CloseWin(win_id);    
            MMIKL_CheckPromptStatus();
        }
        #endif
        if (MMIKL_IsPreviewState())
        {
            MMK_CloseWin(win_id);
        }
		else
		{
			recode = MMI_RESULT_FALSE;		
		}
        break;
#if 0        
    case MSG_TIMER:
        if (MMIKL_GetUnlockTimerID() == *(uint8*)param)
        {
            //stop timer
            MMIKL_StopUnlockConfirmTimer();
            {
                if (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus())
                {
                    MMI_TP_STATUS_E     state = MMI_TP_NONE;
                    uint16              lcd_width = 0;
                    uint16              lcd_height = 0;
                     GUI_POINT_T        point = {0};
                    GUI_RECT_T          tmp_rect = {0};
                    
                    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
                    
                    //取一半的区域 --softkey 
                    tmp_rect.bottom = lcd_height -1;
                    tmp_rect.top = lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
                    tmp_rect.right = lcd_width -1;
                    tmp_rect.left = lcd_width/2;
                     
                    MMK_GetLogicTPMsg(&state, &point);
                    if(((state == MMI_TP_DOWN) || (state == MMI_TP_MOVE)||(state == MMI_TP_LONG))
                        && GUI_PointIsInRect(point,tmp_rect)) //如果此时正touch 在右软键，不做任何处理，重启timer
                    {
                        MMIKL_StartUnlockConfirmTimer(win_id);
                    }
                    else
                    {
                        MMIKL_SetWinStatus(KL_LOCKED);
                        GUIWIN_SetSoftkeyTextId(MMIKL_CLOCK_DISP_WIN_ID, TXT_IDLE_UNLOCK, TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, TRUE);
                        GUIWIN_UpdateSoftkey(win_id);
                    }
                }
            }
        }
        else if(MMIKL_GetTipsTimerID() == *(uint8*)param)
        {
            MMIKL_StopTipsShowTimer();
            if(MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            }
        }
        
        break;
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    //for  directly enter missed call,unread event,unread message

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
       break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_NOTIFY_STATUSBAR_PAINT:
        //s_is_timer_on = MAIN_IsStbDispTime();
        //MAIN_SetStbTimeVisible(FALSE);
        break;

    case MSG_NOTIFY_STATUSBAR_PAINT_END:
        //MAIN_SetStbTimeVisible(s_is_timer_on);
        //s_is_timer_on = FALSE;
        break;

    case MSG_KEY_LOCK_CLOSE_WIN:
        MMK_CloseWin(win_id);
        break;
 #if 0       
    case MSG_CLOSE_WINDOW:  
        //stop timer
        MMIKL_StopTipsShowTimer();
        MMIKL_StopUnlockConfirmTimer();
        
        MMIDEFAULT_TurnOnLCDBackLight();       
    
        MMIKL_SetWinStatus(KL_UNLOCKED);

        MMIDEFAULT_StartAutoKeylockTimer();
        break;
 #endif
    case MSG_KEYLOCK_UNDEAL_EVENT:
        MMK_WinGrabFocus(win_id);
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;

    case MSG_NOTIFY_UPDATE:
        {
            uint16          lcd_width = 0;
            uint16          lcd_height = 0;
            GUI_RECT_T      tp_rect = *(GUI_RECT_T *)param;
             GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
             if((tp_rect.top >= (lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT -1)) &&
                (tp_rect.bottom <(lcd_height)) )
            {
                GUIANIM_UpdateRect(*(GUI_RECT_T *)param,MMIKL_MAIN_ANIM_BG_ID);
            }
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;

    }
    
    return recode;
}

#endif	//MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT

#endif
#endif



/*Edit by script, ignore 2 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
