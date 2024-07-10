/*****************************************************************************
** File Name:      mmiplmn_info.c                                                *
** Author:                                                                   *
** Date:           09/2011                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe PLMN Info module                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2011       wancan.you       Create
******************************************************************************/

#define _MMIPLMN_INFO_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mn_api.h"
#include "mmi_data.h"
#include "mmiplmn_export.h"
#include "mmisms_export.h"
#include "mmi_nv.h"
#include "mmi_custom_define.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
//#include "mmicc_speeddial.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#ifdef WIN32
/*****************************************************************************/
//  Description : keyboard shortcuts of simulator debug
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void plmn_keyboard_shortcuts_debug(MMI_MESSAGE_ID_E msg_id);
#endif
#ifdef ORANGE_SUPPORT
/*****************************************************************************/
//  Description : keyboard shortcuts of Tunisia
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void plmn_keyboard_shortcuts_tunisia(MMI_MESSAGE_ID_E msg_id);
#endif
#endif

/*---------------------------------------------------------------------------*/
/*                          CONSTANT DATA                               */
/*---------------------------------------------------------------------------*/
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#ifdef ORANGE_SUPPORT
//Armenia Orange Network Operator
LOCAL const MMIPLMN_EMERGENCY_CALL_T emergency_call_list_armenia[MMIPLMN_EMERGENCY_CALL_MAX + 1] =
{
    {"101"},
    {"102"},
    {"103"},
    {"104"},
    {"112"},
    {"188"},
     0
};

LOCAL const MMIPLMN_LANGUAGE_T language_settings_armenia =
{
    MMISET_LANGUAGE_ENGLISH
};

LOCAL const MMIPLMN_SMS_T sms_settings_armenia =
{
    "37495980150",
    MMISMS_VALID_PERIOD_86400,//24 hours
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_UCS2
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_armenia =
{
    {283, 10, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    &emergency_call_list_armenia,
    PNULL,
    &language_settings_armenia,
    &sms_settings_armenia,
    120*1000,//pdp time out,120S
    300*1024,//300KB
    PNULL,
};

//Australia Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_australia =
{
    MMISET_LANGUAGE_GERMAN
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_australia =
{
    {232, 5, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_australia,
    PNULL,
    80*1000,//80s
    300*1024,//300KB
    PNULL
};

//Belgium Orange Network Operator
LOCAL const MMIPLMN_EMERGENCY_CALL_T emergency_call_list_belgium[MMIPLMN_EMERGENCY_CALL_MAX + 1] =
{
    {"112"},
     0
};

LOCAL const MMIPLMN_LANGUAGE_T language_settings_belgium =
{
    //MMISET_LANGUAGE_ENGLISH
    MMISET_LANGUAGE_FRENCH//@@@for test
};

LOCAL const MMIPLMN_SMS_T sms_settings_belgium =
{
    "+32495002530",
    MMISMS_VALID_PERIOD_MAX,//max
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_belgium=
{
    "Logos",
    "http://m.mobistar.be/logos",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_belgium=
{
    "Tones",
    "http://m.mobistar.be/tones",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_belgium=
{
    "Videos",
    "http://m.mobistar.be/video",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_belgium=
{
    "Games",
    " http://m.mobistar.be/games",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_belgium =
{
    &shortcut_picture_detail_belgium,
    &shortcut_sound_detail_belgium,
    &shortcut_video_detail_belgium,
    &shortcut_game_detail_belgium,
    PNULL,
    PNULL,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_belgium =
{
    {206, 20, 2},//BASE
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //BROWSER_SUPPORT  ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_BELGIUM,
#else
    NULL,
#endif
    &emergency_call_list_belgium,
    PNULL,
    &language_settings_belgium,
    &sms_settings_belgium,
    150*1000,//150s
    300*1024,//300KB
    &entertainment_shortcuts_belgium,
};

//Bissau Guinea Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_bisaau_guinea =
{
    MMISET_LANGUAGE_PORTUGUESE
};

LOCAL const MMIPLMN_SMS_T sms_settings_bisaau_guinea =
{
    "+2455080010",
    MMISMS_VALID_PERIOD_MAX,//max
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_bisaau_guinea =
{
    {632, 3, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0//BROWSER_SUPPORT ÐÞ¸Ä131801±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_BISSAU_GUINEA,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_bisaau_guinea,
    &sms_settings_bisaau_guinea,
    120*1000,//120s
    NULL,
    PNULL
};

//Botswana Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_botswana =
{
    MMISET_LANGUAGE_ENGLISH
};

LOCAL const MMIPLMN_SMS_T sms_settings_botswana =
{
    "+26772000003",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_botswana =
{
    {652, 2, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0//BROWSER_SUPPORT ÐÞ¸Ä131801±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_BOTSWANA,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_botswana,
    &sms_settings_botswana,
    120*1000,//120s
    NULL,
    PNULL
};

//Cameroon Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_cameroon =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_cameroon =
{
    "+23799900929",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_cameroon =
{
    {624, 2, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_CAMEROON,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_cameroon,
    &sms_settings_cameroon,
    120*1000,//120s
    NULL,
    PNULL
};

//Caribbean Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_caribbean =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_caribbean =
{
    "+590690350012",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_caribbean =
{
    {340, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_CARIBBEAN,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_caribbean,
    &sms_settings_caribbean,
    120*1000,//120s
    NULL,
    PNULL
};

#if 0
//Central African Republic Orange Network Operator
LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_central_african_republic =
{
    {206, 20, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    NULL,
    NULL
};
#endif

//Conakry Guinea Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_conakry_guinea =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_conakry_guinea =
{
    "+22462800010",
    MMISMS_VALID_PERIOD_259200,//72hours
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_conakry_guinea =
{
    {611, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_conakry_guinea,
    &sms_settings_conakry_guinea,
    120*1000,//120s
    NULL,
    PNULL
};

//Cote d'Ivoire Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_cote_divoire =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_cote_divoire =
{
    "+22507070002",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_cote_divoire =
{
    {612, 3, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_COTE_DIVOIRE,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_cote_divoire,
    &sms_settings_cote_divoire,
    120*1000,//120s
    NULL,
    PNULL
};

//Dominican Republic Orange Network Operator
LOCAL const MMIPLMN_EMERGENCY_CALL_T emergency_call_list_dominican[MMIPLMN_EMERGENCY_CALL_MAX + 1] =
{
    {"*411"},//stolen
    {"911"},//official emergency number of country
    {"*511"},//customer service
     0
};

LOCAL const MMIPLMN_LANGUAGE_T language_settings_dominican =
{
    MMISET_LANGUAGE_SPANISH
};

LOCAL const MMIPLMN_SMS_T sms_settings_dominican =
{
    "8098599999",
    MMISMS_VALID_PERIOD_604800,//7Days
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_dominican =
{
    {370, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_DOMINICAN,
#else
    NULL,
#endif
    &emergency_call_list_dominican,
    PNULL,
    &language_settings_dominican,
    &sms_settings_dominican,
    120*1000,//120s
    NULL,
    PNULL
};

//Egypt Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_egypt =
{
    MMISET_LANGUAGE_ARABIC
};

LOCAL const MMIPLMN_SMS_T sms_settings_egypt =
{
    "+20122000020",
    MMISMS_VALID_PERIOD_MAX,//max
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_UCS2
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_egypt =
{
    {602, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_EGYPT,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_egypt,
    &sms_settings_egypt,
    NULL,//default
    500*1024,//500KB
    PNULL,
};

//Equatorial Guinea Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_equatorial_guinea =
{
    MMISET_LANGUAGE_SPANISH
};

LOCAL const MMIPLMN_SMS_T sms_settings_equatorial_guinea =
{
    "+240222279513",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_equatorial_guinea =
{
    {627, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_equatorial_guinea,
    &sms_settings_equatorial_guinea,
    NULL,
    NULL,
    PNULL
};

//France Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_france =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_france  =
{
    "+33689004000",
    MMISMS_VALID_PERIOD_MAX,//max
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET
};

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_france=
{
    "Plus d'images",
    "http://logos.mob2.orange.fr",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_ringtone_detail_france=
{
    "Plus de sonneries ",
    "http://sons.mob2.orange.fr",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_france=
{
    "Plus de musique",
    "http://musicstore.mob2.orange.fr",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_france=
{
    "Plus de vid¨¦os ",
    "http://univideo.mob2.orange.fr ",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_france=
{
    "Plus de jeux",
    "http://jeux2.mob2.orange.fr",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_france =
{
    &shortcut_picture_detail_france,
    &shortcut_sound_detail_france,
    &shortcut_video_detail_france,
    &shortcut_game_detail_france,
    &shortcut_ringtone_detail_france,
    PNULL,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_france =
{
    {208, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_FRANCE,
#else
    NULL,
#endif
    NULL,
    PNULL,
    &language_settings_france,
    &sms_settings_france,
    120*1000,//120s
    600*1024,//600KB
    &entertainment_shortcuts_france,
};

//Jordan Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_jordan =
{
    MMISET_LANGUAGE_ARABIC
};

LOCAL const MMIPLMN_SMS_T sms_settings_jordan =
{
    "96277000161",
    MMISMS_VALID_PERIOD_259200,//3days
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO//ÐèÒªÖ§³Ö¿ÉÅäÖÃ
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_jordan =
{
    {416, 77, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_JORDAN,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_jordan,
    &sms_settings_jordan,
    120*1000,//120s
    300*1024,//300KB
    PNULL,
};

//Kenya Orange Network Operator
LOCAL const MMIPLMN_EMERGENCY_CALL_T emergency_call_list_kenya[MMIPLMN_EMERGENCY_CALL_MAX + 1] =
{
    {"112"},
     0
};

LOCAL const MMIPLMN_LANGUAGE_T language_settings_kenya =
{
    MMISET_LANGUAGE_ENGLISH
};

LOCAL const MMIPLMN_SMS_T sms_settings_kenya =
{
    "+254770000040",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_kenya =
{
    {639, 7, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    &emergency_call_list_kenya,
    PNULL,
    &language_settings_kenya,
    &sms_settings_kenya,
    120*1000,//10s
    NULL,
    PNULL
};

//La Reunion Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_la_reunion =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_la_reunion =
{
    "+262692000606",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_la_reunion=
{
    "Plus de logos",
    "http://mobile.orange.fr/0/accueil/Retour?SA=ORECELLSL",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_la_reunion=
{
    "Plus de sonneries",
    "http://mobile.orange.fr/0/accueil/Retour?SA=ORECELLSL",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_la_reunion=
{
    "Plus de vid¨¦os",
    "http://mobile.orange.fr/0/accueil/Retour?SA=HPOMLUNIVIDEO",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_la_reunion=
{
    "Plus de jeux",
    "http://www.orange.fr/0/accueil/Retour?SA=OMLTELECHJEU",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_la_reunion =
{
    &shortcut_picture_detail_la_reunion,
    &shortcut_sound_detail_la_reunion,
    &shortcut_video_detail_la_reunion,
    &shortcut_game_detail_la_reunion,
    PNULL,
    PNULL,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_la_reunion =
{
    {647, 0, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_LAREUNION,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_la_reunion,
    &sms_settings_la_reunion,
    120*1000,//120s
    NULL,//300KB
    &entertainment_shortcuts_la_reunion,
};

//Madagascar Orange Network Operator
LOCAL const MMIPLMN_EMERGENCY_CALL_T emergency_call_list_madagascar[MMIPLMN_EMERGENCY_CALL_MAX + 1] =
{
    {"017"},
    {"112"},
    {"118"},
    {"117"},
     0
};

LOCAL const MMIPLMN_LANGUAGE_T language_settings_madagascar =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_madagascar =
{
    "+261323232707",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_madagascar =
{
    {646, 2, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_MADAGASCAR,
#else
    NULL,
#endif
    &emergency_call_list_madagascar,
    PNULL,
    &language_settings_madagascar,
    &sms_settings_madagascar,
    180*1000,//180s
    NULL,//300KB
    PNULL,
};

//Mali Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_mali =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_mali =
{
    "+22376000000",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_mali =
{
    {610, 2, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_mali,
    &sms_settings_mali,
    120*1000,//120s
    NULL,//300KB
    PNULL,
};

//Mauritius Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_mauritius =
{
    MMISET_LANGUAGE_ENGLISH
};

LOCAL const MMIPLMN_SMS_T sms_settings_mauritius =
{
    "2302500005",
    MMISMS_VALID_PERIOD_259200,//48hrs->72hrs
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_UCS2
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_mauritius =
{
    {617, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_mauritius,
    &sms_settings_mauritius,
    120*1000,//120s
    1000*1024,//1000 KB
    PNULL,
};

//Moldova Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_moldova =
{
    MMISET_LANGUAGE_ROMANIAN
};

LOCAL const MMIPLMN_SMS_T sms_settings_moldova =
{
    "+37369101030",
    MMISMS_VALID_PERIOD_DEFAULT,//default
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_AUTO
};

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_moldova=
{
    "Imagini",
    "http://wap.orange.md/imagin",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_moldova=
{
    "Melodii",
    "http://wap.orange.md/melodii",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_moldova=
{
    "Video",
    "http://wap.orange.md/video",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_moldova=
{
    "Jocuri",
    "http://wap.orange.md/jocur",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_moldova =
{
    &shortcut_picture_detail_moldova,
    &shortcut_sound_detail_moldova,
    &shortcut_video_detail_moldova,
    &shortcut_game_detail_moldova,
    PNULL,
    PNULL,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_moldova =
{
    {259, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹ý
    R_NETWORK_BOOKMERK_ORANGE_MOLDOVA,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_moldova,
    &sms_settings_moldova,
    120*1000,//120s
    NULL,//300KB
    &entertainment_shortcuts_moldova,
};

#if 0
//Morocco Orange Network Operator
LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_Morocco =
{
    {604, 0, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    NULL,
    NULL
};
#endif

//Niger Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_niger =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_niger =
{
    "+22790000105",
    MMISMS_VALID_PERIOD_259200,//48->72hrs
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_8BIT //8bit
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_niger =
{
    {614, 4, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL, //in sim card
    PNULL,
    &language_settings_niger,
    &sms_settings_niger,
    120*1000,//120s
    100*1024, //300KB
    PNULL,
};

//Poland Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_poland =
{
    MMISET_LANGUAGE_POLISH
};

//no sms settings available

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_poland=
{
    "Dzwonki i Tapety",
    "http://m.orange.pl/ubaw",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_poland=
{
    "TV i wideo",
    "http://m.orange.pl/tv",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_poland=
{
    "Gry",
    "http://m.orange.pl/gry",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_poland =
{
    &shortcut_picture_detail_poland,
    PNULL,
    &shortcut_video_detail_poland,
    &shortcut_game_detail_poland,
    PNULL,
    PNULL,
};
LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_poland =
{
    {260, 3, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_POLAND,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_poland,
    PNULL, //no sms settings available
    120*1000,//180s
    NULL,  //300KB
    &entertainment_shortcuts_poland,
};

//Portugal Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_portugal =
{
    MMISET_LANGUAGE_PORTUGUESE
};

LOCAL const MMIPLMN_SMS_T sms_settings_portugal =
{
    "+35193121314",
    MMISMS_VALID_PERIOD_DEFAULT,
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_DEFAULT//8bit
};

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_ringtone_detail_portugal=
{
    "Mais toques",
    "http://I.optimus.pt/HS_Crings",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_portugal=
{
    "Mais m¨²sicas",
    "http://l.optimus.pt/HS_Musica",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_portugal=
{
    " Mais V¨ªdeos",
    "http://portal.optimus.pt/hierarquia/gotoPage.aspx?ID_Page=10",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_portugal=
{
    "Mais Jogos",
    "http://l.optimus.pt/HS_Jogos",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_portugal =
{
    PNULL,
    &shortcut_sound_detail_portugal,
    &shortcut_video_detail_portugal,
    &shortcut_game_detail_portugal,
    &shortcut_ringtone_detail_portugal,
    PNULL,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_portugal =
{
    {268, 3, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_PORTUGAL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_portugal,
    &sms_settings_portugal,
    120*1000,//120s
    300*1024, //300KB
    &entertainment_shortcuts_portugal,
};

//Romania Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_romania =
{
    MMISET_LANGUAGE_ROMANIAN
};

//no SMS settings available

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_romania=
{
    "Enjoy",
    "http://wap.orange,ro/enjoy",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_romania=
{
    "Sonerii",
    "http://wap.orange.ro/sonerii",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_romania=
{
    "Video",
    "http://wap.orange.ro/video",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_romania=
{
    "Jocuri",
    "http://wap.orange,ro/jocuri",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_romania =
{
    &shortcut_picture_detail_romania,
    &shortcut_sound_detail_romania,
    &shortcut_video_detail_romania,
    &shortcut_game_detail_romania,
    PNULL,
    PNULL,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_romania =
{
    {226, 10, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_ROMANIA,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_romania,
    PNULL, //no sms settings available
    30*1000,//30s
    300*1024, //300KB
    &entertainment_shortcuts_romania,
};

#if 0
//Saint Pierre and Miquelon Orange Network Operator
LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_Saint_Pierre_and_ Miquelon =
{
    {604, 0, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    NULL,
    NULL
};
#endif


//Senegal Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_senegal =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_senegal =
{
    "+221776380010",
    MMISMS_VALID_PERIOD_MAX,
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_DEFAULT//8bit
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_senegal =
{
    {608, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_SENEGAL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_senegal,
    &sms_settings_senegal,
    120*1000,//120s
    NULL, //300KB
    PNULL,
};

//Slovakia Orange Network Operator(Ë¹Âå·¥¿Ë)
LOCAL const MMIPLMN_LANGUAGE_T language_settings_slovakia =
{
    MMISET_LANGUAGE_SLOVAK
};

//no sms settings available

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_slovakia =
{
    {231, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_SLOVAKIA,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_slovakia,
    PNULL, ////no sms settings available
    120*1000,//120s
    300*1024, //300KB
    PNULL,
};

//Spain Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_spain =
{
    MMISET_LANGUAGE_SPANISH
};

//no sms settings available

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_spain=
{
    "Fotograf¨ªa ",
    "http://wap.orange,ro/enjoy",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_spain=
{
    "M¨²sica ",
    "http://wap.orange.es/fotografiaSC",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_spain=
{
    "Tv y Cine",
    "http://wap.orange.es/tvSC",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_spain=
{
    "Videojuegos",
    "http://wap.orange.es/juegosSC",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_app_detail_spain=
{
    "Aplicaciones",
    "http://wap.orange.es/appsSC",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_ringtone_detail_spain=
{
    "MusicStore wap link ",
    "http://wap.orange.es/bajacanciones",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_spain =
{
    &shortcut_picture_detail_spain,
    &shortcut_sound_detail_spain,
    &shortcut_video_detail_spain,
    &shortcut_game_detail_spain,
    &shortcut_ringtone_detail_spain,
    &shortcut_app_detail_spain,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_spain =
{
    {214, 3, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_SPAIN,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_spain,
    PNULL, ////no sms settings available
    120*1000,//120s
    1000*1024, //1000KB
    &entertainment_shortcuts_spain,
};

//Switzerland Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_switzerland =
{
    MMISET_LANGUAGE_GERMAN
};

//no sms settings available

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_switzerland =
{
    "Pictures",
    "http://mobile.orange.ch/logo",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_switzerland =
{
    "Ringtones",
    "http://mobile.orange.ch/ringtones",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_switzerland =
{
    "Video",
    "http://mobile.orange.ch/video",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_switzerland =
{
    "Games",
    "http://mobile.orange.ch/games",
};
/*do not support TV, ignore TV shortcut*/

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_switzerland  =
{
    &shortcut_picture_detail_switzerland ,
    &shortcut_sound_detail_switzerland ,
    &shortcut_video_detail_switzerland ,
    &shortcut_game_detail_switzerland ,
    PNULL,
    PNULL,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_switzerland =
{
    {228, 3, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_SWITZERLAND,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_switzerland,
    PNULL, ////no sms settings available
    120*1000,//120s
    NULL, //300KB
    &entertainment_shortcuts_switzerland,
};


//Tunisia Orange Network Operator
LOCAL const MMIPLMN_KEYBOARD_SHORTCUTS_T keyboard_shortcuts_settings_tunisia =
{
    {
        MSG_KEYLONG_1,
#ifdef BROWSER_SUPPORT
        MSG_KEYLONG_2
#else
        NULL
#endif
    },
    plmn_keyboard_shortcuts_tunisia
};

LOCAL const MMIPLMN_LANGUAGE_T language_settings_tunisia =
{
    MMISET_LANGUAGE_FRENCH
};

LOCAL const MMIPLMN_SMS_T sms_settings_tunisia =
{
    "+21650000003",
    MMISMS_VALID_PERIOD_MAX,//Max
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET
};

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_tunisia =
{
    NULL,//use default
    "http://m.orange.tn",
};


LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_tunisia  =
{
    PNULL,
    PNULL,
    PNULL,
    &shortcut_game_detail_tunisia ,
    PNULL,
    PNULL,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_tunisia =
{
    {605, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_TUNISIA,
#else
    NULL,
#endif
    PNULL,
    &keyboard_shortcuts_settings_tunisia,
    &language_settings_tunisia,
    &sms_settings_tunisia,
    120*1000,//120s
    NULL,
    &entertainment_shortcuts_tunisia,
};


//Uganda Orange Network Operator
LOCAL const MMIPLMN_EMERGENCY_CALL_T emergency_call_list_uganda[MMIPLMN_EMERGENCY_CALL_MAX + 1] =
{
    {"112"},
     0
};

LOCAL const MMIPLMN_LANGUAGE_T language_settings_uganda =
{
    MMISET_LANGUAGE_ENGLISH
};

LOCAL const MMIPLMN_SMS_T sms_settings_uganda =
{
    "+256790000012",
    MMISMS_VALID_PERIOD_DEFAULT,
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_DEFAULT//8bit
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_uganda =
{
    {641, 14, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    &emergency_call_list_uganda,
    PNULL,
    &language_settings_uganda,
    &sms_settings_uganda,
    120*1000,//120s
    NULL //300KB
};

//United Kingdom  Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_united_kingdom  =
{
    MMISET_LANGUAGE_ENGLISH
};

//no sms settings available

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_united_kingdom=
{
    "More pictures",
    "http://orangeworld.co.uk/r/embedpictures/",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_united_kingdom=
{
    "More music",
    "http://orangeworld.co.uk/r/embedmusic/",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_united_kingdom=
{
    "More videos",
    "http://orangeworld.co.uk/r/embedvideo/",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_united_kingdom=
{
    "More games",
    "http://orangeworld.co.uk/r/embedgames/",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_app_detail_united_kingdom=
{
    "More applications",
    "http://orangeworld.co.uk/r/embedsoftware/",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_ringtone_detail_united_kingdom=
{
    "More ringtones",
    "http://orangeworld.co.uk/r/embedringtones/",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_united_kingdom =
{
    &shortcut_picture_detail_united_kingdom,
    &shortcut_sound_detail_united_kingdom,
    &shortcut_video_detail_united_kingdom,
    &shortcut_game_detail_united_kingdom,
    &shortcut_ringtone_detail_united_kingdom,
    &shortcut_app_detail_united_kingdom,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_united_kingdom =
{
    {234, 33, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_ORANGE_UK,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_united_kingdom,
    PNULL,  //no sms settings available
    NULL,//default 50s
    300*1024, //300KB
    &entertainment_shortcuts_united_kingdom,
};

//United Kingdom  T-Mobile  Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_united_kingdom_tm  =
{
    MMISET_LANGUAGE_ENGLISH
};


LOCAL const MMIPLMN_SMS_T sms_settings_united_kingdom_tm =
{
    "+447958879879",
    MMISMS_VALID_PERIOD_MAX,
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_DEFAULT//8bit
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_united_kingdom_tm =
{
    {234, 30, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#if 0 //def BROWSER_SUPPORT ÐÞ¸Ä131801µÄ±àÒë²»¹
    R_NETWORK_BOOKMERK_T_MOBILE_UK,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_united_kingdom_tm,
    &sms_settings_united_kingdom_tm, 
    NULL,//default 50s
    300*1024, //300KB
    PNULL,
};

#if 1
//Vanuatu Orange Network Operator
LOCAL const MMIPLMN_LANGUAGE_T language_settings_vanuatu  =
{
    MMISET_LANGUAGE_ENGLISH
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_vanuatu =
{
    {541, 1, 2},
#ifdef MMI_GPRS_SUPPORT
    R_NETWORK_DATA_ORANGE,
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    NULL,
#else
    NULL,
#endif
    PNULL,
    PNULL,
    &language_settings_vanuatu,
    PNULL,
    NULL,
    NULL,
    PNULL,
};
#endif
#endif

#ifdef WIN32
//list all data for debug
LOCAL const MMIPLMN_EMERGENCY_CALL_T emergency_call_list_debug[MMIPLMN_EMERGENCY_CALL_MAX + 1] =
{
    {"101"},
    {"102"},
    {"103"},
    {"104"},
    {"112"},
    {"188"},
     0
};

//Tunisia Orange Network Operator
LOCAL const MMIPLMN_KEYBOARD_SHORTCUTS_T keyboard_shortcuts_settings_debug =
{
    {
        MSG_KEYLONG_1,
#ifdef BROWSER_SUPPORT
        MSG_KEYLONG_2
#else
        NULL
#endif
    },
    plmn_keyboard_shortcuts_debug
};
LOCAL const MMIPLMN_LANGUAGE_T language_settings_debug =
{
    MMISET_LANGUAGE_ENGLISH     //debug use english
};

LOCAL const MMIPLMN_SMS_T sms_settings_debug  =
{
    "+33689004000",
    MMISMS_VALID_PERIOD_MAX,//max
    MMISMS_PREFERRED_PATH_GSM_PREFERRED,
    MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET
};

LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_picture_detail_debug=
{
    "Plus d'images",
    "http://logos.mob2.orange.fr",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_ringtone_detail_debug=
{
    "Plus de sonneries ",
    "http://sons.mob2.orange.fr",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_sound_detail_debug=
{
    "Plus de musique",
    "http://musicstore.mob2.orange.fr",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_video_detail_debug=
{
    "Plus de vid¨¦os ",
    "http://univideo.mob2.orange.fr ",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_game_detail_debug=
{
    "Plus de jeux",
    "http://jeux2.mob2.orange.fr",
};
LOCAL MMIPLMN_SHORTCUT_DETAIL_T shortcut_app_detail_debug=
{
    "More applications",
    "http://orangeworld.co.uk/r/embedsoftware/",
};

LOCAL const MMIPLMN_SHORTCUTS_T entertainment_shortcuts_debug =
{
    &shortcut_picture_detail_debug,
    &shortcut_sound_detail_debug,//PNULL
    &shortcut_video_detail_debug,
    &shortcut_game_detail_debug,
    &shortcut_ringtone_detail_debug,
    &shortcut_app_detail_debug,
};

LOCAL const MMIPLMN_CONFIG_INFO_T plmn_config_debug =
{
    {001, 01, 2},
#ifdef MMI_GPRS_SUPPORT
#ifdef ORANGE_SUPPORT
		R_NETWORK_DATA_ORANGE,
#else
		R_NETWORK_ACCOUNT,
#endif
#else
    NULL,
#endif
#ifdef BROWSER_SUPPORT
    R_NETWORK_BOOKMARK,
#else
    NULL,
#endif
    &emergency_call_list_debug,
    &keyboard_shortcuts_settings_debug,
    &language_settings_debug,
    &sms_settings_debug,
    120*1000,//120s
    600*1024,//600KB
    &entertainment_shortcuts_debug,
};
#endif

LOCAL const MMIPLMN_CONFIG_INFO_T* const s_plmn_config_table[] =
{
#ifdef ORANGE_SUPPORT
    &plmn_config_armenia,
    &plmn_config_australia,
    &plmn_config_belgium,
    &plmn_config_bisaau_guinea,
    &plmn_config_botswana,
    &plmn_config_cameroon,
    &plmn_config_caribbean,
    //&plmn_config_central_african_republic,
    &plmn_config_conakry_guinea,
    &plmn_config_cote_divoire,
    &plmn_config_dominican,
    &plmn_config_egypt,
    &plmn_config_equatorial_guinea,
    &plmn_config_france,
    &plmn_config_jordan,
    &plmn_config_kenya,
    &plmn_config_la_reunion,
    &plmn_config_madagascar,
    &plmn_config_mali,
    &plmn_config_mauritius,
    &plmn_config_moldova,
    &plmn_config_niger,
    &plmn_config_poland,
    &plmn_config_portugal,
    &plmn_config_romania,
    &plmn_config_senegal,
    &plmn_config_slovakia,
    &plmn_config_spain,
    &plmn_config_switzerland,
    &plmn_config_tunisia,
    &plmn_config_uganda,
    &plmn_config_united_kingdom,
    &plmn_config_united_kingdom_tm,
    &plmn_config_vanuatu,
#endif
#ifdef WIN32
    &plmn_config_debug,//for debug
#endif
    PNULL,
};
#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#ifdef WIN32
/*****************************************************************************/
//  Description : keyboard shortcuts of simulator debug
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void plmn_keyboard_shortcuts_debug(MMI_MESSAGE_ID_E msg_id)
{
    switch (msg_id)
    {
    case MSG_KEYLONG_1:
        MMIAPISMS_OpenCallVMWin(MMI_DUAL_SYS_MAX);
        break;

    case MSG_KEYLONG_2:
#ifdef BROWSER_SUPPORT
        {
            MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
            entry_param.dual_sys = MN_DUAL_SYS_MAX;
            entry_param.url_ptr = "http://m.orange.tn";
            entry_param.user_agent_ptr = PNULL;

            MMIBROWSER_Entry(&entry_param);
        }
#endif
        break;

    default:
        break;
    }
}
#endif

#ifdef ORANGE_SUPPORT
/*****************************************************************************/
//  Description : keyboard shortcuts of Armenia
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL void plmn_keyboard_shortcuts_tunisia(MMI_MESSAGE_ID_E msg_id)
{
    switch (msg_id)
    {
    case MSG_KEYLONG_1:
        MMIAPISMS_OpenCallVMWin(MMI_DUAL_SYS_MAX);
        break;

    case MSG_KEYLONG_2:
#ifdef BROWSER_SUPPORT
        {
            MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
            entry_param.dual_sys = MN_DUAL_SYS_MAX;
            entry_param.url_ptr = "http://m.orange.tn";
            entry_param.user_agent_ptr = PNULL;

            MMIBROWSER_Entry(&entry_param);
        }
#endif
        break;

    default:
        break;
    }
}
#endif
#endif

/*****************************************************************************/
//  Description : Is Same Plmn
//  Global resource dependence : none
//  Author: wancan.you
//  Note:cmp mcc and network name
/*****************************************************************************/
PUBLIC BOOLEAN MMIPLMN_IsSamePlmn(const MN_PLMN_T *plmn1, const MN_PLMN_T *plmn2)
{
    const MMI_NETWORK_NAME_T *plmn1_network_info_ptr = PNULL;
    const MMI_NETWORK_NAME_T *plmn2_network_info_ptr = PNULL;
    PHONE_PLMN_NAME_E plmn1_name = PLMN_INVALID;
    PHONE_PLMN_NAME_E plmn2_name = PLMN_INVALID;
    BOOLEAN is_plmn1_india_mcc = FALSE;
    BOOLEAN is_plmn2_india_mcc = FALSE;

    plmn1_name = MMIAPIPHONE_GetNetWorkId((MN_PLMN_T *)plmn1);
    plmn2_name = MMIAPIPHONE_GetNetWorkId((MN_PLMN_T *)plmn2);

    plmn1_network_info_ptr = MMIAPIPHONE_GetNetWorkInfo(plmn1_name);
    plmn2_network_info_ptr = MMIAPIPHONE_GetNetWorkInfo(plmn2_name);

    if (PNULL != plmn1_network_info_ptr && PNULL != plmn2_network_info_ptr)
    {
        if (404 == plmn1_network_info_ptr->mcc
            || 405 == plmn1_network_info_ptr->mcc)
        {
            is_plmn1_india_mcc = TRUE;
        }

        if (404 == plmn2_network_info_ptr->mcc
            || 405 == plmn2_network_info_ptr->mcc)
        {
            is_plmn2_india_mcc = TRUE;
        }

        if (((plmn1_network_info_ptr->mcc == plmn2_network_info_ptr->mcc) || (is_plmn1_india_mcc && is_plmn2_india_mcc))
            && plmn1_network_info_ptr->name_index == plmn2_network_info_ptr->name_index)
        {
            return TRUE;
        }
    }

    return FALSE;
}

#ifdef BROWSER_SUPPORT
#if 0
/*****************************************************************************/
//  Description : Set All Browsers Default Bookmark
//  Global resource dependence : 
//  Author:dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void SetAllBrowsersDefaultBookmark(void)
{
    MMIBROWSER_BROWSER_TYPE_E browser_type =  MMIBROWSER_TYPE_MAX;
    
    for(browser_type = 0;  browser_type < MMIBROWSER_TYPE_MAX; browser_type++)
    {
        MMIAPIBROWSER_SetDefaultBookmark(browser_type);
    }
}
#endif
#endif

/*****************************************************************************/
//  Description : Load Local Default Config
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPLMN_LoadLocalDefaultConfig(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};
        MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
        MN_PLMN_T hplmn_store[MMI_DUAL_SYS_MAX] = {0};
        
        MMINV_READ(MMINV_HPLMN_INFO, hplmn_store, return_value);


        plmn = MNSIM_GetHplmnEx(dual_sys);

        if (MN_RETURN_SUCCESS != return_value)
        {
            SCI_MEMSET(hplmn_store, 0x00, sizeof(hplmn_store));

            MMINV_WRITE(MMINV_HPLMN_INFO, hplmn_store);
        }

        if (0 == plmn.mcc
            || (hplmn_store[dual_sys].mnc_digit_num == plmn.mnc_digit_num
            && hplmn_store[dual_sys].mcc == plmn.mcc
            && hplmn_store[dual_sys].mnc == plmn.mnc))
        {
            SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_LoadLocalDefaultConfig neednot load default!");

            return FALSE;
        }

        hplmn_store[dual_sys] = plmn;

        MMINV_WRITE(MMINV_HPLMN_INFO, hplmn_store);

        for (i = 0; ; i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    MMISET_LANGUAGE_TYPE_E language_type = MMISET_EDEFAULT_LANGUAGE;//MMISET_LANGUAGE_SIMP_CHINESE; //default type of language

                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_LoadLocalDefaultConfig");

                    //reset bookmark
#ifdef BROWSER_SUPPORT
#if 0
                    SetAllBrowsersDefaultBookmark();
#endif
#endif
                    //language settings
                    MMIAPISET_SetLanguageType(language_type);

                    //sms settings
                    MMIAPISMS_SetLocalSettings(dual_sys);

                    return TRUE;
                }
            }
            else
            {
                break;
            }
        }
    }

    return FALSE;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : Get Local Bookmark
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_BOOKMARK_ID_T MMIPLMN_GetLocalBookmark(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};

        plmn = MNSIM_GetHplmnEx(dual_sys);

        for (i = 0; ;i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_GetLocalBookmark find");

                    return s_plmn_config_table[i]->bookmark;
                }
            }
            else
            {
                break;
            }
        }
    }

    return NULL;
#else
    return NULL;
#endif
}

/*****************************************************************************/
//  Description : Get Local Network Account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_NETWORK_ACC_ID_T MMIPLMN_GetLocalNetworkAccount(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};

        plmn = MNSIM_GetHplmnEx(dual_sys);

        for (i = 0; ;i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_GetLocalNetworkAccount find");

                    return s_plmn_config_table[i]->network_account;
                }
            }
            else
            {
                break;
            }
        }
    }

    return NULL;
#else
    return NULL;
#endif
}

/*****************************************************************************/
//  Description : Get Local Emergency Call Number
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system; MAX NUM:MMIPLMN_EMERGENCY_CALL_MAX
/*****************************************************************************/
PUBLIC const MMIPLMN_EMERGENCY_CALL_T *MMIPLMN_GetLocalEmergency(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};

        plmn = MNSIM_GetHplmnEx(dual_sys);

        for (i = 0; ;i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_GetLocalEmergency find");

                    return s_plmn_config_table[i]->emergency_call_list;
                }
            }
            else
            {
                break;
            }
        }
    }

    return PNULL;
#else
    return PNULL;
#endif
}


/*****************************************************************************/
//  Description : Get Local Keyboard Shortcuts
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_KEYBOARD_SHORTCUTS_T *MMIPLMN_GetLocalKeyboardShortcuts(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};

        plmn = MNSIM_GetHplmnEx(dual_sys);

        for (i = 0; ;i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_GetLocalKeyboardShortcuts find");

                    return s_plmn_config_table[i]->keyboard_shortcuts_list;
                }
            }
            else
            {
                break;
            }
        }
    }

    return PNULL;
#else
    return PNULL;
#endif
}

/*****************************************************************************/
//  Description : Get Local Language settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_LANGUAGE_T *MMIPLMN_GetLocalLanguageSettings(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};

        plmn = MNSIM_GetHplmnEx(dual_sys);

        for (i = 0; ;i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_GetLocalLanguageSettings find");

                    return s_plmn_config_table[i]->language_settings;
                }
            }
            else
            {
                break;
            }
        }
    }

    return PNULL;
#else
    return PNULL;
#endif
}

/*****************************************************************************/
//  Description : Get Local SMS settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_SMS_T *MMIPLMN_GetLocalSMSSettings(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};

        plmn = MNSIM_GetHplmnEx(dual_sys);

        for (i = 0; ;i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_GetLocalSMSSettings find");

                    return s_plmn_config_table[i]->sms_settings;
                }
            }
            else
            {
                break;
            }
        }
    }

    return PNULL;
#else
    return PNULL;
#endif
}

/*****************************************************************************/
//  Description : Get Local PDP Time out
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;(uint:ms)
/*****************************************************************************/
PUBLIC const MMIPLMN_PDP_TIMEOUT_T MMIPLMN_GetLocalPDPTimeout(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};

        plmn = MNSIM_GetHplmnEx(dual_sys);

        for (i = 0; ;i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_GetLocalPDPTimeout find");

                    return s_plmn_config_table[i]->pdp_timeout;
                }
            }
            else
            {
                break;
            }
        }
    }

    return NULL;
#else
    return NULL;
#endif
}

/*****************************************************************************/
//  Description : Get Local MMS Limit
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;(uint:Bytes)
/*****************************************************************************/
PUBLIC const MMIPLMN_MMS_LIMIT_T MMIPLMN_GetLocalMMSLimit(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};

        plmn = MNSIM_GetHplmnEx(dual_sys);

        for (i = 0; ;i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_GetLocalMMSLimit find");

                    return s_plmn_config_table[i]->mms_limit;
                }
            }
            else
            {
                break;
            }
        }
    }

    return NULL;
#else
    return NULL;
#endif
}

/*****************************************************************************/
//  Description : Get Local entertainment shortcut menus
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:NULL:Use default by system;(uint:Bytes)
/*****************************************************************************/
PUBLIC const MMIPLMN_SHORTCUTS_T *MMIPLMN_GetLocalShortcuts(MN_DUAL_SYS_E dual_sys)
{
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        uint32 i = 0;
        MN_PLMN_T plmn = {0};

        plmn = MNSIM_GetHplmnEx(dual_sys);

        for (i = 0; ;i++)
        {
            if (PNULL != s_plmn_config_table[i])
            {
                if (MMIPLMN_IsSamePlmn(&plmn, &s_plmn_config_table[i]->plmn))
                {
                    SCI_TRACE_LOW("[mmiplmn_info] MMIPLMN_GetLocalMMSLimit find");

                    return s_plmn_config_table[i]->entertainment_shortcuts;
                }
            }
            else
            {
                break;
            }
        }
    }

    return PNULL;
#else
    return PNULL;
#endif
}

