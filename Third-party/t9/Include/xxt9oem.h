
#ifndef __XXOEM_H__
#define __XXOEM_H__

/*----------------------------------------------------------------------------
 * The maximum MDB word size 
 *----------------------------------------------------------------------------*/
#define T9MAXMDBWORDSIZE 64

/*----------------------------------------------------------------------------
 * Width of T9 Symbols 
 *----------------------------------------------------------------------------*/
#define T9SYMBOLWIDTH 2

#define KOR_DOT_UPDATE 

//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT) || (defined IM_CHINESE_T_SUPPORT) ||(defined IM_TRAD_CHINESE_SUPPORT)
#define T9CHINESE
//#endif

//#ifdef IM_ENGLISH_SUPPORT
#define T9ALPHABETIC
//#endif


/*----------------------------------------------------------------------------
 * Define maximum cdb matches 
 *----------------------------------------------------------------------------*/
#define T9MAXCDBMATCHES 6

/*----------------------------------------------------------------------------
 * Define OEMID 
 *----------------------------------------------------------------------------*/
#define T9OEMID 50030

/*----------------------------------------------------------------------------
 * Max udb word length supported by T9 system 
 *----------------------------------------------------------------------------*/
#define T9MAXUDBWORDSIZE 64

/*----------------------------------------------------------------------------
 * target machine's word size in bytes 
 *----------------------------------------------------------------------------*/
#define T9MINORWIDTH 2

#ifdef T9CHINESE
/*----------------------------------------------------------------------------
 * Size of Chinese spell buffer 
 *----------------------------------------------------------------------------*/
#define T9CCSPELLBUFMAXSIZE 128

/*----------------------------------------------------------------------------
 * Size of Chinese selection page 
 *----------------------------------------------------------------------------*/
#define T9CCSELECTPAGEMAXSIZE 9

/*----------------------------------------------------------------------------
 * Size of Chinese key buffer 
 *----------------------------------------------------------------------------*/
#define T9CCKEYBUFMAXSIZE 55

#endif /* T9CHINESE */

#define T9CSLatin1

#define T9CSLatinA

#define T9CSLatinB

#define T9CSDiacritics

#define T9CSGreekAlphabetic

#define T9CSCyrillicEven

#define T9CSHebrew

#define T9CSArabicAlternate

#define T9CSKoreanChun

#define T9CSDevanagariAlternate

#define T9CSThaiAlternate

#define T9CSTamil

#endif /* __XXOEM_H__ */

