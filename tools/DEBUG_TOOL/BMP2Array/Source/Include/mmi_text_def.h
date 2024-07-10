#ifndef _MMI_TEXT_DEF_H__
#define _MMI_TEXT_DEF_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

#ifndef  NEW_TEXT_LEN
#define MAX_TEXT_LEN  1024
#else
#define MAX_TEXT_LEN  65536
#endif


#define MAX_TEXT_LEN_D  256

#define MAX_LANG_NUM  256

typedef struct _TEXT_INFO_T_ONE
{
    char szEnglish[ MAX_TEXT_LEN + 1 ];
    char szSimpleChinese[ MAX_TEXT_LEN + 1 ];
} TEXT_INFO_T_ONE, * PTEXTINFOONE;

typedef struct _TEXT_INFO_T
{
    char szEnglish[ MAX_TEXT_LEN + 1 ];
    char szSimpleChinese[ MAX_TEXT_LEN + 1 ];
    char szTraditionalChinese[ MAX_TEXT_LEN + 1 ];
} TEXT_INFO_T, * PTEXTINFO;

typedef struct _TEXT_INFO_T_EX
{
	//之所以多出6个字节
	//对于Unicode有三个字节存放编码类型和文本字节长度
	//另外5个用于在读工程文件时存放4字节对齐的多余的0x00.
	char szLanguage[ MAX_LANG_NUM ][ MAX_TEXT_LEN + 8 ]; 
}TEXT_INFO_T_EX, * PTEXTINFOEX;

typedef enum _TEXT_TYPE_E_ONE
{
	TT_ENGLISH_ONE        = 0,
	TT_CHINESESIMPLE_ONE  = 1,
	TT_MAX_NUM_ONE,
} TEXT_TYPE_E_ONE;

typedef enum _TEXT_TYPE_E
{
	TT_ENGLISH            = 0,
	TT_CHINESESIMPLE      = 1,
    TT_CHINESETRADITIONAL = 2,
	TT_MAX_NUM,
} TEXT_TYPE_E;

typedef struct _KSP_TEXT_T
{
    char   * pTxtTable;
    uint16   nbText;
} KSP_TEXT_T, * PKSP_TEXT_T;



#ifdef __cplusplus
}
#endif

#endif // _MMI_TEXT_DEF_H__