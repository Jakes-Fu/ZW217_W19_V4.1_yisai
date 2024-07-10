/*****************************************************************************
** File Name:      coordinate_wintab.c                                       *
** Author:         guopeng.zhang                                             *
** Date:           20/06/2005                                                *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to test coordinate                      *
** 03/03/2006     Pengyu.li        Must not save wrong coordinate value into NV*
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2005        guopeng.zhang    create                                    *
** 12/15/2005     David.Jia        Reimplement COORDINATE_IsVerifySuccess.   *
** 1/5/2010       Vine.Yuan        Implement TP self-adaptive feature
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_tp_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#if defined TOUCH_PANEL_SUPPORT
#include "tp_text.h"
#include "tp_api.h"
//#include "dal_lcd_sc6800.h"
#include "dal_lcd.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guilabel.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
//#include "coordinate_wintab.h"
#include "tp_export.h"
#include "tp_id.h"
#include "tp_nv.h"
#include "tp_internal.h"
#include "mmi_nv.h"
#include "mmipub.h"
#include "mmiphone_export.h"
#include "tp_internal.h"
#include "guistring.h"
#include "mmk_tp.h"
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
// 浮点数定义
typedef uint32 FLOAT32;

#ifdef TP_SELFADAPTIVE_ENABLE
//@vine.yuan 2010.1.5
typedef BOOLEAN (*TP_CALI_FUN_T)(void);
#endif //TP_SELFADAPTIVE_ENABLE
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// 十字线段长度 
#define COORDINATE_HALF_LINE_LENGTH  5
#define COORDINATE_LINE_LENGTH       (2 * COORDINATE_HALF_LINE_LENGTH)

// 浮点数精度
#define FLOAT32_PRECISION 1000

//#define     TP_HEIGHT_LCD           220     //lcd max 

#define     OFFSET_X                10
#define     OFFSET_Y                10
#define     ADC_OFFSET_X            80
#define     ADC_OFFSET_Y            100
//@David.Jia    2005.12.15  end

//#define     CALIBRATION_FLAG           0x5555aaaa

#define COORDINATE_DEBUG
#ifdef COORDINATE_DEBUG
    #define COORDINATE_TP SCI_TRACE_LOW
#else
    #define COORDINATE_TP   
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
// 校正点位置
// 0: centre
// 1: left top
// 2: right bottom
LOCAL uint8 s_click_times = 0;
LOCAL uint8 s_click_point = 0;

// 用户点击的坐标值    
LOCAL uint16 s_right_top_adc_x = 0;
LOCAL uint16 s_right_top_adc_y = 0;
LOCAL uint16 s_mid_adc_x          = 0;
LOCAL uint16 s_mid_adc_y          = 0;
LOCAL uint16 s_left_bottom_adc_x = 0;
LOCAL uint16 s_left_bottom_adc_y = 0;
//LOCAL uint32 s_factor = 0;


LOCAL TP_CALIBRATION_T s_cal_default;
LOCAL TP_CALIBRATION_T s_cal;
LOCAL uint32           s_cross_half_height=0;
LOCAL uint32           s_cross_height =0;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

#ifdef WIN32
TP_INFO_T   s_tp_info = {0};
PUBLIC TP_INFO_T* TP_GetInfo(void)
{
    // TP_INFO_T   tp_info = {0};

    s_tp_info.tp_width        = 240;  
    s_tp_info.tp_height       = 320;
    s_tp_info.tp_lcd_height   = 320;
                           
    s_tp_info.adc_min_x       = 75; 
    s_tp_info.adc_min_y       = 735; 
    s_tp_info.adc_max_x       = 60; 
    s_tp_info.adc_max_y       = 760;    
    
    s_tp_info.exchange_x      = 0;
    s_tp_info.exchange_y      = 0;

    return &s_tp_info;
}
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : PROCESS COORDINATE MAIN WINDOW MESSAGE
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E COORDINATE_HandleMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

/*****************************************************************************/
//  Description : DRAW THE POINT FOR CLICK     
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
LOCAL void COORDINATE_ShowPoint(uint8 pos);

/*****************************************************************************/
//  Description : judge the point is valid    
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN COORDINATE_IsVerifySuccess(void);

/*****************************************************************************/
//  Description : float number div
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
FLOAT32 float32_div(uint32 num1, uint32 num2);

/*****************************************************************************/
//  Description : display warning infomation
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayWarningInfo(void);

/*****************************************************************************/
//  Description : handle tp Verify fail prompt win
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpVerifyFailPromptWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             );

#ifdef WIN32
/*****************************************************************************/
//  FUNCTION:     TP_SetRawDataMode
//  Description:   
//  return:  
//  Author:         
//  date:  2005.05.11
//  Note:        
/*****************************************************************************/
LOCAL void TP_SetRawDataMode(BOOLEAN act_res);
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
// 提示语label
#define COORDIANTE_PROMPT_LABEL_LEFT   5
#define COORDIANTE_PROMPT_LABEL_TOP    0
//#define COORDIANTE_PROMPT_LABEL_RIGHT  (MMI_MAINSCREEN_WIDTH-COORDIANTE_PROMPT_LABEL_LEFT)
//#define COORDIANTE_PROMPT_LABEL_BOTTOM (MMI_MAINSCREEN_HEIGHT/2 - 20)

// Main Window
WINDOW_TABLE(COORDINATE_WIN_TAB) =
{
    WIN_FUNC( (uint32)COORDINATE_HandleMsg ),
    WIN_ID(TP_COORDINATE_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

/*****************************************************************************/
//  Description : display warning infomation
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayWarningInfo(void)
{
    GUI_RECT_T          rect = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    MMI_STRING_T string = { 0};
    MMI_STRING_T disp_str = {0}; 
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK; 
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;    

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    rect.left = COORDIANTE_PROMPT_LABEL_LEFT+10;
    rect.top = COORDIANTE_PROMPT_LABEL_TOP;
    rect.right = lcd_width - COORDIANTE_PROMPT_LABEL_LEFT;
    rect.bottom = lcd_height/2 -20;
    MMI_GetLabelTextByLang(TXT_COORDINATE_PROMPT, &string);
    
    disp_str.wstr_ptr = SCI_ALLOC_APP(sizeof(wchar)*(string.wstr_len+1));
    SCI_MEMSET(disp_str.wstr_ptr,0x00,(sizeof(wchar)*(string.wstr_len+1)));
    MMIAPICOM_Wstrncpy(disp_str.wstr_ptr,string.wstr_ptr,string.wstr_len);
    disp_str.wstr_len = string.wstr_len;
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = MMI_DEFAULT_NORMAL_FONT;
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.char_space = 1;
    text_style.line_space = 3;
    
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&disp_str,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    SCI_FREE(disp_str.wstr_ptr);
}

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : PROCESS COORDINATE MAIN WINDOW MESSAGE
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E COORDINATE_HandleMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_RECT_T          rect = {0};
    BOOLEAN             *add_data_ptr = PNULL;
    LCD_ANGLE_E         rotate_angle = LCD_ANGLE_0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    //SCI_TRACE_LOW:"MMI:COORDINATE_HandleMsg(0x%x,0x%x)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_261_112_2_18_3_3_30_0,(uint8*)"dd", win_id, msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetCoordinateTpFlag(TRUE);
            s_click_times = 0;
            s_click_point = 0;
            break;

        case MSG_FULL_PAINT:
            // draw white color backgroud
            rect = MMITHEME_GetFullScreenRect();

            LCD_FillRect(&lcd_dev_info, rect, 0xFFFF);
            DisplayWarningInfo();
             
            // draw point
            COORDINATE_ShowPoint(s_click_point);
            break;

        case MSG_APP_OK:
      
            // 打开选项界面
            s_click_times = 0;
            s_click_point = 0;
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            break;
        case MSG_LCD_SWITCH://restart check
            // 打开选项界面
            s_click_times = 0;
            s_click_point = 0;
            break;
#if defined KEYPAD_TYPE_TYPICAL_KEYPAD || defined KEYPAD_TYPE_QWERTY_KEYPAD
        case MSG_APP_CANCEL:
            s_click_times = 0;
            MMK_CloseWin(win_id);
            break;
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DOWN:
            // 获得坐标值
            switch (s_click_times)
            {
                case 0:
                    //COORDINATE_TP:"COORDINATE.C param = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_304_112_2_18_3_3_30_1,(uint8*)"d", *(uint32 *)param);
                    s_mid_adc_x = MMK_GET_TP_X(param);
    
                    s_mid_adc_y = MMK_GET_TP_Y(param);

                    //COORDINATE_TP:"COORDINATE.C mid : x = %d, y = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_309_112_2_18_3_3_30_2,(uint8*)"dd", s_mid_adc_x, s_mid_adc_y);
                    //get rotate angle
                    rotate_angle = GUILCD_GetRotateAngle(lcd_dev_info.lcd_id);
                    switch (rotate_angle)
                    {
                    case LCD_ANGLE_0:
                        s_click_point = 2;
                        break;

                    case LCD_ANGLE_90:
                        s_click_point = 1;
                        break;

                    case LCD_ANGLE_180:
                        s_click_point = 3;
                        break;

                    case LCD_ANGLE_270:
                        s_click_point = 4;
                        break;

                    default:
                        break;
                    }
                    s_click_times++;
                    break;
                    
                 case 1:
                    //COORDINATE_TP:"COORDINATE.C param = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_314_112_2_18_3_3_30_3,(uint8*)"d", *(uint32 *)param);
                    s_right_top_adc_x = MMK_GET_TP_X(param);
  
                    s_right_top_adc_y = MMK_GET_TP_Y(param);

                    //COORDINATE_TP:"COORDINATE.C right top: x = %d, y = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_319_112_2_18_3_3_30_4,(uint8*)"dd", s_right_top_adc_x, s_right_top_adc_y);
                    //get rotate angle
                    rotate_angle = GUILCD_GetRotateAngle(lcd_dev_info.lcd_id);
                    switch (rotate_angle)
                    {
                    case LCD_ANGLE_0:
                        s_click_point = 3;
                        break;

                    case LCD_ANGLE_90:
                        s_click_point = 4;
                        break;

                    case LCD_ANGLE_180:
                        s_click_point = 2;
                        break;

                    case LCD_ANGLE_270:
                        s_click_point = 1;
                        break;

                    default:
                        break;
                    }
                    s_click_times++;
                    break;
             
                 case 2:
                    //COORDINATE_TP:"COORDINATE.C param = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_324_112_2_18_3_3_30_5,(uint8*)"d", *(uint32 *)param);
                    s_left_bottom_adc_x = MMK_GET_TP_X(param);
   
                    s_left_bottom_adc_y = MMK_GET_TP_Y(param);

                    //COORDINATE_TP:"COORDINATE.C left bottom: x = %d, y = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_329_112_2_18_3_3_30_6,(uint8*)"dd", s_left_bottom_adc_x, s_left_bottom_adc_y);
                s_click_times++;
                    break;
                  
                 default:
                    break;                                             
            }            
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:
            if (s_click_times < 3)
            {
                // 在下一个位置绘制十字
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
            }            
            else
            {
                if (COORDINATE_IsVerifySuccess())
                {
                    // 校验成功
                    //MMIPUB_OpenAlertSuccessWin(TXT_COORDINATE_OK);
                    // 关闭窗口
                    MMK_CloseWin(win_id);
                }
                else
                {
                    // 校验失败
                    //MMIPUB_OpenQueryWinByTextId(TXT_COORDINATE_TRY_AGAIN, IMAGE_PUBWIN_QUERY, PNULL, HandleTpVerifyFailPromptWindow);
                    MMIPUB_OpenAlertFailWin(TXT_COORDINATE_TRY_AGAIN);
                }
                s_click_times = 0;
                s_click_point = 0;
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_GET_FOCUS:
            TP_SetRawDataMode(TRUE);
            break;

        case MSG_LOSE_FOCUS:
            TP_SetRawDataMode(FALSE);
            break;

        case MSG_CLOSE_WINDOW:
            // 恢复数据模式到非raw data模式
            MMK_SetCoordinateTpFlag(FALSE);
            TP_SetRawDataMode(FALSE);
            add_data_ptr =(BOOLEAN *) MMK_GetWinAddDataPtr(win_id);
            //SCI_ASSERT(PNULL != add_data_ptr);/*assert verified*/
           
            if (PNULL != add_data_ptr && *add_data_ptr)
            {
                TP_SetPowerOnFirst(FALSE);
                //  第一次开机, 继续初始化
                MMIAPIPHONE_NormalInitAfterCoordinate();     
                SCI_FREE(add_data_ptr);
            }
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return(result);
}
/*****************************************************************************/
//  Description : is coordinate win open
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPITP_Coordinate_IsWinOpen(void)
{
    return MMK_IsOpenWin(TP_COORDINATE_WIN_ID);
}
/*****************************************************************************/
//  Description : handle tp Verify fail prompt win
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpVerifyFailPromptWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_OK:
            MMK_CloseWin(win_id);    
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );
            MMK_CloseWin( TP_COORDINATE_WIN_ID );
            break;
        
        default:
            result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
            break;
    }
    return (result);
}
/*****************************************************************************/
//  Description : coordinate window open
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void COORDINATE_OpenWin(
                                BOOLEAN is_first   // 是否第一次开机
                                )
{
    BOOLEAN  *add_data_ptr =PNULL;
#if 0    
    uint32 g_tp_xfactor = 0;
    uint32 g_tp_yfactor = 0;
    MN_RETURN_RESULT_E  nv_return = MN_RETURN_SUCCESS;
    
    MMINV_READ(TPNV_CAL_DEFAULT_FLAG,  &s_cal_default,  nv_return);
    if (!((MN_RETURN_SUCCESS == nv_return) && (CALIBRATION_FLAG == s_cal_default.is_valid)))
    {
        TP_INFO_T           *tp_info_ptr = TP_GetInfo();

        s_cal_default.is_valid = CALIBRATION_FLAG;
        g_tp_xfactor = tp_info_ptr->adc_max_x - tp_info_ptr->adc_min_x;//TP_ADC_MAX_X - TP_ADC_MIN_X;
        g_tp_xfactor = g_tp_xfactor*FLOAT32_PRECISION;
        g_tp_xfactor = g_tp_xfactor/tp_info_ptr->tp_width;//TP_WIDTH;
        g_tp_yfactor = tp_info_ptr->adc_max_y - tp_info_ptr->adc_min_y;//TP_ADC_MAX_Y - TP_ADC_MIN_Y;
        g_tp_yfactor = g_tp_yfactor*FLOAT32_PRECISION;
        g_tp_yfactor = g_tp_yfactor/tp_info_ptr->tp_height;//TP_HEIGHT;
        s_cal_default.x_top_factor = s_cal_default.x_bottom_factor = g_tp_xfactor;
        s_cal_default.y_left_factor = s_cal_default.y_right_factor = g_tp_yfactor;
        s_cal_default.x_min_top = s_cal_default.x_min_bottom = tp_info_ptr->adc_min_x;//TP_ADC_MIN_X;
        s_cal_default.x_max_top = s_cal_default.x_max_bottom = tp_info_ptr->adc_max_x;//TP_ADC_MAX_X;
        s_cal_default.y_min_left = s_cal_default.y_min_right = tp_info_ptr->adc_min_y;//TP_ADC_MIN_Y;
        s_cal_default.y_max_left = s_cal_default.y_max_right = tp_info_ptr->adc_max_y;//TP_ADC_MAX_Y;
        s_cal_default.x_center = (tp_info_ptr->adc_min_x + tp_info_ptr->adc_max_x)/2;//(TP_ADC_MIN_X + TP_ADC_MAX_X) / 2;
        s_cal_default.y_center = (tp_info_ptr->adc_min_y + tp_info_ptr->adc_max_y)/2;//(TP_ADC_MIN_Y + TP_ADC_MAX_Y) / 2;
        s_cal_default.x_min_up = tp_info_ptr->adc_min_x;//TP_ADC_MIN_X;
        s_cal_default.y_min_up = tp_info_ptr->adc_min_y;//TP_ADC_MIN_Y;
        MMINV_WRITE(TPNV_CAL_DEFAULT_FLAG, &s_cal_default);
    }    
 #endif
 
    // 设置raw data模式
    TP_SetRawDataMode(TRUE);
    add_data_ptr = SCI_ALLOC_APP(sizeof(BOOLEAN));
    if (PNULL == add_data_ptr)
    {
        //SCI_TRACE_LOW:"MMI:COORDINATE_OpenWin(), alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_479_112_2_18_3_3_31_7,(uint8*)"");
    }
    else
    {
        *add_data_ptr  = is_first;
        MMK_CreateWin((uint32*)COORDINATE_WIN_TAB, (ADD_DATA) add_data_ptr);
    }
}

/*****************************************************************************/
//  Description : DRAW THE POINT FOR CLICK     
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
LOCAL void COORDINATE_ShowPoint(uint8 pos)
{
    int16              temp    = 0;
    int16              left    = 0;
    int16              top     = 0;
    int16              right   = 0;
    int16              bottom  = 0;
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    //uint16              draw_pos = pos;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    // 为保证精度，定点扩大1000倍
    s_cross_height =((COORDINATE_HALF_LINE_LENGTH*2+1)*lcd_height*1000)/lcd_width;
    s_cross_height = s_cross_height/1000;
    s_cross_half_height = (s_cross_height-1)/2;

    temp= s_cross_half_height-COORDINATE_HALF_LINE_LENGTH;

    switch (pos)
    {
    case 0: // centre point
        // 横线
        left   = (lcd_width-1) / 2 - COORDINATE_HALF_LINE_LENGTH;
        right  = (lcd_width-1) / 2 + COORDINATE_HALF_LINE_LENGTH;
        
        top    = (lcd_height-1) / 2;
        bottom = (lcd_height-1) / 2;

        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top - 1), right, (uint16)(bottom - 1), 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top + 1), right, (uint16)(bottom + 1), 0);
        
        // 竖线
        left   = (lcd_width-1) / 2;
        right  = (lcd_width-1) / 2;
    
        top    = (lcd_height-1) / 2 - COORDINATE_HALF_LINE_LENGTH;
        bottom = (lcd_height-1) / 2 + COORDINATE_HALF_LINE_LENGTH;

        LCD_DrawLine(&lcd_dev_info, (uint16)(left - 1), top, (uint16)(right - 1), bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, (uint16)(left + 1), top, (uint16)(right + 1), bottom, 0);
        break;
    case 1: // left top point
         // 横线
        left   = 0;
        right  = COORDINATE_LINE_LENGTH;
     
        top    = s_cross_half_height;
        bottom = s_cross_half_height;

        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top - 1), right, (uint16)(bottom - 1), 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top + 1), right, (uint16)(bottom + 1), 0);

        // 竖线
        left   = COORDINATE_HALF_LINE_LENGTH-1;
        right  = COORDINATE_HALF_LINE_LENGTH-1;

        top    = MMI_MAX(temp, 0);
        bottom = MMI_MIN((s_cross_height-1), (s_cross_height-1)-temp);/*lint !e737*/

        LCD_DrawLine(&lcd_dev_info, (uint16)(left - 1), top, (uint16)(right - 1), bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, (uint16)(left + 1), top, (uint16)(right + 1), bottom, 0);
        break;
		case 2: // right top point
        left   = (lcd_width-1) - COORDINATE_LINE_LENGTH;
        right  = (lcd_width-1);
     
        top    = s_cross_half_height;
        bottom = s_cross_half_height;

        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top - 1), right, (uint16)(bottom - 1), 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top + 1), right, (uint16)(bottom + 1), 0);

        // 竖线
        left   = (lcd_width-1) - COORDINATE_HALF_LINE_LENGTH;
        right  = (lcd_width-1) - COORDINATE_HALF_LINE_LENGTH;

        top    = MMI_MAX(temp, 0);
        bottom = MMI_MIN((s_cross_height-1), (s_cross_height-1)-temp);/*lint !e737*/

        LCD_DrawLine(&lcd_dev_info, (uint16)(left - 1), top, (uint16)(right - 1), bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, (uint16)(left + 1), top, (uint16)(right + 1), bottom, 0);
        break;
        
    case 3: // left bottom point
        // 横线
        left   = 0;
        right  = COORDINATE_LINE_LENGTH;
    
        top    =  (lcd_height-1) - s_cross_half_height;
        bottom =  (lcd_height-1) - s_cross_half_height;

        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top - 1), right, (uint16)(bottom - 1), 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top + 1), right, (uint16)(bottom + 1), 0);

        // 竖线
        left   = COORDINATE_HALF_LINE_LENGTH;
        right  = COORDINATE_HALF_LINE_LENGTH;

        top    =  lcd_height - s_cross_height +temp;/*lint !e737*/
        bottom =  MMI_MIN(lcd_height-temp, lcd_height-1);

        LCD_DrawLine(&lcd_dev_info, (uint16)(left - 1), top, (uint16)(right - 1), bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, (uint16)(left + 1), top, (uint16)(right + 1), bottom, 0);
        break;

        
    case 4: // right bottom point
        // 横线
        left   = lcd_width-COORDINATE_LINE_LENGTH;
        right  = lcd_width;
    
        top    = lcd_height-1 - s_cross_half_height;
        bottom = lcd_height-1 - s_cross_half_height;

        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top - 1), right, (uint16)(bottom - 1), 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, (uint16)(top + 1), right, (uint16)(bottom + 1), 0);

        // 竖线
        left   = lcd_width-1 - COORDINATE_HALF_LINE_LENGTH;
        right  = lcd_width-1 - COORDINATE_HALF_LINE_LENGTH;

        top    =  lcd_height - s_cross_height +temp;/*lint !e737*/
        bottom =  MMI_MIN(lcd_height-temp, lcd_height-1);

        LCD_DrawLine(&lcd_dev_info, (uint16)(left - 1), top, (uint16)(right - 1), bottom, 0);
        LCD_DrawLine(&lcd_dev_info, left, top, right, bottom, 0);
        LCD_DrawLine(&lcd_dev_info, (uint16)(left + 1), top, (uint16)(right + 1), bottom, 0);
        break;

    default:
        break;
    }
}

/*
    PointVerify: give a point's coordinate, get result whether ADC value is OK.
        Base on s_cal_default to decide ADC value is between coordinate +/- OFFSET_X/Y's ADC value. 
*/
static BOOLEAN PointVerify(uint16 x, uint16 y, uint16 adc_x, uint16 adc_y)
{
    int16 adc_x_min, adc_x_max, adc_y_min, adc_y_max;       //adc value range
    TP_INFO_T           *tp_info_ptr = TP_GetInfo();
    //SCI_TRACE_LOW:"PointVerify (x,y,adc_x,adc_y)=%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_661_112_2_18_3_3_31_8,(uint8*)"dd",x,y,adc_x,adc_y);
    
    if (x > tp_info_ptr->tp_width/2)     //right
    {
        adc_y_min = (int32)s_cal_default.y_min_right + (tp_info_ptr->tp_lcd_height - y - OFFSET_Y) * s_cal_default.y_right_factor / FLOAT32_PRECISION; 
        adc_y_max = (int32)s_cal_default.y_min_right + (tp_info_ptr->tp_lcd_height - y + OFFSET_Y) * s_cal_default.y_right_factor / FLOAT32_PRECISION; 
    }
    else                    //left
    {
        adc_y_min = (int32)s_cal_default.y_min_left + (tp_info_ptr->tp_lcd_height - y - OFFSET_Y) * s_cal_default.y_left_factor / FLOAT32_PRECISION; 
        adc_y_max = (int32)s_cal_default.y_min_left + (tp_info_ptr->tp_lcd_height - y + OFFSET_Y) * s_cal_default.y_left_factor / FLOAT32_PRECISION;      
    }
    
    adc_y_min = adc_y_min < 0 ? 0 : adc_y_min;
    adc_y_max = adc_y_max > 1023 ? 1023 : adc_y_max;
        
    if (y > tp_info_ptr->tp_lcd_height/2)        //bottom
    {
        adc_x_min = (int32)s_cal_default.x_min_bottom + (tp_info_ptr->tp_width - x - OFFSET_X) * s_cal_default.x_bottom_factor / FLOAT32_PRECISION;
        adc_x_max = (int32)s_cal_default.x_min_bottom + (tp_info_ptr->tp_width - x + OFFSET_X) * s_cal_default.x_bottom_factor / FLOAT32_PRECISION;
    }
    else                            //top
    {
        adc_x_min = (int32)s_cal_default.x_min_top + (tp_info_ptr->tp_width - x - OFFSET_X) * s_cal_default.x_top_factor / FLOAT32_PRECISION;
        adc_x_max = (int32)s_cal_default.x_min_top + (tp_info_ptr->tp_width - x + OFFSET_X) * s_cal_default.x_top_factor / FLOAT32_PRECISION;
    }
    adc_x_min = adc_x_min < 0 ? 0 : adc_x_min;
    adc_x_max = adc_x_max > 1023 ? 1023 : adc_x_max;
    
    
    //COORDINATE_TP:"COORDINATE.C PointVerify: x=%d, adc_x=%d, min=%d, max=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_691_112_2_18_3_3_31_9,(uint8*)"dddd", x, adc_x, adc_x_min, adc_x_max);
    //COORDINATE_TP:"COORDINATE.C PointVerify: y=%d, adc_y=%d, min=%d, max=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_692_112_2_18_3_3_31_10,(uint8*)"dddd", y, adc_y, adc_y_min, adc_y_max);
    return (((adc_x_min<=adc_x) && (adc_x<=adc_x_max)) && ((adc_y_min<=adc_y) && (adc_y<=adc_y_max)));
}

#ifdef TP_SELFADAPTIVE_ENABLE
/*****************************************************************************/
//  Function name:	LocalDoCalibration
//  Description: To do calibration and save the result into "s_cal". This process
//      refers to the previous implementation.
//  Global resource dependence: s_cal
//  Author: Vine.Yuan
//  Note: The global variable, s_cal, holds the final values as result
/*****************************************************************************/
void LocalDoCalibration(
    TP_ADC_T* rt_adc, //Pointer to the right-top point's adc value
    TP_ADC_T* lb_adc, //Pointer to the left-bottom point's adc value
    TP_ADC_T* m_adc   //Pointer to the center point's adc value
    )
{
    FLOAT32 x_rate   = 0;
    FLOAT32 y_rate   = 0;
    int     tmp      = 0;
    
    TP_INFO_T *tp_info_ptr = TP_GetInfo();
    //SCI_ASSERT(NULL != tp_info_ptr);/*assert verified*/
    if(PNULL == tp_info_ptr)
        return;
    
    //X-AXIS
    x_rate = float32_div(lb_adc->x_adc - rt_adc->x_adc, tp_info_ptr->tp_width - 2 * COORDINATE_HALF_LINE_LENGTH);
    tmp = (int)rt_adc->x_adc - (int)(COORDINATE_HALF_LINE_LENGTH*x_rate/FLOAT32_PRECISION);
    s_cal.x_min_top = (tmp < 0) ? 0 : tmp;
    s_cal.x_min_bottom = s_cal.x_min_top;

    tmp = (int)lb_adc->x_adc + COORDINATE_HALF_LINE_LENGTH*x_rate/FLOAT32_PRECISION;
    s_cal.x_max_top = tmp > 1023 ? 1023 : tmp;
    s_cal.x_max_bottom = s_cal.x_max_top;

    s_cal.x_top_factor = x_rate;
    s_cal.x_bottom_factor = x_rate;

    //Y-AXIS
    y_rate = float32_div(rt_adc->y_adc - lb_adc->y_adc, tp_info_ptr->tp_lcd_height - 2 * COORDINATE_HALF_LINE_LENGTH); 
    tmp = (int)lb_adc->y_adc - COORDINATE_HALF_LINE_LENGTH * y_rate / FLOAT32_PRECISION;
    s_cal.y_min_left = (tmp < 0) ? 0 : tmp;
    s_cal.y_min_right = s_cal.y_min_left;

    tmp = (int)rt_adc->y_adc + COORDINATE_HALF_LINE_LENGTH * y_rate / FLOAT32_PRECISION;
    s_cal.y_max_left = tmp > 1023 ? 1023 : tmp;
    s_cal.y_max_right = s_cal.y_max_left;

    s_cal.y_left_factor = y_rate;
    s_cal.y_right_factor = y_rate; 
        
    s_cal.x_center = m_adc->x_adc;
    s_cal.y_center = m_adc->y_adc;

    //Pen up
    s_cal.x_min_up = (s_cal.x_min_top > s_cal.x_min_bottom) ? s_cal.x_min_bottom : s_cal.x_min_top;
    s_cal.y_min_up = (s_cal.y_min_left > s_cal.y_min_right) ? s_cal.y_min_right : s_cal.y_min_left;

    //To mark the valid calibration flag 
    s_cal.is_valid = CALIBRATION_FLAG;
}

/*****************************************************************************/
//  Function name:	LocalExtendY
//  Description: To extend the rang in dimension y of touch panel to cover the 
//      whole area, which includes the shortcut area.
//  Global resource dependence: s_cal
//  Author: Vine.Yuan
//  Note: The global variable, s_cal, holds the final values as result
/*****************************************************************************/
void LocalExtendY(
    TP_MODE_E tp_mode //indication of self-adaptive mode
    )
{
    TP_INFO_T *tp_info_ptr = TP_GetInfo();
    //SCI_ASSERT(NULL != tp_info_ptr);/*assert verified*/
    if(PNULL == tp_info_ptr)
        return;

    switch(tp_mode)
    {
        case TP_MODE_NORMAL:
        case TP_MODE_XW:
        case TP_MODE_XYE:
        case TP_MODE_XYE_YW:
            s_cal.y_min_left = s_cal.y_min_left - (tp_info_ptr->tp_height - tp_info_ptr->tp_lcd_height)
                * s_cal.y_left_factor / FLOAT32_PRECISION;
            s_cal.y_min_left = ((int16)s_cal.y_min_left < 0) ? 0 : s_cal.y_min_left;

            //According to prior implementation, assume the the right side is equal to the left.
            s_cal.y_min_right = s_cal.y_min_left; 
            break;
        case TP_MODE_YW:
        case TP_MODE_XYW:
        case TP_MODE_XYE_XW:
        case TP_MODE_XYE_XYW:
           s_cal.y_max_left = s_cal.y_max_left + (tp_info_ptr->tp_height - tp_info_ptr->tp_lcd_height)
                * s_cal.y_left_factor / FLOAT32_PRECISION;
            s_cal.y_max_left = (s_cal.y_max_left > 1023) ? 1023 : s_cal.y_max_left;

            //According to prior implementation, assume the the right side is equal to the left.
            s_cal.y_max_right = s_cal.y_max_left; 
            break;
        default:
            break;
            
    }  
}

/*****************************************************************************/
//  Function name:	LocalSwapCaliParam
//  Description: To swap the values between x and y for different parameters,
//      respectively.
//  Global resource dependence: s_cal
//  Author: Vine.Yuan
//  Note: The global variable, s_cal, holds the final values as result
/*****************************************************************************/
void LocalSwapCaliParam(void)
{
    uint16 TempBuf;

    //To swap the factor value between x and y
    TempBuf = s_cal.x_top_factor;
    s_cal.x_top_factor = s_cal.y_left_factor;
    s_cal.y_left_factor = TempBuf;
    
    TempBuf = s_cal.x_bottom_factor;
    s_cal.x_bottom_factor = s_cal.y_right_factor;
    s_cal.y_right_factor = TempBuf;

    //To swap the min value between x and y
    TempBuf = s_cal.x_min_top;
    s_cal.x_min_top = s_cal.y_min_left;
    s_cal.y_min_left = TempBuf;

    TempBuf = s_cal.x_min_bottom;
    s_cal.x_min_bottom = s_cal.y_min_right;
    s_cal.y_min_right = TempBuf;

    //To swap the max value between x and y
    TempBuf = s_cal.x_max_top;
    s_cal.x_max_top = s_cal.y_max_left;
    s_cal.y_max_left = TempBuf;

    TempBuf = s_cal.x_max_bottom;
    s_cal.x_max_bottom = s_cal.y_max_right;
    s_cal.y_max_right = TempBuf;

    //To swap the center value between x and y
    TempBuf = s_cal.x_center;
    s_cal.x_center = s_cal.y_center;
    s_cal.y_center = TempBuf;

    //To swap the min value of pen-up between x and y
    TempBuf = s_cal.x_min_up;
    s_cal.x_min_up = s_cal.y_min_up;
    s_cal.y_min_up = TempBuf;
}

/*****************************************************************************/
//  Function name:	IsValidAdapt
//  Description: At first, according to the reference coordinate, offer the 
//      information about whether or not this process is a valid self-adaptive 
//      calibration. Meanwhile, the calibration information has been saved into
//      s_cal. This process refers to the previous implementation.
//  Global resource dependence: s_cal_default, s_cal
//  Author: Vine.Yuan
//  Note: The global variable, s_cal, has held the information of calibration
/*****************************************************************************/
BOOLEAN IsValidAdapt(
    TP_ADC_T* rt_adc, //Pointer to the right-top point's adc value
    TP_ADC_T* lb_adc, //Pointer to the left-bottom point's adc value
    TP_ADC_T* m_adc   //Pointer to the center point's adc value
    )
{

    BOOLEAN set_success = TRUE;
    TP_INFO_T *tp_info_ptr = TP_GetInfo();

    /*
    * Here, assume that left_adc_x > right_adc_x and top_adc_y > s_bottom_adc_y, 
    * because of the reference coordinate
    */
    if (lb_adc->x_adc <= rt_adc->x_adc
        || rt_adc->y_adc <= lb_adc->y_adc)
        return FALSE;

    /*
    * Here, assume that the wider the side is, the more the difference value of
    * adc between two calibration points is.  
    * Caution:Maybe, this supposition is not correct.
    */
    if (tp_info_ptr->tp_width < tp_info_ptr->tp_lcd_height)
    {
        if ((lb_adc->x_adc - rt_adc->x_adc) 
            >= (rt_adc->y_adc - lb_adc->y_adc))
        {
            //COORDINATE_TP:"[VINE]--Not match the scale of x and y(1)\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_884_112_2_18_3_3_31_11,(uint8*)"");
            return FALSE;
        }
    }
    else
    {
        if ((lb_adc->x_adc - rt_adc->x_adc) 
            <= (rt_adc->y_adc - lb_adc->y_adc))
        {
            //COORDINATE_TP:"[VINE]--Not match the scale of x and y(2)\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_893_112_2_18_3_3_32_12,(uint8*)"");
            return FALSE;
        }
    }

    //To carry out the calibration and save final result into "s_cal"
    LocalDoCalibration(rt_adc, lb_adc, m_adc);

    //To set calibration parameter as default
    s_cal_default = s_cal;

    //right top
    //COORDINATE_TP:"COORDINATE.C IsVerifySuccess: right top corner."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_905_112_2_18_3_3_32_13,(uint8*)"");
    set_success = set_success && PointVerify((uint16)(tp_info_ptr->tp_width - COORDINATE_HALF_LINE_LENGTH), 
    	COORDINATE_HALF_LINE_LENGTH, rt_adc->x_adc, rt_adc->y_adc); 
           
    //left bottom
    //COORDINATE_TP:"COORDINATE.C IsVerifySuccess: left bottom corner."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_910_112_2_18_3_3_32_14,(uint8*)"");
    set_success = set_success && PointVerify(COORDINATE_HALF_LINE_LENGTH, (uint16)(tp_info_ptr->tp_lcd_height - COORDINATE_HALF_LINE_LENGTH), 
        lb_adc->x_adc, lb_adc->y_adc);

    //center 
    //COORDINATE_TP:"COORDINATE.C IsVerifySuccess: center."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_915_112_2_18_3_3_32_15,(uint8*)"");
    set_success = set_success && PointVerify((uint16)(tp_info_ptr->tp_width/2), (uint16)(tp_info_ptr->tp_lcd_height/2), 
        m_adc->x_adc, m_adc->y_adc);

    return set_success;
}

/*****************************************************************************/
//  Function name:	_tp_cali_normal
//  Description: To make a self-adaptive attempt for TP_MODE_NORMAL mode, if available,
//      proceed to do further process.
//  Global resource dependence: s_right_top_adc_x, s_right_top_adc_y, 
//          s_left_bottom_adc_x, s_left_bottom_adc_y, s_mid_adc_x, s_mid_adc_y
//  Author: Vine.Yuan
//  Note: The final result will be saved into "s_cal"
/*****************************************************************************/
BOOLEAN _tp_cali_normal(void)
{
    TP_ADC_T rt_adc;
    TP_ADC_T lb_adc;
    TP_ADC_T m_adc;

    //-- Right top --
    rt_adc.x_adc = s_right_top_adc_x;
    rt_adc.y_adc = s_right_top_adc_y;

    //-- Left Bottom --
    lb_adc.x_adc = s_left_bottom_adc_x;
    lb_adc.y_adc = s_left_bottom_adc_y;

    //-- Center --
    m_adc.x_adc = s_mid_adc_x;
    m_adc.y_adc = s_mid_adc_y;

    if (IsValidAdapt(&rt_adc,&lb_adc,&m_adc))
    {
        LocalExtendY(TP_MODE_NORMAL);
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Function name:	_tp_cali_xw
//  Description: To make a self-adaptive attempt for TP_MODE_XW mode, if available,
//      proceed to do further process.
//  Global resource dependence: s_right_top_adc_x, s_right_top_adc_y, 
//          s_left_bottom_adc_x, s_left_bottom_adc_y, s_mid_adc_x, s_mid_adc_y
//  Author: Vine.Yuan
//  Note: The final result will be saved into "s_cal"
/*****************************************************************************/
BOOLEAN _tp_cali_xw(void)
{
    TP_ADC_T rt_adc;
    TP_ADC_T lb_adc;
    TP_ADC_T m_adc;

    /*---------------------------------------
    * To exchange the value between Xr and Xl
    ----------------------------------------*/
    //-- Right top --
    rt_adc.x_adc = s_left_bottom_adc_x;
    rt_adc.y_adc = s_right_top_adc_y;

    //-- Left Bottom --
    lb_adc.x_adc = s_right_top_adc_x;
    lb_adc.y_adc = s_left_bottom_adc_y;

    //-- Center --
    m_adc.x_adc = s_mid_adc_x;
    m_adc.y_adc = s_mid_adc_y;
    /*-----------------------------------------
    * ----------- End -------------------------
    ------------------------------------------*/
    if (IsValidAdapt(&rt_adc,&lb_adc,&m_adc))
    {
        LocalExtendY(TP_MODE_XW);
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Function name:	_tp_cali_yw
//  Description: To make a self-adaptive attempt for TP_MODE_YW mode, if available,
//      proceed to do further process.
//  Global resource dependence: s_right_top_adc_x, s_right_top_adc_y, 
//          s_left_bottom_adc_x, s_left_bottom_adc_y, s_mid_adc_x, s_mid_adc_y
//  Author: Vine.Yuan
//  Note: The final result will be saved into "s_cal"
/*****************************************************************************/
BOOLEAN _tp_cali_yw(void)
{
    TP_ADC_T rt_adc;
    TP_ADC_T lb_adc;
    TP_ADC_T m_adc;

    /*---------------------------------------
    * To exchange the value between Yu and Yd
    ----------------------------------------*/
    //-- Right top --
    rt_adc.x_adc = s_right_top_adc_x;
    rt_adc.y_adc = s_left_bottom_adc_y;

    //-- Left Bottom --
    lb_adc.x_adc = s_left_bottom_adc_x;
    lb_adc.y_adc = s_right_top_adc_y;

    //-- Center --
    m_adc.x_adc = s_mid_adc_x;
    m_adc.y_adc = s_mid_adc_y;
    /*-----------------------------------------
    * ----------- End -------------------------
    ------------------------------------------*/
    if (IsValidAdapt(&rt_adc,&lb_adc,&m_adc))
    {
        LocalExtendY(TP_MODE_YW);
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Function name:	_tp_cali_xyw
//  Description: To make a self-adaptive attempt for TP_MODE_XYW mode, if available,
//      proceed to do further process.
//  Global resource dependence: s_right_top_adc_x, s_right_top_adc_y, 
//          s_left_bottom_adc_x, s_left_bottom_adc_y, s_mid_adc_x, s_mid_adc_y
//  Author: Vine.Yuan
//  Note: The final result will be saved into "s_cal"
/*****************************************************************************/
BOOLEAN _tp_cali_xyw(void)
{
    TP_ADC_T rt_adc;
    TP_ADC_T lb_adc;
    TP_ADC_T m_adc;

    /*---------------------------------------
    * To exchange the value between Xr and Xl, 
    * Yu and Yd
    ----------------------------------------*/
    //-- Right top --
    rt_adc.x_adc = s_left_bottom_adc_x;
    rt_adc.y_adc = s_left_bottom_adc_y;

    //-- Left Bottom --
    lb_adc.x_adc = s_right_top_adc_x;
    lb_adc.y_adc = s_right_top_adc_y;

    //-- Center --
    m_adc.x_adc = s_mid_adc_x;
    m_adc.y_adc = s_mid_adc_y;

    /*-----------------------------------------
    * ----------- End -------------------------
    ------------------------------------------*/
    if (IsValidAdapt(&rt_adc,&lb_adc,&m_adc))
    {
        LocalExtendY(TP_MODE_XYW);
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Function name:	_tp_cali_xye
//  Description: To make a self-adaptive attempt for TP_MODE_XYE mode, if 
//      available, proceed to do further process.
//  Global resource dependence: s_right_top_adc_x, s_right_top_adc_y, 
//          s_left_bottom_adc_x, s_left_bottom_adc_y, s_mid_adc_x, s_mid_adc_y
//  Author: Vine.Yuan
//  Note: The final result will be saved into "s_cal"
/*****************************************************************************/
BOOLEAN _tp_cali_xye(void)
{
    TP_ADC_T rt_adc;
    TP_ADC_T lb_adc;
    TP_ADC_T m_adc;
    /*---------------------------------------
    * To exchange the value between x and y
    ----------------------------------------*/
    //-- Right top --
    rt_adc.x_adc = s_right_top_adc_y;
    rt_adc.y_adc = s_right_top_adc_x;

    //-- Left Bottom --
    lb_adc.x_adc = s_left_bottom_adc_y;
    lb_adc.y_adc = s_left_bottom_adc_x;

    //-- Center --
    m_adc.x_adc = s_mid_adc_y;
    m_adc.y_adc = s_mid_adc_x;
    /*-----------------------------------------
    * ----------- End -------------------------
    ------------------------------------------*/
    if (IsValidAdapt(&rt_adc,&lb_adc,&m_adc))
    {
        LocalExtendY(TP_MODE_XYE);
        
        //To swap the value of calibration parameter
        LocalSwapCaliParam();
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Function name:	_tp_cali_xye_xw
//  Description: To make a self-adaptive attempt for TP_MODE_XYE_XW mode, if 
//      available, proceed to do further process.
//  Global resource dependence: s_right_top_adc_x, s_right_top_adc_y, 
//          s_left_bottom_adc_x, s_left_bottom_adc_y, s_mid_adc_x, s_mid_adc_y
//  Author: Vine.Yuan
//  Note: The final result will be saved into "s_cal"
/*****************************************************************************/
BOOLEAN _tp_cali_xye_xw(void)
{
    TP_ADC_T rt_adc;
    TP_ADC_T lb_adc;
    TP_ADC_T m_adc;

    /*---------------------------------------
    * To exchange the value between x and y, 
    * and then exchange the value between Xr and Xl
    ----------------------------------------*/
    //-- Right top --
    rt_adc.x_adc = s_right_top_adc_y;
    rt_adc.y_adc = s_left_bottom_adc_x;

    //-- Left Bottom --
    lb_adc.x_adc = s_left_bottom_adc_y;
    lb_adc.y_adc = s_right_top_adc_x;

    //-- Center --
    m_adc.x_adc = s_mid_adc_y;
    m_adc.y_adc = s_mid_adc_x;
    /*-----------------------------------------
    * ----------- End -------------------------
    ------------------------------------------*/

    if (IsValidAdapt(&rt_adc,&lb_adc,&m_adc))
    {
        LocalExtendY(TP_MODE_XYE_XW);
        //To swap the value of calibration parameter
        LocalSwapCaliParam();
        
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Function name:	_tp_cali_xye_yw
//  Description: To make a self-adaptive attempt for TP_MODE_XYE_YW mode, if 
//      available, proceed to do further process.
//  Global resource dependence: s_right_top_adc_x, s_right_top_adc_y, 
//          s_left_bottom_adc_x, s_left_bottom_adc_y, s_mid_adc_x, s_mid_adc_y
//  Author: Vine.Yuan
//  Note: The final result will be saved into "s_cal"
/*****************************************************************************/
BOOLEAN _tp_cali_xye_yw(void)
{
    TP_ADC_T rt_adc;
    TP_ADC_T lb_adc;
    TP_ADC_T m_adc;

    /*---------------------------------------
    * To exchange the value between x and y, 
    * and then exchange the value between Yu and Yd
    ----------------------------------------*/
    //-- Right top --
    rt_adc.x_adc = s_left_bottom_adc_y;
    rt_adc.y_adc = s_right_top_adc_x;

    //-- Left Bottom --
    lb_adc.x_adc = s_right_top_adc_y;
    lb_adc.y_adc = s_left_bottom_adc_x;

    //-- Center --
    m_adc.x_adc = s_mid_adc_y;
    m_adc.y_adc = s_mid_adc_x;
    /*-----------------------------------------
    * ----------- End -------------------------
    ------------------------------------------*/

    if (IsValidAdapt(&rt_adc,&lb_adc,&m_adc))
    {
        LocalExtendY(TP_MODE_XYE_YW);
        
        //To swap the value of calibration parameter
        LocalSwapCaliParam();
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Function name:	_tp_cali_xye_xyw
//  Description: To make a self-adaptive attempt for TP_MODE_XYE_XYW mode, if 
//    available, proceed to do further process.
//  Global resource dependence: s_right_top_adc_x, s_right_top_adc_y, 
//          s_left_bottom_adc_x, s_left_bottom_adc_y, s_mid_adc_x, s_mid_adc_y
//  Author: Vine.Yuan
//  Note: The final result will be saved into "s_cal"
/*****************************************************************************/
BOOLEAN _tp_cali_xye_xyw(void)
{
    TP_ADC_T rt_adc;
    TP_ADC_T lb_adc;
    TP_ADC_T m_adc;

    /*---------------------------------------
    * To exchange the value between x and y, 
    * and then exchange the value between Yu and Yd,
    * Xl and Xr
    ----------------------------------------*/
    //-- Right top --
    rt_adc.x_adc = s_left_bottom_adc_y;
    rt_adc.y_adc = s_left_bottom_adc_x;

    //-- Left Bottom --
    lb_adc.x_adc = s_right_top_adc_y;
    lb_adc.y_adc = s_right_top_adc_x;

    //-- Center --
    m_adc.x_adc = s_mid_adc_y;
    m_adc.y_adc = s_mid_adc_x;
    /*-----------------------------------------
    * ----------- End -------------------------
    ------------------------------------------*/

    if (IsValidAdapt(&rt_adc,&lb_adc,&m_adc))
    {
        LocalExtendY(TP_MODE_XYE_XYW);
        //To swap the value of calibration prameter
        LocalSwapCaliParam();
        return TRUE;
    }
    return FALSE;
}

/*
* To initialize the pointer array to function for calibration based on 
* the information of TP's self-adpative mode, @vine.yuan 2010.1.5
*/
const TP_CALI_FUN_T tp_cali_fun[TP_MODE_MAX]=
    {
    	_tp_cali_normal,
    	_tp_cali_xw,
    	_tp_cali_yw,
    	_tp_cali_xyw,
    	_tp_cali_xye,
    	_tp_cali_xye_xw,
    	_tp_cali_xye_yw,
    	_tp_cali_xye_xyw
    };

LOCAL BOOLEAN COORDINATE_IsVerifySuccess(void)
{
    TP_MODE_E adapt_mode = TP_MODE_NORMAL;
	BOOLEAN set_success = FALSE;
    int32 index;

    for (index = 0; index <TP_MODE_MAX; index++)
    {
        if (NULL == tp_cali_fun[index])
        {
            //COORDINATE_TP:"tp_cali_fun is null\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1272_112_2_18_3_3_33_16,(uint8*)"");
            continue;
        }
        if (!tp_cali_fun[index]())
        {
            continue;
        }
        else
        {
            adapt_mode = (TP_MODE_E)index;
            set_success = TRUE;
            break;
        }
    }
	
	if (set_success)
	{
		//COORDINATE_TP:"COORDINATE.C successed."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1289_112_2_18_3_3_33_17,(uint8*)"");
		MMINV_WRITE(TPNV_CALIBRATION_FLAG, &s_cal);
		MMINV_WRITE(TPNV_CAL_DEFAULT_FLAG, &s_cal);
        
        /*
        * To save the self-adaptive mode into nv and update 
        * current mode. vine.yuan 2010.6.3
        */
        TP_UpdateAdaptMode(adapt_mode);
		
#ifndef _WIN32 
		if (TP_UpdateCalibration(&s_cal) == FALSE)
		{
		    //COORDINATE_TP:"COORDINATE.C update failed."
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1302_112_2_18_3_3_33_18,(uint8*)"");
		    return(FALSE);
		}
#endif
	}

#ifndef _WIN32  
	return(set_success);
#else
	// 模拟器上一次成功
	return TRUE;
#endif
}

#else //TP_SELFADAPTIVE_ENABLE

/*
    Linear exchange
*/
PUBLIC int32 _LinearExchange(int32 x,int32 slope,uint32 offset)
{
    return (slope*x + offset);/*lint !e737*/
}

/*
    Mapping x_coordinate to y_coordinate:
    Y1 = slope*X1 + offset;
    Y2 = slope*X2 + offset;    
    ==>
    slope = (PRECISION*(Y2-Y1))/(X2-X1);
    offset= (PRECISION*(Y1*X2-Y2*X1))/(X2-X1);
 */
PUBLIC LINEAR_OBJ_T SOLVING_LinearEquation( int32 Y1,int32 Y2, 
                                    int32 X1,int32 X2,
                                    int32 PRECISION )
{
    LINEAR_OBJ_T linear_obj = {0};

    SCI_TRACE_LOW ( "[SOLVING_LinearEquation:] %d,%d<---%d,%d",Y1,Y2,X1,X2);	
    
    linear_obj.slope = (PRECISION*(Y2-Y1))/(X2-X1);
    linear_obj.offset= (PRECISION*(Y1*X2-Y2*X1))/(X2-X1);

    SCI_TRACE_LOW ( "[SOLVING_LinearEquation:] %d,%d",linear_obj.slope,linear_obj.offset);	
    return (linear_obj);
}

LOCAL BOOLEAN COORDINATE_IsVerifySuccess(void)
{
    LINEAR_OBJ_T x_linear_obj={0};
    LINEAR_OBJ_T y_linear_obj={0};

    uint32 left =0;
    uint32 right=0;
    uint32 top  =0;
    uint32 bottom=0;
    int32 lcd_width=0;
    int32 lcd_height=0;
    //FLOAT32 x_rate   = 0;
    //FLOAT32 y_rate   = 0;
    BOOLEAN set_success = FALSE;
    //int     tmp         = 0;
    //uint16  for_change_x = 0;	    
    //uint16  for_change_y = 0;
    //uint16  tempxl,tempxr,tempyt,tempyb;
    TP_INFO_T           *tp_info_ptr = TP_GetInfo();

    lcd_width = tp_info_ptr->tp_width;
    lcd_height= tp_info_ptr->tp_lcd_height;
        
    //COORDINATE_TP:"COORDINATE.C s_right_top_adc_x = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1343_112_2_18_3_3_33_19,(uint8*)"d", s_right_top_adc_x);
    //COORDINATE_TP:"COORDINATE.C s_right_top_adc_y = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1344_112_2_18_3_3_33_20,(uint8*)"d", s_right_top_adc_y);
    //COORDINATE_TP:"COORDINATE.C s_left_bottom_adc_x = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1345_112_2_18_3_3_33_21,(uint8*)"d", s_left_bottom_adc_x);
    //COORDINATE_TP:"COORDINATE.C s_left_bottom_adc_y = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1346_112_2_18_3_3_33_22,(uint8*)"d", s_left_bottom_adc_y);
    //COORDINATE_TP:"COORDINATE.C s_mid_adc_x = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1347_112_2_18_3_3_33_23,(uint8*)"d", s_mid_adc_x);
    //COORDINATE_TP:"COORDINATE.C s_mid_adc_y = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1348_112_2_18_3_3_33_24,(uint8*)"d", s_mid_adc_y);
    //COORDINATE_TP:"COORDINATE.C tp_info_ptr->tp_width = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1349_112_2_18_3_3_33_25,(uint8*)"d", tp_info_ptr->tp_width);
    //COORDINATE_TP:"COORDINATE.C tp_info_ptr->tp_lcd_height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1350_112_2_18_3_3_33_26,(uint8*)"d",tp_info_ptr->tp_lcd_height);


    //use left/bottom and right/top
    left   = COORDINATE_HALF_LINE_LENGTH; 
    bottom = (lcd_height-1) - s_cross_half_height;/*lint !e737*/
    right  = (lcd_width-1) - COORDINATE_HALF_LINE_LENGTH;
    top    = s_cross_half_height;
    SCI_TRACE_LOW ( "[COORDINATE.C:] SOLVING_LinearEquation left=%d, bottom=%d,right=%d,top=%d",\
                                                       left,bottom,right,top);	
   /* if((s_left_bottom_adc_x >s_right_top_adc_x)  || (s_right_top_adc_y> s_left_bottom_adc_y))
    {
        SCI_TRACE_LOW(" COORDINATE.C DEIRECTION ERROR");

    }
    else */if((abs(s_left_bottom_adc_x-s_right_top_adc_x) > ADC_OFFSET_X) &&
        (abs(s_left_bottom_adc_y-s_right_top_adc_y) > ADC_OFFSET_Y) )
    {
        //Note:Precison is 1000
        x_linear_obj = SOLVING_LinearEquation(left,right, s_left_bottom_adc_x,s_right_top_adc_x, 1000);
        y_linear_obj = SOLVING_LinearEquation(bottom,top, s_left_bottom_adc_y,s_right_top_adc_y, 1000);    

        SCI_TRACE_LOW ( "[COORDINATE.C:] SOLVING_LinearEquation x_slop=%d, x_offset=%d,y_slop=%d,y_offset=%d",\
                                                           x_linear_obj.slope,x_linear_obj.offset,\
    		                                               y_linear_obj.slope,y_linear_obj.offset);	

        //Indirect flags indicating Calibration has been done. 
        s_cal.y_left_factor = 1500;
        s_cal.x_top_factor  = 1500;

        s_cal.x_slope = x_linear_obj.slope;
        s_cal.x_offset= x_linear_obj.offset;
        s_cal.y_slope = y_linear_obj.slope;
        s_cal.y_offset= y_linear_obj.offset;
        
        s_cal.is_valid = CALIBRATION_FLAG;

        //Use the point of center to verify calibration successful.
        {
            int32 x_center =0;
            int32 y_center =0;        
            x_center = _LinearExchange(s_mid_adc_x, s_cal.x_slope, s_cal.x_offset);
            y_center = _LinearExchange(s_mid_adc_y, s_cal.y_slope, s_cal.y_offset);

            //Note:Precison is 1000
            x_center = x_center/1000;
            y_center = y_center/1000;
            
            if((abs((lcd_width>>1)-x_center) >= OFFSET_X) || 
               (abs((lcd_height>>1)-y_center) >= OFFSET_Y)  )
            {
                set_success =SCI_FALSE;
            }
            else
            {
                set_success =SCI_TRUE;            
            }
            SCI_TRACE_LOW ( "[COORDINATE.C:] _LinearExchange x_center=%d, y_center=%d",\
                                                           x_center,y_center);	
            SCI_TRACE_LOW ( "[COORDINATE.C:] _LinearExchange center_delta_x=%d, center_delta_y=%d",\
                                                           ((lcd_width>>1)-x_center),\
    		                                               ((lcd_height>>1)-y_center));	        
        }
    }

    if(!set_success)
    {
        //COORDINATE_TP:"COORDINATE.C failed."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1505_112_2_18_3_3_33_47,(uint8*)"");
        s_cal_default = s_cal;
    }
    else
    {
        //COORDINATE_TP:"COORDINATE.C successed."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1510_112_2_18_3_3_33_48,(uint8*)"");
        MMINV_WRITE(TPNV_CALIBRATION_FLAG, &s_cal);
        MMINV_WRITE(TPNV_CAL_DEFAULT_FLAG, &s_cal);
#ifndef WIN32 
        if (TP_UpdateCalibration(&s_cal) == FALSE)
        {
            //COORDINATE_TP:"COORDINATE.C update failed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,COORDINATE_WINTAB_1516_112_2_18_3_3_33_49,(uint8*)"");
            return(FALSE);
        }
#endif
    }
    

#ifndef WIN32  
    return(set_success);
#else
       // 模拟器上一次成功
       return TRUE;
#endif
}
//@David.Jia    2005.12.15  end
#endif //TP_SELFADAPTIVE_ENABLE

/*****************************************************************************/
//  Description : float number div
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 此程序摘自mmimultim.c中
/*****************************************************************************/
FLOAT32 float32_div(uint32 num1, uint32 num2)
{
    return(num1 * FLOAT32_PRECISION / num2);
}

/*****************************************************************************/
//  Description : 开机时初始化触摸屏参数
//  Global resource dependence : 
//  Author:guopeng.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void COORDINATE_Init(void)
{
    MN_RETURN_RESULT_E  nv_return = MN_RETURN_FAILURE;
    TP_CALIBRATION_T    tp_calibration = {0};
    
    MMINV_READ(TPNV_CALIBRATION_FLAG, &tp_calibration, nv_return);
    
    if (!(MN_RETURN_SUCCESS == nv_return && CALIBRATION_FLAG == tp_calibration.is_valid))
    {
        MMINV_READ(TPNV_CAL_DEFAULT_FLAG, &tp_calibration, nv_return);

        if (!(MN_RETURN_SUCCESS == nv_return && CALIBRATION_FLAG == tp_calibration.is_valid))
        {
            uint32          tp_xfactor,tp_yfactor;
            TP_INFO_T       *tp_info_ptr = TP_GetInfo();

            tp_xfactor = tp_info_ptr->adc_max_x - tp_info_ptr->adc_min_x;//TP_ADC_MAX_X - TP_ADC_MIN_X;
            tp_xfactor = tp_xfactor*FLOAT32_PRECISION;
            tp_xfactor = tp_xfactor/tp_info_ptr->tp_width;//TP_WIDTH;
            tp_yfactor = tp_info_ptr->adc_max_y - tp_info_ptr->adc_min_y;//TP_ADC_MAX_Y - TP_ADC_MIN_Y;
            tp_yfactor = tp_yfactor*FLOAT32_PRECISION;
            tp_yfactor = tp_yfactor/tp_info_ptr->tp_height;//TP_HEIGHT;
            tp_calibration.x_top_factor = tp_calibration.x_bottom_factor = tp_xfactor;
            tp_calibration.y_left_factor = tp_calibration.y_right_factor = tp_yfactor;
            tp_calibration.x_min_top = tp_calibration.x_min_bottom = tp_info_ptr->adc_min_x;
            tp_calibration.x_max_top = tp_calibration.x_max_bottom = tp_info_ptr->adc_max_x;
            tp_calibration.y_min_left = tp_calibration.y_min_right = tp_info_ptr->adc_min_y;
            tp_calibration.y_max_left = tp_calibration.y_max_right = tp_info_ptr->adc_max_y;
            tp_calibration.x_center = (tp_info_ptr->adc_min_x + tp_info_ptr->adc_max_x) / 2;
            tp_calibration.y_center = (tp_info_ptr->adc_min_y + tp_info_ptr->adc_max_y) / 2;
            tp_calibration.x_min_up = tp_info_ptr->adc_min_x;
            tp_calibration.y_min_up = tp_info_ptr->adc_min_y;
        }
    }
#ifndef WIN32          
    TP_UpdateCalibration(&tp_calibration);
#endif
}

#ifdef WIN32
/*****************************************************************************/
//  FUNCTION:     TP_SetRawDataMode
//  Description:   
//  return:  
//  Author:         
//  date:  2005.05.11
//  Note:        
/*****************************************************************************/
LOCAL void TP_SetRawDataMode(BOOLEAN act_res){}
#endif

#endif //#if defined TOUCH_PANEL_SUPPORT


/*Edit by script, ignore 2 case. Thu Apr 26 19:01:08 2012*/ //IGNORE9527
