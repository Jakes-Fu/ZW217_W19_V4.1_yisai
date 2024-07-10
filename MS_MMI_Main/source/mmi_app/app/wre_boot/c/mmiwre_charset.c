#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT
/****************************************************************************
** File Name:      adaptor_charset.c                                        *
** Author:         RiancyZhang                                              *
** DATE:           30/12/2010                                               *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file wraps charset-layer for mocor os			    *
**																            *
*****************************************************************************/

#include "mmiwre_charset.h"
#include "guifont.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "../../../../../../wre/wrekernel//include/sdk/config.h"

/////////////////////////////////////////////////////

extern PUBLIC uint32 GUI_WstrToGBK(                         // return the length of gbk_ptr
                                   uint8        *gbk_ptr,   // store the gbk string code
                                   const wchar  *wstr_ptr,  // the unicode stringcode that need to be converted.
                                   uint32       wstr_len    //the unicode length
                                   );

int cp_wcstombs  ( int  flags,  
                  const unsigned short *  src,  
                  int  srclen,  
                  char *  dst,  
                  int  dstlen,  
                  const char *  defchar,  
                  int *  used 
                  )   
{
    int ret=0,ust=(int)sizeof(unsigned short);
    char *pTmpBuff=NULL;
    
    pTmpBuff=(char*)SCI_ALLOCA(srclen*ust+1);
    if(pTmpBuff == NULL)
    {
        //SCI_TRACE_LOW:"cp_wcstombs : malloc fail\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_CHARSET_39_112_2_18_3_10_35_49,(uint8*)"" );
        return 0;
    }
    memset(pTmpBuff,0x00,srclen*ust+1);
    
#ifdef GBK_SUPPORT
    ret =GUI_WstrToGBK((unsigned char*)pTmpBuff, src, (unsigned int)srclen );
#else
    ret = GUI_WstrToGB((unsigned char*)pTmpBuff, src, (unsigned int)srclen);
#endif
    
    if (ret >0)
    {
        if(ret>dstlen) ret=dstlen;
        
        memcpy(dst,pTmpBuff,ret);
    }
    SCI_FREE(pTmpBuff);
    
    return ret;
}
/* return -1 on dst buffer overflow, -2 on invalid input char */
int cp_mbstowcs(int flags,
                const char *src, int srclen,
                unsigned short *dst, int dstlen )
{
    int ret,ust=(int)sizeof(unsigned short);
    unsigned short *pTmpBuff=NULL;
    
    pTmpBuff=(unsigned short*)SCI_ALLOCA((srclen+1)*ust);
    if(pTmpBuff == NULL)
    {
        //SCI_TRACE_LOW:"cp_mbstowcs : malloc fail\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_CHARSET_70_112_2_18_3_10_36_50,(uint8*)"" );
        return 0;
    }
    memset(pTmpBuff,0x00,(srclen+1)*ust);
    
#ifdef GBK_SUPPORT
	// ret =GUI_GBKToWstr( (unsigned char*)pTmpBuff, src, (unsigned int)srclen );
    ret = GUI_GBKToWstr((wchar *)pTmpBuff, (uint8 *)src, (unsigned int)srclen);
#else
    ret = GUI_GBToWstr(pTmpBuff, (const unsigned char*)src, (unsigned int)srclen);
#endif
    
    if (ret >0)
    {
        if(ret>dstlen) ret=dstlen;
        
        memcpy(dst,pTmpBuff,ret*ust);
    }
    
    SCI_FREE(pTmpBuff);
    
    return ret;
}
/*=============================================================
函数名称 : GetDefaultLanguage
功    能 : 获取当前显示语言
参    数 :  void
返   回  :  >-1 : 成功
-1   :  失败

=============================================================*/
typedef struct tagLanguageCov
{
    int     mocorlangue;
    int     wrelangue;
}LANGUAGECOV, *PLANGUAGECOV;

LOCAL const LANGUAGECOV s_langucov[] =
{
    {MMISET_LANGUAGE_ENGLISH,       LT_ENGLISH},
    {MMISET_LANGUAGE_SIMP_CHINESE,  LT_SIMP_CHINESE},
    {MMISET_LANGUAGE_TRAD_CHINESE,  LT_TRAD_CHINESE},
    {MMISET_LANGUAGE_ARABIC,        LT_ARABIC},
    {MMISET_LANGUAGE_FRENCH,        LT_FRENCH},
    {MMISET_LANGUAGE_HINDI,         LT_HINDI},
    {MMISET_LANGUAGE_HUNGARIAN,     LT_HUNGARIAN},
    {MMISET_LANGUAGE_INDONESIAN,    LT_INDONESIAN},
    {MMISET_LANGUAGE_MALAY,         LT_MALAY},
    {MMISET_LANGUAGE_PORTUGUESE,    LT_PORTUGUESE},
    {MMISET_LANGUAGE_RUSSIAN,       LT_RUSSIAN},
    {MMISET_LANGUAGE_SPANISH,       LT_SPANISH},
    {MMISET_LANGUAGE_TAGALOG,       LT_TAGALOG},
    {MMISET_LANGUAGE_THAI,          LT_THAI},
    {MMISET_LANGUAGE_VIETNAMESE,    LT_UIETNAMESE},
    {MMISET_LANGUAGE_URDU,          LT_URDU},
    {MMISET_LANGUAGE_ITALIAN,       LT_ITALIAN},
    {MMISET_LANGUAGE_PERSIAN,       LT_PERSIAN},
    {MMISET_LANGUAGE_TURKISH,       LT_TURKISH},
    {MMISET_LANGUAGE_GERMAN,        LT_GERMAN},
    {MMISET_LANGUAGE_GREEK,         LT_GEEK},
    {MMISET_LANGUAGE_HEBREW,        LT_HEBREW},
    {MMISET_LANGUAGE_BENGALI,       LT_BENGALI},
    {MMISET_LANGUAGE_CZECH,         LT_CZECH},
    {MMISET_LANGUAGE_SLOVENIAN,     LT_SLOVENIAN},
    {MMISET_LANGUAGE_ROMANIAN,      LT_ROMANIAN},
    {MMISET_LANGUAGE_TELUGU,        LT_TELUGU},
    {MMISET_LANGUAGE_MARATHI,       LT_MARATHI},
    {MMISET_LANGUAGE_TAMIL,         LT_TAMIL},
    {MMISET_LANGUAGE_GUJARATI,      LT_GUJARATI},
    {MMISET_LANGUAGE_KANNADA,       LT_KANNADA},
    {MMISET_LANGUAGE_MALAYALAM,     LT_MALAYALAM},
    {MMISET_LANGUAGE_ORIYA,         LT_ORIYA},
    {MMISET_LANGUAGE_PUNJABI,       LT_PUNJABI},
    {MMISET_LANGUAGE_AFRIKAANS,     LT_AFRIKAANS},
    {MMISET_LANGUAGE_ALBANIAN,      LT_ALBANIAN},
    {MMISET_LANGUAGE_ARMENIAN,      LT_ARMENIAN},
    {MMISET_LANGUAGE_AZERBAIJANI,   LT_AZERBAIJANI},
    {MMISET_LANGUAGE_BASQUE,        LT_BASQUE},
    {MMISET_LANGUAGE_BULGARIAN,     LT_BULGARIAN},
    {MMISET_LANGUAGE_CATALAN,       LT_CATALAN},
    {MMISET_LANGUAGE_CROATIAN,      LT_CROATIAN},
    {MMISET_LANGUAGE_DANISH,        LT_DANISH},
    {MMISET_LANGUAGE_DUTCH,         LT_DUTCH},
    {MMISET_LANGUAGE_ESTONIAN,      LT_ESTONIAN},
    {MMISET_LANGUAGE_FILIPINO,      LT_FILIPINO},
    {MMISET_LANGUAGE_FINNISH,       LT_FINNISH},
    {MMISET_LANGUAGE_GALICIAN,      LT_GALICIAN},
    {MMISET_LANGUAGE_GEORGIAN,      LT_GEORGIAN},
    {MMISET_LANGUAGE_HAUSA,         LT_HAUSA},
    {MMISET_LANGUAGE_ICELANDIC,     LT_ICELANDIC},
    {MMISET_LANGUAGE_IGBO,          LT_IGBO},
    {MMISET_LANGUAGE_IRISH,         LT_IRISH},
    {MMISET_LANGUAGE_KAZAKH,        LT_KAZAKH},
    {MMISET_LANGUAGE_LATVIAN,       LT_LATVIAN},
    {MMISET_LANGUAGE_LITHUANIAN,    LT_LITHUANIAN},
    {MMISET_LANGUAGE_MACEDONIAN,    LT_MACEDONIAN},
    {MMISET_LANGUAGE_MOLDOVAN,      LT_MOLDOVAN},
    {MMISET_LANGUAGE_NORWEGIAN,     LT_NORWEGIAN},
    {MMISET_LANGUAGE_POLISH,        LT_POLISH},
    {MMISET_LANGUAGE_SERBIAN,       LT_SERBIAN},
    {MMISET_LANGUAGE_SESOTHO,       LT_SESOTHO},
    {MMISET_LANGUAGE_SLOVAK,        LT_SLOVAK},
    {MMISET_LANGUAGE_SWEDISH,       LT_SWEDISH},
    {MMISET_LANGUAGE_UKRAINIAN,     LT_UKRAINIAN},
    {MMISET_LANGUAGE_YORUBA,        LT_YORUVA},
    {MMISET_LANGUAGE_XHOSA,         LT_XHOSA},
    {MMISET_LANGUAGE_ZULU,          LT_ZULU},
    {MMISET_LANGUAGE_ASSAMESE,      LT_ASSAMESE},
    {MMISET_LANGUAGE_SWAHILI,       LT_SWAHILI},
    {MMISET_LANGUAGE_MYANMAR,       LT_MYANMAR},
    {MMISET_LANGUAGE_AMHARIC,       LT_AMHARIC},
    {MMISET_LANGUAGE_KHMER,         LT_KHMER},
    {MMISET_LANGUAGE_LAO,           LT_LAO},
    {MMISET_MAX_LANGUAGE,           LT_MAX}
};

/* by wei.zhou, 调整做法，进行语言定义适配操作  */
int GetDefaultLanguage(void)
{
    MMISET_LANGUAGE_TYPE_E language_type=MMISET_LANGUAGE_ENGLISH;
    int     i = 0;
    int     wrelangue = 0;

    MMIAPISET_GetLanguageType(&language_type);
    for(i = 0; i < sizeof(s_langucov) / sizeof(LANGUAGECOV); i++)
    {
        if(language_type == s_langucov[i].mocorlangue)
        {
            wrelangue = s_langucov[i].wrelangue;
            break;
        }
    }

    /*
    return (language_type>=MMISET_MAX_LANGUAGE)?-1:(int)language_type;
    */
    return wrelangue;
}
#endif
