/*****************************************************************************
** File Name:      mmicom_panel.c                                            *
** Author:                                                                   *
** Date:           2010/7/27                                                 *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/7/27      aoke.hu           Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_common_trc.h"
#include "mmk_timer.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmicom_panel.h"
#include "mmidisplay_data.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_id.h"
#include "guires.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmitheme_pos.h"
#include "guicommon.h"
#include "mmiidle_export.h"
#include "gpio_prod_api.h"
#include "guistatusbar.h"
#include "mmiidle_statusbar.h"
#include "mmimultim_image.h"
#include "mmisrv_nv.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMICOM_PANEL_TP_TIMEOUT 300
#define MMICOM_PANEL_TEXT_FONT MMI_DEFAULT_NORMAL_FONT

#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMICOM_PANEL_NUM_FONT SONG_FONT_8
#else
#define MMICOM_PANEL_NUM_FONT SONG_FONT_12
#endif
#if defined (MAINLCD_SIZE_176X220) 
#define MMICOM_PANEL_PROGRESS_TOP_OFFSET 3
#endif
#if defined(MAINLCD_SIZE_128X160) 
#define MMICOM_PANEL_PROGRESS_TOP_OFFSET  2
#define MMICOM_PANEL_PROGRESS_LEFT_OFFSET 1
#endif
#define MMICOM_PANEL_NUM_LENGTH 3
#define MMICOM_PANEL_TITLE_LENGTH 12
#define MMICOM_PANEL_TOTLA_TITLE_LENGTH 30

#ifndef UI_MULTILAYER_SUPPORT
#define MMICOM_PANEL_BG_ALPHA  160
#endif
/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
typedef enum
{
    MMICOM_PANEL_RECT_NONE,
    MMICOM_PANEL_RECT_DEC,
    MMICOM_PANEL_RECT_INC,
    MMICOM_PANEL_RECT_MAX
}MMICOM_PANEL_RECT_E;

typedef struct  
{
    GUI_RECT_T          panel_rect;
    GUI_RECT_T          text_rect;
    GUI_RECT_T          icon_rect;//vol_img_rect;
    GUI_RECT_T          dec_rect;
    GUI_RECT_T          progress_rect;
    GUI_RECT_T          inc_rect;
    GUI_RECT_T          number_rect;
    uint16              progress_width;
    uint16              progress_offset;
}MMICOM_PANEL_RECT_T;

typedef struct  
{
    MMI_WIN_ID_T          parent_win_id;  //MMK中增加获取父窗口的函数后可以去掉此变量
    MMI_WIN_ID_T          last_focus_child_win_id;
    uint8                 control_panel_timer_id;
    uint8                 panel_tp_timer_id;
    MMICOM_PANEL_RECT_E   rect_type;
    GUI_LCD_DEV_INFO      dev_layer_handle;
    wchar                 title_str[MMICOM_PANEL_TITLE_LENGTH+1];
    MMI_HANDLE_T          cur_win_handle; //解决applet机制下，无法操作panel窗口的问题
}MMICOM_PANEL_T;

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifndef MMI_LOW_MEMORY_RESOURCE
/*****************************************************************************/
//  Description : draw panel
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void DrawControlPanel(void);
#endif
/*****************************************************************************/
//  Description : start control panel timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StartControlPanelTimer(void);

/*****************************************************************************/
//  Description : stop control panel timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StopControlPanelTimer(void);

/*****************************************************************************/
//  Description : stop tp timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StopTPTimer(void);

/*****************************************************************************/
//  Description : start tp timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StartTPTimer(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AdjustValue(BOOLEAN is_increase, BOOLEAN is_refresh);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E KeyMSGDefaultProcess(MMI_MESSAGE_ID_E  msg_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void SetBltLayer(const GUI_LCD_DEV_INFO *lcd_info_ptr);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePanelWindow(
                                      MMI_WIN_ID_T      win_id, 
                                      MMI_MESSAGE_ID_E  msg_id, 
                                      DPARAM       param
                                      );

/*****************************************************************************/
//  Description : switch value
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void SwitchPanelValue(BOOLEAN is_refresh);

/*****************************************************************************/
//  Description : deal tp move message
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPMoveMsg(DPARAM  param);

/*****************************************************************************/
//  Description : deal tp down message
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPDownMsg(DPARAM  param);

#ifndef MMI_LOW_MEMORY_RESOURCE
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetPanelTypeImageID(void);
/*****************************************************************************/
//  Description : init all button rect
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void InitDisplayPosition(void);
#endif
/**--------------------------------------------------------------------------*
**                         WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMICOMMON_PANEL_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePanelWindow),    
    WIN_ID( MMICOMMON_PANEL_WIN_ID ),
//    CREATE_ANIM_CTRL(MMICOMMON_PANEL_ANIM_CTRL_ID,MMICOMMON_PANEL_WIN_ID),
    WIN_STYLE(WS_TOOL_WIN),
    WIN_MOVE_STYLE(MOVE_NOT_FULL_SCREEN_WINDOW),
    END_WIN
};

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL MMICOM_PANEL_INFO_T   s_panel_info = {0};
LOCAL MMICOM_PANEL_RECT_T   s_panel_rect = {0};
LOCAL MMICOM_PANEL_T        s_panel_para = {0};


/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open panel
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 默认居中； 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_OpenPanelChildWin(MMI_WIN_ID_T win_id,
                                           MMICOM_PANEL_INFO_T *panel_info_ptr)
{
    uint32 last_value = s_panel_info.last_value;
    uint16  title_len = 0;

    if(PNULL == panel_info_ptr)
    {
        return FALSE;
    }

    if(MMK_IsOpenWin(s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/))
    {
        //SCI_TRACE_LOW:"[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin Panel is opened!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_PANEL_210_112_2_18_1_59_8_85,(uint8*)"");
        if(s_panel_para.parent_win_id == win_id
            && s_panel_info.panel_type == panel_info_ptr->panel_type)
        {
            StartControlPanelTimer();
            if(MMICOM_PANEL_OPEN_INC == panel_info_ptr->open_type)
            {
                AdjustValue(TRUE,TRUE);
            }
            else if(MMICOM_PANEL_OPEN_DEC == panel_info_ptr->open_type)
            {
                AdjustValue(FALSE,TRUE);
            }
            else if(MMICOM_PANEL_OPEN_VALUE_SWITCH == panel_info_ptr->open_type)
            {
                SwitchPanelValue(TRUE);
            } 
            return TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin open other panel!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_PANEL_231_112_2_18_1_59_9_86,(uint8*)"");
            MMK_CloseWin(s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/);
        }
    }
    //SCI_TRACE_LOW:"[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin cur=%d, min_value = %d, max=%d, type=%d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_PANEL_235_112_2_18_1_59_9_87,(uint8*)"dddd",panel_info_ptr->cur_value,panel_info_ptr->min_value, panel_info_ptr->max_value,panel_info_ptr->panel_type);
    s_panel_para.parent_win_id = win_id;
    SCI_MEMCPY(&s_panel_info,panel_info_ptr,sizeof(MMICOM_PANEL_INFO_T));
    if(0 == s_panel_info.max_value)
    {
        //SCI_TRACE_LOW:"[MMICOMPANEL]:MMIAPICOM_OpenPanelChildWin error max_value==0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_PANEL_240_112_2_18_1_59_9_88,(uint8*)"");
        s_panel_info.max_value = 1;
    }
    s_panel_info.min_value = MIN(s_panel_info.min_value, s_panel_info.max_value);

    if(last_value >0
        && 0 == s_panel_info.last_value
        && last_value <= s_panel_info.max_value
        && last_value >= s_panel_info.min_value)
    {
        s_panel_info.last_value = last_value;
    }
    SCI_MEMSET(&s_panel_para.title_str,0,sizeof(s_panel_para.title_str));
    if(panel_info_ptr->title_str.wstr_len >0 
        && PNULL !=panel_info_ptr->title_str.wstr_ptr)
    {
        title_len = MIN(panel_info_ptr->title_str.wstr_len,MMICOM_PANEL_TITLE_LENGTH);
        MMIAPICOM_Wstrncpy(s_panel_para.title_str,panel_info_ptr->title_str.wstr_ptr,title_len);
    }
    s_panel_para.last_focus_child_win_id = MMK_GetFocusChildWinId();
    /* Modify by michael on 3/12/2012 for  NEWMS00178903: 锁屏下不会出现音量调节界面*/
    if(!MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
    {
#ifndef UI_MULTILAYER_SUPPORT
		if(MMICOM_PLAYER_PANEL_CREATE_WIN == panel_info_ptr->createwin_style)
		{
			MMK_CreateWin((uint32*)MMICOMMON_PANEL_WIN_TAB, panel_info_ptr->createwin_style);
		}
		else
		{
			MMK_CreateChildWin(win_id, (uint32*)MMICOMMON_PANEL_WIN_TAB, PNULL);
		}
#else
		MMK_CreateChildWin(win_id, (uint32*)MMICOMMON_PANEL_WIN_TAB, PNULL);
#endif
        //MMK_SetChildWinFocus(win_id, s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/);
        MMK_PostMsg(s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/, MSG_FULL_PAINT, PNULL, 0);
    }
    /************************Modify over*************************/
    return TRUE;
}

/*****************************************************************************/
//  Description : close panel
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_ClosePanelChildWin(void)
{
    MMK_CloseWin(s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/);
}

/*****************************************************************************/
//  Description : get panel type
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC MMICOM_PANEL_TYPE_E MMIAPICOM_GetPanelType(void)
{
    return s_panel_info.panel_type;
}
/*****************************************************************************/
//  Description : get panel is opened or not
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsPanelOpened(void)
{
    return MMK_IsOpenWin(s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/);  
}

/*****************************************************************************/
//  Description : update panel
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_UpdatePanel(void)
{
    if(MMK_IsOpenWin(s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/))
    {
#ifndef MMI_LOW_MEMORY_RESOURCE
        DrawControlPanel();
#endif
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : switch value
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: value在0与非0间切换。    静音与非静音切换
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_SwitchPanelValue(MMICOM_PANEL_TYPE_E  panel_type)
{
    if(MMIAPICOM_IsPanelOpened()
        && panel_type == s_panel_info.panel_type)
    {
        SwitchPanelValue(TRUE);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : display volume icon
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_DisplayVolIcon( MMI_WIN_ID_T win_id,  
                                     uint8 volume, 
                                     GUIBUTTON_OWNER_DRAW_T    *owner_draw_ptr)
{

    MMI_IMAGE_ID_T     image_id     = IMAGE_NULL;  
    GUI_POINT_T     dis_point = {0};
    // uint16     width  = 0;
    // uint16     height = 0;

    if(PNULL == owner_draw_ptr)
    {
        return;
    }

    if (0 == volume)
    {
#ifndef MMI_RES_LOW_COST        
        image_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE;
#endif    
    }
    else
    {
#ifndef MMI_RES_LOW_COST        
        image_id = IMAGE_COMMON_PLAYER_BTN_VOL;
#endif    
    }

   IMG_EnableTransparentColor(TRUE);    
    //GUI_FillRect(&(owner_draw_ptr->lcd_dev), owner_draw_ptr->display_rect, MMI_TRANSPARENCE_COLOR);
#ifdef UI_MULTILAYER_SUPPORT
   UILAYER_ClearRect(&(owner_draw_ptr->lcd_dev), owner_draw_ptr->display_rect);
#endif
    dis_point.x = owner_draw_ptr->display_x;
    dis_point.y = owner_draw_ptr->display_y;

    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        image_id,
        &(owner_draw_ptr->lcd_dev));
/*
    if (0 < volume && volume <= MMISET_VOL_MAX)
    {
        GUIRES_GetImgWidthHeight(&width,&height,image_id,win_id);

        dis_point.x = (int16)(dis_point.x + width/2);
        dis_point.y = (int16)(dis_point.y + height/2);
        image_id = IMAGE_COMMON_PLAYER_VOL_DIGIT_1 + volume - MMISET_VOL_ONE;  
        
        GUIRES_GetImgWidthHeight(&width,&height,image_id,win_id);        
        dis_point.y = (int16)(dis_point.y - height/2);

        GUIRES_DisplayImg(&dis_point,
                        PNULL,
                        PNULL,
                        win_id,
                        image_id,
                        &(owner_draw_ptr->lcd_dev)
                        );       
    }
*/
    IMG_EnableTransparentColor(FALSE);   
}

/*****************************************************************************/
//  Description : start control panel timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StartControlPanelTimer(void)
{
    if (0 < s_panel_para.control_panel_timer_id)
    {
        MMK_StopTimer(s_panel_para.control_panel_timer_id);
    }
    s_panel_para.control_panel_timer_id = MMK_CreateWinTimer(s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/,MMI_2SECONDS,FALSE);
}

/*****************************************************************************/
//  Description : stop control panel timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StopControlPanelTimer(void)
{
    if (0 < s_panel_para.control_panel_timer_id)
    {
        MMK_StopTimer(s_panel_para.control_panel_timer_id);
        s_panel_para.control_panel_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : start control panel timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StartTPTimer(void)
{
    if (0 < s_panel_para.panel_tp_timer_id)
    {
        MMK_StopTimer(s_panel_para.panel_tp_timer_id);
    }
    s_panel_para.panel_tp_timer_id = MMK_CreateWinTimer(s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/,MMICOM_PANEL_TP_TIMEOUT,FALSE);
}

/*****************************************************************************/
//  Description : stop tp timer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void StopTPTimer(void)
{
    s_panel_para.rect_type = MMICOM_PANEL_RECT_NONE;
    if (0 < s_panel_para.panel_tp_timer_id)
    {
        MMK_StopTimer(s_panel_para.panel_tp_timer_id);
        s_panel_para.panel_tp_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AdjustValue(BOOLEAN is_increase, BOOLEAN is_refresh)
{
    BOOLEAN is_change = FALSE;
    MMICOM_PANEL_CALLBACK_T para = {0};

    if(is_increase)
    {
        if(s_panel_info.cur_value < s_panel_info.max_value)
        {
            s_panel_info.cur_value ++;
            is_change = TRUE;
        }
    }
    else
    {
        if(s_panel_info.cur_value > s_panel_info.min_value)
        {
            s_panel_info.cur_value --;
            if(0 == s_panel_info.cur_value)
            {
                s_panel_info.last_value = 1;
            }
            is_change = TRUE;
        }
    }
    if(PNULL != s_panel_info.set_callback
        && is_change)
    {
        para.cur_value = s_panel_info.cur_value;
        para.last_value = s_panel_info.last_value;
        s_panel_info.set_callback(&para);
        if(is_refresh)
        {
#ifndef MMI_LOW_MEMORY_RESOURCE
            DrawControlPanel();
#endif
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E KeyMSGDefaultProcess(MMI_MESSAGE_ID_E  msg_id)
{
    MMI_RESULT_E    recode = MMI_RESULT_FALSE;
    MMI_MESSAGE_ID_E key_msg = msg_id & 0x00ff;

    if(KEY_RED == key_msg || KEY_HANG == key_msg)
    {
        recode = MMI_RESULT_FALSE;
    }
    else if(msg_id >= KEY_PRESSED
        &&msg_id <= KEY_REPEATED + 0xff)
    {
        recode = MMI_RESULT_TRUE;
    }

    return recode;
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void SetBltLayer(const GUI_LCD_DEV_INFO *lcd_info_ptr)
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    append_layer.lcd_dev_info = *lcd_info_ptr;
    append_layer.layer_level = UILAYER_LEVEL_HIGH;
    UILAYER_RemoveBltLayer(lcd_info_ptr); // 先remove再blt，保证在最上层
    UILAYER_AppendBltLayer(&append_layer);        
}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePanelWindow(
                                      MMI_WIN_ID_T      win_id, 
                                      MMI_MESSAGE_ID_E  msg_id, 
                                      DPARAM       param
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_RECT_T      win_rect = {0, 0, 0, 0};
#ifndef UI_MULTILAYER_SUPPORT	
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
    uint32  creatwin_style = (uint32)MMK_GetWinAddDataPtr(win_id);
#endif    
#ifdef PDA_UI_DROPDOWN_WIN
    static BOOLEAN  stb_state = FALSE;
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
		MMISRV_TestSampleNVItem();
        //记录panel的handle，避免重入
        s_panel_para.cur_win_handle = MMK_GetWinHandle( MMK_GetAppletHandleByWin( win_id ), MMICOMMON_PANEL_WIN_ID );
#ifdef PDA_UI_DROPDOWN_WIN
        stb_state = GUIWIN_GetStbState(s_panel_para.parent_win_id,GUISTATUSBAR_STATE_INVALID);
        GUIWIN_SetStbState(s_panel_para.parent_win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
#ifndef MMI_LOW_MEMORY_RESOURCE
        InitDisplayPosition();
#endif
        StartControlPanelTimer();
        win_rect.left = s_panel_rect.panel_rect.left;
        win_rect.top = s_panel_rect.panel_rect.top;
        win_rect.right = s_panel_rect.panel_rect.right;
        win_rect.bottom = s_panel_rect.panel_rect.bottom;
        MMK_SetWinRect(win_id, &win_rect);
#ifdef UI_MULTILAYER_SUPPORT
        if(UILAYER_IsMultiLayerEnable())
        {
            UILAYER_CREATE_T    create_info = {0};
            
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = win_id;
            create_info.offset_x = win_rect.left;
            create_info.offset_y = win_rect.top;
            create_info.width = (win_rect.right-win_rect.left+1);
            create_info.height = (win_rect.bottom-win_rect.top+1);
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = TRUE; 
            UILAYER_CreateLayer(
                &create_info,
                &s_panel_para.dev_layer_handle
                );
            SetBltLayer(&s_panel_para.dev_layer_handle);
            UILAYER_SetLayerColorKey(&s_panel_para.dev_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
            //UILAYER_SetLayerAlpha(&s_panel_para.dev_layer_handle, 255*2/3);
        }
        else
#endif
        {
            s_panel_para.dev_layer_handle.lcd_id = GUI_MAIN_LCD_ID;
            s_panel_para.dev_layer_handle.block_id = GUI_BLOCK_MAIN;
        }
        if(MMICOM_PANEL_OPEN_INC == s_panel_info.open_type)
        {
            AdjustValue(TRUE,FALSE);
        }
        else if(MMICOM_PANEL_OPEN_DEC == s_panel_info.open_type)
        {
            AdjustValue(FALSE,FALSE);
        }
        else if(MMICOM_PANEL_OPEN_VALUE_SWITCH == s_panel_info.open_type)
        {
            SwitchPanelValue(FALSE);
        }        
        break;
        
    case MSG_FULL_PAINT:
#ifndef UI_MULTILAYER_SUPPORT
		if(MMICOM_PLAYER_PANEL_CREATE_WIN == creatwin_style)
		{
			LCD_FillArgbRect( &lcd_dev_info, 
				full_rect, 
				MMI_BLACK_COLOR,
				MMICOM_PANEL_BG_ALPHA );
		}
#endif
#ifndef MMI_LOW_MEMORY_RESOURCE
		DrawControlPanel();
#endif
        break;

    case MSG_TIMER:
        if(s_panel_para.control_panel_timer_id == *((uint8 *)param))
        {
            s_panel_para.control_panel_timer_id = 0;
            MMK_CloseWin(win_id); 
        }
        else if(s_panel_para.panel_tp_timer_id == *((uint8 *)param))
        {
            if(MMICOM_PANEL_RECT_DEC == s_panel_para.rect_type)
            {
                StartControlPanelTimer();
                StartTPTimer();
                AdjustValue(FALSE,TRUE);
            }
            else if(MMICOM_PANEL_RECT_INC == s_panel_para.rect_type)
            {
                StartControlPanelTimer();
                StartTPTimer();
                AdjustValue(TRUE,TRUE);
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_PRE_LCD_SWITCH:
        MMK_CloseWin(win_id);
        recode = MMI_RESULT_FALSE;
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        HandleTPDownMsg(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        StopTPTimer();
        s_panel_para.rect_type = MMICOM_PANEL_RECT_NONE;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        HandleTPMoveMsg(param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
 
    case MSG_KEYREPEAT_UP:
    case MSG_APP_UP:
 //   case MSG_APP_RIGHT: //NEWMS00185675
        StartControlPanelTimer();
        AdjustValue(TRUE,TRUE);
        break;
        
    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_DOWN:
 //   case MSG_APP_LEFT: //NEWMS00185675
        StartControlPanelTimer();
        AdjustValue(FALSE,TRUE);
        break;

    case MSG_KEYREPEAT_UPSIDE:
    case MSG_APP_UPSIDE:
        if(MMICOM_PANEL_VOLUME == s_panel_info.panel_type)
        {
            StartControlPanelTimer();
            AdjustValue(TRUE,TRUE);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_KEYREPEAT_DOWNSIDE:
    case MSG_APP_DOWNSIDE:
        if(MMICOM_PANEL_VOLUME == s_panel_info.panel_type)
        {
            StartControlPanelTimer();
            AdjustValue(FALSE,TRUE);
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;

    case MSG_LOSE_FOCUS:
//    case MSG_APP_CANCEL:
//    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
    case MSG_APP_CANCEL:
		s_panel_para.control_panel_timer_id = 0;
        MMK_CloseWin(win_id);
        break;
#endif

    case MSG_CLOSE_WINDOW:
        StopControlPanelTimer();
        StopTPTimer();
        //UILAYER_ReleaseLayer(&s_panel_para.dev_layer_handle);
        if(0 != s_panel_para.parent_win_id 
            && MMK_IsFocusWin(s_panel_para.parent_win_id))
        {
            if(0 != s_panel_para.last_focus_child_win_id)
            {
                MMK_SetChildWinFocus(win_id, s_panel_para.last_focus_child_win_id);
                s_panel_para.last_focus_child_win_id = 0;
            }

            //NEWMS00200660,必须发异步消息，否则在retrun to idle时会有问题
            MMK_DelayUpdateScreen();
            // fix NEWMS00195201
            //MMK_UpdateScreen();
            //MMK_PostMsg(s_panel_para.parent_win_id, MSG_FULL_PAINT, PNULL, 0);
        }
#ifdef PDA_UI_DROPDOWN_WIN
        if(!stb_state)
        {
            GUIWIN_SetStbState(s_panel_para.parent_win_id, GUISTATUSBAR_STATE_INVALID, FALSE); 
        }
#endif

        //释放panel的handle，避免重入
        s_panel_para.cur_win_handle = 0;
        break;

    case MSG_KEYDOWN_HEADSET_DETECT:
        MMIAPIIDLE_SetStbEpDisplay(TRUE);
        break;

    case MSG_KEYUP_HEADSET_DETECT:
        MMIAPIIDLE_SetStbEpDisplay(FALSE);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    //recode = recode || KeyMSGDefaultProcess(msg_id);
    
    return recode;
    
}

/*****************************************************************************/
//  Description : switch value
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void SwitchPanelValue(BOOLEAN is_refresh)
{
    StartControlPanelTimer();

    if (0 == s_panel_info.min_value)
    {
        MMICOM_PANEL_CALLBACK_T para = {0};

        if(0 == s_panel_info.cur_value)
        {
            s_panel_info.cur_value = MAX(1,s_panel_info.last_value);
        }
        else
        {
            s_panel_info.last_value = s_panel_info.cur_value;
            s_panel_info.cur_value = 0;
        }
        para.cur_value = s_panel_info.cur_value;
        para.last_value = s_panel_info.last_value;
        s_panel_info.set_callback(&para);
        if(is_refresh)
        {
#ifndef MMI_LOW_MEMORY_RESOURCE
            DrawControlPanel();
#endif
        }
    }
}

/*****************************************************************************/
//  Description : deal tp move message
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPMoveMsg(DPARAM  param)
{
    GUI_POINT_T point = {0};    
    uint32      vol = 0;
    MMICOM_PANEL_CALLBACK_T para={0};


    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if(GUI_PointIsInRect(point,s_panel_rect.progress_rect))
    {
        StartControlPanelTimer();
        if(s_panel_rect.progress_width > 0)
        {
            vol = (uint32)((point.x - s_panel_rect.progress_rect.left) * (s_panel_info.max_value + 1) /s_panel_rect.progress_width);  /*lint !e737*/
        }
        vol = MIN(vol, s_panel_info.max_value);
        vol = MAX(vol, s_panel_info.min_value);
        if(PNULL != s_panel_info.set_callback
        && s_panel_info.cur_value != vol)
        {
            if(0 == vol)
            {
                s_panel_info.last_value = s_panel_info.cur_value;
            }
            s_panel_info.cur_value = vol;
            para.cur_value = s_panel_info.cur_value;
            para.last_value = s_panel_info.last_value;
            s_panel_info.set_callback(&para);
#ifndef MMI_LOW_MEMORY_RESOURCE
            DrawControlPanel();
#endif
        } 
    }
    else if ((MMICOM_PANEL_RECT_DEC == s_panel_para.rect_type && !GUI_PointIsInRect(point,s_panel_rect.dec_rect))
        || (MMICOM_PANEL_RECT_INC == s_panel_para.rect_type && !GUI_PointIsInRect(point,s_panel_rect.inc_rect)))
    {
        StopTPTimer();
        s_panel_para.rect_type = MMICOM_PANEL_RECT_NONE;
    }
}

/*****************************************************************************/
//  Description : deal tp down message
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPDownMsg(DPARAM  param)
{
    GUI_POINT_T point = {0};    
    uint32      vol = 0;
    MMICOM_PANEL_CALLBACK_T para = {0};
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    if(GUI_PointIsInRect(point,s_panel_rect.dec_rect)) 
    {
        StartControlPanelTimer();
        AdjustValue(FALSE,TRUE);
        s_panel_para.rect_type = MMICOM_PANEL_RECT_DEC;
        StartTPTimer();
    }
    else if(GUI_PointIsInRect(point,s_panel_rect.inc_rect)) 
    {
        StartControlPanelTimer();
        AdjustValue(TRUE,TRUE);
        s_panel_para.rect_type = MMICOM_PANEL_RECT_INC;
        StartTPTimer();
    }
    else if(GUI_PointIsInRect(point,s_panel_rect.progress_rect)) 
    {
        StartControlPanelTimer();
        if(s_panel_rect.progress_width > 0)
        {
            vol = (uint32)((point.x - s_panel_rect.progress_rect.left) * (s_panel_info.max_value + 1) /s_panel_rect.progress_width);  /*lint !e737*/
        }
        vol = MIN(vol, s_panel_info.max_value);
        vol = MAX(vol, s_panel_info.min_value);
        if(PNULL != s_panel_info.set_callback
        && s_panel_info.cur_value != vol)
        {
            if(0 == vol)
            {
                s_panel_info.last_value = s_panel_info.cur_value;
            }
            s_panel_info.cur_value = vol;
            para.cur_value = s_panel_info.cur_value;
            para.last_value = s_panel_info.last_value;
            s_panel_info.set_callback(&para);
#ifndef MMI_LOW_MEMORY_RESOURCE
            DrawControlPanel();
#endif
        } 
    }
    else if(GUI_PointIsInRect(point,s_panel_rect.icon_rect))
    {
        SwitchPanelValue(TRUE);
    }
    else if(GUI_PointIsInRect(point,s_panel_rect.panel_rect))
    {
        StartControlPanelTimer();
        SCI_TRACE_LOW("[MMICOMPANEL]:HandleTPDownMsg PANEL RECT");  //点在面板上的无效区域
    }
    else
    {
        //MMK_CloseWin(MMICOMMON_PANEL_WIN_ID);
    }
}
#ifndef MMI_LOW_MEMORY_RESOURCE
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetPanelTypeImageID(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;

    if(MMICOM_PANEL_VOLUME == s_panel_info.panel_type)
    {
        if(0 == s_panel_info.cur_value)
        {
            image_id = IMAGE_COMMON_PLAYER_PANEL_VOL_MUTE;
        }
        else
        {
            image_id = IMAGE_COMMON_PLAYER_PANEL_VOL_UNMUTE;
        }
    }
    else if(MMICOM_PANEL_BRIGHTNESS == s_panel_info.panel_type)
    {
        if(0 == s_panel_info.cur_value)
        {
            image_id = IMAGE_COMMON_PLAYER_PANEL_BRIGHT_GREY;
        }
        else
        {
            image_id = IMAGE_COMMON_PLAYER_PANEL_BRIGHT;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMICOMPANEL]:panel_type error. type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_PANEL_903_112_2_18_1_59_10_89,(uint8*)"d",s_panel_info.panel_type);
        image_id = IMAGE_COMMON_PLAYER_PANEL_VOL_MUTE;
    }

    return image_id;
}

/*****************************************************************************/
//  Description : draw panel
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void DrawControlPanel(void)
{
    MMI_WIN_ID_T win_id = s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/;
    GUI_POINT_T         dis_point = {0};
    GUI_RECT_T          dis_rect = {0};
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL; 
    MMI_IMAGE_ID_T      label_image_id = GetPanelTypeImageID();
    uint32              process = 0;
    GUI_RECT_T          image_rect = {0};
    GUI_RECT_T          dest_rect = {0};
    MMI_STRING_T        lable_str = {0};
    MMI_STRING_T        text_str = {0};
    MMI_STRING_T        num_str = {0};
    wchar               title_str[MMICOM_PANEL_TOTLA_TITLE_LENGTH+1] = {0};
    uint16              title_len = 0;
    uint8               char_num_str[MMICOM_PANEL_NUM_LENGTH] = {0}; 
    wchar               wchar_num_str[MMICOM_PANEL_NUM_LENGTH] = {0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    const GUI_LCD_DEV_INFO*     lcd_dev_info_ptr = &s_panel_para.dev_layer_handle;
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
    wchar               space[2]={0x20,0};

#ifdef UI_MULTILAYER_SUPPORT    
    SetBltLayer(&s_panel_para.dev_layer_handle);

    //GUI_FillRect(lcd_dev_info_ptr, s_panel_rect.panel_rect, MMI_TRANSPARENCE_COLOR);
    UILAYER_ClearRect(lcd_dev_info_ptr, s_panel_rect.panel_rect);
#endif
    IMG_EnableTransparentColor(TRUE); 
//背景
    dis_point.x = s_panel_rect.panel_rect.left;
    dis_point.y = s_panel_rect.panel_rect.top;
    image_id = IMAGE_COMMON_PLAYER_PANEL_BG;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        image_id,
        lcd_dev_info_ptr);
//文字
    if(MMICOM_PANEL_VOLUME == s_panel_info.panel_type)
    {
        MMI_GetLabelTextByLang(TXT_RING_VOLUME, &lable_str);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_BRIGHT_ADJUST, &lable_str);
    }
    title_len = MMIAPICOM_Wstrlen(s_panel_para.title_str);
    if(title_len > 0)
    {
        MMIAPISET_GetLanguageType(&lang_type);
        MMIAPICOM_Wstrncpy(title_str,s_panel_para.title_str,title_len);
        if( MMISET_LANGUAGE_SIMP_CHINESE != lang_type
            && MMISET_LANGUAGE_TRAD_CHINESE != lang_type)
        {
            MMIAPICOM_Wstrncat(title_str,space,1);
            title_len += 1;
        }
        title_len = MIN(MMICOM_PANEL_TOTLA_TITLE_LENGTH-title_len, lable_str.wstr_len);
        MMIAPICOM_Wstrncat(title_str,lable_str.wstr_ptr,title_len);
        text_str.wstr_ptr = title_str;
        text_str.wstr_len = MMIAPICOM_Wstrlen(title_str);
    }
    else
    {
        text_str.wstr_ptr = lable_str.wstr_ptr;
        text_str.wstr_len = lable_str.wstr_len;
    }

    text_style.align = ALIGN_HMIDDLE;
    text_style.font = MMICOM_PANEL_TEXT_FONT;
    text_style.font_color = MMI_WHITE_COLOR;
    GUISTR_DrawTextToLCDInRect( 
        lcd_dev_info_ptr,
        (const GUI_RECT_T *)&s_panel_rect.text_rect,       //the fixed display area
        (const GUI_RECT_T *)&s_panel_rect.text_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T *)&text_str,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
//图标
    if(IMAGE_NULL != label_image_id)
    {
        dis_point.x = s_panel_rect.icon_rect.left;
        dis_point.y = s_panel_rect.icon_rect.top;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            label_image_id,
            lcd_dev_info_ptr);
    }
//减号    
    dis_point.x = s_panel_rect.dec_rect.left;
    dis_point.y = s_panel_rect.dec_rect.top;
    if(s_panel_info.min_value == s_panel_info.cur_value)
    {
        image_id = IMAGE_COMMON_PLAYER_PANEL_VOL_DEC_GREY;
    }
    else
    {
        image_id = IMAGE_COMMON_PLAYER_PANEL_VOL_DEC;
    }
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        image_id,
        lcd_dev_info_ptr);
//进度背景
    dis_rect.left = s_panel_rect.progress_rect.left;
    dis_rect.top = s_panel_rect.progress_rect.top + s_panel_rect.progress_offset;
    dis_rect.right = s_panel_rect.progress_rect.left + s_panel_rect.progress_width - 1;
    dis_rect.bottom = s_panel_rect.progress_rect.bottom; 
    image_id = IMAGE_COMMON_PLAYER_PANEL_PROGRESS_BG;
    GUIRES_DisplayImg(PNULL,
        &dis_rect,
        PNULL,
        win_id,
        image_id,
        lcd_dev_info_ptr);
//进度
    if(s_panel_info.max_value > 0)
    {
        process = s_panel_info.cur_value * s_panel_rect.progress_width / s_panel_info.max_value;
    }
    if(process > 0)
    {       
        /** show process **/
        image_rect.left = 0;
        image_rect.right = (int16)process;         
        image_rect.top = 0;
        image_rect.bottom = s_panel_rect.progress_rect.bottom - s_panel_rect.progress_rect.top;
        
        dest_rect.left = s_panel_rect.progress_rect.left;
        dest_rect.right = (int16)(s_panel_rect.progress_rect.left + process - 1); /*lint !e737*/
        dest_rect.top =  s_panel_rect.progress_rect.top + s_panel_rect.progress_offset;
        dest_rect.bottom = s_panel_rect.progress_rect.bottom; 
        GUIRES_DisplayImg(PNULL, &dest_rect, &image_rect, win_id, IMAGE_COMMON_PLAYER_PANEL_PROGRESS, lcd_dev_info_ptr);
        
    }
//加号    
    dis_point.x = s_panel_rect.inc_rect.left;
    dis_point.y = s_panel_rect.inc_rect.top;
    if(s_panel_info.cur_value == s_panel_info.max_value)
    {
        image_id = IMAGE_COMMON_PLAYER_PANEL_VOL_INC_GREY;
    }
    else
    {
        image_id = IMAGE_COMMON_PLAYER_PANEL_VOL_INC;
    }
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        image_id,
        lcd_dev_info_ptr);
//音量数字
    num_str.wstr_len =  sprintf((char*)char_num_str, "%2d", s_panel_info.cur_value);
    MMIAPICOM_StrToWstr(char_num_str,wchar_num_str);
    num_str.wstr_ptr = wchar_num_str;
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = MMICOM_PANEL_NUM_FONT;
    text_style.font_color = MMI_BACKGROUND_COLOR;
    GUISTR_DrawTextToLCDInRect( 
        lcd_dev_info_ptr,
        (const GUI_RECT_T *)&s_panel_rect.number_rect,       //the fixed display area
        (const GUI_RECT_T *)&s_panel_rect.number_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T *)&num_str,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    IMG_EnableTransparentColor(FALSE);
}

/*****************************************************************************/
//  Description : init all button rect
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void InitDisplayPosition(void)
{
    uint16              width = 0;
    uint16              width2 = 0;
    uint16              width3 = 0;
    uint16              height = 0;
    uint16              height2 = 0;
    uint16              height3 = 0;
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    uint16              offset = 0;
    MMI_WIN_ID_T        win_id = s_panel_para.cur_win_handle/*MMICOMMON_PANEL_WIN_ID*/;
    
    
    //GUIRES_GetImgWidthHeight(&width2,&height2,IMAGE_COMMON_PLAYER_PANEL_CLOSE,win_id);
    //GUIRES_GetImgWidthHeight(&width3,&height3,IMAGE_COMMON_PLAYER_PANEL_VOL_UNMUTE,win_id);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
//背景
    GUIRES_GetImgWidthHeight(&width,&height,IMAGE_COMMON_PLAYER_PANEL_BG,win_id);
    if(s_panel_info.x < 0)
    {
        s_panel_rect.panel_rect.left = (int16)(MAX((int16)(lcd_width-width)/2,0));
    }
    else
    {
        s_panel_rect.panel_rect.left = s_panel_info.x;
    }
    if(s_panel_info.y < 0)
    {
        s_panel_rect.panel_rect.top = (int16)MAX((int16)(lcd_height-height)/2,0);
    }
    else
    {
        s_panel_rect.panel_rect.top = s_panel_info.y;
    }  
    s_panel_rect.panel_rect.right = (int16)(s_panel_rect.panel_rect.left + width- 1);
    s_panel_rect.panel_rect.bottom = (int16)(s_panel_rect.panel_rect.top + height - 1);
//文字
    s_panel_rect.text_rect.left = s_panel_rect.panel_rect.left;
    s_panel_rect.text_rect.top = s_panel_rect.panel_rect.top + 3;
    s_panel_rect.text_rect.right = s_panel_rect.panel_rect.right;
#if defined(MAINLCD_SIZE_128X160)
    s_panel_rect.text_rect.bottom = s_panel_rect.text_rect.top + 5; //小屏手机没有音量文字
#else
    s_panel_rect.text_rect.bottom = s_panel_rect.text_rect.top + MMICOM_PANEL_TEXT_FONT;
#endif
//图标
    GUIRES_GetImgWidthHeight(&width2,&height2,IMAGE_COMMON_PLAYER_PANEL_VOL_UNMUTE,win_id);
    s_panel_rect.icon_rect.left = s_panel_rect.panel_rect.left + (int16)(MAX((int16)(width-width2)/2,0));
    s_panel_rect.icon_rect.top = s_panel_rect.text_rect.bottom + 2;
    s_panel_rect.icon_rect.right = s_panel_rect.icon_rect.left + width2;
    s_panel_rect.icon_rect.bottom = s_panel_rect.icon_rect.top + height2;
//减号
    GUIRES_GetImgWidthHeight(&width2,&height2,IMAGE_COMMON_PLAYER_PANEL_VOL_DEC,win_id);
    GUIRES_GetImgWidthHeight(&width3,&height3,IMAGE_COMMON_PLAYER_PANEL_PROGRESS_BG,win_id);
    width3 = MMICOM_PLAYER_PANEL_PROGRESS_WIDTH;
    /*modified by langhua.yang for CR:NEWMS00211579*/
	if(0 == (uint16)MAX((int16)(width - width2*2 - width3)/4,0))
	{
       offset = (uint16)MAX((int16)(width - width2*2 - width3)/2,0);
	}
	else
	{
       offset = (uint16)MAX((int16)(width - width2*2 - width3)/4,0);
	}
    s_panel_rect.dec_rect.left = s_panel_rect.panel_rect.left + offset;
    s_panel_rect.dec_rect.top = s_panel_rect.icon_rect.bottom -2 /*+ 2*/;
    s_panel_rect.dec_rect.right = s_panel_rect.dec_rect.left + width2 - 1;
    s_panel_rect.dec_rect.bottom = s_panel_rect.dec_rect.top + height2 -1;
//进度
    s_panel_rect.progress_width = width3;
    s_panel_rect.progress_offset = height3/4;
    s_panel_rect.progress_rect.left = s_panel_rect.dec_rect.right + offset;
    s_panel_rect.progress_rect.top = s_panel_rect.dec_rect.top + height2/2 - height3/2 -1;
    s_panel_rect.progress_rect.right = s_panel_rect.progress_rect.left + width3 - 1;
    s_panel_rect.progress_rect.bottom = s_panel_rect.progress_rect.top + s_panel_rect.progress_offset + height3 -1;
//加号
    s_panel_rect.inc_rect.left = s_panel_rect.progress_rect.right + offset;
    s_panel_rect.inc_rect.top = s_panel_rect.dec_rect.top;
    s_panel_rect.inc_rect.right = s_panel_rect.inc_rect.left + width2 - 1;
    s_panel_rect.inc_rect.bottom = s_panel_rect.inc_rect.top + height2 -1;
//音量数字
    s_panel_rect.number_rect.left = s_panel_rect.progress_rect.left;
#if !defined(MAINLCD_SIZE_128X160) && !defined(MAINLCD_SIZE_176X220)
    s_panel_rect.number_rect.top = s_panel_rect.progress_rect.top + (int16)(MAX(((int16)(height3-GUI_GetFontHeight(MMICOM_PANEL_NUM_FONT,UNICODE_ASCII))/2),0));        /*lint !e666*/
    s_panel_rect.number_rect.bottom = s_panel_rect.number_rect.top + GUI_GetFontHeight(MMICOM_PANEL_NUM_FONT,UNICODE_ASCII);
#else
    s_panel_rect.number_rect.top = s_panel_rect.progress_rect.bottom + 2;// s_panel_rect.progress_offset + (int16)(MAX((int16)(height3/2-MMICOM_PANEL_NUM_FONT)/2,0));
    s_panel_rect.number_rect.bottom = s_panel_rect.number_rect.top + GUI_GetFontHeight(MMICOM_PANEL_NUM_FONT,UNICODE_ASCII) + 2;//临时解决方案，de 获取字体高度有问题
#endif
    s_panel_rect.number_rect.right = s_panel_rect.progress_rect.right;
    
}

#endif
/*Edit by script, ignore 3 case. Thu Apr 26 19:01:12 2012*/ //IGNORE9527
