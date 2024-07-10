/*****************************************************************************
** File Name:      mmidcd_xml_parse.h                                                  *
** Author:                                                                   *
** Date:           10/2007                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2007        byte.guo       Create                                  *
******************************************************************************/
#ifndef  _MMIDCD_XML_PARSE_H_    
#define  _MMIDCD_XML_PARSE_H_


#include "sci_types.h"
#include "sig_code.h"
#include "sfs.h"
#include "os_api.h"
#include "mmicom_data.h"
#ifdef MMI_ENABLE_DCD
//#include "mmidcd_http.h"
#include "expat_external.h"
#include "expat.h"

#ifndef WIN32
#include "http_api.h"
#endif

#ifdef   __cplusplus
extern   "C" 
{
#endif


//#define DCD_INTERPRETE_IMMEDIATELY

#define DCD_MAX_URL_NUM              4
#define DCD_MAX_HREF_LEN             512 //1024
#define DCD_MAX_EMAIL_LEN            64 
#define DCD_MAX_HREF_NUM             10
#define DCD_MAX_URL_LEN              32

/*Macro for GLOBAL META*/
#define DCD_MAX_GLOBALMETA_ANCHOR_LEN       100
#define DCD_MAX_META_TTL_LEN                15
#define DCD_MAX_META_TIME_LEN               20

/*Macro for GENERATOR*/
#define DCD_MAX_GENERATOR_VERSION_LEN       32
#define DCD_MAX_GENERATOR_TYPE_LEN          32

/*Macro for FEED META*/
#define DCD_MAX_FEEDMETA_ANCHOR_LEN         75
#define DCD_MAX_FEEDMETA_TARGET_LEN         35
#define DCD_MAX_FEEDMETA_CHARGE_LEN         1

/*Macro for TITLE*/
#define DCD_MAX_TITLE_LEN                   32
#define DCD_MAX_TITLE_VALUE_LEN             60

/*Macro for SUMMARY*/
#define DCD_MAX_SUMMARY_TYPE_LEN            32
#define DCD_MAX_SUMMARY_VALUE_LEN           250

/*Macro for LINK*/
#define DCD_MAX_LINK_REL_LEN                10
#define DCD_MAX_LINK_TYPE_LEN               32
/*Macro for TXT*/
#define DCD_MAX_TXT_VALUE_LEN               20

/*Macro for AUTHOR*/    
#define DCD_MAX_AUTHOR_NAME_LEN             32
#define DCD_MAX_AUTHOR_EMAIL_LEN            32

/*Macro for TIME*/
#define DCD_MAX_TIME_TYPE_LEN               32
#define DCD_MAX_TIME_VALUE_LEN              32
#define DCD_MAX_ISSUED_TIME_LEN             32

/*Macro for CONTENT*/
#define DCD_MAX_CONTENT_TYPE_LEN            32
#define DCD_MAX_CONTENT_MODE_LEN            10
#define DCD_MAX_CONTENT_NAME_LEN            20
#define DCD_MAX_CONTENT_AUTOPLAY_LEN        1

/*Macro for EXCERPT*/
#define DCD_MAX_EXCERPT_LEN                 10

/*Macro for FFED*/
#define DCD_MAX_FEED_VERSION_LEN            15
#define DCD_MAX_FEED_TEMPLATE_ID_LEN        20

/*TIME */
#define DCD_LOCAL_TIME_ZONE					8//time zone
#define DCD_100_YEARS						100
#define DCD_MICROSECOND_LEN					3

#define DCDXML_ELEMENT_SYNC_PKG      "sync-pkg"                  //<sync-pkg></sync-pkg>
        
#define DCDXML_ELEMENT_META          "meta"                      //<meta> </meta>

#define DCDXML_ELEMENT_GENERATOR		"generator"	//<generator></generator>
        
#define DCDXML_ELEMENT_GLOBAL_META   "global-meta"               //<global-meta>
#define DCDXML_ELEMENT_FEED_META     "feed-meta"                 //<feed-meta>
        
        
#define DCDXML_ELEMENT_FEED          "feed"                      //<feed></feed>
#define DCDXML_ELEMENT_TITLE         "title"                     //<title></title>
#define DCDXML_ELEMENT_SUMMARY       "summary"                   //<summary></summary>
#define DCDXML_ELEMENT_LINK          "link"                      //<link></link>
#define DCDXML_ELEMENT_CONTENT       "content"                   //<content></content>
#define DCDXML_ELEMENT_ENTRY         "entry"                     //<entry></entry>
#define DCDXML_ELEMENT_ISSUED        "issued"                    //<issued></issued>
#define DCDXML_ELEMENT_AUTHOR        "author"                    //<author></author>
#define DCDXML_ELEMENT_ACTIVATION    "activation"                //<activation></activation>
#define DCDXML_ELEMENT_EXPIRATION    "expiration"                //<expiration></expiration>
#define DCDXML_ELEMENT_NAME          "name"                      //<name></name>
#define DCDXML_ELEMENT_URL           "url"                       //<url><url/>
#define DCDXML_ELEMENT_EMAIL         "email"                     //<email></email>
/*invalid*/
#define  DCDXML_ELEMENT_INVALID_HTML      "html"   //<html></html>

#define  DCDXML_ATTRIBUTE_TYPE_TXT               "type"
#define  DCDXML_ATTRIBUTE_MODE_TXT               "mode"
#define  DCDXML_ATTRIBUTE_NAME_TXT               "name"
#define  DCDXML_ATTRIBUTE_CTXT_ID_TXT            "ctxt-id"
#define  DCDXML_ATTRIBUTE_ENTRY_TYPE_TXT         "entry-type"
#define  DCDXML_ATTRIBUTE_TEMPLATE_ID_TXT        "template-id"
#define  DCDXML_ATTRIBUTE_REL_TXT                "rel"
#define  DCDXML_ATTRIBUTE_HREF_TXT               "href"
#define  DCDXML_ATTRIBUTE_URL_TXT                "url"
#define  DCDXML_ATTRIBUTE_EMAIL_TXT              "email"
#define  DCDXML_ATTRIBUTE_VERSION_TXT            "version"
#define  DCDXML_ATTRIBUTE_ANCHOR_TXT             "anchor"
#define  DCDXML_ATTRIBUTE_TTL_TXT                "ttl"
#define  DCDXML_ATTRIBUTE_SERVICE_TIME_TXT       "server-time"
#define  DCDXML_ATTRIBUTE_TARGET_TXT             "target"
#define  DCDXML_ATTRIBUTE_VERSION_TXT            "version"
#define  DCDXML_ATTRIBUTE_TITLE_TXT              "title"
#define  DCDXML_ATTRIBUTE_CHARGE_TXT             "charge"
#define  DCDXML_ATTRIBUTE_FEEDTYPE_TXT           "feed-type"
#define  DCDXML_ATTRIBUTE_AUTOPLAY_TXT           "autoplay"
#define  DCDXML_ATTRIBUTE_ENTRY_TEMPLATE_ID_TXT  "entry-template-id"
#define  DCDXML_ATTRIBUTE_EXCERPT_TXT            "excerpt"
#define  DCDXML_ATTRIBUTE_EXCERPT_DEFAULT_TXT    "excerpt_dafault"
#define  DCDXML_ATTRIBUTE_TEMPLATE_HREF_TXT      "template-href"

#define  DCD_LINK_REL_ALTERNATE_TXT         "alternate"
#define  DCD_LINK_TYPE_APPLICAION_TXT       "application/xhtml+xml"
#define  DCD_LINK_TYPE_TEXT_TXT             "text"
#define  DCD_LINK_TYPE_PLAIN_TEXT_TXT       "text/plain"

#define  DCD_CONTENT_MODE_BINARY_TXT        "binary"
#define  DCD_CONTENT_TYPE_GIF_TXT           "image/gif"
#define  DCD_CONTENT_TYPE_JPEG_TXT          "image/jpeg"
#define  DCD_CONTENT_NAME_IMAGE_TXT         "image"
#define  DCD_TEMPLATE_ID_DEFAULT_TXT        "default"

#define  DCD_ENTRY_TYPE_CS_TXT              "cs"
#define  DCD_ENTRY_TYPE_CSTO_TXT            "csto"
#define  DCD_ENTRY_TYPE_ETTI_TXT            "etti"
#define  DCD_ENTRY_TYPE_ETTO_TXT            "etto"
#define  DCD_ENTRY_TYPE_ETIO_TXT            "etio"
#define  DCD_ENTRY_TYPE_ETMM_TXT            "etmm"


#define  DCD_FEED_TYPE_FTTI_TXT             "ftti"
#define  DCD_FEED_TYPE_FTTO_TXT             "ftto"
#define  DCD_FEED_TYPE_FTIO_TXT             "ftio"
#define  DCD_FEED_TYPE_FTMM_TXT             "ftmm"

#define  DCD_METAFEED_CHARGE_Y_TXT          "Y"
#define  DCD_METAFEED_CHARGE_N_TXT          "N"
#define  DCD_METAFEED_CHARGE_P_TXT          "P"

#define  DCD_CONTENT_AUTOPLAY_Y_TXT         "Y"
#define  DCD_CONTENT_AUTOPLAY_N_TXT         "N"

#define  DCD_FILE_TYPE_GIF_SUFFIX_TXT       "gif"
#define  DCD_FIEL_TYPE_JPEG_SUFFIX_TXT      "jpg"
#define  DCD_FIEL_TYPE_BMP_SUFFIX_TXT       "bmp"

#define  DCD_FIEL_TYPE_IMAGE_TXT            "image"

#define  DCD_TIME_TYPE_REL_TXT              "rel"
#define  DCD_TIME_TYPE_ABSOLUTE_TXT         "absolute"

typedef enum
{
    DCDXML_ATTRIBUTE_ID_UNKNOWN,
    DCDXML_ATTRIBUTE_ID_TYPE,
    DCDXML_ATTRIBUTE_ID_MODE,
    DCDXML_ATTRIBUTE_ID_NAME,
    DCDXML_ATTRIBUTE_ID_CTXT_ID,
    DCDXML_ATTRIBUTE_ID_ENTRY_TYPE,
    DCDXML_ATTRIBUTE_ID_TEMPLATE_ID,
    DCDXML_ATTRIBUTE_ID_REL,
    DCDXML_ATTRIBUTE_ID_HREF,
    DCDXML_ATTRIBUTE_ID_URL,
    DCDXML_ATTRIBUTE_ID_EMAIL,   
    DCDXML_ATTRIBUTE_ID_VERSION,
    DCDXML_ATTRIBUTE_ID_ANCHOR,
    DCDXML_ATTRIBUTE_ID_TTL,
    DCDXML_ATTRIBUTE_ID_SERVICE_TIME,
    DCDXML_ATTRIBUTE_ID_TARGET,
    DCDXML_ATTRIBUTE_ID_CHARGE,
    DCDXML_ATTRIBUTE_ID_FEED_TYPE,
    DCDXML_ATTRIBUTE_ID_TITLE,
    DCDXML_ATTRIBUTE_ID_AUTOPLAY,
    DCDXML_ATTRIBUTE_ID_ENTRY_TEMPLATE_ID,   
    DCDXML_ATTRIBUTE_ID_EXCERPT,
    DCDXML_ATTRIBUTE_ID_EXCERPT_DEFAULT,
    DCDXML_ATTRIBUTE_ID_TEMPLATE_HREF,
    DCDXML_ATTRIBUTE_ID_MAX
}MMIDCDXML_ATTRIBUTE_ID_E;

/*for LINK type*/
typedef enum
{
    DCD_LINK_TYPE_UNKNOWN,
    DCD_LINK_TYPE_APPLICAION,
    DCD_LINK_TYPE_TEXT,
    DCD_LINK_TYPE_MAX
}DCD_LINK_TYPE_E;//please check

/*for LINK  rel*/
typedef enum
{
    DCD_LINK_REL_UNKNOWN,
    DCD_LINK_REL_ALTERNATE,
    DCD_LINK_REL_MAX
}DCD_LINK_REL_E;//please check

/*for CHARGE  type*/
typedef enum
{
    DCD_CHARGE_TYPE_Y,
    DCD_CHARGE_TYPE_N,
    DCD_CHARGE_TYPE_P,
    DCD_CHARGE_TYPE_MAX
}DCD_CHARGE_TYPE_E;//please check

/*for CONTENT  type*/
typedef enum
{
    DCD_CONTENT_CLASS_UNKNOWN,	
    DCD_CONTENT_CLASS_IMAGE,
    DCD_CONTENT_CLASS_MAX             
}DCD_CONTENT_CLASS_E;//please check

/*for CONTENT  type*/
typedef enum
{
    DCD_CONTENT_TYPE_UNKNOWN,	
    DCD_CONTENT_TYPE_GIF,
    DCD_CONTENT_TYPE_JPEG,
    DCD_CONTENT_TYPE_BMP,       
    DCD_CONTENT_TYPE_MAX
}DCD_CONTENT_TYPE_E;//please check

/*for CONTENT  mode*/
typedef enum
{
    DCD_CONTENT_MODE_UNKNOWN,
    DCD_CONTENT_MODE_BINARY,
    DCD_CONTENT_MODE_MAX
}DCD_CONTENT_MODE_E;//please check

/*for CONTENT  name*/
typedef enum
{
    DCD_CONTENT_NAME_UNKNOWN,
    DCD_CONTENT_NAME_IMAGE,
    DCD_CONTENT_NAME_MAX
}DCD_CONTENT_NAME_E;//please check

/*for CONTENT  autoplay*/
typedef enum
{
    DCD_CONTENT_AUTOPLAY_TYPE_Y,
    DCD_CONTENT_AUTOPLAY_TYPE_N,
    DCD_CONTENT_AUTOPLAY_TYPE_MAX
}DCD_CONTENT_AUTOPLAY_TYPE_E;//please check

/*for ENTRY  type*/
typedef enum
{
    DCD_ENTRY_TYPE_UNKNOWN,
    DCD_ENTRY_TYPE_CS,
    DCD_ENTRY_TYPE_CSTO,
    DCD_ENTRY_TYPE_ETTI,
    DCD_ENTRY_TYPE_ETTO,
    DCD_ENTRY_TYPE_ETIO,
    DCD_ENTRY_TYPE_ETMM,
    DCD_ENTRY_TYPE_MAX
}DCD_ENTRY_TYPE_E;//please check

/*for TEMPLATE_ID*/
typedef enum
{
    DCD_TEMPLATE_ID_UNKNOWN,
    DCD_TEMPLATE_ID_DEFAULT,
    DCD_TEMPLATE_ID_MAX
}DCD_TEMPLATE_ID_E;//please check

/**/
/*for FEED_TYPE_ID*/
typedef enum
{
    DCD_FEED_TYPE_UNKNOWN,
    DCD_FEED_TYPE_FTTO,
    DCD_FEED_TYPE_FTIO,
    DCD_FEED_TYPE_FTTI,
    DCD_FEED_TYPE_FTMM,
    DCD_FEED_TYPE_MAX            
}DCD_FEED_TYPE_E;

typedef enum
{
    DCD_TIME_TYPE_UNKNOWN,
    DCD_TIME_TYPE_REL,
    DCD_TIME_TYPE_ABSOLUTE,
    DCD_TIME_TYPE_MAX            
}DCD_TIME_TYPE_E;

typedef enum _MMIDCDXML_ELEMENT_ID_E
{                          
    DCDXML_ELEMENT_ID_UNUSE = 0, 
    
    //                       
    DCDXML_ELEMENT_ID_SYNC_PKG,      
                            
    DCDXML_ELEMENT_ID_META,         

    DCDXML_ELEMENT_ID_GENERATOR,
                            
    DCDXML_ELEMENT_ID_GLOBAL_META,   
    DCDXML_ELEMENT_ID_FEED_META,     
                            
                            
    DCDXML_ELEMENT_ID_FEED,          
    DCDXML_ELEMENT_ID_TITLE,         
    DCDXML_ELEMENT_ID_SUMMARY,       
    DCDXML_ELEMENT_ID_LINK,          
    DCDXML_ELEMENT_ID_CONTENT,       
    DCDXML_ELEMENT_ID_ENTRY,         
    DCDXML_ELEMENT_ID_ISSUED,        
    DCDXML_ELEMENT_ID_AUTHOR,        
    DCDXML_ELEMENT_ID_ACTIVATION,    
    DCDXML_ELEMENT_ID_EXPIRATION,    
    DCDXML_ELEMENT_ID_NAME,          
    DCDXML_ELEMENT_ID_URL,           
    DCDXML_ELEMENT_ID_EMAIL,                        

    /*invalid*/
    DCDXML_ELEMENT_ID_INVALID_HTML,
    
    DCDXML_ELEMENT_ID_MAX_ID    
} MMIDCDXML_ELEMENT_ID_E;   

//5.parser器的状态:
typedef enum _DCDXML_PARSER_STATUS_T
{
	DCDXML_INIT = 0,	//initial status
	
	DCDXML_START,
	
	DCDXML_META_START,//==meta_parser()<-->meta_struct
	DCDXML_META_END,
	
	DCDXML_FEED_START,//==feed_parser<-->feed_struct
	DCDXML_ENTRY_END  = DCDXML_FEED_START,	//important!!!
	
	DCDXML_ENTRY_START,//==entry_parser<-->entry_struct
	//DCDXML_ENTRY_END,
	
	DCDXML_FEED_END,
	
	
	DCDXML_END = DCDXML_INIT

}	DCDXML_PARSER_STATUS_T;

typedef struct 
{	
    uint8 dcd_sec;     //the second - [0,59] 
    uint8 dcd_min;     //the minute - [0,59] 
    uint8 dcd_hour;    //the hour - [0,23] 
    uint8 dcd_mday;    // day of the month - [1,31] 
    uint8 dcd_mon;     // months since January - [0,11] 
    int   dcd_year;    // years since 2000 
    uint8 dcd_wday;    // days since Sunday - [0,6] 
}DCD_DATE_TIME_T;  

/*global meta*/
typedef struct  _DCD_GLOBAL_META_T
{
    char *global_meta_anchor_ptr;
    uint32 global_meta_ttl;
    uint32 global_meta_server_time;
}DCD_GLOBAL_META_T;

/*feed meta*/
typedef struct _DCD_FEED_META_T
{
    char          feed_meta_ctxt_id[DCD_MAX_TXT_VALUE_LEN + 1];
    char         *feed_meta_anchor_ptr;
    char          feed_meta_target[DCD_MAX_FEEDMETA_TARGET_LEN + 1];
    DCD_CHARGE_TYPE_E   feed_meta_charge;
    struct _DCD_FEED_META_T   *next_ptr;
}DCD_FEED_META_T;

/*meta genernator*/
typedef struct _DCD_META_GENERATOR_T
{
    char generator_url[DCD_MAX_URL_LEN + 1];
    char generator_version[DCD_MAX_GENERATOR_VERSION_LEN + 1];
}DCD_META_GENERATOR_T;

/*title
example :<title>澶╂皵棰勬姤</title>
*/

typedef struct _DCD_UCS2_STR_T
{
    uint16  str_len;
    uint8  *str_ptr;
}DCD_UCS2_STR_T;

typedef struct _DCD_TITLE_T
{
#if 0	
    char title_type[DCD_MAX_TITLE_LEN + 1];
#endif 
    DCD_UCS2_STR_T ucs2_str;
}DCD_TITLE_T;

/*summary
example :<summary>28鏃?8鑷?9鏃?8儴銆佽タ栭浄闃甸洦銆?/summary>
*/
typedef struct _DCD_SUMMARY_T
{
#if 0	
    char summary_type[DCD_MAX_SUMMARY_TYPE_LEN + 1];  
#endif       
    DCD_UCS2_STR_T ucs2_str;
}DCD_SUMMARY_T;

/*link
example :    <link rel="alternate" href="http://wapnews.i139.cn/dcd/dcd.do?j=queryTempl&amp;f=1107&amp;n=channelPara" type="application/xhtml+xml" />
*/
typedef struct _DCD_LINK_T
{
    DCD_LINK_REL_E      link_rel;   //char      rei[DCD_MAX_LINK_REL_LEN];         //"alternate" only
    DCD_TITLE_T         link_title;           //type + title
    char                *href_ptr;            //MAX size = 1024
    DCD_LINK_TYPE_E    link_type;  //char      type[DCD_MAX_LINK_TYPE_LEN];    
}DCD_LINK_T;

/*content*/
typedef struct _DCD_CONTENT_T
{
    DCD_CONTENT_TYPE_E      content_type;      //char      type[DCD_MAX_CONTENT_TYPE_LEN];
    DCD_CONTENT_MODE_E      content_mode;      //char      mode[DCD_MAX_CONTENT_MODE_LEN];
    DCD_CONTENT_NAME_E      content_name;      //char      name[DCD_MAX_CONTENT_NAME_LEN];
    BOOLEAN                 picture_valiad;    // added for 解决更新中有10几秒停顿的问题....
    char                    ctxt_id[DCD_MAX_TXT_VALUE_LEN + 1];
    char                    *href_ptr;
    struct _DCD_CONTENT_T *next_ptr;
}DCD_CONTENT_T;

/*entry author*/
typedef struct _DCD_ENTRY_AUTHOR_T
{
    DCD_UCS2_STR_T    author_name;
    char    author_url[DCD_MAX_URL_LEN + 1];
    char    author_email[DCD_MAX_AUTHOR_EMAIL_LEN + 1];
}DCD_ENTRY_AUTHOR_T;

/*entry*/
typedef struct _DCD_ENTRY_T
{
    char                    ctxt_id[DCD_MAX_TXT_VALUE_LEN + 1];
    DCD_ENTRY_TYPE_E        entry_type;
    DCD_TEMPLATE_ID_E template_id;
    char                  entry_excerpt[DCD_MAX_EXCERPT_LEN + 1];
    char                 *entry_template_href_ptr;
    DCD_TITLE_T   entry_title; 
    DCD_SUMMARY_T entry_summary;
    DCD_LINK_T    entry_link;
    uint32      issued_time;
    DCD_ENTRY_AUTHOR_T  entry_author;
    DCD_CONTENT_T *content_ptr;
    uint32    entry_activation;
    uint32    entry_expiration;
    struct    _DCD_ENTRY_T *next_ptr;
}DCD_ENTRY_T;

/*feed*/
typedef struct _DCD_FEED_T
{
    char                   feed_version[DCD_MAX_FEED_VERSION_LEN + 1];
    char                   ctxt_id[DCD_MAX_TXT_VALUE_LEN + 1];    
    DCD_FEED_TYPE_E        feed_type;
    DCD_TEMPLATE_ID_E      template_id;   
    char                     feed_entry_template_id[DCD_MAX_FEED_TEMPLATE_ID_LEN + 1];
    char                     feed_excerpt[DCD_MAX_EXCERPT_LEN + 1];
    char                     feed_excerpt_default[DCD_MAX_EXCERPT_LEN + 1];    
    char                  *feed_template_href_ptr;
    BOOLEAN  is_order;    
                
    DCD_TITLE_T   feed_title;
    DCD_SUMMARY_T feed_summary;
    DCD_LINK_T    feed_link;
    DCD_CONTENT_T *content_ptr;
    DCD_ENTRY_T   *entry_list_head_ptr;
    struct _DCD_FEED_T    *next_ptr;
}DCD_FEED_T;

/*meta  and a feed list*/
typedef struct _DCD_META_T
{
    DCD_GLOBAL_META_T     global_meta;
    DCD_META_GENERATOR_T  generator;
    DCD_FEED_META_T      *feed_meta_list_ptr;
}DCD_META_T;

/*sync pkg*/
typedef struct _DCD_SYNC_PKG
{
    DCD_META_T    meta;    
    DCD_FEED_T    *feed_list_ptr;   
}DCD_SYNC_PKG_T;
typedef DCD_SYNC_PKG_T*   xmlDocPtr;

typedef struct _XML_USERDATA
{
	
	int depth;	
	MMIDCDXML_ELEMENT_ID_E	cur_element_id;	
	
	//==
	DCDXML_PARSER_STATUS_T parser_status;	

	DCD_META_T			*cur_meta;
	DCD_FEED_T			*cur_feed;			
	DCD_ENTRY_T 		*cur_entry;
	//==
	

	DCD_SYNC_PKG_T		sync_pkg;//dcd_xml;	
	
}	XML_USERDATA;


#define DCD_APN_MAX_LEN  32
#define DCD_USER_AGENT_MAX_LEN  255
#define DCD_PROXY_MAX_LEN  66
#define DCD_USERNAME_MAX_LEN  66
#define DCD_PASSWORD_MAX_LEN  66
#define DCD_REQ_TYPE_MAX_LEN  20
#define DCD_VERSION_MAX_LEN  21



typedef void(*DCD_SigDispatch)(uint16 sig_code, uint16 para);

/****************************************************************************/
//  Description: Parses a XML file
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIDCD_XmlParse(wchar  *  xml_file_full_path,  
                               BOOLEAN   is_prefile_parse  //是否是前一个xml文件的解析...
                               );
/*****************************************************************************/
//  Description :  when download,get the content
//  Global resource dependence : 
//  Author:
//  Note: 
//    /*xml解析完后调用*/
//    MMIDCD_DownloadContentByList(MMIDCD_GetXMLData(),&Http_DownloadContentCallback);
/*****************************************************************************/
typedef void (*DCD_TRAVERSE_CONTENT_CALLBACK)(DCD_CONTENT_T* content_ptr);
PUBLIC void MMIDCD_DownloadContentByList(
                                         DCD_SYNC_PKG_T *pkg_ptr,
                                         DCD_TRAVERSE_CONTENT_CALLBACK callback
                                         );

/*****************************************************************************/
//  Description :  get the suffix of multimedia file
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIDCD_GetContentTypeStr(DCD_CONTENT_TYPE_E type_id);
/*****************************************************************************/
// 	Description : 获取feed总数
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/                         
PUBLIC uint16 MMIDCD_GetFeedNum(xmlDocPtr doc_ptr);

/*****************************************************************************/
// 	Description : 获取指定feed中Item总数
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetEntryNumInFeed(xmlDocPtr doc_ptr, uint16 FeedIndex);
/*****************************************************************************/
// 	Description : 获取指定feed中Item总数
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 与上面一个函数的区别是这里是指定好了的doc, 用户只需要调用就好了...
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetFormalEntryNumInFeed(uint16 FeedIndex);
/*****************************************************************************/
// 	Description : 获取指定feed中content总数，因一个频道可以包含多个content
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/                
PUBLIC uint16 MMIDCD_GetContentNumInFeed(
                                        xmlDocPtr doc_ptr, 
                                        uint16 FeedIndex
                                        );
/*****************************************************************************/
// 	Description : 获取指定频道指定Item的content数目
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetContentNumInEntry(
                                          xmlDocPtr doc_ptr,
                                          uint16 FeedIndex,
                                          uint16 EntryIndex
                                          );
/*****************************************************************************/
// 	Description : 获取指定频道的summary
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC DCD_UCS2_STR_T* MMIDCD_GetFeedSummary(
                                             xmlDocPtr doc_ptr,
                                             uint16 FeedIndex
                                             );
/*****************************************************************************/
// 	Description : 获取指定频道的title
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/9/3 重写了这个函数...9/25,注释后面加上out....
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetFeedTitle(
                                  wchar * wstr_title_ptr, //out 返回wchar型的title
                                  const uint16  dst_max_count,  
                                  const uint16  FeedIndex
                                  );
/*****************************************************************************/
// 	Description : 获取指定频道指定新闻的title(返回uint8 型的，和存储的一致）
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/9/1 重写了这个函数...
/*****************************************************************************/
PUBLIC uint8* MMIDCD_GetEntryStrTitle(
                                      uint16 *titlr_len_ptr,  //out
                                      const uint16  FeedIndex,
                                      const uint16  EntryIndex
                                      ); 
/*****************************************************************************/
// 	Description : 获取指定频道指定新闻的summary
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 
/*****************************************************************************/                   
PUBLIC uint8* MMIDCD_GetEntryStrSummary(
                                        uint16 *summary_len_ptr, //out 返回存储的字符串的长度...
                                        const uint16  FeedIndex,
                                        const uint16  EntryIndex
                                        );        

/*****************************************************************************/
// 	Description : 获取指定频道指定Item的link属性值
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC char * MMIDCD_GetEntryLink(
                                  uint16 FeedIndex,
                                  uint16 EntryIndex,
                                  const char * AttrName
                                  );
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:byte.guo
//  Note:modified by jun.hu 2009/10/13, 根据flag, 看获得哪一个xml 文件对应的内存影像数据...
/*****************************************************************************/
PUBLIC XML_USERDATA* MMIDCD_GetXMLData(BOOLEAN get_prefile_Data);
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:byte.guo
//  Note: modified by jun.hu 2009/10/13, 根据flag, 看释放哪一个xml文件的内存影像数据...
/*****************************************************************************/
PUBLIC void MMIDCD_XMLDataFree(BOOLEAN free_prefile_Data);
/****************************************************************************/
//  Description: Parses a static XML file
//  Global resource dependence
//  Auther: zhuo.wang
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN DCDXML_ParseSysFile(const char* xml_ptr,uint32 len);

/*****************************************************************************/
//  Description :  Does the  ctxt_id exist in a syncpkt ?
//  Global resource dependence : 
//  Author:
//  Note: if exist,return TURE;otherwise return FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsContentExist(DCD_SYNC_PKG_T *syncpkg_ptr,char* ctxt_id_ptr);
/*****************************************************************************/
// 	Description : 获取指定频道指定新闻(即频道项）的内容的全局的唯一的id
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/10/10
/*****************************************************************************/                   
PUBLIC char * MMIDCD_GetEntryContentCTXTId(
                                           const xmlDocPtr doc_ptr,
                                           const uint16  FeedIndex,
                                           const uint16  EntryIndex
                                           );
/*****************************************************************************/
// 	Description : 获取指定频道的全局的唯一的id
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/10/12
/*****************************************************************************/                   
PUBLIC char * MMIDCD_GetFeedCTXTId(
                                  const xmlDocPtr doc_ptr,
                                  const uint16  FeedIndex
                                  );

/*****************************************************************************/
// 	Description : 释放一个feed
//	Global resource dependence : none
//  Author:panvs.zhong
//	Note:
/*****************************************************************************/                         
PUBLIC void MMIDCD_FeedFree(DCD_FEED_T *feed_ptr);
/*****************************************************************************/
// 	Description : 释放一个entry
//	Global resource dependence : none
//  Author:panvs.zhong
//	Note:
/*****************************************************************************/                         
PUBLIC void MMIDCD_EntryFree(DCD_ENTRY_T *entry_ptr);
/*****************************************************************************/
// 	Description : 获取feed总数
//	Global resource dependence : none
//  Author:panvs.zhong
//	Note:
/*****************************************************************************/                         
PUBLIC uint16 MMIDCD_GetRealFeedNum(xmlDocPtr doc_ptr);
/*****************************************************************************/
// 	Description : 获取feed 中Entry 总数
//	Global resource dependence : none
//  Author:panvs.zhong
//	Note: 
/*****************************************************************************/                         
PUBLIC uint16 MMIDCD_GetRealEntryNum(xmlDocPtr doc_ptr, uint16 feedIndex);
/*****************************************************************************/
//    Description : 获取指定频道
//    Global resource dependence : none
//    Author:panvs.zhong
//    Note: 2011-07-15
/*****************************************************************************/                   
PUBLIC DCD_FEED_T * MMIDCD_GetFeedByIndex(DCD_FEED_T * list_head_ptr, const uint16  index);
/*****************************************************************************/
//    Description : 获取指定频道的指定频道项
//    Global resource dependence : none
//    Author:panvs.zhong
//    Note: 2011-07-15
/*****************************************************************************/                  
PUBLIC DCD_ENTRY_T * MMIDCD_GetEntryByIndex(xmlDocPtr doc_ptr, uint16 feedIndex, uint16 index);

#ifdef   __cplusplus
}
#endif

#endif/*#ifdef MMI_ENABLE_DCD*/
#endif/*#ifdef _MMIDCD_XML_PARSE_H_*/
