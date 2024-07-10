
/***************************************************************************
 *                                                                         *
 * Copyright (C) 2000-2007 Beijing Komoxo Inc.                             *
 * All rights reserved.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "IIMEk2api.h"

#include "kmxime_latin.h"

/****************************************************************************
 *
 * Language database configurations.
 *
 ***************************************************************************/
/*
#include "ldb_english_107.h"
	const void* kmx_ldb_english = &LDB_107;
	const ilME_IME_INFO ilME_ldb_english =
	{
		IM_ENGLISH,
		&LDB_107
	};
*/
#if defined(ilME_ALL_IN_ONE_DATABASE)
        #include "ldb_arabic_800.h"
        #include "ldb_arabic_801.h"
        #include "ldb_arabic_802.h"
        #include "ldb_arabic_803.h"
        #include "ldb_arabic_804.h"
        #include "ldb_arabic_805.h"
        #include "ldb_arabic_806.h"
        #include "ldb_arabic_807.h"
        #include "ldb_arabic_8071.h"
        #include "ldb_arabic_808.h"
        #include "ldb_arabic_809.h"
        #include "ldb_arabic_8000.h"
        #include "ldb_arabic_8001.h"
        #include "ldb_arabic_8011.h"
        #include "ldb_arabic_8016.h"

        #include "ldb_hindi_900.h"
        #include "ldb_hindi_902.h"
        #include "ldb_hindi_903.h"
        #include "ldb_hindi_904.h"
        #include "ldb_hindi_905.h"
        #include "ldb_hindi_906.h"
        #include "ldb_hindi_907.h"
        #include "ldb_hindi_9071.h"
        #include "ldb_hindi_908.h"
        #include "ldb_hindi_9000.h"
        #include "ldb_hindi_9001.h"
        #include "ldb_hindi_9002.h"
        #include "ldb_hindi_9008.h"
        #include "ldb_hindi_9010.h"
        #include "ldb_hindi_9012.h"
        #include "ldb_hindi_9013.h"
        #include "ldb_hindi_9015.h"
        #include "ldb_hindi_9017.h"
        #include "ldb_hindi_9018.h"

        #include "ldb_hinglish_910.h"
        #include "ldb_hinglish_915.h"
        #include "ldb_hinglish_917.h"
        #include "ldb_hinglish_9171.h"

    const void *kmx_ldb_arabic_800 = &LDB_800;
    const void *kmx_ldb_arabic_801 = &LDB_801;
    const void *kmx_ldb_arabic_802 = &LDB_802;
    const void *kmx_ldb_arabic_803 = &LDB_803;
    const void *kmx_ldb_arabic_804 = &LDB_804;
    const void *kmx_ldb_arabic_805 = &LDB_805;
    const void *kmx_ldb_arabic_806 = &LDB_806;
    const void *kmx_ldb_arabic_807 = &LDB_807;
    const void *kmx_ldb_arabic_8071 = &LDB_8071;
    const void *kmx_ldb_arabic_808 = &LDB_808;
    const void *kmx_ldb_arabic_809 = &LDB_809;
    const void *kmx_ldb_arabic_8000 = &LDB_8000;
    const void *kmx_ldb_arabic_8001 = &LDB_8001;
    const void *kmx_ldb_arabic_8011 = &LDB_8011;
    const void *kmx_ldb_arabic_8016 = &LDB_8016;

    const void *kmx_ldb_hindi_900 = &LDB_900;
    const void *kmx_ldb_hindi_902 = &LDB_902;
    const void *kmx_ldb_hindi_903 = &LDB_903;
    const void *kmx_ldb_hindi_904 = &LDB_904;    
    const void *kmx_ldb_hindi_905 = &LDB_905;
    const void *kmx_ldb_hindi_906 = &LDB_906;
    const void *kmx_ldb_hindi_907 = &LDB_907;
    const void *kmx_ldb_hindi_9071 = &LDB_9071;
    const void *kmx_ldb_hindi_908 = &LDB_908;
    const void *kmx_ldb_hindi_9000 = &LDB_9000;
    const void *kmx_ldb_hindi_9001 = &LDB_9001;
    const void *kmx_ldb_hindi_9002 = &LDB_9002;
    const void *kmx_ldb_hindi_9008 = &LDB_9008;
    const void *kmx_ldb_hindi_9010 = &LDB_9010;
    const void *kmx_ldb_hindi_9012 = &LDB_9012;
    const void *kmx_ldb_hindi_9013 = &LDB_9013;
    const void *kmx_ldb_hindi_9015 = &LDB_9015;
    const void *kmx_ldb_hindi_9017 = &LDB_9017;
    const void *kmx_ldb_hindi_9018 = &LDB_9018;

    const void *kmx_ldb_hinglish_910 = &LDB_910;
    const void *kmx_ldb_hinglish_915 = &LDB_915;
    const void *kmx_ldb_hinglish_917 = &LDB_917;
    const void *kmx_ldb_hinglish_9171 = &LDB_9171;

    const ilME_IME_INFO ilME_ldb_arabic_800 = {IM_ARABIC, &LDB_800};
    const ilME_IME_INFO ilME_ldb_arabic_801 = {IM_ARABIC, &LDB_801};
    const ilME_IME_INFO ilME_ldb_arabic_802 = {IM_ARABIC, &LDB_802};
    const ilME_IME_INFO ilME_ldb_arabic_803 = {IM_ARABIC, &LDB_803};
    const ilME_IME_INFO ilME_ldb_arabic_804 = {IM_ARABIC, &LDB_804};
    const ilME_IME_INFO ilME_ldb_arabic_805 = {IM_ARABIC, &LDB_805};
    const ilME_IME_INFO ilME_ldb_arabic_806 = {IM_ARABIC, &LDB_806};
    const ilME_IME_INFO ilME_ldb_arabic_807 = {IM_ARABIC, &LDB_807};
    const ilME_IME_INFO ilME_ldb_arabic_8071 = {IM_ARABIC, &LDB_8071};
    const ilME_IME_INFO ilME_ldb_arabic_808 = {IM_ARABIC, &LDB_808};
    const ilME_IME_INFO ilME_ldb_arabic_809 = {IM_ARABIC, &LDB_809};
    const ilME_IME_INFO ilME_ldb_arabic_8000 = {IM_ARABIC, &LDB_8000};
    const ilME_IME_INFO ilME_ldb_arabic_8001 = {IM_ARABIC, &LDB_8001};
    const ilME_IME_INFO ilME_ldb_arabic_8011 = {IM_ARABIC, &LDB_8011};
    const ilME_IME_INFO ilME_ldb_arabic_8016 = {IM_ARABIC, &LDB_8016};

    const ilME_IME_INFO ilME_ldb_hindi_900 = {IM_HINDI, &LDB_900};
    const ilME_IME_INFO ilME_ldb_hindi_902 = {IM_HINDI, &LDB_902};
    const ilME_IME_INFO ilME_ldb_hindi_903 = {IM_HINDI, &LDB_903};
    const ilME_IME_INFO ilME_ldb_hindi_904 = {IM_HINDI, &LDB_904};
    const ilME_IME_INFO ilME_ldb_hindi_905 = {IM_HINDI, &LDB_905};
    const ilME_IME_INFO ilME_ldb_hindi_906 = {IM_HINDI, &LDB_906};
    const ilME_IME_INFO ilME_ldb_hindi_907 = {IM_HINDI, &LDB_907};
    const ilME_IME_INFO ilME_ldb_hindi_9071 = {IM_HINDI, &LDB_9071};
    const ilME_IME_INFO ilME_ldb_hindi_908 = {IM_HINDI, &LDB_908};
    const ilME_IME_INFO ilME_ldb_hindi_9000 = {IM_HINDI, &LDB_9000};
    const ilME_IME_INFO ilME_ldb_hindi_9001 = {IM_HINDI, &LDB_9001};
    const ilME_IME_INFO ilME_ldb_hindi_9002 = {IM_HINDI, &LDB_9002};
    const ilME_IME_INFO ilME_ldb_hindi_9008 = {IM_HINDI, &LDB_9008};
    const ilME_IME_INFO ilME_ldb_hindi_9010 = {IM_HINDI, &LDB_9010};
    const ilME_IME_INFO ilME_ldb_hindi_9012 = {IM_HINDI, &LDB_9012};
    const ilME_IME_INFO ilME_ldb_hindi_9013 = {IM_HINDI, &LDB_9013};
    const ilME_IME_INFO ilME_ldb_hindi_9015 = {IM_HINDI, &LDB_9015};
    const ilME_IME_INFO ilME_ldb_hindi_9017 = {IM_HINDI, &LDB_9017};
    const ilME_IME_INFO ilME_ldb_hindi_9018 = {IM_HINDI, &LDB_9018};

    const ilME_IME_INFO ilME_ldb_hinglish_910 = {IM_HINGLISH, &LDB_910};
    const ilME_IME_INFO ilME_ldb_hinglish_915 = {IM_HINGLISH, &LDB_915};
    const ilME_IME_INFO ilME_ldb_hinglish_917 = {IM_HINGLISH, &LDB_917};

	//ZHMCH	已经在mxime_latin_ldb.c文件中定义!
    const ilME_IME_INFO ilME_ldb_nepali_720 = {IM_NEPALI, &LDB_900};	//IM_NEPALI_SUPPORT	使用的是印地语最底层的ldb数据

#else//ilME_ALL_IN_ONE_DATABASE

  /* Arabic */
  
#if defined(KMX_LDB_ARABIC)
    #if KMX_LDB_ARABIC == 800
        #include "ldb_arabic_800.h"
		  const void* kmx_ldb_arabic = &LDB_800;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_800
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_800")
        #endif
    #elif KMX_LDB_ARABIC == 801
        #include "ldb_arabic_801.h"
		  const void* kmx_ldb_arabic = &LDB_801;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_801
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_801")
        #endif
    #elif KMX_LDB_ARABIC == 802
        #include "ldb_arabic_802.h"
		  const void* kmx_ldb_arabic = &LDB_802;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_802
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_802")
        #endif        
    #elif KMX_LDB_ARABIC == 803
        #include "ldb_arabic_803.h"
		  const void* kmx_ldb_arabic = &LDB_803;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_803
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_803")
        #endif        
    #elif KMX_LDB_ARABIC == 804
        #include "ldb_arabic_804.h"
		  const void* kmx_ldb_arabic = &LDB_804;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_804
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_804")
        #endif        
    #elif KMX_LDB_ARABIC == 805
        #include "ldb_arabic_805.h"
		  const void* kmx_ldb_arabic = &LDB_805;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_805
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_805")
        #endif
    #elif KMX_LDB_ARABIC == 806
        #include "ldb_arabic_806.h"
		  const void* kmx_ldb_arabic = &LDB_806;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_806
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_806")
        #endif
    #elif KMX_LDB_ARABIC == 807
        #include "ldb_arabic_807.h"
		  const void* kmx_ldb_arabic = &LDB_807;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_807
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_807")
        #endif
    #elif KMX_LDB_ARABIC == 8071
        #include "ldb_arabic_8071.h"
		  const void* kmx_ldb_arabic = &LDB_8071;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_8071
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_8071")
        #endif
    #elif KMX_LDB_ARABIC == 808
        #include "ldb_arabic_808.h"
		  const void* kmx_ldb_arabic = &LDB_808;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_808
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_808")
        #endif
    #elif KMX_LDB_ARABIC == 809
        #include "ldb_arabic_809.h"
		  const void* kmx_ldb_arabic = &LDB_809;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_809
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_809")
        #endif
    #elif KMX_LDB_ARABIC == 8000
        #include "ldb_arabic_8000.h"
		  const void* kmx_ldb_arabic = &LDB_8000;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_8000
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_8000")
        #endif
    #elif KMX_LDB_ARABIC == 8001
        #include "ldb_arabic_8001.h"
		  const void* kmx_ldb_arabic = &LDB_8001;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_8001
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_8001")
        #endif
    #elif KMX_LDB_ARABIC == 8011
        #include "ldb_arabic_8011.h"
		  const void* kmx_ldb_arabic = &LDB_8011;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_8011
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_8011")
        #endif
    #elif KMX_LDB_ARABIC == 8016
        #include "ldb_arabic_8016.h"
		  const void* kmx_ldb_arabic = &LDB_8016;
		  const ilME_IME_INFO ilME_ldb_arabic =
		  {
			  IM_ARABIC,
			  &LDB_8016
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARABIC_8016")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Arabic")
            #endif            
        #else
            #error "Arabic database not defined!"
        #endif
  
    #endif
#endif
  
  /* Hindi */
  
#if defined(KMX_LDB_HINDI)
    #if KMX_LDB_HINDI == 900
        #include "ldb_hindi_900.h"
		  const void* kmx_ldb_hindi = &LDB_900;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_900
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_900")
        #endif
    #elif KMX_LDB_HINDI == 902
        #include "ldb_hindi_902.h"
		  const void* kmx_ldb_hindi = &LDB_902;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_902
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_902")
        #endif        
    #elif KMX_LDB_HINDI == 903
        #include "ldb_hindi_903.h"
		  const void* kmx_ldb_hindi = &LDB_903;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_903
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_903")
        #endif
    #elif KMX_LDB_HINDI == 904
        #include "ldb_hindi_904.h"
		  const void* kmx_ldb_hindi = &LDB_904;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_904
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_904")
        #endif
    #elif KMX_LDB_HINDI == 905
        #include "ldb_hindi_905.h"
		  const void* kmx_ldb_hindi = &LDB_905;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_905
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_905")
        #endif
    #elif KMX_LDB_HINDI == 906
        #include "ldb_hindi_906.h"
		  const void* kmx_ldb_hindi = &LDB_906;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_906
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_906")
        #endif
    #elif KMX_LDB_HINDI == 907
        #include "ldb_hindi_907.h"
		  const void* kmx_ldb_hindi = &LDB_907;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_907
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_907")
        #endif
    #elif KMX_LDB_HINDI == 9071
        #include "ldb_hindi_9071.h"
		  const void* kmx_ldb_hindi = &LDB_9071;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9071
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9071")
        #endif
    #elif KMX_LDB_HINDI == 908
        #include "ldb_hindi_908.h"
		  const void* kmx_ldb_hindi = &LDB_908;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_908
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_908")
        #endif
    #elif KMX_LDB_HINDI == 9000
        #include "ldb_hindi_9000.h"
		  const void* kmx_ldb_hindi = &LDB_9000;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9000
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9000")
        #endif
    #elif KMX_LDB_HINDI == 9001
        #include "ldb_hindi_9001.h"
		  const void* kmx_ldb_hindi = &LDB_9001;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9001
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9001")
        #endif
    #elif KMX_LDB_HINDI == 9002
        #include "ldb_hindi_9002.h"
		  const void* kmx_ldb_hindi = &LDB_9002;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9002
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9002")
        #endif
    #elif KMX_LDB_HINDI == 9008
        #include "ldb_hindi_9008.h"
		  const void* kmx_ldb_hindi = &LDB_9008;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9008
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9008")
        #endif
    #elif KMX_LDB_HINDI == 9010
        #include "ldb_hindi_9010.h"
		  const void* kmx_ldb_hindi = &LDB_9010;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9010
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9010")
        #endif
    #elif KMX_LDB_HINDI == 9012
        #include "ldb_hindi_9012.h"
		  const void* kmx_ldb_hindi = &LDB_9012;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9012
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9012")
        #endif
    #elif KMX_LDB_HINDI == 9013
        #include "ldb_hindi_9013.h"
		  const void* kmx_ldb_hindi = &LDB_9013;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9013
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9013")
        #endif
    #elif KMX_LDB_HINDI == 9015
        #include "ldb_hindi_9015.h"
		  const void* kmx_ldb_hindi = &LDB_9015;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9015
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9015")
        #endif
    #elif KMX_LDB_HINDI == 9017
        #include "ldb_hindi_9017.h"
		  const void* kmx_ldb_hindi = &LDB_9017;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9017
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9017")
        #endif
    #elif KMX_LDB_HINDI == 9018
        #include "ldb_hindi_9018.h"
		  const void* kmx_ldb_hindi = &LDB_9018;
		  const ilME_IME_INFO ilME_ldb_hindi =
		  {
			  IM_HINDI,
			  &LDB_9018
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINDI_9018")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Hindi")
            #endif            
        #else
            #error "Hindi database not defined!"
        #endif
  
    #endif
#endif
  
  /* Nepali */
  //已经在Kmxime_latin_ldb.c文件中定义!
#if defined(KMX_LDB_NEPALI)
    #if KMX_LDB_NEPALI == 720
		  //#include "ldb_nepali_720.h"
        #include "ldb_hindi_900.h"
		  const void* kmx_ldb_nepali = &LDB_900;
		  const ilME_IME_INFO ilME_ldb_nepali =
		  {
			  IM_NEPALI,
			  &LDB_900
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_NEPALI_720")
        #endif
	#else
		  //#include "ldb_nepali_720.h"
        #include "ldb_hindi_900.h"
		  const void* kmx_ldb_nepali = &LDB_900;
		  const ilME_IME_INFO ilME_ldb_nepali =
		  {
			  IM_NEPALI,
			  &LDB_900
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_NEPALI_720")
        #endif	
    #endif
#endif
  
  
  /* Hinglish */
  
#if defined(KMX_LDB_HINGLISH)
    #if KMX_LDB_HINGLISH == 910
        #include "ldb_hinglish_910.h"
		  const void* kmx_ldb_hinglish = &LDB_910;
		  const ilME_IME_INFO ilME_ldb_hinglish =
		  {
			  IM_HINGLISH,
			  &LDB_910
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINGLISH_910")
        #endif
    #elif KMX_LDB_HINGLISH == 915
        #include "ldb_hinglish_915.h"
		  const void* kmx_ldb_hinglish = &LDB_915;
		  const ilME_IME_INFO ilME_ldb_hinglish =
		  {
			  IM_HINGLISH,
			  &LDB_915
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINGLISH_915")
        #endif
    #elif KMX_LDB_HINGLISH == 917
        #include "ldb_hinglish_917.h"
		  const void* kmx_ldb_hinglish = &LDB_917;
		  const ilME_IME_INFO ilME_ldb_hinglish =
		  {
			  IM_HINGLISH,
			  &LDB_917
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINGLISH_917")
        #endif
    #elif KMX_LDB_HINGLISH == 9171
        #include "ldb_hinglish_9171.h"
		  const void* kmx_ldb_hinglish = &LDB_9171;
		  const ilME_IME_INFO ilME_ldb_hinglish =
		  {
			  IM_HINGLISH,
			  &LDB_9171
		  };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HINGLISH_9171")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Hinglish")
            #endif            
        #else
            #error "Hinglish database not defined!"
        #endif
  
    #endif
#endif

	#include "ldb_hindi_900.h"
	#include "ldb_hindi_902.h"
	#include "ldb_hindi_903.h"
	#include "ldb_hindi_904.h"
	#include "ldb_hindi_905.h"
	#include "ldb_hindi_906.h"
	#include "ldb_hindi_907.h"
	#include "ldb_hindi_9071.h"
	#include "ldb_hindi_908.h"
	#include "ldb_hindi_9000.h"
	#include "ldb_hindi_9001.h"
	#include "ldb_hindi_9002.h"
	#include "ldb_hindi_9008.h"
	#include "ldb_hindi_9010.h"
	#include "ldb_hindi_9012.h"
	#include "ldb_hindi_9013.h"
	#include "ldb_hindi_9015.h"
	#include "ldb_hindi_9017.h"
	#include "ldb_hindi_9018.h"

	#include "ldb_hinglish_910.h"
	#include "ldb_hinglish_915.h"
	#include "ldb_hinglish_917.h"
	#include "ldb_hinglish_9171.h"

const void *kmx_ldb_arabic_800 = &LDB_800;
const void *kmx_ldb_arabic_801 = &LDB_801;
const void *kmx_ldb_arabic_802 = &LDB_802;
const void *kmx_ldb_arabic_803 = &LDB_803;
const void *kmx_ldb_arabic_804 = &LDB_804;
const void *kmx_ldb_arabic_805 = &LDB_805;
const void *kmx_ldb_arabic_806 = &LDB_806;
const void *kmx_ldb_arabic_807 = &LDB_807;
const void *kmx_ldb_arabic_8071 = &LDB_8071;
const void *kmx_ldb_arabic_808 = &LDB_808;
const void *kmx_ldb_arabic_809 = &LDB_809;
const void *kmx_ldb_arabic_8000 = &LDB_8000;
const void *kmx_ldb_arabic_8001 = &LDB_8001;
const void *kmx_ldb_arabic_8011 = &LDB_8011;
const void *kmx_ldb_arabic_8016 = &LDB_8016;

const void *kmx_ldb_hindi_900 = &LDB_900;
const void *kmx_ldb_hindi_902 = &LDB_902;
const void *kmx_ldb_hindi_903 = &LDB_903;
const void *kmx_ldb_hindi_904 = &LDB_904;	 
const void *kmx_ldb_hindi_905 = &LDB_905;
const void *kmx_ldb_hindi_906 = &LDB_906;
const void *kmx_ldb_hindi_907 = &LDB_907;
const void *kmx_ldb_hindi_9071 = &LDB_9071;
const void *kmx_ldb_hindi_908 = &LDB_908;
const void *kmx_ldb_hindi_9000 = &LDB_9000;
const void *kmx_ldb_hindi_9001 = &LDB_9001;
const void *kmx_ldb_hindi_9002 = &LDB_9002;
const void *kmx_ldb_hindi_9008 = &LDB_9008;
const void *kmx_ldb_hindi_9010 = &LDB_9010;
const void *kmx_ldb_hindi_9012 = &LDB_9012;
const void *kmx_ldb_hindi_9013 = &LDB_9013;
const void *kmx_ldb_hindi_9015 = &LDB_9015;
const void *kmx_ldb_hindi_9017 = &LDB_9017;
const void *kmx_ldb_hindi_9018 = &LDB_9018;

const void *kmx_ldb_hinglish_910 = &LDB_910;
const void *kmx_ldb_hinglish_915 = &LDB_915;
const void *kmx_ldb_hinglish_917 = &LDB_917;
const void *kmx_ldb_hinglish_9171 = &LDB_9171;

const ilME_IME_INFO ilME_ldb_arabic_800 = {IM_ARABIC, &LDB_800};
const ilME_IME_INFO ilME_ldb_arabic_801 = {IM_ARABIC, &LDB_801};
const ilME_IME_INFO ilME_ldb_arabic_802 = {IM_ARABIC, &LDB_802};
const ilME_IME_INFO ilME_ldb_arabic_803 = {IM_ARABIC, &LDB_803};
const ilME_IME_INFO ilME_ldb_arabic_804 = {IM_ARABIC, &LDB_804};
const ilME_IME_INFO ilME_ldb_arabic_805 = {IM_ARABIC, &LDB_805};
const ilME_IME_INFO ilME_ldb_arabic_806 = {IM_ARABIC, &LDB_806};
const ilME_IME_INFO ilME_ldb_arabic_807 = {IM_ARABIC, &LDB_807};
const ilME_IME_INFO ilME_ldb_arabic_8071 = {IM_ARABIC, &LDB_8071};
const ilME_IME_INFO ilME_ldb_arabic_808 = {IM_ARABIC, &LDB_808};
const ilME_IME_INFO ilME_ldb_arabic_809 = {IM_ARABIC, &LDB_809};
const ilME_IME_INFO ilME_ldb_arabic_8000 = {IM_ARABIC, &LDB_8000};
const ilME_IME_INFO ilME_ldb_arabic_8001 = {IM_ARABIC, &LDB_8001};
const ilME_IME_INFO ilME_ldb_arabic_8011 = {IM_ARABIC, &LDB_8011};
const ilME_IME_INFO ilME_ldb_arabic_8016 = {IM_ARABIC, &LDB_8016};

const ilME_IME_INFO ilME_ldb_hindi_900 = {IM_HINDI, &LDB_900};
const ilME_IME_INFO ilME_ldb_hindi_902 = {IM_HINDI, &LDB_902};
const ilME_IME_INFO ilME_ldb_hindi_903 = {IM_HINDI, &LDB_903};
const ilME_IME_INFO ilME_ldb_hindi_904 = {IM_HINDI, &LDB_904};
const ilME_IME_INFO ilME_ldb_hindi_905 = {IM_HINDI, &LDB_905};
const ilME_IME_INFO ilME_ldb_hindi_906 = {IM_HINDI, &LDB_906};
const ilME_IME_INFO ilME_ldb_hindi_907 = {IM_HINDI, &LDB_907};
const ilME_IME_INFO ilME_ldb_hindi_9071 = {IM_HINDI, &LDB_9071};
const ilME_IME_INFO ilME_ldb_hindi_908 = {IM_HINDI, &LDB_908};
const ilME_IME_INFO ilME_ldb_hindi_9000 = {IM_HINDI, &LDB_9000};
const ilME_IME_INFO ilME_ldb_hindi_9001 = {IM_HINDI, &LDB_9001};
const ilME_IME_INFO ilME_ldb_hindi_9002 = {IM_HINDI, &LDB_9002};
const ilME_IME_INFO ilME_ldb_hindi_9008 = {IM_HINDI, &LDB_9008};
const ilME_IME_INFO ilME_ldb_hindi_9010 = {IM_HINDI, &LDB_9010};
const ilME_IME_INFO ilME_ldb_hindi_9012 = {IM_HINDI, &LDB_9012};
const ilME_IME_INFO ilME_ldb_hindi_9013 = {IM_HINDI, &LDB_9013};
const ilME_IME_INFO ilME_ldb_hindi_9015 = {IM_HINDI, &LDB_9015};
const ilME_IME_INFO ilME_ldb_hindi_9017 = {IM_HINDI, &LDB_9017};
const ilME_IME_INFO ilME_ldb_hindi_9018 = {IM_HINDI, &LDB_9018};

const ilME_IME_INFO ilME_ldb_hinglish_910 = {IM_HINGLISH, &LDB_910};
const ilME_IME_INFO ilME_ldb_hinglish_915 = {IM_HINGLISH, &LDB_915};
const ilME_IME_INFO ilME_ldb_hinglish_917 = {IM_HINGLISH, &LDB_917};

//ZHMCH 已经在mxime_latin_ldb.c文件中定义!
const ilME_IME_INFO ilME_ldb_nepali_720 = {IM_NEPALI, &LDB_900};	//IM_NEPALI_SUPPORT 使用的是印地语最底层的ldb数据


#endif



void huhuhuh(void)
{
    //int iii = KMX_LDB_HINDI;
	//经确认，是900!!!!!
	//iii;
}
