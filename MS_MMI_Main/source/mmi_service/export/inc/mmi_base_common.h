#ifndef __MMI_BASE_COMMON_H__
#define __MMI_BASE_COMMON_H__

#include "sci_types.h"
#include "caf.h"
#include "os_api.h"

//the type of language, the sequence of language  in resource editor must be identical to it
typedef enum
{
#ifdef MMI_SIM_LANGUAGE_SUPPORT
    MMISET_LANGUAGE_AUTO,          // Auto.
#endif
    MMISET_LANGUAGE_ENGLISH,        //Ӣ��
#if (defined IM_SIMP_CHINESE_SUPPORT) || (defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
    MMISET_LANGUAGE_SIMP_CHINESE,   //���ļ��� 
#endif
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
    MMISET_LANGUAGE_TRAD_CHINESE,   //���ķ���
#endif
#ifdef IM_ARABIC_SUPPORT
    MMISET_LANGUAGE_ARABIC,         //������
#endif
#ifdef IM_FRENCH_SUPPORT
    MMISET_LANGUAGE_FRENCH,         //����
#endif
#ifdef IM_HINDI_SUPPORT
    MMISET_LANGUAGE_HINDI,          //ӡ��
#endif
#ifdef IM_HUNGARIAN_SUPPORT
    MMISET_LANGUAGE_HUNGARIAN,      //������
#endif
#ifdef IM_INDONESIAN_SUPPORT
    MMISET_LANGUAGE_INDONESIAN,     //ӡ��������
#endif
#ifdef IM_MALAY_SUPPORT
    MMISET_LANGUAGE_MALAY,          //����
#endif
#ifdef IM_PORTUGUESE_SUPPORT
    MMISET_LANGUAGE_PORTUGUESE,     //������
#endif
#ifdef IM_RUSSIAN_SUPPORT
    MMISET_LANGUAGE_RUSSIAN,        //����
#endif
#ifdef IM_SPANISH_SUPPORT
    MMISET_LANGUAGE_SPANISH,        //������
#endif
#ifdef IM_TAGALOG_SUPPORT
    MMISET_LANGUAGE_TAGALOG,        //����ŵ
#endif
#ifdef IM_THAI_SUPPORT
    MMISET_LANGUAGE_THAI,           //̩��
#endif
#ifdef IM_VIETNAMESE_SUPPORT
    MMISET_LANGUAGE_VIETNAMESE,     //Խ����
#endif
#ifdef IM_URDU_SUPPORT
    MMISET_LANGUAGE_URDU,           //�ڶ���
#endif
#ifdef IM_ITALIAN_SUPPORT
    MMISET_LANGUAGE_ITALIAN,        //�����
#endif
#ifdef IM_PERSIAN_SUPPORT
    MMISET_LANGUAGE_PERSIAN,        //��˹
#endif
#ifdef IM_TURKISH_SUPPORT
    MMISET_LANGUAGE_TURKISH,        //������
#endif
#ifdef IM_GERMAN_SUPPORT
    MMISET_LANGUAGE_GERMAN,         //����
#endif
#ifdef IM_GREEK_SUPPORT
    MMISET_LANGUAGE_GREEK,          //ϣ��
#endif
#ifdef IM_HEBREW_SUPPORT
    MMISET_LANGUAGE_HEBREW,         //ϣ����
#endif
#ifdef IM_BENGALI_SUPPORT
	MMISET_LANGUAGE_BENGALI,        //�ϼ���
#endif
#ifdef IM_CZECH_SUPPORT
	MMISET_LANGUAGE_CZECH,          //�ݿ�
#endif
#ifdef IM_SLOVENIAN_SUPPORT
    MMISET_LANGUAGE_SLOVENIAN,      //˹ŵ������
#endif
#ifdef IM_ROMANIAN_SUPPORT
	MMISET_LANGUAGE_ROMANIAN,       //��������
#endif
#ifdef IM_TELUGU_SUPPORT
    MMISET_LANGUAGE_TELUGU,
#endif
#ifdef IM_MARATHI_SUPPORT
    MMISET_LANGUAGE_MARATHI,
#endif
#ifdef IM_TAMIL_SUPPORT
    MMISET_LANGUAGE_TAMIL,
#endif
#ifdef IM_GUJARATI_SUPPORT
    MMISET_LANGUAGE_GUJARATI,
#endif
#ifdef IM_KANNADA_SUPPORT
    MMISET_LANGUAGE_KANNADA,
#endif
#ifdef IM_MALAYALAM_SUPPORT
    MMISET_LANGUAGE_MALAYALAM,
#endif
#ifdef IM_ORIYA_SUPPORT
    MMISET_LANGUAGE_ORIYA,
#endif
#ifdef IM_PUNJABI_SUPPORT
    MMISET_LANGUAGE_PUNJABI,
#endif
#ifdef IM_AFRIKAANS_SUPPORT
    MMISET_LANGUAGE_AFRIKAANS,
#endif
#ifdef IM_ALBANIAN_SUPPORT
    MMISET_LANGUAGE_ALBANIAN,
#endif
#ifdef IM_ARMENIAN_SUPPORT
    MMISET_LANGUAGE_ARMENIAN,
#endif
#ifdef IM_AZERBAIJANI_SUPPORT
    MMISET_LANGUAGE_AZERBAIJANI,
#endif
#ifdef IM_BASQUE_SUPPORT
    MMISET_LANGUAGE_BASQUE,
#endif
#ifdef IM_BULGARIAN_SUPPORT
    MMISET_LANGUAGE_BULGARIAN,
#endif
#ifdef IM_CATALAN_SUPPORT
    MMISET_LANGUAGE_CATALAN,
#endif
#ifdef IM_CROATIAN_SUPPORT
    MMISET_LANGUAGE_CROATIAN,
#endif
#ifdef IM_DANISH_SUPPORT
    MMISET_LANGUAGE_DANISH,
#endif
#ifdef IM_DUTCH_SUPPORT
    MMISET_LANGUAGE_DUTCH,
#endif
#ifdef IM_ESTONIAN_SUPPORT
    MMISET_LANGUAGE_ESTONIAN,
#endif
#ifdef IM_FILIPINO_SUPPORT
    MMISET_LANGUAGE_FILIPINO,
#endif
#ifdef IM_FINNISH_SUPPORT
    MMISET_LANGUAGE_FINNISH,
#endif
#ifdef IM_GALICIAN_SUPPORT
    MMISET_LANGUAGE_GALICIAN,
#endif
#ifdef IM_GEORGIAN_SUPPORT
    MMISET_LANGUAGE_GEORGIAN,
#endif
#ifdef IM_HAUSA_SUPPORT
    MMISET_LANGUAGE_HAUSA,
#endif
#ifdef IM_ICELANDIC_SUPPORT
    MMISET_LANGUAGE_ICELANDIC,
#endif
#ifdef IM_IGBO_SUPPORT
    MMISET_LANGUAGE_IGBO,
#endif
#ifdef IM_IRISH_SUPPORT
    MMISET_LANGUAGE_IRISH,
#endif
#ifdef IM_KAZAKH_SUPPORT
    MMISET_LANGUAGE_KAZAKH,
#endif
#ifdef IM_LATVIAN_SUPPORT
    MMISET_LANGUAGE_LATVIAN,
#endif
#ifdef IM_LITHUANIAN_SUPPORT
    MMISET_LANGUAGE_LITHUANIAN,
#endif
#ifdef IM_MACEDONIAN_SUPPORT
    MMISET_LANGUAGE_MACEDONIAN,
#endif
#ifdef IM_MOLDOVAN_SUPPORT
    MMISET_LANGUAGE_MOLDOVAN,
#endif
#ifdef IM_NORWEGIAN_SUPPORT
    MMISET_LANGUAGE_NORWEGIAN,
#endif
#ifdef IM_POLISH_SUPPORT
    MMISET_LANGUAGE_POLISH,
#endif
#ifdef IM_SERBIAN_SUPPORT
    MMISET_LANGUAGE_SERBIAN,
#endif
#ifdef IM_SESOTHO_SUPPORT
    MMISET_LANGUAGE_SESOTHO,
#endif
#ifdef IM_SLOVAK_SUPPORT
    MMISET_LANGUAGE_SLOVAK,
#endif
#ifdef IM_SWEDISH_SUPPORT
    MMISET_LANGUAGE_SWEDISH,
#endif
#ifdef IM_UKRAINIAN_SUPPORT
    MMISET_LANGUAGE_UKRAINIAN,
#endif
#ifdef IM_YORUBA_SUPPORT
    MMISET_LANGUAGE_YORUBA,
#endif
#ifdef IM_XHOSA_SUPPORT
    MMISET_LANGUAGE_XHOSA,		//������	add. chenyg@spread. 2011-05-12
#endif
#ifdef IM_ZULU_SUPPORT
    MMISET_LANGUAGE_ZULU,		//��³��	add. chenyg@spread. 2011-05-12
#endif
#ifdef IM_ASSAMESE_SUPPORT
    MMISET_LANGUAGE_ASSAMESE,		//(ӡ����֧��)����ķ��	add. chenyg@spread. 2011-05-12
#endif
#ifdef IM_SWAHILI_SUPPORT
    MMISET_LANGUAGE_SWAHILI,	//˹������	add. chenyg@spread. 2011-05-12
#endif
#ifdef IM_MYANMAR_SUPPORT
    MMISET_LANGUAGE_MYANMAR,    //�����
#endif
#ifdef IM_AMHARIC_SUPPORT
    MMISET_LANGUAGE_AMHARIC,    //��ķ����
#endif
#ifdef IM_KHMER_SUPPORT
    MMISET_LANGUAGE_KHMER,    //����կ
#endif
#ifdef IM_LAO_SUPPORT
    MMISET_LANGUAGE_LAO,    //����
#endif
#ifdef IM_UYGHUR_SUPPORT
    MMISET_LANGUAGE_UYGHUR,    //ά�����
#endif
#ifdef IM_TIBETAN_SUPPORT   
    MMISET_LANGUAGE_TIBETAN,   //����
#endif
#ifdef IM_SINHALESE_SUPPORT
    MMISET_LANGUAGE_SINHALESE,	//ɮ٤��
#endif    
    MMISET_MAX_LANGUAGE
} MMISET_LANGUAGE_TYPE_E;

#ifdef STANDALONE_TEST
//#define _MMI_SLST_UNIT_TEST_
//#define _MMI_DLST_UNIT_TEST_
//#define _MMI_STCK_UNIT_TEST_
//#define _MMI_QUEUE_UNIT_TEST_
//#define _MMI_ALST_UNIT_TEST_
//#define _MMI_HASHMAP_UNIT_TEST_
//#define _MMI_STRING_UNIT_TEST_
//#define _MMI_TIMEDATE_STRING_UNIT_TEST_
//#define _MMI_TREE_UNIT_TEST_

//#define MMIBD_ASSERT_DEBUG  assert
#define SCI_ALLOCA          malloc
#define SCI_ReAlloc         realloc
#define SCI_MEMCPY          memcpy
#define SCI_MEMMOVE         memmove
#define SCI_FREE            free
#define SCI_Free            free
#define SCI_MallocEx        malloc
#define PNULL               0

typedef unsigned int    uint32;
typedef int             int32;
typedef unsigned short  uint16;
typedef short           int16;
typedef unsigned char   uint8;
//typedef uint16          wchar;
typedef int             bool32;
typedef int             status_t;
typedef int             ssize_t;
#endif

#define MMIBD_ASSERT_DEBUG(x)

#endif
