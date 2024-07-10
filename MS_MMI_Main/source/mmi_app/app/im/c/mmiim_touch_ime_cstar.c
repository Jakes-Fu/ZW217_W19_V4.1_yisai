/*==============================================================================
File Name: mmiim_touch_ime_cstar.c
Author: Lianxiang.Zhou
Date: 2011/12/13
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/13   Lianxiang.Zhou  Create.
==============================================================================*/



#include "mmi_app_im_trc.h"
#ifdef WIN32
#pragma warning (disable:4244)
#pragma warning (disable:4127)
#pragma warning (disable:4761)
#endif


/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/

#include "mmiim_touch_ime_cstar.h"

#if defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_CSTAR)

#if defined IM_BENGALI_SUPPORT
#define BENGALI_UNICODE_BEGIN 0x0980
#define BENGALI_UNICODE_END 0x09FF
#define BENGALI_DIGIT_BEGIN 0x09E6
#define BENGALI_DIGIT_END 0x09EF

#define HASANATA (0x09CD)
//0xEAC1 = 0x09cd + 0x09b0, 0xEA4B = 0x09cd + 0x09af, 0xEAAB = 0x09b0 + 0x09cd�����ַ�Ϊ����ַ�����������
//CSTAR����������ֺ����Ƕ���������ֲ�ͬ�����Ǻ�������ͬ��
#define RAPHALA (0x09FC) 	//(0xEAC1)
#define JAPHALA (0x09FD) 	//(0xEA4B)
#define RAPH    (0x09FE) 	//(0xEAAB)

LOCAL const wchar s_bengali_consonant[] = 	//����
{
	0x0995, 0x0996, 0x0997, 0x0998, 0x0999, 	//4
	0x099A, 0x099B, 0x099C, 0x099D, 0x099E, 	//5
	0x099F, 0x09A0, 0x09A1, 0x09A2, 0x09A3, 	//6
	0x09A4, 0x09A5, 0x09A6, 0x09A7, 0x09A8, 	//7
	0x09AA, 0x09AB, 0x09AC, 0x09AD, 0x09AE, 	//8
	0x09AF, 0x09B0, 0x09B2, 0x09B6, 0x09B7, 0x09B8,	//9 
	0x09b9, 0x09dc, 0x09dd, 0x09df, 0x09ce,		//0
	0x0000		//������־
};		

LOCAL const wchar s_bengali_independent_vowel[] = //����Ԫ��		
{
	0x0985, 0x0986, 0x0987, 0x0988, 0x0989, 0x098A, 0x098B,  		//2
	0x098F, 0x0990, 0x0993, 0x0994, 			//3
	0x0000		//������־
};	

LOCAL const wchar s_bengali_dependent_vowel[] = //�Ƕ���Ԫ��
{
	0x09BE, 0x09BF, 0x09C0, 0x09C1, 0x09C2, 0x09C3,  	//2
	0x09C7, 0x09C8, 0x09CB, 0x09CC, 	//3
	0x0000		//������־
};		

#endif


/*
��ע1�� ʹ��kmx�ӿڲ�ѯ�õ����ݿ������ʹ��cs_event�ӿڲ�ѯ������һ�£�
        ��������һ������link����ʿ���˷�����
��ע2�� ����PDA��UI���ʾ����ˣ�ƴ�����뷨����ʹ�ô������룬�������ǵ�������
��ע3�� �������û�еĻ�����ԭ���߼�Ĭ�Ϸ��ص�һ��Ӣ�����ݿ�
��ע4�� ��Ϊ���������������ҷ��֣������������⣬Ĭ�ϵ�һ��ƴ�����ڶ����ʻ�
        add by haiwu.chen 2010-11-07
*/
typedef struct
{
    GUIIM_LANGUAGE_T lang;
    const void ** kmx_ldb_pptr;
} KMX_LANG_DATA_T;

static const KMX_LANG_DATA_T kmx_lang_config[] =
{
#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
    {
        GUIIM_LANG_ENGLISH, &kmx_ldb_english_107
    },
#else
    {GUIIM_LANG_ENGLISH, &kmx_ldb_english_103},
#endif

#if defined(IM_SIMP_CHINESE_SUPPORT) //????
#if defined(MMI_IM_PDA_SUPPORT)
#if defined(INPUT_METHOD_CSTAR_PLAIN)
    {
        GUIIM_LANG_CHS, &kmx_ldb_py_gb2312_c2_20k
    },
    {GUIIM_LANG_CHS, &kmx_ldb_stroke_chs},
#else
    {GUIIM_LANG_CHS, &kmx_ldb_py_gb2312_c2_10k},
    {GUIIM_LANG_CHS, &kmx_ldb_stroke_gb2312_mini},
#endif
#else
#if defined(INPUT_METHOD_CSTAR_PLAIN)
    {
        GUIIM_LANG_CHS, &kmx_ldb_py_gb2312_c2_20k
    },
    {GUIIM_LANG_CHS, &kmx_ldb_stroke_gb2312_plain},
#elif defined(INPUT_METHOD_CSTAR_GBK)
    {
        GUIIM_LANG_CHS, &kmx_ldb_py_gbk_c2_20k
    },
    {GUIIM_LANG_CHS, &kmx_ldb_stroke_gbk_plain},
#elif defined(INPUT_METHOD_CSTAR_MINI)
    {
        GUIIM_LANG_CHS, &kmx_ldb_dzpy_gb2312_20k
    },
    {GUIIM_LANG_CHS, &kmx_ldb_stroke_gb2312_mini},
#elif defined(INPUT_METHOD_CSTAR_MINI_GBK)
    {
        GUIIM_LANG_CHS, &kmx_ldb_dzpy_gbk_20k
    },
    {GUIIM_LANG_CHS, &kmx_ldb_stroke_gb2312_mini},
#elif defined(INPUT_METHOD_CSTAR_TINY)
    {
        GUIIM_LANG_CHS, &kmx_ldb_dzpy_gb2312_0k
    },
    {GUIIM_LANG_CHS, &kmx_ldb_stroke_gb2312_mini},
#endif
#endif  //endif MMI_PDA_SUPPORT
#endif  //endif IM_SIMP_CHINESE_SUPPORT

#if defined(IM_TRAD_CHINESE_SUPPORT) //????
#if defined(MMI_IM_PDA_SUPPORT)
    {
        GUIIM_LANG_CHT, &kmx_ldb_zy_big5_20k
    },
    {GUIIM_LANG_CHT, &kmx_ldb_stroke_big5_plain},
#else
#if defined(INPUT_METHOD_CSTAR_PLAIN)
    {
        GUIIM_LANG_CHT, &kmx_ldb_zy_big5_20k
    },
    {GUIIM_LANG_CHT, &kmx_ldb_stroke_big5_plain},
#elif defined(INPUT_METHOD_CSTAR_GBK)
    {
        GUIIM_LANG_CHT, &kmx_ldb_zy_big5_20k
    },
    {GUIIM_LANG_CHT, &kmx_ldb_stroke_big5_plain},
#elif defined(INPUT_METHOD_CSTAR_MINI)
    {
        GUIIM_LANG_CHT, &kmx_ldb_dzzy_big5normal_20k
    },
    {GUIIM_LANG_CHT, &kmx_ldb_stroke_big5_normal_mini},
#elif defined(INPUT_METHOD_CSTAR_MINI_GBK)
    {
        GUIIM_LANG_CHT, &kmx_ldb_dzzy_big5normal_20k
    },
    {GUIIM_LANG_CHT, &kmx_ldb_stroke_big5_normal_mini},
#elif defined(INPUT_METHOD_CSTAR_TINY)
    {
        GUIIM_LANG_CHT, &kmx_ldb_dzzy_big5normal_0k
    },
    {GUIIM_LANG_CHT, &kmx_ldb_stroke_big5_normal_mini},
#endif
#endif  //endif MMI_PDA_SUPPORT
#endif  //endif IM_TRAD_CHINESE_SUPPORT

#if defined(INPUT_METHOD_CSTAR_COMMON_MINI)
#if defined(IM_ARABIC_SUPPORT)//????
#if defined(SPECIAL_ARABIC_PERSIAN)
    {
        GUIIM_LANG_ARABIC, &kmx_ldb_arabic_809
    },
#else
    {GUIIM_LANG_ARABIC, &kmx_ldb_arabic_807},
#endif
#endif

#if defined(IM_HINDI_SUPPORT)//???
#if defined(SPECIAL_HINDI)
    {
        GUIIM_LANG_HINDI, &kmx_ldb_hindi_908
    },
#else
    {GUIIM_LANG_HINDI, &kmx_ldb_hindi_907},
#endif

#endif

#if defined(IM_THAI_SUPPORT)//??
#if defined(SPECIAL_THAI)
    {
        GUIIM_LANG_THAI, &kmx_ldb_thai_368
    },
#else
    {GUIIM_LANG_THAI, &kmx_ldb_thai_367},
#endif
#endif

#if defined(IM_RUSSIAN_SUPPORT)//??
    {
        GUIIM_LANG_RUSSIAN, &kmx_ldb_russian_207
    },
#endif

#if defined(IM_GERMAN_SUPPORT)//??
    {
        GUIIM_LANG_GERMAN, &kmx_ldb_german_307
    },
#endif

#if defined(IM_VIETNAMESE_SUPPORT)//???
    {
        GUIIM_LANG_VIETNAMESE, &kmx_ldb_vietnamese_317
    },
#endif

#if defined(IM_INDONESIAN_SUPPORT)//??????
    {
        GUIIM_LANG_INDONESIAN, &kmx_ldb_indonesian_327
    },
#endif

#if defined(IM_TURKISH_SUPPORT)//????
    {
        GUIIM_LANG_TURKISH, &kmx_ldb_turkish_337
    },
#endif

#if defined(IM_MALAY_SUPPORT)//???
    {
        GUIIM_LANG_MALAY, &kmx_ldb_malay_347
    },
#endif

#if defined(IM_GREEK_SUPPORT)//???
    {
        GUIIM_LANG_GREEK, &kmx_ldb_greek_357
    },
#endif

#if defined(IM_DANISH_SUPPORT)//???
    {
        GUIIM_LANG_DANISH, &kmx_ldb_danish_377
    },
#endif

#if defined(IM_DUTCH_SUPPORT)//???
    {
        GUIIM_LANG_DUTCH, &kmx_ldb_dutch_387
    },
#endif

#if defined(IM_SWAHILI_SUPPORT)//˹��������
    {
        GUIIM_LANG_SWAHILI, &kmx_ldb_swahili_557
    },
#endif

#if defined(IM_POLISH_SUPPORT)//������
    {
        GUIIM_LANG_POLISH, &kmx_ldb_polish_397
    },
#endif

#if defined(IM_FRENCH_SUPPORT)//����
    {
        GUIIM_LANG_FRENCH, &kmx_ldb_french_407
    },
#endif

#if defined(IM_URDU_SUPPORT)//�ڶ�����
    {
        GUIIM_LANG_URDU, &kmx_ldb_urdu_957
    },
#endif

#if defined(IM_HUNGARIAN_SUPPORT)//��������
    {
        GUIIM_LANG_HUNGARIAN, &kmx_ldb_hungarian_417
    },
#endif

#if defined(IM_HEBREW_SUPPORT)//ϣ������
    {
        GUIIM_LANG_HEBREW, &kmx_ldb_hebrew_427
    },
#endif

#if defined(IM_BENGALI_SUPPORT)//�ϼ�����
    {
        GUIIM_LANG_BENGALI, &kmx_ldb_bengali_924
    },
#endif

#if defined(IM_BULGARIAN_SUPPORT)//����������
    {
        GUIIM_LANG_BULGARIAN, &kmx_ldb_bulgarian_437
    },
#endif

#if defined(IM_CZECH_SUPPORT)//�ݿ���
    {
        GUIIM_LANG_CZECH, &kmx_ldb_czech_447
    },
#endif

#if defined(IM_FINNISH_SUPPORT)//������
    {
        GUIIM_LANG_FINNISH, &kmx_ldb_finnish_457
    },
#endif

#if defined(IM_PERSIAN_SUPPORT)//��˹��
#if defined(SPECIAL_ARABIC_PERSIAN)
    {
        GUIIM_LANG_PERSIAN, &kmx_ldb_persian_469
    },
#else
    {GUIIM_LANG_PERSIAN, &kmx_ldb_persian_467},
#endif
#endif

#if defined(IM_SLOVAK_SUPPORT)//˹�工����
    {
        GUIIM_LANG_SLOVAK, &kmx_ldb_slovak_477
    },
#endif

#if defined(IM_NORWEGIAN_SUPPORT)//Ų����
    {
        GUIIM_LANG_NORWEGIAN, &kmx_ldb_norwegian_487
    },
#endif

#if defined(IM_SWEDISH_SUPPORT)//�����
    {
        GUIIM_LANG_SWEDISH, &kmx_ldb_swedish_497
    },
#endif

#if defined(IM_SPANISH_SUPPORT)//��������
    {
        GUIIM_LANG_SPANISH, &kmx_ldb_spanish_507
    },
#endif

#if defined(IM_ROMANIAN_SUPPORT)//����������
    {
        GUIIM_LANG_ROMANIAN, &kmx_ldb_romanian_527
    },
#endif

#if defined(IM_SLOVENIAN_SUPPORT)//˹ŵ��������
    {
        GUIIM_LANG_SLOVENIAN, &kmx_ldb_slovenian_537
    },
#endif

#if defined(IM_PORTUGUESE_SUPPORT)//��������
    {
        GUIIM_LANG_PORTUGUESE, &kmx_ldb_portuguese_607
    },
#endif

#if defined(IM_ITALIAN_SUPPORT)//�������
    {
        GUIIM_LANG_ITALIAN, &kmx_ldb_italian_707
    },
#endif

#if defined(IM_TAGALOG_SUPPORT)//����ŵ��
    {
        GUIIM_LANG_TAGALOG, &kmx_ldb_tagalog_587
    },
#endif

//#if defined(IM_HINGLISH_SUPPORT)//ӡ��Ӣ��
//    {GUIIM_LANG_H, &kmx_ldb_hinglish_917},
//#endif

#if defined(IM_LITHUANIAN_SUPPORT)//��������
    {
        GUIIM_LANG_LITHUANIAN, &kmx_ldb_lithuanian_567
    },
#endif

#if defined(IM_UKRAINIAN_SUPPORT)//�ڿ�����
    {
        GUIIM_LANG_UKRAINIAN, &kmx_ldb_ukrainian_547
    },
#endif
#if defined(IM_TELUGU_SUPPORT)//̩¬����
    {
        GUIIM_LANG_TELUGU, &kmx_ldb_telugu
    },
#endif

#if defined(IM_MARATHI_SUPPORT)//��������
    {
        GUIIM_LANG_MARATHI, &kmx_ldb_marathi
    },
#endif

#if defined(IM_TAMIL_SUPPORT)//̩�׶���
    {
        GUIIM_LANG_TAMIL, &kmx_ldb_tamil
    },
#endif

#if defined(IM_PUNJABI_SUPPORT)//��������
    {
        GUIIM_LANG_PUNJABI, &kmx_ldb_punjabi
    },
#endif

#if defined(IM_GUJARATI_SUPPORT)//����������
    {
        GUIIM_LANG_GUJARATI, &kmx_ldb_gujarati
    },
#endif

#if defined(IM_ORIYA_SUPPORT)//��������
    {
        GUIIM_LANG_ORIYA, &kmx_ldb_oriya
    },
#endif

#if defined(IM_KANNADA_SUPPORT)//���ɵ���
    {
        GUIIM_LANG_KANNADA, &kmx_ldb_kannada
    },
#endif

#if defined(IM_MALAYALAM_SUPPORT)//����������
    {
        GUIIM_LANG_MALAYALAM, &kmx_ldb_malayalam
    },
#endif

#if defined(IM_MYANMAR_SUPPORT)//�����
    {GUIIM_LANG_MYANMAR, &kmx_ldb_myanmar},
#endif

#if defined(IM_KHMER_SUPPORT)//������
    {GUIIM_LANG_KHMER, &kmx_ldb_khmer},
#endif

#if defined(IM_LAO_SUPPORT)//������
    {GUIIM_LANG_LAO, &kmx_ldb_lao},
#endif

//#if defined(IM_AMHARIC_SUPPORT)//��ķ������
//   {GUIIM_LANG_AM, &kmx_ldb_amharic},
//#endif

#if defined(IM_ARMENIAN_SUPPORT)//����������
    {
        GUIIM_LANG_ARMENIAN, &kmx_ldb_armenian
    },
#endif

#if defined(IM_UYGHUR_SUPPORT)//ά�����
    {GUIIM_LANG_UYGHUR, &kmx_ldb_uyghur},
#endif

#if defined(IM_TIBETAN_SUPPORT)  
    {GUIIM_LANG_TIBETAN, &kmx_ldb_tibetan_730},
#endif

#else   /* INPUT_METHOD_CSTAR_COMMON_MINI */

#if defined(IM_ARABIC_SUPPORT)//��������
#if defined(SPECIAL_ARABIC_PERSIAN)
    {
        GUIIM_LANG_ARABIC, &kmx_ldb_arabic_803
    },
#else
    {GUIIM_LANG_ARABIC, &kmx_ldb_arabic_800},
#endif
#endif

#if defined(IM_HINDI_SUPPORT)//ӡ����
#if defined(SPECIAL_HINDI)
    {
        GUIIM_LANG_HINDI, &kmx_ldb_hindi_906
    },
#else
    {GUIIM_LANG_HINDI, &kmx_ldb_hindi_900},
#endif
#endif

#if defined(IM_THAI_SUPPORT)//̩��
#if defined(SPECIAL_THAI)
    {
        GUIIM_LANG_THAI, &kmx_ldb_thai_364
    },
#else
    {GUIIM_LANG_THAI, &kmx_ldb_thai_360},
#endif
#endif

#if defined(IM_RUSSIAN_SUPPORT)//����
    {
        GUIIM_LANG_RUSSIAN, &kmx_ldb_russian_202
    },
#endif

#if defined(IM_GERMAN_SUPPORT)//����
    {
        GUIIM_LANG_GERMAN, &kmx_ldb_german_300
    },
#endif

#if defined(IM_VIETNAMESE_SUPPORT)//Խ����
    {
        GUIIM_LANG_VIETNAMESE, &kmx_ldb_vietnamese_310
    },
#endif

#if defined(IM_INDONESIAN_SUPPORT)//ӡ����������
    {
        GUIIM_LANG_INDONESIAN, &kmx_ldb_indonesian_320
    },
#endif

#if defined(IM_TURKISH_SUPPORT)//��������
    {
        GUIIM_LANG_TURKISH, &kmx_ldb_turkish_330
    },
#endif

#if defined(IM_MALAY_SUPPORT)//������
    {
        GUIIM_LANG_MALAY, &kmx_ldb_malay_340
    },
#endif

#if defined(IM_GREEK_SUPPORT)//ϣ����
    {
        GUIIM_LANG_GREEK, &kmx_ldb_greek_350
    },
#endif

#if defined(IM_DANISH_SUPPORT)//������
    {
        GUIIM_LANG_DANISH, &kmx_ldb_danish_370
    },
#endif

#if defined(IM_DUTCH_SUPPORT)//������
    {
        GUIIM_LANG_DUTCH, &kmx_ldb_dutch_380
    },
#endif

#if defined(IM_SWAHILI_SUPPORT)//˹��������
    {
        GUIIM_LANG_SWAHILI, &kmx_ldb_swahili_550
    },
#endif

#if defined(IM_POLISH_SUPPORT)//������
    {
        GUIIM_LANG_POLISH, &kmx_ldb_polish_391
    },
#endif

#if defined(IM_FRENCH_SUPPORT)//����
    {
        GUIIM_LANG_FRENCH, &kmx_ldb_french_400
    },
#endif

#if defined(IM_URDU_SUPPORT)//�ڶ�����
    {
        GUIIM_LANG_URDU, &kmx_ldb_urdu_950
    },
#endif

#if defined(IM_HUNGARIAN_SUPPORT)//��������
    {
        GUIIM_LANG_HUNGARIAN, &kmx_ldb_hungarian_410
    },
#endif

#if defined(IM_HEBREW_SUPPORT)//ϣ������
    {
        GUIIM_LANG_HEBREW, &kmx_ldb_hebrew_420
    },
#endif

#if defined(IM_BENGALI_SUPPORT)//�ϼ�����
    {
        GUIIM_LANG_BENGALI, &kmx_ldb_bengali_924
    },
#endif

#if defined(IM_BULGARIAN_SUPPORT)//����������
    {
        GUIIM_LANG_BULGARIAN, &kmx_ldb_bulgarian_430
    },
#endif

#if defined(IM_CZECH_SUPPORT)//�ݿ���
    {
        GUIIM_LANG_CZECH, &kmx_ldb_czech_440
    },
#endif

#if defined(IM_FINNISH_SUPPORT)//������
    {
        GUIIM_LANG_FINNISH, &kmx_ldb_finnish_450
    },
#endif

#if defined(IM_PERSIAN_SUPPORT)//��˹��
#if defined(SPECIAL_ARABIC_PERSIAN)
    {
        GUIIM_LANG_PERSIAN, &kmx_ldb_persian_463
    },
#else
    {GUIIM_LANG_PERSIAN, &kmx_ldb_persian_460},
#endif
#endif

#if defined(IM_SLOVAK_SUPPORT)//˹�工����
    {
        GUIIM_LANG_SLOVAK, &kmx_ldb_slovak_470
    },
#endif

#if defined(IM_NORWEGIAN_SUPPORT)//Ų����
    {
        GUIIM_LANG_NORWEGIAN, &kmx_ldb_norwegian_480
    },
#endif

#if defined(IM_SWEDISH_SUPPORT)//�����
    {
        GUIIM_LANG_SWEDISH, &kmx_ldb_swedish_490
    },
#endif

#if defined(IM_SPANISH_SUPPORT)//��������
    {
        GUIIM_LANG_SPANISH, &kmx_ldb_spanish_500
    },
#endif

#if defined(IM_ROMANIAN_SUPPORT)//����������
    {
        GUIIM_LANG_ROMANIAN, &kmx_ldb_romanian_520
    },
#endif

#if defined(IM_SLOVENIAN_SUPPORT)//˹ŵ��������
    {
        GUIIM_LANG_SLOVENIAN, &kmx_ldb_slovenian_530
    },
#endif

#if defined(IM_PORTUGUESE_SUPPORT)//��������
    {
        GUIIM_LANG_PORTUGUESE, &kmx_ldb_portuguese_600
    },
#endif

#if defined(IM_ITALIAN_SUPPORT)//�������
    {
        GUIIM_LANG_ITALIAN, &kmx_ldb_italian_700
    },
#endif

#if defined(IM_TAGALOG_SUPPORT)//����ŵ��
    {
        GUIIM_LANG_TAGALOG, &kmx_ldb_tagalog_587
    },
#endif

#if defined(IM_TELUGU_SUPPORT)//̩¬����
    {
        GUIIM_LANG_TELUGU, &kmx_ldb_telugu
    },
#endif

#if defined(IM_HINGLISH_SUPPORT)//ӡ��Ӣ��
    {
        GUIIM_LANG_HINDI, &kmx_ldb_hinglish_910
    },
#endif

#if defined(IM_LITHUANIAN_SUPPORT)//��������
    {
        GUIIM_LANG_LITHUANIAN, &kmx_ldb_lithuanian_560
    },
#endif

#if defined(IM_UKRAINIAN_SUPPORT)//�ڿ�����
    {
        GUIIM_LANG_UKRAINIAN, &kmx_ldb_ukrainian_540
    },
#endif

#if defined(IM_MARATHI_SUPPORT)//��������
    {
        GUIIM_LANG_MARATHI, &kmx_ldb_marathi
    },
#endif

#if defined(IM_TAMIL_SUPPORT)//̩�׶���
    {
        GUIIM_LANG_TAMIL, &kmx_ldb_tamil
    },
#endif

#if defined(IM_PUNJABI_SUPPORT)//��������
    {
        GUIIM_LANG_PUNJABI, &kmx_ldb_punjabi
    },
#endif

#if defined(IM_GUJARATI_SUPPORT)//����������
    {
        GUIIM_LANG_GUJARATI, &kmx_ldb_gujarati
    },
#endif

#if defined(IM_ORIYA_SUPPORT)//��������
    {
        GUIIM_LANG_ORIYA, &kmx_ldb_oriya
    },
#endif

#if defined(IM_KANNADA_SUPPORT)//���ɵ���
    {
        GUIIM_LANG_KANNADA, &kmx_ldb_kannada
    },
#endif

#if defined(IM_MALAYALAM_SUPPORT)//����������
    {
        GUIIM_LANG_MALAYALAM, &kmx_ldb_malayalam
    },
#endif

#if defined(IM_MYANMAR_SUPPORT)//�����
    {GUIIM_LANG_MYANMAR, &kmx_ldb_myanmar},
#endif

#if defined(IM_KHMER_SUPPORT)//������
    {GUIIM_LANG_KHMER, &kmx_ldb_khmer},
#endif

#if defined(IM_LAO_SUPPORT)//������
    {GUIIM_LANG_LAO, &kmx_ldb_lao},
#endif

//#if defined(IM_AMHARIC_SUPPORT)//��ķ������
//    {GUIIM_LANG_G, &kmx_ldb_amharic},
//#endif

#if defined(IM_ARMENIAN_SUPPORT)//����������
    {
        GUIIM_LANG_ARMENIAN, &kmx_ldb_armenian
    },
#endif

#if defined(IM_UYGHUR_SUPPORT)//ά�����
    {GUIIM_LANG_UYGHUR, &kmx_ldb_uyghur},
#endif

#if defined(IM_TIBETAN_SUPPORT)   
    {GUIIM_LANG_TIBETAN, &kmx_ldb_tibetan_730},
#endif

#endif
};


static wchar const * const s_thumb_cust_symb = L"\x2c\x2e\x3f\xff01\x2026";


#if defined (IM_TRAD_CHINESE_SUPPORT)
LOCAL BOOLEAN ThumbZhuyinVkeyDown (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
#endif



/*==============================================================================
Description: ThumbLatinInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbLatinInit(IM_DATA_T *data_ptr)
{
    int32 init_res = 0;
    KMX_IME_ENGINE_INIT init_info = {0};

    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_LATIN_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_LATIN_T));

    ThumbLatinInitKb(data_ptr);
    ThumbLatinSetMatch(data_ptr);

    init_info.database = GetKmxLdb_ptr(GUIIM_LANG_ENGLISH, FALSE);
    MMIIM_IME_FindUserCacheByIM(
        data_ptr->init_param.method_param_ptr->im_def_ptr->method,
        &init_info.user_data, &init_info.user_data_size);

    init_res = kmx_latin_initialize(&(((KERNEL_DATA_LATIN_T*)(data_ptr->data))->engine), &init_info);
    MMIIM_SetImKerState(0);
    if (0 == init_res)
    {
        return FALSE;
    }

    //SCI_ASSERT(*(uint32*)((KERNEL_DATA_LATIN_T*)(data_ptr->data))->engine.engine == 0x4c4c4c4c); /*assert verified*/

    return TRUE;
}


/*==============================================================================
Description: ThumbLatinTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbLatinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: ThumbAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbLatinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    ThumbLatinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void ThumbLatinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 8;
    uint8 const SYS_CNT = 2;
    uint8 const CAND_CNT = LATIN_CANDIDATE_COUNT;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMI_STRING_T slide_cue = {PNULL, 0};
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�
    vkb_ptr->key_cnt -= g_thumb_panel_expand_py_cnt; //no expand py

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].text[0] = ",.?!-"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //deal tag
    if (GUIIM_TAG_EMAIL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '@';
        vkb_ptr->keys[cust_begin+2].text[0] = '-';
        vkb_ptr->keys[cust_begin+3].text[0] = '_';
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }
    else if (GUIIM_TAG_URL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '/';
        vkb_ptr->keys[cust_begin+2].text_len = 7;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+2].text, L"http://", vkb_ptr->keys[cust_begin+2].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+3].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+3].text, L"www.", vkb_ptr->keys[cust_begin+3].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_EXCLUDE_DIGIT;
        //=========��������˿ӡ����
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbLatinCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbLatinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //--0 ���һ��char
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE;

    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbLatinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbLatinCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--Caps
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_CAPS;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_CAP_L;
    vkb_ptr->caps_key = vkb_ptr->keys + key_idx;    //�ر�ļ�סcaps key
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_EN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;    
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }
    
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbLatinSysUp;
    }

    //--
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
        
    //candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbLatinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //set cap, always init as lower
    if (data_ptr->vkb.cap != CAPITTAL_MODE_LOWER)
    {
        CAPITTAL_MODE_T back_cap = data_ptr->vkb.cap;

        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        SetVkbCaps(data_ptr, back_cap);
    }

	{
		//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);		//�����SetVkbCaps�Ѿ����ù��ú���
	}

    return;
}


/*==============================================================================
Description: Ĵָ����En���볤�� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbLatinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    if (PNULL == key_ptr || ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->input_string_len > 0)
    {
        return TRUE;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        CommitChar(data_ptr, '1' + (key_ptr->code - VKEY_1));
    }
    else if (VKEY_0 == key_ptr->code)
    {
        CommitChar(data_ptr, '0');
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����En���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbLatinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_LATIN_T* data_latin_ptr = PNULL;
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    data_latin_ptr = ((KERNEL_DATA_LATIN_T*)(data_ptr->data));
    //SCI_ASSERT(data_latin_ptr->input_string_len <= KMX_MAX_LATIN_WORD_LENGTH); /*assert verified*/
    if (PNULL == key_ptr || data_latin_ptr->input_string_len > KMX_MAX_LATIN_WORD_LENGTH)
    {
        return TRUE;
    }
    
    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        if (data_latin_ptr->input_string_len < KMX_MAX_LATIN_WORD_LENGTH)
        {
            data_latin_ptr->input_string[data_latin_ptr->input_string_len++] = '1' + (key_ptr->code - VKEY_1);
            data_latin_ptr->cur_page = 0;
            ThumbLatinUpdate(data_ptr);
        }
    }
    else if (VKEY_0 == key_ptr->code)
    {
        if (data_latin_ptr->input_string_len > 0)
        {
            //commit candidate first
            if (data_latin_ptr->cand_cnt > 0)
            {
                CapsCommitStr(data_ptr, data_latin_ptr->cand[0].string, data_latin_ptr->cand[0].length);
                UpdateUdbCache(&data_latin_ptr->engine, 
                    data_latin_ptr->cand[0].string, 
                    data_latin_ptr->cand[0].length, 
                    GUIIM_M_TP_ENGLISH);
            }

            //clear input
            ThumbLatinClear(data_ptr);
        }

        //commit a space
        CommitChar (data_ptr, ' ');
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����En���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbLatinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (((KERNEL_DATA_LATIN_T*)(data_ptr->data))->input_string_len > 0)
        {
            ThumbLatinClear(data_ptr);
        }
        else
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        break;

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����En���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbLatinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_LATIN_T* data_latin_ptr = PNULL;
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    data_latin_ptr = ((KERNEL_DATA_LATIN_T*)(data_ptr->data));
    switch (key_ptr->code)
    {
    case VKEY_CAPS:
        SetVkbNextCaps(data_ptr);
        ThumbLatinSetMatch(data_ptr);

        if (data_latin_ptr->input_string_len > 0)
        {
            ThumbLatinUpdate(data_ptr);
        }

        break;

    case VKEY_DEL:

        if (data_latin_ptr->input_string_len > 1)
        {
            data_latin_ptr->input_string_len--;
            data_latin_ptr->cur_page = 0;
            ThumbLatinUpdate(data_ptr);
        }
        else if (data_latin_ptr->input_string_len == 1)
        {
            ThumbLatinClear(data_ptr);
        }
        else
        {
            CommitChar (data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:

        if (data_latin_ptr->input_string_len > 0)
        {
            wchar buffer[KMX_MAX_LATIN_WORD_LENGTH] = {0};
            int32 buffer_len = 0;

            while (buffer_len < data_latin_ptr->input_string_len)
            {
                buffer[buffer_len] = data_latin_ptr->input_string[buffer_len];
                buffer_len++;
            }

            CapsCommitStr (data_ptr, buffer, buffer_len);
            UpdateUdbCache(&data_latin_ptr->engine, buffer, buffer_len, GUIIM_M_TP_ENGLISH);
            ThumbLatinClear(data_ptr);
        }
        else
        {
            CommitChar (data_ptr, '\n');
        }

        break;

    case VKEY_PAGEUP:
        //SCI_ASSERT(data_latin_ptr->cur_page > 0); /*assert verified*/
        if (data_latin_ptr->cur_page > 0)
        {
            data_latin_ptr->cur_page--;
        }
        ThumbLatinUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        //SCI_ASSERT(1 == data_latin_ptr->has_next_page); /*assert verified*/
        if (1 != data_latin_ptr->has_next_page)
        {
            break;
        }
        data_latin_ptr->page[data_latin_ptr->cur_page + 1]
        = data_latin_ptr->page[data_latin_ptr->cur_page]
          + data_latin_ptr->cand_cnt;
        data_latin_ptr->cur_page++;
        ThumbLatinUpdate(data_ptr);
        break;

    case VKEY_123:
        if (data_latin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        if (data_latin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        if (data_latin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;
        
    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����En���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbLatinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand_cnt);
        ThumbLatinSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����En���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbLatinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_LATIN_T * data_latin_ptr = PNULL;
    int32 select_idx = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }

    data_latin_ptr = ((KERNEL_DATA_LATIN_T*)(data_ptr->data));
    select_idx = key_ptr->code - VKEY_CANDIDATE;

    //SCI_ASSERT(select_idx >= 0); /*assert verified*/
    //SCI_ASSERT(select_idx < ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand_cnt); /*assert verified*/
    if (select_idx < 0 || select_idx >= data_latin_ptr->cand_cnt)
    {
        return TRUE;
    }
    
    //commit candidate
    CapsCommitStr(data_ptr, data_latin_ptr->cand[select_idx].string, data_latin_ptr->cand[select_idx].length);
    UpdateUdbCache(&data_latin_ptr->engine, data_latin_ptr->cand[select_idx].string, data_latin_ptr->cand[select_idx].length, GUIIM_M_TP_ENGLISH);

    //clear input
    ThumbLatinClear(data_ptr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbLatinSetMatch
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbLatinSetMatch(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    switch (data_ptr->vkb.cap)
    {
    case CAPITTAL_MODE_INITIAL:
        ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->match_flags = KMX_LATIN_INITIAL_CASE;
        break;

    case CAPITTAL_MODE_UPPER:
        ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->match_flags = KMX_LATIN_UPPER_CASE;
        break;

        //case CAPITTAL_MODE_LOWER:
    default:
        ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->match_flags = KMX_LATIN_LOWER_CASE;
        break;
    }

    return;
}


/*==============================================================================
Description: ThumbLatinClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbLatinClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    SCI_MEMSET((char*)(data_ptr->data) + sizeof(((KERNEL_DATA_LATIN_T*)(data_ptr->data))->engine), 0,
               sizeof(KERNEL_DATA_LATIN_T) - sizeof(((KERNEL_DATA_LATIN_T*)(data_ptr->data))->engine));

    ThumbLatinSetMatch(data_ptr);

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand_cnt);
    ThumbLatinSetCand(data_ptr);
    ThumbLatinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbLatinUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbLatinUpdate(IM_DATA_T *data_ptr)
{
    KMX_LATIN_MATCH_RESULT match_result = {0};
    uint16 match_index = 0;
    KERNEL_DATA_LATIN_T* data_latin_ptr = PNULL;
    CHECK_DATA_EX(data_ptr);

    /* search */
    data_latin_ptr = ((KERNEL_DATA_LATIN_T*)(data_ptr->data));
    data_latin_ptr->cand_cnt = 0;
    data_latin_ptr->has_next_page = 0;
    match_index = data_latin_ptr->page[data_latin_ptr->cur_page];

    do
    {
        int32 search_res = 0;
        int32 left_cnt = LATIN_CANDIDATE_COUNT - data_latin_ptr->cand_cnt;
        uint8 copy_cnt = 0;

        search_res = kmx_latin_match(&(data_latin_ptr->engine), PNULL,
                                     data_latin_ptr->input_string, data_latin_ptr->input_string_len,
                                     data_latin_ptr->match_flags,
                                     match_index,
                                     &match_result);

        if (match_result.count < left_cnt)
        {
            copy_cnt = match_result.count;
        }
        else if (match_result.count == left_cnt)
        {
            copy_cnt = left_cnt;

            if (match_result.more_available != 0)
            {
                match_result.more_available = 0;
                data_latin_ptr->has_next_page = 1;
            }
        }
        else
        {
            copy_cnt = left_cnt;
            match_result.more_available = 0;
            data_latin_ptr->has_next_page = 1;
        }

        if (0 == search_res || 0 == copy_cnt)
        {
            wchar buffer[KMX_MAX_LATIN_WORD_LENGTH] = {0};
            int32 buffer_len = 0;

            while (buffer_len < data_latin_ptr->input_string_len)
            {
                buffer[buffer_len] = data_latin_ptr->input_string[buffer_len];
                buffer_len++;
            }

            CapsCommitStr(data_ptr, buffer, buffer_len);
            UpdateUdbCache(&data_latin_ptr->engine, buffer, buffer_len, GUIIM_M_TP_ENGLISH);

            ThumbLatinClear(data_ptr);
        }

        SCI_MEMCPY(data_latin_ptr->cand + data_latin_ptr->cand_cnt,
                   match_result.table,
                   copy_cnt * sizeof(KMX_LATIN_CANDIDATE));

        data_latin_ptr->cand_cnt += copy_cnt;
        match_index += match_result.count;
    }
    while (match_result.more_available != 0);

    SetHideKey(data_ptr, data_latin_ptr->cand_cnt);
    ThumbLatinSetCand(data_ptr);
    ThumbLatinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbLatinSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbLatinSetCand(IM_DATA_T *data_ptr)
{
    int8 cand_layer = 0;
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    GUI_RECT_T cand_rect = {0};
    int16 cand_height = 0;
    uint8 line_idx = 0;
    int16 panel_width = 0, panel_height = 0, dw = 0, dh = 0;

    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}

    cand_key_idx = data_ptr->vkb.candidate_first;
	
    if (0 == ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand_cnt)
    {
        goto ThumbLatinSetCandEnd;
    }

    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;
    dw = 3;         ///2 * data_ptr->theme.key_hor_space * panel_width / 1000;
    dh = 6;     ///2 * data_ptr->theme.key_hor_space * panel_height / 1000;

    //set normal candidate
    cand_rect = data_ptr->vkb.vessel_key->rect;
    cand_height = cand_rect.bottom - cand_rect.top - dh - 1;    //cand ���� ����û��ȥ��space���
    cand_rect.left += dw;
    cand_rect.top += 1 + dh / 2;    //������һ������
    cand_rect.bottom -= dh / 2;

    while (cand_cnt < ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        int16 word_width = 0;
        int16 disp_right = 0;

        word_width = GUI_GetStringWidth(data_ptr->theme.key_cand[0].text_font,
                                        ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand[cand_cnt].string, ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand[cand_cnt].length);
        word_width += 2 * data_ptr->theme.key_padding;

        if (word_width < CAND_WIDTH_MIN) word_width = CAND_WIDTH_MIN;

        disp_right = cand_rect.left + word_width - 1;

        if (disp_right > cand_rect.right && line_idx > 0)
        {
            if (0 == data_ptr->vkb.is_expand) break;

            if (++cand_layer >= LATIN_CANDIDATE_ROW) break;

            cand_rect.left = data_ptr->vkb.vessel_key->rect.left + dw / 2;
            cand_rect.right = data_ptr->vkb.vessel_key->rect.right - dw / 2;
            cand_rect.top += cand_height + dh;
            cand_rect.bottom = cand_rect.top + cand_height;

            line_idx = 0;

            disp_right = cand_rect.left + word_width - 1;
        }

        line_idx++;

        data_ptr->vkb.keys[cand_key_idx].rect = cand_rect;
        data_ptr->vkb.keys[cand_key_idx].rect.right = disp_right;

        if (data_ptr->vkb.keys[cand_key_idx].rect.right > cand_rect.right)
        {
            data_ptr->vkb.keys[cand_key_idx].rect.right = cand_rect.right;
        }

        data_ptr->vkb.keys[cand_key_idx].text_len = MIN (((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand[cand_cnt].length, TEXT_LEN_LIMIT);
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].text));		//����գ������´�����ʱ����������bug����Ȼ���뵽�༭��û�����⣬������ʾ������
        SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].text, ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand[cand_cnt].string,
                   data_ptr->vkb.keys[cand_key_idx].text_len * sizeof(wchar));
#if 0//to delete assert
		{
			SCI_ASSERT (((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand[cand_cnt].length <= TEXT_LEN_LIMIT);  //just test whether the IME_engine has error.
		}
#endif
		
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].print_text, 0, sizeof(data_ptr->vkb.keys[cand_key_idx].print_text));
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
			
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
		
        cand_rect.left = disp_right + 1 + dw;
        cand_key_idx++;
        cand_cnt++;
    }

    if (1 == data_ptr->vkb.is_expand)
    {
        if (((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand_cnt > cand_cnt)
        {
            ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand_cnt = cand_cnt;
            ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->has_next_page = 1;
        }

        if (((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);

        if (1 == ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
    }

ThumbLatinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
		
		//clear the text, maybe next time input cause any problem.
		data_ptr->vkb.keys[cand_key_idx].text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].text));  
		data_ptr->vkb.keys[cand_key_idx].print_text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].print_text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].print_text));
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbLatinDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[KMX_MAX_LATIN_WORD_LENGTH] = {0};
    uint32 buffer_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};

    CHECK_DATA_EX(data_ptr);

    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/
    if (!has_edit_layer)
    {
        return;
    }
    
    UILAYER_Clear(&data_ptr->edit_layer);

    if (((KERNEL_DATA_LATIN_T*)(data_ptr->data))->input_string_len == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    while (buffer_len < ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->input_string_len)
    {
        buffer[buffer_len] = ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->input_string[buffer_len];
        buffer_len++;
    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len > edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


#if defined (IM_SIMP_CHINESE_SUPPORT)
/*==============================================================================
Description: ThumbPinyinInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinInit(IM_DATA_T *data_ptr)
{
    int32 init_res = 0;
    KMX_IME_ENGINE_INIT init_info = {0};

    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_PINYIN_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_PINYIN_T));

    ThumbPinyinInitKb(data_ptr);

    init_info.database = GetKmxLdb_ptr(GUIIM_LANG_CHS, FALSE);
    init_info.flags = GetPyFuzzy();
    MMIIM_IME_FindUserCacheByIM(
        data_ptr->init_param.method_param_ptr->im_def_ptr->method,
        &init_info.user_data, &init_info.user_data_size);

    init_res = kmx_pinyin_initialize(&(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->engine), &init_info);
    MMIIM_SetImKerState(0);
    if (0 == init_res)
    {
        return FALSE;
    }

    //SCI_ASSERT(*(uint32*)((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->engine.engine == 0x00000001);  /*assert verified*/

    return TRUE;
}


/*==============================================================================
Description: ThumbPinyinTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: ThumbAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    ThumbPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void ThumbPinyinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 9;
    uint8 const SYS_CNT = 4;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT;
    uint8 const PY_CAND_CNT = 4;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    int32 py_cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMI_STRING_T slide_cue = {PNULL, 0};
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += PY_CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].text[0] = g_thumb_cust_cn_symb[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCustUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_EXCLUDE_DIGIT;
        //=========��������˿ӡ����
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbPinyinCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //--1 �ر��趨
    vkb_ptr->keys[char_begin].code = VKEY_PY_SEPARATE;
    vkb_ptr->keys[char_begin].text_len = 1;
    SCI_MEMCPY(vkb_ptr->keys[char_begin].text, L"'", vkb_ptr->keys[char_begin].text_len*sizeof(wchar));
    vkb_ptr->keys[char_begin].f_up = ThumbPinyinCtrlUp;
    vkb_ptr->keys[char_begin].f_long = ThumbPinyinCharLong;     //������Ϣû�д���
    //--0 ���һ��char
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE;

    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbPinyinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--FACE
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_FACE;
    vkb_ptr->keys[key_idx].text_len = 2;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L":)", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));    
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_PINYIN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;    
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinSysUp;
    }

    //--hide
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--expand
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;

    //--expand py pgup - reset rect
    key_idx++;
    vkb_ptr->py_pgup_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[26]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_PY_PAGEUP;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;

    //--expand py pgdn - reset rect
    key_idx++;
    vkb_ptr->py_pgdn_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[27]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_PY_PAGEDOWN;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;


    //--expand py panel
    key_idx++;
    vkb_ptr->py_vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[25]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
    
    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_thumb_panel_position[0].right - g_thumb_panel_position[0].left - 5;         //�Ҳ���һ������
        int16 h = g_thumb_panel_position[0].bottom - g_thumb_panel_position[0].top - 2 * data_ptr->theme.key_ver_space - 5; //������Щ�հ�,������һ������
        GUI_RECT_T cand_rect = {0};

        //ƫ����ֱ�ӷ���ConvertRect������㣬�����ڶ�����ƫ�������Ǿ��ȵ�
        cand_rect.top = g_thumb_panel_position[0].top + y * (h + 2 * data_ptr->theme.key_ver_space) + data_ptr->theme.key_ver_space + 5;
        cand_rect.bottom = cand_rect.top + h;
        cand_rect.left = g_thumb_panel_position[0].left + x * w / CH_CANDIDATE_COL + 1;
        cand_rect.right = cand_rect.left + w / CH_CANDIDATE_COL;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, 0);

        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //pinyin candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    py_cand_begin = key_idx;

    for (; key_idx < py_cand_begin + PY_CAND_CNT; key_idx++)
    {
        int16 w = g_thumb_panel_position[25].right - g_thumb_panel_position[25].left + 1;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = g_thumb_panel_position[25].top;
        cand_rect.bottom = g_thumb_panel_position[25].bottom;
        cand_rect.left = g_thumb_panel_position[25].left + (key_idx - py_cand_begin) * w / PY_CAND_CNT;
        cand_rect.right = g_thumb_panel_position[25].left + (key_idx - py_cand_begin + 1) * w / PY_CAND_CNT - 1;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].code = VKEY_PY_CANDIDATE + key_idx - py_cand_begin;
        vkb_ptr->keys[key_idx].tag =  KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbPinyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->py_candidate_first = py_cand_begin;
    vkb_ptr->py_candidate_cnt = PY_CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*==============================================================================
Description: Ĵָ����Pinyin���볤�� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return  TRUE;
    }
    
    if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len > 0)
    {
        return TRUE;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        CommitChar(data_ptr, '1' + (key_ptr->code - VKEY_1));
    }
    else if (VKEY_0 == key_ptr->code)
    {
        CommitChar(data_ptr, '0');
    }
    else if (VKEY_PY_SEPARATE == key_ptr->code)     // 1������Ϊ�ָ���
    {
        CommitChar(data_ptr, '1');
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
	KERNEL_DATA_PINYIN_T* data_pinyin_ptr = PNULL;

    CHECK_DATA_EX(data_ptr);
	data_pinyin_ptr = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data));

    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    //SCI_ASSERT(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len <= KMX_MAX_PINYIN_INPUT); /*assert verified*/
    if (PNULL == key_ptr || data_pinyin_ptr->input_string_len > KMX_MAX_PINYIN_INPUT)
    {
        return  TRUE;
    }
    
    if (VKEY_2 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        if (data_pinyin_ptr->input_string_len < KMX_MAX_PINYIN_INPUT
                && data_pinyin_ptr->confirm_cnt < PINYIN_CONFIRM_LIMIT)
        {
            data_pinyin_ptr->input_string[data_pinyin_ptr->input_string_len++] = '2' + (key_ptr->code - VKEY_2);
            data_pinyin_ptr->pinyin_match_index = KMX_PINYIN_MATCH_INDEX_NONE;
            data_pinyin_ptr->cur_page = 0;
            ThumbPinyinUpdate(data_ptr);
        }
    }
    else if (VKEY_0 == key_ptr->code)
    {
        if (data_pinyin_ptr->input_string_len == 0)
        {
            //commit a space
            CommitChar(data_ptr, 0x3000);
        }
		else
		{
			//commit candidate first
			if (data_pinyin_ptr->cand_cnt > 0)
			{
				CapsCommitStr(data_ptr, data_pinyin_ptr->cand[0].string, data_pinyin_ptr->cand[0].length);
				UpdateUdbCache(&data_pinyin_ptr->engine, 
					data_pinyin_ptr->cand[0].string, 
					data_pinyin_ptr->cand[0].length, 
					GUIIM_M_TP_PINYIN);				
			}
			
			//clear input
			ThumbPinyinClear(data_ptr);
		}
    }
    else if (VKEY_1 == key_ptr->code)
    {
        if (data_pinyin_ptr->input_string_len == 0)
        {
            //open face symbol panel
            SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_FACE);
        }
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len > 0)
        {
            ThumbPinyinClear(data_ptr);
        }
        else
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        break;

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_PINYIN_T* data_pinyin_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }

    data_pinyin_ptr = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data));
    switch (key_ptr->code)
    {
    case VKEY_PY_SEPARATE:

        if (0 < data_pinyin_ptr->input_string_len
                && data_pinyin_ptr->input_string_len < KMX_MAX_PINYIN_INPUT
                && data_pinyin_ptr->input_string[data_pinyin_ptr->input_string_len-1] != KMX_PINYIN_SEPARATOR)
        {
            data_pinyin_ptr->input_string[data_pinyin_ptr->input_string_len++] = KMX_PINYIN_SEPARATOR;
            ThumbPinyinDispEdit(data_ptr);
        }

        break;

    case VKEY_DEL:

        if (data_pinyin_ptr->confirm_cnt > 0)
        {
            data_pinyin_ptr->confirm_cnt--;
            data_pinyin_ptr->pinyin_match_index = KMX_PINYIN_MATCH_INDEX_NONE;
            data_pinyin_ptr->cur_page = 0;
            ThumbPinyinUpdate(data_ptr);
        }
        else if (data_pinyin_ptr->input_string_len > 1)
        {
            data_pinyin_ptr->input_string_len--;
            data_pinyin_ptr->pinyin_match_index = KMX_PINYIN_MATCH_INDEX_NONE;
            data_pinyin_ptr->cur_page = 0;
            ThumbPinyinUpdate(data_ptr);
        }
        else if (data_pinyin_ptr->input_string_len == 1)
        {
            ThumbPinyinClear(data_ptr);
        }
        else if (data_pinyin_ptr->cand_cnt > 0)
        {
            ThumbPinyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:

        if (data_pinyin_ptr->input_string_len > 0)
        {
            wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
            int32 buffer_len = 0;

            while (buffer_len < data_pinyin_ptr->input_string_len)
            {
                buffer[buffer_len] = data_pinyin_ptr->input_string[buffer_len];
                buffer_len++;
            }

            CommitStr(data_ptr, buffer, buffer_len);
            UpdateUdbCache(&data_pinyin_ptr->engine, buffer, buffer_len, GUIIM_M_TP_PINYIN);
            ThumbPinyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\n');
        }

        break;

    case VKEY_PAGEUP:
        //SCI_ASSERT(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cur_page > 0); /*assert verified*/
        if (data_pinyin_ptr->cur_page == 0)
        {
            break;
        }
        data_pinyin_ptr->cur_page--;

        if (data_pinyin_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            ThumbPinyinUpdate(data_ptr);
        }
        else if (data_pinyin_ptr->edit_state == EDIT_STATE_ASSO)
        {
            ThumbPinyinAsso(data_ptr);
        }

        break;

    case VKEY_PAGEDOWN:
        //SCI_ASSERT(1 == ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->has_next_page); /*assert verified*/
        if (1 != data_pinyin_ptr->has_next_page)
        {
            break;
        }
        data_pinyin_ptr->cur_page++;

        if (data_pinyin_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            ThumbPinyinUpdate(data_ptr);
        }
        else if (data_pinyin_ptr->edit_state == EDIT_STATE_ASSO)
        {
            ThumbPinyinAsso(data_ptr);
        }

        break;

    case VKEY_123SYMBOL:
        if (data_pinyin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;
 
    case VKEY_FACE:
        if (data_pinyin_ptr->input_string_len > 0) 
            break;
        //open face symbol panel 
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_FACE);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - custom��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN has_turn_page = FALSE;
    KERNEL_DATA_PINYIN_T* data_pinyin_ptr = PNULL;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return  TRUE;
    }

    data_pinyin_ptr = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data));
    if (KEY_STYLE_CUSTOM == data_ptr->vkb.first_key->style)
    {
        if (data_ptr->vkb.first_key->code > key_ptr->code)
        {
            if (data_pinyin_ptr->pinyin_match_result.count > data_pinyin_ptr->cust_pinyin_first + data_ptr->vkb.custom_cnt)
            {
                uint8 slide_cnt = data_ptr->vkb.first_key->code - key_ptr->code;
                uint8 next_page_cnt = data_pinyin_ptr->pinyin_match_result.count;

                next_page_cnt -= data_pinyin_ptr->cust_pinyin_first + data_ptr->vkb.custom_cnt;

                if (slide_cnt > next_page_cnt) slide_cnt = next_page_cnt;

                data_pinyin_ptr->cust_pinyin_first += slide_cnt;
                ThumbPinyinSetCust(data_ptr);
            }

            has_turn_page = TRUE;
        }
        else if (data_ptr->vkb.first_key->code < key_ptr->code)
        {
            if (data_pinyin_ptr->cust_pinyin_first > 0)
            {
                uint8 slide_cnt = key_ptr->code - data_ptr->vkb.first_key->code;
                uint8 next_page_cnt = data_pinyin_ptr->cust_pinyin_first;

                if (slide_cnt > next_page_cnt) slide_cnt = next_page_cnt;

                data_pinyin_ptr->cust_pinyin_first -= slide_cnt;
                ThumbPinyinSetCust(data_ptr);
            }

            has_turn_page = TRUE;
        }
    }

    if (!has_turn_page)
    {
        if (data_pinyin_ptr->input_string_len > 0)
        {
            uint16 select_idx = data_pinyin_ptr->cust_pinyin_first;

            select_idx += key_ptr->code - VKEY_CUSTOM;

            if (select_idx < data_pinyin_ptr->pinyin_match_result.count
                    && data_pinyin_ptr->pinyin_match_index != select_idx)
            {
                data_pinyin_ptr->pinyin_match_index = select_idx;
                data_pinyin_ptr->cur_page = 0;
                ThumbPinyinUpdate(data_ptr);
            }
        }
        else
        {
            CommitChar(data_ptr, key_ptr->text[0]);
            UpdateUdbCache(&data_pinyin_ptr->engine, &(key_ptr->text[0]), 1, GUIIM_M_TP_PINYIN);

			//��text�ֶ�����ͬ����display_text�ֶ�
			key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));			
        }
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return  TRUE;
    }
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand_cnt);
        ThumbPinyinSetCand(data_ptr);
        break;

    case VKEY_PY_PAGEUP:

        if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->expand_pinyin_page > 0)
        {
            ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->expand_pinyin_page--;
            ThumbPinyinSetPyCand(data_ptr);
        }

        break;

    case VKEY_PY_PAGEDOWN:

        if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result.count > ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->expand_pinyin_page * data_ptr->vkb.py_candidate_cnt)
        {
            ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->expand_pinyin_page++;
            ThumbPinyinSetPyCand(data_ptr);
        }

        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN ThumbPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_PINYIN_T* data_pinyin_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }
    
    data_pinyin_ptr = (KERNEL_DATA_PINYIN_T*)(data_ptr->data);
    if (VKEY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_CANDIDATE + data_ptr->vkb.candidate_cnt)
    {
        int32 select_idx = key_ptr->code - VKEY_CANDIDATE;

        if (data_pinyin_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            uint8 confirm_digit_len = 0;
            uint16 i = 0;
            wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
            uint32 buffer_len = 0;

            data_pinyin_ptr->confirm[data_pinyin_ptr->confirm_cnt++] = data_pinyin_ptr->cand[select_idx];

            //���������ѡ��֮���Ƿָ�������Ҫ��comfirm len���Ϸָ����ĳ��ȣ���1
            //ʵ���ϣ�ֻ�����һ���ָ����ᱻ���ԣ��м�ķָ�����������ƥ�䳤����
            for (i = 0; i < data_pinyin_ptr->confirm_cnt; i++)
            {
                confirm_digit_len += data_pinyin_ptr->confirm[i].match_length;

                if (KMX_PINYIN_SEPARATOR == data_pinyin_ptr->input_string[confirm_digit_len]
                        && confirm_digit_len < data_pinyin_ptr->input_string_len)
                {
                    confirm_digit_len++;
                }
            }


            //SCI_ASSERT(confirm_digit_len <= ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/
            if (confirm_digit_len > data_pinyin_ptr->input_string_len)
            {
                return TRUE;
            }
            if (confirm_digit_len < data_pinyin_ptr->input_string_len)
            {
                data_pinyin_ptr->cur_page = 0;
                data_pinyin_ptr->pinyin_match_index = KMX_PINYIN_MATCH_INDEX_NONE;
                ThumbPinyinUpdate(data_ptr);
            }
            else
            {
                //all match, commit
                for (i = 0; i < data_pinyin_ptr->confirm_cnt; i++)
                {
                    SCI_MEMCPY(buffer + buffer_len, data_pinyin_ptr->confirm[i].string,
                               data_pinyin_ptr->confirm[i].length * sizeof(wchar));
                    buffer_len += data_pinyin_ptr->confirm[i].length;
                }

                //SCI_ASSERT(buffer_len > 0); /*assert verified*/

                CommitStr(data_ptr, buffer, buffer_len);
                UpdateUdbCache(&data_pinyin_ptr->engine, buffer, buffer_len, GUIIM_M_TP_PINYIN);
                ThumbPinyinClear(data_ptr);
                if (buffer_len < KMX_MAX_PINYIN_INPUT - 1)
                {
                    data_pinyin_ptr->last_char = buffer[buffer_len - 1];                
                }
                ThumbPinyinAsso(data_ptr);
            }
        }
        else if (data_pinyin_ptr->edit_state == EDIT_STATE_ASSO)
        {
            wchar last_char = data_pinyin_ptr->cand[select_idx].string[0];

            CommitChar(data_ptr, last_char);            
            UpdateUdbCache(&data_pinyin_ptr->engine, &last_char, 1, GUIIM_M_TP_PINYIN);
            ThumbPinyinClear(data_ptr);
            data_pinyin_ptr->last_char = last_char;
            ThumbPinyinAsso(data_ptr);
        }

    }
    else if (VKEY_PY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_PY_CANDIDATE + data_ptr->vkb.py_candidate_cnt)
    {
        uint16 select_idx = data_pinyin_ptr->expand_pinyin_page * data_ptr->vkb.py_candidate_cnt;

        //SCI_ASSERT(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->edit_state == EDIT_STATE_SEARCH); /*assert verified*/
        if (data_pinyin_ptr->edit_state != EDIT_STATE_SEARCH)
        {
            return TRUE;
        }
        select_idx += key_ptr->code - VKEY_PY_CANDIDATE;

        if (select_idx < data_pinyin_ptr->pinyin_match_result.count
                && data_pinyin_ptr->pinyin_match_index != select_idx)
        {
            data_pinyin_ptr->pinyin_match_index = select_idx;
            data_pinyin_ptr->cur_page = 0;
            ThumbPinyinUpdate(data_ptr);
        }
    }
    else
    {
        //SCI_ASSERT(0); /*assert verified*/
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbPinyinClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    SCI_MEMSET((char*)(data_ptr->data) + sizeof(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->engine), 0,
               sizeof(KERNEL_DATA_PINYIN_T) - sizeof(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->engine));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand_cnt);
    ThumbPinyinSetCand(data_ptr);
    ThumbPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbPinyinUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinUpdate(IM_DATA_T *data_ptr)
{
    uint8 confirm_digit_len = 0;
    uint16 i = 0;
    KMX_PINYIN_WORD_MATCH_RESULT match_result = {0};
    uint16 word_match_index = 0;
    uint8 cand_cnt = 0;
    uint8 has_next_page = 0;

    CHECK_DATA_EX(data_ptr);

    ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->edit_state = EDIT_STATE_SEARCH;

    /* search */
    ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cust_pinyin_first = 0;

    for (i = 0; i < ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->confirm_cnt; i++)
    {
        confirm_digit_len += ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->confirm[i].match_length;
    }

    //SCI_ASSERT(confirm_digit_len < ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/
    if (confirm_digit_len >= ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len)
    {
        return;
    }
    
    word_match_index = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cur_page * data_ptr->vkb.candidate_cnt;

    do
    {
        int32 search_res = 0;
        int32 left_cnt = data_ptr->vkb.candidate_cnt - cand_cnt;
        uint8 copy_cnt = 0;

        search_res = kmx_digit_pinyin_match(&(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->engine),
                                            ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string + confirm_digit_len,
                                            ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len - confirm_digit_len,
                                            ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_index,
                                            word_match_index,
                                            &(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_composition),
                                            &(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result),
                                            &match_result);

        if (match_result.count < left_cnt)
        {
            copy_cnt = match_result.count;
        }
        else if (match_result.count == left_cnt)
        {
            copy_cnt = left_cnt;

            if (match_result.more_available != 0)
            {
                match_result.more_available = 0;
                has_next_page = 1;
            }
        }
        else
        {
            copy_cnt = left_cnt;
            match_result.more_available = 0;
            has_next_page = 1;
        }

        if (0 == search_res || 0 == copy_cnt)
        {
            break;
        }

        SCI_MEMCPY(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand + cand_cnt,
                   match_result.table,
                   copy_cnt * sizeof(KMX_PINYIN_CANDIDATE));

        cand_cnt += copy_cnt;
        word_match_index += match_result.count;
    }
    while (match_result.more_available != 0);


    if (cand_cnt > 0)
    {
        ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand_cnt = cand_cnt;
        ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->has_next_page = has_next_page;

        SetHideKey(data_ptr, ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand_cnt);
        ThumbPinyinSetCand(data_ptr);
        ThumbPinyinDispEdit(data_ptr);
    }
    else
    {
        ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len--;
    }

    return;
}


/*==============================================================================
Description: ThumbPinyinSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;

    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}

    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand_cnt)
    {
        goto ThumbPinyinSetCandEnd;
    }
    
    //set normal candidate
    while (cand_cnt < ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        data_ptr->vkb.keys[cand_key_idx].text_len = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand[cand_cnt].length;
        SCI_MEMCPY(data_ptr->vkb.keys[cand_key_idx].text, ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand[cand_cnt].string,
                   ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand[cand_cnt].length * sizeof(wchar));
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
			
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
		
        cand_key_idx++;
        cand_cnt++;
    }


    if (1 == data_ptr->vkb.is_expand)
    {
        ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand_cnt = cand_cnt;

        if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);
		
        if (1 == ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
		
        ThumbPinyinSetPyCand(data_ptr);
    }

ThumbPinyinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
    }

    if (0 == data_ptr->vkb.is_expand)
    {
        ThumbPinyinSetCust(data_ptr);
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
    uint32 buffer_len = 0;
    int16 i = 0;
    uint8 confirm_digit_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};

    CHECK_DATA_EX(data_ptr);

    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/
    if (!has_edit_layer)
    {
        return;
    }
    
    UILAYER_Clear(&data_ptr->edit_layer);

    if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    //����
    for (i = 0; i < ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->confirm_cnt; i++)
    {
        SCI_MEMCPY(buffer + buffer_len, ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->confirm[i].string,
                   ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->confirm[i].length * sizeof(wchar));
        buffer_len += ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->confirm[i].length;
        confirm_digit_len += ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->confirm[i].match_length;
    }

    //ƴ��
    for (i = 0; i < ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_composition.length; i++)
    {
        buffer[buffer_len++] = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_composition.string[i];
    }

    //SCI_ASSERT(confirm_digit_len < ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/
    if (confirm_digit_len >= ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len)
    {
        return;
    }
    
    confirm_digit_len += ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_composition.length;

    //SCI_ASSERT(confirm_digit_len <= ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/
    if (confirm_digit_len > ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len)
    {
        return;
    }
    //����
    for (i = 0; i < ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len - confirm_digit_len; i++)
    {
        buffer[buffer_len++] = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string[confirm_digit_len + i];
    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len > edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


/*==============================================================================
Description: ThumbPinyinSetCust
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinSetCust(IM_DATA_T *data_ptr)
{
    uint8 key_idx = 0;

    CHECK_DATA_EX(data_ptr);

    if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len > 0)
    {
        uint8 pinyin_idx = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cust_pinyin_first;
        uint8 pinyin_left = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result.count - pinyin_idx;

        if (pinyin_left > data_ptr->vkb.custom_cnt)
        {
            pinyin_left = data_ptr->vkb.custom_cnt;
        }

        for (key_idx = 0; key_idx < pinyin_left; key_idx++)
        {
            int16 char_idx = 0;

            for (char_idx = 0; char_idx < ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result.table[pinyin_idx + key_idx].length; char_idx++)
            {
                data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text[char_idx]
                = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result.table[pinyin_idx + key_idx].string[char_idx];
            }

            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result.table[pinyin_idx + key_idx].length;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT- 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));			

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}

        for (; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = 0;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 1;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT- 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));			

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}
    }
    else
    {
        for (key_idx = 0; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text[0] = g_thumb_cust_cn_symb[key_idx];
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = 1;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT- 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));			

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}
    }

    return;
}


/*==============================================================================
Description: ThumbPinyinSetPyCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinSetPyCand(IM_DATA_T *data_ptr)
{
    uint8 key_idx = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.py_pgup_key); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->vkb.py_pgdn_key); /*assert verified*/
    if (PNULL == data_ptr->vkb.py_pgup_key ||PNULL == data_ptr->vkb.py_pgdn_key)
    {
        return;
    }
    
    if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->edit_state == EDIT_STATE_SEARCH)
    {
        uint8 pinyin_idx = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->expand_pinyin_page * data_ptr->vkb.py_candidate_cnt;
        uint8 pinyin_left = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result.count - pinyin_idx;

        //SCI_ASSERT(((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len > 0); /*assert verified*/
        if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string_len == 0)
        {
            return;
        }
        
        if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->expand_pinyin_page > 0)
        {
            data_ptr->vkb.py_pgup_key->is_inactive = 0;
            data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE;
        }
        else
        {
            data_ptr->vkb.py_pgup_key->is_inactive = 1;
            data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE_TINT;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgup_key, TRUE);

        if (pinyin_left > data_ptr->vkb.py_candidate_cnt)
        {
            pinyin_left = data_ptr->vkb.py_candidate_cnt;
            data_ptr->vkb.py_pgdn_key->is_inactive = 0;
            data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE;
        }
        else
        {
            data_ptr->vkb.py_pgdn_key->is_inactive = 1;
            data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE_TINT;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgdn_key, TRUE);

        for (key_idx = 0; key_idx < pinyin_left; key_idx++)
        {
            int16 char_idx = 0;

            for (char_idx = 0; char_idx < ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result.table[pinyin_idx + key_idx].length; char_idx++)
            {
                data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text[char_idx]
                = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result.table[pinyin_idx + key_idx].string[char_idx];
            }

            data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->pinyin_match_result.table[pinyin_idx + key_idx].length;
            data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].is_inactive = 0;

			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len * sizeof (wchar));			

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx], TRUE);
		}
    }
    else if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->edit_state == EDIT_STATE_ASSO)
    {
        data_ptr->vkb.py_pgup_key->is_inactive = 1;
        data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE_TINT;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgup_key, TRUE);
		
        data_ptr->vkb.py_pgdn_key->is_inactive = 1;
        data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE_TINT;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgdn_key, TRUE);
    }

    for (; key_idx < data_ptr->vkb.py_candidate_cnt; key_idx++)
    {
        data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len = 0;
        data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].is_inactive = 1;

		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len * sizeof (wchar));			

		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx], TRUE);
	}

    return;
}


/*==============================================================================
Description: ThumbPinyinAsso
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbPinyinAsso(IM_DATA_T *data_ptr)
{
    wchar buffer[CH_CANDIDATE_COUNT*ASSO_PAGE_LIMIT + 1] = {0};
    int32 offset_asso_cnt = 0;
    int32 asso_cnt = 0;
    int32 asso_idx = 0;

    CHECK_DATA_EX(data_ptr);

    ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->edit_state = EDIT_STATE_ASSO;

    offset_asso_cnt = ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cur_page * CH_CANDIDATE_COUNT;

    asso_cnt = kmx_chinese_query_association(GetKmxLdb_ptr(GUIIM_LANG_CHS, FALSE),
               ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->last_char, buffer,
               offset_asso_cnt + CH_CANDIDATE_COUNT + 1);

    if (asso_cnt <= offset_asso_cnt)
    {
        return;
    }

    ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->has_next_page = 0;

    if (asso_cnt > offset_asso_cnt + CH_CANDIDATE_COUNT)
    {
        if (((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cur_page + 1 < ASSO_PAGE_LIMIT)
        {
            ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->has_next_page = 1;
        }

        asso_cnt = CH_CANDIDATE_COUNT;
    }
    else
    {
        asso_cnt -= offset_asso_cnt;
    }

    for (asso_idx = 0; asso_idx < asso_cnt; asso_idx++)
    {
        ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand[asso_idx].length = 1;
        ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand[asso_idx].string[0] = buffer[offset_asso_cnt + asso_idx];
    }

    ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand_cnt = asso_cnt;

    SetHideKey(data_ptr, ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->cand_cnt);
    ThumbPinyinSetCand(data_ptr);

    return;
}
#endif

#if (defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT))
/*==============================================================================
Description: ThumbStrokeInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeInit(IM_DATA_T *data_ptr)
{
    int32 init_res = 0;
    KMX_IME_ENGINE_INIT init_info = {0};
	GUIIM_LANGUAGE_T im_lang = GUIIM_LANG_CHS;
    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_STROKE_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_STROKE_T));

    ThumbStrokeInitKb(data_ptr);

	if (data_ptr->init_param.method_param_ptr
		&& data_ptr->init_param.method_param_ptr->im_def_ptr)
	{
		im_lang = data_ptr->init_param.method_param_ptr->im_def_ptr->lang;
	}
	init_info.database = GetKmxLdb_ptr(im_lang, TRUE);
	
    MMIIM_IME_FindUserCacheByIM(
        data_ptr->init_param.method_param_ptr->im_def_ptr->method,
        &init_info.user_data, &init_info.user_data_size);

    init_res = kmx_stroke_initialize(&(((KERNEL_DATA_STROKE_T*)(data_ptr->data))->engine), &init_info);
    MMIIM_SetImKerState(0);
    if (0 == init_res)
    {
        return FALSE;
    }

    //SCI_ASSERT(*(uint32*)((KERNEL_DATA_STROKE_T*)(data_ptr->data))->engine.engine == 0x53535353); /*assert verified*/

    return TRUE;
}


/*==============================================================================
Description: ThumbStrokeTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: ThumbAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    ThumbStrokeDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void ThumbStrokeInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 7; //����0�����ո�
    uint8 const CTRL_CNT = 7;
    uint8 const SYS_CNT = 4;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT_E;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_stroke_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_stroke_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].text[0] = g_thumb_cust_cn_symb[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //character * 6
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_stroke_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].text[0] = g_stroke_symb[key_idx - char_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbStrokeCharUp;
    }

    key_idx--;

    //--0 ���һ��char
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].text_len = 0;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_CENTER;

    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_stroke_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbStrokeCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbStrokeCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_STROKE;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;    
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_stroke_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbStrokeSysUp;
    }

    //--hide
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--expand
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_stroke_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;

    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_stroke_panel_position[0].right - g_stroke_panel_position[0].left - 5;         //�Ҳ���һ������
        int16 h = g_stroke_panel_position[0].bottom - g_stroke_panel_position[0].top - 2 * data_ptr->theme.key_ver_space - 5; //������Щ�հ�,������һ������
        GUI_RECT_T cand_rect = {0};

        //ƫ����ֱ�ӷ���ConvertRect������㣬�����ڶ�����ƫ�������Ǿ��ȵ�
        cand_rect.top = g_stroke_panel_position[0].top + y * (h + 2 * data_ptr->theme.key_ver_space) + data_ptr->theme.key_ver_space + 5;
        cand_rect.bottom = cand_rect.top + h;
        cand_rect.left = g_stroke_panel_position[0].left + x * w / CH_CANDIDATE_COL + 1;
        cand_rect.right = cand_rect.left + w / CH_CANDIDATE_COL;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, 0);

        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbStrokeCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*==============================================================================
Description: Ĵָ����Stroke���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
	KERNEL_DATA_STROKE_T* data_stroke_ptr = PNULL;

    CHECK_DATA_EX(data_ptr);

    data_stroke_ptr = ((KERNEL_DATA_STROKE_T*)(data_ptr->data));
	if (PNULL == key_ptr || data_stroke_ptr->input_string_len > KMX_MAX_STROKE_INPUT_LENGTH)
    {
        return  TRUE;
    }
        
    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_6)
    {
        if (data_stroke_ptr->input_string_len < KMX_MAX_PINYIN_INPUT)
        {
            data_stroke_ptr->input_string[data_stroke_ptr->input_string_len++] = '1' + (key_ptr->code - VKEY_1);
            data_stroke_ptr->cur_page = 0;
            ThumbStrokeUpdate(data_ptr);
        }
    }
    else if (VKEY_SPACE == key_ptr->code)
    {
        if (data_stroke_ptr->input_string_len == 0)
        {
            //commit a space
            CommitChar(data_ptr, 0x3000);
        }
		else
		{
			if (data_stroke_ptr->cand_cnt > 0)
			{
				CapsCommitStr(data_ptr, data_stroke_ptr->cand[0].string, data_stroke_ptr->cand[0].length);
				//clear input
				ThumbStrokeClear(data_ptr);
			}     
		}
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Stroke���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (((KERNEL_DATA_STROKE_T*)(data_ptr->data))->input_string_len > 0)
        {
            ThumbStrokeClear(data_ptr);
        }
        else
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        break;

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Stroke���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_STROKE_T* data_stroke_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr )
    {
        return  TRUE;
    }

    data_stroke_ptr = ((KERNEL_DATA_STROKE_T*)(data_ptr->data));
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (data_stroke_ptr->input_string_len > 1)
        {
            data_stroke_ptr->input_string_len--;
            data_stroke_ptr->cur_page = 0;
            ThumbStrokeUpdate(data_ptr);
        }
        else if (data_stroke_ptr->input_string_len == 1)
        {
            ThumbStrokeClear(data_ptr);
        }
        else if (data_stroke_ptr->cand_cnt > 0)
        {
            ThumbStrokeClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:

        if (data_stroke_ptr->input_string_len > 0)
        {
            wchar buffer[KMX_MAX_STROKE_INPUT_LENGTH] = {0};
            int32 buffer_len = 0;

            while (buffer_len < data_stroke_ptr->input_string_len)
            {
                uint8 char_idx = data_stroke_ptr->input_string[buffer_len] - '1';

                //SCI_ASSERT(char_idx >= 0); /*assert verified*/
                //SCI_ASSERT(char_idx < 6); /*assert verified*/
                if(char_idx < 6)
                {
                    buffer[buffer_len++] = g_stroke_symb[char_idx];
                }
            }

            CommitStr(data_ptr, buffer, buffer_len);
            UpdateUdbCache(&data_stroke_ptr->engine, buffer, buffer_len, GUIIM_M_TP_STROKE);
            ThumbStrokeClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\n');
        }

        break;

    case VKEY_PAGEUP:
        //SCI_ASSERT(((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cur_page > 0); /*assert verified*/
        if(data_stroke_ptr->cur_page == 0)
        {
           break;
        }
        data_stroke_ptr->cur_page--;

        if (data_stroke_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            ThumbStrokeUpdate(data_ptr);
        }
        else if (data_stroke_ptr->edit_state == EDIT_STATE_ASSO)
        {
            ThumbStrokeAsso(data_ptr);
        }

        break;

    case VKEY_PAGEDOWN:
        //SCI_ASSERT(1 == ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->has_next_page); /*assert verified*/
        if(1 != data_stroke_ptr->has_next_page)
        {
           break;
        }
        data_stroke_ptr->cur_page++;

        if (data_stroke_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            ThumbStrokeUpdate(data_ptr);
        }
        else if (data_stroke_ptr->edit_state == EDIT_STATE_ASSO)
        {
            ThumbStrokeAsso(data_ptr);
        }

        break;

    case VKEY_123:
        if (data_stroke_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        if (data_stroke_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        if (data_stroke_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Stroke���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
       return TRUE;
    }
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt);
        ThumbStrokeSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Stroke���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN ThumbStrokeCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 select_idx = 0;
    wchar last_char = 0;
    KERNEL_DATA_STROKE_T* data_stroke_ptr = PNULL;
	GUIIM_LANGUAGE_T im_lang = GUIIM_LANG_CHS;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
       return TRUE;
    }

	if (data_ptr->init_param.method_param_ptr
		&& data_ptr->init_param.method_param_ptr->im_def_ptr)
	{
		im_lang = data_ptr->init_param.method_param_ptr->im_def_ptr->lang;
	}
    data_stroke_ptr = ((KERNEL_DATA_STROKE_T*)(data_ptr->data));
    select_idx = key_ptr->code - VKEY_CANDIDATE;

    //SCI_ASSERT(select_idx >= 0); /*assert verified*/
    //SCI_ASSERT(select_idx < ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt); /*assert verified*/
    if(select_idx >= 0 && select_idx < data_stroke_ptr->cand_cnt)
    {
        last_char = data_stroke_ptr->cand[select_idx].string[0];
    }
    
    CommitChar(data_ptr, last_char);

	if (im_lang == GUIIM_LANG_CHS)
    {
    	UpdateUdbCache(&data_stroke_ptr->engine, &last_char, 1, GUIIM_M_TP_STROKE);
	}
	else if (im_lang == GUIIM_LANG_CHT)
	{
    	UpdateUdbCache(&data_stroke_ptr->engine, &last_char, 1, GUIIM_M_TP_STROKE_CHT);
	}

    ThumbStrokeClear(data_ptr);
    data_stroke_ptr->last_char = last_char;
    ThumbStrokeAsso(data_ptr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbStrokeClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    SCI_MEMSET((char*)(data_ptr->data) + sizeof(((KERNEL_DATA_STROKE_T*)(data_ptr->data))->engine), 0,
               sizeof(KERNEL_DATA_STROKE_T) - sizeof(((KERNEL_DATA_STROKE_T*)(data_ptr->data))->engine));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt);
    ThumbStrokeSetCand(data_ptr);
    ThumbStrokeDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbStrokeUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeUpdate(IM_DATA_T *data_ptr)
{
    KMX_STROKE_MATCH_RESULT match_result = {0};
    uint16 word_match_index = 0;
    uint8 cand_cnt = 0;
    uint8 has_next_page = 0;

    CHECK_DATA_EX(data_ptr);

    ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->edit_state = EDIT_STATE_SEARCH;

    /* search */
    word_match_index = ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cur_page * data_ptr->vkb.candidate_cnt;

    do
    {
        int32 search_res = 0;
        int32 left_cnt = data_ptr->vkb.candidate_cnt - cand_cnt;
        uint8 copy_cnt = 0;

        search_res = kmx_stroke_match(&(((KERNEL_DATA_STROKE_T*)(data_ptr->data))->engine),
                                      ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->input_string,
                                      ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->input_string_len,
                                      word_match_index,
                                      &match_result);

        if (match_result.count < left_cnt)
        {
            copy_cnt = match_result.count;
        }
        else if (match_result.count == left_cnt)
        {
            copy_cnt = left_cnt;

            if (match_result.more_available != 0)
            {
                match_result.more_available = 0;
                has_next_page = 1;
            }
        }
        else
        {
            copy_cnt = left_cnt;
            match_result.more_available = 0;
            has_next_page = 1;
        }

        if (0 == search_res || 0 == copy_cnt)
        {
            break;
        }

        SCI_MEMCPY(((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand + cand_cnt,
                   match_result.table,
                   copy_cnt * sizeof(KMX_STROKE_CANDIDATE));

        cand_cnt += copy_cnt;
        word_match_index += match_result.count;
    }
    while (match_result.more_available != 0);

    if (cand_cnt > 0)
    {
        ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt = cand_cnt;
        ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->has_next_page = has_next_page;

        SetHideKey(data_ptr, ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt);
        ThumbStrokeSetCand(data_ptr);
        ThumbStrokeDispEdit(data_ptr);
    }
    else
    {
        //����������һ���ʻ���ʱ�򣬲ſ��ܳ����������������
        ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->input_string_len--;
    }

    return;
}


/*==============================================================================
Description: ThumbStrokeSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;

    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}

    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt)
    {
        goto ThumbStrokeSetCandEnd;
    }

    //set normal candidate
    while (cand_cnt < ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        data_ptr->vkb.keys[cand_key_idx].text_len = ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand[cand_cnt].length;
        SCI_MEMCPY(data_ptr->vkb.keys[cand_key_idx].text, ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand[cand_cnt].string,
                   ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand[cand_cnt].length * sizeof(wchar));
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
		
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
		
        cand_key_idx++;
        cand_cnt++;
    }


    if (1 == data_ptr->vkb.is_expand)
    {
        ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt = cand_cnt;

        if (((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);
		
        if (1 == ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
    }

ThumbStrokeSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
		
		//clear the text, maybe next time input cause any problem.
		data_ptr->vkb.keys[cand_key_idx].text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].text));  
		data_ptr->vkb.keys[cand_key_idx].print_text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].print_text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].print_text));
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[KMX_MAX_STROKE_INPUT_LENGTH] = {0};
    uint32 buffer_len = 0;
    int16 i = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};

    CHECK_DATA_EX(data_ptr);

    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/
    if(!has_edit_layer)
    {
       return;
    }
    
    UILAYER_Clear(&data_ptr->edit_layer);

    if (((KERNEL_DATA_STROKE_T*)(data_ptr->data))->input_string_len == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    //�ʻ�
    for (i = 0; i < ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->input_string_len; i++)
    {
        int32 char_idx = ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->input_string[i] - '1';

        if (0 <= char_idx && char_idx < 6)
        {
            buffer[buffer_len++] = g_stroke_symb[char_idx];
        }
        else
        {
            buffer[buffer_len++] = ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->input_string[i];
        }

    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len > edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


/*==============================================================================
Description: ThumbStrokeAsso
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbStrokeAsso(IM_DATA_T *data_ptr)
{
    wchar buffer[CH_CANDIDATE_COUNT_E*ASSO_PAGE_LIMIT + 1] = {0};
    int32 offset_asso_cnt = 0;
    int32 asso_cnt = 0;
    int32 asso_idx = 0;
	GUIIM_LANGUAGE_T im_lang = GUIIM_LANG_CHS;
	
    CHECK_DATA_EX(data_ptr);

    ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->edit_state = EDIT_STATE_ASSO;

    offset_asso_cnt = ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cur_page * CH_CANDIDATE_COUNT_E;

	if (data_ptr->init_param.method_param_ptr
		&& data_ptr->init_param.method_param_ptr->im_def_ptr)
	{
		im_lang = data_ptr->init_param.method_param_ptr->im_def_ptr->lang;
	}
	asso_cnt = kmx_chinese_query_association(GetKmxLdb_ptr(im_lang, FALSE),
               ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->last_char, buffer,
               offset_asso_cnt + CH_CANDIDATE_COUNT_E + 1);

    if (asso_cnt <= offset_asso_cnt)
    {
        return;
    }

    ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->has_next_page = 0;

    if (asso_cnt > offset_asso_cnt + CH_CANDIDATE_COUNT_E)
    {
        if (((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cur_page + 1 < ASSO_PAGE_LIMIT)
        {
            ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->has_next_page = 1;
        }

        asso_cnt = CH_CANDIDATE_COUNT_E;
    }
    else
    {
        asso_cnt -= offset_asso_cnt;
    }

    for (asso_idx = 0; asso_idx < asso_cnt; asso_idx++)
    {
        ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand[asso_idx].length = 1;
        ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand[asso_idx].string[0] = buffer[offset_asso_cnt + asso_idx];
    }

    ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt = asso_cnt;

    SetHideKey(data_ptr, ((KERNEL_DATA_STROKE_T*)(data_ptr->data))->cand_cnt);
    ThumbStrokeSetCand(data_ptr);

    return;
}
#endif



/*==============================================================================
Description: QwertLatinInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertLatinInit(IM_DATA_T *data_ptr)
{
    int32 init_res = 0;
    KMX_IME_ENGINE_INIT init_info = {0};

    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_Q_LATIN_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_Q_LATIN_T));

    QwertLatinInitKb(data_ptr);
    QwertLatinSetMatch(data_ptr);

    init_info.database = GetKmxLdb_ptr(GUIIM_LANG_ENGLISH, FALSE);
    MMIIM_IME_FindUserCacheByIM(
        data_ptr->init_param.method_param_ptr->im_def_ptr->method,
        &init_info.user_data, &init_info.user_data_size);

    init_res = kmx_latin_initialize(&(((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->engine), &init_info);
    MMIIM_SetImKerState(0);
    if (0 == init_res)
    {
        return FALSE;
    }

    //SCI_ASSERT(*(uint32*)((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->engine.engine == 0x4c4c4c4c); /*assert verified*/

    return TRUE;
}


/*==============================================================================
Description: QwertLatinTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertLatinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: QwertAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertLatinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void QwertLatinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 const CHAR_CNT = 26;
    uint8 const CTRL_CNT = 9;
    uint8 const CAND_CNT = LATIN_CANDIDATE_COUNT;
    uint8 const SYS_CNT = 2;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMI_STRING_T slide_cue = {PNULL, 0};

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_qwert_default;

    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_qwert_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 26
    //character key ��λ��������2��ʼ��27
    data_ptr->vkb.charactor_first = key_idx = 0;

    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].text[0] = "qwertyuiopasdfghjklzxcvbnm"[key_idx];
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertLatinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key ��λ��������28��ʼ������2��
    key_idx++;
    cust_begin = key_idx;

    for (; key_idx < cust_begin + CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].text[0] = ".,"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //deal tag
    if (GUIIM_TAG_EMAIL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '@';
    }
    else if (GUIIM_TAG_URL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '/';
    }

    //control * 6,  caps, del, 123symbol, space, set, enter
    //character key ��λ�������ӿ�ʼ��30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertLatinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = QwertLatinCtrlUp;
    }

    //--Caps
    key_idx = ctrl_begin;
    //�ر�ļ�סcaps key
    vkb_ptr->caps_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_CAPS;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_CAP_L;
    //--delete
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--space
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_Q;
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_EN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }
    
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertLatinSysUp;
    }

    //--
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;


    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_qwert_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
        
    //candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertLatinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //set cap, always init as lower
    if (data_ptr->vkb.cap != CAPITTAL_MODE_LOWER)
    {
        CAPITTAL_MODE_T back_cap = data_ptr->vkb.cap;

        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        SetVkbCaps(data_ptr, back_cap);
    }

	{
		//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);		//�����SetVkbCaps�Ѿ����ù��ú���
	}

    return;
}


/*==============================================================================
Description: En���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertLatinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
       return   TRUE;
    }
    
    //SCI_ASSERT(((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len <= KMX_MAX_LATIN_WORD_LENGTH); /*assert verified*/

    if (((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len < KMX_MAX_LATIN_WORD_LENGTH)
    {
        ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string[((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len++] = key_ptr->text[0];
        ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cur_page = 0;
        QwertLatinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: En���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertLatinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
       return   TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len > 0)
        {
            QwertLatinClear(data_ptr);
        }
        else
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        break;

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: En���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertLatinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_Q_LATIN_T* data_latin_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
       return   TRUE;
    }

    data_latin_ptr = ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data));
    switch (key_ptr->code)
    {
    case VKEY_CAPS:
        SetVkbNextCaps(data_ptr);
        QwertLatinSetMatch(data_ptr);

        if (data_latin_ptr->input_string_len > 0)
        {
            QwertLatinUpdate(data_ptr);
        }

        break;

    case VKEY_DEL:

        if (data_latin_ptr->input_string_len > 1)
        {
            data_latin_ptr->input_string_len--;
            data_latin_ptr->cur_page = 0;
            QwertLatinUpdate(data_ptr);
        }
        else if (data_latin_ptr->input_string_len == 1)
        {
            QwertLatinClear(data_ptr);
        }
        else
        {
            CommitChar (data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:

        if (data_latin_ptr->input_string_len > 0)
        {
            CapsCommitStr(data_ptr, data_latin_ptr->cand[0].string, data_latin_ptr->cand[0].length);
            UpdateUdbCache(&data_latin_ptr->engine, 
                    data_latin_ptr->cand[0].string, 
                    data_latin_ptr->cand[0].length, 
                    GUIIM_M_TP_ENGLISH);
            QwertLatinClear(data_ptr);
        }
        else
        {
            CommitChar (data_ptr, '\n');
        }

        break;

    case VKEY_SPACE:

        //commit candidate first
        if (data_latin_ptr->cand_cnt > 0)
        {
            CapsCommitStr(data_ptr, data_latin_ptr->cand[0].string, data_latin_ptr->cand[0].length);
            UpdateUdbCache(&data_latin_ptr->engine, 
                    data_latin_ptr->cand[0].string, 
                    data_latin_ptr->cand[0].length, 
                    GUIIM_M_TP_ENGLISH);

        }

        //clear input
        QwertLatinClear(data_ptr);

        //commit a space
        CommitChar (data_ptr, ' ');

        break;

    case VKEY_PAGEUP:
        //SCI_ASSERT(((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cur_page > 0); /*assert verified*/
        if(data_latin_ptr->cur_page > 0)
        {
            data_latin_ptr->cur_page--;
        }
        QwertLatinUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        //SCI_ASSERT(1 == ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->has_next_page); /*assert verified*/
        if(1 != data_latin_ptr->has_next_page)
        {
            break;
        }
        data_latin_ptr->page[data_latin_ptr->cur_page + 1]
        = data_latin_ptr->page[data_latin_ptr->cur_page]
          + data_latin_ptr->cand_cnt;
        data_latin_ptr->cur_page++;
        QwertLatinUpdate(data_ptr);
        break;

    case VKEY_123SYMBOL:
        if (data_latin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: En���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertLatinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand_cnt);
        QwertLatinSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: En���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertLatinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_Q_LATIN_T* data_latin_ptr = PNULL;
    int32 select_idx = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    data_latin_ptr = ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data));
    select_idx = key_ptr->code - VKEY_CANDIDATE;
    if(select_idx >= 0 && select_idx < data_latin_ptr->cand_cnt)
    {
        CapsCommitStr(data_ptr, data_latin_ptr->cand[select_idx].string, data_latin_ptr->cand[select_idx].length);
        UpdateUdbCache(&data_latin_ptr->engine, data_latin_ptr->cand[select_idx].string, data_latin_ptr->cand[select_idx].length, GUIIM_M_TP_ENGLISH);
    }
    
    //SCI_ASSERT(select_idx >= 0); /*assert verified*/
    //SCI_ASSERT(select_idx < ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand_cnt); /*assert verified*/

    //commit candidate

    //clear input
    QwertLatinClear(data_ptr);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: QwertLatinSetMatch
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertLatinSetMatch(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    switch (data_ptr->vkb.cap)
    {
    case CAPITTAL_MODE_INITIAL:
        ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->match_flags = KMX_LATIN_INITIAL_CASE;
        break;

    case CAPITTAL_MODE_UPPER:
        ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->match_flags = KMX_LATIN_UPPER_CASE;
        break;

        //case CAPITTAL_MODE_LOWER:
    default:
        ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->match_flags = KMX_LATIN_LOWER_CASE;
        break;
    }

    return;
}


/*==============================================================================
Description: QwertLatinClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertLatinClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    SCI_MEMSET((char*)(data_ptr->data) + sizeof(((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->engine), 0,
               sizeof(KERNEL_DATA_Q_LATIN_T) - sizeof(((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->engine));

    QwertLatinSetMatch(data_ptr);

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand_cnt);
    QwertLatinSetCand(data_ptr);

    return;
}


/*==============================================================================
Description: QwertLatinUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
kmx_latin_query_association
kmx_latin_query_association_ex
���鼸�ε����������һ����
==============================================================================*/
PUBLIC void QwertLatinUpdate(IM_DATA_T *data_ptr)
{
    KMX_LATIN_MATCH_RESULT match_result = {0};
    int16 match_index = 0;
    uint8 cand_cnt = 0;
    uint8 has_next_page = 0;
    KERNEL_DATA_Q_LATIN_T * kernel_data_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    
    kernel_data_ptr = (KERNEL_DATA_Q_LATIN_T*)(data_ptr->data);
    
    //search match
    match_index = kernel_data_ptr->page[kernel_data_ptr->cur_page];

    do
    {
        int32 search_res = 0;
        int32 left_cnt = LATIN_CANDIDATE_COUNT - cand_cnt;
        uint8 copy_cnt = 0;

        search_res = kmx_latin_query_association(&(kernel_data_ptr->engine),
                     kernel_data_ptr->input_string, kernel_data_ptr->input_string_len,
                     kernel_data_ptr->match_flags,
                     match_index,
                     &match_result);

        if (match_result.count < left_cnt)
        {
            copy_cnt = match_result.count;
        }
        else if (match_result.count == left_cnt)
        {
            copy_cnt = left_cnt;

            if (match_result.more_available != 0)
            {
                match_result.more_available = 0;
                has_next_page = 1;
            }
        }
        else
        {
            copy_cnt = left_cnt;
            match_result.more_available = 0;
            has_next_page = 1;
        }

        if (0 == search_res || 0 == copy_cnt)
        {
            break;
        }

        SCI_MEMCPY(kernel_data_ptr->cand + cand_cnt,
                   match_result.table,
                   copy_cnt * sizeof(KMX_LATIN_CANDIDATE));

        cand_cnt += copy_cnt;
        match_index += match_result.count;
    }
    while (match_result.more_available != 0);


    if (cand_cnt > 0)
    {
        kernel_data_ptr->cand_cnt = cand_cnt;
        kernel_data_ptr->has_next_page = has_next_page;
    }
    else
    {
        kernel_data_ptr->cand_cnt = 1;
        kernel_data_ptr->has_next_page = 0;

        kernel_data_ptr->cand[0].length = kernel_data_ptr->input_string_len;
        SCI_MEMCPY(kernel_data_ptr->cand[0].string,
                   kernel_data_ptr->input_string,
                   kernel_data_ptr->input_string_len * sizeof(wchar));
    }

    SetHideKey(data_ptr, kernel_data_ptr->cand_cnt);
    QwertLatinSetCand(data_ptr);

    return;
}


/*==============================================================================
Description: QwertLatinSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertLatinSetCand(IM_DATA_T *data_ptr)
{
    int8 cand_layer = 0;
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;
    GUI_RECT_T cand_rect = {0};
    int16 cand_height = 0;
    uint8 line_idx = 0;
    int16 panel_width, panel_height, dw, dh;

    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}

    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand_cnt)
    {
        goto QwertLatinSetCandEnd;
    }
    
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;
    dw = 3; /// 2 * data_ptr->theme.key_hor_space * panel_width / 1000;
    dh = 6; ///2 * data_ptr->theme.key_hor_space * panel_height / 1000;

    //set normal candidate
    cand_rect = data_ptr->vkb.vessel_key->rect;
    cand_height = cand_rect.bottom - cand_rect.top - dh - 1;    //cand ���� ����û��ȥ��space���
    cand_rect.left += dw;
    cand_rect.top += 1 + dh / 2;    //������һ������
    cand_rect.bottom -= dh / 2;
    
    while (cand_cnt < ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        int16 word_width = 0;
        int16 disp_right = 0;

        word_width = GUI_GetStringWidth(data_ptr->theme.key_cand[0].text_font,
                                        ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand[cand_cnt].string, ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand[cand_cnt].length);
        word_width += 2 * data_ptr->theme.key_padding;

        if (word_width < CAND_WIDTH_MIN) word_width = CAND_WIDTH_MIN;

        disp_right = cand_rect.left + word_width - 1;

        if (disp_right > cand_rect.right && line_idx > 0)
        {
            if (0 == data_ptr->vkb.is_expand) break;

            if (++cand_layer >= LATIN_CANDIDATE_ROW) break;

            cand_rect.left = data_ptr->vkb.vessel_key->rect.left + dw / 2;
            cand_rect.right = data_ptr->vkb.vessel_key->rect.right - dw / 2;
            cand_rect.top += cand_height + dh;
            cand_rect.bottom = cand_rect.top + cand_height;

            line_idx = 0;

            disp_right = cand_rect.left + word_width - 1;
        }

        line_idx++;

        data_ptr->vkb.keys[cand_key_idx].rect = cand_rect;
        data_ptr->vkb.keys[cand_key_idx].rect.right = disp_right;

        if (data_ptr->vkb.keys[cand_key_idx].rect.right > cand_rect.right)
        {
            data_ptr->vkb.keys[cand_key_idx].rect.right = cand_rect.right;
        }

        data_ptr->vkb.keys[cand_key_idx].text_len = MIN (((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand[cand_cnt].length, TEXT_LEN_LIMIT);
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].text, 0, sizeof(data_ptr->vkb.keys[cand_key_idx].text));
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].text, ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand[cand_cnt].string,
                   data_ptr->vkb.keys[cand_key_idx].text_len * sizeof(wchar));
#if 0//to delete assert
		{
			SCI_ASSERT (((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand[cand_cnt].length <= TEXT_LEN_LIMIT);  //just test whether the IME_engine has error.
		}
#endif
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].print_text, 0, sizeof(data_ptr->vkb.keys[cand_key_idx].print_text));
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));
			
        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
        cand_rect.left = disp_right + 1 + dw;
        cand_key_idx++;
        cand_cnt++;
    }

	//pagedown/up
    if (1 == data_ptr->vkb.is_expand)
    {
        if (((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand_cnt > cand_cnt)
        {
            ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cand_cnt = cand_cnt;
            ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->has_next_page = 1;
        }

        if (((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);

        if (1 == ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
    }

QwertLatinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
		
		//clear the text, maybe next time input cause any problem.
		data_ptr->vkb.keys[cand_key_idx].text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].text));  
		data_ptr->vkb.keys[cand_key_idx].print_text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].print_text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].print_text));
    }

    return;
}


#if defined (IM_SIMP_CHINESE_SUPPORT)
/*==============================================================================
Description: QwertPinyinInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinInit(IM_DATA_T *data_ptr)
{
    int32 init_res = 0;
    KMX_IME_ENGINE_INIT init_info = {0};

    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_Q_PINYIN_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_Q_PINYIN_T));

    QwertPinyinInitKb(data_ptr);

    init_info.database = GetKmxLdb_ptr(GUIIM_LANG_CHS, FALSE);
    init_info.flags = GetPyFuzzy();
    MMIIM_IME_FindUserCacheByIM(
        data_ptr->init_param.method_param_ptr->im_def_ptr->method,
        &init_info.user_data, &init_info.user_data_size);

    init_res = kmx_pinyin_initialize(&(((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->engine), &init_info);
    MMIIM_SetImKerState(0);
    if (0 == init_res)
    {
        return FALSE;
    }

    //SCI_ASSERT(*(uint32*)((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->engine.engine == 0x00000001); /*assert verified*/

    return TRUE;
}


/*==============================================================================
Description: QwertPinyinTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: QwertAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    QwertPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void QwertPinyinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 const CHAR_CNT = 26;
    uint8 const CTRL_CNT = 9;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT_E;
    uint8 const SYS_CNT = 2;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMI_STRING_T slide_cue = {PNULL, 0};

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_qwert_default;

    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_qwert_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 26
    //character key ��λ��������2��ʼ��27
    data_ptr->vkb.charactor_first = key_idx = 0;

    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].text[0] = "qwertyuiopasdfghjklzxcvbnm"[key_idx];
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertPinyinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key ��λ��������28��ʼ������2��
    key_idx++;
    cust_begin = key_idx;

    for (; key_idx < cust_begin + CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].text[0] = L"\xff1f\xff0c"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //control * 6,  caps, del, 123symbol, space, set, enter
    //character key ��λ�������ӿ�ʼ��30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertPinyinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = QwertPinyinCtrlUp;
    }

    //--ƴ���ָ��
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_PY_SEPARATE;
    vkb_ptr->keys[key_idx].text_len = 1;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"'", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    vkb_ptr->keys[key_idx].font_size = 20;

    //--delete
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--space
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_Q;
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_PINYIN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertPinyinSysUp;
    }

    //--
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;


    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_qwert_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
        
    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_qwert_panel_position[0].right - g_qwert_panel_position[0].left - 5;         //�Ҳ���һ������
        int16 h = g_qwert_panel_position[0].bottom - g_qwert_panel_position[0].top - data_ptr->theme.key_ver_space - 5; //������Щ�հ�,������һ������
        GUI_RECT_T cand_rect = {0};

        //ƫ����ֱ�ӷ���ConvertRect������㣬�����ڶ�����ƫ�������Ǿ��ȵ�
        cand_rect.top = g_qwert_panel_position[0].top + y * (h + data_ptr->theme.key_ver_space) + data_ptr->theme.key_ver_space / 2 + 5;
        cand_rect.bottom = cand_rect.top + h;
        cand_rect.left = g_qwert_panel_position[0].left + x * w / CH_CANDIDATE_COL + 1;
        cand_rect.right = cand_rect.left + w / CH_CANDIDATE_COL;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, 0);

        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertPinyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    //SCI_ASSERT(((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len <= KMX_MAX_PINYIN_INPUT); /*assert verified*/

    if (((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len < KMX_MAX_PINYIN_INPUT
            && ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->confirm_cnt < PINYIN_CONFIRM_LIMIT)
    {
        ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string[((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len++] = key_ptr->text[0];
        ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cur_page = 0;
        QwertPinyinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len > 0)
        {
            QwertPinyinClear(data_ptr);
        }
        else
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        break;

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_Q_PINYIN_T* data_pinyin_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }

    data_pinyin_ptr = ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data));
    switch (key_ptr->code)
    {
    case VKEY_PY_SEPARATE:

        if (0 < data_pinyin_ptr->input_string_len
                && data_pinyin_ptr->input_string_len < KMX_MAX_PINYIN_INPUT
                && data_pinyin_ptr->input_string[data_pinyin_ptr->input_string_len-1] != PINYIN_SEPARATOR)
        {
            data_pinyin_ptr->input_string[data_pinyin_ptr->input_string_len++] = PINYIN_SEPARATOR;
            QwertPinyinDispEdit(data_ptr);
        }

        break;

    case VKEY_DEL:

        if (data_pinyin_ptr->confirm_cnt > 0)
        {
            data_pinyin_ptr->confirm_cnt--;
            data_pinyin_ptr->cur_page = 0;
            QwertPinyinUpdate(data_ptr);
        }
        else if (data_pinyin_ptr->input_string_len > 1)
        {
            data_pinyin_ptr->input_string_len--;
            data_pinyin_ptr->cur_page = 0;
            QwertPinyinUpdate(data_ptr);
        }
        else if (data_pinyin_ptr->input_string_len == 1)
        {
            QwertPinyinClear(data_ptr);
        }
        else if (data_pinyin_ptr->cand_cnt > 0)
        {
            QwertPinyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:

        if (data_pinyin_ptr->input_string_len > 0)
        {
            wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
            int32 buffer_len = 0;
            int32 str_idx = 0;

            for (str_idx = 0; str_idx < data_pinyin_ptr->input_string_len; str_idx++)
            {
                buffer[buffer_len] = data_pinyin_ptr->input_string[str_idx];
                buffer_len++;
            }

            CommitStr(data_ptr, buffer, buffer_len);
            UpdateUdbCache(&data_pinyin_ptr->engine, buffer, buffer_len, GUIIM_M_TP_PINYIN);
            QwertPinyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\n');
        }

        break;

    case VKEY_SPACE:

        if (data_pinyin_ptr->input_string_len == 0)
        {
            CommitChar(data_ptr, 0x3000);
        }
		else
		{
	        //commit candidate first
	        if (data_pinyin_ptr->cand_cnt > 0)
	        {
	            CapsCommitStr(data_ptr, data_pinyin_ptr->cand[0].string, data_pinyin_ptr->cand[0].length);
	            UpdateUdbCache(&data_pinyin_ptr->engine, 
	                    data_pinyin_ptr->cand[0].string, 
	                    data_pinyin_ptr->cand[0].length, 
	                    GUIIM_M_TP_PINYIN);

	        }

	        //clear input
	        QwertPinyinClear(data_ptr);
		}

        break;

    case VKEY_PAGEUP:
        //SCI_ASSERT(((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cur_page > 0); /*assert verified*/
        if(data_pinyin_ptr->cur_page > 0)
        {
            data_pinyin_ptr->cur_page--;
        }

        if (data_pinyin_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            QwertPinyinUpdate(data_ptr);
        }
        else if (data_pinyin_ptr->edit_state == EDIT_STATE_ASSO)
        {
            QwertPinyinAsso(data_ptr);
        }

        break;

    case VKEY_PAGEDOWN:
        //SCI_ASSERT(1 == ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->has_next_page); /*assert verified*/
        if(1 != data_pinyin_ptr->has_next_page)
        {
            break;
        }
        
        data_pinyin_ptr->cur_page++;

        if (data_pinyin_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            QwertPinyinUpdate(data_ptr);
        }
        else if (data_pinyin_ptr->edit_state == EDIT_STATE_ASSO)
        {
            QwertPinyinAsso(data_ptr);
        }

        break;

    case VKEY_123SYMBOL:
        if (data_pinyin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt);
        QwertPinyinSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Pinyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN QwertPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 select_idx = 0;
    uint8 confirm_digit_len = 0;
    uint16 i = 0;
    wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
    uint32 buffer_len = 0;
    KERNEL_DATA_Q_PINYIN_T* pinyin_data_ptr = PNULL;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    select_idx = key_ptr->code - VKEY_CANDIDATE;

    //SCI_ASSERT(select_idx >= 0); /*assert verified*/
    //SCI_ASSERT(select_idx < ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt); /*assert verified*/

    pinyin_data_ptr = (KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data);
    
    if(select_idx < 0 || select_idx >= pinyin_data_ptr->cand_cnt)
    {
        return TRUE;
    }
    
    if (pinyin_data_ptr->edit_state == EDIT_STATE_SEARCH)
    {
        pinyin_data_ptr->confirm[pinyin_data_ptr->confirm_cnt++] = pinyin_data_ptr->cand[select_idx];

        for (i = 0; i < pinyin_data_ptr->confirm_cnt; i++)
        {
            confirm_digit_len += pinyin_data_ptr->confirm[i].match_length;

            if (PINYIN_SEPARATOR == ((KERNEL_DATA_PINYIN_T*)(data_ptr->data))->input_string[confirm_digit_len]
                    && confirm_digit_len < pinyin_data_ptr->input_string_len)
            {
                confirm_digit_len++;
            }
        }

        //SCI_ASSERT(confirm_digit_len <= ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/

        if (confirm_digit_len < pinyin_data_ptr->input_string_len)
        {
            pinyin_data_ptr->cur_page = 0;
            QwertPinyinUpdate(data_ptr);
        }
        else
        {
            //all match, commit
            for (i = 0; i < pinyin_data_ptr->confirm_cnt; i++)
            {
                SCI_MEMCPY(buffer + buffer_len, pinyin_data_ptr->confirm[i].string,
                           pinyin_data_ptr->confirm[i].length * sizeof(wchar));
                buffer_len += pinyin_data_ptr->confirm[i].length;
            }

            //SCI_ASSERT(buffer_len > 0); /*assert verified*/

            CommitStr(data_ptr, buffer, buffer_len);
            UpdateUdbCache(&pinyin_data_ptr->engine, buffer, buffer_len, GUIIM_M_TPQ_PINYIN);
            QwertPinyinClear(data_ptr);
            if (buffer_len < KMX_MAX_PINYIN_INPUT - 1)
            {
                pinyin_data_ptr->last_char = buffer[buffer_len - 1];
            }
            QwertPinyinAsso(data_ptr);
        }
    }
    else if (pinyin_data_ptr->edit_state == EDIT_STATE_ASSO)
    {
        wchar last_char = pinyin_data_ptr->cand[select_idx].string[0];

        CommitChar(data_ptr, last_char);
        UpdateUdbCache(&pinyin_data_ptr->engine, &last_char, 1, GUIIM_M_TPQ_PINYIN);

        QwertPinyinClear(data_ptr);
        pinyin_data_ptr->last_char = last_char;
        QwertPinyinAsso(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: QwertPinyinClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    SCI_MEMSET((char*)(data_ptr->data) + sizeof(((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->engine), 0,
               sizeof(KERNEL_DATA_Q_PINYIN_T) - sizeof(((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->engine));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt);
    QwertPinyinSetCand(data_ptr);
    QwertPinyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: QwertPinyinUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinUpdate(IM_DATA_T *data_ptr)
{
    uint8 confirm_digit_len = 0;
    uint16 i = 0;
    KMX_PINYIN_WORD_MATCH_RESULT match_result = {0};
    uint16 word_match_index = 0;
    uint8 cand_cnt = 0;
    uint8 has_next_page = 0;
    EDIT_STATE_T old_edit_state = EDIT_STATE_NONE;
    CHECK_DATA_EX(data_ptr);

    old_edit_state = ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->edit_state;
    ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->edit_state = EDIT_STATE_SEARCH;

    /* search */
    for (i = 0; i < ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->confirm_cnt; i++)
    {
        confirm_digit_len += ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->confirm[i].match_length;
    }

    //SCI_ASSERT(confirm_digit_len < ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/
    if(confirm_digit_len >= ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len)
    {
        return;
    }
    
    word_match_index = ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cur_page * data_ptr->vkb.candidate_cnt;

    do
    {
        int32 search_res = 0;
        int32 left_cnt = data_ptr->vkb.candidate_cnt - cand_cnt;
        uint8 copy_cnt = 0;

        search_res = kmx_pinyin_match(&(((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->engine),
                                      ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string + confirm_digit_len,
                                      ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len - confirm_digit_len,
                                      word_match_index,
                                      &match_result);

        if (match_result.count < left_cnt)
        {
            copy_cnt = match_result.count;
        }
        else if (match_result.count == left_cnt)
        {
            copy_cnt = left_cnt;

            if (match_result.more_available != 0)
            {
                match_result.more_available = 0;
                has_next_page = 1;
            }
        }
        else
        {
            copy_cnt = left_cnt;
            match_result.more_available = 0;
            has_next_page = 1;
        }

        if (0 == search_res || 0 == copy_cnt)
        {
            break;
        }

        SCI_MEMCPY(((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand + cand_cnt,
                   match_result.table,
                   copy_cnt * sizeof(KMX_PINYIN_CANDIDATE));

        cand_cnt += copy_cnt;
        word_match_index += match_result.count;
    }
    while (match_result.more_available != 0);

    if (cand_cnt > 0)
    {
        ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->edit_state = EDIT_STATE_SEARCH;
        ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt = cand_cnt;
        ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->has_next_page = has_next_page;

        SetHideKey(data_ptr, ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt);
        QwertPinyinSetCand(data_ptr);
        QwertPinyinDispEdit(data_ptr);
    }
    else
    {
        ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len--;
	 ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->edit_state = old_edit_state;
    }

    return;
}


/*==============================================================================
Description: QwertPinyinSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;

    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}

    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt)
    {
        goto QwertPinyinSetCandEnd;
    }
    
    //set normal candidate
    while (cand_cnt < ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        data_ptr->vkb.keys[cand_key_idx].text_len = ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand[cand_cnt].length;
        SCI_MEMCPY(data_ptr->vkb.keys[cand_key_idx].text, ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand[cand_cnt].string,
                   ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand[cand_cnt].length * sizeof(wchar));
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));

        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
        cand_key_idx++;
        cand_cnt++;
    }


    if (1 == data_ptr->vkb.is_expand)
    {
        ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt = cand_cnt;

        if (((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);
		
        if (1 == ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
    }

QwertPinyinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
		
		//clear the text, maybe next time input cause any problem.
		data_ptr->vkb.keys[cand_key_idx].text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].text));  
		data_ptr->vkb.keys[cand_key_idx].print_text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].print_text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].print_text));
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
    uint32 buffer_len = 0;
    int16 i = 0;
    uint8 confirm_digit_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};

    CHECK_DATA_EX(data_ptr);

    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/
    if(!has_edit_layer)
    {
        return;
    }
    
    UILAYER_Clear(&data_ptr->edit_layer);

    if (((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    //����
    for (i = 0; i < ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->confirm_cnt; i++)
    {
        SCI_MEMCPY(buffer + buffer_len, ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->confirm[i].string,
                   ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->confirm[i].length * sizeof(wchar));
        buffer_len += ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->confirm[i].length;
        confirm_digit_len += ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->confirm[i].match_length;
    }

    //��ĸ
    for (i = 0; i < ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string_len - confirm_digit_len; i++)
    {
        buffer[buffer_len++] = ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->input_string[confirm_digit_len + i];
    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len > edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


/*==============================================================================
Description: QwertPinyinAsso
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertPinyinAsso(IM_DATA_T *data_ptr)
{
    wchar buffer[CH_CANDIDATE_COUNT_E*ASSO_PAGE_LIMIT + 1] = {0};
    int32 offset_asso_cnt = 0;
    int32 asso_cnt = 0;
    int32 asso_idx = 0;

    CHECK_DATA_EX(data_ptr);

    ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->edit_state = EDIT_STATE_ASSO;

    offset_asso_cnt = ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cur_page * CH_CANDIDATE_COUNT_E;

    asso_cnt = kmx_chinese_query_association(GetKmxLdb_ptr(GUIIM_LANG_CHS, FALSE),
               ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->last_char, buffer,
               offset_asso_cnt + CH_CANDIDATE_COUNT_E + 1);

    if (asso_cnt <= offset_asso_cnt)
    {
        return;
    }

    ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->has_next_page = 0;

    if (asso_cnt > offset_asso_cnt + CH_CANDIDATE_COUNT_E)
    {
        if (((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cur_page + 1 < ASSO_PAGE_LIMIT)
        {
            ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->has_next_page = 1;
        }

        asso_cnt = CH_CANDIDATE_COUNT_E;
    }
    else
    {
        asso_cnt -= offset_asso_cnt;
    }

    for (asso_idx = 0; asso_idx < asso_cnt; asso_idx++)
    {
        ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand[asso_idx].length = 1;
        ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand[asso_idx].string[0] = buffer[offset_asso_cnt + asso_idx];
    }

    ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt = asso_cnt;

    SetHideKey(data_ptr, ((KERNEL_DATA_Q_PINYIN_T*)(data_ptr->data))->cand_cnt);
    QwertPinyinSetCand(data_ptr);

    return;
}
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
/*==============================================================================
Description: ThumbZhuyinInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinInit(IM_DATA_T *data_ptr)
{
    int32 init_res = 0;
    KMX_IME_ENGINE_INIT init_info = {0};

    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_ZHUYIN_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_ZHUYIN_T));

    ThumbZhuyinInitKb(data_ptr);

    init_info.database = GetKmxLdb_ptr(GUIIM_LANG_CHT, FALSE);
    if (PNULL == init_info.database)
    {
    	SCI_TRACE_LOW ("[MMIIM] ThumbZhuyinInit PNULL == init_info.database");
        return FALSE;
    }
	
    MMIIM_IME_FindUserCacheByIM(
        data_ptr->init_param.method_param_ptr->im_def_ptr->method,
        &init_info.user_data, &init_info.user_data_size);
    init_res = kmx_zhuyin_initialize(&(((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->engine), &init_info);
    MMIIM_SetImKerState(0);
    if (0 == init_res)
    {
    	SCI_TRACE_LOW ("[MMIIM] ThumbZhuyinInit");
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: ThumbZhuyinTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer > 0)
    {
        MMK_StopTimer(((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer);
        ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer = 0;
    }

    //release engine
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: ThumbAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    ThumbZhuyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void ThumbZhuyinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 8;
    uint8 const SYS_CNT = 4;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT;
    uint8 const PY_CAND_CNT = 4;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    int32 py_cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;
    MMI_STRING_T slide_cue = {PNULL, 0};

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += PY_CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].text[0] = g_thumb_cust_symb[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = ThumbZhuyinVkeyDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCustUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    if (key_table_ptr == PNULL)
    {
        key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    }

    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_2X4_EXCLUDE_DIGIT;        //
        //=========��������˿ӡ����
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));    /*lint !e613*/
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = ThumbZhuyinVkeyDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbZhuyinCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = ThumbZhuyinVkeyDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbZhuyinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--!!!!!!!!!!�ո��ַ�,��ƴ���ȵط���Щ��ͬ
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].text_len = 1;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L" ", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    vkb_ptr->keys[key_idx].font_size = 20;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_CENTER;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_PINYIN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = ThumbZhuyinVkeyDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinSysUp;
    }

    //--hide
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--expand
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;

    //--expand zy pgup - reset rect
    key_idx++;
    vkb_ptr->py_pgup_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[26]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_PY_PAGEUP;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;

    //--expand zy pgdn - reset rect
    key_idx++;
    vkb_ptr->py_pgdn_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[27]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].code = VKEY_PY_PAGEDOWN;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;


    //--expand zy panel
    key_idx++;
    vkb_ptr->py_vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[25]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
        
    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_thumb_panel_position[0].right - g_thumb_panel_position[0].left + 1;
        int16 h = g_thumb_panel_position[0].bottom - g_thumb_panel_position[0].top + 1;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = g_thumb_panel_position[0].top + y * h;
        cand_rect.bottom = g_thumb_panel_position[0].bottom + y * h;
        cand_rect.left = g_thumb_panel_position[0].left + x * w / CH_CANDIDATE_COL;
        cand_rect.right = g_thumb_panel_position[0].left + (x + 1) * w / CH_CANDIDATE_COL - 1;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = ThumbZhuyinVkeyDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //zhuyin candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    py_cand_begin = key_idx;

    for (; key_idx < py_cand_begin + PY_CAND_CNT; key_idx++)
    {
        int16 w = g_thumb_panel_position[25].right - g_thumb_panel_position[25].left + 1;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = g_thumb_panel_position[25].top;
        cand_rect.bottom = g_thumb_panel_position[25].bottom;
        cand_rect.left = g_thumb_panel_position[25].left + (key_idx - py_cand_begin) * w / PY_CAND_CNT;
        cand_rect.right = g_thumb_panel_position[25].left + (key_idx - py_cand_begin + 1) * w / PY_CAND_CNT - 1;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].code = VKEY_PY_CANDIDATE + key_idx - py_cand_begin;
        vkb_ptr->keys[key_idx].tag =  KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = ThumbZhuyinVkeyDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbZhuyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->py_candidate_first = py_cand_begin;
    vkb_ptr->py_candidate_cnt = PY_CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len > 0)
    {
        return TRUE;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        CommitChar(data_ptr, '1' + (key_ptr->code - VKEY_1));
    }
    else if (VKEY_0 == key_ptr->code)
    {
        CommitChar(data_ptr, '0');
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}

/*==============================================================================
Description: ThumbAbcCommitCB
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbZhuYinCommitCB(uint8 timer_id, uint32 param)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)param;

    if (PNULL == data_ptr
            || (sizeof(IM_DATA_T) != data_ptr->check_info)
            || ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data) == PNULL)
            || ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer != timer_id)
    {
        return;
    }

    CHECK_DATA_EX(data_ptr);

    if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer == 0) return;

    ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer = 0;
	((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->last_key_index = 0;

    return;
}

/*==============================================================================
Description: ThumbZhuyinVkeyDown, when the press key changed, need to clear
			multi-tap key state.
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
LOCAL BOOLEAN ThumbZhuyinVkeyDown (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
	KEY_INFO_T *last_key_ptr = PNULL;
	KERNEL_DATA_ZHUYIN_T *data_zhuyin_ptr = PNULL;

	//para check
	if (data_ptr == PNULL || key_ptr == PNULL)
	{
		return FALSE;
	}
	
	data_zhuyin_ptr = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data));
	if (data_zhuyin_ptr == PNULL)
	{
		return FALSE;
	}

	last_key_ptr = data_zhuyin_ptr->last_key;	//save last key,because vkbdown will change it

	//display
	VkbDown (data_ptr, key_ptr);

	//key switch
	if (last_key_ptr && last_key_ptr != key_ptr)	
	{
		//clear multi-tap timer
		if (data_zhuyin_ptr->commit_timer > 0)		
        {
	        MMK_StopTimer(data_zhuyin_ptr->commit_timer);
	        data_zhuyin_ptr->commit_timer = 0;
		}

		//clear multi-tap index
		data_zhuyin_ptr->last_key_index = 0;
	}

	return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    uint16 key_chr_cnt = 0;
    KERNEL_DATA_ZHUYIN_T* kernel_data_zhuyin_ptr = PNULL;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    if (PNULL == key_ptr)
    {
        return TRUE;
    }
    
    kernel_data_zhuyin_ptr = (KERNEL_DATA_ZHUYIN_T*)(data_ptr->data);
        
    if (kernel_data_zhuyin_ptr->last_key != key_ptr)
    {
        //ThumbAbcCommitLast(data_ptr);

        kernel_data_zhuyin_ptr->last_key = key_ptr;

        //reset index
        kernel_data_zhuyin_ptr->last_key_index = 0;

        if (kernel_data_zhuyin_ptr->commit_timer > 0)
        {
            MMK_StopTimer(kernel_data_zhuyin_ptr->commit_timer);
            kernel_data_zhuyin_ptr->commit_timer = 0;
        }
    }

    key_chr_cnt = key_ptr->text_len;
    if (1 == key_chr_cnt)
    {
        if (kernel_data_zhuyin_ptr->input_string_len < KMX_MAX_ZHUYIN_INPUT
                && kernel_data_zhuyin_ptr->confirm_cnt < ZHUYIN_CONFIRM_LIMIT)
        {
            kernel_data_zhuyin_ptr->input_string[kernel_data_zhuyin_ptr->input_string_len++] = key_ptr->text[0];
        }
    }
    else if (key_chr_cnt > 1)
    {
        //set underline character
        kernel_data_zhuyin_ptr->last_key_index += 1;

        if (kernel_data_zhuyin_ptr->last_key_index > key_chr_cnt)
        {
            kernel_data_zhuyin_ptr->last_key_index = 1;
        }

        //�����ַ��������ѯ�ַ�����
        if (kernel_data_zhuyin_ptr->input_string_len < KMX_MAX_ZHUYIN_INPUT
                && kernel_data_zhuyin_ptr->confirm_cnt < ZHUYIN_CONFIRM_LIMIT)
        {
            if (kernel_data_zhuyin_ptr->input_string_len == 0)    //�׸��ַ�����
            {
                kernel_data_zhuyin_ptr->input_string[kernel_data_zhuyin_ptr->input_string_len++] = key_ptr->text[0];
            }
            else if (kernel_data_zhuyin_ptr->commit_timer > 0)     //ȥ��ǰһ���ַ�
            {
                kernel_data_zhuyin_ptr->input_string[kernel_data_zhuyin_ptr->input_string_len - 1] = key_ptr->text[kernel_data_zhuyin_ptr->last_key_index-1];

                MMK_StopTimer(kernel_data_zhuyin_ptr->commit_timer);
                kernel_data_zhuyin_ptr->commit_timer = 0;
            }
            else
            {
                kernel_data_zhuyin_ptr->input_string[kernel_data_zhuyin_ptr->input_string_len++] = key_ptr->text[0];
            }
        }

        kernel_data_zhuyin_ptr->commit_timer = MMK_CreateTimerCallback(
                    ZHUYIN_COMMIT_TICK,
                    ThumbZhuYinCommitCB,
                    (uint32)data_ptr,
                    FALSE);
    }

    kernel_data_zhuyin_ptr->cur_page = 0;
    ThumbZhuyinUpdate(data_ptr);
    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len > 0)
        {
            ThumbZhuyinClear(data_ptr);
        }
        else
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        break;

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_ZHUYIN_T* data_zhuyin_ptr = PNULL;
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if (PNULL == key_ptr)
    {
        return TRUE;
    }
    
    data_zhuyin_ptr = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data));
    switch (key_ptr->code)
    {
    case VKEY_SPACE:

        //commit candidate first
        if (data_zhuyin_ptr->cand_cnt > 0)
        {
            CapsCommitStr(data_ptr, data_zhuyin_ptr->cand[0].string, data_zhuyin_ptr->cand[0].length);
            UpdateUdbCache(&data_zhuyin_ptr->engine, 
                    data_zhuyin_ptr->cand[0].string, 
                    data_zhuyin_ptr->cand[0].length, 
                    GUIIM_M_TP_ZHUYIN);

			//clear input
	        QwertZhuyinClear(data_ptr);
        }        
		else
		{
	        CommitChar(data_ptr, 0x3000);
		}
        break;

    case VKEY_DEL:

        if (data_zhuyin_ptr->confirm_cnt > 0)
        {
            data_zhuyin_ptr->confirm_cnt--;
            //((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index = KMX_PINYIN_MATCH_INDEX_NONE;
            data_zhuyin_ptr->cur_page = 0;
            ThumbZhuyinUpdate(data_ptr);
        }
        else if (data_zhuyin_ptr->input_string_len > 1)
        {
            data_zhuyin_ptr->input_string_len--;
            //((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index = KMX_PINYIN_MATCH_INDEX_NONE;
            data_zhuyin_ptr->cur_page = 0;
            ThumbZhuyinUpdate(data_ptr);
        }
        else if (data_zhuyin_ptr->input_string_len == 1)
        {
            ThumbZhuyinClear(data_ptr);
        }
        else if (data_zhuyin_ptr->cand_cnt > 0)
        {
            ThumbZhuyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:

        if (data_zhuyin_ptr->input_string_len > 0)
        {
            wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
            int32 buffer_len = 0;

            while (buffer_len < data_zhuyin_ptr->input_string_len)
            {
                buffer[buffer_len] = data_zhuyin_ptr->input_string[buffer_len];
                buffer_len++;
            }

            CommitStr(data_ptr, buffer, buffer_len);
            UpdateUdbCache(&data_zhuyin_ptr->engine, buffer, buffer_len, GUIIM_M_TP_ZHUYIN);
            ThumbZhuyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\n');
        }

        break;

    case VKEY_PAGEUP:
        //SCI_ASSERT(((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cur_page > 0); /*assert verified*/
        if(data_zhuyin_ptr->cur_page > 0)
        {
            data_zhuyin_ptr->cur_page--;
        }

        if (data_zhuyin_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            ThumbZhuyinUpdate(data_ptr);
        }
        else if (data_zhuyin_ptr->edit_state == EDIT_STATE_ASSO)
        {
            ThumbZhuyinAsso(data_ptr);
        }

        break;

    case VKEY_PAGEDOWN:
        //SCI_ASSERT(1 == ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->has_next_page); /*assert verified*/
        if(1 != data_zhuyin_ptr->has_next_page)
        {
            break;
        }
        data_zhuyin_ptr->cur_page++;

        if (data_zhuyin_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            ThumbZhuyinUpdate(data_ptr);
        }
        else if (data_zhuyin_ptr->edit_state == EDIT_STATE_ASSO)
        {
            ThumbZhuyinAsso(data_ptr);
        }

        break;

    case VKEY_123:
        if (data_zhuyin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        if (data_zhuyin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        if (data_zhuyin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - custom��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    BOOLEAN has_turn_page = FALSE;
    KERNEL_DATA_ZHUYIN_T * data_zhuyin_ptr = PNULL;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return  TRUE;
    }

    data_zhuyin_ptr = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data));
    if (KEY_STYLE_CUSTOM == data_ptr->vkb.first_key->style)
    {
        if (data_ptr->vkb.first_key->code > key_ptr->code)
        {
            if (data_zhuyin_ptr->zhuyin_match_result.count > data_zhuyin_ptr->cust_zhuyin_first + data_ptr->vkb.custom_cnt)
            {
                uint8 slide_cnt = data_ptr->vkb.first_key->code - key_ptr->code;
                uint8 next_page_cnt = data_zhuyin_ptr->zhuyin_match_result.count;

                next_page_cnt -= data_zhuyin_ptr->cust_zhuyin_first + data_ptr->vkb.custom_cnt;

                if (slide_cnt > next_page_cnt) slide_cnt = next_page_cnt;

                data_zhuyin_ptr->cust_zhuyin_first += slide_cnt;
                ThumbZhuyinSetCust(data_ptr);
            }

            has_turn_page = TRUE;
        }
        else if (data_ptr->vkb.first_key->code < key_ptr->code)
        {
            if (data_zhuyin_ptr->cust_zhuyin_first > 0)
            {
                uint8 slide_cnt = key_ptr->code - data_ptr->vkb.first_key->code;
                uint8 next_page_cnt = data_zhuyin_ptr->cust_zhuyin_first;

                if (slide_cnt > next_page_cnt) slide_cnt = next_page_cnt;

                data_zhuyin_ptr->cust_zhuyin_first -= slide_cnt;
                ThumbZhuyinSetCust(data_ptr);
            }

            has_turn_page = TRUE;
        }
    }

    if (!has_turn_page)
    {
        if (data_zhuyin_ptr->input_string_len > 0)
        {
            uint16 select_idx = data_zhuyin_ptr->cust_zhuyin_first;

            select_idx += key_ptr->code - VKEY_CUSTOM;

            if (select_idx < data_zhuyin_ptr->zhuyin_match_result.count)
                //&& ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index != select_idx)
            {
                //((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index = select_idx;
                data_zhuyin_ptr->cur_page = 0;
                ThumbZhuyinUpdate(data_ptr);
            }
        }
        else
        {
            CommitChar(data_ptr, key_ptr->text[0]);
            UpdateUdbCache(&data_zhuyin_ptr->engine, &(key_ptr->text[0]), 1, GUIIM_M_TP_ZHUYIN);
			//��text�ֶ�����ͬ����display_text�ֶ�
			key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));
        }
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return  TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand_cnt);
        ThumbZhuyinSetCand(data_ptr);
        break;

    case VKEY_PY_PAGEUP:

        if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->expand_zhuyin_page > 0)
        {
            ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->expand_zhuyin_page--;
            ThumbZhuyinSetZyCand(data_ptr);
        }

        break;

    case VKEY_PY_PAGEDOWN:

        if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result.count > ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->expand_zhuyin_page * data_ptr->vkb.py_candidate_cnt)
        {
            ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->expand_zhuyin_page++;
            ThumbZhuyinSetZyCand(data_ptr);
        }

        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_ZHUYIN_T* data_zhuyin_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return  TRUE;
    }
    data_zhuyin_ptr = (KERNEL_DATA_ZHUYIN_T*)(data_ptr->data);
    if (VKEY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_CANDIDATE + data_ptr->vkb.candidate_cnt)
    {
        int32 select_idx = key_ptr->code - VKEY_CANDIDATE;

        if (data_zhuyin_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            uint8 confirm_digit_len = 0;
            uint16 i = 0;
            wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
            uint32 buffer_len = 0;

            data_zhuyin_ptr->confirm[data_zhuyin_ptr->confirm_cnt++] = data_zhuyin_ptr->cand[select_idx];

            //���������ѡ��֮���Ƿָ�������Ҫ��comfirm len���Ϸָ����ĳ��ȣ���1
            //ʵ���ϣ�ֻ�����һ���ָ����ᱻ���ԣ��м�ķָ�����������ƥ�䳤����
            for (i = 0; i < data_zhuyin_ptr->confirm_cnt; i++)
            {
                confirm_digit_len += data_zhuyin_ptr->confirm[i].match_length;

                if (KMX_PINYIN_SEPARATOR == data_zhuyin_ptr->input_string[confirm_digit_len]
                        && confirm_digit_len < data_zhuyin_ptr->input_string_len)
                {
                    confirm_digit_len++;
                }
            }


            //SCI_ASSERT(confirm_digit_len <= ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/

            if (confirm_digit_len < data_zhuyin_ptr->input_string_len)
            {
                data_zhuyin_ptr->cur_page = 0;
                //((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index = KMX_PINYIN_MATCH_INDEX_NONE;
                ThumbZhuyinUpdate(data_ptr);
            }
            else
            {
                //all match, commit
                for (i = 0; i < data_zhuyin_ptr->confirm_cnt; i++)
                {
                    SCI_MEMCPY(buffer + buffer_len, data_zhuyin_ptr->confirm[i].string,
                               data_zhuyin_ptr->confirm[i].length * sizeof(wchar));
                    buffer_len += data_zhuyin_ptr->confirm[i].length;
                }

                //SCI_ASSERT(buffer_len > 0); /*assert verified*/

                CommitStr(data_ptr, buffer, buffer_len);
                UpdateUdbCache(&data_zhuyin_ptr->engine, buffer, buffer_len, GUIIM_M_TP_ZHUYIN);

                ThumbZhuyinClear(data_ptr);
                if (buffer_len - 1 < KMX_MAX_PINYIN_INPUT)
                {
                    data_zhuyin_ptr->last_char = buffer[buffer_len - 1];
                }
                ThumbZhuyinAsso(data_ptr);
            }
        }
        else if (data_zhuyin_ptr->edit_state == EDIT_STATE_ASSO)
        {
            wchar last_char = data_zhuyin_ptr->cand[select_idx].string[0];

            CommitChar(data_ptr, last_char);
            UpdateUdbCache(&data_zhuyin_ptr->engine, &last_char, 1, GUIIM_M_TP_ZHUYIN);

            ThumbZhuyinClear(data_ptr);
            data_zhuyin_ptr->last_char = last_char;
            ThumbZhuyinAsso(data_ptr);
        }

    }
    else if (VKEY_PY_CANDIDATE <= key_ptr->code && key_ptr->code < VKEY_PY_CANDIDATE + data_ptr->vkb.py_candidate_cnt)
    {
        uint16 select_idx = data_zhuyin_ptr->expand_zhuyin_page * data_ptr->vkb.py_candidate_cnt;

        //SCI_ASSERT(((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->edit_state == EDIT_STATE_SEARCH); /*assert verified*/
        if(data_zhuyin_ptr->edit_state != EDIT_STATE_SEARCH)
        {
            return  TRUE;
        }
        
        select_idx += key_ptr->code - VKEY_PY_CANDIDATE;

        if (select_idx < data_zhuyin_ptr->zhuyin_match_result.count)
            //&& ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index != select_idx)
        {
            //((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index = select_idx;
            data_zhuyin_ptr->cur_page = 0;
            ThumbZhuyinUpdate(data_ptr);
        }
    }
    else
    {
        //SCI_ASSERT(0); /*assert verified*/
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: ThumbZhuyinClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    //�˴����쳣��timer id���ĵ�������timerû�������ر�
    if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer > 0)
    {
        MMK_StopTimer(((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer);
        ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->commit_timer = 0;
    }

    SCI_MEMSET((char*)(data_ptr->data) + sizeof(((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->engine), 0,
               sizeof(KERNEL_DATA_ZHUYIN_T) - sizeof(((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->engine));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand_cnt);
    ThumbZhuyinSetCand(data_ptr);
    ThumbZhuyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: ThumbZhuyinUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinUpdate(IM_DATA_T *data_ptr)
{
    uint8 confirm_digit_len = 0;
    uint16 i = 0;
    KMX_ZHUYIN_WORD_MATCH_RESULT match_result = {0};
    uint16 word_match_index = 0;
    uint8 cand_cnt = 0;
    uint8 has_next_page = 0;
    KERNEL_DATA_ZHUYIN_T *kernel_data_zhuyin_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    kernel_data_zhuyin_ptr = (KERNEL_DATA_ZHUYIN_T*)(data_ptr->data);
    kernel_data_zhuyin_ptr->edit_state = EDIT_STATE_SEARCH;

    /* search */
    kernel_data_zhuyin_ptr->cust_zhuyin_first = 0;

    for (i = 0; i < kernel_data_zhuyin_ptr->confirm_cnt; i++)
    {
        confirm_digit_len += kernel_data_zhuyin_ptr->confirm[i].match_length;
    }

    //SCI_ASSERT(confirm_digit_len < kernel_data_zhuyin_ptr->input_string_len); /*assert verified*/
    if(confirm_digit_len >= kernel_data_zhuyin_ptr->input_string_len)
    {
        return;
    }
    word_match_index = kernel_data_zhuyin_ptr->cur_page * data_ptr->vkb.candidate_cnt;

    do
    {
        int32 search_res = 0;
        int32 left_cnt = data_ptr->vkb.candidate_cnt - cand_cnt;
        uint8 cozy_cnt = 0;

        search_res = kmx_zhuyin_match(&(kernel_data_zhuyin_ptr->engine),
                                      kernel_data_zhuyin_ptr->input_string + confirm_digit_len,
                                      kernel_data_zhuyin_ptr->input_string_len - confirm_digit_len,
                                      //kernel_data_zhuyin_ptr->zhuyin_match_index,
                                      word_match_index,
                                      //&(kernel_data_zhuyin_ptr->zhuyin_composition),
                                      //&(kernel_data_zhuyin_ptr->zhuyin_match_result),
                                      &match_result);

        if (match_result.count < left_cnt)
        {
            cozy_cnt = match_result.count;
        }
        else if (match_result.count == left_cnt)
        {
            cozy_cnt = left_cnt;

            if (match_result.more_available != 0)
            {
                match_result.more_available = 0;
                has_next_page = 1;
            }
        }
        else
        {
            cozy_cnt = left_cnt;
            match_result.more_available = 0;
            has_next_page = 1;
        }

        if (0 == search_res || 0 == cozy_cnt)
        {
            break;
        }

        SCI_MEMCPY(kernel_data_zhuyin_ptr->cand + cand_cnt,
                   match_result.table,
                   cozy_cnt * sizeof(KMX_PINYIN_CANDIDATE));

        cand_cnt += cozy_cnt;
        word_match_index += match_result.count;
    }
    while (match_result.more_available != 0);


    if (cand_cnt > 0)
    {
        kernel_data_zhuyin_ptr->cand_cnt = cand_cnt;
        kernel_data_zhuyin_ptr->has_next_page = has_next_page;

        SetHideKey(data_ptr, kernel_data_zhuyin_ptr->cand_cnt);
        ThumbZhuyinSetCand(data_ptr);
        ThumbZhuyinDispEdit(data_ptr);
    }
    else
    {
        kernel_data_zhuyin_ptr->input_string_len--;
    }

    return;
}


/*==============================================================================
Description: ThumbZhuyinSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;

    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}

    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand_cnt)
    {
        goto ThumbZhuyinSetCandEnd;
    }
    
    //set normal candidate
    while (cand_cnt < ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        data_ptr->vkb.keys[cand_key_idx].text_len = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand[cand_cnt].length;
        SCI_MEMCPY(data_ptr->vkb.keys[cand_key_idx].text, ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand[cand_cnt].string,
                   ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand[cand_cnt].length * sizeof(wchar));
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));

        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
		
        cand_key_idx++;
        cand_cnt++;
    }


    if (1 == data_ptr->vkb.is_expand)
    {
        ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand_cnt = cand_cnt;

        if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);
		
        if (1 == ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
		
        ThumbZhuyinSetZyCand(data_ptr);
    }

ThumbZhuyinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
		
		//clear the text, maybe next time input cause any problem.
		data_ptr->vkb.keys[cand_key_idx].text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].text));  
		data_ptr->vkb.keys[cand_key_idx].print_text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].print_text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].print_text));
    }

    if (0 == data_ptr->vkb.is_expand)
    {
        ThumbZhuyinSetCust(data_ptr);
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
    uint32 buffer_len = 0;
    int16 i = 0;
    uint8 confirm_digit_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};

    CHECK_DATA_EX(data_ptr);

    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/
    if(!has_edit_layer)
    {
        return;
    }
    
    UILAYER_Clear(&data_ptr->edit_layer);

    if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    //����
    for (i = 0; i < ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->confirm_cnt; i++)
    {
        SCI_MEMCPY(buffer + buffer_len, ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->confirm[i].string,
                   ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->confirm[i].length * sizeof(wchar));
        buffer_len += ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->confirm[i].length;
        confirm_digit_len += ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->confirm[i].match_length;
    }

    //SCI_ASSERT(confirm_digit_len < ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/

    //SCI_ASSERT(confirm_digit_len <= ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/

    if(confirm_digit_len >= ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len)
    {
        return;
    }
    //����
    for (i = 0; i < ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len - confirm_digit_len; i++)
    {
        buffer[buffer_len++] = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string[confirm_digit_len + i];
    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len > edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


/*==============================================================================
Description: ThumbZhuyinSetCust
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetCust(IM_DATA_T *data_ptr)
{
    uint8 key_idx = 0;

    CHECK_DATA_EX(data_ptr);

    if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len > 0)
    {
        uint8 zhuyin_idx = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cust_zhuyin_first;
        uint8 zhuyin_left = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result.count - zhuyin_idx;

        if (zhuyin_left > data_ptr->vkb.custom_cnt)
        {
            zhuyin_left = data_ptr->vkb.custom_cnt;
        }

        for (key_idx = 0; key_idx < zhuyin_left; key_idx++)
        {
            int16 char_idx = 0;

            for (char_idx = 0; char_idx < ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result.table[zhuyin_idx + key_idx].length; char_idx++)
            {
                data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text[char_idx]
                = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result.table[zhuyin_idx + key_idx].string[char_idx];
            }

            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result.table[zhuyin_idx + key_idx].length;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT- 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}

        for (; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = 0;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 1;

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
        }
    }
    else
    {
        for (key_idx = 0; key_idx < data_ptr->vkb.custom_cnt; key_idx++)
        {
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text[0] = g_thumb_cust_cn_symb[key_idx];
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len = 1;
            data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT- 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx].print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.custom_first + key_idx], TRUE);
		}
    }

    return;
}


/*==============================================================================
Description: ThumbZhuyinSetZyCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetZyCand(IM_DATA_T *data_ptr)
{
    uint8 key_idx = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.py_pgup_key); /*assert verified*/
    //SCI_ASSERT(PNULL != data_ptr->vkb.py_pgdn_key); /*assert verified*/

    if(PNULL == data_ptr->vkb.py_pgup_key || PNULL == data_ptr->vkb.py_pgdn_key)
    {
        return;
    }
    
    if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->edit_state == EDIT_STATE_SEARCH)
    {
        uint8 zhuyin_idx = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->expand_zhuyin_page * data_ptr->vkb.py_candidate_cnt;
        uint8 zhuyin_left = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result.count - zhuyin_idx;

        //SCI_ASSERT(((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->input_string_len > 0); /*assert verified*/

        if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->expand_zhuyin_page > 0)
        {
            data_ptr->vkb.py_pgup_key->is_inactive = 0;
            data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE;
        }
        else
        {
            data_ptr->vkb.py_pgup_key->is_inactive = 1;
            data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE_TINT;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgup_key, TRUE);

        if (zhuyin_left > data_ptr->vkb.py_candidate_cnt)
        {
            zhuyin_left = data_ptr->vkb.py_candidate_cnt;
            data_ptr->vkb.py_pgdn_key->is_inactive = 0;
            data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE;
        }
        else
        {
            data_ptr->vkb.py_pgdn_key->is_inactive = 1;
            data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE_TINT;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgdn_key, TRUE);
		
        for (key_idx = 0; key_idx < zhuyin_left; key_idx++)
        {
            int16 char_idx = 0;

            for (char_idx = 0; char_idx < ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result.table[zhuyin_idx + key_idx].length; char_idx++)
            {
                data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text[char_idx]
                = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result.table[zhuyin_idx + key_idx].string[char_idx];
            }

            data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result.table[zhuyin_idx + key_idx].length;
            data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].is_inactive = 0;
			//��text�ֶ�����ͬ����display_text�ֶ�
			data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len = MIN (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len * sizeof (wchar));
			MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx], TRUE);
		}
    }
    else if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->edit_state == EDIT_STATE_ASSO)
    {
        data_ptr->vkb.py_pgup_key->is_inactive = 1;
        data_ptr->vkb.py_pgup_key->icon = IMG_IMT_LEFT_TRIANGLE_TINT;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgup_key, TRUE);
		
        data_ptr->vkb.py_pgdn_key->is_inactive = 1;
        data_ptr->vkb.py_pgdn_key->icon = IMG_IMT_RIGHT_TRIANGLE_TINT;
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.py_pgdn_key, TRUE);
    }

    for (; key_idx < data_ptr->vkb.py_candidate_cnt; key_idx++)
    {
        data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len = 0;
        data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].is_inactive = 1;
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len = data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len < PRINT_TEXT_LEN_LIMIT ? data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text_len : PRINT_TEXT_LEN_LIMIT;
		SCI_MEMCPY (data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].text, data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx].print_text_len * sizeof (wchar));
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[data_ptr->vkb.py_candidate_first + key_idx], TRUE);
	}

    return;
}


/*==============================================================================
Description: ThumbZhuyinAsso
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinAsso(IM_DATA_T *data_ptr)
{
    wchar buffer[CH_CANDIDATE_COUNT*ASSO_PAGE_LIMIT + 1] = {0};
    int32 offset_asso_cnt = 0;
    int32 asso_cnt = 0;
    int32 asso_idx = 0;

    CHECK_DATA_EX(data_ptr);

    ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->edit_state = EDIT_STATE_ASSO;

    offset_asso_cnt = ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cur_page * CH_CANDIDATE_COUNT;

    asso_cnt = kmx_chinese_query_association(GetKmxLdb_ptr(GUIIM_LANG_CHT, FALSE),
               ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->last_char, buffer,
               offset_asso_cnt + CH_CANDIDATE_COUNT + 1);

    if (asso_cnt <= offset_asso_cnt)
    {
        return;
    }

    ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->has_next_page = 0;

    if (asso_cnt > offset_asso_cnt + CH_CANDIDATE_COUNT)
    {
        if (((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cur_page + 1 < ASSO_PAGE_LIMIT)
        {
            ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->has_next_page = 1;
        }

        asso_cnt = CH_CANDIDATE_COUNT;
    }
    else
    {
        asso_cnt -= offset_asso_cnt;
    }

    for (asso_idx = 0; asso_idx < asso_cnt; asso_idx++)
    {
        ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand[asso_idx].length = 1;
        ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand[asso_idx].string[0] = buffer[offset_asso_cnt + asso_idx];
    }

    ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand_cnt = asso_cnt;

    SetHideKey(data_ptr, ((KERNEL_DATA_ZHUYIN_T*)(data_ptr->data))->cand_cnt);
    ThumbZhuyinSetCand(data_ptr);

    return;
}
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
/*==============================================================================
Description: QwertZhuyinInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinInit(IM_DATA_T *data_ptr)
{
    int32 init_res = 0;
    KMX_IME_ENGINE_INIT init_info = {0};

    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_Q_ZHUYIN_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_Q_ZHUYIN_T));

    QwertZhuyinInitKb(data_ptr);

    init_info.database = GetKmxLdb_ptr(GUIIM_LANG_CHT, FALSE);
    MMIIM_IME_FindUserCacheByIM(
        data_ptr->init_param.method_param_ptr->im_def_ptr->method,
        &init_info.user_data, &init_info.user_data_size);
    init_res = kmx_zhuyin_initialize(&(((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->engine), &init_info);
    MMIIM_SetImKerState(0);
    if (0 == init_res)
    {
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: QwertZhuyinTerm
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*==============================================================================
Description: QwertAbcDraw
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertZhuyinDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    QwertZhuyinDispEdit(data_ptr);

    return;
}

#ifdef IM_TRAD_CHINESE_QWERT_40KEY
/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void QwertZhuyinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 const CTRL_CNT = 9;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT_E;
    uint8 const SYS_CNT = 2;
    uint8 const CHAR_CNT = 40;		//char for engine key count.
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;
    MMIIM_SP_26KEY_KEYCHAR_T *cur_char_ptr = PNULL;
    QWERT_KEYBOARD_LAYOUT_TYPE_E layout = KEYBOARD_LAYOUT_40CHAR;			//KEYBOARD_LAYOUT_26CHAR;
    uint16 const *keymap_ptr = PNULL;
    GUI_RECT_T const *kb_pos_ptr = PNULL;
    MMI_STRING_T slide_cue = {PNULL, 0};

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_qwert_default;
	data_ptr->theme.key_ver_space = 30;		//vertical space, should not use 50, but use 30 %%,because add a line.

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = QwertKbGetKeyCnt (layout);		//g_qwert_panel_position_size; some customers like this layout
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 40
    //character key ��λ��������2��ʼ��27
    data_ptr->vkb.charactor_first = key_idx = 0;
    keymap_ptr = QwertKbGetKeyMap_ptr (layout);		//some customers like this layout
    kb_pos_ptr = QwertKbGetPosition_ptr (layout);

    //key_table_ptr = MMIIM_SP_ML26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);    //�˱��˳���ǰ�a~z����
    //if (key_table_ptr == PNULL)
    {
        key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    }

    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;    //������ʾ�׸��ַ�

        //=========��������˿ӡ����
        cur_char_ptr = key_table_ptr + (keymap_ptr[key_idx] - VKEY_A);
        vkb_ptr->keys[key_idx].text_len = cur_char_ptr->key_chars[0];
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr->key_chars[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertZhuyinCharLong;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key ��λ��������28��ʼ������2��
    key_idx++;
    cust_begin = key_idx;

    for (; key_idx < cust_begin + CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].text[0] = L"\xff1f\xff0c"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //control * 6,  caps, del, 123symbol, space, set, enter
    //character key ��λ�������ӿ�ʼ��30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertZhuyinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinCtrlUp;
    }

    //--ƴ���ָ��, ������
    //--���ü���������Ϊperiod ��Ӧ���ַ���
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_DOT;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;    //������ʾ�׸��ַ�
    vkb_ptr->keys[key_idx].text_len = 1;
    vkb_ptr->keys[key_idx].text[0] = 0x3126;
    //vkb_ptr->keys[key_idx].text[1] = 0x3125;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = QwertZhuyinCharUp;
    vkb_ptr->keys[key_idx].has_tip = TRUE;

    //--delete
    key_idx ++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--space
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_Q;
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_PINYIN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);		//needn't space, data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinSysUp;
    }

    //--
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;


    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(kb_pos_ptr[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = slide_cue.wstr_len < TEXT_LEN_LIMIT - 1? slide_cue.wstr_len : TEXT_LEN_LIMIT - 1;
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
        
    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = kb_pos_ptr[0].right - kb_pos_ptr[0].left + 1;
        int16 h = kb_pos_ptr[0].bottom - kb_pos_ptr[0].top + 1;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = kb_pos_ptr[0].top + y * h;
        cand_rect.bottom = kb_pos_ptr[0].bottom + y * h;
        cand_rect.left = kb_pos_ptr[0].left + x * w / CH_CANDIDATE_COL;
        cand_rect.right = kb_pos_ptr[0].left + (x + 1) * w / CH_CANDIDATE_COL - 1;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

    return;
}
#else
/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void QwertZhuyinInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 const CHAR_CNT = 26;
    uint8 const CTRL_CNT = 9;
    uint8 const CAND_CNT = CH_CANDIDATE_COUNT_E;
    uint8 const SYS_CNT = 2;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;
    MMIIM_SP_26KEY_KEYCHAR_T *cur_char_ptr = PNULL;
    MMI_STRING_T slide_cue = {PNULL, 0};

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_qwert_default;

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_qwert_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 26
    //character key ��λ��������2��ʼ��27
    data_ptr->vkb.charactor_first = key_idx = 0;

    key_table_ptr = MMIIM_SP_ML26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);    //�˱��˳���ǰ�a~z����
    if (key_table_ptr == PNULL)
    {
        key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    }

    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;    //������ʾ�׸��ַ�

        //=========��������˿ӡ����
        cur_char_ptr = key_table_ptr + (g_kb_keymap[key_idx] - VKEY_A);
        vkb_ptr->keys[key_idx].text_len = cur_char_ptr->key_chars[0];
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr->key_chars[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertZhuyinCharLong;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key ��λ��������28��ʼ������2��
    key_idx++;
    cust_begin = key_idx;

    for (; key_idx < cust_begin + CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].text[0] = L"\xff1f\xff0c"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //control * 6,  caps, del, 123symbol, space, set, enter
    //character key ��λ�������ӿ�ʼ��30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertZhuyinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinCtrlUp;
    }

    //--ƴ���ָ��
    //--���ü���������Ϊperiod ��Ӧ���ַ���
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_DOT;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;    //������ʾ�׸��ַ�
    vkb_ptr->keys[key_idx].text_len = 2;
    vkb_ptr->keys[key_idx].text[0] = 0x3121;
    vkb_ptr->keys[key_idx].text[1] = 0x3125;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = QwertZhuyinCharLong;
    vkb_ptr->keys[key_idx].f_up = QwertZhuyinCharUp;
    vkb_ptr->keys[key_idx].has_tip = TRUE;

    //--delete
    key_idx ++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--space
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_Q;
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_PINYIN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_qwert_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);		//needn't space, data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinSysUp;
    }

    //--
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;


    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_qwert_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
        
    //candidate
    //��ѡ����λ��ȷ����������ųɾ���
    //����δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        int16 x = (key_idx - cand_begin) % CH_CANDIDATE_COL;
        int16 y = (key_idx - cand_begin) / CH_CANDIDATE_COL;
        int16 w = g_qwert_panel_position[0].right - g_qwert_panel_position[0].left + 1;
        int16 h = g_qwert_panel_position[0].bottom - g_qwert_panel_position[0].top + 1;
        GUI_RECT_T cand_rect = {0};

        cand_rect.top = g_qwert_panel_position[0].top + y * h;
        cand_rect.bottom = g_qwert_panel_position[0].bottom + y * h;
        cand_rect.left = g_qwert_panel_position[0].left + x * w / CH_CANDIDATE_COL;
        cand_rect.right = g_qwert_panel_position[0].left + (x + 1) * w / CH_CANDIDATE_COL - 1;

        ConvertRect(panel_width, panel_height,
                    &cand_rect,
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);

        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertZhuyinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);

    return;
}
#endif

/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 input_letter_index = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    //SCI_ASSERT(((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len <= KMX_MAX_LATIN_WORD_LENGTH); /*assert verified*/
    if(PNULL == key_ptr || ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len > KMX_MAX_LATIN_WORD_LENGTH)
    {
        return  TRUE;
    }
    //
    if (data_ptr->vkb.state & KEYBOARD_CHOOSE_LETTER)
    {
        input_letter_index = GetLetterSelectIndex();
        if (input_letter_index >= 0
                && ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len < KMX_MAX_ZHUYIN_INPUT
                && ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->confirm_cnt < ZHUYIN_CONFIRM_LIMIT)
        {
            ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string[((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len++] = key_ptr->text[GetLetterSelectIndex()];
            //((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index = KMX_PINYIN_MATCH_INDEX_NONE;
            ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cur_page = 0;
            QwertZhuyinUpdate(data_ptr);
        }

        SetLetterSelectIndex(-1);
        VkbUp(data_ptr, key_ptr);
        data_ptr->vkb.state &= (~KEYBOARD_CHOOSE_LETTER);
        data_ptr->vkb.is_key_block = FALSE; //��������

        return TRUE;
    }

    if (((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len < KMX_MAX_ZHUYIN_INPUT
            && ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->confirm_cnt < ZHUYIN_CONFIRM_LIMIT)
    {
        ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string[((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len++] = key_ptr->text[0];
        //((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index = KMX_PINYIN_MATCH_INDEX_NONE;
        ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cur_page = 0;
        QwertZhuyinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}

/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - character��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    return ChooseLetter(data_ptr, key_ptr);
}


/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return  TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len > 0)
        {
            QwertZhuyinClear(data_ptr);
        }
        else
        {
            GUIIM_NOTIFY_PACK_T notify_pack = {0};

            notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

            GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
        }

        break;

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_Q_ZHUYIN_T* data_zhuying_ptr = PNULL;
        
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return  TRUE;
    }

    data_zhuying_ptr = (KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data);
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (data_zhuying_ptr->confirm_cnt > 0)
        {
            data_zhuying_ptr->confirm_cnt--;
            data_zhuying_ptr->cur_page = 0;
            QwertZhuyinUpdate(data_ptr);
        }
        else if (data_zhuying_ptr->input_string_len > 1)
        {
            data_zhuying_ptr->input_string_len--;
            data_zhuying_ptr->cur_page = 0;
            QwertZhuyinUpdate(data_ptr);
        }
        else if (data_zhuying_ptr->input_string_len == 1)
        {
            QwertZhuyinClear(data_ptr);
        }
        else if (data_zhuying_ptr->cand_cnt > 0)
        {
            QwertZhuyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:

        if (data_zhuying_ptr->input_string_len > 0)
        {
            wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
            int32 buffer_len = 0;
            int32 str_idx = 0;

            for (str_idx = 0; str_idx < data_zhuying_ptr->input_string_len; str_idx++)
            {
                buffer[buffer_len] = data_zhuying_ptr->input_string[str_idx];
                buffer_len++;
            }

            CommitStr(data_ptr, buffer, buffer_len);
            UpdateUdbCache(&data_zhuying_ptr->engine, buffer, buffer_len, GUIIM_M_TPQ_ZHUYIN);
            QwertZhuyinClear(data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\n');
        }

        break;

    case VKEY_SPACE:

        if (data_zhuying_ptr->input_string_len == 0)
        {
            CommitChar(data_ptr, 0x3000);
        }
		else
		{
			if (data_zhuying_ptr->cand_cnt > 0)
	        {
	            CapsCommitStr(data_ptr, data_zhuying_ptr->cand[0].string, data_zhuying_ptr->cand[0].length);
	            UpdateUdbCache(&data_zhuying_ptr->engine, 
	                    data_zhuying_ptr->cand[0].string, 
	                    data_zhuying_ptr->cand[0].length, 
	                    GUIIM_M_TP_ZHUYIN);

				//clear input
		        QwertZhuyinClear(data_ptr);
	        }
		}
		

        break;

    case VKEY_PAGEUP:
        //SCI_ASSERT(((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cur_page > 0); /*assert verified*/
        if(data_zhuying_ptr->cur_page > 0)
        {
            data_zhuying_ptr->cur_page--;
        }

        if (data_zhuying_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            QwertZhuyinUpdate(data_ptr);
        }
        else if (data_zhuying_ptr->edit_state == EDIT_STATE_ASSO)
        {
            QwertZhuyinAsso(data_ptr);
        }

        break;

    case VKEY_PAGEDOWN:
        //SCI_ASSERT(1 == ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->has_next_page); /*assert verified*/
        if(1 != data_zhuying_ptr->has_next_page)
        {
            break;
        }
        data_zhuying_ptr->cur_page++;

        if (data_zhuying_ptr->edit_state == EDIT_STATE_SEARCH)
        {
            QwertZhuyinUpdate(data_ptr);
        }
        else if (data_zhuying_ptr->edit_state == EDIT_STATE_ASSO)
        {
            QwertZhuyinAsso(data_ptr);
        }

        break;

    case VKEY_123SYMBOL:
        if (data_zhuying_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_FULL, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt);
        QwertZhuyinSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 select_idx = 0;
    uint8 confirm_digit_len = 0;
    uint16 i = 0;
    wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
    uint32 buffer_len = 0;
    KERNEL_DATA_Q_ZHUYIN_T* data_zhuyin_ptr = PNULL;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    select_idx = key_ptr->code - VKEY_CANDIDATE;
    data_zhuyin_ptr = ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data));

    //SCI_ASSERT(select_idx >= 0); /*assert verified*/
    //SCI_ASSERT(select_idx < ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt); /*assert verified*/
    if(select_idx < 0 || select_idx >= data_zhuyin_ptr->cand_cnt)
    {
        return TRUE;
    }

    if (data_zhuyin_ptr->edit_state == EDIT_STATE_SEARCH)
    {
        data_zhuyin_ptr->confirm[data_zhuyin_ptr->confirm_cnt++] = data_zhuyin_ptr->cand[select_idx];

        for (i = 0; i < data_zhuyin_ptr->confirm_cnt; i++)
        {
            confirm_digit_len += data_zhuyin_ptr->confirm[i].match_length;

            if (PINYIN_SEPARATOR == data_zhuyin_ptr->input_string[confirm_digit_len]
                    && confirm_digit_len < data_zhuyin_ptr->input_string_len)
            {
                confirm_digit_len++;
            }
        }

        //SCI_ASSERT(confirm_digit_len <= ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/

        if (confirm_digit_len < data_zhuyin_ptr->input_string_len)
        {
            data_zhuyin_ptr->cur_page = 0;
            QwertZhuyinUpdate(data_ptr);
        }
        else
        {
            //all match, commit
            for (i = 0; i < data_zhuyin_ptr->confirm_cnt; i++)
            {
                SCI_MEMCPY(buffer + buffer_len, data_zhuyin_ptr->confirm[i].string,
                           data_zhuyin_ptr->confirm[i].length * sizeof(wchar));
                buffer_len += data_zhuyin_ptr->confirm[i].length;
            }

            //SCI_ASSERT(buffer_len > 0); /*assert verified*/

            CommitStr(data_ptr, buffer, buffer_len);
            UpdateUdbCache(&data_zhuyin_ptr->engine, buffer, buffer_len, GUIIM_M_TPQ_ZHUYIN);
            QwertZhuyinClear(data_ptr);
            if (buffer_len - 1 < KMX_MAX_PINYIN_INPUT)
            {
                data_zhuyin_ptr->last_char = buffer[buffer_len - 1];
            }
            QwertZhuyinAsso(data_ptr);
        }
    }
    else if (data_zhuyin_ptr->edit_state == EDIT_STATE_ASSO)
    {
        wchar last_char = data_zhuyin_ptr->cand[select_idx].string[0];

        CommitChar(data_ptr, last_char);
        UpdateUdbCache(&data_zhuyin_ptr->engine, &last_char, 1, GUIIM_M_TPQ_ZHUYIN);

        QwertZhuyinClear(data_ptr);
        data_zhuyin_ptr->last_char = last_char;
        QwertZhuyinAsso(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: QwertZhuyinClear
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertZhuyinClear(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    if (data_ptr->is_symbol_panel) return;

    SCI_MEMSET((char*)(data_ptr->data) + sizeof(((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->engine), 0,
               sizeof(KERNEL_DATA_Q_ZHUYIN_T) - sizeof(((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->engine));

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey(data_ptr, ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt);
    QwertZhuyinSetCand(data_ptr);
    QwertZhuyinDispEdit(data_ptr);

    return;
}


/*==============================================================================
Description: QwertZhuyinUpdate
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertZhuyinUpdate(IM_DATA_T *data_ptr)
{
    uint8 confirm_digit_len = 0;
    uint16 i = 0;
    KMX_ZHUYIN_WORD_MATCH_RESULT match_result = {0};
    uint16 word_match_index = 0;
    uint8 cand_cnt = 0;
    uint8 has_next_page = 0;
    EDIT_STATE_T old_edit_state = EDIT_STATE_NONE;
    CHECK_DATA_EX(data_ptr);
    
    old_edit_state = ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->edit_state;
    ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->edit_state = EDIT_STATE_SEARCH;

    /* search */
    for (i = 0; i < ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->confirm_cnt; i++)
    {
        confirm_digit_len += ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->confirm[i].match_length;
    }

    //SCI_ASSERT(confirm_digit_len < ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len); /*assert verified*/
    if(confirm_digit_len >= ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len)
    {
        return;
    }
    
    word_match_index = ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cur_page * data_ptr->vkb.candidate_cnt;

    do
    {
        int32 search_res = 0;
        int32 left_cnt = data_ptr->vkb.candidate_cnt - cand_cnt;
        uint8 cozy_cnt = 0;

        search_res = kmx_zhuyin_match(&(((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->engine),
                                      ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string + confirm_digit_len,
                                      ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len - confirm_digit_len,
                                      //((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_index,
                                      word_match_index,
                                      //&(((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->zhuyin_composition),
                                      //&(((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->zhuyin_match_result),
                                      &match_result);

        if (match_result.count < left_cnt)
        {
            cozy_cnt = match_result.count;
        }
        else if (match_result.count == left_cnt)
        {
            cozy_cnt = left_cnt;

            if (match_result.more_available != 0)
            {
                match_result.more_available = 0;
                has_next_page = 1;
            }
        }
        else
        {
            cozy_cnt = left_cnt;
            match_result.more_available = 0;
            has_next_page = 1;
        }

        if (0 == search_res || 0 == cozy_cnt)
        {
            break;
        }

        SCI_MEMCPY(((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand + cand_cnt,
                   match_result.table,
                   cozy_cnt * sizeof(KMX_PINYIN_CANDIDATE));

        cand_cnt += cozy_cnt;
        word_match_index += match_result.count;
    }
    while (match_result.more_available != 0);

    if (cand_cnt > 0)
    {
        ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->edit_state = EDIT_STATE_SEARCH;
        ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt = cand_cnt;
        ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->has_next_page = has_next_page;

        SetHideKey(data_ptr, ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt);
        QwertZhuyinSetCand(data_ptr);
        QwertZhuyinDispEdit(data_ptr);
    }
    else
    {
        ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len--;
	 ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->edit_state = old_edit_state;
    }

    return;
}


/*==============================================================================
Description: QwertZhuyinSetCand
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertZhuyinSetCand(IM_DATA_T *data_ptr)
{
    uint16 cand_key_idx = 0;
    uint16 cand_cnt = 0;

    CHECK_DATA_EX(data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}

    cand_key_idx = data_ptr->vkb.candidate_first;

    if (0 == ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt)
    {
        goto QwertZhuyinSetCandEnd;
    }
    
    //set normal candidate
    while (cand_cnt < ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt && cand_cnt < data_ptr->vkb.candidate_cnt)
    {
        if (0 == data_ptr->vkb.is_expand
                && cand_cnt >= CH_CANDIDATE_COL)
        {
            break;
        }

        data_ptr->vkb.keys[cand_key_idx].text_len = ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand[cand_cnt].length;
        SCI_MEMCPY(data_ptr->vkb.keys[cand_key_idx].text, ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand[cand_cnt].string,
                   ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand[cand_cnt].length * sizeof(wchar));
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));

        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
        cand_key_idx++;
        cand_cnt++;
    }


    if (1 == data_ptr->vkb.is_expand)
    {
        ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt = cand_cnt;

        if (((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cur_page > 0)
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP;
            data_ptr->vkb.pgup_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgup_key->icon = IMG_IMT_PAGEUP_TINT;
            data_ptr->vkb.pgup_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgup_key, TRUE);

        if (1 == ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->has_next_page)
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN;
            data_ptr->vkb.pgdn_key->is_inactive = 0;
        }
        else
        {
            data_ptr->vkb.pgdn_key->icon = IMG_IMT_PAGEDOWN_TINT;
            data_ptr->vkb.pgdn_key->is_inactive = 1;
        }
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.pgdn_key, TRUE);
    }

QwertZhuyinSetCandEnd:

    //����ʣ�µĺ�ѡ��ť
    for (; cand_key_idx < data_ptr->vkb.candidate_first + data_ptr->vkb.candidate_cnt; cand_key_idx++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
		
		//clear the text, maybe next time input cause any problem.
		data_ptr->vkb.keys[cand_key_idx].text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].text));  
		data_ptr->vkb.keys[cand_key_idx].print_text_len = 0;
		SCI_MEMSET (data_ptr->vkb.keys[cand_key_idx].print_text, 0, sizeof (data_ptr->vkb.keys[cand_key_idx].print_text));
    }

    return;
}


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertZhuyinDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
    uint32 buffer_len = 0;
    int16 i = 0;
    uint8 confirm_digit_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};

    CHECK_DATA_EX(data_ptr);

    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/
    if(!has_edit_layer)
    {
        return;
    }
    
    UILAYER_Clear(&data_ptr->edit_layer);

    if (((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    //����
    for (i = 0; i < ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->confirm_cnt; i++)
    {
        SCI_MEMCPY(buffer + buffer_len, ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->confirm[i].string,
                   ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->confirm[i].length * sizeof(wchar));
        buffer_len += ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->confirm[i].length;
        confirm_digit_len += ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->confirm[i].match_length;
    }

    //��ĸ
    for (i = 0; i < ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string_len - confirm_digit_len; i++)
    {
        buffer[buffer_len++] = ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->input_string[confirm_digit_len + i];
    }

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len > edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


/*==============================================================================
Description: QwertZhuyinAsso
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void QwertZhuyinAsso(IM_DATA_T *data_ptr)
{
    wchar buffer[CH_CANDIDATE_COUNT_E*ASSO_PAGE_LIMIT + 1] = {0};
    int32 offset_asso_cnt = 0;
    int32 asso_cnt = 0;
    int32 asso_idx = 0;

    CHECK_DATA_EX(data_ptr);

    ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->edit_state = EDIT_STATE_ASSO;

    offset_asso_cnt = ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cur_page * CH_CANDIDATE_COUNT_E;

    asso_cnt = kmx_chinese_query_association(GetKmxLdb_ptr(GUIIM_LANG_CHT, FALSE),
               ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->last_char, buffer,
               offset_asso_cnt + CH_CANDIDATE_COUNT_E + 1);

    if (asso_cnt <= offset_asso_cnt)
    {
        return;
    }

    ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->has_next_page = 0;

    if (asso_cnt > offset_asso_cnt + CH_CANDIDATE_COUNT_E)
    {
        if (((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cur_page + 1 < ASSO_PAGE_LIMIT)
        {
            ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->has_next_page = 1;
        }

        asso_cnt = CH_CANDIDATE_COUNT_E;
    }
    else
    {
        asso_cnt -= offset_asso_cnt;
    }

    for (asso_idx = 0; asso_idx < asso_cnt; asso_idx++)
    {
        ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand[asso_idx].length = 1;
        ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand[asso_idx].string[0] = buffer[offset_asso_cnt + asso_idx];
    }

    ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt = asso_cnt;

    SetHideKey(data_ptr, ((KERNEL_DATA_Q_ZHUYIN_T*)(data_ptr->data))->cand_cnt);
    QwertZhuyinSetCand(data_ptr);

    return;
}
#endif


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCharLong

Func:   9���������뷨�������ַ���0 ~ 9��Ϣ�������Ϊ�����׸��ַ�ʱ��������ֱ������

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�İ��������ȼ��ð����Ƿ�Ϊ�ַ�����������������

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    if (((KERNEL_DATA_LATIN_T*)(data_ptr->data))->input_string_len > 0)
    {
        return TRUE;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        CommitChar(data_ptr, '1' + (key_ptr->code - VKEY_1));
    }
    else if (VKEY_0 == key_ptr->code)
    {
        CommitChar(data_ptr, '0');
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartInitKb

Func:   9���������뷨���̳�ʼ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 8;
    uint8 const SYS_CNT = 2;
    uint8 const CAND_CNT = LATIN_CANDIDATE_COUNT;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;
    MMI_STRING_T slide_cue = {PNULL, 0};
    uint16 key_max_char_num = 0;
    
    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;
    SetPanel(data_ptr);

    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�
    vkb_ptr->key_cnt -= g_thumb_panel_expand_py_cnt; //no expand py

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].text[0] = ",.?!-"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //deal tag
    if (GUIIM_TAG_EMAIL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '@';
        vkb_ptr->keys[cust_begin+2].text[0] = '-';
        vkb_ptr->keys[cust_begin+3].text[0] = '_';
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }
    else if (GUIIM_TAG_URL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '/';
        vkb_ptr->keys[cust_begin+2].text_len = 7;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+2].text, L"http://", vkb_ptr->keys[cust_begin+2].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+3].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+3].text, L"www.", vkb_ptr->keys[cust_begin+3].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_EXCLUDE_DIGIT;        //
        //=========��������˿ӡ����
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        key_max_char_num = key_max_char_num > vkb_ptr->keys[key_idx].text_len ? key_max_char_num : vkb_ptr->keys[key_idx].text_len;
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;

        vkb_ptr->keys[key_idx].f_long = ThumbSmartCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //��������ַ�����5���İ������������������ʾ
    if (key_max_char_num > KEY_CHAR_LINE_MAX_CHARS_NUM + 1)       //���ּ�Ҳ������,����Ϊ5������4
    {
        int32 iloop = 0;
        
        for (iloop = 0; iloop < CHAR_CNT; iloop++)
        {
            vkb_ptr->keys[key_idx - iloop - 1].add_style.text_disp = KASC_2X4_EXCLUDE_DIGIT;         //key_idxֵ����󱻼�1����Ҫ -1
        }
    }

    //--1�������ã��������Ϊһ���ַ�'1'����ֱ������
    ThumbSmartRedefineKey1 (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &vkb_ptr->keys[char_begin]);
        
    //--0 ���һ��char
    key_idx--;
    ThumbSmartRedefineKey0 (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &vkb_ptr->keys[key_idx]);
    vkb_ptr->keys[key_idx].f_up = ThumbSmartCommitDefaultCharUp;	//Ĭ������Ϊ�ύ�׸���ѡ�ʣ�Ȼ������ո�
	
    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbSmartCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--Caps or Multi-char
    key_idx++;
    if (MMIAPIIM_CapsExist(data_ptr->init_param.method_param_ptr->im_def_ptr->lang))
    {
        vkb_ptr->keys[key_idx].code = VKEY_CAPS;
        vkb_ptr->keys[key_idx].icon = IMG_IMT_CAP_L;
        //�ر�ļ�סcaps key
        vkb_ptr->caps_key = vkb_ptr->keys + key_idx;
    }
    else
    {
        ThumbSmartRedefineCaps (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &vkb_ptr->keys[key_idx]);
    }
    
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_EN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }
    
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartSysUp;
    }

    //--hide
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--expand
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
        
    //candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //set cap, always init as lower
    if (data_ptr->vkb.cap != CAPITTAL_MODE_LOWER)
    {
        CAPITTAL_MODE_T back_cap = data_ptr->vkb.cap;

        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        SetVkbCaps(data_ptr, back_cap);
    }
	
	{
		//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
		MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);		//�����SetVkbCaps�Ѿ����ù��ú���
		MMIIM_TOUCH_ThumbKbRedefinePrintText (data_ptr);	//��Щ���Ե�˿ӡ�Ͱ�������ʾ��һ��
	}

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   SmartInitEngine

Func:   �������뷨�����ʼ��

Input:
            kmx_save_ptr:    �����ʼ���������뷨����ָ��
            lang:   ��ǰ���뷨����
Output:
            kmx_save_ptr:   �����ʼ���������뷨����ָ��

Return:
            TRUE:�ɹ���FALSE:�������Ϸ���ʧ��

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN SmartInitEngine(KMX_IME_ENGINE *kmx_save_ptr, GUIIM_LANGUAGE_T lang, GUIIM_METHOD_T method)
{
    int32 init_res = 0;
    KMX_IME_ENGINE_INIT init_info = {0};

    if (kmx_save_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"SmartInitEngine"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_CSTAR_9124_112_2_18_2_27_47_110,(uint8*)"");
        return FALSE;
    }

    SCI_MEMSET(&init_info, 0, sizeof(KMX_IME_ENGINE_INIT));
    init_info.database = GetKmxLdb_ptr(lang, FALSE);
    MMIIM_IME_FindUserCacheByIM(method,
        &init_info.user_data, &init_info.user_data_size);
    init_res = kmx_latin_initialize(kmx_save_ptr, &init_info);
    MMIIM_SetImKerState(0);
    if (0 == init_res)
    {
        //SCI_TRACE_LOW:"SmartInitEngine"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_CSTAR_9133_112_2_18_2_27_47_111,(uint8*)"");
        return FALSE;
    }

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartInit

Func:   9���������뷨��ʼ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return:
            TRUE:�ɹ���FALSE:�������Ϸ���ʧ��

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartInit(IM_DATA_T *data_ptr)
{
    if (data_ptr == PNULL
            || data_ptr->init_param.method_param_ptr == PNULL
            || data_ptr->init_param.method_param_ptr->im_def_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"ThumbSmartInit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_CSTAR_9166_112_2_18_2_27_47_112,(uint8*)"");
        return FALSE;
    }

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_LATIN_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_LATIN_T));

    //���̳�ʼ��
    ThumbSmartInitKb(data_ptr);

    //���ô�Сд
    SmartSetMatch(data_ptr);

    //�����ʼ��
    SmartInitEngine(&(((KERNEL_DATA_LATIN_T*)(data_ptr->data))->engine), 
        data_ptr->init_param.method_param_ptr->im_def_ptr->lang,
        data_ptr->init_param.method_param_ptr->im_def_ptr->method);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartTerm

Func:   9���������뷨�ر�

Input:
            data_ptr:    ���뷨ָ��

Output:

Return:
            TRUE:�ɹ���FALSE:�������Ϸ���ʧ��

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartDraw

Func:   9���������뷨����

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);
    ThumbSmartDispEdit(data_ptr);
    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCommitDefaultCharUp

Func:   ���ύĬ�ϵĺ�ѡ�֣��ٴ�������������ǰ����CSTAR��0�����ڸ�case������case�������������

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�İ��������ȼ��ð����Ƿ�Ϊ�ַ�����������������

Create: yaoguangchen@spread. 2012-10-26

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCommitDefaultCharUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
	KERNEL_DATA_LATIN_T* data_latin_ptr = PNULL;

	data_latin_ptr = ((KERNEL_DATA_LATIN_T*)(data_ptr->data));
    if(PNULL == key_ptr || data_latin_ptr == PNULL || data_latin_ptr->input_string_len > KMX_MAX_LATIN_WORD_LENGTH)
    {
        return TRUE;
    }
	
	if (VKEY_0 == key_ptr->code)
    {
        if (data_latin_ptr->input_string_len > 0)
        {
            //commit candidate first
            if (data_latin_ptr->cand_cnt > 0)
            {
                CapsCommitStr(data_ptr, data_latin_ptr->cand[0].string, data_latin_ptr->cand[0].length);
                UpdateUdbCache(&data_latin_ptr->engine, 
                    data_latin_ptr->cand[0].string, 
                    data_latin_ptr->cand[0].length, 
                    GUIIM_M_TP_ENGLISH);
            }

            //clear input
            ThumbSmartClear(data_ptr);
        }

        //commit a space
        CommitChar (data_ptr, ' ');
    }

    VkbUp(data_ptr, key_ptr);
	return TRUE;
}

/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCharUp

Func:   9���������뷨���ַ���0 ~ 9��Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�İ��������ȼ��ð����Ƿ�Ϊ�ַ�����������������

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_LATIN_T* data_latin_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/

    data_latin_ptr = ((KERNEL_DATA_LATIN_T*)(data_ptr->data));
    //SCI_ASSERT(((KERNEL_DATA_LATIN_T*)(data_ptr->data))->input_string_len <= KMX_MAX_LATIN_WORD_LENGTH); /*assert verified*/
    if(PNULL == key_ptr || data_latin_ptr->input_string_len > KMX_MAX_LATIN_WORD_LENGTH)
    {
        return TRUE;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        if (data_latin_ptr->input_string_len < KMX_MAX_LATIN_WORD_LENGTH)
        {
            data_latin_ptr->input_string[data_latin_ptr->input_string_len++] = '1' + (key_ptr->code - VKEY_1);
            ThumbSmartUpdate(data_ptr);
        }
    }
    else if (VKEY_0 == key_ptr->code)
    {
	    //��initkbʱ�����Ϊ�ո��0������״̬�����Ĭ�ϵ���ThumbSmartRedefineKey0����multi-tap-char������
		//��Ȼ���������Ӧ�ñ�ʾ�ü��������������ַ��������뷨�������
        if (data_latin_ptr->input_string_len < KMX_MAX_LATIN_WORD_LENGTH)
        {
            data_latin_ptr->input_string[data_latin_ptr->input_string_len++] = '0';
            ThumbSmartUpdate(data_ptr);
        }
#if 0
        if (data_latin_ptr->input_string_len > 0)
        {
            //commit candidate first
            if (data_latin_ptr->cand_cnt > 0)
            {
                CapsCommitStr(data_ptr, data_latin_ptr->cand[0].string, data_latin_ptr->cand[0].length);
                UpdateUdbCache(&data_latin_ptr->engine, 
                    data_latin_ptr->cand[0].string, 
                    data_latin_ptr->cand[0].length, 
                    GUIIM_M_TP_ENGLISH);
            }

            //clear input
            ThumbSmartClear(data_ptr);
        }

        //commit a space
        CommitChar (data_ptr, ' ');
#endif		
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCtrlLong

Func:   9���������뷨�����Ƽ�������Ϣ����,�˴���ʱֱ�ӵ���

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:
        {
            if (((KERNEL_DATA_LATIN_T*)(data_ptr->data))->input_string_len > 0)
            {
                ThumbLatinClear(data_ptr);
            }
            else
            {
                GUIIM_NOTIFY_PACK_T notify_pack = {0};

                notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

                GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
            }

            break;
        }

    default:
        break;
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCtrlUp

Func:   9���������뷨�����Ƽ���Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_LATIN_T* data_latin_ptr = PNULL;
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }

    data_latin_ptr = ((KERNEL_DATA_LATIN_T*)(data_ptr->data));
    switch (key_ptr->code)
    {
    case VKEY_CAPS:
        SetVkbNextCaps(data_ptr);
        SmartSetMatch(data_ptr);

        if (data_latin_ptr->input_string_len > 0)
        {
            ThumbSmartUpdate(data_ptr);
        }

        break;

    case VKEY_DEL:

        if (data_latin_ptr->input_string_len > 1)
        {
            data_latin_ptr->input_string_len--;
            data_latin_ptr->cur_page = 0;
            ThumbLatinUpdate(data_ptr);
        }
        else if (data_latin_ptr->input_string_len == 1)
        {
            ThumbSmartClear(data_ptr);
        }
        else
        {
            CommitChar (data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:

        if (data_latin_ptr->input_string_len > 0)
        {
            wchar buffer[KMX_MAX_LATIN_WORD_LENGTH] = {0};
            int32 buffer_len = 0;

            while (buffer_len < data_latin_ptr->input_string_len)
            {
                buffer[buffer_len] = data_latin_ptr->input_string[buffer_len];
                buffer_len++;
            }

            CapsCommitStr (data_ptr, buffer, buffer_len);
            UpdateUdbCache(&data_latin_ptr->engine, buffer, buffer_len, GUIIM_M_TP_ENGLISH);

            ThumbSmartClear(data_ptr);
        }
        else
        {
            CommitChar (data_ptr, '\n');
        }

        break;

    case VKEY_PAGEUP:
        //SCI_ASSERT(((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cur_page > 0); /*assert verified*/
        if(data_latin_ptr->cur_page > 0)
        {
            data_latin_ptr->cur_page--;
        }
        ThumbSmartUpdate(data_ptr);
        break;

    case VKEY_PAGEDOWN:
        //SCI_ASSERT(1 == ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->has_next_page); /*assert verified*/
        if(1 != data_latin_ptr->has_next_page)
        {
            break;
        }
        data_latin_ptr->page[data_latin_ptr->cur_page + 1]
        = data_latin_ptr->page[data_latin_ptr->cur_page]
          + data_latin_ptr->cand_cnt;
        data_latin_ptr->cur_page++;
        ThumbSmartUpdate(data_ptr);
        break;

    case VKEY_123:
        if (data_latin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        if (data_latin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        if (data_latin_ptr->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCustUp

Func:   9���������뷨���û��Զ��尴����Ϣ�����˴�Ĭ�϶���Ϊ�������밴��

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ���û��Զ��尴�����������ļ������ż�

Return: TRUE:success;  else fail;

Create: yaoguangchen@spread. 2011-10-04
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    ThumbAbcCommitLast(data_ptr);

    CapsCommitStr (data_ptr, key_ptr->text, key_ptr->text_len);

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartSysUp

Func:   9���������뷨��ϵͳ������Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ��ϵͳ����

Return: TRUE:success;  else fail;

Create: yaoguangchen@spread. 2011-10-04
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    switch (key_ptr->code)
    {
    case VKEY_EXPAND:
        SwitchExpandKey(data_ptr);
        SetHideKey(data_ptr, ((KERNEL_DATA_LATIN_T*)(data_ptr->data))->cand_cnt);
        ThumbSmartSetCand(data_ptr);
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCandUp

Func:   9���������뷨����ѡ�ֵ����Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�ĺ�ѡ�ְ���

Return: TRUE:success;  else fail;

Create: yaoguangchen@spread. 2011-10-04
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    ThumbLatinCandUp(data_ptr, key_ptr);
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   SmartSetMatch

Func:   9���������뷨�������Сд�л����ݸ���

Input:
            data_ptr:    ���뷨ָ��

Return: TRUE:success;  else fail;

Create: yaoguangchen@spread. 2011-10-04
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void SmartSetMatch(IM_DATA_T *data_ptr)
{
    ThumbLatinSetMatch(data_ptr);
    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartClear

Func:   9���������뷨�������������

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartClear(IM_DATA_T *data_ptr)
{
    ThumbLatinClear(data_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartUpdate

Func:   9���������뷨������ˢ��

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartUpdate(IM_DATA_T *data_ptr)
{
    ThumbLatinUpdate(data_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartSetCand

Func:   9���������뷨�����ú�ѡ��

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartSetCand(IM_DATA_T *data_ptr)
{
    ThumbLatinSetCand(data_ptr);
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartDispEdit

Func:   9���������뷨����ǰ��������ַ�����ʾ

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartDispEdit(IM_DATA_T *data_ptr)
{
    ThumbLatinDispEdit(data_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartInit

Func:   26���������뷨��ʼ��

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartInit(IM_DATA_T *data_ptr)
{
    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_Q_LATIN_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_Q_LATIN_T));

    QwertSmartInitKb(data_ptr);
    SmartSetMatch(data_ptr);
    SmartInitEngine(&(((KERNEL_DATA_LATIN_T*)(data_ptr->data))->engine), 
        data_ptr->init_param.method_param_ptr->im_def_ptr->lang,
        data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartTerm

Func:   26���������뷨�ر�

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartTerm(IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    //release engine
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartDraw

Func:   26���������뷨����

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartDraw(IM_DATA_T * data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartInitKb

Func:   26���������뷨��ʼ���������

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 2;
    uint8 CHAR_CNT = 26;
    uint8 const CTRL_CNT = 9;
    uint8 const CAND_CNT = LATIN_CANDIDATE_COUNT;
    uint8 const SYS_CNT = 2;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr = PNULL;
    MMIIM_SP_26KEY_KEYCHAR_T *cur_char_ptr = PNULL;
    QWERT_KEYBOARD_LAYOUT_TYPE_E layout = KEYBOARD_LAYOUT_26CHAR;
    uint16 const *keymap_ptr = g_kb_keymap;
    GUI_RECT_T const *kb_pos_ptr = g_qwert_panel_position;
    MMI_STRING_T slide_cue = {PNULL, 0};
    
    CHECK_DATA(data_ptr); 

    layout = QwertKbLayoutType (data_ptr->init_param.method_param_ptr->im_def_ptr->method, 0);
    data_ptr->theme = g_theme_qwert_default;        //QwertKbGetTheme (layout);
    SetPanel(data_ptr);
    
    ReleaseVkb(&data_ptr->vkb);
    InitVkbCaps(data_ptr);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = QwertKbGetKeyCnt (layout);
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //character * 26
    //character key ��λ��������2��ʼ��27
    data_ptr->vkb.charactor_first = key_idx = 0;
    CHAR_CNT = QwertKbGetCharKeyCnt (layout);
    keymap_ptr = QwertKbGetKeyMap_ptr (layout);
    kb_pos_ptr = QwertKbGetPosition_ptr (layout);
    key_table_ptr = MMIIM_SP_SMART26KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);    //�˱��˳���ǰ�a~z����
    for (; key_idx < CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].code = VKEY_A + key_idx;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_FIRST;    //������ʾ�׸��ַ�

        //=========��������˿ӡ����
        cur_char_ptr = key_table_ptr + (keymap_ptr[key_idx] - VKEY_A);
        vkb_ptr->keys[key_idx].text_len = cur_char_ptr->key_chars[0];
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr->key_chars[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));

        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertSmartCharLong;
        vkb_ptr->keys[key_idx].f_up = QwertSmartCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    //custom * 2
    //custom key ��λ��������28��ʼ������2��
    key_idx++;
    cust_begin = key_idx;

    //period����
    ConvertRect(panel_width, panel_height,
                &(kb_pos_ptr[key_idx + 2]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].text[0] = '.';
    vkb_ptr->keys[key_idx].text_len = 1;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
    vkb_ptr->keys[key_idx].has_tip = TRUE;
    //�п�����Ҫʹ���������õķ���
    QwertSmartConfigPeriod (&vkb_ptr->keys[key_idx], key_table_ptr);
    key_idx ++;

    //,        
    ConvertRect(panel_width, panel_height,
                &(kb_pos_ptr[key_idx + 2]),
                &(vkb_ptr->keys[key_idx].rect),
                data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].text[0] = ',';
    vkb_ptr->keys[key_idx].text_len = 1;
    vkb_ptr->keys[key_idx].f_down = VkbDown;
    vkb_ptr->keys[key_idx].f_move = VkbMove;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
    vkb_ptr->keys[key_idx].has_tip = TRUE;

    //key_idx --;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //control * 6,  caps, del, 123symbol, space, set, enter
    //character key ��λ�������ӿ�ʼ��30
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = QwertLatinCtrlLong;
        vkb_ptr->keys[key_idx].f_up = QwertLatinCtrlUp;
    }

    //--Caps
    key_idx = ctrl_begin;
    //�ر�ļ�סcaps key
    if (MMIAPIIM_CapsExist(data_ptr->init_param.method_param_ptr->im_def_ptr->lang))
    {
        vkb_ptr->caps_key = vkb_ptr->keys + key_idx;
        vkb_ptr->keys[key_idx].code = VKEY_CAPS;
        vkb_ptr->keys[key_idx].icon = IMG_IMT_CAP_L;
    }
    else
    {
        QwertSmartRedefineCaps (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &vkb_ptr->keys[key_idx]);
    }

    //--delete
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--space
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SPACE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SPACE_Q;
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_EN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }
    
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(kb_pos_ptr[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertLatinSysUp;
    }

    //--
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;


    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(kb_pos_ptr[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
       
    //candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = QwertLatinCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    //set cap, always init as lower
    if (data_ptr->vkb.cap != CAPITTAL_MODE_LOWER)
    {
        CAPITTAL_MODE_T back_cap = data_ptr->vkb.cap;

        data_ptr->vkb.cap = CAPITTAL_MODE_LOWER;
        SetVkbCaps(data_ptr, back_cap);
    }

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCharUp

Func:   26���������뷨�ַ�������Ϣ�����������潻���İ�����Ϣ
           key_ptr:    ��Ӧ���ַ�����

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    int32 input_letter_index = 0;

    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    //SCI_ASSERT(((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len <= KMX_MAX_LATIN_WORD_LENGTH); /*assert verified*/

    if(PNULL == key_ptr || ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len > KMX_MAX_LATIN_WORD_LENGTH )
    {
        return TRUE;
    }
    //
    if (data_ptr->vkb.state & KEYBOARD_CHOOSE_LETTER)
    {
        input_letter_index = GetLetterSelectIndex();
        if (input_letter_index >= 0 && ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len < KMX_MAX_LATIN_WORD_LENGTH)
        {
            ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string[((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len++] = key_ptr->text[input_letter_index];
            ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cur_page = 0;
            QwertLatinUpdate(data_ptr);
        }

        SetLetterSelectIndex(-1);
        VkbUp(data_ptr, key_ptr);
        data_ptr->vkb.state &= (~KEYBOARD_CHOOSE_LETTER);
        data_ptr->vkb.is_key_block = FALSE; //��������

        return TRUE;
    }

    if (((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len < KMX_MAX_LATIN_WORD_LENGTH)
    {
        ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string[((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->input_string_len++] = key_ptr->text[0];
        ((KERNEL_DATA_Q_LATIN_T*)(data_ptr->data))->cur_page = 0;
        QwertLatinUpdate(data_ptr);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCharLong

Func:   26���������뷨������Ϣ������ĳ�����������ַ�ѡ��ʱ����tips��ʾΪ��ĸѡ��
            ״̬���ȴ��û�ѡ����ĸ

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ���ַ�����

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    return ChooseLetter(data_ptr, key_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCtrlLong

Func:   26���������뷨���ư���������Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    return QwertLatinCtrlLong(data_ptr, key_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCtrlUp

Func:   26���������뷨���ư�����Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    return QwertLatinCtrlUp(data_ptr, key_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartSysUp

Func:   26���������뷨ϵͳ������Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ��ϵͳ����

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    return QwertLatinSysUp(data_ptr, key_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCandUp

Func:   26���������뷨��ѡ����Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ��ϵͳ����

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    return QwertLatinCandUp(data_ptr, key_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartClear

Func:   26���������뷨�����������

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartClear(IM_DATA_T *data_ptr)
{
    QwertLatinClear(data_ptr);
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartConfigPeriod

Func:   ���õ�Ű����ַ�

Input:
            key_ptr:    �����İ���
            key_table_ptr:   ʹ���ĸ��������ݱ��е�period������

Output:

Return:
            û������: < 0  succ: == 0 ���������

Thoughtway:

Create: yaoguangchen@spread. 2012-05-25

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 QwertSmartConfigPeriod (KEY_INFO_T *key_ptr, MMIIM_SP_26KEY_KEYCHAR_T *key_table_ptr)
{
    int32 iloop = 0;

    if(key_ptr == PNULL || key_table_ptr == PNULL)
    {
        SCI_TRACE_LOW("[MMIIM] QwertSmartConfigPeriod");
        return -1;
    }

    for (iloop = 0; iloop < MMIIM_SP_26KEY_INDEX_MAX; iloop ++)
    {
        if (key_table_ptr[iloop].key_index == MMIIM_SP_26KEY_INDEX_PERIOD)      //����period��
        {
            key_ptr->tag = KEY_TAG_NORMAL;
            key_ptr->style = KEY_STYLE_CHARACTER;
            key_ptr->add_style.text_disp = KASC_FIRST;
            key_ptr->text_len = MIN (key_table_ptr[iloop].key_chars[0], MMIIM_SP_26KEY_CHAR_MAX);
            SCI_MEMCPY(key_ptr->text,
                   &key_table_ptr[iloop].key_chars[1],
                   key_ptr->text_len * sizeof (wchar));
            key_ptr->f_down = VkbDown;
            key_ptr->f_move = VkbMove;
            key_ptr->f_long = QwertSmartCharLong;
            key_ptr->f_up = QwertSmartCharUp;
            key_ptr->has_tip = TRUE;
            
			//��text�ֶ�����ͬ����display_text�ֶ�
			key_ptr->print_text_len = MIN (key_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (key_ptr->print_text, key_ptr->text, key_ptr->print_text_len * sizeof (wchar));

            return 0;
        }
        else if (key_table_ptr[iloop].key_index == MMIIM_SP_26KEY_INDEX_MAX)
        {
            break;
        }
    }

    return -1;
}



/*----------------------------------------------------------------------------------------------------------------------
Name:   GetKmxLdb_ptr

Func:   ���ݵ�ǰ���뷨ʹ�õ����ԣ���ȡ���뷨����⣬�������ʼ��ʹ��

Input:
            lang:   ��ǰ���뷨����
            is_stoke_ldb:   �����¿�����������
Output:

Return:
            kmx_ldb_english: ��ǰ����������Ч����ʹ��Ĭ�ϵ�Ӣ��
            else ��Ӧ���Ե����뷨�����

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04
edit:   haiwu.chen 2011-11-07

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC const void *GetKmxLdb_ptr(GUIIM_LANGUAGE_T lang, BOOLEAN is_stoke_ldb)
{
    uint16 i = 0;
    const void * kmx_ldb_ptr = PNULL;

    if (GUIIM_LANG_CHS == lang
            || GUIIM_LANG_CHT == lang)
    {
        for (i = 0; i < ARR_SIZE(kmx_lang_config); i++)
        {
            if (lang == kmx_lang_config[i].lang) break;
        }

        if (i <= ARR_SIZE(kmx_lang_config) - 2)
        {
            if (!is_stoke_ldb)
            {
                kmx_ldb_ptr = *(kmx_lang_config[i].kmx_ldb_pptr);
            }
            else
            {
                kmx_ldb_ptr = *(kmx_lang_config[i+1].kmx_ldb_pptr);
            }
        }
    }
    else
    {
        for (i = 0; i < ARR_SIZE(kmx_lang_config); i++)
        {
            if (lang == kmx_lang_config[i].lang)
            {
                kmx_ldb_ptr = *(kmx_lang_config[i].kmx_ldb_pptr);
                break;
            }
        }
    }

    if (PNULL == kmx_ldb_ptr)
    {
        kmx_ldb_ptr = *(kmx_lang_config[0].kmx_ldb_pptr);
    }

    return kmx_ldb_ptr;
}





/*==============================================================================
Description: ��ȡģ��������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC uint32 GetPyFuzzy(void)
{
    uint32 fuzzy_set_cstar = 0;
    uint32 fuzzy_set_sys = MMIIM_GetPyFuzzy();

    if (fuzzy_set_sys & MMIIM_FUZZY_Z_ZH)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_Z_ZH;
    }

    if (fuzzy_set_sys & MMIIM_FUZZY_C_CH)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_C_CH;
    }

    if (fuzzy_set_sys & MMIIM_FUZZY_S_SH)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_S_SH;
    }

    if (fuzzy_set_sys & MMIIM_FUZZY_H_F)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_H_F;
    }

    if (fuzzy_set_sys & MMIIM_FUZZY_N_L)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_N_L;
    }

    if (fuzzy_set_sys & MMIIM_FUZZY_IN_ING)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_IN_ING;
    }

    if (fuzzy_set_sys & MMIIM_FUZZY_EN_ENG)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_EN_ENG;
    }

    if (fuzzy_set_sys & MMIIM_FUZZY_AN_ANG)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_AN_ANG;
    }

    if (fuzzy_set_sys & MMIIM_FUZZY_IAN_IANG)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_IAN_IANG;
    }

    if (fuzzy_set_sys & MMIIM_FUZZY_UAN_UANG)
    {
        fuzzy_set_cstar |= KMX_PINYIN_FUZZY_UAN_UANG;
    }

    return fuzzy_set_cstar;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   Cstar_SetCapsLock

Func:   ����Cstar���뷨�����Сд

            < 0 err, else 0

Create: yaoguangchen@spread. 2011-02-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Cstar_SetCapsLock (IM_DATA_T *data_ptr)
{
    CHECK_DATA (data_ptr);

    if (data_ptr->is_symbol_panel)
    {
        //SCI_TraceLow:"Cstar_SetCapsLock"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_CSTAR_10339_112_2_18_2_27_50_113,(uint8*)"");
        return -1;
    }

    //�Ĳ����ڴ�Сд�����ԣ�����Ҫ���ô�Сд
    if (!MMIAPIIM_CapsExist (data_ptr->init_param.method_param_ptr->im_def_ptr->lang))
    {
        SCI_TraceLow("MMIIM:Cstar_SetCapsLock");
        return -1;
    }
    
    if (data_ptr->init_param.method_param_ptr->im_def_ptr->type == GUIIM_TYPE_SMART)       //smart
    {
        if (data_ptr->init_param.method_param_ptr->im_def_ptr->input == GUIIM_INPUT_TYPE_TOUCHPEN)   //9 keys 
        {
            ThumbLatinSetMatch (data_ptr);
        }
        else if (data_ptr->init_param.method_param_ptr->im_def_ptr->input == GUIIM_INPUT_TYPE_TP_QWERT)  //26keys 
        {
            QwertLatinSetMatch (data_ptr);
        }
    }
    else if (data_ptr->init_param.method_param_ptr->im_def_ptr->type == GUIIM_TYPE_STROKE)  //stroke
    {
        ;
    }
    else    //not use cstar engine
    {
        ;
    }
    
    return 0;
}


/*****************************************************************************/
//  Description : UpdateUdbCache
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC int32 UpdateUdbCache(KMX_IME_ENGINE* engine, const uint16* string, uint16 length, GUIIM_METHOD_T method)
{
    int32 ret = 0;
    
#ifdef MMI_CSTAR_UDB_SUPPORT
    if (PNULL == engine
        || PNULL == string
        || 0 == length
        || GUIIM_M_NONE == method)
    {
        return ret;
    }

    switch (method)
    {
#if defined (IM_SIMP_CHINESE_SUPPORT)
    case GUIIM_M_TP_PINYIN:
    case GUIIM_M_TPQ_PINYIN:
        ret = kmx_pinyin_update_cache(engine, string, length);
        break;
#endif

#if defined (IM_SIMP_CHINESE_SUPPORT)
    case GUIIM_M_TP_STROKE:
    case GUIIM_M_TPQ_STROKE:
        ret = kmx_stroke_update_cache(engine, string, length);
        break;
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
    case GUIIM_M_TP_ZHUYIN:
    case GUIIM_M_TPQ_ZHUYIN:
        ret = kmx_zhuyin_update_cache(engine, string, length);
        break;
#endif 

#if defined (IM_TRAD_CHINESE_SUPPORT)
    case GUIIM_M_TP_STROKE_CHT:
    case GUIIM_M_TPQ_STROKE_CHT:
        ret = kmx_stroke_update_cache(engine, string, length);
        break;
#endif

#ifdef IM_AMHARIC_SUPPORT
    case GUIIM_M_TP_AMHARIC_ABC:
        break;
#endif

    default:        
        ret = kmx_latin_update_cache(engine, string, length);
        break;
    }       //lint !e764
    
#else
    ret = 0;
#endif

    return ret;
}


//��ķ���������뷽ʽ��multi-tap������Щ��ͬ:���2~9��������ѡ������Ԫ��״̬��
//���ѡ��ͬ�ĸ�������Ԫ������µ���ø���һ�µ�״̬��
//Ԫ���ں�ѡ������ʾ��������
#if defined IM_AMHARIC_SUPPORT
/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicInit

Func:   9��Amharic��ʼ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicInit (IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_AMHARIC_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_AMHARIC_T));

    ThumbAmharicInitKb(data_ptr);
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicTerm

Func:   9��Amharic�رպ���

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicTerm (IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX(data_ptr);

    SCI_FREEIF (data_ptr->data);
    data_ptr->data = PNULL;

    ReleaseVkb (&data_ptr->vkb);
    
    return TRUE;
}

/*==============================================================================
Description: ��ʼ��Amharic����
Global resource dependence: NONE
Author: yaoguang.chen
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void ThumbAmharicInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 7;
    uint8 const SYS_CNT = 2;
    uint8 const CAND_CNT = KMX_THUMB_AMHARIC_CANDIDATE_MAX;   //���8����ѡ�֣���7��Ԫ��+ 1������
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;
    MMI_STRING_T slide_cue = {PNULL, 0};

    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;
    SetPanel(data_ptr);

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�
    vkb_ptr->key_cnt -= 5; //no expand py, no page up,down

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].text[0] = ",.?!-"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbAmharicCustUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //deal tag
    if (GUIIM_TAG_EMAIL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '@';
        vkb_ptr->keys[cust_begin+2].text[0] = '-';
        vkb_ptr->keys[cust_begin+3].text[0] = '_';
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }
    else if (GUIIM_TAG_URL == data_ptr->init_param.method_param_ptr->tag)
    {
        vkb_ptr->keys[cust_begin].text[0] = '.';
        vkb_ptr->keys[cust_begin+1].text[0] = '/';
        vkb_ptr->keys[cust_begin+2].text_len = 7;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+2].text, L"http://", vkb_ptr->keys[cust_begin+2].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+3].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+3].text, L"www.", vkb_ptr->keys[cust_begin+3].text_len*sizeof(wchar));
        vkb_ptr->keys[cust_begin+4].text_len = 4;
        SCI_MEMCPY(vkb_ptr->keys[cust_begin+4].text, L".com", vkb_ptr->keys[cust_begin+4].text_len*sizeof(wchar));
    }

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    if (key_table_ptr == PNULL)
    {
        key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    }

    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_2X4_EXCLUDE_DIGIT;        //
        //=========��������˿ӡ����
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbAmharicCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbAmharicCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //--1�������ã��������Ϊһ���ַ�'1'����ֱ������
    vkb_ptr->keys[char_begin].f_up = MultiTapCharUp; 

    //--0 ���һ��char
    key_idx--;
    ThumbMultitapRedefineKey0 (data_ptr->init_param.method_param_ptr->im_def_ptr->method, &vkb_ptr->keys[key_idx]);
    
    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbAmharicCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbAmharicCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--Multi-char
    key_idx++;
    ThumbMultitapRedefineCaps (data_ptr->init_param.method_param_ptr->im_def_ptr->method, vkb_ptr->keys + key_idx);   
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--symbol
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_123SYMBOL;
    vkb_ptr->keys[key_idx].text_len = 4;
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text, L"123*", vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    //--set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      //IMG_IMT_SET_EN;
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbAmharicSysUp;
    }

    //--hide
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;

    //--expand
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
      
    //candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbAmharicCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

	//��text�ֶ�����ͬ����display_text�ֶΣ�����Ҫ��ÿ��text���õط��޸�
	MMIIM_TOUCH_SynchronizeAllDisplayText (data_ptr);
	
    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicCustUp

Func:   9��Amharic���ư��������߼������ð�����ʾ�����ַ�����������ַ���
           ���º�ѡ�ֵ���Ӧ�ĸ�����ϵ�Ԫ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:
            1.cust������ʾ��Ϊ��Ӧ�ĸ���

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicCustUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    MultiTapCommitLast (data_ptr);  // 1, 0��Ϊmultitap���룬��ʱ��û�е�����Ҫǿ������
    
    if (((KERNEL_DATA_AMHARIC_T *)(data_ptr->data))->input_string_len > 0)   //�������밴��
    {
        if (key_ptr->code - VKEY_CUSTOM < ((KERNEL_DATA_AMHARIC_T *)(data_ptr->data))->consonant_cnt)
        {
            ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant_index = key_ptr->code - VKEY_CUSTOM;
            ThumbAmharicUpdate (data_ptr);
        }
    }
    else    //�ύ������
    {
        CommitStr(data_ptr, key_ptr->text, sizeof (key_ptr->text));
    }

    VkbUp(data_ptr, key_ptr);
    
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicCharLong

Func:   0~9������Ϣ���������ַ�0~9

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: TRUE or err assert

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicCharLong (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    MultiTapCommitLast (data_ptr);  // 1, 0��Ϊmultitap���룬��ʱ��û�е�����Ҫǿ������
    
    if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len > 0)
    {
        //SCI_TRACE_LOW:"ThumbAmharicCharLong"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_CSTAR_10782_112_2_18_2_27_51_114,(uint8*)"");
        return TRUE;
    }

    if (VKEY_1 <= key_ptr->code && key_ptr->code <= VKEY_9)
    {
        CommitChar(data_ptr, '1' + (key_ptr->code - VKEY_1));
    }
    else if (VKEY_0 == key_ptr->code)
    {
        CommitChar(data_ptr, '0');
    }

    VkbLong(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicCharUp

Func:   9��Amharic�ַ���������

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:

Output:

Return: None

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicCharUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    MultiTapCommitLast (data_ptr);  // 1, 0��Ϊmultitap���룬��ʱ��û�е�����Ҫǿ������
    
    //�����һ���ַ�
    if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len == 0)     
    {
        if (key_ptr->code >= VKEY_2 && key_ptr->code <= VKEY_9)
        {
            uint16 input_len = 0;

            //���ú�ѡ���� 
            //((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant_cnt = kmx_amharic_digit_match ('1' + key_ptr->code - VKEY_1, ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant);
            input_len = ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len;
            ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string[input_len] = '1' + key_ptr->code - VKEY_1;
            ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len ++;

            ((KERNEL_DATA_AMHARIC_T *)(data_ptr->data))->consonant_index = 0;

            ThumbAmharicUpdate (data_ptr);
#if 0            
            ThumbAmharicSetCust (data_ptr);

            //Ԫ��������ѡ��
            ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->cand_cnt = kmx_amharic_consonant_match (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant[0].string[0], 
                ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->candidate);
            ThumbAmharicSetCandidate(data_ptr);
#endif            
        }

        ThumbAmharicCharLock (data_ptr, 1);
    }
    //�Ѿ������ַ�����ѡ��Ԫ������״̬
     else if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len > 0)     
    {
        //ʲô������
        if (key_ptr->code >= VKEY_2 && key_ptr->code <= VKEY_9)
        {
                  
        }

        ThumbAmharicCharLock (data_ptr, 0);
    }

    VkbUp(data_ptr, key_ptr);
    
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicCharLock

Func:   ��ѡ��ѡ��״̬���������밴���ַ�

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicCharLock (IM_DATA_T *data_ptr, uint8 locked)
{
    uint16 iloop = 0;
    KEY_INFO_T *key_ptr = PNULL;

    CHECK_DATA_EX(data_ptr);
    
    for (iloop = 0; iloop < data_ptr->vkb.key_cnt; iloop ++)
    {
        key_ptr = data_ptr->vkb.keys + iloop;
        if (key_ptr->code >= VKEY_2 && key_ptr->code <= VKEY_9)
        {
            key_ptr->is_inactive = locked;
			MMIIM_SetVKeyPaintStatus (key_ptr, TRUE);
        }
    }
    
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicClear

Func:   ��պ�ѡ�ֺ�cust�ַ���������

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbAmharicClear (IM_DATA_T *data_ptr)
{
    uint16 iloop = 0;
    KEY_INFO_T *key_cur_ptr = PNULL;
    uint16 CUST_CNT = 5;
    
    CHECK_DATA_EX(data_ptr);

    MultiTapCommitLast (data_ptr);  // 1, 0��Ϊmultitap���룬��ʱ��û�е�����Ҫǿ������
    SCI_MEMSET (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data)), 0, sizeof (KERNEL_DATA_AMHARIC_T));    
    
    //cust�ַ���ԭ
    for (iloop = 0; iloop < CUST_CNT; iloop ++)
    {
        key_cur_ptr = data_ptr->vkb.keys + data_ptr->vkb.custom_first + iloop;
        key_cur_ptr->text[0] = g_thumb_cust_symb[iloop];
        key_cur_ptr->text_len = 1;
        key_cur_ptr->is_inactive = 0;        
		//��text�ֶ�����ͬ����display_text�ֶ�
		key_cur_ptr->print_text_len = MIN (key_cur_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (key_cur_ptr->print_text, key_cur_ptr->text, key_cur_ptr->print_text_len * sizeof (wchar));
		MMIIM_SetVKeyPaintStatus (key_cur_ptr, TRUE);
    }

    //��ѡ�����
    for (iloop = 0; iloop < data_ptr->vkb.candidate_cnt; iloop ++)
    {
        key_cur_ptr = data_ptr->vkb.keys + data_ptr->vkb.candidate_first + iloop;
        key_cur_ptr->text[0] = 0;
        key_cur_ptr->text_len = 0;
        key_cur_ptr->is_hide = 1;       //
		//��text�ֶ�����ͬ����display_text�ֶ�
		key_cur_ptr->print_text_len = MIN (key_cur_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (key_cur_ptr->print_text, key_cur_ptr->text, key_cur_ptr->print_text_len * sizeof (wchar));
    }

    if (data_ptr->vkb.is_expand)
    {
        SwitchExpandKey(data_ptr);
    }

    SetHideKey (data_ptr, 0);
    
    //char�ַ�����
    ThumbAmharicCharLock (data_ptr, 0);

    //ˢ��
    ThumbAmharicSetCandidate (data_ptr);
    ThumbAmharicDispEdit (data_ptr);
    
    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicCtrlLong

Func:   ���ư�������������������

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbAmharicUpdate (IM_DATA_T *data_ptr)
{
    uint16 consonant_index = 0, consonant_cnt = 0, cand_cnt = 0;
    CHECK_DATA_EX(data_ptr);
    
    //
    if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len > 0)
    {
        consonant_cnt = kmx_amharic_digit_match (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string[0], 
            ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant);    

        if (consonant_cnt > 0)      //���ڸ���������Ԫ��
        {
            ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant_cnt = consonant_cnt;
            consonant_index = ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant_index;
            cand_cnt = kmx_amharic_consonant_match (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant[consonant_index].string[0],
                ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->candidate);

            ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->cand_cnt = cand_cnt;
        }
    }
    
    //ˢ��
    SetHideKey (data_ptr, cand_cnt);
    ThumbAmharicSetCust (data_ptr);
    ThumbAmharicSetCandidate (data_ptr);
    ThumbAmharicDispEdit (data_ptr);

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicCtrlLong

Func:   ���ư�������������������

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicCtrlLong (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    MultiTapCommitLast (data_ptr);  // 1, 0��Ϊmultitap���룬��ʱ��û�е�����Ҫǿ������
    
    switch (key_ptr->code)
    {
        case VKEY_DEL:
            {
                if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len > 0)
                {
                    ThumbAmharicClear (data_ptr);
                }
                else
                {
                    GUIIM_NOTIFY_PACK_T notify_pack = {0};

                    notify_pack.notify = GUIIM_NOTIFY_LONG_DEL;

                    GUIIM_SetNotify(data_ptr->init_param.im_handle, &notify_pack);
                }
            }     
        
            VkbUp(data_ptr, key_ptr);
            break;
            
        default:
            break;
    }
    
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicCtrlUp

Func:   ���ư���������������

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicCtrlUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    MultiTapCommitLast (data_ptr);  // 1, 0��Ϊmultitap���룬��ʱ��û�е�����Ҫǿ������
    
    switch (key_ptr->code)
    {
    case VKEY_DEL:

        if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len > 0)
        {
            ThumbAmharicClear (data_ptr);
        }
        else
        {
            CommitChar(data_ptr, '\b');
        }

        break;

    case VKEY_ENTER:
        CommitChar(data_ptr, '\n');

        break;

    case VKEY_123:
        if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    case VKEY_SYMBOL:
        if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_SYM);
        key_ptr = PNULL;
        break;

    case VKEY_123SYMBOL:
        if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len > 0) break;
        SymInit(data_ptr, SYMBOL_TYPE_HALF, SYMBOL_INIT_NUM);
        key_ptr = PNULL;
        break;

    default:
        break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicCandUp

Func:   ��ѡ�ְ��������������̣�ֱ������������յ�ǰ������

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicCandUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    CommitChar (data_ptr, key_ptr->text[0]);
    ThumbAmharicClear (data_ptr);

    VkbUp(data_ptr, key_ptr);
    
    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicSysUp

Func:   ϵͳ���������������̣�ϵͳ��������ҳ�Ȳ�����

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbAmharicSysUp (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    CHECK_DATA_EX(data_ptr);
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == key_ptr)
    {
        return TRUE;
    }
    
    MultiTapCommitLast (data_ptr);  // 1, 0��Ϊmultitap���룬��ʱ��û�е�����Ҫǿ������
    
    switch (key_ptr->code)
    {
        case VKEY_EXPAND:
        {
            SwitchExpandKey(data_ptr);
            SetHideKey(data_ptr, ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->cand_cnt);
            ThumbAmharicSetCandidate (data_ptr);
            break;
        }
        
        default:
            break;
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicSetCust

Func:   ����cust���ַ���ʾ

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbAmharicSetCust(IM_DATA_T *data_ptr)
{
    CHECK_DATA_EX (data_ptr);
    
    if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len > 0)
    {
        KEY_INFO_T *key_cur_ptr = PNULL;
        uint16 iloop = 0;

        for (iloop = 0; iloop < ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant_cnt; iloop ++)
        {
            key_cur_ptr = data_ptr->vkb.keys + data_ptr->vkb.custom_first + iloop;
            key_cur_ptr->text[0] = ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant[iloop].string[0];
            key_cur_ptr->text_len = ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant[iloop].length;
			//��text�ֶ�����ͬ����display_text�ֶ�
			key_cur_ptr->print_text_len = MIN (key_cur_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (key_cur_ptr->print_text, key_cur_ptr->text, key_cur_ptr->print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (key_cur_ptr, TRUE);
        }

        //������ַ��հ���ʾ
        for (; iloop < 5; iloop ++)
        {
            key_cur_ptr = data_ptr->vkb.keys + data_ptr->vkb.custom_first + iloop;
            key_cur_ptr->text[0] = 0;
            key_cur_ptr->text_len = 0;
            key_cur_ptr->is_inactive = 1;
			//��text�ֶ�����ͬ����display_text�ֶ�
			key_cur_ptr->print_text_len = MIN (key_cur_ptr->text_len, PRINT_TEXT_LEN_LIMIT - 1);
			SCI_MEMCPY (key_cur_ptr->print_text, key_cur_ptr->text, key_cur_ptr->print_text_len * sizeof (wchar));

			MMIIM_SetVKeyPaintStatus (key_cur_ptr, TRUE);
        }
    } 

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicDraw

Func:   ���Ƽ���

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbAmharicDraw (IM_DATA_T *data_ptr)
{
	MMIIM_SetVkbPaintStatus (data_ptr, TRUE);
    DrawVkb(data_ptr);
    ThumbAmharicDispEdit(data_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicDispEdit

Func:   ��ʾ��ǰ����ĸ����ַ�

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbAmharicDispEdit(IM_DATA_T *data_ptr)
{
    BOOLEAN has_edit_layer = FALSE;
    UILAYER_APPEND_BLT_T append_layer = {0};
    GUILCD_DRAW_DC_T dc_data = {0};
    BOOLEAN is_dev_ok = FALSE;
    wchar buffer[KMX_MAX_PINYIN_INPUT] = {0};
    uint32 buffer_len = 0;
    int16 edit_disp_len = 0;
    int16 edit_disp_len_limit = 0;
    GUI_RECT_T edit_rect = {0};
    GUISTR_BUF_T des_str_buf = {0};
    MMI_STRING_T str_info = {0};
    GUISTR_STYLE_T str_style = {0};
    KEY_INFO_T *key_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);

    has_edit_layer = CreateEditLayer(data_ptr);

    //SCI_ASSERT(has_edit_layer); /*assert verified*/
    if(!has_edit_layer)
    {
        return;
    }
    
    UILAYER_Clear(&data_ptr->edit_layer);

    if (((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->input_string_len == 0)
    {
        return;
    }

    append_layer.lcd_dev_info = data_ptr->edit_layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    is_dev_ok = LCD_PrepareDrawParam(&data_ptr->edit_layer, &dc_data);

    if (!is_dev_ok)
    {
        return;
    }

    //����
    key_ptr = data_ptr->vkb.keys + data_ptr->vkb.custom_first + ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->consonant_index;
    buffer[0] = key_ptr->text[0];
    buffer_len = 1;

    edit_disp_len = GUI_GetStringWidth(data_ptr->theme.edit_text_font, buffer, buffer_len);

    edit_disp_len_limit = data_ptr->edit_rect.right - data_ptr->edit_rect.left + 1;
    edit_disp_len_limit -= 2 * data_ptr->theme.edit_padding;

    if (edit_disp_len > edit_disp_len_limit)
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len_limit + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = 0;
        edit_rect.bottom = edit_rect.top + 2 * data_ptr->theme.edit_text_font - 1;
    }
    else
    {
        edit_rect.left = 0;
        edit_rect.right = edit_rect.left + edit_disp_len + 2 * data_ptr->theme.edit_padding - 1;
        edit_rect.top = data_ptr->theme.edit_text_font;
        edit_rect.bottom = edit_rect.top + data_ptr->theme.edit_text_font - 1;
    }

    //
    _FillRect(&dc_data, &edit_rect, data_ptr->theme.edit_bg_color);
    _DrawRect(&dc_data, &edit_rect, data_ptr->theme.edit_border_color);

    DcToDstStr(&dc_data, &des_str_buf);

    str_info.wstr_ptr = buffer;
    str_info.wstr_len = buffer_len;

    str_style.font = data_ptr->theme.edit_text_font;
    str_style.font_color = RGB888TO565(data_ptr->theme.edit_text_color);

    edit_rect.left += data_ptr->theme.edit_padding;
    edit_rect.right -= data_ptr->theme.edit_padding;

    GUISTR_DrawTextInRect(
        &des_str_buf,
        &edit_rect,
        &edit_rect,
        &str_info,
        &str_style,
        GUISTR_STATE_WORDBREAK | GUISTR_STATE_ALIGN_BY_DIR,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_WeakLayerAlpha(&data_ptr->edit_layer, data_ptr->theme.alpha);
    MMITHEME_StoreUpdateRect(&data_ptr->edit_layer, edit_rect);

    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbAmharicSetCandidate

Func:   ���ݵ�ǰ�ĸ����ַ�����ʾ�����Ԫ���ַ���ѡ��

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbAmharicSetCandidate (IM_DATA_T *data_ptr)
{
    uint16 iloop = 0, cand_key_idx = 0, line_index = 0;
    GUI_RECT_T cand_rect = {0, 0, 0, 0};
    const uint16 key_padding = 3;
    
    CHECK_DATA_EX (data_ptr);

	if (data_ptr->vkb.is_expand)	//speacial deal, background need to redraw if the cand changed
	{
		MMIIM_SetVkbPaintStatus (data_ptr, TRUE);		 
	}
	else
	{
		MMIIM_SetVKeyPaintStatus (data_ptr->vkb.vessel_key, TRUE);	//cand vessel
	}
	
    //������ֵ
    cand_key_idx = data_ptr->vkb.candidate_first;
    for (iloop = 0; iloop < ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->cand_cnt; iloop ++)
    {
        data_ptr->vkb.keys[cand_key_idx].text_len = ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->candidate[iloop].length;
        SCI_MEMCPY(data_ptr->vkb.keys[cand_key_idx].text, 
                ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->candidate[iloop].string,
                MMIAPICOM_Wstrlen(((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->candidate[iloop].string) * sizeof (wchar));
		//��text�ֶ�����ͬ����display_text�ֶ�
		data_ptr->vkb.keys[cand_key_idx].print_text_len = MIN (data_ptr->vkb.keys[cand_key_idx].text_len, PRINT_TEXT_LEN_LIMIT - 1);
		SCI_MEMCPY (data_ptr->vkb.keys[cand_key_idx].print_text, data_ptr->vkb.keys[cand_key_idx].text, data_ptr->vkb.keys[cand_key_idx].print_text_len * sizeof (wchar));

        data_ptr->vkb.keys[cand_key_idx].is_hide = 0;
		MMIIM_SetVKeyPaintStatus (&data_ptr->vkb.keys[cand_key_idx], TRUE);
		
        cand_key_idx++;
    }

    //û�к�ѡ�ֵİ�������
    for (; iloop < data_ptr->vkb.candidate_cnt; iloop ++)
    {
        data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
        cand_key_idx ++;
    }

    //��������
    cand_key_idx = data_ptr->vkb.candidate_first;
    cand_rect = data_ptr->vkb.vessel_key->rect;
	
    for (iloop = 0; iloop < ((KERNEL_DATA_AMHARIC_T*)(data_ptr->data))->cand_cnt && iloop < data_ptr->vkb.candidate_cnt; iloop ++)
    {
        int16 word_width = 0;
        int16 disp_right = 0;

        word_width = GUI_GetStringWidth(data_ptr->theme.key_cand[0].text_font,
                                        ((KEY_INFO_T *)(data_ptr->vkb.keys + data_ptr->vkb.candidate_first + iloop))->print_text, 
                                        ((KEY_INFO_T *)(data_ptr->vkb.keys + data_ptr->vkb.candidate_first + iloop))->print_text_len);
        word_width += 2 * key_padding;

        if (word_width < THUMB_AMHARIC_CAND_WIDTH_MIN) word_width = THUMB_AMHARIC_CAND_WIDTH_MIN;

        disp_right = cand_rect.left + word_width - 1;

        data_ptr->vkb.keys[cand_key_idx].rect = cand_rect;
        data_ptr->vkb.keys[cand_key_idx].rect.right = disp_right;

        if (data_ptr->vkb.keys[cand_key_idx].rect.right > cand_rect.right)  //����һ����ʾ���
        {
            //��������һ��
            cand_rect.left = data_ptr->vkb.vessel_key->rect.left;
            cand_rect.top += (data_ptr->vkb.vessel_key->rect.bottom - data_ptr->vkb.vessel_key->rect.top) + key_padding;
            cand_rect.bottom += (data_ptr->vkb.vessel_key->rect.bottom - data_ptr->vkb.vessel_key->rect.top) + key_padding;

            data_ptr->vkb.keys[cand_key_idx].rect = cand_rect;
            data_ptr->vkb.keys[cand_key_idx].rect.right = cand_rect.left + word_width - 1;
            cand_rect.left += word_width;
            line_index ++;
        }
        else
        {
            cand_rect.left = disp_right + 1;
        }

        if (data_ptr->vkb.is_expand == 0 && line_index > 0)   //û��չ�����ǵ�һ��
        {
            data_ptr->vkb.keys[cand_key_idx].is_hide = 1;
        }

        cand_key_idx++;
    }
    
    return;     
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   kmx_amharic_digit_match

Func:   ���������ַ�����ѯ��Ӧ�ĸ����ַ�; 2~9��Ч������������Ч

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint16 kmx_amharic_digit_match (uint16 asc_digit, KMX_LATIN_CANDIDATE *consonant_ptr)
{
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL;
    wchar *char_ptr = PNULL;
    int offset = 0;
    uint16 iloop = 0;

    if (consonant_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"kmx_amharic_digit_match"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_CSTAR_11459_112_2_18_2_27_52_115,(uint8*)"");
        return 0;
    }
    
    offset = asc_digit - '1';
    if (offset < 1 || offset > 8)       // 2~9��Ч������������Ч
    {
        //SCI_TRACE_LOW:"kmx_amharic_digit_match"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_CSTAR_11466_112_2_18_2_27_52_116,(uint8*)"");
        return 0;
    }

    key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable(GUIIM_M_TP_AMHARIC_ABC);
    if (key_table_ptr == PNULL)
    {
        key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(GUIIM_M_TP_AMHARIC);
        if (key_table_ptr == PNULL)
        {
            //SCI_TRACE_LOW:"kmx_amharic_digit_match"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_CSTAR_11476_112_2_18_2_27_52_117,(uint8*)"");
            return 0;
        }
    }

    char_ptr = (wchar *)key_table_ptr + (offset * (MMIIM_SP_9KEY_CHAR_MAX + 1));
 
    for (iloop = 0; iloop < char_ptr[0]; iloop ++)
    {
        consonant_ptr[iloop].string[0] = char_ptr[iloop + 1];
        consonant_ptr[iloop].length = 1;
    }

    return char_ptr[0];
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   kmx_amharic_consonant_match

Func:   ���ݸ����ַ������Ҷ�Ӧ��Ԫ���ַ����ù�����cstar����

Create: yaoguangchen@spread. 2012-01-10

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint16 kmx_amharic_consonant_match (wchar consonant_char, KMX_LATIN_CANDIDATE *candidate_ptr)
{
    uint16 iloop = 0, cand_cnt = 0;
    
    if (candidate_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"kmx_amharic_unicode_char_match"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_TOUCH_IME_CSTAR_11508_112_2_18_2_27_52_118,(uint8*)"");
        return 0;
    }

    cand_cnt = 7;
    for (iloop = 0; iloop < cand_cnt; iloop ++)
    {
        candidate_ptr[iloop].string[0] = consonant_char + iloop;
        candidate_ptr[iloop].length = 1;
    }
    
    switch (consonant_char)
    {
        case 0x1200:
        case 0x12A0:
        case 0x12A8:
        case 0x12C8:
        case 0x12E8:
        case 0x1300:
        case 0x1308:
        case 0x1330:
        case 0x1350:
        case 0x1368:
            break;
        
        case 0x1240:
            candidate_ptr[7].string[0] = 0x124B;
            candidate_ptr[7].length = 1;
            cand_cnt = 8;
            break;
        case 0x1340:
            candidate_ptr[7].string[0] = 0x133F;
            candidate_ptr[7].length = 1;
            cand_cnt = 8;
            break;

        default:
            candidate_ptr[7].string[0] = consonant_char + 7;
            candidate_ptr[7].length = 1;
            cand_cnt = 8;
            break;
    }

    return cand_cnt;
}
#endif


//�ϼ��������⴦��
#if defined IM_BENGALI_SUPPORT
/*----------------------------------------------------------------------------------------------------------------------
Func:   9���������뷨��ʼ�����޷�����ThumbSmartInit ����������̳�ʼ����ͬ

Input:
        data_ptr:    ���뷨ָ��

Output:

Return: TRUE SUC; ELSE FALSE

Thoughtway:

Create: yaoguangchen@spread. 2012-07-12

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbBengaliInit (IM_DATA_T *data_ptr)
{
    if (data_ptr == PNULL
            || data_ptr->init_param.method_param_ptr == PNULL
            || data_ptr->init_param.method_param_ptr->im_def_ptr == PNULL)
    {
        SCI_TRACE_LOW("[MMIIM]ThumbBengaliInit");
        return FALSE;
    }

    data_ptr->data = SCI_ALLOC_APP(sizeof(KERNEL_DATA_LATIN_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(KERNEL_DATA_LATIN_T));

    //���̳�ʼ��
    ThumbBengaliInitKb(data_ptr);

    //�����ʼ��
    SmartInitEngine(&(((KERNEL_DATA_LATIN_T*)(data_ptr->data))->engine), 
        data_ptr->init_param.method_param_ptr->im_def_ptr->lang,
        data_ptr->init_param.method_param_ptr->im_def_ptr->method);

    return TRUE;
}


/*----------------------------------------------------------------------------------------------------------------------

Func:   9���������뷨���̳�ʼ��

Input:
        data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-07-12

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbBengaliInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 8;
    uint8 const SYS_CNT = 2;
    uint8 const CAND_CNT = LATIN_CANDIDATE_COUNT;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL, *print_key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;
    MMI_STRING_T slide_cue = {PNULL, 0};
    
    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;
    SetPanel(data_ptr);

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += CAND_CNT; //����candidate
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�
    vkb_ptr->key_cnt -= g_thumb_panel_expand_py_cnt; //no expand py

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].print_text[0] = vkb_ptr->keys[key_idx].text[0] = ",.?!-"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].print_text_len = vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_Smart9Key_GetKeyTable(data_ptr->init_param.method_param_ptr->im_def_ptr->method);
	print_key_table_ptr = MMIIM_SP_Smart9KEY_GetPrintKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_SUBTEXT_MIDDLE + KASC_EXCLUDE_DIGIT;        //��һ�о�����ʾsubtext���ڶ�����ʾprint_text
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        cur_char_ptr = (wchar *)print_key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].print_text_len = MIN (cur_char_ptr[0], PRINT_TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].print_text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].print_text_len*sizeof(wchar));
		vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;

        vkb_ptr->keys[key_idx].f_long = ThumbSmartCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //--0 ���һ��char
    key_idx--;
    
    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbSmartCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--VKEY_SET
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--star *
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_STAR;
	vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].add_style.text_disp = KASC_SUBTEXT_MIDDLE + KASC_EXCLUDE_DIGIT;        //��һ�о�����ʾsubtext���ڶ�����ʾprint_text
    vkb_ptr->keys[key_idx].subtext_len = 1;
    vkb_ptr->keys[key_idx].subtext[0] = '*';
    vkb_ptr->keys[key_idx].text_len = 1;
	vkb_ptr->keys[key_idx].text[0] = HASANATA;
    vkb_ptr->keys[key_idx].print_text_len = 1;
	vkb_ptr->keys[key_idx].print_text[0] = HASANATA;
	vkb_ptr->keys[key_idx].f_up = ThumbBengaliVkeyupStar;
    //-- #
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_HASH;
	vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].add_style.text_disp = KASC_SUBTEXT_MIDDLE + KASC_EXCLUDE_DIGIT;        //��һ�о�����ʾsubtext���ڶ�����ʾprint_text
    vkb_ptr->keys[key_idx].subtext_len = 1;
    vkb_ptr->keys[key_idx].subtext[0] = '#';
    cur_char_ptr = (wchar *)key_table_ptr + ((VKEY_0 - VKEY_1 + 2) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
    vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], PRINT_TEXT_LEN_LIMIT - 1);      //�ַ�����
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
               &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    cur_char_ptr = (wchar *)print_key_table_ptr + ((VKEY_0 - VKEY_1 + 2) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
    vkb_ptr->keys[key_idx].print_text_len = MIN (cur_char_ptr[0], PRINT_TEXT_LEN_LIMIT - 1);      //�ַ�����
    SCI_MEMCPY(vkb_ptr->keys[key_idx].print_text,
               &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].print_text_len*sizeof(wchar));
    vkb_ptr->keys[key_idx].f_up = MultiTapCharUp;    
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }
    
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartSysUp;
    }

    //--hide
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--expand
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].print_text_len = vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
	MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].print_text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].print_text_len);
        
    //candidate
    //��ѡ����λ��δ��������δ��
    //candidate key ���Ƿ��������Ϊ����ܻḲ���������Ĳ���֮�ϣ�����art
    key_idx++;
    cand_begin = key_idx;

    for (; key_idx < cand_begin + CAND_CNT; key_idx++)
    {
        vkb_ptr->keys[key_idx].code = VKEY_CANDIDATE + key_idx - cand_begin;
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CANDIDATE;
        vkb_ptr->keys[key_idx].is_hide = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = ThumbSmartCandUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->candidate_first = cand_begin;
    vkb_ptr->candidate_cnt = CAND_CNT;

    return;
}


/*==============================================================================
Description: 
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliVkeyupStar(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
    KERNEL_DATA_LATIN_T* data_latin_ptr = PNULL;
    
    CHECK_DATA_EX(data_ptr);
    if (PNULL == key_ptr)
    {
        SCI_TraceLow("[MMIIM]ThumbBengaliVkeyupStar");
        return FALSE;
    }

    data_latin_ptr = ((KERNEL_DATA_LATIN_T*)(data_ptr->data));
    if (data_latin_ptr->input_string_len < KMX_MAX_LATIN_WORD_LENGTH)
    {
        data_latin_ptr->input_string[data_latin_ptr->input_string_len++] = '*';
        ThumbSmartUpdate(data_ptr);
    }

	VkbUp (data_ptr, key_ptr);
	
    return TRUE;
}


/*==============================================================================
Description: 
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliTerm (IM_DATA_T *data_ptr)
{
	return ThumbSmartTerm (data_ptr);
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbBengaliMultitapInitKb

Func:   9��multi-tap���뷨���̳�ʼ��, �����ϵĶ�����ThumbSmartBengaliInitKb������ͬ�������ַ�������Ϣ��������ͬ

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-08-02

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbBengaliMultitapInitKb(IM_DATA_T *data_ptr)
{
    uint8 const CUST_CNT = 5;
    uint8 const CHAR_CNT = 10;
    uint8 const CTRL_CNT = 8;
    uint8 const SYS_CNT = 2;
    uint32 keys_size = 0;
    int32 key_idx = 0;
    int32 cust_begin = 0;
    int32 char_begin = 0;
    int32 ctrl_begin = 0;
    int32 sys_begin = 0;
    int32 cand_begin = 0;
    VKEYBOARD_T *vkb_ptr = PNULL;
    int16 panel_width, panel_height;
    MMIIM_SP_9KEY_TABLE_DEF_T *key_table_ptr = PNULL, *print_key_table_ptr = PNULL;
    wchar *cur_char_ptr = PNULL;
    MMI_STRING_T slide_cue = {PNULL, 0};
    
    CHECK_DATA(data_ptr);

    data_ptr->theme = g_theme_thumb_default;
    SetPanel(data_ptr);

    ReleaseVkb(&data_ptr->vkb);

    data_ptr->vkb.f_sliding = SlidingPanel;

    vkb_ptr = &(data_ptr->vkb);
    panel_width = data_ptr->rect.right - data_ptr->rect.left + 1;
    panel_height = data_ptr->rect.bottom - data_ptr->rect.top + 1;

    vkb_ptr->key_cnt = g_thumb_panel_position_size;
    vkb_ptr->key_cnt += 1; //expand & hide ���غϵ�
    vkb_ptr->key_cnt -= g_thumb_panel_expand_py_cnt; //no expand py

    keys_size = vkb_ptr->key_cnt * sizeof(KEY_INFO_T);

    vkb_ptr->keys = SCI_ALLOC_APP(keys_size);
    SCI_MEMSET(vkb_ptr->keys, 0, keys_size);

    //custom * 5
    //custom key ��λ��������1��ʼ������5��
    key_idx = 0;
    cust_begin = key_idx;

    for (; key_idx < CUST_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_CUSTOM + key_idx - cust_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CUSTOM;
        vkb_ptr->keys[key_idx].print_text[0] = vkb_ptr->keys[key_idx].text[0] = ",.?!-"[key_idx - cust_begin];
        vkb_ptr->keys[key_idx].print_text_len = vkb_ptr->keys[key_idx].text_len = 1;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
        vkb_ptr->keys[key_idx].f_up = VkeyUpDeal;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    key_idx--;

    vkb_ptr->custom_first = cust_begin;
    vkb_ptr->custom_cnt = CUST_CNT;

    //character * 10
    //character key ��λ��������6��ʼ��15
    key_idx++;
    data_ptr->vkb.charactor_first = char_begin = key_idx;

    key_table_ptr = MMIIM_SP_ML9KEY_GetKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
	print_key_table_ptr = MMIIM_SP_ML9KEY_GetPrintKeyTable (data_ptr->init_param.method_param_ptr->im_def_ptr->method);
    for (; key_idx < char_begin + CHAR_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].code = VKEY_1 + key_idx - char_begin;
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
        vkb_ptr->keys[key_idx].add_style.text_disp = KASC_SUBTEXT_MIDDLE + KASC_EXCLUDE_DIGIT;        //��һ�о�����ʾsubtext���ڶ�����ʾprint_text
        cur_char_ptr = (wchar *)key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
        cur_char_ptr = (wchar *)print_key_table_ptr + ((key_idx - char_begin) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
        vkb_ptr->keys[key_idx].print_text_len = MIN (cur_char_ptr[0], PRINT_TEXT_LEN_LIMIT - 1);      //�ַ�����
        SCI_MEMCPY(vkb_ptr->keys[key_idx].print_text,
                   &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].print_text_len*sizeof(wchar));
		vkb_ptr->keys[key_idx].subtext[0] = "1234567890"[key_idx - char_begin];
        vkb_ptr->keys[key_idx].subtext_len = 1;
        vkb_ptr->keys[key_idx].font_size = -4;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;

        vkb_ptr->keys[key_idx].f_long = ThumbMultiTapCharLong;
        vkb_ptr->keys[key_idx].f_up = ThumbBengaliMultitapCharUp;
        vkb_ptr->keys[key_idx].has_tip = TRUE;
    }

    //--0 ���һ��char
    key_idx--;
    
    //control, del, 123, caps, symbol, switch, enter, + exand * 2
    //character key ��λ��������16��ʼ��21 + 2
    key_idx++;
    ctrl_begin = key_idx;

    for (; key_idx < ctrl_begin + CTRL_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[key_idx + 2]),
                    &(vkb_ptr->keys[key_idx].rect),
                    data_ptr->theme.key_hor_space, data_ptr->theme.key_ver_space);
        vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_CONTROL;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = ThumbMultiTapCtrlLong;
        vkb_ptr->keys[key_idx].f_up = ThumbMultiTapCtrlUp;
    }

    //--delete
    key_idx = ctrl_begin;
    vkb_ptr->keys[key_idx].code = VKEY_DEL;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_DEL;
    //--VKEY_SET
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_SET;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_SET;      
    vkb_ptr->keys[key_idx].f_down = VkeySetDown;
    vkb_ptr->keys[key_idx].f_move = PNULL;  //MMIIM_MethodSettingMenuRefresh���ش���
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = MMIIM_VkeyUpSet;    //��MMIIM_HandleSettingIMUp���ش���
    
    //--enter
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_ENTER;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_ENTER;
    //--star *
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_STAR;
	vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].add_style.text_disp = KASC_SUBTEXT_MIDDLE + KASC_EXCLUDE_DIGIT;        //��һ�о�����ʾsubtext���ڶ�����ʾprint_text
    vkb_ptr->keys[key_idx].subtext_len = 1;
    vkb_ptr->keys[key_idx].subtext[0] = '*';
    vkb_ptr->keys[key_idx].text_len = 1;
	vkb_ptr->keys[key_idx].text[0] = HASANATA;
    vkb_ptr->keys[key_idx].print_text_len = 1;
	vkb_ptr->keys[key_idx].print_text[0] = HASANATA;
	vkb_ptr->keys[key_idx].f_up = ThumbBengaliMultitapCharUp;
    //-- #
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_HASH;
	vkb_ptr->keys[key_idx].style = KEY_STYLE_CHARACTER;
    vkb_ptr->keys[key_idx].add_style.text_disp = KASC_SUBTEXT_MIDDLE + KASC_EXCLUDE_DIGIT;        //��һ�о�����ʾsubtext���ڶ�����ʾprint_text
    vkb_ptr->keys[key_idx].subtext_len = 1;
    vkb_ptr->keys[key_idx].subtext[0] = '#';
    cur_char_ptr = (wchar *)key_table_ptr + ((VKEY_0 - VKEY_1 + 2) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
    vkb_ptr->keys[key_idx].text_len = MIN (cur_char_ptr[0], PRINT_TEXT_LEN_LIMIT - 1);      //�ַ�����
    SCI_MEMCPY(vkb_ptr->keys[key_idx].text,
               &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].text_len*sizeof(wchar));
    cur_char_ptr = (wchar *)print_key_table_ptr + ((VKEY_0 - VKEY_1 + 2) * (MMIIM_SP_9KEY_CHAR_MAX + 1));
    vkb_ptr->keys[key_idx].print_text_len = MIN (cur_char_ptr[0], PRINT_TEXT_LEN_LIMIT - 1);      //�ַ�����
    SCI_MEMCPY(vkb_ptr->keys[key_idx].print_text,
               &cur_char_ptr[1],
                   vkb_ptr->keys[key_idx].print_text_len*sizeof(wchar));
    vkb_ptr->keys[key_idx].f_up = ThumbBengaliMultitapCharUp;    
    //--ime lang set
    key_idx++;
    vkb_ptr->keys[key_idx].code = VKEY_LANGUAGE_SET;
    vkb_ptr->keys[key_idx].f_long = PNULL;
    vkb_ptr->keys[key_idx].f_up = VkeyUpLangSet;
    vkb_ptr->keys[key_idx].f_draw = DrawLangSwitchKey;
    if (MMIIM_IsLimitSwitchLang (data_ptr->init_param.method_param_ptr))
    {
        vkb_ptr->keys[key_idx].is_inactive = 1;
    }
    
    //--expand pageup
    key_idx++;
    vkb_ptr->pgup_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEUP;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    //--expand pagedown
    key_idx++;
    vkb_ptr->pgdn_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_PAGEDOWN;
    vkb_ptr->keys[key_idx].tag = KEY_TAG_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;

    //key 21,22 , system - hide - expand
    //������������ͬһ��λ�ã��������, ��ʼ������expand
    key_idx++;
    sys_begin = key_idx;

    for (; key_idx < sys_begin + SYS_CNT; key_idx++)
    {
        ConvertRect(panel_width, panel_height,
                    &(g_thumb_panel_position[1]),
                    &(vkb_ptr->keys[key_idx].rect),
                    0, 0);
        vkb_ptr->keys[key_idx].tag = (uint8)KEY_TAG_NORMAL | KEY_TAG_EXPAND;
        vkb_ptr->keys[key_idx].style = KEY_STYLE_SYSTEM;
        vkb_ptr->keys[key_idx].f_down = VkbDown;
        vkb_ptr->keys[key_idx].f_move = VkbMove;
        vkb_ptr->keys[key_idx].f_long = PNULL;
    }

    //--hide
    key_idx = sys_begin;
    vkb_ptr->hide_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_HIDE;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_HIDE;
    vkb_ptr->keys[key_idx].f_up = VkeyUpHide;
    //--expand  ����������������û��ʹ�ã�ԭ���ǣ�multi-tap���뷨�ĺ�ѡ�ֹ��ܱ����ε���
    key_idx++;
    vkb_ptr->expand_key = vkb_ptr->keys + key_idx;
    vkb_ptr->keys[key_idx].code = VKEY_EXPAND;
    vkb_ptr->keys[key_idx].is_hide = 1;
    vkb_ptr->keys[key_idx].icon = IMG_IMT_EXPAND;

    //key art, ����candidate������, λ������0
    //art Ҫ����candidateǰ�棬��ΪҪ�Ȼ�
    key_idx++;
    vkb_ptr->vessel_key = vkb_ptr->keys + key_idx;
    ConvertRect(panel_width, panel_height,
                &(g_thumb_panel_position[0]),
                &(vkb_ptr->keys[key_idx].rect),
                0, 0);
    vkb_ptr->keys[key_idx].tag = KEY_TAG_NORMAL;
    vkb_ptr->keys[key_idx].code = VKEY_NULL;
    vkb_ptr->keys[key_idx].style = KEY_STYLE_ART;
    vkb_ptr->keys[key_idx].is_inactive = 1;
    MMI_GetLabelTextByLang(IM_TXT_SLIDE_SWITCH_METHOD, &slide_cue);
    vkb_ptr->keys[key_idx].print_text_len = vkb_ptr->keys[key_idx].text_len = MIN (slide_cue.wstr_len, TEXT_LEN_LIMIT - 1);
    MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].text_len);
	MMIAPICOM_Wstrncpy(vkb_ptr->keys[key_idx].print_text, slide_cue.wstr_ptr, vkb_ptr->keys[key_idx].print_text_len);
        
    return;
}


/*==============================================================================
Description: 9���ϼ�����multi-tap���뷨������ͨ�ð汾������Ϊ�ϼ�������������BTRC
			���������Լ���multi-tap���뷨�޷��������������������Ҳʹ�õ���cstar��
			���뷨���棬�ʳ�ʼ�����رյȣ�����smart������̣������ڰ�����Ϣ�����ǿ�
			��Ҫ��������
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliMultitapInit (IM_DATA_T *data_ptr)
{
	BENGALI_MULTITAP_DATA_T *pbengali_data = PNULL;
	
    if (data_ptr == PNULL
            || data_ptr->init_param.method_param_ptr == PNULL
            || data_ptr->init_param.method_param_ptr->im_def_ptr == PNULL)
    {
        SCI_TRACE_LOW("[MMIIM]ThumbBengaliMultitapInit");
        return FALSE;
    }

    data_ptr->data = SCI_ALLOC_APP(sizeof(BENGALI_MULTITAP_DATA_T));
    SCI_MEMSET(data_ptr->data, 0, sizeof(BENGALI_MULTITAP_DATA_T));

    //���̳�ʼ��
    ThumbBengaliMultitapInitKb(data_ptr);

    //�����ʼ��
    pbengali_data = (BENGALI_MULTITAP_DATA_T*)(data_ptr->data);
    SmartInitEngine(&(pbengali_data->engine), 
        data_ptr->init_param.method_param_ptr->im_def_ptr->lang,
        data_ptr->init_param.method_param_ptr->im_def_ptr->method);

    return TRUE;
} 


/*==============================================================================
Description: �ϼ���˽���ַ��ֲ�

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC void BengaliSeparatePrivateChar (wchar private_char, wchar *separate_str)
{
	if (separate_str == PNULL)
	{
		return;
	}

	//0xEAC1 = 0x09cd + 0x09b0, 0xEA4B = 0x09cd + 0x09af, 0xEAAB = 0x09b0 + 0x09cd�����ַ�Ϊ����ַ�����������
	switch (private_char)
	{
		case RAPHALA:
		{
			separate_str[0] = 0x09cd;
			separate_str[1] = 0x09b0;
			separate_str[2] = 0;
			break;
		}

		case JAPHALA:
		{
			separate_str[0] = 0x09cd;
			separate_str[1] = 0x09af;
			separate_str[2] = 0;
			break;
		}

		case RAPH:
		{
			separate_str[0] = 0x09b0;
			separate_str[1] = 0x09cd;
			separate_str[2] = 0;
			break;
		}

		default:
		{
			separate_str[0] = private_char;
			separate_str[1] = 0;
			break;
		}
	}
}


/*==============================================================================
Description: �ϼ������ַ��ύ����һ����ύ��ͬ����Ҫ����Ƿ����˽���ַ���
			����˽���ַ����ֲ���

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC void BengaliCommitChar (IM_DATA_T *data_ptr, wchar c, BOOLEAN real_commit)
{
    GUIIM_NOTIFY_PACK_T notify_pack = {0};
	wchar string[4] = {0};
	wchar cursor_char = 0;
	BENGALI_MULTITAP_DATA_T *pbengali_data = PNULL;
	
    CHECK_DATA_EX(data_ptr);

	SCI_MEMSET (string, 0, sizeof (string));
	BengaliSeparatePrivateChar (c, string);

	pbengali_data = (BENGALI_MULTITAP_DATA_T *)(data_ptr->data);
	cursor_char = pbengali_data->cursor_char;
	
	SCI_MEMSET (&notify_pack, 0, sizeof(notify_pack));
    notify_pack.notify = GUIIM_NOTIFY_COMMIT;

	if (c == RAPH)		//1���������ַ���BT ������underline״̬���������һ���ַ�����ʵ���ύʱ����ɾ�����ַ�
	{
		if (real_commit)
		{
			//��ɾ��underline�ַ�
			notify_pack.data.commit_data.commit[0] = '\b';
			notify_pack.data.commit_data.commit_len = 1;
			GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);

			//ɾ����PAPH��ϵ��Ǹ��ַ�
			GUIIM_CommitData(data_ptr->init_param.im_handle, &notify_pack);
			
			//Ӧ���ύ��2���ַ�����commit��
			SCI_MEMSET (&notify_pack, 0, sizeof(notify_pack));
		    notify_pack.notify = GUIIM_NOTIFY_COMMIT;			
			notify_pack.data.commit_data.commit_len = MMIAPICOM_Wstrlen (string);
		    SCI_MEMCPY(notify_pack.data.commit_data.commit, string,  notify_pack.data.commit_data.commit_len * sizeof(wchar));

			//��cursor�ַ��ŵ�����һ���ύ
			notify_pack.data.commit_data.commit[notify_pack.data.commit_data.commit_len] = cursor_char;
			notify_pack.data.commit_data.commit_len ++;
		}
		else
		{
			//��ǰ���һ��cursor�ַ�һ��commit��ȥ
			notify_pack.data.commit_data.underline_len = MMIAPICOM_Wstrlen (string);
			string[notify_pack.data.commit_data.underline_len] = cursor_char;
			notify_pack.data.commit_data.underline_len ++;
		    SCI_MEMCPY(notify_pack.data.commit_data.underline, string,  notify_pack.data.commit_data.underline_len * sizeof(wchar));
		}
	}
	else
	{
		if (real_commit)
		{
			notify_pack.data.commit_data.commit_len = MMIAPICOM_Wstrlen (string);
		    SCI_MEMCPY(notify_pack.data.commit_data.commit, string,  notify_pack.data.commit_data.commit_len * sizeof(wchar));
		}
		else
		{
			notify_pack.data.commit_data.underline_len = MMIAPICOM_Wstrlen (string);
		    SCI_MEMCPY(notify_pack.data.commit_data.underline, string,  notify_pack.data.commit_data.underline_len * sizeof(wchar));
		}
	}
	
    GUIIM_CommitData (data_ptr->init_param.im_handle, &notify_pack);

    return;
}


/*==============================================================================
Description: �ϼ������⼴�������һ���ַ��Ƿ���ǰ����ַ�һ�𣬷����ϼ���BTRC
			����淶

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC int32 BengaliCheckInputCharValid (wchar cursor_char, wchar input_char)
{
	BOOLEAN is_valid = FALSE;

	if (input_char >= BENGALI_UNICODE_BEGIN && cursor_char <= BENGALI_UNICODE_END)
	{
		if (cursor_char < BENGALI_UNICODE_BEGIN || cursor_char > BENGALI_UNICODE_END)	//�����ϼ�����Χ��,����Ϊ�������׸��ϼ����ַ�
		{
			is_valid = dummy_is_valid_bengali_word_start_char (input_char);
		}
		else  	
		{
			is_valid = dummy_is_valid_bengali_follow_char (cursor_char, input_char);
		}
	}
	else
	{
		is_valid = TRUE;
	}

	return is_valid;
}


/*==============================================================================
Description: �ϼ������ַ�����

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC BENGALI_CHAR_TYPE_E BengaliCharType (wchar c)
{
	int32 iloop = 0;
	BENGALI_CHAR_TYPE_E type = BENGALI_OUT_E;

	if (c < BENGALI_UNICODE_BEGIN || c > BENGALI_UNICODE_END)	//���ϼ����ַ�
	{
		return BENGALI_OUT_E;
	}

	//����
	if (c >= 0x0995 && c <= 0x09A8)		//���ٵ�������ɣ�4��7����˳���Ų���
		return BENGALI_CONSONANT_E;
	
	while (s_bengali_consonant[iloop] != 0)
	{
		if (s_bengali_consonant[iloop] == c)
		{
			return BENGALI_CONSONANT_E;
		}

		iloop ++;
	}

	//����Ԫ��
	iloop = 0;
	while (s_bengali_independent_vowel[iloop] != 0)
	{
		if (s_bengali_independent_vowel[iloop] == c)
		{
			return BENGALI_INDEPENDENT_VOWLE_E;
		}

		iloop ++;
	}

	//�Ƕ���Ԫ��
	iloop = 0;
	while (s_bengali_dependent_vowel[iloop] != 0)
	{
		if (s_bengali_dependent_vowel[iloop] == c)
		{
			return BENGALI_DEPENDENT_VOWLE_E;
		}

		iloop ++;
	}

	if (c >= BENGALI_DIGIT_BEGIN && c <= BENGALI_DIGIT_END)
	{
		return BENGALI_NUMBER_E;
	}
	
	return BENGALI_NOT_DEFINE_E;
}

/*==============================================================================
Description: ��ǰ�����е����а��������Ч����multi-tap����ʱ��Ϊ����ÿ�μ���
			����������Ч���ַ�����һ�ν����е���Ч�����ַ���ȡ����
Global resource dependence: NONE
Author: yaoguang.chen
Note:	����㷨��û��Ч�ʣ���ѵİ취�ǣ�ͨ��key_code���ж����Ǹ������ϵ�
		�ַ�����������������ҪԼ��ÿ�������ϵ��ַ����͵ȵ�

		�����ϼ���BTRC����һ���Ƚ�BT�Ĺ淶��˭Ҳ��֪��ʲôʱ����䣬��������
==============================================================================*/
PUBLIC int32 BengaliGetValidChars (wchar cursor_char, wchar *psrc_text, wchar *ptar_text)
{
	uint16 iloop = 0, valid_index = 0, str_len = 0;
	BENGALI_CHAR_TYPE_E cursor_char_type = BENGALI_NOT_DEFINE_E;
	wchar consonant[TEXT_LEN_LIMIT] = {0};	//����
	wchar independent_vowel[TEXT_LEN_LIMIT] = {0};	//����Ԫ��
	wchar dependent_vowel[TEXT_LEN_LIMIT] = {0};	//�Ƕ���Ԫ��
	wchar digit[TEXT_LEN_LIMIT] = {0};	//����
	wchar buffer[TEXT_LEN_LIMIT] = {0};		
	uint16 consonant_count = 0, independent_count = 0, dependent_count = 0, digit_count = 0;
	
	if (psrc_text == PNULL || ptar_text == PNULL)
	{
		return -1;
	}

	str_len = MMIAPICOM_Wstrlen (psrc_text);
	if (str_len >= TEXT_LEN_LIMIT)
	{
		SCI_TRACE_LOW ("BengaliGetValidChars");
		return -2;
	}

	for (iloop = 0; iloop < str_len; iloop ++)
	{
		if (BengaliCheckInputCharValid (cursor_char, psrc_text[iloop]))	
		{
			buffer[valid_index ++] = psrc_text[iloop];
		}
	}

	//ĳЩ����£�Ԫ���Ͷ���Ԫ����Ҫ����˳��
	SCI_MEMSET (independent_vowel, 0, sizeof (independent_vowel));
	SCI_MEMSET (dependent_vowel, 0, sizeof (dependent_vowel));

	//����cursor�ַ����жϺ����ܹ�������ַ��Ƿ���Ҫ����˳��
	cursor_char_type = BengaliCharType (cursor_char);
	switch (cursor_char_type)
	{
		case BENGALI_CONSONANT_E:		//�����ַ�����Ƕ���Ԫ������ǰ��
		{
			for (iloop = 0; iloop < valid_index; iloop ++)
			{
				if (BengaliCharType (psrc_text[iloop]) == BENGALI_INDEPENDENT_VOWLE_E)	//����Ԫ������
				{
					independent_vowel[independent_count ++] = buffer[iloop];
					buffer[iloop] = 0;		//��Ϊ��ǣ��˴�ֵ�Ѿ�������
				}
				else if (BengaliCharType (psrc_text[iloop]) == BENGALI_DEPENDENT_VOWLE_E)	//�Ƕ���Ԫ��
				{
					dependent_vowel[dependent_count ++] = buffer[iloop];
					buffer[iloop] = 0;		//��Ϊ��ǣ��˴�ֵ�Ѿ�������
				}
			}

			SCI_MEMCPY (ptar_text, dependent_vowel, dependent_count * sizeof (wchar));	//�Ƕ���Ԫ����copy
			ptar_text += dependent_count;
			SCI_MEMCPY (ptar_text, independent_vowel, independent_count * sizeof (wchar));	
			ptar_text += independent_count;

			//ʣ�²���Ҫ������ַ���ȫ�����α����ڶ���Ԫ��֮��
			for (iloop = 0; iloop < valid_index; iloop ++)
			{
				if (buffer[iloop] != 0)
				{
					*ptar_text ++ = buffer[iloop];
				}
			}
			
			break;
		}

		case BENGALI_INDEPENDENT_VOWLE_E:		//����Ԫ��
		case BENGALI_DEPENDENT_VOWLE_E:		//�Ƕ���Ԫ���������ڷǶ���Ԫ��֮��ǰ��cstar�����������������
		case BENGALI_NUMBER_E:		//����
		default:
		{
			SCI_MEMCPY (ptar_text, buffer, valid_index * sizeof (wchar));
			break;
		}
	}

	return 0;
}



/*==============================================================================
Description: ThumbAbcCommitLast
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliMultiTapCommitLast(IM_DATA_T *data_ptr)
{
	BENGALI_MULTITAP_DATA_T *pbengali_data = PNULL;
    wchar last_char = 0;
	uint16 valid_index = 0;

    CHECK_DATA_EX(data_ptr);

    if (data_ptr->multi_tap_data.commit_timer == 0) return FALSE;

    if (data_ptr->multi_tap_data.last_key_index == 0)
        return FALSE;

	pbengali_data = (BENGALI_MULTITAP_DATA_T *)(data_ptr->data);
	if (pbengali_data == PNULL)
	{
		return FALSE;
	}
	
    MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
    data_ptr->multi_tap_data.commit_timer = 0;

	valid_index = data_ptr->multi_tap_data.last_key_index % pbengali_data->count;
	if (valid_index == 0)
	{
		valid_index = pbengali_data->count;
	}
    last_char = pbengali_data->text[valid_index - 1];
	
    BengaliCommitChar (data_ptr, last_char, TRUE);

    data_ptr->multi_tap_data.last_key_index = 0;
	data_ptr->multi_tap_data.last_key = PNULL;	//����ط������٣�������del��enter��֮�󣬸�ֵ������
	
    return TRUE;
}


/*==============================================================================
Description: ThumbAbcCommitCB
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void BengaliMultiTapCommitCB(uint8 timer_id, uint32 param)
{
    IM_DATA_T *data_ptr = (IM_DATA_T *)param;
	BENGALI_MULTITAP_DATA_T *pbengali_data = PNULL;
    wchar last_char = 0;

    if (PNULL == data_ptr
            || (sizeof(IM_DATA_T) != data_ptr->check_info)
            || data_ptr->multi_tap_data.commit_timer != timer_id)
    {
        return;
    }

    CHECK_DATA_EX(data_ptr);

    if (data_ptr->multi_tap_data.commit_timer == 0)
    {
        SCI_TraceLow ("BengaliMultiTapCommitCB");
        return;
    }

    if (data_ptr->multi_tap_data.last_key == PNULL)
    {
        SCI_TraceLow ("BengaliMultiTapCommitCB");
        return;
    }

    if (data_ptr->multi_tap_data.last_key_index == 0)
    {
        SCI_TraceLow ("BengaliMultiTapCommitCB");
        return;
    }

	pbengali_data = (BENGALI_MULTITAP_DATA_T *)(data_ptr->data);
    if (pbengali_data == PNULL)
    {
        SCI_TraceLow ("BengaliMultiTapCommitCB");
        return;
    }

    last_char = pbengali_data->text[data_ptr->multi_tap_data.last_key_index - 1];
	BengaliCommitChar (data_ptr, last_char, TRUE);

    data_ptr->multi_tap_data.commit_timer = 0;
    data_ptr->multi_tap_data.last_key_index = 0;
	data_ptr->multi_tap_data.last_key = PNULL;	//����ط������٣�����һ������ʱ�䵽�󣬹���ǰһ���ַ������˱仯�����뷨��֪��
	
    return;
}


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbBengaliMultitapCharUp

Func:   9���������뷨���ַ���0 ~ 9��Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�İ��������ȼ��ð����Ƿ�Ϊ�ַ�����������������

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-08-02

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbBengaliMultitapCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr)
{
	BENGALI_MULTITAP_DATA_T *pbengali_data = PNULL;
	
    CHECK_DATA_EX(data_ptr);

    if (PNULL == key_ptr || PNULL == data_ptr->vkb.keys || data_ptr->data == PNULL) 
    {
        return TRUE;
    }
    
    if (PNULL == data_ptr->vkb.last_key)
    {
        return TRUE;
    }

	pbengali_data = (BENGALI_MULTITAP_DATA_T*)(data_ptr->data);
	
	//��ͬ����,ǰһ�ΰ�������
    if (data_ptr->multi_tap_data.last_key != key_ptr)
    {
    	MMI_HANDLE_T edit_handle = MMK_GetParentCtrlHandle (data_ptr->init_param.im_handle);
		
        ThumbBengaliMultiTapCommitLast(data_ptr);

        data_ptr->multi_tap_data.last_key = key_ptr;

		//����Ч�ַ���Ϣ��ȡ���������ܷ������棬��Ϊ��underline״̬����ȡ��cursor�ַ����ܲ���
		SCI_MEMSET (pbengali_data, 0, sizeof (BENGALI_MULTITAP_DATA_T));
		pbengali_data->cursor_char = MMIIM_GetEditCursorChar (edit_handle);
		BengaliGetValidChars (pbengali_data->cursor_char, key_ptr->text, pbengali_data->text);
		pbengali_data->count = MMIAPICOM_Wstrlen (pbengali_data->text);
		
        //reset index
        data_ptr->multi_tap_data.last_key_index = 0;
    }

    if (data_ptr->multi_tap_data.commit_timer > 0)
    {
        MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
        data_ptr->multi_tap_data.commit_timer = 0;
    }

	//��ǰ������Ϣ����
    if (1 == pbengali_data->count)
    {
        BengaliCommitChar (data_ptr, pbengali_data->text[0], TRUE);
    }
    else if (pbengali_data->count > 1)
    {
        //set underline character
        data_ptr->multi_tap_data.last_key_index += 1;
        if (data_ptr->multi_tap_data.last_key_index > pbengali_data->count)
        {
            data_ptr->multi_tap_data.last_key_index = 1;
        }
		
		BengaliCommitChar (data_ptr, pbengali_data->text[data_ptr->multi_tap_data.last_key_index - 1], FALSE);
        data_ptr->multi_tap_data.commit_timer = MMK_CreateTimerCallback(
                    ABC_COMMIT_TICK,
                    BengaliMultiTapCommitCB,
                    (uint32)data_ptr,
                    FALSE);
    }

    VkbUp(data_ptr, key_ptr);

    return TRUE;
}


/*==============================================================================
Description: 

Global resource dependence: NONE

Author: yaoguang.chen

Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbBengaliMultitapTerm (IM_DATA_T *data_ptr)
{
    CHECK_DATA(data_ptr);

	//multi-tap��ش���ر�
    if (data_ptr->multi_tap_data.commit_timer > 0)
    {
        MMK_StopTimer(data_ptr->multi_tap_data.commit_timer);
        data_ptr->multi_tap_data.commit_timer = 0;
    }
	
    SCI_FREEIF(data_ptr->data);
    ReleaseVkb(&(data_ptr->vkb));

    return TRUE;
}


#endif

#endif //defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_CSTAR)


