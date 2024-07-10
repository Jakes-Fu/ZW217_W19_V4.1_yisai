/*****************************************************************************
** File Name:      mmiidle_cstyle.c                                          *
** Author:                                                                   *
** Date:           11/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma              Creat
******************************************************************************/

#define _MMIIDLE_CSTYLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_app_idle_trc.h"
#include "mmiidle_cstyle.h"
#include "mmk_type.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifm_export.h"
#include "mmiset_text.h"
#include "guires.h" 
#include "mmiidle_func.h"
#include "mmicountedtime_export.h"
#include "mmiidle_func.h"
#include "mmi_id.h"
#include "mmi_appmsg.h"
#include "mmimp3_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#ifdef MMI_ENABLE_DCD
#include "mmidcd_export.h"
#endif
#include "guistatusbar.h"
#if defined MMIWIDGET_SUPPORT || defined MMI_GRID_IDLE_SUPPORT
#include "mmiwidget.h"
#endif
#include "mmi_image.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define IDLE_MP3_TIMER_PERIOD       500 //idle下mp3名字显示的定时器周期

#ifdef MAINLCD_SIZE_128X160 //arvin zhang added for idle softkey icon display
#define MMIIDLE_SOFTKEY_ICON_MAX        2//left icon plus right icon
#define MMIIDLE_SOFTKEY_MARGIN_WIDTH    5//margin space
#endif
#ifdef BAR_PHONE_TOUCH_WIDGET
#define MMIIDLE_SOFTKEY_ADD_HEIGHT    5//176*220 TOUCH 的范围太小，增加高度
#endif
#if defined TOUCH_PANEL_SUPPORT
typedef enum
{
    MMI_COMMON_IDLE_ITEM1 =0,  //sim1运营商名称
    MMI_COMMON_IDLE_ITEM2 = MMI_DUAL_SYS_MAX-1,/*lint !e778*/
    MMI_COMMON_IDLE_ITEM3,  //个性词语/到计时
    MMI_COMMON_IDLE_ITEM4,  //今日日程
    MMI_COMMON_IDLE_ITEM5,  //mp3/fm
    MMI_COMMON_IDLE_ITEM6,  //时间
    MMI_COMMON_IDLE_ITEM7,  //日期
    MMI_COMMON_IDLE_ITEM8,  //星期
    MMI_COMMON_IDLE_ITEM9,  //左软键
    MMI_COMMON_IDLE_ITEM10,  //右软键
    MMI_COMMON_IDLE_ITEM11,  //中软键
    MMI_COMMON_IDLE_MAX_ITEM
}MMI_COMMON_IDLE_ITEM_E;

typedef struct  
{
    GUI_RECT_T                  rect; //显示区域
    MMI_IDLE_DISPLAY_INDEX_E    type; //显示类型
}MMIIDLE_COMMON_DISPLAY_T;
#endif

#if defined MMI_COMMON_IDLE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#if defined TOUCH_PANEL_SUPPORT
LOCAL MMIIDLE_COMMON_DISPLAY_T  s_common_idle_display_rect[MMI_COMMON_IDLE_MAX_ITEM] = {0};//普通模式下idle显示区域
#endif
#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (KURO_SUPPORT)|| defined (FM_SUPPORT)

LOCAL IDLE_MP3_NAME_INFO    g_idle_cur_mp3_name = {0}; //当前播放的MP3名称

#endif
#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)                                        
LOCAL uint8 g_mp3_name_timer_id = 0; //Idle界面循环显示MP3歌词的定时器
#endif
#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO  s_common_idle_paint_layer_handle = {0,UILAYER_NULL_HANDLE};
LOCAL GUI_LCD_DEV_INFO  s_common_idle_stb_layer_handle = {0,UILAYER_NULL_HANDLE};
#endif

#if defined MAINLCD_SIZE_128X160 && defined  MMI_RES_ORIENT_LANDSCAPE
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 2
#elif defined MAINLCD_SIZE_128X160 && defined  MMI_RES_ORIENT_PORTRAIT
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 3
#elif defined MAINLCD_SIZE_176X220 && defined  MMI_RES_ORIENT_LANDSCAPE
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 5
#else
#define MMI_COMMON_IDLE_ITEM_MAX_NUM 6
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : create idle items paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdlePaintLayer(
                              MMI_WIN_ID_T      win_id
                              );

/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetCommonIdlePaintLcdDev(void);

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdlePaintLayer(void);

/*****************************************************************************/
//  Description : create status bar layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note: 
/*****************************************************************************/
LOCAL void CreateCommonIdleStbLayer(
                              MMI_WIN_ID_T      win_id
                              );
                              
/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleStbLayer(MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description : display the  content
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutIdleWinContent(void);
/*****************************************************************************/
//  Description : cal idle string start point
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL GUI_POINT_T CalStartPoint(MMI_IDLE_DISPLAY_T disp_style, uint16 str_width);
/*****************************************************************************/
//  Description : output time with image
//  Global resource dependence : 
//  Author: xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinTimerWithImage(void);

/*****************************************************************************/
//  Description : output Network name
//  Global resource dependence : 
//  Author: 
//  Note: 通过PhoneApp中提供的接口函数来获得String,然后计算坐标，将其显示出来
/*****************************************************************************/
LOCAL void  OutputIdleWinNetworkName(void);
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : output idle win words
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinWords(void);
#endif
/*****************************************************************************/
//  Description : output date
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinDate(void);
#ifdef MAINLCD_SIZE_128X160
LOCAL void  OutputIdleWinDateAndWeek(void);
#endif
/*****************************************************************************/
//  Description : output counted time
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCountedTime(BOOLEAN is_highlight);

#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : output calendar
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCalendar(void);
#endif

/*****************************************************************************/
//  Description : output week
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinWeek(void);

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : display the mp3 name or not
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleMp3Name(void);

/*****************************************************************************/
//  Description : display the cycle mp3 name
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleCycleMp3Name(void);
#endif
/*****************************************************************************/
//  Description : display the softkey of the idle window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void DisplayIdleWinSoftkey(void);

#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : get tp type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IDLE_DISPLAY_INDEX_E GetTpType(
                                        GUI_POINT_T   tp_point
                                        );
#endif
LOCAL void ResetDisplayNum(void);
LOCAL uint16 GetDisplayNum(void);
LOCAL void SetDisplayNum(void);
#if defined TOUCH_PANEL_SUPPORT
LOCAL void ResetDisplayTouchPanelArea(void);
#endif
LOCAL uint8 s_display_num = 0;
/*****************************************************************************/
//  Description : cal idle string start point
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL GUI_POINT_T CalStartPoint(MMI_IDLE_DISPLAY_T disp_style, uint16 str_width)
{
    GUI_POINT_T     point   =   {0};
	GUI_RECT_T                  mainlcd_rect = MMITHEME_GetFullScreenRect();


    switch(disp_style.text_align)
    {
    case IDLE_ALIGN_LEFT:
        point.x = mainlcd_rect.left +10;//disp_style.text_frame_x;
        point.y = disp_style.text_frame_top;
        break;

    case IDLE_ALIGN_RIGHT:
        point.x = disp_style.text_frame_x - str_width;
        point.y = disp_style.text_frame_top;
        break;

    case IDLE_ALIGN_X_CENTER:
        point.x = (mainlcd_rect.right - str_width) / 2;
        point.y = disp_style.text_frame_top;
        break;

    case IDLE_ALIGN_CUSTOM:
        point.x = disp_style.text_frame_x;
        point.y = disp_style.text_frame_top;
        break;

    default:
        break;
    }

    return point;
}


/*****************************************************************************/
//  Description : output time with image
//  Global resource dependence : 
//  Author: xiangjie
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinTimerWithImage(void)
{
    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_POINT_T                 point               = {0};
    SCI_TIME_T                  time                = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type   = MMIAPISET_GetTimeDisplayType();  
    MMI_IMAGE_ID_T              am_pm_image         = IMAGE_IDLE_TIME_AM;
    uint16                      mmi_main_time_y     = MMI_MAIN_TIME_Y;
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        mmi_main_time_y = MMI_MAIN_TIME_Y_H;
    }
    TM_GetSysTime(&time);

    //12小时制
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        if ((MMISET_TIME_PM_HOUR > time.hour))//AM
        {
            if ((MMISET_TIME_1_HOUR > time.hour))//00:00-1:00
            {
                time.hour = time.hour + MMISET_TIME_PM_HOUR;
            }
        }
        else//PM
        {
            if ((MMISET_TIME_PM_HOUR <= time.hour) && (MMISET_TIME_13_HOUR > time.hour))//12:00-13:00
            {
            }
            else
            {
                time.hour = time.hour-MMISET_TIME_PM_HOUR;
            }
            am_pm_image = IMAGE_IDLE_TIME_PM;
        }   
    }
    
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_TIME);
#if defined TOUCH_PANEL_SUPPORT    
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].type = MMI_MAIN_LCD_TIME;    
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.left = MMI_MAIN_TIME_0_X;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.top  = mmi_main_time_y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right = MMI_MAIN_TIME_AM_PM_X;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.bottom = mmi_main_time_y + MMI_MAIN_TIME_H;
#endif
    IMG_EnableTransparentColor(TRUE);
    point.x = MMI_MAIN_TIME_0_X;
    point.y = mmi_main_time_y;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour/10,&lcd_dev_info);
    point.x = MMI_MAIN_TIME_1_X;
    point.y = mmi_main_time_y;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.hour%10,&lcd_dev_info);
    point.x = MMI_MAIN_TIME_COLON_X;
    point.y = mmi_main_time_y;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_COLON,&lcd_dev_info);
    point.x = MMI_MAIN_TIME_2_X;
    point.y = mmi_main_time_y;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.min/10,&lcd_dev_info);
    point.x = MMI_MAIN_TIME_3_X;
    point.y = mmi_main_time_y;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,IMAGE_IDLE_TIME_0 + time.min%10,&lcd_dev_info);
    IMG_EnableTransparentColor(FALSE);

    if(time_display_type == MMISET_TIME_12HOURS)//12小时制
    {
        point.x = MMI_MAIN_TIME_AM_PM_X;
        point.y = mmi_main_time_y;
    //  idle_disp_style.text_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_TIME);//MMI_WHITE_COLOR;//@zhaohui add，cr102440
#if defined TOUCH_PANEL_SUPPORT
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right += MMI_DEFAULT_FONT_ASCII_WIDTH*2;
#endif
	    GUIRES_DisplayImg(&point,PNULL,PNULL,MAIN_IDLE_WIN_ID,am_pm_image,&lcd_dev_info);   
    }                    
}
/*****************************************************************************/
//  Description : output Network name
//  Global resource dependence : 
//  Author: 
//  Note: 通过PhoneApp中提供的接口函数来获得String,然后计算坐标，将其显示出来
/*****************************************************************************/
LOCAL void  OutputIdleWinNetworkName(void)
{
    MMI_IDLE_DISPLAY_T      idle_disp_style = {0};
    MMI_STRING_T            network_str     = {0};
    GUI_POINT_T             point           = {0};
    GUI_FONT_T              font            = SONG_FONT_8;
    GUI_COLOR_T             color           = 0;
    uint16                  str_width       = 0;
    GUI_LCD_DEV_INFO        lcd_dev_info    = *GetCommonIdlePaintLcdDev();
    BOOLEAN                 is_fly_mode_on  = FALSE;
    // MMI_STRING_T            fly_mode_text   = {0};
    int                     i               = 0;
    // MMISET_DUALSYS_TYPE_E e_dualsys_set = MMIAPISET_GetMultiSysSetting();
#ifdef MONOCHROME_LCD_SUPPORT /* lint_lai */
#else    
    GUI_POINT_T             dis_point       = {0};
#endif 
    uint16                  sim_icon_width  = 0;
    uint16                  sim_icon_height = 0;
    GUISTR_STYLE_T text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T              opn_rect = {0};
    uint16                  lcd_width = 0;
    uint16                  lcd_height = 0;
    uint8                   margin = 2;
    MMISET_IDLE_DISPLAY_T   *idle_mode  =   PNULL;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    wchar *sim_deactivated_str = PNULL;

    const MMI_IMAGE_ID_T sim_img_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_PROMPT_SIM
#else
        IMAGE_IDLE_PROMPT_SIM1,
        IMAGE_IDLE_PROMPT_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_SIM4
#endif
#endif
    };

    const MMI_IMAGE_ID_T no_sim_img_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_PROMPT_NO_SIM
#else
        IMAGE_IDLE_PROMPT_NO_SIM1,
        IMAGE_IDLE_PROMPT_NO_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_NO_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_NO_SIM4
#endif
#endif
    };

    const MMI_IMAGE_ID_T gray_sim_img_id[] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_PROMPT_GRAY_SIM
#else
        IMAGE_IDLE_PROMPT_GRAY_SIM1,
        IMAGE_IDLE_PROMPT_GRAY_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_GRAY_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_PROMPT_GRAY_SIM4
#endif
#endif
    };
    idle_mode = MMIAPISET_GetIdleDisplayMode();
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_NETWORKNAME);
    
    // 读取设置模式的值
    is_fly_mode_on = MMIAPISET_GetFlyMode();
    
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(((0==i)&&(0==idle_mode->visible_flag.item_flag.is_show_network_name))||(GetDisplayNum() >= MMI_COMMON_IDLE_ITEM_MAX_NUM)) continue;
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD ||  defined MMI_MULTI_SIM_SYS_DUAL 
        if(((1==i)&&(0==idle_mode->visible_flag.item_flag.is_show_network_name2))||(GetDisplayNum() >= MMI_COMMON_IDLE_ITEM_MAX_NUM)) continue;       
#endif
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD 
        if(((2==i)&&(0==idle_mode->visible_flag.item_flag.is_show_network_name3))||(GetDisplayNum() >= MMI_COMMON_IDLE_ITEM_MAX_NUM)) continue;
#endif
#if defined MMI_MULTI_SIM_SYS_QUAD
        if(((3==i)&&(0==idle_mode->visible_flag.item_flag.is_show_network_name4))||(GetDisplayNum() >= MMI_COMMON_IDLE_ITEM_MAX_NUM)) continue;
#endif        
        //idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + (i - MN_DUAL_SYS_1));
		idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
        font = idle_disp_style.text_font;

        MMIIDLE_GetIdleTextStyle(&text_style, font, color);

        if (is_fly_mode_on)
        {
            MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &network_str);
        }
        else if (MMIAPIPHONE_GetSimExistedStatus(i))
        {
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting((MN_DUAL_SYS_E)i))
            {
                SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
                network_str.wstr_ptr = (wchar *)opn_spn_name;
                SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
            
                MMIAPIPHONE_GetSimAndServiceString(i, &network_str);
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else
            {
                MMISET_SIM_NAME_T sim_name = {0};
                MMI_STRING_T sim_deactivated = {0};

                sim_name = MMIAPISET_GetSimName(i);

                MMI_GetLabelTextByLang(TXT_SIM_DEACTIVATED, &sim_deactivated);

                //此处不判断是否需要加入空格，统一都多分配一个空格的存储空间(2bytes)    
                sim_deactivated_str = (wchar *)SCI_ALLOC_APP(((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));
                SCI_MEMSET(sim_deactivated_str, 0x00, ((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));

                MMI_WSTRNCPY(sim_deactivated_str,
                                            (sim_deactivated.wstr_len + sim_name.wstr_len + 1),
                                            sim_name.wstr_arr,
                                            sim_name.wstr_len,
                                            sim_name.wstr_len);

                // ASCII 码数据，两个字符串中间追加一个空格。
                if (MMIAPICOM_IsASCIIString(sim_name.wstr_arr,sim_name.wstr_len)
                    && MMIAPICOM_IsASCIIString(sim_deactivated.wstr_ptr ,sim_name.wstr_len))
                {
                    MMIAPICOM_Wstrcat(sim_deactivated_str, L" "); 
                    MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len + 1,
                                            (sim_deactivated.wstr_len + 1),
                                            sim_deactivated.wstr_ptr,
                                            sim_deactivated.wstr_len,
                                            sim_deactivated.wstr_len);
                    
                    network_str.wstr_ptr = (wchar *)sim_deactivated_str;
                    network_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len + 1 ;
                }
                else
                {
                    MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len ,
                                            (sim_deactivated.wstr_len + 1),
                                            sim_deactivated.wstr_ptr,
                                            sim_deactivated.wstr_len,
                                            sim_deactivated.wstr_len);
                    network_str.wstr_ptr = (wchar *)sim_deactivated_str;
                    network_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len ;
                }
            }
#endif
        }
        else
        {

#ifndef MMI_IDLE_MINI_SUPPORT
            MMI_GetLabelTextByLang(TXT_INSERT_SIM, &network_str);
#else
            MMI_GetLabelTextByLang(TXT_INSERT_SIM_MINI, &network_str);  
#endif                

        }

        str_width = GUISTR_GetStringWidth(&text_style,&network_str,text_state);
        point = CalStartPoint(idle_disp_style, str_width);
        #if defined MAINLCD_SIZE_128X160
        //point.y += MMI_IDLE_COMMON_NETWORKNAME_ADJUST;
        #else
        //point.y += MMI_IDLE_COMMON_NETWORKNAME_ADJUST;
        #endif
        if (MMIAPIPHONE_GetSimExistedStatus(i))
        {
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(i))
            {
                image_id = sim_img_id[i];
            }
            else
            {
                image_id = gray_sim_img_id[i];
            }
        }
        else
        {
            image_id = no_sim_img_id[i];
        }

        GUIRES_GetImgWidthHeight(&sim_icon_width,&sim_icon_height,image_id,MAIN_IDLE_WIN_ID);
		if(point.x < sim_icon_width+MMI_IDLE_SIM_ICON_INTERVAL)
		{
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
			point.x = point.x + sim_icon_width+MMI_IDLE_SIM_ICON_INTERVAL;
#else
			point.x = sim_icon_width+MMI_IDLE_SIM_ICON_INTERVAL;
#endif
		}
        opn_rect.left = point.x;
        opn_rect.top = point.y;
        opn_rect.right = lcd_width -1;
        opn_rect.bottom = opn_rect.top + (GUIFONT_GetHeight(text_style.font) + margin);
        
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&opn_rect,
            (const GUI_RECT_T      *)&opn_rect,
            (const MMI_STRING_T     *)&network_str,
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );

        if (PNULL != sim_deactivated_str)
        {
            SCI_FREE(sim_deactivated_str);
            sim_deactivated_str = PNULL;
        }
#if defined TOUCH_PANEL_SUPPORT
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].type = MMI_MAIN_LCD_PLMN + (i - MN_DUAL_SYS_1);
                    
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.left=point.x;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.top=point.y;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.right=point.x+str_width;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.bottom=point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif
           
#ifdef MONOCHROME_LCD_SUPPORT            
#else            
        if (point.x - sim_icon_width >= MMI_IDLE_SIM_ICON_INTERVAL)
        {
            IMG_EnableTransparentColor(TRUE);
            dis_point.x = point.x - sim_icon_width - MMI_IDLE_SIM_ICON_INTERVAL;
            dis_point.y = point.y + MMI_DEFAULT_FONT_HEIGHT - sim_icon_height -1;
                
            GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                MAIN_IDLE_WIN_ID,
                image_id,
                &lcd_dev_info);

            IMG_EnableTransparentColor(FALSE);
#if defined TOUCH_PANEL_SUPPORT
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM1+i].rect.left=point.x - sim_icon_width - MMI_IDLE_SIM_ICON_INTERVAL;
#endif
        }
       SetDisplayNum();
#endif	            
    }
}
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
/*****************************************************************************/
//  Description : output idle win words
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinWords(void)
{
    MMI_IDLE_DISPLAY_T      idle_disp_style = {0};
    GUI_POINT_T             point           = {0};
    GUI_FONT_T              font            = SONG_FONT_8;
    GUI_COLOR_T             color           = 0;
    uint16                  str_width       = 0;
    MMISET_IDLE_DISPLAY_T   *idle_mode      = PNULL;
    GUI_LCD_DEV_INFO        lcd_dev_info    = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT;
    MMI_STRING_T            text   = {0};
    MMI_STRING_T            idle_words   = {0};
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    uint16                  icon_width = 0;/*lint !e529*/
#endif
    
    idle_mode = MMIAPISET_GetIdleDisplayMode();

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_IDLEWORD);    

    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    
    //个性词语内容为空时显示默认个性词语
    if(idle_mode->wstr_len == 0)
    {
		MMI_GetLabelTextByLang(TXT_SET_CUSTOM_WORD, &idle_words);			

		//默认内容的长度超过idle_mode->idle_words的最大长度
        text.wstr_ptr = idle_words.wstr_ptr;
        text.wstr_len = idle_words.wstr_len;
    }
    else
    {
        text.wstr_len = idle_mode->wstr_len;
        text.wstr_ptr = idle_mode->idle_words;
    }

    str_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
	
    point = CalStartPoint(idle_disp_style, str_width);
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
   //display icon
    GUIRES_DisplayImg(&point,
      PNULL,
      PNULL,
      MMK_GetFirstAppletHandle(),
      IMAGE_IDLE_PROMPT_WORDS,
      GetCommonIdlePaintLcdDev());
    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_WORDS,MMK_GetFirstAppletHandle());
    point.x = point.x + icon_width + 7;
#endif
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].type = MMI_MAIN_LCD_CUSTOM_WORD;    
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.left=point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.top=point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.right=point.x+str_width;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.bottom=point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif    
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state
        );
	SetDisplayNum();
}
#endif
LOCAL void ResetDisplayNum(void)
{
	s_display_num = 0;

}
LOCAL uint16 GetDisplayNum(void)
{
    return s_display_num;
}
LOCAL void SetDisplayNum(void)
{
	if (s_display_num<=MMI_COMMON_IDLE_ITEM_MAX_NUM)
	{
		s_display_num++;
	}
}
#if defined TOUCH_PANEL_SUPPORT
LOCAL void ResetDisplayTouchPanelArea(void)
{
	uint8 i;
	for (i=MMI_MAIN_LCD_NONE;i<MMI_COMMON_IDLE_MAX_ITEM;i++)
	{
		s_common_idle_display_rect[i].type = MMI_MAIN_LCD_NONE;    
		s_common_idle_display_rect[i].rect.left = 0;
		s_common_idle_display_rect[i].rect.top = 0;
		s_common_idle_display_rect[i].rect.right = 0;
        s_common_idle_display_rect[i].rect.bottom = 0;	
	}

}
#endif
/*****************************************************************************/
//  Description : handle common idle message
//  Global resource dependence : 
//  Author: jianma
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_CommonHandleMsg ( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
#if defined TOUCH_PANEL_SUPPORT
    GUI_POINT_T             point       = {0};
    static MMI_IDLE_DISPLAY_INDEX_E s_common_down_tp_type  = MMI_MAIN_LCD_NONE;
    MMI_IDLE_DISPLAY_INDEX_E up_tp_type  = MMI_MAIN_LCD_NONE;
    LOCAL BOOLEAN            s_is_process_tp_down   = FALSE;  //是否先处理了 tp down key    
#endif 
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
#if defined TOUCH_PANEL_SUPPORT
        SCI_MEMSET(s_common_idle_display_rect,0,sizeof(s_common_idle_display_rect));
#endif 
        OutIdleWinContent();
        //MMIAPIIDLE_DisplaySoftkey();
        //DisplayIdleWinSoftkey();
#ifdef MMI_ENABLE_DCD
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
#endif
        break;
    case MSG_APP_OK:
#ifdef MMI_MOCOR_SAMPLE
    MMIAPISAMPLE_CreateHelloSprdWin();
    break;
#endif
    case MSG_CTL_OK:
#ifdef MMI_ENABLE_DCD
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
#else
        MMIAPIMENU_CreatMainMenu();
#endif
    break;
    
    case MSG_APP_WEB:
#ifdef CMCC_UI_STYLE
    //MO一键上网标记在web键
    MMIAPISET_EnterMonternetWin();
#else
#ifdef MMI_ENABLE_DCD
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
#else
        MMIAPIMENU_CreatMainMenu();
#endif     
#endif
        break;
        
    case MSG_KEYREPEAT_UP:
    case MSG_APP_UP:
        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_UP);
        break;

    case MSG_KEYREPEAT_DOWN:
    case MSG_APP_DOWN:
        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_DOWN);
        break;

    case MSG_KEYREPEAT_LEFT:
    //case MSG_APP_LEFT:
    case MSG_KEYUP_LEFT://bugzilla 4216
        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_LEFT);
        break;

    case MSG_KEYREPEAT_RIGHT:
    //case MSG_APP_RIGHT:
    case MSG_KEYUP_RIGHT://bugzilla 4216
        MMIAPISET_OpenDirectionKeyWin(win_id,MMISET_DIRKEY_RIGHT);
        break;
#if defined TOUCH_PANEL_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        s_is_process_tp_down = TRUE;
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);    
#ifdef MMI_SPRITE_ENABLE
        if (MMIIDLE_PressDownSprite(point.x, point.y))
        {
            break;
        }
#endif
          s_common_down_tp_type = GetTpType(point);
// 	     if(MMI_MAIN_LCD_COUNTEDTIME == s_common_down_tp_type)
//          {
//             //highlight效果
//             OutputIdleWinCountedTime(TRUE);
//          }  
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if(!s_is_process_tp_down)
            break;//没有受到tp down ,不处理up

        s_is_process_tp_down = FALSE;//reset 
        
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
#ifdef MMI_SPRITE_ENABLE
        if (MMIIDLE_PressUpSprite(point.x, point.y))
        {
            s_common_down_tp_type = MMI_MAIN_LCD_NONE;
            break;
        }
#endif

        up_tp_type = GetTpType(point);
        if (s_common_down_tp_type != up_tp_type)
        {
            s_common_down_tp_type = MMI_MAIN_LCD_NONE;
            break;
        }     
		
        switch(up_tp_type) 
        {
        case MMI_MAIN_LCD_TIME:
            MMIAPISET_SetTime();
            break;
            
        case MMI_MAIN_LCD_WEEK:
        case MMI_MAIN_LCD_DATE:
            MMIAPISET_SetDate();
            //MMIAPISET_OpenTimeOrDateWindow((uint32*)MMISET_SET_DATE_WIN_TAB, FALSE);
            break;

        case MMI_MAIN_LCD_MP3:
            if(FM_STOP != MMIAPIFM_GetStatus())
            {
                MMIAPIFM_OpenMainWin();
            }
#ifdef MMI_AUDIO_PLAYER_SUPPORT
            else if(MMIAPIMP3_IsPlayerActive())
            {
                MMIAPIMP3_OpenMp3Player();
            }
#endif
#ifdef KURO_SUPPORT
            else if(MMIAPIKUR_IsPlayerActive())
            {
                MMIAPIKUR_OpenKurPlayer();
            }
#endif            
            break;

        case MMI_MAIN_LCD_COUNTEDTIME:
			 MMIAPICT_OpenCoutimeRunningWin();// 打开倒记时窗口
            break;

#ifdef CALENDAR_SUPPORT
        case MMI_MAIN_LCD_CALENDAR:
            MMIAPIIDLE_OpenScheduleWin(win_id);
            break;
#endif

        case MMI_MAIN_LCD_CUSTOM_WORD:
            MMIAPISET_OpenSetIdleDisWin();
            break;

        case MMI_MAIN_LCD_PLMN:
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            MMIAPIIDLE_OpenSelNetWorkWin(MN_DUAL_SYS_1);
#else
            MMIAPISET_OpenMultiSimSettingsWindow();
#endif
            break;

        #ifndef MMI_MULTI_SIM_SYS_SINGLE
        case MMI_MAIN_LCD_PLMN2:
            MMIAPISET_OpenMultiSimSettingsWindow();
            break;
            
         #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)   
         case MMI_MAIN_LCD_PLMN3:
            MMIAPISET_OpenMultiSimSettingsWindow();
            break;
         #endif /*defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD) */
         
         #if defined(MMI_MULTI_SIM_SYS_QUAD)
         case MMI_MAIN_LCD_PLMN4:
            MMIAPISET_OpenMultiSimSettingsWindow();
            break;
         #endif /* MMI_MULTI_SIM_SYS_QUAD */
         
         #endif /* MMI_MULTI_SIM_SYS_SINGLE */

        case MMI_MAIN_LCD_LEFT_SOFTKEY:
            MMK_SendMsg(win_id,MSG_APP_OK,PNULL);
            break;

        case MMI_MAIN_LCD_RIGHT_SOFTKEY:
            MMK_SendMsg(win_id,MSG_APP_CANCEL,PNULL);
            break;
        default:
            MMIAPIMENU_CreatMainMenu();
            break;
        } 
        s_common_down_tp_type = MMI_MAIN_LCD_NONE;
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_OPEN_WINDOW:
    case MSG_GET_FOCUS:
        GUIWIN_CreateSoftkeyDyn(win_id, TXT_COMM_DIALPAD, 0, TXT_ENG_TITLE);
#ifdef UI_MULTILAYER_SUPPORT        
        if (UILAYER_IsLayerActive(MMITHEME_GetDefaultLcdDev()))
        {
            UILAYER_Clear(MMITHEME_GetDefaultLcdDev());
        }
        ReleaseCommonIdlePaintLayer();
        ReleaseCommonIdleStbLayer(win_id);
        CreateCommonIdlePaintLayer(win_id);
        CreateCommonIdleStbLayer(win_id);
#endif        
        break;
    case MSG_CLOSE_WINDOW:
    case MSG_LOSE_FOCUS:
#ifdef UI_MULTILAYER_SUPPORT        
        ReleaseCommonIdlePaintLayer();
        ReleaseCommonIdleStbLayer(win_id);
#endif        
        break;
    case MSG_IDLE_UPDATE_COUNTEDTIME:
    case MSG_IDLE_UPDATE_MP3: 
    case MSG_IDLE_UPDATE_DATETIME:
    case MSG_STK_SET_UP_IDLE_MODE:
#ifdef UI_MULTILAYER_SUPPORT        
        OutIdleWinContent();
        DisplayIdleWinSoftkey();
#else        
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
#endif
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result ;
}

#ifdef MAINLCD_SIZE_128X160
LOCAL void  OutputIdleWinDateAndWeek(void)
{

    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_POINT_T                 point               = {0};
    uint8                       datestr[30]         = {0};
    wchar                       datewstr[30]        = {0};
    wchar                       wstr_buf[30]        = {0};
    GUI_FONT_T                  font                = SONG_FONT_8;
    GUI_COLOR_T                 color               = 0;
    uint16                      str_width           = 0;
    MMI_STRING_T                m_string            = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();  
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T            text   = {0};
    SCI_DATE_T date = {0};
    MMI_TEXT_ID_T week_table[] = {
                                        TXT_IDLE_SHORT_SUNDAY,
                                        TXT_IDLE_SHORT_MONDAY,
                                        TXT_IDLE_SHORT_TUESDAY,
                                        TXT_IDLE_SHORT_WEDNESDAY,
                                        TXT_IDLE_SHORT_THURSDAY,
                                        TXT_IDLE_SHORT_FRIDAY,
                                        TXT_IDLE_SHORT_SATURDAY
    };
#ifdef MULTI_LAN_SUPPORT_DE
	MMI_TEXT_ID_T short_week_table[] = {
									TXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
									TXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
									TXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
									TXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
									TXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
									TXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
									TXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
	};
#endif
	
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_DATE);
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_DATE);
    TM_GetSysDate(&date);
#ifdef MULTI_LAN_SUPPORT_DE
	if((MMISET_LANGUAGE_SIMP_CHINESE == MMISET_GetCurrentLanguageType())||
		MMISET_LANGUAGE_TRAD_CHINESE == MMISET_GetCurrentLanguageType())
	{
		MMI_GetLabelTextByLang(short_week_table[date.wday], &m_string);
	}
	else
	{
		MMI_GetLabelTextByLang(week_table[date.wday], &m_string);
	}
#else
    MMI_GetLabelTextByLang(week_table[date.wday], &m_string);      
#endif
    MMIAPISET_GetCurrentDateStr(datestr,'-',sizeof(datestr));
    strncat(datestr," ",2);/*lint !e64*/
    MMIAPICOM_StrToWstr(datestr, wstr_buf);    
    MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    text.wstr_ptr = datewstr;
    MMIAPICOM_Wstrncpy(text.wstr_ptr,wstr_buf,MMIAPICOM_Wstrlen(wstr_buf));
    MMIAPICOM_Wstrncat(text.wstr_ptr, m_string.wstr_ptr,m_string.wstr_len);     
    text.wstr_len = (uint16)MMIAPICOM_Wstrlen(text.wstr_ptr);
    str_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
   
    point = CalStartPoint(idle_disp_style, str_width);
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state
        );

}

#endif
/*****************************************************************************/
//  Description : output date
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinDate(void)
{
    MMI_IDLE_DISPLAY_T          idle_disp_style     = {0};
    GUI_POINT_T                 point               = {0};
    //SCI_DATE_T                  date                = {0};
#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/04
    SCI_TIME_T         	 	  time = {0};
    uint8                         datestr[21]         = {0};
    wchar                       datewstr[21]        = {0};
#else    
    uint8                       datestr[16]         = {0};
    wchar                       datewstr[16]        = {0};
#endif 
    uint16                      len                 = {0}; 
    GUI_FONT_T                  font                = SONG_FONT_8;
    GUI_COLOR_T                 color               = 0;
    uint16                      str_width           = 0;
    GUI_LCD_DEV_INFO            lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    //MMISET_DATE_DISPLAY_TYPE_E  date_display_type   = MMISET_GetDateDisplayType();    
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T            text   = {0};


    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_DATE);
    if(MMITHEME_IsMainScreenLandscape())
    {
        
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
		idle_disp_style.text_frame_top = IDLE_DATE_TOP_H;
        idle_disp_style.text_frame_x = MMITHEME_GetClientRectWidth()-IDLE_DATE_LEFT_MARGIN;
#elif defined MAINLCD_SIZE_176X220 
		idle_disp_style.text_frame_top = IDLE_DATE_TOP_H;
        idle_disp_style.text_frame_x = IDLE_DATE_LEFT_H;
#else
        //idle_disp_style.text_frame_x = IDLE_DATE_LEFT_H ;
#endif
    }
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_DATE);
      
    MMIAPISET_GetCurrentDateStr(datestr,'-',16);

#ifdef DPHONE_SUPPORT //added by taiping.lai 2011/01/04
	TM_GetSysTime(&time);
	len = strlen((char *)datestr);
	datestr[len++] = ' ';
	MMIAPISET_FormatTimeStrByTime(time.hour, time.min, datestr + len, sizeof(datestr) - len);
	len = strlen((char *)datestr);
    MMI_STRNTOWSTR( datewstr, 20, datestr, 20, len );
#else
    len = strlen((char *)datestr);
    MMI_STRNTOWSTR( datewstr, 16, datestr, 16, len );
#endif 
    MMIIDLE_GetIdleTextStyle(&text_style, font, color);

    text.wstr_len = len;
    text.wstr_ptr = datewstr;

    str_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
   
    point = CalStartPoint(idle_disp_style, str_width);
#if defined TOUCH_PANEL_SUPPORT  
    if (point.x <s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right)
    {
#ifdef MAINLCD_SIZE_176X220
        //point.x = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right+5;
#elif  defined (MAINLCD_SIZE_128X160)
        //point.x = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right;
#else
        point.x = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM6].rect.right+10;
#endif        
    }

    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].type = MMI_MAIN_LCD_DATE;
   
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.right  = point.x+str_width;
	s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif    
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state
        );
}

/*****************************************************************************/
//  Description : output counted time
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCountedTime(BOOLEAN is_highlight)
{
#ifdef COUNTEDTIME_SUPPORT
    MMI_IDLE_DISPLAY_T          idle_disp_style = {0};
    GUI_POINT_T                 point           = {0};
    SCI_TIME_T                  time            = {0};
    uint8                       timestr[20]     = {0};
    //wchar                         disptime[20] = {0};
    uint8                       disptime_len    = 0;
    MMI_STRING_T                tmp_prompt      = {0};
    MMI_STRING_T                disp_str        = {0};

    GUI_FONT_T                  font            = SONG_FONT_8;
    GUI_COLOR_T                 color           = 0;
    uint16                      str_width       = 0;
    GUI_LCD_DEV_INFO            lcd_dev_info    = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T              text_style = {0};/*lint !e64*/
    GUISTR_STATE_T              text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
//@for light function modify
#if defined MAINLCD_SIZE_240X320
    uint16                      icon_width = 0;
#endif

    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_COUNTTIME);

    //"倒计时"
    MMI_GetLabelTextByLang(TXT_TOOLS_COUNTEDTIME, &tmp_prompt);

    //XX:XX
    time = MMIAPICT_GetCurTime();
//     if (MMIAPICT_IsLessThanOnemin())
//     {
//         sprintf((char*)timestr,": %02d:%02d", time.min, time.sec);//小于1分钟时显示"分分:秒秒"      
//     }
//     else
//     {
//         sprintf((char*)timestr,": %02d:%02d", time.hour, time.min);//大于等于1分钟时显示"时时:分分"
//     }
    sprintf((char*)timestr,": %02d:%02d:%02d", time.hour,time.min, time.sec);
    disptime_len = strlen((char *)timestr); 

    // the total string length
    disp_str.wstr_len =  disptime_len + tmp_prompt.wstr_len ;

    // allocate memory
    disp_str.wstr_ptr = SCI_ALLOC_APP((disp_str.wstr_len + 1) * sizeof(wchar) );
    
    if (PNULL == disp_str.wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMIIDLE OutputIdleWinCountedTime PNULL == disp_str.wstr_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_CSTYLE_988_112_2_18_2_23_42_21,(uint8*)"");
        return;
    }
    //disp_str.is_ucs2 = tmp_prompt.is_ucs2;
    
    //copy the string
    MMIAPICOM_Wstrncpy(
        disp_str.wstr_ptr, 
        tmp_prompt.wstr_ptr,  
        tmp_prompt.wstr_len
                );

    // copy the num
    MMI_STRNTOWSTR(
                &disp_str.wstr_ptr[tmp_prompt.wstr_len], 
                disptime_len,
                timestr,  
                disptime_len,
                disptime_len
                );

    if (is_highlight)
    {
        MMIIDLE_GetIdleTextStyle(&text_style, font, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_COUNTTIME));
    }
    else
    {
        MMIIDLE_GetIdleTextStyle(&text_style, font, color);
    }

    str_width = GUISTR_GetStringWidth(&text_style,&disp_str,text_state);
    
    point = CalStartPoint(idle_disp_style, str_width);

//@for light function modify
#if defined MAINLCD_SIZE_240X320 
   //display icon
    GUIRES_DisplayImg(&point,
      PNULL,
      PNULL,
      MMK_GetFirstAppletHandle(),
      IMAGE_IDLE_PROMPT_COUNT_DOWN,
      GetCommonIdlePaintLcdDev());
    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_COUNT_DOWN,MMK_GetFirstAppletHandle());
    point.x = point.x + icon_width + 7;
#endif
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].type        = MMI_MAIN_LCD_COUNTEDTIME;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.right  = point.x+str_width;
	s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM3].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif

    if (is_highlight)
    {
        GUISTR_DrawTextToLCDSingleLine( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_POINT_T      *)&point,
            (const MMI_STRING_T     *)&disp_str,
            &text_style,
            text_state
            );
    }
    else
    {
        GUISTR_DrawTextToLCDSingleLine( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_POINT_T      *)&point,
            (const MMI_STRING_T     *)&disp_str,
            &text_style,
            text_state
            );
    }

    if (PNULL != disp_str.wstr_ptr)
    {
        SCI_FREE(disp_str.wstr_ptr);
        disp_str.wstr_ptr = PNULL;
    }
	SetDisplayNum();
#endif
}

#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : output calendar
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void OutputIdleWinCalendar(void)
{
    MMI_IDLE_DISPLAY_T			idle_disp_style = {0};   
    GUI_POINT_T		            point           = {0};
    GUI_FONT_T                  font            = SONG_FONT_8;
    uint16                      str_width       = 0;
	GUI_RECT_T                  mainlcd_rect = MMITHEME_GetFullScreenRect();
//@for light function modify
#if defined TOUCH_PANEL_SUPPORT || defined MAINLCD_SIZE_240X320
    uint16                      icon_width = 0;/*lint !e529*/
#endif
    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
    font = idle_disp_style.text_font; 
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
    point.x = idle_disp_style.text_frame_x;
    point.y = idle_disp_style.text_frame_top;

    GUIRES_DisplayImg(&point,
        PNULL,
        PNULL,
        MMK_GetFirstAppletHandle(),
        IMAGE_IDLE_PROMPT_SCHEDULE,
        GetCommonIdlePaintLcdDev());

    GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_SCHEDULE,MMK_GetFirstAppletHandle());
    point.x = idle_disp_style.text_frame_x + icon_width + 7;
    point.y = idle_disp_style.text_frame_top;
   
    str_width = MMIIDLE_DisplaySchedule(GetCommonIdlePaintLcdDev(),MMI_DIRECTION_LEFT,mainlcd_rect.right,&point,font);
#else
    point.x = idle_disp_style.text_frame_x;
    point.y = idle_disp_style.text_frame_top;

    str_width = MMIIDLE_DisplaySchedule(GetCommonIdlePaintLcdDev(),MMI_DIRECTION_HMIDDLE,mainlcd_rect.right,&point,font);
#endif

#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].type        = MMI_MAIN_LCD_CALENDAR;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.left   = idle_disp_style.text_frame_x+ icon_width+ 7;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.right  = point.x+str_width;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM4].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif 
    SetDisplayNum()	;
}
#endif

/*****************************************************************************/
//  Description : output week
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  OutputIdleWinWeek(void)
{
#ifdef DPHONE_SUPPORT
	return;
#else  /* lint_lai */ 
    MMI_IDLE_DISPLAY_T  idle_disp_style = {0};
    GUI_POINT_T         point       = {0};
    SCI_DATE_T          date        = {0};
    GUI_FONT_T          font        = SONG_FONT_8;
    GUI_COLOR_T         color       = 0;
    uint16              str_width   = 0;
    MMI_STRING_T        str_week    = {0};
#if defined TOUCH_PANEL_SUPPORT
	GUI_RECT_T          mainlcd_rect = MMITHEME_GetFullScreenRect();
#endif
    MMI_TEXT_ID_T       week_table[] = {
                                       TXT_SHORT_IDLE_SUNDAY,
                                       TXT_SHORT_IDLE_MONDAY,
                                       TXT_SHORT_IDLE_TUESDAY,
                                       TXT_SHORT_IDLE_WEDNESDAY,
                                       TXT_SHORT_IDLE_THURSDAY,
                                       TXT_SHORT_IDLE_FRIDAY,
                                       TXT_SHORT_IDLE_SATURDAY};
    GUI_LCD_DEV_INFO    lcd_dev_info = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
		


    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_WEEK);
    if(MMITHEME_IsMainScreenLandscape())
    {
        
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
		idle_disp_style.text_frame_top = IDLE_WEEK_TOP_H;
        idle_disp_style.text_frame_x = MMITHEME_GetClientRectWidth() - 6;
#elif defined MAINLCD_SIZE_176X220  
        idle_disp_style.text_frame_top = IDLE_WEEK_TOP_H;
        idle_disp_style.text_frame_x = IDLE_WEEK_LEFT_H;
#else
        //idle_disp_style.text_frame_x = IDLE_WEEK_LEFT_H;
#endif
    }
    font = idle_disp_style.text_font;
    color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_WEEK);

    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday],&str_week);

    MMIIDLE_GetIdleTextStyle(&text_style, font, color);

    str_width = GUISTR_GetStringWidth(&text_style,&str_week,text_state);
    point = CalStartPoint(idle_disp_style, str_width);
#if defined TOUCH_PANEL_SUPPORT    
    if (point.x < s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.right)
    {
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
        point.x = MIN(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.right +5,mainlcd_rect.right-str_width);
#else
        point.x = MIN(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM7].rect.right +10,mainlcd_rect.right-str_width-7);
#endif        
    }
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].type        = MMI_MAIN_LCD_WEEK;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].rect.right  = MIN(point.x+str_width,mainlcd_rect.right);
	s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM8].rect.bottom = point.y+GUI_GetFontHeight(font,UNICODE_HANZI);
#endif    
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&point,
        (const MMI_STRING_T     *)&str_week,
        &text_style,
        text_state
        );
#endif    
}

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : display the mp3 name or not
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleMp3Name(void)
{
//    IDLE_WIN_INFO_T       *idlewin_info_ptr = MMIIDLE_GetIdleWinInfo();
    MMISET_IDLE_DISPLAY_T   *idle_set_ptr  =   PNULL;
    idle_set_ptr = MMIAPISET_GetIdleDisplayMode();
//    if (idlewin_info_ptr->is_display_mp3)//显示mp3 name
//    {
        //display mp3 name
        if (g_idle_cur_mp3_name.is_scroll)
        {
            MMIIDLECOM_StartIdleMp3Timer();
            DisplayIdleCycleMp3Name();
        }
        else
        {
            MMI_IDLE_DISPLAY_T  idle_disp_style = {0};
            GUI_FONT_T          font            = SONG_FONT_8;
            GUI_COLOR_T         color           = 0;
            GUI_POINT_T         point           = {0};
            GUI_LCD_DEV_INFO    lcd_dev_info    = *GetCommonIdlePaintLcdDev();
            uint8               string_width    =0;
            GUISTR_STYLE_T      text_style = {0};/*lint !e64*/
            GUISTR_STATE_T      text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT;
            MMI_STRING_T        text = {0};
            uint16              lcd_width = 0; // lcd宽度
            uint16              lcd_height = 0; // lcd高度
            uint16              music_name_max_width = 0; //MMI_MAINSCREEN_MP3_WIDTH;
            wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};
//@for light function modify
#if defined MAINLCD_SIZE_240X320
            MMI_IMAGE_ID_T      icon_id  = IMAGE_IDLE_PROMPT_PLAY;/*lint !e529*/
            uint16              icon_width = 0;/*lint !e529*/
#endif
            text.wstr_ptr = mp3_name; 
			idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());

            if (MMIAPIMP3_IsPlayerActive() 
                && idle_set_ptr->visible_flag.item_flag.is_show_mp3&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
            {
                #ifdef FM_SUPPORT
                if ((FM_PLAYING == MMIAPIFM_GetStatus()) 
                    && idle_set_ptr->visible_flag.item_flag.is_show_fm&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
                {
                    //display fm
                    MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
                }
                else
                #endif
                {
                    MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
                }
                 SetDisplayNum();   
            }  
            else
            { 
#ifdef FM_SUPPORT
                if ((FM_STOP != MMIAPIFM_GetStatus()) 
                    && idle_set_ptr->visible_flag.item_flag.is_show_fm && (GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
                {
                    //display fm
                    MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
					SetDisplayNum();
                }
				
#endif
            }
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
            music_name_max_width = (lcd_width-MMI_MAINSCREEN_MP3_LEFT-MMI_MAINSCREEN_MP3_LEFT);
           
            font = idle_disp_style.text_font;
            color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_MP3);
            //text.wstr_len = g_idle_cur_mp3_name.length;
            //text.wstr_ptr = g_idle_cur_mp3_name.mp3_name;
            g_idle_cur_mp3_name.str_width = GUI_CalculateStringPiexlNum(text.wstr_ptr,
                text.wstr_len,
                                                                    font,0);
            MMIIDLE_GetIdleTextStyle(&text_style, font, color);
//@for light function modify
#if defined MAINLCD_SIZE_240X320 
        if((idle_set_ptr->visible_flag.item_flag.is_show_mp3&&(MMIMP3_PLAYER_STATE_NONE!=MMIAPIMP3_GetPlayerState())) 
#ifdef FM_SUPPORT
        ||(idle_set_ptr->visible_flag.item_flag.is_show_fm &&((FM_STOP != MMIAPIFM_GetStatus())))&& (GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM)
#endif
          )
        {
            point.x = idle_disp_style.text_frame_x; 
            point.y = idle_disp_style.text_frame_top;
            if(MMIAPIMP3_IsPlayerPlaying() || (FM_PLAYING == MMIAPIFM_GetStatus()))
            {
                icon_id = IMAGE_IDLE_PROMPT_PLAY;
            }
            else if(MMIAPIMP3_IsPlayerActive() || (FM_SUSPENDED == MMIAPIFM_GetStatus()))
            {
                icon_id = IMAGE_IDLE_PROMPT_PAUSE;
            }
            //display icon
            GUIRES_DisplayImg(&point,
                PNULL,
                PNULL,
                MMK_GetFirstAppletHandle(),
                icon_id,
                GetCommonIdlePaintLcdDev());
            GUIRES_GetImgWidthHeight(&icon_width,PNULL, IMAGE_IDLE_PROMPT_SCHEDULE,MMK_GetFirstAppletHandle());
            point.x = point.x + icon_width + 7;
        }
#else
            point.x = ((music_name_max_width - g_idle_cur_mp3_name.str_width)>>1)+MMI_MAINSCREEN_MP3_LEFT;            
            point.y = idle_disp_style.text_frame_top;
#endif
#if defined TOUCH_PANEL_SUPPORT            
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].type = MMI_MAIN_LCD_MP3;
            
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.left = point.x;
#endif
            string_width = GUISTR_GetStringWidth(&text_style,&text,text_state);
#if defined TOUCH_PANEL_SUPPORT
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.right  = point.x + string_width;
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.top    = point.y;
            s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.bottom = point.y + GUI_GetFontHeight(font,UNICODE_HANZI);
#endif
            GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T      *)&point,
                (const MMI_STRING_T     *)&text,
                &text_style,
                text_state
                );
			
        }

//    }
}

/*****************************************************************************/
//  Description : display the cycle mp3 name
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
LOCAL void DisplayIdleCycleMp3Name(void)
{
    uint16              str_index           = 0;
    MMI_IDLE_DISPLAY_T  idle_disp_style     = {0};
    GUI_RECT_T          rect                = {0};
    GUI_FONT_T          font                = SONG_FONT_8;
    GUI_COLOR_T         color               = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info        = *GetCommonIdlePaintLcdDev();
    GUISTR_STYLE_T      text_style          = {0};/*lint !e64*/
#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)	
    GUISTR_STATE_T      text_state          = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE |GUISTR_STATE_EFFECT;
#else
    GUISTR_STATE_T      text_state          = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT;
#endif		
    MMI_STRING_T        text                = {0};
	GUI_RECT_T                  mainlcd_rect = MMITHEME_GetFullScreenRect();
    wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};
    MMISET_IDLE_DISPLAY_T   *idle_set_ptr  =   PNULL;
    idle_set_ptr = MMIAPISET_GetIdleDisplayMode();
    text.wstr_ptr = mp3_name;
	
	MMIIDLE_GetIdleTextStyle(&text_style, font, color);	
#ifdef FM_SUPPORT    
	if (FM_PLAYING == MMIAPIFM_GetStatus()&& idle_set_ptr->visible_flag.item_flag.is_show_fm&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
	{
		MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
		idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
		font = idle_disp_style.text_font;
		color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_FM);
        SetDisplayNum();
	}
	else 
#endif     
             if(MMIAPIMP3_IsPlayerActive() 
                && idle_set_ptr->visible_flag.item_flag.is_show_mp3&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
	{
		MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
		idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_PLMN + GetDisplayNum());
		font = idle_disp_style.text_font;
		color = MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_MP3);
        SetDisplayNum();
    }

    //显示mp3 name,防止定时器到时之前，停止mp3播放，Mp3名字长度为0，导致出错!
    if (MMIIDLE_GetIdleWinInfo()->is_display_mp3)
    {
        //set rect
        rect.left = MMI_MAINSCREEN_MP3_LEFT;
        rect.right = mainlcd_rect.right - MMI_MAINSCREEN_MP3_LEFT;
        rect.top = idle_disp_style.text_frame_top;
        rect.bottom = rect.top + GUI_GetStringHeight(font, text.wstr_ptr, 
                            text.wstr_len);
#if defined TOUCH_PANEL_SUPPORT
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].type     = MMI_MAIN_LCD_MP3;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.left=rect.left;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.right=rect.right;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.top=rect.top;
        s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM5].rect.bottom=rect.bottom;
#endif


        str_index = g_idle_cur_mp3_name.str_index;

        if (str_index < MMIMP3_NAME_MAX_LEN)
        {
            text.wstr_len = (uint16)(text.wstr_len-str_index);
            text.wstr_ptr = text.wstr_ptr+str_index;

            text_style.font = font ;

            GUISTR_DrawTextToLCDInRect( 
                                       (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                       (const GUI_RECT_T      *)&rect,       //the fixed display area
                                       (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                                       (const MMI_STRING_T     *)&text,
                                       &text_style,
                                       text_state,
                                       GUISTR_TEXT_DIR_AUTO
                                       );
        }
    }
      // SetDisplayNum();
}
#endif

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (KURO_SUPPORT)|| defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : set idle mp3 name
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBgPlayName(
                                BOOLEAN     is_display,
                                MMIBGPLAY_TYPE_E bg_type
                                )
{
    MMI_IDLE_DISPLAY_T  idle_disp_style = {0};
    GUI_FONT_T          font            = SONG_FONT_8;
    MMI_STRING_T        text            = {0};
    uint16              lcd_width = 0; // lcd宽度
    uint16              lcd_height = 0; // lcd高度
    uint16              music_name_max_width = 0; 
    wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};


    idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_MAIN_LCD_MP3);
    font = idle_disp_style.text_font;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    music_name_max_width = lcd_width - MMI_MAINSCREEN_MP3_LEFT - MMI_MAINSCREEN_MP3_LEFT;    
    
    //显示当前播放的mp3 name,包括更换当前的mp3 name  

    if (is_display)
    {
        MMIIDLE_GetIdleWinInfo()->is_display_mp3 = is_display;
       
        text.wstr_ptr = mp3_name;

        if(MMIBGPLAY_TYPE_FM == bg_type)
        {
            //get fm name
            MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
        }
#ifdef KURO_SUPPORT        
        else if(MMIBGPLAY_TYPE_KURO == bg_type)
        {
            MMIAPIKUR_GetCurKurName(&text, MMIMP3_NAME_MAX_LEN);
        }
#endif         
#ifdef MMI_AUDIO_PLAYER_SUPPORT        
        else if(MMIBGPLAY_TYPE_MP3 == bg_type)
        {
            MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
        }
#endif        
        //set global variable
        //g_idle_cur_mp3_name.is_ucs2 = text.is_ucs2;
        //g_idle_cur_mp3_name.length = text.wstr_len;
        g_idle_cur_mp3_name.str_index = 0;
        
        //is scroll
        g_idle_cur_mp3_name.str_width = GUI_CalculateStringPiexlNum(text.wstr_ptr,
                                                                    text.wstr_len,
                                                                    font,0);
        if (music_name_max_width < g_idle_cur_mp3_name.str_width)
        {
            g_idle_cur_mp3_name.is_scroll = TRUE;
        }
        else
        {
            g_idle_cur_mp3_name.is_scroll = FALSE;
        }
    }
    else
    {
        if((MMIBGPLAY_TYPE_FM == bg_type && !MMIAPIMP3_IsPlayerActive() 
#ifdef KURO_SUPPORT
		&& !MMIAPIKUR_IsPlayerActive()
#endif
)
           || (MMIBGPLAY_TYPE_MP3 == bg_type && FM_STOP == MMIAPIFM_GetStatus() 
#ifdef KURO_SUPPORT
	 	&& !MMIAPIKUR_IsPlayerActive()
#endif
			)
           || (MMIBGPLAY_TYPE_KURO == bg_type && FM_STOP == MMIAPIFM_GetStatus() && !MMIAPIMP3_IsPlayerActive()))
        {
            //no bg play
            MMIIDLE_GetIdleWinInfo()->is_display_mp3 = is_display;
            SCI_MEMSET(&g_idle_cur_mp3_name,0,sizeof(IDLE_MP3_NAME_INFO));
        }
    }

    //if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
    {
        MMIIDLE_SendMsgToIdleWin(MSG_IDLE_UPDATE_MP3, PNULL);
    }
}
#endif
/*****************************************************************************/
//  Description : display the  content
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void OutIdleWinContent(void)
{
    MMISET_IDLE_DISPLAY_T   *idle_mode  =   PNULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_common_idle_paint_layer_handle))
        {
            UILAYER_Clear(&s_common_idle_paint_layer_handle);
        }
#endif
    idle_mode = MMIAPISET_GetIdleDisplayMode();
	    ResetDisplayNum();
#if defined TOUCH_PANEL_SUPPORT
		ResetDisplayTouchPanelArea();
#endif
    //show time and date
    if ( idle_mode->visible_flag.item_flag.is_show_time_and_date)
    {    
#if defined(DPHONE_SUPPORT) 
#else        
        OutputIdleWinTimerWithImage();  
#endif
#ifdef MAINLCD_SIZE_128X160
        OutputIdleWinDateAndWeek();
#else
        OutputIdleWinDate();                     
        OutputIdleWinWeek();
#endif
    }

    //show network name
        OutputIdleWinNetworkName();

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
		if ((//CR134667
#ifdef MMI_AUDIO_PLAYER_SUPPORT
			(idle_mode->visible_flag.item_flag.is_show_mp3)
#else
			0
#endif
#ifdef FM_SUPPORT
			||(idle_mode->visible_flag.item_flag.is_show_fm)
#endif		
			)&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
        {
			DisplayIdleMp3Name();
		}
#endif 
    //show counted time

#ifdef COUNTEDTIME_SUPPORT
		if (MMIAPICT_IsShowIdleCoutime()&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM)&&idle_mode->visible_flag.item_flag.is_show_countdown)
		{
			OutputIdleWinCountedTime(FALSE);
		}
		else
#endif           
		{
	#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT        
			//show idle words
			if ( idle_mode->visible_flag.item_flag.is_show_idle_words && (GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
			{
				OutputIdleWinWords();
			}
	#endif        
		}


#ifdef MMI_MULTI_SIM_SYS_QUAD
    if(
    (!MMIAPIMP3_IsPlayerActive())
    #ifdef FM_SUPPORT
     && (!MMIAPIFM_IsFmActive())
    #endif
    #ifdef KURO_SUPPORT
     && (!MMIAPIKUR_IsPlayerActive())
    #endif
    )
#endif    
    {
#ifdef CALENDAR_SUPPORT
        //show calendar
        if (idle_mode->visible_flag.item_flag.is_show_today_schedule&&(GetDisplayNum() < MMI_COMMON_IDLE_ITEM_MAX_NUM))
        {
            OutputIdleWinCalendar();
        }
#endif
    }
#ifdef MMI_IDLE_STKTEXT_SUPPORT
   if(idle_mode->visible_flag.item_flag.is_show_stk_idle_text)
    {
		GUI_LCD_DEV_INFO    lcd_dev_info    = *GetCommonIdlePaintLcdDev();
		MAIN_DrawStkIdleModeText(lcd_dev_info.block_id);
    }
#endif

}

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : get idle mp3 info
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_MP3_NAME_INFO* MMIIDLECOM_GetIdleMp3Info(void)
{
    return &g_idle_cur_mp3_name;
}
#endif

/*****************************************************************************/
//  Description : display the softkey of the idle window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void DisplayIdleWinSoftkey(void)
{
    GUI_POINT_T             point                = {0};
	GUI_LCD_DEV_INFO        lcd_dev_info             = *GetCommonIdlePaintLcdDev();
	GUI_RECT_T              mainlcd_rect             = MMITHEME_GetFullScreenRect();

#if defined (MMI_RES_ORIENT_PORTRAIT) && defined (MAINLCD_SIZE_128X160)//arvin zhang added for idle softkey icon    	
	uint16                  idle_softkey_icon_width  = 0;
    uint16                  idle_softkey_icon_height = 0; 
	uint32                  i                        = 0;
	MMI_IMAGE_ID_T          idle_softkey_icon[MMIIDLE_SOFTKEY_ICON_MAX] = {0};
#ifndef MMI_COMMON_IDLE_IMG_SOFTKEY_SUPPORT	
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
		idle_softkey_icon[0] = IMAGE_IDLE_ICON_MENU;
		idle_softkey_icon[1] = IMAGE_IDLE_ICON_SOS;
    }
	else
#endif
	{
		idle_softkey_icon[0] = IMAGE_IDLE_ICON_MENU;
		idle_softkey_icon[1] = IMAGE_IDLE_ICON_PB;
    }
#else

#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
		idle_softkey_icon[0] = IMAGE_IDLE_SHORTCUT_MAINMENU;
		idle_softkey_icon[1] = IMAGE_IDLE_ICON_SOS;
    }
	else
#endif

	{
		idle_softkey_icon[0] = IMAGE_IDLE_SHORTCUT_MAINMENU;
		idle_softkey_icon[1] = IMAGE_IDLE_SHORTCUT_PB;
    }
#endif
#ifdef MMI_COMMON_IDLE_IMG_SOFTKEY_SUPPORT
#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 != MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
#endif
	{
     MMIAPISET_GetFSMainIMGId(&idle_softkey_icon[0],&idle_softkey_icon[MMIIDLE_SOFTKEY_ICON_MAX-1]);
	}

#endif
    GUIRES_GetImgWidthHeight(&idle_softkey_icon_width, &idle_softkey_icon_height, idle_softkey_icon[0], MAIN_IDLE_WIN_ID);

	for (i = 0; i < MMIIDLE_SOFTKEY_ICON_MAX; i++)
	{
		
	    //set softkey icon display dis_point
		if (0 == i)
		{
			point.x = mainlcd_rect.left + MMIIDLE_SOFTKEY_MARGIN_WIDTH;//left icon		  
		}
		else if ((MMIIDLE_SOFTKEY_ICON_MAX-1) == i)
		{
			point.x = mainlcd_rect.right  - idle_softkey_icon_width - MMIIDLE_SOFTKEY_MARGIN_WIDTH;//right icon		    
		}
        else
        {
            point.x = (mainlcd_rect.right>>1 )- idle_softkey_icon_width + 2*MMIIDLE_SOFTKEY_MARGIN_WIDTH; 
        }
		point.y = mainlcd_rect.bottom - idle_softkey_icon_height - MMIIDLE_SOFTKEY_MARGIN_WIDTH;// the same height
		//display softkey icon
		GUIRES_DisplayImg(&point,
			PNULL,
			PNULL,
			MAIN_IDLE_WIN_ID,
			idle_softkey_icon[i],
		    &lcd_dev_info);
	}
#else 
    MMI_STRING_T            mess            = {0};	
    uint16                  str_pixel       = 0;
    uint16                  font_height     = 0; 
    uint16                  str_width       = 0;
    wchar                   *full_str       = PNULL;
    uint16                  full_len        = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
#ifndef TOUCH_PANEL_SUPPORT
    GUI_RECT_T              right_softkey_display_rect ={0};
    GUI_RECT_T              left_softkey_display_rect ={0};
#endif	
    //按照移动定制，左软键硬按键绑定为主菜单

#ifdef BAR_PHONE_WIDGET 
    MMI_TEXT_ID_T           left_key_id     = TXT_COMMON_MAINMENU;
     MMI_TEXT_ID_T           right_key_id    = TXT_IDLE_BUSINESS_MODE;
#else   
    MMI_TEXT_ID_T           left_key_id     = TXT_COMMON_MAINMENU;//STXT_MAIN_MESSAGE;
    MMI_TEXT_ID_T           right_key_id    = STXT_MAIN_PB;
#endif    
    //MMI_TEXT_ID_T           middle_key_id    = TXT_COMM_OPEN;
    GUISTR_STYLE_T          text_style = {0};/*lint !e64*/
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
	
#ifdef CMCC_UI_STYLE
    GUI_POINT_T             dis_point       = {0};
    uint16                  mid_icon_width = 0;
    uint16                  mid_icon_height = 0;
#endif
    full_str = SCI_ALLOCA(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL);
    SCI_MEMSET(full_str,0,(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL));
    MMIAPISET_GetFSMainTextId(&left_key_id,&right_key_id);
    
    //显示左软键
    font_height = GUI_GetFontHeight(MMI_FULLSCREEN_SOFTKEY_TEXT_FONT, UNICODE_HANZI);
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)left_key_id,&mess);
    
#if defined(BAR_PHONE_WIDGET)    
    point.y =  mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2 + 1;/*lint !e737*/
#else
    point.y =  mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2;/*lint !e737*/
#endif    
#ifdef  DPHONE_SUPPORT
    point.x = 1;
#elif defined(BAR_PHONE_WIDGET)    
    point.x = 2;
#else    
    point.x = 1;
#endif
  
    MMIIDLE_GetIdleTextStyle(&text_style, MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,  MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY));
    
    str_pixel = GUISTR_GetStringWidth(&text_style,&mess,text_state);
    MMIIDLE_GetDisPlayString(mess.wstr_ptr,mess.wstr_len,(mainlcd_rect.right -1)/2,MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,full_str,&full_len);
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].type        = MMI_MAIN_LCD_LEFT_SOFTKEY;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right  = point.x+str_pixel+1;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.bottom = point.y+font_height;
    
    str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );
#ifdef BAR_PHONE_TOUCH_WIDGET
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.top    = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.top-MMIIDLE_SOFTKEY_ADD_HEIGHT;
#endif
#else
    left_softkey_display_rect.left   = point.x;
    left_softkey_display_rect.top    = point.y;
    left_softkey_display_rect.right  = point.x+str_pixel+1;
    left_softkey_display_rect.bottom = point.y+font_height;

        str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );
#endif
    //显示右软键
    SCI_MEMSET(full_str,0,(sizeof(wchar)*MMI_MAINSCREEN_RIGHT_MAX_PIXEL));
    full_len = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;

#ifdef MMI_IDLE_NO_SIM_SOS_SUPPORT
    if (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0))
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_SOS,&mess);
    }
    else
#endif
	{
	    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)right_key_id,&mess);  
	}

    str_pixel = GUISTR_GetStringWidth(&text_style,&mess,text_state);
    MMIIDLE_GetDisPlayString(mess.wstr_ptr,mess.wstr_len,(mainlcd_rect.right -1)/2,MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,full_str,&full_len);
#ifdef  DPHONE_SUPPORT
#if defined TOUCH_PANEL_SUPPORT
	point.x = MAX((mainlcd_rect.right - str_pixel-1),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right+1));
#else
	point.x = MAX((mainlcd_rect.right - str_pixel-1),(left_softkey_display_rect.right+1));
#endif
#elif defined(BAR_PHONE_WIDGET)    
#if defined TOUCH_PANEL_SUPPORT
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right+3));
#else
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(left_softkey_display_rect.right+3));
#endif
//#elif defined (MAINLCD_SIZE_128X160)
//    point.x = MAX((mainlcd_rect.right - str_pixel-4),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right));
#else    
#if defined TOUCH_PANEL_SUPPORT
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM9].rect.right+10));
#else 
    point.x = MAX((mainlcd_rect.right - str_pixel-4),(left_softkey_display_rect.right+10));
#endif
#endif
#if defined(BAR_PHONE_WIDGET)    
    point.y = mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2 + 1;/*lint !e737*/
#else
    point.y = mainlcd_rect.bottom - MMITHEME_GetSoftkeyHeight()/2 - font_height/2;/*lint !e737*/
#endif    
#if defined TOUCH_PANEL_SUPPORT
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].type        = MMI_MAIN_LCD_RIGHT_SOFTKEY;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.left   = point.x;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.top    = point.y;
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.right  = MIN((point.x+str_pixel+1),mainlcd_rect.right);
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.bottom = point.y+font_height;
    
    str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );
#ifdef BAR_PHONE_TOUCH_WIDGET
    s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.top    = s_common_idle_display_rect[MMI_COMMON_IDLE_ITEM10].rect.top-MMIIDLE_SOFTKEY_ADD_HEIGHT;
#endif
#else
    right_softkey_display_rect.left   = point.x;
    right_softkey_display_rect.top    = point.y;
    right_softkey_display_rect.right  = MIN((point.x+str_pixel+1),mainlcd_rect.right);
    right_softkey_display_rect.bottom = point.y+font_height;

            str_width = MMIIDLE_DisplayIdleText(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T *)&point,
        MMI_FULLSCREEN_SOFTKEY_TEXT_FONT,
        MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_SOFTKEY),
        full_str, 
        full_len
        );

#endif
    SCI_FREE(full_str);
    full_str = PNULL;
#ifdef CMCC_UI_STYLE
#ifndef MMI_IDLE_MINI_SUPPORT
    GUIRES_GetImgWidthHeight(&mid_icon_width, &mid_icon_height,IMAGE_IDLE_MIDDLE_SOFTKEY_ICON , MAIN_IDLE_WIN_ID);

    dis_point.x = (mainlcd_rect.right>>1 )- mid_icon_width;
    dis_point.y = mainlcd_rect.bottom - mid_icon_height - 2;
    //display middle softkey icon
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        MAIN_IDLE_WIN_ID,
        IMAGE_IDLE_MIDDLE_SOFTKEY_ICON,
        &lcd_dev_info);
#endif    
#endif
#endif//#ifdef MAINLCD_SIZE_128X160
}
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : get tp type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_IDLE_DISPLAY_INDEX_E GetTpType(
                                        GUI_POINT_T   tp_point
                                        )
{
    uint32                            i               = 0;
    MMI_IDLE_DISPLAY_INDEX_E          tp_type         = MMI_MAIN_LCD_NONE;    
    
    for (i = 0; i < MMI_COMMON_IDLE_MAX_ITEM; i++)
    {
        if (GUI_PointIsInRect(tp_point,s_common_idle_display_rect[i].rect))
        {
            tp_type = s_common_idle_display_rect[i].type;
            break;
        }
    }

    return (tp_type);
}
#endif

#if defined (MMI_AUDIO_PLAYER_SUPPORT) || defined (FM_SUPPORT)
/*****************************************************************************/
//  Description : start mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIIDLECOM_StartIdleMp3Timer(void)
{
    BOOLEAN     result  =   FALSE;

    if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
    {
        if ((0 == g_mp3_name_timer_id) &&
            (MMIIDLECOM_GetIdleMp3Info()->is_scroll))
        {
            g_mp3_name_timer_id = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, IDLE_MP3_TIMER_PERIOD, FALSE);
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : stop mp3 name idle timer
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLECOM_StopIdleMp3Timer(void)
{
    BOOLEAN     result  =   FALSE;

    if (0 != g_mp3_name_timer_id)
    {
        MMK_StopTimer(g_mp3_name_timer_id);
        g_mp3_name_timer_id = 0;
        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
// Description : UpdateMp3NameStartIndex
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIIDLECOM_UpdateMp3NameStartIndex(void)
{  
    MMI_STRING_T        text = {0};
    wchar               mp3_name[MMIMP3_NAME_MAX_LEN + 1]={0};
    text.wstr_ptr = mp3_name;

    if(FM_PLAYING == MMIAPIFM_GetStatus())
    {
        MMIAPIFM_GetCurFreqName(&text, MMIMP3_NAME_MAX_LEN);
    }
    else
    {
        MMIAPIMP3_GetCurMp3Name(&text, MMIMP3_NAME_MAX_LEN);
    }
    
    MMIIDLECOM_GetIdleMp3Info()->str_index += 1;
    if (0 == text.wstr_len)//防错处理，防止没有文件名的Mp3
    {
        MMIIDLECOM_GetIdleMp3Info()->str_index = 0;
    }
    else
    {
        MMIIDLECOM_GetIdleMp3Info()->str_index %= text.wstr_len;
    }
    g_mp3_name_timer_id = 0;
}
/*****************************************************************************/
// Description : MMIIDLECOM_GetMp3NameTimerId
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC uint8 MMIIDLECOM_GetMp3NameTimerId(void)
{
    return g_mp3_name_timer_id;
}


#endif
#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : create idle items paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void CreateCommonIdlePaintLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    uint16   layer_width = 0;
    uint16   layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    UILAYER_APPEND_BLT_T        append_layer = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_common_idle_paint_layer_handle))    
    {
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
        UILAYER_CreateLayer(
            &create_info,
            &s_common_idle_paint_layer_handle);

        //set color key
        UILAYER_SetLayerColorKey(&s_common_idle_paint_layer_handle,TRUE,UILAYER_TRANSPARENT_COLOR);
        
        append_layer.lcd_dev_info = s_common_idle_paint_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }

    if (UILAYER_IsLayerActive(&s_common_idle_paint_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_paint_layer_handle);
    }

}
#endif

/*****************************************************************************/
//  Description : get idle items paint layer ptr
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO * GetCommonIdlePaintLcdDev(void)
{
    GUI_LCD_DEV_INFO            *lcd_dev_info_ptr = PNULL;
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsLayerActive(&s_common_idle_paint_layer_handle))
    {
        lcd_dev_info_ptr = &s_common_idle_paint_layer_handle;
    }
    else
#endif
    {
        lcd_dev_info_ptr = MMITHEME_GetDefaultLcdDev(); /*lint !e605*/
    }
    return lcd_dev_info_ptr;
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdlePaintLayer(void)
{
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_common_idle_paint_layer_handle))    
    {
        UILAYER_RELEASELAYER(&s_common_idle_paint_layer_handle); /*lint !e506 !e774 !e831*/
    }
}

/*****************************************************************************/
//  Description : create status bar layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note: 
/*****************************************************************************/
LOCAL void CreateCommonIdleStbLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();    
    UILAYER_APPEND_BLT_T        append_layer = {0};

    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_common_idle_stb_layer_handle))    
    {
        width  = main_rect.right - main_rect.left + 1;
        height = MMI_STATUSBAR_HEIGHT;
          
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = width;
        create_info.height          = height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;   
        create_info.format          = UILAYER_MEM_DOUBLE_COPY;
        
        UILAYER_CreateLayer(&create_info, &s_common_idle_stb_layer_handle);
            
        UILAYER_SetLayerColorKey(&s_common_idle_stb_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
        append_layer.lcd_dev_info = s_common_idle_stb_layer_handle;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
        
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id, MMICOMMON_STATUSBAR_CTRL_ID), &s_common_idle_stb_layer_handle);             
        GUISTATUSBAR_SetBgIsDesplay(win_id, FALSE);                
    }
    
    if (UILAYER_IsLayerActive(&s_common_idle_stb_layer_handle))
    {
        UILAYER_Clear(&s_common_idle_stb_layer_handle);
    }
}

/*****************************************************************************/
//  Description : release idle paint layer
//  Global resource dependence : 
//  Author: Disen Young
//  Note:
/*****************************************************************************/
LOCAL void ReleaseCommonIdleStbLayer(
                              MMI_WIN_ID_T      win_id
                              )
{
    // 释放层
    if (UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&s_common_idle_stb_layer_handle))    
    {
        MMK_SetCtrlLcdDevInfo(MMK_GetCtrlHandleByWin(win_id,
                MMICOMMON_STATUSBAR_CTRL_ID),  MMITHEME_GetDefaultLcdDev()); /*lint !e605*/            
        UILAYER_RELEASELAYER(&s_common_idle_stb_layer_handle);/*lint !e506 !e774 !e831*/
    }
}
#endif
#else
/*****************************************************************************/
//  Description : set idle mp3 name
//  Global resource dependence : 
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBgPlayName(
                                BOOLEAN     is_display,
                                MMIBGPLAY_TYPE_E bg_type
                                )
{
}
#endif //defined MMI_COMMON_IDLE_SUPPORT



/*Edit by script, ignore 2 case. Thu Apr 26 19:00:55 2012*/ //IGNORE9527
