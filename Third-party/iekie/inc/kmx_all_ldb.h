#ifndef __KMX_ALL_LDB_H__
#define __KMX_ALL_LDB_H__

#if defined(IIME_ALL_IN_ONE_DATABASE)
    /*
     *  pinyin and zhuyin ldb
     */
    #if 0//ZHMCH	iekie 的IIME没有中文输入法
    extern const void *kmx_ldb_py_gb2312_c1_20k;
    extern const void *kmx_ldb_py_gb2312_c1_10k;
    extern const void *kmx_ldb_py_gb2312_c1_5k;
    extern const void *kmx_ldb_py_gb2312_c2_20k;
    extern const void *kmx_ldb_py_gb2312_c2_10k;
    extern const void *kmx_ldb_py_gb2312_c2_5k;
    extern const void *kmx_ldb_py_gbk_c1_20k;
    extern const void *kmx_ldb_py_gbk_c1_10k;
    extern const void *kmx_ldb_py_gbk_c1_5k;
    extern const void *kmx_ldb_py_gbk_c2_20k;
    extern const void *kmx_ldb_py_gbk_c2_10k;
    extern const void *kmx_ldb_py_gbk_c2_5k;
    extern const void *kmx_ldb_py_gb18030_c1_20k;
    extern const void *kmx_ldb_py_gb18030_c1_10k;
    extern const void *kmx_ldb_py_gb18030_c1_5k;
    extern const void *kmx_ldb_py_gb18030_c2_20k;
    extern const void *kmx_ldb_py_gb18030_c2_10k;
    extern const void *kmx_ldb_py_gb18030_c2_5k;

    extern const void *kmx_ldb_py_big5_c1_20k;
    extern const void *kmx_ldb_py_big5_c1_10k;
    extern const void *kmx_ldb_py_big5_c2_5k;
    extern const void *kmx_ldb_py_big5_c2_20k;
    extern const void *kmx_ldb_py_big5_c1_10k;
    extern const void *kmx_ldb_py_big5_c2_5k;
    extern const void *kmx_ldb_py_bignormal_c1_20k;
    extern const void *kmx_ldb_py_bignormal_c1_10k;
    extern const void *kmx_ldb_py_bignormal_c1_5k;
    extern const void *kmx_ldb_py_bignormal_c2_20k;
    extern const void *kmx_ldb_py_bignormal_c2_10k;
    extern const void *kmx_ldb_py_bignormal_c2_5k;
    
    extern const void *kmx_ldb_qpy_gb2312_c1_20k;
    extern const void *kmx_ldb_qpy_gb2312_c1_10k;
    extern const void *kmx_ldb_qpy_gb2312_c1_5k;
    extern const void *kmx_ldb_qpy_gb2312_c2_20k;
    extern const void *kmx_ldb_qpy_gb2312_c2_10k;
    extern const void *kmx_ldb_qpy_gb2312_c2_5k;
    extern const void *kmx_ldb_qpy_gbk_c1_20k;
    extern const void *kmx_ldb_qpy_gbk_c1_10k;
    extern const void *kmx_ldb_qpy_gbk_c1_5k;
    extern const void *kmx_ldb_qpy_gbk_c2_20k;
    extern const void *kmx_ldb_qpy_gbk_c2_10k;
    extern const void *kmx_ldb_qpy_gbk_c2_5k;
    extern const void *kmx_ldb_qpy_gb18030_c1_20k;
    extern const void *kmx_ldb_qpy_gb18030_c1_10k;
    extern const void *kmx_ldb_qpy_gb18030_c1_5k;
    extern const void *kmx_ldb_qpy_gb18030_c2_20k;
    extern const void *kmx_ldb_qpy_gb18030_c2_10k;
    extern const void *kmx_ldb_qpy_gb18030_c2_5k;

    extern const void *kmx_ldb_qpy_big5_c1_20k;
    extern const void *kmx_ldb_qpy_big5_c1_10k;
    extern const void *kmx_ldb_qpy_big5_c2_5k;
    extern const void *kmx_ldb_qpy_big5_c2_20k;
    extern const void *kmx_ldb_qpy_big5_c1_10k;
    extern const void *kmx_ldb_qpy_big5_c2_5k;
    extern const void *kmx_ldb_qpy_bignormal_c1_20k;
    extern const void *kmx_ldb_qpy_bignormal_c1_10k;
    extern const void *kmx_ldb_qpy_bignormal_c1_5k;
    extern const void *kmx_ldb_qpy_bignormal_c2_20k;
    extern const void *kmx_ldb_qpy_bignormal_c2_10k;
    extern const void *kmx_ldb_qpy_bignormal_c2_5k;
    
    extern const void *kmx_ldb_stpy_gb2312_c1_20k;
    extern const void *kmx_ldb_stpy_gbk_c1_20k;
    extern const void *kmx_ldb_stpy_gb18030_c1_20k;
    extern const void *kmx_ldb_stpy_big5normal_c1_20k;
    
    extern const void *kmx_ldb_dzpy_gb2312_20k;
    extern const void *kmx_ldb_dzpy_gb2312_10k;
    extern const void *kmx_ldb_dzpy_gb2312_5k;
    extern const void *kmx_ldb_dzpy_gb2312_0k;
    
    extern const void *kmx_ldb_dzpy_gbk_20k;
    extern const void *kmx_ldb_dzpy_gbk_10k;
    extern const void *kmx_ldb_dzpy_gbk_5k;
    extern const void *kmx_ldb_dzpy_gbk_0k;
    
    extern const void *kmx_ldb_zy_gb2312_20k;
    extern const void *kmx_ldb_zy_gb18030_20k;

    extern const void *kmx_ldb_zy_big5_20k;
    extern const void *kmx_ldb_zy_big5_10k;
    extern const void *kmx_ldb_zy_big5_5k;
    extern const void *kmx_ldb_zy_big5normal_20k;
    extern const void *kmx_ldb_zy_big5normal_10k;
    extern const void *kmx_ldb_zy_big5normal_5k;
    extern const void *kmx_ldb_zy_big5hkscs_20k;
    extern const void *kmx_ldb_zy_big5hkscs_10k;
    extern const void *kmx_ldb_zy_big5hkscs_5k;
    extern const void *kmx_ldb_dzzy_big5normal_20k;
    extern const void *kmx_ldb_dzzy_big5normal_10k;
    extern const void *kmx_ldb_dzzy_big5normal_5k;
    extern const void *kmx_ldb_dzzy_big5normal_0k;
    
    extern const IIME_IME_INFO IIME_ldb_py_gb2312_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_py_gb2312_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_py_gb2312_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_py_gb2312_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_py_gb2312_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_py_gb2312_c2_5k;
    extern const IIME_IME_INFO IIME_ldb_py_gbk_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_py_gbk_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_py_gbk_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_py_gbk_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_py_gbk_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_py_gbk_c2_5k;
    extern const IIME_IME_INFO IIME_ldb_py_gb18030_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_py_gb18030_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_py_gb18030_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_py_gb18030_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_py_gb18030_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_py_gb18030_c2_5k;

    extern const IIME_IME_INFO IIME_ldb_py_big5_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_py_big5_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_py_big5_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_py_big5_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_py_big5_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_py_big5_c2_5k;
    extern const IIME_IME_INFO IIME_ldb_py_bignormal_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_py_bignormal_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_py_bignormal_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_py_bignormal_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_py_bignormal_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_py_bignormal_c2_5k;

    extern const IIME_IME_INFO IIME_ldb_qpy_gb2312_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb2312_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb2312_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb2312_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb2312_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb2312_c2_5k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gbk_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gbk_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gbk_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gbk_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gbk_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gbk_c2_5k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb18030_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb18030_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb18030_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb18030_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb18030_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_gb18030_c2_5k;

    extern const IIME_IME_INFO IIME_ldb_qpy_big5_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_big5_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_big5_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_qpy_big5_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_big5_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_big5_c2_5k;
    extern const IIME_IME_INFO IIME_ldb_qpy_bignormal_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_bignormal_c1_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_bignormal_c1_5k;
    extern const IIME_IME_INFO IIME_ldb_qpy_bignormal_c2_20k;
    extern const IIME_IME_INFO IIME_ldb_qpy_bignormal_c2_10k;
    extern const IIME_IME_INFO IIME_ldb_qpy_bignormal_c2_5k;

    extern const IIME_IME_INFO IIME_ldb_stpy_gb2312_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_stpy_gbk_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_stpy_gb18030_c1_20k;
    extern const IIME_IME_INFO IIME_ldb_stpy_big5normal_c1_20k;

    extern const IIME_IME_INFO IIME_ldb_dzpy_gb2312_20k;
    extern const IIME_IME_INFO IIME_ldb_dzpy_gb2312_10k;
    extern const IIME_IME_INFO IIME_ldb_dzpy_gb2312_5k;
    extern const IIME_IME_INFO IIME_ldb_dzpy_gb2312_0k;

    extern const IIME_IME_INFO IIME_ldb_dzpy_gbk_20k;
    extern const IIME_IME_INFO IIME_ldb_dzpy_gbk_10k;
    extern const IIME_IME_INFO IIME_ldb_dzpy_gbk_5k;
    extern const IIME_IME_INFO IIME_ldb_dzpy_gbk_0k;

    extern const IIME_IME_INFO IIME_ldb_zy_gb2312_20k;
    extern const IIME_IME_INFO IIME_ldb_zy_gb18030_20k;

    extern const IIME_IME_INFO IIME_ldb_zy_big5_20k;
    extern const IIME_IME_INFO IIME_ldb_zy_big5_10k;
    extern const IIME_IME_INFO IIME_ldb_zy_big5_5k;
    extern const IIME_IME_INFO IIME_ldb_zy_big5normal_20k;
    extern const IIME_IME_INFO IIME_ldb_zy_big5normal_10k;
    extern const IIME_IME_INFO IIME_ldb_zy_big5normal_5k;
    extern const IIME_IME_INFO IIME_ldb_zy_big5hkscs_20k;
    extern const IIME_IME_INFO IIME_ldb_zy_big5hkscs_10k;
    extern const IIME_IME_INFO IIME_ldb_zy_big5hkscs_5k;

    extern const IIME_IME_INFO IIME_ldb_dzzy_big5normal_20k;
    extern const IIME_IME_INFO IIME_ldb_dzzy_big5normal_10k;
    extern const IIME_IME_INFO IIME_ldb_dzzy_big5normal_5k;
    extern const IIME_IME_INFO IIME_ldb_dzzy_big5normal_0k;



    /*
     * stroke ldb
     */

    /* simplifiled chinese */
    extern const void *kmx_ldb_stroke_gb2312_plain;
    extern const void *kmx_ldb_stroke_gb2312_mini;
    extern const void *kmx_ldb_stroke_gb2312_cizu;

    extern const void *kmx_ldb_stroke_gbk_plain;
    extern const void *kmx_ldb_stroke_gbk_cizu;

    /* traditional chinese */
    extern const void *kmx_ldb_stroke_big5_plain;
    extern const void *kmx_ldb_stroke_big5_normal;
    extern const void *kmx_ldb_stroke_big5_mini;
    extern const void *kmx_ldb_stroke_big5_normal_mini;
    extern const void *kmx_ldb_stroke_big5_cizu;
    extern const void *kmx_ldb_stroke_big5_hkscs;

    /* gb18030 */
    extern const void *kmx_ldb_stroke_gb18030_plain;
    extern const void *kmx_ldb_stroke_gb18030_cizu;

    /* simplifiled chinese */
    extern const IIME_IME_INFO IIME_ldb_stroke_gb2312_plain;
    extern const IIME_IME_INFO IIME_ldb_stroke_gb2312_mini;
    extern const IIME_IME_INFO IIME_ldb_stroke_gb2312_cizu;

    extern const IIME_IME_INFO IIME_ldb_stroke_gbk_plain;
    extern const IIME_IME_INFO IIME_ldb_stroke_gbk_cizu;


    /* traditional chinese */
    extern const IIME_IME_INFO IIME_ldb_stroke_big5_plain;
    extern const IIME_IME_INFO IIME_ldb_stroke_big5_normal;

    extern const IIME_IME_INFO IIME_ldb_stroke_big5_mini;
    extern const IIME_IME_INFO IIME_ldb_stroke_big5_normal_mini;
    extern const IIME_IME_INFO IIME_ldb_stroke_big5_cizu;
    extern const IIME_IME_INFO IIME_ldb_stroke_big5_hkscs;

    /* gb18030 */
    extern const IIME_IME_INFO IIME_ldb_stroke_gb18030_plain;
    extern const IIME_IME_INFO IIME_ldb_stroke_gb18030_cizu;
	#endif
	
    /*
     * latin ldb
     */
    extern const void *kmx_ldb_armenian_570;
    extern const void *kmx_ldb_armenian_575;
    extern const void *kmx_ldb_armenian_577;

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

    extern const void *kmx_ldb_bulgarian_430;
    extern const void *kmx_ldb_bulgarian_435;
    extern const void *kmx_ldb_bulgarian_437;

    extern const void *kmx_ldb_croatian_510;
    extern const void *kmx_ldb_croatian_515;
    extern const void *kmx_ldb_croatian_517;

    extern const void *kmx_ldb_czech_440;
    extern const void *kmx_ldb_czech_442;
    extern const void *kmx_ldb_czech_443;
    extern const void *kmx_ldb_czech_445;
    extern const void *kmx_ldb_czech_447;

    extern const void *kmx_ldb_danish_370;
    extern const void *kmx_ldb_danish_375;
    extern const void *kmx_ldb_danish_377;

    extern const void *kmx_ldb_dutch_380;
    extern const void *kmx_ldb_dutch_385;
    extern const void *kmx_ldb_dutch_387;

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

    extern const void *kmx_ldb_finnish_450;
    extern const void *kmx_ldb_finnish_455;
    extern const void *kmx_ldb_finnish_457;

    extern const void *kmx_ldb_french_400;
    extern const void *kmx_ldb_french_405;
    extern const void *kmx_ldb_french_406;
    extern const void *kmx_ldb_french_407;
    extern const void *kmx_ldb_french_4071;

    extern const void *kmx_ldb_german_300;
    extern const void *kmx_ldb_german_305;
    extern const void *kmx_ldb_german_307;
    extern const void *kmx_ldb_german_308;

    extern const void *kmx_ldb_greek_350;
    extern const void *kmx_ldb_greek_355;
    extern const void *kmx_ldb_greek_357;

    extern const void *kmx_ldb_hebrew_420;
    extern const void *kmx_ldb_hebrew_425;
    extern const void *kmx_ldb_hebrew_427;
    extern const void *kmx_ldb_hebrew_4271;

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

    extern const void *kmx_ldb_hinglish_910;
    extern const void *kmx_ldb_hinglish_915;
    extern const void *kmx_ldb_hinglish_917;
    extern const void *kmx_ldb_hinglish_9171;

    extern const void *kmx_ldb_hungarian_410;
    extern const void *kmx_ldb_hungarian_412;
    extern const void *kmx_ldb_hungarian_413;
    extern const void *kmx_ldb_hungarian_415;
    extern const void *kmx_ldb_hungarian_417;

    extern const void *kmx_ldb_indonesian_320;
    extern const void *kmx_ldb_indonesian_321;
    extern const void *kmx_ldb_indonesian_325;
    extern const void *kmx_ldb_indonesian_327;
    extern const void *kmx_ldb_indonesian_3271;

    extern const void *kmx_ldb_italian_700;
    extern const void *kmx_ldb_italian_705;
    extern const void *kmx_ldb_italian_707;

    extern const void *kmx_ldb_lithuanian_560;
    extern const void *kmx_ldb_lithuanian_565;
    extern const void *kmx_ldb_lithuanian_567;

    extern const void *kmx_ldb_malay_340;
    extern const void *kmx_ldb_malay_345;
    extern const void *kmx_ldb_malay_347;
    extern const void *kmx_ldb_malay_3471;

    extern const void *kmx_ldb_norwegian_480;
    extern const void *kmx_ldb_norwegian_485;
    extern const void *kmx_ldb_norwegian_487;

    extern const void *kmx_ldb_persian_460;
    extern const void *kmx_ldb_persian_462;
    extern const void *kmx_ldb_persian_463;
    extern const void *kmx_ldb_persian_465;
    extern const void *kmx_ldb_persian_467;
    extern const void *kmx_ldb_persian_4671;
    extern const void *kmx_ldb_persian_468;
    extern const void *kmx_ldb_persian_469;
    extern const void *kmx_ldb_persian_4600;

    extern const void *kmx_ldb_polish_391;
    extern const void *kmx_ldb_polish_392;
    extern const void *kmx_ldb_polish_393;
    extern const void *kmx_ldb_polish_395;
    extern const void *kmx_ldb_polish_397;

    extern const void *kmx_ldb_portuguese_600;
    extern const void *kmx_ldb_portuguese_601;
    extern const void *kmx_ldb_portuguese_605;
    extern const void *kmx_ldb_portuguese_607;
    extern const void *kmx_ldb_portuguese_6071;

    extern const void *kmx_ldb_romanian_520;
    extern const void *kmx_ldb_romanian_522;
    extern const void *kmx_ldb_romanian_523;
    extern const void *kmx_ldb_romanian_525;
    extern const void *kmx_ldb_romanian_527;

    extern const void *kmx_ldb_russian_202;
    extern const void *kmx_ldb_russian_205;
    extern const void *kmx_ldb_russian_207;
    extern const void *kmx_ldb_russian_2071;
    extern const void *kmx_ldb_russian_208;

    extern const void *kmx_ldb_slovak_470;
    extern const void *kmx_ldb_slovak_475;
    extern const void *kmx_ldb_slovak_477;

    extern const void *kmx_ldb_slovenian_530;
    extern const void *kmx_ldb_slovenian_535;
    extern const void *kmx_ldb_slovenian_537;

    extern const void *kmx_ldb_spanish_500;
    extern const void *kmx_ldb_spanish_501;
    extern const void *kmx_ldb_spanish_504;
    extern const void *kmx_ldb_spanish_505;
    extern const void *kmx_ldb_spanish_507;
    extern const void *kmx_ldb_spanish_5071;

    extern const void *kmx_ldb_swahili_550;
    extern const void *kmx_ldb_swahili_555;
    extern const void *kmx_ldb_swahili_557;

    extern const void *kmx_ldb_swedish_490;
    extern const void *kmx_ldb_swedish_495;
    extern const void *kmx_ldb_swedish_497;

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
    
    extern const void *kmx_ldb_tibetan_730;	

    extern const void *kmx_ldb_turkish_330;
    extern const void *kmx_ldb_turkish_335;
    extern const void *kmx_ldb_turkish_337;
    extern const void *kmx_ldb_turkish_3371;

    extern const void *kmx_ldb_ukrainian_540;
    extern const void *kmx_ldb_ukrainian_545;
    extern const void *kmx_ldb_ukrainian_547;
    extern const void *kmx_ldb_ukrainian_548;

    extern const void *kmx_ldb_uyghur_940;
    extern const void *kmx_ldb_uyghur_941;
    extern const void *kmx_ldb_uyghur_947;
    extern const void *kmx_ldb_uyghur_948;

    extern const void *kmx_ldb_vietnamese_310;
    extern const void *kmx_ldb_vietnamese_315;
    extern const void *kmx_ldb_vietnamese_317;
    extern const void *kmx_ldb_vietnamese_3171;

    extern const void *kmx_ldb_urdu_950;
    extern const void *kmx_ldb_urdu_955;
    extern const void *kmx_ldb_urdu_957;
    extern const void *kmx_ldb_urdu_9571;

    extern const void *kmx_ldb_tagalog_587;
    extern const void *kmx_ldb_kyrgyz_747;

    extern const void *kmx_ldb_nepali_720;
	extern const void *kmx_ldb_bodo_1100;//daodejing
	extern const void *kmx_ldb_dogri_1110;
	extern const void *kmx_ldb_sanskrit_1120;
	extern const void *kmx_ldb_kashmiri_1130;
	extern const void *kmx_ldb_konkani_1140;
	extern const void *kmx_ldb_maithili_1150;
	extern const void *kmx_ldb_manipuri_1160;
	extern const void *kmx_ldb_santali_1170;
	extern const void *kmx_ldb_somali_1180;
	extern const void *kmx_ldb_sindhi_1190;
	
    extern const void *kmx_ldb_assamese_710;

    extern const void *kmx_ldb_albanian_611;
    extern const void *kmx_ldb_catalan_621;
    extern const void *kmx_ldb_basque_631;
    extern const void *kmx_ldb_galician_641;
    extern const void *kmx_ldb_igbo_651;
    extern const void *kmx_ldb_oromo_661;

    extern const void *kmx_ldb_serbian_233;
    extern const void *kmx_ldb_serbianc_243;
    
    extern const void *kmx_ldb_zulu_677;
    extern const void *kmx_ldb_cebuano_687;

    extern const void *kmx_ldb_kazakh_257;
    extern const void *kmx_ldb_uzbek_267;
    extern const void *kmx_ldb_georgian_277;

    extern const IIME_IME_INFO IIME_ldb_armenian_570;
    extern const IIME_IME_INFO IIME_ldb_armenian_575;
    extern const IIME_IME_INFO IIME_ldb_armenian_577;

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

    extern const IIME_IME_INFO IIME_ldb_bulgarian_430;
    extern const IIME_IME_INFO IIME_ldb_bulgarian_435;
    extern const IIME_IME_INFO IIME_ldb_bulgarian_437;

    extern const IIME_IME_INFO IIME_ldb_croatian_510;
    extern const IIME_IME_INFO IIME_ldb_croatian_515;
    extern const IIME_IME_INFO IIME_ldb_croatian_517;

    extern const IIME_IME_INFO IIME_ldb_czech_440;
    extern const IIME_IME_INFO IIME_ldb_czech_442;
    extern const IIME_IME_INFO IIME_ldb_czech_443;
    extern const IIME_IME_INFO IIME_ldb_czech_445;
    extern const IIME_IME_INFO IIME_ldb_czech_447;

    extern const IIME_IME_INFO IIME_ldb_danish_370;
    extern const IIME_IME_INFO IIME_ldb_danish_375;
    extern const IIME_IME_INFO IIME_ldb_danish_377;

    extern const IIME_IME_INFO IIME_ldb_dutch_380;
    extern const IIME_IME_INFO IIME_ldb_dutch_385;
    extern const IIME_IME_INFO IIME_ldb_dutch_387;

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

    extern const IIME_IME_INFO IIME_ldb_finnish_450;
    extern const IIME_IME_INFO IIME_ldb_finnish_455;
    extern const IIME_IME_INFO IIME_ldb_finnish_457;

    extern const IIME_IME_INFO IIME_ldb_french_400;
    extern const IIME_IME_INFO IIME_ldb_french_405;
    extern const IIME_IME_INFO IIME_ldb_french_406;
    extern const IIME_IME_INFO IIME_ldb_french_407;
    extern const IIME_IME_INFO IIME_ldb_french_4071;

    extern const IIME_IME_INFO IIME_ldb_german_300;
    extern const IIME_IME_INFO IIME_ldb_german_305;
    extern const IIME_IME_INFO IIME_ldb_german_307;
    extern const IIME_IME_INFO IIME_ldb_german_308;

    extern const IIME_IME_INFO IIME_ldb_greek_350;
    extern const IIME_IME_INFO IIME_ldb_greek_355;
    extern const IIME_IME_INFO IIME_ldb_greek_357;

    extern const IIME_IME_INFO IIME_ldb_hebrew_420;
    extern const IIME_IME_INFO IIME_ldb_hebrew_425;
    extern const IIME_IME_INFO IIME_ldb_hebrew_427;
    extern const IIME_IME_INFO IIME_ldb_hebrew_4271;

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

    extern const IIME_IME_INFO IIME_ldb_hinglish_910;
    extern const IIME_IME_INFO IIME_ldb_hinglish_915;
    extern const IIME_IME_INFO IIME_ldb_hinglish_917;
    extern const IIME_IME_INFO IIME_ldb_hinglish_9171;

    extern const IIME_IME_INFO IIME_ldb_hungarian_410;
    extern const IIME_IME_INFO IIME_ldb_hungarian_412;
    extern const IIME_IME_INFO IIME_ldb_hungarian_413;
    extern const IIME_IME_INFO IIME_ldb_hungarian_415;
    extern const IIME_IME_INFO IIME_ldb_hungarian_417;

    extern const IIME_IME_INFO IIME_ldb_indonesian_320;
    extern const IIME_IME_INFO IIME_ldb_indonesian_321;
    extern const IIME_IME_INFO IIME_ldb_indonesian_325;
    extern const IIME_IME_INFO IIME_ldb_indonesian_327;
    extern const IIME_IME_INFO IIME_ldb_indonesian_3271;

    extern const IIME_IME_INFO IIME_ldb_italian_700;
    extern const IIME_IME_INFO IIME_ldb_italian_705;
    extern const IIME_IME_INFO IIME_ldb_italian_707;

    extern const IIME_IME_INFO IIME_ldb_lithuanian_560;
    extern const IIME_IME_INFO IIME_ldb_lithuanian_565;
    extern const IIME_IME_INFO IIME_ldb_lithuanian_567;

    extern const IIME_IME_INFO IIME_ldb_malay_340;
    extern const IIME_IME_INFO IIME_ldb_malay_345;
    extern const IIME_IME_INFO IIME_ldb_malay_347;
    extern const IIME_IME_INFO IIME_ldb_malay_3471;

    extern const IIME_IME_INFO IIME_ldb_norwegian_480;
    extern const IIME_IME_INFO IIME_ldb_norwegian_485;
    extern const IIME_IME_INFO IIME_ldb_norwegian_487;

    extern const IIME_IME_INFO IIME_ldb_persian_460;
    extern const IIME_IME_INFO IIME_ldb_persian_462;
    extern const IIME_IME_INFO IIME_ldb_persian_463;
    extern const IIME_IME_INFO IIME_ldb_persian_465;
    extern const IIME_IME_INFO IIME_ldb_persian_467;
    extern const IIME_IME_INFO IIME_ldb_persian_4671;
    extern const IIME_IME_INFO IIME_ldb_persian_468;
    extern const IIME_IME_INFO IIME_ldb_persian_469;
    extern const IIME_IME_INFO IIME_ldb_persian_4600;

    extern const IIME_IME_INFO IIME_ldb_polish_391;
    extern const IIME_IME_INFO IIME_ldb_polish_392;
    extern const IIME_IME_INFO IIME_ldb_polish_393;
    extern const IIME_IME_INFO IIME_ldb_polish_395;
    extern const IIME_IME_INFO IIME_ldb_polish_397;

    extern const IIME_IME_INFO IIME_ldb_portuguese_600;
    extern const IIME_IME_INFO IIME_ldb_portuguese_601;
    extern const IIME_IME_INFO IIME_ldb_portuguese_605;
    extern const IIME_IME_INFO IIME_ldb_portuguese_607;
    extern const IIME_IME_INFO IIME_ldb_portuguese_6071;

    extern const IIME_IME_INFO IIME_ldb_romanian_520;
    extern const IIME_IME_INFO IIME_ldb_romanian_522;
    extern const IIME_IME_INFO IIME_ldb_romanian_523;
    extern const IIME_IME_INFO IIME_ldb_romanian_525;
    extern const IIME_IME_INFO IIME_ldb_romanian_527;

    extern const IIME_IME_INFO IIME_ldb_russian_202;
    extern const IIME_IME_INFO IIME_ldb_russian_205;
    extern const IIME_IME_INFO IIME_ldb_russian_207;
    extern const IIME_IME_INFO IIME_ldb_russian_2071;
    extern const IIME_IME_INFO IIME_ldb_russian_208;

    extern const IIME_IME_INFO IIME_ldb_slovak_470;
    extern const IIME_IME_INFO IIME_ldb_slovak_475;
    extern const IIME_IME_INFO IIME_ldb_slovak_477;

    extern const IIME_IME_INFO IIME_ldb_slovenian_530;
    extern const IIME_IME_INFO IIME_ldb_slovenian_535;
    extern const IIME_IME_INFO IIME_ldb_slovenian_537;

    extern const IIME_IME_INFO IIME_ldb_spanish_500;
    extern const IIME_IME_INFO IIME_ldb_spanish_501;
    extern const IIME_IME_INFO IIME_ldb_spanish_504;
    extern const IIME_IME_INFO IIME_ldb_spanish_505;
    extern const IIME_IME_INFO IIME_ldb_spanish_507;
    extern const IIME_IME_INFO IIME_ldb_spanish_5071;

    extern const IIME_IME_INFO IIME_ldb_swahili_550;
    extern const IIME_IME_INFO IIME_ldb_swahili_555;
    extern const IIME_IME_INFO IIME_ldb_swahili_557;

    extern const IIME_IME_INFO IIME_ldb_swedish_490;
    extern const IIME_IME_INFO IIME_ldb_swedish_495;
    extern const IIME_IME_INFO IIME_ldb_swedish_497;

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
    
    extern const IIME_IME_INFO IIME_ldb_tibetan_730;
	
    extern const IIME_IME_INFO IIME_ldb_turkish_330;
    extern const IIME_IME_INFO IIME_ldb_turkish_335;
    extern const IIME_IME_INFO IIME_ldb_turkish_337;
    extern const IIME_IME_INFO IIME_ldb_turkish_3371;

    extern const IIME_IME_INFO IIME_ldb_ukrainian_540;
    extern const IIME_IME_INFO IIME_ldb_ukrainian_545;
    extern const IIME_IME_INFO IIME_ldb_ukrainian_547;
    extern const IIME_IME_INFO IIME_ldb_ukrainian_548;

    extern const IIME_IME_INFO IIME_ldb_uyghur_940;
    extern const IIME_IME_INFO IIME_ldb_uyghur_941;
    extern const IIME_IME_INFO IIME_ldb_uyghur_947;
    extern const IIME_IME_INFO IIME_ldb_uyghur_948;

    extern const IIME_IME_INFO IIME_ldb_vietnamese_310;
    extern const IIME_IME_INFO IIME_ldb_vietnamese_315;
    extern const IIME_IME_INFO IIME_ldb_vietnamese_317;
    extern const IIME_IME_INFO IIME_ldb_vietnamese_3171;

    extern const IIME_IME_INFO IIME_ldb_urdu_950;
    extern const IIME_IME_INFO IIME_ldb_urdu_955;
    extern const IIME_IME_INFO IIME_ldb_urdu_957;
    extern const IIME_IME_INFO IIME_ldb_urdu_9571;

    extern const IIME_IME_INFO IIME_ldb_tagalog_587;
    extern const IIME_IME_INFO IIME_ldb_kyrgyz_747;

    extern const IIME_IME_INFO IIME_ldb_nepali_720;
	extern const IIME_IME_INFO IIME_ldb_bodo_1100;//daodejing
	extern const IIME_IME_INFO IIME_ldb_dogri_1110;
	extern const IIME_IME_INFO IIME_ldb_sanskrit_1120;
	extern const IIME_IME_INFO IIME_ldb_kashmiri_1130;
	extern const IIME_IME_INFO IIME_ldb_konkani_1140;
	extern const IIME_IME_INFO IIME_ldb_maithili_1150;
	extern const IIME_IME_INFO IIME_ldb_manipuri_1160;
	extern const IIME_IME_INFO IIME_ldb_santali_1170;
	extern const IIME_IME_INFO IIME_ldb_somali_1180;
	extern const IIME_IME_INFO IIME_ldb_sindhi_1190;



	
	extern const void *kmx_ldb_sinhala_770;
    extern const IIME_IME_INFO IIME_ldb_assamese_710;

	extern const IIME_IME_INFO IIME_ldb_sinhala_770;

    extern const IIME_IME_INFO IIME_ldb_albanian_611;
    extern const IIME_IME_INFO IIME_ldb_catalan_621;
    extern const IIME_IME_INFO IIME_ldb_basque_631;
    extern const IIME_IME_INFO IIME_ldb_galician_641;
    extern const IIME_IME_INFO IIME_ldb_igbo_651;
    extern const IIME_IME_INFO IIME_ldb_oromo_661;

    extern const IIME_IME_INFO IIME_ldb_serbian_233;
    extern const IIME_IME_INFO IIME_ldb_serbianc_243;

    extern const IIME_IME_INFO IIME_ldb_zulu_677;
    extern const IIME_IME_INFO IIME_ldb_cebuano_687;

    extern const IIME_IME_INFO IIME_ldb_kazakh_257;
    extern const IIME_IME_INFO IIME_ldb_uzbek_267;
    extern const IIME_IME_INFO IIME_ldb_georgain_277;

	//zhmch chn
	extern const IIME_IME_INFO IIME_ldb_dzpy_gb2312_20k;
	extern const IIME_IME_INFO IIME_ldb_stroke_gb2312_mini;
#endif

#endif /* __KMX_ALL_LDB_H__ */
