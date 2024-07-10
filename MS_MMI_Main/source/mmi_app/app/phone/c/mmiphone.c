/*****************************************************************************
** File Name:      mmiphone.c                                                *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe phone module                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2003       Jassmine       Create
** 03/2004       Jassmine.Meng     Modify
******************************************************************************/

#define _MMIPHONE_C_



/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_phone_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_signal_ext.h"
#include "mmiphone_export.h"
#include "mmiphone_internal.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "tb_dal.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmisms_text.h"
#include "mmiidle_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmicl_export.h"
#include "mmi_default.h"
#include "mmialarm_export.h"
#include "mmiset_export.h"
#include "mmiset_call_export.h"
#include "mmiacc.h"
#include "guilcd.h"
//#include "guilabel.h"
#include "guilistbox.h"
#include "guianim.h"
#include "dal_chr.h"
#include "mmiudisk_export.h"
#include "mmi_appmsg.h"
#include "mmimms_export.h"
#include "mmimp3_export.h"
#include "mmiacc.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmi_imagefun.h"
#if defined(MMI_MSD_SUPPORT)
#include "mmimsd_export.h"
#endif
#include "mmi_textfun.h"
#include "mmicc_export.h"
#include "mmicc_id.h"
#include "mn_api.h"
#include "mmiidle_export.h"
#include "sig_code.h"
#include "dal_power.h"
#include "mmi_wallpaper_export.h"
#include "mmiengtd_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef WIN32
#include "mmiudisk_simu.h"
#else
#include "usbservice_api.h"
#include "gpio_prod_api.h"
#endif
#include "sci_api.h"
#include "mmipub.h"
#include "mmiidle_subwintab.h"
#include "mmisd_export.h"
#ifdef AOC_SUPPORT
#include "mmicl_costs_wintab.h"
#endif
#include "mmifmm_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif

#ifdef TOUCH_PANEL_SUPPORT
#include "tp_export.h"
#endif
//#include "mmieng.h"
#include "mmieng_export.h"
#ifdef ENG_SUPPORT
#include "mmieng_nv.h"
#endif
#include "mn.h"
#include "mmifm_export.h"
#include "mmivcard_export.h"
#include "mmiset_nv.h"
#include "mn_type.h"
//macro replace
//#ifdef MMI_STREAMING_ENABLE
#ifdef STREAMING_SUPPORT_NEXTREAMING
#include "mmist_id.h"
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#include "mmiset_multisim_export.h"
#endif
#include "mn_api.h"

#include "mmipb_export.h"
#include "mmidc_export.h"
//#include "mmicountedtime_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmisms_export.h"
#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif
#include "guiedit.h"
#include "guires.h"
#include "mmipicview_export.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#ifndef WIN32
#ifndef MMI_DUALMODE_ENABLE
#include "atc_ppp.h"/*lint -e766*/
#endif
#endif
#include "mmistk_export.h"
//#include "brw_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef MMI_ENABLE_DCD
#include "mmidcd_export.h"
#endif
#include "mmidm_export.h"
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
//#ifdef CMMB_SUPPORT //bug 2100875
#include "mmimtv_export.h"
//#endif
#ifdef MBBMS_SUPPORT
#include "mmimbbms_wintab.h"
#endif

#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif

#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#ifdef TTS_SUPPORT
#ifndef WIN32
//#include"mmiebook.h"
#include "mmiebook_export.h"
#include "mmitts_export.h"
#include "mmiebook_id.h"
#endif
#endif
#ifdef HERO_ENGINE_TTS_SUPPORT 
#ifndef WIN32
#include "mmitts_export.h"
#endif
#endif

#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

#ifdef QQ_SUPPORT
#include "mmiqq_export.h"
#endif
#ifdef ENGTD_SUPPORT
#include "mmieng_dynamic_main.h"
#endif
#if defined(MMI_UPDATE_TIME_SUPPORT)
#include "mmiaut_export.h"
#endif /*MMI_UPDATE_TIME_SUPPORT*/
#include "mmiphone_nitz.h"
 
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif

#include "mmiudisk_export.h"

#include "in_message.h"
#ifdef DRM_SUPPORT
#include "mmidrm.h"
#endif

#ifdef IKEYBACKUP_SUPPORT
#include "mmiikeybackup_api.h"
#endif

#ifdef MMI_VCALENDAR_SUPPORT
#include "Mmialarm_id.h"
#include "Mmivcalendar.h"
#include "mmischedule_export.h"
#endif

#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif

#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif

#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif

//#ifdef MMI_DUAL_BATTERY_SUPPORT
#include "dualbat_drvapi.h"
//#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
#include "mmicalendar_export.h"
#include "mmiphone_charge.h"
#include "mmiidle_statusbar.h"
#ifdef BROWSER_SUPPORT_DORADO
#include "mmibrowser_dorado_api.h"
#endif
#ifdef MMI_CSTAR_UDB_SUPPORT
#include "mmiim.h"
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#include "mmi_autotest.h"
#ifdef ADULT_WATCH_SUPPORT
#include "watch_steps.h"
#endif
#ifdef ADULT_WATCH_SUPPORT
#ifdef UAL_SS_SUPPORT
#include "ual_tele.h"
#endif
#include "watch_set_call_forwarding.h"
#include "set_ss_flow.h"
#endif
#ifdef UAL_NETWORKMGR_SUPPORT
#include "ual_networkmgr.h"
#endif
#ifdef SIM_PLUG_IN_SUPPORT
#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif
#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
#include "mmi_default.h"
#endif
#endif

#ifdef UAL_TELE_SUPPORT
#include "ual_tele_sim.h"
#include "ual_tele_radio.h"
#include "ual_tele_data.h"
#endif
#include "mmicc_internal.h"
#include "mmistk_win.h"
#include "ual_device.h"
#include "watch_commonwin_export.h"
#ifdef SFR_SUPPORT_CMCC
#include "mmisfr_cmcc_main.h"
#endif
#ifdef ZDT_APP_SUPPORT
#include "zdt_app.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MAX_NETWORK_NUM (ARR_SIZE(s_network_nametable))

#define MAX_NETWORK_MNC_DIGIT_NUM_3_NUM (ARR_SIZE(s_network_mnc_digit_num_3_nametable))//mnc_digit_num=3

#define MAX_MCC_MNC_DIGIT_DISPLAY 20

#define PHONE_RX_SIG_DEFAULT_LEVEL 5

#ifdef MMI_SIM_NODIGITNUM_SUPPORT
#define MCC_MNC_DIGIT_INVALID 0xFF    
#endif

#ifdef ENG_SUPPORT
#define MMIENG_STANDBY_TIME_COUNT 1000
#endif

#define NONE_SIM_EXISTED  0
#define SIM1_EXISTED      1<<0
#define SIM2_EXISTED      1<<1

#define  CBNET_CHINA_MCC  460
#define  CBNET_CHINA_MNC  15

LOCAL BOOLEAN s_phone_is_power_off_req[MMI_DUAL_SYS_MAX];
#ifdef ADULT_WATCH_SUPPORT
LOCAL BOOLEAN s_interrogate_done = FALSE;
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


LOCAL const MMI_TEXT_ID_T s_sim_status_string[SIM_STATUS_MAX + 1] = 
{
    TXT_NULL,                               //SIM_STATUS_OK,
    TXT_SIM_REJECTED,               //SIM_STATUS_REJECTED,
#ifndef MMI_IDLE_MINI_SUPPORT
    TXT_SIM_REGISTRATION_FAILED,//SIM_STATUS_REGISTRATION_FAILED,
#else
    TXT_SIM_REGISTRATION_FAILED_MINI,//SIM_STATUS_REGISTRATION_FAILED,
#endif

    TXT_PIN_BLOCKED,                //SIM_STATUS_PIN_BLOCKED
    TXT_NOT_SUPPORT_SIM,	//SIM_STATUS_NOT_SUPPORT_SIM, //@zhaohui,cr109170                                                            
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK_BLOCKED,
    TXT_PUK_BLOCKED,                //SIM_STATUS_PUK2_BLOCKED,
#ifndef MMI_IDLE_MINI_SUPPORT
    TXT_INSERT_SIM,                   //SIM_STATUS_NO_SIM,
#else
    TXT_INSERT_SIM_MINI,            //SIM_STATUS_NO_SIM,
#endif                
    TXT_COMMON_SEARCHING, //SIM_NOT_INITED
#if defined(MMI_SIM_LOCK_SUPPORT)
    TXT_SIM_LOCKED,                                //SIM_LOCKED
#endif    
    TXT_NULL                                //SIM_STATUS_MAX
};



LOCAL const MMI_NETWORK_ACI_INFO_T s_network_acitable[] = 
{
    001, "001",
    460, "CHN",//China
    466, "TPE",//Chinese Taipei
    886, "TPE",//Chinese Taipei
    455, "MAC",//Macao China
    454, "HKG",//Hongkong China
//    188, "188",
    202, "GRE",///Greece
    204, "NED",//Netherlands
    206, "BEL",//Belgium
    208, "FRA",//France
    213, "AND",//Andorra
    214, "ESP",//Spain
    216, "HUN",//Hungary
    218, "BIH",//Bosnia
    219, "CRO",//Croatia
    220, "SCG",//Serbia
    222, "ITA",//Italy
    226, "ROM",//Romania
    228, "SUI",//Switzerland
    230, "CZE",//Czech Republic
    231, "SVK",//Slovakia
    232, "AUT",//Austria
//    234, "234",//Guernsey
    238, "DEN",//Denmark
    240, "SWE",//Sweden
    242, "NOR",//Norway
    244, "FIN",//Finland
    246, "LTU",//Lithuania
    247, "LAT",//Latvia
    248, "EST",//Estonia
    250, "RUS",//Russian
    255, "UKR",//Ukraine
    257, "BLR",//Belarus
    259, "MDA",//Moldova
    260, "POL",//Poland
    262, "GER",//Germany
    266, "GIB",//Gibraltar
    268, "POR",//Portugal
    270, "LUX",//Luxembourg
    272, "IRL",//Ireland
    274, "ISL",//Iceland
    276, "ALB",//Albania
    278, "MLT",//Malta
    280, "CYP",//Cyprus
    282, "GEO",//Georgia
    283, "ARM",//Armenia
    284, "BUL",//Bulgaria
    286, "TUR",//Turkey
//    288, "288",//Faroe Islands
//    290, "290",//Greenland
//    293, "293",//Kosovo
    294, "MKD",//Republic of Macedonia
    295, "LIE",//Liechtenstein
    302, "CAN",//Canada
    310, "USA",//USA
    334, "MEX",//Mexico
    338, "ANT",//Antigua and Barbuda
//    340, "340",//Guadeloupe
    344, "ANT",//Antigua and Barbuda
//    350, "350",//Bermuda
//    362, "362",//Netherlands Antilles
    368, "CUB",//Cuba
    370, "DOM",//Dominican Republic
    400, "AZE",//Azerbaijan
    401, "KAZ",//Kazakhstan
    404, "IND",//India
    405, "IND",//India
    410, "PAK",//Pakistan
    412, "AFG",//Afghanistan
    413, "SRI",//Sri Lanka
    414, "MYA",//Myanmar
    415, "LIB",//Lebanon
    416, "JOR",//Jordan
    417, "SYR",//Syria
    419, "KUW",//Kuwait
    420, "KSA",//Saudi Arabia
    421, "YEM",//Yemen
    422, "OMA",//Oman
    424, "UAE",//United Arab Emirates
    425, "ISR",//Israel
    426, "BRN",//Bahrain
    427, "QAT",//Qatar
    428, "MGL",//Mongolia
    429, "NEP",//Nepal
    432, "IRI",//Iran
    434, "UZB",//Uzbekistan
    437, "KGZ",//Kyrgyzstan
    438, "TKM",//Turkmenistan
    440, "JAP",//JAPAN,NIPPON
    450, "KOR",//South Korea
    452, "VIE",//Vietnam
    456, "CAM",//Cambodia
    457, "LAO",//Laos
    470, "BAN",//Bangladesh
    472, "MDV",//Maldives
    502, "MAS",//Malaysia
    505, "AUS",//Australia
    510, "INA",//Indonesia
    515, "PHI",//Philippines
    520, "THA",//Thailand
    525, "SIN",//Singapore
    528, "BRU",//Brunei
    530, "NZL",//New Zealand
    539, "TGA",//Tonga
    541, "VAN",//Vanuatu
    542, "FIJ",//Fiji
//    544, "544",
//    546, "546",//New Caledonia
//    547, "547",//French Polynesia
    550, "FSM",//Federated States of Micronesia
    602, "EGY",//Egypt
    603, "ALG",//Algeria
    604, "MAR",//Morocco
    605, "TUN",//Tunisia
    607, "GAM",//Gambia
    608, "SEN",//Senegal
    610, "MLI",//Mali
    611, "GUI",//Guinea
    612, "COT",//Ivory Coast,Cote D'lvoire
    613, "BUR",//Burkina Faso
    614, "NIG",//Niger
    615, "TOG",//Togo
    616, "BEN",//Benin
    617, "MRI",//Mauritius
    618, "LBR",//Liberia
    620, "GHA",//Ghana
    621, "NGR",//Nigeria
    622, "CHA",//Chad
    624, "CMR",//Cameroon
    625, "CPV",//Cape Verde
    628, "GAB",//Gabon
    629, "CGO",//Republic of the Congo
    630, "COD",//Democratic Republic of the Congo
    631, "ANG",//Angola
    633, "SEY",//Seychelles
    634, "SUD",//Sudan
    635, "RWA",//Rwanda
    636, "ETH",//Ethiopia
    637, "SOM",//Somalia
    639, "KEN",//Kenya
    640, "TAN",//Tanzania
    641, "UGA",//Uganda
    642, "BDI",//Burundi
    643, "MOZ",//Mozambique
    645, "ZAM",//Zambia
    646, "MAD",//Madagascar
    647, "647",//Réunion
    648, "648",//Zimbabwe
    649, "NAM",//Namibia
    650, "MAW",//Malawi
    651, "LES",//Lesotho
    652, "BOT",//Botswana
    653, "SWZ",//Swaziland
    655, "RSA",//South Africa
    704, "GUA",//Guatemala
    706, "ESA",//El Salvador
    708, "HON",//Honduras
    710, "NIC",//Nicaragua
    714, "PAN",//Panama
    716, "PER",//Peru
    722, "ARG",//Argentina
    724, "BRA",//Brazil
    730, "CHI",//Chile
    734, "VEN",//Venezuela
    736, "BOL",//Bolivia
    740, "ECU",//Ecuador
    744, "PAR",//Paraguay
    746, "SUR",//Suriname
};

LOCAL const MMI_NETWORK_NAME_T s_network_nametable[] =
{
    001,01,TXT_NET_TEST,PLMN_TEST,
    460,0,qtn_operator_china_mobile,PLMN_460_00,
    460,1,qtn_operator_china_unicom,PLMN_460_01,
    460,2,qtn_operator_china_mobile,PLMN_460_02,
    460,3,qtn_operator_chn_ct,PLMN_460_03,
    460,4,qtn_operator_china_mobile,PLMN_460_04,
    460,7,qtn_operator_china_mobile,PLMN_460_07,
    460,8,qtn_operator_china_mobile,PLMN_460_08,
    460,9,qtn_operator_china_unicom,PLMN_460_09,
    460,11,qtn_operator_chn_ct,PLMN_460_11,
    460,15,qtn_operator_china_broadcast_network,PLMN_460_15,
    #include "TS25_plmn_table.h"

};

LOCAL const MMI_NETWORK_NAME_T s_network_mnc_digit_num_3_nametable[] =//mnc_digit_num=3
{

    #include "TS25_plmn_table_3digits.h"

};

typedef enum
{
    MMIPHONE_PS_POWER_ON_NONE,
    MMIPHONE_PS_POWER_ON_EMERGE,
    MMIPHONE_PS_POWER_ON_NORMAL
}MMIPHONE_PS_POWER_ON_TYPE;

#ifdef CSC_XML_SUPPORT
typedef struct
{
    uint16 net_work_num;
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *csc_network_info;
}MMI_CSC_NETWORK_NAME_T;
#endif

#ifdef BOCA_ROAMING_INDICATION
typedef struct
{
    uint16 mvno_mcc;
    uint16 mvno_mnc;
    uint16 host_mcc;
    uint16 host_mnc;
}MMI_MVNO_ROAMING_T;

#define MAX_MVNO_ROAMING_NUM (ARR_SIZE(s_mvno_roaming_table))

LOCAL const MMI_MVNO_ROAMING_T s_mvno_roaming_table[] =
{
    #include "mvno_list.h"
};
#endif

#define  MMI_IMS_TOGGLE_CONFIG_PARAMETER 1

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL PHONE_SIM_STATUS_T          s_sim_status[MMI_DUAL_SYS_MAX];        //the status of sim

LOCAL BOOLEAN               s_sim_existed_status[MMI_DUAL_SYS_MAX] = {0}; //变量用来记录SIM卡是否存在
LOCAL BOOLEAN s_is_power_flag = FALSE;

LOCAL char s_4g_icon_display_style = 0;
LOCAL char s_volte_icon_display_style [MMI_DUAL_SYS_MAX] = {0};
LOCAL char s_volte_icon_display_set[MMI_DUAL_SYS_MAX] = {0};
LOCAL char s_volte_menu_display_set[MMI_DUAL_SYS_MAX] = {0};
LOCAL char s_prefer_network_menu_display_style = 0;
LOCAL BOOLEAN s_is_indiasos_supported = FALSE;

#ifdef MMI_DUALMODE_ENABLE
LOCAL MMI_GMMREG_RAT_E  s_is_TD[MMI_DUAL_SYS_MAX];//MN_GMMREG_RAT_GPRS;
#endif
LOCAL MMIPHONE_HANDLE_PIN_STATUS_T s_handle_pin_status;         //add for dual sim
LOCAL BOOLEAN s_is_same_sim[MMI_DUAL_SYS_MAX] = {0};

#ifndef MMI_MULTI_SIM_SYS_SINGLE
LOCAL SIM_QUERY_STATUS_E s_query_sim_status = SIM_QUERY_STATUS_NONE;
#endif

#ifdef ZDT_NFC_SUPPORT
LOCAL BOOLEAN s_nfc_power_on_sim_init = TRUE;
#endif

LOCAL BOOLEAN s_is_power_on_sim_cnf[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL SIM_QUERY_STATUS_E s_query_sim_present_status = SIM_QUERY_STATUS_NONE;
LOCAL MMIPHONE_PS_POWER_ON_TYPE s_ps_ready[MMI_DUAL_SYS_MAX] = {0};/*lint !e64*/
LOCAL BOOLEAN s_is_ps_deactive_cnf[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL BOOLEAN s_is_ps_ready[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL BOOLEAN s_is_sim_present_ind[MMI_DUAL_SYS_MAX] = {FALSE};
LOCAL BOOLEAN s_is_network_status_ready[MMI_DUAL_SYS_MAX] = {FALSE};
#if defined(MMI_SMSCB_SUPPORT)
LOCAL BOOLEAN s_is_startup_smscb[MMI_DUAL_SYS_MAX] = {FALSE};
#endif
LOCAL MMIPHONE_NOTIFY_LIST_T *s_mmiphone_subscriber_list_ptr = PNULL;
LOCAL MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *s_mmiphone_sim_plug_subscriber_list_ptr = PNULL;
MN_DUAL_SYS_E s_plug_sim_id = MN_DUAL_SYS_1;
MMIPHONE_SIM_PLUG_EVENT_E s_plug_event = MMIPHONE_NOTIFY_SIM_PLUG_MAX;
LOCAL MMI_OPERATE_PIN_T s_operate_pin_blocked_info;//用于pin被锁后传递参数
LOCAL MN_DUAL_SYS_E s_cur_handling_sim_id = MMI_DUAL_SYS_MAX;
LOCAL MMI_HANDLE_T s_need_update_network_status_win = 0;//网络状态更新时候需要刷新网络信息的窗口
#ifdef MMI_SIM_LOCK_SUPPORT
LOCAL MN_SIM_LOCK_STATUS_T s_sim_unlock_config_data = {0};//记下 SIM lock 解锁用计数器和定时器的配置信息
LOCAL MMI_OPERATE_SIM_LOCK_T s_sim_lock_operate_value = {0}; 
LOCAL MMIPHONE_HANDLE_SIMLOCK_STATUS_T  s_handle_sim_lock_status = {0};//为多卡记住SIM Lock status
#ifdef MMI_RESET_PHONE_AUTO_PIN
LOCAL BOOLEAN s_use_pin_in_nv = FALSE;
#endif//#ifdef MMI_RESET_PHONE_AUTO_PIN
#endif
#ifdef MMI_SIM_LANGUAGE_SUPPORT
// 是否做过优选语言的动作。
LOCAL BOOLEAN s_is_language_auto_set = FALSE; 

//做优选语言的sim卡，系统运行中只选定一张固定的sim卡做操作。
LOCAL MN_DUAL_SYS_E s_prefer_language_sim = MN_DUAL_SYS_1; 
#endif

#ifdef CSC_XML_SUPPORT
LOCAL MMI_CSC_NETWORK_NAME_T *s_csc_network_name_ptr = PNULL;
#endif

#ifdef MMI_LDN_SUPPORT
uint16  s_ldn_flag = 0;
#endif

#ifdef MMI_SIM_NODIGITNUM_SUPPORT
       BOOLEAN s_is_autoadapt_flag[MMI_DUAL_SYS_MAX]= {FALSE};            
#endif

typedef enum
{
    MMIPHONE_SIM_READY_TO_PS_REASON_NOT_READY,
    MMIPHONE_SIM_READY_TO_PS_REASON_READY_BY_PIN,
    MMIPHONE_SIM_READY_TO_PS_REASON_READY_BY_SIM
}MMIPHONE_SIM_READY_TO_PS_REASON_E;

LOCAL MMIPHONE_SIM_READY_TO_PS_REASON_E g_is_sim_ready_to_ps[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN g_has_init_test_mode = FALSE;

LOCAL int16 s_poweron_modem_counter = 0;
LOCAL BOOLEAN s_is_need_poweron_modem = FALSE;
#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
LOCAL BOOLEAN s_logic_sim_status = TRUE;
LOCAL uint8   s_query_sim_timer_id = 0;
LOCAL uint16  s_sim_plug_timer_count = 0;
#define  TIMER_COUNT_LCD_TURN_OFF  60  // 60*5是灭屏时轮询SIM卡是否存在的时间
#define  MMI_5SECONDS              (MMI_1SECONDS*5)
#endif

LOCAL uint32 s_device_register_handle = 0; //监听device的handle
LOCAL uint8 s_thermal_power_off_timer = 0;
LOCAL BOOLEAN s_is_power_off = FALSE;

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
//extern MMI_SYSTEM_T g_mmi_system;
extern uint32   g_operate_pin_win_tab;
extern BOOLEAN g_is_operate_pin_special_func;
#ifdef SFR_SUPPORT
extern void MMISFR_RunGPRSModule(void);
#endif
#ifdef SFR_SUPPORT_CTCC
extern PUBLIC void MMISFR_CTCC_HandleNetworkStatus(void);
#endif

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_APPLICATION_T   g_mmiphone_app;         //phone的实体
PHONE_SERVICE_STATUS_T  g_service_status[MMI_DUAL_SYS_MAX];         //the status of service
MN_BLOCK_T g_pin_value[MMI_DUAL_SYS_MAX] ={0};
#ifdef DPHONE_SUPPORT 
BOOLEAN g_pin_lock_hookhandfree = FALSE;
#endif
BOOLEAN g_is_sim_only_check_status[MMI_DUAL_SYS_MAX] = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : create input pin window
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E CreateInputPinWindow(APP_MN_SIM_GET_PIN_IND_T sim_get_pin_ind);

/*****************************************************************************/
// 	Description : IsNeedSetGPRSAttachMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetGPRSAttachMode(void);

/*****************************************************************************/
// 	Description : 处理PHONE APP的PS消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePsAndRefMsg(
                                     PWND                app_ptr, 
                                     uint16              msg_id, 
                                     DPARAM              param
                                     );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETWORK_STATUS_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetworkStatusInd(
                                          DPARAM              param
                                          );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_LIST_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreferRatRsp( 
                                        DPARAM  param
                                        );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_SELECT_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectPlmnCnf(
                                       DPARAM              param
                                       );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_POWER_ON_CNF
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOnSimCnf(
                                       DPARAM              param
                                       );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_POWER_OFF_CNF
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOffSimCnf(
                                       DPARAM              param
                                       );

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PREFER_PLMN_CHANGE_CNF
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreferPlmnChangeCnf(
                                       DPARAM              param
                                       );
#endif

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PS_POWER_OFF_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerPsOffCnf(
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_DEACTIVE_PS_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeactivePsCnf(
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_LIST_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlmnListCnf(
                                     DPARAM              param
                                     );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SET_BAND_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectBandCnf(
                                       DPARAM              param
                                       );

/*****************************************************************************/
// 	Description : get the network name
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetNetWorkNameId(
                                     MN_DUAL_SYS_E dual_sys,
                                     MN_PLMN_T *plmn_ptr,
                                     MN_PHONE_PLMN_STATUS_E  plmn_status,
                                     BOOLEAN                 is_slecting
                                     );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_GET_PIN_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPinInd(
                                   DPARAM              param
                                   );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_PIN_FUNC_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPinCnf(
                                   DPARAM              param
                                   );

#ifdef MMI_4G_SUPPORT
/*****************************************************************************/
//  Description : deal with signal of APP_MN_SIM_APPLN_INIT_CNF
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleApplnInitCnf(DPARAM param);
#endif

LOCAL void PHONE_InitPsWhenAllSimReady(MN_DUAL_SYS_E readySimId, MMIPHONE_SIM_READY_TO_PS_REASON_E eReason);
/*****************************************************************************/
// Description : 
//  User can check the netwotk's mcc/mnc, and sim's mcc/mnc.
//  If they are not equal, it is roaming.
//  But maybe the local operator regard this as not-roaming.
//  Then the user can config in this function to resolve this case.
// Global resource dependence : 
// Author: Wuding.Yang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsRealRoaming(MN_DUAL_SYS_E dual_sys, MN_PLMN_T cur_plmn);

/*****************************************************************************/
// 	Description : 保存网络状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreNetworkStatus(
                               APP_MN_NETWORK_STATUS_IND_T *status_ptr
                               );

/*****************************************************************************/
// 	Description : 在提示还可以 输入多少次密码后，启动pin输入框
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePin1Maxtimes(
                                      MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM 		        param		// 相应消息的参数
                                      );

/*****************************************************************************/
// 	Description : 在pin blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinBlocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    );

/*****************************************************************************/
// 	Description : 在pin1或者pin2 blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinBlockedExceptPhone(
                                               MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                               MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                               DPARAM 		        param		// 相应消息的参数
                                               );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_READY_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimReadyInd(
                                     DPARAM              param
                                     );

/*****************************************************************************/
// 	Description : 保存 SIM 状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreSimReadyStatus(
                               APP_MN_SIM_READY_IND_T  *sim_status_ptr
                               );

/*****************************************************************************/
// 	Description : 保存 SIM  not ready 状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreSimNotReadyStatus(
                                  APP_MN_SIM_NOT_READY_IND_T  sim_status_ptr
                                  );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_NOT_READY_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimNotReadyInd(
                                        DPARAM              param
                                        );

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SCELL_RSSI_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScellRssiInd(
                                      DPARAM              param
                                      );

#if defined(MMI_SMSCB_SUPPORT)
/*****************************************************************************/
// 	Description : startup sms cb
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartupSmsCB(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : ClearCbEnable
//	Global resource dependence : 
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
LOCAL void ClearCbEnable(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
// 	Description : get opn display string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:  
/*****************************************************************************/
LOCAL void GetOpnString(MN_DUAL_SYS_E dual_sys,//IN:
                                            MMI_STRING_T *opn_str_ptr,//IN/OUT
                                            uint16 max_opn_len
                                            );

/*****************************************************************************/
// 	Description : get spn display string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:  
/*****************************************************************************/
LOCAL void GetSpnString(MN_DUAL_SYS_E dual_sys,//IN:
                                            MMI_STRING_T *spn_str_ptr,//IN/OUT
                                            uint16 max_spn_len
                                            );

/*****************************************************************************/
// 	Description : compare spn and opn
//	Global resource dependence : 
//  Author: kun.yu
//	Note:  
/*****************************************************************************/
LOCAL BOOLEAN MMIPHONE_IsSPNEqualtoOPN(MMI_STRING_T *opn_str_ptr,//IN:
                                                                                    MMI_STRING_T *spn_str_ptr//IN:
                                                                                    );

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: wancan.you
//    Note: 
/*****************************************************************************/
//LOCAL void MMIPHONE_SetDualSysSetting(BOOLEAN is_need_start_ps);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPresentInd(
                                     DPARAM param
                                     );

/*****************************************************************************/
// 	Description : PowerOnPsForEmerge
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL void PowerOnPsForEmerge(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : the process message function of the virtual window
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMI_RESULT_E  IdleApplet_HandleEvent(    
                                     IAPPLET_T*          iapplet_ptr,
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );

/*****************************************************************************/
//  Description : Notify Rx level Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void NotifyRxLevelChange(MN_DUAL_SYS_E dual_sys, uint8 rx_level);

/*****************************************************************************/
//  Description : Notify PLMN Status Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void NotifyPlmnStatusChange(MN_DUAL_SYS_E dual_sys, MN_PHONE_PLMN_STATUS_E plmn_status);

/*****************************************************************************/
//  Description : Notify Service Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifyServiceChange(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event);

//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmiphone_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_IDLE_APPLET_ID), sizeof(CAF_APPLET_T), IdleApplet_HandleEvent, IMAGE_MAINMENU_ICON_SET, TXT_IDLE },
};

//applet的信息
LOCAL const CAF_STATIC_APPLET_INFO_T s_mmiphone_applet_info = 
{
    (CAF_STATIC_APPLET_T *)s_mmiphone_applet_array,
    ARR_SIZE(s_mmiphone_applet_array)
};

#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
/*****************************************************************************/
// Description : 轮询是否有SIM热插拔的callback
// Global resource dependence :
// Author:xiaotong.su
// Note:
/*****************************************************************************/
LOCAL void QuerySimisPlugInCallback(void);

/*****************************************************************************/
//  Description : send msg of logic sim plug in(out)
//  Global resource dependence :
//  Author:xiaotong.su
//  Note:
/*****************************************************************************/
LOCAL void SendLogicSimPlugMsg(uint16 signal_code);
#endif

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPlugInInd(
                                     DPARAM param
                                     );
#endif

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLdnReadyInd(
                                     DPARAM param
                                     );

/*****************************************************************************/
// 	Description : ldn update confirm
//	Global resource dependence : none
//	Author:dave.ruan
//	Note:delete all/datlete both type is erase
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLDNUpdateCnf( DPARAM param);
#endif

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : Handle SIM Plug Alert Win Msg
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMPlugAlertWinMsg(
                                                                                              MMI_WIN_ID_T win_id,
                                                                                              MMI_MESSAGE_ID_E 	msg_id,
                                                                                              DPARAM param
                                                                                              );

/*****************************************************************************/
//  Description : Notify SIM Plug Ind
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifySIMPlugInd(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event);
#endif

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void OpenSetStandByModeWaiting(void);
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void ImplementStandByModeAsync(void);

/*****************************************************************************/
//  Description : HandleSetStandbyMdoeWaitingWindow
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStandbyMdoeWaitingWindow(
                                         MMI_WIN_ID_T    win_id, // 窗口的ID
                                         MMI_MESSAGE_ID_E   msg_id, // 窗口的内部消息ID
                                         DPARAM             param   // 相应消息的参数
                                         );
/*****************************************************************************/
// 	Description : HandleSetStandByModeQueryWinMsg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStandByModeQueryWinMsg( 
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM              param
                                           );
#endif

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : 在SIM Lock 解锁次数达到限制的情况下弹出"SIM Restricted"窗口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    );
#endif
/*****************************************************************************/
//  Description : Get sim card which need emergency ps
//  Global resource dependence :
//  Author:yanli.yang
//  Note:  return MN_DUAL_SYS_MAX--no sim card need emergency ps
//                MN_DUAL_SYS_1--sim card 0 need emergency ps
//                MN_DUAL_SYS_2--sim card 1 need emergency ps
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E PHONE_GetSimNeedEmergencyPs(void);
/*****************************************************************************/
//   Description : Is All Sim Ready
//	Global resource dependence :
//   Author: mingwei.jia
//   Return value:
//        FALSE -- No available SIM for data service
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPIPHONE_IsAllSimReady(void);

/*****************************************************************************/
//  Description : Is China Telecom
//  Global resource dependence :
//  Author: mingwei.jia
//  Return value:
//      FALSE -- No available SIM for data service
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPIPHONE_IsChinaTelecom(MN_DUAL_SYS_E dual_sys);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register common module applet info
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_CommonMduRegAppletInfo(void)
{
    MMI_RegAppletInfo( MMI_MODULE_COMMON, &s_mmiphone_applet_info );
}

/*****************************************************************************/
//  Description : the process message function of the virtual window
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMI_RESULT_E  IdleApplet_HandleEvent(    
                                     IAPPLET_T*          iapplet_ptr,
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     )
{

    MMI_RESULT_E    result = MMI_RESULT_FALSE;
	//MMIFILE_DEVICE_E device_type = MMI_DEVICE_SDCARD;    
	
#ifdef HERO_ENGINE_SUPPORT
	result = MMIHEROAPP_handleMrMsg(msg_id, param);
#endif 
#ifdef MRAPP_SUPPORT
	result = MMIMRAPP_handleMrMsg(msg_id, param);
#endif 
    //SCI_TRACE_LOW:"VirWin_HandleMsg THE msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2039_112_2_18_2_45_3_0,(uint8*)"d",msg_id);
    switch (msg_id)
    {
#if defined MMI_AUTOTEST_SUPPORT && defined ENG_SUPPORT
    case MSG_MMI_AUTO_TEST_MODULE:
        MMI_HandleAutoTestModule( (const char*)param );
        break;
#endif
        
    case MSG_KEYDOWN_SDCARD_DETECT:
    case MSG_KEYUP_SDCARD_DETECT:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive SD 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2044_112_2_18_2_45_3_1,(uint8*)"d",msg_id);
		MMIAPISD_HandleSDPlug((void *)param, msg_id);
        //MMIAPISD_HandleSDPlug(msg_id);
        break;

#ifdef DPHONE_SUPPORT 
#else       
    case MSG_HANDLE_SD_ERR:	
		MMIAPISD_HandleSDPlug((void *)param, msg_id);	
        break;   
#endif   

    case MSG_TIMER:
#ifdef OD_GAME_SUPPORT
	if(od_game_timer_handler(param))
	{
		SCI_TRACE_LOW("VirWin_HandleMsg: received OD_GAME timer\n");
	}
#endif

	break;
#ifdef JAVA_SUPPORT_MYRIAD
    case MSG_JAVA_MMI_EVENT:
        MMIAPIJAVA_HandleJbedMMIEvent(param);
        break;
    case MSG_JAVA_OTA_INSTALL_START:
        {
            SCI_TRACE_LOW("IdleApplet_HandleEvent:receive MSG_JAVA_OTA_INSTALL_START!");
#ifdef BROWSER_SUPPORT
            if(MMIBROWSER_SUCC == MMIAPIBROWSER_Close())
            {
                if(PNULL != param)
                {
                    uint16* path_name = (uint16*)param;

                    MMIAPIJAVA_Install(path_name,MMIAPICOM_Wstrlen(path_name));
                }
            }
#endif
        }
        break;
#endif

    case MSG_APP_RED:
        result = MMI_RESULT_TRUE;
        break;
#ifdef MOTION_SENSOR_TYPE_OPTICAL
    case MSG_GSENSOR:      
        SCI_TRACE_LOW("receive MSG_OPSENSOR");
        result = MMIDEFAULT_HandleOpsensorMsg(param);
        break;
#endif
    case MSG_FILE_READFILE_ASYN_CALLBACK:
    case MSG_FILE_WRITEFILE_ASYN_CALLBACK:
    case MSG_FILE_READFILE_BYOFFSET_ASYN_CALLBACK:
    case MSG_FILE_WRITEFILE_BYOFFSET_ASYN_CALLBACK:
        MMIAPIFMM_OperationAsynCnf((void*)param, msg_id);
        break;
        
//    case MSG_FMM_COPY_FILE_READ_CNF:
//        MMIFMM_FileCopyWriteDest(*(SFS_ERROR_E *)param);
//        break;
//        
//    case MSG_FMM_COPY_FILE_WRITE_CNF:
//        MMIFMM_FileCopyReadSrc(*(SFS_ERROR_E *)param);
//         break;
        
    case MSG_FILE_COPY_READ_CNF:
#ifdef MMI_FILEMGR_COPY //for 217 ram cutdown
        MMIAPIFMM_CopyWriteDest(*(SFS_ERROR_E *)param);
#endif
        break;
        
    case MSG_FILE_COPY_WRITE_CNF:
#ifdef MMI_FILEMGR_COPY //for 217 ram cutdown
        MMIAPIFMM_CopyReadSrc(*(SFS_ERROR_E *)param);
#endif
        break;
        
    case MSG_ALM_ALERTING_END:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_ALM_ALERTING_END"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2090_112_2_18_2_45_3_2,(uint8*)"");
        if (!MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID))
        {
            if(MMK_IsOpenWin(PHONE_STARTUP_ALARM_WIN_ID))
            {
                MMK_CloseWin(PHONE_STARTUP_ALARM_WIN_ID);
            }
            MMK_CreateWin((uint32*)PHONE_STARTUP_ALARM_WIN_TAB,PNULL);
        }
        result = MMI_RESULT_TRUE;
        break;

#ifdef MMI_AUDIO_PLAYER_SUPPORT
    case MSG_MP3_PLAY_CALL_BACK:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_MP3_PLAY_CALL_BACK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2103_112_2_18_2_45_3_3,(uint8*)"");
        //Mp3正常播放结束的回调函数。
        MMIAPIMP3_PlayMp3CallBack();
        break;

    case MSG_MP3_FATAL_ERROR:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_MP3_FATAL_ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2109_112_2_18_2_45_3_4,(uint8*)"");
        MMIAPIMP3_HandleFatalError();
        break;
#ifdef  MMIAP_LYRIC_SUPPORT    
    case MSG_MP3_START_LIRIC:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received [MMIMP3] MSG_MP3_START_LIRIC"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2114_112_2_18_2_45_3_5,(uint8*)"");
        MMIAPIMP3_StartLyric();
        break;
#endif
    case MSG_MP3_FOLDER_FULL:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received [MMIMP3] MSG_MP3_FOLDER_FULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2119_112_2_18_2_45_3_6,(uint8*)"");
        {
			if (PNULL != param)
			{
                MMIFILE_DEVICE_E dev = (MMIFILE_DEVICE_E)param;/*lint !e549*/
                MMIAPIMP3_FolderFull(dev);
			}
        }
        
        break;
#endif
#ifdef KURO_SUPPORT
    case MSG_KUR_PLAY_CALL_BACK:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_KUR_PLAY_CALL_BACK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2124_112_2_18_2_45_3_7,(uint8*)"");
        MMIAPIKUR_PlayKurCallBack();
        break;

    case MSG_KUR_FATAL_ERROR:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_KUR_FATAL_ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2129_112_2_18_2_45_3_8,(uint8*)"");
        MMIAPIKUR_HandleFatalError();
        break;
#endif

    case MSG_MPEG4_DELETE_ONE:
        //SCI_TRACE_LOW:"VirWin_HandleMsg: received MSG_MPEG4_DELETE_ONE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2134_112_2_18_2_45_3_9,(uint8*)"");
        break;
        
   case MSG_MMS_ACTIVE_RECV_SEND:
       //SCI_TRACE_LOW:"VirWin_HandleMsg: MMIAPIMMS_ActiveRecvSendModule"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2138_112_2_18_2_45_3_10,(uint8*)"");
       //启动彩信发送和接收，通过异步实现
       MMIAPIMMS_ActiveRecvSendModule();
       break;
        
    case MSG_MMIDV_DEL_UNSAVED_MPEG4:
        MMITHEME_SetUpdateDelayCount(2);
        break;
        
    case MSG_MMIDV_CLOSE_RECORD_WIN:
        MMIAPIDC_Exit();
        break;
        
//    case MSG_CMSBRW_FS_READ:	//wanqiangzh modify for STK
//    case MSG_CMSBRW_FS_WRITE:	//wanqiangzh modify for STK
//        MMICMSBRW_HandleWriteAndRead(msg_id, param);
//        break;
//    case MSG_CMSBRW_STOP_BGSOUND:
//        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
//        break;

    case MSG_RECORD_END:
        MMIAPIDC_Exit();
        break;
//    case MSG_CMSBRW_BRW_OP:
//        SCI_TRACE_LOW("VirWin_HandleMsg:MSG_CMSBRW_BRW_OP");
//        {			
//            if(param!=PNULL)
//            {
//                MMIBRW_VIRWIN_MESSAGE_T      *message=NULL;
//                message = (MMIBRW_VIRWIN_MESSAGE_T *)param;
//                MMIBRW_HandleBrwOpMsg(READ_FILE_RESP,&message->param);	
//                //SCI_Free(message->param);
//                //SCI_Free(message);
//            }
//            else
//            {
//                SCI_TRACE_LOW("warning:param null");
//            }
//        }
//        
//        break;
#ifdef  CAMERA_SUPPORT        
    case MSG_DV_OPEN_MSG:
        MMIAPIDC_OpenVideoWin();
        break;
        
    case MSG_DC_OPEN_MSG:
        MMIAPIDC_OpenPhotoWin();
        break;
#endif
        //msg for MobileTV
    case MSG_MBBMS_PRE_EXIT_MSG:
#ifdef VT_SUPPORT
        if(!MMIAPIVT_IsVtCalling())
        {
            MMIAPIMTV_PreExitCallback();
        }
#else
        MMIAPIMTV_PreExitCallback();
#endif
        break;
#ifdef MMI_VCALENDAR_SUPPORT
    case MSG_VCALENDAR_WRITE_SMS_CNF:
    case MSG_VCALENDAR_SAVE_SCH_CNF:
        MMIVCAL_HandleWriteCNF(msg_id, param);
        break;
#endif
#ifdef 	JAVA_SUPPORT_IA		
#if defined JAVA_OTA_SUPPORT && defined BROWSER_SUPPORT
    case MSG_JAVA_OTA_DOWNLOAD_START:
        MMIAPIJAVA_OpenWapWin();        
        break;
#endif
#endif

    case MSG_SET_OPENALARM_EXPIRED_WIN:
        MMIAPIALM_OpenAlarmExpiredWin();
        break;   
        
    case MSG_SLEEP_LCD:
        MMIDEFAULT_TurnOffBackLight();
        break;

    case MSG_AUDIO_CALLACK:
        MMISRVAUD_HandleCallBackMsg((void *)param);
        break;

    case MSG_SET_WALLPAPER_WAIT:
#if defined MMI_WALLPAPER_SUPPORT
        MMIAPIFMM_HandleWallpaperWaitMsg();
#endif
        break;
        
#ifdef BROWSER_SUPPORT_DORADO
    case BRW_SNAPSHOT_FFS_PAGE_SAVE_CNF:			
    case BRW_SNAPSHOT_FFS_CSS_SAVE_CNF:		
    case BRW_SNAPSHOT_FFS_IMAGE_SAVE_ONE_CNF:	
    case BRW_SNAPSHOT_FFS_IMAGE_SAVE_CNF:
    case BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF:
    case BRW_SNAPSHOT_FFS_IMAGE_OPEN_ONE_CNF:
	BRW_SnapshotHandleFfsMsg(msg_id,*((SFS_ERROR_E*)param));
	break;
    
    case MSG_BRW_MSG_INIT_CNF:
    case MSG_BRW_MSG_EXIT_CNF:
    case MSG_BRW_MSG_TYPE_ENTER_FILENM_REQ:
    case MSG_BRW_MSG_TYPE_DL_CONFIRM_REQ:     
    case MSG_BRW_MSG_TYPE_AUTH_REQ:           
    case MSG_BRW_MSG_TYPE_CHANGE_URL_IND:     
    case MSG_BRW_MSG_TYPE_NEW_PAGE_REQ_IND:   
    case MSG_BRW_MSG_TYPE_UPDATE_CONTENT_IND: 
    case MSG_BRW_MSG_TYPE_PROGRESS_IND:       
    case MSG_BRW_MSG_TYPE_ERROR_IND:            
    case MSG_BRW_MSG_TYPE_DOWNLOAD_OK_IND:     
    case MSG_BRW_MSG_TYPE_DOWNLOAD_ERR_IND:   
    case MSG_BRW_MSG_TYPE_PAGE_GET_IND:
    case MSG_BRW_MSG_TYPE_MEDIA_GET_ONE_IND:
    case MSG_BRW_MSG_TYPE_DISPLAY_READY_IND:  
    case MSG_BRW_MSG_TYPE_SSL_CERT_IND:       
    case MSG_BRW_MSG_TYPE_REPAINT_READY_IND:
    case MSG_BRW_MSG_TYPE_PAGE_STOP_IND:
    case MSG_BRW_MSG_TYPE_OMA_DL_CONFIRM_REQ:        
    case MSG_BRW_MSG_TYPE_LAYOUTCTRL_DESTROY_REQ:
//    case MSG_BRW_MSG_TYPE_PDP_ACTIVE_CNF:
    case MSG_BRW_STOP_BGSOUND:
    case MSG_BRW_MSG_TYPE_HTTP_REFRESH_IND:        
        MMIAPIBROWSER_HandleAppMsg(msg_id,param);
        break;
        
#endif

#ifdef BROWSER_SUPPORT
    case MSG_BROWSER_MANAGER_MAKECALL_REQ:
    case MSG_BROWSER_MANAGER_SENDDTMF_REQ:
    case MSG_BROWSER_MANAGER_ADD_PBITEM_REQ:
    case MSG_BROWSER_MANAGER_OPEN_RTSP_IND:
    case MSG_BROWSER_MANAGER_SENDSMS_REQ:
    case MSG_BROWSER_MANAGER_SENDSMSTO_REQ:
    case MSG_BROWSER_MANAGER_SENDMMS_REQ:
    case MSG_BROWSER_MANAGER_SENDMMSTO_REQ:
    case MSG_BROWSER_MANAGER_SENDMAIL_REQ:
    case MSG_BROWSER_MANAGER_SENDMAILTO_REQ:
    case MSG_BROWSER_MANAGER_TEL_REQ:
        MMIAPIBROWSER_HandleAppMsg(msg_id,param);
        break;
#endif

    case MSG_APPLET_CLEAR_FREE_MODULE:
        MMK_PostMsg( MMIAPPLET_MANAGER_LIST_WIN_ID, MSG_APPLET_MANAGER_UPDATE, PNULL, 0 );
        break;

    case MSG_APPLET_SWITCH:
        MMK_ReturnIdleWin();
        break;
        
#if defined(OPERA_MOBILE_SUPPORT_V11) || defined(OPERA_MINI_SUPPORT_VER42) || defined(OPERA_MINI_SUPPORT_VER6)
    case MSG_MMI_OPERA_ASYNC_EXIT:
         {
            MMIBROWSER_ENTRY_PARAM *ptr = (MMIBROWSER_ENTRY_PARAM *)param;

#ifdef BROWSER_SUPPORT_DORADO
#ifndef WIN32
            //opera exit
            MMIAPIBROWSER_Close();//  OperaMini_Exit()
#endif
            //dorado entry
            if (ptr != 0)
            {
                MMIAPIBROWSER_Entry(ptr);
                if (ptr->url_ptr != 0)
                {
                    SCI_FREE(ptr->url_ptr);
                }
            }
        }
#else
            //only free
            if (ptr != 0)
            {
                if (ptr->url_ptr != 0)
                {
                    SCI_FREE(ptr->url_ptr);
                }
            }
        }
#endif
        break;
#endif        
        
#ifdef TTS_SUPPORT
#ifndef WIN32
    case MSG_TTS_PLAY:
        //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_PLAY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2302_112_2_18_2_45_4_11,(uint8*)"");
        MMIAPITTS_SynthTextByTask();
        break;
    case MSG_TTS_STOP:
        //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_STOP"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2306_112_2_18_2_45_4_12,(uint8*)"");
        MMIAPITTS_StopSynthTextByTask();
        break;
    case MSG_TTS_OVER:
#ifdef TTS_SUPPORT_EJ	
        //SCI_TRACE_LOW:"virwin_MSG_TTS_OVER"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2311_112_2_18_2_45_4_13,(uint8*)"");
        MMK_PostMsg( MMIKL_CLOCK_DISP_WIN_ID == MMK_GetFocusWinId() ? MMK_GetPrevWinId(MMIKL_CLOCK_DISP_WIN_ID) : MMK_GetFocusWinId(), MSG_TTS_OVER, PNULL, 0 );
#endif
        break;
    case MSG_TTS_AUDIO_PLAY:
        {
            uint32 pcm_stream_id = *(uint32*)param;
            //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_PLAY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2318_112_2_18_2_45_4_14,(uint8*)"");
            MMIAPITTS_PlayPcmStream(pcm_stream_id);
        }
        break;

    case MSG_TTS_ERROR_RESOURCE:
#ifdef EBOOK_SUPPORT
        if(MMK_IsOpenWin(MMIEBOOK_TXT_SHOW_WIN_ID))
        {
            MMIAPIEBOOK_SetTTSOnOFF(FALSE);
        }
#endif
#if 0        
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_TTS_NO_RESOURCE, 
            TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        break;
#endif
#endif

#ifdef _SW_ZDT_PRODUCT_
	case MSG_USRE_LOG_SAVE:
	{
		  Trace_Log_Save();
	}
	break;
    
	case MSG_USRE_LOG_AT_SAVE:
	{
		Trace_AT_Save();
	}
	break;
    
	case MSG_USRE_LOG_SOCKET_SAVE:
	{
		Trace_Socket_Save();
	}
	break;
    
	case MSG_USRE_LOG_BUF_SAVE:
	{
		Trace_BUF_Save();
	}
	break;
    
	case MSG_USRE_LOG_EXT_SAVE:
	break;
    
	case MSG_USRE_LOG_DEL_ALL:
	{
		UserFile_RemoveDirFile("TEST");
	}
	break;
#endif

#ifdef HERO_ENGINE_TTS_SUPPORT
#ifndef WIN32
    case MSG_TTS_PLAY:
        //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_PLAY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2302_112_2_18_2_45_4_11,(uint8*)"");
        MMIAPITTS_SynthTextByTask();
        break;
    case MSG_TTS_STOP:
        //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_STOP"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2306_112_2_18_2_45_4_12,(uint8*)"");
        MMIAPITTS_StopSynthTextByTask();
        break;
    case MSG_TTS_OVER:
#ifdef TTS_SUPPORT_EJ	
        //SCI_TRACE_LOW:"virwin_MSG_TTS_OVER"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2311_112_2_18_2_45_4_13,(uint8*)"");
        MMK_PostMsg( MMIKL_CLOCK_DISP_WIN_ID == MMK_GetFocusWinId() ? MMK_GetPrevWinId(MMIKL_CLOCK_DISP_WIN_ID) : MMK_GetFocusWinId(), MSG_TTS_OVER, PNULL, 0 );
#endif
        break;
    case MSG_TTS_AUDIO_PLAY:
        {
            uint32 pcm_stream_id = *(uint32*)param;
            //SCI_TRACE_LOW:"virwin_MSG_TTS_AUDIO_PLAY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2318_112_2_18_2_45_4_14,(uint8*)"");
            MMIAPITTS_PlayPcmStream(pcm_stream_id);
        }
        break;

    case MSG_TTS_ERROR_RESOURCE:
        break;
#endif
#endif


	default:
        break;
    }
    
    return (result);
}
PUBLIC char MMIAPIPHONE_4g_icon_display_style(void)
{
    return  s_4g_icon_display_style;
}

PUBLIC char MMIAPIPHONE_prefer_network_display_style(void)
{
	return s_prefer_network_menu_display_style;
}

PUBLIC BOOLEAN MMIAPIPHONE_IsIndiaSOSSupport(void)
{
    return s_is_indiasos_supported;
}

/*****************************************************************************/
//  Description : thermal power off note win
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: chaonan.hao
//  Note:
/*****************************************************************************/
LOCAL void PHONE_Thermal_Win_Enter(void)
{
    MMI_STRING_T            operating_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text  = {NULL, NULL, NULL};

    SCI_TRACE_LOW("enter WatchPHONE_Thermal_NoteWin");
    MMI_GetLabelTextByLang(TXT_THERMAL_POWER_OFF, &operating_str);
    WatchCOM_WaitingWin_1Line_Enter(MMIPHONE_THERMAL_NOTE_WIN_ID,&operating_str,0,softkey_text,PNULL);
}

/*****************************************************************************/
//  Description : 关闭提示窗并关机
//  Parameter: [In] timer_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: chaonan.hao
//  Note:
/*****************************************************************************/
LOCAL void PHONE_ThermalPowerOff(uint8 timer_id, uint32 param)
{
    if(0 != s_thermal_power_off_timer)
    {
        SCI_TRACE_LOW("Thermal:phone will power off");
        if(MMK_IsOpenWin( MMIPHONE_THERMAL_NOTE_WIN_ID ))
        {
            MMK_CloseWin(MMIPHONE_THERMAL_NOTE_WIN_ID);
        }
        s_is_power_off = FALSE;
        MMK_StopTimer(s_thermal_power_off_timer);
        s_thermal_power_off_timer = 0;
    }
    MMIPHONE_PowerOff();
}

/*****************************************************************************/
//  Description: phone监听ual device模块消息
//  Parameter: [In] msg //ual device上来的消息
//             [Out] None
//             [Return] none
//  Author: chaonan.hao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PHONE_HandleDeviceMsg(ual_cms_msg_t msg)
{
    SCI_TRACE_LOW("msg.msg_id = %d.", msg.msg_id);
    switch (msg.msg_id)
    {
        case MSG_UAL_DEVICE_THERMAL_POWER_OFF:
        {
            if(FALSE == s_is_power_off)
            {
                s_is_power_off = TRUE;
                PHONE_Thermal_Win_Enter();
                SCI_TRACE_LOW( "Thermal:receive MSG_UAL_DEVICE_THERMAL_POWER_OFF in phone");
                s_thermal_power_off_timer = MMK_CreateTimerCallback(MMI_3SECONDS, PHONE_ThermalPowerOff, NULL, FALSE);
            }
            break;
        }
        default:
            break;
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : init the phone application
//	Global resource dependence : g_service_status,g_phone_app
//  Author: Jassmine
//	Note:
/*****************************************************************************/
void MMIAPIPHONE_AppInit(void)
{
    //uint16   nv_len = 0;
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;

    // 4G icon display style
    //nv_len = NVITEM_GetLength(OTHER_NV_4G_ICON_DISPLAY_STYLE);
    //if(NVERR_NONE != EFS_NvitemRead(OTHER_NV_4G_ICON_DISPLAY_STYLE, nv_len, (uint8*)&s_4g_icon_display_style))
    {
        s_4g_icon_display_style = 0;
    }

    //Prefer network display style
    //nv_len = NVITEM_GetLength(OTHER_NV_NETWORK_MENU_DISPLAY_STYLE);
    //if(NVERR_NONE !=EFS_NvitemRead(OTHER_NV_NETWORK_MENU_DISPLAY_STYLE, nv_len, (uint8*)&s_prefer_network_menu_display_style))
    {
        s_prefer_network_menu_display_style = 0;
    }   
    
    // 初始化记录服务状态的结构，主要用于IDLE界面显示
    SCI_MEMSET(&s_sim_status[0], 0, sizeof(s_sim_status));
    SCI_MEMSET(&g_service_status[0], 0, sizeof(g_service_status));
    SCI_MEMSET(&g_pin_value[0], 0, sizeof(g_pin_value)); 
    
    //Udisk没有在运行
    MMIAPIUdisk_SetRunStatus(FALSE);
    
    g_mmiphone_app.ProcessMsg = HandlePsAndRefMsg;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        s_sim_status[i].simStatus = SIM_NOT_INITED;

        g_service_status[i].is_first_rssi    = TRUE;
        g_service_status[i].is_slecting = TRUE; 
        
        s_is_ps_ready[i]= FALSE;
        s_ps_ready[i] = MMIPHONE_PS_POWER_ON_NONE;

        s_is_ps_deactive_cnf[i] = TRUE;

        s_sim_existed_status[i] = TRUE;
    } 
#ifdef DPHONE_SUPPORT
	MMIDEFAULT_InitHookAndHF();
#endif

#if defined(MMI_UPDATE_TIME_SUPPORT)
    MMIAPIAUT_AutoUpdateTimeInit();
#endif/*MMI_UPDATE_TIME_SUPPORT*/

#ifdef DRM_SUPPORT
#ifdef BROWSER_SUPPORT    
    {
	MMIBROWSER_MIME_DOWNLOAD_PARAM_T drm_callback = {0};

	drm_callback.call_back = MMIDRM_HandleDLDrmCB;
	
	drm_callback.mime_type = MMIBROWSER_MIME_TYPE_DM;
	MMIAPIBROWSER_RegMimeFunc(&drm_callback);
	
	drm_callback.mime_type = MMIBROWSER_MIME_TYPE_DCF;
	MMIAPIBROWSER_RegMimeFunc(&drm_callback);
    }
#endif
#endif
    MMIPHONE_InitNITZ();
    MMIPHONE_InitNetworkNameInfo();
#ifdef MMI_RESET_PHONE_AUTO_PIN
    {
        MN_BLOCK_T sim_pin_value[MMI_DUAL_SYS_MAX] = {0};
        MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
        //get imsi from nv
        MMINV_READ(MMINV_SIM_PIN_VALUE,sim_pin_value,return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(sim_pin_value, 0, sizeof(MN_BLOCK_T) * MMI_DUAL_SYS_MAX);
            MMINV_WRITE(MMINV_SIM_PIN_VALUE,sim_pin_value);
        }
        SCI_MEMCPY(&g_pin_value[0], &sim_pin_value, sizeof(g_pin_value)); 
    }
#endif
    //register ual device
    ual_device_register(PHONE_HandleDeviceMsg, &s_device_register_handle);
}

/*****************************************************************************/
// 	Description : 处理PHONE APP的PS消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePsAndRefMsg(
                                     PWND                app_ptr, 
                                     uint16              msg_id, 
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E   e_dual_sys = MN_DUAL_SYS_1;

    switch (msg_id)
    {
#ifdef MMI_4G_SUPPORT
    case APP_MN_SIM_APPLN_INIT_CNF:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_SIM_APPLN_INIT_CNF");
        HandleApplnInitCnf(param);
        break;
#endif

    case APP_MN_NETWORK_STATUS_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_NETWORK_STATUS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2430_112_2_18_2_45_4_15,(uint8*)"");
        // 该消息用于在网络服务状态改变时，汇报当前网络服务状态，是紧急呼叫状态，
        // 还是无服务状态，还是GSM服务状态，还是GSM、GPRS状态。
        // 获得网络的状态需在idle界面显示。
        result = HandleNetworkStatusInd(param);
        break;

    case APP_MN_NETWORK_INFO_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_NETWORK_INFO_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2438_112_2_18_2_45_4_16,(uint8*)"");
        // 该消息用于在网络信息改变时，上报当前网络PLMN
        // 网络名称,包括full name/short name,其中每个结构都有状态、字符类型、长度等
        // 还有当前的网络时间LSA等信息
        result = MMIPHONE_HandleNetworkInfoInd(param);
        break;
        
    case APP_MN_PREFER_RAT_RSP:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PREFER_RAT_RSP"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2446_112_2_18_2_45_4_17,(uint8*)"");
        result  = HandlePreferRatRsp(param);
        break;
#ifdef MODEM_SUPPORT_SPRD
    case APP_MN_TEST_MODE_CNF:
        // 该消息用于切换network type后，设置switch rat state
        SCI_TRACE_LOW("HandlePsAndRefMsg: receive APP_MN_TEST_MODE_CNF.");
        MMISET_SetSwitchRatState(FALSE);
        break;
#endif
    case APP_MN_PLMN_LIST_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PLMN_LIST_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2451_112_2_18_2_45_4_18,(uint8*)"");
        //该消息用于通知用户MNPHONE_ListPLMN()的操作结果。
        // 结构中包含存在网络的列表和每个网络的具体信息。
        result = HandlePlmnListCnf(param);
        break;
        
    case APP_MN_PLMN_SELECT_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PLMN_SELECT_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2458_112_2_18_2_45_4_19,(uint8*)"");
        // 该消息用于通知用户MNPHONE_SelectPLMN()的操作结果。
        // 结构中包含选择操作完成后当前的网络状况。
        result = HandleSelectPlmnCnf(param);
        break;
        
    case APP_MN_DISPLAY_PLMN_IND:
        e_dual_sys = ((APP_MN_DISPLAY_PLMN_IND_T *)param)->dual_sys;
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_DISPLAY_PLMN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2466_112_2_18_2_45_4_20,(uint8*)"");
        //该消息用于通知用户进行手动选网	
        //if (!MMK_IsFocusWin(PHONE_POWER_OFF_WIN_ID))
        //{
        //    result = MMIAPISET_HandleDisplayPlmnInd(e_dual_sys);
        //}
        break;
        
    case APP_MN_SIM_PRESENT_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_PRESENT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2475_112_2_18_2_45_4_21,(uint8*)"");
        result = HandleSimPresentInd(param);
        break;

#ifdef ZDT_NFC_SUPPORT
    case ZDT_NFC_SIGNAL_INIT_OVER:
        {
            extern uint8 g_nfc_card_simulation_init_error;
            SCI_TRACE_LOW("ZDT_NFC_SIGNAL_INIT_OVER %d Err=%d",s_nfc_power_on_sim_init,g_nfc_card_simulation_init_error);
            if(g_nfc_card_simulation_init_error)
            {
                ZDTNFC_TaskClose();
                #ifndef WIN32
                GPIO_NFCIRQCtrl(FALSE);
                #endif
            }
            MNPHONE_PowerOnSimEx(MN_DUAL_SYS_1, s_nfc_power_on_sim_init);
        }
        break;
#endif

#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
    case MSG_PHONE_SIM_LOGIC_PLUG_OUT_IND:
#ifdef MCARE_V31_SUPPORT
        McareV31_ExitMcare();
#endif
        MMIPHONE_SetSimStatus(e_dual_sys, SIM_STATUS_NO_SIM);
        MMIAPIPHONE_SetSimExistedStatus(e_dual_sys, FALSE);
        SCI_TRACE_LOW("[Hot_Plugin]: MSG_PHONE_SIM_LOGIC_PLUG_OUT_IND");
        NotifySIMPlugInd(e_dual_sys, MMIPHONE_NOTIFY_SIM_PLUG_OUT);
        break;
    case MSG_PHONE_SIM_LOGIC_PLUG_IN_IND:
        {
            APP_MN_SIM_PLUG_IN_IND_T sim_plug_in_ind = *(APP_MN_SIM_PLUG_IN_IND_T *)param;
            SCI_TRACE_LOW("[Hot_Plugin]: MSG_PHONE_SIM_LOGIC_PLUG_IN_IND");
#ifdef SFR_SUPPORT_CMCC
            MMISFR_CMCC_SetAllowActivePdp(TRUE);
#endif
            result = NotifySIMPlugInd(sim_plug_in_ind.dual_sys, MMIPHONE_NOTIFY_SIM_PLUG_IN); //弹提示框加注网
        }
        break;
#endif
#ifdef SIM_PLUG_IN_SUPPORT_HARDWARE
    case APP_MN_SIM_PLUG_IN_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_PLUG_IN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2480_112_2_18_2_45_4_22,(uint8*)"");
#ifdef SFR_SUPPORT_CMCC
        MMISFR_CMCC_SetAllowActivePdp(TRUE);
#endif
        result = HandleSimPlugInInd(param);
        break;
#endif

#ifdef MMI_LDN_SUPPORT
    case APP_MN_LND_READY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_LND_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2486_112_2_18_2_45_4_23,(uint8*)"");
        result = HandleLdnReadyInd(param);
        break;
    case APP_MN_LND_UPDATE_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_LND_UPDATE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2490_112_2_18_2_45_4_24,(uint8*)"");
        result = HandleLDNUpdateCnf(param);
        break;
#endif
        
    case APP_MN_SET_BAND_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SET_BAND_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2496_112_2_18_2_45_4_25,(uint8*)"");
        // 该消息用于通知用户MNPHONE_SelectBand()的操作结果。
        // 结构中包含选择操作完成后当前的Band状况。
        result = HandleSelectBandCnf(param);
        MMIAPIENG_BandSelectCnf(param);
        break;
        
    case APP_MN_SIM_GET_PIN_IND:
    {
#ifdef MMI_RESET_PHONE_AUTO_PIN
        POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
#endif

        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_GET_PIN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2504_112_2_18_2_45_4_26,(uint8*)"");
        // 该消息用于通知用户要求输入PIN1或者PUK1，并可获得PIN1是否已经被锁。
#if defined(MMI_SIM_LOCK_SUPPORT) || defined(MMI_RESET_PHONE_AUTO_PIN)
        if (FALSE
#if defined(MMI_SIM_LOCK_SUPPORT) && defined(MMI_RESET_PHONE_AUTO_PIN)
        || s_use_pin_in_nv
#endif
#ifdef MMI_RESET_PHONE_AUTO_PIN
        || (RESTART_BY_ASSERT == restart_condition)
#endif
        )   /*lint !e774*/
        {
            APP_MN_SIM_GET_PIN_IND_T *sim_get_pin_ind_ptr = PNULL;
            
            sim_get_pin_ind_ptr = (APP_MN_SIM_GET_PIN_IND_T *)param;
            if(MMI_DUAL_SYS_MAX<= sim_get_pin_ind_ptr->dual_sys)
            {
                //SCI_TRACE_LOW:"HandlePsAndRefMsg:dual_sys=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2514_112_2_18_2_45_4_27,(uint8*)"d",sim_get_pin_ind_ptr->dual_sys);
                result = MMI_RESULT_FALSE;
                break;
            }
            if (MNSIM_IsPin1EnableEx(sim_get_pin_ind_ptr->dual_sys)
                && MMI_PIN_MIN_LENGTH <= g_pin_value[sim_get_pin_ind_ptr->dual_sys].blocklen)//deactive ps后，由于底层不会自动重新输入pin码，需要 手动
            {
                //response pin before receive signal of sim is ready
                MNPHONE_ResponsePinEx(sim_get_pin_ind_ptr->dual_sys, MNSIM_PIN1, FALSE, &g_pin_value[sim_get_pin_ind_ptr->dual_sys], PNULL);
            }
            else
            {
                result = HandleSimPinInd(param);
            }
        }
        else
#endif
        {
            result = HandleSimPinInd(param);
        }
        }
        break;
        
    case APP_MN_SIM_PIN_FUNC_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_PIN_FUNC_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2533_112_2_18_2_45_4_28,(uint8*)"");
        // 该消息用于通知用户输入的PIN1，PUK1或者PIN2,PUK2，并可获得PIN1,PIN2是否已经被锁。
        result = HandleSimPinCnf(param);
        break;
        
    case APP_MN_SIM_READY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2539_112_2_18_2_45_4_29,(uint8*)"");
        // 该消息用于汇报SIM卡准备就绪，同时会附带PIN的状况，HPLMN 等SIM卡的信息。
        HandleSimReadyInd(param);
#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
        SCI_TRACE_LOW("[Hot_Plugin]:s_logic_sim_status =%d",s_logic_sim_status);
        if(FALSE == s_logic_sim_status)
        {
            //卡已插入
            s_logic_sim_status = TRUE;
            SCI_TRACE_LOW("[Hot_Plugin]:send MSG_PHONE_SIM_LOGIC_PLUG_IN_IND manual");
            SendLogicSimPlugMsg( MSG_PHONE_SIM_LOGIC_PLUG_IN_IND);
        }
#endif
        MMIPHONE_PinOrPukProccess(MMIPHONE_PIN_PUK_EVENT_READY);

		#ifdef FLASH_SUPPORT
			if(!MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)&&//pin 码
				!MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID)//puk 码
                #ifdef MMI_SIM_LOCK_SUPPORT
                      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
                      &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
                #endif	   
				&&MMIAPIPHONE_GetOpenIdleFlag()//这里需要打开IDLE窗口
				)
			{
			   //只有在所有的pin码和puk码都输入完成之后，才尝试进入idle，
			   //且，只有在不存在防盗保护和USB窗口的情况下才能成功进入IDLE.
			   MMIAPIPHONE_SetOpenIdleFlag(FALSE);
			   MMIAPIPHONE_AtemptEnterIdle();
			}
        #endif
        //init call log
        MMIAPICL_Init();
        
        result = MMI_RESULT_FALSE;
        break;

#ifdef MMI_NETWORK_NAME_SUPPORT
    case APP_MN_NETNAME_READY_IND:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_NETNAME_READY_IND");
        // 该消息用于在SIM卡初始化完成时，上报当前那张卡可以使用PNN
        result = MMIPHONE_HandleNetNameReadyInd(param);
        break;
                
    case APP_MN_OPER_NAME_IND:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_OPER_NAME_IND");
        // 该消息用于在SIM卡初始化完成时，上报当前那张卡可以使用PNN
        //result = MMIPHONE_HandleOperNameInd(param);
        break;

    case APP_MN_SIM_RECORD_NUM_CNF:
        // 该消息用于汇报SIM卡中PNN,OPL PNNI等总数
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_SIM_RECORD_NUM_CNF");
        result = MMIPHONE_HandleSIMRecordNumCnf(param);
        break;
    
    case APP_MN_SIM_RECORD_READ_CNF:
        // 该消息用于汇报读取SIM卡中PNN,OPL PNNI等具体某条的内容
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_SIM_RECORD_READ_CNF");
        result = MMIPHONE_HandleSIMRecordReadCnf(param);
        break;
    
    case APP_MN_SIM_BINARY_READ_CNF:
        // 该消息用于汇报读取SIM卡中SPNI具体某条的内容
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_SIM_BINARY_READ_CNF");
        //get SPNI cnf
        result = MMIPHONE_HandleSIMBinaryReadCnf(param);
        break;
#endif
            
    case APP_MN_CPHS_READY_IND:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_CPHS_READY_IND");
#ifdef MMI_CPHS_SUPPORT        
        MMIAPISET_HandleCPHSReadyInd(param);
#endif
#ifdef MMI_NETWORK_NAME_SUPPORT
        // 该消息用于在SIM卡初始化完成时，上报当前那张卡可以使用ONS
        result = MMIPHONE_HandleCPHSReadyInd(param);
#endif
        break;

#ifdef MMI_CSP_SUPPORT
    case APP_MN_LINE_LOCKSTATUS_UPDATE_IND: 
        MMIAPISET_HandleLineLockStatusUpdateInd(param);
        break;

    case APP_MN_LINE_SELECTED_UPDATE_IND:
        MMIAPISET_HandleLineSelectedUpdateInd(param);
        break;
#endif
    case APP_MN_FDN_SERVICE_IND://mmi fdn service ind
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_FDN_SERVICE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2565_112_2_18_2_45_4_30,(uint8*)"");
        result = MMI_RESULT_FALSE;
        break;
        
    case APP_MN_PS_READY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PS_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2570_112_2_18_2_45_4_31,(uint8*)"");
        e_dual_sys = ((APP_MN_PS_READY_IND_T *)param)->dual_sys;
        MMIAPISET_GetCallLineInfo(e_dual_sys);
        MMIAPIPHONE_SetIsPsReady(e_dual_sys, TRUE);
#ifdef DROPDOWN_NOTIFY_SHORTCUT
        NotifyCenter_SendMsgToShortcutWin(APP_MN_PS_READY_IND,PNULL);//bug1904633
#endif
        break;
        
    case APP_MN_SIM_NOT_READY_IND:
    {
#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
        MMI_BACKLIGHT_STATUS_E backlight_status = MMIDEFAULT_GetBackLightStateEx();
        if( MMI_BACKLIGHT_OFF == backlight_status && TRUE == s_logic_sim_status)
        {
            MMIDEFAULT_TurnOnBackLight(); // 为了优化灭屏时检测到卡拔出的时间
        }
#endif
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_NOT_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2577_112_2_18_2_45_4_32,(uint8*)"");
        // 该消息用于汇报SIM卡未能成功初始化或是SIMCARD被拔出。从该消息中可获得具体的原因，
        // 并显示在界面上。
        HandleSimNotReadyInd(param);
        MMIPHONE_PinOrPukProccess(MMIPHONE_PIN_PUK_EVENT_NOT_READY);
        result = MMI_RESULT_FALSE;
    }
        break;
        
    case APP_MN_SCELL_RSSI_IND:
        //SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_SCELL_RSSI_IND");
        // 该消息用于在服务小区信号强度改变时，汇报当前服务小区信号强度。
        // 例：在idle界面显示。
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SCELL_RSSI_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2589_112_2_18_2_45_5_33,(uint8*)"");
        result = HandleScellRssiInd(param);
        break;
        
    case APP_MN_PS_POWER_ON_CNF:
    {
        BOOLEAN is_volte_on = FALSE;
        APP_MN_PS_POWER_ON_CNF_T ps_power_on_cnf = {0};
        uint8 volte = 0;
        // 参数有效性判断
        if (PNULL == param)
        {
            SCI_TRACE_LOW("param is PNULL!");
            return MMI_RESULT_FALSE;
        }
        ps_power_on_cnf = (*(APP_MN_PS_POWER_ON_CNF_T *)param);
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PS_POWER_ON_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2594_112_2_18_2_45_5_34,(uint8*)"");
        e_dual_sys = ((APP_MN_PS_POWER_ON_CNF_T *)param)->dual_sys;
        MMIAPISET_GetCallLineInfo(e_dual_sys);
        MMIAPIPHONE_SetIsPsReady(e_dual_sys, TRUE);
        volte = MMIAPIPHONE_GetVolteInfo();
        is_volte_on =  volte & (1 << ps_power_on_cnf.dual_sys);
        if(FALSE == is_volte_on)
        {
            MMIAPIPHONE_SetVolteInfoNotToNV(ps_power_on_cnf.dual_sys, is_volte_on);//bug2100709, 2119662
        }
#ifdef DROPDOWN_NOTIFY_SHORTCUT
        NotifyCenter_SendMsgToShortcutWin(APP_MN_PS_POWER_ON_CNF ,PNULL);//bug1904633
#endif
        break;
    }
    case APP_MN_PS_POWER_OFF_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PS_POWER_OFF_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2601_112_2_18_2_45_5_35,(uint8*)"");
        HandlePowerPsOffCnf(param);
#ifdef DROPDOWN_NOTIFY_SHORTCUT
        NotifyCenter_SendMsgToShortcutWin(APP_MN_PS_POWER_OFF_CNF ,PNULL);//bug1904633
#endif
        break;
        
    case APP_MN_DEACTIVE_PS_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_DEACTIVE_PS_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2606_112_2_18_2_45_5_36,(uint8*)"");
        HandleDeactivePsCnf(param);
#ifdef DROPDOWN_NOTIFY_SHORTCUT
        NotifyCenter_SendMsgToShortcutWin(APP_MN_DEACTIVE_PS_CNF ,PNULL);//bug1904633
#endif
        break;
        
    case APP_MN_SIM_POWER_ON_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_POWER_ON_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2611_112_2_18_2_45_5_37,(uint8*)"");
#ifdef MMI_SIM_LOCK_SUPPORT
        if(MMK_IsOpenWin(PHONE_SIM_LOCK_WAIT_WIN_ID))
        {
            MMK_CloseWin(PHONE_SIM_LOCK_WAIT_WIN_ID);
        }
#endif
#if 1//SPCSS00140094
        if(MMISTK_IsStkRefresh())
        {
            MMIAPISTK_CloseRefreshWaitingWin();
        }
#endif
        HandlePowerOnSimCnf(param);
        break;

    case APP_MN_SIM_POWER_OFF_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_SIM_POWER_OFF_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2622_112_2_18_2_45_5_38,(uint8*)"");
        HandlePowerOffSimCnf(param);
        break;

#ifdef MMI_PREFER_PLMN_SUPPORT
    //更新优选网络的列表信息
    case APP_MN_PREFER_PLMN_CHANGE_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_MN_PREFER_PLMN_CHANGE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2629_112_2_18_2_45_5_39,(uint8*)"");
        HandlePreferPlmnChangeCnf(param);
        break;
#endif

#ifdef MMI_VOLTE_SUPPORT
#ifdef MODEM_SUPPORT_SPRD
    case APP_MN_MODEM_ACTIVATE_PDP_CONTEXT_IND:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_MODEM_ACTIVATE_PDP_CONTEXT_IND");
        result = MMIPHONE_HandleActivatePdpCtxInd(param);
        break;

    case APP_MN_IMS_REG_CNF:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_IMS_REG_CNF");
        break;

    case APP_MN_IMS_DEREG_CNF:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_IMS_DEREG_CNF");
        break;
#endif
    case APP_MN_IMS_STATE_IND:
        SCI_TRACE_LOW("HandlePsAndRefMsg:receive APP_MN_IMS_STATE_IND");
        result = MMIPHONE_HandleImsStatusInd(param);
        break;
#endif

#ifdef AOC_SUPPORT
        //获取PUCT计费标准信号相应处理
    case APP_MN_PUCT_INFO_CNF:
        //SCI_TRACE_LOW:"HandlePuctInfoCnf: receive APP_MN_PUCT_INFO_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2637_112_2_18_2_45_5_40,(uint8*)"");
        HandlePuctInfoCnf(param);
        break;
        
    case APP_MN_AOC_MODIFIED_IND:
        //SCI_TRACE_LOW:"None Handle: receive APP_MN_AOC_MODIFIED_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2642_112_2_18_2_45_5_41,(uint8*)"");
        break;
#endif
#if defined(MMI_SYNCTOOL_SUPPORT) || defined(MMI_SYNCTOOL_V3)//CR159271
	case ATC_AUTO_TEST_CMD_IND:
		SCI_TRACE_LOW("[atc] ATC_AUTO_TEST_CMD_IND !");
		ATC_ExtCmd(param);
		break;
#endif
#ifdef MMI_DUAL_BATTERY_SUPPORT
    case DBAT_MSG_UPDATE_STATE_IND:
    case DBAT_MSG_BAT_PLUGIN_IND:
    case DBAT_MSG_BAT_PLUGOUT_IND:
    case DBAT_MSG_SWITCH_BAT_IND://目前不处理
    case DBAT_MSG_CHARGE_START_IND:
    case DBAT_MSG_WARNING_IND:
    case DBAT_MSG_SHUTDOWN_IND://和单电池处理一致
    case DBAT_MSG_CHARGE_FINISH:
    case DBAT_MSG_CHARGE_DISCONNECT:
    case DBAT_MSG_CHARGE_FAULT:
        result = MMIPHONE_ChargeHandlePsAndRefMsg(app_ptr,msg_id,param);
        break;
#else
    case CHR_MSG_CAP_IND:
    case CHR_MSG_CHARGE_START_IND:
    case CHR_MSG_CHARGE_END_IND:
    case CHR_MSG_WARNING_IND:
    case CHR_MSG_SHUTDOWN_IND:
    case CHR_MSG_CHARGE_FINISH_IND:
    case CHR_MSG_CHARGE_DISCONNECT_IND:
    case CHR_MSG_CHARGE_FAULT_IND:
#if defined (MODEM_SUPPORT_SPRD)
    case CHR_MSG_CHARGE_FAULT_OVERTEMP_IND:
	    result = MMIPHONE_ChargeHandlePsAndRefMsg(app_ptr,msg_id,param);
        break;
#elif defined (MODEM_SUPPORT_RDA)
        case CHR_MSG_CHARGE_OVER_TEMPTURE_IND:
            result = MMIPHONE_ChargeHandlePsAndRefMsg(app_ptr,msg_id,param);
            break;
#endif

#endif
    case MISC_MSG_RTC_ALARM:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive MISC_MSG_RTC_ALARM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2671_112_2_18_2_45_5_42,(uint8*)"");
        // 该消息用于汇报闹钟所设定的时间到，开始闹铃。
        // 注：设备关机时，启动闹铃的方式与该消息无关
        // MMIAPIALM_StartAlarm(TRUE);
        MMIALMSVC_StartService();
        break;
	case MSG_SET_DEL_ALARMSET_CNF:
		//delete one alarm event finished confirm
		MMIALM_DeleteCnf();
		break;        
    case USB_PLUGIN_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_PLUGIN_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2682_112_2_18_2_45_5_43,(uint8*)"");
        // 该消息用于汇报U盘插入。
        result = MMIAPIUdisk_HandleUsbCablePlugIn();
        break;
        
    case USB_PLUGOUT_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_PLUGOUT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2688_112_2_18_2_45_5_44,(uint8*)"");
        // 该消息用于汇报U盘被拔出。
        //   MMIAPIUdisk_SetIsUse(FALSE);
        result = MMIAPIUdisk_HandleUsbCablePlugOut();
        MMIAPIPHONE_CleanEventDelaied(MMI_USB_DELAY);
        //MMIAPIUdisk_SetUSBDelay(FALSE);
        break;
	case USB_SERVICE_UDISK_STARTED_IND:
		//usb service started
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UDISK);
		break;
	case USB_SERVICE_UDISK_STOPPED_IND:
		//usb service stopped
        MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UDISK);
		break;
	case USB_SERVICE_UCOM_STARTED_IND:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UCOM);
		break;
	case USB_SERVICE_UCOM_STOPPED_IND:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UCOM);
		break;
	case USB_SERVICE_UPCC_STARTED_IND:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_UPCC);
		break;
	case USB_SERVICE_UPCC_STOPPED_IND:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_UPCC);
		break;
	case USB_SERVICE_LOG_STARTED_IND:
		MMIAPIUdisk_USBServiceStartInd(MMI_USB_SERVICE_LOG);
		break;
	case USB_SERVICE_LOG_STOPPED_IND:
		MMIAPIUdisk_USBServiceStopInd(MMI_USB_SERVICE_LOG);
		break;
    case USB_READY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2722_112_2_18_2_45_5_45,(uint8*)"");
        // 该消息用于汇报U盘已经就绪
        //USB要在菜单中开启，所以处理也在开启处
    //    MMIAPIUdisk_SetUPMIsReady(TRUE);
        break;
        
    case USB_ERROR_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive UPM_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2729_112_2_18_2_45_5_46,(uint8*)"");
        // 该消息用于汇报U盘操作失败
        //result = MMIUDISK_Handlefail();
        break;

    case APP_AUDIO_END_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_AUDIO_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2735_112_2_18_2_45_5_47,(uint8*)"");
        MMISRVAUD_HandleCallBackMsg((void *)param);
        break;
      
#ifdef WRE_SUPPORT
    case APP_WRE_AUDIO_END_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive APP_WRE_AUDIO_END_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2741_112_2_18_2_45_5_48,(uint8*)"");
        MMIWRE_HandleCallBackMsg((void *)param);
        break;
#endif

#ifdef ASP_SUPPORT
    case APP_ASP_PLAY_AUDIO_END_IND:
        MMIAPIASP_HandlePlayAudioEnd();
        break;
#endif

#ifdef JAVA_SUPPORT_SUN
    case APP_JAVA_MP4_DISPLAY_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg(),receive APP_JAVA_MP4_DISPLAY_IND!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2759_112_2_18_2_45_5_49,(uint8*)"");
        MMIAPIJAVA_CallbackFrameEndNotifyProc();
        break;

    case  APP_JAVA_MP4_END_IND:
         //SCI_TRACE_LOW:"HandlePsAndRefMsg(),receive APP_JAVA_MP4_END_IND!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2764_112_2_18_2_45_5_50,(uint8*)"");
         MMIAPIJAVA_CallbackEndNotifyProc();
         break;
    case APP_JAVA_SNAPSHOT_END_IND:
         //SCI_TRACE_LOW:"HandlePsAndRefMsg(),receive APP_JAVA_SNAPSHOT_END_IND!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2768_112_2_18_2_45_5_51,(uint8*)"");
         MMIAPIJAVA_CallbackSnapshotEndNotifyProc();
         break;
    case APP_JAVA_END_IND:
    case APP_JAVA_LAUNCH_INPUT_METHOD_IND:
    case APP_JAVA_LAUNCH_FILE_BROWSER_IND:
    case APP_JAVA_LAUNCH_WAP_BROWSER_IND:
    case APP_JAVA_START_AMS_IND:
    case APP_JAVA_STATUSBAR_OPERATION_IND:
    case APP_JAVA_SCREEN_REFLUSH_IND:
    case APP_JAVA_LAUNCH_PRELOAD_MIDLET_IND:
    case APP_JAVA_LAUNCH_VM_EXIT_WIN_IND:
    case APP_JAVA_BACKGROUND_TIMER_EVENT:
    case APP_JAVA_PDP_ACTIVE:
    case APP_JAVA_PDP_DEACTIVE:
    case APP_JAVA_VIDEO:
        //SCI_TRACE_LOW("HandlePsAndRefMsg:receive Java related message");
        MMIAPIJAVA_DispatchVmMessage(param);
        break;
#endif
#ifdef JAVA_SUPPORT_IA
    case MSG_OPEN_TEXTEDIT_WIN:
        //SCI_TRACE_LOW:"apple phone param = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2790_112_2_18_2_45_5_52,(uint8*)"d",param);
        MMIAPIJAVA_OpenTextEditWindow(PNULL);
        break;
    case MSG_JAVA_OUT_OF_MEMORY:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;
            //SCI_TRACE_LOW:"HandlePsAndRefMsg msg_id = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2794_112_2_18_2_45_5_53,(uint8*)"d", msg_id);
            MMIAPIJAVA_jwe_vmErrorHandle(common_data.data);
        }        
        break;
    case MSG_JAVA_CLOSE_APP_WIN:
        {
            APP_MN_HANDLE_DATA_T common_data = *(APP_MN_HANDLE_DATA_T*)param;
            //SCI_TRACE_LOW:"HandlePsAndRefMsg msg_id = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2794_112_2_18_2_45_5_53,(uint8*)"d", msg_id);
            MMIAPIJAVA_jwe_vmErrorHandle(common_data.handle_data);
            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_CLOSE_APP_WIN param = %d, %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2798_112_2_18_2_45_5_54,(uint8*)"dd",param,*(MMI_HANDLE_T *)param);
		    if(MMK_IsOpenWin(common_data.handle_data))
	        {
    	        MMK_CloseWin(common_data.handle_data);
	        }
        }        
        break;
    case MSG_JAVA_AUTORUNAPP:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_AUTORUNAPP param = %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2805_112_2_18_2_45_5_55,(uint8*)"d",param);
	        MMIAPIJAVA_AutoRunApp(common_data.data);	
        }        
        break;
    case  MSG_JAVA_SEND_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_SEND_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2809_112_2_18_2_45_5_56,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartSendTimer(common_data.data);
        }        
        break;
    case MSG_JAVA_LINK_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_LINK_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2813_112_2_18_2_45_5_57,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartLinkTimer(common_data.data);
        }        
        break;
    case MSG_JAVA_CONNECT_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_CONNECT_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2817_112_2_18_2_45_5_58,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartConnectTimer(common_data.data);
        }
        
        break;
    case MSG_JAVA_RECEIVE_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_RECEIVE_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2821_112_2_18_2_45_5_59,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartRecvTimer(common_data.data);
        }
        break;
    case MSG_JAVA_STOP_SOCKET_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_STOP_SOCKET_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2825_112_2_18_2_45_5_60,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StopSockTimer(common_data.data);
        }
        break;
    case MSG_JAVA_UDP_LINK:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            MMIAPIJAVA_StartUDPLink(common_data.data);
        }        
        break;
    case MSG_JAVA_PAINT:
        MMIAPIJAVA_CreateWinPaintTimer();
        break;
    case MSG_JAVA_VM_END_IND:
        MMIAPIJAVA_Exit_FinalizeResource();
        //if(MMK_IsOpenWin(*(MMI_HANDLE_T *)param))
        //	MMK_CloseWin(*(MMI_HANDLE_T *)param);
        break;
    case MSG_JAVA_VM_SUSPEND_IND:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_VM_SUSPENDED_IND param = %d %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2840_112_2_18_2_45_5_61,(uint8*)"dd", param,*(int32 *)param);
	    MMIAPIJAVA_StartCheckVmState(MSG_JAVA_VM_SUSPEND_IND,param);	 
        break;
    case MSG_JAVA_MEDIA_STOPTIME_TIMER:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;

            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_MEDIA_STOPTIME_TIMER param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2844_112_2_18_2_45_5_62,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_StartMediaStopTimer(common_data.data);
        }        
        break;
     case MSG_JAVA_DEACTIVE_PDP:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_DEACTIVE_PDP param = %d %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2848_112_2_18_2_45_5_63,(uint8*)"dd", param,*(int32 *)param);
        MMIAPIJAVA_PDPDeactive();
        break;
     case MSG_JAVA_PLATFORM_REQUEST:
        {
            APP_MN_COMMON_DATA_T common_data = *(APP_MN_COMMON_DATA_T*)param;
	
            //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_PLATFORM_REQUEST param = %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2852_112_2_18_2_45_5_64,(uint8*)"dd", param,*(int32 *)param);
            MMIAPIJAVA_PlatformRequest(*(uint32 *)common_data.data);
        }        
        break;
     case MSG_JAVA_AUDIO_PLAY_AGAIN:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg MSG_JAVA_AUDIO_PLAY_AGAIN param = %d %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2856_112_2_18_2_45_5_65,(uint8*)"dd", param,*(int32 *)param);
        MMIAPIJAVA_AudioClientPlayAgain();
        break;
#endif
     case MSG_FS_DELETE_CNF:
         {
			 MMIFILE_ASYN_CALLBACK_T  *callback_ptr = (MMIFILE_ASYN_CALLBACK_T*) param;
			 BOOLEAN                  fs_result = FALSE;

			 if(SFS_ERROR_NONE == callback_ptr->error)
			 {
				 fs_result = TRUE;
			 }
			 MMK_PostMsg(callback_ptr->win_id, callback_ptr->msg_id, &fs_result, sizeof(fs_result));
			 MMIAPICOM_TriggerMMITask();
         }
		 break;

#ifdef DRM_SUPPORT
    case DRM_SIG_DH_INSTALL_FILE_REQ:
    case DRM_SIG_DH_INSTALL_FILE_CNF:
    case DRM_SIG_GET_FILE_MIME_CNF:
	case DRM_SIG_CANCEL_INSTALL_FILE_CNF:
    case DRM_SIG_DH_DELETE_RIGHTS_FILE_CNF:

        MMIDRM_HandleDrmControlMsg(msg_id, param);
        break;
#endif

#ifdef IKEYBACKUP_SUPPORT
    case MSG_SMS_IKB_BACKUP_ITEM_DATA_REQ:
        {
            void *original_param = PNULL;

            original_param = (void*)((uint32)param);
            MMIAPISMS_IKeyBackup(original_param);
        }
        
        break;
    case MSG_SMS_IKB_RESTORE_ITEM_DATA_REQ:
        {
            void *original_param = PNULL;

            original_param = (void*)((uint32)param);
            MMIAPISMS_IKeyRestore(original_param);
        }
        break;
    case MSG_IKB_LOAD_ALL_NODE_RES:
    case MSG_IKB_BACKUP_DATA_RES:
    case MSG_IKB_BACKUP_ITEM_DATA_REQ:
    case MSG_IKB_RESTORE_DATA_RES:
    case MSG_IKB_RESTORE_ITEM_DATA_REQ:
    case MSG_IKB_GET_ALL_PACK_DATA_CNF:
    case MSG_IKB_PROGRESS_VALUE_RES:
    case MSG_IKB_CANCEL_ACTION_RES:
        MMIAPIIKB_HandleAppSigRes(msg_id, param);
        break;
#endif

#ifdef MMI_VCALENDAR_SUPPORT
    case MSG_VCALENDAR_SAVE_IND:
        MMIVCAL_HandleWriteCNF(MSG_VCALENDAR_SAVE_SCH_CNF,PNULL);
        break;
    case   MSG_SCH_GET_ALL_FILE_FINISH:
        MMK_SendMsg(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID, MSG_SCH_GET_ALL_FILE_FINISH,PNULL);
        break;
    case  MSG_SCH_LOAD_NEXT_VCALENDAR:
        MMK_SendMsg(MMISCH_VCALENDAR_LOADFILE_WAITING_WIN_ID, MSG_SCH_LOAD_NEXT_VCALENDAR,PNULL);
        break;
#ifdef CALENDAR_SUPPORT
    case MSG_SCH_DELETE_IND:
        MMIAPISCH_DelSchNotify();
        break;
#endif
#endif
#ifdef FLASH_SUPPORT
    case MSG_FLASH_UPDATE_WINDOW:
		//SCI_TRACE_LOW:"flash_printf:@@@@@MSG_FLASH_UPDATE_WINDOW  0  ,received the message."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2931_112_2_18_2_45_5_66,(uint8*)"");
        if(MMIAPIFlash_FlashIsVisiable() &&  MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
        {
			//SCI_TRACE_LOW:"flash_printf:@@@@@MSG_FLASH_UPDATE_WINDOW  1  ,received the message."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2934_112_2_18_2_45_5_67,(uint8*)"");
            MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_FULL_PAINT, 0, 0);
        }
        break;
#endif
#ifdef MMI_SNTP_SUPPORT
    case SNTP_SIG_GET_TIME_CNF:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:receive SNTP_SIG_GET_TIME_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2941_112_2_18_2_45_5_68,(uint8*)"");
        MMIAPIAUT_ManualUpdateTimeHandleSNTPcnf(msg_id, param);
    break;
#endif
#ifdef BAIDU_DRIVE_SUPPORT
	case MSG_BAIDU_DRV_INIT_CNF:
	case MSG_BAIDU_DRV_SHOW_QR:
	case MSG_BAIDU_DRV_SIGNIN_DONE:
	case MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY :
	case MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF :
	case MSG_BAIDU_DRV_ERROR_NOTIFY:
	case MSG_BAIDU_DRV_DOWNLOAD_PHOTO_NOTIFY:
	case MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF:
		result = MMIAPIBAIDU_DRV_HandleSignal(msg_id, param);
	break;
#endif
#if defined (PLATFORM_ANTISW3)
    case APP_MN_GPS_DATA_SOCKET_IND:
        SCI_TRACE_LOW("MNGPS:receive APP_MN_GPS_DATA_SOCKET_IND in mmiphone");
        MMIGPS_DataSocketHandle(param);
        break;
#endif
    default:
        //SCI_TRACE_LOW:"HandlePsAndRefMsg:msg_id = 0x%04x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2946_112_2_18_2_45_5_69,(uint8*)"d", msg_id);
        result = MMI_RESULT_FALSE;
        break;
    }

#ifdef UAL_TELE_SUPPORT
    adap_sim_handle_ps_msg(msg_id,param);
    handleTeleRadioPsMsg(msg_id,param);
#endif
    return (result);
}

/*****************************************************************************/
// 	Description : Interrogate Cfu for idle icon display
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_InterrogateCfuForIdle(MN_DUAL_SYS_E dual_sys)
{
    uint32 i = 0;
    BOOLEAN is_allow_do_cfu = TRUE;
    MN_DUAL_SYS_E cfu_dual_sys = MN_DUAL_SYS_MAX;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if ((e_dualsys_setting & (0x01 << i)) && MMIAPIPHONE_GetSimExistedStatus(i) && !s_is_network_status_ready[i])
        {
            is_allow_do_cfu = FALSE;
            break;
        }
    }
    
    if (is_allow_do_cfu && !MMIAPISET_GetIsQueryCfu())
    {
        if (!MMIAPISET_GetCfuInfo(dual_sys)
            &&((PLMN_NORMAL_GSM_ONLY == g_service_status[dual_sys].plmn_status)
            || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[dual_sys].plmn_status)))
        {
            cfu_dual_sys = dual_sys;
        }

        if (MN_DUAL_SYS_MAX == cfu_dual_sys)
        {
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (!MMIAPISET_GetCfuInfo(i)
                    &&((PLMN_NORMAL_GSM_ONLY == g_service_status[i].plmn_status)
                    || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[i].plmn_status)))
                {
                    cfu_dual_sys = (MN_DUAL_SYS_E)i;

                    break;
                }
            }
        }
        
        //SCI_TRACE_LOW:"MMIAPIPHONE_InterrogateCfuForIdle dual_sys=%d,cfu_dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_2992_112_2_18_2_45_5_70,(uint8*)"dd", dual_sys, cfu_dual_sys);
        
        if (MN_DUAL_SYS_MAX != cfu_dual_sys)
        {
            if (MMIAPIENG_GetCallForwardQuery())
            {
                //从网络中查询无条件转移状态,只需查询线路1是否设置了CFU，因为线路2无法设置CFU
                MMIAPISET_InterrogateCfuStatusFromPs(cfu_dual_sys, 0);
            }
            else
            {
                if (!MMIAPISET_GetIsQueryCfu())
                {
                    MMIAPISET_SetIsQueryCfu(TRUE,dual_sys);
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_InterrogateCfuForIdle is_allow_do_cfu=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3012_112_2_18_2_45_6_71,(uint8*)"d", is_allow_do_cfu);
    }
}

#if defined(MMI_SMSCB_SUPPORT)
/*****************************************************************************/
// 	Description : startup sms cb
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StartupSmsCB(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("StartupSmsCB:s_is_startup_smscb[dual_sys]%d",s_is_startup_smscb[dual_sys]);
    if (!s_is_startup_smscb[dual_sys])
    {
        s_is_startup_smscb[dual_sys] = TRUE;

        MMIAPISMSCB_Init(dual_sys);
    }
}

/*****************************************************************************/
// 	Description : ClearCbEnable
//	Global resource dependence : 
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
LOCAL void ClearCbEnable(MN_DUAL_SYS_E dual_sys)
{
    s_is_startup_smscb[dual_sys] = FALSE;
}
#endif

#ifdef UAL_TELE_SUPPORT
/*****************************************************************************/
// Description : ActiveUalTeleData
// Global resource dependence :
// Author: shuting.ma
// Note:bug 2114304
/*****************************************************************************/
LOCAL void ActiveUalTeleData(APP_MN_NETWORK_STATUS_IND_T network_status_ind)
{
    ual_tele_data_result_e  tele_data_ret = UAL_TELE_DATA_RES_ERROR;
    ual_tele_sim_id_e       sim_id = SIM_ID_1;

    sim_id = (ual_tele_sim_id_e)(network_status_ind.dual_sys);
    if(sim_id >= SIM_ID_MAX || sim_id < SIM_ID_1)
    {
        SCI_TRACE_LOW("[HandleNetworkStatusInd]: sim_id error: %d\n", sim_id);
        return;
    }

    SCI_TRACE_LOW("[HandleNetworkStatusInd]: sim_id = %d", sim_id);
    if((PLMN_NORMAL_GSM_ONLY == network_status_ind.plmn_status) || (PLMN_NORMAL_GPRS_ONLY == network_status_ind.plmn_status) ||(PLMN_NORMAL_GSM_GPRS_BOTH == network_status_ind.plmn_status)
     ||(PLMN_NORMAL_CS_ONLY == network_status_ind.plmn_status) ||(PLMN_NORMAL_PS_ONLY == network_status_ind.plmn_status) || (PLMN_NORMAL_CS_PS_BOTH == network_status_ind.plmn_status)
     ||(PLMN_FULL_PS_SERVICE == network_status_ind.plmn_status))
    {
        SCI_TRACE_LOW("[HandleNetworkStatusInd] gprs state is available");

        if(TRUE != ual_tele_data_get_is_active(sim_id))
        {
            tele_data_ret = ual_tele_data_active(sim_id);
            if(UAL_TELE_DATA_RES_SUCCESS == tele_data_ret)
            {
                SCI_TRACE_LOW("[HandleNetworkStatusInd] ual_tele_data_active success");
            }
            else
            {
                SCI_TRACE_LOW("[HandleNetworkStatusInd] ual_tele_data_active fail");
            }
        }
        else
        {
            SCI_TRACE_LOW("[HandleNetworkStatusInd] ual tele data is actived");
        }
	MMISRV_BAIDUDRV_Check_Network_status(TRUE);	
    }
    else
    {
        SCI_TRACE_LOW("[HandleNetworkStatusInd] gprs state is unavailable");
	 MMISRV_BAIDUDRV_Check_Network_status(FALSE);
    }
}
#endif

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_NETWORK_STATUS_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleNetworkStatusInd(
                                          DPARAM              param
                                          )
{
    APP_MN_NETWORK_STATUS_IND_T network_status_ind = *((APP_MN_NETWORK_STATUS_IND_T *)param);
    MMICONNECTION_PNNAPN_EXIST_T * pnn_apn_exist = MMIAPICONNECTION_GetPNNAPNExist(network_status_ind.dual_sys);
    PHONE_PLMN_NAME_E conection_plmn_name[MMI_DUAL_SYS_MAX] = {0};
	MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MMI_RESULT_E    result = MMI_RESULT_FALSE;    
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMISET_DAULSYS_ONLY_SIM1_ON;
    MN_DUAL_SYS_E dual_sys   = MN_DUAL_SYS_1;
SCI_TRACE_LOW("[HandleNetworkStatusInd]");
#ifdef WRE_SUPPORT
	{
		MMIWRE_NetworkIndHandle(param);
	}
#endif
    
#ifdef MMI_MULTI_SIM_SYS_SINGLE    
    network_status_ind.dual_sys = MN_DUAL_SYS_1;
#endif

#ifdef MMI_NETWORK_NAME_SUPPORT
#ifdef WIN32
    if(0 == network_status_ind.lac)
    {
         network_status_ind.lac = 0x1001;// set lac temp
    }
#endif
    if(0 != network_status_ind.lac)
    {
        MN_PLMN_T  cur_plmn = {0};

        cur_plmn.mcc  = network_status_ind.mcc;
        cur_plmn.mnc  = network_status_ind.mnc;
        cur_plmn.mnc_digit_num = network_status_ind.mnc_digit_num;
        //set  PNN str when lac changes
        MMIPHONE_SetPNNWithLac(network_status_ind.dual_sys, cur_plmn, network_status_ind.lac);
        if (pnn_apn_exist->bIsPNNAPNExist)
        {
#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
            MMIAPICONNECTION_SetPNNAPNExist(network_status_ind.dual_sys, FALSE, FALSE);
            MMINV_READ(MMINV_CONNECTION_CONNECTION_PLMN, conection_plmn_name, return_value);
        
            if (return_value != MN_RETURN_SUCCESS)
            {
                SCI_MEMSET(conection_plmn_name, 0x00, sizeof(conection_plmn_name));
            }
            else
            {
                conection_plmn_name[network_status_ind.dual_sys] = 0x00;
            }
            MMINV_WRITE(MMINV_CONNECTION_CONNECTION_PLMN, conection_plmn_name);
            SCI_TRACE_LOW("[szp]ReAutoAdapting. PNN");
            MMIAPICONNECTION_AutoAdapting(network_status_ind.dual_sys);
#endif
        }
    }
#endif

	//更新设置界面窗口
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    if ((network_status_ind.plmn_is_selecting != s_is_network_status_ready[network_status_ind.dual_sys])
        ||(network_status_ind.plmn_status != g_service_status[network_status_ind.dual_sys].plmn_status)
        )
    {
        MMIAPIPHONE_UpdateNetworkStatusWin();
    }
#endif   
    
    if (!network_status_ind.plmn_is_selecting)
    {
        s_is_network_status_ready[network_status_ind.dual_sys] = TRUE;
    }
    
    e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    if ((PLMN_NORMAL_GSM_ONLY == network_status_ind.plmn_status)
            || (PLMN_NORMAL_GPRS_ONLY == network_status_ind.plmn_status)
            || (PLMN_NORMAL_GSM_GPRS_BOTH == network_status_ind.plmn_status))
    {
        SCI_TRACE_LOW("===== HandleNetworkStatusInd BQB MMIAPIBT_Service_Indicator TRUE");
        MMIAPIBT_Service_Indicator(TRUE);
    }
    else if ((PLMN_NORMAL_GSM_ONLY != network_status_ind.plmn_status)
                 && (PLMN_NORMAL_GPRS_ONLY != network_status_ind.plmn_status)
                 && (PLMN_NORMAL_GSM_GPRS_BOTH != network_status_ind.plmn_status))
    {
        MMIAPIBT_Service_Indicator(FALSE);
    }
#endif
    
    // 保存网络状态信息
    StoreNetworkStatus(&network_status_ind);
    
    #if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetNetWorkRoamStatus(MMIAPIPHONE_GetIsNetworkRoaming());
    MMIAPIBT_SetOperatorName();
    #endif

#ifdef MMI_DUALMODE_ENABLE
    MMIAPIPHONE_TDOrGsm(network_status_ind);    
#endif

    MMIAPIPHONE_SetIdleGprsState(network_status_ind.dual_sys);

    NotifyPlmnStatusChange(network_status_ind.dual_sys, network_status_ind.plmn_status);
    
    //set idle string
    MAIN_SetIdleNetWorkName();

    MAIN_SetIdleRoamingState(network_status_ind.dual_sys, network_status_ind.plmn_is_roaming);

    //update volte status bar
#ifdef MMI_VOLTE_SUPPORT
#if 0 // TODO:VOLTE
    if(MMI_GMMREG_RAT_LTE != network_status_ind.rat)
    {
        MMIPHONE_SetVolteStatus(network_status_ind.dual_sys,FALSE);
    }
#endif
#endif
#if defined (ADULT_WATCH_SUPPORT) && defined (UAL_SS_SUPPORT)
// ss process
    if((PLMN_NORMAL_GSM_ONLY == network_status_ind.plmn_status) || (PLMN_NORMAL_GPRS_ONLY == network_status_ind.plmn_status) ||(PLMN_NORMAL_GSM_GPRS_BOTH == network_status_ind.plmn_status)
     ||(PLMN_NORMAL_CS_ONLY == network_status_ind.plmn_status) ||(PLMN_NORMAL_PS_ONLY == network_status_ind.plmn_status) || (PLMN_NORMAL_CS_PS_BOTH == network_status_ind.plmn_status)
     ||(PLMN_FULL_PS_SERVICE == network_status_ind.plmn_status))
    {
       //网络状态满足
        if( FALSE ==network_status_ind.plmn_is_selecting)
        {
            ual_tele_set_network_status(network_status_ind.dual_sys,TRUE);
            if(SETTINGS_SS_POWERON_PS_OPERATION == Settings_SS_Get_PowerOn_PS_Type())
            {
                if(FALSE == Watch_Settings_SS_Get_Operation_Status())
                {
                    Watch_Settings_SS_Start_Operation(network_status_ind.dual_sys);
                    Watch_Settings_SS_Set_Operation_Status(TRUE);
                }
            }
            else
            {
				if((FALSE == s_interrogate_done) && (TRUE == MMIAPIENG_GetCallForwardQuery()))//for bug1679904
				{
					s_interrogate_done = TRUE;
					Watch_Settings_SS_Start_Interrogate(network_status_ind.dual_sys);
				}
            }
        }
    }
#endif

#ifdef UAL_TELE_SUPPORT
    ActiveUalTeleData(network_status_ind); //bug 2114304
#endif

#if defined(MMI_CSP_SUPPORT)
    if(MMISET_IsAlsValid(network_status_ind.dual_sys))
       
    {
        MAIN_UpdateLineState();  //重新刷新line state的显示
    }
#endif //MMI_CSP_SUPPORT
#if 0
    if (!MMIAPISET_IsCfuStatusSavedInSim(network_status_ind.dual_sys))
    {
        MMIPHONE_InterrogateCfuForIdle(network_status_ind.dual_sys);        //查询Cfu
    }
#endif
#if defined(MMI_SMSCB_SUPPORT)
    StartupSmsCB(network_status_ind.dual_sys);
#endif
#if defined (MODEM_SUPPORT_SPRD)
#ifdef MMI_CLEARCODE_33_29
	if (network_status_ind.event_id == 15)
	{
		SCI_TRACE_LOW("[zsw] mmiphone.c HandleNetworkStatusInd:network_status_ind.event_id = 15, featurephone do nothing as RAU.");
	}
#endif
#endif
    SCI_TRACE_LOW("mmiphone.c HandleNetworkStatusInd:g_service_status[network_status_ind.dual_sys].plmn_status:%d",g_service_status[network_status_ind.dual_sys].plmn_status);
    if ((PLMN_NORMAL_GSM_ONLY == g_service_status[network_status_ind.dual_sys].plmn_status)
        || (PLMN_NORMAL_GPRS_ONLY == g_service_status[network_status_ind.dual_sys].plmn_status)
        || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[network_status_ind.dual_sys].plmn_status))
    {        
//#ifdef MMI_ENABLE_DCD    //for dcd internation           
//        MMIAPIDCD_SetCurMCC(g_service_status[network_status_ind.dual_sys].plmn.mcc, g_service_status[network_status_ind.dual_sys].plmn.mnc);                		    
//#endif
            
        MMIAPIDM_HandleNetChange(network_status_ind.dual_sys);
    }
    //if no service, the rx display 0 directly
    else if (!g_service_status[network_status_ind.dual_sys].is_slecting
        && PLMN_NO_SERVICE == g_service_status[network_status_ind.dual_sys].plmn_status)
    {
        g_service_status[network_status_ind.dual_sys].is_first_rssi = TRUE;//跳到No Service时，直接显示格数
        g_service_status[network_status_ind.dual_sys].rssiLevel = 0;

        MAIN_SetIdleRxLevel(network_status_ind.dual_sys, 0, TRUE);
    }
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_UpdateMainWin();
#endif

#if defined(MMI_UPDATE_TIME_SUPPORT) && defined(MMI_UPDATE_TIME_WHEN_STARTUP)
    //if(network_status_ind.gprs_support)
    {
        MMIAPIPHONE_StartGetNitzTime();
        MMIAPIAUT_PdpRetryProcess(network_status_ind.dual_sys);
    }
#endif

  if ((PLMN_NO_SERVICE != g_service_status[network_status_ind.dual_sys].plmn_status)
        &&(PLMN_REGISTER_SERVICE != g_service_status[network_status_ind.dual_sys].plmn_status)
        &&(PLMN_REGISTER_GPRS_ONLY != g_service_status[network_status_ind.dual_sys].plmn_status)
        &&(TRUE == MMICC_IsExistNoPSEccNumber()) )
    {
        // 使用先ready的sim卡拨打紧急呼叫
        SCI_TRACE_LOW("%s:%d, implemt nops ecc:plmn_status[%d]=%d", __FUNCTION__, __LINE__, 
                                        network_status_ind.dual_sys, g_service_status[network_status_ind.dual_sys].plmn_status);
        dual_sys = network_status_ind.dual_sys;
        if(MMK_IsOpenWin(MMICC_ENABLE_NETWORK_WAITING_WIN_ID))
        {
            MMK_PostMsg(MMICC_ENABLE_NETWORK_WAITING_WIN_ID, MSG_CC_NOPS_ECC_READY_SIM_ID_MSG, &(dual_sys), sizeof(dual_sys) );
        }
    }

  #ifdef SFR_SUPPORT
	MMISFR_RunGPRSModule();
    if ((MMIAPIPHONE_IsAllSimReady())&&(((PLMN_NORMAL_GSM_ONLY == network_status_ind.plmn_status) || (PLMN_NORMAL_GPRS_ONLY == network_status_ind.plmn_status) ||(PLMN_NORMAL_GSM_GPRS_BOTH == network_status_ind.plmn_status)
        ||(PLMN_NORMAL_CS_ONLY == network_status_ind.plmn_status) ||(PLMN_NORMAL_PS_ONLY == network_status_ind.plmn_status) || (PLMN_NORMAL_CS_PS_BOTH == network_status_ind.plmn_status)
        ||(PLMN_FULL_PS_SERVICE == network_status_ind.plmn_status))))
    {
        SCI_TraceLow("[Self Reg] HandleNetworkStatusInd <All Sim Ready ++MMISFR_Run++ >");
        MMISFR_Run();
    }

  #endif
  #ifdef SFR_SUPPORT_CTCC
	MMISFR_CTCC_HandleNetworkStatus();
  #endif
#ifdef SFR_SUPPORT_CMCC
    if ((MMIAPIPHONE_IsAllSimReady())&&
        (((PLMN_NORMAL_GSM_ONLY == network_status_ind.plmn_status) || (PLMN_NORMAL_GPRS_ONLY == network_status_ind.plmn_status) ||(PLMN_NORMAL_GSM_GPRS_BOTH == network_status_ind.plmn_status)
        ||(PLMN_NORMAL_CS_ONLY == network_status_ind.plmn_status) ||(PLMN_NORMAL_PS_ONLY == network_status_ind.plmn_status) || (PLMN_NORMAL_CS_PS_BOTH == network_status_ind.plmn_status)
        ||(PLMN_FULL_PS_SERVICE == network_status_ind.plmn_status))))
    {
        MMISFR_CMCC_HandleNetworkStatus();
    }
#endif
#ifdef ZDT_NET_SUPPORT
    MMIZDT_Net_StatusInd(&network_status_ind);
#endif
#ifdef ZDT_HTTP_APP_SUPPORT
    MMIHTTP_Net_StatusInd(&network_status_ind);
#endif
#ifdef WLDT_NET_SUPPORT
    MMIWLDT_Net_StatusInd(&network_status_ind);
#endif

  MAIN_UpdateIdleSignalState();//bug2138051
    return (result);
}

/*****************************************************************************/
// 	Description : 设置IDLE GPRS显示状态
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIdleGprsState(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_DUALMODE_ENABLE
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetIdleGprsState err dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3145_112_2_18_2_45_6_72,(uint8*)"d", dual_sys);
        return;
    }

    //SCI_TRACE_LOW:"[dual mode] MMIAPIPHONE_SetIdleGprsState: rat = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3149_112_2_18_2_45_6_73,(uint8*)"d", g_service_status[dual_sys].rat);

    switch(g_service_status[dual_sys].rat) 
    {
    case MMI_GMMREG_RAT_GPRS:
        MMIAPIPHONE_StopSTTDOrGsm();
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，Gprs状态
        MMISUB_UpdateDisplaySubLcd(); 
#endif
        break;
        
    case MMI_GMMREG_RAT_3G:
        MAIN_SetIdleTdState();
        break;
        
    case MMI_GMMREG_RAT_UNKNOWN:
        MMIAPIPHONE_StopSTTDOrGsm();
        MAIN_SetIdleUnknownState();
        break;
        
    default:
        break;
    }
    //Judge XX_PS_XX if support 3G_4G network
    if( (PLMN_NORMAL_PS_ONLY == g_service_status[dual_sys].plmn_status)
        || (PLMN_NORMAL_CS_PS_BOTH == g_service_status[dual_sys].plmn_status)
        || (PLMN_FULL_PS_SERVICE == g_service_status[dual_sys].plmn_status))
    {
#ifdef MMI_GPRS_SUPPORT
        if (MMIPDP_NOTIFY_EVENT_ACTIVED == MMIAPIPDP_GetPdpLastEvent(dual_sys)) /*@CR255469 modify 2011.08.23*/
        {
            MMIAPIIDLE_SetGprsInfo(dual_sys, MMIPDP_NOTIFY_EVENT_ACTIVED, FALSE); /*@CR255469 modify 2011.08.23*/
        }
        else
#endif
        {
            MAIN_SetIdleGprsState(dual_sys, TRUE);
        }
    }
    else
    {
#ifdef MMI_GPRS_SUPPORT
        if (MMIPDP_NOTIFY_EVENT_DEACTIVED == MMIAPIPDP_GetPdpLastEvent(dual_sys)) /*@CR255469 modify 2011.08.23*/
        {
            MMIAPIIDLE_SetGprsInfo(dual_sys, MMIPDP_NOTIFY_EVENT_DEACTIVED, FALSE); /*@CR255469 modify 2011.08.23*/
        }
        else
#endif
        {
            MAIN_SetIdleGprsState(dual_sys, FALSE);
        }
    }
#else
    //SCI_TRACE_LOW:"MMIAPIPHONE_SetIdleGprsState dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3201_112_2_18_2_45_6_74,(uint8*)"d", dual_sys);

    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetIdleGprsState err dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3205_112_2_18_2_45_6_75,(uint8*)"d", dual_sys);
        return;
    }

#if defined(MMI_GPRS_ATTACH_ICON_SUPPORT)
    //只有当进行GPRS连接时才显示G图标
    //Judge XX_PS_XX if support 3G_4G network
    if( (PLMN_NORMAL_PS_ONLY == g_service_status[dual_sys].plmn_status)
        || (PLMN_NORMAL_CS_PS_BOTH == g_service_status[dual_sys].plmn_status)
        || (PLMN_FULL_PS_SERVICE == g_service_status[dual_sys].plmn_status))
    {
        MAIN_SetIdleGprsState(dual_sys, TRUE);
    }
    else
    {
        MAIN_SetIdleGprsState(dual_sys, FALSE);
    }
#else
    //只要网络支持GPRS就显示G图标
    if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1)
    {
        if( (PLMN_NO_SERVICE == g_service_status[dual_sys].plmn_status)
            || (PLMN_EMERGENCY_ONLY == g_service_status[dual_sys].plmn_status) )
        {
            MAIN_SetIdleGprsState(dual_sys, FALSE);
        }
        else
        {
            MAIN_SetIdleGprsState(dual_sys, g_service_status[dual_sys].gprs_support);
        }
    }
    else
    {
        //Judge XX_PS_XX if support 3G_4G network
        if( (PLMN_NORMAL_PS_ONLY == g_service_status[dual_sys].plmn_status)
            || (PLMN_NORMAL_CS_PS_BOTH == g_service_status[dual_sys].plmn_status)
            || (PLMN_FULL_PS_SERVICE == g_service_status[dual_sys].plmn_status))
        {
            MAIN_SetIdleGprsState(dual_sys, TRUE);
        }
        else
        {
            MAIN_SetIdleGprsState(dual_sys, FALSE);
        }
    }
#endif
#endif
}

/*****************************************************************************/
// Description : 
//  User can check the netwotk's mcc/mnc, and sim's mcc/mnc.
//  If they are not equal, it is roaming.
//  But maybe the local operator regard this as not-roaming.
//  Then the user can config in this function to resolve this case.
// Global resource dependence : 
// Author: Wuding.Yang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsRealRoaming(MN_DUAL_SYS_E dual_sys, MN_PLMN_T cur_plmn)
{
    //UK GB
    //Lycamobile-SIM(234/26) on the O2-network(234/10) is not roaming
#ifdef BOCA_ROAMING_INDICATION
        uint32 i = 0;
        SCI_TRACE_LOW("CheckIsRealRoaming cur_plmn.mcc=%d,cur_plmn.mnc=%d,sim mcc=%d,sim mnc=%d",cur_plmn.mcc,cur_plmn.mnc,s_sim_status[dual_sys].hplmn.mcc,s_sim_status[dual_sys].hplmn.mnc);
        for (i = 0; i < MAX_MVNO_ROAMING_NUM; i++)
        {
            if (((s_mvno_roaming_table[i].host_mcc == cur_plmn.mcc) && (s_mvno_roaming_table[i].host_mnc == cur_plmn.mnc)) 
             && ((s_mvno_roaming_table[i].mvno_mcc == s_sim_status[dual_sys].hplmn.mcc) && (s_mvno_roaming_table[i].mvno_mnc== s_sim_status[dual_sys].hplmn.mnc)))
            {
                SCI_TRACE_LOW("CheckIsRealRoaming mvno_mcc=%d,mvno_mnc=%d,host_mcc=%d,host_mnc=%d",s_mvno_roaming_table[i].mvno_mcc,s_mvno_roaming_table[i].mvno_mnc,s_mvno_roaming_table[i].host_mcc,s_mvno_roaming_table[i].host_mnc);
                return FALSE;
            }
        }
    
        //China              460/0  460/1 
        if(((460 == cur_plmn.mcc) 
            && ( cur_plmn.mnc <= 7 || (9 == cur_plmn.mnc) || (11 == cur_plmn.mnc)))
            &&((460 == s_sim_status[dual_sys].hplmn.mcc) 
            && (s_sim_status[dual_sys].hplmn.mnc <= 7 || (9 == s_sim_status[dual_sys].hplmn.mnc) || (11 == s_sim_status[dual_sys].hplmn.mnc))))
        {
            SCI_TRACE_LOW("CheckIsRealRoaming not roaming");
            return FALSE;
        }
        
        //cmcc              460         00/02
        if(((460 == cur_plmn.mcc) 
         && ((0 == cur_plmn.mnc) ||(2 == cur_plmn.mnc)))
         &&((460 == s_sim_status[dual_sys].hplmn.mcc) 
         && ((0 == s_sim_status[dual_sys].hplmn.mnc) || (2 == s_sim_status[dual_sys].hplmn.mnc))))
        {
        SCI_TRACE_LOW("CheckIsRealRoaming cmcc  ");
         return FALSE;
        }
        
        //Virgin              234         30/33/38      EE                      234     30/33  UK
        if(((234 == cur_plmn.mcc) 
            && ((30 == cur_plmn.mnc) ||(33 == cur_plmn.mnc) || (38 == cur_plmn.mnc)))
            &&((234 == s_sim_status[dual_sys].hplmn.mcc) 
            && ((30 == s_sim_status[dual_sys].hplmn.mnc) || (33 == s_sim_status[dual_sys].hplmn.mnc))))
        {
            return FALSE;
        }
    
        //LIFE                234         33            EE                      234     30/31/32/33/34  UK
        if(((234 == cur_plmn.mcc) && ((33 == cur_plmn.mnc)))
            &&((234 == s_sim_status[dual_sys].hplmn.mcc) 
            && ((30 == s_sim_status[dual_sys].hplmn.mnc) || (31 == s_sim_status[dual_sys].hplmn.mnc) || (32 == s_sim_status[dual_sys].hplmn.mnc)
              ||(33 == s_sim_status[dual_sys].hplmn.mnc) || (34 == s_sim_status[dual_sys].hplmn.mnc))))
        {
            return FALSE;
        }
    
        //O2    262 11/7/8  262 3/5/12/17/20/77
        if(((262 == cur_plmn.mcc) 
            && ((7 == cur_plmn.mnc) ||(8 == cur_plmn.mnc)||(11 == cur_plmn.mnc)))
            &&((262 == s_sim_status[dual_sys].hplmn.mcc) 
            && ((3 == s_sim_status[dual_sys].hplmn.mnc) || (5 == s_sim_status[dual_sys].hplmn.mnc)
            || (12 == s_sim_status[dual_sys].hplmn.mnc) || (17 == s_sim_status[dual_sys].hplmn.mnc) 
            || (20 == s_sim_status[dual_sys].hplmn.mnc) || (77 == s_sim_status[dual_sys].hplmn.mnc))))
        {
            return FALSE;
        }
    
        //E-Plus    262 3/5/12/17/20/77 262 2011/7/8
        if(((262 == cur_plmn.mcc) 
             && ((3 == cur_plmn.mnc) || (5 == cur_plmn.mnc)  || (12 == cur_plmn.mnc)
             || (17 == cur_plmn.mnc) || (20 == cur_plmn.mnc) || (77 == cur_plmn.mnc)))
            &&((262 == s_sim_status[dual_sys].hplmn.mcc) 
            && ((7 == s_sim_status[dual_sys].hplmn.mnc) || (8 == s_sim_status[dual_sys].hplmn.mnc))))
        {
            return FALSE;
        }
        
        //Netherland
        if(((204 == cur_plmn.mcc) 
            && ((2 == cur_plmn.mnc)||(9 == cur_plmn.mnc)||(12 == cur_plmn.mnc)||(15 == cur_plmn.mnc)
            ||(16 == cur_plmn.mnc)||(17 == cur_plmn.mnc)||(18 == cur_plmn.mnc)||(20 == cur_plmn.mnc)))
            &&((204 == s_sim_status[dual_sys].hplmn.mcc) 
            && ((4 == s_sim_status[dual_sys].hplmn.mnc) || (8 == s_sim_status[dual_sys].hplmn.mnc) || (16 == s_sim_status[dual_sys].hplmn.mnc))))
        {
           SCI_TRACE_LOW("CheckIsRealRoaming Netherland.");
            return FALSE;
        }
        
        //Belgium
        if(((206 == cur_plmn.mcc) 
            && ((1 == cur_plmn.mnc)||(5 == cur_plmn.mnc)))
            &&((206 == s_sim_status[dual_sys].hplmn.mcc) 
            && ((1 == s_sim_status[dual_sys].hplmn.mnc) || (20 == s_sim_status[dual_sys].hplmn.mnc))))
        {
           SCI_TRACE_LOW("CheckIsRealRoaming Belgium.");
            return FALSE;
        }
        
        //Philippine
        if(((515 == cur_plmn.mcc) 
            && ((3 == cur_plmn.mnc)||(5 == cur_plmn.mnc)))
            &&((515 == s_sim_status[dual_sys].hplmn.mcc) 
            && ((3 == s_sim_status[dual_sys].hplmn.mnc) || (5 == s_sim_status[dual_sys].hplmn.mnc))))
        {
           SCI_TRACE_LOW("CheckIsRealRoaming Philippine.");
            return FALSE;
        }
#else
        //UK GB
        //Lycamobile-SIM(234/26) on the O2-network(234/10) is not roaming
        if (((234 == cur_plmn.mcc) && (10 == cur_plmn.mnc)) && ((234 == s_sim_status[dual_sys].hplmn.mcc) && (26 == s_sim_status[dual_sys].hplmn.mnc)))
        {
            return FALSE;
        }
    
        //HongKong
        //PCCW(3G)-SIM(454/19) on the PCCW(2G)-network(454/16) is not roaming
        // 3(3G)-SIM(454/03) on the 3(2G)-network(454/04) is not roaming
        //NewWorld-SIM(454/10) on the NewWorld-network(454/00) is not roaming
        if ((((454 == cur_plmn.mcc) && (16 == cur_plmn.mnc)) && ((454 == s_sim_status[dual_sys].hplmn.mcc) && (19 == s_sim_status[dual_sys].hplmn.mnc)))
            ||(((454 == cur_plmn.mcc) && (4 == cur_plmn.mnc)) && ((454 == s_sim_status[dual_sys].hplmn.mcc) && (3 == s_sim_status[dual_sys].hplmn.mnc)))
            ||(((454 == cur_plmn.mcc) && (0 == cur_plmn.mnc)) && ((454 == s_sim_status[dual_sys].hplmn.mcc) && (10 == s_sim_status[dual_sys].hplmn.mnc))))
        {
            return FALSE;
        }
#endif

    return TRUE;
}

/*****************************************************************************/
// 	Description : 保存网络状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreNetworkStatus(
                               APP_MN_NETWORK_STATUS_IND_T *status_ptr
                               )
{
    MN_DUAL_SYS_E dual_sys = status_ptr->dual_sys;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"StoreNetworkStatus error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3261_112_2_18_2_45_6_76,(uint8*)"d", dual_sys);
        return;
    }
    g_service_status[dual_sys].plmn_status = status_ptr->plmn_status;
    g_service_status[dual_sys].gprs_support = status_ptr->gprs_support;
    g_service_status[dual_sys].mbms_support = status_ptr->mbms_support;
    g_service_status[dual_sys].edge_support = status_ptr->edge_support;
    g_service_status[dual_sys].hsdpa_support = status_ptr->hsdpa_support;
    g_service_status[dual_sys].hsupa_support = status_ptr->hsupa_support;
    g_service_status[dual_sys].lac = status_ptr->lac;
#ifdef MMI_NETWORK_NAME_SUPPORT
    if((g_service_status[dual_sys].plmn.mcc != status_ptr->mcc)
    ||(g_service_status[dual_sys].plmn.mnc != status_ptr->mnc))
    {
        MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
        
        network_name[dual_sys].opn_dcs = status_ptr->opn_dcs;
        network_name[dual_sys].opn_len = status_ptr->opn_len;
        network_name[dual_sys].is_add_ci = status_ptr->is_add_ci;

        if (status_ptr->is_opn_exist && 0 < status_ptr->opn_len)
        {
            network_name[dual_sys].opn_len = MIN(status_ptr->opn_len, MAX_ONSHORTFORM_LEN);
            SCI_MEMCPY(network_name[dual_sys].opn, status_ptr->opn, network_name[dual_sys].opn_len);
        }
        else
        {
            network_name[dual_sys].opn_len = 0;
            SCI_MEMSET(network_name[dual_sys].opn, 0x00, MAX_ONSHORTFORM_LEN);
        }
    }
#endif
    g_service_status[dual_sys].plmn.mcc = status_ptr->mcc;
    g_service_status[dual_sys].plmn.mnc = status_ptr->mnc;
    g_service_status[dual_sys].plmn.mnc_digit_num	= status_ptr->mnc_digit_num;
    g_service_status[dual_sys].cell_id = status_ptr->cell_id;

    if (status_ptr->plmn_is_roaming)
    {
        g_service_status[dual_sys].isRoaming = CheckIsRealRoaming(dual_sys,g_service_status[dual_sys].plmn);
    }
    else
    {
        g_service_status[dual_sys].isRoaming = FALSE;
    }
    
    g_service_status[dual_sys].is_slecting = status_ptr->plmn_is_selecting;    

#if defined(MMI_NETWORK_NAME_SUPPORT)
    {
        MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
        
        network_name[dual_sys].spn_len= MIN(status_ptr->spn_len,MN_SPN_ID_NUM_LEN);
        if((0 < status_ptr->spn_len) && ((MN_SPN_ID_NUM_LEN + 1) > status_ptr->spn_len))
        {
            network_name[dual_sys].is_spn_supported = TRUE;
            SCI_MEMCPY(network_name[dual_sys].spn, status_ptr->spn, network_name[dual_sys].spn_len);
        }
        else
        {
            network_name[dual_sys].is_spn_supported = FALSE;
            SCI_MEMSET(network_name[dual_sys].spn, 0x00, MN_SPN_ID_NUM_LEN);
        }
    }
#else
    g_service_status[dual_sys].is_add_ci = status_ptr->is_add_ci;
    g_service_status[dual_sys].is_spn_need = status_ptr->is_spn_need;
    SCI_MEMSET(g_service_status[dual_sys].spn, 0x00, sizeof(g_service_status[dual_sys].spn));

    if (status_ptr->is_spn_need && 0 < status_ptr->spn_len)
    {
        g_service_status[dual_sys].spn_len = MIN(status_ptr->spn_len, sizeof(g_service_status[dual_sys].spn));
        SCI_MEMCPY(g_service_status[dual_sys].spn, status_ptr->spn, g_service_status[dual_sys].spn_len);
    }
    else
    {
        g_service_status[dual_sys].spn_len = 0;
    }

    g_service_status[dual_sys].opn_dcs	= status_ptr->opn_dcs;
    g_service_status[dual_sys].is_opn_need	= status_ptr->is_opn_need;
    g_service_status[dual_sys].is_opn_exist	= status_ptr->is_opn_exist;
    SCI_MEMSET(g_service_status[dual_sys].opn, 0x00, sizeof(g_service_status[dual_sys].opn));

    if (status_ptr->is_opn_exist && 0 < status_ptr->opn_len)
    {
        g_service_status[dual_sys].opn_len = MIN(status_ptr->opn_len, sizeof(g_service_status[dual_sys].opn));
        SCI_MEMCPY(g_service_status[dual_sys].opn, status_ptr->opn, g_service_status[dual_sys].opn_len);
    }
    else
    {
        g_service_status[dual_sys].opn_len = 0;
    }
#endif

    g_service_status[dual_sys].rat = (MMI_GMMREG_RAT_E)status_ptr->rat;

	SCI_TRACE_LOW("StoreNetworkStatus, plmn_status = %d, is_slecting = %d, isRoaming = %d, dual_sys = %d", g_service_status[dual_sys].plmn_status, g_service_status[dual_sys].is_slecting, g_service_status[dual_sys].isRoaming, dual_sys);

    //SCI_TRACE_LOW:"NetworkStatus g:%d, e:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3307_112_2_18_2_45_6_77,(uint8*)"ddd", status_ptr->gprs_support, status_ptr->edge_support, status_ptr->hsdpa_support);

}

/*****************************************************************************/
// 	Description : ????plmn
//	Global resource dependence : 
//  Author: yongwei.he
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetCurPLMN(
                                MN_PLMN_T * cur_plmn_ptr,
                                MN_DUAL_SYS_E dual_sys
                                )
{
    cur_plmn_ptr->mcc = 	g_service_status[dual_sys].plmn.mcc;
    cur_plmn_ptr->mnc = 	g_service_status[dual_sys].plmn.mnc;
    cur_plmn_ptr->mnc_digit_num = g_service_status[dual_sys].plmn.mnc_digit_num;	
}

/*****************************************************************************/
// 	Description : get the sim hplmn
//	Global resource dependence : 
//  Author: fen.xie
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetSimHPLMN(
                                MN_PLMN_T * cur_plmn_ptr,
                                MN_DUAL_SYS_E dual_sys
                                )
{
    cur_plmn_ptr->mcc = 	s_sim_status[dual_sys].hplmn.mcc;
    cur_plmn_ptr->mnc = 	s_sim_status[dual_sys].hplmn.mnc;
    cur_plmn_ptr->mnc_digit_num = 	s_sim_status[dual_sys].hplmn.mnc_digit_num;	
}

/*****************************************************************************/
// 	Description : Gen PLMN Display
//	Global resource dependence : 
//  Author: wancan.you
//	Note:is_display_digit_format:TRUE Display 460 00
//          is_display_digit_format:FALSE Display CMCC
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GenPLMNDisplay(
                                                                            MMI_GMMREG_RAT_E rat,//IN
                                                                            MN_PLMN_T *plmn_ptr,//IN
                                                                            wchar *display_str,//[IN/OUT]
                                                                            uint16 *display_str_len,//[IN/OUT]
                                                                            BOOLEAN is_display_digit_format)
{
    MMI_TEXT_ID_T network_name_id = TXT_NULL;
    MMI_STRING_T network_str = {0};
    BOOLEAN is_digit_output = FALSE;
#ifdef CSC_XML_SUPPORT
    BOOLEAN is_csc_name = FALSE;
#endif

    if (PNULL == plmn_ptr || PNULL == display_str || PNULL == display_str_len)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GenPLMNDisplay plmn_ptr=%d, display_str=%d, display_str_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3407_112_2_18_2_45_6_78,(uint8*)"ddd",plmn_ptr, display_str, is_display_digit_format);
        return FALSE;
    }
 
    //SCI_TRACE_LOW:"MMIAPIPHONE_GenPLMNDisplay mcc=%d, mnc=%d, mnc_digit_num=%d, str_len=%d, is_display_digit_format=%d,rat=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3412_112_2_18_2_45_6_79,(uint8*)"dddddd",plmn_ptr->mcc, plmn_ptr->mnc, plmn_ptr->mnc_digit_num, *display_str_len, is_display_digit_format, rat);

    SCI_MEMSET(display_str, 0x00, (*display_str_len * sizeof(wchar)));

#ifdef CSC_XML_SUPPORT
    is_csc_name = MMIAPIPHONE_GetNetWorkNameByCSC(plmn_ptr->mcc, plmn_ptr->mnc, &network_str);

    if (!is_csc_name)
#endif
    {
        network_name_id = MMIAPIPHONE_GetNetWorkNameId(rat, plmn_ptr);

        MMI_GetLabelTextByLang(network_name_id, &network_str);
    }

    if (PNULL != network_str.wstr_ptr
        && 0 != network_str.wstr_len
        && !is_display_digit_format)
    {
        *display_str_len = MIN(*display_str_len, network_str.wstr_len);

        MMI_WSTRNCPY(display_str,
                                *display_str_len,
                                network_str.wstr_ptr,
                                *display_str_len,
                                *display_str_len);

        is_digit_output = FALSE;
    }
    else
    {
        uint8 temp_str[MAX_MCC_MNC_DIGIT_DISPLAY + 1] = {0};
        uint16 temp_str_len = 0;

        switch (plmn_ptr->mnc_digit_num)
        {
        case 2:
            sprintf((char*)temp_str, "%03d %02d", plmn_ptr->mcc, plmn_ptr->mnc);
            break;

        case 3:
            sprintf((char*)temp_str, "%03d %03d", plmn_ptr->mcc, plmn_ptr->mnc);
            break;

        default:
            sprintf((char*)temp_str, "%03d %d", plmn_ptr->mcc, plmn_ptr->mnc);
            break;
        }

        temp_str_len = strlen((char*)temp_str);

        *display_str_len = MIN(*display_str_len, temp_str_len);

        MMI_STRNTOWSTR(display_str,
                                *display_str_len,
                                temp_str,
                                *display_str_len,
                                *display_str_len);

        is_digit_output = TRUE;
    }

    //SCI_TRACE_LOW:"MMIAPIPHONE_GenPLMNDisplay is_digit_output=%d, network_name_id=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3474_112_2_18_2_45_6_80,(uint8*)"dd", is_digit_output, network_name_id);

#ifdef CSC_XML_SUPPORT
    if (is_csc_name && PNULL != network_str.wstr_ptr)
    {
        SCI_FREE(network_str.wstr_ptr);
    }
#endif

    return is_digit_output;
}

/*****************************************************************************/

#ifdef MMI_4G_SUPPORT
LOCAL void HandleApplnInitCnf(DPARAM param)
{
    APP_MN_SIM_APPLN_INIT_CNF_T appln_init_ind = *((APP_MN_SIM_APPLN_INIT_CNF_T *)param);
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

    if (MN_SIM_APPLN_TYPE_ISIM == appln_init_ind.sim_appln_init_result.sim_appln_type)
    {
        SCI_TRACE_LOW("sim card = %d is usim, need to power on PS", appln_init_ind.dual_sys);
        MMIAPIPHONE_PowerOnPs(appln_init_ind.dual_sys);
    }
    else
    {
        SCI_TRACE_LOW("HandleApplnInitCnf dual_sys=%d, sim_appln_type=%d, result=%d, chn_id=%d",
                      appln_init_ind.dual_sys,
                      appln_init_ind.sim_appln_init_result.sim_appln_type,
                      appln_init_ind.sim_appln_init_result.init_result,
                      appln_init_ind.sim_appln_init_result.chn_id);
    }

    return;
}
#endif

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_LIST_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreferRatRsp( 
                                        DPARAM  param
                                        )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
#ifdef MMI_DUALMODE_ENABLE//@fen.xie MS00174570
    APP_MN_PREFER_RAT_RSP_T* prefer_rat_rsp = (APP_MN_PREFER_RAT_RSP_T*)param;
    
    result = MMIAPISET_HandlePreferRatRspResult(prefer_rat_rsp->set_result);
#endif
    return result;
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_LIST_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlmnListCnf(
                                     DPARAM              param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_PLMN_LIST_CNF_T plmn_list_cnf = *((APP_MN_PLMN_LIST_CNF_T *)param);

    if (plmn_list_cnf.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandlePlmnListCnf error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3508_112_2_18_2_45_7_81,(uint8*)"d", plmn_list_cnf.dual_sys);
        return MMI_RESULT_FALSE;
    }
    
    if ((MN_PHONE_OPERATE_SUCCESS == plmn_list_cnf.is_valid_result) 
        && (0 < plmn_list_cnf.plmn_detail_list.plmn_num))
    {
        MMIAPISET_OpenPlmnListWin(plmn_list_cnf.dual_sys, &(plmn_list_cnf.plmn_detail_list));
    }
#ifdef MMI_DUALMODE_ENABLE//@fen.xie MS00174570
    else if (MMIAPISET_IsPromptRetry(plmn_list_cnf.dual_sys)//单模下手动搜网且搜网前已经是无网络
             && (MN_PHONE_OPERATE_SUCCESS == plmn_list_cnf.is_valid_result)
             && (0 == plmn_list_cnf.plmn_detail_list.plmn_num))
    {
        //提示"切换到双模再试"
        MMIPUB_OpenAlertWarningWin(TXT_TO_DUAL_AND_TRY_LATER);
    }
#endif
    else
    {
        //搜索失败
        MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
    }
    
    //关闭网络连接窗口
    MMIAPISET_CloseConnectNetworkWin();
    
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PS_POWER_OFF_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerPsOffCnf(
                                        DPARAM              param
                                        )
{
    APP_MN_PS_POWER_OFF_CNF_T ps_power_off_cnf   = *((APP_MN_PS_POWER_OFF_CNF_T *)param);
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    dual_sys = ps_power_off_cnf.dual_sys;
    
    g_service_status[dual_sys].is_slecting = TRUE;
    MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, TRUE);
    g_service_status[dual_sys].is_first_rssi = TRUE;
    g_service_status[dual_sys].rssiLevel = 0;
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_Service_Indicator(FALSE);
#endif	
    MAIN_SetIdleGprsState(dual_sys, FALSE);//关闭gprs图标
    MAIN_SetIdleRxLevel(dual_sys, 0, TRUE);//信号显示0格
    MMIAPIPHONE_SetIsPsReady(dual_sys, FALSE);
    MAIN_SetIdleRoamingState(dual_sys, FALSE);
    MAIN_SetIdleForwardState(dual_sys, FALSE);
    MAIN_SetIdleGprsState(dual_sys, FALSE);
    MAIN_SetIdleNetWorkName();
#ifdef MMI_CSP_SUPPORT
    MAIN_UpdateLineState(); //重新刷新line state的显示
#endif  
#ifdef SIM_PLUG_IN_SUPPORT
    ImplementStandByModeAsync();
#endif    

#if 0//177149
    if (MMIAPISTK_IsStkRefresh())
    {
        MMIAPIPHONE_PowerReset();
    }
#endif    

    if (s_phone_is_power_off_req[dual_sys])
    {
        s_phone_is_power_off_req[dual_sys] = FALSE;

        MNPHONE_PowerOffSimEx(dual_sys);
    }
#ifdef MMS_SUPPORT
    MMIAPIMMS_DestroyOperateWaitingWin();
#endif


    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_DEACTIVE_PS_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeactivePsCnf(
                                        DPARAM              param
                                        )
{
    APP_MN_DEACTIVE_PS_CNF_T ps_deactive_cnf   = *((APP_MN_DEACTIVE_PS_CNF_T *)param);
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    dual_sys = ps_deactive_cnf.dual_sys;
    
    g_service_status[dual_sys].is_slecting = TRUE;
    MMIPHONE_SetSimStatus(dual_sys, SIM_NOT_INITED);
    MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, TRUE);
    g_service_status[dual_sys].is_first_rssi = TRUE;
    g_service_status[dual_sys].rssiLevel = 0;
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_Service_Indicator(FALSE);
#endif		
    MAIN_SetIdleGprsState(dual_sys, FALSE);//关闭gprs图标
    MAIN_SetIdleRxLevel(dual_sys, 0, TRUE);//信号显示0格
    MMIAPIPHONE_SetIsPsReady(dual_sys, FALSE);
    MAIN_SetIdleRoamingState(dual_sys, FALSE);
    MAIN_SetIdleGprsState(dual_sys, FALSE);    

    if(!MMIAPISET_IsCfuStatusSavedInSim(dual_sys))
    {
        MAIN_SetIdleForwardState(dual_sys, FALSE);			
    }  
    
#ifdef MMI_CSP_SUPPORT    
    if(MMISET_IsAlsValid(dual_sys))
       
    {
        MAIN_UpdateLineState();  //????line state???
    }    
#endif //MMI_CSP_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT
    ImplementStandByModeAsync();
#endif
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
    MMIAPISET_ImplementCTAStandbyModeAsync(dual_sys);
#endif
//bug 134457
/*#ifdef MMI_SIM_LOCK_SUPPORT
        //deactive ps and normal init
        if(MMIPHONE_GetPhoneNeedRestart(ps_deactive_cnf.dual_sys))
        {
            MMIAPIPHONE_PowerOnSIM(ps_deactive_cnf.dual_sys);
            MMIPHONE_SetPhoneNeedRestart(ps_deactive_cnf.dual_sys,FALSE);
        }
#endif     */   

    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_POWER_ON_CNF
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOnSimCnf(
                                       DPARAM              param
                                       )
{
    APP_MN_SIM_POWER_ON_CNF_T power_on_sim_cnf = *((APP_MN_SIM_POWER_ON_CNF_T *)param);
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
#endif
    
    dual_sys = power_on_sim_cnf.dual_sys;
    
    //SCI_TRACE_LOW:"HandlePowerOnSimCnf:dual_sys=%d,is_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3634_112_2_18_2_45_7_82,(uint8*)"dd", dual_sys, power_on_sim_cnf.is_ok);
    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandlePowerOnSimCnf error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3638_112_2_18_2_45_7_83,(uint8*)"d", dual_sys);
        return MMI_RESULT_FALSE;
    }
    
    MMIAPIPHONE_SetSimExistedStatus(dual_sys, power_on_sim_cnf.is_ok);

    s_is_power_on_sim_cnf[dual_sys] = TRUE;

    // Call Init ps API for current SIM Card, Because of Sim
    PHONE_InitPsWhenAllSimReady(dual_sys, MMIPHONE_SIM_READY_TO_PS_REASON_READY_BY_SIM);
#ifdef MMI_MULTI_SIM_SYS_DUAL
    if (SIM_QUERY_STATUS_START == s_query_sim_status)
    {
        uint32 i = 0;
        BOOLEAN is_query_end = TRUE;
        
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
        {
            if(!s_is_power_on_sim_cnf[i])
            {
                is_query_end = FALSE;
                break;
            }
        }

        if (is_query_end)
        {
            s_query_sim_status = SIM_QUERY_STATUS_END;//设置查询结束
        }
    }       
    
    if (SIM_QUERY_STATUS_END == s_query_sim_status && !g_is_sim_only_check_status[dual_sys])
    {
#if defined(MMI_CSP_SUPPORT)
        if(MMISET_IsAlsValid(dual_sys))
        {
            MAIN_UpdateLineState();  //重新刷新line state的显示
        }
#endif  //MMI_CSP_SUPPORT    
        
        MMIAPISET_SetIsQueryCfu(FALSE,dual_sys);
        //MMIAPIPHONE_SetGPRSAttachMode();

        //卡1不存在不用排序
        if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
        {
            MMIAPIPB_InitSimStatus(MN_DUAL_SYS_1);
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        //卡2不存在不用排序
        if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
        {
            MMIAPIPB_InitSimStatus(MN_DUAL_SYS_2);
        }
#endif
        s_query_sim_status = SIM_QUERY_STATUS_NONE;

#if defined(MMI_MSD_SUPPORT)
#ifdef MMI_SIM_LOCK_SUPPORT
        if(!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_ALERT_WIN_ID))
#endif	
        {
            MMIAPIMSD_SimLegalityCheck();//待机确认后验证防盗密码
        }
#endif//MMI_MSD_SUPPORT
    }    
#elif defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    if (SIM_QUERY_STATUS_START == s_query_sim_status)
    {
        uint32 i = 0;
        BOOLEAN is_query_end = TRUE;
        
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
        {
            if(!s_is_power_on_sim_cnf[i])
            {
                is_query_end = FALSE;
                break;
            }
        }

        if (is_query_end)
        {
            s_query_sim_status = SIM_QUERY_STATUS_END;//设置查询结束
        }
        
        if (!g_is_sim_only_check_status[dual_sys])
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
            {
                if ((dual_sys == i) && (e_dualsys_setting & (0x01 << i))
                    && MMIAPIPHONE_GetSimExistedStatus(i))
                {
                    if (MMIAPIIDLE_IdleWinIsOpen())
                    {
                        // TODO: 改为调用PHONE_InitPsWhenAllSimReady
                        MMIAPIPHONE_PowerOnPs(dual_sys);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"HandlePowerOnSimCnf: Idle is not open"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3759_112_2_18_2_45_7_84,(uint8*)"");
                    }
                    break;
                }
            }
        }
        else
        {
            MNPHONE_PowerOffSimEx(dual_sys);
        }
    }       

    if (SIM_QUERY_STATUS_END == s_query_sim_status && !g_is_sim_only_check_status[dual_sys])
    {
        uint32 i = 0;

        if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
            {
                if (e_dualsys_setting & (0x01 << i))
                {
                    // TODO: 改为调用PHONE_InitPsWhenAllSimReady
                    MMIAPIPHONE_PowerOnPs((MN_DUAL_SYS_E)i);
                    break;
                }
            }
        }

#if defined(MMI_CSP_SUPPORT)
        if(MMISET_IsAlsValid(dual_sys))
        {
            MAIN_UpdateLineState();  //重新刷新line state的显示
        }
#endif  //MMI_CSP_SUPPORT  
        
        MMIAPISET_SetIsQueryCfu(FALSE,dual_sys);
        //MMIAPIPHONE_SetGPRSAttachMode();

        //卡不存在不用排序
        for (i = 0; i < MMI_DUAL_SYS_MAX; i ++)
        {
            if (!MMIAPIPHONE_GetSimExistedStatus(i))
            {
                MMIAPIPB_InitSimStatus(i);
            }
        }
        s_query_sim_status = SIM_QUERY_STATUS_NONE;

#if defined(MMI_MSD_SUPPORT)
#ifdef MMI_SIM_LOCK_SUPPORT
        if(!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_ALERT_WIN_ID))
#endif	
        {
            MMIAPIMSD_SimLegalityCheck();//待机确认后验证防盗密码
        }
#endif//MMI_MSD_SUPPORT
    }    
#else
    //MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);//for bug1625643

#if defined(MMI_MSD_SUPPORT)
#ifdef MMI_SIM_LOCK_SUPPORT
    if(!MMK_IsOpenWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID)
            &&!MMK_IsOpenWin(PHONE_SIM_LOCK_ALERT_WIN_ID))
#endif
    {
        MMIAPIMSD_SimLegalityCheck();
    }
#endif//MMI_MSD_SUPPORT
#endif

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    //只做某一张SIM卡的优选语言的配置，其他的sim 卡不做配置。
    if(( !s_is_language_auto_set )&&( power_on_sim_cnf.is_ok ))
    {
        MMISET_LANGUAGE_TYPE_E nv_language_type = (MMISET_LANGUAGE_TYPE_E)MMIAPISET_GetNVLanguageType();

        //语言是否需要优选配置。
        if(MMISET_LANGUAGE_AUTO == nv_language_type ) 
        {
            MMISET_LANGUAGE_TYPE_E prefer_language = {0};
            BOOLEAN is_prefer_set = MMIAPISET_GetPreferLanguageTypeFromSim(dual_sys, &prefer_language);

            //SCI_TRACE_LOW:"HandlePowerOnSimCnf,dual_sys=%d,is_prefer_set=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3840_112_2_18_2_45_7_85,(uint8*)"dd",dual_sys,is_prefer_set);

            if( is_prefer_set )
            {
                s_is_language_auto_set = TRUE; //标记某张sim卡成功做了优选语言
                
                s_prefer_language_sim = dual_sys; //记录当前优选语言采用的SIM卡。

                //做系统资源的加载和设置。   
                MMIAPISET_SetLanguageType(nv_language_type);    
            }
        }
    }
#endif

    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_POWER_OFF_CNF
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePowerOffSimCnf(
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_SIM_POWER_OFF_CNF_T power_off_sim_cnf = *((APP_MN_SIM_POWER_OFF_CNF_T *)param);
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint32 i = 0;
    BOOLEAN is_sim_status_checked = TRUE;

    dual_sys = power_off_sim_cnf.dual_sys;

    //SCI_TRACE_LOW:"HandlePowerOffSimCnf:dual_sys=%d,is_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3874_112_2_18_2_45_7_86,(uint8*)"dd", dual_sys, power_off_sim_cnf.is_ok);

    g_is_sim_only_check_status[dual_sys] = FALSE;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (g_is_sim_only_check_status[i])
        {
            is_sim_status_checked = FALSE;
            break;
        }
    }

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
    if (is_sim_status_checked)
    {
        MMIAPISET_OpenCTAMultiSysStandbyWin(TRUE);
    }
#else
    MMIAPIPHONE_SetSimExistedStatus(dual_sys, FALSE);
    MMIPHONE_SetSimStatus(dual_sys, SIM_NOT_INITED);
#endif
#ifdef MMI_SIM_LOCK_SUPPORT
        //deactive ps and normal init
        if(MMIPHONE_GetPhoneNeedRestart(power_off_sim_cnf.dual_sys))
        {
            MMIAPIPHONE_PowerOnSIM(power_off_sim_cnf.dual_sys);
            MMIPHONE_SetPhoneNeedRestart(power_off_sim_cnf.dual_sys,FALSE);
        }
#else//SPCSS00140094
	if(MMISTK_IsStkRefresh()&& 4 == MMIAPISTK_GetStkRefreshType(power_off_sim_cnf.dual_sys))
	{
        MMIAPIPHONE_PowerOnSIM(power_off_sim_cnf.dual_sys);
		MMIAPISTK_SetStkRefreshType(power_off_sim_cnf.dual_sys, 0);
	}
#endif 

    return result;
}

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PREFER_PLMN_CHANGE_CNF
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreferPlmnChangeCnf(
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_PREFER_PLMN_CHANGE_CNF_T prefer_plmn_sim_cnf = *((APP_MN_PREFER_PLMN_CHANGE_CNF_T *)param);
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    dual_sys = prefer_plmn_sim_cnf.dual_sys;
   
    //SCI_TRACE_LOW:"HandlePowerOffSimCnf:dual_sys=%d,is_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3915_112_2_18_2_45_7_87,(uint8*)"dd", dual_sys, prefer_plmn_sim_cnf.is_ok);

    MMIAPISET_HandlePreferNetworkListCnf(&prefer_plmn_sim_cnf);
    
    return result;
}
#endif

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_PLMN_SELECT_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectPlmnCnf(
                                       DPARAM              param
                                       )
{
    APP_MN_PLMN_SELECT_CNF_T    plmn_select_cnf = *((APP_MN_PLMN_SELECT_CNF_T *)param);
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;
    
    dual_sys = plmn_select_cnf.dual_sys;
    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleSelectPlmnCnf error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3939_112_2_18_2_45_7_88,(uint8*)"d", dual_sys);
        return MMI_RESULT_FALSE;
    }

    if (plmn_select_cnf.plmn_is_selected)
    {
        //success!
        // 保存网络状态信息
        g_service_status[dual_sys].plmn_status = plmn_select_cnf.plmn_status;
        g_service_status[dual_sys].lac         = plmn_select_cnf.lac;
        g_service_status[dual_sys].plmn.mcc         = plmn_select_cnf.mcc;
        g_service_status[dual_sys].plmn.mnc         = plmn_select_cnf.mnc;
        g_service_status[dual_sys].plmn.mnc_digit_num = plmn_select_cnf.mnc_digit_num;
        g_service_status[dual_sys].cell_id     = plmn_select_cnf.cell_id;
           
        MMIPUB_OpenAlertSuccessWin(TXT_PLMN_SELECT_COMPLETE);

        //关闭网络列表窗口
        MMIAPISET_ClosePlmnListWin();
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_PLMN_SELECT_FAIL);
    }
    
    //关闭网络连接窗口
    MMIAPISET_CloseConnectNetworkWin();
    
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SET_BAND_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectBandCnf(
                                       DPARAM              param
                                       )
{
    APP_MN_SET_BAND_CNF_T    band_select_cnf = *((APP_MN_SET_BAND_CNF_T *)param);
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    MMIPUB_CloseAlertWin();
    if (!MMIAPIENG_IsBandSelectWinOpen())
    {
        //SCI_TRACE_LOW:"HandleSelectBandCnf band_select_cnf.is_ok = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_3984_112_2_18_2_45_8_89,(uint8*)"d",band_select_cnf.is_ok);
#if 0
        if (band_select_cnf.is_ok)
        {
            //success!
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
        }
#endif
        //关闭网络连接窗口
        MMIAPISET_CloseConnectNetworkWin();
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : get the network name
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetNetWorkNameId(
                                     MN_DUAL_SYS_E dual_sys,
                                     MN_PLMN_T *plmn_ptr,
                                     MN_PHONE_PLMN_STATUS_E  plmn_status,
                                     BOOLEAN                 is_slecting
                                     )
{
    MMI_TEXT_ID_T   network_name_id = TXT_NULL;

    if(PNULL == plmn_ptr)
    {
        //SCI_TRACE_LOW:"GetNetWorkNameId: the network plmn_ptr is error! %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4016_112_2_18_2_45_8_90,(uint8*)"d",plmn_ptr);
        return  TXT_NULL;
    }

	SCI_TRACE_LOW("GetNetWorkNameId, is_slecting = %d, plmn_status = %d, dual_sys = %d", is_slecting, plmn_status, dual_sys);

    if (is_slecting)
    {
        network_name_id = TXT_PROCESS_SEARCHING_NETWORK;
    }
    else
    {
        switch(plmn_status)
        {
        case PLMN_NO_SERVICE:
            network_name_id = TXT_NO_SERVICE;
            break;

        case PLMN_EMERGENCY_ONLY:
        case PLMN_EMERGENCY_SIM_INVALID:
        case PLMN_EMERGENCY_GPRS_ONLY:
        case PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY:
            network_name_id = TXT_KL_PSW_EMERGENCY_CALL;
            break;

        case PLMN_REGISTER_GPRS_ONLY:
        case PLMN_REGISTER_SERVICE:
            network_name_id = TXT_REGISTER_SERVICE;
            break;

        case PLMN_FULL_PS_SERVICE:
        case PLMN_NORMAL_GPRS_ONLY:
        case PLMN_NORMAL_GSM_ONLY:
        case PLMN_NORMAL_GSM_GPRS_BOTH:
            network_name_id = MMIAPIPHONE_GetNetWorkNameId(MMIAPIPHONE_GetTDOrGsm(dual_sys), plmn_ptr);

            if (TXT_NULL == network_name_id)
            {
                network_name_id = TXT_FULL_SERVICE;
            }
            break;

        default:
            //SCI_TRACE_LOW:"GetNetWorkNameId: the network status is error! %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4057_112_2_18_2_45_8_91,(uint8*)"d", plmn_status);
            network_name_id = TXT_KL_PSW_EMERGENCY_CALL;
            break;
        }
    }

    return (network_name_id);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_GET_PIN_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPinInd(
                                   DPARAM              param
                                   )
{    
    APP_MN_SIM_GET_PIN_IND_T         sim_get_pin_ind = *((APP_MN_SIM_GET_PIN_IND_T *)param);
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 i = 0;
    BOOLEAN is_need_handle = FALSE;
#endif

    if(sim_get_pin_ind.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleSimPinInd:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4084_112_2_18_2_45_8_92,(uint8*)"d", sim_get_pin_ind.dual_sys);
        return MMI_RESULT_FALSE;
    }

    // Call Init ps API for current SIM Card, Because of PIN
    PHONE_InitPsWhenAllSimReady(sim_get_pin_ind.dual_sys, MMIPHONE_SIM_READY_TO_PS_REASON_READY_BY_PIN);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    s_handle_pin_status.sim_pin_ind[sim_get_pin_ind.dual_sys] = sim_get_pin_ind;

    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(( i != sim_get_pin_ind.dual_sys)&&(s_handle_pin_status.is_handling_pin[i]))
        {
            is_need_handle = TRUE;
            break;            
        }
    }
    
    if(is_need_handle)
    {
        s_handle_pin_status.need_handle_pin[sim_get_pin_ind.dual_sys] = TRUE;
		
        return result;
    }
    else
    {            
        s_handle_pin_status.is_handling_pin[sim_get_pin_ind.dual_sys] = TRUE;   //set the status (lock)
        s_handle_pin_status.cur_handling_sim_id = sim_get_pin_ind.dual_sys;
    } 
#else
    s_handle_pin_status.sim_pin_ind[sim_get_pin_ind.dual_sys] = sim_get_pin_ind;
    s_handle_pin_status.cur_handling_sim_id = sim_get_pin_ind.dual_sys;
#endif
    MMIPHONE_PinOrPukProccess(MMIPHONE_PIN_PUK_EVENT_INPUT);
    //CreateInputPinWindow(sim_get_pin_ind);

    //MMIAPIPHONE_SetGPRSAttachMode();
    //Bug 1346427 - [TJ][Telephony][W317][Phone][128M]开启飞行模式后，在SIM卡pin锁界面无法呼通紧急呼叫，一直提示please wait
    s_is_power_on_sim_cnf[sim_get_pin_ind.dual_sys] = TRUE;
    
    return result;
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_PIN_FUNC_CNF
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPinCnf(
                                   DPARAM              param
                                   )
{
    APP_MN_SIM_PIN_FUNC_CNF_T        sim_pin_cnf = *((APP_MN_SIM_PIN_FUNC_CNF_T *)param);
    MMI_WIN_ID_T         window_id = MMK_GetFocusWinId();
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    //cr17666,请稍候界面来闹钟，接收到ps消息仍需要处理
    if (MMK_IsOpenWin(PHONE_PIN_WAIT_WIN_ID))
    {
        window_id = PHONE_PIN_WAIT_WIN_ID;
    }
    else if (MMK_IsOpenWin(IDLE_SPECIAL_WAIT_WIN_ID))
    {
        window_id = IDLE_SPECIAL_WAIT_WIN_ID;
    }
    
    switch (window_id)
    {
    case PHONE_PIN_WAIT_WIN_ID:
        if ((sim_pin_cnf.pin_operate_result) == MNSIM_PIN_REQ_OK)
        {
            if (0 != g_operate_pin_win_tab)
            {
                MMIAPISET_OpenMsgBox(g_operate_pin_win_tab, g_is_operate_pin_special_func);
            }
            
            if (MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID) && (MNSIM_PIN_CHANGE != sim_pin_cnf.pin_function_type)) //记录下正确的pin码
            {
                MMI_STRING_T   pin_value = {0};   
                GUIEDIT_GetString(MMI_PIN_INPUT_EDITBOX_CTRL_ID, &pin_value);
                g_pin_value[sim_pin_cnf.dual_sys].blocklen = pin_value.wstr_len;

                MMI_WSTRNTOSTR(
                    g_pin_value[sim_pin_cnf.dual_sys].blockbuf,
                    MN_MAX_BLOCK_LEN,
                    pin_value.wstr_ptr,
                    g_pin_value[sim_pin_cnf.dual_sys].blocklen,
                    g_pin_value[sim_pin_cnf.dual_sys].blocklen
                    );
#ifdef MMI_RESET_PHONE_AUTO_PIN
                //set sim pin to nv
                MMINV_WRITE(MMINV_SIM_PIN_VALUE, g_pin_value);
#endif       
            }
            
            MMK_CloseWin(MMI_PIN_INPUTING_WIN_ID);
            MMK_CloseWin(MMI_PUK_INPUTING_WIN_ID);

            if (!g_is_operate_pin_special_func || 0 == g_operate_pin_win_tab)
            {
                // 关闭等待窗口
                MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
            }
        }
        else
        {
            if (sim_pin_cnf.pin_status.pin_blocked) //PIN被锁,要求输入PUK
            {
                if (MMK_IsOpenWin(MMI_PUK_INPUTING_WIN_ID))
                {
                    if (MNSIM_PIN1 == sim_pin_cnf.pin_num)//PIN1
                    {
                        if (sim_pin_cnf.unblock_pin_status.pin_blocked)
                        {
                            //提示错误窗口,PUK错被锁!
                            MMIPUB_OpenAlertWarningWin(TXT_PUK_BLOCKED);

                            MMK_CloseWin(MMI_PUK_INPUTING_WIN_ID);
                        }
                        else
                        {
                            //提示错误窗口,PUK错误!
                            MMIPUB_OpenAlertWarningWin(TXT_PUK_ERROR);

                            //清空password
                            GUIEDIT_ClearAllStr(MMI_PUK_INPUT_EDITBOX_CTRL_ID);
                        }
                    }
                    else//PIN2
                    {
                        if (sim_pin_cnf.unblock_pin_status.pin_blocked)
                        {
                            //提示错误窗口,PUK2被锁!
                            MMIPUB_OpenAlertWarningWin(TXT_PUK2_BLOCKED);

                            MMK_CloseWin(MMI_PUK_INPUTING_WIN_ID);
                        }
                        else
                        {
                            //提示错误窗口,PUK2错误!
                            MMIPUB_OpenAlertWarningWin(TXT_PUK2_ERROR);

                            //清空password
                            GUIEDIT_ClearAllStr(MMI_PUK_INPUT_EDITBOX_CTRL_ID);
                        }
                    }
                }
                else
                {
                    MMI_OPERATE_PIN_T   *operate_pin_ptr = (MMI_OPERATE_PIN_T *)MMK_GetWinAddDataPtr(MMI_PIN_INPUTING_WIN_ID);

                    SCI_MEMSET(&s_operate_pin_blocked_info,0,sizeof(MMI_OPERATE_PIN_T));
                    SCI_MEMCPY(&s_operate_pin_blocked_info,operate_pin_ptr,sizeof(MMI_OPERATE_PIN_T));

                    if (MNSIM_PIN1 == sim_pin_cnf.pin_num)//PIN1
                    {
                        //提示错误窗口,PIN被锁!
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN_BLOCKED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlockedExceptPhone);
                    }
                    else//PIN2
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN2_BLOCKED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlockedExceptPhone);
                    }
                }
            }
            else
            {
                if (MNSIM_PIN1 == sim_pin_cnf.pin_num)//PIN1
                {
                    MMIPUB_OpenAlertWarningWin(TXT_PIN_ERROR);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_PIN2_ERROR);
                }

                if (MMK_IsOpenWin(MMI_PIN_INPUTING_WIN_ID))
                {
                    //清空password
                    GUIEDIT_ClearAllStr(MMI_PIN_INPUT_EDITBOX_CTRL_ID);
                }
            }

            // 关闭等待窗口
            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        }
        break;
        
    case IDLE_SPECIAL_WAIT_WIN_ID://idle win,input expecial string
        //SCI_TRACE_LOW:"HandleSimPinCnf:pin_operate_result=%d, pin_num=%d, pin_function_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4267_112_2_18_2_45_8_93,(uint8*)"ddd",sim_pin_cnf.pin_operate_result, sim_pin_cnf.pin_num, sim_pin_cnf.pin_function_type);

        switch (sim_pin_cnf.pin_operate_result)
        {
        case MNSIM_PIN_REQ_OK:
            if (MNSIM_PIN_UNBLOCK == sim_pin_cnf.pin_function_type)
            {
                MMIPUB_OpenAlertSuccessWin(TXT_CODE_ACCEPT);
            }
            else
            {
                if (MNSIM_PIN1 == sim_pin_cnf.pin_num)
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_PIN_CHANGED);
                }
                else
                {
                    MMIPUB_OpenAlertSuccessWin(TXT_PIN2_CHANGED);
                }
            }  
            break;

        case MNSIM_PIN_ERROR_WITH_BLOCKED:
            if (MNSIM_PIN1 == sim_pin_cnf.pin_num)
            {
                if (sim_pin_cnf.unblock_pin_status.pin_blocked)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_PUK_BLOCKED);
                }
                else
                {
                    SCI_MEMSET(&s_operate_pin_blocked_info,0,sizeof(MMI_OPERATE_PIN_T));
                    s_operate_pin_blocked_info.operate_type = sim_pin_cnf.pin_function_type;
                    s_operate_pin_blocked_info.pin_num = sim_pin_cnf.pin_num;

                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN_BLOCKED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlockedExceptPhone);
                }
            }
            else
            {
                if (sim_pin_cnf.unblock_pin_status.pin_blocked)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_PUK2_BLOCKED);
                }
                else
                {
                    SCI_MEMSET(&s_operate_pin_blocked_info,0,sizeof(MMI_OPERATE_PIN_T));
                    s_operate_pin_blocked_info.operate_type = sim_pin_cnf.pin_function_type;
                    s_operate_pin_blocked_info.pin_num = sim_pin_cnf.pin_num;

                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN2_BLOCKED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlockedExceptPhone);
                }
            }
            break;

        case MNSIM_PIN_ERROR_CONTRADICT_STATUS:
            if (MNSIM_PIN1 == sim_pin_cnf.pin_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_ENABLE_PIN1);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_ENABLE_PIN2);
            }
            break;

        case MNSIM_PIN_ERROR_PIN_FAIL:
            if (MNSIM_PIN1 == sim_pin_cnf.pin_num)
            {
                if (MNSIM_PIN_UNBLOCK == sim_pin_cnf.pin_function_type)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_PUK_ERROR);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_PIN_ERROR);
                }
            }
            else
            {
                if (MNSIM_PIN_UNBLOCK == sim_pin_cnf.pin_function_type)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_PUK2_ERROR);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_PIN2_ERROR);
                }
            }
            break;

        default:
            MMIPUB_OpenAlertWarningWin(TXT_ERROR);
            break;
        }
        // 关闭等待窗口
        MMK_CloseWin(IDLE_SPECIAL_WAIT_WIN_ID);
        // close idle dialing win
        MMIAPIIDLE_CloseDialWin();
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    //if assert reset and pin operate fale, enter pin input win
#ifdef MMI_RESET_PHONE_AUTO_PIN
    {
        POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
        
        if((RESTART_BY_ASSERT == restart_condition) &&((sim_pin_cnf.pin_operate_result) != MNSIM_PIN_REQ_OK))
        {
            if(!MMK_IsOpenWin(PHONE_PIN_WAIT_WIN_ID) && !MMK_IsOpenWin(IDLE_SPECIAL_WAIT_WIN_ID))
            {
                APP_MN_SIM_GET_PIN_IND_T         sim_get_pin_ind = {0};
                MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                uint32 i = 0;
                BOOLEAN is_need_handle = FALSE;
#endif
                //set pin ind
                sim_get_pin_ind.dual_sys = sim_pin_cnf.dual_sys;
                sim_get_pin_ind.pin_num = sim_pin_cnf.pin_num;
                sim_get_pin_ind.pin_is_blocked = sim_pin_cnf.pin_status.pin_blocked;
                sim_get_pin_ind.remain_times = sim_pin_cnf.pin_status.false_remain;
                
                if(sim_get_pin_ind.dual_sys >= MMI_DUAL_SYS_MAX)
                {
                    //SCI_TRACE_LOW:"HandleSimPinInd:error dual_sys=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4084_112_2_18_2_45_8_92,(uint8*)"d", sim_get_pin_ind.dual_sys);
                    return MMI_RESULT_FALSE;
                }

                if (e_dualsys_setting & (0x01 << sim_get_pin_ind.dual_sys))
                {
                    PowerOnPsForEmerge(sim_get_pin_ind.dual_sys);
                }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
                s_handle_pin_status.sim_pin_ind[sim_get_pin_ind.dual_sys] = sim_get_pin_ind;

                for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
                {
                    if(( i != sim_get_pin_ind.dual_sys)&&(s_handle_pin_status.is_handling_pin[i]))
                    {
                        is_need_handle = TRUE;
                        break;            
                    }
                }
                
                if(is_need_handle)
                {
                    s_handle_pin_status.need_handle_pin[sim_get_pin_ind.dual_sys] = TRUE;
            		
                    return result;
                }
                else
                {            
                    s_handle_pin_status.is_handling_pin[sim_get_pin_ind.dual_sys] = TRUE;   //set the status (lock)
                    s_handle_pin_status.cur_handling_sim_id = sim_get_pin_ind.dual_sys;
                }    
#endif
                CreateInputPinWindow(sim_get_pin_ind);
            }
        }
    }
#endif    
   
    return (result);
}

/*****************************************************************************/
// 	Description : 在提示还可以 输入多少次密码后，启动pin输入框
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePin1Maxtimes(
                                      MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                      MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                      DPARAM 		        param		// 相应消息的参数
                                      )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        // close input pin window
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
        
        // 打开输入PIN的界面
        MMK_CreatePubEditWin((uint32*)PHONE_PIN_INPUTING_WIN_TAB,PNULL);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_LOSE_FOCUS:
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}


/*****************************************************************************/
// 	Description : 在pin blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinBlocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO	lcd_dev_info = {0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        // close input pin window
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);

        MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
        // 打开输入PUK的界面
        MMK_CreatePubEditWin((uint32*)PHONE_PUK_INPUTING_WIN_TAB,PNULL);
        break;

    case MSG_APP_CANCEL://pin被锁输入界面不允许返回
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 在pin1或者pin2 blocked的情况下关闭窗口，则启动输入puk的窗口
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinBlockedExceptPhone(
                                               MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                               MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                               DPARAM 		        param		// 相应消息的参数
                                               )
{
    MMI_OPERATE_PIN_T   *operate_pin_ptr = &s_operate_pin_blocked_info;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        {
            MMI_OPERATE_PIN_T   *new_opreate_pin_ptr = SCI_ALLOC_APP(sizeof(MMI_OPERATE_PIN_T));
            
            MMI_MEMCPY(new_opreate_pin_ptr,
                sizeof(MMI_OPERATE_PIN_T),
                operate_pin_ptr,
                sizeof(MMI_OPERATE_PIN_T),
                sizeof(MMI_OPERATE_PIN_T));
            
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            
            new_opreate_pin_ptr->operate_type = MNSIM_PIN_UNBLOCK;
            // 打开输入PUK的界面
            MMK_CreatePubEditWin((uint32*)MMI_PUK_INPUTING_WIN_TAB,(ADD_DATA)new_opreate_pin_ptr);
            
            // close input pin window
            MMK_CloseWin(MMI_PIN_INPUTING_WIN_ID);
        }
        break;
        
    case MSG_APP_CANCEL://pin被锁输入界面不允许返回
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

    case MSG_LOSE_FOCUS:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_READY_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimReadyInd(
                                     DPARAM              param
                                     )
{
    APP_MN_SIM_READY_IND_T      sim_ready_ind   = *((APP_MN_SIM_READY_IND_T *)param);
    MMI_WIN_ID_T     window_id       = MMK_GetFocusWinId();
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    uint32 i = 0;
    BOOLEAN is_handling_pin = FALSE;
    MMI_STRING_T   pin_value = {0};
    uint8 index_xcap = 0;
    uint8 index_internet = 0;
    uint8 index_ia = 0;
    uint8 index_ims = 0;
    uint8 index_sos = 0;
    MMICONNECTION_APNTYPEINDEX_T * apntype_index = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_ia_setting_ptr = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_ims_setting_ptr = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_xcap_setting_ptr = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_sos_setting_ptr = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_internet_setting_ptr = PNULL;
    ual_tele_data_apn_info_t apn_info_item = {0};
    ual_tele_radio_apn_info_t apn_info = {0};
    ual_tele_radio_result_e radio_set_apn_ret = 0;

    if (sim_ready_ind.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return result;
    }
#ifdef WIN32
	sim_ready_ind.hplmn.mcc = 640;//460;
	sim_ready_ind.hplmn.mnc = 11;//0;
	sim_ready_ind.hplmn.mnc_digit_num = 2;
#endif
    // 保存 SIM 状态信息
    StoreSimReadyStatus(&sim_ready_ind);
    
    MMIAPIPHONE_SetSimExistedStatus(sim_ready_ind.dual_sys, TRUE);
    if(CBNET_CHINA_MCC == sim_ready_ind.hplmn.mcc && CBNET_CHINA_MNC == sim_ready_ind.hplmn.mnc)
    {
        SCI_TRACE_LOW("mcc = 460, mnc = 15,set cemode = 3");
        MNPHONE_SetCEMode(3, sim_ready_ind.dual_sys);//bug2094189
    }
#ifdef MMI_GPRS_SUPPORT
#ifdef MMI_SIM_NODIGITNUM_SUPPORT
    SCI_TRACE_LOW("mmiphone.c HandleSimReadyInd:sim_ready_ind.hplmn.mnc_digit_num= %d", sim_ready_ind.hplmn.mnc_digit_num);
    if (MCC_MNC_DIGIT_INVALID == sim_ready_ind.hplmn.mnc_digit_num)
    {
        SCI_TRACE_LOW("mmiphone.c HandleSimReadyInd:s_sim_status.mcc= %d,s_sim_status.mnc= %d,s_sim_status.mnc_digit_num= %d", s_sim_status[sim_ready_ind.dual_sys].hplmn.mcc,s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc ,s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num);
        // correct sim mnc and digtal len
        if(404 == s_sim_status[sim_ready_ind.dual_sys].hplmn.mcc) 
        {

            s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc /= 10 ;
            
            s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 2;
        }
        else if(405 == s_sim_status[sim_ready_ind.dual_sys].hplmn.mcc)
        {
            if(s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc > 249 && s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc < 480)
            {
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc /= 10 ;
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 3;
            }
            else if(s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc > 749)
            {
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 3;
            }
            else if(s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc > 0 && s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc < 750)
            {
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc /= 10 ;
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 2;
            }   
            else
            {
                s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 2;
            }           
        }
        else
        {
            s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc /= 10 ;
            s_sim_status[sim_ready_ind.dual_sys].hplmn.mnc_digit_num = 2;
        } 
    }
#endif
    MMIAPICONNECTION_SetPNNAPNExist(sim_ready_ind.dual_sys, FALSE, TRUE);
#ifdef UAL_TELE_SUPPORT
    MMIAPICONNECTION_AutoAdapting(sim_ready_ind.dual_sys);
    apntype_index = MMIAPICONNECTION_GetApnTypeIndex(sim_ready_ind.dual_sys);
    if (NULL != apntype_index)
    {
        if (TRUE == apntype_index->xcap_index[sim_ready_ind.dual_sys].is_valid)
        {
            index_xcap = apntype_index->xcap_index[sim_ready_ind.dual_sys].index;
            SCI_TRACE_LOW("tele radio index xcap = %d", index_xcap);
            link_xcap_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_ready_ind.dual_sys, index_xcap);
        }

        if (TRUE == apntype_index->internet_index[sim_ready_ind.dual_sys].is_valid)
        {
            index_internet = apntype_index->internet_index[sim_ready_ind.dual_sys].index;
            SCI_TRACE_LOW("tele radio index internet = %d", index_internet);
            link_internet_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_ready_ind.dual_sys, index_internet);
        }

        if (TRUE == apntype_index->ia_index[sim_ready_ind.dual_sys].is_valid)
        {
            index_ia = apntype_index->ia_index[sim_ready_ind.dual_sys].index;
            SCI_TRACE_LOW("tele radio ia index = %d", index_ia);
            link_ia_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_ready_ind.dual_sys, index_ia);
        }

        if (TRUE == apntype_index->ims_index[sim_ready_ind.dual_sys].is_valid)
        {
            index_ims = apntype_index->ims_index[sim_ready_ind.dual_sys].index;
            SCI_TRACE_LOW("tele radio ims index = %d", index_ims);
            link_ims_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_ready_ind.dual_sys, index_ims);
        }

        if (TRUE == apntype_index->sos_index[sim_ready_ind.dual_sys].is_valid)
        {
            index_ims = apntype_index->sos_index[sim_ready_ind.dual_sys].index;
            SCI_TRACE_LOW("tele radio sos index = %d", index_sos);
            link_sos_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_ready_ind.dual_sys, index_sos);
        }

        if (NULL != link_xcap_setting_ptr)
        {
            SCI_MEMSET(&apn_info_item, 0, sizeof(ual_tele_data_apn_info_t));
            apn_info_item.auth_type = link_xcap_setting_ptr->auth_type;
            apn_info_item.ip_type = link_xcap_setting_ptr->ip_type;
            SCI_MEMCPY(apn_info_item.apn, link_xcap_setting_ptr->apn, sizeof(uint8) * link_xcap_setting_ptr->apn_len);
            SCI_MEMCPY(apn_info_item.password, link_xcap_setting_ptr->password, sizeof(uint8) * link_xcap_setting_ptr->password_len);
            SCI_MEMCPY(apn_info_item.username, link_xcap_setting_ptr->username, sizeof(uint8) * link_xcap_setting_ptr->username_len);
            SCI_MEMCPY(&(apn_info.xcap_apn_info[sim_ready_ind.dual_sys]),&apn_info_item, sizeof(ual_tele_data_apn_info_t));
            SCI_TRACE_LOW("tele radio link_xcap_setting_ptr->username = %s, apn = %s", link_xcap_setting_ptr->username, link_xcap_setting_ptr->apn);
        }

        if (NULL != link_internet_setting_ptr)
        {
            SCI_MEMSET(&apn_info_item, 0, sizeof(ual_tele_data_apn_info_t));
            apn_info_item.auth_type = link_internet_setting_ptr->auth_type;
            apn_info_item.ip_type = link_internet_setting_ptr->ip_type;
            SCI_MEMCPY(apn_info_item.apn, link_internet_setting_ptr->apn, sizeof(uint8) * link_internet_setting_ptr->apn_len);
            SCI_MEMCPY(apn_info_item.password, link_internet_setting_ptr->password, sizeof(uint8) * link_internet_setting_ptr->password_len);
            SCI_MEMCPY(apn_info_item.username, link_internet_setting_ptr->username, sizeof(uint8) * link_internet_setting_ptr->username_len);
            SCI_MEMCPY(&(apn_info.internet_apn_info[sim_ready_ind.dual_sys]),&apn_info_item, sizeof(ual_tele_data_apn_info_t));
            SCI_TRACE_LOW("tele radio link_internet_setting_ptr->username = %s, apn = %s", link_internet_setting_ptr->username, link_internet_setting_ptr->apn);
        }

        if (NULL != link_ia_setting_ptr)
        {
            SCI_MEMSET(&apn_info_item, 0, sizeof(ual_tele_data_apn_info_t));
            apn_info_item.auth_type = link_ia_setting_ptr->auth_type;
            apn_info_item.ip_type = link_ia_setting_ptr->ip_type;
            SCI_MEMCPY(apn_info_item.apn, link_ia_setting_ptr->apn, sizeof(uint8) * link_ia_setting_ptr->apn_len);
            SCI_MEMCPY(apn_info_item.password, link_ia_setting_ptr->password, sizeof(uint8) * link_ia_setting_ptr->password_len);
            SCI_MEMCPY(apn_info_item.username, link_ia_setting_ptr->username, sizeof(uint8) * link_ia_setting_ptr->username_len);
            SCI_MEMCPY(&(apn_info.ia_apn_info[sim_ready_ind.dual_sys]),&apn_info_item, sizeof(ual_tele_data_apn_info_t));
            SCI_TRACE_LOW("tele radio link_ia_setting_ptr->username = %s, apn = %s", link_ia_setting_ptr->username, link_ia_setting_ptr->apn);
        }

        if (NULL != link_ims_setting_ptr)
        {
            SCI_MEMSET(&apn_info_item, 0, sizeof(ual_tele_data_apn_info_t));
            apn_info_item.auth_type = link_ims_setting_ptr->auth_type;
            apn_info_item.ip_type = link_ims_setting_ptr->ip_type;
            SCI_MEMCPY(apn_info_item.apn, link_ims_setting_ptr->apn, sizeof(uint8) * link_ims_setting_ptr->apn_len);
            SCI_MEMCPY(apn_info_item.password, link_ims_setting_ptr->password, sizeof(uint8) * link_ims_setting_ptr->password_len);
            SCI_MEMCPY(apn_info_item.username, link_ims_setting_ptr->username, sizeof(uint8) * link_ims_setting_ptr->username_len);
            SCI_MEMCPY(&(apn_info.ims_apn_info[sim_ready_ind.dual_sys]),&apn_info_item, sizeof(ual_tele_data_apn_info_t));
            SCI_TRACE_LOW("tele radio link_ims_setting_ptr->username = %s, apn = %s", link_ims_setting_ptr->username, link_ims_setting_ptr->apn);
        }

        if (NULL != link_sos_setting_ptr)
        {
            SCI_MEMSET(&apn_info_item, 0, sizeof(ual_tele_data_apn_info_t));
            apn_info_item.auth_type = link_sos_setting_ptr->auth_type;
            apn_info_item.ip_type = link_sos_setting_ptr->ip_type;
            SCI_MEMCPY(apn_info_item.apn, link_sos_setting_ptr->apn, sizeof(uint8) * link_sos_setting_ptr->apn_len);
            SCI_MEMCPY(apn_info_item.password, link_sos_setting_ptr->password, sizeof(uint8) * link_sos_setting_ptr->password_len);
            SCI_MEMCPY(apn_info_item.username, link_sos_setting_ptr->username, sizeof(uint8) * link_sos_setting_ptr->username_len);
            SCI_MEMCPY(&(apn_info.sos_apn_info[sim_ready_ind.dual_sys]),&apn_info_item, sizeof(ual_tele_data_apn_info_t));
            SCI_TRACE_LOW("tele radio link_sos_setting_ptr->username = %s, apn = %s", link_sos_setting_ptr->username, link_sos_setting_ptr->apn);
        }

        radio_set_apn_ret = ual_tele_radio_set_apn_info(sim_ready_ind.dual_sys, &apn_info);
        SCI_TRACE_LOW("tele radio: set apn_info ret = %d",radio_set_apn_ret);
    }
    //MMIAPICONNECTION_SetPdpContext(sim_ready_ind.dual_sys);
#endif
#endif
#ifdef MMI_NETWORK_NAME_SUPPORT
#ifndef UAL_TELE_SUPPORT
    //get PNN num when SIM ready
    MMIPHONE_GetPNNum(sim_ready_ind.dual_sys);
#endif
#endif

#if defined(MMI_SIM_LOCK_SUPPORT)
    if(MMK_IsOpenWin( PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID)
        && !MMK_IsFocusWin(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
    {
        MMK_HideWindow(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID, FALSE);     
    }
#endif
#ifdef MMI_VOLTE_SUPPORT
    MMIPHONE_SetVolteConfigInfo(sim_ready_ind.dual_sys, s_sim_status[sim_ready_ind.dual_sys].hplmn);
    MMIPHONE_InitVolteInfo(sim_ready_ind.dual_sys);
#endif

    if ( (MMK_IsOpenWin(PHONE_PIN_WAIT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_NEW_PIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        && s_handle_pin_status.cur_handling_sim_id == sim_ready_ind.dual_sys)
    {
        window_id = PHONE_PIN_WAIT_WIN_ID;
        
        if (MMK_IsOpenWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID))//记录下正确的pin码
        {
            GUIEDIT_GetString(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID, &pin_value);
            g_pin_value[sim_ready_ind.dual_sys].blocklen = pin_value.wstr_len;

            MMI_WSTRNTOSTR(
                g_pin_value[sim_ready_ind.dual_sys].blockbuf,
                MN_MAX_BLOCK_LEN,
                pin_value.wstr_ptr,
                g_pin_value[sim_ready_ind.dual_sys].blocklen,
                g_pin_value[sim_ready_ind.dual_sys].blocklen
                );
        }
        else if (MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)) //记录下正确的pin码
        {
            GUIEDIT_GetString(PHONE_PIN_EDITBOX_CTRL_ID, &pin_value);
            g_pin_value[sim_ready_ind.dual_sys].blocklen = pin_value.wstr_len;

            MMI_WSTRNTOSTR(
                g_pin_value[sim_ready_ind.dual_sys].blockbuf,
                MN_MAX_BLOCK_LEN,
                pin_value.wstr_ptr,
                g_pin_value[sim_ready_ind.dual_sys].blocklen,
                g_pin_value[sim_ready_ind.dual_sys].blocklen
                );
        }
        
#ifdef MMI_RESET_PHONE_AUTO_PIN
        //set sim pin to nv
        MMINV_WRITE(MMINV_SIM_PIN_VALUE, g_pin_value);
#endif       

        if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sim_ready_ind.dual_sys])
        {
            g_service_status[sim_ready_ind.dual_sys].is_slecting = TRUE;
        }

        //关闭pin和puk输入窗口
        MMK_CloseWin(PHONE_PIN_ALERT_WIN_ID);
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
    }
    else if (MMK_IsOpenWin(IDLE_SPECIAL_WAIT_WIN_ID))
    {
        window_id = IDLE_SPECIAL_WAIT_WIN_ID;
    }
        
    //add for dual sys
    s_handle_pin_status.need_handle_pin[sim_ready_ind.dual_sys] = FALSE;
    s_handle_pin_status.is_handling_pin[sim_ready_ind.dual_sys] = FALSE;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (s_handle_pin_status.is_handling_pin[i])
        {
            is_handling_pin = TRUE;
            break;
        }
    }
    
    if (!is_handling_pin)
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (i != sim_ready_ind.dual_sys)
            {
                if (s_handle_pin_status.need_handle_pin[i])
                {
                    //set s_handle_pin_status
                    s_handle_pin_status.cur_handling_sim_id = (MN_DUAL_SYS_E)i;
                    s_handle_pin_status.is_handling_pin[i] = TRUE;
                    
                    //create input pin window
                    //CreateInputPinWindow(s_handle_pin_status.sim_pin_ind[i]);
                    return result;
                }
            }
        }
    }
    
    // 如果在输入PIN或者PUK之后，收到该消息
    switch (window_id)
    {
    case PHONE_PIN_WAIT_WIN_ID:
        // 关闭等待窗口        
        if (s_handle_pin_status.cur_handling_sim_id == sim_ready_ind.dual_sys)
        {
            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        }
        break;

    case IDLE_SPECIAL_WAIT_WIN_ID:
        break;
        
    default:
        //关闭pin和puk输入窗口
        if (s_handle_pin_status.cur_handling_sim_id == sim_ready_ind.dual_sys)
        {
            if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sim_ready_ind.dual_sys])
            {
                g_service_status[sim_ready_ind.dual_sys].is_slecting = TRUE;
            }

            MMK_CloseWin(PHONE_PIN_ALERT_WIN_ID);   
            MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
            MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
            MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
            MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
            MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
        }
        break;
    }
    
#if 0 //def MMI_SIM_LOCK_SUPPORT
    //update sim lock data in case,the data is changed in MN layer
    MMIPHONE_UpdateSIMUnlockData();
    //第一次开机,探测到符合条件的卡,打开SP Lock, 提示"SP Lock now on"
    if(MNPHONE_OrangeAutoLockEx(sim_ready_ind.dual_sys))
    {
        MMIPUB_OpenAlertSuccessWin(TXT_SIM_LOCK_SP_LOCK_ON);
    }
#endif    
    
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    //SIM ready,and load local default to system
    MMIAPIPLMN_LoadLocalDefaultConfig(sim_ready_ind.dual_sys);
#endif    
    
    return (result);
}

/*****************************************************************************/
// 	Description : 保存 SIM 状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreSimReadyStatus(
                               APP_MN_SIM_READY_IND_T  *sim_status_ptr
                               )
{
    MN_IMSI_T imsi[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;
    
    dual_sys = sim_status_ptr->dual_sys;
    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"StoreSimReadyStatus:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4715_112_2_18_2_45_9_94,(uint8*)"d", dual_sys);
        return;
    }    
    MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_OK);
    s_sim_status[dual_sys].pin1_is_enabled               = sim_status_ptr->pin1_is_enabled ; 	
    s_sim_status[dual_sys].pin1_is_blocked               = sim_status_ptr->pin1_is_blocked;		
    s_sim_status[dual_sys].pin1_status                   = sim_status_ptr->pin1_status;         
    s_sim_status[dual_sys].pin2_status                   = sim_status_ptr->pin2_status;         
    s_sim_status[dual_sys].unblock_pin1_status           = sim_status_ptr->unblock_pin1_status;	
    s_sim_status[dual_sys].unblock_pin2_Status           = sim_status_ptr->unblock_pin2_Status;	
    s_sim_status[dual_sys].is_pin2_required_acm_max      = sim_status_ptr->is_pin2_required_acm_max; 
    s_sim_status[dual_sys].is_pin2_Required_acm          = sim_status_ptr->is_pin2_Required_acm;	 
    s_sim_status[dual_sys].is_pin2_Required_puct         = sim_status_ptr->is_pin2_Required_puct;	 
    s_sim_status[dual_sys].is_aoc_support         = sim_status_ptr->AOC_is_support;	 
    s_sim_status[dual_sys].imsi                          = sim_status_ptr->imsi;			    
    s_sim_status[dual_sys].hplmn                         = sim_status_ptr->hplmn;
    s_sim_status[dual_sys].location_info                 = sim_status_ptr->location_info;		
    s_sim_status[dual_sys].phase                         = sim_status_ptr->phase;			    
    s_sim_status[dual_sys].sim_service                   = sim_status_ptr->sim_service;		    
    s_sim_status[dual_sys].admin                         = sim_status_ptr->admin;   
    s_sim_status[dual_sys].is_usim                       = sim_status_ptr->is_usim;

    //get imsi from nv
    MMINV_READ(MMINV_PHONE_IMSI,imsi,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(imsi,0,sizeof(imsi));
        MMINV_WRITE(MMINV_PHONE_IMSI,imsi);
    }
    
    //the current sim card is same to the last sim card
    if ((imsi[dual_sys].imsi_len == s_sim_status[dual_sys].imsi.imsi_len) &&
        (0 == memcmp(imsi[dual_sys].imsi_val, s_sim_status[dual_sys].imsi.imsi_val,imsi[dual_sys].imsi_len)))
    {
        s_is_same_sim[dual_sys] = TRUE;
    }
    else
    {
#ifndef SFR_SUPPORT_CTCC_SMS
        // For CTCC Self Register, the imsi will be saved after Registing.
        SCI_MEMSET(&(imsi[dual_sys]), 0x00, sizeof(imsi[dual_sys]));
        SCI_MEMCPY(&(imsi[dual_sys]), &(s_sim_status[dual_sys].imsi), sizeof(imsi[dual_sys]));
        MMINV_WRITE(MMINV_PHONE_IMSI, imsi);
#else
        if(FALSE == MMIAPIPHONE_IsChinaTelecom(dual_sys))
        {
            SCI_MEMSET(&(imsi[dual_sys]), 0x00, sizeof(imsi[dual_sys]));
            SCI_MEMCPY(&(imsi[dual_sys]), &(s_sim_status[dual_sys].imsi), sizeof(imsi[dual_sys]));
            MMINV_WRITE(MMINV_PHONE_IMSI, imsi);
        }
        else
        {
            SCI_TRACE_LOW("[StoreSimReadyStatus] not write nv here");
        }
#endif
        s_is_same_sim[dual_sys] = FALSE;
    }

    //if not the same SIM, clean network name in NV
    if(!s_is_same_sim[dual_sys])
    {
        MMIPHONE_NV_NETWORK_NAME_T* nv_network_name = MMIPHONE_GetNVNetworkNameInfo();
#ifdef MMI_NETWORK_NAME_SUPPORT
        MMIPHONE_NETWORK_NAME_T* network_name = MMIPHONE_GetNetworkNameInfo();
#endif

        nv_network_name[dual_sys].is_opn_exist = FALSE;
        nv_network_name[dual_sys].opn_dcs = 0;
        nv_network_name[dual_sys].opn_len  = 0;
        nv_network_name[dual_sys].is_add_ci = FALSE;
        SCI_MEMSET(nv_network_name[dual_sys].opn, 0x00, MAX_ONSHORTFORM_LEN);
        
        MMINV_WRITE(MMINV_NETWORK_NAME,nv_network_name);
        
    //set network name, store plmn
#ifdef MMI_NETWORK_NAME_SUPPORT
        network_name[dual_sys].opn_dcs = 0;
        network_name[dual_sys].opn_len = 0;
        network_name[dual_sys].is_add_ci = FALSE;
        SCI_MEMSET(network_name[dual_sys].opn, 0x00, MAX_ONSHORTFORM_LEN);
#endif    
#ifdef MMI_RESET_PHONE_AUTO_PIN
        SCI_MEMSET(&g_pin_value[dual_sys], 0x00, sizeof(MN_BLOCK_T));
        //set sim pin to nv
        MMINV_WRITE(MMINV_SIM_PIN_VALUE, g_pin_value);
#endif       
#ifdef MMI_CSP_SUPPORT
        //Reset SIM Related Parameter
        MMIAPISET_ResetSelectLine(dual_sys);
#endif
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
        MMIAPISMS_ResetVMInfo(dual_sys);
#endif
    }
#if 0 //def WIN32
    {
        MN_PLMN_T plmn_list[] = 
        {
            {460, 0, 2},
            {460, 0, 2},
            {460, 0, 2},
            {460, 1, 2},
        };
        
        s_sim_status[dual_sys].hplmn = plmn_list[dual_sys];
    }
#endif
}

#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
/*****************************************************************************/
//  Description :轮询是否有SIM热插拔的callback
//  Global resource dependence :
//  Author:xiaotong.su
//  Note:
/*****************************************************************************/
LOCAL void QuerySimisPlugInCallback(void)
{
    MN_DUAL_SYS_E   e_dual_sys = MN_DUAL_SYS_1;
    MMI_BACKLIGHT_STATUS_E backlight_status = MMI_BACKLIGHT_STATUS_MAX;

    backlight_status = MMIDEFAULT_GetBackLightStateEx();
    SCI_TRACE_LOW("[Hot_Plugin]: QuerySimisPlugInCallback ENTER!backlight_status = %d",backlight_status);

    if( MMI_BACKLIGHT_ON == backlight_status || MMI_BACKLIGHT_HALF_ON == backlight_status)
    {
        SCI_TRACE_LOW("[Hot_Plugin]: screen_is_on backlight_status = %d, s_sim_plug_timer_count = %d",backlight_status,s_sim_plug_timer_count);
        s_sim_plug_timer_count=0;
        MNPHONE_PowerOnSimEx(e_dual_sys, TRUE);
    }
    else
    {
        SCI_TRACE_LOW("[Hot_Plugin]: screen_is_off backlight_status = %d,s_sim_plug_timer_count = %d",backlight_status,s_sim_plug_timer_count);
        s_sim_plug_timer_count++;
        // 60*5 = 300S 是灭屏时的查询热插拔的时间
        if( TIMER_COUNT_LCD_TURN_OFF == s_sim_plug_timer_count)
        {
            SCI_TRACE_LOW("[Hot_Plugin]: screen_is_off, check sim");
            MNPHONE_PowerOnSimEx(e_dual_sys, TRUE);
            s_sim_plug_timer_count=0;
        }
    }

}
/*****************************************************************************/
//  Description : 软件轮询是否热插拔
//  Global resource dependence :
//  Author:xiaotong.su
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_QuerySimisPlugIn(void)
{
    if (0 != s_query_sim_timer_id)
    {
        MMK_StopTimer(s_query_sim_timer_id);
        s_query_sim_timer_id = 0;
    }

    s_query_sim_timer_id = MMK_CreateTimerCallback(MMI_5SECONDS, QuerySimisPlugInCallback, PNULL, TRUE);
}

/*****************************************************************************/
//  Description : send msg of logic sim plug in(out)
//  Global resource dependence :
//  Author:xiaotong.su
//  Note:
/*****************************************************************************/
LOCAL void SendLogicSimPlugMsg(uint16 signal_code)
{
    uint16                signal_size = 0;
    MmiSignalS            *signal = PNULL;

    signal_size = sizeof(MmiSignalS);
    MMI_CREATE_SIGNAL(signal,signal_code,signal_size,SCI_IdentifyThread());
    // Send the signal to the dedicated task's queue
    SCI_TRACE_LOW("[Hot_Plugin]: SendSignalToTask task id=%d",P_APP);
    MMI_SEND_SIGNAL(signal, P_APP);
}
#endif

/*****************************************************************************/
// 	Description : 保存 SIM  not ready 状态
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void StoreSimNotReadyStatus(
                                  APP_MN_SIM_NOT_READY_IND_T  sim_status_ptr
                                  )
{
    MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;
    
    dual_sys = sim_status_ptr.dual_sys;
    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"StoreSimNotReadyStatus:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_4786_112_2_18_2_45_9_95,(uint8*)"d", dual_sys);
        return;
    }
    
    switch (sim_status_ptr.reason)
    {
    case MNSIM_ERROR_NO_SECRETCODE_INITIALIZED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_REJECTED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;
        
    case MNSIM_ERROR_REJECT_BY_NET:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_REGISTRATION_FAILED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;
        
    case MNSIM_ERROR_PIN1_BLOCKED:
    case MNSIM_ERROR_PIN2_BLOCKED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_PIN_BLOCKED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;
        
    case MNSIM_ERROR_PIN1_UNBLOCK_BLOCKED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_PUK_BLOCKED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;
        
    case MNSIM_ERROR_PIN2_UNBLOCK_BLOCKED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_PUK2_BLOCKED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, TRUE);
        break;
//硬件支持热插拔，拔卡时返回消息MNSIM_ERROR_SIM_HOTSWAP_REMOVE
//当硬件不支持热插拔时，走软件逻辑，在MNSIM_ERROR_INSERTED_ERROR消息下做卡被拔出的处理（原本拔卡时返回消息MNSIM_ERROR_REMOVED，这是认为SIM卡有问题了，救卡失败返回的结果）
//不支持热插拔/不支持软件热插拔功能时，MNSIM_ERROR_REMOVED下处理sim卡出现异常的情况
#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
    case MNSIM_ERROR_INSERTED_ERROR: //此消息应该是通用的，当前只有软SIM 热插拔用，暂时放到此宏里
        SCI_TRACE_LOW("[Hot_Plugin]: MNSIM_ERROR_INSERTED_ERROR");

        SCI_TRACE_LOW("[Hot_Plugin]:s_logic_sim_status =%d",s_logic_sim_status);
        if(TRUE == s_logic_sim_status)
        {
            // 卡已拔出
            s_logic_sim_status = FALSE;
            SCI_TRACE_LOW("[Hot_Plugin]: send MSG_PHONE_SIM_LOGIC_PLUG_OUT_IND manual");
            SendLogicSimPlugMsg(MSG_PHONE_SIM_LOGIC_PLUG_OUT_IND);
        }
#else
#ifdef SIM_PLUG_IN_SUPPORT_HARDWARE
    case MNSIM_ERROR_SIM_HOTSWAP_REMOVE: //此消息应该是通用的，当前只有软SIM 热插拔用，暂时放到此宏里

        SCI_TRACE_LOW("[Hot_Plugin]: MNSIM_ERROR_SIM_HOTSWAP_REMOVE");
        SCI_TRACE_LOW("[Hot_Plugin]: GO TO NotifySIMPlugInd MMIPHONE_NOTIFY_SIM_PLUG_OUT!");
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_NO_SIM);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, FALSE);
        NotifySIMPlugInd(dual_sys, MMIPHONE_NOTIFY_SIM_PLUG_OUT);
#endif

    case MNSIM_ERROR_REMOVED:
        SCI_TRACE_LOW("[Hot_Plugin]: MNSIM_ERROR_REMOVED");

#ifdef MCARE_V31_SUPPORT
        McareV31_ExitMcare();
#endif
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_NO_SIM);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, FALSE);
#endif
        break;
#ifdef MMI_SIM_LOCK_SUPPORT
    //case MNSIM_ERROR_SIM_DATA_INTEGRITY_FAIL:  //@20150817
    case MNSIM_ERROR_SIM_NETWORT_LOCKED:
    case MNSIM_ERROR_SIM_NETWORT_SUBSET_LOCKED:
    case MNSIM_ERROR_SIM_SP_LOCKED:
    case MNSIM_ERROR_SIM_CORPORATE_LOCKED:
    case MNSIM_ERROR_SIM_USER_LOCKED:
        MMIPHONE_SetSimStatus(dual_sys, SIM_LOCKED);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, FALSE);
        break;

    case MNSIM_ERROR_SIM_DATA_INTEGRITY_FAIL:  //@20150817
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_OK);
        break;		
#endif 

    default:
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_NO_SIM);
        MMIAPIPHONE_SetSimExistedStatus(dual_sys, FALSE);
        break;
    }
}

/*****************************************************************************/
// 	Description : the current sim card is same to the last sim card
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsSameSim(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    return s_is_same_sim[dual_sys];
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SIM_NOT_READY_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimNotReadyInd(
                                        DPARAM              param
                                        )
{
    APP_MN_SIM_NOT_READY_IND_T   sim_not_ready_ind = *(APP_MN_SIM_NOT_READY_IND_T *)param;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;//MMI_RESULT_TRUE;
#ifndef SUBLCD_SIZE_NONE
    SUB_CONTENT_E       sub_content = SUB_CONTENT_NONE;
#endif
    uint32 i = 0;
    BOOLEAN is_handling_pin = FALSE;
    
    StoreSimNotReadyStatus(sim_not_ready_ind);

#ifdef MMI_LDN_SUPPORT
    if (MMIAPIIDLE_IdleWinIsOpen())
    {
        MMIAPICL_InitCallInfo(sim_not_ready_ind.dual_sys);
    }
    else
    {
        s_ldn_flag |=0x01<<(sim_not_ready_ind.dual_sys);
    }
#endif
    
#if defined(MMI_SIM_LOCK_SUPPORT)
    SCI_TRACE_LOW("mmiphone: sim_not_ready_ind.reason = %d", sim_not_ready_ind.reason);
    switch (sim_not_ready_ind.reason)
    {
    case MNSIM_ERROR_SIM_NETWORT_LOCKED:
    case MNSIM_ERROR_SIM_NETWORT_SUBSET_LOCKED:
    case MNSIM_ERROR_SIM_SP_LOCKED:
    case MNSIM_ERROR_SIM_CORPORATE_LOCKED:
    case MNSIM_ERROR_SIM_USER_LOCKED:
        {
            MMI_STRING_T pin_value ={0};
            
            if (MMK_IsOpenWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID))//记录下正确的pin码
            {
                GUIEDIT_GetString(PHONE_NEW_PIN_AGAIN_EDITBOX_CTRL_ID, &pin_value);
                g_pin_value[sim_not_ready_ind.dual_sys].blocklen = pin_value.wstr_len;

                MMI_WSTRNTOSTR(
                    g_pin_value[sim_not_ready_ind.dual_sys].blockbuf,
                    MN_MAX_BLOCK_LEN,
                    pin_value.wstr_ptr,
                    g_pin_value[sim_not_ready_ind.dual_sys].blocklen,
                    g_pin_value[sim_not_ready_ind.dual_sys].blocklen
                    );
            }
            else if (MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)) //记录下正确的pin码
            {
                GUIEDIT_GetString(PHONE_PIN_EDITBOX_CTRL_ID, &pin_value);
                g_pin_value[sim_not_ready_ind.dual_sys].blocklen = pin_value.wstr_len;

                MMI_WSTRNTOSTR(
                    g_pin_value[sim_not_ready_ind.dual_sys].blockbuf,
                    MN_MAX_BLOCK_LEN,
                    pin_value.wstr_ptr,
                    g_pin_value[sim_not_ready_ind.dual_sys].blocklen,
                    g_pin_value[sim_not_ready_ind.dual_sys].blocklen
                    );
            }
#ifdef MMI_RESET_PHONE_AUTO_PIN
            //set sim pin to nv
            MMINV_WRITE(MMINV_SIM_PIN_VALUE, g_pin_value);
#endif       
            #if 0 //def MMI_MULTI_SIM_SYS_SINGLE  //reserved for Multi SIM
                    BOOLEAN is_need_handle = FALSE;
                    BOOLEAN is_handling_simlock = FALSE;
                    /*set sim_not_ready_ind*/
                    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
                    {
                        /*有其他卡正处在SIM Lock handling 状态，设置当前卡为need handle*/
                        if(( i != sim_not_ready_ind.dual_sys)&&(s_handle_sim_lock_status.is_handling_simlock[i]))
                        {
                            is_need_handle = TRUE;
                            break;            
                        }
                    } 
                    if(is_need_handle)
                    {
                        s_handle_sim_lock_status.need_handle_simlock[sim_not_ready_ind.dual_sys] = TRUE;
                		
                        return result;
                    }
                    else
                    {
                        /*当前卡需要处理SIM Lock,记下状态，进入密码输入界面*/
                        s_handle_sim_lock_status.is_handling_simlock[sim_not_ready_ind.dual_sys] = TRUE;//set the status (lock)
                        s_handle_sim_lock_status.cur_handling_sim_id = sim_not_ready_ind.dual_sys;
                        //create input SIM lock window
                        MMIPHONE_CreateInputSimLockWindow(sim_not_ready_ind);
                        return result;
                    }    
            #else            
                    MMIPHONE_CreateInputSimLockWindow(sim_not_ready_ind);
       
                    if (s_handle_pin_status.is_handling_pin[s_handle_pin_status.cur_handling_sim_id]
                         && s_handle_pin_status.cur_handling_sim_id != sim_not_ready_ind.dual_sys)
                    {
                        MMK_HideWindow(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID, TRUE);                           
                    }
                    else 
                    {
                        MMK_HideWindow(PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID, FALSE);  
                    }               
            #endif
         }
            break;
                
        case MNSIM_ERROR_SIM_DATA_INTEGRITY_FAIL:
            break;
                    
        default:
            break;
        }
 #endif
    if ( (MMK_IsOpenWin(PHONE_PIN_WAIT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PIN_ALERT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_NEW_PIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID)
        || MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
        && s_handle_pin_status.cur_handling_sim_id == sim_not_ready_ind.dual_sys)
    {
        if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sim_not_ready_ind.dual_sys])
        {
            g_service_status[sim_not_ready_ind.dual_sys].is_slecting = TRUE;
        }

        //关闭pin和puk输入窗口
        MMK_CloseWin(PHONE_PIN_ALERT_WIN_ID);
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
    }

    //add for dual sys
    s_handle_pin_status.need_handle_pin[sim_not_ready_ind.dual_sys] = FALSE;
    s_handle_pin_status.is_handling_pin[sim_not_ready_ind.dual_sys] = FALSE;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (s_handle_pin_status.is_handling_pin[i])
        {
            is_handling_pin = TRUE;
            break;
        }
    }
    
    if (!is_handling_pin)
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (i != sim_not_ready_ind.dual_sys)
            {
                if (s_handle_pin_status.need_handle_pin[i])
                {
                    //set s_handle_pin_status
                    s_handle_pin_status.cur_handling_sim_id = (MN_DUAL_SYS_E)i;
                    s_handle_pin_status.is_handling_pin[i] = TRUE;
                    
                    //create input pin window
                    //CreateInputPinWindow(s_handle_pin_status.sim_pin_ind[i]);
                    return result;
                }
            }
        }
    }
#ifndef SUBLCD_SIZE_NONE    
    switch (sim_not_ready_ind.reason)
    {

    case MNSIM_ERROR_NO_SECRETCODE_INITIALIZED:
        // 打开SIM卡被拒界面! 
        sub_content = SUB_CONTENT_SIM_REJECTED;
        break;
        
    case MNSIM_ERROR_REJECT_BY_NET:
        // 打开SIM卡注册失败界面! 
        sub_content = SUB_CONTENT_SIM_REGISTRATION_FAILED;
        break;
        
    case MNSIM_ERROR_NOT_SUPPORT_GSM_ONLY://@zhaohui,cr109170
        MMIPUB_OpenAlertWarningWin(TXT_NOT_SUPPORT_SIM);	
        sub_content = SUB_CONTENT_SIM_REGISTRATION_FAILED;
        break;
        
    case MNSIM_ERROR_PIN1_BLOCKED:
    case MNSIM_ERROR_PIN2_BLOCKED:
        //jassmine cr14034 PIN被锁，不用处理此消息
        break;
        
    case MNSIM_ERROR_PIN1_UNBLOCK_BLOCKED:
        sub_content = SUB_CONTENT_PUK_BLOCKED;
        break;
        
    case MNSIM_ERROR_PIN2_UNBLOCK_BLOCKED:
        sub_content = SUB_CONTENT_PUK2_BLOCKED;
        break;
        
    case MNSIM_ERROR_REMOVED:
        sub_content = SUB_CONTENT_NO_SIM;
        break;
        
    default:
        sub_content = SUB_CONTENT_NO_SIM;
        break;
    }
#endif    
    MAIN_SetIdleNetWorkName();    
    
    //关闭pin和puk输入窗口
    if (s_handle_pin_status.cur_handling_sim_id == sim_not_ready_ind.dual_sys)
    {
        if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sim_not_ready_ind.dual_sys])
        {
            g_service_status[sim_not_ready_ind.dual_sys].is_slecting = TRUE;
        }

        MMK_CloseWin(PHONE_PIN_ALERT_WIN_ID);
        MMK_CloseWin(PHONE_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_NEW_PIN_AGAIN_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_PUK_INPUT_WIN_ID);
        MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    }
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏,显示相应的Sim消息
    if(sub_content != SUB_CONTENT_NONE &&  SUB_CONTENT_NO_SIM != sub_content )
    {
        MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

        //有任意一张sim卡是NO SIM 的状态就直接return !!!
        for(dual_sys = MN_DUAL_SYS_1;dual_sys < MMI_DUAL_SYS_MAX;dual_sys++)
        {
            if(SIM_STATUS_NO_SIM !=MMIAPIPHONE_GetSimStatus(dual_sys))
            {
                return (result);
            }
        }
        
        //刷新小屏,显示相应的Sim消息
        MMISUB_IsPermitUpdate(TRUE);  
        MMISUB_SetSubLcdDisplay(TRUE,TRUE,sub_content,PNULL);
    }
#endif   
    if (MNSIM_ERROR_REMOVED == sim_not_ready_ind.reason)
    {
        MMIAPIPB_AbortPB();
		//bug 184713
#ifndef SIM_PLUG_IN_SUPPORT
        MMIAPICC_ExitMoCall(sim_not_ready_ind.dual_sys);

        //if (MMIAPIPB_IsExportContactOpc())
        {
            MMIPUB_OpenAlertFailWin(TXT_SIM_PLUG_OUT); // 支持热插拔时，会在NotifySIMPlugInd中处理弹窗，这里防止二次弹窗
        }
#endif
        MMIAPIPB_ResortPB();

        //add code by wangwp when hot plug out the sim card
#ifdef SIM_PLUG_IN_SUPPORT //bug878920
        MMIAPIPHONE_PowerOffPs(sim_not_ready_ind.dual_sys); //Power Off PS
        SCI_TRACE_LOW("HandleSimNotReadyInd--hot plug out the sim card--dual_sys=%d", sim_not_ready_ind.dual_sys);		
#endif
    }
    return (result);
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_SCELL_RSSI_IND
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/

//add by song 2022.9.6 增加缓冲减少信号跳变 
#define  RSSI_BUF_SIZE   5
static uint8 s_rssi_arry[RSSI_BUF_SIZE] = {0};
static uint8 s_rssi_idx = 0;
static uint8 GetRssiArry(void)
{
    uint16 i = 0;
    uint8 max_rssi = 0;
    for(i = 0; i < RSSI_BUF_SIZE; i++)
    {
        if(s_rssi_arry[i] > max_rssi)
        {
            max_rssi = s_rssi_arry[i];
        }
    }
    return max_rssi;
}

static BOOLEAN SetRssiArry(uint32 val)
{
    s_rssi_arry[s_rssi_idx++] = val;
    if(s_rssi_idx >= RSSI_BUF_SIZE)
    {
        s_rssi_idx = 0;
    }
    return TRUE;
}
//add by song end

LOCAL MMI_RESULT_E HandleScellRssiInd(
                                      DPARAM              param
                                      )
{
    enum{rxlev_G,rxlev_W,rxlev_L,rxlev_U};
    APP_MN_SCELL_RSSI_IND_T    srri_ind = *(APP_MN_SCELL_RSSI_IND_T *)param;
    MMI_RESULT_E		result = MMI_RESULT_TRUE;
    uint8				rxLevel = 0;
    MN_DUAL_SYS_E		dual_sys = MN_DUAL_SYS_1;
    BOOLEAN is_direct_draw = FALSE;
    uint8 rxlevnindex = 0;
    MMI_GMMREG_RAT_E	network_status_rat = MMI_GMMREG_RAT_GPRS;
    const int16 g_service_rxlev_map[][PHONE_RX_SIG_DEFAULT_LEVEL] =
    {
        {59-10, 51-10, 42-10, 38-10, 33-10 },	//LTE -156 db  -97 -105 -114 -118 -123
        {59-10, 51-10, 42-10, 38-10, 33-10 },	//LTE -156 db  -97 -105 -114 -118 -123
        {59-10, 51-10, 42-10, 38-10, 33-10 },	//LTE -156 db  -97 -105 -114 -118 -123
        {59-10, 51-10, 42-10, 38-10, 33-10 },	//LTE -156 db  -97 -105 -114 -118 -123
    };

    dual_sys = srri_ind.dual_sys;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleScellRssiInd:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5110_112_2_18_2_45_10_96,(uint8*)"d", dual_sys);
        return MMI_RESULT_FALSE;
    }

    //设置全局变量
    if (g_service_status[dual_sys].is_first_rssi
        && 0 == g_service_status[dual_sys].rssiLevel
        && 0 < srri_ind.rxlev)//第一次收到非0的rssi直接显示
    {
        g_service_status[dual_sys].is_first_rssi = FALSE;
        is_direct_draw = TRUE;
    }
#ifdef ZDT_NET_SUPPORT
    MMIZDT_Net_RssiInd(dual_sys,srri_ind.rxlev);
#endif
#ifdef WLDT_NET_SUPPORT
    MMIWLDT_Net_RssiInd(dual_sys,srri_ind.rxlev);
#endif

    g_service_status[dual_sys].rssiLevel = srri_ind.rxlev;

    network_status_rat = MMIAPIPHONE_GetTDOrGsm(dual_sys);
    switch (network_status_rat)
    {
        case MMI_GMMREG_RAT_GPRS:
            rxlevnindex = rxlev_G;
            break;
        case MMI_GMMREG_RAT_3G:
            rxlevnindex = rxlev_W;
            break;
        case MMI_GMMREG_RAT_LTE:
            rxlevnindex = rxlev_L;
            break;
        case MMI_GMMREG_RAT_UNKNOWN:
        default:
            rxlevnindex = rxlev_U;
            break;
    }

    SCI_TRACE_LOW("[%s] dual_sys[%d] rxlev[%d] network_status_rat[%d]",__FUNCTION__,dual_sys,srri_ind.rxlev,network_status_rat);

    //设置信强度,根据参考手机sagem得到的数据，将idle界面下的信号强度划分成5级
    if (g_service_rxlev_map[rxlevnindex][PHONE_RX_SIG_DEFAULT_LEVEL-5] <= srri_ind.rxlev)
    {
        rxLevel = PHONE_RX_SIG_DEFAULT_LEVEL;
    }
    else if (g_service_rxlev_map[rxlevnindex][PHONE_RX_SIG_DEFAULT_LEVEL-4] <= srri_ind.rxlev)
    {
        rxLevel = PHONE_RX_SIG_DEFAULT_LEVEL - 1;
    }
    else if (g_service_rxlev_map[rxlevnindex][PHONE_RX_SIG_DEFAULT_LEVEL-3] <= srri_ind.rxlev)
    {
        rxLevel = 3;
    }
    else if (g_service_rxlev_map[rxlevnindex][PHONE_RX_SIG_DEFAULT_LEVEL-2] <= srri_ind.rxlev)
    {
        rxLevel = 2;
    }
    else if (g_service_rxlev_map[rxlevnindex][PHONE_RX_SIG_DEFAULT_LEVEL-1] <= srri_ind.rxlev)
    {
        rxLevel = 1;
    }
    else
    {
        rxLevel = 0;
    }
    
//add by song 2022.9.6 增加缓冲减少信号跳变    
    SetRssiArry(rxLevel);
    rxLevel = GetRssiArry();
//add by song end

    MAIN_SetIdleRxLevel(dual_sys, rxLevel, is_direct_draw);

    NotifyRxLevelChange(dual_sys, rxLevel);
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetSignalStrength(rxLevel); //mdy by qing.yu@for cr
#endif
    return (result);
}

#define  SHUTDOWN_Y_SET  30 
#define  SOS_Y_SET  10 

#include "watch_launcher_common.h"
#ifdef ADULT_WATCH_SUPPORT
const GUI_RECT_T ADULT_SHUTDOWN_EMERG_ICON_RECT = DP2PX_RECT(56, 24, 56+32, 24+48);
const GUI_RECT_T ADULT_SHUTDOWN_EMERG_TEXT_RECT = DP2PX_RECT(88, 24, 88+120, 24+48);
const GUI_RECT_T ADULT_SHUTDOWN_RESET_BTN_RECT = DP2PX_RECT(48, 96, 48+48+2, 96+48);
const GUI_RECT_T ADULT_SHUTDOWN_OFF_BTN_RECT = DP2PX_RECT(144, 96, 144+48+2, 96+48);
const GUI_RECT_T ADULT_SHUTDOWN_CANCEL_TEXT_RECT = DP2PX_RECT(36, 168, 36+168, 168+48);
#endif
#ifdef SCREEN_SHAPE_CIRCULAR
const GUI_RECT_T SHUTDOWN_OFF_BG_RECT = DP2PX_RECT(40, 30+SHUTDOWN_Y_SET, 40+160, 30+60+SHUTDOWN_Y_SET);
const GUI_RECT_T SHUTDOWN_OFF_BG_TEXT_RECT = DP2PX_RECT(96, 50+SHUTDOWN_Y_SET, 96+88, 70+SHUTDOWN_Y_SET);
const GUI_RECT_T SHUTDOWN_OFF_BTN_RECT = DP2PX_RECT(56, 42+SHUTDOWN_Y_SET, 92, 78+SHUTDOWN_Y_SET);
const GUI_RECT_T SHUTDOWN_RESET_BG_RECT = DP2PX_RECT(40, 30+60, 40+160, 30+60+60);
const GUI_RECT_T SHUTDOWN_RESET_BG_TEXT_RECT = DP2PX_RECT(96, 110, 96+88, 130);
const GUI_RECT_T SHUTDOWN_RESET_BTN_RECT = DP2PX_RECT(56, 102, 92, 138);
const GUI_RECT_T SHUTDOWN_FLYMODE_BG_RECT = DP2PX_RECT(40, 30+60+60, 40+160, 240-30);
const GUI_RECT_T SHUTDOWN_FLYMODE_BG_TEXT_RECT = DP2PX_RECT(96, 170, 96+88, 190);
const GUI_RECT_T SHUTDOWN_FLYMODE_BTN_RECT = DP2PX_RECT(56, 162, 92, 198);

const GUI_RECT_T SHUTDOWN_SOS_BG_RECT = DP2PX_RECT(40, 30+60+60+SOS_Y_SET, 40+160, 240-30+SOS_Y_SET);
const GUI_RECT_T SHUTDOWN_SOS_BG_TEXT_RECT = DP2PX_RECT(116, 170+SOS_Y_SET, 96+88, 190+SOS_Y_SET);
const GUI_RECT_T SHUTDOWN_SOS_BTN_RECT = DP2PX_RECT(40, 162+SOS_Y_SET, 92, 198+SOS_Y_SET);
#else
const GUI_RECT_T SHUTDOWN_OFF_BG_RECT = {28, 36+SHUTDOWN_Y_SET, 28+184, 36+64+SHUTDOWN_Y_SET};
const GUI_RECT_T SHUTDOWN_OFF_BG_TEXT_RECT = {40, 36+SHUTDOWN_Y_SET, 40+184, 36+64+SHUTDOWN_Y_SET};
const GUI_RECT_T SHUTDOWN_OFF_BTN_RECT = {28, 36+SHUTDOWN_Y_SET, 28+64+2, 36+64+SHUTDOWN_Y_SET};
const GUI_RECT_T SHUTDOWN_RESET_BTN_RECT = {28, 140, 28+64+2, 140+64};
const GUI_RECT_T SHUTDOWN_FLYMODE_BTN_RECT = {148, 140, 148+64+2, 140+64};
#endif
LOCAL void SetFlyMode(void)
{
      uint32 i  = 0;
      BOOLEAN is_fly_mode_on = MMIAPISET_GetFlyMode();
#ifdef SCREEN_SHAPE_CIRCULAR
      GUI_RECT_T ctrl_rect = {0};
      GUI_FONT_T font = SONG_FONT_20;
      MMI_STRING_T text = {0};

      MMI_GetLabelTextByLang(TXT_SHUTDOWN_FLYMODE, &text);
#endif
      SCI_TraceLow("setFlyMode in long powerkey");
      if (!is_fly_mode_on)
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
                  SCI_TraceLow("set flymode: is_ps_ready(%d)", dual_sys);
                  if (!is_ps_ready)
                  {
                      break;
                  }
              }
          }
          if (is_ps_ready)
          {
              is_fly_mode_on = TRUE;
              MMIAPISET_SetFlyMode(is_fly_mode_on);
              ual_tele_radio_set_fly_mode(is_fly_mode_on);
              MMIAPIPHONE_DeactivePsAccordingToSetting();
#ifdef SCREEN_SHAPE_CIRCULAR
              ctrl_rect = SHUTDOWN_FLYMODE_BTN_RECT;
              GUILABEL_SetRect(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, &ctrl_rect, TRUE);
              GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_BG_CTRL_ID, res_sys_bg_flymode_on);

              ctrl_rect = SHUTDOWN_FLYMODE_BG_TEXT_RECT;
              GUILABEL_SetRect(WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID, &ctrl_rect, TRUE);
              GUILABEL_SetText(WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID, &text, TRUE);
              GUILABEL_SetFont(WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID, font, MMI_WHITE_COLOR);
#endif
              GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, res_sys_btn_fly);
              MMK_SendMsg(WATCH_SHUTDOWN_WIN_ID, MSG_FULL_PAINT, PNULL);
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
              is_fly_mode_on = FALSE;
              MMIAPISET_SetFlyMode(is_fly_mode_on);
              ual_tele_radio_set_fly_mode(is_fly_mode_on);
              // 激活协议栈
              MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
#ifdef SCREEN_SHAPE_CIRCULAR
              ctrl_rect = SHUTDOWN_FLYMODE_BTN_RECT;
              GUILABEL_SetRect(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, &ctrl_rect, TRUE);
              GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_BG_CTRL_ID, res_sys_bg_flymode_off);

              ctrl_rect = SHUTDOWN_FLYMODE_BG_TEXT_RECT;
              GUILABEL_SetRect(WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID, &ctrl_rect, TRUE);
              GUILABEL_SetText(WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID, &text, TRUE);
              GUILABEL_SetFont(WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID, font, MMI_WHITE_COLOR);
#endif
              GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, res_sys_btn_fly_off);
              MMK_SendMsg(WATCH_SHUTDOWN_WIN_ID, MSG_FULL_PAINT, PNULL);
          }
          else
          {
              SCI_TRACE_LOW("SetFlyMode close fail");
          }
      }
}

LOCAL void HandleNotifyPenokMsg(MMI_NOTIFY_T notify)
{
    if (notify.src_id == WATCH_SHUTDOWN_BG_CTRL_ID
        || notify.src_id == WATCH_SHUTDOWN_OFF_BTN_CTRL_ID
        || notify.src_id == WATCH_SHUTDOWN_OFF_TIP_CTRL_ID)
    {
        MMIPHONE_PowerOff();
    }
#ifdef SCREEN_SHAPE_CIRCULAR
    else if (notify.src_id == WATCH_SHUTDOWN_RESET_BG_CTRL_ID
        || notify.src_id == WATCH_SHUTDOWN_RESET_CTRL_ID
        || notify.src_id == WATCH_SHUTDOWN_RESET_TIP_CTRL_ID)
    {
       MMIPHONE_PowerOff();// MMIAPIPHONE_PowerReset();
    }
    else if (notify.src_id == WATCH_SHUTDOWN_FLYMODE_BG_CTRL_ID
        || notify.src_id == WATCH_SHUTDOWN_FLYMODE_CTRL_ID
        || notify.src_id == WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID)
    {
        SetFlyMode();
    }
#else
    else if (notify.src_id == WATCH_SHUTDOWN_RESET_CTRL_ID)
    {
       MMIPHONE_PowerOff();// MMIAPIPHONE_PowerReset();
    }
    else if (notify.src_id == WATCH_SHUTDOWN_FLYMODE_CTRL_ID)
    {
        SetFlyMode();
    }
#endif
    else if (notify.src_id == WATCH_SHUTDOWN_SOS_BG_CTRL_ID
        || notify.src_id == WATCH_SHUTDOWN_SOS_CTRL_ID
        || notify.src_id == WATCH_SHUTDOWN_SOS_TIP_CTRL_ID)
    {
        if(MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1) != SIM_STATUS_OK)
        {
            // MMIZDT_OpenNoSimOrDataWin();
            MMIPUB_OpenAlertWarningWin(TXT_NO_SIM_OR_DATA);
        }
        else
        {
            MMK_CloseWin(WATCH_SHUTDOWN_WIN_ID);
            MMIZDT_Call_SOS();
        }
    }
}

#ifdef ADULT_WATCH_SUPPORT
LOCAL MMI_RESULT_E HandleShutDownWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CheckAllocatedMemInfo();
    SCI_TRACE_LOW("HandleShutDownWinMsg: Adult: msg_id=0x%x", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T ctrl_rect = {0};
            MMI_STRING_T text = {0};

            //emergency call icon
            ctrl_rect = ADULT_SHUTDOWN_EMERG_ICON_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(ADULT_WATCH_SHUTDOWN_EMERG_ICON_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetIcon(ADULT_WATCH_SHUTDOWN_EMERG_ICON_CTRL_ID, res_sys_emergency_call);
            //emergency call text
            MMI_GetLabelTextByLang(TXT_EMERGENCY_CALL, &text);
            ctrl_rect = ADULT_SHUTDOWN_EMERG_TEXT_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(ADULT_WATCH_SHUTDOWN_EMERG_TXT_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetText(ADULT_WATCH_SHUTDOWN_EMERG_TXT_CTRL_ID, &text, TRUE);
            GUILABEL_SetFont(ADULT_WATCH_SHUTDOWN_EMERG_TXT_CTRL_ID, DP_FONT_26, MMI_WHITE_COLOR);

            //reset
            ctrl_rect = ADULT_SHUTDOWN_RESET_BTN_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(ADULT_WATCH_SHUTDOWN_RESET_BTN_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetIcon(ADULT_WATCH_SHUTDOWN_RESET_BTN_CTRL_ID, res_sys_btn_restart);

            //off
            ctrl_rect = ADULT_SHUTDOWN_OFF_BTN_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(ADULT_WATCH_SHUTDOWN_OFF_BTN_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetIcon(ADULT_WATCH_SHUTDOWN_OFF_BTN_CTRL_ID, res_sys_btn_shutdown);

            //cancel
            MMI_GetLabelTextByLang(STXT_CANCEL, &text);
            ctrl_rect = ADULT_SHUTDOWN_CANCEL_TEXT_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(ADULT_WATCH_SHUTDOWN_CANCEL_TXT_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetText(ADULT_WATCH_SHUTDOWN_CANCEL_TXT_CTRL_ID, &text, TRUE);
            GUILABEL_SetFont(ADULT_WATCH_SHUTDOWN_CANCEL_TXT_CTRL_ID, DP_FONT_26, MMI_WHITE_COLOR);

            break;
        }

        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = DP2PX_RECT(0, 0, 239, 239);
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            GUI_FillRect(&lcd_dev_info, win_rect, LAUNCHER_BACKGROUND_COLOR);
            break;
        }

        case MSG_NOTIFY_PENOK:
        {
            MMI_NOTIFY_T notify = *(MMI_NOTIFY_T*)param;
            if(notify.src_id == ADULT_WATCH_SHUTDOWN_EMERG_ICON_CTRL_ID
                || notify.src_id == ADULT_WATCH_SHUTDOWN_EMERG_TXT_CTRL_ID)
            {
                //emergency call
                if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
                {
                    WatchAPICC_ReleaseCurrentCall();
                }
                WatchAPICC_DialNumber((uint8*)"112", 3);
            }
            else if(notify.src_id == ADULT_WATCH_SHUTDOWN_RESET_BTN_CTRL_ID)
            {
                MMIAPIPHONE_PowerReset();//reset
            }
            else if(notify.src_id == ADULT_WATCH_SHUTDOWN_OFF_BTN_CTRL_ID)
            {
                MMIPHONE_PowerOff();//off
            }
            else if(notify.src_id == ADULT_WATCH_SHUTDOWN_CANCEL_TXT_CTRL_ID)
            {
                MMK_CloseWin(win_id);//cancel
            }
            break;
        }

        case MSG_APP_UP:
            WatchAPICC_DialNumber((uint8*)"112", 3);
            break;
        case MSG_APP_DOWN:
            MMK_CloseWin(win_id);//cancel
            break;

        case MSG_APP_RIGHT:
            MMIPHONE_PowerOff();//off
            break;
        case MSG_APP_LEFT:
            MMIAPIPHONE_PowerReset();//reset
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    return recode;
}

WINDOW_TABLE(WATCH_SHUTDOWN_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleShutDownWinMsg ),
    WIN_ID(WATCH_SHUTDOWN_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, ADULT_WATCH_SHUTDOWN_EMERG_ICON_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, ADULT_WATCH_SHUTDOWN_EMERG_TXT_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, ADULT_WATCH_SHUTDOWN_RESET_BTN_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, ADULT_WATCH_SHUTDOWN_OFF_BTN_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, ADULT_WATCH_SHUTDOWN_CANCEL_TXT_CTRL_ID),
    END_WIN
};
#else
LOCAL MMI_RESULT_E HandleShutDownWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_FONT_T   font = SONG_FONT_20;
#else
    GUI_FONT_T   font = SONG_FONT_24;
#endif
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T win_rect = {0};
            GUI_RECT_T ctrl_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMI_STRING_T text = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

            MMI_GetLabelTextByLang(TXT_SHUT_DOWN, &text);

            ctrl_rect = SHUTDOWN_OFF_BG_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(WATCH_SHUTDOWN_BG_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetIcon(WATCH_SHUTDOWN_BG_CTRL_ID, res_sys_bg_shutdown_off);

            ctrl_rect = SHUTDOWN_OFF_BG_TEXT_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(WATCH_SHUTDOWN_OFF_TIP_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetText(WATCH_SHUTDOWN_OFF_TIP_CTRL_ID, &text, TRUE);
            GUILABEL_SetFont(WATCH_SHUTDOWN_OFF_TIP_CTRL_ID, font, MMI_WHITE_COLOR);

            ctrl_rect = SHUTDOWN_OFF_BTN_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(WATCH_SHUTDOWN_OFF_BTN_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetIcon(WATCH_SHUTDOWN_OFF_BTN_CTRL_ID, res_sys_btn_shutdown);
#ifdef SCREEN_SHAPE_CIRCULAR
           /* MMI_GetLabelTextByLang(TXT_SHUTDOWN_RESET, &text);
            ctrl_rect = SHUTDOWN_RESET_BG_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(WATCH_SHUTDOWN_RESET_BG_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetIcon(WATCH_SHUTDOWN_RESET_BG_CTRL_ID, res_sys_bg_restart);

            ctrl_rect = SHUTDOWN_RESET_BG_TEXT_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(WATCH_SHUTDOWN_RESET_TIP_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetText(WATCH_SHUTDOWN_RESET_TIP_CTRL_ID, &text, TRUE);
            GUILABEL_SetFont(WATCH_SHUTDOWN_RESET_TIP_CTRL_ID, font, MMI_WHITE_COLOR);*/
#endif
           /* ctrl_rect = SHUTDOWN_RESET_BTN_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(WATCH_SHUTDOWN_RESET_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetIcon(WATCH_SHUTDOWN_RESET_CTRL_ID, res_sys_btn_restart);*/
//#ifdef SCREEN_SHAPE_CIRCULAR
//            MMI_GetLabelTextByLang(TXT_SHUTDOWN_FLYMODE, &text);
//            ctrl_rect = SHUTDOWN_FLYMODE_BG_RECT;
//            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
//            GUILABEL_SetRect(WATCH_SHUTDOWN_FLYMODE_BG_CTRL_ID, &ctrl_rect, TRUE);
//
//            ctrl_rect = SHUTDOWN_FLYMODE_BG_TEXT_RECT;
//            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
//            GUILABEL_SetRect(WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID, &ctrl_rect, TRUE);
//            GUILABEL_SetText(WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID, &text, TRUE);
//            GUILABEL_SetFont(WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID, font, MMI_WHITE_COLOR);
//#endif
//            ctrl_rect = SHUTDOWN_FLYMODE_BTN_RECT;
//            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
//            GUILABEL_SetRect(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, &ctrl_rect, TRUE);
//            if(TRUE == MMIAPISET_GetFlyMode())
//            {
//#ifdef SCREEN_SHAPE_CIRCULAR
//                GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_BG_CTRL_ID, res_sys_bg_flymode_on);
//#endif
//                GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, res_sys_btn_fly);
//            }
//            else
//            {
//#ifdef SCREEN_SHAPE_CIRCULAR
//                GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_BG_CTRL_ID, res_sys_bg_flymode_off);
//#endif
//                GUILABEL_SetIcon(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, res_sys_btn_fly_off);
//            }
//            MMK_SetActiveCtrl(WATCH_SHUTDOWN_FLYMODE_CTRL_ID, FALSE);

            MMI_GetLabelTextByLang(TXT_SOS, &text);
            ctrl_rect = SHUTDOWN_SOS_BG_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(WATCH_SHUTDOWN_SOS_BG_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetIcon(WATCH_SHUTDOWN_SOS_BG_CTRL_ID, res_sys_bg_sos);

            ctrl_rect = SHUTDOWN_SOS_BG_TEXT_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(WATCH_SHUTDOWN_SOS_TIP_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetText(WATCH_SHUTDOWN_SOS_TIP_CTRL_ID, &text, TRUE);
            GUILABEL_SetFont(WATCH_SHUTDOWN_SOS_TIP_CTRL_ID, font, MMI_WHITE_COLOR);

            ctrl_rect = SHUTDOWN_SOS_BTN_RECT;
            ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, ctrl_rect);
            GUILABEL_SetRect(WATCH_SHUTDOWN_SOS_CTRL_ID, &ctrl_rect, TRUE);
            GUILABEL_SetIcon(WATCH_SHUTDOWN_SOS_CTRL_ID, res_sys_btn_sos);

            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            break;
        }

        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            GUI_FillRect(&lcd_dev_info, win_rect, LAUNCHER_BACKGROUND_COLOR);
            break;
        }

        case MSG_NOTIFY_PENOK:
        {
            MMI_NOTIFY_T notify = *(MMI_NOTIFY_T*)param;
            HandleNotifyPenokMsg(notify);
            break;
        }

        case MSG_NOTIFY_OK:
        case MSG_APP_OK:
        {
            MMIPHONE_PowerOff();//MMIAPIPHONE_PowerReset();
            break;
        }

        case MSG_NOTIFY_MIDSK:
        case MSG_APP_WEB:
        {
            MMIPHONE_PowerOff();
            break;
        }

        case MSG_APP_UP:
        case MSG_APP_DOWN:
        case MSG_APP_LEFT:
        case MSG_APP_RIGHT:
        {
            break;
        }

        case MSG_NOTIFY_CANCEL:
        case MSG_APP_CANCEL:
        {
           // SetFlyMode();
            break;
        }

        case MSG_APP_RED:
            MMK_CloseWin(WATCH_SHUTDOWN_WIN_ID);//bug2141328
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    return recode;
}

WINDOW_TABLE(WATCH_SHUTDOWN_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleShutDownWinMsg ),
    WIN_ID(WATCH_SHUTDOWN_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, WATCH_SHUTDOWN_BG_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, WATCH_SHUTDOWN_OFF_BTN_CTRL_ID),
#ifdef SCREEN_SHAPE_CIRCULAR
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, WATCH_SHUTDOWN_OFF_TIP_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, WATCH_SHUTDOWN_RESET_BG_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, WATCH_SHUTDOWN_RESET_TIP_CTRL_ID),
//    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, WATCH_SHUTDOWN_FLYMODE_BG_CTRL_ID),
//    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, WATCH_SHUTDOWN_FLYMODE_TIP_CTRL_ID),
#else
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, WATCH_SHUTDOWN_OFF_TIP_CTRL_ID),
#endif
    //CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, WATCH_SHUTDOWN_RESET_CTRL_ID),
//    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, WATCH_SHUTDOWN_FLYMODE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, WATCH_SHUTDOWN_SOS_BG_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, WATCH_SHUTDOWN_SOS_TIP_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, WATCH_SHUTDOWN_SOS_CTRL_ID),
    END_WIN
};
#endif

PUBLIC void WatchCHARGE_ShutDownQuery_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_SHUTDOWN_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_SHUTDOWN_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}


#ifdef ZDT_GSENSOR_SUPPORT
extern void ZDT_SaveCurStepsBeforePowerOff();
#endif

/*****************************************************************************/
// 	Description : power off and query
//	Global resource dependence : 
//  Author: Jassmine
//	Note:sometimes we should query and confirm
/*****************************************************************************/
void MMIAPIPHONE_PowerOff(void)
{
#ifdef ZDT_GSENSOR_SUPPORT
    ZDT_SaveCurStepsBeforePowerOff();
    SCI_Sleep(10);
#endif
#if 0
#ifdef DL_SUPPORT
    if (MMIAPIDL_GetDownloadStatus())
    {
        MMIPHONE_OpenQueryPowerOff(MMIPHONE_QUERY_POWER_OFF_DOWNLOAD);
    }
    else
#endif
    {
        MMIPHONE_PowerOff();
    }
#else
    WatchCHARGE_ShutDownQuery_Enter();
#endif
}

/*****************************************************************************/
// 	Description : power off directly
//	Global resource dependence :
//  Author: Jassmine
//	Note: sometimes we should poweroff directly
/*****************************************************************************/
void MMIAPIPHONE_PowerOffEx(void)
{
    MMIPHONE_PowerOff();
}

/*****************************************************************************/
// 	Description : power off MS
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
#ifdef ZDT_PLAT_SHB_SUPPORT
BOOLEAN g_is_shb_power_off = FALSE;
static uint8    s_shb_power_off_timer_id = 0;             // 关机界面持续时间的ID号
LOCAL void MMIPHONE_SHB_PowerOff_Handle(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MMIPHONE_PowerOff();
}
#endif
#ifdef ZDT_PLAT_YX_SUPPORT
extern uint8        g_low_shudown_battery_cap;
BOOLEAN g_is_yx_power_off = FALSE;
static uint8    s_yx_power_off_timer_id = 0;             // 关机界面持续时间的ID号
LOCAL void MMIPHONE_YX_PowerOff_Handle(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MMIPHONE_PowerOff();
}
#endif
void MMIPHONE_PowerOff(void)
{
    uint32 i=0;
    POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
#ifdef ENG_SUPPORT
    uint32 tick = 0;
#endif
    //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOff:restart_condition=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5199_112_2_18_2_45_10_97,(uint8*)"d", restart_condition);

    //added by bao 正常关机时需要开机铃声
    MMIZDT_NV_SetFirstPWON(1);
#ifdef ZDT_PLAT_SHB_SUPPORT
    if(SHB_Net_IsInit() && SHB_Net_IsAllow())
    {
        if(g_is_shb_power_off == FALSE)
        {
            g_is_shb_power_off = TRUE;
            SHB_HTTP_ZTE1001_Send(2);
            if (s_shb_power_off_timer_id != 0)
            {
                MMK_StopTimer(s_shb_power_off_timer_id);
                s_shb_power_off_timer_id = 0;
            }
            s_shb_power_off_timer_id = MMK_CreateTimerCallback(4000, 
                                                                        MMIPHONE_SHB_PowerOff_Handle, 
                                                                        PNULL,
                                                                        FALSE);
            return;
        }
    }
#endif

#if defined(ZDT_PLAT_YX_SUPPORT_LZ) || defined(ZDT_PLAT_YX_SUPPORT_CY)
    if(YX_Net_IsInit() && YX_Net_Is_Land())
    {
        if(g_is_yx_power_off == FALSE)
        {
            g_is_yx_power_off = TRUE;
            if(g_low_shudown_battery_cap)
            {
	            YX_Net_Send_ALARM_POWER(&g_yx_app,3);
            }

            YX_Net_Send_ALARM_POWER(&g_yx_app,2);
            if (s_yx_power_off_timer_id != 0)
            {
                MMK_StopTimer(s_yx_power_off_timer_id);
                s_yx_power_off_timer_id = 0;
            }
            s_yx_power_off_timer_id = MMK_CreateTimerCallback(2000, 
                                                                        MMIPHONE_YX_PowerOff_Handle, 
                                                                        PNULL,
                                                                        FALSE);
            return;
        }
    }
#endif

#if defined(MMI_SIM_LOCK_SUPPORT)    
        /*在倒计时界面关机时需要记住还剩下的时间*/
    if(MMK_IsOpenWin( PHONE_SIM_LOCK_COUNTDOWN_WIN_ID))
    {
        MMI_OPERATE_SIM_LOCK_T sim_lock_operate = {0};
        uint32 trials_counter = 0;
        uint32 time_out_value = 0;

        //SCI_TRACE_LOW:"MMIPHONE_PowerOff PHONE_SIM_LOCK_COUNTDOWN_WIN_ID open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5209_112_2_18_2_45_10_98,(uint8*)"");
        sim_lock_operate.lock_type = s_sim_lock_operate_value.lock_type;
        MMIAPIPHONE_GetSIMLockRemainTrialsTimer(sim_lock_operate.lock_type,&trials_counter, &time_out_value);
        time_out_value = MMIPHONE_GetCurCountdownTime();
        MNPHONE_UpdateSIMUnlockRetryData(sim_lock_operate.lock_type, trials_counter, time_out_value);
        MMIAPIPHONE_UpdateSIMUnlockData();
    }
#endif    
    
    if(MMIAPIMTV_IsMTVRunning())
    {
        //SCI_TRACE_LOW:"MMIPHONE_PowerOff MMIAPIMTV_IsMTVRunning delay poweroff"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5220_112_2_18_2_45_10_99,(uint8*)"");
        MMIAPIMTV_SetDelayPowerOff(TRUE);
        MMIAPIMTV_Exit();
        return;
    }
    //关闭fm
    MMIAPIFM_Exit();
    
#ifdef MMI_CSTAR_UDB_SUPPORT
    MMIIM_SaveUdbEntry();
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT
    MMIAPIMP3_StopAudioPlayer();
#endif
#ifdef MMI_RECORD_SUPPORT
    MMIAPIRECORD_StopRecordOrPlay();
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
    MMIAPICC_StopCCRecord();
#else
    MMICC_StopRecordInCall();
#endif

	MMIENG_StopENGRecord();
#endif
	MMIAPICC_QuitForPowerOff();
#ifdef MMS_SUPPORT
    if(MMIAPIMMS_IsSendingOrRecving())
        {
        MMIAPIMMS_ClearRecvSendModule();
    }
#endif
    if( MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID))
    {
        MMK_CloseWin(PHONE_STARTUP_NORMAL_WIN_ID);
    }
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchSteps_SaveInfoWhenPowerOff();
#endif
    // 调用MN层函数 请求关机
    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        //MNPHONE_DeactivePsEx(i);
        MNPHONE_SetPowerOffFlag(1,i);

        if (MMIPHONE_PS_POWER_ON_NONE == s_ps_ready[i])//ps not ready
        {
            MNPHONE_PowerOffSimEx(i); //power off SIM
        }
        else
        {
            s_phone_is_power_off_req[i] = TRUE;

#ifdef MMI_VOLTE_SUPPORT
#ifdef MODEM_SUPPORT_SPRD

#ifndef WIN32
            if (MNPHONE_IsImsRegister(i))
            {
                MMIPHONE_DeregisterIms(i);
            }
            else
#endif
#endif
#endif
            {
                if(MNPHONE_IsPsPowerOnEx(i))
                {
                    MNPHONE_PowerOffPsEx(i);
                }
            }
        }
    }

    MMIAPIENG_StopQData();

    MMI_FreeAllRes();
    // 打开关机窗口
    MMIAPIUDISK_ClosePCCamera();
    
#ifdef MMI_WIFI_SUPPORT
    MMIAPIWIFI_Off();
#endif

#ifdef MCARE_V31_SUPPORT 
	McareV31_ExitMcare();
#endif

#ifdef ENG_SUPPORT
    tick = SCI_GetTickCount();
    if(tick > 0)
    {
        tick = tick / MMIENG_STANDBY_TIME_COUNT;	
    }
    else
    {
        tick = 0;
    }
    MMI_WriteNVItem(MMIENG_NV_STANDBY_TIME_ID , &tick);
#endif

    if (RESTART_BY_CHARGE == restart_condition)
    {
        POWER_PowerOff();
    }
    else
    {
        MMK_CreateWin((uint32*)PHONE_POWER_OFF_WIN_TAB,PNULL);
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_AppDeInit();
#endif
    }
#ifdef WRE_SUPPORT
    MMIWRE_PowerOff(TRUE);
#endif
}

LOCAL uint32 MMIAPIPHONE_GetThreadState(void)
{
	char thread_name[2][SCI_MAX_NAME_SIZE];
	char queue_name[2][SCI_MAX_NAME_SIZE];
	uint32 state[2];
	uint32 preempt[2];
	uint32 priority[2];
       uint32 result = 0;
       
	BLOCK_ID thread_id = EFS;

	if(SCI_SUCCESS !=SCI_GetThreadInfo(thread_id, &thread_name[0][0], &queue_name[0][0], &state[0],  &priority[0], &preempt[0]))
	{
		SCI_PASSERT(0,("get thread info error!"));    /*assert verified*/
	}
	
	SCI_TRACE_LOW("thread_name:%s,state=%d\n",&thread_name[0][0] ,state[0]);

	thread_id = P_NVSMS;
	if(SCI_SUCCESS !=SCI_GetThreadInfo(thread_id, &thread_name[1][0], &queue_name[1][0], &state[1],  &priority[1], &preempt[1]))
	{
		SCI_PASSERT(0,("get thread info error!"));    /*assert verified*/
	}
	
	SCI_TRACE_LOW("thread_name:%s,state=%d\n",&thread_name[1][0] ,state[1]);

       if ((state[0] == 5) && (state[1] == 5))
       {
            result = 1;
       }
	return  result;

}
/*****************************************************************************/
// 	Description : power reset the phone should de_init the BT
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerReset(void)
{   
    POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
    
    if (RESTART_BY_CHARGE != restart_condition)
    {
        //Exit Blue tooth
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_AppDeInit();
#endif
    }
    //added by bao 正常重启时需要开机铃声
    MMIZDT_NV_SetFirstPWON(1);
#ifndef WIN32
    SCI_Sleep(10);
#endif
    MMIAPISET_StopAllRing(FALSE);
    
    //在MMIPHONE_PowerReset之前关闭背光
    MMIDEFAULT_TurnOffBackLight();
    MMIDEFAULT_StopKeybadBackLightTimer();
    MMIDEFAULT_SetKeybadBackLight(FALSE);
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchSteps_SaveInfoWhenPowerOff();
#endif

#ifdef ZDT_GSENSOR_SUPPORT    
    ZDT_SaveCurStepsBeforePowerOff();
    SCI_Sleep(10);
#endif

    POWER_Reset();
}

//yangyu add
PUBLIC void MMIAPIPHONE_PowerResetByFactoryReset(void)
{   
    uint32 state;
    POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
    
    SCI_TRACE_LOW("[MMIBT] MMIAPIPHONE_PowerReset restart_condition=%d", restart_condition);
    if (RESTART_BY_CHARGE != restart_condition)
    {
        //Exit Blue tooth
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_AppDeInit();
#endif
    }
    
    MMIAPISET_StopAllRing(FALSE);
    
    //在MMIPHONE_PowerReset之前关闭背光
    MMIDEFAULT_TurnOffBackLight();
    MMIDEFAULT_StopKeybadBackLightTimer();
    MMIDEFAULT_SetKeybadBackLight(FALSE);

   /* while(1)
    {
        if (0 == MMIAPIPHONE_GetThreadState())    //TX_READY=0
        {
            SCI_Sleep(10);
        }
        else
        {
            break;
        }
    }*/
    //ZDT_SaveCurStepsBeforePowerOff();//yangyu add
    //yangyu add begin
#ifdef ZDT_GSENSOR_SUPPORT  
    ZDT_NV_ClearSteps();
#endif
    //yangyu end

    POWER_Reset();
}

/*****************************************************************************/
// 	Description : get plmn name, cmcc, unicom or others
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*****************************************************************************/
PUBLIC PHONE_PLMN_NAME_E MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_E dual_sys)
{
    PHONE_PLMN_NAME_E name = PLMN_INVALID;
    uint16 i = 0;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;
    
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetNetworkName err dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5298_112_2_18_2_45_10_100,(uint8*)"d", dual_sys);
        return PLMN_INVALID;
    }

    if ((g_service_status[dual_sys].plmn.mnc < 100 && (3 == g_service_status[dual_sys].plmn.mnc_digit_num)))
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
        network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;
    }
    else
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
        network_table_len = MAX_NETWORK_NUM;
    }
    
    for (i = 0; i < network_table_len; i++)
    {
        if ((network_table_ptr[i].mnc == g_service_status[dual_sys].plmn.mnc)
            && (network_table_ptr[i].mcc == g_service_status[dual_sys].plmn.mcc))
        {
            name = network_table_ptr[i].name;
            break;
        }
    }
    
    return name;   
}

/*****************************************************************************/
// 	Description : get the network name
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIPHONE_GetNetWorkNameId(MMI_GMMREG_RAT_E rat, MN_PLMN_T *plmn_ptr)
{
    uint16 i = 0;
    MMI_TEXT_ID_T network_name_id = TXT_NULL;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;

    if (PNULL == plmn_ptr)
    {
        return network_name_id;
    }

    if ((plmn_ptr->mnc < 100 && (3 == plmn_ptr->mnc_digit_num)))
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
        network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;
    }
    else
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
        network_table_len = MAX_NETWORK_NUM;
    }
    
    for (i = 0 ; i < network_table_len ; i++)
    {
        if ((network_table_ptr[i].mnc == plmn_ptr->mnc)
            && (network_table_ptr[i].mcc == plmn_ptr->mcc))
        {
            network_name_id = network_table_ptr[i].name_index;

#ifdef CMCC_UI_STYLE
            if(MMI_GMMREG_RAT_3G == rat && TXT_NET_CMCC == network_name_id)
            {
                network_name_id = TXT_NET_CMCC_3G;
            }
#endif
            break;
        }
    }
    
    return (network_name_id);
}

/*****************************************************************************/
// 	Description : get the network id
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC PHONE_PLMN_NAME_E MMIAPIPHONE_GetNetWorkId(MN_PLMN_T *plmn_ptr)
{
    uint16 i = 0;
    PHONE_PLMN_NAME_E plmn_name = PLMN_INVALID;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;

    if (PNULL == plmn_ptr)
    {
        return plmn_name;
    }

    network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
    network_table_len = MAX_NETWORK_NUM;
    
    for (i = 0 ; i < network_table_len ; i++)
    {
        if ((network_table_ptr[i].mnc == plmn_ptr->mnc)
            && (network_table_ptr[i].mcc == plmn_ptr->mcc))
        {
            plmn_name = network_table_ptr[i].name;

            break;
        }
    }

    if (PLMN_INVALID == plmn_name)
    {
        network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
        network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;

        for (i = 0 ; i < network_table_len ; i++)
        {
            if ((network_table_ptr[i].mnc == plmn_ptr->mnc)
                && (network_table_ptr[i].mcc == plmn_ptr->mcc))
            {
                plmn_name = network_table_ptr[i].name;

                break;
            }
        }
    }
    
    return (plmn_name);
}

/*****************************************************************************/
// 	Description : get the network info
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:in gprs data account,we only use one mnc to represent one operator,but it may have other mnc
//           if so,in auto adapting ,we will not find the right network id;so we use mcc and networkname index to get network name
/*****************************************************************************/
PUBLIC const MMI_NETWORK_NAME_T *MMIAPIPHONE_GetNetWorkInfo(PHONE_PLMN_NAME_E plmn_name)
{
    uint16 i = 0;
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;

    network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
    network_table_len = MAX_NETWORK_NUM;
    
    /*search from 0 to higher,so we need to set data account mnc as the smallest  of all mnc for the oprator*/
    for (i = 0 ; i < network_table_len ; i++)
    {
        if (network_table_ptr[i].name == plmn_name)
        {
            return &network_table_ptr[i];
        }
    }

    network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
    network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;

    for (i = 0 ; i < network_table_len ; i++)
    {
        if (network_table_ptr[i].name == plmn_name)
        {
            return &network_table_ptr[i];
        }
    }
    
    return PNULL;
}

/*****************************************************************************/
// 	Description : get full plmn name, cmcc, unicom or others
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetFullPlmn(MN_DUAL_SYS_E dual_sys, PHONE_PLMN_NAME_E *plmn_name, PHONE_PLMN_NAME_E *hplmn_name)
{
    const MMI_NETWORK_NAME_T *network_table_ptr = PNULL;
    uint16 network_table_len = 0;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    if (PNULL != plmn_name)
    {
        *plmn_name = MMIAPIPHONE_GetNetworkName(dual_sys);
    }

    if (PNULL != hplmn_name)
    {
        uint16 i = 0;

        *hplmn_name = PLMN_INVALID;

        if ((s_sim_status[dual_sys].hplmn.mnc < 100 && (3 == s_sim_status[dual_sys].hplmn.mnc_digit_num)))
        {
            network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_mnc_digit_num_3_nametable;
            network_table_len = MAX_NETWORK_MNC_DIGIT_NUM_3_NUM;
        }
        else
        {
            network_table_ptr = (const MMI_NETWORK_NAME_T *)s_network_nametable;
            network_table_len = MAX_NETWORK_NUM;
        }

        if (SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys))
        {
            for (i = 0; i < network_table_len; i++)
            {
                if ((network_table_ptr[i].mnc == s_sim_status[dual_sys].hplmn.mnc) &&
                    (network_table_ptr[i].mcc == s_sim_status[dual_sys].hplmn.mcc))
                {
                    *hplmn_name = network_table_ptr[i].name;
                    break;
                }
            }
        }
    }
}


/*****************************************************************************/
// 	Description : 当sim卡注册失败和注册成功的时候都应该可以设置pin1
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPermitSetPin1(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN     is_permit = FALSE;
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);
    
    is_permit = (BOOLEAN)((SIM_STATUS_OK == sim_status) 
                                        || (SIM_STATUS_REGISTRATION_FAILED == sim_status)
                                        || (SIM_STATUS_PIN_BLOCKED == sim_status));
    
    return(is_permit);
}

/*****************************************************************************/
// 	Description : 获取设置pin1 的sim 个数
//	Global resource dependence : 
//  Author: Michael.Shi
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetPermitSetPin1Num(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsPermitSetPin1(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : according to ps status, judge if wap & mms is available
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_ROAMING_STATUS_E MMIAPIPHONE_GetRoamingStatus(MN_DUAL_SYS_E dual_sys)
{
    MMIPHONE_ROAMING_STATUS_E roaming_status = MMIPHONE_ROAMING_STATUS_MAX;
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetRoamingStatus:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5561_112_2_18_2_45_11_101,(uint8*)"d", dual_sys);
        return MMIPHONE_ROAMING_STATUS_NO_ROMING;
    }
    if(!g_service_status[dual_sys].isRoaming)
    {
        roaming_status = MMIPHONE_ROAMING_STATUS_NO_ROMING;
    }
    else
    {
        if(g_service_status[dual_sys].plmn.mcc == s_sim_status[dual_sys].hplmn.mcc
            ||((g_service_status[dual_sys].plmn.mcc == 404)&&(s_sim_status[dual_sys].hplmn.mcc == 405))    
            ||((g_service_status[dual_sys].plmn.mcc == 405)&&(s_sim_status[dual_sys].hplmn.mcc == 404))//india MCC
        )
        {
            roaming_status = MMIPHONE_ROAMING_STATUS_NATIVE_ROMING;
        }
        else
        {
            roaming_status = MMIPHONE_ROAMING_STATUS_NATIONAL_ROMING;
        }
    }
    //SCI_TRACE_LOW:"roaming status: %d, net isroaming: %d, net mcc: %d, sim mcc: %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5583_112_2_18_2_45_11_102,(uint8*)"dddd", roaming_status, g_service_status[dual_sys].isRoaming, g_service_status[dual_sys].plmn.mcc, s_sim_status[dual_sys].hplmn.mcc);
    return roaming_status;
}

/*****************************************************************************/
// 	Description : get opn display string
//	Global resource dependence : 
//  Author: kun.yu
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_GetAciString(PHONE_SERVICE_STATUS_T *service_status,//IN:
                                         MMI_STRING_T *aci_str_ptr, //OUT
                                         uint16 max_aci_len //IN
                                         )
{
    uint16 i = 0;
    uint16 aci_index = 0;
    char   aic_info[MAX_ACI_INFO_LEN + 1] = {0};

    if( (PNULL == service_status) 
#if !defined(MMI_NETWORK_NAME_SUPPORT)
      || (!service_status->is_add_ci)
#endif
        ||(PNULL == aci_str_ptr))
    {
        return;
    }
    
    for(i = 0; i < ARR_SIZE(s_network_acitable); i++)
    {
        if(s_network_acitable[i].mcc == service_status->plmn.mcc)
        {
            aci_index = i;
            break;
        }
    }

    if(aci_index > 0)
    {
        aci_str_ptr->wstr_len = SCI_STRLEN((char*)s_network_acitable[aci_index].aci_str);
        aci_str_ptr->wstr_len = MIN(aci_str_ptr->wstr_len, max_aci_len);
        MMI_STRNTOWSTR(aci_str_ptr->wstr_ptr,
                                aci_str_ptr->wstr_len,
                                (uint8 *)s_network_acitable[aci_index].aci_str,
                                aci_str_ptr->wstr_len,
                                aci_str_ptr->wstr_len
                                );
    }
    else
    {
        sprintf((char *)aic_info, "%03d", service_status->plmn.mcc);
        aci_str_ptr->wstr_len = SCI_STRLEN((char *)aic_info);
        aci_str_ptr->wstr_len = MIN(aci_str_ptr->wstr_len, max_aci_len);
        MMI_STRNTOWSTR(aci_str_ptr->wstr_ptr,
                                    aci_str_ptr->wstr_len,
                                    (uint8 *)aic_info,
                                    aci_str_ptr->wstr_len,
                                    aci_str_ptr->wstr_len
                                    );
    }
}

/*****************************************************************************/
// 	Description : get opn display string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:  
/*****************************************************************************/
LOCAL void GetOpnString(MN_DUAL_SYS_E dual_sys,//IN:
                                            MMI_STRING_T *opn_str_ptr,//IN/OUT
                                            uint16 max_opn_len
                                            )
{
#if !defined(MMI_NETWORK_NAME_SUPPORT)
    PHONE_SERVICE_STATUS_T  *service_status = &g_service_status[dual_sys];         //the status of service
    int i =0;
    if(PNULL == opn_str_ptr ||dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"GetOpnString dual_sys = %d OR opn_str_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5652_112_2_18_2_45_11_103,(uint8*)"d", dual_sys);
        return;
    }
   SCI_TRACE_BUF("zhangqiang test  service_status->opn ",service_status->opn,strlen((char *)service_status->opn));
    if (service_status->is_opn_exist && 0 < service_status->opn_len)
    {
        if (MN_OPN_DCS_8BIT == service_status->opn_dcs)
        {
            opn_str_ptr->wstr_len = MIN(service_status->opn_len, max_opn_len);

            opn_str_ptr->wstr_len = MMIAPICOM_Default2Wchar(
                                                            (uint8 *)service_status->opn,
                                                            opn_str_ptr->wstr_ptr,
                                                            opn_str_ptr->wstr_len
                                                            );
        }
        else
        {
            opn_str_ptr->wstr_len = MIN(service_status->opn_len/sizeof(wchar), max_opn_len);
            MMI_WSTRNCPY(opn_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    (wchar *)service_status->opn,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len
                                    );
            for(i = 0; i < opn_str_ptr->wstr_len; i++)
            {
                opn_str_ptr->wstr_ptr[i] = (opn_str_ptr->wstr_ptr[i] & 0xFF00)>>8 | (opn_str_ptr->wstr_ptr[i] & 0xFF)<<8;
            }
        }
    }
    else
    {
        MMIPHONE_NV_NETWORK_NAME_T* nv_network_name = MMIPHONE_GetNVNetworkNameInfo();
            //是否相同卡,同一张卡才能使用之前的OPN
        if(MMIAPIPHONE_IsSameSim(dual_sys)
            &&(nv_network_name[dual_sys].is_opn_exist)
            &&(0 < nv_network_name[dual_sys].opn_len))
        {
            if (MN_OPN_DCS_UCS2 == nv_network_name[dual_sys].opn_dcs)
            {
                opn_str_ptr->wstr_len = MIN(nv_network_name[dual_sys].opn_len/sizeof(wchar), max_opn_len);

                MMI_WSTRNCPY(opn_str_ptr->wstr_ptr,
                                        opn_str_ptr->wstr_len,
                                        (wchar *)nv_network_name[dual_sys].opn,
                                        opn_str_ptr->wstr_len,
                                        opn_str_ptr->wstr_len
                                        );
                for(i = 0; i < opn_str_ptr->wstr_len; i++)
                {
                    opn_str_ptr->wstr_ptr[i] = (opn_str_ptr->wstr_ptr[i] & 0xFF00)>>8 | (opn_str_ptr->wstr_ptr[i] & 0xFF)<<8;
                }
            }
            else
            {
                opn_str_ptr->wstr_len = MIN(nv_network_name[dual_sys].opn_len, max_opn_len);

                opn_str_ptr->wstr_len = MMIAPICOM_Default2Wchar(
                                                                (uint8 *)nv_network_name[dual_sys].opn,
                                                                opn_str_ptr->wstr_ptr,
                                                                opn_str_ptr->wstr_len
                                                                );
            }
        }
        else
        {
            opn_str_ptr->wstr_len = max_opn_len;

            MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys),
                                       &service_status->plmn,
                                       opn_str_ptr->wstr_ptr,
                                       &opn_str_ptr->wstr_len,
                                       FALSE);
        }
    }
#endif
}

/*****************************************************************************/
// 	Description : get spn display string
//	Global resource dependence : 
//  Author: wancan.you
//	Note:  
/*****************************************************************************/
LOCAL void GetSpnString(MN_DUAL_SYS_E dual_sys,//IN:
                                            MMI_STRING_T *spn_str_ptr,//IN/OUT
                                            uint16 max_spn_len
                                            )
{
#if !defined(MMI_NETWORK_NAME_SUPPORT)
    PHONE_SERVICE_STATUS_T  *service_status = &g_service_status[dual_sys];         //the status of service

    if(PNULL == spn_str_ptr || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"GetOpnString dual_sys = %d OR spn_str_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5736_112_2_18_2_45_11_104,(uint8*)"d", dual_sys);
        return;
    }

    if (0 < service_status->spn_len)
    {
        MMIAPICOM_ParseSimStr(service_status->spn,
                                            service_status->spn_len,
                                            spn_str_ptr,
                                            max_spn_len);
    }
#endif    
}

/*****************************************************************************/
// 	Description : compare spn and opn
//	Global resource dependence : 
//  Author: kun.yu
//	Note:  
/*****************************************************************************/
LOCAL BOOLEAN MMIPHONE_IsSPNEqualtoOPN(MMI_STRING_T *opn_str_ptr,//IN:
                                                                                    MMI_STRING_T *spn_str_ptr//IN:
                                                                                    )
{
    BOOLEAN result = FALSE;

    if((PNULL == opn_str_ptr->wstr_ptr) || (PNULL == spn_str_ptr->wstr_ptr))
    {
        return FALSE;
    }
    
    if(0 == MMIAPICOM_CompareTwoWstrExt(opn_str_ptr->wstr_ptr,
                                                        opn_str_ptr->wstr_len,
                                                        spn_str_ptr->wstr_ptr,
                                                        spn_str_ptr->wstr_len,
                                                        FALSE
                                                        ))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : cat opn spn display string
//	Global resource dependence : 
//   Author: kun.yu
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_CatAciOpnSpnString(MMI_STRING_T *aci_str_ptr,//IN 
                                        MMI_STRING_T *opn_str_ptr,//IN:
                                        MMI_STRING_T *spn_str_ptr,//IN:
                                        MMI_STRING_T *network_str_ptr//OUT
                                        )
{
    const char *lnk = "_";
    uint16 length = 0;

    if (aci_str_ptr->wstr_len + opn_str_ptr->wstr_len + spn_str_ptr->wstr_len >= MMIPHONE_MAX_OPER_NAME_LEN - 1)
    {
        return;
    }

    if(aci_str_ptr->wstr_len > 0)
    {
        MMI_WSTRNCPY(network_str_ptr->wstr_ptr,
                                aci_str_ptr->wstr_len,
                                aci_str_ptr->wstr_ptr,
                                aci_str_ptr->wstr_len,
                                aci_str_ptr->wstr_len
                                );

        network_str_ptr->wstr_len = aci_str_ptr->wstr_len;
    }

    if (opn_str_ptr->wstr_len > 0)
    {
        if(aci_str_ptr->wstr_len > 0)
        {
            length = strlen(lnk);
    
            MMI_STRNTOWSTR(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                    length,
                                    (uint8 *)lnk,
                                    length,
                                    length
                                    );

            network_str_ptr->wstr_len += length;

            //opn
            MMI_WSTRNCPY(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len
                                    );
            network_str_ptr->wstr_len += opn_str_ptr->wstr_len;

        }
        else
        {
            MMI_WSTRNCPY(network_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_ptr,
                                    opn_str_ptr->wstr_len,
                                    opn_str_ptr->wstr_len
                                    );

            network_str_ptr->wstr_len = opn_str_ptr->wstr_len;
        }
    }

	//select one of spn&opn to display CR 510183
    if ((spn_str_ptr->wstr_len > 0) && (!MMIPHONE_IsSPNEqualtoOPN(opn_str_ptr, spn_str_ptr)))
    {
        if (opn_str_ptr->wstr_len > 0)
        {
            length = strlen(lnk);
            
            MMI_STRNTOWSTR(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                    length,
                                    (uint8 *)lnk,
                                    length,
                                    length
                                    );
            
            network_str_ptr->wstr_len += length;
        }

        MMI_WSTRNCPY(network_str_ptr->wstr_ptr + network_str_ptr->wstr_len,
                                spn_str_ptr->wstr_len,
                                spn_str_ptr->wstr_ptr,
                                spn_str_ptr->wstr_len,
                                spn_str_ptr->wstr_len
                                );

        network_str_ptr->wstr_len += spn_str_ptr->wstr_len;
    }

    return;
}

/*****************************************************************************/
// 	Description : is network selecting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsNetworkSelecting(MN_DUAL_SYS_E dual_sys)
{
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;

    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsNetworkSelecting dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5905_112_2_18_2_45_12_105,(uint8*)"d",dual_sys);
        return FALSE;
    }
    
    plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);

    //SCI_TRACE_LOW:"MMIAPIPHONE_IsNetworkSelecting plmn_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5911_112_2_18_2_45_12_106,(uint8*)"d", plmn_status);
    
    if(( PLMN_REGISTER_GPRS_ONLY == plmn_status)
        || ( PLMN_REGISTER_SERVICE == plmn_status)
        || g_service_status[dual_sys].is_slecting)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : get sim card and plmn service status string to display in idle
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetSimAndServiceString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *network_str_ptr)
{
    MMI_TEXT_ID_T	idle_str_id = TXT_NULL;
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);

    if (PNULL == network_str_ptr || PNULL == network_str_ptr->wstr_ptr || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"GetSimAndServiceString: the network_str_ptr || network_str_ptr->wstr_ptr ||dual_sys"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5936_112_2_18_2_45_12_107,(uint8*)"");
        return;
    }

    if (MMIAPISET_GetFlyMode())
    {
        idle_str_id = TXT_NO_SERVICE;
        MMI_GetLabelTextByLang(idle_str_id, network_str_ptr);
        return;
    }

    //SCI_TRACE_LOW:"MMIAPIPHONE_GetSimAndServiceString s_sim_status[%d].simStatus %d,is_opn_need=%d,opn_len=%d,is_opn_exist=%d,is_spn_need=%d,spn_len=%d"
#if !defined(MMI_NETWORK_NAME_SUPPORT)
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5942_112_2_18_2_45_12_108,(uint8*)"ddddddd",dual_sys, sim_status,g_service_status[dual_sys].is_opn_need,g_service_status[dual_sys].opn_len,g_service_status[dual_sys].is_opn_exist,g_service_status[dual_sys].is_spn_need,g_service_status[dual_sys].spn_len);
#endif

    network_str_ptr->wstr_len = 0;
    //if sim error then display sim string in sim_status_string
    if (sim_status < SIM_STATUS_MAX)
    {
        if (sim_status != SIM_STATUS_OK) //sim not ready
        {
            if(SIM_STATUS_NO_SIM == sim_status)
            {
                idle_str_id = TXT_NO_SIM;
            }
            else
            {
                idle_str_id = s_sim_status_string[sim_status];
            }

            MMI_GetLabelTextByLang(idle_str_id, network_str_ptr);
        }
        else
        {
            if (g_service_status[dual_sys].is_slecting //searching
                || ((PLMN_NORMAL_GSM_ONLY != g_service_status[dual_sys].plmn_status) 
                && (PLMN_NORMAL_GSM_GPRS_BOTH != g_service_status[dual_sys].plmn_status)))
            {
                idle_str_id = GetNetWorkNameId(dual_sys, 
                                               &g_service_status[dual_sys].plmn,
                                               g_service_status[dual_sys].plmn_status,
                                               g_service_status[dual_sys].is_slecting);

                MMI_GetLabelTextByLang(idle_str_id, network_str_ptr);
            }
            else
            {
            #ifdef MMI_NETWORK_NAME_SUPPORT
                MMIPHONE_GetNetworkNameString(dual_sys,network_str_ptr);
            #else
                MMI_STRING_T aci_string = {0}; 
                MMI_STRING_T opn_string = {0};
                MMI_STRING_T spn_string = {0};
                wchar aci[MAX_ACI_INFO_LEN + 1] = {0};
                wchar opn[MAX_ONSHORTFORM_LEN + 1] = {0};
                wchar spn[MN_SPN_ID_NUM_LEN + 1] = {0};
                //get aci string
                if(g_service_status[dual_sys].is_add_ci)
                {
                    aci_string.wstr_ptr = (wchar *)aci;
                    MMIPHONE_GetAciString(&g_service_status[dual_sys], &aci_string, MAX_ACI_INFO_LEN);
                }

                //get opn string
                if (g_service_status[dual_sys].is_opn_need)
                {
                    opn_string.wstr_ptr = (wchar *)opn;
                    GetOpnString(dual_sys, &opn_string, MAX_ONSHORTFORM_LEN);
                }

                //get spn string
                if (g_service_status[dual_sys].is_spn_need)
                {
                    spn_string.wstr_ptr = (wchar *)spn;
                    GetSpnString(dual_sys, &spn_string, MN_SPN_ID_NUM_LEN);
                }

                MMIPHONE_CatAciOpnSpnString(&aci_string, &opn_string, &spn_string, network_str_ptr);
            #endif
                //异常处理
                if (0 == network_str_ptr->wstr_len)
                {
                    network_str_ptr->wstr_len = MMIPHONE_MAX_OPER_NAME_LEN;
                    MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys),
                                               &g_service_status[dual_sys].plmn,
                                               network_str_ptr->wstr_ptr,
                                               &network_str_ptr->wstr_len,
                                               FALSE);
                }
            }
        }
    }
}

LOCAL BOOLEAN IsPlmnInNormalService(MN_PHONE_PLMN_STATUS_E status)
{
    BOOLEAN bInNormal = (status == PLMN_NORMAL_GSM_ONLY)
        || (status == PLMN_NORMAL_GPRS_ONLY)
        || (status == PLMN_NORMAL_GSM_GPRS_BOTH)
        || (status == PLMN_FULL_PS_SERVICE);
    return bInNormal;
}

PUBLIC void MMIAPIPHONE_GetSimAndServiceStringForXtc(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *network_str_ptr)
{
    MMI_TEXT_ID_T	idle_str_id = TXT_NULL;
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);

    if (PNULL == network_str_ptr || PNULL == network_str_ptr->wstr_ptr || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"GetSimAndServiceString: the network_str_ptr || network_str_ptr->wstr_ptr ||dual_sys"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5936_112_2_18_2_45_12_107,(uint8*)"");
        return;
    }

    if (MMIAPISET_GetFlyMode())
    {
        idle_str_id = TXT_NO_SERVICE;
        MMI_GetLabelTextByLang(idle_str_id, network_str_ptr);
        return;
    }

    //SCI_TRACE_LOW:"MMIAPIPHONE_GetSimAndServiceString s_sim_status[%d].simStatus %d,is_opn_need=%d,opn_len=%d,is_opn_exist=%d,is_spn_need=%d,spn_len=%d"
#if !defined(MMI_NETWORK_NAME_SUPPORT)
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_5942_112_2_18_2_45_12_108,(uint8*)"ddddddd",dual_sys, sim_status,g_service_status[dual_sys].is_opn_need,g_service_status[dual_sys].opn_len,g_service_status[dual_sys].is_opn_exist,g_service_status[dual_sys].is_spn_need,g_service_status[dual_sys].spn_len);
#endif

    network_str_ptr->wstr_len = 0;

    //if sim error then display sim string in sim_status_string
    if (sim_status < SIM_STATUS_MAX)
    {
        if (sim_status != SIM_STATUS_OK) //sim not ready
        {
            if(SIM_STATUS_NO_SIM == sim_status)
            {
                idle_str_id = TXT_NO_SIM;
            }
            else
            {
                idle_str_id = TXT_NO_SERVICE;
            }

            MMI_GetLabelTextByLang(idle_str_id, network_str_ptr);
        }
        else
        {
            if (g_service_status[dual_sys].is_slecting //searching
                ||!IsPlmnInNormalService(g_service_status[dual_sys].plmn_status))
            {
                idle_str_id = TXT_NO_SERVICE;
                MMI_GetLabelTextByLang(idle_str_id, network_str_ptr);
                SCI_TRACE_LOW("lzk status name searching.");
            }
            else
            {
            #ifdef MMI_NETWORK_NAME_SUPPORT
                MMIPHONE_GetNetworkNameString(dual_sys,network_str_ptr);
                SCI_TRACE_LOW("lzk status name not telc.");
            #else
                MMI_STRING_T aci_string = {0}; 
                MMI_STRING_T opn_string = {0};
                MMI_STRING_T spn_string = {0};
                wchar aci[MAX_ACI_INFO_LEN + 1] = {0};
                wchar opn[MAX_ONSHORTFORM_LEN + 1] = {0};
                wchar spn[MN_SPN_ID_NUM_LEN + 1] = {0};
                //get aci string
                if(g_service_status[dual_sys].is_add_ci)
                {
                    aci_string.wstr_ptr = (wchar *)aci;
                    MMIPHONE_GetAciString(&g_service_status[dual_sys], &aci_string, MAX_ACI_INFO_LEN);
                }

                //get opn string
                if (g_service_status[dual_sys].is_opn_need)
                {
                    opn_string.wstr_ptr = (wchar *)opn;
                    GetOpnString(dual_sys, &opn_string, MAX_ONSHORTFORM_LEN);
                }

                //get spn string
                if (g_service_status[dual_sys].is_spn_need)
                {
                    spn_string.wstr_ptr = (wchar *)spn;
                    GetSpnString(dual_sys, &spn_string, MN_SPN_ID_NUM_LEN);
                }

                MMIPHONE_CatAciOpnSpnString(&aci_string, &opn_string, &spn_string, network_str_ptr);
            #endif
                //异常处理
                if (0 == network_str_ptr->wstr_len)
                {
                    network_str_ptr->wstr_len = MMIPHONE_MAX_OPER_NAME_LEN;
                    MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys),
                                               &g_service_status[dual_sys].plmn,
                                               network_str_ptr->wstr_ptr,
                                               &network_str_ptr->wstr_len,
                                               FALSE);
                }
            }
        }
    }
}

/*****************************************************************************/
// 	Description : 获得sim card状态
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC SIM_STATUS_E MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetSimStatus err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6032_112_2_18_2_45_12_109,(uint8*)"d", dual_sys);
        return SIM_NOT_INITED;
    }
    SCI_TRACE_LOW("MMIAPIPHONE_GetSimStatus:%d",s_sim_status[dual_sys].simStatus);
    return s_sim_status[dual_sys].simStatus;
}

/*****************************************************************************/
// 	Description : 设置sim card状态
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC void MMIPHONE_SetSimStatus(MN_DUAL_SYS_E dual_sys, SIM_STATUS_E sim_status)
{
    //SCI_TRACE_LOW:"MMIPHONE_SetSimStatus:dual sys = %d, dual status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6045_112_2_18_2_45_12_110,(uint8*)"dd",dual_sys,sim_status);
    
    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
            s_sim_status[dual_sys].simStatus = sim_status;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIPHONE_SetSimStatus err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6053_112_2_18_2_45_12_111,(uint8*)"d", dual_sys);
    }
}

/*****************************************************************************/
// 	Description : set sim card exited status
//	Global resource dependence : 
//  Author:kelly.li
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetSimExistedStatus(
                                            MN_DUAL_SYS_E dual_sys,
                                            BOOLEAN       is_existed
                                            )
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_SetSimExistedStatus:dual sys = %d, is_existed = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6066_112_2_18_2_45_12_112,(uint8*)"dd",dual_sys,is_existed);
    
    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        s_sim_existed_status[dual_sys] = is_existed;

        if (MMIAPIIDLE_IdleWinIsOpen())
        {
            MAIN_UpdateIdleSignalState();
        }
        return TRUE;
    }
    else
    {

        //SCI_TRACE_LOW:"MMIAPIPHONE_SetSimExistedStatus:err dual sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6081_112_2_18_2_45_12_113,(uint8*)"d",dual_sys);
        return FALSE;
    }

}

/*****************************************************************************/
// 	Description : Get sim card exited status
//	Global resource dependence : 
//  Author:kelly.li
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }
    
    return(s_sim_existed_status[dual_sys]);
}

/*****************************************************************************/
// 	Description : Sim卡是否OK，包括PS
//	Global resource dependence : 
//  Author:
//	Note:  SIM OK 的状态: 'SIM_STATUS_OK' and 'PS_Ready_Normal' and '!fly_Mode'
//     Pay attention to this interface used,should better considered MMIAPIPHONE_IsSimAvailable     
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_E dual_sys)
{
    return MMIPHONE_IsSimOk(dual_sys);
}

/*****************************************************************************/
// 	Description : Sim卡是否OK，包括PS
//	Global resource dependence : 
//  Author:
//	Note:  SIM OK 的状态: 'SIM_STATUS_OK' and 'PS_Ready_Normal' and '!fly_Mode'
//     Pay attention to this interface used,should better considered MMIAPIPHONE_IsSimAvailable     
/*****************************************************************************/
BOOLEAN MMIPHONE_IsSimOk(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsSimOk err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6120_112_2_18_2_45_12_114,(uint8*)"d", dual_sys);
        return FALSE;
    }
    
    return((BOOLEAN)(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys)
                                && (MMIPHONE_PS_POWER_ON_NORMAL == s_ps_ready[dual_sys]) && !MMIAPISET_GetFlyMode()));
}

/*****************************************************************************/
// 	Description : Sim卡是否OK，不包括PS
//	Global resource dependence : 
//  Author:
//	Note:  SIM Card OK 的状态包括两种: SIM_STATUS_OK / SIM_STATUS_REGISTRATION_FAILED
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsSimCardOk err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6136_112_2_18_2_45_12_115,(uint8*)"d", dual_sys);
        return FALSE;
    }

    
    return((BOOLEAN)(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(dual_sys)
                                || SIM_STATUS_REGISTRATION_FAILED == MMIAPIPHONE_GetSimStatus(dual_sys)));
}

/*****************************************************************************/
// 	Description : Sim卡是否可用
//	Global resource dependence : 
//  Author: Michael.shi
//	Note:  SIM 卡是否可用的判断条件 : sim 卡存在并且处于待机设置！！
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsSimAvailable err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6153_112_2_18_2_45_12_116,(uint8*)"d", dual_sys);
        return FALSE;
    }

    // 非飞行模式才做处理!!!
    if( !MMIAPISET_GetFlyMode() )
    {
        MMISET_DUALSYS_TYPE_E e_dual_sys = MMIAPISET_GetMultiSysSetting();
        //sim 卡存在并且待机!!!
        if((e_dual_sys & (1<<dual_sys)) && MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
// 	Description : 可用的Sim卡个数
//	Global resource dependence : 
//  Author: Michael.Shi
//	Note:  获取存在并且设置为待机的SIM卡的个数
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSimAvailableNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsSimAvailable(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : Sim卡OK个数和卡，包括PS
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSimOkNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsSimOk(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : Sim卡OK个数和卡，不包括PS
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSimCardOkNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_IsSimCardOk(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : Get sim card existed status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSimExistedNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(i))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : Get ps ready status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetPSReadyNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if ((MMIPHONE_PS_POWER_ON_NONE != s_ps_ready[i]))
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : get sys ok num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetSysOkNum(BOOLEAN  *p, uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    if(PNULL == p)
    {
        //SCI_TRACE_LOW:"GetSysOkNum: the p=%d || idx_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6302_112_2_18_2_45_13_117,(uint8*)"dd", p, idx_ptr);
        return 0;
    }

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (p[i])
        {
            if ((idx_ptr != PNULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : get national roaming num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
uint32 MMIAPIPHONE_GetNationalRoamingNum(uint16 *idx_ptr, uint16 array_len)
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_GetRoamingStatus(i) != MMIPHONE_ROAMING_STATUS_NO_ROMING)
        {
            if ((idx_ptr != NULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : pop up an alert window to show sim wrong status
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
void MMIAPIPHONE_AlertSimNorOKStatusII(MN_DUAL_SYS_E dual_sys)
{
    MMI_TEXT_ID_T 	text_id = TXT_NULL;
    MMI_WIN_ID_T	alert_win_id = MMIPHONE_ALERT_SIM_NOT_OK_WIN_ID;
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);
    MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_WARNING;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    
    if(sim_status >= SIM_STATUS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_AlertSimNorOKStatus:error simStatus=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6359_112_2_18_2_45_13_118,(uint8*)"d", sim_status);
        return;
    }
    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_AlertSimNorOKStatus:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6365_112_2_18_2_45_13_119,(uint8*)"d", dual_sys);
        return;
    }
    
    if (MMIAPISET_GetFlyMode())
    {
        MMIAPISET_AlertFlyMode();
    }    
    else if(!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
        sim_status = SIM_STATUS_NO_SIM;        
        text_id = s_sim_status_string[sim_status];
        
        if (text_id != TXT_NULL)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }
    else if (!(e_dualsys_setting & (0x01 << dual_sys)))
    {
        text_id = TXT_SIM_DEACTIVATED;

        MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {
        text_id = s_sim_status_string[sim_status];
        image_id = IMAGE_PUBWIN_WARNING;

        if (TXT_NULL == text_id)
        {
            if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[dual_sys])
            {
                text_id = TXT_SYS_WAITING_AND_TRY_LATER;
                image_id = IMAGE_PUBWIN_WAIT;
            }
        }
        
        if (text_id != TXT_NULL)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }
}

/*****************************************************************************/
// 	Description : pop up an alert window to show sim wrong status
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
void MMIAPIPHONE_AlertSimNorOKStatus(void)
{
    MMI_TEXT_ID_T 	text_id = TXT_NULL;
    MMI_WIN_ID_T	alert_win_id = MMIPHONE_ALERT_SIM_NOT_OK_WIN_ID;
    SIM_STATUS_E sim_status = SIM_STATUS_OK;
    uint16 sys_ok = MN_DUAL_SYS_1;

    if (MMIAPISET_GetFlyMode())
    {
        text_id = TXT_CLOSE_FLY_MODE_FIRST;
        WatchPUB_Alert_Note_Win(alert_win_id,text_id);
    }
    else
    {
        uint32 sys_num = MMIAPISET_GetMultiSysSettingNum(&sys_ok, 1) ;
        uint32 exist_sim_num = MMIAPIPHONE_GetSimExistedNum(PNULL,0);
        
        if (0 == sys_num)
        {
             ///没有可用的SIM 卡，可能是确实没有卡存在，也可能是待机卡为零
             if((0 < exist_sim_num )/* && ( MMI_DUAL_SYS_MAX > exist_sim_num)*/)
             {
                text_id = TXT_INSERT_SIM_AND_INFO;
                WatchPUB_Alert_Note_Win(alert_win_id,text_id);

                return; /// return directly!!
             }
             else
             {
                sim_status = SIM_STATUS_NO_SIM; //// call text id :insert_sim or insert_sim_and_info
             }
        }
        else
        {
            sim_status = MMIAPIPHONE_GetSimStatus((MN_DUAL_SYS_E)sys_ok);
        }
        
        if(sim_status <= SIM_STATUS_MAX)
        {
            text_id = s_sim_status_string[sim_status];
            
            if (text_id != TXT_NULL)
            {
                WatchPUB_Alert_Note_Win(alert_win_id,text_id);
            }
            else
            {
                if (MMIPHONE_PS_POWER_ON_NORMAL != s_ps_ready[sys_ok])
                {
                    text_id = TXT_SYS_WAITING_AND_TRY_LATER;   
                    WatchPUB_Alert_Note_Win(alert_win_id,text_id);
                }
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIPHONE_AlertSimNorOKStatus err sim status: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6474_112_2_18_2_45_13_120,(uint8*)"d", sim_status);
        }
    }
}


/*****************************************************************************/
// 	Description : Is Pin2 Required ACM
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsPin2RequiredACM(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsPin2RequiredACM err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6488_112_2_18_2_45_13_121,(uint8*)"d", dual_sys);
        return FALSE;
    }    
    return s_sim_status[dual_sys].is_pin2_Required_acm;
}

/*****************************************************************************/
// 	Description : Is Pin2 Required ACM Max
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsPin2RequiredACMMax(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsPin2RequiredACMMax err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6502_112_2_18_2_45_13_122,(uint8*)"d", dual_sys);
        return FALSE;
    }    
    
    return s_sim_status[dual_sys].is_pin2_required_acm_max;
}

/*****************************************************************************/
// 	Description : Is Pin2 Required Puct
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsPin2RequiredPuct(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsPin2RequiredPuct err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6517_112_2_18_2_45_13_123,(uint8*)"d", dual_sys);
        return FALSE;
    }    
    
    return s_sim_status[dual_sys].is_pin2_Required_puct;
}

/*****************************************************************************/
// 	Description : Is aoc support
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_IsAocSupport(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsAocSupport err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6532_112_2_18_2_45_13_124,(uint8*)"d", dual_sys);
        return FALSE;
    }    

#ifdef MMI_DUALMODE_ENABLE
    {
        MNSIM_AOC_T aoc_status = {0};

        aoc_status = MNSIM_GetAocEx(dual_sys);

        //SCI_TRACE_LOW:"MMIAPIPHONE_IsAocSupport aoc_is_support=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6542_112_2_18_2_45_13_125,(uint8*)"d", aoc_status.aoc_is_support);

        return aoc_status.aoc_is_support;
    }
#else
    return s_sim_status[dual_sys].is_aoc_support;
#endif
}

/*****************************************************************************/
// 	Description : get plmn status
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC MN_PHONE_PLMN_STATUS_E MMIAPIPHONE_GetPlmnStatus(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetPlmnStatus err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6559_112_2_18_2_45_13_126,(uint8*)"d", dual_sys);
        return PLMN_NO_SERVICE;
    }    
    
    return g_service_status[dual_sys].plmn_status;
}


/*****************************************************************************/
// 	Description : create input pin window
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
LOCAL MMI_RESULT_E CreateInputPinWindow(APP_MN_SIM_GET_PIN_IND_T sim_get_pin_ind)
{    
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T alert_win = PHONE_PIN_ALERT_WIN_ID;
    
    //SCI_TRACE_LOW:"mmiphone:CreateInputPinWindow dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6576_112_2_18_2_45_13_127,(uint8*)"d", sim_get_pin_ind.dual_sys);
    
    if (MNSIM_PIN1 == sim_get_pin_ind.pin_num)
    {
        MMK_CloseWin(alert_win);
        
        if (sim_get_pin_ind.pin_is_blocked)//PIN被锁，要求输入PUK
        {
            if (MMK_IsOpenWin(PHONE_PUK_INPUT_WIN_ID))
            {
                //提示错误窗口,PUK错误!
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PUK_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlocked);
                
                //清空password
                GUIEDIT_ClearAllStr(PHONE_PUK_EDITBOX_CTRL_ID);
            }
            else
            {
                //提示错误窗口,PIN被锁!
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN_BLOCKED,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinBlocked);
            }
        }
        else//PIN没有被锁，要求输入PIN
        {            
            if (MMK_IsOpenWin(PHONE_PIN_INPUT_WIN_ID))
            {
                //提示错误窗口,PIN 错误!
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PIN_ERROR,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_win,PNULL,MMIPUB_SOFTKEY_ONE,HandlePin1Maxtimes);

                //清空password
                GUIEDIT_ClearAllStr(PHONE_PIN_EDITBOX_CTRL_ID);
            }
            else
            {
                // 打开输入PIN的界面
                MMK_CreatePubEditWin((uint32*)PHONE_PIN_INPUTING_WIN_TAB,PNULL);
            }
        }
    }
    
    // 关闭等待窗口
    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);
    
    return (result);
}

#if defined(MMI_MULTI_SIM_SYS_SINGLE) || defined(MMI_MULTI_SIM_SYS_DUAL)
/*****************************************************************************/
// Description : get dual sim status
// Global resource dependence : 
// Author: bruce.chi
// Note: This API could only be used in SINGLE-SIM or DUAL SIM project,
// If in trisim or quad sim project,Please use another api(eg:MMIAPIPHONE_GetSimOkNum...) or other method.
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_GetDualSimStatus(BOOLEAN *is_sim1_ok_ptr, BOOLEAN *is_sim2_ok_ptr)
{
    if(PNULL == is_sim1_ok_ptr  ||PNULL == is_sim2_ok_ptr)
    {
        //SCI_TRACE_LOW:"GetSimAndServiceString: the is_sim1_ok_ptr=%d || is_sim2_ok_ptr =%d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6636_112_2_18_2_45_13_128,(uint8*)"dd", is_sim1_ok_ptr, is_sim2_ok_ptr);
        return FALSE;
    }
    
    if (MMIAPISET_GetFlyMode())
    {
        *is_sim1_ok_ptr = FALSE;
        *is_sim2_ok_ptr = FALSE;
    }
    else
    {
        *is_sim1_ok_ptr = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1);
        
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        *is_sim2_ok_ptr = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_2);
#else
        *is_sim2_ok_ptr = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1);
#endif
    }
    
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetDualSimStatus, is_sim1_ok=%d, is_sim2_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6656_112_2_18_2_45_13_129,(uint8*)"dd", *is_sim1_ok_ptr, *is_sim2_ok_ptr);
    return TRUE;
}
#endif

/*****************************************************************************/
// 	Description : IsNeedSetGPRSAttachMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetGPRSAttachMode(void)
{
    uint32 i = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (!s_is_sim_present_ind[i])//(!s_is_power_on_sim_cnf[i] && !s_handle_pin_status.is_handling_pin[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSAttachMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSAttachMode(void)
{
#ifndef WIN32
    uint32 i =0;
#ifdef MMI_DUALMODE_ENABLE
    static BOOLEAN is_need_set = TRUE;//Set only when Power ON
#endif

    if (!IsNeedSetGPRSAttachMode())
    {
        return;
    }

#ifdef MMI_DUALMODE_ENABLE
    //SCI_TRACE_LOW:"MMIAPIENG_GetFtaFlag() = %d"
    //set gprs manual attach mode
    if(!is_need_set)
    {
        SCI_TRACE_LOW("MMIAPIPHONE_SetGPRSAttachMode attach has set");
        return;
    }
    is_need_set = FALSE;
#ifdef ENGTD_SUPPORT  
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6695_112_2_18_2_45_13_130,(uint8*)"d", MMIAPIENG_GetFtaFlag());
    if (MMIAPIENG_GetFtaFlag() && !MNPHONE_GetGprsAttachMode())
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MNPHONE_SetGprsAttachModeEx(i, TRUE);
        }
        }
    else
#endif
    {
        //多卡存在或者需要时连接模式
        if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1
            || (!MMIAPICONNECTION_isGPRSNeedAutoAttach()))
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                MNPHONE_SetGprsAttachModeEx(i, TRUE);
                MNGPRS_SetEnergySavingMode(i, FALSE);

            }
        }
        else
        {
            MMICONNECTION_GPRS_SETTINGS_E grps_status = MMIAPICONNECTION_GetGPRSStatus();
        
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                MNPHONE_SetGprsAttachModeEx(i, FALSE);

                if (MMICONNECTION_GPRS_ALWAYS_LC == grps_status)
                {
                    MNGPRS_SetEnergySavingMode(i, TRUE);
                    SCI_TRACE_LOW("MNGPRS_SetEnergySavingMode TRUE by default");
                }
                else
                {
                    MNGPRS_SetEnergySavingMode(i, FALSE);

                }
            }
        }
    }
#else
    //set gprs manual attach mode
#ifdef MMI_GPRS_SUPPORT
    if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1
        || (!MMIAPICONNECTION_isGPRSNeedAutoAttach()))
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MNPHONE_SetGprsAttachModeEx(i, TRUE);
        }
    }
    else
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MNPHONE_SetGprsAttachModeEx(i, FALSE);
        }
    }
#else
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        MNPHONE_SetGprsAttachModeEx(i, TRUE);
    }
#endif
#endif
#endif
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSAttach
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSAttach(void)
{
    uint16 sys_ok = 0;

    if (MMIAPISET_GetMultiSysSettingNum(&sys_ok, 1) > 1)
    {
        //gprs manual, do nothing
    }
    else
    {
        if(MMIAPICONNECTION_isGPRSSwitchedOff())
        {
            return;
        }
        MNPHONE_GprsAttachEx((MN_DUAL_SYS_E)sys_ok, MN_PHONE_GPRS_ATTACH);
    }
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSDetach
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSDetach(void)
{
#ifndef WIN32
#ifdef MMI_DUALMODE_ENABLE
#ifdef ENGTD_SUPPORT 
    if (!MMIAPIENG_GetFtaFlag())
#endif
#endif
    {
#if defined(MMI_MULTI_SIM_SYS_SINGLE)
#ifdef MODEM_SUPPORT_SPRD
        MMIAPIPDP_GprsRetach(MN_DUAL_SYS_1);
#else
        SCI_TRACE_LOW("MMIAPIPHONE_SetGPRSDetach() Do Nothing.");
#endif
#else
        uint32 i =0;
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
        
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetGPRSDetach e_dualsys_setting = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6802_112_2_18_2_45_14_131,(uint8*)"d", e_dualsys_setting);

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (e_dualsys_setting & (0x01 << i))
            {
#ifdef MODEM_SUPPORT_SPRD
                MMIAPIPDP_GprsRetach(i);
#endif
            }
        }
#endif
    }
#endif
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSDetachEx
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSDetachEx(MN_DUAL_SYS_E dual_sys)
{
#ifndef WIN32    
    //SCI_TRACE_LOW:"MMIAPIPHONE_SetGPRSDetachEx dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6823_112_2_18_2_45_14_132,(uint8*)"d", dual_sys);

#ifdef MMI_DUALMODE_ENABLE
#ifdef ENGTD_SUPPORT 
    if (!MMIAPIENG_GetFtaFlag())
#endif
#endif
    {
#ifdef MODEM_SUPPORT_SPRD
        MMIAPIPDP_GprsRetach(dual_sys);
    #endif
    }
#endif
}

/*****************************************************************************/
// 	Description : start up ps according to setting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_StartUpPsAccordingToSetting(BOOLEAN is_power_on, BOOLEAN is_init_sim)
{
    uint32 i =0;
    if (is_power_on)
    {
#ifdef MULTI_SIM_GSM_CTA_SUPPORT
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
#endif

#ifdef MMI_SIM_LOCK_SUPPORT
    	/*l4初始化需要一些sim_lock的信息，在此初始化这部分信息
		以免在特别的启动情况下有问题，比如release版本assert启动*/
        if(!MNPHONE_SIMLockDataIntegrityCheck())
        {         
            /*sim lock data error,the ME alert "sim Locked" then power off*/
            MMIPHONE_SIMLockedOpenShutOffWin();
            return FALSE;
        }
        else
        {
            MN_SIM_LOCK_STATUS_T* sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
            
            /*初始化SIM Lock Data*/
            SCI_MEMSET(sim_unlock_data,0x00,sizeof(MN_SIM_LOCK_STATUS_T));
            MMIAPIPHONE_UpdateSIMUnlockData();
        }
#endif        

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        s_query_sim_status = SIM_QUERY_STATUS_START;//开始查询SIM状态
#endif
        s_is_power_flag = TRUE;
        
        //SCI_TRACE_LOW:"MMIAPIPHONE_StartUpPsAccordingToSetting, start power on sim"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6854_112_2_18_2_45_14_133,(uint8*)"");
        
        //设置默认状态
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MMIPHONE_SetSimStatus(i, SIM_NOT_INITED);

            s_is_power_on_sim_cnf[i] = FALSE;

#ifdef MULTI_SIM_GSM_CTA_SUPPORT
            if (is_init_sim)
            {
                if (e_dualsys_setting & (0x01 << i))
                {
                    MNPHONE_PowerOnSimEx(i, TRUE);
                }
                else
                {
                    MNPHONE_PowerOnSimEx(i, FALSE);
                }
            }
            else
            {
                MNPHONE_PowerOnSimEx(i, FALSE);
            }
#else

#ifdef UAL_TELE_SUPPORT
            ual_tele_sim_power_on(i, is_init_sim);
#else
            MNPHONE_PowerOnSimEx(i, is_init_sim);
#endif

#endif
        }
    }
    else
    {
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
        
        //SCI_TRACE_LOW:"MMIAPIPHONE_StartUpPsAccordingToSetting, e_dualsys_setting=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6888_112_2_18_2_45_14_134,(uint8*)"d", e_dualsys_setting);

        MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
        
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            s_is_network_status_ready[i] = FALSE;

            if ((e_dualsys_setting & (0x01 << i))
                && MMIAPIPHONE_GetSimExistedStatus(i))
            {
                if(i != PHONE_GetSimNeedEmergencyPs()) 
                {
                    SCI_TRACE_LOW("MMIAPIPHONE_StartUpPsAccordingToSetting1 PowerOnPsByUsim sim id = %d", i);
                    MMIAPIPHONE_PowerOnPsByUsim(i);
                }
                else
                {
                    SCI_TRACE_LOW("MMIAPIPHONE_StartUpPsAccordingToSetting1 PowerOnPsForEmerge sim id = %d", i);
                    PowerOnPsForEmerge(i);
                    //break;??poweronò????¨μ?emergency ps?
                 }
            }
        }

        if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
            {
                if (e_dualsys_setting & (0x01 << i))
                {
                    MMIAPIPHONE_PowerOnPs((MN_DUAL_SYS_E)i);
                    break;
                }
            }
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : deactive ps according to setting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_DeactivePsAccordingToSetting(void)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;

    MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        MAIN_SetIdleRoamingState(i, FALSE);
        if(!MMIAPISET_IsCfuStatusSavedInSim(i))    
        {
            MAIN_SetIdleForwardState(i, FALSE);
        }
        MAIN_SetIdleGprsState(i, FALSE);
    }

    //set gprs manual attach mode and start up ps 
    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (!MMIAPIPHONE_GetSimExistedStatus(i) && 0 != MMIAPISET_GetMultiSysSettingNum(PNULL, 0))
        {
            continue;
        }
        if (e_dualsys_setting & (0x01 << i))
        {
            MMIAPIPHONE_PowerOffPs(i);
        }
        else
        {
            MMIAPIPHONE_SetIsPsDeactiveCnf(i, TRUE);
        }
        
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : PowerOnPsForEmerge
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL void PowerOnPsForEmerge(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN  is_fly_mode_on = FALSE;

    if (MMIPHONE_PS_POWER_ON_NORMAL == s_ps_ready[dual_sys]
        ||MMIPHONE_PS_POWER_ON_EMERGE == s_ps_ready[dual_sys] )
    {
        //SCI_TRACE_LOW:"PowerOnPsForEmerge, s_ps_ready[%d]=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6968_112_2_18_2_45_14_135,(uint8*)"dd", dual_sys, s_ps_ready[dual_sys]);

        return;
    }

    is_fly_mode_on = MMIAPISET_GetFlyMode();

    if (is_fly_mode_on)
    {
        //SCI_TRACE_LOW:"PowerOnPsForEmerge Fly mode!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_6977_112_2_18_2_45_14_136,(uint8*)"");

        return;
    }
    
    s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_EMERGE;
    s_is_network_status_ready[dual_sys] = FALSE;

    if(!MNPHONE_IsPsPowerOnEx(dual_sys))
    {
        MMIAPIPHONE_SetIsPsReady(dual_sys, FALSE);
        MNPHONE_PowerOnPsEx(dual_sys);
    }

}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_PowerOnPs
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
void MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN  is_fly_mode_on = FALSE;
    SCI_TRACE_LOW("MMIAPIPHONE_PowerOnPs--Enter");

    if (MMIPHONE_PS_POWER_ON_NORMAL == s_ps_ready[dual_sys])
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOnPs, dual_sys=%d ps is already"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7000_112_2_18_2_45_14_137,(uint8*)"d", dual_sys);
#ifdef DROPDOWN_NOTIFY_SHORTCUT //bug1975552
       NotifyCenter_ClearWaitStateForPs();
#endif
        return;
    }

    is_fly_mode_on = MMIAPISET_GetFlyMode();

    if (is_fly_mode_on)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOnPs Fly mode!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7009_112_2_18_2_45_14_138,(uint8*)"");
#ifdef DROPDOWN_NOTIFY_SHORTCUT //bug1975552
        NotifyCenter_ClearWaitStateForPs();
#endif
        return;
    }
    s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_NORMAL;
    s_is_network_status_ready[dual_sys] = FALSE;

    if(!MNPHONE_IsPsPowerOnEx(dual_sys))
    {
        MMIAPIPHONE_SetIsPsReady(dual_sys, FALSE);
        MNPHONE_PowerOnPsEx(dual_sys);
    }
    MMIAPIPHONE_SetGPRSAttachMode();
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_PowerOffPs
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
void MMIAPIPHONE_PowerOffPs(MN_DUAL_SYS_E dual_sys)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOffPs, dual_sys=%d,s_ps_ready=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7030_112_2_18_2_45_14_139,(uint8*)"dd", dual_sys, s_ps_ready[dual_sys]);

    if (MMIPHONE_PS_POWER_ON_NONE == s_ps_ready[dual_sys])//ps not ready
    {
        return;
    }

    s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_NONE;
    
    s_is_network_status_ready[dual_sys] = FALSE;

    MAIN_SetIdleRoamingState(dual_sys, FALSE);
    if(!MMIAPISET_IsCfuStatusSavedInSim(dual_sys))    
    {
        MAIN_SetIdleForwardState(dual_sys, FALSE);
    }
    MAIN_SetIdleGprsState(dual_sys, FALSE);
    
    MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, FALSE);
#if defined(MMI_SMSCB_SUPPORT)
    ClearCbEnable(dual_sys);
#endif
#ifdef MODEM_SUPPORT_SPRD
#ifdef MMI_VOLTE_SUPPORT
#ifndef WIN32
    if (MNPHONE_IsImsRegister(dual_sys))
    {
        MMIPHONE_DeregisterIms(dual_sys);
    }
    else
#endif
#endif
#endif
    if(MNPHONE_IsPsPowerOnEx(dual_sys))
    {
        MNPHONE_PowerOffPsEx(dual_sys);
    }
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetIsPsDeactiveCnf
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIsPsDeactiveCnf(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ps_deactive_cnf)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetIsPsDeactiveCnf err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7060_112_2_18_2_45_14_140,(uint8*)"d", dual_sys);
        return;
    }
    
    s_is_ps_deactive_cnf[dual_sys] = is_ps_deactive_cnf;
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsDeactiveCnf
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsPsDeactiveCnf(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetIsPsDeactiveCnf err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7075_112_2_18_2_45_14_141,(uint8*)"d", dual_sys);
        return FALSE;
    }

    SCI_TRACE_LOW("MMIAPIPHONE_GetIsPsDeactiveCnf dual sys: %d, is_ps_ready: %d", dual_sys, s_is_ps_deactive_cnf[dual_sys]);

    return s_is_ps_deactive_cnf[dual_sys];
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetIsPsReady
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIsPsReady(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ps_ready)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_SetIsPsReady err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7090_112_2_18_2_45_14_142,(uint8*)"d", dual_sys);
        return;
    }

    SCI_TRACE_LOW("MMIAPIPHONE_SetIsPsReady dual sys: %d, is_ps_ready: %d", dual_sys, is_ps_ready);

    s_is_ps_ready[dual_sys] = is_ps_ready;
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsReady
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsPsReady(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_GetIsPsReady err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7105_112_2_18_2_45_14_143,(uint8*)"d", dual_sys);
        return FALSE;
    }
    
    return s_is_ps_ready[dual_sys];
}

/*****************************************************************************/
// 	Description : get current handling Pin Ind sim id(response)
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetCurResponsePinSimID(void)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetCurResponsePinSimID:sim_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7118_112_2_18_2_45_14_144,(uint8*)"d", s_handle_pin_status.cur_handling_sim_id);
    
    return s_handle_pin_status.cur_handling_sim_id;
}

/*****************************************************************************/
// 	Description : get current handling sim id(operate pin)
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetCurHandlingPinSimID(void)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetCurHandlingPinSimID:sim_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7129_112_2_18_2_45_14_145,(uint8*)"d", s_cur_handling_sim_id);
    return s_cur_handling_sim_id;
}

/*****************************************************************************/
// 	Description : set current handling sim id(operate pin)
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetCurHandlingPinSimID(MN_DUAL_SYS_E  cur_sim_id)
{
    s_cur_handling_sim_id = cur_sim_id;
}

/*****************************************************************************/
//  Description : set current handling sim info
//  Parameter: [In] None
//             [Out] None
//             [Return] s_handle_pin_status.sim_pin_ind[s_handle_pin_status.cur_handling_sim_id]
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APP_MN_SIM_GET_PIN_IND_T MMIPHONE_GetCurHandlingPinSimInfo(void)
{
    SCI_TRACE_LOW("MMIPHONE_GetCurHandlingPinSimInfo:sim_id=%d",s_handle_pin_status.cur_handling_sim_id);
    return s_handle_pin_status.sim_pin_ind[s_handle_pin_status.cur_handling_sim_id];
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
void MMIAPIPHONE_InitTDOrGsm(void)
{
#ifdef MMI_DUALMODE_ENABLE
#ifndef WIN32
    uint32 i = 0;
    MN_GMMREG_RAT_E configue_rat = MN_GMMREG_RAT_UNKNOWN; 

    //SCI_TRACE_LOW:"MMIAPIPHONE_InitTDOrGsm configue_rat=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7152_112_2_18_2_45_14_146,(uint8*)"d", configue_rat);

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        configue_rat = MNPHONE_GetConfiguredRATEx(i);
        switch(configue_rat)
        {
        case MN_GMMREG_RAT_GPRS:
            s_is_TD[i] = MMI_GMMREG_RAT_GPRS;
            break;

        case MN_GMMREG_RAT_TD:
            s_is_TD[i] = MMI_GMMREG_RAT_TD;
            break;
            
        case MMI_GMMREG_RAT_LTE:
            s_is_TD[i] = MMI_GMMREG_RAT_LTE;
            break;
            
        default:
            s_is_TD[i] = MMI_GMMREG_RAT_UNKNOWN;
            break;
        }
    }

    MAIN_UpdateIdleSignalState();
#endif
#endif
}

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
void MMIAPIPHONE_TDOrGsm(APP_MN_NETWORK_STATUS_IND_T     network_status_ind)
{
#ifdef  MMI_DUALMODE_ENABLE
    switch(network_status_ind.rat) 
    {
    case MN_GMMREG_RAT_GPRS:
        s_is_TD[network_status_ind.dual_sys] = MMI_GMMREG_RAT_GPRS;
        break;
        
    case MN_GMMREG_RAT_3G:
        s_is_TD[network_status_ind.dual_sys] = MMI_GMMREG_RAT_3G;
        break;
        
    case MN_GMMREG_RAT_LTE:
        s_is_TD[network_status_ind.dual_sys] = MN_GMMREG_RAT_LTE;
        break;
        
    case MN_GMMREG_RAT_UNKNOWN:
        s_is_TD[network_status_ind.dual_sys] = MMI_GMMREG_RAT_UNKNOWN;
        break;
        
    default:
        break;
    }
#endif
}
/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
PUBLIC MMI_GMMREG_RAT_E MMIAPIPHONE_GetTDOrGsm(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_DUALMODE_ENABLE
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        return s_is_TD[dual_sys];
    }
    else
    {
        uint32 i = 0;

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (MMI_GMMREG_RAT_3G == s_is_TD[i])
            {
                return MMI_GMMREG_RAT_3G;
            }
            
            if (MMI_GMMREG_RAT_LTE == s_is_TD[i])
            {
                return MMI_GMMREG_RAT_LTE;
            }
        }

        return MMI_GMMREG_RAT_GPRS;
    }
#else
    return MMI_GMMREG_RAT_GPRS;
#endif
}


/*****************************************************************************/
// 	Description :get current service status
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetCurServiceStatus(
                                            MN_DUAL_SYS_E dual_sys,
                                            PHONE_SERVICE_STATUS_T *status_ptr
                                            )
{
    SCI_MEMCPY(status_ptr,&g_service_status[dual_sys],sizeof(PHONE_SERVICE_STATUS_T));
}

/*****************************************************************************/
// 	Description :get current service status
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC PHONE_SERVICE_STATUS_T* MMIAPIPHONE_GetServiceStatus(void)
{
    return g_service_status;
}


/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note: modify by yongwei.he 20070621
/*****************************************************************************/
void MMIAPIPHONE_StopSTTDOrGsm(void)
{
//macro replace
//#ifdef MMI_STREAMING_ENABLE
#ifdef STREAMING_SUPPORT_NEXTREAMING
    MMI_WIN_ID_T  focus_win_id;
    focus_win_id = MMK_GetFocusWinId();
    
    if(MMIST_MAIN_PLAY_WIN_ID == focus_win_id)
    {
        MMIAPIST_Close();
    }
    for (focus_win_id = MMIST_MAIN_PLAY_WIN_ID;focus_win_id < MMIST_MAX_WIN_ID;focus_win_id++)
    {
        MMK_CloseWin(focus_win_id);    
    }
#endif
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPresentInd(
                                     DPARAM param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_SIM_PRESENT_IND_T sim_present_ind = *(APP_MN_SIM_PRESENT_IND_T *)param;

    if(sim_present_ind.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleSimPresentInd:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7263_112_2_18_2_45_14_147,(uint8*)"d", sim_present_ind.dual_sys);
        return MMI_RESULT_FALSE;
    }

    //SCI_TRACE_LOW:"HandleSimPresentInd:dual_sys:%d, sim_sms_num:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7267_112_2_18_2_45_14_148,(uint8*)"dd", sim_present_ind.dual_sys, sim_present_ind.sms_num);

    s_is_sim_present_ind[sim_present_ind.dual_sys] = TRUE;

    MMIAPISMS_SetMaxSimSmsNum(sim_present_ind.dual_sys, sim_present_ind.sms_num);

    MMIAPIPHONE_SetSimExistedStatus(sim_present_ind.dual_sys, sim_present_ind.is_sim_present);

    MMIAPIPHONE_SetGPRSAttachMode();

    //stk refresh type = SIMAT_RF_RESET(4) need power on ps.
    if (4 == MMIAPISTK_GetStkRefreshType(sim_present_ind.dual_sys)) 
    {
        MMIAPISTK_SetStkRefreshType(sim_present_ind.dual_sys, 0);

        MMIAPIPHONE_PowerOnPsByUsim(sim_present_ind.dual_sys);

        MMIAPISTK_CloseRefreshWaitingWin();
    }

    return result;
}

#ifdef MMI_LDN_SUPPORT

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLdnReadyInd(
                                     DPARAM param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_LND_READY_IND_T ldn_ready_ind = *(APP_MN_LND_READY_IND_T *)param;

    //SCI_TRACE_LOW:"HandleLdnReadyInd:dual_sys:%d,is_lnd_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7292_112_2_18_2_45_15_149,(uint8*)"dd", ldn_ready_ind.dual_sys, ldn_ready_ind.is_lnd_ok);

    //Just remember dual_sys,and wait until when...do it
    if (ldn_ready_ind.dual_sys>=MMI_DUAL_SYS_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMIAPIIDLE_IdleWinIsOpen())
    {
        MMIAPICL_InitCallInfo(ldn_ready_ind.dual_sys);
    }
    else
    {
        s_ldn_flag |=0x01<<(ldn_ready_ind.dual_sys);
    }    

    return result;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPHONE_CheckLdnReadyInd(void)
{
    MN_DUAL_SYS_E dual_sys=0;   

    while(s_ldn_flag)
    {
        if(s_ldn_flag&0x01)
        {
            MMIAPICL_InitCallInfo(dual_sys);
        }
        s_ldn_flag =s_ldn_flag>>1;        
        dual_sys++;
    }
    return MMI_RESULT_TRUE;
}
#endif

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSimPlugInInd(
                                     DPARAM param
                                     )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_SIM_PLUG_IN_IND_T sim_plug_in_ind = *(APP_MN_SIM_PLUG_IN_IND_T *)param;

    MMIAPIPHONE_PowerOffPs(sim_plug_in_ind.dual_sys); // 解决未插卡开机，插入SIM卡后无法成功注网的问题
    //lzk begin 2018.07.25
    SCI_TRACE_LOW("lzk log. HandleSimPlugInInd(), dual_sys = %d.", sim_plug_in_ind.dual_sys);
    //MMIPHONE_PowerOnSIM(sim_plug_in_ind.dual_sys);
    //lzk end 2018.07.25

    if(sim_plug_in_ind.dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleSimPlugInInd:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7313_112_2_18_2_45_15_150,(uint8*)"d", sim_plug_in_ind.dual_sys);
        return MMI_RESULT_FALSE;
    }
    //SCI_TRACE_LOW:"HandleSimPlugInInd:dual_sys:%d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7316_112_2_18_2_45_15_151,(uint8*)"d", sim_plug_in_ind.dual_sys);
    
    MMIPHONE_SetSimStatus(sim_plug_in_ind.dual_sys, SIM_NOT_INITED);

    MMIAPIPHONE_SetSimExistedStatus(sim_plug_in_ind.dual_sys, TRUE);

    NotifySIMPlugInd(sim_plug_in_ind.dual_sys, MMIPHONE_NOTIFY_SIM_PLUG_IN);

    return result;
}

/*****************************************************************************/
// 	Description : Handle SIM Plug Alert Win Msg
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMPlugAlertWinMsg(
                                                                                              MMI_WIN_ID_T win_id,
                                                                                              MMI_MESSAGE_ID_E 	msg_id,
                                                                                              DPARAM param
                                                                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    LOCAL MN_DUAL_SYS_E plug_sim_id = MN_DUAL_SYS_1;
    LOCAL MMIPHONE_SIM_PLUG_EVENT_E plug_event = MMIPHONE_NOTIFY_SIM_PLUG_MAX;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        plug_sim_id = s_plug_sim_id;
        plug_event = s_plug_event;
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    case MSG_CLOSE_WINDOW:
        if (plug_sim_id == s_plug_sim_id && plug_event == s_plug_event)
        {
            //MMIAPIPHONE_SetStandByMode(s_plug_sim_id, s_plug_event);//bug889326
        }

        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Notify SIM Plug Ind
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifySIMPlugInd(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event)
{
    MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *cursor = PNULL;
    MMI_WIN_ID_T alert_win_id = PHONE_SIM_PLUG_ALERT_WIN_ID;
    MMI_WIN_PRIORITY_E win_priority = WIN_ONE_LEVEL;

    //SCI_TRACE_LOW:"NotifySIMPlugInd dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7381_112_2_18_2_45_15_152,(uint8*)"dd", dual_sys, notify_event);

    if (dual_sys >= MMI_DUAL_SYS_MAX || notify_event >= MMIPHONE_NOTIFY_SIM_PLUG_MAX)
    {
        return FALSE;
    }

    if (!s_is_power_flag)
    {
        //SCI_TRACE_LOW:"NotifySIMPlugInd Intial not start, donot handle!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7390_112_2_18_2_45_15_153,(uint8*)"");

        return FALSE;
    }

    MMIDEFAULT_TurnOnBackLight();

    s_plug_sim_id = dual_sys;
    s_plug_event = notify_event;

    MMK_CloseWin(alert_win_id);

    if (MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID)
        || MMK_IsOpenWin(PHONE_POWER_OFF_WIN_ID))
    {
        win_priority = WIN_ONE_LEVEL;
    }
    else
    {
        win_priority = WIN_THREE_LEVEL;
    }

    if (MMIPHONE_NOTIFY_SIM_PLUG_IN == notify_event)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SIM_PLUG_IN, TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, &win_priority, MMIPUB_SOFTKEY_ONE, HandleSIMPlugAlertWinMsg);
#ifdef BBM_ROLE_SUPPORT
        if( BBM_MODE_PHONE == Bbm_GetBTMode())
        {
            s_is_need_poweron_modem = TRUE; // 解决sim卡插入后不能注网的问题
        }
#endif
        MMIPHONE_SetPinPukState(MMIPHONE_PIN_PUK_STATE_WAIT);
        MMIPHONE_OpenPinPukWin();
        //add code by wangwp when hot plug in the sim card
        //MMIAPIPHONE_PowerOffPs(s_plug_sim_id);//bug878920
        //MMIAPIPHONE_PowerOnPs(s_plug_sim_id);
        SCI_TRACE_LOW("NotifySIMPlugInd--hot plug in the sim card--notify_event=%d, s_plug_sim_id=%d", notify_event, s_plug_sim_id);
    }
    else if (MMIPHONE_NOTIFY_SIM_PLUG_OUT == notify_event)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SIM_PLUG_OUT, TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, &win_priority, MMIPUB_SOFTKEY_ONE, HandleSIMPlugAlertWinMsg);
        //add code by wangwp when hot plug out the sim card
        if(FALSE == ual_tele_radio_get_fly_mode_state())//bug2080829 in fly mode not need to change ps_ready
        {
            s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_EMERGE;//bug1954723 power on ps by ATC,mmi need update state
        }
        SCI_TRACE_LOW("NotifySIMPlugInd--hot plug out the sim card--notify_event=%d, s_plug_sim_id=%d", notify_event, s_plug_sim_id);
    }

    cursor = s_mmiphone_sim_plug_subscriber_list_ptr;
    while(PNULL != cursor)
    {
        cursor->notify_info.notify_func(dual_sys, notify_event, NULL);
        cursor = cursor->next;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : register SIM Plug Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_RegisterSIMPlugNotifyInfo(MMIPHONE_SIM_PLUG_NOTIFY_INFO_T *notify_info_ptr)
{
    BOOLEAN return_val = FALSE;
    MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *cursor = PNULL;

    if(PNULL != notify_info_ptr)
    {
        if(PNULL == s_mmiphone_sim_plug_subscriber_list_ptr)
        {
            s_mmiphone_sim_plug_subscriber_list_ptr = SCI_ALLOC_APP(sizeof(MMIPHONE_SIM_PLUG_NOTIFY_LIST_T));
            SCI_MEMSET(s_mmiphone_sim_plug_subscriber_list_ptr, 0, sizeof(MMIPHONE_SIM_PLUG_NOTIFY_LIST_T));
            SCI_MEMCPY(&(s_mmiphone_sim_plug_subscriber_list_ptr->notify_info), notify_info_ptr, sizeof(MMIPHONE_SIM_PLUG_NOTIFY_INFO_T));
        }
        else
        {
            cursor = s_mmiphone_sim_plug_subscriber_list_ptr;
            while(PNULL != cursor->next)
            {
                if(cursor->notify_info.module_id == notify_info_ptr->module_id)
                {
                    return FALSE;   //already register
                }
                cursor = cursor->next;
            }
            if(cursor->notify_info.module_id == notify_info_ptr->module_id)
            {
                return FALSE;   //already register
            }
            cursor->next = SCI_ALLOC_APP(sizeof(MMIPHONE_SIM_PLUG_NOTIFY_LIST_T));
            SCI_MEMSET(cursor->next, 0, sizeof(MMIPHONE_SIM_PLUG_NOTIFY_LIST_T));
            SCI_MEMCPY(&(cursor->next->notify_info), notify_info_ptr, sizeof(MMIPHONE_SIM_PLUG_NOTIFY_INFO_T));
        }

        return_val = TRUE;
    }

    return return_val;
}

/*****************************************************************************/
// 	Description : Unregister SIM Plug Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_UnregisterSIMPlugNotifyInfo(uint32 module_id)
{
    MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *cursor = PNULL;
    MMIPHONE_SIM_PLUG_NOTIFY_LIST_T *cursor_free = PNULL;

    cursor = s_mmiphone_sim_plug_subscriber_list_ptr;

    if(PNULL != cursor)
    {
        if(cursor->notify_info.module_id == module_id)
        {
            s_mmiphone_sim_plug_subscriber_list_ptr = cursor->next;
            SCI_FREE(cursor);
        }
        else
        {
            while(PNULL != cursor->next)
            {
                if (cursor->next->notify_info.module_id == module_id)
                {
                    cursor_free = cursor->next;
                    cursor->next = cursor_free->next;
                    SCI_FREE(cursor_free);
                    break;
                }
                cursor = cursor->next;
            }
        }
    }
}
#endif

/*****************************************************************************/
// 	Description : Power On SIM
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnSIM(MN_DUAL_SYS_E dual_sys)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOnSIM, start power on sim dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7515_112_2_18_2_45_15_154,(uint8*)"d", dual_sys);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    s_query_sim_status = SIM_QUERY_STATUS_START;//开始查询SIM状态
#endif
    s_is_power_on_sim_cnf[dual_sys] = FALSE;
    
    MMIPHONE_SetSimStatus(dual_sys, SIM_NOT_INITED);

#ifdef UAL_TELE_SUPPORT
    ual_tele_sim_power_on(dual_sys,TRUE);
#else
    MNPHONE_PowerOnSimEx(dual_sys, TRUE);
#endif
}

/*****************************************************************************/
// 	Description : Deactive PS
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_DeactivePS(MN_DUAL_SYS_E dual_sys)
{
    //SCI_TRACE_LOW:"MMIAPIPHONE_DeactivePS, dual_sys=%d,s_ps_ready=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7533_112_2_18_2_45_15_155,(uint8*)"dd", dual_sys, s_ps_ready);

    s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_NONE;
    s_is_network_status_ready[dual_sys] = FALSE;
    MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_NO_SIM);
    
    MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, FALSE);
#if defined(MMI_SMSCB_SUPPORT)
    ClearCbEnable(dual_sys);
#endif
    MNPHONE_DeactivePsEx(dual_sys);
}

/*****************************************************************************/
// 	Description : Power off  PS or SIM
//	Global resource dependence : none
//  Author: chunjie.liu
//	Note:if ps is ready,power off ps,else power off sim
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PoweroffPsOrSim(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("MMIAPIPHONE_PoweroffPSorSim, dual_sys=%d,s_ps_ready=%d",dual_sys, s_ps_ready[dual_sys]);

    if(MMIPHONE_PS_POWER_ON_NONE != s_ps_ready[dual_sys]) //ps is ready
    {
        s_ps_ready[dual_sys] = MMIPHONE_PS_POWER_ON_NONE;
        s_is_network_status_ready[dual_sys] = FALSE; 
        MMIAPIPHONE_SetIsPsDeactiveCnf(dual_sys, FALSE);
#if defined(MMI_SMSCB_SUPPORT)
        ClearCbEnable(dual_sys);
#endif
        s_phone_is_power_off_req[dual_sys] = TRUE;
#ifdef MMI_VOLTE_SUPPORT
#ifdef MODEM_SUPPORT_SPRD
#ifndef WIN32
        if (MNPHONE_IsImsRegister(dual_sys))
        {
            MMIPHONE_DeregisterIms(dual_sys);
        }
        else
#endif
#endif
#endif
        {
            if(MNPHONE_IsPsPowerOnEx(dual_sys))
            {
                MNPHONE_PowerOffPsEx(dual_sys);
            }
        }
    }
    else //ps is not ready
    {
        MMIPHONE_SetSimStatus(dual_sys, SIM_STATUS_NO_SIM);
        MNPHONE_PowerOffSimEx(dual_sys);
    }       
}

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
// 	Description : HandleSetStandByModeQueryWinMsg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStandByModeQueryWinMsg( 
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E    msg_id, 
                                           DPARAM              param
                                           )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMIAPISET_OpenMultiSimStandbySettingsWindow();

        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : HandleSetStandbyMdoeWaitingWindow
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStandbyMdoeWaitingWindow(
                                         MMI_WIN_ID_T    win_id, // ???ID
                                         MMI_MESSAGE_ID_E   msg_id, // ???????ID
                                         DPARAM             param   // ???????
                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    //SCI_TRACE_LOW:"HandleSetStandbyMdoeWaitingWindow msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7595_112_2_18_2_45_15_156,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:       
        break;

    case MSG_APP_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void OpenSetStandByModeWaiting(void)
{
    MMI_WIN_PRIORITY_E win_priority = WIN_ONE_LEVEL;
    MMI_WIN_ID_T wait_win_id = PHONE_SET_STANDBY_MODE_WAITING_WIN_ID;
    MMI_STRING_T wait_text = {0};

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);

    if (MMIAPICC_IsInState(CC_IN_CALL_STATE)
        || MMIAPISET_IsNetworkConnecting()
#ifdef BROWSER_SUPPORT
        || MMIAPIBROWSER_IsRunning()
#endif
        )
    {
        win_priority = WIN_LOWEST_LEVEL;
    }
    else
    {
        win_priority = WIN_ONE_LEVEL;
    }

    MMIPUB_OpenWaitWin(1, &wait_text, PNULL, PNULL, wait_win_id, IMAGE_NULL,
                                        ANIM_PUBWIN_WAIT, win_priority, MMIPUB_SOFTKEY_NONE, HandleSetStandbyMdoeWaitingWindow);

    MMK_SetWinDisplayStyleState(wait_win_id, WS_DISABLE_RETURN_WIN, TRUE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void ImplementStandByModeAsync(void)
{
    //SCI_TRACE_LOW:"ImplementStandByModeAsync s_plug_sim_id=%d, s_plug_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7662_112_2_18_2_45_15_157,(uint8*)"dd", s_plug_sim_id, s_plug_event);

    if (s_plug_sim_id >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    if (MMK_IsOpenWin(PHONE_SET_STANDBY_MODE_WAITING_WIN_ID))
    {
        switch (s_plug_event)
        {
        case MMIPHONE_NOTIFY_SIM_PLUG_IN:
            MMIAPIPHONE_PowerOnSIM(s_plug_sim_id);
            break;

        default:
            break;
        }
    }

    MMK_CloseWin(PHONE_SET_STANDBY_MODE_WAITING_WIN_ID);
}

/*****************************************************************************/
// 	Description : set standby mode
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetStandByMode(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E plug_event)
{
    uint32 i = 0;
    BOOLEAN is_need_standby[MMI_DUAL_SYS_MAX] = {0};//是否需要待机
    uint32 sys_standby_num = 0;
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MMI_WIN_ID_T query_win_id = PHONE_SET_STANDBY_MODE_QUERY_WIN_ID;

    //SCI_TRACE_LOW:"MMIAPIPHONE_SetStandByMode dual_sys=%d, plug_event=%d, e_sys_setting=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7698_112_2_18_2_45_15_158,(uint8*)"ddd",dual_sys, plug_event, e_dualsys_setting);

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (e_dualsys_setting & (0x01 << i))
        {
            is_need_standby[i] = TRUE;
            sys_standby_num ++;
        }
    }

    MMIPUB_CloseQuerytWin(&query_win_id);

    MMK_CloseWin(PHONE_SET_STANDBY_MODE_WAITING_WIN_ID);

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMIAPIPHONE_SetIdleGprsState(i);
    }

    switch (plug_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
        MMIAPISET_SetIsQueryCfu(FALSE,dual_sys);

#ifdef MMI_GPRS_SUPPORT
        if (!MMIAPIPDP_PsIsPdpLinkExist())
        {
            if ((!MMIAPICONNECTION_isGPRSNeedAutoAttach())
                 || (sys_standby_num > 1))
            {
                MMIAPIPHONE_SetGPRSDetach();
            }
        }
#endif

        if (e_dualsys_setting & (0x01 << dual_sys))//待机卡插入
        {
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i ++)
            {
                if ((e_dualsys_setting & (0x01 << i))
                    && !MMIAPIPHONE_GetSimExistedStatus(i)
                    && (dual_sys != i))//关闭不存在SIM卡的PS
                {
                    MMIAPIPHONE_PowerOffPs((MN_DUAL_SYS_E)i);
                }
            }
        }

        OpenSetStandByModeWaiting();

        MMIAPIPHONE_DeactivePS(dual_sys);
        break;

    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
#ifdef MMI_GPRS_SUPPORT
#ifndef WIN32
        MMIAPIPHONE_SetGPRSAttachMode();

        MMIAPIPDP_ResetPdp(dual_sys);

        if (!MMIAPIPDP_PsIsPdpLinkExist())
        {
            if (1 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))
            {
                if ((!MMIAPICONNECTION_isGPRSNeedAutoAttach()))
                {
                    MMIAPIPHONE_SetGPRSDetach();
                }
                else
                {
                    MMIAPIPHONE_SetGPRSAttach();
                }
            }
        }
#endif
#endif

        if(!MMIAPISET_IsCfuStatusSavedInSim(dual_sys))    
        {
            MAIN_SetIdleForwardState(dual_sys, FALSE);
        }
        MAIN_SetIdleRoamingState(dual_sys, FALSE);

        if (MMIAPIPHONE_GetPSReadyNum(PNULL, 0) > 1)
        {
            MMIAPIPHONE_PowerOffPs(dual_sys);
#ifdef QQ_SUPPORT
            MMIAPIQQ_UpdateSimSelect(dual_sys);
#endif
        }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if ((1 == sys_standby_num) && is_need_standby[dual_sys])
        {
            MMIPUB_OpenQueryWinByTextId(TXT_SIM_PLUG_OUT_SET_STANDBY_MODE_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, HandleSetStandByModeQueryWinMsg);
        }
#endif
        break;

    default:
        break;
    }
}
#endif

/*****************************************************************************/
// 	Description : according to ps status, judge if wap & mms is available
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPsServiceAvailble(void)
{
#ifdef WIN32
    return TRUE;
#else
#ifdef MMI_DUALMODE_ENABLE  //
    return TRUE;
#else
    return (ATC_GetModemStatus() != ATC_MODEM_ONDIAL);
#endif
#endif
}

/*****************************************************************************/
// 	Description :get current data service type
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_DATA_SERVICE_TYPE_E MMIAPIPHONE_GetDataServiceType( MN_DUAL_SYS_E dual_sys )
{
    MMIPHONE_DATA_SERVICE_TYPE_E service_type = NO_DATA_SERVICE_TYPE;
    if (g_service_status[dual_sys].hsdpa_support)
    {
        service_type = HSDPA_DATA_SERVICE_TYPE;
    }
    else if (g_service_status[dual_sys].edge_support)
    {
        service_type = EDGE_DATA_SERVICE_TYPE;
    }
    else if ((MMI_GMMREG_RAT_GPRS_AND_3G == g_service_status[dual_sys].rat)
            ||(MMI_GMMREG_RAT_TD == g_service_status[dual_sys].rat))
    {
        service_type = TD_DATA_SERVICE_TYPE;
    }
    else if(g_service_status[dual_sys].gprs_support)
    {
        service_type = GPRS_DATA_SERVICE_TYPE;
    }
    else
    {
        //service_type = NO_DATA_SERVICE_TYPE;
    }

    //SCI_TRACE_LOW:"MMIAPIPHONE_GetDataServiceType: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7851_112_2_18_2_45_16_159,(uint8*)"d", service_type);
    return service_type;
}

/*****************************************************************************/
//  Description : Notify Rx level Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void NotifyRxLevelChange(MN_DUAL_SYS_E dual_sys, uint8 rx_level)
{
    LOCAL uint8 s_notify_rx_level[MMI_DUAL_SYS_MAX] = {0};/*lint !e64*/

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"NotifyRxLevelChange error dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7865_112_2_18_2_45_16_160,(uint8*)"d", dual_sys);
        return;
    }

    //SCI_TRACE_LOW:"NotifyRxLevelChange rx_level=%d, s_notify_rx_level=%d, PlmnStatus=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7870_112_2_18_2_45_16_161,(uint8*)"ddd",rx_level, s_notify_rx_level[dual_sys], MMIAPIPHONE_GetPlmnStatus(dual_sys));

    if (0 == rx_level && 0 != s_notify_rx_level[dual_sys])
    {
        if (PLMN_NORMAL_GSM_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GPRS_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GSM_GPRS_BOTH == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        {
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);

            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);
        }
    }
    else if (0 != rx_level && 0 == s_notify_rx_level[dual_sys])
    {
        if (PLMN_NORMAL_GSM_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        {
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
        }
        else if (PLMN_NORMAL_GPRS_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        {
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
        }
        else if(PLMN_NORMAL_GSM_GPRS_BOTH == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        {
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
            NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
        }
    }

    s_notify_rx_level[dual_sys] = rx_level;
}

/*****************************************************************************/
//  Description : Notify PLMN Status Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL void NotifyPlmnStatusChange(MN_DUAL_SYS_E dual_sys, MN_PHONE_PLMN_STATUS_E plmn_status)
{
    LOCAL MN_PHONE_PLMN_STATUS_E s_notify_plmn_status[MMI_DUAL_SYS_MAX] = {0};/*lint !e64*/

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    //SCI_TRACE_LOW:"NotifyPlmnStatusChange plmn_status=%d, s_notify_plmn_status=%d, rssiLevel=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_7917_112_2_18_2_45_16_162,(uint8*)"ddd",plmn_status, s_notify_plmn_status[dual_sys], g_service_status[dual_sys].rssiLevel);

    if (0 < g_service_status[dual_sys].rssiLevel)
    {
        switch (plmn_status)
        {
        case PLMN_NORMAL_GSM_ONLY:
            switch (s_notify_plmn_status[dual_sys])
            {
            case PLMN_NORMAL_GSM_ONLY:
                //plmn status not change
                break;

            case PLMN_NORMAL_GPRS_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);

                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
                break;

            case PLMN_NORMAL_GSM_GPRS_BOTH:
                //NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);
                break;

            default:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
                break;
            }
            break;

        case PLMN_NORMAL_GPRS_ONLY:
            switch (s_notify_plmn_status[dual_sys])
            {
            case PLMN_NORMAL_GSM_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);

                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
                break;

            case PLMN_NORMAL_GPRS_ONLY:
                //plmn status not change
                break;

            case PLMN_NORMAL_GSM_GPRS_BOTH:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);
                break;

            default:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
                break;
            }
            break;

        case PLMN_NORMAL_GSM_GPRS_BOTH:
            switch (s_notify_plmn_status[dual_sys])
            {
            case PLMN_NORMAL_GSM_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
                break;

            case PLMN_NORMAL_GPRS_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);
                break;

            case PLMN_NORMAL_GSM_GPRS_BOTH:
                //plmn status not change
                break;

            default:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_AVAIL);

                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_AVAIL);
                break;
            }
            break;

        default:
            switch (s_notify_plmn_status[dual_sys])
            {
            case PLMN_NORMAL_GSM_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);
                break;

            case PLMN_NORMAL_GPRS_ONLY:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);
                break;

            case PLMN_NORMAL_GSM_GPRS_BOTH:
                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL);

                NotifyServiceChange(dual_sys, MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL);
                break;

            default:
                //need not notify
                break;
            }
            break;
        }
    }

    s_notify_plmn_status[dual_sys] = plmn_status;
}

/*****************************************************************************/
//  Description : Notify Service Change
//  Global resource dependence : none
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NotifyServiceChange(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event)
{
    MMIPHONE_NOTIFY_LIST_T *cursor = PNULL;

    //SCI_TRACE_LOW:"NotifyServiceChange dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8028_112_2_18_2_45_16_163,(uint8*)"dd", dual_sys, notify_event);

    if (dual_sys >= MMI_DUAL_SYS_MAX || notify_event >= MMIPHONE_NOTIFY_EVENT_MAX)
    {
        return FALSE;
    }

    cursor = s_mmiphone_subscriber_list_ptr;
    while(PNULL != cursor)
    {
        cursor->notify_info.notify_func(dual_sys, notify_event, NULL);
        cursor = cursor->next;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : register PS/CS Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_RegisterNotifyInfo(MMIPHONE_NOTIFY_INFO_T *notify_info_ptr)
{
    BOOLEAN return_val = FALSE;
    MMIPHONE_NOTIFY_LIST_T *cursor = PNULL;

    if(PNULL != notify_info_ptr)
    {
        if(PNULL == s_mmiphone_subscriber_list_ptr)
        {
            s_mmiphone_subscriber_list_ptr = SCI_ALLOC_APP(sizeof(MMIPHONE_NOTIFY_LIST_T));
            SCI_MEMSET(s_mmiphone_subscriber_list_ptr, 0, sizeof(MMIPHONE_NOTIFY_LIST_T));
            SCI_MEMCPY(&(s_mmiphone_subscriber_list_ptr->notify_info), notify_info_ptr, sizeof(MMIPHONE_NOTIFY_INFO_T));
        }
        else
        {
            cursor = s_mmiphone_subscriber_list_ptr;
            while(PNULL != cursor->next)
            {
                if(cursor->notify_info.module_id == notify_info_ptr->module_id)
                {
                    return FALSE;   //already register
                }
                cursor = cursor->next;
            }
            if(cursor->notify_info.module_id == notify_info_ptr->module_id)
            {
                return FALSE;   //already register
            }
            cursor->next = SCI_ALLOC_APP(sizeof(MMIPHONE_NOTIFY_LIST_T));
            SCI_MEMSET(cursor->next, 0, sizeof(MMIPHONE_NOTIFY_LIST_T));
            SCI_MEMCPY(&(cursor->next->notify_info), notify_info_ptr, sizeof(MMIPHONE_NOTIFY_INFO_T));
        }

        return_val = TRUE;
    }

    return return_val;
}

/*****************************************************************************/
// 	Description : Unregister PS/CS Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_UnregisterNotifyInfo(uint32 module_id)
{
    MMIPHONE_NOTIFY_LIST_T *cursor = PNULL;
    MMIPHONE_NOTIFY_LIST_T *cursor_free = PNULL;

    cursor = s_mmiphone_subscriber_list_ptr;
    if(PNULL != cursor)
    {
        if(cursor->notify_info.module_id == module_id)
        {
            s_mmiphone_subscriber_list_ptr = cursor->next;
            SCI_FREE(cursor);
        }
        else
        {
            while(PNULL != cursor->next)
            {
                if (cursor->next->notify_info.module_id == module_id)
                {
                    cursor_free = cursor->next;
                    cursor->next = cursor_free->next;
                    SCI_FREE(cursor_free);
                    break;
                }
                cursor = cursor->next;
            }
        }
    }
    
}

/*****************************************************************************/
// 	Description : Is CS Available
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsCSAvail(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_cs_avail = FALSE;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return is_cs_avail;
    }

    if (0 < g_service_status[dual_sys].rssiLevel
        && (PLMN_NORMAL_GSM_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GSM_GPRS_BOTH == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        && !g_service_status[dual_sys].is_slecting)
    {
        is_cs_avail = TRUE;
    }

    return is_cs_avail;
}

/*****************************************************************************/
// 	Description : Is PS Available
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPSAvail(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_ps_avail = FALSE;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsPSAvail error dual_sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8157_112_2_18_2_45_17_164,(uint8*)"d", dual_sys);
        return is_ps_avail;
    }

    if (0 < g_service_status[dual_sys].rssiLevel
        && (PLMN_NORMAL_GSM_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GPRS_ONLY == MMIAPIPHONE_GetPlmnStatus(dual_sys)
            || PLMN_NORMAL_GSM_GPRS_BOTH == MMIAPIPHONE_GetPlmnStatus(dual_sys))
        && !g_service_status[dual_sys].is_slecting)
    {
        is_ps_avail = TRUE;
    }

    return is_ps_avail;
}

#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
/*****************************************************************************/
// 	Description : MMIAPIPHONE_PowerOnPsEx
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnPsEx(void)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (e_dualsys_setting & (0x01 << i))
        {
            if (MMIAPIIDLE_IdleWinIsOpen()
                && s_is_power_on_sim_cnf[i]
                && MMIAPIPHONE_GetSimExistedStatus(i))
            {
                MMIAPIPHONE_PowerOnPsByUsim((MN_DUAL_SYS_E)i);
            }
            else
            {
                //SCI_TRACE_LOW:"MMIAPIPHONE_PowerOnPsEx s_is_power_on_sim_cnf[%d]=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8195_112_2_18_2_45_17_165,(uint8*)"dd", i, s_is_power_on_sim_cnf[i]);
            }
        }
    }
}

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsReadyBySys
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsPsReadyBySys(MN_DUAL_SYS_E sys_type)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = 0;
    BOOLEAN is_ps_ready = FALSE;

    e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    if (e_dualsys_setting & (1 << sys_type))
    {
        is_ps_ready = (MMIPHONE_PS_POWER_ON_NONE != s_ps_ready[sys_type]);
    }

    return is_ps_ready;
}
#endif /* !defined(MMI_MULTI_SIM_SYS_SINGLE) */

#ifdef DPHONE_SUPPORT 
/*****************************************************************************/
// 	Description : Get Pin Lock Hook Handfree Status
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIPHONE_GetPinLockHookHandfreeStatus(void)
{
    return g_pin_lock_hookhandfree;
}

/*****************************************************************************/
// 	Description : Set Pin Lock Hook Handfree Status
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
void MMIAPIPHONE_SetPinLockHookHandfreeStatus(BOOLEAN hook_locked)
{
    g_pin_lock_hookhandfree = hook_locked;
}
#endif

#if defined(MMI_UPDATE_TIME_SUPPORT)
/*****************************************************************************/
// 	Description : 当前SIM是否注册网络 成功
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsRegNetSuccess(MN_DUAL_SYS_E cur_sim_index)
{
    BOOLEAN    result = FALSE;
    
    if(cur_sim_index < MMI_DUAL_SYS_MAX)
    {
#ifndef WIN32
        if (s_is_network_status_ready[cur_sim_index])
#endif
        {
            result = TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsRegNetSuccess err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8260_112_2_18_2_45_17_166,(uint8*)"d", cur_sim_index);
    }

    return result;
}

/*****************************************************************************/
// 	Description : 是否所有SIM卡搜网 成功
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsAllSIMSelectNetSucc(void)
{
    BOOLEAN    result = TRUE;
    uint32  i = 0;
    
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        //SIM not avalible is also "status ready"
#if 1 //ndef WIN32
        if (MMIAPIPHONE_IsSimAvailable(i)  && !s_is_network_status_ready[i])
#endif
        {
            result = FALSE;
            break;
        }
    }

    SCI_TRACE_LOW("[mmiphone]: IsAllSIMSelectNetSucc result = %d, i = %d", result, i);

    return result;
}
/*****************************************************************************/
// 	Description : 判断SIM是否Attach gprs 成功
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsAttachGprsSuccess(MN_DUAL_SYS_E cur_sim_index)
{
    BOOLEAN    result = FALSE;
    
    if(cur_sim_index >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_IsAttachGprsSuccess err dual sys: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8276_112_2_18_2_45_17_167,(uint8*)"d", cur_sim_index);
        return FALSE;        
    }
    
    if ((PLMN_NORMAL_GPRS_ONLY == g_service_status[cur_sim_index].plmn_status)
        || (PLMN_NORMAL_GSM_GPRS_BOTH == g_service_status[cur_sim_index].plmn_status))
    {
        result = TRUE;
    }
    
    return result;
}
#endif /*MMI_UPDATE_TIME_SUPPORT*/

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
//+++ mdy by qing.yu@for cr238585
/*****************************************************************************/
// 	Description : get sim card and plmn service status string text id
//	Global resource dependence : 
//  Author: qing.yu
//	Note:  
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIPHONE_OpeartorTextID(MN_DUAL_SYS_E dual_sys)
{
    MMI_TEXT_ID_T	idle_str_id = TXT_NULL;
    SIM_STATUS_E sim_status = MMIAPIPHONE_GetSimStatus(dual_sys);

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIPHONE_OpeartorTextID param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8303_112_2_18_2_45_17_168,(uint8*)"");
        return idle_str_id;
    }

    //if sim error then display sim string in sim_status_string
    if (sim_status < SIM_STATUS_MAX)
    {
        if (sim_status != SIM_STATUS_OK) //sim not ready
        {
            idle_str_id = s_sim_status_string[sim_status];

        }
        else
        {
            idle_str_id = GetNetWorkNameId(
                dual_sys,
                &g_service_status[dual_sys].plmn,
                g_service_status[dual_sys].plmn_status,
                g_service_status[dual_sys].is_slecting
                );
        }
    }
	return idle_str_id;
}

PUBLIC BOOLEAN MMIAPIPHONE_GetIsNetworkRoaming(void)
{
	MN_DUAL_SYS_E dual_sys = MMIAPISET_GetActiveSim();

    return g_service_status[dual_sys].isRoaming;
}
//--- mdy by qing.yu@for cr238585
#endif

/*****************************************************************************/
// 	Description : Get Update NetworkStatus Win
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIPHONE_GetUpdateNetworkStatusWin(void)
{
    return s_need_update_network_status_win;
}

/*****************************************************************************/
// 	Description : is network selecting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(MMI_HANDLE_T win_handle)
{
    s_need_update_network_status_win = win_handle;    
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_UpdateNetworkStatusWin(void)
{
    if (MMK_IsFocusWin(s_need_update_network_status_win))
    {
        MMK_PostMsg(s_need_update_network_status_win, MSG_FULL_PAINT, PNULL, 0);
    }
}

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : 在SIM Lock 解锁次数达到限制的情况下弹出"SIM Restricted"窗口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSIMLocked(
                                    MMI_WIN_ID_T     win_id, 	// Pin boloked窗口的ID
                                    MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                    DPARAM 		        param		// 相应消息的参数
                                    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO	lcd_dev_info = {0};
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_APP_CANCEL://pin被锁输入界面不允许返回
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : create input sim lock window
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:  
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPHONE_CreateInputSimLockWindow(APP_MN_SIM_NOT_READY_IND_T sim_not_ready_ind)
{    
    MN_SIM_LOCK_STATUS_T *sim_unlock_data = MMIAPIPHONE_GetSIMUnlockConfigDataPtr();
    MMI_OPERATE_SIM_LOCK_T   *opreate_sim_lock_ptr =SCI_ALLOCAZ(sizeof(MMI_OPERATE_SIM_LOCK_T));
    uint32 unlock_trials = 0;
    uint32 time_out_value = 0;
    int32 left_trials = 0;
    MMI_RESULT_E result =MMI_RESULT_TRUE;
    
    if(PNULL == opreate_sim_lock_ptr)
    {
        //SCI_TRACE_LOW:"mmiphone:CreateInputSimLockWindow opreate_sim_lock_ptr = NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8423_112_2_18_2_45_17_169,(uint8*)"");
        return result;
    }
    
    //SCI_TRACE_LOW:"mmiphone:CreateInputSimLockWindow dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8427_112_2_18_2_45_17_170,(uint8*)"d", sim_not_ready_ind.dual_sys);
    
    SCI_MEMSET(opreate_sim_lock_ptr,0x00,sizeof(MMI_OPERATE_SIM_LOCK_T));
    opreate_sim_lock_ptr->dual_sys = sim_not_ready_ind.dual_sys;
    opreate_sim_lock_ptr->lock_type = MMI_SIM_NETWORK_LOCK<<((uint32)sim_not_ready_ind.reason - (uint32)MNSIM_ERROR_SIM_NETWORT_LOCKED);
    opreate_sim_lock_ptr->is_enable = TRUE;
    
    /*remember SIM Lock type in case power off the phone*/
    SCI_MEMSET(&s_sim_lock_operate_value,0x00,sizeof(MMI_OPERATE_SIM_LOCK_T));
    s_sim_lock_operate_value.dual_sys = opreate_sim_lock_ptr->dual_sys;
    s_sim_lock_operate_value.lock_type = opreate_sim_lock_ptr->lock_type;
    s_sim_lock_operate_value.is_enable = opreate_sim_lock_ptr->is_enable;
    
    MMIAPIPHONE_GetSIMLockRemainTrialsTimer(opreate_sim_lock_ptr->lock_type,&unlock_trials,&time_out_value);
    left_trials = sim_unlock_data->max_num_trials - unlock_trials;
    
    SCI_TRACE_LOW("MMIPHONE: MMIPHONE_CreateInputSimLockWindow, time_out_value = %d, left_trials = %d", time_out_value,left_trials);
    /*open sim unlock window*/
    if( 0 != time_out_value && 0 < left_trials)
    {
        if(!MMK_IsOpenWin(PHONE_SIM_LOCK_COUNTDOWN_WIN_ID))
        {
            MMK_CreateWin((uint32*)SIM_LOCK_COUNTDOWN_RUN_WIN_TAB,(ADD_DATA)opreate_sim_lock_ptr);
        }
        return MMI_RESULT_TRUE;//
    }
    else if(!MMIPHONE_GetIsSIMUnlocked() && 0 < left_trials)
    {
        if(!MMK_IsOpenWin( PHONE_SIM_LOCK_PASSWORD_INPUTING_WIN_ID))
        {
            MMK_CreatePubEditWin((uint32*)SIM_LOCK_PASSWORD_INPUTING_WIN_TAB,(ADD_DATA)opreate_sim_lock_ptr);
        }
        return MMI_RESULT_TRUE;//
    }      
    else if(0 >= left_trials)
    {
        MMI_WIN_ID_T alert_win_id = PHONE_SIM_LOCK_ALERT_WIN_ID;
        uint32 alert_time = 0;
        
        MMIPUB_OpenAlertWinByTextId(&alert_time,TXT_SIM_LOCKED,TXT_NULL,
                                                        IMAGE_PUBWIN_WARNING,&alert_win_id,
                                                        PNULL,MMIPUB_SOFTKEY_NONE,HandleSIMLocked);

        SCI_TRACE_LOW("MMIPHONE: alert_win_id = %d", alert_win_id);
      
        MMK_SetWinDisplayStyleState(alert_win_id, WS_DISABLE_RETURN_WIN, TRUE);
        SCI_FREE(opreate_sim_lock_ptr);
    }
    else
    {
        SCI_FREE(opreate_sim_lock_ptr);
    }    
    return (result);
}

/*****************************************************************************/
// 	Description : get handle sim lock status ptr for multi sim card
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_HANDLE_SIMLOCK_STATUS_T *MMIPHONE_GetHandleSIMLockStatus(void)
{
    return &s_handle_sim_lock_status;
}
/*****************************************************************************/
// 	Description : MMIPHONE_Get SIM lock Config Data Ptr
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MN_SIM_LOCK_STATUS_T *MMIAPIPHONE_GetSIMUnlockConfigDataPtr(void)
{
    return  &s_sim_unlock_config_data;
}

/*****************************************************************************/
// 	Description : MMIPHONE Update SIM Unlock Data if MMI update sim lock data to MN
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_UpdateSIMUnlockData(void)
{
    return MNPHONE_GetSIMLockStatus(&s_sim_unlock_config_data);
}
#endif

#ifdef MMI_SIM_LANGUAGE_SUPPORT
/*****************************************************************************/
// 	Description : 判断是哪张sim卡做优选语言。
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetPreferLangSim(void)
{
    return s_prefer_language_sim;
}
#endif

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// 	Description : Append All Plmn List ...
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AppendPlmnList(
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                MMI_CTRL_ID_T ctrl_id,                                                                               
                                                                                uint16 index
                                                                                )
{   
    GUILIST_ITEM_DATA_T item_data = {0};
    MN_PLMN_T plmn = {0};
    MMI_STRING_T plmn_name = {0};
    
    wchar plmn_code[MAX_MCC_MNC_DIGIT_DISPLAY +1] = {0};        
    uint16 plmn_code_len = MAX_MCC_MNC_DIGIT_DISPLAY;
    
    //SCI_TRACE_LOW:"MMIAPIPHONE_AppendPlmnList,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8534_112_2_18_2_45_18_171,(uint8*)"d",index);
    
    if(index < MAX_NETWORK_NUM -1 )
    {
        plmn.mcc = s_network_nametable[index + 1].mcc;
        plmn.mnc = s_network_nametable[index + 1].mnc;        
        if(s_network_nametable[index+1].mnc >= 100)
        {
            plmn.mnc_digit_num = 3; // mnc 的长度有可能为2 或3
        }
        else
        {
            plmn.mnc_digit_num = 2; // mnc 的长度有可能为2 或3
        }        

        //PLMN NAME Content.
        //get plmn name info    
        MMI_GetLabelTextByLang(s_network_nametable[index+1].name_index, &plmn_name);
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[0].item_data.text_buffer.wstr_len = plmn_name.wstr_len;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = plmn_name.wstr_ptr;

        //PLMN CODE Content .        
        //get plmn code info
        MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys), &plmn, plmn_code, &plmn_code_len, TRUE);
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[1].item_data.text_buffer.wstr_len = plmn_code_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = plmn_code;

        //set softkey
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;

        GUILIST_SetItemData( ctrl_id, &item_data, index );    
        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
    }
}

/*****************************************************************************/
// 	Description : Append prefer plmn List ...
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AppendPreferPlmnList(
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                MMI_CTRL_ID_T ctrl_id,
                                                                                MN_PLMN_T *prefer_network,
                                                                                uint16 index)
{    
    GUILIST_ITEM_DATA_T item_data = {0};
    MN_PLMN_T plmn = {0};
    MMI_STRING_T plmn_name = {0};        
    wchar plmn_name_arr[MAX_ONSHORTFORM_LEN+1]={0};
    wchar plmn_code[MAX_MCC_MNC_DIGIT_DISPLAY +1] = {0};
    uint16 plmn_code_len = MAX_MCC_MNC_DIGIT_DISPLAY;

    //SCI_TRACE_LOW:"MMIAPIPHONE_AppendPreferPlmnList,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8589_112_2_18_2_45_18_172,(uint8*)"d",index);

    if((PNULL!=prefer_network) && (index < MMISET_PREFER_PLMN_LIST_MAX) )
    {
        plmn.mcc = prefer_network[index].mcc;
        plmn.mnc = prefer_network[index].mnc;
        plmn.mnc_digit_num = prefer_network[index].mnc_digit_num;

        //PLMN NAME Content.
        //get plmn name info
        plmn_name.wstr_ptr =(wchar *)plmn_name_arr;
		plmn_name.wstr_len = MAX_ONSHORTFORM_LEN;
        MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys), &plmn, plmn_name.wstr_ptr, &plmn_name.wstr_len, FALSE);
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[0].item_data.text_buffer.wstr_len = plmn_name.wstr_len;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = plmn_name.wstr_ptr;

        //PLMN CODE Content .        
        //get plmn code info
        MMIAPIPHONE_GenPLMNDisplay(MMIAPIPHONE_GetTDOrGsm(dual_sys), &plmn,plmn_code, &plmn_code_len, TRUE);
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
        item_data.item_content[1].item_data.text_buffer.wstr_len = plmn_code_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = plmn_code;
        
        //set softkey
        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;

        GUILIST_SetItemData( ctrl_id, &item_data, index );    
        GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
    }
}
#endif

/*****************************************************************************/
// 	Description : get Plmn List Num...
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPHONE_GetPlmnListNum(void)
{
    //s_network_nametable[]数组的第一项是test项，非实际的plmn。
    return MAX_NETWORK_NUM - 1;
}

/*****************************************************************************/
// 	Description :MMIAPIPHONE_GetPlmnInfoByIndex
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPlmnInfoByIndex(                                                                                
                                                                                MN_PLMN_T *prefer_network,
                                                                                uint32 index)
{
    if(PNULL == prefer_network )
    {
        //SCI_TRACE_LOW:"GetSimAndServiceString: the prefer_network =%d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8643_112_2_18_2_45_18_173,(uint8*)"d", prefer_network);
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"MMIAPIPHONE_GetPlmnInfoByIndex,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8647_112_2_18_2_45_18_174,(uint8*)"d",index);

    if( index < MAX_NETWORK_NUM)
    {
        prefer_network->mcc = s_network_nametable[index].mcc;
        prefer_network->mnc = s_network_nametable[index].mnc;
        if( s_network_nametable[index].mnc >= 100)
        {
            prefer_network->mnc_digit_num = 3; /// set mnc length as 3...
        }
        else
        {
            prefer_network->mnc_digit_num = 2; /// set mnc length as 2...
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : ldn update confirm
//	Global resource dependence : none
//	Author:dave.ruan
//	Note:delete all/datlete both type is erase
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLDNUpdateCnf( DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_LND_UPDATE_CNF_T *ldn_update_cnf_ptr = (APP_MN_LND_UPDATE_CNF_T *)param;
    #if 0
    BOOLEAN  is_ldn_all_deleted = TRUE;
    uint32  i = 0;
    #endif
    
    if (!ldn_update_cnf_ptr)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"ccapp.c HandleLDNUpdateCnf,type= %d, result =%d, dual_sys= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8683_112_2_18_2_45_18_175,(uint8*)"ddd",ldn_update_cnf_ptr->operate_type,ldn_update_cnf_ptr->is_ok,ldn_update_cnf_ptr->dual_sys);
                                
    if(MN_LND_ERASE == ldn_update_cnf_ptr->operate_type)
    {
	    if(MMIAPICL_GetLdnIsDeleteAll() && ldn_update_cnf_ptr->is_ok)//liezhen如果底层返回非OK的状态，则直接提示用户失败，不再向底层发送请求了。
	    {
	        MMIAPICL_SetLdnIsDeleteAll(FALSE);
	        if(MMIAPICL_DeleteAllLdnInfo(MMICL_CALL_DIALED))
	        {
	            return result;
	        }
	    }
	       
	    MMK_CloseWin(MMICC_LDN_UPDATE_WAITING_WIN_ID);

	    if(ldn_update_cnf_ptr->is_ok)
	    {
	        //success
	        //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
	    }
	    else
	    {
	        //reject
	        MMIPUB_OpenAlertWarningWin(TXT_COMMON_DEL_FAIL);
	    }
	    //close progess waiting win
	    //CC_UpdateCurrentCallStatus(TRUE);
	}
    return result;
}
#endif

/*****************************************************************************/
// 	Description : 获取状态正常的SIM卡总数和具体卡的位置
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:   sim_idex_ptr :point to ok sim index,if null,do not set sim index
//              array_len :ok sim index len
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetAllSimStateOKCount(uint16 *sim_idex_ptr, uint16 array_len)
{
    uint8 active_sim_count = 0;
    int32 sim_index = MMI_DUAL_SYS_MAX;

    for (sim_index = MN_DUAL_SYS_1; sim_index < MMI_DUAL_SYS_MAX; sim_index++)
    {
        if (MMIAPIPHONE_IsSimOk(sim_index))
        {
            if ((sim_idex_ptr != NULL) && (active_sim_count < array_len))
            {
                sim_idex_ptr[active_sim_count] = sim_index;
            }

            active_sim_count++;
        }
    }
    //SCI_TRACE_LOW:"[mmiphone]: MMIAPIPHONE_GetAllSimStateOKCount, active_sim_count=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONE_8760_112_2_18_2_45_18_176,(uint8*)"d",active_sim_count);
    return active_sim_count;
}

/*****************************************************************************/
// 	Description : is ofm(Operational Feature Monitor) actived, if it is, then the ciphering off indicator should displaying 
//    when the call connection is not encrypted
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsOfmActived(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_ofm_actived = FALSE;

    if(MMI_DUAL_SYS_MAX  <= dual_sys )
    {
        SCI_TRACE_LOW("MMIAPIPHONE_IsOfmActived err dual sys: %d", dual_sys);
        return FALSE;
    }
    
    is_ofm_actived = s_sim_status[dual_sys].admin.ofm_actived;

    return is_ofm_actived;
}

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
// 	Description : Set the network name from csc
//	Global resource dependence : 
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetNetWorkNameByCSC(MMICUS_CSC_GENERALINFO_NETWORKINFO_T *csc_network_name_ptr,
                                                                                                            uint16 network_num)
{
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *temp_csc_network_ptr = PNULL;
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *cur_csc_network_ptr = PNULL;
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *temp_cur_csc_network_ptr = PNULL;

    if (PNULL == csc_network_name_ptr || 0 == network_num)
    {
        return FALSE;
    }

    if (PNULL != s_csc_network_name_ptr)
    {
        cur_csc_network_ptr = s_csc_network_name_ptr->csc_network_info;

        while (PNULL != cur_csc_network_ptr)
        {
            temp_csc_network_ptr = cur_csc_network_ptr;
            cur_csc_network_ptr = cur_csc_network_ptr->next;

            SCI_FREE(temp_csc_network_ptr);
            temp_csc_network_ptr = PNULL;
        }

        SCI_FREE(s_csc_network_name_ptr);
        s_csc_network_name_ptr = PNULL;
    }

    s_csc_network_name_ptr = (MMI_CSC_NETWORK_NAME_T *)SCI_ALLOCAZ(sizeof(MMI_CSC_NETWORK_NAME_T));

    if (PNULL == s_csc_network_name_ptr)
    {
        return FALSE;
    }

    s_csc_network_name_ptr->net_work_num = network_num;

    temp_cur_csc_network_ptr = csc_network_name_ptr;

    while (PNULL != temp_cur_csc_network_ptr)
    {
        cur_csc_network_ptr = (MMICUS_CSC_GENERALINFO_NETWORKINFO_T *)SCI_ALLOCAZ(sizeof(MMICUS_CSC_GENERALINFO_NETWORKINFO_T));

        if (PNULL == cur_csc_network_ptr)
        {
            return FALSE;
        }
        SCI_MEMCPY(cur_csc_network_ptr, temp_cur_csc_network_ptr, sizeof(MMICUS_CSC_GENERALINFO_NETWORKINFO_T));
        cur_csc_network_ptr->next = PNULL;
        temp_csc_network_ptr = s_csc_network_name_ptr->csc_network_info;

        if (PNULL == temp_csc_network_ptr)
        {
            s_csc_network_name_ptr->csc_network_info = cur_csc_network_ptr;
        }
        else
        {
            while (PNULL != temp_csc_network_ptr->next)
            {
                temp_csc_network_ptr = temp_csc_network_ptr->next;
            }
            temp_csc_network_ptr->next = cur_csc_network_ptr;
        }

        temp_cur_csc_network_ptr = temp_cur_csc_network_ptr->next;
     }    

    return TRUE;
}

/*****************************************************************************/
// 	Description : Get the network name from csc
//	Global resource dependence : 
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetNetWorkNameByCSC(uint16 mcc, 
                                                                                                            uint16 mnc,
                                                                                                            MMI_STRING_T *name_string_str)
{
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *cur_csc_network_ptr = PNULL;
    uint16 network_num = 0;
    
    if (PNULL == s_csc_network_name_ptr || PNULL == name_string_str)
    {
        return FALSE;
    }

    network_num = s_csc_network_name_ptr->net_work_num;
    cur_csc_network_ptr = s_csc_network_name_ptr->csc_network_info;
    while (PNULL != cur_csc_network_ptr && 0 < network_num--)
    {
        if (cur_csc_network_ptr->mcc == mcc && cur_csc_network_ptr->mnc == mnc)
        {
            name_string_str->wstr_len = strlen((char *)cur_csc_network_ptr->network_name);
            name_string_str->wstr_len = MIN(name_string_str->wstr_len, XML_TAG_NAME_MAX_LEN);
            name_string_str->wstr_ptr = (wchar *)SCI_ALLOCAZ((name_string_str->wstr_len + 1)*sizeof(wchar));
            if (PNULL == name_string_str->wstr_ptr)
            {
                return FALSE;
            }

            MMI_STRNTOWSTR(name_string_str->wstr_ptr, name_string_str->wstr_len, cur_csc_network_ptr->network_name, name_string_str->wstr_len, name_string_str->wstr_len);
            return TRUE;
        }
    }

    return FALSE;
}
#endif


#if (defined MMI_SIM_LOCK_SUPPORT) && (defined MMI_RESET_PHONE_AUTO_PIN)
/*****************************************************************************/
// 	Description : Set whether use pin in nv for SIM lock
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetUsePinInNV(
                      BOOLEAN  use_pin_in_nv)
{
    SCI_TRACE_LOW("MMIAPIPHONE_SetUsePinInNV use_pin_in_nv:%d", use_pin_in_nv);
    s_use_pin_in_nv = use_pin_in_nv;
}
#endif //#if defined MMI_SIM_LOCK_SUPPORT && defined MMI_RESET_PHONE_AUTO_PIN

/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527

LOCAL void InitDataCard(void)
{
    MN_DUAL_SYS_E sim_num = MN_DUAL_SYS_1;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
        && MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        sim_num = MMIAPISET_GetDataSimFromNV();
    }
    else if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        sim_num = MN_DUAL_SYS_2;
    }
    else
#endif
    {
        sim_num = MN_DUAL_SYS_1;
    }

    MNGPRS_SWDatatEx(sim_num);
    MMIAPISET_SaveDataSimToNV(sim_num);

    SCI_TRACE_LOW("InitDataCard sim_num = %d, s_sim_existed_status:%d", sim_num, s_sim_existed_status[sim_num]);
    return;
}

LOCAL void PHONE_InitTestModeBeforePowerOnPs(MN_DUAL_SYS_E dual_sys)
{
#ifdef MODEM_SUPPORT_SPRD
#ifndef WIN32
    uint8 one_slot_two_active = 0;
    uint8 multi_sys_work_mode = 0;
    uint8 current_lte_rat     = 0;
    uint8 s_current_umts_rat  = 0 ;
    uint8 multi_mode_sys_id   = 0xff;
    uint8 gsm_only_ps_id      = 0xff;
    uint8 sim1_test_mode = MN_PHONE_POWER_TEST_MODE_INVALID;
    uint8 sim2_test_mode = MN_PHONE_POWER_TEST_MODE_INVALID;
    /*bug839326 del*/
	uint32 i = 0; /*bug833310*/
    uint8 testmode[4]         =
    {
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID,
        MN_PHONE_POWER_TEST_MODE_INVALID
    };
    BOOLEAN update_lte_flag[MN_DUAL_SYS_MAX]= {0};
    uint8 lte_flag = 0xff;
    BOOLEAN is_first_bootup=FALSE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;

    if (TRUE == g_has_init_test_mode)
    {
        // 如果已经初始化过Test Mode，则直接返回
        SCI_TRACE_LOW("Init test mode has been done.");
        return;
    }

    InitDataCard();

    MMINV_READ(MMINV_SET_NETWORK_TYPE_MODE, &testmode, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        testmode[0] = MN_PHONE_POWER_TD_LTE_FIVE_MODE_ONE_ACTIVE;
        testmode[1] = MN_PHONE_POWER_TD_LTE_FIVE_MODE_ONE_ACTIVE;
        MMINV_WRITE(MMINV_SET_NETWORK_TYPE_MODE, &testmode);
    }
    sim1_test_mode = testmode[0];
    sim2_test_mode = testmode[1];
    multi_mode_sys_id = MNPHONE_GetMultiModeCard((int32)dual_sys);/*bug762721*/
    if (0xff == multi_mode_sys_id )
    {
        is_first_bootup = TRUE;
    }

    if (!MNPHONE_IsSupportTestModeEnum(sim1_test_mode, sim2_test_mode))
    {
        SCI_TRACE_LOW("MMIPHONE: ERR_OPERATION_NOT_SUPPORTED!");
        return;
    }

    if (s_is_ps_ready[MN_DUAL_SYS_1]
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        != s_is_ps_ready[MN_DUAL_SYS_2]
#endif
        )
    {
        MNPHONE_DspChange(sim1_test_mode,sim2_test_mode);
        MNPHONE_Local_SwitchData_Bytestmode(sim1_test_mode,sim2_test_mode);/*Bug583530*/

        if (FALSE == s_is_ps_ready[MN_DUAL_SYS_1])
        {
            MNPHONE_SetTestModeEx(sim1_test_mode, MN_DUAL_SYS_1);
            if (MN_DUAL_SYS_1 == MNPHONE_GetMultiModeCardFromTestMode(sim1_test_mode, sim2_test_mode))
            {
                MNPHONE_LteSaveTestMode(MN_DUAL_SYS_1, sim1_test_mode);
                MNPHONE_Set_Test_Mode_Param_Value(sim1_test_mode,MN_DUAL_SYS_1);
                update_lte_flag[MN_DUAL_SYS_1] = MNPHONE_GetLteFlagFromTestMode(sim1_test_mode);
            }
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else if (FALSE == s_is_ps_ready[MN_DUAL_SYS_2])
        {
            MNPHONE_SetTestModeEx(sim2_test_mode, MN_DUAL_SYS_2);
            if (MN_DUAL_SYS_2 == MNPHONE_GetMultiModeCardFromTestMode(sim1_test_mode,sim2_test_mode))
            {
                MNPHONE_LteSaveTestMode(MN_DUAL_SYS_2, sim2_test_mode);
                MNPHONE_Set_Test_Mode_Param_Value(sim2_test_mode,MN_DUAL_SYS_2);
                update_lte_flag[MN_DUAL_SYS_2] = MNPHONE_GetLteFlagFromTestMode(sim2_test_mode);
            }
        }
#endif
        else
        {
            SCI_TRACE_LOW("MMIPHONE: InitTestMode : No need to change value!");
            return;
        }

        for (i = 0; i < MN_DUAL_SYS_MAX; i++)
        {
            SCI_TRACE_LOW("MMIPHONE: InitTestMode update_lte_flag[%d]=%d, ", i, update_lte_flag[i]);
            if (update_lte_flag[i])
            {
                lte_flag = TRUE;
            }
            else
            {
                em_get_lte_enable_flag(&lte_flag);
            }

            MNPHONE_SetCpLteFlag(i, lte_flag);
        }
    }
    else
    {
        if (TRUE == s_is_ps_ready[MN_DUAL_SYS_1]
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            || TRUE == s_is_ps_ready[MN_DUAL_SYS_2]
#endif
            )
        {
            SCI_TRACE_LOW("MMIPHONE: InitTestMode  There has Ps power on");
            return;
        }

        if (FALSE == is_first_bootup)
        {
            MNPHONE_DspChange(sim1_test_mode, sim2_test_mode);
        }

        MNPHONE_SetTestModeEx(sim1_test_mode, MN_DUAL_SYS_1);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MNPHONE_SetTestModeEx(sim2_test_mode, MN_DUAL_SYS_2);
#endif
        /*Set the multi_mode_card_id

        SIM1 =no sim               set  multi_mode_card = 0
        sim0 =no sim sim1=no sim   set  multi_mode_card = 0
        SIM0 !=GSM&& SIM0 !=254    set  multi_mode_card = 0
        SIM0 ==GSM SIM1 ==GSM      set  multi_mode_card = 0
        SIM0 ==GSM SIM1 ==no sim   set  multi_mode_card = 0

        else
        set  multi_mode_card = 1
        */
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (((TRUE == is_first_bootup) && (MN_PHONE_POWER_TEST_MODE_INVALID==sim2_test_mode)) ||
            ((MN_PHONE_POWER_TEST_MODE_INVALID == sim1_test_mode) && (MN_PHONE_POWER_TEST_MODE_INVALID == sim2_test_mode)) ||
            (MN_DUAL_SYS_1 == MNPHONE_GetMultiModeCardFromTestMode(sim1_test_mode,sim2_test_mode)))
        {
            multi_mode_sys_id = MN_DUAL_SYS_1;
            MNPHONE_SetMultiModeCard(multi_mode_sys_id, FALSE);/*bug389498*/
            MNPHONE_Set_Test_Mode_Param_Value(sim1_test_mode, MN_DUAL_SYS_1);
            update_lte_flag[MN_DUAL_SYS_1] = MNPHONE_GetLteFlagFromTestMode(sim1_test_mode);
        }
        else
        {
            multi_mode_sys_id = MN_DUAL_SYS_2;
            MNPHONE_SetMultiModeCard(multi_mode_sys_id, FALSE);/*bug389498*/
            MNPHONE_Set_Test_Mode_Param_Value(sim2_test_mode,MN_DUAL_SYS_2);
            update_lte_flag[MN_DUAL_SYS_2] = MNPHONE_GetLteFlagFromTestMode(sim2_test_mode);
        }
        if ((MN_PHONE_POWER_GSM_ONLY==sim1_test_mode)&&(MN_PHONE_POWER_GSM_ONLY_MASTER_CARD==sim2_test_mode))
        {
            multi_mode_sys_id = MN_DUAL_SYS_2;
            MNPHONE_SetMultiModeCard(multi_mode_sys_id, FALSE);
        }
        else if ((MN_PHONE_POWER_GSM_ONLY==sim2_test_mode)&&(MN_PHONE_POWER_GSM_ONLY_MASTER_CARD==sim1_test_mode))
        {
            multi_mode_sys_id = MN_DUAL_SYS_1;
            MNPHONE_SetMultiModeCard(multi_mode_sys_id, FALSE);
        }
#else
        multi_mode_sys_id = MN_DUAL_SYS_1;
        MNPHONE_SetMultiModeCard(multi_mode_sys_id, FALSE);
        MNPHONE_Set_Test_Mode_Param_Value(sim1_test_mode,MN_DUAL_SYS_1);
        update_lte_flag[MN_DUAL_SYS_1] = MNPHONE_GetLteFlagFromTestMode(sim1_test_mode);
#endif

        if (MNPHONE_IsSupportCloudSimFeature()
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            || (MN_SIM_CARD_TYPE_NO_SIM != MNPHONE_GetSimCardType(MN_DUAL_SYS_1) && MN_SIM_CARD_TYPE_NO_SIM != MNPHONE_GetSimCardType(MN_DUAL_SYS_2))
#endif
            )/*bug857387*///BUG516593 bug594303
        {
            MNPHONE_SetWorkModeEx(20);//20 is dual model
        }
        else
        {
            MNPHONE_SetWorkModeEx(19);//19 is single model.
        }
        /*if multi_mode_card testmode is not support lte
          need to chang the lte_enable_flag_to_false
          else do nothing
        */
        for (i = 0; i < MN_DUAL_SYS_MAX ; i++)
        {
            if (update_lte_flag[i])
            {
                lte_flag = TRUE;
            }
            else
            {
                em_get_lte_enable_flag(&lte_flag);
            }
            MNPHONE_SetCpLteFlag(i,lte_flag);
        }

        if (5 == MNPHONE_GetProductForm())
        {
            if (MN_DUAL_SYS_1 == multi_mode_sys_id)
            {
                gsm_only_ps_id = MN_SYS_SIM_2;//use modem macro
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else if (MN_DUAL_SYS_2 == multi_mode_sys_id)
            {
                gsm_only_ps_id = MN_SYS_SIM_1;
            }
#endif
            SCI_TRACE_LOW("MMIPHONE: InitTestMode : sptestmodem set auto attach multimode_ps_id=%d,gsm_only_ps_id=%,is_support_lte_dsds=%d",
                          multi_mode_sys_id, gsm_only_ps_id, MNPHONE_IsSupportLteDSDS());

            if (MNPHONE_IsSupportLteDSDS() && is_first_bootup)
            {
                if(!MNGPRS_GetSpManuAttachFlagEx((MN_DUAL_SYS_E)multi_mode_sys_id))/*Bug850155*/
                {
                    gmm_api_set_ps_auto_attach_flag(multi_mode_sys_id, TRUE);
                    MNPHONE_SetGprsAttachModeEx((MN_DUAL_SYS_E)multi_mode_sys_id,FALSE);
                }

                if(!MNGPRS_GetSpManuAttachFlagEx((MN_DUAL_SYS_E)gsm_only_ps_id))/*Bug850155*/
                {
                    gmm_api_set_ps_auto_attach_flag(gsm_only_ps_id, TRUE);
                    MNPHONE_SetGprsAttachModeEx((MN_DUAL_SYS_E)gsm_only_ps_id, FALSE);
                }
            }
            else if (is_first_bootup) /*Bug531927*/
            {
                if(!MNGPRS_GetSpManuAttachFlagEx((MN_DUAL_SYS_E)multi_mode_sys_id))/*Bug850155*/
                {
                    gmm_api_set_ps_auto_attach_flag(multi_mode_sys_id, TRUE);
                    MNPHONE_SetGprsAttachModeEx((MN_DUAL_SYS_E)multi_mode_sys_id,FALSE);/*pclint bug337935*/
                }
                gmm_api_set_ps_auto_attach_flag(gsm_only_ps_id, FALSE);
                MNPHONE_SetGprsAttachModeEx((MN_DUAL_SYS_E)gsm_only_ps_id, TRUE);/*pclint bug337935*/
            }

            if (!MNPHONE_IsSupportLteDSDS())
            {
                CellSyncInfo_ResetParamInfoChangeCard(multi_mode_sys_id);/*bug747212*/
            }
        }
        MNPHONE_Local_SwitchData_Bytestmode(sim1_test_mode,sim2_test_mode);
        MNPHONE_LteSaveTestMode(MN_DUAL_SYS_1, sim1_test_mode);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MNPHONE_LteSaveTestMode(MN_DUAL_SYS_2, sim2_test_mode);
#endif
        MNPHONE_SetCurrentUmtsRatBeforeManulModePowerOn(dual_sys);   /*bug614633*/
        MNPHONE_UpdateL1WorkMode();/*bug914571*/
    }

    g_has_init_test_mode = TRUE;

    SCI_TRACE_LOW("MMIPHONE: InitTestMode Change successfully!");
#endif
#endif
    return;
}

/*****************************************************************************/
//  Description : Get is all sim ready to init ps
//  Global resource dependence :
//  Author:yanli.yang
//  Note:  return TRUE--allow to power on ps
//                FALSE--do not allow to power on ps
/*****************************************************************************/
LOCAL BOOLEAN PHONE_IsAllSimReadyToInitPs(void)
{
    int32 i = 0;

    //for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIPHONE_SIM_READY_TO_PS_REASON_NOT_READY == g_is_sim_ready_to_ps[MN_DUAL_SYS_1])
        {
            SCI_TRACE_LOW("PHONE_IsAllSimReadyToInitPs is NOT ready. SimId = %d.", i);
            return FALSE;
        }
    }

    SCI_TRACE_LOW("PHONE_IsAllSimReadyToInitPs return TRUE.");
    return TRUE;
}

/*****************************************************************************/
//  Description : Get sim card which need emergency ps
//  Global resource dependence :
//  Author:yanli.yang
//  Note:  return MN_DUAL_SYS_MAX--no sim card need emergency ps
//                MN_DUAL_SYS_1--sim card 0 need emergency ps
//                MN_DUAL_SYS_2--sim card 1 need emergency ps
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E PHONE_GetSimNeedEmergencyPs(void)
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_MAX;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)//cid604344
    {
        if (MMIPHONE_SIM_READY_TO_PS_REASON_READY_BY_PIN == g_is_sim_ready_to_ps[i])
        {
            SCI_TRACE_LOW("Have sim card to emergency ps. i = %d.", i);
            return i;
        }
    }

    SCI_TRACE_LOW("No sim card need emergency ps. return MN_DUAL_SYS_MAX.");
    return MN_DUAL_SYS_MAX;
}

/*****************************************************************************/
//  Description : Get sim card exited status extend
//  Global resource dependence :
//  Author:jiaxin.sun
//  Note:  return 0---no sim
//                1---only sim1
//                2---only sim2
//                3---sim1 & sim2
/*****************************************************************************/
LOCAL uint16 PHONE_GetSimExistedStatusEx(void)
{
    uint16 status = NONE_SIM_EXISTED;

    if (TRUE == MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
        status = status | SIM1_EXISTED;
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (TRUE == MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        status = status | SIM2_EXISTED;
    }
#endif
    return status;
}

/*****************************************************************************/
//  Description : power on ps
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] None
//  Author: yanli.yang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnPsByUsim(MN_DUAL_SYS_E dual_sys)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("s_sim_status[%d].is_usim = %d.", dual_sys, s_sim_status[dual_sys].is_usim);
#ifndef WIN32
#ifdef MODEM_SUPPORT_SPRD
#ifdef MMI_4G_SUPPORT
    if (TRUE == s_sim_status[dual_sys].is_usim)
    {
        SCI_TRACE_LOW("sim card = %d is usim card.", dual_sys);
        // 如果当前卡是usim卡，则需要先调用PS层的下面接口函数
        // 目前得到的信息是必须传MN_SIM_APPLN_TYPE_ISIM类型，且在收到APP_MN_SIM_APPLN_INIT_CNF消息后判断当类型为该类型时才启用PS
        result = MNSIM_SimApplnInit(dual_sys, MN_SIM_API_TASK_ID_APP, MN_SIM_APPLN_TYPE_ISIM);
    }
    else
#endif
    {
        SCI_TRACE_LOW("sim card = %d is not usim card.", dual_sys);
        // 如果当前卡不是usim卡，则直接调用原有的2G、3G启动ps接口函数
        MMIAPIPHONE_PowerOnPs(dual_sys);
    }
#elif MODEM_SUPPORT_RDA
    SCI_TRACE_LOW("sim card = %d power on ps.", dual_sys);
    MMIAPIPHONE_PowerOnPs(dual_sys);
#endif
#else
    MMIAPIPHONE_PowerOnPs(dual_sys);
#endif
}

LOCAL void PHONE_InitPsWhenAllSimReady(MN_DUAL_SYS_E readySimId, MMIPHONE_SIM_READY_TO_PS_REASON_E eReason)
{
    MN_DUAL_SYS_E pinSim = MN_DUAL_SYS_MAX;

    SCI_TRACE_LOW("enter PHONE_InitPsWhenAllSimReady. readySimId = %d.", readySimId);

    // 1、设置所需的flag
    g_is_sim_ready_to_ps[readySimId] = eReason;

    // 2、获取是否有需要emergency ps的sim card
    pinSim = PHONE_GetSimNeedEmergencyPs();

    // 3、判断是否允许启用PS
    if (TRUE == PHONE_IsAllSimReadyToInitPs())
    {
        // 4、初始化Test Mode
        PHONE_InitTestModeBeforePowerOnPs(readySimId); // 注意里面判断是否会二次调用
#ifndef BBM_ROLE_SUPPORT
        // 5、判断是否未插卡
        if (NONE_SIM_EXISTED == PHONE_GetSimExistedStatusEx())
        {
            SCI_TRACE_LOW("Power on SIM1 PS as no sim card.");
            // 启动卡1的正常PS
            MMIAPIPHONE_PowerOnPsByUsim(MN_DUAL_SYS_1);
        }
        else if (MN_DUAL_SYS_MAX != pinSim) // 判断是否需要启动Emergency PS
        {
            SCI_TRACE_LOW("Power on emergency ps of current handling sim id. cur_handling_sim_id = %d.", s_handle_pin_status.cur_handling_sim_id);
            // 如果需要启动Emergency PS，则启动当前handling pin的Emergency PS
            PowerOnPsForEmerge(pinSim);
        }
        else
        {
            int32 i = 0;

            SCI_TRACE_LOW("No need to power on emergency ps.");

            // 如果不需要启动Emergency PS
            for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
            {
                if (TRUE == MMIAPIPHONE_GetSimExistedStatus(i))
                {
                    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                    if (e_dualsys_setting & (0x01 << i))
                    {
                        // 如果卡存在，且没有禁用，则启动正常的PS
                        MMIAPIPHONE_PowerOnPsByUsim(i);
                    }
                    else
                    {
                        // 如果卡存在，但被禁用，则不启动PS
                        SCI_TRACE_LOW("sim card = %d is forbidden, no need to power on ps.", i);
                    }
                }
                else
                {
                    // 该卡不存在，不需要启动PS
                    SCI_TRACE_LOW("sim card = %d is not existed, no need to power on ps.", i);
                }
            }
        }
#else
        //when bbm request power on ps, sim card power on cnf is not recevied, response bbm power on request here.
        if(TRUE == s_is_need_poweron_modem)
        {
            s_is_need_poweron_modem = FALSE;
            MMIAPIPHONE_PowerOnModem();
        }
#endif
#ifdef ADULT_WATCH_SUPPORT
#ifdef SS_POWERON_INTERROGATE_SUPPORT
        if (NONE_SIM_EXISTED == PHONE_GetSimExistedStatusEx())
        {
            SCI_TRACE_LOW("Power on SIM1 PS as no sim card.");
        }
        else if (MN_DUAL_SYS_MAX != pinSim)
        {
            SCI_TRACE_LOW("Power on emergency ps of current handling sim id. cur_handling_sim_id = %d.", s_handle_pin_status.cur_handling_sim_id);
        }
        else
        {
            MMIAPIPHONE_PowerOnModem();
            Settings_SS_Interrogate_StartWaitPSTimer();//for bug1625643
            Settings_SS_Set_PowerOn_PS_Type(SETTINGS_SS_POWERON_PS_INTERROGATE);
        }
#endif
#endif
    }
    else
    {
        SCI_TRACE_LOW("NOT all sim ready to inital PS.");
    }
}
/*****************************************************************************/
//    Description : get is sim power on 
//    Global resource dependence : 
//    [In] dual_sys sim id
//    [Out] None
//    [Return] is sim card already power on :TRUE, power on;FALSE, not power on
//    Author: yonghua.zhang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsSimPowerOn(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("MMIAPIPHONE_GetIsSimPowerOn, SIM %d is power_on=%d", dual_sys, s_is_power_on_sim_cnf[dual_sys]);
    return s_is_power_on_sim_cnf[dual_sys];
}



/*****************************************************************************/
//    Description : power on Modem, increase counter s_poweron_modem_counter
//    Global resource dependence : 
//    [In] None
//    [Out] None
//    [Return] None
//    Author: yonghua.zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnModem( void )
{
    uint8 i = 0;
    MN_DUAL_SYS_E pinSim = MN_DUAL_SYS_MAX;

    SCI_TRACE_LOW("MMIAPIPHONE_PowerOnModem");
    // 3、判断是否允许启用PS
    if (TRUE == PHONE_IsAllSimReadyToInitPs())
    {
        pinSim = PHONE_GetSimNeedEmergencyPs();
        if (NONE_SIM_EXISTED == PHONE_GetSimExistedStatusEx())//for bug1625643
        {
            SCI_TRACE_LOW("MMIAPIPHONE_PowerOnModem,Power on SIM1 PS as no sim card.");
            // 启动卡1的正常PS
            MMIAPIPHONE_PowerOnPsByUsim(MN_DUAL_SYS_1);
        }
        else if (MN_DUAL_SYS_MAX != pinSim) // 判断是否需要启动Emergency PS
        {
            SCI_TRACE_LOW("MMIAPIPHONE_PowerOnModem Power on emergency ps of current handling sim id. cur_handling_sim_id = %d.", s_handle_pin_status.cur_handling_sim_id);
            // 如果需要启动Emergency PS，则启动当前handling pin的Emergency PS
            PowerOnPsForEmerge(pinSim);
        }
        else
        {
            SCI_TRACE_LOW("MMIAPIPHONE_PowerOnModem No need to power on emergency ps.");

            // 如果不需要启动Emergency PS
            for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
            {
                if (TRUE == MMIAPIPHONE_GetSimExistedStatus(i))
                {
                    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

                    if (e_dualsys_setting & (0x01 << i))
                    {
                        // 如果卡存在，且没有禁用，则启动正常的PS
                        MMIAPIPHONE_PowerOnPsByUsim(i);
                    }
                    else
                    {
                        // 如果卡存在，但被禁用，则不启动PS
                        SCI_TRACE_LOW("MMIAPIPHONE_PowerOnModem sim card = %d is forbidden, no need to power on ps.", i);
                     }
                }
                else
                {
                    // 该卡不存在，不需要启动PS
                    SCI_TRACE_LOW("MMIAPIPHONE_PowerOnModem sim card = %d is not existed, no need to power on ps.", i);
                }
            }
         }

        //每调用过一次power on ps, 计数器加1
        s_poweron_modem_counter++;
        SCI_TRACE_LOW("MMIAPIPHONE_PowerOnModem, s_poweron_modem_counter = %d", s_poweron_modem_counter);
    }
    else
    {
        //sim card is not ready when bbm request power on ps, set flag here and response request when all sim card poweron cnf is recevied  
        s_is_need_poweron_modem = TRUE;
        SCI_TRACE_LOW("MMIAPIPHONE_PowerOnModem NOT all sim ready to inital PS. s_is_need_poweron_modem = %d", s_is_need_poweron_modem);
    }
}
/*****************************************************************************/
//    Description : power off modem , decrease counter s_poweron_modem_counter,
//                  modem is power off only when counter is <= 0
//    Global resource dependence :
//    [In] None
//    [Out] None
//    [Return] None
//    Author: yonghua.zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOffModem( void )
{
    uint8 i = 0;

    s_poweron_modem_counter--;
    SCI_TRACE_LOW("MMIAPIPHONE_PowerOffModem, s_poweron_modem_counter = %d", s_poweron_modem_counter);

    //多个服务调用过power on , 当前计数器未清0， 不能power off
    if(0 < s_poweron_modem_counter)
    {
        SCI_TRACE_LOW("MMIAPIPHONE_PowerOffModem fail, s_poweron_modem_counter = %d", s_poweron_modem_counter);
        return;
    }

    //当前处于通话状态(包括蓝牙通话和PS通话)，不能power off
    if(TRUE == MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        SCI_TRACE_LOW("MMIAPIPHONE_PowerOffModem fail, current in call state");
        return;
    }

    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(TRUE == MMIAPIPHONE_GetSimExistedStatus((MN_DUAL_SYS_E)i))
        {
            // 找到已插入的sim卡，power off
            SCI_TRACE_LOW("%s:%d , power off modem of sim =%d", __FUNCTION__,__LINE__, i);
            MMIAPIPHONE_PowerOffPs((MN_DUAL_SYS_E)i);
        }
    }
}

/*****************************************************************************/
//   Description : Get Data ServiceSIM
//	Global resource dependence :
//   Author: sam.hua
//   Return value:
//        FALSE -- No available SIM for data service
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetDataServiceSIM(MN_DUAL_SYS_E *p_dual_sys_ptr)
{
    BOOLEAN ret = FALSE;

    if(p_dual_sys_ptr ==PNULL)
    {
        SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: dual_sys_ptr == PNULL");
        return FALSE;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE        //Only use in Dual SIM mode
    do
    {
        uint32 sim_ok_num = 0;
        uint16 sim_ok = MN_DUAL_SYS_1;
        MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

        sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);/*lint !e64*/
        if(sim_ok_num ==0)
        {
            *p_dual_sys_ptr =MMI_DUAL_SYS_MAX;
            ret = FALSE;
            break;
        }
        else if (sim_ok_num == 1)
        {
            *p_dual_sys_ptr = (MN_DUAL_SYS_E)sim_ok;
            ret = TRUE;
            break;
        }
        //目前只支持单卡，此部分为多卡内容
        major_sim_type = MMISET_GetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_WAP);
        if (major_sim_type < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
        {
            *p_dual_sys_ptr = MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1);
            ret =TRUE;
            SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: MMISET_GetMajorSimSelType dual_sys= %d",*p_dual_sys_ptr);
            break;
        }
        else
        {
            SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: Warning, MMISET_GetMajorSimSelType shouldn't be manual select");
            *p_dual_sys_ptr = (MN_DUAL_SYS_E)sim_ok;
            ret =TRUE;
            break;
        }
    }while(0);

    SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: result = %d , dual_sys= %d",ret,*p_dual_sys_ptr);
    return ret;

#else
    *p_dual_sys_ptr = MN_DUAL_SYS_1;
    if(MMIAPIPHONE_IsSimAvailable(*p_dual_sys_ptr))
    {
        ret =TRUE;
    }
    SCI_TRACE_LOW("mmiphone MMIAPIPHONE_GetDataServiceSIM: result = %d , dual_sys= %d",ret,*p_dual_sys_ptr);
    return  ret;
#endif
}

/*****************************************************************************/
//  Description : Is All Sim Ready
//  Global resource dependence :
//  Author: mingwei.jia
//  Return value:
//      FALSE -- No available SIM for data service
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPIPHONE_IsAllSimReady(void)
{
    uint16 sim_ok = MN_DUAL_SYS_1;
    BOOLEAN b_all_sim_ready = TRUE;
    for(sim_ok = MN_DUAL_SYS_1; sim_ok < MMI_DUAL_SYS_MAX; sim_ok++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(sim_ok))
        {
            if (! MMIPHONE_IsSimOk(sim_ok))
            {
                SCI_TraceLow("[Self Reg] HandleNetworkStatusInd <sim %d exist but not ready>", sim_ok);
                b_all_sim_ready = FALSE;
            }
        }
    }
    return b_all_sim_ready;
}

/*****************************************************************************/
//  Description : Is China Telecom
//  Global resource dependence :
//  Author: mingwei.jia
//  Return value:
//      FALSE -- No available SIM for data service
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPIPHONE_IsChinaTelecom(MN_DUAL_SYS_E dual_sys)
{
    MN_PLMN_T cur_plmn = {0};
    MMIAPIPHONE_GetSimHPLMN(&cur_plmn,dual_sys);
    SCI_TRACE_LOW("[MMIAPIPHONE_IsChinaTelecom] sim [%d] mcc =%d, mnc = %d", dual_sys, cur_plmn.mcc, cur_plmn.mnc);

    if( (460 == cur_plmn.mcc) &&
        ((03 == cur_plmn.mnc ) || (05 == cur_plmn.mnc ) || (11 == cur_plmn.mnc )))
    {
        SCI_TRACE_LOW("[MMIAPIPHONE_IsChinaTelecom] sim [%d] mcc =%d, mnc = %d (CTCC)", dual_sys, cur_plmn.mcc, cur_plmn.mnc);
        return TRUE;
    }

    return FALSE;
}

