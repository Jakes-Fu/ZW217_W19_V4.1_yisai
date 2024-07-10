/*****************************************************************************
** File Name:      mmieng_main.c                                                  *
** Author:                                                                   *
** Date:           08/2004                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2004         Allen

******************************************************************************/

#define _MMIENG_MAIN_C_

/**--------------------------------------------------------------------------*
		                        Include Files      
 **--------------------------------------------------------------------------*/
#ifdef AUTODEMO_SUPPORT
#include "mmiautodemo_export.h"
#endif
#include "mmi_app_eng_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "usbservice_api.h"

#include "mmk_app.h"
#include "mmieng_internal.h"
#include "mmieng_win.h"
#include "mmieng_uitestwin.h"

#ifdef TP_TEST_SUPPORT
#include "mmk_tp.h"
#include "tp_srv.h"
#endif

#include "mmi_default.h"
#include "mmi_menutable.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guitext.h"
#include "guictrl_api.h"
//#include "guilabel.h"
#include "ref_param.h"

#ifndef _WIN32
#include "deep_sleep.h"
#include "sio.h"
//#include "Layer1_engineering.h"
#include "sci_log.h"
#include "xsr_partition.h"
#include "ubot_config.h"
#endif
#include "mmiset_func.h"
#include "mmiset_export.h"
#include "mmicc_export.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#ifndef WIN32
#include "ref_engineering.h"
#endif
#include "tb_hal.h"
#include "mmieng_nv.h"
#include "mmieng_id.h"
#include "mmieng_menutable.h"
#include "mmieng_main.h"
#include "mmi_nv.h"
#include "mmisd_export.h"
#include "mmipub.h"
#include "sig_code.h"
//#include "mmisms_text.h"
#ifdef RECORD_SUPPORT
#include "mmisrvrecord_export.h"
#endif
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmimp3_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
//#include "Gpio_prod_api.h"
#include "mmisd_internal.h"
#ifdef ENGTD_SUPPORT
#include "mmiengtd_export.h"
#endif
#include "mmienvset_export.h"
#include "mmieng_text.h"
//#include "mmimtv_text.h"
#ifndef WIN32
#include "ubot_config.h"
#endif
#include "mmistk_export.h"
#include "mmiidle_subwintab.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_export.h"
#endif
#ifdef DM_SUPPORT
#include "mmidm_export.h"
#endif

#ifdef MMI_ENABLE_DCD
#include "mmidcd_export.h"
#endif

#ifdef CMMB_SUPPORT
#include "mmimtv_export.h"
#include "mmimbbms_main.h"
#include "mmimtv_main.h"
#include "mmimtv_nv.h"
#include "mmimtv_text.h"
#include "mmimtv_image.h"
#include "mtv_api.h"
#include "lcd_tv.h"
#include "dal_lcd.h"
#include "mbbms_service.h"
#endif
#include "in_message.h"
#include "app_tcp_if.h"
#ifdef PIM_SUPPORT
#include "mmipim_export.h"
#endif

#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#include "java_mmi_text.h"
#endif

#ifdef QQ_SUPPORT
#include "qq2009.h"
#include "qq_text.h"
#endif

#ifdef CARD_LOG
#ifndef WIN32
#include "logsave_output.h"
#endif
#endif

#include "mmieng_uitestwin.h"
#include "tcpip_api.h"
#include "os_param.h"
//#include "run_mode.h"
#include "sci_log.h"
#include "mn_api.h"
#include "mmidc_export.h"
#include "mmidc_setting.h"
//MS00216980 cheney
#include "mmi_custom_define.h"
#include "mmiudisk_export.h"

#ifdef ESIM_SUPPORT_HB
#include "mmiesim_hb_export.h"
#endif

#ifdef PRINTSCREEN_SUPPORT
#include "mmiptscr_export.h"
#endif
#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif
#include "mmipb_export.h"
#include "mmiset_id.h"
//#include "mmisrvaud_drv.h"

//michael wang
#include "mmiconnection_export.h"
#ifndef WIN32
#ifdef NANDBOOT_SUPPORT
#include "sci_ftl_api.h"
#endif
#endif
#if defined (PLATFORM_ANTISW3)
#ifndef WIN32
#include "ulog.h"
#endif
#endif
#include "watch_commonwin_export.h"
#if ((defined GPS_SUPPORT)&&(defined MMI_WIFI_SUPPORT))
#include "mmieng_gps_ota.h"
#endif
#include "mmieng_memtester.h"
#include "mmicom_trace.h"
#include "mmi_appmsg.h"
#ifndef WIN32
#ifdef GOODIX_SUPPORT_GR5515
#include "production_line.h"
#include "uart_data_process.h"
#endif
#endif

#ifdef AUDIOPLAYER_DEMOAPP_SUPPORT
#include "audioplayer_demoapp_main_win.h"
#endif

#ifdef MMI_RF_AM_SUPPORT
#include "mmieng_rf_am_test_win.h"
#endif
#include "mmi_mainmenu_item_func.h"
#include "watch_set_brightness.h"

#include "mmieng_export.h"
#include "ual_tele_radio.h"
#if defined ENG_BAND_FREQ_SUPPORT
#include "mmi_appmsg.h"
#endif
#include "mmialarm_export.h"

/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/
//#define SCREEN_LOG_FONT  16
#define SCREEN_LOG_FONT  9
#define SCREEN_STR_LEN   50

#ifndef DSP_DATA_FRAME_LENGTH
#define DSP_DATA_FRAME_LENGTH  350//328
#endif
#ifndef DSP_DATA_FRAME_LENGTH_VOICE_DATA
#define DSP_DATA_FRAME_LENGTH_VOICE_DATA  480
#endif
#ifndef DSP_DATA_BLOCK_COUNT_2
#define DSP_DATA_BLOCK_COUNT_2  2
#endif
#ifndef DSP_DATA_BLOCK_COUNT_3
#define DSP_DATA_BLOCK_COUNT_3  3
#endif
#ifdef ENG_SUPPORT
#ifdef  TP_TEST_SUPPORT
#define TP_TEST_EDIDBOX_PARA_COUNT  4
#define TP_TEST_PARA_PRINT_COUNT  40
#define TP_TEST_GET_POINT_TIMER  50
#define TP_TEST_CIRCLE_RADIUS   7
#define TP_TEST_BUTTON_GET_RC   {0,0,10,60} 
#define TP_TEST_BUTTON_SET_RC   {0,70,10,130}
#define TP_TEST_BUTTON_DRAW_RC   {0,140,10,200}
#define TP_TEST_EDITBOX_RC   {0,0,10,20}
#define TP_TEST_EDITBOX_HEIGHT   20
#define TP_TEST_PARA_MSG   0x200
#define TP_TEST_BUTTON_WIDTH  60
#ifdef MAINLCD_SIZE_240X320
#define TP_TEST_BUTTON_HEIGHT  20
#else
#define TP_TEST_BUTTON_HEIGHT  50
#endif
#define TP_TEST_EDITBOX_WIDTH  70
#define TP_TEST_MORE_PARA L"para:"
#define WIN32_TP_TEST_PARA_NUM 12 
#define TP_TEST_SEVEN_WORD   7
#define TP_TEST_GET_WORD_MAX 30
#endif
#endif
#define MMIENG_ENTRY_TICK        (5)
#define MMIENG_ENTRY_TIME_OUT    (3000)
/**--------------------------------------------------------------------------*
			                         STATIC DEFINITION
 **--------------------------------------------------------------------------*/
typedef struct
{
    const char *                                        num_str_ptr;
    MMIENG_IDLE_DIAL_NUM_TYPE_E opt_type;
    parsCtrlCodeE                             ctrl_code;
    BOOLEAN				(*processFun)(void);
} MMIENG_IDLE_DIAL_NUM_APP_T;

typedef struct
{
     const char *             num_str_ptr;
     MMISET_LANGUAGE_TYPE_E   language_type;
}MMIENG_IDLE_SET_LANGUAGETYPE_T;
/**--------------------------------------------------------------------------*
			                         TYPE AND CONSTANT
 **--------------------------------------------------------------------------*/
// extern  BOOLEAN L1API_ApplyMaxPower();
extern BOOLEAN g_mmieng_showversion_for1;

#ifdef BROWSER_SUPPORT_DORADO
extern BOOLEAN g_is_http_log_file_save; 
extern BOOLEAN g_is_ssl_log_save; 
#endif


#ifdef ZDT_ZFB_SUPPORT
extern BOOLEAN ZFB_DB_INIT_OK_Reset_1(void);
extern BOOLEAN ZFB_DB_INIT_OK_Reset_2(void);
extern BOOLEAN ZFB_DB_INIT_OK_Reset_All(void);
#endif

#if defined(ZDT_SHIPPING_MODE_CMD)// 船运模式
void ZDT_open_shipping_mode(void)
{
     //MMIPUB_OpenAlertWaitingWin(TXT_CHARGE_FINISH);// 弹出2 秒消失框
     //MMIPUB_OpenAlertWarningWin(TXT_CHARGE_FINISH);  
    //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CHARGE_FINISH,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_DIALOG_OK,HandleZDTPubWin);
     
    GPIO_SetValue(8, 0); // gpio 8

    MMIPHONE_PowerOff();
}


LOCAL MMI_RESULT_E Zdt_handleConfirmationDialog_ShippingMode(
                                                MMI_WIN_ID_T    win_id, // 窗口的ID
                                                MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                                DPARAM             param   // 相应消息的参数
                                                )
{
    MMI_RESULT_E    result  =   MMI_RESULT_FALSE;
    MMI_WIN_ID_T                wait_win_id             =   0;
    MMIPUB_INFO_T *add_data_ptr = PNULL;
    uint32                      is_delete_all_data      =   FALSE;
    uint32                      time_out                =   0;

    add_data_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL != add_data_ptr)
    {
        is_delete_all_data= (uint32)add_data_ptr->user_data;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
          GUIWIN_SeSoftkeytButtonTextId(win_id, IMAGE_NULL, LEFT_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_OK, MIDDLE_BUTTON, TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_CANCEL, RIGHT_BUTTON, TRUE);
        break;
   
    case MSG_APP_OK:
        //break;
    case MSG_APP_WEB:
         //MMIAPIPHONE_PowerOff(); //关机
       // MMIPUB_CloseQuerytWin(PNULL);
       //船运模式
       ZDT_open_shipping_mode();
         
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;

    default:
        break;
    }

   result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
    return (result);
}

void ZDT_entry_shipping_mode_screen(void)
{
    MMI_WIN_ID_T query_id = MMIPUB_QUERY_WIN_ID;
    
    MMI_WIN_PRIORITY_E       win_priority = WIN_THREE_LEVEL;
    
    //MMIPUB_OpenConfirmationDialogWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,qtn_query_poweroff,PNULL,PNULL,&query_id,PNULL,MMIPUB_SOFTKEY_OKCANCEL/*MMIPUB_SOFTKEY_DIALOG_OK*/,Zdt_handleConfirmationDialog,PNULL);
    //MMIPUB_OpenConfirmationDialogByTextId(PNULL,(MMI_TEXT_ID_T)TXT_ZDT_RESTORE_FACTORY,PNULL,PNULL,&ctrl_id,&win_priority,MMIPUB_SOFTKEY_DIALOG_OK,handleConfirmationDialogUNISOC_new);
    //MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_IDLE_FLYMODE_QUERY,IMAGE_PUBWIN_WARNING,PNULL,HandleFlyModeCloseQueryWinMsg,PNULL);        
    MMIPUB_OpenQueryWinByTextId(STR_OPEN_SHIPPING_MODE,IMAGE_PUBWIN_QUERY,&query_id,Zdt_handleConfirmationDialog_ShippingMode);

}


#endif


#if defined(ZDT_UNBIND_APP_CMD)//#指令解除绑定APP
extern int YX_API_DLT_Send(void);
void ZDT_unbind_app_cmd_func(void)
{
    YX_API_DLT_Send();
}
#endif





#ifdef ENG_SUPPORT
/*for idle dial num start*/
LOCAL const MMIENG_IDLE_DIAL_NUM_APP_T s_mmieng_idle_dial_num_app_tab[] =
{
#ifdef MEDIA_DEMO_SUPPORT
    {
    "###01",
    MMIENG_IDLE_DIAL_NUM_AUDIO_DEMO,
    PARS_MANUFACTURE_TEST,
    UITESTWIN_OpenAudioDemoWin      
    }
    ,

    {
    "###02",
    MMIENG_IDLE_DIAL_NUM_VIDEO_DEMO,
    PARS_MANUFACTURE_TEST,
    UITESTWIN_OpenVideoDemoWin     
    }
    ,
    
    {
    "###11",
    MMIENG_IDLE_DIAL_NUM_MEDIA_DEMO,
    PARS_ENGINEER_MEDIA_MENU,
    UITESTWIN_OpenMediaDemoWin
    }
    ,
    
#endif
    {
    "#*8378#0#",
    MMIENG_IDLE_DIAL_NUM_UI_TEST1,
    PARS_MANUFACTURE_TEST,
   // MMIAPIENG_StartUITest		//MS00193068
    UITESTWIN_OpenPhoneTestWin      
    }
    ,
    {
    "####1111#",
    MMIENG_IDLE_DIAL_NUM_UI_TEST2,
    PARS_MANUFACTURE_TEST,
    MMIAPIENG_StartUITest
    }
    ,
    {
    "#*8378#3#",
    MMIENG_IDLE_DIAL_NUM_PRODUCT,
    PARS_MANUFACTURE_TEST,
    MMIAPIENG_CreateProductWin
    }
    ,
#ifdef AUTODEMO_SUPPORT
    {
    "#*8378#7#",
    MMI_AUTODEMO_WINDOW,
    AUTODEMO_TEST,
    MMIAPIAUTODEMO_OpenAutodemoWindow
    }
    ,
#endif
    {
    "#*8378#4#",
    MMIENG_IDLE_DIAL_NUM_PHONE_INFO,
    PARS_MANUFACTURE_TEST,
    MMIAPIENG_CreatePhoneInfoWin
    }
    ,
//    {
//    "####7900#",
//    MMIENG_IDLE_DIAL_NUM_SET_MAX_POWER,
//    PARS_MANUFACTURE_TEST,
////    MMIAPIENG_SetMaxPowerOff
//    0
//     }
//  ,
    {
    "####2222#",
    MMIENG_IDLE_DIAL_NUM_PRODUCT_SN,
    PARS_MANUFACTURE_TEST,
    MMIAPIENG_CreateProductSNWin
    }
    ,
    {
    "####1040#",
    MMIENG_IDLE_DIAL_NUM_SHOW_NET,
    PARS_MANUFACTURE_TEST,
    MMIAPIENG_CreateShowNetWin
    }
    ,
    {
    "#*8378#1#",
    MMIENG_IDLE_DIAL_NUM_MENU,
    PARS_ENGINEER_MENU,
    MMIAPIENG_StartEngineerMenu
    }
    ,
    {
    "#*8378#2#",
    MMIENG_IDLE_DIAL_NUM_SHOW_VERSION1,
    PARS_ENGINEER_MENU,
#ifdef ENGTD_SUPPORT

    MMIAPIENGTD_StartEngineerMenu
#else
    MMIAPIENG_CreateShowOutVersionWin
#endif
    }
    ,
    {
    "####0000#",
    MMIENG_IDLE_DIAL_NUM_SHOW_VERSION2,
    PARS_ENGINEER_MENU,
    MMIAPIENG_CreateShowOutVersionWin
    }
	,
	{
    "#*8378#5#",
    MMIENG_IDLE_DIAL_NUM_CHIP_TEST,
    PARS_ENGINEER_MENU,
    MMIAPIENG_CreateChipTestWin
    }
#ifdef CMMB_SUPPORT
    ,
    {
    "#*8378#6#",
    MMIENG_IDLE_DIAL_NUM_CMMB_TEST,
    PARS_ENGINEER_MENU,
    MMIAPIENG_CreateCMMBTestWin
    }
#endif
    ,
  {
    "#*786646468#",
     MMIENG_IDLE_DIAL_NUM_NV_COUNTING,
    PARS_ENGINEER_MENU,
    MMIENGWIN_OpenNVCountingWin
    }

    ,
    {
    "#*8378#8#",
    MMIENG_IDLE_DIAL_NUM_MONKEY_TEST_AUTOSET,
    PARS_ENGINEER_MENU,
    MMIENG_OpenMonkeyTestSettingWin
    }
    ,
    {
    "#*8378#9#",
    MMIENG_IDLE_DIAL_NUM_OTA_ENTER,
    PARS_ENGINEER_MENU,
    MMIAPIENG_CreateOTAWin
    }
#ifdef TF_LOAD_SUPPORT
    ,
    {
    "#*8378#9#",
    MMIENG_IDLE_DIAL_NUM_TFLOAD_ENTER,
    PARS_ENGINEER_MENU,
    MMIAPIENG_CreateTFloadWin
    }
#endif
#ifdef MET_MEX_SUPPORT
	,
	{
	"*#6666#",
	MMIENG_IDLE_DIAL_NUM_MET_MEX,    
	PARS_ENGINEER_MENU,     
	MMIMEX_ShowPlatformEngineerMode
	}
#endif    
#ifdef MCARE_V31_SUPPORT
    ,
    {
        "*#3366*0#",
        MMIENG_IDLE_DIAL_MCARE_TEST,
        PARS_MANUFACTURE_TEST,
        MMIAPIENG_StartMcareTest
    }
#endif
// MMI_IMEI_EDIT_FUNC 
,
{
    "*#066#",//"#*66#",
    MMIENG_IMEI_WRITE,
    PARS_MANUFACTURE_TEST,
    MMIAPIENG_StartIMEIEdit
}
,
#ifdef ZDT_ZFB_SUPPORT // 支付宝快捷键重置
    {
        "#*7871#",
        MMIENG_ZFB_KEY_RESET_1,
        PARS_MANUFACTURE_TEST,
        ZFB_DB_INIT_OK_Reset_2,
    }   
    ,
    {
        "#*7872#",
        MMIENG_ZFB_KEY_RESET_2,
        PARS_MANUFACTURE_TEST,
        ZFB_DB_INIT_OK_Reset_All,
    }   
    ,
    {
        "#*7878#",
        MMIENG_ZFB_KEY_RESET_ALL,
        PARS_MANUFACTURE_TEST,
        ZFB_DB_INIT_OK_Reset_1,
    }  
    ,
#endif
#if defined(ZDT_SHIPPING_MODE_CMD)//shipping mode
{
    "#*880088#",
    MMIENG_SHIPPING_MODE_TEST,
    PARS_MANUFACTURE_TEST,
    ZDT_entry_shipping_mode_screen,
}  
,
#endif
    {
        "#*331#",
        MMIENG_OPEN_UDISK,
        PARS_MANUFACTURE_TEST,
        MMIUDISK_StartUdisk,
    }
    ,
#if defined(ZDT_UNBIND_APP_CMD)//#指令解除绑定APP
{
    "#*915368#",
    MMIENG_UNBIND_APP_CMD,
    PARS_MANUFACTURE_TEST,
    ZDT_unbind_app_cmd_func,
}  
,
#endif

};
/*for idle dial num end*/
#endif
LOCAL  MMIENG_LTE_MODE_E s_lte_mode_info = MMIENG_LTE_OPERATION_MODE_MAX;
/*for idle set language type num start*/
//各语言顺序必须与MMISET_LANGUAGE_TYPE_E的定义顺序相同
LOCAL const MMIENG_IDLE_SET_LANGUAGETYPE_T s_mmieng_idle_set_languagetype_tab[] =
{
    {//英语
    "*#0044#",
    MMISET_LANGUAGE_ENGLISH
    }
    ,
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    {//简体中文
    "*#0086#",
    MMISET_LANGUAGE_SIMP_CHINESE
    }
    ,
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    {//繁体中文
    "*#0886#",
    MMISET_LANGUAGE_TRAD_CHINESE
    }
    ,
#endif
#ifdef IM_ARABIC_SUPPORT
    {//阿拉伯语
    "*#0966#",
    MMISET_LANGUAGE_ARABIC
    }
    ,
#endif
#ifdef IM_FRENCH_SUPPORT
    {//法语
    "*#0033#",
    MMISET_LANGUAGE_FRENCH
    }
    ,	
#endif
#ifdef IM_HINDI_SUPPORT
    {//印地语
    "*#0091#",
    MMISET_LANGUAGE_HINDI
    }
    ,		
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    {//匈牙利语
    "*#0036#",
    MMISET_LANGUAGE_HUNGARIAN
    }
    ,
#endif
#ifdef IM_INDONESIAN_SUPPORT
    {//印度尼西亚语
    "*#0062#",
    MMISET_LANGUAGE_INDONESIAN
    }
    ,
#endif
#ifdef IM_MALAY_SUPPORT
    {//马来西亚语
    "*#0060#",
    MMISET_LANGUAGE_MALAY
    }
    ,	
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    {//葡萄牙语
    "*#0351#",
    MMISET_LANGUAGE_PORTUGUESE
    }
    ,	
#endif
#ifdef IM_RUSSIAN_SUPPORT
    {//俄语
    "*#0007#",
    MMISET_LANGUAGE_RUSSIAN
    }
    ,
#endif
#ifdef IM_SPANISH_SUPPORT
    {//西班牙语
    "*#0034#",
    MMISET_LANGUAGE_SPANISH
    }
    ,	
#endif
#ifdef IM_TAGALOG_SUPPORT
    {//塔加路语
    "*#0000#",
    MMISET_LANGUAGE_TAGALOG
    }
    ,			
#endif
#ifdef IM_THAI_SUPPORT
    {//泰语
    "*#0066#",
    MMISET_LANGUAGE_THAI
    }
    ,
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    {//越南语
    "*#0084#",
    MMISET_LANGUAGE_VIETNAMESE
    }
    ,
#endif
#ifdef IM_URDU_SUPPORT
    {//乌尔都语
    "*#0000#",
    MMISET_LANGUAGE_URDU
    }
    ,
#endif
#ifdef IM_ITALIAN_SUPPORT
    {//意大利语
    "*#0039#",
    MMISET_LANGUAGE_ITALIAN
    }
    ,
#endif
#ifdef IM_PERSIAN_SUPPORT
    {//波斯语
    "*#0000#",
    MMISET_LANGUAGE_PERSIAN
    }
    ,				
#endif
#ifdef IM_TURKISH_SUPPORT
    {//土耳其语
    "*#0090#",
    MMISET_LANGUAGE_TURKISH
    }
    ,		
#endif
#ifdef IM_GERMAN_SUPPORT
    {//德语
    "*#0049#",
    MMISET_LANGUAGE_GERMAN
    }
    ,	
#endif
#ifdef IM_GREEK_SUPPORT
    {//希腊语
    "*#0030#",
    MMISET_LANGUAGE_GREEK
    }
    ,
#endif
//以下为MMISET_LANGUAGE_TYPE_E中没有定义的语言		
    {//丹麦语
    "*#0045#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//捷克语
    "*#0420#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//波兰语
    "*#0048#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//芬兰语
    "*#0358#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//挪威语
    "*#0047#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//斯洛伐克语
    "*#0421#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//荷兰语
    "*#0031#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//瑞典语
    "*#0046#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//克罗地亚语
    "*#0385#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//罗马尼亚语
    "*#0040#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//斯洛文尼亚语
    "*#0386#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//希伯来语
    "*#0972#",
    MMISET_MAX_LANGUAGE
    }
    ,
    {//保加利亚语
    "*#0359#",
    MMISET_MAX_LANGUAGE
    }
};
/*for idle set language type num end*/

#ifdef ENG_SUPPORT
LOCAL BOOLEAN s_browser_change_url = TRUE;

LOCAL const wchar s_language_wstr[] = { 'L', 'a', 'n', 'g', 'u', 'a', 'g', 'e', 0 };
LOCAL const wchar s_at_wstr[] = { '@', 0 };
LOCAL const wchar s_englist_wstr[] = { 'E', 'n', 'g', 'l', 'i', 's', 'h', 0 };
LOCAL const wchar s_sms_send_wstr[] = { 'S', 'M', 'S', ' ', 's', 'e', 'n', 'd', 0 };
LOCAL const wchar s_assert_wstr[] = { 'a', 's', 's', 'e', 'r', 't', 0 };
LOCAL const wchar s_on_wstr[] = { 'O', 'N', 0 };
LOCAL const wchar s_on_failure_wstr[] = { 'O', 'N', ' ', 'f', 'a', 'i', 'l', 'u', 'r', 'e', 0 };
LOCAL const wchar s_off_wstr[] = { 'O', 'F', 'F', 0 };
LOCAL const wchar s_off_failure_wstr[] = { 'O', 'F', 'F', ' ', 'f', 'a', 'i', 'l', 'u', 'r', 'e', 0 };

LOCAL const wchar s_set_to_wstr[] = { 's', 'e', 't', ' ', 't', 'o', 0 };
LOCAL const wchar s_data_wstr[] = { 'D', 'a', 't', 'a', 0 };
LOCAL const wchar s_data_failure_wstr[] = { 'D', 'a', 't', 'a', ' ', 'f', 'a', 'i', 'l', 'u', 'r', 'e', 0 };
LOCAL const wchar s_atc_wstr[] = { 'A', 'T', 'C', 0 };
LOCAL const wchar s_atc_failure_wstr[] = { 'A', 'T', 'C', ' ', 'f', 'a', 'i', 'l', 'u', 'r', 'e', 0 };
LOCAL const wchar s_mixed_wstr[] = { 'M', 'I', 'X', 'E', 'D', 0 };
LOCAL const wchar s_mixed_failure_wstr[] = { 'M', 'I', 'X', 'E', 'D', ' ', 'f', 'a', 'i', 'l', 'u', 'r', 'e', 0 };
LOCAL const wchar s_closed_wstr[] = { 'C', 'l', 'o', 's', 'e', 'd', 0 };
LOCAL const wchar s_restart_to_wstr[] = { 'R', 'e', 's', 't', 'a', 'r', 't', ' ', 't', 'o',  0 };
LOCAL const wchar s_set_assert_wstr[] = {  's', 'e', 't', ' ', 'a', 's', 's', 'e', 'r', 't', 0 };
LOCAL const wchar s_debug_mode_wstr[] = {  'D', 'e', 'b', 'u', 'g', ' ', 'm', 'o', 'd', 'e', 0 };
LOCAL const wchar s_release_mode_wstr[] = {  'R', 'e', 'l', 'e', 'a', 's', 'e', ' ', 'm', 'o', 'd', 'e', 0 };
LOCAL const wchar s_send_wstr[] = { 'S', 'e', 'n', 'd', 0 };
LOCAL const wchar s_max_power_wstr[] = { 'm', 'a', 'x', ' ', 'p', 'o', 'w', 'e', 'r', 0 };
LOCAL const wchar s_restart_wstr[] = { 'R', 'e', 's', 't', 'a', 'r', 't', 0 };
LOCAL const wchar s_can_close_wstr[] = { 'c', 'a', 'n', ' ', 'c', 'l', 'o', 's', 'e', 0 };
LOCAL const wchar s_arm_log_wstr[] = { 'A', 'R', 'M', ' ', 'L', 'o', 'g', 0 };
LOCAL const wchar s_to_wstr[] = { 't', 'o', 0 };
LOCAL const wchar s_sleep_mode_wstr[] = {  'S', 'l', 'e', 'e', 'p', 'm', 'o', 'd', 'e', 0 };
LOCAL const wchar s_auto_power_wstr[] = {  'A', 'u', 't', 'o', 'P', 'o', 'w', 'e', 'r', 0 };

LOCAL const wchar s_set_hidden_partition_wstr[] = {  'S', 'e', 't', ' ', 'h', 'i', 'd', 'd', 'e', 'n', ' ', 'p', 'a', 'r', 't','i', 't', 'i', 'o', 'n', 0 };
LOCAL const wchar s_set_call_forward_query_wstr[] = {  'S', 'e', 't', ' ', 'C', 'a', 'l', 'l', ' ', 'F', 'o', 'r', 'w', 'a', 'r', 'd', ' ', 'Q', 'u', 'e', 'r', 'y', 0 };
LOCAL const wchar s_set_wap_log_wstr[] = {  'S', 'e', 't', ' ', 'W', 'a', 'p', ' ', 'L', 'o', 'g', 0 };
LOCAL const wchar s_set_wap_ua_wstr[] = {  'S', 'e', 't', ' ', 'W', 'a', 'p', ' ', 'U', 'A', 0 };
LOCAL const wchar s_default_wstr[] = {  'd', 'e', 'f', 'a', 'u', 'l', 't', 0 };

#ifdef JAVA_SUPPORT_IA
LOCAL const wchar s_set_java_auto_tck_mode_wstr[] = {  'S', 'e', 't', ' ', 'A', 'u', 't', 'o', ' ', 'T', 'c', 'k', ' ', 'M','o', 'd', 'e', 0 };
#endif
#ifdef CMMB_SUPPORT
LOCAL MMIMBBMS_FLOW_INFO_T      s_curr_flow_info = {0};
LOCAL MBBMS_SERVICE_RESULT_E    s_play_result = MTVSERVICE_RST_RET_OK;
LOCAL MMIMTV_PROG_ID_T          s_mtv_prog_info = {0};
LOCAL MMIENG_MTV_CHANNEL_DISP_MODE_E s_display_mode = MMIENG_MTV_CHANNEL_PLAY;
//LOCAL wchar record_file_full_name[MMIMTV_FULL_PATH_MAX_LEN +1]; 
#endif

LOCAL const wchar s_set_xsr_result_log_wstr[] = {  'P', 'r', 'i','n','t', ' ', 'B', 'A', 'D', ' ', 'B', 'l','o','c','k',' ','i','n','f','o', 0 };

#ifdef PLATFORM_SC6800H
LOCAL int32 s_plmn_sim_index = 01;  //PLMN_SELECT_INDEX 1~4
#endif

LOCAL uint32 s_screenlog_time[MMIENG_SCREEN_LOG_MAX] = {0};
LOCAL const char* s_screenlog_tag[MMIENG_SCREEN_LOG_MAX] = {0};
LOCAL MMIENG_SCREEN_DEBUG_T s_screen_debug_type = MMIENG_SCREEN_DEBUG_NONE;
#ifdef UI_MULTILAYER_SUPPORT	
LOCAL GUI_LCD_DEV_INFO s_debug_layer_handle = {0, UILAYER_NULL_HANDLE};
#endif
LOCAL BOOLEAN s_is_monkey_test_mode = 0;
LOCAL uint8 s_eng_enter_pressed_count = 0;
LOCAL uint8 s_eng_enter_timer_id = 0;

#ifdef MMI_WIFI_SUPPORT
LOCAL BOOLEAN s_flight_mode_wifi = TRUE;
#endif

#ifdef MMI_ENG_WIFI_SUPPORT
LOCAL uint8 s_wifi_scan_win_show_button = 0;
#endif

/**--------------------------------------------------------------------------*
			                         LOCAL FUNCTION DECLARE
 **--------------------------------------------------------------------------*/

LOCAL MMI_RESULT_E AppHandleEngMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

LOCAL MMI_RESULT_E ENGMainMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef MEDIA_DEMO_SUPPORT

LOCAL MMI_RESULT_E ENGDemoMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#endif

LOCAL void SetLanguageToEnglish(void);

LOCAL void OpenSMSAssert(void);

LOCAL void CloseSMSAssert(void);
#if 0
LOCAL void SetWapLog(BOOLEAN mode);

LOCAL void ResetWapUA(void);
#endif
#ifdef TP_TEST_SUPPORT
LOCAL MMI_RESULT_E GetParamEditbox(void);
LOCAL MMI_RESULT_E GetParamHandler(uint32 msg_id);
LOCAL void SetParamHandler(uint32 msg_id, uint32 value);
LOCAL MMI_RESULT_E SetParamEditbox(void);
LOCAL void GetPoints(uint32* data, uint32* size);
LOCAL MMI_RESULT_E ENGTpTestWinHandleMsg(
                                         MMI_WIN_ID_T      win_id, 	
                                         MMI_MESSAGE_ID_E  msg_id,
                                         DPARAM            param
                                         );
LOCAL MMI_RESULT_E ENGTpDrawWinHandleMsg(
                                         MMI_WIN_ID_T      win_id, 	
                                         MMI_MESSAGE_ID_E  msg_id,
                                         DPARAM            param
                                         );
LOCAL MMI_RESULT_E TestParamHandler(void);
#ifdef WIN32
LOCAL void Test_GetPoints(uint32* buf_addr, uint32* size);
#endif
PUBLIC uint32 TPSRV_GetParamHandler (uint32 msg_id);
PUBLIC uint32 TPSRV_GetPoints (uint32* buf_addr, uint32* size);
PUBLIC uint32 TPSRV_SetParamHandler (uint32 msg_id, uint32 data);
#endif
LOCAL MMI_RESULT_E ENGMonkeyTestSetWinHandleMsg(
                                         MMI_WIN_ID_T      win_id, 	
                                         MMI_MESSAGE_ID_E  msg_id,
                                         DPARAM            param
                                         );
LOCAL MMI_RESULT_E ENGMonkeyTestSetItemWinHandleMsg(
                                         MMI_WIN_ID_T      win_id, 	
                                         MMI_MESSAGE_ID_E  msg_id,
                                         DPARAM            param
                                         );
LOCAL void MMIAPIENG_SetCallForwardQuery(BOOLEAN   is_on);

LOCAL void InitIQMode(void);

#ifdef CMMB_SUPPORT
LOCAL BOOLEAN CmmbTest_ParseLine(char *FileInfo, char *str, char *buf, int buflen);
#endif

#if (defined(DM_SUPPORT) || defined(DM_ONLY_SELFREG_SUPPORT))
extern PUBLIC void    MMIAPIDM_OpenDebugMenuWin(void);           
#endif
#if 0
extern void MMIBRW_SetUA(uint8 *ua_ptr, uint16 ua_length);
extern void MMIBRW_SetDebugMode(BOOLEAN mode);
extern void MMIBRW_SetUAToDefault(void);
#endif
#ifdef MMI_RECORD_SUPPORT
LOCAL RECORD_SRV_HANDLE s_eng_record_handle = PNULL;
#endif
/********************************************************************************
 NAME:			CardLogOnForStartup
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			
********************************************************************************/
LOCAL void CardLogOnForStartup(void);

#ifdef PRINTSCREEN_SUPPORT
/*****************************************************************************/
//     Description : 抓屏功能
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngPrintScreenWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

LOCAL void SetXsrResult(BOOLEAN is_on);
#ifndef WIN32
#ifdef NANDBOOT_SUPPORT
/*****************************************************************************/
//     Description : 打印块信息功能
//    Global resource dependence : 
//  Author:   jinju.ma
//    Note:
/*****************************************************************************/
LOCAL void MMIENG_PrintFtlInfo(void);
#endif
#endif

#if 0
LOCAL MMI_RESULT_E EngDebugPortModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif
LOCAL MMI_RESULT_E EngDebugAssertModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngRFSendPowerOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngARMLOGOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngDSPLOGSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngSleepModeOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngAutoPoweronOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngGPRSOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EnCallForwardQueryOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngHiddenPartitionVisibleOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef JAVA_SUPPORT_IA
LOCAL MMI_RESULT_E EngJAVAAutoTCKOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

LOCAL MMI_RESULT_E EngDCDUpdateLOGOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

#ifdef PLATFORM_SC6800H
LOCAL MMI_RESULT_E EngPLMNSelectWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif


LOCAL MMI_RESULT_E EngStandbyWinOnOffHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngBacklightWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E ENGAutoUsbPortHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL BOOLEAN IsDCAllowedToBeOpened(void);
#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description : show error message as error type 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param [Input]:  fs_error    error  type as MMIFILE_ERROR_E
/*****************************************************************************/
LOCAL void  ShowRecordErrorMsg(MMIFILE_ERROR_E fs_error);
/*****************************************************************************/
//  Description : GetCurrentDev
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetCurrentDev(void);

/*****************************************************************************/
//  Description : 开始录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL MMIRECORD_SRV_RESULT_E StartENGRecord(void);

/*****************************************************************************/
// 	Description : HandleRecordEndMsg
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void HandleRecordEndMsg(MMIRECORD_SRV_RESULT_E result);

/*****************************************************************************/
// 	Description : NotifyCallback
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void NotifyCallback(RECORD_SRV_HANDLE record_srv_handle, 
						  MMIRECORD_SRV_SUPPORT_TYPE_E support_type, 
						  MMIRECORD_SRV_RESULT_E result);
#endif
#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//  Description : EngFlightModeWiFiModeOnOffWinHandleMsg
//  Global resource dependence :
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngFlightModeWiFiModeOnOffWinHandleMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param );
#endif
/*****************************************************************************/
//  Description : EngFlightModeOnOffWinHandleMsg
//  Global resource dependence :
//  Author: zhenpeng.yao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngFlightModeOnOffWinHandleMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param );
/*****************************************************************************/
//  Description : EngPasswordWinHandleMsg
//  Global resource dependence :
//  Author: siyuan.fei
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngPasswordWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

LOCAL MMI_RESULT_E EngSeLTEOperationModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :   HandleENGForbidAllgprsDataWinMsg
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleENGForbidAllgprsDataWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#if defined ENG_BAND_FREQ_SUPPORT
LOCAL MMI_RESULT_E ENGBandFreqWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL void BandFreqSIMSelectWin(void);
LOCAL MMI_RESULT_E BandFreqSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);
LOCAL MMI_RESULT_E ENGSetLTEBandWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E ENGSetGSMBandWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E ENGSetFreqWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleEngFreqOptionMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E HandleFreqEditBoxWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

/**--------------------------------------------------------------------------*
			                         GLOBAL DEFINITION
 **--------------------------------------------------------------------------*/

MMI_VOICEPARAM_E g_mmieng_voice_param = MMI_VOICEPARAM_NULL; /*lint !e552*/

//uint32 g_mmieng_afc_value = 0;		    /*lint !e552*/		// AFC
//uint32 g_mmieng_pa_value[4] = {0};			// PA
//uint32 g_mmieng_agc_value = 0;		    /*lint !e552*/		// AGC

uint32 g_mmieng_total_pch_number = 0;	/*lint !e552*/	// total data block
uint32 g_mmieng_lost_pch_number = 0;	/*lint !e552*/	// lost data block
// Engineer mode application
MMI_APPLICATION_T	g_mmieng_app;       /*lint !e552*/

LOCAL MMIENG_IQ_MODE_T s_iq_mode_param = {0};

#include "mmipdp_export.h"
#include "http_api.h"
#include "mmi_module.h"

typedef struct _GprsStatus
{
    BOOLEAN flags;
    MN_DUAL_SYS_E sim_sys;
    uint32 net_id;
}GprsStatus;

typedef struct __GPRSTEST_HTTP_CONTEXT
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance;
}GPRSTEST_HTTP_CONTEXT;

MMI_APPLICATION_T g_mmiGprsTest_app;
HTTP_APP_PARAM_T app_param = 0;
LOCAL GPRSTEST_HTTP_CONTEXT s_gprstest_http_context = {0};
GprsStatus mGprsStatus = {0};
uint32 g_mmi_pdp_id = 0;
TCPIP_NETID_T g_mmi_net_id = 0;

#define GPRS_TEST_HTTPS_URI_STRING "http://116.228.149.59/WAP/Download/index_download.html" //"https://www.baidu.com/" //"http://111.205.140.139:1080/index.html"

void GprsTestGetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size));
    }
}

#define HTTP_SIGNAL_ANALYTIC_EX(param,signal,app_param,http_context_ptr,app_module_id,sig_type) do{\
        GprsTestGetSignalStruct(param, &signal, sizeof(sig_type));\
        if(signal.module_id != app_module_id)\
        {\
            SCI_TRACE_LOW("HTTP_SIGNAL_ANALYTIC_EX: signal.module_id = %d, app_module_id = %d.", signal.module_id, app_module_id);\
            return MMI_RESULT_FALSE;\
        }\
        app_param = signal.app_param;\
        http_context_ptr->context_id   = signal.context_id;\
        http_context_ptr->app_instance = 0;\
        http_context_ptr->request_id = 0;\
        http_context_ptr->module_id    = signal.module_id;\
}while(0)\

#define HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,sig_type) do{\
    GprsTestGetSignalStruct(param, &signal, sizeof(sig_type));\
    if(signal.context_id != http_context_ptr->context_id)\
    {\
            return MMI_RESULT_FALSE;\
    }\
}while(0)\

LOCAL void GprsTest_SendHttpInitReq(void)
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    HTTP_ERROR_E error_code;

    SCI_TRACE_LOW("Gprs_SendHttpInitReq() Enter.");
    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    if(PNULL == init_param_ptr)
    {
        SCI_TRACE_LOW("Gprs_SendHttpInitReq() init_param_ptr alloc fail.");
        return;
    }
    init_param_ptr->is_cookie_enable = FALSE;
    init_param_ptr->is_cache_enable = FALSE;

    init_param_ptr->net_id = mGprsStatus.net_id;

#ifdef HTTP_SUPPORT
    error_code = HTTP_InitRequest(MMI_MODULE_GPRS_TEST, 1, init_param_ptr);
#endif
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;
    SCI_TRACE_LOW("Gprs_SendHttpInitReq() Exit.error_code = %d.", error_code);
}

//Send http get request
LOCAL void GprsTest_SendHttpGetReq(uint32 in_contextid, uint32 in_app_instance)
{
    uint32 context_id = in_contextid;
    uint32 app_instance = in_app_instance;
    HTTP_GET_PARAM_T* get_param_ptr = PNULL;
    HTTP_ERROR_E error_code = HTTP_SUCCESS;

    SCI_TRACE_LOW("GprsTest_SentHttpGetReq() Enter.");

    if((get_param_ptr = SCI_ALLOC(sizeof (HTTP_GET_PARAM_T))) ==NULL)
    {
        //SCI_TRACE_LOW:"\n HTTP TEST no memory \n"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,HTTP_TEST_88_112_2_17_23_18_0_765,(uint8*)"");
        return;
    }

    memset(get_param_ptr, 0, sizeof(HTTP_GET_PARAM_T));

#if 0
    len = strlen(GPRS_TEST_HTTPS_URI_STRING);
    SCI_TRACE_LOW("GprsTest HTTPS_URI_STRING = %s.", GPRS_TEST_HTTPS_URI_STRING);
    SCI_TRACE_LOW("GprsTest HTTPS_URI_STRING len = %d.", len);
    get_param_ptr->uri.uri_ptr = SCI_ALLOC(len+1);
    memset(get_param_ptr->uri.uri_ptr, 0, len+1);
    strcpy(get_param_ptr->uri.uri_ptr, GPRS_TEST_HTTPS_URI_STRING);

    SCI_TRACE_LOW("GprsTest_SendHttpGetReq()-- call HTTP_GetRequest().");
#else
    get_param_ptr->accept.accept_ptr = "text/vnd.wap.wml,application/vnd.wap.xhtml+xml,application/xhtml+xml,\
image/vnd.wap.wbmp,image/gif,image/jpg,image/jpeg,image/png,image/bmp,text/html,\
application/vnd.oma.drm.message,application/vnd.oma.drm.content,application/vnd.oma.drm.rights+xml,application/vnd.oma.drm.rights+wbxml,\
*/*;q=0.1";
    get_param_ptr->accept_charset.accept_charset_ptr = "UTF-8,ISO-8859-1,US-ASCII,windows-1252";
    get_param_ptr->accept_language.accept_language_ptr = "zh-cn,en,zh-tw";
#ifndef WIN32
    get_param_ptr->accept_encoding = "gzip";
#endif
    get_param_ptr->get_data.style =HTTP_DATA_TRANS_STYLE_UNKNOW;
    get_param_ptr->need_net_prog_ind=TRUE;
    get_param_ptr->ua_profile.ua_profile_ptr = "http://wap.sonyericsson.com/UAprof/K700cR201.xml";
    get_param_ptr->uri.uri_ptr ="http://www.baidu.com";
    get_param_ptr->user_agent.user_agent_ptr = "Dorado WAP-Browser/1.0.0";
    get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
#endif
    error_code = HTTP_GetRequest(context_id,get_param_ptr,app_instance);

    SCI_TRACE_LOW("GprsTest_SendHttpGetReq()--- call HTTP_GetRequest() end. error_code = %d.",error_code);
#if 0
    if (NULL != get_param_ptr->uri.uri_ptr)
    {
        SCI_FREE(get_param_ptr->uri.uri_ptr);
        get_param_ptr->uri.uri_ptr = NULL;
    }
#endif    
    SCI_FREE(get_param_ptr);
    get_param_ptr = NULL;
    SCI_TRACE_LOW("GprsTest_SendHttpGetReq() Exit.error_code = %d", error_code);
}

//Http Init Confirm
LOCAL MMI_RESULT_E GprsTest_HandleHttpInitCnf(GPRSTEST_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;
    SCI_TRACE_LOW("GprsTest_HandleHttpInitCnf() enter");

    if(PNULL == http_context_ptr)  //modify for coverity
    {
        SCI_TRACE_LOW("http_context_ptr is PNULL!");
        return MMI_RESULT_FALSE;
    }

    HTTP_SIGNAL_ANALYTIC_EX(param, signal, app_param, http_context_ptr,MMI_MODULE_GPRS_TEST,HTTP_INIT_CNF_SIG_T);
       SCI_TRACE_LOW("GprsTest_HandleHttpInitCnf() call Save param to s_gprstest_http_context.");
       SCI_TRACE_LOW("GprsTest_HandleHttpInitCnf() \
       s_gprstest_http_context.app_instance = %d, \
       s_gprstest_http_context.context_id = %d, \
       s_gprstest_http_context.module_id = %d, \
       s_gprstest_http_context.request_id = %d.",
       s_gprstest_http_context.app_instance,
       s_gprstest_http_context.context_id,
       s_gprstest_http_context.module_id,
       s_gprstest_http_context.request_id);

    SCI_TRACE_LOW("GprsTest_HandleHttpInitCnf() call SendHttpGetRequest()");
    GprsTest_SendHttpGetReq(http_context_ptr->context_id, http_context_ptr->app_instance);

    SCI_TRACE_LOW("GprsTest_HandleHttpInitCnf() exit, result = %d.", result);
    return result;
}

//Http Request Inds
LOCAL MMI_RESULT_E GprsTest_HandleHttpRequestInd(GPRSTEST_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};
    HTTP_APP_PARAM_T app_param = 0;

    SCI_TRACE_LOW("GprsTest_HandleHttpRequestInd() enter");

    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_REQUEST_ID_IND_SIG_T);

    s_gprstest_http_context.app_instance = signal.app_instance;
    s_gprstest_http_context.context_id = signal.context_id;
    s_gprstest_http_context.request_id = signal.request_id;

    SCI_TRACE_LOW("GprsTest_HandleHttpRequestInd() call Save param to s_gprstest_http_context.");
    SCI_TRACE_LOW("GprsTest_HandleHttpRequestInd() \
    s_gprstest_http_context.app_instance = %d, \
    s_gprstest_http_context.context_id = %d, \
    s_gprstest_http_context.module_id = %d, \
    s_gprstest_http_context.request_id = %d.",
    s_gprstest_http_context.app_instance,
    s_gprstest_http_context.context_id,
    s_gprstest_http_context.module_id,
    s_gprstest_http_context.request_id);

    SCI_TRACE_LOW("GprsTest_HandleHttpRequestInd() exit, result = %d.", result);
    return result;
}

//Http Header Ind
LOCAL MMI_RESULT_E GprsTest_HandleHttpHeaderInd(GPRSTEST_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    HTTP_HEADER_IND_SIG_T signal = {0};

    SCI_TRACE_LOW("GprsTest_HandleHttpHeaderInd() enter");

    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_HEADER_IND_SIG_T);
    SCI_TRACE_LOW("GprsTest_HandleHttpHeaderInd() call Save param to s_gprstest_http_context.");
    SCI_TRACE_LOW("GprsTest_HandleHttpHeaderInd() \
    s_gprstest_http_context.app_instance = %d, \
    s_gprstest_http_context.context_id = %d, \
    s_gprstest_http_context.module_id = %d, \
    s_gprstest_http_context.request_id = %d.",
    s_gprstest_http_context.app_instance,
    s_gprstest_http_context.context_id,
    s_gprstest_http_context.module_id,
    s_gprstest_http_context.request_id);

    if(signal.rsp_header_info.response_code == 200)
    {
        result = HTTP_HeaderResponse(http_context_ptr->context_id,http_context_ptr->app_instance,
                                     http_context_ptr->request_id,HTTP_DATA_TRANS_STYLE_BUFFER,PNULL,0);
    }

    SCI_TRACE_LOW("GprsTest_HandleHttpHeaderInd() exit, result = %d.", result);
    return result;
}

//Http Data Ind
LOCAL MMI_RESULT_E GprsTest_HandleHttpDataInd(GPRSTEST_HTTP_CONTEXT *http_context_ptr, DPARAM param)
{
    //---1---
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_DATA_IND_SIG_T signal = {0};
    //uint32 response_code = 0;
    uint32 data_len = 0;
    uint8 *data_ptr = PNULL;
    char* tmp_data_buff=NULL;

    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_DATA_IND_SIG_T);

    SCI_TRACE_LOW("GprsTest_HandleHttpDataInd() enter.");

    data_len = signal.data_len;
    data_ptr = signal.data_ptr;
    tmp_data_buff = (char *)malloc((data_len + 1) * sizeof(char));

    if (tmp_data_buff == NULL)
    {
        SCI_TRACE_LOW("tmp_data_buff malloc() failed.");
        return MMI_RESULT_FALSE;
    }

    memset(tmp_data_buff, 0, (data_len + 1) * sizeof(char));
    memcpy(tmp_data_buff, data_ptr, data_len);

    SCI_TRACE_LOW("GprsTest_HandleHttpDataInd() data = %s.", tmp_data_buff);
    SCI_TRACE_LOW("GprsTest_HandleHttpDataInd() length = %d.", data_len);
    SCI_TRACE_LOW("GprsTest_HandleHttpDataInd() signal.request_id = %d,signal.app_instance = %d.", signal.request_id, signal.app_instance);

    if (tmp_data_buff != NULL)
        free(tmp_data_buff);

    SCI_TRACE_LOW("GprsTest_HandleHttpDataInd() exit. result = %d.", result);
    return result;
}

//Http Get Confirm
LOCAL MMI_RESULT_E GprsTest_HandleHttpGetCnf(GPRSTEST_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_GET_CNF_SIG_T signal = {0};
    SCI_TRACE_LOW("GprsTest_HandleHttpGetCnf() enter.");

    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_GET_CNF_SIG_T);
    SCI_TRACE_LOW("response_code = %d.\n", signal.rsp_header_info.response_code);
    if (signal.rsp_header_info.response_code == 200)
    HTTP_CloseRequest(http_context_ptr->context_id);
    {
        SCI_TRACE_LOW("HandleSalesHttpGetCnf() success.");
    }

    SCI_TRACE_LOW("GprsTest_HandleHttpGetCnf() exit. result = %d.", result);
    return result;
}

//Http Close Confirm
LOCAL MMI_RESULT_E GprsTest_HandleHttpCloseCnf(GPRSTEST_HTTP_CONTEXT * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_CLOSE_CNF_SIG_T signal = {0};
    SCI_TRACE_LOW("GprsTest_HandleHttpCloseCnf() enter.");

    HTTP_SIGNAL_ANALYTIC(param,signal,http_context_ptr,HTTP_CLOSE_CNF_SIG_T);

    if(http_context_ptr->context_id == signal.context_id)
    {
        SCI_TRACE_LOW("GprsTest_HandleHttpCloseCnf() success");
        http_context_ptr->context_id = 0;
        http_context_ptr->request_id = 0;
        result = MMI_RESULT_TRUE;
#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
        MMIAPIPDP_Deactive(MMI_MODULE_GPRS_TEST);
#endif
    }

    SCI_TRACE_LOW("GprsTest_HandleHttpCloseCnf() exit. result = %d.", result);
    return result;
}

LOCAL MMI_RESULT_E GprsTest_HandleHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    BOOLEAN msgOk = FALSE;
    void *original_param = NULL;
    GPRSTEST_HTTP_CONTEXT *http_context_ptr = &s_gprstest_http_context;

    SCI_TRACE_LOW("GprsTest_HandleHttpMsg(), msg_id = %d.", msg_id);
    msgOk = HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END;

    if (!msgOk)
    {
        return result;
    }

    SCI_TRACE_LOW("GprsTest_HandleHttpMsg() Recv Vaild HttpMsg, msg_id = %d.", msg_id);

    original_param= (void*)((uint32)param-sizeof(xSignalHeaderRec));

    switch (msg_id)
    {
        case HTTP_SIG_INIT_CNF:
        {
            MMK_SendMsg(MMIENG_GPRS_TEST_WIN_ID, MSG_ASYN_SOCKET_ACCEPT_IND, "recv HTTP_SIG_INIT_CNF.");
            SCI_TRACE_LOW("recv HTTP_SIG_INIT_CNF.");
            GprsTest_HandleHttpInitCnf(http_context_ptr, param);
            break;
        }

        case HTTP_SIG_REQUEST_ID_IND:
        {
            MMK_SendMsg(MMIENG_GPRS_TEST_WIN_ID, MSG_ASYN_SOCKET_ACCEPT_IND, "recv HTTP_SIG_REQUEST_ID_IND.\n");
            SCI_TRACE_LOW("recv HTTP_SIG_REQUEST_ID_IND.");
            GprsTest_HandleHttpRequestInd(http_context_ptr, param);
            break;
        }

        case HTTP_SIG_HEADER_IND:
        {
            MMK_SendMsg(MMIENG_GPRS_TEST_WIN_ID, MSG_ASYN_SOCKET_ACCEPT_IND, "recv HTTP_SIG_HEADER_IND.\n");
            SCI_TRACE_LOW("recv HTTP_SIG_HEADER_IND.");
            GprsTest_HandleHttpHeaderInd(http_context_ptr, param);
            break;
        }

        case HTTP_SIG_DATA_IND:
        {
            MMK_SendMsg(MMIENG_GPRS_TEST_WIN_ID, MSG_ASYN_SOCKET_ACCEPT_IND, "recv HTTP_SIG_DATA_IND.\n");
            SCI_TRACE_LOW("recv HTTP_SIG_DATA_IND.");
            GprsTest_HandleHttpDataInd(http_context_ptr, param);
            break;
        }

        case HTTP_SIG_GET_CNF:
        {
            MMK_SendMsg(MMIENG_GPRS_TEST_WIN_ID, MSG_ASYN_SOCKET_ACCEPT_IND, "recv HTTP_SIG_GET_CNF.\n");
            SCI_TRACE_LOW("recv HTTP_SIG_GET_CNF.");
            GprsTest_HandleHttpGetCnf(http_context_ptr, param);
            break;
        }

        case HTTP_SIG_CLOSE_CNF:
        {
            MMK_SendMsg(MMIENG_GPRS_TEST_WIN_ID, MSG_ASYN_SOCKET_ACCEPT_IND, "recv HTTP_SIG_CLOSE_CNF.\n");
            SCI_TRACE_LOW("recv HTTP_SIG_CLOSE_CNF.");
            GprsTest_HandleHttpCloseCnf(http_context_ptr, param);
            break;
        }

        default:
            break;
    }

    return result;
}

LOCAL void HandleGprsTes_PdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("HandleGprsTes_PdpMsg(), msg_ptr->msg_id = %d.", msg_ptr->msg_id);
    switch(msg_ptr->msg_id)
    {
        case MMIPDP_ACTIVE_CNF:
        {
            SCI_TRACE_LOW("HandleGprsTes_PdpMsg(), MMIPDP_ACTIVE_CNF.msg_ptr->result = %d", msg_ptr->result);

            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                MMK_SendMsg(MMIENG_GPRS_TEST_WIN_ID, MSG_ASYN_SOCKET_ACCEPT_IND, "recv PDP ACTIVED.\n");
                mGprsStatus.flags = 1;
                mGprsStatus.net_id = msg_ptr->nsapi;
#ifdef MODEM_SUPPORT_SPRD
                g_mmi_pdp_id = msg_ptr->pdp_id;
#endif
                GprsTest_SendHttpInitReq();
            }
#if defined(MMI_GPRS_SUPPORT)
            else
            {
                MMIAPIPDP_Deactive(MMI_MODULE_GPRS_TEST);
            }
#endif
        }
            break;
        case MMIPDP_DEACTIVE_CNF:
            mGprsStatus.net_id = 0;
            break;

        case MMIPDP_DEACTIVE_IND:
            mGprsStatus.net_id = 0;
#if defined(MMI_GPRS_SUPPORT)
            MMIAPIPDP_Deactive(MMI_MODULE_GPRS_TEST);
#endif
            break;
        default:
            break;
    }
}

PUBLIC void GprsTest_SetUsedSim(int simId)
{
    mGprsStatus.sim_sys = simId;

    SCI_TRACE_LOW("GprsTest_SetUsedSim() simId = %d",mGprsStatus.sim_sys);
}

PUBLIC BOOLEAN GprsTest_RunPdpActive(void)
{
    MMIPDP_ACTIVE_INFO_T            active_info     = {0};
    BOOLEAN                         return_val      = FALSE;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

    SCI_TRACE_LOW("GprsTest_RunPdpActive()");

    {
        int i = 0;
        int kk = 0;
        //MN_DUAL_SYS_E sim_sys = 0;//s_sales.sim_sys;
        MN_DUAL_SYS_E sim_sys = mGprsStatus.sim_sys;
        uint8 linkNum = 0;

        linkNum = MMIAPICONNECTION_GetLinkSettingNum(sim_sys);
        SCI_TRACE_LOW("GprsTest_RunPdpActive： linkNum = %d.", linkNum);

        for (i = 0; i < linkNum; i++)
        {
            linksetting = PNULL;
            linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, i);
            SCI_TRACE_LOW("linksetting = %x", linksetting);

            // print apn information and plmn
            if(PNULL != linksetting && 0 != linksetting->apn_len)
            {
                SCI_TRACE_LOW("%d", linksetting->apn_len);
                for (kk = 0; kk < linksetting->apn_len; kk++)
                {
                    SCI_TRACE_LOW("[%d]--%x", kk, linksetting->apn[kk]);
                }
                SCI_TRACE_LOW("hplmn = %d, [%d, %d, %d]", linksetting->hplmn, linksetting->plmn.mcc, linksetting->plmn.mnc, linksetting->plmn.mnc_digit_num);

                // set apn, username and psw
                active_info.apn_ptr       = linksetting->apn;
                active_info.user_name_ptr = linksetting->username;
                active_info.psw_ptr       = linksetting->password;
                break;
            }
        }
        active_info.app_handler         = MMI_MODULE_GPRS_TEST;
        active_info.dual_sys            = sim_sys;
        active_info.priority            = 3;
        active_info.ps_service_rat      = MN_UNSPECIFIED;
        active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
        active_info.handle_msg_callback = HandleGprsTes_PdpMsg;
        active_info.ps_service_type = BROWSER_E;
        active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
        active_info.ip_type = MMICONNECTION_IP_V4;
#endif

        SCI_TRACE_LOW("SalesPDP call MMIAPIPDP_Active ");
#if defined(MMI_GPRS_SUPPORT)
        if(MMIAPIPDP_Active(&active_info))
        {
            return_val = TRUE;
        }
#endif
    }

    SCI_TRACE_LOW("GprsTest_RunPdpActive(), return_val = %d", return_val);
    return return_val;
}

PUBLIC void GprsTest_RunPdpDeActive(void)
{
    MMIAPIPDP_Deactive(MMI_MODULE_GPRS_TEST);
}

PUBLIC void GprsTest_InitModule(void)
{
    SCI_TRACE_LOW("MMIGprsTest_InitModule Enter");
    g_mmiGprsTest_app.ProcessMsg = GprsTest_HandleHttpMsg;
    SCI_TRACE_LOW("MMIGprsTest_InitModule Exit");
}

PUBLIC void GprsTest_StartByDelayTime(uint32 timeDelay)
{

}

WINDOW_TABLE(MMIENG_MAINMENU_WIN_TAB) = 
{
	WIN_HIDE_STATUS,
	WIN_FUNC((uint32)ENGMainMenuWinHandleMsg),
	WIN_ID(MMIENG_MAINMENU_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MENU_ENG, MMIENG_MAINMENU_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_AUTO_USB_PORT_WIN_TAB) = 
{
	WIN_HIDE_STATUS,
	WIN_FUNC((uint32)ENGAutoUsbPortHandleMsg),
	WIN_ID(MMIENG_AUTO_USB_PORT_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_AUTO_USB_PORT_CTRL_ID),
	END_WIN
};

#ifdef MEDIA_DEMO_SUPPORT

WINDOW_TABLE(MMIENG_MEDIA_DEMO_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)ENGDemoMenuWinHandleMsg),
    WIN_ID(MMIENG_MEDIA_DEMO_MENU_WIN_ID),
    WIN_TITLE(TXT_ENG_MEDIA_DEMO),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MEDIA_MENU_ENG, MMIENG_MEDIA_DEMO_CTRL_ID),
    END_WIN
};

#endif

#ifdef  TP_TEST_SUPPORT

WINDOW_TABLE(MMIENG_TP_TEST_WIN_TAB) = 
{   
        WIN_FUNC((uint32)ENGTpTestWinHandleMsg),
        WIN_ID(MMIENG_TP_TEST_WIN_ID),
        WIN_HIDE_STATUS,
        CREATE_BUTTON_CTRL(PNULL, TP_GET_BUTTON_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, TP_SET_BUTTON_CTRL_ID),
        CREATE_BUTTON_CTRL(PNULL, TP_TEST_BUTTON_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,TP_WIDTH_EDITBOX_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,TP_HEIGHT_EDITBOX_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,REFERENCE_RESISTOR_EDITBOX_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,REFERENCE_VOLTAGE_EDITBOX_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,DPOINT_XLIMIT_EDITBOX_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,DPOINT_YLIMIT_EDITBOX_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,SPOINT_RT_MAX_EDITBOX_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,SPOINT_RT_MIN_EDITBOX_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,DPOINT_RT_MAX_EDITBOX_CTRL_ID),
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,DPOINT_RT_MIN_EDITBOX_CTRL_ID),        
        CREATE_EDIT_DIGITAL_CTRL(TP_TEST_EDIDBOX_PARA_COUNT,DOUBLE_POINT_EDITBOX_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(MMIENG_TP_TEST_DRAW_WIN_TAB) = 
{   
        WIN_FUNC((uint32)ENGTpDrawWinHandleMsg),
        WIN_ID(MMIENG_TP_TEST_DRAW_WIN_ID),
        WIN_HIDE_STATUS,
        END_WIN
};
#endif

WINDOW_TABLE(MMIENG_MONKEY_TEST_SET_TAB) = 
{
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)ENGMonkeyTestSetWinHandleMsg),
        WIN_ID(MMIENG_MONKEY_TEST_SET_WIN_ID),
        WIN_TITLE(TXT_ENG_MONKEY_TEST_SET),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(MMIENG_MONKEY_TEST_SET_ITEM_TAB) = 
{   
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)ENGMonkeyTestSetItemWinHandleMsg),
        WIN_ID(MMIENG_MONKEY_TEST_SET_ITEM_WIN_ID),
        WIN_TITLE(TXT_ENG_MONKEY_TEST_SET),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIENG_LISTBOX_CTRL_ID),
        END_WIN
};



WINDOW_TABLE(MMIENG_SCREEN_ALWAYS_ON_WIN_TAB) = 
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)EngBacklightWinHandleMsg),
    WIN_ID(MMIENG_SCREEN_ALWAYS_ON_WIN_ID),
    WIN_TITLE(TXT_ENG_UIBACKLIGHT_TITLE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_SET_LTE_OPERATION_MODE_WIN_TAB) = //开关
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)EngSeLTEOperationModeWinHandleMsg),
    WIN_ID(MMIENG_SET_LTE_OPERATION_MODE_WIN_ID),
    WIN_TITLE(TXT_ENG_SET_LTE_OPERATION_MODE),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

//Forbid All gprs Data
WINDOW_TABLE(MMIENG_FORBID_ALLGPRS_DATA_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleENGForbidAllgprsDataWinMsg),
    WIN_ID(MMIENG_FORBID_GPRS_DATA_WIN_ID),
    WIN_TITLE(TXT_ENG_FORBID_ALL_DATA),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
    END_WIN
};

/********************************************************************************
 NAME:          EngBacklightWinHandleMsg
 DESCRIPTION:
 PARAM IN:      win_id -
                msg_id -
                param -
 PARAM OUT:
 AUTHOR:        chunjuan.liang
 DATE:
********************************************************************************/
LOCAL MMI_RESULT_E EngBacklightWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_1RADIO_ENG);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_1RADIO_ENG);

        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
        if(TRUE != MMIDEFAULT_IsBacklightOn())
        {
            cur_selection = 1;
        }
        else
        {
            cur_selection = 0;
        }
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_KEYDOWN_OK:
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        if(0 == cur_selection) //open backlight
        {
            if(TRUE != MMIDEFAULT_IsBacklightOn())
            {
                MMIDEFAULT_TurnOnBackLight();
            }
        }
        else if(1 == cur_selection)//close backlight
        {
            if(TRUE == MMIDEFAULT_IsBacklightOn())
            {
                MMIDEFAULT_TurnOffBackLight();
                MMIDEFAULT_CloseAllLight_Watch();
            }
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#if defined ENG_BAND_FREQ_SUPPORT
WINDOW_TABLE(MMIENG_BAND_FREQ_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)ENGBandFreqWinHandleMsg),
    WIN_ID(MMIENG_BAND_FREQ_WIN_ID),
    WIN_TITLE(TXT_ENG_BAND_FREQ_SET),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(BAND_FREQ_MENU_ENG, MMIENG_BAND_FREQ_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_SET_LTE_BAND_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)ENGSetLTEBandWinHandleMsg),
    WIN_ID(MMIENG_SET_LTE_BAND_WIN_ID),
    WIN_TITLE(TXT_ENG_SET_LTE_BAND),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIENG_BAND_LISTBOX_CTRL_ID),
    END_WIN
};
WINDOW_TABLE(MMIENG_SET_GSM_BAND_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)ENGSetGSMBandWinHandleMsg),
    WIN_ID(MMIENG_SET_GSM_BAND_WIN_ID),
    WIN_TITLE(TXT_ENG_SET_GSM_BAND),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIENG_BAND_LISTBOX_CTRL_ID),
    END_WIN
};
//set freq win
WINDOW_TABLE(MMIENG_SET_FREQ_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)ENGSetFreqWinHandleMsg),
    WIN_ID(MMIENG_SET_FREQ_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIENG_FREQ_LISTBOX_CTRL_ID),
    END_WIN
};
//freq option menu
WINDOW_TABLE(ENG_FREQ_OPTION_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleEngFreqOptionMsg),
    WIN_ID(ENG_FREQ_OPTION_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

// freq edit win
#define ENG_FREQ_EDIT_LEN 5
WINDOW_TABLE(ENG_FREQ_EDIT_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_ID( ENG_FREQ_EDIT_WIN_ID),
    WIN_TITLE(TXT_ENG_SET_FREQ),
    WIN_FUNC((uint32)HandleFreqEditBoxWinMsg),
    CREATE_EDIT_DIGITAL_CTRL(ENG_FREQ_EDIT_LEN,MMIENG_FREQ_EDIT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
/*===============================================================================
								IMPLEMENTATION
===============================================================================*/

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_InitModule(void)
{
    MMIAPIENG_RegNv();
    MMIENG_RegWinIdNameArr();
}

/********************************************************************************
 NAME:			MMIAPIENG_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENG_AppInit(void)
{
    g_mmieng_app.ProcessMsg = AppHandleEngMsg;  /*lint !e552*/
//    g_mmieng_app.component_type = CT_APPLICATION;

    //注册菜单
    MMIENG_RegMenuGroup();

    InitIQMode();
    
    MMIENG_RecordStandbyInit();
}

/********************************************************************************
 NAME:			MMIAPIENG_RedkeyIsEnableInIdle
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RedkeyIsEnableInIdle(void)
{
	return MMIAPIENG_GetPowerKeyFlag();
}

/********************************************************************************
 NAME:			MMIAPIENG_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartEngineerMenu(void)
{
	return MMK_CreateWin((uint32*)MMIENG_MAINMENU_WIN_TAB, PNULL);
}
// MMI_IMEI_EDIT_FUNC 
PUBLIC BOOLEAN MMIAPIENG_StartIMEIEdit(void)
{
    MMIENG_OpenuUpdateIMEIWin();
    return TRUE;
}

/********************************************************************************
 NAME:			MMIAPIENG_MainWinEnter
 DESCRIPTION:	access point
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		chunjuan.liang 
 DATE:			2020.4.4
********************************************************************************/
PUBLIC void MMIAPIENG_MainWinEnter(uint8 timer_id, uint32 param)
{
    BOOLEAN new_enter = (BOOLEAN)param;
    SCI_TRACE_LOW("[ENG] WatchEng_MainWinEnter new enter = %d,timer id = %d",new_enter,timer_id);
    if(FALSE == new_enter)
    {
        if(0 < s_eng_enter_timer_id)
        {
            s_eng_enter_pressed_count++;
            SCI_TRACE_LOW("[ENG] WatchEng_MainWinEnter s_eng_enter_pressed_count = %d",s_eng_enter_pressed_count);

            if(MMIENG_ENTRY_TICK == s_eng_enter_pressed_count)
            {
                s_eng_enter_pressed_count = 0;
                MMK_StopTimer(s_eng_enter_timer_id);
                s_eng_enter_timer_id = 0;

                MMIAPIENG_StartEngineerMenu();
            }
        }
        else
        {
            s_eng_enter_pressed_count++;
            s_eng_enter_timer_id = MMK_CreateTimerCallback(MMIENG_ENTRY_TIME_OUT, MMIAPIENG_MainWinEnter, TRUE, TRUE);
        }
    }else
    {
        s_eng_enter_pressed_count = 0;

        if(0 < s_eng_enter_timer_id)
        {
            MMK_StopTimer(s_eng_enter_timer_id);
            s_eng_enter_timer_id = 0;
        }
    }

    SCI_TRACE_LOW("[ENG] WatchEng_MainWinEnter OUT: s_eng_enter_timer_id = %d, s_eng_enter_pressed_count = %d", s_eng_enter_timer_id,s_eng_enter_pressed_count);
}

#ifdef MEDIA_DEMO_SUPPORT

PUBLIC BOOLEAN UITESTWIN_OpenMediaDemoWin(void)
{
    return MMK_CreateWin((uint32*)MMIENG_MEDIA_DEMO_WIN_TAB, PNULL);
}

#endif

/********************************************************************************
 NAME:			MMIAPIENG_ParseEngPhoneInfoString
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.25
********************************************************************************/
BOOLEAN MMIAPIENG_ParseEngPhoneInfoString(uint8* str_str, uint8 str_len)
{
	if(str_str == PNULL)
	{
	    //SCI_TRACE_LOW:"mmieng_main.c MMIAPIENG_ParseEngPhoneInfoString str_str == null"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_793_112_2_18_2_17_11_0,(uint8*)"");
		return  FALSE;
	}
	
	if (str_len != 9)
	{
		return  FALSE;
	}
	if (strncmp((char*)str_str, "#*8378#4#", 9) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/********************************************************************************
 NAME:			GetWapMmsSasTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		hui.zhao
 DATE:			2009.07.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetWapMmsSasTestMode(void)
{
    BOOLEAN             is_on = FALSE;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_WAPMMS_SAS_TEST_MODE_ID, &is_on, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        is_on = FALSE;
        MMINV_WRITE(MMIENG_NV_WAPMMS_SAS_TEST_MODE_ID, &is_on);
    }
    
    return is_on;
}

/********************************************************************************
 NAME:			MMIAPIENG_SetSensorMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.08.08
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetSensorMode(MMIENG_SENSOR_MODE_E sensor_mode)
{
    //SCI_TRACE_LOW:"MMIENG_MAIN.C MMIAPIENG_SetSensorMode sensor_mode is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_843_112_2_18_2_17_11_1,(uint8*)"d", sensor_mode);
	if(sensor_mode >=  MMIENG_SENSOR_MODE_MAX)
	{
        return FALSE;
	}
    MMINV_WRITE(MMIENG_NV_SENSOR_MODE_ID, &sensor_mode);    
	return TRUE;
}

/********************************************************************************
 NAME:			MMIAPIENG_SetChipTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2006.08.08
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetChipTestMode(MMIENG_CHIP_TEST_SETTING_E test_mode)
{
    //SCI_TRACE_LOW:"MMIENG_MAIN.C MMIAPIENG_SetChipTestMode test_mode is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_861_112_2_18_2_17_11_2,(uint8*)"d",test_mode);
	if(test_mode >=  MMIENG_CHIP_TEST_SETTING_MAX)
	{
        return FALSE;
	}
    MMINV_WRITE(MMIENG_NV_CHIP_TEST_MODE_ID, &test_mode);    
	return TRUE;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetSensorMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.08.08
********************************************************************************/
PUBLIC MMIENG_SENSOR_MODE_E MMIAPIENG_GetSensorMode(void)
{
	MMIENG_SENSOR_MODE_E sensor_mode = MMIENG_SENSOR_MODE_DEFAULT;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_SENSOR_MODE_ID, &sensor_mode, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        sensor_mode = MMIENG_SENSOR_MODE_DEFAULT;;
        MMIAPIENG_SetSensorMode(sensor_mode);
    }
    
    //SCI_TRACE_LOW:"MMIENG_MAIN.C MMIAPIENG_GetSensorMode sensor_mode is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_890_112_2_18_2_17_11_3,(uint8*)"d",sensor_mode);

	return sensor_mode;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetChipTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2007.08.08
********************************************************************************/
PUBLIC MMIENG_CHIP_TEST_SETTING_E MMIAPIENG_GetChipTestMode(void)
{
	MMIENG_CHIP_TEST_SETTING_E test_mode = MMIENG_CHIP_TEST_SETTING_ON;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_CHIP_TEST_MODE_ID, &test_mode, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        test_mode = MMIENG_CHIP_TEST_SETTING_OFF;
        MMINV_WRITE(MMIENG_NV_CHIP_TEST_MODE_ID, &test_mode);    
        //MMIAPIENG_SetChipTestMode(test_mode);
    }
    //SCI_TRACE_LOW:"MMIENG_MAIN.C MMIAPIENG_GetChipTestMode test_mode is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_915_112_2_18_2_17_11_4,(uint8*)"d",test_mode);
	return test_mode;
}

/********************************************************************************
 NAME:			MMIAPIENG_SetDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetDVFrameFre(MMIENG_DV_FRAME_FRE_E dv_frame_frequency)
{
    //SCI_TRACE_LOW:"MMIENG_MAIN.C MMIAPIENG_SetDVFrameFre dv_frame_frequency is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_928_112_2_18_2_17_11_5,(uint8*)"d",dv_frame_frequency);
    if(dv_frame_frequency >= MMIENG_DV_FRAME_FRE_MAX)
    {
        return FALSE;
    }
    MMINV_WRITE(MMIENG_NV_DV_FRAME_FRE_ID, &dv_frame_frequency);    
    return TRUE;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC MMIENG_DV_FRAME_FRE_E MMIAPIENG_GetDVFrameFre(void)
{
    MMIENG_DV_FRAME_FRE_E dv_frame_frequency = MMIENG_DV_FRAME_FRE_30HZ;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_DV_FRAME_FRE_ID, &dv_frame_frequency, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        dv_frame_frequency = MMIENG_DV_FRAME_FRE_30HZ;
        MMIAPIENG_SetDVFrameFre(dv_frame_frequency);
    }
    //SCI_TRACE_LOW:"MMIENG_MAIN.C MMIAPIENG_GetDVFrameFre dv_frame_frequency is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_956_112_2_18_2_17_11_6,(uint8*)"d",dv_frame_frequency);
	return dv_frame_frequency;
}


#ifdef PCLINK_SUPPORT
extern TCPIP_PING_HANDLE    g_pclink_ping_handle;
extern TCPIP_PING_RESULT_E  g_pclink_ping_res;
#endif /* PCLINK_SUPPORT */

/********************************************************************************
 NAME:			AppHandleEngMsg
 DESCRIPTION:	
 PARAM IN:		app_ptr - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
LOCAL MMI_RESULT_E AppHandleEngMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
	Signal* sig = PNULL;
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	if(param == PNULL)
	{
	    //SCI_TRACE_LOW:"mmieng_main.c AppHandleEngMsg app_ptr == null"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_982_112_2_18_2_17_11_7,(uint8*)"");
	    return MMI_RESULT_FALSE;
	}
	//SCI_TRACE_LOW:">>>>>>>>>  enter AppHandleEngMsg: msg_id = 0x%04X"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_985_112_2_18_2_17_11_8,(uint8*)"d", msg_id);
	sig = (Signal*)param;
	switch (msg_id) 
	{
#if 0        
		case APP_L1_GET_AFC_CNF:
			g_mmieng_afc_value = ((MMI_L1_MESSAGE*)sig)->param1;
            MMK_PostMsg(MMIENG_AFC_WIN_ID,MSG_ENG_L1_GET_CNF,PNULL,0);		
//			MMK_CloseWin(MMIENG_WAITING_WIN_ID);
			break;
		
		case APP_L1_GET_PA_CNF:
			//g_mmieng_pa_value = ((MMI_L1_MESSAGE*)sig)->param1;	                          
			SCI_MEMCPY(g_mmieng_pa_value, ((MMI_L1_MESSAGE*)sig)->param1, 4*sizeof(uint32));/*lint !e64*/	
		 MMK_PostMsg(MMIENG_PA_WIN_ID,MSG_ENG_L1_GET_CNF,PNULL,0); 
			//			MMK_CloseWin(MMIENG_WAITING_WIN_ID);
			break;
			
		case APP_L1_GET_AGC_CNF:
			g_mmieng_agc_value = ((MMI_L1_MESSAGE*)sig)->param1;
            MMK_PostMsg(MMIENG_AGC_WIN_ID,MSG_ENG_L1_GET_CNF,PNULL,0);		
//			MMK_CloseWin(MMIENG_WAITING_WIN_ID);
			break;
#endif				
		case APP_L1_GET_BLOCK_ERROR_RATIO_CNF:			
			g_mmieng_total_pch_number = ((MMI_L1_MESSAGE*)sig)->param1;
			g_mmieng_lost_pch_number = ((MMI_L1_MESSAGE*)sig)->param2;		
			MMK_CloseWin(MMIENG_WAITING_WIN_ID);
			break;


#ifdef PCLINK_SUPPORT
        case TCPIP_PING_RESULT_IND:
            {
                TCPIP_PING_RESULT_IND_SIG_T* sig_ptr = 
                    (TCPIP_PING_RESULT_IND_SIG_T*)((uint32)param);
                if( g_pclink_ping_handle == sig_ptr->handle )
                {
                    g_pclink_ping_res = sig_ptr->result;
                    if( PINGRES_SUCCESS == sig_ptr->result )
                    {
                        MMK_PostMsg(
                            MMIENG_PCLINK_PING_TEST_WAIT_WIN_ID,
                            MSG_ENG_PCLINK_PING_SUC,
                            PNULL,
                            0);
                    }
                    else
                    {
                        MMK_PostMsg(
                            MMIENG_PCLINK_PING_TEST_WAIT_WIN_ID,
                            MSG_ENG_PCLINK_PING_FAIL,
                            PNULL,
                            0);
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"mmieng pclink: ping result not for ours 0x%x, received 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_1044_112_2_18_2_17_11_9,(uint8*)"dd",g_pclink_ping_handle, sig_ptr->handle);
                }
            }
            break;
#endif /* PCLINK_SUPPORT */
#ifndef WIN32
#ifdef GOODIX_SUPPORT_GR5515
        case MSG_ENG_SENSORHUB_TEST:
             {
                SENSORHUB_ENGTEST_CB_T  *callback_ptr = (SENSORHUB_ENGTEST_CB_T*) param;
                MMI_STRING_T process_info = {0};
                wchar wstr_temp[20] = {0};
                char str_temp[20] = {0};
                uint8 len = 0;
                if(PNULL == param)
                {
                    break;
                }
                if(callback_ptr->opcode == STATE_DOWN_TEST_FW)
                {
                    //success
                    sprintf(str_temp, "TEST_FW_Pro: %d%%", callback_ptr->status);
                    len = strlen(str_temp);
                }
                else  if(callback_ptr->opcode == STATE_DOWN_APP_FW)
                {
                    //success
                    sprintf(str_temp, "APP_FW_Pro: %d%%", callback_ptr->status);
                    len = strlen(str_temp);
                }
                else
                {
                    //fail
                    sprintf(str_temp, "Fail: %d", callback_ptr->opcode);
                    len = strlen(str_temp);
                }
                if(callback_ptr->status > 100)
                {
                    //may success(callback_ptr->status = PRO_END_SUCCESS)
                    SCI_TRACE_LOW("[SensorHub]:MSG_ENG_SENSORHUB_TEST opcode = %d,status = %d",callback_ptr->opcode ,callback_ptr->status);
                }
                else
                {
                    MMI_STRNTOWSTR(wstr_temp, len, str_temp, len, len);
                    process_info.wstr_ptr = wstr_temp;
                    process_info.wstr_len = len;
                    SCI_TRACE_LOW("MSG_ENG_SENSORHUB_TEST opcode = %d,status = %d",callback_ptr->opcode ,callback_ptr->status);
                    MMK_SendMsg(callback_ptr->win_id, callback_ptr->msg_id, &process_info);
                }
            }
            break;
#endif
#endif
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
    return (result);
}

#ifdef JAVA_SUPPORT_IA
LOCAL void SetJavaAutoTckOn(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = s_set_java_auto_tck_mode_wstr;/*lint !e605*/
	text1.wstr_len = MMIAPICOM_Wstrlen(s_set_java_auto_tck_mode_wstr);
	text2.wstr_ptr = s_to_wstr;/*lint !e605*/
	text2.wstr_len = MMIAPICOM_Wstrlen(s_to_wstr);
	text3.wstr_ptr = s_on_wstr;/*lint !e605*/
	text3.wstr_len = MMIAPICOM_Wstrlen(s_on_wstr);
	MMIAPIJAVA_SetJavaAutoTckOn();
	MMIEng_SetResultWinText(text1, text2, text3);

	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
	
}

LOCAL void SetJavaAutoTckOff(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = s_set_java_auto_tck_mode_wstr;/*lint !e605*/
	text1.wstr_len = MMIAPICOM_Wstrlen(s_set_java_auto_tck_mode_wstr);
	text2.wstr_ptr = s_to_wstr;/*lint !e605*/ //(uint8*)"to";
	text2.wstr_len = MMIAPICOM_Wstrlen(s_to_wstr);
	text3.wstr_ptr = s_off_wstr;/*lint !e605*/ //(uint8*)"off";
	text3.wstr_len = MMIAPICOM_Wstrlen(s_off_wstr);
	MMIJAVA_SetJavaAutoTckOff();
	MMIEng_SetResultWinText(text1, text2, text3);

	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
	
}
#endif

#ifdef MEDIA_DEMO_SUPPORT
/********************************************************************************
 NAME: ENG_MainMenuWin_HandleMsg
 DESCRIPTION:
 PARAM IN:  win_id - 
            msg_id - 
            param - 
 PARAM OUT:
 AUTHOR:    allen
 DATE:      2004.08.18
********************************************************************************/
LOCAL MMI_RESULT_E ENGDemoMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 group_id = 0;
    uint32 menu_id = 0;

    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);
            MMK_SetAtvCtrl(MMIENG_MEDIA_DEMO_MENU_WIN_ID, MMIENG_MEDIA_DEMO_CTRL_ID);
/*
            #ifdef CAMERA_SUPPORT
            {
                DCAMERA_IMG_SENSOR_TYPE_E sensor_type = 0;

                sensor_type = MMIAPIDC_GetDCModulSensorType();
                if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
                {
                    MMIAPICOM_EnableGrayed(win_id, MENU_DC_PARAM, ID_ENG_DC_PARAM_SCREEN_SWITCH, TRUE);
                    MMIAPICOM_EnableGrayed(win_id, MENU_DC_PARAM, ID_ENG_DC_PARAM_PREV_SET, TRUE);
                    MMIAPICOM_EnableGrayed(win_id, MENU_DC_PARAM, ID_ENG_DC_PARAM_SENSOR_ANGLE, TRUE);
                }
            }
            #endif*/
        break;

        case MSG_FULL_PAINT:
        break;
        
       	
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(MMIENG_MEDIA_DEMO_MENU_WIN_ID);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB: 

            GUIMENU_GetId(MMIENG_MEDIA_DEMO_CTRL_ID, &group_id, &menu_id);

            switch (menu_id)
            {
                case ID_ENG_HELPINFO:
                    MMK_CreateWin((uint32*)MMIENG_SHOWHELP_WIN_TAB,  PNULL);
                break;

                case ID_ENG_MEDIA_AUDIO_DEMO:
                    UITESTWIN_OpenAudioDemoWin();
                break;

                case ID_ENG_MEDIA_VIDEO_DEMO:
                    UITESTWIN_OpenVideoDemoWin();
                break;

                default:
                break;
            }
        break;

        default:
            result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#endif

PUBLIC BOOLEAN MMIENG_OpenMonkeyTestSettingWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_MONKEY_TEST_SET_TAB, PNULL);
    return TRUE;
}
LOCAL MMI_RESULT_E ENGAutoUsbPortHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 cur_selection = 0;
    MMI_CTRL_ID_T ctrl_id = MMIENG_AUTO_USB_PORT_CTRL_ID;
 
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
        {
            break;
        }

        case MSG_FULL_PAINT:
        {
            GUILIST_SetMaxItem(ctrl_id,2, FALSE );//max item 2
            GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
            MMIAPISET_AppendListItemByTextIdExt(TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_1RADIO_ENG);
            MMIAPISET_AppendListItemByTextIdExt(TXT_COMM_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_1RADIO_ENG);

#ifndef WIN32
#if !defined(PLATFORM_UWS6121E)
            if(USB_GetIsAutoUsbLog())
            {
                cur_selection = 0;
            }
            else
            {
                cur_selection = 1;
            }
#endif
#endif

            //set selected item
            GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

            //set current item
            GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
            MMK_SetAtvCtrl(win_id,ctrl_id); 
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif 
        case MSG_CTL_OK:
        case MSG_APP_WEB: 
        {
            uint8 is_usb_log_close = 1;
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            if(0 == cur_selection)
            {
                is_usb_log_close = 0;
#ifdef PLATFORM_ANTISW3
#ifndef WIN32
                USB_SetAutoUsbLog(is_usb_log_close);
#endif
#endif
            }
            else if(1 == cur_selection)
            {
                is_usb_log_close = 1;
#ifdef PLATFORM_ANTISW3
#ifndef WIN32
                USB_SetAutoUsbLog(is_usb_log_close);
#endif
#endif
            }
            //set current item
            GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }

        default:
            break;
    }
    return MMI_RESULT_TRUE;
}

/********************************************************************************
 NAME:			ENG_MainMenuWin_HandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.18
********************************************************************************/
LOCAL MMI_RESULT_E ENGMainMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 group_id = 0;
    uint32 menu_id = 0;
#ifdef MMI_AUTOTEST_SUPPORT
    uint8 at_set_id = 0;
#endif
#ifdef CAMERA_SUPPORT
    LCD_INFO_T lcd_info = {0};
#endif

    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            CTRLMENU_SetSecMenuFontSize(MMIENG_MAINMENU_CTRL_ID,MMIENG_MENU_ITEM_FONT,MMIENG_MENU_ITEM_FONT);
            CTRLMENU_SetSecMenuItemHeight(MMIENG_MAINMENU_CTRL_ID,MMIENG_MENU_ITEM_HEIGHT,MMIENG_MENU_ITEM_HEIGHT);
            CTRLMENU_SetFontColor(MMIENG_MAINMENU_CTRL_ID,MMI_WHITE_COLOR);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);
            MMK_SetAtvCtrl(MMIENG_MAINMENU_WIN_ID, MMIENG_MAINMENU_CTRL_ID);
#ifdef CAMERA_SUPPORT    
            GUILCD_GetInfo(GUI_MAIN_LCD_ID, &lcd_info);
            if (lcd_info.lcd_height < lcd_info.lcd_width)
            {
                MMIAPICOM_EnableGrayed(win_id, MENU_DC_PARAM, ID_ENG_DC_PARAM_SENSOR_ANGLE, TRUE);
            }
#endif
            break;

        case MSG_FULL_PAINT:
            break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:

            MMK_CloseWin(MMIENG_MAINMENU_WIN_ID);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB: 
            GUIMENU_GetId(MMIENG_MAINMENU_CTRL_ID, &group_id, &menu_id);
            switch (menu_id)
            {
#ifdef AUDIOPLAYER_DEMOAPP_SUPPORT
                case ID_ENG_AUDIOPLAYER:
                    Audioplayer_View_MainWin_Enter();
                    break;
#endif
#ifdef MMI_RF_AM_SUPPORT
                case ID_ENG_RF_AM_TEST:
                    Eng_RF_AM_Test_Win_Enter();
                    break;
#endif
                case ID_ENG_AUTO_POWERONOFF://自动开关机
                    MMIAPIAUTOPOWER_OpenMainWin();
                    break;
                case ID_ENG_HELPINFO:
                    MMK_CreateWin((uint32*)MMIENG_SHOWHELP_WIN_TAB,  PNULL);
                    break;

                case ID_ENG_VERSIONINFO:
                    g_mmieng_showversion_for1 = TRUE;
                    MMK_CreateWin((uint32*)MMIENG_SHOWVERSION_WIN_TAB, PNULL);
                    break;
#ifdef WRE_SUPPORT
                case ID_ENG_WRE_VERSIONINFO:
                    MMK_CreateWin((uint32*)MMIENG_SHOW_WREVERSION_WIN_TAB, PNULL);
                    break;
#endif
                case ID_ENG_3RDPARTY_VERSIONINFO://查看第三芳软件版本号
                    MMK_CreateWin((uint32*)MMIENG_SHOW_3RDPARTY_VERSION_WIN_TAB, PNULL);
                    break;

                case ID_ENG_NETINFO:
                    MMIAPIENG_CreateShowNetWin();
                    break;

                case ID_ENG_GPRS_TEST:
                {
                    MMIAPIENG_CreateShowGprsTestWin();
                    break;
                }

                case ID_ENG_PHONEINFO:
                    MMK_CreateWin((uint32*)MMIENG_SHOWPHONE_WIN_TAB, PNULL);
                    break;

                case ID_ENG_ADC_CALIBRATE_INFO:
                    MMK_CreateWin((uint32*)MMIENG_SHOW_ADC_CALIBRATE_WIN_TAB, PNULL);
                    break;

                case ID_ENG_RESTOREENG:
                    SetLanguageToEnglish();
                    break;

                case ID_ENG_POWERKEY:
                    MMK_CreateWin((uint32*)MMIENG_POWERKEY_TAB, PNULL);
                    break;
                case ID_ENG_FLIGHT_MODE:
                    {
                        MMIENG_OpenListSelectWin((uint32)EngFlightModeOnOffWinHandleMsg);
                    }
                    break;

                case ID_ENG_PASSWORD_SET:
                    MMIENG_OpenListSelectWin((uint32)EngPasswordWinHandleMsg);
                    break;

#ifdef DSP_USB_LOG
                case ID_ENG_DSPCARD_LOG_ENABLE://xuefangjiang 20110729
                    MMK_CreateWin((uint32*)MMIENG_DSPCARD_LOG_ENABLE_TAB, PNULL);
                    break;
#endif

                case ID_ENG_SENSOR_MODE://@songbin.zeng,2006-08-08
                    MMK_CreateWin((uint32*)MMIENG_SENSOR_MODE_TAB, PNULL);
                    break;

                case ID_ENG_CMCC_TEST_FLAG://@cheney.chen MS00216980
                    MMK_CreateWin((uint32*)MMIENG_CMCC_TEST_WIN_TAB, PNULL);
                    break;

                case ID_ENG_XSR_DEBUG:
                    SetXsrResult(TRUE);
                    break;

                case ID_ENG_CAP_OUTPUT:
                    MMK_CreateWin((uint32*)MMIENG_CAPOUPUT_WIN_TAB, PNULL);
                    break;
                case ID_ENG_DV_FRAME_FRE://@songbin.zeng,2006-12-05,MS00065101
                    MMIAPIENG_CreateWinDVFrameFre();
                    break;

                case ID_ENG_VOICEPARAM_HDSTTONE:
                case ID_ENG_VOICEPARAM_CLOSE_HDSTTONE:
                    g_mmieng_voice_param = MMI_HANDHOLD_SIDETONE_GAIN;
                    MMK_CreateWin((uint32*)MMIENG_SIDETONE_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_EARPHTONE:
                case ID_ENG_VOICEPARAM_CLOSE_EARPHTONE:
                    g_mmieng_voice_param = MMI_EARPHONE_SIDETONE_GAIN;
                    MMK_CreateWin((uint32*)MMIENG_SIDETONE_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_HDSTDGT:
                    g_mmieng_voice_param = MMI_HANDHOLD_DIGITAL_GAIN;
                    MMK_CreateWin((uint32*)MMIENG_VOICEPARAM_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_HDFREDGT:
                    g_mmieng_voice_param = MMI_HANDFREE_DIGITAL_GAIN;
                    MMK_CreateWin((uint32*)MMIENG_VOICEPARAM_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_EARPHDGT:
                    g_mmieng_voice_param = MMI_EARPHONE_DIGITAL_GAIN;
                    MMK_CreateWin((uint32*)MMIENG_VOICEPARAM_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_HDSTPGA:
                    g_mmieng_voice_param = MMI_HANDHOLD_PGA_GAIN;
                    MMK_CreateWin((uint32*)MMIENG_VOICEPARAM_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_HDFREPGA:
                    g_mmieng_voice_param = MMI_HANDFREE_PGA_GAIN;
                    MMK_CreateWin((uint32*)MMIENG_VOICEPARAM_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_EARPHPGA:
                    g_mmieng_voice_param = MMI_EARPHONE_PGA_GAIN;
                    MMK_CreateWin((uint32*)MMIENG_VOICEPARAM_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_HDSTSCL:
                    g_mmieng_voice_param = MMI_HANDHOLD_DIGITAL_SCALE;
                    MMK_CreateWin((uint32*)MMIENG_VOICEPARAM_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_HDFRESCL:
                    g_mmieng_voice_param = MMI_HANDFREE_DIGITAL_SCALE;
                    MMK_CreateWin((uint32*)MMIENG_VOICEPARAM_TAB, PNULL);
                    break;

                case ID_ENG_VOICEPARAM_EARPHSCL:
                    g_mmieng_voice_param = MMI_EARPHONE_DIGITAL_SCALE;
                    MMK_CreateWin((uint32*)MMIENG_VOICEPARAM_TAB, PNULL);
                    break;

#ifdef SUBLCD_SIZE_NONE
                case ID_ENG_LCDPARAM_CON:
                    break;
#else
                case ID_ENG_LCDPARAM_CON_MAIN:

                    break;
                    
                case ID_ENG_LCDPARAM_CON_SUB:

                    break;
#endif

#ifdef SUBLCD_SIZE_NONE
                case ID_ENG_LCDPARAM_BRT:
                    Settings_BrightnessWin_Enter();
                    //MMK_CreateWin((uint32*)MMIENG_BRIGHTNESS_WIN_TAB, PNULL);
                    break;
#else 
                case ID_ENG_LCDPARAM_BRT_MAIN:
                    MMK_CreateWin((uint32*)MMIENG_BRIGHTNESS_WIN_TAB, PNULL);
                    break;

                case ID_ENG_LCDPARAM_BRT_SUB:
                    //小屏亮度和大屏是一起的
                    MMK_CreateWin((uint32*)MMIENG_BRIGHTNESS_WIN_TAB, PNULL);
                    // no sub LCD
                    break;
#endif
                case ID_ENG_DEBUGPARAM_SMSASSERT_OPEN:	// open SMS send assert
                    OpenSMSAssert();
                    break;

                case ID_ENG_DEBUGPARAM_SMSASSERT_CLOSE:	// close SMS send assert
                    CloseSMSAssert();
                    break;

                case ID_ENG_DEBUGPARAM_BAND:
                    MMK_CreateWin((uint32*)MMIENG_BANDSELECT_WIN_TAB, PNULL);
                    break;
#if 0
                case ID_ENG_DEBUGPARAM_PORT:		// port1 to data mode
                    MMIENG_OpenListSelectWin((uint32)EngDebugPortModeWinHandleMsg);
                break;
#endif	

                case ID_ENG_DEBUGPARAM_ASSERTMODE:
                    MMIENG_OpenListSelectWin((uint32)EngDebugAssertModeWinHandleMsg);
                    break;

                case ID_ENG_DEBUGPARAM_ASSERTTARGET:
                    MMK_CreateWin((uint32*)MMIENG_ASSERTTARGET_WIN_TAB, PNULL);
                    break;
                case ID_ENG_DEBUGPARAM_WCN_ASSERT:
#ifdef PLATFORM_ANTISW3
#ifndef WIN32
                    wcn_manually_assert();
#endif
#endif
                    break;
                case ID_ENG_DEBUGPARAM_MANUALASSERT_RESTART:
                    //Debug版本弹出assert信息，release版本重启
                    SCI_PASSERT(FALSE,("Debug: display assert info; Release: restart!"));   /*assert verified*/
                    break;

                case ID_ENG_DEBUGPARAM_MANUALASSERT_INFO:
                    //Debug and release都弹出assert信息
#ifndef WIN32
                    SCI_DASSERT(FALSE);/*lint !e506 */ /*assert verified*/
#endif
                    break;

                case ID_ENG_DEBUGPARAM_FORBID_PLMN:
                    MMK_CreateWin((uint32*)MMIENG_FORBIDPLMN_WIN_TAB, PNULL);
                    break;
#ifdef PLATFORM_SC6800H                    
                case ID_ENG_DEBUGPARAM_SIM1_PLMN_SELECT:
                    s_plmn_sim_index = 1;
                    MMIENG_OpenListSelectWin((uint32)EngPLMNSelectWinHandleMsg);
                    break;

                case ID_ENG_DEBUGPARAM_SIM2_PLMN_SELECT:
                    s_plmn_sim_index = 2;
                    MMIENG_OpenListSelectWin((uint32)EngPLMNSelectWinHandleMsg);
                    break;	
                    
                case ID_ENG_DEBUGPARAM_SIM3_PLMN_SELECT:
                    s_plmn_sim_index = 3;
                    MMIENG_OpenListSelectWin((uint32)EngPLMNSelectWinHandleMsg);
                    break;	
                    
                case ID_ENG_DEBUGPARAM_SIM4_PLMN_SELECT:
                    s_plmn_sim_index = 4;
                    MMIENG_OpenListSelectWin((uint32)EngPLMNSelectWinHandleMsg);
                    break;  
#endif                    
                case ID_ENG_RFPARAM_SEND:
#ifdef ENG_SUPPORT
                    MMIENG_OpenListSelectWin((uint32)EngRFSendPowerOnOffWinHandleMsg);
#endif
                    break;

                case ID_ENG_HWPARAM_NETLOST:
                    MMK_CreateWin((uint32*)MMIENG_NETLOST_WIN_TAB, PNULL);
                    break;

                case ID_ENG_HWPARAM_TCV:
                    MMK_CreateWin((uint32*)MMIENG_TCV_WIN_TAB, PNULL);
                    break;
#if 0
                case ID_ENG_HWPARAM_AFC:
                    MMK_CreateWin((uint32*)MMIENG_AFC_WIN_TAB, PNULL);
                    break;

                case ID_ENG_HWPARAM_PA:
                    MMK_CreateWin((uint32*)MMIENG_PA_WIN_TAB, PNULL);
                    break;

                case ID_ENG_HWPARAM_AGC:
                    MMK_CreateWin((uint32*)MMIENG_AGC_WIN_TAB, PNULL);
                    break;
#endif
                case ID_ENG_HWPARAM_RATE:
                    MMK_CreateWin((uint32*)MMIENG_ERRRATE_WIN_TAB, PNULL);
                    break;


                case ID_ENG_ARMLOG:
                    MMIENG_OpenListSelectWin((uint32)EngARMLOGOnOffWinHandleMsg);
                    break;

                case ID_ENG_DSPLOG:
                    MMIENG_OpenListSelectWin((uint32)EngDSPLOGSetWinHandleMsg);
                    break;        

                case ID_ENG_ARMLOG_POWER:
                    MMK_CreateWin((uint32*)MMIENG_USBLOG_WIN_TAB, PNULL);
                    break;

#ifdef CMMB_SUPPORT
                case ID_ENG_MTV_LOG_SWITCH:
                    MMK_CreateWin((uint32*)MMIENG_MTV_LOG_SWITCH_WIN_TAB, PNULL);
                    break;

                case ID_ENG_MTV_LOG_TO_BB_SWITCH:
                    MMK_CreateWin((uint32*)MMIENG_MTV_LOG_TO_BB_SWITCH_WIN_TAB, PNULL);
                    break;

                case ID_ENG_MTV_LOG_STORAGE:
                    MMK_CreateWin((uint32*)MMIENG_MTV_LOG_STORAGE_WIN_TAB, PNULL);
                    break;
#endif

                case ID_ENG_AUDIO_HANDSET_SIDETONE:
                case ID_ENG_AUDIO_HANDSET_RECEIVER:
                case ID_ENG_AUDIO_HANDSET_MIC:
                case ID_ENG_AUDIO_HEADSET_EAR:
                case ID_ENG_AUDIO_HEADSET_MIC:
                case ID_ENG_AUDIO_HANDSFREE_SPEAKER:
                case ID_ENG_AUDIO_HANDSFREE_MIC:
                case ID_ENG_AUDIO_MP4HFTP_SPEAKER:
                case ID_ENG_AUDIO_MP4HFTR_MIC:
                case ID_ENG_AUDIO_MP4HEADSET_EAR:
                case ID_ENG_AUDIO_RECORD_MIC:
                case ID_ENG_AUDIO_BT_EAR:
                case ID_ENG_AUDIO_BT_MIC:
                case ID_ENG_AUDIO_MP3_LOUDSPK_SPEAKER:
                case ID_ENG_AUDIO_MP3_EARPHONE_EAR:
                case ID_ENG_AUDIO_MP3_HEADFREE_SPEAKER:
                    //MMK_CreateWin((uint32*)MMIENG_AUDIOSET_WIN_TAB, (ADD_DATA)menu_id);
                    MMIENGWIN_OpenAudioSetWin(menu_id);
                    break;


                case ID_ENG_SLEEPMODE:
                    MMIENG_OpenListSelectWin((uint32)EngSleepModeOnOffWinHandleMsg);
                    break;

                case ID_ENG_AUTOPOWERON:
                    MMIENG_OpenListSelectWin((uint32)EngAutoPoweronOnOffWinHandleMsg);
                    break;

                //for SUPPORT_STANDBY_RECORD    
                case ID_ENG_SHOW_STANDBY_TIME:
                    MMIENG_OpenStandbyInfoWin();
                    break;
                    
                case ID_ENG_STANDBY_SET:
                    MMIENG_OpenListSelectWin((uint32)EngStandbyWinOnOffHandleMsg);
                    break;
                //end SUPPORT_STANDBY_RECORD    

                case ID_ENG_GET_FLASH_INFO:
                    MMIENG_OpenGetFlashInfoWin();
                    break;

                case ID_ENG_UPDATE_IMEI:
                    MMIENG_OpenuUpdateIMEIWin();
                    break;

#if 0
                case ID_ENG_USER_AGENT:
                    MMK_CreateWin((uint32*)MMIENG_WAPUA_WIN_TAB, PNULL);
                    break;

                case ID_ENG_UA_PROFILE:
                    MMK_CreateWin((uint32*)MMIENG_WAPUAPROFILE_WIN_TAB, PNULL);
                    break;

                case ID_ENG_UA_DEFAULT:
                    ResetWapUA();
                    break;
                    case ID_ENG_WAPMMS_SAS_TEST_MODE:
                    MMK_CreateWin((uint32*)MMIENG_WAPMMS_SAS_TEST_MODE_WIN_TAB, PNULL);
                    break;

                case ID_ENG_WAP_LOG_ON:
                    SetWapLog(TRUE);
                    MMIBRW_SetDebugMode(TRUE);
                    break;

                case ID_ENG_WAP_LOG_OFF:
                    SetWapLog(FALSE);
                    MMIBRW_SetDebugMode(FALSE);
                    break;
#endif
                case ID_ENG_GPRS:
                    MMIENG_OpenListSelectWin((uint32)EngGPRSOnOffWinHandleMsg);
                    break;

                case ID_ENG_HIDDEN_PARTITION_VISIBLE:
                    MMIENG_OpenListSelectWin((uint32)EngHiddenPartitionVisibleOnOffWinHandleMsg);
                    break;



#ifdef DCD_SUPPORT
                case ID_ENG_DCD_UPDATE_NOTE:
                    MMIENG_OpenListSelectWin((uint32)EngDCDUpdateLOGOnOffWinHandleMsg);
                    break;

                case ID_ENG_DCD_NET_SETTING:
                    MMIAPIDCD_OpenLinkListWin();
                    break;
#endif
                case ID_ENG_CALLFORWARD_QUERY:
                    MMIENG_OpenListSelectWin((uint32)EnCallForwardQueryOnOffWinHandleMsg);
                    break;                    

                case ID_ENG_LAYER1MONITOR:
                    MMK_CreateWin((uint32*)MMIENG_LAYER1MONITOR_WIN_TAB, PNULL);
                    break;

                case ID_ENG_CARDLOG:
                    MMIAPIENG_OpenCardLogWin();
                    break;
#ifdef TP_TEST_SUPPORT	                    
                case ID_ENG_TP_TEST:
                    MMK_CreateWin((uint32*)MMIENG_TP_TEST_DRAW_WIN_TAB, PNULL);									
                    break;
#endif

                case ID_ENG_IQ_MODE:
                    MMIENG_OpenIQModeWin();
                    break;

/*[START] Jerry Liang for ENG MODE powersweep 2008/07/30*/
//                case ID_ENG_POWERSWEEP_INFO:
//			MMK_CreateWin((uint32*)MMIENG_POWERSWEEPINFO_WIN_TAB,  PNULL);
//			break;//modify by mary
/*[END] Jerry Liang for ENG MODE powersweep 2008/07/30*/					

#ifdef	BLUETOOTH_SUPPORT
                case ID_ENG_BT_SSP_DEBUG_MODE:
                    EngBTopenSSPDebugWin();	
                    break;

                case ID_ENG_BT_BQB_MODE:
                    EngBTopenBQBModeWin();
                    SCI_TraceLow("Test");
                    break;
#if 0
#ifdef BT_PS_BQB_TEST
                case ID_ENG_BT_PS_BQB_TEST:
                    EngBTopenBQBPsTestWin();	
                    break;
#endif
#endif
                case ID_ENG_BT_ADDRESS:
                    MMK_CreateWin((uint32*)MMIENG_BT_ADDRESS_INFO_TAB, PNULL);
                    break;


                case ID_ENG_ENTER_NOSIGNAL_MODE:
                    EngBTopenNOSIGNALTESTModeWin();
                    break;

#ifdef  BLUETOOTH_SUPPORT_SPRD_BT
                case ID_ENG_BT_VER_INFO:
                    EngBTopenVerInfoWin();
                    break;
#endif
                case ID_ENG_ENTER_EUT_MODE:
                    //Modify by michael wang for NEWMS00159941 :add a popup msg.
                    //bug623701
#ifndef WIN32
                    //MNPHONE_SetDisablePowerSweep(TRUE);
#endif
                    if(BT_SUCCESS == MMIAPIBT_EnterEutMode())
                    {
                        MMIPUB_OpenAlertSuccessWin(TXT_ENG_ENTER_BT_EUT);
                    }
                    else
                    {
                        //MMIPUB_OpenAlertFailWin(TXT_ERROR/*TXT_COMMON_SD_FAIL*/);
                    }
                    //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);      
                    //Modify over.
                    break;
                case ID_ENG_LEAVE_EUT_MODE:
#ifndef WIN32  
                    if( BT_SUCCESS == BT_LeaveEutMode() )
                    {
                        MMIPUB_OpenAlertSuccessWin( TXT_COMPLETE );
                    }else
                    {
                        MMIPUB_OpenAlertFailWin( TXT_ERROR/*TXT_COMMON_SD_FAIL*/ );
                    }
#endif
                    break;
#ifdef BT_BLE_SUPPORT
                case ID_ENG_BLE_TEST:
                    MMIENG_OpenListSelectWin((uint32)BTBLETestWinHandleMsg);
                    break;
#endif
#ifdef ADULT_WATCH_SUPPORT
                case ID_ENG_BT_ROLE_TEST:
                    MMIENG_OpenListSelectWin((uint32)BTRoleTestWinHandleMsg);
                    break;
                case ID_ENG_BT_VISIBLITY_SET:
                    MMIENG_OpenListSelectWin((uint32)BTVisibilitySetWinHandleMsg);
                    break;
                case ID_ENG_BT_LOCAL_NAME_SET:
                    MMK_CreateWin((uint32*)MMIENG_BT_LOCAL_NAME_SET_TAB, PNULL);
                    break;
#endif
#endif
#ifdef FM_SUPPORT
//FM Start
                case ID_ENG_FM_REG_MODE:
                case ID_ENG_FM_OBSERVE_STATE:
                case ID_ENG_FM_DATA_CAPTURE:
                case ID_ENG_FM_CHANNEL_SET:
                    EngConnectivityOpenInfoWin(menu_id);
                    break;
//FM End      
#endif
//Telephony
                //case 
                //EngTelephonyOpenInfoWin
//Debug&log
                case ID_ENG_DEB_GPRS_ATTACH_SERVICE:
                case ID_ENG_DEB_THERMAL_SWITCH:
                case ID_ENG_DEB_APR_SET:
                    EngDebugAndLogOpenInfoWin(menu_id);
                    break;
//Hardware
                case ID_ENG_VIBRATE_TEST_INFO:
                case ID_ENG_CAMERA_LCD_INFO:
                case ID_ENG_CAM_POW_TEST_MODE:
                    EngHardWareOpenInfoWin(menu_id);
                    break;

#ifdef PCLINK_SUPPORT
                case ID_ENG_PCLINK_ENABLE:
                    MMK_CreateWin((uint32*)MMIENG_PCLINK_ENABLE_WIN_TAB, PNULL);
                    break;

                case ID_ENG_PCLINK_IP:
                    MMK_CreateWin((uint32*)MMIENG_PCLINK_INPUT_IPADDR_WIN_TAB, PNULL);	
                    break;

                case ID_ENG_PCLINK_DNS:
                    MMK_CreateWin((uint32*)MMIENG_PCLINK_INPUT_DNS_WIN_TAB, PNULL);	
                    break;

                case ID_ENG_PCLINK_GATEWAY:
                    MMK_CreateWin((uint32*)MMIENG_PCLINK_INPUT_DEFGW_WIN_TAB, PNULL);	
                    break;

                case ID_ENG_PCLINK_SUBNET_MAS:
                    MMK_CreateWin((uint32*)MMIENG_PCLINK_INPUT_SNMASK_WIN_TAB, PNULL);	
                    break;

                case ID_ENG_PCLINK_TEST_ADDRESS:
                    MMK_CreateWin((uint32*)MMIENG_PCLINK_INPUT_TEST_ADDRESS_WIN_TAB, PNULL);	
                    break;

                case ID_ENG_PCLINK_TEST_PING:
                    MMIENG_StartPingTest();
                    break;
#endif

#ifdef  CAMERA_SUPPORT
                case ID_ENG_DC_PARAM_SCREEN_SIZE:
                    if(IsDCAllowedToBeOpened())
                    {
                        MMK_CreateWin((uint32*)MMIENG_DC_PARAM_SCREEN_SIZE_WIN_TAB, PNULL);
                    }
                    break;

                case ID_ENG_DC_PARAM_SCREEN_SWITCH:
                    if(IsDCAllowedToBeOpened())
                    {
                        MMK_CreateWin((uint32*)MMIENG_DC_PARAM_SCREEN_SWITCH_WIN_TAB, PNULL);
                    }
                    break;

                case ID_ENG_DC_PARAM_PREV_SET:
                    if(IsDCAllowedToBeOpened())
                    {
                        MMK_CreateWin((uint32*)MMIENG_DC_PARAM_PREV_SET_WIN_TAB, PNULL);
                    }
                    break;
                case ID_ENG_DC_PARAM_SENSOR_ANGLE:
                    if(IsDCAllowedToBeOpened())
                    {
                        int32 sensor_number = 0;

                        sensor_number = MMIAPIDC_GetDCModuleSensorNumber();
                        if (sensor_number < MAX_MMI_SENSOR_NUM)
                        {
                            MMK_CreateWin((uint32*)MMIENG_DC_PARAM_SENSOR_ANGLE_WIN_TAB, 0);
                        }
                        else
                        {
                            MMK_CreateWin((uint32*)MMIENG_DC_PARAM_SENSOR_ID_WIN_TAB, PNULL);
                        }
                    }
                    break;
#endif               
                case ID_ENG_AUTO_ANSWER_CALL_SET:
                    MMK_CreateWin((uint32*)MMIENG_AUTO_ANSWER_CALL_WIN_TAB, PNULL);
                    break;

                case ID_ENG_UA_AGENT_SET:
                    MMIAPIENG_OpenUserAgentWin();
                    break;

                case ID_ENG_UA_PROFILE_SET:
                    MMIAPIENG_OpenUaProfileWin();
                    break;
                case ID_ENG_PIM_DEBUG_SET:
                    MMIAPIPIM_OpenDebugMenuWin();
                    break;
#ifdef MMI_SMS_RETRY_SUPPORT
                case ID_ENG_SMS_RETRY:
                    MMK_CreateWin((uint32*)MMIENG_SMS_RETRY_WIN_TAB, PNULL);
                    break;
#endif
#if (defined(DM_SUPPORT) || defined(DM_ONLY_SELFREG_SUPPORT))
                case ID_ENG_DM_PRAR_SET:
                    MMIAPIDM_OpenDebugMenuWin();
                    break;
#endif
#ifdef JAVA_SUPPORT_IA
                case ID_ENG_JAVA_AUTO_TCK_MODE:
                    MMIENG_OpenListSelectWin((uint32)EngJAVAAutoTCKOnOffWinHandleMsg);
                    break;
/*
                case ID_ENG_JAVA_AUTO_TCK_MODE_ON:
                    SetJavaAutoTckOn();
                    break;

                case ID_ENG_JAVA_AUTO_TCK_MODE_OFF:
                    SetJavaAutoTckOff();
                    break;
*/
#endif
#ifdef PRINTSCREEN_SUPPORT
                case ID_ENG_PRINTSCREEN:
                    MMIENG_OpenListSelectWin((uint32)EngPrintScreenWinHandleMsg);
                    break;
#endif                
                case ID_ENG_CALL_BARRING://MS00186848
                    MMK_CreateWin((uint32*)MMIENG_CALL_BARRING_WIN_TAB, PNULL);
                    break;

                case ID_ENG_SET_ANGLE:
                    MMK_CreateWin((uint32*)MMIENG_SET_ANGLE_WIN_TAB, PNULL);
                    break;

                case ID_ENG_NVCOUNT:
                    MMIENGWIN_OpenNVCountingWin();
                    break;
                case ID_ENG_BROWSER_LOG:
                    MMIAPIENG_OpenBrowserLogWin();
                    break;

                case ID_ENG_BROWSER_Change_URL:
                    MMIAPIENG_OpenBrowserChangeURLWin();
                    break;

                case ID_ENG_SCREEN_LOG:
                    MMIAPIENG_OpenScreenLogWin();
                    break;

#ifdef MMI_AUTOTEST_SUPPORT
                case ID_ENG_AT_ALL_SET:// @CR247145 renwei add
                case ID_ENG_AT_IMAGE_ID:
                case ID_ENG_AT_TEXT:
                case ID_ENG_AT_WIN_MSG:
                case ID_ENG_AT_TP:
                case ID_ENG_AT_KEY:
                case ID_ENG_AT_MSG:
                case ID_ENG_AT_SOFTKEY:
                case ID_ENG_AT_TITLE:
                case ID_ENG_AT_POINTER_STATE:
                case ID_ENG_AT_BACKLIGHT:
                    at_set_id = menu_id - ID_ENG_AT_ALL_SET;// @CR247145 renwei modify
                    MMIAPIENG_OpenAutoTestSetWin(at_set_id);
                    break;
#endif
#ifdef MMI_WIFI_SUPPORT
                case ID_ENG_WLAN_PERFORMANCE_MODE:
                    MMIAPIENG_OpenWlanPerformanceModeWin();
                    break;
                case ID_ENG_WLAN_IPERF:
                    MMIAPIENG_OpenWifiIperfWin();
                    break;
                case ID_ENG_WLAN_RADIO_FREQUENCY:
                    MMIAPIENG_OpenWlanRadioFrequencyWin();
                    break;
                case ID_ENG_CON_WIFI_TX:
                case ID_ENG_CON_WIFI_RX:
                case ID_ENG_CON_WIFI_REGWR:
                case ID_ENG_CON_WIFI_SAVEMODE:
                case ID_ENG_CON_WIFI_PACKETSWITCHMODE:
                case ID_ENG_CON_WIFI_PACKETITEMMODE:
                    EngWifiInfoOpenWin(menu_id);
                    break;
                case ID_ENG_CON_WIFI_FLIGHTMODE:
                    MMIENG_OpenListSelectWin((uint32)EngFlightModeWiFiModeOnOffWinHandleMsg);
                    break;
#endif
#ifdef MMI_ENG_WIFI_SUPPORT
                case ID_ENG_WIFI_LOCATION_SCAN:
                {
#ifdef UAL_WIFI_SUPPORT
                    MMIAPIENG_OpenWifiScanWin(s_wifi_scan_win_show_button);
#endif
                    break;
                }
#endif
#if (defined(GPS_SUPPORT) && defined(MMI_WIFI_SUPPORT))
                case ID_ENG_GPS_OTA:
                    MMIENG_OpenGPSOtaTestWin();
                    break;
#endif
#ifdef GPS_SUPPORT
#ifndef MMI_RF_AM_SUPPORT
                case ID_ENG_GPS_TEST:
                    MMIENG_OpenGPSTestWin();
                    break;
#endif
#endif
#ifdef MMI_DUALMODE_ENABLE
                case ID_ENG_NETWORK_TYPE:
                    MMIAPISET_OpenNetWorkTypeWin();
                    break;
#endif
#ifdef MMISRV_AUDIO_TEST_SUPPORT            
                case ID_ENG_AUDIO_SELF_TEST:
                    MMK_CreateWin((uint32*)MMIENG_AUDIO_SELF_TEST_TAB, PNULL);
                    //MMIENG_OpenListSelectWin((uint32 *)EngAudioSelfTestHandleMsg);
                    break;
#endif                
             
                case ID_ENG_MONKEY_TEST_SET:
                    //MMK_CreateWin((uint32*)MMIENG_MONKEY_TEST_SET_TAB, PNULL);
                    MMIENG_OpenMonkeyTestSettingWin();
                    break;

                case ID_ENG_SCREN_ALWAYS_ON:
                    MMK_CreateWin((uint32*)MMIENG_SCREEN_ALWAYS_ON_WIN_TAB, PNULL);
                    break;
                case ID_ENG_TEL_SIM_TRACE_ALL:
                case ID_ENG_TEL_NETWORK_MODE_ALL:
                case ID_ENG_BAND_SELECT:
                case ID_ENG_TEL_NETINFO:
                case ID_ENG_NETINFO_STATISTICS:
                case ID_ENG_TEL_VOLTE_SETTINGS_ALL:
                case ID_ENG_TEL_VOLTE_IMS_SET_ALL:
                case ID_ENG_TEL_PLMN_WHITE_LIST_ALL:
#ifdef MMI_VOLTE_SUPPORT
                case ID_ENG_TEL_VOLTE_ONOFF:
                case ID_ENG_TEL_IMS_PDN:
#endif
                case ID_ENG_TEL_CSFBGSM_DELAY:
                case ID_ENG_RESTORE_FACTORY_RESET:
                case ID_ENG_EDIT_SMS_CENTER_NUMBER:
                case ID_ENG_FAST_DORMANCY:
                case ID_ENG_QOS_SWTICH:
                case ID_ENG_TEL_GPRS_POWER_SAVE_MODE:
                {
                    EngTelephonyOpenInfoWin(menu_id);
                    break;
                }
                case ID_ENG_ADD_CONTACT:
                    MMIENG_AddContactTest();
                    break;
#ifdef  CAMERA_SUPPORT
                case ID_ENG_TEST_CAMERA:
                    UITestCameraEnter();
                    break;
                case ID_ENG_TEST_VIDEO:
                    UITestVideoEnter();
                    break;
#endif
                case ID_ENG_MEM_PRESS_TEST:
                {
                    MMK_CreateWin((uint32*)MMIENG_MEM_PRESS_TEST_ENTER_WIN_TAB,	PNULL);
                    break;
                }
                case ID_ENG_LOG_SWITCH_AP:
                case ID_ENG_LOG_SWITCH_MODEM:
                case ID_ENG_LOG_SWITCH_WCN:
                case ID_ENG_LOG_SWITCH_GNSS:
                    MMIENG_OpenSwitchLogWin(menu_id);
                    break;
                case ID_ENG_LOG_TOPC_AP:
                case ID_ENG_LOG_TOPC_MODEM:
                case ID_ENG_LOG_TOPC_WCN:
                    MMIENG_OpenLogToPCWin(menu_id);
                    break;
                case ID_ENG_LOG_UART_LOG:
                    MMIENG_OpenUartLogWin();
                    break;
                case ID_ENG_LOG_CLEAR_ALL:
                    {
                        MMI_STRING_T txt_tip = {0};
                        WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};
#ifdef PLATFORM_ANTISW3
#ifndef WIN32
                        if(0 == ulog_clearlog())
                        {
                            MMI_GetLabelTextByLang(TXT_SUCCESS, &txt_tip);
                            WatchCOM_NoteWin_1Line_Enter(MMIENG_LOG_NOTE_WIN_ID, &txt_tip,0,softkey,PNULL);
                        }
#if 0 //bug1585522
                        else
                        {
                            MMI_GetLabelTextByLang(TXT_ERROR, &txt_tip);
                            WatchCOM_NoteWin_1Line_Enter(MMIENG_LOG_NOTE_WIN_ID, &txt_tip,0,softkey,PNULL);
                        }
#endif
#endif
#endif
                    }
                    break;
                case ID_ENG_MP3_PLAY_TEST:
                    MMIEng_OpenPlayMp3Win();
                    break;
                case ID_ENG_MEM_TESTER:
                    MMIENG_OpenMemTeserWin();
                    break;
                case ID_ENG_REGISTER_PS_SERVICE:
                    MMIENG_OpenRegisterPsServiceWin();
                    break;
                case ID_ENG_LIGHT_SLEEP_TEST:
                    MMIEng_OpenLightSleepWin();//bug1496314
                    break;
                case ID_ENG_STK_TEST:
                    MMIMAINMENU_StartSTK(PNULL,PNULL);
                break;
                case ID_ENG_MEMCHECK_TEST:
                    MMIENG_OpenListSelectWin((uint32)EngMemCheckWinHandleMsg);
                break;
#ifdef SENSORHUB_SUPPORT
#ifndef WIN32
                case ID_ENG_SENSOR_VERSION:
                    MMIENG_OpenSensorVersionWin();
                    break;
                case ID_ENG_SENSOR_FIRMWARE_DL:
                    MMIENG_OpenSensorFirmwareDLWin();
                    break;
                case ID_ENG_SENSOR_PRODUCTTEST_DL:
                    MMIENG_OpenSensorProductTestWin();
                    break;
                case ID_ENG_SENSOR_HEARTRATE_ID:
                    MMIENG_OpenSensorHeartRateIDWin();
                    break;
                case ID_ENG_SENSOR_ACC_ID:
                    MMIENG_OpenSensorAccIDWin();
                    break;
                case ID_ENG_SENSOR_ACC_CALIBRATION:
                    MMIENG_OpenSensorAccCalibrationWin();
                    break;
                case ID_ENG_SENSOR_BLE_BROADCAST:
                    MMIENG_OpenSensorBleBroadcastWin();
                    break;
#endif
#endif
#ifdef ADULT_WATCH_SUPPORT
            case ID_ENG_OLD_MAINMENU_ENTER:
            {
                MMIAPISET_SetMainmenuStyle(MMIMAINMENU_MATRIX_E);
                MMIAPIMENU_CreatMainMenu();
                break;
            }
#endif
            case ID_ENG_PHONE_START_USB_PORT:
            {
                MMK_CreateWin((uint32*)MMIENG_AUTO_USB_PORT_WIN_TAB, PNULL);
                break;
            }
#ifdef ESIM_SUPPORT_HB
            case ID_ESIM_INFORMATION_ENTER:
            {
                MMI_Esim_HB_Information_Enter();
                break;
            }
#endif

#ifdef TOUCH_PANEL_SUPPORT
            case ID_ENG_TEST_PRESS_TEST:
            {
                MMK_CreateWin((uint32*)MMIENG_LIST_PRESS_TEST_ENTER_WIN_TAB,PNULL);
                break;
            }
#endif
#ifdef ADULT_WATCH_SUPPORT
            case ID_ENG_CALL_FORWARD_VIEW_TEST:
                MMIENG_OpenListSelectWin((uint32)MMIEng_CallForwardWinHandleMsg);
                break;
            case ID_ENG_BBM_VIEW_TEST:
                MMIENG_OpenListSelectWin((uint32)MMIEng_BBMViewWinHandleMsg);
                break;
#endif
            case ID_ENG_CSCON_STATUS_INFO: // 显示RAT窗口信息
                MMK_CreateWin((uint32*)MMIENG_SHOWCSCON_WIN_TAB,PNULL);
                break;
#ifdef DATA_ROAMING_SUPPORT 
            case ID_ENG_GPRS_ROAMING_MODE_SET: // 显示RAT窗口信息
                MMK_CreateWin((uint32*)MMIENG_DATA_ROAMING_WIN_TAB,PNULL);
                break;
#endif

            case ID_ENG_FLOWRETE_INFO:
                MMK_CreateWin((uint32*)MMIENG_SHOW_FLOWRATE_WIN_TAB, PNULL);
                break;

            case ID_ENG_IMS_NV_PARAM_ID:
                MMIAPIENG_OpenIMS_NV_PARAM_ShowWin();
                break;

            case ID_ENG_SET_LTE_OPERATION_MODE:
                MMK_CreateWin((uint32*)MMIENG_SET_LTE_OPERATION_MODE_WIN_TAB, PNULL);
                break;

            case ID_ENG_FORBID_ALL_DATA:
                MMK_CreateWin((uint32*)MMIENG_FORBID_ALLGPRS_DATA_WIN_TAB, PNULL);
                break;

#if defined ENG_BAND_FREQ_SUPPORT
            case ID_ENG_BAND_FREQ_SET:
                BandFreqSIMSelectWin();
                break;
#endif
            default:
                    break;
            }
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}

/********************************************************************************
 NAME:			SetLanguageToEnglish
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
LOCAL void SetLanguageToEnglish(void)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    MMIAPISET_SetLanguageType(MMISET_LANGUAGE_ENGLISH);
    text1.wstr_ptr = s_language_wstr;/*lint !e605*/
    text1.wstr_len = MMIAPICOM_Wstrlen(s_language_wstr);
    text2.wstr_ptr = s_at_wstr;/*lint !e605*/
    text2.wstr_len = MMIAPICOM_Wstrlen(s_at_wstr);
    text3.wstr_ptr = s_englist_wstr;/*lint !e605*/
    text3.wstr_len = MMIAPICOM_Wstrlen(s_englist_wstr);
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			OpenSMSAssert
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.26
********************************************************************************/
LOCAL void OpenSMSAssert(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0};
         
#ifndef _WIN32    
    BOOLEAN is_success = FALSE;
    uint32 i = MN_DUAL_SYS_1;
#endif

	text1.wstr_ptr = s_sms_send_wstr;/*lint !e605*/ //(uint8*)"SMS send";
	text1.wstr_len = MMIAPICOM_Wstrlen(s_sms_send_wstr);
	text2.wstr_ptr = s_assert_wstr;/*lint !e605*/ //(uint8*)"assert";
	text2.wstr_len = MMIAPICOM_Wstrlen(s_assert_wstr);


	#ifndef _WIN32
         for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
         {
             if(MN_RETURN_SUCCESS == MNSMS_SetSmsAssertFuncEx((MN_DUAL_SYS_E)i, 1))
             {
                is_success = TRUE;
                break;
             }
         }
         
	if (is_success)
	{
		text3.wstr_ptr = s_on_wstr;     /*lint !e605*/  // (uint8*)"ON";
		text3.wstr_len = MMIAPICOM_Wstrlen(s_on_wstr);
	}
	else
	{
		text3.wstr_ptr = s_on_failure_wstr;     /*lint !e605*/  //(uint8*)"ON failure";
		text3.wstr_len = MMIAPICOM_Wstrlen(s_on_failure_wstr);
	}
	#else
	text3.wstr_ptr = s_on_wstr;/*lint !e605*/ //(uint8*)"ON";
	text3.wstr_len = MMIAPICOM_Wstrlen(s_on_wstr);
	#endif
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			CloseSMSAssert
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.26
********************************************************************************/
LOCAL void CloseSMSAssert(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
#ifndef _WIN32     
    BOOLEAN is_success = FALSE;
    uint32 i = MN_DUAL_SYS_1;
#endif

	text1.wstr_ptr = s_sms_send_wstr;/*lint !e605*/ //(uint8*)"SMS send";
	text1.wstr_len = 8;
	text2.wstr_ptr = s_assert_wstr;/*lint !e605*/ //(uint8*)"assert";
	text2.wstr_len = 6;
    
	#ifndef _WIN32
         for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
         {
             if( MN_RETURN_SUCCESS == MNSMS_SetSmsAssertFuncEx((MN_DUAL_SYS_E)i, 1) )
             {
                is_success = TRUE;
                break;
             }
         }
         
	if (is_success)
	{
		text3.wstr_ptr = s_off_wstr;    /*lint !e605*/  //(uint8*)"OFF";
		text3.wstr_len = 3;
	}
	else
	{
		text3.wstr_ptr = s_on_failure_wstr;     /*lint !e605*/  //(uint8*)"OFF failure";
		text3.wstr_len = 11;
	}
	#else
	text3.wstr_ptr = s_off_wstr;    /*lint !e605*/ //(uint8*)"OFF";
	text3.wstr_len = 3;
	#endif
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}


/********************************************************************************
 NAME:			SetPortClose
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.26
********************************************************************************/
LOCAL void SetAssertToDebug(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = s_restart_to_wstr;/*lint !e605*/ //(uint8*)"Restart to";
	text1.wstr_len = MMIAPICOM_Wstrlen(s_restart_to_wstr);
	text2.wstr_ptr = s_set_assert_wstr;/*lint !e605*/ //(uint8*)"set assert";
	text2.wstr_len = MMIAPICOM_Wstrlen(s_set_assert_wstr);
	text3.wstr_ptr = s_debug_mode_wstr;/*lint !e605*/ //(uint8*)"DEBUG mode";
	text3.wstr_len = MMIAPICOM_Wstrlen(s_debug_mode_wstr);
	#ifndef _WIN32
	SCI_SetAssertMode(SCI_TRUE); 	//debug mode
	#endif
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			SetAssertToRelease
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.26
********************************************************************************/
LOCAL void SetAssertToRelease(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = s_restart_to_wstr;/*lint !e605*/ //(uint8*)"Restart to";
	text1.wstr_len = MMIAPICOM_Wstrlen(s_restart_to_wstr);
	text2.wstr_ptr = s_set_assert_wstr;/*lint !e605*/ //(uint8*)"set assert";
	text2.wstr_len = MMIAPICOM_Wstrlen(s_set_assert_wstr);
	text3.wstr_ptr = s_release_mode_wstr;/*lint !e605*/ //(uint8*)"RELEASE mode";
	text3.wstr_len = MMIAPICOM_Wstrlen(s_release_mode_wstr);
	#ifndef _WIN32
	SCI_SetAssertMode(SCI_FALSE); 	//release mode
	#endif
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}

#if 0

/********************************************************************************
 NAME:			SetMaxPowerOn
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
LOCAL void SetMaxPowerOn(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = s_send_wstr;//(uint8*)"Send";
	text1.wstr_len = MMIAPICOM_Wstrlen(s_send_wstr);
	text2.wstr_ptr = s_max_power_wstr;//(uint8*)"max power";
	text2.wstr_len = MMIAPICOM_Wstrlen(s_max_power_wstr);
	text3.wstr_ptr = s_on_wstr;//(uint8*)"on";
	text3.wstr_len = MMIAPICOM_Wstrlen(s_on_wstr);
	#ifndef _WIN32
//	L1API_ApplyMaxPower();
	#endif
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}



/********************************************************************************
 NAME:			SetMaxPowerOff
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
LOCAL void SetMaxPowerOff(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = s_restart_wstr;//(uint8*)"Restart";
	text1.wstr_len = MMIAPICOM_Wstrlen(s_restart_wstr);
	text2.wstr_ptr = s_can_close_wstr;//(uint8*)"can close";
	text2.wstr_len = MMIAPICOM_Wstrlen(s_can_close_wstr);
	text3.wstr_ptr = s_max_power_wstr;//(uint8*)"max power";
	text3.wstr_len = MMIAPICOM_Wstrlen(s_max_power_wstr);
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			MMIAPIENG_SetMaxPowerOff
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
    


PUBLIC BOOLEAN MMIAPIENG_SetMaxPowerOff(void)
{
MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = s_send_wstr;//(uint8*)"Send";
	text1.wstr_len = MMIAPICOM_Wstrlen(s_send_wstr);
	text2.wstr_ptr = s_max_power_wstr;//(uint8*)"max power";
	text2.wstr_len = MMIAPICOM_Wstrlen(s_max_power_wstr);
	text3.wstr_ptr = s_on_wstr;//(uint8*)"on";
	text3.wstr_len = MMIAPICOM_Wstrlen(s_on_wstr);
	#ifndef _WIN32
//	L1API_ApplyMaxPower();
	#endif
	MMIEng_SetResultWinText(text1, text2, text3);
	return MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_NOAUTOCLOSE_WIN_TAB, PNULL);
}


/********************************************************************************
 NAME:			SetWapLog
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/

LOCAL void SetWapLog(BOOLEAN mode)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = s_set_wap_log_wstr;//(uint8*)"Set Wap Log";
	text1.wstr_len = MMIAPICOM_Wstrlen(s_set_wap_log_wstr);
	text2.wstr_ptr = s_to_wstr;//(uint8*)"to";
	text2.wstr_len = MMIAPICOM_Wstrlen(s_to_wstr);
	if(mode)
	{
		text3.wstr_ptr = s_on_wstr;//(uint8*)"on";
		text3.wstr_len = MMIAPICOM_Wstrlen(s_on_wstr);
		MMIBRW_SetDebugMode(TRUE);
	}
	else
	{
		text3.wstr_ptr = s_off_wstr;//(uint8*)"off";
		text3.wstr_len = MMIAPICOM_Wstrlen(s_off_wstr);
		MMIBRW_SetDebugMode(FALSE);
	}
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			ResetWapUA
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL void ResetWapUA(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = s_set_wap_ua_wstr;//(uint8*)"Set Wap UA";
	text1.wstr_len = MMIAPICOM_Wstrlen(s_set_wap_ua_wstr);
	text2.wstr_ptr = s_to_wstr;//(uint8*)"to";
	text2.wstr_len = MMIAPICOM_Wstrlen(s_to_wstr);
	text3.wstr_ptr = s_default_wstr;//(uint8*)"default";
	text3.wstr_len = MMIAPICOM_Wstrlen(s_default_wstr);
	MMIBRW_SetUAToDefault();
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}
#endif

/********************************************************************************
 NAME:			SetXsrResult
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		zhuowang
 DATE:			2010.10.03
********************************************************************************/
LOCAL void SetXsrResult(BOOLEAN is_on)
{
#ifdef  NANDBOOT_SUPPORT
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    text1.wstr_ptr = s_set_xsr_result_log_wstr; /*lint !e605*/
    text1.wstr_len = MMIAPICOM_Wstrlen(text1.wstr_ptr);
    text2.wstr_ptr = PNULL;
    text2.wstr_len = 0;
    if(is_on)
    {
        text3.wstr_ptr = PNULL;
    }
    else
    {
        text3.wstr_ptr = s_off_wstr; /*lint !e605*/
    }
    text3.wstr_len = MMIAPICOM_Wstrlen(s_off_wstr);	

    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
#ifndef WIN32
    MMIENG_PrintFtlInfo();
#endif
#endif	
}
#ifndef WIN32
#ifdef NANDBOOT_SUPPORT
/*****************************************************************************/
//     Description : 打印块信息功能
//    Global resource dependence : 
//  Author:   jinju.ma
//    Note:
/*****************************************************************************/
LOCAL void MMIENG_PrintFtlInfo(void)
{
	SCI_FTL_MAP_INFO _mapInfo = {0};
	uint32 bytesReturned = 0;
	uint32 i = 0;
	if(SCI_FTL_ERROR_NONE == SCI_FTL_IOCtl((void*) 0, SCI_FTL_Get_MapInfo, 0, 0,&_mapInfo, sizeof(SCI_FTL_MAP_INFO), &bytesReturned))
	{
		SCI_TRACE_LOW("FTL: Bad block num: %d", _mapInfo.mapNum);
		for(i = 0; i < _mapInfo.mapNum; i++)
		{
			SCI_TRACE_LOW("FTL: %d -> %d",_mapInfo.map[i].badBlk,_mapInfo.map[i].rpBlk);
		}
	}
}
#endif
#endif
/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallForwardQuery(void)
{
    BOOLEAN             is_on = FALSE;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_CALLFORWARD_QUERY_ID, &is_on, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
#if 0
#ifdef TDPS_UEIT_ARM_L1SIM
        is_on = FALSE;
#else
        is_on = TRUE;
#endif
#endif
        is_on = TRUE;//for bug1679904
        MMINV_WRITE(MMIENG_NV_CALLFORWARD_QUERY_ID, &is_on);
    }
    return is_on;
}

/********************************************************************************
 NAME:			MMIAPIENG_SetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
LOCAL void MMIAPIENG_SetCallForwardQuery(
                                          BOOLEAN   is_on
                                          )
{
    MMINV_WRITE(MMIENG_NV_CALLFORWARD_QUERY_ID, &is_on);
}

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetAutoAnswerCallSet(void)
{
    BOOLEAN             is_on = FALSE;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_AUTO_ANSWER_CALL_SET_ID, &is_on, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        is_on = FALSE;
        MMINV_WRITE(MMIENG_NV_AUTO_ANSWER_CALL_SET_ID, &is_on);
    }
    
    return is_on;
}

/********************************************************************************
 NAME:			MMIAPIENG_SetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC void MMIAPIENG_SetAutoAnswerCallSet(
                                          BOOLEAN   is_on
                                          )
{
    MMINV_WRITE(MMIENG_NV_AUTO_ANSWER_CALL_SET_ID, &is_on);
//	MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);       
}


/********************************************************************************
 NAME:			MMIAPIENG_GetIsCardLogOn
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsCardLogOn(void)
{
    BOOLEAN             is_on = FALSE;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_IS_CARDLOG_ON, &is_on, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        is_on = FALSE;
        MMINV_WRITE(MMIENG_NV_IS_CARDLOG_ON, &is_on);
    }
    return is_on;
}

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIsBrowserLogOn 
//	Global resource dependence : none
//  Author: sally.he
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsBrowserLogOn(void)
{
#ifdef BROWSER_SUPPORT_DORADO  
    return (g_is_http_log_file_save && g_is_ssl_log_save);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description : MMIAPIENG_SetIsBrowserLogOn 
//	Global resource dependence : none
//  Author: sally.he
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIsBrowserLogOn(BOOLEAN is_browser_log_on)
{
#ifdef BROWSER_SUPPORT_DORADO
    g_is_http_log_file_save = is_browser_log_on;
    g_is_ssl_log_save = is_browser_log_on;
#endif
}

/*****************************************************************************/
// 	Description : set user wre url change 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIsBrowserChangeURL(BOOLEAN is_URL_change)
{
#ifdef BROWSER_SUPPORT_DORADO
    s_browser_change_url = is_URL_change;
#endif
}

/*****************************************************************************/
// 	Description : get user wre url change 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsBrowserChangeURL(void)
{
    return s_browser_change_url;
}

#ifdef MMI_WIFI_SUPPORT
/********************************************************************************
 NAME:          MMIAPIENG_SetIsFlightModeWifi
 DESCRIPTION:   set flight mode wifi
 PARAM IN:      flight mode of wifi
 PARAM OUT:
 AUTHOR:        zhenpeng.yao
 DATE:          2020.11.25
********************************************************************************/
LOCAL void MMIAPIENG_SetIsFlightModeWifi(BOOLEAN is_flight_mode_wifi)
{
    SCI_TRACE_LOW("MMIAPIENG_SetIsFlightModeWifi s_flight_mode_wifi = %d",s_flight_mode_wifi);
    s_flight_mode_wifi = is_flight_mode_wifi;
}

/********************************************************************************
 NAME:          MMIAPIENG_GetIsFlightModeWifi
 DESCRIPTION:   get flight mode wifi
 PARAM IN:
 PARAM OUT:     flight mode of wifi
 AUTHOR:        zhenpeng.yao
 DATE:          2020.11.25
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsFlightModeWifi(void)
{
    return s_flight_mode_wifi;
}
#endif

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIsScreenLogOn 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMIENG_SCREEN_DEBUG_T MMIAPIENG_GetIsScreenLogOn(void)
{
    return s_screen_debug_type;
}

/*****************************************************************************/
// 	Description : MMIAPIENG_SetIsScreenLogOn 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIsScreenLogOn(MMIENG_SCREEN_DEBUG_T type)
{
    if ( type < MMIENG_SCREEN_DEBUG_MAX )
    {
        s_screen_debug_type = type;
    }   
}

/*****************************************************************************/
// 	Description : MMIAPIENG_SetScreenTag 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
void MMIAPIENG_SetScreenTag( const char* tag, MMIENG_SCREEN_LOG_E type, uint32 ticks )
{
    if ( type < MMIENG_SCREEN_LOG_MAX )
    {
        s_screenlog_tag[type] = tag;
        s_screenlog_time[type] = ticks;
    }
}

/*****************************************************************************/
// 	Description : MMIAPIENG_PrintScreenLog 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
void MMIAPIENG_PrintScreenLog( void )
{
    if ( s_screen_debug_type
        && MMITHEME_IsUpdateRectDirty() )
    {
        uint8 temp_str[SCREEN_STR_LEN + 1] = {0};
        wchar temp_wstr[SCREEN_STR_LEN + 1] = {0};
        uint32 test_time[MMIENG_SCREEN_LOG_MAX] = {0};
        uint16 str_len = 0;
        GUI_POINT_T point = {0};
        GUI_RECT_T  rect = MMITHEME_GetFullScreenRect();
        uint32 i = 0;
        MMI_STRING_T temp_string = {0};
        GUISTR_STATE_T temp_state = GUISTR_STATE_INVERSE | GUISTR_STATE_SINGLE_LINE;
        GUISTR_STYLE_T temp_style = {0};
        GUI_LCD_DEV_INFO lcd_dev = {0};
        IGUICTRL_T *iguictrl_statusbar = PNULL;

        iguictrl_statusbar = MMK_GetCtrlPtr(MMITHEME_GetStatusBarCtrlId());
        if(iguictrl_statusbar)
        {
            GUICTRL_GetLcdDevInfo(iguictrl_statusbar,&lcd_dev);
        }
#ifdef UI_MULTILAYER_SUPPORT	
        if(!UILAYER_IsLayerActive(&lcd_dev))
        {
            lcd_dev.block_id = 0;
            lcd_dev.lcd_id = 0;
        }

        if ( UILAYER_IsLayerActive(&s_debug_layer_handle) )
        {
            lcd_dev = s_debug_layer_handle;
        }
#endif
        SCI_MEMCPY(test_time, s_screenlog_time, sizeof(s_screenlog_time));

        temp_string.wstr_ptr = temp_wstr;
        temp_style.font = SCREEN_LOG_FONT;
        temp_style.font_color = 0xFFFF;
        
        for ( i = 0; i < MMIENG_SCREEN_LOG_MAX; i++ )
        {
            rect.top = point.y;
            point.y += SCREEN_LOG_FONT;
            rect.bottom = point.y;

            if ( NULL != s_screenlog_tag[i] )
            {
                _snprintf( (char*)temp_str, SCREEN_STR_LEN, "%10s:%10d     ", s_screenlog_tag[i], s_screenlog_time[i] );
                str_len = strlen( (char*)temp_str);
                MMI_STRNTOWSTR( temp_wstr, SCREEN_STR_LEN, temp_str, str_len, str_len );
                temp_string.wstr_len = str_len;
                GUISTR_DrawTextToLCDInRect(&lcd_dev, &rect, &rect, &temp_string, &temp_style, temp_state, -1 );
            }
        }
    }
}


/*****************************************************************************/
// 	Description : display debug rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
void MMIAPIENG_DisplayDebugRect(
                                GUI_RECT_T update_rect     //in:
                                )
{     
#ifdef UI_MULTILAYER_SUPPORT	
    if ( MMIENG_SCREEN_DEBUG_RECT == MMIAPIENG_GetIsScreenLogOn() )
    {
        if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_debug_layer_handle))
        {
            UILAYER_CREATE_T  create_info = {0};
            uint16            layer_width = 0;
            uint16            layer_height = 0;

            GUILCD_GetLogicWidthHeight( GUI_MAIN_LCD_ID, &layer_width, &layer_height);
            
            create_info.width  = layer_width;
            create_info.height = layer_height;
            create_info.is_static_layer = TRUE;
            create_info.is_bg_layer = TRUE;
            create_info.format = UILAYER_MEM_FORMAT_IMMUTABLE;

            if ( UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer( &create_info, &s_debug_layer_handle ) )
            {
                UILAYER_APPEND_BLT_T append_layer = {0};

                append_layer.lcd_dev_info = s_debug_layer_handle;
                append_layer.layer_level = UILAYER_LEVEL_HIGH;
                UILAYER_AppendBltLayer(&append_layer);

                UILAYER_SetLayerColorKey(&s_debug_layer_handle, TRUE, 0);
                UILAYER_SetLayerAlpha(&s_debug_layer_handle, 0x60);
                UILAYER_Clear(&s_debug_layer_handle);
            }
        }

        if ( UILAYER_IsLayerActive(&s_debug_layer_handle) )
        {
            LCD_FillRect( &s_debug_layer_handle, update_rect, MMI_RED_COLOR );
        }
    }
    else
    {
        UILAYER_RELEASELAYER( &s_debug_layer_handle );
    }

#endif    
}

/*****************************************************************************/
// 	Description : clear debug rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
void MMIAPIENG_ClearDebugRect( void )
{    
#ifdef UI_MULTILAYER_SUPPORT	
    UILAYER_Clear(&s_debug_layer_handle);
#endif
}

/*****************************************************************************/
// 	Description : is debug layer
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIENG_IsDebugLayer( 
                               GUI_LCD_DEV_INFO const *dev_info_ptr
                               )
{
#ifdef UI_MULTILAYER_SUPPORT	
    if ( PNULL != dev_info_ptr 
        && dev_info_ptr->lcd_id == s_debug_layer_handle.lcd_id 
        && dev_info_ptr->block_id == s_debug_layer_handle.block_id )
    {
        return TRUE;
    }
    else
#endif		
    {
        return FALSE;
    }
}

/********************************************************************************
 NAME:			MMIAPIENG_SetIsCardLogOn
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			
********************************************************************************/
PUBLIC void MMIAPIENG_SetIsCardLogOn(
                                     BOOLEAN is_cardlog_on
                                     )
{  
    BOOLEAN is_cardlog_com_on = MMIAPISD_IsCardLogOn();
    BOOLEAN card_log_sdcard_ok = FALSE;
    BOOLEAN is_success = FALSE;    

    
    SCI_TRACE_LOW("MMIENG_MAIN MMIAPIENG_SetIsCardLogOn MMIAPISD_IsCardLogOn return == %d", is_cardlog_com_on);
    
    __MMI_START_DEADLOOP_CHECK__(50*1000);
            ///SCI_Sleep(2000);

    //REF确定将SLOT0作为CARDLOG保存路径，所以MMI没有必要做遍历
    if (is_cardlog_on)
    {
#ifdef MSDC_CARD_SUPPORT
        if(MMIAPISD_IsSDPlugIn(MMI_DEVICE_SDCARD) && (SCM_SLOT_0 == MMISD_GetSLOTName(MMI_DEVICE_SDCARD)))
        {
            card_log_sdcard_ok = TRUE;
        }
#endif
        SCI_TRACE_LOW("MMIENG_MAIN MMIAPIENG_SetIsCardLogOn card_log_sdcard_ok == %d", card_log_sdcard_ok);
        
        if (is_cardlog_com_on)
        {
            //已经打开cardlog           
            is_success = TRUE;
        }
        
        if (card_log_sdcard_ok)   		 //sd卡工作正常
        {
            __MMI_PRINT_DEADLOOP_CHECK__();
            //stop all sd card      
            MMIAPISD_StopSDCard(MMI_DEVICE_SDCARD);
            __MMI_PRINT_DEADLOOP_CHECK__();
            
            is_success = MMIAPISD_OpenCardLog();
            SCI_TRACE_LOW("MMIENG_MAIN MMIAPIENG_SetIsCardLogOn MMIAPISD_OpenCardLog return == %d", is_success);
            
            __MMI_PRINT_DEADLOOP_CHECK__();
            //is_cardlog_on写MMI NV
            //is_success = TRUE;
        }
        else
        {
            is_success = FALSE;
        }
        if (is_success)
        {
            MMINV_WRITE(MMIENG_NV_IS_CARDLOG_ON, &is_cardlog_on);
        }
    }
    else
    {
        if (is_cardlog_com_on)
        {
            __MMI_PRINT_DEADLOOP_CHECK__();
            MMIAPISD_CloseCardLog();
            __MMI_PRINT_DEADLOOP_CHECK__();

            //cardlog is saved in slot0 cadrd only
            if(MMIAPISD_IsSDPlugIn(MMI_DEVICE_SDCARD))
            {
                MMIAPISD_InitForFAT(MMI_DEVICE_SDCARD);
            }
            else
            {
                SCI_TRACE_LOW("MMIENG_MAIN MMIAPIENG_SetIsCardLogOn MMIAPISD_IsSDPlugIn return FALSE");
            }
        }
        is_success = TRUE;
        MMINV_WRITE(MMIENG_NV_IS_CARDLOG_ON, &is_cardlog_on);
    }
    __MMI_END_DEADLOOP_CHECK__();
    //窗口处理
    if (is_success)
    {
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);       
    }
    else
    {
        MMIPUB_OpenAlertFailWin(TXT_ERROR/*TXT_COMMON_SD_FAIL*/);
    }

}

/********************************************************************************
 NAME:			CardLogOnForStartup
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			
********************************************************************************/
LOCAL void CardLogOnForStartup(void)
{  
   
    BOOLEAN is_hasSDCard = FALSE;
    BOOLEAN is_cardlog_com_on = MMIAPISD_IsCardLogOn();	
                
	if (!is_cardlog_com_on)
	{
        if(MMIAPISD_IsSDPlugIn(MMI_DEVICE_SDCARD))
        {
            MMIAPISD_StopSDCard(MMI_DEVICE_SDCARD);
            is_hasSDCard = TRUE;
        }

        if(is_hasSDCard)
        {
            MMIAPISD_OpenCardLog();
        }
	}

}

/********************************************************************************
 NAME:			MMIAPIENG_StartupAdjustCardLogOn
 DESCRIPTION:	开机调整CardLog状态
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			2007.10.11
********************************************************************************/
PUBLIC void MMIAPIENG_StartupAdjustCardLogOn(void)
{
    BOOLEAN is_cardlog_on = MMIAPIENG_GetIsCardLogOn();   

    if (is_cardlog_on)
    {
        CardLogOnForStartup();
    }
}

/*****************************************************************************/
// 	Description : InitIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void InitIQMode(void)
{
    MN_RETURN_RESULT_E nv_result =MN_RETURN_SUCCESS;

    MMINV_READ(MMIENG_NV_IQ_MODE, &s_iq_mode_param, nv_result);
    s_iq_mode_param.b_is_start = FALSE;

    if (MN_RETURN_SUCCESS != nv_result)
    {
        uint32 i = 0;
        SCI_MEMSET(&s_iq_mode_param, 0, sizeof(MMIENG_IQ_MODE_T));
        s_iq_mode_param.b_status = FALSE;
        s_iq_mode_param.b_power_on_start = FALSE;

        s_iq_mode_param.cur_mode = MMIENG_IQ_MODE_A;

/*[START] Deleted by Jerry Liang for IQ MODE 2008/07/31*/
#if 0
        for (i = 0; i < MMIENG_IQ_MODE_MAX; i++)
        {
            s_iq_mode_param.record_param[i].format = i + AUD_REC_DSP_DATA_MIN;
            s_iq_mode_param.record_param[i].output_channel = RECORD_OUTPUT_SAVE_TO_FILE;
            s_iq_mode_param.record_param[i].time_limit = 0;
            s_iq_mode_param.record_param[i].frame_length = DSP_DATA_FRAME_LENGTH;
            s_iq_mode_param.record_param[i].block_count= DSP_DATA_BLOCK_COUNT_2;
        }
#endif
/*[END]Deleted by Jerry Liang for IQ MODE 2008/07/31*/

/*[START]Jerry Liang for IQ MODE 2008/07/31*/
        for (i = 0; i < MMIENG_IQ_MODE_MAX; i++)
        {
            s_iq_mode_param.record_param[i].format = i + AUD_REC_DSP_DATA_MIN;/*lint !e64*/
            s_iq_mode_param.record_param[i].output_channel = RECORD_OUTPUT_SAVE_TO_FILE;
            s_iq_mode_param.record_param[i].time_limit = 0;
	     if((MMIENG_IQ_MODE_E)i == MMIENG_IQ_MODE_B)
	     {
	     		s_iq_mode_param.record_param[i].frame_length = DSP_DATA_FRAME_LENGTH_VOICE_DATA;
			s_iq_mode_param.record_param[i].block_count= DSP_DATA_BLOCK_COUNT_2;	
	     }
	     else
	     {
            		s_iq_mode_param.record_param[i].frame_length = DSP_DATA_FRAME_LENGTH;
			s_iq_mode_param.record_param[i].block_count= DSP_DATA_BLOCK_COUNT_3;		
	     }
	     s_iq_mode_param.record_param[i].arfcn = 8;
        }
/*[END]Jerry Liang for IQ MODE 2008/07/31*/

        MMINV_WRITE(MMIENG_NV_IQ_MODE, &s_iq_mode_param);
    }

    if (MMIAPIENG_GetIQModeStatus() && MMIAPIENG_GetIQPowerOnStartStatus())
    {
        if (MMIAPIENG_StartIQData())
        {
            s_iq_mode_param.b_is_start = TRUE;
            MMINV_WRITE(MMIENG_NV_IQ_MODE, &s_iq_mode_param);
        }
    }
}

/*****************************************************************************/
// 	Description : MMIAPIENG_SetIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIQMode(MMIENG_IQ_MODE_T *iq_mode)
{
    if(iq_mode == PNULL)
    {
        //SCI_TRACE_LOW:"mmieng_main.c MMIAPIENG_SetIQMode iq_mode == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_2570_112_2_18_2_17_14_10,(uint8*)"");
        return ;
    }

    SCI_MEMSET(&s_iq_mode_param, 0x00, sizeof(MMIENG_IQ_MODE_T));
    SCI_MEMCPY(&s_iq_mode_param, iq_mode, sizeof(MMIENG_IQ_MODE_T));

    MMINV_WRITE(MMIENG_NV_IQ_MODE, &s_iq_mode_param);
}

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMIENG_IQ_MODE_T MMIAPIENG_GetIQMode(void)
{
    return s_iq_mode_param;
}

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQModeStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQModeStatus(void)
{
    return s_iq_mode_param.b_status;
}

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQDataStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQDataStatus(void)
{
    return s_iq_mode_param.b_is_start;
}

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQPowerOnStartStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQPowerOnStartStatus(void)
{
    return s_iq_mode_param.b_power_on_start;
}

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
// 	Description : NotifyCallback
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void NotifyCallback(RECORD_SRV_HANDLE record_srv_handle, 
						  MMIRECORD_SRV_SUPPORT_TYPE_E support_type, 
						  MMIRECORD_SRV_RESULT_E result)
{

	SCI_TRACE_LOW("[ENG RECORD] NotifyCallback record_srv_handle = %d, s_record_info.record_srv_handle = %d, result = %d, support_type = %d",record_srv_handle,s_eng_record_handle,result, support_type);
	if (record_srv_handle == s_eng_record_handle
		&& MMIRECORD_SRV_SUPPORT_RECORD == support_type)
	{			
		
		if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED ,GetCurrentDev(), MMIRECORD_SRV_MIN_NEED_SPACE, PNULL))
		{
			result = MMIRECORD_SRV_RESULT_NO_SPACE;
		}
		HandleRecordEndMsg(result);//最好post msg到相应的窗口处理		
	}
}

/*****************************************************************************/
// 	Description : HandleRecordEndMsg
//	Global resource dependence :
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
LOCAL void HandleRecordEndMsg(MMIRECORD_SRV_RESULT_E result)
{
	MMIFILE_ERROR_E    fs_ret = SFS_ERROR_NONE;
	
	MMIDEFAULT_TurnOnBackLight();
	switch (result)
    {
    case MMIRECORD_SRV_RESULT_SUCCESS:  
        break;
        
    case MMIRECORD_SRV_RESULT_MAX_FILE:  
        MMIPUB_OpenAlertFailWin(TXT_COMMON_RECORD_MAX_FILE);
        break;
        
    case MMIRECORD_SRV_RESULT_NO_SPACE:
        //提示空间不足
        fs_ret = SFS_ERROR_NO_SPACE;
        ShowRecordErrorMsg(fs_ret);
        break;
    case MMIRECORD_SRV_RESULT_FOLDER_FULL:
        fs_ret = SFS_ERROR_FOLD_FULL;
        ShowRecordErrorMsg(fs_ret);
        break;
        
    default:
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
        break;
    }
}

/*****************************************************************************/
//  Description : 开始录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL MMIRECORD_SRV_RESULT_E StartENGRecord(void)
{    
    MMI_STRING_T  call_name_str = {0};
	MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();
	MMIRECORD_SRV_RECORD_PARAM_T record_param = {0};
	MMIFILE_DEVICE_E record_dev = MMIAPIRECORD_GetRecordStorage();
   
	
	if (PNULL != s_eng_record_handle)
	{
		return MMIRECORD_SRV_RESULT_ERROR;
	}
		
	s_eng_record_handle = MMIRECORDSRV_RequestRecordHandle(NotifyCallback);
	if (PNULL == s_eng_record_handle)
	{
		return MMIRECORD_SRV_RESULT_ERROR;
	}
    
	if (iq_mode_param.cur_mode < MMIENG_IQ_MODE_MAX)//测试模式(在通话中使用record的通道记录通话内容，使用的是record的通道)
    {   	
		call_name_str.wstr_ptr = L"ENGIQDATA";
		call_name_str.wstr_len = 10;

		record_param.fmt = MMISRVAUD_RECORD_FMT_ADPCM;
		record_param.prefix_ptr = PNULL;
		record_param.record_dev_ptr = &record_dev;
		record_param.record_file_id_ptr = PNULL;
		record_param.record_name_ptr = &call_name_str;
		record_param.source = MMISRVAUD_RECORD_SOURCE_CALL;
		record_param.record_param_ptr = &(iq_mode_param.record_param[iq_mode_param.cur_mode]);
		record_param.volume = MMIAPISET_GetMultimVolume();
		record_param.is_create_file_name = TRUE;
		return MMIRECORDSRV_StartRecord(s_eng_record_handle, record_param);        
	}
	return MMIRECORD_SRV_RESULT_ERROR;
}
/*****************************************************************************/
//  Description : 停止录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIENG_StopENGRecord(void)
{
	if (PNULL != s_eng_record_handle)
	{
		MMIRECORDSRV_StopRecord(s_eng_record_handle);
		MMIRECORDSRV_FreeRecordHandle(s_eng_record_handle);
		s_eng_record_handle = 0;
	}

}

/*****************************************************************************/
//  Description : MMIENG_DeletRecordFile
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIENG_DeletRecordFile(void)
{
	MMIRECORDSRV_DeleteRecordFile(s_eng_record_handle,TRUE);
}

/*****************************************************************************/
//  Description : GetCurrentDev
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetCurrentDev(void)
{
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_NUM;
    wchar *file_path_ptr = MMIRECORDSRV_GetRecordFilePath(s_eng_record_handle);

    if(PNULL != file_path_ptr)
    {
        file_dev = MMIAPIFMM_GetDeviceTypeByPath(file_path_ptr, MMIFILE_DEVICE_LEN);
    }    

    if(MMI_DEVICE_NUM <= file_dev)
    {
        file_dev= MMIAPIRECORD_GetRecordStorage();
    }

    SCI_TRACE_LOW("[ENG RECORD] GetCurrentDev file_dev = %d",file_dev);
    return file_dev;
}

/*****************************************************************************/
//  Description : show error message as error type 
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param [Input]:  fs_error    error  type as MMIFILE_ERROR_E
/*****************************************************************************/
LOCAL void  ShowRecordErrorMsg(MMIFILE_ERROR_E fs_error)
{
    MMIFILE_DEVICE_E   filedev ;
    BOOLEAN  ret = TRUE;

    filedev = GetCurrentDev();
    
    ret= MMIAPIFMM_CheckMemoryCard(filedev);

    //除磁盘格式错误外的
    if(ret)
    {
        if(SFS_ERROR_ABORT == fs_error)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, 
                MMIPUB_SOFTKEY_ONE, PNULL);                      
        }
        else
        {
            MMIAPIFMM_PromptFileErrorType(PNULL,fs_error, filedev);
        }
    }
}
#endif
/*****************************************************************************/
// 	Description : MMIAPIENG_StartIQDataAcquire
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartIQData(void)
{
#ifdef MMI_RECORD_SUPPORT
    MMIRECORD_SRV_RESULT_E  error_code = MMIRECORD_SRV_RESULT_ERROR;
#endif
   

#ifndef CARD_LOG
    MMIAPISET_StopAllRing(TRUE);
    
    MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
#endif

    MMIAPISET_ResetDisplaySetting();

#ifdef CARD_LOG
    MMISRVAUD_ReqVirtualHandle("IQ Mode", MMISRVAUD_PRI_HIGH_N10);
    #ifndef WIN32
    LogSaveOutput_SetReady();
    #endif
#ifdef MMI_RECORD_SUPPORT
    error_code = MMIRECORD_SRV_RESULT_SUCCESS;
#endif
#else
#ifdef MMI_RECORD_SUPPORT
    error_code = StartENGRecord();    
#endif
#endif

#ifdef MMI_RECORD_SUPPORT
    HandleRecordEndMsg(error_code);
    return (MMIRECORD_SRV_RESULT_SUCCESS == error_code);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description : MMIAPIENG_StopQData
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_StopQData(void)
{
    MMIENG_IQ_MODE_T iq_mode_param = MMIAPIENG_GetIQMode();

    iq_mode_param.b_is_start = FALSE;

    MMIAPIENG_SetIQMode(&iq_mode_param);

#ifdef CARD_LOG
    #ifndef WIN32
    LogSaveOutput_SetNotReady();
    #endif    
    MMISRVAUD_FreeVirtualHandle("IQ Mode");
#else
#ifdef MMI_RECORD_SUPPORT
    MMIENG_StopENGRecord();
#endif
#endif
}
/********************************************************************************
 NAME:			MMIAPIENG_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC void MMIAPIENG_SetPktOutPut(void)
{
    BOOLEAN is_on = FALSE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
	
    MMINV_READ(MMIENG_NV_CAP_OUTPUT,&is_on,return_value);   
    if(MN_RETURN_SUCCESS!=return_value)
    {
#ifdef MMI_GPRS_SUPPORT
        is_on = TCPIPDEBUG_IsPktOutputEnable();
#endif
        MMINV_WRITE(MMIENG_NV_CAP_OUTPUT,&is_on);
    }
#ifdef MMI_GPRS_SUPPORT
#ifndef _WIN32
    TCPIPDEBUG_EnablePktOutput( is_on );	
#endif
#endif
}

/*****************************************************************************/
//  Description : MMIENG_SetUa
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_SetUa(MMIENG_UA_T *ua)
{
    MMINV_WRITE(MMIENG_NV_UA,ua);
    return TRUE;
    
}

/*****************************************************************************/
//  Description : MMIENG_GetUa
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUa(MMIENG_UA_T *ua_ptr)
{
   MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	
    SCI_MEMSET(ua_ptr, 0, sizeof(MMIENG_UA_T));
    MMINV_READ(MMIENG_NV_UA, ua_ptr, return_value);
    
    if (return_value != MN_RETURN_SUCCESS)
    {
        //NOTES:DON'T modify!!!  default value is NULL
        SCI_MEMSET(ua_ptr,0,sizeof(MMIENG_UA_T));
	 MMINV_WRITE(MMIENG_NV_UA,ua_ptr);
    }
    return TRUE;
}


/*****************************************************************************/
//  Description : MMIENG_GetUserAgentIsUseList
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUserAgentIsUseList(void)
{
    BOOLEAN             is_use_list = FALSE;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_IS_USER_AGENT_USE_LIST, &is_use_list, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        is_use_list = FALSE;
        MMINV_WRITE(MMIENG_NV_IS_USER_AGENT_USE_LIST, &is_use_list);
    }
    
    return is_use_list;
}

/*****************************************************************************/
//  Description : MMIENG_SetUserAgentIsUseList
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENG_SetUserAgentIsUseList(BOOLEAN is_use_list)
{	
    MMINV_WRITE(MMIENG_NV_IS_USER_AGENT_USE_LIST, &is_use_list);    
}

/*****************************************************************************/
//  Description : MMIENG_GetUaProfileIsUseList
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUaProfileIsUseList(void)
{
    BOOLEAN             is_use_list = FALSE;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_IS_UA_PROFILE_USE_LIST, &is_use_list, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        is_use_list = FALSE;
        MMINV_WRITE(MMIENG_NV_IS_UA_PROFILE_USE_LIST, &is_use_list);
    }
    
    return is_use_list;
}
/*****************************************************************************/
//  Description : MMIENG_SetUaProfileIsUseList
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENG_SetUaProfileIsUseList(BOOLEAN is_use_list)
{	
    MMINV_WRITE(MMIENG_NV_IS_UA_PROFILE_USE_LIST, &is_use_list);    
}

/*****************************************************************************/
//  Description : MMIENG_GetUserAgentListNum
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIENG_GetUserAgentListNum(void)
{
    uint32             list_num = 0;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_USER_AGENT_NUM, &list_num, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        list_num = 0;
        MMINV_WRITE(MMIENG_NV_USER_AGENT_NUM, &list_num);
    }
    
    return list_num;
}

/*****************************************************************************/
//  Description : MMIENG_SetUserAgentListNum
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENG_SetUserAgentListNum(uint32 list_num)
{	
    MMINV_WRITE(MMIENG_NV_USER_AGENT_NUM, &list_num);    
}

/*****************************************************************************/
//  Description : MMIENG_GetUaProfileListNum
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIENG_GetUaProfileListNum(void)
{
    uint32             list_num = 0;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_UA_PROFILE_NUM, &list_num, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        list_num = 0;
        MMINV_WRITE(MMIENG_NV_UA_PROFILE_NUM, &list_num);
    }
    
    return list_num;
}

/*****************************************************************************/
//  Description : MMIENG_SetUaProfileListNum
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENG_SetUaProfileListNum(uint32 list_num)
{	
    MMINV_WRITE(MMIENG_NV_UA_PROFILE_NUM, &list_num);    
}

/*****************************************************************************/
//  Description : MMIENG_GetUserAgentIsUse
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUserAgentIsUse(void)
{
    BOOLEAN             is_use = FALSE;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_IS_USER_AGENT_USE, &is_use, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        is_use = FALSE;
        MMINV_WRITE(MMIENG_NV_IS_USER_AGENT_USE, &is_use);
    }
    
    return is_use;
}

/*****************************************************************************/
//  Description : MMIENG_SetUserAgentIsUse
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENG_SetUserAgentIsUse(BOOLEAN is_use)
{	
    MMINV_WRITE(MMIENG_NV_IS_USER_AGENT_USE, &is_use);    
}

/*****************************************************************************/
//  Description : MMIENG_GetUaProfileIsUse
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetUaProfileIsUse(void)
{
    BOOLEAN             is_use = FALSE;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_IS_UA_PROFILE_USE, &is_use, nv_value);
    
    if(MN_RETURN_SUCCESS != nv_value)
    {
        is_use = FALSE;
        MMINV_WRITE(MMIENG_NV_IS_UA_PROFILE_USE, &is_use);
    }
    
    return is_use;
}
/*****************************************************************************/
//  Description : MMIENG_SetUaProfileIsUse
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIENG_SetUaProfileIsUse(BOOLEAN is_use)
{	
    MMINV_WRITE(MMIENG_NV_IS_UA_PROFILE_USE, &is_use);    
}

/*****************************************************************************/
//  Description : MMIAPIENG_SetUa
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetUa(MMIENG_UA_T *ua_ptr)
{
    MMIENG_SetUa(ua_ptr);
    return TRUE;
    
}

/*****************************************************************************/
//  Description : MMIAPIENG_GetUa,when eng ua is open, return the eng ua info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetUa(MMIENG_UA_T *ua_ptr)
{
    MMIENG_UA_T ua = {0};
    
    if (ua_ptr != PNULL)
    {
        SCI_MEMSET(ua_ptr, 0, sizeof(MMIENG_UA_T));
        
        MMIENG_GetUa(&ua);
        if (MMIENG_GetUserAgentIsUse())//user_agent
        {
            SCI_MEMCPY(ua_ptr->user_agent, ua.user_agent, sizeof(ua.user_agent));
        }
        
        if (MMIENG_GetUaProfileIsUse())//uaprofile
        {
            SCI_MEMCPY(ua_ptr->uaprofile, ua.uaprofile, sizeof(ua.uaprofile));
        }
    }
    
    return TRUE;
}

#ifdef CMMB_SUPPORT

/***********************************************************************/
// 	Description : CmmbTest_ParseIni
//	Global resource dependence : nones
//  Author:
//	Note: 解析cmmbtest.ini配置文件
/***********************************************************************/
PUBLIC BOOLEAN CmmbTest_ParseIni(MMIENG_CMMB_INI_FILE_INFO_T* ini_file_info)
{
    uint32 read_len = 0;
    uint32 sfs_size = 0;
    SFS_HANDLE sfs_handle;
    BOOLEAN result = FALSE;
    SFS_ERROR_E sfs_error  = SFS_NO_ERROR;
    char *fileInfo;
    char DataBuf[MMIFMM_FULL_FILENAME_LEN+1];
    wchar dfile_name[MMIFMM_FULL_FILENAME_LEN] = {'D', ':', '\\', 'C','M','M','B','\\','8', '8', '0', '0', 'g', 'c', 'm', 'm', 'b','t','e','s','t','.','i','n','i',0};/*lint !e785 */
    wchar efile_name[MMIFMM_FULL_FILENAME_LEN] = {'E', ':', '\\', 'C','M','M','B','\\','8', '8', '0', '0', 'g', 'c', 'm', 'm', 'b','t','e','s','t','.','i','n','i',0};/*lint !e785*/
    
    //SCI_MEMSET(file_name, 0, sizeof(file_name));
    
    //MMIAPICOM_StrToWstr("D:\\cmmbtest.ini", file_name);
    
    sfs_handle = SFS_CreateFile((uint16*)dfile_name, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, SCI_NULL, 0);
    
    //SCI_TRACE_LOW:"Open file cmmbtest.ini , SFS_HANDLE = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_2984_112_2_18_2_17_15_11,(uint8*)"d", sfs_handle);
    if (sfs_handle == SFS_INVALID_HANDLE)
    {
        //SCI_TRACE_LOW:"Open file d:cmmbtest.ini fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_2987_112_2_18_2_17_15_12,(uint8*)"");
        
        sfs_handle = SFS_CreateFile((uint16*)efile_name, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, SCI_NULL, 0);
        if (sfs_handle == SFS_INVALID_HANDLE)
        {
            //SCI_TRACE_LOW:"Open file e:cmmbtest.ini fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_2992_112_2_18_2_17_15_13,(uint8*)"");
            return FALSE;
        }
    }
    
    sfs_error = SFS_GetFileSize(sfs_handle, &sfs_size);
    if (sfs_error > SFS_NO_ERROR)
    {
        //SCI_TRACE_LOW:"Get file size error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3000_112_2_18_2_17_15_14,(uint8*)"");
        SFS_CloseFile(sfs_handle);
        return FALSE;
    }
    
    fileInfo = (uint8*)SCI_ALLOC_APP(sfs_size + 3);/*lint !e64*/
    sfs_error = SFS_ReadFile(sfs_handle, (void*)fileInfo, sfs_size, &read_len, PNULL);
    
    if ((sfs_error > SFS_NO_ERROR) || (sfs_size != read_len))
    {
        //SCI_TRACE_LOW:"Read file error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3010_112_2_18_2_17_15_15,(uint8*)"");
        SFS_CloseFile(sfs_handle);
        return FALSE;
    }
    
    strcpy(fileInfo + read_len, "\r\n");
    
    //network_id
    if (CmmbTest_ParseLine(fileInfo, "network_id", DataBuf, CMMB_TEST_FREQ_LEN + 1))
    {
        ini_file_info->network_id = atol(DataBuf);
        //SCI_TRACE_LOW:"CmmbTest_ParseIni parse network_id = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3021_112_2_18_2_17_15_16,(uint8*)"d", ini_file_info->network_id);
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"parse network_id error "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3026_112_2_18_2_17_15_17,(uint8*)"");
        ini_file_info->network_id = 0;
        SCI_FREE(fileInfo);
        SFS_CloseFile(sfs_handle);
        return FALSE;
    }
    
    //service_id
    if (CmmbTest_ParseLine(fileInfo, "service_id", DataBuf, CMMB_TEST_FREQ_LEN + 1))
    {
        ini_file_info->service_id = atol(DataBuf);
        //SCI_TRACE_LOW:"CmmbTest_ParseIni parse service_id = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3037_112_2_18_2_17_15_18,(uint8*)"d", ini_file_info->service_id);
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"parse service_id error "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3042_112_2_18_2_17_15_19,(uint8*)"");
        ini_file_info->service_id = 0;
        SCI_FREE(fileInfo);
        SFS_CloseFile(sfs_handle);
        return FALSE;
    }
    
    //rf_search_freq
    SCI_MEMSET(DataBuf, 0, sizeof(DataBuf));
    if (CmmbTest_ParseLine(fileInfo, "rf_search_freq", DataBuf, CMMB_TEST_FREQ_LEN + 1))
    {
        ini_file_info->rf_test_freq = atol(DataBuf);
        //SCI_TRACE_LOW:"CmmbTest_ParseIni parse rf_search_freq = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3054_112_2_18_2_17_15_20,(uint8*)"d", ini_file_info->rf_test_freq);
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"parse search_freq error "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3059_112_2_18_2_17_15_21,(uint8*)"");
        ini_file_info->rf_test_freq = 0;
        SCI_FREE(fileInfo);
        SFS_CloseFile(sfs_handle);
        return FALSE;
    }
    
    //bler_err_blk
    SCI_MEMSET(DataBuf, 0, sizeof(DataBuf));
    if (CmmbTest_ParseLine(fileInfo, "bler_err_blk", DataBuf, CMMB_TEST_BLER_LEN + 1))
    {
        ini_file_info->channel_info.bler_err_blk = atol(DataBuf);
        //SCI_TRACE_LOW:"CmmbTest_ParseIni parse bler_err_blk = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3071_112_2_18_2_17_15_22,(uint8*)"d", ini_file_info->channel_info.bler_err_blk);
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"parse bler_err_blk error "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3076_112_2_18_2_17_15_23,(uint8*)"");
        SCI_FREE(fileInfo);
        SFS_CloseFile(sfs_handle);
        return FALSE;
    }
    
    //bler_num_blk
    SCI_MEMSET(DataBuf, 0, sizeof(DataBuf));
    if (CmmbTest_ParseLine(fileInfo, "bler_num_blk", DataBuf, CMMB_TEST_BLER_LEN + 1))
    {
        ini_file_info->channel_info.bler_num_blk = atol(DataBuf);
        //SCI_TRACE_LOW:"CmmbTest_ParseIni parse bler_num_blk = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3087_112_2_18_2_17_15_24,(uint8*)"d", ini_file_info->channel_info.bler_num_blk);
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"parse bler_num_blk error "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3092_112_2_18_2_17_15_25,(uint8*)"");
        SCI_FREE(fileInfo);
        SFS_CloseFile(sfs_handle);
        return FALSE;
    }
    
    //rssi_min
    SCI_MEMSET(DataBuf, 0, sizeof(DataBuf));
    if (CmmbTest_ParseLine(fileInfo, "rssi_min", DataBuf, CMMB_TEST_RSSI_LEN + 1))
    {
        ini_file_info->channel_info.rssi_min = atoi(DataBuf);
        //SCI_TRACE_LOW:"CmmbTest_ParseIni parse rssi_min = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3103_112_2_18_2_17_15_26,(uint8*)"d", ini_file_info->channel_info.rssi_min);
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"parse rssi_min error "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3108_112_2_18_2_17_15_27,(uint8*)"");
        SCI_FREE(fileInfo);
        SFS_CloseFile(sfs_handle);
        return FALSE;
    }
    
    //rssi_max
    SCI_MEMSET(DataBuf, 0, sizeof(DataBuf));
    if (CmmbTest_ParseLine(fileInfo, "rssi_max", DataBuf, CMMB_TEST_RSSI_LEN + 1))
    {
        ini_file_info->channel_info.rssi_max = atoi(DataBuf);
        //SCI_TRACE_LOW:"CmmbTest_ParseIni parse rssi_max = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3119_112_2_18_2_17_15_28,(uint8*)"d", ini_file_info->channel_info.rssi_max);
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"parse rssi_max error "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3124_112_2_18_2_17_15_29,(uint8*)"");
        SCI_FREE(fileInfo);
        SFS_CloseFile(sfs_handle);
        return FALSE;
    }
    
    SCI_FREE(fileInfo);
    SFS_CloseFile(sfs_handle);
    
    return result;
}

/*****************************************************************************/
// Description : MTVSERVICE_CALLBACK实现
// Global resource dependence : 
// Author: xin.li
// Note: 
/*****************************************************************************/
LOCAL void MTVServiceCallback(MBBMS_SERVICE_INFO_T *service_info_ptr)
{
    switch(service_info_ptr->type)
    {
    case MBBMS_SERVICE_MANUAL_SG:
        //SCI_TRACE_LOW:"MTV: MBBMS_SERVICE_MANUAL_SG"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3145_112_2_18_2_17_15_30,(uint8*)"");
        if(MBBMS_SERVICE_MANUAL_SG == service_info_ptr->subtype)
        {
            //CMMB搜网主流程结束
            s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
            s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_IDLE;
            
            MMK_PostMsg(MMIENG_CMMB_WAIT_WIN_ID, MSG_ENG_MTV_SEARCH_CNF, PNULL, NULL);
        }
        break;
        
    case MBBMS_SERVICE_STOP_MANUAL_SG:
        //SCI_TRACE_LOW:"MTV MBBMS_SERVICE_STOP_MANUAL_SG"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3157_112_2_18_2_17_15_31,(uint8*)"");
        s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
        s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_IDLE;
        break;
        
    case MBBMS_SERVICE_PLAY:
        //SCI_TRACE_LOW:"MTV received MBBMS_SERVICE_PLAY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3163_112_2_18_2_17_15_32,(uint8*)"");
        if(MBBMS_SERVICE_PLAY == service_info_ptr->subtype)
        {
            //播放主流程结束
            s_play_result = service_info_ptr->result;
            
            MMK_PostMsg(MMIENG_CMMB_WAIT_WIN_ID, MSG_ENG_MTV_PLAY_CNF, PNULL, NULL);
        }
        break;
        
    case MBBMS_SERVICE_STOP:
        //SCI_TRACE_LOW:"MTV received MBBMS_SERVICE_STOP"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3174_112_2_18_2_17_15_33,(uint8*)"");
        if(MBBMS_SERVICE_STOP == service_info_ptr->subtype)
        {
            s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
            s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_IDLE;
        }
        break;
        
    default:
        break;
    }
}

/********************************************************************************
 NAME:			MMIAPIENG_InitMTV
 DESCRIPTION:	初始化MTV
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2010-12-30
********************************************************************************/
PUBLIC void MMIAPIENG_InitMTV(void)
{
    MBBMS_Service_Init_Para_T   init_para = {0};
    DPLAYER_DISPLAY_PARAM_T     display_param = {0};
    AUDIO_DEVICE_MODE_TYPE_E    audio_mode = 0;
    
    //SCI_TRACE_LOW:"MMIAPIENG_InitMTV start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3200_112_2_18_2_17_15_34,(uint8*)"");
    
    //init mtvservice
    {
        wchar   *device_ptr = PNULL;
        uint16  device_len = 0;        
        MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
        wchar   full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
        uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
        //BOOLEAN is_udisk_exist = FALSE;
        //BOOLEAN is_sdcard_exist = FALSE;
        
        
        //cmmb mode only
        init_para.mode = MBBMS_SERVICE_MODE_CMMB;
        
        //SG path 
        device_type = MMIAPIFMM_GetFirstValidSD();
        //MMIAPIUDISK_GetAllDeviceStatus(&is_udisk_exist, &is_sdcard_exist);
        if(device_type < MMI_DEVICE_NUM)
        {
            device_ptr = (wchar*)MMIAPIFMM_GetDevicePath(device_type);
            device_len = MMIAPIFMM_GetDevicePathLen(device_type);
            

        }
        else
        {    
            device_ptr = (wchar*)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
            device_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK);
        }
        MMIAPIFMM_CombineFullPath(
            device_ptr, 
            device_len,
            PNULL, 
            0,
            PNULL, 
            0,
            full_path_name,
            &full_path_len
            );
        SCI_MEMCPY(init_para.sg_path, (uint8*)full_path_name, full_path_len*2);
        init_para.sg_path_len = full_path_len*2;
        
        //cmmb esg path
        SCI_MEMCPY(init_para.cmmb_esg_path, (uint8*)full_path_name, full_path_len*2);
        init_para.cmmb_esg_len = full_path_len*2;
    }
    MTVSERVICE_Init(&init_para, MTVServiceCallback);

    //OSD BLOCK set
    GUIBLOCK_SetType(175, MMIMTV_TRANSPARENT_COLOR, GUIBLOCK_TYPE_OSD,(GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev());
    
    //display param set
    {
        uint16 w, h;
        uint32 video_width = 0;
        uint32 video_height = 0;
        
        GUILCD_GetLogicWidthHeight(0, &w, &h);
        
        //calc video size
        video_width = w;
        video_height = (video_width * 3) / 4;
        
        display_param.disp_rect.x = 0;
        display_param.disp_rect.y = 0;
        display_param.disp_rect.dx = video_width;
        display_param.disp_rect.dy = video_height;
        
        display_param.target_rect.x = 0;
        display_param.target_rect.y = 0;
        display_param.target_rect.dx = video_width;
        display_param.target_rect.dy = video_height;
        
        display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
        display_param.RotateAngle = GUILCD_GetLogicAngle(0);
        display_param.audioSupport = 1;
        display_param.videoSupport = 1;
        display_param.LcdId = 0;
        display_param.LcdBlkId = 0;
        display_param.b_support_seek = 0;
    }
    MTVSERVICE_SetDisplayParam(&display_param);
        
    //flow info
    s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
    s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_IDLE;
    s_display_mode = MMIENG_MTV_CHANNEL_PLAY;
    
    //SCI_TRACE_LOW:"MMIAPIENG_InitMTV end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3290_112_2_18_2_17_16_35,(uint8*)"");
}

/********************************************************************************
 NAME:			MMIAPIENG_ReleaseMTV
 DESCRIPTION:	MTV资源释放
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2010-12-30
********************************************************************************/
PUBLIC void MMIAPIENG_ReleaseMTV(void)
{
    //SCI_TRACE_LOW:"MMIAPIENG_ReleaseMTV"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3302_112_2_18_2_17_16_36,(uint8*)"");
    
    //stop flow first
    MMIAPIENG_StopMTVFlow();
    while(MMIMBBMS_FLOW_STATE_IDLE != s_curr_flow_info.flow_type)
    {
        SCI_Sleep(100);
    }
    
    //reset osd block
    GUIBLOCK_ResetMainType();
    
    MTVSERVICE_CMMBClose();
    MTVSERVICE_MBMSExit();
    
    //flow info reset
    s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
    s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_IDLE;
}

/********************************************************************************
 NAME:			MMIAPIENG_ReleaseMTV
 DESCRIPTION:	MTV停止流程
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2010-12-30
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StopMTVFlow(void)
{
    MBBMS_SERVICE_RET_E service_ret = MBBMS_SERVICE_FUNC_INVALID;
    BOOLEAN             recode = TRUE;
    
    //SCI_TRACE_LOW:"MMIAPIENG_StopMTVFlow curr flow status=%d,curr_flow type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3334_112_2_18_2_17_16_37,(uint8*)"dd",s_curr_flow_info.status,s_curr_flow_info.flow_type);
    
    if(MMIMBBMS_FLOW_STATE_IDLE==s_curr_flow_info.status) 
    {
        return FALSE;
    }else if(MMIMBBMS_FLOW_STATE_STOPPING==s_curr_flow_info.status)
    {
        return TRUE;    
    }
    
    switch(s_curr_flow_info.flow_type)
    {
    case MMIMBBMS_FLOW_SG_MANUAL_UPDATE:
        {
            s_curr_flow_info.flow_type = MMIMBBMS_FLOW_SG_MANUAL_UPDATE;
            s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_STOPPING;
            
            service_ret = MTVSERVICE_StopManualUpdateSG();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIAPIENG_StopMTVFlow MTVSERVICE_StopParseServiceGuide ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3354_112_2_18_2_17_16_38,(uint8*)"d", service_ret);
                s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
                s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_IDLE;
            }
        }
        break;
        
    case MMIMBBMS_FLOW_PLAY:
        {
            s_curr_flow_info.flow_type = MMIMBBMS_FLOW_PLAY;
            s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_STOPPING;
            
            service_ret = MTVSERVICE_Stop();
            if(MBBMS_SERVICE_FUNC_OK != service_ret)
            {
                //SCI_TRACE_LOW:"MMIAPIENG_StopMTVFlow MTVSERVICE_Stop ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3369_112_2_18_2_17_16_39,(uint8*)"d", service_ret);
                s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
                s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_IDLE;
            }
        }
        break;
        
    default:
        recode = FALSE;
        break;
    }
    
    return recode;
}

/********************************************************************************
 NAME:			MMIAPIENG_PlayMTVService
 DESCRIPTION:	直播
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_PlayMTVService(uint32 net_id, uint32 service_id)
{
    MBBMS_SERVICE_RET_E service_ret = MBBMS_SERVICE_FUNC_OK;
    
    //SCI_TRACE_LOW:"MMIAPIENG_PlayMTVService  net_id=%d, service_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3395_112_2_18_2_17_16_40,(uint8*)"dd", net_id, service_id);
    
    s_display_mode = MMIENG_MTV_CHANNEL_PLAY;
    
    //stop flow first
    MMIAPIENG_StopMTVFlow();
    while(MMIMBBMS_FLOW_STATE_IDLE != s_curr_flow_info.flow_type)
    {
        SCI_Sleep(100);
    }
    
    //start play
    s_curr_flow_info.flow_type = MMIMBBMS_FLOW_PLAY;
    s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_STARTING;
    service_ret = MTVSERVICE_Play(net_id, service_id);
    if(MBBMS_SERVICE_FUNC_OK != service_ret)
    {
        //SCI_TRACE_LOW:"MMIAPIENG_PlayMTVService  MTVSERVICE_Play return = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3412_112_2_18_2_17_16_41,(uint8*)"d", service_ret);
        s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
        s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_IDLE;
        
        //play error tip
        MMIPUB_OpenAlertFailWin(TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);
    }
}

/********************************************************************************
NAME:			MMIAPIENG_MTVRFTest
DESCRIPTION:	RF测试(用于无信号时的工厂测试)
PARAM IN:		
PARAM OUT:		
AUTHOR: xin.li
DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_MTVRFTest(uint32 net_frequency)
{
    MBBMS_SERVICE_RET_E service_ret = MBBMS_SERVICE_FUNC_OK;
    
    //SCI_TRACE_LOW:"MMIAPIENG_MTVRFTest  net_frequency=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3432_112_2_18_2_17_16_42,(uint8*)"d", net_frequency);
    
    //RF测试的步骤是：按频率搜网，返回消息后，RSSI发生变化时，获取RSSI显示至屏幕
    
    s_display_mode = MMIENG_MTV_CHANNEL_RF_TEST;
    
    //stop flow first
    MMIAPIENG_StopMTVFlow();
    while(MMIMBBMS_FLOW_STATE_IDLE != s_curr_flow_info.flow_type)
    {
        SCI_Sleep(100);
    }
    
    //start cmmb program search
    s_curr_flow_info.flow_type = MMIMBBMS_FLOW_SG_MANUAL_UPDATE;
    s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_STARTING;
    service_ret = MTVSERVICE_ManualUpdateSG(MTVSERVICE_SEARCH_PROG_BY_FRQ, net_frequency);
    if(MBBMS_SERVICE_FUNC_OK != service_ret)
    {
        //SCI_TRACE_LOW:"MMIAPIENG_MTVRFTest  MTVSERVICE_Play return = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3451_112_2_18_2_17_16_43,(uint8*)"d", service_ret);
        s_curr_flow_info.flow_type = MMIMBBMS_FLOW_NONE;
        s_curr_flow_info.status    = MMIMBBMS_FLOW_STATE_IDLE;
        
        //play error tip
        MMIPUB_OpenAlertFailWin(TXT_MBBMS_SG_UPDATE_FAILED);
    }
}

/********************************************************************************
 NAME:			MMIAPIENG_GetMTVChannelDisplayMode
 DESCRIPTION:	获取状态信息
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC MMIMBBMS_FLOW_INFO_T* MMIAPIENG_GetMTVFlowInfo(void)
{
    return &s_curr_flow_info;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetPlayResult
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC MBBMS_SERVICE_RESULT_E MMIAPIENG_GetPlayResult(void)
{
    return s_play_result;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetMTVChannelDisplayMode
 DESCRIPTION:	获取Channel信息显示模式
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC MMIENG_MTV_CHANNEL_DISP_MODE_E MMIAPIENG_GetMTVChannelDisplayMode(void)
{
    return s_display_mode;
}

#endif

/********************************************************************************
 NAME:			MMIAPIENG_GetCallBarring
 DESCRIPTION:	MS00186848
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallBarring(void)
{
    BOOLEAN             is_on = FALSE;
    MN_RETURN_RESULT_E 	nv_value = MN_RETURN_FAILURE;
        
    MMINV_READ(MMIENG_NV_IS_CALL_BARRING, &is_on, nv_value);

    SCI_TRACE_LOW("MMIAPIENG_GetCallBarring  start  is_on=%d", is_on);

    if(MN_RETURN_SUCCESS != nv_value)
    {
        MMINV_WRITE(MMIENG_NV_IS_CALL_BARRING, &is_on);
    }

    SCI_TRACE_LOW("MMIAPIENG_GetCallBarring end  is_on=%d", is_on);

    return is_on;
}

/********************************************************************************
 NAME:			MMIAPIENG_SetCallBarring
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC void MMIAPIENG_SetCallBarring(
                                          BOOLEAN   is_on
                                          )
{
    SCI_TRACE_LOW("MMIAPIENG_SetCallBarring    is_on=%d", is_on);
    MMINV_WRITE(MMIENG_NV_IS_CALL_BARRING, &is_on);
}


/*****************************************************************************/
// 	Description : restore factory setting of ENG
//	Global resource dependence : 
//  Author: fen.xie
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIENG_RestoreFactorySetting(void)
{    
    /*
    MMIENG_UA_T ua = {0};
	
    SCI_STRCPY((void*)ua->user_agent, (void*)MMIAPICOM_GetUserAgent() );
    SCI_STRCPY((void*)ua->uaprofile, (void*)MMIAPICOM_GetUserAgentProfile() );
    MMIENG_SetUa(&ua);
    */
    //MMIAPIENG_SetChipTestMode(MMIENG_CHIP_TEST_SETTING_OFF);
    //MMIAPIENG_SetSensorMode(MMIENG_SENSOR_MODE_DEFAULT);
    //MMIAPIENG_SetDVFrameFre(MMIENG_DV_FRAME_FRE_7HZ);
    //MMIAPIENG_SetIsCardLogOn(FALSE);
    //MMIENG_SetUserAgentIsUseList(TRUE);
    //MMIENG_SetUaProfileIsUseList(TRUE);
    //MMIENG_SetUserAgentListNum(0);
    //MMIENG_SetUaProfileListNum(0);
    
    MMIAPIENG_SetCallBarring(FALSE);
    MMIAPIENG_SetCallForwardQuery(FALSE);
    MMIAPIENG_SetAutoAnswerCallSet(FALSE);

}

PUBLIC int32 MMIENG_OpenListSelectWin(uint32  handle_function)
{
    
    MMI_WINDOW_CREATE_T     win_create = {0};
    MMI_HANDLE_T            win_handle = 0;
    GUILIST_INIT_DATA_T     init_data = {0};
#ifndef MMI_PDA_SUPPORT
    GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    MMI_CTRL_ID_T   softkey_ctrl_id = 0;
#endif
    const uint32 win_id =      MMIENG_LIST_SELECT_WIN_ID;
    MMI_HANDLE_T list_handle = 0;
    if(0 == handle_function)
        return 1;
/*    
    if(MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(MMK_ConvertIdToHandle( win_id));
        return 2;
    }
  */  
    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = win_id;
    win_create.win_priority = WIN_ONE_LEVEL;
    win_create.add_data_ptr = 0 ;
    win_create.window_style = WS_HAS_TRANSPARENT;
    win_create.window_move_style = MOVE_BY_SETTING;          
    win_create.func = (PROCESSMSG_FUNC)handle_function;      
    win_handle = MMK_CreateWindow( &win_create );
    
    //create softkey
    if (0 == win_handle)
    {
        return 3;
    }              
    
#ifndef MMI_PDA_SUPPORT	//pda大多数场景不显示softkey
    softkey_data.leftsoft_id    = TXT_COMMON_OK;
    softkey_data.midsoft_id     = TXT_NULL;
    softkey_data.rightsoft_id   = STXT_RETURN;
    
    softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id = softkey_ctrl_id;
    softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = win_handle;
    softkey_create.init_data_ptr = &softkey_data;
    
    MMK_CreateControl( &softkey_create );
    MMK_SetWinSoftkeyCtrlId(win_handle, softkey_ctrl_id);
#endif
    
#ifdef MMI_PDA_SUPPORT
    GUIWIN_CreateStbDyn(win_handle);
#endif
    
    //create title    
    GUIWIN_CreateTitleDyna(win_handle, TXT_NULL);
    
    //create list       
    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_handle);           
    init_data.type = GUILIST_RADIOLIST_E;
    init_data.is_dynamic_item_data = FALSE;   
    
    list_handle = GUILIST_CreateListBox(win_handle, 0, MMIENG_LISTBOX_CTRL_ID, &init_data);

    return list_handle;    
}

#if 0
LOCAL MMI_RESULT_E EngDebugPortModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    int32 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    int32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};       
            GUILIST_SetMaxItem( list_id, 4, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
            
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ENG_PORT_AT;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ENG_PORT_DATA ;
            GUILIST_AppendItem( list_id, &item_t );    
            item_data.item_content[0].item_data.text_id = TXT_ENG_PORT_MIXED;    
            GUILIST_AppendItem( list_id, &item_t );
            
#ifndef WIN32
            index = SIO_GetUserPortMode();
#endif
            
            if ( 0 <= index && 3>= index )
            {   
                GUILIST_SetSelectedItem( list_id, index, TRUE );
                GUILIST_SetCurItemIndex( list_id, index);
            }
            else
            {
                GUILIST_SetSelectedItem( list_id, 0, TRUE );
                GUILIST_SetCurItemIndex( list_id, 0 );
            }     
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);
#ifndef WIN32
        if(0 == index)
        {      
            SIO_Close(COM_DATA);
        }
        else if( 3 >= index)
        {
            SIO_ChangeMode(COM_DATA);
            SIO_SetUserPortMode(index);    	    
        }
        else
        {    
            SIO_Close(COM_DATA);
        }	
#endif
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif

LOCAL MMI_RESULT_E EngDebugAssertModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};    
            
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_ASSERTMODE, FALSE);
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;     
            
            item_data.item_content[0].item_data.text_id = TXT_COMM_DEBUG_MODE;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ENG_RELEASEMODE;    
            GUILIST_AppendItem( list_id, &item_t );  
            
            OSPARAM_GetAssertMode((uint8*)&index);  
            if (SCI_ASSERT_DEBUG_MODE  ==  index)
            {   
                GUILIST_SetSelectedItem( list_id, 0, TRUE );
                GUILIST_SetCurItemIndex( list_id, 0);
            }
            else
            {
                GUILIST_SetSelectedItem( list_id, 1, TRUE );
                GUILIST_SetCurItemIndex( list_id, 1 );
            }     
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);         
        
        if(0 == index)
        {             
            OSPARAM_SetAssertMode(SCI_ASSERT_DEBUG_MODE); 	//debug mode
        }
        else if( 1  >= index)
        {
            OSPARAM_SetAssertMode(SCI_ASSERT_RELEASE_MODE);    
        }
        else
        {
            OSPARAM_SetAssertMode(SCI_ASSERT_DEBUG_MODE);    
        }   
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);    
}

#ifdef MMI_WIFI_SUPPORT
/********************************************************************************
 NAME:          EngFlightModeWiFiModeOnOffWinHandleMsg
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:
 AUTHOR:        zhenpeng.yao
 DATE:          2020.11.25
********************************************************************************/
LOCAL MMI_RESULT_E EngFlightModeWiFiModeOnOffWinHandleMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    BOOLEAN flag = 0; // 0:OPEN; 1: CLOSE

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_WIFI_FLIGHTMODE, FALSE);
            GUILIST_SetMaxItem(list_id, 2, FALSE);
            GUILIST_SetOwnSofterKey(list_id, FALSE);
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;
            GUILIST_AppendItem(list_id, &item_t);
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;
            GUILIST_AppendItem(list_id, &item_t);
            flag = MMIAPIENG_GetIsFlightModeWifi();
            if (TRUE == flag)
            {
                index = 0;
            }
            else
            {
                index = 1;
            }

            GUILIST_SetSelectedItem(list_id, index, TRUE);
            GUILIST_SetCurItemIndex(list_id, index);
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);
        SCI_TRACE_LOW("EngFlightModeWiFiModeOnOffWinHandleMsg index=%d",index);
        if (0 == index)
        {
            MMIAPIENG_SetIsFlightModeWifi(TRUE);
        }
        else
        {
            MMIAPIENG_SetIsFlightModeWifi(FALSE);
        }

        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif

/********************************************************************************
 NAME:          SetFlyMode
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:
 AUTHOR:        zhenpeng.yao
 DATE:          2021.4.14
********************************************************************************/
LOCAL void SetFlyMode(BOOLEAN is_fly_mode_on)
{
    uint32 i  = 0;
    if (is_fly_mode_on)
    {
        uint32 dual_sys =0;
        BOOLEAN is_ps_ready = TRUE;
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
        for (dual_sys = 0; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
        {
            if ((e_dualsys_setting & (0x01 << dual_sys))
            && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
            {
                is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);
                if (!is_ps_ready)
                {
                    break;
                }
            }
        }
        if (is_ps_ready)
        {
            MMIAPISET_SetFlyMode(is_fly_mode_on);
            ual_tele_radio_set_fly_mode(is_fly_mode_on);
            MMIAPIPHONE_DeactivePsAccordingToSetting();
        }
        else
        {
            SCI_TRACE_LOW("SetFlyMode open fail");
        }
    }
    else
    {
        BOOLEAN is_ps_deactive = TRUE;
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
            {
                is_ps_deactive = FALSE;
                break;
            }
        }
        if (is_ps_deactive)
        {
            MMIAPISET_SetFlyMode(is_fly_mode_on);
            // 激活协议栈
            ual_tele_radio_set_fly_mode(is_fly_mode_on);
            MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
        }
        else
        {
            SCI_TRACE_LOW("SetFlyMode close fail");
        }
    }
}

/********************************************************************************
 NAME:          EngFlightModeOnOffWinHandleMsg
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:
 AUTHOR:        zhenpeng.yao
 DATE:          2021.4.14
********************************************************************************/
LOCAL MMI_RESULT_E EngFlightModeOnOffWinHandleMsg( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    BOOLEAN flag = 0; // 0:OPEN; 1: CLOSE

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_FLIGHT_MODE, FALSE);
            GUILIST_SetMaxItem(list_id, 2, FALSE);
            GUILIST_SetOwnSofterKey(list_id, FALSE);
            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;
            GUILIST_AppendItem(list_id, &item_t);
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;
            GUILIST_AppendItem(list_id, &item_t);
            flag = MMIAPISET_GetFlyMode();
            if (TRUE == flag)
            {
                index = 0;
            }
            else
            {
                index = 1;
            }

            GUILIST_SetSelectedItem(list_id, index, TRUE);
            GUILIST_SetCurItemIndex(list_id, index);
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;

    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);
        SCI_TRACE_LOW("EngFlightModeOnOffWinHandleMsg index=%d",index);
        if (0 == index)
        {
            SetFlyMode(TRUE);
        }
        else
        {
            SetFlyMode(FALSE);
        }

        MMK_CloseWin(win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
/********************************************************************************
 NAME:          EngPasswordWinHandleMsg
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:
 AUTHOR:        siyuan.fei
********************************************************************************/
LOCAL MMI_RESULT_E EngPasswordWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
        uint16 index = 0;
        MMI_RESULT_E result = MMI_RESULT_TRUE;
        uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
        BOOLEAN flag = 0; // 0:OPEN; 1: CLOSE

        switch (msg_id)
        {
            case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};
                GUIWIN_SetTitleTextId(win_id, TXT_ENG_PASSWORD_SET, FALSE);
                GUILIST_SetMaxItem(list_id, 2, FALSE);
                GUILIST_SetOwnSofterKey(list_id, FALSE);
                item_t.item_style = GUIITEM_STYLE_1RADIO_ENG;
                item_t.item_data_ptr = &item_data;
                item_data.item_content[0].item_data_type	= GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;
                GUILIST_AppendItem(list_id, &item_t);
                item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;
                GUILIST_AppendItem(list_id, &item_t);
                flag = MMIAPISET_GetPowerOnPwdStatus();
                if (TRUE == flag)
                {
                    index = 0;
                }
                else
                {
                    index = 1;
                }

                GUILIST_SetSelectedItem(list_id, index, TRUE);
                GUILIST_SetCurItemIndex(list_id, index);
                MMK_SetAtvCtrl(win_id, list_id);
            }
            break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            index = GUILIST_GetCurItemIndex(list_id);
            SCI_TRACE_LOW("EngPasswordWinHandleMsg index=%d",index);
            if (0 == index)
            {
                MMISET_SetPowerOnPwdStatus(TRUE);
            }
            else
            {
                MMISET_SetPowerOnPwdStatus(FALSE);
            }

            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
        }

        return (result);
}

LOCAL MMI_RESULT_E EngRFSendPowerOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    static int32 max_power_send_flag = 1; // 1:OFF; 0: ON    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};       

            GUIWIN_SetTitleTextId(win_id, TXT_ENG_SEND_POWER, FALSE);            
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;         
            
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );                      
            
            GUILIST_SetSelectedItem( list_id,max_power_send_flag, TRUE );
            GUILIST_SetCurItemIndex( list_id, max_power_send_flag);
            
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);              
        if(0 == index)
        {             
            max_power_send_flag = 0;
 //           L1API_ApplyMaxPower();
        }
        else if( 1  >= index)
        {
            max_power_send_flag = 1;  
//            L1API_ApplyMaxPower();
        }
        else
        {
            max_power_send_flag = 1; 
 //           L1API_ApplyMaxPower();
        }   
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);    
    
}

LOCAL MMI_RESULT_E EngARMLOGOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    int flag = 0; // 0:OFF; 1: ON    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};       
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_ARM_LOG, FALSE);
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
            
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );                      
#ifndef WIN32           
#if defined (PLATFORM_ANTISW3)
          flag = ulog_get_log(1);// 1 代表 ULOG_MODEM
#elif defined (PLATFORM_UWS6121E)
          flag = SCI_GetArmLogFlag();    
#endif
          //SCI_TRACE_LOW:"SCI_GetArmLogFlag, retrun = %d"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3904_112_2_18_2_17_17_44,(uint8*)"d", flag);
#endif    
            if( 0 == flag)
            {
                index = 1;
            }
            else
            {
                index = 0;
            }
            GUILIST_SetSelectedItem( list_id,index, TRUE );
            GUILIST_SetCurItemIndex( list_id, index);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);         
         //SCI_TRACE_LOW:"SCI_SetArmLogFlag, index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3934_112_2_18_2_17_17_45,(uint8*)"d", index);
#if defined (PLATFORM_ANTISW3)
        if(0 == index)
        {
#ifndef WIN32
            ulog_set_log( ULOG_MODEM,1);
#endif
        }
        else if( 1  == index)
        {
#ifndef WIN32
            ulog_set_log( ULOG_MODEM,0);
#endif
        }
        else
        {
#ifndef WIN32
            ulog_set_log( ULOG_MODEM,0);
#endif
        }   
#elif defined (PLATFORM_UWS6121E)
        if(0 == index)
        {     
            SCI_SetArmLogFlag(TRUE);
        }
        else if( 1  == index)
        {
            SCI_SetArmLogFlag(FALSE);
        }
        else
        {
            SCI_SetArmLogFlag(FALSE);
        }   
#else
    #error
#endif        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);        
}

LOCAL MMI_RESULT_E EngDSPLOGSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    int32 flag = 1; // 0:OFF; 1: ON    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};     
            
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_DSP_LOG, FALSE);
            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;
            
#if !defined (PLATFORM_ANTISW3)  
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);           
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );                   
#ifndef WIN32           
          flag = SCI_GetModemLogFlag();    
#endif    
	     index = !flag;
#if 0
            if( 0 == flag)
            {
                index = 1;
            }
            else
            {
                index = 0;
            }
#endif			
#else
            GUILIST_SetMaxItem( list_id, 3, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ENG_UART;    
            GUILIST_AppendItem( list_id, &item_t );                 
            item_data.item_content[0].item_data.text_id = TXT_ENG_USB;    
            GUILIST_AppendItem( list_id, &item_t );    
            
#ifndef WIN32        
#ifdef DSP_USB_LOG                   
            //Returns the dsp log setting //0.Disable  1. COM1  2.ARM USB
            index = (uint16)(REFPARAM_GetDspDebugPortPhyNo());    
            //SCI_TRACE_LOW:"REFPARAM_GetDspDebugPortPhyNo, retrun = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_3991_112_2_18_2_17_17_46,(uint8*)"d", index);
#endif  
#endif                
#endif
            GUILIST_SetSelectedItem( list_id,index, TRUE );
            GUILIST_SetCurItemIndex( list_id, index);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);         
        //[IN]The  value to be set.  0.Disable  1. COM1  2.ARM USB
#ifndef WIN32                     
#if !defined (PLATFORM_ANTISW3)  
        if(0 == index)			
        {   
       	 //SCI_TRACE_LOW("mymark EngDSPLOGSetWinHandleMsg index%d",index);
	 	SCI_SetModemLogFlag(TRUE);     
        }
        else //if( 1  == index)
        {
         	//SCI_TRACE_LOW("mymark EngDSPLOGSetWinHandleMsg index%d",index);
	  	SCI_SetModemLogFlag(FALSE);	             
        }
#else
#ifdef DSP_USB_LOG 
        REFPARAM_SetDspDebugPortPhyNo((uint8)index);     
        //SCI_TRACE_LOW:"REFPARAM_SetDspDebugPortPhyNo, index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_4018_112_2_18_2_17_17_47,(uint8*)"d", index);
#endif        
#endif           
#endif           
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);    
    
}


LOCAL MMI_RESULT_E EngSleepModeOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    uint16 flag = 0;     
#ifndef WIN32

	int sleep_item_ary[4] = {ENABLE_PM0 , ENABLE_PM1 , ENABLE_PM2 , DISABLE_SLEEP};

#if 0
typedef enum
{
    ENABLE_PM0,
    ENABLE_PM1,
    DISABLE_SLEEP,
	ENABLE_PM2,
	ENABLE_PM3,
    SLEEP_TYPE_MAX
} SLP_SLEEP_TYPE_E;
#endif
#endif
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};  
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_SLEEP_MODE, FALSE);
            GUILIST_SetMaxItem( list_id, 4, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;   
            
            item_data.item_content[0].item_data.text_id = TXT_ENG_SLEEP_ENABLE_PM0;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ENG_SLEEP_ENABLE_PM1;    
            GUILIST_AppendItem( list_id, &item_t );
			item_data.item_content[0].item_data.text_id = TXT_ENG_SLEEP_ENABLE_PM2;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );                      
#ifndef WIN32            
            flag = SCI_ENGINEER_GetEnableArmSleepFlag();               

	     for(index = 0 ; index < 4 ; index ++)
	     {
			if(flag == sleep_item_ary[index])
				break;

	     }
#if 0		 
	     switch(flag)
	     {
			case ENABLE_PM0:
				index = 0 ;
				break;
			case ENABLE_PM1:
				index = 1 ;
				break;
			case ENABLE_PM2:
				index = 2 ;
				break;

			case DISABLE_SLEEP:
			default:
				index = 3;
				break;
            }
#endif
#endif
            GUILIST_SetSelectedItem( list_id,index, TRUE );
            GUILIST_SetCurItemIndex( list_id, index);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);      
#ifndef WIN32
	SCI_ENGINEER_EnableDeepSleep(sleep_item_ary[index]);
#if 0
        if(0 == index)
        {     
            SCI_ENGINEER_EnableDeepSleep(ENABLE_PM0);
        }
        else if( 1  == index)
        {
            SCI_ENGINEER_EnableDeepSleep(ENABLE_PM1);
        }
        else if( 2  == index)
        {
            SCI_ENGINEER_EnableDeepSleep(ENABLE_PM2);
        }
        else
        {
            SCI_ENGINEER_EnableDeepSleep(DISABLE_SLEEP);
        }   
#endif        
#endif        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);  
}

LOCAL MMI_RESULT_E EngAutoPoweronOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    BOOLEAN flag = 0; // 0:OFF; 1: ON    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};       
            
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_AUTO_POWERON, FALSE);
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
 
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );                      
            
            flag = REFPARAM_GetAutoPowerOnFlag();    
            if( 0 == flag)
            {
                index = 1;
            }
            else
            {
                index = 0;
            }
            GUILIST_SetSelectedItem( list_id,index, TRUE );
            GUILIST_SetCurItemIndex( list_id, index);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);              
        if(0 == index)
        {     
            REFPARAM_SetAutoPowerOnFlag(TRUE);
        }
        else if( 1  == index)
        {
            REFPARAM_SetAutoPowerOnFlag(FALSE);
        }
        else
        {
            REFPARAM_SetAutoPowerOnFlag(FALSE);
        }   
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);    
    
}

/*****************************************************************************/
// Description : OpenGPRSOnOffWin
// Global resource dependence :
// Author: shuting.ma
// Note:bug 2114304
/*****************************************************************************/
LOCAL void OpenGPRSOnOffWin(uint16 index, uint32 list_id, uint32 win_id)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
#ifdef MMI_GPRS_SWITCH_SUPPORT
    MMICONNECTION_SETTING_GPRS_SWITCH_E flag = 0; // 0:OFF; 1: ON
#endif

    GUIWIN_SetTitleTextId(win_id, TXT_GPRS, FALSE);
    GUILIST_SetMaxItem( list_id, 2, FALSE );
    GUILIST_SetOwnSofterKey(list_id,FALSE);

    item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;

    item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;
    GUILIST_AppendItem( list_id, &item_t );
    item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;
    GUILIST_AppendItem( list_id, &item_t );

#ifdef MMI_GPRS_SUPPORT
    //flag = MNPHONE_GetServiceTypeEx(MMIAPISET_GetActiveSim());
    if(0 == MMIAPIPHONE_GetSimAvailableNum(NULL,0))
    {
        index = 1;
    }
    else
    {
#ifdef MMI_GPRS_SWITCH_SUPPORT
    //michael wang
    flag = MMIAPICONNECTION_GetGPRSSwitchStatus();

    if(MMICONNECTION_SETTING_GPRS_SWITCH_ON == flag)
    {
        index = 0;
    }
    else if(MMICONNECTION_SETTING_GPRS_SWITCH_OFF == flag)
    {
        index = 1;
    }
#endif
    }
#else
    index = 1; //no gprs
#endif
    /*
    if( 1 >=  flag)
    {
        index = 1;
    }
    else
    {
        index = 0;
    }
    */
    GUILIST_SetSelectedItem( list_id,index, TRUE );
    GUILIST_SetCurItemIndex( list_id, index);
    MMK_SetAtvCtrl(win_id, list_id);
}

LOCAL MMI_RESULT_E EngGPRSOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{    
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
#ifndef WIN32    
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
#endif
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            OpenGPRSOnOffWin(index, list_id, win_id); //bug 2114304
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);              
        if(0 == index)
        {     
#ifdef MMI_GPRS_SWITCH_SUPPORT
#ifndef WIN32
            for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
            {
#ifdef MODEM_SUPPORT_SPRD
                MNPHONE_SetServiceTypeEx(dual_sys, MN_PHONE_USER_SELECT_GSM_GPRS_BOTH);
                MNPHONE_GprsAttachEx(dual_sys, MN_PHONE_GPRS_ATTACH);
#endif
                MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_ON); //michael wang
            }
#endif
#endif
        }
        else if( 1  == index)
        {
#ifdef MMI_GPRS_SWITCH_SUPPORT
#ifndef WIN32
            for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
            {
#ifdef MODEM_SUPPORT_SPRD
                MNPHONE_SetServiceTypeEx(dual_sys, MN_PHONE_USER_SELECT_GSM_ONLY);
                MNPHONE_GprsDetachEx(dual_sys, MN_PHONE_GPRS_DETACH);
#endif
                MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_OFF);    //michael wang
            }
#endif
#endif
        }
        else
        {            
#ifndef WIN32
            for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
            {
                MNPHONE_SetServiceTypeEx(dual_sys, MN_PHONE_USER_SELECT_GSM_ONLY);
                MNPHONE_GprsDetachEx(dual_sys, MN_PHONE_GPRS_DETACH);
            }
#endif
        }   
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);    
}

LOCAL MMI_RESULT_E EnCallForwardQueryOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
            uint16 index = 0;
            MMI_RESULT_E result = MMI_RESULT_TRUE;
            uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
            BOOLEAN flag = 0; // 0:OFF; 1: ON    
            
            switch (msg_id) 
            {
            case MSG_OPEN_WINDOW:
                {
                    GUILIST_ITEM_T      item_t = {0};
                    GUILIST_ITEM_DATA_T item_data = {0};       

                    GUIWIN_SetTitleTextId(win_id, TXT_ENG_CALLFORWARD_QUERY, FALSE);
                    
                    GUILIST_SetMaxItem( list_id, 2, FALSE );
                    GUILIST_SetOwnSofterKey(list_id,FALSE);
                    
                    item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
                    item_t.item_data_ptr = &item_data;
                    
                    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;       
                    
                    item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
                    GUILIST_AppendItem( list_id, &item_t );
                    item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
                    GUILIST_AppendItem( list_id, &item_t );                      
                    
                    flag = MMIAPIENG_GetCallForwardQuery();    
                    if( 1 ==  flag)
                    {
                        index = 0;
                    }
                    else
                    {
                        index = 1;
                    }
                    GUILIST_SetSelectedItem( list_id,index, TRUE );
                    GUILIST_SetCurItemIndex( list_id, index);          
                    MMK_SetAtvCtrl(win_id, list_id);
                }
                break;
                
            case MSG_FULL_PAINT:
                break;      
                
            case MSG_KEYDOWN_CANCEL:
            case MSG_CTL_CANCEL:
                MMK_CloseWin(win_id);
                break;
                
            case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
            case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            case MSG_KEYDOWN_OK:
            case MSG_CTL_OK:
            case MSG_APP_WEB:
                index = GUILIST_GetCurItemIndex(list_id);              
                if(0 == index)
                {  
                 MMIAPIENG_SetCallForwardQuery(TRUE);
                }
                else if( 1  == index)
                {
                  MMIAPIENG_SetCallForwardQuery(FALSE);
                }
                else
                {
                    MMIAPIENG_SetCallForwardQuery(FALSE);
                }   
                
                MMK_CloseWin(win_id);
                break;
                
            default:
                result = MMI_RESULT_FALSE;
                break;
            }
            return (result);    
}

LOCAL MMI_RESULT_E EngHiddenPartitionVisibleOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    BOOLEAN flag = 0; // 0:OFF; 1: ON    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};      
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_HIDDEN_PARTITION_VISIBLE, FALSE);
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;              
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );                      
#ifndef WIN32   
#if !defined(PLATFORM_UWS6121E)
            flag = UBOT_GetDiskHiddenState((uint8*)STL_UDISK_HIDDEN_FS_PART);   
#endif
#endif
            GUILIST_SetSelectedItem( list_id,flag, TRUE );
            GUILIST_SetCurItemIndex( list_id, flag);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);              
        if(0 == index)
        { 
#if !defined(PLATFORM_UWS6121E)
#ifndef WIN32      
            UBOT_SetDiskHidden((uint8*)STL_UDISK_HIDDEN_FS_PART, FALSE);	
#endif 
#endif
#if (defined JAVA_SUPPORT_IA) && (defined SELECT_INSTALL_DISK_SUPPORT)
            MMIAPIJAVA_SetSysDiskHiddenOn();
#endif
        }
        else 
        { 
#if !defined(PLATFORM_UWS6121E)
#ifndef WIN32
            UBOT_SetDiskHidden((uint8*)STL_UDISK_HIDDEN_FS_PART, TRUE);     
#endif
#endif
#if (defined JAVA_SUPPORT_IA) && (defined SELECT_INSTALL_DISK_SUPPORT)
            MMIAPIJAVA_SetSysDiskHiddenOff();
#endif
        }        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);    
}

#ifdef JAVA_SUPPORT_IA
LOCAL MMI_RESULT_E EngJAVAAutoTCKOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
        uint16 index = 0;
        MMI_RESULT_E result = MMI_RESULT_TRUE;
        uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
        BOOLEAN flag = 0; // 0:OFF; 1: ON    
        
        switch (msg_id) 
        {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};       

                GUIWIN_SetTitleTextId(win_id, TXT_ENG_JAVA_AUTO_TCK_MODE, FALSE);
                
                GUILIST_SetMaxItem( list_id, 2, FALSE );
                GUILIST_SetOwnSofterKey(list_id,FALSE);
                
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
                item_t.item_data_ptr = &item_data;
                
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
                
                item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
                GUILIST_AppendItem( list_id, &item_t );
                item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
                GUILIST_AppendItem( list_id, &item_t );                      
               
                flag = MMIAPIJAVA_GetJavaAutoTckState();
                    if( 1 ==  flag)
                    {
                        index = 0;
                    }
                    else
                    {
                        index = 1;
                    }             
                GUILIST_SetSelectedItem( list_id,index, TRUE );
                GUILIST_SetCurItemIndex( list_id, index);          
                MMK_SetAtvCtrl(win_id, list_id);
            }
            break;
            
        case MSG_FULL_PAINT:
            break;      
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            index = GUILIST_GetCurItemIndex(list_id);              
            if(0 == index)
            {  
                MMIAPIJAVA_SetJavaAutoTckOn();
            }
            else 
            {
                MMIAPIJAVA_SetJavaAutoTckOff();
            }
                    
            MMK_CloseWin(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
        }
        return (result); 
}
#endif

LOCAL MMI_RESULT_E EngDCDUpdateLOGOnOffWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
        uint16 index = 0;
        MMI_RESULT_E result = MMI_RESULT_TRUE;
        uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
        BOOLEAN flag = 0; // 0:OFF; 1: ON    
        
        switch (msg_id) 
        {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};       
                
                GUIWIN_SetTitleTextId(win_id, TXT_ENG_DCD_NOTE, FALSE);       
                
                GUILIST_SetMaxItem( list_id, 2, FALSE );
                GUILIST_SetOwnSofterKey(list_id,FALSE);
                
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
                item_t.item_data_ptr = &item_data;
                
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
                
                item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
                GUILIST_AppendItem( list_id, &item_t );
                item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
                GUILIST_AppendItem( list_id, &item_t );                      
#ifdef MMI_ENABLE_DCD    
                flag = MMIAPIDCD_GetUpdateLogOnOff();
                if( 1 ==  flag)
                {
                    index = 0;
                }
                else
                {
                    index = 1;
                }
#else
                flag = TRUE;
                index = 0;
#endif            
                GUILIST_SetSelectedItem( list_id,index, TRUE );
                GUILIST_SetCurItemIndex( list_id, index);          
                MMK_SetAtvCtrl(win_id, list_id);
            }
            break;
            
        case MSG_FULL_PAINT:
            break;      
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            index = GUILIST_GetCurItemIndex(list_id);              
            if(0 == index)
            { 
#ifdef MMI_ENABLE_DCD
                MMIAPIDCD_SetUpdateLogOnOff(TRUE);
#endif

            }
            else 
            {
#ifdef MMI_ENABLE_DCD
                  MMIAPIDCD_SetUpdateLogOnOff(FALSE);
#endif
            }
                    
            MMK_CloseWin(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
        }
        return (result);        
}

#ifdef PLATFORM_SC6800H
LOCAL MMI_RESULT_E EngPLMNSelectWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    BOOLEAN flag = 0; // 0:OFF; 1: ON    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {                          
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};  
            
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_PLMN_SELECT, FALSE);
            
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
            
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );    
#ifndef WIN32
            {
            BOOLEAN call_result = 0;
            call_result = MNNV_GetNVPlmnSupportFlagEx(s_plmn_sim_index - 1,  &flag);
            //SCI_TRACE_LOW:"MNNV_GetNVPlmnSupportFlagEx: sim index = %d, flag = %d, return=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_4654_112_2_18_2_17_18_48,(uint8*)"ddd",s_plmn_sim_index, flag, call_result);
            }
#endif

            if( TRUE ==  flag)
            {
                index = 0;
            }
            else
            {
                index = 1;
            }     
            GUILIST_SetSelectedItem( list_id,index, TRUE );
            GUILIST_SetCurItemIndex( list_id, index);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);              
        if(0 == index)
        { 
            flag = TRUE;
        }
        else 
        {
            flag = FALSE;
        }
#ifndef WIN32
        {
            BOOLEAN call_result = 0;
            call_result = MNNV_SetNVPlmnSupportFlagEx(s_plmn_sim_index - 1,  flag);
            //SCI_TRACE_LOW:"MNNV_SetNVPlmnSupportFlagEx: sim index = %d, flag = %d, return=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_4699_112_2_18_2_17_19_49,(uint8*)"ddd",s_plmn_sim_index, flag, call_result);
        }       
#endif        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);        
}
#endif

LOCAL MMI_RESULT_E EngStandbyWinOnOffHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint16 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    uint32 flag = 0; // 0:OFF; 1: ON    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};       
            
            GUIWIN_SetTitleTextId(win_id, TXT_ENG_SHOW_STANDBY_TIME, FALSE);
            
            GUILIST_SetMaxItem( list_id, 2, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_1RADIO_ENG;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
            
            item_data.item_content[0].item_data.text_id = TXT_OPEN_COMN;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_COMM_CLOSE;    
            GUILIST_AppendItem( list_id, &item_t );                      
            
            flag = MMIENG_GetStanbyRecordEnableState();
            if( 1 ==  flag)
            {
                index = 0;
            }
            else
            {
                index = 1;
            }             
            GUILIST_SetSelectedItem( list_id,index, TRUE );
            GUILIST_SetCurItemIndex( list_id, index);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);              
        if(0 == index)
        {  
            MMIENG_EnableAndResetStanbyRecord(1);
        }
        else 
        {
            MMIENG_EnableAndResetStanbyRecord(0);
        }
        
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result); 
}

/********************************************************************************
 NAME:			IsDCAllowedToBeOpened
 DESCRIPTION:	
 AUTHOR:		robert wang
 DATE:			2011.06.12
********************************************************************************/
LOCAL BOOLEAN IsDCAllowedToBeOpened(void)
{	
#ifdef CAMERA_SUPPORT
    {
        //开启IQ时由于内存冲突，不做DC判断。（DC sensor判断需要分内存）
        BOOLEAN is_allowed = TRUE;
        
        if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            is_allowed = FALSE;
        }
        else if (MMIAPIENG_GetIQModeStatus())
        {
            MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
            is_allowed = FALSE;
        }
        //QQ运行后，内存不足，进入DC要提示QQ退出
#ifdef QQ_SUPPORT
        else if (QQ_IsRunning())
        {
            MMIPUB_OpenAlertWarningWin( TXT_EXIT_QQ);
            is_allowed = FALSE;
        }
#endif
        //JAVA 后台运行,内存不足，提示退出
#ifdef JAVA_SUPPORT
        else if (MMIAPIJAVA_IsJavaRuning())
        {
            MMIPUB_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
            is_allowed = FALSE;
        }
#endif              
        else
        {
            DCAMERA_IMG_SENSOR_TYPE_E sensor_type = 0;
            DCAMERA_IMG_SENSOR_TYPE_E sensor_type1 = 0;
            int32 sensor_number = 0;

            sensor_number = MMIAPIDC_GetDCModuleSensorNumber();
            if (sensor_number < MAX_MMI_SENSOR_NUM)
            {
                sensor_type = MMIAPIDC_GetDCModulSensorType();
                if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type)
                {
                    is_allowed = FALSE;
                }
            }
            else
            {
                sensor_type = MMIAPIDC_GetDCModulSensorTypeById(SETTING_SENSOR_REAR);
                sensor_type1 = MMIAPIDC_GetDCModulSensorTypeById(SETTING_SENSOR_FRONT);
                if (DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type
                    && DCAMERA_IMG_SENSOR_TYPE_3P4 == sensor_type1)
                {
                    is_allowed = FALSE;
                }
            }            
        }
        return is_allowed;
    }
#else
    return FALSE;
#endif
}

/********************************************************************************
//MS00216980 cheney
 NAME:			BOOLEAN MMIAPIENG_GetCMCCTestFlag
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
PUBLIC  BOOLEAN MMIAPIENG_GetCMCCTestFlag(void)
{
	BOOLEAN test_flag = FALSE;
	MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
	
	MMINV_READ(MMIENG_NV_CMCC_TEST_FLAG, &test_flag, return_value);
	 if (return_value!=MN_RETURN_SUCCESS)
	 {
#ifdef CMCC_TEST_FLAG
		test_flag = TRUE;
#else
		test_flag = FALSE;
#endif
		MMINV_WRITE(MMIENG_NV_CMCC_TEST_FLAG, &test_flag);
	 }
	 //SCI_TRACE_LOW:"[mmieng]GET cmcc test flag = %d"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_4867_112_2_18_2_17_19_50,(uint8*)"d", test_flag);
	 return test_flag;
}

/********************************************************************************
//MS00216980 cheney
 NAME:			BOOLEAN MMIAPIENG_SetCMCCTestFlag
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetCMCCTestFlag(BOOLEAN *pTestFlag)
{
#ifdef PIM_SUPPORT
    BOOLEAN testFlag = FALSE;
    const server_setting_info_type nv_sync_syncml_info = {"./contact", "./calendar", PIM_SRV_ADDR_URL, "80", "", ""};
    const server_setting_info_type nv_sync_syncml_info_test = {"./contact", "./calendar", PIM_SRV_ADDR_URL_TEST, "80", "", ""};

    if(pTestFlag == PNULL)
    {
        //SCI_TRACE_LOW:"mmieng_main.c MMIAPIENG_SetCMCCTestFlag pTestFlag == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_4887_112_2_18_2_17_19_51,(uint8*)"");
        return FALSE;
    }
     
    testFlag = *pTestFlag;

    //SCI_TRACE_LOW:"[mmieng]SET cmcc test flag = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_4893_112_2_18_2_17_19_52,(uint8*)"d", *pTestFlag);
    MMINV_WRITE(MMIENG_NV_CMCC_TEST_FLAG,  pTestFlag);

    /*-----------------------PIM------------------------------*/
    if(testFlag)
        MMIAPIPIM_SetServerConfig(&nv_sync_syncml_info_test); /*lint !e605*/ 
    else
        MMIAPIPIM_SetServerConfig(&nv_sync_syncml_info);/*lint !e605*/ 
	
#endif
    return TRUE;
}

#ifdef TP_TEST_SUPPORT
/*****************************************************************************/
//    Description : TP test
//    Global resource dependence : 
//    Author:   guodong.liu
//    Note:
/*****************************************************************************/
typedef struct
{
    MMI_CTRL_ID_T   TP_EDITBOX_ID;
    wchar*          w_text;
    
}TP_PARAM;

LOCAL TP_PARAM TP_PAINT_PARAM[] = {
    {TP_WIDTH_EDITBOX_CTRL_ID,           L"tp_width           0x200"},
    {TP_HEIGHT_EDITBOX_CTRL_ID,          L"tp_height          0x201"},
    {REFERENCE_RESISTOR_EDITBOX_CTRL_ID, L"reference resistor 0x202"},
    {REFERENCE_VOLTAGE_EDITBOX_CTRL_ID,  L"reference voltage  0x203"},
    {DPOINT_XLIMIT_EDITBOX_CTRL_ID,      L"dpoint_xlimit      0x204"},
    {DPOINT_YLIMIT_EDITBOX_CTRL_ID,      L"dpoint_ylimit      0x205"},
    {SPOINT_RT_MAX_EDITBOX_CTRL_ID,      L"spoint_rt_max      0x206"},
    {SPOINT_RT_MIN_EDITBOX_CTRL_ID,      L"spoint_rt_min      0x207"},
    {DPOINT_RT_MAX_EDITBOX_CTRL_ID,      L"dpoint_rt_max      0x208"},
    {DPOINT_RT_MIN_EDITBOX_CTRL_ID,      L"dpoint_rt_min      0x209"},
    {DOUBLE_POINT_EDITBOX_CTRL_ID,       L"dpoint_enable      0x210"}
    
};

LOCAL MMI_RESULT_E ENGTpTestWinHandleMsg(
											MMI_WIN_ID_T      win_id, 	
											MMI_MESSAGE_ID_E  msg_id,
											DPARAM            param
													   )//set and get param win

{   

   
	MMI_RESULT_E            recode     = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	    lcd_dev_info = {GUI_MAIN_LCD_ID};
	GUI_RECT_T              rect = {0, 0, 0, 0};
	GUISTR_STYLE_T          text_style = {0};
	GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN;
	MMI_STRING_T            text_str  = {0};  
	GUI_POINT_T             start_point = {0,0};
    uint32                   i = 0;
	text_style.align        = ALIGN_LEFT;
    #ifdef MAINLCD_SIZE_320X480
    text_style.font         = SONG_FONT_19;
    #else
    text_style.font         = SONG_FONT_14;
    #endif
    text_style.font_color   = MMI_BLACK_COLOR;
    MMK_GetWinRect(win_id, &rect);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T btn_get_rc  = TP_TEST_BUTTON_GET_RC;        
            GUI_RECT_T btn_set_rc  = TP_TEST_BUTTON_SET_RC;
            GUI_RECT_T btn_test_rc = TP_TEST_BUTTON_DRAW_RC;
            GUI_RECT_T editbox_rc  = TP_TEST_EDITBOX_RC;
            GUI_BG_T   bg_btn = {0};        
            GUI_FONT_ALL_T  button_font_all = {0};//button的字体与颜色
            
            btn_get_rc.left = rect.left; //set button posision 
            btn_get_rc.right = rect.left+TP_TEST_BUTTON_WIDTH;
            btn_get_rc.bottom = rect.bottom;
            btn_get_rc.top = btn_get_rc.bottom - TP_TEST_BUTTON_HEIGHT;

            btn_set_rc.left = (rect.right-1)/2-1-TP_TEST_BUTTON_WIDTH/2;  // center 
            btn_set_rc.right = (rect.right-1)/2-1+TP_TEST_BUTTON_WIDTH/2; // center
            btn_set_rc.bottom = btn_get_rc.bottom;
            btn_set_rc.top = btn_get_rc.top;

            btn_test_rc.left = rect.right-TP_TEST_BUTTON_WIDTH;
            btn_test_rc.right = rect.right;
            btn_test_rc.bottom = btn_get_rc.bottom;
            btn_test_rc.top = btn_get_rc.top;

            editbox_rc.left = rect.right-TP_TEST_EDITBOX_WIDTH;
            editbox_rc.right = rect.right;        
            button_font_all.font  = SONG_FONT_16 ;
            button_font_all.color =  MMI_BLACK_COLOR;        
 	        bg_btn.bg_type = GUI_BG_COLOR;
	        bg_btn.color = MMI_DARK_WHITE_COLOR;
	        GUIBUTTON_SetRunSheen( TP_GET_BUTTON_CTRL_ID, TRUE);//btn_get
            GUIBUTTON_SetPressedBg(TP_GET_BUTTON_CTRL_ID, &bg_btn);
            GUIBUTTON_SetFont(TP_GET_BUTTON_CTRL_ID, &button_font_all);           
            GUIBUTTON_SetTextId(TP_GET_BUTTON_CTRL_ID, TXT_TP_ASDF);
            GUIBUTTON_SetRect(TP_GET_BUTTON_CTRL_ID, &btn_get_rc);
            GUIBUTTON_SetBg(TP_GET_BUTTON_CTRL_ID, &bg_btn);
            GUIBUTTON_SetCallBackFunc(TP_GET_BUTTON_CTRL_ID, GetParamEditbox);//the button get param of TP
            
 	        GUIBUTTON_SetRunSheen( TP_SET_BUTTON_CTRL_ID, TRUE);//btn_set
            GUIBUTTON_SetPressedBg(TP_SET_BUTTON_CTRL_ID, &bg_btn);           
            GUIBUTTON_SetFont(TP_SET_BUTTON_CTRL_ID, &button_font_all);
            GUIBUTTON_SetTextId(TP_SET_BUTTON_CTRL_ID, TXT_TP_SET);
            GUIBUTTON_SetRect(TP_SET_BUTTON_CTRL_ID, &btn_set_rc);
            GUIBUTTON_SetBg(TP_SET_BUTTON_CTRL_ID, &bg_btn);
            GUIBUTTON_SetCallBackFunc(TP_SET_BUTTON_CTRL_ID, SetParamEditbox);//the button set param of TP

 	        GUIBUTTON_SetRunSheen( TP_TEST_BUTTON_CTRL_ID, TRUE);//btn_test
            GUIBUTTON_SetPressedBg(TP_TEST_BUTTON_CTRL_ID, &bg_btn);              
            GUIBUTTON_SetFont(TP_TEST_BUTTON_CTRL_ID, &button_font_all);
            GUIBUTTON_SetTextId(TP_TEST_BUTTON_CTRL_ID, TXT_TP_TEST);
            GUIBUTTON_SetRect(TP_TEST_BUTTON_CTRL_ID, &btn_test_rc);
            GUIBUTTON_SetBg(TP_TEST_BUTTON_CTRL_ID, &bg_btn);
            GUIBUTTON_SetCallBackFunc(TP_TEST_BUTTON_CTRL_ID, TestParamHandler);//the button draw TP
            
            for (i=0;i<sizeof(TP_PAINT_PARAM)/sizeof(TP_PARAM);i++)
            {
                GUIEDIT_SetFont(TP_PAINT_PARAM[i].TP_EDITBOX_ID, SONG_FONT_14);
                GUIAPICTRL_SetRect(TP_PAINT_PARAM[i].TP_EDITBOX_ID, &editbox_rc);
                editbox_rc.bottom += TP_TEST_EDITBOX_HEIGHT;
                editbox_rc.top += TP_TEST_EDITBOX_HEIGHT;             
            }
        }

		break;
	
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_UP:

		break;	
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	case MSG_GET_FOCUS:
		break;
	case MSG_FULL_PAINT:

        GUI_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);
        
        for(i=0;i<(sizeof(TP_PAINT_PARAM)/sizeof(TP_PARAM));i++)
        {
            text_str.wstr_ptr = TP_PAINT_PARAM[i].w_text;
            text_str.wstr_len = MMIAPICOM_Wstrlen(TP_PAINT_PARAM[i].w_text);		                      
            GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T      *)& start_point,
                (const MMI_STRING_T     *)&text_str,
                &text_style,
                text_state
                );
            start_point.y = start_point.y + TP_TEST_EDITBOX_HEIGHT;
        }

		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:		
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
		break;
    case MSG_CLOSE_WINDOW:
		break;
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	
	return recode;
}
/*****************************************************************************/
//    Description : get the param from editbox
//    Global resource dependence : 
//    Author:   guodong.liu
//    Note:
/*****************************************************************************/


LOCAL MMI_RESULT_E GetParamEditbox(void)
{   
    wchar  wchar_buffer[TP_TEST_EDIDBOX_PARA_COUNT + 1] = {0};
    uint8  char_buffer[TP_TEST_EDIDBOX_PARA_COUNT + 1] = {0};
    uint32  i = 0;
    for(i = 0;i<sizeof(TP_PAINT_PARAM)/sizeof(TP_PARAM); i++)
    {
        GUIEDIT_ClearAllStr(TP_PAINT_PARAM[i].TP_EDITBOX_ID);
        _snprintf((char*)char_buffer, TP_TEST_EDIDBOX_PARA_COUNT, "%d", GetParamHandler(TP_TEST_PARA_MSG+i));
        MMIAPICOM_StrToWstr(char_buffer, wchar_buffer);           
        GUIEDIT_SetString(TP_PAINT_PARAM[i].TP_EDITBOX_ID, wchar_buffer, MMIAPICOM_Wstrlen(wchar_buffer));
        MMK_SendMsg(TP_PAINT_PARAM[i].TP_EDITBOX_ID, MSG_CTL_PAINT, 0);
        
    }
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//    Description : set the param from editbox
//    Global resource dependence : 
//    Author:   guodong.liu
//    Note:
/*****************************************************************************/
#ifdef WIN32
LOCAL uint32 g_ParamBuf[WIN32_TP_TEST_PARA_NUM]={0};
LOCAL void Test_GetPoints(uint32* buf_addr, uint32* size)
{   
    uint32 points_date[] = {70,70,160,300,25,87,67,90,114,35,44};
    uint32 j=0;     
    uint32* data_ptr = 0;
    *buf_addr = (uint32)(&(g_ParamBuf[0]));
    for (j=0;j<sizeof(points_date)/sizeof(points_date[0]);j++)
    {
        g_ParamBuf[j] = points_date[j];
    }
    data_ptr =(uint32*)(*buf_addr);
    * size = sizeof(points_date)/sizeof(points_date[0]);

}
#endif

LOCAL MMI_RESULT_E SetParamEditbox(void)
{
    uint32 msg_id = TP_TEST_PARA_MSG;
    uint32 i = 0;    
    uint32 edit_text = 0;//cid601732
    MMI_STRING_T text_str = {0};
    
    for (i=0;i<sizeof(TP_PAINT_PARAM)/sizeof(TP_PARAM);i++)
    {
        GUIEDIT_GetString(TP_PAINT_PARAM[i].TP_EDITBOX_ID,&text_str);
        MMIAPICOM_WstrToNum(text_str.wstr_ptr, (uint8)text_str.wstr_len,&edit_text);
        SetParamHandler( msg_id+i, edit_text);
    }
    
    return MMI_RESULT_TRUE;
}

//test button call back
LOCAL MMI_RESULT_E TestParamHandler(void)
{
    
    MMK_CreateWin((uint32 *)MMIENG_TP_TEST_DRAW_WIN_TAB , PNULL);
    return MMI_RESULT_TRUE;    
}



//the driver interface TPSRV_GetPoints()
LOCAL void GetPoints(uint32* data, uint32* size)
{
#ifdef WIN32
    Test_GetPoints(data, size);
#else
#ifdef TOUCHPANEL_TYPE_HW
    TPSRV_GetPoints(data, size);  /*lint !e718 !e746 !e628*/
#else
       TPDSVR_SIG_T tp_sig = {0};
	uint32 num_point = 0 ;

        if ( SCI_SUCCESS == TP_Read( &tp_sig ) )
        {
            uint32 i = 0 , j=0 ;

		num_point =  MIN( tp_sig.num_of_point, MMI_TP_POINTS_MAX );
		
		data[j] =  tp_sig.x_key;
		data[j+1] = tp_sig.y_key;
		j+=2;
		
            for ( i = 1; i < num_point ; i++ )
            {
		data[j] = tp_sig.xy_position[i-1].x_position;
		data[j+1] = tp_sig.xy_position[i-1].y_position;
		j+=2 ;		
            }

	*size = num_point;            
        }
#endif
#endif    
}
//the driver interface TPSRV_GetParamHandler()
LOCAL MMI_RESULT_E GetParamHandler(uint32 msg_id)
{
#ifdef WIN32

#else
#ifdef TOUCHPANEL_TYPE_HW
    return TPSRV_GetParamHandler(msg_id); /*lint !e718 !e746 !e628*/
#endif
#endif
    return MMI_RESULT_FALSE;//cid607654
}
//the driver interface TPSRV_SetParamHandler()
LOCAL void SetParamHandler(uint32 msg_id, uint32 value)
{
#ifdef WIN32
    
#else
#ifdef TOUCHPANEL_TYPE_HW
    TPSRV_SetParamHandler( msg_id, value); /*lint !e718 !e746 !e628*/
#endif
#endif
}

LOCAL MMI_RESULT_E ENGTpDrawWinHandleMsg(
                                         MMI_WIN_ID_T      win_id, 	
                                         MMI_MESSAGE_ID_E  msg_id,
                                         DPARAM            param
                                         )//draw TP points
                                         
{   
    
    MMI_RESULT_E            recode     = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	    lcd_dev_info = {GUI_MAIN_LCD_ID};
    GUI_RECT_T              rect = {0, 0, 0, 0};
    GUI_RECT_T              font_rect = {0, 0, 0, 0};
    GUISTR_STYLE_T          text_style = {0};
    GUISTR_STATE_T          text_state = GUISTR_STATE_ALIGN;
    MMI_STRING_T            text_str  = {0};
    GUI_POINT_T             start_point_xy = {0,0};   
    uint32                   i = 0 , j=0;
    LOCAL uint32            s_size = 0;  //num of size of TPSRV_GetPoints()
    LOCAL uint32            s_data[TP_TEST_SEVEN_WORD] = {0}; //data of TPSRV_GetPoints()
    LOCAL uint8             s_string_time_id = 0;//timer of get TP points
    wchar                   wchar_point_xy[TP_TEST_PARA_PRINT_COUNT + 1] = {0};
    uint8                   char_point_xy[TP_TEST_PARA_PRINT_COUNT + 1] = {0};
    uint8                   char_buffer[TP_TEST_EDIDBOX_PARA_COUNT + 1] = {0};
    wchar                   wchar_buffer[TP_TEST_EDIDBOX_PARA_COUNT + 1] = {0};
    GUI_COLOR_T             cur_color  = 0;
    uint32                  getdata_addr = 0;
    uint32*                 getdata_ptr = SCI_NULL;
	GUI_POINT_T points = {0};
	MMI_TP_STATUS_E tp_status = 0;
 
    text_style.align         = ALIGN_LEFT;
    #ifdef MAINLCD_SIZE_320X480
    text_style.font          = SONG_FONT_16;
    #else 
    text_style.font          = SONG_FONT_15;
    #endif
    text_style.font_color    = MMI_BLACK_COLOR;
    cur_color                = MMI_RED_COLOR;
    MMK_GetWinRect(win_id, &rect);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef TOUCHPANEL_TYPE_HW		
        GetPoints(&getdata_addr, &s_size);
        if(s_size<TP_TEST_GET_WORD_MAX)
            {
                getdata_ptr = SCI_ALLOCA(s_size * sizeof(uint32));
                if (PNULL != getdata_ptr)
                {
                    SCI_MEMSET(getdata_ptr, 0x00, (s_size * sizeof(wchar)));
                }
            }
        else
            {
                    MMIPUB_OpenAlertFailWin(TXT_ERROR/*TXT_COMMON_SD_FAIL*/);
                    return MMI_RESULT_FALSE;
            }
#endif		
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        
        s_string_time_id = MMK_CreateWinTimer(win_id, TP_TEST_GET_POINT_TIMER, FALSE);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_TIMER:
        	{
        
#ifdef TOUCHPANEL_TYPE_HW
        GetPoints(&getdata_addr, &s_size);
        getdata_ptr = (uint32*)(getdata_addr);

        if (s_size>=MMI_TP_POINTS_MAX * 2)
            {
	    for (i=0;i<MMI_TP_POINTS_MAX * 2;i++)
                {
                    s_data[i] = *(getdata_ptr+i);
                }               

            }
        else
            {
                for (i=0;i<s_size;i++)
                {
                    s_data[i] = *(getdata_ptr+i);
                }               

            }

            LCD_FillCircle(&lcd_dev_info, &rect, s_data[0], s_data[1], TP_TEST_CIRCLE_RADIUS, cur_color);
            LCD_FillCircle(&lcd_dev_info, &rect, s_data[2], s_data[3], TP_TEST_CIRCLE_RADIUS, cur_color);
#else	
		MMK_GetMultiTPMsg( &tp_status ,&points);
		LCD_FillCircle(&lcd_dev_info, &rect, points.x ,points.y , TP_TEST_CIRCLE_RADIUS, cur_color);
#endif

 
            font_rect.left = rect.left;
            font_rect.top = rect.bottom/2;
            font_rect.right = rect.right;
	     font_rect.bottom = font_rect.top + 1 * GUIFONT_GetHeight(text_style.font);          			
            //font_rect.bottom = rect.top + 3*GUIFONT_GetHeight(text_style.font);          
            GUI_FillRect(&lcd_dev_info,font_rect,MMI_WHITE_COLOR);                
            //line1
#ifdef TOUCHPANEL_TYPE_HW            
            _snprintf((char*)char_point_xy, TP_TEST_PARA_PRINT_COUNT, "x1=%d y1=%d x2=%d y2=%d", s_data[0],s_data[1], s_data[2],s_data[3]);
#else            
            _snprintf((char*)char_point_xy, TP_TEST_PARA_PRINT_COUNT, "x1=%d y1=%d ",  points.x, points.y);
#endif
            start_point_xy.y = font_rect.top ;//rect.top;
            MMIAPICOM_StrToWstr(char_point_xy, wchar_point_xy);
            text_str.wstr_ptr = wchar_point_xy;
            text_str.wstr_len = MMIAPICOM_Wstrlen(wchar_point_xy);
            GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T      *)& start_point_xy,
                (const MMI_STRING_T     *)& text_str,
                &text_style,
                text_state
                );
#ifdef TOUCHPANEL_TYPE_HW			
            SCI_MEMSET(wchar_point_xy, 0x00, ((TP_TEST_PARA_PRINT_COUNT + 1) * sizeof(wchar)));
            //line2
            _snprintf((char*)char_point_xy, TP_TEST_PARA_PRINT_COUNT, "delta_x=%d delta_y=%d rtouch=%d", s_data[4],s_data[5], s_data[6]);
            start_point_xy.y = rect.top + GUIFONT_GetHeight(text_style.font);
            MMIAPICOM_StrToWstr(char_point_xy, wchar_point_xy);
            text_str.wstr_ptr = wchar_point_xy;
            text_str.wstr_len = MMIAPICOM_Wstrlen(wchar_point_xy);

            GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T      *)& start_point_xy,
                (const MMI_STRING_T     *)& text_str,
                &text_style,
                text_state
                );
            SCI_MEMSET(wchar_point_xy, 0x00, ((TP_TEST_PARA_PRINT_COUNT + 1) * sizeof(wchar)));
            //line3
            MMIAPICOM_Wstrncpy(wchar_point_xy, TP_TEST_MORE_PARA, MMIAPICOM_Wstrlen(TP_TEST_MORE_PARA));
         for (i=TP_TEST_SEVEN_WORD;i<s_size;i++)
         {
             _snprintf((char*)char_buffer, TP_TEST_EDIDBOX_PARA_COUNT, "%d ", *(getdata_ptr+i));
             MMIAPICOM_StrToWstr(char_buffer, wchar_buffer);
             MMIAPICOM_Wstrncat(wchar_point_xy, wchar_buffer, MMIAPICOM_Wstrlen(wchar_buffer));
         }
            start_point_xy.y = rect.top + 2*GUIFONT_GetHeight(text_style.font);
            text_str.wstr_ptr = wchar_point_xy;
            text_str.wstr_len = MMIAPICOM_Wstrlen(wchar_point_xy);

            GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T      *)& start_point_xy,
                (const MMI_STRING_T     *)& text_str,
                &text_style,
                text_state
                );
#endif	

        if (s_string_time_id != 0)
        {
            MMK_StopTimer( s_string_time_id );
        }
        s_string_time_id = MMK_CreateWinTimer(win_id, TP_TEST_GET_POINT_TIMER, FALSE);		
    	}
        break;
        
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:        
            MMK_StopTimer( s_string_time_id );
            s_string_time_id = 0;
            s_size = 0;
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
            break;	
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
        case MSG_GET_FOCUS:
            break;
        case MSG_FULL_PAINT:
            
            GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);
            
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:		
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            if (s_string_time_id != 0)
            {
                MMK_StopTimer( s_string_time_id );
                s_string_time_id = 0;
            }            
            s_size = 0;
            SCI_FREE(getdata_ptr);
            getdata_ptr = PNULL; 
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
}
#endif

extern BOOLEAN DRVAUD_SetMute(uint32  handle, BOOLEAN is_mute);

PUBLIC BOOLEAN MMIENG_IsMonkeyTestMode(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMIENG_NV_IS_MONKEY_MODE,&s_is_monkey_test_mode,return_value);
    if(MN_RETURN_FAILURE == return_value)
    {
        s_is_monkey_test_mode = FALSE;
        MMINV_WRITE(MMIENG_NV_IS_MONKEY_MODE, &s_is_monkey_test_mode);
    }
    TRACE_APP_ENGINEERMODE("s_is_monkey_test_mode=%d",s_is_monkey_test_mode);
    return s_is_monkey_test_mode;
}

PUBLIC void MMIENG_SetMonkeyTestMode(BOOLEAN flag)
{
    s_is_monkey_test_mode = flag;
    TRACE_APP_ENGINEERMODE("s_is_monkey_test_mode=%d",s_is_monkey_test_mode);
    MMINV_WRITE(MMIENG_NV_IS_MONKEY_MODE, &s_is_monkey_test_mode);
}
LOCAL void AppendMonkeyTestSetWinListItem(  
                            MMI_WIN_ID_T     win_id,
                            MMI_CTRL_ID_T    ctrl_id
                            )
{
    uint32 index = 0;
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    const MMI_TEXT_ID_T     test_item[]= {
    TXT_ENG_USB,
    TXT_ENG_CALL_BARRING,
    TXT_ENG_UIBACKLIGHT_TITLE,
    TXT_ENG_SLIENT_ENVIRONMENT,
    TXT_ENG_KEY_LOCK_OFF,    
    TXT_ENG_DISABLE_FLYMODE_SETTINGS,
    TXT_ENG_PB_FULL,
    TXT_ENG_SMS_FULL,
    };
    item_t.item_data_ptr = &item_data;
    item_t.item_style    = GUIITEM_STYLE_1STR_ENG;   
    item_t.item_state    = GUIITEM_STATE_SELFADAPT_RECT|GUIITEM_STATE_CONTENT_CHECK;
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_IMAGE_ID;
	if(MMIENG_IsMonkeyTestMode())
	{
		item_data.item_content[1].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;  
	}
	else
	{
		item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
	}
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
    for(index = 0; index < sizeof(test_item)/sizeof(test_item[0]); index++)
    {
        item_data.item_content[0].item_data.text_id = test_item[index];
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}

/********************************************************************************
 NAME:			mokey test set colletion
 DESCRIPTION:	直播
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: 
 DATE:
 ********************************************************************************/
LOCAL MMI_RESULT_E ENGMonkeyTestSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    
    MMI_RESULT_E result = MMI_RESULT_TRUE; 
    MMI_CTRL_ID_T   ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    MMISET_BACKLIGHT_SET_T      backlight_info  =   {0};
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:  
	    GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMMON_OPEN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_1RADIO_ENG);
        MMIAPISET_AppendListItemByTextIdExt(TXT_COMMON_CLOSE,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_1RADIO_ENG);  

        if (MMIENG_IsMonkeyTestMode())
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }

        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        MMK_SetAtvCtrl(win_id,ctrl_id);    
        break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif   
    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:    
    case MSG_KEYDOWN_OK:        
    case MSG_APP_WEB:        
    case MSG_APP_MENU:    
	    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

        if (0 == cur_selection)
        {
             MMIENG_SetMonkeyTestMode(TRUE);
			 MMIENG_SetUSBLog(TRUE);
			 {
				 uint8   active_mode_id = MMIENVSET_GetCurModeId();
				 uint8  envset_mode = MMIENVSET_SILENT_MODE;
				 MMIENVSET_SetCurModeId(envset_mode);
				 MMIAPIENVSET_ActiveMode(envset_mode);
				 MMIENVSET_SetPreActiveMode(active_mode_id);   
				 DRVAUD_SetMute(0,1);
			 }
			 
			 MMIAPIENG_SetCallBarring(TRUE);
			 MMIDEFAULT_AllowTurnOffBackLight(FALSE);
			 MMISET_SetAutoBoardKey(MMISET_KEYLOCK_CLOSE);
			 MMIAPIPB_FillPhoneContactFull();
             backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN_FOREVER;
             MMIAPISET_SetBackLightInfo(backlight_info);//in platform 317,lock and backlight time is seperate
        }
        else
        {
             MMIENG_SetMonkeyTestMode(FALSE);
			 MMIENG_SetUSBLog(FALSE);
			 {
				 uint8   active_mode_id = MMIENVSET_GetCurModeId();
				 uint8  envset_mode = MMIENVSET_STANDARD_MODE;
				 MMIENVSET_SetCurModeId(envset_mode);
				 MMIAPIENVSET_ActiveMode(envset_mode);
				 MMIENVSET_SetPreActiveMode(active_mode_id);
				 DRVAUD_SetMute(0,0);
			 }
			 MMIAPIENG_SetCallBarring(FALSE);
			 MMIDEFAULT_AllowTurnOffBackLight(TRUE);
#ifndef PLATFORM_ANTISW3 //in 317 not allowed to lock phone
			 MMISET_SetAutoBoardKey(MMISET_KEYLOCK_15_SEC);
#endif
            backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN;
            backlight_info.backlight_time = MMISET_TIME_15;
            MMIAPISET_SetBackLightInfo(backlight_info);
        }
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        MMK_CreateWin((uint32*)MMIENG_MONKEY_TEST_SET_ITEM_TAB, PNULL);
        break;
    
    case MSG_CTL_CANCEL:    
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_LIST_CHECK_CONTENT:         
        break;

        
    case MSG_CLOSE_WINDOW:
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
/********************************************************************************
 NAME:			mokey test set colletion
 DESCRIPTION:	直播
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: 
 DATE:
 ********************************************************************************/
LOCAL MMI_RESULT_E ENGMonkeyTestSetItemWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    
    MMI_RESULT_E result = MMI_RESULT_TRUE; 
    MMI_CTRL_ID_T   ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:  
        GUILIST_SetMaxItem(ctrl_id, 10, FALSE);
        AppendMonkeyTestSetWinListItem(win_id, ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);        
        break;
        
    case MSG_CTL_MIDSK:     
    case MSG_CTL_OK:    
    case MSG_KEYDOWN_OK:        
    case MSG_APP_WEB:        
    case MSG_APP_MENU:    
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CTL_CANCEL:    
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;
        

    case MSG_CTL_LIST_CHECK_CONTENT:         
        break;

        
    case MSG_CLOSE_WINDOW:
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

#else

MMI_APPLICATION_T	g_mmieng_app;   /*lint !e552*/
/*for idle dial num start*/
LOCAL MMIENG_IDLE_DIAL_NUM_APP_T s_mmieng_idle_dial_num_app_tab[] =
{
    {
    "#*8378#0#",
    MMIENG_IDLE_DIAL_NUM_UI_TEST1,
    PARS_MANUFACTURE_TEST,
    MMIAPIENG_StartUITest
    }
    ,
#ifdef ENGTD_SUPPORT
    {
    "#*8378#2#",
    MMIENG_IDLE_DIAL_NUM_SHOW_VERSION1,
    PARS_ENGINEER_MENU,
    MMIAPIENGTD_StartEngineerMenu
    }
    ,
#endif
    {
    "#*8378#1#",
    MMIENG_IDLE_DIAL_NUM_SHOW_VERSION1,
    PARS_ENGINEER_MENU,
    MMIAPIENG_CreateShowVersionWin
    }
    ,
  {
    "#*786646468#",
     MMIENG_IDLE_DIAL_NUM_NV_COUNTING,
    PARS_ENGINEER_MENU,
    MMIENGWIN_OpenNVCountingWin
    }
#ifdef MET_MEX_SUPPORT
		,
	{
	"*#6666#",
	MMIENG_IDLE_DIAL_NUM_MET_MEX,    
	PARS_ENGINEER_MENU,    
	MMIMEX_ShowPlatformEngineerMode
	}
#endif
};
/*for idle dial num end*/

/*****************************************************************************/
// 	Description : MMIAPIENG_StopQData
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_StopQData(void)
{
}

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQDataStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQDataStatus(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_InitModule(void)
{

}

/********************************************************************************
 NAME:			MMIAPIENG_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENG_AppInit(void)
{

}

/********************************************************************************
 NAME:			MMIAPIENG_RedkeyIsEnableInIdle
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RedkeyIsEnableInIdle(void)
{
	return TRUE;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetSensorMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.08.08
********************************************************************************/
PUBLIC MMIENG_SENSOR_MODE_E MMIAPIENG_GetSensorMode(void)
{
   	return MMIENG_SENSOR_MODE_DEFAULT;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC MMIENG_DV_FRAME_FRE_E MMIAPIENG_GetDVFrameFre(void)
{
	return MMIENG_DV_FRAME_FRE_30HZ;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetChipTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2007.08.08
********************************************************************************/
PUBLIC MMIENG_CHIP_TEST_SETTING_E MMIAPIENG_GetChipTestMode(void)
{
	return MMIENG_CHIP_TEST_SETTING_OFF;
}

/********************************************************************************
 NAME:			SetXsrResult
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		zhuowang
 DATE:			2010.10.03
********************************************************************************/
LOCAL void SetXsrResult(BOOLEAN is_on)
{
	;	
}


/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallForwardQuery(void)
{
	return TRUE;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetAutoAnswerCallSet(void)
{
	return FALSE;
}

/********************************************************************************
 NAME:			MMIAPIENG_GetIsCardLogOn
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsCardLogOn(void)
{
    return FALSE;
}

/********************************************************************************
 NAME:			MMIAPIENG_StartupAdjustCardLogOn
 DESCRIPTION:	开机调整CardLog状态
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			2007.10.11
********************************************************************************/
PUBLIC void MMIAPIENG_StartupAdjustCardLogOn(void)
{
}

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMIENG_IQ_MODE_T MMIAPIENG_GetIQMode(void)
{
	MMIENG_IQ_MODE_T iq_mode_param = {0};
	
    	return iq_mode_param;
}

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQModeStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQModeStatus(void)
{
    return FALSE;
}

/********************************************************************************
 NAME:			GetWapMmsSasTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		hui.zhao
 DATE:			2009.07.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetWapMmsSasTestMode(void)
{
	return FALSE;
}
/********************************************************************************
 NAME:			MMIAPIENG_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC void MMIAPIENG_SetPktOutPut(void)
{
  

}
/*****************************************************************************/
//  Description : MMIAPIENG_SetUa
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetUa(MMIENG_UA_T *ua_ptr)
{
    return TRUE;
    
}

/*****************************************************************************/
//  Description : MMIAPIENG_GetUa,when eng ua is open, return the eng ua info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetUa(MMIENG_UA_T *ua_ptr)
{
    //NOTES:DON'T modify!!!---for ENG test, do nothing        
    return TRUE;

}

/********************************************************************************
 NAME:			MMIAPIENG_GetCallBarring
 DESCRIPTION:	MS00186848
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallBarring(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : restore factory setting of ENG
//	Global resource dependence : 
//  Author: fen.xie
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIENG_RestoreFactorySetting(void)
{

}
/********************************************************************************
//MS00216980 cheney
 NAME:			BOOLEAN MMIAPIENG_GetCMCCTestFlag
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
PUBLIC  BOOLEAN MMIAPIENG_GetCMCCTestFlag(void)
{
	return FALSE;
}
#endif//ENG_SUPPORT

/********************************************************************************
 NAME:			MMIAPIENG_ParseIdleSetLanguageTypeNumString
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			2009.2.25
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_ParseIdleSetLanguageTypeNumString(
                                                uint8* num_str_ptr, 
                                                uint8 num_str_len,
                                                MMISET_LANGUAGE_TYPE_E *language_type_ptr
                                                )
{
    BOOLEAN isStrParsered = FALSE;
    uint16 i = 0;

    if(language_type_ptr == PNULL)
	{
	    //SCI_TRACE_LOW:"mmieng_main.c MMIAPIENG_ParseIdleSetLanguageTypeNumString language_type_ptr == null"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_5212_112_2_18_2_17_20_53,(uint8*)"");
	    return FALSE;
	}

    *language_type_ptr = MMISET_MAX_LANGUAGE;
    
    for (i = 0; i < ARR_SIZE(s_mmieng_idle_set_languagetype_tab); i ++)//该暗码是否找到对应的语言
    {
       if (0 == 
          GUI_StringCompare((uint8 *)s_mmieng_idle_set_languagetype_tab[i].num_str_ptr, 
                    (uint16)strlen(s_mmieng_idle_set_languagetype_tab[i].num_str_ptr),
                    (uint8 *)num_str_ptr, 
                    (uint16)strlen((const char *)num_str_ptr)))
        {
            isStrParsered = TRUE;
            *language_type_ptr = s_mmieng_idle_set_languagetype_tab[i].language_type;       
            break;
        }
    }
    return isStrParsered;
}

/********************************************************************************
 NAME:			MMIAPIENG_DoIdleSetLanguageTypeNumOpt
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaoghui
 DATE:			2009.2.25
********************************************************************************/
PUBLIC void MMIAPIENG_DoIdleSetLanguageTypeNumOpt(
                                                MMISET_LANGUAGE_TYPE_E language_type
                                                )
{   
    if (MMIAPISET_CheckLanguageStatus(language_type)//该语言状态是否有效
            &&MMIAPISET_IsSelectedLanguageValid(language_type)//该语言字库存在
    )
    {   
        MMIAPISET_SetLanguageType(language_type);
#ifndef SUBLCD_SIZE_NONE
        //需要立即重新刷新小屏，否则小屏的内容不会改变
        MMISUB_UpdateDisplaySubLcd();
#endif
       // MMIPUB_OpenAlertSuccessWin(TXT_ENG_LANGUAGE_SET_SUCC);
    }
    else
    {
        //fail!,提示不存在
        MMIPUB_OpenAlertFailWin(TXT_COMMON_NOT_EXIST);
    }
}

/********************************************************************************
 NAME:			MMIAPIENG_ParseIdleDialNumString
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_ParseIdleDialNumString(
                                                uint8* num_str_ptr, 
                                                uint8 num_str_len,
                                                MMIENG_IDLE_DIAL_NUM_TYPE_E *opt_type_ptr,//OUT
                                                parsCtrlCodeE *ctrl_code_ptr//OUT
                                                )
{
    BOOLEAN isStrParsered = FALSE;
    uint32 i = 0;
     
    if(opt_type_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"mmieng_main.c MMIAPIENG_ParseIdleDialNumString opt_type_ptr == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_5282_112_2_18_2_17_20_54,(uint8*)"");
        return FALSE;
    }

    if(ctrl_code_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"mmieng_main.c MMIAPIENG_ParseIdleDialNumString ctrl_code_ptr == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_5288_112_2_18_2_17_20_55,(uint8*)"");
        return FALSE;
    }

    *opt_type_ptr = MMIENG_IDLE_DIAL_NUM_MIN;
    *ctrl_code_ptr = PARS_CHANGE_PIN1;

    for (i = 0; i < ARR_SIZE(s_mmieng_idle_dial_num_app_tab); i ++)
    {
       if (0 == 
          GUI_StringCompare((uint8 *)s_mmieng_idle_dial_num_app_tab[i].num_str_ptr, 
                    (uint16)strlen(s_mmieng_idle_dial_num_app_tab[i].num_str_ptr),
                    (uint8 *)num_str_ptr, 
                    (uint16)strlen((const char *)num_str_ptr)))
        {
            isStrParsered = TRUE;
            *opt_type_ptr = s_mmieng_idle_dial_num_app_tab[i].opt_type;
            *ctrl_code_ptr = s_mmieng_idle_dial_num_app_tab[i].ctrl_code;               
            break;
        }
        
    }
    return isStrParsered;
}

/********************************************************************************
 NAME:			GetIdleDialNumTabIndexByType
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
LOCAL int16 GetIdleDialNumTabIndexByType(MMIENG_IDLE_DIAL_NUM_TYPE_E opt_type)
{
    uint32 i = 0;

    //SCI_TRACE_LOW:"s_mmieng_idle_dial_num_app_tab size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_5326_112_2_18_2_17_20_56,(uint8*)"d", ARR_SIZE(s_mmieng_idle_dial_num_app_tab));
    
    for (i = 0; i < ARR_SIZE(s_mmieng_idle_dial_num_app_tab); i ++)
    {
        if (s_mmieng_idle_dial_num_app_tab[i].opt_type == opt_type)
        {
            break;
        }
        else
        {
        }
    }
    
    return i;
}

/********************************************************************************
 NAME:			MMIAPIENG_DoIdleDialNumOpt
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC void MMIAPIENG_DoIdleDialNumOpt(
                                        MMIENG_IDLE_DIAL_NUM_TYPE_E opt_type
                                        )
{
    uint32 index = 0;   

    index = (uint32)(GetIdleDialNumTabIndexByType(opt_type));   

    //SCI_TRACE_LOW:"MMIENG_MAIN.C MMIAPIENG_DoIdleDialNumOpt index is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_MAIN_5357_112_2_18_2_17_20_57,(uint8*)"d",index);
    if(index >=  ARR_SIZE(s_mmieng_idle_dial_num_app_tab))
    {
        return ;
    }
    
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE) 
        && (MMIENG_IDLE_DIAL_NUM_MENU != s_mmieng_idle_dial_num_app_tab[index].opt_type))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        return;
    }  
#ifdef CMMB_SUPPORT    
    if (MMIAPIMTV_IsMTVOpen()) //CMMB播放中
    {
        //TV播放中进入CMMB工程模式
        if(MMIENG_IDLE_DIAL_NUM_CMMB_TEST == opt_type)
        {
            s_mmieng_idle_dial_num_app_tab[index].processFun();
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_EXIT_MOBILETV);
        }
        return ;
    }
#endif
    /* Modify by michael on 2/27/2012 for NEWMS00172242:U盘使用时，禁止进入芯片测试 */     
    if(MMIENG_IDLE_DIAL_NUM_CHIP_TEST == s_mmieng_idle_dial_num_app_tab[index].opt_type)
    {
       if(MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            return;
        }
    }
    /************************Modify over*************************/
    
#ifndef ENG_SUPPORT 
	if (MMIENG_IDLE_DIAL_NUM_SHOW_VERSION1 == opt_type)
	{
		g_mmieng_showversion_for1 = TRUE;
	}
#endif	

    s_mmieng_idle_dial_num_app_tab[index].processFun();
}

#ifdef CMMB_SUPPORT
//解析INI 文件中的一行；
LOCAL BOOLEAN CmmbTest_ParseLine(char *file_info_ptr, char *str, char *buf, int buflen)
{
    char *StrPos = NULL;
    char *LinePos = NULL;
    char *DataStartPos = NULL;
    char *DataEndPos = NULL;
    int  len=0;
    BOOLEAN ret = FALSE;
    
    if (file_info_ptr == NULL ||
        str      == NULL ||
        buf      == NULL)
    {
        return ret;
    }

    StrPos    = strstr(file_info_ptr, str);
    if (StrPos == NULL)
    {
        return ret;
    }
                
    LinePos = strstr(StrPos, "\r\n");
    if (LinePos == NULL)
    {
        return ret;
    }

    do
    {
        DataEndPos = --LinePos;
    }while(*DataEndPos == ' ');

    do
    {
        DataStartPos = --LinePos;
        if (*DataStartPos == ' ' || *DataStartPos == '=')
        {
            break;
        }
    }while(1);/*lint !e506 */
    DataStartPos++;
    
    len = DataEndPos - DataStartPos + 1;

    if(len < buflen)
    {
        strncpy(buf, DataStartPos, len);
        buf[len] = 0;
        
        ret = TRUE;
    }
    
    return ret;
}
#endif


#ifdef PRINTSCREEN_SUPPORT
/*****************************************************************************/
//     Description : 抓屏功能
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EngPrintScreenWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
        int32 index = 0;
        MMI_RESULT_E result = MMI_RESULT_TRUE;
        int32 list_id = MMIENG_LISTBOX_CTRL_ID ;
        
        switch (msg_id) 
        {
        case MSG_OPEN_WINDOW:
            {
                GUILIST_ITEM_T      item_t = {0};
                GUILIST_ITEM_DATA_T item_data = {0};       

                GUIWIN_SetTitleTextId(win_id, TXT_ENG_PRINTSCREEN, FALSE);
                
                GUILIST_SetMaxItem( list_id, MMIPTSCR_RUNMODE_MAX, FALSE );
                GUILIST_SetOwnSofterKey(list_id,FALSE);
                
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
                item_t.item_data_ptr = &item_data;
                
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
                
                item_data.item_content[0].item_data.text_id = TXT_ENG_PRINTSCREEN_OFF;    
                GUILIST_AppendItem( list_id, &item_t );
                //renwei cr00114478 modify
                #ifdef KEYPAD_TYPE_FOUR_KEY_UNIVERSE
                item_data.item_content[0].item_data.text_id = TXT_ENG_PRINTSCREEN_PRESS_CAMREA_ONLYLCD_MODE;    
                GUILIST_AppendItem( list_id, &item_t );
                item_data.item_content[0].item_data.text_id = TXT_ENG_PRINTSCREEN_PRESS_CAMREA_LCDANDLAYER_MODE;   
                #else
                item_data.item_content[0].item_data.text_id = TXT_ENG_PRINTSCREEN_PRESS_ONLYLCD_MODE;    
                GUILIST_AppendItem( list_id, &item_t );
                item_data.item_content[0].item_data.text_id = TXT_ENG_PRINTSCREEN_PRESS_LCDANDLAYER_MODE;   
                #endif
                //renwei cr00114478 modify
                GUILIST_AppendItem( list_id, &item_t );                      
                item_data.item_content[0].item_data.text_id = TXT_ENG_PRINTSCREEN_AUTO_MODE;    
                GUILIST_AppendItem( list_id, &item_t );
               
                index = MMIAPIPTSCR_GetRunMode(); 
                GUILIST_SetSelectedItem( list_id, index, TRUE );
                GUILIST_SetCurItemIndex( list_id, index);          
                MMK_SetAtvCtrl(win_id, list_id);
            }
            break;
            
        case MSG_FULL_PAINT:
            break;      
            
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            index = GUILIST_GetCurItemIndex(list_id);   
            MMIAPIPTSCR_ChangeRunMode(index);
            MMK_CloseWin(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
        }
        return (result); 
}
#endif
#ifdef MCARE_V31_SUPPORT
PUBLIC BOOLEAN MMIAPIENG_StartMcareTest(void)
{
	McareV31_CleanDataInEng();
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description :get power key flag
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetPowerKeyFlag(void)
{
    BOOLEAN is_enable = TRUE;
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    MMINV_READ(MMIENG_NV_POWER_KEY_SETTING, &is_enable, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        is_enable = TRUE;
        MMINV_WRITE(MMIENG_NV_POWER_KEY_SETTING, &is_enable);
    }

    return is_enable;
}

/*****************************************************************************/
//  Description :set power key flag
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetPowerKeyFlag(BOOLEAN is_enable)
{
    MMINV_WRITE(MMIENG_NV_POWER_KEY_SETTING, &is_enable);
}

/*****************************************************************************/
//  Description :set Register Ps Service
//  Parameter: [In] is_enable: True: register PS service
//                             False:not register PS service
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetRegisterPsServiceEnable( BOOLEAN is_enable)
{
    MMINV_WRITE(MMIENG_NV_IS_REGISTER_PS_SERVICE, &is_enable);
}

/*****************************************************************************/
//  Description :Get Register Ps Service enable
//  Parameter: [In] None
//             [Out] None
//             [Return] is_enable: True: register PS service
//                                 False:not register PS service
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetRegisterPsServiceEnable( void )
{
    BOOLEAN is_enable = TRUE;
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    MMINV_READ(MMIENG_NV_IS_REGISTER_PS_SERVICE, &is_enable, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        is_enable = TRUE;
        MMINV_WRITE(MMIENG_NV_IS_REGISTER_PS_SERVICE, &is_enable);
    }

    return is_enable;
}

/*****************************************************************************/
//  Description :  MMIAPIENGTestMode_Write
//  Global resource dependence :
//  Author:
//  return
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIENG_ForbidAllGprsData_Write(BOOLEAN value)
{
    BOOLEAN ret = FALSE;
    ERR_MNDATAMAG_CODE_E  nv_result = ERR_MNDATAMAG_NO_ERR;
    nv_result = MNNV_WriteItem(MMI_GetTrueNVItemId(MMIENG_NV_FORBID_ALL_GPRS_DATA), sizeof(BOOLEAN), (void *)(&value));
    ret = (nv_result == ERR_MNDATAMAG_NO_ERR ? TRUE : FALSE);
    return ret;
}

/*****************************************************************************/
//  Description : MMIAPIENGTestMode_IsSet
//  Global resource dependence :
//  Author:
//  return
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIENG_IsForbidAllGprsData(void)
{
    ERR_MNDATAMAG_CODE_E  nv_result = ERR_MNDATAMAG_NO_ERR;
    BOOLEAN value = FALSE;

    nv_result = MNNV_ReadItem(MMI_GetTrueNVItemId(MMIENG_NV_FORBID_ALL_GPRS_DATA), sizeof(BOOLEAN), (void *)(&value));

    if(ERR_MNDATAMAG_NO_ERR != nv_result)
    {
        value = FALSE;
        MMIAPIENG_ForbidAllGprsData_Write(value);
    }
    return value;
}

LOCAL void ENGSaveLTEOperationMode(MMIENG_LTE_MODE_E *p_lte_operation_mode,MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys == 0)
        MMINV_WRITE(MMIENG_NV_LTE_OPERATION_MODE_SIM1,p_lte_operation_mode);
    else
        MMINV_WRITE(MMIENG_NV_LTE_OPERATION_MODE_SIM2,p_lte_operation_mode);
}

LOCAL MMI_RESULT_E EngSeLTEOperationModeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    uint32  dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUILIST_SetMaxItem(ctrl_id,MMIENG_LTE_OPERATION_MODE_MAX, FALSE );//max item 3
            GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
            MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_SET_LTE_PS_2,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
            MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_SET_LTE_CS_PS_1,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
            MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_SET_LTE_CS_PS_2,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  
            MMIAPISET_AppendListItemByTextIdExt(TXT_ENG_SET_LTE_PS_1,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);  

#ifndef WIN32
            MNPHONE_GetCEMode(&s_lte_mode_info, dual_sys);
#endif
            cur_selection = s_lte_mode_info;
            //set selected item
            GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

            //set current item
            GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
            MMK_SetAtvCtrl(win_id,ctrl_id);
            break;

        case MSG_FULL_PAINT:
            break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_KEYDOWN_OK:
            break;

        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            s_lte_mode_info= cur_selection;
    // call mn interface
    //set support field
#ifndef WIN32
            MNPHONE_SetCEMode(s_lte_mode_info,dual_sys);
#endif
            ENGSaveLTEOperationMode(&s_lte_mode_info,dual_sys);
            MMK_CloseWin(win_id);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

/*****************************************************************************/
//  Description :   HandleENGForbidAllgprsDataWinMsg
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleENGForbidAllgprsDataWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_LISTBOX_CTRL_ID;
    uint16 cur_selection = 0;
    BOOLEAN is_on=FALSE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        MMIAPISET_AppendListItemByTextIdExt( TXT_COMM_CLOSE ,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextIdExt( TXT_OPEN_COMN,TXT_ENG_OK,TXT_NULL,TXT_ENG_RETURN,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        is_on=MMIAPIENG_IsForbidAllGprsData();
        //set selected item
        GUILIST_SetSelectedItem(ctrl_id, is_on, TRUE);
        //set current item
        GUILIST_SetCurItemIndex(ctrl_id,is_on);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
        SCI_TRACE_LOW("mmieng_main.c HandleENGForbidAllgprsDataWinMsg cur_selection = %d",cur_selection);
        MMIAPIENG_ForbidAllGprsData_Write(cur_selection);
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

#if defined ENG_BAND_FREQ_SUPPORT
#define ENG_FREQ_TITLE           "FREQ"
#define ENG_FREQ_MAX_LEN           20
#define ENG_FREQ_NAME_LEN           5

//option pop menu
#define ENG_FREQ_POP_MENU_NODE_ID    0
#define ENG_FREQ_POP_MENU_TEXT_ID    1
#define ENG_FREQ_POP_MENU_ITEM_DIMEN 2

#define NW_BAND_GSM_850   0
#define NW_BAND_GSM_900   1
#define NW_BAND_DCS_1800  2
#define NW_BAND_PCS_1900  3

#define MMI_NW_BAND_GSM_450 (1 << 0)
#define MMI_NW_BAND_GSM_480 (1 << 1)
#define MMI_NW_BAND_GSM_900P (1 << 2)
#define MMI_NW_BAND_GSM_900E (1 << 3)
#define MMI_NW_BAND_GSM_900R (1 << 4)
#define MMI_NW_BAND_GSM_850 (1 << 5)
#define MMI_NW_BAND_DCS_1800 (1 << 6)
#define MMI_NW_BAND_PCS_1900 (1 << 7)

typedef enum
{
    ENG_FREQ_OPT_NULL,
    ENG_FREQ_EDIT,//编辑
    ENG_FREQ_DELETE,//删除
    ENG_FREQ_DELETE_ALL,//删除全部
    ENG_FREQ_OPT_MAX
}ENG_FREQ_OPT_E;

typedef enum
{
    ENG_FREQ_LTE,
    ENG_FREQ_GSM,
    ENG_FREQ_TYPE_MAX
}ENG_FREQ_TYPE_E;

const uint32 s_eng_freq_opt_item[][ENG_FREQ_POP_MENU_ITEM_DIMEN] =
{
    {ENG_FREQ_EDIT, TXT_EDIT},//编辑
    {ENG_FREQ_DELETE, TXT_COMMON_CLEAR},//删除
    {ENG_FREQ_DELETE_ALL, TXT_DELALL}//删除全部
};

LOCAL uint16  s_current_sel_freq= 0;
LOCAL uint16 g_lte_band_num = 0;
LOCAL MN_DUAL_SYS_E g_dual_sys = 0;
LOCAL MMIENG_BAND_FREQ_T s_band_freq_info={0};
LOCAL ENG_FREQ_TYPE_E  g_freq_type = ENG_FREQ_LTE;
const uint16 LTE_Band_Value[LTE_BAND_NUM]=
{
    1,2,3,4,5,7,8,12,13,17,20,28,34,38,39,40,41,66
};
const uint32 LTE_freq_range[LTE_BAND_NUM][2]=
{
    {1,599},
    {600,1199},
    {1200,1949},
    {1950,2399},
    {2400,2649},
    {2750,3449},
    {3450,3799},
    {5010,5179},
    {5180,5279},
    {5730,5849},
    {6150,6449},
    {9210,9659},
    {37750,38249},
    {38250,38649},
    {38650,39649},
    {39650,41589},
    {66436,67335}
};

const uint32 GSM_freq_range[GSM_BAND_MAX+1][2]=
{
    {128,251},
    {1,124},
    {512,885},
    {512,810},
    {975,1023}
};

LOCAL void ENGSaveBandFreqInfo(MMIENG_BAND_FREQ_T *bandfreqinfo,MN_DUAL_SYS_E  dual_sys)
{
    if(dual_sys == 0)
    {
        MMINV_WRITE(MMIENG_NV_BAND_FREQ_SIM1, bandfreqinfo);
    }
    else
    {
        MMINV_WRITE(MMIENG_NV_BAND_FREQ_SIM2, bandfreqinfo);
    }
}

LOCAL MN_RETURN_RESULT_E ENGReadBandFreqInfo(MMIENG_BAND_FREQ_T *bandfreqinfo,MN_DUAL_SYS_E  dual_sys)
{
    MN_RETURN_RESULT_E nv_result =MN_RETURN_SUCCESS;
    if(dual_sys == 0)
    {
        MMINV_READ(MMIENG_NV_BAND_FREQ_SIM1, bandfreqinfo, nv_result);
    }
    else
    {
        MMINV_READ(MMIENG_NV_BAND_FREQ_SIM2, bandfreqinfo, nv_result);
    }
     return nv_result;
}

LOCAL void  ENGInitBandFreq(MN_DUAL_SYS_E  dual_sys)
{
    uint8 freqnum = 0;
    uint8 gsmband = 0;
    int   i = 0;
    MN_RETURN_RESULT_E ret=MN_RETURN_SUCCESS;
    SCI_MEMSET(&s_band_freq_info,0x0,sizeof(s_band_freq_info));
    ret = ENGReadBandFreqInfo(&s_band_freq_info,dual_sys);

    if(ret != MN_RETURN_SUCCESS)
    {
        SCI_MEMSET(s_band_freq_info.GSM_Freq,0xFFFFFFFF,sizeof(s_band_freq_info.GSM_Freq));
    }
    SCI_MEMSET(s_band_freq_info.Freq_list,0xFFFFFFFF,sizeof(s_band_freq_info.Freq_list));
#ifndef WIN32
    MNPHONE_GetLteLockBand(&g_lte_band_num,s_band_freq_info.LTE_band,dual_sys);
    MNPHONE_GetLteLockFreq(&freqnum,s_band_freq_info.Freq_list,dual_sys);
    if(freqnum == 0)
    {
        SCI_MEMSET(s_band_freq_info.Freq_list,0xFFFFFFFF,sizeof(s_band_freq_info.Freq_list));
    }
    MNPHONE_GetGsmBand(&gsmband,dual_sys);
    if(gsmband & MMI_NW_BAND_GSM_850)
    {
        s_band_freq_info.GSM_band[NW_BAND_GSM_850] =TRUE;
    }
    if(gsmband & MMI_NW_BAND_GSM_900P)
    {
        s_band_freq_info.GSM_band[NW_BAND_GSM_900] =TRUE;
    }
    if(gsmband & MMI_NW_BAND_DCS_1800)
    {
        s_band_freq_info.GSM_band[NW_BAND_DCS_1800] =TRUE;
    }
    if(gsmband & MMI_NW_BAND_PCS_1900)
    {
        s_band_freq_info.GSM_band[NW_BAND_PCS_1900] =TRUE;
    }
    ENGSaveBandFreqInfo(&s_band_freq_info,dual_sys);
#endif

    SCI_TraceLow("zhouqin-ENGInitBandFreq -g_lte_band_num=%d,gsmband=%d,freqnum=%d",g_lte_band_num,gsmband,freqnum);

    for(i=0;i<g_lte_band_num;i++)
    {
        SCI_TraceLow("zhouqin-LTE_band[%d] =%d",i,s_band_freq_info.LTE_band[i]);
    }
    for(i=0;i<ENG_FREQ_MAX_NUM;i++)
    {
        SCI_TraceLow("zhouqin-Freq_list[%d] =%d",i,s_band_freq_info.Freq_list[i]);
        SCI_TraceLow("zhouqin-GSM_Freq[%d] =%d",i,s_band_freq_info.GSM_Freq[i]);
    }
    for(i=0;i<GSM_BAND_MAX;i++)
    {
        SCI_TraceLow("zhouqin-gsm_band[%d] =%d",i,s_band_freq_info.GSM_band[i]);
    }
}

LOCAL MMI_RESULT_E ENGBandFreqWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 group_id = 0;
    uint32 menu_id = 0;
    uint32  dual_sys = (uint32)MMK_GetWinAddDataPtr(win_id);
    MN_RETURN_RESULT_E nv_result =MN_RETURN_SUCCESS;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            g_dual_sys = dual_sys;
            ENGInitBandFreq(g_dual_sys);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_ENG_OK, TXT_NULL, TXT_ENG_RETURN, FALSE);
            CTRLMENU_SetFontColor(MMIENG_BAND_FREQ_CTRL_ID,MMI_WHITE_COLOR);
            MMK_SetAtvCtrl(MMIENG_BAND_FREQ_WIN_ID, MMIENG_BAND_FREQ_CTRL_ID);
            break;

        case MSG_FULL_PAINT:
            break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(MMIENG_BAND_FREQ_WIN_ID);
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            GUIMENU_GetId(MMIENG_BAND_FREQ_CTRL_ID, &group_id, &menu_id);
            switch ( menu_id )
            {
                case ID_ENG_SET_LTE_BAND:
                    MMK_CreateWin((uint32*)MMIENG_SET_LTE_BAND_WIN_TAB, PNULL);
                    break;
                case ID_ENG_SET_GSM_BAND:
                    MMK_CreateWin((uint32*)MMIENG_SET_GSM_BAND_WIN_TAB, PNULL);
                    break;
                case ID_ENG_SET_LTE_FREQ:
                    g_freq_type = ENG_FREQ_LTE;
                    MMK_CreateWin((uint32*)MMIENG_SET_FREQ_WIN_TAB, PNULL);
                    break;
                case ID_ENG_SET_GSM_FREQ:
                    g_freq_type = ENG_FREQ_GSM;
                    MMK_CreateWin((uint32*)MMIENG_SET_FREQ_WIN_TAB, PNULL);
                    break;

                default:
                    break;
            }
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : band freq  sim select
//  Global resource dependence :
//  Author: qin.zhou
//  Note:
/*****************************************************************************/
LOCAL void BandFreqSIMSelectWin(void)
{
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMK_CreateWin((uint32*)MMIENG_BAND_FREQ_WIN_TAB, (ADD_DATA)sim_sys);
    }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    else
    {
        MMISET_SelectSIMFunc(BandFreqSIMSelectCallback, MMISET_APPEND_SYS_OK,PNULL);
    }
#endif
}

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence :
//  Author: qin.zhou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BandFreqSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;

    if(isReturnOK)
    {
        MMK_CreateWin((uint32*)MMIENG_BAND_FREQ_WIN_TAB,(ADD_DATA)dual_sys);
    }

    return (recode);
}

LOCAL void AppendSetLTEBandListItem(
                            MMI_WIN_ID_T     win_id,
                            MMI_CTRL_ID_T    ctrl_id
                            )
{
    uint32 index = 0,i=0;
    GUILIST_ITEM_T          item_t    = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    BOOLEAN                 select_item[LTE_BAND_NUM]={FALSE};
    const MMI_TEXT_ID_T     band_item[]= {
    TXT_ENG_SET_LTE_BAND_1,
    TXT_ENG_SET_LTE_BAND_2,
    TXT_ENG_SET_LTE_BAND_3,
    TXT_ENG_SET_LTE_BAND_4,
    TXT_ENG_SET_LTE_BAND_5,
    TXT_ENG_SET_LTE_BAND_7,
    TXT_ENG_SET_LTE_BAND_8,
    TXT_ENG_SET_LTE_BAND_12,
    TXT_ENG_SET_LTE_BAND_13,
    TXT_ENG_SET_LTE_BAND_17,
    TXT_ENG_SET_LTE_BAND_20,
    TXT_ENG_SET_LTE_BAND_28,
    TXT_ENG_SET_LTE_BAND_34,
    TXT_ENG_SET_LTE_BAND_38,
    TXT_ENG_SET_LTE_BAND_39,
    TXT_ENG_SET_LTE_BAND_40,
    TXT_ENG_SET_LTE_BAND_41,
    TXT_ENG_SET_LTE_BAND_66
    };
    item_t.item_data_ptr = &item_data;
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;

    for(index = 0; index < sizeof(band_item)/sizeof(band_item[0]); index++)
    {
        item_data.item_content[0].item_data.text_id = band_item[index];
        GUILIST_AppendItem( ctrl_id, &item_t );
    }

    for (index = 0;  index< LTE_BAND_NUM; index++ )
    {
        for(i=0;i<g_lte_band_num;i++)
        {
            if(LTE_Band_Value[index] == s_band_freq_info.LTE_band[i])
            {
                SCI_TRACE_LOW("LTE_band[%d]=%d", i, s_band_freq_info.LTE_band[i]);
                select_item[index]= TRUE;
                break;
            }
        }
    }

    for (index = 0;  index< LTE_BAND_NUM; index++ )
    {
        if (select_item[index]==TRUE)
        {
            GUILIST_SetSelectedItem(ctrl_id,index,TRUE);
        }
        else
        {
            GUILIST_SetSelectedItem(ctrl_id,index,FALSE);
        }
    }
}

PUBLIC void ENGSetLTEBandSetInfo(MMI_CTRL_ID_T ctrl_id)
{
    uint16  i = 0;
    uint16  selected_num = 0;
    uint16  selected_item[LTE_BAND_NUM] = {0};
    uint8   number = 0;

    //get current selected items
    selected_num = GUILIST_GetSelectedItemIndex(ctrl_id, selected_item, LTE_BAND_NUM);

    for ( i = 0; i < LTE_BAND_NUM; i++)
    {
        s_band_freq_info.LTE_band[i] = 0;
    }

    for (i=0; i<selected_num; i++)
    {
        if (selected_item[i] < LTE_BAND_NUM)
        {
            s_band_freq_info.LTE_band[number] = LTE_Band_Value[selected_item[i]];
            SCI_TRACE_LOW("[ENGSetLTEBandSetInfo] LTE_band[%d]=%d", i, s_band_freq_info.LTE_band[number]);
            number++;
        }
    }

    //call interface to set LTE band
    g_lte_band_num = number;
#ifndef WIN32
    MNPHONE_SetLteLockBand(g_lte_band_num, (s_band_freq_info.LTE_band),g_dual_sys);
#endif
    //save LTE band info into nv
    ENGSaveBandFreqInfo(&s_band_freq_info, g_dual_sys);
}

LOCAL MMI_RESULT_E ENGSetLTEBandWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E       result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T      ctrl_id = MMIENG_BAND_LISTBOX_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, LTE_BAND_NUM, FALSE);
        GUILIST_SetMaxSelectedItem(ctrl_id,LTE_BAND_NUM);
        AppendSetLTEBandListItem(win_id, ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_KEYDOWN_OK:
    case MSG_APP_WEB:
    case MSG_APP_MENU:
        //MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        ENGSetLTEBandSetInfo(ctrl_id);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_LIST_CHECK_CONTENT:
        break;

    case MSG_CLOSE_WINDOW:
        ENGSetLTEBandSetInfo(ctrl_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

LOCAL void AppendSetGSMBandListItem(
                            MMI_WIN_ID_T     win_id,
                            MMI_CTRL_ID_T    ctrl_id
                            )
{
    uint32 index = 0;
    GUILIST_ITEM_T          item_t    = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    const MMI_TEXT_ID_T     band_item[]= {
    TXT_ENG_SET_GSM_BAND_850,
    TXT_ENG_SET_GSM_BAND_900,
    TXT_ENG_SET_GSM_BAND_1800,
    TXT_ENG_SET_GSM_BAND_1900
    };
    item_t.item_data_ptr = &item_data;
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
    item_data.item_content[1].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;

    for(index = 0; index < sizeof(band_item)/sizeof(band_item[0]); index++)
    {
        item_data.item_content[0].item_data.text_id = band_item[index];
        GUILIST_AppendItem( ctrl_id, &item_t );
    }

    for (index = 0;  index< GSM_BAND_MAX; index++ )
    {
        if (s_band_freq_info.GSM_band[index])
        {
            GUILIST_SetSelectedItem(ctrl_id,index,TRUE);
        }
        else
        {
            GUILIST_SetSelectedItem(ctrl_id,index,FALSE);
        }
    }
}

PUBLIC void ENGSetGSMBandSetInfo(MMI_CTRL_ID_T ctrl_id)
{
    uint16            i = 0;
    uint16            selected_num = 0;
    uint16            selected_item[GSM_BAND_MAX] = {0};
    uint8             gsmband = 0 ;

    //get current selected items
    selected_num = GUILIST_GetSelectedItemIndex(ctrl_id,selected_item,GSM_BAND_MAX);

    for ( i = 0; i < GSM_BAND_MAX; i++)
    {
        s_band_freq_info.GSM_band[i] = FALSE;
    }

    //get select band item from ctrl
    for (i=0; i<selected_num; i++)
    {
        if (selected_item[i] < GSM_BAND_MAX)
        {
            s_band_freq_info.GSM_band[selected_item[i]] = TRUE;
        }
    }

    //CALL MN INTERFACE
    //set GSM band
    if(s_band_freq_info.GSM_band[NW_BAND_GSM_850] ==TRUE)
        gsmband = gsmband|MMI_NW_BAND_GSM_850;
    if(s_band_freq_info.GSM_band[NW_BAND_GSM_900] ==TRUE)
        gsmband = gsmband|MMI_NW_BAND_GSM_900E|MMI_NW_BAND_GSM_900P;
    if(s_band_freq_info.GSM_band[NW_BAND_DCS_1800] ==TRUE)
        gsmband = gsmband|MMI_NW_BAND_DCS_1800;
    if(s_band_freq_info.GSM_band[NW_BAND_PCS_1900] ==TRUE)
        gsmband = gsmband|MMI_NW_BAND_PCS_1900;

#ifndef WIN32
    MNPHONE_SetGsmBand(gsmband, g_dual_sys);
#endif

    //save GSM band info into nv
    ENGSaveBandFreqInfo(&s_band_freq_info,g_dual_sys);
}

LOCAL MMI_RESULT_E ENGSetGSMBandWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E     result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T    ctrl_id = MMIENG_BAND_LISTBOX_CTRL_ID;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id, GSM_BAND_MAX, FALSE);
        GUILIST_SetMaxSelectedItem(ctrl_id,GSM_BAND_MAX);
        AppendSetGSMBandListItem(win_id, ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_KEYDOWN_OK:
    case MSG_APP_WEB:
    case MSG_APP_MENU:
        break;

    case MSG_CTL_CANCEL:
    case MSG_KEYDOWN_CANCEL:
        ENGSetGSMBandSetInfo(ctrl_id);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_LIST_CHECK_CONTENT:
        break;

    case MSG_CLOSE_WINDOW:
        ENGSetGSMBandSetInfo(ctrl_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

//LTE freq Handle
BOOLEAN ENG_Freq_IsFreqValid(uint32 freq)
{
    int i=0,index=0;
    BOOLEAN ret= FALSE;
    BOOLEAN select_item[LTE_BAND_NUM]={FALSE};

    for (index = 0;  index< LTE_BAND_NUM; index++ )
    {
        for(i=0;i<g_lte_band_num;i++)
        {
            if(LTE_Band_Value[index] == s_band_freq_info.LTE_band[i])
            {
                select_item[index]= TRUE;
                break;
            }
        }
    }

    for(i=0;i<LTE_BAND_NUM;i++)
    {
        if(select_item[i])
        {
            if(freq >= LTE_freq_range[i][0] && freq <= LTE_freq_range[i][1])
            {
                ret = TRUE;
                break;
            }
        }
    }
    return ret;
}

BOOLEAN ENG_IsGsmFreqValid(uint32 freq)
{
    int i=0;
    BOOLEAN ret= FALSE;

    for(i=0;i<GSM_BAND_MAX;i++)
    {
        if(s_band_freq_info.GSM_band[i] == TRUE)
        {
            if(freq >=GSM_freq_range[i][0] && freq <= GSM_freq_range[i][1])
            {
                ret = TRUE;
                break;
            }
            if(i == NW_BAND_GSM_900)
            {
                 if(freq >=GSM_freq_range[GSM_BAND_MAX][0] && freq <= GSM_freq_range[GSM_BAND_MAX][1])
                 {
                     ret = TRUE;
                     break;
                }
             }
         }
    }
    return ret;
}

PUBLIC BOOLEAN  ENG_Freq_IsFreqExist(uint16 index)
{
    BOOLEAN ret = FALSE;

    if(index < ENG_FREQ_MAX_NUM)
    {
        if(g_freq_type == ENG_FREQ_LTE)
        {
            if(ENG_Freq_IsFreqValid(s_band_freq_info.Freq_list[index]))
            {
                ret = TRUE;
            }
        }
        else
        {
            if(ENG_IsGsmFreqValid(s_band_freq_info.GSM_Freq[index]))
            {
                ret = TRUE;
            }
        }
    }
    SCI_TRACE_LOW("[ENG] ENG_Freq_IsFreqExist ret %d",ret);
    return ret;
}

PUBLIC uint16  ENG_GetValidFreqCount(void)
{
    uint16 count = 0;
    uint16  i  =0;

    for(i = 0; i < ENG_FREQ_MAX_NUM; i++)
    {
       if(ENG_Freq_IsFreqExist(i))
       {
           count++;
       }
    }
    SCI_TRACE_LOW("[ENG] ENG_GetValidFreqCount count %d",count);
    return count;
}

LOCAL void SetCurrentSelFreq(uint16 ch_no)
{
    s_current_sel_freq= ch_no;
    SCI_TRACE_LOW("SetCurrentSelFreq ch_no %d",ch_no);
}

LOCAL uint16 GetCurrentSelFreq(void)
{
    SCI_TRACE_LOW("GetCurrentSelFreq s_current_sel_channel %d",s_current_sel_freq);
    return s_current_sel_freq;
}

PUBLIC void ENG_GetFreqDispName(uint32 freq, uint16 index,wchar *disp_name_ptr, uint16 max_name_len)
{
    uint8 disp_name[ENG_FREQ_MAX_LEN] = {0};
    uint8 freq_str[ENG_FREQ_MAX_LEN  +1] = {0};
    wchar wfreq_str[ENG_FREQ_MAX_LEN  +1] = {0};
    uint16 len = 0;
    MMI_STRING_T str_t = {0};
    BOOLEAN    freq_exist = FALSE;

    if(PNULL == disp_name_ptr || 0 == max_name_len)
    {
        SCI_TRACE_LOW("[MMIFM] GetChannelDispName disp_name_ptr %d max_name_len %d ", disp_name_ptr, max_name_len);
        return;
    }

    if(g_freq_type == ENG_FREQ_LTE)
    {
        if(!ENG_Freq_IsFreqValid(freq))
        {
            freq_exist = FALSE;
        }
        else
        {
            freq_exist = TRUE;
        }
    }
    else
    {
        if(!ENG_IsGsmFreqValid(freq))
        {
            freq_exist = FALSE;
        }
        else
        {
            freq_exist = TRUE;
        }
    }
    //"FREQXX("
    sprintf((char*)disp_name, "%s%d%s",ENG_FREQ_TITLE,index + 1,"(");
    len = (uint16)strlen((char*)disp_name);

    if(len <= max_name_len)
    {
        MMI_STRNTOWSTR(disp_name_ptr, max_name_len, disp_name, max_name_len, len);
    }
    if(!freq_exist)
    {
        //channel not exist
        MMI_GetLabelTextByLang(TXT_BLANK, &str_t);
    }
    else
    {
        // display frequency text
        sprintf((char *)freq_str,"%d",freq);
        str_t.wstr_len = strlen((char*)freq_str);
        str_t.wstr_len = MIN(str_t.wstr_len, ENG_FREQ_MAX_LEN);
        str_t.wstr_ptr = wfreq_str;
        MMI_STRNTOWSTR(str_t.wstr_ptr, ENG_FREQ_MAX_LEN, freq_str, ENG_FREQ_MAX_LEN, str_t.wstr_len);
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

LOCAL void ReadFreqToListBox(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    uint16 i = 0;
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    GUILIST_SetMaxItem(ctrl_id,ENG_FREQ_MAX_NUM, FALSE );
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    item_data.softkey_id[2] = (MMI_TEXT_ID_T)STXT_RETURN;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    for(i = 0; i < ENG_FREQ_MAX_NUM; i++)
    {
        SCI_MEMSET(temp_wstr, 0x00, sizeof(wchar)*(GUILIST_STRING_MAX_NUM + 1));
        {
            item_data.softkey_id[0] = (MMI_TEXT_ID_T)STXT_OPTION;
            if(g_freq_type == ENG_FREQ_LTE)
            {
                ENG_GetFreqDispName(s_band_freq_info.Freq_list[i], i,temp_wstr, GUILIST_STRING_MAX_NUM);
            }
            else
            {
                ENG_GetFreqDispName(s_band_freq_info.GSM_Freq[i], i,temp_wstr, GUILIST_STRING_MAX_NUM);
            }
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(item_data.item_content[0].item_data.text_buffer.wstr_ptr);
            item_t.user_data = i;
            GUILIST_AppendItem( ctrl_id, &item_t);
        }
    }
}

LOCAL void CreateEngFreqOptionMenu(uint32 ctr_id)
{
    MMI_STRING_T  kstring = {0};
    uint16        node_index =0;
    uint32         i = 0;
    uint32        item_num = 0;
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    item_num = ARR_SIZE(s_eng_freq_opt_item);

    SCI_TRACE_LOW("CreateEngFreqOptionMenu enter");
    for(i = 0; i < item_num; i++)
    {
        MMI_GetLabelTextByLang(s_eng_freq_opt_item[i][ENG_FREQ_POP_MENU_TEXT_ID], &kstring);
        node_item.item_text_ptr = &kstring;
        GUIMENU_InsertNode(node_index, s_eng_freq_opt_item[i][ENG_FREQ_POP_MENU_NODE_ID], 0, &node_item, ctr_id);
        if(!ENG_Freq_IsFreqExist(GetCurrentSelFreq()))
        {
            if(ENG_FREQ_DELETE == s_eng_freq_opt_item[i][ENG_FREQ_POP_MENU_NODE_ID])
            {
                //empty freq can not delete
                GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
            }
        }

        if(ENG_GetValidFreqCount() == 0)
        {
            //no freq, need not to delete
            if(ENG_FREQ_DELETE_ALL == s_eng_freq_opt_item[i][ENG_FREQ_POP_MENU_NODE_ID])
            {
                GUIMENU_SetNodeGrayed(TRUE, node_index, 0,  ctr_id);
            }
        }
        node_index ++;
    }
}

LOCAL uint32 ConvertWString2Digit(uint8 *str_ptr,uint16 length )
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < length*2; i=i+2)
    {
        num = (num * 10 + (str_ptr[i] - '0'));
    }
    return num;
}

LOCAL MMI_RESULT_E HandleFreqEditBoxWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_HANDLE_T        ctrl_handle = MMK_GetCtrlHandleByWin(win_id,MMIENG_FREQ_EDIT_CTRL_ID);
    MMI_CTRL_ID_T       ctrl_id = MMIENG_FREQ_EDIT_CTRL_ID;
    MMI_TEXT_ID_T       title_id = 0;
    MMI_RESULT_E        result   = MMI_RESULT_TRUE;
    MMI_STRING_T        str_info = {0};
    MMI_STRING_T        str_t    = {0};
    BOOLEAN             str_is_valid = TRUE;
    uint32              edit_freq = 0;
    uint16              index=0;
    wchar               wstr[20] = {0};
    char                temp_str[20] = {0};
    uint16              length= 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
            index= GetCurrentSelFreq();
            SCI_MEMSET(temp_str, 0 , length);
            SCI_MEMSET(wstr, 0 , length*2);
            if(g_freq_type == ENG_FREQ_LTE)
            {
                if(s_band_freq_info.Freq_list[index]!= 0xFFFFFFFF)
                    length = sprintf(temp_str, "%d", s_band_freq_info.Freq_list[index]);
            }
            else
            {
                if(s_band_freq_info.GSM_Freq[index]!= 0xFFFFFFFF)
                    length = sprintf(temp_str, "%d", s_band_freq_info.GSM_Freq[index]);
            }
            MMI_STRNTOWSTR(wstr, 20, (uint8*)temp_str, length, length);
            GUIEDIT_SetString(MMIENG_FREQ_EDIT_CTRL_ID, wstr,length);

            MMK_SetAtvCtrl(win_id, ctrl_handle);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        // 从EDITBOX中获得数据
        GUIEDIT_GetString(ctrl_id, &str_info);
            if (str_info.wstr_len > 0 && PNULL != str_info.wstr_ptr)
            {
                str_t.wstr_len = str_info.wstr_len;
                str_t.wstr_ptr = str_info.wstr_ptr;
                edit_freq = ConvertWString2Digit(str_t.wstr_ptr,str_info.wstr_len);
            }

        if(g_freq_type == ENG_FREQ_LTE)
        {
            if(ENG_Freq_IsFreqValid(edit_freq))
            {
                index= GetCurrentSelFreq();
                s_band_freq_info.Freq_list[index]=edit_freq;
                if (MMK_IsOpenWin(MMIENG_SET_FREQ_WIN_ID))
                {
                    MMK_SendMsg(MMIENG_SET_FREQ_WIN_ID, MSG_ENG_FREQ_EDIT_UPDATE,NULL);
                }
                MMK_CloseWin(win_id);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ENG_INVALID_FREQ);
            }
        }
        else
        {
            if(ENG_IsGsmFreqValid(edit_freq))
            {
                index= GetCurrentSelFreq();
                s_band_freq_info.GSM_Freq[index]=edit_freq;
                if (MMK_IsOpenWin(MMIENG_SET_FREQ_WIN_ID))
                {
                    MMK_SendMsg(MMIENG_SET_FREQ_WIN_ID, MSG_ENG_FREQ_EDIT_UPDATE,NULL);
                }
                MMK_CloseWin(win_id);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ENG_INVALID_FREQ);
            }
        }
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

LOCAL MMI_RESULT_E HandleEngFreqOptionMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
   MMI_RESULT_E            result = MMI_RESULT_TRUE;
   uint32                  menu_id = 0;
   uint16 index=0;
   SCI_TRACE_LOW("HandleEngFreqOptionMsg msg_id=%d", msg_id);

   switch (msg_id)
   {
   case MSG_OPEN_WINDOW:
       SCI_TRACE_LOW("HandleEngFreqOptionMsg MSG_OPEN_WINDOW");
       CreateEngFreqOptionMenu(MENU_POPUP_CTRL_ID);
       MMK_SetAtvCtrl(win_id, MENU_POPUP_CTRL_ID);
       break;

   case MSG_APP_OK:
   case MSG_CTL_OK:
   case MSG_APP_WEB:
   case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
   case MSG_CTL_PENOK:
#endif
       menu_id = GUIMENU_GetCurNodeId(MENU_POPUP_CTRL_ID);
       SCI_TRACE_LOW("HandleEngFreqOptionMsg menu_id=%d", menu_id);
       switch(menu_id)
       {
       case ENG_FREQ_EDIT:
            //edit this freq
            MMK_CreateWin((uint32 *)ENG_FREQ_EDIT_WIN_TAB, PNULL);
            break;
       case ENG_FREQ_DELETE:
            //delete this freq
            index= GetCurrentSelFreq();
            if(g_freq_type == ENG_FREQ_LTE)
            {
                s_band_freq_info.Freq_list[index] = 0xFFFFFFFF;
            }
            else
            {
                s_band_freq_info.GSM_Freq[index] = 0xFFFFFFFF;
            }
            // if(index > ENG_FREQ_MAX_NUM)
            //SetCurrentSelFreq(index);
            if (MMK_IsOpenWin(MMIENG_SET_FREQ_WIN_ID))
            {
                MMK_SendMsg(MMIENG_SET_FREQ_WIN_ID, MSG_ENG_FREQ_LIST_UPDATE,NULL);
            }
            //SetCurrentOperator(MMIFM_DELETE_ONE_CHANNEL);
            //MMIPUB_OpenQueryWinByTextId(TXT_FM_DELETE_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin);
           break;
       case ENG_FREQ_DELETE_ALL:
            //delete all freq
            if(g_freq_type == ENG_FREQ_LTE)
            {
               memset((s_band_freq_info.Freq_list),0xFFFFFFFF,sizeof(s_band_freq_info.Freq_list));
            }
            else
            {
                memset((s_band_freq_info.GSM_Freq),0xFFFFFFFF,sizeof(s_band_freq_info.GSM_Freq));
            }
            SetCurrentSelFreq(0);
            if (MMK_IsOpenWin(MMIENG_SET_FREQ_WIN_ID))
            {
                MMK_SendMsg(MMIENG_SET_FREQ_WIN_ID, MSG_ENG_FREQ_LIST_UPDATE,NULL);
            }
            // SetCurrentOperator(MMIFM_DELETE_ALL_CHANNEL);
            // MMIPUB_OpenQueryWinByTextId(TXT_FM_DELETE_ALL_CHA_QUERY, IMAGE_PUBWIN_QUERY, &query_id, HandleQuerywin);
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

LOCAL MMI_RESULT_E ENGSetFreqWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T                str_t= {0};
    const GUILIST_ITEM_T        *list_item_ptr = PNULL;
    static uint16               s_cur_list_index = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    uint16                      i = 0;
    uint8 number = 0;
    MN_StoredPlmnList plmnlist ={0};
    SCI_TraceLow("ENGSetFreqWinHandleMsg msg_id=%d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SCI_TraceLow("ENGSetFreqWinHandleMsg MSG_OPEN_WINDOW");
        s_cur_list_index = 0;//high light first item
        MMI_GetLabelTextByLang(TXT_ENG_SET_FREQ, &str_t);
        ReadFreqToListBox(win_id, MMIENG_FREQ_LISTBOX_CTRL_ID);/*lint !e64*/
        GUILIST_SetCurItemIndex(MMIENG_FREQ_LISTBOX_CTRL_ID, s_cur_list_index);
        MMK_SetAtvCtrl(win_id, MMIENG_FREQ_LISTBOX_CTRL_ID);
        GUIWIN_SetTitleText(win_id, str_t.wstr_ptr, str_t.wstr_len, FALSE);
        break;
    case MSG_ENG_FREQ_EDIT_UPDATE:
        SCI_TraceLow("ENGSetFreqWinHandleMsg MSG_ENG_FREQ_EDIT_UPDATE");
        //update current list
        MMI_GetLabelTextByLang(TXT_ENG_SET_FREQ, &str_t);
        GUIWIN_SetTitleText(win_id, str_t.wstr_ptr, str_t.wstr_len, FALSE);
        ReadFreqToListBox(win_id, MMIENG_FREQ_LISTBOX_CTRL_ID);/*lint !e64*/
        for(i = 0; i < GUILIST_GetTotalItemNum(MMIENG_FREQ_LISTBOX_CTRL_ID); i++)
        {
            list_item_ptr = GUILIST_GetItemPtrByIndex(MMIENG_FREQ_LISTBOX_CTRL_ID, i);
            //SCI_ASSERT(PNULL != list_item_ptr);/*assert verified*/
            if(PNULL == list_item_ptr)
            {
                break;
            }

            if(GetCurrentSelFreq() == (uint16)list_item_ptr->user_data)
            {
                s_cur_list_index = i;
                break;
            }
        }
        GUILIST_SetCurItemIndex(MMIENG_FREQ_LISTBOX_CTRL_ID, s_cur_list_index);
        MMK_SetAtvCtrl(win_id, MMIENG_FREQ_LISTBOX_CTRL_ID);
        //call interface
        //ENGSetLteLockFreq(g_dual_sys,&(s_band_freq_info.Freq_list));
        number = ENG_GetValidFreqCount();
        if(g_freq_type == ENG_FREQ_LTE)
        {
#ifndef WIN32
            MNPHONE_SetLteLockFreq(number,(s_band_freq_info.Freq_list),g_dual_sys);
#endif
            SCI_TraceLow("zhouqin set freq number = %d",number);
            for(i=0;i< ENG_FREQ_MAX_NUM;i++)
            SCI_TraceLow("zhouqin set freq Freq_list[%d] = %d",i,s_band_freq_info.Freq_list[i]);
        }
        else
        {
            memset(&plmnlist,0x0,sizeof(plmnlist));
            for(i = 0; i < ENG_FREQ_MAX_NUM; i++)
            {
                if(ENG_Freq_IsFreqExist(i))
                {
                    plmnlist.ArfcnL[plmnlist.nCount]= (uint16)(s_band_freq_info.GSM_Freq[i]);
                    plmnlist.nCount++;
                }
            }
            SCI_TraceLow("zhouqin set gsm freq plmnlist.nCount = %d",plmnlist.nCount);
            for(i=0;i< plmnlist.nCount;i++)
            {
                SCI_TraceLow("zhouqin set gsm freq gsm_Freq[%d] = %d",i,plmnlist.ArfcnL[i]);
            }

#ifndef WIN32
        MNPHOE_CfgSetStoredPlmnList(&plmnlist, g_dual_sys);
        MNPHONE_NwSetLockBCCH(TRUE, g_dual_sys);
#endif
        }
         //save nv
         ENGSaveBandFreqInfo(&s_band_freq_info,g_dual_sys);

        break;
    case MSG_ENG_FREQ_LIST_UPDATE:
        SCI_TraceLow("ENGSetFreqWinHandleMsg MSG_ENG_FREQ_LIST_UPDATE");
        s_cur_list_index = GetCurrentSelFreq();//high light first item
        MMI_GetLabelTextByLang(TXT_ENG_SET_FREQ, &str_t);

        ReadFreqToListBox(win_id, MMIENG_FREQ_LISTBOX_CTRL_ID);/*lint !e64*/
        GUILIST_SetCurItemIndex(MMIENG_FREQ_LISTBOX_CTRL_ID, s_cur_list_index);
        MMK_SetAtvCtrl(win_id, MMIENG_FREQ_LISTBOX_CTRL_ID);
        GUIWIN_SetTitleText(win_id, str_t.wstr_ptr, str_t.wstr_len, FALSE);
        //call interface
        //ENGSetLteLockFreq(g_dual_sys,&(s_band_freq_info.Freq_list));
        number = ENG_GetValidFreqCount();
        if(g_freq_type == ENG_FREQ_LTE)
        {
#ifndef WIN32
            MNPHONE_SetLteLockFreq(number,(s_band_freq_info.Freq_list),g_dual_sys);
#endif
        }
        else
        {
            memset(&plmnlist,0x0,sizeof(plmnlist));
            for(i = 0; i < ENG_FREQ_MAX_NUM; i++)
            {
                if(ENG_Freq_IsFreqExist(i))
                {
                    plmnlist.ArfcnL[plmnlist.nCount]= (uint16)(s_band_freq_info.GSM_Freq[i]);
                    plmnlist.nCount++;
                }
            }
            SCI_TraceLow("zhouqin set gsm freq plmnlist.nCount = %d",plmnlist.nCount);
            for(i=0;i< plmnlist.nCount;i++)
            {
                SCI_TraceLow("zhouqin set gsm freq gsm_Freq[%d] = %d",i,plmnlist.ArfcnL[i]);
            }

#ifndef WIN32
            MNPHOE_CfgSetStoredPlmnList(&plmnlist, g_dual_sys);
            MNPHONE_NwSetLockBCCH(TRUE, g_dual_sys);
#endif
        }
        //save nv
        ENGSaveBandFreqInfo(&s_band_freq_info,g_dual_sys);
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
    case MSG_CTL_MIDSK:
        SCI_TraceLow("ENGSetFreqWinHandleMsg MSG_APP_OK");
        s_cur_list_index = GUILIST_GetCurItemIndex(MMIENG_FREQ_LISTBOX_CTRL_ID);
        list_item_ptr = GUILIST_GetItemPtrByIndex(MMIENG_FREQ_LISTBOX_CTRL_ID, s_cur_list_index);
        //SCI_ASSERT(PNULL != list_item_ptr);/*assert verified*/
        if(PNULL == list_item_ptr)
        {
            break;
        }
        SetCurrentSelFreq((uint16)list_item_ptr->user_data);
        //option
        MMK_CreateWin((uint32 *)ENG_FREQ_OPTION_WIN_TAB,PNULL);

        //creat dynamic menu
        GUIMENU_CreatDynamic(PNULL,ENG_FREQ_OPTION_WIN_ID,MENU_POPUP_CTRL_ID,GUIMENU_STYLE_POPUP);
        break;

    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        s_cur_list_index = GUILIST_GetCurItemIndex(MMIENG_FREQ_LISTBOX_CTRL_ID);
        list_item_ptr = GUILIST_GetItemPtrByIndex(MMIENG_FREQ_LISTBOX_CTRL_ID, s_cur_list_index);
        //SCI_ASSERT(PNULL != list_item_ptr);/*assert verified*/
        if(PNULL == list_item_ptr)
        {
            break;
        }
        //SetCurrentSelChannel((uint16)list_item_ptr->user_data);
        SetCurrentSelFreq((uint16)list_item_ptr->user_data);
        //option
        MMK_CreateWin((uint32 *)ENG_FREQ_OPTION_WIN_TAB,PNULL);

        //creat dynamic menu
        GUIMENU_CreatDynamic(PNULL,ENG_FREQ_OPTION_WIN_ID,MENU_POPUP_CTRL_ID,GUIMENU_STYLE_POPUP);

        //MMK_CloseWin(win_id);
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
/*Edit by script, ignore 4 case. Thu Apr 26 19:00:53 2012*/ //IGNORE9527


/*Edit by script, ignore 19 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
