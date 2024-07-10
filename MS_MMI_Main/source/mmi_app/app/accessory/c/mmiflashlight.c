/****************************************************************************
** File Name:      mmiflashlight.c                            
** Author:                                                                   
** Date:            
** Copyright:     
** Description:    
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 
****************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_accessory_trc.h"
#ifdef WIN32
#include "std_header.h" 
#endif
#include "window_parse.h"
#include "mmk_timer.h"

#include "mmi_default.h"
#include "mmiacc_id.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "mmiacc_position.h"
#include "guires.h"
#include "mmikl_export.h"
#include "guistring.h"
#include "mmi_theme.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "mmi_textfun.h"
#include "mmiflashlight_export.h"
#ifndef WIN32
#include "gpio_prod_api.h"
#endif
#if defined(TORCH_SUPPORT)

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                           */
/*---------------------------------------------------------------------------*/
#if 0//wuxx del it 20231031
LOCAL const MMI_IMAGE_ID_T THEME_FLASHLIGHT_BACK_ICON [MMITHEME_ACCENT_COLOR_MAX] =
{
        CUSTOM_EXTRAS_FLASHLIGHT_BACK_VIOLET,
        CUSTOM_EXTRAS_FLASHLIGHT_BACK_RED,
        CUSTOM_EXTRAS_FLASHLIGHT_BACK_YELLOW,
        CUSTOM_EXTRAS_FLASHLIGHT_BACK_GREEN,
        CUSTOM_EXTRAS_FLASHLIGHT_BACK_COPPER,
        CUSTOM_EXTRAS_FLASHLIGHT_BACK_BLUE
};

LOCAL const MMI_IMAGE_ID_T THEME_FLASHLIGHT_LSK_ICON [MMITHEME_ACCENT_COLOR_MAX] =
{
        CUSTOM_EXTRAS_FLASHLIGHT_LSK_VIOLET,
        CUSTOM_EXTRAS_FLASHLIGHT_LSK_RED,
        CUSTOM_EXTRAS_FLASHLIGHT_LSK_YELLOW,
        CUSTOM_EXTRAS_FLASHLIGHT_LSK_GREEN,
        CUSTOM_EXTRAS_FLASHLIGHT_LSK_COPPER,
        CUSTOM_EXTRAS_FLASHLIGHT_LSK_BLUE
};

LOCAL const MMI_IMAGE_ID_T THEME_FLASHLIGHT_ON_ICON [MMITHEME_ACCENT_COLOR_MAX] =
{
        CUSTOM_EXTRAS_FLASHLIGHT_ON_VIOLET,
        CUSTOM_EXTRAS_FLASHLIGHT_ON_RED,
        CUSTOM_EXTRAS_FLASHLIGHT_ON_YELLOW,
        CUSTOM_EXTRAS_FLASHLIGHT_ON_GREEN,
        CUSTOM_EXTRAS_FLASHLIGHT_ON_COPPER,
        CUSTOM_EXTRAS_FLASHLIGHT_ON_BLUE
};
#endif


#define MMIACC_TORCH_TIMER_PERIOD            (5*60*1000)       //同步联系人请求响应过时保护时长
LOCAL uint8 s_mmiacc_torch_timeout_timer = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFlashlightWinMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );



/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_mmiacc_screen_flashlight_on = FALSE;
#ifdef WIN32
LOCAL BOOLEAN s_mmiacc_torch_on = FALSE; 
#endif

//空函数, 不需要状态条,只编译通过, wuxx add
PUBLIC void MMIAPICOM_StatusAreaSetTorchState(BOOLEAN is_torch_on)
{

}

WINDOW_TABLE( MMIACC_FLASHLIGHT_WIN_TAB ) = 
{
  //  WIN_TITLE(STR_FLASHL_HDR_TORCH),
    WIN_FUNC((uint32) HandleFlashlightWinMsg ),    
    WIN_ID( MMIACC_FLASHLIGHT_WIN_ID ),
 //   WIN_SOFTKEY(TXT_NULL, TXT_COMM_ON, TXT_COMMON_COMMON_BACK),
 WIN_HIDE_STATUS,
    END_WIN
};

/*===============================================================================
IMPLEMENTATION
===============================================================================*/

/*****************************************************************************/
//  Description : get torch status
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_GetTorchStatus(void)
{
#ifndef WIN32
    return GPIO_GetTorchStatus();
#else
    return s_mmiacc_torch_on;
#endif
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:   
/*****************************************************************************/
LOCAL void FlashlightWinPaint(MMI_WIN_ID_T win_id)
{	
    GUI_RECT_T mainlcd_rect  = MMITHEME_GetFullScreenRect();
    GUI_RECT_T client_rect =MMITHEME_GetClientRect();// MMITHEME_GetFullScreenRect();//MMITHEME_GetClientRect();
    GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_POINT_T             point                = {0};
	GUI_POINT_T				start_point			 = {0,0};
    uint16 image_width = 0, image_height =0;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    MMI_IMAGE_ID_T image_back_id = IMAGE_NULL;
    MMI_IMAGE_ID_T image_lsk_id = IMAGE_NULL;
    MMI_IMAGE_ID_T image_on_id = IMAGE_NULL;
    ////MMITHEME_ACCENT_COLOR_E theme_index = MMITHEME_GetAccentId();// wuxx del it .no MMITHEME_GetAccentId
    
    //client_rect.top -= 30;
#if 0//wuxx del it 20231031
    image_back_id = THEME_FLASHLIGHT_BACK_ICON[theme_index];
    image_lsk_id = THEME_FLASHLIGHT_LSK_ICON[theme_index];
    image_on_id = THEME_FLASHLIGHT_ON_ICON[theme_index];
#endif
    
    #if 0
    if(1)
    {
        GUIWIN_SetSoftkeyStyle(win_id,GUI_RGB2RGB565(0xde, 0x94, 0xe0),GUI_SOFTKEY_BG_COLOR_ONLY,MMI_WHITE_COLOR,0);

        LCD_FillRect(&lcd_dev_info, client_rect, MMI_WHITE_COLOR);
        //GUIWIN_SeSoftkeytButtonIconId(win_id, image_lsk_id, 0, TRUE);
        GUIWIN_SeSoftkeytButtonIconId(win_id, image_back_id, 2, TRUE);
    }
    else
    {
        GUIWIN_SetSoftkeyStyle(win_id,MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT),GUI_SOFTKEY_BG_COLOR_ONLY,GUI_RGB2RGB565(0x36, 0xb4, 0x36),0);
        
        LCD_FillRect(&lcd_dev_info, client_rect, MMI_BLACK_COLOR);
        //GUIWIN_SeSoftkeytButtonIconId(win_id, CUSTOM_EXTRAS_FLASHLIGHT_LSK_RED, 0, TRUE);
        //GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_BACK, 2, TRUE);
    }
	#endif

#ifndef WIN32
    if(GPIO_GetTorchStatus())
    {
    //    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMM_OFF, 1, TRUE);
        image_id = IMAG_FLASHLIGHT_ON; //image_on_id;
    }
    else
    {
     //   GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMM_ON, 1, TRUE);
        image_id = IMAG_FLASHLIGHT_OFF; //CUSTOM_EXTRAS_FLASHLIGHT_OFF;
    }
#else
    if(s_mmiacc_torch_on)
    {
    //    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMM_OFF, 1, TRUE);
        image_id = IMAG_FLASHLIGHT_ON; //image_on_id;
    }
    else
    {
    //    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMM_ON, 1, TRUE);
        image_id = IMAG_FLASHLIGHT_OFF; //CUSTOM_EXTRAS_FLASHLIGHT_OFF;
		
    }
#endif

#if 0
    GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id, win_id);
    point.x = (client_rect.right - client_rect.left)/2 - image_width/2;
    point.y = (client_rect.bottom - client_rect.top)/2 - image_height/2 + client_rect.top;
    
    GUIRES_DisplayImg(&point,
        PNULL ,
        PNULL,
        win_id,
        image_id,
        &lcd_dev_info);
#endif		

		GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_id,image_id,&lcd_dev_info);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:   
/*****************************************************************************/
static int s_torch_tp_x;
static int s_torch_tp_y;
static BOOLEAN s_is_torch_tp_down = FALSE;

LOCAL MMI_RESULT_E HandleFlashlightWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{   
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_mmiacc_screen_flashlight_on = FALSE;
#ifdef WIN32
        s_mmiacc_torch_on = FALSE;
#endif
        //if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_FLASHL_HDR_TORCH))
        {
            GUIWIN_UpdateStb();
        }
        break;
    case MSG_GET_FOCUS:
        //if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_FLASHL_HDR_TORCH))
        {
            GUIWIN_UpdateStb();
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            s_torch_tp_x = point.x;
            s_torch_tp_y = point.y;
            s_is_torch_tp_down = TRUE;
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            int offset_y =  0;//up to down
            int offset_x =  0;//up to down
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(s_is_torch_tp_down)
            {
                offset_y =  point.y - s_torch_tp_y;
                offset_x =  point.x - s_torch_tp_x;
                if(offset_x <= -(20))	
                {
                    MMK_CloseWin(win_id);
                }	
                else if(offset_x >= (20))
                {
                    MMK_CloseWin(win_id);
                }
                else
                {
                    MMIACC_SwtichTorch(MMIACC_FLASHLIGHT_MODE_SWITCH);
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }

            }
            s_is_torch_tp_down = FALSE;
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
        
    case MSG_FULL_PAINT:
        FlashlightWinPaint(win_id);
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
        if(s_mmiacc_screen_flashlight_on)
        {
            s_mmiacc_screen_flashlight_on = FALSE;
        }
        else
        {
            s_mmiacc_screen_flashlight_on = TRUE;
        }
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMIACC_SwtichTorch(MMIACC_FLASHLIGHT_MODE_SWITCH);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

 /*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIACC_OpenFlashlighWin(void)
{
    MMK_CreateWin((uint32 *)MMIACC_FLASHLIGHT_WIN_TAB, PNULL);
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: cheney.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIACC_SwtichTorch(MMIACC_FLASHLIGHT_MODE_E mode)
{
    switch (mode)
    {
    case MMIACC_FLASHLIGHT_MODE_ON:
#ifndef WIN32
        GPIO_SetTorch(TRUE);
        MMIACC_CreateTorchTimer();
#else
        s_mmiacc_torch_on = TRUE;
        MMIACC_CreateTorchTimer();
#endif
        MMIAPICOM_StatusAreaSetTorchState(TRUE);
        break;

    case MMIACC_FLASHLIGHT_MODE_OFF:

#ifndef WIN32
        MMIACC_StopTorchTimer();
        GPIO_SetTorch(FALSE);
#else
        MMIACC_StopTorchTimer();
        s_mmiacc_torch_on = FALSE;
#endif
        MMIAPICOM_StatusAreaSetTorchState(FALSE);
        break;

    case MMIACC_FLASHLIGHT_MODE_SWITCH:
    default:
#ifndef WIN32
        if(GPIO_GetTorchStatus())
        {
            MMIACC_StopTorchTimer();
            GPIO_SetTorch(FALSE);
            MMIAPICOM_StatusAreaSetTorchState(FALSE);
        }
        else
        {
            GPIO_SetTorch(TRUE);
            MMIAPICOM_StatusAreaSetTorchState(TRUE);
            MMIACC_CreateTorchTimer();
        }
#else
        if(s_mmiacc_torch_on)
        {
            MMIACC_StopTorchTimer();
            s_mmiacc_torch_on = FALSE;
            MMIAPICOM_StatusAreaSetTorchState(FALSE);
        }
        else
        {
            s_mmiacc_torch_on = TRUE;
            MMIAPICOM_StatusAreaSetTorchState(TRUE);
            MMIACC_CreateTorchTimer();
        }
#endif
        break;
    }

}
/*****************************************************************************/
//  Description : MMIACC_CreateTorchTimer
//  Global resource dependence : 
//  Author: cheney.wu
//  Note: 创建超时定时器
/*****************************************************************************/
PUBLIC void MMIACC_CreateTorchTimer(void)
{
#if MMI_TORCH_AUTO_OFF_TIMER_SUPPORT
    SCI_TRACE_LOW("[MMIACC] MMIACC_CreateTorchTimer enter");
    
    if(s_mmiacc_torch_timeout_timer != 0) 
    {
        MMIACC_StopTorchTimer();
    }
    
    s_mmiacc_torch_timeout_timer = MMK_CreateTimerCallback(MMIACC_TORCH_TIMER_PERIOD, MMIACC_ProcessTorchTimerOver, 0, FALSE);
    
    SCI_TRACE_LOW("[MMIBT] MMIACC_CreateTorchTimer exit, timer_id = %d", s_mmiacc_torch_timeout_timer);
#endif
}

/******************************************************************************/
// Description: MMIACC_StopTorchTimer
// Global resource dependence: 
// Author: cheney.wu
// Note: 停止超时定时器
/******************************************************************************/
PUBLIC void MMIACC_StopTorchTimer(void)
{
#if MMI_TORCH_AUTO_OFF_TIMER_SUPPORT
    SCI_TRACE_LOW("[MMIBT] MMIACC_StopTorchTimer enter");
    
    if(s_mmiacc_torch_timeout_timer != 0)    
    {   
        MMK_StopTimer(s_mmiacc_torch_timeout_timer);   
        s_mmiacc_torch_timeout_timer = 0;    
    }   
    
    SCI_TRACE_LOW("[MMIBT] MMIACC_StopTorchTimer exit, timer_id = %d", s_mmiacc_torch_timeout_timer);
#endif
}

/******************************************************************************/
// Description: MMIACC_ProcessTorchTimerOver
// Global resource dependence: 
// Author: cheney.wu
// Note: 超时保护机制
/******************************************************************************/
PUBLIC void MMIACC_ProcessTorchTimerOver(uint8 timer_id, uint32 param)
{
    SCI_TRACE_LOW("[MMIACC] MMIACC_ProcessTorchTimerOver enter");

    MMIACC_SwtichTorch(MMIACC_FLASHLIGHT_MODE_OFF);
    
    if(MMK_IsOpenWin(MMIACC_FLASHLIGHT_WIN_ID))
    {
        MMK_SendMsg(MMIACC_FLASHLIGHT_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}
#endif
