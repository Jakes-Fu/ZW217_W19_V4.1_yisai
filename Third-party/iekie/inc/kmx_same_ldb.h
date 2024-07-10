
#include "IIMEk2api.h"

#ifndef __KMX_SAME_LDB_H__
#define __KMX_SAME_LDB_H__

/*
 *  此文件，用于添加未打开IIME_ALL_IN_ONE_DATABASE情况下为了满足特定语言ldb且节省空间必须的ldb数据
 */
#if defined(IEKIE_INPUT_SAME_LANGUAGE_ALL_LDB)
  #if defined(KMX_LDB_ENGLISH)
  extern const void *kmx_ldb_english_100;
  extern const void *kmx_ldb_english_101;
  extern const void *kmx_ldb_english_102;
  extern const void *kmx_ldb_english_103;
  extern const void *kmx_ldb_english_104;
  extern const void *kmx_ldb_english_105;
  extern const void *kmx_ldb_english_106;
  extern const void *kmx_ldb_english_107;
  extern const void *kmx_ldb_english_1071;
  extern const void *kmx_ldb_english_108;
  extern const void *kmx_ldb_english_109;
  extern const void *kmx_ldb_english_10001;

  extern const IIME_IME_INFO IIME_ldb_english_100;
  extern const IIME_IME_INFO IIME_ldb_english_101;
  extern const IIME_IME_INFO IIME_ldb_english_102;
  extern const IIME_IME_INFO IIME_ldb_english_103;
  extern const IIME_IME_INFO IIME_ldb_english_104;
  extern const IIME_IME_INFO IIME_ldb_english_105;
  extern const IIME_IME_INFO IIME_ldb_english_106;
  extern const IIME_IME_INFO IIME_ldb_english_107;
  extern const IIME_IME_INFO IIME_ldb_english_1071;
  extern const IIME_IME_INFO IIME_ldb_english_108;
  extern const IIME_IME_INFO IIME_ldb_english_109;
  extern const IIME_IME_INFO IIME_ldb_english_10001;
  #else
  extern const IIME_IME_INFO IIME_ldb_english_107;
  #endif/*KMX_LDB_ENGLISH*/


#if defined(KMX_LDB_ARABIC)
  extern const void *kmx_ldb_arabic_800;
  extern const void *kmx_ldb_arabic_801;
  extern const void *kmx_ldb_arabic_802;
  extern const void *kmx_ldb_arabic_803;
  extern const void *kmx_ldb_arabic_804;
  extern const void *kmx_ldb_arabic_805;
  extern const void *kmx_ldb_arabic_806;
  extern const void *kmx_ldb_arabic_807;
  extern const void *kmx_ldb_arabic_8071;
  extern const void *kmx_ldb_arabic_808;
  extern const void *kmx_ldb_arabic_809;
  extern const void *kmx_ldb_arabic_8000;
  extern const void *kmx_ldb_arabic_8001;
  extern const void *kmx_ldb_arabic_8011;
  extern const void *kmx_ldb_arabic_8016;
  
  extern const IIME_IME_INFO IIME_ldb_arabic_800;
  extern const IIME_IME_INFO IIME_ldb_arabic_801;
  extern const IIME_IME_INFO IIME_ldb_arabic_802;
  extern const IIME_IME_INFO IIME_ldb_arabic_803;
  extern const IIME_IME_INFO IIME_ldb_arabic_804;
  extern const IIME_IME_INFO IIME_ldb_arabic_805;
  extern const IIME_IME_INFO IIME_ldb_arabic_806;
  extern const IIME_IME_INFO IIME_ldb_arabic_807;
  extern const IIME_IME_INFO IIME_ldb_arabic_8071;
  extern const IIME_IME_INFO IIME_ldb_arabic_808;
  extern const IIME_IME_INFO IIME_ldb_arabic_809;
  extern const IIME_IME_INFO IIME_ldb_arabic_8000;
  extern const IIME_IME_INFO IIME_ldb_arabic_8001;
  extern const IIME_IME_INFO IIME_ldb_arabic_8011;
  extern const IIME_IME_INFO IIME_ldb_arabic_8016;
#endif/*KMX_LDB_ARABIC*/

#if defined(KMX_LDB_ARMENIAN)
  extern const void *kmx_ldb_armenian_570;
  extern const void *kmx_ldb_armenian_575;
  extern const void *kmx_ldb_armenian_577;
  extern const IIME_IME_INFO IIME_ldb_armenian_570;
  extern const IIME_IME_INFO IIME_ldb_armenian_575;
  extern const IIME_IME_INFO IIME_ldb_armenian_577;
#endif/*KMX_LDB_ARABIC*/

    /*
     * latin ldb
     */
#ifdef KMX_LDB_BENGALI
  extern const void *kmx_ldb_bengali_920;
  extern const void *kmx_ldb_bengali_921;
  extern const void *kmx_ldb_bengali_922;
  extern const void *kmx_ldb_bengali_923;
  extern const void *kmx_ldb_bengali_924;
  extern const void *kmx_ldb_bengali_925;
  extern const void *kmx_ldb_bengali_927;
  extern const void *kmx_ldb_bengali_928;
  extern const void *kmx_ldb_bengali_929;
  extern const void *kmx_ldb_bengali_9200;
  extern const void *kmx_ldb_bengali_9202;    
  extern const void *kmx_ldb_bengali_9203;    
  extern const IIME_IME_INFO IIME_ldb_bengali_920;
  extern const IIME_IME_INFO IIME_ldb_bengali_921;
  extern const IIME_IME_INFO IIME_ldb_bengali_922;
  extern const IIME_IME_INFO IIME_ldb_bengali_923;
  extern const IIME_IME_INFO IIME_ldb_bengali_924;
  extern const IIME_IME_INFO IIME_ldb_bengali_925;
  extern const IIME_IME_INFO IIME_ldb_bengali_927;
  extern const IIME_IME_INFO IIME_ldb_bengali_928;
  extern const IIME_IME_INFO IIME_ldb_bengali_929;
  extern const IIME_IME_INFO IIME_ldb_bengali_9200;
  extern const IIME_IME_INFO IIME_ldb_bengali_9201;
  extern const IIME_IME_INFO IIME_ldb_bengali_9202;    
  extern const IIME_IME_INFO IIME_ldb_bengali_9203; 
#endif/*KMX_LDB_BENGALI*/

#ifdef KMX_LDB_BULGARIAN
  extern const void *kmx_ldb_bulgarian_430;
  extern const void *kmx_ldb_bulgarian_435;
  extern const void *kmx_ldb_bulgarian_437;
  extern const IIME_IME_INFO IIME_ldb_bulgarian_430;
  extern const IIME_IME_INFO IIME_ldb_bulgarian_435;
  extern const IIME_IME_INFO IIME_ldb_bulgarian_437;
#endif/*KMX_LDB_BULGARIAN*/

#ifdef KMX_LDB_CROATIAN
  extern const void *kmx_ldb_croatian_510;
  extern const void *kmx_ldb_croatian_515;
  extern const void *kmx_ldb_croatian_517;
  extern const IIME_IME_INFO IIME_ldb_croatian_510;
  extern const IIME_IME_INFO IIME_ldb_croatian_515;
  extern const IIME_IME_INFO IIME_ldb_croatian_517;
#endif/*KMX_LDB_CROATIAN*/

#ifdef KMX_LDB_CZECH
  extern const void *kmx_ldb_czech_440;
  extern const void *kmx_ldb_czech_442;
  extern const void *kmx_ldb_czech_443;
  extern const void *kmx_ldb_czech_445;
  extern const void *kmx_ldb_czech_447;
  extern const IIME_IME_INFO IIME_ldb_czech_440;
  extern const IIME_IME_INFO IIME_ldb_czech_442;
  extern const IIME_IME_INFO IIME_ldb_czech_443;
  extern const IIME_IME_INFO IIME_ldb_czech_445;
  extern const IIME_IME_INFO IIME_ldb_czech_447;
#endif/*KMX_LDB_CZECH*/

#ifdef KMX_LDB_DANISH
  extern const void *kmx_ldb_danish_370;
  extern const void *kmx_ldb_danish_375;
  extern const void *kmx_ldb_danish_377;
  extern const IIME_IME_INFO IIME_ldb_danish_370;
  extern const IIME_IME_INFO IIME_ldb_danish_375;
  extern const IIME_IME_INFO IIME_ldb_danish_377;
#endif

#ifdef KMX_LDB_DUTCH
  extern const void *kmx_ldb_dutch_380;
  extern const void *kmx_ldb_dutch_385;
  extern const void *kmx_ldb_dutch_387;
  extern const IIME_IME_INFO IIME_ldb_dutch_380;
  extern const IIME_IME_INFO IIME_ldb_dutch_385;
  extern const IIME_IME_INFO IIME_ldb_dutch_387;
#endif/*KMX_LDB_DUTCH*/

#ifdef KMX_LDB_FINNISH
  extern const void *kmx_ldb_finnish_450;
  extern const void *kmx_ldb_finnish_455;
  extern const void *kmx_ldb_finnish_457;
  extern const IIME_IME_INFO IIME_ldb_finnish_450;
  extern const IIME_IME_INFO IIME_ldb_finnish_455;
  extern const IIME_IME_INFO IIME_ldb_finnish_457;
#endif

#ifdef KMX_LDB_FRENCH
  extern const void *kmx_ldb_french_400;
  extern const void *kmx_ldb_french_405;
  extern const void *kmx_ldb_french_406;
  extern const void *kmx_ldb_french_407;
  extern const void *kmx_ldb_french_4071;
  extern const IIME_IME_INFO IIME_ldb_french_400;
  extern const IIME_IME_INFO IIME_ldb_french_405;
  extern const IIME_IME_INFO IIME_ldb_french_406;
  extern const IIME_IME_INFO IIME_ldb_french_407;
  extern const IIME_IME_INFO IIME_ldb_french_4071;
#endif/*KMX_LDB_FRENCH*/

#ifdef KMX_LDB_GERMAN
  extern const void *kmx_ldb_german_300;
  extern const void *kmx_ldb_german_305;
  extern const void *kmx_ldb_german_307;
  extern const void *kmx_ldb_german_308;
  extern const IIME_IME_INFO IIME_ldb_german_300;
  extern const IIME_IME_INFO IIME_ldb_german_305;
  extern const IIME_IME_INFO IIME_ldb_german_307;
  extern const IIME_IME_INFO IIME_ldb_german_308;
#endif/*KMX_LDB_GERMAN*/

#ifdef KMX_LDB_GREEK
  extern const void *kmx_ldb_greek_350;
  extern const void *kmx_ldb_greek_355;
  extern const void *kmx_ldb_greek_357;
  extern const IIME_IME_INFO IIME_ldb_greek_350;
  extern const IIME_IME_INFO IIME_ldb_greek_355;
  extern const IIME_IME_INFO IIME_ldb_greek_357;
#endif/*KMX_LDB_GREEK*/

#ifdef KMX_LDB_HEBREW
  extern const void *kmx_ldb_hebrew_420;
  extern const void *kmx_ldb_hebrew_425;
  extern const void *kmx_ldb_hebrew_427;
  extern const void *kmx_ldb_hebrew_4271;
  extern const IIME_IME_INFO IIME_ldb_hebrew_420;
  extern const IIME_IME_INFO IIME_ldb_hebrew_425;
  extern const IIME_IME_INFO IIME_ldb_hebrew_427;
  extern const IIME_IME_INFO IIME_ldb_hebrew_4271;
#endif/*KMX_LDB_HEBREW*/

#ifdef KMX_LDB_HINDI
  extern const void *kmx_ldb_hindi_900;
  extern const void *kmx_ldb_hindi_902;
  extern const void *kmx_ldb_hindi_903;
  extern const void *kmx_ldb_hindi_904;
  extern const void *kmx_ldb_hindi_905;
  extern const void *kmx_ldb_hindi_906;
  extern const void *kmx_ldb_hindi_907;
  extern const void *kmx_ldb_hindi_9071;
  extern const void *kmx_ldb_hindi_908;
  extern const void *kmx_ldb_hindi_9000;
  extern const void *kmx_ldb_hindi_9001;
  extern const void *kmx_ldb_hindi_9002;
  extern const void *kmx_ldb_hindi_9008;
  extern const void *kmx_ldb_hindi_9010;
  extern const void *kmx_ldb_hindi_9012;
  extern const void *kmx_ldb_hindi_9013;
  extern const void *kmx_ldb_hindi_9015;
  extern const void *kmx_ldb_hindi_9017;
  extern const void *kmx_ldb_hindi_9018;
  extern const IIME_IME_INFO IIME_ldb_hindi_900;
  extern const IIME_IME_INFO IIME_ldb_hindi_902;
  extern const IIME_IME_INFO IIME_ldb_hindi_903;
  extern const IIME_IME_INFO IIME_ldb_hindi_904;
  extern const IIME_IME_INFO IIME_ldb_hindi_905;
  extern const IIME_IME_INFO IIME_ldb_hindi_906;
  extern const IIME_IME_INFO IIME_ldb_hindi_907;
  extern const IIME_IME_INFO IIME_ldb_hindi_9071;
  extern const IIME_IME_INFO IIME_ldb_hindi_908;
  extern const IIME_IME_INFO IIME_ldb_hindi_9000;
  extern const IIME_IME_INFO IIME_ldb_hindi_9001;
  extern const IIME_IME_INFO IIME_ldb_hindi_9002;
  extern const IIME_IME_INFO IIME_ldb_hindi_9008;
  extern const IIME_IME_INFO IIME_ldb_hindi_9010;
  extern const IIME_IME_INFO IIME_ldb_hindi_9012;
  extern const IIME_IME_INFO IIME_ldb_hindi_9013;
  extern const IIME_IME_INFO IIME_ldb_hindi_9015;
  extern const IIME_IME_INFO IIME_ldb_hindi_9017;
  extern const IIME_IME_INFO IIME_ldb_hindi_9018;
#endif/*KMX_LDB_HINDI*/

#ifdef KMX_LDB_HINGLISH
  extern const void *kmx_ldb_hinglish_910;
  extern const void *kmx_ldb_hinglish_915;
  extern const void *kmx_ldb_hinglish_917;
  extern const void *kmx_ldb_hinglish_9171;
  extern const IIME_IME_INFO IIME_ldb_hinglish_910;
  extern const IIME_IME_INFO IIME_ldb_hinglish_915;
  extern const IIME_IME_INFO IIME_ldb_hinglish_917;
  extern const IIME_IME_INFO IIME_ldb_hinglish_9171;
#endif/*KMX_LDB_HINGLISH*/

#ifdef KMX_LDB_HUNGARIAN
  extern const void *kmx_ldb_hungarian_410;
  extern const void *kmx_ldb_hungarian_412;
  extern const void *kmx_ldb_hungarian_413;
  extern const void *kmx_ldb_hungarian_415;
  extern const void *kmx_ldb_hungarian_417;
  extern const IIME_IME_INFO IIME_ldb_hungarian_410;
  extern const IIME_IME_INFO IIME_ldb_hungarian_412;
  extern const IIME_IME_INFO IIME_ldb_hungarian_413;
  extern const IIME_IME_INFO IIME_ldb_hungarian_415;
  extern const IIME_IME_INFO IIME_ldb_hungarian_417;
#endif/*KMX_LDB_HUNGARIAN*/

#ifdef KMX_LDB_INDONESIAN
  extern const void *kmx_ldb_indonesian_320;
  extern const void *kmx_ldb_indonesian_321;
  extern const void *kmx_ldb_indonesian_325;
  extern const void *kmx_ldb_indonesian_327;
  extern const void *kmx_ldb_indonesian_3271;
  extern const IIME_IME_INFO IIME_ldb_indonesian_320;
  extern const IIME_IME_INFO IIME_ldb_indonesian_321;
  extern const IIME_IME_INFO IIME_ldb_indonesian_325;
  extern const IIME_IME_INFO IIME_ldb_indonesian_327;
  extern const IIME_IME_INFO IIME_ldb_indonesian_3271;
#endif/*KMX_LDB_INDONESIAN*/

#ifdef KMX_LDB_ITALIAN
  extern const void *kmx_ldb_italian_700;
  extern const void *kmx_ldb_italian_705;
  extern const void *kmx_ldb_italian_707;
  extern const IIME_IME_INFO IIME_ldb_italian_700;
  extern const IIME_IME_INFO IIME_ldb_italian_705;
  extern const IIME_IME_INFO IIME_ldb_italian_707;
#endif/*KMX_LDB_ITALIAN*/

#ifdef KMX_LDB_LITHUANIAN
  extern const void *kmx_ldb_lithuanian_560;
  extern const void *kmx_ldb_lithuanian_565;
  extern const void *kmx_ldb_lithuanian_567;
  extern const IIME_IME_INFO IIME_ldb_lithuanian_560;
  extern const IIME_IME_INFO IIME_ldb_lithuanian_565;
  extern const IIME_IME_INFO IIME_ldb_lithuanian_567;
#endif/*KMX_LDB_LITHUANIAN*/

#ifdef KMX_LDB_MALAY
  extern const void *kmx_ldb_malay_340;
  extern const void *kmx_ldb_malay_345;
  extern const void *kmx_ldb_malay_347;
  extern const void *kmx_ldb_malay_3471;
  extern const IIME_IME_INFO IIME_ldb_malay_340;
  extern const IIME_IME_INFO IIME_ldb_malay_345;
  extern const IIME_IME_INFO IIME_ldb_malay_347;
  extern const IIME_IME_INFO IIME_ldb_malay_3471;
#endif/*KMX_LDB_MALAY*/

#ifdef KMX_LDB_NORWEGIAN
  extern const void *kmx_ldb_norwegian_480;
  extern const void *kmx_ldb_norwegian_485;
  extern const void *kmx_ldb_norwegian_487;
  extern const IIME_IME_INFO IIME_ldb_norwegian_480;
  extern const IIME_IME_INFO IIME_ldb_norwegian_485;
  extern const IIME_IME_INFO IIME_ldb_norwegian_487;
#endif/*KMX_LDB_NORWEGIAN*/

#ifdef KMX_LDB_PERSIAN
  extern const void *kmx_ldb_persian_460;
  extern const void *kmx_ldb_persian_462;
  extern const void *kmx_ldb_persian_463;
  extern const void *kmx_ldb_persian_465;
  extern const void *kmx_ldb_persian_467;
  extern const void *kmx_ldb_persian_4671;
  extern const void *kmx_ldb_persian_468;
  extern const void *kmx_ldb_persian_469;
  extern const void *kmx_ldb_persian_4600;
  extern const IIME_IME_INFO IIME_ldb_persian_460;
  extern const IIME_IME_INFO IIME_ldb_persian_462;
  extern const IIME_IME_INFO IIME_ldb_persian_463;
  extern const IIME_IME_INFO IIME_ldb_persian_465;
  extern const IIME_IME_INFO IIME_ldb_persian_467;
  extern const IIME_IME_INFO IIME_ldb_persian_4671;
  extern const IIME_IME_INFO IIME_ldb_persian_468;
  extern const IIME_IME_INFO IIME_ldb_persian_469;
  extern const IIME_IME_INFO IIME_ldb_persian_4600;
#endif/*KMX_LDB_PERSIAN*/

#ifdef KMX_LDB_POLISH
  extern const void *kmx_ldb_polish_391;
  extern const void *kmx_ldb_polish_392;
  extern const void *kmx_ldb_polish_393;
  extern const void *kmx_ldb_polish_395;
  extern const void *kmx_ldb_polish_397;
  extern const IIME_IME_INFO IIME_ldb_polish_391;
  extern const IIME_IME_INFO IIME_ldb_polish_392;
  extern const IIME_IME_INFO IIME_ldb_polish_393;
  extern const IIME_IME_INFO IIME_ldb_polish_395;
  extern const IIME_IME_INFO IIME_ldb_polish_397;
#endif/*KMX_LDB_POLISH*/

#ifdef KMX_LDB_PORTUGUESE
  extern const void *kmx_ldb_portuguese_600;
  extern const void *kmx_ldb_portuguese_601;
  extern const void *kmx_ldb_portuguese_605;
  extern const void *kmx_ldb_portuguese_607;
  extern const void *kmx_ldb_portuguese_6071;
  extern const IIME_IME_INFO IIME_ldb_portuguese_600;
  extern const IIME_IME_INFO IIME_ldb_portuguese_601;
  extern const IIME_IME_INFO IIME_ldb_portuguese_605;
  extern const IIME_IME_INFO IIME_ldb_portuguese_607;
  extern const IIME_IME_INFO IIME_ldb_portuguese_6071;
#endif/*KMX_LDB_PORTUGUESE*/

#ifdef KMX_LDB_ROMANIAN
  extern const void *kmx_ldb_romanian_520;
  extern const void *kmx_ldb_romanian_522;
  extern const void *kmx_ldb_romanian_523;
  extern const void *kmx_ldb_romanian_525;
  extern const void *kmx_ldb_romanian_527;
  extern const IIME_IME_INFO IIME_ldb_romanian_520;
  extern const IIME_IME_INFO IIME_ldb_romanian_522;
  extern const IIME_IME_INFO IIME_ldb_romanian_523;
  extern const IIME_IME_INFO IIME_ldb_romanian_525;
  extern const IIME_IME_INFO IIME_ldb_romanian_527;
#endif/*KMX_LDB_ROMANIAN*/

#ifdef KMX_LDB_RUSSIAN
  extern const void *kmx_ldb_russian_202;
  extern const void *kmx_ldb_russian_205;
  extern const void *kmx_ldb_russian_207;
  extern const void *kmx_ldb_russian_2071;
  extern const void *kmx_ldb_russian_208;
  extern const IIME_IME_INFO IIME_ldb_russian_202;
  extern const IIME_IME_INFO IIME_ldb_russian_205;
  extern const IIME_IME_INFO IIME_ldb_russian_207;
  extern const IIME_IME_INFO IIME_ldb_russian_2071;
  extern const IIME_IME_INFO IIME_ldb_russian_208;
#endif/*KMX_LDB_RUSSIAN*/

#ifdef KMX_LDB_SLOVAK
  extern const void *kmx_ldb_slovak_470;
  extern const void *kmx_ldb_slovak_475;
  extern const void *kmx_ldb_slovak_477;
  extern const IIME_IME_INFO IIME_ldb_slovak_470;
  extern const IIME_IME_INFO IIME_ldb_slovak_475;
  extern const IIME_IME_INFO IIME_ldb_slovak_477;
#endif/*KMX_LDB_SLOVAK*/

#ifdef KMX_LDB_SLOVENIAN
  extern const void *kmx_ldb_slovenian_530;
  extern const void *kmx_ldb_slovenian_535;
  extern const void *kmx_ldb_slovenian_537;
  extern const IIME_IME_INFO IIME_ldb_slovenian_530;
  extern const IIME_IME_INFO IIME_ldb_slovenian_535;
  extern const IIME_IME_INFO IIME_ldb_slovenian_537;
#endif/*KMX_LDB_SLOVENIAN*/

#ifdef KMX_LDB_SPANISH
  extern const void *kmx_ldb_spanish_500;
  extern const void *kmx_ldb_spanish_501;
  extern const void *kmx_ldb_spanish_504;
  extern const void *kmx_ldb_spanish_505;
  extern const void *kmx_ldb_spanish_507;
  extern const void *kmx_ldb_spanish_5071;
  extern const IIME_IME_INFO IIME_ldb_spanish_500;
  extern const IIME_IME_INFO IIME_ldb_spanish_501;
  extern const IIME_IME_INFO IIME_ldb_spanish_504;
  extern const IIME_IME_INFO IIME_ldb_spanish_505;
  extern const IIME_IME_INFO IIME_ldb_spanish_507;
  extern const IIME_IME_INFO IIME_ldb_spanish_5071;
#endif/*KMX_LDB_SPANISH*/

#ifdef KMX_LDB_SWAHILI
  extern const void *kmx_ldb_swahili_550;
  extern const void *kmx_ldb_swahili_555;
  extern const void *kmx_ldb_swahili_557;
  extern const IIME_IME_INFO IIME_ldb_swahili_550;
  extern const IIME_IME_INFO IIME_ldb_swahili_555;
  extern const IIME_IME_INFO IIME_ldb_swahili_557;
#endif/*KMX_LDB_SWAHILI*/

#ifdef KMX_LDB_SWEDISH
  extern const void *kmx_ldb_swedish_490;
  extern const void *kmx_ldb_swedish_495;
  extern const void *kmx_ldb_swedish_497;
  extern const IIME_IME_INFO IIME_ldb_swedish_490;
  extern const IIME_IME_INFO IIME_ldb_swedish_495;
  extern const IIME_IME_INFO IIME_ldb_swedish_497;
#endif/*KMX_LDB_SWEDISH*/

#ifdef KMX_LDB_THAI
  extern const void *kmx_ldb_thai_360;
  extern const void *kmx_ldb_thai_361;
  extern const void *kmx_ldb_thai_362;
  extern const void *kmx_ldb_thai_363;
  extern const void *kmx_ldb_thai_364;
  extern const void *kmx_ldb_thai_365;
  extern const void *kmx_ldb_thai_366;		
  extern const void *kmx_ldb_thai_367;
  extern const void *kmx_ldb_thai_3671;
  extern const void *kmx_ldb_thai_368;
  extern const void *kmx_ldb_thai_369;
  extern const void *kmx_ldb_thai_3600;	
  extern const void *kmx_ldb_thai_3601;	
  extern const void *kmx_ldb_thai_3602;	
  extern const void *kmx_ldb_thai_3603;	
  extern const void *kmx_ldb_thai_3604;		
  extern const void *kmx_ldb_thai_3606;	
  extern const void *kmx_ldb_thai_3607;	
  extern const void *kmx_ldb_thai_3608;	
  extern const void *kmx_ldb_thai_3611;	
  extern const void *kmx_ldb_thai_3616;	
  extern const void *kmx_ldb_thai_3640;	
  extern const void *kmx_ldb_thai_3632;	
  extern const IIME_IME_INFO IIME_ldb_thai_360;
  extern const IIME_IME_INFO IIME_ldb_thai_361;
  extern const IIME_IME_INFO IIME_ldb_thai_362;
  extern const IIME_IME_INFO IIME_ldb_thai_363;
  extern const IIME_IME_INFO IIME_ldb_thai_364;
  extern const IIME_IME_INFO IIME_ldb_thai_365;
  extern const IIME_IME_INFO IIME_ldb_thai_366;	
  extern const IIME_IME_INFO IIME_ldb_thai_367;
  extern const IIME_IME_INFO IIME_ldb_thai_3671;
  extern const IIME_IME_INFO IIME_ldb_thai_368;
  extern const IIME_IME_INFO IIME_ldb_thai_369;
  extern const IIME_IME_INFO IIME_ldb_thai_3600;	
  extern const IIME_IME_INFO IIME_ldb_thai_3601;	
  extern const IIME_IME_INFO IIME_ldb_thai_3602;	
  extern const IIME_IME_INFO IIME_ldb_thai_3603;	
  extern const IIME_IME_INFO IIME_ldb_thai_3604;		
  extern const IIME_IME_INFO IIME_ldb_thai_3606;	
  extern const IIME_IME_INFO IIME_ldb_thai_3607;	
  extern const IIME_IME_INFO IIME_ldb_thai_3608;	
  extern const IIME_IME_INFO IIME_ldb_thai_3611;	
  extern const IIME_IME_INFO IIME_ldb_thai_3616;	
  extern const IIME_IME_INFO IIME_ldb_thai_3640;	
  extern const IIME_IME_INFO IIME_ldb_thai_3632;	
#endif/*KMX_LDB_THAI*/

#ifdef KMX_LDB_TIBETAN
  extern const void *kmx_ldb_tibetan_730;	
  extern const IIME_IME_INFO IIME_ldb_tibetan_730;
#endif/*KMX_LDB_TIBETAN*/

#ifdef KMX_LDB_TURKISH
  extern const void *kmx_ldb_turkish_330;
  extern const void *kmx_ldb_turkish_335;
  extern const void *kmx_ldb_turkish_337;
  extern const void *kmx_ldb_turkish_3371;
  extern const IIME_IME_INFO IIME_ldb_turkish_330;
  extern const IIME_IME_INFO IIME_ldb_turkish_335;
  extern const IIME_IME_INFO IIME_ldb_turkish_337;
  extern const IIME_IME_INFO IIME_ldb_turkish_3371;
#endif/*KMX_LDB_TURKISH*/

#ifdef KMX_LDB_UKRAINIAN
  extern const void *kmx_ldb_ukrainian_540;
  extern const void *kmx_ldb_ukrainian_545;
  extern const void *kmx_ldb_ukrainian_547;
  extern const void *kmx_ldb_ukrainian_548;
  extern const IIME_IME_INFO IIME_ldb_ukrainian_540;
  extern const IIME_IME_INFO IIME_ldb_ukrainian_545;
  extern const IIME_IME_INFO IIME_ldb_ukrainian_547;
  extern const IIME_IME_INFO IIME_ldb_ukrainian_548;
#endif/*KMX_LDB_UKRAINIAN*/

#ifdef KMX_LDB_UYGHUR
  extern const void *kmx_ldb_uyghur_940;
  extern const void *kmx_ldb_uyghur_941;
  extern const void *kmx_ldb_uyghur_947;
  extern const void *kmx_ldb_uyghur_948;
  extern const IIME_IME_INFO IIME_ldb_uyghur_940;
  extern const IIME_IME_INFO IIME_ldb_uyghur_941;
  extern const IIME_IME_INFO IIME_ldb_uyghur_947;
  extern const IIME_IME_INFO IIME_ldb_uyghur_948;
#endif/*KMX_LDB_UYGHUR*/

#ifdef KMX_LDB_VIETNAMESE
  extern const void *kmx_ldb_vietnamese_310;
  extern const void *kmx_ldb_vietnamese_315;
  extern const void *kmx_ldb_vietnamese_317;
  extern const void *kmx_ldb_vietnamese_3171;
  extern const IIME_IME_INFO IIME_ldb_vietnamese_310;
  extern const IIME_IME_INFO IIME_ldb_vietnamese_315;
  extern const IIME_IME_INFO IIME_ldb_vietnamese_317;
  extern const IIME_IME_INFO IIME_ldb_vietnamese_3171;
#endif/*KMX_LDB_VIETNAMESE*/

#ifdef KMX_LDB_URDU
  extern const void *kmx_ldb_urdu_950;
  extern const void *kmx_ldb_urdu_955;
  extern const void *kmx_ldb_urdu_957;
  extern const void *kmx_ldb_urdu_9571;
  extern const IIME_IME_INFO IIME_ldb_urdu_950;
  extern const IIME_IME_INFO IIME_ldb_urdu_955;
  extern const IIME_IME_INFO IIME_ldb_urdu_957;
  extern const IIME_IME_INFO IIME_ldb_urdu_9571;
#endif/*KMX_LDB_URDU*/

#ifdef KMX_LDB_TAGALOG
  extern const void *kmx_ldb_tagalog_587;
  extern const IIME_IME_INFO IIME_ldb_tagalog_587;
#endif/*KMX_LDB_TAGALOG*/

#ifdef KMX_LDB_KYRGYZ
  extern const void *kmx_ldb_kyrgyz_747;
  extern const IIME_IME_INFO IIME_ldb_kyrgyz_747;
#endif/*KMX_LDB_KYRGYZ*/

#ifdef KMX_LDB_NEPALI
  extern const void *kmx_ldb_nepali_720;
  extern const IIME_IME_INFO IIME_ldb_nepali_720;
#endif/*KMX_LDB_NEPALI*/

#ifdef KMX_LDB_SINHALA
  extern const void *kmx_ldb_sinhala_770;
  extern const IIME_IME_INFO IIME_ldb_sinhala_770;
#endif/*KMX_LDB_NEPALI*/

#ifdef KMX_LDB_BODO//daodejing
  extern const void *kmx_ldb_bodo_1100;
  extern const IIME_IME_INFO IIME_ldb_bodo_1100;
#endif/*KMX_LDB_BODO*/

#ifdef KMX_LDB_DOGRI
  extern const void *kmx_ldb_dogri_1110;
  extern const IIME_IME_INFO IIME_ldb_dogri_1110;
#endif/*KMX_LDB_DOGRI*/

#ifdef KMX_LDB_SANSKRIT
  extern const void *kmx_ldb_sanskrit_1120;
  extern const IIME_IME_INFO IIME_ldb_sanskrit_1120;
#endif/*KMX_LDB_SANSKRIT*/

#ifdef KMX_LDB_KASHMIRI
  extern const void *kmx_ldb_kashmiri_1130;
  extern const IIME_IME_INFO IIME_ldb_kashmiri_1130;
#endif/*KMX_LDB_KASHMIRI*/

#ifdef KMX_LDB_KONKANI
  extern const void *kmx_ldb_konkani_1140;
  extern const IIME_IME_INFO IIME_ldb_konkani_1140;
#endif/*KMX_LDB_KONKANI*/

#ifdef KMX_LDB_MAITHILI
  extern const void *kmx_ldb_maithili_1150;
  extern const IIME_IME_INFO IIME_ldb_maithili_1150;
#endif/*KMX_LDB_MAITHILI*/

#ifdef KMX_LDB_MANIPURI
  extern const void *kmx_ldb_manipuri_1160;
  extern const IIME_IME_INFO IIME_ldb_manipuri_1160;
#endif/*KMX_LDB_MANIPURI*/

#ifdef KMX_LDB_SANTALI
  extern const void *kmx_ldb_santali_1170;
  extern const IIME_IME_INFO IIME_ldb_santali_1170;
#endif/*KMX_LDB_SANTALI*/

#ifdef KMX_LDB_SOMALI
  extern const void *kmx_ldb_somali_1180;
  extern const IIME_IME_INFO IIME_ldb_somali_1180;
#endif/*KMX_LDB_SOMALI*/

#ifdef KMX_LDB_SINDHI
  extern const void *kmx_ldb_sindhi_1190;
  extern const IIME_IME_INFO IIME_ldb_sindhi_1190;
#endif/*KMX_LDB_SINDHI*/

#ifdef KMX_LDB_ASSAMESE
  extern const void *kmx_ldb_assamese_710;
  extern const IIME_IME_INFO IIME_ldb_assamese_710;	
#endif/*KMX_LDB_ASSAMESE*/

#ifdef KMX_LDB_ALBANIAN
  extern const void *kmx_ldb_albanian_611;
  extern const IIME_IME_INFO IIME_ldb_albanian_611;
#endif/*KMX_LDB_ALBANIAN*/

#ifdef KMX_LDB_CATALAN
  extern const void *kmx_ldb_catalan_621;
  extern const IIME_IME_INFO IIME_ldb_catalan_621;
#endif/*KMX_LDB_CATALAN*/

#ifdef KMX_LDB_BASQUE
  extern const void *kmx_ldb_basque_631;
  extern const IIME_IME_INFO IIME_ldb_basque_631;
#endif/*KMX_LDB_BASQUE*/

#ifdef KMX_LDB_GALICIAN
  extern const void *kmx_ldb_galician_641;
  extern const IIME_IME_INFO IIME_ldb_galician_641;
#endif/*KMX_LDB_GALICIAN*/

#ifdef KMX_LDB_IGBO
  extern const void *kmx_ldb_igbo_651;
  extern const IIME_IME_INFO IIME_ldb_igbo_651;
#endif/*KMX_LDB_IGBO*/

#ifdef KMX_LDB_OROMO
  extern const void *kmx_ldb_oromo_661;
  extern const IIME_IME_INFO IIME_ldb_oromo_661;
#endif/*KMX_LDB_OROMO*/

#ifdef KMX_LDB_SERBIAN
  extern const void *kmx_ldb_serbian_233;
  extern const IIME_IME_INFO IIME_ldb_serbian_233;
#endif/*KMX_LDB_SERBIAN*/

#ifdef KMX_LDB_SERBIANC
  extern const void *kmx_ldb_serbianc_243;
  extern const IIME_IME_INFO IIME_ldb_serbianc_243;
#endif/*KMX_LDB_SERBIANC*/

#ifdef KMX_LDB_ZULU
  extern const void *kmx_ldb_zulu_677;
  extern const IIME_IME_INFO IIME_ldb_zulu_677;
#endif/*KMX_LDB_ZULU*/

#ifdef KMX_LDB_CEBUANO
  extern const void *kmx_ldb_cebuano_687;
  extern const IIME_IME_INFO IIME_ldb_cebuano_687;
#endif/*KMX_LDB_CEBUANO*/

#ifdef KMX_LDB_KAZAKH
  extern const void *kmx_ldb_kazakh_257;
  extern const IIME_IME_INFO IIME_ldb_kazakh_257;
#endif/*KMX_LDB_KAZAKH*/

#ifdef KMX_LDB_UZBEK
  extern const void *kmx_ldb_uzbek_267;
  extern const IIME_IME_INFO IIME_ldb_uzbek_267;
#endif/*KMX_LDB_UZBEK*/

#ifdef KMX_LDB_GEORGIAN
  extern const void *kmx_ldb_georgian_277;
  extern const IIME_IME_INFO IIME_ldb_georgian_277;
#endif/*KMX_LDB_GEORGIAN*/


//以下是每种打开的kmx宏，只配置一个ldb文件!
#elif defined(IEKIE_INPUT_SAME_LANGUAGE_ONE_LDB)


#if defined(KMX_LDB_ENGLISH)
  #if (KMX_LDB_ENGLISH==100)
	extern const void *kmx_ldb_english_100;
	extern const IIME_IME_INFO IIME_ldb_english_100;
  #elif (KMX_LDB_ENGLISH==101)
	extern const void *kmx_ldb_english_101;
	extern const IIME_IME_INFO IIME_ldb_english_101;
  #elif (KMX_LDB_ENGLISH==102)
	extern const void *kmx_ldb_english_102;
	extern const IIME_IME_INFO IIME_ldb_english_102;
  #elif (KMX_LDB_ENGLISH==103)
	extern const void *kmx_ldb_english_103;
	extern const IIME_IME_INFO IIME_ldb_english_103;
  #elif (KMX_LDB_ENGLISH==104)
	extern const void *kmx_ldb_english_104;
	extern const IIME_IME_INFO IIME_ldb_english_104;
  #elif (KMX_LDB_ENGLISH==105)
	extern const void *kmx_ldb_english_105;
	extern const IIME_IME_INFO IIME_ldb_english_105;
  #elif (KMX_LDB_ENGLISH==106)
	extern const void *kmx_ldb_english_106;
	extern const IIME_IME_INFO IIME_ldb_english_106;
  #elif (KMX_LDB_ENGLISH==107)
	extern const void *kmx_ldb_english_107;
	extern const IIME_IME_INFO IIME_ldb_english_107;
  #elif (KMX_LDB_ENGLISH==1071)
	extern const void *kmx_ldb_english_1071;
	extern const IIME_IME_INFO IIME_ldb_english_1071;
  #elif (KMX_LDB_ENGLISH==108)
	extern const void *kmx_ldb_english_108;
	extern const IIME_IME_INFO IIME_ldb_english_108;
  #elif (KMX_LDB_ENGLISH==109)
	extern const void *kmx_ldb_english_109;
	extern const IIME_IME_INFO IIME_ldb_english_109;
  #elif (KMX_LDB_ENGLISH==10001)
	extern const void *kmx_ldb_english_10001;
	extern const IIME_IME_INFO IIME_ldb_english_10001;
  #endif
#else
   	extern const IIME_IME_INFO IIME_ldb_english_107;//zhmch
#endif/*KMX_LDB_ENGLISH*/

#if defined(KMX_LDB_ARABIC)
  #if (KMX_LDB_ARABIC==800)
	extern const void *kmx_ldb_arabic_800;
	extern const IIME_IME_INFO IIME_ldb_arabic_800;
  #elif (KMX_LDB_ARABIC==801)
  	extern const void *kmx_ldb_arabic_801;
	extern const IIME_IME_INFO IIME_ldb_arabic_801;
  #elif (KMX_LDB_ARABIC==802)
  	extern const void *kmx_ldb_arabic_802;
	extern const IIME_IME_INFO IIME_ldb_arabic_802;
  #elif (KMX_LDB_ARABIC==803)
  	extern const void *kmx_ldb_arabic_803;
	extern const IIME_IME_INFO IIME_ldb_arabic_803;
  #elif (KMX_LDB_ARABIC==804)
  	extern const void *kmx_ldb_arabic_804;
	extern const IIME_IME_INFO IIME_ldb_arabic_804;
  #elif (KMX_LDB_ARABIC==805)
  	extern const void *kmx_ldb_arabic_805;
	extern const IIME_IME_INFO IIME_ldb_arabic_805;
  #elif (KMX_LDB_ARABIC==806)
  	extern const void *kmx_ldb_arabic_806;
	extern const IIME_IME_INFO IIME_ldb_arabic_806;
  #elif (KMX_LDB_ARABIC==807)
  	extern const void *kmx_ldb_arabic_807;
	extern const IIME_IME_INFO IIME_ldb_arabic_807;
  #elif (KMX_LDB_ARABIC==8071)
  	extern const void *kmx_ldb_arabic_8071;
	extern const IIME_IME_INFO IIME_ldb_arabic_8071;
  #elif (KMX_LDB_ARABIC==808)
  	extern const void *kmx_ldb_arabic_808;
	extern const IIME_IME_INFO IIME_ldb_arabic_808;
  #elif (KMX_LDB_ARABIC==809)
  	extern const void *kmx_ldb_arabic_809;
	extern const IIME_IME_INFO IIME_ldb_arabic_809;
  #elif (KMX_LDB_ARABIC==8000)
  	extern const void *kmx_ldb_arabic_8000;
	extern const IIME_IME_INFO IIME_ldb_arabic_8000;
  #elif (KMX_LDB_ARABIC==8001)
  	extern const void *kmx_ldb_arabic_8001;
	extern const IIME_IME_INFO IIME_ldb_arabic_8001;
  #elif (KMX_LDB_ARABIC==8011)
  	extern const void *kmx_ldb_arabic_8011;
	extern const IIME_IME_INFO IIME_ldb_arabic_8011;
  #elif (KMX_LDB_ARABIC==8016)
  	extern const void *kmx_ldb_arabic_8016;
	extern const IIME_IME_INFO IIME_ldb_arabic_8016;
  #endif
#endif/*KMX_LDB_ARABIC*/

#if defined(KMX_LDB_ARMENIAN)
  #if (KMX_LDB_ARMENIAN==570)
  	extern const void *kmx_ldb_armenian_570;
  	extern const IIME_IME_INFO IIME_ldb_armenian_570;
  #elif (KMX_LDB_ARMENIAN==575)
  	extern const void *kmx_ldb_armenian_575;
  	extern const IIME_IME_INFO IIME_ldb_armenian_575;
  #elif (KMX_LDB_ARMENIAN==577)
  	extern const void *kmx_ldb_armenian_577;
  	extern const IIME_IME_INFO IIME_ldb_armenian_577;
  #endif
#endif/*KMX_LDB_ARMENIAN*/

    /*
     * latin ldb
     */
#ifdef KMX_LDB_BENGALI
  #if (KMX_LDB_BENGALI==920)
  	extern const void *kmx_ldb_bengali_920;
  	extern const IIME_IME_INFO IIME_ldb_bengali_920;
  #elif (KMX_LDB_BENGALI==921)
  	extern const void *kmx_ldb_bengali_921;
  	extern const IIME_IME_INFO IIME_ldb_bengali_921;
  #elif (KMX_LDB_BENGALI==922)
  	extern const void *kmx_ldb_bengali_922;
  	extern const IIME_IME_INFO IIME_ldb_bengali_922;
  #elif (KMX_LDB_BENGALI==923)
  	extern const void *kmx_ldb_bengali_923;
  	extern const IIME_IME_INFO IIME_ldb_bengali_923;
  #elif (KMX_LDB_BENGALI==924)
  	extern const void *kmx_ldb_bengali_924;
  	extern const IIME_IME_INFO IIME_ldb_bengali_924;
  #elif (KMX_LDB_BENGALI==925)
  	extern const void *kmx_ldb_bengali_925;
  	extern const IIME_IME_INFO IIME_ldb_bengali_925;
  #elif (KMX_LDB_BENGALI==927)
  	extern const void *kmx_ldb_bengali_927;
  	extern const IIME_IME_INFO IIME_ldb_bengali_927;
  #elif (KMX_LDB_BENGALI==928)
  	extern const void *kmx_ldb_bengali_928;
  	extern const IIME_IME_INFO IIME_ldb_bengali_928;
  #elif (KMX_LDB_BENGALI==929)
  	extern const void *kmx_ldb_bengali_929;
  	extern const IIME_IME_INFO IIME_ldb_bengali_929;
  #elif (KMX_LDB_BENGALI==9200)
  	extern const void *kmx_ldb_bengali_9200;
  	extern const IIME_IME_INFO IIME_ldb_bengali_9200;
  #elif (KMX_LDB_BENGALI==9201)
  	extern const void *kmx_ldb_bengali_9201;
  	extern const IIME_IME_INFO IIME_ldb_bengali_9201;
  #elif (KMX_LDB_BENGALI==9202)
  	extern const void *kmx_ldb_bengali_9202;
  	extern const IIME_IME_INFO IIME_ldb_bengali_9202;
  #elif (KMX_LDB_BENGALI==9203)
  	extern const void *kmx_ldb_bengali_9203;
  	extern const IIME_IME_INFO IIME_ldb_bengali_9203;
  #endif
#endif/*KMX_LDB_BENGALI*/

#ifdef KMX_LDB_BULGARIAN
  #if (KMX_LDB_BULGARIAN==430)
  	extern const void *kmx_ldb_bulgarian_430;
  	extern const IIME_IME_INFO IIME_ldb_bulgarian_430;
  #elif (KMX_LDB_BULGARIAN==435)
  	extern const void *kmx_ldb_bulgarian_435;
  	extern const IIME_IME_INFO IIME_ldb_bulgarian_435;
  #elif (KMX_LDB_BULGARIAN==437)
  	extern const void *kmx_ldb_bulgarian_437;
  	extern const IIME_IME_INFO IIME_ldb_bulgarian_437;
  #endif
#endif/*KMX_LDB_BULGARIAN*/

#ifdef KMX_LDB_CROATIAN
  #if (KMX_LDB_CROATIAN==510)
  	extern const void *kmx_ldb_croatian_510;
  	extern const IIME_IME_INFO IIME_ldb_croatian_510;
  #elif (KMX_LDB_CROATIAN==515)
  	extern const void *kmx_ldb_croatian_515;
  	extern const IIME_IME_INFO IIME_ldb_croatian_515;
  #elif (KMX_LDB_CROATIAN==517)
  	extern const void *kmx_ldb_croatian_517;
  	extern const IIME_IME_INFO IIME_ldb_croatian_517;
  #endif
#endif/*KMX_LDB_CROATIAN*/

#ifdef KMX_LDB_CZECH
  #if (KMX_LDB_CZECH==440)
  	extern const void *kmx_ldb_czech_440;
  	extern const IIME_IME_INFO IIME_ldb_czech_440;
  #elif (KMX_LDB_CZECH==442)
  	extern const void *kmx_ldb_czech_442;
  	extern const IIME_IME_INFO IIME_ldb_czech_442;
  #elif (KMX_LDB_CZECH==443)
  	extern const void *kmx_ldb_czech_443;
  	extern const IIME_IME_INFO IIME_ldb_czech_443;
  #elif (KMX_LDB_CZECH==445)
  	extern const void *kmx_ldb_czech_445;
  	extern const IIME_IME_INFO IIME_ldb_czech_445;
  #elif (KMX_LDB_CZECH==447)
  	extern const void *kmx_ldb_czech_447;
  	extern const IIME_IME_INFO IIME_ldb_czech_447;
  #endif  
#endif/*KMX_LDB_CZECH*/

#ifdef KMX_LDB_DANISH
  #if (KMX_LDB_DANISH==370)
  	extern const void *kmx_ldb_danish_370;
  	extern const IIME_IME_INFO IIME_ldb_danish_370;
  #elif (KMX_LDB_DANISH==375)
  	extern const void *kmx_ldb_danish_375;
  	extern const IIME_IME_INFO IIME_ldb_danish_375;
  #elif (KMX_LDB_DANISH==377)
  	extern const void *kmx_ldb_danish_377;
  	extern const IIME_IME_INFO IIME_ldb_danish_377;
  #endif  
#endif/*KMX_LDB_DANISH*/

#ifdef KMX_LDB_DUTCH
  #if (KMX_LDB_DUTCH==380)
  	extern const void *kmx_ldb_dutch_380;
  	extern const IIME_IME_INFO IIME_ldb_dutch_380;
  #elif (KMX_LDB_DUTCH==385)
  	extern const void *kmx_ldb_dutch_385;
  	extern const IIME_IME_INFO IIME_ldb_dutch_385;
  #elif (KMX_LDB_DUTCH==387)
  	extern const void *kmx_ldb_dutch_387;
  	extern const IIME_IME_INFO IIME_ldb_dutch_387;
  #endif
#endif/*KMX_LDB_DUTCH*/

#ifdef KMX_LDB_FINNISH
  #if (KMX_LDB_DUTCH==450)
  	extern const void *kmx_ldb_finnish_450;
  	extern const IIME_IME_INFO IIME_ldb_finnish_450;
  #elif (KMX_LDB_DUTCH==455)
  	extern const void *kmx_ldb_finnish_455;
  	extern const IIME_IME_INFO IIME_ldb_finnish_455;
  #elif (KMX_LDB_DUTCH==457)
  	extern const void *kmx_ldb_finnish_457;
  	extern const IIME_IME_INFO IIME_ldb_finnish_457;
  #endif
#endif/*KMX_LDB_FINNISH*/

#ifdef KMX_LDB_FRENCH
  #if (KMX_LDB_FRENCH==440)
  	extern const void *kmx_ldb_french_400;
  	extern const IIME_IME_INFO IIME_ldb_french_400;
  #elif (KMX_LDB_FRENCH==405)
  	extern const void *kmx_ldb_french_405;
  	extern const IIME_IME_INFO IIME_ldb_french_405;
  #elif (KMX_LDB_FRENCH==406)
  	extern const void *kmx_ldb_french_406;
  	extern const IIME_IME_INFO IIME_ldb_french_406;
  #elif (KMX_LDB_FRENCH==407)
  	extern const void *kmx_ldb_french_407;
  	extern const IIME_IME_INFO IIME_ldb_french_407;
  #elif (KMX_LDB_FRENCH==4071)
  	extern const void *kmx_ldb_french_4071;
  	extern const IIME_IME_INFO IIME_ldb_french_4071;
  #endif  
#endif/*KMX_LDB_FRENCH*/

#ifdef KMX_LDB_GERMAN
  #if (KMX_LDB_GERMAN==300)
  	extern const void *kmx_ldb_german_300;
  	extern const IIME_IME_INFO IIME_ldb_german_300;
  #elif (KMX_LDB_GERMAN==305)
  	extern const void *kmx_ldb_german_305;
  	extern const IIME_IME_INFO IIME_ldb_german_305;
  #elif (KMX_LDB_GERMAN==307)
  	extern const void *kmx_ldb_german_307;
  	extern const IIME_IME_INFO IIME_ldb_german_307;
  #elif (KMX_LDB_GERMAN==308)
  	extern const void *kmx_ldb_german_308;
  	extern const IIME_IME_INFO IIME_ldb_german_308;
  #endif
#endif/*KMX_LDB_GERMAN*/

#ifdef KMX_LDB_GREEK
  #if (KMX_LDB_GREEK==350)
  	extern const void *kmx_ldb_greek_350;
  	extern const IIME_IME_INFO IIME_ldb_greek_350;
  #elif (KMX_LDB_GREEK==355)
  	extern const void *kmx_ldb_greek_355;
  	extern const IIME_IME_INFO IIME_ldb_greek_355;
  #elif (KMX_LDB_GREEK==357)
  	extern const void *kmx_ldb_greek_357;
  	extern const IIME_IME_INFO IIME_ldb_greek_357;
  #endif
#endif/*KMX_LDB_GREEK*/

#ifdef KMX_LDB_HEBREW
  #if (KMX_LDB_HEBREW==420)
  	extern const void *kmx_ldb_hebrew_420;
  	extern const IIME_IME_INFO IIME_ldb_hebrew_420;
  #elif (KMX_LDB_HEBREW==425)
  	extern const void *kmx_ldb_hebrew_425;
  	extern const IIME_IME_INFO IIME_ldb_hebrew_425;
  #elif (KMX_LDB_HEBREW==427)
  	extern const void *kmx_ldb_hebrew_427;
  	extern const IIME_IME_INFO IIME_ldb_hebrew_427;
  #elif (KMX_LDB_HEBREW==4271)
  	extern const void *kmx_ldb_hebrew_4271;
  	extern const IIME_IME_INFO IIME_ldb_hebrew_4271;
  #endif
#endif/*KMX_LDB_HEBREW*/

#ifdef KMX_LDB_HINDI
  #if (KMX_LDB_HINDI==900)
  	extern const void *kmx_ldb_hindi_900;
  	extern const IIME_IME_INFO IIME_ldb_hindi_900;
  #elif (KMX_LDB_HINDI==902)
	extern const void *kmx_ldb_hindi_902;
    extern const IIME_IME_INFO IIME_ldb_hindi_902;
  #elif (KMX_LDB_HINDI==903)
	extern const void *kmx_ldb_hindi_903;
    extern const IIME_IME_INFO IIME_ldb_hindi_903;
  #elif (KMX_LDB_HINDI==904)
	extern const void *kmx_ldb_hindi_904;
    extern const IIME_IME_INFO IIME_ldb_hindi_904;
  #elif (KMX_LDB_HINDI==905)
	extern const void *kmx_ldb_hindi_905;
    extern const IIME_IME_INFO IIME_ldb_hindi_905;\
  #elif (KMX_LDB_HINDI==906)
	extern const void *kmx_ldb_hindi_906;
    extern const IIME_IME_INFO IIME_ldb_hindi_906;
  #elif (KMX_LDB_HINDI==907)
	extern const void *kmx_ldb_hindi_907;
    extern const IIME_IME_INFO IIME_ldb_hindi_907;
  #elif (KMX_LDB_HINDI==9071)
	extern const void *kmx_ldb_hindi_9071;
    extern const IIME_IME_INFO IIME_ldb_hindi_9071;
  #elif (KMX_LDB_HINDI==908)
	extern const void *kmx_ldb_hindi_908;
    extern const IIME_IME_INFO IIME_ldb_hindi_908;
  #elif (KMX_LDB_HINDI==9000)
	extern const void *kmx_ldb_hindi_9000;
    extern const IIME_IME_INFO IIME_ldb_hindi_9000;
  #elif (KMX_LDB_HINDI==9001)
	extern const void *kmx_ldb_hindi_9001;
    extern const IIME_IME_INFO IIME_ldb_hindi_9001;
  #elif (KMX_LDB_HINDI==9002)
	extern const void *kmx_ldb_hindi_9002;
    extern const IIME_IME_INFO IIME_ldb_hindi_9002;
  #elif (KMX_LDB_HINDI==9008)
	extern const void *kmx_ldb_hindi_9008;
    extern const IIME_IME_INFO IIME_ldb_hindi_9008;	
  #elif (KMX_LDB_HINDI==9010)
	extern const void *kmx_ldb_hindi_9010;
	extern const IIME_IME_INFO IIME_ldb_hindi_9010;
  #elif (KMX_LDB_HINDI==9012)
	extern const void *kmx_ldb_hindi_9012;
	extern const IIME_IME_INFO IIME_ldb_hindi_9012;
  #elif (KMX_LDB_HINDI==9013)
	extern const void *kmx_ldb_hindi_9013;
	extern const IIME_IME_INFO IIME_ldb_hindi_9013;
  #elif (KMX_LDB_HINDI==9015)
	extern const void *kmx_ldb_hindi_9015;
	extern const IIME_IME_INFO IIME_ldb_hindi_9015; 
  #elif (KMX_LDB_HINDI==9017)
	extern const void *kmx_ldb_hindi_9017;
	extern const IIME_IME_INFO IIME_ldb_hindi_9017;
  #elif (KMX_LDB_HINDI==9018)
	extern const void *kmx_ldb_hindi_9018;
	extern const IIME_IME_INFO IIME_ldb_hindi_9018;
  #endif
#endif/*KMX_LDB_HINDI*/

#ifdef KMX_LDB_HINGLISH
  #if (KMX_LDB_HINGLISH==910)
  	extern const void *kmx_ldb_hinglish_910;
  	extern const IIME_IME_INFO IIME_ldb_hinglish_910;
  #elif (KMX_LDB_HINGLISH==915)
  	extern const void *kmx_ldb_hinglish_915;
  	extern const IIME_IME_INFO IIME_ldb_hinglish_915;
  #elif (KMX_LDB_HINGLISH==917)
  	extern const void *kmx_ldb_hinglish_917;
  	extern const IIME_IME_INFO IIME_ldb_hinglish_917;
  #elif (KMX_LDB_HINGLISH==9171)
  	extern const void *kmx_ldb_hinglish_9171;
  	extern const IIME_IME_INFO IIME_ldb_hinglish_9171;
  #endif
#endif/*KMX_LDB_HINGLISH*/

#ifdef KMX_LDB_HUNGARIAN
  #if (KMX_LDB_HUNGARIAN==410)
  	extern const void *kmx_ldb_hungarian_410;
  	extern const IIME_IME_INFO IIME_ldb_hungarian_410;
  #elif (KMX_LDB_HUNGARIAN==412)
  	extern const void *kmx_ldb_hungarian_412;
  	extern const IIME_IME_INFO IIME_ldb_hungarian_412;
  #elif (KMX_LDB_HUNGARIAN==413)
  	extern const void *kmx_ldb_hungarian_413;
  	extern const IIME_IME_INFO IIME_ldb_hungarian_413;
  #elif (KMX_LDB_HUNGARIAN==415)
  	extern const void *kmx_ldb_hungarian_415;
  	extern const IIME_IME_INFO IIME_ldb_hungarian_415;
  #elif (KMX_LDB_HUNGARIAN==417)
	extern const void *kmx_ldb_hungarian_417;
	extern const IIME_IME_INFO IIME_ldb_hungarian_417;	
  #endif
#endif/*KMX_LDB_HUNGARIAN*/

#ifdef KMX_LDB_INDONESIAN
  #if (KMX_LDB_INDONESIAN==320)
  	extern const void *kmx_ldb_indonesian_320;
  	extern const IIME_IME_INFO IIME_ldb_indonesian_320;
  #elif (KMX_LDB_INDONESIAN==321)
  	extern const void *kmx_ldb_indonesian_321;
  	extern const IIME_IME_INFO IIME_ldb_indonesian_321;
  #elif (KMX_LDB_INDONESIAN==325)
  	extern const void *kmx_ldb_indonesian_325;
  	extern const IIME_IME_INFO IIME_ldb_indonesian_325;
  #elif (KMX_LDB_INDONESIAN==327)
  	extern const void *kmx_ldb_indonesian_327;
  	extern const IIME_IME_INFO IIME_ldb_indonesian_327;
  #elif (KMX_LDB_INDONESIAN==3271)
	extern const void *kmx_ldb_indonesian_3271;
	extern const IIME_IME_INFO IIME_ldb_indonesian_3271;	
  #endif
#endif/*KMX_LDB_INDONESIAN*/

#ifdef KMX_LDB_ITALIAN
  #if (KMX_LDB_ITALIAN==700)
  	extern const void *kmx_ldb_italian_700;
  	extern const IIME_IME_INFO IIME_ldb_italian_700;
  #elif (KMX_LDB_ITALIAN==705)
  	extern const void *kmx_ldb_italian_705;
  	extern const IIME_IME_INFO IIME_ldb_italian_705;
  #elif (KMX_LDB_ITALIAN==707)
  	extern const void *kmx_ldb_italian_707;
  	extern const IIME_IME_INFO IIME_ldb_italian_707;
  #endif
#endif/*KMX_LDB_ITALIAN*/

#ifdef KMX_LDB_LITHUANIAN
  #if (KMX_LDB_LITHUANIAN==560)
  	extern const void *kmx_ldb_lithuanian_560;
  	extern const IIME_IME_INFO IIME_ldb_lithuanian_560;
  #elif (KMX_LDB_LITHUANIAN==565)
  	extern const void *kmx_ldb_lithuanian_565;
  	extern const IIME_IME_INFO IIME_ldb_lithuanian_565;
  #elif (KMX_LDB_LITHUANIAN==567)
  	extern const void *kmx_ldb_lithuanian_567;
  	extern const IIME_IME_INFO IIME_ldb_lithuanian_567;
  #endif
#endif/*KMX_LDB_LITHUANIAN*/

#ifdef KMX_LDB_MALAY
  #if (KMX_LDB_MALAY==340)
  	extern const void *kmx_ldb_malay_340;
  	extern const IIME_IME_INFO IIME_ldb_malay_340;
  #elif (KMX_LDB_MALAY==345)
  	extern const void *kmx_ldb_malay_345;
  	extern const IIME_IME_INFO IIME_ldb_malay_345;
  #elif (KMX_LDB_MALAY==347)
  	extern const void *kmx_ldb_malay_347;
  	extern const IIME_IME_INFO IIME_ldb_malay_347;
  #elif (KMX_LDB_MALAY==3471)
	extern const void *kmx_ldb_malay_3471;
	extern const IIME_IME_INFO IIME_ldb_malay_3471;
  #endif
#endif/*KMX_LDB_MALAY*/

#ifdef KMX_LDB_NORWEGIAN
  #if (KMX_LDB_NORWEGIAN==560)
  	extern const void *kmx_ldb_norwegian_480;
  	extern const IIME_IME_INFO IIME_ldb_norwegian_480;
  #elif (KMX_LDB_NORWEGIAN==565)
  	extern const void *kmx_ldb_norwegian_485;
  	extern const IIME_IME_INFO IIME_ldb_norwegian_485;
  #elif (KMX_LDB_NORWEGIAN==567)
  	extern const void *kmx_ldb_norwegian_487;
  	extern const IIME_IME_INFO IIME_ldb_norwegian_487;
  #endif  
#endif/*KMX_LDB_NORWEGIAN*/

#ifdef KMX_LDB_PERSIAN
  #if (KMX_LDB_PERSIAN==460)
  	extern const void *kmx_ldb_persian_460;
  	extern const IIME_IME_INFO IIME_ldb_persian_460;
  #elif (KMX_LDB_PERSIAN==462)
	extern const void *kmx_ldb_persian_462;
    extern const IIME_IME_INFO IIME_ldb_persian_462;
  #elif (KMX_LDB_PERSIAN==463)
	extern const void *kmx_ldb_persian_463;
    extern const IIME_IME_INFO IIME_ldb_persian_463;
  #elif (KMX_LDB_PERSIAN==465)
	extern const void *kmx_ldb_persian_465;
    extern const IIME_IME_INFO IIME_ldb_persian_465;
  #elif (KMX_LDB_PERSIAN==467)
	extern const void *kmx_ldb_persian_467;
    extern const IIME_IME_INFO IIME_ldb_persian_467;
  #elif (KMX_LDB_PERSIAN==4671)
	extern const void *kmx_ldb_persian_4671;
    extern const IIME_IME_INFO IIME_ldb_persian_4671;
  #elif (KMX_LDB_PERSIAN==468)
	extern const void *kmx_ldb_persian_468;
    extern const IIME_IME_INFO IIME_ldb_persian_468;
  #elif (KMX_LDB_PERSIAN==469)
	extern const void *kmx_ldb_persian_469;
    extern const IIME_IME_INFO IIME_ldb_persian_469;
  #elif (KMX_LDB_PERSIAN==4600)
	extern const void *kmx_ldb_persian_4600;
    extern const IIME_IME_INFO IIME_ldb_persian_4600;
  #endif
#endif/*KMX_LDB_PERSIAN*/

#ifdef KMX_LDB_POLISH
  #if (KMX_LDB_POLISH==391)
  	extern const void *kmx_ldb_polish_391;
  	extern const IIME_IME_INFO IIME_ldb_polish_391;
  #elif (KMX_LDB_POLISH==392)
	extern const void *kmx_ldb_polish_392;
    extern const IIME_IME_INFO IIME_ldb_polish_392;
  #elif (KMX_LDB_POLISH==393)
	extern const void *kmx_ldb_polish_393;
    extern const IIME_IME_INFO IIME_ldb_polish_393;
  #elif (KMX_LDB_POLISH==395)
	extern const void *kmx_ldb_polish_395;
    extern const IIME_IME_INFO IIME_ldb_polish_395;
  #elif (KMX_LDB_POLISH==397)
	extern const void *kmx_ldb_polish_397;
    extern const IIME_IME_INFO IIME_ldb_polish_397;
  #endif
#endif/*KMX_LDB_POLISH*/

#ifdef KMX_LDB_PORTUGUESE
  #if (KMX_LDB_PORTUGUESE==600)
  	extern const void *kmx_ldb_portuguese_600;
  	extern const IIME_IME_INFO IIME_ldb_portuguese_600;
  #elif (KMX_LDB_PORTUGUESE==601)
	extern const void *kmx_ldb_portuguese_601;
    extern const IIME_IME_INFO IIME_ldb_portuguese_601;
  #elif (KMX_LDB_PORTUGUESE==605)
	extern const void *kmx_ldb_portuguese_605;
    extern const IIME_IME_INFO IIME_ldb_portuguese_605;
  #elif (KMX_LDB_PORTUGUESE==607)
	extern const void *kmx_ldb_portuguese_607;
    extern const IIME_IME_INFO IIME_ldb_portuguese_607;
  #elif (KMX_LDB_PORTUGUESE==6071)
	extern const void *kmx_ldb_portuguese_6071;
    extern const IIME_IME_INFO IIME_ldb_portuguese_6071;
  #endif
#endif/*KMX_LDB_PORTUGUESE*/

#ifdef KMX_LDB_ROMANIAN
  #if (KMX_LDB_ROMANIAN==520)
  	extern const void *kmx_ldb_romanian_520;
  	extern const IIME_IME_INFO IIME_ldb_romanian_520;
  #elif (KMX_LDB_ROMANIAN==522)
	extern const void *kmx_ldb_romanian_522;
    extern const IIME_IME_INFO IIME_ldb_romanian_522;
  #elif (KMX_LDB_ROMANIAN==523)
	extern const void *kmx_ldb_romanian_523;
    extern const IIME_IME_INFO IIME_ldb_romanian_523;
  #elif (KMX_LDB_ROMANIAN==525)
	extern const void *kmx_ldb_romanian_525;
    extern const IIME_IME_INFO IIME_ldb_romanian_525;
  #elif (KMX_LDB_ROMANIAN==527)
	extern const void *kmx_ldb_romanian_527;
    extern const IIME_IME_INFO IIME_ldb_romanian_527;
  #endif
#endif/*KMX_LDB_ROMANIAN*/

#ifdef KMX_LDB_RUSSIAN
  #if (KMX_LDB_RUSSIAN==202)
  	extern const void *kmx_ldb_russian_202;
  	extern const IIME_IME_INFO IIME_ldb_russian_520;
  #elif (KMX_LDB_RUSSIAN==205)
	extern const void *kmx_ldb_russian_205;
    extern const IIME_IME_INFO IIME_ldb_russian_205;
  #elif (KMX_LDB_RUSSIAN==207)
	extern const void *kmx_ldb_russian_207;
    extern const IIME_IME_INFO IIME_ldb_russian_207;
  #elif (KMX_LDB_RUSSIAN==2071)
	extern const void *kmx_ldb_russian_2071;
    extern const IIME_IME_INFO IIME_ldb_russian_2071;
  #elif (KMX_LDB_RUSSIAN==208)
	extern const void *kmx_ldb_russian_208;
    extern const IIME_IME_INFO IIME_ldb_russian_208;
  #endif
#endif/*KMX_LDB_RUSSIAN*/

#ifdef KMX_LDB_SLOVAK
  #if (KMX_LDB_SLOVAK==470)
  	extern const void *kmx_ldb_slovak_470;
  	extern const IIME_IME_INFO IIME_ldb_slovak_470;
  #elif (KMX_LDB_SLOVAK==475)
  	extern const void *kmx_ldb_slovak_475;
  	extern const IIME_IME_INFO IIME_ldb_slovak_475;
  #elif (KMX_LDB_SLOVAK==477)
  	extern const void *kmx_ldb_slovak_477;
  	extern const IIME_IME_INFO IIME_ldb_slovak_477;
  #endif
#endif/*KMX_LDB_SLOVAK*/

#ifdef KMX_LDB_SLOVENIAN
  #if (KMX_LDB_SLOVENIAN==530)
  	extern const void *kmx_ldb_slovenian_530;
  	extern const IIME_IME_INFO IIME_ldb_slovenian_530;
  #elif (KMX_LDB_SLOVENIAN==535)
  	extern const void *kmx_ldb_slovenian_535;
  	extern const IIME_IME_INFO IIME_ldb_slovenian_535;
  #elif (KMX_LDB_SLOVENIAN==537)
  	extern const void *kmx_ldb_slovenian_537;
  	extern const IIME_IME_INFO IIME_ldb_slovenian_537;
  #endif
#endif/*KMX_LDB_SLOVENIAN*/

#ifdef KMX_LDB_SPANISH
  #if (KMX_LDB_SPANISH==500)
  	extern const void *kmx_ldb_spanish_500;
  	extern const IIME_IME_INFO IIME_ldb_spanish_500;
  #elif (KMX_LDB_SPANISH==501)
	extern const void *kmx_ldb_spanish_501;
    extern const IIME_IME_INFO IIME_ldb_spanish_501;
  #elif (KMX_LDB_SPANISH==504)
	extern const void *kmx_ldb_spanish_504;
    extern const IIME_IME_INFO IIME_ldb_spanish_504;
  #elif (KMX_LDB_SPANISH==505)
	extern const void *kmx_ldb_spanish_505;
    extern const IIME_IME_INFO IIME_ldb_spanish_505;
  #elif (KMX_LDB_SPANISH==507)
	extern const void *kmx_ldb_spanish_507;
    extern const IIME_IME_INFO IIME_ldb_spanish_507;
  #elif (KMX_LDB_SPANISH==5071)
	extern const void *kmx_ldb_spanish_5071;
	extern const IIME_IME_INFO IIME_ldb_spanish_5071;
  #endif
#endif/*KMX_LDB_SPANISH*/

#ifdef KMX_LDB_SWAHILI
  #if (KMX_LDB_SWAHILI==550)
  	extern const void *kmx_ldb_swahili_550;
  	extern const IIME_IME_INFO IIME_ldb_swahili_550;
  #elif (KMX_LDB_SWAHILI==555)
  	extern const void *kmx_ldb_swahili_555;
  	extern const IIME_IME_INFO IIME_ldb_swahili_555;
  #elif (KMX_LDB_SWAHILI==557)
  	extern const void *kmx_ldb_swahili_557;
  	extern const IIME_IME_INFO IIME_ldb_swahili_557;
  #endif
#endif/*KMX_LDB_SWAHILI*/

#ifdef KMX_LDB_SWEDISH
  #if (KMX_LDB_SWEDISH==490)
  	extern const void *kmx_ldb_swedish_490;
  	extern const IIME_IME_INFO IIME_ldb_swedish_490;
  #elif (KMX_LDB_SWEDISH==495)
  	extern const void *kmx_ldb_swedish_495;
  	extern const IIME_IME_INFO IIME_ldb_swedish_495;
  #elif (KMX_LDB_SWEDISH==497)
  	extern const void *kmx_ldb_swedish_497;
  	extern const IIME_IME_INFO IIME_ldb_swedish_497;
  #endif
#endif/*KMX_LDB_SWEDISH*/

#ifdef KMX_LDB_THAI
  #if (KMX_LDB_THAI==360)
  	extern const void *kmx_ldb_thai_360;
  	extern const IIME_IME_INFO IIME_ldb_thai_360;
  #elif (KMX_LDB_THAI==361)
	extern const void *kmx_ldb_thai_361;
    extern const IIME_IME_INFO IIME_ldb_thai_361;
  #elif (KMX_LDB_THAI==362)
	extern const void *kmx_ldb_thai_362;
    extern const IIME_IME_INFO IIME_ldb_thai_362;
  #elif (KMX_LDB_THAI==363)
	extern const void *kmx_ldb_thai_363;
    extern const IIME_IME_INFO IIME_ldb_thai_363;
  #elif (KMX_LDB_THAI==364)
	extern const void *kmx_ldb_thai_364;
    extern const IIME_IME_INFO IIME_ldb_thai_364;
  #elif (KMX_LDB_THAI==365)
	extern const void *kmx_ldb_thai_365;
    extern const IIME_IME_INFO IIME_ldb_thai_365;
  #elif (KMX_LDB_THAI==366)
	extern const void *kmx_ldb_thai_366;
    extern const IIME_IME_INFO IIME_ldb_thai_366;
  #elif (KMX_LDB_THAI==367)
	extern const void *kmx_ldb_thai_367;
    extern const IIME_IME_INFO IIME_ldb_thai_367;
  #elif (KMX_LDB_THAI==3671)
	extern const void *kmx_ldb_thai_3671;
    extern const IIME_IME_INFO IIME_ldb_thai_3671;
  #elif (KMX_LDB_THAI==3600)
		extern const void *kmx_ldb_thai_3600;
		extern const IIME_IME_INFO IIME_ldb_thai_3600;
  #elif (KMX_LDB_THAI==3601)
		extern const void *kmx_ldb_thai_3601;
		extern const IIME_IME_INFO IIME_ldb_thai_3601;
  #elif (KMX_LDB_THAI==3602)
		extern const void *kmx_ldb_thai_3602;
		extern const IIME_IME_INFO IIME_ldb_thai_3602;
  #elif (KMX_LDB_THAI==3603)
		extern const void *kmx_ldb_thai_3603;
		extern const IIME_IME_INFO IIME_ldb_thai_3603;
  #elif (KMX_LDB_THAI==3604)
		extern const void *kmx_ldb_thai_3604;
		extern const IIME_IME_INFO IIME_ldb_thai_3604;
  #elif (KMX_LDB_THAI==3606)
		extern const void *kmx_ldb_thai_3606;
		extern const IIME_IME_INFO IIME_ldb_thai_3606;
  #elif (KMX_LDB_THAI==3607)
		extern const void *kmx_ldb_thai_3607;
		extern const IIME_IME_INFO IIME_ldb_thai_3607;
  #elif (KMX_LDB_THAI==3608)
		extern const void *kmx_ldb_thai_3608;
		extern const IIME_IME_INFO IIME_ldb_thai_3608;
  #elif (KMX_LDB_THAI==3611)
		extern const void *kmx_ldb_thai_3611;
		extern const IIME_IME_INFO IIME_ldb_thai_3611;
  #elif (KMX_LDB_THAI==3616)
		extern const void *kmx_ldb_thai_3616;
		extern const IIME_IME_INFO IIME_ldb_thai_3616;
  #elif (KMX_LDB_THAI==3640)
		extern const void *kmx_ldb_thai_3640;
		extern const IIME_IME_INFO IIME_ldb_thai_3640;
  #elif (KMX_LDB_THAI==3632)
		extern const void *kmx_ldb_thai_3632;
		extern const IIME_IME_INFO IIME_ldb_thai_3632;	
  #endif	
#endif/*KMX_LDB_THAI*/

#ifdef KMX_LDB_TIBETAN
  extern const void *kmx_ldb_tibetan_730;	
  extern const IIME_IME_INFO IIME_ldb_tibetan_730;
#endif/*KMX_LDB_TIBETAN*/

#ifdef KMX_LDB_TURKISH
  #if (KMX_LDB_TURKISH==330)
	extern const void *kmx_ldb_turkish_330;
	extern const IIME_IME_INFO IIME_ldb_turkish_330;
  #elif (KMX_LDB_TURKISH==335)
	extern const void *kmx_ldb_turkish_335;
	extern const IIME_IME_INFO IIME_ldb_turkish_335;
  #elif (KMX_LDB_TURKISH==337)
	extern const void *kmx_ldb_turkish_337;
	extern const IIME_IME_INFO IIME_ldb_turkish_337;
  #elif (KMX_LDB_TURKISH==3371)
	extern const void *kmx_ldb_turkish_3371;
	extern const IIME_IME_INFO IIME_ldb_turkish_3371;	
  #endif
#endif/*KMX_LDB_TURKISH*/

#ifdef KMX_LDB_UKRAINIAN
  #if (KMX_LDB_UKRAINIAN==540)
	extern const void *kmx_ldb_ukrainian_540;
	extern const IIME_IME_INFO IIME_ldb_ukrainian_540;
  #elif (KMX_LDB_UKRAINIAN==545)
	extern const void *kmx_ldb_ukrainian_545;
	extern const IIME_IME_INFO IIME_ldb_ukrainian_545;
  #elif (KMX_LDB_UKRAINIAN==547)
	extern const void *kmx_ldb_ukrainian_547;
	extern const IIME_IME_INFO IIME_ldb_ukrainian_547;
  #elif (KMX_LDB_UKRAINIAN==548)
	extern const void *kmx_ldb_ukrainian_548;
	extern const IIME_IME_INFO IIME_ldb_ukrainian_548;	
  #endif
#endif/*KMX_LDB_UKRAINIAN*/

#ifdef KMX_LDB_UYGHUR
  #if (KMX_LDB_UYGHUR==940)
	extern const void *kmx_ldb_uyghur_940;
	extern const IIME_IME_INFO IIME_ldb_uyghur_940;
  #elif (KMX_LDB_UYGHUR==941)
	extern const void *kmx_ldb_uyghur_941;
	extern const IIME_IME_INFO IIME_ldb_uyghur_941;
  #elif (KMX_LDB_UYGHUR==947)
	extern const void *kmx_ldb_uyghur_947;
	extern const IIME_IME_INFO IIME_ldb_uyghur_947;
  #elif (KMX_LDB_UYGHUR==948)
	extern const void *kmx_ldb_uyghur_948;
	extern const IIME_IME_INFO IIME_ldb_uyghur_948;	
  #endif
#endif/*KMX_LDB_UYGHUR*/

#ifdef KMX_LDB_VIETNAMESE
  #if (KMX_LDB_VIETNAMESE==310)
	extern const void *kmx_ldb_vietnamese_310;
	extern const IIME_IME_INFO IIME_ldb_vietnamese_310;
  #elif (KMX_LDB_VIETNAMESE==315)
	extern const void *kmx_ldb_vietnamese_315;
	extern const IIME_IME_INFO IIME_ldb_vietnamese_315;
  #elif (KMX_LDB_VIETNAMESE==317)
	extern const void *kmx_ldb_vietnamese_317;
	extern const IIME_IME_INFO IIME_ldb_vietnamese_317;
  #elif (KMX_LDB_VIETNAMESE==3171)
	extern const void *kmx_ldb_vietnamese_3171;
	extern const IIME_IME_INFO IIME_ldb_vietnamese_3171;	
  #endif
#endif/*KMX_LDB_VIETNAMESE*/

#ifdef KMX_LDB_URDU
  #if (KMX_LDB_URDU==950)
	extern const void *kmx_ldb_urdu_950;
	extern const IIME_IME_INFO IIME_ldb_urdu_950;
  #elif (KMX_LDB_URDU==955)
	extern const void *kmx_ldb_urdu_955;
	extern const IIME_IME_INFO IIME_ldb_urdu_955;
  #elif (KMX_LDB_URDU==957)
	extern const void *kmx_ldb_urdu_957;
	extern const IIME_IME_INFO IIME_ldb_urdu_957;
  #elif (KMX_LDB_URDU==9571)
	extern const void *kmx_ldb_urdu_9571;
	extern const IIME_IME_INFO IIME_ldb_urdu_9571;	
  #endif
#endif/*KMX_LDB_URDU*/

#ifdef KMX_LDB_TAGALOG
  extern const void *kmx_ldb_tagalog_587;
  extern const IIME_IME_INFO IIME_ldb_tagalog_587;
#endif/*KMX_LDB_TAGALOG*/

#ifdef KMX_LDB_KYRGYZ
  extern const void *kmx_ldb_kyrgyz_747;
  extern const IIME_IME_INFO IIME_ldb_kyrgyz_747;
#endif/*KMX_LDB_KYRGYZ*/

#ifdef KMX_LDB_NEPALI
  extern const void *kmx_ldb_nepali_720;
  extern const IIME_IME_INFO IIME_ldb_nepali_720;
#endif/*KMX_LDB_NEPALI*/

#ifdef KMX_LDB_SINHALA
  extern const void *kmx_ldb_sinhala_770;
  extern const IIME_IME_INFO IIME_ldb_sinhala_770;
#endif/*KMX_LDB_NEPALI*/

#ifdef KMX_LDB_BODO//daodejing
  extern const void *kmx_ldb_bodo_1100;
  extern const IIME_IME_INFO IIME_ldb_bodo_1100;
#endif/*KMX_LDB_BODO*/

#ifdef KMX_LDB_DOGRI
  extern const void *kmx_ldb_dogri_1110;
  extern const IIME_IME_INFO IIME_ldb_dogri_1110;
#endif/*KMX_LDB_DOGRI*/

#ifdef KMX_LDB_SANSKRIT
  extern const void *kmx_ldb_sanskrit_1120;
  extern const IIME_IME_INFO IIME_ldb_sanskrit_1120;
#endif/*KMX_LDB_SANSKRIT*/

#ifdef KMX_LDB_KASHMIRI
  extern const void *kmx_ldb_kashmiri_1130;
  extern const IIME_IME_INFO IIME_ldb_kashmiri_1130;
#endif/*KMX_LDB_KASHMIRI*/

#ifdef KMX_LDB_KONKANI
  extern const void *kmx_ldb_konkani_1140;
  extern const IIME_IME_INFO IIME_ldb_konkani_1140;
#endif/*KMX_LDB_KONKANI*/

#ifdef KMX_LDB_MAITHILI
  extern const void *kmx_ldb_maithili_1150;
  extern const IIME_IME_INFO IIME_ldb_maithili_1150;
#endif/*KMX_LDB_MAITHILI*/

#ifdef KMX_LDB_MANIPURI
  extern const void *kmx_ldb_manipuri_1160;
  extern const IIME_IME_INFO IIME_ldb_manipuri_1160;
#endif/*KMX_LDB_MANIPURI*/

#ifdef KMX_LDB_SANTALI
  extern const void *kmx_ldb_santali_1170;
  extern const IIME_IME_INFO IIME_ldb_santali_1170;
#endif/*KMX_LDB_SANTALI*/

#ifdef KMX_LDB_SOMALI
  extern const void *kmx_ldb_somali_1180;
  extern const IIME_IME_INFO IIME_ldb_somali_1180;
#endif/*KMX_LDB_SOMALI*/

#ifdef KMX_LDB_SINDHI
  extern const void *kmx_ldb_sindhi_1190;
  extern const IIME_IME_INFO IIME_ldb_sindhi_1190;
#endif/*KMX_LDB_SINDHI*/

#ifdef KMX_LDB_ASSAMESE
  extern const void *kmx_ldb_assamese_710;
  extern const IIME_IME_INFO IIME_ldb_assamese_710;	
#endif/*KMX_LDB_ASSAMESE*/

#ifdef KMX_LDB_ALBANIAN
  extern const void *kmx_ldb_albanian_611;
  extern const IIME_IME_INFO IIME_ldb_albanian_611;
#endif/*KMX_LDB_ALBANIAN*/

#ifdef KMX_LDB_CATALAN
  extern const void *kmx_ldb_catalan_621;
  extern const IIME_IME_INFO IIME_ldb_catalan_621;
#endif/*KMX_LDB_CATALAN*/

#ifdef KMX_LDB_BASQUE
  extern const void *kmx_ldb_basque_631;
  extern const IIME_IME_INFO IIME_ldb_basque_631;
#endif/*KMX_LDB_BASQUE*/

#ifdef KMX_LDB_GALICIAN
  extern const void *kmx_ldb_galician_641;
  extern const IIME_IME_INFO IIME_ldb_galician_641;
#endif/*KMX_LDB_GALICIAN*/

#ifdef KMX_LDB_IGBO
  extern const void *kmx_ldb_igbo_651;
  extern const IIME_IME_INFO IIME_ldb_igbo_651;
#endif/*KMX_LDB_IGBO*/

#ifdef KMX_LDB_OROMO
  extern const void *kmx_ldb_oromo_661;
  extern const IIME_IME_INFO IIME_ldb_oromo_661;
#endif/*KMX_LDB_OROMO*/

#ifdef KMX_LDB_SERBIAN
  extern const void *kmx_ldb_serbian_233;
  extern const IIME_IME_INFO IIME_ldb_serbian_233;
#endif/*KMX_LDB_SERBIAN*/

#ifdef KMX_LDB_SERBIANC
  extern const void *kmx_ldb_serbianc_243;
  extern const IIME_IME_INFO IIME_ldb_serbianc_243;
#endif/*KMX_LDB_SERBIANC*/

#ifdef KMX_LDB_ZULU
  extern const void *kmx_ldb_zulu_677;
  extern const IIME_IME_INFO IIME_ldb_zulu_677;
#endif/*KMX_LDB_ZULU*/

#ifdef KMX_LDB_CEBUANO
  extern const void *kmx_ldb_cebuano_687;
  extern const IIME_IME_INFO IIME_ldb_cebuano_687;
#endif/*KMX_LDB_CEBUANO*/

#ifdef KMX_LDB_KAZAKH
  extern const void *kmx_ldb_kazakh_257;
  extern const IIME_IME_INFO IIME_ldb_kazakh_257;
#endif/*KMX_LDB_KAZAKH*/

#ifdef KMX_LDB_UZBEK
  extern const void *kmx_ldb_uzbek_267;
  extern const IIME_IME_INFO IIME_ldb_uzbek_267;
#endif/*KMX_LDB_UZBEK*/

#ifdef KMX_LDB_GEORGIAN
  extern const void *kmx_ldb_georgian_277;
  extern const IIME_IME_INFO IIME_ldb_georgian_277;
#endif/*KMX_LDB_GEORGIAN*/

#endif/*IEKIE_INPUT_SAME_LANGUAGE_ALL_LDB*/
#endif /* __KMX_SAME_LDB_H__ */

