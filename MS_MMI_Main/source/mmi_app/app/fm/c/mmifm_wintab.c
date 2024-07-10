/*****************************************************************************
** File Name:      mmifm_wintab.c                                            *
** Author:         baokun.yin                                                *
** Date:           2008/12/17                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe FM window                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008/12/17                                                                *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmi_app_fm_trc.h"
#ifdef FM_SUPPORT
#include "window_parse.h"
//#include "sci_types.h"
#include "mmi_default.h"
#include "mmifm_export.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmifm_text.h"
#include "mmifm_image.h"
#include "mmialarm_image.h"
#include "mmialarm_text.h"
#include "mmifm_id.h"
#include "mmipub.h"
#include "mmk_timer.h"
#include "mmifm_position.h"
#include "mmi_mainmenu_export.h"
//#include "mmimp3_image.h"
#include "mmifm_internal.h"
#include "guilabel.h"
#include "mmi_appmsg.h"
#include "guibutton.h"
#include "guitext.h"
#include "gpio_prod_api.h"
#include "guimenu.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "guires.h"
#include "mmiidle_export.h"
#include "guistring.h"
#include "guiform.h"
#include "mmicom_panel.h"
#include "mmisrvaud_api.h"
#include "guiownerdraw.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiudisk_export.h"
#include "mmicc_export.h"
#include "mmiacc_event.h"
#include "mmialarm_export.h"
#include "mmiidle_subwintab.h"
#include "guisetlist.h"
#include "mmi_text.h"
#ifdef FM_NEW_UI
#include "Mmk_tp.h"
#endif
#include "guictrl_api.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
//#include <math.h>
#include "graphics_draw.h"
#include "mmimultim_image.h"
#include "mmiphone_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/      

#define MMIFM_SMALL_TEXT_COLOR               0xcff6//0x045b
#define MMIFM_SMALL_TEXT_FONT                SONG_FONT_12
#define MMIFM_ANI_TIME      10
#define MMIFM_INPUTR_PERIOD 3000
#define MMIFM_FONT_CLOLOR   MMI_WHITE_COLOR

//pop menu
#define  MMIFM_POP_MENU_NODE_ID    0
#define  MMIFM_POP_MENU_TEXT_ID    1
#define  MMIFM_POP_MENU_ITEM_DIMEN 2 
//draw item
#define MMIFM_DRAW_FULL               0xffff
#define MMIFM_DRAW_ADJ_FREQ           0X0001
#define MMIFM_DRAW_ADJ_FREQ_LEFT_BUT  0X0002
#define MMIFM_DRAW_ADJ_FREQ_RIGHT_BUT 0X0004
#define MMIFM_DRAW_LEFT_ARROW         0X0008
#define MMIFM_DRAW_RIGHT_ARROW        0X0010
#define MMIFM_DRAW_UP_ARROW           0X0020
#define MMIFM_DRAW_DOWN_ARROW         0X0040
#define MMIFM_DRAW_CHANNEL_NAME       0X0080
#define MMIFM_DRAW_FREQ_TEXT          0X0100
#define MMIFM_DRAW_VOLUME             0X0200
#define MMIFM_DRAW_SPK                0X0400
#define MMIFM_CHANNEL_NUMBER_MAX_BIT  2
#define MMIFM_CHANNEL_TITLE           "CH"
#define MMIFM_CHANNEL_TITLE_ADD_MAX_LEN 10   

#define  MMIFM_GET_UP_VALUE_STEP    1
#define  MMIFM_GET_DOWN_VALUE_STEP    -1
#ifdef MMIFM_SUPPORT_FM_RECORD
static uint8                s_time_timer_id = 0;  //record timer
static uint32               s_record_cur_timer_elapsed = 0; //cur timer elapsed in seconds
#endif
#define MMIFMRECORD_TIME_STRING_LENGTH      9

#ifdef MMI_TASK_MANAGER
#define MMK_CreateFmWin( _WIN_TABLE_, _ADD_DATA_ )  OpenFmWin( _WIN_TABLE_, _ADD_DATA_ )
#else
#define MMK_CreateFmWin   MMK_CreateWin
#endif
//#define PI  3.14159265358979323846  /* pi */
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


typedef enum
{
    MMIFM_OPT_NULL,
    MMIFM_FULL_AUTO_SEARCH,//自动搜索
    MMIFM_CHANNEL_LIST,//频道列表
    MMIFM_SAVE_FREQ, //保存
    MMIFM_MANUAL_SEARCH,//手动输入频率
    MMIFM_PLAY_MODE,//用耳机、手机播放
    MMIFM_SET_BG_PLAY,//后台播放
#ifdef MMIFM_SUPPORT_EQ_MODE
    MMIFM_SET_EQ_MODE,
    MMIFM_EQ_REGULAR, //常规
    MMIFM_EQ_CLASSIC, //古典
    MMIFM_EQ_ODEUM,  //音乐厅
    MMIFM_EQ_JAZZ, //爵士
    MMIFM_EQ_ROCK,  //摇滚
    MMIFM_EQ_SOFT_ROCK,  //轻摇滚 
    MMIFM_EQ_MAX,
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
    MMIFM_RECORD,
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    MMIFM_RECORD_TIMER,
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
    MMIFM_RECORD_FILE,
    MMIFM_RECORD_FILE_LOCATION,
#endif
#ifdef MMIFM_SUPPORT_FORMAT_AMR    	
    MMIFM_RECORD_FILE_FORMAT,
    MMIFM_RECORD_FORMAT_AMR,   
    MMIFM_RECORD_FORMAT_WAV,  
#endif 
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMI_FM_TIMER_RECORD_SUPPORT)        
    MMIFM_RECORD_FILE_UDISK,
    MMIFM_RECORD_FILE_MEMCARD_MAX = MMIFM_RECORD_FILE_UDISK + MMI_SDCARD_MAX_NUM,
#endif    
    MMIFM_HELP,//帮助
    
    MMIFM_CHANNEL_PLAY,//播放
    MMIFM_CHANNEL_EDIT,//编辑
    MMIFM_CHANNEL_DELETE,//删除
    MMIFM_CHANNEL_DELETE_ALL,//删除全部
#ifdef FM_RDS_SUPPORT
    MMIFM_RDS_ENABLE, 
#endif   
    MMIFM_OPT_MAX    
}MMIFM_OPT_E;

typedef enum
{
    MMIFM_BUTTON_ID,
    MMIFM_BUTTON_UN_IMAGE,
    MMIFM_BUTTON_PR_IMAGE,  
    MMIFM_BUTTON_GR_IMAGE,    
    MMIFM_BUTTON_INFO_NUM
} MMIFM_BUTTON_INFO_E;


typedef enum
{
    MMIFM_OPERATION_LIST,
    MMIFM_SELECTION_LIST
}MMIFM_CHANNEL_LIST_TYPE_E;

typedef enum
{
    MMIFM_DO_NOTHING,//无操作
    MMIFM_SAVE_FREQUENCY,//保存当前频率
    MMIFM_EDIT_CHANNEL_NAME,//编辑电台名称
    MMIFM_EDIT_CHANNEL_FREQ,//编辑电台频率 
    MMIFM_DELETE_ONE_CHANNEL,//删除当前电台 
    MMIFM_DELETE_ALL_CHANNEL,//删除全部电台 
    MMIFM_MANAUL_SEARCH
}MMIFM_OPERTION_TYPE_E;

typedef enum
{
    MMIFM_CHANNEL_NAME,
    MMIFM_CHANNEL_FREQ
}MMIFM_CHANNEL_DETAILS_POS ;
#ifdef MMIFM_SUPPORT_FM_RECORD
typedef struct
{
    uint8                   second_low;
    uint8                   second_high;
    uint8                   minute_low;
    uint8                   minute_high;
    uint8                   hour_low;
    uint8                   hour_high;
}MMIFMRECORD_TIME_T;
typedef enum
{
        MMIFMRECORD_BUTTON_VOLUME,     //音量
        MMIFMRECORD_BUTTON_PLAY_PAUSE,//播放停止录音
        MMIFMRECORD_BUTTON_STOP,      //停止播放或者录音
        MMIFMRECORD_BUTTON_RECORD_PAUSE,  //录音    
        MMIFMRECORD_BUTTON_LIST,      //列表录音文件
        MMIFMRECORD_BUTTON_MAX
}MMIFMRECORD_BUTTON_E;
typedef struct 
{
    MMIFILE_DEVICE_E    dev;
    uint32              record_file_id;
    uint32              call_file_id;
}MMIFMRECORD_SETTING_INFO_T;
#endif
typedef struct 
{
    const void				*data_ptr;      
    uint32					data_size;
    IMGREF_DATA_FORMAT_E    data_type;      
    uint16					width;             
    uint16					height;
    int16					center_x;       //horizontal coordinate of rotation center 
    int16					center_y;       //vertical coordinate of rotation center 
}MMIFM_IMAGE_T;
typedef struct
{
    int  freq;         
    int  angle;        
}MMIFM_POINT_DATA_T;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
const uint32 s_fm_main_opt_item[][MMIFM_POP_MENU_ITEM_DIMEN] = 
{
    {MMIFM_FULL_AUTO_SEARCH, TXT_FM_FULL_AUTO_SEARCH},//自动搜索

#ifdef MMI_PDA_SUPPORT
#ifdef  MMIFM_SUPPORT_FM_RECORD 
    {MMIFM_RECORD,TXT_FM_RECORD},
#endif
#else
    {MMIFM_CHANNEL_LIST, TXT_COMM_CHANNEL_LIST},//频道列表
#endif

    {MMIFM_SAVE_FREQ, STXT_SAVE},//保存

#ifdef MMI_PDA_SUPPORT
#ifndef MMI_TASK_MANAGER
    {MMIFM_SET_BG_PLAY, TXT_FM_BACKGROUND_PLAY},//后台播放
#endif/*#ifndef MMI_TASK_MANAGER*/    
#else
    {MMIFM_MANUAL_SEARCH, TXT_MANUAL_SEARCH},//手动输入频率
#endif

    {MMIFM_PLAY_MODE, TXT_FM_SPEAKER_MODE},//用耳机播放

#ifdef MMI_PDA_SUPPORT
    {MMIFM_MANUAL_SEARCH, TXT_MANUAL_SEARCH},//手动输入频率
#else
#ifndef MMI_TASK_MANAGER
    {MMIFM_SET_BG_PLAY, TXT_FM_BACKGROUND_PLAY},//后台播放
#endif/*#ifndef MMI_TASK_MANAGER*/    
#endif
#ifdef MMIFM_SUPPORT_EQ_MODE
    {MMIFM_SET_EQ_MODE, TXT_COMMON_EQ_SET},//eq mode set
#endif
#ifdef  MMIFM_SUPPORT_FM_RECORD 
#ifndef MMI_PDA_SUPPORT
    {MMIFM_RECORD,TXT_FM_RECORD},
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    {MMIFM_RECORD_TIMER,TXT_FM_RECORD_TIMER},
#endif
    {MMIFM_RECORD_FILE,TXT_COMMON_RECORD_FILE_LIST},
#if defined(NANDBOOT_SUPPORT) || defined(MMI_UDISK_MEM_ENABLE) 
    {MMIFM_RECORD_FILE_LOCATION,STXT_STORE_MEDIUM},
#endif
#endif
#ifdef MMIFM_SUPPORT_FORMAT_AMR
    {MMIFM_RECORD_FILE_FORMAT,TXT_FILE_FORMAT},
#endif
#ifdef FM_RDS_SUPPORT
     {MMIFM_RDS_ENABLE, TXT_FM_RDS_ENABLE},
#endif
    {MMIFM_HELP, TXT_HELP},//帮助

};
#ifdef MMIFM_SUPPORT_FORMAT_AMR
const uint32 s_fm_main_sub_opt_item [][MMIFM_POP_MENU_ITEM_DIMEN]= 
{
	{MMIFM_RECORD_FORMAT_AMR,   TXT_FM_FORMATAMR},    
    {MMIFM_RECORD_FORMAT_WAV,   TXT_FM_FORMATWAV}   
};
#endif
#ifdef MMIFM_SUPPORT_EQ_MODE
const uint32 s_fm_eq_mode_opt_item [][MMIFM_POP_MENU_ITEM_DIMEN]= 
{
	{MMIFM_EQ_REGULAR,   TXT_COMM_REGULAR},  //常规
    {MMIFM_EQ_CLASSIC,   TXT_COMMON_CLASSICAL}, //古典
    {MMIFM_EQ_ODEUM,   TXT_ODEUM_EQ}, //音乐厅
    {MMIFM_EQ_JAZZ,   TXT_COMMON_EQ_JAZZ}, //爵士
    {MMIFM_EQ_ROCK,   TXT_ROCK_EQ}, //摇滚
    {MMIFM_EQ_SOFT_ROCK,   TXT_COMMON_SOFT_ROCK_EQ}  //轻摇滚 
};
#endif
const uint32 s_fm_channel_opt_item[][MMIFM_POP_MENU_ITEM_DIMEN] = 
{
#ifndef MMI_PDA_SUPPORT
    {MMIFM_CHANNEL_PLAY, TXT_FM_PLAY},//播放
#endif
    {MMIFM_CHANNEL_EDIT, TXT_EDIT},//编辑
    {MMIFM_CHANNEL_DELETE, TXT_DELETE},//删除
#ifndef MMI_PDA_SUPPORT
    {MMIFM_CHANNEL_DELETE_ALL, TXT_DELALL},//删除全部
#endif
};

//lint -esym(551, s_adj_freq_left_rect, s_adj_freq_right_rect, s_left_rect, s_right_rect, s_up_rect, s_down_rect)
LOCAL const GUI_RECT_T s_adj_freq_left_rect = MMIFM_ADJ_LEFT_RECT;
LOCAL const GUI_RECT_T s_adj_freq_right_rect = MMIFM_ADJ_RIGHT_RECT;
LOCAL const GUI_RECT_T s_left_rect = MMIFM_AUTO_LEFT_RECT;
LOCAL const GUI_RECT_T s_right_rect = MMIFM_AUTO_RIGHT_RECT;
LOCAL const GUI_RECT_T s_up_rect = MMIFM_AUTO_UP_RECT;
LOCAL const GUI_RECT_T s_down_rect = MMIFM_AUTO_DOWN_RECT;

#ifdef FM_NEW_UI
MMI_CTRL_ID_T return_button_id = MMIFM_RETURN_BUTTON_CTRL_ID;
MMI_CTRL_ID_T menu_button_id = MMIFM_MENU_BUTTON_CTRL_ID;
#endif

/*   BUTTON ID ----------------------BUTTON RELEASE IMAGE -- BUTTON PRESSED IMAGE--  BUTTON GRAD IMAGE -- BUTTON RECT----*/
#ifdef FM_NEW_UI
LOCAL const uint32 s_button_ids[][MMIFM_BUTTON_INFO_NUM] = {
    {MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,   IMAGE_FM_LEFT_DEFAUT,    IMAGE_FM_LEFT_PRESSED,  IMAGE_FM_LEFT_DISABLED},
    {MMIFM_GET_UPFRE_BUTTON_CTRL_ID,     IMAGE_FM_RIGHT_DEFAULT,  IMAGE_FM_RIGHT_PRESSED, IMAGE_FM_RIGHT_DISABLED},
    {MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID,    IMAGE_FM_VOLUME_DEFAUT,  IMAGE_FM_VOLUME_PRESSED,IMAGE_FM_VOLUME_DISABLED},
    {MMIFM_LIST_BUTTON_CTRL_ID,          IMAGE_FM_LIST_DEFAULT,   IMAGE_FM_LIST_PRESSED,  IMAGE_FM_LIST_DISABLED},
 };
#else
LOCAL const uint32 s_button_ids[][MMIFM_BUTTON_INFO_NUM] = {
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
    {MMIFM_ADJ_FREQ_LEFT_BUTTON_ID,         IMAGE_FM_ADJ_LEFT_DEFAULT,  IMAGE_FM_ADJ_LEFT_PRESSED,  IMAGE_FM_ADJ_LEFT_DISABLED},
    {MMIFM_ADJ_FREQ_RIGHT_BUTTON_ID,        IMAGE_FM_ADJ_RIGHT_DEFAULT, IMAGE_FM_ADJ_RIGHT_PRESSED, IMAGE_FM_ADJ_RIGHT_DISABLED},
#endif
    {MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,      IMAGE_FM_LEFT_DEFAULT,   IMAGE_FM_LEFT_PRESSED,    IMAGE_FM_LEFT_DISABLED},
    {MMIFM_GET_UPFRE_BUTTON_CTRL_ID,        IMAGE_FM_RIGHT_DEFAULT,  IMAGE_FM_RIGHT_PRESSED,   IMAGE_FM_RIGHT_DISABLED},
    {MMIFM_START_SUSPEND_BUTTON_CTRL_ID,    IMAGE_FM_PAUSE_DEFAULT,  IMAGE_FM_PAUSE_PRESSED,   IMAGE_FM_PAUSE_DISABLED},
 };
#endif

LOCAL uint16  s_current_sel_channel = 0;
LOCAL MMIFM_OPERTION_TYPE_E s_current_operator = MMIFM_DO_NOTHING;
LOCAL BOOLEAN  s_is_need_set_bgplay = FALSE;
#ifdef MMIFM_SUPPORT_FM_RECORD
static MMIFMRECORD_BUTTON_E   s_focused_button = MMIFMRECORD_BUTTON_PLAY_PAUSE;
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
LOCAL uint8	s_alarm_hour = 0;
LOCAL uint8	s_alarm_minute = 0;
LOCAL MMIACC_MODE_TYPE_E	s_alarm_fre_mode = 0;
LOCAL MMIACC_MODE_TYPE_E	s_alarm_fre = 0;
//LOCAL EVENT_CURRENT_DATA_T	s_fm_timer_record_event_info = {0, 0, {0}};/*lint !e651*/
LOCAL BOOLEAN s_weeks_button_is_selected[ALM_WEEK_DAY_NUM]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
#endif

#ifdef FM_NEW_UI
LOCAL  GUI_LCD_DEV_INFO s_lcd_dev_info = { 0,UILAYER_NULL_HANDLE };
LOCAL  GUI_LCD_DEV_INFO s_needle_dev_info = { 0,UILAYER_NULL_HANDLE };
LOCAL  GUI_LCD_DEV_INFO s_main_lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
LOCAL  MMIFM_SLIDE_INFO_T  s_slide_info={0};
#endif

wchar       s_channel_name[MMIFM_CHANNEL_NAME_LEN + 1]={0};

extern BOOLEAN MMIAPIENG_GetIQModeStatus(void);
extern void MMIAPIENG_AlertIQMode(void);

static const int tan_0_90_table[90] = 
{
  0,
 17, 34, 52, 69, 87,105,122,140,158,176,
194,212,230,249,267,286,305,324,344,364,
383,404,424,445,466,487,509,531,554,577,
600,624,649,674,700,726,753,781,809,839,
869,900,932,965,1000,1035,1072,1110,1150,1191,
1234,1279,1327,1376,1428,1482,1539,1600,1664,1732,
1804,1880,1962,2050,2144,2246,2355,2475,2605,2747,
2904,3077,3270,3487,3732,4010,4331,4704,5144,5671,
6313,7115,8144,9514,11430,14300,19081,28636,57290,
};

#ifdef MMI_TASK_MANAGER	
/*****************************************************************************/
//  Description : the process message function of the FM
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  FMApplet_HandleEvent(    
            IAPPLET_T*          iapplet_ptr,
            MMI_MESSAGE_ID_E    msg_id, 
            DPARAM              param
            );

typedef struct 
{
    CAF_APPLET_T caf_applet;
    uint32  user_data;
}MMIFM_APPLET_T;   //vt applet定义

LOCAL MMIFM_APPLET_T *s_fm_applet_instance = PNULL; 
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmifm_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_FM_APPLET_ID), sizeof(MMIFM_APPLET_T), FMApplet_HandleEvent , 
        IMAGE_SECMENU_ICON_MULTIM_FM, TXT_FM},
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmifm_applet_info = 
{
    s_mmifm_applet_array,/*lint !e605*/
    ARR_SIZE(s_mmifm_applet_array)
};
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : set current selected channel no. 
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelChannel(uint16 ch_no);

/*****************************************************************************/
//  Description : get current selected channel no.
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetCurrentSelChannel(void);

/*****************************************************************************/
//  Description : set current operation type
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL void SetCurrentOperator(MMIFM_OPERTION_TYPE_E opt_type);

/*****************************************************************************/
//  Description : get current selected channel no.
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL MMIFM_OPERTION_TYPE_E GetCurrentOperator(void);

/*****************************************************************************/
//  Description : create option menu items 
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL void CreateMainOptMenu(uint32 ctr_id);

/*****************************************************************************/
//  Description : create option menu items 
//  Global resource dependence : 
//  Parameter: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateChannelOptionMenu(uint32 ctr_id);

/*****************************************************************************/
//  Description : Open AuotSearch Win
//  Global resource dependence : 
//  Parameter: 
//  Note: 
/*****************************************************************************/
LOCAL void OpenAuotSearchWin(MMIFM_SEARCH_FREQUENCY_TYPE_E e_search_type,int32 direct);
#ifdef TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description : do action by touch pannel location
//  Global resource dependence : 
//  Parameter: point[IN]:touch pannel location
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPAction(DPARAM param, MMI_WIN_ID_T win_id);
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : do action by touch pannel long press 
//  Global resource dependence : 
//  Parameter: point[IN]:touch pannel location
//  Note: 
/*****************************************************************************/
LOCAL void HandleLongTPAction(DPARAM param, MMI_WIN_ID_T win_id);
#endif
#if 0
/*****************************************************************************/
//  Description : do action by button pressed
//  Global resource dependence : 
//  Parameter: point[IN]:touch pannel location
//  Note: 
/*****************************************************************************/
LOCAL void HandleButtonPress(MMI_WIN_ID_T win_id, uint32 button_id);
#endif
/*****************************************************************************/
//  Description : save currrent frequency 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleSaveCurrentFreq(uint16 ch_index);
#ifndef FM_NEW_UI
/*****************************************************************************/
//  Description : FM main window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmMainMsg(
                                   MMI_WIN_ID_T          win_id,    
                                   MMI_MESSAGE_ID_E      msg_id, 
                                   DPARAM                param
                                   );
#endif
#if 0
/*****************************************************************************/
//  Discription: Handle headset plug out or in  win msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleHeadsetPlugOutWinMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param 
                                               );
#endif
/*****************************************************************************/
//  Discription: Handle auto searching window msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAutoSearchWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param 
                                           );
/*****************************************************************************/
//  Discription: Handle get one valid frequency window msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSearchValidFreqWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param 
                                           );
/*****************************************************************************/
//  Description : FM option window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmMainOptionMsg(
                                         MMI_WIN_ID_T        win_id,    
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );

/*****************************************************************************/
//  Description : channel list window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChannelListWinMsg(
                                           MMI_WIN_ID_T        win_id,  
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM              param
                                           );
/*****************************************************************************/
//  Description :Alarm channel list window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmChannelListWinMsg(
                                           MMI_WIN_ID_T        win_id,  
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM              param
                                           );
/*****************************************************************************/
//  Description : FM option window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmChannelOptionMsg(
                                            MMI_WIN_ID_T     win_id,    
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );


/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditBoxWinMsg(
                                       MMI_WIN_ID_T        win_id, 
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Description : channel details处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChannelDetailWinMsg(
                                             MMI_WIN_ID_T        win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM              param
                                             );

/*****************************************************************************/
//  Description : FM option window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQuerywin(   MMI_WIN_ID_T     win_id,   
                                  MMI_MESSAGE_ID_E    msg_id, 
                                  DPARAM              param
                                  );

/*****************************************************************************/
//  Discription: Handle textbox win msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleTextBoxWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param 
                                         );
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Discription: Handle Fm Record win msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmRecordMsg(
                                              MMI_WIN_ID_T      win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM                param
                                              );
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Discription: Handle Fm Timer Record win msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmTimerRecordMsg(
                                              MMI_WIN_ID_T      win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM                param
                                              );
/*****************************************************************************/
// 	Description : Handle Fm Set Timer Info Msg
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFmSetTimerInfoMsg(
                                          MMI_WIN_ID_T      win_id, 	
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          );
#endif
#ifndef FM_NEW_UI
/*****************************************************************************/
//  Discription: draw fm radio's main pannel
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayMainWindow(MMI_WIN_ID_T win_id, uint32 items_need_draw);
#endif
#ifdef FM_NEW_UI
/*****************************************************************************/
//  Discription: draw channel's name
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DrawChannelName(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Discription: draw channel's name by freq
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DrawChannelNameByFreq(MMI_WIN_ID_T win_id,uint16 frequency);
#endif
/*****************************************************************************/
//  Discription: draw channel's NO.
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DrawChannelNumber(const wchar *num_str_ptr, MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Discription: draw freqency string text
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DrawFrqqencyText(MMI_WIN_ID_T win_id);
#if 0
/*****************************************************************************/
//  Description : draw fm volume images.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayVolumeImage(MMI_WIN_ID_T      win_id);
#endif
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  CreateButtons(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : read channels to list box
//  Global resource dependence : 
//  Author:Liqing Peng
//  Note: 
/*****************************************************************************/
LOCAL void ReadChannelsToListBox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMIFM_CHANNEL_LIST_TYPE_E ch_type);


/*****************************************************************************/
//  Description : handle fm actions
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleFMAction(MMIFM_OPERTION_TYPE_E act_type, const MMI_STRING_T *str_ptr);

/*****************************************************************************/
//  Description : create EDITBOX
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateEditBox(const MMI_STRING_T *text_ptr,
                         uint16       max_len,
                         uint8        allow_inputmethod,
                         MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : draw channel details win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DrawChannelDetails(MMI_WIN_ID_T win_id,const MMIFM_CHANNEL_T *channel_ptr);

/*****************************************************************************/
//  Description : EditWinDrawFocusRect 
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
LOCAL void DrawEditRect(
                        GUI_RECT_T rect,
                        GUI_COLOR_T color,
                        BOOLEAN     is_focus,
                        MMI_CTRL_ID_T ctrl_id
                        );

/*****************************************************************************/
//  Description : change string with point to integer
//  Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int16 ConvertFloatStrToInt(const MMI_STRING_T *str_ptr, int32 mul_scale);

/*****************************************************************************/
//  Discription: Clear the client rect  
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL  void  ClrClientRect(void);

/*****************************************************************************/
//  Discription: play user input channel
//  Global resource dependence: none 
//  Parameter: ch_num_str: channel's number string
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL  void  PlayInputChannel(const wchar *ch_num_str);
/*****************************************************************************/
//  Description : get current Rect.
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetCurrentRect(uint16 left, uint16 top,  uint16 right, uint16 bottom);
#ifndef FM_NEW_UI
/*****************************************************************************/
//  Description : Set FM Main Form Param
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFMMainFormParam(MMI_WIN_ID_T win_id);
#ifdef MMI_FM_MINI_SUPPORT
/*****************************************************************************/
//  Description : Display FM Backgroud
//  Global resource dependence : none
//  Author: langhua.yang
//  Note:just for MAINLCD_SIZE_128X160 and MAINLCD_SIZE_176X220
/*****************************************************************************/
LOCAL void DisplayFMBg(MMI_WIN_ID_T win_id);
#endif
#endif
/*****************************************************************************/
//  Description : OpenMusicVolumePanel
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void OpenFMVolumePanel(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type);
/*****************************************************************************/
//  Description : FM Volume Callback
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AdjustFMVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr);
/*****************************************************************************/
//  Description : Set FM StartSuspend Button Status
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFMStartSuspendButtonStatus(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : draw status Button
//  Global resource dependence : 
//  Parameter: 
//  Note: 
/*****************************************************************************/
LOCAL void DrawStatusButton(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Display FM Fre Pic
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DisplayFMFrePic(GUIANIM_OWNER_DRAW_T    *draw_ptr);
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetFMRecordFormParam(void);
/*****************************************************************************/
//  Description : SetPortraitFormParam
//  Global resource dependence : none
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
LOCAL void SetFMRecordPortraitFormParam(void);
/*****************************************************************************/
//  Description : display record time
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void FmRecordDisplayRecordTime(void);
/*****************************************************************************/
//  Description : display record
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FMRecordUpdateScreen(void);
/*****************************************************************************/
//  Description : calculate time of MMIFMRECORD_TIME_T
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void FMGetRecordTime(MMIFMRECORD_TIME_T* time_ptr);
/*****************************************************************************/
//  Description : Start Display Progress Timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FMStartDisplayProgressTimer(void);
/*****************************************************************************/
//  Description : Stop Display Progress Timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FMStopDisplayProgressTimer(void);
/*****************************************************************************/
//  Description : start FM record
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFMRECORD_RESULT_E FMRecordStartRecord(void);
/*****************************************************************************/
//  Description : Set Label Text
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void FMRecordSetLabelText(void);
/*****************************************************************************/
//  Description : handle app ok msg of record
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleFMRecordAppOkMsg(MMI_WIN_ID_T win_id);
/*****************************************************************************/
// Description : Fm Record Rename
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmRecordRenameWinMsg(MMI_WIN_ID_T  win_id, MMI_MESSAGE_ID_E  msg_id, DPARAM  param);

/*****************************************************************************/
//  Discription: Handle Cancel Rename Win Msg 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCancelRenameWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param); 
#endif
/*****************************************************************************/
//  Description : read channels to list box
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ReadChannelsToAlarmListBox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id);
/*****************************************************************************/
//  Description : Display fm volume images.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFMVolumePanelOpen(void);
/*****************************************************************************/
//  Description : udpate the button bg according the valid channel
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FMMainUpdateScreen(void);
/*****************************************************************************/
//  Description : Set FM StartSuspend Button BG when be resumeed
//  Global resource dependence : 
//  Parameter: Set Button Status
//  Note: 
/*****************************************************************************/
LOCAL void SetButtonBgWhenResume(void);
/*****************************************************************************/
//  Description : udpate the button bg according the FM start way
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FMMainUpdateButtonBg(void);
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
/*****************************************************************************/
//  Description : Set FM volume Button BG when volume change to 0
//  Global resource dependence : 
//  Parameter: Set Button Status
//  Note: 
/*****************************************************************************/
LOCAL void SetVolumeBgWhenZero(void);
#endif
/*****************************************************************************/
//  Description : Handle FM Alarm prview 
//  Global resource dependence : 
//  Parameter: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmChannelprview(MMI_WIN_ID_T win_id,MMI_MESSAGE_ID_E msg_id, DPARAM param);
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Description : Set FM Timer Record Param
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetFMTimerRecordParam(MMIACC_SMART_EVENT_T *event_ptr);
/*****************************************************************************/
// 	Description : set time ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetTimeParamEditWin(uint8 hour,
                                                             uint8 minute);
/*****************************************************************************/
// 	Description : set mode ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetModeParamEditWin(MMIACC_MODE_TYPE_E	fre_mode);
/*****************************************************************************/
// 	Description : set  weeks ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetWeeksParamEditWin(MMI_WIN_ID_T      win_id,  uint8 fre);
/*****************************************************************************/
// 	Description : set  weeks display
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetWeeksDisplayEditWin(void);
#endif
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);
/*****************************************************************************/
// 	Description : set frequency ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetfrequencyParamEditWin(const MMIFM_TIMER_RECORD_INFO_T *record_info_ptr);
/*****************************************************************************/
// 	Description : add item for setlist
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void  AddItemSetlist(void);
/*****************************************************************************/
// 	Description : set record endure ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetRecordEndureParamEditWin(const MMIFM_TIMER_RECORD_INFO_T *record_info_ptr);
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void setEditSelectWeeksSoftkey(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id,
                                          	BOOLEAN is_update);
/*****************************************************************************/
// 	Description : Set EditWin Button bg
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetEditWinButtonBg(MMI_HANDLE_T    win_id,uint32 ctrl_id);
/*****************************************************************************/
// 	Description : handle alarm edit windows penok messages
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void  HandleFmWeeksBtnEditMsg(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id
                                          );
/*****************************************************************************/
// 	Description : handle alarm edit windows FormSwitchActive messages
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void  HandleEditFormSwitchActiveMsg(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id
                                          );
/*****************************************************************************/
// 	Description : save time
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimeEditWin(void);
/*****************************************************************************/
// 	Description : save mode
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void  SaveModeEditWin(void);
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : save endure time
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimerRecordInfo(void);
#endif
#endif
/*****************************************************************************/
// 	Description : save fre
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveFreEditWin(void);
#if 0
/*****************************************************************************/
// 	Description : get frequency ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN savefrequency(void);
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : channel list window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChannelListPdaWinMsg(
                                           MMI_WIN_ID_T        win_id,  
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );
#endif

#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : FM main window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePDAFmMainMsg(
                                   MMI_WIN_ID_T          win_id,    
                                   MMI_MESSAGE_ID_E      msg_id, 
                                   DPARAM                param
                                   );

#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  drawFre(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *dev_info_ptr,uint16 fre,uint16 x_step);
#endif

/*****************************************************************************/
//  Description : create  latyer
//  Global resource dependence : none
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void FMCreateLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : release  latyer
//  Global resource dependence : none
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void FMReleaseLayer(GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : release  latyer
//  Global resource dependence : none
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void FMAppendLayer(GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : Set FM Main Form Param
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetPDAFMMainParam(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  drawBg(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Discription: draw fm radio's main pannel
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayMainFre(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO* lcd_dev_info,uint16 frequency);
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  SetVolumnAndListButtonsRect(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  SetButtonsRect(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : set return and menu  button rectangle
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  SetReturnMenuButtonRect(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description :  update return and menu  button 
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  UpdateReturnMenuButton(MMI_WIN_ID_T win_id, BOOLEAN is_refresh);

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleFMSlideTpMove(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_HANDLE_T                handle,
                                MMIFM_SLIDE_INFO_T    *slide_info_ptr    // [in]
                                );

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void ExitSlide( MMIFM_SLIDE_INFO_T    *slide_info_ptr);

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideTimer( MMIFM_SLIDE_INFO_T    *slide_info_ptr);

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideTimer( MMIFM_SLIDE_INFO_T    *slide_info_ptr);

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlideTimer(
                                       MMIFM_SLIDE_INFO_T    *slide_info_ptr, // [in]
                                       MMI_WIN_ID_T win_id,
                                       DPARAM           param           // [in]
                                       );

#if 0
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  drawSlideOnly(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *dev_info_ptr,uint16 x_step);

/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  drawFreOnly(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *dev_info_ptr,uint16 fre);
#endif

/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  IsInFreRect(MMI_WIN_ID_T win_id, GUI_POINT_T in_point);
/*****************************************************************************/
//  Description : get angel
//  Global resource dependence : 
//  Author: 
//  Note:point1:原点, point2:手指所在位置
/*****************************************************************************/
LOCAL MMIFM_POINT_DATA_T GetPointValueByPoint(GUI_POINT_T point1, GUI_POINT_T point2);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T GetFreqNeedleCenterPoint(void);
/******************************************************************************/
//  Description: decompressed the ARGB image
//  Author: 
//  Input:   
//          src_img_ptr:	pointer of input image information
//			dst_buf_ptr:	pointer to the target buffer
//			dst_buf_size:	target buffer size
//  Output: 
//			dst_img_ptr:	pointer to the decompressed image information
//  Return: 
//			TRUE:			successful
//          FALSE:			failed
//  Note:   
/******************************************************************************/
LOCAL BOOLEAN DecompressARGB(const MMIFM_IMAGE_T *src_img_ptr, MMIFM_IMAGE_T *dst_img_ptr, 
							  uint8 *dst_buf_ptr, uint32 dst_buf_size);
/*****************************************************************************/
//  Description : Display Freq Pointer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayFreqPointerByAngle(int new_freq_angle);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateFreqNeedleLayer(void);
/*****************************************************************************/
//  Description : get angel By Freq
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int GetAngleByFreq(uint16 fre);
/*****************************************************************************/
//  Description : draw fm ui according the point
//  Global resource dependence : 
//  Parameter: point[IN]:touch pannel location
//  Note: 
/*****************************************************************************/
LOCAL void DrawFmUIAccordingPoint(MMI_WIN_ID_T win_id,GUI_POINT_T point);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : FM option window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmChannelMenuOptionMsg(
   MMI_WIN_ID_T     win_id,     
   MMI_MESSAGE_ID_E    msg_id, 
   DPARAM              param
   );
/*****************************************************************************/
//  Description : create option menu items 
//  Global resource dependence : 
//  Parameter: 
//  Note: 

/*****************************************************************************/
LOCAL void CreateChannelMenuOptionMenu(uint32 ctr_id);
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Description : HandleEditTimeMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTimeMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );
/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditTimeEdureMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );
/*****************************************************************************/
//  Discription: HandleEditFmFreMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditFmFreMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );

/*****************************************************************************/
// 	Description : save endure time
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimerRecordFre(void);
/*****************************************************************************/
// 	Description : save endure time
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimerRecordEndureTime(void);
#endif

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : Get game box Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIFM_APPLET_T *MMIFM_GetAppletInstance(void);

/*****************************************************************************/
//  Description : Create box setting win
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T OpenFmWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  );

#endif
#ifndef FM_NEW_UI
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  SetButtonsAndLabelRect(MMI_WIN_ID_T win_id);
#endif
/**--------------------------------------------------------------------------*
**                         WINTAB DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef FM_NEW_UI 
WINDOW_TABLE( MMI_FM_MAIN_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePDAFmMainMsg ),    
    WIN_ID( FM_MAIN_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    CREATE_BUTTON_CTRL(IMAGE_FM_RETURN_BUTTON_DEFAULT,MMIFM_RETURN_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_FM_MENU_BUTTON_DEFAULT,MMIFM_MENU_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_FM_VOLUME_DEFAUT,MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_FM_LEFT_DEFAUT,MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_FM_AJUST_PLAY,MMIFM_START_SUSPEND_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_FM_RIGHT_DEFAULT,MMIFM_GET_UPFRE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_FM_LIST_DEFAULT,MMIFM_LIST_BUTTON_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FM_CHANNEL_NAME_TEXT_CTRL_ID),
    END_WIN
};
#else
// FM main window
WINDOW_TABLE( MMI_FM_MAIN_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_FUNC( (uint32)HandleFmMainMsg ),    
    WIN_ID( FM_MAIN_WIN_ID ),
//#ifdef MMI_PDA_SUPPORT
#ifndef MMI_FM_MINI_SUPPORT
    WIN_STATUSBAR,
#endif
//#endif
#ifdef MMI_FM_MINI_SUPPORT
    WIN_TITLE(TXT_FM_TITLE),
#endif
    CREATE_ANIM_CTRL(MMIFM_FREQUENCY_BAR_CTRL_ID,FM_MAIN_WIN_ID),
    #ifdef FM_RDS_SUPPORT    
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,MMIFM_RDS_TEXT_CTRL_ID),
    #endif
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
    CREATE_BUTTON_CTRL(IMAGE_FM_ADJ_LEFT_DEFAULT,MMIFM_ADJ_FREQ_LEFT_BUTTON_ID),
#endif
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIFM_FREQUENCY_TEXT_CTRL_ID),
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
    CREATE_BUTTON_CTRL(IMAGE_FM_ADJ_RIGHT_DEFAULT,MMIFM_ADJ_FREQ_RIGHT_BUTTON_ID),
#endif
    CREATE_BUTTON_CTRL(IMAGE_FM_LEFT_DEFAULT,MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_FM_PAUSE_DEFAULT,MMIFM_START_SUSPEND_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_FM_RIGHT_DEFAULT,MMIFM_GET_UPFRE_BUTTON_CTRL_ID),
    
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_EXIT),    
    END_WIN
};
#endif

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(MMI_FM_CHANNEL_LIST_PDA_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleChannelListPdaWinMsg),   
    WIN_ID( MMI_FM_CHANNEL_LIST_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_NULL),
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    //WIN_SOFTKEY(TXT_NULL, TXT_DELALL, TXT_NULL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMI_FM_CHANNEL_LIST_CTRL_ID),
    END_WIN
};

//fm channel list option menu
WINDOW_TABLE(FM_CHANNEL_MENU_OPTION_WIN_TAB) = 
{   
    WIN_FUNC( (uint32)HandleFmChannelMenuOptionMsg),
    WIN_ID(FM_CHANNEL_MENU_OPTION_WIN_ID),
    END_WIN
};
#endif
// FM channel list
WINDOW_TABLE(MMI_FM_CHANNEL_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleChannelListWinMsg),   
    WIN_ID( MMI_FM_CHANNEL_LIST_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMI_FM_CHANNEL_LIST_CTRL_ID),
    END_WIN
};

// Alarm FM channel list
WINDOW_TABLE(MMI_ALARM_FM_CHANNEL_LIST_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleAlarmChannelListWinMsg),   
    WIN_ID( MMI_ALARM_FM_CHANNEL_LIST_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID),
    END_WIN
};

// 编辑窗口
WINDOW_TABLE(MMIFM_EDITBOX_WIN_TAB) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)HandleEditBoxWinMsg),    
        WIN_ID( MMIFM_EDITBOX_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
        WIN_TITLE(TXT_NULL),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
       // CREATE_SOFTKEY_CTRL(STXT_OK, TXT_NULL, STXT_RETURN, GUISOFTKEY_STYLE_NULL, MMICOMMON_SOFTKEY_CTRL_ID),
        END_WIN
};

// channel details win
WINDOW_TABLE(MMIFM_CHANNEL_DETAIL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleChannelDetailWinMsg),    
    WIN_ID( MMIFM_CHANNEL_DETAIL_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIFM_DETAILS_FROM_ID),

    //name
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIFM_DETAILS_NAME_FORM_CTRL_ID,MMIFM_DETAILS_FROM_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIFM_DETAILS_NAME_LABEL_CTRL_ID,MMIFM_DETAILS_NAME_FORM_CTRL_ID),        
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIFM_CHANNEL_NAME_LEN,MMIFM_DETAILS_NAME_EDIT_LABEL_CTRL_ID,MMIFM_DETAILS_NAME_FORM_CTRL_ID),
        
    //frequency
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIFM_DETAILS_FREQ_FORM_CTRL_ID,MMIFM_DETAILS_FROM_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIFM_DETAILS_FREQ_LABEL_CTRL_ID,MMIFM_DETAILS_FREQ_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIFM_DIGIT_MAX_LEN,MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID,MMIFM_DETAILS_FREQ_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    //WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_CANCEL),
    CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_CANCEL,MMICOMMON_SOFTKEY_CTRL_ID,MMIFM_DETAILS_FROM_ID),
#else
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN), 
#endif
    END_WIN
};

//help window
WINDOW_TABLE(MMIFM_TEXT_BOX_WIN_TAB) = 
{ 
    WIN_FUNC((uint32) HandleTextBoxWinMsg ),    
    WIN_ID(MMIFM_TEXT_BOX_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_HELP),
    CREATE_TEXT_CTRL(MMIFM_TEXTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN    
};

//fm option menu
WINDOW_TABLE(FM_OPTION_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleFmMainOptionMsg),
    WIN_ID(FM_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//fm channel list option menu
WINDOW_TABLE(FM_CHANNEL_OPTION_WIN_TAB) = 
{   
    WIN_FUNC( (uint32)HandleFmChannelOptionMsg),
    WIN_ID(FM_CHANNEL_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#ifdef MMIFM_SUPPORT_FM_RECORD
// FM main window
WINDOW_TABLE( MMI_FM_RECORD_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleFmRecordMsg ),    
    WIN_ID( FM_RECORD_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif

    WIN_TITLE(TXT_RECORDER),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIFM_MAINPLAY_FORM_CTRL_ID), 
    
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIFM_TIME_OWNDRAW_CTRL_ID,MMIFM_MAINPLAY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIFM_NAME_LABEL_CTRL_ID,MMIFM_MAINPLAY_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIFM_FORM_ANIM_CTRL_ID,MMIFM_MAINPLAY_FORM_CTRL_ID),

    WIN_SOFTKEY(TXT_PAUSE, TXT_NULL, STXT_STOP),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};

// FM main window no status bar
WINDOW_TABLE( MMI_FM_RECORD_NO_STATUSBAR_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleFmRecordMsg ),    
    WIN_ID( FM_RECORD_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_RECORDER),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE),
#endif
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIFM_MAINPLAY_FORM_CTRL_ID), 
    
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIFM_TIME_OWNDRAW_CTRL_ID,MMIFM_MAINPLAY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIFM_NAME_LABEL_CTRL_ID,MMIFM_MAINPLAY_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE,FALSE,MMIFM_FORM_ANIM_CTRL_ID,MMIFM_MAINPLAY_FORM_CTRL_ID),

    WIN_SOFTKEY(TXT_PAUSE, TXT_NULL, STXT_STOP),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};

//rename win 
WINDOW_TABLE(MMIFM_RECORD_RENAME_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleFmRecordRenameWinMsg ),    
    WIN_ID(MMIFM_RECORD_RENAME_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_RECORDER),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIFM_RENAME_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIFMRECORD_MAX_FILE_NAME_LEN,MMIFM_RECORD_RENAME_EDITBOX_CTRL_ID,MMIFM_RENAME_FORM_CTRL_ID),
        CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMIFM_RENAME_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIFMRECORD_MAX_FILE_NAME_LEN,MMIFM_RECORD_RENAME_EDITBOX_CTRL_ID),
#endif

    END_WIN
};

//rename win no status bar
WINDOW_TABLE(MMIFM_RECORD_RENAME_NO_STATUSBAR_STATUSWIN_TAB) = 
{
    WIN_FUNC((uint32)HandleFmRecordRenameWinMsg ),    
    WIN_ID(MMIFM_RECORD_RENAME_WIN_ID),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_RECORDER),
#ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIFM_RENAME_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIFMRECORD_MAX_FILE_NAME_LEN,MMIFM_RECORD_RENAME_EDITBOX_CTRL_ID,MMIFM_RENAME_FORM_CTRL_ID),
        CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMIFM_RENAME_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIFMRECORD_MAX_FILE_NAME_LEN,MMIFM_RECORD_RENAME_EDITBOX_CTRL_ID),
#endif

    END_WIN
};
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
// FM Timer window
WINDOW_TABLE( MMI_FM_TIMER_RECORD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFmTimerRecordMsg ),    
    WIN_ID( FM_TIMER_RECORD_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_FM_RECORD_TIMER),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIFM_TIMER_RECORD_LIST_CTRL_ID),
    END_WIN
};
// FM Timer window
WINDOW_TABLE( MMI_FM_SET_TIMER_INFO_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFmSetTimerInfoMsg ),    
    WIN_ID( FM_SET_TIMER_INFO_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_TITLE(TXT_FM_TIME_FREQUENCY),

    WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
    //WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_CANCEL),
#else
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIFM_EDITWIN_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_CANCEL,MMICOMMON_SOFTKEY_CTRL_ID,MMIFM_EDITWIN_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIFM_EDIT_TIME_FORM_CTRL_ID,MMIFM_EDITWIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIFM_EDIT_TIME_LABEL_CTRL_ID,MMIFM_EDIT_TIME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIFM_EDIT_TIME_EDIT_LABEL_CTRL_ID,MMIFM_EDIT_TIME_FORM_CTRL_ID),
#else
        CHILD_EDIT_TIME_CTRL(TRUE,MMIFM_EDIT_TIME_EDITBOX_CTRL_ID,MMIFM_EDIT_TIME_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIFM_EDIT_TIME_ENDURE_FORM_CTRL_ID,MMIFM_EDITWIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIFM_EDIT_TIME_ENDURE_LABEL_CTRL_ID,MMIFM_EDIT_TIME_ENDURE_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIFM_EDIT_TIME_ENDURE_EDIT_LABEL_CTRL_ID,MMIFM_EDIT_TIME_ENDURE_FORM_CTRL_ID),
#else
        CHILD_EDIT_DIGITAL_CTRL(TRUE,MMIFM_ENDURE_TIME_MAX_LEN,MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID,MMIFM_EDIT_TIME_ENDURE_FORM_CTRL_ID),
#endif

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIFM_EDIT_FREQUENCY_FORM_CTRL_ID,MMIFM_EDITWIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIFM_EDIT_FREQUENCY_LABEL_CTRL_ID,MMIFM_EDIT_FREQUENCY_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIFM_EDIT_FREQUENCY_EDIT_LABEL_CTRL_ID,MMIFM_EDIT_FREQUENCY_FORM_CTRL_ID),
#else
        CHILD_EDIT_TEXT_CTRL(TRUE,MMIFM_DIGIT_MAX_LEN,MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID,MMIFM_EDIT_FREQUENCY_FORM_CTRL_ID),
#endif
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIFM_EDIT_MODE_FORM_CTRL_ID,MMIFM_EDITWIN_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIFM_EDIT_MODE_LABEL_CTRL_ID,MMIFM_EDIT_MODE_FORM_CTRL_ID),
	    CHILD_SETLIST_CTRL(TRUE,MMIFM_EDIT_MODE_SETLIST_CTRL_ID,MMIFM_EDIT_MODE_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIFM_EDITWIN_CHILD_FORM_CTRL_ID,MMIFM_EDITWIN_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMIFM_EDITWIN_BUTTON0_CTRL_ID,MMIFM_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMIFM_EDITWIN_BUTTON1_CTRL_ID,MMIFM_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMIFM_EDITWIN_BUTTON2_CTRL_ID,MMIFM_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMIFM_EDITWIN_BUTTON3_CTRL_ID,MMIFM_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMIFM_EDITWIN_BUTTON4_CTRL_ID,MMIFM_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMIFM_EDITWIN_BUTTON5_CTRL_ID,MMIFM_EDITWIN_CHILD_FORM_CTRL_ID),
    	CHILD_BUTTON_CTRL(TRUE,IMAGE_CLOCK_ALARM_WEEK_DISABLED,MMIFM_EDITWIN_BUTTON6_CTRL_ID,MMIFM_EDITWIN_CHILD_FORM_CTRL_ID),
    
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
WINDOW_TABLE( MMIFM_TIMEWIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditTimeMsg ),    
    WIN_ID( MMIFM_TIMEWIN_ID ),
    WIN_TITLE( TXT_START_TIME ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIFM_TIME_EDIT_FORM_CTRL_ID),
        CHILD_EDIT_TOUCH_TIME_CTRL(TRUE,MMIFM_EDIT_TIME_EDITBOX_CTRL_ID,MMIFM_TIME_EDIT_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
WINDOW_TABLE(MMIFM_TIMEEDUREWIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditTimeEdureMsg),    
    WIN_ID(MMIFM_TIMEEDUREWIN_ID),
    WIN_TITLE(TXT_FM_RECORD_TIME_LENGTH),
    //CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIFM_EDIT_TIME_ENDURE_EDIT_FORM_CTRL_ID),
    CREATE_EDIT_DIGITAL_CTRL(MMIFM_ENDURE_TIME_MAX_LEN,MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
    END_WIN
};
WINDOW_TABLE(MMIFM_EDITFMFRE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditFmFreMsg),    
    WIN_ID(MMIFM_EDITFMFRE_WIN_ID),
    WIN_TITLE(TXT_FM_RECORD_FREQUENCY),
    CREATE_EDIT_TEXT_CTRL(MMIFM_DIGIT_MAX_LEN,MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
**                         EXTERNAL FUNCTION DEFINITION                     *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open play FM main window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_OpenMainWin(void)
{
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        return; 
    }        
    if(MMITHEME_IsTVOpen())
    {
        //TV out, FM can't be used
        MMIPUB_OpenAlertWarningWin(TXT_FM_CANNOT_USE);
        return;
    }
    if(MMIAPISET_GetFlyMode())
    {
        MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST);
        return;
    }
    
    if(!MMIFM_CheckHeadsetStatus())
    {  
        return;
    }
        
    if(FM_STOP == MMIFM_GetStatus())
    {
      if(!MMIFM_Play(TRUE))
      {                
        MMIFM_Exit(FALSE);
        MMIPUB_OpenAlertFailWin(TXT_FM_INIT_FAIL);               
        return;
      }
    }
    /* if FM was suspended by other App, try to resume it; But if it was suspened by itself, do not resume */
    else if(FM_SUSPENDED == MMIFM_GetStatus())
    {
      MMISRVMGR_Resume(MMIFM_GetPlayHandle());
      if(!MMIFM_GetSuspendByFM())
      {
        MMIFM_Play(FALSE);
      }  
    }  
          
     // FM win
#ifndef MMI_TASK_MANAGER
    MMK_CreateFmWin((uint32 *)MMI_FM_MAIN_WIN_TAB, PNULL);
#else
    {
        MMI_APPLET_START_T start ={0};
        
        //SCI_TRACE_LOW:"MMIFM_OpenMainWin"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_1655_112_2_18_2_19_30_62,(uint8*)"");
        start.guid = SPRD_FM_APPLET_ID;
        start.state = MMI_APPLET_STATE_BG_HIDE_WIN|MMI_APPLET_STATE_TASK_MANAGER;
        MMK_StartApplet( &start );
    }
#endif
} 

#ifdef FM_RDS_SUPPORT
PUBLIC void SetRDSText(wchar  * param){

    wchar *rds_text_ptr = PNULL;
    int32     str_len = 0;
    MMI_STRING_T      * string_ptr  = PNULL;
    MMI_STRING_T       string  = {0};

    if(NULL != param){
        string_ptr= (MMI_STRING_T *)param;       
        str_len = MMIAPICOM_Wstrlen(string_ptr);
        string.wstr_len = str_len;
        string.wstr_ptr = string_ptr; 
    }
    GUILABEL_SetFont(MMIFM_RDS_TEXT_CTRL_ID, SONG_FONT_15, MMIFM_FONT_CLOLOR);
    GUILABEL_SetText(MMIFM_RDS_TEXT_CTRL_ID,&string,TRUE);

    
}
#endif

#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : FM main window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePDAFmMainMsg(
                                   MMI_WIN_ID_T          win_id,    
                                   MMI_MESSAGE_ID_E      msg_id, 
                                   DPARAM                param
                                   )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE; 
    MMIFM_POINT_DATA_T point_value = {0};
    MMIFM_START_WAY_E  start_way = (uint32)MMK_GetWinAddDataPtr(win_id);    
    MMIFM_CUR_INFO_T *cur_info_ptr = MMIFM_GetFMCurrentInfo();
        
    //SCI_TRACE_LOW:"[MMIFM] [HandleFmMainMsg] msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_1678_112_2_18_2_19_30_63,(uint8*)"d", msg_id);
    if(PREVIEW_START_FM == start_way)
    {
        if(MMI_RESULT_FALSE == HandleAlarmChannelprview(win_id,msg_id,param))
            return MMI_RESULT_FALSE;
    }
    if(PNULL == cur_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            SetPDAFMMainParam(win_id);
            FMCreateLayer(&s_lcd_dev_info,win_id);
            FMAppendLayer(&s_lcd_dev_info);
            CreateFreqNeedleLayer();
            FMAppendLayer(&s_needle_dev_info);
#ifdef MMI_TASK_MANAGER
#ifdef PDA_UI_DROPDOWN_WIN
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_FM);//@fen.xie
#endif
#endif
//NEWMS00180335  2012/3/23
            if(!MMIFM_CheckHeadsetStatusWithoutPrompt())
            {
                MMIFM_Stop();
            }
            if(0 == MMIFM_GetValidChannelCount()
                && MMIFM_CheckHeadsetStatusWithoutPrompt()
                )
            {
                OpenAuotSearchWin(MMIFM_AUTOSEARCH,FM_SCAN_UP);
            }
            MMK_SetAtvCtrl(win_id,FM_CHANNEL_NAME_TEXT_CTRL_ID);
        } 
        break;
        case MSG_FULL_PAINT:
        {
            drawBg(win_id,&s_main_lcd_dev_info);
			DrawStatusButton(win_id);
            DrawChannelName(win_id);
            UpdateReturnMenuButton(win_id, FALSE);
            point_value.angle = GetAngleByFreq(cur_info_ptr->cur_freq);
            DisplayFreqPointerByAngle(point_value.angle);
            if (UILAYER_IsLayerActive(&s_lcd_dev_info))
            {
                DisplayMainFre(win_id,&s_lcd_dev_info,cur_info_ptr->cur_freq);
            }
            else
            {
                DisplayMainFre(win_id,&s_main_lcd_dev_info,cur_info_ptr->cur_freq);
            }
        }
        break;
        case MSG_GET_FOCUS:
        {  
            FMAppendLayer(&s_lcd_dev_info);
            FMAppendLayer(&s_needle_dev_info);
        }
        break;
       case MSG_LOSE_FOCUS:
        {
            //DisplayMainFre(win_id,&s_main_lcd_dev_info,cur_info_ptr->cur_freq);
        }
        break;
       case MSG_LCD_SWITCH:
        SetPDAFMMainParam(win_id);
        FMReleaseLayer(&s_lcd_dev_info);
        FMReleaseLayer(&s_needle_dev_info);
        FMCreateLayer(&s_lcd_dev_info,win_id);
        FMAppendLayer(&s_lcd_dev_info);
        CreateFreqNeedleLayer();
        FMAppendLayer(&s_needle_dev_info);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T point;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            DrawFmUIAccordingPoint(win_id,point);
        }
      break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point;
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            HandleFMSlideTpMove(&point,win_id,&s_slide_info);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_TP_PRESS_UP:
        {
            ExitSlide(&s_slide_info);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
       case MSG_TIMER:
        HandleSlideTimer(&s_slide_info,win_id,param);
        break;
        case MSG_APP_FM_ADJ_VOLUME:
        {
            //adjust volume, param is steps value
            MMIFM_AdjustVolume((int)param);
        }
        break;
        case MSG_APP_FM_ADJ_FREQ:
        {
            //adjust frequency, param is steps value
            MMIFM_AdjustFrequency((int)param);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;  
        case MSG_APP_FM_ADJ_CHANNEL:
        {
            //adjust channel, param is steps value
            if(MMIFM_GetValidChannelCount() > 1)
            {
                MMIFM_AdjustChannel((int)param);
                SetButtonBgWhenResume();
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break; 
        case MSG_APP_FM_SEARCH_VALID_FREQ:
        {
            //search valid freqency
            OpenAuotSearchWin(MMIFM_GET_ONE_VALID,(int)param);
            SetButtonBgWhenResume();
            //Draw freqency name name
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break; 
        case MSG_APP_FM_SWITCH_AUDIO_DEVICE:
        {
            //Switch Audio Device from headset to handfree or otherwise
            MMIFM_SwitchAudioDevice();
            //DisplayMainWindow(win_id, MMIFM_DRAW_SPK);
        }
        break;        
        
        case MSG_APP_WEB:
        {
            SetFMStartSuspendButtonStatus(win_id);
        }
        break;
        case MSG_APP_UPSIDE:
        case MSG_APP_UP:
        {
            OpenFMVolumePanel(MMICOM_PANEL_OPEN_INC);
        }
        break;

        case MSG_APP_RIGHT:
        {
            OpenAuotSearchWin(MMIFM_GET_ONE_VALID,MMIFM_GET_UP_VALUE_STEP);
            SetButtonBgWhenResume();
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        case MSG_APP_LEFT:
        {
            OpenAuotSearchWin(MMIFM_GET_ONE_VALID,MMIFM_GET_DOWN_VALUE_STEP);
            SetButtonBgWhenResume();
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);            
        }
        break;
        case MSG_APP_DOWNSIDE:
        case MSG_APP_DOWN: 
        {
            OpenFMVolumePanel(MMICOM_PANEL_OPEN_DEC);
        }
        break;          
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_MENU:
        {
            if (MMIAPICOM_IsPanelOpened())
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            //option
            MMK_CreateWin((uint32 *)FM_OPTION_WIN_TAB,PNULL);
            //creat dynamic menu
            GUIMENU_CreatDynamic(PNULL,FM_OPTION_WIN_ID,MENU_POPUP_CTRL_ID,GUIMENU_STYLE_POPUP);
        }
        break;
        
        case MSG_APP_FM_AUTOSEARCH_START:
		{
			OpenAuotSearchWin(MMIFM_AUTOSEARCH,FM_SCAN_UP);
		} 
		break;
        case MSG_APP_FM_AUTOSEARCH_END:
        //play this channel
        if(MMIFM_IsChannelExist(0))
        {
            MMIFM_PlayChannel(0);
        }
        else
        {
            //channel empty, play default frequency 87.5
            MMIFM_PlayFreq(MMIFM_FREQ_RANGE_LOW);
        }        
        SetButtonBgWhenResume();
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
        {
            if (MMIAPICOM_IsPanelOpened())
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            HandleTPAction(param, win_id);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
     
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENLONGOK:
        {  
            if (MMIAPICOM_IsPanelOpened())
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            HandleLongTPAction(param, win_id);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            if (MMIAPICOM_IsPanelOpened())
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            else
            {
                if(PREVIEW_START_FM != start_way)
                {
                    s_is_need_set_bgplay = FALSE;
                }
                MMK_CloseWin(win_id);
            }
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
            //exit fm           
            if(PREVIEW_START_FM == start_way)
            {
                MMISRVAUD_Stop(MMIAPIALARM_GetPlayHandle());
		        MMIAPIFM_Stop();
                MMIAPIALARM_FreePlaySrv();
            }
            else
            {
#ifndef MMI_TASK_MANAGER
              MMIFM_Exit(s_is_need_set_bgplay);
#else
              MMIFM_Exit(FALSE);//@fen.xie: for applet it must exit
#endif
            }
            FMReleaseLayer(&s_lcd_dev_info);
            FMReleaseLayer(&s_needle_dev_info);
        }
        break;
        default:
            result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}
#else
/*****************************************************************************/
//  Description : FM main window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmMainMsg(
                                   MMI_WIN_ID_T          win_id,    
                                   MMI_MESSAGE_ID_E      msg_id, 
                                   DPARAM                param
                                   )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE; 
    static wchar    s_input_str[MMIFM_CHANNEL_NUMBER_MAX_BIT + 1] = {0};
    static uint8    s_input_timer_id = 0;
    static uint8    s_input_times = 0;
    MMIFM_START_WAY_E  start_way = (uint32)MMK_GetWinAddDataPtr(win_id);
#ifdef UI_MULTILAYER_SUPPORT
    GUI_LCD_DEV_INFO    layer = {0};
#endif
        
    //SCI_TRACE_LOW:"[MMIFM] [HandleFmMainMsg] msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_1974_112_2_18_2_19_31_64,(uint8*)"d", msg_id);
    if(PREVIEW_START_FM == start_way)
    {   
#ifdef MMI_FM_MINI_SUPPORT
		MMK_SetAtvCtrl(win_id, MMIFM_FREQUENCY_TEXT_CTRL_ID);   
#endif
        if(MMI_RESULT_FALSE == HandleAlarmChannelprview(win_id,msg_id, param))
            return MMI_RESULT_FALSE;
    }
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            SetFMMainFormParam(win_id);
#ifdef PDA_UI_DROPDOWN_WIN
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_FM);//@fen.xie
#endif
#ifdef FM_RDS_SUPPORT
        if(MMIFM_GetRdsStatus() == FM_RDS_STATUS_OPEN){
            MMIFM_StartRDSTimer();
        }else if(MMIFM_GetRdsStatus() == FM_RDS_STATUS_CLOSE){
            MMIFM_StopRDSTimer();
        }
#endif

        /* modify by langhua.yang to fix the CR: NEWMS00204950 */ 
#ifdef MMI_FM_MINI_SUPPORT
            MMK_SetAtvCtrl(win_id, MMIFM_FREQUENCY_TEXT_CTRL_ID);   
#endif
             if(0 == MMIFM_GetValidChannelCount()
                && MMIFM_CheckHeadsetStatusWithoutPrompt()
                )
            {
//#ifndef MMI_FM_MINI_SUPPORT
            OpenAuotSearchWin(MMIFM_AUTOSEARCH,FM_SCAN_UP);
//#else
                  /*异步打开AUTOSEARCH等待PUBWIN，避免FM窗口未绘制结束*/
           // MMK_PostMsg(FM_MAIN_WIN_ID, MSG_APP_FM_AUTOSEARCH_START, PNULL, 0);
//#endif

            }
        } 
        break;

        case MSG_APP_FM_AUTOSEARCH_START:
		{
			OpenAuotSearchWin(MMIFM_AUTOSEARCH,FM_SCAN_UP);
		} 
		break;

        case MSG_FULL_PAINT:
        {
#ifdef UI_MULTILAYER_SUPPORT
            UILAYER_Clear(&layer);
#endif
            DisplayMainWindow(win_id, MMIFM_DRAW_FULL);
        }
        break;
        case MSG_LOSE_FOCUS:
        {
            //clean input channel number string
            if(s_input_timer_id > 0)
            {
                MMK_StopTimer(s_input_timer_id);
                s_input_timer_id = 0;
            }
            s_input_times = 0;
            s_input_str[s_input_times] = 0;
        }
        break;
        case MSG_GET_FOCUS:
        case MSG_LCD_SWITCH:
            SetFMMainFormParam(win_id);
            break;
        case MSG_APP_FM_ADJ_VOLUME:
        {
            //adjust volume, param is steps value
            MMIFM_AdjustVolume((int)param);
        }
        break;
        case MSG_APP_FM_ADJ_FREQ:
        {
            //adjust frequency, param is steps value
            MMIFM_AdjustFrequency((int)param);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;  
        case MSG_APP_FM_ADJ_CHANNEL:
        {
            //adjust channel, param is steps value
            if(MMIFM_GetValidChannelCount() > 1)
            {
                MMIFM_AdjustChannel((int)param);
                SetButtonBgWhenResume();
            }    
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL); 
        }
        break; 
        case MSG_APP_FM_SEARCH_VALID_FREQ:
        {
            //search valid freqency
            OpenAuotSearchWin(MMIFM_GET_ONE_VALID,(int)param);
            SetButtonBgWhenResume();
            //Draw freqency name name
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break; 
        case MSG_APP_FM_SWITCH_AUDIO_DEVICE:
        {
            //Switch Audio Device from headset to handfree or otherwise
            MMIFM_SwitchAudioDevice();
            DisplayMainWindow(win_id, MMIFM_DRAW_SPK);
        }
        break;              
        case MSG_NOTIFY_MIDSK:
        case MSG_APP_WEB:
        {
            SetFMStartSuspendButtonStatus(win_id);
        }
        break;
        case MSG_APP_UPSIDE:
        case MSG_APP_UP:
        {
            OpenFMVolumePanel(MMICOM_PANEL_OPEN_INC);
        }
        break;
        case MSG_APP_DOWNSIDE:
        case MSG_APP_DOWN: 
        {
            OpenFMVolumePanel(MMICOM_PANEL_OPEN_DEC);
        }
        break;
        case MSG_KEYLONG_LEFT: 
        {
            //search next valid freqency
            MMK_SendMsg(win_id, MSG_APP_FM_SEARCH_VALID_FREQ, (DPARAM)MMIFM_GET_DOWN_VALUE_STEP);
        }
        break; 
        case MSG_KEYLONG_RIGHT:
        {
            //search previous valid freqency
            MMK_SendMsg(win_id, MSG_APP_FM_SEARCH_VALID_FREQ, (DPARAM)MMIFM_GET_UP_VALUE_STEP);
        }
        break;
        case MSG_APP_LEFT:
        case MSG_APP_RIGHT:
         {   
            /*避免消息被LABEL 控件处理，导致刷新两次造成的闪烁*/
            break;
         }
        case MSG_KEYUP_LEFT:
        {
            //skip to next channel
            MMK_SendMsg(win_id, MSG_APP_FM_ADJ_CHANNEL, (DPARAM)MMIFM_GET_DOWN_VALUE_STEP);
        }
        break;   
        case MSG_KEYUP_RIGHT: 
        {
            //skip to previous channel
            MMK_SendMsg(win_id, MSG_APP_FM_ADJ_CHANNEL, (DPARAM)MMIFM_GET_UP_VALUE_STEP);
        }
        break;           
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_MENU:
        {
            if (MMIAPICOM_IsPanelOpened())
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            //option
            MMK_CreateWin((uint32 *)FM_OPTION_WIN_TAB,PNULL);
            //creat dynamic menu
            GUIMENU_CreatDynamic(PNULL,FM_OPTION_WIN_ID,MENU_POPUP_CTRL_ID,GUIMENU_STYLE_POPUP);
        }
        break;
        
        case MSG_APP_FM_AUTOSEARCH_END:
       if(!MMIAPICC_IsCcOnTopWin())
        {
          //play this channel
          if(MMIFM_IsChannelExist(0))
          {
            if(MMIFM_PlayChannel(0))
            {
                MMIFM_SetStatus(FM_PLAYING);
            }
          }
          else
          {
            //channel empty, play default frequency 87.5
            if(MMIFM_PlayFreq(MMIFM_FREQ_RANGE_LOW))
            {
               MMIFM_SetStatus(FM_PLAYING);
            }
          }        
          SetButtonBgWhenResume();
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
        {
            if (MMIAPICOM_IsPanelOpened())
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            HandleTPAction(param, win_id);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENLONGOK:
        {  
            if (MMIAPICOM_IsPanelOpened())
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            HandleLongTPAction(param, win_id);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_0:
        case MSG_APP_1:
        case MSG_APP_2:
        case MSG_APP_3:
        case MSG_APP_4:
        case MSG_APP_5:
        case MSG_APP_6:
        case MSG_APP_7:
        case MSG_APP_8:
        case MSG_APP_9:
        {
            //input channel number
            s_input_times ++;
            if(s_input_times > MMIFM_CHANNEL_NUMBER_MAX_BIT)
            {
                MMK_StopTimer(s_input_timer_id);
                s_input_timer_id = 0;
                s_input_times = 0;
                break;
            }
            s_input_str[s_input_times -1] = ((msg_id == MSG_APP_0) ? '0' : (msg_id - MSG_APP_1) +'1');
            s_input_str[s_input_times] = 0;
            DrawChannelNumber(s_input_str, win_id);
            if(s_input_times > 1)
            {
                //play this channel right now
                MMK_StopTimer(s_input_timer_id);
                PlayInputChannel(s_input_str);
                s_input_timer_id = 0;
                s_input_times = 0;
            }
            else
            {
                //wait 3s
                s_input_timer_id = MMK_CreateTimer(MMIFM_INPUTR_PERIOD, TRUE);
            }
        }
        break;
        case MSG_TIMER:
        {
            if(s_input_timer_id == *((uint8*)param)){
                MMK_StopTimer(s_input_timer_id);
                s_input_timer_id = 0;
                //play this channel right now
                s_input_times = 0;
                PlayInputChannel(s_input_str);
            }
#ifdef FM_RDS_SUPPORT
            // process RDS timer ,read and show RDS message
            else {
                MMIFM_ReadRDS();
            }
#endif

        }
        break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            if (MMIAPICOM_IsPanelOpened())
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            else
            {
                if(PREVIEW_START_FM != start_way)
                {
                    s_is_need_set_bgplay = FALSE;
                }
                MMK_CloseWin(win_id);
            }
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
            s_input_times = 0;
            if(s_input_timer_id != 0)
            {
                MMK_StopTimer(s_input_timer_id);
            }
            #ifdef FM_RDS_SUPPORT
            MMIFM_StopRDSTimer();
            #endif
            //exit fm           
            if(PREVIEW_START_FM == start_way)
            {
                MMISRVAUD_Stop(MMIAPIALARM_GetPlayHandle());
		        MMIAPIFM_Stop();
                MMIAPIALARM_FreePlaySrv();
            }
            else
            {
                MMIFM_Exit(s_is_need_set_bgplay);
            }
        }
        break;
        default:
            result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}
#endif

/*****************************************************************************/
//  Description : FM option window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmMainOptionMsg(
                                         MMI_WIN_ID_T     win_id,   
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    //MMIFM_CUR_INFO_T        *info_ptr = PNULL; 
    uint32                  menu_id = 0;

    //info_ptr = MMIFM_GetFMCurrentInfo();

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            CreateMainOptMenu(MENU_POPUP_CTRL_ID); 
            MMK_SetAtvCtrl(win_id, MENU_POPUP_CTRL_ID);
        }
        break;

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        menu_id = GUIMENU_GetCurNodeId(MENU_POPUP_CTRL_ID);   
        switch(menu_id) 
        {
        case MMIFM_FULL_AUTO_SEARCH:
            MMK_CloseWin(win_id);
            //auto search
           // MMK_PostMsg(FM_MAIN_WIN_ID, MSG_APP_FM_AUTOSEARCH_START, PNULL, 0);
            OpenAuotSearchWin(MMIFM_AUTOSEARCH,FM_SCAN_UP);
            break;
		case MMIFM_CHANNEL_LIST:
			//display channel list
#ifdef MMI_PDA_SUPPORT
                MMK_CreateWin((uint32 *)MMI_FM_CHANNEL_LIST_PDA_WIN_TAB, PNULL);
#else
                MMK_CreateWin((uint32 *)MMI_FM_CHANNEL_LIST_WIN_TAB, (ADD_DATA)MMIFM_OPERATION_LIST);
#endif
            break;
		case MMIFM_SAVE_FREQ:
			//save frequency to channel
			MMK_CreateWin((uint32 *)MMI_FM_CHANNEL_LIST_WIN_TAB, (ADD_DATA)MMIFM_SELECTION_LIST);
            break;
		case MMIFM_MANUAL_SEARCH:
			//manual search
			SetCurrentOperator(MMIFM_MANAUL_SEARCH);
			HandleFMAction(MMIFM_MANAUL_SEARCH, PNULL);
            break; 
		case MMIFM_PLAY_MODE:
			//switch loadspeaker
			MMIFM_SwitchAudioDevice();
            break;
		case MMIFM_SET_BG_PLAY:
			//set bg play
			s_is_need_set_bgplay = TRUE;
			MMIFM_Exit(s_is_need_set_bgplay);
			MMK_ReturnIdleWin();
            break;
#ifdef MMIFM_SUPPORT_FM_RECORD
		case MMIFM_RECORD:
            {          
                if (MMIAPIUDISK_UdiskIsRun())
                {
                   MMIPUB_OpenAlertWarningWin(TXT_UDISK_DISABEL_FM_RECORD);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMI_FM_RECORD_WIN_TAB, (ADD_DATA)FM_record);
                }
            }
            break;
#endif			
#ifdef MMI_FM_TIMER_RECORD_SUPPORT			
        case MMIFM_RECORD_TIMER:
            {
                 MMK_CreateWin((uint32 *)MMI_FM_TIMER_RECORD_WIN_TAB, PNULL);           
            }
            break;
#endif
#ifdef MMIFM_SUPPORT_FORMAT_AMR
        case MMIFM_RECORD_FORMAT_AMR:
            {
                MMIFMRECORD_SetFileFormat(MMISRVAUD_RECORD_FMT_AMR);
                MMK_CloseWin(win_id);
            }
            break;
        case MMIFM_RECORD_FORMAT_WAV:
            {
                MMIFMRECORD_SetFileFormat(MMISRVAUD_RECORD_FMT_ADPCM);
                MMK_CloseWin(win_id);
            }
            break;
#endif   
#ifdef MMIFM_SUPPORT_EQ_MODE
        case MMIFM_EQ_REGULAR:
            {
                MMIFM_SetEqMode(MMISRVAUD_EQ_REGULAR);
                MMK_CloseWin(win_id);
            }
            break;
        case MMIFM_EQ_CLASSIC:
            {
                MMIFM_SetEqMode(MMISRVAUD_EQ_CLASSIC);
                MMK_CloseWin(win_id);
            }
            break;
        case MMIFM_EQ_ODEUM:
            {
                MMIFM_SetEqMode(MMISRVAUD_EQ_ODEUM);
                MMK_CloseWin(win_id);
            }
            break;
        case MMIFM_EQ_JAZZ:
            {
                MMIFM_SetEqMode(MMISRVAUD_EQ_JAZZ);
                MMK_CloseWin(win_id);
            }
            break;
        case MMIFM_EQ_ROCK:
            {
                MMIFM_SetEqMode(MMISRVAUD_EQ_ROCK);
                MMK_CloseWin(win_id);
            }
            break;
        case MMIFM_EQ_SOFT_ROCK:
            {
                MMIFM_SetEqMode(MMISRVAUD_EQ_SOFT_ROCK);
                MMK_CloseWin(win_id);
            }
            break;
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
            case MMIFM_RECORD_FILE:
            {
                MMIFM_RecordOpenRecordFileList();
                MMK_CloseWin(win_id);
            }
            break;            
#endif
#ifdef FM_RDS_SUPPORT
            case MMIFM_RDS_ENABLE:            
             //enable RDS and set button to disable rds ,lv.dong
             MMIFM_SwitchRDS();            
             break;
#endif             
   
           case MMIFM_HELP:
           //help
            MMK_CreateWin((uint32 *)MMIFM_TEXT_BOX_WIN_TAB, PNULL);
            break;
		default:
#ifdef MMIFM_SUPPORT_FM_RECORD
            if(menu_id >= MMIFM_RECORD_FILE_UDISK && menu_id <= MMIFM_RECORD_FILE_MEMCARD_MAX)
			{
				MMIFMRECORD_SetRecordStorage(menu_id - MMIFM_RECORD_FILE_UDISK + MMI_DEVICE_UDISK);
			}
#endif
			//SCI_TRACE_LOW:"[MMIFM] HandleFmMainOptionMsg menu_id %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_2332_112_2_18_2_19_32_65,(uint8*)"d", menu_id);
            break;
			
        }
            MMK_CloseWin(win_id);
        break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
        break;
       /*modify for NEWMS00229074*/
       // case MSG_LOSE_FOCUS:
       //     MMK_CloseWin(win_id);
       // break;

        default:
            result = MMI_RESULT_FALSE;
        break;
        }

    return result;
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : channel list window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChannelListPdaWinMsg(
                                           MMI_WIN_ID_T        win_id,  
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_STRING_T                str_t= {0};
    const GUILIST_ITEM_T        *list_item_ptr = PNULL;
    static uint16               s_cur_list_index = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    static uint32               ch_type = MMIFM_OPERATION_LIST;
    uint16                      i = 0;

    MMI_WIN_ID_T                query_id = FM_QUERY_WIN_ID; 
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_cur_list_index = 0;//high light first item
        MMI_GetLabelTextByLang(TXT_COMM_CHANNEL_LIST, &str_t);
        
        
        ReadChannelsToListBox(win_id, MMI_FM_CHANNEL_LIST_CTRL_ID, ch_type);/*lint !e64*/
        GUILIST_SetCurItemIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index);
        //MMK_SendMsg(win_id, MSG_APP_FM_EDIT_UPDATE, PNULL);
        MMK_SetAtvCtrl(win_id, MMI_FM_CHANNEL_LIST_CTRL_ID);
        GUIWIN_SetTitleText(win_id, str_t.wstr_ptr, str_t.wstr_len, FALSE);
        if(MMIFM_GetValidChannelCount() == 0)
        {
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
            //GUIWIN_UpdateSoftkey(win_id);
        }
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
        GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, MMI_FM_CHANNEL_LIST_CTRL_ID);

        if (0 != MMIFM_GetValidChannelCount())
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif
        break;
    case MSG_APP_FM_EDIT_UPDATE:
        //update current list
        ch_type = MMIFM_OPERATION_LIST;//MMK_GetWinAddDataPtr(win_id);
        MMI_GetLabelTextByLang(TXT_COMM_CHANNEL_LIST, &str_t);
        GUIWIN_SetTitleText(win_id, str_t.wstr_ptr, str_t.wstr_len, FALSE);
        ReadChannelsToListBox(win_id, MMI_FM_CHANNEL_LIST_CTRL_ID, ch_type);/*lint !e64*/
        for(i = 0; i < GUILIST_GetTotalItemNum(MMI_FM_CHANNEL_LIST_CTRL_ID); i++)
        {
            list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, i); 
            //SCI_ASSERT(PNULL != list_item_ptr);/*assert verified*/
            if(PNULL == list_item_ptr)
            {
                break;
            }
            
            if(GetCurrentSelChannel() == (uint16)list_item_ptr->user_data)
            {
                s_cur_list_index = i;
                break;
            }
        }
        GUILIST_SetCurItemIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index);
        if(MMIFM_GetValidChannelCount() == 0)
        {
            GUIWIN_SetSoftkeyBtnState(win_id, 1, TRUE, FALSE);
            GUIWIN_UpdateSoftkey(win_id);
        }
        MMK_SetAtvCtrl(win_id, MMI_FM_CHANNEL_LIST_CTRL_ID);
        break;

    case MSG_GET_FOCUS:
#ifdef MMI_PDA_SUPPORT
        if (0 != MMIFM_GetValidChannelCount())
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
        if (MMIFM_OPERATION_LIST == ch_type)
        {
            if (0 != MMIFM_GetValidChannelCount())
            {
                MMI_WIN_ID_T query_win_id = FM_QUERY_WIN_ID; 
                SetCurrentOperator(MMIFM_DELETE_ALL_CHANNEL);
                MMIPUB_OpenQueryWinByTextId(TXT_FM_DELETE_ALL_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, HandleQuerywin);;

            }
        }
        break;
#endif

    case MSG_APP_MENU:
        if(MMIFM_OPERATION_LIST == ch_type)
        {
            //option
            MMK_CreateWin((uint32 *)FM_CHANNEL_MENU_OPTION_WIN_TAB,PNULL);

            //creat dynamic menu
            GUIMENU_CreatDynamic(PNULL,FM_CHANNEL_MENU_OPTION_WIN_ID,MMIFM_CHANNEL_LIST_MENU_OPTION,GUIMENU_STYLE_POPUP);
        }
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_LIST_LONGOK:
        s_cur_list_index = GUILIST_GetCurItemIndex(MMI_FM_CHANNEL_LIST_CTRL_ID);       
        list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index);    
        //SCI_ASSERT(PNULL != list_item_ptr);/*assert verified*/
        if(PNULL == list_item_ptr)
        {
            break;
        }
        //ch_type = MMK_GetWinAddDataPtr(win_id);
        SetCurrentSelChannel((uint16)list_item_ptr->user_data);
        //SCI_TRACE_LOW:"[MMIFM] HandleChannelListWinMsg MSG_APP_OK s_current_sel_channel %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_2482_112_2_18_2_19_32_66,(uint8*)"d", GetCurrentSelChannel());
        //option
        MMK_CreateWin((uint32 *)FM_CHANNEL_OPTION_WIN_TAB,PNULL);

        //creat dynamic menu
        GUIMENU_CreatDynamic(PNULL,FM_CHANNEL_OPTION_WIN_ID,MENU_POPUP_CTRL_ID,GUIMENU_STYLE_POPUP_AUTO);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            MMIFM_CHANNEL_T new_channel_info = {0};
            
            switch (src_id)
            {
            case MMI_FM_CHANNEL_LIST_CTRL_ID:
                s_cur_list_index = GUILIST_GetCurItemIndex(MMI_FM_CHANNEL_LIST_CTRL_ID);       
                list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index); 
                //SCI_ASSERT(PNULL != list_item_ptr);/*assert verified*/
                if(PNULL == list_item_ptr)
                {
                    break;
                }
                SetCurrentSelChannel((uint16)list_item_ptr->user_data);
                //ch_type = MMK_GetWinAddDataPtr(win_id);
                //SCI_TRACE_LOW:"[MMIFM] HandleChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_2508_112_2_18_2_19_32_67,(uint8*)"d", GetCurrentSelChannel());
                
                if(MMIFM_IsChannelExist(GetCurrentSelChannel()))
                {
                	MMIFM_GetOneChannel(GetCurrentSelChannel(), &new_channel_info);
                    if(MMIFM_GetFMCurrentInfo()->cur_freq != new_channel_info.freqency)
                    {
                    	MMIFM_PlayChannel(GetCurrentSelChannel());
                    }
                    SetButtonBgWhenResume();
               		MMK_CloseWin(win_id);
                }
                else
                {
#ifdef MMI_PDA_SUPPORT
                    MMIPUB_DisplayTipsIDEx(PNULL, TXT_FM_INVALID_FREQ, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
#else
                    MMIPUB_OpenAlertWarningWin(TXT_FM_INVALID_FREQ);
#endif          
                }
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                SetCurrentOperator(MMIFM_DELETE_ALL_CHANNEL);
#ifndef MMI_TASK_MANAGER
                MMIPUB_OpenQueryWinByTextId(TXT_FM_DELETE_ALL_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin);
#else
                MMIPUB_OpenQueryWinByTextIdEx(SPRD_FM_APPLET_ID,
                    TXT_FM_DELETE_ALL_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin,
                    PNULL);
#endif
                break;          
            default:
                break;
            }

        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
#endif
/*****************************************************************************/
//  Description : channel list window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChannelListWinMsg(
                                           MMI_WIN_ID_T        win_id,  
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_STRING_T                str_t= {0};
    const GUILIST_ITEM_T        *list_item_ptr = PNULL;
    static uint16               s_cur_list_index = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    static uint32               ch_type = MMIFM_OPERATION_LIST;
    uint16                      i = 0;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        ch_type = MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
        if(MMIFM_OPERATION_LIST == ch_type)/*lint !e64*/
        {
            s_cur_list_index = 0;//high light first item
            MMI_GetLabelTextByLang(TXT_COMM_CHANNEL_LIST, &str_t);
        }
        else
        {
            //high light first empty channel
            s_cur_list_index = MMIFM_GetValidChannelCount();
            if(s_cur_list_index >= MMIFM_MAX_CHANNEL_NUM)
            {
                //not empty channel, high light first channle
                s_cur_list_index = 0;
            }
            MMI_GetLabelTextByLang(TXT_COMM_SAVE_TO, &str_t);
        }
        ReadChannelsToListBox(win_id, MMI_FM_CHANNEL_LIST_CTRL_ID, ch_type);/*lint !e64*/
        GUILIST_SetCurItemIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index);
        //MMK_SendMsg(win_id, MSG_APP_FM_EDIT_UPDATE, PNULL);
        MMK_SetAtvCtrl(win_id, MMI_FM_CHANNEL_LIST_CTRL_ID);
        GUIWIN_SetTitleText(win_id, str_t.wstr_ptr, str_t.wstr_len, FALSE);
#ifdef MMI_PDA_SUPPORT
        if(MMIFM_OPERATION_LIST == ch_type)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
#endif
        break;
    case MSG_APP_FM_EDIT_UPDATE:
        //update current list
        ch_type =MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
        MMI_GetLabelTextByLang(TXT_COMM_CHANNEL_LIST, &str_t);
        GUIWIN_SetTitleText(win_id, str_t.wstr_ptr, str_t.wstr_len, FALSE);
        ReadChannelsToListBox(win_id, MMI_FM_CHANNEL_LIST_CTRL_ID, ch_type);/*lint !e64*/
        for(i = 0; i < GUILIST_GetTotalItemNum(MMI_FM_CHANNEL_LIST_CTRL_ID); i++)
        {
            list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, i); 
            //SCI_ASSERT(PNULL != list_item_ptr);/*assert verified*/
            if(PNULL == list_item_ptr)
            {
                break;
            }
            
            if(GetCurrentSelChannel() == (uint16)list_item_ptr->user_data)
            {
                s_cur_list_index = i;
                break;
            }
        }
        GUILIST_SetCurItemIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index);
        MMK_SetAtvCtrl(win_id, MMI_FM_CHANNEL_LIST_CTRL_ID);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        s_cur_list_index = GUILIST_GetCurItemIndex(MMI_FM_CHANNEL_LIST_CTRL_ID);       
        list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index);    
        //SCI_ASSERT(PNULL != list_item_ptr);/*assert verified*/
        if(PNULL == list_item_ptr)
        {
            break;
        }
        //ch_type = MMK_GetWinAddDataPtr(win_id);
        SetCurrentSelChannel((uint16)list_item_ptr->user_data);
        //SCI_TRACE_LOW:"[MMIFM] HandleChannelListWinMsg MSG_APP_OK s_current_sel_channel %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_2642_112_2_18_2_19_33_68,(uint8*)"d", GetCurrentSelChannel());
        if(MMIFM_OPERATION_LIST == ch_type)
        {
            //option
            MMK_CreateWin((uint32 *)FM_CHANNEL_OPTION_WIN_TAB,PNULL);

            //creat dynamic menu
            GUIMENU_CreatDynamic(PNULL,FM_CHANNEL_OPTION_WIN_ID,MENU_POPUP_CTRL_ID,GUIMENU_STYLE_POPUP);
        }
        else
        {
            //select
            if(MSG_APP_MENU!=msg_id)
            {
                HandleSaveCurrentFreq(GetCurrentSelChannel());
            }
        }
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        s_cur_list_index = GUILIST_GetCurItemIndex(MMI_FM_CHANNEL_LIST_CTRL_ID);       
        list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index); 
        //SCI_ASSERT(PNULL != list_item_ptr);/*assert verified*/
        if(PNULL == list_item_ptr)
        {
            break;
        }
        SetCurrentSelChannel((uint16)list_item_ptr->user_data);
        //SCI_TRACE_LOW:"[MMIFM] HandleChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_2672_112_2_18_2_19_33_69,(uint8*)"d", GetCurrentSelChannel());
        
        if(MMIFM_OPERATION_LIST == ch_type)
        {
            //play this channel
            if(MMIFM_IsChannelExist(GetCurrentSelChannel()))
            {
                MMIFM_PlayChannel(GetCurrentSelChannel());
#if defined(MMI_FM_MINI_SUPPORT) || defined( MMI_MINI_QVGA_UI_SUPPORT)
                /* fixed by langhua.yang for NEWMS00207297 */
                MMIFM_SetCurrrentChannel(GetCurrentSelChannel());
#endif
                SetButtonBgWhenResume();
            }
            MMK_CloseWin(win_id);            
        }
        else
        {
            //select
            s_cur_list_index = GUILIST_GetCurItemIndex(MMI_FM_CHANNEL_LIST_CTRL_ID);       
            //list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index);
            HandleSaveCurrentFreq(GetCurrentSelChannel());
        }  
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : FM option window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmChannelMenuOptionMsg(
   MMI_WIN_ID_T     win_id,     
   MMI_MESSAGE_ID_E    msg_id, 
   DPARAM              param
   )
{
   MMI_RESULT_E            result = MMI_RESULT_TRUE;
   uint32                  menu_id = 0;
   MMI_WIN_ID_T            query_id = FM_QUERY_WIN_ID; 
   
   switch (msg_id)
   {
   case MSG_OPEN_WINDOW:
       CreateChannelMenuOptionMenu(MMIFM_CHANNEL_LIST_MENU_OPTION); 
       MMK_SetAtvCtrl(win_id, MMIFM_CHANNEL_LIST_MENU_OPTION);
       break;
       
   case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_OK:
   case MSG_APP_WEB:
   case MSG_CTL_MIDSK:
       menu_id = GUIMENU_GetCurNodeId(MMIFM_CHANNEL_LIST_MENU_OPTION);     
       switch(menu_id)  
       {
       case MMIFM_CHANNEL_DELETE_ALL:
           //delete this channel
           SetCurrentOperator(MMIFM_DELETE_ALL_CHANNEL);
           MMIPUB_OpenQueryWinByTextId(TXT_FM_DELETE_ALL_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin);;
           break;
       default:
           break;
           
       }
       MMK_CloseWin(win_id);
       
       break;
       
       case MSG_APP_CANCEL:
       case MSG_CTL_CANCEL:
       case MSG_LOSE_FOCUS:
           MMK_CloseWin(win_id);
           break;
           
       default:
           result = MMI_RESULT_FALSE;
           break;
   }
   
   return result;
}
#endif
/*****************************************************************************/
//  Description : FM option window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmChannelOptionMsg(
   MMI_WIN_ID_T     win_id,     
   MMI_MESSAGE_ID_E    msg_id, 
   DPARAM              param
   )
{
   MMI_RESULT_E            result = MMI_RESULT_TRUE;
   uint32                  menu_id = 0;
   MMI_WIN_ID_T            query_id = FM_QUERY_WIN_ID; 
   
   switch (msg_id)
   {
   case MSG_OPEN_WINDOW:
       CreateChannelOptionMenu(MENU_POPUP_CTRL_ID); 
       MMK_SetAtvCtrl(win_id, MENU_POPUP_CTRL_ID);
       break;
       
   case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_OK:
   case MSG_APP_WEB:
   case MSG_CTL_MIDSK:
       menu_id = GUIMENU_GetCurNodeId(MENU_POPUP_CTRL_ID);     
       switch(menu_id)  
       {
       case MMIFM_CHANNEL_PLAY:
           //play this channel
            if(MMIFM_IsChannelExist(GetCurrentSelChannel()))
            {
                MMIFM_PlayChannel(GetCurrentSelChannel());                
            }
            else
            {
                //channel empty, play default frequency 87.5
                MMIFM_PlayFreq(MMIFM_FREQ_RANGE_LOW);
            }
            SetButtonBgWhenResume();
           break;
       case MMIFM_CHANNEL_EDIT:
           //edit this channel
           MMK_CreateWin((uint32 *)MMIFM_CHANNEL_DETAIL_WIN_TAB, PNULL);
           break;
       case MMIFM_CHANNEL_DELETE:
           //delete this channel
           SetCurrentOperator(MMIFM_DELETE_ONE_CHANNEL);
#ifndef MMI_TASK_MANAGER
           MMIPUB_OpenQueryWinByTextId(TXT_FM_DELETE_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin);
#else
           MMIPUB_OpenQueryWinByTextIdEx(SPRD_FM_APPLET_ID,
                TXT_FM_DELETE_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin,
                PNULL);
#endif
           break;
       case MMIFM_CHANNEL_DELETE_ALL:
           //delete this channel
           SetCurrentOperator(MMIFM_DELETE_ALL_CHANNEL);
#ifndef MMI_TASK_MANAGER
           MMIPUB_OpenQueryWinByTextId(TXT_FM_DELETE_ALL_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin);
#else
           MMIPUB_OpenQueryWinByTextIdEx(SPRD_FM_APPLET_ID,
                TXT_FM_DELETE_ALL_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin,
                PNULL);
#endif
           break;
       default:
           break;
           
       }
       MMK_CloseWin(win_id);
       
       break;
       
       case MSG_APP_CANCEL:
       case MSG_CTL_CANCEL:
       case MSG_LOSE_FOCUS:
           MMK_CloseWin(win_id);
           break;
           
       default:
           result = MMI_RESULT_FALSE;
           break;
   }
   
   return result;
}

/*****************************************************************************/
//  Description : FM option window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQuerywin(   MMI_WIN_ID_T     win_id,   
   MMI_MESSAGE_ID_E    msg_id, 
   DPARAM              param
   )
{
   MMI_RESULT_E    result = MMI_RESULT_TRUE;
   MMI_STRING_T    kstring = {0};
   MMIFM_CHANNEL_T channel ={0};
   
   switch (msg_id)
   {
   case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_OK:
       MMIFM_GetOneChannel(GetCurrentSelChannel(), &channel);
       kstring.wstr_ptr = channel.name;
       kstring.wstr_len = MMIAPICOM_Wstrlen(channel.name);
       HandleFMAction(GetCurrentOperator(), &kstring);
       MMK_CloseWin(win_id);
       break;
       
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
       MMK_CloseWin(win_id);
       break;
       
   case MSG_CLOSE_WINDOW:
       
       break;
       
   default:
       result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
       break;
   }
   
   return result;
}

/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditBoxWinMsg(
   MMI_WIN_ID_T        win_id, 
   MMI_MESSAGE_ID_E    msg_id, 
   DPARAM              param
   )
{
   MMI_RESULT_E   result   = MMI_RESULT_TRUE;
   MMI_STRING_T   str_info = {0};
   MMI_STRING_T   str_t    = {0};
   MMI_TEXT_ID_T  title_id = TXT_NULL;
   MMIFM_CUR_INFO_T *cur_info_ptr = PNULL;
   MMIFM_CHANNEL_T channel = {0};   
   MMIFM_OPERTION_TYPE_E opt_type = MMIFM_DO_NOTHING;
   
   switch (msg_id)
   {
   case MSG_OPEN_WINDOW:
       title_id = (MMI_TEXT_ID_T)MMK_GetWinAddDataPtr(win_id);
       GUIWIN_SetTitleTextId(win_id, title_id, FALSE);
//#ifdef MMI_PDA_SUPPORT
//        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
//#endif
	   //设置edit控件不处理红键消息
//	   GUIEDIT_SetHandleRedKey(FALSE, MMIFM_EDITBOX_CTRL_ID);
       MMK_SetAtvCtrl(win_id, MMIFM_EDITBOX_CTRL_ID);
       break;
       
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK: 
#ifdef MMI_PDA_SUPPORT
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK,  PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_CTL_CANCEL,  PNULL);
                break;            
            default:
                break;
            }
        }
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_APP_OK:
   case MSG_CTL_OK:
   case MSG_CTL_MIDSK:
   //case MSG_APP_MENU:
       // 从EDITBOX中获得数据
       opt_type = GetCurrentOperator();
       //SCI_TRACE_LOW:"[MMIFM] HandleEditBoxWinMsg MSG_APP_WEB s_current_operator %d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_2947_112_2_18_2_19_33_70,(uint8*)"d", opt_type);
       
       //normal edit box
       GUIEDIT_GetString(MMIFM_EDITBOX_CTRL_ID, &str_info);
       if (str_info.wstr_len > 0 && PNULL != str_info.wstr_ptr)
       {
           str_t.wstr_len = str_info.wstr_len; //MIN(MMIPB_MAX_TITLE_LEN, str_info.str_len);
           str_t.wstr_ptr = str_info.wstr_ptr;
       }
       else
       {
            str_t.wstr_len = 0;
       }
       if(MMIFM_SAVE_FREQUENCY == opt_type)
       {
           //save current frequency
           cur_info_ptr = MMIFM_GetFMCurrentInfo();
           if(PNULL == cur_info_ptr)
           {
               channel.freqency = MMIFM_FREQ_RANGE_LOW;
           }
           else
           {
               channel.freqency = cur_info_ptr->cur_freq;
           }
           str_t.wstr_len = MIN(MMIFM_CHANNEL_NAME_LEN, str_t.wstr_len);
           MMIAPICOM_Wstrncpy(channel.name, str_t.wstr_ptr, str_t.wstr_len);
           if(!MMIFM_SetOneChannel(GetCurrentSelChannel(), &channel))
           {
               //save error
               MMIPUB_OpenAlertFailWin(TXT_FM_INVALID_FREQ);           
           }
           else
           {
#ifndef  MMI_PDA_SUPPORT
              // MMIPUB_OpenAlertSuccessWin(TXT_FM_SAVE_CHANNEL);                
#endif
               //update current channal list
               if(MMK_IsOpenWin(MMI_FM_CHANNEL_LIST_WIN_ID))
               {
                   MMK_SendMsg(MMI_FM_CHANNEL_LIST_WIN_ID, MSG_APP_FM_EDIT_UPDATE, PNULL);
               }
               MMIFM_SetCurrrentChannel(GetCurrentSelChannel());
           }
       }
       else if(MMIFM_MANAUL_SEARCH == opt_type)
       {
         //MANAUL_SEARCH           
         if (MMIAPIALM_IsValidFM(str_t.wstr_ptr, str_t.wstr_len))
         {
           if(MMIFM_PlayFreq(((uint16)ConvertFloatStrToInt(&str_t, 10))))
           {
             SetButtonBgWhenResume();
           }
		   /*
           else            
           {
             MMIPUB_OpenAlertFailWin(TXT_FM_INVALID_FREQ);      
           }
           */
         }
	     else
	     {
		   MMIPUB_OpenAlertFailWin(TXT_FM_INVALID_FREQ);  
	     }
       }
       else if(MMIFM_EDIT_CHANNEL_FREQ == opt_type || MMIFM_EDIT_CHANNEL_NAME == opt_type)
       {
           MMK_SendMsg(MMIFM_CHANNEL_DETAIL_WIN_ID, MSG_APP_FM_EDIT_UPDATE, &str_t);
       }
       else
       {
           //SCI_TRACE_LOW:"HandleEditBoxWinMsg OK s_current_operator %d !"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_3011_112_2_18_2_19_33_71,(uint8*)"d", opt_type);
       }
       MMK_CloseWin(win_id);
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
//  Discription: Handle textbox msg
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleTextBoxWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param 
                                         )
{
    MMI_RESULT_E      result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T     ctrl_id = MMIFM_TEXTBOX_CTRL_ID;
    MMI_STRING_T        content_str = {0};
    
    switch( msg_id ) 
    {
    case MSG_OPEN_WINDOW:
        //ClrClientRect();
        MMI_GetLabelTextByLang(TXT_HELP, &content_str);
        GUIWIN_SetTitleText(win_id, content_str.wstr_ptr, content_str.wstr_len, FALSE);
        MMI_GetLabelTextByLang(TXT_HELP_CONTENT, &content_str);
        //display text
        GUITEXT_SetString(
            ctrl_id, 
            content_str.wstr_ptr,
            content_str.wstr_len,
            FALSE
            );

        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        ClrClientRect();
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : channel details处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleChannelDetailWinMsg(
                                             MMI_WIN_ID_T        win_id, 
                                             MMI_MESSAGE_ID_E    msg_id, 
                                             DPARAM              param
                                             )
{
    MMI_RESULT_E     result   = MMI_RESULT_TRUE;
    //MMI_STRING_T     str_t    = {0};
    //MMI_STRING_T     *str_ptr = PNULL;
    LOCAL MMIFM_CHANNEL_T s_channel = {0};
    //LOCAL uint16      s_high_list_id = MMIFM_CHANNEL_NAME;
    uint16            freq = 0;
    uint16            ch_index = 0;
    wchar             wdisp_name[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
    uint8             disp_name[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
    //LOCAL GUI_POINT_T s_tp_point = {0};
    int32 str_len = 0;
    //MMI_CTRL_ID_T      get_active_ctrl = 0;
    MMI_STRING_T   fm_name_info = {0};
    MMI_STRING_T   fm_fre_info = {0};
	wchar                   dot_str[] = {'.'};
#ifdef MMI_PDA_SUPPORT
    //MMI_CONTROL_CREATE_T        softkey_ctrl = {0};
    //GUISOFTKEY_INIT_DATA_T      softkey_init = {0};
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
/*#ifdef MMI_PDA_SUPPORT
        softkey_init.style        = GUISOFTKEY_STYLE_2BUTTON;
        //init softkey
        softkey_init.leftsoft_id  = STXT_SAVE;
        softkey_init.midsoft_id   = TXT_NULL;
        softkey_init.rightsoft_id = STXT_CANCEL;

        //create softkey control
        softkey_ctrl.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
        softkey_ctrl.guid              = SPRD_GUI_SOFTKEY_ID;
        softkey_ctrl.init_data_ptr     = &softkey_init;
        softkey_ctrl.parent_win_handle = win_id;
        MMK_CreateControl(&softkey_ctrl);
        MMK_SetWinSoftkeyCtrlId(win_id,softkey_ctrl.ctrl_id);
//#endif*/
        ch_index = GetCurrentSelChannel();
        MMIFM_GetOneChannel(ch_index, &s_channel);
        //display title
        sprintf((char*)disp_name, "%s%d",MMIFM_CHANNEL_TITLE, ch_index +1);
        str_len = strlen((char*)disp_name);
        str_len = MIN(MMIFM_CHANNEL_NAME_LEN, str_len);
        MMI_STRNTOWSTR(wdisp_name, MMIFM_CHANNEL_NAME_LEN, disp_name, MMIFM_CHANNEL_NAME_LEN, str_len);
        GUIWIN_SetTitleText(win_id, wdisp_name, MMIAPICOM_Wstrlen(wdisp_name), FALSE); 
        MMK_SetAtvCtrl(win_id, MMIFM_DETAILS_NAME_EDIT_LABEL_CTRL_ID);
        GUIEDIT_SetIm(MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
        GUIEDIT_SetImTag(MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID, GUIIM_TAG_DOT_NUM);
		GUIEDIT_ConfigImCustomKey(MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID, GUIIM_TAG_DOT_NUM);
//#ifdef MMI_FM_MINI_SUPPORT
#ifndef MMI_PDA_SUPPORT
        GUIFORM_SetType(MMIFM_DETAILS_FROM_ID,GUIFORM_TYPE_TP);
		GUIEDIT_SetAlign(MMIFM_DETAILS_NAME_EDIT_LABEL_CTRL_ID,ALIGN_LVMIDDLE);
		GUIEDIT_SetAlign(MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID,ALIGN_LVMIDDLE);
//#endif
#endif
        DrawChannelDetails(win_id,&s_channel);
		//设置edit控件不处理红键消息
		//GUIEDIT_SetHandleRedKey(FALSE, MMIFM_DETAILS_NAME_EDIT_LABEL_CTRL_ID);
		//GUIEDIT_SetHandleRedKey(FALSE, MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID);
        break;
    case MSG_APP_OK:
    case MSG_CTL_OK: 
        GUIEDIT_GetString(MMIFM_DETAILS_NAME_EDIT_LABEL_CTRL_ID, &fm_name_info);
        GUIEDIT_GetString(MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID, &fm_fre_info);
        SCI_MEMSET(s_channel.name, 0x00, sizeof(s_channel.name));
        if(fm_name_info.wstr_len > 0  && PNULL != fm_name_info.wstr_ptr)
        {
            MMIAPICOM_Wstrncpy(s_channel.name, fm_name_info.wstr_ptr, MIN(fm_name_info.wstr_len, MMIFM_CHANNEL_NAME_LEN));
        }
        freq = ConvertFloatStrToInt(&fm_fre_info, 10);
	    if(!MMIAPIALM_IsValidFM(fm_fre_info.wstr_ptr, fm_fre_info.wstr_len))
        {
            //valid freqency
           MMIPUB_OpenAlertFailWin(TXT_FM_INVALID_FREQ);                
        }
        else
        {
            s_channel.freqency = freq;
            if(MMIFM_SetOneChannel(GetCurrentSelChannel(), &s_channel))
            {
                if(MMK_IsOpenWin(MMI_FM_CHANNEL_LIST_WIN_ID))
                {
                    MMK_SendMsg(MMI_FM_CHANNEL_LIST_WIN_ID, MSG_APP_FM_EDIT_UPDATE, PNULL);
                }
                MMK_CloseWin(win_id);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_FM_INVALID_FREQ);                
            }
        }                
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK,  PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL,  PNULL);
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
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
//  Description : change float string to integer value
//  Global resource dependence : 
//  Parameter: str_ptr[IN] : the number string with decimal
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int16 ConvertFloatStrToInt(const MMI_STRING_T *str_ptr, int32 mul_scale)
{
    BOOLEAN result = TRUE;
    int32  i = 0;
    int16  ret =0;
    uint16 num_len = 0;
    int16  point_index =0;
    uint8  num_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
    uint8  decimal_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
    
    if(str_ptr != PNULL && str_ptr->wstr_ptr != PNULL)
    {
        num_len = MIN(MMIFM_CHANNEL_NAME_LEN, str_ptr->wstr_len);
        MMI_WSTRNTOSTR(num_buf, MMIFM_CHANNEL_NAME_LEN, str_ptr->wstr_ptr, MMIFM_CHANNEL_NAME_LEN, num_len);
        //get point postion
        for(i =0; i< num_len; i++)
        {
            if('.' == num_buf[i])
            {
                point_index = i;
                break;
            }
        }
        
        for(i = point_index +1; i < num_len; i++)
        {
             if( ( '0' > num_buf[i] ) || ( '9' < num_buf[i] )) 
             {
                 result = FALSE;
                 break;
             }
        }
        
        if(result)
        {
            if(0 != point_index && (point_index + 1 < num_len))
            {
                strncpy((char*)decimal_buf, (char*)&num_buf[point_index +1], 1);
            }
            if(point_index > 0)
            {
                //no point
                num_buf[point_index] = 0;
            }
            ret = atoi((char*)num_buf)*mul_scale;
            ret += atoi((char*)decimal_buf);
        }
        else
        {
            ret = 0;
        }
    }
    //SCI_TRACE_LOW:"[MMIFM] ConvertFloatStrToInt ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_3259_112_2_18_2_19_34_72,(uint8*)"d", ret);
    return ret;
}
/*****************************************************************************/
//  Description : draw channel details win
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DrawChannelDetails(MMI_WIN_ID_T win_id,const MMIFM_CHANNEL_T *channel_ptr)
{
    MMI_STRING_T str_t = {0};
    //BOOLEAN      is_focus = FALSE;
    uint8 num_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
    wchar wnum_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};

    GUIFORM_SetStyle(MMIFM_DETAILS_NAME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIFM_DETAILS_FREQ_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    //draw name label
    MMI_GetLabelTextByLang(TXT_FM_CHANNEL_ANME_COLON, &str_t);
    GUILABEL_SetText(MMIFM_DETAILS_NAME_LABEL_CTRL_ID, &str_t, FALSE);
    
    if(PNULL != channel_ptr)
    {
        str_t.wstr_ptr = channel_ptr->name; //lint !e605
        str_t.wstr_len = MMIAPICOM_Wstrlen(channel_ptr->name);
    }
    else
    {
        str_t.wstr_len = 0;
    }
    GUIEDIT_SetString(MMIFM_DETAILS_NAME_EDIT_LABEL_CTRL_ID, str_t.wstr_ptr, str_t.wstr_len);
    //draw channel number
    MMI_GetLabelTextByLang(TXT_FM_FREQEUENCY_COLON, &str_t);
    GUILABEL_SetText(MMIFM_DETAILS_FREQ_LABEL_CTRL_ID, &str_t, FALSE);
    
    if(PNULL != channel_ptr && MMIFM_IsChannelValid(channel_ptr))
    {
        //channel's frequency valid
        sprintf((char*)num_buf, "%d.%d",channel_ptr->freqency/10, channel_ptr->freqency%10);
        str_t.wstr_len = strlen((char*)num_buf);
        str_t.wstr_len = MIN(MMIFM_CHANNEL_NAME_LEN, str_t.wstr_len);
        str_t.wstr_ptr = wnum_buf;
        MMI_STRNTOWSTR(str_t.wstr_ptr, MMIFM_CHANNEL_NAME_LEN, num_buf, MMIFM_CHANNEL_NAME_LEN, str_t.wstr_len);
    }
    else
    {
        str_t.wstr_len = 0;
        str_t.wstr_ptr = PNULL;
    }
    GUIEDIT_SetString(MMIFM_DETAILS_FREQ_EDIT_LABEL_CTRL_ID, str_t.wstr_ptr, str_t.wstr_len);
}

/*****************************************************************************/
//  Description : EditWinDrawFocusRect 
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
LOCAL void DrawEditRect(
                        GUI_RECT_T rect,
                        GUI_COLOR_T color,
                        BOOLEAN     is_focus,
                        MMI_CTRL_ID_T ctrl_id
                        )
{
    GUI_COLOR_T         bg_color = MMI_DARK_BLUE_COLOR;
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);
    //draw rect line
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.left, rect.top, rect.bottom, color);
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.right, rect.top, rect.bottom, color);
    
    //draw label
    if(is_focus)
    {
        bg_color = MMI_DARK_BLUE_COLOR;
    }

    GUILABEL_SetBackgroundColor(ctrl_id,bg_color);
}

/*****************************************************************************/
//  Description : handle fm actions
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleFMAction(MMIFM_OPERTION_TYPE_E act_type, const MMI_STRING_T *str_ptr)
{
    //SCI_TRACE_LOW:"[MMIFM] HandleFMAction act_type %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_3347_112_2_18_2_19_34_73,(uint8*)"d", act_type);
    switch(act_type)
    {
    case MMIFM_SAVE_FREQUENCY:
        //save frequency
        MMK_CreateWin((uint32 *)MMIFM_EDITBOX_WIN_TAB, (ADD_DATA)TXT_FM_CHANNEL_NAME);
        CreateEditBox(str_ptr, MMIFM_CHANNEL_NAME_LEN, GUIIM_TYPE_SET_ALL, MMIFM_EDITBOX_WIN_ID);
        break;
    case MMIFM_MANAUL_SEARCH:
        //manual search
        MMK_CreateWin((uint32 *)MMIFM_EDITBOX_WIN_TAB, (ADD_DATA)TXT_FM_FREQEUENCY);
        CreateEditBox(str_ptr, MMIFM_DIGIT_MAX_LEN, GUIIM_TYPE_DIGITAL, MMIFM_EDITBOX_WIN_ID);
        break;
    case MMIFM_EDIT_CHANNEL_NAME:
        //edit channel name
        MMK_CreateWin((uint32 *)MMIFM_EDITBOX_WIN_TAB, (ADD_DATA)TXT_FM_CHANNEL_NAME);
        CreateEditBox(str_ptr, MMIFM_CHANNEL_NAME_LEN, GUIIM_TYPE_SET_ALL, MMIFM_EDITBOX_WIN_ID);
        break;
    case MMIFM_EDIT_CHANNEL_FREQ:
        //edit channel freq
        MMK_CreateWin((uint32 *)MMIFM_EDITBOX_WIN_TAB, (ADD_DATA)TXT_FM_FREQEUENCY);
        CreateEditBox(str_ptr, MMIFM_CHANNEL_NAME_LEN, GUIIM_TYPE_DIGITAL, MMIFM_EDITBOX_WIN_ID);
        break;
    case MMIFM_DELETE_ONE_CHANNEL:
        //delete one channel
        MMIFM_DeleteOneChannel(GetCurrentSelChannel());
        if(MMK_IsOpenWin(MMI_FM_CHANNEL_LIST_WIN_ID))
        {
            MMK_SendMsg(MMI_FM_CHANNEL_LIST_WIN_ID, MSG_APP_FM_EDIT_UPDATE, PNULL);
        }
        break;
    case MMIFM_DELETE_ALL_CHANNEL:
        //delete one channel
        MMIFM_DeleteAllChannels();
        if(MMK_IsOpenWin(MMI_FM_CHANNEL_LIST_WIN_ID))
        {
            MMK_SendMsg(MMI_FM_CHANNEL_LIST_WIN_ID, MSG_APP_FM_EDIT_UPDATE, PNULL);
        }
        break;
    default:
        //SCI_TRACE_LOW:"[MMIFM] HandleFMAction act_type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_3387_112_2_18_2_19_34_74,(uint8*)"d", act_type);
        break;
    }
}


/*****************************************************************************/
//  Description : create EDITBOX
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateEditBox(const MMI_STRING_T *text_ptr,
                         uint16       max_len,
                         uint8        allow_inputmethod,
                         MMI_WIN_ID_T win_id)
{
    MMISET_LANGUAGE_TYPE_E  lang_type = MMISET_LANGUAGE_ENGLISH;
    GUIIM_TYPE_T         input_mode = 0;    
    GUIEDIT_INIT_DATA_T     init_data = {0};
    wchar                   dot_str[] = {'.'};
#ifndef MMI_PDA_SUPPORT
    MMI_CONTROL_CREATE_T    create = {0};
#endif
#ifdef MMI_PDA_SUPPORT
    GUIFORM_DYNA_CHILD_T  child_data={0};
    GUISOFTKEY_INIT_DATA_T      softkey_init = {0};
#endif
    if(GUIIM_TYPE_DIGITAL == allow_inputmethod)
    {
        //only digital mode 
        input_mode = GUIIM_TYPE_DIGITAL;
    }
    else 
    {
        MMIAPISET_GetLanguageType(&lang_type);
        if (MMISET_LANGUAGE_SIMP_CHINESE == lang_type)
        {
            input_mode = GUIIM_TYPE_SMART;
        }
        else
        {
            input_mode = GUIIM_TYPE_ABC;
        }
    }
    
#ifdef MMI_PDA_SUPPORT
    GUIFORM_CreatDynaCtrl(win_id,MMIFM_EDITWIN_PDA_FORM_CTRL_ID,GUIFORM_LAYOUT_ORDER);

    child_data.is_bg=FALSE;
    child_data.guid=SPRD_GUI_EDITBOX_ID;
    child_data.is_get_active=TRUE;
    child_data.child_handle=MMIFM_EDITBOX_CTRL_ID;
    init_data.type = GUIEDIT_TYPE_TEXT;
    init_data.is_rect_adaptive = TRUE;
    init_data.str_max_len = max_len;
    child_data.init_data_ptr=&init_data;
    GUIFORM_CreatDynaChildCtrl(win_id,MMIFM_EDITWIN_PDA_FORM_CTRL_ID,&child_data);

    GUIEDIT_SetIm(MMIFM_EDITBOX_CTRL_ID,allow_inputmethod,input_mode);
    if(GUIIM_TYPE_DIGITAL == input_mode)
    {
        GUIEDIT_ConfigImCustomKey(MMIFM_EDITBOX_CTRL_ID, input_mode, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(MMIFM_EDITBOX_CTRL_ID, GUIIM_TAG_DOT_NUM);
    }
    child_data.is_softkey = TRUE;
    child_data.guid=SPRD_GUI_SOFTKEY_ID;
    child_data.child_handle=MMITHEME_GetSoftkeyCtrlId();
    softkey_init.leftsoft_id  = TXT_COMMON_OK;
    softkey_init.midsoft_id   = TXT_NULL;
    softkey_init.rightsoft_id = STXT_CANCEL;
    child_data.init_data_ptr=&softkey_init;
        
    GUIFORM_CreatDynaChildCtrl(win_id,MMIFM_EDITWIN_PDA_FORM_CTRL_ID,&child_data);
    
#else
    init_data.type = GUIEDIT_TYPE_TEXT;
    init_data.both_rect= MMITHEME_GetWinClientBothRect(win_id);
    init_data.str_max_len = max_len;

    create.ctrl_id = MMIFM_EDITBOX_CTRL_ID;
    create.guid    = SPRD_GUI_EDITBOX_ID;
    create.parent_win_handle = win_id;//MMIPB_EDITBOX_WIN_ID;
    create.init_data_ptr = &init_data;
    MMK_CreateControl( &create );

    GUIEDIT_SetIm(create.ctrl_id,allow_inputmethod,input_mode);
    if(GUIIM_TYPE_DIGITAL == input_mode)
    {
        GUIEDIT_ConfigImCustomKey(create.ctrl_id, input_mode, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(create.ctrl_id, GUIIM_TAG_DOT_NUM);
    }
    if (PNULL != text_ptr)
    {
        GUIEDIT_SetString(create.ctrl_id,text_ptr->wstr_ptr,text_ptr->wstr_len);
    }
#endif    
}

/*****************************************************************************/
//  Description : read channels to list box
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIAPIFM_GetChannelName(uint16 fre)
{ 
    uint16 i = 0;
    MMIFM_CHANNEL_T channel = {0};
    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
       SCI_MEMSET(&channel, 0x00, sizeof(MMIFM_CHANNEL_T)); 
       if(MMIFM_GetOneChannel(i, &channel) && MMIFM_IsChannelValid(&channel))
       {
            if((fre==channel.freqency)&& (MMIAPICOM_Wstrlen(channel.name)>0))
            {
                SCI_MEMSET(s_channel_name,0,(MMIFM_CHANNEL_NAME_LEN + 1)*sizeof(wchar));
                MMIAPICOM_Wstrncpy(s_channel_name,channel.name,MMIAPICOM_Wstrlen(channel.name));
                return s_channel_name;
            }
       }
    }

    return  PNULL;
}
/*****************************************************************************/
//  Description : read channels to list box
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ReadChannelsToListBox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMIFM_CHANNEL_LIST_TYPE_E ch_type)
{   
    uint16 i = 0;
    MMIFM_CHANNEL_T channel = {0};   /*lint !e64*/
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    
    GUILIST_SetMaxItem(ctrl_id,MMIFM_MAX_CHANNEL_NUM, FALSE );
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;   
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
    
    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
        //get exist channels firstly
        SCI_MEMSET(&channel, 0x00, sizeof(MMIFM_CHANNEL_T));
        SCI_MEMSET(temp_wstr, 0x00, sizeof(wchar)*(GUILIST_STRING_MAX_NUM + 1));        
        if(MMIFM_GetOneChannel(i, &channel) && MMIFM_IsChannelValid(&channel))
        {
            //exist channel
            if(MMIFM_OPERATION_LIST == ch_type)
            {
                item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
#ifndef GUI_MID_SOFTKEY_USE_IMG 
                item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_FM_PLAY;
#else           
                item_data.softkey_id[1] = (MMI_IMAGE_ID_T)IMAGE_COMMON_SOFTKEY_START_ICON;
#endif
            }
            else
            {
                item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_SELECT;
#ifndef MMI_GUI_STYLE_MINISCREEN 
                item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
#else           
                item_data.softkey_id[1] = (MMI_IMAGE_ID_T)IMAGE_NULL;
#endif
            }
            //SCI_MEMSET(item_data.item_content[0].item_data.text_buffer.wstr, 0x00, sizeof(item_data.item_content[0].item_data.text_buffer.wstr));
            MMIFM_GetChannelDispName(&channel, i, temp_wstr, GUILIST_STRING_MAX_NUM);

            item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data.item_content[0].item_data.text_buffer.wstr_ptr);
            item_t.user_data = i;
            GUILIST_AppendItem( ctrl_id, &item_t);
        }
    }
    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
        //get not exist channels 
        SCI_MEMSET(&channel, 0x00, sizeof(MMIFM_CHANNEL_T));
        SCI_MEMSET(temp_wstr, 0x00, sizeof(wchar)*(GUILIST_STRING_MAX_NUM + 1));
        if(!MMIFM_GetOneChannel(i, &channel) || !MMIFM_IsChannelValid(&channel))
        {
            //not exist channel
            if(MMIFM_OPERATION_LIST == ch_type)
            {
                item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
            }
            else
            {
                item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_SELECT;
            }
#ifndef MMI_GUI_STYLE_MINISCREEN 
                item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
#else           
                item_data.softkey_id[1] = (MMI_IMAGE_ID_T)IMAGE_NULL;
#endif
            //SCI_MEMSET(item_data.item_content[0].item_data.text_buffer.wstr, 0x00, sizeof(item_data.item_content[0].item_data.text_buffer.wstr));
            MMIFM_GetChannelDispName(&channel, i, temp_wstr, GUILIST_STRING_MAX_NUM);
            item_t.user_data = i;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data.item_content[0].item_data.text_buffer.wstr_ptr);
            GUILIST_AppendItem( ctrl_id, &item_t);
        }
    }
}

/*****************************************************************************/
//  Description : get channel's display name
//  Global resource dependence : 
//  Author:
//  Note: 
//  return: it will return "CHXX(NAME)"
/*****************************************************************************/
PUBLIC void MMIFM_GetChannelDispName(const MMIFM_CHANNEL_T *channel_ptr, uint16 channel_index, wchar *disp_name_ptr, uint16 max_name_len)
{
    uint8 disp_name[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
    uint8 freq_str[MMIFM_FREQ_MAX_LEN  +1] = {0};
    wchar wfreq_str[MMIFM_FREQ_MAX_LEN  +1] = {0};
    uint16 len = 0;
    MMI_STRING_T str_t = {0};
    BOOLEAN    channel_exist = FALSE;
    
    if(PNULL == channel_ptr || PNULL == disp_name_ptr || 0 == max_name_len)
    {
        //SCI_TRACE_LOW:"[MMIFM] GetChannelDispName channel_ptr %d disp_name_ptr %d max_name_len %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_3599_112_2_18_2_19_35_75,(uint8*)"ddd", channel_ptr, disp_name_ptr, max_name_len);
        return;
    }
    if(!MMIFM_IsChannelValid(channel_ptr))
    {
        channel_exist = FALSE;
    }
    else
    {
        channel_exist = TRUE;
    }
    //"CHXX("
    sprintf((char*)disp_name, "%s%d%s",MMIFM_CHANNEL_TITLE,channel_index + 1,"(");
    len = (uint16)strlen((char*)disp_name);
    if(len <= max_name_len)
    {
        MMI_STRNTOWSTR(disp_name_ptr, max_name_len, disp_name, max_name_len, len);
    }
    //"channel name"
    if(!channel_exist)
    {
        //channel not exist
        MMI_GetLabelTextByLang(TXT_BLANK, &str_t);
    }
    else
    {
        //draw channel name
        str_t.wstr_len = MMIAPICOM_Wstrlen(channel_ptr->name);
        if(str_t.wstr_len == 0)
        {
            //no name, display frequency text
            sprintf((char *)freq_str,"%d.%d",channel_ptr->freqency/10, channel_ptr->freqency%10);
            str_t.wstr_len = strlen((char*)freq_str);
            str_t.wstr_len = MIN(str_t.wstr_len, MMIFM_FREQ_MAX_LEN);
            str_t.wstr_ptr = wfreq_str;
            MMI_STRNTOWSTR(str_t.wstr_ptr, MMIFM_FREQ_MAX_LEN, freq_str, MMIFM_FREQ_MAX_LEN, str_t.wstr_len);
        }
        else
        {
            //display name 
            str_t.wstr_ptr = channel_ptr->name; //lint !e605
        }
    }
    if(str_t.wstr_len + len <= max_name_len)
    {
        MMIAPICOM_Wstrncat(disp_name_ptr, str_t.wstr_ptr, str_t.wstr_len);
    }
    else
    {
        MMIAPICOM_Wstrncat(disp_name_ptr, str_t.wstr_ptr, max_name_len - len);
    }
    //")"
    if(MMIAPICOM_Wstrlen(disp_name_ptr) < max_name_len)
    {
        len = MMIAPICOM_Wstrlen(disp_name_ptr);
        MMI_STRNTOWSTR(&disp_name_ptr[len], 1, (const uint8*)")", 1, 1);
    }
}

/*****************************************************************************/
//  Description : save currrent frequency 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void HandleSaveCurrentFreq(uint16 ch_index)
{
    MMIFM_CHANNEL_T channel = {0}; 
    MMI_WIN_ID_T    query_id = FM_QUERY_WIN_ID;
    MMI_STRING_T    str_t = {0};
    
    SetCurrentOperator(MMIFM_SAVE_FREQUENCY);
    SetCurrentSelChannel(ch_index);
#if defined(MMI_FM_MINI_SUPPORT) || defined( MMI_MINI_QVGA_UI_SUPPORT)
    /* fixed by langhua.yang for NEWMS00207297 */
    MMIFM_SetCurrrentChannel(ch_index);
#endif
    MMIFM_GetOneChannel(ch_index, &channel);
    if(MMIFM_IsChannelValid(&channel))
    {
        //not empty channel
#ifndef MMI_TASK_MANAGER
        MMIPUB_OpenQueryWinByTextId(TXT_FM_OVERWRITE_CHANNEL, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin);
#else
        MMIPUB_OpenQueryWinByTextIdEx(SPRD_FM_APPLET_ID,
            TXT_FM_OVERWRITE_CHANNEL, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin,
            PNULL);
#endif
    }
    else
    {
        str_t.wstr_len = MMIAPICOM_Wstrlen(channel.name);
        str_t.wstr_ptr = channel.name;
        HandleFMAction(GetCurrentOperator(), &str_t);
    }
    
}

/*****************************************************************************/
//  Description : close all of fm windows
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_ExitAllWindows(void)
{
    //if((uint32)(FM_MAX_WIN_ID -1) >= (uint32)(FM_WIN_ID_START + 1))
    {
        MMK_CloseMiddleWin(FM_WIN_ID_START + 1, FM_MAX_WIN_ID -1);
    }
}
#if 0
/*****************************************************************************/
//  Description : do action by touch pannel location
//  Global resource dependence : 
//  Parameter: point[IN]:touch pannel location
//  Note: 
/*****************************************************************************/
LOCAL void HandleButtonPress(MMI_WIN_ID_T win_id, uint32 button_id)
{
    switch(button_id) 
    {
    case MMIFM_LEFT_BUTTON_ID:
        //auto search next valid freqency
        MMK_SendMsg(win_id, MSG_APP_FM_ADJ_CHANNEL, (ADD_DATA)-1);
        break;
    case MMIFM_RIGHT_BUTTON_ID:
        //auto search prev valid freqency
        MMK_SendMsg(win_id, MSG_APP_FM_ADJ_CHANNEL, (ADD_DATA)1);
        break;
    case MMIFM_DOWN_BUTTON_ID:
        //skip to previous channel
        MMK_SendMsg(win_id, MSG_APP_FM_SEARCH_VALID_FREQ, (ADD_DATA)-1);
        break;
    case  MMIFM_UP_BUTTON_ID:
        //skip to next channel
        MMK_SendMsg(win_id, MSG_APP_FM_SEARCH_VALID_FREQ, (ADD_DATA)1);
        break;
    case MMIFM_ADJ_FREQ_RIGHT_BUTTON_ID:
        //increase freqency
        MMK_SendMsg(win_id, MSG_APP_FM_ADJ_FREQ, (ADD_DATA)1);
        break;
    case MMIFM_ADJ_FREQ_LEFT_BUTTON_ID:
        //Decrease freqency
        MMK_SendMsg(win_id, MSG_APP_FM_ADJ_FREQ, (ADD_DATA)-1);
        break;
    default:
        break;
    }
    return;        
}
#endif
#ifdef TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description : do action by touch pannel location
//  Global resource dependence : 
//  Parameter: point[IN]:touch pannel location
//  Note: 
/*****************************************************************************/
LOCAL void HandleTPAction(DPARAM param, MMI_WIN_ID_T win_id)
{
        if(NULL == param) return;
        switch(((MMI_NOTIFY_T*) param)->src_id)
        {
        case MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID:
        {
            OpenFMVolumePanel(MMICOM_PANEL_OPEN_NONE);
        }
        break;
        case MMIFM_ADJ_FREQ_LEFT_BUTTON_ID:
        {
            MMIFM_AdjustFrequency(MMIFM_GET_DOWN_VALUE_STEP);            
            SetButtonBgWhenResume();
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        case MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID:
        {
            //adjust channel, param is steps value
            if(MMIFM_GetValidChannelCount() > 1)
            {               
                MMIFM_AdjustChannel(MMIFM_GET_DOWN_VALUE_STEP);
                SetButtonBgWhenResume();
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
        case MMIFM_START_SUSPEND_BUTTON_CTRL_ID:
        {
            SetFMStartSuspendButtonStatus(win_id);
        }
        break;
        case MMIFM_ADJ_FREQ_RIGHT_BUTTON_ID:
        {
            MMIFM_AdjustFrequency(MMIFM_GET_UP_VALUE_STEP);
            SetButtonBgWhenResume();
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        case MMIFM_GET_UPFRE_BUTTON_CTRL_ID:
        {   
            //adjust channel, param is steps value
            if(MMIFM_GetValidChannelCount() > 1)
            {
                MMIFM_AdjustChannel(MMIFM_GET_UP_VALUE_STEP);
                SetButtonBgWhenResume();                
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
#ifdef MMI_PDA_SUPPORT
        case MMIFM_LIST_BUTTON_CTRL_ID:
            MMK_CreateWin((uint32 *)MMI_FM_CHANNEL_LIST_PDA_WIN_TAB, PNULL);
            break;
        case MMIFM_RETURN_BUTTON_CTRL_ID:
            MMK_PostMsg( win_id,  MSG_APP_CANCEL, 0 , 0 );
            break;
            
        case MMIFM_MENU_BUTTON_CTRL_ID:
            MMK_PostMsg( win_id,  MSG_APP_MENU, 0 , 0 );
            break;           
#endif
        default:
            break;
     }
    return;        
}
/*****************************************************************************/
//  Description : do action by touch pannel long press 
//  Global resource dependence : 
//  Parameter: point[IN]:touch pannel location
//  Note: 
/*****************************************************************************/
LOCAL void HandleLongTPAction(DPARAM param, MMI_WIN_ID_T win_id)
{
    if(NULL == param) return;
    switch(((MMI_NOTIFY_T*) param)->src_id)
    {
        case MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID:
        {             
            OpenAuotSearchWin(MMIFM_GET_ONE_VALID,MMIFM_GET_DOWN_VALUE_STEP);
            SetButtonBgWhenResume();
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        case MMIFM_GET_UPFRE_BUTTON_CTRL_ID:
        {   
            OpenAuotSearchWin(MMIFM_GET_ONE_VALID,MMIFM_GET_UP_VALUE_STEP);
            SetButtonBgWhenResume();
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        default:
            break;
    }
    return;        
}
#endif
/*****************************************************************************/
//  Description : create option menu items 
//  Global resource dependence : 
//  Parameter: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateMainOptMenu(uint32 ctr_id)
{
    MMI_STRING_T  kstring = {0};
    uint16        node_index =0;
    uint32         i = 0;
    uint32        item_num = 0;  
	//uint32        base_index = 0; 
    GUIMENU_DYNA_ITEM_T     node_item = {0};
#if defined(MMIFM_SUPPORT_FM_RECORD) || defined(MMIFM_SUPPORT_EQ_MODE)
    GUIMENU_POP_SEL_INFO_T  pop_info = {0};
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
    MMIFILE_DEVICE_E record_dev = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
#endif
#ifdef MMIFM_SUPPORT_EQ_MODE
    MMISRVAUD_EQ_MODE_E fm_eq_mode = MMISRVAUD_EQ_REGULAR;
#endif
#ifdef MMI_PDA_SUPPORT
    GUIMENU_BUTTON_INFO_T    button_info={0};
#endif
    item_num = ARR_SIZE(s_fm_main_opt_item); //sizeof(s_fm_main_opt_item)/sizeof(*s_fm_main_opt_item);
    
    for(i = 0; i < item_num; i++)
    {  

        if(MMIFM_PLAY_MODE == s_fm_main_opt_item[i][MMIFM_POP_MENU_NODE_ID] 
            && FM_SPEAKER_MODE == MMIFM_GetFMCurrentDeviceMode())
        {
            MMI_GetLabelTextByLang(TXT_FM_HEADSET_MODE, &kstring);
            //node_item.select_icon_id = s_fm_main_opt_item_img_id[i+1];
        }
        #ifdef FM_RDS_SUPPORT
        else if((MMIFM_RDS_ENABLE == s_fm_main_opt_item[i][MMIFM_POP_MENU_NODE_ID] )){
            if(FM_RDS_STATUS_OPEN == MMIFM_GetRdsStatus())
            {
                MMI_GetLabelTextByLang(TXT_FM_RDS_DISABLE, &kstring);         
            } else {
                MMI_GetLabelTextByLang(TXT_FM_RDS_ENABLE, &kstring); 
            } 
        }
        #endif 
        else
        {
            MMI_GetLabelTextByLang(s_fm_main_opt_item[i][MMIFM_POP_MENU_TEXT_ID], &kstring);
            //node_item.select_icon_id = s_fm_main_opt_item_img_id[i];
        }                
        node_item.item_text_ptr = &kstring;
        GUIMENU_InsertNode(node_index, s_fm_main_opt_item[i][MMIFM_POP_MENU_NODE_ID], 0, &node_item, ctr_id);
#ifdef MMIFM_SUPPORT_FM_RECORD
        if((FM_PLAYING != MMIAPIFM_GetStatus())
                &&(MMIFM_RECORD == s_fm_main_opt_item[i][MMIFM_POP_MENU_NODE_ID]))
        {
            GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
        }
#endif
#ifndef MMI_FM_NEED_HEADSET
        if(!GPIO_CheckHeadsetStatus()
                &&(MMIFM_PLAY_MODE == s_fm_main_opt_item[i][MMIFM_POP_MENU_NODE_ID]))
        {
            GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
        }
#endif
        node_index ++;     
    }
#ifdef MMIFM_SUPPORT_FM_RECORD
    record_dev = MMIFMRECORD_GetRecordStorage();
    if(!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(record_dev), MMIAPIFMM_GetDevicePathLen(record_dev)))
    {
        record_dev = MMIAPIFMM_GetDefaultDisk();
        MMIFMRECORD_SetRecordStorage(record_dev);
    }
    
    if(MMI_DEVICE_NUM > record_dev)
    {
		node_index = 0;
        for(device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
        {
			MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(device), &kstring);
			node_item.item_text_ptr = &kstring;
			GUIMENU_InsertNode(node_index, MMIFM_RECORD_FILE_UDISK + node_index, MMIFM_RECORD_FILE_LOCATION, &node_item, ctr_id);
            
			if(!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device)))
            {
				//灰化
				GUIMENU_SetNodeGrayed(TRUE, node_index, MMIFM_RECORD_FILE_LOCATION,  ctr_id);
            }  
			else
			{
				if(device == record_dev)
				{
					pop_info.node_id = MMIFM_RECORD_FILE_UDISK + node_index;
				}
			}
			node_index++;
        }
        
    }           
    pop_info.is_static = FALSE;    
    pop_info.ctrl_id = ctr_id;
    GUIMENU_SetPopItemStatus(TRUE, &pop_info);
    GUIMENU_SetSubMenuStyle(ctr_id, MMIFM_RECORD_FILE_LOCATION,  GUIMENU_STYLE_POPUP_RADIO);
#ifdef MMIFM_SUPPORT_FORMAT_AMR    
    item_num = ARR_SIZE(s_fm_main_sub_opt_item);
    for(node_index = 1;node_index<=item_num;node_index++)
    {
        MMI_GetLabelTextByLang(s_fm_main_sub_opt_item[node_index-1][MMIFM_POP_MENU_TEXT_ID], &kstring);
        node_item.item_text_ptr = &kstring;
		GUIMENU_InsertNode(node_index, MMIFM_RECORD_FILE_FORMAT + node_index, MMIFM_RECORD_FILE_FORMAT, &node_item, ctr_id);
    }

    if(MMISRVAUD_RECORD_FMT_AMR == MMIFMRECORD_GetFileFormat())
    {
        pop_info.node_id = MMIFM_RECORD_FORMAT_AMR;
    }
    else
    {
        pop_info.node_id = MMIFM_RECORD_FORMAT_WAV;
    }
    pop_info.is_static = FALSE;    
    pop_info.ctrl_id = ctr_id;
    GUIMENU_SetPopItemStatus(TRUE, &pop_info);
    GUIMENU_SetSubMenuStyle(ctr_id, MMIFM_RECORD_FILE_FORMAT,  GUIMENU_STYLE_POPUP_RADIO);
#endif    
#endif
#ifdef MMIFM_SUPPORT_EQ_MODE
    fm_eq_mode = MMIFM_GetEqMode();
    item_num = ARR_SIZE(s_fm_eq_mode_opt_item );
    for(node_index = 1; node_index <= item_num; node_index++)
    {
        MMI_GetLabelTextByLang(s_fm_eq_mode_opt_item[node_index - 1][MMIFM_POP_MENU_TEXT_ID], &kstring);
        node_item.item_text_ptr = &kstring;
		GUIMENU_InsertNode(node_index, MMIFM_SET_EQ_MODE + node_index, MMIFM_SET_EQ_MODE, &node_item, ctr_id);
    }
    if(fm_eq_mode < (MMIFM_EQ_MAX - MMIFM_EQ_REGULAR))
    {
      pop_info.node_id = s_fm_eq_mode_opt_item[fm_eq_mode][MMIFM_POP_MENU_NODE_ID];
    }
    else
    {
      pop_info.node_id = MMIFM_EQ_REGULAR;
    }
    pop_info.is_static = FALSE;    
    pop_info.ctrl_id = ctr_id;
    GUIMENU_SetPopItemStatus(TRUE, &pop_info);
    GUIMENU_SetSubMenuStyle(ctr_id, MMIFM_SET_EQ_MODE,  GUIMENU_STYLE_POPUP_RADIO);
#endif
#ifdef MMI_PDA_SUPPORT
    button_info.is_static=FALSE;
    button_info.node_id=MMIFM_RECORD_FILE_LOCATION;
    button_info.button_style=GUIMENU_BUTTON_STYLE_CANCEL;
    GUIMENU_SetButtonStyle(ctr_id,&button_info);
#ifdef MMIFM_SUPPORT_FORMAT_AMR     
    button_info.node_id=MMIFM_RECORD_FILE_FORMAT;
    GUIMENU_SetButtonStyle(ctr_id,&button_info);
#endif
#endif
    
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : create option menu items 
//  Global resource dependence : 
//  Parameter: 
//  Note: 

/*****************************************************************************/
LOCAL void CreateChannelMenuOptionMenu(uint32 ctr_id)
{
    MMI_STRING_T  kstring = {0};
    uint16        node_index =0;
    //uint32         i = 0;
    //uint32        item_num = 0;  
    GUIMENU_DYNA_ITEM_T     node_item = {0};    
    
    MMI_GetLabelTextByLang(TXT_DELALL, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(node_index, MMIFM_CHANNEL_DELETE_ALL, 0, &node_item, ctr_id);
   
   
    if(MMIFM_GetValidChannelCount() == 0)
    {
      GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
    }  
}
#endif
/*****************************************************************************/
//  Description : create option menu items 
//  Global resource dependence : 
//  Parameter: 
//  Note: 

/*****************************************************************************/
LOCAL void CreateChannelOptionMenu(uint32 ctr_id)
{
    MMI_STRING_T  kstring = {0};
    uint16        node_index =0;
    uint32         i = 0;
    uint32        item_num = 0;  
    GUIMENU_DYNA_ITEM_T     node_item = {0};
#ifdef MMI_PDA_SUPPORT 
    GUIMENU_BUTTON_INFO_T    button_info={0};
    GUIMENU_TITLE_INFO_T      title_info={0};
    MMIFM_CHANNEL_T channel={0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM+1]={0};
    //uint16 temp_wstr_len=0;
    MMI_STRING_T title={0};
#endif
    item_num = ARR_SIZE(s_fm_channel_opt_item); //sizeof(s_fm_channel_opt_item)/sizeof(*s_fm_channel_opt_item);
    
    for(i = 0; i < item_num; i++)
    {
        MMI_GetLabelTextByLang(s_fm_channel_opt_item[i][MMIFM_POP_MENU_TEXT_ID], &kstring);
        node_item.item_text_ptr = &kstring;
        GUIMENU_InsertNode(node_index, s_fm_channel_opt_item[i][MMIFM_POP_MENU_NODE_ID], 0, &node_item, ctr_id);
#ifdef MMI_PDA_SUPPORT        
        button_info.is_static=FALSE;
        button_info.node_id=0;
        button_info.button_style=GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctr_id,&button_info);
#endif
        if(!MMIFM_IsChannelExist(GetCurrentSelChannel()))
        {
            if(MMIFM_CHANNEL_DELETE == s_fm_channel_opt_item[i][MMIFM_POP_MENU_NODE_ID])
            {
                //empty channel can not delete
                GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
            }
#ifdef MMI_PDA_SUPPORT
#else
            if(MMIFM_CHANNEL_PLAY == s_fm_channel_opt_item[i][MMIFM_POP_MENU_NODE_ID])
            {
                GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
            }
#endif
        }
#ifdef MMI_PDA_SUPPORT
#else
        if(MMIFM_GetValidChannelCount() == 0)
        {
            //no channel, need not to delete
            if(MMIFM_CHANNEL_DELETE_ALL == s_fm_channel_opt_item[i][MMIFM_POP_MENU_NODE_ID])
            {
                GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
            }
        }
#endif 
        node_index ++;
    }
#ifdef MMI_PDA_SUPPORT
    MMIFM_GetOneChannel(GetCurrentSelChannel(), &channel);
    MMIFM_GetChannelDispName(&channel, GetCurrentSelChannel(), temp_wstr, GUILIST_STRING_MAX_NUM);
    title_info.node_id=0;
    title_info.is_static=FALSE;
    title.wstr_ptr=temp_wstr;
    title.wstr_len=MMIAPICOM_Wstrlen(temp_wstr);
    title_info.title_ptr=&title;
    GUIMENU_SetMenuTitleEx(ctr_id,&title_info);
#endif
    
}



/*****************************************************************************/
// 	Description : open auto searching waiting window 
//	Global resource dependence : 
//  Parameter: 
//	Note: 
/*****************************************************************************/
LOCAL void OpenAuotSearchWin(MMIFM_SEARCH_FREQUENCY_TYPE_E e_search_type,int32 direct)
{
    MMI_STRING_T	prompt_str = {0};
    MMIPUB_HANDLE_FUNC  func   = (MMIPUB_HANDLE_FUNC)HandleAutoSearchWinMsg;
    
    if(MMIFM_CheckHeadsetStatus())
    {
        MMI_GetLabelTextByLang(TXT_FM_SEARCH_CHANNEL, &prompt_str);
        if(MMIFM_GET_ONE_VALID == e_search_type)
        { 
            func = (MMIPUB_HANDLE_FUNC )HandleSearchValidFreqWinMsg;
        }    
#ifndef MMI_TASK_MANAGER
        MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,FM_AUTO_SEARCH_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,func);
#else
        MMIPUB_OpenWaitWinEx(SPRD_FM_APPLET_ID,
            1,&prompt_str,PNULL,PNULL,FM_AUTO_SEARCH_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,func,
            PNULL);
#endif
        if(MMIFM_GET_ONE_VALID == e_search_type)
        {
            MMK_SendMsg(FM_AUTO_SEARCH_WIN_ID, MSG_APP_FM_GETSEARCH_PARAM, (ADD_DATA)direct);
        }
    }
}

/*****************************************************************************/
//  Discription: Handle auto searching window msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAutoSearchWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param 
                                           )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE; 
    static uint16 channel_index = 0;
    static uint8  s_ani_timer_id = 0;//MMK_CreateTimer(MMIFM_ANI_TIME, FALSE);
    static uint16 s_search_ret  = 0;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIFM_EnableMute();
        MMIFM_SetStatus(FM_STOP);
#ifdef MMI_PDA_SUPPORT
        MMIPUB_SetWinSoftkey(win_id,  TXT_NULL,  STXT_CANCEL, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif
        MMIFM_DeleteAllChannels();
        //GUISOFTKEY_SetTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        s_ani_timer_id = MMK_CreateTimer(MMIFM_ANI_TIME, FALSE);
        // just create a timer, let's MSG_TIMER do auto search
        channel_index = 0;        
        #if 0
        s_search_ret = MMIFM_AuotSearchFreq(channel_index);
        if(MMIFM_AUTOSEARCH_FAIL == s_search_ret)
        {
            //search fail
            MMK_SendMsg(win_id, MSG_APP_FM_FAIL, PNULL);
        }
        #endif
        break;
    case MSG_TIMER:
        // 自动搜台功能
        if (s_ani_timer_id == *((uint8 *) param))
        {
            s_search_ret = MMIFM_AuotSearchFreq(channel_index);
            if(MMIFM_AUTOSEARCH_END == s_search_ret)
            {
                MMK_CloseWin(win_id);
                MMK_SendMsg(FM_MAIN_WIN_ID, MSG_APP_FM_AUTOSEARCH_END,PNULL);
            }
            else if(MMIFM_AUTOSEARCH_FAIL == s_search_ret)
            {
                //search fail
                MMK_SendMsg(win_id, MSG_APP_FM_FAIL, PNULL);
            }
            else if(MMIFM_AUTOSEARCH_CONTINUE == s_search_ret)
            {  //search next agin
               channel_index++;
               s_ani_timer_id = MMK_CreateTimer(MMIFM_ANI_TIME, FALSE);
            }
            else
            {
                //search next agin
               // MMIFM_AUTOSEARCH_NOT_COMPLETE
               // search not complete, just start a timer to serch it
                s_ani_timer_id = MMK_CreateTimer(MMIFM_ANI_TIME, FALSE);
            }
        }
        
        break;
        
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
        break;
        
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"HandleAutoSearchWinMsg  MSG_APP_CANCEL channel_index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_4118_112_2_18_2_19_36_76,(uint8*)"d", channel_index);
        //MMK_SendMsg(FM_MAIN_WIN_ID, MSG_APP_FM_AUTOSEARCH_END,PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_FM_FAIL:
        //search fail
        MMIPUB_OpenAlertWarningWin(TXT_FM_NO_CHANNEL);
        MMK_SendMsg(FM_MAIN_WIN_ID, MSG_APP_FM_AUTOSEARCH_END,PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_ani_timer_id);                          
        //SCI_TRACE_LOW:"[MMIFM]HandleAutoSearchWinMsg channel_index %d, s_search_ret %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_4131_112_2_18_2_19_36_77,(uint8*)"dd", channel_index, s_search_ret);
        // if MMIFM_AUTOSEARCH_NOT_COMPLETE,
        // need re-open FM to stop serching job in FM chip
        if(MMIFM_AUTOSEARCH_NOT_COMPLETE == s_search_ret )
        {
            FM_Close();
            FM_Init();
            //MMK_SendMsg(FM_MAIN_WIN_ID, MSG_APP_FM_AUTOSEARCH_END,PNULL);
        }
        //search end
        //MMIFM_DisableMute();
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        channel_index = 0;
        s_ani_timer_id = 0;//MMK_CreateTimer(MMIFM_ANI_TIME, FALSE);
        s_search_ret  = 0;
	   // MMK_PostMsg(FM_MAIN_WIN_ID, MSG_APP_FM_AUTOSEARCH_END, PNULL, 0);
		MMK_SendMsg(FM_MAIN_WIN_ID, MSG_APP_FM_AUTOSEARCH_END, PNULL);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);;
        break;
    }
    
    return result;        
}
/*****************************************************************************/
//  Discription: Handle get one valid frequency window msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSearchValidFreqWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param 
                                           )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE; 
    static uint8  s_ani_timer_id = 0;
    static uint16 s_search_ret  = 0;
    static int32 direct = FM_SCAN_UP;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:        
        MMIFM_EnableMute();
#ifdef MMI_PDA_SUPPORT
        MMIPUB_SetWinSoftkey(win_id,  TXT_NULL,  STXT_CANCEL, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif
        s_ani_timer_id = MMK_CreateTimer(MMIFM_ANI_TIME, FALSE);
        break;
        
    case MSG_APP_FM_GETSEARCH_PARAM:
        direct = (int)param;
        break;
        
    case MSG_TIMER:
        //搜台功能        
        if (s_ani_timer_id == *((uint8 *) param))
        {
            s_search_ret = MMIFM_SearchValidFreq(direct);
            
            if(MMIFM_AUTOSEARCH_END == s_search_ret)
            { 
                MMK_CloseWin(win_id);
            }
            else if(MMIFM_AUTOSEARCH_FAIL == s_search_ret)
            {
                //search fail                
                MMK_SendMsg(win_id, MSG_APP_FM_FAIL, PNULL);
            } 
            else 
            {  
               // search not complete, just start a timer to serch it
               s_ani_timer_id = MMK_CreateTimer(MMIFM_ANI_TIME, FALSE);
               
            }
        }        
        break;

    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
        break;
        
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"HandleSearchValidFreqWinMsg  MSG_APP_CANCEL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_4210_112_2_18_2_19_36_78,(uint8*)"");
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_FM_FAIL:
        //search fail
        MMIPUB_OpenAlertWarningWin(TXT_FM_NO_CHANNEL);
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_ani_timer_id);                          
        //SCI_TRACE_LOW:"[MMIFM]HandleSearchValidFreqWinMsg s_search_ret %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_4220_112_2_18_2_19_36_79,(uint8*)"d",s_search_ret);
        // need re-open FM to stop serching job in FM chip
        if(MMIFM_AUTOSEARCH_NOT_COMPLETE == s_search_ret )            
        {             
            FM_Close();            
            FM_Init();
        }
        //search end
        //MMIFM_DisableMute(); 
        
        // after seek, play it no matter it is valid or not
        MMIFM_Play(FALSE);
        MMIFM_DisableMute();
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        s_ani_timer_id = 0;
        s_search_ret  = 0;
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);;
        break;
    }
    
    return result;        
}
#ifdef FM_NEW_UI
/*****************************************************************************/
//  Discription: draw fm radio's main pannel
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayMainFre(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO* lcd_dev_info,uint16 frequency)
{
    MMIFM_CUR_INFO_T *cur_info_ptr = MMIFM_GetFMCurrentInfo();
    MMIFM_START_WAY_E    start_way = NORMAL_START_FM;
    uint16 fre=0;
    
    if(PNULL == cur_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFM] DrawFrqqencyText PNULL == cur_info_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_4258_112_2_18_2_19_36_80,(uint8*)"");
        return;
    } 
    start_way = (uint32)MMK_GetWinAddDataPtr(win_id);
    if(PREVIEW_START_FM == start_way)
    {
        fre=cur_info_ptr->export_freq;
    }
    else
    {
        fre=frequency;
    }
    drawFre( win_id, lcd_dev_info, fre, 0);
}
#else
/*****************************************************************************/
//  Discription: draw fm radio's main pannel
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayMainWindow(MMI_WIN_ID_T win_id, uint32 items_need_draw)
{
#ifdef MMI_FM_MINI_SUPPORT
    /*add by langhua.yang for MINI UI*/
    DisplayFMBg(win_id);
#else
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetStatusBarBothRect();
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    GUI_RECT_T fm_bg_display_rect = {0};
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    fm_bg_display_rect.left   = 0;
    fm_bg_display_rect.right  = fm_bg_display_rect.left + lcd_width;
    fm_bg_display_rect.bottom = fm_bg_display_rect.top + lcd_height;
    if (MMITHEME_IsMainScreenLandscape())
    {        
        fm_bg_display_rect.top    = both_rect.h_rect.bottom;
    }
    else
    {
        fm_bg_display_rect.top    = both_rect.v_rect.bottom;
    }
    
    GUIRES_DisplayImg(PNULL,
            &fm_bg_display_rect,
            PNULL,
            win_id,
            IMAGE_FM_BG,
            &lcd_dev_info);
#endif			
    GUIANIM_SetOwnerDraw(MMIFM_FREQUENCY_BAR_CTRL_ID,DisplayFMFrePic);
#ifdef FM_NEW_UI    
    //draw channel name
    if(items_need_draw & MMIFM_DRAW_CHANNEL_NAME)
    {
        DrawChannelName(win_id);
    }
#endif    
    //draw frequency text
    if(items_need_draw & MMIFM_DRAW_FREQ_TEXT)
    {    
#ifdef MMI_FM_MINI_SUPPORT
        /*add by langhua.yang for MINI UI*/
        GUILABEL_FlushBg(MMIFM_FREQUENCY_TEXT_CTRL_ID);
#endif
        DrawFrqqencyText(win_id);
    }
	DrawStatusButton(win_id);
    #ifdef FM_RDS_SUPPORT
    if(FM_RDS_STATUS_OPEN == MMIFM_GetRdsStatus()){
        MMIFM_ReadRDS();
    } else {
        SetRDSText(PNULL);
    }
    #endif
}
#endif
#ifdef FM_NEW_UI
/*****************************************************************************/
//  Discription: draw channel's name
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DrawChannelName(MMI_WIN_ID_T win_id)
{
    MMIFM_CUR_INFO_T  *cur_info_ptr = MMIFM_GetFMCurrentInfo();    
    if(PNULL == cur_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFM] DrawChannelName PNULL == cur_info_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_4307_112_2_18_2_19_36_81,(uint8*)"");
        return;
    } 
    
    DrawChannelNameByFreq(win_id,cur_info_ptr->cur_freq);
}
/*****************************************************************************/
//  Discription: draw channel's name by freq
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DrawChannelNameByFreq(MMI_WIN_ID_T win_id,uint16 frequency)
{
    MMIFM_CHANNEL_T   channel_info = {0};  
    MMI_STRING_T      default_str = {0};
    MMI_STRING_T      disp_string = {0};
    uint16            channel_name_len = 0;
    wchar             disp_name[MMIFM_CHANNEL_NAME_LEN +MMIFM_CHANNEL_TITLE_ADD_MAX_LEN +1] = {0};
    MMIFM_START_WAY_E    start_way = NORMAL_START_FM;
    uint16 ch_index = MMIFM_INVALID_CHANNEL;
    
    start_way = (uint32)MMK_GetWinAddDataPtr(win_id); 
    ch_index = MMIFM_GetChannelIndexByFreq(frequency);
    if(MMIFM_INVALID_CHANNEL != ch_index)
    {     
        if(MMIFM_GetOneChannel(ch_index, &channel_info))
        {
            if(channel_info.freqency == frequency)
            {
                //now play one channel, so channel name must be display            
                MMIFM_GetChannelDispName(&channel_info, ch_index, disp_name, MMIFM_CHANNEL_NAME_LEN +MMIFM_CHANNEL_TITLE_ADD_MAX_LEN);
                channel_name_len = MMIAPICOM_Wstrlen(disp_name);            
            }
        }
    }
    if(channel_name_len == 0 ||PREVIEW_START_FM == start_way)
    {
        //show "FM Radio"
        MMI_GetLabelTextByLang(TXT_FM, &default_str);
        channel_name_len = MIN(default_str.wstr_len, MMIFM_CHANNEL_NAME_LEN +MMIFM_CHANNEL_TITLE_ADD_MAX_LEN);
        MMIAPICOM_Wstrncpy(disp_name, default_str.wstr_ptr, channel_name_len);
        
    }
    disp_string.wstr_len = channel_name_len;
    disp_string.wstr_ptr = disp_name;
    GUILABEL_SetFont(FM_CHANNEL_NAME_TEXT_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMIFM_FONT_CLOLOR);
    GUILABEL_SetText(FM_CHANNEL_NAME_TEXT_CTRL_ID, &disp_string, TRUE);
}

#endif
/*****************************************************************************/
//  Discription: draw channel's NO.
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DrawChannelNumber(const wchar *num_str_ptr, MMI_WIN_ID_T win_id)
{
    MMI_STRING_T      disp_string = {0};
    GUI_RECT_T       rect ={MMIFM_CHANNEL_TEXT_X, MMIFM_CHANNEL_TEXT_Y, MMI_MAINSCREEN_WIDTH - (MMIFM_CHANNEL_TEXT_X *2), 0};
    uint16           channel_name_len = 0;
    
    if(PNULL == num_str_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFM] DrawChannelNumber PNULL == num_str_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_4370_112_2_18_2_19_36_82,(uint8*)"");
        return;
    }
    channel_name_len = MMIAPICOM_Wstrlen(num_str_ptr);
    rect.bottom = rect.top + GUI_GetStringHeight(MMI_DEFAULT_TEXT_FONT, num_str_ptr, channel_name_len) + 10;//GUI_CalculateStringPiexlNum(channel_info.name, channel_name_len, FALSE, SONG_FONT_20, 0);
    //draw window bg
    disp_string.wstr_len = channel_name_len;
    disp_string.wstr_ptr = num_str_ptr; //lint !e605
#ifndef MMI_PDA_SUPPORT
    GUILABEL_SetFont(FM_CHANNEL_NAME_TEXT_CTRL_ID, MMI_DEFAULT_TEXT_FONT, MMIFM_FONT_CLOLOR);
#endif
    GUILABEL_SetText(FM_CHANNEL_NAME_TEXT_CTRL_ID, &disp_string, TRUE);
}

/*****************************************************************************/
//  Discription: draw freqency string text
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DrawFrqqencyText(MMI_WIN_ID_T win_id)
{
    MMIFM_CUR_INFO_T *cur_info_ptr = MMIFM_GetFMCurrentInfo();
    uint16           freq_len = 0;
    wchar            wfreq_number[MMIFM_FREQ_MAX_LEN + 1] = {0};
    uint8            freq_number[MMIFM_FREQ_MAX_LEN + 1] = {0};
    MMI_STRING_T        string = {0};/*lint !e64*/
    int32 str_len = 0;
    MMIFM_START_WAY_E    start_way = NORMAL_START_FM;
    
    if(PNULL == cur_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIFM] DrawFrqqencyText PNULL == cur_info_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_4401_112_2_18_2_19_36_83,(uint8*)"");
        return;
    } 
    start_way = (uint32)MMK_GetWinAddDataPtr(win_id);
    if(PREVIEW_START_FM == start_way)
    {
        sprintf((char *)freq_number,"%d.%d",cur_info_ptr->export_freq/10, cur_info_ptr->export_freq%10);
    }
    else
    {
        sprintf((char *)freq_number,"%d.%d",cur_info_ptr->cur_freq/10, cur_info_ptr->cur_freq%10);
    }    
    str_len = strlen((char*)freq_number);
    str_len = MIN(MMIFM_FREQ_MAX_LEN, str_len);
    MMI_STRNTOWSTR(wfreq_number, MMIFM_FREQ_MAX_LEN, freq_number, MMIFM_FREQ_MAX_LEN, str_len);
    MMIAPICOM_Wstrcat(wfreq_number, L"MHz");
    freq_len = MMIAPICOM_Wstrlen(wfreq_number);
    if(freq_len > 0)
    {           
        string.wstr_len = freq_len;
        string.wstr_ptr = wfreq_number;        
        GUILABEL_SetFont(MMIFM_FREQUENCY_TEXT_CTRL_ID, MMIFM_DEFAULT_BIG_FONT, MMIFM_FONT_CLOLOR);
        GUILABEL_SetText(MMIFM_FREQUENCY_TEXT_CTRL_ID,&string,FALSE);
    }            
}
#if 0
/*****************************************************************************/
//  Description : draw fm volume images.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void DisplayVolumeImage(
                              MMI_WIN_ID_T      win_id
                              )
{
#if 0   
    MMI_IMAGE_ID_T   volume_image   = IMAGE_COMMON_VOL_0;
    GUI_LCD_DEV_INFO lcd_dev_info   = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T       inc_rect       = MMIFM_VOLUME_INCREASE_RECT;
    GUI_RECT_T       dec_rect       = MMIFM_VOLUME_DECREASE_RECT;
    MMI_IMAGE_ID_T   inc_image      = IMAGE_COMMON_VOL_INC;//(inc_btn_pressed)?IMAGE_COMMON_VOL_INC_FOCUS:IMAGE_COMMON_VOL_INC;
    MMI_IMAGE_ID_T   dec_image      = IMAGE_COMMON_VOL_DEC;//(dec_btn_pressed)?IMAGE_COMMON_VOL_DEC_FOCUS:IMAGE_COMMON_VOL_DEC;
    uint8            volume         = 0;
    MMIFM_CUR_INFO_T *cur_info_ptr  =  MMIFM_GetFMCurrentInfo();
    GUI_POINT_T         dis_point = {0};
    
    if(!MMK_IsFocusWin(win_id))
    {
        return;
    }   

    volume = MMIAPISET_GetMultimVolume();
    volume_image = IMAGE_COMMON_VOL_0 + volume - MMISET_VOL_ZERO;

    dis_point.x = MMIFM_VOLUME_LEFT;
    dis_point.y = MMIFM_VOLUME_TOP;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        volume_image,
        &lcd_dev_info);
    
    dis_point.x = inc_rect.left;
    dis_point.y = inc_rect.top;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        inc_image,
        &lcd_dev_info);

    dis_point.x = dec_rect.left;
    dis_point.y = dec_rect.top;
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        dec_image,
        &lcd_dev_info);
#endif
}
#endif
#if 0
/*****************************************************************************/
//  Discription: Handle headset plug out or in  win msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleHeadsetPlugOutWinMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param 
                                               )
{
    
    LOCAL BOOLEAN is_exit_fm = TRUE; // 判断是否需要退出FM
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_exit_fm = TRUE;
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;  
    case MSG_APP_FM_HEADSET_PLUG_OUT:
        is_exit_fm = TRUE;
        break;      
    case MSG_APP_FM_HEADSET_PLUG_IN:
        is_exit_fm = FALSE;
        break;
    case MSG_CLOSE_WINDOW:
        if(is_exit_fm)
        {
            MMIFM_Exit(FALSE);            
        }        
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return recode;
}
#endif
#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetDigitalImg(char c)
{
    MMI_IMAGE_ID_T image_id[]={
        IMAGE_FM_NUMBER_0,
        IMAGE_FM_NUMBER_1,
        IMAGE_FM_NUMBER_2,
        IMAGE_FM_NUMBER_3,
        IMAGE_FM_NUMBER_4,
        IMAGE_FM_NUMBER_5,
        IMAGE_FM_NUMBER_6,
        IMAGE_FM_NUMBER_7,
        IMAGE_FM_NUMBER_8,
        IMAGE_FM_NUMBER_9,
        IMAGE_FM_NUMBER_POINT
        };

    if(c=='.')
    {
        return image_id[10];
    }
    else
    {
        return image_id[c-'0'];
    }
}

#if 0

/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  drawSlideOnly(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *dev_info_ptr,uint16 x_step)
{
    GUI_POINT_T point={0};
    GUI_RECT_T rect={0};
    //GUI_RECT_T image_rect={0};
    uint16 width=0;
    uint16 height=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    //int i=0;
    //MMI_IMAGE_ID_T img_id=0;
    //uint16 img_right=0;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    

    if (MMITHEME_IsMainScreenLandscape())
    {
        GUIRES_GetImgWidthHeight(&width, &height, FM_PDA_SLIDE_LINE, win_id);
        point.x=FM_H_BUTTON_START_X+(lcd_width-FM_H_BUTTON_START_X-(width-FM_ONE_SLIDE_WIDTH))/2;
        point.y=FM_H_SLIDE_START_Y+FM_H_SLIDE_INTERVAL;
        rect.left=x_step;
        rect.right=rect.left+width-FM_ONE_SLIDE_WIDTH;
        rect.top=0;
        rect.bottom=rect.top+height;
        GUIRES_DisplayImg(&point,
            PNULL,
            &rect,
            win_id,
            FM_PDA_SLIDE_LINE,
            dev_info_ptr);

        point.x=FM_H_BUTTON_START_X;
        point.y=FM_H_SLIDE_START_Y;
        GUIRES_DisplayImg(&point,
            PNULL,
            PNULL,
            win_id,
            FM_PDA_SLIDE_SHADOW,
            dev_info_ptr);
    }
    else
    {

        GUIRES_GetImgWidthHeight(&width, &height, FM_PDA_SLIDE_LINE, win_id);
        point.x=(lcd_width-(width-FM_ONE_SLIDE_WIDTH))/2;
        point.y=FM_V_SLIDE_START_Y+FM_V_SLIDE_INTERVAL;
        rect.left=x_step;
        rect.right=rect.left+width-FM_ONE_SLIDE_WIDTH;
        rect.top=0;
        rect.bottom=rect.top+height;
        GUIRES_DisplayImg(&point,
            PNULL,
            &rect,
            win_id,
            FM_PDA_SLIDE_LINE,
            dev_info_ptr);

        GUIRES_GetImgWidthHeight(&width, &height, FM_PDA_SLIDE_SHADOW, win_id);
        point.x=(lcd_width-width)/2;
        point.y=FM_V_SLIDE_START_Y;
        GUIRES_DisplayImg(&point,
            PNULL,
            PNULL,
            win_id,
            FM_PDA_SLIDE_SHADOW,
            dev_info_ptr);
    }

}


/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  drawFreOnly(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *dev_info_ptr,uint16 fre)
{
    GUI_BOTH_RECT_T         both_rect={0};
    GUI_POINT_T point={0};
    //GUI_RECT_T rect={0};
    //GUI_RECT_T image_rect={0};
    uint16 width=0;
    uint16 height=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    //uint16           freq_len = 0;
    char            freq_number[MMIFM_FREQ_MAX_LEN + 1] = {0};
    int32 str_len = 0;
    int i=0;
    MMI_IMAGE_ID_T img_id=0;
    uint16 img_right=0;
    
    both_rect=MMITHEME_GetStatusBarBothRect();
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    
    sprintf(freq_number,"%d.%d",fre/10, fre%10);
    str_len = strlen((char*)freq_number);
    str_len = MIN(MMIFM_FREQ_MAX_LEN, str_len);

    if (MMITHEME_IsMainScreenLandscape())
    {
        point.y=FM_H_FRE_DIGITAL_Y;
        img_right=FM_H_FRE_DIGITAL_RIGHT;
        for(i=str_len-1;i>=0;i--)
        {
            img_id=GetDigitalImg(freq_number[i]);
            GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
            img_right-=width;
            point.x=img_right;
            GUIRES_DisplayImg(&point,
            PNULL,
            PNULL,
            win_id,
            img_id,
            dev_info_ptr);
        }

        if(MMITHEME_IsIstyle())
        {
            img_id = FM_PDA_IPHONE_ADJ_BUT;
            point.y=both_rect.h_rect.bottom + FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_H;
        }
        else
        {
            img_id = FM_PDA_ADJ_BUT;
            point.y=both_rect.h_rect.bottom + 1;
        }        
        
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
        point.x=(fre-MMIFM_FREQ_RANGE_LOW)*(FM_H_BUTTON_START_X-width)/(MMIFM_FREQ_RANGE_HIGH-MMIFM_FREQ_RANGE_LOW);
        GUIRES_DisplayImg(&point,
            PNULL,
            PNULL,
            win_id,
            img_id,
            dev_info_ptr);
    }
    else
    {
        point.y=FM_V_FRE_DIGITAL_Y;
        img_right=FM_V_FRE_DIGITAL_RIGHT;
        for(i=str_len-1;i>=0;i--)
        {
            img_id=GetDigitalImg(freq_number[i]);
            GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
            img_right-=width;
            point.x=img_right;
            GUIRES_DisplayImg(&point,
            PNULL,
            PNULL,
            win_id,
            img_id,
            dev_info_ptr);
        }
        
        if(MMITHEME_IsIstyle())
        {
            img_id = FM_PDA_IPHONE_ADJ_BUT;
            point.y=both_rect.v_rect.bottom + FM_FREQUECY_ADJUST_LINE_Y_OFFSET_FROM_STATUSBAR_V;
        }
        else
        {
            img_id = FM_PDA_ADJ_BUT;
            point.y=both_rect.v_rect.bottom + 1;
        }
        
        GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
        point.x=(fre-MMIFM_FREQ_RANGE_LOW)*(lcd_width-width)/(MMIFM_FREQ_RANGE_HIGH-MMIFM_FREQ_RANGE_LOW);
        GUIRES_DisplayImg(&point,
            PNULL,
            PNULL,
            win_id,
            img_id,
            dev_info_ptr);
    }

}
#endif

#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  drawFre(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *dev_info_ptr,uint16 fre,uint16 x_step)
{
    GUI_POINT_T point={0};
    //GUI_RECT_T rect={0};
    uint16 width=0;
    uint16 height=0;
    uint16 mzh_width=0;
    uint16 mzh_height=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    char            freq_number[MMIFM_FREQ_MAX_LEN + 1] = {0};
    int32 str_len = 0;
    int i=0;
    MMI_IMAGE_ID_T img_id=0;
    uint16 img_left=0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
        
    sprintf(freq_number,"%d.%d",fre/10, fre%10);
    str_len = strlen((char*)freq_number);
    str_len = MIN(MMIFM_FREQ_MAX_LEN, str_len);
    if(UILAYER_GetHandleType(dev_info_ptr) !=UILAYER_HANDLE_MAIN)
    {
    UILAYER_Clear(dev_info_ptr);
    }
    if (MMITHEME_IsMainScreenLandscape())
    {
        GUIRES_GetImgWidthHeight(&mzh_width, &mzh_height, IMAGE_FM_MHZ, win_id);
        point.x=FM_H_FRQ_MHZ_START_X;
        point.y=FM_H_FRQ_MHZ_BOTTOM_Y - mzh_height;
        GUIRES_DisplayImg(&point,
        PNULL,
        PNULL,
        win_id,
        IMAGE_FM_MHZ,
        dev_info_ptr);
        
        img_left=FM_H_FRQ_MHZ_START_X + mzh_width + FM_H_FRQ_DATA_SPACE;
        for(i=str_len-1;i>=0;i--)
        {
            img_id=GetDigitalImg(freq_number[i]);
            GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
            img_left = img_left- FM_H_FRQ_DATA_SPACE - width;
            point.y = FM_H_FRQ_DATA_BOTTOM_Y - height;
            point.x=img_left;
            GUIRES_DisplayImg(&point,
            PNULL,
            PNULL,
            win_id,
            img_id,
            dev_info_ptr);
            
        }           
    }
    else
    {
        GUIRES_GetImgWidthHeight(&mzh_width, &mzh_height, IMAGE_FM_MHZ, win_id);
        point.x=FM_V_FRQ_MHZ_START_X;
        point.y=FM_V_FRQ_MHZ_BOTTOM_Y - mzh_height;
        GUIRES_DisplayImg(&point,
        PNULL,
        PNULL,
        win_id,
        IMAGE_FM_MHZ,
        dev_info_ptr);
        
        img_left=FM_V_FRQ_MHZ_START_X;
        for(i=str_len-1;i>=0;i--)
        {
            img_id=GetDigitalImg(freq_number[i]);
            GUIRES_GetImgWidthHeight(&width, &height, img_id, win_id);
            img_left = img_left - FM_V_FRQ_DATA_SPACE - width;
            point.y = FM_V_FRQ_MHZ_BOTTOM_Y - height;
            point.x = img_left;
            GUIRES_DisplayImg(&point,
            PNULL,
            PNULL,
            win_id,
            img_id,
            dev_info_ptr);
        }          
    }
}


/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  drawBg(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *dev_info_ptr)
{
    GUI_BOTH_RECT_T         both_rect={0};
    uint16 height=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    GUI_RECT_T img_display_rect = {0};

    GUIRES_GetImgWidthHeight(PNULL, &height, IMAGE_FM_NUMBER_BG, win_id);
    both_rect=MMITHEME_GetStatusBarBothRect();
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if (MMITHEME_IsMainScreenLandscape())
    {
        img_display_rect.left   = 0;
        img_display_rect.top    = both_rect.h_rect.bottom;
        img_display_rect.right  = lcd_width-1;
        img_display_rect.bottom = lcd_height-1;
        GUIRES_DisplayImg(PNULL,
            &img_display_rect,
            PNULL,
            win_id,
            IMAGE_FM_BG,
            dev_info_ptr);

        img_display_rect.left   = FM_H_FRQ_BG_START_X;
        img_display_rect.top    = FM_H_FRQ_BG_START_Y;
        img_display_rect.right  = img_display_rect.left+FM_H_FRQ_BG_WIDTH;
        img_display_rect.bottom = img_display_rect.top + height;
        GUIRES_DisplayImg(PNULL,
            &img_display_rect,
            PNULL,
            win_id,
            IMAGE_FM_NUMBER_BG,
            dev_info_ptr);
    }
    else
    {
        img_display_rect.left   = 0;
        img_display_rect.top    = both_rect.v_rect.bottom;
        img_display_rect.right  = lcd_width-1;
        img_display_rect.bottom = lcd_height-1;
        GUIRES_DisplayImg(PNULL,
            &img_display_rect,
            PNULL,
            win_id,
            IMAGE_FM_BG,
            dev_info_ptr);

        img_display_rect.left   = FM_V_FRQ_BG_START_X;
        img_display_rect.top    = FM_V_FRQ_BG_START_Y;
        img_display_rect.right  = img_display_rect.left+FM_V_FRQ_BG_WIDTH;
        img_display_rect.bottom = img_display_rect.top + height;
        GUIRES_DisplayImg(PNULL,
            &img_display_rect,
            PNULL,
            win_id,
            IMAGE_FM_NUMBER_BG,
            dev_info_ptr);

    }

}
#endif
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  IsInFreRect(MMI_WIN_ID_T win_id, GUI_POINT_T in_point)
{
    GUI_RECT_T rect={0};
    GUI_RECT_T suspend_btn_rec={0};
    GUI_POINT_T center_point = {0};
    uint16 width=0;
    uint16 height=0;
    uint16 suspend_btn_width = 0;
    uint16 suspend_btn_height = 0;
    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_FM_AJUST_PLAY_BG, win_id);
    GUIRES_GetImgWidthHeight(&suspend_btn_width, &suspend_btn_height, IMAGE_FM_AJUST_PLAY, win_id);

    center_point = GetFreqNeedleCenterPoint();
    
    rect.left = center_point.x - width/2;
    rect.right= rect.left + width;
    rect.top = center_point.y -height/2;
    rect.bottom=rect.top+height;

    suspend_btn_rec.left = center_point.x -suspend_btn_width/2;
    suspend_btn_rec.right = suspend_btn_rec.left + suspend_btn_width;
    suspend_btn_rec.top = center_point.y -suspend_btn_height/2;
    suspend_btn_rec.bottom= suspend_btn_rec.top + suspend_btn_height;
    if(GUI_PointIsInRect(in_point,rect) && !GUI_PointIsInRect(in_point,suspend_btn_rec))
    {
        return TRUE;
    }
    return FALSE;
}
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  SetVolumnAndListButtonsRect(MMI_WIN_ID_T win_id)
{
    //GUI_BOTH_RECT_T         both_rect={0};
    GUI_RECT_T rect={0};
    uint16 bg_height=0;
    uint16 width=0;
    uint16 height=0;
    GUI_BOTH_RECT_T    full_both_rect=MMITHEME_GetFullScreenBothRect();
    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_FM_VOLUME_PRESSED, win_id);
    GUIRES_GetImgWidthHeight(PNULL, &bg_height, IMAGE_FM_NUMBER_BG, win_id);     
    if (MMITHEME_IsMainScreenLandscape())
    {
        rect.left=FM_H_FRQ_BG_START_X+FM_H_FREQ_LIST_BG_X_SPACE;
        rect.right=rect.left+width;
        rect.top=FM_H_FREQ_LIST_BUTTON_START_Y;
        rect.bottom=rect.top+height;
        GUIBUTTON_SetRect(MMIFM_LIST_BUTTON_CTRL_ID, &rect);        

        rect.left=rect.right;
        rect.right=FM_H_FRQ_BG_START_X+FM_H_FRQ_BG_WIDTH-FM_H_FREQ_LIST_BG_X_SPACE-width;
        GUILABEL_SetRect(FM_CHANNEL_NAME_TEXT_CTRL_ID, &rect, FALSE);
        rect.left=rect.right;
        rect.right=rect.left+width;
        GUIBUTTON_SetRect(MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID, &rect);
    }
    else
    {        
        rect.left=FM_V_FREQ_LIST_BUTTON_START_X;
        rect.right=rect.left+width;
        rect.top = FM_V_FREQ_LIST_BUTTON_START_Y;
        rect.bottom=rect.top+height;
        GUIBUTTON_SetRect(MMIFM_LIST_BUTTON_CTRL_ID, &rect);

        rect.left=rect.right;
        rect.right=full_both_rect.v_rect.right - FM_V_FREQ_LIST_BUTTON_START_X - width;
        GUILABEL_SetRect(FM_CHANNEL_NAME_TEXT_CTRL_ID, &rect, FALSE);
        rect.left=rect.right;
        rect.right=rect.left+width;
        GUIBUTTON_SetRect(MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID, &rect);
    }
}
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  SetButtonsRect(MMI_WIN_ID_T win_id)
{

    GUI_BOTH_RECT_T         both_rect={0};
    uint16 left_width=0;
    uint16 left_height=0;
    uint16 adjust_button_width=0;
    uint16 adjust_button_height=0;
    uint16 fm_adjust_bg_width=0;
    uint16 fm_adjust_bg_height=0;
    GUI_BOTH_RECT_T    full_both_rect=MMITHEME_GetFullScreenBothRect();
    GUI_POINT_T center_point = GetFreqNeedleCenterPoint();
    
    GUIRES_GetImgWidthHeight(&left_width, &left_height, IMAGE_FM_LEFT_DEFAUT, win_id);
    GUIRES_GetImgWidthHeight(&adjust_button_width, &adjust_button_height, IMAGE_FM_AJUST_PLAY, win_id);
    GUIRES_GetImgWidthHeight(&fm_adjust_bg_width, &fm_adjust_bg_height, IMAGE_FM_AJUST_PLAY_BG, win_id);    
    
    both_rect.v_rect.top    =center_point.y - left_height/2;
    both_rect.v_rect.bottom =both_rect.v_rect.top+left_height;
    both_rect.v_rect.left   =(full_both_rect.v_rect.right-fm_adjust_bg_width)/2-FM_V_LEFT_ADJUST_BUTTON_SPACE-left_width;
    both_rect.v_rect.right  =both_rect.v_rect.left+left_width;
    
    both_rect.h_rect.top = center_point.y + fm_adjust_bg_height/2;
    both_rect.h_rect.bottom=both_rect.h_rect.top + left_height;
    both_rect.h_rect.left= center_point.x - fm_adjust_bg_width/2+FM_H_LEFT_ADJUST_BUTTON_X_SPACE;
    both_rect.h_rect.right=both_rect.h_rect.left+left_width;
    GUIAPICTRL_SetBothRect(MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,&both_rect);

    both_rect.v_rect.left=(full_both_rect.v_rect.right+fm_adjust_bg_width)/2+FM_V_LEFT_ADJUST_BUTTON_SPACE;
    both_rect.v_rect.right=both_rect.v_rect.left+left_width;
    both_rect.h_rect.left= center_point.x + fm_adjust_bg_width/2-FM_H_LEFT_ADJUST_BUTTON_X_SPACE-left_width;
    both_rect.h_rect.right=both_rect.h_rect.left+left_width;
    GUIAPICTRL_SetBothRect(MMIFM_GET_UPFRE_BUTTON_CTRL_ID,&both_rect);
    
    both_rect.v_rect.top    =center_point.y - adjust_button_height/2;
    both_rect.v_rect.bottom =both_rect.v_rect.top + adjust_button_height;
    both_rect.v_rect.left=(full_both_rect.v_rect.right-adjust_button_width)/2;
    both_rect.v_rect.right=both_rect.v_rect.left+adjust_button_width;
    
    both_rect.h_rect.top=center_point.y-adjust_button_height/2;
    both_rect.h_rect.bottom=both_rect.h_rect.top + adjust_button_height;
    both_rect.h_rect.left=center_point.x-adjust_button_width/2;
    both_rect.h_rect.right=both_rect.h_rect.left+adjust_button_width;
    GUIAPICTRL_SetBothRect(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,&both_rect);       
}

/*****************************************************************************/
//  Description : set return and menu  button rectangle.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  SetReturnMenuButtonRect(MMI_WIN_ID_T win_id)
{
    GUI_BOTH_RECT_T         both_rect={0};
    uint16 return_width=0;
    uint16 return_height=0;
    uint16 menu_width=0;
    uint16 menu_height=0;
    MMI_IMAGE_ID_T return_image = IMAGE_FM_RETURN_BUTTON_DEFAULT;
    MMI_IMAGE_ID_T menu_image = IMAGE_FM_MENU_BUTTON_DEFAULT;
    GUI_BG_T                gray_bg = {0};
    GUI_BOTH_RECT_T   both_client_rect=MMITHEME_GetWinClientBothRect(win_id);
    
    GUIRES_GetImgWidthHeight(&return_width, &return_height, return_image, win_id);
    GUIRES_GetImgWidthHeight(&menu_width, &menu_height, menu_image, win_id);
    
    both_rect.v_rect.top=FM_V_RETURN_BUTTON_Y_START; 
    both_rect.v_rect.bottom=both_rect.v_rect.top + return_height;
    both_rect.v_rect.left= FM_V_RETURN_BUTTON_X_START;
    both_rect.v_rect.right=both_rect.v_rect.left + return_width;

    both_rect.h_rect.left= FM_H_RETURN_BUTTON_X_START;
    both_rect.h_rect.right=both_rect.h_rect.left + return_width;
    both_rect.h_rect.top=FM_H_RETURN_BUTTON_Y_START;    
    both_rect.h_rect.bottom=both_rect.h_rect.top + return_height;    
    GUIAPICTRL_SetBothRect(return_button_id,&both_rect);

    both_rect.v_rect.left= both_client_rect.v_rect.right - FM_V_RETURN_BUTTON_X_START - menu_width;
    both_rect.v_rect.right=both_rect.v_rect.left + menu_width;
    both_rect.h_rect.left= FM_H_FRQ_BG_START_X+FM_H_FRQ_BG_WIDTH-menu_width-(FM_H_RETURN_BUTTON_X_START-FM_H_FRQ_BG_START_X);
    both_rect.h_rect.right=both_rect.h_rect.left + menu_width;    
    GUIAPICTRL_SetBothRect(menu_button_id,&both_rect);  
    
    gray_bg.bg_type = GUI_BG_IMG;
    gray_bg.img_id  = IMAGE_FM_RETURN_BUTTON_PRESSED;
    GUIBUTTON_SetRunSheen(return_button_id,FALSE);
    GUIBUTTON_SetPressedBg(return_button_id,&gray_bg);
    gray_bg.img_id  = IMAGE_FM_MENU_BUTTON_PRESSED;
    GUIBUTTON_SetRunSheen(menu_button_id,FALSE);
    GUIBUTTON_SetPressedBg(menu_button_id,&gray_bg);
}

/*****************************************************************************/
//  Description : update return and menu  button 
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  UpdateReturnMenuButton(MMI_WIN_ID_T win_id, BOOLEAN is_refresh)
{
    BOOLEAN is_display = MMITHEME_IsIstyle()?TRUE:FALSE;   
    GUIBUTTON_SetVisible( return_button_id,  is_display,  is_refresh);
    GUIBUTTON_SetVisible( menu_button_id,  is_display,  is_refresh);
}

#endif
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  CreateButtons(MMI_WIN_ID_T win_id)
{
    int32                   i = 0;
    int32                   loop_count = 0;
    GUI_BG_T                gray_bg = {0};
    
    loop_count = sizeof(s_button_ids)/sizeof(*s_button_ids);
    for(i = 0; i < loop_count; i++)
    {
        gray_bg.bg_type = GUI_BG_IMG;
        gray_bg.img_id  = s_button_ids[i][MMIFM_BUTTON_GR_IMAGE];
        GUIBUTTON_SetGrayed(s_button_ids[i][MMIFM_BUTTON_ID], FALSE, &gray_bg, FALSE);
        
        gray_bg.img_id  = s_button_ids[i][MMIFM_BUTTON_PR_IMAGE];
        GUIBUTTON_SetPressedBg(s_button_ids[i][MMIFM_BUTTON_ID],&gray_bg);
        GUIBUTTON_SetRunSheen(s_button_ids[i][MMIFM_BUTTON_ID], FALSE);
    }
   
    GUIBUTTON_SetHandleLong(MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,TRUE);
    GUIBUTTON_SetHandleLong(MMIFM_GET_UPFRE_BUTTON_CTRL_ID,TRUE);

}

/*****************************************************************************/
//  Discription: Clear the client rect  
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL  void  ClrClientRect(void)
{
    GUI_RECT_T          rect = MMITHEME_GetClientRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    LCD_FillRect(&lcd_dev_info, rect, MMI_WINDOW_BACKGROUND_COLOR);
}

/*****************************************************************************/
//  Discription: play user input channel
//  Global resource dependence: none 
//  Parameter: ch_num_str: channel's number string
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL  void  PlayInputChannel(const wchar *ch_num_str)
{
    BOOLEAN is_ok = FALSE;
    uint8   a_str[MMIFM_CHANNEL_NUMBER_MAX_BIT + 1] = {0};
    uint16  ch_num_len = 0;
    uint16  ch_index = 0;
    if(PNULL != ch_num_str)
    {
        ch_num_len = MMIAPICOM_Wstrlen(ch_num_str);
        MMI_WSTRNTOSTR(a_str, MMIFM_CHANNEL_NUMBER_MAX_BIT, ch_num_str, MMIFM_CHANNEL_NUMBER_MAX_BIT, ch_num_len);
        ch_index = atoi((char*)a_str);
        if(ch_index > 0)
        {
            if(MMIFM_IsChannelExist(ch_index -1))
            {
                MMIFM_PlayChannel(ch_index -1);
                SetButtonBgWhenResume();
                is_ok = TRUE;
            }
        }
    }
    if(!is_ok)
    {
        MMIPUB_OpenAlertWarningWin(TXT_FM_CHANNEL_NOT_EXIST);
    }
    MMK_SendMsg(FM_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
}

/*****************************************************************************/
//  Description : handle headset plug in and plug out action
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_HandleHeadsetAction(MMI_MESSAGE_ID_E msg_id)
{
    MMI_WIN_ID_T     alert_id = MMIFM_HEADSET_PLUG_ALERT_ID;
    MMIFM_CUR_INFO_T *cur_info_ptr = PNULL;
    MMIFM_START_WAY_E  start_way = (uint32)MMK_GetWinAddDataPtr(FM_MAIN_WIN_ID);
    
    //SCI_TRACE_LOW:"[MMIFM] MMIFM_HandleHeadsetAction msg_id %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_5185_112_2_18_2_19_38_84,(uint8*)"d", msg_id);
    cur_info_ptr = MMIFM_GetFMCurrentInfo();
    if(FM_IsInit())
    {        
        if (MSG_KEYUP_HEADSET_DETECT == msg_id)
        {            
            //plug out headset          
            if(PREVIEW_START_FM == start_way)
            {
                MMK_CloseWin(FM_MAIN_WIN_ID);
            }
            else if(MMK_IsOpenWin(alert_id))
            {
                FM_PlayStop();
                MMISRVAUD_Stop(MMIFM_GetPlayHandle());
                MMK_SendMsg(alert_id, MSG_APP_FM_HEADSET_PLUG_OUT, PNULL);
            }
            else
            {
#ifdef MMIFM_SUPPORT_FM_RECORD
                MMIAPIFM_StopRecord();
#endif
               // FM_PlayStop();
               // MMISRVAUD_Stop(MMIFM_GetPlayHandle());
                if(cur_info_ptr->is_bgplay)
                {
                  MMIFM_Exit(FALSE); 
                  MMIAPIIDLE_MAIN_UpdateIdle();
                }
                else if(FM_PLAYING == MMIAPIFM_GetStatus())
                {               
                  MMIAPIFM_Pause(TRUE);
                  if(MMK_IsFocusWin(FM_MAIN_WIN_ID))
                  {
                    MMK_SendMsg(FM_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
                  }
                } 
            }            
        }
        else
        {
            //plug in headset
            if(MMK_IsOpenWin(alert_id))
            {
                if(FM_PLAYING == cur_info_ptr->status)
                {
                    MMIFM_Play(FALSE);
                }
#ifdef MMIFM_SUPPORT_FM_RECORD
                if(MMIFMRECORD_STATE_RECORD_PAUSED == MMIFM_GetRecordStatus())
				{
					MMIFM_RecordResumeRecord(TRUE);
				}
#endif				
                MMK_SendMsg(FM_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
                MMK_SendMsg(alert_id, MSG_APP_FM_HEADSET_PLUG_IN, PNULL);                
            }
        }
    }
    else if(PNULL != cur_info_ptr && cur_info_ptr->is_bgplay)
    {
        //bg play if it is in playing or suspend, must stop it
        if (MSG_KEYUP_HEADSET_DETECT == msg_id && (FM_PLAYING == cur_info_ptr->status || FM_SUSPENDED== cur_info_ptr->status))
        {
            MMIFM_Stop();
            MMIAPIIDLE_MAIN_UpdateIdle();
        }
    }
}

/*****************************************************************************/
//  Discription: stop auto search
//  Global resource dependence: none 
//  Parameter: 
//  Note:
//
/*****************************************************************************/
PUBLIC  void  MMIFM_StopAutoSearch(void)
{
    MMK_CloseWin(FM_AUTO_SEARCH_WIN_ID);
}

/*****************************************************************************/
//  Description : set current selected channel no. 
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL void SetCurrentSelChannel(uint16 ch_no)
{
    s_current_sel_channel = ch_no;
    //SCI_TRACE_LOW:"SetCurrentSelChannel ch_no %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_5257_112_2_18_2_19_38_85,(uint8*)"d", ch_no);
}

/*****************************************************************************/
//  Description : get current selected channel no.
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetCurrentSelChannel(void)
{
    //SCI_TRACE_LOW:"GetCurrentSelChannel s_current_sel_channel %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_5266_112_2_18_2_19_38_86,(uint8*)"d", s_current_sel_channel);
    return s_current_sel_channel;
}

/*****************************************************************************/
//  Description : set current operation type
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL void SetCurrentOperator(MMIFM_OPERTION_TYPE_E opt_type)
{
    s_current_operator = opt_type;
    //SCI_TRACE_LOW:"SetCurrentOperator s_current_operator %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_5277_112_2_18_2_19_38_87,(uint8*)"d", s_current_operator);
}

/*****************************************************************************/
//  Description : get current selected channel no.
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL MMIFM_OPERTION_TYPE_E GetCurrentOperator(void)
{
    //SCI_TRACE_LOW:"GetCurrentOperator s_current_operator %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_5286_112_2_18_2_19_38_88,(uint8*)"d", s_current_operator);
    return s_current_operator;
}
/*****************************************************************************/
//  Description : get current Rect.
//  Global resource dependence : 
//  Parameter:
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetCurrentRect(uint16 left, uint16 top,  uint16 right, uint16 bottom)
{

    GUI_RECT_T rect = {0};
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;

    //SCI_ASSERT(rect.left <= rect.right);/*assert verified*/
    //SCI_ASSERT(rect.top <= rect.bottom);/*assert verified*/
    return rect;
}
#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : Set FM Main Form Param
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetPDAFMMainParam(MMI_WIN_ID_T win_id)
{
    MMIFM_START_WAY_E           start_way = NORMAL_START_FM;
    SetButtonsRect(win_id);
    SetVolumnAndListButtonsRect(win_id);
    SetReturnMenuButtonRect(win_id);
    CreateButtons(win_id);    

    start_way = (uint32)MMK_GetWinAddDataPtr(win_id);
    if(PREVIEW_START_FM == start_way)
    {
        FMMainUpdateButtonBg();
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,TRUE);
        s_is_need_set_bgplay = FALSE;
    }
    else
    {
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
        SetVolumeBgWhenZero();
#endif
        s_is_need_set_bgplay = TRUE;
    }
}
#else
/*****************************************************************************/
//  Description : Set FM Main Form Param
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetFMMainFormParam(MMI_WIN_ID_T win_id)
{
    GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0}; 
    MMIFM_START_WAY_E           start_way = NORMAL_START_FM;
    
    data_info.img_id = IMAGE_FM_ADJ_BG;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIFM_FREQUENCY_BAR_CTRL_ID;
    control_info.ctrl_ptr = PNULL;

    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HMIDDLE_TOP;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);    

    SetButtonsAndLabelRect(win_id);
    CreateButtons(win_id);

    start_way = (uint32)MMK_GetWinAddDataPtr(win_id);
    if(PREVIEW_START_FM == start_way)
    {
        FMMainUpdateButtonBg();
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,TRUE);
        s_is_need_set_bgplay = FALSE;
    }
    else
    {
        FMMainUpdateScreen();
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
	SetVolumeBgWhenZero();
#endif
        s_is_need_set_bgplay = TRUE;
    }
}
#ifdef MMI_FM_MINI_SUPPORT
/*****************************************************************************/
//  Description : Display FM Backgroud
//  Global resource dependence : none
//  Author: langhua.yang
//  Note:just for MAINLCD_SIZE_128X160 and MAINLCD_SIZE_176X220
/*****************************************************************************/
LOCAL void DisplayFMBg(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetStatusBarBothRect();
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    GUI_RECT_T fm_bg_display_rect = {0};
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    fm_bg_display_rect.left   = 0;
    fm_bg_display_rect.right  = fm_bg_display_rect.left + lcd_width;
    fm_bg_display_rect.bottom = fm_bg_display_rect.top + lcd_height;
    if (MMITHEME_IsMainScreenLandscape())
    {        
        fm_bg_display_rect.top    = both_rect.h_rect.bottom;
    }
    else
    {
        fm_bg_display_rect.top    = both_rect.v_rect.bottom;
    }
    GUI_FillRect(&lcd_dev_info, fm_bg_display_rect, MMI_WHITE_COLOR);
}
#endif
#endif
/*****************************************************************************/
//  Description : Open FM Volume Panel
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void OpenFMVolumePanel(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type)
{    
    MMICOM_PANEL_INFO_T vol_panel = {0};
    MMI_WIN_ID_T win_id = FM_MAIN_WIN_ID;
    
    vol_panel.x = -1;
    vol_panel.y = -1;
    vol_panel.panel_type = MMICOM_PANEL_VOLUME;
    vol_panel.open_type  = turn_vol_type;
    vol_panel.min_value  = MMIFM_VOL_LEVEL_0;
    vol_panel.max_value  = MMIFM_VOL_LEVEL_MAX;
    vol_panel.cur_value  = MMIAPISET_GetMultimVolume();
    vol_panel.last_value = MMIAPISET_GetLastMultimVolume();
    vol_panel.set_callback = AdjustFMVolumeCallback;
    MMIAPICOM_OpenPanelChildWin(win_id, &vol_panel);
}
/*****************************************************************************/
//  Description : AdjustFMVolumeCallback
//  Global resource dependence : 
//  Parameter: adjust volume to current volume + step 
//  Note: 
/*****************************************************************************/
LOCAL void AdjustFMVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr)
{
    if(NULL == para_ptr)
    {
        return;
    }
    MMIFM_AdjustVolumeTP((int)para_ptr->cur_value);
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
	SetVolumeBgWhenZero();
#endif
}

/*****************************************************************************/
//  Description : draw status Button
//  Global resource dependence : 
//  Parameter: 
//  Note: 
/*****************************************************************************/
LOCAL void DrawStatusButton(MMI_WIN_ID_T win_id)
{
    GUI_BG_T btn_bg = {0};
#ifdef MMI_FM_MINI_SUPPORT   
    /* add by langhua.yang to update the button rect*/
    IGUICTRL_T* button_ptr = MMK_GetCtrlPtr(MMIFM_START_SUSPEND_BUTTON_CTRL_ID);
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T  rect = {0};
    if (button_ptr)
    {
        IGUICTRL_GetLcdDevInfo(button_ptr, &lcd_dev_info);
        IGUICTRL_GetRect(button_ptr, &rect);
        GUI_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
    }
#endif

#ifdef FM_NEW_UI 
    GUI_POINT_T point_t = {0};    
    uint16 bg_width=0;
    uint16 bg_height=0;
#endif    
    btn_bg.bg_type = GUI_BG_IMG;
#ifdef FM_NEW_UI   
    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, IMAGE_FM_AJUST_PAUSE_BG, win_id);    
    point_t = GetFreqNeedleCenterPoint();
    point_t.x = point_t.x - bg_width/2;
    point_t.y = point_t.y - bg_height/2; 
#endif    
    if(FM_SUSPENDED == MMIAPIFM_GetStatus() || FM_STOP == MMIAPIFM_GetStatus())
    { 
		//pause or stop status , draw play button
#ifdef FM_NEW_UI
        btn_bg.img_id  = IMAGE_FM_AJUST_PAUSE;
#else
        btn_bg.img_id  = IMAGE_FM_PLAY_DEFAULT;
#endif
        GUIBUTTON_SetBg(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,&btn_bg);
#ifndef FM_NEW_UI
        btn_bg.img_id  = IMAGE_FM_PLAY_PRESSED;
        GUIBUTTON_SetPressedBg(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,&btn_bg);
#endif
#ifdef FM_NEW_UI
        GUIRES_DisplayImg(&point_t, 
                           PNULL, 
                           PNULL, 
                           win_id, 
                           IMAGE_FM_AJUST_PAUSE_BG, 
                           &s_main_lcd_dev_info);
#endif
    }
    else if(FM_PLAYING == MMIAPIFM_GetStatus() )
    {
       //play status , draw pause button
#ifdef FM_NEW_UI
        btn_bg.img_id  = IMAGE_FM_AJUST_PLAY;
#else
        btn_bg.img_id  = IMAGE_FM_PAUSE_DEFAULT;
#endif
        GUIBUTTON_SetBg(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,&btn_bg);
#ifndef FM_NEW_UI
        btn_bg.img_id  = IMAGE_FM_PAUSE_PRESSED;
        GUIBUTTON_SetPressedBg(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,&btn_bg);
#endif
#ifdef FM_NEW_UI
        GUIRES_DisplayImg(&point_t, 
                           PNULL, 
                           PNULL, 
                           win_id, 
                           IMAGE_FM_AJUST_PLAY_BG, 
                           &s_main_lcd_dev_info);
#endif
    }
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
	SetVolumeBgWhenZero();
#endif
}
/*****************************************************************************/
//  Description : Set FM StartSuspend Button Status
//  Global resource dependence : 
//  Parameter: Set FM Status
//  Note: 
/*****************************************************************************/
LOCAL void SetFMStartSuspendButtonStatus(MMI_WIN_ID_T win_id)
{
    GUI_BG_T btn_bg = {0};
    btn_bg.bg_type = GUI_BG_IMG;

    if(MMIFM_CheckHeadsetStatus())
    {      
        if(FM_PLAYING == MMIAPIFM_GetStatus())
        {               
            MMIAPIFM_Pause(TRUE);  
        }
        else if(FM_SUSPENDED == MMIAPIFM_GetStatus() )
        {
            MMIAPIFM_Resume(TRUE);
        }
        else if(FM_STOP == MMIFM_GetStatus())
        {
            if(!MMIFM_Play(TRUE))
            {                
                MMIFM_Exit(FALSE);
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_FM_INIT_FAIL,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);               
                return;
            }
            else if(0 == MMIFM_GetValidChannelCount())                
            {
                OpenAuotSearchWin(MMIFM_AUTOSEARCH,FM_SCAN_UP);
            }
        }
        if(MMK_IsFocusWin(FM_MAIN_WIN_ID))
        {
    	    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
    }
}
/*****************************************************************************/
//  Description : Display FM Fre Pic
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void DisplayFMFrePic(GUIANIM_OWNER_DRAW_T    *draw_ptr)
{
#ifndef FM_NEW_UI
    uint16              adj_but_x = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMIFM_CUR_INFO_T    *cur_info_ptr = MMIFM_GetFMCurrentInfo();
#ifndef MMI_FM_MINI_SUPPORT
    GUI_BOTH_RECT_T     statusbar_both_rect=MMITHEME_GetStatusBarBothRect();
#endif
    GUI_POINT_T         dis_point = {0};
    uint16              adj_but_left_y =0;   
    uint16              icon_width = 0;
#ifndef MMI_FM_MINI_SUPPORT
    if(MMITHEME_IsMainScreenLandscape())
    {
        adj_but_left_y = statusbar_both_rect.h_rect.bottom-statusbar_both_rect.h_rect.top + MMIFM_ADJ_BUT_H_START_Y;
    }
    else
    {
        adj_but_left_y = statusbar_both_rect.v_rect.bottom-statusbar_both_rect.v_rect.top + MMIFM_ADJ_BUT_V_START_Y;
    }
#else
      if(MMITHEME_IsMainScreenLandscape())
        {
           adj_but_left_y = MMIFM_TITLE_HEIGHT + MMIFM_ADJ_BUT_H_START_Y;
        }
        else
        {
           adj_but_left_y = MMIFM_TITLE_HEIGHT + MMIFM_ADJ_BUT_V_START_Y;
        }
#endif
    if(PNULL != cur_info_ptr)
    {
        MMIFM_START_WAY_E           start_way = NORMAL_START_FM;
        
        GUIRES_GetImgWidthHeight(&icon_width, NULL, IMAGE_FM_ADJ_BUT, FM_MAIN_WIN_ID);

        //窗口创建之前该状态必须设置，否则报错
        if(draw_ptr != NULL)
            start_way = (uint32)MMK_GetWinAddDataPtr(draw_ptr->win_handle);
        if(start_way == PREVIEW_START_FM)
            adj_but_x = (MMITHEME_GetClientRectWidth()- icon_width)* (cur_info_ptr->export_freq - MMIFM_FREQ_RANGE_LOW)/(MMIFM_FREQ_RANGE_HIGH - MMIFM_FREQ_RANGE_LOW);
        else
            adj_but_x = (MMITHEME_GetClientRectWidth()- icon_width)* (cur_info_ptr->cur_freq - MMIFM_FREQ_RANGE_LOW)/(MMIFM_FREQ_RANGE_HIGH - MMIFM_FREQ_RANGE_LOW);
    }
    
    dis_point.x = adj_but_x;
    dis_point.y = adj_but_left_y;
    GUIRES_DisplayImg(&dis_point, PNULL, PNULL, FM_MAIN_WIN_ID, IMAGE_FM_ADJ_BUT, &lcd_dev_info);
#endif
}
/*****************************************************************************/
//  Description :Alarm channel list window message handle
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAlarmChannelListWinMsg(
                                           MMI_WIN_ID_T        win_id,  
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM              param
                                           )
{
    MMI_STRING_T    str_t       ={0};
#ifdef MMI_PDA_SUPPORT   
    static uint16   s_cur_list_index = 0; 
#endif    
    const GUILIST_ITEM_T        *list_item_ptr = PNULL;
    MMI_RESULT_E    result      = MMI_RESULT_TRUE;
    uint16          index       = 0;    
    //uint8           ring_vol    = 0;
    MMIFM_CHANNEL_T channel     ={0};    
    MMI_WIN_ID_T *w_id=(MMI_WIN_ID_T *) MMK_GetWinAddDataPtr(win_id);
    //AUDIO_DEVICE_MODE_TYPE_E dev_mode = AUDIO_DEVICE_MODE_HANDHOLD;
    //SCI_ASSERT(NULL != w_id);/*assert verified*/
    if(PNULL == w_id)
    {
		return result;
	}
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            index = 0;//high light first item
            MMI_GetLabelTextByLang(TXT_COMM_CHANNEL_LIST, &str_t);
            ReadChannelsToAlarmListBox(win_id, MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID);/*lint !e64*/
            GUILIST_SetCurItemIndex(MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID, index);
            MMK_SetAtvCtrl(win_id, MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID);
            GUIWIN_SetTitleText(win_id, str_t.wstr_ptr, str_t.wstr_len, FALSE);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;
#ifdef MMI_PDA_SUPPORT   
     case MSG_CTL_LIST_CHECK_CONTENT:
         {
            s_cur_list_index = GUILIST_GetCurItemIndex(MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID);
            list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID, s_cur_list_index);
            SetCurrentSelChannel((uint16)list_item_ptr->user_data);
            //SCI_TRACE_LOW:"[MMIFM] HandleAlarmChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_5631_112_2_18_2_19_39_89,(uint8*)"d", GetCurrentSelChannel());
            if(MMIAPISET_GetFlyMode())
            {
        	    MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST);
        	    break;
   			}
            if(MMIFM_CheckHeadsetStatus())
            {            
                if(MMIFM_IsChannelExist(GetCurrentSelChannel()))
                {
                    MMIAPIALARM_ReqPlaySrv();
                    MMIFM_GetOneChannel(GetCurrentSelChannel(), &channel);                    
                    MMIAPIFM_Play(MMIAPIALARM_GetPlayHandle(),channel.freqency);
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, STXT_STOP, STXT_RETURN, TRUE);
                    MMK_CreateWin((uint32 *)MMI_FM_MAIN_WIN_TAB, (ADD_DATA)PREVIEW_START_FM);
                }
            }
        }
         break;
#endif
        case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT
        index=GUILIST_GetCurItemIndex(MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID);
        list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID, index);        
        MMIFM_GetOneChannel((uint16)list_item_ptr->user_data, &channel);
        if(MMIFM_IsChannelValid(&channel))
        {
            MMK_SendMsg(*w_id, MSG_ALM_SELECTED_FM_RETURN, (void*)&channel);
            MMK_CloseWin(win_id);
        }
#else
        {
            index = GUILIST_GetCurItemIndex(MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID);
            list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID, index);
            SetCurrentSelChannel((uint16)list_item_ptr->user_data);
            
            //SCI_TRACE_LOW:"[MMIFM] HandleAlarmChannelListWinMsg MSG_APP_WEB s_current_sel_channel %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_5668_112_2_18_2_19_39_90,(uint8*)"d", GetCurrentSelChannel());
            if(MMIAPISET_GetFlyMode())
            {
        	    MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST);
        	    break;
   			}
            if (MMIFM_CheckHeadsetStatus())
            {            
                if(MMIFM_IsChannelExist(GetCurrentSelChannel()))
                {
                    MMIAPIALARM_ReqPlaySrv();
                    MMIFM_GetOneChannel(GetCurrentSelChannel(), &channel);
                    MMIAPIFM_Play(MMIAPIALARM_GetPlayHandle(),channel.freqency);
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, STXT_STOP, STXT_RETURN, TRUE);
                    MMK_CreateWin((uint32 *)MMI_FM_MAIN_WIN_TAB, (ADD_DATA)PREVIEW_START_FM);
                }
            }
        }
#endif
        break;
        case MSG_CTL_OK:
        case MSG_APP_OK:
        index=GUILIST_GetCurItemIndex(MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID);
        list_item_ptr = GUILIST_GetItemPtrByIndex(MMI_ALARM_FM_CHANNEL_LIST_CTRL_ID, index);        
        MMIFM_GetOneChannel((uint16)list_item_ptr->user_data, &channel);
        if(MMIFM_IsChannelValid(&channel))
        {
            MMK_SendMsg(*w_id, MSG_ALM_SELECTED_FM_RETURN, (void*)&channel);
            MMK_CloseWin(win_id);
        }
        break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;
        case MSG_CLOSE_WINDOW:
        SCI_FREE(w_id);
        break;
        default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
/*****************************************************************************/
//  Description : Create Alarm FM Channel List
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_CreateAlarmChannelList(ADD_DATA param)
{
    MMK_CreateWin((uint32 *)MMI_ALARM_FM_CHANNEL_LIST_WIN_TAB, param);
}
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Description : Create FM timer record 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void StartFMTimerRecord(void)
{      
    if(STARTUP_NORMAL == MMIAPIPHONE_GetStartUpCondition())
    {
        MMK_CreateWin((uint32 *)MMI_FM_RECORD_WIN_TAB, (ADD_DATA)FM_timer_record);
    }
    else 
    {
        MMK_CreateWin((uint32 *)MMI_FM_RECORD_NO_STATUSBAR_WIN_TAB, (ADD_DATA)FM_timer_record);
    }
}
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : read channels to list box
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ReadChannelsToAlarmListBox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{   
    uint16 i = 0;
    MMIFM_CHANNEL_T channel = {0};   /*lint !e64*/
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    
    GUILIST_SetMaxItem(ctrl_id,MMIFM_MAX_CHANNEL_NUM, FALSE );
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;   
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
        //get exist channels firstly
        SCI_MEMSET(&channel, 0x00, sizeof(MMIFM_CHANNEL_T));
        SCI_MEMSET(temp_wstr, 0x00, sizeof(wchar)*(GUILIST_STRING_MAX_NUM + 1));
        if(MMIFM_GetOneChannel(i, &channel) && MMIFM_IsChannelValid(&channel))
        {
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_COMMON_OK;
            item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_FM_PLAY;
            //SCI_MEMSET(item_data.item_content[0].item_data.text_buffer.wstr, 0x00, sizeof(item_data.item_content[0].item_data.text_buffer.wstr));
            MMIFM_GetChannelDispName(&channel, i, temp_wstr, GUILIST_STRING_MAX_NUM);

            item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data.item_content[0].item_data.text_buffer.wstr_ptr);
            item_t.user_data = i;
            item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[1].item_data.image_id=IMAGE_FM_PREVIEW_ICON;
            item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
            GUILIST_AppendItem( ctrl_id, &item_t);
        }
    }
 /*  
    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
        //get not exist channels 
        SCI_MEMSET(&channel, 0x00, sizeof(MMIFM_CHANNEL_T));
        SCI_MEMSET(temp_wstr, 0x00, sizeof(wchar)*(GUILIST_STRING_MAX_NUM + 1));
        if(!MMIFM_GetOneChannel(i, &channel) || !MMIFM_IsChannelValid(&channel))
        {
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_NULL;
            item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
            MMIFM_GetChannelDispName(&channel, i, temp_wstr, GUILIST_STRING_MAX_NUM);
            item_t.user_data = i;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data.item_content[0].item_data.text_buffer.wstr_ptr);
            //item_data.item_content[1].item_data_type = GUIITEM_DATA_NONE;
            //item_t.item_state = 0;
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
            GUILIST_AppendItem( ctrl_id, &item_t);
        }
    }
*/   
}
#else
/*****************************************************************************/
//  Description : read channels to list box
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ReadChannelsToAlarmListBox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{   
    uint16 i = 0;
    MMIFM_CHANNEL_T channel = {0};   /*lint !e64*/
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    
    GUILIST_SetMaxItem(ctrl_id,MMIFM_MAX_CHANNEL_NUM, FALSE );
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;   
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
    
    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
        //get exist channels firstly
        SCI_MEMSET(&channel, 0x00, sizeof(MMIFM_CHANNEL_T));
        SCI_MEMSET(temp_wstr, 0x00, sizeof(wchar)*(GUILIST_STRING_MAX_NUM + 1));
        if(MMIFM_GetOneChannel(i, &channel) && MMIFM_IsChannelValid(&channel))
        {
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_COMMON_OK;
#ifndef GUI_MID_SOFTKEY_USE_IMG 
                item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_FM_PLAY;
#else           
                item_data.softkey_id[1] = (MMI_IMAGE_ID_T)IMAGE_COMMON_SOFTKEY_START_ICON;
#endif            
            //SCI_MEMSET(item_data.item_content[0].item_data.text_buffer.wstr, 0x00, sizeof(item_data.item_content[0].item_data.text_buffer.wstr));
            MMIFM_GetChannelDispName(&channel, i, temp_wstr, GUILIST_STRING_MAX_NUM);

            item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data.item_content[0].item_data.text_buffer.wstr_ptr);
            item_t.user_data = i;
            GUILIST_AppendItem( ctrl_id, &item_t);
        }
    }
    for(i = 0; i < MMIFM_MAX_CHANNEL_NUM; i++)
    {
        //get not exist channels 
        SCI_MEMSET(&channel, 0x00, sizeof(MMIFM_CHANNEL_T));
        SCI_MEMSET(temp_wstr, 0x00, sizeof(wchar)*(GUILIST_STRING_MAX_NUM + 1));
        if(!MMIFM_GetOneChannel(i, &channel) || !MMIFM_IsChannelValid(&channel))
        {
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_NULL;
#ifndef MMI_GUI_STYLE_MINISCREEN 
                item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
#else           
                item_data.softkey_id[1] = (MMI_IMAGE_ID_T)IMAGE_NULL;
#endif            
            MMIFM_GetChannelDispName(&channel, i, temp_wstr, GUILIST_STRING_MAX_NUM);
            item_t.user_data = i;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data.item_content[0].item_data.text_buffer.wstr_ptr);
            GUILIST_AppendItem( ctrl_id, &item_t);
        }
    }
}
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Discription: Handle Fm Record win msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmRecordMsg(
                                              MMI_WIN_ID_T      win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM                param
                                              )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMIFMRECORD_RESULT_E    result = MMIFMRECORD_RESULT_SUCCESS; 
    MMIFM_record_type_E  record_type = FM_record;
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    uint16  i_endure_time_min = 0;
#endif
    MMIFM_TIMER_RECORD_INFO_T record_info = {0};
    GUI_BORDER_T    edit_border = {0};
/*#ifdef MMI_PDA_SUPPORT
    MMI_CONTROL_CREATE_T        softkey_ctrl = {0};
    GUISOFTKEY_INIT_DATA_T      softkey_init = {0};
//#endif*/

    record_type = (uint32)MMK_GetWinAddDataPtr(win_id);
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    if(FM_timer_record == record_type)
    {
        MMIFM_GetTimerRecordInfo(&record_info);
        i_endure_time_min = record_info.endure_time;
    }
#endif
    if (MSG_TIMER != msg_id)
    {
        //SCI_TRACE_LOW:"[MMIFMRECORD] HandleFmRecordMsg() msg_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_5874_112_2_18_2_19_39_91,(uint8*)"d", msg_id);
    }

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            if(MMIFM_CheckHeadsetStatus())
            {
                SetFMRecordFormParam();
                MMK_SetAtvCtrl(win_id, MMIFM_TIME_OWNDRAW_CTRL_ID);
                edit_border.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT);
                GUIFORM_SetActiveChildTheme(MMIFM_MAINPLAY_FORM_CTRL_ID,PNULL, &edit_border, PNULL);
                MMIFM_RecordConstructor();  
                result = FMRecordStartRecord();            
                switch(result)
                {
                    case MMIFMRECORD_RESULT_NO_SPACE:
                        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);         
                        MMK_CloseWin(win_id);
                        break;
                        
                    case MMIFMRECORD_RESULT_MAX_FILE:
                        MMIPUB_OpenAlertWarningWin(TXT_FM_RECORD_DELETE_FILES);           
                        MMK_CloseWin(win_id);
                        break;

                    case MMIFMRECORD_RESULT_DEVICE_NOT_EXIST:
                        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);         
                        MMK_CloseWin(win_id);
                        break;
                        
                    case MMIFMRECORD_RESULT_ERROR:
                        MMIPUB_OpenAlertWarningWin(TXT_ERROR);
                        MMK_CloseWin(win_id);
                        break;
                        
                    default:
                        break;
                }
                
                if(MMIFMRECORD_STATE_RECORDING == MMIFM_GetRecordStatus())
                {
                    FMStartDisplayProgressTimer();
                    FMRecordSetLabelText();  
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                }
                MMIDEFAULT_TurnOnBackLight();
			}
        }
        break;

        case MSG_FULL_PAINT: 
        {
            FMRecordUpdateScreen();
        }
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK,  PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                //MMK_SendMsg(win_id, MSG_APP_CANCEL,  PNULL);
                MMIFM_RecordStopRecord();
                if(STARTUP_NORMAL == MMIAPIPHONE_GetStartUpCondition())
                {
                    MMK_CreateWin((uint32 *)MMIFM_RECORD_RENAME_WIN_TAB ,PNULL);
                }
                else
                {
                    MMK_CreateWin((uint32 *)MMIFM_RECORD_RENAME_NO_STATUSBAR_STATUSWIN_TAB ,PNULL);
                    
                }
                break;            
            default:
                break;
            }
        }
        break;
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527
#endif 

        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:     
        case MSG_APP_OK:
        case MSG_CTL_OK:
        {
            HandleFMRecordAppOkMsg(win_id);
        }
        break;
        case MSG_LOSE_FOCUS:
        {
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            if(MMIAPICC_IsCcOnTopWin())
            {
                MMIFM_RecordStopRecord();
                MMK_CloseWin(win_id);
                MMIFM_ExitAllWindows();
        //        MMIAPIFM_Exit();
            }
        }
        break;
        case MSG_GET_FOCUS:
        {
            if(MMIFMRECORD_STATE_RECORDING == MMIFM_GetRecordStatus())
            {                
                FmRecordDisplayRecordTime();
                FMStartDisplayProgressTimer();
            }
        }
        break;
        case MSG_CLOSE_WINDOW:
        {               
            MMIFM_RecordDestructor();
            MMIFM_RecordStopRecord();            
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            FMStopDisplayProgressTimer();
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
            if(FM_timer_record == record_type)
            {                 
                if(!MMK_IsOpenWin(FM_MAIN_WIN_ID) && !s_is_need_set_bgplay)
                {   
                  MMIFM_Exit(s_is_need_set_bgplay);
                }
                else
                {
                  if(FM_SUSPENDED == MMIFM_GetStatus())
                  { 
                    if(MMIFM_GetSuspendByFM())
                    {
                      FM_PlayStop();
                      MMISRVAUD_Stop(MMIFM_GetPlayHandle());//NEWMS00229455
                    }
                  }
                }

            }
#endif
            MMIAPIALM_CheckandStartExpiredEvent();
        }
        break;
            
        case MSG_TIMER:         
        if (s_time_timer_id == *((uint8 *) param))
        {
            FMStopDisplayProgressTimer();               
            if(MMK_IsFocusWin(FM_RECORD_WIN_ID))
            {
                FmRecordDisplayRecordTime();
                FMStartDisplayProgressTimer();
            }
			SCI_TRACE_LOW("s_record_cur_timer_elapsed is %d", s_record_cur_timer_elapsed);
            
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
            if((FM_timer_record == record_type)
                &&(s_record_cur_timer_elapsed >= i_endure_time_min*60)
              )
            {                  
                MMK_CloseWin(win_id);                 
            }
#endif
        }           
        else
        {
            recode = MMI_RESULT_FALSE;
        }         
        break;
        
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {   
            MMIFM_RecordStopRecord();
            //MMK_CloseWin(win_id);    
            if(STARTUP_NORMAL == MMIAPIPHONE_GetStartUpCondition())
            {
                MMK_CreateWin((uint32 *)MMIFM_RECORD_RENAME_WIN_TAB ,PNULL);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMIFM_RECORD_RENAME_NO_STATUSBAR_STATUSWIN_TAB ,PNULL);
                
            }
        }
        break;
        default:
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    }
    return recode;
}
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Discription: Handle Fm Timer Record win msg
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmTimerRecordMsg(
                                              MMI_WIN_ID_T      win_id, 
                                              MMI_MESSAGE_ID_E    msg_id, 
                                              DPARAM                param
                                              )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    static uint16           s_cur_list_index = 0;
    MMIACC_SMART_EVENT_T	record_smart_event_info = {0};

    if (MSG_TIMER != msg_id)
    {
        //SCI_TRACE_LOW:"[MMIFMRECORD] HandleFmTimerRecordMsg() msg_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_6052_112_2_18_2_19_40_92,(uint8*)"d", msg_id);
    }
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
#ifdef MMI_FM_TIMER_RECORD_SUPPORT            
            MMIAPIALM_FormInteEventInfo(EVENT_FM_TIMER_RECORD,&record_smart_event_info);			
#endif
            SetFMTimerRecordParam(&record_smart_event_info);
            GUILIST_SetCurItemIndex(MMIFM_TIMER_RECORD_LIST_CTRL_ID, 0);
            MMK_SetAtvCtrl(win_id, MMIFM_TIMER_RECORD_LIST_CTRL_ID);
        }
        break;

        case MSG_FULL_PAINT:
            break;
        case MSG_GET_FOCUS:
#ifdef MMI_FM_TIMER_RECORD_SUPPORT            
            MMIAPIALM_FormInteEventInfo(EVENT_FM_TIMER_RECORD,&record_smart_event_info);			
#endif
            SetFMTimerRecordParam(&record_smart_event_info);
            break;
        
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:     
        case MSG_APP_OK:
        case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {  
#ifdef MMI_FM_TIMER_RECORD_SUPPORT 
            SCI_MEMCPY((uint8 *)&record_smart_event_info.event_fast_info, 
               (uint8 *)MMIALM_GeEvent((uint16)EVENT_FM_TIMER_RECORD), 
               sizeof(MMIACC_SMART_EVENT_FAST_TABLE_T));
#endif
            s_cur_list_index = GUILIST_GetCurItemIndex(MMIFM_TIMER_RECORD_LIST_CTRL_ID); 
            
            switch(s_cur_list_index)
            {
                case 0:
                {
                	MMI_TEXT_ID_T    text_id = TXT_DISABLE_FM_RECORD;
                    MMI_TEXT_ID_T    hint_text_id = TXT_ENABLE_FM_RECORD_ALREADY;
                    
                    if(TRUE == record_smart_event_info.event_fast_info.is_on)
                    {
                        record_smart_event_info.event_fast_info.is_on = FALSE; 
                        text_id = TXT_ENABLE_FM_RECORD;
                        hint_text_id = TXT_DISABLE_FM_RECORD_ALREADY;
                    }
                    else
                    {
                        record_smart_event_info.event_fast_info.is_on = TRUE; 
                    }
#ifdef MMI_FM_TIMER_RECORD_SUPPORT                    
                    MMIAPIALM_SaveInteEventInfo(EVENT_FM_TIMER_RECORD,&record_smart_event_info);
#endif
#ifndef MMI_GUI_STYLE_MINISCREEN 
                    MMIFM_SetStringListItem(text_id,GUIITEM_STYLE_ONE_LINE_TEXT,
                         TXT_COMMON_OK,TXT_NULL,STXT_RETURN,
                         MMIFM_TIMER_RECORD_LIST_CTRL_ID,TRUE,0);
#else
                    MMIFM_SetStringListItem(text_id,GUIITEM_STYLE_ONE_LINE_TEXT,
                         TXT_COMMON_OK,IMAGE_NULL,STXT_RETURN,
                         MMIFM_TIMER_RECORD_LIST_CTRL_ID,TRUE,0);
#endif
#ifdef  MMI_PDA_SUPPORT
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#else
                    MMIPUB_OpenAlertSuccessWin(hint_text_id);
#endif
                }
                break;
                case 1:
                {
                    if(FALSE == record_smart_event_info.event_fast_info.is_on)
                    {
#ifdef MMI_PDA_SUPPORT
                        MMIPUB_DisplayTipsIDEx(PNULL, TXT_ENABLE_FM_RECORD_HINT, MMIPUB_TIPS_DIS_MIDDLE, PNULL);
#else
                        MMIPUB_OpenAlertWarningWin(TXT_ENABLE_FM_RECORD_HINT);
#endif
                    }
                    else
                    {                        
                        MMK_CreateWin((uint32 *)MMI_FM_SET_TIMER_INFO_WIN_TAB, PNULL);
                    }
                }
                break;
                default:
                break;
            }
        }
        break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
        break;
        default:
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    }
    return recode;
}
/*****************************************************************************/
// 	Description : Handle Fm Set Timer Info Msg
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFmSetTimerInfoMsg(
                                          MMI_WIN_ID_T      win_id, 	
                                          MMI_MESSAGE_ID_E  msg_id,
                                          DPARAM            param
                                          )
{
	MMI_RESULT_E        recode          = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T ctrl_id =MMIFM_EDIT_MODE_SETLIST_CTRL_ID;
    MMIFM_TIMER_RECORD_INFO_T record_info = {0};
    EVENT_CURRENT_DATA_T alarm_event={0};
#ifdef MMI_PDA_SUPPORT
    MMISET_TIME_T       time={0};
#endif
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
        MMIAPIALM_FormInteEventInfo(EVENT_FM_TIMER_RECORD,&alarm_event.event);
        s_alarm_hour=  alarm_event.event.event_fast_info.hour; 
        s_alarm_minute = alarm_event.event.event_fast_info.minute;
	    s_alarm_fre_mode = alarm_event.event.event_fast_info.fre_mode;
        s_alarm_fre = alarm_event.event.event_fast_info.fre;
#endif 
#ifndef MMI_PDA_SUPPORT
        //循环显示form控件中的各子项
		GUIFORM_SetCircularHandleUpDown(MMIFM_EDITWIN_FORM_CTRL_ID,TRUE);
#endif
//#ifdef MMI_FM_MINI_SUPPORT
        //修改风格，居左显示各项
        GUIFORM_SetType(MMIFM_EDITWIN_FORM_CTRL_ID,GUIFORM_TYPE_TP);
		GUIEDIT_SetAlign(MMIFM_EDIT_TIME_EDITBOX_CTRL_ID,ALIGN_LVMIDDLE);
	    GUISETLIST_SetFontAlign(MMIFM_EDIT_MODE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
//#endif
        MMIFM_GetTimerRecordInfo(&record_info);
#ifndef MMI_PDA_SUPPORT
        SetTimeParamEditWin(s_alarm_hour, s_alarm_minute);        
        SetRecordEndureParamEditWin(&record_info);
        SetfrequencyParamEditWin(&record_info);
#endif
        AddItemSetlist();
        SetModeParamEditWin(s_alarm_fre_mode);        
		SetWeeksParamEditWin(win_id, s_alarm_fre);	
		SetWeeksDisplayEditWin();
        MMK_SetAtvCtrl(win_id, MMIFM_EDIT_TIME_EDITBOX_CTRL_ID);
		break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
	     //MMIAPIALM_FormInteEventInfo(EVENT_FM_TIMER_RECORD,&alarm_event.event);
            ctrl_id = MMK_GetActiveCtrlId(win_id);	
            HandleFmWeeksBtnEditMsg(win_id,ctrl_id);
        }
		break;
	case MSG_FULL_PAINT:
#ifdef MMI_PDA_SUPPORT
        MMIFM_GetTimerRecordInfo(&record_info);
        SetTimeParamEditWin(s_alarm_hour, s_alarm_minute);        
        SetRecordEndureParamEditWin(&record_info);
        SetfrequencyParamEditWin(&record_info);
        if(ALM_MODE_EVERYWEEK==GUISETLIST_GetCurIndex(MMIFM_EDIT_MODE_SETLIST_CTRL_ID))
        {
            wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
            MMI_STRING_T str={0};
            
            //MMIAPIALM_FormInteEventInfo(EVENT_FM_TIMER_RECORD,&alarm_event.event);
            str.wstr_len=MMIAPIALARM_GetFreModeString(ALM_MODE_EVERYWEEK,s_alarm_fre,buff,GUILIST_STRING_MAX_NUM+1);
            str.wstr_ptr=buff;
            GUISETLIST_SetDispText(MMIFM_EDIT_MODE_SETLIST_CTRL_ID,&str,FALSE);
        }
#endif
		break;      
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
		ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
		HandleFmWeeksBtnEditMsg(win_id,ctrl_id);
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
		 ctrl_id = MMK_GetActiveCtrlId(win_id);
		 HandleEditFormSwitchActiveMsg(win_id,ctrl_id);
       	 break;
         
    case MSG_APP_OK:
    case MSG_CTL_OK:  
        MMIAPIALM_FormInteEventInfo(EVENT_FM_TIMER_RECORD,&alarm_event.event);
#ifdef MMI_PDA_SUPPORT        
        time.time_hour=  s_alarm_hour; 
        time.time_minute= s_alarm_minute;
        if (!MMIAPICOM_CheckTimeIsValid(time))
        {

            MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
            break;
        }
        MMIFM_GetTimerRecordInfo(&record_info);
        if (0 == record_info.endure_time)
    	{
    		MMIPUB_OpenAlertWarningWin(TXT_INVALID_ENDURE_TIME);
    		break;
    	}    
    	if(!MMIFM_IsValidFre(record_info.record_freq))	
    	{
            MMIPUB_OpenAlertWarningWin(TXT_FM_INVALID_FREQ);
		
            return FALSE;
    	}
#else
		//save start time
		if(!SaveTimeEditWin() ||!SaveTimerRecordInfo())
		{
			break;
		}
#endif
		//save mode
		SaveModeEditWin();
        SCI_TRACE_LOW("HandleFmSetTimerInfoMsg s_alarm_fre_mode %d", s_alarm_fre_mode);
        
		if(s_alarm_fre_mode == ALM_MODE_EVERYWEEK)
		{
			if(!SaveFreEditWin())
			{
				break;
			}
		}
        alarm_event.event.event_fast_info.hour = s_alarm_hour;
        alarm_event.event.event_fast_info.minute = s_alarm_minute;
        
        alarm_event.event.event_fast_info.fre_mode = s_alarm_fre_mode;
        alarm_event.event.event_fast_info.is_valid = TRUE;
	    alarm_event.event.event_fast_info.fre = s_alarm_fre;
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
              alarm_event.id = EVENT_FM_TIMER_RECORD;
#endif
        alarm_event.event.event_content_info.ring_type = ALM_FM_RING;
		// 提示窗口
		if (MMIAPIALM_SaveInteEventInfo(alarm_event.id,&alarm_event.event))
		{
#ifdef MMI_FM_NEED_HEADSET
#ifdef  MMI_PDA_SUPPORT
                MMIPUB_OpenAlertWarningWin(TXT_FM_TIMER_SAVE_PROMPT);
#else
                MMIPUB_OpenAlertSuccessWin(TXT_FM_TIMER_SAVE_PROMPT);
#endif
#endif
        }
		/*
#ifndef MAINLCD_SIZE_240X320		
		else
		{
                    MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
		} 	
#endif	
             */
		MMK_CloseWin(win_id);
        break;
        
	case MSG_CTL_CANCEL:
	case MSG_KEYDOWN_CANCEL:
        //s_fm_timer_record_event_info.event.event_fast_info.is_valid = FALSE;
		MMK_CloseWin(win_id);
	break;

	case MSG_CLOSE_WINDOW:
	break;
        
	case MSG_CTL_SETLIST_SWITCH:
        ctrl_id = MMK_GetActiveCtrlId(win_id);
     	if(MMIFM_EDIT_MODE_SETLIST_CTRL_ID==ctrl_id)
     	{
    		SetWeeksDisplayEditWin();
     	}
    break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
    return recode;
}
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetFMRecordFormParam(void)
{
    GUI_BG_T FormBg ={0};
    
    GUILABEL_SetFont(MMIFM_NAME_LABEL_CTRL_ID,  MMIFM_FILENAME_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
    GUILABEL_SetFont(MMIFM_TIME_OWNDRAW_CTRL_ID, MMIFMRECORD_TIME_FONT, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));

    FormBg.img_id = IMAGE_COMMON_BTN_BG;
    FormBg.bg_type = GUI_BG_IMG;
    GUIFORM_SetBg(MMIFM_BUTTON_FORM_CTRL_ID, &FormBg);
    
    SetFMRecordPortraitFormParam();
}
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetFMTimerRecordParam(MMIACC_SMART_EVENT_T *event_ptr)
{   
    MMI_TEXT_ID_T    text_id = TXT_ENABLE_FM_RECORD;

    if(PNULL == event_ptr)
    {
        return;
    }
    
    GUILIST_SetMaxItem(MMIFM_TIMER_RECORD_LIST_CTRL_ID, 2, FALSE );
    
    if(TRUE == event_ptr->event_fast_info.is_on)
    {  
        text_id = TXT_DISABLE_FM_RECORD;
    }  
#ifndef MMI_GUI_STYLE_MINISCREEN 
    MMIFM_SetStringListItem(text_id,GUIITEM_STYLE_ONE_LINE_TEXT,
                         TXT_COMMON_OK,TXT_NULL,STXT_RETURN,
                         MMIFM_TIMER_RECORD_LIST_CTRL_ID,FALSE,0);
    MMIFM_SetStringListItem(TXT_FM_TIME_FREQUENCY,GUIITEM_STYLE_ONE_LINE_TEXT,
                         TXT_COMMON_OK,TXT_NULL,STXT_RETURN,
                         MMIFM_TIMER_RECORD_LIST_CTRL_ID,FALSE,1);
#else
	MMIFM_SetStringListItem(text_id,GUIITEM_STYLE_ONE_LINE_TEXT,
		TXT_COMMON_OK,IMAGE_NULL,STXT_RETURN,
		MMIFM_TIMER_RECORD_LIST_CTRL_ID,FALSE,0);
    MMIFM_SetStringListItem(TXT_FM_TIME_FREQUENCY,GUIITEM_STYLE_ONE_LINE_TEXT,
		TXT_COMMON_OK,IMAGE_NULL,STXT_RETURN,
                         MMIFM_TIMER_RECORD_LIST_CTRL_ID,FALSE,1);
#endif
}
#endif
#ifdef MMIFM_SUPPORT_FM_RECORD
/*****************************************************************************/
//  Description : SetPortraitFormParam
//  Global resource dependence : none
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
LOCAL void SetFMRecordPortraitFormParam(void)
{
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    GUIFORM_SetChildDisplay(MMIFM_MAINPLAY_FORM_CTRL_ID,
                MMIFM_TIME_OWNDRAW_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
    GUIFORM_SetChildDisplay(MMIFM_MAINPLAY_FORM_CTRL_ID,
                MMIFM_NAME_LABEL_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
    GUIFORM_SetChildDisplay(MMIFM_MAINPLAY_FORM_CTRL_ID,
                MMIFM_FORM_ANIM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
    GUIFORM_SetChildDisplay(MMIFM_MAINPLAY_FORM_CTRL_ID,
                MMIFM_TIME_FORM_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);

    control_info.is_ctrl_id = TRUE;    
    control_info.ctrl_ptr = PNULL;
    control_info.ctrl_id = MMIFM_FORM_ANIM_CTRL_ID;
    data_info.img_id = IMAGE_COMMON_RECORD_BG;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
}
/*****************************************************************************
  Description : display record time
  Global resource dependence : none
  Author: 
  Note:
*****************************************************************************/
LOCAL void FmRecordDisplayRecordTime(void)
{
    MMIFMRECORD_TIME_T  cur_time = {0};
    MMI_STRING_T string = {0};
    MMIFMRECORD_STATE_E status = MMIFM_GetRecordStatus();
    wchar            wstrRecord_Time[MMIFMRECORD_TIME_STRING_LENGTH] = {0};
    uint8            strRecord_Time[MMIFMRECORD_TIME_STRING_LENGTH] = {0};
    int32 str_len = 0;
    
    switch(status) 
    {
        case MMIFMRECORD_STATE_NONE:
        {
            s_record_cur_timer_elapsed = 0;
        }
        break;

        case MMIFMRECORD_STATE_RECORDING:
        case MMIFMRECORD_STATE_RECORD_PAUSED:
        {
            s_record_cur_timer_elapsed = MMIFM_GetRecTime() / 1000 + 1;
        }
        break;

        default:
        {
            s_record_cur_timer_elapsed = MMIFM_GetRecTime() / 1000 + 1;
        }
        break;
    }

    FMGetRecordTime(&cur_time);

    sprintf((char *)strRecord_Time,"%d%d:%d%d:%d%d",cur_time.hour_high, cur_time.hour_low,cur_time.minute_high,cur_time.minute_low,cur_time.second_high,cur_time.second_low);
    str_len = strlen((char*)strRecord_Time);
    str_len = MIN(MMIFMRECORD_TIME_STRING_LENGTH, str_len);
    MMI_STRNTOWSTR(wstrRecord_Time, MMIFMRECORD_TIME_STRING_LENGTH, strRecord_Time, MMIFMRECORD_TIME_STRING_LENGTH, str_len);

    string.wstr_len = MMIFMRECORD_TIME_STRING_LENGTH;
    string.wstr_ptr = wstrRecord_Time;

    GUILABEL_SetText(MMIFM_TIME_OWNDRAW_CTRL_ID, &string, TRUE);
}
/*****************************************************************************/
//  Description : display record
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FMRecordUpdateScreen(void)
{
    GUI_RECT_T                      anim_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    GUIRES_DisplayImg(PNULL,
        &anim_rect,
        &anim_rect,
        FM_RECORD_WIN_ID,
        IMAGE_COMMON_BG,
        &lcd_dev_info);
    
    FmRecordDisplayRecordTime();
}
/*****************************************************************************/
//  Description : calculate time of MMIFMRECORD_TIME_T
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void FMGetRecordTime(MMIFMRECORD_TIME_T* time_ptr)                     
{
    uint32  total_hours = (s_record_cur_timer_elapsed / 3600);
    uint32  total_minutes = ((s_record_cur_timer_elapsed % 3600) / 60);
    uint32  total_seconds = ((s_record_cur_timer_elapsed % 3600) % 60);
    
    if (PNULL != time_ptr)
    {
        time_ptr->second_low = (uint8)(total_seconds % 10);
        time_ptr->second_high = (uint8)(total_seconds / 10);
        time_ptr->minute_low = (uint8)(total_minutes % 10);
        time_ptr->minute_high = (uint8)(total_minutes / 10);
        time_ptr->hour_low = (uint8)(total_hours % 10);
        time_ptr->hour_high = (uint8)(total_hours / 10);
    }
    else
    {
        //SCI_TRACE_LOW:"FMGetRecordTime fail ! time_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_6462_112_2_18_2_19_40_93,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : start record timer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void FMStartDisplayProgressTimer(void)
{
    s_time_timer_id = MMK_CreateWinTimer(FM_RECORD_WIN_ID, 300, FALSE);
}

/*****************************************************************************/
//  Description : stop record timer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void FMStopDisplayProgressTimer(void)
{
    if(0 != s_time_timer_id)
    {
        MMK_StopTimer(s_time_timer_id);
        s_time_timer_id = 0;
    }
}
/*****************************************************************************/
//  Description : start FM record
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFMRECORD_RESULT_E FMRecordStartRecord(void)
{
    MMIFMRECORD_RESULT_E error_code = MMIFMRECORD_RESULT_SUCCESS;
    MMIFILE_ERROR_E   fs_error = SFS_ERROR_NO_SPACE;
    
    //SCI_TRACE_LOW:"[MMIFM] FMRecordStartRecord()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_6496_112_2_18_2_19_40_94,(uint8*)"");
    
    if (!MMIAPIENG_GetIQModeStatus())
    {
        fs_error = MMIFM_StartRecord();
    }
    else
    {
        error_code = MMIFMRECORD_RESULT_ERROR;
        MMIAPIENG_AlertIQMode();
        return error_code;
    }
    
    switch (fs_error)
    {
    case SFS_ERROR_NONE:
        MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_RECORDING);
        error_code = MMIFMRECORD_RESULT_SUCCESS;
        break;
        
    case SFS_ERROR_NO_SPACE:
        MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_NONE);
        error_code = MMIFMRECORD_RESULT_NO_SPACE;
        break;
        
    case SFS_ERROR_FOLD_FULL:
        MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_NONE);
        error_code = MMIFMRECORD_RESULT_MAX_FILE;
        break;
        
    case SFS_ERROR_NOT_EXIST:
        MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_NONE);        
        error_code = MMIFMRECORD_RESULT_DEVICE_NOT_EXIST;
        break;
        
    default:
        MMIFM_RecordSetRecordStatus(MMIFMRECORD_STATE_NONE);
        error_code = MMIFMRECORD_RESULT_ERROR;
        break;
    }
    //SCI_TRACE_LOW:"[MMIFM] FMRecordStartRecord() fs_error %d,error_code %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_6536_112_2_18_2_19_41_95,(uint8*)"dd",fs_error,error_code);
    return error_code;
}
/*****************************************************************************/
//  Description : SetLabelText
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void FMRecordSetLabelText(void)
{
    MMI_STRING_T cur_file_s = {0};
    wchar     *full_path_ptr = PNULL;
    uint16   path_len = 0;

    cur_file_s.wstr_ptr = SCI_ALLOC_APP((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
    SCI_MEMSET(cur_file_s.wstr_ptr, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
    full_path_ptr = SCI_ALLOC_APP((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
    SCI_MEMSET(full_path_ptr, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));

    MMIAPICOM_Wstrcpy(full_path_ptr,MMIFM_GetRecordFilePath());
    
    path_len = MMIAPICOM_Wstrlen(full_path_ptr);

    MMIAPIFMM_SplitLastName(full_path_ptr, &path_len, cur_file_s.wstr_ptr, &cur_file_s.wstr_len);

    GUILABEL_SetText(MMIFM_NAME_LABEL_CTRL_ID, &cur_file_s, FALSE);

    if(PNULL != cur_file_s.wstr_ptr)
    {
        SCI_FREE(cur_file_s.wstr_ptr);
        cur_file_s.wstr_ptr = PNULL;
    }

    SCI_FREE(full_path_ptr);
}
/*****************************************************************************/
//  Description : handle app ok msg of record
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void HandleFMRecordAppOkMsg(MMI_WIN_ID_T   win_id)
{
    MMI_TEXT_ID_T left_text_id[] = {TXT_PAUSE,TXT_COMMON_COMMON_CONTINUE};
    switch (MMIFM_GetRecordStatus())
    {
        case MMIFMRECORD_STATE_RECORDING:
        {
            MMIFM_RecordPauseRecord(FALSE);
            GUIWIN_SetSoftkeyTextId(win_id, left_text_id[1], TXT_NULL, STXT_STOP, TRUE);
            FMStopDisplayProgressTimer(); 
        }
        break;
        case MMIFMRECORD_STATE_RECORD_PAUSED:
        {
            MMIFM_RecordResumeRecord(FALSE);
            GUIWIN_SetSoftkeyTextId(win_id, left_text_id[0], TXT_NULL, STXT_STOP, TRUE);   
            FMStartDisplayProgressTimer();
        }       
        break;
        default:
        break;
    }
    FMRecordUpdateScreen();
}
/*****************************************************************************/
// Description : Fm Record Rename
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmRecordRenameWinMsg(
                                         MMI_WIN_ID_T         win_id,
                                         MMI_MESSAGE_ID_E     msg_id,
                                         DPARAM               param
                                         )
{   
    MMI_RESULT_E      result = FALSE;
    MMI_CTRL_ID_T     ctrl_id = MMIFM_RECORD_RENAME_EDITBOX_CTRL_ID;
    MMI_STRING_T      name_str = {0};   
    wchar             *buffer_ptr = PNULL;
    wchar             *old_path_ptr = PNULL;
    MMI_STRING_T      cur_file_s = {0};
    uint16            slash_index=0;
    uint32            full_path_len = 0;
    uint32            old_path_len = 0;
    uint32            edit_ctrl_len = 0;
    BOOLEAN           is_rename_success = FALSE;
    uint16            file_name_len = MMIFMRECORD_MAX_FILE_NAME_LEN;
    wchar             *new_file_name = PNULL;
    uint16            new_file_name_len = 0;
    
        
    //SCI_TRACE_LOW:"MMIFM: HandleFmRecordRenameWinMsg, win_id =0x%x, msg_id = 0x%x."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_6624_112_2_18_2_19_41_96,(uint8*)"dd", win_id, msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {               
            old_path_ptr = MMIFM_GetRecordFilePath();            
            old_path_len = MMIAPICOM_Wstrlen(old_path_ptr); 
            
            cur_file_s.wstr_ptr = SCI_ALLOC_APP((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar));
            SCI_MEMSET(cur_file_s.wstr_ptr, 0, ((MMIFMRECORD_MAX_FILE_NAME_LEN + 1)* sizeof(wchar)));
            
            
            MMIAPIFMM_SplitFullPathExt(old_path_ptr,old_path_len, 
                        PNULL,PNULL,
                        cur_file_s.wstr_ptr, &file_name_len,
                        PNULL,PNULL); 
           
            edit_ctrl_len = MMIFMRECORD_MAX_FILE_NAME_LEN-old_path_len+file_name_len;
            GUIEDIT_SetMaxLen(ctrl_id,edit_ctrl_len);
            GUIEDIT_SetString(ctrl_id, cur_file_s.wstr_ptr , file_name_len); 
            GUIEDIT_SetSoftkey(ctrl_id, 0, 1, TXT_NULL, TXT_COMMON_OK, PNULL);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            
            if(PNULL != cur_file_s.wstr_ptr)
            {
                SCI_FREE(cur_file_s.wstr_ptr);
                cur_file_s.wstr_ptr = PNULL;
            }           

//#ifdef MMI_PDA_SUPPORT
//            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
//#endif
        }
        break;      
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK,  PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMIPUB_OpenQueryWinByTextId(TXT_FM_CANCEL_RECORD,IMAGE_PUBWIN_QUERY,PNULL,HandleCancelRenameWinMsg);
                break;            
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:     
        case MSG_APP_OK:
        case MSG_CTL_OK: 
        //case MSG_APP_MENU: 
        {
            GUIEDIT_GetString(ctrl_id, &name_str);
            if (0 ==name_str.wstr_len)
            {
                break;
            }
            if (MMIAPICOM_FileNameIsValid(&name_str))
            {
                buffer_ptr = (wchar *)SCI_ALLOC_APP(sizeof(wchar)*(MMIFMRECORD_MAX_FILE_NAME_LEN+1));
                SCI_MEMSET(buffer_ptr,0x00,(sizeof(wchar)*(MMIFMRECORD_MAX_FILE_NAME_LEN+1)));
                new_file_name = (wchar *)SCI_ALLOC_APP(sizeof(wchar)*(MMIFMRECORD_MAX_FILE_NAME_LEN+1));
                SCI_MEMSET(buffer_ptr,0x00,(sizeof(wchar)*(MMIFMRECORD_MAX_FILE_NAME_LEN+1)));

                old_path_ptr = MMIFM_GetRecordFilePath();
                //SCI_ASSERT(PNULL != old_path_ptr);/*assert verified*/
                if(PNULL == old_path_ptr)
                {
                    break;
                }
                old_path_len =  MMIAPICOM_Wstrlen(old_path_ptr);
                
                slash_index = MMIFM_RecordGetFinallySlashIndex(old_path_ptr,old_path_len);
                MMIAPICOM_Wstrncpy(buffer_ptr,old_path_ptr, slash_index + 1 );
                MMIAPICOM_Wstrcpy(new_file_name,name_str.wstr_ptr);
#ifdef MMIFM_SUPPORT_FORMAT_AMR
                if(MMISRVAUD_RECORD_FMT_AMR == MMIFMRECORD_GetFileFormat())
                {
                    MMIAPICOM_Wstrcat(new_file_name,L".amr");
                }
                else
#endif          
                {                
                    MMIAPICOM_Wstrcat(new_file_name,L".wav");
                }
                MMIAPICOM_Wstrcat(buffer_ptr,new_file_name);                
                full_path_len = MMIAPICOM_Wstrlen(buffer_ptr);
                new_file_name_len = MMIAPICOM_Wstrlen(new_file_name);
                
                if(!MMIFM_CheckRecordNameBeModified(name_str.wstr_ptr) 
                    || !MMIAPIFMM_IsFileExist(buffer_ptr,full_path_len))
                {
                    is_rename_success = MMIAPIFMM_RenameFile(old_path_ptr, old_path_len, new_file_name, new_file_name_len);
                    if(is_rename_success)
                    {
#ifndef  MMI_PDA_SUPPORT
                     //   MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
#endif
                        MMIFM_RecordDestructor();
                        MMK_CloseWin(FM_RECORD_WIN_ID);
                        MMK_CloseWin(win_id);                       
                    }
                    else
                    {
                        MMIPUB_OpenAlertFailWin(TXT_ERROR);
                    }   
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL, TXT_FILE_EXISTED, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
                            MMIPUB_SOFTKEY_ONE, PNULL);                 
                }

                SCI_FREE(buffer_ptr);
                SCI_FREE(new_file_name);
                buffer_ptr = PNULL;
                new_file_name = PNULL;
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_DC_NAMEINVALID);
                break;
            }           
        }
        break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {               
            MMIPUB_OpenQueryWinByTextId(TXT_FM_CANCEL_RECORD,IMAGE_PUBWIN_QUERY,PNULL,HandleCancelRenameWinMsg);
        }
        break;
        default:        
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Discription: Handle Cancel Rename Win Msg 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCancelRenameWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)    
{
    MMI_RESULT_E         result = MMI_RESULT_TRUE;
    MMIFMRECORD_RESULT_E  deletefileresult =MMIFMRECORD_RESULT_SUCCESS;
    
    switch(msg_id) 
    { 
        
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {   
            deletefileresult = MMIFM_RecordDeleteTempFile();
            if(MMIFMRECORD_RESULT_ERROR == deletefileresult)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_FM_RECORD_FILE_NOT_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
                    MMIPUB_SOFTKEY_ONE, PNULL);
            }
            MMIFM_RecordDestructor();
            MMK_CloseWin(FM_RECORD_WIN_ID);
            MMK_CloseWin(MMIFM_RECORD_RENAME_WIN_ID);
            MMK_CloseWin(win_id);
        }
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
#ifdef  MMI_PDA_SUPPORT            
            MMK_CloseWin(FM_RECORD_WIN_ID);
            MMK_CloseWin(MMIFM_RECORD_RENAME_WIN_ID);
#endif                       
            MMK_CloseWin(win_id);
        }
        break;
                
        default:
        {
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        }
        break;
    }
    return result;
}
#endif
/*****************************************************************************/
//  Description : Display fm volume images.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFMVolumePanelOpen(void)
{
    return MMIAPICOM_IsPanelOpened();
}   

/*****************************************************************************/
//  Description : udpate the button bg according the valid channel
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FMMainUpdateScreen(void)
{ 
#ifdef FM_NEW_UI
    if(!MMIFM_CheckHeadsetStatusWithoutPrompt())
    {
        GUIBUTTON_SetGrayed(MMIFM_GET_UPFRE_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
        GUIBUTTON_SetGrayed(MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
    }
    else
    {
        GUIBUTTON_SetGrayed(MMIFM_GET_UPFRE_BUTTON_CTRL_ID,FALSE,NULL,TRUE);
        GUIBUTTON_SetGrayed(MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,FALSE,NULL,TRUE);
    }
#endif    
}
/*****************************************************************************/
//  Description : udpate the button bg according the FM start way
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FMMainUpdateButtonBg(void)
{
#ifdef FM_NEW_UI
    GUIBUTTON_SetGrayed(MMIFM_GET_UPFRE_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
    GUIBUTTON_SetGrayed(MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
    GUIBUTTON_SetGrayed(MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
    GUIBUTTON_SetGrayed(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
	GUIBUTTON_SetGrayed(MMIFM_LIST_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
#else
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
    GUIBUTTON_SetGrayed(MMIFM_ADJ_FREQ_LEFT_BUTTON_ID,TRUE,NULL,TRUE);
    GUIBUTTON_SetGrayed(MMIFM_ADJ_FREQ_RIGHT_BUTTON_ID,TRUE,NULL,TRUE);
#endif
    GUIBUTTON_SetGrayed(MMIFM_GET_UPFRE_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
    GUIBUTTON_SetGrayed(MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
#endif
    GUIBUTTON_SetGrayed(MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
    GUIBUTTON_SetGrayed(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,TRUE,NULL,TRUE);
#endif
}
/*****************************************************************************/
//  Description : Set FM StartSuspend Button BG when be suspended
//  Global resource dependence : 
//  Parameter: Set Button Status
//  Note: 
/*****************************************************************************/
LOCAL void SetButtonBgWhenResume(void)
{
    GUI_BG_T btn_bg = {0};  
    if(FM_SUSPENDED == MMIAPIFM_GetStatus() )
    {
#ifdef FM_NEW_UI
        btn_bg.img_id  = IMAGE_FM_AJUST_PLAY;
#else
        btn_bg.img_id  = IMAGE_FM_PAUSE_DEFAULT;
#endif
        btn_bg.bg_type = GUI_BG_IMG;
        GUIBUTTON_SetBg(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,&btn_bg);
        MMIAPIFM_Resume(TRUE);
    }
}
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
/*****************************************************************************/
//  Description : Set FM volume Button BG when volume change to 0
//  Global resource dependence : 
//  Parameter: Set Button Status
//  Note: 
/*****************************************************************************/
LOCAL void SetVolumeBgWhenZero(void)
{
    GUI_BG_T btn_bg = {0};
    GUI_BG_T btn_pressed_bg = {0};
    uint8 current_volume = MMIAPISET_GetMultimVolume();
    btn_bg.bg_type = GUI_BG_IMG;
    btn_pressed_bg.bg_type = GUI_BG_IMG;
#ifdef FM_NEW_UI
    if(MMIFM_VOL_LEVEL_0 == current_volume)
    {
        btn_bg.img_id  = IMAGE_FM_SILENCE_DEFAUT;
        btn_pressed_bg.img_id = IMAGE_FM_SILENCE_PRESSED;
    }
    else
    {
        btn_bg.img_id  = IMAGE_FM_VOLUME_DEFAUT;
        btn_pressed_bg.img_id = IMAGE_FM_VOLUME_PRESSED;
    }
#else
    if(MMIFM_VOL_LEVEL_0 == current_volume)
    {
        btn_bg.img_id  = IMAGE_FM_SILENCE_DEFAULT;
        btn_pressed_bg.img_id = IMAGE_FM_SILENCE_PRESSED;
    }
    else
    {
        btn_bg.img_id  = IMAGE_FM_VOLUME_DEFAULT;
        btn_pressed_bg.img_id = IMAGE_FM_VOLUME_PRESSED;
    }
#endif
    GUIBUTTON_SetBg(MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID,&btn_bg);
    GUIBUTTON_SetPressedBg(MMIFM_ADJ_VOLUMN_BUTTON_CTRL_ID, &btn_pressed_bg);
}
#endif
/*****************************************************************************/
//  Description : Handle FM Alarm prview 
//  Global resource dependence : 
//  Parameter: 
//  Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleAlarmChannelprview(MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE; 
    switch(msg_id)
    {
        case MSG_LOSE_FOCUS:
            MMK_CloseWin(FM_MAIN_WIN_ID);
            break;
        case MSG_GET_FOCUS:
        case MSG_NOTIFY_MIDSK:
        case MSG_APP_WEB:
        case MSG_KEYUP_WEB:
        case MSG_APP_UP:
        case MSG_APP_DOWN:
        case MSG_APP_LEFT:
        case MSG_APP_RIGHT:
        case MSG_KEYUP_LEFT:
        case MSG_KEYUP_RIGHT:
        case MSG_KEYLONG_LEFT:
        case MSG_KEYLONG_RIGHT:
        case MSG_CTL_OK:
        case MSG_APP_OK:
#ifdef FM_NEW_UI
        case MSG_APP_MENU:
#endif
        case MSG_APP_0:
        case MSG_APP_1:
        case MSG_APP_2:
        case MSG_APP_3:
        case MSG_APP_4:
        case MSG_APP_5:
        case MSG_APP_6:
        case MSG_APP_7:
        case MSG_APP_8:
        case MSG_APP_9:
        case MSG_TIMER:
#ifdef FM_NEW_UI
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DOWN:
        case MSG_TP_PRESS_MOVE:
        case MSG_TP_PRESS_UP:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
        result = MMI_RESULT_FALSE;
            break;


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#ifdef FM_NEW_UI                
            if(NULL == param) break;
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {                
            case MMIFM_RETURN_BUTTON_CTRL_ID:
                MMIAPICOM_ClosePanelChildWin();             
                MMK_CloseWin(win_id);
                break;
            default:
                break;
            }
#else
            result = MMI_RESULT_FALSE;
#endif            
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
        default:
            break;
    }
    return result;
}
/*****************************************************************************/
//  Description : recorder exit
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMRECORD_Exit(void)
{
#ifdef MMIFM_SUPPORT_FM_RECORD
    if(MMK_IsOpenWin(FM_RECORD_WIN_ID))
    {
        MMK_CloseWin(FM_RECORD_WIN_ID);
    }
#endif
}
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
// 	Description : set time ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetTimeParamEditWin(uint8 hour,
                                                             uint8 minute)
{
#ifndef MMI_PDA_SUPPORT
	GUIEDIT_TIME_STYLE_E time_style =GUIEDIT_TIME_STYLE_24;
	MMISET_TIME_DISPLAY_TYPE_E time_type=MMISET_TIME_24HOURS;
#endif
	MMI_STRING_T        time_text            = {0};

    GUIFORM_SetStyle(MMIFM_EDIT_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	
	MMI_GetLabelTextByLang(TXT_START_TIME, &time_text);
	GUILABEL_SetText(MMIFM_EDIT_TIME_LABEL_CTRL_ID, &time_text, FALSE);

#ifdef MMI_PDA_SUPPORT
    {
        uint8               time_str[MMIALM_TIME_STR_12HOURS_LEN + 1]    = {0};

        MMI_STRING_T        text            = {0};

        
    	MMIAPISET_FormatTimeStrByTime(hour,
    		minute,
    		time_str,
    		MMIALM_TIME_STR_12HOURS_LEN+1
    		);
        

        text.wstr_len = 10;
        text.wstr_ptr = SCI_ALLOC_APP((text.wstr_len +1)* sizeof(wchar));
        SCI_MEMSET(text.wstr_ptr,0,((text.wstr_len +1)* sizeof(wchar)));
        MMI_STRNTOWSTR(text.wstr_ptr,10,(uint8*)time_str,10,10);

        GUILABEL_SetText(MMIFM_EDIT_TIME_EDIT_LABEL_CTRL_ID, &text, FALSE);
        if (PNULL != text.wstr_ptr)
        {
            SCI_FREE(text.wstr_ptr);
            text.wstr_ptr = PNULL;
        }

    }
#else
	time_type = MMIAPISET_GetTimeDisplayType();
	if (MMISET_TIME_12HOURS == time_type)
	{
		time_style = GUIEDIT_TIME_STYLE_12;
	}
	else
	{
		time_style = GUIEDIT_TIME_STYLE_24;
	}

	//set time display style
	GUIEDIT_SetTimeStyle(MMIFM_EDIT_TIME_EDITBOX_CTRL_ID,PNULL,&time_style,PNULL,FALSE);

	GUIEDIT_SetTime(MMIFM_EDIT_TIME_EDITBOX_CTRL_ID,
						hour, 
						minute,
						0);
#endif
}
/*****************************************************************************/
// 	Description : set frequency ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetfrequencyParamEditWin(const MMIFM_TIMER_RECORD_INFO_T *record_info_ptr)
{
	MMI_STRING_T    text = {0};
    int16           frequency = 0;
    uint8 num_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
    wchar wnum_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
#ifndef MMI_PDA_SUPPORT
    wchar  dot_str[] = {'.'};
#endif
    GUIFORM_SetStyle(MMIFM_EDIT_FREQUENCY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);	
    
	MMI_GetLabelTextByLang(TXT_FM_RECORD_FREQUENCY, &text);
	GUILABEL_SetText(MMIFM_EDIT_FREQUENCY_LABEL_CTRL_ID, &text, FALSE);    
#ifdef MMI_PDA_SUPPORT
    frequency = record_info_ptr->record_freq;
    if (0 != frequency)
	{
        sprintf((char*)num_buf, "%d.%d",frequency/10, frequency%10);
        text.wstr_len = strlen((char*)num_buf);
        text.wstr_len = MIN(MMIFM_CHANNEL_NAME_LEN, text.wstr_len);
        text.wstr_ptr = wnum_buf;
        MMI_STRNTOWSTR(text.wstr_ptr, MMIFM_CHANNEL_NAME_LEN, num_buf, MMIFM_CHANNEL_NAME_LEN, text.wstr_len);
	}
    else
    {
        text.wstr_len = 5;
        text.wstr_ptr = L"87.5";
    }
    GUILABEL_SetText(MMIFM_EDIT_FREQUENCY_EDIT_LABEL_CTRL_ID, &text, FALSE);
#else
    GUIEDIT_SetIm(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
    GUIEDIT_SetStyle(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID, GUIEDIT_STYLE_SINGLE);
    GUIEDIT_SetAlign(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID, ALIGN_LVMIDDLE);
    GUIEDIT_ConfigImCustomKey(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
    GUIEDIT_SetImTag(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID, GUIIM_TAG_DOT_NUM);

    frequency = record_info_ptr->record_freq;
    if (0 != frequency)
	{
        sprintf((char*)num_buf, "%d.%d",frequency/10, frequency%10);
        text.wstr_len = strlen((char*)num_buf);
        text.wstr_len = MIN(MMIFM_CHANNEL_NAME_LEN, text.wstr_len);
        text.wstr_ptr = wnum_buf;
        MMI_STRNTOWSTR(text.wstr_ptr, MMIFM_CHANNEL_NAME_LEN, num_buf, MMIFM_CHANNEL_NAME_LEN, text.wstr_len);
	}
    else
    {
        text.wstr_len = 5;
        text.wstr_ptr = L"87.5";
    }
    GUIEDIT_SetString(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID, text.wstr_ptr, text.wstr_len);
#endif
}
#if 0
/*****************************************************************************/
// 	Description : get frequency ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN savefrequency(void)
{
	MMI_STRING_T    str = {0};
	
    GUIEDIT_GetString(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID,&str);
	if(MMIAPIALM_IsValidFM(str.wstr_ptr, str.wstr_len))
	{
		s_fm_timer_record_event_info.event.event_content_info.ring_type = ALM_FM_RING;
		s_fm_timer_record_event_info.event.event_ring_info.dring_name_len = str.wstr_len;

		SCI_MEMSET(s_fm_timer_record_event_info.event.event_ring_info.dring_name_arr, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)* sizeof(wchar)));
		MMI_WSTRNCPY(s_fm_timer_record_event_info.event.event_ring_info.dring_name_arr, MMIFILE_FULL_PATH_MAX_LEN,
		str.wstr_ptr, str.wstr_len, str.wstr_len);
		s_fm_timer_record_event_info.event.event_content_info.ring = MMISET_MAX_RING_ID;
	}
	else
	{
		MMIPUB_OpenAlertWarningWin(TXT_FM_INVALID_FREQ);
        return FALSE;
	}
    return TRUE;
}
#endif
/*****************************************************************************/
// 	Description : set mode ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetModeParamEditWin(MMIACC_MODE_TYPE_E	fre_mode)
{
	MMI_STRING_T        text            = {0};

    GUIFORM_SetStyle(MMIFM_EDIT_MODE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
	
	MMI_GetLabelTextByLang(TXT_ALARM_EDIT_MODE, &text);
	GUILABEL_SetText(MMIFM_EDIT_MODE_LABEL_CTRL_ID, &text, FALSE);

	GUISETLIST_SetCurIndex(MMIFM_EDIT_MODE_SETLIST_CTRL_ID,
							fre_mode);
    
    GUISETLIST_SetTitleTextId(MMIFM_EDIT_MODE_SETLIST_CTRL_ID, TXT_ALARM_EDIT_MODE);
}
/*****************************************************************************/
// 	Description : set  weeks ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetWeeksParamEditWin(MMI_WIN_ID_T      win_id,uint8 fre)
{
    uint8 day_num=0;
    uint8   week_day =0;
    uint32 ctrl_id[]={	MMIFM_EDITWIN_BUTTON0_CTRL_ID,
      	MMIFM_EDITWIN_BUTTON1_CTRL_ID,
      	MMIFM_EDITWIN_BUTTON2_CTRL_ID,
      	MMIFM_EDITWIN_BUTTON3_CTRL_ID,
      	MMIFM_EDITWIN_BUTTON4_CTRL_ID,
      	MMIFM_EDITWIN_BUTTON5_CTRL_ID,
      	MMIFM_EDITWIN_BUTTON6_CTRL_ID,
    };
    GUIFORM_CHILD_WIDTH_T  width={0}; 
    GUI_BG_T  bg={0};
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};
    uint16 hor_space = 0;
    uint16 image_width = 0;
    uint16 image_height = 0;
    GUI_BORDER_T border = {0};
	uint16 week_day_image_adjust = 0;
#ifdef MMI_GUI_STYLE_MINISCREEN
	uint16 space_sub = MMITHEME_SLIDEWIN_SCROLLBAR_WIDTH-1;
#else
	uint16 space_sub = 1;
#endif
   
    bg.bg_type=GUI_BG_IMG;
    enabled_font.color=MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    enabled_font.font=SONG_FONT_14;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    disabled_font.font=SONG_FONT_14;
#else
    disabled_font.font=SONG_FONT_16;
#endif
    
    GUIFORM_SetStyle(MMIFM_EDITWIN_CHILD_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_CLOCK_ALARM_WEEK_ENABLED, win_id);

    width.type=GUIFORM_CHILD_WIDTH_FIXED;
    width.add_data = (MMI_MAINSCREEN_WIDTH- space_sub * 2) / ALM_WEEK_DAY_NUM;
    GUIFORM_SetSpace(MMIFM_EDITWIN_CHILD_FORM_CTRL_ID,&hor_space,PNULL);
    GUIFORM_SetMargin(MMIFM_EDITWIN_CHILD_FORM_CTRL_ID,0);
    GUIFORM_PermitChildFont(MMIFM_EDITWIN_CHILD_FORM_CTRL_ID, FALSE);    
    border.type = GUI_BORDER_NONE;
    IGUICTRL_SetBorder(MMK_GetCtrlPtr(MMIFM_EDITWIN_CHILD_FORM_CTRL_ID), &border);
    
    for(day_num = 0; day_num < ALM_WEEK_DAY_NUM; day_num++)   
    {
		if ((width.add_data * ALM_WEEK_DAY_NUM + week_day_image_adjust) < (MMI_MAINSCREEN_WIDTH- space_sub * 2))
		{
			width.add_data = width.add_data + 1;
			week_day_image_adjust++;
		}
        week_day = MMIAPISET_GetWeekDayByOffset(day_num );
        GUIBUTTON_SetTextAlign(ctrl_id[day_num],ALIGN_HVMIDDLE);
        GUIBUTTON_SetTextId(ctrl_id[day_num],s_alm_week_text_id[ week_day ]);
        if(0!=(fre & s_week_mask[week_day]) ) 
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_ENABLED;
            s_weeks_button_is_selected[day_num]=TRUE;            
            GUIBUTTON_SetFont(ctrl_id[day_num], &enabled_font);
        }
        else
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_DISABLED;
            s_weeks_button_is_selected[day_num]=FALSE;
            GUIBUTTON_SetFont(ctrl_id[day_num], &disabled_font);
        }
        GUIBUTTON_SetBg(ctrl_id[day_num],&bg);
        GUIBUTTON_PermitBorder(ctrl_id[day_num],FALSE);
        GUIBUTTON_SetOwnerDraw(ctrl_id[day_num],ButtonOwnerDrawFun);
        bg.img_id=IMAGE_CLOCK_ALARM_WEEK_PRESSED;
        GUIBUTTON_SetRunSheen(ctrl_id[day_num],FALSE);
        GUIBUTTON_SetPressedBg(ctrl_id[day_num],&bg);        
        GUIFORM_SetChildWidth(MMIFM_EDITWIN_CHILD_FORM_CTRL_ID,ctrl_id[day_num],&width);
		width.add_data = (MMI_MAINSCREEN_WIDTH - space_sub * 2) / ALM_WEEK_DAY_NUM;
    }
}
/*****************************************************************************/
// 	Description : set  weeks display
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetWeeksDisplayEditWin(void)
{
	uint32 setlist_index=0;

    GUIFORM_SetCircularHandleLeftRight(MMIFM_EDITWIN_CHILD_FORM_CTRL_ID,TRUE);

	setlist_index=GUISETLIST_GetCurIndex(MMIFM_EDIT_MODE_SETLIST_CTRL_ID);
	if(ALM_MODE_EVERYWEEK!=setlist_index)
	{
		
		GUIFORM_SetChildDisplay(MMIFM_EDITWIN_FORM_CTRL_ID,
							MMIFM_EDITWIN_CHILD_FORM_CTRL_ID,
							GUIFORM_CHILD_DISP_HIDE);
	}
	else
	{
		
		GUIFORM_SetChildDisplay(MMIFM_EDITWIN_FORM_CTRL_ID, 
							MMIFM_EDITWIN_CHILD_FORM_CTRL_ID,
							GUIFORM_CHILD_DISP_NORMAL);
		
	}
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
	uint32 ctrl_id=MMIFM_EDITWIN_BUTTON0_CTRL_ID;
	GUI_RECT_T rect={0};//max_rect与owner_draw_ptr->display_rect相交区域，也即图片最后显示区域
	GUI_RECT_T max_rect={0};//图片可以显示的最大区域
	GUI_RECT_T img_rect={0};//图片裁剪区
	MMI_HANDLE_T win_handle = 0;
	MMI_WIN_ID_T    win_id=0;
	uint16 image_w=0;
	uint16 iamge_h=0;

	ctrl_id=MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);
	win_handle=MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
	win_id=MMK_GetWinId(win_handle);
	GUIRES_GetImgWidthHeight(&image_w, &iamge_h, IMAGE_CLOCK_ALARM_WEEK_MARK, win_id);

	max_rect.left=owner_draw_ptr->display_x;
	max_rect.top=owner_draw_ptr->display_y;
	max_rect.right=max_rect.left+image_w;
	max_rect.bottom=max_rect.top+iamge_h;
	
	if(!GUI_IntersectRect(&rect,max_rect,owner_draw_ptr->display_rect))
	{//没有相交区域，返回
		return ;
	}

	img_rect.left=rect.left-max_rect.left;
	img_rect.top=rect.top-max_rect.top;
	img_rect.right=img_rect.left+(rect.right-rect.left);
	img_rect.bottom=img_rect.top+(rect.bottom-rect.top);

	if(GUI_IsInvalidRect(img_rect)||GUI_IsInvalidRect(rect))//无效矩形不要画了
	{
		return ;
	}
	
	if(s_weeks_button_is_selected[ctrl_id-MMIFM_EDITWIN_BUTTON0_CTRL_ID])
	{
		GUIRES_DisplayImg(PNULL, &rect, &img_rect, win_handle, IMAGE_CLOCK_ALARM_WEEK_MARK, &owner_draw_ptr->lcd_dev);
	}
}
/*****************************************************************************/
// 	Description : add item for setlist
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void  AddItemSetlist(void)
{
	MMI_CTRL_ID_T           current_ctrl_id = MMIFM_EDIT_MODE_SETLIST_CTRL_ID;
	uint32 text_id[3]=
	{
		TXT_ALARM_MODE_ONCE,
		TXT_ALARM_MODE_EVERYDAY,
		TXT_ALARM_MODE_WEEK
	};
	int32 loop=0;

	
	for(loop=0;loop<3;loop++)
	{
	    GUISETLIST_AddItemById(current_ctrl_id, text_id[loop]);
	}
    
    return;
}
/*****************************************************************************/
// 	Description : set record endure ctrl param for edit win
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetRecordEndureParamEditWin(const MMIFM_TIMER_RECORD_INFO_T *record_info_ptr)
{
	MMI_STRING_T    text = {0};
    wchar wnum_buf[MMIFM_DIGIT_MAX_LEN+1] = {0};
    uint8  num_buf[MMIFM_DIGIT_MAX_LEN+1] = {0};
    uint16 wnum_temp = 0;
    int s_endure_len = 0;
    
    GUIFORM_SetStyle(MMIFM_EDIT_TIME_ENDURE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);	
    
	MMI_GetLabelTextByLang(TXT_FM_RECORD_TIME_LENGTH, &text);
	GUILABEL_SetText(MMIFM_EDIT_TIME_ENDURE_LABEL_CTRL_ID, &text, FALSE);    
#ifdef MMI_PDA_SUPPORT
    wnum_temp = record_info_ptr->endure_time;	
    MMIAPICOM_Int2Str(wnum_temp, num_buf, 6);
    MMIAPICOM_StrToWstr(num_buf, wnum_buf);
    s_endure_len = MMIAPICOM_Wstrlen(wnum_buf);
    text.wstr_ptr=wnum_buf;
    text.wstr_len=s_endure_len;
    GUILABEL_SetText(MMIFM_EDIT_TIME_ENDURE_EDIT_LABEL_CTRL_ID, &text, FALSE);
#else
    GUIEDIT_SetIm(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL); 
    GUIEDIT_SetImTag(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID, GUIIM_TAG_DOT_NUM);
    GUIEDIT_SetStyle(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID, GUIEDIT_STYLE_SINGLE);
    GUIEDIT_SetAlign(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID, ALIGN_LVMIDDLE);
	wnum_temp = record_info_ptr->endure_time;	
    MMIAPICOM_Int2Str(wnum_temp, num_buf, 6);
    MMIAPICOM_StrToWstr(num_buf, wnum_buf);
    s_endure_len = MMIAPICOM_Wstrlen(wnum_buf);
    
    GUIEDIT_SetString(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID, wnum_buf, s_endure_len);
#endif
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void setEditSelectWeeksSoftkey(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id,
                                          	BOOLEAN is_update)
{
	if(ctrl_id>=MMIFM_EDITWIN_BUTTON0_CTRL_ID && ctrl_id<=MMIFM_EDITWIN_BUTTON6_CTRL_ID)
	{
		if(s_weeks_button_is_selected[ctrl_id-MMIFM_EDITWIN_BUTTON0_CTRL_ID])
		{
#ifndef GUI_MID_SOFTKEY_USE_IMG 
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_CANCEL, STXT_RETURN, is_update);

#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_CANCEL, STXT_RETURN, is_update);
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_CANCEL_ICON, 1, is_update);
#endif
        }
		else
		{
#ifndef GUI_MID_SOFTKEY_USE_IMG 
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_SELECT, STXT_RETURN, is_update);

#else
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, STXT_SELECT, STXT_RETURN, is_update);
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_SELECTED_ICON, 1, is_update);
#endif
		}
	}
}
/*****************************************************************************/
// 	Description : Set EditWin Button bg
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void SetEditWinButtonBg(MMI_HANDLE_T    win_id,uint32 ctrl_id)
{	
    uint32 i=0;
    GUI_BG_T  bg={0};
    GUI_FONT_ALL_T  enabled_font={0};    
    GUI_FONT_ALL_T  disabled_font={0};

    enabled_font.color=MMI_WHITE_COLOR;
#ifdef MAINLCD_SIZE_320X480
    enabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    enabled_font.font=SONG_FONT_14;
#else
    enabled_font.font=SONG_FONT_16;
#endif
    disabled_font.color=MMI_GRAY_COLOR;
#ifdef MAINLCD_SIZE_320X480
    disabled_font.font=SONG_FONT_20;
#elif defined MAINLCD_SIZE_128X160
    disabled_font.font=SONG_FONT_14;
#else
    disabled_font.font=SONG_FONT_16;
#endif    
    bg.bg_type=GUI_BG_IMG;
    for(i=0;i<ALM_WEEK_DAY_NUM;i++)
    {
        if(s_weeks_button_is_selected[i])
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_ENABLED;            
            GUIBUTTON_SetFont(MMIFM_EDITWIN_BUTTON0_CTRL_ID+i, &enabled_font);
        }
        else
        {
            bg.img_id=IMAGE_CLOCK_ALARM_WEEK_DISABLED;            
            GUIBUTTON_SetFont(MMIFM_EDITWIN_BUTTON0_CTRL_ID+i, &disabled_font);
        }
        GUIBUTTON_SetBg(MMIFM_EDITWIN_BUTTON0_CTRL_ID+i, &bg);
        GUIBUTTON_Update(MMIFM_EDITWIN_BUTTON0_CTRL_ID+i);
    }	
}
/*****************************************************************************/
// 	Description : handle alarm edit windows penok messages
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void  HandleFmWeeksBtnEditMsg(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id
                                          )
{
	switch(ctrl_id)
	{
#ifdef MMI_PDA_SUPPORT
        case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
			MMK_SendMsg(win_id, MSG_APP_OK,  PNULL);
			break;
        case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
			MMK_SendMsg(win_id, MSG_APP_CANCEL,  PNULL);
			break;
        case MMIFM_EDIT_TIME_EDIT_LABEL_CTRL_ID:
			MMK_CreatePubFormWin((uint32 *)MMIFM_TIMEWIN_TAB,PNULL);
			break;
        case MMIFM_EDIT_TIME_ENDURE_EDIT_LABEL_CTRL_ID:
			MMK_CreatePubEditWin((uint32 *)MMIFM_TIMEEDUREWIN_TAB,PNULL);
			break;
        case MMIFM_EDIT_FREQUENCY_EDIT_LABEL_CTRL_ID:
			MMK_CreatePubEditWin((uint32 *)MMIFM_EDITFMFRE_WIN_TAB,PNULL);
			break;
#endif
		case MMIFM_EDITWIN_BUTTON0_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON1_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON2_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON3_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON4_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON5_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON6_CTRL_ID:
			if(s_weeks_button_is_selected[ctrl_id-MMIFM_EDITWIN_BUTTON0_CTRL_ID])
			{
				s_weeks_button_is_selected[ctrl_id-MMIFM_EDITWIN_BUTTON0_CTRL_ID]=FALSE;
			}
			else
			{
				s_weeks_button_is_selected[ctrl_id-MMIFM_EDITWIN_BUTTON0_CTRL_ID]=TRUE;
			}
#ifndef MMI_PDA_SUPPORT
			setEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
			SetEditWinButtonBg(win_id,ctrl_id);
#ifdef MMI_PDA_SUPPORT
            SaveFreEditWin();
            if(ALM_MODE_EVERYWEEK==GUISETLIST_GetCurIndex(MMIFM_EDIT_MODE_SETLIST_CTRL_ID))
            {
                wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
                MMI_STRING_T str={0};
                str.wstr_len=MMIAPIALARM_GetFreModeString(ALM_MODE_EVERYWEEK,s_alarm_fre,buff,GUILIST_STRING_MAX_NUM+1);
                str.wstr_ptr=buff;
                GUISETLIST_SetDispText(MMIFM_EDIT_MODE_SETLIST_CTRL_ID,&str,TRUE);
            }
#endif
			break;
		default:
			break;
	}
}
/*****************************************************************************/
// 	Description : handle alarm edit windows FormSwitchActive messages
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void  HandleEditFormSwitchActiveMsg(
											MMI_HANDLE_T    win_id,
                                          	uint32 ctrl_id
                                          )
{
	switch(ctrl_id)
	{	
	    case MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID:
        case MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID:
#ifndef MMI_PDA_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, TXT_NULL, TXT_DELETE, TRUE);
#endif
			break;
		case MMIFM_EDITWIN_BUTTON0_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON1_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON2_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON3_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON4_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON5_CTRL_ID:
		case MMIFM_EDITWIN_BUTTON6_CTRL_ID:
#ifndef MMI_PDA_SUPPORT
			setEditSelectWeeksSoftkey(win_id,ctrl_id,TRUE);
#endif
			SetEditWinButtonBg(win_id,ctrl_id);
			break;
		default:
#ifndef MMI_PDA_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
#endif
			SetEditWinButtonBg(win_id,ctrl_id);
			break;
	}
}
/*****************************************************************************/
// 	Description : save time
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimeEditWin(void)
{
	MMISET_TIME_T   time = {0};
	
	GUIEDIT_GetTime(MMIFM_EDIT_TIME_EDITBOX_CTRL_ID, &time.time_hour, &time.time_minute, PNULL);
	if ( MMIAPICOM_CheckTimeIsValid(time))
	{
		s_alarm_hour= time.time_hour; 
		s_alarm_minute = time.time_minute;
	}
	else   // time invalid
	{
		MMIPUB_OpenAlertWarningWin(TXT_INVALID_TIME);
		return FALSE;
	}

	return TRUE ;
}
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
// 	Description : save endure time
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimerRecordFre(void)
{
	//MMI_STRING_T    string = {0};
    MMI_STRING_T    str = {0};
    MMIFM_TIMER_RECORD_INFO_T record_info = {0};
   // int16           i_endure_time = 0;
    MMIFM_GetTimerRecordInfo(&record_info);
	
    GUIEDIT_GetString(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID,&str);  
	if(FALSE == MMIAPIALM_IsValidFM(str.wstr_ptr, str.wstr_len))	
	{
		MMIPUB_OpenAlertWarningWin(TXT_FM_INVALID_FREQ);
        return FALSE;
	}
    record_info.record_freq = (uint16)ConvertFloatStrToInt(&str, 10);

    MMIFM_SetTimerRecordInfo(&record_info);
    
	return TRUE ;
}
/*****************************************************************************/
// 	Description : save endure time
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimerRecordEndureTime(void)
{
	MMI_STRING_T    string = {0};
    //MMI_STRING_T    str = {0};
    MMIFM_TIMER_RECORD_INFO_T record_info = {0};
    int16           i_endure_time = 0;

    MMIFM_GetTimerRecordInfo(&record_info);
	
    GUIEDIT_GetString(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID,&string);
    i_endure_time = ConvertStringToInt(&string,10);
	if (0 >= i_endure_time)
	{
		MMIPUB_OpenAlertWarningWin(TXT_INVALID_ENDURE_TIME);
		return FALSE;
	}    
	
    record_info.endure_time = (uint16)i_endure_time;

    MMIFM_SetTimerRecordInfo(&record_info);
    
	return TRUE ;
}
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : save endure time
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveTimerRecordInfo(void)
{
	MMI_STRING_T    string = {0};
    MMI_STRING_T    str = {0};
    MMIFM_TIMER_RECORD_INFO_T record_info = {0};
    int16           i_endure_time = 0;
	
    GUIEDIT_GetString(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID,&string);
    GUIEDIT_GetString(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID,&str);
    i_endure_time = ConvertStringToInt(&string,10);
	if (0 >= i_endure_time)
	{
		MMIPUB_OpenAlertWarningWin(TXT_INVALID_ENDURE_TIME);
		return FALSE;
	}    
	if(FALSE == MMIAPIALM_IsValidFM(str.wstr_ptr, str.wstr_len))	
	{
            MMIPUB_OpenAlertWarningWin(TXT_FM_INVALID_FREQ);
		
            return FALSE;
	}
    
    record_info.endure_time = (uint16)i_endure_time;
    record_info.record_freq = (uint16)ConvertFloatStrToInt(&str, 10);

    MMIFM_SetTimerRecordInfo(&record_info);
    
	return TRUE ;
}
#endif
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
// 	Description : save mode
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL void  SaveModeEditWin(void)
{
	uint32 setlist_index=GUISETLIST_GetCurIndex(MMIFM_EDIT_MODE_SETLIST_CTRL_ID);

	switch(setlist_index)
	{
		case 0:
			 s_alarm_fre_mode = ALM_MODE_ONCE;
			break;
		case 1:
			s_alarm_fre_mode = ALM_MODE_EVERYDAY;
			break;
		case 2:
			s_alarm_fre_mode = ALM_MODE_EVERYWEEK;
			break;
		default:
			s_alarm_fre_mode = ALM_MODE_ONCE;
			break;
			
	}
}
/*****************************************************************************/
// 	Description : save fre
//	Global resource dependence : 
//  Author:
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN  SaveFreEditWin(void)
{
	uint8   week_day =0;
	int32 loop=0;
	uint16      mode = 0;
	
	for(loop=0;loop<ALM_WEEK_DAY_NUM;loop++)
	{
		week_day = MMIAPISET_GetWeekDayByOffset(loop);
		if(s_weeks_button_is_selected[loop])
		{
			mode |= s_week_mask[week_day];
		}
	}

    SCI_TRACE_LOW("SaveFreEditWin mode %d,s_alarm_fre %d", mode,s_alarm_fre);
    
	if (mode != s_alarm_fre) 
	{
		s_alarm_fre = mode & 0x00ff;
	}

	if(0==s_alarm_fre)
	{
#ifdef MMI_PDA_SUPPORT
        MMIPUB_DisplayTipsIDEx(PNULL, TXT_ALARM_SELECT_ONE_DAY, MMIPUB_TIPS_DIS_MIDDLE, PNULL);
#else
		MMIPUB_OpenAlertWarningWin(TXT_ALARM_SELECT_ONE_DAY);
#endif

		return FALSE;
	}
	
	return TRUE;
}
/*****************************************************************************/
//  Description : Create FM Timer Record Win
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void CreateFMTimerRecordInfoWin(void)
{
    MMK_CreateWin((uint32 *)MMI_FM_TIMER_RECORD_WIN_TAB, PNULL);
}
#endif
#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : create  latyer
//  Global resource dependence : none
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void FMCreateLayer(GUI_LCD_DEV_INFO *dev_info_ptr,MMI_HANDLE_T handle)
{
    uint16 logic_lcd_width = 0;
    uint16 logic_lcd_height = 0;

    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, &logic_lcd_height);
        
        create_info.lcd_id = MAIN_LCD_ID;
        create_info.owner_handle =handle;
        create_info.offset_x = 0;
        create_info.offset_y = 28;
        create_info.width = logic_lcd_width;
        if(MMITHEME_IsMainScreenLandscape())
        {
            create_info.height = logic_lcd_height;
        }
        else
        {
            create_info.height = 167;//FM_LAYER_HEIGHT_V;
        }
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;        
        create_info.format = UILAYER_MEM_DOUBLE_COPY;
        UILAYER_CreateLayer(&create_info, dev_info_ptr);
    }
}


/*****************************************************************************/
//  Description : release  latyer
//  Global resource dependence : none
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void FMReleaseLayer(GUI_LCD_DEV_INFO *dev_info_ptr)
{
    if(PNULL!=dev_info_ptr)
    {
        if(dev_info_ptr->block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(dev_info_ptr);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(dev_info_ptr);
//            dev_info_ptr->block_id=UILAYER_NULL_HANDLE;
//            dev_info_ptr->lcd_id=0;
        }
    }
}

/*****************************************************************************/
//  Description : release  latyer
//  Global resource dependence : none
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void FMAppendLayer(GUI_LCD_DEV_INFO *dev_info_ptr)
{
    UILAYER_APPEND_BLT_T append_layer={0};
    
    append_layer.layer_level=UILAYER_LEVEL_NORMAL;
    append_layer.lcd_dev_info=*dev_info_ptr;
    UILAYER_AppendBltLayer(&append_layer);
}

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleFMSlideTpMove(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_HANDLE_T                handle,
                                MMIFM_SLIDE_INFO_T    *slide_info_ptr    // [in]
                                )
{
    if (MMK_TP_SLIDE_TP_SCROLL != slide_info_ptr->slide_state && IsInFreRect(handle,*point_ptr))
    {
        slide_info_ptr->handle=handle;
        slide_info_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
        slide_info_ptr->slide_pre_point.x = point_ptr->x;
        slide_info_ptr->slide_pre_point.y = point_ptr->y;
        StopSlideTimer(slide_info_ptr);
        StartSlideTimer(slide_info_ptr);
    }

    
    return;
}

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void ExitSlide( MMIFM_SLIDE_INFO_T    *slide_info_ptr)
{
    MMIFM_CUR_INFO_T *cur_inof_ptr = MMIFM_GetFMCurrentInfo();
    StopSlideTimer(slide_info_ptr);
    if((MMIFM_IsValidFre(slide_info_ptr->cur_freq))&&(slide_info_ptr->cur_freq!=cur_inof_ptr->cur_freq))
    {
       MMIFM_AdjustFrequencyByFre(slide_info_ptr->cur_freq);
    } 
    SCI_MEMSET(slide_info_ptr,0,sizeof(MMIFM_SLIDE_INFO_T));
}

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideTimer( MMIFM_SLIDE_INFO_T    *slide_info_ptr)
{
    if (PNULL != slide_info_ptr)
    {
        if(0 != slide_info_ptr->draw_slide_timer_id)
        {
            StopSlideTimer(slide_info_ptr);
        }
        slide_info_ptr->draw_slide_timer_id = MMK_CreateWinTimer(
            slide_info_ptr->handle, 
            MMIFM_SLIDE_TIME,
            FALSE );
    }
}

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideTimer( MMIFM_SLIDE_INFO_T    *slide_info_ptr)
{
    if (PNULL != slide_info_ptr && 0 != slide_info_ptr->draw_slide_timer_id)
    {
        MMK_StopTimer(slide_info_ptr->draw_slide_timer_id);
        slide_info_ptr->draw_slide_timer_id = 0;
    }
}
/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlideTimer(
                                       MMIFM_SLIDE_INFO_T    *slide_info_ptr, // [in]
                                       MMI_WIN_ID_T win_id,
                                       DPARAM           param           // [in]
                                       )
{
    MMI_RESULT_E        result              = MMI_RESULT_FALSE;
    GUI_POINT_T         point               = {0};
    MMI_TP_STATUS_E     state               = MMI_TP_NONE;
    //MMIFM_CUR_INFO_T *cur_inof_ptr = MMIFM_GetFMCurrentInfo();

    if (slide_info_ptr->draw_slide_timer_id == *(uint8*)param)  
    {
        MMK_GetLogicTPMsg(&state, &point);

        if((slide_info_ptr->slide_pre_point.x==point.x 
            && slide_info_ptr->slide_pre_point.y == point.y) 
                || (!IsInFreRect(win_id,point)))
        {
            StopSlideTimer(slide_info_ptr);
            StartSlideTimer(slide_info_ptr);
            return result;
        }
        DrawFmUIAccordingPoint(win_id,point);        
        StopSlideTimer(slide_info_ptr);
        StartSlideTimer(slide_info_ptr);
        slide_info_ptr->slide_pre_point.x = point.x;
        slide_info_ptr->slide_pre_point.y = point.y;
        result = MMI_RESULT_TRUE;
    }

    return result;
}
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
/*****************************************************************************/
//  Description : HandleEditTimeMsg
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTimeMsg(
                                        MMI_WIN_ID_T    win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    GUIEDIT_TIME_STYLE_E time_style =GUIEDIT_TIME_STYLE_24;
	MMISET_TIME_DISPLAY_TYPE_E time_type=MMISET_TIME_24HOURS;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUIFORM_IsSlide(MMIFM_TIME_EDIT_FORM_CTRL_ID,FALSE);
            time_type = MMIAPISET_GetTimeDisplayType();
        	if (MMISET_TIME_12HOURS == time_type)
        	{
        		time_style = GUIEDIT_TIME_STYLE_12;
        	}
        	else
        	{
        		time_style = GUIEDIT_TIME_STYLE_24;
        	}

        	//set time display style
        	GUIEDIT_SetTimeStyle(MMIFM_EDIT_TIME_EDITBOX_CTRL_ID,PNULL,&time_style,PNULL,FALSE);

        	GUIEDIT_SetTime(MMIFM_EDIT_TIME_EDITBOX_CTRL_ID,
        						s_alarm_hour, 
        						s_alarm_minute,
        						0);
            MMK_SetAtvCtrl(win_id,MMIFM_EDIT_TIME_EDITBOX_CTRL_ID);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(SaveTimeEditWin())
        {
            MMK_CloseWin(win_id);
        }
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
/*****************************************************************************/
//  Discription: HandleEditNameMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditTimeEdureMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T            ctrl_id = MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID;
    //wchar  dot_str[] = {'.'};
    //MMI_STRING_T str={0};
    MMIFM_TIMER_RECORD_INFO_T record_info = {0};

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            wchar wnum_buf[MMIFM_DIGIT_MAX_LEN+1] = {0};
            uint8  num_buf[MMIFM_DIGIT_MAX_LEN+1] = {0};
            uint16 wnum_temp = 0;
            int s_endure_len = 0;
            MMIFM_GetTimerRecordInfo(&record_info);
            GUIEDIT_SetIm(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_SetImTag(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID, GUIIM_TAG_DOT_NUM);
            GUIEDIT_SetStyle(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID, GUIEDIT_STYLE_SINGLE); 
        	wnum_temp = record_info.endure_time;	
            MMIAPICOM_Int2Str(wnum_temp, num_buf, 6);
            MMIAPICOM_StrToWstr(num_buf, wnum_buf);
            s_endure_len = MMIAPICOM_Wstrlen(wnum_buf);
            
            GUIEDIT_SetString(MMIFM_EDIT_TIME_ENDURE_EDITBOX_CTRL_ID, wnum_buf, s_endure_len);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		if(SaveTimerRecordEndureTime())
		{
		    MMK_CloseWin(win_id);
		}
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Discription: HandleEditFmFreMsg
//  Global resource dependence: none 
//  Author: renyi.hu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditFmFreMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T            ctrl_id = MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID;
    //wchar  dot_str[] = {'.'};
    //MMI_STRING_T str={0};
    MMIFM_TIMER_RECORD_INFO_T record_info = {0};


    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
        	MMI_STRING_T    text = {0};
            int16           frequency = 0;
            uint8 num_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
            wchar wnum_buf[MMIFM_CHANNEL_NAME_LEN + 1] = {0};
            wchar  dot_str[] = {'.'};
            MMIFM_GetTimerRecordInfo(&record_info);

            GUIFORM_SetStyle(MMIFM_EDIT_FREQUENCY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);	
            
        	MMI_GetLabelTextByLang(TXT_FM_RECORD_FREQUENCY, &text);
        	GUILABEL_SetText(MMIFM_EDIT_FREQUENCY_LABEL_CTRL_ID, &text, FALSE);    

            GUIEDIT_SetIm(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_SetStyle(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID, GUIEDIT_STYLE_SINGLE); 
            GUIEDIT_ConfigImCustomKey(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
            GUIEDIT_SetImTag(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID, GUIIM_TAG_DOT_NUM);

            frequency = record_info.record_freq;
            if (0 != frequency)
        	{
                sprintf((char*)num_buf, "%d.%d",frequency/10, frequency%10);
                text.wstr_len = strlen((char*)num_buf);
                text.wstr_len = MIN(MMIFM_CHANNEL_NAME_LEN, text.wstr_len);
                text.wstr_ptr = wnum_buf;
                MMI_STRNTOWSTR(text.wstr_ptr, MMIFM_CHANNEL_NAME_LEN, num_buf, MMIFM_CHANNEL_NAME_LEN, text.wstr_len);
        	}
            else
            {
                text.wstr_len = 5;
                text.wstr_ptr = L"87.5";
            }
            GUIEDIT_SetString(MMIFM_EDIT_FREQUENCY_EDITBOX_CTRL_ID, text.wstr_ptr, text.wstr_len);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        if(SaveTimerRecordFre())
		{
		    MMK_CloseWin(win_id);
		}
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
#endif
#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register FM module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIFM_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_FM, &g_mmifm_applet_info);
}

/*****************************************************************************/
//  Description : the process message function of the FM
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  FMApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIFM_APPLET_T* applet_ptr = (MMIFM_APPLET_T*)iapplet_ptr;

    //SCI_TRACE_LOW:"mmi FMApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFM_WINTAB_8122_112_2_18_2_19_44_97,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_APPLET_START:
        s_fm_applet_instance = applet_ptr;

        //not start bg
        if ( !MMK_GetAppletState( applet_ptr->caf_applet.app_handle, MMI_APPLET_STATE_START_BG ) )
        {
            MMK_CreateFmWin((uint32*)MMI_FM_MAIN_WIN_TAB,PNULL);
        }
        break;

    case MSG_APPLET_SWITCH:
        if(!MMK_IsOpenWin(FM_MAIN_WIN_ID))
        {
            MMK_CreateFmWin((uint32*)MMI_FM_MAIN_WIN_TAB,PNULL);
        }
        break;
        
    case MSG_APPLET_STOP: 
        MMIFM_Exit(FALSE);
        s_fm_applet_instance = PNULL;
        break;

    case MSG_APPLET_RESUME: 
    {
        if(FM_STOP == MMIFM_GetStatus() || FM_SUSPENDED == MMIFM_GetStatus())
        {               
            //start FM
			if(FM_STOP == MMIFM_GetStatus())
			{
				if(!MMIFM_Play(FALSE)) 
				{                
					//start FM fail
					MMIFM_Exit(FALSE);
					MMIPUB_OpenAlertFailWin(TXT_FM_INIT_FAIL);               
				}
			}
        }
    }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : Get FM Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIFM_APPLET_T *MMIFM_GetAppletInstance(void)
{
    return s_fm_applet_instance;
}

/*****************************************************************************/
//  Description : Create FM reference window for MMI_APPLET_STATE_BG_HIDE_WIN
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T OpenFmWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  )
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = SPRD_FM_APPLET_ID;
    win_table_create.win_table_ptr = win_table_ptr;
    win_table_create.add_data_ptr = add_data_ptr;
    
    return MMK_CreateWinTable(&win_table_create);
}

 /*****************************************************************************/
//  Description : Stop FM applet
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFM_StopApplet(BOOLEAN is_minimize)
{
    if (is_minimize)
    {
        //MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
        MMK_HideWinByApplet(SPRD_FM_APPLET_ID);
    }
    else
    {
        MMK_CloseApplet(SPRD_FM_APPLET_ID);
    }
}
#endif/*MMI_TASK_MANAGER*/
/*****************************************************************************/
//  Description : preview fm frequency status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFM_IsPreviewFM(void)
{
    BOOLEAN result = FALSE;
    
	MMIFM_START_WAY_E  start_way = (uint32)MMK_GetWinAddDataPtr(FM_MAIN_WIN_ID);
	
    if(PREVIEW_START_FM == start_way || MMK_IsFocusWin(MMI_ALARM_FM_CHANNEL_LIST_WIN_ID))
        result = TRUE;
    
    return result;   
    
}
#ifdef FM_NEW_UI
/*****************************************************************************/
//  Description : get angel
//  Global resource dependence : 
//  Author: 
//  Note:point1:原点, point2:手指所在位置
/*****************************************************************************/
LOCAL MMIFM_POINT_DATA_T GetPointValueByPoint(GUI_POINT_T point1, GUI_POINT_T point2)
{
    int i=0;
    int value = 0;
    float freq_cell = 0;
    MMIFM_POINT_DATA_T point_data = {0};

    freq_cell = (float)(1045-970)/90;//104.5,97分别是右上方90度角两边对应的频率数值。
    if((-5<(point2.y- point1.y)) &&((point2.y- point1.y)<5))//
    {
        if(point2.x > point1.x)
        {
            point_data.angle = 90;
            point_data.freq = 1045;
            
        }
        else
        {
            point_data.angle = 270;
            point_data.freq = 895;
        }
    }
    if((-5<(point2.x-point1.x)) && ((point2.x-point1.x)<5))//
    {
        if(point2.y > point1.y)
        {
            point_data.angle = 180;
            point_data.freq = 112; //这个频率值是无效的。
        }
        else
        {
            point_data.angle = 0;
            point_data.freq = 970;
        }
    }

   if((point2.x > point1.x) && (point2.y < point1.y))//右上角
   {
       value = ((point2.x - point1.x)*1000)/(point1.y-point2.y);
       for(i=0;i<90; i++)
       {
            if(value< tan_0_90_table[i])
            {
            //   point_data.angle = i;
               point_data.freq = 970+i*freq_cell; /*lint !e524*/
               point_data.angle = GetAngleByFreq(point_data.freq);
               break;
            }
       }
       
   }
   else if((point2.x > point1.x) && (point2.y > point1.y))//右下角
   {
       value = ((point2.y - point1.y)*1000)/(point2.x-point1.x);
       for(i=0;i<90; i++)
       {
            if(value< tan_0_90_table[i])
            {
            //   point_data.angle = 90+i;
               point_data.freq = 1045+i*freq_cell;/*lint !e524*/
            point_data.angle = GetAngleByFreq(point_data.freq);
               break;
            }
       }

   }
   else if((point2.x < point1.x) && (point2.y > point1.y))//左下角
   {
       value = ((point1.x - point2.x)*1000)/(point2.y-point1.y);
       for(i=0;i<90; i++)
       {
            if(value< tan_0_90_table[i])
            {
            //   point_data.angle = 180+i;
               point_data.freq = 895-(90-i)*freq_cell;/*lint !e524*/
            point_data.angle = GetAngleByFreq(point_data.freq);
               break;
            }
       }

   }
   else if((point2.x < point1.x) && (point2.y < point1.y))//左上角
   {
       value = ((point1.y - point2.y)*1000)/(point1.x-point2.x);
       for(i=0;i<90; i++)
       {
            if(value< tan_0_90_table[i])
            {
            //   point_data.angle = 270+i;
               point_data.freq = 895+i*freq_cell;/*lint !e524*/
            point_data.angle = GetAngleByFreq(point_data.freq);
               break;
            }
       }
   }
 
   return point_data;
}
/*****************************************************************************/
//  Description : get angel By Freq
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int GetAngleByFreq(uint16 fre)
{
    int value = 0;
    float freq_cell = 0;

    freq_cell = (float)(1045-970)/90;//104.5,97分别是右上方90度角两边对应的频率数值。
    if( fre>=970)
    {
        value = (fre-970)/freq_cell;/*lint !e524*/
    }
    else
    {
        value = 360-(970-fre)/freq_cell;/*lint !e524*/
    }

    return value;
}
/******************************************************************************/
//  Description: decompressed the ARGB image
//  Author: 
//  Input:   
//          src_img_ptr:	pointer of input image information
//			dst_buf_ptr:	pointer to the target buffer
//			dst_buf_size:	target buffer size
//  Output: 
//			dst_img_ptr:	pointer to the decompressed image information
//  Return: 
//			TRUE:			successful
//          FALSE:			failed
//  Note:   
/******************************************************************************/
LOCAL BOOLEAN DecompressARGB(const MMIFM_IMAGE_T *src_img_ptr, MMIFM_IMAGE_T *dst_img_ptr, 
							  uint8 *dst_buf_ptr, uint32 dst_buf_size)
{
	uint32			img_bytes	= 0;
	IMG_RES_SRC_T	img_src		= {0};
	IMG_RES_DST_T	img_dst		= {0};
	IMG_RES_INFO_T	img_info	= {0};
	IMG_RES_ERROR_E error		= 0;
	IMG_RES_DST_CTL_T dstctl   = {0};

    //get the compressed ARGB info, width and height
    //calc img_bytes
    if(PNULL == src_img_ptr)
            return FALSE;
	img_src.imgstream_ptr = (uint8 *)src_img_ptr->data_ptr;
	img_src.imgstreamsize = src_img_ptr->data_size;

    error = IMG_RES_GetImageInfo(&img_src, &img_info);
	if (IMG_RES_SUCCESS != error)
	{
		return FALSE;
	}

	img_bytes = img_info.width * img_info.height * 4;	

	if (PNULL == dst_buf_ptr || dst_buf_size < img_bytes)
	{
		return FALSE;
	}

	img_src.imgrect.left = 0;
	img_src.imgrect.top = 0;
	img_src.imgrect.right = img_info.width - 1;
	img_src.imgrect.bottom = img_info.height - 1;
	
	//decompress ARGB image
	img_dst.dstctl = &dstctl;
	img_dst.dstctl->bg_type = IMG_BG_NORMAL;
	img_dst.dstctl->format = IMGREF_FORMAT_ARGB888;
	img_dst.dstctl->dstmem = dst_buf_ptr;
	img_dst.dstctl->width = img_info.width;
	img_dst.dstctl->height = img_info.height;
	img_dst.dstrect.top = 0;
	img_dst.dstrect.left = 0;
	img_dst.dstrect.right = img_info.width - 1;
	img_dst.dstrect.bottom = img_info.height - 1;

	SCI_MEMSET(dst_buf_ptr, 0, dst_buf_size);
       
       error = IMG_RES_Display_Img(&img_src, &img_dst);	
	if (IMG_RES_SUCCESS != error)
	{
		return FALSE;
	}
	
	*dst_img_ptr = *src_img_ptr;

    dst_img_ptr->data_ptr = dst_buf_ptr;
    dst_img_ptr->data_size = img_bytes;
    dst_img_ptr->data_type = IMGREF_FORMAT_ARGB888;
    
	return TRUE;
}
/*****************************************************************************/
//  Description : Display Freq Pointer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayFreqPointerByAngle(int new_freq_angle)
{
    //GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    uint16 img_width = 0;
    uint16 img_height = 0;
    //GUI_RECT_T rect = {0};
    ROTATE_ARBITRARY_IN_T	rot_in		= {0};
    ROTATE_ARBITRARY_OUT_T	rot_out		= {0};
    uint32              img_size       = 0;
    uint8*              img_ptr         = PNULL;
    MMIFM_IMAGE_T src_img = {0};
    MMIFM_IMAGE_T dst_img = {0};  
    uint32  rotated_buf_size  = 0;
    uint8*  rotated_buf_ptr   = PNULL;
    uint8* decomp_buf_ptr = PNULL;
    uint32 decomp_buf_size = 0; 
    GUI_LCD_DEV_INFO tmp_dev_info = { 0,UILAYER_NULL_HANDLE };
    
    if(132 < new_freq_angle && new_freq_angle < 245)//132是108MHz对应角度；246是87.5MHz对应角度；87.5-108是有效频率
    {
        return;  
    }
    if(UILAYER_IsLayerActive(&s_needle_dev_info))
    {
        UILAYER_Clear(&s_needle_dev_info);
    }
    else
    {
        if(UILAYER_IsMultiLayerEnable())
        {
            UILAYER_CREATE_T    create_info = {0};
            GUI_POINT_T center_point = {0};

            center_point = GetFreqNeedleCenterPoint();
            
            GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_FM_AJUST_NEEDLE, FM_MAIN_WIN_ID);        
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = FM_MAIN_WIN_ID;
            create_info.offset_x = center_point.x- (img_width + img_height)/2;
            create_info.offset_y = center_point.y- (img_width + img_height)/2;
            create_info.width = img_width + img_height;
            create_info.height = img_width + img_height;
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = TRUE;
            UILAYER_CreateLayer(&create_info, &tmp_dev_info);
        }
    }
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_FM_AJUST_NEEDLE, FM_MAIN_WIN_ID);
    
    rotated_buf_size = (img_width + img_height) * (img_width + img_height) * sizeof(uint32);
    if(UILAYER_IsLayerActive(&s_needle_dev_info))
    {
        rotated_buf_ptr = UILAYER_GetLayerBufferPtr(&s_needle_dev_info);
    }
    else if(UILAYER_IsLayerActive(&tmp_dev_info))
    {
        rotated_buf_ptr = UILAYER_GetLayerBufferPtr(&tmp_dev_info);
    }
    else
    {
        SCI_TRACE_LOW("DisplayFreqPointerByAngle:no memery 1!");
        return; 
    }
    
    decomp_buf_size = img_width * img_height * sizeof(uint32);
    decomp_buf_ptr = SCI_ALLOCA((decomp_buf_size) * sizeof(uint8));
    if(PNULL == decomp_buf_ptr)
    {
        SCI_TRACE_LOW("DisplayFreqPointerByAngle:no memery 2!");
        if(UILAYER_IsLayerActive(&tmp_dev_info))
        {
            FMReleaseLayer(&tmp_dev_info);
        }
        return;
    }
    
    img_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_FM_AJUST_NEEDLE,FM_MAIN_WIN_ID,&img_size);
    src_img.data_ptr = img_ptr;
    src_img.data_size = img_size;
    src_img.data_type = IMGREF_FORMAT_RGB565;
    src_img.width = img_width;
    src_img.height = img_height;
    SCI_MEMSET(decomp_buf_ptr, 0, decomp_buf_size);
    if (!DecompressARGB(&src_img, &dst_img, decomp_buf_ptr, decomp_buf_size))
    {
        SCI_TRACE_LOW("DisplayFreqPointerByAngle:DecompressARGB error!");
        if(PNULL != decomp_buf_ptr) /*lint !e774*/
        {
            SCI_FREE(decomp_buf_ptr);
        }
        if(UILAYER_IsLayerActive(&tmp_dev_info))
        {
            FMReleaseLayer(&tmp_dev_info);
        }
        return;
    }
    rot_in.angle = new_freq_angle;
    rot_in.src_format = IMGREF_FORMAT_ARGB888;
    rot_in.src_size.w = img_width;
    rot_in.src_size.h = img_height;
    rot_in.src_center.x = img_width/2;
    rot_in.src_center.y = img_height/2;
    rot_in.src_chn.chn0.ptr = (void *)dst_img.data_ptr; 
    rot_in.src_chn.chn0.size = img_width * img_height * sizeof(uint32); 

    rot_in.target_format = IMGREF_FORMAT_ARGB888;
    rot_in.target_size.w = img_width + img_height;
    rot_in.target_size.h = img_width + img_height;
    rot_in.target_chn.chn0.ptr = (void *)rotated_buf_ptr;
    rot_in.target_chn.chn0.size = rotated_buf_size;
    rot_in.target_center.x = (img_width + img_height)/2;
    rot_in.target_center.y = (img_width + img_height)/2;
    
    rot_in.mode = ROTATE_ARBITRARY_AUTO_CROP;

    SCI_MEMSET(rotated_buf_ptr, 0, rotated_buf_size);
    if (SCI_SUCCESS != GRAPH_RotateArbitrary(&rot_in, &rot_out))
    {
        SCI_TRACE_LOW("DisplayFreqPointerByAngle:GRAPH_RotateArbitrary error!");
        if(PNULL != decomp_buf_ptr) /*lint !e774*/
        {
            SCI_FREE(decomp_buf_ptr);
        }
        if(UILAYER_IsLayerActive(&tmp_dev_info))
        {
            FMReleaseLayer(&tmp_dev_info);
        }
    	return;
    }
    if(UILAYER_IsLayerActive(&tmp_dev_info))
    {
        GUI_RECT_T rect = UILAYER_GetLayerRect(&tmp_dev_info);
        UILAYER_BltLayerToLayer(&s_main_lcd_dev_info,&tmp_dev_info , &rect,TRUE);
        FMReleaseLayer(&tmp_dev_info);
    }
    
    SCI_FREE(decomp_buf_ptr); 
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T GetFreqNeedleCenterPoint(void)
{
     GUI_POINT_T center_point = {0};
     
     if(MMITHEME_IsMainScreenLandscape())
     {
        center_point.x = FM_H_FRQ_POINTER_CENTER_X;
        center_point.y = FM_H_FRQ_POINTER_CENTER_Y;         
     }
     else
     {
        center_point.x = FM_V_FRQ_POINTER_CENTER_X;
        center_point.y = FM_V_FRQ_POINTER_CENTER_Y;
     }

     return center_point;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CreateFreqNeedleLayer(void)
{
    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T    create_info = {0};
        uint16 img_width = 0;
        uint16 img_height = 0;
        GUI_POINT_T center_point = {0};

        center_point = GetFreqNeedleCenterPoint();
        
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_FM_AJUST_NEEDLE, FM_MAIN_WIN_ID);        
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = FM_MAIN_WIN_ID;
        create_info.offset_x = center_point.x- (img_width + img_height)/2;
        create_info.offset_y = center_point.y- (img_width + img_height)/2;
        create_info.width = img_width + img_height;
        create_info.height = img_width + img_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(&create_info, &s_needle_dev_info);
    }
}
/*****************************************************************************/
//  Description : draw fm ui according the point
//  Global resource dependence : 
//  Parameter: point[IN]:touch pannel location
//  Note: 
/*****************************************************************************/
LOCAL void DrawFmUIAccordingPoint(MMI_WIN_ID_T win_id,GUI_POINT_T point)
{
    MMIFM_POINT_DATA_T point_value = {0};
    MMIFM_CUR_INFO_T *cur_info_ptr = MMIFM_GetFMCurrentInfo();
    if(IsInFreRect(win_id,point) && (FM_PLAYING == cur_info_ptr->status))
    { 
        point_value = GetPointValueByPoint(GetFreqNeedleCenterPoint(), point);
        DisplayFreqPointerByAngle(point_value.angle);
        if(MMIFM_IsValidFre(point_value.freq))
        {
            s_slide_info.cur_freq = point_value.freq;
            DrawChannelNameByFreq(win_id,point_value.freq);
            if (UILAYER_IsLayerActive(&s_lcd_dev_info))
            {
                DisplayMainFre(win_id,&s_lcd_dev_info,point_value.freq);
            }
            else
            {
                DisplayMainFre(win_id,&s_main_lcd_dev_info,point_value.freq);
            }
        }
    }
}
#endif
PUBLIC void MMIAPIFM_AutosearchChannel(void)
{
    OpenAuotSearchWin(MMIFM_AUTOSEARCH,FM_SCAN_UP);
}
/*****************************************************************************/
//  Description : Handle headset button action
//  Global resource dependence : 
//  Author:
//  Parameter: 无
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFM_HandleHeadsetButtonAction(MMIFM_HEADSET_BUTTON_ACTION_TYPE_E action_type)
{   
    MMIFM_CUR_INFO_T *cur_info_ptr = MMIFM_GetFMCurrentInfo();
    SCI_TRACE_LOW("[&&&]MMIFM_HandleHeadsetButtonAction: action_type=%d",action_type);
    if(!MMK_IsOpenWin(FM_AUTO_SEARCH_WIN_ID))
    {
        if(HEADSET_BUTTON_SHORT_PRESS == action_type)
        {
            //SetFMStartSuspendButtonStatus(FM_MAIN_WIN_ID);
          if(MMIFM_CheckHeadsetStatus())
          {
            if(MMIFM_GetStatus()== FM_PLAYING)
            {   
                FM_PlayStop();
                MMIFM_SetSuspendByFM(TRUE);               
                MMIFM_SetStatus(FM_SUSPENDED);
            }
            else if(MMIFM_GetStatus() == FM_SUSPENDED)
            {
              if(MMIFM_GetSuspendByFM())
              {
                cur_info_ptr->cur_freq = MIN(cur_info_ptr->cur_freq, MMIFM_FREQ_RANGE_HIGH);
                cur_info_ptr->cur_freq = MAX(cur_info_ptr->cur_freq, MMIFM_FREQ_RANGE_LOW);
                MMIFM_EnableMute();
                FM_ManualSeek(cur_info_ptr->cur_freq, FM_SCAN_UP, 0, NULL);
                MMIFM_DisableMute();
                MMIFM_SetStatus(FM_PLAYING);
              }
            }
            if(MMK_IsFocusWin(FM_MAIN_WIN_ID))
            {
    	      MMK_SendMsg(FM_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
          }
        }
        else if(HEADSET_BUTTON_LONG_PRESS == action_type)
        {
            if(MMIFM_GetValidChannelCount() > 1)
            {               
                MMIFM_AdjustChannel(MMIFM_GET_UP_VALUE_STEP);
                SetButtonBgWhenResume();
                if(MMK_IsFocusWin(FM_MAIN_WIN_ID))
                {
                    MMK_SendMsg(FM_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
                }
                /*NEWMS00232743*/
                MMIAPIIDLE_MAIN_UpdateIdle();
            }
        }
    }
}
#ifndef FM_NEW_UI
#ifndef MMI_FM_MINI_SUPPORT
/*****************************************************************************/
//  Description : create buttons.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  SetButtonsAndLabelRect(MMI_WIN_ID_T win_id)
{

    GUI_BOTH_RECT_T         both_rect={0};
    uint16 left_width=0;
    uint16 left_height=0;
    uint16 anim_width=0;
    uint16 anim_height=0;
    uint16 search_button_width=0;
    uint16 search_button_height=0;
    GUI_BOTH_RECT_T  status_bar_both_rect =  MMITHEME_GetStatusBarBothRect();

    GUI_BOTH_RECT_T  win_client_both_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifndef MMI_MINI_QVGA_UI_SUPPORT  
    GUIRES_GetImgWidthHeight(&left_width, &left_height, IMAGE_FM_ADJ_LEFT_DEFAULT, win_id);   
#endif
    GUIRES_GetImgWidthHeight(&anim_width, &anim_height, IMAGE_FM_ADJ_BG, win_id);   
    GUIRES_GetImgWidthHeight(&search_button_width, &search_button_height, IMAGE_FM_LEFT_DEFAULT, win_id); 
#ifdef FM_RDS_SUPPORT

    both_rect.v_rect.top    =win_client_both_rect.v_rect.top + anim_height;
    both_rect.v_rect.bottom = both_rect.v_rect.top + GUI_ConvertGUIFontToSize(SONG_FONT_15);
    both_rect.v_rect.left   =MMIFM_V_LEFT_BUTTON_START_X;
    both_rect.v_rect.right  =anim_width;
    
    both_rect.h_rect.top = win_client_both_rect.h_rect.top + anim_height;
    both_rect.h_rect.bottom=both_rect.h_rect.top + GUI_ConvertGUIFontToSize(SONG_FONT_15);
    both_rect.h_rect.left= MMIFM_H_LEFT_BUTTON_START_X;
    both_rect.h_rect.right=anim_width;
    GUIAPICTRL_SetBothRect(MMIFM_RDS_TEXT_CTRL_ID,&both_rect);
#endif


#ifndef MMI_MINI_QVGA_UI_SUPPORT
    #ifdef FM_RDS_SUPPORT
    both_rect.v_rect.top    =both_rect.v_rect.bottom +1;
    #else
    both_rect.v_rect.top    =MMIFM_V_LEFT_BUTTON_START_Y;
    #endif
    both_rect.v_rect.bottom =both_rect.v_rect.top+left_height;
    both_rect.v_rect.left   =MMIFM_V_LEFT_BUTTON_START_X;
    both_rect.v_rect.right  =both_rect.v_rect.left+left_width;
    
    both_rect.h_rect.top = MMIFM_H_LEFT_BUTTON_START_Y;
    both_rect.h_rect.bottom=both_rect.h_rect.top + left_height;
    both_rect.h_rect.left= MMIFM_H_LEFT_BUTTON_START_X;
    both_rect.h_rect.right=both_rect.h_rect.left+left_width;
    GUIAPICTRL_SetBothRect(MMIFM_ADJ_FREQ_LEFT_BUTTON_ID,&both_rect);
    
    both_rect.v_rect.left   =both_rect.v_rect.right;
    both_rect.v_rect.right  =both_rect.v_rect.left+MMIFM_V_LABEL_WIDTH;
    
    both_rect.h_rect.left= both_rect.h_rect.right;
    both_rect.h_rect.right=both_rect.h_rect.left+MMIFM_H_LABEL_WIDTH;
    GUIAPICTRL_SetBothRect(MMIFM_FREQUENCY_TEXT_CTRL_ID,&both_rect);
    
    both_rect.v_rect.left   =both_rect.v_rect.right;
    both_rect.v_rect.right  =both_rect.v_rect.left+left_width;
    
    both_rect.h_rect.left= both_rect.h_rect.right;
    both_rect.h_rect.right=both_rect.h_rect.left+left_width;
    GUIAPICTRL_SetBothRect(MMIFM_ADJ_FREQ_RIGHT_BUTTON_ID,&both_rect);
#else
    both_rect.v_rect.top    =MMIFM_V_FREQUENCY_TEXT_START_Y;
    both_rect.v_rect.bottom =both_rect.v_rect.top + FREQUENCY_TEXT_HEIGHT;
    both_rect.v_rect.left   =MMIFM_V_FREQUENCY_TEXT_START_X;//both_rect.v_rect.right;
    both_rect.v_rect.right  =both_rect.v_rect.left+MMIFM_V_LABEL_WIDTH;

    both_rect.h_rect.top    =MMIFM_H_FREQUENCY_TEXT_START_Y;
    both_rect.h_rect.bottom =both_rect.h_rect.top + FREQUENCY_TEXT_HEIGHT;
    both_rect.h_rect.left   =MMIFM_H_FREQUENCY_TEXT_START_X;//both_rect.h_rect.right;
    both_rect.h_rect.right  =both_rect.h_rect.left+MMIFM_H_LABEL_WIDTH;
    GUIAPICTRL_SetBothRect(MMIFM_FREQUENCY_TEXT_CTRL_ID,&both_rect);
#endif

    both_rect.v_rect.top    =MMIFM_V_SEARCH_UPPER_BUTTON_START_Y;
    both_rect.v_rect.bottom =both_rect.v_rect.top+search_button_height;
    both_rect.v_rect.left   =MMIFM_V_SEARCH_UPPER_BUTTON_START_X;
    both_rect.v_rect.right  =both_rect.v_rect.left+search_button_width;
    #ifdef FM_RDS_SUPPORT
    both_rect.h_rect.top = win_client_both_rect.h_rect.top + anim_height + GUI_ConvertGUIFontToSize(SONG_FONT_15);
    #else
    both_rect.h_rect.top = MMIFM_H_SEARCH_UPPER_BUTTON_START_Y;
    #endif
    both_rect.h_rect.bottom=both_rect.h_rect.top + search_button_height;
    both_rect.h_rect.left= MMIFM_H_SEARCH_UPPER_BUTTON_START_X;
    both_rect.h_rect.right=both_rect.h_rect.left+search_button_width;
    GUIAPICTRL_SetBothRect(MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,&both_rect);
    
    both_rect.v_rect.left   =both_rect.v_rect.right+MMIFM_V_SEARCH_BUTTON_H_SPACE;
    both_rect.v_rect.right  =both_rect.v_rect.left+search_button_width;    
    both_rect.h_rect.left= both_rect.h_rect.right + MMIFM_H_SEARCH_BUTTON_H_SPACE;
    both_rect.h_rect.right=both_rect.h_rect.left+search_button_width;
    GUIAPICTRL_SetBothRect(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,&both_rect);
    
    both_rect.v_rect.left  =both_rect.v_rect.right+MMIFM_V_SEARCH_BUTTON_H_SPACE;
    both_rect.v_rect.right =both_rect.v_rect.left+search_button_width;    
    both_rect.h_rect.left = both_rect.h_rect.right + MMIFM_H_SEARCH_BUTTON_H_SPACE;
    both_rect.h_rect.right=both_rect.h_rect.left+search_button_width;
    GUIAPICTRL_SetBothRect(MMIFM_GET_UPFRE_BUTTON_CTRL_ID,&both_rect);
}
#else
/*****************************************************************************/
//  Description : create buttons of LCD_SIZA128x160.
//  Global resource dependence : 
//  Parameter: win_id[IN]:button's window id 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void  SetButtonsAndLabelRect(MMI_WIN_ID_T win_id)
{

    GUI_BOTH_RECT_T         both_rect={0};
    uint16 left_width=0;
    uint16 left_height=0;
    uint16 search_button_width=0;
    uint16 search_button_height=0;
    uint16 pause_button_width=0;
    uint16 pause_button_height=0;
 
    GUIRES_GetImgWidthHeight(&search_button_width, &search_button_height, IMAGE_FM_LEFT_DEFAULT, win_id); 
    GUIRES_GetImgWidthHeight(&left_width, &left_height, IMAGE_FM_ADJ_BG, win_id); 
    GUIRES_GetImgWidthHeight(&pause_button_width, &pause_button_height, IMAGE_FM_PAUSE_DEFAULT, win_id);

    //LABEL RECT 
    both_rect.v_rect.top    =MMIFM_V_FREQUENCY_TEXT_START_Y + MMIFM_TITLE_HEIGHT;
    both_rect.v_rect.bottom =both_rect.v_rect.top + FREQUENCY_TEXT_HEIGHT;
    both_rect.v_rect.left   =MMIFM_V_FREQUENCY_TEXT_START_X;
    both_rect.v_rect.right  =both_rect.v_rect.left+MMIFM_V_LABEL_WIDTH;
    
    both_rect.h_rect.top = MMIFM_H_FREQUENCY_TEXT_START_Y + MMIFM_TITLE_HEIGHT;
    both_rect.h_rect.bottom=both_rect.h_rect.top + FREQUENCY_TEXT_HEIGHT;
    both_rect.h_rect.left= MMIFM_H_FREQUENCY_TEXT_START_X;
    both_rect.h_rect.right=both_rect.h_rect.left+MMIFM_H_LABEL_WIDTH;
    GUIAPICTRL_SetBothRect(MMIFM_FREQUENCY_TEXT_CTRL_ID,&both_rect);
    //LEFT ARROW RECT
    both_rect.v_rect.top    =MMIFM_TITLE_HEIGHT +left_height + MMIFM_V_SEARCH_BUTTON_V_SPACE;
    both_rect.v_rect.bottom =both_rect.v_rect.top+search_button_height;
    both_rect.v_rect.left   =MMIFM_V_SEARCH_UPPER_BUTTON_START_X;
    both_rect.v_rect.right  =both_rect.v_rect.left+search_button_width;
    
    both_rect.h_rect.top = MMIFM_TITLE_HEIGHT + left_height + MMIFM_H_SEARCH_BUTTON_V_SPACE;
    both_rect.h_rect.bottom=both_rect.h_rect.top + search_button_height;
    both_rect.h_rect.left= MMIFM_H_SEARCH_UPPER_BUTTON_START_X;
    both_rect.h_rect.right=both_rect.h_rect.left+search_button_width;
    GUIAPICTRL_SetBothRect(MMIFM_GET_DOWNFRE_BUTTON_CTRL_ID,&both_rect);  
    //PAUSE RECT
    both_rect.v_rect.top    =MMIFM_TITLE_HEIGHT + left_height + MMIFM_V_PAUSE_BUTTON_V_SPACE;
    both_rect.v_rect.bottom =both_rect.v_rect.top + pause_button_height;
    both_rect.v_rect.left   =both_rect.v_rect.right+MMIFM_V_SEARCH_BUTTON_H_SPACE;
    both_rect.v_rect.right  =both_rect.v_rect.left+pause_button_width; 

    both_rect.h_rect.top = MMIFM_TITLE_HEIGHT + left_height + MMIFM_H_PAUSE_BUTTON_V_SPACE;
    both_rect.h_rect.bottom=both_rect.h_rect.top + pause_button_height;
    both_rect.h_rect.left= both_rect.h_rect.right + MMIFM_H_SEARCH_BUTTON_H_SPACE;
    both_rect.h_rect.right=both_rect.h_rect.left+pause_button_width;
    GUIAPICTRL_SetBothRect(MMIFM_START_SUSPEND_BUTTON_CTRL_ID,&both_rect);
    //RIGHT ARROW RECT
    both_rect.v_rect.top    =MMIFM_TITLE_HEIGHT + left_height + MMIFM_V_SEARCH_BUTTON_V_SPACE;
    both_rect.v_rect.bottom =both_rect.v_rect.top+search_button_height;
    both_rect.v_rect.left   =both_rect.v_rect.right+MMIFM_V_SEARCH_BUTTON_H_SPACE;
    both_rect.v_rect.right  =both_rect.v_rect.left+search_button_width;  

    both_rect.h_rect.top = MMIFM_TITLE_HEIGHT + left_height + MMIFM_H_SEARCH_BUTTON_V_SPACE;
    both_rect.h_rect.bottom=both_rect.h_rect.top + search_button_height;
    both_rect.h_rect.left= both_rect.h_rect.right + MMIFM_H_SEARCH_BUTTON_H_SPACE;
    both_rect.h_rect.right=both_rect.h_rect.left+search_button_width;
    GUIAPICTRL_SetBothRect(MMIFM_GET_UPFRE_BUTTON_CTRL_ID,&both_rect);      
}
#endif

#endif

#endif
/*Edit by script, ignore 6 case. Thu Apr 26 19:00:54 2012*/ //IGNORE9527


/*Edit by script, ignore 21 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
