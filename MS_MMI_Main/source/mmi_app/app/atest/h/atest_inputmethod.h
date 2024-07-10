/*****************************************************************************
** File Name:      atest_inputmethod.h                                             *
** Author:         lin.chen                                                  *
** Date:           20/12/2011                                                *
** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe inputmethod          		 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE           NAME             DESCRIPTION                             *
**  20/12/2011     lin.chen          Created                                 **
******************************************************************************/

#ifndef _ATEST_INPUTMETHOD_H
#define _ATEST_INPUTMETHOD_H 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "stdio.h"
//#include "mmiset.h"

#ifdef WIN32
/**--------------------------------------------------------------------*
 **                         Compiler Flag                              *
 **--------------------------------------------------------------------*/

#define  MAX_CHAR_NUM   50
#define  MAX_MULTI_PATH 260
#define  MAX_ABC_NUM  26
#define  MAX_NUMBER_NUM  10
#define  LANGUAGE_NUM  48
#define  TYPICAL_MAX_KEY  9
#define  ATEST_STRING_MAX_LEN  1000

typedef struct
{
	char* Qwerty_key;
	uint16 Qwerty_char;	
}QWERTY_KEY_MAP_T;

typedef struct _ATEST_LANGUAGE_MAP_T
{
	uint16	gui_im_Q_language;
	uint16	gui_im_T_language;
	char*	cstar_qwerty_sheet_name;
	char*	cstar_typical_sheet_name;
	char*	language_up_name;
	char*	language_Chi_name;
}ATEST_LANGUAGE_MAP_T;

typedef enum
{
#ifdef IM_ENGLISH_SUPPORT
	ATEST_LANGUAGE_ENGLISH,
#endif
	
#ifdef IM_ARABIC_SUPPORT
    ATEST_LANGUAGE_ARABIC,
#endif

#ifdef IM_FRENCH_SUPPORT
    ATEST_LANGUAGE_FRENCH,
#endif

#ifdef IM_HINDI_SUPPORT
    ATEST_LANGUAGE_HINDI,
#endif

#ifdef IM_HUNGARIAN_SUPPORT
    ATEST_LANGUAGE_HUNGARIAN,
#endif

#ifdef IM_INDONESIAN_SUPPORT
    ATEST_LANGUAGE_INDONESIAN,
#endif
        
#ifdef IM_MALAY_SUPPORT
    ATEST_LANGUAGE_MALAY,
#endif

#ifdef IM_PORTUGUESE_SUPPORT
    ATEST_LANGUAGE_PORTUGUESE,
#endif

#ifdef IM_RUSSIAN_SUPPORT
    ATEST_LANGUAGE_RUSSIAN,
#endif

#ifdef IM_SPANISH_SUPPORT
    ATEST_LANGUAGE_SPANISH,
#endif

#ifdef IM_TAGALOG_SUPPORT 
    ATEST_LANGUAGE_TAGALOG,
#endif

#ifdef IM_THAI_SUPPORT
    ATEST_LANGUAGE_THAI,
#endif

#ifdef IM_VIETNAMESE_SUPPORT
    ATEST_LANGUAGE_VIETNAMESE,
#endif

#ifdef IM_URDU_SUPPORT
    ATEST_LANGUAGE_URDU,
#endif

#ifdef IM_ITALIAN_SUPPORT
    ATEST_LANGUAGE_ITALIAN,
#endif

#ifdef IM_PERSIAN_SUPPORT
    ATEST_LANGUAGE_PERSIAN,
#endif

#ifdef IM_TURKISH_SUPPORT
    ATEST_LANGUAGE_TURKISH,
#endif

#ifdef IM_GERMAN_SUPPORT
    ATEST_LANGUAGE_GERMAN,
#endif

#ifdef IM_GREEK_SUPPORT
    ATEST_LANGUAGE_GREEK, 
#endif   

#ifdef IM_HEBREW_SUPPORT
    ATEST_LANGUAGE_HEBREW,
 #endif
 
 #ifdef IM_BENGALI_SUPPORT
    ATEST_LANGUAGE_BENGALI,
 #endif
 
 #ifdef IM_CZECH_SUPPORT
    ATEST_LANGUAGE_CZECH,
 #endif
 
 #ifdef IM_SLOVENIAN_SUPPORT
    ATEST_LANGUAGE_SLOVENIAN,
 #endif
 
 #ifdef IM_ROMANIAN_SUPPORT
    ATEST_LANGUAGE_ROMANIAN,
 #endif
 
 #ifdef IM_TELUGU_SUPPORT
    ATEST_LANGUAGE_TELUGU,
 #endif
 
 #ifdef IM_MARATHI_SUPPORT
    ATEST_LANGUAGE_MARATHI,
 #endif
 
 #ifdef IM_TAMIL_SUPPORT
    ATEST_LANGUAGE_TAMIL,
 #endif
 
 #ifdef IM_GUJARATI_SUPPORT
    ATEST_LANGUAGE_GUJARATI,
 #endif
 
 #ifdef IM_KANNADA_SUPPORT
    ATEST_LANGUAGE_KANNADA,
 #endif
 
 #ifdef IM_MALAYALAM_SUPPORT
    ATEST_LANGUAGE_MALAYALAM,
 #endif
 
 #ifdef IM_ORIYA_SUPPORT
    ATEST_LANGUAGE_ORIYA,
 #endif
 
 #ifdef IM_PUNJABI_SUPPORT
    ATEST_LANGUAGE_PUNJABI,
 #endif
 
 #ifdef IM_AFRIKAANS_SUPPORT
    ATEST_LANGUAGE_AFRIKAANS,
 #endif
 
 #ifdef IM_ALBANIAN_SUPPORT
    ATEST_LANGUAGE_ALBANIAN,
 #endif
 
 #ifdef IM_ARMENIAN_SUPPORT
    ATEST_LANGUAGE_ARMENIAN,
 #endif
 
 #ifdef IM_AZERBAIJANI_SUPPORT
    ATEST_LANGUAGE_AZERBAIJANI,
 #endif
 
 #ifdef IM_BASQUE_SUPPORT
    ATEST_LANGUAGE_BASQUE,
 #endif
 
 #ifdef IM_BULGARIAN_SUPPORT
    ATEST_LANGUAGE_BULGARIAN,
 #endif
 
 #ifdef IM_CATALAN_SUPPORT
    ATEST_LANGUAGE_CATALAN,
 #endif
 
 #ifdef IM_CROATIAN_SUPPORT
    ATEST_LANGUAGE_CROATIAN,
 #endif
 
 #ifdef IM_DANISH_SUPPORT
    ATEST_LANGUAGE_DANISH,
 #endif
 
 #ifdef IM_DUTCH_SUPPORT
    ATEST_LANGUAGE_DUTCH,
 #endif
 
 #ifdef IM_ESTONIAN_SUPPORT
    ATEST_LANGUAGE_ESTONIAN,
 #endif
 
 #ifdef IM_FILIPINO_SUPPORT
    ATEST_LANGUAGE_FILIPINO,
 #endif
 
 #ifdef IM_FINNISH_SUPPORT
    ATEST_LANGUAGE_FINNISH,
 #endif
 
 #ifdef IM_GALICIAN_SUPPORT
    ATEST_LANGUAGE_GALICIAN,
 #endif
 
 #ifdef IM_GEORGIAN_SUPPORT
    ATEST_LANGUAGE_GEORGIAN,
 #endif
 
 #ifdef IM_HAUSA_SUPPORT
    ATEST_LANGUAGE_HAUSA,
 #endif
 
 #ifdef IM_ICELANDIC_SUPPORT
    ATEST_LANGUAGE_ICELANDIC,
 #endif
 
 #ifdef IM_IGBO_SUPPORT
    ATEST_LANGUAGE_IGBO,
 #endif
 
 #ifdef IM_IRISH_SUPPORT
    ATEST_LANGUAGE_IRISH,
 #endif
 
 #ifdef IM_KAZAKH_SUPPORT
    ATEST_LANGUAGE_KAZAKH,
 #endif
 
 #ifdef IM_LATVIAN_SUPPORT
    ATEST_LANGUAGE_LATVIAN,
 #endif
 
 #ifdef IM_LITHUANIAN_SUPPORT
    ATEST_LANGUAGE_LITHUANIAN,
 #endif
 
 #ifdef IM_MACEDONIAN_SUPPORT
    ATEST_LANGUAGE_MACEDONIAN,
 #endif
 
 #ifdef IM_MOLDOVAN_SUPPORT
    ATEST_LANGUAGE_MOLDOVAN,
 #endif
 
 #ifdef IM_NORWEGIAN_SUPPORT
    ATEST_LANGUAGE_NORWEGIAN,
 #endif
 
 #ifdef IM_POLISH_SUPPORT
    ATEST_LANGUAGE_POLISH,
 #endif
 
 #ifdef IM_SERBIAN_SUPPORT
    ATEST_LANGUAGE_SERBIAN,
 #endif
 
 #ifdef IM_SESOTHO_SUPPORT
    ATEST_LANGUAGE_SESOTHO,
 #endif
 
 #ifdef IM_SLOVAK_SUPPORT
    ATEST_LANGUAGE_SLOVAK,
 #endif
 
 #ifdef IM_SWEDISH_SUPPORT
    ATEST_LANGUAGE_SWEDISH,
 #endif
 
 #ifdef IM_UKRAINIAN_SUPPORT
    ATEST_LANGUAGE_UKRAINIAN,
 #endif
 
 #ifdef IM_YORUBA_SUPPORT
    ATEST_LANGUAGE_YORUBA,
 #endif
 
 #ifdef IM_XHOSA_SUPPORT
    ATEST_LANGUAGE_XHOSA,
 #endif
 
 #ifdef IM_ZULU_SUPPORT	
    ATEST_LANGUAGE_ZULU,
 #endif
 
 #ifdef IM_ASSAMESE_SUPPORT	
    ATEST_LANGUAGE_ASSAMESE,
 #endif
 
 #ifdef IM_SWAHILI_SUPPORT	
    ATEST_LANGUAGE_SWAHILI,
 #endif
 
 #ifdef IM_MYANMAR_SUPPORT	
    ATEST_LANGUAGE_MYANMAR,
 #endif

  #ifdef IM_AMHARIC_SUPPORT	
    ATEST_LANGUAGE_AMHARIC,
 #endif

 #ifdef IM_KHMER_SUPPORT	
    ATEST_LANGUAGE_KHMER,
 #endif

#ifdef IM_LAO_SUPPORT	
    ATEST_LANGUAGE_LAO,
 #endif
 
 
    ATEST_MAX_LANGUAGE
} ATEST_LANGUAGE_TYPE_E;


/**-----------------------------------------------------------------------*
 **                                Function Prototypes                    *
 **-----------------------------------------------------------------------*/
PUBLIC void Atest_InputMethodTesting(BOOLEAN  KeyPadFlag,char *LangTestingFilePath);

#endif//WIN32

#endif //_ATEST_INPUTMETHOD_H

