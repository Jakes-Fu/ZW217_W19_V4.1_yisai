
#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "mmicc_export.h"
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

#include "zdt_app.h"
#include "mmiidle_export.h"
#include "mmi_position.h"
#include "ldo_drvapi.h"
#include "mmi_resource.h"
#include "mmiset_id.h"
#include "version.h"
#ifdef  CAMERA_SUPPORT
#include "dal_dcamera.h"
#include "sensor_drv.h"
#endif

#define CAMERA_SUB_SUPPORT 0
#ifdef TOUCH_PANEL_SUPPORT
#define FACTORY_TP_TEST_SUPPORT 1
#else
#define FACTORY_TP_TEST_SUPPORT 0
#endif

#define FACTORY_KEY_TEST_SUPPORT 1
#define FACTORY_GPS_TEST_SUPPORT 1
#define FACTORY_WIFI_TEST_SUPPORT 1

typedef struct
{
    int x;
    int y;
} mmi_pen_point_struct;

#define UI_DEVICE_WIDTH  MMI_MAINSCREEN_WIDTH
#define UI_DEVICE_HEIGHT  MMI_MAINSCREEN_HEIGHT

//#define ZDT_WL_SUPPORT

#define SCALE  1

#define FACTORY_BIG_FONT  SONG_FONT_24// SONG_FONT_20
#define FACTORY_NORMAL_FONT SONG_FONT_20 // SONG_FONT_15

PUBLIC BOOLEAN IsWatchListFactoryWinOpen();//yangyu add

//#define ZDT_VIBRATE_SUPPORT
#define FACTORY_KEY_TEST

#ifdef ZDT_NFC_SUPPORT
#define FACTORY_NFC_TEST
#endif


extern void mmi_idle_display(void);
extern void ZdtWatch_Ecg_ShowData(uint16 offset_len);
extern BOOLEAN  Ecg_Blood_Data_Get(uint16* pBloodHData,uint16* pBloodLData);
extern uint8 ecg_hr_bp_hw_on;
    
#define SCREEN_X 	  0
#define SCREEN_Y 	  40
#define SCREEN_DX 	  UI_DEVICE_WIDTH
#define SCREEN_DY	  (UI_DEVICE_HEIGHT-SCREEN_Y)

#define EMAPP_START_X  0
#define EMAPP_START_Y  0
#define EMAPP_FULL_CX  UI_DEVICE_WIDTH

#define WATCH_FACTORY_TITLE_Y	  (2)
#define WATCH_FACTORY_ITEM_HEIGHT   25//	20///  16//(24)*SCALE //yangyu 

typedef void (*PFNDLGHANDLER)(void);
typedef struct _EM_STATUS_METHOD_
{
    PFNDLGHANDLER      DlgStart;
    PFNDLGHANDLER      DlgEnd;
}EmStatusMethodType;

#ifdef ZDT_LCD_NOT_SUPPORT
typedef enum
{
    EM_TEST_BEGIN = 0,
    EM_TEST_SWVER,
    EM_TEST_LOOPBACK,
#ifdef ZDT_WIFI_SUPPORT
    EM_TEST_WIFI,
#endif
#ifdef FACTORY_KEY_TEST
    EM_TEST_KEY,
#endif
#ifdef ZDT_SUPPORT_BEACON
    EM_TEST_BEACON,
#endif
#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
    EM_TEST_GPS,
#endif
    EM_TEST_CALL,
    EM_TEST_END,
    
    EM_TEST_SPEAKER,
#ifdef  CAMERA_SUPPORT
    EM_TEST_CAMERA_MAIN,
#if CAMERA_SUB_SUPPORT
    EM_TEST_CAMERA_SUB,
#endif
#endif
#if FACTORY_TP_TEST_SUPPORT
    EM_TEST_TP,
#endif
    EM_TEST_MAINL,
    EM_TEST_SIM,
#ifdef ZDT_VIBRATE_SUPPORT
    EM_TEST_VIB,
#endif
#ifdef ZDT_GSENSOR_SUPPORT
    EM_TEST_GSENSOR,
#endif
#if defined(ZDT_HSENSOR_SUPPORT) && !defined(ZDT_HSENSOR_SUPPORT_TW)
    EM_TEST_HSENSOR,
    EM_TEST_HSENSOR_PRE,
#endif
#if defined(ZDT_HSENSOR_SUPPORT_TW) || defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
    EM_TEST_TW,
#endif
#ifdef FACTORY_NFC_TEST
    EM_TEST_NFC,
#endif
#ifdef ZDT_WL_SUPPORT
    EM_TEST_WL,
#endif
#if defined(TORCH_SUPPORT)// TORCH_FACTORY_TEST // wuxx add
    EM_TEST_TORCH,
#endif
    EM_TEST_RESTORE,
#ifdef ZDT_FOTA_SUPPORT
    EM_TEST_FOTA,
#endif
}EM_TEST_STAT_E;
#else
typedef enum
{
    EM_TEST_BEGIN = 0,
    EM_TEST_SWVER,
    EM_TEST_CALL,
#ifdef  CAMERA_SUPPORT
    EM_TEST_CAMERA_MAIN,
#if CAMERA_SUB_SUPPORT
    EM_TEST_CAMERA_SUB,
#endif
#endif
    EM_TEST_LOOPBACK,
    EM_TEST_SPEAKER,
#if FACTORY_TP_TEST_SUPPORT
    EM_TEST_TP,
#endif
#ifdef FACTORY_KEY_TEST
    EM_TEST_KEY,
#endif
    EM_TEST_MAINL,
#ifdef ZDT_WIFI_SUPPORT
    EM_TEST_WIFI,
#endif
    EM_TEST_SIM,
#ifdef ZDT_VIBRATE_SUPPORT
    EM_TEST_VIB,
#endif
#ifdef ZDT_GSENSOR_SUPPORT
    EM_TEST_GSENSOR,
#endif
#if defined(ZDT_HSENSOR_SUPPORT) && !defined(ZDT_HSENSOR_SUPPORT_TW)
    EM_TEST_HSENSOR,
    EM_TEST_HSENSOR_PRE,
#endif
#if defined(ZDT_HSENSOR_SUPPORT_TW) || defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
    EM_TEST_TW,
#endif
#ifdef ZDT_SUPPORT_BEACON
    EM_TEST_BEACON,
#endif
#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
    EM_TEST_GPS,
#endif
#ifdef FACTORY_NFC_TEST
    EM_TEST_NFC,
#endif
#ifdef ZDT_WL_SUPPORT
    EM_TEST_WL,
#endif
#if defined(TORCH_SUPPORT)// TORCH_FACTORY_TEST // wuxx add
    EM_TEST_TORCH,
#endif
    EM_TEST_RESTORE,
#ifdef ZDT_FOTA_SUPPORT
    EM_TEST_FOTA,
#endif
    EM_TEST_END
}EM_TEST_STAT_E;
#endif

const char zdt_sw_rev[] = ZDT_SFR_SW_VER;
EM_TEST_STAT_E g_em_test_idx = EM_TEST_BEGIN;
static EM_TEST_STAT_E fastctory_cur_active = 0;
static BOOLEAN fastctory_test_one = FALSE;

uint8 g_facotry_disp_timer_id = 0;
uint8 g_facotry_tp_timer_id = 0;

LOCAL BOOLEAN is_just_entry_call = FALSE;


#if defined(W217_AGING_TEST_CUSTOM)//wuxx add.
BOOLEAN g_factory_aging_test_flag = FALSE;
#endif

extern BOOLEAN ZdtWatch_Factory_StartCur(void);
extern BOOLEAN ZdtWatch_Factory_EndCur(void);
extern BOOLEAN ZdtWatch_Factory_StartNext(void);

extern void ZdtWatch_Factory_SWVER_Exit(void);
extern void ZdtWatch_Factory_SWVER_Entry(void);

extern void ZdtWatch_Factory_CALL_Exit(void);
extern void ZdtWatch_Factory_CALL_Entry(void);

extern void ZdtWatch_Factory_SIM_Exit(void);
extern void ZdtWatch_Factory_SIM_Entry(void);
extern void ZdtWatch_Factory_LoopBack_Exit(void);
extern void ZdtWatch_Factory_LoopBack_Entry(void);
extern void ZdtWatch_Factory_Speaker_Exit(void);
extern void ZdtWatch_Factory_Speaker_Entry(void);
extern void ZdtWatch_Factory_MAINL_Exit(void);
extern void ZdtWatch_Factory_MAINL_Entry(void);
#if FACTORY_TP_TEST_SUPPORT
extern void ZdtWatch_Factory_TP_Exit(void);
extern void ZdtWatch_Factory_TP_Entry(void);
#endif
#ifdef FACTORY_KEY_TEST
extern void ZdtWatch_Factory_KEY_Exit(void);
extern void ZdtWatch_Factory_KEY_Entry(void);
#endif

#ifdef ZDT_VIBRATE_SUPPORT
extern void ZdtWatch_Factory_VIB_Exit(void);
extern void ZdtWatch_Factory_VIB_Entry(void);
#endif
#ifdef ZDT_GSENSOR_SUPPORT
extern void ZdtWatch_Factory_GSENSOR_Exit(void);
extern void ZdtWatch_Factory_GSENSOR_Entry(void);
#endif
#if defined(ZDT_HSENSOR_SUPPORT) && !defined(ZDT_HSENSOR_SUPPORT_TW)
extern void ZdtWatch_Factory_HSENSOR_Exit(void);
extern void ZdtWatch_Factory_HSENSOR_Entry(void);
extern void ZdtWatch_Factory_HSENSOR_Pre_Exit(void);
extern void ZdtWatch_Factory_HSENSOR_Pre_Entry(void);
#endif
#if defined(ZDT_HSENSOR_SUPPORT_TW) || defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
extern void ZdtWatch_Factory_TW_Exit(void);
extern void ZdtWatch_Factory_TW_Entry(void);
#endif
#ifdef ZDT_SUPPORT_BEACON
extern void ZdtWatch_Factory_BEACON_Exit(void);
extern void ZdtWatch_Factory_BEACON_Entry(void);
#endif

#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
extern void ZdtWatch_Factory_GPS_Exit(void);
extern void ZdtWatch_Factory_GPS_Entry(void);
#endif
#ifdef FACTORY_NFC_TEST
extern void ZdtWatch_Factory_NFC_Exit(void);
extern void ZdtWatch_Factory_NFC_Entry(void);
#endif
#ifdef ZDT_WL_SUPPORT
extern void ZdtWatch_Factory_WL_Exit(void);
extern void ZdtWatch_Factory_WL_Entry(void);
#endif
#if defined(TORCH_SUPPORT)// TORCH_FACTORY_TEST // wuxx add
extern void ZdtWatch_Factory_TORCH_Exit(void);
extern void ZdtWatch_Factory_TORCH_Entry(void);
#endif

#ifdef ZDT_WIFI_SUPPORT
extern void ZdtWatch_Factory_WIFI_Exit(void);
extern void ZdtWatch_Factory_WIFI_Entry(void);
#endif
#ifdef  CAMERA_SUPPORT
extern void ZdtWatch_Factory_CameraMain_Exit(void);
extern void ZdtWatch_Factory_CameraMain_Entry(void);
#if CAMERA_SUB_SUPPORT
extern void ZdtWatch_Factory_CameraSub_Exit(void);
extern void ZdtWatch_Factory_CameraSub_Entry(void);
#endif
#endif
extern void ZdtWatch_Factory_RESTORE_Exit(void);
extern void ZdtWatch_Factory_RESTORE_Entry(void);
#ifdef ZDT_FOTA_SUPPORT
extern void ZdtWatch_Factory_FOTA_Exit(void);
extern void ZdtWatch_Factory_FOTA_Entry(void);
#endif

#ifdef ZDT_PLAY_RING_CUSTOM
PUBLIC BOOLEAN MMIZDT_Play_IMEI(void)
{
    MMIAPISET_PlayCustomRing(MMISET_CUST_ID_ERR,1);
    MMIAPISET_PlayCustomRingNumStr(&g_zdt_phone_imei[0],15);
}

PUBLIC BOOLEAN MMIZDT_Play_Ver(void)
{
    MMIAPISET_PlayCustomRing(MMISET_CUST_FOTA_VER,1);
    MMIAPISET_PlayCustomRingNumStr(&zdt_sw_rev[1],2);
}
#endif

#ifdef ZDT_LCD_NOT_SUPPORT
static const EmStatusMethodType EMDlgEventHandlers[] = {
      NULL,NULL,
	ZdtWatch_Factory_SWVER_Entry, ZdtWatch_Factory_SWVER_Exit,
	ZdtWatch_Factory_LoopBack_Entry, ZdtWatch_Factory_LoopBack_Exit,
#ifdef ZDT_WIFI_SUPPORT
	ZdtWatch_Factory_WIFI_Entry, ZdtWatch_Factory_WIFI_Exit,
#endif
#ifdef FACTORY_KEY_TEST
	ZdtWatch_Factory_KEY_Entry, ZdtWatch_Factory_KEY_Exit,
#endif
#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
	ZdtWatch_Factory_GPS_Entry, ZdtWatch_Factory_GPS_Exit,
#endif
	ZdtWatch_Factory_CALL_Entry, ZdtWatch_Factory_CALL_Exit,
	NULL,NULL,
};
#else
static const EmStatusMethodType EMDlgEventHandlers[] = {
      NULL,NULL,
	ZdtWatch_Factory_SWVER_Entry, ZdtWatch_Factory_SWVER_Exit,
	ZdtWatch_Factory_CALL_Entry, ZdtWatch_Factory_CALL_Exit,
#ifdef  CAMERA_SUPPORT
	ZdtWatch_Factory_CameraMain_Entry, ZdtWatch_Factory_CameraMain_Exit,
#if CAMERA_SUB_SUPPORT
	ZdtWatch_Factory_CameraSub_Entry, ZdtWatch_Factory_CameraSub_Exit,
#endif
#endif
	ZdtWatch_Factory_LoopBack_Entry, ZdtWatch_Factory_LoopBack_Exit,
	ZdtWatch_Factory_Speaker_Entry, ZdtWatch_Factory_Speaker_Exit,
#if FACTORY_TP_TEST_SUPPORT
	ZdtWatch_Factory_TP_Entry, ZdtWatch_Factory_TP_Exit,
#endif
#ifdef FACTORY_KEY_TEST
	ZdtWatch_Factory_KEY_Entry, ZdtWatch_Factory_KEY_Exit,
#endif
	ZdtWatch_Factory_MAINL_Entry, ZdtWatch_Factory_MAINL_Exit,
#ifdef ZDT_WIFI_SUPPORT
	ZdtWatch_Factory_WIFI_Entry, ZdtWatch_Factory_WIFI_Exit,
#endif
	ZdtWatch_Factory_SIM_Entry, ZdtWatch_Factory_SIM_Exit,
#ifdef ZDT_VIBRATE_SUPPORT
	ZdtWatch_Factory_VIB_Entry, ZdtWatch_Factory_VIB_Exit,
#endif
#ifdef ZDT_GSENSOR_SUPPORT
	ZdtWatch_Factory_GSENSOR_Entry, ZdtWatch_Factory_GSENSOR_Exit,
#endif
#if defined(ZDT_HSENSOR_SUPPORT) && !defined(ZDT_HSENSOR_SUPPORT_TW)
	ZdtWatch_Factory_HSENSOR_Entry, ZdtWatch_Factory_HSENSOR_Exit,
	ZdtWatch_Factory_HSENSOR_Pre_Entry, ZdtWatch_Factory_HSENSOR_Pre_Exit,
#endif
#if defined(ZDT_HSENSOR_SUPPORT_TW) || defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
	ZdtWatch_Factory_TW_Entry, ZdtWatch_Factory_TW_Exit,
#endif
#ifdef ZDT_SUPPORT_BEACON
	ZdtWatch_Factory_BEACON_Entry, ZdtWatch_Factory_BEACON_Exit,
#endif
#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
	ZdtWatch_Factory_GPS_Entry, ZdtWatch_Factory_GPS_Exit,
#endif
#ifdef FACTORY_NFC_TEST
	ZdtWatch_Factory_NFC_Entry, ZdtWatch_Factory_NFC_Exit,
#endif
#ifdef ZDT_WL_SUPPORT
	ZdtWatch_Factory_WL_Entry, ZdtWatch_Factory_WL_Exit,
#endif
#if defined(TORCH_SUPPORT)// TORCH_FACTORY_TEST // wuxx add
       ZdtWatch_Factory_TORCH_Entry, ZdtWatch_Factory_TORCH_Exit,
#endif
	ZdtWatch_Factory_RESTORE_Entry, ZdtWatch_Factory_RESTORE_Exit,
#ifdef ZDT_FOTA_SUPPORT
	ZdtWatch_Factory_FOTA_Entry, ZdtWatch_Factory_FOTA_Exit,
#endif
	NULL,NULL,
};
#endif
static void ZdtWatch_BackLight(BOOLEAN is_alway_on)
{
      if(is_alway_on)
      {
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		MMIDEFAULT_TurnOnBackLight();
      }
      else
      {
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
      }
}

#ifdef FACTORY_KEY_TEST
BOOLEAN	g_ecg_key_enter_status = FALSE;
BOOLEAN	g_ecg_key_power_status = FALSE;
void ZdtWatch_Display_TimeStop(void);

#define ZDT_TEST_KEY_TEXT_MAX_LEN             9
#define ZDT_TEST_KEY_MARGIN_X			8
#define ZDT_TEST_KEY_MARGIN_Y			8
#define ZDT_TEST_KEY_HIGHT		40
#if defined(ZDT_W206_FACTORY_KEY_TEST)// only one key
#define ZDT_TEST_KEY_NUM		1
#else
#define ZDT_TEST_KEY_NUM		5
#endif

static uint32 s_zdt_test_key_flag;
static uint8 s_zdt_key_ind_buf[2] = {0x00,0x00};
#if defined(ZDT_W206_FACTORY_KEY_TEST)// only one key
#define ZDT_TEST_KEY_COMPLETE_FLAG		0x01
#else
#define ZDT_TEST_KEY_COMPLETE_FLAG		0x1f
#endif

#if defined(ZDT_W206_FACTORY_KEY_TEST)// only one key
typedef enum
{
	ZDT_TEST_KEY_RED,
	ZDT_TEST_KEY_MAX
} ZDT_TEST_KEY_E;
#else
typedef enum
{
	ZDT_TEST_KEY_UP=0x00,
	ZDT_TEST_KEY_SOS,
	ZDT_TEST_KEY_DOWN,
	ZDT_TEST_KEY_CALL,
	ZDT_TEST_KEY_RED,
	ZDT_TEST_KEY_MAX
} ZDT_TEST_KEY_E;
#endif
LOCAL void ZDT_TestEditWinDrawFocusRect(
                                GUI_RECT_T rect,
                                GUI_COLOR_T color
                                )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};    
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.top, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.left, rect.top, rect.bottom, color);
    LCD_DrawHLine(&lcd_dev_info, rect.left, rect.bottom, rect.right, color);
    LCD_DrawVLine(&lcd_dev_info, rect.right, rect.top, rect.bottom, color);
}

#if defined(ZDT_W206_FACTORY_KEY_TEST)// only one key
LOCAL void ZdtWatch_Factory_KEY_DrawTestKeyBG( void )
{
    uint16      x = 0;
    uint16      y = 0;
    wchar       disp_text[9] = {0};
    GUI_RECT_T  draw_rect = MMITHEME_GetFullScreenRect();
    int16       TESTPAD_KEY_WIDTH = (draw_rect.right - draw_rect.left - ZDT_TEST_KEY_MARGIN_X*2)>>2 ;
    const GUI_LCD_DEV_INFO  *lcd_dev_info = MMITHEME_GetDefaultLcdDev() ;
    MMI_STRING_T        string = {0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
	
    char s_test_key_text[][9] =
    {
        {"RED"}, 
    };
	    
    draw_rect.top = 0;
    LCD_FillRect(lcd_dev_info, draw_rect, MMI_WHITE_COLOR);
    
    for (y=0;y<1;y++)
    {
        for (x=0;x<1;x++)
        {
            draw_rect.left = (uint16)ZDT_TEST_KEY_MARGIN_X + (uint16)(x * TESTPAD_KEY_WIDTH);
            draw_rect.top = ZDT_TEST_KEY_MARGIN_Y + y * ZDT_TEST_KEY_HIGHT;
            draw_rect.right = draw_rect.left + TESTPAD_KEY_WIDTH-1;
            draw_rect.bottom = draw_rect.top + ZDT_TEST_KEY_HIGHT-1;
            ZDT_TestEditWinDrawFocusRect(draw_rect, MMI_BLACK_COLOR);
            
            string.wstr_len = strlen(s_test_key_text[y*1+x]);
            MMI_STRNTOWSTR(disp_text,ZDT_TEST_KEY_TEXT_MAX_LEN, (uint8*)s_test_key_text[y*3+x],string.wstr_len,string.wstr_len);
            string.wstr_ptr = disp_text;
            
            // display
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = MMI_DEFAULT_TEXT_FONT;
            text_style.font_color = MMI_BLACK_COLOR;
            //  for(;GUI_GetStringWidth(text_style.font, string.wstr_ptr,string.wstr_len)>(TESTPAD_KEY_WIDTH-1)&&(text_style.font>2);text_style.font-=2);
            while(GUI_GetStringWidth(text_style.font, string.wstr_ptr,string.wstr_len)>(TESTPAD_KEY_WIDTH-1)&&(text_style.font>2))
            {
                text_style.font-=2    ;         
            }
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)lcd_dev_info,
                (const GUI_RECT_T      *)&draw_rect,       //the fixed display area
                (const GUI_RECT_T      *)&draw_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&string,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
        }
    }
    
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN ZDT_TestKeyboard(MMI_MESSAGE_ID_E key_msg_id)
{
	int16               test_key_x      = 0;
	int16               test_key_y      = 0;
	int16               key_on_map_idx  = 0;
	int16               byte_idx        = 0;
	int16               bit_idx         = 0;
	uint8               key_buf_byte    = 0x00;
	GUI_RECT_T          draw_rect       = {0};
	GUI_RECT_T          update_rect     = {0};
	GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	
	
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_1908_112_2_18_2_17_27_62,(uint8*)"d",key_msg_id);
	
	
	switch (key_msg_id)
	{
        case MSG_KEYUP_RED:
              test_key_y = 0;
              test_key_x = 0;
              s_zdt_test_key_flag |= (0x00000001 << ZDT_TEST_KEY_RED);
         break;

	default:
		break;
    }
    key_on_map_idx = test_key_y * 1 + test_key_x;
    key_on_map_idx %= ZDT_TEST_KEY_NUM;
    byte_idx = key_on_map_idx / 8;
    bit_idx = key_on_map_idx % 8;
    
    key_buf_byte = s_zdt_key_ind_buf[byte_idx];
    s_zdt_key_ind_buf[byte_idx] &= (unsigned char)(~(1<<bit_idx));/*lint !e502*/  
    // show key passed
    if ((key_buf_byte^s_zdt_key_ind_buf[byte_idx]) != 0)
    {        
        {        
            GUI_RECT_T rect = MMITHEME_GetClientRect();
            int32 TESTPAD_KEY_WIDTH = (rect.right- rect.left - ZDT_TEST_KEY_MARGIN_X*2)>>2 ;
            
            draw_rect.left = ZDT_TEST_KEY_MARGIN_X + test_key_x * TESTPAD_KEY_WIDTH;
            draw_rect.top = ZDT_TEST_KEY_MARGIN_X + test_key_y * ZDT_TEST_KEY_HIGHT;
            draw_rect.right = draw_rect.left + TESTPAD_KEY_WIDTH - 1;
            draw_rect.bottom = draw_rect.top + ZDT_TEST_KEY_HIGHT-1;
            LCD_FillRect(&lcd_dev_info, draw_rect, MMI_BLACK_COLOR);
            
            update_rect.right = rect.right;
            update_rect.bottom = rect.bottom;
            GUILCD_InvalidateRect(GUI_MAIN_LCD_ID,update_rect, GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN));
        }
    }
   
    return (0);
}


#else
LOCAL void ZdtWatch_Factory_KEY_DrawTestKeyBG( void )
{
    uint16      x = 0;
    uint16      y = 0;
    wchar       disp_text[9] = {0};
    GUI_RECT_T  draw_rect = MMITHEME_GetFullScreenRect();
    int16       TESTPAD_KEY_WIDTH = (draw_rect.right - draw_rect.left - ZDT_TEST_KEY_MARGIN_X*2)>>2 ;
    const GUI_LCD_DEV_INFO  *lcd_dev_info = MMITHEME_GetDefaultLcdDev() ;
    MMI_STRING_T        string = {0};
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
	
    char s_test_key_text[][9] =
    {
        {"1"},      {"SOS"}, {"2"},      
        {"3"},      {"RED"},  {" "}
    };
	    
    draw_rect.top = 0;
    LCD_FillRect(lcd_dev_info, draw_rect, MMI_WHITE_COLOR);
    
    for (y=0;y<2;y++)
    {
        for (x=0;x<3;x++)
        {
            draw_rect.left = (uint16)ZDT_TEST_KEY_MARGIN_X + (uint16)(x * TESTPAD_KEY_WIDTH);
            draw_rect.top = ZDT_TEST_KEY_MARGIN_Y + y * ZDT_TEST_KEY_HIGHT;
            draw_rect.right = draw_rect.left + TESTPAD_KEY_WIDTH-1;
            draw_rect.bottom = draw_rect.top + ZDT_TEST_KEY_HIGHT-1;
            ZDT_TestEditWinDrawFocusRect(draw_rect, MMI_BLACK_COLOR);
            
            string.wstr_len = strlen(s_test_key_text[y*3+x]);
            MMI_STRNTOWSTR(disp_text,ZDT_TEST_KEY_TEXT_MAX_LEN, (uint8*)s_test_key_text[y*3+x],string.wstr_len,string.wstr_len);
            string.wstr_ptr = disp_text;
            
            // display
            text_style.align = ALIGN_HVMIDDLE;
            text_style.font = MMI_DEFAULT_TEXT_FONT;
            text_style.font_color = MMI_BLACK_COLOR;
            //  for(;GUI_GetStringWidth(text_style.font, string.wstr_ptr,string.wstr_len)>(TESTPAD_KEY_WIDTH-1)&&(text_style.font>2);text_style.font-=2);
            while(GUI_GetStringWidth(text_style.font, string.wstr_ptr,string.wstr_len)>(TESTPAD_KEY_WIDTH-1)&&(text_style.font>2))
            {
                text_style.font-=2    ;         
            }
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)lcd_dev_info,
                (const GUI_RECT_T      *)&draw_rect,       //the fixed display area
                (const GUI_RECT_T      *)&draw_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&string,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
        }
    }
    
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN ZDT_TestKeyboard(MMI_MESSAGE_ID_E key_msg_id)
{
	int16               test_key_x      = 0;
	int16               test_key_y      = 0;
	int16               key_on_map_idx  = 0;
	int16               byte_idx        = 0;
	int16               bit_idx         = 0;
	uint8               key_buf_byte    = 0x00;
	GUI_RECT_T          draw_rect       = {0};
	GUI_RECT_T          update_rect     = {0};
	GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	
	
	//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_UITESTWIN_1908_112_2_18_2_17_27_62,(uint8*)"d",key_msg_id);
	
	
	switch (key_msg_id)
	{
	case MSG_KEYDOWN_UP:
		test_key_y = 0;
		test_key_x = 0;
		s_zdt_test_key_flag |= (0x00000001 << ZDT_TEST_KEY_UP);
#ifdef ZDT_PLAY_RING_CUSTOM
            MMIAPISET_StopCustomRing();
            MMIAPISET_PlayCustomRing(MMISET_CUST_NUM_1,1);
#endif
		break;
	case MSG_KEYDOWN_OK:
		test_key_y = 0;
		test_key_x = 1;
		s_zdt_test_key_flag |= (0x00000001 << ZDT_TEST_KEY_SOS);
#ifdef ZDT_PLAY_RING_CUSTOM
            MMIAPISET_StopCustomRing();
            MMIAPISET_PlayCustomRing(MMISET_CUST_NUM_2,1);
#endif
		break;
	case MSG_KEYDOWN_DOWN:
                test_key_y = 0;
                test_key_x = 2;
                s_zdt_test_key_flag |= (0x00000001 << ZDT_TEST_KEY_DOWN);
#ifdef ZDT_PLAY_RING_CUSTOM
            MMIAPISET_StopCustomRing();
            MMIAPISET_PlayCustomRing(MMISET_CUST_NUM_3,1);
#endif
                break;
	case MSG_KEYDOWN_GREEN:
	   test_key_y = 1;
	   test_key_x = 0;
	   s_zdt_test_key_flag |= (0x00000001 << ZDT_TEST_KEY_CALL);
#ifdef ZDT_PLAY_RING_CUSTOM
            MMIAPISET_StopCustomRing();
            MMIAPISET_PlayCustomRing(MMISET_CUST_NUM_4,1);
#endif
	   break;
	case MSG_KEYUP_CANCEL:
          test_key_y = 1;
          test_key_x = 1;
          s_zdt_test_key_flag |= (0x00000001 << ZDT_TEST_KEY_RED);
#ifdef ZDT_PLAY_RING_CUSTOM
            MMIAPISET_StopCustomRing();
            MMIAPISET_PlayCustomRing(MMISET_CUST_NUM_5,1);
#endif
	   break;
	default:
		break;
    }
    key_on_map_idx = test_key_y * 3 + test_key_x;
    key_on_map_idx %= ZDT_TEST_KEY_NUM;
    byte_idx = key_on_map_idx / 8;
    bit_idx = key_on_map_idx % 8;
    
    key_buf_byte = s_zdt_key_ind_buf[byte_idx];
    s_zdt_key_ind_buf[byte_idx] &= (unsigned char)(~(1<<bit_idx));/*lint !e502*/  
    // show key passed
    if ((key_buf_byte^s_zdt_key_ind_buf[byte_idx]) != 0)
    {        
        {        
            GUI_RECT_T rect = MMITHEME_GetClientRect();
            int32 TESTPAD_KEY_WIDTH = (rect.right- rect.left - ZDT_TEST_KEY_MARGIN_X*2)>>2 ;
            
            draw_rect.left = ZDT_TEST_KEY_MARGIN_X + test_key_x * TESTPAD_KEY_WIDTH;
            draw_rect.top = ZDT_TEST_KEY_MARGIN_X + test_key_y * ZDT_TEST_KEY_HIGHT;
            draw_rect.right = draw_rect.left + TESTPAD_KEY_WIDTH - 1;
            draw_rect.bottom = draw_rect.top + ZDT_TEST_KEY_HIGHT-1;
            LCD_FillRect(&lcd_dev_info, draw_rect, MMI_BLACK_COLOR);
            
            update_rect.right = rect.right;
            update_rect.bottom = rect.bottom;
            GUILCD_InvalidateRect(GUI_MAIN_LCD_ID,update_rect, GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN));
        }
    }
   
    return (0);
}
#endif


void ZdtWatch_Factory_KEY_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_KEY)
    {
        ZdtWatch_Factory_StartNext();
    }
}

void ZdtWatch_Factory_KEY_ShowData(void)
{

    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[60] = {0x6309, 0x952E, 0x6D4B, 0x8BD5,0}; //按键测试6309 952E 6D4B 8BD5 
    uint16 cal_ok[10] = {0x5DF2, 0x6821, 0x51C6,0}; //已校准5DF2 6821 51C6 
    uint16 cal_fail[10] = {0x672A, 0x6821, 0x51C6,0}; //未校准672A 6821 51C6 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    BOOLEAN is_calibration = FALSE; //zdt_app_is_calibrationed();

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
       s_zdt_key_ind_buf[0] = 0xFF;
       s_zdt_test_key_flag = 0;
	ZdtWatch_Factory_KEY_DrawTestKeyBG();
    return;
}

void ZdtWatch_Factory_KEY_Exit(void)
{
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_KEY_Entry(void)
{
    g_ecg_key_enter_status = FALSE;
    g_ecg_key_power_status = FALSE;
    ZdtWatch_Factory_KEY_ShowData();
#ifdef ZDT_PLAY_RING_CUSTOM
    MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_KEY,1);
#endif
    return;
}
#endif

void ZdtWatch_Display_TimeStart(uint32 time_ms,MMI_TIMER_FUNC  func)
{
    if(0 != g_facotry_disp_timer_id)
    {
        MMK_StopTimer(g_facotry_disp_timer_id);
        g_facotry_disp_timer_id = 0;
    }
    
    g_facotry_disp_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        func, 
                                                                        (uint32)PNULL, 
                                                                        FALSE);
}

void ZdtWatch_Display_TimeStop(void)
{
    if(0 != g_facotry_disp_timer_id)
    {
        MMK_StopTimer(g_facotry_disp_timer_id);
        g_facotry_disp_timer_id = 0;
    }
}

PUBLIC char* ZDT_GetVersionInfo()
{
    #ifdef WIN32
    char *disp_str = "ZW31F_ZDT_Z001_V01_202009121010";
    return disp_str;

    #else
    char disp_str[100] = {0};
    sprintf((char*)disp_str, (char*) "%s_%s", zdt_sw_rev, VERSION_GetInfo(BUILD_TIME));
    return disp_str;
    #endif

}
//yangyu end

#if 1
void ZdtWatch_Factory_SWVER_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[60] = {0x8F6F, 0x4EF6, 0x7248, 0x672C,0}; //软件版本 8F6F 4EF6 7248 672C 
    uint16 cal_ok[10] = {0x5DF2, 0x6821, 0x51C6,0}; //已校准5DF2 6821 51C6 
    uint16 cal_fail[10] = {0x672A, 0x6821, 0x51C6,0}; //未校准672A 6821 51C6 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    BOOLEAN is_calibration = FALSE; //zdt_app_is_calibrationed();
    uint32 cal_info = 0;
    
    is_calibration = ZDT_CheckChipCalibrationed(&cal_info);

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    text_style.font = f_mid-1;//yangyu modify
    cur_rect.top    = cur_rect.bottom+WATCH_FACTORY_ITEM_HEIGHT -10; //yangyu
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT +15;//yangyu
    //cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s", zdt_sw_rev);
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s_%s", zdt_sw_rev, VERSION_GetInfo(BUILD_TIME));//yangyu add
    #ifndef WIN32
    //cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "CAM: 0x%02x_0x%02x", g_sensor_bf30a2_id_buf[0], g_sensor_bf30a2_id_buf[1]);//yangyu add
    #endif
    state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK; 
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    text_style.font = f_mid;//yangyu modify
#ifdef ZDT_NET_SUPPORT
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+2;//10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "IMEI: %s",g_zdt_phone_imei);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    if(ZDT_SIM_Exsit() && MMIZDT_Net_IsInit())
    {
        SCI_MEMSET(disp_str,0,sizeof(disp_str));
        SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
        cur_rect.top    = cur_rect.bottom+2;//10*SCALE; 
        cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "IMSI: %s",g_zdt_sim_imsi);
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 
        
        SCI_MEMSET(disp_str,0,sizeof(disp_str));
        SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
        cur_rect.top    = cur_rect.bottom+0;//10*SCALE; 
        cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT+10;
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "ICCID: %s",g_zdt_sim_iccid);
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 
    }
#endif
#if 0//def ZDT_ZFB_SUPPORT
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    temp_len = 100;
    ls_hal_get_dev_sn(temp_buf,&temp_len);
    cur_rect.top    = cur_rect.bottom+2;//10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "SN:%s",temp_buf);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+2;//10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "IMEI: %s",g_zdt_phone_imei);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    SCI_MEMSET(temp_buf,0,sizeof(temp_buf));
    temp_len = 100;
    ls_hal_get_dev_mac(temp_buf,&temp_len);
    cur_rect.top    = cur_rect.bottom+2;//10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "MAC:%s",temp_buf);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
#endif
    if(is_calibration)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cal_ok);
        cur_str_t.wstr_ptr = cal_ok;
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cal_fail);
        cur_str_t.wstr_ptr = cal_fail;
    }
    cur_rect.top    = cur_rect.bottom+13;//26*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    return;
}

void ZdtWatch_Factory_SWVER_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_SWVER_Exit");
    ZdtWatch_Display_TimeStop();
    return;
}

#ifdef ZDT_PLAY_RING_CUSTOM
void ZdtWatch_Factory_SWVER_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_SWVER)
    {
        MMIAPISET_StopCustomRing();
        ZdtWatch_Factory_StartNext();
    }
}
#endif

void ZdtWatch_Factory_SWVER_Entry(void)
{
#ifdef ZDT_PLAY_RING_CUSTOM
    uint32 cal_info = 0;
    BOOLEAN is_calibration = FALSE;
#endif
    //ZDT_LOG("ZdtWatch_Factory_SWVER_Entry");
    ZdtWatch_Factory_SWVER_ShowData();
    is_just_entry_call = TRUE;
#ifdef ZDT_PLAY_RING_CUSTOM
    is_calibration = ZDT_CheckChipCalibrationed(&cal_info);
    if(is_calibration == FALSE)
    {
        MMIAPISET_PlayCustomRing(MMISET_CUST_NO_CALIB,1);
    }
    else
    {
        MMIZDT_Play_Ver();
        MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_LOOPBACK,1);
        ZdtWatch_Display_TimeStart(6000, ZdtWatch_Factory_SWVER_HandleTimer);
    }
#endif
    return;
}
#endif

#if 1
void ZdtWatch_Factory_SIM_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x7CFB, 0x7EDF, 0x72B6, 0x6001,0}; //系统状态7CFB 7EDF 72B6 6001 
    uint16 sim_ok[10] = {0x0053, 0x0049, 0x004D, 0x5361,0x5DF2, 0x63D2,0x5165,0}; //SIM卡已插入0053 0049 004D 5361 5DF2 63D2 5165 
    uint16 no_sim[10] = {0x65E0, 0x0053, 0x0049, 0x004D, 0x5361, 0}; //无SIM卡65E0 0053 0049 004D 5361 
    uint16 ip_port[20] = {0x0049, 0x0050, 0x5730, 0x5740, 0x003A, 0}; //IP地址:0049 0050 5730 5740 003A 
    uint16 charge_txt[20] = {0x5145, 0x7535, 0x4E2D, 0x0}; //充电中5145 7535 4E2D 
    uint16 charge_out_t[20] = {0x672A, 0x5145, 0x7535, 0x0}; //未充电672A 5145 7535 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =SONG_FONT_16;//FACTORY_NORMAL_FONT;
    BOOLEAN is_charge = ZDT_GetIsCharge();
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = f_mid;

    if(ZDT_SIM_Exsit())
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(sim_ok);
        cur_str_t.wstr_ptr = sim_ok;
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(no_sim);
        cur_str_t.wstr_ptr = no_sim;
    }
    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    if(is_charge)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(charge_txt);
        cur_str_t.wstr_ptr = charge_txt;
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(charge_out_t);
        cur_str_t.wstr_ptr = charge_out_t;
    }
    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    //电池电压:7535 6C60 7535 538B 003A 
    disp_wstr[0] = 0x7535;
    disp_wstr[1] = 0x6C60;
    disp_wstr[2] = 0x7535;
    disp_wstr[3] = 0x538B;
    disp_wstr[4] = 0x003A;
    disp_wstr[5] = 0x0;

    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    #ifndef WIN32
    //cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "0x%02x,0x%02x", g_sensor_bf30a2_id_buf[0], g_sensor_bf30a2_id_buf[1]);//yangyu add
    #endif

    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) " %d (%d%%)", p_chgmng_info->bat_cur_vol,p_chgmng_info->bat_remain_cap);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(&disp_wstr[5], cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    cur_str_t.wstr_len += 5;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    #ifdef ZDT_PLAT_YX_SUPPORT
    //网络状态:7F51 7EDC 72B6 6001 003A 
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    disp_wstr[0] = 0x7F51;
    disp_wstr[1] = 0x7EDC;
    disp_wstr[2] = 0x72B6;
    disp_wstr[3] = 0x6001;
    disp_wstr[4] = 0x003A;
    disp_wstr[5] = 0x0;
    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) " %d,%d", yx_DB_Set_Rec.net_open,YX_Net_Is_Land());
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(&disp_wstr[5], cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    cur_str_t.wstr_len += 5;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
     //IP地址:0049 0050 5730 5740 003A 
        SCI_MEMSET(disp_str,0,sizeof(disp_str));
        SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
        disp_wstr[0] = 0x0049;
        disp_wstr[1] = 0x0050;
        disp_wstr[2] = 0x5730;
        disp_wstr[3] = 0x5740;
        disp_wstr[4] = 0x003A;
        disp_wstr[5] = 0x0;
        cur_rect.top    = cur_rect.bottom; 
        cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) " %s", yx_DB_Set_Rec.ip_domain);
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(&disp_wstr[5], cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        cur_str_t.wstr_len += 5;
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
        
    //端口:7AEF 53E3 003A 
        SCI_MEMSET(disp_str,0,sizeof(disp_str));
        SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
        disp_wstr[0] = 0x7AEF;
        disp_wstr[1] = 0x53E3;
        disp_wstr[2] = 0x003A;
        disp_wstr[3] = 0x0;
        cur_rect.top    = cur_rect.bottom; 
        cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) " %d", yx_DB_Set_Rec.ip_port);
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(&disp_wstr[3], cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        cur_str_t.wstr_len += 3;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    #endif
    return;
}


void ZdtWatch_Factory_SIM_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_SIM)
    {
        ZdtWatch_Factory_SIM_ShowData();
        ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_SIM_HandleTimer);
    }
}

void ZdtWatch_Factory_SIM_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_SIM_Exit");
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_SIM_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_SIM_Entry");
    ZdtWatch_Factory_SIM_ShowData();
    ZdtWatch_Display_TimeStop();
    ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_SIM_HandleTimer);
    return;
}
#endif

#if 1
static void ZdtWatch_Factory_LoopBack_Stop(void)
{
    #if defined(W18_FACTORY_LOOP_BACK_TEST)
    MMISRVAUD_EnableVoiceLB(FALSE, 6) ; // 3-6
    #else
    MMISRVAUD_EnableVoiceLB(FALSE, 9) ; //yangyu delete modify
    #endif
    //MMISRVAUD_EnableVoiceLBWithRoute(FALSE, 9, MMISRVAUD_ROUTE_SPEAKER) ;
    return;
}

extern void ZdtWatch_Factory_LoopBack_Start(void)
{
    #if defined(W18_FACTORY_LOOP_BACK_TEST)
    MMISRVAUD_EnableVoiceLB(TRUE, 3) ;
    #else
    MMISRVAUD_EnableVoiceLB(TRUE, 9) ;//yangyu delete modify
    #endif
    //MMISRVAUD_EnableVoiceLBWithRoute(TRUE, 9, MMISRVAUD_ROUTE_SPEAKER) ;
    return;
}

void ZdtWatch_Factory_LoopBack_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x97F3, 0x9891, 0x56DE, 0x8DEF, 0x6D4B, 0x8BD5, 0}; 
    uint16 testing[10] = {0x6D4B, 0x8BD5, 0x4E2D, 0x002E, 0x002E, 0x002E, 0};
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 


    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(testing);
    cur_str_t.wstr_ptr = testing;
    cur_rect.top    = 120*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

        return;
}

void ZdtWatch_Factory_LoopBack_Exit(void)
{
    ZdtWatch_Factory_LoopBack_Stop();
    return;
}

void ZdtWatch_Factory_LoopBack_Entry(void)
{
    ZdtWatch_Factory_LoopBack_ShowData();
    ZdtWatch_Factory_LoopBack_Start();
    return;
}
#endif

#if 1
extern void ZdtWatch_Factory_Speaker_Stop(void)
{
    MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
    return;
}

extern void ZdtWatch_Factory_Speaker_Start(void)
{
    uint8  ring_id = 1;
    MMIAPISET_PlayCallRingByVol(9, ring_id,AUD_PLAY_FOREVER, MMISET_RING_TYPE_CALL, PNULL);
    return;     
}

void ZdtWatch_Factory_Speaker_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x5587, 0x53ED, 0x6D4B, 0x8BD5, 0};
    uint16 testing[10] = {0x6D4B, 0x8BD5, 0x4E2D, 0x002E, 0x002E, 0x002E, 0};
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(testing);
    cur_str_t.wstr_ptr = testing;
    cur_rect.top    = 120*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    return;
}

void ZdtWatch_Factory_Speaker_Exit(void)
{
    ZdtWatch_Factory_Speaker_Stop();
    return;
}

void ZdtWatch_Factory_Speaker_Entry(void)
{
    ZdtWatch_Factory_Speaker_ShowData();
    #if defined(W217_AGING_TEST_CUSTOM)//wuxx add.
    if (g_factory_aging_test_flag == TRUE)
    {
        #if defined(MMI_OCR_AUDIO_PLAY_TEST_LONG)// wuxx 20210805
         // 进入界面开始播放RING 语音
        MMIAPI_Play_Ocr_Audio_Test();
        #endif
    }
    else
    {
        ZdtWatch_Factory_Speaker_Start();
    }
    #else
    ZdtWatch_Factory_Speaker_Start();
    #endif
    return;
}
#endif



#if 1
typedef enum _emTestDspState
{
	/*显示的测试*/
	DISPLAY_IDLE_STATE=0,			
	DISPLAY_RED_STATE,
	DISPLAY_GREEN_STATE,
	DISPLAY_BLUE_STATE,
	DISPLAY_WHITE_STATE,
	DISPLAY_BLACK_STATE,
	DISPLAY_VBANNER_STATE,
	DISPLAY_HBANNER_STATE,
	DISPLAY_TEST_END,

}emTestDspState;

static emTestDspState m_DspStat = DISPLAY_IDLE_STATE;

void ZdtWatch_Factory_MAINL_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   rect        = {0}; 
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[60] = {0x5C4F, 0x5E55, 0x6D4B, 0x8BD5,0}; //屏幕测试5C4F 5E55 6D4B 8BD5 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;
    
    cur_rect = rect;
    cur_rect.top    = 110*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    switch(m_DspStat)
    {
    	case DISPLAY_IDLE_STATE:
		LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
		break;
        
	case DISPLAY_RED_STATE:
		LCD_FillRect(&lcd_dev_info, rect, MMI_RED_COLOR);
		break;

	case DISPLAY_GREEN_STATE:
		LCD_FillRect(&lcd_dev_info, rect, MMI_GREEN_COLOR);
		break;

	case DISPLAY_BLUE_STATE:
		LCD_FillRect(&lcd_dev_info, rect, MMI_BLUE_COLOR);
		break;

	case DISPLAY_WHITE_STATE:
            text_style.font_color = MMI_BLACK_COLOR;
		LCD_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
		break;

	case DISPLAY_BLACK_STATE:
		LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
		break;

	case DISPLAY_VBANNER_STATE:
		LCD_FillRect(&lcd_dev_info, rect, MMI_RED_COLOR);
		break;

	case DISPLAY_HBANNER_STATE:
		LCD_FillRect(&lcd_dev_info, rect, MMI_BLUE_COLOR);
		break;
	default:
		break;
	}

      cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
      cur_str_t.wstr_ptr = tittle;
  
      GUISTR_DrawTextToLCDInRect( 
          (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
          (const GUI_RECT_T      *)&cur_rect,       
          (const GUI_RECT_T      *)&cur_rect,     
          (const MMI_STRING_T    *)&cur_str_t,
          &text_style,
          state,
          GUISTR_TEXT_DIR_AUTO
          );
      MMITHEME_UpdateRect();
      return;
}


void ZdtWatch_Factory_MAINL_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_MAINL)
    {
        m_DspStat ++;
        if(m_DspStat >= DISPLAY_TEST_END)
        {
            m_DspStat = DISPLAY_RED_STATE;
            #if defined(W217_AGING_TEST_CUSTOM)//wuxx add.
            if (g_factory_aging_test_flag == TRUE)
            {
                ZdtWatch_Factory_MAINL_Entry();
            }
            else
            {
                ZdtWatch_Factory_StartNext();
            }
            #else
            ZdtWatch_Factory_StartNext();
            #endif
        }
        else
        {
            ZdtWatch_Factory_MAINL_ShowData();
            ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_MAINL_HandleTimer);
        }
    }
}

void ZdtWatch_Factory_MAINL_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_MAINL_Exit");
    m_DspStat = DISPLAY_IDLE_STATE;
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_MAINL_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_MAINL_Entry");
    m_DspStat = DISPLAY_IDLE_STATE;
    ZdtWatch_Factory_MAINL_ShowData();
    ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_MAINL_HandleTimer);
    return;
}

#if defined(W217_AGING_TEST_CUSTOM)//wuxx add.

// 老化测试LCD
void ZdtWatch_Factory_MAINLCD_AGING_TEST_Entry(void);

BOOLEAN MMIZDT_OpenWatchListFactoryWin(void);

BOOLEAN MMIZDT_OpenWatchFactoryWinIndex(uint8 index);


#if 0
void ZdtWatch_Factory_MAINLCD_AGING_TEST_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_MAINL)
    {
        m_DspStat ++; 
        if(m_DspStat >= DISPLAY_TEST_END)
        {
            m_DspStat = DISPLAY_RED_STATE;
            //ZdtWatch_Factory_StartNext();
            ZdtWatch_Factory_MAINLCD_AGING_TEST_Entry();
        }
        else
        {
            ZdtWatch_Factory_MAINL_ShowData();
            ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_MAINLCD_AGING_TEST_HandleTimer);
        }
    }
}

// 老化测试LCD
void ZdtWatch_Factory_MAINLCD_AGING_TEST_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_MAINLCD_AGING_TEST_Entry");

    ZdtWatch_BackLight(TRUE);

    
    m_DspStat = DISPLAY_IDLE_STATE;
    ZdtWatch_Factory_MAINL_ShowData();
    ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_MAINLCD_AGING_TEST_HandleTimer);
    return;
}
#endif

// 老化测试LCD
void ZdtWatch_Factory_MAINLCD_AGING_TEST_Entry(void)
{
    g_factory_aging_test_flag = TRUE;

    g_em_test_idx = EM_TEST_MAINL;

    MMIZDT_OpenWatchListFactoryWin();

    MMIZDT_OpenWatchFactoryWinIndex(g_em_test_idx);
}

//老化测试震动三秒停止两秒
void ZdtWatch_Factory_VIB_AGING_TEST_Entry(void)
{
    g_factory_aging_test_flag = TRUE;

    g_em_test_idx = EM_TEST_VIB;

    MMIZDT_OpenWatchListFactoryWin();

    MMIZDT_OpenWatchFactoryWinIndex(g_em_test_idx);
}

// 老化测试 SPEAKER
void ZdtWatch_Factory_Speaker_AGING_TEST_Entry(void)
{
    g_factory_aging_test_flag = TRUE;

    g_em_test_idx = EM_TEST_SPEAKER;

    MMIZDT_OpenWatchListFactoryWin();

    MMIZDT_OpenWatchFactoryWinIndex(g_em_test_idx);
}

#endif


#endif

#if FACTORY_TP_TEST_SUPPORT
#define EM_TP_LINE_TEST 0 // 1
#define TP_TEST_COLUMN		4
#define TP_TEST_ROW		4
uint8 g_tp_test_status[TP_TEST_ROW][TP_TEST_ROW] = {0};
uint8 g_tp_test_sum = 0;
mmi_pen_point_struct g_tp_disp_old_pt = {0};

#if EM_TP_LINE_TEST
void ZdtWatch_Factory_TP_ShowPoint(mmi_pen_point_struct pt,uint8 type)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUI_FONT_T f_big =SONG_FONT_16;

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    cur_rect = rect;
    cur_rect.top    = (UI_DEVICE_HEIGHT-WATCH_FACTORY_ITEM_HEIGHT)/2; ; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    if(type == 0)
    {
        text_style.font_color = MMI_RED_COLOR;
    }
    else if(type == 1)
    {
        text_style.font_color = MMI_BLUE_COLOR;
    }
    else if(type == 2)
    {
        text_style.font_color = MMI_GREEN_COLOR;
    }
    else
    {
        text_style.font_color = MMI_WHITE_COLOR;
    }
    LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%03d,%03d-%d", pt.x, pt.y,type);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    MMITHEME_UpdateRect();
    return;
}
static void ZdtWatch_Factory_TP_HandleMove(mmi_pen_point_struct pt,uint8 type)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   rect        = {0}; 
    LCD_DrawLine(&lcd_dev_info,g_tp_disp_old_pt.x,g_tp_disp_old_pt.y,pt.x,pt.y,MMI_BLACK_COLOR);
    g_tp_disp_old_pt = pt;
    ZdtWatch_Factory_TP_ShowPoint(pt,type);
}

static void ZdtWatch_Factory_TP_HandleDown(mmi_pen_point_struct pt)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   rect        = {0}; 
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;
    
    LCD_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
    g_tp_disp_old_pt = pt;
    ZdtWatch_Factory_TP_HandleMove(pt,0);
    return;
}

static void ZdtWatch_Factory_TP_HandleUp(mmi_pen_point_struct pt)
{
    ZdtWatch_Factory_TP_HandleMove(pt,2);
    return;
}

void ZdtWatch_Factory_TP_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x89E6, 0x6478, 0x6D4B, 0x8BD5,0}; //触摸测试89E6 6478 6D4B 8BD5 
    GUI_FONT_T f_big =SONG_FONT_16;

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);

    cur_rect = rect;
    cur_rect.top    = (UI_DEVICE_HEIGHT-WATCH_FACTORY_ITEM_HEIGHT)/2; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_BLACK_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    return;
}
#else
void ZdtWatch_Factory_TP_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_TP)
    {
        ZdtWatch_Factory_StartNext();
    }
}
static void ZdtWatch_Factory_TP_HandleMove(mmi_pen_point_struct pt,uint8 type)
{
#if 1
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   rect        = {0}; 
    int offset_y =  pt.y;
    int offset_x =  pt.x;
    uint16 m_pos_x=0;
    uint16 m_pos_y=0;
    uint16 x1;
    uint16 y1;
    if(g_em_test_idx == EM_TEST_TP)
    {
        m_pos_x = offset_x/(UI_DEVICE_WIDTH/TP_TEST_COLUMN);
        m_pos_y = offset_y/(UI_DEVICE_HEIGHT/TP_TEST_ROW);
        if(g_tp_test_status[m_pos_x][m_pos_y] == 0)
        {
            g_tp_test_sum ++;
            g_tp_test_status[m_pos_x][m_pos_y] = 1;
        }
        rect.left = m_pos_x*(UI_DEVICE_WIDTH/TP_TEST_COLUMN);
        rect.top = m_pos_y*(UI_DEVICE_HEIGHT/TP_TEST_ROW);
        rect.right = rect.left+(UI_DEVICE_WIDTH/TP_TEST_COLUMN)-1;
        rect.bottom = rect.top+(UI_DEVICE_HEIGHT/TP_TEST_ROW)-1;
        LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
        if(g_tp_test_sum >= (TP_TEST_ROW * TP_TEST_COLUMN))
        {
            ZdtWatch_Display_TimeStart(800, ZdtWatch_Factory_TP_HandleTimer);
        }
    }
#endif
}

static void ZdtWatch_Factory_TP_HandleDown(mmi_pen_point_struct pt)
{
    ZdtWatch_Factory_TP_HandleMove(pt,0);
    return;
}

static void ZdtWatch_Factory_TP_HandleUp(mmi_pen_point_struct pt)
{
    return;
}

void ZdtWatch_Factory_TP_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x89E6, 0x6478, 0x6D4B, 0x8BD5,0}; //触摸测试89E6 6478 6D4B 8BD5 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    uint16 i =0,j=0 ;

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);

    for(i=0; i<(TP_TEST_ROW-1); i++)
    {
        j = UI_DEVICE_HEIGHT/TP_TEST_ROW *(i+1);
        cur_rect.left = 0;
        LCD_DrawHLine(&lcd_dev_info,0,j,UI_DEVICE_WIDTH-1,MMI_BLACK_COLOR);
    }
    for(i=0; i<(TP_TEST_COLUMN-1); i++)
    {
        j = UI_DEVICE_WIDTH/TP_TEST_COLUMN *(i+1);
        LCD_DrawVLine(&lcd_dev_info,j,0,UI_DEVICE_HEIGHT-1,MMI_BLACK_COLOR);
    }		

    cur_rect = rect;
    cur_rect.top    = (UI_DEVICE_HEIGHT-WATCH_FACTORY_ITEM_HEIGHT)/2; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_BLACK_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    MMITHEME_UpdateRect();
    return;
}
#endif

void ZdtWatch_Factory_TP_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_TP_Exit");
    return;
}

void ZdtWatch_Factory_TP_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_TP_Entry");
    ZdtWatch_Factory_TP_ShowData();
    SCI_MEMSET(g_tp_test_status,0,sizeof(g_tp_test_status));
    g_tp_test_sum = 0;
    return;
}
#endif

//#if 1
#ifdef ZDT_VIBRATE_SUPPORT

static uint8 m_BibStat = 0;

void ZdtWatch_Factory_VIB_Req(uint8 level)
{
    if(level)
    {
        MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
    }
    else
    {
        MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
    }
    return;
}

void ZdtWatch_Factory_VIB_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x9707, 0x52A8, 0x6D4B, 0x8BD5,0}; //震动测试9707 52A8 6D4B 8BD5 
    uint16 testing[10] = {0x6D4B, 0x8BD5, 0x4E2D, 0x002E, 0x002E, 0x002E, 0};
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(testing);
    cur_str_t.wstr_ptr = testing;
    cur_rect.top    = 120*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    MMITHEME_UpdateRect();
    return;
}

void ZdtWatch_Factory_VIB_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    #if 0
    if(g_em_test_idx == EM_TEST_VIB)
    {
        if(m_BibStat)
        {
            m_BibStat = 0;
            ZdtWatch_Factory_VIB_Req(0);
        }
        else
        {
            m_BibStat = 1;
            ZdtWatch_Factory_VIB_Req(1);
        }
        ZdtWatch_Factory_VIB_ShowData();
        ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_VIB_HandleTimer);
    }
    #endif
    
    #if defined(W217_AGING_TEST_CUSTOM)//wuxx add. 震动三秒,停止两秒
    if (g_factory_aging_test_flag == TRUE)
    {
        ZdtWatch_Factory_VIB_Req(0);

        ZdtWatch_Display_TimeStart(2000, ZdtWatch_Factory_VIB_Entry);

    }
    #endif
}

void ZdtWatch_Factory_VIB_Exit(void)
{
    if(m_BibStat)
    {
        ZdtWatch_Factory_VIB_Req(0);
        m_BibStat = 0;
    }
    //ZDT_LOG("ZdtWatch_Factory_VIB_Exit");
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_VIB_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_VIB_Entry");
    m_BibStat = 1;
    ZdtWatch_Factory_VIB_Req(1);
    ZdtWatch_Factory_VIB_ShowData();
    #if defined(W217_AGING_TEST_CUSTOM)//wuxx add. 震动三秒,停止两秒
    if (g_factory_aging_test_flag == TRUE)
    {
        ZdtWatch_Display_TimeStart(3000, ZdtWatch_Factory_VIB_HandleTimer);
    }
    else
    {
        ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_VIB_HandleTimer);
    }
    #else
    ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_VIB_HandleTimer);
    #endif
    return;
}
#endif

#ifdef ZDT_GSENSOR_SUPPORT

#include "Zdt_gsensor.h"

void ZdtWatch_Factory_GSENSOR_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x8BA1, 0x6B65, 0x5668, 0x6D4B, 0x8BD5, 0}; //计步器测试8BA1 6B65 5668 6D4B 8BD5 
    uint16 hw_status_ok[10] = {0x786C, 0x4EF6, 0x6B63, 0x5E38, 0}; //硬件正常786C 4EF6 6B63 5E38 
    uint16 hw_status_err[10] = {0x786C, 0x4EF6, 0x51FA, 0x9519, 0}; //硬件出错786C 4EF6 51FA 9519 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    BOOLEAN is_hw_ok = ZDT_GSensor_HW_IsOK();
    uint32 cur_step = ZDT_GSensor_GetStep();
    BOOLEAN is_still = ZDT_GSensor_GetStill();
    ZDT_GSENSOR_IC_TYPE_E gsenser_type = ZDT_GSensor_GetType();
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    if(is_hw_ok)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_ok);
        cur_str_t.wstr_ptr = hw_status_ok;
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_err);
        cur_str_t.wstr_ptr = hw_status_err;
    }
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    if(gsenser_type == GSENSOR_TYPE_DA213)
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "DA213");
    }
    else if (gsenser_type == GSENSOR_TYPE_QMA7981)
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "QMA7981");
    }
    #if 0// wuxx del
    else if(gsenser_type == GSENSOR_TYPE_QMA7981)
    {
        #ifdef WIN32
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "6100(0x09)");
        #else
        if(qmaX981_get_chip_type_ext() == 3)
        {
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "6100(0x%x)",qmaX981_get_chip_id_ext());
        }
        else if(qmaX981_get_chip_type_ext() == 2)
        {
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "QMA7981(0x%x)",qmaX981_get_chip_id_ext());
        }
        else if(qmaX981_get_chip_type_ext() == 1)
        {
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "QMA6981(0x%x)",qmaX981_get_chip_id_ext());
        }
        else
        {
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "UnKnow(0x%x)",qmaX981_get_chip_id_ext());
        }
        #endif
    }
    #endif
    else
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "ERROR");
    }
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "Step: %d",cur_step);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "Still: %d",is_still);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    #if 0 //yangyu
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    #endif
    MMITHEME_UpdateRect();
    return;
}

void ZdtWatch_Factory_GSENSOR_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_GSENSOR)
    {
        ZdtWatch_Factory_GSENSOR_ShowData();
        ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_GSENSOR_HandleTimer);
    }
}

void ZdtWatch_Factory_GSENSOR_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_GSENSOR_Exit");
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_GSENSOR_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_GSENSOR_Entry");
    ZDT_GSensor_Open();
    ZdtWatch_Factory_GSENSOR_ShowData();
    ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_GSENSOR_HandleTimer);
    return;
}
#endif

#if defined(ZDT_HSENSOR_SUPPORT) && !defined(ZDT_HSENSOR_SUPPORT_TW)
void ZdtWatch_Factory_HSENSOR_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x5FC3, 0x7387, 0x8840, 0x538B,0x6D4B, 0x8BD5, 0}; //心率血压测试5FC3 7387 8840 538B 6D4B 8BD5 
    //uint16 tittle[10] = {0x4F53, 0x6E29, 0x6D4B, 0x8BD5 , 0}; //体温测试4F53 6E29 6D4B 8BD5 
    uint16 hw_status_ok[10] = {0x786C, 0x4EF6, 0x6B63, 0x5E38, 0}; //硬件正常786C 4EF6 6B63 5E38 
    uint16 hw_status_err[10] = {0x786C, 0x4EF6, 0x51FA, 0x9519, 0}; //硬件出错786C 4EF6 51FA 9519 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    BOOLEAN is_hw_ok = ZDT_HSensor_HW_IsOK();
    uint32 cur_hr = ZDT_HSensor_GetHR();
    float cur_spo = ZDT_HSensor_GetSPO();
    uint16 cur_spo2 = ZDT_HSensor_GetSPO2();
    uint16 cur_bp_h = ZDT_HSensor_GetBP_up();
    uint16 cur_bp_l = ZDT_HSensor_GetBP_down();
    ZDT_HSENSOR_IC_TYPE_E gsenser_type = ZDT_HSensor_GetType();
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    if(is_hw_ok)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_ok);
        cur_str_t.wstr_ptr = hw_status_ok;
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_err);
        cur_str_t.wstr_ptr = hw_status_err;
    }
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    if(gsenser_type == HSENSOR_TYPE_HRS3300)
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "HRS3300");
    }
    else if(gsenser_type == HSENSOR_TYPE_VCHR02)
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "VCHR02");
    }
    else if(gsenser_type == HSENSOR_TYPE_PAH8009)
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "PAH8009");
    }
    else
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "NULL");
    }
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "HR: %d, BP: %d-%d",cur_hr,cur_bp_l,cur_bp_h);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "SPO2: %d,%f",cur_spo2,cur_spo);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    MMITHEME_UpdateRect();
    return;
}

void ZdtWatch_Factory_HSENSOR_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_HSENSOR)
    {
        ZdtWatch_Factory_HSENSOR_ShowData();
        ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_HSENSOR_HandleTimer);
    }
}

void ZdtWatch_Factory_HSENSOR_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_HSENSOR_Exit");
    ZDT_HSensor_Close();
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_HSENSOR_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_HSENSOR_Entry");
    ZDT_HSensor_Open();
    ZdtWatch_Factory_HSENSOR_ShowData();
    ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_HSENSOR_HandleTimer);
    return;
}

void ZdtWatch_Factory_HSENSOR_Pre_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x5FC3, 0x7387, 0x8840, 0x538B,0x6D4B, 0x8BD5, 0}; //心率血压测试5FC3 7387 8840 538B 6D4B 8BD5 
    uint16 hw_status_ok[10] = {0x786C, 0x4EF6, 0x6B63, 0x5E38, 0}; //硬件正常786C 4EF6 6B63 5E38 
    uint16 hw_status_err[10] = {0x786C, 0x4EF6, 0x51FA, 0x9519, 0}; //硬件出错786C 4EF6 51FA 9519 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    BOOLEAN is_hw_ok = ZDT_HSensor_HW_IsOK();
    uint32 cur_hr_pre = ZDT_HSensor_GetHR();
    ZDT_HSENSOR_IC_TYPE_E gsenser_type = ZDT_HSensor_GetType();
    
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    if(is_hw_ok)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_ok);
        cur_str_t.wstr_ptr = hw_status_ok;
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_err);
        cur_str_t.wstr_ptr = hw_status_err;
    }
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+10*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    if(gsenser_type == HSENSOR_TYPE_HRS3300)
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "HRS3300");
    }
    else if(gsenser_type == HSENSOR_TYPE_VCHR02)
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "VCHR02");
    }
    else if(gsenser_type == HSENSOR_TYPE_PAH8009)
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "PAH8009");
    }
    else
    {
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "NULL");
    }
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    if(gsenser_type == HSENSOR_TYPE_PAH8009)
    {
        uint8 i = 0;
        for(i = 0; i < 3; i++)
        {
            SCI_MEMSET(disp_str,0,sizeof(disp_str));
            SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
            cur_rect.top    = cur_rect.bottom+10*SCALE; 
            cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
            #ifndef WIN32
                pah_sensor_get_verify_result(i,disp_str);
                cur_str_t.wstr_len = strlen(disp_str);
            #else
                cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "HR: %d",cur_hr_pre);
            #endif
            cur_str_t.wstr_ptr = disp_wstr;
            MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&cur_rect,       
                (const GUI_RECT_T      *)&cur_rect,     
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
        }
    }
    
    MMITHEME_UpdateRect();
    return;
}

void ZdtWatch_Factory_HSENSOR_Pre_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_HSENSOR_PRE)
    {
        ZdtWatch_Factory_HSENSOR_Pre_ShowData();
        ZDT_HSensor_Factory_Close();
        ZDT_HSensor_Factory_Open();
        ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_HSENSOR_Pre_HandleTimer);
    }
}

void ZdtWatch_Factory_HSENSOR_Pre_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_HSENSOR_Pre_Exit");
    ZDT_HSensor_Factory_Close();
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_HSENSOR_Pre_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_HSENSOR_Pre_Entry");
    ZDT_HSensor_Factory_Open();
    ZdtWatch_Factory_HSENSOR_Pre_ShowData();
    ZdtWatch_Display_TimeStart(1000, ZdtWatch_Factory_HSENSOR_Pre_HandleTimer);
    return;
}
#endif

#if defined(ZDT_HSENSOR_SUPPORT_TW) || defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
void ZdtWatch_Factory_TW_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x4F53, 0x6E29, 0x6D4B, 0x8BD5, 0}; //体温测试4F53 6E29 6D4B 8BD5 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    double cur_tw = 0;
    double rel_tw = 0;
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    cur_tw = MMIZDT_TW_HW_Get(&rel_tw);
    
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
   
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+10*SCALE+20; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "TW: %.1lf",cur_tw);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom+10*SCALE+10; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "Real Tw: %.2lf",rel_tw);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    MMITHEME_UpdateRect();
    return;
}

void ZdtWatch_Factory_TW_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_TW)
    {
        ZdtWatch_Factory_TW_ShowData();
        ZdtWatch_Display_TimeStart(2000, ZdtWatch_Factory_TW_HandleTimer);
    }
}

void ZdtWatch_Factory_TW_Exit(void)
{
    MMIZDT_TW_HW_Close();
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_TW_Entry(void)
{
    MMIZDT_TW_HW_Open();
    ZdtWatch_Factory_TW_ShowData();
    ZdtWatch_Display_TimeStart(2000, ZdtWatch_Factory_TW_HandleTimer);
    return;
}
#endif

#ifdef ZDT_SUPPORT_BEACON
PUBLIC void MMIIB_Scan_Over(uint8 is_ok);

void ZdtWatch_Factory_BEACON_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16 i = 0;
    uint16 j = 0;
    uint16 height = 26;//20;//yangyu
    char tmp[100] = {0};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};
    BOOLEAN is_hw_ok = beacon_hw_is_ok();
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle_beacon[10] = {0x84DD, 0x7259, 0x8DF3, 0x7EF3, 0x6D4B, 0x8BD5, 0x0020,0x0};
    uint16 hw_status_ok[10] = {0x786C, 0x4EF6, 0x6B63, 0x5E38, 0}; //硬件正常786C 4EF6 6B63 5E38 
    uint16 hw_status_err[10] = {0x786C, 0x4EF6, 0x51FA, 0x9519, 0}; //硬件出错786C 4EF6 51FA 9519 
    const uint16 GSP_ITEM_HEIGHT = 26;//12;
    char * mac_str = MMIBEACON_NVGetMAC();

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + GSP_ITEM_HEIGHT/*WATCH_FACTORY_ITEM_HEIGHT*/;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_20;
    text_style.font_color = MMI_WHITE_COLOR;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle_beacon);
    cur_str_t.wstr_ptr = tittle_beacon;
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
        
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = SONG_FONT_20;

    if(is_hw_ok)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_ok);
        cur_str_t.wstr_ptr = hw_status_ok;
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_err);
        cur_str_t.wstr_ptr = hw_status_err;
    }
    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + height;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + height;
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) mac_str);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    MMITHEME_UpdateRect();
    return;
}

static BOOLEAN ib_is_scanning = FALSE;
BOOLEAN MMIIB_TTS_PlayScanStart(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //开始蓝牙扫描
        uint16 wstr[12] = {0x5F00, 0x59CB, 0x84DD, 0x7259, 0x626B, 0x63CF, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayScanOK(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        // 扫描成功
        uint16 wstr[12] = {0x626B, 0x63CF, 0x6210, 0x529F, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

BOOLEAN MMIIB_TTS_PlayScanFail(void)
{
    #ifdef HERO_ENGINE_TTS_SUPPORT
        //扫描失败
        uint16 wstr[12] = {0x626B, 0x63CF, 0x5931, 0x8D25, 0x0020,0x0};
        MMI_STRING_T play_str = {0};
        play_str.wstr_ptr = wstr;
        play_str.wstr_len = MMIAPICOM_Wstrlen(play_str.wstr_ptr);
        TTSAPI_PlayTextForZDT(&play_str,1,1);
        return TRUE;
    #else
        return FALSE;
    #endif
}

static uint8 ib_scan_wait_timer_id = 0;
PUBLIC void MMIIB_StopScanWaitWinTimer()
{
    if(ib_scan_wait_timer_id != 0)
    {
        MMK_StopTimer(ib_scan_wait_timer_id);
        ib_scan_wait_timer_id = 0;
    }
}

void MMIIB_HandleScanWaitWinTimer(uint8 timer_id,uint32 param)
{
    MMIIB_Scan_Over(0);
    return;
}

LOCAL void MMIIB_StartScanWaitWinTimer()
{
	MMIIB_StopScanWaitWinTimer();
	ib_scan_wait_timer_id = MMK_CreateTimerCallback(5*1000, 
												   MMIIB_HandleScanWaitWinTimer, 
												    (uint32)PNULL, 
												     FALSE);
	return;
}

PUBLIC BOOLEAN MMIIB_Scan_Start()
{
    BOOLEAN res = FALSE;
    if(ib_is_scanning == FALSE)
    {
        MMIIB_StartScanWaitWinTimer();
        ZDT_IB_PowerOn();
        ZDT_IB_AT_Send_Wakeup();
        ZDT_IB_AT_Send_Scan();
        //MMIIB_TTS_PlayScanStart();
        ib_is_scanning = TRUE;
        res = TRUE;
    }
    return res;
}

PUBLIC void MMIIB_Scan_Over(uint8 is_ok)
{
    if(ib_is_scanning)
    {
        #if 0
        if(is_ok)
        {
            MMIIB_TTS_PlayScanOK();
        }
        else
        {
            BOOLEAN hw_is_ok = beacon_hw_is_ok();
            if(hw_is_ok)
            {
                MMIIB_TTS_PlayScanFail();
            }
            else
            {
                MMIIB_TTS_PlayConnectHWError();
            }
        }
        #endif
        MMIZDT_BEACON_Scan_Over();
        ib_is_scanning = FALSE;
    }
    return;
}

void ZdtWatch_Factory_BEACON_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_BEACON)
    {
        ZdtWatch_Factory_BEACON_ShowData();
        ZdtWatch_Display_TimeStart(2000, ZdtWatch_Factory_BEACON_HandleTimer);
    }
}

void ZdtWatch_Factory_BEACON_Exit(void)
{
    ZdtWatch_Display_TimeStop();
    MMIIB_StopConnect();
    return;
}

void ZdtWatch_Factory_BEACON_Start_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_BEACON)
    {
#ifdef ZDT_ADD_RING_PLAY_FACTORY_TEST
       //MMIIB_StartConnect();
       MMIIB_Scan_Start();
       ZdtWatch_Factory_BEACON_ShowData();
       ZdtWatch_NoLcd_TimeStop();
       ZdtWatch_NoLcd_TimeStart(FACTORY_RING_PLAY_TIMER, ZdtWatch_Factory_BEACON_HandleTimer);	
#endif
    }
}

void ZdtWatch_Factory_BEACON_Entry(void)
{
#ifdef ZDT_ADD_RING_PLAY_FACTORY_TEST
    MMIIB_TTS_PlayBT_Test();
    ZdtWatch_NoLcd_TimeStop();
    ZdtWatch_NoLcd_TimeStart(FACTORY_RING_PLAY_TIMER, ZdtWatch_Factory_BEACON_Start_HandleTimer);
#else
    ZdtWatch_Factory_BEACON_ShowData();
    ZdtWatch_Display_TimeStart(500, ZdtWatch_Factory_BEACON_HandleTimer);	
#endif
    return;
}
#endif

#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
uint8        g_zdt_gps_display_mode = 0;
static BOOLEAN s_zdt_gps_test_first = FALSE;
static uint32 entry_gps_ms =0;

void ZdtWatch_Factory_GPS_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint8 pause_ms =90;
    uint16 i = 0;
    uint16 j = 0;
    uint16 height = 14;//20;//yangyu
    char tmp[100] = {0};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle_gps[10] = {0x0047, 0x0050, 0x0053, 0x6D4B, 0x8BD5, 0}; //GPS测试0047 0050 0053 6D4B 8BD5 
    uint16 tittle_bd[10] = {0x5317, 0x6597,0x6D4B, 0x8BD5, 0}; //北斗测试5317 6597  6D4B 8BD5 
    uint16 tittle_gps_bd[10] = {0x0047, 0x0050, 0x0053, 0x002B, 0x5317, 0x6597, 0}; //GPS+北斗0047 0050 0053 002B 5317 6597 
    uint16 hw_status_ok[10] = {0x786C, 0x4EF6, 0x6B63, 0x5E38, 0}; //硬件正常786C 4EF6 6B63 5E38 
    uint16 hw_status_err[10] = {0x786C, 0x4EF6, 0x51FA, 0x9519, 0}; //硬件出错786C 4EF6 51FA 9519 
    GUI_FONT_T f_big =SONG_FONT_14;//FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =SONG_FONT_12;//FACTORY_NORMAL_FONT;
    BOOLEAN is_hw_ok = gps_hw_is_ok();
    BOOLEAN hw_type = gps_hwic_type_is_td();
    GPS_GSV_DATA_T gpgsv_data = {0};
    GPS_GSV_DATA_T bdgsv_data = {0};
    GPS_DATA_T gps_data = {0};
    const uint16 GSP_ITEM_HEIGHT = 12;
#ifndef WIN32
    GPS_API_GetCurGPGSV(&gpgsv_data);
    GPS_API_GetCurBDGSV(&bdgsv_data);
    GPS_API_GetCurPos(&gps_data);
#endif
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + GSP_ITEM_HEIGHT/*WATCH_FACTORY_ITEM_HEIGHT*/;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;
    if(g_zdt_gps_display_mode == 0)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle_gps_bd);
        cur_str_t.wstr_ptr = tittle_gps_bd;
    }
    else if(g_zdt_gps_display_mode == 1)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle_gps);
        cur_str_t.wstr_ptr = tittle_gps;
    }
    else if(g_zdt_gps_display_mode == 2)
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle_bd);
        cur_str_t.wstr_ptr = tittle_bd;
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle_gps_bd);
        cur_str_t.wstr_ptr = tittle_gps_bd;
    }
    if(((MMIAPICOM_GetFfsTime()-entry_gps_ms)/1000>90)&&(0 == gps_data.Latitude)&&(0 == gps_data.Longitude))
    {
        g_test_pause=TRUE;
        em_test_is_ok[g_em_test_idx] = FAIL;
        ZdtWatch_Display_TimeStop();
    }

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
        
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_mid;

    if(is_hw_ok)
    {
    #ifndef WIN32
        if(gps_hwic_type_is_td())
        {
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "GPS(%d) TD V:%d-%d, Set=%d",ZDT_GPS_Hw_IsOn(),gps_data.is_valid,gps_data.sate_num,g_gps_eph_is_set);
        }
        else if(gps_hwic_type_is_ubx())
        {
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "GPS(%d) UBX V:%d-%d, Set=%d",ZDT_GPS_Hw_IsOn(),gps_data.is_valid,gps_data.sate_num,g_gps_eph_is_set);
        }
        else if(gps_hwic_type_is_casic())
        {
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "GPS(%d) CASIC V:%d-%d, Set=%d",ZDT_GPS_Hw_IsOn(),gps_data.is_valid,gps_data.sate_num,g_gps_eph_is_set);
        }
        else if(gps_hwic_type_is_allystar())
        {
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "GPS(%d) ALLYSTAR V:%d-%d, Set=%d",ZDT_GPS_Hw_IsOn(),gps_data.is_valid,gps_data.sate_num,g_gps_eph_is_set);
        }
        else
        {
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "GPS(%d) NC V:%d-%d, Set=%d",ZDT_GPS_Hw_IsOn(),gps_data.is_valid,gps_data.sate_num,g_gps_eph_is_set);
        }
	#endif	
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
#ifdef ZDT_PLAY_RING_CUSTOM
        if(gps_data.is_valid)
        {
            if(s_zdt_gps_test_first)
            {
                MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_SUCCESS,1);
                s_zdt_gps_test_first = FALSE;
            }
        }
#endif
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_err);
        cur_str_t.wstr_ptr = hw_status_err;
#ifdef ZDT_PLAY_RING_CUSTOM
        if(s_zdt_gps_test_first)
        {
            MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_FAIL,1);
            s_zdt_gps_test_first = FALSE;
        }
#endif
    }
    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + height;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
        
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    GPS_Data_Get_StrLat(gps_data.Lat_Dir,gps_data.Latitude,tmp);
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s",tmp);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    
    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + height;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    SCI_MEMSET(tmp,0,sizeof(tmp));
    GPS_Data_Get_StrLong(gps_data.Long_Dir,gps_data.Longitude,tmp);
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s",tmp);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    
    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + height;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
	
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
     if(TRUE==g_test_pause)
     {
     	 text_style.font_color = MMI_RED_COLOR;
     	 cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "positioning time:%d s",pause_ms);
     }else
     {
     	  cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "positioning time:%d s",(MMIAPICOM_GetFfsTime()-entry_gps_ms)/1000);
     }
    
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);

    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + height+4;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 
    text_style.font_color = MMI_WHITE_COLOR;
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    SCI_MEMSET(tmp,0,sizeof(tmp));
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "H:%d S:%d C:%d",gps_data.hdop,gps_data.speed,gps_data.cog);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    
    cur_rect.top    = cur_rect.bottom; 
    cur_rect.bottom = cur_rect.top + height;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    if(is_hw_ok)
    {
        if(g_zdt_gps_display_mode == 0 || g_zdt_gps_display_mode == 1)
        {
            SCI_MEMSET(disp_str,0,sizeof(disp_str));
            SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
            sprintf((char*)disp_str, (char*) "GPGSV(%d):view=%d max=%d,min=%d",gpgsv_data.msg_sum,gpgsv_data.sates_in_view,gpgsv_data.max_snr,gpgsv_data.min_snr);
            cur_str_t.wstr_len = strlen(disp_str);
            cur_str_t.wstr_ptr = disp_wstr;
            MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
            cur_rect.top    = cur_rect.bottom; 
            cur_rect.bottom = cur_rect.top + height;
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&cur_rect,       
                (const GUI_RECT_T      *)&cur_rect,     
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            
            SCI_MEMSET(disp_str,0,sizeof(disp_str));
            SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
            j = 0;
            for(i = 0; i < GPS_NMEA_MAX_SVVIEW; i++)
            {
                if(gpgsv_data.rsv[i].is_valid)
                {
                    j++;
                    SCI_MEMSET(tmp,0,sizeof(tmp));
                    sprintf((char*)tmp, (char*) "%d=%d,",gpgsv_data.rsv[i].sate_id,gpgsv_data.rsv[i].snr);
                    strcat((char*)disp_str,(char*)tmp);
                }
                if(j == 4)
                {
                    cur_str_t.wstr_len = strlen(disp_str);
                    cur_str_t.wstr_ptr = disp_wstr;
                    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
                    cur_rect.top    = cur_rect.bottom; 
                    cur_rect.bottom = cur_rect.top + height;
                    if(cur_rect.bottom >= UI_DEVICE_HEIGHT)
                    {
                        break;
                    }
                    GUISTR_DrawTextToLCDInRect( 
                        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                        (const GUI_RECT_T      *)&cur_rect,       
                        (const GUI_RECT_T      *)&cur_rect,     
                        (const MMI_STRING_T    *)&cur_str_t,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        );
                    SCI_MEMSET(disp_str,0,sizeof(disp_str));
                    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
                    j = 0;
                }
            }
            if(j > 0)
            {
                cur_str_t.wstr_len = strlen(disp_str);
                cur_str_t.wstr_ptr = disp_wstr;
                MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
                cur_rect.top    = cur_rect.bottom; 
                cur_rect.bottom = cur_rect.top + height;
                if(cur_rect.bottom >= UI_DEVICE_HEIGHT)
                {
                    return;
                }
                GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    (const GUI_RECT_T      *)&cur_rect,       
                    (const GUI_RECT_T      *)&cur_rect,     
                    (const MMI_STRING_T    *)&cur_str_t,
                    &text_style,
                    state,
                    GUISTR_TEXT_DIR_AUTO
                    );
            }

        }
        if(g_zdt_gps_display_mode == 0 || g_zdt_gps_display_mode == 2)
        {
                SCI_MEMSET(disp_str,0,sizeof(disp_str));
                SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
                sprintf((char*)disp_str, (char*) "BDGSV(%d):view=%d max=%d,min=%d",bdgsv_data.msg_sum,bdgsv_data.sates_in_view,bdgsv_data.max_snr,bdgsv_data.min_snr);
                cur_str_t.wstr_len = strlen(disp_str);
                cur_str_t.wstr_ptr = disp_wstr;
                MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
                cur_rect.top    = cur_rect.bottom; 
                cur_rect.bottom = cur_rect.top + height;
                GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    (const GUI_RECT_T      *)&cur_rect,       
                    (const GUI_RECT_T      *)&cur_rect,     
                    (const MMI_STRING_T    *)&cur_str_t,
                    &text_style,
                    state,
                    GUISTR_TEXT_DIR_AUTO
                    );
                
                
                SCI_MEMSET(disp_str,0,sizeof(disp_str));
                SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
                j = 0;
                for(i = 0; i < GPS_NMEA_MAX_SVVIEW; i++)
                {
                    if(bdgsv_data.rsv[i].is_valid)
                    {
                        j++;
                        SCI_MEMSET(tmp,0,sizeof(tmp));
                        sprintf((char*)tmp, (char*) "%d=%d,",bdgsv_data.rsv[i].sate_id,bdgsv_data.rsv[i].snr);
                        strcat((char*)disp_str,(char*)tmp);
                    }
                    if(j == 4)
                    {
                        cur_str_t.wstr_len = strlen(disp_str);
                        cur_str_t.wstr_ptr = disp_wstr;
                        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
                        cur_rect.top    = cur_rect.bottom; 
                        cur_rect.bottom = cur_rect.top + height;
                        if(cur_rect.bottom >= UI_DEVICE_HEIGHT)
                        {
                            break;
                        }
                        GUISTR_DrawTextToLCDInRect( 
                            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                            (const GUI_RECT_T      *)&cur_rect,       
                            (const GUI_RECT_T      *)&cur_rect,     
                            (const MMI_STRING_T    *)&cur_str_t,
                            &text_style,
                            state,
                            GUISTR_TEXT_DIR_AUTO
                            );
                        SCI_MEMSET(disp_str,0,sizeof(disp_str));
                        SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
                        j = 0;
                    }
                }
                if(j > 0)
                {
                    cur_str_t.wstr_len = strlen(disp_str);
                    cur_str_t.wstr_ptr = disp_wstr;
                    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
                    cur_rect.top    = cur_rect.bottom; 
                    cur_rect.bottom = cur_rect.top + height;
                    if(cur_rect.bottom >= UI_DEVICE_HEIGHT)
                    {
                        return;
                    }
                    GUISTR_DrawTextToLCDInRect( 
                        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                        (const GUI_RECT_T      *)&cur_rect,       
                        (const GUI_RECT_T      *)&cur_rect,     
                        (const MMI_STRING_T    *)&cur_str_t,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        );
                }
            }
    }
    MMITHEME_UpdateRect();
    return;
}


void ZdtWatch_Factory_GPS_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_GPS)
    {
        ZdtWatch_Factory_GPS_ShowData();
        ZdtWatch_Display_TimeStart(500, ZdtWatch_Factory_GPS_HandleTimer);
    }
}

void ZdtWatch_Factory_GPS_Exit(void)
{
    entry_gps_ms=0;
    //ZDT_LOG("ZdtWatch_Factory_GPS_Exit");
    ZDT_GPS_Set_PowerNoOff(FALSE);
    ZdtWatch_Display_TimeStop();
    ZDT_GPS_PowerOff();

#if 0//defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
        MMIZDTGPS_PowerOff_for_w217();
#endif

    return;
}

void ZdtWatch_Factory_GPS_Entry(void)
{
    entry_gps_ms = MMIAPICOM_GetFfsTime();
    //ZDT_LOG("ZdtWatch_Factory_GPS_Entry");
    #ifndef WIN32
    g_zdt_gps_display_mode = ZDT_GPS_Get_RunMode();
   #endif
    ZDT_GPS_Set_PowerNoOff(TRUE);
    ZDT_GPS_PowerOn();
    ZdtWatch_Factory_GPS_ShowData();
#ifdef ZDT_PLAY_RING_CUSTOM
    MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_GPS,1);
#endif
    s_zdt_gps_test_first = TRUE;
    ZdtWatch_Display_TimeStart(500, ZdtWatch_Factory_GPS_HandleTimer);

#if 0//defined(ZDT_W217_FACTORY_GPS)// wuxx add 20231127
    MMIZDTGPS_PowerOn_for_w217();
#endif

    
    return;
}
#endif

#ifdef FACTORY_NFC_TEST
void ZdtWatch_Factory_NFC_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16 i = 0;
    uint16 j = 0;
    uint16 height = 26;//20;//yangyu
    char tmp[100] = {0};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle_nfc[10] = {0x004E, 0x0046, 0x0043, 0x6D4B, 0x8BD5, 0}; //NFC测试004E 0046 0043 6D4B 8BD5 
    uint16 hw_status_ok[10] = {0x786C, 0x4EF6, 0x6B63, 0x5E38, 0}; //硬件正常786C 4EF6 6B63 5E38 
    uint16 hw_status_err[10] = {0x786C, 0x4EF6, 0x51FA, 0x9519, 0}; //硬件出错786C 4EF6 51FA 9519 
    uint16 nfc_sim_ok[20] = {0x8D85, 0x7EA7, 0x0053, 0x0049, 0x004D, 0x5361, 0x6B63, 0x5E38, 0x0}; // 超级SIM卡正常8D85 7EA7 0053 0049 004D 5361 6B63 5E38 
    uint16 nfc_sim_err[20] = {0x672A, 0x8BC6, 0x522B, 0x5230, 0x8D85, 0x7EA7, 0x0053, 0x0049, 0x004D, 0x5361, 0x0}; // 示识别到超级SIM卡 672A 8BC6 522B 5230 8D85 7EA7 0053 0049 004D 5361 
    GUI_FONT_T f_big =SONG_FONT_24;//FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =SONG_FONT_20;//FACTORY_NORMAL_FONT;
    uint8 nfc_uid_str[100] = {'U','I','D',':',0};
    uint16 nfc_uid_len = 0;
    BOOLEAN is_hw_ok = ZDT_NFC_HW_IsOK();
    
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + 26;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle_nfc);
    cur_str_t.wstr_ptr = tittle_nfc;
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
        
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;

    if(is_hw_ok)
    {
        cur_str_t.wstr_ptr = hw_status_ok;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    }
    else
    {
        cur_str_t.wstr_ptr = hw_status_err;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    }

    cur_rect.top    = cur_rect.bottom + height; 
    cur_rect.bottom = cur_rect.top + height;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    nfc_uid_len = ZDT_NFC_SIM_UID_IsOK(nfc_uid_str+4,90);
    if(nfc_uid_len > 0)
    {
        SCI_MEMCPY(nfc_uid_str+4,"0",1);
        cur_str_t.wstr_ptr = nfc_sim_ok;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    }
    else
    {
        cur_str_t.wstr_ptr = nfc_sim_err;
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cur_str_t.wstr_ptr);
    }
    cur_rect.top    = cur_rect.bottom + height; 
    cur_rect.bottom = cur_rect.top + height;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    text_style.font = f_big;
    SCI_MEMSET(disp_str,0,sizeof(disp_str));
    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
    cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s",nfc_uid_str);
    cur_str_t.wstr_ptr = disp_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    
    cur_rect.top    = cur_rect.bottom + height; 
    cur_rect.bottom = cur_rect.top + height;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
        MMITHEME_UpdateRect();
    return;
}


void ZdtWatch_Factory_NFC_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_NFC)
    {
        ZdtWatch_Factory_NFC_ShowData();
        ZdtWatch_Display_TimeStart(3000, ZdtWatch_Factory_NFC_HandleTimer);
    }
    return;
}

void ZdtWatch_Factory_NFC_Exit(void)
{
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_NFC_Entry(void)
{
    ZdtWatch_Factory_NFC_ShowData();
    ZdtWatch_Display_TimeStart(3000, ZdtWatch_Factory_NFC_HandleTimer);
    return;
}
#endif

#ifdef ZDT_WL_SUPPORT
extern double g_yx_wl_Latitude;
extern double g_yx_wl_Longitude;
extern uint32 g_yx_wl_distance;

void ZdtWatch_Factory_WL_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16 i = 0;
    uint16 j = 0;
    uint16 height = 20;
    uint16 cur_wl_idx = 0;
    BOOLEAN have_lw = FALSE;
    char tmp[100] = {0};
    wchar tmp_wstr[100] = {0};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    uint16 len = 0;
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GPS_DATA_T gps_data = {0};
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle_round[10] = {0x56F4, 0x680F, 0x6D4B, 0x8BD5, 0x002D, 0x5706, 0x5F62, 0x0}; //围栏测试-圆形56F4 680F 6D4B 8BD5 002D 5706 5F62 
    uint16 tittle_rect[10] = {0x56F4, 0x680F, 0x6D4B, 0x8BD5, 0x002D, 0x77E9, 0x5F62, 0x0}; //围栏测试-矩形56F4 680F 6D4B 8BD5 002D 77E9  5F62 
    uint16 tittle_poly[10] = {0x56F4, 0x680F, 0x6D4B, 0x8BD5, 0x002D, 0x591A, 0x8FB9, 0x5F62, 0x0}; //围栏测试-多边形56F4 680F 6D4B 8BD5 002D 591A 8FB9 5F62 
    uint16 no_valid_wl[10] = {0x65E0, 0x6709, 0x6548, 0x56F4, 0x680F, 0x0 }; //无有效围栏65E0 6709 6548 56F4 680F 
    uint16 in_wl[10] = {0x56F4, 0x680F, 0x5185, 0x0 }; //围栏内56F4 680F 5185 
    uint16 out_wl[10] = {0x56F4, 0x680F, 0x5916, 0x0 }; //围栏外56F4 680F 5916 
    uint16 gps_val[10] = {0x5F53, 0x524D, 0x7ECF, 0x7EAC, 0x5EA6, 0x003A, 0x0 }; //围栏经纬度:5F53 524D 7ECF 7EAC 5EA6 003A 
    uint16 wl_val[10] = {0x56F4, 0x680F, 0x7ECF, 0x7EAC, 0x5EA6, 0x003A, 0x0 }; //当前经纬度:5F53 524D 7ECF 7EAC 5EA6 003A 
    uint16 radius_str[10] = {0x534A, 0x5F84, 0x003A,0x0 }; //半径534A 5F84 
    uint16 distance_str[10] = {0x8DDD, 0x79BB, 0x003A,0x0 }; //距离8DDD 79BB 
    GUI_FONT_T f_big =SONG_FONT_22;
    GUI_FONT_T f_mid =SONG_FONT_18;
    const uint16 GSP_ITEM_HEIGHT = 22;
    
    GPS_API_GetLastPos(&gps_data);
    
    for(i = 0; i <  YX_DB_REGIONAL_MAX_SUM; i++)
    {
        if(YX_DB_REGIONAL_IsTimerValid(i))
        {
            cur_wl_idx = i;
            have_lw = TRUE;
            break;
        }
    }
    
    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + GSP_ITEM_HEIGHT/*WATCH_FACTORY_ITEM_HEIGHT*/;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;
    if(have_lw)
    {
        if(yx_DB_Regional_Reclist[cur_wl_idx].zone_shape == 1)
        {
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle_round);
            cur_str_t.wstr_ptr = tittle_round;
        }
        else if(yx_DB_Regional_Reclist[cur_wl_idx].zone_shape == 2)
        {
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle_rect);
            cur_str_t.wstr_ptr = tittle_rect;
        }
        else if(yx_DB_Regional_Reclist[cur_wl_idx].zone_shape == 3)
        {
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle_poly);
            cur_str_t.wstr_ptr = tittle_poly;
        }
    }
    else
    {
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(no_valid_wl);
        cur_str_t.wstr_ptr = no_valid_wl;
    }

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    if(have_lw)    
    {
        text_style.align = ALIGN_HVMIDDLE;
        text_style.font = f_mid;

        //围栏ID
        cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "ID(%d): %d",i,yx_DB_Regional_Reclist[cur_wl_idx].zone_id);
        cur_str_t.wstr_ptr = disp_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        
        cur_rect.top    = cur_rect.bottom; 
        cur_rect.bottom = cur_rect.top + height;
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
        
        //围栏状态
        if(yx_DB_Regional_Reclist[cur_wl_idx].is_in)
        {
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(in_wl);
            cur_str_t.wstr_ptr = in_wl;
        }
        else
        {
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(out_wl);
            cur_str_t.wstr_ptr = out_wl;
        }

        cur_rect.top    = cur_rect.bottom; 
        cur_rect.bottom = cur_rect.top + height;
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
        
        text_style.align = ALIGN_LVMIDDLE;
        if(yx_DB_Regional_Reclist[cur_wl_idx].zone_shape == 1)
        {
            //半径
            SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
            SCI_MEMSET(tmp,0,sizeof(tmp));
            SCI_MEMSET(tmp_wstr,0,sizeof(tmp_wstr));
            len = sprintf((char*)tmp, (char*) "%d",yx_DB_Regional_Reclist[cur_wl_idx].gps_radius);
            MMI_STRNTOWSTR(tmp_wstr, 100, (uint8*)tmp, 100, len);
            MMIAPICOM_Wstrcpy(disp_wstr, radius_str);
            MMIAPICOM_Wstrcat(disp_wstr, tmp_wstr);

            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(disp_wstr);
            cur_str_t.wstr_ptr = disp_wstr;
            cur_rect.top    = cur_rect.bottom; 
            cur_rect.bottom = cur_rect.top + height;
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&cur_rect,       
                (const GUI_RECT_T      *)&cur_rect,     
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
            //距离
            SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
            SCI_MEMSET(tmp,0,sizeof(tmp));
            SCI_MEMSET(tmp_wstr,0,sizeof(tmp_wstr));
            len = sprintf((char*)tmp, (char*) "%d",g_yx_wl_distance);
            MMI_STRNTOWSTR(tmp_wstr, 100, (uint8*)tmp, 100, len);
            MMIAPICOM_Wstrcpy(disp_wstr, distance_str);
            MMIAPICOM_Wstrcat(disp_wstr, tmp_wstr);

            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(disp_wstr);
            cur_str_t.wstr_ptr = disp_wstr;
            cur_rect.top    = cur_rect.bottom; 
            cur_rect.bottom = cur_rect.top + height;
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&cur_rect,       
                (const GUI_RECT_T      *)&cur_rect,     
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
        }

        //围栏经纬度
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(wl_val);
        cur_str_t.wstr_ptr = wl_val;
        cur_rect.top    = cur_rect.bottom; 
        cur_rect.bottom = cur_rect.top + height;
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 
        
        for(j = 0; j < yx_DB_Regional_Reclist[cur_wl_idx].gps_sum; j++)
        {
            SCI_MEMSET(disp_str,0,sizeof(disp_str));
            SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
            SCI_MEMSET(tmp,0,sizeof(tmp));
            sprintf((char*)tmp, (char*) "%f  %f",yx_DB_Regional_Reclist[cur_wl_idx].regional_gps[j].Longitude,yx_DB_Regional_Reclist[cur_wl_idx].regional_gps[j].Latitude);
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s",tmp);
            cur_str_t.wstr_ptr = disp_wstr;
            MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
            
            cur_rect.top    = cur_rect.bottom; 
            cur_rect.bottom = cur_rect.top + height;
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&cur_rect,       
                (const GUI_RECT_T      *)&cur_rect,     
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
        }
        //当前经围度
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(gps_val);
        cur_str_t.wstr_ptr = gps_val;
        cur_rect.top    = cur_rect.bottom; 
        cur_rect.bottom = cur_rect.top + height;
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            ); 
            SCI_MEMSET(disp_str,0,sizeof(disp_str));
            SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
            SCI_MEMSET(tmp,0,sizeof(tmp));
            sprintf((char*)tmp, (char*) "%f  %f",g_yx_wl_Longitude,g_yx_wl_Latitude);
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s",tmp);
            cur_str_t.wstr_ptr = disp_wstr;
            MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
            
            cur_rect.top    = cur_rect.bottom; 
            cur_rect.bottom = cur_rect.top + height;
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&cur_rect,       
                (const GUI_RECT_T      *)&cur_rect,     
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
            
        //GPS经纬度
        if(gps_data.is_valid)
        {
            double gps_long = 0;
            double gps_lat = 0;
            uint32 d = 0;
            gps_long = gps_data.Longitude;
            gps_long = gps_long/180000;
            if(gps_data.Long_Dir != 'E')
            {
                gps_long = 0 - gps_long;
            }
            d = gps_data.Latitude/180000;
            gps_lat = gps_data.Lat_m;
            gps_lat = gps_lat/600000;
            gps_lat = gps_lat + d;
            if(gps_data.Lat_Dir != 'N')
            {
                gps_lat = 0 - gps_lat;
            }
            SCI_MEMSET(disp_str,0,sizeof(disp_str));
            SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
            SCI_MEMSET(tmp,0,sizeof(tmp));
            sprintf((char*)tmp, (char*) "GPS:%f  %f",gps_long,gps_lat);
            cur_str_t.wstr_len = sprintf((char*)disp_str, (char*) "%s",tmp);
            cur_str_t.wstr_ptr = disp_wstr;
            MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
            
            cur_rect.top    = cur_rect.bottom; 
            cur_rect.bottom = cur_rect.top + height;
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&cur_rect,       
                (const GUI_RECT_T      *)&cur_rect,     
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                ); 
        }
    }   
    
    MMITHEME_UpdateRect();
    return;
}


void ZdtWatch_Factory_WL_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_WL)
    {
        ZdtWatch_Factory_WL_ShowData();
        ZdtWatch_Display_TimeStart(500, ZdtWatch_Factory_WL_HandleTimer);
    }
}

void ZdtWatch_Factory_WL_Exit(void)
{
    ZdtWatch_Display_TimeStop();
    return;
}

void ZdtWatch_Factory_WL_Entry(void)
{
    ZdtWatch_Factory_WL_ShowData();
    ZdtWatch_Display_TimeStart(5000, ZdtWatch_Factory_WL_HandleTimer);
    return;
}
#endif
PUBLIC BOOLEAN ZDT_Factory_IsTesting_WL()
{
    BOOLEAN ret = FALSE;
#ifdef ZDT_WL_SUPPORT
    if(g_em_test_idx == EM_TEST_WL && MMK_IsFocusWin(MMIZDT_WATCH_FACTORY_WIN_ID))
    {
        ZdtWatch_Factory_WL_ShowData();
       ret = TRUE;
    }
#endif
    return ret;
}

#if defined(ZDT_W217_FACTORY_WIFI)// wuxx add for W217 WIFI API

//是否网络调用WIFI 接口, TRUE IS NET ,FALSE IS FACTORY TEST.
extern BOOLEAN g_is_get_wifi_data_by_net;
extern BOOLEAN g_is_get_wifi_data_from_factory;


//是否重新获取WIFI 标志
uint8 g_zdt_is_re_get_wifi_list = FALSE;

uint8 mmi_is_re_get_wifi_list_flag(void)
{
    return g_zdt_is_re_get_wifi_list;
}

#if 1// 使用定时器重新获取WIFI AP LIST
static uint8 s_re_get_wifi_list_delay_timer_id = 0;

void mmi_re_get_wifi_list_delay(void)
{
    SCI_TRACE_LOW("TIMER CHECK,mmi_re_get_wifi_list_delay.");

    if (ZDT_WIFI_Get_DataNum() == 0)
    {
        MMIZDTWIFI_Stop(); 
        
        ////SCI_Sleep(2000);  

        SCI_TRACE_LOW("mmi_re_get_wifi_list_delay, re get wifi list start.");
        g_zdt_is_re_get_wifi_list++;

        //g_zdt_is_reopen_wifi_scan = TRUE; // entry msg MSG_UAL_WIFI_CLOSE_CNF
        ZDT_WIFI_Set_Wifi_Reopen_Scan(TRUE);
        
        MMIZDTWIFI_Start();

    }

}

BOOLEAN mmi_re_get_wifi_list_delay_StartTimer(uint32 duaration)
{
    if (0 == duaration)
    {
        return FALSE;
    }
    if(0 != s_re_get_wifi_list_delay_timer_id)
    {
        MMK_StopTimer(s_re_get_wifi_list_delay_timer_id);
        s_re_get_wifi_list_delay_timer_id = 0;
    }

    // W217 WIFI RE-GET SCANN, stop LocOnceTimer.// 20231213
    MMIZDTWIFI_ClosePwOnLocOnceTimer();

    s_re_get_wifi_list_delay_timer_id = MMK_CreateTimerCallback(duaration, 
                                                                        mmi_re_get_wifi_list_delay, 
                                                                        (uint32)0, 
                                                                        FALSE);
    return TRUE;
}

BOOLEAN mmi_re_get_wifi_list_delay_StopTimer(void)
{
    if (s_re_get_wifi_list_delay_timer_id != 0)
    {
        MMK_StopTimer(s_re_get_wifi_list_delay_timer_id);
        s_re_get_wifi_list_delay_timer_id = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif



//调用 ZdtWatch_Factory_WIFI_ShowData 显示函数, 填充如下数据
// g_zdt_wifi_hw_is_ok
// ZDT_WIFI_Get_DataNum
// ZDT_WIFI_Get_Data

void ZdtWatch_Factory_WIFI_ShowData(uint8 is_scan);


void MMIZDTWIFI_Factory_ShowScanData(void)
{
    ZdtWatch_Factory_WIFI_ShowData(0);
}

void MMIZDTWIFI_Factory_WIFI_Start(void)
{
    uint8 res = 0;
    ZDT_WIFI_HW_Set_IsOK(FALSE);
    ZDT_WIFI_Set_DataNum(0);
    ZDT_WIFI_Clean_Data_Public();

    SCI_TRACE_LOW("MMIZDTWIFI_Factory_WIFI_Start ");

    // show scanning...
    if(!g_is_get_wifi_data_by_net)
    {
        ZdtWatch_Factory_WIFI_ShowData(1);
    }
    
    res = MMIZDTWIFI_Start();

    //定时, 如果没有搜索到,重新搜索一次
    g_zdt_is_re_get_wifi_list = 0;
    if(res != 2)
    {
        mmi_re_get_wifi_list_delay_StartTimer(5000);
    }
    
    return;
}


#endif

#ifdef ZDT_WIFI_SUPPORT
extern BOOLEAN        g_zdt_wifi_hw_is_ok;
uint16        g_zdt_wifi_on_times = 0;
static BOOLEAN s_zdt_wifi_test_first = FALSE;
void ZdtWatch_Factory_WIFI_Start(void);

void ZdtWatch_Factory_WIFI_ShowData(uint8 is_scan)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16 i = 0;
    uint16 j = 0;
    uint16 height = 20;
    char tmp[100] = {0};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    uint16 tmp_buf[20] = {0};
    uint16 tittle[10] = {0x0057, 0x0049, 0x0046, 0x0049, 0x6D4B, 0x8BD5, 0}; //WIFI测试0047 0050 0053 6D4B 8BD5 
    uint16 hw_status_ok[20] = {0x786C, 0x4EF6, 0x6B63, 0x5E38, 0}; //硬件正常786C 4EF6 6B63 5E38 
    uint16 hw_status_err[10] = {0x786C, 0x4EF6, 0x51FA, 0x9519, 0}; //硬件出错786C 4EF6 51FA 9519 
    uint16 txt_scaning[10] = {0x6B63, 0x5728, 0x626B, 0x63CF,0x002E,0x002E, 0x002E,0}; //正在扫描...6B63 5728 626B 63CF 002E 002E 002E 
    GUI_FONT_T f_big = SONG_FONT_17;//FACTORY_BIG_FONT;//yangyu
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    BOOLEAN is_hw_ok = g_zdt_wifi_hw_is_ok;
    uint8 w_cnt = ZDT_WIFI_Get_DataNum();
    ZDT_WIFI_DATA_T last_wifi = {0};
    if(g_em_test_idx == EM_TEST_WIFI)
    {
#ifdef ZDT_PLAY_RING_CUSTOM
        if(s_zdt_wifi_test_first)
        {
            if(is_hw_ok == FALSE)
            {
                MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_FAIL,1);
                s_zdt_wifi_test_first = FALSE;
            }
            
        }
#endif
        rect.left   = 0;
        rect.top    = 0; 
        rect.right  = UI_DEVICE_WIDTH-1;
        rect.bottom = UI_DEVICE_HEIGHT-1;

        LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

        cur_rect = rect;
        cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
        cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

        text_style.align = ALIGN_HVMIDDLE;
        text_style.font = f_big;
        text_style.font_color = MMI_WHITE_COLOR;

        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
        cur_str_t.wstr_ptr = tittle;

        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );

        if(is_scan)
        {
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(txt_scaning);
            cur_str_t.wstr_ptr = txt_scaning;
            cur_rect.top    = cur_rect.bottom; 
            cur_rect.bottom = cur_rect.top + height;
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_RECT_T      *)&cur_rect,       
                (const GUI_RECT_T      *)&cur_rect,     
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            MMITHEME_UpdateRect();
            return;
        }
        if(is_hw_ok)
        {
            sprintf(disp_str," %d",w_cnt);
            j = strlen(disp_str);
            MMI_STRNTOWSTR(disp_wstr, j, (uint8*)disp_str, j, j);
            MMIAPICOM_Wstrcat(hw_status_ok, disp_wstr);
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_ok);
            cur_str_t.wstr_ptr = hw_status_ok;
        }
        else
        {
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(hw_status_err);
            cur_str_t.wstr_ptr = hw_status_err;
        }
        cur_rect.top    = cur_rect.bottom; 
        cur_rect.bottom = cur_rect.top + height;
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&cur_rect,       
            (const GUI_RECT_T      *)&cur_rect,     
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
#ifdef ZDT_PLAY_RING_CUSTOM
            if(w_cnt > 0)
            {
                if(s_zdt_wifi_test_first)
                {
                    if(is_hw_ok)
                    {
                        MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_SUCCESS,1);
                    }
                    s_zdt_wifi_test_first = FALSE;
                }
            }
#endif

            for(i = 0; i < w_cnt; i++)
            {
                ZDT_WIFI_Get_Data(i,&last_wifi);
                SCI_MEMSET(disp_str,0,100);
                sprintf(disp_str,"%s,%02x:%02x:%02x:%02x:%02x:%02x,%d",last_wifi.ssid,last_wifi.bssid[0],last_wifi.bssid[1],last_wifi.bssid[2],last_wifi.bssid[3],last_wifi.bssid[4],last_wifi.bssid[5],last_wifi.rssi);
                {
                    cur_str_t.wstr_len = strlen(disp_str);
                    cur_str_t.wstr_ptr = disp_wstr;
                    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)disp_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
                    cur_rect.top    = cur_rect.bottom; 
                    cur_rect.bottom = cur_rect.top + height;
                    if(cur_rect.bottom >= UI_DEVICE_HEIGHT)
                    {
                        break;
                    }
                    GUISTR_DrawTextToLCDInRect( 
                        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                        (const GUI_RECT_T      *)&cur_rect,       
                        (const GUI_RECT_T      *)&cur_rect,     
                        (const MMI_STRING_T    *)&cur_str_t,
                        &text_style,
                        state,
                        GUISTR_TEXT_DIR_AUTO
                        );
                    SCI_MEMSET(disp_str,0,sizeof(disp_str));
                    SCI_MEMSET(disp_wstr,0,sizeof(disp_wstr));
                }
            }
            MMITHEME_UpdateRect();
    }
    return;
}


void ZdtWatch_Factory_WIFI_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_WIFI)
    {
        ZdtWatch_Factory_WIFI_Start();
    }
}

void ZdtWatch_Factory_WIFI_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_WIFI_Exit");
    ZdtWatch_Display_TimeStop();
    MMIZDTWIFI_API_LocStop();
    //ZDT_WIFI_PowerOff();
    
#if defined(ZDT_W217_FACTORY_WIFI)// wuxx add for W217 WIFI API
    g_is_get_wifi_data_by_net = FALSE;
    g_is_get_wifi_data_from_factory  = FALSE;
    mmi_re_get_wifi_list_delay_StopTimer();
    MMIZDTWIFI_Stop();
#endif

    return;
}

void ZdtWatch_Factory_WIFI_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_WIFI_Entry");
    g_zdt_wifi_on_times = 0;
#ifdef ZDT_PLAY_RING_CUSTOM
    MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_WIFI,1);
#endif

#if defined(ZDT_W217_FACTORY_WIFI)// wuxx add for W217 WIFI API
    g_is_get_wifi_data_by_net = FALSE;
    MMIZDTWIFI_Factory_WIFI_Start();
#else
    ZdtWatch_Factory_WIFI_Start();
#endif
    s_zdt_wifi_test_first = TRUE;
    g_is_get_wifi_data_from_factory  = TRUE;
    return;
}

int ZdtWatch_WIFI_CallBack (uint8 loc_ok,void * pLoctionData)
{
    ZdtWatch_Factory_WIFI_ShowData(0);
    ZdtWatch_Display_TimeStart(3000, ZdtWatch_Factory_WIFI_HandleTimer);
    return 0;
}

void ZdtWatch_Factory_WIFI_Start(void)
{
    ZdtWatch_Factory_WIFI_ShowData(1);
    MMIZDTWIFI_API_LocStart(10000,ZdtWatch_WIFI_CallBack);
    return;
}
#endif

#ifdef CAMERA_SUPPORT
LOCAL int8 ZDT_GetSensorID(void)
{
    SENSOR_ID_E sensor_id = 0;
    uint32 sensor_param = 0;

    DCAMERA_GetInfo(DCAMERA_INFO_CUR_IMG_SENSOR, &sensor_param);
    sensor_id = sensor_param;

    return sensor_id;
}

LOCAL int16 ZDT_GetSensorNumber(void)
{
    int16 sensor_number = 0;
    uint32 sensor_param = 0;

    DCAMERA_GetInfo(DCAMERA_INFO_IMG_SENSOR_NUM, &sensor_param);
    sensor_number = sensor_param;

    return sensor_number;
}

LOCAL DCAMERA_RETURN_VALUE_E ZDT_DC_Preview(int16 sensor_id)
{
    DCAMERA_RETURN_VALUE_E  ret = DCAMERA_OP_SUCCESS;
    DCAMERA_PREVIEW_PARAM_T preview_param = {0};/*lint !e64*/
    LCD_INFO_T lcd_info = {0};
    GUI_LCD_DEV_INFO dev_info_ptr = {0};
    GUI_RECT_T       rect = {0};    
    MMI_STRING_T     text_str = {0};

    GUISTR_STYLE_T   text_style = {0};/*lint !e64*/
    GUISTR_STATE_T   state = GUISTR_STATE_BOLD;

    GUI_COLOR_T      color = MMI_BLACK_COLOR;

    uint32 sensor_number = 0;
    GUI_RECT_T  str_rect = {0};
    wchar  sensor_wchar[] = {'S', '1', '/', '1',0};
    int16 cur_sensor_id = 0;
#ifdef ZDT_PCBA_ZW31_SUPPORT
#ifdef WIN32
    if(1)
#else
    if(Sensor_Main_Sub_Switch_Get())
#endif
    {
        sensor_wchar[0] = 'M';
    }
#endif
    // text style
    text_style.char_space = 1;
    text_style.line_space = 0;
    text_style.effect = FONT_EFFECT_NONE;
    text_style.font = SONG_FONT_16;
    text_style.font_color = MMI_GRAY_WHITE_COLOR;

    GUILCD_GetInfo(0, &lcd_info); /*lint !e64*/       

    
    sensor_number = ZDT_GetSensorNumber();
    if (2 <= sensor_number)
    {
        cur_sensor_id = sensor_id;
    }
    else
    {
       cur_sensor_id = ZDT_GetSensorID();
    }
    
    sensor_wchar[1] = sensor_wchar[1] + cur_sensor_id;
    sensor_wchar[3] = sensor_wchar[3] + sensor_number - 1;
    
    DCAMERA_StopPreview();

    //填充透明色
    rect = MMITHEME_GetFullScreenRect();
    GUI_FillRect(&dev_info_ptr, rect, color);
    MMITHEME_UpdateRect();
    
    str_rect.left = 10;
    str_rect.right = str_rect.left + 50 - 1;
    str_rect.top = 5;
    str_rect.bottom = str_rect.top + 20 - 1;
    
    text_str.wstr_ptr = sensor_wchar;
    text_str.wstr_len = MMIAPICOM_Wstrlen(sensor_wchar);

#if 0
    // display
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
        (const GUI_RECT_T      *)&str_rect,       //the fixed display area
        (const GUI_RECT_T      *)&str_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&text_str,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
#endif    
    MMITHEME_UpdateRect();

    if (2 <= sensor_number)
    {
        ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE, sensor_id, PNULL);
    }

    //added, @robert.wang, 09-9-24, cr153905
    //需要设置默认参数，否则会画面暗
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_BRIGHTNESS, DCAMERA_EV_03,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CONTRAST, DCAMERA_EV_03,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EFFECT, DCAMERA_EFFECT_NORMAL,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION, DCAMERA_EV_03,PNULL); 
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION, DCAMERA_EV_00,PNULL);
    
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_PREVIEW_MODE, DCAMERA_ENVIRONMENT_NORMAL,PNULL);
    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_WB_MODE, DCAMERA_WB_MODE_AUTO,PNULL);
    
    //end cr153905
    
    preview_param.lcd_id        = GUI_MAIN_LCD_ID;     /*lint !e64*/   
    preview_param.disp_rect.w   = lcd_info.lcd_width;
    preview_param.disp_rect.h   = lcd_info.lcd_height;
    preview_param.disp_mode = DCAMERA_DISP_FULLSCREEN;
    preview_param.rotation_mode = DCAMERA_ROTATION_0;
    
    preview_param.disp_rect.x   = 0;
    preview_param.disp_rect.y   = 0;
    preview_param.lcd_block_id  = GUI_BLOCK_0;
    preview_param.target_rect = preview_param.disp_rect;

    ret = DCAMERA_StartPreview(&preview_param);
    
    if(DCAMERA_OP_SUCCESS != ret)
    {
        GUI_RECT_T      title_rect = {0};  
        MMI_STRING_T     cur_str_t = {0};
        uint16 txt_main[10] = {0x4E3B, 0x6444, 0x50CF, 0x5934, 0x6D4B, 0x8BD5, 0}; //主摄像头测试4E3B 6444 50CF 5934 6D4B 8BD
        uint16 txt_sub[10] = {0x526F, 0x6444, 0x50CF, 0x5934, 0x6D4B, 0x8BD5, 0}; //副摄像头测试526F 6444 50CF 5934 6D4B 8BD5 
        DCAMERA_Close();
        rect = MMITHEME_GetFullScreenRect();
        GUI_FillRect(&dev_info_ptr, rect, MMI_WINDOW_BACKGROUND_COLOR);
        MMI_GetLabelTextByLang(TXT_DC_OPEN_FAIL , &text_str);
        title_rect = rect;
        title_rect.top = 10;
        title_rect.bottom = title_rect.top + 30;
        title_rect.left = 10;
#ifdef ZDT_PCBA_ZW31_SUPPORT
#ifdef WIN32
        if(1)
#else
        if(Sensor_Main_Sub_Switch_Get())
#endif
        {
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(txt_main);
            cur_str_t.wstr_ptr = txt_main;

        }
        else
        {
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(txt_sub);
            cur_str_t.wstr_ptr = txt_sub;
        }
#else
        cur_str_t.wstr_len = MMIAPICOM_Wstrlen(txt_main);
        cur_str_t.wstr_ptr = txt_main;
#endif
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&cur_str_t,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
        
        rect.top = rect.bottom / 4;
        rect.left = (rect.right - GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, SONG_FONT_16, 1)) / 2;

        // display
        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&text_str,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
        
        MMITHEME_UpdateRect();
    }  

    return ret;
}

static void ZdtWatch_Factory_Camera_Stop(void)
{
    DCAMERA_StopPreview();
    DCAMERA_Close();
    return;
}

extern void ZdtWatch_Factory_Camera_Start(void)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    DCAMERA_RETURN_VALUE_E  ret = DCAMERA_OP_SUCCESS;
    GUI_LCD_DEV_INFO dev_info_ptr = {0};
    GUI_RECT_T       rect = {0};    
    MMI_STRING_T     text_str = {0};
    GUISTR_STYLE_T   text_style = {0};/*lint !e64*/
    GUISTR_STATE_T   state = GUISTR_STATE_BOLD;
    GUI_COLOR_T      color = MMI_BLACK_COLOR;

    uint32 sensor_number = 0;
    MMIDEFAULT_TurnOnBackLight();  
    MMIDEFAULT_AllowTurnOffBackLight(FALSE);     
        
        
#ifdef WIN32
        GUIBLOCK_SetType(0, color, GUIBLOCK_TYPE_OSD, &dev_info_ptr);
#else
        GUIBLOCK_SetType(255, color, GUIBLOCK_TYPE_COMMON, &dev_info_ptr);
#endif

        MMITHEME_SetUpdateDelayCount(0);

        // text style
        text_style.char_space = 1;
        text_style.line_space = 0;
        text_style.effect = FONT_EFFECT_NONE;
        text_style.font = SONG_FONT_16;
        text_style.font_color = MMI_GRAY_WHITE_COLOR;
        ret = DCAMERA_Open(DCAMERA_OP_NORMAL);
        if(DCAMERA_OP_SUCCESS != ret)
        {
            GUI_RECT_T      title_rect = {0};  
            MMI_STRING_T     cur_str_t = {0};
            uint16 txt_main[10] = {0x4E3B, 0x6444, 0x50CF, 0x5934, 0x6D4B, 0x8BD5, 0}; //主摄像头测试4E3B 6444 50CF 5934 6D4B 8BD
            uint16 txt_sub[10] = {0x526F, 0x6444, 0x50CF, 0x5934, 0x6D4B, 0x8BD5, 0}; //副摄像头测试526F 6444 50CF 5934 6D4B 8BD5 
            rect = MMITHEME_GetFullScreenRect();
            GUI_FillRect(&dev_info_ptr, rect, MMI_WINDOW_BACKGROUND_COLOR);
            MMI_GetLabelTextByLang(TXT_DC_OPEN_FAIL , &text_str);

            title_rect = rect;
            title_rect.top = 10*SCALE;
            title_rect.bottom = title_rect.top + 30*SCALE;
            title_rect.left = 10*SCALE;
#ifdef ZDT_PCBA_ZW31_SUPPORT
#ifdef WIN32
            if(1)
#else
            if(Sensor_Main_Sub_Switch_Get())
#endif
            {
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(txt_main);
                cur_str_t.wstr_ptr = txt_main;

            }
            else
            {
                cur_str_t.wstr_len = MMIAPICOM_Wstrlen(txt_sub);
                cur_str_t.wstr_ptr = txt_sub;
            }
#else
            cur_str_t.wstr_len = MMIAPICOM_Wstrlen(txt_main);
            cur_str_t.wstr_ptr = txt_main;
#endif
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&cur_str_t,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            rect.top = rect.bottom / 4;
            rect.left = (rect.right - GUI_CalculateStringPiexlNum(text_str.wstr_ptr, text_str.wstr_len, SONG_FONT_16, 1)) / 2;
            
            // display
            GUISTR_DrawTextToLCDInRect( 
                (const GUI_LCD_DEV_INFO *)&dev_info_ptr,
                (const GUI_RECT_T      *)&rect,       //the fixed display area
                (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&text_str,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            MMITHEME_UpdateRect();
            return;
        }

        ZDT_DC_Preview(SENSOR_MAIN);
    return;     
}

void ZdtWatch_Factory_CameraMain_Exit(void)
{
    ZdtWatch_Factory_Camera_Stop();
    return;
}

void ZdtWatch_Factory_CameraMain_Entry(void)
{
#if CAMERA_SUB_SUPPORT
#ifndef WIN32
    Sensor_Main_Sub_Switch(TRUE);
#endif
#endif
    ZdtWatch_Factory_Camera_Start();
    return;
}

#if CAMERA_SUB_SUPPORT
void ZdtWatch_Factory_CameraSub_Exit(void)
{
    ZdtWatch_Factory_Camera_Stop();
    return;
}

void ZdtWatch_Factory_CameraSub_Entry(void)
{
#ifndef WIN32
    Sensor_Main_Sub_Switch(FALSE);
#endif
    ZdtWatch_Factory_Camera_Start();
    return;
}
#endif
#endif

#if 1
void ZdtWatch_Factory_RESTORE_ShowData(void)
{
#if 1
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    GUISTR_STATE_T      state2 =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK; 
    uint16 tittle[10] = {0x6062, 0x590D, 0x51FA, 0x5382, 0x9ED8, 0x8BA4,0x503C,0}; //恢复出厂默认值6062 590D 51FA 5382 9ED8 8BA4 503C 
    //uint16 content[30] = { 0x6309 , 0x8FD4 , 0x56DE , 0x952E , 0xFF0C , 0x6062 , 0x590D , 0x51FA , 0x5382 , 0x8BBE , 0x7F6E , 0x3002 , 0x6ED1 , 0x52A8 , 0x5C4F , 0x5E55 , 0xFF0C , 0x76F4 , 0x63A5 , 0x9000 , 0x51FA , 0x3002, 0 }; //按返回键，恢复出厂设置。滑动屏幕，直接退出。
    uint16 content[30] = { 0x6309 , 0x786E , 0x8bA4 , 0x952E , 0xFF0C , 0x6062 , 0x590D , 0x51FA , 0x5382 , 0x8BBE , 0x7F6E , 0x3002 ,0 }; //按返回键，恢复出厂设置。滑动屏幕，直接退出。

    uint16 ok_key[3] = {0x786E, 0x8bA4,0};  
    uint16 cancel_key[3] = {0x8fd4, 0x56de,0};

	GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    BOOLEAN is_calibration = FALSE; //zdt_app_is_calibrationed();

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 


    cur_rect.top += 40*SCALE;
    cur_rect.bottom = MMI_MAINSCREEN_HEIGHT - 1;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(content);
    cur_str_t.wstr_ptr = content;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state2,
        GUISTR_TEXT_DIR_AUTO
        ); 

    cur_rect.top = MMI_MAINSCREEN_HEIGHT - 26;
    cur_rect.bottom = MMI_MAINSCREEN_HEIGHT - 2;
    cur_rect.left= 15;
    cur_rect.right= 100;
    text_style.align = ALIGN_LVMIDDLE;

	cur_str_t.wstr_len = MMIAPICOM_Wstrlen(ok_key);
	cur_str_t.wstr_ptr = ok_key;

	GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		(const GUI_RECT_T	   *)&cur_rect, 	  
		(const GUI_RECT_T	   *)&cur_rect, 	
		(const MMI_STRING_T    *)&cur_str_t,
		&text_style,
		state2,
		GUISTR_TEXT_DIR_AUTO
		); 

    cur_rect.top = MMI_MAINSCREEN_HEIGHT - 26;
    cur_rect.bottom = MMI_MAINSCREEN_HEIGHT - 2;
    cur_rect.left= MMI_MAINSCREEN_WIDTH- 100;
    cur_rect.right= MMI_MAINSCREEN_WIDTH - 15;
    text_style.align = ALIGN_RVMIDDLE;

	cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cancel_key);
	cur_str_t.wstr_ptr = cancel_key;

	GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		(const GUI_RECT_T	   *)&cur_rect, 	  
		(const GUI_RECT_T	   *)&cur_rect, 	
		(const MMI_STRING_T    *)&cur_str_t,
		&text_style,
		state2,
		GUISTR_TEXT_DIR_AUTO
		); 

	
    
#else
    int x = 0, y = 0, str_w = 0,str_h = 0;
    GUI_COLOR_T c = gui_color(0, 0, 0);
    GUI_COLOR_T t_color = gui_color(255, 255, 255);
    uint16 tittle[10] = {0x6062, 0x590D, 0x51FA, 0x5382, 0x9ED8, 0x8BA4,0x503C,0}; //恢复出厂默认值6062 590D 51FA 5382 9ED8 8BA4 503C 
    uint16 Confirm[10] = {0x786E, 0x8BA4, 0}; //确认786E 8BA4 
    uint16 Cancle[10] = {0x9000, 0x51FA,0}; //退出 9000 51FA 
    GUI_FONT_T f_big ={0,0,0,LARGE_FONT,0,1};
    
    gui_fill_rectangle(0, 0, UI_DEVICE_WIDTH - 1, UI_DEVICE_HEIGHT - 1,c);
    gui_set_font(&f_big);
    gui_measure_string((UI_string_type)tittle, &str_w, &str_h);
    x = (LCD_WIDTH - str_w)/2;
    y = 80;
    gui_move_text_cursor(x, y);
    gui_set_text_color(t_color);
    gui_print_text((UI_string_type)tittle);

    gui_measure_string((UI_string_type)Confirm, &str_w, &str_h);
    x = 2;
    y = (LCD_HEIGHT- str_h) - 2;
    gui_move_text_cursor(x, y);
    gui_set_text_color(t_color);
    gui_print_text((UI_string_type)Confirm);

    gui_measure_string((UI_string_type)Cancle, &str_w, &str_h);
    x = (LCD_WIDTH - str_w) -2;
    y = (LCD_HEIGHT- str_h) - 2;
    gui_move_text_cursor(x, y);
    gui_set_text_color(t_color);
    gui_print_text((UI_string_type)Cancle);
    
    gdi_layer_blt_previous(0, 0, UI_DEVICE_WIDTH - 1, UI_DEVICE_HEIGHT - 1);
#endif
    MMITHEME_UpdateRect();
    return;
}

static MMI_RESULT_E HandleResetOrCleanDataWaitWinZDT(
                                                 MMI_WIN_ID_T    win_id, // 窗口的ID
                                                 MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                 DPARAM             param   // 相应消息的参数
                                                 )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    static uint32   s_waitting_num = 0;
    static BOOLEAN is_cleandata =FALSE;

    SCI_TRACE_LOW("xx_fac HandleResetOrCleanDataWaitWinZDT msg_id = 0x%x" ,msg_id);
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_waitting_num = 0;
       GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL,RIGHT_BUTTON,TRUE);
       GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, LEFT_BUTTON, TRUE);
       GUIWIN_SeSoftkeytButtonTextId(win_id, IMAGE_NULL, MIDDLE_BUTTON, TRUE);
        //recode  =   MMI_RESULT_FALSE;
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_KEYDOWN_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_APP_RED:
        break;
    case MSG_SET_RESET_NEED_WAIT_IND:
        //add one wait action
        s_waitting_num++;
        break;
    case MSG_SET_RESET_FACTORY_OVER_IND:
        if(s_waitting_num > 0)
        {
            //finished one, wait number -1
            s_waitting_num --;
        }
        //no one to wait, finished
        if(s_waitting_num == 0)
        {
            //MMK_CloseWin(win_id);
            MMIAPIPHONE_PowerReset();
        }
        break;

    case MSG_SET_CLEAN_USER_DATA_OVER_IND:
        is_cleandata = TRUE;
        break;

    case MSG_CLOSE_WINDOW:
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_ClearAllDevice();
#endif
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return recode;
}

void ZdtWatch_Factory_RESTORE_Start(MMI_WIN_ID_T win_id)
{
    MMI_WIN_ID_T                wait_win_id             =   0;

    MMIPUB_CloseQuerytWin(PNULL);
    wait_win_id = MMISET_RESET_FACTORY_WAITING_WIN_ID;
    {
        MMI_STRING_T    wait_text = {0};
        MMI_GetLabelTextByLang (STR_NOTE_WAITING, &wait_text);
        MMIPUB_OpenWaitWin (1, &wait_text, PNULL, PNULL, wait_win_id,
            COMMON_IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL,
            MMIPUB_SOFTKEY_NONE, HandleResetOrCleanDataWaitWinZDT);
    }

    MMK_PostMsg(win_id, MSG_SET_CLEAN_DATE_IND, PNULL,PNULL);
}
void ZdtWatch_Factory_RESTORE_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_RESTORE_Exit");
    return;
}

void ZdtWatch_Factory_RESTORE_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_RESTORE_Entry");
    ZdtWatch_Factory_RESTORE_ShowData();
    return;
}
#endif

#if 1
U16 g_mmi_ecg_call_number[90];
void ZdtWatch_Factory_CALL_Req(void)
{
    MMIZDT_MoCallByNum(MN_DUAL_SYS_1,"112");         
}
void ZdtWatch_Factory_CALL_ShowData(void)
{
#if 1
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    MMI_STRING_T    sim_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x901A, 0x8BDD, 0x6D4B, 0x8BD5,0}; //通话测试 901A 8BDD 6D4B 8BD5 
    uint16 cal_ok[10] = {0x5DF2, 0x6821, 0x51C6,0}; //已校准5DF2 6821 51C6 
    uint16 cal_fail[10] = {0x672A, 0x6821, 0x51C6,0}; //未校准672A 6821 51C6 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    BOOLEAN is_calibration = FALSE; //zdt_app_is_calibrationed();
    BOOLEAN sim1_card_ok = FALSE;
    uint16 no_sim_str[10] = { 0x65E0 , 0x0073 , 0x0069 , 0x006D , 0x5361 ,0};// no sim card
    uint16 have_sim_str[10] = {0x6709 , 0x0073 , 0x0069 , 0x006D , 0x5361 ,0};// have sim card

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    if(MMIAPIPHONE_GetSimExistedStatus(0))
	{
		sim1_card_ok = TRUE;
	}

	text_style.font_color = sim1_card_ok ? MMI_GREEN_COLOR : MMI_RED_COLOR;
	
	cur_rect.top += 90*SCALE;
	cur_rect.bottom += 90*SCALE;
	sim_str_t.wstr_ptr = sim1_card_ok ? have_sim_str : no_sim_str ;
	sim_str_t.wstr_len = MMIAPICOM_Wstrlen(sim_str_t.wstr_ptr);
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&sim_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    
#else
    int x = 0, y = 0, str_w = 0,str_h = 0;
    GUI_COLOR_T c = gui_color(0, 0, 0);
    GUI_COLOR_T t_color = gui_color(255, 255, 255);
    char dis_buf[100] = {0};
    char dis_str[200] = {0};
    uint16 tittle[10] = {0x901A, 0x8BDD, 0x6D4B, 0x8BD5,0}; //通话测试 901A 8BDD 6D4B 8BD5 
    uint16 Confirm[10] = {0x547C, 0x53EB, 0}; //呼叫547C 53EB 
    uint16 Cancle[10] = {0x9000, 0x51FA,0}; //退出 9000 51FA 
    GUI_FONT_T f_big ={0,0,0,LARGE_FONT,0,1};
    
    gui_fill_rectangle(0, 0, UI_DEVICE_WIDTH - 1, UI_DEVICE_HEIGHT - 1,c);
    
    gui_set_font(&f_big);
    gui_measure_string((UI_string_type)tittle, &str_w, &str_h);
    x = (LCD_WIDTH - str_w)/2;
    y = WATCH_FACTORY_TITLE_Y;
    gui_move_text_cursor(x, y);
    gui_set_text_color(t_color);
    gui_print_text((UI_string_type)tittle);

    SCI_MEMSET(dis_buf,0,sizeof(dis_buf));
    SCI_MEMSET(dis_str,0,sizeof(dis_str));
    if(srv_sim_ctrl_is_available(MMI_SIM1))
    {
        sprintf((char*) dis_buf, (char*) "10086");
    }
    else
    {
        sprintf((char*) dis_buf, (char*) "112");
    }
    mmi_asc_to_wcs((Wchar*) dis_str, dis_buf);
    gui_measure_string((UI_string_type)dis_str, &str_w, &str_h);
    x = (LCD_WIDTH - str_w)/2;
    y = 110;
    gui_move_text_cursor(x, y);
    gui_print_text((UI_string_type)dis_str);

    gui_measure_string((UI_string_type)Confirm, &str_w, &str_h);
    x = 2;
    y = (LCD_HEIGHT- str_h) - 2;
    gui_move_text_cursor(x, y);
    gui_set_text_color(t_color);
    gui_print_text((UI_string_type)Confirm);

    gui_measure_string((UI_string_type)Cancle, &str_w, &str_h);
    x = (LCD_WIDTH - str_w) -2;
    y = (LCD_HEIGHT- str_h) - 2;
    gui_move_text_cursor(x, y);
    gui_set_text_color(t_color);
    gui_print_text((UI_string_type)Cancle);
    
    gdi_layer_blt_previous(0, 0, UI_DEVICE_WIDTH - 1, UI_DEVICE_HEIGHT - 1);
#endif
    MMITHEME_UpdateRect();
	//yangyu add begin
	if(is_just_entry_call)
	{
		MMIZDT_MakeAll("112", 3, NULL);//yangyu add call 
		is_just_entry_call = FALSE;
	}

	//yangyu end

    return;
}

void ZdtWatch_Factory_CALL_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_CALL_Exit");
    return;
}

#ifdef ZDT_PLAY_RING_CUSTOM
void ZdtWatch_Factory_Call_HandleTimer(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
    if(g_em_test_idx == EM_TEST_CALL)
    {
        ZdtWatch_Factory_CALL_Req();
        ZdtWatch_Factory_StartNext();
    }
}
#endif

void ZdtWatch_Factory_CALL_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_CALL_Entry");
    ZdtWatch_Factory_CALL_ShowData();
#ifdef ZDT_PLAY_RING_CUSTOM
    MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_CALL,1);
    ZdtWatch_Display_TimeStart(2000, ZdtWatch_Factory_Call_HandleTimer);
#endif
    return;
}
#endif

#ifdef ZDT_FOTA_SUPPORT
void ZdtWatch_Factory_FOTA_ShowData(void)
{
    int x = 0, y = 0, str_w = 0,str_h = 0;
    GUI_COLOR_T c = gui_color(0, 0, 0);
    GUI_COLOR_T t_color = gui_color(255, 255, 255);
    uint16 tittle[10] = {0x7CFB, 0x7EDF, 0x5347, 0x7EA7,0}; //系统升级7CFB 7EDF 5347 7EA7 
    uint16 Confirm[10] = {0x786E, 0x8BA4, 0}; //确认786E 8BA4 
    uint16 Cancle[10] = {0x9000, 0x51FA,0}; //退出 9000 51FA 
    GUI_FONT_T f_big ={0,0,0,LARGE_FONT,0,1};
    
    gui_fill_rectangle(0, 0, UI_DEVICE_WIDTH - 1, UI_DEVICE_HEIGHT - 1,c);
    gui_set_font(&f_big);
    gui_measure_string((UI_string_type)tittle, &str_w, &str_h);
    x = (LCD_WIDTH - str_w)/2;
    y = 80;
    gui_move_text_cursor(x, y);
    gui_set_text_color(t_color);
    gui_print_text((UI_string_type)tittle);

    gui_measure_string((UI_string_type)Confirm, &str_w, &str_h);
    x = 2;
    y = (LCD_HEIGHT- str_h) - 2;
    gui_move_text_cursor(x, y);
    gui_set_text_color(t_color);
    gui_print_text((UI_string_type)Confirm);

    gui_measure_string((UI_string_type)Cancle, &str_w, &str_h);
    x = (LCD_WIDTH - str_w) -2;
    y = (LCD_HEIGHT- str_h) - 2;
    gui_move_text_cursor(x, y);
    gui_set_text_color(t_color);
    gui_print_text((UI_string_type)Cancle);
    
    gdi_layer_blt_previous(0, 0, UI_DEVICE_WIDTH - 1, UI_DEVICE_HEIGHT - 1);
    MMITHEME_UpdateRect();
    return;
}

void ZdtWatch_Factory_FOTA_Exit(void)
{
    //ZDT_LOG("ZdtWatch_Factory_RESTORE_Exit");
    return;
}

void ZdtWatch_Factory_FOTA_Entry(void)
{
    //ZDT_LOG("ZdtWatch_Factory_RESTORE_Entry");
    ZdtWatch_Factory_FOTA_ShowData();
    return;
}
#endif

#if FACTORY_TP_TEST_SUPPORT
#define WATCH_TP_OFFSET_VALUE 20
U8 Factory_pen_timer_falg=0;
static mmi_pen_point_struct factory_tp_old_point = {0,0};

void watch_factory_timer_handler(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
	Factory_pen_timer_falg=0;
      if(0 != g_facotry_tp_timer_id)
      {
          MMK_StopTimer(g_facotry_tp_timer_id);
          g_facotry_tp_timer_id = 0;
      }
}

/*****************************************************************************
 * FUNCTION
 *  watch_factory_register_input_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void watch_factory_pen_down_hdlr(mmi_pen_point_struct pt)
{
    if(g_em_test_idx == EM_TEST_TP)
    {
        ZdtWatch_Factory_TP_HandleDown(pt);
    }
    factory_tp_old_point = pt;
    Factory_pen_timer_falg=1;
    if(0 != g_facotry_tp_timer_id)
    {
        MMK_StopTimer(g_facotry_tp_timer_id);
        g_facotry_tp_timer_id = 0;
    }
    
    g_facotry_tp_timer_id = MMK_CreateTimerCallback(100, 
                                                                        watch_factory_timer_handler, 
                                                                        (uint32)PNULL, 
                                                                        FALSE);
}

static void watch_factory_pen_left(void)
{
    if(g_em_test_idx == EM_TEST_TP)
    {
    }
    else
    {
        if(!IsWatchListFactoryWinOpen())
        {
            ZdtWatch_Factory_StartNext();
        }
        else
        {
            MMK_CloseWin(MMIZDT_WATCH_FACTORY_WIN_ID);
        }
    }
}
static void watch_factory_pen_right(void)
{
    if(g_em_test_idx == EM_TEST_TP)
    {
    }
    else
    {
        if(!IsWatchListFactoryWinOpen())
        {
            ZdtWatch_Factory_StartNext();
        }
        else
        {
            MMK_CloseWin(MMIZDT_WATCH_FACTORY_WIN_ID);
        }
    }
}

static void watch_factory_pen_enter(void)
{
#ifdef ZDT_WIFI_SUPPORT
    if(g_em_test_idx == EM_TEST_WIFI)
    {
        //zdt_wifi_scan_start(1,ZdtWatch_Factory_WIFI_ScanCB);
    }
#endif
    return;
}

static void watch_factory_pen_up_hdlr(mmi_pen_point_struct pt)
{
    int offset_y =  pt.y - factory_tp_old_point.y;//up to down
    int offset_x =  pt.x - factory_tp_old_point.x;//up to down
    if(g_em_test_idx == EM_TEST_TP)
    {
        ZdtWatch_Factory_TP_HandleUp(pt);
    }

    if(g_em_test_idx == EM_TEST_RESTORE)
    {
        if((pt.x>0)&&(pt.x<110)&&(pt.y>MMI_MAINSCREEN_HEIGHT - 46)&&(pt.y<MMI_MAINSCREEN_HEIGHT))
        {
            ZdtWatch_Factory_RESTORE_Start(MMIZDT_WATCH_FACTORY_WIN_ID);
            return;
        }
        else if((pt.x>MMI_MAINSCREEN_WIDTH - 100)&&(pt.x<MMI_MAINSCREEN_WIDTH)&&(pt.y>MMI_MAINSCREEN_HEIGHT - 46)&&(pt.y<MMI_MAINSCREEN_HEIGHT))
        {
            ZdtWatch_Factory_StartNext();
            return;
        }
    }
    else if(g_em_test_idx == EM_TEST_CALL)
    {
        if((pt.x>0)&&(pt.x<100*SCALE)&&(pt.y>160*SCALE)&&(pt.y<240*SCALE))
        {
            ZdtWatch_Factory_CALL_Req();
            return;
        }
        else if((pt.x>130*SCALE)&&(pt.x<240*SCALE)&&(pt.y>160*SCALE)&&(pt.y<240*SCALE))
        {
            ZdtWatch_Factory_StartNext();
            return;
        }
    }
#ifdef ZDT_FOTA_SUPPORT
    else if(g_em_test_idx == EM_TEST_FOTA)
    {
        if((pt.x>0)&&(pt.x<100*SCALE)&&(pt.y>160*SCALE)&&(pt.y<240*SCALE))
        {
            MMK_CloseWin(MMIZDT_WATCH_FACTORY_WIN_ID);
            ZdtWatch_Setting_upgrade_entry();
            return;
        }
        else if((pt.x>130*SCALE)&&(pt.x<240*SCALE)&&(pt.y>160*SCALE)&&(pt.y<240*SCALE))
        {
            ZdtWatch_Factory_StartNext();
            return;
        }
    }
#endif
	if(offset_x <= -(WATCH_TP_OFFSET_VALUE))	
	{
            watch_factory_pen_left();
	}	
	else if(offset_x >= (WATCH_TP_OFFSET_VALUE))
	{
            watch_factory_pen_right();
	}
	else if(Factory_pen_timer_falg==0)
	{
            if((pt.x>40*SCALE)&&(pt.x<200*SCALE)&&(pt.y>40*SCALE)&&(pt.y<200*SCALE))
            {
                watch_factory_pen_enter();
            }
	}
}
#endif /*__MMI_TOUCH_SCREEN__*/

static void watch_factory_key_enter_hdlr(void)
{
    if(g_em_test_idx == EM_TEST_RESTORE)
    {
        ZdtWatch_Factory_RESTORE_Start(MMIZDT_WATCH_FACTORY_WIN_ID);
    }
#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
    else if(g_em_test_idx == EM_TEST_GPS)
    {
        g_zdt_gps_display_mode++;
        if(g_zdt_gps_display_mode > 2)
        {
            g_zdt_gps_display_mode = 0;
        }
#ifndef WIN32
        if(FALSE == ZDT_GPS_Set_RunMode(g_zdt_gps_display_mode))
        {
            ZDT_GPS_Set_PowerNoOff(FALSE);
            ZDT_GPS_PowerOff();
            SCI_Sleep(100);
            ZDT_GPS_Set_PowerNoOff(TRUE);
            ZDT_GPS_PowerOn();
        }
#endif
        ZdtWatch_Factory_GPS_ShowData();
    }
#endif
    else if(g_em_test_idx == EM_TEST_CALL)
    {
        ZdtWatch_Factory_CALL_Req();
    }
    else
    {
        if(!IsWatchListFactoryWinOpen())
        {
            ZdtWatch_Factory_StartNext();
        }
    }
}

static void watch_factory_key_power_hdlr(void)
{
    if(IsWatchListFactoryWinOpen())
    {
        MMK_CloseWin(MMIZDT_WATCH_FACTORY_WIN_ID);
    }
    else
    {
        ZdtWatch_Factory_StartNext();
    }
}

BOOLEAN ZdtWatch_Factory_StartCur(void)
{
    if(EMDlgEventHandlers[g_em_test_idx].DlgStart != NULL)
    {
        EMDlgEventHandlers[g_em_test_idx].DlgStart();
        return TRUE;
    }
    return FALSE;
}

BOOLEAN ZdtWatch_Factory_EndCur(void)
{
    if(EMDlgEventHandlers[g_em_test_idx].DlgEnd != NULL)
    {
        EMDlgEventHandlers[g_em_test_idx].DlgEnd();
        return TRUE;
    }
    return FALSE;
}

BOOLEAN ZdtWatch_Factory_StartNext(void)
{
    if(g_em_test_idx >=  EM_TEST_END)
    {
        return FALSE;
    }
    if(fastctory_test_one)
    {
        fastctory_test_one = FALSE;
        if(EMDlgEventHandlers[g_em_test_idx].DlgEnd != NULL)
        {
            EMDlgEventHandlers[g_em_test_idx].DlgEnd();
        }
        MMK_CloseWin(MMIZDT_WATCH_FACTORY_WIN_ID);
        return FALSE;
    }
    if(EMDlgEventHandlers[g_em_test_idx].DlgEnd != NULL)
    {
        EMDlgEventHandlers[g_em_test_idx].DlgEnd();
    }
    g_em_test_idx++;
    fastctory_cur_active = g_em_test_idx;
    if(g_em_test_idx >= EM_TEST_END)
    {
        MMK_CloseWin(MMIZDT_WATCH_FACTORY_WIN_ID);
        return FALSE;
    }
    if(EMDlgEventHandlers[g_em_test_idx].DlgStart != NULL)
    {
        EMDlgEventHandlers[g_em_test_idx].DlgStart();
        return TRUE;
    }
    return FALSE;
}

void ZdtWatch_Factory_Exit(void)
{
    ZDT_LOG("FactoryWatch_Factory_Exit");
    ZdtWatch_Factory_EndCur();
    fastctory_cur_active = EM_TEST_END;
    fastctory_test_one = FALSE;
    g_em_test_idx = EM_TEST_BEGIN;
    ZdtWatch_BackLight(FALSE);

#if defined(MMI_OCR_AUDIO_PLAY_TEST_LONG)// wuxx 20210805
    #if defined(MMI_OCR_AUDIO_PLAY_TEST_LONG)// wuxx 20210805
    // 退出界面停止播放RING 语音
    MMIAPISET_StopAllRing(TRUE);
    #endif

    g_factory_aging_test_flag = FALSE;
#endif
    
    return;
}

void ZdtWatch_Factory_Entry(void)
{
    ZDT_LOG("ZdtWatch_Factory_Entry");
#ifdef ZDT_GSENSOR_SUPPORT
    ZDT_GSensor_Open();
#endif
    if(g_em_test_idx > EM_TEST_BEGIN && g_em_test_idx < EM_TEST_END)
    {
        //ZDT_LOG("ZdtWatch_Factory_Entry %d",g_em_test_idx);
    }
    else
    {
        g_em_test_idx = EM_TEST_SWVER;
        //ZDT_LOG("ZdtWatch_Factory_Entry Start SWVER");
    }
    return;
}

void ZdtWatch_Factory_Entry_App(void)
{
    ZDT_LOG("ZdtWatch_Factory_Entry_App");
    ZdtWatch_BackLight(TRUE);
    fastctory_test_one = FALSE;
    fastctory_cur_active = g_em_test_idx;
    ZdtWatch_Factory_Entry();
    return;
}

void ZdtWatch_Factory_Entry_AppExt(uint8 test_idx)
{
    ZDT_LOG("ZdtWatch_Factory_Entry_AppExt test_idx=%d",test_idx);
    if(test_idx > EM_TEST_BEGIN && test_idx < EM_TEST_END)
    {
        ZdtWatch_BackLight(TRUE);
        fastctory_test_one = TRUE;
        fastctory_cur_active = test_idx;
        g_em_test_idx = test_idx;
        ZdtWatch_Factory_Entry_App();
    }
    return;
}

BOOLEAN ZdtWatch_Factory_IsInCall(void)
{
    if(fastctory_cur_active == EM_TEST_CALL)
    {
        return TRUE;
    }
    return FALSE;
}

void ZdtWatch_GetSoftVersion(char * pVer)
{
    if(pVer != NULL)
    {
        strcpy(pVer,zdt_sw_rev);
    }
}

LOCAL MMI_RESULT_E HandleZDT_WatchFactoryWinMsg (
                                      MMI_WIN_ID_T   win_id, 	// 窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM            param		// 相应消息的参数
                                      );

WINDOW_TABLE( MMIZDT_WATCH_FACTORY_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_WatchFactoryWinMsg),    
    WIN_ID( MMIZDT_WATCH_FACTORY_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenWatchFactoryWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_FACTORY_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseWatchFactoryWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_WATCH_FACTORY_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WATCH_FACTORY_WIN_ID);
    }
    return TRUE;
}

BOOLEAN MMIZDT_OpenWatchFactoryWinIndex(uint8 index)
{
    uint8 * p_idx = SCI_ALLOC_APP(sizeof(uint8));
    *p_idx = index;
    MMK_CreateWin((uint32*)MMIZDT_WATCH_FACTORY_WIN_TAB,(ADD_DATA)p_idx);
    return TRUE;
}

PUBLIC BOOLEAN ZDT_IsInFactoryWin()
{
    BOOLEAN ret = FALSE;
    //if(MMK_IsFocusWin(MMIZDT_WATCH_FACTORY_WIN_ID))
    if(MMK_IsOpenWin(MMIZDT_WATCH_FACTORY_WIN_ID) || MMK_IsOpenWin(MMIZDT_WATCH_FACTORY_LIST_WIN_ID))
    {
        ret = TRUE;
    }
    return ret;
}

PUBLIC BOOLEAN ZDT_Factory_IsTestingWifi()
{
    BOOLEAN ret = FALSE;
#ifdef ZDT_WIFI_SUPPORT
    if(g_em_test_idx == EM_TEST_WIFI)
    {
       ret = TRUE;
    }
#endif
    return ret;
   
}

/*****************************************************************************/
// 	Description : to handle the message of atcmd test 
//	Global resource dependence : 
//  Author:jianshengqi
//	Note: 2006/3/19 
/*****************************************************************************/
//extern uint32 test_main_lcd_id[20];
//extern uint32 test_sub_lcd_id[20];

LOCAL MMI_RESULT_E  HandleZDT_WatchFactoryWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    uint8 * p_index = PNULL;
    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
	SCI_TRACE_LOW("xx_fac FACTORY HandleZDT_WatchFactoryWinMsg, msg_id=0x%x",msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint32 idx = 0;
            //MMIZDT_StopShutdownTimer();//yangyu add
            if(p_index != PNULL)
            {
                idx = *p_index;
                ZdtWatch_Factory_Entry_AppExt(idx);
            }
            else
            {
                ZdtWatch_Factory_Entry_App();
            }
        }
        break;
    case MSG_SET_CLEAN_DATE_IND:
#if defined(MSA_SUPPORT)		
        if(MMIAPIMSA_IsAccountExist(MSA_APP_TYPE_CAMERA_ROLL))
        {
            MMIAPIMSA_sign_off(MSA_APP_TYPE_CAMERA_ROLL);
            MMK_CloseWin(win_id);
        }
        else
#endif
        {
#if defined( ZDT_GSENSOR_SUPPORT)
			ZDT_GSensor_SetStepOneDay(0);
			ZDT_NV_ClearSteps();
#endif
                //清除所有数据
                MMISET_CleanUserData();
                MMISET_ResetFactorySetting();
                MMK_CloseWin(win_id);
        }
        break;

        case MSG_FULL_PAINT:
                ZdtWatch_Factory_StartCur();
            break;
            
        case MSG_GET_FOCUS:
                ZdtWatch_BackLight(TRUE);
                ZdtWatch_Factory_StartCur();
            break;
            
        case MSG_LOSE_FOCUS:
                ZdtWatch_Factory_EndCur();
            break;
            
#if FACTORY_TP_TEST_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            mmi_pen_point_struct tp_point = {0,0};
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            tp_point.x = point.x;
            tp_point.y = point.y;
            ZDT_LOG("FACTORY TP MSG_TP_PRESS_DOWN, x=%d,y=%d",point.x,point.y);
            watch_factory_pen_down_hdlr(tp_point);
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            mmi_pen_point_struct tp_point = {0,0};
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            tp_point.x = point.x;
            tp_point.y = point.y;
            ZDT_LOG("FACTORY TP MSG_TP_PRESS_UP, x=%d,y=%d",point.x,point.y);
            watch_factory_pen_up_hdlr(tp_point);
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            mmi_pen_point_struct tp_point = {0,0};
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            tp_point.x = point.x;
            tp_point.y = point.y;
            ZDT_LOG("FACTORY TP MSG_TP_PRESS_MOVE, x=%d,y=%d",point.x,point.y);
            if(g_em_test_idx == EM_TEST_TP)
            {
                ZdtWatch_Factory_TP_HandleMove(tp_point,1);
            }
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            mmi_pen_point_struct tp_point = {0,0};
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            tp_point.x = point.x;
            tp_point.y = point.y;
            ZDT_LOG("FACTORY TP MSG_TP_PRESS_LONG, x=%d,y=%d",point.x,point.y);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_CANCEL:
    case MSG_KEYUP_RED:
             if(g_em_test_idx == EM_TEST_KEY)
             {
                ZDT_TestKeyboard(msg_id);
                if(ZDT_TEST_KEY_COMPLETE_FLAG == s_zdt_test_key_flag)
                {
#ifdef ZDT_PLAY_RING_CUSTOM
                    MMIAPISET_PlayCustomRing(MMISET_CUST_ENG_SUCCESS,1);
#endif
                    watch_factory_key_power_hdlr();
                }                
             }
             else
             {
    		    watch_factory_key_power_hdlr();
             }
        break;
        
    case MSG_KEYDOWN_UP:
             if(g_em_test_idx == EM_TEST_KEY)
             {
                ZDT_TestKeyboard(msg_id);
             }
        break;
        
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_OK:    
             if(g_em_test_idx == EM_TEST_KEY)
             {
                ZDT_TestKeyboard(msg_id);
             }
            else
            {
                watch_factory_key_enter_hdlr();
            }
        break;
        
    case MSG_KEYUP_UP:
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_OK:
    case MSG_KEYUP_GREEN:
        break;

    case MSG_CLOSE_WINDOW:
            ZdtWatch_Factory_Exit();
            if(p_index != PNULL)
            {
                SCI_FREE(p_index);
            }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}






LOCAL MMI_RESULT_E HandleZDT_WatchListFactoryWinMsg (
                                      MMI_WIN_ID_T   win_id, 	// 窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM            param		// 相应消息的参数
                                      );

WINDOW_TABLE( MMIZDT_WATCH_FACTORY_LIST_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_WatchListFactoryWinMsg),    
    WIN_ID( MMIZDT_WATCH_FACTORY_LIST_WIN_ID ),
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMIZDT_FACTORY_LIST_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC BOOLEAN IsWatchListFactoryWinOpen()
{
    BOOLEAN ret = FALSE;
    if(MMK_IsOpenWin(MMIZDT_WATCH_FACTORY_LIST_WIN_ID))
    {
        ret = TRUE;
    }
    return ret;
}

BOOLEAN MMIZDT_OpenWatchListFactoryWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_WATCH_FACTORY_LIST_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseWatchListFactoryWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_WATCH_FACTORY_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_WATCH_FACTORY_LIST_WIN_ID);
    }
    return TRUE;
}

BOOLEAN MMIZDT_OpenWatchListFactoryWinIndex(uint8 index)
{
    uint8 * p_idx = SCI_ALLOC_APP(sizeof(uint8));
    *p_idx = index;
    MMK_CreateWin((uint32*)MMIZDT_WATCH_FACTORY_WIN_TAB,(ADD_DATA)p_idx);
    return TRUE;
}
#if FACTORY_TP_TEST_SUPPORT
BOOLEAN MMIZDT_OpenWatchListFactoryWinTP(void)
{
    uint8 * p_idx = SCI_ALLOC_APP(sizeof(uint8));
    *p_idx = EM_TEST_TP;
    MMK_CreateWin((uint32*)MMIZDT_WATCH_FACTORY_WIN_TAB,(ADD_DATA)p_idx);
    return TRUE;
}
#endif

BOOLEAN MMIZDT_OpenWatchFactoryWin_WL(void)
{
#ifdef ZDT_WL_SUPPORT
    uint8 * p_idx = SCI_ALLOC_APP(sizeof(uint8));
    *p_idx = EM_TEST_WL;
    MMK_CreateWin((uint32*)MMIZDT_WATCH_FACTORY_WIN_TAB,(ADD_DATA)p_idx);
    return TRUE;
#endif
}

BOOLEAN MMIZDT_OpenWatchFactoryWin_SPK(void)
{
    uint8 * p_idx = SCI_ALLOC_APP(sizeof(uint8));
    *p_idx = EM_TEST_SPEAKER;
    MMK_CreateWin((uint32*)MMIZDT_WATCH_FACTORY_WIN_TAB,(ADD_DATA)p_idx);
    return TRUE;
}

LOCAL void Watch_FactoryListInit()
{

#ifdef ZDT_LCD_NOT_SUPPORT
    MMI_TEXT_ID_T text_id_list[EM_TEST_END-1] =   
    {
        TXT_TEST_SWVER,
        TXT_TEST_LOOPBACK,
#ifdef ZDT_WIFI_SUPPORT
        TXT_TEST_WIFI,
#endif
#ifdef FACTORY_KEY_TEST
        TXT_TEST_KEY,
#endif
#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
        TXT_TEST_GPS,
#endif
        TXT_TEST_CALL,
    };
#else
    MMI_TEXT_ID_T text_id_list[EM_TEST_END-1] =   
    {
        //TXT_TEST_BEGIN = 0,
        TXT_TEST_SWVER,
        TXT_TEST_CALL,
#ifdef  CAMERA_SUPPORT
        TXT_TEST_CAMERA_MAIN,
#if CAMERA_SUB_SUPPORT
        TXT_TEST_CAMERA_SUB,
#endif
#endif
        TXT_TEST_LOOPBACK,
        TXT_TEST_SPEAKER,
#if FACTORY_TP_TEST_SUPPORT
        TXT_TEST_TP,
#endif
#ifdef FACTORY_KEY_TEST
        TXT_TEST_KEY,
#endif
        TXT_TEST_MAINL,
#ifdef ZDT_WIFI_SUPPORT
        TXT_TEST_WIFI,
#endif
        TXT_TEST_SIM,
#ifdef ZDT_VIBRATE_SUPPORT
        TXT_TEST_VIB,
#endif
#ifdef ZDT_GSENSOR_SUPPORT
        TXT_TEST_GSENSOR,
#endif
#if defined(ZDT_HSENSOR_SUPPORT) && !defined(ZDT_HSENSOR_SUPPORT_TW)
        TXT_TEST_HSENSOR,
        TXT_TEST_HSENSOR_PRE,
#endif
#if defined(ZDT_HSENSOR_SUPPORT_TW) || defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
        TXT_TEST_TW,
#endif
#ifdef ZDT_SUPPORT_BEACON
        TXT_TEST_GPS,
#endif
#if defined(ZDT_GPS_SUPPORT)||defined(ZDT_W217_FACTORY_GPS)
        TXT_TEST_GPS,
#endif
#ifdef FACTORY_NFC_TEST
        TXT_TEST_NFC,
#endif
#ifdef ZDT_WL_SUPPORT
        TXT_TEST_WL,
#endif
#if defined(TORCH_SUPPORT)// TORCH_FACTORY_TEST // wuxx add
        TXT_ZDT_TORCH,
#endif

        TXT_TEST_RESTORE,
#ifdef ZDT_FOTA_SUPPORT
        TXT_TEST_FOTA,
#endif
        //TXT_TEST_END
    };
#endif


    
    GUILIST_ITEM_T              item_t    =  {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    wchar buff[GUILIST_STRING_MAX_NUM + 1] = {0};
    uint16 total_item_num = 0;
    int i = 0;
    BOOLEAN ret = FALSE;
    
    item_t.item_data_ptr = &item_data; 
    item_t.item_style =  GUIITEM_STYLE_ONE_LINE_TEXT;//GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT;//GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS;//GUIITEM_STYLE_ONE_LINE_ICON_TEXT;//GUIITEM_STYLE_ONE_LINE_TEXT; 


   
    GUILIST_RemoveAllItems(MMIZDT_FACTORY_LIST_CTRL_ID);

    for(i = 0 ;i < EM_TEST_END-1 ; i++)
    {
        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        
        item_data.item_content[0].item_data.text_id = text_id_list[i];
       
        ret = GUILIST_AppendItem (MMIZDT_FACTORY_LIST_CTRL_ID, &item_t);
        if(ret)
        {
            //CTRLLIST_SetItemUserData(MMIZDT_FACTORY_LIST_CTRL_ID, position, &i); 
        #if 1//def NEW_LEFT_RIGHT_STYLE 
            //GUILIST_SetItemStyleEx(MMIZDT_FACTORY_LIST_CTRL_ID, position, &new_style );
        #endif
            //position++;   
        }
        
    }


}


/*****************************************************************************/
// 	Description : to handle the message of atcmd test 
//	Global resource dependence : 
//  Author:jianshengqi
//	Note: 2006/3/19 
/*****************************************************************************/
//extern uint32 test_main_lcd_id[20];
//extern uint32 test_sub_lcd_id[20];

LOCAL MMI_RESULT_E  HandleZDT_WatchListFactoryWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    uint8 * p_index = PNULL;
    
    MMI_CTRL_ID_T ctrl_id = MMIZDT_FACTORY_LIST_CTRL_ID;
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    p_index = (uint8 *) MMK_GetWinAddDataPtr(win_id);
    SCI_TRACE_LOW("xx_fac HandleZDT_WatchListFactoryWinMsg msg_id = 0x%x" ,msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //GUI_FillRect((const GUI_LCD_DEV_INFO  * )&lcd_dev_info , full_rect, MMI_BLACK_COLOR);
            ZdtWatch_BackLight(TRUE);
            GUILIST_SetMaxItem(ctrl_id, 20, FALSE);
            GUILIST_SetRect(ctrl_id, &full_rect);
            CTRLLIST_SetTextFont(ctrl_id, SONG_FONT_25, MMI_WHITE_COLOR);
            Watch_FactoryListInit();
            #if 0
            uint32 idx = 0;
            if(p_index != PNULL)
            {
                idx = *p_index;
                Watch_Factory_Entry_AppExt(idx);
            }
            else
            {
                Watch_Factory_Entry_App();
            }
            #endif
        }
        break;
  

        case MSG_FULL_PAINT:
                //Watch_Factory_StartCur();
            break;
            
        case MSG_GET_FOCUS:
            ZdtWatch_BackLight(TRUE);
                ///Watch_Factory_StartCur();
            break;
            
        case MSG_LOSE_FOCUS:
                //Watch_Factory_EndCur();
            break;

        case MSG_CTL_PENOK:
        case MSG_CTL_MIDSK:
        {
            uint16 index = GUILIST_GetCurItemIndex(ctrl_id);
            if(index+1 == EM_TEST_CALL)
            {
                is_just_entry_call = TRUE;
            }
            MMIZDT_OpenWatchFactoryWinIndex(index+1);
        }
        break;

    case MSG_KEYDOWN_RED:
        break;
        
    case MSG_KEYUP_RED:
			//watch_factory_key_power_hdlr();                    
        break;
        
    case MSG_KEYDOWN_CANCEL:
        break;
    case MSG_CTL_CANCEL:
    case MSG_KEYUP_CANCEL:
            //watch_factory_key_enter_hdlr(); 
            MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
            //Watch_Factory_Exit();
            ZdtWatch_BackLight(FALSE);
            if(p_index != PNULL)
            {
                SCI_FREE(p_index);
            }
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}


#if defined(ZDT_SUPPORT_TW_GD609) || defined(ZDT_SUPPORT_TW_FM78102)
LOCAL MMI_RESULT_E HandleZDT_TwCaliWinMsg (
                                      MMI_WIN_ID_T   win_id, 	// 窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM            param		// 相应消息的参数
                                      );

WINDOW_TABLE( MMIZDT_TW_CALI_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleZDT_TwCaliWinMsg),    
    WIN_ID( MMIZDT_TW_CALI_WIN_ID ),
    WIN_HIDE_STATUS,
    END_WIN
};

BOOLEAN MMIZDT_OpenTwCaliWin(void)
{
    MMK_CreateWin((uint32*)MMIZDT_TW_CALI_WIN_TAB,PNULL);
    return TRUE;
}

BOOLEAN MMIZDT_CloseTwCaliWin(void)
{
    if(MMK_IsOpenWin(MMIZDT_TW_CALI_WIN_ID))
    {
        MMK_CloseWin(MMIZDT_TW_CALI_WIN_ID);
    }
    return TRUE;
}

int fm78102_objectDelta = 0;

LOCAL uint8  *s_temp_nv_org_ptr = NULL;
LOCAL BOOLEAN temp_IMEI_FixedNvAccess(uint16 ItemID,uint8 *data,uint32 length)
{
    BOOLEAN		result = FALSE;
#ifndef WIN32
    s_temp_nv_org_ptr = (uint8*)NVITEM_MallocCaliBuf();
    if(NVITEM_UpdateCaliBuf(ItemID, MN_MAX_IMEI_LENGTH, data, s_temp_nv_org_ptr)== NVERR_NONE)
    {
    	result = NVITEM_UpdateNvParam(s_temp_nv_org_ptr);
    }

    if(s_temp_nv_org_ptr)
    {
        SCI_Free(s_temp_nv_org_ptr);
        s_temp_nv_org_ptr= NULL;
    }
#endif	
    return TRUE;
}
LOCAL BOOLEAN temp_IMEI_ReadFixedNv(uint16 ItemID,uint8 *data)
{
	uint32		ret=0;
#ifndef WIN32	
    s_temp_nv_org_ptr = (uint8*)NVITEM_MallocCaliBuf();

    if(NVITEM_ReadCaliBuf(ItemID, MN_MAX_IMEI_LENGTH, data, s_temp_nv_org_ptr)==NVERR_NONE)
    {
        ret = MN_MAX_IMEI_LENGTH;
    }
    if(s_temp_nv_org_ptr)
    {
        SCI_Free(s_temp_nv_org_ptr);
        s_temp_nv_org_ptr = NULL;
    }
#endif

}
void ZdtWatch_TW_Cali_ShowData(void)
{

    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[5] = {0x6E29 ,0x611F ,0x6821 ,0x51C6 ,0}; //温感校准  6E29 611F 6821 51C6
    uint16 cali_tip1[11] = {0x6309 ,0x4E0A ,0x952E ,0x6216 ,0x4E0B ,0x952E ,0x5F00 ,0x59CB ,0x6821 ,0x51C6 ,0}; //按上键或下键开始校准 6309 4E0A 952E 6216 4E0B 952E 5F00 59CB 6821 51C6 
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cali_tip1);
    cur_str_t.wstr_ptr = cali_tip1;
    cur_rect.top    = 100; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    return;
}

void ZdtWatch_TW_Cali_Show_result(void)
{

    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    uint16 disp_str_len =0;
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 cali_result[16]={0};
    uint16 cali_result_len = 0;
    uint16 cali_ok[6] = {0x6821 ,0x51C6 ,0x6210 ,0x529F ,0x3A,0}; //校准成功 6821 51C6 6210 529F 
    uint16 cali_fail[6] = {0x6821 ,0x51C6 ,0x5931 ,0x8D25 ,0x3A,0}; //校准失败 6821 51C6 5931 8D25
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;
    uint8 delta_str[10] = {0};

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    if((fm78102_objectDelta>0) && (fm78102_objectDelta<=1000))
    {
        SCI_MEMCPY(cali_result,cali_ok,10);
        sprintf((char*)delta_str, (char*) "%05d",fm78102_objectDelta);
        temp_IMEI_FixedNvAccess(NV_IMEI3, delta_str, MN_MAX_IMEI_LENGTH);
    }
    else
    {
        SCI_MEMCPY(cali_result,cali_fail,10);
    }
    cali_result_len =5;
    sprintf((char*)disp_str, (char*) "%d",fm78102_objectDelta);
    disp_str_len = SCI_STRLEN((char*)disp_str);
    MMIAPICOM_StrcatFromStrToUCS2(cali_result, &cali_result_len, (uint8*)disp_str, disp_str_len);
    
    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(cali_result);
    cur_str_t.wstr_ptr = cali_result;
    cur_rect.top    = 150; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    LCD_FillRect(&lcd_dev_info, cur_rect, MMI_BLACK_COLOR);
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    return;
}


PUBLIC int MMIZDT_Get_temp78102_delta(void)
{
    char delta_buf[20] = {0};
    temp_IMEI_ReadFixedNv(NV_IMEI3,delta_buf);
    SCI_TRACE_LOW("MMIZDT_Get_temp78102_delta delta_buf=%s",delta_buf); 
    if(delta_buf[0])
    {
        fm78102_objectDelta= MMIAPICOM_StrToInt(delta_buf,5);
        return fm78102_objectDelta;
    }
    else
    {
        fm78102_objectDelta =0;
        return 0;
    }
}

LOCAL MMI_RESULT_E  HandleZDT_TwCaliWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    uint8 * p_index = PNULL;
    
    MMI_CTRL_ID_T ctrl_id = MMIZDT_FACTORY_LIST_CTRL_ID;
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
   
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
              ZdtWatch_TW_Cali_ShowData();
#ifndef WIN32              
              Sensor_temperature_cali_start();              
#endif
        }
        break;
  
    case MSG_FULL_PAINT:
        break;

    case MSG_APP_UP:
    case MSG_APP_DOWN:
#ifndef WIN32        
        fm78102_objectDelta = fm78102_ObjectCalibrationDelta();
#endif        
        SCI_TRACE_LOW("fm78102 HandleZDT_TwCaliWinMsg fm78102_objectDelta=%d",fm78102_objectDelta);            
        ZdtWatch_TW_Cali_Show_result();
        break;
        
    case MSG_KEYDOWN_CANCEL:
        break;
    case MSG_CTL_CANCEL:
    case MSG_KEYUP_CANCEL:
            MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
#ifndef WIN32        
        Sensor_temperature_cali_stop();
#endif
        break;      

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif



#if defined(TORCH_SUPPORT)// TORCH_FACTORY_TEST // wuxx add

void ZdtWatch_Factory_Torch_ShowData(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    char disp_str[100] = {0};
    wchar disp_wstr[100] = {0};
    MMI_STRING_T    cur_str_t   = {0};
    GUI_RECT_T   cur_rect          = {0};
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    uint16 tittle[10] = {0x624B, 0x7535, 0x7B52, 0};// 手电筒
    //uint16 testing[10] = {0x6D4B, 0x8BD5, 0x4E2D, 0x002E, 0x002E, 0x002E, 0};
    uint16 testing[10] = {0x8BF7, 0X67E5, 0X770B, 0x624B, 0x7535, 0x7B52, 0}; // 请查看手电筒
    GUI_FONT_T f_big =FACTORY_BIG_FONT;
    GUI_FONT_T f_mid =FACTORY_NORMAL_FONT;

    rect.left   = 0;
    rect.top    = 0; 
    rect.right  = UI_DEVICE_WIDTH-1;
    rect.bottom = UI_DEVICE_HEIGHT-1;

    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);

    cur_rect = rect;
    cur_rect.top    = WATCH_FACTORY_TITLE_Y; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = f_big;
    text_style.font_color = MMI_WHITE_COLOR;

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(tittle);
    cur_str_t.wstr_ptr = tittle;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    cur_str_t.wstr_len = MMIAPICOM_Wstrlen(testing);
    cur_str_t.wstr_ptr = testing;
    cur_rect.top    = 120*SCALE; 
    cur_rect.bottom = cur_rect.top + WATCH_FACTORY_ITEM_HEIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_rect,       
        (const GUI_RECT_T      *)&cur_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    return;
}


void ZdtWatch_Factory_TORCH_Exit(void)
{
#ifndef WIN32
    GPIO_SetTorch(FALSE);
#endif
    return;
}

void ZdtWatch_Factory_TORCH_Entry(void)
{
    
#ifndef WIN32
    GPIO_SetTorch(TRUE);
#endif
    ZdtWatch_Factory_Torch_ShowData();
    return;
}

#endif

