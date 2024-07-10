/*****************************************************************************
** File Name:      mmiwidget_speed_dial.c                                        *
** Author:                                                                   *
** Date:           02/08/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                  NAME             DESCRIPTION                        *
** 05/05/2011        xiaoming.ren              Creat
******************************************************************************/

#define _MMIWIDGET_SPEED_DIAL_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#ifdef MMI_WIDGET_SPEED_DIAL
#include "window_parse.h"
#include "guilabel.h"
#include "mmipub.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmitheme_widget.h"
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmiwidget_position.h"
#include "mmiwidget_export.h"
#include "gpio_prod_api.h"
#include "mmicc_nv.h"
#include "mmicc_speeddial.h"
#include "mmicc_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**-------------------------------------------------------------------------*/
#if defined(MAINLCD_SIZE_240X320)
#define   MMIWIDGET_SPEED_DIAL_TEXT_FONT                   SONG_FONT_16            //文本信息字体
//#define   MMIWIDGET_SPEED_DIAL_BIG_FONT                   SONG_FONT_20            //文本信息字体
#define 	MMIWIDGET_SPEED_DIAL_TEXT_COLOR		MMI_WHITE_COLOR  //文本字体颜色
//#define 	MMIWIDGET_SPEED_DIAL_FILL_COLOR		MMI_GRAY_COLOR //选中填充颜色
#define   MMIWIDGET_SPEED_DIAL_DEFAULT_X                   50                      //widget初始化x坐标
#define   MMIWIDGET_SPEED_DIAL_DEFAULT_Y                   50                      //widget初始化y坐标
#define 	MMIWIDGET_SPEED_DIAL_PROMPT_LEN         6////字符串最大长度
#define 	MMIWIDGET_SPEED_DIAL_NUMBER_LEN         6//数字最大长度
#define 	MMIWIDGET_SPEED_DIAL_V_SPACE        40
#define 	MMIWIDGET_SPEED_DIAL_H_SPACE        6
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_X			13
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_Y			57
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH			66
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT			16
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_X			8  //tite x 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_Y			5  //tite y 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_WIDTH			225  // tite 宽度
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_HEIGHT			20  //tite 高度
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_X			14
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_Y			205
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_WIDTH			212
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_HEIGHT			16

#define MMIWIDGET_SPEED_DIAL_V_SPACE_H        14
#define MMIWIDGET_SPEED_DIAL_H_SPACE_H        26
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_X_H			32
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_Y_H			34
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH_H			72
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT_H		16
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_X_H			20  //tite x 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_Y_H			6  //tite y 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_WIDTH_H			280  // tite 宽度
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_HEIGHT_H			16  //tite 高度
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_X_H			20
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_Y_H			124
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_WIDTH_H			280
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_HEIGHT_H			16

#if 0
#define	MMIWIDGET_SPEED_DIAL_SE_POS_X			7  //选中框起点x 坐标
#define	MMIWIDGET_SPEED_DIAL_SE_POS_Y			26   //选中框起点y 坐标
#define	MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH			73  //选中框宽度
#define	MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT			56  //选中框高度

#define	MMIWIDGET_SPEED_DIAL_NUMBER_SPACE_TOP			5  //向上偏移量
#define	MMIWIDGET_SPEED_DIAL_NUMBER_SPACE_BOTTOM			25      //向下偏移量
#endif
#elif defined(MAINLCD_SIZE_240X400)
#define   MMIWIDGET_SPEED_DIAL_TEXT_FONT                   SONG_FONT_16            //文本信息字体
//#define   MMIWIDGET_SPEED_DIAL_BIG_FONT                   SONG_FONT_20            //文本信息字体
#define 	MMIWIDGET_SPEED_DIAL_TEXT_COLOR		MMI_WHITE_COLOR  //文本字体颜色
//#define 	MMIWIDGET_SPEED_DIAL_FILL_COLOR		MMI_GRAY_COLOR //选中填充颜色
#define   MMIWIDGET_SPEED_DIAL_DEFAULT_X                   50                      //widget初始化x坐标
#define   MMIWIDGET_SPEED_DIAL_DEFAULT_Y                   50                      //widget初始化y坐标
#define 	MMIWIDGET_SPEED_DIAL_PROMPT_LEN         6////字符串最大长度
#define 	MMIWIDGET_SPEED_DIAL_NUMBER_LEN         6//数字最大长度
#define 	MMIWIDGET_SPEED_DIAL_V_SPACE        40
#define 	MMIWIDGET_SPEED_DIAL_H_SPACE        6
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_X			13
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_Y			59
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH			66
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT			16
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_X			8  //tite x 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_Y			5  //tite y 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_WIDTH			225  // tite 宽度
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_HEIGHT			20  //tite 高度
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_X			14
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_Y			205
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_WIDTH			212
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_HEIGHT			16

#define MMIWIDGET_SPEED_DIAL_V_SPACE_H        14
#define MMIWIDGET_SPEED_DIAL_H_SPACE_H        26
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_X_H			32
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_Y_H			34
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH_H			72
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT_H		16
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_X_H			20  //tite x 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_Y_H			6  //tite y 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_WIDTH_H			280  // tite 宽度
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_HEIGHT_H			16  //tite 高度
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_X_H			20
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_Y_H			124
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_WIDTH_H			280
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_HEIGHT_H			16

#if 0
#define	MMIWIDGET_SPEED_DIAL_SE_POS_X			7  //选中框起点x 坐标
#define	MMIWIDGET_SPEED_DIAL_SE_POS_Y			26   //选中框起点y 坐标
#define	MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH			73  //选中框宽度
#define	MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT			56  //选中框高度

#define	MMIWIDGET_SPEED_DIAL_NUMBER_SPACE_TOP			5  //向上偏移量
#define	MMIWIDGET_SPEED_DIAL_NUMBER_SPACE_BOTTOM			25      //向下偏移量
#endif
#elif defined(MAINLCD_SIZE_320X480)
#define   MMIWIDGET_SPEED_DIAL_TEXT_FONT                   SONG_FONT_20            //文本信息字体
//#define   MMIWIDGET_SPEED_DIAL_BIG_FONT                   SONG_FONT_20            //文本信息字体
#define 	MMIWIDGET_SPEED_DIAL_TEXT_COLOR		MMI_WHITE_COLOR  //文本字体颜色
//#define 	MMIWIDGET_SPEED_DIAL_FILL_COLOR		MMI_GRAY_COLOR //选中填充颜色
#define   MMIWIDGET_SPEED_DIAL_DEFAULT_X                   50                      //widget初始化x坐标
#define   MMIWIDGET_SPEED_DIAL_DEFAULT_Y                   50                      //widget初始化y坐标
#define 	MMIWIDGET_SPEED_DIAL_PROMPT_LEN         6////字符串最大长度
#define 	MMIWIDGET_SPEED_DIAL_NUMBER_LEN         6//数字最大长度
#define 	MMIWIDGET_SPEED_DIAL_V_SPACE        53
#define 	MMIWIDGET_SPEED_DIAL_H_SPACE        6
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_X			22
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_Y			78
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH			90
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT			21
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_X			8  //tite x 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_Y			5  //tite y 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_WIDTH			308  // tite 宽度
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_HEIGHT			27  //tite 高度
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_X			19
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_Y			272
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_WIDTH			290
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_HEIGHT			21

#define MMIWIDGET_SPEED_DIAL_V_SPACE_H        17
#define MMIWIDGET_SPEED_DIAL_H_SPACE_H        39
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_X_H			48
#define	MMIWIDGET_SPEED_DIAL_NAME_POS_Y_H			43
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH_H			108
#define	MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT_H		19
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_X_H			30  //tite x 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_Y_H			7  //tite y 坐标
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_WIDTH_H			420  // tite 宽度
#define	MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_HEIGHT_H			19  //tite 高度
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_X_H			30
#define	MMIWIDGET_SPEED_DIAL_TIP_POS_Y_H			150
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_WIDTH_H			420
#define	MMIWIDGET_SPEED_DIAL_TIP_RECT_HEIGHT_H			19

#if 0
#define	MMIWIDGET_SPEED_DIAL_SE_POS_X			7  //选中框起点x 坐标
#define	MMIWIDGET_SPEED_DIAL_SE_POS_Y			26   //选中框起点y 坐标
#define	MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH			73  //选中框宽度
#define	MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT			56  //选中框高度

#define	MMIWIDGET_SPEED_DIAL_NUMBER_SPACE_TOP			5  //向上偏移量
#define	MMIWIDGET_SPEED_DIAL_NUMBER_SPACE_BOTTOM			25      //向下偏移量
#endif
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/






/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/
extern  CC_SPEEDKEY_ENTRY_T s_speeddial_record[SPEEDKEY_MAX_NUM];//记录快捷拨号对应号码
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint8 s_cur_index = 0;    //0---8    <------>  key1--- key9
LOCAL   uint8 s_old_index = 0;  //0-8       <------>  key1--- key9
LOCAL BOOLEAN is_from_idle_widget = FALSE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description : widget界面事件处理
//    Global resource dependence : 
//    Author:   
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetSpeedDialWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
);

/*****************************************************************************/
//     Description : reset child win, just for lose focus to init button status.
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL void ResetWidgetSpeedDial(void);

/*****************************************************************************/
//    Description : 显示Widget 信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void DisplayWidgetSpeedDialInfo(MMI_WIN_ID_T        win_id);
/*****************************************************************************/
//    Description : 读取快捷设置信息 index 0 到8 对应key2到key9
//    Global resource dependence :
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadSpeedDialNameStr(MMI_STRING_T *string_ptr ,wchar* str_ptr ,uint8 index);



//快捷拨号widget
PUBLIC MMIWIDGET_ITEM_INFO_T g_speed_dial_widget =
{
        MMIWIDGET_SPEED_DIAL_ID, 
		HandleWidgetSpeedDialWinMsg, 
		PNULL,
		ResetWidgetSpeedDial,
		PNULL,		
		WIDGET_SPEED_DIAL_WIN_ID,
		IMAGE_WIDGET_SPEED_DIAL_SHORTCUT,
		IMAGE_WIDGET_SPEED_DIAL_BG,
		TXT_WIDGET_SPEED_DIAL,
		MMIWIDGET_SPEED_DIAL_DEFAULT_X,
		MMIWIDGET_SPEED_DIAL_DEFAULT_Y,
		FALSE,
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description : widget界面事件处理
//    Global resource dependence : 
//    Author:   
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetSpeedDialWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E    msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint8 sel_list = 0;

    //SCI_TRACE_LOW:"HandleWidgetSpeedDialWinMsg msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SPEED_DIAL_267_112_2_18_3_8_39_239,(uint8*)"d", msg_id);
	
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:	
            ResetWidgetSpeedDial();
		break;

#if 0		
	case MSG_APP_UP:
               s_old_index = s_cur_index;
		if(s_cur_index>=1)
			s_cur_index--;
		else
			s_cur_index = SPEEDKEY_MAX_NUM;
		break;
        
	case MSG_APP_DOWN:
                s_old_index = s_cur_index;
	        if(s_cur_index<SPEEDKEY_MAX_NUM)
                    s_cur_index++;
                else
                    s_cur_index = 0;
		break;
#endif
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
    case MSG_KEYLONG_1:
    //cphs 有四个语音号码，暂时只呼叫LINE1语音信箱号码
        MMIAPISMS_OpenCallVMWin(MMI_DUAL_SYS_MAX);
        break;
#endif        
    case MSG_KEYLONG_2:
    case MSG_KEYLONG_3:
    case MSG_KEYLONG_4:
    case MSG_KEYLONG_5:
    case MSG_KEYLONG_6:
    case MSG_KEYLONG_7:
    case MSG_KEYLONG_8:
    case MSG_KEYLONG_9:
        s_old_index = s_cur_index;
        s_cur_index = (msg_id - MSG_KEYLONG_1);
        {
            MMI_STRING_T name_str_temp={0};
            uint8 index = 0;
            BOOLEAN flag = FALSE;
            wchar *str_ptr = PNULL;
            wchar dis_str1[SPEEDKEY_PROMPT_LEN + 1] = {0};
            wchar dis_str2[SPEEDKEY_PROMPT_LEN + 1] = {0};
            SCI_MEMSET(dis_str1,0xff,SPEEDKEY_MAX_LEN + 1);
            name_str_temp.wstr_ptr = dis_str1;        
            name_str_temp.wstr_len = SPEEDKEY_PROMPT_LEN; 
            
            index = msg_id - MSG_KEYLONG_2;
            
            flag = ReadSpeedDialNameStr(&name_str_temp,dis_str2 ,index);//读取名称
            if(flag)
            {
                MMIAPICC_SpeedDial(MMI_DUAL_SYS_MAX, (uint16)(msg_id - MSG_KEYLONG_1 + 1));
            }
            else
            {
                OpenSpeedDialListWinSetCurItem(&s_cur_index);//进入快速拨号界面
            }
        }
        break;      
 
    case MSG_FULL_PAINT:
            DisplayWidgetSpeedDialInfo(win_id);
            break;

#ifdef BAR_PHONE_WIDGET 
    case MSG_WIDGET_UPDATE_SOFTKEY:
            //显示softkey            
            MMIWIDGET_SoftkeySetTextId(win_id,TXT_VIEW,FALSE);            
        break;
#endif

    case MSG_APP_WEB:
        if(s_cur_index != 0)
        {
            OpenSpeedDialListWinSetCurItem(&s_cur_index);//进入快速拨号界面
        }
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
        else
        {     
            MMIAPISMS_OpenVoicemailWin();
        }
#endif        
        break;
        
    case MSG_LCD_SWITCH:
        ResetWidgetSpeedDial();
        break;
        
    default:
		recode = MMI_RESULT_FALSE;
		break;
    }
    
    return recode;        
}
/*****************************************************************************/
//    Description : 显示Widget 信息
//    Global resource dependence : 
//    Author:   
//    Note:
/*****************************************************************************/
LOCAL void DisplayWidgetSpeedDialInfo(MMI_WIN_ID_T        win_id)
{
    GUI_RECT_T		win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    if (!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
    }

         if (MMITHEME_IsMainScreenLandscape())
         {
            GUI_POINT_T		point = {0};
            uint8 index = 0;    
            GUI_RECT_T			     rect = {0};
            GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
            GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_NOT_HALF_CHAR;  

            MMI_STRING_T name_str_temp={0};
            wchar dis_str1[SPEEDKEY_PROMPT_LEN + 1] = {0};
            wchar dis_str2[SPEEDKEY_PROMPT_LEN + 1] = {0};
            int16 str_len = 0;	
            
            UILAYER_ClearRect(&lcd_dev_info, win_rect);
                
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = MMIWIDGET_SPEED_DIAL_TEXT_FONT;
            text_style.font_color = MMIWIDGET_SPEED_DIAL_TEXT_COLOR;
            text_style.line_space = 4;
            
            //显示背景图
            point.x = win_rect.left;
            point.y = win_rect.top;
            GUIRES_DisplayImg(&point, PNULL, PNULL, win_id,
                IMAGE_WIDGET_SPEED_DIAL_BG, &lcd_dev_info);
                
            //显示TITIE   
            rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_X_H;
            rect.top = win_rect.top + MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_Y_H;
            rect.right = rect.left + MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_WIDTH_H;
            rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_HEIGHT_H;

            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WIDGET_SPEED_DIAL, &name_str_temp);
            GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&name_str_temp,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
                

            //显示各个快捷拨号名称
            for( index = 0;index < SPEEDKEY_MAX_NUM+1; index++ )
        	{
        		if(index % 3 == 0)
        		{
        			rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_NAME_POS_X_H;
        		}
        		else
        		{
        			rect.left =(win_rect.left + MMIWIDGET_SPEED_DIAL_NAME_POS_X_H) + (MMIWIDGET_SPEED_DIAL_H_SPACE_H + MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH_H)* (index%3) ;
        		}
        		rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_NAME_POS_Y_H) + (MMIWIDGET_SPEED_DIAL_V_SPACE_H + MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT_H)* (index/3) ;
        		rect.right = rect.left + MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH_H;
        		rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT_H;
        		
        		if(index != 0)
        		{
        		#if 0
        		    GUI_FillRect(&lcd_dev_info, rect, MMI_GRAY_COLOR );
        		#else        		

        		        SCI_MEMSET(dis_str1, 0, SPEEDKEY_PROMPT_LEN);
                        SCI_MEMSET(dis_str2, 0, SPEEDKEY_PROMPT_LEN);
                        name_str_temp.wstr_ptr = dis_str1;
                        name_str_temp.wstr_len = SPEEDKEY_PROMPT_LEN;
                
                        ReadSpeedDialNameStr(&name_str_temp,dis_str2 ,(index - 1));//读取名称
                        GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                            (const GUI_RECT_T      *)&rect,       //the fixed display area
                            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                            (const MMI_STRING_T    *)&name_str_temp,
                            &text_style,
                            state,
                            GUISTR_TEXT_DIR_AUTO
                            );
        		#endif
        		}
        	}
        	
        	//显示提示语
        	rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_TIP_POS_X_H;
        	rect.top = win_rect.top + MMIWIDGET_SPEED_DIAL_TIP_POS_Y_H;
        	rect.right = rect.left + MMIWIDGET_SPEED_DIAL_TIP_RECT_WIDTH_H;
        	rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_TIP_RECT_HEIGHT_H;
        	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WIDGET_SPEED_DIAL_TIP, &name_str_temp);
        	GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        		(const GUI_RECT_T      *)&rect,       //the fixed display area
        		(const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        		(const MMI_STRING_T    *)&name_str_temp,
        		&text_style,
        		state,
        		GUISTR_TEXT_DIR_AUTO
        		);
        }
         else
         {
            GUI_POINT_T		point = {0};
            uint8 index = 0;    
            GUI_RECT_T			     rect = {0};
            GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
            GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_NOT_HALF_CHAR;  

            MMI_STRING_T name_str_temp={0};
            wchar dis_str1[SPEEDKEY_PROMPT_LEN + 1] = {0};
            wchar dis_str2[SPEEDKEY_PROMPT_LEN + 1] = {0};
            int16 str_len = 0;	
            
            UILAYER_ClearRect(&lcd_dev_info, win_rect);                

            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = MMIWIDGET_SPEED_DIAL_TEXT_FONT;
            text_style.font_color = MMIWIDGET_SPEED_DIAL_TEXT_COLOR;
            text_style.line_space = 4;
            
            //显示背景图
            point.x = win_rect.left;
            point.y = win_rect.top;
            GUIRES_DisplayImg(&point, PNULL, PNULL, win_id,
                IMAGE_WIDGET_SPEED_DIAL_BG, &lcd_dev_info);
                
            //显示TITIE   
            rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_X;
            rect.top = win_rect.top + MMIWIDGET_SPEED_DIAL_TITE_NAME_POS_Y;
            rect.right = rect.left + MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_WIDTH;
            rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_TITE_NAME_RECT_HEIGHT;

            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WIDGET_SPEED_DIAL, &name_str_temp);
            GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&name_str_temp,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
                

            //显示各个快捷拨号名称
            for( index = 0;index < SPEEDKEY_MAX_NUM+1; index++ )
        	{
        		if(index % 3 == 0)
        		{
        			rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_NAME_POS_X;
        		}
        		else
        		{
        			rect.left =(win_rect.left + MMIWIDGET_SPEED_DIAL_NAME_POS_X) + (MMIWIDGET_SPEED_DIAL_H_SPACE + MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH)* (index%3) ;
        		}
        		rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_NAME_POS_Y) + (MMIWIDGET_SPEED_DIAL_V_SPACE + MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT)* (index/3) ;
        		rect.right = rect.left + MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH;
        		rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT;
        		
        		if(index != 0)
        		{        		        
        		        SCI_MEMSET(dis_str1, 0, SPEEDKEY_PROMPT_LEN);
                        SCI_MEMSET(dis_str2, 0, SPEEDKEY_PROMPT_LEN);
                        
                        name_str_temp.wstr_ptr = dis_str1;
                        name_str_temp.wstr_len = SPEEDKEY_PROMPT_LEN;
                        
                        ReadSpeedDialNameStr(&name_str_temp,dis_str2 ,(index - 1));//读取名称
                        GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                            (const GUI_RECT_T      *)&rect,       //the fixed display area
                            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                            (const MMI_STRING_T    *)&name_str_temp,
                            &text_style,
                            state,
                            GUISTR_TEXT_DIR_AUTO
                            );
        		}
        	}
        	
        	//显示提示语
        	rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_TIP_POS_X;
        	rect.top = win_rect.top + MMIWIDGET_SPEED_DIAL_TIP_POS_Y;
        	rect.right = rect.left + MMIWIDGET_SPEED_DIAL_TIP_RECT_WIDTH;
        	rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_TIP_RECT_HEIGHT;
        	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WIDGET_SPEED_DIAL_TIP, &name_str_temp);
        	GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        		(const GUI_RECT_T      *)&rect,       //the fixed display area
        		(const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        		(const MMI_STRING_T    *)&name_str_temp,
        		&text_style,
        		state,
        		GUISTR_TEXT_DIR_AUTO
        		);
        }
   
}
/*****************************************************************************/
//     Description : reset child win, just for lose focus to init button status.
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL void ResetWidgetSpeedDial(void)
{   
    s_cur_index = 1;
    s_old_index = 0;
}
/*****************************************************************************/
//    Description : 读取快捷设置信息 index 0 到8 对应key2到key9
//    Global resource dependence :
//    Author:  
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadSpeedDialNameStr(MMI_STRING_T *string_ptr,wchar* str_ptr ,uint8 index)
{
        BOOLEAN result = TRUE;
        MN_RETURN_RESULT_E	ret_val = MN_RETURN_FAILURE;
        MMI_STRING_T name_str_temp={0};
        CC_SPEEDKEY_ENTRY_T	speedkey_entry = {0};
        CC_SPEEDKEY_ENTRY_T  *speedkey_entry_ptr=&speedkey_entry;
        int16 str_len = 0;
	

        SCI_MEMSET(&speedkey_entry, 0, sizeof(speedkey_entry));
        // @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
        MMINV_READ((MMI_CC_SPEEDKEY_2 + index), &speedkey_entry, ret_val);
        if( MN_RETURN_SUCCESS != ret_val )
        {
            SCI_MEMSET(&speedkey_entry, 0, sizeof(speedkey_entry));
            speedkey_entry.number_len = 0;
            s_speeddial_record[index] = speedkey_entry;     
            
            // @baokun : 电话本里设置2快捷拨号后，在idle长按2，提示未设置号码
            MMINV_WRITE((MMI_CC_SPEEDKEY_2 + index), &speedkey_entry);
        }
        else
        {
            s_speeddial_record[index]= speedkey_entry;
        }
        //判断是否存在于pb中
        {  
            if (speedkey_entry_ptr->number_len > 0
            || 0 != speedkey_entry_ptr->npi_ton)     //非空记录
            {
                BOOLEAN is_find = FALSE;
                MMIPB_BCD_NUMBER_T pb_number = {0};

                name_str_temp.wstr_ptr = str_ptr;
                name_str_temp.wstr_len = SPEEDKEY_PROMPT_LEN;
                SCI_MEMSET(pb_number.number, 0xff, MMIPB_BCD_NUMBER_MAX_LEN);        
                pb_number.npi_ton = speedkey_entry_ptr->npi_ton;
                pb_number.number_len = speedkey_entry_ptr->number_len;
                MMI_MEMCPY(
                                        pb_number.number,
                                        pb_number.number_len,
                                        speedkey_entry_ptr->number,
                                        sizeof(speedkey_entry_ptr->number),
                                        speedkey_entry_ptr->number_len
                                        );

                is_find = MMIAPIPB_GetNameByNumber(&pb_number, &name_str_temp, SPEEDKEY_PROMPT_LEN, FALSE);/*lint !e506 */

                if (!is_find || 0 == name_str_temp.wstr_len)   //在pb中查找不存在或名字为空
                {
                        uint8 temp_str[MMIWIDGET_SPEED_DIAL_NUMBER_LEN + 1] = {0};
                        uint8 temp_str_two[SPEEDKEY_MAX_LEN + 1] = {0};
                        uint8 j=0,flag=0;
                        name_str_temp.wstr_len = (uint16)MMIAPICOM_GenDispNumber((MN_NUMBER_TYPE_E)
                                                ((speedkey_entry_ptr->npi_ton & 0xf0) >> 4),
                                                speedkey_entry_ptr->number_len,                
                                                speedkey_entry_ptr->number,
                                                temp_str_two,
                                                SPEEDKEY_MAX_LEN
                                                );
                        //取电话后面几位
                        {                    
                            if(name_str_temp.wstr_len > MMIWIDGET_SPEED_DIAL_PROMPT_LEN )//电话长度大于N 位
                            {
                                flag = name_str_temp.wstr_len - MMIWIDGET_SPEED_DIAL_PROMPT_LEN;
                                for(j= 0; j< MMIWIDGET_SPEED_DIAL_PROMPT_LEN; j++)
                                {
                                    temp_str[j] = temp_str_two[j + flag];
                                }
                                MMI_STRNTOWSTR(name_str_temp.wstr_ptr, MMIWIDGET_SPEED_DIAL_NUMBER_LEN + 1, (uint8 *)temp_str, SPEEDKEY_MAX_LEN + 1,MMIWIDGET_SPEED_DIAL_NUMBER_LEN + 1);           
                             }
                            else
                            {
                                MMI_STRNTOWSTR(name_str_temp.wstr_ptr, MMIWIDGET_SPEED_DIAL_NUMBER_LEN + 1, (uint8 *)temp_str_two, SPEEDKEY_MAX_LEN + 1,MMIWIDGET_SPEED_DIAL_NUMBER_LEN + 1);    
                            }
                       }
                 }
            }
            else
            {
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_BLANK, &name_str_temp);
                result = FALSE;
            }
            //截取前面的N位
            str_len = (name_str_temp.wstr_len>MMIWIDGET_SPEED_DIAL_PROMPT_LEN)?MMIWIDGET_SPEED_DIAL_PROMPT_LEN:name_str_temp.wstr_len;
            string_ptr->wstr_len = str_len;
            MMI_WSTRNCPY(string_ptr->wstr_ptr,str_len,name_str_temp.wstr_ptr,str_len,str_len);
        }
        return result;
}
#if 0//cfy
/*****************************************************************************/
//  Description : display SpeedDial select
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void WidgetSpeedDial_Select(
                           MMI_WIN_ID_T     win_id,
                           uint16 index
                           )
{
    GUI_RECT_T		win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T			     rect = {0};
    MMI_STRING_T name_str_temp={0};
    int16 str_len = 0;
    const uint32 buffer_size = 8;
    char     button_str[8] = {0};
    wchar    button_wstr[8] = {0};
    uint32   button_wstr_length = 0;
  
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_NOT_HALF_CHAR;  

    wchar dis_str1[SPEEDKEY_PROMPT_LEN + 1] = {0};
    wchar dis_str2[SPEEDKEY_PROMPT_LEN + 1] = {0};


    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = MMIWIDGET_SPEED_DIAL_BIG_FONT;
    text_style.font_color = MMIWIDGET_SPEED_DIAL_TEXT_COLOR;
    text_style.line_space = 4;
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
//计算区域
    if(index % 3 == 0)
    {
        rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_SE_POS_X + 1;
        rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_SE_POS_Y) + (MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT)* (index/3) ;
        rect.right = rect.left + MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH - 1;
        rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT;
    }
    else if(index % 3 == 2)
    {
        rect.left =(win_rect.left + MMIWIDGET_SPEED_DIAL_SE_POS_X) + (MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH)* (index%3) ;
        rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_SE_POS_Y) + (MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT)* (index/3) ;
        rect.right = rect.left + MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH - 1;
        rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT;
    }
    else
    {
        rect.left =(win_rect.left + MMIWIDGET_SPEED_DIAL_SE_POS_X) + (MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH)* (index%3) ;
        rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_SE_POS_Y) + (MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT)* (index/3) ;
        rect.right = rect.left + MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH;
        rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT;
    }
    //显示选中图片
    GUI_FillRect(&lcd_dev_info,rect, MMIWIDGET_SPEED_DIAL_FILL_COLOR);
   // GUIRES_DisplayImg(PNULL, &rect, &rect, win_id,IMAGE_WIDGET_SPEED_DIAL_SE, &lcd_dev_info);
   
      //选中框中数字
    SCI_MEMSET(button_str, 0 , buffer_size);
    SCI_MEMSET(button_wstr, 0 , buffer_size* 2);
    button_str[0] = 0x31 + index;
    button_wstr_length = SCI_STRLEN(button_str);
    MMI_STRNTOWSTR( button_wstr, buffer_size - 1,  (uint8*)button_str  , buffer_size -1  , button_wstr_length );
    name_str_temp.wstr_ptr = button_wstr;
    str_len = (buffer_size - 1) > button_wstr_length ? button_wstr_length: (buffer_size - 1);
    name_str_temp.wstr_len = str_len;
    rect.top += MMIWIDGET_SPEED_DIAL_NUMBER_SPACE_TOP;
    rect.bottom -= MMIWIDGET_SPEED_DIAL_NUMBER_SPACE_BOTTOM;
    GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&name_str_temp,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    //计算TEXT 区域
    //显示快捷拨号名称
    text_style.font = MMIWIDGET_SPEED_DIAL_TEXT_FONT;
    if(index % 3 == 0)
    {
    	rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_NAME_POS_X;
    }
    else
    {
    	rect.left =(win_rect.left + MMIWIDGET_SPEED_DIAL_NAME_POS_X) + (MMIWIDGET_SPEED_DIAL_H_SPACE + MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH)* (index%3) ;
    }
    rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_NAME_POS_Y) + (MMIWIDGET_SPEED_DIAL_V_SPACE + MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT)* (index/3) ;
    rect.right = rect.left + MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH;
    rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT;

    name_str_temp.wstr_ptr = dis_str1;
    name_str_temp.wstr_len = SPEEDKEY_PROMPT_LEN; 
    if(index != 0)
    {
    	ReadSpeedDialNameStr(&name_str_temp,dis_str2,(index - 1));
    	GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
    		(const GUI_RECT_T      *)&rect,       //the fixed display area
    		(const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
    		(const MMI_STRING_T    *)&name_str_temp,
    		&text_style,
    		state,
    		GUISTR_TEXT_DIR_AUTO
    		);
    }
}
/*****************************************************************************/
//  Description : display SpeedDial unselect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void WidgetSpeedDial_UnSelect(
                           MMI_WIN_ID_T     win_id,
                           uint16 index
                           )
{
    GUI_RECT_T		win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T			     rect = {0};
    GUI_RECT_T			     img_rect = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_NOT_HALF_CHAR;  
    
    MMI_STRING_T name_str_temp={0};
    wchar dis_str1[SPEEDKEY_PROMPT_LEN + 1] = {0};
    wchar dis_str2[SPEEDKEY_PROMPT_LEN + 1] = {0};

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = MMIWIDGET_SPEED_DIAL_BIG_FONT;
    text_style.font_color = MMIWIDGET_SPEED_DIAL_TEXT_COLOR;
    text_style.line_space = 4;
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    //计算背景区域
    if(index % 3 == 0)
    {
        rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_SE_POS_X + 1;
        rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_SE_POS_Y) + (MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT)* (index/3) ;
        rect.right = rect.left + MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH - 1;
        rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT;
    }
    else if(index % 3 == 2)
    {
        rect.left =(win_rect.left + MMIWIDGET_SPEED_DIAL_SE_POS_X) + (MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH)* (index%3) ;
        rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_SE_POS_Y) + (MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT)* (index/3) ;
        rect.right = rect.left + MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH - 1;
        rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT;
    }
    else
    {
        rect.left =(win_rect.left + MMIWIDGET_SPEED_DIAL_SE_POS_X) + (MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH)* (index%3) ;
        rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_SE_POS_Y) + (MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT)* (index/3) ;
        rect.right = rect.left + MMIWIDGET_SPEED_DIAL_SE_RECT_WIDTH;
        rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_SE_RECT_HEIGHT;
    }

    img_rect.left = rect.left - win_rect.left;
    img_rect.top = rect.top - win_rect.top;
    img_rect.right = rect.right -win_rect.left;
    img_rect.bottom = rect.bottom - win_rect.top;
    UILAYER_ClearRect(&lcd_dev_info, rect);//清理
    //显示背景图
    GUIRES_DisplayImg(PNULL,&rect, &img_rect, win_id,
        IMAGE_WIDGET_SPEED_DIAL_BG, &lcd_dev_info);
    
    //计算TEXT 区域
    //显示快捷拨号名称
    text_style.font = MMIWIDGET_SPEED_DIAL_TEXT_FONT;
    rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_NAME_POS_X;
    rect.top = win_rect.top + MMIWIDGET_SPEED_DIAL_NAME_POS_Y;
    if(index % 3 == 0)
    {
    	rect.left = win_rect.left + MMIWIDGET_SPEED_DIAL_NAME_POS_X;
    }
    else
    {
    	rect.left =(win_rect.left + MMIWIDGET_SPEED_DIAL_NAME_POS_X) + (MMIWIDGET_SPEED_DIAL_H_SPACE + MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH)* (index%3) ;
    }
    rect.top = (win_rect.top + MMIWIDGET_SPEED_DIAL_NAME_POS_Y) + (MMIWIDGET_SPEED_DIAL_V_SPACE + MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT)* (index/3) ;
    rect.right = rect.left + MMIWIDGET_SPEED_DIAL_NAME_RECT_WIDTH;
    rect.bottom = rect.top + MMIWIDGET_SPEED_DIAL_NAME_RECT_HEIGHT;

    name_str_temp.wstr_ptr = dis_str1;
    name_str_temp.wstr_len = SPEEDKEY_PROMPT_LEN; 
   
    if(index != 0)
    {
        ReadSpeedDialNameStr(&name_str_temp,dis_str2,(index -1));
    	GUISTR_DrawTextToLCDInRect(        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
    		(const GUI_RECT_T      *)&rect,       //the fixed display area
    		(const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
    		(const MMI_STRING_T    *)&name_str_temp,
    		&text_style,
    		state,
    		GUISTR_TEXT_DIR_AUTO
    		);
    }
}
#endif
#endif


