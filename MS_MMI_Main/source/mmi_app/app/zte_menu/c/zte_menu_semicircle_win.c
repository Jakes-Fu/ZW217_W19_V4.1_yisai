#include "dal_time.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "guibutton.h"
#include "guifont.h"
#include "mmi_theme.h"
#include "mmidisplay_data.h"

#include "os_api.h"
#include "mmk_type.h"

#include "mmi_default.h"
#include "mmiacc_id.h"
#include "mmiacc_position.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "mmiset_export.h"
#include "mmiacc_nv.h"
#include "guistring.h"

#include <math.h>
#include "zte_menu_id.h"
#include "zte_menu_image.h"
#include "zte_menu_win.h"
#include "zdt_text.h"
#include "watch_launcher_main.h"
#if defined(ZDT_VIDEOCHAT_SUPPORT) && defined(VIDEO_CALL_AGORA_SUPPORT) //������Ƶͨ��
#include "video_call_text.h"
#endif

#ifdef XYSDK_SUPPORT
#include "libxmly_api.h"
#endif
/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef void (*MENUCLICKFUNC) (void);

/*! @struct  GUIZTEMENU_ITEM_T
@brief �˵������ݽṹ
*/
typedef struct
{
    MENUCLICKFUNC                      event_func;                 /*!< �����¼����� */
    MMI_IMAGE_ID_T              icon_id;                     /*!< �˵�ͼ��ID */
    MMI_TEXT_ID_T              text_id;                     /*!< �˵�����ID */
    int is_need_requirement;                                   //�Ƿ���Ҫ���� 0����Ҫ��1��Ҫ�� 2��Ҫ����
} GUIZTEMENU_ITEM_T;

/*! @struct  GUIZTEMENU_ITEM_CLICZzz
@brief ������ݽṹ
*/
typedef struct
{
    GUI_RECT_T rect;//�������
    int     idx;                 /*!< �˵��±� */
}GUIZTEMENU_ITEM_CLICK_T;

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
PUBLIC void MMIPUB_OpenAlertWarningWin(MMI_TEXT_ID_T text_id);

//Ai����
#ifdef TULING_AI_SUPPORT
PUBLIC void MMIAPIMENU_EnterAiChat(void);
#endif



//������
PUBLIC void MMIZDT_Open2VMSelectWin(void);

//���
#ifdef ZTE_STOPWATCH_SUPPORT
PUBLIC void ZTE_STOPWATCH_OpenMainWin( void );
#else
PUBLIC void WatchStopWatch_MainWin_Enter( void );
#endif

//�Ʋ�
#ifdef ZTE_PEDOMETER_SUPPORT
PUBLIC void MMIPEDOMETER_CreateTodayWin();
#endif

//����
PUBLIC void MMIZDT_OpenDialWin(void );
//ͨѶ¼
PUBLIC void MMIZDT_OpenPBWin(void);

//����
#ifdef ZTE_WEATHER_SUPPORT
PUBLIC void ZTEWEATHER_CreateMainWin();
#else
PUBLIC void MMIZDT_OpenWeatherWin();
#endif

//�Ӻ���
PUBLIC void MMIZDT_OpenFindFriendWin();
PUBLIC void MMIZDT_OpenFriendPPWin(void);
//�γ̱�
PUBLIC void MMIZDT_OpenScheduleWin(void);
#if defined(ZDT_VIDEOCHAT_SUPPORT) && defined(VIDEO_CALL_AGORA_SUPPORT) //������Ƶͨ��
//��Ƶͨ��
PUBLIC void MMIVideo_Call_MainWin();
#endif
//���
PUBLIC BOOLEAN MMIAPIDC_OpenPhotoWin(void);
//���
PUBLIC void WatchGallery_MainWin_Enter(void);

//����
PUBLIC void WatchSET_MainWin_Enter( void );
//����
PUBLIC void Settings_CallRingSelectWin_Enter( void );
//��������
PUBLIC void MMIAPISET_ZdtBrightnessWin(void);
//���ⳬʱ
PUBLIC void Settings_BackLight_Enter( void );
//����
PUBLIC void MMIAPISET_ZdtVolumeWin(void);
//�ָ�����
PUBLIC void MMIAPISET_ZdtPowerRestoreWin(void);
//�ػ�
PUBLIC void MMIAPISET_ZdtPowerOffWin(void);
//����
PUBLIC void MMIAPISET_ZdtPowerRestartWin(void);
//����
PUBLIC void Settings_RegardWin_Enter(void);

//΢��
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
PUBLIC void MMIAPIMENU_EnterTinyChat();
#endif
#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
extern void MMIZDT_OpenChatGroupWin();
#endif 
//����
#ifdef LOCAL_ALARM_CLOCK_SUPPORT
PUBLIC void MMIALARMCLOCK_CreateMainWin();
#else
PUBLIC void WatchAlarm_MainWin_Enter( void );
#endif

//֧����
#ifdef ZDT_ZFB_SUPPORT
PUBLIC void MMIZFB_OpenMainWin();
#endif

#ifdef LEBAO_MUSIC_SUPPORT
PUBLIC void MMIAPIMENU_EnterLebao(void);//�乾����
#endif
#if defined(XYSDK_SUPPORT)|| defined(XYSDK_SRC_SUPPORT)
 //ϲ������
#endif
#ifdef MAINMENU_STYLE_SUPPORT
PUBLIC void WatchSET_MenuStyle_Enter( void );//�˵��л�
#endif
PUBLIC void WatchOpen_Panel_SelectWin(void); //�����л�

#if defined(TORCH_SUPPORT) // wuxx add
extern PUBLIC BOOLEAN MMIACC_OpenFlashlighWin(void);
#endif

#ifdef XYSDK_SUPPORT
PUBLIC void MMIZDT_OpenXmlyWin(void)
{
    LIBXMLYAPI_CreateXysdkMainWin();
    return ;
}
#endif

LOCAL void temp_test(){}
/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
#define START_Y 28
LOCAL GUIZTEMENU_ITEM_T	ZTE_MENUS[]  =
{ 
#ifdef MAINMENU_STYLE_SUPPORT
    {WatchSET_MenuStyle_Enter,ZTE_MENU_STYLE,TXT_MENU_STYLE_SET,0},//���˵�����л�
#endif
    {WatchOpen_Panel_SelectWin,ZTE_MENU_PANEL,TXT_SETTINGS_WATCH_SWITCH,0},//�����л�
    {MMIZDT_OpenDialWin,ZTE_MENU_BH,TXT_DIALER,0},//����
#ifdef ZDT_PLAT_YX_SUPPORT_VOICE
#ifdef ZDT_PLAT_YX_SUPPORT_FRIEND
    {MMIZDT_OpenChatGroupWin,ZTE_MENU_WL,TXT_WECHART,2},//΢�� �����б�
#else       //ZDT_PLAT_YX_SUPPORT_FRIEND
    {MMIAPIMENU_EnterTinyChat,ZTE_MENU_WL,TXT_WECHART,2},//΢��
#endif      //ZDT_PLAT_YX_SUPPORT_FRIEND
#endif
    {MMIZDT_OpenPBWin,ZTE_MENU_TX,TXT_CONTACT,0},//ͨѶ¼

    {MMIZDT_Open2VMSelectWin,ZTE_MENU_BD,TXT_BIND_STEP,0},//��
#if defined(ZDT_VIDEOCHAT_SUPPORT) && defined(VIDEO_CALL_AGORA_SUPPORT) //������Ƶͨ��
//��Ƶͨ��
    {MMIVideo_Call_MainWin,ZTE_MENU_SP,TXT_VIDEO_CALL,1},//��Ƶͨ��
#endif;
#ifdef    ZDT_PLAT_YX_SUPPORT_FRIEND
    { MMIZDT_OpenFriendPPWin /* MMIZDT_OpenFindFriendWin */,ZTE_MENU_HY,TXT_FIND_FRIEND,2},//�Ӻ���
#endif 
    {MMIAPIDC_OpenPhotoWin,ZTE_MENU_XJ,TXT_LAUNCHER_CAMERA_TITLE,0},//���

    {WatchGallery_MainWin_Enter,ZTE_MENU_XC,TXT_GALLERY,0},//���

#ifdef LOCAL_ALARM_CLOCK_SUPPORT
    {MMIALARMCLOCK_CreateMainWin,ZTE_MENU_NZ,TXT_ALARM_CLOCK,0},//����
#else
    {WatchAlarm_MainWin_Enter,ZTE_MENU_NZ,TXT_ALARM_CLOCK,0},//���� 
#endif
    {WatchSET_MainWin_Enter,ZTE_MENU_SZ,TXT_LAUNCHER_SETTINGS_TITLE,0},//����

    {MMIZDT_OpenScheduleWin,ZTE_MENU_KC,STXT_TOOLS_SCHEDULE,2},//�γ̱�

#ifdef TULING_AI_SUPPORT
    {MMIAPIMENU_EnterAiChat,ZTE_MENU_AI,TXT_AI_CHAT,2},//ai����
#endif

#ifdef ZTE_WEATHER_SUPPORT
    {ZTEWEATHER_CreateMainWin,ZTE_MENU_TQ,TXT_WEATHER,2},//����
#else
    {MMIZDT_OpenWeatherWin,ZTE_MENU_TQ,TXT_WEATHER,2},//����
#endif
#ifdef LEBAO_MUSIC_SUPPORT
    {MMIAPIMENU_EnterLebao,ZTE_MENU_MUSIC,TXT_MIGU_MUSIC,1},//�乾����
#endif
#if defined(XYSDK_SUPPORT)|| defined(XYSDK_SRC_SUPPORT)
    {MMIZDT_OpenXmlyWin,ZTE_MENU_XMLY,TXT_XMLY,1}, //ϲ������
#endif
#ifdef ZDT_ZFB_SUPPORT
    {MMIZFB_OpenMainWin,ZTE_MENU_ZF,TXT_ALIPAY,1},//֧����
#endif

#ifdef ZTE_PEDOMETER_SUPPORT
    {MMIPEDOMETER_CreateTodayWin,ZTE_MENU_JB,TXT_PEDOMETE},//�Ʋ�
#endif

#ifdef ZTE_STOPWATCH_SUPPORT
    {ZTE_STOPWATCH_OpenMainWin,ZTE_MENU_MB,TXT_LAUNCHER_STOPWATCH_TITLE,0},//���
#else
	{WatchStopWatch_MainWin_Enter,ZTE_MENU_MB,TXT_LAUNCHER_STOPWATCH_TITLE,0},//���
#endif

    //{Settings_BackLight_Enter,ZTE_MENU_BG,ZTE_MENU_BG_T},//���ⳬʱ
    // {temp_test,ZTE_MENU_RL,ZTE_MENU_RL_T},//����
    // {temp_test,ZTE_MENU_SD,ZTE_MENU_SD_T},//�ֵ�Ͳ
    // {temp_test,ZTE_MENU_SJ,ZTE_MENU_SJ_T},//Զ������
    //{MMIAPISET_ZdtPowerRestartWin,ZTE_MENU_CQ,ZTE_MENU_CQ_T},//����
    //{MMIAPISET_ZdtPowerRestoreWin,ZTE_MENU_FH,ZTE_MENU_FH_T},//�ָ�����
    //{MMIAPISET_ZdtPowerOffWin,ZTE_MENU_GJ,ZTE_MENU_GJ_T},//�ػ�
    //{Settings_RegardWin_Enter,ZTE_MENU_GY,ZTE_MENU_GY_T},//����
    //{MMIAPISET_ZdtVolumeWin,ZTE_MENU_YL,ZTE_MENU_YL_T},//����
    //{MMIAPISET_ZdtBrightnessWin,ZTE_MENU_LD,ZTE_MENU_LD_T},//��������
    //{Settings_CallRingSelectWin_Enter,ZTE_MENU_LS,ZTE_MENU_LS_T},//����

};
LOCAL int ZTE_MENU_LEN =sizeof(ZTE_MENUS)/sizeof(GUIZTEMENU_ITEM_T);
#define CUTTING_X 30
#define MAX_CLICK_RECT 5 //��ͼ���ڿɼ���������
LOCAL GUIZTEMENU_ITEM_CLICK_T menu_click_rects[MAX_CLICK_RECT];
#define ROUND_X  0
#define ROUND_Y  142.0 // Բ������
#define ROUND_R  130.0 // Բ�İ뾶
#define PERIMETER_MOVE 70.0 //����ͼ��֮����ܳ�����
#define ICON_IMG_WIDTH 50 
#define ICON_IMG_HEIGHT 50 
#define TOP_AND_BOTTOM_CLICK_OFFSET 10 
#define ICON_TEXT_OFFSET 3 
#define NAME_TEXT_WIDTH 100
LOCAL double ANGLE_MOVE=PERIMETER_MOVE/ROUND_R;//����ͼ��֮��Ļ���
LOCAL double cur_poin_x=0;
LOCAL double cur_poin_y=0;
LOCAL double all_move_perimeter=0.0;//���ƶ�����
LOCAL double move_perimeter=0.0;//��ǰҪ�ƶ�����
LOCAL GUI_POINT_T down_point={0};
LOCAL int last_y=0;
LOCAL float fling_velocity = 0; 
LOCAL uint8 fling_timer_id = 0;
LOCAL int  fling_offset_y = 0; 

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

LOCAL void CircleMenu_EnableFlingTimer(MMI_WIN_ID_T win_id, BOOLEAN on)
{
	if(on)
	{
	    if(fling_timer_id != 0)
	    {
	        MMK_StopTimer(fling_timer_id);
	    }
	    fling_timer_id =  MMK_CreateWinTimer(win_id, 50, FALSE);
    }
    else
    {
		if(fling_timer_id != 0)
	    {
	        MMK_StopTimer(fling_timer_id);
	    }
	    fling_timer_id = 0;
	    fling_velocity = 0; 
        fling_offset_y = 0;
    }

}

LOCAL void paint_content(MMI_WIN_ID_T win_id,double rx,double ry,int i,int j)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_POINT_T point={0};
    MMI_STRING_T   name_str     = {0};
    GUISTR_STYLE_T text_style = {0};
    GUI_RECT_T name_rect = {0};
    if(rx<CUTTING_X)
    {
        return;
    }
	MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    point.y=ry-ICON_IMG_HEIGHT/2;
    point.x=rx-ICON_IMG_WIDTH/2;
    GUIRES_DisplayImg(&point, PNULL, PNULL, win_id,ZTE_MENUS[i].icon_id, &lcd_dev_info);
    text_style.font = SONG_FONT_23;
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.align = ALIGN_LVMIDDLE;
    name_rect.top = point.y;
    name_rect.left = point.x + ICON_IMG_WIDTH + ICON_TEXT_OFFSET;
    name_rect.right = name_rect.left + NAME_TEXT_WIDTH;
    name_rect.bottom = name_rect.top + ICON_IMG_HEIGHT;
    MMIRES_GetText(ZTE_MENUS[i].text_id, win_id, &name_str);
    GUISTR_DrawTextToLCDInRect(&lcd_dev_info,&name_rect,&name_rect,&name_str, &text_style,GUISTR_STATE_SINGLE_LINE, GUISTR_TEXT_DIR_AUTO);
//    GUIRES_DisplayImg(&point, PNULL, PNULL, win_id,ZTE_MENUS[i].icon_t_id, &lcd_dev_info);
    menu_click_rects[j].rect.top=ry-ICON_IMG_HEIGHT/2;
    menu_click_rects[j].rect.left=rx-ICON_IMG_WIDTH/2;
    menu_click_rects[j].rect.right=rx+150;
    menu_click_rects[j].rect.bottom=ry+ICON_IMG_HEIGHT/2;
    if(menu_click_rects[j].rect.top < 3)
    {
        menu_click_rects[j].rect.top=ry-ICON_IMG_HEIGHT/2-TOP_AND_BOTTOM_CLICK_OFFSET;
        menu_click_rects[j].rect.left=rx-ICON_IMG_WIDTH/2;
        menu_click_rects[j].rect.right=rx+150;
        menu_click_rects[j].rect.bottom=ry+ICON_IMG_HEIGHT/2-TOP_AND_BOTTOM_CLICK_OFFSET;
    }
    if(menu_click_rects[j].rect.bottom > MMI_MAINSCREEN_HEIGHT)
    {
        menu_click_rects[j].rect.top=ry-ICON_IMG_HEIGHT/2+TOP_AND_BOTTOM_CLICK_OFFSET;
        menu_click_rects[j].rect.left=rx-ICON_IMG_WIDTH/2;
        menu_click_rects[j].rect.right=rx+150;
        menu_click_rects[j].rect.bottom=ry+ICON_IMG_HEIGHT/2;
    }
    menu_click_rects[j].idx=i;
}

LOCAL void paint_item(MMI_WIN_ID_T win_id)
{
    int i=0,j=0,start=0;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_POINT_T point={0};
    double ry=0.0,rx=0.0;

    all_move_perimeter+=move_perimeter;
    
    if(all_move_perimeter==0)
    {
        double part_of_equation=0;
        ry=START_Y;
        part_of_equation = ROUND_R * ROUND_R- (ry - ROUND_Y) * (ry - ROUND_Y);
        rx=ROUND_X + sqrt(part_of_equation);
        cur_poin_x=rx;
        cur_poin_y=ry;
        paint_content(win_id,rx,ry,i++,j++);
        for(;i<MAX_CLICK_RECT;i++)
        {
            double theta = atan2(ry - ROUND_Y, rx - ROUND_X)+ANGLE_MOVE;
            rx = ROUND_X + ROUND_R * cos(theta);
            ry = ROUND_Y + ROUND_R * sin(theta);
            paint_content(win_id,rx,ry,i,j++);
        }
    }else if(all_move_perimeter<0)
    {
        int all_num=(-all_move_perimeter)/PERIMETER_MOVE;
        int start_idx=all_num%ZTE_MENU_LEN;
        double theta = atan2(cur_poin_y - ROUND_Y, cur_poin_x - ROUND_X)+move_perimeter/ROUND_R;
        rx = ROUND_X + ROUND_R * cos(theta);
        ry = ROUND_Y + ROUND_R * sin(theta);
        cur_poin_x=rx;
        cur_poin_y=ry;

        theta = atan2(cur_poin_y - ROUND_Y, cur_poin_x - ROUND_X)+all_num*PERIMETER_MOVE/ROUND_R;
        rx = ROUND_X + ROUND_R * cos(theta);
        ry = ROUND_Y + ROUND_R * sin(theta);
        paint_content(win_id,rx,ry,start_idx,j++);
        for(i=start_idx+1;i<start_idx+MAX_CLICK_RECT+1;i++)
        {
            theta = atan2(ry - ROUND_Y, rx - ROUND_X)+ANGLE_MOVE;
            rx = ROUND_X + ROUND_R * cos(theta);
            ry = ROUND_Y + ROUND_R * sin(theta);
            if(i<ZTE_MENU_LEN)
            {
                paint_content(win_id,rx,ry,i,j++);
            }else
            {
                paint_content(win_id,rx,ry,i-ZTE_MENU_LEN,j++);
            }
        }
    }else if(all_move_perimeter>0)
    {
        int all_num=all_move_perimeter/PERIMETER_MOVE;
        int start_idx=all_num%ZTE_MENU_LEN;
        double theta = atan2(cur_poin_y - ROUND_Y, cur_poin_x - ROUND_X)+move_perimeter/ROUND_R;
        rx = ROUND_X + ROUND_R * cos(theta);
        ry = ROUND_Y + ROUND_R * sin(theta);
        cur_poin_x=rx;
        cur_poin_y=ry;

        theta = atan2(cur_poin_y - ROUND_Y, cur_poin_x - ROUND_X)-all_num*PERIMETER_MOVE/ROUND_R;
        rx = ROUND_X + ROUND_R * cos(theta);
        ry = ROUND_Y + ROUND_R * sin(theta);
        if(start_idx>0)
        {
            paint_content(win_id,rx,ry,ZTE_MENU_LEN-start_idx,j++);
        }else
        {
            paint_content(win_id,rx,ry,-start_idx,j++);
        }
        
        theta = atan2(ry - ROUND_Y, rx - ROUND_X)-ANGLE_MOVE;
        if(start_idx+1>0)
        {
            paint_content(win_id,ROUND_X + ROUND_R * cos(theta),ROUND_Y + ROUND_R * sin(theta),ZTE_MENU_LEN-start_idx-1,j++);
        }else
        {
            paint_content(win_id,ROUND_X + ROUND_R * cos(theta),ROUND_Y + ROUND_R * sin(theta),-start_idx-1,j++);
        }
        

        for(i=start_idx-1;i>start_idx-MAX_CLICK_RECT-1;i--)
        {
            theta = atan2(ry - ROUND_Y, rx - ROUND_X)+ANGLE_MOVE;
            rx = ROUND_X + ROUND_R * cos(theta);
            ry = ROUND_Y + ROUND_R * sin(theta);
            if(i>0)
            {
                paint_content(win_id,rx,ry,ZTE_MENU_LEN-i,j++);
            }else
            {
                paint_content(win_id,rx,ry,-i,j++);
            }
        }
    }
    
    for(;j<MAX_CLICK_RECT;j++)
    {
        menu_click_rects[j].rect.top=0;
        menu_click_rects[j].rect.left=0;
        menu_click_rects[j].rect.right=0;
        menu_click_rects[j].rect.bottom=0;
    }
    move_perimeter=0;
}

LOCAL void CircleMenu_TimerCallBack(MMI_WIN_ID_T win_id,DPARAM param)
{
    if (*(uint8*)param == fling_timer_id)
    {
		fling_offset_y = MMK_GetFlingOffset(fling_offset_y, fling_velocity, (float)FLING_TIME, &fling_velocity);//FLING_TIME
        SCI_TRACE_LOW("fling_offset_y:%d fling_velocity:%f",fling_offset_y,fling_velocity);
		if(fling_offset_y > 0) //���»�
		{
            //move_perimeter = 10;
		}
		else //���ϻ�
		{
            //move_perimeter = -10;
		}
					
		if(fling_velocity != 0)
		{
            move_perimeter = fling_velocity/20;
			CircleMenu_EnableFlingTimer(win_id,TRUE);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
		}
        else
        {
            //move_perimeter = fling_velocity/4;
        }
    }
}

LOCAL void CircleMenu_TpUp(MMI_WIN_ID_T win_id,DPARAM param)
{
    int i=0;
    GUI_POINT_T point = {0};
    int x = MMK_GET_TP_X(param);
    int y = MMK_GET_TP_Y(param);
    point.x = x;
    point.y = y;
    if(abs(down_point.x-x)<5&&abs(down_point.y-y)<5)
    {
        for(;i<MAX_CLICK_RECT;i++)
        {
            if(GUI_PointIsInRect(point,menu_click_rects[i].rect))
            {
                if(ZTE_MENUS[menu_click_rects[i].idx].is_need_requirement==1)
                {
                    if( MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1) != SIM_STATUS_OK)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_NO_SIM_OR_DATA);
                    }else
                    {
                        ZTE_MENUS[menu_click_rects[i].idx].event_func();
                    }
                }else if(ZTE_MENUS[menu_click_rects[i].idx].is_need_requirement==2)
                {
                    if(MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1) != SIM_STATUS_OK)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_NO_SIM_OR_DATA);
                    }else
                    {
                        ZTE_MENUS[menu_click_rects[i].idx].event_func();
                    }
                }else
                {
                    ZTE_MENUS[menu_click_rects[i].idx].event_func();
                }
                return;
            }
        }
    }
    else
    {
        MMK_ComputeCurrentVelocity(PNULL, &fling_velocity, FLING_MAX_VELOCITY, FLING_MAX_VELOCITY);
        if(fling_velocity != 0)
        {
			CircleMenu_EnableFlingTimer(win_id,TRUE);
        }
    }
}

PUBLIC MMI_RESULT_E HandleMsgMenuSemicircleWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode      = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
           
        }
        break;
        case MSG_GET_FOCUS:
            //WatchLAUNCHER_MainMenuSwitch(win_id);
            break;

        case MSG_FULL_PAINT:
        {
            if(MMK_GetFocusWinId() == win_id)
            {
                GUI_LCD_DEV_INFO lcd_dev_info = {0};
                GUI_POINT_T point={0};
                GUI_RECT_T rect={0,0,240,284};
                MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
           	    GUIRES_DisplayImg(&point, PNULL, PNULL, win_id,ZTE_MENU_SEMICIRCLE_BG, &lcd_dev_info);
                paint_item(win_id);
            }
        }
        break;

        case MSG_TP_PRESS_DOWN:
        {
           down_point.y=MMK_GET_TP_Y(param);
           down_point.x=MMK_GET_TP_X(param);
           last_y=down_point.y;
           CircleMenu_EnableFlingTimer(win_id,FALSE);
           MMK_ResetVelocityItem(); 
        }
        break;
        case MSG_TP_PRESS_UP:
        {
            if(MMK_GetFocusWinId() == win_id)
            {
                CircleMenu_TpUp(win_id,param);
            }
        }
        break;
        case MSG_TP_PRESS_MOVE:
        {
            int y = MMK_GET_TP_Y(param);
            move_perimeter =  y-last_y;
            last_y=y;
            if(move_perimeter!=0)
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;

        case MSG_CTL_OK:
        case MSG_APP_OK:
        {
           
        }
        break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_KEYDOWN_RED:
        case MSG_KEYUP_RED:
            break;
        case MSG_TIMER:
            if(MMK_GetFocusWinId() == win_id)
            {
               CircleMenu_TimerCallBack(win_id,param);
            }
            break;

        case MSG_CLOSE_WINDOW:
        {
            cur_poin_x=0;
            cur_poin_y=0;
            all_move_perimeter=0.0;
            move_perimeter=0.0;
        }
        break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

// window table of stop watch win
WINDOW_TABLE(ZTE_MENU_SEMICIRCLE_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_ID(ZTE_MENU_SEMICIRCLE_WIN_ID),
    WIN_FUNC((uint32)HandleMsgMenuSemicircleWindow),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};


PUBLIC void zte_menu_semicircle_open_win( void )
{
    if (MMK_IsOpenWin(ZTE_MENU_SEMICIRCLE_WIN_ID))
    {
        MMK_CloseWin(ZTE_MENU_SEMICIRCLE_WIN_ID);
    }
    MMK_CreateWin((uint32 *)ZTE_MENU_SEMICIRCLE_WIN_TAB,PNULL);
}

PUBLIC void zte_menu_semicircle_close_win( void )
{
    MMK_CloseWin(ZTE_MENU_SEMICIRCLE_WIN_ID);
}
