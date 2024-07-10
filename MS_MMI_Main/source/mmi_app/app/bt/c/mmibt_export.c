/****************************************************************************
** File Name:      mmibt.c                                       *
** Author:         kelly.li                                            *
** Date:           April/06/2006                                             *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** April/06/2006  kelly.li         Create                                  *
**                                                                         *
****************************************************************************/
//#ifndef _MMIBT_C_
#define _MMIBT_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#include "mmibt_func.h"
#include "mmibt_mainwin.h"
#include "mmibt_filetransfer.h"
#include "mmiset_export.h"
#include "mmicc_export.h"
#ifdef WIN32
    #include "mmibt_a2dp.h"
#else
#ifdef BLUETOOTH_SUPPORT
#include "audio_config.h"
#endif
#ifdef BT_A2DP_SUPPORT
#include "mmibt_a2dp.h"
#include "a2dp_api.h"
#endif
#endif
#include "mmibt_app.h"
#include "mmipub.h"
#include "mmibt_text.h"
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmibt_uitest.h"
#include "mmibt_inquireddevice.h"
#include "mmibt_headset.h"
#include "watch_bt_main_win.h"

#ifdef BT_DIALER_SUPPORT
//#include "mmipb_text.h"
#include "mmicl_export.h"
#include "mmisms_export.h"
#endif
#include "watch_bt_main_win.h"
#ifdef BLUETOOTH_SUPPORT
#include "ual_bt.h"
#endif

#ifdef ADULT_WATCH_SUPPORT
#include "adultwatch_bt_win.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

#ifdef BT_BQB_SUPPORT
typedef struct
{
    MMI_TEXT_ID_T name_index;
    char name[32];
}MMIBT_NETWORK_NAME_T; 
#endif
/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
MMI_APPLICATION_T	   g_bluetooth_app = {0};
MMIFMM_BT_SENDFILE_INFO_T *s_bt_sendfile_info = PNULL;
//static int cursendfileindex = 0;
//static int send_file_num = 0;
int cursendfileindex = 0;
int send_file_num = 0;
//CR178620
//#define MMIBT_SENDFILE_MAX_NUM  10

#if defined(TRACE_MAP_BUFFER)
#define TRACE_MAP_BUF_COUNT_BYTE 120
#endif

#ifdef BT_BQB_SUPPORT
LOCAL const MMIBT_NETWORK_NAME_T s_bt_network_nametable[] = {
    {TXT_PROCESS_SEARCHING_NETWORK, "Network search"},
    {TXT_NO_SERVICE,            "No service"},
    {TXT_REGISTER_SERVICE,      "Registering"},
    {TXT_FULL_SERVICE,          "Full service"},
    {TXT_KL_PSW_EMERGENCY_CALL,         "SOS only"},
    {TXT_NET_CUCC,              "CU-GSM"},
    {TXT_NET_CMCC,              "CMCC"},
#ifdef CMCC_UI_STYLE
    {TXT_NET_CMCC_3G,           "CMCC 3G"},
#endif
    {TXT_NET_CTCC,              "CT-GSM"},
    {TXT_NET_CTT,               "CTT"},
    {TXT_NET_FAREASTONE1,       "Far EasTone"},
    {TXT_NET_FAREASTONE1,       "Fas EasTone"},
    {TXT_NET_TUNTEX,            "TUNTEX"},
    {TXT_NET_TWLDTADCS,         "TW LDTA DCS"},
    {TXT_NET_KG_ONLINE,         "KG-Online"},
    {TXT_NET_CHUNGHWA,          "ChuangHwa"},
    {TXT_NET_MOBITAIGSM,        "MoBITai GSM"},
    {TXT_NET_TWNGSM1800,        "TWN GSM 1800"},
    {TXT_NET_TRANSASIA,         "TransAsia"},
    {TXT_NET_TWLDGSM,           "TW LDGSM"},
    {TXT_NET_SMARTONE,          "SmarTone"},
    {TXT_NET_MACCTMGSM,         "MAC CTMGSM"},
    {TXT_NET_3_2G,              "3{2G},"},
    {TXT_NET_3_3G,              "3{3G},"},
    {TXT_NET_3_CDMA,            "3{CDMA},"},
    {TXT_NET_CMCC_HK,           "CMHK"},
    {TXT_NET_CSL,               "CSL"},
    {TXT_NET_HKCITIC,           "CITIC"},
    {TXT_NET_TRIDENT,           "Trident"},
    {TXT_NET_HKCTCC,            "CTCC HK"},
    {TXT_NET_HUTCH,             "Hutchison"},
    {TXT_NET_PCCW_2G,           "PCCW{2G},"},
    {TXT_NET_PCCW_3G,           "PCCW{3G},"},
    {TXT_NET_PCCW_CDMA,         "PCCW{CDMA},"},
    {TXT_NET_VIBO,              "VIBO"},
    {TXT_NET_ORANGE,            "ORANGE"},
    {TXT_NET_HKSMC,             "SMC-Voda"},
    {TXT_NET_NEWWORLD,          "NEW WORLD"},
    {TXT_NET_PEOPLES,           "PEOPLES"},
    {TXT_NET_SUNDAY,            "SUNDAY"},
    {TXT_NET_PACIFICLINK,       "PACIFIC LINK"},
    {TXT_NET_HKPPLUS,           "HK P PLUS"},
    {TXT_NET_IND_PSN,           "PSN"},  
    {TXT_NET_IND_STARONE,       "StarOne"},
    {TXT_NET_IND_TELKOMFLEXI,   "Telkom Flexi"},
    {TXT_NET_IND_TELKOMMOBILE,  "Telkom Mobile"},
    {TXT_NET_IND_CERIA,         "Ceria"},
    {TXT_NET_IND_FREN,          "Fren/Hepi"},
    {TXT_NET_IND_3,             "3"},
    {TXT_NET_IND_ESIA,          "Esia"},
    {TXT_NET_KH_QB,             "qb"},
    {TXT_NET_KH_STARCELL,       "Star-Cell"},
    {TXT_NET_KH_SMART,          "Smart"},
    {TXT_NET_KH_METFONE,        "Metfone"},
    {TXT_NET_KH_BEELINE,        "Beeline KH"},
    {TXT_NET_IND_SMART,         "SMART"},
    {TXT_NETWORK_SELECT,        "Network sel"},
    {TXT_NETWORK_LIST,          "Network list"},
    {TXT_NET_ICE,               "I.C.E."},
    {TXT_NET_COSMOTE,           "C-OTE"},
    {TXT_NET_GR_VODA,           "VODA"},
    {TXT_NET_Q_TELCOM,          "Q-TELCOM"},
    {TXT_NET_GR_TELESTET,       "TELESTET"},
    {TXT_NET_NLVODA,            "NLVODA"},
    {TXT_NET_NL_KPN_TELECOM,    "NL KPN"},
    {TXT_NET_NL_TELFORT,        "Telfort"},
//   {TXT_NET_BEN_NL,            "Ben-NL"},
//    {TXT_NET_DUTCHTONE,         "dtone"},
    {TXT_NET_BEL_PROXIMUS,      "BEL PROXI"},
    {TXT_NET_MOBISTAR,          "Mobi"},
    {TXT_NET_BASE,              "BASE"},
    {TXT_NET_F_SFR,             "SFR"},
    {TXT_NET_BOUYGTEL,          "BOUYGTEL"},
    {TXT_NET_STA_MOBILAND,      "M-AND"},
    {TXT_NET_E_AIRTEL,          "E21401"},
//    {TXT_NET_PANNON_GSM,        "Pannon"},
//    {TXT_NET_WESTEL,            "WESTEL"},
    {TXT_NET_VODAFONE_1,        "Vodafone"},
    {TXT_NET_BIH_ERONET,        "ERONET"},
    {TXT_NET_GSM_MS1,           "GSM-MS1"},
    {TXT_NET_PTT_GSMBIH,        "BHGSM"},
    {TXT_NET_HR_CRONET,         "T-Mobile HR"},
//    {TXT_NET_219_02,            "HR TELE2"},
    {TXT_NET_HR_VIP,            "HR VIP"},
    {TXT_NET_YU_MOBTEL,         "MOBTEL"},
    {TXT_NET_PROMONTE_GSM,      "ProMonte"},
    {TXT_NET_YUG_03,            "YUG 03"},
    {TXT_NET_MONET,             "Monet"},
    {TXT_NET_I_TELECOM,         "TIM"},
    {TXT_NET_GR_VODA,       "voda"},
    {TXT_NET_I_WIND,            "WIND"},
    {TXT_NET_I_BLU,             "Blu"},
    {TXT_NET_CONNEX_GSM,        "CONNEX GSM"},
    {TXT_NET_COSMOROM,          "Cosmorom"},
    {TXT_NET_RO_DIALOG,         "DIALOG"},
    {TXT_NET_SWISS_GSM,         "SWISS"},
    {TXT_NET_SUNRISE,           "CH 02"},
    {TXT_NET_ORANGE_CH,         "ORANGE"},
    {TXT_NET_CZ_PAEGAS,         "PAEGAS"},
    {TXT_NET_EUROTEL_CZ,        "EUROTEL"},
    {TXT_NET_OSKAR,             "OSKAR"},
    {TXT_NET_EUROTEL_SK,        "ET-SK"},
    {TXT_NET_A1,                "A1"},
    {TXT_NET_T_MOBILE,          "T-Mobile"},
    {TXT_NET_A_ONE,             "Orange A"},
    {TXT_NET_A_TELE_RING,       "Telering"},
    {TXT_NET_O2_UK,             "O2 - UK"},
    {TXT_NET_UK_VODAFONE,       "Vodafone"},
    {TXT_NET_TMO_UK,            "TMO UK"},
    {TXT_NET_UK_GSY_TEL,        "GSY-TEL"},
    {TXT_NET_PRONTO_GSM,        "Pronto"},
    {TXT_NET_DK_TDK_MOBIL,      "TD MOB"},
    {TXT_NET_SONOFON,           "SONO"},
    {TXT_NET_DK_TELIA,          "Telia"},
    {TXT_NET_ORANGE,         "Orange"},
//    {TXT_NET_S_COMVIQ,          "IQ"},
    {TXT_NET_S_EUROPOLITAN,     "EURO"},
    {TXT_NET_N_TELE_MOBIL,      "TELE N"},
    {TXT_NET_N_NETCOM_GSM,      "N COM"},
    {TXT_NET_FI_RL,             "RL"},
    {TXT_NET_FI_FINNET,         "Finnet"},
    {TXT_NET_FI_2G,             "2G"},
    {TXT_NET_AMT,               "AMT"},
    {TXT_NET_FI_SONERA,         "sonera"},
    {TXT_NET_OMNITEL_LITH,      "OMT"},
    {TXT_NET_LT_BITE_GSM,       "Bite GSM"},
    {TXT_NET_TELE2,             "TELE2"},
    {TXT_NET_LV_LMT_GSM,        "LMT"},
    {TXT_NET_LV_BALTCOM,        "B-COM"},
    {TXT_NET_EMT_GSM,           "EE EMT"},
    {TXT_NET_EE_RLE_GSM,        "RLE"},
    {TXT_NET_TELE2,          "TELE2"},
    {TXT_NET_250_01,            "MTS RUS"},
    {TXT_NET_RUS_02,            "MEGAFON"},
    {TXT_NET_RUS_03,            "NCC"},
    {TXT_NET_250_04,            "250 04"},
    {TXT_NET_RUS_05,            "RUS 05"},
    {TXT_NET_250_15,            "RUS 07"},
    {TXT_NET_RUS_10,            "RUS 10"},
    {TXT_NET_250_11,            "250 11"},
    {TXT_NET_RUS_12,            "RUS 12"},
    {TXT_NET_RUS_KUBAN_GSM,     "KU GSM"},
    {TXT_NET_250_16,            "NTC"},
    {TXT_NET_250_20,            "Tele2"},
    {TXT_NET_NC_GSM,            "NC-GSM"},
    {TXT_NET_RUS_92,            "Primtel"},
    {TXT_NET_RUS_93,            "RUS 93"},
    {TXT_NET_BEELINE,           "BeeLine"},
    {TXT_NET_UMC_UA,            "UMC"},
    {TXT_NET_FLASH_UKR,         "FLASH"},
    {TXT_NET_UA_KYIVSTAR,       "KYIVSTAR"},
    {TXT_NET_UA_GOLDEN_TELECOM, "UA GT"},
    {TXT_NET_BY_VELCOM,         "VELCOM"},
    {TXT_NET_MD_VOXTEL,         "VOXTEL"},
    {TXT_NET_MD_02,             "MD 02"},
    {TXT_NET_PL_PLUS,           "PLUS"},
    {TXT_NET_T_MOBILE,        "T-Mobile"},
    {TXT_NET_VODAFONE_1,       "vodafone"},
    {TXT_NET_E_PLUS,            "E-Plus"},
    {TXT_NET_O2_DE,             "O2 - DE"},
    {TXT_NET_GIBTEL,            "GIB"},
    {TXT_NET_VODAFONE_1,        "VODAFONE"},
    {TXT_NET_P_OPTIMUS,         "P03"},
    {TXT_NET_P_TELEMOVEL,       "P TMN"},
    {TXT_NET_L_LUXGSM,          "P&T L"},
    {TXT_NET_L_TANGO,           "Tango"},
    {TXT_NET_IRL_EIRCELL,       "EIRCELL"},
    {TXT_NET_02_IRL,            "02-IRL"},
    {TXT_NET_METEOR,            "METEOR"},
    {TXT_NET_IS_SIMINN,         "SIMINN"},
    {TXT_NET_IS_TAL,            "TAL"},
    {TXT_NET_ISLANDSSIMI,       "Islandssimi"},
    {TXT_NET_274_04,            "274 04"},
    {TXT_NET_AMC_AL,            "AMC-AL"},
    {TXT_NET_VODAFONE_2,        "AL 02"},
    {TXT_NET_VODAFONE_MLA,      "VODA M"},
    {TXT_NET_GO_MOBILE,         "go mobile"},
    {TXT_NET_CY_CYTAGSM,        "CY-GSM"},
    {TXT_NET_GEO_GEOCELL,       "GEOGC"},
    {TXT_NET_GEO_MAGTI,         "MAGTI"},
    {TXT_NET_RA_ARMGSM,         "ARMMO1"},
    {TXT_NET_M_TEL_GSM_BG,      "M-TEL"},
    {TXT_NET_GLOBUL,            "GloBul"},
    {TXT_NET_TR_TCELL,          "TCEL"},
    {TXT_NET_TR_TELSIM,         "TLSM"},
    {TXT_NET_ARIA,              "ARIA"},
    {TXT_NET_AYCELL,            "AYCELL"},
    {TXT_NET_FROFT,             "FROFT"},
    {TXT_NET_TELE_GRL,          "TELE GRL"},
    {TXT_NET_SI_MOBIL,          "SI.MOBIL"},
    {TXT_NET_SI_GSM,            "SI GSM"},
    {TXT_NET_VEGA_070,          "VEGA 070"},
    {TXT_NET_MKD_MOBIMAK,       "MOBI-M"},
    {TXT_NET_295_01,            "295 01"},
    {TXT_NET_EUROPLATFORM,      "OFL"},
    {TXT_NET_FL1,               "FL1"},
    {TXT_NET_LI_TANGO,          "LI 77"},
    {TXT_NET_MCELL,         "MCELL"},
    {TXT_NET_ROGERS_AT_T_WIRELESS,    "ROGERS"},
    {TXT_NET_MCELL,             "MCELL"},
    {TXT_NET_310_01,            "310 01"},
    {TXT_NET_WTTCKY_2,          "PTI Pacifica"},
    {TXT_NET_SPROCKET,          "Sprocket"},
    {TXT_NET_AT_T_WIRELESS_1,    "AT&T"},
//    {TXT_NET_VOICESTREAM_1,         "VStream"},
//    {TXT_NET_CINGULAR_WIRELESS_2,    "Cingular"},
    {TXT_NET_CINGULAR_WIRELESS_3,    "West Central"},
//    {TXT_NET_VOICESTREAM_2,      "VStream"},
//    {TXT_NET_VOICESTREAM_3,      "VStream"},
//    {TXT_NET_VOICESTREAM_4,      "VStream"},
//    {TXT_NET_VOICESTREAM_5,      "VStream"},
//    {TXT_NET_VOICESTREAM_6,      "VStream"},
//    {TXT_NET_VOICESTREAM_7,      "VStream"},
//    {TXT_NET_VOICESTREAM_8,      "VStream"},
//    {TXT_NET_VOICESTREAM_9,      "VStream"},
//    {TXT_NET_VOICESTREAM_10,     "VStream"},
    {TXT_NET_WESTLINK_COMM,      "WestLink"},
    {TXT_NET_CAROLINA_PHONE,     "Carolina"},
    {TXT_NET_AT_T_WIRELESS_1,    "AT&T"},
    {TXT_NET_310_460,            "Simmetry"},
    {TXT_NET_EPIC_TOUCH,         "EpicTouch"},
    {TXT_NET_AMERILINK_PCS,      "AmeriLink"},
    {TXT_NET_EINSTEIN_PCS,       "Airadigm"},
    {TXT_NET_WIRELESS_2000_PCS,  "Northstar"},
    {TXT_NET_CONESTOGA,          "Conestoga"},
    {TXT_NET_TELEMETRIX,         "Convey"},
    {TXT_NET_PTSI,               "Panhandle"},
    {TXT_NET_IWS,                "i wireless"},
    {TXT_NET_AIRLINK_PCS,        "AirLink"},
    {TXT_NET_PINPOINT,           "Pinpoint"},
    {TXT_NET_VOICESTREAM_12,     "VStream"},
    {TXT_NET_AT_T_WIRELESS_1,    "AT&T"},
    {TXT_NET_WTTCKY_1,           " WTTCKy"},
    {TXT_NET_AERIAL,             " AERIAL"},
    {TXT_NET_BSMDCS,             " BSMDCS"},
    {TXT_NET_WWC_1,              " WWC"},
    {TXT_NET_WWC_1,              " WWC"},
    {TXT_NET_WWC_1,              " WWC"},
    {TXT_NET_WWC_1,              " WWC"},
    {TXT_NET_WWC_1,              " WWC"},
    {TXT_NET_WWC_1,              " WWC"},
    {TXT_NET_T_MOBILE,              " T-Mobile"},
    {TXT_NET_USA27,              " USA27"},
    {TXT_NET_334_04,             "Iusacell/Unefon"},
    {TXT_NET_TELCEL,             "TELCEL"},
    {TXT_NET_334_050,            "Iusacell"},
//    {TXT_NET_338_05,             "338 05"},
    {TXT_NET_DIGICEL,             "340 20"},
    {TXT_NET_APUA_PCS_ANTIGUA,   "APUA-PCS"},
    {TXT_NET_TELECOM,            "Telecom"},
    {TXT_NET_TELCELL_GSM,        "Telcell"},
    {TXT_NET_UTS,                "UTS"},
    {TXT_NET_C_COM,              "C_Com"},
    {TXT_NET_ORANGE_3,           "orange"},
    {TXT_NET_AZE_AZERCELL_GSM,   "AZE ACELL"},
    {TXT_NET_BAKCELL_GSM_2000,   "BKCELL"},
    {TXT_NET_KZ_K_MOBILE,        "K MOBILE"},
    {TXT_NET_KCELL_KZ,           "KZ 02"},
    {TXT_NET_CELFORCE,           "Vodafone IN"},
    {TXT_NET_TATA,               "TATA"},
    {TXT_NET_INA_08,             "USHAFONE"},
    {TXT_NET_INA_09,             "Reliance"},
    {TXT_NET_INA_ESSAR,          "Vodafone IN"},
    {TXT_NET_HUTCH_1,            "Vodafone IN"},
    {TXT_NET_IN_14,              "SPICE"},
    {TXT_NET_AIRCEL_1,           "Vodafone IN"},
    {TXT_NET_INA_18,             "Reliance"},
    {TXT_NET_INA_19,             "IDEA"},
    {TXT_NET_INA_ORANGE,         "Vodafone IN"},
    {TXT_NET_BPL,                "BPL"},
    {TXT_NET_IDEA,             "IDEA"},
    {TXT_NET_INA_08,             "USHAFONE"},
    {TXT_NET_IDEA,             "IDEA"},
    {TXT_NET_INA_08,             "USHAFONE"},
    {TXT_NET_BPL_MO,             "BPL MO"},
    {TXT_NET_INA_COMMAND,        "Vodafone IN"},
    {TXT_NET_INA_32,             "USHAFONE"},
    {TXT_NET_INA_36,             "Reliance"},
    {TXT_NET_RPG,                "AIRCEL"},
    {TXT_NET_AIRCEL_2,           "AIRCEL"},
    {TXT_NET_INA_43,             "BPL MO"},
    {TXT_NET_IN_44,              "SPICE"},
    {TXT_NET_INA_46,             "BPL MO"},
    {TXT_NET_INA_50,             "Reliance"},
    {TXT_NET_INA_52,             "Reliance"},
    {TXT_NET_INA_56,             "IDEA"},
    {TXT_NET_INA_67,             "Reliance"},
    {TXT_NET_IN_DOLPHIN_1,       "IN Dolphin"},
    {TXT_NET_IN_DOLPHIN_1,       "IN Dolphin"},
    {TXT_NET_INDH1,            "INDH1"},
    {TXT_NET_IDEA,           "IDEA"},
    {TXT_NET_INA_09,             "Reliance"},
    {TXT_NET_VODAFONE_IN,            "Vodafone IN"},
#if 0        
    {TXT_NET_404_90,             "AirTel"},
    {TXT_NET_404_92,             "AirTel"},
    {TXT_NET_404_93,             "AirTel"},
    {TXT_NET_404_94,             "AirTel"},
    {TXT_NET_404_95,             "AirTel"},
    {TXT_NET_404_96,             "AirTel"},
    {TXT_NET_404_97,             "AirTel"},
    {TXT_NET_404_98,             "AirTel"},
#endif        
    {TXT_NET_VODAFONE_IN,              "Vodafone IN"},
    {TXT_NET_ESSARR,             " ESSARR"},
//    {TXT_NET_412_01,             "412 01"},
//    {TXT_NET_SRI_DIALOG,         "DALOG"},
    {TXT_NET_SRI_CELLTEL,        "CELLTEL"},
    {TXT_NET_MPT,             "MPT"},
    {TXT_NET_ALFA,          "Alfa"},
    {TXT_NET_RL_LIBANCELL,       "RL LibCL"},
    {TXT_NET_JOR_FASTLINK,       "FASTLINK"},
    {TXT_NET_MOBILECOM,          "Orange JO"},
    {TXT_NET_SYRIATEL,             "Syriatel"},
    {TXT_NET_SYR_MOB,            "SYR MOB"},
    {TXT_NET_417_93,             "417 93"},
    {TXT_NET_KT_MTCNET,          "MTCNet"},
//    {TXT_NET_419_03,             "419 03"},
    {TXT_NET_KSA_AL_JAWAL,       "STC"},
    {TXT_NET_420_03,       "Mobily"},
    {TXT_NET_420_04,       "Zain SA"},
//    {TXT_NET_421_02,             "421 02"},
    {TXT_NET_OMAN_MOBILE,        "GTO"},
    {TXT_NET_ETSLT,              "Etisalat"},
    {TXT_NET_424_03,             "DU"},
    {TXT_NET_ORANGE,          "Orange"},
    {TXT_NET_JAWWAL_PALESTINE,   "JAWWAL"},
//    {TXT_NET_BHR_MOBILE_PLUS,    "BHR M.Plus"},
    {TXT_NET_QATARNET,           "QATARNET"},
    {TXT_NET_MOBICOM,            "MobiCom"},
    {TXT_NET_250_16,             "NTC"},
    {TXT_NET_IR_TCI,             "IR-MCI"},
    {TXT_NET_IR_KISH,            "TKC"},
    {TXT_NET_432_19,            "MTCE"},
    {TXT_NET_TALIYA,            "Taliya"},
    {TXT_NET_IRANCELL,            "Irancell"},
    {TXT_NET_432_70,            "TCI"},
    {TXT_NET_BUZTEL,             "Buztel"},
    {TXT_NET_UZMACOM,            "Uzmacom"},
//    {TXT_NET_UZB_DAEWOO_GSM,     "DW-GSM"},
//    {TXT_NET_UZB_COSCOM_GSM,     "Coscom"},
//    {TXT_NET_BITEL,          "BITEL"},
    {TXT_NET_BITEL,              "BITEL"},
//    {TXT_NET_BCTI,               "BCTI"},
    {TXT_NET_452_01,             "Mobifone"},
    {TXT_NET_452_02,             "Vinaphone"},
    {TXT_NET_HUTCHISON,          " HTL"},
    {TXT_NET_HUTCHISON_MAC,      "HT Macau"},
    {TXT_NET_MOBITEL_KHM,        "MT-KHM"},
    {TXT_NET_KHM_SAMART_GSM,     "KHM-SM"},
    {TXT_NET_CAMSHIN,            "CAMSHIN"},
//    {TXT_NET_457_01,             "457 01"},
//    {TXT_NET_457_02,             "457 02"},
    {TXT_NET_BGD_GP,             "BGD GP"},
    {TXT_NET_ROBI,              "AKTEL"},
    {TXT_NET_GRAMEENPHONE,       "Grameenphone"},
    {TXT_NET_ROBI,               "Robi"},
    {TXT_NET_BANGLALINK,         "Banglalink"},
    {TXT_NET_TELETALK,           "TeleTalk"},
    {TXT_NET_WARID,              "Warid"},
//    {TXT_NET_D_MOBILE,           "D-MOBILE"},
    {TXT_NET_MY_MAXIS,           "maxis"},
    {TXT_NET_TM_TOUCH,           "TM Touch"},
    {TXT_NET_DIGI,               "DiGi"},
    {TXT_NET_MY_ADAM,            "MY17"},
    {TXT_NET_MY_CELCOM,          "CELCOM"},
    {TXT_NET_TELSTRA_MOBILENET,  "M-NET"},
    {TXT_NET_CABLE_WIRELESS_OPTUS,    "OPTUS"},
    {TXT_NET_VODAFONE_AUS,        "VFONE AUS"},
    {TXT_NET_IND_SATELINDOCEL,    "SAT-C"},
    {TXT_NET_LIPPO_TEL,           "LIPPO TEL"},
    {TXT_NET_TELKOMSEL,           "TELKOMSEL"},
    {TXT_NET_PROXL,               "proXL"},
    {TXT_NET_IM_3,                "IM-3"},
    {TXT_NET_ISLACOM,             "ISLA"},
    {TXT_NET_GLOBE_TELECOM_PH,    "GLOBE"},
    {TXT_NET_SMART_PH03,          "SMART Gold"},
    {TXT_NET_PH_DIGITEL,          "SUN"},
    {TXT_NET_TH_AIS_GSM,          "AIS GSM"},
    {TXT_NET_TH_ACT_1900,         "ACT-1900"},
    {TXT_NET_WP_1800,             "TH GSM 1800"},
    {TXT_NET_TH_HELLO,            "DTAC"},
    {TXT_NET_TH_ORANGE,           "True"},
    {TXT_NET_WCS,                 "TH WCS"},
    {TXT_NET_ST_GSM_SGP,          "STGSM"},
    {TXT_NET_STPCN,               "STPCN"},
    {TXT_NET_M1_GSM_SGP,          "M1-GSM"},
    {TXT_NET_STARHUB,             "STARHUB"},
    {TXT_NET_BRU_DSTCOM,          "BRU DSTCOM"},
    {TXT_NET_VODAFONE_1,         "Vodafone"},
    {TXT_NET_U_CALL,              "U-CALL"},
    {TXT_NET_SMILE,               "SMILE"},
    {TXT_NET_VODAFONE_1,       "Vodafone"},
    {TXT_NET_BLUE_SKY,            "Blue Sky"},
    {TXT_NET_MOBILIS,             "MOBILIS"},
    {TXT_NET_VINI,                "VINI"},
    {TXT_NET_550_01,              "550 01"},
    {TXT_NET_ECMS_MOBINIL,        "EGY MobiNiL"},
    {TXT_NET_VODAFONE_1,           "CLICK"},
    {TXT_NET_ALGERIAN_MOBILE_NETWORK,    "AMN"},
    {TXT_NET_DJEZZY,              "DJEZZY"},
    {TXT_NET_MEDITEL,             "MEDITEL"},
    {TXT_NET_MORIAM,              "IAM"},
    {TXT_NET_INWI,                "INWI"},
    {TXT_NET_TUNICELL,            "TUNICELL"},
    {TXT_NET_GAMCEL,              "GAMCEL"},
    {TXT_NET_AFRICEL,              "AFRICEL"},
    {TXT_NET_SEN_ALIZE,           "ALIZE"},
//  {TXT_NET_SENTEL,              "Sentel"},
    {TXT_NET_MALITEL,             "Malitel"},
    {TXT_NET_610_02,              "OML"},
    {TXT_NET_MOBILIS,          "Mobilis"},
    {TXT_NET_GN_LAGUI,            "Lagui"},
    {TXT_NET_CORA,                "CORA"},
    {TXT_NET_TELCEL,          "TELCEL"},
    {TXT_NET_ONATEL,              "Onatel"},
    {TXT_NET_CELTEL,           "CELTEL"},
    {TXT_NET_CELTEL,           "CELTEL"},
    {TXT_NET_TOGOCEL,             "TOGOCEL"},
    {TXT_NET_LIBERCOM,            "LIBERCOM"},
    {TXT_NET_TELECEL_BENIN,       "Telecel"},
    {TXT_NET_BBCOM,             "BBCOM"},
    {TXT_NET_CELLPLUS_MRU,        "CELL+"},
    {TXT_NET_EMTEL,               "EMTEL"},
    {TXT_NET_LONESTAR,            "LoneStar"},
    {TXT_NET_GH_SPACEFON,         "SPACE"},
    {TXT_NET_GH_ONETOUCH,         "ONETouch"},
    {TXT_NET_MOBITEL,             "mobitel"},
    {TXT_NET_ECONET,              "Zain"},
    {TXT_NET_MTN_NG,              "MTN NG"},
    {TXT_NET_M_TEL_GSM_BG,              "M-Tel"},
    {TXT_NET_621_50,              "Glo"},
    {TXT_NET_CELTEL,        "CELTEL"},
    {TXT_NET_GO_LIBERTIS,      "LIBERTIS"},
    {TXT_NET_CAM_CELLNET,         "MTN-CAM"},
    {TXT_NET_CPVMOVEL,            "CPVMOVEL"},
    {TXT_NET_GO_LIBERTIS,         "LIBERTIS"},
//    {TXT_NET_628_02,              "628 02"},
    {TXT_NET_CELTEL,              "CELTEL"},
    {TXT_NET_GO_LIBERTIS,        "LIBERTIS"},
    {TXT_NET_RCB_CELLNET,         "CELLNET"},
    {TXT_NET_CELTEL,          "CELTEL"},
    {TXT_NET_CELLCO,              "CELLCO"},
    {TXT_NET_SAIT_TELECOM,               "SAIT TELECOM"},
    {TXT_NET_UNITEL,              "UNITEL"},
    {TXT_NET_SEZ_SEYCEL,          "SEYCEL"},
    {TXT_NET_SDN_MOB,             "Zain SDN"},
    {TXT_NET_634_02,              "MTN Sudan"},
    {TXT_NET_634_07,              "Sudani One"},
    {TXT_NET_RCELL,               "RCELL"},
    {TXT_NET_ETH_ETA,             "ETA"},
    {TXT_NET_BARAKAAT,            "BARAKAAT"},
    {TXT_NET_NATIONLINK,          "Nationlink"},
    {TXT_NET_TELSOM,              "telsom"},
    {TXT_NET_PTT,                 "PTT"},
    {TXT_NET_KE_KENCELL,          "KE-KENCELL"},
    {TXT_NET_SAFARICOM,           "Safaricom"},
    {TXT_NET_ZAINKE,              "Zain KE"},
    {TXT_NET_YU,                  "Yu"},
    {TXT_NET_TRITEL_TZ,           "TRITEL"},
//    {TXT_NET_640_02,              "640 02"},
    {TXT_NET_TZ_03,               "TZ 03"},
    {TXT_NET_VODACOM,             "Vodacom"},
//  {TXT_NET_640_05,              "640 05"},
    {TXT_NET_CELTEL_CELLULAR,     "Celtel"},
    {TXT_NET_MTN_UGANDA,          "MTN-UG"},
    {TXT_NET_UTL_MOBILE,          "UTL"},
    {TXT_NET_SPACETEL_BDI,        "MDG01"},
    {TXT_NET_SAFARIS,             "SAFARIS"},
    {TXT_NET_MOZ_MCEL,            "mCel"},
//    {TXT_NET_645_01,              "645 01"},
//    {TXT_NET_654_02,              "654 02"},
//    {TXT_NET_646_01,              "1.000000"},
    {TXT_NET_MDG_ANTARIS,          "OrangeMG"},
//    {TXT_NET_647_02,               "647 02"},
    {TXT_NET_SFR_REUNION,          "SFR RU"},
    {TXT_NET_ZWNET_ONE,            "Net*One Cellular"},
    {TXT_NET_648_03,               "Telecel ZW"},
    {TXT_NET_ZW_04,                "ZW Econet"},
    {TXT_NET_MTC_NAM,              "MTC NAM"},
    {TXT_NET_MW_CP_900,            "MW CP 900"},
//    {TXT_NET_650_10,               "650 10"},
    {TXT_NET_VCL_COMMS,            "VCLCOM"},
    {TXT_NET_BW_MASCOM,            "MASCOM"},
    {TXT_NET_653_10,            "653 10"},
    {TXT_NET_GR_VODA,        "VODA"},
    {TXT_NET_CELL_C,            "Cell C"},
    {TXT_NET_MTN_SA,            "MTN"},
    {TXT_NET_ESV_PERSONAL,        "CTE Telecom"},
    {TXT_NET_DIGICEL,             "Digicel"},
    {TXT_NET_MOVISTAR,            "Movistar"},
    {TXT_NET_NEXTEL,              "NEXTEL"},
    {TXT_NET_TIM_PERU,            "Claro"},
//    {TXT_NET_UNIFON,              "UNIFON"},
//    {TXT_NET_PERSONAL,            "AR TP"},
    {TXT_NET_P_HABLE,             "P-HABLE"},
    {TXT_NET_ENTEL_PCS,           " ENTEL"},
    {TXT_NET_VZ_INFONET,          "Infonet"},
    {TXT_NET_DIGITEL,             "VZ 02"},
    {TXT_NET_NUEVATEL,            "VIVA"},
    {TXT_NET_EMOVIL,              "BOMOV"},
    {TXT_NET_HOLA_PARAGUAY,        "VOX"},
    {TXT_NET_PY_02,                "PY 02"},
    {TXT_NET_ICMS_SR,              "ICMS"},
    {TXT_NET_TIM_BRASIL,          "TIM BRASIL"},
    {TXT_NET_OI_BRASIL,           "Oi"},
    {TXT_NET_CLARO_BRASIL,        "Claro"},
    {TXT_NET_VIVO_BRASIL,        "VIVO"},
    {TXT_NET_CTBC_BRASIL,        "CTBC Celular"},
    {TXT_NET_SERCOMTEL_BRASIL,    "Sercomtel"},
    {TXT_NET_AEIOU_BRASIL,       "Aeiou"},
//    {TXT_NET_UNITECH,            "Unitech Wireless"},
    {TXT_NET_VIDEOCON,            "Videocon"},
    {TXT_NET_LOOP_MOBILE,        "Loop Mobile"},
    {TXT_NET_KTF,                "KTF"},
    {TXT_NET_DIGITAL,            "Digital 017"},
    {TXT_NET_SKT,                "SKT"},
    {TXT_NET_LGT,                "LGT"},
    {TXT_NET_KTFSHOW,            "KFT SHOW"},
    {TXT_NET_AEIOU_BRASIL,        "aeiou"},
    {TXT_NET_NEXTEL_BRASIL,        "Nextel"},
    {TXT_NET_410_01,            "Mobilink"},
    {TXT_NET_410_03,            "Ufone"},
    {TXT_NET_410_04,            "ZONG"},
    {TXT_NET_TELENOR,            "Telenor"},
    {TXT_NET_WARID,            "Warid"},
    {TXT_NET_452_04,            "Viettel"},
    {TXT_NET_452_03,            "S-Fone"},
    {TXT_NET_452_05,            "Vietnamobile"},
    {TXT_NET_452_06,            "E-Mobile"},
    {TXT_NET_452_07,            "Beeline VN"},
    {TXT_NETWORK_CONNECT,        "Network connect"},
    {TXT_NET_CNC,                "CNC"},
    {TXT_NET_VODAFONE_IN,        "Vodafone IN"},
    {TXT_NET_MTS_IN,        "MTS IN"},
    {TXT_NET_RSG,                "Reliance Smart GSM"},
    {TXT_NET_TATADOCO,            "TATA DOCOMO"},
    {TXT_NET_TATA_ORISSA,         "TATA DOCOMO Orissa"},
    {TXT_NET_UNINOR,             "Uninor"},
    {TXT_NET_STEL,                 "S Tel"},
    {TXT_NET_ETISALAT,             "Etisalat DB"},
    {TXT_NET_ALLIANZ,            "Allianz Infratech"},
    {TXT_NET_AIRTEL,            "AirTel"},
    {TXT_NET_IDEA,                "IDEA"},
    {TXT_NET_SPICE,                "SPICE"},
    {TXT_NET_AIRCEL_NEW,        "AIRCEL"},
    {TXT_NET_BPL_MOBILE,        "BPL MOBILE"},
    {TXT_NET_CELLONE,            "CellOne"},
    {TXT_NET_DOLPHIN,            "DOLPHIN"},
    {TXT_NET_620_07,            "GloGhana"},
    {TXT_NET_INDH1,                "INDH1"},
    {TXT_NET_HONDUTEL,          "HONDUTEL"},
#ifdef CMCC_UI_STYLE    
    {TXT_NET_FETION,            "Fetion"},
#endif
    
    {TXT_NET_ORANGE_UG,            "OUL"},
    {TXT_NET_SFR,            "SFR"},
    {TXT_NET_FREE,            "Free"},
    {TXT_NET_BOUYGUES,            "Bouygues"},
//    {TXT_NET_F_CONTACT,            "Orange"},
    {TXT_NET_UK_01,            "UK01"},
    {TXT_NET_234_50,            "JT-Wave"},
    {TXT_NET_234_75,            "Inquam"},
    {TXT_NET_UK_BT,            "BT"},
    {TXT_NET_CABLE_WIRELESS,            "Cable & Wireless"},
    {TXT_NET_234_78,            "Airwave"},

    {TXT_NET_464,"CHINA MOBILE"},
    {TXT_NET_465,"CHN-UNICOM"},
    {TXT_NET_467,"CHN-CT"},
    {TXT_NET_471,"Far EasTone"},
    {TXT_NET_459,"TWN APT"},
    {TXT_NET_473,"KGT-Online"},
    {TXT_NET_474,"T Star"},
    {TXT_NET_475,"Chunghwa"},
    {TXT_NET_476,"TWN MOBITAI"},
    {TXT_NET_477,"TW Mobile"},
    {TXT_NET_478,"TWM TransAsi"}
};
#endif
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/																	//store if the blue tooth is power off because USB

#ifdef BLUETOOTH_SUPPORT

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Discription: This fucntion is used to open the bluetooth main menu win  
//  
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenMainMenuWin(void)
{
   //set main operation
   MMIBT_OpenMainMenuWin();
}

/****************************************************************************/
//  Description : enter bluetooth main window
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void MMIAPIBT_MainWin_Enter( void )
{
    BT_MainWin_Enter();
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Discription: This fucntion is used to open the BtPhone main menu win  
//  
//  Global resource dependence: None
//  Author: yang.yang 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBtPhoneWin(void)
{
   //set main operation
   MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_MAIN);
   MMIBT_OpenBtPhoneWin();
}
#endif

#ifdef BT_DUN_SUPPORT
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_CloseDUNWin(void)
{
	MMIBT_CloseDUNWin();
}
#endif
/*****************************************************************************/
//  Discription: send file with blue tooth
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_SendMultiFile(MMIFMM_BT_SENDFILE_INFO_T *send_file_info_ptr,int file_num)
{  
    int i = 0;
    if(PNULL == send_file_info_ptr)
    {
        return;
    }    
    //SCI_TRACE_LOW:"MMIAPIBT_SendMultiFile PNULL != send_file_info_ptr"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_732_112_2_18_2_7_48_54,(uint8*)"");
    
    if(file_num > MMIBT_SENDFILE_MAX_NUM)
    {
        MMIPUB_OpenAlertWarningWin(TXT_BT_SENDFILE_MAX_NUMBER);
        return;
    }
    
    s_bt_sendfile_info = (MMIFMM_BT_SENDFILE_INFO_T *)SCI_ALLOC_APP(file_num*sizeof(MMIFMM_BT_SENDFILE_INFO_T));/*lint !e737*/
    if(PNULL == s_bt_sendfile_info)
    {
		return;
    }
    SCI_MEMSET(s_bt_sendfile_info,0,file_num*sizeof(MMIFMM_BT_SENDFILE_INFO_T));/*lint !e737*/
    SCI_MEMCPY(s_bt_sendfile_info,send_file_info_ptr,file_num*sizeof(MMIFMM_BT_SENDFILE_INFO_T));/*lint !e737*/
    send_file_num = file_num;
    
    //set send operation
    if(MMIAPIBT_IsOpenFileWin())
    {
        MMIPUB_OpenAlertWarningWin(TXT_BT_TRANSFER);
        for(i = 0;i < file_num; i ++)
        {
            if(s_bt_sendfile_info[i].is_temp_file)
            {
            	//need delete temp file
            	MMIAPIFMM_DeleteFile(s_bt_sendfile_info[i].filepath_name, PNULL);
            }
        }
        return;
    }
    for(i = 0;i < file_num; i ++)
    {
#ifdef DRM_SUPPORT    
        if(MMIAPIDRM_IsDRMFile(SFS_INVALID_HANDLE , s_bt_sendfile_info[i].filepath_name))  
        {
            //只要不能发送，则立马提示返回
            if (!MMIAPIDRM_IsCanForward(PNULL, SFS_INVALID_HANDLE, s_bt_sendfile_info[i].filepath_name))
            {
                MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);               
                return;
            }
        } 
#endif        
    }
    MMIBT_SetOpertor(MMIBT_OPC_OPP_SEND, MMIBT_OPC_OPP_SEND);	
	MMIBT_SendFile(s_bt_sendfile_info[cursendfileindex].filepath_name,/*lint !e605 */
				s_bt_sendfile_info[cursendfileindex].filepath_len,
				s_bt_sendfile_info[cursendfileindex].file_size,
				s_bt_sendfile_info[cursendfileindex].is_temp_file);
    
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SendNextFileCallBack(void)
{   	
	cursendfileindex ++;
    //SCI_TRACE_LOW:"[&&&]MMIBT_SendNextFileCallBack start,cursendfileindex =%d,send_file_num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_793_112_2_18_2_7_48_55,(uint8*)"dd",cursendfileindex,send_file_num);

    if(cursendfileindex < send_file_num)
	{	
        if(!MMIAPIFMM_IsFileExist(s_bt_sendfile_info[cursendfileindex].filepath_name,/*lint !e605 */ 
                                s_bt_sendfile_info[cursendfileindex].filepath_len))
        {
            cursendfileindex ++;
        } 
	}   
	if(cursendfileindex < send_file_num)
	{
		MMIBT_SendNextFile(s_bt_sendfile_info[cursendfileindex].filepath_name,/*lint !e605 */
							s_bt_sendfile_info[cursendfileindex].filepath_len,
							s_bt_sendfile_info[cursendfileindex].file_size,
							s_bt_sendfile_info[cursendfileindex].is_temp_file);
		MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_FULL_PAINT, PNULL);
	}
	else
	{
		MMIBT_SetSendFileEndTimer();
        cursendfileindex = 0;
        send_file_num = 0;        
		if(PNULL != s_bt_sendfile_info)
		{
			SCI_FREE(s_bt_sendfile_info);
		}        
        MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
        MMIBT_UpdateMainMenu(TRUE);
#ifdef BT_DIALER_SUPPORT
        if(MMK_IsOpenWin(MMIBT_BTPHONE_WIN_ID))
        {
            MMIBT_UpdateBtPhoneWin(TRUE);
        }
#endif
	}	
}
/*****************************************************************************/
//  Discription: if blue tooth open send or receive file win
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPIBT_IsOpenFileWin(void)
{
     return MMIBT_IsOnTransferStatus();
}
/*****************************************************************************/
//  Discription: get bt device status
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:TRUE: bt device on. FALSE: bt device off
//
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIBT_IsBtOn(void)
{
    BOOLEAN   bt_on = BT_GetState();
    return(bt_on);
}
/*****************************************************************************/
//  Discription: get bt sending connect and receiving window status
//  Global resource dependence: none 
//  Author: leiming.ma 
//  Note:TRUE: window is open . FALSE: window is closed
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsSendingOrReceiving(void)
{
    if( MMK_IsOpenWin(MMIBT_SENDING_FILE_WAIT_WIN_ID)
        || MMK_IsOpenWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID)
        || MMK_IsOpenWin(MMIBT_RECEIVING_FILE_WIN_ID)
        )
	{
	    return TRUE;
	}
	return FALSE;
}

/*****************************************************************************/
//  Discription: bt remote alert
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_RemoteAlerting(void)
{
	BT_STATUS status = BT_NO_CONNECTION;
    if (BT_GetActivatedHS(0))
	{
//		SCI_TRACE_LOW("MMIAPIBT_RemoteAlerting");
		status =BT_RemoteAlerting();
	}
	return status;
}

/*****************************************************************************/
//  Discription: bt call connect
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_SetBtCallStatus(uint32 status)
{
    return BT_HfgCallIndStatus(status);
}

/*****************************************************************************/
//  Discription: bt call connect
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_CallConnect(BT_ADDRESS *addr)
{
	BT_STATUS  status = BT_NO_CONNECTION;
	if (BT_GetActivatedHS(addr))
	{
//		SCI_TRACE_LOW("MMIAPIBT_CallConnect");
		BT_CallConnected();
	}
	return status;
}

/*****************************************************************************/
//  Discription: bt get active handfree 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIBT_GetActivatedHS(BT_ADDRESS *addr)
{
    if (PNULL == addr)
    {
        //SCI_TRACE_LOW:"MMIAPIBT_GetActivatedHS PNULL == addr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_894_112_2_18_2_7_49_56,(uint8*)"");
        return FALSE;
    }
	return BT_GetActivatedHS(addr);
}

/*****************************************************************************/
//  Discription: bt Stop Alert 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_StopAlert()
{
	return BT_StopAlert();
}


/*****************************************************************************/
//  Discription: bt Stop Ring 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_StopRing()
{
	return BT_StopRing();
}


/*****************************************************************************/
//  Discription: bt call disconnect 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_CallDisconnect()
{
	return BT_CallDisconnected();
}

/*****************************************************************************/
//  Discription: bt audio transfer 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_TransferAudioConn(uint8 direct)
{
    //set audio transfer operation
    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_TRANSFER_AUDIO);	
	return BT_TransferAudioConn(direct);
}

/*********************s********************************************************/
// Description : set bt volume
// Global resource dependence : 
// Author: baokun.yin
// Note:2007-8-6
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_SetVolume(uint8 channel, uint8 value)
{
	return BT_SetVolume(channel,value);
}

//mdy by qing.yu@BT_BQBfor cr238867
/*********************s********************************************************/
// Description : get bt volume
// Global resource dependence : 
// Author: qing.yu
// Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIBT_GetVolume(uint8 channel)
{
    return BT_GetVolume(channel);
}

/*****************************************************************************/
// Description : 蓝牙开关设置页面
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIBT_SetBluetoothMode ()
{
    if(MMIAPIBT_IsBtOn())
    {
        return MMIBT_SetBluetoothMode();
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
// 	Description : set ftp root folder 
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_SetFtpRootFolder(MMIFILE_DEVICE_E storage)
{
	BOOLEAN ret = FALSE; 
	if(BT_SUCCESS == MMIBT_SetFtpRootFolder(storage))
	{
		ret = TRUE;
	}
	return ret;
}

/*****************************************************************************/
//  Discription: blue tooth action make a outgoing call
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_OutgoingCall(void)
{
	BT_ADDRESS		address = {0};

	//if blue tooth is power on and visible, should hidden the device. recovery it after calling
	//MMIBT_CancelFileTransfer(FALSE);
	//MMIBT_HiddenBtForAPP(MMIBT_MODULE_CC,BT_NONE_ENABLE);
	
	//get if there is actived blue tooth headset	
	if (BT_GetActivatedHS(&address))
	{
		//SCI_TRACE_LOW:"MMIBT_OutgoingCall :Call the function: BT_StartAlert"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1039_112_2_18_2_7_49_57,(uint8*)"");
		BT_StartAlert();
	}
}

/*****************************************************************************/
//  Discription: blue tooth action when receive incoming call
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_IncomingCall(MMI_STRING_T		prompt_str)
{
	BT_ADDRESS		address = {0};
    uint8           number_buf[MMIPB_BCD_NUMBER_MAX_LEN << 1] = {0};
	uint16          num_len = 0;
	//if the blue tooth is receiving or sending file should cancel them. 
	//MMIBT_CancelFileTransfer(FALSE);

	//if blue tooth is power on and visible, should hidden the device. recovery it after calling
	//MMIBT_HiddenBtForAPP(MMIBT_MODULE_CC,BT_NONE_ENABLE);

	//get if there is actived blue tooth headset	
	if (BT_GetActivatedHS(&address))
	{
		//SCI_TRACE_LOW:"MMIBT_IncomingCall  :Call the function: BT_StartRing"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_1063_112_2_18_2_7_49_58,(uint8*)"");
		num_len = MIN(prompt_str.wstr_len, (MMIPB_BCD_NUMBER_MAX_LEN << 1)); 
		MMI_WSTRNTOSTR(number_buf, num_len, prompt_str.wstr_ptr, num_len, num_len);
		BT_StartRing(number_buf);
	}
}

/*****************************************************************************/
//  Discription: open blue tooth ui test window to test
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenUitestWin(BOOLEAN		bluetooth_on)
{
    MMIBT_SetOpertor(MMIBT_OPC_AUTO_TEST, MMIBT_OPC_INIT);
    MMIBT_OpenUitestWin(bluetooth_on);
}

/*****************************************************************************/
//  Discription: set save file location info 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_SaveFileLocation(MMIFILE_DEVICE_E storage)
{
	MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    setting.filedev = storage;
    MMIBT_SaveUserSetting(MMINV_BT_FILE_LOCATION, &setting);
}
	
/*****************************************************************************/
//  Discription: get  file location info 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIBT_GetFileLocation(void)
{
	MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
	MMIBT_GetFileUserSetting(MMINV_BT_FILE_LOCATION, &setting);
	return setting.filedev;
}
				
/*****************************************************************************/
//  Discription: Get if using blue tooth headset to call
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN		MMIAPIBT_IsHeadsetUsedInCall(void)
{
    return FALSE;
}
/**--------------------------------------------------------------------------*
**                          BLUETOOTH FUCNTIONS NOT IN WIN TAB
**                          BlueTooth Interface Functions implement
**                          *I**N**T**E**R**F**A**C**E*
**--------------------------------------------------------------------------**/
/*****************************************************************************/
//  Discription: This function is used to Init the Blue tooth sub app
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_AppInit(void)
{
#ifdef	BT_A2DP_SUPPORT
	A2DP_DEVICE_INIT_PARAM_T    device_param = {0};/*lint !e64*/
#endif
    uint8  bt_role = 0;
    uint32 app_handle = 0;
	
	g_bluetooth_app.ProcessMsg = AppHandleBTMsg;

    //6600I的REF支持A2DP，但MMI不支持。
#ifdef BT_A2DP_SUPPORT
	//init A2DP device parameter, this initialization should done before BT_Init() 
#ifdef MMI_BT_HIGH_QUALITY
	device_param.sample_rate= A2DP_AUDIO_SAMPLERATE_48K;  //支持的最大的采样率
	device_param.audio_quality = A2DP_AUDIO_QUALITY_HIGH; //音效
#else  
#ifdef MMI_BT_MID_QUALITY
	device_param.sample_rate= A2DP_AUDIO_SAMPLERATE_32K;  //支持的最大的采样率
	device_param.audio_quality = A2DP_AUDIO_QUALITY_MID; //音效
#else
	device_param.sample_rate= A2DP_AUDIO_SAMPLERATE_32K;  //支持的最大的采样率
	device_param.audio_quality = A2DP_AUDIO_QUALITY_MID; //音效
#endif
#endif

	device_param.need_create_link = TRUE;
#ifndef MMI_BT_HIGH_QUALITY
     device_param.support_trans_samplerate = TRUE;    
#endif
	device_param.is_sbccodec_run_interram = FALSE; //Is SBC Codec run in internal Ram
	device_param.callback = (A2DP_CALLBACK)MMIBT_A2DPCallBackFunc;

	A2DP_InitDevice(&device_param);
#endif

//#ifdef MMI_BLUETOOTH
	//Init blue tooth driver
	BT_Init((BT_CALLBACK)MMIBT_CallBackFunc);
//#endif
	//init operation
	MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);
	MMIBT_GetBtStatusForIcon();

	//set blue tooth file save location
	MMIBT_SetFileSaveLocation();
    MMIBT_Init_ActionQueue();
#ifdef BT_DIALER_SUPPORT
	MMIBT_Init_NVItems();
#endif

#ifdef ADULT_WATCH_SUPPORT
    ual_bt_dev_register(BT_HandleDevMsg,&app_handle);
#endif

#ifdef UAL_BT_HFU_SUPPORT
    bt_role =  UAL_BT_PHONE;//bug id 1591384/*UAL TODO,后续使用BBM接口*/
    ual_bt_set_product_role(bt_role);
#endif
}

/*****************************************************************************/
//  Discription: This function is used to deInit the Blue tooth sub app
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_AppDeInit(void)
{
	 //init bt operation
     MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);
     return BT_Deinit();
}

/*****************************************************************************/
// 	Description : intial bt module  
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_InitModule(void)
{
    MMIBT_RegMenuGroup();
    MMIBT_RegBtNv();
    MMIBT_RegWinIdNameArr();
}

/*****************************************************************************/
//  Discription: get the current bt address
//  Global resource dependence: none 
//  Author: samboo.shen
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_GetAddress(BT_ADDRESS *addr)
{
    if (PNULL == addr)
    {
        //SCI_TRACE_LOW:"MMIAPIBT_GetAddress PNULL == addr_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2301_112_2_18_2_7_52_229,(uint8*)"");
        return;
    }
	BT_GetBdAddr(addr);
}
extern BOOLEAN SSCHandle_RDA_BT_TEST_DUT(void);
/*****************************************************************************/
//  Description : if need A2DP to play the ring
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_EnterEutMode(void)
{
#ifdef BT_CHIP_RDA
#ifndef WIN32
    SSCHandle_RDA_BT_TEST_DUT();
    return BT_SUCCESS;
#endif
#endif


#ifdef BT_CHIP_BEKEN
#ifndef WIN32
    if(BEKEN_BT_TEST_DUT())
    {
    	return BT_SUCCESS;
    }
	else
	{
		return BT_ERROR;
	}
#endif
#else
#ifndef BT_DIALER_SUPPORT
	if(MMIAPISET_GetFlyMode())
	{
		return BT_SUCCESS;
	}
	else
#endif
	{
#ifdef BT_HL_PATCH_SUPPORT

#ifndef WIN32
        BTI_EutPatch();
#endif

		return BT_SUCCESS;
#else
        return BT_EnterEutMode();
#endif
	}
#endif
}

/*****************************************************************************/
//  Description : open or close blue toothe
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBlueTooth(BOOLEAN is_open)
{
	MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);
	MMIBT_OpenBluetooth(is_open, FALSE);
}
PUBLIC void MMIAPIBT_OpenBlueToothInFlyMode(BOOLEAN is_open)
{
	MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);
	MMIBT_OpenBluetooth(is_open, TRUE);
}

/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_Reset(void)
{
    MMIBT_Reset();
}

/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_ClearAllDevice(void)
{
    MMIBT_ClearAllDevice(TRUE);
}
/*****************************************************************************/
//  Description : pause a2dp device of playing mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpPauseDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
    return MMIBT_PauseDevice(a2dp_app_type);
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_A2DP_APP_TYPE_E MMIAPIBT_A2dpGetAppType(
                                    void
                                    )
{
    return MMIBT_GetA2dpAppType();
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_A2dpSetAppInfo(
                    MMIBT_A2DP_APP_INFO_T  a2dp_app_info
                    )
{
    MMIBT_SetAppInfo(a2dp_app_info);
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_A2dpClearAppInfo(
                    MMIBT_A2DP_APP_TYPE_E   app_type
                    )
{
    MMIBT_ClearAppInfo(app_type);
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpOpenDevice_1(
                    uint32 sample_ret
                    )
{
    return MMIBT_OpenDevice(sample_ret);
}

PUBLIC BOOLEAN MMIAPIBT_A2dpOpenDevice(
                    MMIBT_A2DP_APP_INFO_T  a2dp_app_info
                    )
{
    return FALSE;
}
/*****************************************************************************/
//  Description : Resume a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPIBT_A2dpResumeDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
       return MMIBT_ResumeDevice(a2dp_app_type);
}

/*****************************************************************************/
//  Description : stop a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpStopDevice_1(
                    void
                    )
{
	return MMIBT_StopDevice(0);
}

PUBLIC BOOLEAN MMIAPIBT_A2dpStopDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIBT_IsA2dpReadyForPlay(void)
{
#ifdef BT_A2DP_SUPPORT	
    return MMIBT_IsA2dpReadyForPlay();
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIBT_IsA2dpPaused(void)
{
#ifdef BT_A2DP_SUPPORT
    return MMIBT_IsA2dpPaused();
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : get the sample rate of a2dp stream
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIBT_GetA2DPSampleRate(void)
{
#ifdef BT_A2DP_SUPPORT
	return MMIBT_GetA2DPSampleRate();
#else
    return 0;
#endif
}

/*****************************************************************************/
//  Discription: get blue tooth status from nv and show icon on the idle win
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_GetBtStatusForIcon(void)
{
    MMIBT_GetBtStatusForIcon();
}
/*****************************************************************************/
//  Discription: send Multi_file to other device cancelled
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_TransferCancelled(void)
{
    cursendfileindex = 0;
    send_file_num = 0; 
    MMIBT_TransferCancelled(TRUE);
}

/*****************************************************************************/
//  Discription: Get Call held status
//               call status
//  Global resource dependence: None
//  Author: yang.yang 
//  Note : 为BQB更新，底层协议要求支持3种情况:
//          1.没有hold call---0
//          2.只有hold call---2
//          3.既有hold call，又有active call ---1
//  Return:
/*****************************************************************************/
PUBLIC uint8 MMIAPIBT_GetCallHeldStatus(void)
{
    uint8 call_status = 0;
        
    if(MMIAPICC_GetHoldCallCount() == 0)//no hold call existed
    {
        call_status = 0;
    }
    else
    {
        if(MMIAPICC_GetActiveCallCount() > 0)//both hold call and active call existed
        {
            call_status = 1;
        }
        else//only hold call existed(no active call)
        {
            call_status = 2;
        }            
    }

    return call_status;
}

#ifdef BT_BQB_SUPPORT
//mdy by qing.yu@for cr238605 cr238609
/*****************************************************************************/
//  Discription: request sending +CIEV command, inform HF side about the current
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_CIEVCommandIndicator(uint32 ciev_command,uint32 command_status)
{
    BT_ADDRESS address = {0};
    if (BT_GetActivatedHS(&address))
    {
        BT_CIEVCommandIndicator(ciev_command, command_status);
    }
}

/*****************************************************************************/
//  Discription: Set Call held status
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 为BQB更新，底层协议要求支持3种情况:
//          1.没有hold call---0
//          2.只有hold call---2
//          3.既有hold call，又有active call ---1
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallHeldStatus(void)
{
    BT_ADDRESS address = {0}; 
    uint8     call_status = 0;

    if (BT_GetActivatedHS(&address))
    {
        call_status = MMIAPIBT_GetCallHeldStatus();
        
        SCI_TRACE_LOW("[BT_BQB]MMIAPIBT_SetCallHeldStatus: call_status =%d",call_status);
        BT_CIEVCommandIndicator(BT_CALL_HELD_INDICATOR, call_status);
    }
}

/*****************************************************************************/
//  Discription: Set Signal Strength Information
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetSignalStrength(uint32 status)
{
    BT_ADDRESS address = {0};
    if (BT_GetActivatedHS(&address))
    {
        BT_CIEVCommandIndicator(BT_SIGNAL_STRENGTH_INDICATOR, status);
    }
}

/*****************************************************************************/
//  Discription: Set Battery Charge Information
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetBatteryCharge(uint32 status)
{
    BT_ADDRESS address = {0};
    if (BT_GetActivatedHS(&address))
    {
        BT_CIEVCommandIndicator(BT_BATTERY_CHARGE_INDICATOR, status);
    }
}

#if 0
//mdy by qing.yu@for cr238585
/*****************************************************************************/
//  Description : GetExistSIMType

//  Global resource dependence : 
//  Author: qing.yu
//  Note: 
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GetExistSIMType(void)
{
    uint16 dual = MN_DUAL_SYS_1;
       
    MMIAPIPHONE_GetSimOkNum(&dual, 1);

    return (MN_DUAL_SYS_E)dual;
}
#endif

/*****************************************************************************/
//  Discription: set the name of net operator
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetOperatorName(void)
{
    MMI_STRING_T network_str = {0};
    MMI_TEXT_ID_T text_id = TXT_NULL;
    uint16 sim_ok = MN_DUAL_SYS_1;
    uint8 operatorName[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    MMI_STRING_T        netname_str    = {0};
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
    uint32 i = 0;
    if(0 < MMIAPIPHONE_GetSimOkNum(&sim_ok, 1))
    {


        SCI_MEMSET(&netname_str, 0x00, sizeof(netname_str));
        netname_str.wstr_ptr = (wchar *)opn_spn_name;
        SCI_MEMSET(netname_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
        /*1.get operator name from network
            2.get from local db
            3.NO service*/
        MMIAPIPHONE_GetSimAndServiceString((MN_DUAL_SYS_E)sim_ok,&netname_str);

        if(PNULL != netname_str.wstr_ptr)
        {
            MMIAPICOM_WstrToStr(netname_str.wstr_ptr,operatorName);
        }else
        {
            text_id = MMIAPIPHONE_OpeartorTextID((MN_DUAL_SYS_E)sim_ok);

            for(i = 0; i < sizeof(s_bt_network_nametable)/sizeof(MMIBT_NETWORK_NAME_T); i++)
            {
                if(s_bt_network_nametable[i].name_index == text_id)
                {
                    break;
                }
            }

            if(i < sizeof(s_bt_network_nametable)/sizeof(MMIBT_NETWORK_NAME_T))
            {
                SCI_STRCPY((char*)operatorName, &s_bt_network_nametable[i].name[0]);
            }
            else
            {
                SCI_STRCPY((char*)operatorName, (char*)"No service");
            }

        }

        BT_SetNameOfNetOperator(operatorName);
    }
    else
    {
        SCI_TRACE_LOW("sim is not ok");
        SCI_STRCPY((char*)operatorName, (char*)"No service");
        BT_SetNameOfNetOperator(operatorName);
    }
    //SCI_TRACE_LOW:"MMIAPIBT_SetOperatorName name <%s>"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2610_112_2_18_2_7_52_230,(uint8*)"s", operatorName);
}

/*****************************************************************************/
//  Discription: set infomation of call
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallInfo(
    uint8 call_index,
    uint8 call_dir,
    uint8 call_status,
    uint8 mode,
    uint8 mpty,
    uint8* number,
    uint8* call_type)
{
	//SCI_TRACE_LOW:"MMIAPIBT_SetCallInfo idx %d dir %d call_status %d mode %d mpty %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2627_112_2_18_2_7_52_231,(uint8*)"ddddd",call_index, call_dir, call_status, mode, mpty);
	//SCI_TRACE_LOW:"MMIAPIBT_SetCallInfo number  %s"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2628_112_2_18_2_7_52_232,(uint8*)"s",number);
	BT_ClccCommandSend(call_index +1,call_dir,call_status,mode,mpty, number,PNULL);
}

/*****************************************************************************/
//  Description : update network status

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetNetWorkRoamStatus(BOOLEAN is_in_roam)
{
	uint32 roam = BT_ROAM_OFF;
	if(is_in_roam)
	{
		roam = BT_ROAM_ON;
	}
	//SCI_TRACE_LOW:"MMIAPIBT_SetNetWorkRoamStatus roam %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2644_112_2_18_2_7_52_233,(uint8*)"d", roam);
    //MMIBT_A2DPSetClose_Reason(reason);
	BT_CIEVCommandIndicator(BT_ROAM_INDICATOR,roam);
}

/*****************************************************************************/
//  Description : set my phone number

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetMyPhoneNumber(void)
{
    uint8 tele_number[CC_MAX_TELE_NUM_LEN + 2] = {0};
    MMIPB_BCD_NUMBER_T number = {0};
    uint8 max_tele_len  = CC_MAX_TELE_NUM_LEN + 2;

    MMIAPIPB_GetMSISDNumber(&number);
    if(number.number_len > 0)
    {
        MMIAPIPB_BCDNumberToString(&number, max_tele_len, tele_number);
    }
    //SCI_TRACE_LOW:"MMIAPIBT_SetMyPhoneNumber num_buf %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2671_112_2_18_2_7_52_234,(uint8*)"s", tele_number);
    BT_SetLocalNumber(tele_number);   
}

/*****************************************************************************/
//  Description : inform bt has number to call

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallSuccess(void)
{
    //SCI_TRACE_LOW:"MMIAPIBT_SetCallSuccess "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2682_112_2_18_2_7_52_235,(uint8*)"");
    BT_ATStatusSend(TRUE);
}

/*****************************************************************************/
//  Description : inform bt no number to call

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallFail(void)
{
	//SCI_TRACE_LOW:"MMIAPIBT_SetCallFail "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2694_112_2_18_2_7_52_236,(uint8*)"");

	BT_ATStatusSend(FALSE);
}

/*****************************************************************************/
//  Description : Set Service is valid
//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_Service_Indicator(BOOLEAN is_in_net)
{
	uint32 cmd_value = (is_in_net ? BT_SERVICE_PRESENT_VALUE : BT_NO_SERVICE_VALUE);
	//SCI_TRACE_LOW:"MMIAPIBT_Service_Indicator cmd_value %d is_in_net %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_EXPORT_2708_112_2_18_2_7_52_237,(uint8*)"dd", cmd_value, is_in_net);

	BT_CIEVCommandIndicator(BT_SERVICE_INDICATOR, cmd_value);
}

/*****************************************************************************/
//  Description : Open BT Paried Device Windos
//  Global resource dependence : 
//  Author: qing.yu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_PariedHandFree(void)
{
    MMIBT_PariedHandFree();
}
#endif //BT_BQB_SUPPORT
/*****************************************************************************/
//  Description : Open BT transfer file win(send/receive)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenTransferWin(void)
{
    MMIBT_OpenTransferWin();
}
/*****************************************************************************/
//  Description : stop BT transfer file (send/receive)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_StopBTTransferFile(void)
{
    MMIBT_StopBTTransferFile();
}
/*****************************************************************************/
//  Discription:  MMIAPIBT_CloseStorageMenu
//  Global 
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_CloseStorageMenu(void)
{
    MMIBT_CloseStorageMenu();
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : receive msg: MMIBT_IsHfuConnected
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsHfuConnected(void)
{
    uint16	device_number =0;
    BT_DEVICE_INFO dev = {0};
    uint16      index =0;
    BOOLEAN connected = FALSE;
    
    device_number = BT_GetPairedDeviceCount(BT_SERVICE_ALL);
    
    for (index = 1; index <= device_number; index++)
    {
        BT_GetPairedDeviceInfo(BT_SERVICE_ALL, index, &dev);
        SCI_TRACE_LOW("BT_AV, paired address: %d:%d:%d:%d:%d:%d", dev.addr.addr[5], dev.addr.addr[4], dev.addr.addr[3], dev.addr.addr[2], dev.addr.addr[1], dev.addr.addr[0]);
        
#ifndef WIN32
        connected = BT_HfuGetConnStatus(&dev.addr);
        SCI_TRACE_LOW("BT_AV, is connected: %d", connected);
#else
        connected = TRUE;
#endif
        
        if(connected)
        {
            SCI_TRACE_LOW("BT_AV, connected address: %02X-%02X-%02X-%02X-%02X-%02X", dev.addr.addr[5], dev.addr.addr[4], dev.addr.addr[3], dev.addr.addr[2], dev.addr.addr[1], dev.addr.addr[0]);
            return TRUE;
        }
    }
    return connected;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_DisconnectDevice(void)
{
	MMIBT_DisconnectDevice();
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_ConnectDevice(void)
{
	BT_ADDRESS dev_addr = {0};

	if(MMIBT_HasConnected(&dev_addr))
	{
		MMIBT_ConnectDevice(&dev_addr);
	}
	else
	{
		SCI_TRACE_LOW("[MMIBT] MMIAPIBT_ConnectDevice device is disconnected!");
	}
}

/*****************************************************************************/
//  Description : MMIAPIBT_IsConnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsConnectDevice(void)
{
	BT_ADDRESS dev_addr = {0};

	return ((BT_GetState()) && (MMIBT_IsConnectDevice(&dev_addr)));
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuSwitchAudio(BOOLEAN is_audio_on)
{
#ifdef WIN32
    return BT_SUCCESS;
#else
    return BT_HfuSwitchAudio(is_audio_on);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuDialNumber(const uint8 *phone_num)
{
#ifdef WIN32
	return BT_SUCCESS;
#else
	return BT_HfuDialNumber(phone_num);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuAnswerCall(void)
{
#ifdef WIN32
	return BT_SUCCESS;
#else
    return BT_HfuAnswerCall();
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuListCurrentCalls(void)
{
#ifdef WIN32
	return BT_SUCCESS;
#else
    return BT_HfuListCurrentCalls();
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuHangup(void)
{
#ifdef WIN32
	return BT_SUCCESS;
#else
    return BT_HfuHangup();
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuSendDtmf(uint8 dtmfTone)
{
    SCI_TRACE_LOW("[MMIBT] BT_HfuSendDtmf");
    
#ifdef WIN32
	return BT_SUCCESS;
#else
    return BT_HfuSendDtmf(dtmfTone);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuCallHold(uint8 hold_action)
{
#ifdef WIN32
	return BT_SUCCESS;
#else
    return BT_HfuCallHold(hold_action);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuRedial(void)
{
#ifdef WIN32
	return BT_SUCCESS;
#else
    return BT_HfuRedial();
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuIsAGVoiceRecognition(BOOLEAN *is_support)
{
#ifdef WIN32
	return BT_SUCCESS;
#else
    return BT_HfuIsAGSupportVoiceRecognition(is_support);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuEnableVoiceRecognition(BOOLEAN is_enable)
{
#ifdef WIN32
	return BT_SUCCESS;
#else
    return BT_HfuEnableVoiceRecognition(is_enable);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuIsAGSupportThreeWayCall(BOOLEAN *is_support,BT_HFU_GW_HOLD_FEATUERS_T *GwHoldFeatures)
{
#ifdef WIN32
	return BT_SUCCESS;
#else
    return BT_HfuIsAGSupportThreeWayCall(is_support , GwHoldFeatures);
#endif
}

/*****************************************************************************/
//  Description : Get HFU connected device BT address
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetHfuConnectedAddress(BT_ADDRESS *addr)
{
    return MMIBT_GetHfuConnectedAddress(addr);
}

#ifdef MMI_SMS_SYN_SUPPORT
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetSmsSynVal(void)
{
    return MMIBT_GetSmsSynValue();
}
#endif

#endif

#ifdef BT_SPP_SUPPORT
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_SppWriteData(const uint8 *src_buf, uint32 data_len)
{
#ifndef WIN32
    BT_SppWriteData(src_buf, data_len);
#endif
}
#endif

/**--------------------------------------------------------------------------*
 **                 dummy funciton implement                                 *
 **--------------------------------------------------------------------------*/
#else  //if undef MMI_BLUETOOTH

/*****************************************************************************/
//  Discription: This fucntion is used to open the bluetooth main menu win  
//  
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenMainMenuWin(void)
{

}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Discription: This fucntion is used to open the BtPhone main menu win  
//  
//  Global resource dependence: None
//  Author: yang.yang 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBtPhoneWin(void)
{

}
#endif

/*****************************************************************************/
//  Discription: send file with blue tooth
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_SendMultiFile(MMIFMM_BT_SENDFILE_INFO_T *send_file_info_ptr,int file_num)
{

}

/*****************************************************************************/
//  Discription: get bt device status
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:TRUE: bt device on. FALSE: bt device off
//
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIBT_IsBtOn(void)
{
	return FALSE;
}

/*****************************************************************************/
//  Discription: bt remote alert
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_RemoteAlerting(void)
{
    return BT_SUCCESS;
}

/*****************************************************************************/
//  Discription: bt call connect
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_SetBtCallStatus(uint32 status)
{
    return BT_SUCCESS;
}

/*****************************************************************************/
//  Discription: bt call connect
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_CallConnect(BT_ADDRESS *addr)
{
	return BT_SUCCESS;
}

/*****************************************************************************/
//  Discription: bt get active handfree 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIBT_GetActivatedHS(BT_ADDRESS *addr)
{
	return FALSE;
}

/*****************************************************************************/
//  Discription: bt Stop Alert 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_StopAlert()
{
	return BT_SUCCESS;
}

/*****************************************************************************/
//  Discription: bt Stop Ring 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_StopRing()
{
	return BT_SUCCESS;
}

/*****************************************************************************/
//  Discription: bt call disconnect 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_CallDisconnect()
{
	return BT_SUCCESS;
}

/*****************************************************************************/
//  Discription: bt audio transfer 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_TransferAudioConn(uint8 direct)
{
	return BT_SUCCESS;
}

/*****************************************************************************/
//  Discription: if blue tooth open send or receive file win
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPIBT_IsOpenFileWin(void)
{
	return FALSE;
}
								
/*****************************************************************************/
//  Discription: This fucntion is used to Init bluetooth app 
//  recive the bt app response 
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_AppInit(void)
{
	
}

/*****************************************************************************/
//  Discription: This function is used to deInit the Blue tooth sub app
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_AppDeInit(void)
{
	return BT_SUCCESS;
}

/*****************************************************************************/
//  Discription: Get if using blue tooth headset to call
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN		MMIAPIBT_IsHeadsetUsedInCall(void)
{
	return FALSE;
}

/*****************************************************************************/
// 	Description : intial bt module  
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_InitModule(void)
{
	return;
}

/*****************************************************************************/
//  Discription: blue tooth action make a outgoing call
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_OutgoingCall(void)
{
}

/*****************************************************************************/
//  Discription: blue tooth action when receive incoming call
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_IncomingCall(MMI_STRING_T		prompt_str)
{
}

/*****************************************************************************/
//  Discription: open blue tooth ui test window to test
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenUitestWin(BOOLEAN		bluetooth_on)
{
}
/*****************************************************************************/
// Description : 蓝牙开关设置页面
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_SetVolume(uint8 channel, uint8 value)
{
    return BT_SUCCESS;
} 

//mdy by qing.yu@for cr238867
/*********************s********************************************************/
// Description : get bt volume
// Global resource dependence : 
// Author: qing.yu
// Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIBT_GetVolume(uint8 channel)
{
    return 0;
}

/*****************************************************************************/
// Description : 蓝牙开关设置页面
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIBT_SetBluetoothMode ()
{
    return 0;
}

/*****************************************************************************/
//  Discription: get the current bt address
//  Global resource dependence: none 
//  Author: samboo.shen
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_GetAddress(BT_ADDRESS *addr)
{
}

/*****************************************************************************/
//  Discription: get blue tooth status from nv and show icon on the idle win
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_GetBtStatusForIcon(void)
{
}
/*****************************************************************************/
//  Discription: set save file location info 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_SaveFileLocation(
									MMIFILE_DEVICE_E storage
									)
{
}
#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
// 	Description : set ftp root folder 
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_SetFtpRootFolder(MMIFILE_DEVICE_E storage)
{
	return TRUE;
}
#endif
/*****************************************************************************/
// 	Description : set ftp root folder 
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_EnterEutMode(void)
{
	return BT_SUCCESS;
}

/*****************************************************************************/
//  Discription: get  file location info 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIBT_GetFileLocation(void)
{
	return MMI_DEVICE_UDISK;
}

/*****************************************************************************/
//  Description : open or close blue toothe
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBlueTooth(BOOLEAN is_open)
{
}
/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_Reset(void)
{
}
//#else//not define BLUETOOTH_SUPPORT

/*****************************************************************************/
//  Description : pause a2dp device of playing mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpPauseDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
	return TRUE;
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_A2DP_APP_TYPE_E MMIAPIBT_A2dpGetAppType(
                                    void
                                    )
{
    return MMIBT_A2DP_AUDIO_NONE;
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_A2dpSetAppInfo(
                    MMIBT_A2DP_APP_INFO_T  a2dp_app_info
                    )
{
    return;
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_A2dpClearAppInfo(
                    MMIBT_A2DP_APP_TYPE_E   app_type
                    )
{
    return;
}

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpOpenDevice(
                    MMIBT_A2DP_APP_INFO_T  a2dp_app_info
                    )
{
	return TRUE;
}
/*****************************************************************************/
//  Description : Resume a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPIBT_A2dpResumeDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
	return TRUE;
}

/*****************************************************************************/
//  Description : stop a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpStopDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    )
{
	return TRUE;
}

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIBT_IsA2dpReadyForPlay(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIBT_IsA2dpPaused(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : get the sample rate of a2dp stream
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIBT_GetA2DPSampleRate(void)
{
	return 0;
}

#ifdef BT_BQB_SUPPORT
//mdy by qing.yu@for cr238605 cr238609
/*****************************************************************************/
//  Discription: request sending +CIEV command, inform HF side about the current
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_CIEVCommandIndicator(uint32 ciev_command,uint32 command_status)
{
    return;
}

/*****************************************************************************/
//  Discription: Set Call held status
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallHeldStatus(void)
{
    return;
}

/*****************************************************************************/
//  Discription: Set Signal Strength Information
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetSignalStrength(uint32 status)
{
    return;
}

/*****************************************************************************/
//  Discription: Set Battery Charge Information
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetBatteryCharge(uint32 status)
{
    return;
}


/*****************************************************************************/
//  Description : update network status

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetNetWorkRoamStatus(BOOLEAN is_in_roam)
{
	return;
}

/*****************************************************************************/
//  Discription: set infomation of call
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallInfo(
    uint8 call_index,
    uint8 call_dir,
    uint8 call_status,
    uint8 mode,
    uint8 mpty,
    uint8* number,
    uint8* call_type)
{
    return;
}


//mdy by qing.yu@for cr238585
/*****************************************************************************/
//  Discription: set the name of net operator
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetOperatorName(void)
{
	return;
}

//mdy by qing.yu@for cr238786 cr238787
/*****************************************************************************/
//  Description : inform bt has number to call

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallSuccess(void)
{
	return;
}


/*****************************************************************************/
//  Description : inform bt no number to call

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallFail(void)
{
	return;
}
/*--- mdy by qing.yu*/

/*****************************************************************************/
//  Description : set my phone number

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetMyPhoneNumber(void)
{
    return;
}

/*****************************************************************************/
//  Description : Set Service is valid
//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_Service_Indicator(BOOLEAN is_in_net)
{
    return;
}

/*****************************************************************************/
//  Description : Open BT Paried Device Windos
//  Global resource dependence : 
//  Author: qing.yu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_PariedHandFree(void)
{
    return;
}

#endif //BT_BQB_SUPPORT
/*****************************************************************************/
//  Discription:  MMIAPIBT_CloseStorageMenu
//  Global 
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_CloseStorageMenu(void)
{
   return ;
}
#endif// MMI_BLUETOOTH_A2DP
#ifndef BT_DUN_SUPPORT
/*****************************************************************************/
// 	Description :get vuart run status 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_DUNIsRun(void)
{
	return FALSE;
}

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_CloseDUNWin(void)
{
}
#endif//USE_BT_DG

/*****************************************************************************/
//  Description : Open alert tip win
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC void MMIBT_OpenAlertTipWin(MMI_TEXT_ID_T text_id)
{
    MMI_WIN_ID_T   alert_id = MMI_BT_ALERT_WIN_ID; 
    MMI_IMAGE_ID_T image_id = IMAGE_PUBWIN_WARNING;

    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,text_id,TXT_NULL,image_id,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL,0); 
}

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Discription: MMIAPIBT_IsPbapCancel
//  Global resource dependence: none 
//  Author: Allen Lee
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsPbapCancel(void)
{
    return MMIBT_GetPbapCancelValue();
}

/*****************************************************************************/
//  Discription: MMIAPIBT_SetPbapCancel
//  Global resource dependence: none 
//  Author: Allen Lee
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetPbapCancel(BOOLEAN is_cancel)
{
    MMIBT_SetPbapCancelValue(is_cancel);
}

/*****************************************************************************/
//  Discription: check if pbap is connected
//  Global resource dependence: none 
//  Author: cheney wu
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_PbapIsConnected(void)
{
#ifndef WIN32
    return BT_PbapIsConnected();
#else
    return TRUE;
#endif
}

#endif

#ifdef BT_AV_SNK_SUPPORT
/*****************************************************************************/
//  Description : Create a transport connection for A2DP Sink
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_A2dpSinkConnect(BT_ADDRESS addr)
{
    return MMIBT_A2dpSinkConnect(addr);
}

/*****************************************************************************/
//  Description : Disconnect for A2DP Sink
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_A2dpSinkDisconnect(void)
{
    return MMIBT_A2dpSinkDisconnect();
}

/*****************************************************************************/
//  Description : A2DP SINK connection status
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsA2dpSinkConnected(void)
{
    return MMIBT_IsA2dpSinkConnected();
}

/*****************************************************************************/
//  Description : HFU connection status
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_isHfuConnected(void)
{
    return MMIBT_IsHfuConnected();
}

#endif


#ifdef BT_MAP_SUPPORT
/*****************************************************************************/
//  Discription: check if map is connected
//  Global resource dependence: none 
//  Author: cheney wu
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_MapIsConnected(void)
{
#ifndef WIN32
    return BT_MapIsConnected();
#else
    return TRUE;
#endif
}

/*****************************************************************************/
//  Description : Deregisters the Map Access Client
//  Global resource dependence :                                
//  Author: Candice
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_MapClientDeregister(void)
{
    MMIBT_MapClientDeregister();
}

#endif

#ifdef BT_AV_CT_SUPPORT
/*****************************************************************************/
//  Description : Create a transport connection for AVRCP CT
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_AvrcpCtConnect(BT_ADDRESS addr)
{
    return MMIBT_AvrcpCtConnect(addr);
}

/*****************************************************************************/
//  Description : Disconnect for AVRCP CT
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_AvrcpCtDisconnect(void)
{
    return MMIBT_AvrcpCtDisconnect();
}

/*****************************************************************************/
//  Description : AVRCP CT connection status
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsAvrcpCtConnected(void)
{
    return MMIBT_IsAvrcpCtConnected();
}

/*****************************************************************************/
//  Description : Execute Avrcp Operation
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_SetAvrcpPanelKey(uint16 op, BOOLEAN press, BOOLEAN auto_release)
{
    return MMIBT_SetAvrcpPanelKey(op,press,auto_release);
}

/*****************************************************************************/
//  Description : Get Mda play status
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_GetMdaPlayStatus(void)
{
    return MMIBT_GetMdaPlayStatus();
}

/*****************************************************************************/
//  Description : set a2dp sink audio volume
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetA2dpSinkVolume(uint32 volume)
{
    MMIBT_SetA2dpSinkVolume(volume);
}

/*****************************************************************************/
//  Description : get a2dp sink sample rate
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIBT_GetA2dpSinkSampleRate(void)
{
    return MMIBT_GetA2dpSinkSampleRate();
}

/*****************************************************************************/
//  Description : get a2dp sink channel num
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIBT_GetA2dpSinkchannelNum(void)
{
    return MMIBT_GetA2dpSinkChannelNum();
}

/*****************************************************************************/
//  Description : route a2dp sink audio
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_RouteA2dpSinkAudio(void)
{
    MMIBT_RouteA2dpSinkAudio();
}
#endif

#ifdef BT_HID_SUPPORT
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsHIDConnected(void)
{
#ifdef WIN32
    return FALSE;
#else
    return BT_HID_IsConnect();
#endif
}

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HIDConnect(BT_ADDRESS addr)
{
#ifdef WIN32
    return BT_ERROR;
#else
    return BT_HID_Connect(addr);
#endif
}

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HIDDisconnect(void)
{
#ifdef WIN32
    return BT_ERROR;
#else
    return BT_HID_Disconnect();
#endif
}
#endif

