/******************************************************************************
** Author:                                                             
** Date:           23/10/2008                                    
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.   
** Description:    
*******************************************************************************
			     
*****************************************************************************
**                         Important Edit History                           
** --------------------------------------------------------------------------
** DATE           	NAME             	DESCRIPTION                               
** 23/10/2008       byte.guo       		Create                                 
******************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_dcd_trc.h"
#ifdef MMI_ENABLE_DCD	//!宏定义开头
#ifdef WIN32
#include "std_header.h"
#endif
#include "expat_external.h"
#include "expat.h"
#include "mmidcd_export.h"
#include "mmidcd_main.h"
#include "mmidcd_xml_parse.h"
#include "dal_time.h"
#include "gui_ucs2b_converter.h"
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
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
#define MMIDCD_TYPE_NAME        "ctxt-id"   //名字
#define MMIDCD_TYPE_SUFFIX      "type"      //后缀
#define MMIDCD_LINK_ATTR        "href"      //link attr
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
//LOCAL uint32 s_expat_malloc_count=0;
//LOCAL uint32 s_expat_realloc_count=0;
LOCAL  BOOLEAN s_xml_valid = TRUE;

LOCAL const SCI_DATE_T s_sys_begin_date = 
    {1,  1,  1970,  6};

LOCAL const uint16  s_day_of_month[2][12] =
    {
        {31,  28,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31  },
        {31,  29,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31  }
    };

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern int stricmp(char * s1, char * s2);/*lint !e18*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
XML_USERDATA	  g_user_data = {0};
//added by jun.hu 2009/10/13, for cr 用来保存解析更新前的XML文件的数据...
static XML_USERDATA	  s_user_pre_data = {0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void    *expat_Malloc(size_t s);
LOCAL void    *expat_realloc(void *p, size_t ns);
LOCAL void    expat_Free(void *p);
size_t  expat_AllocationSize(void *p);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL MMIDCDXML_ATTRIBUTE_ID_E GetAttributeID(const char *name);

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL uint16 UTF82UCS2B(//ucs2b len
					uint8 *ucs2b_ptr,//out
					uint16 ucs2b_buf_len,//in
					const uint8 *utf8_ptr,//in
					uint16 utf8_len//in
					);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL int ConvertToUCS2(void *data, const char *s);
/*****************************************************************************/
//  Description :  the functioms above convert the string to enum
//  Global resource dependence :none
//  Author:
//  Note: 
//  map[i]  值意义,
//      >=0 表示对应的值是单字符
//      -1  表示非法字符
//      -2,-3,-4    表示是多字节字符的第一个,分别是2字节、3字节、4字节的字符
//  使用:
//      expat会根据map[(unsigned char)s[i]] 的值判断某个字符后面跟多少个字节，
//      然后跳过对应的数目.
//  
//  info->data    附加数据，这个只会回传给convert和release,expat内部不进行处理,
//  expat解析过程中会调用convert,解析完后调用release(如果这两个值不为NULL的话)
//  如果是单字节字符集，那convert可以为NULL,
//  如果是多字节安符集,convert应该有值,负责把对应的语言转换成ucs2编码
/*****************************************************************************/
LOCAL int XMLCALL EncodingHandle( void *encodingHandlerData,
                                  const XML_Char *name_ptr,
                                  XML_Encoding *info_ptr
                                  );

/*****************************************************************************/
//  Description :  the functioms above convert the string to enum
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIDCDXML_ELEMENT_ID_E GetElementID(const char *name);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void SetCharData(	XML_USERDATA* userdata,uint8*	ucs2_buf_ptr,uint16  ucs2_str_len  );
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void StatusInit(XML_USERDATA* userdata,MMIDCDXML_ELEMENT_ID_E start_element_id);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note://MetaParser()对<meta数据>进行<内部操作>
/****************************************************************************/ 
LOCAL void MetaParser(XML_USERDATA* userdata, const char *el, const char **attr);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note://<内部处理>构建&设置相关属性
/****************************************************************************/ 
LOCAL void FeedParser(XML_USERDATA* userdata, const char *el, const char **attr);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void EntryParser(XML_USERDATA* userdata, const char *el, const char **attr);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void XMLCALL CharacterDataHandler(void *data,const XML_Char *s,int len);
/****************************************************************************/
//  Description: StartElementHandler
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void XMLCALL StartElementHandler(
                                        void *data,
                                        const XML_Char *name,
                                        const XML_Char **atts
                                      );
/****************************************************************************/
//  Description: StartElementHandler
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void XMLCALL EndElementHandler(void *data, const XML_Char *name);

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_LINK_REL_E GetLinkRel(const char *str);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_CONTENT_MODE_E GetContentMode(const char *str);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_CONTENT_TYPE_E GetContentType(const char *str);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_CONTENT_NAME_E GetContentName(const char *str);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_TEMPLATE_ID_E GetTemplateId(const char *str);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_ENTRY_TYPE_E GetEntryType(const char *str);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_FEED_TYPE_E GetFeedType(const char *str);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_CHARGE_TYPE_E GetMetaFeedCharge(const char *str);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_TIME_TYPE_E GetTimeType(const char *str);
/*****************************************************************************/
//  Description : convert str to second
//  Global resource dependence : 
//  Author:
//  Note:	
///*****************************************************************************/
LOCAL uint32 StrToSecond(const char *str_ptr) ;
/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,
//                which's in the struct of DCD_FEED_META_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFeedMetaAnchor(DCD_FEED_META_T * feed_meta_ptr,
                        const char *auchor_ptr,
                        uint16 auchor_len
                        );
/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_FEED_META_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FeedMetaAnchorFree(DCD_FEED_META_T * feed_meta_ptr);
/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,
//                  which's in the struct of DCD_GLOBAL_META_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetGlobalMetaAnchor(DCD_GLOBAL_META_T * global_meta_ptr,
                                const char *auchor_ptr, 
                                uint16 auchor_len
                                );
/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_GLOBAL_META_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void GlobalMetaAnchorFree(DCD_GLOBAL_META_T * global_meta_ptr);
/*****************************************************************************/
//  Description :   allocate memory of  the certain size for href ,
//                  which's in the struct of DCD_LINK_T
//  Global resource dependence :none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetLinkHref(DCD_LINK_T * link_ptr,const char *href_ptr, uint16 href_len);
/*****************************************************************************/
//  Description :   free memory of  href ,which's in the struct of DCD_LINK_T
//  Global resource dependence :none
//  Author:
//  Note: free memory of  href ,which's in the struct of DCD_LINK_T
/*****************************************************************************/
LOCAL void LinkHrefFree(DCD_LINK_T * link_ptr);
/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,
//                 which's in the struct of DCD_FEED_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFeedHref(DCD_FEED_T * feed_ptr,const char *href_ptr, uint16 href_len);
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void FeedHrefFree(DCD_FEED_T * feed_ptr);
/*****************************************************************************/
//  Description :  allocate memory of  the certain size for str ,
//                 which's in the struct of DCD_UCS2_STR_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CopyString(DCD_UCS2_STR_T *ucs2_ptr,uint8 *data_ptr, uint16 data_len);
/**************************************************************************/
//  Description :  free memory of  str ,which's in the struct of DCD_UCS2_STR_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StringFree(DCD_UCS2_STR_T *ucs2_ptr);
/*****************************************************************************/
//  Description :   free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetTitle(DCD_TITLE_T * title_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void TitleFree(DCD_TITLE_T * title_ptr);

/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetSummary(DCD_SUMMARY_T * summary_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SummaryFree(DCD_SUMMARY_T * summary_ptr);
/*****************************************************************************/
//  Description :  set feed title
//  Global resource dependence : the feed exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFeedTitle(DCD_FEED_T * feed_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  set entry title
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryTitle(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  set feed summary
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFeedSummary(DCD_FEED_T * feed_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  set entry summary
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntrySummary(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,
//                 which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetContentHref(DCD_CONTENT_T * content_ptr,const char *href_ptr, uint16 href_len);
/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ContentHrefFree(DCD_CONTENT_T * content_ptr);
/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,
//                 which's in the struct of DCD_ENTRY_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryHref(DCD_ENTRY_T * entry_ptr,const char *href_ptr, uint16 href_len);
/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_ENTRY_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EntryHrefFree(DCD_ENTRY_T * entry_ptr);
/*****************************************************************************/
//  Description :  set the attributes for global_meta
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GlobalMetaSetAttribute(DCD_GLOBAL_META_T  *global_meta_ptr, 
                                     const char **attr_pptr
                                     );
/*****************************************************************************/
//  Description :  set the attributes for feed_meta
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FeedMetaSetAttribute(DCD_FEED_META_T  *feed_meta_ptr, 
                                   const char **attr_pptr
                                   );
/*****************************************************************************/
//  Description :  set the attributes for meta_generator
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MetaGeneratorSetAttribute(DCD_META_GENERATOR_T  *meta_generator_ptr, 
                                        const char **attr_pptr
                                        );
/*****************************************************************************/
//  Description : set the attributes for link
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN LinkSetAttribute(DCD_LINK_T  *link_ptr,const char **attr_pptr);
/*****************************************************************************/
//  Description :  set the attributes for entry author
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN EntryAuthorSetAttribute(DCD_ENTRY_AUTHOR_T *entry_author_ptr, 
                                      const char **attr_pptr
                                      );
/*****************************************************************************/
//  Description :  set the attributes for ontent
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ContentSetAttribute(DCD_CONTENT_T * content_ptr, 
                                  const char **attr_pptr
                                  );
/*****************************************************************************/
//  Description :  set the attributes for entry
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN EntrySetAttribute(DCD_ENTRY_T* entry_ptr, const char **attr_pptr);
/*****************************************************************************/
//  Description :  set the attributes for feed
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FeedSetAttribute(DCD_FEED_T* feed_ptr, const char **attr_pptr);
/*****************************************************************************/
//  Description :   free feed_meta 
//  Global resource dependence :the content exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FeedMetaFrameFree( DCD_FEED_META_T *feed_meta_ptr);
/*****************************************************************************/
//  Description :  allocate memory of DCD_FEED_META_T
//  Author:
//  Note: 

/*****************************************************************************/
LOCAL DCD_FEED_META_T *FeedMetaFrameMalloc(void);
/*****************************************************************************/
//  Description :  allocate memory of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: allocate memory of DCD_CONTENT_T
/*****************************************************************************/
LOCAL DCD_CONTENT_T *ContentFrameMalloc(void);
/*****************************************************************************/
//  Description :  free content
//  Global resource dependence :the content exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ContentFrameFree( DCD_CONTENT_T *content_ptr);
/*****************************************************************************/
//  Description :  allocate memory of DCD_ENTRY_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_ENTRY_T *EntryFrameMalloc(void);
/*****************************************************************************/
//  Description :  free a entry
//  Global resource dependence :the entry exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EntryFrameFree(DCD_ENTRY_T *entry_ptr);
/*****************************************************************************/
//  Description :  allocate memory of DCD_FEED_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_FEED_T *FeedFrameMalloc(void);
/*****************************************************************************/
//  Description :  free  a  feed
//  Global resource dependence :the feed exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FeedFrameFree(DCD_FEED_T *feed_ptr);
/*****************************************************************************/
//  Description :  build feed_meta list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL DCD_FEED_META_T *FeedMetaAdd2List(DCD_FEED_META_T **list_head_ptr, DCD_FEED_META_T *node_ptr);
/*****************************************************************************/
//  Description :  free feed_meta list
//  Global resource dependence :the content list exists
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FeedMetaListFree( DCD_FEED_META_T **list_head_ptr);
/*****************************************************************************/
//  Description : build content list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL  DCD_CONTENT_T *ContentAdd2List(DCD_CONTENT_T **list_head_ptr, DCD_CONTENT_T *node_ptr);
/*****************************************************************************/
//  Description :   free content list
//  Global resource dependence :the content list exists
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ContentListFree( DCD_CONTENT_T **list_head_ptr);
/*****************************************************************************/
//  Description :  build entry list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL DCD_ENTRY_T *EntryAdd2List(DCD_ENTRY_T **list_head_ptr, DCD_ENTRY_T *node_ptr);
/*****************************************************************************/
//  Description :  free  entry list
//  Global resource dependence :entry list exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EntryListFree(DCD_ENTRY_T **list_head_ptr);
/*****************************************************************************/
//  Description :  build feed list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL DCD_FEED_T *FeedAdd2List(DCD_FEED_T **list_head_ptr, DCD_FEED_T *node_ptr);
/*****************************************************************************/
//  Description :  free  feed list
//  Global resource dependence :the feed list exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FeedListFree(DCD_FEED_T  **list_head_ptr);
/*****************************************************************************/
//  Description :  Does the content named ctxt_id exist in a entry ?
//  Global resource dependence : 
//  Author:
//  Note: if exist,return the content node;otherwise return PNULL
/*****************************************************************************/
LOCAL DCD_CONTENT_T *ContentIsExistInEntry(DCD_ENTRY_T *entry_ptr,char* ctxt_id_ptr);
/*****************************************************************************/
//  Description :  Does the content named ctxt_id exist in a feed ?
//  Global resource dependence : 
//  Author:
//  Note: if exist,return the content node;otherwise return PNULL
/*****************************************************************************/
LOCAL DCD_CONTENT_T *ContentIsExistInFeed(DCD_FEED_T *feed_ptr,char* ctxt_id_ptr);
/*****************************************************************************/
//  Description :   Reset global_meta
//  Global resource dependence : the entry exist
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void GlobalMetaReset(DCD_GLOBAL_META_T  *global_meta_ptr);

/*****************************************************************************/
//  Description :  Reset meta
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MetaReset( DCD_META_T *meta_ptr);

/*****************************************************************************/
//  Description :  Reset ync_pkg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SyncPkgReset(DCD_SYNC_PKG_T *sysc_pkg_ptr);
/*****************************************************************************/
//  Description :   Reset global_meta
//  Global resource dependence : the entry exist
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UserDataReset(XML_USERDATA* userdata_ptr);
/*****************************************************************************/
//  Description :  set entry issued time
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryIssued(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  set entry activation time
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryActivation(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  set entry expiration time
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryExpiration(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  set entry autherURL
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryAutherName(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len);

/*****************************************************************************/
//  Description :  free memory of  author_name ,which's in the struct of DCD_ENTRY_AUTHOR_T
//  Global resource dependence :none
//  Author:
//  Note: free memory of  href ,which's in the struct of DCD_CONTENT_T
/*****************************************************************************/
LOCAL void EntryAuthorNameFree(DCD_ENTRY_AUTHOR_T * author_ptr);

/*****************************************************************************/
//  Description :  set entry autherURL
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryAutherUrl(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  set entry auther Name
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryAutherEmail(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len);
/*****************************************************************************/
//  Description :  get the suffix of multimedia file
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL char* GetContentTypeStr(DCD_CONTENT_TYPE_E type_id);
/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: index from 0.....
/*****************************************************************************/
LOCAL DCD_FEED_META_T *GetFeedMetaByIndex(DCD_FEED_META_T * list_head_ptr,uint16 index);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: Note: index from 0.....
/*****************************************************************************/
LOCAL DCD_CONTENT_T *GetContentByIndex(DCD_CONTENT_T * list_head_ptr,uint16 index);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: Note: index from 0.....
/*****************************************************************************/
LOCAL DCD_ENTRY_T *GetEntryByIndex(DCD_ENTRY_T * list_head_ptr,uint16 index);


/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: Note: index from 0.....
/*****************************************************************************/
LOCAL DCD_FEED_T *GetFeedByIndex(DCD_FEED_T * list_head_ptr,uint16 index);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : the feed exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetFeedMetaListNum(DCD_FEED_META_T *list_head_ptr);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : the feed exist
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL uint16 GetContentListNum(DCD_CONTENT_T *list_head_ptr);

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL uint16 GetEntryListNum(DCD_ENTRY_T *list_head_ptr);
/*****************************************************************************/
//  Description :  
//  Global resource dependence : the feed exist
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL uint16 GetFeedListNum(DCD_FEED_T *list_head_ptr);
/*****************************************************************************/
// 	Description : 获取指定feed指定Item节点
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
LOCAL DCD_ENTRY_T* GetEntryNode( 
                                DCD_SYNC_PKG_T* doc, 
                                uint16 FeedIndex, 
                                uint16 EntryIndex 
                                );
/*****************************************************************************/
//  Description : judge whether this year is leap year
//  Global resource dependence : 
//  Author: 
//Note:	
///*****************************************************************************/
LOCAL BOOLEAN IsLeapyear(int  year);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void *expat_Malloc(size_t s)
{
    char *p = PNULL; 
    
    if(s == 0)
    {
        return PNULL;
    }
//    SCI_TRACE_LOW("dcd_xml_parser:expat_Malloc, size=%d", s);
    p = (char *)MMIDCD_ALLOC(s+8);

    if( p != PNULL )
    {
        *(size_t *)p = s;
        p += 8;
    }    
//    s_expat_malloc_count ++;
//    SCI_TRACE_LOW("dcd_xml_parser:expat_Malloc ptr=0x%d malloc=%d", p, s_expat_malloc_count);

    return (void *)p;
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void *expat_realloc(void *p, size_t ns)
{
    char      *p2 = PNULL;
    size_t     len = 0;

    if(ns == 0)
    {
        return PNULL;
    }
#ifdef WIN32
    SCI_TRACE_LOW(("dcd_xml_parser:expat_realloc, new size=%d", ns));  /*lint !e64*/
#endif
    if (PNULL == p)
    {
        p2 = expat_Malloc(ns);
    }
    else
    {
    p2 = ((char *)p - 8);
    len = expat_AllocationSize(p);

//    SCI_TRACE_LOW(("dcd_xml_parser:expat_realloc, org size=%d", len));  /*lint !e64*/

    SCI_ASSERT(len>0);  /*assert verified*/
    SCI_ASSERT(ns>=len);  /*assert verified*/
    p2 = (char*)MMIDCD_ALLOC(ns+8);
    SCI_ASSERT( PNULL != p2 ); /*assert verified*/
    SCI_MEMSET( p2, 0, (ns+8) );
    *(size_t *)p2 = ns;
    p2 += 8;
    SCI_MEMCPY(p2,p,len);
    expat_Free(p);
//    s_expat_realloc_count ++;
//    SCI_TRACE_LOW(("dcd_xml_parser:expat_realloc ptr=0x%p realloc=%d", p, s_expat_realloc_count)); /*lint !e64*/

    }
    return (void *)p2;
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void expat_Free(void *p)
{
    void *rp = PNULL;

    if (PNULL != p)
    {
        rp = (void *)(((char*)p)-8);
        MMIDCD_FreeMem(rp);
    }

}

size_t expat_AllocationSize(void *p)
{
    //return p ? *(size_t *)((char *)p - 8) : 0;
    return (PNULL != p ? *(size_t *)((char *)p - 8) : 0);
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL const XML_Memory_Handling_Suite memsuite =
{
    expat_Malloc,
    expat_realloc,
    expat_Free
};


/*****************************************************************************/
// 	Description : convert utf8 to ucs big endian
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL uint16 UTF82UCS2B(//ucs2b len
					uint8 *ucs2b_ptr,//out
					uint16 ucs2b_buf_len,//in
					const uint8 *utf8_ptr,//in
					uint16 utf8_len//in
					)
{
	uint16	i = 0;
	uint16	j = 0;
	uint16	ucs2_char = 0;
	uint8	head_char = 0;
	
	if(PNULL== ucs2b_ptr || PNULL== utf8_ptr)
    {
        return 0;
    }

	while(i < utf8_len)
	{
		if(j + 1 >= ucs2b_buf_len)
        {
            return 0;
        }
		head_char = utf8_ptr[i++];

		if (head_char <= 0x7F)
		{
			//0xxx xxxx
			//0111 1111 = 0X7F
			ucs2b_ptr[j ++] = 0;
			ucs2b_ptr[j ++] = head_char;
		}
		else if (head_char <= 0xDF)
		{
			if (i < utf8_len)
			{
				//110x xxxx 10xx xxxx
				//1101 1111	== 0xDF

				//0001 1111 == 0x1F		0011 1111 == 0x3F
				ucs2_char = ((head_char & 0x1F) << 6) | ((utf8_ptr[i++] & 0x3F));
				ucs2b_ptr[j++] = (uint8)((ucs2_char & 0xFF00) >> 8);
				ucs2b_ptr[j ++] = (uint8)(ucs2_char & 0xFF);
			}
			else
			{
				break;
			}						
		}
		else if (head_char <= 0xEF)
		{
			if (i + 1 < utf8_len)
			{
				//1110 xxxx 10xx xxxx 10xx xxxx
				//1110 1111 == 0xEF

				//0000 1111 == 0xF	0011 1111 == 0x3F
				ucs2_char = ((head_char & 0xF) << 12) | ((utf8_ptr[i] & 0x3F) << 6) | (utf8_ptr[i + 1] & 0x3F);
				i += 2;
				ucs2b_ptr[j++] = (uint8)((ucs2_char & 0xFF00) >> 8);
				ucs2b_ptr[j++] = (uint8)(ucs2_char & 0xFF);
			}
			else
			{
				break;
			}
			
		}
		else
		{
			//SCI_TRACE_LOW:"[MMIDCD]:UTF82UCS2B ERROR!!!!!!!!!!!!!!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_785_112_2_18_2_13_8_299,(uint8*)"");
		}

	}

	return j;
}

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL int ConvertToUCS2(void *data, const char *s)
{
	//transform gb to ucs2,return ucs2 value

	return 1;
}



/*****************************************************************************/
//  Description :  the functioms above convert the string to enum
//  Global resource dependence :none
//  Author:
//  Note: 
//  map[i]  值意义,
//      >=0 表示对应的值是单字符
//      -1  表示非法字符
//      -2,-3,-4    表示是多字节字符的第一个,分别是2字节、3字节、4字节的字符
//  使用:
//      expat会根据map[(unsigned char)s[i]] 的值判断某个字符后面跟多少个字节，
//      然后跳过对应的数目.
//  
//  info->data    附加数据，这个只会回传给convert和release,expat内部不进行处理,
//  expat解析过程中会调用convert,解析完后调用release(如果这两个值不为NULL的话)
//  如果是单字节字符集，那convert可以为NULL,
//  如果是多字节安符集,convert应该有值,负责把对应的语言转换成ucs2编码
/*****************************************************************************/
LOCAL int XMLCALL EncodingHandle( void *encodingHandlerData,
                                  const XML_Char *name_ptr,
                                  XML_Encoding *info_ptr
                                  )
{

   int i   = 0;
   int ret = XML_STATUS_ERROR;
   if( PNULL == name_ptr ||  stricmp(name_ptr,"gb2312") )
   {
       ret= XML_STATUS_ERROR;
   }
   else
   {
       for(i=0; i<128; i++)
       {
           info_ptr->map[i] = i;
       }
       for(; i<256; i++)
       {
           info_ptr->map[i] = -2;
       }

       info_ptr->convert   = ConvertToUCS2;
       info_ptr->release   = NULL;
       ret = XML_STATUS_OK;
   }

   return ret;
}
/*****************************************************************************/
//  Description :  the functioms above convert the string to enum
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIDCDXML_ELEMENT_ID_E GetElementID(const char *name)
{
             
    if(  !stricmp(  name, DCDXML_ELEMENT_SYNC_PKG    ) )          return      DCDXML_ELEMENT_ID_SYNC_PKG;                                                          
    if(  !stricmp(  name, DCDXML_ELEMENT_META        ) )          return      DCDXML_ELEMENT_ID_META;   
    if(  !stricmp(  name, DCDXML_ELEMENT_GENERATOR   ) )          return      DCDXML_ELEMENT_ID_GENERATOR;   
    if(  !stricmp(  name, DCDXML_ELEMENT_GLOBAL_META ) )          return      DCDXML_ELEMENT_ID_GLOBAL_META;   
    if(  !stricmp(  name, DCDXML_ELEMENT_FEED_META   ) )          return      DCDXML_ELEMENT_ID_FEED_META;                                                                                                         
    if(  !stricmp(  name, DCDXML_ELEMENT_FEED        ) )          return      DCDXML_ELEMENT_ID_FEED;          
    if(  !stricmp(  name, DCDXML_ELEMENT_TITLE       ) )          return      DCDXML_ELEMENT_ID_TITLE;         
    if(  !stricmp(  name, DCDXML_ELEMENT_SUMMARY     ) )          return      DCDXML_ELEMENT_ID_SUMMARY;       
    if(  !stricmp(  name, DCDXML_ELEMENT_LINK        ) )          return      DCDXML_ELEMENT_ID_LINK;          
    if(  !stricmp(  name, DCDXML_ELEMENT_CONTENT     ) )          return      DCDXML_ELEMENT_ID_CONTENT;       
    if(  !stricmp(  name, DCDXML_ELEMENT_ENTRY       ) )          return      DCDXML_ELEMENT_ID_ENTRY;         
    if(  !stricmp(  name, DCDXML_ELEMENT_ISSUED      ) )          return      DCDXML_ELEMENT_ID_ISSUED;        
    if(  !stricmp(  name, DCDXML_ELEMENT_AUTHOR      ) )          return      DCDXML_ELEMENT_ID_AUTHOR;        
    if(  !stricmp(  name, DCDXML_ELEMENT_ACTIVATION  ) )          return      DCDXML_ELEMENT_ID_ACTIVATION;    
    if(  !stricmp(  name, DCDXML_ELEMENT_EXPIRATION  ) )          return      DCDXML_ELEMENT_ID_EXPIRATION;    
    if(  !stricmp(  name, DCDXML_ELEMENT_NAME        ) )          return      DCDXML_ELEMENT_ID_NAME;          
    if(  !stricmp(  name, DCDXML_ELEMENT_URL         ) )          return      DCDXML_ELEMENT_ID_URL;           
    if(  !stricmp(  name, DCDXML_ELEMENT_EMAIL       ) )          return      DCDXML_ELEMENT_ID_EMAIL;                        

    /*invalid*/	
     if(  !stricmp(name, DCDXML_ELEMENT_INVALID_HTML) )  return      DCDXML_ELEMENT_ID_INVALID_HTML;
	 
    return DCDXML_ELEMENT_ID_UNUSE;
}     

/*****************************************************************************/
//  Description :  the functioms above convert the string to enum
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMIDCDXML_ATTRIBUTE_ID_E GetAttributeID(const char *name)
{
    if(PNULL == name)
    {
        return DCDXML_ATTRIBUTE_ID_MAX;
    }
    
    if(!stricmp(name,DCDXML_ATTRIBUTE_TYPE_TXT))                 return DCDXML_ATTRIBUTE_ID_TYPE;
    if(!stricmp(name,DCDXML_ATTRIBUTE_MODE_TXT))                 return DCDXML_ATTRIBUTE_ID_MODE;          
    if(!stricmp(name,DCDXML_ATTRIBUTE_NAME_TXT))                 return DCDXML_ATTRIBUTE_ID_NAME;
    if(!stricmp(name,DCDXML_ATTRIBUTE_CTXT_ID_TXT))              return DCDXML_ATTRIBUTE_ID_CTXT_ID;
    if(!stricmp(name,DCDXML_ATTRIBUTE_ENTRY_TYPE_TXT))           return DCDXML_ATTRIBUTE_ID_ENTRY_TYPE;
    if(!stricmp(name,DCDXML_ATTRIBUTE_TEMPLATE_ID_TXT))          return DCDXML_ATTRIBUTE_ID_TEMPLATE_ID;
    if(!stricmp(name,DCDXML_ATTRIBUTE_REL_TXT))                  return DCDXML_ATTRIBUTE_ID_REL;
    if(!stricmp(name,DCDXML_ATTRIBUTE_HREF_TXT))                 return DCDXML_ATTRIBUTE_ID_HREF;
    if(!stricmp(name,DCDXML_ATTRIBUTE_URL_TXT))                  return DCDXML_ATTRIBUTE_ID_URL;
    if(!stricmp(name,DCDXML_ATTRIBUTE_EMAIL_TXT))                return DCDXML_ATTRIBUTE_ID_EMAIL;
    if(!stricmp(name,DCDXML_ATTRIBUTE_VERSION_TXT))              return DCDXML_ATTRIBUTE_ID_VERSION;
    if(!stricmp(name,DCDXML_ATTRIBUTE_ANCHOR_TXT))               return DCDXML_ATTRIBUTE_ID_ANCHOR;
    if(!stricmp(name,DCDXML_ATTRIBUTE_TTL_TXT))                  return DCDXML_ATTRIBUTE_ID_TTL;
    if(!stricmp(name,DCDXML_ATTRIBUTE_SERVICE_TIME_TXT))         return DCDXML_ATTRIBUTE_ID_SERVICE_TIME;
    if(!stricmp(name,DCDXML_ATTRIBUTE_TARGET_TXT))               return DCDXML_ATTRIBUTE_ID_TARGET;
    if(!stricmp(name,DCDXML_ATTRIBUTE_CHARGE_TXT))               return DCDXML_ATTRIBUTE_ID_CHARGE;        
    if(!stricmp(name,DCDXML_ATTRIBUTE_FEEDTYPE_TXT))             return DCDXML_ATTRIBUTE_ID_FEED_TYPE;        
    if(!stricmp(name,DCDXML_ATTRIBUTE_TITLE_TXT))                return DCDXML_ATTRIBUTE_ID_TITLE;     
    if(!stricmp(name,DCDXML_ATTRIBUTE_AUTOPLAY_TXT))             return DCDXML_ATTRIBUTE_ID_AUTOPLAY;   
    if(!stricmp(name,DCDXML_ATTRIBUTE_ENTRY_TEMPLATE_ID_TXT))    return DCDXML_ATTRIBUTE_ID_ENTRY_TEMPLATE_ID;
    if(!stricmp(name,DCDXML_ATTRIBUTE_EXCERPT_TXT))              return DCDXML_ATTRIBUTE_ID_EXCERPT;
    if(!stricmp(name,DCDXML_ATTRIBUTE_EXCERPT_DEFAULT_TXT))      return DCDXML_ATTRIBUTE_ID_EXCERPT_DEFAULT;
    if(!stricmp(name,DCDXML_ATTRIBUTE_TEMPLATE_HREF_TXT))        return DCDXML_ATTRIBUTE_ID_TEMPLATE_HREF;
	
    //SCI_TRACE_LOW:"GetATTRIBUTE_AttributeID() str='%s'	"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_919_112_2_18_2_13_9_300,(uint8*)"s",name);
    
    return DCDXML_ATTRIBUTE_ID_MAX;
}   
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void SetCharData(	XML_USERDATA* userdata, uint8*	ucs2_buf_ptr,uint16  ucs2_str_len  )
{
    if(PNULL == userdata)
    {
        return;
    }
    //对转换后的char数据进行保存。。。
    switch(userdata->cur_element_id)
    {
    case DCDXML_ELEMENT_ID_GENERATOR:
            break;
	
    case DCDXML_ELEMENT_ID_TITLE:
        {
            if( userdata->parser_status == DCDXML_FEED_START )
            {					
                SetFeedTitle(userdata->cur_feed, ucs2_buf_ptr, ucs2_str_len);
            }
            else if(  userdata->parser_status == DCDXML_ENTRY_START  )
            {
                SetEntryTitle(userdata->cur_entry, ucs2_buf_ptr, ucs2_str_len);
            }
            else
            {
                //xml is a general xml,but is invalid to DCD.
                s_xml_valid = FALSE;
                //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_TITLE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_953_112_2_18_2_13_9_301,(uint8*)"");
            }
        }
        break;
    
    case DCDXML_ELEMENT_ID_SUMMARY:
    {
        if( userdata->parser_status == DCDXML_FEED_START )
        {					
            SetFeedSummary(userdata->cur_feed, ucs2_buf_ptr, ucs2_str_len);/**/
        }
        else if(  userdata->parser_status == DCDXML_ENTRY_START  )
        {
            SetEntrySummary(userdata->cur_entry, ucs2_buf_ptr, ucs2_str_len);
        }
        else
        {
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_SUMMARY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_972_112_2_18_2_13_9_302,(uint8*)"");
        }
    }
    break;
    
    case DCDXML_ELEMENT_ID_ISSUED:
    {
        if(  userdata->parser_status == DCDXML_ENTRY_START  )
        {
            SetEntryIssued(userdata->cur_entry, ucs2_buf_ptr,ucs2_str_len);
        }
        else
        {
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_ISSUED"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_987_112_2_18_2_13_9_303,(uint8*)"");
        }
    }
    break;
    
    case DCDXML_ELEMENT_ID_ACTIVATION:
    {				
        if(  userdata->parser_status == DCDXML_ENTRY_START  )
        {				
            SetEntryActivation(userdata->cur_entry, ucs2_buf_ptr,ucs2_str_len);		
        }
        else
        {
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_ACTIVATION"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1002_112_2_18_2_13_9_304,(uint8*)"");
        }		
    }				
    break;
    
    case DCDXML_ELEMENT_ID_EXPIRATION:
    {							
        if(  userdata->parser_status == DCDXML_ENTRY_START  )
        {					
            SetEntryExpiration(userdata->cur_entry,ucs2_buf_ptr,ucs2_str_len);		
        }
        else
        {
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_EXPIRATION"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1017_112_2_18_2_13_9_305,(uint8*)"");
        }		
    }						
    break;
    
    
    case DCDXML_ELEMENT_ID_AUTHOR:
    {				
        if(  userdata->parser_status == DCDXML_ENTRY_START  )
        {								
        }
        else
        {
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_AUTHOR"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1032_112_2_18_2_13_9_306,(uint8*)"");
        }		
    }					
    break;
    
    
    case DCDXML_ELEMENT_ID_NAME:
    {				
        if(  userdata->parser_status == DCDXML_ENTRY_START  )
        {								
            SetEntryAutherName(userdata->cur_entry,ucs2_buf_ptr,ucs2_str_len);	
        }
        else
        {
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_NAME"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1048_112_2_18_2_13_9_307,(uint8*)"");
        }		
    }						
    break;			
    
    case DCDXML_ELEMENT_ID_URL:
    {				
        if(  userdata->parser_status == DCDXML_ENTRY_START  )
        {								
            SetEntryAutherUrl(userdata->cur_entry,ucs2_buf_ptr,ucs2_str_len);
        }
        else
        {
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_URL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1063_112_2_18_2_13_9_308,(uint8*)"");
        }		
    }				
    break;
    
    case DCDXML_ELEMENT_ID_EMAIL:
    {				
        if(  userdata->parser_status == DCDXML_ENTRY_START  )
        {								
            SetEntryAutherEmail(userdata->cur_entry,ucs2_buf_ptr,ucs2_str_len);
        }
        else
        {
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_EMAIL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1078_112_2_18_2_13_9_309,(uint8*)"");
        }		
    }				
    break;
    
    default:
    //SCI_TRACE_LOW:"DCD XML INVALID chardata = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1084_112_2_18_2_13_9_310,(uint8*)"d",userdata->cur_element_id);
    break;							
    }	
}

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void StatusInit(XML_USERDATA* userdata,MMIDCDXML_ELEMENT_ID_E start_element_id)
{
    if(PNULL == userdata)
    {
        return;
    }
    
    userdata->cur_element_id = start_element_id;
    switch(start_element_id)
    {
        case DCDXML_ELEMENT_ID_SYNC_PKG:
            userdata->parser_status = DCDXML_START;
            userdata->cur_meta  = NULL;
            userdata->cur_feed  = NULL;			
            userdata->cur_entry = NULL;			
            break;
		
        case DCDXML_ELEMENT_ID_META:
            userdata->parser_status = DCDXML_META_START;
            userdata->cur_meta  = &(userdata->sync_pkg.meta);
            userdata->cur_feed  = NULL;			
            userdata->cur_entry = NULL;				
            break;
        
        case DCDXML_ELEMENT_ID_FEED:
            userdata->parser_status = DCDXML_FEED_START;
            userdata->cur_meta  = NULL;
            //==
            ////////userdata->cur_feed  = 当前feed节点,???;		
            userdata->cur_feed = FeedFrameMalloc();			
            FeedAdd2List( &(userdata->sync_pkg.feed_list_ptr), userdata->cur_feed );
            //==
            userdata->cur_entry = NULL;				
            break;
			
        case DCDXML_ELEMENT_ID_ENTRY:
            userdata->parser_status = DCDXML_ENTRY_START;
            //cur_meta  = NULL;
            //cur_feed  = NULL;	
            //==
            ////////userdata->cur_entry = malloc_node,add_node_in_list,当前entry节点;	
            userdata->cur_entry = EntryFrameMalloc();
            EntryAdd2List( &(userdata->cur_feed->entry_list_head_ptr), userdata->cur_entry);
            //==
            break;	
			
        case DCDXML_ELEMENT_ID_INVALID_HTML:
            //If it's a html file,we should give up parser the file.          
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID DCDXML_EL_ID_INVALID_HTML"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1142_112_2_18_2_13_9_311,(uint8*)"");
            break;	
			
        default:
        break;							
    }
    
    
    
    userdata->depth += 1;	
	
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note://MetaParser()对<meta数据>进行<内部操作>
/****************************************************************************/ 
LOCAL void MetaParser(XML_USERDATA* userdata, const char *el, const char **attr)
{
    if(PNULL == userdata)
    {
        return;
    }
    //<内部处理>构建&设置相关属性
    switch(userdata->cur_element_id)
    {
    case DCDXML_ELEMENT_ID_META:
        break;
			
    case DCDXML_ELEMENT_ID_GENERATOR:
        MetaGeneratorSetAttribute(   &(userdata->cur_meta->generator), attr  );
        break;        
			
    case DCDXML_ELEMENT_ID_GLOBAL_META:
		GlobalMetaSetAttribute(  &(userdata->cur_meta->global_meta),attr  );
        break;     
			
    case DCDXML_ELEMENT_ID_FEED_META:
		{
			DCD_FEED_META_T * node_ptr = FeedMetaFrameMalloc();
			FeedMetaSetAttribute(  node_ptr,attr  );
			
            FeedMetaAdd2List(  &(userdata->cur_meta->feed_meta_list_ptr),node_ptr  );
			
		}
        break;
			
    case DCDXML_ELEMENT_ID_MAX_ID:
        //xml is a general xml,but is invalid to DCD.
        s_xml_valid = FALSE;
        //SCI_TRACE_LOW:"DCD XML INVALID  MAX ID MetaParser "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1191_112_2_18_2_13_9_312,(uint8*)"");
        break;
			
    default:
        //xml is a general xml,but is invalid to DCD.
        s_xml_valid = FALSE;
        //SCI_TRACE_LOW:"DCD XML INVALID  MetaParser"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1197_112_2_18_2_13_9_313,(uint8*)"");
        break;
            
    }
    
}  
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note://<内部处理>构建&设置相关属性
/****************************************************************************/ 
LOCAL void FeedParser(XML_USERDATA* userdata, const char *el, const char **attr)
{
    if(PNULL == userdata)
    {
        return;
    }

    switch(userdata->cur_element_id)
    {
    case DCDXML_ELEMENT_ID_FEED:
			FeedSetAttribute(  userdata->cur_feed,attr  );
        	break;

    case DCDXML_ELEMENT_ID_TITLE:
        	break;  
            
    case DCDXML_ELEMENT_ID_SUMMARY:	
        	break; 
            
    case DCDXML_ELEMENT_ID_LINK:		
			LinkSetAttribute(  &(userdata->cur_feed->feed_link),attr  );
        	break;  
            
    case DCDXML_ELEMENT_ID_CONTENT:
		    {
			    DCD_CONTENT_T * node_ptr = ContentFrameMalloc();							
			    ContentSetAttribute(  node_ptr,attr  );

			    ContentAdd2List(  &(userdata->cur_feed->content_ptr), node_ptr  );			
		    }			
        	break;    			

    case DCDXML_ELEMENT_ID_MAX_ID:
	        //xml is a general xml,but is invalid to DCD.
	        s_xml_valid = FALSE;
	        //SCI_TRACE_LOW:"DCD XML INVALID  MAX ID FeedParser "
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1242_112_2_18_2_13_9_314,(uint8*)"");
            break;
			
    default:
	        //xml is a general xml,but is invalid to DCD.
	        s_xml_valid = FALSE;
	        //SCI_TRACE_LOW:"DCD XML INVALID  FeedParser"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1248_112_2_18_2_13_9_315,(uint8*)"");
            break;
            
    }
	
} 

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void EntryParser(XML_USERDATA* userdata, const char *el, const char **attr)
{
    //<内部处理>构建&设置相关属性
    if(PNULL == userdata)
    {
        return ;
    }
    
    switch(userdata->cur_element_id)
    {
    case DCDXML_ELEMENT_ID_ENTRY:
			EntrySetAttribute(  userdata->cur_entry,attr  );
        	break;   			
                    
    case DCDXML_ELEMENT_ID_TITLE:
		break;

    case DCDXML_ELEMENT_ID_SUMMARY:
		break;

    case DCDXML_ELEMENT_ID_LINK:		
        LinkSetAttribute(  &(userdata->cur_entry->entry_link),attr  );
		break;

    case DCDXML_ELEMENT_ID_CONTENT:
		{
			DCD_CONTENT_T * node_ptr = ContentFrameMalloc();							
			ContentSetAttribute(  node_ptr,attr  );

			ContentAdd2List(  &(userdata->cur_entry->content_ptr), node_ptr  );			
		}
		break;

    case DCDXML_ELEMENT_ID_ISSUED:
    case DCDXML_ELEMENT_ID_ACTIVATION:
    case DCDXML_ELEMENT_ID_EXPIRATION:
    case DCDXML_ELEMENT_ID_AUTHOR:
    case DCDXML_ELEMENT_ID_NAME:	
    case DCDXML_ELEMENT_ID_URL:
    case DCDXML_ELEMENT_ID_EMAIL:
		break;
        
    case DCDXML_ELEMENT_ID_MAX_ID:
        //xml is a general xml,but is invalid to DCD.
        s_xml_valid = FALSE;
        //SCI_TRACE_LOW:"DCD XML INVALID  MAX ID EntryParser "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1304_112_2_18_2_13_10_316,(uint8*)"");
        break;
			
    default:
        //xml is a general xml,but is invalid to DCD.
        s_xml_valid = FALSE;
        //SCI_TRACE_LOW:"DCD XML INVALID  EntryParser"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1310_112_2_18_2_13_10_317,(uint8*)"");
        break;
            
    }
    
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void XMLCALL CharacterDataHandler(void *data,const XML_Char *s,int len)
{
    XML_USERDATA* userdata = (XML_USERDATA*) data;
    
    if(PNULL != userdata)// 加上对传入参数的检查...
    {
        //对得到的char数据进行取舍，转化，保存。。。
        switch(userdata->cur_element_id)
        {
          case DCDXML_ELEMENT_ID_GENERATOR:			
          case DCDXML_ELEMENT_ID_TITLE:					
          case DCDXML_ELEMENT_ID_SUMMARY:			
          case DCDXML_ELEMENT_ID_ISSUED:			
          case DCDXML_ELEMENT_ID_NAME:
            if( len > 0 )//有数据，则需要处理，否则不处理
            {
                uint8*  ucs2_buf_ptr = NULL;
                uint16  ucs2_str_size = 0;
                uint16  ucs2_str_len = 0;	
                
                ucs2_str_size = (len+1)*3;
                ucs2_buf_ptr = (uint8*)MMIDCD_ALLOC(  ucs2_str_size  );	
                
                //  非直接处理UTF-8的应用，需要转换编码格式
                ucs2_str_len = UTF82UCS2B((uint8*)ucs2_buf_ptr,ucs2_str_size,(uint8*)s,len);
                SetCharData(userdata,ucs2_buf_ptr,ucs2_str_len);
                
                //MMIDCD_FFS_fwrite( s,len, 1,userdata->parsed_stream );
                
                MMIDCD_FreeMem(ucs2_buf_ptr);	
            }	
            break;	
            
          case DCDXML_ELEMENT_ID_MAX_ID:
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;
            //SCI_TRACE_LOW:"DCD XML INVALID  MAX ID MetaParser "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1356_112_2_18_2_13_10_318,(uint8*)"");
            break;
            
          default:
            break;							
        }
    }
    
    return;
}  
/****************************************************************************/
//  Description: StartElementHandler
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void XMLCALL StartElementHandler(
                                        void *data,
                                        const XML_Char *name,
                                        const XML_Char **atts
                                      )
{
    XML_USERDATA* userdata = (XML_USERDATA*) data;
    
    //对状态变量进行设置！！！
    StatusInit( userdata,GetElementID(name) );
    
    //构建相关数据结构
    //设置相关属性
    switch(userdata->parser_status)
    {
        case DCDXML_INIT:
        case DCDXML_START:
            break;
			
        case DCDXML_META_START:
            MetaParser(userdata,name,atts);
            break;
			
        case DCDXML_FEED_START:
            FeedParser(userdata,name,atts);
            break;
			
        case DCDXML_ENTRY_START:
            EntryParser(userdata,name,atts);
            break;
        
        default:
            //xml is a general xml,but is invalid to DCD.
            s_xml_valid = FALSE;			
            //SCI_TRACE_LOW:"DCD XML INVALID  parser_status = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1404_112_2_18_2_13_10_319,(uint8*)"d",userdata->parser_status);
            break;									
    }
										 
}

/****************************************************************************/
//  Description: StartElementHandler
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
LOCAL void XMLCALL EndElementHandler(void *data, const XML_Char *name)
{
	XML_USERDATA* userdata = (XML_USERDATA*) data; 

    MMIDCDXML_ELEMENT_ID_E end_element_id = GetElementID(name);
	//EndStatus( userdata,GetElementID(name) );
    if(PNULL == userdata)
    {
        return;
    }
    
    userdata->cur_element_id = DCDXML_ELEMENT_ID_UNUSE;//end_element_id;//??
	
	switch(end_element_id)
	{
		case DCDXML_ELEMENT_ID_SYNC_PKG:
			userdata->parser_status = DCDXML_END;
			userdata->cur_meta  = NULL;
			userdata->cur_feed  = NULL;			
			userdata->cur_entry = NULL;			
			break;
		case DCDXML_ELEMENT_ID_META:
			userdata->parser_status = DCDXML_META_END;
			userdata->cur_meta  = NULL;
			userdata->cur_feed  = NULL;			
			userdata->cur_entry = NULL;			
			break;
			
		case DCDXML_ELEMENT_ID_FEED:
			userdata->parser_status = DCDXML_FEED_END;
			userdata->cur_meta  = NULL;
			userdata->cur_feed  = NULL;			
			userdata->cur_entry = NULL;		
			break;
		case DCDXML_ELEMENT_ID_ENTRY:
			userdata->parser_status = DCDXML_ENTRY_END;
			//cur_meta  = NULL;
			//cur_feed  = NULL;			
			userdata->cur_entry = NULL;			
			break;	
				
		default:
			break;							
	}
	
	userdata->depth -= 1;
}

/****************************************************************************/
//  Description: Parses a XML file
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIDCD_XmlParse(wchar  *  xml_file_full_path, 
                               BOOLEAN   is_prefile_parse  //是否是前一个xml文件的解析...
                               )
{
	BOOLEAN           ret = TRUE;
	uint32            charsread = 0;
	XML_Parser        parser = PNULL;
	int               done = 0;
	char              buf[BUFSIZ]={0};    
	MMIFILE_HANDLE    file_handle          = 0;
	SFS_ERROR_E       ffs_error  = SFS_NO_ERROR;
	
	//SCI_TRACE_LOW:"MMIDCD_XmlParse() enter"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1478_112_2_18_2_13_10_320,(uint8*)"");
	if( PNULL == xml_file_full_path)
    {
        return FALSE;
    }
	
	s_xml_valid = TRUE;
	
	MMIDCD_XMLDataFree(is_prefile_parse);//init&CLEAR myuserdata struct
	
	//open xml file
    file_handle = MMIAPIFMM_CreateFile(xml_file_full_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
	if (SFS_INVALID_HANDLE == file_handle)		
	{
		//SCI_TRACE_LOW:"MMIDCD_XmlParse(): parsed_stream fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1492_112_2_18_2_13_10_321,(uint8*)"");
		//SFS_CloseFile(xml_file_stream); 		
        MMIAPIFMM_CloseFile(file_handle);
		return FALSE;
	}
	
	charsread = MMIAPIFMM_GetFileSize(file_handle);
	if(0 == charsread)
	{
		//SCI_TRACE_LOW:"MMIDCD_XmlParse(): parsed_stream fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1501_112_2_18_2_13_10_322,(uint8*)"");
		//SFS_CloseFile(xml_file_stream); 
        MMIAPIFMM_CloseFile(file_handle);
		return FALSE;
	}
	
	parser = XML_ParserCreate_MM(NULL, &memsuite, NULL);
	if (!parser) 
	{
		//SCI_TRACE_LOW:"MMIDCD_XmlParse(): create xml parser fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1510_112_2_18_2_13_10_323,(uint8*)"");
		//SFS_CloseFile(xml_file_stream);
        MMIAPIFMM_CloseFile(file_handle);
		return FALSE;
	}
	
	/* Call this fun to set something data If you wanna set user_data. */
	XML_SetUserData(parser, MMIDCD_GetXMLData(is_prefile_parse));
	
	/* If the callback can provide information about the encoding, it must
	fill in the XML_Encoding structure, and return XML_STATUS_OK.
	Otherwise it must return XML_STATUS_ERROR.*/
	XML_SetUnknownEncodingHandler(parser,EncodingHandle,0);
	
	/* set start element handler and end element handler fun */
	XML_SetElementHandler(parser,StartElementHandler,EndElementHandler);
	
	/*  if you want to check some string that will calling  CharacterDataHandler */
	XML_SetCharacterDataHandler(parser,CharacterDataHandler);
	
	
	/* Start parse XML file*/
	do
	{
		SCI_MEMSET(buf, 0, sizeof(buf));
		//ffs_error = SFS_ReadFile(xml_file_stream,buf,sizeof(buf),&charsread,PNULL);
        ffs_error = MMIAPIFMM_ReadFile(file_handle, buf, sizeof(buf), &charsread, PNULL);
		if(  (0 < charsread) && (SFS_NO_ERROR == ffs_error)  )
		{
			done = charsread < sizeof(buf);
			if( XML_STATUS_ERROR == XML_Parse(parser, buf, charsread, done) )
			{
				ret = FALSE;
				break;
			}
		}
		
	} while( ret && (0 < charsread) );
	
	XML_ParserFree(parser);
	MMIAPIFMM_CloseFile(file_handle);
	
	return (ret&&s_xml_valid);
}


/****************************************************************************/
//  Description: Parses a static XML file
//  Global resource dependence
//  Auther: zhoufeng
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN DCDXML_ParseSysFile(const char* xml_ptr,uint32 len)
{
    BOOLEAN           ret = TRUE;
    uint32            charsremain = 0;    
    uint32            charsread = 0;
    const char*            readptr = PNULL;    
    XML_Parser        parser = PNULL;
    int               done = 0;
    char              buf[BUFSIZ];    
    
    //SCI_TRACE_LOW:"DCDXML_ParseStaticFile() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1570_112_2_18_2_13_10_324,(uint8*)"");
    if( PNULL == xml_ptr)
    {
        return FALSE;
    }
    
    charsremain = len;
    readptr = xml_ptr;
    
    MMIDCD_XMLDataFree(FALSE);
    
    parser = XML_ParserCreate_MM(NULL, &memsuite, NULL);
    if (!parser) 
    {
        //SCI_TRACE_LOW:"create xml parser fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1584_112_2_18_2_13_10_325,(uint8*)"");
        return FALSE;
    }
    
    
    //===
    // 注册回调函数
    //  设置用户数据，如果需要传给处理函数附加的信息，那就使用它，不然就不用
    XML_SetUserData(parser,&g_user_data);
    
    //  如果是utf-8 us-ascii ucs2 iso8859-1 那不用使用下面这个函数，
    //  不然要使用，不使用会报字符集不支持的错误,
    //  读xml头部时会调用到<?xml version="1.0" encoding="gb2312" ?>
    
    XML_SetUnknownEncodingHandler(parser,EncodingHandle,0);
    
    //  设置事件处理函数,当碰到开始元素和结可元素时调用哪个函数来处理
    XML_SetElementHandler(parser,StartElementHandler,EndElementHandler);
    
    
    
    //  设置当碰到数据时的处理函数
    //  如<img> "hello.gif"</img> 那么碰到img时会调用startElementHandler,
    //  读完"hello.gif"后会调用characterDataHandler,碰到/img后会调用endElementHandler
    XML_SetCharacterDataHandler(parser,CharacterDataHandler);
    
    
    // 开始解析
    do
    {
        // 同步读文件
        SCI_MEMSET(buf, 0, BUFSIZ);
        //ffs_error = SFS_ReadFile(xml_file_stream,buf,sizeof(buf),&charsread,PNULL);
        readptr += charsread;
        if(charsremain > BUFSIZ)
        {
            charsread = BUFSIZ;
            charsremain -= charsread;
            SCI_MEMCPY(buf, readptr, charsread);
        }
        else
        {
            charsread = charsremain;
            charsremain -= charsread;
            SCI_MEMCPY(buf, readptr, charsread);
        }
        
        if(  (0 < charsread))
        {
            done = charsread < sizeof(buf);
            if( XML_STATUS_ERROR == XML_Parse(parser, buf, charsread, done) )
            {
                //SCI_TRACE_LOW:"DCDXML_ParseStaticFile():Parse error at line %dn %sn"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1638_112_2_18_2_13_10_326,(uint8*)"ds",XML_GetCurrentLineNumber(parser),XML_ErrorString(XML_GetErrorCode(parser)));
                ret = FALSE;
                break;
            }
        }
    } while( ret && (0 < charsread) );
    //===
    
    XML_ParserFree(parser);        
    
    return ret;
}
 
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_LINK_REL_E GetLinkRel(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_LINK_REL_ALTERNATE_TXT))    return DCD_LINK_REL_ALTERNATE;
        //SCI_TRACE_LOW:"GetLinkRel() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1660_112_2_18_2_13_10_327,(uint8*)"s",str);
    }
    return DCD_LINK_REL_MAX;
}  
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_LINK_TYPE_E GetLinkType(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_LINK_TYPE_APPLICAION_TXT))    return DCD_LINK_TYPE_APPLICAION;
        if(!stricmp(str,DCD_LINK_TYPE_TEXT_TXT))          return DCD_LINK_TYPE_TEXT;
        if(!stricmp(str,DCD_LINK_TYPE_PLAIN_TEXT_TXT))          return DCD_LINK_TYPE_TEXT;
        
        //SCI_TRACE_LOW:"GetLinkType() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1676_112_2_18_2_13_10_328,(uint8*)"s",str);
    }
    return DCD_LINK_TYPE_MAX;
} 
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_CONTENT_MODE_E GetContentMode(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_CONTENT_MODE_BINARY_TXT))    return DCD_CONTENT_MODE_BINARY;
        
        //SCI_TRACE_LOW:"GetContentMode() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1690_112_2_18_2_13_10_329,(uint8*)"s",str);
    }  
    return DCD_CONTENT_MODE_MAX;
} 
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_CONTENT_TYPE_E GetContentType(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_CONTENT_TYPE_GIF_TXT))    return DCD_CONTENT_TYPE_GIF;
        if(!stricmp(str,DCD_CONTENT_TYPE_JPEG_TXT))        return DCD_CONTENT_TYPE_JPEG;
        
        //SCI_TRACE_LOW:"GetContentType() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1705_112_2_18_2_13_10_330,(uint8*)"s",str);
    }  
    return DCD_CONTENT_TYPE_MAX;
} 
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_CONTENT_NAME_E GetContentName(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_CONTENT_NAME_IMAGE_TXT))    return DCD_CONTENT_NAME_IMAGE;
        
        //SCI_TRACE_LOW:"GetContentName() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1719_112_2_18_2_13_10_331,(uint8*)"s",str);
    }
    return DCD_CONTENT_NAME_MAX;
} 
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_TEMPLATE_ID_E GetTemplateId(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_TEMPLATE_ID_DEFAULT_TXT))    return DCD_TEMPLATE_ID_DEFAULT;
        
        //SCI_TRACE_LOW:"GetTemplateId() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1733_112_2_18_2_13_10_332,(uint8*)"s",str);
    }  
    return DCD_TEMPLATE_ID_MAX;	
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_ENTRY_TYPE_E GetEntryType(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_ENTRY_TYPE_CS_TXT))    return DCD_ENTRY_TYPE_CS;
        if(!stricmp(str,DCD_ENTRY_TYPE_CSTO_TXT))    return DCD_ENTRY_TYPE_CS;
        if(!stricmp(str, DCD_ENTRY_TYPE_ETTI_TXT))    return DCD_ENTRY_TYPE_ETTI;
        if(!stricmp(str, DCD_ENTRY_TYPE_ETTO_TXT))    return DCD_ENTRY_TYPE_ETTO;
        if(!stricmp(str, DCD_ENTRY_TYPE_ETIO_TXT))   return DCD_ENTRY_TYPE_ETIO;
        if(!stricmp(str, DCD_ENTRY_TYPE_ETMM_TXT))   return DCD_ENTRY_TYPE_ETMM;
        
        //SCI_TRACE_LOW:"GetEntryType() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1752_112_2_18_2_13_10_333,(uint8*)"s",str);
    }  
    return DCD_ENTRY_TYPE_MAX;		
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_FEED_TYPE_E GetFeedType(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_FEED_TYPE_FTTI_TXT))    return DCD_FEED_TYPE_FTTI;
        if(!stricmp(str,DCD_FEED_TYPE_FTTO_TXT))    return DCD_FEED_TYPE_FTTO;
        if(!stricmp(str, DCD_FEED_TYPE_FTIO_TXT))    return DCD_FEED_TYPE_FTIO;
        if(!stricmp(str, DCD_FEED_TYPE_FTMM_TXT))    return DCD_FEED_TYPE_FTMM;
        
        //SCI_TRACE_LOW:"GetFeedType() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1769_112_2_18_2_13_10_334,(uint8*)"s",str);
    }  
    return DCD_FEED_TYPE_MAX;		
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_CHARGE_TYPE_E GetMetaFeedCharge(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_METAFEED_CHARGE_Y_TXT))    return DCD_CHARGE_TYPE_Y;
        if(!stricmp(str,DCD_METAFEED_CHARGE_N_TXT))    return DCD_CHARGE_TYPE_N;
        if(!stricmp(str, DCD_METAFEED_CHARGE_P_TXT))    return DCD_CHARGE_TYPE_P;
        
        //SCI_TRACE_LOW:"GetFeedType() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1785_112_2_18_2_13_11_335,(uint8*)"s",str);
    }
    return DCD_CHARGE_TYPE_MAX;		
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL DCD_TIME_TYPE_E GetTimeType(const char *str)
{
    if(PNULL != str)
    {
        if(!stricmp(str,DCD_TIME_TYPE_REL_TXT))    return DCD_TIME_TYPE_REL;
        if(!stricmp(str,DCD_TIME_TYPE_ABSOLUTE_TXT))    return DCD_TIME_TYPE_ABSOLUTE;
        
        //SCI_TRACE_LOW:"GetTimeType() str='%s'	"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_1800_112_2_18_2_13_11_336,(uint8*)"s",str);
    }  
    return DCD_TIME_TYPE_MAX;		
}

/*****************************************************************************/
//  Description : convert str to second
//  Global resource dependence : 
//  Author:
//  Note:	
///*****************************************************************************/
LOCAL uint32 StrToSecond(const char *str_ptr) 
{
    char *cur_str = PNULL;
    uint8 i = 0;
    uint32 second = 0;
    uint8 str_len = 0;

    if(PNULL == str_ptr)
    {
        return 0;
    }

    cur_str = (char*)str_ptr;
    str_len = strlen(str_ptr);
    for(; i < (str_len - DCD_MICROSECOND_LEN);i++)
    {
	    second *= 0xa;
        second = (uint32)((int32)second + ((*cur_str) - '0'));

        cur_str ++;
    }   

    
    return second;	
} 
/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,which's in the struct of DCD_FEED_META_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFeedMetaAnchor(DCD_FEED_META_T * feed_meta_ptr,
                        const char *auchor_ptr,
                        uint16 auchor_len
                        )
{
   
    uint16  buff_size = ( auchor_len+1);
    char* buff_ptr   = PNULL;
    
       
    if(PNULL == feed_meta_ptr || PNULL == auchor_ptr || auchor_len > DCD_MAX_FEEDMETA_ANCHOR_LEN)
    {
        return;
    }
  
    //if feed_meta_anchor_ptr exist ,free it!
    if(feed_meta_ptr->feed_meta_anchor_ptr)
    {
        FeedMetaAnchorFree(feed_meta_ptr);
    }
    
    if(0 == auchor_len)
    {
        return;
    }
           
    buff_ptr = (char*)MMIDCD_ALLOC(buff_size);    
    SCI_MEMSET(buff_ptr,0,buff_size);
    strcpy(buff_ptr,auchor_ptr);
    feed_meta_ptr->feed_meta_anchor_ptr = buff_ptr;    
}

/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_FEED_META_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FeedMetaAnchorFree(DCD_FEED_META_T * feed_meta_ptr)
{
    if(PNULL == feed_meta_ptr || PNULL == feed_meta_ptr->feed_meta_anchor_ptr)
    {
        return;
    }
        	
    MMIDCD_FreeMem(feed_meta_ptr->feed_meta_anchor_ptr);
    feed_meta_ptr->feed_meta_anchor_ptr = PNULL;  
}

/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,
//                  which's in the struct of DCD_GLOBAL_META_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetGlobalMetaAnchor(DCD_GLOBAL_META_T * global_meta_ptr,
                                const char *auchor_ptr, 
                                uint16 auchor_len
                                )
{

    uint16  buff_size = ( auchor_len+1);
    char* buff_ptr   = PNULL;

           
    if(PNULL == global_meta_ptr || PNULL == auchor_ptr || auchor_len > DCD_MAX_GLOBALMETA_ANCHOR_LEN)
    {
        return;
    }

    //if global_meta_anchor_ptr exist ,free it!
    if(global_meta_ptr->global_meta_anchor_ptr)
    {
        GlobalMetaAnchorFree(global_meta_ptr);
    }

    if(0 == auchor_len)
    {
        return;       
    }
         
    buff_ptr = (char*)MMIDCD_ALLOC(buff_size);    
    SCI_MEMSET(buff_ptr,0,buff_size);
    strcpy(buff_ptr,auchor_ptr);
    global_meta_ptr->global_meta_anchor_ptr = buff_ptr;            
}


/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_GLOBAL_META_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void GlobalMetaAnchorFree(DCD_GLOBAL_META_T * global_meta_ptr)
{
    if(PNULL == global_meta_ptr || PNULL == global_meta_ptr->global_meta_anchor_ptr)
    {
        return;
    }
        	
    MMIDCD_FreeMem(global_meta_ptr->global_meta_anchor_ptr);
    global_meta_ptr->global_meta_anchor_ptr = PNULL;  
}

/*****************************************************************************/
//  Description :   allocate memory of  the certain size for href ,
//                  which's in the struct of DCD_LINK_T
//  Global resource dependence :none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetLinkHref(DCD_LINK_T * link_ptr,const char *href_ptr, uint16 href_len)
{
    uint16  buff_size = ( href_len+1);
    char* buff_ptr   = PNULL;
           
    if(PNULL == link_ptr || PNULL == href_ptr || href_len > DCD_MAX_HREF_LEN)
    {
        return;
    }

    //if href_ptr exist ,free it!

    if(PNULL != link_ptr->href_ptr)
    {
        LinkHrefFree(link_ptr);
    }

    if(0 == href_len)
    {
        return;
    }

    buff_ptr = (char*)MMIDCD_ALLOC(buff_size);    
    SCI_MEMSET(buff_ptr,0,buff_size);   
    strcpy(buff_ptr,href_ptr);
    link_ptr->href_ptr = buff_ptr;                  
}

/*****************************************************************************/
//  Description :   free memory of  href ,which's in the struct of DCD_LINK_T
//  Global resource dependence :none
//  Author:
//  Note: free memory of  href ,which's in the struct of DCD_LINK_T
/*****************************************************************************/
LOCAL void LinkHrefFree(DCD_LINK_T * link_ptr)
{
    if(PNULL == link_ptr || PNULL == link_ptr->href_ptr)
    {
        return;
    }

    MMIDCD_FreeMem(link_ptr->href_ptr);
    link_ptr->href_ptr = PNULL;
}
/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,which's in the struct of DCD_FEED_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFeedHref(DCD_FEED_T * feed_ptr,const char *href_ptr, uint16 href_len)
{
    uint16  buff_size = ( href_len+1);
    char* buff_ptr   = PNULL;
           
    if(PNULL == feed_ptr)
    {
        return;
    }

    //if entry_template_href_ptr exist ,free it!
    if(PNULL != feed_ptr->feed_template_href_ptr)
    {
        FeedHrefFree(feed_ptr);
    }

    if(PNULL == href_ptr || 0 == href_len || href_len > DCD_MAX_HREF_LEN)
    {
        return;
    }
    
    buff_ptr = (char*)MMIDCD_ALLOC(buff_size);    
    SCI_MEMSET(buff_ptr,0,buff_size); 
    strcpy(buff_ptr,href_ptr);
    feed_ptr->feed_template_href_ptr = buff_ptr;       
}
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/ 
LOCAL void FeedHrefFree(DCD_FEED_T * feed_ptr)
{
    if(PNULL == feed_ptr || PNULL == feed_ptr->feed_template_href_ptr)
    {
        return;
    }
        	
    MMIDCD_FreeMem(feed_ptr->feed_template_href_ptr);
    feed_ptr->feed_template_href_ptr = PNULL;  
}
/*****************************************************************************/
//  Description :  allocate memory of  the certain size for str ,which's in the struct of DCD_UCS2_STR_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CopyString(DCD_UCS2_STR_T *ucs2_ptr,uint8 *data_ptr, uint16 data_len)
{
    uint16  buff_size = 0;
    uint8*  buff_ptr   = PNULL;
  
    uint16  old_ucs2_str_len = 0;
    uint8*  old_ucs2_str_ptr = PNULL;    

    if(PNULL == ucs2_ptr || PNULL == data_ptr || 0 == data_len)
    {
        return;
    }
       
	if(PNULL != ucs2_ptr->str_ptr) 
	{
		old_ucs2_str_len = ucs2_ptr->str_len;
		old_ucs2_str_ptr = ucs2_ptr->str_ptr;
	}
    else
    {
		old_ucs2_str_len = 0;
		old_ucs2_str_ptr = PNULL;    	
    } 
       

    buff_size = old_ucs2_str_len + data_len + 2;   
               
    buff_ptr = (uint8*)MMIDCD_ALLOC(buff_size);    
    SCI_MEMSET(buff_ptr,0,buff_size); 
    
    if(PNULL != old_ucs2_str_ptr)
    {
        MMI_MEMCPY(buff_ptr, buff_size, old_ucs2_str_ptr,old_ucs2_str_len, old_ucs2_str_len);
    }

    MMI_MEMCPY(buff_ptr + old_ucs2_str_len, buff_size - old_ucs2_str_len, data_ptr, data_len, data_len);

    //free old str_ptr
    if(PNULL != ucs2_ptr->str_ptr)
    {
        StringFree(ucs2_ptr);
    }
	    
    ucs2_ptr->str_ptr = buff_ptr;
    ucs2_ptr->str_len = old_ucs2_str_len + data_len;  
              
}
/**************************************************************************/
//  Description :  free memory of  str ,which's in the struct of DCD_UCS2_STR_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StringFree(DCD_UCS2_STR_T *ucs2_ptr)
{
    if (PNULL != ucs2_ptr && NULL != ucs2_ptr->str_ptr)
    {
        MMIDCD_FreeMem(ucs2_ptr->str_ptr);
        ucs2_ptr->str_ptr = PNULL;

        ucs2_ptr-> str_len = 0;
    }
}
/*****************************************************************************/
//  Description :   free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetTitle(DCD_TITLE_T * title_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != title_ptr && PNULL != data_ptr)
    {
        CopyString(&(title_ptr->ucs2_str),data_ptr,data_len);
    }	
}
/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void TitleFree(DCD_TITLE_T * title_ptr)
{
    if(PNULL != title_ptr)
    {
        StringFree(&(title_ptr->ucs2_str));
    }
}
/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetSummary(DCD_SUMMARY_T * summary_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != summary_ptr && PNULL != data_ptr)
    {
        CopyString(&(summary_ptr->ucs2_str),data_ptr,data_len);	
    }	
}
/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SummaryFree(DCD_SUMMARY_T * summary_ptr)
{
    if(PNULL != summary_ptr)
    {
        StringFree(&(summary_ptr->ucs2_str));
    }
}

/*****************************************************************************/
//  Description :  set feed title
//  Global resource dependence : the feed exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFeedTitle(DCD_FEED_T * feed_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != feed_ptr)
    {
        SetTitle(&(feed_ptr -> feed_title),data_ptr,data_len);
    }	
}

/*****************************************************************************/
//  Description :  set entry title
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryTitle(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != entry_ptr)
    {
        SetTitle(&(entry_ptr -> entry_title),data_ptr,data_len);
    }	
}

/*****************************************************************************/
//  Description :  set feed summary
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetFeedSummary(DCD_FEED_T * feed_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != feed_ptr)
    {
        SetSummary(&(feed_ptr ->feed_summary),data_ptr,data_len);
    }		
}

/*****************************************************************************/
//  Description :  set entry summary
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntrySummary(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != entry_ptr)
    {
        SetSummary(&(entry_ptr -> entry_summary),data_ptr,data_len);
    }	
}

/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetContentHref(DCD_CONTENT_T * content_ptr,const char *href_ptr, uint16 href_len)
{
    uint16  buff_size = ( href_len+1);
    char* buff_ptr   = PNULL;
           
    if(PNULL == content_ptr || 0 == href_len || href_len > DCD_MAX_HREF_LEN)
    {
        return;
    }

    //if href_ptr exist ,free it!
    if(content_ptr->href_ptr)
    {
        ContentHrefFree(content_ptr);
    }

               
    buff_ptr = (char*)MMIDCD_ALLOC(buff_size);    
    SCI_MEMSET(buff_ptr,0,buff_size); 
    strcpy(buff_ptr,href_ptr);
    content_ptr->href_ptr = buff_ptr;         
}

/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ContentHrefFree(DCD_CONTENT_T * content_ptr)
{
    if(PNULL == content_ptr || PNULL == content_ptr->href_ptr)
        return;
        	
    MMIDCD_FreeMem(content_ptr->href_ptr);
    content_ptr->href_ptr = PNULL;  
}

/*****************************************************************************/
//  Description :  allocate memory of  the certain size for href ,which's in the struct of DCD_ENTRY_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryHref(DCD_ENTRY_T * entry_ptr,const char *href_ptr, uint16 href_len)
{           
    uint16  buff_size = ( href_len+1);
    char* buff_ptr   = PNULL;
           
    if(PNULL == entry_ptr || PNULL == href_ptr || href_len > DCD_MAX_HREF_LEN)
    {
        return;
    }

    //if entry_template_href_ptr exist ,free it!
    if(entry_ptr->entry_template_href_ptr)
    {
        EntryHrefFree(entry_ptr);
    }

    if(!href_len)
       return;
               
    buff_ptr = (char*)MMIDCD_ALLOC(buff_size);    
    SCI_MEMSET(buff_ptr,0,buff_size);  
    strcpy(buff_ptr,href_ptr);
    entry_ptr->entry_template_href_ptr = buff_ptr;         
}

/*****************************************************************************/
//  Description :  free memory of  href ,which's in the struct of DCD_ENTRY_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EntryHrefFree(DCD_ENTRY_T * entry_ptr)
{
    if(PNULL == entry_ptr || PNULL == entry_ptr->entry_template_href_ptr)
        return;
        	
    MMIDCD_FreeMem(entry_ptr->entry_template_href_ptr);
    entry_ptr->entry_template_href_ptr = PNULL;  
}



/*****************************************************************************/
//  Description :  set the attributes for global_meta
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GlobalMetaSetAttribute(DCD_GLOBAL_META_T  *global_meta_ptr, 
                                     const char **attr_pptr
                                     )
{
    BOOLEAN ret = TRUE;
    uint16  i   = 0;
    uint16 attr_len = 0;
    MMIDCDXML_ATTRIBUTE_ID_E attr_id = DCDXML_ATTRIBUTE_ID_MAX;

    if(PNULL == global_meta_ptr)
    {
        return FALSE;
    }
    
    
    for(i=0; PNULL != attr_pptr[i]; i+=2)
    {
        attr_id = GetAttributeID(attr_pptr[i]);        
        switch(attr_id)
        {
            case DCDXML_ATTRIBUTE_ID_ANCHOR:
                attr_len = strlen(attr_pptr[i + 1]);
                SetGlobalMetaAnchor(global_meta_ptr,attr_pptr[i + 1],attr_len);
                break;

            case DCDXML_ATTRIBUTE_ID_TTL:
                global_meta_ptr->global_meta_ttl = StrToSecond(attr_pptr[i + 1]); 
                break;            

            case DCDXML_ATTRIBUTE_ID_SERVICE_TIME:
                global_meta_ptr->global_meta_server_time = StrToSecond(attr_pptr[i + 1]);                
                break;   
                            
            default:
                ret = FALSE;
                break;                                                 
        }
    }
    return   ret;
}



/*****************************************************************************/
//  Description :  set the attributes for feed_meta
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN FeedMetaSetAttribute(DCD_FEED_META_T  *feed_meta_ptr, 
                                   const char **attr_pptr
                                   )
{    
    BOOLEAN ret = TRUE;
    uint16 i    = 0;
    uint16 attr_len = 0;
    MMIDCDXML_ATTRIBUTE_ID_E attr_id = DCDXML_ATTRIBUTE_ID_MAX;

    if(PNULL == feed_meta_ptr)
    {
        return FALSE;
    }
    
    for(i=0; PNULL != attr_pptr[i]; i+=2)
    {
        attr_id = GetAttributeID(attr_pptr[i]);
        switch(attr_id)
        {
            case DCDXML_ATTRIBUTE_ID_CTXT_ID:
                strcpy(feed_meta_ptr->feed_meta_ctxt_id,attr_pptr[i + 1]);
                break;

            case DCDXML_ATTRIBUTE_ID_ANCHOR:
                attr_len = strlen(attr_pptr[i + 1]);
                SetFeedMetaAnchor(feed_meta_ptr,attr_pptr[i + 1],attr_len);
                break;

            case DCDXML_ATTRIBUTE_ID_TARGET:
                strcpy(feed_meta_ptr->feed_meta_target,attr_pptr[i + 1]);                
                break;            
            
            case DCDXML_ATTRIBUTE_ID_CHARGE:
                feed_meta_ptr->feed_meta_charge = GetMetaFeedCharge(attr_pptr[i + 1]);
                break;   
				
            default:
                ret = FALSE;
                break;                                                 
        }
    }
    return   ret;
}



/*****************************************************************************/
//  Description :  set the attributes for meta_generator
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MetaGeneratorSetAttribute(DCD_META_GENERATOR_T  *meta_generator_ptr, 
                                        const char **attr_pptr
                                        )
{    
    BOOLEAN ret = TRUE;
    uint16 i    = 0;
    MMIDCDXML_ATTRIBUTE_ID_E attr_id = DCDXML_ATTRIBUTE_ID_MAX;

    if(PNULL == meta_generator_ptr)
    {
        return FALSE;
    }
    
    for(i=0; PNULL != attr_pptr[i]; i+=2)
    {
        attr_id = GetAttributeID(attr_pptr[i]);
        switch(attr_id)
        {
            case DCDXML_ATTRIBUTE_ID_URL:
                strcpy(meta_generator_ptr->generator_url,attr_pptr[i + 1]);
                break;

            case DCDXML_ATTRIBUTE_ID_VERSION:
                strcpy(meta_generator_ptr->generator_version,attr_pptr[i + 1]);
                break;
           
            
            default:
                ret = FALSE;
                break;                                                  
        }
    }
    return   ret;
}
 
/*****************************************************************************/
//  Description : set the attributes for link
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN LinkSetAttribute(DCD_LINK_T  *link_ptr,const char **attr_pptr)
{   
    BOOLEAN ret = TRUE;
    uint16 i    = 0;
    uint16 attr_len = 0;
    MMIDCDXML_ATTRIBUTE_ID_E attr_id = DCDXML_ATTRIBUTE_ID_MAX;

    if(PNULL == link_ptr)
    {
        return FALSE;
    }
    
    for(i=0; PNULL != attr_pptr[i]; i+=2)
    {
        attr_id = GetAttributeID(attr_pptr[i]);
        switch(attr_id)
        {
            case DCDXML_ATTRIBUTE_ID_HREF:
                attr_len = strlen(attr_pptr[i + 1]);
                SetLinkHref(link_ptr,attr_pptr[i + 1],attr_len);
                break;

            case DCDXML_ATTRIBUTE_ID_TYPE:
                link_ptr -> link_rel = GetLinkType(attr_pptr[i + 1]);  /*lint !e64*/
                break;

            case DCDXML_ATTRIBUTE_ID_REL:
                link_ptr -> link_type = GetLinkRel(attr_pptr[i + 1]); /*lint !e64*/          
                break;            
            
            case DCDXML_ATTRIBUTE_ID_TITLE:                         
                break; 

            default:
                ret = FALSE;
                break;                                                 
        }
    }
    return   ret;    
}

/*****************************************************************************/
//  Description :  set the attributes for entry author
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN EntryAuthorSetAttribute(DCD_ENTRY_AUTHOR_T *entry_author_ptr, 
                                      const char **attr_pptr
                                      )
{
    BOOLEAN ret = TRUE;
    uint16 i    = 0;
    MMIDCDXML_ATTRIBUTE_ID_E attr_id = DCDXML_ATTRIBUTE_ID_MAX;

    if(PNULL == entry_author_ptr)
    {
        return FALSE;
    }
    
    for(i=0; PNULL != attr_pptr[i]; i+=2)
    {
        attr_id = GetAttributeID(attr_pptr[i]);
        switch(attr_id)
        {
            case DCDXML_ATTRIBUTE_ID_URL:
                strcpy(entry_author_ptr->author_url,attr_pptr[i + 1]);   
                break;

            case DCDXML_ATTRIBUTE_ID_EMAIL:
                strcpy(entry_author_ptr->author_email,attr_pptr[i + 1]);           
                break;            
            
            default:
                ret = FALSE;
                break;                                                 
        }
    }
    return   ret; 
}

/*****************************************************************************/
//  Description :  set the attributes for ontent
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN ContentSetAttribute(DCD_CONTENT_T * content_ptr, 
                                  const char **attr_pptr
                                  )
{
    BOOLEAN ret = TRUE;
    uint16 i = 0;
    uint16 attr_len = 0;
    MMIDCDXML_ATTRIBUTE_ID_E attr_id = DCDXML_ATTRIBUTE_ID_MAX;

    if(PNULL == content_ptr)
    {
        return FALSE;
    }
    
    for(i=0; PNULL != attr_pptr[i]; i+=2)
    {
        attr_id = GetAttributeID(attr_pptr[i]);
        switch(attr_id)
        {
            case DCDXML_ATTRIBUTE_ID_TYPE:
                content_ptr->content_type = GetContentType(attr_pptr[i + 1]);    
                break;

            case DCDXML_ATTRIBUTE_ID_MODE:
                content_ptr->content_mode = GetContentMode(attr_pptr[i + 1]);   
                break;

            case DCDXML_ATTRIBUTE_ID_NAME:
                content_ptr->content_name = GetContentName(attr_pptr[i + 1]);            
                break; 
                                   

            case DCDXML_ATTRIBUTE_ID_CTXT_ID:
                strcpy(content_ptr->ctxt_id,attr_pptr[i + 1]);   
                break;

            case DCDXML_ATTRIBUTE_ID_HREF:  
                attr_len = strlen(attr_pptr[i + 1]); 
                SetContentHref(content_ptr,attr_pptr[i + 1],attr_len);                       
                break;                                      
                        
            default:
                ret = FALSE;
                break;                                                 
        }
    }
    return   ret;     
}

 
/*****************************************************************************/
//  Description :  set the attributes for entry
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN EntrySetAttribute(DCD_ENTRY_T* entry_ptr, const char **attr_pptr)
{
    BOOLEAN ret = TRUE;
    uint16  i  = 0 ;
    uint16 attr_len = 0;
    MMIDCDXML_ATTRIBUTE_ID_E attr_id = DCDXML_ATTRIBUTE_ID_MAX;
   
    if(PNULL == *attr_pptr || PNULL == entry_ptr)
    {
        return FALSE;
    }

    for(i=0; PNULL !=attr_pptr[i]; i+=2)
    {
        attr_id = GetAttributeID(attr_pptr[i]);
        switch(attr_id)
        {
            case DCDXML_ATTRIBUTE_ID_CTXT_ID:
                strcpy(entry_ptr->ctxt_id,attr_pptr[i + 1]);   
                break;

            case DCDXML_ATTRIBUTE_ID_ENTRY_TYPE:
                entry_ptr->entry_type = GetEntryType(attr_pptr[i + 1]);   
                break;

            case DCDXML_ATTRIBUTE_ID_TEMPLATE_ID:
                entry_ptr->template_id = GetTemplateId(attr_pptr[i + 1]);            
                break; 
                                   
            case DCDXML_ATTRIBUTE_ID_HREF:
                attr_len = strlen(attr_pptr[i + 1]); 
                SetEntryHref(entry_ptr,attr_pptr[i + 1],attr_len);
                break;                                  
                        
            case DCDXML_ATTRIBUTE_ID_TITLE:                     
                break; 
				
            default:
                ret = FALSE;
                break;                                                 
        }
    }
    return   ret;   
}

/*****************************************************************************/
//  Description :  set the attributes for feed
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FeedSetAttribute(DCD_FEED_T* feed_ptr, const char **attr_pptr)
{
    BOOLEAN ret = TRUE;
    uint16 i = 0;
    uint16 attr_len = 0;
    MMIDCDXML_ATTRIBUTE_ID_E attr_id = DCDXML_ATTRIBUTE_ID_MAX;
   
    if(PNULL == *attr_pptr || PNULL == feed_ptr)
    {
        return FALSE;
    }

    for(i=0; PNULL != attr_pptr[i]; i+=2)
    {
        attr_id = GetAttributeID(attr_pptr[i]);
        switch(attr_id)
        {
            case DCDXML_ATTRIBUTE_ID_VERSION:
                strcpy(feed_ptr->feed_version,attr_pptr[i + 1]);   
                break;

            case DCDXML_ATTRIBUTE_ID_CTXT_ID:
                strcpy(feed_ptr->ctxt_id,attr_pptr[i + 1]); 
                break;

            case DCDXML_ATTRIBUTE_ID_TEMPLATE_ID:
                feed_ptr->template_id = GetTemplateId(attr_pptr[i + 1]);            
                break; 
                                   
            case DCDXML_ATTRIBUTE_ID_HREF:
                attr_len = strlen(attr_pptr[i + 1]); 
                SetFeedHref(feed_ptr,attr_pptr[i + 1],attr_len);
                break;                                  
                        
            case DCDXML_ATTRIBUTE_ID_FEED_TYPE:
                feed_ptr ->feed_type = GetFeedType(attr_pptr[i + 1]);
                break;   
				
            case DCDXML_ATTRIBUTE_ID_TITLE:                  
                break; 				
						
            default:
                ret = FALSE;
                break;                                                 
        }
    }
    return   ret;   
}

/*****************************************************************************/
//  Description :  allocate memory of DCD_FEED_META_T
//  Author:
//  Note: 

/*****************************************************************************/
LOCAL DCD_FEED_META_T *FeedMetaFrameMalloc(void)
{
    DCD_FEED_META_T * feed_meta_ptr = (DCD_FEED_META_T *)MMIDCD_ALLOC(sizeof(DCD_FEED_META_T));    
    SCI_MEMSET( feed_meta_ptr,0,sizeof(DCD_FEED_META_T) );	

    return feed_meta_ptr;   	   
}
/*****************************************************************************/
//  Description :   free feed_meta 
//  Global resource dependence :the content exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FeedMetaFrameFree( DCD_FEED_META_T *feed_meta_ptr)
{
    if(NULL != feed_meta_ptr)  
    {
        FeedMetaAnchorFree(feed_meta_ptr);
        MMIDCD_FreeMem(feed_meta_ptr);    
    
    }	
}
/*****************************************************************************/
//  Description :  allocate memory of DCD_CONTENT_T
//  Global resource dependence :none
//  Author:
//  Note: allocate memory of DCD_CONTENT_T
/*****************************************************************************/
LOCAL DCD_CONTENT_T *ContentFrameMalloc(void)
{
    DCD_CONTENT_T * content_ptr = (DCD_CONTENT_T *)MMIDCD_ALLOC(sizeof(DCD_CONTENT_T));    
    SCI_MEMSET( content_ptr,0,sizeof(DCD_CONTENT_T) );	
    
    return content_ptr;   	   
}
/*****************************************************************************/
//  Description :  free content
//  Global resource dependence :the content exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ContentFrameFree( DCD_CONTENT_T *content_ptr)
{
    if(NULL != content_ptr)
    {
        ContentHrefFree(content_ptr);
        MMIDCD_FreeMem(content_ptr);    
    }
}
/*****************************************************************************/
//  Description :  allocate memory of DCD_ENTRY_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_ENTRY_T *EntryFrameMalloc(void)
{  
    DCD_ENTRY_T *entry_ptr = (DCD_ENTRY_T *)MMIDCD_ALLOC(sizeof(DCD_ENTRY_T));   
    SCI_MEMSET( entry_ptr,0,sizeof(DCD_ENTRY_T) );	
        
    return entry_ptr;  	 
}
/*****************************************************************************/
//  Description :  free a entry
//  Global resource dependence :the entry exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EntryFrameFree(DCD_ENTRY_T *entry_ptr)
{
    if(NULL != entry_ptr) 
    {
        TitleFree(&(entry_ptr->entry_title));
        SummaryFree(&(entry_ptr->entry_summary));
        LinkHrefFree(&(entry_ptr->entry_link));
        EntryAuthorNameFree(&(entry_ptr->entry_author));
        EntryHrefFree(entry_ptr);       
        if(entry_ptr->content_ptr)
        {
            ContentListFree(&(entry_ptr->content_ptr));
        } 

        MMIDCD_FreeMem(entry_ptr);    
    }
}
/*****************************************************************************/
//  Description :  allocate memory of DCD_FEED_T
//  Global resource dependence :none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL DCD_FEED_T *FeedFrameMalloc(void)
{
    DCD_FEED_T *feed_ptr = (DCD_FEED_T *)MMIDCD_ALLOC(sizeof(DCD_FEED_T));   
    SCI_MEMSET( feed_ptr,0,sizeof(DCD_FEED_T) );	
        
   return feed_ptr;    	   
}
/*****************************************************************************/
//  Description :  free  a  feed
//  Global resource dependence :the feed exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FeedFrameFree(DCD_FEED_T *feed_ptr)
{
    if(PNULL !=  feed_ptr) 
    {
        TitleFree(&(feed_ptr->feed_title)); 
        SummaryFree(&(feed_ptr->feed_summary));
        LinkHrefFree(&(feed_ptr->feed_link));  
        FeedHrefFree(feed_ptr);
        if(feed_ptr->content_ptr)
        {
            ContentListFree(&(feed_ptr->content_ptr));
        } 
    
        if(feed_ptr->entry_list_head_ptr)
        {
            EntryListFree(&(feed_ptr->entry_list_head_ptr));
        } 
        
        MMIDCD_FreeMem(feed_ptr);      
    }
}
/*****************************************************************************/
//  Description :  build feed_meta list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL DCD_FEED_META_T *FeedMetaAdd2List(DCD_FEED_META_T **list_head_ptr, DCD_FEED_META_T *node_ptr)
{
    DCD_FEED_META_T *next_ptr =PNULL;
 
    if(PNULL == list_head_ptr || PNULL == node_ptr) 
    {
        return PNULL; 
    }
	
    //SCI_TRACE_LOW:"DCD_FeedMetaListAdd() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_2775_112_2_18_2_13_13_337,(uint8*)"");
    if(PNULL == (*list_head_ptr))
    {
        (*list_head_ptr)= node_ptr;   
    }
    else    
    {
        next_ptr = (*list_head_ptr);   
        /* if the next ptr is NULL,add the node to the ptr,and loop is over*/
        while(  (PNULL != next_ptr->next_ptr)  )
        {            
            next_ptr = next_ptr->next_ptr;
        }
        
        next_ptr->next_ptr = node_ptr;   
    }
    node_ptr->next_ptr = PNULL;   
    
    return node_ptr;
}
/*****************************************************************************/
//  Description :  free feed_meta list
//  Global resource dependence :the content list exists
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FeedMetaListFree( DCD_FEED_META_T **list_head_ptr)
{
    DCD_FEED_META_T  *cur_ptr = PNULL;
    DCD_FEED_META_T  *pre_ptr = PNULL;

    if(PNULL == list_head_ptr || PNULL == *list_head_ptr) 
    {
        return; 
    }
    
    cur_ptr = (*list_head_ptr);
    while(cur_ptr)
    {        
        pre_ptr = cur_ptr;
        cur_ptr = (DCD_FEED_META_T *)pre_ptr->next_ptr;
        
        FeedMetaFrameFree(pre_ptr);
    }
    (*list_head_ptr) = PNULL;       
}
/*****************************************************************************/
//  Description : build content list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL  DCD_CONTENT_T *ContentAdd2List(DCD_CONTENT_T **list_head_ptr, DCD_CONTENT_T *node_ptr)
{
    DCD_CONTENT_T *next_ptr =PNULL;
 
    if(PNULL == list_head_ptr || PNULL == node_ptr) 
    {
        return PNULL; 
    }

    //SCI_TRACE_LOW:"DCD_ContentListAdd() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_2832_112_2_18_2_13_13_338,(uint8*)"");
    if(PNULL == (*list_head_ptr))
    {
        (*list_head_ptr)= node_ptr;   
    }
    else    
    {
        next_ptr = (*list_head_ptr);   
        /* if the next ptr is NULL,add the node to the ptr,and loop is over*/
        while(  (PNULL != next_ptr->next_ptr)  )
        {            
            next_ptr = next_ptr->next_ptr;
        }
        
        next_ptr->next_ptr = node_ptr;   
    }
    node_ptr->next_ptr = PNULL;   
    
    return node_ptr;
}
/*****************************************************************************/
//  Description :   free content list
//  Global resource dependence :the content list exists
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ContentListFree( DCD_CONTENT_T **list_head_ptr)
{
    DCD_CONTENT_T  *cur_ptr = PNULL;
    DCD_CONTENT_T  *pre_ptr = PNULL;
    
    if(PNULL == list_head_ptr || PNULL == *list_head_ptr) 
    {
        return; 
    }
    
    cur_ptr = (*list_head_ptr);
    while(cur_ptr)
    {        
        pre_ptr = cur_ptr;
        cur_ptr = (DCD_CONTENT_T *)pre_ptr->next_ptr;
        
        ContentFrameFree(pre_ptr);
    }
    (*list_head_ptr) = PNULL;           
}
/*****************************************************************************/
//  Description :  build entry list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL DCD_ENTRY_T *EntryAdd2List(DCD_ENTRY_T **list_head_ptr, DCD_ENTRY_T *node_ptr)
{
    DCD_ENTRY_T *next_ptr =PNULL;
 
    if(PNULL == list_head_ptr || PNULL == node_ptr)
    {
        return PNULL;
    }
    //SCI_TRACE_LOW:"DCD_EntryListAdd() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_2888_112_2_18_2_13_13_339,(uint8*)"");

    if(PNULL == (*list_head_ptr))
    {
        (*list_head_ptr)= node_ptr;   
    }
    else    
    {
        next_ptr = (*list_head_ptr);   
        /* if the next ptr is NULL,add the node to the ptr,and loop is over*/
        while(  (PNULL != next_ptr->next_ptr)  )
        {            
            next_ptr = next_ptr->next_ptr;
        }
        
        next_ptr->next_ptr = node_ptr;   
    }
    node_ptr->next_ptr = PNULL;   
    
    return node_ptr;
}
/*****************************************************************************/
//  Description :  free  entry list
//  Global resource dependence :entry list exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EntryListFree(DCD_ENTRY_T **list_head_ptr)
{    
    DCD_ENTRY_T  *cur_ptr = PNULL;
    DCD_ENTRY_T  *pre_ptr = PNULL;
    
    if(PNULL == list_head_ptr || PNULL == *list_head_ptr) 
    {
        return; 
    }
    
    cur_ptr = (*list_head_ptr);
    while(cur_ptr)
    {        
        pre_ptr = cur_ptr;
        cur_ptr = (DCD_ENTRY_T *)pre_ptr->next_ptr;
        
        EntryFrameFree(pre_ptr);
    }
    (*list_head_ptr) = PNULL;        
}
/*****************************************************************************/
//  Description :  build feed list
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL DCD_FEED_T *FeedAdd2List(DCD_FEED_T **list_head_ptr, 
                               DCD_FEED_T *node_ptr
                               )
{
    DCD_FEED_T *next_ptr =PNULL;
 
    if(PNULL == list_head_ptr || PNULL == node_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:FeedAdd2List ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_2945_112_2_18_2_13_13_340,(uint8*)"");
        return PNULL;
    }

    //SCI_TRACE_LOW:"DCD_FeedListAdd() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_2949_112_2_18_2_13_13_341,(uint8*)"");
    if(PNULL == (*list_head_ptr))
    {
        (*list_head_ptr)= node_ptr;   
    }
    else    
    {
        next_ptr = (*list_head_ptr);   
        /* if the next ptr is NULL,add the node to the ptr,and loop is over*/
        while(  (PNULL != next_ptr->next_ptr)  )
        {            
            next_ptr = next_ptr->next_ptr;
        }
        
        next_ptr->next_ptr = node_ptr;   
    }
    node_ptr->next_ptr = PNULL;   
    
    return node_ptr;
}

/*****************************************************************************/
//  Description :  free  feed list
//  Global resource dependence :the feed list exists
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FeedListFree(DCD_FEED_T  **list_head_ptr)
{
    DCD_FEED_T  *cur_ptr = PNULL;
    DCD_FEED_T  *pre_ptr = PNULL;
    
    if(PNULL == list_head_ptr || PNULL == (*list_head_ptr))
    {
        return;
    }
    
    cur_ptr = (*list_head_ptr);
    while(cur_ptr)
    {        
        pre_ptr = cur_ptr;
        cur_ptr = (DCD_FEED_T *)pre_ptr->next_ptr;
        
        FeedFrameFree(pre_ptr);
    }
    (*list_head_ptr) = PNULL;      
}



/*****************************************************************************/
//  Description :  Does the content named ctxt_id exist in a entry ?
//  Global resource dependence : 
//  Author:
//  Note: if exist,return the content node;otherwise return PNULL
/*****************************************************************************/
LOCAL DCD_CONTENT_T *ContentIsExistInEntry(DCD_ENTRY_T *entry_ptr,char* ctxt_id_ptr)
{
    DCD_CONTENT_T *cur_content_ptr = PNULL;
    
    if(PNULL == entry_ptr || PNULL == ctxt_id_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:ContentIsExistInEntry ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_3007_112_2_18_2_13_13_342,(uint8*)"");
        return PNULL;
    } 
    
    cur_content_ptr =  entry_ptr -> content_ptr;
    while (cur_content_ptr) 
    {
        if(!stricmp(cur_content_ptr ->ctxt_id,ctxt_id_ptr))
        {
            return cur_content_ptr;
        }
        
        cur_content_ptr = (DCD_CONTENT_T *)cur_content_ptr ->next_ptr;
    }	
    return PNULL;
}

/*****************************************************************************/
//  Description :  Does the content named ctxt_id exist in a feed ?
//  Global resource dependence : 
//  Author:
//  Note: if exist,return the content node;otherwise return PNULL
/*****************************************************************************/
LOCAL DCD_CONTENT_T *ContentIsExistInFeed(DCD_FEED_T *feed_ptr,char* ctxt_id_ptr)
{
    DCD_CONTENT_T *cur_content_ptr = PNULL;
    DCD_ENTRY_T *cur_entry_ptr = PNULL;
        
    if(PNULL == feed_ptr || PNULL == ctxt_id_ptr)
    {
        return PNULL;
    }  
    
    cur_content_ptr =  feed_ptr -> content_ptr;
    while (cur_content_ptr) 
    {
        if(!stricmp(cur_content_ptr ->ctxt_id,ctxt_id_ptr))
        {
            return cur_content_ptr;
        }
        
        cur_content_ptr = (DCD_CONTENT_T *)cur_content_ptr ->next_ptr;
    }	
    
    cur_entry_ptr = feed_ptr -> entry_list_head_ptr;
    while (cur_entry_ptr) 
    {
    	cur_content_ptr = ContentIsExistInEntry(cur_entry_ptr,ctxt_id_ptr);
        if(cur_content_ptr)
        {
            return cur_content_ptr;
        }
        
        cur_entry_ptr = (DCD_ENTRY_T *)cur_entry_ptr ->next_ptr;
    }	    
    return PNULL;
}

/*****************************************************************************/
//  Description :  Does the  ctxt_id exist in a syncpkt ?
//  Global resource dependence : 
//  Author:
//  Note: if exist,return TURE;otherwise return FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IsContentExist(DCD_SYNC_PKG_T *syncpkg_ptr, char* ctxt_id_ptr)
{
    DCD_FEED_T    *cur_feed_ptr = PNULL;
    DCD_CONTENT_T *cur_content_ptr = PNULL;
    BOOLEAN       result = FALSE;
    
    if(PNULL == syncpkg_ptr || PNULL == ctxt_id_ptr)
    {
        return FALSE;
    }
    
    cur_feed_ptr =  syncpkg_ptr -> feed_list_ptr;
    while (PNULL != cur_feed_ptr) 
    {
        cur_content_ptr = ContentIsExistInFeed(cur_feed_ptr,ctxt_id_ptr);
        if(PNULL != cur_content_ptr)
        {
            cur_content_ptr->picture_valiad = TRUE;
            //SCI_TRACE_MID:"MMIDCD nend not download picutre"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_3085_112_2_18_2_13_13_343,(uint8*)"");
           
            result = TRUE;            
            //break; 和所有需下载的比较        
        }
        
        cur_feed_ptr = (DCD_FEED_T *)cur_feed_ptr ->next_ptr;
    }   
    
    return result;
}


/*****************************************************************************/
//  Description :   Reset global_meta
//  Global resource dependence : the entry exist
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void GlobalMetaReset(DCD_GLOBAL_META_T  *global_meta_ptr)
{
    if(PNULL != global_meta_ptr)
    {
        GlobalMetaAnchorFree(global_meta_ptr);
        
        SCI_MEMSET( global_meta_ptr,0,sizeof(DCD_GLOBAL_META_T) );
    }
}

/*****************************************************************************/
//  Description :  Reset meta
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MetaReset( DCD_META_T *meta_ptr)
{
    if(PNULL != meta_ptr)
    {
        GlobalMetaReset( &(meta_ptr->global_meta) );
        FeedMetaListFree(&(meta_ptr->feed_meta_list_ptr));
    }
}

/*****************************************************************************/
//  Description :  Reset ync_pkg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SyncPkgReset(DCD_SYNC_PKG_T *sysc_pkg_ptr)
{
    if(PNULL != sysc_pkg_ptr)
    {
        MetaReset(&(sysc_pkg_ptr->meta));    
        FeedListFree(&(sysc_pkg_ptr->feed_list_ptr)); 
    }
}
/*****************************************************************************/
//  Description :   Reset global_meta
//  Global resource dependence : the entry exist
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void UserDataReset(XML_USERDATA* userdata_ptr)
{
    if(PNULL != userdata_ptr)
    {
        userdata_ptr->depth = 0;                    
        userdata_ptr->cur_element_id =	DCDXML_ELEMENT_ID_UNUSE;
        userdata_ptr->parser_status  =	DCDXML_INIT;
        userdata_ptr->cur_meta       =	PNULL;
        userdata_ptr->cur_feed       =	PNULL;
        userdata_ptr->cur_entry      =	PNULL;
        
        SyncPkgReset(  &(userdata_ptr->sync_pkg)  );
    }
    
	      

}

/*****************************************************************************/
//  Description :  set entry issued time
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryIssued(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != entry_ptr)
    {
        entry_ptr -> issued_time = StrToSecond((char*)data_ptr);
    }
}

/*****************************************************************************/
//  Description :  set entry activation time
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryActivation(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != entry_ptr)
    {
        entry_ptr -> entry_activation = StrToSecond((char*)data_ptr);
    }
}

/*****************************************************************************/
//  Description :  set entry expiration time
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryExpiration(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != entry_ptr)
    {
        entry_ptr -> entry_expiration = StrToSecond((char*)data_ptr);
    }	
}

/*****************************************************************************/
//  Description :  set entry autherURL
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryAutherName(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len)
{
    if(PNULL != entry_ptr)
    {
        CopyString(&(entry_ptr->entry_author.author_name),data_ptr,data_len);
    }	 
}
/*****************************************************************************/
//  Description :  free memory of  author_name ,which's in the struct of DCD_ENTRY_AUTHOR_T
//  Global resource dependence :none
//  Author:
//  Note: free memory of  href ,which's in the struct of DCD_CONTENT_T
/*****************************************************************************/
LOCAL void EntryAuthorNameFree(DCD_ENTRY_AUTHOR_T * author_ptr)
{
    if(PNULL != author_ptr)
    {
        StringFree(&(author_ptr->author_name));
    }
}
/*****************************************************************************/
//  Description :  set entry autherURL
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryAutherUrl(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len)
{
    uint32 len = data_len;

    if(PNULL == entry_ptr)
    {
        return;
    }
	
    if( len > DCD_MAX_URL_LEN)
    {
        len = DCD_MAX_URL_LEN;
    }
    
    SCI_MEMSET(entry_ptr ->entry_author.author_url,0x00,(DCD_MAX_URL_LEN + 1));
    SCI_MEMCPY(entry_ptr ->entry_author.author_url, data_ptr, len);
}

/*****************************************************************************/
//  Description :  set entry auther Name
//  Global resource dependence : the entry exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SetEntryAutherEmail(DCD_ENTRY_T * entry_ptr,uint8 *data_ptr, uint16 data_len)
{
    uint32 len = data_len;

    if(PNULL == entry_ptr)
    {
        return;
    }
	
    if( len > DCD_MAX_AUTHOR_EMAIL_LEN)
    {
        len = DCD_MAX_AUTHOR_EMAIL_LEN;
    }
    
    SCI_MEMSET(entry_ptr ->entry_author.author_email,0x00,(DCD_MAX_AUTHOR_EMAIL_LEN + 1));
    SCI_MEMCPY(entry_ptr ->entry_author.author_email, data_ptr, len);
    
}

/*****************************************************************************/
//  Description :  when download,get the content
//  Global resource dependence : 
//  Author:
//  Note: 
//    /*xml解析完后调用*/
//    MMIDCD_DownloadContentByList(MMIDCD_GetXMLData(),&Http_DownloadContentCallback);
/*****************************************************************************/
PUBLIC void MMIDCD_DownloadContentByList(DCD_SYNC_PKG_T *pkg_ptr,DCD_TRAVERSE_CONTENT_CALLBACK callback)
{
    DCD_FEED_T *feed_ptr = PNULL;
    DCD_ENTRY_T *entry_ptr = PNULL;
    DCD_CONTENT_T *content_ptr = PNULL;

    if(PNULL == pkg_ptr || PNULL == callback)
    {
        return;
    }
    
    feed_ptr = pkg_ptr->feed_list_ptr;
    
    while(feed_ptr)
    {
        /*feed content*/
        content_ptr = feed_ptr->content_ptr;
        while(content_ptr)
        {     
            if(!content_ptr->picture_valiad)
            {
                callback(content_ptr);
            }
            content_ptr = content_ptr->next_ptr;
        }
        
        /*entry content*/
        entry_ptr = feed_ptr ->entry_list_head_ptr;
        while(entry_ptr)
        {
            content_ptr = entry_ptr ->content_ptr;
            while(content_ptr)
            {
                if(!content_ptr->picture_valiad)
                {
                    callback(content_ptr);
                }
                content_ptr = content_ptr->next_ptr;
            }            
            entry_ptr = entry_ptr ->next_ptr;
        }
        
        feed_ptr = feed_ptr->next_ptr;
    }
    
}

/*****************************************************************************/
//                   Interface for MMI                                             
/*****************************************************************************/
LOCAL const char* s_suffix[DCD_CONTENT_TYPE_MAX] =
{
    PNULL,
    DCD_FILE_TYPE_GIF_SUFFIX_TXT,
    DCD_FIEL_TYPE_JPEG_SUFFIX_TXT,
    DCD_FIEL_TYPE_BMP_SUFFIX_TXT, 
//    DCD_FIEL_TYPE_MP3_SUFFIX_TXT,
//    DCD_FIEL_TYPE_WAV_SUFFIX_TXT,    
//    DCD_FIEL_TYPE_AAC_SUFFIX_TXT,
//    DCD_FIEL_TYPE_EAAC_SUFFIX_TXT,
//    DCD_FIEL_TYPE_MPG4_SUFFIX_TXT,
//    DCD_FIEL_TYPE_H263_SUFFIX_TXT,     
};

/*****************************************************************************/
//  Description :  get the suffix of multimedia file
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL char* GetContentTypeStr(DCD_CONTENT_TYPE_E type_id)
{
    if(type_id < DCD_CONTENT_TYPE_MAX)
    {
        return (char*)s_suffix[type_id];/*lint !e661*/
    }
    else
    {
        return PNULL;
    }
    
}
/*****************************************************************************/
//  Description :  get the suffix of multimedia file
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIDCD_GetContentTypeStr(DCD_CONTENT_TYPE_E type_id)
{
    return GetContentTypeStr(type_id);
}
/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: index from 0.....
/*****************************************************************************/
LOCAL DCD_FEED_META_T *GetFeedMetaByIndex(DCD_FEED_META_T * list_head_ptr,uint16 index)
{
    DCD_FEED_META_T *cur_ptr = PNULL;
    uint16 cur_index = 0;
    
    if(PNULL == list_head_ptr)
    {
        return PNULL;
    }
    
    cur_ptr = list_head_ptr;
    while(cur_ptr)
    {
        if( index == cur_index)
        {
            return cur_ptr;
        }
        cur_index ++;  
        cur_ptr = cur_ptr -> next_ptr;      
    }   
    
    return PNULL;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: Note: index from 0.....
/*****************************************************************************/
LOCAL DCD_CONTENT_T *GetContentByIndex(DCD_CONTENT_T * list_head_ptr,uint16 index)
{
    DCD_CONTENT_T *cur_ptr = PNULL;
    uint16 cur_index = 0;
    
    if(PNULL == list_head_ptr)
    {
        return PNULL;
    }
    
    cur_ptr = list_head_ptr;
    while(cur_ptr)
    {
        if( index == cur_index)
        {
            return cur_ptr;
        }
        cur_index ++;  
        cur_ptr = cur_ptr -> next_ptr;      
    }  
    
    return PNULL;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: Note: index from 0.....
/*****************************************************************************/
LOCAL DCD_ENTRY_T *GetEntryByIndex(DCD_ENTRY_T * list_head_ptr,uint16 index)
{
    DCD_ENTRY_T *cur_ptr = PNULL;
    uint16 cur_index = 0;
    
    if(PNULL == list_head_ptr)
    {
        return PNULL;
    }   
    
    cur_ptr = list_head_ptr;
    while(cur_ptr)
    {
        if( index == cur_index)
        {
            return cur_ptr;
        }
        cur_index ++;  
        cur_ptr = cur_ptr -> next_ptr;      
    }   
    
    return PNULL;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note: Note: index from 0.....
/*****************************************************************************/
LOCAL DCD_FEED_T *GetFeedByIndex(DCD_FEED_T * list_head_ptr,uint16 index)
{
    DCD_FEED_T *cur_ptr = PNULL;
    uint16 cur_index = 0;
    
    if(PNULL == list_head_ptr)
    {
        return PNULL;
    }
    
    cur_ptr = list_head_ptr;
    while(cur_ptr)
    {
        if( index == cur_index)
        {
            return cur_ptr;
        }
        cur_index ++;  
        cur_ptr = cur_ptr -> next_ptr;      
    }   
    
    return PNULL;
}
/*****************************************************************************/
//  Description :  
//  Global resource dependence : the feed exist
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetFeedMetaListNum(DCD_FEED_META_T *list_head_ptr)
{
    uint16  list_len = 0;
    DCD_FEED_META_T *cur_ptr = PNULL;

    if(PNULL == list_head_ptr)
    {
        return list_len;
    }   
    
    cur_ptr  =  list_head_ptr;
    while( cur_ptr )
    {
        list_len ++;
        cur_ptr = cur_ptr -> next_ptr;
    }
        
    return list_len;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : the feed exist
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL uint16 GetContentListNum(DCD_CONTENT_T *list_head_ptr)
{
    uint16  list_len = 0;
    DCD_CONTENT_T *cur_ptr = PNULL;

    if(PNULL == list_head_ptr)
    {
        return list_len;
    }    
    
    cur_ptr  =  list_head_ptr;
    while( cur_ptr )
    {
        list_len ++;
        cur_ptr = cur_ptr -> next_ptr;
    }
 
    return list_len;
}

/*****************************************************************************/
//  Description :  
//  Global resource dependence : 
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL uint16 GetEntryListNum(DCD_ENTRY_T *list_head_ptr)
{
    uint16  list_len = 0;
    DCD_ENTRY_T *cur_ptr = PNULL;

    if(PNULL == list_head_ptr)
    {
        return list_len;
    }      
    
    cur_ptr  =  list_head_ptr;
    while( cur_ptr )
    {
        list_len ++;
        cur_ptr = cur_ptr -> next_ptr;
    }
        
    return list_len;
}
/*****************************************************************************/
//  Description :  
//  Global resource dependence : the feed exist
//  Author:
//  Note:  
/*****************************************************************************/
LOCAL uint16 GetFeedListNum(DCD_FEED_T *list_head_ptr)
{
    uint16  list_len = 0;
    DCD_FEED_T *cur_ptr = PNULL;

    if(PNULL == list_head_ptr)
    {
        return list_len;
    }    
    
    cur_ptr  =  list_head_ptr;
    while( cur_ptr )
    {
        list_len ++;
        cur_ptr = cur_ptr -> next_ptr;
    }

    return list_len;
}
/*****************************************************************************/
// 	Description : 获取指定feed指定Item节点
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
LOCAL DCD_ENTRY_T* GetEntryNode( xmlDocPtr doc_ptr, uint16 FeedIndex, uint16 EntryIndex )
{
	if( PNULL != doc_ptr )
    {
	    DCD_FEED_T* feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr,FeedIndex);
	    if(NULL != feed_ptr)
	    {
	        return GetEntryByIndex(feed_ptr->entry_list_head_ptr,EntryIndex);         
	    }    
    }
    
    return PNULL;   
}
                 
/*****************************************************************************/
// 	Description : 获取feed总数
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/                         
PUBLIC uint16 MMIDCD_GetFeedNum(xmlDocPtr doc_ptr)
{
    uint16 feednum = 0;
    
    if(PNULL != doc_ptr)
    {
        feednum =  GetFeedListNum(doc_ptr->feed_list_ptr);  
    }
    //added by jun.hu for 和外面的相匹配（受静态数组容量的限制）....
    if(MMIDCD_MAX_PAGE_NUM < feednum)
    {
        feednum = MMIDCD_MAX_PAGE_NUM;
    }
    
    return feednum;
}
/*****************************************************************************/
// 	Description : 获取外面使用的feed总数
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 区别于上面一个函数的地方在于这里固定了doc
/*****************************************************************************/                         
PUBLIC uint16 MMIAPIDCD_GetFormalFeedNum(void)
{
    uint16 feednum = 0;
    xmlDocPtr doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr(); 
    if(PNULL != doc_ptr)
    {
        feednum = GetFeedListNum(doc_ptr->feed_list_ptr);   
    }
    //added by jun.hu for 和外面的相匹配（受静态数组容量的限制）....
    if(MMIDCD_MAX_PAGE_NUM < feednum)
    {
        feednum = MMIDCD_MAX_PAGE_NUM;
    }
    
    return feednum;
}
/*****************************************************************************/
// 	Description : 获取指定feed中Item总数
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetEntryNumInFeed(xmlDocPtr doc_ptr, uint16 FeedIndex)
{
    uint16      entrynum = 0;
    DCD_FEED_T* feed_ptr = PNULL;

    if(PNULL != doc_ptr)
    {
        feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr,FeedIndex);
    }

    if(PNULL != feed_ptr)
    {
        entrynum = GetEntryListNum(feed_ptr->entry_list_head_ptr);
    }    
    //added by jun.hu for 和外面的相匹配（受静态数组容量的限制）....  
    if(MMIDCD_MAX_ITEM_NUM_PER_PAGE < entrynum)
    {
        entrynum = MMIDCD_MAX_ITEM_NUM_PER_PAGE;
    }
    return entrynum;       
}
/*****************************************************************************/
// 	Description : 获取指定feed中Item总数
//	Global resource dependence : none
//  Author:jun.hu
//	Note:与上面一个函数的区别是这里是指定好了的doc, 用户只需要调用就好了...
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetFormalEntryNumInFeed(uint16 FeedIndex)
{
    uint16      entrynum = 0;
    DCD_FEED_T* feed_ptr = PNULL;
    xmlDocPtr doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr(); 
    if( PNULL == doc_ptr )
    {
        return 0;
    }
    
    feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr,FeedIndex);
    
    if(PNULL != feed_ptr)
    {
        entrynum = GetEntryListNum(feed_ptr->entry_list_head_ptr);
    }
    
    if(MMIDCD_MAX_ITEM_NUM_PER_PAGE < entrynum)
    {
        entrynum = MMIDCD_MAX_ITEM_NUM_PER_PAGE;
    }
    
    return entrynum;   
}
/*****************************************************************************/
// 	Description : 获取指定feed中content总数，因一个频道可以包含多个content
//	Global resource dependence : none
//  Author:byte.guo
//	Note: 此函数暂时没有被用到..................
/*****************************************************************************/                
PUBLIC uint16 MMIDCD_GetContentNumInFeed(
                                         xmlDocPtr doc_ptr, 
                                         uint16 FeedIndex
                                         )
{
    if( PNULL != doc_ptr )
    {
        DCD_FEED_T* feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr,FeedIndex);
        if(NULL != feed_ptr)
        {
            return GetContentListNum(feed_ptr->content_ptr);
        }
    }
    
    return 0;   
}                   

                    

/*****************************************************************************/
// 	Description : 获取指定频道指定Item的content数目
//	Global resource dependence : none
//  Author:byte.guo
//	Note:此函数暂时没有被用到..................
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetContentNumInEntry(
                                          xmlDocPtr doc_ptr,
                                          uint16 FeedIndex,
                                          uint16 EntryIndex
                                          )
{
    if(PNULL != doc_ptr)
    {
        DCD_FEED_T* feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr,FeedIndex);
        
        if(NULL != feed_ptr)
        {
            DCD_ENTRY_T* entry_ptr = GetEntryByIndex(feed_ptr->entry_list_head_ptr,EntryIndex);
            if(NULL != entry_ptr)
            {           
                return GetContentListNum(entry_ptr->content_ptr);
                
            }
        }
    }    
    
    return 0;   
}            
/*****************************************************************************/
// 	Description : 获取指定频道的title
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/9/3 重写了这个函数...9/25,注释后面加上out....
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetFeedTitle(
                                  wchar *  wstr_title_ptr, //out 返回wchar型的title
                                  const uint16  dst_max_count,  
                                  const uint16  FeedIndex
                                  )
{
    uint8   *feed_title_ptr = PNULL;
    uint16   title_len = 0;
    DCD_FEED_T* feed_ptr = PNULL;
    xmlDocPtr doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();

    if(PNULL == doc_ptr || PNULL == wstr_title_ptr)
    {
        return 0;
    }
   
    
    feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr, FeedIndex);
    
    if(PNULL != feed_ptr)
    {
        
        feed_title_ptr = feed_ptr->feed_title.ucs2_str.str_ptr;
        title_len = feed_ptr->feed_title.ucs2_str.str_len;
        if(dst_max_count < title_len)
        {
            title_len = dst_max_count;
        }

        title_len = GUI_OtherCodeToWstr(wstr_title_ptr, title_len, GUI_CHAR_CODE_UCS2_BE, feed_title_ptr, title_len);
    }
    
    return title_len;       
}  
/*****************************************************************************/
// 	Description : 获取指定频道的summary
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC DCD_UCS2_STR_T* MMIDCD_GetFeedSummary(xmlDocPtr doc_ptr,uint16 FeedIndex)
{
    if( PNULL != doc_ptr )
    {
        DCD_FEED_T* feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr,FeedIndex);
        if(NULL != feed_ptr)
        {
            return &(feed_ptr->feed_summary.ucs2_str);		
        }
    }
    
    return PNULL;	    
}
/*****************************************************************************/
// 	Description : 获取指定频道指定新闻的title(返回uint8 型的，和存储的一致）
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/9/1 重写了这个函数...
/*****************************************************************************/
PUBLIC uint8* MMIDCD_GetEntryStrTitle(
                                      uint16  *titlr_len_ptr,  //out 返回存储的字符串的长度...
                                      const uint16  FeedIndex,
                                      const uint16  EntryIndex
                                      )
{
    uint8 *entry_title_ptr = PNULL;
    DCD_ENTRY_T *entry_ptr = PNULL;
    DCD_FEED_T *feed_ptr = PNULL;
    xmlDocPtr doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();

    if(PNULL == doc_ptr || PNULL == titlr_len_ptr)
    {
        return PNULL;
    }
    
    feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr, FeedIndex);
    
    if(PNULL != feed_ptr)
    {
        entry_ptr = GetEntryByIndex(feed_ptr->entry_list_head_ptr, EntryIndex);
        if(PNULL != entry_ptr)
        {           
            entry_title_ptr = entry_ptr->entry_title.ucs2_str.str_ptr;
            *titlr_len_ptr = entry_ptr->entry_title.ucs2_str.str_len;
        }
    }
    
    return entry_title_ptr;       
}
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
                                        )
{
    uint8* summary_ptr = PNULL;
    DCD_ENTRY_T* entry_ptr = PNULL;
    DCD_FEED_T* feed_ptr = PNULL;
    xmlDocPtr doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();

    if( PNULL == doc_ptr || PNULL == summary_len_ptr)
    {
        return PNULL;
    }
   
    
    feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr, FeedIndex);
    if(NULL != feed_ptr)
    {
        entry_ptr = GetEntryByIndex(feed_ptr->entry_list_head_ptr, EntryIndex);
        if(NULL != entry_ptr)
        {           
            summary_ptr = entry_ptr->entry_summary.ucs2_str.str_ptr;
            *summary_len_ptr = entry_ptr->entry_summary.ucs2_str.str_len;
        }
    }
    
    return summary_ptr;      
}
/*****************************************************************************/
// 	Description : 获取指定频道的全局的唯一的id
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/10/12
/*****************************************************************************/                   
PUBLIC char * MMIDCD_GetFeedCTXTId(
                                  const xmlDocPtr doc_ptr,
                                  const uint16  FeedIndex
                                  )
{
    char * feedid_ptr = PNULL;
    DCD_FEED_T * feed_ptr = PNULL;
    uint16 feed_num = 0;

    if(PNULL == doc_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_GetFeedCTXTId ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_3858_112_2_18_2_13_15_344,(uint8*)"");
        return PNULL;
    }
    
    // 对index进行检查
    feed_num = MMIDCD_GetFeedNum(doc_ptr);

    // 获取指定频道的内容的指针...
    feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr, FeedIndex);
    if(PNULL != feed_ptr)
    {
        // 获取该频道中指定频道项的指针....
        feedid_ptr = feed_ptr->ctxt_id;
    }
    else
    {
        //SCI_TRACE_LOW:"MMI DCD this index has no chanel"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_3874_112_2_18_2_13_15_345,(uint8*)"");
    }
    return feedid_ptr;      
} 
/*****************************************************************************/
// 	Description : 获取指定频道指定新闻(即频道项）的内容的全局的唯一的id
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/10/22
/*****************************************************************************/                   
PUBLIC char * MMIDCD_GetEntryContentCTXTId(
                                           const xmlDocPtr doc_ptr,
                                           const uint16  FeedIndex,
                                           const uint16  EntryIndex
                                           )
{
    char* contentid_ptr = PNULL;
    DCD_ENTRY_T* entry_ptr = PNULL;
    DCD_FEED_T* feed_ptr = PNULL;
    if( PNULL == doc_ptr)
    {
        return PNULL;
    }

    // 获取指定频道的内容的指针...
    feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr, FeedIndex);
    if(PNULL != feed_ptr)
    {
        // 获取该频道中指定频道项的指针....
        entry_ptr = GetEntryByIndex(feed_ptr->entry_list_head_ptr, EntryIndex);
        if(PNULL != entry_ptr)
        {  
            if(PNULL != entry_ptr->content_ptr)
            {
                contentid_ptr = entry_ptr->content_ptr->ctxt_id;
            }
        }
    }
    
    return contentid_ptr;      
}
/*****************************************************************************/
// 	Description : 获取指定频道指定新闻的title, 返回的是str的长度
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/9/1 重写了这个函数...
/*****************************************************************************/
PUBLIC uint16 MMIAPIDCD_GetEntryTitle(
                                    wchar  * wstr_title_ptr, //out 返回wchar型的title
                                    const uint16  dst_max_count,  
                                    const uint16  FeedIndex,
                                    const uint16  EntryIndex
                                    )
{
    uint8* entry_title_ptr = PNULL;
    uint16 title_len = 0;
    DCD_ENTRY_T* entry_ptr = PNULL;
    DCD_FEED_T* feed_ptr = PNULL;

    xmlDocPtr doc_ptr = PNULL;

    doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();
    if(PNULL == doc_ptr || PNULL == wstr_title_ptr)
    {
        return 0;
    }
    
    feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr, FeedIndex);
    
    if(PNULL != feed_ptr)
    {
        entry_ptr = GetEntryByIndex(feed_ptr->entry_list_head_ptr, EntryIndex);
        if(PNULL != entry_ptr)
        {           
            entry_title_ptr = entry_ptr->entry_title.ucs2_str.str_ptr;
            title_len = entry_ptr->entry_title.ucs2_str.str_len;
            // 源是以uint8 为单位的，而要转化出去的目标却是为wchar为单位的，故要换算一下....
            if(dst_max_count * sizeof(wchar) < title_len)
            {
                title_len = dst_max_count * sizeof(wchar);
            }
            GUI_OtherCodeToWstr(wstr_title_ptr, dst_max_count, GUI_CHAR_CODE_UCS2_BE, entry_title_ptr, title_len);
            // 重新求一下长度值， 因为上面记录的title_len可能不准确.....
            title_len = MMIAPICOM_Wstrlen(wstr_title_ptr);
        }
    }
    
    return title_len;       
}  
/*****************************************************************************/
// 	Description : 获取指定频道指定新闻的summary
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 
/*****************************************************************************/                   
PUBLIC uint16 MMIAPIDCD_GetEntrySummary(
                                     wchar * wstr_summary_ptr, // out 返回wchar型的title
                                     const uint16  dst_max_count, 
                                     uint16 FeedIndex,
                                     uint16 EntryIndex
                                     )
{
    uint8* summary_ptr = PNULL;
    DCD_ENTRY_T* entry_ptr = PNULL;
    DCD_FEED_T* feed_ptr = PNULL;
    uint16 summary_len = 0;
    xmlDocPtr doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();

    if( PNULL == doc_ptr || PNULL == wstr_summary_ptr)
    {
        return 0;
    }
    
    feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr, FeedIndex);
    if(PNULL != feed_ptr)
    {
        entry_ptr = GetEntryByIndex(feed_ptr->entry_list_head_ptr, EntryIndex);
        if(PNULL != entry_ptr)
        {           
            summary_ptr = entry_ptr->entry_summary.ucs2_str.str_ptr;
            summary_len = entry_ptr->entry_summary.ucs2_str.str_len;
            // 源是以uint8 为单位的，而要转化出去的目标却是为wchar为单位的，故要换算一下....
            // jun.hu 2009/11/26 for cr162192.....
            if(dst_max_count * sizeof(wchar) < summary_len)
            {
                summary_len = dst_max_count * sizeof(wchar);
            }
            GUI_OtherCodeToWstr(wstr_summary_ptr, dst_max_count, GUI_CHAR_CODE_UCS2_BE, summary_ptr,summary_len);
            // 重新求一下长度值， 因为上面记录的summary_len可能不准确.....
            summary_len = MMIAPICOM_Wstrlen(wstr_summary_ptr);
        }
    }
    
    return summary_len;      
} 

/*****************************************************************************/
//  Description : judge whether this year is leap year
//  Global resource dependence : 
//  Author: 
//Note:	
///*****************************************************************************/
LOCAL BOOLEAN IsLeapyear(int  year)
{
    if (year % 400 == 0)
    {
        return TRUE;
    }
    else if (year % 100 == 0)
    {
        return FALSE;
    }
    else if (year % 4 == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 获取指定Feed中指定Content属性值
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC char* MMIAPIDCD_GetFeedContentInfo(
                                        uint16 FeedIndex,
                                        uint16 iContentIndex,
                                        char * name_suffix
                                        )
{
    DCD_FEED_T* feed_ptr = PNULL;
    xmlDocPtr doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();
    if( PNULL == doc_ptr)
    {
        return PNULL;
    }
    
    feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr, FeedIndex);
    if(PNULL != feed_ptr)
    {
        DCD_CONTENT_T* content_ptr = GetContentByIndex(feed_ptr->content_ptr,iContentIndex);
        if(PNULL != content_ptr)
        {
            if( stricmp(name_suffix, MMIDCD_TYPE_SUFFIX)==0 )
            {
                return GetContentTypeStr(content_ptr->content_type);
                
            }
            else if( stricmp(name_suffix, MMIDCD_TYPE_NAME)==0 )
            {
                return content_ptr->ctxt_id;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIDCD content_ptr no cmp"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_4063_112_2_18_2_13_15_346,(uint8*)"");
            }
            
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDCD content_ptr is null now"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_4069_112_2_18_2_13_15_347,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD feed_ptr is null now"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_4074_112_2_18_2_13_15_348,(uint8*)"");
    }
    
    
    return PNULL;  	
}							 
/*****************************************************************************/
// 	Description : 获取指定Feed中指定Item指定Content属性值
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/				 
PUBLIC char * MMIAPIDCD_GetEntryContentInfo(
                                        uint16 FeedIndex,
                                        uint16 EntryIndex,
                                        uint16 iContentIndex,
                                        char *name_suffix 
                                        )
{
    xmlDocPtr doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();
    DCD_ENTRY_T* entry_ptr = PNULL;
    if(PNULL == doc_ptr)
    {
        return PNULL;
    }

    entry_ptr = GetEntryNode(doc_ptr,FeedIndex,EntryIndex );
    if(PNULL != entry_ptr)
    {           
        DCD_CONTENT_T* content_ptr = GetContentByIndex(entry_ptr->content_ptr,iContentIndex);
        if(PNULL != content_ptr)
        {
            if( stricmp(name_suffix,MMIDCD_TYPE_SUFFIX)==0 )
            {
                return GetContentTypeStr(content_ptr->content_type);
            }
            else if( stricmp(name_suffix,MMIDCD_TYPE_NAME)==0 )
            {
                return content_ptr->ctxt_id;
            }
            else
            {  
                //SCI_TRACE_LOW:"[MMIDCD]:MMIAPIDCD_GetEntryContentInfo type dont matching ERROR!!!!!!!!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_4114_112_2_18_2_13_15_349,(uint8*)"");
            }
            
        }
        
    }
                
    return PNULL;       
}							 

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
                                  )
{
    DCD_ENTRY_T *entry_ptr = PNULL;
    xmlDocPtr   doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();
    if(PNULL == doc_ptr)
    {
        return PNULL;
    }
    
    entry_ptr = GetEntryNode(doc_ptr,FeedIndex,EntryIndex );
    if(NULL != entry_ptr)
    {           
        if(stricmp(AttrName, MMIDCD_LINK_ATTR)==0 )
        {
            return entry_ptr->entry_link.href_ptr;
        }                       
        else
        {
            //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_GetEntryLink xml type dont matching ERROR!!!!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_XML_PARSE_4150_112_2_18_2_13_15_350,(uint8*)"");
        }                                                           
    }
    
    return PNULL;   
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:byte.guo
//  Note:modified by jun.hu 2009/10/13, 根据flag, 看获得哪一个xml 文件对应的内存影像数据...
/*****************************************************************************/
PUBLIC XML_USERDATA* MMIDCD_GetXMLData(BOOLEAN get_prefile_Data)
{
    if(get_prefile_Data)
    {
        return &s_user_pre_data;
    }
    else
    {
        return &g_user_data;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:byte.guo
//  Note: modified by jun.hu 2009/10/13, 根据flag, 看释放哪一个xml文件的内存影像数据...
/*****************************************************************************/
PUBLIC void MMIDCD_XMLDataFree(BOOLEAN free_prefile_Data)
{
    if(free_prefile_Data)
    {
        UserDataReset(&s_user_pre_data);//DCDXML_FreeXMLData();
    }
    else
    {
        UserDataReset(&g_user_data);
    }
}

                 
/*****************************************************************************/
// 	Description : 释放一个feed
//	Global resource dependence : none
//  Author:panvs.zhong
//	Note:
/*****************************************************************************/                         
PUBLIC void MMIDCD_FeedFree(DCD_FEED_T *feed_ptr)
{
    FeedFrameFree(feed_ptr);
}
/*****************************************************************************/
// 	Description : 释放一个entry
//	Global resource dependence : none
//  Author:panvs.zhong
//	Note:
/*****************************************************************************/                         
PUBLIC void MMIDCD_EntryFree(DCD_ENTRY_T *entry_ptr)
{
    EntryFrameFree(entry_ptr);
}
/*****************************************************************************/
// 	Description : 获取feed总数
//	Global resource dependence : none
//  Author:panvs.zhong
//	Note:
/*****************************************************************************/                         
PUBLIC uint16 MMIDCD_GetRealFeedNum(xmlDocPtr doc_ptr)
{
    uint16 feednum = 0;
    
    if(PNULL != doc_ptr)
    {
        feednum =  GetFeedListNum(doc_ptr->feed_list_ptr);  
    }
    
    return feednum;
}
/*****************************************************************************/
// 	Description : 获取feed 中Entry 总数
//	Global resource dependence : none
//  Author:panvs.zhong
//	Note: 
/*****************************************************************************/                         
PUBLIC uint16 MMIDCD_GetRealEntryNum(xmlDocPtr doc_ptr, uint16 feedIndex)
{
    DCD_FEED_T *feed_ptr = PNULL;
    uint16      entrynum = 0;
    
    if(PNULL != doc_ptr)
    {
        feed_ptr = GetFeedByIndex(doc_ptr->feed_list_ptr,feedIndex);
        if(PNULL != feed_ptr)
        {
            entrynum = GetEntryListNum(feed_ptr->entry_list_head_ptr);
        }
    }
    
    return entrynum;   
}
/*****************************************************************************/
//    Description : 获取指定频道
//    Global resource dependence : none
//    Author:panvs.zhong
//    Note: 2011-07-15
/*****************************************************************************/                   
PUBLIC DCD_FEED_T * MMIDCD_GetFeedByIndex(DCD_FEED_T * list_head_ptr, const uint16  index)
{
    return GetFeedByIndex(list_head_ptr, index);
} 
/*****************************************************************************/
//    Description : 获取指定频道的指定频道项
//    Global resource dependence : none
//    Author:panvs.zhong
//    Note: 2011-07-15
/*****************************************************************************/                  
PUBLIC DCD_ENTRY_T * MMIDCD_GetEntryByIndex(xmlDocPtr doc_ptr, uint16 feedIndex, uint16 index)
{
    return GetEntryNode(doc_ptr, feedIndex, index);
} 

#endif /*#ifdef MMI_ENABLE_DCD*/
