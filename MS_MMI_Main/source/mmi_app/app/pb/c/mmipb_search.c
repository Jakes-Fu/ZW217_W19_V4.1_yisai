/*****************************************************************************
** File Name:      mmipb_search.c                                            *
** Author:                                                           *
** Date:           23/11/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe mmi phonebook search        *
**                 function .                                                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2004        Lin Lin      Create
******************************************************************************/
#define _MMIPB_SEARCH_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmipb_search.h"
#include "mmipb_adapt.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_app.h"
#include "mmiim.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipb_app.h"
#ifdef FDN_SUPPORT 
#include "mmifdn_export.h"
#endif
#include "mmipb_datalist.h"

#include "mmi_modu_main.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MIN_SEARCH_PHONE_NUMBER_COMPARE_LEN 6
#define MAX_SEARCH_PHONE_NUMBER_LEN         11	// 号码最多匹配后10个数字
#define MAX_SEARCH_PHONE_NUMBER_LOW_LEN     8

#define MMIPB_BASE_CHAR_MAX_LEN             10	
#define MOBILE_PHONE_NUMBER_MAX_LEN         11	// 手机号码的长度
#define MMIPB_COUNTRY_AREA_CODE_MAX_LEN	     6//最长的国家码	
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

 /**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
//LOCAL uint32             s_key_press_number = 0;
LOCAL MMIPB_SEARCH_LIST_T s_pb_number_list = {0};
#ifdef MMIPB_MOST_USED_SUPPORT
LOCAL MMIPB_SEARCH_LIST_T s_pb_mostused_number_list = {0};
#endif
const uint8 s_search_exact_match_byte_table[] = 
{
	0x31, /*mobile number: 13xxxxxxxxxx*/
	0x51,/*mobile number: 15xxxxxxxxxx*/
	0x81 /*mobile number: 18xxxxxxxxxx*/
};

const uint8 s_countryarea_code_table[][MMIPB_COUNTRY_AREA_CODE_MAX_LEN >> 1] = 
{
	{0x53, 0xF5, 0xFF},/*Albania  阿尔巴尼亚  355*/ 
	{0x12, 0xF3, 0xFF},/*Algeria  阿尔及利亚  213*/
	{0x33, 0x26, 0xF8},/* Andorra  安道尔  33628 */
	{0x42, 0xF4, 0xFF},/* Angola  安哥拉  244*/  
	{0x45, 0xFF, 0xFF},/* Argentina  阿根廷  54 */ 
	{0x16, 0xFF, 0xFF},/* Australia  澳大利亚  61 */ 
	{0x34, 0xFF, 0xFF},/* Austria  奥地利  43 */ 
	{0x81, 0x90, 0xFF},/* Bahamas  巴哈马 | Bermuda  百幕大|Cayman Islands  开曼群岛|Barbados  巴巴多斯 1809 */ 
	{0x79, 0xF3, 0xFF},/* Bahrain  巴林  973 */ 
	{0x88, 0xF0, 0xFF},/* Bangladesh  孟加拉国  880 */ 
	{0x55, 0xFF, 0xFF},/* Basil  巴西  55 */ 
	{0x23, 0xFF, 0xFF},/* Belgium  比利时  32*/  
	{0x05, 0xF1, 0xFF},/* Belize  伯利兹  501 */ 
	{0x22, 0xF9, 0xFF},/* Benin  贝宁  229 */ 
	{0x79, 0xF5, 0xFF},/* Bhutan  不丹  975 */ 
	{0x95, 0xF1, 0xFF},/* Bolivia  波利维亚  591*/  
	{0x62, 0xF7, 0xFF},/* Botswana  博茨瓦纳  267 */ 
	{0x76, 0xF3, 0xFF},/* Brunei  文莱  673 */ 
	{0x53, 0xF9, 0xFF},/* Bulgaria  保加利亚  359*/  
	{0x59, 0xFF, 0xFF},/* Burma  缅甸  95 */ 
	{0x52, 0xFF, 0xFF},/* Burundi  布隆迪  257*/  
	{0x32, 0xF7, 0xFF},/* Cameroon  喀麦隆  237 */ 
	{0xF1, 0xFF, 0xFF},/* Canada  加拿大  1 */ 
	{0x43, 0xFF, 0xFF},/* Canary Islands  加那利群岛  34 */ 
	{0x32, 0xF8, 0xFF},/* Cape Verde  佛得角  238 */ 
	{0x32, 0xF6, 0xFF},/* Central Africah Republic  中非共和国  236 */ 
	{0x32, 0xF5, 0xFF},/* Chad  乍得  235 */ 
	{0x65, 0xFF, 0xFF},/* Chile  智利  56 */ 
	{0x68, 0xFF, 0xFF},/* China  中国  86 */ 
	{0x75, 0xFF, 0xFF},/* Colombia  哥伦比亚  57*/  
	{0x42, 0xF2, 0xFF},/* Congo  刚果  242 */ 
	{0x05, 0xF6, 0xFF},/* Costa Rica  哥斯达黎加  506 */ 
	{0x35, 0xFF, 0xFF},/* Cube  古巴  53 */ 
	{0x53, 0xF7, 0xFF},/* Cyprus  塞浦路斯  357 */ 
	{0x24, 0xFF, 0xFF},/* Czechoslovakia  捷克斯洛伐克  42 */ 
	{0x54, 0xFF, 0xFF},/* Danmark  丹麦  45 */ 
	{0x52, 0xF3, 0xFF},/* Djibouti  吉布提  253 */ 
	{0x81, 0x90, 0xFF},/* Dominican Republic  多米尼加国  1809 */ 
	{0x86, 0xF5, 0xFF},/* Eastern Samoa  东萨摩亚  685 */ 
	{0x95, 0xF3, 0xFF},/* Ecuador  厄瓜多尔  593 */ 
	{0x02, 0xFF, 0xFF},/* Egypt  埃及  20 */ 
	{0x05, 0xF3, 0xFF},/* El Salvador  萨尔瓦多  503 */ 
	{0x44, 0xFF, 0xFF},/* England  英国  44 */ 
	{0x42, 0xF0, 0xFF},/* Equatorial Guinea  赤道几内亚  240 */ 
	{0x52, 0xF1, 0xFF},/* Ethiopia  埃塞俄比亚  251 */ 
	{0x03, 0xF6, 0xFF},/* Falkland  福克兰群岛  306 */ 
	{0x76, 0xF9, 0xFF},/* Fiji  斐济  679 */ 
	{0x53, 0xF8, 0xFF},/* Finland  芬兰  358 */ 
	{0x33, 0xFF, 0xFF},/* France  法国  33 */ 
	{0x86, 0xF9, 0xFF},/* French Polynesia  法属波里尼西亚  689 */ 
	{0x42, 0xF1, 0xFF},/* Gabon  加蓬  241 */ 
	{0x22, 0xF0, 0xFF},/* Gambia  冈比亚  220 */
	{0x94, 0xFF, 0xFF},/* Germany  德国  49 */ 
	{0x33, 0xF2, 0xFF},/* Ghana  加纳  233 */ 
	{0x03, 0xFF, 0xFF},/* Greece  希腊  30 */ 
	{0x81, 0x90, 0xFF},/* Grenada  格林纳达  1809 */ 
	{0x76, 0xF1, 0xFF},/* Guam  关岛  671 */ 
	{0x95, 0xF2, 0xFF},/* Guiana  圭亚那  592*/  
	{0x05, 0xF9, 0xFF},/* Haiti  海地  509 */ 
	{0x05, 0xF4, 0xFF},/* Honduras  洪都拉斯  504 */ 
	{0x58, 0xF2, 0xFF},/* Hongkong  香港  852 */ 
	{0x63, 0xFF, 0xFF},/* Hungary  匈牙利  36 */ 
	{0x53, 0xF4, 0xFF},/* Iceland  冰岛  354 */ 
	{0x19, 0xFF, 0xFF},/* India  印度  91 */ 
	{0x26, 0xFF, 0xFF},/* Indonesia  印度尼西亚  62 */ 
	{0x89, 0xFF, 0xFF},/* Iran  伊朗  98 */ 
	{0x69, 0xF4, 0xFF},/* Iraq  伊拉克  964 */ 
	{0x53, 0xF3, 0xFF},/* Irish Republic  爱尔兰  353 */ 
	{0x79, 0xF2, 0xFF},/* Israel  以色列  972 */ 
	{0x93, 0xFF, 0xFF},/* Italy  意大利  39 */ 
	//{0x82, 0x63, 0x03},/* Jamaica  牙买加  1809*/  
	{0x18, 0xFF, 0xFF},/* Japan  日本  81 */ 
	{0x69, 0xF2, 0xFF},/* Jorban  约旦  962 */ 
	{0x22, 0xF4, 0xFF},/* Juinea  几内亚  224 */ 
	{0x58, 0xF5, 0xFF},/* Kampuchea  柬埔寨  855 */ 
	{0x52, 0xF4, 0xFF},/* Kenya  肯尼亚  254 */ 
	{0x58, 0xF0, 0xFF},/* Korea N.  朝鲜民主主义人民共和国  850 */ 
	{0x28, 0xFF, 0xFF},/* Korea S.  韩国  82 */ 
	{0x69, 0xF5, 0xFF},/* Kuwait  科威特  965 */ 
	{0x58, 0x63, 0x03},/* Laos  老挝  856 */ 
	{0x58, 0xF6, 0xFF},/* Lebanon  黎巴嫩  961*/  
	{0x62, 0xF6, 0xFF},/* Lesotho  莱索托  266 */ 
	{0x32, 0xF1, 0xFF},/* Liberia  利比里亚  231 */ 
	{0x12, 0xF8, 0xFF},/* Libya  利比亚  218 */ 
	{0x14, 0xFF, 0xFF},/* Liechtenstein  列支敦士登  41 */ 
	{0x53, 0xF2, 0xFF},/* Luxemburg  卢森堡  352 */ 
	{0x62, 0xF1, 0xFF},/* Madagascar  马达加斯加  261 */ 
	{0x62, 0xF5, 0xFF},/* Malawi  马拉维  265 */ 
	{0x06, 0xFF, 0xFF},/* Malaysia  马来西亚  60 */ 
	{0x69, 0xF0, 0xFF},/* Maldive  马尔代夫  960 */ 
	{0x22, 0xF3, 0xFF},/* Mali  马里  223 */ 
	{0x53, 0xF6, 0xFF},/* Malta  马耳他  356 */ 
	{0x76, 0xF0, 0xFF},/* Mariana Islands  马里亚纳群岛  670 */ 
	{0x95, 0xF6, 0xFF},/* Martinique  马提尼克(法)  596 */ 
	{0x22, 0xF2, 0xFF},/* Mauritania  毛里塔尼亚  222 */ 
	{0x32, 0xF0, 0xFF},/* Mauritius  毛里求斯  230 */ 
	{0x25, 0xFF, 0xFF},/* Mexico  墨西哥  52 */ 
	{0x33, 0x39, 0xFF},/* Monaco  摩纳哥  3393 */ 
	//{0x82, 0x63, 0x03},/* Montserrat  蒙特塞拉特岛(英)  1809 */ 
	{0x12, 0xF2, 0xFF},/* Morocco  摩洛哥  212 */ 
	{0x52, 0xF8, 0xFF},/* Mozambique  莫桑比克  258 */ 
	{0x62, 0xF4, 0xFF},/* Namibia  纳米比亚  264 */ 
	{0x76, 0xF4, 0xFF},/* Nauru  瑙鲁  674 */ 
	{0x79, 0xF7, 0xFF},/*  Nepal  尼伯尔  977 */ 
	{0x86, 0xF7, 0xFF},/* New Caledonia  新喀里多尼亚  687 */ 
	{0x46, 0xFF, 0xFF},/* New Zealand  新西兰  64 */ 
	{0x05, 0xF5, 0xFF},/* Nicaragua  尼加拉瓜  505 */ 
	{0x22, 0xF7, 0xFF},/* Niger  尼日尔  227 */ 
	{0x32, 0xF4, 0xFF},/* Nigeria  尼日利亚  234*/  
	{0x86, 0xF3, 0xFF},/* Niue Island  纽埃岛  683 */ 
	{0x76, 0x32, 0xFF},/* Norfolk Island  诺福克岛  6723 */ 
	{0x74, 0xFF, 0xFF},/* Norway  挪威  47 */ 
	{0x13, 0xFF, 0xFF},/* Ntherland  荷兰  31 */ 
	{0x69, 0xF8, 0xFF},/* Oman  阿曼  968 */ 
	{0x29, 0xFF, 0xFF},/* Pakistan  巴基斯坦  92 */ 
	{0x05, 0xF7, 0xFF},/* Panama  巴拿马  507 */ 
	{0x76, 0xF5, 0xFF},/* Papua New Guiea  巴布亚新几内亚  675 */ 
	{0x95, 0xF5, 0xFF},/* Paraguay  巴拉圭  595 */ 
	{0x15, 0xFF, 0xFF},/* Peru  秘鲁  51 */ 
	//{0x82, 0x63, 0x03},/* Peurto Rico  波多黎各  1809 */ 
	{0x36, 0xFF, 0xFF},/* Philippines  菲律宾  63 */ 
	{0x84, 0xFF, 0xFF},/* Porland  波兰  48 */ 
	{0x53, 0xF1, 0xFF},/* Portugal  葡萄牙  351*/  
	{0x79, 0xF4, 0xFF},/* Qatar  卡塔尔  974 */ 
	{0x62, 0xF2, 0xFF},/* Reunion  留尼旺岛(法)  262 */ 
	{0x04, 0xFF, 0xFF},/* Romania  罗马尼亚  40 */ 
	{0xF7, 0xFF, 0xFF},/* Russia 俄罗斯 7 */
	{0x52, 0xF0, 0xFF},/* Rwanda  卢旺达  250 */ 
	{0x93, 0x45, 0xF9},/* San Marino  圣马力诺  39549 */ 
	{0x32, 0xF9, 0xFF},/* Sao Tome and Principe  圣多美及普林西比  239 */ 
	{0x69, 0xF6, 0xFF},/* Saudi Arabia  沙特阿拉伯  966 */ 
	{0x22, 0xF1, 0x22},/* Senegal  塞内加尔  221 */ 
	{0x42, 0xF8, 0xFF},/* Seychelles  塞舌尔  248 */ 
	{0x32, 0xF2, 0xFF},/* Sierra Leone  塞拉利昂  232*/  
	{0x56, 0xFF, 0xFF},/* Singapore  新加坡  65 */ 
	{0x76, 0xF7, 0xFF},/* Solomon Islands  所罗门群岛  677 */ 
	{0x52, 0xF2, 0xFF},/* Somalia  索马里  252 */ 
	{0x72, 0xFF, 0xFF},/* South Africa  南非  27 */ 
	{0x43, 0xFF, 0xFF},/* Spain  西班牙  34 */ 
	{0x49, 0xFF, 0xFF},/* Sri Lanka  斯里兰卡  94 */ 
	//{0x49, 0xFF, 0xFF},/* St. Kitts-Nevis-Anguilla  圣克里斯托弗和尼维斯  1809 */ 
	//{0x82, 0x63, 0x03},/* St. Lucia  圣卢西亚  1809 */ 
	{0x81, 0x90, 0xFF},/* St. Vincent  圣文森特岛  1809*/  
	{0x42, 0xF9, 0xFF},/* Sudan  苏丹  249 */ 
	{0x95, 0xF7, 0xFF},/* Surinam  苏里南  597*/  
	{0x62, 0xF8, 0xFF},/* Swaziland  斯威士兰  268 */ 
	{0x64, 0xFF, 0xFF},/* Sweden  瑞典  46 */ 
	{0x14, 0xFF, 0xFF},/* Switzerland  瑞士  41 */ 
	{0x69, 0xF3, 0xFF},/* Syria  叙利亚  963 */ 
	{0x88, 0xF6, 0xFF},/* Taiwan  台湾  886 */ 
	{0x52, 0xF5, 0xFF},/* Tanzania  坦桑尼亚  255 */ 
	{0x66, 0xFF, 0xFF},/* Thailand  泰国  66 */ 
	{0x22, 0xF8, 0xFF},/* Togo  多哥  228 */ 
	{0x76, 0xF6, 0xFF},/* Tonga  汤加  676 */ 
	//{0x82, 0x63, 0xFF},/* Trinidad and Tobago  特立尼达和多巴哥  1809 */ 
	{0x12, 0xF6, 0xFF},/* Tunisia  突尼斯  216 */ 
	{0x09, 0xFF, 0xFF},/* Turkey  土耳其  90 */ 
	{0x81, 0x90, 0xFF},/* Turks and Caicos Islands  特克斯和凯科斯群岛(英)  1809 */ 
	{0x62, 0xF9, 0xFF},/* the Comoros  科摩罗  269 */ 
	{0x86, 0xF2, 0xFF},/* the Cook Islands  科克群岛(新)  682 */ 
	{0x79, 0xF1, 0xFF},/* the United Arad Emirates  阿拉伯联合酋长国  971 */ 
	{0x52, 0xF6, 0xFF},/* Uganda  乌干达  256 */ 
	//{0x82, 0x63, 0x03},/* United State of America  美国  1 */ 
	{0x95, 0xF8, 0xFF},/* Uruguay  乌拉圭  598 */ 
	{0x85, 0xFF, 0xFF},/* Venezuela  委内瑞拉  58 */ 
	{0x48, 0xFF, 0xFF},/* Vietnam  越南  84 */ 
	{0x69, 0xF9, 0xFF},/* Yemen  也门  969 */ 
	{0x83, 0xFF, 0xFF},/* Yugoslavia  南斯拉夫  38 */ 
	{0x42, 0xF3, 0xFF},/* Zaire  扎伊尔  243 */ 
	{0x62, 0xF0, 0xFF},/* Zambia  赞比亚  260 */ 
	{0x62, 0xF3, 0xFF},/* Zimbabwe  津巴布韦  263*/  
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//Description : get high value mask and low value mask
// Return: void
/*****************************************************************************/
LOCAL void GetNumberMask(uint16 compare_bcd_len,//IN:bcd len: one byte = 2 
						 uint16 *high_mask_ptr,//OUT
						 uint32 *low_mask_ptr//OUT
						 );

/*****************************************************************************/
//  Description :remove "0"  and Country Area code from head
//  Global resource dependence : none
//  Author: 
//Return: 
/*****************************************************************************/
LOCAL void RemoveZeroAndAreaCode(const MMIPB_BCD_NUMBER_T  *sour_num_ptr, 
								   MMIPB_BCD_NUMBER_T *dest_num_ptr);

/*****************************************************************************/
//  Description : insert item to list and sort list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
LOCAL void InsertHexNumberToList(
                        MMIPB_SEARCH_ITEM_T* item_ptr
                        );

/*****************************************************************************/
//  Description : sort list by phone number
//  Global resource dependence : none
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
LOCAL int ComparePhoneNumber(uint32 base_index, void *compare_data, void *list);

#if 0
/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int CompareLettersInSortTable(uint32 base_index, void *compare_data, void* list);
#endif
/*****************************************************************************/
//  Description : 号码智能匹配
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IntelligentSearch(
                        MMIPB_SEARCH_ITEM_T* item_ptr,
                        uint16* next_pos_ptr,
						uint16 *find_pos_ptr,
						MMIPB_SEARCH_LIST_T *search_list_ptr
                        );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 初始化号码搜索列表。
//  Global resource dependence : s_pb_number_list, s_pb_mostused_number_list
//  Author: baokun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitNumberList(uint32 num_max_count)
{

    if(PNULL == s_pb_number_list.item_ptr)
    {
        s_pb_number_list.item_ptr = (MMIPB_SEARCH_ITEM_T*)SCI_ALLOC_CONST(num_max_count * sizeof(MMIPB_SEARCH_ITEM_T));//maryxiao 分配静态内存改为分配动态内存
    }
    if(PNULL != s_pb_number_list.item_ptr)
    {
        SCI_MEMSET(s_pb_number_list.item_ptr, 0x00, (num_max_count * sizeof(MMIPB_SEARCH_ITEM_T)));
		
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPB]alloc s_pb_number_list.item_ptr failed !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_316_112_2_18_2_44_0_272,(uint8*)"");
    }
    s_pb_number_list.list_inof.count = 0;
	s_pb_number_list.list_inof.max_count = num_max_count;
#ifdef MMIPB_MOST_USED_SUPPORT    
    if(PNULL == s_pb_mostused_number_list.item_ptr)
    {
        s_pb_mostused_number_list.item_ptr = (MMIPB_SEARCH_ITEM_T*)SCI_ALLOC_CONST(MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS * sizeof(MMIPB_SEARCH_ITEM_T));
    }
    if(PNULL != s_pb_mostused_number_list.item_ptr)
    {
        SCI_MEMSET(s_pb_mostused_number_list.item_ptr, 0x00, (MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS * sizeof(MMIPB_SEARCH_ITEM_T)));//maryxiao 分配静态内存改为分配动态内存
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPB]alloc s_pb_mostused_number_list.item_ptr failed !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_330_112_2_18_2_44_0_273,(uint8*)"");
    }
    s_pb_mostused_number_list.list_inof.count = 0;
	s_pb_mostused_number_list.list_inof.max_count = MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS;
#endif
    return;
}
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 初始化号码搜索列表。
//  Global resource dependence : s_pb_number_list, s_pb_mostused_number_list
//  Author: baokun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CleanAllMostusedNumber(void)
{
    if(PNULL != s_pb_mostused_number_list.item_ptr)
    {
        SCI_MEMSET(s_pb_mostused_number_list.item_ptr, 0x00, (MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS * sizeof(MMIPB_SEARCH_ITEM_T)));//maryxiao 分配静态内存改为分配动态内存
    }
    s_pb_mostused_number_list.list_inof.count = 0;
	s_pb_mostused_number_list.list_inof.max_count = MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS;

    return;
}
#endif
/*****************************************************************************/
//  Description : add phone number to search list
//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertNumberToList(
                                const MMIPB_CONTACT_T* entry_ptr
                                )
{
    int32 i = 0;    
    MMIPB_SEARCH_ITEM_T item = {0};
    
    if(PNULL == entry_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] AddPhoneNumberToSearchList PNULL == entry_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_368_112_2_18_2_44_0_274,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }
    for (i =0; i<MMIPB_MAX_NV_PHONE_NUM; i++)
    {
        if(entry_ptr->number[i].number_len > 0)
        {            
            //change number to hex number value
            item.num_index     = i;
            item.storage_id     = entry_ptr->storage_id;        
            item.entry_id       = entry_ptr->contact_id;
            item.npi_ton        = entry_ptr->number[i].npi_ton;
            item.original_len =  MMIPB_GetBCDNumberStrlen(&entry_ptr->number[i]);
            item.number_len     = PhoneNumberToHex(&entry_ptr->number[i], &item.phone_number);
            InsertHexNumberToList(&item);              
        }
    }
    return MMIPB_ERROR_SUCCESS;
}

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : add phone number to search list
//  Global resource dependence : 
//  Author: yao.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertAddtionNumberToList(
                                const MMIPB_CONTACT_T* entry_ptr
                                )
{
    int32 i = 0;    
    MMIPB_SEARCH_ITEM_T item = {0};
    
    if(PNULL == entry_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_InsertAddtionNumberToList PNULL == entry_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_368_112_2_18_2_44_0_274,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }
    for (i =1; i<MMIPB_MAX_NV_PHONE_NUM; i++)
    {
        if(entry_ptr->number[i].number_len > 0)
        {            
            //change number to hex number value
            item.num_index     = i;
            item.storage_id     = entry_ptr->storage_id;        
            item.entry_id       = entry_ptr->contact_id;
            item.npi_ton        = entry_ptr->number[i].npi_ton;
            item.original_len =  MMIPB_GetBCDNumberStrlen(&entry_ptr->number[i]);
            item.number_len     = PhoneNumberToHex(&entry_ptr->number[i], &item.phone_number);
            
            SCI_TRACE_LOW("storage%d,contact_id%d,low%d,high%d",entry_ptr->storage_id,entry_ptr->contact_id,item.phone_number.low,item.phone_number.high);
            InsertHexNumberToList(&item);          
        }
    }
    return MMIPB_ERROR_SUCCESS;
}
#endif
/*****************************************************************************/
//  Description : 获得存储位置所对应的姓名、群组ID、图片ID
//  Global resource dependence : none
//  Parameter:  bcd_number : 输入的电话号码[IN]
//              name_ptr:  姓名[OUT]
//              max_name_len:姓名的最大长度(以字节计)  [IN]
//   RETURN: TRUE, 操作成功；FALSE, 操作失败
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNameByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
                                     MMI_STRING_T *name_ptr,
                                     uint16 max_name_len,
                                     uint16* entry_id_ptr,
                                     uint16* storage_id_ptr,
                                     uint32* group_ptr,
                                     BOOLEAN is_use_default_name,
                                     BOOLEAN is_protect)
{
    BOOLEAN                 result   = FALSE;
    uint16                  entry_id = 0;
    MMI_STRING_T			noname_str = {0};
    uint16                  storage_id = 0;
	MMIPB_NAME_T            *pb_name_ptr = PNULL;
    MMIPB_NAME_NUMBER_MAIL_T base_string = {0};
    MMIPB_CONTACT_T         *contact_ptr = PNULL;

    if(PNULL == bcd_number || PNULL == name_ptr || PNULL == name_ptr->wstr_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetNameByNumber bcd_number %d name_ptr %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_412_112_2_18_2_44_0_275,(uint8*)"dd", bcd_number, name_ptr);
        return FALSE;
    }
    
    if (!MMIAPIPB_IsPbReady())
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetNameByNumber MMIAPIPB_IsPbReady not ready !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_418_112_2_18_2_44_1_276,(uint8*)"");
        return (FALSE);
    }

    result = MMIPB_SearchPhoneNumber(
        bcd_number,
        &entry_id,                                    
        &storage_id,
        PNULL);

    if (result)
    {
        if(
#ifdef FDN_SUPPORT            
            MMIPB_IsFdnContact(storage_id)||            
#endif        
         MMIPB_IsSDNContact(storage_id))
         
        {
            contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(contact_ptr != PNULL)
            {
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr))
                {
                    pb_name_ptr = &contact_ptr->name;
                }
            }
        }
        else            
        {
            MMIPB_ReadContactNameNumberMail(entry_id, storage_id, &base_string);
            pb_name_ptr = &base_string.name;
            if(PNULL != group_ptr)
            {
            
                *group_ptr = base_string.group;
            }
        }
    }

    if(pb_name_ptr != PNULL && (pb_name_ptr->wstr_len > 0))
    {
        name_ptr->wstr_len  = MIN(pb_name_ptr->wstr_len, max_name_len);
        MMIAPICOM_Wstrncpy(name_ptr->wstr_ptr, pb_name_ptr->wstr, name_ptr->wstr_len);
    }
    else
    {
        name_ptr->wstr_len = 0;
        SCI_MEMSET(name_ptr->wstr_ptr, 0x00, max_name_len);
    }

    if (result && name_ptr->wstr_len == 0 && is_use_default_name) 
    {
        MMI_GetLabelTextByLang(
            TXT_NO_NAME, 
            &noname_str
            );
        //name_ptr->is_ucs2 = noname_str.is_ucs2;
        name_ptr->wstr_len  = MIN(noname_str.wstr_len, max_name_len);
        MMIAPICOM_Wstrncpy(name_ptr->wstr_ptr, noname_str.wstr_ptr, name_ptr->wstr_len);  		
    }
    if(PNULL != entry_id_ptr)
    {
        *entry_id_ptr = entry_id;
    }
    if(PNULL != storage_id_ptr)
    {
        *storage_id_ptr = storage_id;
    }
    if((PNULL != group_ptr) && (PNULL != contact_ptr))
    {
        *group_ptr = contact_ptr->union_t.group;
    }
     SCI_TRACE_LOW("[mmipb] MMIPB_GetNameByNumber result(%d) name_len(%d) stoage_id(%d) entry_id(%d)",result,name_ptr->wstr_len, storage_id, entry_id);
    if(contact_ptr != PNULL)
    {
        SCI_FREE(contact_ptr);
    }
   

    return (result);
}

/*****************************************************************************/
//  Description : 获取给定号码对应的号码类型
//  Global resource dependence : none
//  Parameter:  bcd_number : 输入的电话号码[IN]
//              type_ptr:  号码类型[OUT]
//              max_name_len:号码类型字符串的最大长度(以wchar计)  [IN]
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNumberTypeByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
                                           wchar *type_ptr,
                                           uint16 max_type_len)
{
    BOOLEAN result   = FALSE;
    uint16  contact_id = 0;
    uint16  storage_id = 0;
    uint8   num_index = 0;
    uint8   num_type = 0;
    MMIPB_CONTACT_BASE_INFO_T base_info = {0};
    MMI_STRING_T str = {0};

    if(PNULL == bcd_number || PNULL == type_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetNameByNumber bcd_number %d type_ptr %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_497_112_2_18_2_44_1_277,(uint8*)"dd", bcd_number, type_ptr);
        return FALSE;
    }
    
    SCI_MEMSET(type_ptr, 0x00, sizeof(wchar)*max_type_len);
    if (!MMIAPIPB_IsPbReady())
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetNameByNumber MMIAPIPB_IsPbReady not ready !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_504_112_2_18_2_44_1_278,(uint8*)"");
        return (FALSE);
    }

    if(MMIPB_SearchPhoneNumber(
        bcd_number,
        &contact_id,                                    
        &storage_id,
        &num_index)
     )      
    {
       base_info = MMIPB_GetContactBaseInfo(MMIPB_GetContactListIndex(contact_id, storage_id));
       if(num_index < MMIPB_MAX_NV_PHONE_NUM)
       {
           num_type = base_info.num_type[num_index];
       }     
       if(num_type != MMIPB_INVLID_NUMBER)
       {
           MMIPB_GetNumberTypeString(storage_id, num_type, &str);
           if(str.wstr_ptr != PNULL && str.wstr_len > 0)
           {
               MMIAPICOM_Wstrncpy(type_ptr, str.wstr_ptr, MIN(str.wstr_len, max_type_len));
               result = TRUE;
           }
       }

    }   
    return (result);
}

/*****************************************************************************/
//  Description : transfer bcd phone number to hex number
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
uint8 PhoneNumberToHex                          //return phone number length
(
    const MMIPB_BCD_NUMBER_T	*bcd_num_ptr,	//IN:
    SEARCH_PHONE_NUMBER_T       *phone_number_ptr //OUT
)
{
	MMIPB_BCD_NUMBER_T new_number = {0};
    int32   i = 0;
    uint8   bcdCode = 0;
    uint8   len = 0;
	//uint8   zero_len_in_head = 0;
	int8   num_index = 0;
	uint8  max_cmp_len = MAX_SEARCH_PHONE_NUMBER_LEN;//GetMaxCmpLenByCrrentPLMN();
	//BOOLEAN is_head_zero = TRUE;

	
	if(bcd_num_ptr == PNULL || phone_number_ptr == PNULL || bcd_num_ptr->number_len > MMIPB_BCD_NUMBER_MAX_LEN)
	{
		return len;
	}
    if(DIALBCD_EXPANSION == (bcd_num_ptr->number[0] & 0x0F) || (MN_NUM_TYPE_INTERNATIONAL == MMIPB_GetNumberTypeFromUint8(bcd_num_ptr->npi_ton)))
	{
		//first number is "+"
        phone_number_ptr->is_plus_in_head = TRUE;
	}
	else
	{
        phone_number_ptr->is_plus_in_head = FALSE;
	}
    RemoveZeroAndAreaCode(bcd_num_ptr, &new_number);
	if(new_number.number_len == 0)
	{
        if(bcd_num_ptr->number_len == 0)
        {
            return len;
        }
        else
        {
            SCI_MEMCPY(&new_number, bcd_num_ptr, sizeof(new_number));
        }
	}
    phone_number_ptr->high = 0;
    phone_number_ptr->low = 0;
    
    num_index = (new_number.number_len << 1) -1;
	bcdCode = (new_number.number[num_index/2] >> (((num_index+1) & 1) ? 0 : 4)) & 0x0F;
	if((bcdCode & 0x0F) == 0x0F)
	{
		//remove invalid number 0xF
		num_index --;
	}
    for (i= max_cmp_len -1; i >= 0; i--)
    {
		if(num_index >= 0)
		{
			bcdCode = (new_number.number[num_index/2] >> (((num_index+1) & 1) ? 0 : 4)) & 0x0F;
			if(bcdCode == DIALBCD_FILLER)
			{
				//remove first "+"
				bcdCode = 0;
			}
			else
			{
				len++;
 			}
			num_index--;
		}
		else
		{
			bcdCode  = 0;
		}

        if(i >= MAX_SEARCH_PHONE_NUMBER_LOW_LEN)
        {
			//first set high 
			phone_number_ptr->high <<= 4;
			phone_number_ptr->high |= bcdCode;
            //phone_number_ptr->high |= (phone_number_ptr->low & 0xF0000000) >> 28;
        }
        else
		{
			phone_number_ptr->low <<= 4;
			phone_number_ptr->low |= bcdCode;
		}
    }

    phone_number_ptr->high &= 0x0FFF;
    phone_number_ptr->low  &= 0xFFFFFFFF;
    
    //len = len;//(len > MAX_SEARCH_PHONE_NUMBER_LEN) ? MAX_SEARCH_PHONE_NUMBER_LEN : i;
	if(new_number.number_len > 0)
	{
		len = ((0xF0 == (new_number.number[new_number.number_len -1] & 0xF0)) ? ((new_number.number_len << 1) -1) : new_number.number_len << 1);
	}
    return len;
}

/*****************************************************************************/
//  Description : sort list by phone number
//  Global resource dependence : none
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
LOCAL int ComparePhoneNumber(uint32 base_index, void *compare_data, void *list)
{
    MMIPB_SEARCH_ITEM_T *item_base = (MMIPB_SEARCH_ITEM_T *)compare_data;
	MMIPB_SEARCH_LIST_T* search_list_ptr = (MMIPB_SEARCH_LIST_T*)list;
	//uint16 high_mask = 0;
	//uint32 low_mask = 0;

	if(PNULL != search_list_ptr && base_index < search_list_ptr->list_inof.count)
	{
		if(PNULL != item_base && search_list_ptr->item_ptr != PNULL)
		{
			//mask compare number
			//GetNumberMask(GetMaxCmpLenByCrrentPLMN(), &high_mask, &low_mask);
#ifdef PB_PRECISE_MATCH_CTA_SUPPORT
                    if (search_list_ptr->item_ptr[base_index].original_len > item_base->original_len)
                    {
                        return 1;
                    }
                    else if (search_list_ptr->item_ptr[base_index].original_len < item_base->original_len)
                    {
                        return -1;
                    }
                    else
#endif
			if((search_list_ptr->item_ptr[base_index].phone_number.high) > (item_base->phone_number.high))
			{
				return 1;
			}
			else if(search_list_ptr->item_ptr[base_index].phone_number.high < item_base->phone_number.high)
			{
				return -1;
			}	
			else if(search_list_ptr->item_ptr[base_index].phone_number.low > item_base->phone_number.low)
			{
				return 1;
			}
			else if(search_list_ptr->item_ptr[base_index].phone_number.low< item_base->phone_number.low)
			{
				return -1;
			}		
			else
			{
				return 0;
			}
		}
		return 0;
	}
	else
	{
		return -1;
	}

}

/*****************************************************************************/
//  Description : insert item to list and sort list
//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
LOCAL void InsertHexNumberToList(
                        MMIPB_SEARCH_ITEM_T* item_ptr
                        )
{
    uint32 pos = 0;
	//uint16 i = 0;
    uint16 need_moved_num = 0;
    MMIPB_SEARCH_LIST_T *search_list_ptr = PNULL;
    MMI_BIN_SEARCH_INFO_T search_info = {0};
    if(PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] InsertToList PNULL == item_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_709_112_2_18_2_44_1_279,(uint8*)"");
        return;
    }
           
    // 号码排序表移动时必须锁定
#ifdef MMIPB_MOST_USED_SUPPORT    
    if(MMIPB_IsMostUsedContact(item_ptr->storage_id))
	{
        //常用联系人
		search_list_ptr = &s_pb_mostused_number_list;
	}
	else
#endif     
	{
		search_list_ptr = &s_pb_number_list;
	}
	if(search_list_ptr->list_inof.count >= search_list_ptr->list_inof.max_count)
	{
		//SCI_TRACE_LOW:"[MMIPB] InsertToList search_list_ptr->count %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_726_112_2_18_2_44_1_280,(uint8*)"d", search_list_ptr->list_inof.count);
		return;
	}
    if(search_list_ptr->item_ptr == PNULL)
    {
		//SCI_TRACE_LOW:"[MMIPB] InsertToList search_list_ptr->item_ptr == PNULL !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_731_112_2_18_2_44_1_281,(uint8*)"");
        return;
    }
    search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
    search_info.end_pos = search_list_ptr->list_inof.count -1;
    search_info.compare_base_data = item_ptr;
    MMIAPICOM_BinSearch(&search_info, (BIN_COMPARE_FUNC)ComparePhoneNumber, &pos, search_list_ptr);
    
    if(pos != search_list_ptr->list_inof.count )
    {
        need_moved_num = search_list_ptr->list_inof.count - pos;
        memmove(&search_list_ptr->item_ptr[pos+1], 
                &search_list_ptr->item_ptr[pos], 
                need_moved_num * sizeof(MMIPB_SEARCH_ITEM_T));
    }
    //if(&search_list_ptr->item_ptr[pos] != PNULL)
    {
        SCI_MEMCPY(&search_list_ptr->item_ptr[pos], item_ptr, sizeof(MMIPB_SEARCH_ITEM_T));
        search_list_ptr->list_inof.count++;        
    }
    // 释放
}

/*****************************************************************************/
//  Description : 号码智能匹配
//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IntelligentSearch(
                        MMIPB_SEARCH_ITEM_T* item_ptr,
                        uint16* next_pos_ptr,
						uint16 *find_pos_ptr,
						MMIPB_SEARCH_LIST_T *search_list_ptr
                        )
{
    uint32 pos = 0;
	uint32 match_pos = 0;
	uint32 most_match_pos = 0;
    uint32 start_pos = 0;
	uint32 last_pos = 0;
	//int32  i = 0;
	uint32  j = 0;
	int32  result = 0;
	//uint32 low_mask = 0;
	//uint16 high_mask = 0;
    MMI_BIN_SEARCH_INFO_T search_info = {0};
    BOOLEAN is_find = FALSE;    
	uint8   num_len = 0;

    if(PNULL == item_ptr || PNULL == next_pos_ptr || PNULL == search_list_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] IntelligentSearch item_ptr 0x%x next_pos_ptr 0x%x !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_781_112_2_18_2_44_1_282,(uint8*)"dd", item_ptr, next_pos_ptr);
        return FALSE;
    }    
    //SCI_TRACE_LOW:"[baokun] item_ptr len %d high 0x%x, low 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_784_112_2_18_2_44_1_283,(uint8*)"ddd", item_ptr->number_len, item_ptr->phone_number.high, item_ptr->phone_number.low);
    if(search_list_ptr->list_inof.count > 0 && search_list_ptr->item_ptr != PNULL)
    {
        search_info.search_type = MMICOM_SEARCH_FIRST_EQUAL;
        search_info.end_pos = search_list_ptr->list_inof.count -1;
        search_info.compare_base_data = item_ptr;
        result = MMIAPICOM_BinSearch(&search_info, (BIN_COMPARE_FUNC)ComparePhoneNumber, &pos, search_list_ptr);
        match_pos = pos;
        if(0 == result)
        {
            //match_pos = pos;
            is_find = TRUE;
        }
        else if(result < 0 && (match_pos > 0))
        {
            match_pos--;
        }
        if((match_pos < search_list_ptr->list_inof.count)
             &&(
                !is_find && (find_pos_ptr != PNULL)  
                ||item_ptr->number_len != search_list_ptr->item_ptr[match_pos].number_len
                )
           )
        {
            //没有完全匹配，要找个最匹配的一个
            //是否查找的号码包含在某个号码中
            if(!MMIPB_IsExactMatchNumber(item_ptr->phone_number, item_ptr->number_len))
            {
                //被查找的号码不是需要精确匹配的号码
                //find first large than this number
                if(result > 0 && (match_pos > 0))
                {
                    start_pos = match_pos -1;
                }
                else
                {
                    start_pos = match_pos;
                }
                search_info.search_type = MMICOM_SEARCH_LAST_EQUAL;
                search_info.end_pos = search_list_ptr->list_inof.count -1;
                search_info.compare_base_data = item_ptr;
                result = MMIAPICOM_BinSearch(&search_info, (BIN_COMPARE_FUNC)ComparePhoneNumber, &last_pos, search_list_ptr);						                
				//Remove below 4 lines code for Bug 112749
				/*
				if(result < 0 && (last_pos > 0))
				{
				  last_pos--;
				}
				*/
                most_match_pos = last_pos;
                num_len = item_ptr->number_len;
                for(j = start_pos; j <= last_pos; j++)
                {
                    //first check current match or not
                    if(MMIPB_IsNumberPartialMatch(&search_list_ptr->item_ptr[j], item_ptr))
                    {
                        is_find = TRUE;
                        //找到一个长度最接近的匹配号码
                        if(search_list_ptr->item_ptr[j].number_len >= item_ptr->number_len)
                        {
                            if(num_len > (search_list_ptr->item_ptr[j].number_len - item_ptr->number_len))
                            {
                                num_len = (search_list_ptr->item_ptr[j].number_len - item_ptr->number_len);
                                most_match_pos = j;
                            }
                        }
                        else
                        {
                            if(num_len > (item_ptr->number_len - search_list_ptr->item_ptr[j].number_len))
                            {
                                num_len = (item_ptr->number_len - search_list_ptr->item_ptr[j].number_len);
                                most_match_pos = j;
                            }
                        }
                        //break;
                    }
                }
                match_pos = most_match_pos;
            }
            
        }	//if(number_len)
    }
	if(is_find && (find_pos_ptr != PNULL))
	{
		*find_pos_ptr = match_pos;
	}
    *next_pos_ptr = pos;
    if(is_find)
    {
        if(item_ptr->original_len < MIN_SEARCH_PHONE_NUMBER_COMPARE_LEN
          ||search_list_ptr->item_ptr[match_pos].original_len < MIN_SEARCH_PHONE_NUMBER_COMPARE_LEN
          )//两个匹配的号码中其中有个号码长度 < 6
        {
            //要求两个号码完全匹配
            if(item_ptr->original_len != search_list_ptr->item_ptr[match_pos].original_len)
            {
                //两个号码不是完全匹配
                is_find = FALSE;
            }
        }            
    }
	return is_find;
}

/*****************************************************************************/
//  Description : Search phone number in useful contacts list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_SearchPhoneNumber(
                                MMIPB_BCD_NUMBER_T  *bcd_num_ptr, //IN: 源号码
                                uint16* entry_id,                 //OUT: entry_id
                                uint16* storage_id,               //OUT: storage_id
                                uint8*  num_index_ptr             //OUT: num_index
                                )
{
    MMIPB_SEARCH_ITEM_T key_item   = {0};
    BOOLEAN recode = FALSE;
    uint16 next_index = 0;
    uint16 find_index = 0;
//#ifdef FDN_SUPPORT 
    MMIPB_HANDLE_T find_handle = 0;
//#endif
    MN_DUAL_SYS_E dual_sys = 0;
    uint16 find_storage_id = 0;
    MMIPB_CONTACT_BASE_INFO_T find_data = {0};
    MMIPB_ERROR_E find_ret = 0;
    
    MMIPB_CONTACT_T *contact_ptr = PNULL;
	if((0 == bcd_num_ptr->number_len))
	{
		return FALSE;
	}
    contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(contact_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_SearchPhoneNumber alloc failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_885_112_2_18_2_44_1_284,(uint8*)"");
        return FALSE;
    }
    //first search in fdn
#ifdef FDN_SUPPORT 
    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX && (!recode); dual_sys++)
    {
        if(MMIAPIFDN_Is2GFdnEnable(dual_sys))
        {
            find_storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1+dual_sys, PHONEBOOK_FDN_STORAGE);
            find_ret = MMIPB_FindFirstContact(MMIPB_ALL_GROUP_MASK, 
                                   find_storage_id, 
                                   &find_index, 
                                   PNULL, 
                                   &find_data,
                                   &find_handle);
            while(MMIPB_ERROR_SUCCESS == find_ret)
            {                
                //compare number
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(find_data.contact_id, find_storage_id, contact_ptr))
                {
                    if(!MMIPB_BcdSmartCmp(bcd_num_ptr, &contact_ptr->number[0]))
                    {
                        //find matched contact
                        recode = TRUE;
                        if(entry_id != PNULL)
                        {
                            *entry_id = find_data.contact_id;
                        }
                        if(storage_id != PNULL)
                        {
                            *storage_id = find_storage_id; 
                        }
                        if(num_index_ptr != PNULL)
                        {
                            *num_index_ptr = 0;
                        }
                        break;
                    }
                }
                SCI_MEMSET(&find_data, 0x00, sizeof(MMIPB_CONTACT_BASE_INFO_T));
                find_ret = MMIPB_FindNextContact(find_handle, 
                                                MMIPB_ALL_GROUP_MASK, 
                                                find_storage_id,
                                                &find_index,
                                                PNULL,
                                                &find_data);
            }
            MMIPB_FindClose(find_handle);
            find_handle = 0;
        }
    }    
#endif

#ifndef CMCC_UI_STYLE
    //then search in sdn
    if(0 < s_pb_number_list.list_inof.count)
    {
        for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX && (!recode); dual_sys++)
        {
#ifndef MMI_DUALMODE_ENABLE
            if(MNSIM_GetSdnStatusEx(dual_sys))
#endif
            {
                find_storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1+dual_sys, PHONEBOOK_SDN_STORAGE);
                find_ret = MMIPB_FindFirstContact(MMIPB_ALL_GROUP_MASK, 
                    find_storage_id, 
                    &find_index, 
                    PNULL, 
                    &find_data,
                    &find_handle);
                while(MMIPB_ERROR_SUCCESS == find_ret)
                {                
                    //compare number
                    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                    if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(find_data.contact_id, find_storage_id, contact_ptr))
                    {
                        if(!MMIAPIPB_BCDCmp(bcd_num_ptr, &contact_ptr->number[0]))
                        {
                            //find matched contact
                            recode = TRUE;
                            if(entry_id != PNULL)
                            {
                                *entry_id = find_data.contact_id;
                            }
                            if(storage_id != PNULL)
                            {
                                *storage_id = find_storage_id; 
                            }
                            if(num_index_ptr != PNULL)
                            {
                                *num_index_ptr = 0;
                            }
                            break;
                        }
                    }
                    SCI_MEMSET(&find_data, 0x00, sizeof(MMIPB_CONTACT_BASE_INFO_T));
                    find_ret = MMIPB_FindNextContact(find_handle, 
                        MMIPB_ALL_GROUP_MASK, 
                        find_storage_id,
                        &find_index,
                        PNULL,
                        &find_data);
                }
                MMIPB_FindClose(find_handle);
            }
        }
    }
#endif

    SCI_FREE(contact_ptr);
    //search in adn and phone contact
    if(!recode)
    {
        key_item.npi_ton      = bcd_num_ptr->npi_ton;
        key_item.number_len     =   PhoneNumberToHex(bcd_num_ptr, &key_item.phone_number);
        key_item.original_len =  MMIPB_GetBCDNumberStrlen(bcd_num_ptr);
        if(IntelligentSearch(&key_item, &next_index, &find_index, &s_pb_number_list) && s_pb_number_list.item_ptr != PNULL)
        {
            if(entry_id != PNULL)
            {
                *entry_id = s_pb_number_list.item_ptr[find_index].entry_id;
            }
            if(storage_id != PNULL)
            {
                *storage_id = s_pb_number_list.item_ptr[find_index].storage_id; 
            }
            if(num_index_ptr != PNULL)
            {
                *num_index_ptr = s_pb_number_list.item_ptr[find_index].num_index;
            }
            recode = TRUE;    
        }
    }
    if(PNULL != s_pb_number_list.item_ptr)
    {
    
        SCI_TRACE_LOW("[mmipb] MMIPB_SearchPhoneNumber recode(%d) find_index(%d) entry_id(%d) storage_id(%d) num_index(%d)",
                    recode,find_index,s_pb_number_list.item_ptr[find_index].entry_id,s_pb_number_list.item_ptr[find_index].storage_id,
                    s_pb_number_list.item_ptr[find_index].num_index);
    }
    return recode;
}
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : Search phone number in list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_SearchPhoneNumberInMostUsedlist(
                                MMIPB_BCD_NUMBER_T  *bcd_num_ptr,   //IN: 源号码
                                uint16* entry_id,                   //OUT: entry_id
                                uint16* storage_id                   //OUT: entry_id
                                )
{
    BOOLEAN recode = FALSE;
    MMIPB_SEARCH_ITEM_T key_item = {0};
    MMI_BIN_SEARCH_INFO_T search_info = {0};
    uint32   pos  = 0;
	
    if(bcd_num_ptr == PNULL || entry_id == PNULL || storage_id == PNULL)
	{
		return FALSE;
	}



	if((0 == bcd_num_ptr->number_len) || (0 == s_pb_mostused_number_list.list_inof.count))
	{
		return FALSE;
	}

    key_item.original_len =  MMIPB_GetBCDNumberStrlen(bcd_num_ptr);
    key_item.npi_ton      = bcd_num_ptr->npi_ton;
    key_item.number_len     =   PhoneNumberToHex(bcd_num_ptr, &key_item.phone_number);

	if(s_pb_mostused_number_list.list_inof.count > 0 && s_pb_mostused_number_list.item_ptr != PNULL)
	{
		search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
		search_info.end_pos = s_pb_mostused_number_list.list_inof.count -1;
		search_info.compare_base_data = &key_item;
		recode = MMIAPICOM_BinSearch(&search_info, (BIN_COMPARE_FUNC)ComparePhoneNumber, &pos, &s_pb_mostused_number_list);
        if((0 == recode) && (pos < s_pb_mostused_number_list.list_inof.count))
        {
            *entry_id = s_pb_mostused_number_list.item_ptr[pos].entry_id;
            *storage_id = s_pb_mostused_number_list.item_ptr[pos].storage_id;
            recode = TRUE;
        }
        else
        {
            recode = FALSE;
        }
	}    


    return recode;
}                 
#endif
/*****************************************************************************/
//  Description : delete phone number from search list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DelNumberFromList(
                                        uint16 entry_id,	//IN: 待删除记录的记录号
                                        uint16 storage_id,  //IN: 待删除记录的storage_id                                        
                                        uint8* num_index_ptr //IN: 删除的号码index,如果为空，表示删除该记录对应的所有号码
                                        )
{
    int32				  i = 0;
    MMIPB_SEARCH_ITEM_T  *item_ptr = PNULL;
	MMIPB_SEARCH_LIST_T *search_list_ptr = PNULL;
    uint32 cur_index = 0;
    uint32 del_count = 0;
    BOOLEAN is_valid = FALSE;
#ifdef MMIPB_MOST_USED_SUPPORT    
    if(MMIPB_IsMostUsedContact(storage_id))
	{
		//删除常用联系人号码查找表
		search_list_ptr = &s_pb_mostused_number_list;
	}
	else
#endif     
	{
		search_list_ptr = &s_pb_number_list;
	}
    if(search_list_ptr->item_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_DelNumberFromList search_list_ptr->item_ptr == PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_1094_112_2_18_2_44_2_285,(uint8*)"");
        return FALSE;
    }
	for (i=0; i<search_list_ptr->list_inof.count; i++)
	{
        is_valid = TRUE;
		item_ptr = &search_list_ptr->item_ptr[i];
        if((item_ptr->entry_id == entry_id) 
           && storage_id == item_ptr->storage_id
           &&(num_index_ptr == PNULL || *num_index_ptr == item_ptr->num_index)
           ) 
		{
            //是要删除的号码
            is_valid = FALSE;
            del_count++;
		}
        if(is_valid)
        {
            if(del_count > 0)
            {
                search_list_ptr->item_ptr[cur_index] = search_list_ptr->item_ptr[i];
            }
            cur_index++;
        }
	}
    search_list_ptr->list_inof.count = search_list_ptr->list_inof.count - del_count; 
    return TRUE;
}

/*****************************************************************************/
//  Description :判断该号码是否需要精确匹配
//  Global resource dependence : none
//  Author: 
//Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsExactMatchNumber(SEARCH_PHONE_NUMBER_T phone_number, uint16 num_len)
{
#ifndef PB_PRECISE_MATCH_CTA_SUPPORT
	BOOLEAN result = FALSE;
    uint32  i  = 0;
	if(phone_number.is_plus_in_head)
	{
		//是以"+"开始的号码，“+”开头的号码不需要精确匹配
		return FALSE;
	}
	for(i = 0; i < (ARR_SIZE(s_search_exact_match_byte_table)); i++)
	{
		if((phone_number.low & 0x000000ff) == s_search_exact_match_byte_table[i] && (num_len == MOBILE_PHONE_NUMBER_MAX_LEN))
		{
			//以"13","15","18"开头的号码需要精确匹配
			result = TRUE;
			break;
		}
	}
	return result;
#else
    return TRUE;
#endif
}

/*****************************************************************************/
//  Description :check two number is partial matched
//  Global resource dependence : none
//  Author: 
//Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsNumberPartialMatch(MMIPB_SEARCH_ITEM_T* base_item_ptr, MMIPB_SEARCH_ITEM_T* table_item_ptr)
{
    uint8 compare_num_len = 0;
	uint16 high_mask = 0;
	//uint16 max_cmp_len = GetMaxCmpLenByCrrentPLMN();
	uint32 low_mask = 0;
	//uint16 i = 0;
	BOOLEAN is_find = FALSE;
	
	if(PNULL == base_item_ptr ||PNULL == table_item_ptr)
	{
		return FALSE;
	}

	if(!MMIPB_IsExactMatchNumber(base_item_ptr->phone_number, base_item_ptr->number_len))
	{
		//列表中号码是不需要精确匹配的号码
		//compare_num_len取两个不需要精确匹配的最小的长度作为匹配长度
		compare_num_len = MIN(base_item_ptr->number_len, table_item_ptr->number_len);
        if(compare_num_len >= MIN_SEARCH_PHONE_NUMBER_COMPARE_LEN)
        {
            //compare_num_len的最小长度不能小于6
            compare_num_len = MAX(MIN_SEARCH_PHONE_NUMBER_COMPARE_LEN, compare_num_len);
            GetNumberMask(compare_num_len, &high_mask, &low_mask);
            
            if((table_item_ptr->phone_number.high & high_mask) 
                == (base_item_ptr->phone_number.high & high_mask)
                )
            {
                if((table_item_ptr->phone_number.low & low_mask) 
                    == (base_item_ptr->phone_number.low & low_mask)
                    )
                {
                    is_find = TRUE;
                }
            }
        }
	}
	
	return is_find;
}

/*****************************************************************************/
//  Description : 该函数实现模糊查找
// str_input：用户的字符串
// group：要查找的记录的所有group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示
/*****************************************************************************/
PUBLIC uint32 MMIPB_MultiSearch
(
     MMI_STRING_T *str_input,//IN
     uint32 group,//IN
     uint32 exclude_group,
     uint32 search_type
)
{
    uint32 count = 0;
//#ifndef MMI_PDA_SUPPORT
//    uint32 i = 0;
//    uint32 j = 0;
//#endif
    MMI_STRING_T wchar_lower = {0};

	if(!MMIPB_IsPbReady())
	{
		return 0;
	}

    if(PNULL == str_input)
    {
        return 0;
    }
    
    MMIPB_CleanVisibleTable(group);
#ifdef MMIPB_MOST_USED_SUPPORT    
    if((1<<PB_GROUP_MOSTUSED) != group)
#endif        
    {

        wchar_lower.wstr_len = str_input->wstr_len;
        wchar_lower.wstr_ptr = SCI_ALLOCA((str_input->wstr_len + 1)*sizeof(wchar));
        if(PNULL !=  wchar_lower.wstr_ptr)
        {
            SCI_MEMSET(wchar_lower.wstr_ptr, 0x00, (str_input->wstr_len + 1)*sizeof(wchar));
            if(str_input->wstr_len > 0)
            {
                MMIAPICOM_Wstrncpy(wchar_lower.wstr_ptr, str_input->wstr_ptr, str_input->wstr_len);
                MMIAPICOM_Wstrlower(wchar_lower.wstr_ptr);                
            }
        }
        MMIPB_SearchQSortList(&wchar_lower, group, exclude_group, TRUE);
        count = MMIPB_SearchSubStringInList(wchar_lower.wstr_ptr,search_type, TRUE, group, exclude_group);
        if(wchar_lower.wstr_ptr != PNULL)
        {
            SCI_FREE(wchar_lower.wstr_ptr);
        }
    }
#ifdef MMIPB_MOST_USED_SUPPORT
	else 
	{
        //常用联系人        
		count = MMIPB_GetAllMostUsedContactCount();//MMIPB_SearchSubStringInList(str_input->wstr_ptr, MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_NAME, group_id, exclude_group);
	}
    #endif
	//SCI_TRACE_LOW:"MMIPB_MultiSearch group %d count %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_1307_112_2_18_2_44_2_286,(uint8*)"dd",group, count);
	return count;   
}

/*****************************************************************************/
//  Description : 根据输入的字符串查找号码中包含该串的记录和姓名中按照首字母匹配的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 找到的号码的个数
/*****************************************************************************/
PUBLIC uint32 MMIPB_SearchList(MMI_STRING_T *search_str, 
                               uint32 field_type, 
                               MMIPB_SEARCH_TYPE_T search_type, 
                               MMIPB_HANDLE_T *handle_ptr//[OUT]
                               )
{
     uint16 i = 0;
     uint16 index = 0;
     BOOLEAN is_char = FALSE;
     uint16 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE ,MMIPB_NORMAL_PHONE_CONTACT);
     uint8  num_index  = 0;
     uint16 count = 0; // Bug-1369485
     MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T valid_table = {0};
     MMIPB_CONTACT_BASE_INFO_T contact_node = {0};
     MMIPB_VISIBLE_INDEX_TABLE_T *tab_ptr = PNULL;
     MMIPB_NAME_NUMBER_MAIL_T *base_str = PNULL;
     MMI_STRING_T wchar_lower = {0};
	 //SCI_TRACE_LOW:"[MMIPB]MMIPB_SearchList enter %d "
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_1336_112_2_18_2_44_2_287,(uint8*)"d");
	 if((!MMIPB_IsPbReady()))
	 {
		 //SCI_TRACE_LOW:"[mmipb] MMIPB_SearchList not ready !"
		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_1339_112_2_18_2_44_2_288,(uint8*)"");
		 return 0;
	 }
	 
	 if(search_str == PNULL 
         ||(PNULL == search_str->wstr_ptr)
         ||search_str->wstr_len == 0)
     {
         //show all contacts
         MMIPB_EnableAllContactVisible();
         count = MMIPB_GetAllNormalContactCount();
     }
     else
     {
         MMIPB_CleanVisibleTable(MMIPB_ALL_GROUP_MASK);
         //search in name
         if(MMIAPICOM_IsNumberString(search_str->wstr_ptr, search_str->wstr_len))
         {
            is_char = FALSE;
         }
         else
         {
            is_char = TRUE;            
         }
         wchar_lower.wstr_len = search_str->wstr_len;
         wchar_lower.wstr_ptr = SCI_ALLOCA((search_str->wstr_len + 1)*sizeof(wchar));
         if(PNULL !=  wchar_lower.wstr_ptr)
         {
             SCI_MEMSET(wchar_lower.wstr_ptr, 0x00, (search_str->wstr_len + 1)*sizeof(wchar));
             if(search_str->wstr_len > 0)
             {
                 MMIAPICOM_Wstrncpy(wchar_lower.wstr_ptr, search_str->wstr_ptr, search_str->wstr_len);
                 MMIAPICOM_Wstrlower(wchar_lower.wstr_ptr);
             }
         }
         MMIPB_SearchQSortList(&wchar_lower, MMIPB_ALL_GROUP_MASK, 0, is_char);
         count = MMIPB_SearchSubStringInList(wchar_lower.wstr_ptr,search_type,is_char, MMIPB_ALL_GROUP_MASK, 0);         
         if(wchar_lower.wstr_ptr != PNULL)
         {
             SCI_FREE(wchar_lower.wstr_ptr);
         }
     }
     //遍历所有符合条件的记录，并把它们的索引保存到表中
     if(count > 0 )
     {
         if(MMIPB_ERROR_SUCCESS == MMIPB_FindFirstContact(MMIPB_ALL_GROUP_MASK, storage_id, &index, &valid_table, &contact_node, handle_ptr))
         {
             MMIPB_HANDLE_NODE_T  handle_node = {0};
             if(MMIPB_GetHandleContent(*handle_ptr, &handle_node))
             {
                 //分配表格，保存所有查找的记录索引和号码的索引
                 //handle_content_ptr->show_field = field_type;
                 handle_node.data_ptr = (void *)SCI_ALLOCA(sizeof(MMIPB_VISIBLE_INDEX_TABLE_T));
                 tab_ptr = (MMIPB_VISIBLE_INDEX_TABLE_T*)handle_node.data_ptr;
                 if(tab_ptr == PNULL)
                 {
                     return 0;
                 }

                 base_str = SCI_ALLOCA(sizeof(MMIPB_NAME_NUMBER_MAIL_T));
                 if(base_str == PNULL)
                 {
                     SCI_FREE(tab_ptr);
                     return 0;
                 }
                 SCI_MEMSET(base_str, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));
                 //if(handle_content_ptr->select_list != PNULL)
                 {
                     SCI_MEMSET(handle_node.data_ptr, 0x00, sizeof(MMIPB_VISIBLE_INDEX_TABLE_T));
                     if(MMIPB_LIST_FIELD_WHOLE == field_type)
                     {
                         tab_ptr->max_count = count;
                     }
                     else
                     {                         
                         if(MMIPB_LIST_FIELD_NUMBER &field_type)
                         {
                             tab_ptr->max_count = count* MMIPB_MAX_NV_PHONE_NUM;
                         }
                         if(MMIPB_LIST_FIELD_MAIL &field_type)
                         {
                             tab_ptr->max_count += count;
                         }
                     }
                   
                     tab_ptr->used_count = 0;
                     tab_ptr->index_table = (uint32 *)SCI_ALLOCA(tab_ptr->max_count*sizeof(uint32));
                     if(tab_ptr->index_table != PNULL)
                     {
                         SCI_MEMSET(tab_ptr->index_table, 0x00, (tab_ptr->max_count*sizeof(uint32)));
                     }
                 }
                 do                  
                 {
                     if(tab_ptr->used_count >= tab_ptr->max_count)
                     {
                         break;
                     }
                     if(MMIPB_LIST_FIELD_WHOLE == field_type)
                     {
                        //号码不需要分开显示
                         MMIPB_GetContactNameNumberByIndex(base_str, index);
#ifndef PB_SUPPORT_LOW_MEMORY
                         num_index = MMIPB_GetFirstValidNumberIndex(base_str->number_t, MMIPB_MAX_NV_PHONE_NUM);
#endif
                         if(tab_ptr->index_table != PNULL)
                         {
                             tab_ptr->index_table[tab_ptr->used_count]= index + (num_index << 16);
                             tab_ptr->used_count++;
                         }
                     }
                     else
                     {   
                         MMIPB_GetContactNameNumberByIndex(base_str, index);
                         if(MMIPB_LIST_FIELD_NUMBER & field_type)
                         {
                             //号码需要分开显示
                             for(i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++)
                             {

                                 if(valid_table.visible_num_index[i] == 1
                                    && MMIPB_IsValidPhoneNumber(base_str->number_t[i].npi_ton, base_str->number_t[i].number_len)
                                   )
                                 {
                                     if(handle_node.data_ptr != PNULL)
                                     {
                                         if(tab_ptr->used_count >= tab_ptr->max_count)
                                         {
                                             break;
                                         }
                                         if(tab_ptr->index_table != PNULL)
                                         {
                                             tab_ptr->index_table[tab_ptr->used_count]= index + (i << 16);
                                             tab_ptr->used_count++;
                                         }
                                     }                           
                                 }
                                 if(tab_ptr->used_count >= tab_ptr->max_count)
                                 {
                                     break;
                                 }
                             }
                         }
#ifdef MMIPB_MAIL_SUPPORT
                         if(MMIPB_LIST_FIELD_MAIL & field_type)
                         {
                             //email需要分开显示
                             if(tab_ptr->used_count< tab_ptr->max_count)
                             {
                                 if(valid_table.is_mail_visible && base_str->mail.wstr_len > 0)
                                 {
                                     tab_ptr->index_table[tab_ptr->used_count] = index;
                                     tab_ptr->used_count++;
                                 }
                             }
                         }
#endif
                     }
                     SCI_MEMSET(&valid_table, 0x00, sizeof(valid_table));
                     SCI_MEMSET(base_str, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));
                     
                 } 
                 while(MMIPB_ERROR_SUCCESS == MMIPB_FindNextContact(*handle_ptr, MMIPB_ALL_GROUP_MASK, storage_id, &index, &valid_table, &contact_node));
                 MMIPB_SetHandleContent(*handle_ptr, &handle_node);
             }
         }
         if(tab_ptr == PNULL)
         {
             count = 0;
         }
         else
         {
             count = tab_ptr->used_count;
         }
         if(base_str != PNULL)
         {
             SCI_FREE(base_str);
         }
     }
     //SCI_TRACE_LOW:"MMIPB_SearchList field_type 0x%x search_type 0x%x count %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_1497_112_2_18_2_44_3_289,(uint8*)"ddd",field_type, search_type, count);
	 return count;
}

/*****************************************************************************/
//  Description : 根据输入的字符串查找号码中包含该串的记录和姓名中按照首字母匹配的记录
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 找到的号码的个数
/*****************************************************************************/
PUBLIC void MMIPB_ExitSearchList(MMIPB_HANDLE_T handle)
{
    MMIPB_HANDLE_NODE_T handle_node = {0};

    if(MMIPB_GetHandleContent(handle, &handle_node))
    {
         if(handle_node.data_ptr != PNULL)
         {
             if(((MMIPB_VISIBLE_INDEX_TABLE_T *)handle_node.data_ptr)->index_table != PNULL)
             {
                   SCI_FREE(((MMIPB_VISIBLE_INDEX_TABLE_T *)handle_node.data_ptr)->index_table);                  
             }
             SCI_FREE(handle_node.data_ptr);
             handle_node.data_ptr = PNULL;
         }
         MMIPB_SetHandleContent(handle, &handle_node);
    }
    MMIPB_FindClose(handle);   
}

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的姓名，和号码
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 为外部进行进pb选择后
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSearchEntry(MMIPB_HANDLE_T handle,
                                       MMI_STRING_T *name_str,//[OUT] 
                                       uint16 max_name_len, 
                                       MMIPB_BCD_NUMBER_T *bcd_num,//[OUT]
                                       uint32*   group,
                                       uint32 index)
{
	BOOLEAN result = FALSE;    
    MMIPB_HANDLE_NODE_T  handle_node = {0};
    MMIPB_VISIBLE_INDEX_TABLE_T *select_list = PNULL;
    MMIPB_NAME_NUMBER_MAIL_T base_string = {0};

    uint16  contact_list_index = 0;
    uint16  num_index = 0;

    if(MMIPB_GetHandleContent(handle, &handle_node))
    {
         select_list = (MMIPB_VISIBLE_INDEX_TABLE_T *)handle_node.data_ptr;
         if(select_list != PNULL)
         {
             if(index < select_list->used_count)
             {
                 contact_list_index = (uint16)(select_list->index_table[index] & 0xFFFF);
                 num_index = (select_list->index_table[index] >> 16);
                 if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactNameNumberByIndex(&base_string, contact_list_index))
                 {
                      MMIPB_CONTACT_BASE_INFO_T   contact_node_info = {0};
       
                     if(bcd_num != PNULL)
                     {
                        if(num_index < MMIPB_MAX_NV_PHONE_NUM)
                         {
                             SCI_MEMCPY(bcd_num, &base_string.number_t[num_index], sizeof(MMIPB_BCD_NUMBER_T));
                         }
                     }
                     if(name_str != PNULL && name_str->wstr_ptr != PNULL)
                     {
                         name_str->wstr_len = MIN(max_name_len, base_string.name.wstr_len);
                         MMIAPICOM_Wstrncpy(name_str->wstr_ptr, base_string.name.wstr, name_str->wstr_len);
                     }

                    if(PNULL!=group)
                    {
                     contact_node_info =  MMIPB_GetContactBaseInfo(contact_list_index);

                     *group = contact_node_info.group;
                    }
                     result = TRUE;
                 }
             }
         }
    }
    //SCI_TRACE_LOW:"MMIPB_GetSearchEntry index %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_1573_112_2_18_2_44_3_290,(uint8*)"dd",index, result);
	return result;
}

/*****************************************************************************/
//  Description : 返回搜素到的第index（从0开始）个记录的contact_id and storage id
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 为外部进行进pb选择后
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSearchEntryID(MMIPB_HANDLE_T handle,                                       
                                       uint32 index,
                                       uint16 *contact_id_ptr,
                                       uint16 *storage_id_ptr)
{
	BOOLEAN result = FALSE; 
    MMIPB_HANDLE_NODE_T handle_node = {0};
    MMIPB_VISIBLE_INDEX_TABLE_T *select_list = PNULL;
    MMIPB_CONTACT_BASE_INFO_T base_info = {0};
    uint16  contact_list_index = 0;

    if(MMIPB_GetHandleContent(handle, &handle_node))
    {
         select_list = (MMIPB_VISIBLE_INDEX_TABLE_T *)handle_node.data_ptr;
         if(select_list != PNULL)
         {
             if(index < select_list->used_count)
             {
                 contact_list_index = (uint16)(select_list->index_table[index] & 0xFFFF);
                 base_info = MMIPB_GetContactBaseInfo(contact_list_index);
                 if(contact_id_ptr != PNULL)
                 {
                      *contact_id_ptr = base_info.contact_id;
                 }
                 if(storage_id_ptr != PNULL)
                 {
                     *storage_id_ptr = base_info.storage_id;
                 }
                 result = TRUE;
             }
         }
    }
    //SCI_TRACE_LOW:"MMIPB_GetSearchEntryID index %d result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_SEARCH_1614_112_2_18_2_44_3_291,(uint8*)"dd",index, result);
	return result;
}
/*****************************************************************************/
//  Description : 按照UCS2字符串的字符序列进行比较
//  Global resource dependence : none
//  Author: 
//  Note: 例如 “百”和"不"，是按照"bai"和"bu"进行比较的
/*****************************************************************************/
PUBLIC int MMIAPIPB_CmpStrByLetters(
										   wchar   *ucs2_string_ptr1,        //IN:
										   uint8   wstr1_len,            //IN:
										   wchar   *ucs2_string_ptr2,        //IN:
										   uint8   wstr2_len            //IN:
										   )
{
    int ret_val = 0;
	uint8 wstr_len = MIN(wstr1_len, wstr2_len);
	uint8 i = 0;
	// uint8 j = 0;
	wchar ucs2_str1[MMIPB_BASE_CHAR_MAX_LEN] = {0};
	wchar ucs2_str2[MMIPB_BASE_CHAR_MAX_LEN] = {0};
	uint16 unicode_1 = 0;
	uint16 unicode_2 = 0;

    if(PNULL == ucs2_string_ptr1)
	{
		return -1;
	}
	if((PNULL == ucs2_string_ptr2))
	{
		return 1;
	}
    
    if (0 == wstr1_len && 0 == wstr2_len) //equal
    {
        ret_val = 0;
    }
    else if (0 == wstr1_len && 0 != wstr2_len)
    {
        ret_val = -1;
    }
    else if (0 != wstr1_len && 0 == wstr2_len)
    {
        ret_val = 1;
    }
    else
    {
        //如果ucs2都一样就不需要再转换并比较了
        if (0 == memcmp((uint8 *)ucs2_string_ptr1, (uint8 *)ucs2_string_ptr2, wstr_len * sizeof(wchar)))
        {
            ret_val = 0;
        }
        else
        {

            for (i=0; i<(uint8)(wstr_len); i++)
            {
                if (ucs2_string_ptr1[i] < 0x80)
                {
                    if (ucs2_string_ptr2[i] < 0x80)
                    {
                        //都是英文
                        //大写转成小写
                        if ((ucs2_string_ptr1[i] >= 'A') && (ucs2_string_ptr1[i] <= 'Z'))
                        {
                            unicode_1 = ucs2_string_ptr1[i] + ('a' - 'A');
                        }
						else
						{
							unicode_1 = ucs2_string_ptr1[i];
						}
                        if ((ucs2_string_ptr2[i] >= 'A') && (ucs2_string_ptr2[i] <= 'Z'))
                        {
                            unicode_2 = ucs2_string_ptr2[i] + ('a' - 'A');
                        }
						else
						{
							unicode_2 = ucs2_string_ptr2[i];
						}
                        if (unicode_1 > unicode_2)
                        {
                            ret_val = 1;
                            break;
                        }
                        else if (unicode_1 < unicode_2)
                        {
                            ret_val = -1;
                            break;
                        }
                    }
                    else
                    {
                        //ucs2_str1是英文，ucs2_str2是中文
                        ret_val = -1;
                        break;
                    }
                }
                else
                {
                    if (ucs2_string_ptr2[i] < 0x80)
                    {
                        //ucs2_str1是非英文，ucs2_str2是英文
                        ret_val = 1;
                        break;
                    }
                    else
                    {
                        //都是非英文
                        MMIAPIIM_GetTextLetters(ucs2_string_ptr1[i], ucs2_str1);
                        MMIAPIIM_GetTextLetters(ucs2_string_ptr2[i], ucs2_str2);
                        ret_val = MMIAPICOM_Wstrcmp(ucs2_str1, ucs2_str2);
						if(ret_val)
						{
							break;
						}
                    }
                }
            }
        }

        if (0 == ret_val)
        {
            if (wstr1_len > wstr2_len)
            {
                ret_val = 1;
            }
            else if (wstr1_len < wstr2_len)
            {
                ret_val = -1;
            }
        }
    }

    return (ret_val);
}

/*****************************************************************************/
//Description : compare bcd number
// Return: 0: number1 = number2; 1: number1 > number2; -1:number1 < number2
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_BCDCmp(
							 const MMIPB_BCD_NUMBER_T * number1,
                             const MMIPB_BCD_NUMBER_T * number2
							 )
{
	int32 cmp_ret = 1;
	MMIPB_SEARCH_ITEM_T item1 = {0};
	MMIPB_SEARCH_ITEM_T item2 = {0};

	if(number1 == number2)
	{
		return 0;
	}
	if(number1 == PNULL)
	{
		return -1;
	}
	if(number2 == PNULL)
	{
		return 1;
	}

	item1.number_len = PhoneNumberToHex(number1, &item1.phone_number); 
	item2.number_len = PhoneNumberToHex(number2, &item2.phone_number);
	
	// 比较两个号码

	if(item1.phone_number.high > item2.phone_number.high)
	{
		cmp_ret  = 1;
	}
    else if(item1.phone_number.high < item2.phone_number.high)
	{
         cmp_ret  = -1;
	}
	else if(item1.phone_number.low > item2.phone_number.low)
	{
		cmp_ret  = 1;
	}
    else if(item1.phone_number.low < item2.phone_number.low)
	{
         cmp_ret  = -1;
	}
	else
	{
       cmp_ret = 0;
	}
    return cmp_ret;
}

/*****************************************************************************/
//Description : get high value mask and low value mask
// Return: void
/*****************************************************************************/
LOCAL void GetNumberMask(uint16 compare_bcd_len,//IN:bcd len: one byte = 2 
						 uint16 *high_mask_ptr,//OUT
						 uint32 *low_mask_ptr//OUT
						 )
{
	uint16 i = 0;
	uint16 max_cmp_len = MAX_SEARCH_PHONE_NUMBER_LEN;//GetMaxCmpLenByCrrentPLMN();

	for(i = compare_bcd_len; i > 0; i--)
	{
		if((i + MAX_SEARCH_PHONE_NUMBER_LOW_LEN) <= max_cmp_len)
		{
			if(high_mask_ptr != PNULL)
			{
				*high_mask_ptr |= 0X0F <<(((max_cmp_len - MAX_SEARCH_PHONE_NUMBER_LOW_LEN) -i)*4);
			}
		}
		else
		{
			if(low_mask_ptr != PNULL)
			{
				*low_mask_ptr |= 0X0F<<((max_cmp_len - i)*4);
			}
		}
	}
}

/*****************************************************************************/
//  Description : 电话号码的职能匹配
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: 0:两个号码能够职能匹配，其他不能匹配
/*****************************************************************************/
PUBLIC int32 MMIPB_BcdSmartCmp(const MMIPB_BCD_NUMBER_T *bcd_number1, const MMIPB_BCD_NUMBER_T *bcd_number2)
{
	MMIPB_SEARCH_ITEM_T key_item1   = {0};
	MMIPB_SEARCH_ITEM_T key_item2   = {0};
	int32               ret = 0;
	if(bcd_number1 == PNULL || bcd_number2 == PNULL)
	{
		return -1;
	}
	
    key_item1.npi_ton      = bcd_number1->npi_ton;
    key_item1.original_len =  MMIPB_GetBCDNumberStrlen(bcd_number1);
    key_item1.number_len     =   PhoneNumberToHex(bcd_number1, &key_item1.phone_number);
    key_item2.npi_ton      = bcd_number2->npi_ton;
    key_item2.original_len =  MMIPB_GetBCDNumberStrlen(bcd_number2);
    key_item2.number_len     =   PhoneNumberToHex(bcd_number2, &key_item2.phone_number);
    
	if((key_item1.phone_number.high) > (key_item2.phone_number.high))
	{
		ret = 1;
	}
	else if(key_item1.phone_number.high < key_item2.phone_number.high)
	{
		ret = -1;
	}	
	else if(key_item1.phone_number.low > key_item2.phone_number.low)
	{
		ret = 1;
	}
	else if(key_item1.phone_number.low< key_item2.phone_number.low)
	{
		ret = -1;
	}		
	else
	{
		ret = 0;
	}	
	if(ret)
	{
		if(MMIPB_IsNumberPartialMatch(&key_item1, &key_item2))
		{
			ret = 0;
		}
	}
	return ret;
}

/*****************************************************************************/
//  Description :remove "0"  and Country Area code from head
//  Global resource dependence : none
//  Author: 
//Return: 
/*****************************************************************************/
LOCAL void RemoveZeroAndAreaCode(const MMIPB_BCD_NUMBER_T  *sour_num_ptr, 
								   MMIPB_BCD_NUMBER_T *dest_num_ptr)
{
       int i = 0;
	   uint32 j = 0;
	   uint16 compare_len = 0;
	   uint16 start_pos = 0;
	   uint16 zero_len = 0;
	   uint16 max_pos = 0;
	   uint8  dest_len = 0;
	   uint8  bcdCode = 0;
	   uint8  areaCode = 0;
	   BOOLEAN is_valid_num_start = FALSE;

       if(sour_num_ptr != PNULL && (sour_num_ptr->number_len > 0) && (dest_num_ptr != PNULL) && (sour_num_ptr->number_len <= MMIPB_BCD_NUMBER_MAX_LEN))
	   {
		   //get zero len in head
		   for (i=0; i<sour_num_ptr->number_len << 1; i++)
		   {
			   bcdCode = (sour_num_ptr->number[i/2] >> (((i+1) & 1) ? 0 : 4)) & 0x0F;
			   if(0 != bcdCode)
			   {
				   zero_len = i;
				   break;
			   }
		   }
		   max_pos = zero_len;
		   //remove country area code
		   if( MN_NUM_TYPE_INTERNATIONAL == MMIPB_GetNumberTypeFromUint8(sour_num_ptr->npi_ton))
		   {
			   
			   for(j = 0; j < ARR_SIZE(s_countryarea_code_table); j++)
			   {
				   start_pos = zero_len;
				   for (i= zero_len; i<MIN((sour_num_ptr->number_len << 1), (zero_len +MMIPB_COUNTRY_AREA_CODE_MAX_LEN)); i++)
				   {
					   areaCode = (s_countryarea_code_table[j][i/2] >> (((i+1) & 1) ? 0 : 4)) & 0x0F;   
					   bcdCode = (sour_num_ptr->number[i/2] >> (((i+1) & 1) ? 0 : 4)) & 0x0F;
					   if(areaCode != DIALBCD_FILLER && areaCode == bcdCode)
					   {
						   start_pos++;
						   if(max_pos < start_pos)
						   {
							   max_pos = start_pos;
						   }
					   }
					   else
					   {
						   break;
					   }
				   }
				   if(areaCode == DIALBCD_FILLER)
				   {
					   break;
				   }
			   }			   
		   }
		   start_pos = max_pos;

		   compare_len = sour_num_ptr->number_len << 1; //MIN((((sour_num_ptr->number_len) << 1)), (MAX_SEARCH_PHONE_NUMBER_LEN + start_pos));
		   
		   for (i=start_pos; i<compare_len; i++)
		   {
			   bcdCode = (sour_num_ptr->number[i/2] >> (((i+1) & 1) ? 0 : 4)) & 0x0F;
			   if(!is_valid_num_start && DIALBCD_EXPANSION != bcdCode)
			   {
				   //Remove "+", "0", from head
				   is_valid_num_start = TRUE;
			   }
			   if(is_valid_num_start)
			   {
				   //"P","W","+"号码后面的去掉
				   if(bcdCode == DIALBCD_PAUSE || bcdCode == DIALBCD_WILD || bcdCode == DIALBCD_EXPANSION || bcdCode == DIALBCD_FILLER)
				   {
					   //dest_num_ptr->number[dest_len/2] = 0xF0;
					   break;
				   }
				   else
				   {
					   dest_num_ptr->number[dest_len/2] |= bcdCode << (((dest_len+1) & 1) ? 0 : 4);
					   dest_len ++;
				   }
			   }
		   }
		   if(dest_len & 0x1)
		   {
                dest_num_ptr->number[dest_len/2] |= 0xF0;
		   }
		   dest_num_ptr->number_len = (dest_len & 0x01) + (dest_len >> 1);
		   dest_num_ptr->npi_ton = sour_num_ptr->npi_ton;
	   }
}

/*****************************************************************************/
//  Description : 该函数实现名字查找
// str_input：用户的字符串
// group：要查找的记录的所有group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsNameExist(MMI_STRING_T *str_input, uint32 storage_id, uint32* pos)
{
    BOOLEAN is_exist = FALSE;

    if(PNULL == str_input)
    {
        return is_exist;
    }
    if((PNULL!=str_input->wstr_ptr)&&(0!=str_input->wstr_len))
    {
        is_exist = MMIPB_SearchFullStringInList(str_input, storage_id, pos);
    }
	SCI_TRACE_LOW("MMIPB_IsNameExist is_exist %d",is_exist);
	return is_exist;
}
