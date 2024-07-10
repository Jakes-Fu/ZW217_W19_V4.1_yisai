/****************************************************************************
** File Name:      mmiwclk_main.c                            
** Author:                                                                   
** Date:            2004/08/24
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to process msg to main window of worlk clock
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 08/24/2004     taul.zhu          Create
** 
****************************************************************************/
#define _MMIWCLK_MAIN_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "mmi_app_accessory_trc.h"
#include "sci_types.h"
#include "mmiwclk_export.h"

#ifdef WORLD_CLOCK_SUPPORT
#include "window_parse.h"
#include "mmk_app.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmialarm_export.h"
#include "tb_dal.h"
#include "mmi_nv.h"
//#include "mmiacc_winmain.h"
//#include "mmigame.h"
//#include "mmigame_export.h"
#include "mmi_textfun.h"
#include "mmipub.h"
#include "mmiacc_id.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "mmiacc_menutable.h"
#include "mmiacc_nv.h"
#include "mmialarm_image.h"
#include "mmialarm_text.h"
#include "mmi_theme.h"
//#include "guibutton.h"
#include "guidropdownlist.h"
#include "guilabel.h"
#include "mmi_mainmenu_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guiform.h"
#include "mmiidle_export.h"
#include "guires.h"
#include "mmicom_time.h"
#if defined(MMI_UPDATE_TIME_SUPPORT)
#include "mmiset_id.h"
#include "mmiset_text.h"
#endif
#ifdef MMI_PDA_SUPPORT
#include "guisetlist.h"
#endif
//#include "guictrl_api.h"  /*lint !e766*/
#include "mmiidle_statusbar.h"
#include "graphics_draw.h" //for analog clock draw
#include "guitab.h"
#include "mmipb_export.h"
#include "guictrl_api.h"
#include "mmi_appmsg.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

//#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#if defined (MAINLCD_SIZE_128X160) 
#define WCLK_INFO_CITY_WIDTH_PERCENT    65
#define WCLK_INFO_LOCAL_WIDTH_PERCENT   35
#define WCLK_INFO_TIME_WIDTH_PERCENT    85
#define WCLK_INFO_DATE_WIDTH_PERCENT    100
#define WCLK_INFO_SUMMER_WIDTH_PERCENT  15
#define WCLK_INFO_FORM_SPACE            0
#define	WCLK_BIG_FONT            SONG_FONT_11
#define WCLK_SMALL_FONT          SONG_FONT_11
#define	WCLK_WORK_COLOR          MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT)
#define WCLK_GRAY_COLOR          WCLK_WORK_COLOR//MMI_DARK_GRAY_COLOR

#elif defined (MAINLCD_DEV_SIZE_176X220)
#define WCLK_INFO_CITY_WIDTH_PERCENT    75
#define WCLK_INFO_LOCAL_WIDTH_PERCENT   25
#define WCLK_INFO_TIME_WIDTH_PERCENT    35
#define WCLK_INFO_DATE_WIDTH_PERCENT    45
#define WCLK_INFO_SUMMER_WIDTH_PERCENT  20
#define WCLK_INFO_FORM_SPACE            1
#define	WCLK_BIG_FONT            SONG_FONT_12
#define WCLK_SMALL_FONT          SONG_FONT_12
#define	WCLK_WORK_COLOR          MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT)
#define WCLK_GRAY_COLOR          WCLK_WORK_COLOR//MMI_DARK_GRAY_COLOR
#elif defined (MAINLCD_DEV_SIZE_220X176)
#define WCLK_INFO_CITY_WIDTH_PERCENT    80
#define WCLK_INFO_LOCAL_WIDTH_PERCENT   20
#define WCLK_INFO_TIME_WIDTH_PERCENT    28
#define WCLK_INFO_DATE_WIDTH_PERCENT    54
#define WCLK_INFO_SUMMER_WIDTH_PERCENT  18
#define WCLK_INFO_FORM_SPACE            1
#define	WCLK_BIG_FONT            SONG_FONT_12
#define WCLK_SMALL_FONT          SONG_FONT_12
#define	WCLK_WORK_COLOR          MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT)
#define WCLK_GRAY_COLOR          WCLK_WORK_COLOR//MMI_DARK_GRAY_COLOR

#else
#define WCLK_INFO_CITY_WIDTH_PERCENT    80
#define WCLK_INFO_LOCAL_WIDTH_PERCENT   20
#define WCLK_INFO_TIME_WIDTH_PERCENT    27
#define WCLK_INFO_DATE_WIDTH_PERCENT    53
#define WCLK_INFO_SUMMER_WIDTH_PERCENT  20
#define WCLK_INFO_FORM_SPACE            1
#define	WCLK_BIG_FONT               SONG_FONT_26
#define WCLK_SMALL_FONT             SONG_FONT_14
#define	WCLK_WORK_COLOR             MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT)
#define WCLK_GRAY_COLOR             MMI_DARK_GRAY_COLOR

#endif



#ifndef BLEND_MASK
#define BLEND_MASK 0x7E0F81F
#endif

#ifndef ARGB_GET_A
#define ARGB_GET_A(_argb) (((_argb) >> 24) & 0xff)
#endif
#ifndef ARGB_GET_R
#define ARGB_GET_R(_argb) (((_argb) >> 16) & 0xff)
#endif
#ifndef ARGB_GET_G
#define ARGB_GET_G(_argb) (((_argb) >> 8) & 0xff)
#endif
#ifndef ARGB_GET_B
#define ARGB_GET_B(_argb) ((_argb) & 0xff)
#endif

#ifndef RGB888_TO_RGB565
#define RGB888_TO_RGB565(r, g, b)  (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | ((b >> 3) & 0x1f))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     (sizeof(a)/sizeof (a)[0])
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                           */
/*---------------------------------------------------------------------------*/
typedef struct _WT_TIME_DST_VALUE
{
	uint16	mcc;    //mobile country code
	uint16	current_year;//year index
	int16 	standard_timezone;  //the difference,between Coordinated Universal Time (UTC) and local time; in minutes, 
	int16         timezone_with_dst; 
	MMI_TM_T    daylight_start_datetime;//the day summer time start
	MMI_TM_T    daylight_end_datetime;  //the day winter time start
	//BOOLEAN      is_southern_hemisphere;  //是否处于南半球
}WT_TIME_DST_VALUE_T;

typedef struct 
{
    void   *data_ptr;
    uint32  data_type;  //0: RGB565, 1: ARGB888
    uint16  width;
    uint16  height;
    int16   start_x;
    int16   start_y;
}MMI_WORLD_CLOCK_ALPHA_IMG_T; /*lint !e770*/


//all city enum
typedef enum
{
      CITY_ORDER_BY_GMT,
      CITY_ORDER_BY_ALPHABET,
      CITY_ORDER_MAX
}WT_CITY_ORDER_E;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/     
LOCAL   WT_CITY_ORDER_E  s_city_list_order = CITY_ORDER_BY_GMT;

// 本地城市信息
LOCAL   MCC_WT_CITY_INFO_T          s_local_city_info;
// 异地城市信息
LOCAL   MCC_WT_CITY_INFO_T          s_other_city_info;
//gmt dropdownlist type
LOCAL   MMIACC_RTC_DATE_T           s_date;
LOCAL   MMIACC_RTC_TIME_T           s_time;
LOCAL   WT_CITY_RECORD_INFO         s_default_city_info[WT_DEFAULT_CITY_NUM] = {0}; /*lint !e785*/

LOCAL   const uint16                s_RTC_month_info_leap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//因为有小数点，所以转为分钟计算
LOCAL   const int16		           s_time_zone_arr[WT_TIME_ZONE_NUM_TOTAL]  = { 0,60,120,180,210,240,270,300,330,345,360,390,420,480,540,600,630,660,690,720,765,
                                                 -720,-660,-600,-570,-540,-480,-420,-360,-300,-240,-210,-180,-120,-60};


LOCAL const WT_TIME_CITY_ZONE      s_zone_city_info[] =
{
    {TXT_ACC_WORLD_GMT_0_LISBON,TXT_ACC_WORLD_GMT_0,268, MMI_ACC_0_LISBON},//Portugal 41
    {TXT_ACC_WORLD_GMT_0_CASABLANCA,TXT_ACC_WORLD_GMT_0,604, MMI_ACC_0_CASABLANCA},//Morocco - MA 37
    {TXT_ACC_WORLD_GMT_0_RABAT,TXT_ACC_WORLD_GMT_0,604, MMI_ACC_0_RABAT},//Morocco - MA 40
    {TXT_ACC_WORLD_GMT_0_LONDON,TXT_ACC_WORLD_GMT_0,234, MMI_ACC_0_LONDON},//235 United Kingdom of Great Britain and Northern Ireland 42

    {TXT_ACC_WORLD_GMT_1_P_MADRID,TXT_ACC_WORLD_GMT_1_P,214, MMI_ACC_1_P_MADRID},//Spain 46
    {TXT_ACC_WORLD_GMT_1_P_PARIS,TXT_ACC_WORLD_GMT_1_P,208, MMI_ACC_1_P_PARIS},//France 5
    {TXT_ACC_WORLD_GMT_1_P_ZURICH,TXT_ACC_WORLD_GMT_1_P,228, MMI_ACC_1_P_ZURICH},////Switzerland (Confederation of) 61
    {TXT_ACC_WORLD_GMT_1_P_GENEVA,TXT_ACC_WORLD_GMT_1_P,228, MMI_ACC_1_P_GENEVA},//Switzerland (Confederation of) 57
    {TXT_ACC_WORLD_GMT_1_P_COPENHAGEN,TXT_ACC_WORLD_GMT_1_P,238,MMI_ACC_1_P_COPENHAGEN},//Denmark (Kingdom of Denmark) - DK 23
    {TXT_ACC_WORLD_GMT_1_P_STOCKHOLM,TXT_ACC_WORLD_GMT_1_P,240, MMI_ACC_1_P_STOCKHOLM},//Sweden 60
    {TXT_ACC_WORLD_GMT_1_P_AMSTERDAM,TXT_ACC_WORLD_GMT_1_P,204, MMI_ACC_1_P_AMSTERDAM},// 1Netherlands
    {TXT_ACC_WORLD_GMT_1_P_BERLIN,TXT_ACC_WORLD_GMT_1_P,262, MMI_ACC_1_P_BERLIN},  //Germany 9    {TXT_ACC_WORLD_GMT_1_P_ROME,TXT_ACC_WORLD_GMT_1_P,222, MMI_ACC_1_P_ROME},//Italy 43
    {TXT_ACC_WORLD_GMT_1_P_LUXEMBOURG,TXT_ACC_WORLD_GMT_1_P,270, MMI_ACC_1_P_LUXEMBOURG},//Luxembourg 45
    {TXT_ACC_WORLD_GMT_1_P_PRAGUE,TXT_ACC_WORLD_GMT_1_P,230, MMI_ACC_1_P_PRAGUE},//Czech Republic - CZ 13
    {TXT_ACC_WORLD_GMT_1_P_WARSAW,TXT_ACC_WORLD_GMT_1_P,260,MMI_ACC_1_P_WARSAW},//Poland - PL 27
    {TXT_ACC_WORLD_GMT_1_P_VIENNA,TXT_ACC_WORLD_GMT_1_P,232, MMI_ACC_1_P_VIENNA},//Austria 68
    {TXT_ACC_WORLD_GMT_1_P_BUDAPEST,TXT_ACC_WORLD_GMT_1_P,216, MMI_ACC_1_P_BUDAPEST},//Hungary - HU 11
    {TXT_ACC_WORLD_GMT_1_P_BRUSSELS,TXT_ACC_WORLD_GMT_1_P,206,MMI_ACC_1_P_BRUSSELS},//Belgium - BE 15  

    {TXT_ACC_WORLD_GMT_2_P_CAPE_TOWN,TXT_ACC_WORLD_GMT_2_P,655, MMI_ACC_2_P_CAPE_TOWN},//South Africa - ZA 35
    {TXT_ACC_WORLD_GMT_2_P_JOHANNESBURG,TXT_ACC_WORLD_GMT_2_P,655, MMI_ACC_2_P_JOHANNESBURG},//South Africa (Republic of) 79
    {TXT_ACC_WORLD_GMT_2_P_SOFIA,TXT_ACC_WORLD_GMT_2_P,284, MMI_ACC_2_P_SOFIA},//Bulgaria (Republic of) 62
    {TXT_ACC_WORLD_GMT_2_P_BUCHAREST,TXT_ACC_WORLD_GMT_2_P,226, MMI_ACC_2_P_BUCHAREST},//Romania - RO 12 
    {TXT_ACC_WORLD_GMT_2_P_ISTANBUL,TXT_ACC_WORLD_GMT_2_P,286, MMI_ACC_2_P_ISTANBUL},//Turkey 78
    {TXT_ACC_WORLD_GMT_2_P_KIEV,TXT_ACC_WORLD_GMT_2_P,255, MMI_ACC_2_P_KIEV},//Ukraine - UA 30
    {TXT_ACC_WORLD_GMT_2_P_CAIRO,TXT_ACC_WORLD_GMT_2_P,602, MMI_ACC_2_P_CAIRO},//Egypt - EG 34
    {TXT_ACC_WORLD_GMT_2_P_ATHENS,TXT_ACC_WORLD_GMT_2_P,202, MMI_ACC_2_P_ATHENS},//Greece 74

    {TXT_ACC_WORLD_GMT_3P5_P_TEHRAN,TXT_ACC_WORLD_GMT_3P5_P,432,MMI_ACC_3P5_P_TEHRAN},//Iran - IR 19 

    {TXT_ACC_WORLD_GMT_3_P_MOSCOW,TXT_ACC_WORLD_GMT_4_P,250, MMI_ACC_3_P_MOSCOW},//Russian Federation 52
    {TXT_ACC_WORLD_GMT_4_P_ABUDHABI,TXT_ACC_WORLD_GMT_4_P,424, MMI_ACC_4_P_ABUDHABI},//0 United Arab Emirates

    {TXT_ACC_WORLD_GMT_4P5_P_KABUL,TXT_ACC_WORLD_GMT_4P5_P,412, MMI_ACC_4P5_P_KABUL},//Afghanistan - AF 33

    {TXT_ACC_WORLD_GMT_5_P_ISLAMABAD,TXT_ACC_WORLD_GMT_5_P,410, MMI_ACC_5_P_ISLAMABAD},//Pakistan (Islamic Republic of) 77

    {TXT_ACC_WORLD_GMT_5P5_P_NEWDELHI,TXT_ACC_WORLD_GMT_5P5_P,404, MMI_ACC_5P5_P_NEWDELHI},//India (Republic of) 71
    {TXT_ACC_WORLD_GMT_5P5_P_MUMBAI,TXT_ACC_WORLD_GMT_5P5_P,404, MMI_ACC_5P5_P_MUMBAI},////404~406 India 49
    {TXT_ACC_WORLD_GMT_5P5_P_BANGALORE,TXT_ACC_WORLD_GMT_5P5_P,404, MMI_ACC_5P5_P_BANGALORE},//404~406 India 7
    {TXT_ACC_WORLD_GMT_5P5_P_KOLKATA,TXT_ACC_WORLD_GMT_5P5_P,404, MMI_ACC_5P5_P_KOLKATA},//India - IN 28

    {TXT_ACC_WORLD_GMT_5P75_P_KATHMANDU,TXT_ACC_WORLD_GMT_5P75_P,429, MMI_ACC_5P75_P_KATHMANDU},//Nepal - NP 29

    {TXT_ACC_WORLD_GMT_6_P_DHAKA,TXT_ACC_WORLD_GMT_6_P,470,MMI_ACC_6_P_DHAKA},//Bangladesh - BD 17

    {TXT_ACC_WORLD_GMT_6P5_P_YANGON,TXT_ACC_WORLD_GMT_6P5_P,414, MMI_ACC_6P5_P_YANGON},//Myanmar (Union of) 76

    {TXT_ACC_WORLD_GMT_7_P_BANGKOK,TXT_ACC_WORLD_GMT_7_P,520, MMI_ACC_7_P_BANGKOK},//Thailand 47
    {TXT_ACC_WORLD_GMT_7_P_HANOI,TXT_ACC_WORLD_GMT_7_P,452,MMI_ACC_7_P_HANOI},//Vietnam - VN 26
    {TXT_ACC_WORLD_GMT_7_P_JAKARTA,TXT_ACC_WORLD_GMT_7_P,510, MMI_ACC_7_P_JAKARTA},//Indonesia (Republic of) 75

    {TXT_ACC_WORLD_GMT_8_P_BEIJING,TXT_ACC_WORLD_GMT_8_P,460, MMI_ACC_8_P_BEIJING}, //china 8
    {TXT_ACC_WORLD_GMT_8_P_KUALA_LUMPUR,TXT_ACC_WORLD_GMT_8_P,502, MMI_ACC_8_P_KUALA_LUMPUR},//Malaysia - MY 31 
    {TXT_SINGAPORE,TXT_ACC_WORLD_GMT_8_P,525, MMI_ACC_8_P_SINGAPORE},//Singapore (Republic of) 73
    {TXT_ACC_WORLD_GMT_8_P_HONGKONG,TXT_ACC_WORLD_GMT_8_P,454, MMI_ACC_8_P_HONGKONG},//Hong Kong, China 70
    {TXT_ACC_WORLD_GMT_8_P_TAIPEI,TXT_ACC_WORLD_GMT_8_P,466, MMI_ACC_8_P_TAIPEI},//Taiwan, China 64
    {TXT_ACC_WORLD_GMT_8_P_MANILA,TXT_ACC_WORLD_GMT_8_P,515, MMI_ACC_8_P_MANILA},//Philippines (Republic of the) 48

    {TXT_ACC_WORLD_GMT_9_P_TOKYO,TXT_ACC_WORLD_GMT_9_P,440,MMI_ACC_9_P_TOKYO},//Japan - JP 20

    {TXT_ACC_WORLD_GMT_9P5_P_MELBOURNE,TXT_ACC_WORLD_GMT_10_P,505, MMI_ACC_9P5_P_MELBOURNE},//Australia - AU 51

    {TXT_ACC_WORLD_GMT_10_P_VLADIVOSTOK,TXT_ACC_WORLD_GMT_10_P,250,MMI_ACC_10_P_VLADIVOSTOK},//Russian Federation 25
    {TXT_ACC_WORLD_GMT_10_P_GUAM,TXT_ACC_WORLD_GMT_10_P,310,MMI_ACC_10_P_GUAM},//Guam (United States of America) - GU 24
    {TXT_ACC_WORLD_GMT_10_P_CANBERRA,TXT_ACC_WORLD_GMT_10_P,505, MMI_ACC_10_P_CANBERRA},//Australia - AU 36
    {TXT_ACC_WORLD_GMT_10_P_SYDNEY,TXT_ACC_WORLD_GMT_10_P,505, MMI_ACC_10_P_SYDNEY},////Australia - AU 72

    {TXT_ACC_WORLD_GMT_10P5_P_BRISBANE,TXT_ACC_WORLD_GMT_10P5_P,505, MMI_ACC_10P5_P_BRISBANE},//Australia - AU 14

    {TXT_ACC_WORLD_GMT_11_P_SOLOMON,TXT_ACC_WORLD_GMT_11_P,540, MMI_ACC_11_P_SOLOMON},//Solomon Islands 63

    {TXT_ACC_WORLD_GMT_11P5_P_NORFOLD_ISLAND,TXT_ACC_WORLD_GMT_11P5_P,505, MMI_ACC_11P5_P_NORFOLD_ISLAND},//Norfolk island 55

    {TXT_ACC_WORLD_GMT_12_P_AUCKLAND,TXT_ACC_WORLD_GMT_12_P,530, MMI_ACC_12_P_AUCKLAND},// 3New Zealand
    {TXT_ACC_WORLD_GMT_12_P_WELLINGTON,TXT_ACC_WORLD_GMT_12_P,530, MMI_ACC_12_P_WELLINGTON},//New Zealand 67

    {TXT_ACC_WORLD_GMT_12P75_P_TONGA,TXT_ACC_WORLD_GMT_12P75_P,539, MMI_ACC_12P75_P_TONGA},//Tonga (Kingdom of) 65

    {TXT_ACC_WORLD_GMT_12_N_KWAJALEIN,TXT_ACC_WORLD_GMT_12_N,551, MMI_ACC_12_N_KWAJALEIN},//Marshall Islands - MH 38

    {TXT_ACC_WORLD_GMT_11_N_APIA,TXT_ACC_WORLD_GMT_11_N,549, MMI_ACC_11_N_APIA},// 4 Samoa - WS

    {TXT_ACC_WORLD_GMT_10_N_HONOLULU,TXT_ACC_WORLD_GMT_10_N,310, MMI_ACC_10_N_HONOLULU},//USA 66

    {TXT_ACC_WORLD_GMT_9P5_N_POLYNESIA,TXT_ACC_WORLD_GMT_9P5_N,547, MMI_ACC_9P5_N_POLYNESIA},//French Polynesia 10

    {TXT_ACC_WORLD_GMT_9_N_ANCHORAGE,TXT_ACC_WORLD_GMT_9_N,310, MMI_ACC_9_N_ANCHORAGE},// 2 310,~316United States of America

    {TXT_ACC_WORLD_GMT_8P5_N_EASTER_ISLAND,TXT_ACC_WORLD_GMT_8_N,730,MMI_ACC_8P5_N_EASTER_ISLAND},//Chile - CL 22

    {TXT_ACC_WORLD_GMT_8_N_LOSANGELES,TXT_ACC_WORLD_GMT_8_N,310, MMI_ACC_8_N_LOSANGELES},// //310~316United States of America 44
    {TXT_ACC_WORLD_GMT_8_N_SAN_FRANCISCO,TXT_ACC_WORLD_GMT_8_N,310, MMI_ACC_8_N_SAN_FRANCISCO},////United States of America 32
    {TXT_ACC_WORLD_GMT_8_N_VANCOUVER,TXT_ACC_WORLD_GMT_8_N,302, MMI_ACC_8_N_VANCOUVER},//Canada 69

    {TXT_ACC_WORLD_GMT_7_N_PHOENIX,TXT_ACC_WORLD_GMT_7_N,310,MMI_ACC_7_N_PHOENIX},//US 21

    {TXT_COMMON_WORLD_MEXICO,TXT_ACC_WORLD_GMT_6_N,334, MMI_ACC_6_N_MEXICO},//Mexico 53
    {TXT_ACC_WORLD_GMT_6_N_CHICAGO,TXT_ACC_WORLD_GMT_6_N,310, MMI_ACC_6_N_CHICAGO},//USA 80

    {TXT_ACC_WORLD_GMT_5_N_BOGOTA,TXT_ACC_WORLD_GMT_5_N,732, MMI_ACC_5_N_BOGOTA},//Colombia (Republic of) 59
    {TXT_ACC_WORLD_GMT_5_N_MONTREAL,TXT_ACC_WORLD_GMT_5_N,302, MMI_ACC_5_N_MONTREAL},//Canada 50
    {TXT_ACC_WORLD_GMT_5_N_NEWYORK,TXT_ACC_WORLD_GMT_5_N,310, MMI_ACC_5_N_NEWYORK},//us 54 

    {TXT_ACC_WORLD_GMT_4_N_SANTIAGO,TXT_ACC_WORLD_GMT_4_N,730, MMI_ACC_4_N_SANTIAGO},//Chile 58

    {TXT_ACC_WORLD_GMT_3P5_N_LAPAZ,TXT_ACC_WORLD_GMT_3P5_N,736, MMI_ACC_3P5_N_LAPAZ},//Bolivia - BO 39

    {TXT_ACC_WORLD_GMT_3_N_BUENOS_AIRES,TXT_ACC_WORLD_GMT_3_N,722,MMI_ACC_3_N_BUENOS_AIRES},//Argentina - AR 16
    {TXT_ACC_WORLD_GMT_3_N_BRASILIA,TXT_ACC_WORLD_GMT_3_N,724, MMI_ACC_3_N_BRASILIA},//Brazil 6

    {TXT_ACC_WORLD_GMT_2_N_CENTRAL_ATLANTIC,TXT_ACC_WORLD_GMT_2_N,208,MMI_ACC_2_N_CENTRAL_ATLANTIC},//central atlantic is France 18

    {TXT_ACC_WORLD_GMT_1_N_PRAIA,TXT_ACC_WORLD_GMT_1_N,625, MMI_ACC_1_N_PRAIA},//Cape Verde (Republic of)佛得角 56
};

#define   WT_TOTAL_CITY_NUM  (sizeof(s_zone_city_info)/sizeof(s_zone_city_info[0]))

LOCAL   uint16                s_city_real_order[WT_TOTAL_CITY_NUM] = {0};

#if defined(MMI_UPDATE_TIME_SUPPORT)
//LOCAL BOOLEAN s_is_summer_flag_nitz_dst =0;
#define TONGA_DEFAULT_TIME_ZONE_IN_MIN    (15*4*13)
//multi cities in one timezone of one one country,if find the same mcc and timezone,NITZ first set this city
LOCAL const WT_TIME_CITY_ZONE      s_default_timezone_city_info[] = 
{
    {TXT_ACC_WORLD_GMT_1_P_GENEVA, TXT_ACC_WORLD_GMT_1_P, 228, MMI_ACC_1_P_GENEVA},      //Switzerland (Confederation of)
    {TXT_ACC_WORLD_GMT_8_N_SAN_FRANCISCO, TXT_ACC_WORLD_GMT_8_N, 310, MMI_ACC_8_N_SAN_FRANCISCO},////United States of America
    {TXT_ACC_WORLD_GMT_5P5_P_NEWDELHI, TXT_ACC_WORLD_GMT_5P5_P, 404, MMI_ACC_5P5_P_NEWDELHI},//India (Republic of)
    {TXT_ACC_WORLD_GMT_10_P_SYDNEY, TXT_ACC_WORLD_GMT_10_P, 505, MMI_ACC_10_P_SYDNEY},////Australia - AU
    {TXT_ACC_WORLD_GMT_12_P_WELLINGTON, TXT_ACC_WORLD_GMT_12_P, 530, MMI_ACC_12_P_WELLINGTON},//New Zealand 67
    {TXT_ACC_WORLD_GMT_0_CASABLANCA, TXT_ACC_WORLD_GMT_0, 604, MMI_ACC_0_CASABLANCA},//Morocco - MA
    //reserved for others
};

//this is load form outer internet,it is defined by the law,and cannot be changed
LOCAL const WT_TIME_DST_VALUE_T      s_all_dst_value_info[] = 
{
    // 2012
    //mcc, year, timezone, dst time,{sec,min, hour, mday, mon, year(0),wday(0),yday(0), isdst(0)},
    
    {432,2012,210,270,{0, 30, 19, 20, 3, 0, 0, 0, 0},{0, 30, 19, 20, 9, 0, 0, 0, 0}}, //Iran - All locations Tehran 
    {549,2012,660,720,{0,0,14,28,9,0,5,0,0},{0,0,14,1,4,0,6,0,0}}, //Samoa - All locations Apia 
    {505,2012,600,660,{0,0,16,6,10,0,0,0,0},{0,0,16,31,3,0,0,0,0}}, //Australia Melbourne
    
    {730,2012,-360,-300,{0,0,4,14,10,0,0,0,0},{0,0,4,11,3,0,0,0,0}}, //Chile - Some locations Easter Island 
    {730,2012,-240,-180,{0,0,4,14,10,0,0,0,0},{0,0,4,11,3,0,0,0,0}}, //Chile - Most locations Punta Arenas Santiago 

    {724,2012,-180,-120,{0,0,3,21,10,0,0,0,0},{0,0,3,26,2,0,0,0,0}}, //Brazil - Most of locations observing DST Brasilia  Rio de Janeiro  Salvador Sao Paulo  
    {724,2012,-240,-180,{0,0,3,21,10,0,0,0,0},{0,0,3,26,2,0,0,0,0}}, //Brazil -Some of locations observing DSTCampo Grande  

    {268,2012,0,60,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Portugal - Most locations Lisbon 
    {234,2012,0,60,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //United Kingdom - All locations Belfast (Northern Ireland) Cardiff (Wales)Glasgow (Scotland)London (England) 
    
    {232,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Austria Vienna
    {206,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Belgium  Brussels
    {230,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Czech Republic - All locations Prague 
    {238,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Denmark - All locations Copenhagen  
    {208,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //France - All of locations observing DST Paris   
    {262,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Germany - All locations Berlin (Berlin) Frankfurt  
    {216,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Hungary - All locations Budapest   
    {222,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Italy - All locations Milan Rome   
    {270,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Luxembourg - All locations Luxembourg  
    {204,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Netherlands - All of locations observing DST Amsterdam  
    {260,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Poland - All locations Warsaw 
    {214,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Spain - Most locations Barcelona (Barcelona) Madrid  Palma (Majorca)
    {240,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Sweden - All locations Stockholm 
    {228,2012,60,120,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Switzerland - All locations Bern Geneva  Zurich 

    {284,2012,120,180,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Bulgaria - All locations Sofia 
    {202,2012,120,180,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Greece - All locations Athens 
    {226,2012,120,180,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Romania - All locations Bucharest 
    {286,2012,120,180,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Turkey - All locations Ankara Istanbul 
    {255,2012,120,180,{0,0,1,25,3,0,0,0,0},{0,0,1,28,10,0,0,0,0}}, //Ukraine - All locations Kyiv Odesa 
    
    {334,2012,-360,-300,{0,0,7,1,4,0,0,0,0},{0,0,7,28,10,0,0,0,0}}, //Mexico Most of locations Mexico City (Federal District)

    {302,2012,-300,-240,{0,0,6,11,3,0,0,0,0},{0,0,6,4,11,0,0,0,0}}, //Canada - Some of locations observing DST Montreal (Quebec)Ottawa Pond Inlet Quebec Toronto  
    {310,2012,-300,-240,{0,0,6,11,3,0,0,0,0},{0,0,6,4,11,0,0,0,0}}, //United States - Some of locations observing DST Atlanta  Detroit New York (New York)

    {334,2012,-360,-300,{0,0,7,11,3,0,0,0,0},{0,0,7,4,11,0,0,0,0}}, //Mexico Some of locations Matamoros 
    {302,2012,-360,-300,{0,0,7,11,3,0,0,0,0},{0,0,7,4,11,0,0,0,0}}, //Canada - Some of locations observing DST Baker Lake 
    {310,2012,-360,-300,{0,0,7,11,3,0,0,0,0},{0,0,7,4,11,0,0,0,0}}, //United States - Some of locations observing DST Austin (Texas)Chicago 

    {302,2012,-420,-360,{0,0,8,11,3,0,0,0,0},{0,0,8,4,11,0,0,0,0}}, //Canada - Some of locations observing DST Yellowknife 
    {310,2012,-420,-360,{0,0,8,11,3,0,0,0,0},{0,0,8,4,11,0,0,0,0}}, //United States - Some of locations observing DST Denver 
    {334,2012,-420,-360,{0,0,8,11,3,0,0,0,0},{0,0,8,4,11,0,0,0,0}}, //Mexico Some of locations Ciudad Juárez (Chihuahua)
    
    {302,2012,-480,-420,{0,0,9,11,3,0,0,0,0},{0,0,9,4,11,0,0,0,0}}, //Canada - Some of locations observing DST Vancouver 
    {334,2012,-480,-420,{0,0,9,11,3,0,0,0,0},{0,0,9,4,11,0,0,0,0}}, //Mexico Some of locations Mexicali (Baja California)
    {310,2012,-480,-420,{0,0,9,11,3,0,0,0,0},{0,0,9,4,11,0,0,0,0}}, //United States - Some of locations observing DST Las Vegas Los Angeles Portland 

    {310,2012,-540,-480,{0,0,10,11,3,0,0,0,0},{0,0,10,4,11,0,0,0,0}}, //United States - Some of locations observing DST Anchorage 
    {310,2012,-600,-540,{0,0,11,11,3,0,0,0,0},{0,0,10,4,11,0,0,0,0}}, //United States - Some of locations observing DST Adak (Alaska)
 
    {368,2012,-300,-240,{0,0,5,18,3,0,0,0,0},{0,0,5,11,11,0,0,0,0}}, //United States - Some of locations observing DST Adak (Alaska)

    {250,2012,240,240,{0,0,0,0,0,0, 0,0, 0},{0,0,0,0,0,0,0,0,0}}, //DST all year in 2012

    // 2013
    {0,0,0,0,{0,0,0,0,0,0, 0,0,0},{0,0,0,0,0,0,0,0,0}},
};
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/***********************************************************************************
 Name:			MMIALM_IsDateValidByString
 Description:	This function checks the date the user has entered before it is saved.
				(The date format: YYYY_MM_DD)
 Input:			date -- 
 Return:		 True if  date is ok, otherwise false.
***********************************************************************************/
LOCAL BOOLEAN  MMIALM_IsDateValidByString(MMIACC_RTC_DATE_T* date);

/*****************************************************************************/
//   FUNCTION:      MMIACC_SetRTCTimeByString
// 	Description :   set current sys time
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN MMIACC_SetRTCTimeByString( MMIACC_RTC_TIME_T *time );

/*****************************************************************************/
//  FUNCTION:      MMIACC_SetRTCDateByString
// 	Description :   set current sys date
//	Global resource dependence : 
//  Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN MMIACC_SetRTCDateByString( MMIACC_RTC_DATE_T *date );

/*****************************************************************************/
// 	Description : get weekday from mon to sun (1 to 7)
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
// LOCAL int16 GetWeekDayByDate(
// 							 int16 year,
// 							 int16  month,
// 							 int16 day
// 							 );

/*****************************************************************************/
// 	Description : convert string type of date to unicode type, including week day information
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void ConvertDateToShowFormat( 
								   MMIACC_RTC_DATE_T *current_date, 
								   MMI_STRING_T *convert_date 
								   );

/*****************************************************************************/
// 	Description : init world clock application
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void UpdateDisplayCityInfo(
					void
					);


/*****************************************************************************/
// 	Description :caculate date and time
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void CalculateCityTime(
                             uint8 operate_info,
                             MMIACC_RTC_DATE_T* work_date,
                             MMIACC_RTC_TIME_T* work_time,
                             MMI_TEXT_ID_T dest_city_index
                             );


/*****************************************************************************/
// 	Description : the process message function of the accessory menu
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleWclkWinMsg( 
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									);
/*****************************************************************************/
// 	Description : the process message function of the accessory menu
//	Global resource dependence : 
//   Author:        murphy.xie
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleWclkOptionDeleteCityWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );

/*****************************************************************************/
// 	Description : the process message function of the accessory menu
//	Global resource dependence : 
//  Author:        murphy.xie
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleWclkSetDSTWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );

/*****************************************************************************/
// 	Description : the process message function of the accessory menu
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleWclkOptionWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );

/*****************************************************************************/
// 	Description : the process message function of the accessory menu
//	Global resource dependence : 
//  Author:murphy.xie
//	Note:   
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleWclkAddCityWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );
/*****************************************************************************/
// Description : the process message function of the accessory menu
// Global resource dependence : 
// Author:        murphy.xie
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWclkCityListOptionWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );

/*****************************************************************************/
// Description : the process message function of the accessory menu
// Global resource dependence : 
// Author:        murphy.xie
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWclkCityListOrderWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );
/*****************************************************************************/
// 	Description : SetWorldClockToLocal
//         
//	Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN: 
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN SetWorldClockToLocal(MMI_TEXT_ID_T city_index);


/*****************************************************************************/
// 	Description : display local city information such as time, date, time difference etc.
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void DisplayLocalCityInfo(void);

/*****************************************************************************/
// 	Description : display current city information such as time difference, time, date etc.
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void DisplayotherCityInfo(MMI_TEXT_ID_T dest_city_index,uint8 displayformindex);

/*****************************************************************************/
//   FUNCTION:      MMIACC_SetRTCDateTimeByString
// 	Description :   set current sys time
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN MMIACC_SetRTCDateTimeByString(
                                            MMIACC_RTC_DATE_T *date,
                                            MMIACC_RTC_TIME_T *time 
                                            );

/*****************************************************************************/
//  FUNCTION:       SetWCFormParam
// 	Description :   set the from parameter
//	Global resource dependence : 
//  Author:         murphy.xie
//  Date:           2010/05/12
//	Note:   
/*****************************************************************************/
LOCAL void SetWCFormParam(void);

/*****************************************************************************/
// 	Description : GetZoneTextIDByCityTextID
//	Global resource dependence : none
//  Author:         murphy.xie
//  Date:           2012-2-24
//	Note:   
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetZoneTextIDByCityTextID(MMI_TEXT_ID_T city_text_id);

/*****************************************************************************/
//  FUNCTION:       CompareCityByName
//	Global resource dependence : 
//  Author:         murphy.xie
//  Date:           2012-2-24
//	Note:   
/*****************************************************************************/
LOCAL int32 CompareCityByAlphabet(const void* node1,
                                  const void* node2)
{
    int32 ret_val;
    uint16 index1 = *(uint16 *)node1;
    uint16 index2 = *(uint16 *)node2;
    MMI_STRING_T name1 = {0};
    MMI_STRING_T name2 = {0};
    
    MMI_GetLabelTextByLang(s_zone_city_info[index1].city_index,&name1);
    MMI_GetLabelTextByLang(s_zone_city_info[index2].city_index,&name2);

    ret_val = MMIAPIPB_CmpStrByLetters( name1.wstr_ptr,
                                        name1.wstr_len,
                                        name2.wstr_ptr,
                                        name2.wstr_len);
    return ret_val;
}

/*****************************************************************************/
//  FUNCTION:       InitCityOrderBySort
//	Global resource dependence : 
//  Author:         murphy.xie
//  Date:           2012-2-24
//	Note:   
/*****************************************************************************/
LOCAL void InitCityOrderBySort(void)
{
    uint16                   i =0;
    for(i = 0; i < WT_TOTAL_CITY_NUM;i++)
    {
        s_city_real_order[i] = i;
    }

    if(CITY_ORDER_BY_ALPHABET == s_city_list_order)
    {
        qsort((void*)s_city_real_order, WT_TOTAL_CITY_NUM, sizeof(s_city_real_order[0]), CompareCityByAlphabet);/*lint !e64*/
    }
}

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
WINDOW_TABLE( WCLK_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleWclkWinMsg ),
    WIN_ID( MMIACC_WCLK_WIN_ID ),
    WIN_TITLE(TXT_TOOLS_WORLDTIME),
#if !defined(MAINLCD_SIZE_128X160) 
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIWT_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWT_CITY_1_INFO_FORM_CTRL_ID, MMIWT_FORM_CTRL_ID),
            CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID, MMIWT_CITY_1_INFO_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_CITY_1_INFO_CITY_CTRL_ID, MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_RIGHT,FALSE,MMIWT_CITY_1_INFO_LOCAL_ICON_CTRL_ID, MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID),
            CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID, MMIWT_CITY_1_INFO_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_CITY_1_INFO_TIME_CTRL_ID, MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_CITY_1_INFO_DATE_CTRL_ID,MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_RIGHT,FALSE,MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID, MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID),
#else
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIWT_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWT_CITY_1_INFO_FORM_CTRL_ID, MMIWT_FORM_CTRL_ID),
            CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID, MMIWT_CITY_1_INFO_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_CITY_1_INFO_CITY_CTRL_ID, MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_RIGHT,FALSE,MMIWT_CITY_1_INFO_LOCAL_ICON_CTRL_ID, MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID),
            CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID, MMIWT_CITY_1_INFO_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_CITY_1_INFO_TIME_CTRL_ID, MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_RIGHT,FALSE,MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID, MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_CITY_1_INFO_DATE_CTRL_ID,MMIWT_CITY_1_INFO_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(STXT_OPTION, STXT_ADD, STXT_RETURN),

    END_WIN
};

#else
WINDOW_TABLE( WCLK_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleWclkWinMsg ),
    WIN_ID( MMIACC_WCLK_WIN_ID ),

    //WIN_TITLE(TXT_TOOLS_WORLDTIME),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIWT_FORM_CTRL_ID),
#ifdef TOUCH_PANEL_SUPPORT    
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIWT_ADD_CITY_FORM_CTRL_ID,MMIWT_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIWT_BOTTON_ADD_CITY_ID,MMIWT_ADD_CITY_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_LABEL_ADD_CITY_ID,MMIWT_ADD_CITY_FORM_CTRL_ID),
#endif            
         CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIWT_CITY_1_FORM_CTRL_ID,MMIWT_FORM_CTRL_ID),
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIWT_CITY_1_INFO_FORM_CTRL_ID,MMIWT_CITY_1_FORM_CTRL_ID), 
                CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID,MMIWT_CITY_1_INFO_FORM_CTRL_ID), 
                    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_CITY_1_INFO_CITY_CTRL_ID,MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID),
                    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID,MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,TRUE,MMIWT_CITY_1_INFO_TIME_CTRL_ID,MMIWT_CITY_1_INFO_FORM_CTRL_ID),
                CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIWT_CITY_1_INFO_DATE_CTRL_ID,MMIWT_CITY_1_INFO_FORM_CTRL_ID),
    #if defined(ANALOG_CLK_SUPPORT)
            CHILD_ANIM_CTRL(FALSE,TRUE,MMIWT_CITY_1_CLOCK_CTRL_ID,MMIWT_CITY_1_FORM_CTRL_ID),
    #endif
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),

    END_WIN
};
#endif

WINDOW_TABLE( WCLK_DELETE_CITY_TAB ) = 
{
    WIN_FUNC( (uint32)HandleWclkOptionDeleteCityWinMsg ),
    WIN_ID(MMIACC_WCLK_DELETE_CITY_WIN_ID ),
    WIN_TITLE(TXT_ACC_DELETE_CITY),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E,MMIACC_WCLK_OPTION_DELETE_LIST_CTRL_ID),
    
    END_WIN
};


WINDOW_TABLE( WCLK_SET_DST_CITY_TAB ) = 
{
    WIN_FUNC( (uint32)HandleWclkSetDSTWinMsg ),    
    WIN_ID(MMIACC_WCLK_SET_DST_WIN_ID ),
    WIN_TITLE(TXT_ACC_SET_DST),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E,MMIACC_WCLK_OPTION_SET_DST_LIST_CTRL_ID),
    
    END_WIN
};

WINDOW_TABLE( WCLK_OPTION1_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleWclkOptionWinMsg),
    WIN_ID(MMIACC_WCLK_OPTION1_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_WCLK_DST_OPTION,MMIACC_WCLK_OPTION_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( WCLK_ADD_CITY_WIN_TAB ) = 
{ 
    WIN_FUNC( (uint32)HandleWclkAddCityWinMsg), 
    WIN_TITLE(TXT_ACC_ADD_CITY),
    WIN_ID( MMIACC_WCLK_ADD_CITY_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIWT_ADD_CITY_LIST_CTRL_ID),
#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_OK, STXT_RETURN),
#else
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

WINDOW_TABLE( WCLK_CITY_LIST_OPTION_WIN_TAB ) = 
{ 
    WIN_FUNC( (uint32)HandleWclkCityListOptionWinMsg), 
    WIN_ID( MMIACC_WCLK_CITY_LIST_OPTION_WIN_ID ),
    CREATE_POPMENU_CTRL(MENU_WCLK_CITY_LIST_OPTION,MMIACC_WCLK_ADD_CITY_OPTION_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( WCLK_CITY_LIST_OPTION_ORDER_WIN_TAB ) = 
{ 
    WIN_FUNC( (uint32)HandleWclkCityListOrderWinMsg), 
    WIN_TITLE(TXT_ACC_WORLD_CITY_LIST_ORDER),
    WIN_ID( MMIACC_WCLK_CITY_LIST_ODER_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIACC_WCLK_ORDER_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
/**---------------------------------------------------------------------------*
 **                       function bodies  -- global
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                       function bodies  -- local
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                     local function in world clock 
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : GetZoneTextIDByCityTextID
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetZoneTextIDByCityTextID(MMI_TEXT_ID_T city_text_id)
{
    uint16 total_city_num = sizeof(s_zone_city_info)/sizeof(WT_TIME_CITY_ZONE);
    int32  i = 0;
    MMI_TEXT_ID_T  zone_text_id = TXT_NULL;
    for(i = 0; i < total_city_num; i++)
    {
        if(s_zone_city_info[i].city_index == city_text_id)
        {
            zone_text_id = s_zone_city_info[i].zone_index;
            break;
        }
    }
    
    return zone_text_id;
}

/*****************************************************************************/
// 	Description : GetZoneMinByZoneTextID
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL int16 GetZoneMinByZoneTextID(MMI_TEXT_ID_T zone_text_id)
{
    int16 zone_min = 0;
    
    if((zone_text_id - TXT_ACC_WORLD_GMT_0) < WT_TIME_ZONE_NUM_TOTAL)
    {
        zone_min = s_time_zone_arr[zone_text_id- TXT_ACC_WORLD_GMT_0];
    }
    
    return zone_min;
}
/*****************************************************************************/
// 	Description : GetZoneTextIDByCityTextID
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetSummerFlagByCityTextID(MMI_TEXT_ID_T text_id)
{
    int32       i = 0;
    BOOLEAN     flag = FALSE;
    for(i = 0; i < WT_DEFAULT_CITY_NUM; i++)
    {
        if(s_default_city_info[i].city_text_id == text_id)
        {
            if(s_default_city_info[i].summer_flag)
            {
                flag = TRUE;
                break;
            }
        }
    }
    
    return flag;
}
/*****************************************************************************/
//  Description : alpha blending RGB565 data quickly
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16 BlendRGB565(uint16 bk_color, uint16 fore_color, uint8 alpha)
{
    uint32 blend_color = 0;
    uint32 temp_back_color = 0;
    uint32 temp_fore_color = 0;
    
    alpha = (alpha + 1) >> 3;

    temp_fore_color = ( fore_color | (fore_color << 16)) & BLEND_MASK;
    temp_back_color = ( bk_color | (bk_color << 16)) & BLEND_MASK;
    
    blend_color = ((((temp_fore_color - temp_back_color) * alpha ) >> 5 ) + temp_back_color) & BLEND_MASK;
    blend_color = ((blend_color & 0xFFFF) | (blend_color >> 16));
    
    return (uint16)blend_color;
}

/*****************************************************************************/
//  Description : Alpha blend two images
//  Global resource dependence : 
//  Author:
//  Note: the output image is in the background image buffer, overwritting the orignial data.
//		  the type of the output image data is the same as that of the background image
/*****************************************************************************/
LOCAL BOOLEAN ProcessAlphaBlending(MMI_WORLD_CLOCK_ALPHA_IMG_T *bk_img_ptr, MMI_WORLD_CLOCK_ALPHA_IMG_T *fore_img_ptr,GUI_RECT_T display_rect)
{
#if defined(ANALOG_CLK_SUPPORT)

    uint32 fore_width  = fore_img_ptr->width;
    uint32 fore_height = fore_img_ptr->height;
    uint32 bk_width = bk_img_ptr->width;
    int32 start_x = fore_img_ptr->start_x;
    int32 start_y = fore_img_ptr->start_y;
    uint32 blend_width = fore_width;
    uint32 blend_height = fore_height;
    uint32 *fore_ptr = (uint32 *)fore_img_ptr->data_ptr;
    GUI_RECT_T dst_rect = {0};
    GUI_RECT_T bk_rect = {0};
    GUI_RECT_T for_rect = {0};  
    
    bk_rect.left  = bk_img_ptr->start_x;
    bk_rect.top   = bk_img_ptr->start_y;
    bk_rect.right = bk_img_ptr->start_x + bk_img_ptr->width-1;
    bk_rect.bottom  = bk_img_ptr->start_y + bk_img_ptr->height-1;    
    
    for_rect.left  = fore_img_ptr->start_x;
    for_rect.top   = fore_img_ptr->start_y;
    for_rect.right = fore_img_ptr->start_x + fore_img_ptr->width-1;
    for_rect.bottom  = fore_img_ptr->start_y + fore_img_ptr->height-1;    

    if (!GUI_IntersectRect(&dst_rect, bk_rect, for_rect))
    {
        return FALSE;
    }


    if(!GUI_IntersectRect(&dst_rect, dst_rect, display_rect))
    {
        return FALSE;
    }
    
    //交叉区域
    blend_width = (uint32)(dst_rect.right-dst_rect.left+1);
    blend_height = (uint32)(dst_rect.bottom-dst_rect.top+1);

    //从fore img什么地方开始裁 
    start_x = dst_rect.left - fore_img_ptr->start_x;
    start_y = dst_rect.top - fore_img_ptr->start_y;
    
    if (IMGREF_FORMAT_RGB565 == bk_img_ptr->data_type)     //output RGB565
    {        
        uint32 i = 0, j = 0;
        uint16 *bk_ptr = (uint16 *)bk_img_ptr->data_ptr;

        bk_ptr += (dst_rect.top * bk_width + dst_rect.left);       /*lint !e737*/
        fore_ptr+=  (start_y * fore_width + start_x); /*lint !e737*/
        
        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                uint32 alpha, r, g, b;
                uint32 fore_value = *(fore_ptr + j);    
                uint16 bk_value = *(bk_ptr + j);

                alpha = ARGB_GET_A(fore_value);
                r = ARGB_GET_R(fore_value);
                g = ARGB_GET_G(fore_value);
                b = ARGB_GET_B(fore_value);

                fore_value = RGB888_TO_RGB565(r, g, b);
                bk_value = BlendRGB565(bk_value, (uint16)fore_value, alpha);
                *(bk_ptr + j) = bk_value;
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    } 
#endif
    return TRUE;
}
/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DrawAnlogClock(MMI_WIN_ID_T win_id,int16 left,int16 top,GUI_RECT_T display_rect,SCI_TIME_T sys_time)
{
#if defined(ANALOG_CLK_SUPPORT)
    GUI_RECT_T        win_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    CLOCK_DRAW_IN_T   aclock_input_param  = {0};
    CLOCK_DRAW_OUT_T  aclock_output_param = {0};
    MMI_WORLD_CLOCK_ALPHA_IMG_T bk_img = {0};
    MMI_WORLD_CLOCK_ALPHA_IMG_T fore_img = {0};
    UILAYER_INFO_T    layer_info = {0};
    uint16            lcd_width  = 0;
    uint16            lcd_height = 0;
    uint16            width  = 0;
    uint16            height = 0;
    MMI_IMAGE_ID_T    clock_bg = IMAGE_CLOCK_WORLDCLOCK_NIGHT_BG;
    MMI_IMAGE_ID_T    clock_hour = IMAGE_CLOCK_WORLDCLOCK_NIGHT_HOUR;
    MMI_IMAGE_ID_T    clock_min = IMAGE_CLOCK_WORLDCLOCK_NIGHT_MINUTE;
    MMI_IMAGE_ID_T    clock_center = IMAGE_CLOCK_WORLDCLOCK_NIGHT_CENTER;
    GUI_RECT_T        image_rect = {0};

    //daytime
    if(sys_time.hour >= 6 && sys_time.hour <18)
    {
        clock_bg = IMAGE_CLOCK_WORLDCLOCK_DAYTIME_BG;
        clock_hour = IMAGE_CLOCK_WORLDCLOCK_DAYTIME_HOUR;
        clock_min = IMAGE_CLOCK_WORLDCLOCK_DAYTIME_MINUTE;
        clock_center = IMAGE_CLOCK_WORLDCLOCK_DAYTIME_CENTER;
    }
 
    //aclock dial.
    aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, clock_bg, win_id);
    aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(clock_bg,win_id,&(aclock_input_param.clock_dial.data_size));
    aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
    aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2;   

    //aclock hour hand
    aclock_input_param.hour_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.hour_handle.width, &aclock_input_param.hour_handle.height, clock_hour, win_id);    
    aclock_input_param.hour_handle.data_ptr = (uint8 *)MMI_GetLabelImage(clock_hour,win_id,&(aclock_input_param.hour_handle.data_size));
    aclock_input_param.hour_handle.center_x = aclock_input_param.hour_handle.width/2;
    aclock_input_param.hour_handle.center_y = aclock_input_param.hour_handle.height/2;    

    //aclock min hand
    aclock_input_param.min_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.min_handle.width, &aclock_input_param.min_handle.height, clock_min, win_id);        
    aclock_input_param.min_handle.data_ptr = (uint8 *)MMI_GetLabelImage(clock_min,win_id,&(aclock_input_param.min_handle.data_size));
    aclock_input_param.min_handle.center_x = aclock_input_param.min_handle.width/2;
    aclock_input_param.min_handle.center_y = aclock_input_param.min_handle.height/2; 

    aclock_input_param.target_buf_size = aclock_input_param.clock_dial.width * aclock_input_param.clock_dial.height * 4;
    aclock_input_param.target_buf_ptr = (void*)SCI_ALLOCA(aclock_input_param.target_buf_size);

    if (PNULL == aclock_input_param.target_buf_ptr)
    {
        //SCI_TRACE_LOW:"error: DrawAClock target_buf_ptr alloc failed,  return directly"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_721_112_2_18_2_2_28_110,(uint8*)"");
        return;
    }

    aclock_input_param.time.hour = sys_time.hour;
    aclock_input_param.time.minute  = sys_time.min;
    aclock_input_param.time.second = sys_time.sec;
    
    if(0 != GRAPH_DrawClock(&aclock_input_param, &aclock_output_param))
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
        return ;
    }

    //display aclock
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    UILAYER_GetLayerInfo(&lcd_dev_info, &layer_info);
    
    bk_img.data_type = IMGREF_FORMAT_RGB565;

    bk_img.data_ptr = UILAYER_GetLayerBufferPtr(&lcd_dev_info);
    if (PNULL == bk_img.data_ptr)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
        return;
    }

    bk_img.width = layer_info.mem_width;
    bk_img.height = layer_info.layer_height;
    
    fore_img.data_ptr = aclock_output_param.output_ptr;
    fore_img.data_type = IMGREF_FORMAT_ARGB888;
    fore_img.width = aclock_input_param.clock_dial.width;
    fore_img.height = aclock_input_param.clock_dial.height;
   
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    fore_img.start_x = left;   
    fore_img.start_y = top; 
    
    if (!ProcessAlphaBlending(&bk_img, &fore_img,display_rect))
    {
        //SCI_TRACE_LOW:"error: foreground image rect is not in background image rect, return directly"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_765_112_2_18_2_2_28_111,(uint8*)"");
        SCI_FREE(aclock_input_param.target_buf_ptr);    
        return;
    }

    //in the end, display the center point.
    GUIRES_GetImgWidthHeight(&width, &height, clock_center, win_id); 

    image_rect.left = left+ (aclock_input_param.clock_dial.width - width)/2;
    image_rect.top = top + (aclock_input_param.clock_dial.height - height)/2;
    image_rect.right = image_rect.left + width;
    image_rect.bottom = image_rect.top + height;
    
    if(!GUI_IntersectRect(&display_rect, display_rect, image_rect))
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
        return;
    }

    GUIRES_DisplayImg(PNULL,
                      &display_rect,
                      PNULL,
                      win_id,
                      clock_center,
                      &lcd_dev_info);
        
    SCI_FREE(aclock_input_param.target_buf_ptr);
#endif

}

/*****************************************************************************/
// 	Description :caculate current date and time by local date time
//	Global resource dependence : none
//  Author: murphy.xie
//	Note: 
/*****************************************************************************/

LOCAL void CalculateOtherAnalogClockTime( uint8 operate_info,
                                MMI_TEXT_ID_T local_city_index,
                                MMI_TEXT_ID_T dest_city_index,
                                SCI_TIME_T* dest_systime
                             )
{
    SCI_TIME_T  local_systime = {0};
	int16          local_hour_int;  
	int16          local_min_int;  
	int16          world_hour_int = 0;  
	int16          world_min_int = 0;  
	int16          time_diff_tmp;  
	int16		  local_total_min;
	int16 		  local_day_diff = 0;      

	/****************************/  
	/* Read the local date&time */  
	/****************************/  
    TM_GetSysTime(&local_systime);
    
	/*****************************************/  
	/* Convert the time to integers for hour */  
	/*****************************************/  
	local_hour_int = local_systime.hour;  
	
	/*****************************************/  
	/* Convert the time to integers for min */  
	/*****************************************/  
	local_min_int = local_systime.min;  

	/**************/  
	/* Check time */  
	/**************/  
	if (( local_hour_int < 0 ) || ( local_hour_int > 23 )) local_hour_int = 0;


	/*****************************/  
	/* Calculate the world time  */  
	/*****************************/  

    if(operate_info == 0)
    {
        /*********************************/  
        /* Calculate the time difference */  
        /*********************************/  
        
	    time_diff_tmp  = GetZoneMinByZoneTextID(GetZoneTextIDByCityTextID(dest_city_index)) - GetZoneMinByZoneTextID(GetZoneTextIDByCityTextID(local_city_index));  

        local_total_min = (local_hour_int - GetSummerFlagByCityTextID(local_city_index)+ GetSummerFlagByCityTextID(dest_city_index))*60 + time_diff_tmp+local_min_int;
        if(local_total_min < 0)
        	local_day_diff = -1;
        else if(local_total_min > (24*60))
        	local_day_diff = 1;
        else
        	local_day_diff = 0;
        local_total_min = (local_total_min +24*60)%(24*60);
        world_hour_int = local_total_min/60;  
        world_min_int  = local_total_min%60; 
    }
    else if(operate_info == 1)
    {  
		world_hour_int = local_hour_int + 1;  
		world_min_int  = local_min_int;
		if( world_hour_int > 23 )
		{
			world_hour_int -= 24;  
			local_day_diff = +1;
		}

    }
	/*************************/  
	/* Output the world time */  
	/*************************/  	
    dest_systime->hour = world_hour_int;
    dest_systime->min = world_min_int;
    dest_systime->sec = local_systime.sec;

}


LOCAL void DisplayAnlogClockCallbackFun (GUIANIM_OWNER_DRAW_T    *draw_ptr)
{
    SCI_TIME_T local_systime = {0};

    int16 display_index = (MMK_GetCtrlId(draw_ptr->ctrl_handle) - MMIWT_CITY_1_CLOCK_CTRL_ID)/ONE_FROM_TOTAL_CHILD;
    
    TM_GetSysTime(&local_systime);

    if(display_index == 0)
    {
        DrawAnlogClock(MMIACC_WCLK_WIN_ID,draw_ptr->display_x,draw_ptr->display_y,draw_ptr->display_rect,local_systime);
    }
    //caculate other city time by local city time 
    else if(s_default_city_info[display_index].city_text_id != TXT_NULL)
    {
        CalculateOtherAnalogClockTime(0,s_default_city_info[0].city_text_id,s_default_city_info[display_index].city_text_id,&local_systime);
        DrawAnlogClock(MMIACC_WCLK_WIN_ID,draw_ptr->display_x,draw_ptr->display_y,draw_ptr->display_rect,local_systime);
    }
    
}

/*****************************************************************************/
//  Description : the callback of num botton in keyboard1
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
// LOCAL MMI_RESULT_E AddCityButtonCallBack(void)
// {
// 	MMK_CreateWin((uint32 *)WCLK_ADD_CITY_WIN_TAB,PNULL);
//     return MMI_RESULT_TRUE;
// }

#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
/*****************************************************************************/
//  Description : Add dynamic form
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void  AddWCFormDynamicControl(void)
{
    int32     i= 0;

    for(i=1; i< WT_DEFAULT_CITY_NUM; i++)
    {
        //MMIWT_CITY_1_FORM_CTRL_ID
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     form_init_data = {0};
        GUILABEL_INIT_DATA_T    label_init_data = {0};

#if defined(ANALOG_CLK_SUPPORT)
        GUIANIM_INIT_DATA_T     anim_init_data = {0};
#endif

        //MMIWT_CITY_1_INFO_FORM_CTRL_ID
        form_init_data.layout_type = GUIFORM_LAYOUT_ORDER;
        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIWT_CITY_1_INFO_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD;
        child_data.init_data_ptr = &form_init_data;
        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_FORM_CTRL_ID,
        &child_data);


        //MMIWT_CITY_1_INFO_FORM_CTRL_ID
        form_init_data.layout_type = GUIFORM_LAYOUT_SBS;
        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD;
        child_data.init_data_ptr = &form_init_data;
        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);


        //MMIWT_CITY_1_INFO_CITY_CTRL_ID
        label_init_data.align = GUILABEL_ALIGN_LEFT;
        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_LABEL_ID;
        child_data.child_handle  = MMIWT_CITY_1_INFO_CITY_CTRL_ID+i*ONE_FROM_TOTAL_CHILD;
        child_data.init_data_ptr = &label_init_data;
        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);

        //MMIWT_CITY_1_INFO_LOCAL_ICON_CTRL_ID
        label_init_data.align = GUILABEL_ALIGN_LEFT;
        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_LABEL_ID;
        child_data.child_handle  = MMIWT_CITY_1_INFO_LOCAL_ICON_CTRL_ID+i*ONE_FROM_TOTAL_CHILD;
        child_data.init_data_ptr = &label_init_data;
        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);


        //MMIWT_CITY_1_INFO_FORM2_CTRL_ID
        form_init_data.layout_type = GUIFORM_LAYOUT_SBS;
        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID+i*ONE_FROM_TOTAL_CHILD;
        child_data.init_data_ptr = &form_init_data;
        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);

        //MMIWT_CITY_1_INFO_TIME_CTRL_ID
        label_init_data.align = GUILABEL_ALIGN_LEFT;
        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_LABEL_ID;
        child_data.child_handle  = MMIWT_CITY_1_INFO_TIME_CTRL_ID+i*ONE_FROM_TOTAL_CHILD;
        child_data.init_data_ptr = &label_init_data;
        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);

        //MMIWT_CITY_1_INFO_DATE_CTRL_ID
        label_init_data.align = GUILABEL_ALIGN_LEFT;
        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_LABEL_ID;
        child_data.child_handle  = MMIWT_CITY_1_INFO_DATE_CTRL_ID+i*ONE_FROM_TOTAL_CHILD;
        child_data.init_data_ptr = &label_init_data;
        //create
#ifndef MAINLCD_SIZE_128X160  //3行格式
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);
#else
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);
#endif

        //MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID
        label_init_data.align = GUILABEL_ALIGN_RIGHT;
        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_LABEL_ID;
        child_data.child_handle  = MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID+i*ONE_FROM_TOTAL_CHILD;
        child_data.init_data_ptr = &label_init_data;
        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);
    }

}
#else
LOCAL void  AddWCFormDynamicControl(void)
{   
    int32     i= 0;

    for(i=1; i< WT_DEFAULT_CITY_NUM; i++)
    {
        //MMIWT_CITY_1_FORM_CTRL_ID
        GUIFORM_DYNA_CHILD_T    child_data = {0};
        GUIFORM_INIT_DATA_T     form_init_data = {0};
        GUILABEL_INIT_DATA_T    label_init_data = {0};
#if defined(ANALOG_CLK_SUPPORT)
        GUIANIM_INIT_DATA_T     anim_init_data = {0};
#endif
        //init data
        form_init_data.layout_type = GUIFORM_LAYOUT_SBS;

        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = (uint32)(MMIWT_CITY_1_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD);
        child_data.init_data_ptr = &form_init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_FORM_CTRL_ID,
        &child_data);
         

        //MMIWT_CITY_1_INFO_FORM_CTRL_ID
        form_init_data.layout_type = GUIFORM_LAYOUT_ORDER;

        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = (uint32)(MMIWT_CITY_1_INFO_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD);
        child_data.init_data_ptr = &form_init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        (uint32)(MMIWT_CITY_1_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD),
        &child_data);
       
         
        //MMIWT_CITY_1_INFO_FORM_CTRL_ID
        form_init_data.layout_type = GUIFORM_LAYOUT_SBS;

        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_FORM_ID;
        child_data.child_handle  = (uint32)(MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD);
        child_data.init_data_ptr = &form_init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);


        //MMIWT_CITY_1_INFO_CITY_CTRL_ID
        label_init_data.align = GUILABEL_ALIGN_LEFT;

        child_data.is_get_active = TRUE;
        child_data.guid          = SPRD_GUI_LABEL_ID;
        child_data.child_handle  = (uint32)(MMIWT_CITY_1_INFO_CITY_CTRL_ID+i*ONE_FROM_TOTAL_CHILD);
        child_data.init_data_ptr = &label_init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        (uint32)(MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD),
        &child_data);

        //MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID
        label_init_data.align = GUILABEL_ALIGN_LEFT;

        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_LABEL_ID;
        child_data.child_handle  = (uint32)(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID+i*ONE_FROM_TOTAL_CHILD);
        child_data.init_data_ptr = &label_init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);


        //MMIWT_CITY_1_INFO_TIME_CTRL_ID
        label_init_data.align = GUILABEL_ALIGN_LEFT;

        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_LABEL_ID;
        child_data.child_handle  = (uint32)(MMIWT_CITY_1_INFO_TIME_CTRL_ID+i*ONE_FROM_TOTAL_CHILD);
        child_data.init_data_ptr = &label_init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        (uint32)(MMIWT_CITY_1_INFO_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD),
        &child_data);
        
        //MMIWT_CITY_1_INFO_DATE_CTRL_ID
        label_init_data.align = GUILABEL_ALIGN_LEFT;

        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_LABEL_ID;
        child_data.child_handle  = (uint32)(MMIWT_CITY_1_INFO_DATE_CTRL_ID+i*ONE_FROM_TOTAL_CHILD);
        child_data.init_data_ptr = &label_init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_INFO_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);

        //MMIWT_CITY_1_CLOCK_CTRL_ID
        //im_init_data.is_form_bg= FALSE;
    #if defined(ANALOG_CLK_SUPPORT)
        child_data.is_get_active = FALSE;
        child_data.guid          = SPRD_GUI_ANIM_ID;
        child_data.child_handle  = (uint32)(MMIWT_CITY_1_CLOCK_CTRL_ID+i*ONE_FROM_TOTAL_CHILD);
        child_data.init_data_ptr = &anim_init_data;

        //create
        GUIFORM_CreatDynaChildCtrl(MMIACC_WCLK_WIN_ID,
        MMIWT_CITY_1_FORM_CTRL_ID+i*ONE_FROM_TOTAL_CHILD,
        &child_data);
    #endif

    }

}
#endif
/*****************************************************************************/
//  FUNCTION:       SetWCFormParam
// 	Description :   set the from parameter
//	Global resource dependence : 
//  Author:         murphy.xie
//  Date:           2010/05/12
//	Note:   
/*****************************************************************************/
LOCAL void SetWCFormParam(void)
{
#if defined(ANALOG_CLK_SUPPORT)
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
    GUIANIM_CTRL_INFO_T         control_info = {0};
#endif
    int32                       index = -1;
#if defined(TOUCH_PANEL_SUPPORT) || defined(ANALOG_CLK_SUPPORT)
    uint16                      image_width = 0,image_height = 0; /*lint !e529*/
#endif
//#elif TOUCH_PANEL_SUPPORT 
    GUIFORM_CHILD_WIDTH_T       child_width = {0};
//   uint16                      image_width = 0,image_height = 0;
//#else
//#if defined(ANALOG_CLK_SUPPORT)
//    GUIFORM_CHILD_WIDTH_T       child_width = {0};
//    uint16                      image_width = 0,image_height = 0;
//#endif
//#endif


#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
    MMI_STRING_T                string = {0};
#endif
    //add dynamic control
    AddWCFormDynamicControl();
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    //set param
    GUIFORM_SetType(MMIWT_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#ifndef MMI_PDA_SUPPORT
    GUIFORM_SetStyle(MMIWT_ADD_CITY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif

#ifdef TOUCH_PANEL_SUPPORT    
    //set MMIWT_BOTTON_ADD_CITY_ID
    GUILABEL_SetIcon(MMIWT_BOTTON_ADD_CITY_ID,IMAGE_CLOCK_WORLDCLOCK_ADD_ICON);
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_CLOCK_WORLDCLOCK_ADD_ICON,MMIACC_WCLK_WIN_ID);
//    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    child_width.add_data = image_width+4;
    GUIFORM_SetChildWidth(MMIWT_ADD_CITY_FORM_CTRL_ID,MMIWT_BOTTON_ADD_CITY_ID,&child_width);
    //set MMIWT_LABEL_ADD_CITY_ID
    GUILABEL_SetTextById(MMIWT_LABEL_ADD_CITY_ID,TXT_ACC_ADD_CITY,FALSE);
#endif
    //city1 summer icon
    GUILABEL_SetIcon(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID,IMAGE_CLOCK_WORLDCLOCK_DST);
#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
    MMI_GetLabelTextByLang(TXT_LOCAL, &string);
    GUILABEL_SetText(MMIWT_CITY_1_INFO_LOCAL_ICON_CTRL_ID, &string, FALSE);
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_LOCAL_ICON_CTRL_ID,WCLK_SMALL_FONT,WCLK_WORK_COLOR);
#endif
    //GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_CLOCK_WORLDCLOCK_DST,MMIACC_WCLK_WIN_ID);
    //child_width.add_data = image_width;
    //GUIFORM_SetChildWidth(MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID,MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID,&child_width);

#if defined(ANALOG_CLK_SUPPORT)
    data_info.img_id = IMAGE_CLOCK_WORLDCLOCK_DAYTIME_BG;//IMAGE_IDLE_KEYLOCK_TIME_BG;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_CLOCK_WORLDCLOCK_DAYTIME_BG,MMIACC_WCLK_WIN_ID);
    child_width.add_data = image_width;
#endif
    for(index = 0; index < WT_DEFAULT_CITY_NUM;index ++)
    {
#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
        uint16 space = WCLK_INFO_FORM_SPACE;
        //GUIFORM_SetStyle(MMIWT_CITY_1_INFO_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD, GUIFORM_STYLE_UNIT);
        GUIFORM_SetMargin(MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,0);
        GUIFORM_SetMargin(MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,0);
        GUIFORM_SetMargin(MMIWT_FORM_CTRL_ID,0);
        GUIFORM_SetSpace(MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD, &space, &space);
        GUIFORM_SetSpace(MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID+index*ONE_FROM_TOTAL_CHILD, &space, &space);
        if(0<index)
        {
            child_width.add_data = WCLK_INFO_CITY_WIDTH_PERCENT+WCLK_INFO_LOCAL_WIDTH_PERCENT;
        }
        else
        {
            child_width.add_data = WCLK_INFO_CITY_WIDTH_PERCENT;
        }
        child_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
        GUIFORM_SetChildWidth(
                                MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                MMIWT_CITY_1_INFO_CITY_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                &child_width);

        child_width.add_data = WCLK_INFO_LOCAL_WIDTH_PERCENT;
        GUIFORM_SetChildWidth(
                                MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                MMIWT_CITY_1_INFO_LOCAL_ICON_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                &child_width);

        child_width.add_data = WCLK_INFO_TIME_WIDTH_PERCENT;
        GUIFORM_SetChildWidth(
                                MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                MMIWT_CITY_1_INFO_TIME_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                &child_width);

        child_width.add_data = WCLK_INFO_DATE_WIDTH_PERCENT;
#ifndef MAINLCD_SIZE_128X160  //3行格式
        GUIFORM_SetChildWidth(
                                MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                MMIWT_CITY_1_INFO_DATE_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                &child_width);
#else
        GUIFORM_SetChildWidth(
                                MMIWT_CITY_1_INFO_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                MMIWT_CITY_1_INFO_DATE_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                &child_width);
#endif
        child_width.add_data = WCLK_INFO_SUMMER_WIDTH_PERCENT;
        GUIFORM_SetChildWidth(
                                MMIWT_CITY_1_INFO_CITY_FORM2_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,
                                &child_width);
#else
        #if defined(ANALOG_CLK_SUPPORT)
        control_info.ctrl_id = (uint32)(MMIWT_CITY_1_CLOCK_CTRL_ID+index*ONE_FROM_TOTAL_CHILD);
        GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
        GUIANIM_SetOwnerDraw((uint32)(MMIWT_CITY_1_CLOCK_CTRL_ID+index*ONE_FROM_TOTAL_CHILD),DisplayAnlogClockCallbackFun);        
        GUIFORM_SetChildWidth(MMIWT_CITY_1_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD,(uint32)(MMIWT_CITY_1_CLOCK_CTRL_ID+index*ONE_FROM_TOTAL_CHILD),&child_width);
        #endif
        GUIFORM_PermitChildFont((uint32)(MMIWT_CITY_1_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD),FALSE);
#ifndef MMI_PDA_SUPPORT
        GUIFORM_SetStyle((uint32)(MMIWT_CITY_1_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD),GUIFORM_STYLE_UNIT);
#endif
        GUIFORM_SetMargin((uint32)(MMIWT_CITY_1_INFO_CITY_FORM_CTRL_ID+index*ONE_FROM_TOTAL_CHILD),0);
#endif
    }

    UpdateDisplayCityInfo();
	
#ifndef MMI_PDA_SUPPORT
#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
#ifdef GUI_MID_SOFTKEY_USE_IMG
    GUIWIN_SeSoftkeytButtonIconId(MMIACC_WCLK_WIN_ID, IMAGE_COMMON_SOFTKEY_ADD_ICON, 1, FALSE);
#endif    
    MMK_SetAtvCtrl(MMIACC_WCLK_WIN_ID, MMIWT_CITY_1_INFO_CITY_CTRL_ID); 
#else
#ifdef TOUCH_PANEL_SUPPORT 
    MMK_SetAtvCtrl(MMIACC_WCLK_WIN_ID,MMIWT_ADD_CITY_FORM_CTRL_ID);
#else
    MMK_SetAtvCtrl(MMIACC_WCLK_WIN_ID,MMIWT_FORM_CTRL_ID);
#endif
#endif
#endif

}
 

/*****************************************************************************/
// 	Description : get weekday from mon to sun (1 to 7)
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
// LOCAL int16 GetWeekDayByDate(
//                              int16 year,
//                              int16  month,
//                              int16 day
//                              )
// {
// 	  int16      firstday         = MMICOM_FIRSTDAY_WDAY;
// 	  int16      firstyear        = MMICOM_DATE_MIN_YEAR;
// 	  int16      count            = 0;
// 	  int16      leap_years       = 0;
// 	  uint8      weekday          = 0;
// 	  uint16     total_days       = 0;
// 	  uint8      month_of_days[]  = { 31,28,31,30,31,30,31,31,30,31,30,31 };
// 	  /************************************/
// 	  /* Calculate the days that has past */
// 	  /************************************/
// 	  leap_years = 0;
// 	  total_days = 0;
// 	  for ( count = firstyear;count < year;count ++ ) {
// 	     if ( MMIAPICOM_IsLeapYear( count )) leap_years += 1;
// 	  }
// 	  total_days = ( year - firstyear ) * 365 + leap_years;
// 	  for ( count = 1;count < month;count ++ ) {
// 	     total_days += month_of_days[count - 1];
// 	  }
// 	  total_days += day;
// 	  if ( MMIAPICOM_IsLeapYear( year ) && month > 2 ) total_days += 1;
// 	  /*************************/
// 	  /* Calculate the weekday */
// 	  /*************************/
// 	  weekday = (( total_days - 1 ) % 7 + firstday ) % 7;
// 	  return weekday;
// } /* end GetWeekDayByDate */

/*****************************************************************************/
// 	Description : convert string type of date to unicode type
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/

LOCAL void ConvertDateToShowFormat( 
                                   MMIACC_RTC_DATE_T *current_date, 
                                   MMI_STRING_T *convert_date 
                                   )
{
      uint8   year_char[5];
      uint8   month_char[3];
      uint8   day_char[3];
      int16   year_int = 0, month_int = 0, day_int = 0;
      uint8   date_str[MMISET_DATE_STR_LEN] = {0};
	  MMISET_DATE_DISPLAY_TYPE_E date_display_type = MMISET_DATE_YMD;//CR211588
      
	  /**********************************************/
	  /* Convert the date to integers for month&day */
	  /**********************************************/
	  MMI_MEMCPY( &year_char[0],  5, 
	            *current_date, RTC_DATE_STRING_LENGTH,
	            4 );
	  MMI_MEMCPY( &month_char[0], 3, 
	            *current_date + 5, RTC_DATE_STRING_LENGTH-5,
	            2 );
	  MMI_MEMCPY( &day_char[0],   3,
	            *current_date + 8, RTC_DATE_STRING_LENGTH-8,
	            2 );
	  year_char[4]  = '\0';
	  month_char[2] = '\0';
	  day_char[2]   = '\0';
	  year_int  = atoi( (char*)&year_char[0]  );
	  month_int = atoi( (char*)&month_char[0] );
	  day_int   = atoi( (char*)&day_char[0]   );

	  /*************************/
	  /* Calculate the weekday */
	  /*************************/
	  //no need weekday now murphy
        //weekday = GetWeekDayByDate( year_int, month_int, day_int );
	  date_display_type = MMIAPISET_GetDateDisplayType();//CR211588
	  if(MMISET_DATE_YMD == date_display_type)
	  {
#ifdef MMI_GUI_STYLE_MINISCREEN 
		  sprintf((char*)date_str,"%.4d-%.2d-%.2d", year_int,month_int, day_int);
#else
		  MMIAPISET_FormatDateStrByDateStyle((uint16)year_int,month_int,day_int,'.',date_str,MMISET_DATE_STR_LEN);
#endif
	  }
	  else if (MMISET_DATE_MDY == date_display_type)
	  {
#ifdef MMI_GUI_STYLE_MINISCREEN 
		  sprintf((char*)date_str,"%.2d-%.2d-%.4d",month_int,day_int,year_int);
#else
		  MMIAPISET_FormatDateStrByDateStyle((uint16)month_int,day_int,year_int,'.',date_str,MMISET_DATE_STR_LEN);
#endif
	  }
	  else if (MMISET_DATE_DMY == date_display_type)
	  {
#ifdef MMI_GUI_STYLE_MINISCREEN 
		  sprintf((char*)date_str,"%.2d-%.2d-%.4d", day_int,month_int,year_int);
#else
		  MMIAPISET_FormatDateStrByDateStyle((uint16)day_int,month_int,year_int,'.',date_str,MMISET_DATE_STR_LEN);
#endif
	  }

    MMI_STRNTOWSTR( &convert_date->wstr_ptr[0], MMISET_DATE_STR_LEN,date_str,MMISET_DATE_STR_LEN,MMISET_DATE_STR_LEN );
    convert_date->wstr_len = strlen((char*)date_str);
}

 LOCAL void AddNewCityInfoToRecord(MMI_TEXT_ID_T text_id)
 {
     int32                 city_count= 0;
     int32                 find_index = -1;
     
    for(city_count=0; city_count < WT_DEFAULT_CITY_NUM;city_count++)
    {
        if(s_default_city_info[city_count].city_text_id == TXT_NULL )
        {
            find_index = city_count;
            break;
        }
        else if(s_default_city_info[city_count].city_text_id== text_id)
        {
            //已经存在 提示无法重复添加
            MMIPUB_OpenAlertWarningWin(TXT_ACC_CITY_HAS_EXIST);
            break;
        }

    }

    if(find_index >= 0 && find_index < WT_DEFAULT_CITY_NUM)
    {
        s_default_city_info[find_index].city_text_id = text_id;
        //添加成功 
    }
    else
    {
         //prompt 已满 无法添加
         MMIPUB_OpenAlertWarningWin(TXT_ACC_CITY_LIST_IS_FULL);
         
    }
 }

/*****************************************************************************/
// 	Description : init world clock application
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL void UpdateDisplayCityInfo(void)
{
    int32                 city_count= 0;

    //count the first local city info
    DisplayLocalCityInfo();

    //count other city info and display
    for(city_count=1; city_count<WT_DEFAULT_CITY_NUM;city_count++)
    {
#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
        if(s_default_city_info[city_count].city_text_id != TXT_NULL)
        {
            GUIFORM_SetChildDisplay(MMIWT_FORM_CTRL_ID,MMIWT_CITY_1_INFO_FORM_CTRL_ID+ city_count*ONE_FROM_TOTAL_CHILD, GUIFORM_CHILD_DISP_NORMAL);
            DisplayotherCityInfo(s_default_city_info[city_count].city_text_id,city_count);
        }
        else
        {
            GUIFORM_SetChildDisplay(MMIWT_FORM_CTRL_ID,MMIWT_CITY_1_INFO_FORM_CTRL_ID+ city_count*ONE_FROM_TOTAL_CHILD, GUIFORM_CHILD_DISP_HIDE);
        }
#else
        if(s_default_city_info[city_count].city_text_id != TXT_NULL)
        {
            GUIFORM_SetChildDisplay(MMIWT_FORM_CTRL_ID,(uint32)(MMIWT_CITY_1_FORM_CTRL_ID+ city_count*ONE_FROM_TOTAL_CHILD), GUIFORM_CHILD_DISP_NORMAL);
            DisplayotherCityInfo(s_default_city_info[city_count].city_text_id,city_count);
        }
        else
        {
            GUIFORM_SetChildDisplay(MMIWT_FORM_CTRL_ID,(uint32)(MMIWT_CITY_1_FORM_CTRL_ID+ city_count*ONE_FROM_TOTAL_CHILD), GUIFORM_CHILD_DISP_HIDE);
        }
#endif
    }
    
}

/*****************************************************************************/
// 	Description :caculate current date and time by local date time
//	Global resource dependence : none
//  Author: murphy.xie
//	Note: 
/*****************************************************************************/
LOCAL void CalculateCityTime(
                             uint8 operate_info,
                             MMIACC_RTC_DATE_T* work_date,
                             MMIACC_RTC_TIME_T* work_time,
                             MMI_TEXT_ID_T dest_city_index
                             )
{
    MMIACC_RTC_DATE_T        *l_date = PNULL;
    MMIACC_RTC_TIME_T        *l_time = PNULL;
    int8        local_hour_char[3];
    int16          local_hour_int;
    int8        local_min_char[3];
    int16          local_min_int;
    int8        local_day_char[3];
    int16          local_day_int;
    int8        local_month_char[3];
    int16          local_month_int;
    int8        local_year_char[5];
    int16          leap_year_flg;
    int16          local_year_int;
    int16          world_hour_int;
    int16          world_min_int;
    int16          world_day_int;
    int16          world_month_int;
    int16          world_year_int;
    int16          time_diff_tmp;
    int16		  local_total_min;
    int16 		  local_day_diff = 0;

    /****************************/
    /* Read the local date&time */
    /****************************/
    l_date = MMIAPICOM_GetRTCDate();
    l_time = MMIAPICOM_GetRTCTime();

    /*****************************************/
    /* Convert the time to integers for hour */
    /*****************************************/
    MMI_MEMCPY( &local_hour_char[0], 2,&(*l_time)[0], 2 ,2);
    local_hour_char[2] = '\0';
    local_hour_int = atoi((char*) &local_hour_char[0] );

    /*****************************************/
    /* Convert the time to integers for min */
    /*****************************************/
    MMI_MEMCPY( &local_min_char[0], 2,&(*l_time)[3], 2 ,2);
    local_min_char[2] = '\0';
    local_min_int = atoi((char*) &local_min_char[0] );

    /**************/
    /* Check time */
    /**************/
    if (( local_hour_int < 0 ) || ( local_hour_int > 23 )) local_hour_int = 0;
    /***************************************************/
    /* Convert the date to integers for year&month&day */
    /***************************************************/
    MMI_MEMCPY( &local_year_char[0], 4,  &(*l_date)[0], 4, 4 );
    local_year_char[4] = '\0';
    local_year_int = atoi((char*) &local_year_char[0] );
    MMI_MEMCPY( local_month_char, 2, &(*l_date)[5], 2, 2 );
    local_month_char[2] = '\0';
    local_month_int = atoi((char*) local_month_char );
    MMI_MEMCPY( local_day_char, 2,  &(*l_date)[8], 2,2 );
    local_day_char[2] = '\0';
    local_day_int = atoi((char*) local_day_char );
    /*******************/
    /* Check leap year */
    /*******************/
    leap_year_flg = 0;
    if ( !( local_year_int % 4 ) && ( local_year_int % 100 ) ||
         !( local_year_int % 400 )) {
        leap_year_flg = 1;
    }
    /*********************************/
    /* Calculate the time difference */
    /*********************************/
    time_diff_tmp  = GetZoneMinByZoneTextID(GetZoneTextIDByCityTextID(dest_city_index)) - GetZoneMinByZoneTextID(GetZoneTextIDByCityTextID(s_default_city_info[0].city_text_id));

    /*****************************/
    /* Calculate the world time  */
    /*****************************/
    switch( operate_info ){
        case 0:  /* View worldclock */
            local_total_min = (local_hour_int - (uint8)s_default_city_info[0].summer_flag + GetSummerFlagByCityTextID(dest_city_index))*60 + time_diff_tmp+local_min_int;
            if(local_total_min < 0)
            local_day_diff = -1;
            else if(local_total_min > (24*60))
            local_day_diff = 1;
            else
            local_day_diff = 0;
            local_total_min = (local_total_min +24*60)%(24*60);
            world_hour_int = local_total_min/60;
            world_min_int  = local_total_min%60;
            break;
        case 1:  /* Set DST */
            world_hour_int = local_hour_int + 1;
            world_min_int  = local_min_int;
            if( world_hour_int > 23 )
            {
                world_hour_int -= 24;
                local_day_diff = +1;
            }

            break;
        case 2:  /* Remove DST */
        default:
            world_hour_int = local_hour_int - 1;
            world_min_int  = local_min_int;

            if ( world_hour_int < 0 )
            {
                world_hour_int += 24;
                local_day_diff = -1;
            }

            break;
            }

    world_day_int = local_day_int + local_day_diff;
    /****************************/
    /* Calculate the world date */
    /****************************/
    world_month_int = local_month_int;
    world_year_int  = local_year_int;
    switch( local_month_int ){
        case 1:                                            /* Jan */
            if( world_day_int == 0 )
            {
                world_day_int   = 31;
                world_month_int = 12;
                world_year_int  = (MMICOM_DATE_MIN_YEAR == world_year_int)? MMICOM_DATE_MAX_YEAR : local_year_int - 1;//@zhaohui,cr108981 
            }
            else if ( world_day_int > 31 )
            {
                world_day_int   = 1;
                world_month_int = local_month_int + 1;
            }
            break;
        case 2:                                            /* Feb */
            if ( world_day_int == 0 )
            {
                world_day_int   = 31;
                world_month_int = local_month_int - 1;
            }
            else if ( leap_year_flg )
            {
                if ( world_day_int > 29 )
                {
                    world_day_int   = 1;
                    world_month_int = local_month_int + 1;
                }
            }
            else
            {
                if ( world_day_int > 28 )
                {
                    world_day_int   = 1;
                    world_month_int = local_month_int + 1;
                }
            }
            break;
        case 3:                                            /* Mar */
            if ( world_day_int == 0 )
            {
                if( leap_year_flg )
                {
                world_day_int = 29;
                }
                else
                {
                world_day_int = 28;
                }
                world_month_int = local_month_int - 1;
            } else if ( world_day_int > 31 ) {
                world_day_int   = 1;
                world_month_int = local_month_int + 1;
            }
            break;
        case 5:                                            /* May */
        case 7:                                            /* Jul */
        case 10:                                           /* Oct */
            if ( world_day_int == 0 ) {
                world_day_int   = 30;
                world_month_int = local_month_int - 1;
            } else if ( world_day_int > 31 ) {
                world_day_int   = 1;
                world_month_int = local_month_int + 1;
            }
            break;
        case 8:                                            /* Aug */
            if ( world_day_int == 0 ) {
                world_day_int   = 31;
                world_month_int = local_month_int - 1;
            } else if ( world_day_int > 31 ) {
                world_day_int   = 1;
                world_month_int = local_month_int + 1;
            }
            break;
        case 4:                                            /* Apr */
        case 6:                                            /* Jun */
        case 9:                                            /* Sep */
        case 11:                                           /* Nov */
            if ( world_day_int == 0 ) {
                world_day_int   = 31;
                world_month_int = local_month_int - 1;
            } else if ( world_day_int > 30 ) {
                world_day_int   = 1;
                world_month_int = local_month_int + 1;
            }
            break;
        case 12:                                           /* Dec */
        default:
            if ( world_day_int == 0 ) {
                world_day_int   = 30;
                world_month_int = local_month_int - 1;
            } else if ( world_day_int > 31 ) {
                world_day_int   = 1;
                world_month_int = 1;
                world_year_int  = (MMICOM_DATE_MAX_YEAR == world_year_int)? MMICOM_DATE_MIN_YEAR : local_year_int + 1;//@zhaohui,cr108981
            }
            break;
        }

    /*************************/
    /* Output the world time */
    /*************************/
    sprintf(( char * )work_time,"%02d%c%02d%c%c%c",world_hour_int,(*l_time)[2],world_min_int,(*l_time)[5],(*l_time)[6],(*l_time)[7] );

    /*************************/
    /* Output the world date */
    /*************************/
    sprintf(( char * )work_date,"%04d-%02d-%02d",world_year_int,world_month_int,world_day_int );

    return;
}

/*****************************************************************************/
// 	Description : display local city information such as time, date, time difference etc.
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void DisplayLocalCityInfo(void)
{
	wchar            			wdate_temp[20] = {0};
	MMI_STRING_T     			world_time     = {0};
    wchar                       time_temp1[10] = {0};
    wchar                       time_temp2[6]  = {0};
	MMI_STRING_T     			world_date     = {0};
	char 						time_buf[MMISET_TIME_STR_12HOURS_LEN+1]={0};
	GUI_LCD_DEV_INFO			lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMIACC_RTC_DATE_T*			init_date = NULL;
    MMIACC_RTC_TIME_T*			init_time= NULL;
	MMISET_TIME_DISPLAY_TYPE_E  time_type       = MMISET_TIME_12HOURS;

	time_type = MMIAPISET_GetTimeDisplayType();
    MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);

	//和当前date time同步更新
	init_date = MMIAPICOM_GetRTCDate();  
	init_time = MMIAPICOM_GetRTCTime();  
    //计算夏日制的时间,如果开启的话
#if 0
#if defined(MMI_UPDATE_TIME_SUPPORT)
   // SCI_TRACE_LOW("MMIAUT::MMIAPIWCLK_SetTimeZone mcc= %d,time_zone = %d, current_year =%d, dst_value=%d");
    if (MMIAPISET_GetIsAutoUpdateTime() &&s_is_summer_flag_nitz_dst)  
    {
        if (s_default_city_info[0].summer_flag)
        {
            CalculateCityTime(1,&s_date,&s_time,s_default_city_info[0].city_text_id);
            init_time = &s_time;
            init_date = &s_date;
            MMIACC_SetRTCDateTimeByString(&s_date, &s_time);
             //立即更新状态栏时间
            MAIN_SetStbDispTime();
            MMIIDLE_SendMsgToIdleWin( MSG_IDLE_UPDATE_DATETIME, PNULL);
            s_is_summer_flag_nitz_dst = FALSE;
           // SCI_TRACE_LOW("MMIAUT::MMIAPICOM_GetRTCTime middle  init_date= %d,init_time = %d,init_date, init_time");            
        }
    }
#endif  
#endif  

	if (MMISET_TIME_12HOURS == time_type) 
	{
		uint8 hour = (init_time[0][0] -'0')*10+(init_time[0][1]-'0');
		uint8 min = (init_time[0][3] -'0')*10+(init_time[0][4]-'0');
        
		MMIAPISET_FormatTimeStrByTime(hour, min, (uint8*)time_buf, MMISET_TIME_STR_12HOURS_LEN+1);
		MMI_STRNTOWSTR(time_temp1,10,(const uint8*)time_buf,MMISET_TIME_STR_12HOURS_LEN+1, MMISET_TIME_STR_12HOURS_LEN+1);
		world_time.wstr_ptr = time_temp1;
		world_time.wstr_len = 10;
	}
	else
	{
		MMI_STRNTOWSTR(time_temp2,6,(const uint8*)init_time,5,5);
		world_time.wstr_ptr = time_temp2;
		world_time.wstr_len = 6;
	}


    /*********************/  	
    /* Save current data */  	
    /*********************/  		

	world_date.wstr_ptr = wdate_temp;
	ConvertDateToShowFormat( init_date, &world_date );
    SCI_TRACE_LOW("DisplayLocalCityInfo local city id = %d",s_default_city_info[0].city_text_id);

    GUILABEL_SetTextById(MMIWT_CITY_1_INFO_CITY_CTRL_ID,s_default_city_info[0].city_text_id,TRUE);
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_CITY_CTRL_ID,WCLK_SMALL_FONT,WCLK_GRAY_COLOR);
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID,WCLK_SMALL_FONT,WCLK_GRAY_COLOR);
    GUILABEL_SetMargin(MMIWT_CITY_1_INFO_CITY_CTRL_ID,0);
    GUILABEL_SetMargin(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID,0);


    GUILABEL_SetText(MMIWT_CITY_1_INFO_TIME_CTRL_ID, &world_time, TRUE);
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_TIME_CTRL_ID,WCLK_BIG_FONT,WCLK_WORK_COLOR);
    
    GUILABEL_SetText(MMIWT_CITY_1_INFO_DATE_CTRL_ID, &world_date, TRUE);
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_DATE_CTRL_ID,WCLK_SMALL_FONT,WCLK_GRAY_COLOR);

#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_LOCAL_ICON_CTRL_ID,WCLK_SMALL_FONT,WCLK_WORK_COLOR);
#endif
	// 显示时间和日期，以及时候夏令时
	if (s_default_city_info[0].summer_flag)
	{
        GUILABEL_SetVisible(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID,TRUE,FALSE);
	}
	else 
	{
        GUILABEL_SetVisible(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID,FALSE,FALSE);
	}
    
}

/*****************************************************************************/
// 	Description : display current city information such as time difference, time, date etc.
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void DisplayotherCityInfo(MMI_TEXT_ID_T dest_city_index,uint8 displayformindex)
{
	wchar            wdate_temp[20] = {0};
	MMI_STRING_T     world_time     = {0};
	MMI_STRING_T     world_date     = {0};
    wchar            time_temp1[10] = {0};
    wchar            time_temp2[6]  = {0};
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

	MMISET_TIME_DISPLAY_TYPE_E  time_type       = MMISET_TIME_12HOURS;
	time_type = MMIAPISET_GetTimeDisplayType();
	MMIAPICOM_GetLcdDevInfo(&lcd_dev_info);
    //set current time info
    CalculateCityTime( 0,&s_date,&s_time,dest_city_index);
	if (MMISET_TIME_12HOURS == time_type) //modify by mary
	{
		uint8 hour = (s_time[0] -'0')*10+(s_time[1]-'0');
		uint8 min = (s_time[3] -'0')*10+(s_time[4]-'0');

		memset(s_time, 0, MMISET_TIME_STR_12HOURS_LEN+1);

		MMIAPISET_FormatTimeStrByTime(hour, min, s_time, MMISET_TIME_STR_12HOURS_LEN+1);
		MMI_STRNTOWSTR(time_temp1,10,(const uint8*)s_time,MMISET_TIME_STR_12HOURS_LEN+1, MMISET_TIME_STR_12HOURS_LEN+1);
		world_time.wstr_ptr = time_temp1;
		world_time.wstr_len = 10;
	}
	else
	{
		MMI_STRNTOWSTR(time_temp2,6,(const uint8*)s_time,5,5);
		world_time.wstr_ptr = time_temp2;
		world_time.wstr_len = 6;
	}

	world_date.wstr_ptr = wdate_temp;
	ConvertDateToShowFormat(&(s_date), &world_date );

    SCI_TRACE_LOW("DisplayotherCityInfo dest city id = %d",dest_city_index);
    GUILABEL_SetTextById(MMIWT_CITY_1_INFO_CITY_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex,dest_city_index,FALSE);
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_CITY_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex,WCLK_SMALL_FONT,WCLK_GRAY_COLOR);
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex,WCLK_SMALL_FONT,WCLK_GRAY_COLOR);
    GUILABEL_SetMargin(MMIWT_CITY_1_INFO_CITY_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex,0);


    GUILABEL_SetText(MMIWT_CITY_1_INFO_TIME_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex, &world_time, FALSE);
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_TIME_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex, WCLK_BIG_FONT, WCLK_WORK_COLOR);

    
    GUILABEL_SetText(MMIWT_CITY_1_INFO_DATE_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex, &world_date, FALSE);
    GUILABEL_SetFont(MMIWT_CITY_1_INFO_DATE_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex,WCLK_SMALL_FONT,WCLK_GRAY_COLOR);

	// 显示时间、日期、夏令时

    GUILABEL_SetIcon(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex,IMAGE_CLOCK_WORLDCLOCK_DST);
	if (GetSummerFlagByCityTextID(dest_city_index))
	{
        GUILABEL_SetVisible(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex,TRUE,FALSE);
	}
	else 
	{
        GUILABEL_SetVisible(MMIWT_CITY_1_INFO_SUMMER_ICON_CTRL_ID+ONE_FROM_TOTAL_CHILD*displayformindex,FALSE,FALSE);
	}
}

/*****************************************************************************/
// 	Description : SetWorldClockToLocal
//         
//	Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN: 
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN SetWorldClockToLocal(MMI_TEXT_ID_T text_id)
{
    int32   i = 0;
	WT_CITY_RECORD_INFO pre_local_city = {0};
	int32                 city_count= 0;

    SCI_TRACE_LOW("SetWorldClockToLocal text_id =%d",text_id);
    if(TXT_NULL == text_id)
    {
        return FALSE;
    }
    //修改手机时间
    if (s_default_city_info[0].city_text_id == text_id)
    {
        return FALSE;
    }
#if defined(MMI_UPDATE_TIME_SUPPORT)
    if (!MMIAPISET_GetIsAutoUpdateTime())
#endif
    {
        CalculateCityTime( 0,&s_date,&s_time,text_id);
        MMIACC_SetRTCDateTimeByString(&s_date, &s_time);
    }
    //立即更新状态栏时间
    MAIN_SetStbDispTime();
    //clear other city info is the same to be set local city index
    for(i = 1; i < WT_DEFAULT_CITY_NUM; i++)
    {
        if(s_default_city_info[i].city_text_id == text_id)
        {
            s_default_city_info[i].city_text_id = TXT_NULL;
            break;
        }
    }

    //set to local
    pre_local_city.city_text_id = s_default_city_info[0].city_text_id ;
    pre_local_city.summer_flag  = s_default_city_info[0].summer_flag ;
    s_default_city_info[0].city_text_id = text_id;
    if(WT_DEFAULT_CITY_NUM > i)
    {
        s_default_city_info[0].summer_flag = s_default_city_info[i].summer_flag;
    }
    else
    {
        s_default_city_info[0].summer_flag = FALSE;
    }
    for(city_count=0; city_count < WT_DEFAULT_CITY_NUM;city_count++)
    {
        if(s_default_city_info[city_count].city_text_id == TXT_NULL )
        {

            break;
        }
    }
    if (city_count < WT_DEFAULT_CITY_NUM)
    {
        AddNewCityInfoToRecord(pre_local_city.city_text_id);
        s_default_city_info[city_count].summer_flag = pre_local_city.summer_flag; 
    }
    return TRUE;
}

LOCAL BOOLEAN SavaCityInfoToNV(void)
{
    //save s_default_city_info to nv
    MMINV_WRITE(MMINV_ACC_WORLDCLOCK_CITY_INFO, (void *)s_default_city_info);

    return TRUE;
}


/*****************************************************************************/
// Description : the process message function of the accessory menu
// Global resource dependence : 
// Author:        murphy.xie
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWclkCityListOrderWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E                result      = MMI_RESULT_TRUE;
    int16                       list_index   = -1;

    //SCI_TRACE_LOW:"HandleWclkCityListOrderWinMsg, msg_id = 0x%02x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_1683_112_2_18_2_2_30_112,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {    
    case MSG_OPEN_WINDOW: 
        //add option
        GUILIST_SetMaxItem(MMIACC_WCLK_ORDER_LIST_CTRL_ID, 2, FALSE );
        MMIAPISET_AppendListItemByTextId( TXT_ACC_WORLD_CITY_LIST_ORDER_BY_GMT, TXT_COMMON_OK, MMIACC_WCLK_ORDER_LIST_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId( TXT_ACC_WORLD_CITY_LIST_ORDER_BY_ALPHABET, TXT_COMMON_OK, MMIACC_WCLK_ORDER_LIST_CTRL_ID, GUIITEM_STYLE_ONE_LINE_RADIO);
        GUILIST_SetSelectedItem(MMIACC_WCLK_ORDER_LIST_CTRL_ID, s_city_list_order, TRUE);
        GUILIST_SetCurItemIndex(MMIACC_WCLK_ORDER_LIST_CTRL_ID,s_city_list_order);    

        MMK_SetAtvCtrl(win_id, MMIACC_WCLK_ORDER_LIST_CTRL_ID);
      
        break;
    case MSG_CTL_MIDSK:  
    case MSG_KEYDOWN_WEB:
    case MSG_APP_OK:        
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            list_index = GUILIST_GetCurItemIndex(MMIACC_WCLK_ORDER_LIST_CTRL_ID);
            if(s_city_list_order != list_index)
            {
                s_city_list_order = (WT_CITY_ORDER_E)list_index;
                InitCityOrderBySort();
            }
            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}


/*****************************************************************************/
// Description : the process message function of the accessory menu
// Global resource dependence : 
// Author:        murphy.xie
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWclkCityListOptionWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E                result      = MMI_RESULT_TRUE;

    //SCI_TRACE_LOW:"HandleWclkCityListOptionWinMsg, msg_id = 0x%02x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_1683_112_2_18_2_2_30_112,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {    
    case MSG_OPEN_WINDOW: 
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:
        {
            MMI_MENU_ID_T           menu_id			= 0;
            MMI_MENU_GROUP_ID_T     group_id       = 0;

            GUIMENU_GetId(MMIACC_WCLK_ADD_CITY_OPTION_CTRL_ID, &group_id, &menu_id);
            if (ID_ACC_WCLK_CITY_LIST_ORDER == menu_id)
            {
                MMK_CreateWin((uint32 *)WCLK_CITY_LIST_OPTION_ORDER_WIN_TAB,PNULL);
                MMK_CloseWin( win_id );
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// Description : AppendCityListBoxItem
// Global resource dependence : 
// Author:        murphy.xie
/*****************************************************************************/
void AppendCityListBoxItem(void)
{
    GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint32 index = 0;
    item_t.item_style = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
    
    GUILIST_RemoveAllItems(MMIWT_ADD_CITY_LIST_CTRL_ID);

    GUILIST_SetMaxItem(MMIWT_ADD_CITY_LIST_CTRL_ID, WT_TOTAL_CITY_NUM, FALSE );

    for (index = 0; index < WT_TOTAL_CITY_NUM; index++)
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
            
        item_data.item_content[0].item_data.text_id = s_zone_city_info[s_city_real_order[index]].city_index;

        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[1].item_data.text_id = s_zone_city_info[s_city_real_order[index]].zone_index;
        GUILIST_AppendItem( MMIWT_ADD_CITY_LIST_CTRL_ID, &item_t );
    }
}

/*****************************************************************************/
// Description : the process message function of the accessory menu
// Global resource dependence : 
// Author:        murphy.xie
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWclkAddCityWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E                result      = MMI_RESULT_TRUE;
    uint16                       list_index   = 0;
    MMI_MENU_ID_T               menu_id = (MMI_MENU_ID_T)MMK_GetWinAddDataPtr(win_id);
    static   WT_CITY_ORDER_E    old_order = CITY_ORDER_BY_GMT;

    //SCI_TRACE_LOW:"HandleWclkAddCityWinMsg, msg_id = 0x%02x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_1683_112_2_18_2_2_30_112,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {    
    case MSG_OPEN_WINDOW: 
        //add city info
        AppendCityListBoxItem();
#ifdef GUI_MID_SOFTKEY_USE_IMG
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_OK_ICON, 1, FALSE);
#endif        
        if(ID_ACC_WCLK_SET_LOCAL1 == menu_id)
        {
            //set win title
            GUIWIN_SetTitleTextId(win_id,TXT_ACC_SET_LOCAL,FALSE);
        }
        
        MMK_SetAtvCtrl(win_id, MMIWT_ADD_CITY_LIST_CTRL_ID);
      
        break;
    case MSG_GET_FOCUS: 
        if(old_order != s_city_list_order)
        {
            AppendCityListBoxItem();
            old_order = s_city_list_order;
        }
        break;
		
    
    case MSG_APP_MENU://record the focus 
    case MSG_APP_OK:   
    case MSG_CTL_OK:
       {
            old_order = s_city_list_order;
            MMK_CreateWin((uint32 *)WCLK_CITY_LIST_OPTION_WIN_TAB,PNULL);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527  
    case MSG_CTL_MIDSK:  
    case MSG_KEYDOWN_WEB:
        {
            GUILIST_ITEM_T *            list_item_ptr      = PNULL;
            MMI_TEXT_ID_T               select_text_id = TXT_NULL;

            list_index = GUILIST_GetCurItemIndex(MMIWT_ADD_CITY_LIST_CTRL_ID); 
            list_item_ptr = GUILIST_GetItemPtrByIndex(MMIWT_ADD_CITY_LIST_CTRL_ID, list_index);  /*lint !e605*/
            if(PNULL == list_item_ptr)
            {
                break;
            }
            
            select_text_id = list_item_ptr->item_data_ptr->item_content[0].item_data.text_id;
            if(ID_ACC_WCLK_SET_LOCAL1 == menu_id)
            {
                SetWorldClockToLocal(select_text_id);  
            }
            else
            {
                AddNewCityInfoToRecord(select_text_id);
            }
            
            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// Description : the process message function of the accessory menu
// Global resource dependence : 
// Author:        taul.zhu
// Note:   
// Revisor: xiaoqing.lu
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleWclkWinMsg( 
									MMI_WIN_ID_T win_id, 
									MMI_MESSAGE_ID_E msg_id, 
									DPARAM param
									)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;

	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:
            SetWCFormParam();  
            InitCityOrderBySort();
            break;

		case MSG_FULL_PAINT:
            UpdateDisplayCityInfo();         
			break;
	    case MSG_LCD_SWITCH :
            SetWCFormParam();
            break;

        case MSG_KEYDOWN_LEFT:
        case MSG_KEYREPEAT_LEFT:
            if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
            {
                GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID,  GUITAB_SWITCH_PRE);
            }
            break;
        
        case MSG_KEYDOWN_RIGHT:
        case MSG_KEYREPEAT_RIGHT:
            if(GUITAB_GetItemNum(MMI_TIMER_PARENT_TAB_ID)>1)
            {
                GUITAB_SwitchToNextItem(MMI_TIMER_PARENT_TAB_ID, GUITAB_SWITCH_NEXT);
            }
            break;

        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
        #if defined(MMI_UPDATE_TIME_SUPPORT)
            if(MMISET_SET_UPDATE_TIME_WIN_ID == MMK_GetPrevWinId(win_id))
            {
                MMIPUB_OpenAlertSuccessWin(TXT_OPEN_AUTO_UPDATE_TIME_PROMPT);
            }
        #endif
            if(!MMK_IsChildWin(win_id))
            {
                MMK_CloseWin(win_id);
            }
            else
            {
                MMK_CloseParentWin(MMI_TIMER_PARENT_WIN_ID);
            }
            break;
            
#ifdef MMI_PDA_SUPPORT
		case MSG_LOSE_FOCUS:
            GUIWIN_SetTitleButtonState(MMIACC_WCLK_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
			break;
		case MSG_GET_FOCUS:
            GUIWIN_SetTitleButtonState(MMIACC_WCLK_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            break;

#endif
        case MSG_APP_WEB:
        case MSG_NOTIFY_MIDSK:
#ifndef MMI_WORLDCLOCK_MINI_SUPPORT
            if(MMIWT_BOTTON_ADD_CITY_ID == MMK_GetActiveCtrlId(win_id))
#endif
            {

                MMK_CreateWin((uint32 *)WCLK_ADD_CITY_WIN_TAB,PNULL);
            }
            break;
		case MSG_KEYDOWN_OK:
        case MSG_CTL_OK:
    	case MSG_APP_MENU:
            MMK_CreateWin( (uint32 *)WCLK_OPTION1_WIN_TAB, PNULL);

			break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:
		case MSG_TP_PRESS_DOWN:
            {
                GUI_RECT_T  form_rect = {0};
                GUI_POINT_T point = {0};

                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);
                GUIAPICTRL_GetRect(MMIWT_ADD_CITY_FORM_CTRL_ID,&form_rect);/*lint !e746*/
                if(GUI_PointIsInRect(point,form_rect))
                {
                    MMK_CreateWin((uint32 *)WCLK_ADD_CITY_WIN_TAB,PNULL);
                }
                
                break;
    	    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_CLOSE_WINDOW:
            SavaCityInfoToNV();    
            break;
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}

	return recode;
}



/*****************************************************************************/
//  Description :append HandleWclkOptionDeleteCityWinMsg item
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void AppendAllDisplayCitys(
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    int16      i = 0;
    int32      item_count = 0;
    int16      start = 0;

	 //set list max item and selected max item
    GUILIST_SetMaxItem(ctrl_id,WT_DEFAULT_CITY_NUM,FALSE);
    GUILIST_SetMaxSelectedItem(ctrl_id,WT_DEFAULT_CITY_NUM);
	GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
    GUILIST_ChangeCheckListSoftkeyIndex(ctrl_id, MIDDLE_BUTTON);

#if defined(MMI_UPDATE_TIME_SUPPORT)
    if (MMIAPISET_GetIsAutoUpdateTime() )
    {
        //start from 1 not 0
        start++;
    }
#endif

/*#ifdef MMI_WORLDCLOCK_MINI_SUPPORT*/  //解决其他工程设置夏令时左软件是select/cancel的问题
    GUILIST_ChangeCheckListSoftkeyIndex(ctrl_id, 1); //set middle softkey to display the Select/Cancel state;
    {
        GUILIST_ITEM_T      item_t = {0};
        GUILIST_ITEM_DATA_T item_data = {0};

        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
        item_t.item_data_ptr = &item_data;

        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = STXT_SELECT;
        item_data.softkey_id[2] = STXT_RETURN;
        for(i = start; i<WT_DEFAULT_CITY_NUM;i++)
        {
            if(s_default_city_info[i].city_text_id != TXT_NULL)
            {
                item_data.item_content[0].item_data.text_id = s_default_city_info[i].city_text_id;
                GUILIST_AppendItem(ctrl_id, &item_t);
                if(s_default_city_info[i].summer_flag)
                {
                    GUILIST_SetSelectedItem(ctrl_id,item_count,TRUE);
                }
                item_count++;
            }
        }
    }
// #else
//     //append item
//     for (i = start; i<WT_DEFAULT_CITY_NUM;i++)
//     {
//         if(s_default_city_info[i].city_text_id != TXT_NULL)
//         {
//             MMIAPISET_AppendListItemByTextId(s_default_city_info[i].city_text_id ,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
//             if(s_default_city_info[i].summer_flag)
//             {
//                 GUILIST_SetSelectedItem(ctrl_id,item_count,TRUE);
//             }
//             item_count++;
//         }
//     }
// #endif
}

/*****************************************************************************/
//  Description :CheckAfterDeleteCitysAction
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void CheckAfterSetDSTAction(
                        MMI_CTRL_ID_T   ctrl_id
                        )
{
    uint16      selected_num = 0;
    uint16      selected_item[WT_DEFAULT_CITY_NUM] = {0};
    int32       i = 0;
    int16       start = 0;
    int32       j = 0;
    GUILIST_ITEM_T*   list_item_ptr      = PNULL;
    MMI_TEXT_ID_T       select_text_id = TXT_NULL;
#if defined(MMI_UPDATE_TIME_SUPPORT)
    if (MMIAPISET_GetIsAutoUpdateTime() )
    {
        //start from 1 not 0
        start++;
    }
#endif    
    //get current selected items
    selected_num = GUILIST_GetSelectedItemIndex(ctrl_id,selected_item,WT_DEFAULT_CITY_NUM);
    for(i=start; i < WT_DEFAULT_CITY_NUM;i++)
    {
        //reset summer flag
        s_default_city_info[i].summer_flag = FALSE;

        for (j=0; j < selected_num;j++)
        {
            list_item_ptr = GUILIST_GetItemPtrByIndex(MMIACC_WCLK_OPTION_SET_DST_LIST_CTRL_ID, selected_item[j]);  /*lint !e605*/
            if(PNULL == list_item_ptr )
                break;

            select_text_id = list_item_ptr->item_data_ptr->item_content[0].item_data.text_id;

            if(select_text_id == s_default_city_info[i].city_text_id)
            {
                s_default_city_info[i].summer_flag  = TRUE;
            } 
      }
   }
}
/*****************************************************************************/
//  Description : HandleWclkOptionDeleteCityWinMsg
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWclkSetDSTWinMsg(
                                             MMI_WIN_ID_T       win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIACC_WCLK_OPTION_SET_DST_LIST_CTRL_ID;
    static BOOLEAN      local_summer_flag =0; 

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendAllDisplayCitys(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        local_summer_flag =s_default_city_info[0].summer_flag; 
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
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
                CheckAfterSetDSTAction(MMIACC_WCLK_OPTION_SET_DST_LIST_CTRL_ID);
                //改变手机的时间
                if(local_summer_flag != s_default_city_info[0].summer_flag)
                {
                    //设置夏日时
                    if(!local_summer_flag && s_default_city_info[0].summer_flag)
                    {
                        CalculateCityTime( 1,&s_date,&s_time,s_default_city_info[0].city_text_id);
                    }
                    else//去掉夏日时 
                    {
                        CalculateCityTime( 2,&s_date,&s_time,s_default_city_info[0].city_text_id);
                    }

                    MMIACC_SetRTCDateTimeByString(&s_date, &s_time);
                    //立即更新状态栏时间
                    MAIN_SetStbDispTime();
                }
                MMK_CloseWin(win_id);
                break;
                
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
                
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
             default:
                break;
           }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
	case MSG_NOTIFY_LIST_SELECT_QUERY:
		{
			GUILIST_QRERY_SELECT_ITEM_T *item_it_ptr = PNULL;

			item_it_ptr = (GUILIST_QRERY_SELECT_ITEM_T*)param;
			
			if (PNULL != item_it_ptr)
			{
				if (MSG_APP_WEB == item_it_ptr->msg_id || MSG_TP_PRESS_UP == item_it_ptr->msg_id)
				{
					item_it_ptr->is_check_select = TRUE;
				}
			}
		}
		break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
	//case MSG_APP_WEB:
	//case MSG_CTL_MIDSK:
        CheckAfterSetDSTAction(ctrl_id);
        //改变手机的时间
        if(local_summer_flag != s_default_city_info[0].summer_flag)
        {
            //设置夏日时
            if(!local_summer_flag && s_default_city_info[0].summer_flag)
            {
                CalculateCityTime( 1,&s_date,&s_time,s_default_city_info[0].city_text_id);
            }
            else//去掉夏日时 
            {
                CalculateCityTime( 2,&s_date,&s_time,s_default_city_info[0].city_text_id);
            }

            MMIACC_SetRTCDateTimeByString(&s_date, &s_time);
            //立即更新状态栏时间
            MAIN_SetStbDispTime();

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
//  Description :append HandleWclkOptionDeleteCityWinMsg item
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void AppendCurrentDisplayCitys(
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    uint16      max_item = WT_DEFAULT_CITY_NUM;
    uint16      i = 0;
    uint8       city_count = 0;

	//set max before append
    GUILIST_SetMaxItem(ctrl_id,max_item,FALSE);
    GUILIST_SetMaxSelectedItem(ctrl_id,max_item);
	GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
    GUILIST_ChangeCheckListSoftkeyIndex(ctrl_id, MIDDLE_BUTTON);

    //append item
#ifdef MMI_WORLDCLOCK_MINI_SUPPORT
    GUILIST_ChangeCheckListSoftkeyIndex(ctrl_id, 1); //set middle softkey to display the Select/Cancel state;
    {
        GUILIST_ITEM_T      item_t = {0};
        GUILIST_ITEM_DATA_T item_data = {0};

        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_CHECK;
        item_t.item_data_ptr = &item_data;

        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = STXT_SELECT;
        item_data.softkey_id[2] = STXT_RETURN;
        for (i=1; i<WT_DEFAULT_CITY_NUM;i++)
        {
            if(s_default_city_info[i].city_text_id != TXT_NULL)
            {
                item_data.item_content[0].item_data.text_id = s_default_city_info[i].city_text_id;
                GUILIST_AppendItem(ctrl_id, &item_t);
                city_count++;
            }
        }
    }
#else
    for (i=1; i<WT_DEFAULT_CITY_NUM;i++)
    {
        if(s_default_city_info[i].city_text_id != TXT_NULL)
        {
            MMIAPISET_AppendListItemByTextId(s_default_city_info[i].city_text_id,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_CHECK);
            city_count++;
        }
    }
#endif
    if(0 == city_count)
    {
        GUIWIN_SetSoftkeyTextId(MMIACC_WCLK_DELETE_CITY_WIN_ID,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
    }
    
}

/*****************************************************************************/
//  Description :CheckAfterDeleteCitysAction
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL void CheckAfterDeleteCitysAction(
                        MMI_CTRL_ID_T   ctrl_id
                        )
{
    uint16                      selected_num = 0;
    uint16                      selected_item[WT_DEFAULT_CITY_NUM] = {0};
    uint16                      i = 0;
    int32                       j = 0;
    GUILIST_ITEM_T *            list_item_ptr      = PNULL;
    MMI_TEXT_ID_T               select_text_id = TXT_NULL;
    //get current selected items
    selected_num = GUILIST_GetSelectedItemIndex(ctrl_id,selected_item,WT_DEFAULT_CITY_NUM);

    for (j=0; j < selected_num;j++)
    {
        list_item_ptr = GUILIST_GetItemPtrByIndex(MMIACC_WCLK_OPTION_DELETE_LIST_CTRL_ID, selected_item[j]);  /*lint !e605*/
        if(PNULL == list_item_ptr )
            break;
        
        select_text_id = list_item_ptr->item_data_ptr->item_content[0].item_data.text_id;
        for(i=1; i < WT_DEFAULT_CITY_NUM;i++)
        {
            if(s_default_city_info[i].city_text_id != TXT_NULL)
            {
                   if(select_text_id == s_default_city_info[i].city_text_id)
                   {
                        s_default_city_info[i].summer_flag  = FALSE;
                        s_default_city_info[i].city_text_id = TXT_NULL;
                   }
            }
        }
    }
}

/*****************************************************************************/
//  Description : HandleWclkOptionDeleteCityWinMsg
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWclkOptionDeleteCityWinMsg(
                                             MMI_WIN_ID_T       win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIACC_WCLK_OPTION_DELETE_LIST_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendCurrentDisplayCitys(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);

        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
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
                    {
                        uint8 selected_num = 0;
                        selected_num = GUILIST_GetSelectedItemIndex(ctrl_id,PNULL,0);
                        if(0 == selected_num)
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_MUST_SELECT_ONE);
                            break;
                        }
                    }
                    CheckAfterDeleteCitysAction(ctrl_id);
                    MMK_CloseWin(win_id);
                    break;
                    
                case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                    break;
                    
                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                    break;
                default:
                    break;
           }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

	case MSG_NOTIFY_LIST_SELECT_QUERY:
		{
			GUILIST_QRERY_SELECT_ITEM_T *item_it_ptr = PNULL;

			item_it_ptr = (GUILIST_QRERY_SELECT_ITEM_T*)param;
			
			if (PNULL != item_it_ptr)
			{
				if (MSG_APP_WEB == item_it_ptr->msg_id || MSG_TP_PRESS_UP == item_it_ptr->msg_id)
				{
					item_it_ptr->is_check_select = TRUE;
				}
			}
		}
		break;

	// case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
	// case MSG_CTL_MIDSK:
        {
            uint8 selected_num = 0;
            selected_num = GUILIST_GetSelectedItemIndex(ctrl_id,PNULL,0);
            if(0 == selected_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_MUST_SELECT_ONE);
                break;
            }
	    }
        CheckAfterDeleteCitysAction(ctrl_id);
        MMK_CloseWin(win_id);
        MMK_PostMsg(MMIACC_WCLK_WIN_ID, MSG_APP_UP, PNULL, 0);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : CheckIfShowSetDSTMenu
//	Global resource dependence : 
//  Author:         murphy.xie
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN CheckIfShowSetDSTMenu(
                               )
{
    uint16      i = 0;
    uint16      city_count = 0;

    for (i=0; i<WT_DEFAULT_CITY_NUM;i++)
    {
        if(s_default_city_info[i].city_text_id != TXT_NULL)
        {
            city_count++;
        }
    }
    
    if(city_count > 0)
        return TRUE;
    else
        return FALSE;
}

/*****************************************************************************/
// 	Description : CheckIfShowDeleteMenu
//	Global resource dependence : 
//  Author:        murphy.xie
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN CheckIfShowDeleteMenu(
                               )
{
    uint16      i = 0;
    uint16      city_count = 0;

    for (i=1; i<WT_DEFAULT_CITY_NUM;i++)
    {
        if(s_default_city_info[i].city_text_id != TXT_NULL)
        {
            city_count++;
        }
    }
    
    if(city_count > 0)
        return TRUE;
    else
        return FALSE;
}
/*****************************************************************************/
// 	Description : the process message function of the accessory menu
//	Global resource dependence : 
//  Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWclkOptionWinMsg( 
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E			recode			= MMI_RESULT_TRUE;
    MMI_MENU_ID_T           menu_id			= 0;
    MMI_CTRL_ID_T			current_ctrl_id = MMIACC_WCLK_OPTION_CTRL_ID;
    MMI_MENU_GROUP_ID_T     group_id		= 0; 
    //BOOLEAN                 result          = FALSE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        MMK_SetAtvCtrl( win_id,  current_ctrl_id);
        if(!CheckIfShowSetDSTMenu())
        {
            GUIMENU_SetItemGrayed(MMIACC_WCLK_OPTION_CTRL_ID,MENU_WCLK_DST_OPTION, ID_ACC_WCLK_SET_DST, TRUE);
        }
        
        if(!CheckIfShowDeleteMenu())
        {
            GUIMENU_SetItemGrayed(MMIACC_WCLK_OPTION_CTRL_ID,MENU_WCLK_DST_OPTION, ID_ACC_WCLK_SET_DELETE_CITY, TRUE);
        }
        
#if defined(MMI_UPDATE_TIME_SUPPORT)
        if (MMIAPISET_GetIsAutoUpdateTime())
        {//CR178691
            GUIMENU_SetItemVisible(MMIACC_WCLK_OPTION_CTRL_ID,MENU_WCLK_DST_OPTION, ID_ACC_WCLK_SET_LOCAL1, FALSE);
        }
#endif  /*MMI_UPDATE_TIME_SUPPORT*/
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
	case MSG_APP_WEB:
	case MSG_CTL_MIDSK:

        GUIMENU_GetId(current_ctrl_id, &group_id, &menu_id);
        if (ID_ACC_WCLK_SET_LOCAL1 == menu_id)
        {
        	MMK_CreateWin((uint32 *)WCLK_ADD_CITY_WIN_TAB,(ADD_DATA)menu_id);
                //result = TRUE;

        }
        else if(ID_ACC_WCLK_SET_ADD_CITY == menu_id)
        {
            MMK_CreateWin((uint32 *)WCLK_ADD_CITY_WIN_TAB,PNULL);
                //result = FALSE;

        }
        else if(ID_ACC_WCLK_SET_DELETE_CITY == menu_id)
        {
            MMK_CreateWin((uint32 *)WCLK_DELETE_CITY_TAB,PNULL);
                //result = FALSE;
        }
        else
        {   
            MMK_CreateWin((uint32 *)WCLK_SET_DST_CITY_TAB,PNULL);
                //result = TRUE;
        }
       
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//   FUNCTION:      MMIAPIWCLK_Init
// 	        Init NV, ...
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWCLK_Init(void)
{
    
    MN_RETURN_RESULT_E nv_return = MN_RETURN_SUCCESS;

    SCI_MEMSET(s_default_city_info,0,sizeof(s_default_city_info));
    MMINV_READ(MMINV_ACC_WORLDCLOCK_CITY_INFO, (void *)s_default_city_info, nv_return);   
    if (MN_RETURN_SUCCESS != nv_return)
    {
        int i= 0;
        for(i= 0; i < WT_DEFAULT_CITY_NUM; i++ )
        {
            s_default_city_info[i].city_text_id = TXT_NULL;
            s_default_city_info[i].summer_flag = FALSE;

        }
        //北京 
        s_default_city_info[0].city_text_id = TXT_ACC_WORLD_GMT_8_P_BEIJING;

        MMINV_WRITE(MMINV_ACC_WORLDCLOCK_CITY_INFO, (void *)s_default_city_info);
        #ifndef RELEASE_INFO //add for test 
        SCI_ASSERT(s_default_city_info[0].city_text_id == TXT_ACC_WORLD_GMT_8_P_BEIJING);   /*assert verified*/
        #endif
    }
    return TRUE;
}



/***********************************************************************************
Name:			MMIWCLK_ResetEnvSetSetting
Description:	reset to default value of worldtime GMT
Author:    Murphy.xie
DATE:  2010/11/26
Return:		 True if  date is ok, otherwise false.
***********************************************************************************/
PUBLIC BOOLEAN MMIAPIWCLK_ResetFactorySetting(void)
{
 
    int i= 0;
    for(i= 0; i < WT_DEFAULT_CITY_NUM; i++ )
    {
        s_default_city_info[i].city_text_id = TXT_NULL;
        s_default_city_info[i].summer_flag = FALSE;

    }
    //北京 
    s_default_city_info[0].city_text_id = TXT_ACC_WORLD_GMT_8_P_BEIJING;

    MMINV_WRITE(MMINV_ACC_WORLDCLOCK_CITY_INFO, s_default_city_info);


    return TRUE;
}


/***********************************************************************************
 Name:			MMIALM_IsDateValidByString
 Description:	This function checks the date the user has entered before it is saved.
				(The date format: YYYY_MM_DD)
 Input:			date -- 
 Return:		 True if  date is ok, otherwise false.
***********************************************************************************/
LOCAL BOOLEAN  MMIALM_IsDateValidByString(
                                          MMIACC_RTC_DATE_T* date
                                          )
{
    int16	year_int = 0;
    int16	month_int = 0;
    int16	day_int = 0;
    int8	year_char[5] = {0};
    int8	month_char[3] = {0};
    int8	day_char[3] = {0};
    BOOLEAN	is_leap_year = FALSE;

    //SCI_ASSERT(date != PNULL);/*assert verified*/
    if(date == PNULL)
    {
        return FALSE;
    }
    // Convert the date to integers for year
    MMI_MEMCPY(&year_char[0], 4, *date, 4, 4);
    year_char[4] = '\0';
    year_int = atoi((char*)&year_char[0]);

    // Convert the date to integers for month
    MMI_MEMCPY(&month_char[0], 2,  *date + 5, 2,  2);
    month_char[2] = '\0';
    month_int = atoi((char*) &month_char[0]);

    // Convert the date to integers for day
    MMI_MEMCPY(&day_char[0],2,  *date + 8, 2,  2);
    day_char[2] = '\0';
    day_int = atoi( (char*) &day_char[0]);

    // Check leap year
    is_leap_year = MMIAPICOM_IsLeapYear((uint32)year_int);

    // Check date
    if (!is_leap_year) 
    {
        if (month_int == 2 && day_int >= 29) 
        {
            MMI_MEMCPY(*date + 8, 2, "28",2,  2);
            return TRUE;
        }
    }

    if ((year_int < MMICOM_DATE_MIN_YEAR) || (year_int > MMICOM_DATE_MAX_YEAR))
    {
        return FALSE;
    }
    else if ((month_int < 1) || (month_int > 12)) 
    {
        return FALSE;
    }
    else if (day_int < 1)
    {
        return FALSE;
    }
    else if (day_int > s_RTC_month_info_leap[month_int - 1]) 
    {
        sprintf( (char*) &day_char[0], "%02d", s_RTC_month_info_leap[month_int - 1]);
        MMI_MEMCPY(*date + 8, 2,  &day_char[0], 2,  2);
        return TRUE;
    }
    return TRUE;
}


/*****************************************************************************/
//   FUNCTION:      MMIACC_SetRTCTimeByString
// 	Description :   set current sys time
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN MMIACC_SetRTCTimeByString(
                                        MMIACC_RTC_TIME_T *time 
                                        )
{
	SCI_TIME_T systime = {0};

	systime.hour = atoi((char*)time);
	systime.min = atoi((char*)time+3);
	systime.sec = atoi((char*)time+6);
	if(FALSE == MMIAPICOM_SetSysTime(systime))
	{
		return FALSE;
	}
	return( TRUE );
} 


/*****************************************************************************/
//  FUNCTION:      MMIACC_SetRTCDateByString
// 	Description :   set current sys date
//	Global resource dependence : 
//  Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN MMIACC_SetRTCDateByString(
                                        MMIACC_RTC_DATE_T *date
                                        )
{
	SCI_DATE_T sysdate = {0};

	sysdate.year = atoi((char*)date);
	sysdate.mon = atoi((char*)date+5);
	sysdate.mday = atoi((char*)date+8);
	if(FALSE == MMIAPICOM_SetSysData(sysdate))
	{
		return FALSE;
	}
	return TRUE;
}

/*****************************************************************************/
//   FUNCTION:      MMIACC_SetRTCDateTimeByString
// 	Description :   set current sys time
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN MMIACC_SetRTCDateTimeByString(
                                            MMIACC_RTC_DATE_T *date,
                                            MMIACC_RTC_TIME_T *time 
                                            )
{
    BOOLEAN     result = FALSE;

	if ( MMIALM_IsDateValidByString( date )) 
	{
		if(MMIACC_SetRTCDateByString( date ))
        {
            if(MMIACC_SetRTCTimeByString( time ))
            {
	            MMIAPISET_UpdateAlarmSet();//更新闹钟/日程/开关机等设置,
                result = TRUE;
            }
        }
	}

    return result;
}

/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZoneInfo
//	Global resource dependence : 
//  Author:       murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MCC_WT_CITY_INFO_T* MMIAPIACC_GetLocalTimeZoneInfo(void)
{
    MMIACC_RTC_DATE_T     *init_date = NULL;
    MMIACC_RTC_TIME_T     *init_time = NULL;
    /**********************/  	
    /* Read the date&time */  	
    /**********************/  	
    init_date = MMIAPICOM_GetRTCDate();	// length 10	
    init_time = MMIAPICOM_GetRTCTime();	// length 5	

    /*********************/  	
    MMI_MEMCPY(& s_local_city_info.date, sizeof( MMIACC_RTC_DATE_T ),
            init_date, sizeof( MMIACC_RTC_DATE_T ),
            sizeof( MMIACC_RTC_DATE_T ));  	
    MMI_MEMCPY( &s_local_city_info.time, sizeof( MMIACC_RTC_TIME_T ),
            init_time, sizeof( MMIACC_RTC_TIME_T ),
            sizeof( MMIACC_RTC_TIME_T ));  
    
    return &s_local_city_info;
}

/*****************************************************************************/
// 	Description : MMIAPIACC_GetOtherTimeZoneInfo
//	Global resource dependence : 
//  Author:   murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MCC_WT_CITY_INFO_T* MMIAPIACC_GetOtherTimeZoneInfo(void)
{
    int32   index  = 0;
    SCI_MEMSET(&s_other_city_info,0,sizeof(s_other_city_info));	

    for(index = 1; index < WT_DEFAULT_CITY_NUM; index++)
    {
        if(s_default_city_info[index].city_text_id != TXT_NULL)
        {
            CalculateCityTime( 0,&s_date,&s_time,s_default_city_info[index].city_text_id );
            MMI_MEMCPY(& s_other_city_info.date, sizeof( MMIACC_RTC_DATE_T ),
            s_date, sizeof( MMIACC_RTC_DATE_T ),
            sizeof( MMIACC_RTC_DATE_T ));  	
            MMI_MEMCPY( &s_other_city_info.time, sizeof( MMIACC_RTC_TIME_T ),
            s_time, sizeof( MMIACC_RTC_TIME_T ),
            sizeof( MMIACC_RTC_TIME_T ));  
                return &s_other_city_info;

        }
    }
    
    return PNULL;
}

/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZoneStringID
//	Global resource dependence : 
//  Author:       murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIACC_GetLocalCityStringID(void)
{
	return s_default_city_info[0].city_text_id ;
}

/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZoneStringID
//	Global resource dependence : 
//  Author:       murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIACC_GetOtherCityStringID(void)
{

    //找一遍看看有没有城市
    int32   index  = 0;
    for(index = 1; index < WT_DEFAULT_CITY_NUM; index++)
    {
        if(s_default_city_info[index].city_text_id != TXT_NULL)
        {
            return s_default_city_info[index].city_text_id;
        }
    }

    //没有找到，返回NULL
    return TXT_NULL;
}

/*****************************************************************************/
// 	Description : MMIAPIWCLK_OpenMainWin
//         
//	Global resource dependence : 
//   Author:        zhaohui
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIWCLK_OpenMainWin(void)
{
	MMK_CreateWin((uint32 *)WCLK_WIN_TAB,PNULL);
}

/*****************************************************************************/
//   FUNCTION:      MMIAPIACC_GetLocalTimeZone
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   由于新的需求增加半时区，所以此接口返回的值将会是float
/*****************************************************************************/
PUBLIC float MMIAPIACC_GetLocalTimeZone(void)
{
    float min = (float)60.0;
    float min_value = (float)GetZoneMinByZoneTextID(GetZoneTextIDByCityTextID(s_default_city_info[0].city_text_id)); 
    float result = 0.0f;

    result = min_value/min;

    SCI_TRACE_LOW("MMIAPIACC_GetLocalTimeZone time zone result =%f, min_value = %f, min= %f", result, min_value, min);

    return result; 
}

/*****************************************************************************/
//   FUNCTION:      MMIAPIACC_GetLocalTimeZoneEx
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   由于新的需求增加半时区，所以此接口返回的值将会是float
/*****************************************************************************/
PUBLIC void MMIAPIACC_GetLocalTimeZoneEx(float* fOut)
{
    float min = (float)60.0;
    float min_value = (float)GetZoneMinByZoneTextID(GetZoneTextIDByCityTextID(s_default_city_info[0].city_text_id));
    float result = 0.0f;

    result = min_value/min;

    SCI_TRACE_LOW("MMIAPIACC_GetLocalTimeZoneEx time zone result =%f, min_value = %f, min= %f", result, min_value, min);

    if(PNULL != fOut)
    {
        *fOut = result;
    }
}

/*****************************************************************************/
//   FUNCTION:      MMIAPIACC_IsLocalTimeZoneInSummer
//	Global resource dependence : 
//   Author:        dave.ruan
//	Note:   判断当前时区是否处于夏令时
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIACC_IsLocalTimeZoneInSummer(void)
{

    return  s_default_city_info[0].summer_flag; 
}

/*****************************************************************************/
//   FUNCTION:      Get Local Time Zone Info
//	Global resource dependence : 
//   Author:        dave.ruan
//	Note:   get local city,timezone,and mcc info
/*****************************************************************************/
PUBLIC WT_TIME_CITY_ZONE MMIAPIACC_GetLocalTimeZoneCityInfo(void)
{
    uint32 i = 0;

    for(i = 0; i < WT_TOTAL_CITY_NUM; i++)
    {
        if(s_zone_city_info[i].city_index == s_default_city_info[0].city_text_id)
        {
            return s_zone_city_info[i];
        }
    }

    return s_zone_city_info[0];
}

#if defined(MMI_UPDATE_TIME_SUPPORT)
/*****************************************************************************/
// Description : 设置本地时区和夏令时
//              (由网络时间更新系统时间的同时,时区和夏令时也需同步 更新)
// Global resource dependence :
// Author:dave.ruan
// Note: mcc  从plmn可以获取，是当前运营商的plmn，而非SIM卡的
//      time zone is int8,表示以15分钟为单位的时区，是整数时区的4倍,
//      dst_value is daylight saving time的值，可以是0,1,2, 表示夏令时偏移1或2小时
//      current_year    当前年份
/*****************************************************************************/
PUBLIC void MMIAPIWCLK_SetTimeZone(
                                   uint16  mcc,     //MN_PLMN_T.mcc
                                   const int8  time_zone,   //(-12*4~12.75*4)
                                   uint16 current_year,  //year index from NITZ
                                   uint8 dst_value      //is 0,1,2
                                   )
{
    MMI_TEXT_ID_T   city_index_id = TXT_NULL;
    uint8   default_city_timezone_num = ARRAY_SIZE(s_default_timezone_city_info);//sizeof(s_default_timezone_city_info)/sizeof(WT_TIME_DEFAULT_CITY_ZONE);  
    uint8   all_city_timezone_num = ARRAY_SIZE(s_zone_city_info);//sizeof(s_zone_city_info)/sizeof(WT_TIME_CITY_ZONE);
    uint16 default_dst_value_num = ARRAY_SIZE(s_all_dst_value_info);//sizeof(s_all_dst_value_info)/sizeof(WT_TIME_DST_VALUE_T);
    int16 time_zone_in_minutes = time_zone* 15; //以分钟计算的timezone
    uint8  i = 0;

    //SCI_TRACE_LOW:"MMIAUT::MMIAPIWCLK_SetTimeZone mcc= %d,time_zone = %d, current_year =%d, dst_value=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_2642_112_2_18_2_2_32_113,(uint8*)"dddd",mcc,time_zone,current_year,dst_value);

    //search the current city from a default city list,
    for(i = 0; i < default_city_timezone_num; i++)
    {
        if(((s_default_timezone_city_info[i].mcc == mcc) ||  (404 == s_default_timezone_city_info[i].mcc && 405 == mcc))
            &&((s_time_zone_arr[s_default_timezone_city_info[i].zone_index-TXT_ACC_WORLD_GMT_0]) == time_zone_in_minutes)
        )
        {
            city_index_id = s_default_timezone_city_info[i].city_index;
            break;
        }
    }

    //SCI_TRACE_LOW:"MMIAUT::MMIAPIWCLK_SetTimeZone city_index1=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_2656_112_2_18_2_2_32_114,(uint8*)"d",city_index_id);
    /*if do not find the right city, search it from the whole list according to the mcc & timezone*/
    if(TXT_NULL == city_index_id) 
    {
        for(i = 0; i < all_city_timezone_num; i++)
        {
            if(((s_time_zone_arr[s_zone_city_info[i].zone_index -TXT_ACC_WORLD_GMT_0]) == time_zone_in_minutes)
                &&(s_zone_city_info[i].mcc== mcc)
                ||(s_zone_city_info[i].mcc== mcc && (TONGA_DEFAULT_TIME_ZONE_IN_MIN == time_zone_in_minutes))
            )
            {
                city_index_id = s_zone_city_info[i].city_index;
                break;
            }
        }

        //SCI_TRACE_LOW:"MMIAUT::MMIAPIWCLK_SetTimeZone city_index_id2=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_2672_112_2_18_2_2_32_115,(uint8*)"d",city_index_id);
        /*if do not find the right city, get the first city of current timezone*/
        if(TXT_NULL == city_index_id)
        {
            for(i = 0; i < all_city_timezone_num; i++)
            {
                if(((s_time_zone_arr[s_zone_city_info[i].zone_index -TXT_ACC_WORLD_GMT_0]) == time_zone_in_minutes))
                {
                    city_index_id = s_zone_city_info[i].city_index;
                    break;
                }
            }        
        }
    }
    


    //SCI_TRACE_LOW:"MMIAUT::MMIAPIWCLK_SetTimeZone city_index_id3=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_2692_112_2_18_2_2_32_116,(uint8*)"d",city_index_id);
    if(TXT_NULL == city_index_id)
    {
        return;
    }
    
    SetWorldClockToLocal(city_index_id);
    
    //deal with DST
    if(2 >= dst_value)
    {
        MMI_TM_T    daylight_start_datetime ={0};
        MMI_TM_T    daylight_end_datetime ={0};
        uint8       dst_index =0;
        
        //set daylight datetime value
        for(i = 0; i < default_dst_value_num; i++ )
        {
            
            if((s_all_dst_value_info[i].current_year == current_year)
            &&(s_all_dst_value_info[i].mcc == mcc)
            &&(s_all_dst_value_info[i].standard_timezone == time_zone))
            {
                daylight_start_datetime.tm_min = s_all_dst_value_info[i].daylight_start_datetime.tm_min; 
                daylight_start_datetime.tm_hour = s_all_dst_value_info[i].daylight_start_datetime.tm_hour; 
                daylight_start_datetime.tm_mday = s_all_dst_value_info[i].daylight_start_datetime.tm_mday; 
                daylight_start_datetime.tm_mon = s_all_dst_value_info[i].daylight_start_datetime.tm_mon; 
                
                daylight_end_datetime.tm_min = s_all_dst_value_info[i].daylight_end_datetime.tm_min;
                daylight_end_datetime.tm_hour = s_all_dst_value_info[i].daylight_end_datetime.tm_hour;
                daylight_end_datetime.tm_mday= s_all_dst_value_info[i].daylight_end_datetime.tm_mday;
                daylight_end_datetime.tm_mon = s_all_dst_value_info[i].daylight_end_datetime.tm_mon;   
                dst_index = i;
                break;
            }
        }
        
        //SCI_TRACE_LOW:"MMIAUT::MMIAPIWCLK_SetTimeZone daylight_start_datetime=%d:%d:%d,%d-%d-%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_2726_112_2_18_2_2_32_117,(uint8*)"dddddd",0,daylight_start_datetime.tm_min,daylight_start_datetime.tm_hour,daylight_start_datetime.tm_mday,daylight_start_datetime.tm_mon,s_all_dst_value_info[dst_index].current_year);
        //SCI_TRACE_LOW:"MMIAUT::MMIAPIWCLK_SetTimeZone daylight_end_datetime=%d:%d:%d,%d-%d-%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_2729_112_2_18_2_2_32_118,(uint8*)"dddddd",0,daylight_end_datetime.tm_min,daylight_end_datetime.tm_hour,daylight_end_datetime.tm_mday,daylight_end_datetime.tm_mon,s_all_dst_value_info[dst_index].current_year);

        if((uint8)s_default_city_info[0].summer_flag != dst_value)
        {
            if(0 != dst_value)
            {
                s_default_city_info[0].summer_flag = TRUE;
                //s_is_summer_flag_nitz_dst = TRUE;
            }
            else    //异常处理
            {
                //dst exist
                if((0 != daylight_start_datetime.tm_mday) 
                &&(0 !=  daylight_start_datetime.tm_mon))
                {
                    SCI_DATE_T sysdate = {0};
                    SCI_TIME_T systime = {0};
                    uint32 sys_second = 0;
                    uint32 dst_start_date_second, dst_end_date_second= 0;
                    
                    if(TM_GetSysDate(&sysdate)
                    &&TM_GetSysTime(&systime))
                    {
                        //transfer all time to seconds
                        sys_second = MMIAPICOM_Tm2Second((uint32) systime.sec, 
                                                                                (uint32) systime.min, 
                                                                                (uint32) systime.hour, 
                                                                                (uint32) sysdate.mday, 
                                                                                (uint32) sysdate.mon, 
                                                                                (uint32) sysdate.year);
                                                                                
                        dst_start_date_second = MMIAPICOM_Tm2Second( 0, 
                                                                                (uint32) daylight_start_datetime.tm_min, 
                                                                                (uint32) daylight_start_datetime.tm_hour, 
                                                                                (uint32) daylight_start_datetime.tm_mday, 
                                                                                (uint32) daylight_start_datetime.tm_mon, 
                                                                                (uint32) s_all_dst_value_info[dst_index].current_year);
                        
                        dst_end_date_second = MMIAPICOM_Tm2Second( 0, 
                                                                                (uint32) daylight_end_datetime.tm_min, 
                                                                                (uint32) daylight_end_datetime.tm_hour, 
                                                                                (uint32) daylight_end_datetime.tm_mday, 
                                                                                (uint32) daylight_end_datetime.tm_mon, 
                                                                                (uint32) s_all_dst_value_info[dst_index].current_year);
                        //SCI_TRACE_LOW:"MMIAUT::MMIAPIWCLK_SetTimeZone sys_second = %d,dst_start = %d,dst_end= %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWCLK_MAIN_2773_112_2_18_2_2_32_119,(uint8*)"ddd",sys_second,dst_start_date_second,dst_end_date_second);
                        //is_southern_hemisphere?
                        if(dst_start_date_second  < dst_end_date_second)
                        {
                            //it is in northern hemisphere
                            //such as daylight saving mode start 2:00 1.4.2012, end 2:00,28.10.2012
                            if(dst_start_date_second < sys_second  && dst_end_date_second > sys_second)
                            {
                                //dst value is 1,if it has until now,if it is not 1, use follow value
                                //(s_all_dst_value_info[dst_index].timezone_with_dst-s_all_dst_value_info[dst_index].standard_timezone)/60 
                                s_default_city_info[0].summer_flag  = TRUE;
                            }
                            else
                            {
                                //it is in winter time
                                s_default_city_info[0].summer_flag = FALSE;
                            }
                        }
                        else    //城市属于南半球
                        {
                            //such as daylight saving mode start 2:00,28.10.2012,end 2:00 1.4.2012
                            if(dst_start_date_second < sys_second  ||dst_end_date_second > sys_second)
                            {
                                //it is in winter time
                                s_default_city_info[0].summer_flag = FALSE;
                            }
                            else
                            {
                                //dst value is 1,if it has until now,if it is not 1, use follow value
                                //(s_all_dst_value_info[dst_index].timezone_with_dst-s_all_dst_value_info[dst_index].standard_timezone)/60 
                                s_default_city_info[0].summer_flag = TRUE;
                            }
                        }
                    }
                }
            }
            
        }
    }
    else
    {
        //do  nothing
    }

    SCI_TRACE_LOW("MMIAUT::MMIAPIWCLK_SetTimeZone s_dst_fs_default_city_info[0].summer_flag  =%d",s_default_city_info[0].summer_flag);/*lint !e662*/
    
    SavaCityInfoToNV();
    UpdateDisplayCityInfo();         
}

#endif /*MMI_UPDATE_TIME_SUPPORT*/

#else
/*****************************************************************************/
//   FUNCTION:      MMIAPIWCLK_Init
// 	        Init NV, ...
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWCLK_Init(void)
{
    return TRUE;
}

/***********************************************************************************
Name:			MMIWCLK_ResetEnvSetSetting
Description:	reset to default value of worldtime GMT
Author:    Murphy.xie
DATE:  2010/11/26
Return:		 True if  date is ok, otherwise false.
***********************************************************************************/
PUBLIC BOOLEAN MMIAPIWCLK_ResetFactorySetting(void)
{
    return TRUE;
}
/*****************************************************************************/
// 	Description : MMIAPIWCLK_OpenMainWin
//         
//	Global resource dependence : 
//   Author:        zhaohui
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIWCLK_OpenMainWin(void)
{

}

/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZone
//	Global resource dependence : 
//   Author:        zhaohui
//	Note:   由于新的需求增加半时区，所以此接口返回的值将会是float
/*****************************************************************************/
PUBLIC float MMIAPIACC_GetLocalTimeZone(void)
{
     return 0;
}
/*****************************************************************************/
//   FUNCTION:      MMIAPIACC_GetLocalTimeZoneEx
//	Global resource dependence : 
//   Author:        taul.zhu
//	Note:   由于新的需求增加半时区，所以此接口返回的值将会是float
/*****************************************************************************/
PUBLIC void MMIAPIACC_GetLocalTimeZoneEx(float* fOut)
{
    return;
}
/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZoneInfo
//	Global resource dependence : 
//  Author:       murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MCC_WT_CITY_INFO_T* MMIAPIACC_GetLocalTimeZoneInfo(void)
{
       return (MCC_WT_CITY_INFO_T*)NULL;
}

/*****************************************************************************/
// 	Description : MMIAPIACC_GetOtherTimeZoneInfo
//	Global resource dependence : 
//  Author:   murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MCC_WT_CITY_INFO_T* MMIAPIACC_GetOtherTimeZoneInfo(void)
{
       return (MCC_WT_CITY_INFO_T*)NULL;
}

/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZoneStringID
//	Global resource dependence : 
//  Author:       murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIACC_GetLocalCityStringID(void)
{
     return  (MMI_TEXT_ID_T)0;
}

/*****************************************************************************/
// 	Description : MMIAPIACC_GetLocalTimeZoneStringID
//	Global resource dependence : 
//  Author:       murphy.xie
//	Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIACC_GetOtherCityStringID(void)
{
    return (MMI_TEXT_ID_T)0;
}

#if defined(MMI_UPDATE_TIME_SUPPORT)
/*****************************************************************************/
// 	Description 设置本地时区和夏令时
//	Global resource dependence :
//  Author: xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWCLK_SetTimeZone(
                                   uint16  mcc,     //MN_PLMN_T.mcc
                                   const int8  time_zone,
                                   uint16 current_year,  //year index from NITZ
                                   uint8 dst_value      //is 0,1,2
                                   )
{
}
#endif /*MMI_UPDATE_TIME_SUPPORT*/

#endif//WORLD_CLOCK_SUPPORT

/*Edit by script, ignore 2 case. Thu Apr 26 19:00:48 2012*/ //IGNORE9527


/*Edit by script, ignore 4 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527

