#ifdef SCRIPTEASY_SUPPORT
/*****************************************************************************
** File Name:       gui_scripteasy.c                                     *
** Author:                                                                   *
** Date:           5/5/2008                                               *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file provide functions about font liabrary and display string               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME             DESCRIPTION                               *
** 5/05/2008       apple.zhang          Create                                  *
******************************************************************************/

#ifndef _GUI_SCIPTEASY_C_
#define _GUI_SCIPTEASY_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "guilcd.h"
#include "mmi_theme.h"
#include "scripteasy.h"
#include "gui_scripteasy.h"

/* File manager related macro */
#define MMIFILE_SetFilePointer					 MMIAPIFMM_SetFilePointer

/*TIANYURD1:liushj 20090929 multi-language test  for PROD00783717 begin*/

#pragma message("gui_scripteasy Here!!!")

//#define FONT_SWITCH_SUPPORT
//#define FONT_STUB_SUPPORT

//#define USE_FZ_XIHEI
//#define USE_FZ_LANTINGHEI
//#define USE_FZ_LIJINGYAXIAN
//#define USE_FZ_BEIWEIKAI
//#define USE_FZ_KATONG

/*TIANYURD:liushj 20091010 add 非矢量字库for  PROD00794533 begin*/
/* font data */
/*非矢量*/
#include "kmx_digit_b29.h"
#include "kmx_digit_b38.h"

#include "kmx_helv_b14.h"
#include "kmx_helv_b16.h"
#include "kmx_helv_b18.h"
#include "kmx_helv_b20.h"
#include "kmx_helv_b24.h"
 
/*矢量字*/

#ifdef FONT_TYPE_SUPPORT_VECTOR
#include "kmx_latin4_ttf.h"
#define LATIN_ID 0
#endif

#if defined(USE_FZ_XIHEI)
    #include "fzxh_gb18030_1_ttf.h"
    #define XIHEI_ID (LATIN_ID + 1)
    #define FZXH_ID XIHEI_ID
#else
    #define XIHEI_ID LATIN_ID
#endif

#if defined(USE_FZ_LANTINGHEI)
    #include "fzlantinghei_gbk_ttf_stub.h"
    #define LANTINGHEI_ID (XIHEI_ID + 1)
#else
    #define LANTINGHEI_ID XIHEI_ID
#endif

#if defined(USE_FZ_LIJINGYAXIAN)
    #include "kmx_stroke_gbk_stf_stub.h"
    #define LIJINGYAXIAN_1_ID (LANTINGHEI_ID + 1)

    #include "fzxh_gb2312_bsy_ttf.h"            // 丽晶雅线所需的标点符号
    #define LIJINGYAXIAN_SYMBOL_ID (LIJINGYAXIAN_1_ID + 1)
    
    #define LIJINGYAXIAN_ID LIJINGYAXIAN_SYMBOL_ID 
#else
    #define LIJINGYAXIAN_ID LANTINGHEI_ID
#endif

#if defined(USE_FZ_BEIWEIKAI)
    #include "fzbwksk_gbk_ttf_stub.h"
    #define BEIWEIKAI_ID (LIJINGYAXIAN_ID + 1)
#else
    #define BEIWEIKAI_ID LIJINGYAXIAN_ID
#endif

#if defined(USE_FZ_KATONG)
    #include "fzkatong_gb2312_ttf_stub.h"
    #define KATONG_1_ID (BEIWEIKAI_ID + 1)
    
    #if defined(FZXH_ID)
        #define KATONG_ID KATONG_1_ID
    #else                                       // 卡通字体无GBK，需要从FZXH中补充
        #include "fzxh_gb18030_1_ttf_stub.h"
        #define FZXH_ID (KATONG_1_ID + 1)

        #define FZXH_ID_BY_KATONG
        #define KATONG_ID FZXH_ID
    #endif
#else
    #define KATONG_ID BEIWEIKAI_ID
#endif

#if 0
#include "fzxh_gb18030_2_ttf_stub.h"            // gb18030 补充的字符
#define EXTENSION_ID (KATONG_ID + 1)

#include "fzlth_gb18030_ttf_stub.h"
#include "fzkatong_gb2312_ttf_stub.h"
#include "kmx_stroke_gbk_stf_stub.h"
#include "fzbwksk_gbk_ttf_stub.h"
#include "fzxh_gb18030_1_ttf_stub.h"
#include "fzxh_gb18030_2_ttf_stub.h"
#endif

/*TIANYURD:liushj 20091010 add 非矢量字库for  PROD00794533 end*/

/*TIANYURD:liushj 20091015 add for unitel begin*/
#if defined(IM_ARABIC_SUPPORT)|| defined(IM_PERSIAN_SUPPORT) || defined(IM_URDU_SUPPORT)
#include "kmx_arabic1_b14.h"
#include "kmx_arabic1_b16.h"
#include "kmx_arabic1_b18.h"
#include "kmx_arabic1_b20.h"
#include "kmx_arabic1_b24.h"
#endif

#if defined(IM_BENGALI_SUPPORT)
#include "kmx_bengali1_b16.h"
#include "kmx_bengali1_b18.h"
#include "kmx_bengali1_b20.h"
#include "kmx_bengali1_b24.h"
#endif

#if defined(IM_HEBREW_SUPPORT)
#include "hebrew_b14.h"
#include "hebrew_b16.h"
#include "hebrew_b18.h"
#include "hebrew_b20.h"
#include "hebrew_b24.h"
#endif

#if defined(IM_KHMER_SUPPORT)
#include "kmx_khmer2_b14.h"
#include "kmx_khmer2_b16.h"
#include "kmx_khmer2_b18.h"
#include "kmx_khmer2_b20.h"
#include "kmx_khmer2_b24.h"
#endif

#if defined(IM_MYANMAR_SUPPORT)
#include "kmx_myanmar_3_b14.h"
#include "kmx_myanmar_3_b16.h"
#include "kmx_myanmar_3_b18.h"
#include "kmx_myanmar_3_b20.h"
#include "kmx_myanmar_3_b24.h"
#endif

#if defined(IM_THAI_SUPPORT)
#include "kmx_thai6_b14.h"
#include "kmx_thai6_b16.h"
#include "kmx_thai6_b18.h"
#include "kmx_thai6_b20.h"
#include "kmx_thai6_b24.h"
#endif

#if defined(IM_HINDI_SUPPORT)
#include "kmx_hindi1_b14.h"
#include "kmx_hindi1_b16.h"
#include "kmx_hindi1_b18.h"
#include "kmx_hindi1_b20.h"
#include "kmx_hindi1_b24.h"
#endif

#if defined(IM_LAO_SUPPORT)
#include "kmx_lao1_b14.h"
#include "kmx_lao1_b16.h"
#include "kmx_lao1_b18.h"
#include "kmx_lao1_b20.h"
#include "kmx_lao1_b24.h"
#endif

#if defined(IM_TELUGU_SUPPORT)
#include "kmx_telugu_2_b16.h"
#include "kmx_telugu_2_b18.h"
#include "kmx_telugu_2_b20.h"
#include "kmx_telugu_2_b24.h"
#endif

//#if defined(IM_PERSIAN_SUPPORT)
//#include "kmx_persian1_ttf.h"
//#endif
/*TIANYURD:liushj 20091015 add for unitel end*/
/*TIANYURD: futao 20091027 add sm Chinese PROD00841425 begin*/
#ifdef IM_SIMP_CHINESE_SUPPORT
#include "kmx_gb2312_b24.h"
#include "kmx_gb2312_b20.h"
#include "kmx_gb2312_b16.h"
#include "kmx_gb2312_b14.h"
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
#include "kmx_big5_b24.h"
#include "kmx_big5_b20.h"
#include "kmx_big5_b16.h"
#include "kmx_big5_b14.h"
#endif

/*TIANYURD: futao 20091027 add sm Chinese PROD00841425 end*/

#endif
/*TIANYURD1:liushj 20090929 multi-language test  for PROD00783717 end*/

/**---------------------------------------------------------------------------*
**                         Macro Declaration                                  *
**---------------------------------------------------------------------------*/
#define GUI_SE_MAX_FONT_LIB_NUM (2 * GUI_MAX_FONT_NUM)

#ifdef FONT_TYPE_SUPPORT_VECTOR
#define SCRIPTEASY_MEMORY_POOL_SIZE    	(2 * 65536)
#define SCRIPTEASY_FONT_POOL_SIZE    	(128 * 1024)
#else 
#define SCRIPTEASY_MEMORY_POOL_SIZE    	(65536)
#define SCRIPTEASY_FONT_POOL_SIZE    	(4)
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/*TIANYURD1:liushj 20090929 multi-language test  for PROD00783717 begin*/
LOCAL uint8* s_font_table_ptr[] = 
{
#ifdef FONT_TYPE_SUPPORT_VECTOR
    (void*)kmx_latin4_ttf, 0,

#if defined(USE_FZ_XIHEI)
    (void*)fzxh_gb18030_1_ttf, 0,//L"fzxh_gb18030_1_ttf",
#endif

#if defined(USE_FZ_LANTINGHEI)
    (void*)fzlantinghei_gbk_ttf, L"fzlantinghei_gbk_ttf",
#endif

#if defined(USE_FZ_LIJINGYAXIAN)
    (void*)kmx_stroke_gbk_stf, L"kmx_stroke_gbk_stf",
    (void*)fzxh_gb2312_bsy_ttf,    0,            // 丽晶雅线所需的标点符号
#endif

#if defined(USE_FZ_BEIWEIKAI)
    (void*)fzbwksk_gbk_ttf, L"fzbwksk_gbk_ttf",
#endif

#if defined(USE_FZ_KATONG)
    (void*)fzkatong_gb2312_ttf, L"fzkatong_gb2312_ttf",
    #ifdef FZXH_ID_BY_KATONG
        (void*)fzxh_gb18030_1_ttf, L"fzxh_gb18030_1_ttf",
    #endif
#endif

    (void*)fzxh_gb18030_2_ttf, L"fzxh_gb18030_2_ttf",
    0, 0

#else /* FONT_TYPE_SUPPORT_VECTOR */

/*TIANYURD:liushj 20091010 add 非矢量字库for  PROD00794533 begin*/
/*非矢量字*/
//	(uint8*)kmx_digit_b29,0,
//	(uint8*)kmx_digit_b38,0,

	(uint8*)kmx_helv_b14,0,
	(uint8*)kmx_helv_b16,0,
	(uint8*)kmx_helv_b18,0,
	(uint8*)kmx_helv_b20,0,
	(uint8*)kmx_helv_b24,0,	

/*TIANYURD:liushj 20091010 add 非矢量字库for  PROD00794533 end*/
/*TIANYURD:liushj 20091015 add  unitel for begin*/
#if defined(IM_ARABIC_SUPPORT)|| defined(IM_PERSIAN_SUPPORT) || defined(IM_URDU_SUPPORT)
	(uint8*)kmx_arabic1_b14,0,
	(uint8*)kmx_arabic1_b16,0,
	(uint8*)kmx_arabic1_b18,0,
	(uint8*)kmx_arabic1_b20,0,
 	(uint8*)kmx_arabic1_b24,0,  
#endif

#if defined(IM_BENGALI_SUPPORT)
    (uint8*)kmx_bengali1_b16, 0,
    (uint8*)kmx_bengali1_b18, 0,
    (uint8*)kmx_bengali1_b20, 0,
    (uint8*)kmx_bengali1_b24, 0,
#endif

#if defined(IM_HEBREW_SUPPORT)
    (uint8*)hebrew_b14, 0,
    (uint8*)hebrew_b16, 0,
    (uint8*)hebrew_b18, 0,
    (uint8*)hebrew_b20, 0,
    (uint8*)hebrew_b24, 0,
#endif

#if defined(IM_KHMER_SUPPORT)
    (uint8*)kmx_khmer2_b14,0,
    (uint8*)kmx_khmer2_b16,0,
    (uint8*)kmx_khmer2_b18,0,
    (uint8*)kmx_khmer2_b20,0,
    (uint8*)kmx_khmer2_b24,0,    
#endif

#if defined(IM_MYANMAR_SUPPORT)
    (uint8*)kmx_myanmar_3_b14,0,
    (uint8*)kmx_myanmar_3_b16,0,
    (uint8*)kmx_myanmar_3_b18,0,
    (uint8*)kmx_myanmar_3_b20,0,
    (uint8*)kmx_myanmar_3_b24,0,    
#endif

#if defined(IM_THAI_SUPPORT)
    (uint8*)kmx_thai6_b14,0,
    (uint8*)kmx_thai6_b16,0,
    (uint8*)kmx_thai6_b18,0,
    (uint8*)kmx_thai6_b20,0,
    (uint8*)kmx_thai6_b24,0,    
#endif

#if defined(IM_HINDI_SUPPORT)
    (uint8*)kmx_hindi1_b14,0,     
    (uint8*)kmx_hindi1_b16,0,     
    (uint8*)kmx_hindi1_b18,0, 
    (uint8*)kmx_hindi1_b20,0,    
    (uint8*)kmx_hindi1_b24,0,    
#endif

#if defined(IM_LAO_SUPPORT)
    (uint8*)kmx_lao1_b14,0,     
    (uint8*)kmx_lao1_b16,0,     
    (uint8*)kmx_lao1_b18,0, 
    (uint8*)kmx_lao1_b20,0,    
    (uint8*)kmx_lao1_b24,0,    
#endif

#if defined(IM_TELUGU_SUPPORT)
    (uint8*)kmx_telugu_2_b16,0,     
    (uint8*)kmx_telugu_2_b18,0, 
    (uint8*)kmx_telugu_2_b20,0,    
    (uint8*)kmx_telugu_2_b24,0,    
#endif
/*TIANYURD:futao 20091027 add sm Chinese PROD00841425 begin*/
#ifdef IM_SIMP_CHINESE_SUPPORT
    #if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
    (uint8*)kmx_gb2312_b16,0,
    #elif defined MAINLCD_SIZE_128X64
    #else
    (uint8*)kmx_gb2312_b20,0,
    #endif
    (uint8*)kmx_gb2312_b14,0,
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
    #if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
    (uint8*)kmx_big5_b16,0,    
    #elif defined MAINLCD_SIZE_128X64
    #else
    (uint8*)kmx_big5_b20,0,
    #endif
    (uint8*)kmx_big5_b14,0,
#endif

/*TIANYURD:futao  20091027 add sm Chinese PROD00841425 end*/
//#if defined(IM_PERSIAN_SUPPORT)
    //kmx_persian1_ttf,
//#endif
/*TIANYURD:liushj 20091015 add for unitel  for  end*/
    0, 0

#endif /* FONT_TYPE_SUPPORT_VECTOR */

};

#ifdef FONT_SWITCH_SUPPORT

#define MAX_LOGICAL_FONT_SIZE 60

#define LOGICAL_ID -1

static unsigned char logical_font_table_config[MAX_LOGICAL_FONT_SIZE] =
{
#if defined(USE_FZ_XIHEI)
    3, LATIN_ID, XIHEI_ID, EXTENSION_ID,
    #define LOGICAL_XIHEI_ID (LOGICAL_ID + 1)
#else
    #define LOGICAL_XIHEI_ID LOGICAL_ID
#endif

#if defined(USE_FZ_LANTINGHEI)
    3, LATIN_ID, LANTINGHEI_ID, EXTENSION_ID,
    #define LOGICAL_LANTINGHEI_ID (LOGICAL_XIHEI_ID + 1)
#else
    #define LOGICAL_LANTINGHEI_ID LOGICAL_XIHEI_ID
#endif

#if defined(USE_FZ_LIJINGYAXIAN)
    4, LATIN_ID, LIJINGYAXIAN_1_ID, LIJINGYAXIAN_SYMBOL_ID, EXTENSION_ID,
    #define LOGICAL_LIJINGYAXIAN_ID (LOGICAL_LANTINGHEI_ID + 1)
#else
    #define LOGICAL_LIJINGYAXIAN_ID LOGICAL_LANTINGHEI_ID
#endif

#if defined(USE_FZ_BEIWEIKAI)
    3, LATIN_ID, BEIWEIKAI_ID, EXTENSION_ID,
    #define LOGICAL_BEIWEIKAI_ID (LOGICAL_LIJINGYAXIAN_ID + 1)
#else
    #define LOGICAL_BEIWEIKAI_ID LOGICAL_LIJINGYAXIAN_ID
#endif

#if defined(USE_FZ_KATONG)
    4, LATIN_ID, KATONG_1_ID, FZXH_ID, EXTENSION_ID,
    #define LOGICAL_KATONG_ID (LOGICAL_BEIWEIKAI_ID + 1)
#else
    #define LOGICAL_KATONG_ID LOGICAL_BEIWEIKAI_ID
#endif

    0
};
#endif /* FONT_SWITCH_SUPPORT */

/*TIANYURD1:liushj 20090929 multi-language test  for PROD00783717 end*/
LOCAL uint32 scripteasy_memory_pool[SCRIPTEASY_MEMORY_POOL_SIZE/sizeof(uint32)] = {0};

LOCAL uint32 font_cache_pool[SCRIPTEASY_FONT_POOL_SIZE/sizeof(uint32)] = {0};/*矢量字*/

LOCAL SeDeviceContext * s_device_context = PNULL;
LOCAL SeEditorInit* s_editor_init = PNULL;
LOCAL SeEditor**    s_editor = PNULL;
LOCAL SeEditorPalette* s_editor_palette = PNULL;
LOCAL SeScrollInfo    s_EditboxScrollInfo = {0};

LOCAL SCI_MUTEX_PTR s_scripteasy_mutex_p = PNULL;

LOCAL uint8 s_ebook_ascii_width[128] = {0};
LOCAL uint8 s_ebook_hanzi_width = 0;

#ifdef FONT_STUB_SUPPORT
static unsigned int FontAccessCallback(void* name, void* buffer, unsigned int offset, unsigned int size);
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 判断字库是否存在
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
//extern PUBLIC BOOLEAN MMI_CheckFontLibIsExist(
//                        GUI_FONT_T font,
//                        BOOLEAN     is_ucs2
//                        );

/*****************************************************************************/
//  Description : 获得指定字体的数据指针
//  Global resource dependence : none
//  Author: Great.Tian
//  Note:   
/*****************************************************************************/
//extern PUBLIC uint8* MMI_GetFontDataPtr(
//                        GUI_FONT_T font,
//                        BOOLEAN     is_ucs2     // 1: a gb char  0:an ascii char
//                         );
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                                                             */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 改变矩形区域的大小
//  Global resource dependence : 
//   Author: apple.zhang
//  Note: 如果dx和dy都为0，矩形区域的大小保持不变
/*****************************************************************************/
PUBLIC void GUI_Inflate_Rect(
                             SeRectangle * box, //[输入/输出] 指向一个GUI_RECT_T数据结构的指针，存放矩形区域的坐标。不能为空指针
                             int dx, //[输入] 调整矩形区域左边和右边的数值。
                                     //   大于0的数值将增加矩形的宽度，矩形左边的坐标减小，矩形右边的坐标增大。
                                     //   小于0的数值将减小矩形的宽度，矩形左边的坐标增大，矩形右边的坐标减小。
                             int dy //[输入] 调整矩形区域上边和下边的数值。
                                    //大于0的数值将增加矩形的高度，矩形上边的坐标减小，矩形下边的坐标增大。
                                    //小于0的数值将减小矩形的高度，矩形上边的坐标增大，矩形下边的坐标减小。
                              )
{   
    se_inflate_rect(box,dx,dy);
}

static int GUI_Str_Length(const uint16 *text)
{
    int len = 0;
    if (text == PNULL)
        return len;

    while (*text++)
    {
        len++;
    }

    return len;
}

LOCAL void scripteasy_enter_mutex(void)
{
    if (s_scripteasy_mutex_p)
    {
        SCI_GetMutex(s_scripteasy_mutex_p, SCI_WAIT_FOREVER);
    }
}

LOCAL void scripteasy_leave_mutex(void)
{
    if (s_scripteasy_mutex_p)
    {
        SCI_PutMutex(s_scripteasy_mutex_p);
    }
}

/*****************************************************************************/
//  Description : Init SE Library
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Initialize_Scripteasy_Library(int language)
{
    int status = SE_STATUS_OK;
    SeInitInfo init = {0};
   
    SCI_ASSERT(language >= 0);/*assert verified*/

    SCI_MEMSET(&init, 0x00, sizeof(init));

    init.language = language;
    init.font_table = s_font_table_ptr;
#ifdef FONT_SWITCH_SUPPORT
    init.logical_font_table = logical_font_table_config;
#endif
    init.mem_pool = scripteasy_memory_pool;
    init.mem_pool_size = SCRIPTEASY_MEMORY_POOL_SIZE;
/*矢量字*/
   init.cache_mem_pool = font_cache_pool;
   init.cache_mem_pool_size = SCRIPTEASY_FONT_POOL_SIZE;
#ifdef FONT_STUB_SUPPORT
    init.font_access_callback = FontAccessCallback;
#endif
    /* Language data is needed for Thai only */
#if defined(MMI_LANGUAGE_THAI)
        init.language_data = &se_thai_language_data;
#endif

    status = se_initialize(&init);
    if (status != SE_STATUS_OK)
    {
        /* Can not proceed, crash here for debugging */
        SCI_PASSERT(FALSE,("GUI_Initialize_Scripteasy_Library: status %d is error!",status));/*assert verified*/
    }

    s_scripteasy_mutex_p = SCI_CreateMutex("scripteasy_mutex", SCI_INHERIT);
}

static int logic_font_id = 0;
PUBLIC void GUI_SetLogicFont(int id)
{
    logic_font_id = id;
    se_set_cache(0, 0);
    se_set_cache(font_cache_pool, SCRIPTEASY_FONT_POOL_SIZE);
}

PUBLIC int  GUI_GetLogicFont(void)
{
    return logic_font_id;
}

/*****************************************************************************/
//  Description : 计算指定的文本显示的宽度、高度和行数
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Text_Info_Ex(
                              const SeDeviceContext* dc, 
                              const uint16 * text, //[输入] 指向一个字符串缓冲区的指针，存放了需要处理的文本。不能为空指针。
                              int  length, //[输入] text指向的字符串的长度。如果数值小于0，表示该字符串是一个以0结尾的字符串。字符串长度的最大值不能超过SE_MAX_TEXT_LENGTH。
                              int dir, // 1 for rtl, 0 for ltr
                              int * width, //[输出] 指向一个整数的指针，用于返回文本显示的宽度。可以是空指针，表示不需要返回该信息。
                              int * height, //[输出] 指向一个整数的指针，用于返回文本显示的高度。可以是空指针，表示不需要返回该信息。
                              int * lines) //[输出] 指向一个整数的指针，用于返回文本显示的行数。可以是空指针，表示不需要返回该信息。
{
    sestatus status;

    int temp_length = GUI_Str_Length(text);
    if (length > temp_length)
        length = temp_length;

    scripteasy_enter_mutex();
    /* lvjun@komoxo.com at 20100426 to speed up text op begin */
    if (length == 1  && !dc->text_flags&&!dc->font.attr)
    {
        SeRectangle rect;
        status = se_get_char_extent(dc, text[0], &rect, 0);
        if (SE_STATUS_OK == status)
        {
            if (lines) 
                *lines = 1;
            if (height)
                *height = dc->font.size;
            if (width)
                *width = rect.right - rect.left;
        }
    }
    else
    {
        status = se_get_text_extent_with_dir(dc, text, length, dir, width, height, lines);
    }
    /* lvjun@komoxo.com at 20100426 to speed up text op end */
    scripteasy_leave_mutex();

    return status;
}


/*****************************************************************************/
//  Description : 计算指定的文本显示的宽度、高度和行数
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Text_Info(
                              const SeDeviceContext* dc, 
                              const uint16 * text, //[输入] 指向一个字符串缓冲区的指针，存放了需要处理的文本。不能为空指针。
                              int  length, //[输入] text指向的字符串的长度。如果数值小于0，表示该字符串是一个以0结尾的字符串。字符串长度的最大值不能超过SE_MAX_TEXT_LENGTH。
                              int * width, //[输出] 指向一个整数的指针，用于返回文本显示的宽度。可以是空指针，表示不需要返回该信息。
                              int * height, //[输出] 指向一个整数的指针，用于返回文本显示的高度。可以是空指针，表示不需要返回该信息。
                              int * lines) //[输出] 指向一个整数的指针，用于返回文本显示的行数。可以是空指针，表示不需要返回该信息。
{
    sestatus status;

    int temp_length = GUI_Str_Length(text);
    if (length > temp_length)
        length = temp_length;

    scripteasy_enter_mutex();
    /* lvjun@komoxo.com at 20100426 to speed up text op begin */
    if (length == 1  && !dc->text_flags&&!dc->font.attr)
    {
        SeRectangle rect;
        status = se_get_char_extent(dc, text[0], &rect, 0);
        if (SE_STATUS_OK == status)
        {
            if (lines) 
                *lines = 1;
            if (height)
                *height = dc->font.size;
            if (width)
                *width = rect.right - rect.left;
        }
    }
    else
    {
        status = se_get_text_extent(dc, text, length, width, height, lines);
    }
    /* lvjun@komoxo.com at 20100426 to speed up text op end */
    scripteasy_leave_mutex();

    return status;
}

PUBLIC uint16 GUI_CalculateTextLines(
    uint16          pixelnum,
    const wchar*    str_ptr,
    uint16     		str_len, 
    GUI_FONT_T 		font,
    uint8      		char_space,
    BOOLEAN         is_byword)
{
    SeDeviceContext* dc;
    int num_lines;

    dc = GUI_Get_Default_Device_Context(NULL);
    GUI_Set_Device_Context_Font(dc, font);
    dc->text_flags = SE_FLAG_MULTILINE;
    dc->wrap_width = pixelnum;
    GUI_Set_DeviceContent(dc);

    num_lines = 0;
    GUI_Get_Text_Info(dc, str_ptr, str_len, 0, 0, &num_lines);

    (void) is_byword;
    (void) char_space;

    return (uint16) num_lines;
}

/*****************************************************************************/
//  Description : 得到与显示设备和文本显示相关的信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_DeviceContent(void)
{
    return s_device_context;
}
/*****************************************************************************/
//  Description : 放了与显示设备和文本显示相关的信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_DeviceContent(void* dc)
{
    s_device_context = dc;
}
/*****************************************************************************/
//  Description : 在指定位置显示一段文本
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Draw_text(                          
                          const SeDeviceContext* dc, 
                          int  x,//[输入] 文本显示的坐标
                          int y,//[输入] 文本显示的坐标
                          const uint16* text, //[输入] 指向一个字符串缓冲区的指针，存放了需要显示的文本。不能为空指针
                          int   length) //[输入] text指向的字符串的长度。如果数值小于0，表示该字符串是一个以0结尾的字符串。字符串长度的最大值不能超过SE_MAX_TEXT_LENGTH。
{
    sestatus status;

    scripteasy_enter_mutex();
    status = se_draw_text(dc, x, y, text, length);
    scripteasy_leave_mutex();

    return status;
}

/*****************************************************************************/
//  Description : 在指定位置显示一段文本
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Draw_text_Ex(                          
                          const SeDeviceContext* dc, 
                          int  x,//[输入] 文本显示的坐标
                          int y,//[输入] 文本显示的坐标
                          const uint16* text, //[输入] 指向一个字符串缓冲区的指针，存放了需要显示的文本。不能为空指针
                          int   length, //[输入] text指向的字符串的长度。如果数值小于0，表示该字符串是一个以0结尾的字符串。字符串长度的最大值不能超过SE_MAX_TEXT_LENGTH。
                          int dir // 1 for rtl, 0 for ltr
                          )
{
    sestatus status;

    scripteasy_enter_mutex();
    status = se_draw_text_with_dir(dc, x, y, text, length, dir);
    scripteasy_leave_mutex();

    return status;
}


/*****************************************************************************/
//  Description : 创建并返回一个文本编辑器对象
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Create(void)
{
/*TIANYURD: cstar 2009-11-25 modify begin for cstar patch */
    /* chenbo@komoxo.com 2009.11.24 start */
        int status = SE_STATUS_OK;
        SeEditorInit* init;
        SeEditor* editor;
    
        init = (SeEditorInit*) GUI_Get_Editor_Init_Info();
    
        /* Set a psuedo value for background color. */
        init->palette.bk_color = 0xFFFFFFFF;
    /* chenbo@komoxo.com 2009.11.24 end */

    editor = (SeEditor*) GUI_Get_Editor();

/*TIANYURD:CSTAR 2009-11-14 modify begin for komoxo patch */
/* chenbo@komoxo.com 2009.11.12 start
 *
 * Add code to detect bug for the future: before editor object is created, editor object should be NULL.
 */

    SCI_ASSERT(editor == 0);/*assert verified*/

/* chenbo@komoxo.com 2009.11.12 end */
/*TIANYURD: 展讯 2009-11-14 modify end for Komoxo patch */


    scripteasy_enter_mutex();
    status = se_editor_create(init,&editor);
    if (SE_STATUS_OK == status)
        se_editor_lock(editor);
    scripteasy_leave_mutex();

    GUI_Set_Editor(editor);
    //作容错处理
    //SCI_ASSERT(status == SE_STATUS_OK);
    if(status != SE_STATUS_OK)
    {
        SCI_TRACE_LOW("GUI_Editor_Create: status = %d", status);
    }
    return status;
}
/*****************************************************************************/
//  Description : 得到初始化文本编辑器所需要的相关信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_Editor_Init_Info(void)
{
    return s_editor_init;
}
/*****************************************************************************/
//  Description : 设置 初始化文本编辑器所需要的相关信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor_Init_Info(void* init)
{
    s_editor_init = init;
}
/*****************************************************************************/
//  Description :  得到新创建的文本编辑器对象的指针
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_Editor(void)
{
    return s_editor;
}
/*****************************************************************************/
//  Description : 设置 新创建的文本编辑器对象的指针
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor(void* editor)
{
    s_editor = editor;
}

/*****************************************************************************/
//  Description : 删除一个文本编辑器对象，并释放相关资源
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_destroy( void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_destroy(editor);
    scripteasy_leave_mutex();

    SCI_ASSERT(status == SE_STATUS_OK);/*assert verified*/

/*TIANYURD:CSTAR 2009-11-14 modify begin for komoxo patch */
/* chenbo@komoxo.com 2009.11.12 start
 *
 * Set editor object to NULL after it is destroyed.
 */

    GUI_Set_Editor(0);

/* chenbo@komoxo.com 2009.11.12 end */
/*TIANYURD:CSTAR 2009-11-14 modify end for komoxo patch */



    return status;
}
 
/*****************************************************************************/
//  Description : 获得一个文本编辑器能容纳的文本内容的最大长度。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Max_Buffer( 
                              int * max_buffer) //[输出] 指向一个整数的指针，用于返回编辑器能容纳的文本内容的最大长度。不能为空指针。
{
    const SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    se_editor_get_max_buffer(editor, max_buffer);
    scripteasy_leave_mutex();

    return 1;
}
/*****************************************************************************/
//  Description :获得一个文本编辑器的文本显示边距。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Margin(SeRectangle*     margin) //[输出] 指向一个GUI_RECT_T数据结构的指针，用于返回编辑器的文本显示边距。不能为空指针。
{
    sestatus status;
    const SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_get_margin(editor, margin);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :获得一个文本编辑器对象的当前位置。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Pos(SeRectangle*     pos) //[输出] 指向一个GUI_RECT_T数据结构的指针，用于返回编辑器的坐标。不能为空指针
{
    sestatus status;
    const SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_get_pos(editor, pos);
    scripteasy_leave_mutex();

    return status;
}

/*TIANYURD:CSTAR 2009-11-14 modify begin for komoxo patch */
/* chenbo@komoxo.com 2009.11.12 start
 * Add function to adjust editor position on the screen.
 */

/*****************************************************************************/
//  Description : 设置文本编辑器对象的新位置。
/*****************************************************************************/
PUBLIC int GUI_Set_Editor_Pos(const SeRectangle* pos)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_set_pos(editor, (const SeRectangle*)pos);
    scripteasy_leave_mutex();

    return status;
}

/* chenbo@komoxo.com 2009.11.12 end */

/*TIANYURD:CSTAR 2009-11-14 modify end for komoxo patch */

/* chenbo@komoxo.com 2009.11.26 start
 *
 * Fix: add new ScriptEasy functions to handle caret position for text inserting.
 */

PUBLIC int GUI_Editor_Get_Caret_Text_Index(void)
{
    sestatus status;
    SeEditor* editor;
    int text_index;

    editor = (SeEditor*) GUI_Get_Editor();
    SCI_ASSERT(editor != 0);/*assert verified*/

    text_index = 0;

    scripteasy_enter_mutex();
    status = se_editor_get_caret_text_index(editor, &text_index);
    scripteasy_leave_mutex();

    SCI_ASSERT(status == SE_STATUS_OK);/*assert verified*/

    return text_index;
}

PUBLIC void GUI_Editor_Set_Caret_Text_Index(int text_index)
{
    sestatus status;
    SeEditor* editor;

    editor = (SeEditor*) GUI_Get_Editor();
    SCI_ASSERT(editor != 0);/*assert verified*/

    scripteasy_enter_mutex();
    status = se_editor_set_caret_text_index(editor, text_index);
    scripteasy_leave_mutex();

    SCI_ASSERT(status == SE_STATUS_OK);/*assert verified*/
}

PUBLIC void GUI_Editor_Delete_Text_Before_Caret(void)
{
    sestatus status;
    SeEditor* editor;
    int caret_text_index;

    editor = (SeEditor*) GUI_Get_Editor();
    SCI_ASSERT(editor != 0);/*assert verified*/

    caret_text_index = 0;

    scripteasy_enter_mutex();

    status = se_editor_get_caret_text_index(editor, &caret_text_index);
    if (status == SE_STATUS_OK && caret_text_index > 0)
    {
        se_editor_lock(editor);
        se_editor_set_selection(editor, 0, caret_text_index);
        se_editor_backspace(editor);
        se_editor_unlock(editor);
    }

    scripteasy_leave_mutex();
}

/* chenbo@komoxo.com 2009.11.26 end */

/*****************************************************************************/
//  Description :获得一个文本编辑器的显示颜色。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Palette(void) 
{
    sestatus status;
    const SeEditor* editor;
    SeEditorPalette* palette;

    editor = GUI_Get_Editor();
    palette = GUI_Get_Editor_Palette_Info();

    scripteasy_enter_mutex();
    status = se_editor_get_palette(editor, palette);
    scripteasy_leave_mutex();

    return status;
    
}

/*****************************************************************************/
//  Description :  得到描述文本编辑器的显示颜色 
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_Editor_Palette_Info(void)
{
    return s_editor_palette;
}
/*****************************************************************************/
//  Description : 设置描述文本编辑器的显示颜色
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor_Palette_Info(void* palette)
{
    s_editor_palette = palette;
}
/*****************************************************************************/
//  Description :用于显示一个文本编辑器。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Paint_Editor(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_paint(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :锁定并禁止一个文本编辑器更新显示。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Lock(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_lock(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :解除对一个文本编辑器显示更新的锁定。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_UNLock(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_unlock(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :获取一个文本编辑器当前文本的内容和长度。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Text(
                                const uint16** text, //[输出] 指向一个字符串缓冲区指针的指针，用于返回一个指向编辑器内部文本字符串缓冲区的指针，该缓冲区内存放了编辑器当前的文本内容。可以是空指针，表示不需要返回该信息。
                                int*           length, //[输出] 指向一个整数的指针，用于返回编辑器当前文本内容的长度。可以是空指针，表示不需要返回该信息。
                                int*           num_chars) //[输出] 指向一个整数的指针，用于返回编辑器当前文本内容所包含的字符的个数。可以是空指针，表示不需要返回该信息。
{
    sestatus status;
    const SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_get_text(editor, text, length, num_chars);
    scripteasy_leave_mutex();

    return status;
}

/*****************************************************************************/
//  Description :清空一个文本编辑器的文本。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Clear_Content(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_clear_content(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :在一个文本编辑器的当前光标位置进行一次退格删除操作。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_BackSpace(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_backspace(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :在一个文本编辑器的当前光标位置插入指定的文本。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Insert_Text(
                                const uint16* text,
                                int         length)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_insert_text(editor, text, length);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :切换一个文本编辑器内光标的显示状态。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Switch_Caret(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    se_editor_unlock(editor);
    status = se_editor_switch_caret(editor);
    se_editor_lock(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向左移动一次。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Left(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_move_caret_left(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向右移动一次。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Right(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_move_caret_right(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向上移动一次。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Up(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_move_caret_up(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向下移动一次。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Down(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_move_caret_down(editor);
    scripteasy_leave_mutex();

    return status;
}
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向上移动一页。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_PageUp(void)
{
    sestatus status = SE_STATUS_OK;
    SeEditor *editor;
    SeScrollInfo info = {0};

    int first_visible_line;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_get_scroll_info(editor, &info);

    first_visible_line = info.first_visible_line / info.max_line_height - info.num_visible_lines;
    if (first_visible_line < 0)
        first_visible_line = 0;

    status = se_editor_set_first_visible_line(editor, first_visible_line);

    scripteasy_leave_mutex();

    return status;
}

/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向下移动一页。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_PageDown(void)
{
    sestatus status = SE_STATUS_OK;
    SeEditor *editor;
    SeScrollInfo info = {0};

    int first_visible_line;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_get_scroll_info(editor, &info);

    first_visible_line = info.first_visible_line / info.max_line_height + info.num_visible_lines;
    if (first_visible_line > info.num_total_lines - info.num_visible_lines)
        first_visible_line = info.num_total_lines - info.num_visible_lines;

    status = se_editor_set_first_visible_line(editor, first_visible_line);

    scripteasy_leave_mutex();

    return status;
}

/*****************************************************************************/
//  Description :删除一个文本编辑器内光标当前位置的字符。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Delete_Editor_Caret_Char(void)
{
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    se_editor_delete_caret_char(editor);
    scripteasy_leave_mutex();
}
/*****************************************************************************/
//  Description :清除一个文本编辑器内输入法当前的编辑内容。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Clear_Editor_IME(void)
{
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    se_editor_clear_ime_comp(editor);
    scripteasy_leave_mutex();
}
/*****************************************************************************/
//  Description :结束一个文本编辑器内输入法当前的编辑操作。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_End_Editor_IME(void)
{
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    se_editor_end_ime_comp(editor);
    scripteasy_leave_mutex();
}
/*****************************************************************************/
//  Description :在一个文本编辑器的当前光标位置插入指定的文本，作为输入法的编辑内容。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor_IME(                           
                            const uint16* text,//[输入] 指定要插入的文本内容和长度。text不能为空指针。如果length小于0，表示text指向是一个以0结尾的字符串。
                            int length)
{
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    se_editor_set_ime_comp(editor, text, length);
    scripteasy_leave_mutex();
}
/*****************************************************************************/
//  Description :返回一个文本编辑器当前输入法的编辑内容。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Get_Editor_IME(                          
                            const uint16** text,//[输出] 指向一个字符串缓冲区指针的指针，用于返回一个指向编辑器内部文本字符串缓冲区的指针，该缓冲区内存放了编辑器当前输入法的编辑内容。可以是空指针，表示不需要返回该信息。
                            int* length) //[输出] 指向一个整数的指针，用于返回编辑器当前输入法的编辑内容的长度。可以是空指针，表示不需要返回该信息。
{
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    se_editor_get_ime_comp(editor, text, length);
    scripteasy_leave_mutex();
}
/*****************************************************************************/
//  Description :返回一个指定了red,green,blue颜色分量的颜色值。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 GUI_Color_RGB(   
                            uint8 red,
                            uint8 green,
                            uint8 blue)
{
    return se_color_rgb(red, green, blue);
}
/*****************************************************************************/
//  Description :获得一个编辑器的滚行信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Set_Editor_Scroll_Info(void)
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_get_scroll_info(editor, &s_EditboxScrollInfo);
    scripteasy_leave_mutex();

    return status;
}

/*****************************************************************************/
//  Description :返回一个编辑器的滚行信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC SeScrollInfo GUI_Get_Editor_Scroll_Info(void)
{
    return s_EditboxScrollInfo;
}

/*****************************************************************************/
//  Description :将一个文本编辑器内的光标移动到给定坐标最近的位置
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Locate_Caret(
                                   int x,
                                   int y
                                    )
{
    sestatus status;
    SeEditor* editor;

    editor = GUI_Get_Editor();

    scripteasy_enter_mutex();
    status = se_editor_locate_caret(editor, x, y);
    scripteasy_leave_mutex();

    return status;
}

/*****************************************************************************/
//            Description : Get char width for ebook
//            Global resource dependence : none
//  Author: ke wen
//            Note:    
/*****************************************************************************/
PUBLIC uint8 GUI_GetCharWidth_EBook(uint16 unicode,GUI_FONT_T font_type)
{
    SeDeviceContext* dc = PNULL;
    int width = 0;
    
    if (s_ebook_hanzi_width == 0)
    {
        uint16 c = 1;
        
        dc = GUI_Get_Default_Device_Context(NULL);
        GUI_Set_Device_Context_Font(dc, font_type);

        scripteasy_enter_mutex();

        while (c < 0x80)
        {
            se_get_text_extent(dc, &c, 1, &width, PNULL, PNULL);
            s_ebook_ascii_width[c] = (uint8)width;
            c++;
        }
 
        c = 0x4e00;
        se_get_text_extent(dc, &c, 1, &width, PNULL, PNULL);
        s_ebook_hanzi_width = (uint8) width;

        scripteasy_leave_mutex();
    }
 
    if (unicode < 0x80)
        return s_ebook_ascii_width[unicode];
    else
        return s_ebook_hanzi_width;
}

#if 0
/*******************************************************************************************/
// 	Description : word bread char
//	Global resource dependence :
//  Author: James Zhang
//	Note: 按词换行的字符
/*********************************************************************************************/
LOCAL BOOLEAN IsWordBreakChar(
                               wchar ch 
                               )
{
	switch (ch) 
    {
    case '\r':		/* carriage return */
    case '\n':		/* new line */
    case ' ':		/* space */
    case '\t':		/* horizontal tab */
//     case '!':		/* exclamatory mark */
//     case '\"':		/* double quote mark */
//     case '&':		/* ampersand */
//     case '\'':		/* single quote mark */
//     case ')':		/* close parentheses */
//     case ',':		/* comma */
//     case '-':		/* subtraction, or hyphen */
//     case '.':		/* period, full stop */
//     case ':':		/* colon */
//     case ';':		/* semicolon */
//     case '>':		/* right crocodile */
//     case '?':		/* question mark */
//     case ']':		/* suit of armor */
//     case '}':		/* open curly brackets */
    case 0x060c:	/* Arabic comma */
    case 0x061b:	/* Arabic semicolon */
    case 0x061f:	/* Arabic question mark */
	case 0x2019:	/* right single quote */
	case 0x201D:	/* right double quote */
		return TRUE;
	}

	return (BOOLEAN)
		(  (ch >= 0x2e80 && ch <= 0x9fff)	/* CJK ideographs */
		|| (ch >= 0xf900 && ch <= 0xfaff)	/* CJK compatibility ideographs */
		|| (ch >= 0x1100 && ch <= 0x11ff)	/* Hangul Jamo */
		|| (ch >= 0xac00 && ch <= 0xd7A3)	/* Hangul Syllables and Yi */
		|| (ch >= 0xff00 && ch <= 0xffef));	/* full-width & half-width CJK */
}

PUBLIC BOOLEAN GUI_AnalysisNextLine(
                                const wchar*  str_ptr,
                                uint16 str_len,
                                GUI_FONT_T font_type,
                                uint16 width,
                                uint16* char_num,
                                uint16* pixel_num
                                )
{
    uint16  w_char = 0;
    uint16  i = 0;
    uint8   step = 0;
    uint8   char_width = 0;
    uint8   han_char_width = 0; //汉字宽度
    uint16  pre_char_num  = 0;
    uint16  pre_pixel_num = 0;
    BOOLEAN is_exist_word_bread_ch = FALSE;

    SCI_ASSERT(PNULL != str_ptr);/*assert verified*/

    step = 1;

    *char_num = 0;
    *pixel_num = 0;

    do {
        w_char = str_ptr[i];
    
        if( 0 == w_char )//james, \0返回
        {
            break;
        }
        if (CR_CHAR == w_char)  //回车
        {
            //CR28985, @Lin.Lin, 2005-8-8, 当字符串中包含0D0A时,计算行数不正确, start
            uint16 next_char = 0;
        
            if(i+1 < str_len)
            {
                next_char = str_ptr[i+1];
            }
        
            if(LF_CHAR == next_char)
            {
                *char_num += 2*step;
            }
            else
                //CR28985, @Lin.Lin, 2005-8-8, 当字符串中包含0D0A时,计算行数不正确, end
            {
                *char_num += step;
            }
            break;
        }
        else if(LF_CHAR == w_char)  //换行
        {
            //if (0 == *char_num)
            {
                *char_num += step;
            }
            break;
        }
        else
        {
            char_width = GUI_GetStringWidthEx(font_type, &w_char, 1, 0);

            if ((*pixel_num + char_width) > width)
            {
                //存在按词换行字符
                if( is_exist_word_bread_ch )
                {
                    *pixel_num = pre_pixel_num;
                    *char_num  = pre_char_num;
                }
                break;
            }
            else    
            {
                if (i+1 == str_len)
                {
                    *pixel_num += char_width;
                }
                else
                {
                    *pixel_num += char_width;
                }
                *char_num = (uint16)(i + step);
            
                //按词换行
                if( IsWordBreakChar( w_char ) )
                {
                    pre_pixel_num = *pixel_num;
                    pre_char_num  = *char_num;
                
                    is_exist_word_bread_ch = TRUE;
                }
            }
        }
    
        i += step;        
    } while(i < str_len);

    //可以显示的下最后一个字符,但是无法显示的下最后一个字间距
    if (*pixel_num > width)
    {
        *pixel_num = width;
    }
}

#else
/******************************************************************************/
/*
 *  \param[in]  text
 *  \param[in]  length
 *  \param[in]  font_type            font type
 *  \param[in]  width
 *  \param[out]  char_num 
 *  \param[out]  pixel_num
 */ 
/******************************************************************************/    
PUBLIC BOOLEAN GUI_AnalysisNextLine(
                                const wchar*  text,
                                uint16 length,
                                GUISTR_STATE_T state,
                                GUI_FONT_T font_type,
                                uint16 width,
                                uint16* char_num,
                                uint16* pixel_num
                                )
{/*lint !e18 !e516*/
    int line_end_offset;
    int line_width;
    SeDeviceContext* dc;
    
    dc = GUI_Get_Default_Device_Context(NULL);
    dc->device_width = width;
    GUI_Set_Device_Context_Font(dc, font_type);
    
    if (GUISTR_STATE_UNDERLINE & state)
    {
        dc->font.attr |= SE_LOGFONT_UNDERLINE;
    }

    if (GUISTR_STATE_ITALIC & state)
    {
        dc->font.attr |= SE_LOGFONT_ITALIC;
    }

    if (GUISTR_STATE_BOLD & state)
    {
        dc->font.attr |= SE_LOGFONT_BOLD;
    }

    if ((GUISTR_STATE_WORDBREAK & state) == 0)
    {
        dc->text_flags |= SE_FLAG_WRAPCHAR;
    }
    line_end_offset = 0;
    line_width = 0;
    
    // 速度优化
    if (length > 128)
        length = 128;
    
    SCI_GetMutex(s_scripteasy_mutex_p, SCI_WAIT_FOREVER);
    if (dc->text_flags & SE_FLAG_WRAPCHAR)
        se_analysis_next_line_opt((SeDeviceContext*)dc, text, length, &line_end_offset, &line_width);
    else
    se_analysis_next_line((SeDeviceContext*)dc, text, length, &line_end_offset, &line_width);
    SCI_PutMutex(s_scripteasy_mutex_p);
    
	if (line_width > width)
	{
		line_end_offset = 0;
		line_width = 0;
	}

    *char_num = (uint16)line_end_offset;
    *pixel_num = (uint16)line_width;
    
    return TRUE;
}
#endif
/* 
 * font access callback
 */

#ifdef FONT_STUB_SUPPORT
#include "mmi_filemgr.h"

static wchar font_path_name[] = L"E:\\System\\";
static wchar font_file_name[64] = L"\0";

#if defined(__arm) && defined(__BIG_ENDIAN)
static wchar font_ext_name[8] = L".be";
#else
static wchar font_ext_name[8] = L".le";
#endif

static MMIFILE_HANDLE font_file_handle = PNULL;

static unsigned int FontAccessCallback(void* name, void* buffer, unsigned int offset, unsigned int size)
{
    uint32 bytes_read = 0;

    if (MMIAPICOM_Wstrcmp((wchar*)name, font_file_name) != 0 || font_file_handle == 0)
    {
        wchar full_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = L"\0";
        uint32 opt_mode = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;

        MMIAPICOM_Wstrcpy(font_file_name, (wchar*)name);
        if (font_file_handle)
        {
            MMIFILE_CloseFile(font_file_handle);
        }

        MMIAPICOM_Wstrcpy(full_name, font_path_name);
        MMIAPICOM_Wstrcat(full_name, font_file_name);
        MMIAPICOM_Wstrcat(full_name, font_ext_name);

        font_file_handle = MMIFILE_CreateFile(full_name, opt_mode, PNULL, PNULL);

        if (font_file_handle == PNULL)
            return 0;

    }
 /*
    if (font_file_handle == 0)
    {
    #if defined(__arm) && defined(__BIG_ENDIAN)
        wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = L"E:\\System\\fzlth_gb18030_ttf.be";
    #else
        wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1] = L"E:\\System\\fzlth_gb18030_ttf.le";
    #endif
        uint32 opt_mode = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;

        font_file_handle = MMIFILE_CreateFile(path, opt_mode, PNULL, PNULL);

        if (font_file_handle == PNULL)
            return 0;
    }
*/
    if (font_file_handle)
    {
        MMIFILE_SetFilePointer(font_file_handle, offset, 0);
        MMIFILE_ReadFile(font_file_handle, buffer, size, &bytes_read, NULL);
    }

    return bytes_read;
}
#endif /* FONT_STUB_SUPPORT */

#endif
