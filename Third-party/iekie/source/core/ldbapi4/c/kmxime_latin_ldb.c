
/***************************************************************************
 *                                                                         *
 * Copyright (C) 2000-2007 Beijing Komoxo Inc.                             *
 * All rights reserved.                                                    *
 *                                                                         *
 ***************************************************************************/

//#include "inc/IIMEk2api.h"
#include "IIMEk2api.h"
#include "kmxime_latin.h"

#ifdef IEKIE_IM_LDB_TEST
#include "ldb_all_define.h"
#endif

/****************************************************************************
 *
 * Language database configurations.
 *
 ***************************************************************************/
/* Armenian */
#if 1
#ifdef KMX_LDB_ARMENIAN
    #if KMX_LDB_ARMENIAN == 570
        #include "ldb_armenian_570.h"
        const void *kmx_ldb_armenian = &LDB_570;
        const IIME_IME_INFO IIME_ldb_armenian =
        {
            IM_ARMENIAN,
            &LDB_570
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARMENIAN_570")
        #endif
    #elif KMX_LDB_ARMENIAN == 575
        #include "ldb_armenian_575.h"
        const void *kmx_ldb_armenian = &LDB_575;
        const IIME_IME_INFO IIME_ldb_armenian =
        {
            IM_ARMENIAN,
            &LDB_575
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARMENIAN_575")
        #endif
    #elif KMX_LDB_ARMENIAN == 577
        #include "ldb_armenian_577.h"
        const void *kmx_ldb_armenian = &LDB_577;
        const IIME_IME_INFO IIME_ldb_armenian =
        {
            IM_ARMENIAN,
            &LDB_577
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ARMENIAN_577")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Armenian")
            #endif            
        #else
            #error "Armenian database not defined!"
        #endif

    #endif
#endif

/* Bengali */

#if defined(KMX_LDB_BENGALI)
    #if KMX_LDB_BENGALI == 920
        #include "ldb_bengali_920.h"
        const void* kmx_ldb_bengali = &LDB_920;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_920
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_920")
        #endif
    #elif KMX_LDB_BENGALI == 921
        #include "ldb_bengali_921.h"
        const void* kmx_ldb_bengali = &LDB_921;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_921
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_921")
        #endif        
    #elif KMX_LDB_BENGALI == 922
        #include "ldb_bengali_922.h"
        const void* kmx_ldb_bengali = &LDB_922;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_922
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_922")
        #endif
    #elif KMX_LDB_BENGALI == 923
        #include "ldb_bengali_923.h"
        const void* kmx_ldb_bengali = &LDB_923;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_923
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_923")
        #endif
    #elif KMX_LDB_BENGALI == 924
        #include "ldb_bengali_924.h"
        const void* kmx_ldb_bengali = &LDB_924;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_924
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_924")
        #endif
    #elif KMX_LDB_BENGALI == 925
        #include "ldb_bengali_925.h"
        const void* kmx_ldb_bengali = &LDB_925;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_925
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_925")
        #endif
    #elif KMX_LDB_BENGALI == 927
        #include "ldb_bengali_927.h"
        const void* kmx_ldb_bengali = &LDB_927;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_927
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_927")
        #endif
    #elif KMX_LDB_BENGALI == 928
        #include "ldb_bengali_928.h"
        const void* kmx_ldb_bengali = &LDB_928;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_928
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_928")
        #endif        
    #elif KMX_LDB_BENGALI == 929
        #include "ldb_bengali_929.h"
        const void* kmx_ldb_bengali = &LDB_929;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_929
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_929")
        #endif                    
    #elif KMX_LDB_BENGALI == 9200
        #include "ldb_bengali_9200.h"
        const void* kmx_ldb_bengali = &LDB_9200;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_9200
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_9200")
        #endif
    #elif KMX_LDB_BENGALI == 9201
        #include "indic_mt_ldb.h"
        const void* kmx_ldb_bengali = &dummy_bengali_ldb;
        const IIME_IME_INFO IIME_ldb_bengali= 
        {
            IM_BENGALI,
            &dummy_bengali_ldb
        };
        #ifdef _MSC_VER
                #pragma message("    Using database LDB_BENGALI_9201")
        #endif    
    #elif KMX_LDB_BENGALI == 9202
        #include "ldb_bengali_9202.h"
        const void* kmx_ldb_bengali = &LDB_9202;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_9202
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_9202")
        #endif        
    #elif KMX_LDB_BENGALI == 9203
        #include "ldb_bengali_9203.h"
        const void* kmx_ldb_bengali = &LDB_9203;
        const IIME_IME_INFO IIME_ldb_bengali =
        {
            IM_BENGALI,
            &LDB_9203
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BENGALI_9203")
        #endif        
    #else            
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Bengali")
            #endif            
        #else
            #error "Bengali database not defined!"
        #endif

    #endif
#endif

/* Bulgarian */

#if defined(KMX_LDB_BULGARIAN)
    #if KMX_LDB_BULGARIAN == 430
        #include "ldb_bulgarian_430.h"
        const void* kmx_ldb_bulgarian = &LDB_430;
        const IIME_IME_INFO IIME_ldb_bulgarian =
        {
            IM_BULGARIAN,
            &LDB_430
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BULGARIAN_430")
        #endif
    #elif KMX_LDB_BULGARIAN == 435
        #include "ldb_bulgarian_435.h"
        const void* kmx_ldb_bulgarian = &LDB_435;
        const IIME_IME_INFO IIME_ldb_bulgarian =
        {
            IM_BULGARIAN,
            &LDB_435
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BULGARIAN_435")
        #endif
    #elif KMX_LDB_BULGARIAN == 437
        #include "ldb_bulgarian_437.h"
        const void* kmx_ldb_bulgarian = &LDB_437;
        const IIME_IME_INFO IIME_ldb_bulgarian =
        {
            IM_BULGARIAN,
            &LDB_437
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BULGARIAN_437")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Bulgarian")
            #endif            
        #else
            #error "Bulgarian database not defined!"
        #endif

    #endif
#endif

/* Croatian */

#ifdef KMX_LDB_CROATIAN
    #if KMX_LDB_CROATIAN == 510
        #include "ldb_croatian_510.h"
        const void* kmx_ldb_croatian = &LDB_510;
        const IIME_IME_INFO IIME_ldb_croatian =
        {
            IM_CROATIAN,
            &LDB_510
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CROATIAN_510")
        #endif
    #elif KMX_LDB_CROATIAN == 515
        #include "ldb_croatian_515.h"
        const void* kmx_ldb_croatian = &LDB_515;        
        const IIME_IME_INFO IIME_ldb_croatian =
        {
            IM_CROATIAN,
            &LDB_515
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CROATIAN_515")
        #endif        
    #elif KMX_LDB_CROATIAN == 517
        #include "ldb_croatian_517.h"
        const void* kmx_ldb_croatian = &LDB_517;        
        const IIME_IME_INFO IIME_ldb_croatian =
        {
            IM_CROATIAN,
            &LDB_517
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CROATIAN_517")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Croatian")
            #endif            
        #else
	        #error "Croatian database not defined!"
		#endif
    #endif
#endif

/* Czech */

#if defined(KMX_LDB_CZECH)
    #if KMX_LDB_CZECH == 440
        #include "ldb_czech_440.h"
        const void* kmx_ldb_czech = &LDB_440;
        const IIME_IME_INFO IIME_ldb_czech =
        {
            IM_CZECH,
            &LDB_440
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CZECH_440")
        #endif
    #elif KMX_LDB_CZECH == 442
        #include "ldb_czech_442.h"
        const void* kmx_ldb_czech = &LDB_442;
        const IIME_IME_INFO IIME_ldb_czech =
        {
            IM_CZECH,
            &LDB_442
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CZECH_442")
        #endif
    #elif KMX_LDB_CZECH == 443
        #include "ldb_czech_443.h"
        const void* kmx_ldb_czech = &LDB_443;
        const IIME_IME_INFO IIME_ldb_czech =
        {
            IM_CZECH,
            &LDB_443
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CZECH_443")
        #endif
    #elif KMX_LDB_CZECH == 445
        #include "ldb_czech_445.h"
        const void* kmx_ldb_czech = &LDB_445;
        const IIME_IME_INFO IIME_ldb_czech =
        {
            IM_CZECH,
            &LDB_445
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CZECH_445")
        #endif
    #elif KMX_LDB_CZECH == 447
        #include "ldb_czech_447.h"
        const void* kmx_ldb_czech = &LDB_447;
        const IIME_IME_INFO IIME_ldb_czech =
        {
            IM_CZECH,
            &LDB_447
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CZECH_447")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Czech")
            #endif            
        #else
            #error "Czech database not defined!"
        #endif

    #endif
#endif

/* Danish */

#if defined(KMX_LDB_DANISH)
    #if KMX_LDB_DANISH == 370
        #include "ldb_danish_370.h"
        const void* kmx_ldb_danish = &LDB_370;
        const IIME_IME_INFO IIME_ldb_danish =
        {
            IM_DANISH,
            &LDB_370
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_DANISH_370")
        #endif
    #elif KMX_LDB_DANISH == 375
        #include "ldb_danish_375.h"
        const void* kmx_ldb_danish = &LDB_375;
        const IIME_IME_INFO IIME_ldb_danish =
        {
            IM_DANISH,
            &LDB_375
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_DANISH_375")
        #endif
    #elif KMX_LDB_DANISH == 377
        #include "ldb_danish_377.h"
        const void* kmx_ldb_danish = &LDB_377;
        const IIME_IME_INFO IIME_ldb_danish =
        {
            IM_DANISH,
            &LDB_377
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_DANISH_377")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Danish")
            #endif            
        #else
            #error "Danish database not defined!"
        #endif

    #endif
#endif

/* Dutch */

#if defined(KMX_LDB_DUTCH)
    #if KMX_LDB_DUTCH == 380
        #include "ldb_dutch_380.h"
        const void* kmx_ldb_dutch = &LDB_380;
        const IIME_IME_INFO IIME_ldb_dutch =
        {
            IM_DUTCH,
            &LDB_380
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_DUTCH_380")
        #endif
    #elif KMX_LDB_DUTCH == 385
        #include "ldb_dutch_385.h"
        const void* kmx_ldb_dutch = &LDB_385;
        const IIME_IME_INFO IIME_ldb_dutch =
        {
            IM_DUTCH,
            &LDB_385
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_DUTCH_385")
        #endif
    #elif KMX_LDB_DUTCH == 387
        #include "ldb_dutch_387.h"
        const void* kmx_ldb_dutch = &LDB_387;
        const IIME_IME_INFO IIME_ldb_dutch =
        {
            IM_DUTCH,
            &LDB_387
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_DUTCH_387")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Dutch")
            #endif            
        #else
            #error "Dutch database not defined!"
        #endif

    #endif
#endif

/* English */

#if defined(KMX_LDB_ENGLISH)
	#if KMX_LDB_ENGLISH == 100
	    #include "ldb_english_100.h"
        const void* kmx_ldb_english = &LDB_100;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
            &LDB_100
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_100")
        #endif
    #elif KMX_LDB_ENGLISH == 101
	    #include "ldb_english_101.h"
        const void* kmx_ldb_english = &LDB_101;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
		    &LDB_101
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_101")
        #endif
    #elif KMX_LDB_ENGLISH == 102
	    #include "ldb_english_102.h"
        const void* kmx_ldb_english = &LDB_102;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
		    &LDB_102
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_102")
        #endif
    #elif KMX_LDB_ENGLISH == 103
        #include "ldb_english_103.h"
        const void* kmx_ldb_english = &LDB_103;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
            &LDB_103
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_103")
        #endif
		#elif KMX_LDB_ENGLISH == 104
        #include "ldb_english_104.h"
        const void* kmx_ldb_english = &LDB_104;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
            &LDB_104
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_104")
        #endif        
    #elif KMX_LDB_ENGLISH == 105
	    #include "ldb_english_105.h"
        const void* kmx_ldb_english = &LDB_105;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
		    &LDB_105
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_105")
        #endif
    #elif KMX_LDB_ENGLISH == 106
        #include "ldb_english_106.h"
        const void* kmx_ldb_english = &LDB_106;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
		    &LDB_106
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_106")
        #endif
    #elif KMX_LDB_ENGLISH == 107
        #include "ldb_english_107.h"
        const void* kmx_ldb_english = &LDB_107;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
		    &LDB_107
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_107")
        #endif
    #elif KMX_LDB_ENGLISH == 1071
        #include "ldb_english_1071.h"
        const void* kmx_ldb_english = &LDB_1071;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
		    &LDB_1071
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_1071")
        #endif
    #elif KMX_LDB_ENGLISH == 108
        #include "ldb_english_108.h"
        const void* kmx_ldb_english = &LDB_108;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
		    &LDB_108
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_108")
        #endif
    #elif KMX_LDB_ENGLISH == 109
        #include "ldb_english_109.h"
        const void* kmx_ldb_english = &LDB_109;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
		    &LDB_109
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_109")
        #endif
    #elif KMX_LDB_ENGLISH == 10001
        #include "ldb_english_10001.h"
        const void* kmx_ldb_english = &LDB_10001;
        const IIME_IME_INFO IIME_ldb_english =
        {
            IM_ENGLISH,
		    &LDB_10001
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_10001")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for English")
            #endif            
        #else
            #error "English database not defined!"
        #endif

    #endif
#endif

#if defined(KMX_LDB_ENGLISH_SURETYPE)
    #if KMX_LDB_ENGLISH_SURETYPE == 10001
        #include "ldb_english_10001.h"
        const void* kmx_ldb_english_suretype = &LDB_10001;
        const IIME_IME_INFO IIME_ldb_english_suretype =
        {
            IM_ENGLISH + IM_SURETYPE_BASE,
		    &LDB_10001
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ENGLISH_10001")
        #endif
    #else
        #error "Suretype English database not defined!"
    #endif
#endif

/* Finnish */

#if defined(KMX_LDB_FINNISH)
    #if KMX_LDB_FINNISH == 450
        #include "ldb_finnish_450.h"
        const void* kmx_ldb_finnish = &LDB_450;
        const IIME_IME_INFO IIME_ldb_finnish =
        {
            IM_FINNISH,
            &LDB_450
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_FINNISH_450")
        #endif
    #elif KMX_LDB_FINNISH == 455
        #include "ldb_finnish_455.h"
        const void* kmx_ldb_finnish = &LDB_455;
        const IIME_IME_INFO IIME_ldb_finnish =
        {
            IM_FINNISH,
            &LDB_455
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_FINNISH_455")
        #endif
    #elif KMX_LDB_FINNISH == 457
        #include "ldb_finnish_457.h"
        const void* kmx_ldb_finnish = &LDB_457;
        const IIME_IME_INFO IIME_ldb_finnish =
        {
            IM_FINNISH,
            &LDB_457
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_FINNISH_457")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Finnish")
            #endif            
        #else
            #error "Finnish database not defined!"
        #endif

    #endif
#endif

/* French */

#if defined(KMX_LDB_FRENCH)
    #if KMX_LDB_FRENCH == 400
        #include "ldb_french_400.h"
        const void* kmx_ldb_french = &LDB_400;
        const IIME_IME_INFO IIME_ldb_french =
        {
            IM_FRENCH,
            &LDB_400
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_FRENCH_400")
        #endif
    #elif KMX_LDB_FRENCH == 405
        #include "ldb_french_405.h"
        const void* kmx_ldb_french = &LDB_405;
        const IIME_IME_INFO IIME_ldb_french =
        {
            IM_FRENCH,
            &LDB_405
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_FRENCH_405")
        #endif
    #elif KMX_LDB_FRENCH == 406
        #include "ldb_french_406.h"
        const void* kmx_ldb_french = &LDB_406;
        const IIME_IME_INFO IIME_ldb_french =
        {
            IM_FRENCH,
            &LDB_406
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_FRENCH_406")
        #endif
    #elif KMX_LDB_FRENCH == 407
        #include "ldb_french_407.h"
        const void* kmx_ldb_french = &LDB_407;
        const IIME_IME_INFO IIME_ldb_french =
        {
            IM_FRENCH,
            &LDB_407
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_FRENCH_407")
        #endif
    #elif KMX_LDB_FRENCH == 4001
        #include "ldb_french_4001.h"
        const void* kmx_ldb_french = &LDB_4001;
        const IIME_IME_INFO IIME_ldb_french =
        {
            IM_FRENCH,
            &LDB_4001
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_FRENCH_4001")
        #endif        
    #elif KMX_LDB_FRENCH == 4071
        #include "ldb_french_4071.h"
        const void* kmx_ldb_french = &LDB_4071;
        const IIME_IME_INFO IIME_ldb_french =
        {
            IM_FRENCH,
            &LDB_4071
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_FRENCH_4071")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for French")
            #endif            
        #else
            #error "French database not defined!"
        #endif

    #endif
#endif

/* German */

#if defined(KMX_LDB_GERMAN)
    #if KMX_LDB_GERMAN == 300
        #include "ldb_german_300.h"
        const void* kmx_ldb_german = &LDB_300;
        const IIME_IME_INFO IIME_ldb_german =
        {
            IM_GERMAN,
            &LDB_300
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_GERMAN_300")
        #endif
    #elif KMX_LDB_GERMAN == 305
        #include "ldb_german_305.h"
        const void* kmx_ldb_german = &LDB_305;
        const IIME_IME_INFO IIME_ldb_german =
        {
            IM_GERMAN,
            &LDB_305
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_GERMAN_305")
        #endif
    #elif KMX_LDB_GERMAN == 307
        #include "ldb_german_307.h"
        const void* kmx_ldb_german = &LDB_307;
        const IIME_IME_INFO IIME_ldb_german =
        {
            IM_GERMAN,
            &LDB_307
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_GERMAN_307")
        #endif
    #elif KMX_LDB_GERMAN == 308
        #include "ldb_german_308.h"
        const void* kmx_ldb_german = &LDB_308;
        const IIME_IME_INFO IIME_ldb_german =
        {
            IM_GERMAN,
            &LDB_308
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_GERMAN_308")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for German")
            #endif            
        #else
            #error "German database not defined!"
        #endif

    #endif
#endif

/* Greek */

#if defined(KMX_LDB_GREEK)
    #if KMX_LDB_GREEK == 350
        #include "ldb_greek_350.h"
        const void* kmx_ldb_greek = &LDB_350;
        const IIME_IME_INFO IIME_ldb_greek =
        {
            IM_GREEK,
            &LDB_350
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_GREEK_350")
        #endif
    #elif KMX_LDB_GREEK == 355
        #include "ldb_greek_355.h"
        const void* kmx_ldb_greek = &LDB_355;
        const IIME_IME_INFO IIME_ldb_greek =
        {
            IM_GREEK,
            &LDB_355
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_GREEK_355")
        #endif
    #elif KMX_LDB_GREEK == 357
        #include "ldb_greek_357.h"
        const void* kmx_ldb_greek = &LDB_357;
        const IIME_IME_INFO IIME_ldb_greek =
        {
            IM_GREEK,
            &LDB_357
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_GREEK_357")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Greek")
            #endif            
        #else
            #error "Greek database not defined!"
        #endif

    #endif
#endif

/* Hebrew */

#if defined(KMX_LDB_HEBREW)
    #if KMX_LDB_HEBREW == 420
        #include "ldb_hebrew_420.h"
        const void* kmx_ldb_hebrew = &LDB_420;
        const IIME_IME_INFO IIME_ldb_hebrew =
        {
            IM_HEBREW,
            &LDB_420
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HEBREW_420")
        #endif
    #elif KMX_LDB_HEBREW == 425
        #include "ldb_hebrew_425.h"
        const void* kmx_ldb_hebrew = &LDB_425;
        const IIME_IME_INFO IIME_ldb_hebrew =
        {
            IM_HEBREW,
            &LDB_425
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HEBREW_425")
        #endif
    #elif KMX_LDB_HEBREW == 427
        #include "ldb_hebrew_427.h"
        const void* kmx_ldb_hebrew = &LDB_427;
        const IIME_IME_INFO IIME_ldb_hebrew =
        {
            IM_HEBREW,
            &LDB_427
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HEBREW_427")
        #endif
    #elif KMX_LDB_HEBREW == 4271
        #include "ldb_hebrew_4271.h"
        const void* kmx_ldb_hebrew = &LDB_4271;
        const IIME_IME_INFO IIME_ldb_hebrew =
        {
            IM_HEBREW,
            &LDB_4271
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HEBREW_4271")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Hebrew")
            #endif            
        #else
            #error "Hebrew database not defined!"
        #endif

    #endif
#endif

/* Hungarian */

#if defined(KMX_LDB_HUNGARIAN)
    #if KMX_LDB_HUNGARIAN == 410
        #include "ldb_hungarian_410.h"
        const void* kmx_ldb_hungarian = &LDB_410;
        const IIME_IME_INFO IIME_ldb_hungarian =
        {
            IM_HUNGARIAN,
            &LDB_410
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HUNGARIAN_410")
        #endif
    #elif KMX_LDB_HUNGARIAN == 412
        #include "ldb_hungarian_412.h"
        const void* kmx_ldb_hungarian = &LDB_412;
        const IIME_IME_INFO IIME_ldb_hungarian =
        {
            IM_HUNGARIAN,
            &LDB_412
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HUNGARIAN_412")
        #endif
    #elif KMX_LDB_HUNGARIAN == 413
        #include "ldb_hungarian_413.h"
        const void* kmx_ldb_hungarian = &LDB_413;
        const IIME_IME_INFO IIME_ldb_hungarian =
        {
            IM_HUNGARIAN,
            &LDB_413
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HUNGARIAN_413")
        #endif
    #elif KMX_LDB_HUNGARIAN == 415
        #include "ldb_hungarian_415.h"
        const void* kmx_ldb_hungarian = &LDB_415;
        const IIME_IME_INFO IIME_ldb_hungarian =
        {
            IM_HUNGARIAN,
            &LDB_415
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HUNGARIAN_415")
        #endif
    #elif KMX_LDB_HUNGARIAN == 417
        #include "ldb_hungarian_417.h"
        const void* kmx_ldb_hungarian = &LDB_417;
        const IIME_IME_INFO IIME_ldb_hungarian =
        {
            IM_HUNGARIAN,
            &LDB_417
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_HUNGARIAN_417")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Hungarian")
            #endif            
        #else
            #error "Hungarian database not defined!"
        #endif

    #endif
#endif

/* Indonesian */

#if defined(KMX_LDB_INDONESIAN)
    #if KMX_LDB_INDONESIAN == 320
        #include "ldb_indonesian_320.h"
        const void* kmx_ldb_indonesian = &LDB_320;
        const IIME_IME_INFO IIME_ldb_indonesian =
        {
            IM_INDONESIAN,
            &LDB_320
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_INDONESIAN_320")
        #endif
    #elif KMX_LDB_INDONESIAN == 321
        #include "ldb_indonesian_321.h"
        const void* kmx_ldb_indonesian = &LDB_321;
        const IIME_IME_INFO IIME_ldb_indonesian =
        {
            IM_INDONESIAN,
            &LDB_321
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_INDONESIAN_321")
        #endif
    #elif KMX_LDB_INDONESIAN == 325
        #include "ldb_indonesian_325.h"
        const void* kmx_ldb_indonesian = &LDB_325;
        const IIME_IME_INFO IIME_ldb_indonesian =
        {
            IM_INDONESIAN,
            &LDB_325
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_INDONESIAN_325")
        #endif
    #elif KMX_LDB_INDONESIAN == 327
        #include "ldb_indonesian_327.h"
        const void* kmx_ldb_indonesian = &LDB_327;
        const IIME_IME_INFO IIME_ldb_indonesian =
        {
            IM_INDONESIAN,
            &LDB_327
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_INDONESIAN_327")
        #endif
    #elif KMX_LDB_INDONESIAN == 3271
        #include "ldb_indonesian_3271.h"
        const void* kmx_ldb_indonesian = &LDB_3271;
        const IIME_IME_INFO IIME_ldb_indonesian =
        {
            IM_INDONESIAN,
            &LDB_3271
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_INDONESIAN_3271")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Indonesian")
            #endif            
        #else
            #error "Indonesian database not defined!"
        #endif

    #endif
#endif

/* Italian */

#if defined(KMX_LDB_ITALIAN)
    #if KMX_LDB_ITALIAN == 700
        #include "ldb_italian_700.h"
        const void* kmx_ldb_italian = &LDB_700;
        const IIME_IME_INFO IIME_ldb_italian =
        {
            IM_ITALIAN,
            &LDB_700
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ITALIAN_700")
        #endif
    #elif KMX_LDB_ITALIAN == 705
        #include "ldb_italian_705.h"
        const void* kmx_ldb_italian = &LDB_705;
        const IIME_IME_INFO IIME_ldb_italian =
        {
            IM_ITALIAN,
            &LDB_705
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ITALIAN_705")
        #endif
    #elif KMX_LDB_ITALIAN == 707
        #include "ldb_italian_707.h"
        const void* kmx_ldb_italian = &LDB_707;
        const IIME_IME_INFO IIME_ldb_italian =
        {
            IM_ITALIAN,
            &LDB_707
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ITALIAN_707")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Italian")
            #endif            
        #else
            #error "Italian database not defined!"
        #endif

    #endif
#endif

/* Lithuanian */

#if defined(KMX_LDB_LITHUANIAN)
    #if KMX_LDB_LITHUANIAN == 560
        #include "ldb_lithuanian_560.h"
        const void* kmx_ldb_lithuanian = &LDB_560;
        const IIME_IME_INFO IIME_ldb_lithuanian =
        {
            IM_LITHUANIAN,
            &LDB_560
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_LITHUANIAN_560")
        #endif
    #elif KMX_LDB_LITHUANIAN == 565
        #include "ldb_lithuanian_565.h"
        const void* kmx_ldb_lithuanian = &LDB_565;
        const IIME_IME_INFO IIME_ldb_lithuanian =
        {
            IM_LITHUANIAN,
            &LDB_565
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_LITHUANIAN_565")
        #endif
    #elif KMX_LDB_LITHUANIAN == 567
        #include "ldb_lithuanian_567.h"
        const void* kmx_ldb_lithuanian = &LDB_567;
        const IIME_IME_INFO IIME_ldb_lithuanian =
        {
            IM_LITHUANIAN,
            &LDB_567
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_LITHUANIAN_567")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Lithuanian")
            #endif            
        #else
            #error "Lithuanian database not defined!"
        #endif

    #endif
#endif

/* Malay */

#if defined(KMX_LDB_MALAY)
    #if KMX_LDB_MALAY == 340
        #include "ldb_malay_340.h"
        const void* kmx_ldb_malay = &LDB_340;
        const IIME_IME_INFO IIME_ldb_malay =
        {
            IM_MALAY,
            &LDB_340
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_MALAY_340")
        #endif
    #elif KMX_LDB_MALAY == 345
        #include "ldb_malay_345.h"
        const void* kmx_ldb_malay = &LDB_345;
        const IIME_IME_INFO IIME_ldb_malay =
        {
            IM_MALAY,
            &LDB_345
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_MALAY_345")
        #endif
    #elif KMX_LDB_MALAY == 347
        #include "ldb_malay_347.h"
        const void* kmx_ldb_malay = &LDB_347;
        const IIME_IME_INFO IIME_ldb_malay =
        {
            IM_MALAY,
            &LDB_347
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_MALAY_347")
        #endif
    #elif KMX_LDB_MALAY == 3471
        #include "ldb_malay_3471.h"
        const void* kmx_ldb_malay = &LDB_3471;
        const IIME_IME_INFO IIME_ldb_malay =
        {
            IM_MALAY,
            &LDB_3471
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_MALAY_3471")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Malay")
            #endif            
        #else
            #error "Malay database not defined!"
        #endif

    #endif
#endif

/* Norwegian */

#ifdef KMX_LDB_NORWEGIAN
    #if KMX_LDB_NORWEGIAN == 480
        #include "ldb_norwegian_480.h"
        const void *kmx_ldb_norwegian = &LDB_480;
        const IIME_IME_INFO IIME_ldb_norwegian =
        {
            IM_NORWEGIAN,
            &LDB_480
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_NORWEGIAN_480")
        #endif
    #elif KMX_LDB_NORWEGIAN == 485
        #include "ldb_norwegian_485.h"
        const void *kmx_ldb_norwegian = &LDB_485;
        const IIME_IME_INFO IIME_ldb_norwegian =
        {
            IM_NORWEGIAN,
            &LDB_485
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_NORWEGIAN_485")
        #endif
    #elif KMX_LDB_NORWEGIAN == 487
        #include "ldb_norwegian_487.h"
        const void *kmx_ldb_norwegian = &LDB_487;
        const IIME_IME_INFO IIME_ldb_norwegian =
        {
            IM_NORWEGIAN,
            &LDB_487
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_NORWEGIAN_487")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Norwegian")
            #endif            
        #else
            #error "Norwegian database not defined!"
        #endif

    #endif
#endif

/* Persian */

#if defined(KMX_LDB_PERSIAN)
    #if KMX_LDB_PERSIAN == 460
        #include "ldb_persian_460.h"
        const void* kmx_ldb_persian = &LDB_460;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_460
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_460")
        #endif
    #elif KMX_LDB_PERSIAN == 461
        #include "ldb_persian_461.h"
        const void* kmx_ldb_persian = &LDB_461;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_461
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_461")
        #endif
    #elif KMX_LDB_PERSIAN == 462
        #include "ldb_persian_462.h"
        const void* kmx_ldb_persian = &LDB_462;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_462
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_462")
        #endif
    #elif KMX_LDB_PERSIAN == 463
        #include "ldb_persian_463.h"
        const void* kmx_ldb_persian = &LDB_463;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_463
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_463")
        #endif
    #elif KMX_LDB_PERSIAN == 465
        #include "ldb_persian_465.h"
        const void* kmx_ldb_persian = &LDB_465;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_465
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_465")
        #endif
    #elif KMX_LDB_PERSIAN == 467
        #include "ldb_persian_467.h"
        const void* kmx_ldb_persian = &LDB_467;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_467
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_467")
        #endif
    #elif KMX_LDB_PERSIAN == 4671
        #include "ldb_persian_4671.h"
        const void* kmx_ldb_persian = &LDB_4671;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_4671
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_4671")
        #endif
    #elif KMX_LDB_PERSIAN == 468
        #include "ldb_persian_468.h"
        const void* kmx_ldb_persian = &LDB_468;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_468
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_468")
        #endif
    #elif KMX_LDB_PERSIAN == 469
        #include "ldb_persian_469.h"
        const void* kmx_ldb_persian = &LDB_469;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_469
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_469")
        #endif
    #elif KMX_LDB_PERSIAN == 4600
        #include "ldb_persian_4600.h"
        const void* kmx_ldb_persian = &LDB_4600;
        const IIME_IME_INFO IIME_ldb_persian =
        {
            IM_PERSIAN,
            &LDB_4600
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PERSIAN_4600")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Persian")
            #endif            
        #else
            #error "Persian database not defined!"
        #endif

    #endif
#endif

/* Polish */

#if defined(KMX_LDB_POLISH)
    #if KMX_LDB_POLISH == 391
        #include "ldb_polish_391.h"
        const void* kmx_ldb_polish = &LDB_391;
        const IIME_IME_INFO IIME_ldb_polish =
        {
            IM_POLISH,
            &LDB_391
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_POLISH_391")
        #endif
    #elif KMX_LDB_POLISH == 392
        #include "ldb_polish_392.h"
        const void* kmx_ldb_polish = &LDB_392;
        const IIME_IME_INFO IIME_ldb_polish =
        {
            IM_POLISH,
            &LDB_392
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_POLISH_392")
        #endif
    #elif KMX_LDB_POLISH == 393
        #include "ldb_polish_393.h"
        const void* kmx_ldb_polish = &LDB_393;
        const IIME_IME_INFO IIME_ldb_polish =
        {
            IM_POLISH,
            &LDB_393
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_POLISH_393")
        #endif
    #elif KMX_LDB_POLISH == 395
        #include "ldb_polish_395.h"
        const void* kmx_ldb_polish = &LDB_395;
        const IIME_IME_INFO IIME_ldb_polish =
        {
            IM_POLISH,
            &LDB_395
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_POLISH_395")
        #endif
    #elif KMX_LDB_POLISH == 397
        #include "ldb_polish_397.h"
        const void* kmx_ldb_polish = &LDB_397;
        const IIME_IME_INFO IIME_ldb_polish =
        {
            IM_POLISH,
            &LDB_397
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_POLISH_397")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Polish")
            #endif            
        #else
            #error "Polish database not defined!"
        #endif

    #endif
#endif

/* Portuguese */

#if defined(KMX_LDB_PORTUGUESE)
    #if KMX_LDB_PORTUGUESE == 600
        #include "ldb_portuguese_600.h"
        const void* kmx_ldb_portuguese = &LDB_600;
        const IIME_IME_INFO IIME_ldb_portuguese =
        {
            IM_PORTUGUESE,
            &LDB_600
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PORTUGUESE_600")
        #endif
    #elif KMX_LDB_PORTUGUESE == 601
        #include "ldb_portuguese_601.h"
        const void* kmx_ldb_portuguese = &LDB_601;
        const IIME_IME_INFO IIME_ldb_portuguese =
        {
            IM_PORTUGUESE,
            &LDB_601
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PORTUGUESE_601")
        #endif
    #elif KMX_LDB_PORTUGUESE == 605
        #include "ldb_portuguese_605.h"
        const void* kmx_ldb_portuguese = &LDB_605;
        const IIME_IME_INFO IIME_ldb_portuguese =
        {
            IM_PORTUGUESE,
            &LDB_605
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PORTUGUESE_605")
        #endif
    #elif KMX_LDB_PORTUGUESE == 607
        #include "ldb_portuguese_607.h"
        const void* kmx_ldb_portuguese = &LDB_607;
        const IIME_IME_INFO IIME_ldb_portuguese =
        {
            IM_PORTUGUESE,
            &LDB_607
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PORTUGUESE_607")
        #endif
    #elif KMX_LDB_PORTUGUESE == 6071
        #include "ldb_portuguese_6071.h"
        const void* kmx_ldb_portuguese = &LDB_6071;
        const IIME_IME_INFO IIME_ldb_portuguese =
        {
            IM_PORTUGUESE,
            &LDB_6071
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_PORTUGUESE_6071")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Portuguese")
            #endif            
        #else
            #error "Portuguese database not defined!"
        #endif

    #endif
#endif

/* Romanian */

#if defined(KMX_LDB_ROMANIAN)
    #if KMX_LDB_ROMANIAN == 520
        #include "ldb_romanian_520.h"
        const void* kmx_ldb_romanian = &LDB_520;
        const IIME_IME_INFO IIME_ldb_romanian =
        {
            IM_ROMANIAN,
            &LDB_520
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ROMANIAN_520")
        #endif
    #elif KMX_LDB_ROMANIAN == 522
        #include "ldb_romanian_522.h"
        const void* kmx_ldb_romanian = &LDB_522;
        const IIME_IME_INFO IIME_ldb_romanian =
        {
            IM_ROMANIAN,
            &LDB_522
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ROMANIAN_522")
        #endif
    #elif KMX_LDB_ROMANIAN == 523
        #include "ldb_romanian_523.h"
        const void* kmx_ldb_romanian = &LDB_523;
        const IIME_IME_INFO IIME_ldb_romanian =
        {
            IM_ROMANIAN,
            &LDB_523
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ROMANIAN_523")
        #endif
    #elif KMX_LDB_ROMANIAN == 525
        #include "ldb_romanian_525.h"
        const void* kmx_ldb_romanian = &LDB_525;
        const IIME_IME_INFO IIME_ldb_romanian =
        {
            IM_ROMANIAN,
            &LDB_525
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ROMANIAN_525")
        #endif
    #elif KMX_LDB_ROMANIAN == 527
        #include "ldb_romanian_527.h"
        const void* kmx_ldb_romanian = &LDB_527;
        const IIME_IME_INFO IIME_ldb_romanian =
        {
            IM_ROMANIAN,
            &LDB_527
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ROMANIAN_527")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Romanian")
            #endif            
        #else
            #error "Romanian database not defined!"
        #endif

    #endif
#endif

/* Russian */

#if defined(KMX_LDB_RUSSIAN)
    #if KMX_LDB_RUSSIAN == 202
        #include "ldb_russian_202.h"
        const void* kmx_ldb_russian = &LDB_202;
        const IIME_IME_INFO IIME_ldb_russian =
        {
            IM_RUSSIAN,
            &LDB_202
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_RUSSIAN_202")
        #endif
    #elif KMX_LDB_RUSSIAN == 205
        #include "ldb_russian_205.h"
        const void* kmx_ldb_russian = &LDB_205;
        const IIME_IME_INFO IIME_ldb_russian =
        {
            IM_RUSSIAN,
            &LDB_205
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_RUSSIAN_205")
        #endif
    #elif KMX_LDB_RUSSIAN == 207
        #include "ldb_russian_207.h"
        const void* kmx_ldb_russian = &LDB_207;
        const IIME_IME_INFO IIME_ldb_russian =
        {
            IM_RUSSIAN,
            &LDB_207
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_RUSSIAN_207")
        #endif
    #elif KMX_LDB_RUSSIAN == 2071
        #include "ldb_russian_2071.h"
        const void* kmx_ldb_russian = &LDB_2071;
        const IIME_IME_INFO IIME_ldb_russian =
        {
            IM_RUSSIAN,
            &LDB_2071
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_RUSSIAN_2071")
        #endif
    #elif KMX_LDB_RUSSIAN == 208
        #include "ldb_russian_208.h"
        const void* kmx_ldb_russian = &LDB_208;
        const IIME_IME_INFO IIME_ldb_russian =
        {
            IM_RUSSIAN,
            &LDB_208
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_RUSSIAN_208")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Russian")
            #endif            
        #else
            #error "Russian database not defined!"
        #endif

    #endif
#endif

/* Slovak */

#if defined(KMX_LDB_SLOVAK)
    #if KMX_LDB_SLOVAK == 470
        #include "ldb_slovak_470.h"
        const void* kmx_ldb_slovak = &LDB_470;
        const IIME_IME_INFO IIME_ldb_slovak =
        {
            IM_SLOVAK,
            &LDB_470
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SLOVAK_470")
        #endif
    #elif KMX_LDB_SLOVAK == 475
        #include "ldb_slovak_475.h"
        const void* kmx_ldb_slovak = &LDB_475;
        const IIME_IME_INFO IIME_ldb_slovak =
        {
            IM_SLOVAK,
            &LDB_475
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SLOVAK_475")
        #endif
    #elif KMX_LDB_SLOVAK == 477
        #include "ldb_slovak_477.h"
        const void* kmx_ldb_slovak = &LDB_477;
        const IIME_IME_INFO IIME_ldb_slovak =
        {
            IM_SLOVAK,
            &LDB_477
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SLOVAK_477")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Slovak")
            #endif            
        #else
            #error "Slovak database not defined!"
        #endif

    #endif
#endif

/* Slovenian */

#if defined(KMX_LDB_SLOVENIAN)
    #if KMX_LDB_SLOVENIAN == 530
        #include "ldb_slovenian_530.h"
        const void* kmx_ldb_slovenian = &LDB_530;
        const IIME_IME_INFO IIME_ldb_slovenian =
        {
            IM_SLOVENIAN,
            &LDB_530
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SLOVENIAN_530")
        #endif
    #elif KMX_LDB_SLOVENIAN == 535
        #include "ldb_slovenian_535.h"
        const void* kmx_ldb_slovenian = &LDB_535;
        const IIME_IME_INFO IIME_ldb_slovenian =
        {
            IM_SLOVENIAN,
            &LDB_535
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SLOVENIAN_535")
        #endif
    #elif KMX_LDB_SLOVENIAN == 537
        #include "ldb_slovenian_537.h"
        const void* kmx_ldb_slovenian = &LDB_537;
        const IIME_IME_INFO IIME_ldb_slovenian =
        {
            IM_SLOVENIAN,
            &LDB_537
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SLOVENIAN_537")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Slovenian")
            #endif            
        #else
            #error "Slovenian database not defined!"
        #endif

    #endif
#endif

/* Spanish */

#if defined(KMX_LDB_SPANISH)
    #if KMX_LDB_SPANISH == 500
        #include "ldb_spanish_500.h"
        const void* kmx_ldb_spanish = &LDB_500;
        const IIME_IME_INFO IIME_ldb_spanish =
        {
            IM_SPANISH,
            &LDB_500
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SPANISH_500")
        #endif
    #elif KMX_LDB_SPANISH == 501
        #include "ldb_spanish_501.h"
        const void* kmx_ldb_spanish = &LDB_501;
        const IIME_IME_INFO IIME_ldb_spanish =
        {
            IM_SPANISH,
            &LDB_501
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SPANISH_501")
        #endif
    #elif KMX_LDB_SPANISH == 504
        #include "ldb_spanish_504.h"
        const void* kmx_ldb_spanish = &LDB_504;
        const IIME_IME_INFO IIME_ldb_spanish =
        {
            IM_SPANISH,
            &LDB_504
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SPANISH_504")
        #endif
    #elif KMX_LDB_SPANISH == 505
        #include "ldb_spanish_505.h"
        const void* kmx_ldb_spanish = &LDB_505;
        const IIME_IME_INFO IIME_ldb_spanish =
        {
            IM_SPANISH,
            &LDB_505
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SPANISH_505")
        #endif
    #elif KMX_LDB_SPANISH == 507
        #include "ldb_spanish_507.h"
        const void* kmx_ldb_spanish = &LDB_507;
        const IIME_IME_INFO IIME_ldb_spanish =
        {
            IM_SPANISH,
            &LDB_507
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SPANISH_507")
        #endif
    #elif KMX_LDB_SPANISH == 5071
        #include "ldb_spanish_5071.h"
        const void* kmx_ldb_spanish = &LDB_5071;
        const IIME_IME_INFO IIME_ldb_spanish =
        {
            IM_SPANISH,
            &LDB_5071
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SPANISH_5071")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Spanish")
            #endif            
        #else
            #error "Spanish database not defined!"
        #endif

    #endif
#endif

/* Swahili */

#if defined(KMX_LDB_SWAHILI)
    #if KMX_LDB_SWAHILI == 550
        #include "ldb_swahili_550.h"
        const void* kmx_ldb_swahili = &LDB_550;
        const IIME_IME_INFO IIME_ldb_swahili =
        {
            IM_SWAHILI,
            &LDB_550
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SWAHILI_550")
        #endif
    #elif KMX_LDB_SWAHILI == 555
        #include "ldb_swahili_555.h"
        const void* kmx_ldb_swahili = &LDB_555;
        const IIME_IME_INFO IIME_ldb_swahili =
        {
            IM_SWAHILI,
            &LDB_555
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SWAHILI_555")
        #endif
    #elif KMX_LDB_SWAHILI == 557
        #include "ldb_swahili_557.h"
        const void* kmx_ldb_swahili = &LDB_557;
        const IIME_IME_INFO IIME_ldb_swahili =
        {
            IM_SWAHILI,
            &LDB_557
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SWAHILI_557")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Swahili")
            #endif            
        #else
            #error "Swahili database not defined!"
        #endif

    #endif
#endif

/* Swedish */

#if defined(KMX_LDB_SWEDISH)
    #if KMX_LDB_SWEDISH == 490
        #include "ldb_swedish_490.h"
        const void* kmx_ldb_swedish = &LDB_490;
        const IIME_IME_INFO IIME_ldb_swedish =
        {
            IM_SWEDISH,
            &LDB_490
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SWEDISH_490")
        #endif
    #elif KMX_LDB_SWEDISH == 495
        #include "ldb_swedish_495.h"
        const void* kmx_ldb_swedish = &LDB_495;
        const IIME_IME_INFO IIME_ldb_swedish =
        {
            IM_SWEDISH,
            &LDB_495
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SWEDISH_495")
        #endif
    #elif KMX_LDB_SWEDISH == 497
        #include "ldb_swedish_497.h"
        const void* kmx_ldb_swedish = &LDB_497;
        const IIME_IME_INFO IIME_ldb_swedish =
        {
            IM_SWEDISH,
            &LDB_497
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SWEDISH_497")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Swedish")
            #endif            
        #else
            #error "Swedish database not defined!"
        #endif

    #endif
#endif

/* Thai */

#if defined(KMX_LDB_THAI)
    #if KMX_LDB_THAI == 360
        #include "ldb_thai_360.h"
        const void* kmx_ldb_thai = &LDB_360;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_360
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_360")
        #endif
    #elif KMX_LDB_THAI == 361
        #include "ldb_thai_361.h"
        const void* kmx_ldb_thai = &LDB_361;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_361
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_361")
        #endif
    #elif KMX_LDB_THAI == 362
        #include "ldb_thai_362.h"
        const void* kmx_ldb_thai = &LDB_362;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_362
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_362")
        #endif
    #elif KMX_LDB_THAI == 363
        #include "ldb_thai_363.h"
        const void* kmx_ldb_thai = &LDB_363;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_363
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_363")
        #endif
    #elif KMX_LDB_THAI == 364
        #include "ldb_thai_364.h"
        const void* kmx_ldb_thai = &LDB_364;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_364
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_364")
        #endif
    #elif KMX_LDB_THAI == 365
        #include "ldb_thai_365.h"
        const void* kmx_ldb_thai = &LDB_365;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_365
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_365")
        #endif
    #elif KMX_LDB_THAI == 366
        #include "ldb_thai_366.h"
        const void* kmx_ldb_thai = &LDB_366;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_366
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_366")
        #endif
    #elif KMX_LDB_THAI == 367
        #include "ldb_thai_367.h"
        const void* kmx_ldb_thai = &LDB_367;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_367
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_367")
        #endif
    #elif KMX_LDB_THAI == 3671
        #include "ldb_thai_3671.h"
        const void* kmx_ldb_thai = &LDB_3671;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3671
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3671")
        #endif
    #elif KMX_LDB_THAI == 368
        #include "ldb_thai_368.h"
        const void* kmx_ldb_thai = &LDB_368;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_368
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_368")
        #endif
    #elif KMX_LDB_THAI == 369
        #include "ldb_thai_369.h"
        const void* kmx_ldb_thai = &LDB_369;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_369
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_369")
        #endif
    #elif KMX_LDB_THAI == 3600
        #include "ldb_thai_3600.h"
        const void* kmx_ldb_thai = &LDB_3600;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3600
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3600")
        #endif
    #elif KMX_LDB_THAI == 3601
        #include "ldb_thai_3601.h"
        const void* kmx_ldb_thai = &LDB_3601;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3601
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3601")
        #endif
    #elif KMX_LDB_THAI == 3602
        #include "ldb_thai_3602.h"
        const void* kmx_ldb_thai = &LDB_3602;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3602
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3602")
        #endif
    #elif KMX_LDB_THAI == 3603
        #include "ldb_thai_3603.h"
        const void* kmx_ldb_thai = &LDB_3603;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3603
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3603")
        #endif
    #elif KMX_LDB_THAI == 3604
        #include "ldb_thai_3604.h"
        const void* kmx_ldb_thai = &LDB_3604;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3604
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3604")
        #endif		
    #elif KMX_LDB_THAI == 3640
        #include "ldb_thai_3640.h"
        const void* kmx_ldb_thai = &LDB_3640;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3640
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3640")
        #endif
    #elif KMX_LDB_THAI == 3606
        #include "ldb_thai_3606.h"
        const void* kmx_ldb_thai = &LDB_3606;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3606
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3606")
        #endif
    #elif KMX_LDB_THAI == 3607
        #include "ldb_thai_3607.h"
        const void* kmx_ldb_thai = &LDB_3607;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3607
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3607")
        #endif
    #elif KMX_LDB_THAI == 3608
        #include "ldb_thai_3608.h"
        const void* kmx_ldb_thai = &LDB_3608;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3608
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3608")
        #endif
    #elif KMX_LDB_THAI == 3611
        #include "ldb_thai_3611.h"
        const void* kmx_ldb_thai = &LDB_3611;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3611
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3611")
        #endif
    #elif KMX_LDB_THAI == 3616
        #include "ldb_thai_3616.h"
        const void* kmx_ldb_thai = &LDB_3616;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3616
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3616")
        #endif
    #elif KMX_LDB_THAI == 3632
        #include "ldb_thai_3632.h"
        const void* kmx_ldb_thai = &LDB_3632;
        const IIME_IME_INFO IIME_ldb_thai =
        {
            IM_THAI,
            &LDB_3632
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_THAI_3632")
        #endif		
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Thai")
            #endif            
        #else
            #error "Thai database not defined!"
        #endif

    #endif
#endif

#if defined(KMX_LDB_TIBETAN)
    #if KMX_LDB_TIBETAN == 730
        #include "ldb_tibetan_730.h"
        const void* kmx_ldb_tibetan = &LDB_730;
        const IIME_IME_INFO IIME_ldb_tibetan =
        {
            IM_TIBETAN,
            &LDB_730
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_TIBETAN_730")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Tibetan")
            #endif            
        #else
            #error "Tibetan database not defined!"
        #endif

    #endif
#endif

/* Turkish */

#if defined(KMX_LDB_TURKISH)
    #if KMX_LDB_TURKISH == 330
        #include "ldb_turkish_330.h"
        const void* kmx_ldb_turkish = &LDB_330;
        const IIME_IME_INFO IIME_ldb_turkish =
        {
            IM_TURKISH,
            &LDB_330
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_TURKISH_330")
        #endif
    #elif KMX_LDB_TURKISH == 335
        #include "ldb_turkish_335.h"
        const void* kmx_ldb_turkish = &LDB_335;
        const IIME_IME_INFO IIME_ldb_turkish =
        {
            IM_TURKISH,
            &LDB_335
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_TURKISH_335")
        #endif
    #elif KMX_LDB_TURKISH == 337
        #include "ldb_turkish_337.h"
        const void* kmx_ldb_turkish = &LDB_337;
        const IIME_IME_INFO IIME_ldb_turkish =
        {
            IM_TURKISH,
            &LDB_337
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_TURKISH_337")
        #endif
    #elif KMX_LDB_TURKISH == 3371
        #include "ldb_turkish_3371.h"
        const void* kmx_ldb_turkish = &LDB_3371;
        const IIME_IME_INFO IIME_ldb_turkish =
        {
            IM_TURKISH,
            &LDB_3371
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_TURKISH_3371")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Turkish")
            #endif            
        #else
            #error "Turkish database not defined!"
        #endif

    #endif
#endif

/* Ukrainian */

#if defined(KMX_LDB_UKRAINIAN)
    #if KMX_LDB_UKRAINIAN == 540
        #include "ldb_ukrainian_540.h"
        const void* kmx_ldb_ukrainian = &LDB_540;
        const IIME_IME_INFO IIME_ldb_ukrainian =
        {
            IM_UKRAINIAN,
            &LDB_540
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_UKRAINIAN_540")
        #endif
    #elif KMX_LDB_UKRAINIAN == 545
        #include "ldb_ukrainian_545.h"
        const void* kmx_ldb_ukrainian = &LDB_545;
        const IIME_IME_INFO IIME_ldb_ukrainian =
        {
            IM_UKRAINIAN,
            &LDB_545
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_UKRAINIAN_545")
        #endif
    #elif KMX_LDB_UKRAINIAN == 547
        #include "ldb_ukrainian_547.h"
        const void* kmx_ldb_ukrainian = &LDB_547;
        const IIME_IME_INFO IIME_ldb_ukrainian =
        {
            IM_UKRAINIAN,
            &LDB_547
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_UKRAINIAN_547")
        #endif
    #elif KMX_LDB_UKRAINIAN == 548
        #include "ldb_ukrainian_548.h"
        const void* kmx_ldb_ukrainian = &LDB_548;
        const IIME_IME_INFO IIME_ldb_ukrainian =
        {
            IM_UKRAINIAN,
            &LDB_548
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_UKRAINIAN_548")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Ukrainian")
            #endif            
        #else
            #error "Ukrainian database not defined!"
        #endif

    #endif
#endif

/* Uyghur */

#ifdef KMX_LDB_UYGHUR
    #if KMX_LDB_UYGHUR == 940
        #include "ldb_uyghur_940.h"
        const void* kmx_ldb_uyghur = &LDB_940;
        const IIME_IME_INFO IIME_ldb_uyghur =
        {
            IM_UYGHUR,
            &LDB_940
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_UYGHUR_940")
        #endif
    #elif KMX_LDB_UYGHUR == 941
        #include "ldb_uyghur_941.h"
        const void* kmx_ldb_uyghur = &LDB_941;
        const IIME_IME_INFO IIME_ldb_uyghur =
        {
            IM_UYGHUR,
            &LDB_941
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_UYGHUR_941")
        #endif
    #elif KMX_LDB_UYGHUR == 947
        #include "ldb_uyghur_947.h"
        const void* kmx_ldb_uyghur = &LDB_947;
        const IIME_IME_INFO IIME_ldb_uyghur =
        {
            IM_UYGHUR,
            &LDB_947
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_UYGHUR_947")
        #endif
    #elif KMX_LDB_UYGHUR == 948
        #include "ldb_uyghur_948.h"
        const void* kmx_ldb_uyghur = &LDB_948;
        const IIME_IME_INFO IIME_ldb_uyghur =
        {
            IM_UYGHUR,
            &LDB_948
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_UYGHUR_948")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Uyghur")
            #endif            
        #else
            #error "Uyghur database not defined!"
        #endif

    #endif
#endif

/* Vietnamese */

#if defined(KMX_LDB_VIETNAMESE)
    #if KMX_LDB_VIETNAMESE == 310
        #include "ldb_vietnamese_310.h"
        const void* kmx_ldb_vietnamese = &LDB_310;
        const IIME_IME_INFO IIME_ldb_vietnamese =
        {
            IM_VIETNAMESE,
            &LDB_310
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_VIETNAMESE_310")
        #endif
    #elif KMX_LDB_VIETNAMESE == 315
        #include "ldb_vietnamese_315.h"
        const void* kmx_ldb_vietnamese = &LDB_315;
        const IIME_IME_INFO IIME_ldb_vietnamese =
        {
            IM_VIETNAMESE,
            &LDB_315
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_VIETNAMESE_315")
        #endif
    #elif KMX_LDB_VIETNAMESE == 317
        #include "ldb_vietnamese_317.h"
        const void* kmx_ldb_vietnamese = &LDB_317;
        const IIME_IME_INFO IIME_ldb_vietnamese =
        {
            IM_VIETNAMESE,
            &LDB_317
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_VIETNAMESE_317")
        #endif
    #elif KMX_LDB_VIETNAMESE == 3171
        #include "ldb_vietnamese_3171.h"
        const void* kmx_ldb_vietnamese = &LDB_3171;
        const IIME_IME_INFO IIME_ldb_vietnamese =
        {
            IM_VIETNAMESE,
            &LDB_3171
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_VIETNAMESE_3171")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Vietnamese")
            #endif            
        #else
            #error "Vietnamese database not defined!"
        #endif

    #endif
#endif

/* Hangul */
#if defined(KMX_LDB_HANGUL)
    static int dummy_hangul_db = 0;
    const void* kmx_ldb_hangul = &dummy_hangul_db;
    const IIME_IME_INFO IIME_ldb_hangul = 
    {
        IM_HANGUL,
        &dummy_hangul_db
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_HANGUL")
    #endif    
#endif

/* Urdu */

#ifdef KMX_LDB_URDU
    #if KMX_LDB_URDU == 950
        #include "ldb_urdu_950.h"
        const void* kmx_ldb_urdu = &LDB_950;
        const IIME_IME_INFO IIME_ldb_urdu =
        {
            IM_URDU,
            &LDB_950
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_URDU_950")
        #endif
    #elif KMX_LDB_URDU == 955
        #include "ldb_urdu_955.h"
        const void* kmx_ldb_urdu = &LDB_955;
        const IIME_IME_INFO IIME_ldb_urdu =
        {
            IM_URDU,
            &LDB_955
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_URDU_955")
        #endif
    #elif KMX_LDB_URDU == 957
        #include "ldb_urdu_957.h"
        const void* kmx_ldb_urdu = &LDB_957;
        const IIME_IME_INFO IIME_ldb_urdu =
        {
            IM_URDU,
            &LDB_957
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_URDU_957")
        #endif
    #elif KMX_LDB_URDU == 9571
        #include "ldb_urdu_9571.h"
        const void* kmx_ldb_urdu = &LDB_9571;
        const IIME_IME_INFO IIME_ldb_urdu =
        {
            IM_URDU,
            &LDB_9571
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_URDU_9571")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Urdu")
            #endif            
        #else
            #error "Urdu database not defined!"
        #endif

    #endif
#endif

/* Telugu */
#if defined(KMX_LDB_TELUGU)
    #include "indic_mt_ldb.h"
    const void* kmx_ldb_telugu = &dummy_telugu_ldb;
    const IIME_IME_INFO IIME_ldb_telugu = 
    {
        IM_TELUGU,
        &dummy_telugu_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_TELUGU")
    #endif    
#endif

/* Marathi */
#if defined(KMX_LDB_MARATHI)
    #include "indic_mt_ldb.h"
    const void* kmx_ldb_marathi = &dummy_marathi_ldb;
    const IIME_IME_INFO IIME_ldb_marathi = 
    {
        IM_MARATHI,
        &dummy_marathi_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_MARATHI")
    #endif    
#endif

/* Tamil */
#if defined(KMX_LDB_TAMIL)
    #include "indic_mt_ldb.h"
    const void* kmx_ldb_tamil = &dummy_tamil_ldb;
    const IIME_IME_INFO IIME_ldb_tamil = 
    {
        IM_TAMIL,
        &dummy_tamil_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_TAMIL")
    #endif    
#endif

/* Punjabi */
#if defined(KMX_LDB_PUNJABI)
    #include "indic_mt_ldb.h"
    const void* kmx_ldb_punjabi = &dummy_punjabi_ldb;
    const IIME_IME_INFO IIME_ldb_punjabi = 
    {
        IM_PUNJABI,
        &dummy_punjabi_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_PUNJABI")
    #endif    
#endif

/* Gujarati */
#if defined(KMX_LDB_GUJARATI)
    #include "indic_mt_ldb.h"
    const void* kmx_ldb_gujarati = &dummy_gujarati_ldb;
    const IIME_IME_INFO IIME_ldb_gujarati= 
    {
        IM_GUJARATI,
        &dummy_gujarati_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_GUJARATI")
    #endif    
#endif

/* Oriya */
#if defined(KMX_LDB_ORIYA)
    #include "indic_mt_ldb.h"
    const void* kmx_ldb_oriya = &dummy_oriya_ldb;
    const IIME_IME_INFO IIME_ldb_oriya = 
    {
        IM_ORIYA,
        &dummy_oriya_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_ORIYA")
    #endif    
#endif

/* Kannada */
#if defined(KMX_LDB_KANNADA)
    #include "indic_mt_ldb.h"
    const void* kmx_ldb_kannada = &dummy_kannada_ldb;
    const IIME_IME_INFO IIME_ldb_kannada = 
    {
        IM_KANNADA,
        &dummy_kannada_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_KANNADA")
    #endif    
#endif

/* Malayalam */
#if defined(KMX_LDB_MALAYALAM)
    #include "indic_mt_ldb.h"
    const void* kmx_ldb_malayalam = &dummy_malayalam_ldb;
    const IIME_IME_INFO IIME_ldb_malayalam = 
    {
        IM_MALAYALAM,
        &dummy_malayalam_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_MALAYALAM")
    #endif    
#endif

/* Tagalog */
#if defined(KMX_LDB_TAGALOG)
    #if KMX_LDB_TAGALOG == 587
        #include "ldb_tagalog_587.h"
        const void* kmx_ldb_tagalog = &LDB_587;
        const IIME_IME_INFO IIME_ldb_tagalog =
        {
            IM_TAGALOG,
            &LDB_587
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_TAGALOG_587")
        #endif
    #endif
#endif

/* Kyrgyz */
#if defined(KMX_LDB_KYRGYZ)
    #if KMX_LDB_KYRGYZ == 747
        #include "ldb_kyrgyz_747.h"
        const void* kmx_ldb_kyrgyz = &LDB_747;
        const IIME_IME_INFO IIME_ldb_kyrgyz =
        {
            IM_KYRGYZ,
            &LDB_747
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_KYRGYZ_747")
        #endif
    #endif
#endif

/* Myanmar */
#if defined(KMX_LDB_MYANMAR)
    static int dummy_myanmar_db = 0;
    const void* kmx_ldb_myanmar = &dummy_myanmar_db;
    const IIME_IME_INFO IIME_ldb_myanmar = 
    {
        IM_MYANMAR,
        &dummy_myanmar_db
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_MYANMAR")
    #endif    
#endif

#if defined(KMX_LDB_KHMER)
    #include "indic_mt_ldb.h"
    const void *kmx_ldb_khmer = &dummy_khmer_ldb;
    const IIME_IME_INFO IIME_ldb_khmer = 
    {
        IM_KHMER,
        &dummy_khmer_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_KHMER")
    #endif    
#endif

#if defined(KMX_LDB_LAO)
    #include "indic_mt_ldb.h"
    const void* kmx_ldb_lao = &dummy_lao_ldb;
    const IIME_IME_INFO IIME_ldb_lao = 
    {
        IM_LAO,
        &dummy_lao_ldb
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_LAO")
    #endif    
#endif

#if defined(KMX_LDB_AMHARIC)
    static int dummy_amharic_db = 0;
    const void* kmx_ldb_amharic = &dummy_amharic_db;
    const IIME_IME_INFO IIME_ldb_amharic = 
    {
        IM_AMHARIC,
        &dummy_amharic_db
    };
    #ifdef _MSC_VER
            #pragma message("    Using database LDB_AMHARIC")
    #endif    
#endif

#if defined(KMX_LDB_ASSAMESE)
    #if KMX_LDB_ASSAMESE == 710
        #include "ldb_assamese_710.h"
        const void* kmx_ldb_assamese = &LDB_710;
        const IIME_IME_INFO IIME_ldb_assamese =
        {
            IM_ASSAMESE,
            &LDB_710
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ASSAMESE_710")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Assamese")
            #endif            
        #else
            #error "Assamese database not defined!"
        #endif

    #endif
#endif

//不能同时在kmxime_latin_ldb.c和Kmxime_latin2_ldb.c定义!--ZHMCH
#if 0//defined(KMX_LDB_NEPALI)
    #if KMX_LDB_NEPALI == 720
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_nepali = &LDB_720;
        const IIME_IME_INFO IIME_ldb_nepali =
        {
            IM_NEPALI,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_NEPALI_720")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Nepali")
            #endif            
        #else
            #error "Nepali database not defined!"
        #endif

    #endif
#endif
//不能同时在kmxime_latin_ldb.c和Kmxime_latin2_ldb.c定义!--ZHMCH
#if 0//defined(KMX_LDB_SINHALA)
    #if KMX_LDB_SINHALA == 770
        #include "ldb_sinhala_770.h"
        const void* kmx_ldb_sinhala = &LDB_770;
        const IIME_IME_INFO IIME_ldb_sinhala =
        {
            IM_SINHALA,
            &LDB_770
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SINHALA_770")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Sinhala")
            #endif            
        #else
            #error "Sinhala database not defined!"
        #endif

    #endif
#endif

//daodejing need
//不能同时在kmxime_latin_ldb.c和Kmxime_latin2_ldb.c定义!
#if 0//defined(KMX_LDB_BODO)
    #if KMX_LDB_BODO == 1100
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_bodo = &LDB_720;
        const IIME_IME_INFO IIME_ldb_bodo =
        {
            IM_BODO,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BODO_1100")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Bodo")
            #endif            
        #else
            #error "Bodo database not defined!"
        #endif

    #endif
#endif

#if 0//defined(KMX_LDB_DOGRI)
    #if KMX_LDB_DOGRI == 1110
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_dogri = &LDB_720;
        const IIME_IME_INFO IIME_ldb_dogri =
        {
            IM_DOGRI,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_DOGRI_1110")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Dogri")
            #endif            
        #else
            #error "Dogri database not defined!"
        #endif

    #endif
#endif

#if 0//defined(KMX_LDB_SANSKRIT)
    #if KMX_LDB_SANSKRIT == 1120
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_sanskrit = &LDB_720;
        const IIME_IME_INFO IIME_ldb_sanskrit =
        {
            IM_SANSKRIT,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SANSKRIT_1120")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Sanskrit")
            #endif            
        #else
            #error "Sanskrit database not defined!"
        #endif

    #endif
#endif

#if 0//defined(KMX_LDB_KASHMIRI)
    #if KMX_LDB_KASHMIRI == 1130
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_kashmiri = &LDB_720;
        const IIME_IME_INFO IIME_ldb_kashmiri =
        {
            IM_KASHMIRI,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_KASHMIRI_1130")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Kashmiri")
            #endif            
        #else
            #error "Kashmiri database not defined!"
        #endif

    #endif
#endif

#if 0//defined(KMX_LDB_KONKANI)
    #if KMX_LDB_KONKANI == 1140
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_konkani = &LDB_720;
        const IIME_IME_INFO IIME_ldb_konkani =
        {
            IM_KONKANI,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_KONKANI_1140")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Konkani")
            #endif            
        #else
            #error "Konkani database not defined!"
        #endif

    #endif
#endif

#if 0//defined(KMX_LDB_MAITHILI)
    #if KMX_LDB_MAITHILI == 1150
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_maithili = &LDB_720;
        const IIME_IME_INFO IIME_ldb_maithili =
        {
            IM_MAITHILI,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_MAITHILI_1150")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Maithili")
            #endif            
        #else
            #error "Maithili database not defined!"
        #endif

    #endif
#endif

#if 0//defined(KMX_LDB_MANIPURI)
    #if KMX_LDB_MANIPURI == 1160
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_manipuri = &LDB_720;
        const IIME_IME_INFO IIME_ldb_manipuri =
        {
            IM_MANIPURI,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_MANIPURI_1160")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Manipuri")
            #endif            
        #else
            #error "Manipuri database not defined!"
        #endif

    #endif
#endif

#if 0//defined(KMX_LDB_SANTALI)
    #if KMX_LDB_SANTALI == 1170
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_santali = &LDB_720;
        const IIME_IME_INFO IIME_ldb_santali =
        {
            IM_SANTALI,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SANTALI_1170")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Santali")
            #endif            
        #else
            #error "Santali database not defined!"
        #endif

    #endif
#endif

#if 0//defined(KMX_LDB_SOMALI)
    #if KMX_LDB_SOMALI == 1180
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_somali = &LDB_720;
        const IIME_IME_INFO IIME_ldb_somali =
        {
            IM_SOMALI,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SOMALI_1180")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Somali")
            #endif            
        #else
            #error "Somali database not defined!"
        #endif

    #endif
#endif

#if 0//defined(KMX_LDB_SINDHI)
    #if KMX_LDB_SINDHI == 1190
        #include "ldb_nepali_720.h"
        const void* kmx_ldb_sindhi = &LDB_720;
        const IIME_IME_INFO IIME_ldb_sindhi =
        {
            IM_SINDHI,
            &LDB_720
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SINDHI_1190")
        #endif
    #else
        #ifdef KMX_CFG_DYNAMIC_LDB
            #ifdef _MSC_VER
                #pragma message("    Must use dynamic database for Sindhi")
            #endif            
        #else
            #error "Sindhi database not defined!"
        #endif

    #endif
#endif

#if defined(KMX_LDB_ALBANIAN)
    #if KMX_LDB_ALBANIAN == 611
        #include "ldb_albanian_611.h"
        const void* kmx_ldb_albanian = &LDB_611;
        const IIME_IME_INFO IIME_ldb_albanian =
        {
            IM_ALBANIAN,
            &LDB_611
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ALBANIAN_611")
        #endif
    #else
        #error "Albanian database not defined!"
    #endif
#endif

#if defined(KMX_LDB_CATALAN)
    #if KMX_LDB_CATALAN == 621
        #include "ldb_catalan_621.h"
        const void* kmx_ldb_catalan = &LDB_621;
        const IIME_IME_INFO IIME_ldb_catalan =
        {
            IM_CATALAN,
            &LDB_621
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CATALAN_621")
        #endif
    #else
        #error "Catalan database not defined!"
    #endif
#endif

#if defined(KMX_LDB_BASQUE)
    #if KMX_LDB_BASQUE == 631
        #include "ldb_basque_631.h"
        const void* kmx_ldb_basque = &LDB_631;
        const IIME_IME_INFO IIME_ldb_basque =
        {
            IM_BASQUE,
            &LDB_631
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_BASQUE_631")
        #endif
    #else
        #error "Basque database not defined!"
    #endif
#endif

#if defined(KMX_LDB_GALICIAN)
    #if KMX_LDB_GALICIAN == 641
        #include "ldb_galician_641.h"
        const void* kmx_ldb_galician = &LDB_641;
        const IIME_IME_INFO IIME_ldb_galician =
        {
            IM_GALICIAN,
            &LDB_641
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_GALICIAN_641")
        #endif
    #else
        #error "Galician database not defined!"
    #endif
#endif

#if defined(KMX_LDB_IGBO)
    #if KMX_LDB_IGBO == 651
        #include "ldb_igbo_651.h"
        const void* kmx_ldb_igbo = &LDB_651;
        const IIME_IME_INFO IIME_ldb_igbo =
        {
            IM_IGBO,
            &LDB_651
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_IGBO_651")
        #endif
    #else
        #error "Igbo database not defined!"
    #endif
#endif

#if defined(KMX_LDB_OROMO)
    #if KMX_LDB_OROMO == 661
        #include "ldb_oromo_661.h"
        const void* kmx_ldb_oromo = &LDB_661;
        const IIME_IME_INFO IIME_ldb_oromo =
        {
            IM_OROMO,
            &LDB_661
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_OROMO_661")
        #endif
    #else
        #error "Oromo database not defined!"
    #endif
#endif

#if defined(KMX_LDB_SERBIAN)
    #if KMX_LDB_SERBIAN == 233
        #include "ldb_serbian_233.h"
        const void* kmx_ldb_serbian = &LDB_233;
        const IIME_IME_INFO IIME_ldb_serbian =
        {
            IM_SERBIAN,
            &LDB_233
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SERBIAN_233")
        #endif
    #else
        #error "Serbian database not defined!"
    #endif
#endif

#if defined(KMX_LDB_SERBIANC)
    #if KMX_LDB_SERBIANC == 243
        #include "ldb_serbianc_243.h"
        const void* kmx_ldb_serbianc = &LDB_243;
        const IIME_IME_INFO IIME_ldb_serbianc =
        {
            IM_SERBIAN,
            &LDB_243
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_SERBIANC_243")
        #endif
    #else
        #error "Serbian cyrillic database not defined!"
    #endif
#endif

/* Zulu */
#if defined(KMX_LDB_ZULU)
    #if KMX_LDB_ZULU == 677
        #include "ldb_zulu_677.h"
        const void* kmx_ldb_zulu = &LDB_677;
        const IIME_IME_INFO IIME_ldb_zulu =
        {
            IM_ZULU,
            &LDB_677
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_ZULU_677")
        #endif
    #endif
#endif

/* Cebuano */
#if defined(KMX_LDB_CEBUANO)
    #if KMX_LDB_CEBUANO == 687
        #include "ldb_cebuano_687.h"
        const void* kmx_ldb_cebuano = &LDB_687;
        const IIME_IME_INFO IIME_ldb_cebuano =
        {
            IM_CEBUANO,
            &LDB_687
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_CEBUANO_687")
        #endif
    #endif
#endif

/* Kazakh */
#if defined(KMX_LDB_KAZAKH)
    #if KMX_LDB_KAZAKH == 257
        #include "ldb_kazakh_257.h"
        const void* kmx_ldb_kazakh = &LDB_257;
        const IIME_IME_INFO IIME_ldb_kazakh =
        {
            IM_KAZAKH,
            &LDB_257
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_KAZAKH_257")
        #endif
    #endif
#endif

/* Uzbek */
#if defined(KMX_LDB_UZBEK)
    #if KMX_LDB_UZBEK == 267
        #include "ldb_uzbek_267.h"
        const void* kmx_ldb_uzbek = &LDB_267;
        const IIME_IME_INFO IIME_ldb_uzbek =
        {
            IM_UZBEK,
            &LDB_267
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_UZBEK_267")
        #endif
    #endif
#endif

/* Georgian */
#if defined(KMX_LDB_GEORGIAN)
    #if KMX_LDB_GEORGIAN == 277
        #include "ldb_georgian_277.h"
        const void* kmx_ldb_georgian = &LDB_277;
        const IIME_IME_INFO IIME_ldb_georgian =
        {
            IM_GEORGIAN,
            &LDB_277
        };
        #ifdef _MSC_VER
            #pragma message("    Using database LDB_GEORGIAN_277")
        #endif
    #endif
#endif
#endif
#if defined(IIME_ALL_IN_ONE_DATABASE)
        #include "ldb_armenian_570.h"
        #include "ldb_armenian_575.h"
        #include "ldb_armenian_577.h"

        #include "ldb_bengali_920.h"
        #include "ldb_bengali_921.h"
        #include "ldb_bengali_922.h"        
        #include "ldb_bengali_923.h"
        #include "ldb_bengali_924.h"
        #include "ldb_bengali_925.h"
        #include "ldb_bengali_927.h"
        #include "ldb_bengali_928.h"        
        #include "ldb_bengali_929.h"        
        #include "ldb_bengali_9200.h"                
        #include "ldb_bengali_9202.h"                        
        #include "ldb_bengali_9203.h"                        

        #include "ldb_bulgarian_430.h"
        #include "ldb_bulgarian_435.h"
        #include "ldb_bulgarian_437.h"

        #include "ldb_croatian_510.h"
        #include "ldb_croatian_515.h"
        #include "ldb_croatian_517.h"

        #include "ldb_czech_440.h"
        #include "ldb_czech_442.h"
        #include "ldb_czech_443.h"
        #include "ldb_czech_445.h"
        #include "ldb_czech_447.h"

        #include "ldb_danish_370.h"
        #include "ldb_danish_375.h"
        #include "ldb_danish_377.h"

        #include "ldb_dutch_380.h"
        #include "ldb_dutch_385.h"
        #include "ldb_dutch_387.h"
		#if 1
        #include "ldb_english_100.h"
        #include "ldb_english_101.h"
        #include "ldb_english_102.h"
        #include "ldb_english_103.h"
        #include "ldb_english_104.h"
        #include "ldb_english_105.h"
        #include "ldb_english_106.h"
        #include "ldb_english_107.h"
        #include "ldb_english_1071.h"
        #include "ldb_english_108.h"
        #include "ldb_english_109.h"
        #include "ldb_english_10001.h"
		#else
		#include "ldb_english_103.h"
		#include "ldb_english_107.h"
		#endif
        #include "ldb_finnish_450.h"
        #include "ldb_finnish_455.h"
        #include "ldb_finnish_457.h"

        #include "ldb_french_400.h"
        #include "ldb_french_405.h"
        #include "ldb_french_406.h"
        #include "ldb_french_407.h"
        #include "ldb_french_4071.h"

        #include "ldb_german_300.h"
        #include "ldb_german_305.h"
        #include "ldb_german_307.h"
        #include "ldb_german_308.h"

        #include "ldb_greek_350.h"
        #include "ldb_greek_355.h"
        #include "ldb_greek_357.h"

        #include "ldb_hebrew_420.h"
        #include "ldb_hebrew_425.h"
        #include "ldb_hebrew_427.h"
        #include "ldb_hebrew_4271.h"

        #include "ldb_hungarian_410.h"
        #include "ldb_hungarian_412.h"
        #include "ldb_hungarian_413.h"
        #include "ldb_hungarian_415.h"
        #include "ldb_hungarian_417.h"

        #include "ldb_indonesian_320.h"
        #include "ldb_indonesian_321.h"
        #include "ldb_indonesian_325.h"
        #include "ldb_indonesian_327.h"
        #include "ldb_indonesian_3271.h"

        #include "ldb_italian_700.h"
        #include "ldb_italian_705.h"
        #include "ldb_italian_707.h"

        #include "ldb_lithuanian_560.h"
        #include "ldb_lithuanian_565.h"
        #include "ldb_lithuanian_567.h"

        #include "ldb_malay_340.h"
        #include "ldb_malay_345.h"
        #include "ldb_malay_347.h"
        #include "ldb_malay_3471.h"

        #include "ldb_norwegian_480.h"
        #include "ldb_norwegian_485.h"
        #include "ldb_norwegian_487.h"

        #include "ldb_persian_460.h"
        #include "ldb_persian_462.h"
        #include "ldb_persian_463.h"
        #include "ldb_persian_465.h"
        #include "ldb_persian_467.h"
        #include "ldb_persian_4671.h"
        #include "ldb_persian_468.h"
        #include "ldb_persian_469.h"
        #include "ldb_persian_4600.h"

        #include "ldb_polish_391.h"
        #include "ldb_polish_392.h"
        #include "ldb_polish_393.h"
        #include "ldb_polish_395.h"
        #include "ldb_polish_397.h"

        #include "ldb_portuguese_600.h"
        #include "ldb_portuguese_601.h"
        #include "ldb_portuguese_605.h"
        #include "ldb_portuguese_607.h"
        #include "ldb_portuguese_6071.h"

        #include "ldb_romanian_520.h"
        #include "ldb_romanian_522.h"
        #include "ldb_romanian_523.h"
        #include "ldb_romanian_525.h"
        #include "ldb_romanian_527.h"

        #include "ldb_russian_202.h"
        #include "ldb_russian_205.h"
        #include "ldb_russian_207.h"
        #include "ldb_russian_2071.h"
        #include "ldb_russian_208.h"

        #include "ldb_slovak_470.h"
        #include "ldb_slovak_475.h"
        #include "ldb_slovak_477.h"

        #include "ldb_slovenian_530.h"
        #include "ldb_slovenian_535.h"
        #include "ldb_slovenian_537.h"

        #include "ldb_spanish_500.h"
        #include "ldb_spanish_501.h"
        #include "ldb_spanish_504.h"
        #include "ldb_spanish_505.h"
        #include "ldb_spanish_507.h"
        #include "ldb_spanish_5071.h"

        #include "ldb_swahili_550.h"
        #include "ldb_swahili_555.h"
        #include "ldb_swahili_557.h"

        #include "ldb_swedish_490.h"
        #include "ldb_swedish_495.h"
        #include "ldb_swedish_497.h"

        #include "ldb_thai_360.h"
        #include "ldb_thai_361.h"
        #include "ldb_thai_362.h"
        #include "ldb_thai_363.h"
        #include "ldb_thai_364.h"
        #include "ldb_thai_365.h"
        #include "ldb_thai_366.h"
        #include "ldb_thai_367.h"
        #include "ldb_thai_3671.h"
        #include "ldb_thai_368.h"
        #include "ldb_thai_369.h"
        #include "ldb_thai_3600.h"
        #include "ldb_thai_3601.h"
        #include "ldb_thai_3602.h"
        #include "ldb_thai_3603.h"
        #include "ldb_thai_3604.h"		
        #include "ldb_thai_3606.h"
        #include "ldb_thai_3607.h"
        #include "ldb_thai_3608.h"
        #include "ldb_thai_3611.h"
        #include "ldb_thai_3616.h"
        #include "ldb_thai_3640.h"
        #include "ldb_thai_3632.h"		
        
        #include "ldb_tibetan_730.h"

        #include "ldb_turkish_330.h"
        #include "ldb_turkish_335.h"
        #include "ldb_turkish_337.h"
        #include "ldb_turkish_3371.h"

        #include "ldb_ukrainian_540.h"
        #include "ldb_ukrainian_545.h"
        #include "ldb_ukrainian_547.h"
        #include "ldb_ukrainian_548.h"

        #include "ldb_uyghur_940.h"
        #include "ldb_uyghur_941.h"
        #include "ldb_uyghur_947.h"
        #include "ldb_uyghur_948.h"

        #include "ldb_vietnamese_310.h"
        #include "ldb_vietnamese_315.h"
        #include "ldb_vietnamese_317.h"
        #include "ldb_vietnamese_3171.h"

        #include "ldb_urdu_950.h"
        #include "ldb_urdu_955.h"
        #include "ldb_urdu_957.h"
        #include "ldb_urdu_9571.h"

        #include "ldb_tagalog_587.h"
        #include "ldb_kyrgyz_747.h"

		#include "ldb_hindi_900.h"
        #include "ldb_nepali_720.h"
		#include "ldb_sinhala_770.h"
		//#include "ldb_bodo_1100.h"//daodejing need
		//#include "ldb_bodo_1110.h"
		//#include "ldb_bodo_1120.h"
		//#include "ldb_bodo_1130.h"
		//#include "ldb_bodo_1140.h"
		//#include "ldb_bodo_1150.h"
		//#include "ldb_bodo_1160.h"
		//#include "ldb_bodo_1170.h"
		//#include "ldb_bodo_1180.h"
		//#include "ldb_bodo_1190.h"

        #include "ldb_assamese_710.h"

        #include "ldb_albanian_611.h"
        #include "ldb_catalan_621.h"
        #include "ldb_basque_631.h"
        #include "ldb_galician_641.h"
        #include "ldb_igbo_651.h"
        #include "ldb_oromo_661.h"

        #include "ldb_serbian_233.h"
        #include "ldb_serbianc_243.h"

        #include "ldb_zulu_677.h"
        #include "ldb_cebuano_687.h"

        #include "ldb_kazakh_257.h"
        #include "ldb_uzbek_267.h"
        #include "ldb_georgian_277.h"
		#include "indic_mt_ldb.h"

    const void *kmx_ldb_armenian_570 = &LDB_570;
    const void *kmx_ldb_armenian_575 = &LDB_575;
    const void *kmx_ldb_armenian_577 = &LDB_577;

    const void *kmx_ldb_bengali_920 = &LDB_920;
    const void *kmx_ldb_bengali_921 = &LDB_921;
    const void *kmx_ldb_bengali_922 = &LDB_922;    
    const void *kmx_ldb_bengali_923 = &LDB_923;
    const void *kmx_ldb_bengali_924 = &LDB_924;
    const void *kmx_ldb_bengali_925 = &LDB_925;
    const void *kmx_ldb_bengali_927 = &LDB_927;
    const void *kmx_ldb_bengali_928 = &LDB_928;    
    const void *kmx_ldb_bengali_929 = &LDB_929;    
    const void *kmx_ldb_bengali_9200 = &LDB_9200;        
    const void *kmx_ldb_bengali_9202 = &LDB_9202;            
    const void *kmx_ldb_bengali_9203 = &LDB_9203;            

    const void *kmx_ldb_bulgarian_430 = &LDB_430;
    const void *kmx_ldb_bulgarian_435 = &LDB_435;
    const void *kmx_ldb_bulgarian_437 = &LDB_437;

    const void *kmx_ldb_croatian_510 = &LDB_510;
    const void *kmx_ldb_croatian_515 = &LDB_515;
    const void *kmx_ldb_croatian_517 = &LDB_517;

    const void *kmx_ldb_czech_440 = &LDB_440;
    const void *kmx_ldb_czech_442 = &LDB_442;
    const void *kmx_ldb_czech_443 = &LDB_443;
    const void *kmx_ldb_czech_445 = &LDB_445;
    const void *kmx_ldb_czech_447 = &LDB_447;

    const void *kmx_ldb_danish_370 = &LDB_370;
    const void *kmx_ldb_danish_375 = &LDB_375;
    const void *kmx_ldb_danish_377 = &LDB_377;

    const void *kmx_ldb_dutch_380 = &LDB_380;
    const void *kmx_ldb_dutch_385 = &LDB_385;
    const void *kmx_ldb_dutch_387 = &LDB_387;
	#if 1
    const void *kmx_ldb_english_100 = &LDB_100;
    const void *kmx_ldb_english_101 = &LDB_101;
    const void *kmx_ldb_english_102 = &LDB_102;
    const void *kmx_ldb_english_103 = &LDB_103;
    const void *kmx_ldb_english_104 = &LDB_104;
    const void *kmx_ldb_english_105 = &LDB_105;
    const void *kmx_ldb_english_106 = &LDB_106;
    const void *kmx_ldb_english_107 = &LDB_107;
    const void *kmx_ldb_english_1071 = &LDB_1071;
    const void *kmx_ldb_english_108 = &LDB_108;
    const void *kmx_ldb_english_109 = &LDB_109;
    const void *kmx_ldb_english_10001 = &LDB_10001;
	#else
	const void *kmx_ldb_english_103 = &LDB_103;
	const void *kmx_ldb_english_107 = &LDB_107;
	#endif

    const void *kmx_ldb_finnish_450 = &LDB_450;
    const void *kmx_ldb_finnish_455 = &LDB_455;
    const void *kmx_ldb_finnish_457 = &LDB_457;

    const void *kmx_ldb_french_400 = &LDB_400;
    const void *kmx_ldb_french_405 = &LDB_405;
    const void *kmx_ldb_french_406 = &LDB_406;
    const void *kmx_ldb_french_407 = &LDB_407;
    const void *kmx_ldb_french_4071 = &LDB_4071;

    const void *kmx_ldb_german_300 = &LDB_300;
    const void *kmx_ldb_german_305 = &LDB_305;
    const void *kmx_ldb_german_307 = &LDB_307;
    const void *kmx_ldb_german_308 = &LDB_308;

    const void *kmx_ldb_greek_350 = &LDB_350;
    const void *kmx_ldb_greek_355 = &LDB_355;
    const void *kmx_ldb_greek_357 = &LDB_357;

    const void *kmx_ldb_hebrew_420 = &LDB_420;
    const void *kmx_ldb_hebrew_425 = &LDB_425;
    const void *kmx_ldb_hebrew_427 = &LDB_427;
    const void *kmx_ldb_hebrew_4271 = &LDB_4271;

    const void *kmx_ldb_hungarian_410 = &LDB_410;
    const void *kmx_ldb_hungarian_412 = &LDB_412;
    const void *kmx_ldb_hungarian_413 = &LDB_413;
    const void *kmx_ldb_hungarian_415 = &LDB_415;
    const void *kmx_ldb_hungarian_417 = &LDB_417;

    const void *kmx_ldb_indonesian_320 = &LDB_320;
    const void *kmx_ldb_indonesian_321 = &LDB_321;
    const void *kmx_ldb_indonesian_325 = &LDB_325;
    const void *kmx_ldb_indonesian_327 = &LDB_327;
    const void *kmx_ldb_indonesian_3271 = &LDB_3271;

    const void *kmx_ldb_italian_700 = &LDB_700;
    const void *kmx_ldb_italian_705 = &LDB_705;
    const void *kmx_ldb_italian_707 = &LDB_707;

    const void *kmx_ldb_lithuanian_560 = &LDB_560;
    const void *kmx_ldb_lithuanian_565 = &LDB_565;
    const void *kmx_ldb_lithuanian_567 = &LDB_567;

    const void *kmx_ldb_malay_340 = &LDB_340;
    const void *kmx_ldb_malay_345 = &LDB_345;
    const void *kmx_ldb_malay_347 = &LDB_347;
    const void *kmx_ldb_malay_3471 = &LDB_3471;

    const void *kmx_ldb_norwegian_480 = &LDB_480;
    const void *kmx_ldb_norwegian_485 = &LDB_485;
    const void *kmx_ldb_norwegian_487 = &LDB_487;

    const void *kmx_ldb_persian_460 = &LDB_460;
    const void *kmx_ldb_persian_462 = &LDB_462;
    const void *kmx_ldb_persian_463 = &LDB_463;
    const void *kmx_ldb_persian_465 = &LDB_465;
    const void *kmx_ldb_persian_467 = &LDB_467;
    const void *kmx_ldb_persian_4671= &LDB_4671;
    const void *kmx_ldb_persian_468 = &LDB_468;
    const void *kmx_ldb_persian_469 = &LDB_469;
    const void *kmx_ldb_persian_4600 = &LDB_4600;

    const void *kmx_ldb_polish_391 = &LDB_391;
    const void *kmx_ldb_polish_392 = &LDB_392;
    const void *kmx_ldb_polish_393 = &LDB_393;
    const void *kmx_ldb_polish_395 = &LDB_395;
    const void *kmx_ldb_polish_397 = &LDB_397;

    const void *kmx_ldb_portuguese_600 = &LDB_600;
    const void *kmx_ldb_portuguese_601 = &LDB_601;
    const void *kmx_ldb_portuguese_605 = &LDB_605;
    const void *kmx_ldb_portuguese_607 = &LDB_607;
    const void *kmx_ldb_portuguese_6071 = &LDB_6071;

    const void *kmx_ldb_romanian_520 = &LDB_520;
    const void *kmx_ldb_romanian_522 = &LDB_522;
    const void *kmx_ldb_romanian_523 = &LDB_523;
    const void *kmx_ldb_romanian_525 = &LDB_525;
    const void *kmx_ldb_romanian_527 = &LDB_527;

    const void *kmx_ldb_russian_202 = &LDB_202;
    const void *kmx_ldb_russian_205 = &LDB_205;
    const void *kmx_ldb_russian_207 = &LDB_207;
    const void *kmx_ldb_russian_2071 = &LDB_2071;
    const void *kmx_ldb_russian_208 = &LDB_208;

    const void *kmx_ldb_slovak_470 = &LDB_470;
    const void *kmx_ldb_slovak_475 = &LDB_475;
    const void *kmx_ldb_slovak_477 = &LDB_477;

    const void *kmx_ldb_slovenian_530 = &LDB_530;
    const void *kmx_ldb_slovenian_535 = &LDB_535;
    const void *kmx_ldb_slovenian_537 = &LDB_537;

    const void *kmx_ldb_spanish_500 = &LDB_500;
    const void *kmx_ldb_spanish_501 = &LDB_501;
    const void *kmx_ldb_spanish_504 = &LDB_504;
    const void *kmx_ldb_spanish_505 = &LDB_505;
    const void *kmx_ldb_spanish_507 = &LDB_507;
    const void *kmx_ldb_spanish_5071 = &LDB_5071;

    const void *kmx_ldb_swahili_550 = &LDB_550;
    const void *kmx_ldb_swahili_555 = &LDB_555;
    const void *kmx_ldb_swahili_557 = &LDB_557;

    const void *kmx_ldb_swedish_490 = &LDB_490;
    const void *kmx_ldb_swedish_495 = &LDB_495;
    const void *kmx_ldb_swedish_497 = &LDB_497;

    const void *kmx_ldb_thai_360 = &LDB_360;
    const void *kmx_ldb_thai_361 = &LDB_361;
    const void *kmx_ldb_thai_362 = &LDB_362;
    const void *kmx_ldb_thai_363 = &LDB_363;
    const void *kmx_ldb_thai_364 = &LDB_364;
    const void *kmx_ldb_thai_365 = &LDB_365;
    const void *kmx_ldb_thai_366 = &LDB_366;
    const void *kmx_ldb_thai_367 = &LDB_367;
    const void *kmx_ldb_thai_3671 = &LDB_3671;
    const void *kmx_ldb_thai_368 = &LDB_368;
    const void *kmx_ldb_thai_369 = &LDB_369;
    const void *kmx_ldb_thai_3600 = &LDB_3600;
    const void *kmx_ldb_thai_3601 = &LDB_3601;
    const void *kmx_ldb_thai_3602 = &LDB_3602;
    const void *kmx_ldb_thai_3603 = &LDB_3603;
    const void *kmx_ldb_thai_3604 = &LDB_3604;	
    const void *kmx_ldb_thai_3606 = &LDB_3606;
    const void *kmx_ldb_thai_3607 = &LDB_3607;
    const void *kmx_ldb_thai_3608 = &LDB_3608;
    const void *kmx_ldb_thai_3611 = &LDB_3611;
    const void *kmx_ldb_thai_3616 = &LDB_3616;    
    const void *kmx_ldb_thai_3640 = &LDB_3640;
    const void *kmx_ldb_thai_3632 = &LDB_3632;
	
    const void *kmx_ldb_tibetan_730 = &LDB_730;

    const void *kmx_ldb_turkish_330 = &LDB_330;
    const void *kmx_ldb_turkish_335 = &LDB_335;
    const void *kmx_ldb_turkish_337 = &LDB_337;
    const void *kmx_ldb_turkish_3371 = &LDB_3371;

    const void *kmx_ldb_ukrainian_540 = &LDB_540;
    const void *kmx_ldb_ukrainian_545 = &LDB_545;
    const void *kmx_ldb_ukrainian_547 = &LDB_547;
    const void *kmx_ldb_ukrainian_548 = &LDB_548;

    const void *kmx_ldb_uyghur_940 = &LDB_940;
    const void *kmx_ldb_uyghur_941 = &LDB_941;
    const void *kmx_ldb_uyghur_947 = &LDB_947;
    const void *kmx_ldb_uyghur_948 = &LDB_948;

    const void *kmx_ldb_vietnamese_310 = &LDB_310;
    const void *kmx_ldb_vietnamese_315 = &LDB_315;
    const void *kmx_ldb_vietnamese_317 = &LDB_317;
    const void *kmx_ldb_vietnamese_3171 = &LDB_3171;

    const void *kmx_ldb_urdu_950 = &LDB_950;
    const void *kmx_ldb_urdu_955 = &LDB_955;
    const void *kmx_ldb_urdu_957 = &LDB_957;
    const void *kmx_ldb_urdu_9571 = &LDB_9571;

    const void *kmx_ldb_tagalog_587 = &LDB_587;
    const void *kmx_ldb_kyrgyz_587 = &LDB_747;
    
    const void *kmx_ldb_nepali_720 = &LDB_720;
    const void *kmx_ldb_sinhala_770 = &LDB_770;
    const void *kmx_ldb_bodo_1100 = &LDB_720;//daodejing  need
    const void *kmx_ldb_bodo_1110 = &LDB_720;
	const void *kmx_ldb_bodo_1120 = &LDB_720;
	const void *kmx_ldb_bodo_1130 = &LDB_720;
	const void *kmx_ldb_bodo_1140 = &LDB_720;
	const void *kmx_ldb_bodo_1150 = &LDB_720;
	const void *kmx_ldb_bodo_1160 = &LDB_720;
	const void *kmx_ldb_bodo_1170 = &LDB_720;
	const void *kmx_ldb_bodo_1180 = &LDB_720;
	const void *kmx_ldb_bodo_1190 = &LDB_720;
    const void *kmx_ldb_assamese_710 = &LDB_710;

    const void *kmx_ldb_albanian_611 = &LDB_611;
    const void *kmx_ldb_catalan_621 = &LDB_621;
    const void *kmx_ldb_basque_631 = &LDB_631;
    const void *kmx_ldb_galician_641 = &LDB_641;
    const void *kmx_ldb_igbo_651 = &LDB_651;
    const void *kmx_ldb_oromo_661 = &LDB_661;

    const void *kmx_ldb_serbian_233 = &LDB_233;
    const void *kmx_ldb_serbianc_641 = &LDB_243;

    const void *kmx_ldb_zulu_677 = &LDB_677;
    const void *kmx_ldb_cebuano_687 = &LDB_687;

    const void *kmx_ldb_kazakh_257 = &LDB_257;
    const void *kmx_ldb_uzbek_267 = &LDB_267;
    const void *kmx_ldb_georgian_277 = &LDB_277;

    const IIME_IME_INFO IIME_ldb_armenian_570 = {IM_ARMENIAN, &LDB_570};
    const IIME_IME_INFO IIME_ldb_armenian_575 = {IM_ARMENIAN, &LDB_575};
    const IIME_IME_INFO IIME_ldb_armenian_577 = {IM_ARMENIAN, &LDB_577};

    const IIME_IME_INFO IIME_ldb_bengali_920 = {IM_BENGALI, &LDB_920};
    const IIME_IME_INFO IIME_ldb_bengali_921 = {IM_BENGALI, &LDB_921};    
    const IIME_IME_INFO IIME_ldb_bengali_922 = {IM_BENGALI, &LDB_922};        
    const IIME_IME_INFO IIME_ldb_bengali_923 = {IM_BENGALI, &LDB_923};
    const IIME_IME_INFO IIME_ldb_bengali_924 = {IM_BENGALI, &LDB_924};
    const IIME_IME_INFO IIME_ldb_bengali_925 = {IM_BENGALI, &LDB_925};
    const IIME_IME_INFO IIME_ldb_bengali_927 = {IM_BENGALI, &LDB_927};
    const IIME_IME_INFO IIME_ldb_bengali_928 = {IM_BENGALI, &LDB_928};    
    const IIME_IME_INFO IIME_ldb_bengali_929 = {IM_BENGALI, &LDB_929};        
    const IIME_IME_INFO IIME_ldb_bengali_9200 = {IM_BENGALI, &LDB_9200};            
    const IIME_IME_INFO IIME_ldb_bengali_9201 = {IM_BENGALI, &dummy_bengali_ldb};            
    const IIME_IME_INFO IIME_ldb_bengali_9202 = {IM_BENGALI, &LDB_9202};                
    const IIME_IME_INFO IIME_ldb_bengali_9203 = {IM_BENGALI, &LDB_9203};                

    const IIME_IME_INFO IIME_ldb_bulgarian_430 = {IM_BULGARIAN, &LDB_430};
    const IIME_IME_INFO IIME_ldb_bulgarian_435 = {IM_BULGARIAN, &LDB_435};
    const IIME_IME_INFO IIME_ldb_bulgarian_437 = {IM_BULGARIAN, &LDB_437};

    const IIME_IME_INFO IIME_ldb_croatian_510 = {IM_CROATIAN, &LDB_510};
    const IIME_IME_INFO IIME_ldb_croatian_515 = {IM_CROATIAN, &LDB_515};
    const IIME_IME_INFO IIME_ldb_croatian_517 = {IM_CROATIAN, &LDB_517};

    const IIME_IME_INFO IIME_ldb_czech_440 = {IM_CZECH, &LDB_440};
    const IIME_IME_INFO IIME_ldb_czech_442 = {IM_CZECH, &LDB_442};
    const IIME_IME_INFO IIME_ldb_czech_443 = {IM_CZECH, &LDB_443};
    const IIME_IME_INFO IIME_ldb_czech_445 = {IM_CZECH, &LDB_445};
    const IIME_IME_INFO IIME_ldb_czech_447 = {IM_CZECH, &LDB_447};

    const IIME_IME_INFO IIME_ldb_danish_370 = {IM_DANISH, &LDB_370};
    const IIME_IME_INFO IIME_ldb_danish_375 = {IM_DANISH, &LDB_375};
    const IIME_IME_INFO IIME_ldb_danish_377 = {IM_DANISH, &LDB_377};

    const IIME_IME_INFO IIME_ldb_dutch_380 = {IM_DUTCH, &LDB_380};
    const IIME_IME_INFO IIME_ldb_dutch_385 = {IM_DUTCH, &LDB_385};
    const IIME_IME_INFO IIME_ldb_dutch_387 = {IM_DUTCH, &LDB_387};
	#if 1
    const IIME_IME_INFO IIME_ldb_english_100 = {IM_ENGLISH, &LDB_100};
    const IIME_IME_INFO IIME_ldb_english_101 = {IM_ENGLISH, &LDB_101};
    const IIME_IME_INFO IIME_ldb_english_102 = {IM_ENGLISH, &LDB_102};
    const IIME_IME_INFO IIME_ldb_english_103 = {IM_ENGLISH, &LDB_103};
    const IIME_IME_INFO IIME_ldb_english_104 = {IM_ENGLISH, &LDB_104};
    const IIME_IME_INFO IIME_ldb_english_105 = {IM_ENGLISH, &LDB_105};
    const IIME_IME_INFO IIME_ldb_english_106 = {IM_ENGLISH, &LDB_106};
    const IIME_IME_INFO IIME_ldb_english_107 = {IM_ENGLISH, &LDB_107};

	//zhmch add
	const IIME_IME_INFO IIME_ldb_dzpy_gb2312_20k={IM_ENGLISH, &LDB_107};
	const IIME_IME_INFO IIME_ldb_stroke_gb2312_mini={IM_ENGLISH, &LDB_107};

    const IIME_IME_INFO IIME_ldb_english_1071 = {IM_ENGLISH, &LDB_1071};
    const IIME_IME_INFO IIME_ldb_english_108 = {IM_ENGLISH, &LDB_108};
    const IIME_IME_INFO IIME_ldb_english_109 = {IM_ENGLISH, &LDB_109};
    const IIME_IME_INFO IIME_ldb_english_10001 = {IM_ENGLISH, &LDB_10001};
	#else
	const IIME_IME_INFO IIME_ldb_english_103 = {IM_ENGLISH, &LDB_103};
	const IIME_IME_INFO IIME_ldb_english_107 = {IM_ENGLISH, &LDB_107};
	#endif

    const IIME_IME_INFO IIME_ldb_finnish_450 = {IM_FINNISH, &LDB_450};
    const IIME_IME_INFO IIME_ldb_finnish_455 = {IM_FINNISH, &LDB_455};
    const IIME_IME_INFO IIME_ldb_finnish_457 = {IM_FINNISH, &LDB_457};

    const IIME_IME_INFO IIME_ldb_french_400 = {IM_FRENCH, &LDB_400};
    const IIME_IME_INFO IIME_ldb_french_405 = {IM_FRENCH, &LDB_405};
    const IIME_IME_INFO IIME_ldb_french_406 = {IM_FRENCH, &LDB_406};
    const IIME_IME_INFO IIME_ldb_french_407 = {IM_FRENCH, &LDB_407};
    const IIME_IME_INFO IIME_ldb_french_4071 = {IM_FRENCH, &LDB_4071};

    const IIME_IME_INFO IIME_ldb_german_300 = {IM_GERMAN, &LDB_300};
    const IIME_IME_INFO IIME_ldb_german_305 = {IM_GERMAN, &LDB_305};
    const IIME_IME_INFO IIME_ldb_german_307 = {IM_GERMAN, &LDB_307};
    const IIME_IME_INFO IIME_ldb_german_308 = {IM_GERMAN, &LDB_308};

    const IIME_IME_INFO IIME_ldb_greek_350 = {IM_GREEK, &LDB_350};
    const IIME_IME_INFO IIME_ldb_greek_355 = {IM_GREEK, &LDB_355};
    const IIME_IME_INFO IIME_ldb_greek_357 = {IM_GREEK, &LDB_357};

    const IIME_IME_INFO IIME_ldb_hebrew_420 = {IM_HEBREW, &LDB_420};
    const IIME_IME_INFO IIME_ldb_hebrew_425 = {IM_HEBREW, &LDB_425};
    const IIME_IME_INFO IIME_ldb_hebrew_427 = {IM_HEBREW, &LDB_427};
    const IIME_IME_INFO IIME_ldb_hebrew_4271 = {IM_HEBREW, &LDB_4271};

    const IIME_IME_INFO IIME_ldb_hungarian_410 = {IM_HUNGARIAN, &LDB_410};
    const IIME_IME_INFO IIME_ldb_hungarian_412 = {IM_HUNGARIAN, &LDB_412};
    const IIME_IME_INFO IIME_ldb_hungarian_413 = {IM_HUNGARIAN, &LDB_413};
    const IIME_IME_INFO IIME_ldb_hungarian_415 = {IM_HUNGARIAN, &LDB_415};
    const IIME_IME_INFO IIME_ldb_hungarian_417 = {IM_HUNGARIAN, &LDB_417};

    const IIME_IME_INFO IIME_ldb_indonesian_320 = {IM_INDONESIAN, &LDB_320};
    const IIME_IME_INFO IIME_ldb_indonesian_321 = {IM_INDONESIAN, &LDB_321};
    const IIME_IME_INFO IIME_ldb_indonesian_325 = {IM_INDONESIAN, &LDB_325};
    const IIME_IME_INFO IIME_ldb_indonesian_327 = {IM_INDONESIAN, &LDB_327};

    const IIME_IME_INFO IIME_ldb_italian_700 = {IM_ITALIAN, &LDB_700};
    const IIME_IME_INFO IIME_ldb_italian_705 = {IM_ITALIAN, &LDB_705};
    const IIME_IME_INFO IIME_ldb_italian_707 = {IM_ITALIAN, &LDB_707};

    const IIME_IME_INFO IIME_ldb_lithuanian_560 = {IM_LITHUANIAN, &LDB_560};
    const IIME_IME_INFO IIME_ldb_lithuanian_565 = {IM_LITHUANIAN, &LDB_565};
    const IIME_IME_INFO IIME_ldb_lithuanian_567 = {IM_LITHUANIAN, &LDB_567};

    const IIME_IME_INFO IIME_ldb_malay_340 = {IM_MALAY, &LDB_340};
    const IIME_IME_INFO IIME_ldb_malay_345 = {IM_MALAY, &LDB_345};
    const IIME_IME_INFO IIME_ldb_malay_347 = {IM_MALAY, &LDB_347};

    const IIME_IME_INFO IIME_ldb_norwegian_480 = {IM_NORWEGIAN, &LDB_480};
    const IIME_IME_INFO IIME_ldb_norwegian_485 = {IM_NORWEGIAN, &LDB_485};
    const IIME_IME_INFO IIME_ldb_norwegian_487 = {IM_NORWEGIAN, &LDB_487};

    const IIME_IME_INFO IIME_ldb_persian_460 = {IM_PERSIAN, &LDB_460};
    const IIME_IME_INFO IIME_ldb_persian_462 = {IM_PERSIAN, &LDB_462};
    const IIME_IME_INFO IIME_ldb_persian_463 = {IM_PERSIAN, &LDB_463};
    const IIME_IME_INFO IIME_ldb_persian_465 = {IM_PERSIAN, &LDB_465};
    const IIME_IME_INFO IIME_ldb_persian_467 = {IM_PERSIAN, &LDB_467};
    const IIME_IME_INFO IIME_ldb_persian_4671 = {IM_PERSIAN, &LDB_4671};
    const IIME_IME_INFO IIME_ldb_persian_468 = {IM_PERSIAN, &LDB_468};
    const IIME_IME_INFO IIME_ldb_persian_469 = {IM_PERSIAN, &LDB_469};
    const IIME_IME_INFO IIME_ldb_persian_4600 = {IM_PERSIAN, &LDB_4600};

    const IIME_IME_INFO IIME_ldb_polish_391 = {IM_POLISH, &LDB_391};
    const IIME_IME_INFO IIME_ldb_polish_392 = {IM_POLISH, &LDB_392};
    const IIME_IME_INFO IIME_ldb_polish_393 = {IM_POLISH, &LDB_393};
    const IIME_IME_INFO IIME_ldb_polish_395 = {IM_POLISH, &LDB_395};
    const IIME_IME_INFO IIME_ldb_polish_397 = {IM_POLISH, &LDB_397};

    const IIME_IME_INFO IIME_ldb_portuguese_600 = {IM_PORTUGUESE, &LDB_600};
    const IIME_IME_INFO IIME_ldb_portuguese_601 = {IM_PORTUGUESE, &LDB_601};
    const IIME_IME_INFO IIME_ldb_portuguese_605 = {IM_PORTUGUESE, &LDB_605};
    const IIME_IME_INFO IIME_ldb_portuguese_607 = {IM_PORTUGUESE, &LDB_607};

    const IIME_IME_INFO IIME_ldb_romanian_520 = {IM_ROMANIAN, &LDB_520};
    const IIME_IME_INFO IIME_ldb_romanian_522 = {IM_ROMANIAN, &LDB_522};
    const IIME_IME_INFO IIME_ldb_romanian_523 = {IM_ROMANIAN, &LDB_523};
    const IIME_IME_INFO IIME_ldb_romanian_525 = {IM_ROMANIAN, &LDB_525};
    const IIME_IME_INFO IIME_ldb_romanian_527 = {IM_ROMANIAN, &LDB_527};

    const IIME_IME_INFO IIME_ldb_russian_202 = {IM_RUSSIAN, &LDB_202};
    const IIME_IME_INFO IIME_ldb_russian_205 = {IM_RUSSIAN, &LDB_205};
    const IIME_IME_INFO IIME_ldb_russian_207 = {IM_RUSSIAN, &LDB_207};
    const IIME_IME_INFO IIME_ldb_russian_208 = {IM_RUSSIAN, &LDB_208};

    const IIME_IME_INFO IIME_ldb_slovak_470 = {IM_SLOVAK, &LDB_470};
    const IIME_IME_INFO IIME_ldb_slovak_475 = {IM_SLOVAK, &LDB_475};
    const IIME_IME_INFO IIME_ldb_slovak_477 = {IM_SLOVAK, &LDB_477};

    const IIME_IME_INFO IIME_ldb_slovenian_530 = {IM_SLOVENIAN, &LDB_530};
    const IIME_IME_INFO IIME_ldb_slovenian_535 = {IM_SLOVENIAN, &LDB_535};
    const IIME_IME_INFO IIME_ldb_slovenian_537 = {IM_SLOVENIAN, &LDB_537};

    const IIME_IME_INFO IIME_ldb_spanish_500 = {IM_SPANISH, &LDB_500};
    const IIME_IME_INFO IIME_ldb_spanish_501 = {IM_SPANISH, &LDB_501};
    const IIME_IME_INFO IIME_ldb_spanish_504 = {IM_SPANISH, &LDB_504};
    const IIME_IME_INFO IIME_ldb_spanish_505 = {IM_SPANISH, &LDB_505};
    const IIME_IME_INFO IIME_ldb_spanish_507 = {IM_SPANISH, &LDB_507};

    const IIME_IME_INFO IIME_ldb_swahili_550 = {IM_SWAHILI, &LDB_550};
    const IIME_IME_INFO IIME_ldb_swahili_555 = {IM_SWAHILI, &LDB_555};
    const IIME_IME_INFO IIME_ldb_swahili_557 = {IM_SWAHILI, &LDB_557};

    const IIME_IME_INFO IIME_ldb_swedish_490 = {IM_SWEDISH, &LDB_490};
    const IIME_IME_INFO IIME_ldb_swedish_495 = {IM_SWEDISH, &LDB_495};
    const IIME_IME_INFO IIME_ldb_swedish_497 = {IM_SWEDISH, &LDB_497};

    const IIME_IME_INFO IIME_ldb_thai_360 = {IM_THAI, &LDB_360};
    const IIME_IME_INFO IIME_ldb_thai_361 = {IM_THAI, &LDB_361};
    const IIME_IME_INFO IIME_ldb_thai_362 = {IM_THAI, &LDB_362};
    const IIME_IME_INFO IIME_ldb_thai_363 = {IM_THAI, &LDB_363};
    const IIME_IME_INFO IIME_ldb_thai_364 = {IM_THAI, &LDB_364};
    const IIME_IME_INFO IIME_ldb_thai_365 = {IM_THAI, &LDB_365};
    const IIME_IME_INFO IIME_ldb_thai_366 = {IM_THAI, &LDB_366};
    const IIME_IME_INFO IIME_ldb_thai_367 = {IM_THAI, &LDB_367};
    const IIME_IME_INFO IIME_ldb_thai_3671 = {IM_THAI, &LDB_3671};
    const IIME_IME_INFO IIME_ldb_thai_368 = {IM_THAI, &LDB_368};
    const IIME_IME_INFO IIME_ldb_thai_369 = {IM_THAI, &LDB_369};
    const IIME_IME_INFO IIME_ldb_thai_3600 = {IM_THAI, &LDB_3600};
    const IIME_IME_INFO IIME_ldb_thai_3601 = {IM_THAI, &LDB_3601};
    const IIME_IME_INFO IIME_ldb_thai_3602 = {IM_THAI, &LDB_3602};
    const IIME_IME_INFO IIME_ldb_thai_3603 = {IM_THAI, &LDB_3603};
    const IIME_IME_INFO IIME_ldb_thai_3604 = {IM_THAI, &LDB_3604};	
    const IIME_IME_INFO IIME_ldb_thai_3606 = {IM_THAI, &LDB_3606};
    const IIME_IME_INFO IIME_ldb_thai_3607 = {IM_THAI, &LDB_3607};
    const IIME_IME_INFO IIME_ldb_thai_3608 = {IM_THAI, &LDB_3608};
    const IIME_IME_INFO IIME_ldb_thai_3611 = {IM_THAI, &LDB_3611};
    const IIME_IME_INFO IIME_ldb_thai_3616 = {IM_THAI, &LDB_3616};    
    const IIME_IME_INFO IIME_ldb_thai_3632 = {IM_THAI, &LDB_3632};	
    const IIME_IME_INFO IIME_ldb_thai_3640 = {IM_THAI, &LDB_3640};
    
    const IIME_IME_INFO IIME_ldb_tibetan_730 = {IM_TIBETAN, &LDB_730};    

    const IIME_IME_INFO IIME_ldb_turkish_330 = {IM_TURKISH, &LDB_330};
    const IIME_IME_INFO IIME_ldb_turkish_335 = {IM_TURKISH, &LDB_335};
    const IIME_IME_INFO IIME_ldb_turkish_337 = {IM_TURKISH, &LDB_337};

    const IIME_IME_INFO IIME_ldb_ukrainian_540 = {IM_UKRAINIAN, &LDB_540};
    const IIME_IME_INFO IIME_ldb_ukrainian_545 = {IM_UKRAINIAN, &LDB_545};
    const IIME_IME_INFO IIME_ldb_ukrainian_547 = {IM_UKRAINIAN, &LDB_547};
    const IIME_IME_INFO IIME_ldb_ukrainian_548 = {IM_UKRAINIAN, &LDB_548};

    const IIME_IME_INFO IIME_ldb_uyghur_940 = {IM_UYGHUR, &LDB_940};
    const IIME_IME_INFO IIME_ldb_uyghur_941 = {IM_UYGHUR, &LDB_941};
    const IIME_IME_INFO IIME_ldb_uyghur_947 = {IM_UYGHUR, &LDB_947};
    const IIME_IME_INFO IIME_ldb_uyghur_948 = {IM_UYGHUR, &LDB_948};

    const IIME_IME_INFO IIME_ldb_vietnamese_310 = {IM_VIETNAMESE, &LDB_310};
    const IIME_IME_INFO IIME_ldb_vietnamese_315 = {IM_VIETNAMESE, &LDB_315};
    const IIME_IME_INFO IIME_ldb_vietnamese_317 = {IM_VIETNAMESE, &LDB_317};

    const IIME_IME_INFO IIME_ldb_urdu_950 = {IM_URDU, &LDB_950};
    const IIME_IME_INFO IIME_ldb_urdu_955 = {IM_URDU, &LDB_955};
    const IIME_IME_INFO IIME_ldb_urdu_957 = {IM_URDU, &LDB_957};
    const IIME_IME_INFO IIME_ldb_urdu_9571 = {IM_URDU, &LDB_9571};

    const IIME_IME_INFO IIME_ldb_tagalog_587 = {IM_TAGALOG, &LDB_587};
    const IIME_IME_INFO IIME_ldb_kyrgyz_587 = {IM_KYRGYZ, &LDB_747};

    const IIME_IME_INFO IIME_ldb_nepali_720 = {IM_NEPALI, &LDB_720};
    const IIME_IME_INFO IIME_ldb_sinhala_770 = {IM_SINHALA, &LDB_770};
    const IIME_IME_INFO IIME_ldb_bodo_1100 = {IM_BODO, &LDB_720};//daodejing  need
    const IIME_IME_INFO IIME_ldb_dogri_1110 = {IM_DOGRI, &LDB_720};
	const IIME_IME_INFO IIME_ldb_sanskrit_1120 = {IM_SANSKRIT, &LDB_720};
	const IIME_IME_INFO IIME_ldb_kashmiri_1130 = {IM_KASHMIRI, &LDB_720};
	const IIME_IME_INFO IIME_ldb_konkani_1140 = {IM_KONKANI, &LDB_720};
	const IIME_IME_INFO IIME_ldb_maithili_1150 = {IM_MAITHILI, &LDB_720};
	const IIME_IME_INFO IIME_ldb_manipuri_1160 = {IM_MANIPURI, &LDB_720};
	const IIME_IME_INFO IIME_ldb_santali_1170 = {IM_SANTALI, &LDB_720};
	const IIME_IME_INFO IIME_ldb_somali_1180 = {IM_SOMALI, &LDB_720};
	const IIME_IME_INFO IIME_ldb_sindhi_1190 = {IM_SINDHI, &LDB_720};


	
    const IIME_IME_INFO IIME_ldb_assamese_710 = {IM_ASSAMESE, &LDB_710};

    const IIME_IME_INFO IIME_ldb_albanian_611 = {IM_ALBANIAN, &LDB_611};
    const IIME_IME_INFO IIME_ldb_catalan_621 = {IM_CATALAN, &LDB_621};
    const IIME_IME_INFO IIME_ldb_basque_631 = {IM_BASQUE, &LDB_631};
    const IIME_IME_INFO IIME_ldb_galician_641 = {IM_GALICIAN, &LDB_641};
    const IIME_IME_INFO IIME_ldb_igbo_651 = {IM_IGBO, &LDB_651};
    const IIME_IME_INFO IIME_ldb_oromo_661 = {IM_OROMO, &LDB_661};

    const IIME_IME_INFO IIME_ldb_serbian_233 = {IM_SERBIAN, &LDB_233};
    const IIME_IME_INFO IIME_ldb_serbianc_243 = {IM_SERBIANC, &LDB_243};

    const IIME_IME_INFO IIME_ldb_zulu_677 = {IM_ZULU, &LDB_677};
    const IIME_IME_INFO IIME_ldb_cebuano_687 = {IM_CEBUANO, &LDB_687};

    const IIME_IME_INFO IIME_ldb_kazakh_257 = {IM_KAZAKH, &LDB_257};
    const IIME_IME_INFO IIME_ldb_uzbek_267 = {IM_UZBEK, &LDB_267};
    const IIME_IME_INFO IIME_ldb_georgian_277 = {IM_GEORGIAN, &LDB_277};

/*
    static int dummy_myanmar_db = 0;
    const void* kmx_ldb_myanmar = &dummy_myanmar_db;
    const IIME_IME_INFO IIME_ldb_myanmar = {IM_MYANMAR, &dummy_myanmar_db};

    const void* kmx_ldb_tamil = &dummy_tamil_ldb;
    const IIME_IME_INFO IIME_ldb_tamil = {IM_TAMIL, &dummy_tamil_ldb};
	
	const void *kmx_ldb_khmer = &dummy_khmer_ldb;
    const IIME_IME_INFO IIME_ldb_khmer = {IM_KHMER, &dummy_khmer_ldb};
*/


//上面定义的变量，引用了所有可能用到的ldb
#elif defined(IEKIE_INPUT_SAME_LANGUAGE_ALL_LDB)//下面是平台代码生硬使用的变量，所以直接给出
  #ifdef KMX_LDB_ENGLISH//all english ldb file
  #include "ldb_english_100.h"
  #include "ldb_english_101.h"
  #include "ldb_english_102.h"
  #include "ldb_english_103.h"
  #include "ldb_english_104.h"
  #include "ldb_english_105.h"
  #include "ldb_english_106.h"
  #include "ldb_english_107.h"
  #include "ldb_english_1071.h"
  #include "ldb_english_108.h"
  #include "ldb_english_109.h"
  #include "ldb_english_10001.h"
  const void *kmx_ldb_english_100 = &LDB_100;
  const void *kmx_ldb_english_101 = &LDB_101;
  const void *kmx_ldb_english_102 = &LDB_102;
  const void *kmx_ldb_english_103 = &LDB_103;
  const void *kmx_ldb_english_104 = &LDB_104;
  const void *kmx_ldb_english_105 = &LDB_105;
  const void *kmx_ldb_english_106 = &LDB_106;
  const void *kmx_ldb_english_107 = &LDB_107;
  const void *kmx_ldb_english_1071 = &LDB_1071;
  const void *kmx_ldb_english_108 = &LDB_108;
  const void *kmx_ldb_english_109 = &LDB_109;
  const void *kmx_ldb_english_10001 = &LDB_10001;
  const IIME_IME_INFO IIME_ldb_english_100 = {IM_ENGLISH, &LDB_100};
  const IIME_IME_INFO IIME_ldb_english_101 = {IM_ENGLISH, &LDB_101};
  const IIME_IME_INFO IIME_ldb_english_102 = {IM_ENGLISH, &LDB_102};
  const IIME_IME_INFO IIME_ldb_english_103 = {IM_ENGLISH, &LDB_103};
  const IIME_IME_INFO IIME_ldb_english_104 = {IM_ENGLISH, &LDB_104};
  const IIME_IME_INFO IIME_ldb_english_105 = {IM_ENGLISH, &LDB_105};
  const IIME_IME_INFO IIME_ldb_english_106 = {IM_ENGLISH, &LDB_106};
  const IIME_IME_INFO IIME_ldb_english_107 = {IM_ENGLISH, &LDB_107};
  const IIME_IME_INFO IIME_ldb_english_1071 = {IM_ENGLISH, &LDB_1071};
  const IIME_IME_INFO IIME_ldb_english_108 = {IM_ENGLISH, &LDB_108};
  const IIME_IME_INFO IIME_ldb_english_109 = {IM_ENGLISH, &LDB_109};
  const IIME_IME_INFO IIME_ldb_english_10001 = {IM_ENGLISH, &LDB_10001};
  #endif/*KMX_LDB_ENGLISH*/

  #ifdef KMX_LDB_ARMENIAN
  #include "ldb_armenian_570.h"
  #include "ldb_armenian_575.h"
  #include "ldb_armenian_577.h"
  const void *kmx_ldb_armenian_570 = &LDB_570;
  const void *kmx_ldb_armenian_575 = &LDB_575;
  const void *kmx_ldb_armenian_577 = &LDB_577;
  const IIME_IME_INFO IIME_ldb_armenian_570 = {IM_ARMENIAN, &LDB_570};
  const IIME_IME_INFO IIME_ldb_armenian_575 = {IM_ARMENIAN, &LDB_575};
  const IIME_IME_INFO IIME_ldb_armenian_577 = {IM_ARMENIAN, &LDB_577};
  #endif/*KMX_LDB_ARMENIAN*/
  
  #if defined(KMX_LDB_BENGALI)
  #include "ldb_bengali_920.h"
  #include "ldb_bengali_921.h"
  #include "ldb_bengali_922.h"        
  #include "ldb_bengali_923.h"
  #include "ldb_bengali_924.h"
  #include "ldb_bengali_925.h"
  #include "ldb_bengali_927.h"
  #include "ldb_bengali_928.h"        
  #include "ldb_bengali_929.h"        
  #include "ldb_bengali_9200.h"                
  #include "ldb_bengali_9202.h"                        
  #include "ldb_bengali_9203.h"                        
  const void *kmx_ldb_bengali_920 = &LDB_920;
  const void *kmx_ldb_bengali_921 = &LDB_921;
  const void *kmx_ldb_bengali_922 = &LDB_922;	 
  const void *kmx_ldb_bengali_923 = &LDB_923;
  const void *kmx_ldb_bengali_924 = &LDB_924;
  const void *kmx_ldb_bengali_925 = &LDB_925;
  const void *kmx_ldb_bengali_927 = &LDB_927;
  const void *kmx_ldb_bengali_928 = &LDB_928;	 
  const void *kmx_ldb_bengali_929 = &LDB_929;	 
  const void *kmx_ldb_bengali_9200 = &LDB_9200; 	   
  const void *kmx_ldb_bengali_9202 = &LDB_9202; 		   
  const void *kmx_ldb_bengali_9203 = &LDB_9203;
  const IIME_IME_INFO IIME_ldb_bengali_920 = {IM_BENGALI, &LDB_920};
  const IIME_IME_INFO IIME_ldb_bengali_921 = {IM_BENGALI, &LDB_921};	
  const IIME_IME_INFO IIME_ldb_bengali_922 = {IM_BENGALI, &LDB_922};		
  const IIME_IME_INFO IIME_ldb_bengali_923 = {IM_BENGALI, &LDB_923};
  const IIME_IME_INFO IIME_ldb_bengali_924 = {IM_BENGALI, &LDB_924};
  const IIME_IME_INFO IIME_ldb_bengali_925 = {IM_BENGALI, &LDB_925};
  const IIME_IME_INFO IIME_ldb_bengali_927 = {IM_BENGALI, &LDB_927};
  const IIME_IME_INFO IIME_ldb_bengali_928 = {IM_BENGALI, &LDB_928};	
  const IIME_IME_INFO IIME_ldb_bengali_929 = {IM_BENGALI, &LDB_929};		
  const IIME_IME_INFO IIME_ldb_bengali_9200 = {IM_BENGALI, &LDB_9200};			  
  const IIME_IME_INFO IIME_ldb_bengali_9201 = {IM_BENGALI, &dummy_bengali_ldb}; 		   
  const IIME_IME_INFO IIME_ldb_bengali_9202 = {IM_BENGALI, &LDB_9202};				  
  const IIME_IME_INFO IIME_ldb_bengali_9203 = {IM_BENGALI, &LDB_9203}; 
  #endif/*KMX_LDB_BENGALI*/
  
  #ifdef KMX_LDB_BULGARIAN
  #include "ldb_bulgarian_430.h"
  #include "ldb_bulgarian_435.h"
  #include "ldb_bulgarian_437.h"
  const void *kmx_ldb_bulgarian_430 = &LDB_430;
  const void *kmx_ldb_bulgarian_435 = &LDB_435;
  const void *kmx_ldb_bulgarian_437 = &LDB_437;
  const IIME_IME_INFO IIME_ldb_bulgarian_430 = {IM_BULGARIAN, &LDB_430};
  const IIME_IME_INFO IIME_ldb_bulgarian_435 = {IM_BULGARIAN, &LDB_435};
  const IIME_IME_INFO IIME_ldb_bulgarian_437 = {IM_BULGARIAN, &LDB_437};
  #endif/*KMX_LDB_BULGARIAN*/

  #ifdef KMX_LDB_CROATIAN
  #include "ldb_croatian_510.h"
  #include "ldb_croatian_515.h"
  #include "ldb_croatian_517.h"
  const void *kmx_ldb_croatian_510 = &LDB_510;
  const void *kmx_ldb_croatian_515 = &LDB_515;
  const void *kmx_ldb_croatian_517 = &LDB_517;
  const IIME_IME_INFO IIME_ldb_croatian_510 = {IM_CROATIAN, &LDB_510};
  const IIME_IME_INFO IIME_ldb_croatian_515 = {IM_CROATIAN, &LDB_515};
  const IIME_IME_INFO IIME_ldb_croatian_517 = {IM_CROATIAN, &LDB_517};
  #endif/*KMX_LDB_CROATIAN*/
  
  #ifdef KMX_LDB_CZECH
  #include "ldb_czech_440.h"
  #include "ldb_czech_442.h"
  #include "ldb_czech_443.h"
  #include "ldb_czech_445.h"
  #include "ldb_czech_447.h"
  const void *kmx_ldb_czech_440 = &LDB_440;
  const void *kmx_ldb_czech_442 = &LDB_442;
  const void *kmx_ldb_czech_443 = &LDB_443;
  const void *kmx_ldb_czech_445 = &LDB_445;
  const void *kmx_ldb_czech_447 = &LDB_447;  
  const IIME_IME_INFO IIME_ldb_czech_440 = {IM_CZECH, &LDB_440};
  const IIME_IME_INFO IIME_ldb_czech_442 = {IM_CZECH, &LDB_442};
  const IIME_IME_INFO IIME_ldb_czech_443 = {IM_CZECH, &LDB_443};
  const IIME_IME_INFO IIME_ldb_czech_445 = {IM_CZECH, &LDB_445};
  const IIME_IME_INFO IIME_ldb_czech_447 = {IM_CZECH, &LDB_447};
  #endif
  
  #ifdef KMX_LDB_DANISH
  #include "ldb_danish_370.h"
  #include "ldb_danish_375.h"
  #include "ldb_danish_377.h"
  const void *kmx_ldb_danish_370 = &LDB_370;
  const void *kmx_ldb_danish_375 = &LDB_375;
  const void *kmx_ldb_danish_377 = &LDB_377;
  const IIME_IME_INFO IIME_ldb_danish_370 = {IM_DANISH, &LDB_370};
  const IIME_IME_INFO IIME_ldb_danish_375 = {IM_DANISH, &LDB_375};
  const IIME_IME_INFO IIME_ldb_danish_377 = {IM_DANISH, &LDB_377};
  #endif/*KMX_LDB_DANISH*/
  
  
  #ifdef KMX_LDB_DUTCH
  #include "ldb_dutch_380.h"
  #include "ldb_dutch_385.h"
  #include "ldb_dutch_387.h"
  const void *kmx_ldb_dutch_380 = &LDB_380;
  const void *kmx_ldb_dutch_385 = &LDB_385;
  const void *kmx_ldb_dutch_387 = &LDB_387;
  const IIME_IME_INFO IIME_ldb_dutch_380 = {IM_DUTCH, &LDB_380};
  const IIME_IME_INFO IIME_ldb_dutch_385 = {IM_DUTCH, &LDB_385};
  const IIME_IME_INFO IIME_ldb_dutch_387 = {IM_DUTCH, &LDB_387};
  #endif/*KMX_LDB_DUTCH*/
  
  #ifdef KMX_LDB_FINNISH
  #include "ldb_finnish_450.h"
  #include "ldb_finnish_455.h"
  #include "ldb_finnish_457.h"
  const void *kmx_ldb_finnish_450 = &LDB_450;
  const void *kmx_ldb_finnish_455 = &LDB_455;
  const void *kmx_ldb_finnish_457 = &LDB_457;
  const IIME_IME_INFO IIME_ldb_finnish_450 = {IM_FINNISH, &LDB_450};
  const IIME_IME_INFO IIME_ldb_finnish_455 = {IM_FINNISH, &LDB_455};
  const IIME_IME_INFO IIME_ldb_finnish_457 = {IM_FINNISH, &LDB_457};
  #endif/*KMX_LDB_FINNISH*/

  #ifdef KMX_LDB_FRENCH
  #include "ldb_french_400.h"
  #include "ldb_french_405.h"
  #include "ldb_french_406.h"
  #include "ldb_french_407.h"
  #include "ldb_french_4071.h"
  const void *kmx_ldb_french_400 = &LDB_400;
  const void *kmx_ldb_french_405 = &LDB_405;
  const void *kmx_ldb_french_406 = &LDB_406;
  const void *kmx_ldb_french_407 = &LDB_407;
  const void *kmx_ldb_french_4071 = &LDB_4071;
  const IIME_IME_INFO IIME_ldb_french_400 = {IM_FRENCH, &LDB_400};
  const IIME_IME_INFO IIME_ldb_french_405 = {IM_FRENCH, &LDB_405};
  const IIME_IME_INFO IIME_ldb_french_406 = {IM_FRENCH, &LDB_406};
  const IIME_IME_INFO IIME_ldb_french_407 = {IM_FRENCH, &LDB_407};
  const IIME_IME_INFO IIME_ldb_french_4071 = {IM_FRENCH, &LDB_4071};
  #endif/*KMX_LDB_FRENCH*/

  #ifdef KMX_LDB_GERMAN
  #include "ldb_german_300.h"
  #include "ldb_german_305.h"
  #include "ldb_german_307.h"
  #include "ldb_german_308.h"
  const void *kmx_ldb_german_300 = &LDB_300;
  const void *kmx_ldb_german_305 = &LDB_305;
  const void *kmx_ldb_german_307 = &LDB_307;
  const void *kmx_ldb_german_308 = &LDB_308;
  const IIME_IME_INFO IIME_ldb_german_300 = {IM_GERMAN, &LDB_300};
  const IIME_IME_INFO IIME_ldb_german_305 = {IM_GERMAN, &LDB_305};
  const IIME_IME_INFO IIME_ldb_german_307 = {IM_GERMAN, &LDB_307};
  const IIME_IME_INFO IIME_ldb_german_308 = {IM_GERMAN, &LDB_308};
  #endif/*KMX_LDB_GERMAN*/

  #ifdef KMX_LDB_GREEK
  #include "ldb_greek_350.h"
  #include "ldb_greek_355.h"
  #include "ldb_greek_357.h"
  const void *kmx_ldb_greek_350 = &LDB_350;
  const void *kmx_ldb_greek_355 = &LDB_355;
  const void *kmx_ldb_greek_357 = &LDB_357;
  const IIME_IME_INFO IIME_ldb_greek_350 = {IM_GREEK, &LDB_350};
  const IIME_IME_INFO IIME_ldb_greek_355 = {IM_GREEK, &LDB_355};
  const IIME_IME_INFO IIME_ldb_greek_357 = {IM_GREEK, &LDB_357};
  #endif/*KMX_LDB_GREEK*/

  #ifdef KMX_LDB_HEBREW
  #include "ldb_hebrew_420.h"
  #include "ldb_hebrew_425.h"
  #include "ldb_hebrew_427.h"
  #include "ldb_hebrew_4271.h"
  const void *kmx_ldb_hebrew_420 = &LDB_420;
  const void *kmx_ldb_hebrew_425 = &LDB_425;
  const void *kmx_ldb_hebrew_427 = &LDB_427;
  const void *kmx_ldb_hebrew_4271 = &LDB_4271;
  const IIME_IME_INFO IIME_ldb_hebrew_420 = {IM_HEBREW, &LDB_420};
  const IIME_IME_INFO IIME_ldb_hebrew_425 = {IM_HEBREW, &LDB_425};
  const IIME_IME_INFO IIME_ldb_hebrew_427 = {IM_HEBREW, &LDB_427};
  const IIME_IME_INFO IIME_ldb_hebrew_4271 = {IM_HEBREW, &LDB_4271};
  #endif/*KMX_LDB_HEBREW*/


  #ifdef KMX_LDB_HUNGARIAN
  #include "ldb_hungarian_410.h"
  #include "ldb_hungarian_412.h"
  #include "ldb_hungarian_413.h"
  #include "ldb_hungarian_415.h"
  #include "ldb_hungarian_417.h"
  const void *kmx_ldb_hungarian_410 = &LDB_410;
  const void *kmx_ldb_hungarian_412 = &LDB_412;
  const void *kmx_ldb_hungarian_413 = &LDB_413;
  const void *kmx_ldb_hungarian_415 = &LDB_415;
  const void *kmx_ldb_hungarian_417 = &LDB_417;
  const IIME_IME_INFO IIME_ldb_hungarian_410 = {IM_HUNGARIAN, &LDB_410};
  const IIME_IME_INFO IIME_ldb_hungarian_412 = {IM_HUNGARIAN, &LDB_412};
  const IIME_IME_INFO IIME_ldb_hungarian_413 = {IM_HUNGARIAN, &LDB_413};
  const IIME_IME_INFO IIME_ldb_hungarian_415 = {IM_HUNGARIAN, &LDB_415};
  const IIME_IME_INFO IIME_ldb_hungarian_417 = {IM_HUNGARIAN, &LDB_417};
  #endif/*KMX_LDB_HUNGARIAN*/


  #ifdef KMX_LDB_INDONESIAN
  #include "ldb_indonesian_320.h"
  #include "ldb_indonesian_321.h"
  #include "ldb_indonesian_325.h"
  #include "ldb_indonesian_327.h"
  #include "ldb_indonesian_3271.h"
  const void *kmx_ldb_indonesian_320 = &LDB_320;
  const void *kmx_ldb_indonesian_321 = &LDB_321;
  const void *kmx_ldb_indonesian_325 = &LDB_325;
  const void *kmx_ldb_indonesian_327 = &LDB_327;
  const void *kmx_ldb_indonesian_3271 = &LDB_3271;  
  const IIME_IME_INFO IIME_ldb_indonesian_320 = {IM_INDONESIAN, &LDB_320};
  const IIME_IME_INFO IIME_ldb_indonesian_321 = {IM_INDONESIAN, &LDB_321};
  const IIME_IME_INFO IIME_ldb_indonesian_325 = {IM_INDONESIAN, &LDB_325};
  const IIME_IME_INFO IIME_ldb_indonesian_327 = {IM_INDONESIAN, &LDB_327};
  #endif/*KMX_LDB_INDONESIAN*/

  #ifdef KMX_LDB_ITALIAN
  #include "ldb_italian_700.h"
  #include "ldb_italian_705.h"
  #include "ldb_italian_707.h"
  const void *kmx_ldb_italian_700 = &LDB_700;
  const void *kmx_ldb_italian_705 = &LDB_705;
  const void *kmx_ldb_italian_707 = &LDB_707;
  const IIME_IME_INFO IIME_ldb_italian_700 = {IM_ITALIAN, &LDB_700};
  const IIME_IME_INFO IIME_ldb_italian_705 = {IM_ITALIAN, &LDB_705};
  const IIME_IME_INFO IIME_ldb_italian_707 = {IM_ITALIAN, &LDB_707};
  #endif/*KMX_LDB_ITALIAN*/

  #ifdef KMX_LDB_LITHUANIAN
  #include "ldb_lithuanian_560.h"
  #include "ldb_lithuanian_565.h"
  #include "ldb_lithuanian_567.h"
  const void *kmx_ldb_lithuanian_560 = &LDB_560;
  const void *kmx_ldb_lithuanian_565 = &LDB_565;
  const void *kmx_ldb_lithuanian_567 = &LDB_567;
  const IIME_IME_INFO IIME_ldb_lithuanian_560 = {IM_LITHUANIAN, &LDB_560};
  const IIME_IME_INFO IIME_ldb_lithuanian_565 = {IM_LITHUANIAN, &LDB_565};
  const IIME_IME_INFO IIME_ldb_lithuanian_567 = {IM_LITHUANIAN, &LDB_567};
  #endif/*KMX_LDB_LITHUANIAN*/

  #ifdef KMX_LDB_MALAY
  #include "ldb_malay_340.h"
  #include "ldb_malay_345.h"
  #include "ldb_malay_347.h"
  #include "ldb_malay_3471.h"
  const void *kmx_ldb_malay_340 = &LDB_340;
  const void *kmx_ldb_malay_345 = &LDB_345;
  const void *kmx_ldb_malay_347 = &LDB_347;
  const void *kmx_ldb_malay_3471 = &LDB_3471;
  const IIME_IME_INFO IIME_ldb_malay_340 = {IM_MALAY, &LDB_340};
  const IIME_IME_INFO IIME_ldb_malay_345 = {IM_MALAY, &LDB_345};
  const IIME_IME_INFO IIME_ldb_malay_347 = {IM_MALAY, &LDB_347};
  #endif/*KMX_LDB_MALAY*/

  #ifdef KMX_LDB_NORWEGIAN
  #include "ldb_norwegian_480.h"
  #include "ldb_norwegian_485.h"
  #include "ldb_norwegian_487.h"
  const void *kmx_ldb_norwegian_480 = &LDB_480;
  const void *kmx_ldb_norwegian_485 = &LDB_485;
  const void *kmx_ldb_norwegian_487 = &LDB_487;
  const IIME_IME_INFO IIME_ldb_norwegian_480 = {IM_NORWEGIAN, &LDB_480};
  const IIME_IME_INFO IIME_ldb_norwegian_485 = {IM_NORWEGIAN, &LDB_485};
  const IIME_IME_INFO IIME_ldb_norwegian_487 = {IM_NORWEGIAN, &LDB_487};
  #endif/*KMX_LDB_NORWEGIAN*/

  #ifdef KMX_LDB_PERSIAN
  #include "ldb_persian_460.h"
  #include "ldb_persian_462.h"
  #include "ldb_persian_463.h"
  #include "ldb_persian_465.h"
  #include "ldb_persian_467.h"
  #include "ldb_persian_4671.h"
  #include "ldb_persian_468.h"
  #include "ldb_persian_469.h"
  #include "ldb_persian_4600.h"
  const void *kmx_ldb_persian_460 = &LDB_460;
  const void *kmx_ldb_persian_462 = &LDB_462;
  const void *kmx_ldb_persian_463 = &LDB_463;
  const void *kmx_ldb_persian_465 = &LDB_465;
  const void *kmx_ldb_persian_467 = &LDB_467;
  const void *kmx_ldb_persian_4671= &LDB_4671;
  const void *kmx_ldb_persian_468 = &LDB_468;
  const void *kmx_ldb_persian_469 = &LDB_469;
  const void *kmx_ldb_persian_4600 = &LDB_4600;
  const IIME_IME_INFO IIME_ldb_persian_460 = {IM_PERSIAN, &LDB_460};
  const IIME_IME_INFO IIME_ldb_persian_462 = {IM_PERSIAN, &LDB_462};
  const IIME_IME_INFO IIME_ldb_persian_463 = {IM_PERSIAN, &LDB_463};
  const IIME_IME_INFO IIME_ldb_persian_465 = {IM_PERSIAN, &LDB_465};
  const IIME_IME_INFO IIME_ldb_persian_467 = {IM_PERSIAN, &LDB_467};
  const IIME_IME_INFO IIME_ldb_persian_4671 = {IM_PERSIAN, &LDB_4671};
  const IIME_IME_INFO IIME_ldb_persian_468 = {IM_PERSIAN, &LDB_468};
  const IIME_IME_INFO IIME_ldb_persian_469 = {IM_PERSIAN, &LDB_469};
  const IIME_IME_INFO IIME_ldb_persian_4600 = {IM_PERSIAN, &LDB_4600};
  #endif/*KMX_LDB_PERSIAN*/

  #ifdef KMX_LDB_POLISH
  #include "ldb_polish_391.h"
  #include "ldb_polish_392.h"
  #include "ldb_polish_393.h"
  #include "ldb_polish_395.h"
  #include "ldb_polish_397.h"
  const void *kmx_ldb_polish_391 = &LDB_391;
  const void *kmx_ldb_polish_392 = &LDB_392;
  const void *kmx_ldb_polish_393 = &LDB_393;
  const void *kmx_ldb_polish_395 = &LDB_395;
  const void *kmx_ldb_polish_397 = &LDB_397;
  const IIME_IME_INFO IIME_ldb_polish_391 = {IM_POLISH, &LDB_391};
  const IIME_IME_INFO IIME_ldb_polish_392 = {IM_POLISH, &LDB_392};
  const IIME_IME_INFO IIME_ldb_polish_393 = {IM_POLISH, &LDB_393};
  const IIME_IME_INFO IIME_ldb_polish_395 = {IM_POLISH, &LDB_395};
  const IIME_IME_INFO IIME_ldb_polish_397 = {IM_POLISH, &LDB_397};
  #endif/*KMX_LDB_POLISH*/

  #ifdef KMX_LDB_PORTUGUESE
  #include "ldb_portuguese_600.h"
  #include "ldb_portuguese_601.h"
  #include "ldb_portuguese_605.h"
  #include "ldb_portuguese_607.h"
  #include "ldb_portuguese_6071.h"
  const void *kmx_ldb_portuguese_600 = &LDB_600;
  const void *kmx_ldb_portuguese_601 = &LDB_601;
  const void *kmx_ldb_portuguese_605 = &LDB_605;
  const void *kmx_ldb_portuguese_607 = &LDB_607;
  const void *kmx_ldb_portuguese_6071 = &LDB_6071;
  const IIME_IME_INFO IIME_ldb_portuguese_600 = {IM_PORTUGUESE, &LDB_600};
  const IIME_IME_INFO IIME_ldb_portuguese_601 = {IM_PORTUGUESE, &LDB_601};
  const IIME_IME_INFO IIME_ldb_portuguese_605 = {IM_PORTUGUESE, &LDB_605};
  const IIME_IME_INFO IIME_ldb_portuguese_607 = {IM_PORTUGUESE, &LDB_607};
  #endif/*KMX_LDB_PORTUGUESE*/

  #ifdef KMX_LDB_ROMANIAN	  
  #include "ldb_romanian_520.h"
  #include "ldb_romanian_522.h"
  #include "ldb_romanian_523.h"
  #include "ldb_romanian_525.h"
  #include "ldb_romanian_527.h"
  const void *kmx_ldb_romanian_520 = &LDB_520;
  const void *kmx_ldb_romanian_522 = &LDB_522;
  const void *kmx_ldb_romanian_523 = &LDB_523;
  const void *kmx_ldb_romanian_525 = &LDB_525;
  const void *kmx_ldb_romanian_527 = &LDB_527;
  
  const IIME_IME_INFO IIME_ldb_romanian_520 = {IM_ROMANIAN, &LDB_520};
  const IIME_IME_INFO IIME_ldb_romanian_522 = {IM_ROMANIAN, &LDB_522};
  const IIME_IME_INFO IIME_ldb_romanian_523 = {IM_ROMANIAN, &LDB_523};
  const IIME_IME_INFO IIME_ldb_romanian_525 = {IM_ROMANIAN, &LDB_525};
  const IIME_IME_INFO IIME_ldb_romanian_527 = {IM_ROMANIAN, &LDB_527};
  #endif/*KMX_LDB_ROMANIAN*/

  #ifdef KMX_LDB_RUSSIAN
  #include "ldb_russian_202.h"
  #include "ldb_russian_205.h"
  #include "ldb_russian_207.h"
  #include "ldb_russian_2071.h"
  #include "ldb_russian_208.h"
  const void *kmx_ldb_russian_202 = &LDB_202;
  const void *kmx_ldb_russian_205 = &LDB_205;
  const void *kmx_ldb_russian_207 = &LDB_207;
  const void *kmx_ldb_russian_2071 = &LDB_2071;
  const void *kmx_ldb_russian_208 = &LDB_208;
  const IIME_IME_INFO IIME_ldb_russian_202 = {IM_RUSSIAN, &LDB_202};
  const IIME_IME_INFO IIME_ldb_russian_205 = {IM_RUSSIAN, &LDB_205};
  const IIME_IME_INFO IIME_ldb_russian_207 = {IM_RUSSIAN, &LDB_207};
  const IIME_IME_INFO IIME_ldb_russian_208 = {IM_RUSSIAN, &LDB_208};
  #endif/*KMX_LDB_RUSSIAN*/

  #ifdef KMX_LDB_SLOVAK
  #include "ldb_slovak_470.h"
  #include "ldb_slovak_475.h"
  #include "ldb_slovak_477.h"
  const void *kmx_ldb_slovak_470 = &LDB_470;
  const void *kmx_ldb_slovak_475 = &LDB_475;
  const void *kmx_ldb_slovak_477 = &LDB_477;
  const IIME_IME_INFO IIME_ldb_slovak_470 = {IM_SLOVAK, &LDB_470};
  const IIME_IME_INFO IIME_ldb_slovak_475 = {IM_SLOVAK, &LDB_475};
  const IIME_IME_INFO IIME_ldb_slovak_477 = {IM_SLOVAK, &LDB_477};
  #endif/*KMX_LDB_SLOVAK*/

  #ifdef KMX_LDB_SLOVENIAN
  #include "ldb_slovenian_530.h"
  #include "ldb_slovenian_535.h"
  #include "ldb_slovenian_537.h"
  const void *kmx_ldb_slovenian_530 = &LDB_530;
  const void *kmx_ldb_slovenian_535 = &LDB_535;
  const void *kmx_ldb_slovenian_537 = &LDB_537;
  const IIME_IME_INFO IIME_ldb_slovenian_530 = {IM_SLOVENIAN, &LDB_530};
  const IIME_IME_INFO IIME_ldb_slovenian_535 = {IM_SLOVENIAN, &LDB_535};
  const IIME_IME_INFO IIME_ldb_slovenian_537 = {IM_SLOVENIAN, &LDB_537};
  #endif/*KMX_LDB_SLOVENIAN*/

  #ifdef KMX_LDB_SPANISH
  #include "ldb_spanish_500.h"
  #include "ldb_spanish_501.h"
  #include "ldb_spanish_504.h"
  #include "ldb_spanish_505.h"
  #include "ldb_spanish_507.h"
  #include "ldb_spanish_5071.h"
  const void *kmx_ldb_spanish_500 = &LDB_500;
  const void *kmx_ldb_spanish_501 = &LDB_501;
  const void *kmx_ldb_spanish_504 = &LDB_504;
  const void *kmx_ldb_spanish_505 = &LDB_505;
  const void *kmx_ldb_spanish_507 = &LDB_507;
  const void *kmx_ldb_spanish_5071 = &LDB_5071;
  
  const IIME_IME_INFO IIME_ldb_spanish_500 = {IM_SPANISH, &LDB_500};
  const IIME_IME_INFO IIME_ldb_spanish_501 = {IM_SPANISH, &LDB_501};
  const IIME_IME_INFO IIME_ldb_spanish_504 = {IM_SPANISH, &LDB_504};
  const IIME_IME_INFO IIME_ldb_spanish_505 = {IM_SPANISH, &LDB_505};
  const IIME_IME_INFO IIME_ldb_spanish_507 = {IM_SPANISH, &LDB_507};
  #endif/*KMX_LDB_SPANISH*/
		
  #ifdef KMX_LDB_SWAHILI
  #include "ldb_swahili_550.h"
  #include "ldb_swahili_555.h"
  #include "ldb_swahili_557.h"
  const void *kmx_ldb_swahili_550 = &LDB_550;
  const void *kmx_ldb_swahili_555 = &LDB_555;
  const void *kmx_ldb_swahili_557 = &LDB_557;
  const IIME_IME_INFO IIME_ldb_swahili_550 = {IM_SWAHILI, &LDB_550};
  const IIME_IME_INFO IIME_ldb_swahili_555 = {IM_SWAHILI, &LDB_555};
  const IIME_IME_INFO IIME_ldb_swahili_557 = {IM_SWAHILI, &LDB_557};
  #endif/*KMX_LDB_SWAHILI*/
  
  #ifdef KMX_LDB_SWEDISH
  #include "ldb_swedish_490.h"
  #include "ldb_swedish_495.h"
  #include "ldb_swedish_497.h"
  const void *kmx_ldb_swedish_490 = &LDB_490;
  const void *kmx_ldb_swedish_495 = &LDB_495;
  const void *kmx_ldb_swedish_497 = &LDB_497;
  const IIME_IME_INFO IIME_ldb_swedish_490 = {IM_SWEDISH, &LDB_490};
  const IIME_IME_INFO IIME_ldb_swedish_495 = {IM_SWEDISH, &LDB_495};
  const IIME_IME_INFO IIME_ldb_swedish_497 = {IM_SWEDISH, &LDB_497};
  #endif/*KMX_LDB_SWEDISH*/

  #ifdef KMX_LDB_THAI
  #include "ldb_thai_360.h"
  #include "ldb_thai_361.h"
  #include "ldb_thai_362.h"
  #include "ldb_thai_363.h"
  #include "ldb_thai_364.h"
  #include "ldb_thai_365.h"
  #include "ldb_thai_366.h"
  #include "ldb_thai_367.h"
  #include "ldb_thai_3671.h"
  #include "ldb_thai_368.h"
  #include "ldb_thai_369.h"
  #include "ldb_thai_3600.h"
  #include "ldb_thai_3601.h"
  #include "ldb_thai_3602.h"
  #include "ldb_thai_3603.h"
  #include "ldb_thai_3604.h"		
  #include "ldb_thai_3606.h"
  #include "ldb_thai_3607.h"
  #include "ldb_thai_3608.h"
  #include "ldb_thai_3611.h"
  #include "ldb_thai_3616.h"
  #include "ldb_thai_3640.h"
  #include "ldb_thai_3632.h"		
  const void *kmx_ldb_thai_360 = &LDB_360;
  const void *kmx_ldb_thai_361 = &LDB_361;
  const void *kmx_ldb_thai_362 = &LDB_362;
  const void *kmx_ldb_thai_363 = &LDB_363;
  const void *kmx_ldb_thai_364 = &LDB_364;
  const void *kmx_ldb_thai_365 = &LDB_365;
  const void *kmx_ldb_thai_366 = &LDB_366;
  const void *kmx_ldb_thai_367 = &LDB_367;
  const void *kmx_ldb_thai_3671 = &LDB_3671;
  const void *kmx_ldb_thai_368 = &LDB_368;
  const void *kmx_ldb_thai_369 = &LDB_369;
  const void *kmx_ldb_thai_3600 = &LDB_3600;
  const void *kmx_ldb_thai_3601 = &LDB_3601;
  const void *kmx_ldb_thai_3602 = &LDB_3602;
  const void *kmx_ldb_thai_3603 = &LDB_3603;
  const void *kmx_ldb_thai_3604 = &LDB_3604;  
  const void *kmx_ldb_thai_3606 = &LDB_3606;
  const void *kmx_ldb_thai_3607 = &LDB_3607;
  const void *kmx_ldb_thai_3608 = &LDB_3608;
  const void *kmx_ldb_thai_3611 = &LDB_3611;
  const void *kmx_ldb_thai_3616 = &LDB_3616;	
  const void *kmx_ldb_thai_3640 = &LDB_3640;
  const void *kmx_ldb_thai_3632 = &LDB_3632;
  const IIME_IME_INFO IIME_ldb_thai_360 = {IM_THAI, &LDB_360};
  const IIME_IME_INFO IIME_ldb_thai_361 = {IM_THAI, &LDB_361};
  const IIME_IME_INFO IIME_ldb_thai_362 = {IM_THAI, &LDB_362};
  const IIME_IME_INFO IIME_ldb_thai_363 = {IM_THAI, &LDB_363};
  const IIME_IME_INFO IIME_ldb_thai_364 = {IM_THAI, &LDB_364};
  const IIME_IME_INFO IIME_ldb_thai_365 = {IM_THAI, &LDB_365};
  const IIME_IME_INFO IIME_ldb_thai_366 = {IM_THAI, &LDB_366};
  const IIME_IME_INFO IIME_ldb_thai_367 = {IM_THAI, &LDB_367};
  const IIME_IME_INFO IIME_ldb_thai_3671 = {IM_THAI, &LDB_3671};
  const IIME_IME_INFO IIME_ldb_thai_368 = {IM_THAI, &LDB_368};
  const IIME_IME_INFO IIME_ldb_thai_369 = {IM_THAI, &LDB_369};
  const IIME_IME_INFO IIME_ldb_thai_3600 = {IM_THAI, &LDB_3600};
  const IIME_IME_INFO IIME_ldb_thai_3601 = {IM_THAI, &LDB_3601};
  const IIME_IME_INFO IIME_ldb_thai_3602 = {IM_THAI, &LDB_3602};
  const IIME_IME_INFO IIME_ldb_thai_3603 = {IM_THAI, &LDB_3603};
  const IIME_IME_INFO IIME_ldb_thai_3604 = {IM_THAI, &LDB_3604};  
  const IIME_IME_INFO IIME_ldb_thai_3606 = {IM_THAI, &LDB_3606};
  const IIME_IME_INFO IIME_ldb_thai_3607 = {IM_THAI, &LDB_3607};
  const IIME_IME_INFO IIME_ldb_thai_3608 = {IM_THAI, &LDB_3608};
  const IIME_IME_INFO IIME_ldb_thai_3611 = {IM_THAI, &LDB_3611};
  const IIME_IME_INFO IIME_ldb_thai_3616 = {IM_THAI, &LDB_3616};	 
  const IIME_IME_INFO IIME_ldb_thai_3632 = {IM_THAI, &LDB_3632};  
  const IIME_IME_INFO IIME_ldb_thai_3640 = {IM_THAI, &LDB_3640};
  #endif/*KMX_LDB_THAI*/

  #ifdef KMX_LDB_TIBETAN
  #include "ldb_tibetan_730.h"
  const void *kmx_ldb_tibetan_730 = &LDB_730;
  const IIME_IME_INFO IIME_ldb_tibetan_730 = {IM_TIBETAN, &LDB_730};
  #endif/*KMX_LDB_TIBETAN*/

  #ifdef KMX_LDB_TURKISH
  #include "ldb_turkish_330.h"
  #include "ldb_turkish_335.h"
  #include "ldb_turkish_337.h"
  #include "ldb_turkish_3371.h"
  const void *kmx_ldb_turkish_330 = &LDB_330;
  const void *kmx_ldb_turkish_335 = &LDB_335;
  const void *kmx_ldb_turkish_337 = &LDB_337;
  const void *kmx_ldb_turkish_3371 = &LDB_3371;
  const IIME_IME_INFO IIME_ldb_turkish_330 = {IM_TURKISH, &LDB_330};
  const IIME_IME_INFO IIME_ldb_turkish_335 = {IM_TURKISH, &LDB_335};
  const IIME_IME_INFO IIME_ldb_turkish_337 = {IM_TURKISH, &LDB_337};
  #endif/*KMX_LDB_TURKISH*/

  #ifdef KMX_LDB_UKRAINIAN
  #include "ldb_ukrainian_540.h"
  #include "ldb_ukrainian_545.h"
  #include "ldb_ukrainian_547.h"
  #include "ldb_ukrainian_548.h"
  const void *kmx_ldb_ukrainian_540 = &LDB_540;
  const void *kmx_ldb_ukrainian_545 = &LDB_545;
  const void *kmx_ldb_ukrainian_547 = &LDB_547;
  const void *kmx_ldb_ukrainian_548 = &LDB_548;
  const IIME_IME_INFO IIME_ldb_ukrainian_540 = {IM_UKRAINIAN, &LDB_540};
  const IIME_IME_INFO IIME_ldb_ukrainian_545 = {IM_UKRAINIAN, &LDB_545};
  const IIME_IME_INFO IIME_ldb_ukrainian_547 = {IM_UKRAINIAN, &LDB_547};
  const IIME_IME_INFO IIME_ldb_ukrainian_548 = {IM_UKRAINIAN, &LDB_548};
  #endif/*KMX_LDB_UKRAINIAN*/

  #ifdef KMX_LDB_UYGHUR
  #include "ldb_uyghur_940.h"
  #include "ldb_uyghur_941.h"
  #include "ldb_uyghur_947.h"
  #include "ldb_uyghur_948.h"
  const void *kmx_ldb_uyghur_940 = &LDB_940;
  const void *kmx_ldb_uyghur_941 = &LDB_941;
  const void *kmx_ldb_uyghur_947 = &LDB_947;
  const void *kmx_ldb_uyghur_948 = &LDB_948;
  const IIME_IME_INFO IIME_ldb_uyghur_940 = {IM_UYGHUR, &LDB_940};
  const IIME_IME_INFO IIME_ldb_uyghur_941 = {IM_UYGHUR, &LDB_941};
  const IIME_IME_INFO IIME_ldb_uyghur_947 = {IM_UYGHUR, &LDB_947};
  const IIME_IME_INFO IIME_ldb_uyghur_948 = {IM_UYGHUR, &LDB_948};
  #endif

  #ifdef KMX_LDB_VIETNAMESE
  #include "ldb_vietnamese_310.h"
  #include "ldb_vietnamese_315.h"
  #include "ldb_vietnamese_317.h"
  #include "ldb_vietnamese_3171.h"
  const void *kmx_ldb_vietnamese_310 = &LDB_310;
  const void *kmx_ldb_vietnamese_315 = &LDB_315;
  const void *kmx_ldb_vietnamese_317 = &LDB_317;
  const void *kmx_ldb_vietnamese_3171 = &LDB_3171;
  const IIME_IME_INFO IIME_ldb_vietnamese_310 = {IM_VIETNAMESE, &LDB_310};
  const IIME_IME_INFO IIME_ldb_vietnamese_315 = {IM_VIETNAMESE, &LDB_315};
  const IIME_IME_INFO IIME_ldb_vietnamese_317 = {IM_VIETNAMESE, &LDB_317};
  #endif/*KMX_LDB_VIETNAMESE*/

  #ifdef KMX_LDB_URDU
  #include "ldb_urdu_950.h"
  #include "ldb_urdu_955.h"
  #include "ldb_urdu_957.h"
  #include "ldb_urdu_9571.h"
  const void *kmx_ldb_urdu_950 = &LDB_950;
  const void *kmx_ldb_urdu_955 = &LDB_955;
  const void *kmx_ldb_urdu_957 = &LDB_957;
  const void *kmx_ldb_urdu_9571 = &LDB_9571;
  const IIME_IME_INFO IIME_ldb_urdu_950 = {IM_URDU, &LDB_950};
  const IIME_IME_INFO IIME_ldb_urdu_955 = {IM_URDU, &LDB_955};
  const IIME_IME_INFO IIME_ldb_urdu_957 = {IM_URDU, &LDB_957};
  const IIME_IME_INFO IIME_ldb_urdu_9571 = {IM_URDU, &LDB_9571};
  #endif/*KMX_LDB_URDU*/

  #ifdef KMX_LDB_TAGALOG
  #include "ldb_tagalog_587.h"
  const void *kmx_ldb_tagalog_587 = &LDB_587;
  const IIME_IME_INFO IIME_ldb_tagalog_587 = {IM_TAGALOG, &LDB_587};
  #endif/*KMX_LDB_TAGALOG*/

  #ifdef KMX_LDB_KYRGYZ
  #include "ldb_kyrgyz_747.h"
  const void *kmx_ldb_kyrgyz_587 = &LDB_747;
  const IIME_IME_INFO IIME_ldb_kyrgyz_587 = {IM_KYRGYZ, &LDB_747};
  #endif/*KMX_LDB_KYRGYZ*/

  #ifdef KMX_LDB_ASSAMESE
  #include "ldb_assamese_710.h"
  const void *kmx_ldb_assamese_710 = &LDB_710;
  const IIME_IME_INFO IIME_ldb_assamese_710 = {IM_ASSAMESE, &LDB_710};
  #endif/*KMX_LDB_ASSAMESE*/

  #ifdef KMX_LDB_ALBANIAN
  #include "ldb_albanian_611.h"
  const void *kmx_ldb_albanian_611 = &LDB_611;
  const IIME_IME_INFO IIME_ldb_albanian_611 = {IM_ALBANIAN, &LDB_611};
  #endif/*KMX_LDB_ALBANIAN*/

  #ifdef KMX_LDB_CATALAN
  #include "ldb_catalan_621.h"
  const void *kmx_ldb_catalan_621 = &LDB_621;
  const IIME_IME_INFO IIME_ldb_catalan_621 = {IM_CATALAN, &LDB_621};
  #endif/*KMX_LDB_CATALAN*/

  #ifdef KMX_LDB_BASQUE
  #include "ldb_basque_631.h"
  const void *kmx_ldb_basque_631 = &LDB_631;
  const IIME_IME_INFO IIME_ldb_basque_631 = {IM_BASQUE, &LDB_631};
  #endif/*KMX_LDB_BASQUE*/

  #ifdef KMX_LDB_GALICIAN
  #include "ldb_galician_641.h"
  const void *kmx_ldb_galician_641 = &LDB_641;
  const IIME_IME_INFO IIME_ldb_galician_641 = {IM_GALICIAN, &LDB_641};
  #endif/*KMX_LDB_GALICIAN*/

  #ifdef KMX_LDB_IGBO
  #include "ldb_igbo_651.h"
  const void *kmx_ldb_igbo_651 = &LDB_651;
  const IIME_IME_INFO IIME_ldb_igbo_651 = {IM_IGBO, &LDB_651};
  #endif/*KMX_LDB_IGBO*/
  
  #ifdef KMX_LDB_OROMO
  #include "ldb_oromo_661.h"
  const void *kmx_ldb_oromo_661 = &LDB_661;
  const IIME_IME_INFO IIME_ldb_oromo_661 = {IM_OROMO, &LDB_661};
  #endif/*KMX_LDB_OROMO*/
	  
  #ifdef KMX_LDB_SERBIAN
  #include "ldb_serbian_233.h"
  #include "ldb_serbianc_243.h"
  const void *kmx_ldb_serbian_233 = &LDB_233;
  const void *kmx_ldb_serbianc_641 = &LDB_243;
  const IIME_IME_INFO IIME_ldb_serbian_233 = {IM_SERBIAN, &LDB_233};
  const IIME_IME_INFO IIME_ldb_serbianc_243 = {IM_SERBIANC, &LDB_243};
  #endif/*KMX_LDB_SERBIAN*/

  #ifdef KMX_LDB_ZULU
  #include "ldb_zulu_677.h"
  const void *kmx_ldb_zulu_677 = &LDB_677;
  const IIME_IME_INFO IIME_ldb_zulu_677 = {IM_ZULU, &LDB_677};
  #endif/*KMX_LDB_ZULU*/

  #ifdef KMX_LDB_CEBUANO
  #include "ldb_cebuano_687.h"
  const void *kmx_ldb_cebuano_687 = &LDB_687;
  const IIME_IME_INFO IIME_ldb_cebuano_687 = {IM_CEBUANO, &LDB_687};
  #endif/*KMX_LDB_CEBUANO*/

  #ifdef KMX_LDB_KAZAKH
  #include "ldb_kazakh_257.h"
  const void *kmx_ldb_kazakh_257 = &LDB_257;
  const IIME_IME_INFO IIME_ldb_kazakh_257 = {IM_KAZAKH, &LDB_257};
  #endif/*KMX_LDB_KAZAKH*/

  #ifdef KMX_LDB_UZBEK
  #include "ldb_uzbek_267.h"
  const void *kmx_ldb_uzbek_267 = &LDB_267;
  const IIME_IME_INFO IIME_ldb_uzbek_267 = {IM_UZBEK, &LDB_267};
  #endif/*KMX_LDB_UZBEK*/

  #ifdef KMX_LDB_GEORGIAN
  #include "ldb_georgian_277.h"
  const void *kmx_ldb_georgian_277 = &LDB_277;
  const IIME_IME_INFO IIME_ldb_georgian_277 = {IM_GEORGIAN, &LDB_277};
  #endif/*KMX_LDB_GEORGIAN*/

  #ifdef KMX_LDB_NEPALI
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_nepali_720 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_nepali_720 = {IM_NEPALI, &LDB_720};
  #endif/*KMX_LDB_NEPALI*/

  #ifdef KMX_LDB_SINHALA
  #include "ldb_sinhala_770.h"
  const IIME_IME_INFO IIME_ldb_sinhala_770 = {IM_SINHALA, &LDB_770};
  const void *kmx_ldb_sinhala_770 = &LDB_770;
  #endif/*KMX_LDB_SINHALA*/

  #ifdef KMX_LDB_BODO//daodejing  need
  #include "ldb_nepali_720.h"//daodejing  need
    const void *kmx_ldb_bodo_1100 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_bodo_1100 = {IM_BODO, &LDB_720};
  #endif/*KMX_LDB_BODO*/

  #ifdef KMX_LDB_DOGRI//daodejing  need
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_dogri_1110 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_dogri_1110 = {IM_DOGRI, &LDB_720};
  #endif/*KMX_LDB_DOGRI*/

  #ifdef KMX_LDB_SANSKRIT
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_sanskrit_1120 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_sanskrit_1120 = {IM_SANSKRIT, &LDB_720};
  #endif/*KMX_LDB_SANSKRIT*/

  #ifdef KMX_LDB_KASHMIRI
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_kashmiri_1130 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_kashmiri_1130 = {IM_KASHMIRI, &LDB_720};
  #endif/*KMX_LDB_KASHMIRI*/

  #ifdef KMX_LDB_KONKANI
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_konkani_1140 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_konkani_1140 = {IM_KONKANI, &LDB_720};
  #endif/*KMX_LDB_KONKANI*/

  #ifdef KMX_LDB_MAITHILI
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_maithili_1150 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_maithili_1150 = {IM_MAITHILI, &LDB_720};
  #endif/*KMX_LDB_MAITHILI*/

  #ifdef KMX_LDB_MANIPURI
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_manipuri_1160 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_manipuri_1160 = {IM_MANIPURI, &LDB_720};
  #endif/*KMX_LDB_MANIPURI*/

  #ifdef KMX_LDB_SANTALI
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_santali_1170 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_santali_1170 = {IM_SANTALI, &LDB_720};
  #endif/*KMX_LDB_SANTALI*/

  #ifdef KMX_LDB_SOMALI
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_somali_1180 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_somali_1180 = {IM_SOMALI, &LDB_720};
  #endif/*KMX_LDB_SOMALI*/

  #ifdef KMX_LDB_SINDHI
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_sindhi_1190 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_sindhi_1190 = {IM_SINDHI, &LDB_720};
  #endif/*KMX_LDB_SINDHI*/
  
  #ifndef KMX_LDB_ENGLISH
  #include "ldb_english_107.h"
   const IIME_IME_INFO IIME_ldb_english_107 = {IM_ENGLISH, &LDB_107};
   const void *kmx_ldb_english_107 = &LDB_107;
  #endif/*KMX_LDB_ENGLISH*/


#elif defined(IEKIE_INPUT_SAME_LANGUAGE_ONE_LDB)
  #if defined(KMX_LDB_ENGLISH)
  	#if KMX_LDB_ENGLISH == 10001
		#include "ldb_english_10001.h"
		const void *kmx_ldb_english_10001 = &LDB_10001;
		const IIME_IME_INFO IIME_ldb_english_10001 = {IM_ENGLISH, &LDB_10001};
	//#elif KMX_LDB_ENGLISH == 100
	//	#include "ldb_english_100.h"
	//	const void *kmx_ldb_english_100 = &LDB_100;
	//	const IIME_IME_INFO IIME_ldb_english_100 = {IM_ENGLISH, &LDB_100};
	//#elif KMX_LDB_ENGLISH == 101
	//	#include "ldb_english_101.h"
	//	const void *kmx_ldb_english_101 = &LDB_101;
	//	const IIME_IME_INFO IIME_ldb_english_101 = {IM_ENGLISH, &LDB_101};
	#elif KMX_LDB_ENGLISH == 102
		#include "ldb_english_102.h"
		const void *kmx_ldb_english_102 = &LDB_102;
		const IIME_IME_INFO IIME_ldb_english_102 = {IM_ENGLISH, &LDB_102};
	#elif KMX_LDB_ENGLISH == 103
		#include "ldb_english_103.h"
		const void *kmx_ldb_english_103 = &LDB_103;
		const IIME_IME_INFO IIME_ldb_english_103 = {IM_ENGLISH, &LDB_103};
	#elif KMX_LDB_ENGLISH == 104
		#include "ldb_english_104.h"
		const void *kmx_ldb_english_104 = &LDB_104;
		const IIME_IME_INFO IIME_ldb_english_104 = {IM_ENGLISH, &LDB_104};
	#elif KMX_LDB_ENGLISH == 105
		#include "ldb_english_105.h"
		const void *kmx_ldb_english_105 = &LDB_105;
		const IIME_IME_INFO IIME_ldb_english_105 = {IM_ENGLISH, &LDB_105};
	#elif KMX_LDB_ENGLISH == 106
		#include "ldb_english_106.h"
		const void *kmx_ldb_english_106 = &LDB_106;
		const IIME_IME_INFO IIME_ldb_english_106 = {IM_ENGLISH, &LDB_106};
	#elif KMX_LDB_ENGLISH == 107
		#include "ldb_english_107.h"
		const void *kmx_ldb_english_107 = &LDB_107;
		const IIME_IME_INFO IIME_ldb_english_107 = {IM_ENGLISH, &LDB_107};
	#elif KMX_LDB_ENGLISH == 1071
		#include "ldb_english_1071.h"
		const void *kmx_ldb_english_1071 = &LDB_1071;
		const IIME_IME_INFO IIME_ldb_english_1071 = {IM_ENGLISH, &LDB_1071};
	#elif KMX_LDB_ENGLISH == 108
		#include "ldb_english_108.h"
		const void *kmx_ldb_english_108 = &LDB_108;
		const IIME_IME_INFO IIME_ldb_english_108 = {IM_ENGLISH, &LDB_108};
	#elif KMX_LDB_ENGLISH == 109
		#include "ldb_english_109.h"	
		const void *kmx_ldb_english_109 = &LDB_109;
		const IIME_IME_INFO IIME_ldb_english_109 = {IM_ENGLISH, &LDB_109};
	#else
        #error "Suretype English database not defined!"
    #endif

	#if (KMX_LDB_ENGLISH != 107)
  		#include "ldb_english_107.h"
   		const IIME_IME_INFO IIME_ldb_english_107 = {IM_ENGLISH, &LDB_107};
   		const void *kmx_ldb_english_107 = &LDB_107;
  	#endif/*KMX_LDB_ENGLISH*/
	
  #else
	#include "ldb_english_107.h"
	const IIME_IME_INFO IIME_ldb_english_107 = {IM_ENGLISH, &LDB_107};
   	const void *kmx_ldb_english_107 = &LDB_107;
  #endif/*KMX_LDB_ENGLISH*/


  
  #if defined(KMX_LDB_ARMENIAN)
	#if KMX_LDB_ARMENIAN == 570
		#include "ldb_armenian_570.h"
		const void *kmx_ldb_armenian_570 = &LDB_570;
		const IIME_IME_INFO IIME_ldb_armenian_570 = {IM_ARMENIAN, &LDB_570};
	#elif KMX_LDB_ARMENIAN == 575
		#include "ldb_armenian_575.h"
		const void *kmx_ldb_armenian_575 = &LDB_575;
		const IIME_IME_INFO IIME_ldb_armenian_575 = {IM_ARMENIAN, &LDB_575};
	#elif KMX_LDB_ARMENIAN == 577
		#include "ldb_armenian_577.h" 
		const void *kmx_ldb_armenian_577 = &LDB_577;
		const IIME_IME_INFO IIME_ldb_armenian_577 = {IM_ARMENIAN, &LDB_577};
	#else
		#error "Suretype ARMENIAN database not defined!"
	#endif
  #endif/*KMX_LDB_ARMENIAN*/


  #if defined(KMX_LDB_BENGALI)
	#if KMX_LDB_BENGALI == 920
  		#include "ldb_bengali_920.h"
		const void *kmx_ldb_bengali_920 = &LDB_920;
		const IIME_IME_INFO IIME_ldb_bengali_920 = {IM_BENGALI, &LDB_920};
	#elif KMX_LDB_BENGALI == 921
  		#include "ldb_bengali_921.h"
		const void *kmx_ldb_bengali_921 = &LDB_921;
		const IIME_IME_INFO IIME_ldb_bengali_921 = {IM_BENGALI, &LDB_921};
	#elif KMX_LDB_BENGALI == 922
  		#include "ldb_bengali_922.h"
		const void *kmx_ldb_bengali_922 = &LDB_922;
		const IIME_IME_INFO IIME_ldb_bengali_922 = {IM_BENGALI, &LDB_922};
	#elif KMX_LDB_BENGALI == 923
  		#include "ldb_bengali_923.h"
		const void *kmx_ldb_bengali_923 = &LDB_923;
		const IIME_IME_INFO IIME_ldb_bengali_923 = {IM_BENGALI, &LDB_923};
	#elif KMX_LDB_BENGALI == 924
  		#include "ldb_bengali_924.h"
		const void *kmx_ldb_bengali_924 = &LDB_924;
		const IIME_IME_INFO IIME_ldb_bengali_924 = {IM_BENGALI, &LDB_924};
	#elif KMX_LDB_BENGALI == 925
  		#include "ldb_bengali_925.h"
		const void *kmx_ldb_bengali_925 = &LDB_925;
		const IIME_IME_INFO IIME_ldb_bengali_925 = {IM_BENGALI, &LDB_925};
	#elif KMX_LDB_BENGALI == 927
  		#include "ldb_bengali_927.h"
		const void *kmx_ldb_bengali_927 = &LDB_927;
		const IIME_IME_INFO IIME_ldb_bengali_927 = {IM_BENGALI, &LDB_927};
	#elif KMX_LDB_BENGALI == 928
  		#include "ldb_bengali_928.h"
		const void *kmx_ldb_bengali_928 = &LDB_928;
		const IIME_IME_INFO IIME_ldb_bengali_928 = {IM_BENGALI, &LDB_928};
	#elif KMX_LDB_BENGALI == 929
  		#include "ldb_bengali_929.h"
		const void *kmx_ldb_bengali_929 = &LDB_929;
		const IIME_IME_INFO IIME_ldb_bengali_929 = {IM_BENGALI, &LDB_929};
	#elif KMX_LDB_BENGALI == 9200
  		#include "ldb_bengali_9200.h"
		const void *kmx_ldb_bengali_9200 = &LDB_9200;
		const IIME_IME_INFO IIME_ldb_bengali_9200 = {IM_BENGALI, &LDB_9200};
	#elif KMX_LDB_BENGALI == 9201
  		#include "ldb_bengali_9201.h"
        #include "indic_mt_ldb.h"
		//const void *kmx_ldb_bengali_920 = &LDB_920;
		const IIME_IME_INFO IIME_ldb_bengali_9201 = {IM_BENGALI, &dummy_bengali_ldb};
	#elif KMX_LDB_BENGALI == 9202
  		#include "ldb_bengali_9202.h"
		const void *kmx_ldb_bengali_9202 = &LDB_9202;
		const IIME_IME_INFO IIME_ldb_bengali_9202 = {IM_BENGALI, &LDB_9202};
	#elif KMX_LDB_BENGALI == 9203
  		#include "ldb_bengali_9203.h"
		const void *kmx_ldb_bengali_9203 = &LDB_9203;
		const IIME_IME_INFO IIME_ldb_bengali_9203 = {IM_BENGALI, &LDB_9203};		
  	#else
	  	#error "Suretype BENGALI database not defined!"
  	#endif
  #endif/*KMX_LDB_BENGALI*/


  #if defined(KMX_LDB_BULGARIAN)
  	#if KMX_LDB_BULGARIAN == 430
	  	#include "ldb_bulgarian_430.h"
		const void *kmx_ldb_bulgarian_430 = &LDB_430;
		const IIME_IME_INFO IIME_ldb_bulgarian_430 = {IM_BULGARIAN, &LDB_430};
  	#elif KMX_LDB_BULGARIAN == 435
	  	#include "ldb_bulgarian_435.h"
		const void *kmx_ldb_bulgarian_435 = &LDB_435;
		const IIME_IME_INFO IIME_ldb_bulgarian_435 = {IM_BULGARIAN, &LDB_435};
  	#elif KMX_LDB_BULGARIAN == 437
		#include "ldb_bulgarian_437.h"
		const void *kmx_ldb_bulgarian_437 = &LDB_437;
		const IIME_IME_INFO IIME_ldb_bulgarian_437 = {IM_BULGARIAN, &LDB_437};	  
  	#else
		#error "Suretype BULGARIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_BULGARIAN*/

  #if defined(KMX_LDB_CROATIAN)
  	#if KMX_LDB_CROATIAN == 510
	  	#include "ldb_croatian_510.h"
		const void *kmx_ldb_croatian_510 = &LDB_510;
		const IIME_IME_INFO IIME_ldb_croatian_510 = {IM_CROATIAN, &LDB_510};
  	#elif KMX_LDB_CROATIAN == 515
	  	#include "ldb_croatian_515.h"
		const void *kmx_ldb_croatian_515 = &LDB_515;
		const IIME_IME_INFO IIME_ldb_croatian_515 = {IM_CROATIAN, &LDB_515};
  	#elif KMX_LDB_CROATIAN == 517
		#include "ldb_croatian_517.h"
		const void *kmx_ldb_croatian_517 = &LDB_517;
		const IIME_IME_INFO IIME_ldb_croatian_517 = {IM_CROATIAN, &LDB_517};	  
  	#else
		#error "Suretype CROATIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_CROATIAN*/

  #if defined(KMX_LDB_CZECH)
  	#if KMX_LDB_CZECH == 440
		#include "ldb_czech_440.h"
		const void *kmx_ldb_czech_440 = &LDB_440;
		const IIME_IME_INFO IIME_ldb_czech_440 = {IM_CZECH, &LDB_440};
  	#elif KMX_LDB_CZECH == 442
		#include "ldb_czech_442.h"
		const void *kmx_ldb_czech_442 = &LDB_442;
		const IIME_IME_INFO IIME_ldb_czech_442 = {IM_CZECH, &LDB_442};
  	#elif KMX_LDB_CZECH == 443
		#include "ldb_czech_443.h"
		const void *kmx_ldb_czech_443 = &LDB_443;
		const IIME_IME_INFO IIME_ldb_czech_443 = {IM_CZECH, &LDB_443};
  	#elif KMX_LDB_CZECH == 445
		#include "ldb_czech_445.h"
		const void *kmx_ldb_czech_445 = &LDB_445;
		const IIME_IME_INFO IIME_ldb_czech_445 = {IM_CZECH, &LDB_445};
  	#elif KMX_LDB_CZECH == 447
		#include "ldb_czech_447.h"
		const void *kmx_ldb_czech_447 = &LDB_447;
		const IIME_IME_INFO IIME_ldb_czech_447 = {IM_CZECH, &LDB_447};	
  	#else
		#error "Suretype CZECH database not defined!"
  	#endif
  #endif/*KMX_LDB_CZECH*/

  #if defined(KMX_LDB_DANISH)
  	#if KMX_LDB_DANISH == 370
	  	#include "ldb_danish_370.h"
		const void *kmx_ldb_danish_370 = &LDB_370;
		const IIME_IME_INFO IIME_ldb_danish_370 = {IM_DANISH, &LDB_370};
  	#elif KMX_LDB_DANISH == 375
	  	#include "ldb_danish_375.h"
		const void *kmx_ldb_danish_375 = &LDB_375;
		const IIME_IME_INFO IIME_ldb_danish_375 = {IM_DANISH, &LDB_375};
  	#elif KMX_LDB_DANISH == 377
		#include "ldb_danish_377.h"
		const void *kmx_ldb_danish_377 = &LDB_377;
		const IIME_IME_INFO IIME_ldb_danish_377 = {IM_DANISH, &LDB_377};  
  	#else
		#error "Suretype DANISH database not defined!"
  	#endif
  #endif/*KMX_LDB_DANISH*/  
  
  #if defined(KMX_LDB_DUTCH)
  	#if KMX_LDB_DUTCH == 380
	  	#include "ldb_dutch_380.h"
		const void *kmx_ldb_dutch_380 = &LDB_380;
		const IIME_IME_INFO IIME_ldb_dutch_380 = {IM_DUTCH, &LDB_380};
  	#elif KMX_LDB_DUTCH == 385
	  	#include "ldb_dutch_385.h"
		const void *kmx_ldb_dutch_385 = &LDB_385;
		const IIME_IME_INFO IIME_ldb_dutch_385 = {IM_DUTCH, &LDB_385};
  	#elif KMX_LDB_DUTCH == 387
	  	#include "ldb_dutch_387.h"
		const void *kmx_ldb_dutch_387 = &LDB_387;
		const IIME_IME_INFO IIME_ldb_dutch_387 = {IM_DUTCH, &LDB_387};
  	#else
	  #error "Suretype DUTCH database not defined!"
  	#endif
  #endif/*KMX_LDB_DUTCH*/ 

  #if defined(KMX_LDB_FINNISH)
  	#if KMX_LDB_FINNISH == 450
  		#include "ldb_finnish_450.h"
  		const void *kmx_ldb_finnish_450 = &LDB_450;
  		const IIME_IME_INFO IIME_ldb_finnish_450 = {IM_FINNISH, &LDB_450};
  	#elif KMX_LDB_FINNISH == 455
  		#include "ldb_finnish_455.h"
  		const void *kmx_ldb_finnish_455 = &LDB_455;
  		const IIME_IME_INFO IIME_ldb_finnish_455 = {IM_FINNISH, &LDB_455};
  	#elif KMX_LDB_FINNISH == 457
  		#include "ldb_finnish_457.h"
  		const void *kmx_ldb_finnish_457 = &LDB_457;
  		const IIME_IME_INFO IIME_ldb_finnish_457 = {IM_FINNISH, &LDB_457};
  	#else
  		#error "Suretype FINNISH database not defined!"
  	#endif
  #endif/*KMX_LDB_FINNISH*/
  
  #if defined(KMX_LDB_FRENCH)
  	#if KMX_LDB_FRENCH == 400
	  	#include "ldb_french_400.h"
		const void *kmx_ldb_french_400 = &LDB_400;
		const IIME_IME_INFO IIME_ldb_french_400 = {IM_FRENCH, &LDB_400};
  	#elif KMX_LDB_FRENCH == 405
	  	#include "ldb_french_405.h"
		const void *kmx_ldb_french_405 = &LDB_405;
		const IIME_IME_INFO IIME_ldb_french_405 = {IM_FRENCH, &LDB_405};
  	#elif KMX_LDB_FRENCH == 406
	  	#include "ldb_french_406.h"
		const void *kmx_ldb_french_406 = &LDB_406;
		const IIME_IME_INFO IIME_ldb_french_406 = {IM_FRENCH, &LDB_406};
  	#elif KMX_LDB_FRENCH == 407
	  	#include "ldb_french_407.h"
		const void *kmx_ldb_french_407 = &LDB_407;
		const IIME_IME_INFO IIME_ldb_french_407 = {IM_FRENCH, &LDB_407};
  	#elif KMX_LDB_FRENCH == 4071
	  	#include "ldb_french_4071.h"
		const void *kmx_ldb_french_4071 = &LDB_4071;
		const IIME_IME_INFO IIME_ldb_french_4071 = {IM_FRENCH, &LDB_4071};
  	#else
	  	#error "Suretype FRENCH database not defined!"
  	#endif
  #endif/*KMX_LDB_FRENCH*/

  #if defined(KMX_LDB_GERMAN)
  	#if KMX_LDB_GERMAN == 300
	  	#include "ldb_german_300.h"
		const void *kmx_ldb_german_300 = &LDB_300;
		const IIME_IME_INFO IIME_ldb_german_300 = {IM_GERMAN, &LDB_300};
  	#elif KMX_LDB_GERMAN == 305
	  	#include "ldb_german_305.h"
		const void *kmx_ldb_german_305 = &LDB_305;
		const IIME_IME_INFO IIME_ldb_german_305 = {IM_GERMAN, &LDB_305};
  	#elif KMX_LDB_GERMAN == 307
	  	#include "ldb_german_307.h"
		const void *kmx_ldb_german_307 = &LDB_307;
		const IIME_IME_INFO IIME_ldb_german_307 = {IM_GERMAN, &LDB_307};
  	#elif KMX_LDB_GERMAN == 308
	  	#include "ldb_german_308.h"
		const void *kmx_ldb_german_308 = &LDB_308;
		const IIME_IME_INFO IIME_ldb_german_308 = {IM_GERMAN, &LDB_308};
  	#else
	  	#error "Suretype GERMAN database not defined!"
  	#endif
  #endif/*KMX_LDB_GERMAN*/

  #if defined(KMX_LDB_GREEK)
  	#if KMX_LDB_GREEK == 350
	  	#include "ldb_greek_350.h"
		const void *kmx_ldb_greek_350 = &LDB_350;
		const IIME_IME_INFO IIME_ldb_greek_350 = {IM_GREEK, &LDB_350};
  	#elif KMX_LDB_GREEK == 355
	  	#include "ldb_greek_355.h"
		const void *kmx_ldb_greek_355 = &LDB_355;
		const IIME_IME_INFO IIME_ldb_greek_355 = {IM_GREEK, &LDB_355};
  	#elif KMX_LDB_GREEK == 357
	  	#include "ldb_greek_357.h"
		const void *kmx_ldb_greek_357 = &LDB_357;
		const IIME_IME_INFO IIME_ldb_greek_357 = {IM_GREEK, &LDB_357};
  	#else
	  	#error "Suretype GREEK database not defined!"
  	#endif
  #endif/*KMX_LDB_GREEK*/
    
  #if defined(KMX_LDB_HEBREW)
  	#if KMX_LDB_HEBREW == 420
	  	#include "ldb_hebrew_420.h"
		const void *kmx_ldb_hebrew_420 = &LDB_420;
		const IIME_IME_INFO IIME_ldb_hebrew_420 = {IM_HEBREW, &LDB_420};
  	#elif KMX_LDB_HEBREW == 425
	  	#include "ldb_hebrew_425.h"
		const void *kmx_ldb_hebrew_425 = &LDB_425;
		const IIME_IME_INFO IIME_ldb_hebrew_425 = {IM_HEBREW, &LDB_425};
  	#elif KMX_LDB_HEBREW == 427
	  	#include "ldb_hebrew_427.h"
		const void *kmx_ldb_hebrew_427 = &LDB_427;
		const IIME_IME_INFO IIME_ldb_hebrew_427 = {IM_HEBREW, &LDB_427};
  	#elif KMX_LDB_HEBREW == 4271
	  	#include "ldb_hebrew_4271.h"
		const void *kmx_ldb_hebrew_4271 = &LDB_4271;
		const IIME_IME_INFO IIME_ldb_hebrew_4271 = {IM_HEBREW, &LDB_4271};
  	#else
	  	#error "Suretype HEBREW database not defined!"
  	#endif
  #endif/*KMX_LDB_HEBREW*/
  
  #if defined(KMX_LDB_HUNGARIAN)
  	#if KMX_LDB_HUNGARIAN == 410
	  	#include "ldb_hungarian_410.h"
		const void *kmx_ldb_hungarian_410 = &LDB_410;
		const IIME_IME_INFO IIME_ldb_hungarian_410 = {IM_HUNGARIAN, &LDB_410};
  	#elif KMX_LDB_HUNGARIAN == 412
	  	#include "ldb_hungarian_412.h"
		const void *kmx_ldb_hungarian_412 = &LDB_412;
		const IIME_IME_INFO IIME_ldb_hungarian_412 = {IM_HUNGARIAN, &LDB_412};
  	#elif KMX_LDB_HUNGARIAN == 413
	  	#include "ldb_hungarian_413.h"
		const void *kmx_ldb_hungarian_413 = &LDB_413;
		const IIME_IME_INFO IIME_ldb_hungarian_413 = {IM_HUNGARIAN, &LDB_413};
  	#elif KMX_LDB_HUNGARIAN == 415
	  	#include "ldb_hungarian_415.h"
		const void *kmx_ldb_hungarian_415 = &LDB_415;
		const IIME_IME_INFO IIME_ldb_hungarian_415 = {IM_HUNGARIAN, &LDB_415};
  	#elif KMX_LDB_HUNGARIAN == 417
	  	#include "ldb_hungarian_417.h"
		const void *kmx_ldb_hungarian_417 = &LDB_417;
		const IIME_IME_INFO IIME_ldb_hungarian_417 = {IM_HUNGARIAN, &LDB_417};
  	#else
	  #error "Suretype HUNGARIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_HUNGARIAN*/

  //ZHMCH other error
  #if defined(KMX_LDB_INDONESIAN)
  	#if KMX_LDB_INDONESIAN == 320
	  	#include "ldb_indonesian_320.h"
		const void *kmx_ldb_indonesian_320 = &LDB_320;
		const IIME_IME_INFO IIME_ldb_indonesian_320 = {IM_INDONESIAN, &LDB_320};
  	#elif KMX_LDB_INDONESIAN == 321
	  	#include "ldb_indonesian_321.h"
		const void *kmx_ldb_indonesian_321 = &LDB_321;
		const IIME_IME_INFO IIME_ldb_indonesian_321 = {IM_INDONESIAN, &LDB_321};
  	#elif KMX_LDB_INDONESIAN == 325
	  	#include "ldb_indonesian_325.h"
		const void *kmx_ldb_indonesian_325 = &LDB_325;
		const IIME_IME_INFO IIME_ldb_indonesian_325 = {IM_INDONESIAN, &LDB_325};
  	#elif KMX_LDB_INDONESIAN == 327
	  	#include "ldb_indonesian_327.h"
		const void *kmx_ldb_indonesian_327 = &LDB_327;
		const IIME_IME_INFO IIME_ldb_indonesian_327 = {IM_INDONESIAN, &LDB_327};
  	#elif KMX_LDB_INDONESIAN == 3271
	  	#include "ldb_indonesian_3271.h"
		const void *kmx_ldb_indonesian_3271 = &LDB_3271;
		const IIME_IME_INFO IIME_ldb_indonesian_3271 = {IM_INDONESIAN, &LDB_3271};
  	#else
	  #error "Suretype INDONESIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_INDONESIAN*/
  
  #if defined(KMX_LDB_ITALIAN)
  	#if KMX_LDB_ITALIAN == 700
	  	#include "ldb_italian_700.h"
		const void *kmx_ldb_italian_700 = &LDB_700;
		const IIME_IME_INFO IIME_ldb_italian_700 = {IM_ITALIAN, &LDB_700};
  	#elif KMX_LDB_ITALIAN == 705
	  	#include "ldb_italian_705.h"
		const void *kmx_ldb_italian_705 = &LDB_705;
		const IIME_IME_INFO IIME_ldb_italian_705 = {IM_ITALIAN, &LDB_705};
  	#elif KMX_LDB_ITALIAN == 707
	  	#include "ldb_italian_707.h"
		const void *kmx_ldb_italian_707 = &LDB_707;
		const IIME_IME_INFO IIME_ldb_italian_707 = {IM_ITALIAN, &LDB_707};
  	#else
	  	#error "Suretype ITALIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_ITALIAN*/
  
  #if defined(KMX_LDB_LITHUANIAN)
  	#if KMX_LDB_LITHUANIAN == 560
	  	#include "ldb_lithuanian_560.h"
		const void *kmx_ldb_lithuanian_560 = &LDB_560;
		const IIME_IME_INFO IIME_ldb_lithuanian_560 = {IM_LITHUANIAN, &LDB_560};
  	#elif KMX_LDB_LITHUANIAN == 565
	  	#include "ldb_lithuanian_565.h"
		const void *kmx_ldb_lithuanian_565 = &LDB_565;
		const IIME_IME_INFO IIME_ldb_lithuanian_565 = {IM_LITHUANIAN, &LDB_565};
  	#elif KMX_LDB_LITHUANIAN == 567
	  	#include "ldb_lithuanian_567.h"
		const void *kmx_ldb_lithuanian_567 = &LDB_567;
		const IIME_IME_INFO IIME_ldb_lithuanian_567 = {IM_LITHUANIAN, &LDB_567};
  	#else
	  	#error "Suretype LITHUANIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_LITHUANIAN*/

  #if defined(KMX_LDB_MALAY)
  	#if KMX_LDB_MALAY == 340
	  	#include "ldb_malay_340.h"
		const void *kmx_ldb_malay_340 = &LDB_340;
		const IIME_IME_INFO IIME_ldb_malay_340 = {IM_MALAY, &LDB_340};
  	#elif KMX_LDB_MALAY == 345
	  	#include "ldb_malay_345.h"
		const void *kmx_ldb_malay_345 = &LDB_345;
		const IIME_IME_INFO IIME_ldb_malay_345 = {IM_MALAY, &LDB_345};
  	#elif KMX_LDB_MALAY == 347
	  	#include "ldb_malay_347.h"
		const void *kmx_ldb_malay_347 = &LDB_347;
		const IIME_IME_INFO IIME_ldb_malay_347 = {IM_MALAY, &LDB_347};
  	#elif KMX_LDB_MALAY == 3471
	  	#include "ldb_malay_3471.h"
		const void *kmx_ldb_malay_3471 = &LDB_3471;
		const IIME_IME_INFO IIME_ldb_malay_347 = {IM_MALAY, &LDB_347};		
  	#else
	  #error "Suretype MALAY database not defined!"
  	#endif
  #endif/*KMX_LDB_MALAY*/

  #if defined(KMX_LDB_NORWEGIAN)
  	#if KMX_LDB_NORWEGIAN == 480
	  	#include "ldb_norwegian_480.h"
		const void *kmx_ldb_norwegian_480 = &LDB_480;
		const IIME_IME_INFO IIME_ldb_norwegian_480 = {IM_NORWEGIAN, &LDB_480};
  	#elif KMX_LDB_NORWEGIAN == 485
	  	#include "ldb_norwegian_485.h"
		const void *kmx_ldb_norwegian_485 = &LDB_485;
		const IIME_IME_INFO IIME_ldb_norwegian_485 = {IM_NORWEGIAN, &LDB_485};
  	#elif KMX_LDB_NORWEGIAN == 487
	  	#include "ldb_norwegian_487.h"
		const void *kmx_ldb_norwegian_487 = &LDB_487;
		const IIME_IME_INFO IIME_ldb_norwegian_487 = {IM_NORWEGIAN, &LDB_487};
  	#else
		#error "Suretype NORWEGIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_NORWEGIAN*/

  #if defined(KMX_LDB_PERSIAN)
  	#if KMX_LDB_PERSIAN == 460
	  	#include "ldb_persian_460.h"
		const void *kmx_ldb_persian_460 = &LDB_460;
		const IIME_IME_INFO IIME_ldb_persian_460 = {IM_PERSIAN, &LDB_460};
  	#elif KMX_LDB_PERSIAN == 462
	  	#include "ldb_persian_462.h"
		const void *kmx_ldb_persian_462 = &LDB_462;
		const IIME_IME_INFO IIME_ldb_persian_462 = {IM_PERSIAN, &LDB_462};
  	#elif KMX_LDB_PERSIAN == 463
  		#include "ldb_persian_463.h"
		const void *kmx_ldb_persian_463 = &LDB_463;
		const IIME_IME_INFO IIME_ldb_persian_463 = {IM_PERSIAN, &LDB_463};
	#elif KMX_LDB_PERSIAN == 465
  		#include "ldb_persian_465.h"
		const void *kmx_ldb_persian_465 = &LDB_465;
		const IIME_IME_INFO IIME_ldb_persian_465 = {IM_PERSIAN, &LDB_465};
	#elif KMX_LDB_PERSIAN == 467
		#include "ldb_persian_467.h"
		const void *kmx_ldb_persian_467 = &LDB_467;
		const IIME_IME_INFO IIME_ldb_persian_467 = {IM_PERSIAN, &LDB_467};
	#elif KMX_LDB_PERSIAN == 4671
  		#include "ldb_persian_4671.h"
		const void *kmx_ldb_persian_4671= &LDB_4671;
		const IIME_IME_INFO IIME_ldb_persian_4671 = {IM_PERSIAN, &LDB_4671};
	#elif KMX_LDB_PERSIAN == 468
  		#include "ldb_persian_468.h"
		const void *kmx_ldb_persian_468 = &LDB_468;
		const IIME_IME_INFO IIME_ldb_persian_468 = {IM_PERSIAN, &LDB_468};
	#elif KMX_LDB_PERSIAN == 469
		#include "ldb_persian_469.h"
		const void *kmx_ldb_persian_469 = &LDB_469;
		const IIME_IME_INFO IIME_ldb_persian_469 = {IM_PERSIAN, &LDB_469};
	#elif KMX_LDB_PERSIAN == 4600
  		#include "ldb_persian_4600.h"
		const void *kmx_ldb_persian_4600 = &LDB_4600;
		const IIME_IME_INFO IIME_ldb_persian_4600 = {IM_PERSIAN, &LDB_4600};
  	#else
	  #error "Suretype PERSIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_PERSIAN*/

  #if defined(KMX_LDB_POLISH)
  	#if KMX_LDB_POLISH == 391
	  	#include "ldb_polish_391.h"
		const void *kmx_ldb_polish_391 = &LDB_391;
		const IIME_IME_INFO IIME_ldb_polish_391 = {IM_POLISH, &LDB_391};
  	#elif KMX_LDB_POLISH == 392
	  	#include "ldb_polish_392.h"
		const void *kmx_ldb_polish_392 = &LDB_392;
		const IIME_IME_INFO IIME_ldb_polish_392 = {IM_POLISH, &LDB_392};
  	#elif KMX_LDB_POLISH == 393
	  	#include "ldb_polish_393.h"
		const void *kmx_ldb_polish_393 = &LDB_393;
		const IIME_IME_INFO IIME_ldb_polish_393 = {IM_POLISH, &LDB_393};
  	#elif KMX_LDB_POLISH == 395
	  	#include "ldb_polish_395.h"
		const void *kmx_ldb_polish_395 = &LDB_395;
		const IIME_IME_INFO IIME_ldb_polish_395 = {IM_POLISH, &LDB_395};
  	#elif KMX_LDB_POLISH == 397
	  	#include "ldb_polish_397.h"
		const void *kmx_ldb_polish_397 = &LDB_397;
		const IIME_IME_INFO IIME_ldb_polish_397 = {IM_POLISH, &LDB_397};			
  	#else
		#error "Suretype POLISH database not defined!"
  	#endif
  #endif/*KMX_LDB_POLISH*/

  #if defined(KMX_LDB_PORTUGUESE)
  	#if KMX_LDB_PORTUGUESE == 600
	  	#include "ldb_portuguese_600.h"
		const void *kmx_ldb_portuguese_600 = &LDB_600;
		const IIME_IME_INFO IIME_ldb_portuguese_600 = {IM_PORTUGUESE, &LDB_600};
  	#elif KMX_LDB_PORTUGUESE == 601
	  	#include "ldb_portuguese_601.h"
		const void *kmx_ldb_portuguese_601 = &LDB_601;
		const IIME_IME_INFO IIME_ldb_portuguese_601 = {IM_PORTUGUESE, &LDB_601};
  	#elif KMX_LDB_PORTUGUESE == 605
	  	#include "ldb_portuguese_605.h"
		const void *kmx_ldb_portuguese_605 = &LDB_605;
		const IIME_IME_INFO IIME_ldb_portuguese_605 = {IM_PORTUGUESE, &LDB_605};
  	#elif KMX_LDB_PORTUGUESE == 607
	  	#include "ldb_portuguese_607.h"
		const void *kmx_ldb_portuguese_607 = &LDB_607;
		const IIME_IME_INFO IIME_ldb_portuguese_607 = {IM_PORTUGUESE, &LDB_607};
  	#elif KMX_LDB_PORTUGUESE == 6071
	  	#include "ldb_portuguese_6071.h"
		const void *kmx_ldb_portuguese_6071 = &LDB_6071;
		const IIME_IME_INFO IIME_ldb_portuguese_6071 = {IM_PORTUGUESE, &LDB_6071};			
  	#else
	  	#error "Suretype PORTUGUESE database not defined!"
  	#endif
  #endif/*KMX_LDB_PORTUGUESE*/

  #if defined(KMX_LDB_ROMANIAN)
  	#if KMX_LDB_ROMANIAN == 520
	  	#include "ldb_romanian_520.h"
		const void *kmx_ldb_romanian_520 = &LDB_520;
		const IIME_IME_INFO IIME_ldb_romanian_520 = {IM_ROMANIAN, &LDB_520};
  	#elif KMX_LDB_ROMANIAN == 522
	  	#include "ldb_romanian_522.h"
		const void *kmx_ldb_romanian_522 = &LDB_522;
		const IIME_IME_INFO IIME_ldb_romanian_522 = {IM_ROMANIAN, &LDB_522};
  	#elif KMX_LDB_ROMANIAN == 523
	  	#include "ldb_romanian_523.h"
		const void *kmx_ldb_romanian_523 = &LDB_523;
		const IIME_IME_INFO IIME_ldb_romanian_523 = {IM_ROMANIAN, &LDB_523};
  	#elif KMX_LDB_ROMANIAN == 525
	  	#include "ldb_romanian_525.h"
		const void *kmx_ldb_romanian_525 = &LDB_525;
		const IIME_IME_INFO IIME_ldb_romanian_525 = {IM_ROMANIAN, &LDB_525};
  	#elif KMX_LDB_ROMANIAN == 527
	  	#include "ldb_romanian_527.h"
		const void *kmx_ldb_romanian_527 = &LDB_527;
		const IIME_IME_INFO IIME_ldb_romanian_527 = {IM_ROMANIAN, &LDB_527};		
  	#else
	  	#error "Suretype ROMANIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_ROMANIAN*/

  #if defined(KMX_LDB_RUSSIAN)
  	#if KMX_LDB_RUSSIAN == 202
	  	#include "ldb_russian_202.h"
		const void *kmx_ldb_russian_202 = &LDB_202;
		const IIME_IME_INFO IIME_ldb_russian_202 = {IM_RUSSIAN, &LDB_202};
  	#elif KMX_LDB_RUSSIAN == 205
	  	#include "ldb_russian_205.h"
		const void *kmx_ldb_russian_205 = &LDB_205;
		const IIME_IME_INFO IIME_ldb_russian_205 = {IM_RUSSIAN, &LDB_205};
  	#elif KMX_LDB_RUSSIAN == 207
	  	#include "ldb_russian_207.h"
		const void *kmx_ldb_russian_207 = &LDB_207;
		const IIME_IME_INFO IIME_ldb_russian_207 = {IM_RUSSIAN, &LDB_207};
  	#elif KMX_LDB_RUSSIAN == 2071
	  	#include "ldb_russian_2071.h"
		const void *kmx_ldb_russian_2071 = &LDB_2071;
		const IIME_IME_INFO IIME_ldb_russian_2071 = {IM_RUSSIAN, &LDB_2071};
  	#elif KMX_LDB_RUSSIAN == 208
	  	#include "ldb_russian_208.h"
		const void *kmx_ldb_russian_208 = &LDB_208;
		const IIME_IME_INFO IIME_ldb_russian_208 = {IM_RUSSIAN, &LDB_208};
  	#else
	  	#error "Suretype RUSSIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_RUSSIAN*/

  #if defined(KMX_LDB_SLOVAK)
  	#if KMX_LDB_SLOVAK == 470
	  	#include "ldb_slovak_470.h"
		const void *kmx_ldb_slovak_470 = &LDB_470;
		const IIME_IME_INFO IIME_ldb_slovak_470 = {IM_SLOVAK, &LDB_470};
  	#elif KMX_LDB_SLOVAK == 475
	  	#include "ldb_slovak_475.h"
		const void *kmx_ldb_slovak_475 = &LDB_475;
		const IIME_IME_INFO IIME_ldb_slovak_475 = {IM_SLOVAK, &LDB_475};
  	#elif KMX_LDB_SLOVAK == 477
	  	#include "ldb_slovak_477.h"
		const void *kmx_ldb_slovak_477 = &LDB_477;
		const IIME_IME_INFO IIME_ldb_slovak_477 = {IM_SLOVAK, &LDB_477};			
  	#else
	  	#error "Suretype SLOVAK database not defined!"
  	#endif
  #endif/*KMX_LDB_SLOVAK*/
  
  #if defined(KMX_LDB_SLOVENIAN)
  	#if KMX_LDB_SLOVENIAN == 530
	  	#include "ldb_slovenian_530.h"
		const void *kmx_ldb_slovenian_530 = &LDB_530;
		const IIME_IME_INFO IIME_ldb_slovenian_530 = {IM_SLOVENIAN, &LDB_530};
  	#elif KMX_LDB_SLOVENIAN == 535
	  	#include "ldb_slovenian_535.h"
		const void *kmx_ldb_slovenian_535 = &LDB_535;
		const IIME_IME_INFO IIME_ldb_slovenian_535 = {IM_SLOVENIAN, &LDB_535};
  	#elif KMX_LDB_SLOVENIAN == 537
	  	#include "ldb_slovenian_537.h"
		const void *kmx_ldb_slovenian_537 = &LDB_537;
		const IIME_IME_INFO IIME_ldb_slovenian_537 = {IM_SLOVENIAN, &LDB_537};			
  	#else
	  	#error "Suretype SLOVENIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_SLOVENIAN*/

  #if defined(KMX_LDB_SPANISH)
  	#if KMX_LDB_SPANISH == 500
	  	#include "ldb_spanish_500.h"
		const void *kmx_ldb_spanish_500 = &LDB_500;
		const IIME_IME_INFO IIME_ldb_spanish_500 = {IM_SPANISH, &LDB_500};
  	#elif KMX_LDB_SPANISH == 501
	  	#include "ldb_spanish_501.h"
		const void *kmx_ldb_spanish_501 = &LDB_501;
		const IIME_IME_INFO IIME_ldb_spanish_501 = {IM_SPANISH, &LDB_501};
  	#elif KMX_LDB_SPANISH == 504
	  	#include "ldb_spanish_504.h"
		const void *kmx_ldb_spanish_504 = &LDB_504;
		const IIME_IME_INFO IIME_ldb_spanish_504 = {IM_SPANISH, &LDB_504};
  	#elif KMX_LDB_SPANISH == 505
	  	#include "ldb_spanish_505.h"
		const void *kmx_ldb_spanish_505 = &LDB_505;
		const IIME_IME_INFO IIME_ldb_spanish_505 = {IM_SPANISH, &LDB_505};
  	#elif KMX_LDB_SPANISH == 507
	  	#include "ldb_spanish_507.h"
		const void *kmx_ldb_spanish_507 = &LDB_507;
		const IIME_IME_INFO IIME_ldb_spanish_507 = {IM_SPANISH, &LDB_507};
  	#elif KMX_LDB_SPANISH == 5071
	  	#include "ldb_spanish_5071.h"
		const void *kmx_ldb_spanish_5071 = &LDB_5071;
		const IIME_IME_INFO IIME_ldb_spanish_5071 = {IM_SPANISH, &LDB_5071};
  	#else
	  	#error "Suretype SPANISH database not defined!"
  	#endif
  #endif/*KMX_LDB_SPANISH*/
  
  #if defined(KMX_LDB_SWAHILI)
  	#if KMX_LDB_SWAHILI == 550
	  	#include "ldb_swahili_550.h"
		const void *kmx_ldb_swahili_550 = &LDB_550;
		const IIME_IME_INFO IIME_ldb_swahili_550 = {IM_SWAHILI, &LDB_550};
  	#elif KMX_LDB_SWAHILI == 555
	  	#include "ldb_swahili_555.h"
		const void *kmx_ldb_swahili_555 = &LDB_555;
		const IIME_IME_INFO IIME_ldb_swahili_555 = {IM_SWAHILI, &LDB_555};
  	#elif KMX_LDB_SWAHILI == 557
	  	#include "ldb_swahili_557.h"
		const void *kmx_ldb_swahili_557 = &LDB_557;
		const IIME_IME_INFO IIME_ldb_swahili_557 = {IM_SWAHILI, &LDB_557};		
  	#else
	  	#error "Suretype SWAHILI database not defined!"
  	#endif
  #endif/*KMX_LDB_SWAHILI*/
  
  #if defined(KMX_LDB_SWEDISH)
  	#if KMX_LDB_SWEDISH == 490
	  	#include "ldb_swedish_490.h"
		const void *kmx_ldb_swedish_490 = &LDB_490;
		const IIME_IME_INFO IIME_ldb_swedish_490 = {IM_SWEDISH, &LDB_490};
  	#elif KMX_LDB_SWEDISH == 495
	  	#include "ldb_swedish_495.h"
		const void *kmx_ldb_swedish_495 = &LDB_495;
		const IIME_IME_INFO IIME_ldb_swedish_495 = {IM_SWEDISH, &LDB_495};
  	#elif KMX_LDB_SWEDISH == 497
	  	#include "ldb_swedish_497.h"
		const void *kmx_ldb_swedish_497 = &LDB_497;
		const IIME_IME_INFO IIME_ldb_swedish_497 = {IM_SWEDISH, &LDB_497};	
  	#else
	  	#error "Suretype SWEDISH database not defined!"
  	#endif
  #endif/*KMX_LDB_SWEDISH*/

  #if defined(KMX_LDB_THAI)
  	#if KMX_LDB_THAI == 360
	  	#include "ldb_thai_360.h"
		const void *kmx_ldb_thai_360 = &LDB_360;
		const IIME_IME_INFO IIME_ldb_thai_360 = {IM_THAI, &LDB_360};
  	#elif KMX_LDB_THAI == 361
		#include "ldb_thai_361.h"
  		const void *kmx_ldb_thai_361 = &LDB_361;
  		const IIME_IME_INFO IIME_ldb_thai_361 = {IM_THAI, &LDB_361};
  	#elif KMX_LDB_THAI == 362
		#include "ldb_thai_362.h"
		const void *kmx_ldb_thai_362 = &LDB_362;
		const IIME_IME_INFO IIME_ldb_thai_362 = {IM_THAI, &LDB_362};
	#elif KMX_LDB_THAI == 363
		#include "ldb_thai_363.h"
		const void *kmx_ldb_thai_363 = &LDB_363;
		const IIME_IME_INFO IIME_ldb_thai_363 = {IM_THAI, &LDB_363};
	#elif KMX_LDB_THAI == 364
		#include "ldb_thai_364.h"
		const void *kmx_ldb_thai_364 = &LDB_364;
		const IIME_IME_INFO IIME_ldb_thai_364 = {IM_THAI, &LDB_364};
	#elif KMX_LDB_THAI == 365
		#include "ldb_thai_365.h"
		const void *kmx_ldb_thai_365 = &LDB_365;
		const IIME_IME_INFO IIME_ldb_thai_365 = {IM_THAI, &LDB_365};
	#elif KMX_LDB_THAI == 366
		#include "ldb_thai_366.h"
		const void *kmx_ldb_thai_366 = &LDB_366;
		const IIME_IME_INFO IIME_ldb_thai_366 = {IM_THAI, &LDB_366};
	#elif KMX_LDB_THAI == 367
		#include "ldb_thai_367.h"
		const void *kmx_ldb_thai_367 = &LDB_367;
		const IIME_IME_INFO IIME_ldb_thai_367 = {IM_THAI, &LDB_367};
	#elif KMX_LDB_THAI == 3671
		#include "ldb_thai_3671.h"
		const void *kmx_ldb_thai_3671 = &LDB_3671;
		const IIME_IME_INFO IIME_ldb_thai_3671 = {IM_THAI, &LDB_3671};
	#elif KMX_LDB_THAI == 368
		#include "ldb_thai_368.h"
		const void *kmx_ldb_thai_368 = &LDB_368;
		const IIME_IME_INFO IIME_ldb_thai_368 = {IM_THAI, &LDB_368};
	#elif KMX_LDB_THAI == 369
		#include "ldb_thai_369.h"
		const void *kmx_ldb_thai_369 = &LDB_369;
		const IIME_IME_INFO IIME_ldb_thai_369 = {IM_THAI, &LDB_369};
	#elif KMX_LDB_THAI == 3600
		#include "ldb_thai_3600.h"
		const void *kmx_ldb_thai_3600 = &LDB_3600;
		const IIME_IME_INFO IIME_ldb_thai_3600 = {IM_THAI, &LDB_3600};
	#elif KMX_LDB_THAI == 3601
		#include "ldb_thai_3601.h"
		const void *kmx_ldb_thai_3601 = &LDB_3601;
		const IIME_IME_INFO IIME_ldb_thai_3601 = {IM_THAI, &LDB_3601};
	#elif KMX_LDB_THAI == 3602
		#include "ldb_thai_3602.h"
		const void *kmx_ldb_thai_3602 = &LDB_3602;
		const IIME_IME_INFO IIME_ldb_thai_3602 = {IM_THAI, &LDB_3602};
	#elif KMX_LDB_THAI == 3603
		#include "ldb_thai_3603.h"
		const void *kmx_ldb_thai_3603 = &LDB_3603;
		const IIME_IME_INFO IIME_ldb_thai_3603 = {IM_THAI, &LDB_3603};
	#elif KMX_LDB_THAI == 3604
		#include "ldb_thai_3604.h"
		const void *kmx_ldb_thai_3604 = &LDB_3604;
		const IIME_IME_INFO IIME_ldb_thai_3604 = {IM_THAI, &LDB_3604};
	#elif KMX_LDB_THAI == 3606
		#include "ldb_thai_3606.h"
		const void *kmx_ldb_thai_3606 = &LDB_3606;
		const IIME_IME_INFO IIME_ldb_thai_3606 = {IM_THAI, &LDB_3606};
	#elif KMX_LDB_THAI == 3607
		#include "ldb_thai_3607.h"
		const void *kmx_ldb_thai_3607 = &LDB_3607;
		const IIME_IME_INFO IIME_ldb_thai_3607 = {IM_THAI, &LDB_3607};
	#elif KMX_LDB_THAI == 3608
		#include "ldb_thai_3608.h"
		const void *kmx_ldb_thai_3608 = &LDB_3608;
		const IIME_IME_INFO IIME_ldb_thai_3608 = {IM_THAI, &LDB_3608};
	#elif KMX_LDB_THAI == 3611
		#include "ldb_thai_3611.h"
		const void *kmx_ldb_thai_3611 = &LDB_3611;
		const IIME_IME_INFO IIME_ldb_thai_3611 = {IM_THAI, &LDB_3611};
	#elif KMX_LDB_THAI == 3616
		#include "ldb_thai_3616.h"
		const void *kmx_ldb_thai_3616 = &LDB_3616;
		const IIME_IME_INFO IIME_ldb_thai_3616 = {IM_THAI, &LDB_3616};
	#elif KMX_LDB_THAI == 3640
		#include "ldb_thai_3640.h"
		const void *kmx_ldb_thai_3640 = &LDB_3640;
		const IIME_IME_INFO IIME_ldb_thai_3640 = {IM_THAI, &LDB_3640};
	#elif KMX_LDB_THAI == 3632
		#include "ldb_thai_3632.h"
		const void *kmx_ldb_thai_3632 = &LDB_3632;
		const IIME_IME_INFO IIME_ldb_thai_3632 = {IM_THAI, &LDB_3632};
  	#else
	  	#error "Suretype THAI database not defined!"
  	#endif
  #endif/*KMX_LDB_THAI*/

  
  #ifdef KMX_LDB_TIBETAN
  #include "ldb_tibetan_730.h"
  const void *kmx_ldb_tibetan_730 = &LDB_730;
  const IIME_IME_INFO IIME_ldb_tibetan_730 = {IM_TIBETAN, &LDB_730};
  #endif/*KMX_LDB_TIBETAN*/
  
  #if defined(KMX_LDB_TURKISH)
  	#if KMX_LDB_TURKISH == 330
	  	#include "ldb_turkish_330.h"
		const void *kmx_ldb_turkish_330 = &LDB_330;
		const IIME_IME_INFO IIME_ldb_turkish_330 = {IM_TURKISH, &LDB_330};
  	#elif KMX_LDB_TURKISH == 335
	  #include "ldb_turkish_335.h"
		const void *kmx_ldb_turkish_335 = &LDB_335;
		const IIME_IME_INFO IIME_ldb_turkish_335 = {IM_TURKISH, &LDB_335};
  	#elif KMX_LDB_TURKISH == 337
		#include "ldb_turkish_337.h"
		const void *kmx_ldb_turkish_337 = &LDB_337;
		const IIME_IME_INFO IIME_ldb_turkish_337 = {IM_TURKISH, &LDB_337};
  	#elif KMX_LDB_TURKISH == 3371
	  	#include "ldb_turkish_3371.h"
		const void *kmx_ldb_turkish_3371 = &LDB_3371;
		const IIME_IME_INFO IIME_ldb_turkish_3371 = {IM_TURKISH, &LDB_3371};		
 	#else
	  	#error "Suretype TURKISH database not defined!"
  	#endif
  #endif/*KMX_LDB_TURKISH*/
  
  #if defined(KMX_LDB_UKRAINIAN)
  	#if KMX_LDB_UKRAINIAN == 540
	  	#include "ldb_ukrainian_540.h"
		const void *kmx_ldb_ukrainian_540 = &LDB_540;
		const IIME_IME_INFO IIME_ldb_ukrainian_540 = {IM_UKRAINIAN, &LDB_540};
  	#elif KMX_LDB_UKRAINIAN == 545
		#include "ldb_ukrainian_545.h"
		const void *kmx_ldb_ukrainian_545 = &LDB_545;
		const IIME_IME_INFO IIME_ldb_ukrainian_545 = {IM_UKRAINIAN, &LDB_545};
  	#elif KMX_LDB_UKRAINIAN == 547
	  	#include "ldb_ukrainian_547.h"
		const void *kmx_ldb_ukrainian_547 = &LDB_547;
		const IIME_IME_INFO IIME_ldb_ukrainian_547 = {IM_UKRAINIAN, &LDB_547};
  	#elif KMX_LDB_UKRAINIAN == 548
	  	#include "ldb_ukrainian_548.h"
		const void *kmx_ldb_ukrainian_548 = &LDB_548;
		const IIME_IME_INFO IIME_ldb_ukrainian_548 = {IM_UKRAINIAN, &LDB_548};		
  	#else
	  	#error "Suretype UKRAINIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_UKRAINIAN*/
  
  #if defined(KMX_LDB_UYGHUR)
  	#if KMX_LDB_UYGHUR == 540
	  	#include "ldb_uyghur_940.h"
		const void *kmx_ldb_uyghur_940 = &LDB_940;
		const IIME_IME_INFO IIME_ldb_uyghur_940 = {IM_UYGHUR, &LDB_940};
  	#elif KMX_LDB_UYGHUR == 545
	  	#include "ldb_uyghur_941.h"
		const void *kmx_ldb_uyghur_941 = &LDB_941;
		const IIME_IME_INFO IIME_ldb_uyghur_941 = {IM_UYGHUR, &LDB_941};
  	#elif KMX_LDB_UYGHUR == 547
	  	#include "ldb_uyghur_947.h"
		const void *kmx_ldb_uyghur_947 = &LDB_947;
		const IIME_IME_INFO IIME_ldb_uyghur_947 = {IM_UYGHUR, &LDB_947};
  	#elif KMX_LDB_UYGHUR == 548
	  	#include "ldb_uyghur_948.h"
		const void *kmx_ldb_uyghur_948 = &LDB_948;
		const IIME_IME_INFO IIME_ldb_uyghur_948 = {IM_UYGHUR, &LDB_948};	
  	#else
	  	#error "Suretype UYGHUR database not defined!"
  	#endif
  #endif/*KMX_LDB_UYGHUR*/

  #if defined(KMX_LDB_VIETNAMESE)
  	#if KMX_LDB_VIETNAMESE == 310
	  	#include "ldb_vietnamese_310.h"
		const void *kmx_ldb_vietnamese_310 = &LDB_310;
		const IIME_IME_INFO IIME_ldb_vietnamese_310 = {IM_VIETNAMESE, &LDB_310};
  	#elif KMX_LDB_VIETNAMESE == 315
	  	#include "ldb_vietnamese_315.h"
		const void *kmx_ldb_vietnamese_315 = &LDB_315;
		const IIME_IME_INFO IIME_ldb_vietnamese_315 = {IM_VIETNAMESE, &LDB_315};
  	#elif KMX_LDB_VIETNAMESE == 317
	  	#include "ldb_vietnamese_317.h"
		const void *kmx_ldb_vietnamese_317 = &LDB_317;
		const IIME_IME_INFO IIME_ldb_vietnamese_317 = {IM_VIETNAMESE, &LDB_317};
  	#elif KMX_LDB_VIETNAMESE == 3171
	  	#include "ldb_vietnamese_3171.h"
		const void *kmx_ldb_vietnamese_3171 = &LDB_3171;
		const IIME_IME_INFO IIME_ldb_vietnamese_3171 = {IM_VIETNAMESE, &LDB_3171};	
  	#else
	  	#error "Suretype VIETNAMESE database not defined!"
  	#endif
  #endif/*KMX_LDB_VIETNAMESE*/

  #if defined(KMX_LDB_URDU)
  	#if KMX_LDB_URDU == 950
	  	#include "ldb_urdu_950.h"
		const void *kmx_ldb_urdu_950 = &LDB_950;
		const IIME_IME_INFO IIME_ldb_urdu_950 = {IM_URDU, &LDB_950};
  	#elif KMX_LDB_URDU == 955
	  	#include "ldb_urdu_955.h"
		const void *kmx_ldb_urdu_955 = &LDB_955;
		const IIME_IME_INFO IIME_ldb_urdu_955 = {IM_URDU, &LDB_955};
  	#elif KMX_LDB_URDU == 957
	  	#include "ldb_urdu_957.h"
		const void *kmx_ldb_urdu_957 = &LDB_957;
		const IIME_IME_INFO IIME_ldb_urdu_957 = {IM_URDU, &LDB_957};
  	#elif KMX_LDB_URDU == 9571
	  	#include "ldb_urdu_9571.h"
		const void *kmx_ldb_urdu_9571 = &LDB_9571;
		const IIME_IME_INFO IIME_ldb_urdu_9571 = {IM_URDU, &LDB_9571};
  	#else
	  	#error "Suretype URDU database not defined!"
  	#endif
  #endif/*KMX_LDB_URDU*/

  
  #ifdef KMX_LDB_TAGALOG
  #include "ldb_tagalog_587.h"
  const void *kmx_ldb_tagalog_587 = &LDB_587;
  const IIME_IME_INFO IIME_ldb_tagalog_587 = {IM_TAGALOG, &LDB_587};
  #endif/*KMX_LDB_TAGALOG*/
  
  #ifdef KMX_LDB_KYRGYZ
  #include "ldb_kyrgyz_747.h"
  const void *kmx_ldb_kyrgyz_587 = &LDB_747;
  const IIME_IME_INFO IIME_ldb_kyrgyz_587 = {IM_KYRGYZ, &LDB_747};
  #endif/*KMX_LDB_KYRGYZ*/
  
  #ifdef KMX_LDB_ASSAMESE
  #include "ldb_assamese_710.h"
  const void *kmx_ldb_assamese_710 = &LDB_710;
  const IIME_IME_INFO IIME_ldb_assamese_710 = {IM_ASSAMESE, &LDB_710};
  #endif/*KMX_LDB_ASSAMESE*/
  
  #ifdef KMX_LDB_ALBANIAN
  #include "ldb_albanian_611.h"
  const void *kmx_ldb_albanian_611 = &LDB_611;
  const IIME_IME_INFO IIME_ldb_albanian_611 = {IM_ALBANIAN, &LDB_611};
  #endif/*KMX_LDB_ALBANIAN*/
  
  #ifdef KMX_LDB_CATALAN
  #include "ldb_catalan_621.h"
  const void *kmx_ldb_catalan_621 = &LDB_621;
  const IIME_IME_INFO IIME_ldb_catalan_621 = {IM_CATALAN, &LDB_621};
  #endif/*KMX_LDB_CATALAN*/
  
  #ifdef KMX_LDB_BASQUE
  #include "ldb_basque_631.h"
  const void *kmx_ldb_basque_631 = &LDB_631;
  const IIME_IME_INFO IIME_ldb_basque_631 = {IM_BASQUE, &LDB_631};
  #endif/*KMX_LDB_BASQUE*/
  
  #ifdef KMX_LDB_GALICIAN
  #include "ldb_galician_641.h"
  const void *kmx_ldb_galician_641 = &LDB_641;
  const IIME_IME_INFO IIME_ldb_galician_641 = {IM_GALICIAN, &LDB_641};
  #endif/*KMX_LDB_GALICIAN*/
  
  #ifdef KMX_LDB_IGBO
  #include "ldb_igbo_651.h"
  const void *kmx_ldb_igbo_651 = &LDB_651;
  const IIME_IME_INFO IIME_ldb_igbo_651 = {IM_IGBO, &LDB_651};
  #endif/*KMX_LDB_IGBO*/
  
  #ifdef KMX_LDB_OROMO
  #include "ldb_oromo_661.h"
  const void *kmx_ldb_oromo_661 = &LDB_661;
  const IIME_IME_INFO IIME_ldb_oromo_661 = {IM_OROMO, &LDB_661};
  #endif/*KMX_LDB_OROMO*/
  	
  #if defined(KMX_LDB_SERBIAN)
  	#if KMX_LDB_SERBIAN == 233
	  	#include "ldb_serbian_233.h"
		const void *kmx_ldb_serbian_233 = &LDB_233;
		const IIME_IME_INFO IIME_ldb_serbian_233 = {IM_SERBIAN, &LDB_233};
  	#elif KMX_LDB_SERBIAN == 243
	  	#include "ldb_serbianc_243.h"
		const void *kmx_ldb_serbianc_641 = &LDB_243;
		const IIME_IME_INFO IIME_ldb_serbianc_243 = {IM_SERBIANC, &LDB_243};
  	#else
	  	#error "Suretype SERBIAN database not defined!"
  	#endif
  #endif/*KMX_LDB_SERBIAN*/

  
  #ifdef KMX_LDB_ZULU
  #include "ldb_zulu_677.h"
  const void *kmx_ldb_zulu_677 = &LDB_677;
  const IIME_IME_INFO IIME_ldb_zulu_677 = {IM_ZULU, &LDB_677};
  #endif/*KMX_LDB_ZULU*/
  
  #ifdef KMX_LDB_CEBUANO
  #include "ldb_cebuano_687.h"
  const void *kmx_ldb_cebuano_687 = &LDB_687;
  const IIME_IME_INFO IIME_ldb_cebuano_687 = {IM_CEBUANO, &LDB_687};
  #endif/*KMX_LDB_CEBUANO*/
  
  #ifdef KMX_LDB_KAZAKH
  #include "ldb_kazakh_257.h"
  const void *kmx_ldb_kazakh_257 = &LDB_257;
  const IIME_IME_INFO IIME_ldb_kazakh_257 = {IM_KAZAKH, &LDB_257};
  #endif/*KMX_LDB_KAZAKH*/
  
  #ifdef KMX_LDB_UZBEK
  #include "ldb_uzbek_267.h"
  const void *kmx_ldb_uzbek_267 = &LDB_267;
  const IIME_IME_INFO IIME_ldb_uzbek_267 = {IM_UZBEK, &LDB_267};
  #endif/*KMX_LDB_UZBEK*/
  
  #ifdef KMX_LDB_GEORGIAN
  #include "ldb_georgian_277.h"
  const void *kmx_ldb_georgian_277 = &LDB_277;
  const IIME_IME_INFO IIME_ldb_georgian_277 = {IM_GEORGIAN, &LDB_277};
  #endif/*KMX_LDB_GEORGIAN*/
  
  #ifdef KMX_LDB_NEPALI
  #include "ldb_nepali_720.h"
    const void *kmx_ldb_nepali_720 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_nepali_720 = {IM_NEPALI, &LDB_720};
  #endif/*KMX_LDB_NEPALI*/

  //daodejing  need
  #ifdef KMX_LDB_BODO
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_bodo_1100 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_bodo_1100 = {IM_BODO, &LDB_720};
  #endif/*KMX_LDB_BODO*/

  #ifdef KMX_LDB_DOGRI
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_dogri_1110 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_dogri_1110 = {IM_DOGRI, &LDB_720};
  #endif/*KMX_LDB_DOGRI*/

  #ifdef KMX_LDB_SANSKRIT
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_sanskrit_1120 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_sanskrit_1120 = {IM_SANSKRIT, &LDB_720};
  #endif/*KMX_LDB_SANSKRIT*/

  #ifdef KMX_LDB_KASHMIRI
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_kashmiri_1130 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_kashmiri_1130 = {IM_KASHMIRI, &LDB_720};
  #endif/*KMX_LDB_KASHMIRI*/

  #ifdef KMX_LDB_KONKANI
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_konkani_1140 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_konkani_1140 = {IM_KONKANI, &LDB_720};
  #endif/*KMX_LDB_KONKANI*/

  #ifdef KMX_LDB_MAITHILI
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_maithili_1150 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_maithili_1150 = {IM_MAITHILI, &LDB_720};
  #endif/*KMX_LDB_MAITHILI*/

  #ifdef KMX_LDB_MANIPURI
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_manipuri_1160 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_manipuri_1160 = {IM_MANIPURI, &LDB_720};
  #endif/*KMX_LDB_MANIPURI*/

  #ifdef KMX_LDB_SANTALI
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_santali_1170 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_santali_1170 = {IM_SANTALI, &LDB_720};
  #endif/*KMX_LDB_SANTALI*/

  #ifdef KMX_LDB_SOMALI
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_somali_1180 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_somali_1180 = {IM_SOMALI, &LDB_720};
  #endif/*KMX_LDB_SOMALI*/

  #ifdef KMX_LDB_SINDHI
    #include "ldb_nepali_720.h"
    const void *kmx_ldb_sindhi_1190 = &LDB_720;
    const IIME_IME_INFO IIME_ldb_sindhi_1190 = {IM_SINDHI, &LDB_720};
  #endif/*KMX_LDB_SINDHI*/
  
  #ifdef KMX_LDB_SINHALA
  #include "ldb_sinhala_770.h"//IM_SINHALA_SUPPORT
  const IIME_IME_INFO IIME_ldb_sinhala_770 = {IM_SINHALA, &LDB_770};//IM_SINHALA_SUPPORT
  const void *kmx_ldb_sinhala_770 = &LDB_770;//IM_SINHALA_SUPPORT
  #endif/*KMX_LDB_SINHALA*/

  #ifdef KMX_LDB_KHMER
  #include "indic_mt_ldb.h"
  /* Myanmar */
  const void *kmx_ldb_khmer = &dummy_khmer_ldb;
  const IIME_IME_INFO IIME_ldb_khmer = {IM_KHMER, &dummy_khmer_ldb};
  #endif
  
  #ifdef KMX_LDB_MYANMAR
  /* Myanmar */
  static int dummy_myanmar_db = 0;
  const void* kmx_ldb_myanmar = &dummy_myanmar_db;
  const IIME_IME_INFO IIME_ldb_myanmar = {IM_MYANMAR, &dummy_myanmar_db};
  #endif
//end of IEKIE_INPUT_SAME_LANGUAGE_ONE_LDB

#endif/*IIME_ALL_IN_ONE_DATABASE*/

#if defined(IEKIE_INPUT_SAME_LANGUAGE_ONE_LDB)
#include "ldb_english_100.h"
const void *kmx_ldb_english_100 = &LDB_100;
const IIME_IME_INFO IIME_ldb_english_100 = {IM_ENGLISH, &LDB_100};

#include "ldb_english_101.h"
const void *kmx_ldb_english_101 = &LDB_101;
const IIME_IME_INFO IIME_ldb_english_101 = {IM_ENGLISH, &LDB_101};
#endif

const IIME_IME_INFO_PTR IIME_input_method_def[] =
{
    &IIME_ldb_english_100,
	&IIME_ldb_english_101
};

IIME_BOOL checkLdbDef(){
	int i=0;
	LDB* pdb;
	int size = sizeof(IIME_input_method_def)/sizeof(IIME_IME_INFO_PTR);
	if(size <= 0) return IIME_FALSE;
	for(i=0;i<size;i++)
	{
		if(IIME_input_method_def[i]&&IIME_input_method_def[i]->language_database)
		{
			pdb = IIME_input_method_def[i]->language_database;
			if(sizeof(pdb->word_table)<=0)
			{
				return IIME_FALSE;
			}
		}
	}
 	return IIME_TRUE;
}

