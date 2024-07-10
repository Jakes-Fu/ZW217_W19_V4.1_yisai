/*****************************************************************************
** File Name:      mmi_resource.c                                               *
** Author:                                                                   *
** Date:           05/27/2008                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to control mmi module                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2008       gang.tong         Creat
******************************************************************************/

//nand默认使用动态加载flash字体的方式
//之前将字体全部加载到内存的方式太费内存
#include "mmi_app_kernel_trc.h"
#ifdef NANDBOOT_SUPPORT
#ifndef VECTOR_FONT_SUPPORT_ETRUMP
#define NAND_FONT_LOAD_SUPPORT
#endif
#endif

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
//#include "std_header.h"
#endif
#include "mmi_resource.h"
#include "mmi_resource_def.h"
#include "mmi_module.h" 
#include "mmi_image.h"
#include "mmi_anim.h"
#include "mmi_text.h" 
#include "mmi_ring.h"
#include "mmi_font.h"
#include "mmi_nv.h"
#include "mn_api.h"
#include "block_mem.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiset_nv.h"
#include "mmi_link.h"
#include "mmi_mem.h"
#include "mmifmm_export.h"
#include "mmk_modinfo.h"
#include "spml_api.h"
#include "mmi_modu_main.h"
#ifdef BROWSER_SUPPORT
#include "mmi_data.h"
#include "mmibrowser_export.h"
#endif
//added by andrew.zhang at 2009/09/30 for caf application resource
typedef struct _CAF_RES_T CAF_RES_T;
typedef struct _RES_NODE_DATA_T RES_NODE_DATA_T; 
#include "cafresource_internal.h"
//end 

#ifdef QBTHEME_SUPPORT
#include "mmi_qbtheme_map.h"
#include "mmiqbtheme_export.h"
#endif

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_utility.h"
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif

#include "mmi_textfun.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_MODU_OFFSET                     16
#define MMI_MODU_LOW_WORD                   0x0000ffff
#define MMI_MODU_HIGH_WORD                  0xffff0000

#define MMI_GET_MODU_ID(X)                  (uint16)(((X & ( ~MMI_SYS_RES_FLAG ) ) >> MMI_MODU_OFFSET) & MMI_MODU_LOW_WORD)

//#define MMI_FONT_RESOURCE_MAX_SIZE          (1016*1024) //这个数字从resource editor中查看font的大小而来
//#define MMI_FONT_RESOURCE_MAX_SIZE          (1200*1024) 

#define MMI_RES_MAX_RECORD_WIN_NUM          64      //方便扩展，将来可以扩展到64位
#define MMI_MODU_FONT_IDX                   1       //font模块在资源中的第二个位置，common为0

#ifdef NANDBOOT_SUPPORT
//大内存分配表
#ifdef MMI_PDA_SUPPORT      //for cr NEWMS00112625
    #if defined MAINLCD_SIZE_320X480
    #define RES_MEM_TOTAL_SIZE                  (1300 * 1024)   //BLOCK总的大小
    #elif defined MAINLCD_SIZE_240X400
    #define RES_MEM_TOTAL_SIZE                  (1000 * 1024)   //BLOCK总的大小
    #else
    #define RES_MEM_TOTAL_SIZE                  (900 * 1024)   //BLOCK总的大小
    #endif
#else
    #if defined MAINLCD_SIZE_320X480
    #define RES_MEM_TOTAL_SIZE                  (1000 * 1024)   //BLOCK总的大小
    #elif defined MAINLCD_SIZE_240X400
    #define RES_MEM_TOTAL_SIZE                  (900 * 1024)   //BLOCK总的大小
    #else
    #define RES_MEM_TOTAL_SIZE                  (700 * 1024)   //BLOCK总的大小
    #endif
#endif  //MMI_PDA_SUPPORT
#endif  //NANDBOOT_SUPPORT

#ifndef NANDBOOT_SUPPORT
	#ifdef MAINLCD_SIZE_320X480
		#define NOR_FILE_THEME_MEM_TOTAL_SIZE       (240 * 1024)   /* 在nor方案中从文件中读取资源所需的内存大小 */
	#elif defined MAINLCD_SIZE_240X400
		#define NOR_FILE_THEME_MEM_TOTAL_SIZE       (200 * 1024)
	#else
		#define NOR_FILE_THEME_MEM_TOTAL_SIZE       (160 * 1024)	
	#endif 
#endif

#define MMI_CAF_RES_MAX                     10
#define MMI_INVALID_COM_ID                  0
#define MMIRES_DEFAULT_COM_ID               1

#define MMIRES_TEXT_FLAG                    0x01

#ifdef BROWSER_SUPPORT
#define BOOKMARK_MAGIC_FLAG "BKMK"
#define BOOKMARK_MAGIC_MAGIC_OFFSET 0x00
#define BOOKMARK_MAGIC_MAGIC_LENGTH 0x04

#define BOOKMARK_VERSION_OFFSET 0x04
#define BOOKMARK_VERSION_LENGTH  0x02

#define BOOKMARK_NUM_OFFSET 0x06

#define BOOKMARK_HEADER_LENGTH 0x08

#define BOOKMARK_OFFSET_LENGTH 0x04

#define BOOKMARK_ITEM_NAME_LEN_OFFSET 0x00
#define BOOKMARK_ITEM_NAME_LEN_LENGTH 0x02

#define BOOKMARK_ITEM_URL_LENGTH  0x02

#if 1
#define CONVERT2U8TOU16(a, b) (a + ((b << 8)&0xFF00))
#define CONVERT4U8TOU32(a, b, c, d) (a + ((b << 8)&0xFF00) + ((c << 16)&0xFF0000) + ((d << 24)&0xFF000000))
#else
#define CONVERT2U8TOU16(a, b) (b + ((a << 8)&0xFF00))
#define CONVERT4U8TOU32(a, b, c, d) (d + ((c << 8)&0xFF00) + ((b << 16)&0xFF0000) + ((a << 24)&0xFF000000))
#endif
#endif

//MS_Code\make\perl_script\ResGen\ResView.ini
//MMI_RES_TEXT_ALL_UNICODE_SUPPORT---->AllUseUnicode = 1
//Not define MMI_RES_TEXT_ALL_UNICODE_SUPPORT---->AllUseUnicode = 0
#if !defined( MMI_RES_TEXT_ALL_UNICODE_SUPPORT)
#ifdef MMI_RES_LOW_COST
#define MMI_TEXT_CACHE_BUF_SIZE 1024
#else
#define MMI_TEXT_CACHE_BUF_SIZE (5 * 1024)
#endif
#endif

/*lint -e18 -e64 -e516*/
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/** 描述资源包中数据的结构 **/
/* all module offset array */
typedef struct
{
    TOOL_USED_INFO_T resource_tool_info;
    uint32 total_usedsize;                      //所有资源所用空间
    uint32 mmi_module_adrr[MMI_MAX_MODU_NUM];   //各模块在数据里的头指针
    uint8 resource_version_array[MAX_RES_VERSION_LENGTH];
    uint8 resource_name_array[MAX_RES_NAME_LENGTH];
}MMI_RESOURCE_BRIEF_T,*PMMI_RESOURCE_BRIEF_T;

/** 描述资源包中数据的结构 end **/

/* resource location define */
typedef enum
{
    RES_STORE_CAF_FILE = 0,     /* 标明资源是caf组件的资源，并存储在文件中 */
    RES_STORE_THEME_FILE,       /* 标明资源是theme资源，并存储在文件中 */
    RES_STORE_FLASH_NOR,        /* 标明资源存储在nor flash中 */
    RES_STORE_FLASH_NAND,       /* 标明资源存储在nand flash中 */
	RES_STORE_THEME_PACKAGE,    /* 标明资源是theme主题包，一般存储在T卡上*/ 
    RES_STORE_POS_MAX
}RES_STORE_POS_E;


/* every unit resource data, for example, a image, a txt */
typedef struct 
{
    uint32 data_addr;
    uint32 data_size;
    uint32 data_addition;
}RES_UNIT_INFO_T;

typedef struct 
{   
    uint32          unit_total;
    RES_UNIT_INFO_T *unit_info_arry;
}RES_ARRY_INFO_T;

/* node struct of loaded res unit */
/*  这个节点是用来记录已load资源数据的结构，这个节点会被挂在链表中。
    当有新资源申请时，先从链表中找是否已经load。 */
struct _RES_NODE_DATA_T
{
    MMI_RES_TYPE_E type;                    //资源类型 image,anim。
    uint32  resource_id ;                   //资源id。
    uint32  handle_bit_flag1;               //每一个bit表示一个窗口或者app的handle。这样才可以快速定位资源。0~32
    uint32  handle_bit_flag2;               //每一个bit表示一个窗口或者app的handle。这样才可以快速定位资源。32~64
    uint32  com_id;                         //表明这个资源是归属于哪个组件资源的。
    uint8   *data_ptr;                      //数据内容指针。
    uint32  data_type;                      //数据类型。    
    uint32  size;                           //数据大小。    
}; 

/* info of some res in a project module */
/*  记录了某个module中某类资源的某一笔数据，这类资源包含txt, color, rect。
    在nand 和 file 存储方式中，它们会根据模块划分，全部读取出来通过这个结构记录在内存中。 */
typedef struct _RES_MODULE_DATA_T
{
    uint32  module_id;                      //模块id
    uint32  size;                           //此模块该资源的大小
    uint32  offset_base;                    //offset地址的基地址
    uint8   *unit_data_ptr;                 //此模块资源数据
    uint32  *unit_offset_arry;              //此模块资源数据offset     
}RES_MODULE_DATA_T;

/* resource location info */
typedef struct 
{
    RES_STORE_POS_E  store_type;            /* 记录资源的存放设备类型 */
    wchar* res_file;                        /* 如果是文件方式，这个就是文件名 */
    uint32 res_addr;                        /* 如果是nand或者nor方式，这个就是记录在flash上的地址 */
}CAF_RES_LOCATION_T;

/* total res info */
struct _CAF_RES_T
{
    uint32                  com_id;      
    CAF_RES_LOCATION_T      res_location;               /* 记录当前资源块的存储位置 */
    uint32                  res_data_offset;            /* 记录当前资源块在资源包中的偏移量 */
    uint32                  res_module_total;           /* 资源中模块的总数 */
    uint32                  res_handle;                 /* 读取资源内容所用的文件handle, 当资源存贮在文件中时，加快读文件速度 */
    MMI_RESOURCE_BRIEF_T    *res_brief_ptr;             /* 对default资源包的简要描述 */
    MMIRES_MODULE_INFO_T    *res_module_arry;           /* 这是个资源包中，各模块信息的数组指针。 */   
    MMI_LINK_NODE_T         *color_prj_module_link_ptr;
    MMI_LINK_NODE_T         *rect_prj_module_link_ptr;
    MMI_LINK_NODE_T         *text_prj_module_link_ptr;                  
    RES_ARRY_INFO_T         *image_module_arry;     
    RES_ARRY_INFO_T         *anim_module_arry;  
    RES_ARRY_INFO_T         *ring_module_arry;
    RES_ARRY_INFO_T         *data_module_arry;
    
    //added by andrew.zhang at 2009/09/30
    //caf 资源信息单独定义
    CAF_MMI_MODULE_TOTAL_INFO_T     *caf_res_header_ptr;  //caf资源头信息
    CAF_MMIRES_MODULE_INFO_T        *caf_res_module_arry; //caf资源模块信息
    CAF_RES_COMMON_INFO_T           *caf_image_module_arry;     
    CAF_RES_COMMON_INFO_T           *caf_anim_module_arry; 
    CAF_RES_COMMON_INFO_T           *caf_ring_module_arry;          
    BOOLEAN                         is_caf_resource;

    CAF_RESOURCE_HEADER_INFO        *caf_res_theme_info_ptr;

};

/* for handle map struct */
typedef struct
{    
    uint32  handle_id;      
}RES_HANDLE_FLAG_T;

/* resource location define */
typedef enum
{
    RES_SET_LOCATION_FAILED = 0,
    RES_SET_LOCATION_SUCCEEDED,
    RES_SET_LOCATION_NEED_NOT_RES

}RES_SET_LOCATION_E;

#define MACRO_RES_PRJ_TYPE      MACRO_RES_PRJ_FONT

#include "macro_res_prj_def.h"
typedef enum MMI_LANG_FONT_E
{
    #include "mmi_res_prj_def.h"
    
    LANG_FONT_MAX_ID
} MMI_LANG_FONT_E;

#undef MACRO_RES_PRJ_TYPE

//字体和字体语言库的映射关系
// typedef struct
// {
//     GUI_FONT_T font;
//     MMI_LANG_FONT_E lang_font;
//     MMI_LANG_FONT_E ucs2_lang_font;
// }RES_LANG_FONT_MAP_T;
// 
// typedef struct
// {
//     MMISET_LANGUAGE_TYPE_E lang_type;
//     BOOLEAN is_exist;    
// }RES_LANG_FONT_STATUS_T;

typedef BOOLEAN (*GETIMAGERESDATAFUNC)(
                                       uint32 image_id, 
                                       uint8** data_pptr, 
                                       uint32* len_ptr
                                       );
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint8 *s_from_heap_ptr   = PNULL; //used for large mp4 mem malloc
LOCAL uint8 *s_mmi_res_bin_ptr = PNULL;
#ifdef NANDBOOT_SUPPORT
/* all used resource will be load to this buffer pool for application */
LOCAL uint32 s_res_pool[RES_MEM_TOTAL_SIZE / 4] = {0};
//记录最近一次的ring id，避免释放所有资源时的野指针
LOCAL uint32 s_last_ring_id = 0;
/* resource manager id */
LOCAL MPOOL_HANDLE s_nand_mem_manager_id = 0;
//LOCAL uint8 s_font_ptr[MMI_FONT_RESOURCE_MAX_SIZE] = {0};
LOCAL uint8 * s_font_ptr = PNULL;
#else
/* all used resource will be load to this buffer pool for application */
LOCAL uint32* s_nor_file_theme_pool = PNULL;
/* resource manager id */
LOCAL MPOOL_HANDLE s_nor_file_theme_mem_manager_id = 0;

LOCAL int32 s_nor_file_theme_pool_refcnt = 0;
#endif
    
#ifdef DYNAMIC_MODULE_SUPPORT
LOCAL MPOOL_HANDLE s_caf_res_mem_id = 0;
#endif

/* all dyn loaded resource link */
LOCAL MMI_LINK_NODE_T * s_allocated_res_link_head = PNULL;

/* 用来记录所有资源包信息 */
LOCAL RESOURCE_PACKAGE_INFO_T * s_package_info_ptr = PNULL;

/* all caf com resource info */
LOCAL CAF_RES_T s_caf_res_info[MMI_CAF_RES_MAX] = {0};
/* appoint theme resource info */
LOCAL CAF_RES_T * s_appoint_res_ptr = PNULL;
#ifdef MMI_RES_ORIENT_BOTH
LOCAL CAF_RES_T s_landscape_res = {0};
LOCAL CAF_RES_T *s_appoint_landscape_res_ptr = PNULL;
#endif

/* handle flag map */
LOCAL RES_HANDLE_FLAG_T s_handle_map[MMI_RES_MAX_RECORD_WIN_NUM] = {0};

#ifndef MMI_RES_TEXT_ALL_UNICODE_SUPPORT
LOCAL wchar s_text_cache_buf[MMI_TEXT_CACHE_BUF_SIZE] = {0};
LOCAL uint32 s_text_cache_offset = 0;
#endif
/*lint -save -e785 */
LOCAL const BOOLEAN s_lang_status_info[MMISET_MAX_LANGUAGE + 1][2] = 
{
#ifdef MMI_SIM_LANGUAGE_SUPPORT
    {MMISET_LANGUAGE_AUTO,FALSE},// is different with "MMISET_LANG_SET_T" define!!
#endif

    {MMISET_LANGUAGE_ENGLISH, TRUE},

#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    {MMISET_LANGUAGE_SIMP_CHINESE, TRUE},
//#else
//   {MMISET_LANGUAGE_SIMP_CHINESE, FALSE},
#endif

#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    {MMISET_LANGUAGE_TRAD_CHINESE, TRUE},
//#else
//    {MMISET_LANGUAGE_TRAD_CHINESE, FALSE},
#endif  

#ifdef IM_ARABIC_SUPPORT
    {MMISET_LANGUAGE_ARABIC, TRUE},
//#else
//   {MMISET_LANGUAGE_ARABIC, FALSE},
#endif

#ifdef IM_FRENCH_SUPPORT
    {MMISET_LANGUAGE_FRENCH, TRUE},
//#else
//    {MMISET_LANGUAGE_FRENCH, FALSE},
#endif

#ifdef IM_HINDI_SUPPORT
    {MMISET_LANGUAGE_HINDI, TRUE},
//#else
//    {MMISET_LANGUAGE_HINDI, FALSE},
#endif
   
#ifdef IM_HUNGARIAN_SUPPORT
    {MMISET_LANGUAGE_HUNGARIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_HUNGARIAN, FALSE},
#endif

#ifdef IM_INDONESIAN_SUPPORT
    {MMISET_LANGUAGE_INDONESIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_INDONESIAN, FALSE},
#endif

#ifdef IM_MALAY_SUPPORT
    {MMISET_LANGUAGE_MALAY, TRUE},
//#else
//    {MMISET_LANGUAGE_MALAY, FALSE},
#endif

#ifdef IM_PORTUGUESE_SUPPORT
    {MMISET_LANGUAGE_PORTUGUESE, TRUE},
//#else
//    {MMISET_LANGUAGE_PORTUGUESE, FALSE},
#endif

#ifdef IM_RUSSIAN_SUPPORT
    {MMISET_LANGUAGE_RUSSIAN, TRUE},
//#else
//   {MMISET_LANGUAGE_RUSSIAN, FALSE},
#endif

#ifdef IM_SPANISH_SUPPORT
    {MMISET_LANGUAGE_SPANISH, TRUE},
//#else
//    {MMISET_LANGUAGE_SPANISH, FALSE},
#endif

#ifdef IM_TAGALOG_SUPPORT
    {MMISET_LANGUAGE_TAGALOG, TRUE},
//#else
//    {MMISET_LANGUAGE_TAGALOG, FALSE},
#endif

#ifdef IM_THAI_SUPPORT
    {MMISET_LANGUAGE_THAI, TRUE},
//#else
//   {MMISET_LANGUAGE_THAI, FALSE},
#endif

#ifdef IM_VIETNAMESE_SUPPORT
    {MMISET_LANGUAGE_VIETNAMESE, TRUE},
//#else
//    {MMISET_LANGUAGE_VIETNAMESE, FALSE},
#endif

#ifdef IM_URDU_SUPPORT
    {MMISET_LANGUAGE_URDU, TRUE},
//#else
//    {MMISET_LANGUAGE_URDU, FALSE},
#endif

#ifdef IM_ITALIAN_SUPPORT
    {MMISET_LANGUAGE_ITALIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_ITALIAN, FALSE},
#endif

#ifdef IM_PERSIAN_SUPPORT
    {MMISET_LANGUAGE_PERSIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_PERSIAN, FALSE},
#endif

#ifdef IM_TURKISH_SUPPORT
    {MMISET_LANGUAGE_TURKISH, TRUE},
//#else
//    {MMISET_LANGUAGE_TURKISH, FALSE},
#endif

#ifdef IM_GERMAN_SUPPORT
    {MMISET_LANGUAGE_GERMAN, TRUE},
//#else
//    {MMISET_LANGUAGE_GERMAN, FALSE},
#endif

#ifdef IM_GREEK_SUPPORT
    {MMISET_LANGUAGE_GREEK, TRUE},
//#else
//    {MMISET_LANGUAGE_GREEK, FALSE},
#endif

#ifdef IM_HEBREW_SUPPORT
    {MMISET_LANGUAGE_HEBREW, TRUE},
//#else
//    {MMISET_LANGUAGE_HEBREW, FALSE},
#endif

#ifdef IM_BENGALI_SUPPORT
    {MMISET_LANGUAGE_BENGALI, TRUE},
//#else
//    {MMISET_LANGUAGE_BENGALI, FALSE},
#endif

#ifdef IM_CZECH_SUPPORT
    {MMISET_LANGUAGE_CZECH, TRUE},
//#else
//    {MMISET_LANGUAGE_CZECH, FALSE},
#endif

#ifdef IM_SLOVENIAN_SUPPORT
    {MMISET_LANGUAGE_SLOVENIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_SLOVENIAN, FALSE},
#endif

#ifdef IM_ROMANIAN_SUPPORT
    {MMISET_LANGUAGE_ROMANIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_ROMANIAN, FALSE},
#endif

#ifdef IM_TELUGU_SUPPORT
    {MMISET_LANGUAGE_TELUGU, TRUE},
//#else
//    {MMISET_LANGUAGE_TELUGU, FALSE},
#endif

#ifdef IM_MARATHI_SUPPORT
    {MMISET_LANGUAGE_MARATHI, TRUE},
//#else
//    {MMISET_LANGUAGE_MARATHI, FALSE},
#endif

#ifdef IM_TAMIL_SUPPORT
    {MMISET_LANGUAGE_TAMIL, TRUE},
//#else
//    {MMISET_LANGUAGE_TAMIL, FALSE},
#endif

#ifdef IM_GUJARATI_SUPPORT
    {MMISET_LANGUAGE_GUJARATI, TRUE},
//#else
//    {MMISET_LANGUAGE_GUJARATI, FALSE},
#endif

#ifdef IM_KANNADA_SUPPORT
    {MMISET_LANGUAGE_KANNADA, TRUE},
//#else
//    {MMISET_LANGUAGE_KANNADA, FALSE},
#endif

#ifdef IM_MALAYALAM_SUPPORT
    {MMISET_LANGUAGE_MALAYALAM, TRUE},
//#else
//    {MMISET_LANGUAGE_MALAYALAM, FALSE},
#endif

#ifdef IM_ORIYA_SUPPORT
    {MMISET_LANGUAGE_ORIYA, TRUE},
//#else
//    {MMISET_LANGUAGE_ORIYA, FALSE},
#endif

#ifdef IM_PUNJABI_SUPPORT
    {MMISET_LANGUAGE_PUNJABI, TRUE},
//#else
//    {MMISET_LANGUAGE_PUNJABI, FALSE},
#endif

#ifdef IM_AFRIKAANS_SUPPORT
    {MMISET_LANGUAGE_AFRIKAANS, TRUE},
//#else
//    {MMISET_LANGUAGE_AFRIKAANS, FALSE},
#endif

#ifdef IM_ALBANIAN_SUPPORT
    {MMISET_LANGUAGE_ALBANIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_ALBANIAN, FALSE},
#endif

#ifdef IM_ARMENIAN_SUPPORT
    {MMISET_LANGUAGE_ARMENIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_ARMENIAN, FALSE},
#endif

#ifdef IM_AZERBAIJANI_SUPPORT
    {MMISET_LANGUAGE_AZERBAIJANI, TRUE},
//#else
//    {MMISET_LANGUAGE_AZERBAIJANI, FALSE},
#endif

#ifdef IM_BASQUE_SUPPORT
    {MMISET_LANGUAGE_BASQUE, TRUE},
//#else
//    {MMISET_LANGUAGE_BASQUE, FALSE},
#endif

#ifdef IM_BULGARIAN_SUPPORT
    {MMISET_LANGUAGE_BULGARIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_BULGARIAN, FALSE},
#endif

#ifdef IM_CATALAN_SUPPORT
    {MMISET_LANGUAGE_CATALAN, TRUE},
//#else
//    {MMISET_LANGUAGE_CATALAN, FALSE},
#endif

#ifdef IM_CROATIAN_SUPPORT
    {MMISET_LANGUAGE_CROATIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_CROATIAN, FALSE},
#endif

#ifdef IM_DANISH_SUPPORT
    {MMISET_LANGUAGE_DANISH, TRUE},
//#else
//    {MMISET_LANGUAGE_DANISH, FALSE},
#endif

#ifdef IM_DUTCH_SUPPORT
    {MMISET_LANGUAGE_DUTCH, TRUE},
//#else
//    {MMISET_LANGUAGE_DUTCH, FALSE},
#endif

#ifdef IM_ESTONIAN_SUPPORT
    {MMISET_LANGUAGE_ESTONIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_ESTONIAN, FALSE},
#endif

#ifdef IM_FILIPINO_SUPPORT
    {MMISET_LANGUAGE_FILIPINO, TRUE},
//#else
//    {MMISET_LANGUAGE_FILIPINO, FALSE},
#endif

#ifdef IM_FINNISH_SUPPORT
    {MMISET_LANGUAGE_FINNISH, TRUE},
//#else
//    {MMISET_LANGUAGE_FINNISH, FALSE},
#endif

#ifdef IM_GALICIAN_SUPPORT
    {MMISET_LANGUAGE_GALICIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_GALICIAN, FALSE},
#endif

#ifdef IM_GEORGIAN_SUPPORT
    {MMISET_LANGUAGE_GEORGIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_GEORGIAN, FALSE},
#endif

#ifdef IM_HAUSA_SUPPORT
    {MMISET_LANGUAGE_HAUSA, TRUE},
//#else
//    {MMISET_LANGUAGE_HAUSA, FALSE},
#endif

#ifdef IM_ICELANDIC_SUPPORT
    {MMISET_LANGUAGE_ICELANDIC, TRUE},
//#else
//    {MMISET_LANGUAGE_ICELANDIC, FALSE},
#endif

#ifdef IM_IGBO_SUPPORT
    {MMISET_LANGUAGE_IGBO, TRUE},
//#else
//    {MMISET_LANGUAGE_IGBO, FALSE},
#endif

#ifdef IM_IRISH_SUPPORT
    {MMISET_LANGUAGE_IRISH, TRUE},
//#else
//    {MMISET_LANGUAGE_IRISH, FALSE},
#endif

#ifdef IM_KAZAKH_SUPPORT
    {MMISET_LANGUAGE_KAZAKH, TRUE},
//#else
//    {MMISET_LANGUAGE_KAZAKH, FALSE},
#endif

#ifdef IM_LATVIAN_SUPPORT
    {MMISET_LANGUAGE_LATVIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_LATVIAN, FALSE},
#endif

#ifdef IM_LITHUANIAN_SUPPORT
    {MMISET_LANGUAGE_LITHUANIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_LITHUANIAN, FALSE},
#endif

#ifdef IM_MACEDONIAN_SUPPORT
    {MMISET_LANGUAGE_MACEDONIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_MACEDONIAN, FALSE},
#endif

#ifdef IM_MOLDOVAN_SUPPORT
    {MMISET_LANGUAGE_MOLDOVAN, TRUE},
//#else
//    {MMISET_LANGUAGE_MOLDOVAN, FALSE},
#endif

#ifdef IM_NORWEGIAN_SUPPORT
    {MMISET_LANGUAGE_NORWEGIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_NORWEGIAN, FALSE},
#endif

#ifdef IM_POLISH_SUPPORT
    {MMISET_LANGUAGE_POLISH, TRUE},
//#else
//    {MMISET_LANGUAGE_POLISH, FALSE},
#endif

#ifdef IM_SERBIAN_SUPPORT
    {MMISET_LANGUAGE_SERBIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_SERBIAN, FALSE},
#endif

#ifdef IM_SESOTHO_SUPPORT
    {MMISET_LANGUAGE_SESOTHO, TRUE},
//#else
//    {MMISET_LANGUAGE_SESOTHO, FALSE},
#endif

#ifdef IM_SLOVAK_SUPPORT
    {MMISET_LANGUAGE_SLOVAK, TRUE},
//#else
//    {MMISET_LANGUAGE_SLOVAK, FALSE},
#endif

#ifdef IM_SWEDISH_SUPPORT
    {MMISET_LANGUAGE_SWEDISH, TRUE},
//#else
//    {MMISET_LANGUAGE_SWEDISH, FALSE},
#endif

#ifdef IM_UKRAINIAN_SUPPORT
    {MMISET_LANGUAGE_UKRAINIAN, TRUE},
//#else
//    {MMISET_LANGUAGE_UKRAINIAN, FALSE},
#endif

#ifdef IM_YORUBA_SUPPORT
    {MMISET_LANGUAGE_YORUBA, TRUE},
//#else
//    {MMISET_LANGUAGE_YORUBA, FALSE},
#endif

//科萨语    add. chenyg@spread. 2011-05-12
#ifdef IM_XHOSA_SUPPORT
    {MMISET_LANGUAGE_XHOSA, TRUE},
//#else
//    {MMISET_LANGUAGE_XHOSA, FALSE},
#endif

//祖鲁语    add. chenyg@spread. 2011-05-12
#ifdef IM_ZULU_SUPPORT
    {MMISET_LANGUAGE_ZULU, TRUE},
//#else
//    {MMISET_LANGUAGE_ZULU, FALSE},
#endif

//(印度语支的)阿萨姆语  add. chenyg@spread. 2011-05-12
#ifdef IM_ASSAMESE_SUPPORT
    {MMISET_LANGUAGE_ASSAMESE, TRUE},
//#else
//    {MMISET_LANGUAGE_ASSAMESE, FALSE},
#endif

//斯瓦西里  add. chenyg@spread. 2011-05-12
#ifdef IM_SWAHILI_SUPPORT
    {MMISET_LANGUAGE_SWAHILI, TRUE},
//#else
//    {MMISET_LANGUAGE_SWAHILI, FALSE},
#endif

#ifdef IM_MYANMAR_SUPPORT
    {MMISET_LANGUAGE_MYANMAR, TRUE},
//#else
//    {MMISET_LANGUAGE_MYANMAR, FALSE},
#endif

#ifdef IM_AMHARIC_SUPPORT
    {MMISET_LANGUAGE_AMHARIC, TRUE},
//#else
//    {MMISET_LANGUAGE_AMHARIC, FALSE},
#endif

#ifdef IM_KHMER_SUPPORT
    {MMISET_LANGUAGE_KHMER, TRUE},
//#else
//    {MMISET_LANGUAGE_KHMER, FALSE},
#endif

#ifdef IM_LAO_SUPPORT
    {MMISET_LANGUAGE_LAO, TRUE},
//#else
//    {MMISET_LANGUAGE_LAO, FALSE},
#endif

#ifdef IM_UYGHUR_SUPPORT
    {MMISET_LANGUAGE_UYGHUR, TRUE},
//#else
//    {MMISET_LANGUAGE_UYGHUR, FALSE},
#endif
	
#ifdef IM_TIBETAN_SUPPORT
    {MMISET_LANGUAGE_TIBETAN, TRUE},
//#else
//    {MMISET_LANGUAGE_TIBETAN, FALSE},
#endif
	
#ifdef IM_SINHALESE_SUPPORT
    {MMISET_LANGUAGE_SINHALESE, TRUE},
//#else
//    {MMISET_LANGUAGE_SINHALESE, FALSE},
#endif

    {0 , 0}
};
/*lint -restore */
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern BOOLEAN ResParam_Read(void *buf, uint32 offset, uint32 size);
#ifndef NANDBOOT_SUPPORT 
extern uint8 * PROD_GetResMemAdd (void);
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef MMI_SIM_LANGUAGE_SUPPORT
PUBLIC uint8 g_ksp_Lang = 1; //0--auto, 1 -- english
#else
PUBLIC uint8 g_ksp_Lang = 0;
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get langfont from font and is_ucs2
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
//LOCAL MMI_LANG_FONT_E GetLangFont(
//                                  GUI_FONT_T font,
//                                  BOOLEAN is_ucs2     // 1: a gb char  0:an ascii char
//                                  );

/*****************************************************************************/
//  Description : get the flag bit location of handle
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL CAF_RES_T* GetComCafResInfo(uint32 com_id);

/*****************************************************************************/
//  Description : read  resource to buffer 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void ReadResource( 
                        SFS_HANDLE sfs_handle,
                        void * buffer,
                        uint32 offset,
                        uint32 size,
                        RES_STORE_POS_E store_pos
                        );

/*****************************************************************************/
//  Description : open resource to buffer 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL SFS_HANDLE OpenResource( 
                              RES_STORE_POS_E store_pos,
                              const uint16 * res_file
                              );

/*****************************************************************************/
//  Description : close  resource to buffer 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void CloseResource( 
                         RES_STORE_POS_E store_pos,
                         SFS_HANDLE sfs_handle
                         );

/*****************************************************************************/
//  Description : print res table info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void PrintfRESTabInfo(void);

/*****************************************************************************/
//  Description : get animation data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL uint32 GetComIdByHandle( CAF_HANDLE_T handle_id );

/*****************************************************************************/
//  Description : update theme resource
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void UpdateStaticThemeRes(uint8 theme_id);

/*****************************************************************************/
//  Description : get lang index from land status info table
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint32 GetCurLangIndex(uint8 lang);
/*****************************************************************************/
//  Description : Free dynamic resource node of indicated com id from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void FreeComNodeRes(
                          uint32 com_id,
                          BOOLEAN is_lcd_switch //是否横竖屏切换, 是则只清除图片资源
                          );

/*****************************************************************************/
//  Description : load font resource to ram 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void LoadFontRes(const CAF_RES_T * pRes);
/*****************************************************************************/
//  Description : load packages info
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void GetFlashPackagesInfo(void);
/*****************************************************************************/
//  Description : load com resource to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadResourceData(CAF_RES_T * res_info_ptr);
/*****************************************************************************/
//  Description : load rect resource to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void LoadRectRes(CAF_RES_T *pRes );

/*****************************************************************************/
//  Description : load color resource to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void LoadColorRes(CAF_RES_T *pRes );
/*****************************************************************************/
//  Description : load text resource to ram 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void LoadTextRes(CAF_RES_T *pRes );
/*****************************************************************************/
//  Description : to get the raw res data by label
//  Global resource dependence : 
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GetRawResData(
                             MMI_RES_TYPE_E res_type,
                             uint32 res_id,
                             CAF_HANDLE_T handle_id,
                             void * data_ptr
                             );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:分配内存
/*****************************************************************************/
LOCAL void* MMIRES_MemPoolMalloc( 
                                 uint32 handle, 
                                 uint32 size
                                 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:释放内存
/*****************************************************************************/
LOCAL void MMIRES_MemPoolFree( void* mem_ptr );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:重置内存
/*****************************************************************************/
LOCAL uint32 MMIRES_MemPoolConfig( 
                                  SPML_CACHE_TYPE_E type,
                                  uint32 handle,
                                  BOOLEAN is_create
                                  );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:读文件
/*****************************************************************************/
LOCAL BOOLEAN MMIRES_GetFontData( 
                                 SPML_FONT_STORE_E store, 
                                 uint32 handle,
                                 void*  data_ptr,
                                 uint32 offset, 
                                 uint32 size
                                 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:如果存在阿拉伯语或希伯来语, 返回true
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_IsNeedBidi( void );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:获得字体库个数
/*****************************************************************************/
PUBLIC uint16 MMIRES_GetFontLibNum( void );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:获得字体库的指针
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetFontLibPtr(
                                   uint16 index,
                                   uint8** lib_ptr, 
                                   uint32* lib_size
                                   );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:根据当前语言获得OTF语言标签
/*****************************************************************************/
LOCAL uint32 MMIRES_GetFontLangTag( void );

/*****************************************************************************/
//  Description : get current theme number
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint8 GetStaticThemeTotalNum(void);

/*****************************************************************************/
//  Description : Get Ascii Res Text
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetAsciiResText(MMI_STRING_T *str_ptr, uint8 *re_str_ptr);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                  */
/*---------------------------------------------------------------------------*/

#ifdef FONT_TYPE_SUPPORT_VECTOR
#ifndef VECTOR_FONT_SUPPORT_ETRUMP
//显示缓冲区
#define VECTOR_BUFFER_SIZE 16800

#ifndef VECTOR_NO_HINTER_SUPPORT
#ifdef NAND_FONT_LOAD_SUPPORT
#define ADVANCE_CACHE_SIZE (32*1024)
#else 
#define ADVANCE_CACHE_SIZE (16*1024)
#endif
#else
#define ADVANCE_CACHE_SIZE  0
#endif

#ifndef RES_VECTOR_CACHE_SUPPORT
#define VECTOR_CACHE_SIZE  0
#else
#ifdef NAND_FONT_LOAD_SUPPORT
//字体缓冲区
#define VECTOR_CACHE_SIZE    (128*1024)
#else
//字体缓冲区
#define VECTOR_CACHE_SIZE    (64*1024)
#endif
#endif

#else //else of VECTOR_FONT_SUPPORT_ETRUMP
#define VECTOR_BUFFER_SIZE 0
#define VECTOR_CACHE_SIZE  0
#define ADVANCE_CACHE_SIZE  0
#endif

#else//else of FONT_TYPE_SUPPORT_VECTOR
#define VECTOR_BUFFER_SIZE 0
#define VECTOR_CACHE_SIZE  0
#endif//end of FONT_TYPE_SUPPORT_VECTOR

#ifdef COMPLEX_TEXT_SCRIPT
//字形缓冲区, 对复杂语言起效
#ifndef VECTOR_FONT_SUPPORT_ETRUMP
#if ((defined IM_ARABIC_SUPPORT ||defined IM_HINDI_SUPPORT)&&defined MMI_GLYPH_CACHE_MINI_SUPPORT)
#define GLYPH_CACHE_SIZE (5*1024) 
#else
#define GLYPH_CACHE_SIZE 0
#endif
#else
#define GLYPH_CACHE_SIZE 0
#endif
#else
#define GLYPH_CACHE_SIZE 0
#endif

//矢量字体显示至少需要16k的缓冲区, 最大能显示64号字体
//如需要显示96号或128号字体, 需要按比例调大缓冲区
//96号推荐24k, 128号推荐32k, 工具端也需要做相应的修改
#if (VECTOR_BUFFER_SIZE > 0)
LOCAL uint8 s_vector_buffer[VECTOR_BUFFER_SIZE] = {0};
#endif

#if (VECTOR_CACHE_SIZE > 0)
LOCAL uint8 s_vector_cache[VECTOR_CACHE_SIZE] = {0};
#endif

#if (GLYPH_CACHE_SIZE > 0)
LOCAL uint8 s_glyph_cache[GLYPH_CACHE_SIZE] = {0};
#endif

#if (ADVANCE_CACHE_SIZE > 0)
LOCAL uint8 s_advance_cache[ADVANCE_CACHE_SIZE] = {0};
#endif

#ifndef RES_STORE_CACHE_SUPPORT
#define STORE_CACHE_SIZE  0
#else
#ifdef NAND_FONT_LOAD_SUPPORT
//文件或nand缓冲区, 加载nand font时由于省了内存, 可适当调大缓冲区
#define STORE_CACHE_SIZE    (128*1024)
#else
#ifdef FILE_FONT_SUPPORT
//文件或nand缓冲区
#define STORE_CACHE_SIZE    (64*1024)
#else
#define STORE_CACHE_SIZE    0
#endif
#endif
#endif

#if (STORE_CACHE_SIZE > 0)
//LOCAL uint8 s_store_cache[STORE_CACHE_SIZE] = {0};
LOCAL uint8* s_store_cache = PNULL;
#endif

uint32 s_font_file_handle = 0;

LOCAL const SPML_INIT_DATA_T s_spml_init_data =
{
    MMIRES_MemPoolMalloc,
    MMIRES_MemPoolFree,
    MMIRES_MemPoolConfig,
    MMIRES_GetFontData,
    MMIRES_IsNeedBidi,
    MMIRES_GetFontLibNum,
    MMIRES_GetFontLibPtr,
    MMIRES_GetFontLangTag,

#if (VECTOR_BUFFER_SIZE > 0)
    s_vector_buffer,
#else
    PNULL,
#endif

    VECTOR_BUFFER_SIZE,

//nand ram不够, 可以使用动态读nand font的方案
#if defined NANDBOOT_SUPPORT && defined NAND_FONT_LOAD_SUPPORT
    SPML_FONT_STORE_NAND
#else
    SPML_FONT_STORE_DEFAULT
#endif
};

//added by andrew.zhang at 2009/09/30 for caf application resource
LOCAL BOOLEAN FoundModuleIDCallback(MMI_LINK_NODE_T const* node_ptr, uint32 condition, uint32 type);
LOCAL void ResFree(
                   uint8* pointer
                   );
LOCAL uint8 * ResMalloc(
                        RES_STORE_POS_E type,
                        uint32 size
                        );
LOCAL RES_NODE_DATA_T* GetResNode(const CAF_RES_T * com_res_info_ptr, uint32 res_id, CAF_HANDLE_T handle_id, MMI_RES_TYPE_E res_type);

#include "cafresource_internal.c"

#define RES_FREE(_MEM_PTR)  \
    ResFree(_MEM_PTR);  (_MEM_PTR) = PNULL;
//end

/*****************************************************************************/
//  Description : condition for find
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FoundModuleIDCallback(MMI_LINK_NODE_T const* node_ptr, uint32 condition, uint32 type)
{
    RES_MODULE_DATA_T* prj_module_ptr = PNULL;
    prj_module_ptr = (RES_MODULE_DATA_T*)(node_ptr->data);
    /* whether the handle flag contain the handle bit */
    return (BOOLEAN)(prj_module_ptr->module_id == condition);
}

/*****************************************************************************/
//  Description : 获得存储在nand flash上的模块信息的存储地址
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL uint32 GetNandModuleInfoDataAddr(MMI_LINK_NODE_T * module_header_ptr, uint16 module_id)
{    
    MMI_LINK_NODE_T * module_link_node_ptr = PNULL;        
    if(PNULL == module_header_ptr)
    {
        return 0;
    }        
    module_link_node_ptr = MMILINK_SearchNode(module_header_ptr, TRUE, FoundModuleIDCallback, module_id, 0);
    if(module_link_node_ptr != PNULL)
    {
        //SCI_ASSERT(module_link_node_ptr != PNULL);    
        return module_link_node_ptr->data;   
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
//  Description : 获得module Info 在资源bin中的绝对偏移地址
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL uint32 GetModuleInfoAbsOffset(CAF_RES_T const *pRes, uint16 module_id)
{
    uint32 module_offset = 0;
    SCI_ASSERT(PNULL != pRes); /*assert verified*/
    if(RES_STORE_FLASH_NOR != pRes->res_location.store_type)
    {    
        module_offset = pRes->res_data_offset + pRes->res_brief_ptr->mmi_module_adrr[module_id];
    }
    else
    {        
        uint32 resource_brief_offset  = pRes->res_data_offset;
        MMI_RESOURCE_BRIEF_T * resource_brief_ptr = (MMI_RESOURCE_BRIEF_T*)(resource_brief_offset + s_mmi_res_bin_ptr);        
        module_offset = (uint32)(resource_brief_ptr->mmi_module_adrr[module_id] + resource_brief_offset);    
    }
    return module_offset;
}
/*****************************************************************************/
//  Interface:      IfTextResHasFlag
//  Description :   IfTextResHasFlag
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN IfTextResHasFlag(CAF_RES_T const* pRes)
{
    uint8 flag = 0;
    
    SCI_ASSERT(PNULL != pRes);  /*assert verified*/
    if(RES_STORE_FLASH_NOR != pRes->res_location.store_type)
    {
        flag = pRes->res_brief_ptr->resource_tool_info.res_flag;
    }
    else
    {
        MMI_RESOURCE_BRIEF_T * resource_brief_ptr = (MMI_RESOURCE_BRIEF_T*)(pRes->res_data_offset + s_mmi_res_bin_ptr);
        flag = resource_brief_ptr->resource_tool_info.res_flag;
    }
    
    flag = flag & MMIRES_TEXT_FLAG; 
    
    return (flag != MMIRES_TEXT_FLAG);
}

/*****************************************************************************/
//  Description : 获得存储在Nand或者file上的模块信息
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL MMIRES_MODULE_INFO_T * GetNandModuleInfo(const CAF_RES_T *pRes, uint16 module_id)
{
    PMMIRES_MODULE_INFO_T module_info_ptr = PNULL;
    module_info_ptr = &(pRes->res_module_arry[module_id]);
    return module_info_ptr;
}

/*****************************************************************************/
//  Description : 获得存储在nor flash上的模块信息
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL MMIRES_MODULE_INFO_T * GetNorModuleInfo(const CAF_RES_T *pRes, uint16 module_id)
{        
    uint32 offset = GetModuleInfoAbsOffset(pRes, module_id);
    PMMIRES_MODULE_INFO_T module_info_ptr = (PMMIRES_MODULE_INFO_T)(s_mmi_res_bin_ptr + offset);        
    return module_info_ptr;
}

/*****************************************************************************/
//  Description : 获得模块信息
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL uint32 GetModuleDataAddr(const CAF_RES_T*  com_res_info_ptr, MMI_RES_TYPE_E res_type, uint16 module_id)
{    
    uint32 data_addr = 0;               
 
    SCI_ASSERT(PNULL != com_res_info_ptr); /*assert verified*/

    if(RES_STORE_FLASH_NOR != com_res_info_ptr->res_location.store_type)
    {
        if(MMI_RT_TEXT == res_type)
        {           
            data_addr = GetNandModuleInfoDataAddr(com_res_info_ptr->text_prj_module_link_ptr, module_id);    
        }
        else if(MMI_RT_RECT == res_type)
        {           
            data_addr = GetNandModuleInfoDataAddr(com_res_info_ptr->rect_prj_module_link_ptr, module_id);    
        }
        else if(MMI_RT_COLOR == res_type)
        {           
            data_addr = GetNandModuleInfoDataAddr(com_res_info_ptr->color_prj_module_link_ptr, module_id);    
        }
        else if(MMI_RT_IMAGE == res_type)
        {
            data_addr = (uint32)&(com_res_info_ptr->image_module_arry[module_id]);            
        }       
        else if(MMI_RT_ANIM == res_type)
        {
            data_addr = (uint32)&(com_res_info_ptr->anim_module_arry[module_id]);
        }
        else if(MMI_RT_RING == res_type)
        {
            data_addr = (uint32)&(com_res_info_ptr->ring_module_arry[module_id]);
//            MMIRES_MODULE_INFO_T * module_info_ptr = GetNandModuleInfo(com_res_info_ptr, module_id);
//            data_addr = (uint32)module_info_ptr->mmi_data_table[MMI_RT_RING].pData + GetModuleInfoAbsOffset(com_res_info_ptr, module_id);
        }
        else if(MMI_RT_DATA == res_type)
        {
            data_addr = (uint32)&(com_res_info_ptr->data_module_arry[module_id]);
        }
    }
    else
    {
        MMIRES_MODULE_INFO_T * module_info_ptr = GetNorModuleInfo(com_res_info_ptr, module_id);
        if(module_info_ptr->mmi_data_table[res_type].uCount != 0)
        {
            data_addr = ((uint32)module_info_ptr->mmi_data_table[res_type].pData + (uint32)module_info_ptr);        
        }
    }

    return data_addr;
}

/*****************************************************************************/
//  Description : 判断module 是否为空
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsModuleInfoEmpty(CAF_RES_T const *pRes, uint16 module_id)
{
    return (0 == pRes->res_brief_ptr->mmi_module_adrr[module_id]);
}



/*****************************************************************************/
//  Description : get rect value by Label
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetRect(const CAF_RES_T * com_res_info_ptr, MMI_RECT_ID_T rect_id, GUI_RECT_T * res_rect_ptr)
{    
    BOOLEAN ret = FALSE;
    uint16 module_id = MMI_GET_MODU_ID(rect_id);
    uint16 rect_index = MMICOM_GET_WORD_LB(rect_id);
    uint32 data_addr = 0;
    RES_MODULE_DATA_T * module_data_ptr = PNULL;
    MMIRES_RECT_T * rect_array_ptr = PNULL;                

    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(0 != rect_index);     /*assert verified*/

    SCI_ASSERT(PNULL != com_res_info_ptr); /*assert verified*/

    data_addr = GetModuleDataAddr(com_res_info_ptr, MMI_RT_RECT, module_id);   
    if(data_addr != 0)
    {
        if(RES_STORE_FLASH_NOR != com_res_info_ptr->res_location.store_type)
        {      
            module_data_ptr = (RES_MODULE_DATA_T*)data_addr;        
            rect_array_ptr = (MMIRES_RECT_T*)module_data_ptr->unit_data_ptr;
        }         
        else
        {
            rect_array_ptr = (MMIRES_RECT_T*)data_addr;
        }

        if(0 != rect_array_ptr[rect_index-1].validate)
        {
            res_rect_ptr->left = rect_array_ptr[rect_index-1].left;                
            res_rect_ptr->top = rect_array_ptr[rect_index-1].top;
            res_rect_ptr->right = rect_array_ptr[rect_index-1].right;
            res_rect_ptr->bottom = rect_array_ptr[rect_index-1].bottom;
            ret = TRUE;
        }        
    }
    return ret;
}

/*****************************************************************************/
//  Description : get color value  by Label
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetColor(const CAF_RES_T*  com_res_info_ptr, MMI_COLOR_ID_T color_id, uint16* color_ptr)
{    
    BOOLEAN ret = FALSE;
    uint16 module_id = MMI_GET_MODU_ID(color_id);
    uint16 color_index = MMICOM_GET_WORD_LB(color_id);    
    uint32 data_addr = 0;
    RES_MODULE_DATA_T * module_data_ptr = PNULL;
    MMIRES_COLOR_T * color_array_ptr = PNULL;
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(0 != color_index);     /*assert verified*/
    
    SCI_ASSERT(PNULL != com_res_info_ptr); /*assert verified*/

    data_addr = GetModuleDataAddr(com_res_info_ptr, MMI_RT_COLOR, module_id);            
    if(data_addr != 0)
    {
        if(RES_STORE_FLASH_NOR != com_res_info_ptr->res_location.store_type)
        {            
            module_data_ptr = (RES_MODULE_DATA_T*)data_addr;
            color_array_ptr = (MMIRES_COLOR_T*)module_data_ptr->unit_data_ptr;            
        }
        else
        {
            
            color_array_ptr = (MMIRES_COLOR_T*)data_addr;
        }   
        {
            //uint32 *p = (uint32 *)(&(color_array_ptr[color_index-1]));
            //SCI_PASSERT(0, ("%x,%x", *p, *(p+1)));
        }
        if(0 != color_array_ptr[color_index-1].validate)
        {
            *color_ptr = (uint16)color_array_ptr[color_index-1].color;                        
            ret = TRUE;
        }        
    }
    return ret;
}

/*****************************************************************************/
//  Description : get Text ptr  by Label and lang mode
//  Global resource dependence : none
//  Author: wancan.you
//  Note:Modify by jian.ma on 07/01/11
/*****************************************************************************/
LOCAL uint8* GetText(const CAF_RES_T * com_res_info_ptr, MMI_TEXT_ID_T text_id, uint32 lang_code)
{
    uint16 module_id = MMI_GET_MODU_ID(text_id);
    uint16 text_index = MMICOM_GET_WORD_LB(text_id);
    uint32 data_addr = 0;    
    uint8* ret_ptr = PNULL;
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(0 != text_index); /*assert verified*/
    
    SCI_ASSERT(PNULL != com_res_info_ptr); /*assert verified*/

    data_addr = GetModuleDataAddr(com_res_info_ptr, MMI_RT_TEXT, module_id);
    if(data_addr != 0)
    {
        if(RES_STORE_FLASH_NOR != com_res_info_ptr->res_location.store_type)
        {            
            RES_MODULE_DATA_T* prj_module_ptr = PNULL;
            uint32 offset = 0;
            prj_module_ptr = (RES_MODULE_DATA_T*)data_addr;
            if(0 != prj_module_ptr->unit_offset_arry[text_index-1])
            {
                offset = prj_module_ptr->unit_offset_arry[text_index-1] - prj_module_ptr->offset_base;
                ret_ptr = (uint8 *)(prj_module_ptr->unit_data_ptr + offset);            
            }
        }
        else
        {           
            PMMIRES_MODULE_INFO_T module_info_ptr = GetNorModuleInfo(com_res_info_ptr, module_id);            
            PKSP_TEXT_T  txt_table_ptr = (PKSP_TEXT_T)data_addr;        
            uint32 * txt_data_ptr = (uint32*)((uint32)txt_table_ptr[lang_code].pTxtTable + (uint32)module_info_ptr);                
            if(0 != txt_data_ptr[text_index - 1])
            {
                ret_ptr = (uint8 *)(txt_data_ptr[text_index - 1] + (uint32)module_info_ptr);        
            }
        }
    }
    return ret_ptr;
}

/*****************************************************************************/
//  Description : load font resource to ram 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void LoadFontRes(const CAF_RES_T * pRes )
{
#ifdef NANDBOOT_SUPPORT   
    if(RES_STORE_FLASH_NAND == pRes->res_location.store_type)
    {    

        uint32 num = pRes->res_module_arry[MMI_MODU_FONT_IDX].mmi_data_table[MMI_RT_FONT].uCount;
        uint32 size = pRes->res_module_arry[MMI_MODU_FONT_IDX].mmi_data_table[MMI_RT_FONT].uSize;  
        uint32 font_offset = (uint32)pRes->res_module_arry[MMI_MODU_FONT_IDX].mmi_data_table[MMI_RT_FONT].pData;
        uint32 offset = GetModuleInfoAbsOffset(pRes, MMI_MODU_FONT_IDX) + font_offset;

        if( PNULL == s_font_ptr
            && 0 != size
            && 0 != num )
        {
#ifdef NAND_FONT_LOAD_SUPPORT
            //提前准备好font的数据
            uint32 i = 0;
            uint32 temp = 0;
            GUI_FONT_INFO_EX_T* info_ptr = SCI_ALLOC_APP( num * sizeof(GUI_FONT_INFO_EX_T) );
            GUI_FONT_DB_T* db_ptr = SCI_ALLOC_APP( num * sizeof(GUI_FONT_DB_T) );
            
            ReadResource( pRes->res_handle, info_ptr, offset, num * sizeof(GUI_FONT_INFO_EX_T), pRes->res_location.store_type ); 

            s_font_ptr = (uint8*)db_ptr;
            
            for ( i = 0; i < num; i++ )
            {
                temp = (uint32)info_ptr[i].pdbtt - font_offset;

                ReadResource( pRes->res_handle, &db_ptr[i], offset + temp, sizeof(GUI_FONT_DB_T), pRes->res_location.store_type );

                db_ptr[i].pdb = (uint8*)(offset + (uint32)db_ptr[i].pdb - font_offset);
            }

            SCI_FREE( info_ptr );
#else
            s_font_ptr = SCI_ALLOC_APP( size );
            
            ReadResource( pRes->res_handle, s_font_ptr, offset, size, pRes->res_location.store_type );
#endif
        }
    }
#endif
}


/*****************************************************************************/
//  Description : read  resource from file
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void ReadFileResource(
                            SFS_HANDLE sfs_handle, 
                            void * buffer,
                            uint32 offset,
                            uint32 size
                            )
{    
    BOOLEAN result = FALSE;
    uint32 read_size = 0;

    SCI_ASSERT( sfs_handle != 0 ); /*assert verified*/
    
    if( SFS_ERROR_NONE == SFS_SetFilePointer( sfs_handle, (int64)offset, SFS_SEEK_BEGIN ) )
    {        
        if( SFS_ERROR_NONE == SFS_ReadFile( sfs_handle, buffer, size, &read_size, NULL ) )
        {
            result = TRUE;
        }
    }

    SCI_ASSERT( result ); /*assert verified*/
}

/*****************************************************************************/
//  Description : open  resource from file
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL SFS_HANDLE OpenFileResource(
                                  const uint16 * res_file
                                  )
{
    SFS_HANDLE sfs_handle = SFS_CreateFile( res_file, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL );

    SCI_ASSERT( sfs_handle ); /*assert verified*/

    return sfs_handle;
}

/*****************************************************************************/
//  Description : close  resource from file
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void CloseFileResource(
                             SFS_HANDLE sfs_handle
                             )
{
    SCI_ASSERT( 0 != sfs_handle ); /*assert verified*/

    if( SFS_ERROR_NONE != SFS_CloseFile( sfs_handle ) )
    {
        SCI_ASSERT( FALSE ); /*assert verified*/
    }
}

/*****************************************************************************/
//  Description : read  resource from flash
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadFlashResource(void * buffer,uint32 offset, uint32 size)
{
    BOOLEAN result = TRUE;

#ifdef WIN32
    SCI_MEMCPY(buffer, (void *)((uint32)s_mmi_res_bin_ptr + offset),size);
#else
    //增加返回值, 有小概率会读取失败
    result = ResParam_Read(buffer,offset,size);
#endif

    return result;
}

/*****************************************************************************/
//  Description : read  resource from flash
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void ReadNorFlashResource(void * buffer,uint32 offset, uint32 size)
{
    SCI_MEMCPY(buffer, (void *)((uint32)s_mmi_res_bin_ptr + offset),size);
}

/*****************************************************************************/
//  Description : read  resource to buffer 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void ReadResource( 
                        SFS_HANDLE sfs_handle, 
                        void * buffer,
                        uint32 offset,
                        uint32 size,
                        RES_STORE_POS_E store_pos
                        )
{
    switch(store_pos)
    {
    case RES_STORE_CAF_FILE:
    case RES_STORE_THEME_FILE:
        ReadFileResource( sfs_handle, buffer, offset, size );
        break;
    case RES_STORE_FLASH_NOR:
        ReadNorFlashResource(buffer, offset, size);
        break;
    case RES_STORE_FLASH_NAND:
        ReadFlashResource( buffer, offset, size );
        break;
	case RES_STORE_THEME_PACKAGE:
		break;
    default:
        SCI_ASSERT(0); /*assert verified*/
        break;
    }
}

/*****************************************************************************/
//  Description : read  resource to buffer 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL SFS_HANDLE OpenResource( 
                              RES_STORE_POS_E store_pos,
                              const uint16 * res_file
                              )
{
    SFS_HANDLE sfs_handle = 0;

    switch(store_pos)
    {
    case RES_STORE_CAF_FILE:
    case RES_STORE_THEME_FILE:
        sfs_handle = OpenFileResource( res_file );
        break;
    case RES_STORE_FLASH_NOR:
    case RES_STORE_FLASH_NAND:
    case RES_STORE_THEME_PACKAGE:
        break;
    default:
        SCI_ASSERT(0); /*assert verified*/
        break;
    }

    return sfs_handle;
}

/*****************************************************************************/
//  Description : read  resource to buffer 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void CloseResource( 
                         RES_STORE_POS_E store_pos,
                         SFS_HANDLE sfs_handle
                         )
{
    switch(store_pos)
    {
    case RES_STORE_CAF_FILE:
    case RES_STORE_THEME_FILE:
        CloseFileResource( sfs_handle );
        break;
    case RES_STORE_FLASH_NOR:
    case RES_STORE_FLASH_NAND:
	case RES_STORE_THEME_PACKAGE:
        break;
    default:
        SCI_ASSERT(0); /*assert verified*/
        break;
    }
}
/*****************************************************************************/
//  Description : Get D link head ptr
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T * GetDLinkHeadPtr(void)
{
    return s_allocated_res_link_head ;
}

/*****************************************************************************/
//  Description : printf win map info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:for test
/*****************************************************************************/
// LOCAL void PrintfWinMapInfo(void)
// {
//     uint32 i = 0;
//     for (; i < MMI_RES_MAX_RECORD_WIN_NUM;i++) 
//     {
//         //SCI_TRACE_LOW("PrintfWinMapInfo s_win_info[%d] win_id= %d",i,s_win_info[i].win_id);
//     }
// }
/*****************************************************************************/
//  Description : free resource memory
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void ResFree(
                   uint8* pointer
                   )
{        
    /* free resource memory */
    if (PNULL != s_from_heap_ptr && pointer == s_from_heap_ptr)
    {
        SCI_FREE(pointer);
        s_from_heap_ptr = PNULL;
    }
    else
    {
         SCI_MPFREE(pointer);
    }
}
/*****************************************************************************/
//  Description:   destroy text link call back 
//  Global resource dependence: 
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void TextDestructCallback(MMI_LINK_NODE_T const * node_ptr)
{
    RES_MODULE_DATA_T * module_ptr = PNULL;
    module_ptr = (RES_MODULE_DATA_T*)(node_ptr->data);
    /* destruct data */
    RES_FREE(module_ptr->unit_data_ptr); 
    RES_FREE(module_ptr->unit_offset_arry); /*lint !e63*/
    RES_FREE(module_ptr); /*lint !e63*/
}
/*****************************************************************************/
//  Description : print res table info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void PrintfWinTabInfo(void)
{
    uint32 i =0;    
    for (; i< MMI_RES_MAX_RECORD_WIN_NUM;i++)
    {
        //SCI_TRACE_LOW:"[RES]: s_handle_map[%d].handle_id = 0x%x = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_1711_112_2_18_2_0_16_233,(uint8*)"ddd", i, s_handle_map[i].handle_id, s_handle_map[i].handle_id);
    }
}

/*****************************************************************************/
//  Description : print res table info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PrintfNodeCallback(MMI_LINK_NODE_T const* node_ptr, uint32 condition, uint32 type)
{
    RES_NODE_DATA_T * res_node_ptr = PNULL;
    res_node_ptr = (RES_NODE_DATA_T*)(node_ptr->data);

    //SCI_TRACE_LOW:"[RES]: com_id = %d, res_id = 0x%x, size = %d, flag1 = 0x%x, flag2 = 0x%x, type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_1724_112_2_18_2_0_16_234,(uint8*)"dddddd", res_node_ptr->com_id, res_node_ptr->resource_id, res_node_ptr->size, res_node_ptr->handle_bit_flag1, res_node_ptr->handle_bit_flag2, res_node_ptr->type);
    return FALSE;
}
/*****************************************************************************/
//  Description : print res table info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void PrintfResListNodInfo(void)
{    
    if(PNULL != GetDLinkHeadPtr())
    {
        MMILINK_SearchNode(GetDLinkHeadPtr(), TRUE, PrintfNodeCallback, 0, 0);
    }
}


/*****************************************************************************/
//  Description : print res table info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
void MMIRES_PrintfRESTabInfo(void)
{
    //SCI_TRACE_LOW("[RES]: MMIRES_PrintfRESTabInfo start");
    PrintfRESTabInfo();
    //SCI_TRACE_LOW("[RES]: MMIRES_PrintfRESTabInfo end");
}

/*****************************************************************************/
//  Description : print res table info
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void PrintfRESTabInfo(void)
{    
//  SCI_Sleep(1000);
    PrintfWinTabInfo();
//    SCI_Sleep(500);
    PrintfResListNodInfo();
//    SCI_Sleep(500);   
}

/*****************************************************************************/
//  Description : get the flag bit location of handle
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL CAF_RES_T* GetComCafResInfo(uint32 com_id)
{
    uint16 i = 0;
    for(i = 0; i < MMI_CAF_RES_MAX; i ++)
    {
        if(com_id == s_caf_res_info[i].com_id)
        {
            return &(s_caf_res_info[i]);
        }
    }    
    return PNULL;
}

/*****************************************************************************/
//  Description : get the flag bit location of handle
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL int32 SearchHandleFlagIndex(CAF_HANDLE_T handle_id)
{
    uint32 i  = 0;
    uint32 index = (handle_id&(MMI_RES_MAX_RECORD_WIN_NUM-1));
    
    
    // 先根据 handle的值作为下标是否能找到
    if (s_handle_map[index].handle_id == handle_id)
    {
        return index;
    }
    else
    {
        //搜索s_handle_map表，看是否有此handle_id记录    
        for (i = 0; i < MMI_RES_MAX_RECORD_WIN_NUM; i++)
        {
            if(s_handle_map[i].handle_id == handle_id)
            {
                return i;
            }
        }        
    }
    return -1; //返回此handle_id 表中所在的位置
}

/*****************************************************************************/
//  Description : delete the flag bit location of handle
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void DelHandleFromHandleMap(CAF_HANDLE_T handle_id)
{
    uint32 i = 0;
    uint32 index = 0;   

    index = (handle_id & (MMI_RES_MAX_RECORD_WIN_NUM-1));

    if(s_handle_map[index].handle_id == handle_id)
    {
        s_handle_map[index].handle_id = 0;                  
        return ;
    }
    else
    {
        for (i = 0 ;i < MMI_RES_MAX_RECORD_WIN_NUM;i++)
        {
            if (s_handle_map[i].handle_id == handle_id)
            {
                s_handle_map[i].handle_id = 0;                                  
                return;
            }
        }
    }
}

/*****************************************************************************/
//  Description : Set the flag bit location of handle
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL int32 AddHandleToHandleMap(CAF_HANDLE_T handle_id)
{
    uint32 i = 0;
    uint32 index = 0;       

    index = (handle_id & (MMI_RES_MAX_RECORD_WIN_NUM-1));

    if (s_handle_map[index].handle_id == 0)
    {
        s_handle_map[index].handle_id = handle_id;      
        return index;
    }
    else
    {
        for (i = 0 ;i < MMI_RES_MAX_RECORD_WIN_NUM;i++)
        {
            if (s_handle_map[i].handle_id == 0)
            {
                s_handle_map[i].handle_id = handle_id;              
                return i;
            }
        }
    }
    return -1;
}
/*****************************************************************************/
//  Description : set the handle flag of refered res node
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void BitFlagOperate(uint32 * param_ptr, uint32 bit_pos, BOOLEAN is_set)
{
    SCI_ASSERT(PNULL != param_ptr); /*assert verified*/
    SCI_ASSERT(bit_pos < 32); /*assert verified*/

    if(is_set)
    {
        *param_ptr = *param_ptr | (0x00000001 << bit_pos);
    }
    else
    {
        *param_ptr = *param_ptr & ~(0x00000001U << bit_pos);
    }    
}
/*****************************************************************************/
//  Description : set the handle flag of refered res node
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void SetResNodeRefFlag(RES_NODE_DATA_T* node_ptr, CAF_HANDLE_T handle_id)
{
    int32 bit_index = 0;
    /* get handle bit index */
    bit_index = SearchHandleFlagIndex(handle_id);   
    if(bit_index < 0)
    {
        /* add the handle to handle map */
        bit_index = AddHandleToHandleMap(handle_id);        
    }
    
    SCI_ASSERT(bit_index >= 0); /*assert verified*/
    if(bit_index < 32)
    {
        BitFlagOperate(&node_ptr->handle_bit_flag1, bit_index, TRUE);        
    }
    else if(bit_index < 64)
    {
        BitFlagOperate(&node_ptr->handle_bit_flag2, bit_index-32, TRUE);        
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
}

/*****************************************************************************/
//  Description : set the handle flag of refered res node
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL uint32 ClearResNodeRefFlag(RES_NODE_DATA_T * node_ptr, CAF_HANDLE_T handle_id)
{
    int32 bit_index = 0;
    bit_index = SearchHandleFlagIndex(handle_id);    

    if(bit_index < 0)
    {
        return 0;
    }
    else if(bit_index < 32)
    {
        BitFlagOperate(&node_ptr->handle_bit_flag1, bit_index, FALSE);
    }
    else if(bit_index < 64)
    {
        BitFlagOperate(&node_ptr->handle_bit_flag2, bit_index-32, FALSE);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return (node_ptr->handle_bit_flag1 | node_ptr->handle_bit_flag2);
}

/*****************************************************************************/
//  Description : set the handle flag of refered res node
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckResNodeRefFlag(RES_NODE_DATA_T const * node_ptr, CAF_HANDLE_T handle_id)
{
    int32 bit_index = 0;
    bit_index = SearchHandleFlagIndex(handle_id);
    //SCI_ASSERT(bit_index >= 0);

    if(bit_index < 0)
    {
        return 0;
    }
    else if(bit_index < 32)
    {
        return (BOOLEAN)((node_ptr->handle_bit_flag1 & (0x1 << bit_index))>0);        
    }
    else if(bit_index < 64)
    {
        return (BOOLEAN)((node_ptr->handle_bit_flag2 & (0x1 << (bit_index-32)))>0);            
    }
    else
    {
        SCI_ASSERT(0);/*lint !e527 */ /*assert verified*/
    }        
    return FALSE;/*lint !e527 */
}
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
//  Description : init resource memory
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC MPOOL_HANDLE CAFRES_MemConfig( void )
{
    /* config memory */
    if( 0 == s_caf_res_mem_id )
    {
        uint32 size    = 0;
        void*  mem_ptr = PNULL;
        APP_MEM_CFG_T    mem_cfg = {0};
        
        if( NOT_USE == BL_GetStatus((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_CAF_RESOURCE ) )
        {      
            mem_ptr = MMI_BL_MALLOC((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_CAF_RESOURCE );
            size    = BL_GetSize((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_CAF_RESOURCE );
        }
        
        SCI_ASSERT( PNULL != mem_ptr ); /*assert verified*/
        
        /* config memory manager */
//         mem_cfg.min_pool_size = 0;
//         mem_cfg.pool_space_size =  0;
//         mem_cfg.heap_mem_watermark = 0;
        mem_cfg.pool_mode = MEM_POOL_RELEASE_MODE;
        s_caf_res_mem_id = SCI_CreateMemPool("caf res", (uint8*)mem_ptr, size, &mem_cfg);

        SCI_ASSERT( s_caf_res_mem_id > 0 ); /*assert verified*/
    }

    return s_caf_res_mem_id;
}

/*****************************************************************************/
//  Description : init resource memory
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void CAFRES_MemDestroy( void )
{
    if( 0 != s_caf_res_mem_id )
    {
        SCI_DeleteMemPool( s_caf_res_mem_id, TRUE);
        s_caf_res_mem_id = 0;

        MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_CAF_RESOURCE );      
    }
    else
    {
        //SCI_TRACE_LOW:"CAFResMemDestroy: warring! the mmk manager memory has be destroy"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_2022_112_2_18_2_0_17_235,(uint8*)"");
    }
}
#endif

/*****************************************************************************/
//  Description : destroy file theme resource memory on nor platform
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void NorFileThemeResMemDestroy(void)
{
#ifndef NANDBOOT_SUPPORT
	if (s_nor_file_theme_pool_refcnt > 0)
    {
        s_nor_file_theme_pool_refcnt--;
        if(0 != s_nor_file_theme_mem_manager_id
            && 0 == s_nor_file_theme_pool_refcnt)
        {
            if (PNULL != s_allocated_res_link_head)
            {
                MMI_FreeAllRes();
                s_allocated_res_link_head = PNULL;
            }
            
            /* config resource memory heap */
            SCI_DeleteMemPool(s_nor_file_theme_mem_manager_id, TRUE);
            SCI_FREE(s_nor_file_theme_pool);
            s_nor_file_theme_mem_manager_id = 0; 
        }
    }

    SCI_TRACE_LOW("@NorFileThemeResMemDestroy:s_nor_file_theme_mem_manager_id = 0x%x, s_nor_file_theme_pool_refcnt=%d",s_nor_file_theme_mem_manager_id, s_nor_file_theme_pool_refcnt);
#endif
}

/*****************************************************************************/
//  Description : init file theme resource memory on nor platform
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NorFileThemeResMemInit(void)
{ 
#ifndef NANDBOOT_SUPPORT
    if(0 == s_nor_file_theme_mem_manager_id)
    {
        APP_MEM_CFG_T    mem_cfg = {0};

        mem_cfg.pool_mode = MEM_POOL_RELEASE_MODE;

        BLKMEM_GarbageClean();

        s_nor_file_theme_pool = SCI_ALLOCA(NOR_FILE_THEME_MEM_TOTAL_SIZE);
        if (PNULL == s_nor_file_theme_pool)
        {
            return FALSE;
        }
        SCI_MEMSET(s_nor_file_theme_pool, 0, NOR_FILE_THEME_MEM_TOTAL_SIZE);

        /* config resource memory heap */
        s_nor_file_theme_mem_manager_id = SCI_CreateMemPool("nor file res", (uint8*)s_nor_file_theme_pool, NOR_FILE_THEME_MEM_TOTAL_SIZE, &mem_cfg);
        if (0 == s_nor_file_theme_mem_manager_id)
        {
            SCI_FREE(s_nor_file_theme_pool);
            s_nor_file_theme_pool_refcnt = 0;
            return FALSE;
        }
    }
    ++s_nor_file_theme_pool_refcnt;
    
    SCI_TRACE_LOW("@NorFileThemeResMemInit:s_nor_file_theme_mem_manager_id = 0x%x, s_nor_file_theme_pool_refcnt=%d",s_nor_file_theme_mem_manager_id, s_nor_file_theme_pool_refcnt);
#endif
    
    return TRUE;
}

/*******************************************************************/
//  Interface:		MMIRES_NorFileResInit
//  Description : 	MMIRES_NorFileResInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRES_NorFileResInit(void)
{
	return NorFileThemeResMemInit();
}

/*******************************************************************/
//  Interface:		MMIRES_NorFileResUnInit
//  Description : 	MMIRES_NorFileResUnInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMIRES_NorFileResUnInit(void)
{
	NorFileThemeResMemDestroy();
}

/*****************************************************************************/
//  Description : init resource memory
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void ResMemInit(void)
{   
#ifdef NANDBOOT_SUPPORT
    if(0 == s_nand_mem_manager_id)
    {
        APP_MEM_CFG_T    mem_cfg = {0};
//         mem_cfg.min_pool_size = 0;
//         mem_cfg.pool_space_size =  0;
//         mem_cfg.heap_mem_watermark = 0;  
        mem_cfg.pool_mode = MEM_POOL_RELEASE_MODE;
        /* config resource memory heap */
        s_nand_mem_manager_id = SCI_CreateMemPool("nand res", (uint8*)s_res_pool, RES_MEM_TOTAL_SIZE, &mem_cfg);
        SCI_ASSERT(s_nand_mem_manager_id > 0);         /*assert verified*/
    }
#endif
}

/*****************************************************************************/
//  Description : malloc resource memory
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL MPOOL_HANDLE GetResMemId(
                        RES_STORE_POS_E type
                        )
{
    MPOOL_HANDLE mem_id = 0;
    
    switch( type )
    {
    case RES_STORE_CAF_FILE:    
#ifdef DYNAMIC_MODULE_SUPPORT
        mem_id = s_caf_res_mem_id;        
#else
        SCI_ASSERT( FALSE );         /*assert verified*/
#endif
        break;
    case RES_STORE_THEME_FILE:
    case RES_STORE_THEME_PACKAGE:
#ifndef NANDBOOT_SUPPORT
        mem_id = s_nor_file_theme_mem_manager_id;
#else
        mem_id = s_nand_mem_manager_id;        
#endif
        break;
        
    case RES_STORE_FLASH_NAND:
#ifdef NANDBOOT_SUPPORT    
        mem_id = s_nand_mem_manager_id;
#else
        SCI_ASSERT( FALSE ); /*assert verified*/
#endif
        break;
        
    default:
        SCI_ASSERT( FALSE ); /*assert verified*/
        break;
    }
    
    return mem_id;/*lint !e527 */
}

/*****************************************************************************/
//  Description : malloc resource memory
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL uint8 * ResMalloc(
                        RES_STORE_POS_E type,
                        uint32 size
                        )
{ 
    uint8* ret_ptr = PNULL;
    MPOOL_HANDLE  mem_id  = GetResMemId( type );

    SCI_ASSERT( mem_id > 0 );         /*assert verified*/

    /* malloc from resource memory heap */
    ret_ptr = SCI_MPALLOC(size, mem_id);  

    if( PNULL == ret_ptr )
    {
        MMIRES_FreeAllNodeRes();
        ret_ptr = SCI_MPALLOC(size, mem_id);
        //SCI_TRACE_LOW:"ResMalloc fail,will malloc 1, ret_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_2180_112_2_18_2_0_17_236,(uint8*)"d", ret_ptr);
    }

#ifdef NANDBOOT_SUPPORT
    if( PNULL == ret_ptr )
    {
        //删除最近的铃声资源
        s_last_ring_id = 0;
        MMIRES_FreeAllNodeRes();
        ret_ptr = SCI_MPALLOC(size, mem_id); 
        //SCI_TRACE_LOW:"ResMalloc fail,will malloc 2, ret_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_2190_112_2_18_2_0_17_237,(uint8*)"d", ret_ptr);
    }
#else
    //nor版本千变主题分配不到，使用默认
    if ( PNULL == ret_ptr && mem_id == s_nor_file_theme_mem_manager_id )
    {
        return PNULL;
    }
#endif

    if (PNULL == ret_ptr )
    {
        //SCI_TRACE_LOW:"ResMalloc fail,will malloc from heap, cur size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_2202_112_2_18_2_0_17_238,(uint8*)"d", size);
        ret_ptr = SCI_ALLOCA(size);
        if (PNULL != ret_ptr)
        {
            s_from_heap_ptr = ret_ptr;
        }
    }
    SCI_ASSERT( PNULL != ret_ptr ); /*assert verified*/

    return ret_ptr;
}



/*****************************************************************************/
//  Description : Init resource module and info
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_ResourceInit(void)
{
#ifdef WIN32
    /* 从模拟器中获得资源的地址 */
    GetMMIResEntry(&s_mmi_res_bin_ptr);
    SCI_ASSERT(PNULL != s_mmi_res_bin_ptr); /*assert verified*/
#else
#ifndef NANDBOOT_SUPPORT 
    s_mmi_res_bin_ptr = PROD_GetResMemAdd();
#endif        
#endif
    /* init s_caf_res_info */
    SCI_MEMSET(s_caf_res_info, 0, (sizeof(CAF_RES_T)*MMI_CAF_RES_MAX));
    /* init res memory */
    ResMemInit();
    /* get packages info */
    GetFlashPackagesInfo();
}

/*****************************************************************************/
//  Description : add node to link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void AddNodeToLink(MMI_LINK_NODE_T ** link_head_pptr, uint32 node_data)
{
    SCI_ASSERT(PNULL != link_head_pptr); /*assert verified*/
    if(PNULL == *link_head_pptr)
    {
        *link_head_pptr = (MMI_LINK_NODE_T*)MMILINK_CreateHead(node_data);
    }
    else
    {
        *link_head_pptr = (MMI_LINK_NODE_T*)MMILINK_AddNodeBeforeBaseNode((MMI_LINK_NODE_T*)*link_head_pptr, MMILINK_CreateNode(node_data));
    }    
}

/*****************************************************************************/
//  Description : add dynamic resource node to link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void AddDynResNodeToLink(RES_NODE_DATA_T const *res_node_ptr)
{
    AddNodeToLink(&s_allocated_res_link_head, (uint32)res_node_ptr);        
}

/*****************************************************************************/
//  Description : destruct the node data
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void NodeDestructCallback(MMI_LINK_NODE_T const * node_ptr)
{
    RES_NODE_DATA_T * res_node_ptr = PNULL;
    res_node_ptr = (RES_NODE_DATA_T*)(node_ptr->data);
    //SCI_ASSERT(0 == (res_node_ptr->handle_bit_flag1|res_node_ptr->handle_bit_flag2));
    /* destruct data */
    RES_FREE(res_node_ptr->data_ptr);
    RES_FREE(res_node_ptr);  /*lint !e63*/  
}

/*****************************************************************************/
//  Description : condition for find
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FoundByHandleCallback(MMI_LINK_NODE_T const* node_ptr, uint32 condition, uint32 type)
{
    RES_NODE_DATA_T * res_node_ptr = PNULL;
    res_node_ptr = (RES_NODE_DATA_T*)(node_ptr->data);
    /* whether the handle flag contain the handle bit */
    if(CheckResNodeRefFlag(res_node_ptr, condition))
    {        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Free dynamic resource node from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void FreeDynResNodeByHandle(CAF_HANDLE_T handle_id)
{
    MMI_LINK_NODE_T * free_node_ptr = PNULL;    
    RES_NODE_DATA_T * res_node_ptr = PNULL;    
    
    //SCI_TRACE_LOW:"[RES]: FreeDynResNodeByHandle: handle_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_2307_112_2_18_2_0_17_239,(uint8*)"d", handle_id);
    do                  
    {
        if(PNULL == GetDLinkHeadPtr())
        {
            break;
        }
        else
        {
            free_node_ptr = MMILINK_SearchNode(GetDLinkHeadPtr(), TRUE, FoundByHandleCallback, handle_id, NULL);
            if(PNULL != free_node_ptr)
            {
                /* get res node */
                res_node_ptr = (RES_NODE_DATA_T*)(free_node_ptr->data);
                /* clear reference bit, if the reference is zero, remove and destroy the node from link. */
                if(0 == ClearResNodeRefFlag(res_node_ptr, handle_id))
                {
                    /* delete this res node from link */
                    s_allocated_res_link_head = MMILINK_DestroyNode((MMI_LINK_NODE_T*)free_node_ptr, GetDLinkHeadPtr(), NodeDestructCallback); 
                }                
            }
            //SCI_TRACE_LOW("[RES]: FreeDynResNodeByHandle, free end");
        }
    }
    while(PNULL != free_node_ptr);        
    
    DelHandleFromHandleMap(handle_id);
}

/*****************************************************************************/
//  Description : condition for find
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchNodeCallback(MMI_LINK_NODE_T const* node_ptr, uint32 condition, uint32 type)
{
    RES_NODE_DATA_T * res_node_ptr = PNULL;
    res_node_ptr = (RES_NODE_DATA_T*)(node_ptr->data);
    /* whether the handle flag contain the handle bit */
    if( (res_node_ptr->resource_id == *((uint32*)condition+0))
        //&& (CheckResNodeRefFlag(res_node_ptr, *((uint32*)condition+1)))
        && (res_node_ptr->com_id == *((uint32*)condition+1))
        && (res_node_ptr->type == (MMI_RES_TYPE_E)*((uint32*)condition+2)) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : add dynamic resource node to link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL RES_NODE_DATA_T* SearchResFromDLink(uint32 res_id, CAF_HANDLE_T handle_id, uint32 com_id, MMI_RES_TYPE_E type)
{
    MMI_LINK_NODE_T* found_node_ptr = PNULL;
    uint32 condition[4] = {0};    
    if(PNULL != GetDLinkHeadPtr())
    {        
        condition[0] = res_id;        
        condition[1] = com_id;
        condition[2] = type;
        //condition[3] = handle_id;
        found_node_ptr = MMILINK_SearchNode(GetDLinkHeadPtr(), TRUE, SearchNodeCallback, (uint32)condition, 3);
    }

    if(PNULL == found_node_ptr)
    {
        return PNULL;
    }
    
    return (RES_NODE_DATA_T*)(found_node_ptr->data);
}

/*****************************************************************************/
//  Description : get Image data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note: add this fuc to get size of image,the fuc is temp for test
/*****************************************************************************/
void GetResOffsetAndSize(const CAF_RES_T*  com_res_info_ptr, 
                         uint32 * res_unit_offset_ptr, 
                         uint32 * res_unit_size_ptr, 
                         uint32 * data_type_ptr,
                         uint32 res_id,                          
                         MMI_RES_TYPE_E res_type)
{
    uint32      project_module_offset = 0;
    uint16      prj_module_id   = MMI_GET_MODU_ID(res_id);
    uint16      res_unit_index  = MMICOM_GET_WORD_LB(res_id);    
    if(0 < res_unit_index)
    {
        res_unit_index --;
    }
    
    *res_unit_offset_ptr = 0;
    *res_unit_size_ptr = 0;
    if(PNULL != com_res_info_ptr)
    {
        SCI_ASSERT(RES_STORE_FLASH_NOR != com_res_info_ptr->res_location.store_type); /*assert verified*/
        /* get project module offset */
        project_module_offset = GetModuleInfoAbsOffset(com_res_info_ptr, prj_module_id);        
        /* get the offset of resource unit in project module */
        switch(res_type)
        {
        case MMI_RT_IMAGE:         
            if(res_unit_index < com_res_info_ptr->image_module_arry[prj_module_id].unit_total)
            {
                *res_unit_offset_ptr = com_res_info_ptr->image_module_arry[prj_module_id].unit_info_arry[res_unit_index].data_addr + project_module_offset;
                *res_unit_size_ptr   = com_res_info_ptr->image_module_arry[prj_module_id].unit_info_arry[res_unit_index].data_size;
            }            
            break;            
        case MMI_RT_ANIM:            
            if(res_unit_index < com_res_info_ptr->anim_module_arry[prj_module_id].unit_total)
            {
                *res_unit_offset_ptr = com_res_info_ptr->anim_module_arry[prj_module_id].unit_info_arry[res_unit_index].data_addr + project_module_offset;
                *res_unit_size_ptr   = com_res_info_ptr->anim_module_arry[prj_module_id].unit_info_arry[res_unit_index].data_size;
            }
            break;            
        case MMI_RT_DATA:    
            if(res_unit_index < com_res_info_ptr->data_module_arry[prj_module_id].unit_total)
            {
                *res_unit_offset_ptr = com_res_info_ptr->data_module_arry[prj_module_id].unit_info_arry[res_unit_index].data_addr + project_module_offset;
                *res_unit_size_ptr   = com_res_info_ptr->data_module_arry[prj_module_id].unit_info_arry[res_unit_index].data_size;                
            }
            break;
        case MMI_RT_RING:
            if(res_unit_index < com_res_info_ptr->ring_module_arry[prj_module_id].unit_total)
            {
                *res_unit_offset_ptr = com_res_info_ptr->ring_module_arry[prj_module_id].unit_info_arry[res_unit_index].data_addr + project_module_offset;
                *res_unit_size_ptr   = com_res_info_ptr->ring_module_arry[prj_module_id].unit_info_arry[res_unit_index].data_size;
                *data_type_ptr = com_res_info_ptr->ring_module_arry[prj_module_id].unit_info_arry[res_unit_index].data_addition;

//                *res_unit_offset_ptr = (uint32)(com_res_info_ptr->res_module_arry[prj_module_id].mmi_data_table[res_type].pData) + project_module_offset;
//                ReadResource( com_res_info_ptr->res_handle, &ring_info, *res_unit_offset_ptr + (res_unit_index)*sizeof(MMIAUD_RING_DATA_INFO_T), sizeof(MMIAUD_RING_DATA_INFO_T), com_res_info_ptr->res_location.store_type );
//                *res_unit_size_ptr = ring_info.data_len;
//                *data_type_ptr = ring_info.type;
//                *res_unit_offset_ptr = (uint32)ring_info.data_ptr + project_module_offset;
            }
            break;            
        default:
            SCI_ASSERT(0); /*assert verified*/
            break;
        }
    }
}
/*****************************************************************************/
//  Description : get Image data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note: add this fuc to get size of image,the fuc is temp for test
/*****************************************************************************/
RES_NODE_DATA_T* LoadResNode(const CAF_RES_T * com_res_info_ptr, uint32 res_id, CAF_HANDLE_T handle_id, MMI_RES_TYPE_E res_type)
{       
    uint32      res_unit_offset = 0;
    uint32      res_unit_size = 0;    
    uint8*      data_buffer_ptr= PNULL;
    RES_NODE_DATA_T * res_node_ptr = PNULL;    
    uint32      data_type = 0;    
    
    //com_res_info_ptr = GetComCafResInfo(com_id);
    if(com_res_info_ptr == PNULL)
    {
        return res_node_ptr;
    }

    if(com_res_info_ptr->is_caf_resource)
    {
        return CAF_LoadResNode(com_res_info_ptr, res_id, handle_id, (CAF_MMI_RES_TYPE_E)res_type);
    }
    else
    {
        //if(PNULL != com_res_info_ptr)
        {
            GetResOffsetAndSize(com_res_info_ptr, &res_unit_offset, &res_unit_size, &data_type, res_id, res_type);
        
            if(0 != res_unit_size)
            {            
                /* malloc buffer for load resource data */
                //SCI_TRACE_LOW("[RES]:res_unit_size = %d",res_unit_size);
                data_buffer_ptr = (uint8*)ResMalloc( com_res_info_ptr->res_location.store_type, res_unit_size );
                SCI_ASSERT(data_buffer_ptr!=PNULL); /*assert verified*/
                SCI_MEMSET(data_buffer_ptr, 0x00, res_unit_size);
            
                /* load resource data */
                ReadResource( com_res_info_ptr->res_handle, data_buffer_ptr, res_unit_offset, res_unit_size, com_res_info_ptr->res_location.store_type );
            
                /* config resource dynamic node */
                res_node_ptr = (RES_NODE_DATA_T*)ResMalloc( com_res_info_ptr->res_location.store_type, sizeof(RES_NODE_DATA_T) );
                SCI_ASSERT(PNULL != res_node_ptr); /*assert verified*/
                SCI_MEMSET(res_node_ptr, 0x00, sizeof(RES_NODE_DATA_T));
            
                res_node_ptr->com_id        = com_res_info_ptr->com_id;
                res_node_ptr->resource_id   = res_id;
                res_node_ptr->type          = res_type;
                res_node_ptr->data_ptr      = data_buffer_ptr;
                res_node_ptr->data_type     = data_type;
                res_node_ptr->size          = res_unit_size;                
            }
        }

        return res_node_ptr;   
    }
}

/*****************************************************************************/
//  Description : load packages info
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void GetFlashPackagesInfo(void)
{ 
#ifdef NANDBOOT_SUPPORT
    s_package_info_ptr = (RESOURCE_PACKAGE_INFO_T*)ResMalloc(RES_STORE_FLASH_NAND, sizeof(RESOURCE_PACKAGE_INFO_T));//SCI_ALLOC_APP(sizeof(RESOURCE_PACKAGE_INFO_T));
    SCI_MEMSET(s_package_info_ptr, 0, sizeof(RESOURCE_PACKAGE_INFO_T));
    /* 读取资源包信息 */
    ReadResource(0, s_package_info_ptr, 0, sizeof(RESOURCE_PACKAGE_INFO_T), RES_STORE_FLASH_NAND);        
#else    
    /* 读取资源包信息 */
    s_package_info_ptr = (PRESOURCE_PACKAGE_INFO_T)s_mmi_res_bin_ptr;    
    //ReadResource(0, s_package_info_ptr, 0, sizeof(RESOURCE_PACKAGE_INFO_T), RES_STORE_FLASH_NOR);        
#endif    
    SCI_ASSERT(s_package_info_ptr->addition_num < MAX_PACKAGE_RESOURCE_NUM); /*assert verified*/
}

/*****************************************************************************/
//  Description : load resource and get every module offset
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void GetComResHeadInfo( CAF_RES_T *pRes )
{    
    int16 i = 0;
    uint32 j = 0;
    uint32 module_offset = 0;

    /** get resource brief **/
    pRes->res_brief_ptr = (MMI_RESOURCE_BRIEF_T*)ResMalloc(pRes->res_location.store_type, sizeof(MMI_RESOURCE_BRIEF_T));
    ReadResource( pRes->res_handle, pRes->res_brief_ptr, pRes->res_data_offset, sizeof(MMI_RESOURCE_BRIEF_T), pRes->res_location.store_type );        

    /** get resource every module info **/
    /* get module total number */
    pRes->res_module_total = 0;
    for(i = MMI_MAX_MODU_NUM-1; i >= 0; i --)
    {        
        if(!IsModuleInfoEmpty(pRes, i))
        {   
            break;
        }        
    }
    pRes->res_module_total = (uint16)(i+1);

    SCI_ASSERT(pRes->res_module_total > 0); /*assert verified*/
    
    pRes->res_module_arry = (MMIRES_MODULE_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(MMIRES_MODULE_INFO_T)*pRes->res_module_total );
    SCI_MEMSET(pRes->res_module_arry, 0, (sizeof(MMIRES_MODULE_INFO_T)*pRes->res_module_total));
    
    for (j = 0; j < pRes->res_module_total; j ++)
    {        
        module_offset = GetModuleInfoAbsOffset(pRes, j);
        if (IsModuleInfoEmpty(pRes, j))
        {
            continue;
        }

        /* read module info */
        ReadResource( pRes->res_handle, &(pRes->res_module_arry[j]), module_offset, sizeof(MMIRES_MODULE_INFO_T), pRes->res_location.store_type );
    }
}

/*****************************************************************************/
//  Description : load resource and get every module offset
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void GetImageResInfo(CAF_RES_T *pRes)
{
    uint16 i = 0;
    uint16 j = 0;
    uint32 offset = 0;
    uint32 module_offset = 0;   
    uint32 count = 0;
    IMG_FLAG_T image_flag = {0};    
    MMI_IMG_DATA_T image_data = {0};

    //SCI_TRACE_LOW("GetImageResInfo ");    
    
    pRes->image_module_arry = (RES_ARRY_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_ARRY_INFO_T)*pRes->res_module_total);
    SCI_MEMSET(pRes->image_module_arry, 0, (sizeof(RES_ARRY_INFO_T)*pRes->res_module_total));
    for (i = 0; i < pRes->res_module_total; i ++)
    {                        
        //计算模块绝对位移
        module_offset = GetModuleInfoAbsOffset(pRes, i);
        
        if(IsModuleInfoEmpty(pRes, i))
        {
            continue;
        }
        //得到当前模块image的数量
        count = pRes->res_module_arry[i].mmi_data_table[MMI_RT_IMAGE].uCount;
        if (count!=0)
        {
            offset = module_offset + (uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_IMAGE].pData;
            ReadResource( pRes->res_handle, &image_data, offset, sizeof(MMI_IMG_DATA_T), pRes->res_location.store_type );
            
            //分配count RES_ARRY_INFO_T数据空间
            pRes->image_module_arry[i].unit_info_arry = (RES_UNIT_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_UNIT_INFO_T)*count);
            pRes->image_module_arry[i].unit_total = count;
            
            for (j = 0; j < count; j ++)
            {
                ReadResource( pRes->res_handle, &(pRes->image_module_arry[i].unit_info_arry[j].data_addr), (uint32)(image_data.pImgData) + module_offset+j*sizeof(uint32), sizeof(uint32), pRes->res_location.store_type );
                ReadResource( pRes->res_handle, &image_flag, module_offset+(uint32)(image_data.pFlag)+(j)*sizeof(IMG_FLAG_T), sizeof(IMG_FLAG_T), pRes->res_location.store_type );
                pRes->image_module_arry[i].unit_info_arry[j].data_size = image_flag.nSize;

            }
        }   
    }
}


/*****************************************************************************/
//  Description : load resource and get every module offset
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void GetRingResInfo(CAF_RES_T *pRes)
{
    uint16 i = 0;
    uint16 j = 0;    
    uint32 offset = 0;
    uint32 module_offset = 0;   
    uint32 count = 0;
    MMIAUD_RING_DATA_INFO_T ring_info = {0};        

    //SCI_TRACE_LOW("GetImageResInfo ");    
    
    pRes->ring_module_arry = (RES_ARRY_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_ARRY_INFO_T)*pRes->res_module_total);
    SCI_MEMSET(pRes->ring_module_arry, 0, (sizeof(RES_ARRY_INFO_T)*pRes->res_module_total));
    for (i = 0; i < pRes->res_module_total; i ++)
    {                        
        //计算模块绝对位移
        module_offset = GetModuleInfoAbsOffset(pRes, i);
        
        if(IsModuleInfoEmpty(pRes, i))
        {
            continue;
        }
        //得到当前模块ring的数量
        count = pRes->res_module_arry[i].mmi_data_table[MMI_RT_RING].uCount;
        if (count!=0)
        {   
            offset = module_offset + (uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_RING].pData;
            //分配count RES_ARRY_INFO_T数据空间
            pRes->ring_module_arry[i].unit_info_arry = (RES_UNIT_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_UNIT_INFO_T)*count);
            pRes->ring_module_arry[i].unit_total = count;
            
            for (j = 0; j < count; j ++)
            {
                
                ReadResource( pRes->res_handle, &ring_info, offset + (j)*sizeof(MMIAUD_RING_DATA_INFO_T), sizeof(MMIAUD_RING_DATA_INFO_T), pRes->res_location.store_type );
                pRes->ring_module_arry[i].unit_info_arry[j].data_size = ring_info.data_len;
                pRes->ring_module_arry[i].unit_info_arry[j].data_addr = (uint32)ring_info.data_ptr;
                pRes->ring_module_arry[i].unit_info_arry[j].data_addition = ring_info.type;

            }
        }   
    }
}

/*****************************************************************************/
//  Description : load resource and get every module offset
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void GetDataResInfo(CAF_RES_T *pRes)
{
    uint16 i = 0;
    uint16 j = 0;    
    uint32 offset = 0;
    uint32 module_offset = 0;   
    uint32 count = 0;
    MMIAUD_RING_DATA_INFO_T ring_info = {0};        

    //SCI_TRACE_LOW("GetImageResInfo ");    
    
    pRes->data_module_arry = (RES_ARRY_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_ARRY_INFO_T)*pRes->res_module_total);
    SCI_MEMSET(pRes->data_module_arry, 0, (sizeof(RES_ARRY_INFO_T)*pRes->res_module_total));
    for (i = 0; i < pRes->res_module_total; i ++)
    {                        
        //计算模块绝对位移
        module_offset = GetModuleInfoAbsOffset(pRes, i);
        
        if(IsModuleInfoEmpty(pRes, i))
        {
            continue;
        }
        //得到当前模块ring的数量
        count = pRes->res_module_arry[i].mmi_data_table[MMI_RT_DATA].uCount;
        if (count!=0)
        {   
            offset = module_offset + (uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_DATA].pData;
            //分配count RES_ARRY_INFO_T数据空间
            pRes->data_module_arry[i].unit_info_arry = (RES_UNIT_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_UNIT_INFO_T)*count);
            pRes->data_module_arry[i].unit_total = count;
            
            for (j = 0; j < count; j ++)
            {
                
                ReadResource( pRes->res_handle, &ring_info, offset + (j)*sizeof(MMIAUD_RING_DATA_INFO_T), sizeof(MMIAUD_RING_DATA_INFO_T), pRes->res_location.store_type );
                pRes->data_module_arry[i].unit_info_arry[j].data_size = ring_info.data_len;
                pRes->data_module_arry[i].unit_info_arry[j].data_addr = (uint32)ring_info.data_ptr;
                pRes->data_module_arry[i].unit_info_arry[j].data_addition = ring_info.type;
            }
        }   
    }
}

/*****************************************************************************/
//  Description : load resource and get every module offset
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void GetAnimResInfo(CAF_RES_T *pRes )
{
    uint16 i = 0;
    uint16 j = 0;
    uint32 offset = 0;
    uint32 module_offset = 0;   
    uint32 count = 0;    
    ANIM_FLAG_T anim_flag = {0};
    MMI_ANIM_DATA_T anim_data = {0};

    //SCI_TRACE_LOW("GetAnimResInfo ");
        
    pRes->anim_module_arry = (RES_ARRY_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_ARRY_INFO_T)*pRes->res_module_total);
    SCI_MEMSET(pRes->anim_module_arry, 0, (sizeof(RES_ARRY_INFO_T)*pRes->res_module_total));
    for (i = 0; i < pRes->res_module_total; i ++)
    {                
        //计算模块绝对位移
        module_offset = GetModuleInfoAbsOffset(pRes, i);
        
        if(IsModuleInfoEmpty(pRes, i))
        {
            continue;
        }
        //得到当前模块anim的数量
        count = pRes->res_module_arry[i].mmi_data_table[MMI_RT_ANIM].uCount;
        if (count!=0)
        {
            offset = module_offset+(uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_ANIM].pData;
            ReadResource( pRes->res_handle, &anim_data, offset, sizeof(MMI_ANIM_DATA_T), pRes->res_location.store_type );
            
            //分配count RES_ARRY_INFO_T数据空间
            pRes->anim_module_arry[i].unit_info_arry = (RES_UNIT_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_UNIT_INFO_T)*count);
            pRes->anim_module_arry[i].unit_total = count;
            
            for (j = 0; j < count; j ++)
            {
                ReadResource( pRes->res_handle, &(pRes->anim_module_arry[i].unit_info_arry[j].data_addr), (uint32)(anim_data.pAnimData) + module_offset+j*sizeof(uint32), sizeof(uint32), pRes->res_location.store_type );
                ReadResource( pRes->res_handle, &anim_flag, module_offset+(uint32)(anim_data.pFlag)+(j)*sizeof(ANIM_FLAG_T), sizeof(ANIM_FLAG_T), pRes->res_location.store_type );
                pRes->anim_module_arry[i].unit_info_arry[j].data_size = anim_flag.nDataSize;

            }
        }   
    }
}


/*****************************************************************************/
//  Description : Free caf info
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void FreeCafResInfo(CAF_RES_T * res_info_ptr)
{
    uint32 i = 0;
    if(PNULL != res_info_ptr)
    {
        if(res_info_ptr->is_caf_resource) //caf application resource 
        {
            for(i = 0; i < res_info_ptr->res_module_total; i ++)
            {
                if((res_info_ptr->caf_image_module_arry != PNULL) && (res_info_ptr->caf_image_module_arry[i].common_info_arry != PNULL))
                {
                    RES_FREE(res_info_ptr->caf_image_module_arry[i].common_info_arry);  /*lint !e63*/
                    res_info_ptr->caf_image_module_arry[i].common_info_arry = PNULL;
                }
                if((res_info_ptr->caf_anim_module_arry != PNULL) && (res_info_ptr->caf_anim_module_arry[i].common_info_arry != PNULL))
                {
                    RES_FREE(res_info_ptr->caf_anim_module_arry[i].common_info_arry);  /*lint !e63*/
                    res_info_ptr->caf_anim_module_arry[i].common_info_arry = PNULL;
                }
                if((res_info_ptr->caf_ring_module_arry != PNULL) && (res_info_ptr->caf_ring_module_arry[i].common_info_arry != PNULL))
                {
                    RES_FREE(res_info_ptr->caf_ring_module_arry[i].common_info_arry);  /*lint !e63*/
                    res_info_ptr->caf_ring_module_arry[i].common_info_arry = PNULL;
                }
            }
            
            if(res_info_ptr->caf_res_header_ptr != PNULL)
            {
                RES_FREE(res_info_ptr->caf_res_header_ptr); /*lint !e63*/
                res_info_ptr->caf_res_header_ptr = PNULL;
            }
            if(res_info_ptr->caf_res_module_arry != PNULL)
            {
                RES_FREE(res_info_ptr->caf_res_module_arry); /*lint !e63*/
                res_info_ptr->caf_res_module_arry = PNULL;
            }
            
            if(res_info_ptr->caf_image_module_arry != PNULL)
            {
                RES_FREE(res_info_ptr->caf_image_module_arry); /*lint !e63*/
                res_info_ptr->caf_image_module_arry = PNULL;
            }
            
            if(res_info_ptr->caf_anim_module_arry != PNULL)
            {
                RES_FREE(res_info_ptr->caf_anim_module_arry); /*lint !e63*/
                res_info_ptr->caf_anim_module_arry = PNULL;
            }
            
            if(res_info_ptr->caf_ring_module_arry != PNULL) 
            {
                RES_FREE(res_info_ptr->caf_ring_module_arry); /*lint !e63*/
                res_info_ptr->caf_ring_module_arry = PNULL;
            }
            
            if(res_info_ptr->caf_res_theme_info_ptr != PNULL)
            {
                RES_FREE(res_info_ptr->caf_res_theme_info_ptr); /*lint !e63*/
                res_info_ptr->caf_res_theme_info_ptr = PNULL;
            }
            
            
            if(PNULL != res_info_ptr->text_prj_module_link_ptr)
            {
                MMILINK_DestroyLink((MMI_LINK_NODE_T*)res_info_ptr->text_prj_module_link_ptr, TextDestructCallback);        
            }
        }
        else
        {
            if(RES_STORE_FLASH_NOR != res_info_ptr->res_location.store_type)
            {
                for(i = 0; i < res_info_ptr->res_module_total; i ++)
                {
                    RES_FREE(res_info_ptr->anim_module_arry[i].unit_info_arry); /*lint !e63*/
                    RES_FREE(res_info_ptr->image_module_arry[i].unit_info_arry);  /*lint !e63*/           
                    RES_FREE(res_info_ptr->ring_module_arry[i].unit_info_arry);  /*lint !e63*/           
                    RES_FREE(res_info_ptr->data_module_arry[i].unit_info_arry);   /*lint !e63*/          
                }
                
                RES_FREE(res_info_ptr->res_brief_ptr); /*lint !e63*/
                RES_FREE(res_info_ptr->res_module_arry); /*lint !e63*/
                RES_FREE(res_info_ptr->anim_module_arry); /*lint !e63*/
                RES_FREE(res_info_ptr->image_module_arry); /*lint !e63*/
                RES_FREE(res_info_ptr->ring_module_arry); /*lint !e63*/
                RES_FREE(res_info_ptr->data_module_arry); /*lint !e63*/
                if(PNULL != res_info_ptr->text_prj_module_link_ptr)
                {
                    MMILINK_DestroyLink((MMI_LINK_NODE_T*)res_info_ptr->text_prj_module_link_ptr, TextDestructCallback);        
                }
                if(PNULL != res_info_ptr->color_prj_module_link_ptr)
                {
                    MMILINK_DestroyLink((MMI_LINK_NODE_T*)res_info_ptr->color_prj_module_link_ptr, TextDestructCallback);        
                }
                if(PNULL != res_info_ptr->rect_prj_module_link_ptr)
                {
                    MMILINK_DestroyLink((MMI_LINK_NODE_T*)res_info_ptr->rect_prj_module_link_ptr, TextDestructCallback);        
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : load resource and get every module offset for other theme
//  Global resource dependence : none
//  Author: apple.zhang
//  Note: static_theme_id from 1, 0: default theme.
/*****************************************************************************/
PUBLIC void MMIRES_UpdateStaticThemeRes(uint8 static_theme_id) 
{   
    UpdateStaticThemeRes(static_theme_id);
#ifndef NANDBOOT_SUPPORT
//    NorFileThemeResMemDestroy();
#endif
}

/*****************************************************************************/
//  Description : switch static theme
//  Global resource dependence : none
//  Author: apple.zhang
//  Note: 横竖屏切换时调用
/*****************************************************************************/
PUBLIC void MMIRES_SwitchStaticThemeRes(void) 
{
#ifdef MMI_RES_ORIENT_BOTH
    FreeComNodeRes(MMIRES_DEFAULT_COM_ID, TRUE);
#endif
}

/*****************************************************************************/
//  Description : load theme resource
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void FreeThemeRes(void)
{
    if(PNULL != s_appoint_res_ptr)
    {
        FreeCafResInfo(s_appoint_res_ptr);
        CloseResource(s_appoint_res_ptr->res_location.store_type, s_appoint_res_ptr->res_handle );        
        SCI_FREE(s_appoint_res_ptr);       
        s_appoint_res_ptr = PNULL;
    }

    //横屏资源
#ifdef MMI_RES_ORIENT_BOTH
    if(PNULL != s_appoint_landscape_res_ptr)
    {
        FreeCafResInfo(s_appoint_landscape_res_ptr);
        CloseResource(s_appoint_landscape_res_ptr->res_location.store_type, s_appoint_landscape_res_ptr->res_handle );        
        SCI_FREE(s_appoint_landscape_res_ptr);       
        s_appoint_landscape_res_ptr = PNULL;
    }
#endif

}

/*****************************************************************************/
//  Description : load theme resource
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void LoadFileThemeRes(const wchar * file_name)
{

    FreeThemeRes();  

    s_appoint_res_ptr = SCI_ALLOC_APP(sizeof(CAF_RES_T));
    SCI_MEMSET(s_appoint_res_ptr, 0, sizeof(CAF_RES_T));

    s_appoint_res_ptr->com_id = MMIRES_DEFAULT_COM_ID;
    s_appoint_res_ptr->res_location.store_type  = RES_STORE_THEME_FILE;    
    s_appoint_res_ptr->res_location.res_file    = (wchar*)file_name;      
    s_appoint_res_ptr->res_data_offset          = 0;
    LoadResourceData(s_appoint_res_ptr);

}
/*****************************************************************************/
//  Description : update theme resource from file
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void UpdateFileThemeRes(const wchar * file_name)
{
    FreeComNodeRes(MMIRES_DEFAULT_COM_ID, FALSE);    
    /* load 其他的主题资源 */
    LoadFileThemeRes(file_name);
}

/*****************************************************************************/
//  Description : load resource and get every module offset for other theme from file
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC void MMIRES_UpdateFileThemeRes(const wchar* file_name) 
{   
#ifndef NANDBOOT_SUPPORT
//    NorFileThemeResMemInit();
#endif
    UpdateFileThemeRes(file_name);
    
}
/*****************************************************************************/
//  Description : load theme resource
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void LoadStaticThemeRes(uint32 static_theme_id)
{
    if( static_theme_id <= MMIRES_GetStaticThemeTotalNum()
        && static_theme_id > 0 )
    {
        FreeThemeRes();
        
        s_appoint_res_ptr = SCI_ALLOC_APP(sizeof(CAF_RES_T));
        SCI_MEMSET(s_appoint_res_ptr, 0, sizeof(CAF_RES_T));
        
        s_appoint_res_ptr->com_id = MMIRES_DEFAULT_COM_ID;
#ifdef NANDBOOT_SUPPORT
        s_appoint_res_ptr->res_location.store_type = RES_STORE_FLASH_NAND;
#else
        s_appoint_res_ptr->res_location.store_type = RES_STORE_FLASH_NOR;
#endif
        
#ifdef MMI_RES_ORIENT_BOTH        
        s_appoint_res_ptr->res_data_offset = s_package_info_ptr->addition_info[static_theme_id * 2 - 1].offset;
#else
        s_appoint_res_ptr->res_data_offset = s_package_info_ptr->addition_info[static_theme_id - 1].offset;

#endif
        LoadResourceData(s_appoint_res_ptr);    

        //横屏资源
#ifdef MMI_RES_ORIENT_BOTH
        if ( static_theme_id * 2 < GetStaticThemeTotalNum() )
        {
            s_appoint_landscape_res_ptr = SCI_ALLOC_APP(sizeof(CAF_RES_T));
            SCI_MEMSET(s_appoint_landscape_res_ptr, 0, sizeof(CAF_RES_T));
            
            s_appoint_landscape_res_ptr->com_id = MMIRES_DEFAULT_COM_ID;
#ifdef NANDBOOT_SUPPORT
            s_appoint_landscape_res_ptr->res_location.store_type = RES_STORE_FLASH_NAND;
#else
            s_appoint_landscape_res_ptr->res_location.store_type = RES_STORE_FLASH_NOR;
#endif
            
            s_appoint_landscape_res_ptr->res_data_offset = s_package_info_ptr->addition_info[static_theme_id * 2].offset;
            
            LoadResourceData(s_appoint_landscape_res_ptr);    
        }
#endif
    }   
}



/*****************************************************************************/
//  Description : update theme resource
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void UpdateStaticThemeRes(uint8 theme_id)
{
    //SCI_Sleep(200);
    if (0 != theme_id)
    {       
        FreeComNodeRes(MMIRES_DEFAULT_COM_ID, FALSE);     

        /* load 其他的主题资源 */
        LoadStaticThemeRes(theme_id);        
    }
    else
    {
        /* 清空其他主题的内容 */
        FreeThemeRes();
        FreeComNodeRes(MMIRES_DEFAULT_COM_ID, FALSE);
    }
}

/*****************************************************************************/
//  Description : load rect resource to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void LoadRectRes(CAF_RES_T *pRes )
{
    uint8 * data_buffer = PNULL;
    uint32  size= 0;
    uint16  i = 0 ;    
    uint32  offset = 0;
    uint32  data_offset = 0;
    uint32  count = 0;        
    RES_MODULE_DATA_T *module_data_ptr = PNULL;    

    for (i =0; i<pRes->res_module_total; i++)
    {   
        if(IsModuleInfoEmpty(pRes, i))
        {
            continue;
        }

        /* malloc module buffer */
        module_data_ptr = (RES_MODULE_DATA_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_MODULE_DATA_T));
        SCI_MEMSET(module_data_ptr, 0x00, sizeof(RES_MODULE_DATA_T));

        /* get every project module info */
        data_offset = (uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_RECT].pData;
        count = (uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_RECT].uCount;
        size = pRes->res_module_arry[i].mmi_data_table[MMI_RT_RECT].uSize;
        offset = GetModuleInfoAbsOffset(pRes, i) + data_offset;

//        /* get multi country language info of every project module */        
//        ReadResource( pRes->res_handle, &color_module_info, offset, sizeof(RES_COLOR_MODULE_INFO_T), pRes->res_location.store_type );
//
//        /* get offset of indicated language text data */
//        offset = color_module_info.color_table_offset + pRes->res_brief_ptr->mmi_module_adrr[i];
//        count = color_module_info.color_number;
        
        if ( (0 != count) && (0 != size))
        {
            data_buffer = (uint8 *)ResMalloc( pRes->res_location.store_type, count*sizeof(MMIRES_RECT_T));
            SCI_ASSERT(data_buffer != PNULL); /*assert verified*/
            SCI_MEMSET(data_buffer, 0x00, (count*sizeof(MMIRES_RECT_T)));            
            ReadResource( pRes->res_handle, data_buffer, offset, count*sizeof(MMIRES_RECT_T), pRes->res_location.store_type );
            
            //为每个节点指定一个head数据空间
            module_data_ptr->module_id = i;
            module_data_ptr->size = count*sizeof(MMIRES_RECT_T);
            module_data_ptr->unit_data_ptr = data_buffer;
            module_data_ptr->unit_offset_arry = 0;

            /* add module to link */
            AddNodeToLink(&pRes->rect_prj_module_link_ptr, (uint32)module_data_ptr);            
        }
        else
        {
            RES_FREE(module_data_ptr); /*lint !e63*/
        }
    }    
}

/*****************************************************************************/
//  Description : load color resource to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void LoadColorRes(CAF_RES_T *pRes )
{
    uint8 * data_buffer = PNULL;
    uint32  size= 0;
    uint16  i = 0 ;    
    uint32  offset = 0;
    uint32  data_offset = 0;
    uint32  count = 0;        
    RES_MODULE_DATA_T *module_data_ptr = PNULL;    

    for (i =0; i<pRes->res_module_total; i++)
    {   
        if(IsModuleInfoEmpty(pRes, i))
        {
            continue;
        }

        /* malloc module buffer */
        module_data_ptr = (RES_MODULE_DATA_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_MODULE_DATA_T));
        SCI_MEMSET(module_data_ptr, 0x00, sizeof(RES_MODULE_DATA_T));

        /* get every project module info */
        data_offset = (uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_COLOR].pData;
        count = (uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_COLOR].uCount;
        size = pRes->res_module_arry[i].mmi_data_table[MMI_RT_COLOR].uSize;
        offset = GetModuleInfoAbsOffset(pRes, i) + data_offset;

       
        if ( (0 != count) && (0 != size))
        {
            data_buffer = (uint8 *)ResMalloc( pRes->res_location.store_type, count*sizeof(MMIRES_COLOR_T));
            SCI_ASSERT(data_buffer != PNULL); /*assert verified*/
            SCI_MEMSET(data_buffer, 0x00, (count*sizeof(MMIRES_COLOR_T)));            
            ReadResource( pRes->res_handle, data_buffer, offset, count*sizeof(MMIRES_COLOR_T), pRes->res_location.store_type );
            
            //为每个节点指定一个head数据空间
            module_data_ptr->module_id = i;
            module_data_ptr->size = count*sizeof(MMIRES_COLOR_T);
            module_data_ptr->unit_data_ptr = data_buffer;
            module_data_ptr->unit_offset_arry = 0;

            /* add module to link */
            AddNodeToLink(&pRes->color_prj_module_link_ptr, (uint32)module_data_ptr);                        
        }
        else
        {
            RES_FREE(module_data_ptr); /*lint !e63*/
        }
    }    
}

/*****************************************************************************/
//  Description : load text resource to ram 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void LoadTextRes(CAF_RES_T *pRes )
{
    uint8 * data_buffer = PNULL;
    uint32 * offset_buffer = PNULL;
    uint32  size= 0;
    uint16  i = 0 ;    
    uint32  j = 0;
    uint32  offset = 0;
    uint32  data_offset = 0;
    uint32  count = 0;    
    KSP_TEXT_T *ksp_text_ptr = PNULL;
    RES_MODULE_DATA_T *module_data_ptr = PNULL;
    MMISET_LANGUAGE_TYPE_E  language_type = MMISET_EDEFAULT_LANGUAGE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;        
    uint32 index = 0;
    
    /* read the type of language */
    MMINV_READ(MMINV_SET_LANGUAGE_TYPE,&language_type,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_LANGUAGE_TYPE,&language_type);
    }

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    //做一次实际的系统语言类型的转换，nv中的数据包含 auto类型，需要转换为确切的语言。
    language_type = (MMISET_LANGUAGE_TYPE_E)MMIAPISET_ConvertNVLangToSysType(language_type);
#endif

    index = GetCurLangIndex(language_type);
    
    /* 集中申请MMISET_MAX_LANGUAGE个KSP_TEXT_T数据空间,存放每种语言的text信息 */
    /* 这个buffer是用来读取每个project module中文字模块的多国语言信息的，
        然后再根据当前的语言language_type来选择对应的文字 */
    //if (PNULL == ksp_text_ptr)
    {
        ksp_text_ptr = (KSP_TEXT_T*)ResMalloc( pRes->res_location.store_type, sizeof(KSP_TEXT_T)*MMISET_MAX_LANGUAGE);
        SCI_ASSERT(ksp_text_ptr!=PNULL); /*assert verified*/
    }

    
    for (i =0; i<pRes->res_module_total; i++)
    {   
        if(IsModuleInfoEmpty(pRes, i))
        {
            continue;
        }

        /* malloc module buffer */
        module_data_ptr = (RES_MODULE_DATA_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_MODULE_DATA_T));
        SCI_MEMSET(module_data_ptr, 0x00, sizeof(RES_MODULE_DATA_T));

        /* get every project module info */
        data_offset = (uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_TEXT].pData;
        count = (uint32)pRes->res_module_arry[i].mmi_data_table[MMI_RT_TEXT].uCount;
        offset = GetModuleInfoAbsOffset(pRes, i) + data_offset;

        /* get multi country language info of every project module */
        SCI_MEMSET(ksp_text_ptr, 0x00, (sizeof(KSP_TEXT_T)*MMISET_MAX_LANGUAGE));
        ReadResource( pRes->res_handle, ksp_text_ptr, offset, sizeof(KSP_TEXT_T)*MMISET_MAX_LANGUAGE, pRes->res_location.store_type );

        /* get offset of indicated language text data */
        offset = GetModuleInfoAbsOffset(pRes, i) + (uint32)ksp_text_ptr[index].pTxtTable;

        size = (uint32)(ksp_text_ptr[index].nbText) * 4;//该语言该模块的text data大小
        
        if ( (0 != size) && (0 != count) )
        {
            offset_buffer = (uint32 *)ResMalloc( pRes->res_location.store_type, count*sizeof(uint32));
            SCI_ASSERT(offset_buffer != PNULL); /*assert verified*/
            SCI_MEMSET(offset_buffer, 0x00, (count*sizeof(uint32)));            
            ReadResource( pRes->res_handle, offset_buffer, offset, count*sizeof(uint32), pRes->res_location.store_type );
            
            for(j = 0; j < count; j ++)
            {
                if(0 != *(offset_buffer+j))
                {
                    data_offset = GetModuleInfoAbsOffset(pRes, i) + *(offset_buffer+j);
                    break;
                }
            }

            data_buffer = (uint8 *)ResMalloc( pRes->res_location.store_type, size);
            SCI_ASSERT(data_buffer != PNULL); /*assert verified*/
            SCI_MEMSET(data_buffer, 0x00, size);            
            ReadResource( pRes->res_handle, data_buffer,data_offset,size, pRes->res_location.store_type );

            //为每个text节点指定一个head数据空间
            module_data_ptr->module_id = i;
            module_data_ptr->size = size;
            module_data_ptr->unit_data_ptr = data_buffer;
            module_data_ptr->unit_offset_arry = offset_buffer;            
            module_data_ptr->offset_base = *(offset_buffer+j);

            /* add module to link */
            AddNodeToLink(&pRes->text_prj_module_link_ptr, (uint32)module_data_ptr);                        
        }
        else
        {
            RES_FREE(module_data_ptr); /*lint !e63*/
        }
    }
    /* free 多国语言buffer */
    //if ( PNULL != ksp_text_ptr)
    {
        RES_FREE(ksp_text_ptr); /*lint !e63*/
        ksp_text_ptr = PNULL;
    }
}

/*******************************************************************/
//  Interface:		MMIRES_ReadImageFromFile
//  Description : 	MMIRES_ReadImageFromFile
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMIRES_ReadImageDataFromFile(
											SFS_HANDLE handle, 
											uint8** buffer_pptr,
											uint32 read_bytes
											)
{
	BOOLEAN bret = FALSE;
	uint8* data_buff_ptr = PNULL;
	
	if (PNULL != buffer_pptr && 0 < read_bytes)
	{
		uint32 read_len = 0;
		
		/* malloc buffer for load resource data */
		data_buff_ptr = (uint8*)ResMalloc(RES_STORE_THEME_PACKAGE, read_bytes);
		SCI_ASSERT(data_buff_ptr != PNULL); /*assert verified*/
		SCI_MEMSET(data_buff_ptr, 0x00, read_bytes);
		
		if (SFS_ERROR_NONE == SFS_ReadFile(handle ,data_buff_ptr, read_bytes, &read_len, NULL))
		{
			*buffer_pptr = data_buff_ptr;
			bret = TRUE;	 
		}
	}
	
	return bret;
}

/*******************************************************************/
//  Interface:		LoadResNodeFormQbTheme
//  Description : 	LoadResNodeFormQbTheme
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL RES_NODE_DATA_T* LoadResNodeFormOut(
                                          const CAF_RES_T * com_res_info_ptr, 
                                          uint32	   res_id, 
                                          CAF_HANDLE_T handle_id,
                                          GETIMAGERESDATAFUNC func
                                          )
{
	uint8*      data_buffer_ptr = PNULL;
	uint32		data_buffer_len = 0;
    RES_NODE_DATA_T * res_node_ptr = PNULL;    
    
    if(com_res_info_ptr == PNULL || PNULL == func)
    {
        return PNULL;
    }
	
    func(res_id, &data_buffer_ptr, &data_buffer_len);
	
    /* config resource dynamic node */
	if (0 < data_buffer_len && PNULL != data_buffer_ptr)
	{
		res_node_ptr = (RES_NODE_DATA_T*)ResMalloc( RES_STORE_THEME_PACKAGE, sizeof(RES_NODE_DATA_T) );
		SCI_ASSERT(PNULL != res_node_ptr); /*assert verified*/
		SCI_MEMSET(res_node_ptr, 0x00, sizeof(RES_NODE_DATA_T));
		
		res_node_ptr->com_id        = com_res_info_ptr->com_id;
		res_node_ptr->resource_id   = res_id;
		res_node_ptr->type          = MMI_RT_IMAGE;
		res_node_ptr->data_ptr      = data_buffer_ptr;
		res_node_ptr->data_type     = 0;
		res_node_ptr->size          = data_buffer_len;                		
	}
	
    return res_node_ptr;   
}

/*******************************************************************/
//  Interface:		GetNorImgNodeFromQbTheme
//  Description : 	GetNorImgNodeFromQbTheme
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL RES_NODE_DATA_T* GetNorImgNodeFromOut(
                                            const CAF_RES_T * com_res_info_ptr, 
                                            uint32 res_id, 
                                            CAF_HANDLE_T handle_id,
                                            GETIMAGERESDATAFUNC func
                                            )
{
    RES_NODE_DATA_T* res_node_ptr = PNULL;  
    int32 handle_flag_index = 0;
	
    /* get handle flag bit index */
    handle_flag_index = SearchHandleFlagIndex(handle_id);
    if(0 > handle_flag_index)
    {
        /* add the handle to handle map */
        AddHandleToHandleMap(handle_id);        
    }
	
    /* whether the animation res had been loaded in dynamic Link */
    res_node_ptr = SearchResFromDLink(res_id, handle_id, com_res_info_ptr->com_id, MMI_RT_IMAGE);
    if(PNULL == res_node_ptr)
    {
		res_node_ptr = LoadResNodeFormOut(com_res_info_ptr, res_id, handle_id, func);
		
        if(PNULL != res_node_ptr)
        {
            /* set reference bit */
            SetResNodeRefFlag(res_node_ptr, handle_id);        
            
            /* add resource data to dynamic link */
            AddDynResNodeToLink(res_node_ptr);
        }
    }
    else
    {
        /* set reference bit */
        SetResNodeRefFlag(res_node_ptr, handle_id);
    }
	
    return res_node_ptr;
}

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
/*******************************************************************/
//  Interface:		LoadLivewallpaerResNode
//  Description : 	LoadLivewallpaerResNode
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL RES_NODE_DATA_T* LoadLivewallpaerResNode(
                                               const CAF_RES_T* com_res_info_ptr, 
                                               uint32       res_id,
                                               const char*  res_id_str,
                                               CAF_HANDLE_T handle_id
                                               )
{
    uint8*      data_buffer_ptr = PNULL;
    uint32		data_buffer_len = 0;
    RES_NODE_DATA_T * res_node_ptr = PNULL;    
    
    if(com_res_info_ptr == PNULL)
    {
        return res_node_ptr;
    }
    
    MMI_GetLivewallpaerImageResData(res_id, res_id_str, &data_buffer_ptr, &data_buffer_len);
    
    /* config resource dynamic node */
    if (0 < data_buffer_len && PNULL != data_buffer_ptr)
    {
        res_node_ptr = (RES_NODE_DATA_T*)ResMalloc( RES_STORE_THEME_PACKAGE, sizeof(RES_NODE_DATA_T) );
        SCI_ASSERT(PNULL != res_node_ptr); /*assert verified*/
        SCI_MEMSET(res_node_ptr, 0x00, sizeof(RES_NODE_DATA_T));
        
        res_node_ptr->com_id        = com_res_info_ptr->com_id;
        res_node_ptr->resource_id   = res_id;
        res_node_ptr->type          = MMI_RT_IMAGE;
        res_node_ptr->data_ptr      = data_buffer_ptr;
        res_node_ptr->data_type     = 0;
        res_node_ptr->size          = data_buffer_len;                		
    }
    
    return res_node_ptr;   
}

/*******************************************************************/
//  Interface:		GetLiveWallpaperImgNodeFromOutside
//  Description : 	GetLiveWallpaperImgNodeFromOutside
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL RES_NODE_DATA_T* GetLiveWallpaperImgNodeFromOutside(
                                                          const CAF_RES_T* com_res_info_ptr, 
                                                          uint32        res_id,
                                                          const char*   res_id_str,
                                                          CAF_HANDLE_T  handle_id
                                                          )
{
    RES_NODE_DATA_T* res_node_ptr = PNULL;  
    int32 handle_flag_index = 0;
    
    /* get handle flag bit index */
    handle_flag_index = SearchHandleFlagIndex(handle_id);
    if(0 > handle_flag_index)
    {
        /* add the handle to handle map */
        AddHandleToHandleMap(handle_id);        
    }
    
    /* whether the animation res had been loaded in dynamic Link */
    res_node_ptr = SearchResFromDLink(res_id, handle_id, com_res_info_ptr->com_id, MMI_RT_IMAGE);
    if(PNULL == res_node_ptr)
    {
        res_node_ptr = LoadLivewallpaerResNode(com_res_info_ptr, res_id, res_id_str, handle_id);
        
        if(PNULL != res_node_ptr)
        {
            /* set reference bit */
            SetResNodeRefFlag(res_node_ptr, handle_id);        
            
            /* add resource data to dynamic link */
            AddDynResNodeToLink(res_node_ptr);
        }
    }
    else
    {
        /* set reference bit */
        SetResNodeRefFlag(res_node_ptr, handle_id);
    }
    
    return res_node_ptr;
}
#endif

/*****************************************************************************/
//  Description : get animation data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL RES_NODE_DATA_T* GetResNode(const CAF_RES_T * com_res_info_ptr, uint32 res_id, CAF_HANDLE_T handle_id, MMI_RES_TYPE_E res_type)
{ 
    RES_NODE_DATA_T* res_node_ptr = PNULL;  
    int32 handle_flag_index = 0;
    
    //SCI_TRACE_LOW("[RES]: res id:0x%x, handle:0x%x", res_id, handle_id);
    
    /* get handle flag bit index */
    handle_flag_index = SearchHandleFlagIndex(handle_id);
    if(0 > handle_flag_index)
    {
        /* add the handle to handle map */
        AddHandleToHandleMap(handle_id);        
    }

    /* whether the animation res had been loaded in dynamic Link */
    res_node_ptr = SearchResFromDLink(res_id, handle_id, com_res_info_ptr->com_id, res_type);
    if(PNULL == res_node_ptr)/*lint !e831*/
    {
#ifdef MMI_RES_OUTSIDE_SUPPORT
        if (MMI_RT_IMAGE == res_type
            && MMIRESOUT_IsEnable()
            && MMI_SYS_RES_FLAG == (res_id & MMI_SYS_RES_FLAG))
        {
            res_node_ptr = LoadResNodeFormOut(com_res_info_ptr, res_id, handle_id, MMIRESOUT_GetImageResData);
        }
#endif
#ifdef QBTHEME_SUPPORT
		if (PNULL == res_node_ptr   /*lint !e774 !e831*/
            && MMI_RT_IMAGE == res_type /*lint !e831*/
			&& MMIAPIQBTHEME_GetNeedTransform()
			&& !MMIAPIQBTHEME_IsDefaultTheme()
			&& MMI_SYS_RES_FLAG == ( res_id & MMI_SYS_RES_FLAG ))/*lint !e774*/
		{
			res_node_ptr = LoadResNodeFormOut(com_res_info_ptr, res_id, handle_id, MMIQBTHEME_GetImageResData);
		}
		
		if (PNULL == res_node_ptr)
#endif
		{
        /* load this animation to Link */
        res_node_ptr = LoadResNode(com_res_info_ptr, res_id, handle_id, res_type);
		}
		
        if(PNULL != res_node_ptr)
        {
            /* set reference bit */
            SetResNodeRefFlag(res_node_ptr, handle_id);        
            
            /* add resource data to dynamic link */
            AddDynResNodeToLink(res_node_ptr);
        }
    }
    else
    {
        /* set reference bit */
        SetResNodeRefFlag(res_node_ptr, handle_id);
    }

    return res_node_ptr;
}


/*****************************************************************************/
//  Description : get animation data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL uint32 GetComIdByHandle( CAF_HANDLE_T handle_id )
{
    uint32     com_id = 0;
    CAF_GUID_T guid   = MMK_GetGuidByHandle( handle_id );
    
    if( 0 != guid )
    {
        MMI_MODINFO_NODE_T* mod_info_ptr = MMK_SearchModInfo(guid, MMI_QUERY_BY_GUID);

        SCI_ASSERT( PNULL != mod_info_ptr ); /*assert verified*/

        com_id = mod_info_ptr->mod_id;
    }

    return com_id;
}

/*****************************************************************************/
//  Description : get Anim data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note: 
/*****************************************************************************/
PUBLIC uint8* GetNorAnim(
                              MMI_ANIM_ID_T anim_id, 
                              CAF_HANDLE_T handle_id, 
                              uint32 *size_ptr,
                              uint32 res_data_offset
                              )
{
    uint16  module_id       = MMI_GET_MODU_ID(anim_id);
    uint16  anim_index     = MMICOM_GET_WORD_LB(anim_id);
    PMMI_RESOURCE_BRIEF_T  module_addr_ptr  = PNULL;
    PMMIRES_MODULE_INFO_T  mmi_res_ptr      = PNULL;
    uint8           *anim_dat_ptr        = PNULL;
    MMI_ANIM_DATA_T  *anim_ptr            = PNULL;
    uint32          *anim_data_table_ptr = PNULL;
    ANIM_FLAG_T      *anim_flag           = PNULL;
    uint8           *ret_ptr            = PNULL;
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(0 != anim_index); /*assert verified*/
    module_addr_ptr  = (PMMI_RESOURCE_BRIEF_T)(res_data_offset + s_mmi_res_bin_ptr);     
    if(0 != module_addr_ptr->mmi_module_adrr[module_id])
    {
        mmi_res_ptr      = (PMMIRES_MODULE_INFO_T)(module_addr_ptr->mmi_module_adrr[module_id] + (uint32)module_addr_ptr);
        /* get image ptr info */
        if(anim_index <= mmi_res_ptr->mmi_data_table[MMI_RT_ANIM].uCount)
        {
            anim_dat_ptr        = (uint8 *)((uint32)mmi_res_ptr->mmi_data_table[MMI_RT_ANIM].pData + (uint32)mmi_res_ptr);
            anim_ptr            = (PMMI_ANIM_DATA_T)(anim_dat_ptr);   
            anim_data_table_ptr = (uint32 *)((uint32)mmi_res_ptr + (uint32)anim_ptr->pAnimData); 
            
            if(0 != anim_data_table_ptr[anim_index - 1])
            {
                if (PNULL!= size_ptr)
                {
                    anim_flag = (PANIM_FLAG_T)((uint32)mmi_res_ptr + (uint32)anim_ptr->pFlag + (anim_index-1)*sizeof(ANIM_FLAG_T));            
                    *size_ptr = anim_flag->nDataSize;
                }
                
                ret_ptr = (uint8 *)(anim_data_table_ptr[anim_index - 1] + (uint32)mmi_res_ptr);
                return (uint8*)ret_ptr;
            }
        }
    }
    return PNULL;
}

/*****************************************************************************/
//  Description : get animation data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL uint8* GetAnim(const CAF_RES_T * com_res_info_ptr, MMI_ANIM_ID_T anim_id, CAF_HANDLE_T handle_id, uint32 *size_ptr)
{       
    uint8 * ret_ptr = PNULL;
    if(RES_STORE_FLASH_NOR != com_res_info_ptr->res_location.store_type)
    {            
        RES_NODE_DATA_T* res_node_ptr = PNULL;      
        
        /* get animation node */
        res_node_ptr = GetResNode(com_res_info_ptr, anim_id, handle_id, MMI_RT_ANIM);    
        if(res_node_ptr != PNULL)
        {            
            /* set animation size */
            if (NULL != size_ptr)
            {
                *size_ptr = res_node_ptr->size;
            }
            /* set animation data ptr */
            ret_ptr = res_node_ptr->data_ptr;
            SCI_ASSERT(ret_ptr!=PNULL); /*assert verified*/
        }
    }
    else
    {        
        /* 从资源包中获取anim */
        ret_ptr = GetNorAnim(anim_id, handle_id, size_ptr, com_res_info_ptr->res_data_offset);                               
    }
        
    return ret_ptr;
}

/*****************************************************************************/
//  Description : get animation data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIRES_GetAnim(MMI_ANIM_ID_T anim_id, CAF_HANDLE_T handle_id, uint32 *size_ptr)
{   
    uint32  com_id  = 0;
    uint8 * ret_ptr = PNULL;
    CAF_RES_T* caf_res_ptr = PNULL;
    
    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(anim_id))
    {
        return PNULL;
    }
    
    if( MMI_SYS_RES_FLAG == ( anim_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }
    
    anim_id = anim_id & ( ~MMI_SYS_RES_FLAG );
    
    
    if( 0 == com_id || 0 == anim_id)
    {
        //SCI_TRACE_LOW:"MMI_GetLabelAnim com_id = %d, anim_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_3674_112_2_18_2_0_20_240,(uint8*)"dd", com_id, anim_id);
        return PNULL;
    }
    
    caf_res_ptr = GetComCafResInfo(com_id);
    
    if(caf_res_ptr->is_caf_resource) //load caf application resource
    {
        return CAFRES_GetAnim(anim_id, handle_id, size_ptr);
    }
    else
    {      

#ifdef MMI_RES_ORIENT_BOTH
        if (MMITHEME_IsMainScreenLandscape())
        {
            if(PNULL != s_appoint_landscape_res_ptr)
            {
                ret_ptr = GetAnim( s_appoint_landscape_res_ptr, anim_id, handle_id, size_ptr);
            }

            if(PNULL == ret_ptr)
            {
                /* 如果没有获取到，则从default包中获取anim */
                ret_ptr = GetAnim( &s_landscape_res, anim_id, handle_id, size_ptr);
            }
            
            if(PNULL == ret_ptr)
            {
                /* 如果没有获取到，则从default包中获取anim */
                ret_ptr = GetAnim(GetComCafResInfo(com_id), anim_id, handle_id, size_ptr);
            }
        }
        else
#endif
        {
            if(PNULL != s_appoint_res_ptr)
            {
                /* 从其他资源包中获取anim */
                ret_ptr = GetAnim(s_appoint_res_ptr, anim_id, handle_id, size_ptr);
            }
            
            if(PNULL == ret_ptr)
            {
                /* 如果没有获取到，则从default包中获取anim */
                ret_ptr = GetAnim(GetComCafResInfo(com_id), anim_id, handle_id, size_ptr);
            }
        }


        return ret_ptr;
    }
}

/*****************************************************************************/
//  Description : get Image data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note: add this fuc to get size of image,the fuc is temp for test
/*****************************************************************************/
PUBLIC uint8* GetNorImage(
                              MMI_IMAGE_ID_T image_id, 
                              CAF_HANDLE_T handle_id, 
                              uint32 *size_ptr,
                              uint32 res_data_offset
                              )
{
    uint16  module_id       = MMI_GET_MODU_ID(image_id);
    uint16  image_index     = MMICOM_GET_WORD_LB(image_id);
    PMMI_RESOURCE_BRIEF_T  module_addr_ptr  = PNULL;
    PMMIRES_MODULE_INFO_T  mmi_res_ptr      = PNULL;
    uint8           *img_dat_ptr        = PNULL;
    MMI_IMG_DATA_T  *img_ptr            = PNULL;
    uint32          *img_data_table_ptr = PNULL;
    IMG_FLAG_T      *img_flag           = PNULL;
    uint8           *ret_ptr            = PNULL;
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(0 != image_index); /*assert verified*/
    module_addr_ptr  = (PMMI_RESOURCE_BRIEF_T)(res_data_offset + s_mmi_res_bin_ptr);     
    if(0 != module_addr_ptr->mmi_module_adrr[module_id])
    {
        mmi_res_ptr      = (PMMIRES_MODULE_INFO_T)(module_addr_ptr->mmi_module_adrr[module_id] + (uint32)module_addr_ptr);
        /* get image ptr info */
        if(image_index <= mmi_res_ptr->mmi_data_table[MMI_RT_IMAGE].uCount)
        {
            img_dat_ptr        = (uint8 *)((uint32)mmi_res_ptr->mmi_data_table[MMI_RT_IMAGE].pData + (uint32)mmi_res_ptr);
            img_ptr            = (PMMI_IMG_DATA_T)(img_dat_ptr);   
            img_data_table_ptr = (uint32 *)((uint32)mmi_res_ptr + (uint32)img_ptr->pImgData); 
            
            if(0 != img_data_table_ptr[image_index - 1])
            {
                if (PNULL!= size_ptr)
                {
                    img_flag = (PIMG_FLAG_T)((uint32)mmi_res_ptr + (uint32)img_ptr->pFlag + (image_index-1)*sizeof(IMG_FLAG_T));            
                    *size_ptr = img_flag->nSize;
                }
                
                ret_ptr = (uint8 *)(img_data_table_ptr[image_index - 1] + (uint32)mmi_res_ptr);
                return (uint8*)ret_ptr;
            }
        }
    }
    return PNULL;
}

/*****************************************************************************/
//  Description : get Image data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note: add this fuc to get size of image,the fuc is temp for test
/*****************************************************************************/
LOCAL uint8* GetImage(
                      const CAF_RES_T * com_res_info_ptr, 
                      MMI_IMAGE_ID_T image_id, 
                      CAF_HANDLE_T handle_id, 
                      uint32 *size_ptr
                      )
{   
    uint8 * ret_ptr = PNULL;
    if(RES_STORE_FLASH_NOR != com_res_info_ptr->res_location.store_type)   
    {    
        RES_NODE_DATA_T* res_node_ptr = PNULL;      
        
        /* get image node */
        res_node_ptr = GetResNode(com_res_info_ptr, image_id, handle_id, MMI_RT_IMAGE);
        if(res_node_ptr != PNULL)
        {
            /* set image size */
            if (NULL != size_ptr)
            {
                *size_ptr = res_node_ptr->size;
            }
            /* set image data ptr */
            ret_ptr = res_node_ptr->data_ptr;
            SCI_ASSERT(ret_ptr!=PNULL); /*assert verified*/
        }
    }
    else    
    {
        RES_NODE_DATA_T* res_node_ptr = PNULL;      

#ifdef MMI_RES_OUTSIDE_SUPPORT
        if (MMIRESOUT_IsEnable()
            && MMI_SYS_RES_FLAG == ( image_id & MMI_SYS_RES_FLAG ))
        {
            res_node_ptr = GetNorImgNodeFromOut(com_res_info_ptr, image_id, handle_id, MMIRESOUT_GetImageResData);
        }
#endif

#ifdef QBTHEME_SUPPORT		
        if (PNULL == res_node_ptr
            && MMIAPIQBTHEME_GetNeedTransform()
            && !MMIAPIQBTHEME_IsDefaultTheme()
            && MMI_SYS_RES_FLAG == ( image_id & MMI_SYS_RES_FLAG ))/*lint !e774 !e831*/
        {
            res_node_ptr = GetNorImgNodeFromOut(com_res_info_ptr, image_id, handle_id, MMIQBTHEME_GetImageResData);
        }
#endif
        if (PNULL != res_node_ptr) /*lint !e774 !e831*/
        {
            /* set image size */
            if (NULL != size_ptr)
            {
                *size_ptr = res_node_ptr->size;
            }
            ret_ptr = res_node_ptr->data_ptr;
        }
        if (PNULL == ret_ptr)
        {
            ret_ptr = GetNorImage(image_id, handle_id, size_ptr, com_res_info_ptr->res_data_offset);        
        }
    }
 
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT   
    if (PNULL != ret_ptr
        && 'u' == ret_ptr[0]
        && 's' == ret_ptr[1]
        && 'e' == ret_ptr[2]
        && 'r' == ret_ptr[3] 
        && IMG_USER_LIVE_WALLPAPER == *(ret_ptr + 4))
    {
        RES_NODE_DATA_T* lw_node_ptr = PNULL;      
        
        lw_node_ptr = GetLiveWallpaperImgNodeFromOutside(com_res_info_ptr, \
            image_id, (char*)(ret_ptr + USER_HEAD_SIZE), handle_id);
        if (PNULL != lw_node_ptr)
        {
            /* set image size */
            if (NULL != size_ptr)
            {
                *size_ptr = lw_node_ptr->size;
            }
            ret_ptr = lw_node_ptr->data_ptr;
        }
    }
#endif

    return ret_ptr;
}

/*****************************************************************************/
//  Description : get Image data pointer and its size
//  Global resource dependence : none
//  Author: gang.tong
//  Note: add this fuc to get size of image,the fuc is temp for test
/*****************************************************************************/
PUBLIC uint8* MMIRES_GetImage(
                              MMI_IMAGE_ID_T image_id, 
                              CAF_HANDLE_T handle_id, 
                              uint32 *size_ptr
                              )
{
    uint32  com_id  = 0;
    uint8 * ret_ptr = PNULL;
    CAF_RES_T* caf_res_ptr = PNULL;
    
    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(image_id))
    {
        return PNULL;
    }
    
    if( MMI_SYS_RES_FLAG == ( image_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }

#ifdef MMI_RES_OUTSIDE_SUPPORT
    if (MMIRESOUT_IsEnable()
        && MMIRESOUT_GetImageResData(image_id, PNULL, PNULL))
    {
    }
    else
#endif    
#ifdef QBTHEME_SUPPORT
    if (MMIAPIQBTHEME_GetNeedTransform()
        && !MMIAPIQBTHEME_IsDefaultTheme()
        && MMIQBTHEME_GetImageResData(image_id, PNULL, PNULL))
        {
        }
    else
#endif
    {
        image_id = image_id & ( ~MMI_SYS_RES_FLAG );
    }
    
    if( 0 == com_id || 0 == image_id)
    {
        //SCI_TRACE_LOW:"MMI_GetLabelImage com_id = %d, image_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_3906_112_2_18_2_0_21_241,(uint8*)"dd", com_id, image_id);
        return PNULL;
    }
    
    caf_res_ptr = GetComCafResInfo(com_id);
    
    if(caf_res_ptr->is_caf_resource) //load caf application resource
    {
        return CAFRES_GetImage(image_id, handle_id, size_ptr);
    }
    else
    {     
#ifdef MMI_RES_ORIENT_BOTH
        if (MMITHEME_IsMainScreenLandscape())
        {
            if(PNULL != s_appoint_landscape_res_ptr)
            {
                ret_ptr = GetImage( s_appoint_landscape_res_ptr, image_id, handle_id, size_ptr);
            }
            
            if(PNULL == ret_ptr)
            {
                /* 如果没有获取到，则从default包中获取image */
                ret_ptr = GetImage( &s_landscape_res, image_id, handle_id, size_ptr);
            }
            
            if(PNULL == ret_ptr)
            {
                /* 如果没有获取到，则从default包中获取image */
                ret_ptr = GetImage(GetComCafResInfo(com_id), image_id, handle_id, size_ptr);
            }
        }
        else
#endif
        {
            if(PNULL != s_appoint_res_ptr)
            {
                /* 从其他资源包中获取image */
                ret_ptr = GetImage(s_appoint_res_ptr, image_id, handle_id, size_ptr);
                //SCI_TRACE_LOW("[gang.tong] offset =0x%x", s_appoint_res_ptr->res_data_offset);
                //SCI_TRACE_LOW("[gang.tong] s_appoint_res_ptr=0x%x, image_id = %d, ret = 0x%x", s_appoint_res_ptr, image_id, ret_ptr);
            }
            
            if(PNULL == ret_ptr)
            {
                /* 如果没有获取到，则从default包中获取image */
                ret_ptr = GetImage(GetComCafResInfo(com_id), image_id, handle_id, size_ptr);
            }
        }

        return ret_ptr;
    }
}

/*****************************************************************************/
//  Description : to get the ring by label
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetBinary(
                                  const CAF_RES_T * com_res_info_ptr, 
                                  uint32 res_id,
                                  CAF_HANDLE_T handle_id,
                                  MMIAUD_RING_DATA_INFO_T * pRingInfo,
                                  MMI_RES_TYPE_E res_type
                                  )
{    
    uint16  module_index = MMICOM_GET_WORD_HB(res_id);
    uint16  res_index = MMICOM_GET_WORD_LB(res_id);
    PMMI_RESOURCE_BRIEF_T   mmi_res_header  = PNULL;
    PMMIRES_MODULE_INFO_T   mmi_res_ptr     = PNULL;
    PMMIAUD_RING_DATA_INFO_T ring_info_ptr  = PNULL;
    BOOLEAN ret = FALSE;
    SCI_ASSERT(MMI_RT_RING == res_type || MMI_RT_DATA == res_type); /*assert verified*/
    if(RES_STORE_FLASH_NOR != com_res_info_ptr->res_location.store_type)
    {
        RES_NODE_DATA_T * res_node_ptr = PNULL;
        /* get ring node */
        res_node_ptr = GetResNode(com_res_info_ptr, res_id, handle_id, res_type);    
        if(PNULL != res_node_ptr)
        {   
            pRingInfo->data_ptr = res_node_ptr->data_ptr;    
            pRingInfo->data_len = res_node_ptr->size;
            pRingInfo->type = res_node_ptr->data_type;
            ret = TRUE;
        }
    }
    else
    {   
        mmi_res_header = (PMMI_RESOURCE_BRIEF_T)(com_res_info_ptr->res_data_offset  + s_mmi_res_bin_ptr);
        if(0 != mmi_res_header->mmi_module_adrr[module_index])
        {
            mmi_res_ptr     = (PMMIRES_MODULE_INFO_T)(mmi_res_header->mmi_module_adrr[module_index] + (uint32)mmi_res_header);        
            if(res_index <= mmi_res_ptr->mmi_data_table[res_type].uCount)
            {
                ring_info_ptr = (PMMIAUD_RING_DATA_INFO_T)((uint32)mmi_res_ptr->mmi_data_table[res_type].pData + (uint32)mmi_res_ptr);
                
                //SCI_ASSERT( label >= 0 && label < RING_MAX_ID );
                SCI_ASSERT( pRingInfo != PNULL ); /*assert verified*/
                
                ring_info_ptr += (res_index - 1);
                if((0 != ring_info_ptr->data_len) && (PNULL != ring_info_ptr->data_ptr))
                {                    
                    pRingInfo->type     = ring_info_ptr->type;
                    pRingInfo->data_len = ring_info_ptr->data_len;
                    pRingInfo->data_ptr = (uint8 *)((uint32)mmi_res_ptr + (uint32)ring_info_ptr->data_ptr);                
                    
                    ret = TRUE;
                }
            }
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description : to get the ring by label
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetDataInfo(
                                  MMI_DATA_ID_T data_id,
                                  CAF_HANDLE_T handle_id,
                                  MMIRES_DATA_INFO_T * pdataInfo                                  
                                  )
{
    return GetRawResData(MMI_RT_DATA, data_id, handle_id, pdataInfo);         
}

/*****************************************************************************/
//  Description : to get the ring by label
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetRingInfo(
                                  MMI_RING_ID_T ring_id,
                                  CAF_HANDLE_T handle_id,
                                  MMIAUD_RING_DATA_INFO_T * pRingInfo
                                  )
{
    CAF_RES_T* caf_res_ptr = PNULL;
    uint32  com_id  = 0;
    
    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(ring_id))
    {
        return PNULL;
    }
    
    if( MMI_SYS_RES_FLAG == ( ring_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }
    
    ring_id = ring_id & ( ~MMI_SYS_RES_FLAG );
    
    if( 0 == com_id )
    {
        return PNULL;
    }

//记录最近的铃声，不删除
#ifdef NANDBOOT_SUPPORT
    s_last_ring_id = ring_id;
#endif
    
    caf_res_ptr = GetComCafResInfo(com_id);
    
    if(caf_res_ptr->is_caf_resource) //load caf application resource
    {
        return CAFRES_GetRingInfo(ring_id, handle_id, pRingInfo);
    }
    else
    {      
        return GetRawResData(MMI_RT_RING, ring_id, handle_id, pRingInfo); 
    }       
}

/*****************************************************************************/
//  Description : to get the raw res data by label
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetRawResDataProcess(                                   
                                  const CAF_RES_T * com_res_info_ptr, 
                                  MMI_RES_TYPE_E res_type,
                                  uint32 res_id,
                                  CAF_HANDLE_T handle_id,
                                  void * data_ptr
                                  )
{
    BOOLEAN ret = FALSE;
    switch(res_type)
    {    
    case MMI_RT_DATA:
        ret = GetBinary(com_res_info_ptr, res_id, handle_id, data_ptr, MMI_RT_DATA);
        break;
    case MMI_RT_COLOR:
        ret = GetColor(com_res_info_ptr, res_id, data_ptr);
        break;
    case MMI_RT_RECT:
        ret = GetRect(com_res_info_ptr, res_id, data_ptr);
        break;
    case MMI_RT_RING:
        ret = GetBinary(com_res_info_ptr, res_id, handle_id, data_ptr, MMI_RT_RING);
        break;
    default:
        SCI_ASSERT(0); /*assert verified*/
        break;
    }
    return ret;
}

/*****************************************************************************/
//  Description : to get the raw res data by label
//  Global resource dependence : 
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GetRawResData(
                             MMI_RES_TYPE_E res_type,
                             uint32 res_id,
                             CAF_HANDLE_T handle_id,
                             void * data_ptr
                             )
{
    uint32  com_id = 0;
    BOOLEAN ret = FALSE;

    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(res_id))
    {
        return PNULL;
    }
    
    com_id  = GetComIdByHandle( handle_id );
    
    if( MMI_SYS_RES_FLAG == ( res_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }

    res_id = res_id & ( ~MMI_SYS_RES_FLAG );

    if( 0 == com_id || 0 == res_id)
    {
        return FALSE;
    }
    
    if(PNULL != s_appoint_res_ptr)
    {
        /* 从其他资源包中获取ring */
        ret = GetRawResDataProcess(s_appoint_res_ptr, res_type, res_id, handle_id, data_ptr);        
    }
    
    if(!ret)
    {
        /* 如果没有获取到，则从default包中获取ring */
        ret = GetRawResDataProcess(GetComCafResInfo(com_id), res_type, res_id, handle_id, data_ptr);
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : get rect value by Label
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetRect(
                                   MMI_RECT_ID_T      rect_id,     
                                   CAF_HANDLE_T       handle_id,
                                   GUI_RECT_T       * rect_ptr
                                   )    //[out] the data of rect
{    
    return GetRawResData(MMI_RT_RECT, rect_id, handle_id, rect_ptr);         
}

/*****************************************************************************/
//  Description : get color value by Label
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetColor(
                                   MMI_COLOR_ID_T      color_id,     
                                   CAF_HANDLE_T       handle_id, 
                                   GUI_COLOR_T          *color_ptr
                                   )    //[out] the data of color                                   
{   
    return GetRawResData(MMI_RT_COLOR, color_id, handle_id, color_ptr);         
}

/*****************************************************************************/
//  Description : Get Ascii Res Text
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetAsciiResText(MMI_STRING_T *str_ptr, uint8 *re_str_ptr)
{
    if (PNULL == str_ptr || PNULL == re_str_ptr)
    {
        return;
    }

#ifndef MMI_RES_TEXT_ALL_UNICODE_SUPPORT
    str_ptr->wstr_len = (uint16)strlen((char *)re_str_ptr);

    if (0 == str_ptr->wstr_len)
    {
        str_ptr->wstr_ptr = PNULL;

        return;
    }
    
    if (s_text_cache_offset + str_ptr->wstr_len + 2 > MMI_TEXT_CACHE_BUF_SIZE)//align:4bytes
    {
        //SCI_TRACE_LOW:"[MMIRES] Text cache full. offset = %d, wstr_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_4226_112_2_18_2_0_21_242,(uint8*)"dd", s_text_cache_offset, str_ptr->wstr_len);

        s_text_cache_offset = 0;
    }
    
    MMI_STRNTOWSTR(s_text_cache_buf + s_text_cache_offset, str_ptr->wstr_len,
        re_str_ptr, str_ptr->wstr_len, str_ptr->wstr_len);
    
    str_ptr->wstr_ptr = (wchar *)(s_text_cache_buf + s_text_cache_offset);
    
    s_text_cache_offset += str_ptr->wstr_len;

    //add ending character
    if (0 == (s_text_cache_offset % 2))
    {
        //align:4bytes
        s_text_cache_buf[s_text_cache_offset] = 0x00;
        s_text_cache_buf[s_text_cache_offset + 1] = 0x00;
        s_text_cache_offset += 2;
    }
    else
    {
        //padding to even address //align:4bytes
        s_text_cache_buf[s_text_cache_offset] = 0x00;
        s_text_cache_offset += 1;
    }
#else
    SCI_ASSERT (*re_str_ptr < 0x80); /*assert verified*/
    str_ptr->wstr_ptr = (wchar*)re_str_ptr;
    //str_ptr->is_ucs2 = FALSE;
    str_ptr->wstr_len = (uint16)strlen((char*)re_str_ptr);
#endif
}

/*****************************************************************************/
//  Description : get Text ptr  by Label
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC const wchar* MMIRES_GetText(
                                   MMI_TEXT_ID_T      text_id,     //[in] the text id 
                                   CAF_HANDLE_T       handle_id,
                                   MMI_STRING_T       *str_ptr
                                   )    //[out] the data of text                                   
{    
    uint8           *re_str_ptr = PNULL;    
    uint32          com_id  = 0;
    CAF_RES_T       *caf_res_ptr = PNULL;
    MMI_TEXT_ID_T   convert_text_id = 0;
    
    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(text_id))
    {
        return PNULL;
    }
    
    if( MMI_SYS_RES_FLAG == ( text_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }
    
    convert_text_id = text_id & ( ~MMI_SYS_RES_FLAG );
    
    if( 0 == com_id )
    {
        return PNULL;
    }
    
    caf_res_ptr = GetComCafResInfo(com_id);
    
    if(caf_res_ptr->is_caf_resource) //load caf application resource
    {
        return CAFRES_GetText(convert_text_id, handle_id, str_ptr);
    }
    else
    {       
        if(PNULL != s_appoint_res_ptr)
        {
            caf_res_ptr = s_appoint_res_ptr;
            re_str_ptr = GetText(caf_res_ptr, convert_text_id, GetCurLangIndex(g_ksp_Lang));
        }
        
        if(PNULL == re_str_ptr)
        {
            caf_res_ptr = GetComCafResInfo(com_id);
            re_str_ptr = GetText(caf_res_ptr, convert_text_id, GetCurLangIndex(g_ksp_Lang));
        }
        
        SCI_ASSERT( PNULL != str_ptr ); /*assert verified*/
        if(MMI_IsTextNull(text_id))
        {
            //不正确的text_id
            return PNULL;
        }
        
        if (!IfTextResHasFlag(caf_res_ptr))
        {
            str_ptr->wstr_ptr = (uint16*)(re_str_ptr + 2); // no flag
            str_ptr->wstr_len = (uint16)(*((uint16*)re_str_ptr) / sizeof(wchar));
        }
        else
        {
        switch (*re_str_ptr)
        {
        case 0x80:
            str_ptr->wstr_ptr  = (uint16*)(re_str_ptr + 4); //for memory alignment
            //str_ptr->is_ucs2 = TRUE;
            str_ptr->wstr_len  = (uint16)(*((uint16*)(re_str_ptr + 2)) / sizeof(wchar));//(((*(re_str_ptr + 2)<<8) & 0xFF00) + *(re_str_ptr + 3))/sizeof(wchar);
            break;
            
        case 0x81://????
            SCI_ASSERT (0); /*assert verified*/
            break;
            
        case 0x82:
            SCI_ASSERT (0); /*assert verified*/
            break;
            
        default:
            GetAsciiResText(str_ptr, re_str_ptr);
            break;
            }
        }
        return str_ptr->wstr_ptr;
    }
}   

/*****************************************************************************/
//  Description : get Text ptr  by Label
//  Global resource dependence : none
//  Author: 
//  Note: Application should Free str_ptr->wstr_ptr
/*****************************************************************************/
PUBLIC const wchar* MMIRES_GetTextEx(
                                   MMI_TEXT_ID_T      text_id,     //[in] the text id 
                                   CAF_HANDLE_T       handle_id,
                                   MMI_STRING_T       *str_ptr
                                   )    //[out] the data of text
{    
    uint8           *re_str_ptr = PNULL;    
    uint32          com_id  = 0;
    CAF_RES_T       *caf_res_ptr = PNULL;
    MMI_TEXT_ID_T   convert_text_id = 0;
    
    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(text_id))
    {
        return PNULL;
    }
    
    if( MMI_SYS_RES_FLAG == ( text_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }
    
    convert_text_id = text_id & ( ~MMI_SYS_RES_FLAG );
    
    if( 0 == com_id )
    {
        return PNULL;
    }
    
    caf_res_ptr = GetComCafResInfo(com_id);
    
    if(caf_res_ptr->is_caf_resource) //load caf application resource
    {
        return CAFRES_GetText(convert_text_id, handle_id, str_ptr);
    }
    else
    {       
        if(PNULL != s_appoint_res_ptr)
        {
            caf_res_ptr = s_appoint_res_ptr;
            re_str_ptr = GetText(caf_res_ptr, convert_text_id, GetCurLangIndex(g_ksp_Lang));

        }
        
        if(PNULL == re_str_ptr)
        {
            caf_res_ptr = GetComCafResInfo(com_id);
            re_str_ptr = GetText(caf_res_ptr, convert_text_id, GetCurLangIndex(g_ksp_Lang));
        }
        
        SCI_ASSERT( PNULL != str_ptr ); /*assert verified*/
        if(MMI_IsTextNull(text_id))
        {
            //不正确的text_id
            return PNULL;
        }
        
        if (!IfTextResHasFlag(caf_res_ptr))
        {
            str_ptr->wstr_ptr = (uint16*)(re_str_ptr + 2); // no flag
            str_ptr->wstr_len = (uint16)(*((uint16*)re_str_ptr) / sizeof(wchar));
        }
        else
        {
        switch (*re_str_ptr)
        {
        case 0x80:
            str_ptr->wstr_ptr  = (uint16*)(re_str_ptr + 4); //for memory alignment
            //str_ptr->is_ucs2 = TRUE;
            str_ptr->wstr_len  = (uint16)(*((uint16*)(re_str_ptr + 2)) / sizeof(wchar));//(((*(re_str_ptr + 2)<<8) & 0xFF00) + *(re_str_ptr + 3))/sizeof(wchar);
            break;
            
        case 0x81://????
            SCI_ASSERT (0); /*assert verified*/
            break;
            
        case 0x82:
            SCI_ASSERT (0); /*assert verified*/
            break;
            
        default:
            str_ptr->wstr_len = (uint16)strlen((char*)re_str_ptr);
            str_ptr->wstr_ptr = SCI_ALLOC_APPZ((str_ptr->wstr_len + 1) * sizeof(wchar));

            if (PNULL != str_ptr->wstr_ptr)
            {
                MMI_STRNTOWSTR(str_ptr->wstr_ptr, str_ptr->wstr_len,
                    re_str_ptr, str_ptr->wstr_len, str_ptr->wstr_len);
            }
            break;
            }
        }
        return str_ptr->wstr_ptr;
    }
}   

/*****************************************************************************/
//  Description : check the text whether is NULL
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_IsTextNull(MMI_TEXT_ID_T text_id)
{
    BOOLEAN is_text_null = FALSE;

    // added COMMON_TXT_NULL by @arthur
    // bug fixed for some assert when using COMMON_TXT_NULL
    //  in menu.
    if( TXT_NULL == text_id || COMMON_TXT_NULL == text_id)
    {
        is_text_null = TRUE;
    }

    return is_text_null;
}

/*****************************************************************************/
//  Description : SetResLocationInfo
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
//#define MOD_MIF_RES "\x00\x44\x00\x3a\x00\x5c\x00\x43\x00\x6f\x00\x6d\x00\x5c\x00\x6e\x00\x00"
LOCAL RES_SET_LOCATION_E SetResLocationInfo(CAF_RES_T * pRes)
{    
    MMI_MODINFO_NODE_T * mod_info_ptr = PNULL;
    uint16 file_name_len = 0;
    
    mod_info_ptr = MMK_SearchModInfo(pRes->com_id, MMI_QUERY_BY_MODID);
    
    SCI_ASSERT( PNULL != mod_info_ptr ); /*assert verified*/
    
    switch(mod_info_ptr->mod_type)
    {
    case CAF_MOD_DYNAMIC:
        pRes->res_location.store_type = RES_STORE_CAF_FILE;    
        pRes->res_location.res_file   = mod_info_ptr->module_info.res_file_name; 
        pRes->is_caf_resource = TRUE;        
        /* check resource file exist */
        if( ( file_name_len = (uint16)MMIAPICOM_Wstrlen( mod_info_ptr->module_info.res_file_name ) ) > 0)
        {
            if(!MMIAPIFMM_IsFileExist(mod_info_ptr->module_info.res_file_name, file_name_len ) )
            {
                return RES_SET_LOCATION_FAILED;
            }
        }
        else
        {
            return RES_SET_LOCATION_NEED_NOT_RES;
        }
        
        break;
    case CAF_MOD_STATIC:
#ifdef NANDBOOT_SUPPORT
        pRes->res_location.store_type = RES_STORE_FLASH_NAND;
#else
        pRes->res_location.store_type = RES_STORE_FLASH_NOR;
#endif
        break;
    default:
        SCI_ASSERT(0); /*assert verified*/
        break;
    }                
    
    return RES_SET_LOCATION_SUCCEEDED;
}
/*****************************************************************************/
//  Description : load all static  com resource to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_LoadAllStaticComResource(void)
{
    MMI_MODINFO_LIST_T *  modinfo_head_ptr = PNULL;
    MMI_MODINFO_NODE_T *  modinfo_node_ptr = PNULL;
    uint32 i = 0;
    modinfo_head_ptr = MMK_GetStaticModInfoList();
    SCI_ASSERT(PNULL != modinfo_head_ptr); /*assert verified*/
    modinfo_node_ptr = modinfo_head_ptr->modinfo_list_ptr;
    for(i = 0; i < modinfo_head_ptr->mod_num; i ++)
    {
        MMIRES_LoadComResource(modinfo_node_ptr->mod_id);
        modinfo_node_ptr = modinfo_node_ptr->next_ptr;
    }

#ifdef MMI_RES_ORIENT_BOTH 
    s_landscape_res.com_id = MMIRES_DEFAULT_COM_ID;
    s_landscape_res.res_location.store_type =
#ifdef NANDBOOT_SUPPORT        
        RES_STORE_FLASH_NAND;
#else
        RES_STORE_FLASH_NOR;
#endif
    
    s_landscape_res.res_data_offset = s_package_info_ptr->addition_info[0].offset;
    
    LoadResourceData(&s_landscape_res);
#endif

    //初始化显示引擎, 必须在font模块资源初始化之后
#ifdef SCRIPTEASY_SUPPORT
    GUI_Initialize_Scripteasy_Library(179);
#else
    SPMLAPI_Init( &s_spml_init_data );
#endif
}

/*****************************************************************************/
//  Description : load resource content or info to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadResourceData(CAF_RES_T * res_info_ptr)
{
    //modified by andrew.zhang at 2009/09/30
    if(res_info_ptr->is_caf_resource) //load caf application resource
    {
        return CAF_LoadResourceData(res_info_ptr);
    }
    else
    {
        if(RES_STORE_FLASH_NOR != res_info_ptr->res_location.store_type)
        {
            res_info_ptr->res_handle = OpenResource( res_info_ptr->res_location.store_type, res_info_ptr->res_location.res_file );        
            
            /* get resource header info */
            GetComResHeadInfo( res_info_ptr );
            /* get image info */
            GetImageResInfo(res_info_ptr);
            /* get animation info */
            GetAnimResInfo(res_info_ptr);
            /* get ring info */
            GetRingResInfo(res_info_ptr);
            /* get data info */
            GetDataResInfo(res_info_ptr);
            if(res_info_ptr == GetComCafResInfo(MMIRES_DEFAULT_COM_ID))
            {
                /* load font content */
                LoadFontRes(res_info_ptr);                
            }
            /* load text content */
            LoadTextRes(res_info_ptr);
            /* load color content */
            LoadColorRes(res_info_ptr);
            /* load color content */
            LoadRectRes(res_info_ptr);
        }
        else
        {   
            /* load font content */
            LoadFontRes(res_info_ptr);                   
        }
    }
    
    return TRUE;
}
/*****************************************************************************/
//  Description : load com resource to ram 
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_LoadComResource(uint32 com_id)
{   
    CAF_RES_T * res_info_ptr = PNULL;

    //SCI_TRACE_LOW:"[RES]: MMIRES_LoadComResource comd_id = %d start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_4623_112_2_18_2_0_22_243,(uint8*)"d", com_id);

    if(PNULL == GetComCafResInfo(com_id))
    {           
        RES_SET_LOCATION_E result = 0;
        /* get not used res info space */
        res_info_ptr = GetComCafResInfo(MMI_INVALID_COM_ID);
        SCI_ASSERT(PNULL != res_info_ptr); /*assert verified*/
        SCI_MEMSET(res_info_ptr, 0, sizeof(CAF_RES_T));
        res_info_ptr->com_id = com_id;                  

        result = SetResLocationInfo( res_info_ptr );
        /* set resource location info */
        if( RES_SET_LOCATION_FAILED == result )
        {        
            SCI_MEMSET(res_info_ptr, 0, sizeof(CAF_RES_T));
            return FALSE;
        }
        else if( RES_SET_LOCATION_NEED_NOT_RES == result )
        {
            return TRUE;
        }
                
        if(MMIRES_DEFAULT_COM_ID == com_id)
        {
            /* load Content or Info of resource  */        
            res_info_ptr->res_data_offset = s_package_info_ptr->default_info.offset;
        }
        else
        {
            res_info_ptr->res_data_offset = 0;
        }

        LoadResourceData(res_info_ptr);
    }
    //SCI_TRACE_LOW:"[RES]: MMIRES_LoadComResource end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_4658_112_2_18_2_0_22_244,(uint8*)"");
    return TRUE;
}

/*****************************************************************************/
//  Description : Free dynamic resource node of indicated handle from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_FreeNodeResByHandle(CAF_HANDLE_T handle_id)
{
    FreeDynResNodeByHandle(handle_id);
}

/*****************************************************************************/
//  Description : Free all dynamic resource node from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_FreeAllNodeRes(void)
{
    FreeComNodeRes(MMIRES_DEFAULT_COM_ID, FALSE);
//     uint32 i = 0;
//     for (i = 0; i < MMI_RES_MAX_RECORD_WIN_NUM; i++)
//     {
//         if(0 != s_handle_map[i].handle_id)
//         {
//             MMIRES_FreeNodeResByHandle(s_handle_map[i].handle_id);            
//         }
//     }           
}

/*****************************************************************************/
//  Description : condition for find
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FoundByComIDCallback(MMI_LINK_NODE_T const* node_ptr, uint32 com_id, uint32 is_lcd_switch )
{
    BOOLEAN result = FALSE;
    RES_NODE_DATA_T * res_node_ptr = (RES_NODE_DATA_T*)(node_ptr->data);

    /* whether the handle flag contain the handle bit */
    if(res_node_ptr->com_id == com_id)
    {
        if ( ( !is_lcd_switch
            || ( MMI_RT_IMAGE == res_node_ptr->type || MMI_RT_ANIM == res_node_ptr->type ) )
#ifdef NANDBOOT_SUPPORT
            //不删除最近的铃声资源
            && ( !( res_node_ptr->resource_id == s_last_ring_id && MMI_RT_RING == res_node_ptr->type ) )
#endif
            )
        {
            result = TRUE;
        }
        //SCI_TRACE_LOW("[RES]:FoundByComIDCallback yes com_id = %d, res_id = 0x%x, size = %d, flag1 = 0x%x, flag2 = 0x%x, type = %d", res_node_ptr->com_id, res_node_ptr->resource_id, res_node_ptr->size, res_node_ptr->handle_bit_flag1, res_node_ptr->handle_bit_flag2, res_node_ptr->type);        
    }

    return result;
}


/*****************************************************************************/
//  Description : Free dynamic resource node from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void FreeDynResNodeByComID(
                                 uint32 com_id,
                                 BOOLEAN is_lcd_switch //是否横竖屏切换, 是则只清除图片资源
                                 )
{
    MMI_LINK_NODE_T * free_node_ptr = PNULL;    

    //SCI_TRACE_LOW:"[RES]: FreeDynResNodeByComID: com_id = %d, GetDLinkHeadPtr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_4728_112_2_18_2_0_22_245,(uint8*)"dd", com_id, GetDLinkHeadPtr());

    do                     
    {
        if(PNULL == GetDLinkHeadPtr())
        {
            break;
        }
        else
        {
            free_node_ptr = MMILINK_SearchNode(GetDLinkHeadPtr(), TRUE, FoundByComIDCallback, com_id, is_lcd_switch);
            
            if(PNULL != free_node_ptr)
            {                
                /* delete this res node from link */
                s_allocated_res_link_head = MMILINK_DestroyNode((MMI_LINK_NODE_T*)free_node_ptr, GetDLinkHeadPtr(), NodeDestructCallback);                 
            }
            //SCI_TRACE_LOW("[RES]: FreeDynResNodeByHandle, free end");
        }
    }
    while(PNULL != free_node_ptr);    
}

/*****************************************************************************/
//  Description : clear not used handle from handle map
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void ClearNotUsedHandleIndex(void)
{
    MMI_LINK_NODE_T * node_ptr = PNULL;    
    uint32 i = 0;
    if(PNULL != GetDLinkHeadPtr())
    {   
        for (i = 0; i < MMI_RES_MAX_RECORD_WIN_NUM; i++)
        {
            if(0 != s_handle_map[i].handle_id)
            {
                node_ptr = MMILINK_SearchNode(GetDLinkHeadPtr(), TRUE, FoundByHandleCallback, s_handle_map[i].handle_id, NULL);
                if(PNULL == node_ptr)
                {
                    DelHandleFromHandleMap(s_handle_map[i].handle_id);
                }
            }
        }
    }    
}

/*****************************************************************************/
//  Description : Free dynamic resource node of indicated com id from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
LOCAL void FreeComNodeRes(
                          uint32  com_id,
                          BOOLEAN is_lcd_switch //是否横竖屏切换, 是则只清除图片资源
                          )
{
    CAF_RES_T * res_info_ptr = PNULL;
    res_info_ptr = GetComCafResInfo(com_id);
    if(PNULL != res_info_ptr)
    {
        //if(RES_STORE_FLASH_NOR != res_info_ptr->res_location.store_type)
        {
            FreeDynResNodeByComID(com_id, is_lcd_switch);
            ClearNotUsedHandleIndex();            
        }
    }
}

/*****************************************************************************/
//  Description : Free dynamic resource node of indicated com id from link
//  Global resource dependence : none
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_EmptyComRes(uint32 com_id)
{
    CAF_RES_T * res_info_ptr = PNULL;
    res_info_ptr = GetComCafResInfo(com_id);    
    if(PNULL != res_info_ptr)
    {
        //SCI_TRACE_LOW:"[RES]: MMIRES_EmptyComRes: com_id = %d, store_type = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_4806_112_2_18_2_0_23_246,(uint8*)"dd", com_id, res_info_ptr->res_location.store_type);

        if(RES_STORE_FLASH_NOR != res_info_ptr->res_location.store_type)
        {
            FreeComNodeRes(com_id, FALSE);
            
            FreeCafResInfo(res_info_ptr);

            CloseResource( res_info_ptr->res_location.store_type, res_info_ptr->res_handle );

            SCI_MEMSET(res_info_ptr, 0, sizeof(CAF_RES_T));
        }
    }

    MMIRES_PrintfRESTabInfo();
}

/*****************************************************************************/
//  Description : load new text resource to ram when changing language
//  Global resource dependence : none
//  Author: yongwei.he
//  Note:
/*****************************************************************************/
PUBLIC void MMIRES_LoadNewTextRes(void)
{    
#ifdef NANDBOOT_SUPPORT 
    CAF_RES_T * res_info_ptr = PNULL;
    uint32 i = 0;
    
    //避免产生内存碎片
    MMIRES_FreeAllNodeRes();

    for(i = 0; i < MMI_CAF_RES_MAX; i ++)
    {
        res_info_ptr = &s_caf_res_info[i];//GetComCafResInfo(i);
        
        if( PNULL != res_info_ptr->text_prj_module_link_ptr )
        {   
            MMILINK_DestroyLink((MMI_LINK_NODE_T*)res_info_ptr->text_prj_module_link_ptr, TextDestructCallback);        
            res_info_ptr->text_prj_module_link_ptr = PNULL;
            
            //modified by andrew.zhang at 2009/09/30
            if(res_info_ptr->is_caf_resource) //load caf application resource
            {
                CAF_LoadTextRes( res_info_ptr );
            }
            else
            {
                LoadTextRes( res_info_ptr );
            }
        }
    }    
#endif
}

/*****************************************************************************/
//  Description : get current theme number
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint8 GetStaticThemeTotalNum(void)
{    
    return s_package_info_ptr->addition_num;
}

/*****************************************************************************/
//  Description : get current theme number
//  Global resource dependence : none
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIRES_GetStaticThemeTotalNum(void)
{    
    uint8 num = GetStaticThemeTotalNum();

    //支持横竖屏切换, 每种theme必须有两套
#ifdef MMI_RES_ORIENT_BOTH
    num = num / 2;
#endif

    return num;
}

///*****************************************************************************/
////    Description : get langfont from font and is_ucs2
////    Global resource dependence : none
////  Author: bin.ji
////    Note:
///*****************************************************************************/
//LOCAL MMI_LANG_FONT_E GetLangFont(
//                                  GUI_FONT_T font,
//                                  BOOLEAN is_ucs2     // 1: a gb char  0:an ascii char
//                                  )
//{
//    SCI_ASSERT( FALSE );
////    uint32 i = 0;
////    MMI_LANG_FONT_E lang_font = LANG_FONT_MAX_ID;
////
////    for (i = 0; i < ARR_SIZE(s_lang_font_table); i++)
////    {
////        if (font == s_lang_font_table[i].font)
////        {
////            if (is_ucs2)
////            {
////                lang_font = s_lang_font_table[i].ucs2_lang_font;
////            }
////            else
////            {
////                lang_font = s_lang_font_table[i].lang_font;
////            }
////            break;
////        }
////    }
////
////    return lang_font;
//}
/*****************************************************************************/
//  Description : get lang index from land status info table
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL uint32 GetCurLangIndex(uint8 lang)
{
    uint32 i = 0;
    int32 index = -1;
    for (i=0;i<MMISET_MAX_LANGUAGE;i++)
    {
        if (TRUE == s_lang_status_info[i][1])
        {
            index++;
            if (lang == s_lang_status_info[i][0])
            {
                break;
            }
        }
    }
    return index;
}

//james for i18n
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:分配内存
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_LoadFileFont( 
                                   const uint16 *file_name
                                   )
{
#ifdef SCRIPTEASY_SUPPORT
    return TRUE;
#else	
    BOOLEAN result    = FALSE;
    uint32  file_size = 0;

    MMIRES_UnloadFileFont();

    if ( 0 != ( s_font_file_handle = SFS_CreateFile( file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL ) ) )
    {
        SFS_GetFileSize( s_font_file_handle, &file_size );

        result = SPMLAPI_LoadFont( file_size );
    }

    return result;
#endif    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:释放内存
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_UnloadFileFont( void )
{
#ifdef SCRIPTEASY_SUPPORT
    return TRUE;
#else	
    BOOLEAN result = FALSE;

    result = SPMLAPI_UnloadFont();

    SFS_CloseFile( s_font_file_handle );
    s_font_file_handle = 0;

    return result;
#endif    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:分配内存
/*****************************************************************************/
LOCAL void* MMIRES_MemPoolMalloc( 
                                 uint32 handle, 
                                 uint32 size
                                 )
{
    void* ret_ptr = PNULL;

    if ( handle > 0 )
    {
        ret_ptr = SCI_MPALLOC( size, handle );  
    }

    return ret_ptr;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:释放内存
/*****************************************************************************/
LOCAL void MMIRES_MemPoolFree( 
                              void* mem_ptr
                              )
{
    SCI_MPFREE( mem_ptr );
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:重置内存
/*****************************************************************************/
LOCAL uint32 MMIRES_MemPoolConfig( 
                                  SPML_CACHE_TYPE_E type,
                                  uint32 handle,
                                  BOOLEAN is_create
                                  )
{
    uint32 result = 0;
    APP_MEM_CFG_T mem_cfg = {0};

    mem_cfg.pool_mode = MEM_POOL_RELEASE_MODE;

    if ( 0 != handle )
    {
        SCI_DeleteMemPool( handle, TRUE );
    }

    if ( SPML_VECTOR_CACHE == type )
    {
#if (VECTOR_CACHE_SIZE > 0)
        mem_cfg.min_pool_size      = 32;
        mem_cfg.pool_space_size    = VECTOR_CACHE_SIZE;
        result                     = SCI_CreateMemPool( "vector font cache", s_vector_cache, VECTOR_CACHE_SIZE, &mem_cfg );
        
#endif
    }
    else if ( SPML_FILE_CACHE == type )
    {
#if (STORE_CACHE_SIZE > 0)
        if ( PNULL != s_store_cache )
        {
            SCI_FREE( s_store_cache );
        }

        if ( is_create )
        {
            s_store_cache = SCI_ALLOCA( STORE_CACHE_SIZE );
        }

        if ( PNULL != s_store_cache )
        {
            mem_cfg.min_pool_size      = 4;
            mem_cfg.pool_space_size    = STORE_CACHE_SIZE;
            result                     = SCI_CreateMemPool( "store font cache", s_store_cache, STORE_CACHE_SIZE, &mem_cfg );
        }
#endif
    }
    else if ( SPML_GLYPH_CACHE == type )
    {
#if (GLYPH_CACHE_SIZE > 0)
        mem_cfg.min_pool_size      = 0;
        mem_cfg.heap_mem_watermark = 0;
        mem_cfg.pool_space_size    = 0;
        result                     = SCI_CreateMemPool( "glyph font cache", s_glyph_cache, GLYPH_CACHE_SIZE, &mem_cfg );
        
#endif
    }
    else if ( SPML_ADVANCE_CACHE == type )
    {
#if (ADVANCE_CACHE_SIZE > 0)
        mem_cfg.min_pool_size      = 32;
        mem_cfg.heap_mem_watermark = 0;
        mem_cfg.pool_space_size    = ADVANCE_CACHE_SIZE;
        result                     = SCI_CreateMemPool( "advance font cache", s_advance_cache, ADVANCE_CACHE_SIZE, &mem_cfg );
        
#endif
    }
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:读文件或flash
/*****************************************************************************/
LOCAL BOOLEAN MMIRES_GetFontData( 
                                 SPML_FONT_STORE_E store, 
                                 uint32 handle,
                                 void*  data_ptr,
                                 uint32 offset, 
                                 uint32 size
                                 )
{
    BOOLEAN result  = FALSE;

    if ( SPML_FONT_STORE_FILE == store )
    {
        if ( PNULL != data_ptr && 0 != s_font_file_handle )
        {
            if( SFS_ERROR_NONE == SFS_SetFilePointer( s_font_file_handle, (int64)offset, SFS_SEEK_BEGIN ) )
            {        
                if( SFS_ERROR_NONE == SFS_ReadFile( s_font_file_handle, data_ptr, size, &size, NULL ) )
                {
                    result = TRUE;
                }
            }
        }
    }
    else if ( SPML_FONT_STORE_NAND == store )
    {
#if defined NANDBOOT_SUPPORT && defined NAND_FONT_LOAD_SUPPORT
        GUI_FONT_DB_T* db_ptr = (GUI_FONT_DB_T*)s_font_ptr;

        result = ReadFlashResource( data_ptr, (uint32)db_ptr[handle].pdb + offset, size );
#endif
    }

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:如果存在阿拉伯语或希伯来语, 返回true
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_IsNeedBidi( void )
{
    //如果将来支持希伯来文, 需要加上或判断
#if (defined IM_ARABIC_SUPPORT) ||  (defined IM_PERSIAN_SUPPORT) || (defined IM_URDU_SUPPORT) || (defined IM_HEBREW_SUPPORT)
    if(TRUE == s_lang_status_info[MMISET_LANGUAGE_ARABIC][1]
        || TRUE == s_lang_status_info[MMISET_LANGUAGE_PERSIAN][1]
        || TRUE == s_lang_status_info[MMISET_LANGUAGE_URDU][1]
        || TRUE == s_lang_status_info[MMISET_LANGUAGE_HEBREW][1])
    {
        return TRUE;
    }
    else
#endif

	{
	    return FALSE;
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:获得字体库个数
/*****************************************************************************/
PUBLIC uint16 MMIRES_GetFontLibNum( void )
{
    uint16 total_num = 0;
    
#ifdef NANDBOOT_SUPPORT 
    CAF_RES_T * caf_res_info_ptr = GetComCafResInfo(MMIRES_DEFAULT_COM_ID);
    
    SCI_ASSERT(PNULL != caf_res_info_ptr); /*assert verified*/
    
    total_num = caf_res_info_ptr->res_module_arry[MMI_MODU_FONT_IDX].mmi_data_table[MMI_RT_FONT].uCount;
#else
    //uint8* ret_ptr = s_mmi_res_bin_ptr;

    MMIRES_MODULE_INFO_T * module_info_ptr = GetNorModuleInfo(GetComCafResInfo(MMIRES_DEFAULT_COM_ID), MMI_MODU_FONT_IDX);

    total_num = module_info_ptr->mmi_data_table[MMI_RT_FONT].uCount;
#endif
    
    return total_num;
    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:获得字体库的指针
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_GetFontLibPtr(
                                   uint16 index,
                                   uint8** lib_ptr, 
                                   uint32* lib_size
                                   )
{
    BOOLEAN result = FALSE;
    uint8 * ret_ptr = PNULL;
    GUI_FONT_DB_T* font_db_ptr = PNULL;

    SCI_ASSERT( PNULL != lib_ptr && PNULL != lib_size ); /*assert verified*/

    if ( index < MMIRES_GetFontLibNum() )
    {
#ifdef NANDBOOT_SUPPORT 

#ifdef NAND_FONT_LOAD_SUPPORT
        font_db_ptr = (GUI_FONT_DB_T*)s_font_ptr + index;
#else//else of NAND_FONT_LOAD_SUPPORT
        GUI_FONT_INFO_EX_T* font_info_ex_ptr = PNULL;
        uint32 offset = 0;
        CAF_RES_T * caf_res_info_ptr = GetComCafResInfo(MMIRES_DEFAULT_COM_ID);
        uint32 font_offset = (uint32)caf_res_info_ptr->res_module_arry[MMI_MODU_FONT_IDX].mmi_data_table[MMI_RT_FONT].pData;
        
        SCI_ASSERT(PNULL != caf_res_info_ptr); /*assert verified*/

        font_info_ex_ptr = (GUI_FONT_INFO_EX_T*)(s_font_ptr);

        font_info_ex_ptr += index;//font;

        offset = (uint32)font_info_ex_ptr->pdbtt - font_offset;
        font_db_ptr = (GUI_FONT_DB_T*)((uint32)(s_font_ptr+offset));
        
        offset = (uint32)font_db_ptr->pdb - font_offset;
        ret_ptr =(uint8 *)(GUI_FONT_DB_T*)((uint32)(s_font_ptr+offset));
#endif//end of NAND_FONT_LOAD_SUPPORT
#else//else of NANDBOOT_SUPPORT
        MMIRES_MODULE_INFO_T * module_info_ptr = GetNorModuleInfo(GetComCafResInfo(MMIRES_DEFAULT_COM_ID), MMI_MODU_FONT_IDX);    
        GUI_FONT_INFO_EX_T* font_info_ex_ptr = PNULL;
        
        font_info_ex_ptr = (GUI_FONT_INFO_EX_T*)((uint32)module_info_ptr->mmi_data_table[MMI_RT_FONT].pData + (uint32)module_info_ptr);
        font_info_ex_ptr += index;//font;
        
        font_db_ptr = (GUI_FONT_DB_T*)((uint32)font_info_ex_ptr->pdbtt + (uint32)module_info_ptr);
        ret_ptr = (uint8 *)((uint32)(font_db_ptr->pdb) + (uint32)module_info_ptr);
#endif//end of NANDBOOT_SUPPORT

        *lib_ptr  = ret_ptr;
        *lib_size = font_db_ptr->db_size;
        
        result = TRUE;
    }
 
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:根据当前语言获得OTF语言标签
/*****************************************************************************/
LOCAL uint32 MMIRES_GetFontLangTag( void )
{
    //OTF的标签
#define OTF_TAG(a, b, c, d) ((uint32)(((a)<<24)|((b)<<16)|((c)<<8)|(d)))

    //每种语言对应的OTF语言标签的数组
    LOCAL const uint32 s_lang_tag_arry[MMISET_MAX_LANGUAGE] = 
    {
        0,//MMISET_LANGUAGE_ENGLISH,        
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
        OTF_TAG('Z','H','S',' '),//MMISET_LANGUAGE_SIMP_CHINESE,   
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
        OTF_TAG('Z','H','T',' '),//MMISET_LANGUAGE_TRAD_CHINESE,   
#endif
#ifdef IM_ARABIC_SUPPORT
        OTF_TAG('A','R','A',' '),//MMISET_LANGUAGE_ARABIC, 
#endif
#ifdef IM_FRENCH_SUPPORT
        0,//MMISET_LANGUAGE_FRENCH,
#endif
#ifdef IM_HINDI_SUPPORT
        OTF_TAG('H','I','N',' '),//MMISET_LANGUAGE_HINDI,
#endif
#ifdef IM_HUNGARIAN_SUPPORT
        0,//MMISET_LANGUAGE_HUNGARIAN,
#endif
#ifdef IM_INDONESIAN_SUPPORT
        0,//MMISET_LANGUAGE_INDONESIAN,
#endif
#ifdef IM_MALAY_SUPPORT
        0,//MMISET_LANGUAGE_MALAY,
#endif
#ifdef IM_PORTUGUESE_SUPPORT
        0,//MMISET_LANGUAGE_PORTUGUESE,
#endif
#ifdef IM_RUSSIAN_SUPPORT
        0,//MMISET_LANGUAGE_RUSSIAN,
#endif
#ifdef IM_SPANISH_SUPPORT
        0,//MMISET_LANGUAGE_SPANISH,
#endif
#ifdef IM_TAGALOG_SUPPORT
        0,//MMISET_LANGUAGE_TAGALOG,
#endif
#ifdef IM_THAI_SUPPORT
        OTF_TAG('T','H','A',' '),//MMISET_LANGUAGE_THAI,
#endif
#ifdef IM_VIETNAMESE_SUPPORT
        0,//MMISET_LANGUAGE_VIETNAMESE,
#endif
#ifdef IM_URDU_SUPPORT
        OTF_TAG('U','R','D',' '),//MMISET_LANGUAGE_URDU,
#endif
#ifdef IM_ITALIAN_SUPPORT
        0,//MMISET_LANGUAGE_ITALIAN,
#endif
#ifdef IM_PERSIAN_SUPPORT
        OTF_TAG('F','A','R',' '),//MMISET_LANGUAGE_PERSIAN,
#endif
#ifdef IM_TURKISH_SUPPORT
        0,//MMISET_LANGUAGE_TURKISH,
#endif
#ifdef IM_GERMAN_SUPPORT
        0,//MMISET_LANGUAGE_GERMAN,
#endif
#ifdef IM_GREEK_SUPPORT
        0,//MMISET_LANGUAGE_GREEK,
#endif
#ifdef IM_HEBREW_SUPPORT
        OTF_TAG('I','W','R',' '),//MMISET_LANGUAGE_HEBREW,
#endif
#ifdef IM_BENGALI_SUPPORT
        OTF_TAG('B','E','N',' '),//MMISET_LANGUAGE_BENGALI,
#endif
#ifdef IM_CZECH_SUPPORT
        0,//MMISET_LANGUAGE_CZECH,
#endif
#ifdef IM_SLOVENIAN_SUPPORT
        0,//MMISET_LANGUAGE_SLOVENIAN,
#endif
#ifdef IM_ROMANIAN_SUPPORT
        0,//MMISET_LANGUAGE_ROMANIAN,
#endif
#ifdef IM_TELUGU_SUPPORT
        OTF_TAG('T','E','L',' '),//MMISET_LANGUAGE_TELUGU,
#endif
#ifdef IM_MARATHI_SUPPORT
        OTF_TAG('M','A','R',' '),//MMISET_LANGUAGE_MARATHI,
#endif
#ifdef IM_TAMIL_SUPPORT
        OTF_TAG('T','A','M',' '),//MMISET_LANGUAGE_TAMIL,
#endif
#ifdef IM_GUJARATI_SUPPORT
        OTF_TAG('G','U','J',' '),//MMISET_LANGUAGE_GUJARATI,
#endif
#ifdef IM_KANNADA_SUPPORT
        OTF_TAG('K','A','N',' '),//MMISET_LANGUAGE_KANNADA,
#endif
#ifdef IM_MALAYALAM_SUPPORT
        OTF_TAG('M','A','L',' '),//MMISET_LANGUAGE_MALAYALAM,
#endif
#ifdef IM_ORIYA_SUPPORT
        OTF_TAG('O','R','I',' '),//MMISET_LANGUAGE_ORIYA,
#endif
#ifdef IM_PUNJABI_SUPPORT
        0,//MMISET_LANGUAGE_PUNJABI,
#endif
#ifdef IM_AFRIKAANS_SUPPORT
        0,//MMISET_LANGUAGE_AFRIKAANS,
#endif
#ifdef IM_ALBANIAN_SUPPORT
        0,//MMISET_LANGUAGE_ALBANIAN,
#endif
#ifdef IM_ARMENIAN_SUPPORT
        0,//MMISET_LANGUAGE_ARMENIAN,
#endif
#ifdef IM_AZERBAIJANI_SUPPORT
        0,//MMISET_LANGUAGE_AZERBAIJANI,
#endif
#ifdef IM_BASQUE_SUPPORT
        0,//MMISET_LANGUAGE_BASQUE,
#endif
#ifdef IM_BULGARIAN_SUPPORT
        0,//MMISET_LANGUAGE_BULGARIAN,
#endif
#ifdef IM_CATALAN_SUPPORT
        0,//MMISET_LANGUAGE_CATALAN,
#endif
#ifdef IM_CROATIAN_SUPPORT
        0,//MMISET_LANGUAGE_CROATIAN,
#endif
#ifdef IM_DANISH_SUPPORT
        0,//MMISET_LANGUAGE_DANISH,
#endif
#ifdef IM_DUTCH_SUPPORT
        0,//MMISET_LANGUAGE_DUTCH,
#endif
#ifdef IM_ESTONIAN_SUPPORT
        0,//MMISET_LANGUAGE_ESTONIAN,
#endif
#ifdef IM_FILIPINO_SUPPORT
        0,//MMISET_LANGUAGE_FILIPINO,
#endif
#ifdef IM_FINNISH_SUPPORT
        0,//MMISET_LANGUAGE_FINNISH,
#endif
#ifdef IM_GALICIAN_SUPPORT
        0,//MMISET_LANGUAGE_GALICIAN,
#endif
#ifdef IM_GEORGIAN_SUPPORT
        0,//MMISET_LANGUAGE_GEORGIAN,
#endif
#ifdef IM_HAUSA_SUPPORT
        0,//MMISET_LANGUAGE_HAUSA,
#endif
#ifdef IM_ICELANDIC_SUPPORT
        0,//MMISET_LANGUAGE_ICELANDIC,
#endif
#ifdef IM_IGBO_SUPPORT
        0,//MMISET_LANGUAGE_IGBO,
#endif
#ifdef IM_IRISH_SUPPORT
        0,//MMISET_LANGUAGE_IRISH,
#endif
#ifdef IM_KAZAKH_SUPPORT
        0,//MMISET_LANGUAGE_KAZAKH,
#endif
#ifdef IM_LATVIAN_SUPPORT
        0,//MMISET_LANGUAGE_LATVIAN,
#endif
#ifdef IM_LITHUANIAN_SUPPORT
        0,//MMISET_LANGUAGE_LITHUANIAN,
#endif
#ifdef IM_MACEDONIAN_SUPPORT
        0,//MMISET_LANGUAGE_MACEDONIAN,
#endif
#ifdef IM_MOLDOVAN_SUPPORT
        0,//MMISET_LANGUAGE_MOLDOVAN,
#endif
#ifdef IM_NORWEGIAN_SUPPORT
        0,//MMISET_LANGUAGE_NORWEGIAN,
#endif
#ifdef IM_POLISH_SUPPORT
        0,//MMISET_LANGUAGE_POLISH,
#endif
#ifdef IM_SERBIAN_SUPPORT
        0,//MMISET_LANGUAGE_SERBIAN,
#endif
#ifdef IM_SESOTHO_SUPPORT
        0,//MMISET_LANGUAGE_SESOTHO,
#endif
#ifdef IM_SLOVAK_SUPPORT
        0,//MMISET_LANGUAGE_SLOVAK,
#endif
#ifdef IM_SWEDISH_SUPPORT
        0,//MMISET_LANGUAGE_SWEDISH,
#endif
#ifdef IM_UKRAINIAN_SUPPORT
        0,//MMISET_LANGUAGE_UKRAINIAN,
#endif
#ifdef IM_YORUBA_SUPPORT
        0//MMISET_LANGUAGE_YORUBA,
#endif
    };/*lint !e785*/

    uint32 lang_tag = 0;

    if ( g_ksp_Lang < MMISET_MAX_LANGUAGE )
    {
        lang_tag = s_lang_tag_arry[g_ksp_Lang];
    }

    return lang_tag;
}

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
//  Description : Load Bookmark Info From Resource
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_LoadBookmarkFromRes(MMIRES_BOOKMARK_INFO_T *res_info_ptr)
{
    MMIRES_DATA_INFO_T file_info = {0}; /*lint !e64*/
    BOOLEAN is_res_data_exist = FALSE;
    uint8 bookmark_num = 0;
    uint32 i = 0;
    uint32 bookmark_id = NULL;

#ifdef CMCC_UI_STYLE
    bookmark_id = R_NETWORK_BOOKMARK_CMCC;
#else
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        bookmark_id = MMIAPIPLMN_GetLocalBookmark(i);

        if (NULL != bookmark_id)
        {
            break;
        }
    }

    if (NULL == bookmark_id)
    {
#ifdef ORANGE_SUPPORT
        return FALSE;
#else
        #if defined(MMI_GPRS_SUPPORT) && !defined(CMCC_CUSTMER_SAMPLE)
        bookmark_id = R_NETWORK_BOOKMARK;
        #else
        return FALSE;        
        #endif
#endif
    }
#else
#if defined(MMI_GPRS_SUPPORT) && !defined(CMCC_CUSTMER_SAMPLE)
    bookmark_id = R_NETWORK_BOOKMARK;
#else
    return FALSE;
#endif
#endif
#endif

    is_res_data_exist = MMI_GetDataInfo(bookmark_id, &file_info);

    if (is_res_data_exist && BOOKMARK_HEADER_LENGTH < file_info.data_len)
    {
        bookmark_num = MMICOM_READ_2BYTE(file_info.data_ptr, BOOKMARK_NUM_OFFSET);

        if (0 == bookmark_num)
        {
            is_res_data_exist = FALSE;
        }
        else if (PNULL != res_info_ptr)
        {
            uint32 offset_addr = 0;

            res_info_ptr->res_item_ptr = SCI_ALLOCA(sizeof(MMIRES_BOOKMARK_ITEM_T) * bookmark_num);

            if (PNULL == res_info_ptr->res_item_ptr)
            {
                res_info_ptr->res_num = 0;
            }
            else
            {
                SCI_MEMSET(res_info_ptr->res_item_ptr, 0x00, (sizeof(MMIRES_BOOKMARK_ITEM_T) * bookmark_num));
                res_info_ptr->res_num = bookmark_num;
                res_info_ptr->is_res_data = TRUE;
            
                for (i = 0; i < bookmark_num; i++)
                {
                    uint8 a = 0;
                    uint8 b = 0;
                    uint8 c = 0;
                    uint8 d = 0;
                    uint16 len = 0;
                    uint32 data_offset = 0;
                    const wchar *name_ptr = PNULL;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                    uint16 j = 0;
#endif
                    a = MMICOM_READ_BYTE(file_info.data_ptr, (BOOKMARK_HEADER_LENGTH + i * BOOKMARK_OFFSET_LENGTH));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (BOOKMARK_HEADER_LENGTH + i * BOOKMARK_OFFSET_LENGTH + 1));
                    c = MMICOM_READ_BYTE(file_info.data_ptr, (BOOKMARK_HEADER_LENGTH + i * BOOKMARK_OFFSET_LENGTH + 2));
                    d = MMICOM_READ_BYTE(file_info.data_ptr, (BOOKMARK_HEADER_LENGTH + i * BOOKMARK_OFFSET_LENGTH + 3));
                
                    offset_addr = CONVERT4U8TOU32(a, b, c, d);

                    //get name
                    name_ptr = (const wchar *)(file_info.data_ptr + offset_addr + BOOKMARK_ITEM_NAME_LEN_OFFSET + BOOKMARK_ITEM_NAME_LEN_LENGTH);
                    len = MMIAPICOM_Wstrlen(name_ptr);
                    res_info_ptr->res_item_ptr[i].name_len = MIN(MMIBROWSER_BOOKMARK_MAX_TITLE_LENGTH, len);

                    res_info_ptr->res_item_ptr[i].name = SCI_ALLOC_APPZ((res_info_ptr->res_item_ptr[i].name_len + 1) * sizeof(wchar));

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                    for (j = 0; j < res_info_ptr->res_item_ptr[i].name_len; j++)
                    {
                        res_info_ptr->res_item_ptr[i].name[j] = ((name_ptr[j]<< 8)&0xFF00) + ((name_ptr[j]>>8)&0x00FF);
                    }
#else
                    MMI_WSTRNCPY(res_info_ptr->res_item_ptr[i].name, res_info_ptr->res_item_ptr[i].name_len, name_ptr, res_info_ptr->res_item_ptr[i].name_len, res_info_ptr->res_item_ptr[i].name_len);
#endif

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + BOOKMARK_ITEM_NAME_LEN_OFFSET));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + BOOKMARK_ITEM_NAME_LEN_OFFSET + 1));

                    data_offset = CONVERT2U8TOU16(a, b) + BOOKMARK_ITEM_NAME_LEN_OFFSET + BOOKMARK_ITEM_NAME_LEN_LENGTH;

                    //get url
                    res_info_ptr->res_item_ptr[i].url = (const char *)(file_info.data_ptr + offset_addr + data_offset + BOOKMARK_ITEM_URL_LENGTH);

                    len = strlen(res_info_ptr->res_item_ptr[i].url);
                    res_info_ptr->res_item_ptr[i].url_len = MIN(MMIBROWSER_BOOKMARK_MAX_URL_LENGTH, len);

                    a = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset));
                    b = MMICOM_READ_BYTE(file_info.data_ptr, (offset_addr + data_offset + 1));

                    data_offset += CONVERT2U8TOU16(a, b) + BOOKMARK_ITEM_URL_LENGTH;
                }
            }
        }
    }
    else
    {
        is_res_data_exist = FALSE;
    }

    //SCI_TRACE_LOW:"MMIRES_LoadBookmarkFromRes is_res_data_exist=%d, bookmark_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_RESOURCE_5410_112_2_18_2_0_24_247,(uint8*)"dd", is_res_data_exist, bookmark_num);

    return is_res_data_exist;
}

/*****************************************************************************/
//  Description : Unload Bookmark Info
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRES_UnloadBookmarkInfo(MMIRES_BOOKMARK_INFO_T *res_info_ptr)
{
    if (PNULL != res_info_ptr && PNULL != res_info_ptr->res_item_ptr)
    {
        uint32 i = 0;

        for (i = 0; i < res_info_ptr->res_num; i++)
        {
            if (PNULL != res_info_ptr->res_item_ptr[i].name)
            {
                SCI_FREE(res_info_ptr->res_item_ptr[i].name);
                res_info_ptr->res_item_ptr[i].name = PNULL;
            }
        }
    
        SCI_FREE(res_info_ptr->res_item_ptr);
        res_info_ptr->res_item_ptr = PNULL;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}
#endif
/*lint -e18 -e64 -e516*/
