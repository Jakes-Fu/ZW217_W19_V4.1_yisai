/****************************************************************************
** File Name:      mmiaut_net.c
** Author:                                                                  
** Date:           2010/07/01
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is mmi nettime api file.AUT is short for auto update time
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 07/01/2010        xiaoming             Create
** 01/20/2012       dave.ruan             modify,add sntp
****************************************************************************/

#include "mmi_app_aut_trc.h"
#ifdef  MMI_UPDATE_TIME_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmiidle_subwintab.h"
#include "mmicom_time.h"
#include "mmipub.h"
#ifdef MMI_SNTP_SUPPORT
#include "sntp_api.h"
#include "in_message.h"
#endif
#include "mmiaut_internal.h"
#include "mmiaut_export.h"
#include "mmiwclk_export.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

#if defined(MMI_WIDGET_WEATHER1) && defined(PDA_WEATHER_TIME_SE)
#include "mmiweather_export.h"
#endif

#include "mmiphone_nitz.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     (sizeof(a)/sizeof (a)[0])
#endif
#define AUT_SNTP_TIME_DIFF_FROM_1970_TO_1980     ((365*10+2)*24*3600)//315619200-86400(1970-1979)

#define AUT_N_ZONES  (75 + 1)
#define AUT_MAX_TIMEZONE_VALUE 45900  
#define AUT_MIN_TIMEZONE_VALUE -43200  
#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
#define AUT_NUM_PDPRETRY 16
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct  
{
    uint16   mcc;     //mobile country code
    int32     timezone;  /* major timezone if this country only one timezone;unit is seconds*/
}TIMEZONE_OF_COUNTRY_T;

#ifdef MMI_SNTP_SUPPORT
typedef struct  
{
    int32 z_gmtoff; /* zone's basic GMT offset */
    int16 z_rules;  /* index of rules to use */
}AUT_ZONE_DST_RULE_T;

typedef struct  
{
    uint16 mcc;     //mobile country code
    int16 z_rules;  /* index of dst rules to use */
}RULES_OF_COUNTRY_T;

#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
typedef struct
{
    BOOLEAN isAction;
   uint8 active_retry_num;
   MN_DUAL_SYS_E dual_sys;
}AUT_PDP_RETRY_T;
#endif

LOCAL  char* sntp_server_addr[] =
{
#ifdef WIN32
        "172.16.14.56",//it is the developer's computer IP addr
#endif        
        "clock.fmt.he.net",
        "clock.nyc.he.net",
        "clock.sjc.he.net",
        "clock.via.net",
        "ntp1.tummy.com",
        "time.cachenetworks.com",
        "time.nist.gov",
};
/*as defined in sntp_dst.c*/
LOCAL const RULES_OF_COUNTRY_T s_rules_map[] =
{
    {302,0},    //north America DST
    {310,0},
    //{334,0},
    
    {268, 1},   //Portugal
    {625, 1},//cape verde 佛得角
    {234, 1},//UK
    {232, 1},//Austria Vienna
    {206, 1},// Belgium  Brussels
    {230, 1},//Czech Republic
    {238, 1},//Denmark
    {208, 1},    //france
    {262, 1},    //Germany
    {216, 1},    //Hungary
    {222, 1},    //Italy
    {270, 1},    //Luxembourg
    {204, 1},    //Netherlands
    {260, 1},    //Poland
    {214, 1},    //Spain
    {240, 1},    //Sweden
    {228, 1},    //Switzerland
    {284, 1},    //Bulgaria
    {202, 1},    //Greece
    {226, 1},    //Romania
    {286, 1},    //Turkey
    {255, 1},    //Ukraine
    
    {730,2},     //Chile

    {505,3},    //Australia

    {602,4},    //Egypt,Cairo

    {425,5},    //Israel
    
    {418,6},    //Iraq

    {250,7},    //Russia
    {540,7},    //Solomon Is.
    {546,7},    //New Caledonia

    {432,8},    //iran Tehran

    {282,9},    //Georgia
    {283,9},    //Armenia
    {400,9},    //Azerbaijani Republic

//    {505,10},//Australia Hobart

    {530,11},   //NEW zealand

//    {250,12},       //russia Aisa

    {334,13},       //Mexico

    {724,14},       //brazil
};

/* This must match sntpcfg.html and sntp.c */
LOCAL const AUT_ZONE_DST_RULE_T s_time_zones [] = 
{
    /* offset rules */
    { -43200, -1 }, /* (GMT-12:00) International Date Line West */
    { -39600, -1 }, /* (GMT-11:00) Midway Island, Samoa */
    { -36000, -1 }, /* (GMT-10:00) Hawaii */
    { -32400,  0 }, /* (GMT-09:00) Alaska */
    { -28800,  0 }, /* (GMT-08:00) Pacific Time, Tijuana */
    { -25200, -1 }, /* (GMT-07:00) Arizona, Mazatlan */
    { -25200, 13 }, /* (GMT-07:00) Chihuahua, La Paz */
    { -25200,  0 }, /* (GMT-07:00) Mountain Time */
    { -21600,  0 }, /* (GMT-06:00) Central America */
    { -21600,  0 }, /* (GMT-06:00) Central Time */
    { -21600, 13 }, /* (GMT-06:00) Guadalajara, Mexico City, Monterrey */
    { -21600, -1 }, /* (GMT-06:00) Saskatchewan */
    { -18000, -1 }, /* (GMT-05:00) Bogota, Lima, Quito */
    { -18000,  0 }, /* (GMT-05:00) Eastern Time */
    { -18000, -1 }, /* (GMT-05:00) Indiana */
    { -14400,  0 }, /* (GMT-04:00) Atlantic Time */
    { -14400, -1 }, /* (GMT-04:00) Caracas, La Paz */
    { -14400,  2 }, /* (GMT-04:00) Santiago */
    { -12600,  0 }, /* (GMT-03:30) Newfoundland */
    { -10800, 14 }, /* (GMT-03:00) Brasilia */
    { -10800, -1 }, /* (GMT-03:00) Buenos Aires, Georgetown */
    { -10800, -1 }, /* (GMT-03:00) Greenland */
    {  -7200, -1 }, /* (GMT-02:00) Mid-Atlantic */
    {  -3600,  1 }, /* (GMT-01:00) Azores */
    {  -3600, -1 }, /* (GMT-01:00) Cape Verde Is. */
    {      0, -1 }, /* (GMT) Casablanca, Monrovia */
    {      0,  1 }, /* (GMT) Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London */
    {   3600,  1 }, /* (GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna */
    {   3600,  1 }, /* (GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague */
    {   3600,  1 }, /* (GMT+01:00) Brussels, Copenhagen, Madrid, Paris */
    {   3600,  1 }, /* (GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb */
    {   3600, -1 }, /* (GMT+01:00) West Central Africa */
    {   7200,  1 }, /* (GMT+02:00) Athens, Istanbul, Minsk */
    {   7200,  1 }, /* (GMT+02:00) Bucharest */
    {   7200,  4 }, /* (GMT+02:00) Cairo */
    {   7200, -1 }, /* (GMT+02:00) Harare, Pretoria */
    {   7200,  1 }, /* (GMT+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius */
    {   7200,  5 }, /* (GMT+02:00) Jerusalem */
    {  10800,  6 }, /* (GMT+03:00) Baghdad */
    {  10800, -1 }, /* (GMT+03:00) Kuwait, Riyadh */
    {  10800,  7 }, /* (GMT+03:00) Moscow, St. Petersburg, Volgograd */
    {  10800, -1 }, /* (GMT+03:00) Nairobi */
    {  12600,  8 }, /* (GMT+03:30) Tehran */
    {  14400, -1 }, /* (GMT+04:00) Abu Dhabi, Muscat */
    {  14400,  9 }, /* (GMT+04:00) Baku, Tbilisi, Yerevan */
    {  16200, -1 }, /* (GMT+04:30) Kabul */
    {  18000,  7 }, /* (GMT+05:00) Ekaterinburg */
    {  18000, -1 }, /* (GMT+05:00) Islamabad, Karachi, Tashkent */
    {  19800, -1 }, /* (GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi */
    {  20700, -1 }, /* (GMT+05:45) Kathmandu */
    {  21600, 12 }, /* (GMT+06:00) Almaty, Novosibirsk */
    {  21600, -1 }, /* (GMT+06:00) Astana, Dhaka */
    {  21600, -1 }, /* (GMT+06:00) Sri Jayawardenepura */
    {  23400, -1 }, /* (GMT+06:30) Rangoon */
    {  25200, -1 }, /* (GMT+07:00) Bangkok, Hanoi, Jakarta */
    {  25200,  7 }, /* (GMT+07:00) Krasnoyarsk */
    {  28800, -1 }, /* (GMT+08:00) Beijing, Chongquing, Hong Kong, Urumqi */
    {  28800, -1 }, /* (GMT+08:00) Irkutsk, Ulaan Bataar */
    {  28800, -1 }, /* (GMT+08:00) Kuala Lumpur, Singapore */
    {  28800, -1 }, /* (GMT+08:00) Perth */
    {  28800, -1 }, /* (GMT+08:00) Taipei */
    {  32400, -1 }, /* (GMT+09:00) Osaka, Sapporo, Tokyo */
    {  32400, -1 }, /* (GMT+09:00) Seoul */
    {  32400,  7 }, /* (GMT+09:00) Yakutsk */
    {  34200,  3 }, /* (GMT+09:30) Adelaide */
    {  34200, -1 }, /* (GMT+09:30) Darwin */
    {  36000, -1 }, /* (GMT+10:00) Brisbane */
    {  36000,  3 }, /* (GMT+10:00) Canberra, Melbourne, Sydney */
    {  36000, -1 }, /* (GMT+10:00) Guam, Port Moresby */
    {  36000, 10 }, /* (GMT+10:00) Hobart */
    {  36000,  7 }, /* (GMT+10:00) Vladivostok */
    {  39600, -1 }, /* (GMT+11:00) Magadan */
    {  39600,  7 }, /* (GMT+11:00) Solomon Is., New Caledonia */
    {  43200, 11 }, /* (GMT+12:00) Auckland, Wellington */
    {  43200, -1 }, /* (GMT+12:00) Fiji, Kamchatka, Marshall Is. */
    {  45900, -1 }, /* (GMT+12:75) Samoa Is. */
};

#define MAX_SNTP_SERVER_ADDR  (ARRAY_SIZE(sntp_server_addr))
#endif

LOCAL const TIMEZONE_OF_COUNTRY_T s_timezones_of_country_map[] =
{
    //europe
    {202,7200},//Greece
    {204,3600},//Netherlands (Kingdom of the)
    {206,3600},//Belgium
    {208,3600},//France
    {212,3600},//Monaco (Principality of)
    {213,3600},//Andorra (Principality of)
    {214,3600},//Spain 
    {216,3600},//Hungary (Republic of)
    {218,7200},//Bosnia and Herzegovina//???1à-·ò
    {219,7200},//Croatia (Republic of)
    {220,7200},//Serbia (Republic of)
    {222,3600},//Italy
    {225,3600},//Vatican City State
    {226,7200},//Romania
    {228,3600},//Switzerland (Confederation of)
    {230,3600},// Czech Republic,prague
    {231,3600},// Slovak Republic
    {232,3600},// Austria
    {234,0},// United Kingdom
    {238,3600},// Denmark
    {240,3600},// Sweden
    {242,3600},// Norway
    {244,7200},// Finland
    {246,7200},// Lithuania (Republic of),Kaunas
    {247,7200},// Latvia (Republic of),Riga
    {248,7200},// Estonia (Republic of),Talinn
    {250,14400},// Russian Federation,Moscaw +4 10800 
    {255,7200},// Ukraine,Kyiv
    {257,7200},// Belarus (Republic of),minsk
    {259,7200},// Moldova (Republic of)
    {260,3600},// Poland (Republic of)
    {262,3600},// Germany (Federal Republic of)
    {266,3600},// Gibraltar,?±2??Tíó
    {268,0},// Portugal
    {270,3600},// Luxembourg
    {272,0},// Ireland,dublin
    {274,0},// Iceland ,Reykjavik
    {276,3600},// Albania (Republic of)
    {278,3600},// Malta
    {280,7200},// Cyprus (Republic of)
    {282,14400},// Georgia,Tibilisi
    {283,14400},// Armenia (Republic of)
    {284,7200},// Bulgaria (Republic of),sofia
    {288,0},// Faroe Islands
    {290,-10800},//Greenland (Denmark),nuuk
    {292,3600},// San Marino (Republic of)
    {293,3600},// Slovenia (Republic of)
    {294,3600},// The Former Yugoslav Republic of Macedonia
    {295,3600},// Liechtenstein (Principality of)
    {297,3600},// Montenegro (Republic of)

    //North America
    {302,-18000},	// Canada,toranto,OttaWa
    {308,-14400},	// Saint Pierre and Miquelon(FRANCE) (Collectivit¨| territoriale de la R¨|publiquefran?aise)
    {310,-18000},	// United States of America
    {311,-18000},	// United States of America
    {312,-18000},	// United States of America
    {313,-18000},	// United States of America
    {314,-18000},	// United States of America
    {315,-18000},	// United States of America
    {316,-18000},	// United States of America
    {330,-14400},	// Puerto Rico,(SAN Juan)
    {332,-14400},	// United States Virgin Islands
    {334,-21600},	// Mexico, Mexico City
    {338,-18000},	// Jamaica
    {340,-14400},	// Martinique (French Department of)
    {340,-14400},	// Guadeloupe (French Department of)
    {342,-14400},	// Barbados
    {344,-14400},	// Antigua and Barbuda
    {346,-18000},	// Cayman Islands
    {348,-14400},	// British Virgin Islands
    {350,-14400},	// Bermuda
    {352,-14400},	// Grenada
    {354,-14400},	// Montserrat
    {356,-14400},	// Saint Kitts and Nevis
    {358,-14400},	// Saint Lucia
    {360,-14400},	// Saint Vincent and the Grenadines
    {362,-14400},	// Netherlands Antilles
    {363,-14400},	// Aruba
    {364,-18000},	// Bahamas (Commonwealth of the)
    {365,-18000},	// Anguilla
    {366,-14400},	// Dominica (Commonwealth of)?à?×?á??
    {368,-18000},	// Cuba
    {370,-14400},	// Dominican Republic￡??à?×?á?ó
    {372,-18000},	// Haiti (Republic of),Port-au-Prince
    {374,-14400},	// Trinidad and Tobago￡?Port of Spain
    {376,-14400},	// Turks and Caicos Islands

    //Asia
    {400, 14400},	// Azerbaijani Republic,Baku
    {401, 21600},	// Kazakhstan (Republic of),Alamty
    {402, 21600},	// Bhutan (Kingdom of),Thimphu
    {404, 19800},	// India (Republic of),5.5
    {405, 19800},	// India (Republic of)
    {406, 19800},	// India (Republic of)
    {410, 18000},	// Pakistan (Islamic Republic of)
    {412, 16200},	// Afghanistan,4:30
    {413, 18000},	// Sri Lanka (Democratic Socialist Republic of),Colombo
    {414, 23400},	// Myanmar (Union of),Yangon,6.5
    {415, 7200},	// Lebanon,Beirut
    {416, 7200},	// Jordan (Hashemite Kingdom of),amman
    {417, 7200},	// Syrian Arab Republic,damascas
    {418, 10800},	// Iraq (Republic of)
    {419, 10800},	// Kuwait (State of)
    {420, 10800},	// Saudi Arabia (Kingdom of)
    {421, 10800},	// Yemen (Republic of),Aden
    {422, 14400},	// Oman (Sultanate of),Mascut
    {424, 14400},	// United Arab Emirates,Abu Dhabi
    {425, 7200},	// Israel (State of),Jerusalem
    {426, 10800},	// Bahrain (Kingdom of),Manama
    {427, 10800},	// Qatar (State of),Doha
    {428, 28800},	// Mongolia,UlaanBaatar
    {429, 20700},	// Nepal (Federal Democratic Republic of),Kathmandu,5.75
    {430, 14400},	// United Arab Emirates b
    {431, 14400},	// United Arab Emirates b
    {432, 12600},	// Iran (Islamic Republic of),Tehran
    {434, 18000},	// Uzbekistan (Republic of),TashKent
    {436, 18000},	// Tajikistan (Republic of),DushanBe
    {437, 21600},	// Kyrgyz Republic,bishKek
    {438, 18000},	// Turkmenistan,AshGabat
    {440, 32400},	// Japan
    {441, 32400},	// Japan
    {450, 32400},	// Korea (Republic of)
    {452, 25200},	// Viet Nam (Socialist Republic of),+7:00
    {454, 28800},	// Hong Kong, China+8:00
    {455, 28800},	// Macao, China
    {456, 25200},	// Cambodia (Kingdom of),?e±?
    {457, 25200},	// Lao People's Democratic Republic,Vienance
    {460, 28800},	// China (People's Republic of)
    {461, 28800},	// China (People's Republic of)
    {466, 28800},	// Taiwan, China
    {467, 32400},	// Democratic People's Republic of Korea
    {470, 21600},	// Bangladesh (People's Republic of)
    {472, 18000},	// Maldives (Republic of)

    {502, 28800},	// Malaysia,Kuala Pumpur
    {505, 36000},	// Australia,Sidny +10:00
    {510, 25200},	// Indonesia (Republic of),JaKarta
    {514, 32400},	// Democratic Republic of Timor-Leste,??μ???Dili
    {515, 28800},	// Philippines (Republic of the),Manila
    {520, 25200},	// Thailand,BangKok
    {525, 28800},	// Singapore (Republic of)
    {528, 28800},	// Brunei Darussalam(??à3)

    //Oceania
    {530, 43200},	// New Zealand,+12:00
    {536, 43200},	// Nauru (Republic of) ,Yaran
    {537, 36000},	// Papua New Guinea, +10:00
    {539, 45900},	// Tonga (Kingdom of), +12.75, from 2012 is +13:00
    {540, 39600},	// Solomon Islands,+11:00
    {541, 39600},	// Vanuatu (Republic of),Port Vila
    {542, 43200},	// Fiji (Republic of),Suva,+12:00
    {543, 43200},	// Wallis and Futuna (Territoire fran?ais d'outre-mer)靠近Fiji
    {544, -39600},	// American Samoa
    {545, 43200},	// Kiribati (Republic of),Talawa,
    {546, 43200},	// New Caledonia (Territoire fran?ais d'outre-mer)
    {547, -36000},	// French Polynesia (Territoire fran?ais d'outre-mer),Papeete
    {548, -36000},	// Cook Islands,拉落汤加岛,Rarotanga
    {549, -39600},	// Samoa (Independent State of),Apia
    {550, 39600},	// Micronesia (Federated States of),Palikir波赫恩
    {551, 43200},	// Marshall Islands (Republic of the)，马朱罗，majuro
    {552, 32400},	// Palau (Republic of)科罗尔，koror

    //Africa
    {602, 7200},   // Egypt (Arab Republic of),
    {603, 3600},   // Algeria (People's Democratic Republic of),Algiers,阿尔及尔
    {604, 0} ,  	  // Morocco (Kingdom of)，Casablanca，rabat
    {605, 3600},   // Tunisia，tunis
    {606, 7200},   // Libya (Socialist People's Libyan Arab Jamahiriya)，tripoli
    {607, 0},   // Gambia (Republic of the)，Banjul 班珠尔
    {608, 0},   // Senegal (Republic of)，Dakar达喀尔
    {609, 0},   // Mauritania (Islamic Republic of)，nouakchott，努瓦克肖特
    {610, 0},   // Mali (Republic of)，Bamako，巴马科
    {611, 0},   // Guinea (Republic of)，Bissau，conakry
    {612, 0},   // C?te d'Ivoire (Republic of)，阿比让Abidjan
    {613, 0},   // Burkina Faso布基纳法索(Burkina Faso)瓦加杜古(Ouagadougou)
    {614, 3600},   // Niger (Republic of the),Niamey 尼亚美
    {615, 0},   // Togolese Republic，lome
    {616, 3600},   // Benin (Republic of)，Porto Novo
    {617, 14400},   // Mauritius (Republic of)，port Louis+4：00
    {618, 0},   // Liberia (Republic of)，蒙罗维亚，monrivia
    {619, 0},   // Sierra Leone 费里敦，Freetown
    {620, 0},   // Ghana，Accra
    {621, 3600},   // Nigeria (Federal Republic of)
    {622, 3600},   // Chad (Republic of)，恩贾梅纳，Ndjamena
    {623, 3600},   // Central African Republic，班吉，bangui
    {624, 3600},   // Cameroon (Republic of)，雅温得，yaounde
    {625, -3600},   // Cape Verde (Republic of)，Praia，普拉亚
    {626, 3600},   // Sao Tome and Principe (Democratic Republic of)???
    {627, 3600},   // Equatorial Guinea (Republic of),malabo马拉博
    {628, 3600},   // Gabonese Republic加蓬
    {629, 3600},   // Congo (Republic of the)布拉柴维尔
    {630, 3600},   // Democratic Republic of the Congo民主刚果，金沙萨
    {631, 3600},   // Angola (Republic of)，罗安达
    {632, 0},   // Guinea-Bissau (Republic of)
    {633, 14400},   // Seychelles (Republic of)，塞舌尔，victoria
    {634, 10800},   // Sudan (Republic of the)喀土穆
    {635, 7200} ,  // Rwanda (Republic of)基加利
    {636, 10800},   // Ethiopia (Federal Democratic Republic of)亚的斯亚贝巴
    {637, 10800},   // Somali Democratic Republic，摩加迪沙，Mogadishu
    {638, 10800},   // Djibouti (Republic of)吉布提
    {639, 10800},   // Kenya (Republic of)内罗毕，Nairobi
    {640, 10800},   // Tanzania (United Republic of)，达累斯萨拉姆dares salaam
    {641, 10800},   // Uganda (Republic of)，坎帕拉，Kampala
    {642, 7200},   // Burundi (Republic of)布隆迪
    {643, 7200},   // Mozambique (Republic of)马普托，Maputo
    {645, 7200},   // Zambia (Republic of)，卢萨卡，Lusaka
    {646, 10800},   // Madagascar (Republic of)，塔纳那里佛，antananarivo
    {647, 14400},   // French Departments and Territories in the Indian Ocean
    {648, 7200},   // Zimbabwe (Republic of)津巴布韦，哈拉雷(harare)
    {649, 3600},   // Namibia (Republic of),温得和克Windhoek
    {650, 7200},   // Malawi，利隆圭(lilongwe)
    {651, 7200},   // Lesotho (Kingdom of)莱索托，马赛卢maseru
    {652, 7200},   // Botswana (Republic of)，Gaborone哈博罗内
    {653, 7200},   // Swaziland (Kingdom of)斯威士兰，姆巴巴纳(Mbabane)
    {654, 10800},   // Comoros (Union of the)科摩罗，莫罗尼，moronic
    {655, 7200},   // South Africa (Republic of)，cape town
    {657, 10800},   // Eritrea厄立特里亚，阿斯马拉，Asmara

    //Latin America
    {702, -21600},	 // Belize，贝尔莫潘，Belmopan
    {704, -21600},	 // Guatemala (Republic of)危地马拉
    {706, -21600},	 // El Salvador (Republic of)-6;00
    {708, -21600},	 // Honduras (Republic of)特古西加尔巴Tegucigalpa
    {710, -21600},	 // Nicaragua，Managua，马那瓜
    {712, -21600},	 // Costa Rica，圣何塞，san jose
    {714, -18000},	 // Panama (Republic of)巴拿马
    {716, -18000},	 // Peru利马，lima
    {722, -10800},	 // Argentine Republic,buenos Aires
    {724, -10800},	 // Brazil (Federative Republic of)，Brasilia，Sao Paulo，Riode Janeiro
    {730, -14400},	 // Chile，Santiago圣地亚哥
    {732, -18000},	 // Colombia (Republic of)波哥大，Bogota
    {734, -16200},	 // Venezuela (Bolivarian Republic of)，委内瑞拉，加拉加斯，Caracas
    {736, -14400},	 // Bolivia (Plurinational State of)La Paz，-4：00
    {738, -14400},	 // Guyana乔治敦，GeorgeTown
    {740, -18000},	 // Ecuador，Quito，基多
    {742, -14400},	 // French Guiana (French Department of)
    {744, -14400},	 // Paraguay (Republic of)，亚松森，Asuncion
    {746, -10800},	 // Suriname (Republic of)，帕拉马里博，Paramaribo
    {748, -10800},	 // Uruguay (Eastern Republic of)蒙得维的亚Montevideo
    {750, -10800},	 // Falkland Islands (Malvinas)马尔维纳斯群岛
    
};

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN   s_is_user_cancel       = FALSE;    //is user cancel when manul update time
LOCAL uint8          s_sntp_server_addr_index     = 0;    //get sntp server addr index in case some time server is not avalible
LOCAL uint32        s_sntp_net_id            = 0;       //get net id ,if pdp actived, it read from PDP; if on WIN32 ,it is 0
LOCAL MMICONNECTION_SETTING_DETAIL_ITEM_T  s_detail_item ={0};    //remember linksetting detail when PDP release all 
LOCAL uint8 s_dettach_pdp_timer = 0; //dettach wait pdp timer
LOCAL uint8 s_active_next_pdp_timer = 0; //active next wait pdp timer
#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
AUT_PDP_RETRY_T s_active_pdp_retry_info = {0};
#endif
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle time server net pdp msg 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void HandleNetTimeServerPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr);

#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
/*****************************************************************************/
//  Description : handle gprs for nitz active msg 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void HandleNitzPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr);
/*****************************************************************************/
//  Description : need retry pdp if faild in case of dual sim however one influence the other
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL void MMIAPIAUT_SetPdpRetryNum(uint8 num);
/*****************************************************************************/
//  Description : need retry pdp if faild in case of dual sim however one influence the other
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL void MMIAPIAUT_SetPdpRetryDualsys(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description : set action flage
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL void MMIAPIAUT_SetPdpRetryAction(BOOLEAN isAction);
/*****************************************************************************/
//  Description : Get action flage
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPIAUT_GetPdpRetryAction();
/*****************************************************************************/
//  Description : get retry pdp num
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL uint8 MMIAPIAUT_GetPdpRetryNum(void);
/*****************************************************************************/
//  Description : get retry pdp dual sys
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E MMIAPIAUT_GetPdpRetryDualsys(void);

/*****************************************************************************/
// 	Description :Start Pdp active Wait Timer
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void StartPdpDeactiveWaitTimer(void);

/*****************************************************************************/
// 	Description :Start Pdp Active Next Wait Timer
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void StartPdpActiveNextWaitTimer(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void DeactiveNitzPDP(uint8  timer_id, uint32 param);

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void ActiveNextNitzPDP(uint8  timer_id, uint32 param);
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 设置系统时间
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIAUT_SetSysTime(uint32 second)
{
    BOOLEAN  bret   = FALSE;
    //SCI_TM_T Stm    = {0};
    MMI_TM_T mtm    = {0};
    SCI_DATE_T   sysdate      = {0};
    SCI_TIME_T   systime      = {0};
    MMISET_DATE_T   mmiset_date ={0};
    MMISET_TIME_T   mmiset_time ={0};
     
    mtm = MMIAPICOM_Second2Tm(second);

    //SCI_TRACE_LOW:"[mmiaut]:MMIAPIAUT_SetSysTime, Set time = %d-%d-%d %d:%d:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_516_112_2_18_2_5_22_4,(uint8*)"dddddd",mtm.tm_year, mtm.tm_mon, mtm.tm_mday, mtm.tm_hour, mtm.tm_min, mtm.tm_sec);

    sysdate.mday = (uint8)mtm.tm_mday;
    sysdate.mon= (uint8)mtm.tm_mon;
    sysdate.year =(uint16)mtm.tm_year;
    systime.hour = (uint8)mtm.tm_hour;
    systime.min = (uint8)mtm.tm_min;
    systime.sec= (uint8)mtm.tm_sec;
    
    mmiset_date.date_day   = (uint8)mtm.tm_mday;
    mmiset_date.date_month= (uint8)mtm.tm_mon;
    mmiset_date.date_year  = (uint16)mtm.tm_year;
    mmiset_time.time_hour   = (uint8)mtm.tm_hour;
    mmiset_time.time_minute = (uint8)mtm.tm_min;
    
#ifndef WIN32    
    if (MMIAPICOM_CheckDateIsValid(mmiset_date))
    {
        bret = MMIAPICOM_SetSysData(sysdate);
        if (TRUE == bret)
        {
            if(MMIAPICOM_CheckTimeIsValid(mmiset_time))
            {
                bret = MMIAPICOM_SetSysTime(systime);
            }
            else
            {
                return  FALSE;
            }
        }

        if(TRUE == bret)
        {
#ifdef DRM_SUPPORT
            MMIAPIDRM_ResetNetworkTime();
#endif

            MMIAPISET_UpdateAlarmSet();/*更新闹钟的设置*/
#if defined(MMI_WIDGET_WEATHER1) && defined(PDA_WEATHER_TIME_SE)
            MMIAPIWEATHER_HandleTimeUpdate();
#endif //PDA_WEATHER_TIME_SE 

#if  ((defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)) && (defined PDA_TIME_ANIM_SUPPORT)
            MMIAPIWIDGET_Time_HandleTimeUpdate();
#endif
#ifndef SUBLCD_SIZE_NONE
            //刷新小屏的显示，刷新时间
            MMISUB_UpdateDisplaySubLcd(); 
#endif
        }
    }
    else
    {
        return  FALSE;
    }
#endif   

    //SCI_TRACE_LOW:"[mmiaut]:MMIAPIAUT_SetSysTime, status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_574_112_2_18_2_5_22_5,(uint8*)"d",bret);

    return bret;
}

/*****************************************************************************/
// 	Description : 获取系统时间
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMIAUT_GetSysTime(void)
{
    SCI_TM_T sys_time = {0};
    uint32   second   = 0;

    TM_GetTime(&sys_time);
    second = MMIAPICOM_Tm2Second(sys_time.tm_sec, sys_time.tm_min, sys_time.tm_hour, sys_time.tm_mday, sys_time.tm_mon, sys_time.tm_year);

    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetSysTime, Cur time = %d-%d-%d %d:%d:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_591_112_2_18_2_5_22_6,(uint8*)"dddddd",sys_time.tm_year, sys_time.tm_mon, sys_time.tm_mday, sys_time.tm_hour, sys_time.tm_min, sys_time.tm_sec);

    return second;
}

#ifdef MMI_NET_TIME_SUPPORT
/*****************************************************************************/
// 	Description : 更新网络时间
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAUT_UpdateSysTimeByNetTM(uint32 net_second)
{
    
    BOOLEAN       result   = FALSE;
    int                 offset   = 0;
    uint32           net_time_second = 0;
    
    net_time_second = net_second;

    offset = net_second - MMIAUT_GetSysTime();
    
   /*网络时间与机器系统时间相差小于59s 只提示成功不更新时间*/
    if (NETTIME_CMP_CURTIME_OFFSET > abs(offset))
    {
        //manual update pub sussecc win but not update time;auto update, return and do not pub
        if(MMIAPIAUT_IsManualUpdateTimeRunning())
        {
            MMIPUB_OpenAlertSuccessWin(TXT_SET_DTAE_AND_TIME_UPDATED);
        }
        return FALSE;
    }
    
    if (MMIAPIAUT_SetSysTime(net_time_second))
    {
#ifdef DRM_SUPPORT
        MMIAPIDRM_ResetNetworkTime();
#endif
        //  更新状态栏的时间
        MAIN_SetStbDispTime();//update status bar
        MMIAPISET_RefreshTimeDateEditCtrl();
        MMIPUB_OpenAlertSuccessWin(TXT_SET_DTAE_AND_TIME_UPDATED);
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : 从Socket Buffer中获取网络时间
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUT_FindNetTimeInNetBuf(char *SocketBuf, uint32 BufLen, uint32 *net_second)
{
    MMI_TM_T net_time   = {0};
    float    time_zone  = MMI_DEFAULT_TIME_ZONE;
    char     *pStart    = PNULL;
    char     *pEnd      = PNULL;
    char     *pcache_control      = PNULL;
    char     *pcache_control_type      = PNULL;
    char     *found_redirection_flag      = PNULL;
    char     *not_modify_ptr      = PNULL;
    char     *month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
    char     ch_year[NET_TIME_YEAR_LEN + 1] = {0};
    char     ch_mon[NET_TIME_MON_LEN + 1]   = {0};
    char     ch_day[NET_TIME_DAY_LEN + 1]   = {0};
    char     ch_hour[NET_TIME_SEC_LEN + 1]  = {0};
    char     ch_min[NET_TIME_SEC_LEN + 1]   = {0};
    char     ch_sec[NET_TIME_SEC_LEN + 1]   = {0};

    //入参检测
    if (PNULL == SocketBuf || PNULL == net_second)
    {
        //SCI_TRACE_LOW:"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), Param is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_668_112_2_18_2_5_23_7,(uint8*)"");
        return FALSE;
    }
     /* 测试专用，发布时关闭*/
    //MMIAPIAUT_WriteFile(PNULL, SocketBuf, BufLen); 
    //302 Moved Temporarily
    found_redirection_flag =strstr(SocketBuf, NET_URL_REDIRECT_TIPS);
    if(PNULL != found_redirection_flag)     
    {
        //SCI_TRACE_LOW:"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), Moved Temporarily,url has redirected!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_677_112_2_18_2_5_23_8,(uint8*)"");
        return FALSE;

    }
    //304 not modify
    not_modify_ptr =strstr(SocketBuf, NET_URL_NOT_MODIFY_TIPS);
    if(PNULL != not_modify_ptr)     
    {
        //SCI_TRACE_LOW:"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), 304 Not Modified,time should not update!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_685_112_2_18_2_5_23_9,(uint8*)"");
        return FALSE;
    }
    
    pcache_control =strstr(SocketBuf, NET_CACHE_CONTROL_TIPS);
    if (PNULL != pcache_control)
    {
        pcache_control_type = strstr(SocketBuf, NET_CACHE_CONTROL_TYPE_TIPS);
        //SCI_TRACE_LOW:"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf():%s, %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_693_112_2_18_2_5_23_10,(uint8*)"ss", pcache_control, pcache_control_type);
        
        if(PNULL ==pcache_control_type)
        {
            //SCI_TRACE_LOW:"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), web entry from cache!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_697_112_2_18_2_5_23_11,(uint8*)"");
            return FALSE;
        }
    }

    //Date域存在检验
    pStart = strstr(SocketBuf, NET_TIME_START_TIPS);
    if (PNULL != pStart)
    {
        pEnd   = strstr(pStart, NET_TIME_END_TIPS);
    }
    
    if (PNULL == pStart || PNULL == pEnd)
    {
        //SCI_TRACE_LOW:"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), no exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_711_112_2_18_2_5_23_12,(uint8*)"");
        return FALSE;
    }

    //有效检验
    if (pEnd - pStart != NET_TIME_STRING_LEN)
    {
        //SCI_TRACE_LOW:"[MMIAUT]::MMIAPIAUT_FindNetTimeInNetBuf(), error string!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_718_112_2_18_2_5_23_13,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"[MMIAUT]:dave:%s, %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_722_112_2_18_2_5_23_14,(uint8*)"ss", pStart, pEnd);

    //时间项字符串提取
    SCI_MEMCPY(ch_sec, pEnd - 3, NET_TIME_SEC_LEN);
    strcat(ch_sec, "\0");
    SCI_MEMCPY(ch_min, pEnd - 6, NET_TIME_SEC_LEN);
    strcat(ch_min, "\0");
    SCI_MEMCPY(ch_hour, pEnd - 9, NET_TIME_SEC_LEN);
    strcat(ch_hour, "\0");
    SCI_MEMCPY(ch_year, pEnd - 14, NET_TIME_YEAR_LEN);
    strcat(ch_year, "\0");
    SCI_MEMCPY(ch_mon, pEnd - 18, NET_TIME_MON_LEN);
    strcat(ch_mon, "\0");
    SCI_MEMCPY(ch_day, pEnd - 21, NET_TIME_DAY_LEN);
    strcat(ch_day, "\0");

    //转换为时间格式
    net_time.tm_year = atoi(ch_year);
    net_time.tm_mday = atoi(ch_day);
    net_time.tm_hour = atoi(ch_hour);
    net_time.tm_min  = atoi(ch_min);
    net_time.tm_sec  = atoi(ch_sec);

    {
        uint32 i = 0;
        for (i = 0; i < 12; i++)
        {
            if (!memcmp(ch_mon, month[i], NET_TIME_MON_LEN))
            {
                net_time.tm_mon = i + 1;
                break;
            }
        }
    }

    *net_second = MMIAPICOM_Tm2Second(net_time.tm_sec, net_time.tm_min, net_time.tm_hour, net_time.tm_mday, net_time.tm_mon, net_time.tm_year);

    //计算当前时区
#ifdef WORLD_CLOCK_SUPPORT
    //SCI_TRACE_LOW:"[MMIAUT] ::is in summer:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_761_112_2_18_2_5_23_15,(uint8*)"d",MMIAPIACC_IsLocalTimeZoneInSummer());
    if(MMIAPIACC_IsLocalTimeZoneInSummer())
    {
        time_zone = (float)MMIAPIACC_GetLocalTimeZone() + 1;
    }
    else
    {
        time_zone = (float)MMIAPIACC_GetLocalTimeZone();
    }
#endif
    //SCI_TRACE_LOW:"[MMIAUT] ::dave:%2f"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_771_112_2_18_2_5_23_16,(uint8*)"f",time_zone);
    
    //计算有效性
    if (time_zone > TIME_ZONE_MAX || time_zone < TIME_ZONE_MIN)
    {
        time_zone = MMI_DEFAULT_TIME_ZONE;
    }
    //加上时区偏移量
    {
        int32 second_temp = (int32)*net_second;
        second_temp += (int32)(time_zone * 3600);
        *net_second = (uint32)second_temp;
    }

    return TRUE;
}
#endif/*MMI_NET_TIME_SUPPORT*/

/*****************************************************************************/
// 	Description : 自动更新时间相关参数初始化
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_UpdateTimeInit(void)
{
    s_is_user_cancel = FALSE;
    s_sntp_server_addr_index = 0;
    s_sntp_net_id = 0;
    MMIAUT_SetIsNeedAutoUpdateTime(FALSE);
    SCI_MEMSET(&s_detail_item,0x00,sizeof(MMICONNECTION_SETTING_DETAIL_ITEM_T));
    s_detail_item.dual_sys = MMI_DUAL_SYS_MAX;
    s_detail_item.item_index = MMIAUT_MAX_DIRECT_LINK_SETTING_NUM;
}

/*****************************************************************************/
// 	Description : 静态变量复位
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_Reset(void)
{
    s_sntp_server_addr_index = 0;
    s_sntp_net_id = 0;
    MMIAUT_SetIsNeedAutoUpdateTime(FALSE);

    //set item detail
    SCI_MEMSET(&s_detail_item,0x00,sizeof(MMICONNECTION_SETTING_DETAIL_ITEM_T));
    s_detail_item.dual_sys = MMI_DUAL_SYS_MAX;
    s_detail_item.item_index = MMIAUT_MAX_DIRECT_LINK_SETTING_NUM;
    
    MMIAUT_SetUserCancelFlag(FALSE);
    MMIAUT_SetManualUpdateTimeRunning(FALSE);
}

#ifdef MMI_SNTP_SUPPORT
/*****************************************************************************/
//  Description :   Handle SNTP Cnf Msg,SNTP_GET_TIME_SIG_CNF_T
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 处理SNTP反馈消息
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_HandleSNTPCnfMsg(MMI_MESSAGE_ID_E msg_id, void *param)
{
    SNTP_ERROR_E error = SNTP_HD_SUCCESS;
    void * original_param = PNULL;
    BOOLEAN return_val = FALSE;
    
    //SCI_TRACE_LOW:"[MMIAUT] MMIAUT_HandleSNTPCnfMsg msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_834_112_2_18_2_5_23_17,(uint8*)"d", msg_id);
    
    if (PNULL == param)
    {
        //SCI_TRACE_LOW:"[MMIAUT] MMIAUT_HandleSNTPCnfMsg: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_838_112_2_18_2_5_23_18,(uint8*)"");
        return return_val;
    }    
    
    if(MMIAUT_IsUserCancel())
    {
        //SCI_TRACE_LOW:"[MMIAUT] MMIAUT_HandleSNTPCnfMsg: user canceled!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_844_112_2_18_2_5_23_19,(uint8*)"");
        return return_val;
    }
    original_param = (void*)((uint32)param);
    
    switch (msg_id)
    {
        case SNTP_SIG_GET_TIME_CNF:
            {
                //response the ro file of push message
                SNTP_GET_TIME_SIG_CNF_T *get_time_cnf_sig_ptr = PNULL;
                get_time_cnf_sig_ptr = (SNTP_GET_TIME_SIG_CNF_T *)original_param;
                //SCI_TRACE_LOW:"[MMIAUT]: MMIAUT_HandleSNTPCnfMsg error_code=%d, net_time=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_858_112_2_18_2_5_23_20,(uint8*)"dd", get_time_cnf_sig_ptr->error_code, get_time_cnf_sig_ptr->net_time);
                    
                error = get_time_cnf_sig_ptr->error_code;
                //更新成功
                if((SNTP_HD_SUCCESS == error))
                {
                    if(0 != get_time_cnf_sig_ptr->net_time)
                    {
                        BOOLEAN is_nitz_update = FALSE;
                        uint32 nettime_from_1980 = (uint32)get_time_cnf_sig_ptr->net_time - AUT_SNTP_TIME_DIFF_FROM_1970_TO_1980;
                        BOOLEAN is_set_ok = FALSE;
                        is_nitz_update = MMIAPIPHONE_IsNITZUpdated();
                        if (FALSE == is_nitz_update)
                        {
                            is_set_ok = MMIAPIAUT_SetSysTime(nettime_from_1980);
                            //SCI_TRACE_LOW:"[MMIAUT]: MMIAUT_HandleSNTPCnfMsg set systime set_value= %d,nettime_from_1980 =%d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_872_112_2_18_2_5_23_21,(uint8*)"dd", is_set_ok,nettime_from_1980);
                        }
                    }
                    if(MMIAPIAUT_IsManualUpdateTimeRunning())
                    {
                        MMIAPIAUT_CloseManualUpdateWaitWin();
                        //popup success notification even if set_value is not true
                        MMIAPISET_ShowUpdateTimeMsgWin(TRUE,TRUE);
                    }
                    else//Update SNTP when startup
                    {
                        MMIAPIAUT_SetIsNeedUpdateTimeByNet(FALSE);
                        //MMIIDLE_SendMsgToIdleWin( MSG_IDLE_UPDATE_DATETIME, PNULL);//cr132485
                        // TODO: Send msg to clock
                        //  更新状态栏的时间
                        MAIN_SetStbDispTime();//update status bar
                        //  更新表盘日期
#ifdef DYNAMIC_WATCHFACE_SUPPORT
                        WATCHFACE_UpdateDate();
#endif
                        /*if in time&date setting window reset the ctrl id display*/
                        MMIAPISET_RefreshTimeDateEditCtrl();
                    }

                    MMIAUT_DeactivePDPLink();
                }
                else //current SNTP server addr update failed
                {
                     if((MAX_SNTP_SERVER_ADDR-1) > s_sntp_server_addr_index)
                    {
                        //current sntp server addr is not the last one,try next addr
                        s_sntp_server_addr_index++;
                        MMIAUT_GetSNTPTime(s_sntp_server_addr_index);
                    }
                    else
                    {
                        if(MMIAPIAUT_IsManualUpdateTimeRunning())
                        {
                            //尝试完所有服务器地址，提示"连接服务器失败"
                            MMIAPIAUT_CloseManualUpdateWaitWin();
                            MMIPUB_OpenAlertFailWin(TXT_CONNECT_SERVER_FAIL);
                        }
                        MMIAUT_DeactivePDPLink();// deactive PDP
                    }
                }
            }
            break;

        default:
            break;
    }

    return TRUE;    
}

/*****************************************************************************/
//  Description : get timezone index for SNTP
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   local_timezone is  in seconds
/*****************************************************************************/
PUBLIC uint32 MMIAUT_GetTimezoneIndexForSNTP(uint16 mcc, int32 local_timezone)
{
    uint32 timezone_num = ARRAY_SIZE(s_time_zones);
    uint32 dst_rule_num = ARRAY_SIZE(s_rules_map);
    int16 dst_rule_index = -1;
    uint32  timezone_index = AUT_N_ZONES;
    uint8 i = 0;

    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP, mcc =%d,local_timezone=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_916_112_2_18_2_5_23_22,(uint8*)"dd",mcc,local_timezone);
    
    if(AUT_MAX_TIMEZONE_VALUE < local_timezone ||AUT_MIN_TIMEZONE_VALUE > local_timezone)
    {
        //SCI_TRACE_LOW:"[MMIAUT]:MMIAUT_GetTimezoneIndexForSNTP  error local_timezone"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_920_112_2_18_2_5_23_23,(uint8*)"");
        return 0;
    }

    //get dst rule index
    for(i=0;i < dst_rule_num; i++)
    {
        if(s_rules_map[i].mcc == mcc)
        {
            dst_rule_index = s_rules_map[i].z_rules;
            break;
        }
    }
    
    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP dst_rule_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_934_112_2_18_2_5_23_24,(uint8*)"d",  dst_rule_index);
    //获取对应的timezone index
    for(i=0; i< timezone_num; i++)
    {
        if(s_time_zones[i].z_gmtoff == local_timezone
        &&(s_time_zones[i].z_rules == dst_rule_index))
        {
            timezone_index = i;
            break;
        }
    }

    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP timezone_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_946_112_2_18_2_5_23_25,(uint8*)"d",  timezone_index);
    //容错处理，出错时直接找timezone遇到的第一条
    if( AUT_N_ZONES == timezone_index)
    {
        for(i=0; i< timezone_num; i++)
        {
            if(s_time_zones[i].z_gmtoff == local_timezone)
            {
                timezone_index = i;
                break;
            }
        }
    }
    
    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetTimezoneIndexForSNTP timezone_index1 = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_960_112_2_18_2_5_23_26,(uint8*)"d",  timezone_index);
    return timezone_index;
}

/*****************************************************************************/
//  Description : get mcc and timezone value for sntp
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   local_timezone_ptr is  in seconds
//              mcc is from local network service
/*****************************************************************************/
PUBLIC void MMIAUT_GetMccAndTimezone(MN_DUAL_SYS_E dual_sys,uint16* mcc_ptr, int32* local_timezone_ptr)
{
    uint16  timezone_of_country_num = ARRAY_SIZE(s_timezones_of_country_map);
    uint16  default_timezone = 0xFFFF;//this is defined 
    MN_PLMN_T curr_plmn = {0};
    uint16  i = 0;
    
    if(PNULL == mcc_ptr|| PNULL == local_timezone_ptr)
    {
        //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetMccAndTimezone, mcc ||local_timezone is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_978_112_2_18_2_5_23_27,(uint8*)"");
        return;
    }
    
    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetMccAndTimezone, mcc =%d,local_timezone=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_982_112_2_18_2_5_23_28,(uint8*)"dd",*mcc_ptr,*local_timezone_ptr);
    MMIAPIPHONE_GetCurPLMN(&curr_plmn,  dual_sys);

    //if defined world clock, check is the world clock setting right,and if not right ,find loacl timezone from the default list
    {
#ifdef WORLD_CLOCK_SUPPORT
        WT_TIME_CITY_ZONE city_timezone_info = MMIAPIACC_GetLocalTimeZoneCityInfo();   

        if(curr_plmn.mcc == city_timezone_info.mcc ||460 !=city_timezone_info.mcc)
        {
            *mcc_ptr = curr_plmn.mcc;
            *local_timezone_ptr =  (int32)(MMIAPIACC_GetLocalTimeZone()*3600);
            //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetMccAndTimezone2, mcc =%d,local_timezone=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_994_112_2_18_2_5_23_29,(uint8*)"dd",*mcc_ptr,*local_timezone_ptr);
            return;
        }
#endif    
    }
    //get dst rule index
    for(i = 0; i < timezone_of_country_num; i++)
    {
        if(s_timezones_of_country_map[i].mcc == curr_plmn.mcc)
        {
            *local_timezone_ptr = s_timezones_of_country_map[i].timezone;
            default_timezone =  *local_timezone_ptr;
            break;
        }
    }
    
    if(0xFFFF == default_timezone)
    {
        //default use beijing,and timezone is GMT +8:00
        curr_plmn.mcc = 460;
        *local_timezone_ptr = 28800;
    }
    else
    {
        *mcc_ptr =curr_plmn.mcc;
    }
    
    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetMccAndTimezone3, mcc =%d,local_timezone=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1021_112_2_18_2_5_23_30,(uint8*)"dd",*mcc_ptr,*local_timezone_ptr);
}

/*****************************************************************************/
//  Description : active pdp for auto update net time 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAUT_GetSNTPTime(uint8 sntp_server_addr_index)
{
    SNTP_ERROR_E return_val = SNTP_HD_SUCCESS;
    SNTP_GET_TIME_INFO_T net_time_info = {0};
    uint16 mcc = 0;
    int32   local_timezone_value = 0;
    
    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetSNTPTime sntp_server_addr_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1035_112_2_18_2_5_23_31,(uint8*)"d",sntp_server_addr_index);
    
    if(sntp_server_addr_index >= MAX_SNTP_SERVER_ADDR)
    {
        //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetSNTPTime sntp_server_addr_index= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1039_112_2_18_2_5_23_32,(uint8*)"d",sntp_server_addr_index);
        return;
    }

    //current service and timezone value
    MMIAUT_GetMccAndTimezone(s_detail_item.dual_sys, &mcc, &local_timezone_value);
    
#ifdef WIN32
    {
        uint32 ip_addr =0;
        //模拟器上使用本机IP模拟
        if(inet_aton(sntp_server_addr[sntp_server_addr_index], &ip_addr))
        {
            net_time_info.ip_addr= ip_addr;
            net_time_info.is_url_type = FALSE;
        }
        else
        {
            net_time_info.url_addr_ptr = sntp_server_addr[0];
            net_time_info.is_url_type = TRUE;
        }
    }
#else
    net_time_info.url_addr_ptr = sntp_server_addr[sntp_server_addr_index];
    net_time_info.is_url_type = TRUE;
#endif    
    net_time_info.net_id = s_sntp_net_id;//?? which net?
    net_time_info.time_zone = (uint32)MMIAUT_GetTimezoneIndexForSNTP(mcc,local_timezone_value);
    return_val = SNTP_GetNTPTime(&net_time_info);
            
    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_GetSNTPTime GetNTPTime return_val = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1069_112_2_18_2_5_23_33,(uint8*)"d", return_val);
    //获取返回值不对直接提示参数错误
    if (SNTP_HD_SUCCESS != return_val) 
    {
        MMIPUB_OpenAlertWarningWin(TXT_AUT_REF_ERROR_INVALID_PARAM);
    }

}

/*****************************************************************************/
// 	Description : Handle Deactive PDP Prompt WinMsg
//	Global resource dependence : 
//    Author: dave.ruan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAUT_ReleaseAllPDPCallback(void)
{
    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_ReleaseAllPDPCallback detail_item.dual_sys=%d,detail_item.item_index =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1084_112_2_18_2_5_23_34,(uint8*)"dd",s_detail_item.dual_sys,s_detail_item.item_index);
    //MMIAUT_OpenManualUpdateWaitWin();
    if(MMIPDP_LINK_STATE_DEACTIVED == MMIAPIPDP_GetLinkState(MMIAUT_GetModuleID())
        ||MMIPDP_LINK_STATE_MAX== MMIAPIPDP_GetLinkState(MMIAUT_GetModuleID())
    )
    {
        MMIAUT_ActivePDPLink(s_detail_item);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
    }
}
#endif

/*****************************************************************************/
//  Description : get  timezone value by MCC
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   local_timezone_ptr is  in 15 minutes
//              mcc is from local network service
/*****************************************************************************/
PUBLIC void MMIAPIAUT_GetTimezoneByMcc(uint16 mcc, int8* local_timezone_ptr)
{
    uint16  timezone_of_country_num = ARRAY_SIZE(s_timezones_of_country_map);
    int32  default_timezone = 0xFFFF;//this is defined 
    uint16  i = 0;
    
    if(PNULL == local_timezone_ptr)
    {
        return;
    }
    
    SCI_TRACE_LOW("[MMIAUT]::MMIAUT_GetTimezoneByMcc, mcc =%d, local_timezone=%d",mcc, *local_timezone_ptr);

    //get time zone
    for(i = 0; i < timezone_of_country_num; i++)
    {
        if(s_timezones_of_country_map[i].mcc == mcc)
        {
            default_timezone = s_timezones_of_country_map[i].timezone;
            break;
        }
    }
    
    if(0xFFFF == default_timezone)
    {
        //default use 0,and timezone is GMT +0:00
        *local_timezone_ptr = 0;
    }
    else
    {
        //time_zone is in15 mins
        *local_timezone_ptr = (int8) (default_timezone / 900);
    }
    
    SCI_TRACE_LOW("[MMIAUT]::MMIAUT_GetTimezoneByMcc, mcc =%d, local_timezone=%d",mcc, *local_timezone_ptr);
}

/*****************************************************************************/
//  Description : active pdp for auto update net time 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_ActivePDPLink(MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item)
{
    BOOLEAN return_val = FALSE;       

    return_val = MMIAUT_ActiveGPRS(detail_item, HandleNetTimeServerPdpMsg);
    
#if 0
    //if validate data account item is 0, we deactive PDP at once
    {
        uint8   item_num = 0;
        MMICONNECTION_SETTING_DETAIL_ITEM_T internal_detail_item[MMIAUT_MAX_DIRECT_LINK_SETTING_NUM]= {0}; 

#if defined MMI_GPRS_SUPPORT
        MMIAPICONNECTION_GetNoProxyAccountIndex(&item_num,internal_detail_item,MMIAUT_MAX_DIRECT_LINK_SETTING_NUM);
#endif

        if(0 == item_num) //item_num = 0, active PDP for NITZ,if success,deactive PDP at once
        {
            MMI_WIN_ID_T wait_win_id  = MMIAUT_GetManualUpdateWaitWinID();

            if (MMK_IsOpenWin(wait_win_id))
            {
                MMIPUB_OpenAlertWarningWin(TXT_AUT_CURRENT_SERVICE_NO_TIME);
                MMIAPIAUT_CloseManualUpdateWaitWin();
            }
            // deactive PDP 
            MMIAUT_DeactivePDPLink();
        }
    }
#endif  
    return return_val;
}

/*****************************************************************************/
//  Description : handle time server net pdp msg 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void HandleNetTimeServerPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"[MMIAUT]::HandleNetTimeServerPdpMsg msg_ptr->msg_id =%d, msg_ptr->result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1161_112_2_18_2_5_24_39,(uint8*)"dd",msg_ptr->msg_id,msg_ptr->result);
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            uint8   item_num = 0;
            MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item[MMIAUT_MAX_DIRECT_LINK_SETTING_NUM]= {0}; 

#if defined MMI_GPRS_SUPPORT
            MMIAPICONNECTION_GetNoProxyAccountIndex(&item_num,detail_item,MMIAUT_MAX_DIRECT_LINK_SETTING_NUM);
#endif

            if(0 == item_num) //item_num = 0, active PDP for NITZ,if success,deactive PDP at once
            {
                MMI_WIN_ID_T wait_win_id  = MMIAUT_GetManualUpdateWaitWinID();

                if (MMK_IsOpenWin(wait_win_id))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_AUT_CURRENT_SERVICE_NO_TIME);
                    MMIAPIAUT_CloseManualUpdateWaitWin();
                }
                // deactive PDP is fail when PDP active success
                MMIAUT_DeactivePDPLink(); //no use
            }
            else
            {
#ifdef MMI_SNTP_SUPPORT
                //get SNTP 
#ifdef WIN32
                s_sntp_net_id = 0;
#else
                s_sntp_net_id = msg_ptr->nsapi;
#endif            
                s_sntp_server_addr_index = 0;//link SNTP from first addr
                MMIAUT_GetSNTPTime(s_sntp_server_addr_index);
#endif                
            }
            //SCI_TRACE_LOW:"[MMIAUT]::HandleNetTimePdpMsg Active GPRS OK! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1197_112_2_18_2_5_24_40,(uint8*)"");
        }
        else
        {
            MMI_TEXT_ID_T pub_text_id = TXT_DIAL_FAILED;
            
            if(MMIPDP_RESULT_FAIL == msg_ptr->result)
            {
                uint8  item_num = 0;
                MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item[MMIAUT_MAX_DIRECT_LINK_SETTING_NUM]= {0}; 

#if defined MMI_GPRS_SUPPORT
                MMIAPICONNECTION_GetCurSIMNoProxyAccountIndex(s_detail_item.dual_sys, &item_num, detail_item,MMIAUT_MAX_DIRECT_LINK_SETTING_NUM);
#endif

                //item_num >1,and connection get 2 items,and actually,the first is not right,
                //such as 404 68/69,include prepaid and postpaid 2 account
                //here reslove this problem temply
                if((1 < item_num) &&(s_detail_item.item_index ==detail_item[0].item_index))
                {
                    if(MMIAUT_ActivePDPLink(detail_item[1]))
                    {
                        //only if success action then break(only this case can have ps msg), otherwise alert
                        break;
                    }
                }
            }
            else if(MMIPDP_RESULT_DIFFER_DUALSYS == msg_ptr->result)
            {
                MN_DUAL_SYS_E  sim_id = MMIAPIPDP_GetPdpLinkSIM();
                MMI_TEXT_ID_T sim_name_id[MMI_DUAL_SYS_MAX] = {
#if  !defined(MMI_MULTI_SIM_SYS_SINGLE) 
                TXT_SIM_SEL_SIM1,            //SIM1 
                TXT_SIM_SEL_SIM2,            //SIM2
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
                TXT_SIM_SEL_SIM3,            //SIM3 
#if defined(MMI_MULTI_SIM_SYS_QUAD)
                TXT_SIM_SEL_SIM4,            //SIM4 
#endif
#endif
#else
                TXT_SIM_SEL_SIM1,
#endif
                            };

                if(sim_id < MMI_DUAL_SYS_MAX)
                {
                    pub_text_id = sim_name_id[sim_id];
                }
                else
                {
                    pub_text_id = sim_name_id[0];//default is SIM0
                }
            }
            else if(MMIPDP_RESULT_FDN_NOT_PERMIT== msg_ptr->result)
            {
                pub_text_id = TXT_CC_FDN_ONLY;
            }
            else
            {
                pub_text_id = TXT_DIAL_FAILED;
            }

#ifdef WIN32
#ifdef MMI_SNTP_SUPPORT
            s_sntp_net_id = 0;
            s_sntp_server_addr_index = 0;
            MMIAUT_GetSNTPTime(s_sntp_server_addr_index);
#endif
#else
            MMIAPIAUT_CloseManualUpdateWaitWin();
            MMIAUT_DeactivePDPLink();// deactive PDP
            MMIPUB_OpenAlertFailWin(pub_text_id);
#endif
        }
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        //SCI_TRACE_LOW:"[MMIAUT]::HandleNetTimeServerPdpMsg  MMIPDP_DEACTIVE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1270_112_2_18_2_5_24_41,(uint8*)"");
        MMIAPIAUT_CloseManualUpdateWaitWin();
        MMIAUT_Reset();
        break;
        
    case MMIPDP_DEACTIVE_IND:
        //SCI_TRACE_LOW:"[MMIAUT]::HandleNetTimeServerPdpMsg  MMIPDP_DEACTIVE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1275_112_2_18_2_5_24_42,(uint8*)"");
        MMIAPIAUT_CloseManualUpdateWaitWin();
        MMIAUT_Reset();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : deactive pdp link
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_DeactivePDPLink(void)
{
    BOOLEAN return_val = FALSE;

    return_val = MMIAPIPDP_Deactive(MMIAUT_GetModuleID());
    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_DeactivePDPLink  active pdp return_val= %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1293_112_2_18_2_5_24_43,(uint8*)"d", return_val);
}

/*****************************************************************************/
// 	Description : 是否用户取消
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_IsUserCancel(void)
{
    return s_is_user_cancel;
}

/*****************************************************************************/
// 	Description : 设置用户取消操作标志位
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_SetUserCancelFlag(BOOLEAN user_cancel)
{
    s_is_user_cancel = user_cancel;
}

/*****************************************************************************/
// 	Description : get auto update time module id
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAUT_GetModuleID(void)
{
   return MMI_MODULE_FONT;
}

/*****************************************************************************/
// 	Description : get auto update time Sntp Link Detail Item
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMICONNECTION_SETTING_DETAIL_ITEM_T* MMIAUT_GetSntpLinkDetailItem(void)
{
   return &s_detail_item;
}

/*****************************************************************************/
// 	Description : set auto update time Sntp Link Detail Item
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_SetSntpLinkDetailItem(MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item)
{
     s_detail_item.dual_sys = detail_item.dual_sys;
     s_detail_item.item_index= detail_item.item_index;
     //SCI_TRACE_LOW:"SetSntpLinkDetailItem dual_sys= %d, item_index=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1340_112_2_18_2_5_24_44,(uint8*)"dd",s_detail_item.dual_sys, s_detail_item.item_index);
}

/*****************************************************************************/
// 	Description : active GPRS
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_ActiveGPRS(MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item, MMIPDP_MSG_CALLBACK pdp_callback)
{
    BOOLEAN return_val = FALSE;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;

    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_ActivePDPLink dual_sys=%d, item_index= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1109_112_2_18_2_5_24_35,(uint8*)"dd",detail_item.dual_sys,detail_item.item_index);

    s_detail_item.dual_sys = detail_item.dual_sys;
    s_detail_item.item_index = detail_item.item_index;
    
    //if (!MMIAUT_GetIsUseWifi())
    {
        setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(detail_item.dual_sys , detail_item.item_index);
        
        if(setting_item_ptr != PNULL)
        {        
            app_info.app_handler = MMIAUT_GetModuleID();
            app_info.apn_ptr = (char*)setting_item_ptr->apn;
            app_info.user_name_ptr = (char*)setting_item_ptr->username;
            app_info.psw_ptr = (char*)setting_item_ptr->password;
            app_info.dual_sys = detail_item.dual_sys;
            app_info.priority = 3;
            app_info.ps_service_rat = MN_UNSPECIFIED;
            app_info.handle_msg_callback = pdp_callback;
            app_info.ps_service_type = BROWSER_E;
            app_info.storage = MN_GPRS_STORAGE_ALL;

            return_val = MMIAPIPDP_Active(&app_info);
            //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_ActivePDPLink use GPRS"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1131_112_2_18_2_5_24_36,(uint8*)"");
        }
    }
#if 0//def WIFI_SUPPORT
    else
    {
        app_info.app_handler = MMIAUT_GetModuleID();
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
        app_info.handle_msg_callback = HandleNetTimeServerPdpMsg;   
        return_val = MMIAPIPDP_Active(&app_info);
        s_MMI_GSockets.gprs_status = MMI_GPRS_STATUS_CONNECTING;

        //SCI_TRACE_LOW:"MMIAUT::MMIAUT_ActivePDPLink use WIFI!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1143_112_2_18_2_5_24_37,(uint8*)"");
    }
#endif    

    //SCI_TRACE_LOW:"[MMIAUT]::MMIAUT_ActivePDPLink active pdp return_val = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1147_112_2_18_2_5_24_38,(uint8*)"d",  return_val);

    return return_val;
}

#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
/*****************************************************************************/
//  Description : handle gprs for nitz active msg 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void HandleNitzPdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("[MMIAUT]::HandleNitzPdpMsg msg_ptr->msg_id =%d, msg_ptr->result %d", msg_ptr->msg_id, msg_ptr->result);
    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            uint8   item_num = 0;
            MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item[MMIAUT_MAX_DIRECT_LINK_SETTING_NUM]= {0};
            MMIAPIAUT_SetPdpRetryNum(0);
#if defined MMI_GPRS_SUPPORT
            MMIAPICONNECTION_GetNoProxyAccountIndex(&item_num,detail_item,MMIAUT_MAX_DIRECT_LINK_SETTING_NUM);
#endif
            if(0 == item_num) //item_num = 0, active PDP for NITZ,if success,deactive PDP at once
            {
                // deactive PDP is fail when PDP active success
                MMIAUT_DeactivePDPLink(); //no use
            }
            else
            {
#ifdef MMI_SNTP_SUPPORT
                //get SNTP
#ifdef WIN32
                s_sntp_net_id = 0;
#else
                s_sntp_net_id = msg_ptr->nsapi;
#endif
                s_sntp_server_addr_index = 0;//link SNTP from first addr
                MMIAUT_GetSNTPTime(s_sntp_server_addr_index);
#endif
            }
            //SCI_TRACE_LOW:"[MMIAUT]::HandleNitzPdpMsg Active GPRS OK! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_NET_1197_112_2_18_2_5_24_40,(uint8*)"");
        }
        else
        {
            if(MMIPDP_RESULT_FAIL == msg_ptr->result)
            {
                uint8  item_num = 0;
                MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item[MMIAUT_MAX_DIRECT_LINK_SETTING_NUM]= {0};
#if defined MMI_GPRS_SUPPORT
                MMIAPICONNECTION_GetCurSIMNoProxyAccountIndex(s_detail_item.dual_sys, &item_num, detail_item,MMIAUT_MAX_DIRECT_LINK_SETTING_NUM);
#endif
                //item_num >1,and connection get 2 items,and actually,the first is not right,
                //such as 404 68/69,include prepaid and postpaid 2 account
                //here reslove this problem temply
                if((1 < item_num) &&(s_detail_item.item_index ==detail_item[0].item_index))
                {
                    if(MMIAUT_ActivePDPLink(detail_item[1]))
                    {
                        //only if success action then break(only this case can have ps msg)
                        break;
                    }
                }
            }

#ifdef WIN32
#ifdef MMI_SNTP_SUPPORT
            s_sntp_net_id = 0;
            s_sntp_server_addr_index = 0;
            MMIAUT_GetSNTPTime(s_sntp_server_addr_index);
#endif
#else
            MMIAUT_DeactivePDPLink();// deactive PDP
#endif
        }
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        SCI_TRACE_LOW("[MMIAUT]::HandleNitzPdpMsg  MMIPDP_DEACTIVE_CNF");
        {
            BOOLEAN sim_available_num = MMIAPIPHONE_GetSimAvailableNum(PNULL, 0);
            
            SCI_TRACE_LOW("[MMIAUT]::HandleNitzPdpMsg  sim_available_num = %d, isGPRSSwitchedOff = %d", sim_available_num, MMIAPICONNECTION_isGPRSSwitchedOff());
            
            if(MMIAPIPHONE_IsAllSIMSelectNetSucc() 
                && !MMIAPICONNECTION_isGPRSSwitchedOff()
                && (1 < sim_available_num)  //only multi sim need attach next
                && MMIAPIAUT_GetIsNeedUpdateTimeByNet())
            {
                //attach next sim
                if(MMIAPIPHONE_GetAttachingNitzSIM() +1 < MMI_DUAL_SYS_MAX)
                {
                    StartPdpActiveNextWaitTimer();
                }
                else
                {
                    MMIAPIPHONE_SetAttachingNitzSIM(MMI_DUAL_SYS_MAX);
                    MMIAPIPHONE_SetAttachNitzTimeFinish(TRUE);
                }
            }
            else
            {
                MMIAPIPHONE_SetAttachingNitzSIM(MMI_DUAL_SYS_MAX);
                MMIAPIPHONE_SetAttachNitzTimeFinish(TRUE);
            }
            MMIAUT_Reset();
            
            if(MMIAPIAUT_GetPdpRetryNum())
            {
                MMIAPIAUT_SetPdpRetryNum(((uint8)MMIAPIAUT_GetPdpRetryNum())-1);
            }
            MMIAPIAUT_SetPdpRetryAction(FALSE);
        }
        break;
        
    case MMIPDP_DEACTIVE_IND:
        SCI_TRACE_LOW("[MMIAUT]::HandleNitzPdpMsg  MMIPDP_DEACTIVE_IND");
        MMIAUT_Reset();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : need retry pdp if faild in case of dual sim however one influence the other
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL void MMIAPIAUT_SetPdpRetryNum(uint8 num)
{
    SCI_TRACE_LOW("MMIAPIAUT_SetPdpRetryNum active_retry_num:%d,num:%d", s_active_pdp_retry_info.active_retry_num, num);
    s_active_pdp_retry_info.active_retry_num = num;
}
/*****************************************************************************/
//  Description : need retry pdp if faild in case of dual sim however one influence the other
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL void MMIAPIAUT_SetPdpRetryDualsys(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("MMIAPIAUT_SetPdpRetryDualsys dual_sys:%d,dual_sys_in:%d", s_active_pdp_retry_info.dual_sys, dual_sys);
    s_active_pdp_retry_info.dual_sys = dual_sys;
}

/*****************************************************************************/
//  Description : set action flage
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL void MMIAPIAUT_SetPdpRetryAction(BOOLEAN isAction)
{
    SCI_TRACE_LOW("MMIAPIAUT_SetPdpRetryAction isAction:%d,isAction_in:%d", s_active_pdp_retry_info.isAction, isAction);    
    s_active_pdp_retry_info.isAction = isAction;
}
/*****************************************************************************/
//  Description : Get action flage
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIAPIAUT_GetPdpRetryAction()
{
    SCI_TRACE_LOW("MMIAPIAUT_GetPdpRetryAction isAction:%d",s_active_pdp_retry_info.isAction);
    return s_active_pdp_retry_info.isAction;
}
/*****************************************************************************/
//  Description : get retry pdp num
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL uint8 MMIAPIAUT_GetPdpRetryNum(void)
{
    SCI_TRACE_LOW("MMIAPIAUT_GetPdpRetryNum active_retry_num:%d",s_active_pdp_retry_info.active_retry_num);    
    return s_active_pdp_retry_info.active_retry_num;
}
/*****************************************************************************/
//  Description : get retry pdp dual sys
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E MMIAPIAUT_GetPdpRetryDualsys(void)
{
    SCI_TRACE_LOW("MMIAPIAUT_GetPdpRetryDualsys dual_sys:%d",s_active_pdp_retry_info.dual_sys);        
    return s_active_pdp_retry_info.dual_sys;
}

/*****************************************************************************/
//  Description : retry pdp dual sys process
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUT_PdpRetryProcess(MN_DUAL_SYS_E dual_sys)
{
    SCI_TRACE_LOW("MMIAPIAUT_PdpRetryProcess sys:%d,Gsys:%d,Gnum:%d,Gact:%d", dual_sys,MMIAPIAUT_GetPdpRetryDualsys(),MMIAPIAUT_GetPdpRetryNum(),MMIAPIAUT_GetPdpRetryAction());
    if((dual_sys == MMIAPIAUT_GetPdpRetryDualsys()) && (MMIAPIAUT_GetPdpRetryNum()) && !(MMIAPIAUT_GetPdpRetryAction()))
    {
        MMIAPIPHONE_SetAttachingNitzSIM(dual_sys);
        MMIAPIAUT_UpdateNitzTime(dual_sys);
    }
}

/*****************************************************************************/
// 	Description : set auto update time active gprs for NITZ
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIAUT_UpdateNitzTime(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_SETTING_DETAIL_ITEM_T link_item ={0};
    BOOLEAN sim_available_num = MMIAPIPHONE_GetSimAvailableNum(PNULL, 0);
    AUT_PDP_RETRY_T pdp_retry_info = {0};

    link_item.dual_sys= dual_sys;
    link_item.item_index= 0;//use first item

    SCI_TRACE_LOW("MMIAPIAUT_UpdateNitzTime dual_sys = %d, sim_available_num = %d, GetLinkState = %d ", dual_sys, sim_available_num, MMIAPIPDP_GetLinkState(MMIAUT_GetModuleID()));

    if(!MMIAPIAUT_GetPdpRetryAction())
    {
        MMIAPIAUT_SetPdpRetryAction(TRUE);
        MMIAPIAUT_SetPdpRetryDualsys(dual_sys);
        MMIAPIAUT_SetPdpRetryNum(AUT_NUM_PDPRETRY);
    }

    if(MMIPDP_LINK_STATE_DEACTIVED == MMIAPIPDP_GetLinkState(MMIAUT_GetModuleID())
            ||MMIPDP_LINK_STATE_MAX == MMIAPIPDP_GetLinkState(MMIAUT_GetModuleID()))
    {            
        if(MMIAUT_ActiveGPRS(link_item, HandleNitzPdpMsg))
        {
            //if success,deactive PDP right NOW
            //StartPdpDeactiveWaitTimer();//for bug1655309
        }
        else
        {
            if((1 < sim_available_num) && (MMIAPIPHONE_GetAttachingNitzSIM() +1 < MMI_DUAL_SYS_MAX))
            {
                //attach next sim card
                MMIAPIPHONE_GetNitzTime(MMIAPIPHONE_GetAttachingNitzSIM());
            }
            else
            {
                MMIAUT_DeactivePDPLink();
            }
        }
    }
}

/*****************************************************************************/
// 	Description :Start Pdp deactive Wait Timer
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void StartPdpDeactiveWaitTimer(void)
{
    if (0 != s_dettach_pdp_timer)
    {
        MMK_StopTimer(s_dettach_pdp_timer);
        s_dettach_pdp_timer = 0;
    }
    //MMIAUT_11SECOND instead of 5SECOND for PDP reactive
    s_dettach_pdp_timer = MMK_CreateTimerCallback(MMIAUT_11SECOND, DeactiveNitzPDP, PNULL, FALSE);
}

/*****************************************************************************/
// 	Description :Start Pdp Active Next Wait Timer
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void StartPdpActiveNextWaitTimer(void)
{
    //dettach pdp timer stop first
    if (0 != s_dettach_pdp_timer)
    {
        MMK_StopTimer(s_dettach_pdp_timer);
        s_dettach_pdp_timer = 0;
    }
    
    if (0 != s_active_next_pdp_timer)
    {
        MMK_StopTimer(s_active_next_pdp_timer);
        s_active_next_pdp_timer = 0;
    }

    s_active_next_pdp_timer = MMK_CreateTimerCallback(MMIAUT_1SECOND, ActiveNextNitzPDP, PNULL, FALSE);
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void DeactiveNitzPDP(uint8  timer_id, uint32 param)
{
    if (0 != s_dettach_pdp_timer)
    {
        MMIAUT_DeactivePDPLink();
        MMK_StopTimer(s_dettach_pdp_timer);
        s_dettach_pdp_timer = 0;
    }
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//    Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL void ActiveNextNitzPDP(uint8  timer_id, uint32 param)
{
    if (0 != s_active_next_pdp_timer)
    {
        MMIAPIPHONE_GetNitzTime(MMIAPIPHONE_GetAttachingNitzSIM());
        
        MMK_StopTimer(s_active_next_pdp_timer);
        s_active_next_pdp_timer = 0;
    }
}
#endif

#endif /*MMI_UPDATE_TIME_SUPPORT*/
