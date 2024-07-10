/*****************************************************************************
** File Name:      mmibrowser.c                                              *
** Author:         Gaily.Wang                                                *
** Date:           18/04/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the weather forecast app    *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_weather_trc.h"
#include "mn_type.h"
#include "sci_types.h"
#include "in_message.h"
#include "http_api.h"
#include "mmk_app.h"
#include "Mmk_timer.h"
#include "mmi_appmsg.h"
#include "mmiphone_export.h"
#include "mmiweather.h"
#include "mmiweather_text.h"
#include "mmiweather_win.h"
#include "mmiweather_parse.h"
#include "mmiweather_funcs.h"
#include "mmiweather_nv.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "mmiweather_funcs.h"
#include "mmialarm_service.h"
#include "mmipub.h"
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmiim.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define PDP_ACTIVE_TIMEOUT       20000  
#define PDP_RETRY_TIMEOUT        3000 //重新尝试连接pdp，换卡连接
//#define ABS(x) (((x) >= 0) ? (x) : (-(x)))   

#define ACCEPT_STRING           "*/*"
#define ACCEPT_LANGUAGE_STRING  "zh-cn, en"
#define USER_AGENT_STRING       "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)"
#define ACCEPT_CHARSET_STRING   "utf-8, utf-16, iso-8859-1, iso-8859-5, iso-10646-ucs-2, GB2312, windows-1252, us-ascii"
#define UA_PROFILE_STRING       "http://172.16.14.173:8700"
#ifdef WIN32
#define HTTP_URI_STRING         "http://172.16.13.3:80/index.html"
#define HTTP_URI_STRING_BASE    "http://www.google.com/ig/api?hl=en&weather="
#else
#ifdef WEATHER_SUPPORT_GOOGLE
#define HTTP_URI_STRING_BASE    "http://www.google.com/ig/api?hl=en&weather="
#endif
#endif
#ifdef MMI_WEATHER_JSON_PARSE
//#ifdef MMI_WEATHER_SUPPORT_WUNDERGROUND
#define HTTP_URI_WD_STRING_BASE    "http://api.wunderground.com/api/d44fb265236eb1fb/forecast/conditions/EN/q/"
#define HTTP_URI_WD_STRING_1       "http://api.wunderground.com/api/"
#define HTTP_URI_WD_STRING_ID      "d44fb265236eb1fb"
#define HTTP_URI_WD_STRING_QUERY   "/forecast/conditions/EN/q/" 
#define HTTP_URI_WD_STRING_TYPE    ".json" 
//#endif
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
#define HTTP_URI_STRING_BASE    "http://weather0.51wre.com/weather_api/get_weather/?city="
#define HTTP_URI_SECRET_STRING_EXAMPLE  "&sign=58A210F13D513F3F07E1F20E4373ABB6&time=1032549789&v=1.0&api_key=weather&format=json&sign_method=md5"
#define HTTP_URI_STRING_1       "http://weather"
#define HTTP_URI_STRING_QUERY   ".51wre.com/weather_api/get_weather/?city="
#define HTTP_URI_MAX_NUM        10
#define HTTP_SECRET_BEGIN         "&sign="
#define HTTP_SECRET_MD5_BASE      "75732173c43259cf6beec19b285fad3eapi_keyweathercity%sformatjsonsign_methodmd5time%dv1.0"
#define HTTP_URI_STRING_END       "&time=%d&v=1.0&api_key=weather&format=json&sign_method=md5"
#define M_SECOND_1970_TO_1980    ((365*8 +366*2)*24*3600)
#define HTTP_URI_USE_WD_INDEX    3
#endif
#endif

#define AUTO_UPDATE_EVENT_MAX_NUM   48      //每天自动更新的最大次数

#if defined (WEATHER_SUPPORT_GOOGLE)|| defined(MMIWEATHER_SUPPORT_SPRD_SRV)
#define HTTP_CONTENT_BUFFER_SIZE   (1024 * 2)  //和WEATHER_PARSE_BUF_SIZE 保持一致。
#endif
#ifdef MMI_WEATHER_SUPPORT_WUNDERGROUND
#define HTTP_CONTENT_BUFFER_SIZE   (1024 * 20)  //和WEATHER_PARSE_BUF_SIZE 保持一致。
#endif
/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef struct _MMIWEATHER_CITY_INFO
{
    char  country_code[MMIWEATHER_MAX_COUNTRY_CODE_LEN + 1];  //iso code
    char  city_name[MMIWEATHER_MAX_CITY_NAME_LEN + 1];        //city name string in local language
    char  city_code[MMIWEATHER_MAX_CITY_CODE_LEN + 1];        //city code for weather api
}MMIWEATHER_CITY_INFO_T;


typedef struct _MMIWEATHER_APN_TAB
{
    PHONE_PLMN_NAME_E   hplmn; 
    char          *apn;
}MMIWEATHER_APN_TAB_T;

typedef struct _MMIWEATHER_HTTP_CONTEXT_T_
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance; //用于区分不同的HTTP请求。同时只有一个HTTP请求时可以为任意值。
}MMIWEATHER_HTTP_CONTEXT_T;


typedef struct _MMIWEATHER_TIME_TABLE_
{
    uint8   hour;   // (0-23)
    uint8   min;
    BOOLEAN need_update;
}MMIWEATHER_TIME_TABLE_T;

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
MMI_APPLICATION_T                 g_weather_app       = {0};
MMIWEATHER_DATA_T                 g_weather_data      = {0};
LOCAL uint8                       s_pdp_timer_id      = 0;
LOCAL uint8                       s_pdp_retry_timer_id = 0;
LOCAL MMIWEATHER_HTTP_CONTEXT_T   s_http_context      = {0};
LOCAL uint32                      s_net_id            = 0;
LOCAL BOOLEAN                     s_is_support_city   = FALSE;
BOOLEAN                     s_is_retry_en_name  = FALSE;
LOCAL char *s_http_content_weather_buffer_address = PNULL; 
LOCAL MN_DUAL_SYS_E         cur_sys_num   = MN_DUAL_SYS_1;
LOCAL uint8 s_auto_update_timer = 0;

MMIWEATHER_TIME_TABLE_T s_time_table[] =
{
    {0, 0,  FALSE},
    {1, 0,  FALSE},
    {2, 0,  FALSE},
    {3, 0,  FALSE},
    {4, 0,  FALSE},
    {5, 0,  FALSE},
    {6, 0,  FALSE},
    {7, 0,  FALSE},
    {8, 0,  FALSE},
    {9, 0,  FALSE},
    {10,0,  FALSE},
    {11,0,  FALSE},
    {12,0,  FALSE},
    {13,0,  FALSE},
    {14,0,  FALSE},
    {15,0,  FALSE},
    {16,0,  FALSE},
    {17,0,  FALSE},
    {18,0,  FALSE},
    {19,0,  FALSE},
    {20,0,  FALSE},
    {21,0,  FALSE},
    {22,0,  FALSE},
    {23,0,  FALSE},
};

//由于google weather api的限制，国内不能使用代理方式访问。
//国外运营商若也有此问题请在此表中添加。
//天气预报在开机初始化时会先检查此表。若有匹配项则调整初始值，
//从而保证初次天气预报不需要更改网络配置就能更新成功。
const MMIWEATHER_APN_TAB_T s_apn_table[] =
{
    //中国移动
    {PLMN_TEST,     "cmnet"},
    {PLMN_CMCC,     "cmnet"},
    {PLMN_460_02,   "cmnet"},
    {PLMN_460_07,   "cmnet"},
    //中国联通
    {PLMN_UNICOM,   "uninet"},
    {PLMN_460_06,   "uninet"},
};

//天气预报模拟器调试用天气数据
#ifdef WIN32
#ifdef WEATHER_SUPPORT_GOOGLE
const char* s_test_weather_data_ptr = 
"<?xml version=\"1.0\"?> \
<xml_api_reply version=\"1\">\
<weather module_id=\"0\" tab_id=\"0\" mobile_row=\"0\" mobile_zipped=\"1\" row=\"0\" section=\"0\" >\
<forecast_information>\
<city data=\"Shanghai, Shanghai\"/>\
<postal_code data=\"shanghai\"/>\
<latitude_e6 data=\"\"/>\
<longitude_e6 data=\"\"/>\
<forecast_date data=\"2011-05-19\"/>\
<current_date_time data=\"2011-05-19 19:30:00 +0000\"/>\
<unit_system data=\"US\"/>\
</forecast_information>\
<current_conditions>\
<condition data=\"Clear\"/>\
<temp_f data=\"91\"/>\
<temp_c data=\"33\"/>\
<humidity data=\"Humidity: 34%\"/>\
<icon data=\"/ig/images/weather/sunny.gif\"/>\
<wind_condition data=\"Wind: S at 9 mph\"/>\
</current_conditions>\
<forecast_conditions>\
<day_of_week data=\"Thu\"/>\
<low data=\"70\"/>\
<high data=\"88\"/>\
<icon data=\"/ig/images/weather/sunny.gif\"/>\
<condition data=\"Light rain\"/>\
</forecast_conditions>\
<forecast_conditions>\
<day_of_week data=\"Fr\i\"/>\
<low data=\"68\"/>\
<high data=\"93\"/>\
<icon data=\"/ig/images/weather/sunny.gif\"/>\
<condition data=\"Clear\"/>\
</forecast_conditions>\
<forecast_conditions>\
<day_of_week data=\"Sat\"/>\
<low data=\"66\"/>\
<high data=\"91\"/>\
<icon data=\"/ig/images/weather/mostly_sunny.gif\"/>\
<condition data=\"Light rain\"/>\
</forecast_conditions>\
<forecast_conditions>\
<day_of_week data=\"Sun\"/>\
<low data=\"63\"/>\
<high data=\"73\"/>\
<icon data=\"/ig/images/weather/mostly_sunny.gif\"/>\
<condition data=\"Partly Sunny\"/>\
</forecast_conditions>\
</weather>\
</xml_api_reply>";

#else
uint8  * s_test_buffer_ptr = PNULL;
wchar * s_test_file_path = L"E:\\Others\\shanghai.json";
/*the file "shanghai.json" get from internet:
  put the url:http://api.wunderground.com/api/d44fb265236eb1fb/forecast/conditions/EN/q/shanghai.json
  in the web browser(IE or firefox...),and  save the file "shanghai.json"in you local disk,
  then import the file to the phone file system.
  The path of the file also can be modified to any path you want
  If you just want to confirm the weather info ,change ".json" to ".xml" can open a xml format webpage
*/
#endif
#endif
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
uint8    s_url_request_timer = 0;
BOOLEAN  s_is_url_retry = FALSE;
uint8    g_url_idx = 0;
#endif
//const MMIWEATHER_CITY_INFO_T s_city_table[] =
//{
    /*country code*/   /*city name*/        /*City code*/
    
    //直辖市
    /*{"cn",        "北京",        "beijing"     },   
    {"cn",        "上海",        "shanghai"    },     
    {"cn",        "天津",        "tianjin"     },      
    {"cn",        "重庆",        "chongqing"   },  
    //黑龙江
    {"cn",        "哈尔滨"   ,      "harbin"   },   
    {"cn",        "齐齐哈尔" ,      "qiqihar"   },   
    {"cn",        "牡丹江"   ,      "mudanjiang"   },   
    {"cn",        "佳木斯"   ,      "jiamusi"   },   
    {"cn",        "绥化"     ,      "suihua"   },   
    {"cn",        "黑河"     ,      "heihe"   },   
    {"cn",        "大兴安岭" ,      "daxinganling"   },   
    {"cn",        "伊春"     ,      "yichun"   },   
    {"cn",        "大庆"     ,      "daqing"   },   
    {"cn",        "七台河"   ,      "qitaihe"   },   
    {"cn",        "鸡西"     ,      "jixi"   },   
    {"cn",        "鹤岗"     ,      "hegang"   },   
    {"cn",        "双鸭山"   ,      "shuangyashan"   },   
    {"cn",        "长春"     ,      "changchun"   },   
    {"cn",        "吉林"     ,      "jilin"   },   
    {"cn",        "延吉"     ,      "yanji"   },   
    {"cn",        "四平"     ,      "siping"   },   
    {"cn",        "通化"     ,      "tonghua"   },   
    {"cn",        "白城"     ,      "baicheng"   },   
    {"cn",        "辽源"     ,      "liaoyuan"   },   
    {"cn",        "松原"     ,      "songyuan"   },   
    {"cn",        "白山"     ,      "baishan"   },   
    //辽宁
    {"cn",        "沈阳"  ,      "shenyang"   },   
    {"cn",        "大连"  ,      "dalian"   },   
    {"cn",        "鞍山"  ,      "anshan"   },   
    {"cn",        "抚顺"  ,      "fushun"   },   
    {"cn",        "本溪"  ,      "benxi"   },   
    {"cn",        "丹东"  ,      "dandong"   },   
    {"cn",        "锦州"  ,      "jinzhou"   },   
    {"cn",        "营口"  ,      "yingkou"   },   
    {"cn",        "阜新"  ,      "fuxin"   },   
    {"cn",        "辽阳"  ,      "liaoyang"   },   
    {"cn",        "铁岭"  ,      "tieling"   },   
    {"cn",        "朝阳"  ,      "chaoyang"   },   
    {"cn",        "盘锦"  ,      "panjin"   },   
    //内蒙古
    {"cn",        "呼和浩特"   ,      "huhehaote"   },   
    {"cn",        "包头"       ,      "baotou"   },   
    {"cn",        "乌海"       ,      "wuhai"   },   
    {"cn",        "乌兰察布"   ,      "wulanchabu"   },   
    {"cn",        "通辽"       ,      "tongliao"   },   
    {"cn",        "赤峰"       ,      "chifeng"   },   
    {"cn",        "鄂尔多斯"   ,      "eerduosi"   },   
    {"cn",        "巴彦淖尔"   ,      "bayannaoer"   },   
    {"cn",        "锡林郭勒盟" ,      "xilinguole"   },   
    {"cn",        "呼伦贝尔"   ,      "hulunbeier"   },   
    {"cn",        "兴安盟"     ,      "anxing"   },   
    {"cn",        "阿拉善盟"   ,      "alashan"   },   
    //河北
    {"cn",        "石家庄" ,      "shijiazhuang"   },   
    {"cn",        "保定"   ,      "baoding"   },   
    {"cn",        "张家口" ,      "zhangjiajie"   },   
    {"cn",        "承德"   ,      "chengde"   },   
    {"cn",        "唐山"   ,      "tangshan"   },   
    {"cn",        "廊坊"   ,      "langfang"   },   
    {"cn",        "沧州"   ,      "cangzhou"   },   
    {"cn",        "衡水"   ,      "hengshui"   },   
    {"cn",        "邢台"   ,      "xingtai"   },   
    {"cn",        "邯郸"   ,      "handan"   },   
    {"cn",        "秦皇岛" ,      "qinhuangdao"   },   
    //山西   
    {"cn",        "太原",      "taiyuan"   },   
    {"cn",        "大同",      "datong"   },   
    {"cn",        "阳泉",      "yangquan"   },   
    {"cn",        "晋中",      "jinzhong"   },   
    {"cn",        "长治",      "changzhi"   },   
    {"cn",        "晋城",      "jincheng"   },   
    {"cn",        "临汾",      "linfen"   },   
    {"cn",        "运城",      "yuncheng"   },   
    {"cn",        "朔州",      "shuozhou"   },   
    {"cn",        "忻州",      "xinzhou"   },   
    {"cn",        "吕梁",      "luliang"   },   
    //陕西    
    {"cn",        "西安",      "xian"   },   
    {"cn",        "咸阳",      "xianyang"   },   
    {"cn",        "延安",      "yanan"   },   
    {"cn",        "榆林",      "yulin"   },   
    {"cn",        "渭南",      "weinan"   },   
    {"cn",        "商洛",      "shangluo"   },   
    {"cn",        "安康",      "ankang"   },   
    {"cn",        "汉中",      "hanzhong"   },   
    {"cn",        "宝鸡",      "baoji"   },   
    {"cn",        "铜川",      "tongchuan"   },  
    //山东
    {"cn",        "济南",      "jinan"   },   
    {"cn",        "青岛",      "qingdao"   },   
    {"cn",        "淄博",      "zibo"   },   
    {"cn",        "德州",      "dezhou"   },   
    {"cn",        "烟台",      "yantai"   },   
    {"cn",        "潍坊",      "weifang"   },   
    {"cn",        "济宁",      "jining"   },   
    {"cn",        "泰安",      "taian"   },   
    {"cn",        "临沂",      "linyi"   },   
    {"cn",        "菏泽",      "heze"   },   
    {"cn",        "滨州",      "binzhou"   },   
    {"cn",        "东营",      "dongying"   },   
    {"cn",        "威海",      "weihai"   },   
    {"cn",        "枣庄",      "zaozhuang"   },   
    {"cn",        "日照",      "rizhao"   },   //
    //新疆    
    {"cn",        "乌鲁木齐",     "wulumuqi"   },   
    {"cn",        "克拉玛依",     "kelamayi"   },   
    {"cn",        "石河子" ,      "shihezi"   },   
    {"cn",        "昌吉"   ,      "changji"   },   
    {"cn",        "吐鲁番" ,      "tulufan"   },   
    {"cn",        "巴音郭楞",     "bayinguoleng"   },   
    {"cn",        "阿拉尔" ,      "alaer"   },   
    {"cn",        "阿克苏" ,      "akesu"   },   
    {"cn",        "喀什"   ,      "kashi"   },   
    {"cn",        "伊犁"   ,      "yili"   },   
    {"cn",        "塔城"   ,      "tacheng"   },   
    {"cn",        "哈密"   ,      "hami"   },   
    {"cn",        "和田"   ,      "hetian"   },   
    {"cn",        "阿勒泰" ,      "alatai"   },   //
    //西藏    
    {"cn",        "拉萨"  ,      "lasa"   },   
    {"cn",        "日喀则",      "rikaze"   },   
    {"cn",        "山南"  ,      "shannan"   },   
    {"cn",        "林芝"  ,      "linzhi"   },   
    {"cn",        "昌都"  ,      "changdu"   },   
    {"cn",        "那曲"  ,      "naqu"   },   
    {"cn",        "阿里"  ,      "ali"   },   
    //青海   
    {"cn",        "西宁",      "xining"   },   
    {"cn",        "海东",      "haidong"   },   
    {"cn",        "黄南",      "huangnan"   },   
    {"cn",        "海南",      "hainan"   },   
    {"cn",        "果洛",      "guoluo"   },   
    {"cn",        "玉树",      "yushu"   },   
    {"cn",        "海西",      "haixi"   },   
    {"cn",        "海北",      "haibei"   },   
    //甘肃
    {"cn",        "兰州",      "lanzhou"   },  
    {"cn",        "定西",      "dingxi"   },  
    {"cn",        "平凉",      "pingliang"   },  
    {"cn",        "庆阳",      "qingyang"   },  
    {"cn",        "武威",      "weiwu"   },  
    {"cn",        "金昌",      "jinchang"   },  
    {"cn",        "张掖",      "zhangye"   },  
    {"cn",        "酒泉",      "jiuquan"   },  
    {"cn",        "天水",      "tianshui"   },  
    {"cn",        "陇南",      "longnan"   },  
    {"cn",        "临夏",      "linxia"   },  
    {"cn",        "甘南",      "gannan"   },  
    {"cn",        "白银",      "baiyin"   },  
    //宁夏
    {"cn",        "银川"  ,      "yinchuan"   },  
    {"cn",        "石嘴山",      "shizuishan"   },  
    {"cn",        "吴忠"  ,      "wuzhong"   },  
    {"cn",        "固原"  ,      "guyuan"   },  
    {"cn",        "中卫"  ,      "zhongwei"   },  
    //河南
    {"cn",        "郑州",      "zhengzhou"   },  
    {"cn",        "安阳",      "anyang"   },  
    {"cn",        "新乡",      "xinxiang"   },  
    {"cn",        "许昌",      "xuchang"   },  
    {"cn",        "平顶山",    "pingdingshan"   },  
    {"cn",        "信阳",      "xinyang"   },  
    {"cn",        "南阳",      "nanyang"   },  
    {"cn",        "开封",      "kaifeng"   },  
    {"cn",        "洛阳",      "luoyang"   },  
    {"cn",        "商丘",      "shangqiu"   },  
    {"cn",        "焦作",      "jiaozuo"   },  
    {"cn",        "鹤壁",      "hebi"   },  
    {"cn",        "濮阳",      "puyang"   },  
    {"cn",        "周口",      "zhoukou"   },  
    {"cn",        "漯河",      "luohe"   },  
    {"cn",        "驻马店",    "zhumadian"   },   
    //江苏    
    {"cn",        "南京"  ,      "nanjing"   },  
    {"cn",        "无锡"  ,      "wuxi"   },  
    {"cn",        "镇江"  ,      "zhengjiang"   },  
    {"cn",        "苏州"  ,      "suzhou"   },  
    {"cn",        "南通"  ,      "nantong"   },  
    {"cn",        "扬州"  ,      "yangzhou"   },  
    {"cn",        "盐城"  ,      "yancheng"   },  
    {"cn",        "徐州"  ,      "xuzhou"   },  
    {"cn",        "淮安"  ,      "huaian"   },  
    {"cn",        "连云港",      "lianyungang"   },  
    {"cn",        "常州"  ,      "changzhou"   },  
    {"cn",        "泰州"  ,      "taizhou"   },  
    {"cn",        "宿迁"  ,      "suqian"   },  
    //湖北
    {"cn",        "武汉"  ,      "wuhan"   },  
    {"cn",        "襄阳"  ,      "xiangyang"   },  
    {"cn",        "鄂州"  ,      "ezhou"   },  
    {"cn",        "孝感"  ,      "xiaogan"   },  
    {"cn",        "黄冈"  ,      "huanggang"   },  
    {"cn",        "黄石"  ,      "huangshi"   },  
    {"cn",        "咸宁"  ,      "xianning"   },  
    {"cn",        "荆州"  ,      "jingzhou"   },  
    {"cn",        "宜昌"  ,      "yichang"   },  
    {"cn",        "恩施"  ,      "enshi"   },  
    {"cn",        "十堰"  ,      "shiyan"   },  
    {"cn",        "神农架",      "shennongjia"   },  
    {"cn",        "随州"  ,      "suizhou"   },  
    {"cn",        "荆门"  ,      "jingmen"   },  
    {"cn",        "天门"  ,      "tianmen"   },   //
    //浙江
    {"cn",        "杭州",      "hangzhou"   },  
    {"cn",        "湖州",      "huzhou"   },  
    {"cn",        "嘉兴",      "jiaxing"   },  
    {"cn",        "宁波",      "ningbo"   },  
    {"cn",        "绍兴",      "shaoxing"   },  
    {"cn",        "台州",      "taizhou"   }, 
    {"cn",        "温州",      "wenzhou"   },  
    {"cn",        "丽水",      "lishui"   },  
    {"cn",        "金华",      "jinhua"   },  
    {"cn",        "衢州",      "quzhou"   },  
    {"cn",        "舟山",      "zhoushan"   },  
    //安徽    
    {"cn",        "合肥"  ,      "hefei"   }, 
    {"cn",        "蚌埠"  ,      "bengbu"   },  
    {"cn",        "芜湖"  ,      "wuhu"   },  
    {"cn",        "淮南"  ,      "huainan"   },  
    {"cn",        "马鞍山",      "maanshan"   },  
    {"cn",        "安庆"  ,      "anqing"   },  
    {"cn",        "宿州"  ,      "suzhou"   }, 
    {"cn",        "阜阳"  ,      "fuyang"   }, 
    {"cn",        "亳州"  ,      "haozhou"   },  
    {"cn",        "黄山"  ,      "huangshan"   },  
    {"cn",        "滁州"  ,      "chuzhou"   },  
    {"cn",        "淮北"  ,      "huaibei"   },  
    {"cn",        "铜陵"  ,      "tongling"   },  
    {"cn",        "宣城"  ,      "xuancheng"   }, 
    {"cn",        "六安"  ,      "luan"   }, //
    //福建    
    {"cn",        "福州",      "fuzhou"   },  
    {"cn",        "厦门",      "xiamen"   },  
    {"cn",        "宁德",      "ningde"   },  
    {"cn",        "莆田",      "putian"   },  
    {"cn",        "泉州",      "quanzhou"   },  
    {"cn",        "漳州",      "zhangzhou"   }, 
    {"cn",        "龙岩",      "longyan"   },  
    {"cn",        "三明",      "sanming"   },  
    {"cn",        "南平",      "nanping"   },  
    //江西
    {"cn",        "南昌"  ,      "nanchang"   },  
    {"cn",        "九江"  ,      "jiujiang"   },  
    {"cn",        "上饶"  ,      "shangrao"   }, 
    {"cn",        "抚州"  ,      "wuzhou,jiangxi"   },  
    {"cn",        "宜春"  ,      "yichun"   },  
    {"cn",        "吉安"  ,      "jian"   }, 
    {"cn",        "赣州"  ,      "ganzhou"   },  
    {"cn",        "景德镇",      "jingdezhen"   },  
    {"cn",        "萍乡"  ,      "pingxiang"   }, 
    {"cn",        "新余"  ,      "xinyu"   },  
    {"cn",        "鹰潭"  ,      "yingtan"   },  
    //湖南    
    {"cn",        "长沙"  ,      "changsha"   }, 
    {"cn",        "湘潭"  ,      "xiangtan"   },  
    {"cn",        "株洲"  ,      "zhuzhou"   },  
    {"cn",        "衡阳"  ,      "hengyang"   }, 
    {"cn",        "郴州"  ,      "chenzhou"   },  
    {"cn",        "常德"  ,      "changde"   },  
    {"cn",        "益阳"  ,      "yiyang"   }, 
    {"cn",        "娄底"  ,      "loudi"   }, 
    {"cn",        "邵阳"  ,      "shaoyang"   },  
    {"cn",        "岳阳"  ,      "yueyang"   },  
    {"cn",        "张家界",      "zhangjiajie"   }, 
    {"cn",        "怀化"  ,      "huaihua"   },  
    {"cn",        "永州"  ,      "yongzhou"   }, 
    {"cn",        "湘西"  ,      "xiangxi"   }, 
    //贵州    
    {"cn",        "贵阳"  ,      "guiyang"   },  
    {"cn",        "遵义"  ,      "zunyi"   },  
    {"cn",        "安顺"  ,      "anshun"   }, 
    {"cn",        "黔南"  ,      "qiannan"   },  
    {"cn",        "黔东南" ,     "qiandongnan"   },  
    {"cn",        "铜仁"  ,      "tongren"   }, 
    {"cn",        "毕节"  ,      "bijie"   },  
    {"cn",        "六盘水",      "liupanshui"   },  
    {"cn",        "黔西南" ,      "qianxinan"   }, 
    //四川
    {"cn",        "成都"  ,      "chengdu"   },  
    {"cn",        "攀枝花",      "panzhihua"   },  
    {"cn",        "自贡"  ,      "zigong"   }, 
    {"cn",        "绵阳"  ,      "mianyang"   },  
    {"cn",        "南充"  ,      "nanchong"   },  
    {"cn",        "达州"  ,      "dazhou"   }, 
    {"cn",        "遂宁"  ,      "suining"   },  
    {"cn",        "广安"  ,      "guangan"   },  
    {"cn",        "巴中"  ,      "bazhong"   }, 
    {"cn",        "泸州"  ,      "luzhou"   },  
    {"cn",        "宜宾"  ,      "yibin"   },  
    {"cn",        "内江"  ,      "neijing"   }, 
    {"cn",        "资阳"  ,      "ziyang"   },  
    {"cn",        "乐山"  ,      "leshan"   },  
    {"cn",        "眉山"  ,      "meishan"   }, //
    //广东    
    {"cn",        "广州",      "guangzhou"   },  
    {"cn",        "韶关",      "shaoguan"   },  
    {"cn",        "惠州",      "huizhou"   }, 
    {"cn",        "梅州",      "meizhou"   },  
    {"cn",        "汕头",      "shantou"   },  
    {"cn",        "深圳",      "shenzhen"   }, 
    {"cn",        "珠海",      "zhuhai"   },  
    {"cn",        "佛山",      "foshan"   },  
    {"cn",        "肇庆",      "zhaoqing"   }, 
    {"cn",        "湛江",      "zhanjiang"   },  
    {"cn",        "江门",      "jiangmen"   },  
    {"cn",        "河源",      "heyuan"   }, 
    {"cn",        "清远",      "qingyuan"   },  
    {"cn",        "云浮",      "yunfu"   },  
    {"cn",        "潮州",      "chaozhou"   }, //
    //云南 
    {"cn",        "昆明",      "kunming"   },  
    {"cn",        "大理",      "dali"   },  
    {"cn",        "红河",      "honghe"   },  
    {"cn",        "曲靖",      "qujing"   },  
    {"cn",        "保山",      "baoshan"   },  
    {"cn",        "文山",      "wenshan"   },  
    {"cn",        "玉溪",      "yuxi"   },  
    {"cn",        "楚雄",      "chuxiong"   },  
    {"cn",        "思茅",      "simao"   },  
    {"cn",        "昭通",      "zhaotong"   },  
    {"cn",        "临沧",      "lincang"   },  
    {"cn",        "怒江",      "nujiang"   },  
    {"cn",        "迪庆",      "diqing"   },  
    {"cn",        "丽江",      "lijiang"   },  
    {"cn",        "德宏",      "dehong"   },  //
    //广西
    {"cn",        "南宁"  ,      "nanning"   },  
    {"cn",        "崇左"  ,      "chongzuo"   },  
    {"cn",        "柳州"  ,      "liuzhou"   },  
    {"cn",        "来宾"  ,      "laibin"   },  
    {"cn",        "桂林"  ,      "guilin"   },  
    {"cn",        "梧州"  ,      "wuzhou"   },  
    {"cn",        "贺州"  ,      "hezhou"   },  
    {"cn",        "贵港"  ,      "guigang"   },  
    {"cn",        "玉林"  ,      "yulin"   },  
    {"cn",        "百色"  ,      "baise"   },  
    {"cn",        "钦州"  ,      "qinzhou"   },  
    {"cn",        "河池"  ,      "hechi"   },  
    {"cn",        "北海"  ,      "beihai"   },  
    {"cn",        "防城港",      "fangchenggang"   },  
    //海南
    {"cn",        "海口",      "haikou"   },  
    {"cn",        "琼山",      "qiongshan"   },  
    {"cn",        "三亚",      "sanya"   },  
    {"cn",        "东方",      "dongfang"   },  
    {"cn",        "临高",      "lingao"   },  
    {"cn",        "澄迈",      "chengmai"   },  
    {"cn",        "儋州",      "danzhou"   },  
    {"cn",        "昌江",      "changjiang"   },  
    {"cn",        "白沙",      "baisha"   },  
    {"cn",        "琼中",      "qiongzhong"   },  
    {"cn",        "定安",      "dingan"   },  
    {"cn",        "屯昌",      "tunchang"   },  
    {"cn",        "琼海",      "qionghai"   },  
    {"cn",        "文昌",      "wenchang"   },  
    {"cn",        "清兰",      "qinglan"   },  //
    //港澳台  
    {"HK",        "香港",      "hongkong"   },  
    {"MC",        "澳门",      "aomen"   },  
    {"tw",        "高雄",      "Kaohsiung"   },  
    {"tw",        "恒春",      "Hengchun"   },  
    {"tw",        "花莲",      "Hualien"   },  
    {"tw",        "基隆",      "Keelung"   },  
    {"tw",        "嘉义",      "Chiayi"   },  
    {"tw",        "金门",      "Kinmen"   },  
    {"tw",        "兰屿",      "Lanyu"   },  
    {"tw",        "马祖",      "Matzu"   },  
    {"tw",        "苗栗",      "Miaoli"   },  
    {"tw",        "南投",      "Nantou"   },  
    {"tw",        "澎湖",      "Penghu"   },  
    {"tw",        "屏东",      "Pingtung"   },
    {"tw",        "台北",      "Taipei"   },  
    {"tw",        "台东",      "Taitung"   },  
    {"tw",        "台南",      "Tainan"   },  
    {"tw",        "台中",      "Taichung"   },  
    {"tw",        "桃园",      "Taoyuan"   },  
    {"tw",        "新竹",      "Hsinchu"   },  
    {"tw",        "宜兰",      "Yilan"   },  
    {"tw",        "云林",      "Yunlin"   },
    {"tw",        "彰化",      "Changhua"   },  
 
};*/

//此表用于扩充添加城市时城市名支持的输入语言。
//比如google原本不支持输入中文"澳门"添加城市，加入此表后可通过输入"澳门"添加此城市。
#if 0
const MMIWEATHER_CITY_TAB_T s_special_city_table[] =
{
    /*某语言城市名Unicode码*/     /*英文名*/              /*中文城市名*/
#ifdef WEATHER_SUPPORT_GOOGLE
    {L"\x6fb3\x95e8",           "Macau"         },     /*lint !e64*/      //澳门/
    {L"\x91cd\x5e86",           "chongqing"     },    //重庆
#elif defined(MMI_WEATHER_SUPPORT_WUNDERGROUND) || defined(MMIWEATHER_SUPPORT_SPRD_SRV)
    {L"\x91cd\x5e86",           "chongqing"     },    //重庆
    {L"\x6c88\x9633",           "shenyang"},//沈阳
    {L"\x53a6\x95e8",           "xiamen"},//厦门
    {L"\x8d63\x5dde",           "ganzhou"},//赣州
    {L"\x8d35\x9633",           "guiyang"},//贵阳
    {L"\x5927\x8fde",           "dalian"},//大连
    {L"\x6d4e\x5357",           "jinan"},//济南
    {L"\x5357\x5b81",           "nanning"},//南宁
    {L"\x5b81\x6ce2",           "ningbo"},//宁波
    {L"\x77f3\x5bb6\x5e84",     "shijiazhuang"},//石家庄
    {L"\x5510\x5c71",           "ningbo"},//唐山
    {L"\x897f\x5b81",           "xining"},//西宁
    {L"\x94f6\x5ddd",           "yinchuan"},//银川
    {L"\x957f\x6625",           "changchun"},//长春
    {L"\x90d1\x5dde",           "zhengzhou"},//郑州
    {L"\x54c8\x5c14\x6ee8",     "harbin"},//哈尔滨
    {L"\x4e4c\x9c81\x6728\x9f50", "urumqi"},//乌鲁木齐
    {L"\x957f\x6c99",           "zgha"},//长沙
    {L"changsha",               "zgha"},//长沙
    {L"\x62c9\x8428",           "lhasa"},//拉萨
    {L"\x4e1c\x4eac",           "tokyo"},//东京
    {L"lucknow",                 "vilk"},//lucknow
    {L"pondicherry","M.O. Pondicherry"},//Pondicherry
    {L"\x9f50\x9f50\x54c8\x5c14",    "qiqihar"},//齐齐哈尔
    {L"\x5e38\x5dde",           "changzhou"},//常州
    {L"\x5609\x5174",           "jiaxing"},//嘉兴
    {L"\x6e56\x5dde",           "huzhou"},//湖州
    {L"\x7ecd\x5174",           "shaoxing"},//绍兴
    {L"\x91de\x534e",           "jinhua"},//金华
    {L"\x829c\x6e56",           "wuhu"},//芜湖
    {L"\x5eca\x574a",           "langfang"},//廊坊
#endif
};
#endif
/**-------------------------------------------------------------------------*
**            LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Weather forecast application's message handler
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

LOCAL BOOLEAN WeatherPDPTry(MN_DUAL_SYS_E sim_sys);

LOCAL void HandlePdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                );

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void StartHttpRequest(void);

#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
/*****************************************************************************/
//  Description : write url index in nv
//  Global resource dependence : 
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL void WriteWeatherURLIndex(uint8 idx);
/*****************************************************************************/
//  Description : set url index
//  Global resource dependence : 
//  Author:juan.wu
//  Note:
/*****************************************************************************/
LOCAL uint8 ReadWeatherURLIndex(void);
#endif
/*****************************************************************************/
//  Description : change chinese name to alphabetic name
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void WeatherGetCityLetters(wchar *city_name_ptr, wchar * city_code_ptr);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init weather app
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_InitApp(void)
{
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_InitApp---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_624_112_2_18_3_6_33_0,(uint8*)"");
    g_weather_app.ProcessMsg = HandleWeatherMsg;
    MMIWEATHER_ReadNV(MMIWEATHER_NV_SETTING_INFO);
    MMIWEATHER_ReadNV(MMIWEATHER_NV_WEATHER_INFO); 
    MMIWEATHER_SetUpdateTimeTable(g_weather_data.setting_info.is_allow_auto_update);
}
#ifdef WIN32
#if defined (MMIWEATHER_SUPPORT_SPRD_SRV) || defined(MMI_WEATHER_SUPPORT_WUNDERGROUND)
LOCAL BOOLEAN  WIN32Getweatherinfo(void)
{
    uint32 to_read_size = 1024*20;    
    uint16 full_path_len = MMIAPICOM_Wstrlen(s_test_file_path);
    
    if(PNULL != s_test_buffer_ptr)
    {
        SCI_FREE(s_test_buffer_ptr);
        s_test_buffer_ptr = PNULL;
    }
    s_test_buffer_ptr = SCI_ALLOC_APPZ(1024*20);
    if(PNULL == s_test_buffer_ptr)
    {
        return FALSE;
    }

    if(MMIAPIFMM_ReadFilesDataSyn(s_test_file_path, 
        full_path_len, 
        s_test_buffer_ptr, 
        to_read_size))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}
#endif
#endif
/*****************************************************************************/
//  Description : Reset weather app
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_ResetApp(void)
{
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_ResetApp---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_639_112_2_18_3_6_33_1,(uint8*)"");
    SCI_MEMSET(&g_weather_data.setting_info, 0, sizeof(g_weather_data.setting_info));
    g_weather_data.setting_info.update_range_start = MMIWEATHER_UPDATE_TIME_BEGIN_NOLIMIT;
    g_weather_data.setting_info.update_range_end   = MMIWEATHER_UPDATE_TIME_END_NOLIMIT;
    MMIWEATHER_WriteNV(MMIWEATHER_NV_SETTING_INFO);
    SCI_MEMSET(&g_weather_data.weather_info, 0, sizeof(g_weather_data.weather_info));
    MMIWEATHER_WriteNV(MMIWEATHER_NV_WEATHER_INFO);  
    MMIWEATHER_SetUpdateTimeTable(g_weather_data.setting_info.is_allow_auto_update);
    MMIWEATHER_InitNetworkAccount();
}


/*****************************************************************************/
//  Description : Write settings info to NV 
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_WriteNV(MMIWEATHER_NV_ITEM_E nv_item)
{
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_WriteNV---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_657_112_2_18_3_6_33_2,(uint8*)"");

    switch(nv_item)
    {
        case MMIWEATHER_NV_SETTING_INFO:
        {
            MMINV_WRITE(nv_item, &g_weather_data.setting_info);
            break;			            
        }
        case MMIWEATHER_NV_WEATHER_INFO:
        {
            MMINV_WRITE(nv_item, &g_weather_data.weather_info);
            break;			            
        }
        default:
        {
            break;
        }
    }   
}

/*****************************************************************************/
//  Description : Read settings info from NV 
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_ReadNV(MMIWEATHER_NV_ITEM_E nv_item)
{
    MN_RETURN_RESULT_E    ret_value = MN_RETURN_FAILURE;
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_ReadNV---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_685_112_2_18_3_6_33_3,(uint8*)"");

    switch(nv_item)		   	
    {
        case MMIWEATHER_NV_SETTING_INFO:
        {
            MMINV_READ(nv_item, &g_weather_data.setting_info, ret_value);
            
            if (MN_RETURN_SUCCESS != ret_value)
            {
                SCI_MEMSET(&g_weather_data.setting_info, 0, sizeof(g_weather_data.setting_info));
                g_weather_data.setting_info.update_range_start = MMIWEATHER_UPDATE_TIME_BEGIN_NOLIMIT;
                g_weather_data.setting_info.update_range_end = MMIWEATHER_UPDATE_TIME_END_NOLIMIT;
                MMINV_WRITE(nv_item, &g_weather_data.setting_info);
            }
            break;			            
        }
        case MMIWEATHER_NV_WEATHER_INFO:
        {
            MMINV_READ(nv_item, &g_weather_data.weather_info, ret_value);
            
            if (MN_RETURN_SUCCESS != ret_value)
            {
                SCI_MEMSET(&g_weather_data.weather_info, 0, sizeof(g_weather_data.weather_info));
                MMINV_WRITE(nv_item, &g_weather_data.weather_info);
            }
            break;			            
        }
        default:
        {
            break;			            
        }
    }   

}



/*****************************************************************************/
//  Description : Get Setting info
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_DATA_T* MMIWEATHER_GetAppData(void)
{
    return &g_weather_data;
}

/*****************************************************************************/
//  Description : Get system info
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_SYS_INFO_T* MMIWEATHER_GetSysInfo(void)
{
    return &g_weather_data.system_info;
}

/*****************************************************************************/
//  Description : Get Setting info
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_SETTING_T* MMIWEATHER_GetSettingInfo(void)
{
    return &g_weather_data.setting_info;
}

#ifdef MMIWIDGET_WEATHER_ANIM
/*****************************************************************************/
//  Description : Get anim info
//  Global resource dependence : 
//  Author: Jin.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_ANIM_T* MMIWEATHER_GetAnimInfo(void)
{
    return &g_weather_data.anim_info;
}
#endif

/*****************************************************************************/
//  Description : Get weather info
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC MMIWEATHER_WEATHER_T* MMIWEATHER_GetCityWeatherInfo(uint8 index)
{
    if(index < MMIWEATHER_MAX_CITY_NUM)
    {
        return &g_weather_data.weather_info[index];
    }
    else
    {
        return PNULL;
    }
}

PUBLIC MMIWEATHER_WEATHER_T* MMIWEATHER_GetCurCityWeatherInfo(void)
{
    if(g_weather_data.setting_info.cur_city < MMIWEATHER_MAX_CITY_NUM)
    {
        return &g_weather_data.weather_info[g_weather_data.setting_info.cur_city];
    }
    else
    {
        return PNULL;
    }
}

#ifdef MMI_WEATHER_JSON_PARSE
PUBLIC MMIWEATHER_ERROR_INFO_T * MMIWEATHER_GetErrorInfo(void)
{
    return &g_weather_data.error_descript;
}

PUBLIC MMIWEATHER_COUNTRY_INFO_T * MMIWEATHER_GetCountryInfo(void)
{
    return &g_weather_data.country_info;
}
#endif
PUBLIC BOOLEAN MMIWEATHER_IsCityWeatherInfoLegal(uint8 index)
{
    BOOLEAN result = FALSE;
    if (index < MMIWEATHER_MAX_CITY_NUM && WEATHER_CONDITION_UNKNOW != g_weather_data.weather_info[index].current.condition)
    {
        result = TRUE;
    }
    return result;
}

PUBLIC BOOLEAN MMIWEATHER_IsCurCityWeatherInfoLegal(void)
{
    return MMIWEATHER_IsCityWeatherInfoLegal(g_weather_data.setting_info.cur_city);
}

/*PUBLIC BOOLEAN MMIWEATHER_AddCity(MMIWEATHER_WEATHER_T* city_info_ptr)
{
    BOOLEAN is_success = FALSE;
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_AddCity---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_807_112_2_18_3_6_33_4,(uint8*)"");
    if (setting_ptr->city_num < MMIWEATHER_MAX_CITY_NUM)
    {
        if(PNULL != city_info_ptr)
        {
            SCI_MEMCPY(&g_weather_data.weather_info[setting_ptr->city_num], city_info_ptr, sizeof (MMIWEATHER_WEATHER_T));
            setting_ptr->city_num ++;
            is_success = TRUE;

            MMIWEATHER_WriteNV(MMIWEATHER_NV_SETTING_INFO);
            MMIWEATHER_WriteNV(MMIWEATHER_NV_WEATHER_INFO);
        }
    }

    return is_success;
}*/


/*****************************************************************************/
//  Description : MMIWEATHER_AddCity
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_AddCity(wchar* city_name_wstr)
{
    BOOLEAN recode = FALSE;
    
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_AddCity---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_833_112_2_18_3_6_33_5,(uint8*)"");
    
    if (PNULL != city_name_wstr)
    {
        uint32 city_name_len = MMIAPICOM_Wstrlen(city_name_wstr);

        if(city_name_len > 0)
        {
            uint8 city_num = MMIWEATHER_GetCityNum();
           
            if(city_num < MMIWEATHER_MAX_CITY_NUM)
            {
                MMIWEATHER_WEATHER_T* weather_info_ptr = MMIWEATHER_GetCityWeatherInfo(city_num);
                SCI_MEMSET(weather_info_ptr->city_name, 0, sizeof(weather_info_ptr->city_name));
                SCI_MEMSET(weather_info_ptr->city_code, 0, sizeof(weather_info_ptr->city_code));
                MMIAPICOM_Wstrncpy(weather_info_ptr->city_name, city_name_wstr, city_name_len);
                recode = TRUE;
            }
            else
            {
                MMIWEATHER_ProcAddCityStatus(MMIWEATHER_ADD_CITY_STATE_FULL);
            }
        }
        else
        {
            //提示输入正确的城市名
        }
    
    }

    return recode;
}


PUBLIC BOOLEAN MMIWEATHER_DelCity(uint8 index)
{
    BOOLEAN is_success = FALSE;
    MMIWEATHER_SETTING_T* setting_ptr = MMIWEATHER_GetSettingInfo();
    
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_DelCity---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_871_112_2_18_3_6_33_6,(uint8*)"");

    if (setting_ptr->city_num > index)
    {
        uint32 copy_num = setting_ptr->city_num - 1 - index;
        if (copy_num > 0)
        {
            SCI_MEMCPY(&g_weather_data.weather_info[index], &g_weather_data.weather_info[index + 1], copy_num * sizeof (MMIWEATHER_WEATHER_T));
        }
        SCI_MEMSET(&g_weather_data.weather_info[setting_ptr->city_num - 1], 0, sizeof (MMIWEATHER_WEATHER_T));
        
        setting_ptr->city_num --;
        if (index <= setting_ptr->cur_city)
        {
            uint8 cur_city = setting_ptr->cur_city;
            if (cur_city > 0)
            {
                cur_city --;
            }
            MMIWEATHER_SetCurCityIndex(cur_city);
        }
        is_success = TRUE;

        MMIWEATHER_WriteNV(MMIWEATHER_NV_SETTING_INFO);
        MMIWEATHER_WriteNV(MMIWEATHER_NV_WEATHER_INFO);
    }

    return is_success;
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* GetApnStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   apn_str = PNULL;
    uint8   index   = 0;

    index = g_weather_data.setting_info.account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        apn_str = (char*)linksetting->apn;
    }

    return apn_str;
    
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* GetUserNameStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   user_name_str = PNULL;
    uint8   index   = 0;

    index = g_weather_data.setting_info.account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->username_len)
    {
        user_name_str = (char*)linksetting->username;
    }
    
    return user_name_str;
    
}

/*****************************************************************************/
//  Discription: Get apn str 
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL char* GetPasswordStr(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   password_str = PNULL;
    uint8   index   = 0;

    index = g_weather_data.setting_info.account_index[dual_sys]; 
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);

    if(PNULL != linksetting && 0 != linksetting->password_len)
    {
        password_str = (char*)linksetting->password;
    }
    
    return password_str;
}


/*****************************************************************************/
//  Description : handle socket connect timeout
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpActiveTimer(
                                            uint8 timer_id,
                                            uint32 param
                                            )
{
    //SCI_TRACE_LOW:"[weather]:HandlePdpActiveTimer---Time out error!---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_976_112_2_18_3_6_34_7,(uint8*)"");
    MMIWEATHER_PdpDeactive();
    MMIWEATHER_SendMsgToApp(MSG_WEATHER_CONN_FAILED_CNF);
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StartPdpActiveTimer(void)
{
    if(0 != s_pdp_timer_id)
    {
        MMK_StopTimer(s_pdp_timer_id);
        s_pdp_timer_id = 0;
    }
    
    s_pdp_timer_id = MMK_CreateTimerCallback(PDP_ACTIVE_TIMEOUT, 
                            HandlePdpActiveTimer, 
                            PNULL, 
                            FALSE);

    //SCI_TRACE_LOW:"[weather]:Weather_StartSockTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_998_112_2_18_3_6_34_8,(uint8*)"");
}

/*****************************************************************************/
//  Discription: Start socket connect timer
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
/*****************************************************************************/
LOCAL void StopPdpActiveTimer(void)
{
    //SCI_TRACE_LOW:"[weather]:Weather_StopSockTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1007_112_2_18_3_6_34_9,(uint8*)"");

    if(0 != s_pdp_timer_id)
    {
        MMK_StopTimer(s_pdp_timer_id);
        s_pdp_timer_id = 0;
    }
}


/*****************************************************************************
//  Description : handle pdp msg 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
*****************************************************************************/
LOCAL void HandlePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    
    if(PNULL == msg_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_HandlePdpMsg--- msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1024_112_2_18_3_6_34_10,(uint8*)"d",msg_ptr->msg_id);
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_HandlePdpMsg--- result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1025_112_2_18_3_6_34_11,(uint8*)"d",msg_ptr->result);

    switch(msg_ptr->msg_id) 
    {
    case MMIPDP_ACTIVE_CNF:     //PDP激活成功
        //SCI_TRACE_LOW:"[weather]:MMIWEATHER_HandlePdpMsg--- MMIPDP_ACTIVE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1035_112_2_18_3_6_34_12,(uint8*)"");
        StopPdpActiveTimer();
        if(MMIPDP_RESULT_SUCC == msg_ptr->result)
		{
            s_net_id = msg_ptr->nsapi;
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_PDP_ACTIVE_CNF);
		}
		else if (MMIPDP_RESULT_FAIL == msg_ptr->result)
		{
		    MMIAPIPDP_Deactive(MMI_MODULE_WEATHER);
		    if(0 != s_pdp_retry_timer_id)
		    {
                MMK_StopTimer(s_pdp_retry_timer_id);
                s_pdp_retry_timer_id = 0;
		    }
		    s_pdp_retry_timer_id = MMK_CreateTimerCallback(
	                                                    PDP_RETRY_TIMEOUT, 
                                        		        HandlePdpRetryTimer, 
                                        		        PNULL, 
                                            		    FALSE
                                            		    );
		}
        else 
        {
            MMIWEATHER_PdpDeactive();
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_PDP_ACTIVE_FIILED_CNF);
        }        
        break;
        
    case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功。
        //SCI_TRACE_LOW:"[weather]:MMIWEATHER_HandlePdpMsg--- MMIPDP_DEACTIVE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1050_112_2_18_3_6_34_13,(uint8*)"");
        s_net_id = 0;
        break;
        
    case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活。
        //SCI_TRACE_LOW:"[weather]:MMIWEATHER_HandlePdpMsg--- MMIPDP_DEACTIVE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1055_112_2_18_3_6_34_14,(uint8*)"");
        s_net_id = 0;
        MMIWEATHER_PdpDeactive();
        break;
        
    default:
        break;
    }
    
}



/*****************************************************************************/
//  Description : Pdp Active, 
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_PdpActive(void)
{
    BOOLEAN                 return_val      = FALSE;
    MN_DUAL_SYS_E           dual_sys        = MN_DUAL_SYS_MAX;
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_PdpActive"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1078_112_2_18_3_6_34_15,(uint8*)"");

    switch(g_weather_data.setting_info.network)	
    {
        case WEATHER_NETWORK_AUTO:
            {
#ifdef MMI_WIFI_SUPPORT
                //优先wifi
                if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
                {
                    active_info.app_handler         = MMI_MODULE_WEATHER;
                    active_info.handle_msg_callback = HandlePdpMsg;
                    active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
                    if(MMIAPIPDP_Active(&active_info))
                    {
                        StartPdpActiveTimer();
                        return_val = TRUE;
                    }
                }
                else
#endif //WIFI_SUPPORT
                {
                    uint32 sim_ok_num = 0;
                    uint16 sim_ok = MN_DUAL_SYS_1;

                    cur_sys_num   = MN_DUAL_SYS_1;
                    sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);/*lint !e64*/
                    cur_sys_num = (MN_DUAL_SYS_E)sim_ok;
                    SCI_TRACE_LOW("[weather]:sim_ok_num = %d,sim_ok = %d",sim_ok_num,sim_ok);
                    if(0 == sim_ok_num)
                    {
                        MMIAPIPHONE_AlertSimNorOKStatus();
                        break;
                    }
                    
                    return_val = WeatherPDPTry(cur_sys_num);//第一张有效卡
                    if(FALSE == return_val)
                    {
            		    if(0 != s_pdp_retry_timer_id)
            		    {
                            MMK_StopTimer(s_pdp_retry_timer_id);
                            s_pdp_retry_timer_id = 0;
            		    }
            		    s_pdp_retry_timer_id = MMK_CreateTimerCallback(
            	                                                    PDP_RETRY_TIMEOUT, 
                                                    		        HandlePdpRetryTimer, 
                                                    		        PNULL, 
                                                        		    FALSE
                                                        		    );
                    }
                }
            }
            break;
    
        case WEATHER_NETWORK_SIM1:
        case WEATHER_NETWORK_SIM2:
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        case WEATHER_NETWORK_SIM3:
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case WEATHER_NETWORK_SIM4:
#endif   
            {
                dual_sys = (MN_DUAL_SYS_E)(g_weather_data.setting_info.network - 1);
                if(dual_sys < MMI_DUAL_SYS_MAX && MMIAPIPHONE_IsSimOk(dual_sys))
                {
                    active_info.app_handler         = MMI_MODULE_WEATHER;
                    active_info.dual_sys            = dual_sys;
                    active_info.apn_ptr             = GetApnStr(dual_sys);
                    active_info.user_name_ptr       = GetUserNameStr(dual_sys);
                    active_info.psw_ptr             = GetPasswordStr(dual_sys);
                    active_info.priority            = 3;
                    active_info.ps_service_rat      = MN_UNSPECIFIED;
                    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
                    active_info.handle_msg_callback = HandlePdpMsg;
                    active_info.ps_service_type = BROWSER_E;
                    active_info.storage = MN_GPRS_STORAGE_ALL;

                    if(MMIAPIPDP_Active(&active_info))
                    {
                        StartPdpActiveTimer();
                        return_val = TRUE;
                    }
                }
            }
            break;
#ifdef MMI_WIFI_SUPPORT
        case WEATHER_NETWORK_WIFI:
            {              
                if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
                {
                    active_info.app_handler         = MMI_MODULE_WEATHER;
                    active_info.handle_msg_callback = HandlePdpMsg;
                    active_info.ps_interface        = MMIPDP_INTERFACE_WIFI;
                    if(MMIAPIPDP_Active(&active_info))
                    {
                        StartPdpActiveTimer();
                        return_val = TRUE;
                    }
                }
            }
            break;
#endif // WIFI_SUPPORT    
            
        default:
            break;
    }   
    
    if(!return_val)
    {
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_CONN_FAILED_CNF);
    }

    return return_val;

}

/*****************************************************************************/
//  Description : Deactive Pdp
//  Global resource dependence : none
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_PdpDeactive(void)
{
    BOOLEAN return_val = FALSE;
    
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_PdpDeactive---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1195_112_2_18_3_6_34_16,(uint8*)"");
    
    MMIWEATHER_WeatherInfoRelease();  //compatible modification to store weather information with buffer and file mode;
    return_val = MMIAPIPDP_Deactive(MMI_MODULE_WEATHER);

    return return_val;
}


/*****************************************************************************/
//  Description : is supporting city
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
/*PUBLIC BOOLEAN MMIWEATHER_IsSupportCity(wchar *city_name)
{
    BOOLEAN ret_val = FALSE;
    uint32 i = 0;

    if(PNULL != city_name)
    {
        char city_name_gb_str[(MMIWEATHER_MAX_CITY_NAME_LEN +1) * 2 ] = {0};
        uint32 wstr_len = MMIAPICOM_Wstrlen(city_name);

        //Translate unicode to GB code
        GUI_WstrToGB((uint8*)city_name_gb_str, city_name, wstr_len);
        
        for(i = 0; i < ARR_SIZE(s_city_table); i++)
        {
            if(0 == strcmp(city_name_gb_str, s_city_table[i].city_name))
            {
                //SCI_TRACE_LOW:"[weather]:MMIWEATHER_IsSupportCity---TRUE---"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1225_112_2_18_3_6_34_17,(uint8*)"");
                ret_val = TRUE;
                break;
            }
            
			#if 0            
            else if(MMIAPICOM_WstrStrncmp(MMIAPICOM_Wstrlower(city_name), 
                                        s_city_table[i].city_code, 
                                        MMIAPICOM_Wstrlen(city_name)))
            {
                ret_val = TRUE;
                break;
            }
			#endif            
        }
    }
    
	//SCI_TRACE_LOW:"[weather]:MMIWEATHER_IsSupportCity---res_val = %d "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1242_112_2_18_3_6_34_18,(uint8*)"d" ,ret_val);

    return ret_val;
}

*/
/*****************************************************************************/   
//  Description : is supporting city
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
/*PUBLIC char* MMIWEATHER_GetCityCodeByName(wchar *city_name)
{
    char*   city_code  = PNULL;
    
	//SCI_TRACE_LOW:"[weather]:MMIWEATHER_GetCityCodeByName---"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1256_112_2_18_3_6_34_19,(uint8*)"");

    if(PNULL != city_name)
    {
        uint32 i = 0;
        char city_name_gb_str[MMIWEATHER_MAX_CITY_CODE_LEN + 1] = {0};
        uint32 wstr_len = MMIAPICOM_Wstrlen(city_name);
        
        GUI_WstrToGB((uint8*)city_name_gb_str, city_name, wstr_len);

        for(i = 0; i < ARR_SIZE(s_city_table); i++)
        {
            if(0 == strncmp(city_name_gb_str, (char*)s_city_table[i].city_name, strlen(city_name_gb_str))) //lint64
            {
                city_code = (char*)s_city_table[i].city_code;
                break;  
            }
        }
    }
    
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_GetCityCodeByName---city_code = %s "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1276_112_2_18_3_6_34_20,(uint8*)"s" ,city_code);

    return city_code;
     
}*/


/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if((PNULL != param)&&(PNULL != signal_struct_ptr)&&(struct_size >= sizeof(xSignalHeaderRec)))
    {
        SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size));
    }
}

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMIWEATHER_HTTP_CONTEXT_T* GetHttpContextPtr(void)
{
    return &s_http_context;
}

/*var type*/
#define URL_ESCAPE       0    // $(var:escape)
#define URL_NOCHANGE     1    // $(var:nochange)
#define URL_UNESCAPE     2    // $(var:unescape)

LOCAL BOOLEAN IsUnreservedChar(uint8 c)
{
    if( (c>='0'&&c<='9') ||
        (c>='a'&&c<='z') ||
        (c>='A'&&c<='Z') ||
        c=='-' || c=='_' || c=='.' || c=='/'/*|| c=='!' || c=='~' || c=='*' || c=='\''  || c=='(' || c==')'*/) 
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Escape URL
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint32 encode_url(uint8 *dst_ptr, uint32 dst_len, const uint8 *url_ptr, char var_type)
{
    uint8 c = 0;
    uint32 ret_len = 0;

    if(PNULL == dst_ptr || PNULL == url_ptr)
    {
        return 0;
    }

    while (0 != (c = *url_ptr++))
    {
        if (IsUnreservedChar(c) || URL_UNESCAPE == var_type)
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = c;
            }
            else
            {
                break;
            }
        }
#ifndef MMI_WEATHER_JSON_PARSE
        else if (' ' == c)
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = '+';
            }
            else
            {
                break;
            }
        }
#endif
        else
        {
            char str[10] = {0};
            uint16 len = 0;

            sprintf(str, "%02X", c);
            len = strlen(str);

            if (ret_len + len <= dst_len)
            {
                dst_ptr[ret_len ++] = '%';

                SCI_MEMCPY(dst_ptr+ ret_len, str, len);

                ret_len += len;
            }
            else
            {
                break;
            }
        }
    }

    return ret_len;
}
#ifdef MMI_WEATHER_JSON_PARSE
/*****************************************************************************/
//  Description : get real url
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void GetWDURL(char * dest,char * url_city,uint32 dest_max_len)
{
    uint32 total_len = strlen(HTTP_URI_WD_STRING_1) + strlen(HTTP_URI_WD_STRING_ID)
                        + strlen(HTTP_URI_WD_STRING_QUERY)+strlen(url_city)/*lint !e64*/
                        +strlen(HTTP_URI_WD_STRING_TYPE);
    if(total_len > dest_max_len)
    {
        SCI_TRACE_LOW("[weather]:GetRealURL total_len = %d ,dest_max_len = %d",total_len,dest_max_len);
        return;
    }
    strcpy(dest,HTTP_URI_WD_STRING_1);/*lint !e64*/
    strcat(dest,HTTP_URI_WD_STRING_ID);    /*lint !e64*/
    strcat(dest,HTTP_URI_WD_STRING_QUERY);/*lint !e64*/
    strcat(dest,url_city);/*lint !e64*/
    strcat(dest,HTTP_URI_WD_STRING_TYPE);/*lint !e64*/
}
#endif

#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
/*****************************************************************************/
//  Description : handle Md5 str
//  Global resource dependence : 
//  Author: juan.wu
//  Note:if there has a "/" in the str,change it to"%2F"
/*****************************************************************************/
LOCAL void HandleMd5Str(uint8 * md5_str ,uint16 md5_max_len )
{
    uint32  slash_idx = 0;
    uint8 * dest_ptr = PNULL;
    uint8 * temp_str_ptr = PNULL;
    uint32 cpy_len = 0;

    if(PNULL == md5_str || 0 == md5_max_len)
    {
        SCI_TRACE_LOW("HandleMd5Str param error md5_str = %x, md5_max_len = 0",md5_str,md5_max_len);
        return;
    }
    
    dest_ptr = (uint8 *)strstr((char *)md5_str,"/");
    if(0 != dest_ptr)
    {
        temp_str_ptr = SCI_ALLOC_APPZ(md5_max_len + 1);
        if(PNULL == temp_str_ptr)
        {
            SCI_TRACE_LOW("HandleMd5Str PNULL == temp_str_ptr");
            return;

        }
        slash_idx = dest_ptr - md5_str;
        if(md5_max_len >= slash_idx)
        {
            SCI_MEMCPY(temp_str_ptr,md5_str,slash_idx);
        }
        if(md5_max_len >= slash_idx +3)
        {
            temp_str_ptr[slash_idx]   = '%';
            temp_str_ptr[slash_idx+1] = '2';
            temp_str_ptr[slash_idx+2] = 'F';
        }
        cpy_len = strlen(md5_str)-slash_idx-1; /*lint !e64*/ 
        if(md5_max_len >= slash_idx +3 + cpy_len)
        {
            SCI_MEMCPY(temp_str_ptr+slash_idx+3,md5_str+slash_idx+1,cpy_len);
        }
        SCI_MEMSET(md5_str,0,md5_max_len);
		cpy_len = strlen(temp_str_ptr); /*lint !e64*/ 
        if(md5_max_len >= cpy_len)
        {
            SCI_MEMCPY(md5_str,temp_str_ptr,cpy_len);
        }
        SCI_FREE(temp_str_ptr);
    }
}

/*****************************************************************************/
//  Description : get secret string
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void GetSecretString(char* city_name,char *secret_ptr,uint32 secret_len)
{
    uint32      time = 0;
    SCI_DATE_T  sys_date = {0};
    SCI_TIME_T  sys_time = {0};
    uint8      i = 0;
    char signtxt_array[3]={0};
    char signtxt_str[36]={0};
    char     end_str[100+1] = {0};
    uint8 *   md5_str_ptr = PNULL;
    uint8     md5_out[16+1]  = {0};
    char    time_str[24+1]= {0};
    uint32   md5_max_len = 0;
    
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);
    SCI_TRACE_LOW("weather:GetSecretString day = %d,month = %d,year = %d",sys_date.mday,sys_date.mon,sys_date.year);
    SCI_TRACE_LOW("weather:GetSecretString sec = %d,min = %d,hour = %d",sys_time.sec,sys_time.min,sys_time.hour);
    time = MMIAPICOM_Tm2Second(sys_time.sec, 
                                sys_time.min, 
                                sys_time.hour, 
                                sys_date.mday, 
                                sys_date.mon, 
                                sys_date.year);
    /* 取到的秒数为1980年以来的秒数，还需要加上70~80的秒数 */
    time += M_SECOND_1970_TO_1980;
    sprintf(time_str,"%d",time);
    SCI_TRACE_LOW("weather:GetSecretString time = %d",time);
    
    md5_max_len = strlen(HTTP_SECRET_MD5_BASE) +strlen(time_str)+ strlen(city_name) + 3;
    md5_str_ptr = SCI_ALLOC_APPZ(md5_max_len +1);
    if(PNULL == md5_str_ptr)
    {
        SCI_TRACE_LOW("GetSecretString PNULL == md5_str_ptr");
        return;
    }
    sprintf(md5_str_ptr, HTTP_SECRET_MD5_BASE, city_name,time);/*lint !e64*/ 
    HandleMd5Str(md5_str_ptr,md5_max_len);	
    
    MMIAPICOM_ProduceSgnMD5Str(md5_str_ptr, strlen(md5_str_ptr), md5_out); /*lint !e64*/  
    SCI_FREE(md5_str_ptr);
    md5_str_ptr = PNULL;
    
	for(i = 0 ; i < 16; i++)
	{
		sprintf(signtxt_array,"%02X",md5_out[i]);
        if((strlen(signtxt_str) + 2) <= 35)
        {
    		memcpy(signtxt_str+2*i,signtxt_array,2); 
        }
	}
    sprintf(end_str, HTTP_URI_STRING_END, time); 
    
    if(secret_len >= strlen(HTTP_SECRET_BEGIN))
    {
        strcpy(secret_ptr,HTTP_SECRET_BEGIN);
    }
    if(secret_len >= strlen(secret_ptr)+strlen(signtxt_str))
    {
        strcat(secret_ptr,signtxt_str);  
    }
    if(secret_len >= strlen(secret_ptr)+strlen(end_str))
    {
        strcat(secret_ptr,end_str);
    }
}
/*****************************************************************************/
//  Description : get url
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void GetRealURL(char * dest,char * url_city,uint32 dest_max_len)
{
    char  weather_srv[2] = {0}; 
    char  *secret_ptr = PNULL;
    
    SCI_TRACE_LOW("GetRealURL s_is_url_retry = %d,url_idx = %d",s_is_url_retry,g_url_idx);
    secret_ptr = SCI_ALLOC_APPZ(strlen(HTTP_URI_SECRET_STRING_EXAMPLE)+1);
    if(PNULL == secret_ptr)
    {
        SCI_TRACE_LOW("[weather]:GetRealURL NO MEMORY");
        return;
    }
    GetSecretString(url_city,secret_ptr,strlen(HTTP_URI_SECRET_STRING_EXAMPLE));
    SCI_TRACE_LOW("weather:GetRealURL secret string = %s ",secret_ptr);

    if(!s_is_url_retry)
    {
        g_url_idx = ReadWeatherURLIndex();
    }
    if(g_url_idx < HTTP_URI_MAX_NUM)
    {
        if(HTTP_URI_USE_WD_INDEX == g_url_idx)
        {
            GetWDURL(dest,url_city,dest_max_len);
        }
        else
        {
            SCI_MEMSET(dest,0,dest_max_len);
            if(dest_max_len > strlen(HTTP_URI_STRING_1))
            {
                strcpy(dest,HTTP_URI_STRING_1);
            }
            sprintf(weather_srv, "%d", g_url_idx);
            if(dest_max_len > strlen(dest)+strlen(weather_srv))
            {
                strcat(dest,weather_srv);  
            }
            if(dest_max_len > strlen(dest)+strlen(HTTP_URI_STRING_QUERY))
            {
                strcat(dest,HTTP_URI_STRING_QUERY);
            }
            if(dest_max_len > strlen(dest)+strlen(url_city))
            {
                strcat(dest,url_city);
            }
            if(dest_max_len > strlen(dest) + strlen((char*)secret_ptr))
            {
                strcat(dest,(char*)secret_ptr);
            }
        }
    }    
    else
    {
        SCI_TRACE_LOW("weather:GetRealURL g_url_idx = %d is error!",g_url_idx);
    } 
    SCI_FREE(secret_ptr);
}
/*****************************************************************************/
//  Description :retry the next url,to find a workable url
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandleURLRetry(void)
{
    MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr = PNULL;
    HTTP_CONTEXT_ID_T   context_id = 0;

    SCI_TRACE_LOW("weather:HandleURLRetry ENTER");
    MMK_StopTimer(s_url_request_timer);
    s_url_request_timer = 0;
    if(!s_is_url_retry)
    {
        s_is_url_retry = TRUE;
    }
    g_url_idx++;
    http_context_ptr = GetHttpContextPtr();

    if(PNULL != http_context_ptr)
    {
        context_id   = http_context_ptr->context_id;
        HTTP_CloseRequest(context_id);
    }
    if(HTTP_URI_USE_WD_INDEX == g_url_idx)
    {
        StartHttpRequest();
    }
    else
    {
        if(g_url_idx >= HTTP_URI_MAX_NUM)
        {
            g_url_idx = 0;
            MMK_StopTimer(s_url_request_timer);
            s_url_request_timer = 0;
            s_is_url_retry = FALSE;
            WriteWeatherURLIndex(g_url_idx);
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_CONN_FAILED_CNF);
            SCI_TRACE_LOW("weather:HandleURLRetry reset variable");
        }
        else
        {
            StartHttpRequest();
        }
    }
    SCI_TRACE_LOW("HandleURLRetry g_url_idx = %d",g_url_idx);
}

#endif

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpInitCnf(MMIWEATHER_HTTP_CONTEXT_T * http_context_ptr, DPARAM param)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_GET_PARAM_T*   get_param_ptr = PNULL;
    HTTP_APP_PARAM_T app_param = 0;
    int len =0;
    //char * city_code = PNULL;

    if(PNULL == http_context_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_TRACE_LOW("HandleHttpInitCnf Enter");
    //---1--- 
    GetSignalStruct(param, &signal, sizeof(HTTP_INIT_CNF_SIG_T));
    app_param = signal.app_param;   //可以传app需要的参数过来。暂未用到。

    http_context_ptr->context_id   = signal.context_id;
    http_context_ptr->app_instance = 0;
    http_context_ptr->module_id    = signal.module_id;

    //---2---
    if (MMI_MODULE_WEATHER != http_context_ptr->module_id)
    {
        //SCI_TRACE_LOW:"[weather]:HandleHttpInitCnf---Not weather task---"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1412_112_2_18_3_6_34_21,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    //---3---
    get_param_ptr = SCI_ALLOCA(sizeof(HTTP_GET_PARAM_T));
    
    if(PNULL == get_param_ptr)
    {
        //SCI_TRACE_LOW:"[weather]:HandleHttpInitCnf--- no memory---"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1421_112_2_18_3_6_34_22,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));

    //accept
    len = strlen(ACCEPT_STRING);
    get_param_ptr->accept.accept_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->accept.accept_ptr,ACCEPT_STRING);
    //accept language
    len = strlen(ACCEPT_LANGUAGE_STRING);
    get_param_ptr->accept_language.accept_language_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->accept_language.accept_language_ptr,ACCEPT_LANGUAGE_STRING);
    //UA
    len = strlen(USER_AGENT_STRING);
    get_param_ptr->user_agent.user_agent_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->user_agent.user_agent_ptr,USER_AGENT_STRING);
    //URI
#ifdef WIN32
    len = strlen(HTTP_URI_STRING);
    get_param_ptr->uri.uri_ptr = SCI_ALLOC_APPZ(len+1);
    strcpy(get_param_ptr->uri.uri_ptr,HTTP_URI_STRING);
#else
    {
        char city_code[MMIWEATHER_MAX_CITY_CODE_LEN + 1] = {0};
        char url_city[MMIWEATHER_MAX_CITY_CODE_LEN*3+1] = {0};
        MMIWEATHER_GetCityCode(city_code);
        encode_url((uint8 *)url_city, 3 * MMIWEATHER_MAX_CITY_CODE_LEN,(const uint8 *)city_code, URL_ESCAPE);
#ifdef MMI_WEATHER_JSON_PARSE
        {
            uint32 url_len = 0;
#ifdef MMI_WEATHER_SUPPORT_WUNDERGROUND
            url_len = strlen(HTTP_URI_WD_STRING_BASE) + strlen(HTTP_URI_WD_STRING_TYPE)  
                                                        +MMIWEATHER_MAX_CITY_CODE_LEN*3 +1;//add"/"

#elif defined(MMIWEATHER_SUPPORT_SPRD_SRV)
            url_len = strlen(HTTP_URI_STRING_BASE) + strlen(HTTP_URI_SECRET_STRING_EXAMPLE)
                                                    +MMIWEATHER_MAX_CITY_CODE_LEN*3 + 1;//add"/"
#else
            SCI_FREE(get_param_ptr->accept.accept_ptr);
            SCI_FREE(get_param_ptr->accept_language.accept_language_ptr);
            SCI_FREE(get_param_ptr->user_agent.user_agent_ptr);            
            SCI_TRACE_LOW("weather:macro error! no macro defined in weather");
            return MMI_RESULT_FALSE;
            
#endif
//#if defined(MMI_WEATHER_SUPPORT_WUNDERGROUND) || defined(MMIWEATHER_SUPPORT_SPRD_SRV)
#ifdef MMI_WEATHER_JSON_PARSE
            get_param_ptr->uri.uri_ptr = SCI_ALLOC_APPZ(url_len + 1);
            GetWDURL(get_param_ptr->uri.uri_ptr,url_city,url_len); /*lint !e64*/ 
#endif
        }
#else
        get_param_ptr->uri.uri_ptr = SCI_ALLOC_APPZ(strlen(HTTP_URI_STRING_BASE) + MMIWEATHER_MAX_CITY_CODE_LEN*3 + 1);
        strcpy(get_param_ptr->uri.uri_ptr,HTTP_URI_STRING_BASE);
        strcat(get_param_ptr->uri.uri_ptr,url_city);
#endif
    }
#endif
    //SCI_TRACE_LOW:"[weather]: uri.uri_ptr = %s ---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1454_112_2_18_3_6_35_23,(uint8*)"s",get_param_ptr->uri.uri_ptr);
    //accept charset
    len = strlen(ACCEPT_CHARSET_STRING);
    get_param_ptr->accept_charset.accept_charset_ptr = SCI_ALLOC_APPZ(len + 1);
    strcpy(get_param_ptr->accept_charset.accept_charset_ptr,ACCEPT_CHARSET_STRING);

#ifndef WEATHER_INFORMATION_BUFFER_STORE_SUPPORT
    //File name
    get_param_ptr->get_data.is_header_use_file = TRUE;
    memcpy(get_param_ptr->get_data.header_file_name, HTTP_HEADER_FILE_NAME, 50);
    memcpy(get_param_ptr->get_data.content_file_name,HTTP_CONTENT_FILE_NAME, 50);
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_FILE;
#else
    //buffer store;
    get_param_ptr->get_data.is_header_use_file = FALSE;
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_BUFFER;
#endif
    
    //Connection
    get_param_ptr->connection = HTTP_CONNECTION_KEEP_ALIVE;
    get_param_ptr->need_net_prog_ind = FALSE;
    //---5---
    HTTP_GetRequest(http_context_ptr->context_id, get_param_ptr, http_context_ptr->app_instance);
    //---6---
    SCI_FREE(get_param_ptr->accept.accept_ptr);
    SCI_FREE(get_param_ptr->accept_charset.accept_charset_ptr);
    SCI_FREE(get_param_ptr->accept_language.accept_language_ptr);
    SCI_FREE(get_param_ptr->user_agent.user_agent_ptr);
    SCI_FREE(get_param_ptr->uri.uri_ptr);
    SCI_FREE(get_param_ptr);

#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
    if(0 != s_url_request_timer)
    {
        MMK_StopTimer(s_url_request_timer);
        s_url_request_timer = 0;
    }
    s_url_request_timer = MMK_CreateTimerCallback(30000, HandleURLRetry, PNULL, FALSE);/*lint !e64*/ 

#endif    
    return result;
}

#ifdef WEATHER_INFORMATION_BUFFER_STORE_SUPPORT
/*****************************************************************************/
//  Description : HandleHttpWeatherDataInd
//  Global resource dependence : 
//  Author: Glen Li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpWeatherDataInd(MMIWEATHER_HTTP_CONTEXT_T * http_context_ptr, DPARAM param)
{
    //---1--- 
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    HTTP_DATA_IND_SIG_T signal = {0};
    uint32 response_code = 0;
    uint32 data_len = 0;
    uint8 *data_ptr = PNULL;
    uint32 buffer_len = 0;
    char * temp_ptr = PNULL;
    uint32 temp_len = 0;
    uint32 cpy_len  = 0;

    //SCI_TRACE_LOW:"[weather]:HandleHttpWeatherDataInd---receive weather information as buffer mode---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1503_112_2_18_3_6_35_24,(uint8*)"");

    if(PNULL == http_context_ptr)
    {
        return MMI_RESULT_FALSE;
    }
     
    GetSignalStruct(param, &signal, sizeof(HTTP_DATA_IND_SIG_T));
    
    http_context_ptr->request_id = signal.request_id;
    http_context_ptr->context_id = signal.context_id;
    http_context_ptr->app_instance = signal.app_instance;
    
    response_code = signal.response_code;
        
    data_len = signal.data_len;
    data_ptr = signal.data_ptr;
    SCI_TRACE_LOW("HandleHttpWeatherDataInd len = %d",signal.data_len);
    SCI_TRACE_LOW("HandleHttpWeatherDataInd data_ptr = %s",signal.data_ptr);

    if(PNULL != s_http_content_weather_buffer_address)
    {
        temp_len = strlen(s_http_content_weather_buffer_address);
    }
    else
    {
        SCI_TRACE_LOW("HandleHttpWeatherDataInd no buffer!");
        return MMI_RESULT_FALSE;
    }
    SCI_TRACE_LOW("HandleHttpWeatherDataInd temp_len = %d",temp_len);

    buffer_len = temp_len + signal.data_len;
    SCI_TRACE_LOW("HandleHttpWeatherDataInd buffer_len = %d",buffer_len);
    if(PNULL != s_http_content_weather_buffer_address)
    {
        if(buffer_len > HTTP_CONTENT_BUFFER_SIZE && 0 != temp_len)
        {
            temp_ptr = SCI_ALLOC_APPZ(temp_len +1 );
            if(PNULL == temp_ptr)
            {
                SCI_TRACE_LOW("HandleHttpWeatherDataInd PNULL == temp_ptr");
                return MMI_RESULT_FALSE;
            }
            
            SCI_MEMCPY(temp_ptr,s_http_content_weather_buffer_address,temp_len);
            SCI_FREE(s_http_content_weather_buffer_address);
            s_http_content_weather_buffer_address = SCI_ALLOC_APPZ(buffer_len+1);
            cpy_len = MIN(temp_len,buffer_len);
            SCI_MEMCPY(s_http_content_weather_buffer_address,temp_ptr,cpy_len);
            SCI_FREE(temp_ptr);
            temp_ptr = PNULL;
            SCI_TRACE_LOW("HandleHttpWeatherDataInd len1= %d,len2 = %d,len3 = %d",strlen(s_http_content_weather_buffer_address),data_len,buffer_len);
            if(strlen(s_http_content_weather_buffer_address) + data_len <= buffer_len)
            {
                SCI_MEMCPY((s_http_content_weather_buffer_address + cpy_len),data_ptr,data_len);
            }
        }
        else
        {
            if(0 == temp_len)
            {
                if(data_len > HTTP_CONTENT_BUFFER_SIZE)
                {
                    SCI_FREE(s_http_content_weather_buffer_address);
                    s_http_content_weather_buffer_address = SCI_ALLOC_APPZ(data_len+1);
                    SCI_MEMCPY(s_http_content_weather_buffer_address,data_ptr,data_len);
                }
                else
                {
                    SCI_MEMCPY(s_http_content_weather_buffer_address,data_ptr,data_len);

                }
            }
            else
            {
                if(strlen(s_http_content_weather_buffer_address) + data_len <= HTTP_CONTENT_BUFFER_SIZE)
                {
                    SCI_MEMCPY((s_http_content_weather_buffer_address + temp_len),data_ptr,data_len);
                }
            }
        }   
    }
    return result;
}
#endif

/*****************************************************************************/
//  Description : Handle Http Need Auth Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpNeedAuthInd(MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpRequestIdInd(MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};

    if(PNULL == http_context_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_REQUEST_ID_IND_SIG_T));

    http_context_ptr->context_id = signal.context_id;
    http_context_ptr->request_id = signal.request_id;

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpGetCnf(MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_GET_CNF_SIG_T signal = {0};

    if(PNULL == http_context_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_GET_CNF_SIG_T));
    
    http_context_ptr->context_id = signal.context_id;
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
        if(0 != s_url_request_timer)
        {
            MMK_StopTimer(s_url_request_timer);
            s_url_request_timer = 0;
            SCI_TRACE_LOW("HandleHttpGetCnf stop url request timer!");
        }
        if(s_is_url_retry)
        {
            s_is_url_retry = FALSE;
            WriteWeatherURLIndex(g_url_idx);
        }
#endif

    if(signal.result != HTTP_SUCCESS)
    {
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_CONN_FAILED_CNF);
    }
    else
    {
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_PARSE_START_IND);
    }

    HTTP_CloseRequest(http_context_ptr->context_id);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Header Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpHeaderInd(MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //当APP设置content使用buffer方式时，HTTP内容会保存成buffer方式,并发该消息给APP
    return result;
}

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpErrorInd(MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E            result      = MMI_RESULT_TRUE;
    HTTP_ERROR_IND_SIG_T    signal      = {0};
    HTTP_CONTEXT_ID_T       context_id  = 0;
    HTTP_ERROR_E            http_result = HTTP_SUCCESS;         //http error
    
    GetSignalStruct(param, &signal, sizeof(HTTP_ERROR_IND_SIG_T));
    context_id  = signal.context_id;
    http_result = signal.result;
    //SCI_TRACE_LOW:"[weather]:HandleHttpErrorInd error = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1620_112_2_18_3_6_35_25,(uint8*)"d",http_result);
    if(signal.result != HTTP_SUCCESS)
    {
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_CONN_FAILED_CNF);
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
        if(0 != s_url_request_timer)
        {
            MMK_StopTimer(s_url_request_timer);
            s_url_request_timer = 0;
        }
#endif
    }
    
    HTTP_CloseRequest(context_id); 

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpCloseCnf(MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    //成功关闭HTTP
    http_context_ptr->app_instance = 0;
    http_context_ptr->context_id   = 0;
    http_context_ptr->request_id   = 0;
    http_context_ptr->module_id    = 0;

    MMIWEATHER_SendMsgToApp(MSG_WEATHER_HTTP_CLOSE_CNF);
    return result;
}

/*****************************************************************************/
//  Description : Handle Http Cancel Cnf
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpCancelInd(MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_CANCEL_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;

    if(PNULL == http_context_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    GetSignalStruct(param, &signal, sizeof(HTTP_CANCEL_CNF_SIG_T));
    context_id = signal.context_id;
    HTTP_CloseRequest(context_id);
    MMIWEATHER_SendMsgToApp(MSG_WEATHER_UPDATE_CANCEL_CNF);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Net Prog Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpNetProgInd(MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpRedirectInd(MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_REDIRECT_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;

    if(PNULL == http_context_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_REDIRECT_IND_SIG_T));

    context_id = signal.context_id;


    return result;
}

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void StartHttpRequest(void)
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    init_param_ptr = SCI_ALLOC_APPZ(sizeof(HTTP_INIT_PARAM_T));
    SCI_TRACE_LOW("[weather]:StartHttpRequest");
    if(0 != s_net_id)
    {
        init_param_ptr->net_id = s_net_id;
    }
    init_param_ptr->is_cookie_enable = FALSE;
    init_param_ptr->is_cache_enable = FALSE;
    HTTP_InitRequest(MMI_MODULE_WEATHER, 1, init_param_ptr);
    SCI_FREE(init_param_ptr);
    init_param_ptr = PNULL;
}

/*****************************************************************************/
//  Description : Initialize http request
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void CancelHttpRequest()
{
    MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr = PNULL;
    HTTP_CONTEXT_ID_T   context_id = 0;
    HTTP_APP_INSTANCE_T app_instance = 0;
    HTTP_REQUEST_ID_T   request_id = 0;
    
    http_context_ptr = GetHttpContextPtr();

    if(PNULL != http_context_ptr)
    {
        context_id   = http_context_ptr->context_id;
        app_instance = http_context_ptr->app_instance;
        request_id   = http_context_ptr->request_id;
        HTTP_CancelRequest(context_id,app_instance,request_id); 
    }

}

/*****************************************************************************/
//  Description : handle open wait window message
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCancelUpdateWaitingWinMsg(
                                                   MMI_WIN_ID_T win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param
                                                   )
{
    MMI_RESULT_E    result   = MMI_RESULT_TRUE;  
    
    switch(msg_id)
    {
        case MSG_APP_RED:
            MMIWEATHER_SetUpdateState(FALSE);
            result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
            break; 

        default:
            result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
            break;
    }
    return result;
}

#ifdef MMI_WEATHER_JSON_PARSE
/*****************************************************************************/
//  Description : JsonParse_CallBack
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL int JsonParse_CallBack(void* ctx, int type, const JSON_value* value)
{
    int res = 1;
    
    SCI_TRACE_LOW("JsonParse_CallBack type = %d",type);
    
    switch(type) 
    {
    case JSON_T_INTEGER:        // 5
        MMIWEATHER_GetJsonNumber(ctx,value);
        break;
        
    case JSON_T_FLOAT:   
    case JSON_T_STRING:         // 10
        SCI_TRACE_LOW("JsonParse_CallBack get string ");
        MMIWEATHER_GetJsonString(ctx,value);
         break;
         
    case JSON_T_KEY:            // 11
        MMIWEATHER_GetJsonTAG(ctx,value);
        break;
    
    default:
        res = -1;
        break;
    } 
    return res;
} 
#endif
/*****************************************************************************/
//  Description : change chinese name to alphabetic name
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void WeatherGetCityLetters(wchar *city_name_ptr, wchar * city_code_ptr)
{
    uint16 unicode_len = MMIAPICOM_Wstrlen(city_name_ptr);
    uint8  i = 0;
    uint16 letters[MMIWEATHER_MAX_CITY_NAME_LEN + 1] = {0};
    wchar  buffer[MMIWEATHER_MAX_CITY_CODE_LEN + 1] = {0};
    uint16 letter_len = 0;

    if(PNULL == city_name_ptr || PNULL == city_code_ptr)
    {
        return;
    }

    for(i = 0; i < unicode_len; i++)
    {
        MMIAPIIM_GetTextLetters(city_name_ptr[i], letters);
        letter_len += MMIAPICOM_Wstrlen(letters);
        if(letter_len <= MMIWEATHER_MAX_CITY_CODE_LEN)
        {
            MMIAPICOM_Wstrcat(buffer, letters);
        }
    }
    SCI_MEMSET(city_code_ptr,0,MMIWEATHER_MAX_CITY_CODE_LEN);
    letter_len = MIN(letter_len,MMIWEATHER_MAX_CITY_CODE_LEN);
    MMIAPICOM_Wstrncpy(city_code_ptr, buffer, letter_len);
    SCI_TRACE_LOW("weather:WeatherGetCityLetters city_code = %s,letter_len = %d",city_code_ptr,letter_len);
}
LOCAL BOOLEAN NeedDeactedPDP(void)
{
    //if need retry or will go another url,it will not deactive pdp
    if(s_is_retry_en_name 
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
     || s_is_url_retry 
#endif
    )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
/*****************************************************************************/
//  Description : Handle weather app Msg
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherAppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMIWEATHER_UPDATE_FROM_E update_type = MMIWEATHER_GetUpdateType();

#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
    //if the error msg has been showed ,it will not prompt here;
    BOOLEAN          is_need_show_error_msg = !MMIWEATHER_IsErrorMsgShowed();
#endif
    //SCI_TRACE_LOW:"[weather]---HandleWeatherMsg---:msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1783_112_2_18_3_6_35_26,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_WEATHER_PDP_ACTIVE_CNF: 
        {
            //SCI_TRACE_LOW:"[weather]---MSG_WEATHER_PDP_ACTIVE_CNF---"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1788_112_2_18_3_6_35_27,(uint8*)"");

            MMIAPIFMM_DeleteFile(HTTP_HEADER_FILE_NAME, PNULL);
            MMIAPIFMM_DeleteFile(HTTP_CONTENT_FILE_NAME, PNULL);
            
            MMIWEATHER_WeatherInfoAlloc();  //compatible modification to store weather information with buffer and file mode;

            StartHttpRequest();
        }
        break;
    case MSG_WEATHER_PDP_ACTIVE_FIILED_CNF:
    case MSG_WEATHER_CONN_FAILED_CNF:
    case MSG_WEATHER_PARSE_FAILED_CNF:
        {
            //SCI_TRACE_LOW:"[weather]---MSG_WEATHER_PDP_ACTIVE_FIILED_CNF---"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1802_112_2_18_3_6_35_28,(uint8*)"");
            MMK_CloseWin(MMIWEATHER_PUB_WIN_ID);
            MMIWEATHER_SetUpdateState(FALSE);
            switch(update_type)		   	
            {
            case MMIWEATHER_UPDATE_FROM_WIDGET:
                {
#if defined MMI_WIDGET_WEATHER1
                    MMK_PostMsg(g_weather_widget.win_id, MSG_WEATHER_WGT_UPDATE_CNF, PNULL, 0);  
#endif
#if defined MMI_WIDGET_WEATHER2
                    MMK_PostMsg(g_weather_widget2.win_id, MSG_WEATHER_WGT_UPDATE_CNF, PNULL, 0);
#endif
                }
                break;			            
            case MMIWEATHER_UPDATE_FROM_MAIN_WIN:
                {
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
                    if(is_need_show_error_msg)
#endif
                    {
                        MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_FAIL, TXT_COMMON_LINK_FAILED, PNULL);
                    }
                }
                break;			            
            case MMIWEATHER_UPDATE_FROM_AUTO:
                {
                    //Do nothing if update failed.
                }
                break;	

            case MMIWEATHER_UPDATE_FROM_ADD_CITY:
                {
                    uint8   city_num = 0;

                    city_num = MMIWEATHER_GetCityNum();                   
                    if(city_num < MMIWEATHER_MAX_CITY_NUM)
                    {
                        MMIWEATHER_WEATHER_T* weather_info_ptr = MMIWEATHER_GetCityWeatherInfo(city_num);
                        SCI_MEMSET(weather_info_ptr->city_name, 0, sizeof(weather_info_ptr->city_name));
                        SCI_MEMSET(weather_info_ptr->city_code, 0, sizeof(weather_info_ptr->city_code));
                    }
                    //SCI_TRACE_LOW:"MMIWEATHER_UPDATE_FROM_ADD_CITY failed city_num = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1838_112_2_18_3_6_35_29,(uint8*)"d",city_num);
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
                    if(is_need_show_error_msg)
#endif
                    {
                        MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_FAIL, TXT_COMMON_LINK_FAILED, PNULL);
                    }
                }
                break;
                
            default:
                break;			            
            }   
            
        }
        break; 
    case MSG_WEATHER_PARSE_START_IND:
        {
            //SCI_TRACE_LOW:"[weather]---MSG_WEATHER_PARSE_START_IND---"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1851_112_2_18_3_6_35_30,(uint8*)"");
#ifdef WEATHER_SUPPORT_GOOGLE
            MMIWEATHER_ParseStart();
#elif defined(MMI_WEATHER_JSON_PARSE)
            MMIWEATHER_JsonParseStart(JsonParse_CallBack);
#endif
        }
        break;         
    case MSG_WEATHER_PARSE_SUCCESS_CNF:
        {
            //SCI_TRACE_LOW:"[weather]---MSG_WEATHER_PARSE_SUCCESS_CNF---"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1857_112_2_18_3_6_35_31,(uint8*)"");
            MMIWEATHER_SetUpdateState(FALSE);
            MMIWEATHER_WriteNV(MMIWEATHER_NV_WEATHER_INFO);
            MMK_CloseWin(MMIWEATHER_PUB_WIN_ID);
            switch(update_type)		   	
            {
            case MMIWEATHER_UPDATE_FROM_WIDGET:
                {
#if defined MMI_WIDGET_WEATHER1 
                    MMK_PostMsg(g_weather_widget.win_id, MSG_WEATHER_WGT_UPDATE_CNF, PNULL, 0); 
#endif
#if defined MMI_WIDGET_WEATHER2
                    MMK_PostMsg(g_weather_widget2.win_id, MSG_WEATHER_WGT_UPDATE_CNF, PNULL, 0);
#endif
                }
                break;			            
            case MMIWEATHER_UPDATE_FROM_MAIN_WIN:
                {
                    MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_SUCCESS, TXT_WEATHER_UPDATE_OK, PNULL);
                    //MMIPUB_DisplayTipsIDEx(PNULL, TXT_WEATHER_UPDATE_OK, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
                }
                break;
            case MMIWEATHER_UPDATE_FROM_AUTO:
                {
                    MMI_WIN_ID_T win_id = MMK_GetFocusWinId();
//#if  (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
 
//                    if (g_weather_widget.win_id == win_id)
//#ifdef MMI_WIDGET_WEATHER2
//                    if (g_weather_widget2.win_id == win_id)
//#endif
                    {
                        MMK_PostMsg(win_id, MSG_WEATHER_WGT_UPDATE_CNF, PNULL, 0); 
                    }                    
//#endif                   
                    if (MMIWEATHER_MAIN_WIN_ID == win_id)
                    {
                        MMIWEATHER_UpdateWinAllPage();
                    }
                }
                break;	

            case MMIWEATHER_UPDATE_FROM_ADD_CITY:
                {
                    if(MMIWEATHER_IsSupprotCity())
                    {
                        MMIWEATHER_ProcAddCityStatus(MMIWEATHER_ADD_CITY_STATE_OK);
                    }
                    else
                    {
                        MMIWEATHER_ProcAddCityStatus(MMIWEATHER_ADD_CITY_STATE_UNSUPPORT);
                    }
                }
                break;
                
            default:
                break;			            
            }   
        }
        break; 

#ifdef MMI_WEATHER_JSON_PARSE
    case MSG_WEATHER_ERROR_DESCRIPTION_CNF:
        {
            MMIWEATHER_ERROR_INFO_T *weather_error_ptr = MMIWEATHER_GetErrorInfo();
            MMI_STRING_T   text_info = {0};
            MMI_WIN_ID_T   win_id = MMIWEATHER_PUB_WIN_ID;
            BOOLEAN    deactive_res = FALSE;
            
            MMK_CloseWin(win_id);            
            if(0 != weather_error_ptr->error_des_len)
            {
                text_info.wstr_len = weather_error_ptr->error_des_len;
                text_info.wstr_ptr = weather_error_ptr->error_description;

                MMIPUB_OpenAlertTextWinByTextPtr(PNULL, 
                                                &text_info, 
                                                &win_id,
                                                PNULL, 
                                                MMIPUB_SOFTKEY_NONE, 
                                                PNULL);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
            MMIWEATHER_SetUpdateState(FALSE);
            deactive_res = MMIWEATHER_PdpDeactive();
            SCI_TRACE_LOW("weather error descrip deactive_res = %d",deactive_res);
        }
        break;

    case MSG_WEATHER_RETRY_EN_NAME_CNF:
        //convert city name to english name
		{
			uint8   city_num = 0;
			MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr = PNULL;
			HTTP_CONTEXT_ID_T   context_id = 0;

			city_num = MMIWEATHER_GetCityNum();                   
			if(city_num < MMIWEATHER_MAX_CITY_NUM)
			{
				MMIWEATHER_WEATHER_T* weather_info_ptr = MMIWEATHER_GetCityWeatherInfo(city_num);

				WeatherGetCityLetters(weather_info_ptr->city_name,weather_info_ptr->city_code);

				http_context_ptr = GetHttpContextPtr();

				if(PNULL != http_context_ptr)
				{
					context_id   = http_context_ptr->context_id;
					HTTP_CloseRequest(context_id);
				}
				if(PNULL != s_http_content_weather_buffer_address)
				{
					SCI_MEMSET(s_http_content_weather_buffer_address,0,HTTP_CONTENT_BUFFER_SIZE);
				}
				StartHttpRequest();
                SCI_TRACE_LOW("[weather]:retry en name again");
                s_is_retry_en_name = TRUE;
			}
		}
        break;


    case MSG_WEATHER_NEED_APPEND_COUNTRY_LIST_CNF:
        {
            MMI_WIN_ID_T   win_id = MMIWEATHER_PUB_WIN_ID;
            BOOLEAN    deactive_res = FALSE;

            MMK_CloseWin(win_id);
            deactive_res = MMIWEATHER_PdpDeactive();  
            MMIWEATHER_SetUpdateState(FALSE);
            MMIWEATHER_OpenCountryList();
            SCI_TRACE_LOW("weather open country_win deactive_res = %d",deactive_res);
        }
        break;

    case MSG_WEATHER_ONLY_COUNTRY_INFO:
        MMK_CloseWin(MMIWEATHER_PUB_WIN_ID);
        MMIPUB_OpenAlertFailWin(TXT_WEATHER_NO_WEATHER_INFO);
        MMIWEATHER_PdpDeactive();  
        MMIWEATHER_SetSelCountryFlag(FALSE);
        MMIWEATHER_SetUpdateState(FALSE);
        break;

    case MSG_WEATHER_TOO_MANY_COUNTRY:
        MMK_CloseWin(MMIWEATHER_PUB_WIN_ID);
        MMIPUB_OpenAlertFailWin(TXT_WEATHER_TOO_MANY_COUNTRY);
        MMIWEATHER_FreeCountryInfoMem();
        MMIWEATHER_PdpDeactive();  
        MMIWEATHER_SetUpdateState(FALSE);
        break;
#endif

    case MSG_WEATHER_WGT_SWITCH_CITY_IND:
        {
            uint8 index = MMIWEATHER_GetCurCityIndex();
            uint8 city_num = MMIWEATHER_GetCityNum();
            //SCI_TRACE_LOW:"[weather]---MSG_WEATHER_TASK_DELETE---"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1921_112_2_18_3_6_35_32,(uint8*)"");
			MMIWEATHER_SetCurCityIndex((index + 1) == city_num ? 0 : (index + 1));
#if defined MMI_WIDGET_WEATHER1
			MMK_PostMsg(g_weather_widget.win_id, MSG_WEATHER_WGT_SWITCH_CITY_CNF, PNULL, 0);  
#endif
#if defined MMI_WIDGET_WEATHER2
            MMK_PostMsg(g_weather_widget2.win_id, MSG_WEATHER_WGT_SWITCH_CITY_CNF, PNULL, 0);
#endif
        }
        break;
        
    case MSG_WEATHER_WGT_UPDATE_IND:
        {
            MMIWEATHER_UpdateWeatherInfo(MMIWEATHER_UPDATE_FROM_WIDGET);
        }
        break;
        
    case MSG_WEATHER_UPDATE_CANCEL_IND:  //取消更新开始
        {
          //  if(MMIWEATHER_UPDATE_FROM_MAIN_WIN == update_type)
            {
                StopPdpActiveTimer();
                MMK_CloseWin(MMIWEATHER_PUB_WIN_ID);
                MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_WAITING, TXT_WEATHER_CANCELING, HandleCancelUpdateWaitingWinMsg);
                CancelHttpRequest();
            }
        }
        break;
        
    case MSG_WEATHER_UPDATE_CANCEL_CNF: //取消更新成功
        {
            MMIWEATHER_SetUpdateState(FALSE);
            MMK_CloseWin(MMIWEATHER_PUB_WIN_ID);
        }
        break;
        
    case MSG_WEATHER_HTTP_CLOSE_CNF:
        {
            if(NeedDeactedPDP())
            {
                MMIWEATHER_PdpDeactive();
            }
        }
        break;
        
    case MSG_WEATHER_AUTO_UPDATE_IND:
        {
            MMIWEATHER_UpdateWeatherInfo(MMIWEATHER_UPDATE_FROM_AUTO);
            SCI_TRACE_LOW("weather:auto update!!");
        }
        break;

    case MSG_WEATHER_ADD_CITY_UPDATE_IND:
        {
            if(MMIWEATHER_UpdateWeatherInfo(MMIWEATHER_UPDATE_FROM_ADD_CITY))
            {
                //提示正在添加城市，请稍候。
                MMIWEATHER_ProcAddCityStatus(MMIWEATHER_ADD_CITY_STATE_ADDING);
            }
        }
        break;
    
    default:
        {
            //SCI_TRACE_LOW:"[weather]---default---"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_1981_112_2_18_3_6_36_33,(uint8*)"");
        }
        break;
        
    }
    
    return TRUE;
}
/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIWEATHER_HTTP_CONTEXT_T *http_context_ptr = PNULL;
    void    *original_param = (void*)((uint32)param);
    

    http_context_ptr = GetHttpContextPtr();
    SCI_TRACE_LOW("[weather]:HandleWeatherHttpMsg begin msg_id = %x",msg_id);

    switch (msg_id)
    {
    case HTTP_SIG_INIT_CNF:
        {
            //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_INIT_CNF"
            HTTP_INIT_CNF_SIG_T * cnf = (HTTP_INIT_CNF_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2007_112_2_18_3_6_36_34,(uint8*)"");            

            SCI_TRACE_LOW("[weather]:HTTP_SIG_INIT_CNF app_param = %d,cnf->module_id = %d",cnf->app_param,cnf->module_id);
            if(cnf->module_id != MMI_MODULE_WEATHER || 1 != cnf->app_param)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpInitCnf(http_context_ptr, param);
        }
        break;

#ifdef WEATHER_INFORMATION_BUFFER_STORE_SUPPORT
    case HTTP_SIG_DATA_IND:
        {
            HTTP_DATA_IND_SIG_T *cnf = (HTTP_DATA_IND_SIG_T*)original_param;
            //SCI_TRACE_LOW:"HandleHttpWeatherDataInd receive HTTP_SIG_DATA_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2013_112_2_18_3_6_36_35,(uint8*)"");
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpWeatherDataInd(http_context_ptr, param);
        }
        break;
#endif
    case HTTP_SIG_NEED_AUTH_IND:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND"
        {
            HTTP_AUTH_IND_SIG_T *cnf = (HTTP_AUTH_IND_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2018_112_2_18_3_6_36_36,(uint8*)"");
            
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpNeedAuthInd(http_context_ptr, param);
            }
        break;

    case HTTP_SIG_REQUEST_ID_IND:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND"
        {
            HTTP_REQUEST_ID_IND_SIG_T *cnf = (HTTP_REQUEST_ID_IND_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2023_112_2_18_3_6_36_37,(uint8*)"");
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpRequestIdInd(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_GET_CNF:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_GET_CNF"
        {
            HTTP_GET_CNF_SIG_T *cnf = (HTTP_GET_CNF_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2028_112_2_18_3_6_36_38,(uint8*)"");
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpGetCnf(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_HEADER_IND:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_HEADER_IND"
        {
            HTTP_HEADER_IND_SIG_T *cnf = (HTTP_HEADER_IND_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2033_112_2_18_3_6_36_39,(uint8*)"");
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpHeaderInd(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_ERROR_IND:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_ERROR_IND"
        {
            HTTP_ERROR_IND_SIG_T *cnf = (HTTP_ERROR_IND_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2038_112_2_18_3_6_36_40,(uint8*)"");
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpErrorInd(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_CLOSE_CNF:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_CLOSE_CNF"
        {
            HTTP_CLOSE_CNF_SIG_T *cnf = (HTTP_CLOSE_CNF_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2043_112_2_18_3_6_36_41,(uint8*)"");
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpCloseCnf(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_CANCEL_CNF:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_CANCEL_CNF"
        {
            HTTP_CANCEL_CNF_SIG_T *cnf = (HTTP_CANCEL_CNF_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2048_112_2_18_3_6_36_42,(uint8*)"");
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpCancelInd(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_NET_PROG_IND:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_NET_PROG_IND"
        {
            HTTP_NET_PROG_IND_SIG_T *cnf = (HTTP_NET_PROG_IND_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2053_112_2_18_3_6_36_43,(uint8*)"");
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpNetProgInd(http_context_ptr, param);
        }
        break;

    case HTTP_SIG_REDIRECT_IND:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg receive HTTP_SIG_REDIRECT_IND"
        {
            HTTP_REDIRECT_IND_SIG_T *cnf = (HTTP_REDIRECT_IND_SIG_T*)original_param;
            
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2058_112_2_18_3_6_36_44,(uint8*)"");
            if(cnf->context_id != http_context_ptr->context_id)
            {
                return MMI_RESULT_FALSE;
            }
            result = HandleHttpRedirectInd(http_context_ptr, param);
        }
        break;

    default:
        //SCI_TRACE_LOW:"[weather]:HandleHttpMsg Not handle msg_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2063_112_2_18_3_6_36_45,(uint8*)"d", msg_id);
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}
/*****************************************************************************/
//  Description : auto update weather by timer
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandleAutoUpdateTimer( 
                               uint8  timer_id,
                               uint32 param
                               )
{
    if(s_auto_update_timer == timer_id)
    {
        HandleWeatherAppMsg(PNULL, MSG_WEATHER_AUTO_UPDATE_IND, PNULL);  
    }
}

/*****************************************************************************/
//  Description : handle weather msg
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWeatherMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 time_out = 0;

    SCI_TRACE_LOW("[weather]:HandleWeatherMsg msg_id = %d",msg_id);

    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
        result = HandleWeatherHttpMsg(app_ptr, msg_id, param);
    }
    else if (MSG_WEATHER_START <= msg_id && msg_id <= MSG_WEATHER_END)
    {
        result = HandleWeatherAppMsg(app_ptr, msg_id, param);
    }
    else if(MISC_MSG_RTC_SEC <= msg_id && msg_id <= MISC_MSG_RTC_DAY)
    {
        if(MMIWEATHER_IsTimeToUpdate())
        {
            time_out = rand()%60;
            SCI_TRACE_LOW("HandleWeatherMsg time_out = %d",time_out);
            if(0 != s_auto_update_timer)
            {
                MMK_StopTimer(s_auto_update_timer);
                s_auto_update_timer = 0;
            }
            s_auto_update_timer = MMK_CreateTimerCallback(time_out*1000, HandleAutoUpdateTimer, PNULL, FALSE);
        }
        result = MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return result;
}


/*****************************************************************************/
//  Description : MMIWEATHER_SetUpdateTimeTable
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetUpdateTimeTable(BOOLEAN auto_update)
{
    uint8  start = 0;
    uint8  end   = 0;
    uint32 i     = 0;
    uint32 table_idx = 0;
	uint32 freq  = MMIWEATHER_DEFAULT_UPDATE_FREQ;


    for(i = 0; i < 24; i++)
    {
        s_time_table[i].need_update = FALSE;
    }

    if(auto_update) //开启定时更新
    {
        start = MMIWEATHER_GetUpdateRangeStart();
        end   = MMIWEATHER_GetUpdateRangeEnd();
        freq  = MMIWEATHER_GetUpdateFrequency();
        for(i = start; i <= end; i += freq)
        {
            if(0 == i)
            {
                table_idx = 23;
            }
            else
            {
                table_idx = i - 1;
            }
            s_time_table[table_idx].need_update = TRUE;
            s_time_table[table_idx].min = rand()%50+10;
            if(s_time_table[table_idx].min >= 60)
            {
                s_time_table[table_idx].min = 0;
            }
            SCI_TRACE_LOW("MMIWEATHER_SetUpdateTimeTable table_idx = %d,min = %d",table_idx,s_time_table[table_idx].min);
        }
    }     
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_SetUpdateTimeTable---start=%d, end=%d, freq=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2126_112_2_18_3_6_36_46,(uint8*)"ddd", start, end, freq);

    
}

/*****************************************************************************/
//  Description : MMIWEATHER_IsTimeToUpdate
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_IsTimeToUpdate(void)
{
    BOOLEAN     result = FALSE;
    SCI_TIME_T  sys_time = {0};    

    TM_GetSysTime(&sys_time);
    
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_IsTimeToUpdate---%02d:%02d:%02d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2142_112_2_18_3_6_36_47,(uint8*)"ddd", sys_time.hour, sys_time.min, sys_time.sec);
    if(s_time_table[sys_time.hour].need_update)
    {
        if(s_time_table[sys_time.hour].min == sys_time.min)
        {
            result = TRUE;
            SCI_TRACE_LOW("[weather]:MMIWEATHER_IsTimeToUpdate TRUE");
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : Get current temp ramge of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIWEATHER_GetCurTempRangeWstr(uint8 city_index)
{
    MMI_STRING_T        unit_str  = {0};
    int32               temp_low  = 0;
    int32               temp_high = 0;
    WEATHER_TEMP_UNIT_E temp_unit = 0;
    uint8               temp_range_str[MMIWEATHER_TEMP_RANGE_STR_LEN + 1] = {0};
    MMIWEATHER_SETTING_T* setting_ptr  = MMIWEATHER_GetSettingInfo();
    MMIWEATHER_WEATHER_T* weather_info = MMIWEATHER_GetCurCityWeatherInfo();
    uint16 wstr_len = 0;
    char   cur_date_str[12] = {0};
    wchar  cur_date_wstr[6] = {0};
    SCI_DATE_T cur_date = {0};
    MMIWEATHER_FC_DAY_E cur_day = FC_DAY_MAX;
    //uint8 city_index = MMIWEATHER_GetCurCityIndex();
    
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_GetCurTempRangeStr---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2171_112_2_18_3_6_36_48,(uint8*)"");
    if(PNULL == weather_info || PNULL == setting_ptr)
    {
        //SCI_TRACE_LOW:"[weather]:MMIWEATHER_GetCurTempRangeStr--- error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2174_112_2_18_3_6_36_49,(uint8*)"");
        return PNULL;
    }
	if(city_index  >= MMIWEATHER_MAX_CITY_NUM)
    {
        //SCI_TRACE_LOW:"[WEATHER]MMIWEATHER_GetCurTempRangeWstr city_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2179_112_2_18_3_6_36_50,(uint8*)"d",city_index);
        return PNULL;
    }

    //clean old data
    SCI_MEMSET(weather_info->temp_range_wstr, 0, sizeof(wchar)*(MMIWEATHER_TEMP_RANGE_STR_LEN + 1));
    
    TM_GetSysDate(&cur_date);
    sprintf(cur_date_str, "%d/%d", cur_date.mon, cur_date.mday);
    MMIAPICOM_StrToWstr((uint8*)cur_date_str, cur_date_wstr);
    wstr_len = MMIAPICOM_Wstrlen(cur_date_wstr);
    
    if(0 == MMIAPICOM_Wstrncmp(cur_date_wstr, 
                                g_weather_data.weather_info[city_index].forecast.date1, 
                                wstr_len))
    {
        cur_day = FC_DAY_1;
    }
    else if(0 == MMIAPICOM_Wstrncmp(cur_date_wstr, 
                                g_weather_data.weather_info[city_index].forecast.date2, 
                                wstr_len))
    {
        cur_day = FC_DAY_2;
    }
    else if(0 == MMIAPICOM_Wstrncmp(cur_date_wstr, 
                                g_weather_data.weather_info[city_index].forecast.date3, 
                                wstr_len))
    {
        cur_day = FC_DAY_3;
    }
    else if(0 == MMIAPICOM_Wstrncmp(cur_date_wstr, 
                                g_weather_data.weather_info[city_index].forecast.date4, 
                                wstr_len))
    {
        cur_day = FC_DAY_4;
    }

    if( setting_ptr->city_num > 0 
        && MMIWEATHER_IsCurCityWeatherInfoLegal() 
        && cur_day < FC_DAY_MAX)
    {
        temp_low  = MMIWEATHER_GetLowTempByDayAndIndex(cur_day,city_index);
        temp_high = MMIWEATHER_GetHighTempByDayAndIndex(cur_day,city_index);
        temp_unit = MMIWEATHER_GetTempUnitType();

        if(temp_low <= temp_high)
        {
            if(WEATHER_TEMP_UNIT_C == temp_unit)
            {
                temp_low  = MMIWEATHER_GetTemperature(TEMP_F2C(temp_low));
                temp_high = MMIWEATHER_GetTemperature(TEMP_F2C(temp_high));
                
                sprintf((char*)temp_range_str, "%d/%d", temp_low, temp_high);
                MMIAPICOM_StrToWstr(temp_range_str, weather_info->temp_range_wstr);
                MMI_GetLabelTextByLang(TXT_WEATHER_CEL_SIGNAL, &unit_str);
                MMIAPICOM_Wstrncat(weather_info->temp_range_wstr, unit_str.wstr_ptr,unit_str.wstr_len);
            }
            else if(WEATHER_TEMP_UNIT_F == temp_unit)
            {
                sprintf((char*)temp_range_str, "%d/%d", temp_low, temp_high);
                MMIAPICOM_StrToWstr(temp_range_str, weather_info->temp_range_wstr);
                MMI_GetLabelTextByLang(TXT_WEATHER_FAH_SIGNAL, &unit_str);
                MMIAPICOM_Wstrncat(weather_info->temp_range_wstr, unit_str.wstr_ptr,unit_str.wstr_len);
            }
        }

    }

    return weather_info->temp_range_wstr;
    
}


/*****************************************************************************/
//  Description : 初始化互联网账户设置。如果有预设值，则初始化为预设值
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_InitNetworkAccount(void)
{
    uint32  dual_sys    = 0;
    BOOLEAN is_changed  = FALSE;
    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_InitNetworkAccount-----"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2260_112_2_18_3_6_36_51,(uint8*)"");
    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        char*   apn_str     = PNULL;
        uint32  index       = 0;
        if(MMIAPIPHONE_IsSimCardOk(dual_sys))
        {
            PHONE_PLMN_NAME_E   hplmn_name  = PLMN_TEST;
            MMIAPIPHONE_GetFullPlmn(dual_sys, PNULL, &hplmn_name);
            for(index = 0; index < ARR_SIZE(s_apn_table); index++)
            {
                if(hplmn_name == s_apn_table[index].hplmn)
                {
                    apn_str = s_apn_table[index].apn;
                    //SCI_TRACE_LOW:"[weather]:MMIWEATHER_InitNetworkAccount----sim%d apn = %s"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2274_112_2_18_3_6_36_52,(uint8*)"ds", dual_sys + 1, apn_str);
                    break;
                }
            }

            if(PNULL != apn_str)
            {
                uint8   linkset_num = MMIAPICONNECTION_GetLinkSettingNum(dual_sys);
                for(index = 0; index < linkset_num; index++)
                {
                    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys, index);
                    if(PNULL != linksetting_ptr && 0 == strcmp(apn_str, (char*)linksetting_ptr->apn))
                    {
                        //SCI_TRACE_LOW:"[weather]:MMIWEATHER_InitNetworkAccount----sim%d  index = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2287_112_2_18_3_6_36_53,(uint8*)"dd", dual_sys + 1, index);
                        g_weather_data.setting_info.account_index[dual_sys] = index;
                        is_changed = TRUE;
                        break;
                    }
                }
            }
        }
    }

    if(is_changed)
    {
        MMIWEATHER_WriteNV(MMIWEATHER_NV_SETTING_INFO);
    }
    
}

/*****************************************************************************/
//  Description : Get the index of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIWEATHER_GetCurCity(void)
{
	return g_weather_data.setting_info.cur_city;
}

/*****************************************************************************/
//  Description : Set the index of current city.
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetCurCity(uint8 index)
{
#if  (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
	g_weather_data.setting_info.cur_city = index;
#endif	
    MMIWEATHER_WriteNV(MMIWEATHER_NV_SETTING_INFO);
}

/*****************************************************************************/
//  Description : 判断是否曾经添加过该城市
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_IsAddedCity(MMI_STRING_T* city_name_ptr)
{   
    BOOLEAN                is_existed       = FALSE;
    MMIWEATHER_WEATHER_T * city_info_ptr    = PNULL;
    uint8                  city_num         = MMIWEATHER_GetCityNum();
    uint8                  i                = 0;
    
    for(i = 0; i < city_num; i++)
    {
        city_info_ptr = MMIWEATHER_GetCityWeatherInfo(i);       

        if(0 == MMIAPICOM_Wstrcmp(city_name_ptr->wstr_ptr, city_info_ptr->city_name))
        {
            is_existed = TRUE;
            break;
        }
    }
    //SCI_TRACE_LOW:"MMIWEATHER_IsAddedCity is_existed = %d,city_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2346_112_2_18_3_6_36_54,(uint8*)"dd",is_existed,city_num);
    return is_existed;
}
/*****************************************************************************/
//  Description : MMIWEATHER_SetSupprotCity
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_SetCitySupprot(BOOLEAN is_supprot)
{
    s_is_support_city = is_supprot;
}


/*****************************************************************************/
//  Description : MMIWEATHER_IsSupprotCity
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_IsSupprotCity(void)
{
    return s_is_support_city;
}

/*****************************************************************************/
// 	Description : 获得天气信息
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
PUBLIC char* MMIWEATHER_GetWeatherInformation(void)
{
#ifdef WIN32
    #ifdef WEATHER_SUPPORT_GOOGLE
        return s_test_weather_data_ptr;
    #else
        WIN32Getweatherinfo();
        return s_test_buffer_ptr;
    #endif
#else
    return s_http_content_weather_buffer_address;
#endif
}

/*****************************************************************************/
// 	Description : 天气信息BUFFER 分配
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_WeatherInfoAlloc(void)
{
    if(PNULL == s_http_content_weather_buffer_address)
    {
        //SCI_TRACE_LOW:"MMIWEATHER_WeatherInfoAlloc,because PNULL == s_http_content_weather_buffer_address, then alloc"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2389_112_2_18_3_6_36_55,(uint8*)"");
        s_http_content_weather_buffer_address = SCI_ALLOC_APPZ(HTTP_CONTENT_BUFFER_SIZE);
    }

    if(PNULL != s_http_content_weather_buffer_address)
    {
        SCI_MEMSET(s_http_content_weather_buffer_address, 0, HTTP_CONTENT_BUFFER_SIZE);     
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : 天气信息BUFFER 释放
//	Global resource dependence : 
//  Author: Glen Li
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_WeatherInfoRelease(void)
{
     if(PNULL != s_http_content_weather_buffer_address)
    {
        SCI_FREE(s_http_content_weather_buffer_address);
        s_http_content_weather_buffer_address = PNULL;
    }
    SCI_TRACE_LOW("weather release buffer!");
}
/*****************************************************************************/
// 	Description : MMIWEATHER_GetTemperature
//	Global resource dependence : 
//  Author: juan.wu
//	Note:对天气温度四舍五入
/*****************************************************************************/
PUBLIC int32 MMIWEATHER_GetTemperature(int32 temp)
{
    int32  temp_g = 0;//个位
    int32  temp_s = 0;//十位
    
    if(0 == temp)
    {
        return temp;
    }
    
    temp_g = temp%10;
    temp_s = temp/10;
    if(ABS(temp_g) > 5)
    {
        if(temp > 0)
        {
            temp_s += 1;
        }
        else
        {
            temp_s -= 1;
        }
    }
    
    return temp_s;
}

/*****************************************************************************/
//  Description : 如果为表中内置城市名，返回转换后的城市名，否则返回空。
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
PUBLIC const char* MMIWEATHER_GetSpecialCityCode(wchar* city_name)
{
    const char* city_code = PNULL;
    uint32 i = 0;
    MMIWEATHER_CITY_TAB_T * special_city_table_ptr = PNULL;
    uint32 special_city_num = 0;

    if(MMIAPICOM_IsASCIIString(city_name,MMIAPICOM_Wstrlen(city_name)))
    {
        MMIAPICOM_Wstrlower(city_name);
    }
    special_city_table_ptr = MMIWEATHER_GetSpecialCityTable();
    special_city_num = MMIWEATHER_GetSpecialCityNum();
    
    for (i = 0; i < special_city_num; i++)
    {
        if(0 == MMIAPICOM_Wstrncmp(city_name, 
                    special_city_table_ptr[i].city_name,
                    MMIAPICOM_Wstrlen(city_name)))
        {
            city_code = special_city_table_ptr[i].city_code;
            break;
        }
    }

    //SCI_TRACE_LOW:"[weather]: city_code = %s ---"
    if(NULL != city_code)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWEATHER_2463_112_2_18_3_6_37_56,(uint8*)"s",city_code);
    }
    
    return city_code;
    
}
/*****************************************************************************/
//  Description : pdp retry if it can't active
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WeatherPDPTry(MN_DUAL_SYS_E sim_sys)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    BOOLEAN                 return_val      = FALSE;
    
    active_info.app_handler         = MMI_MODULE_WEATHER;
    active_info.dual_sys            = sim_sys;
    active_info.apn_ptr             = GetApnStr(sim_sys);
    active_info.user_name_ptr       = GetUserNameStr(sim_sys);
    active_info.psw_ptr             = GetPasswordStr(sim_sys);
    active_info.priority            = 3;
    active_info.ps_service_rat      = MN_UNSPECIFIED;
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = HandlePdpMsg;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;

    if(MMIAPIPDP_Active(&active_info))
    {
        return_val = TRUE;
    }
    SCI_TRACE_LOW("[weather]:WeatherPDPTry return_val = %d",return_val);
    return return_val;
}
/*****************************************************************************/
//  Description :HandlePdpRetryTimer
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void HandlePdpRetryTimer(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MN_DUAL_SYS_E sys_sim = 0;
    BOOLEAN       sim_ok =  FALSE;

    SCI_TRACE_LOW("[WEATHER]:HandlePdpRetryTimer begin");
    if(timer_id == s_pdp_retry_timer_id && 0 != s_pdp_retry_timer_id)
    {
        MMK_StopTimer(s_pdp_retry_timer_id);
        s_pdp_retry_timer_id = 0;
    }
    cur_sys_num++;
    if(cur_sys_num < MMI_DUAL_SYS_MAX)
    {
        for(sys_sim = cur_sys_num; sys_sim < MMI_DUAL_SYS_MAX; sys_sim++)
        {
            if(MMIPHONE_IsSimOk(sys_sim))
            {
                cur_sys_num = sys_sim;
                sim_ok = TRUE;
                break;
            }
        }
        if(sim_ok)
        {
            if(!WeatherPDPTry(cur_sys_num))
            {
        	    s_pdp_retry_timer_id = MMK_CreateTimerCallback(
                                                            PDP_RETRY_TIMEOUT, 
                                            		        HandlePdpRetryTimer, 
                                            		        PNULL, 
                                                		    FALSE
                                                		    );
            }
        }
        else
        {
            MMIAPIPDP_Deactive(MMI_MODULE_WEATHER);
            MMIWEATHER_SendMsgToApp(MSG_WEATHER_PDP_ACTIVE_FIILED_CNF);
        }
    }
    else//找不到卡了
    {
        MMIAPIPDP_Deactive(MMI_MODULE_WEATHER);
        MMIWEATHER_SendMsgToApp(MSG_WEATHER_PDP_ACTIVE_FIILED_CNF);
    }
}
#ifdef MMIWEATHER_SUPPORT_SPRD_SRV
/*****************************************************************************/
//  Description :write url index in nv
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void WriteWeatherURLIndex(uint8 idx)
{
    SCI_TRACE_LOW("[weather]:WriteWeatherURLIndex idx = %d",idx);
    MMINV_WRITE(MMIWEAHTER_URL_INDEX, &idx);
}
/*****************************************************************************/
//  Description :read url index from nv
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL uint8 ReadWeatherURLIndex(void)
{
    MN_RETURN_RESULT_E    ret_value = MN_RETURN_FAILURE;
    uint8                 weather_url_idx = 0;
    
    MMINV_READ(MMIWEAHTER_URL_INDEX, &weather_url_idx, ret_value);
    if(MN_RETURN_SUCCESS != ret_value)
    {
        weather_url_idx = 0;
        MMINV_WRITE(MMIWEAHTER_URL_INDEX, &weather_url_idx);
    }
    SCI_TRACE_LOW("[weather]:ReadWeatherURLIndex weather_url_idx = %d",weather_url_idx);
    return weather_url_idx;
}
#endif
PUBLIC void MMIAPIWEATHER_SetTryEnNameFlag(BOOLEAN is_try)
{
    s_is_retry_en_name = is_try;
}
PUBLIC BOOLEAN MMIAPIWEATHER_GetHasTryEnName(void)
{
    return s_is_retry_en_name;
}
/*****************************************************************************/
//  Description :Some name can't be add,but there has info from wunderground eg:taiwan
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWEATHER_IsCityCanBeAdd(MMI_STRING_T * city_name_ptr)
{
    wchar  *cannot_add_city[]= {
                                L"\x53f0\x6e7e",
                                L"taiwan",
                               };
    uint32  cmp_len = 0;
    uint32  i = 0;
    BOOLEAN is_can_be_add = TRUE;
    wchar *temp_name_ptr = PNULL;
    uint32 temp_name_len = 0;


    temp_name_ptr = SCI_ALLOC_APPZ(sizeof(wchar) * (city_name_ptr->wstr_len+1));
    if(PNULL == temp_name_ptr)
    {
        SCI_TRACE_LOW("MMIWEATHER_IsCityCanBeAdd PNULL == temp_name_ptr");
        return FALSE;
    }
    MMIAPICOM_Wstrncpy(temp_name_ptr, city_name_ptr->wstr_ptr, city_name_ptr->wstr_len);
    temp_name_len = city_name_ptr->wstr_len;
    if(MMIAPICOM_IsASCIIString(temp_name_ptr,temp_name_len))
    {
        MMIAPICOM_Wstrlower(temp_name_ptr);
    }
    
    for(i = 0; i < ARR_SIZE(cannot_add_city); i++)
    {
        cmp_len  = MMIAPICOM_Wstrlen(cannot_add_city[i]);
        cmp_len  = MAX(temp_name_len,cmp_len);
        if(0 == MMIAPICOM_Wstrncmp(temp_name_ptr, 
                                    cannot_add_city[i], 
                                    cmp_len))
        {
            is_can_be_add = FALSE;
            break;
        }
    }
    SCI_FREE(temp_name_ptr);
    temp_name_ptr = PNULL;
    return is_can_be_add;
    
}
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif



