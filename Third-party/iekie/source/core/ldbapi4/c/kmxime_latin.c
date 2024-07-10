
/***************************************************************************
 *                                                                         *
 * Copyright (C) 2000-2007 Beijing Komoxo Inc.                             *
 * All rights reserved.                                                    *
 *                                                                         *
 ***************************************************************************/

#include "IIMEstd.h"

#include "kmxime_latin.h"
#include "indic_mt.h"

/****************************************************************************
 *
 * User database structure.
 *
 ***************************************************************************/

typedef struct
{
    int                     size;

    int                     did;
    int                     major_version;
    int                     minor_version;
    int                     build_number;

    LDB_WID                 wid_oov;
    LDB_WID                 wid_sos;

    int                     check;

    int                     hpos;
    int                     hsize;
    int                     hlast;

    int                     opos;
    int                     osize;
    int                     olast;
} UDB;

typedef struct
{
    LDB_WORD                prev;   /* Size of previous item */
    LDB_WORD                next;   /* Size of current item */
    LDB_WID                 wid;
    LDB_WORD                len;
} UDB_HIS;

typedef struct
{
    LDB_WORD                prev;   /* Size of previous item */
    LDB_WORD                next;   /* Size of current item */
} UDB_OOV;

/* Minimum byte size of cache database */
#define UDB_MIN             1024

/* Maximum byte size of cache database */
#define UDB_MAX             8192

/* Iterator for history items */
#define UDB_HIT             int

/* Iterator for OOV items */
#define UDB_OIT             int

#define USIZE               ((int)sizeof(UDB))
#define HSIZE               ((int)sizeof(UDB_HIS))
#define OSIZE               ((int)sizeof(UDB_OOV))
#define IIMEIZE               ((int)sizeof(LDB_CHAR))

/****************************************************************************
 *
 * Match engine structure.
 *
 ***************************************************************************/

/* Constants for MATCH.rank */

#define RANK_DELTA          10240

#define RANK_TOP            0
#define RANK_PREFIX         (RANK_DELTA * 5)

/* Exact match rank level */

#define RANK_OOVT           (RANK_DELTA * 20)
#define RANK_CACT           (RANK_DELTA * 20)
#define RANK_DICT           (RANK_DELTA * 30)

#define RANK_OOVB           (RANK_DELTA * 50)
#define RANK_CACB           (RANK_DELTA * 50)
#define RANK_DICB           (RANK_DELTA * 60)

#define RANK_OOVU           (RANK_DELTA * 80)
#define RANK_CACU           (RANK_DELTA * 80)
#define RANK_DICU           (RANK_DELTA * 90)

/* Forward match rank level */

#define RANK_FOOVT          (RANK_DELTA * 110)
#define RANK_FCACT          (RANK_DELTA * 110)
#define RANK_FDICT          (RANK_DELTA * 120)

#define RANK_FOOVB          (RANK_DELTA * 140)
#define RANK_FCACB          (RANK_DELTA * 140)
#define RANK_FDICB          (RANK_DELTA * 150)

#define RANK_FOOVU          (RANK_DELTA * 170)
#define RANK_FCACU          (RANK_DELTA * 170)
#define RANK_FDICU          (RANK_DELTA * 180)

/* Template rank level */

#define RANK_TOOVT          (RANK_DELTA * 200)
#define RANK_TCACT          (RANK_DELTA * 200)
#define RANK_TDICT          (RANK_DELTA * 210)

#define RANK_TOOVB          (RANK_DELTA * 230)
#define RANK_TCACB          (RANK_DELTA * 230)
#define RANK_TDICB          (RANK_DELTA * 240)

#define RANK_TOOVU          (RANK_DELTA * 260)
#define RANK_TCACU          (RANK_DELTA * 260)
#define RANK_TDICU          (RANK_DELTA * 270)

#define RANK_LEGRAM         (RANK_DELTA * 280)

#define RANK_BOTTOM         (RANK_DELTA * 290)

typedef struct
{
    int                     rank;
    int                     length;
    LDB_CHAR                string[MAX_WORD_LENGTH];
} MATCH;

#define MAX_MATCH           32

#define FORWARD_MATCH       5

#define ENGINE_SIGNATURE    0x4C4C4C4C  /* "LLLL" */

#define MATCH_TYPE_WORD     0
#define MATCH_TYPE_ASSO     1

#define MAX_CHAR_CODE_COUNT     128
#define MAX_ASSO_KEY_COUNT       10

typedef struct
{
    LDB_WORD                type;
    LDB_WID                 wid1;
    LDB_WID                 wid2;
    LDB_WORD                flags;
    LDB_WORD                max_match_length;
    LDB_WORD                length;
    LDB_CHAR                string[MAX_WORD_LENGTH];
} MATCH_INPUT;

typedef struct
{
    unsigned int            signature;

    MATCH_INPUT             input;

    int                     did;
    int                     major_version;
    int                     minor_version;
    int                     build_number;

    const LDB_CHAR*         alphabet;
    int                     alphabet_length;

    const LDB_CHAR*         lc2ucm;
    int                     lc2ucm_length;

    const LDB_CHAR*         uc2lcm;
    int                     uc2lcm_length;

    const LDB_CHAR*         sos_table;
    int                     sos_table_length;

    const LDB_CHAR*         sou_table;
    int                     sou_table_length;

    const LDB_WORD*         mt_index;
    const LDB_CHAR*         mt_string;
    int                     mt_string_length;

    const LDB_WORD*         legram_index;
    const LDB_CHAR*         legram_string;
    int                     legram_string_length;

    int                     word_count;
    int                     word_id_bits;
    LDB_WID                 wid_oov;
    LDB_WID                 wid_sos;

    const LDB_BYTE*         word_index;
    int                     max_word_length;

    const LDB_BYTE*         word_table;
    int                     word_level_bits;
    int                     word_index_bits;

    int                     char_code_count;
    int                     char_code_bits;

    const LDB_CHAR*         char_table;
    const LDB_BYTE*         ckey_table;

    const LDB_BYTE*         string_table;
    int                     string_table_length;

    const LDB_BYTE*         bigram_index;
    int                     bigram_index_count;
    int                     bigram_index_bits;
    const LDB_BYTE*         bigram;
    int                     bigram_count;

    const LDB_BYTE*         trigram;
    int                     trigram_count;

    const LDB_BYTE*         exgram_index;
    int                     exgram_index_count;
    int                     exgram_index_bits;
    const LDB_BYTE*         exgram;

    UDB*                    udb;

    int                     is_sure_type;
    int                     language;

    /* Bigram search cache */
    int                     bc_wid;
    int                     bc_start;
    int                     bc_end;

    /* Trigram search cache */
    int                     tc_wid1;
    int                     tc_wid2;
    int                     tc_start;
    int                     tc_end;

    /* Exgram search cache */
    int                     ec_wid;
    int                     ec_start;
    int                     ec_end;

    /* Template candidate */
    int                     tmp_rank;
    int                     tmp_len;
    LDB_CHAR                tmp_str[MAX_WORD_LENGTH];

    int                     is_exgram_found;
    int                     max_match;
    int                     match_count;
    MATCH                   match_list[MAX_MATCH];

    MATCH_INPUT             cache_input;
    int                     cache_valid;

    LDB_CHAR                buffer1[MAX_WORD_LENGTH];
    LDB_CHAR                buffer2[MAX_WORD_LENGTH];

    /* Key count */
    int                     key_count;
    int                     flags;    

    /* Letter group  */   
    LDB_BYTE                group_id_table[MAX_CHAR_CODE_COUNT];
    int                     group_count;
    
} ENGINE;

static const LDB_BYTE BASIC_LATIN_9KEY_MAP[26] =
{
    2, 2, 2,                /* a b c   */
    3, 3, 3,                /* d e f   */
    4, 4, 4,                /* g h i   */
    5, 5, 5,                /* j k l   */
    6, 6, 6,                /* m n o   */
    7, 7, 7, 7,             /* p q r s */
    8, 8, 8,                /* t u v   */
    9, 9, 9, 9              /* w x y z */
};

static const LDB_BYTE BASIC_LATIN_SURE_TYPE_MAP[26] =
{
    11, 8, 7,                /* a b c   */
    4, 1, 4,                 /* d e f   */
    5, 5, 3,                 /* g h i   */
    6, 6, 14,                /* j k l   */
    9, 8, 13,                /* m n o   */
    13, 10, 1, 11,           /* p q r s */
    2, 3, 7,                 /* t u v   */
    10, 12, 2, 12            /* w x y z */
};

/****************************************************************************
 *
 * Character code constants.
 *
 ***************************************************************************/

#define KMX_CHAR_SPACE              0x20
#define KMX_CHAR_APOSTROPHE         0x27
#define KMX_CHAR_COMMA              0x2C
#define KMX_CHAR_FULL_STOP          0x2E

#define KMX_CHAR_DIGIT_0            0x30
#define KMX_CHAR_DIGIT_1            0x31
#define KMX_CHAR_DIGIT_2            0x32
#define KMX_CHAR_DIGIT_3            0x33
#define KMX_CHAR_DIGIT_4            0x34
#define KMX_CHAR_DIGIT_5            0x35
#define KMX_CHAR_DIGIT_6            0x36
#define KMX_CHAR_DIGIT_7            0x37
#define KMX_CHAR_DIGIT_8            0x38
#define KMX_CHAR_DIGIT_9            0x39

#define KMX_CHAR_ARABIC_DIGIT_0     0x0660
#define KMX_CHAR_ARABIC_DIGIT_9     0x0669

#define KMX_CHAR_PERSIAN_DIGIT_0    0x06F0
#define KMX_CHAR_PERSIAN_DIGIT_9    0x06F9

#define KMX_CHAR_HINDI_DIGIT_0      0x0966
#define KMX_CHAR_HINDI_DIGIT_9      0x096F

#define KMX_CHAR_NEPALI_DIGIT_0      0x0966
#define KMX_CHAR_NEPALI_DIGIT_9      0x096F

#define KMX_CHAR_SINHALA_DIGIT_0      0x0966
#define KMX_CHAR_SINHALA_DIGIT_9      0x096F

//daodejing need
#define KMX_CHAR_BODO_DIGIT_0      0x0966
#define KMX_CHAR_BODO_DIGIT_9      0x096F

#define KMX_CHAR_DOGRI_DIGIT_0      0x0966
#define KMX_CHAR_DOGRI_DIGIT_9      0x096F

#define KMX_CHAR_SANSKRIT_DIGIT_0      0x0966
#define KMX_CHAR_SANSKRIT_DIGIT_9      0x096F

#define KMX_CHAR_KASHMIRI_DIGIT_0      0x0966
#define KMX_CHAR_KASHMIRI_DIGIT_9      0x096F

#define KMX_CHAR_KONKANI_DIGIT_0      0x0966
#define KMX_CHAR_KONKANI_DIGIT_9      0x096F

#define KMX_CHAR_MAITHILI_DIGIT_0      0x0966
#define KMX_CHAR_MAITHILI_DIGIT_9      0x096F

#define KMX_CHAR_MANIPURI_DIGIT_0      0x0966
#define KMX_CHAR_MANIPURI_DIGIT_9      0x096F

#define KMX_CHAR_SANTALI_DIGIT_0      0x0966
#define KMX_CHAR_SANTALI_DIGIT_9      0x096F

#define KMX_CHAR_SOMALI_DIGIT_0      0x0966
#define KMX_CHAR_SOMALI_DIGIT_9      0x096F

#define KMX_CHAR_SINDHI_DIGIT_0      0x0966
#define KMX_CHAR_SINDHI_DIGIT_9      0x096F

#define KMX_CHAR_BENGALI_DIGIT_0    0x09E6
#define KMX_CHAR_BENGALI_DIGIT_9    0x09EF

#define KMX_CHAR_THAI_DIGIT_0       0x0E50
#define KMX_CHAR_THAI_DIGIT_9       0x0E59

#define KMX_9KEY_FIRST_KEY          KMX_CHAR_DIGIT_0
#define KMX_9KEY_LAST_KEY           (KMX_9KEY_FIRST_KEY + 9)
#define KMX_9KEY_STAR_KEY           ('*')
#define KMX_9KEY_POUND_KEY           ('#')
#define IS_VALID_9KEY_INPUT(c)      ( (c) >= KMX_9KEY_FIRST_KEY && (c) <= KMX_9KEY_LAST_KEY )

#define KMX_LDB_KEY_STAR            10
#define KMX_LDB_KEY_POUND          11

#define KMX_SURE_TYPE_FIRST_KEY     KMX_CHAR_DIGIT_0
#define KMX_SURE_TYPE_LAST_KEY      (KMX_SURE_TYPE_FIRST_KEY + 15)
#define IS_VALID_SURE_TYPE_INPUT(c) ( (c) >= KMX_SURE_TYPE_FIRST_KEY && (c) <= KMX_SURE_TYPE_LAST_KEY )

#define VALIDATE(exp)           \
    if (!(exp)) return KMX_FALSE

/****************************************************************************
 *
 * Helper functions.
 *
 ***************************************************************************/

/* CRC_BASE is the largest prime smaller than 65536 */
#define CRC_BASE            65521UL

/* CRC_NMAX is the largest n such that 255n(n+1)/2 + (n+1)(CRC_BASE-1) <= 2^32-1 */
#define CRC_NMAX            5552

#define CRC_DO1(i)          { s1 += b[i]; s2 += s1; }
#define CRC_DO2(i)          CRC_DO1(i); CRC_DO1(i+1);
#define CRC_DO4(i)          CRC_DO2(i); CRC_DO2(i+2);
#define CRC_DO8(i)          CRC_DO4(i); CRC_DO4(i+4);
#define CRC_DO16            CRC_DO8(0); CRC_DO8(8);

static int crc(
    IN const void*          buf,
    IN int                  len)
{
    int                     k;
    const unsigned char*    b;
    unsigned long           s1;
    unsigned long           s2;

    s1 = 1;
    s2 = 0;
    b = (const unsigned char*)buf;
    while (len > 0)
    {
        k = len < CRC_NMAX ? len : CRC_NMAX;
        len -= k;
        while (k >= 16)
        {
            CRC_DO16;
            b += 16;
            k -= 16;
        }
        while (k)
        {
            s1 += *b++;
            s2 += s1;
            k--;
        }
        s1 %= CRC_BASE;
        s2 %= CRC_BASE;
    }

    return (s2 << 16) | s1;
}

static int is_aligned(
    IN int                  value,
    IN int                  unit)
{
    KMX_ASSERT(value >= 0);
    KMX_ASSERT(unit > 0);

    return ((value % unit) == 0) ? KMX_TRUE : KMX_FALSE;
}

static int align_adjust(
    IN int                  value,
    IN int                  unit)
{
    int                     test;

    KMX_ASSERT(value >= 0);
    KMX_ASSERT(unit > 0);

    test = value % unit;

    return (test > 0) ? (value + (unit - test)) : value;
}

static int read_bits(
    IN const LDB_BYTE*      data,
    IN int                  offset,
    IN int                  bits)
{
    static const int BITS_MASK[20] =
    {
        0x00000000, 0x00000001, 0x00000003, 0x00000007,
        0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F,
        0x000000FF, 0x000001FF, 0x000003FF, 0x000007FF,
        0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF,
        0x0000FFFF, 0x0001FFFF, 0x0003FFFF, 0x0007FFFF
    };

    KMX_ASSERT(data);
    KMX_ASSERT(offset >= 0);
    KMX_ASSERT( (bits > 0) && (bits <= 19) );

    data += offset / 8;

    if (bits == 1)
        return (data[0] >> (offset % 8)) & 1;
    else if (bits <= 9)
        return (((data[1] << 8) | data[0]) >> (offset % 8)) & BITS_MASK[bits];
    else
        return (((data[2] << 16) | (data[1] << 8) | data[0]) >> (offset % 8)) & BITS_MASK[bits];
}

static int sorted_strchr(
    IN const LDB_CHAR*      str,
    IN int                  len,
    IN LDB_CHAR             c)
{
    if ( (str != 0) && (len > 0) )
    {
        int s;
        int e;

        KMX_ASSERT(str);

        s = 0;
        e = len - 1;

        while (s <= e)
        {
            int i;

            i = (s + e) / 2;
            if (c < str[i])
                e = i - 1;
            else if (c > str[i])
                s = i + 1;
            else /* c == str[i] */
                return i;
        }
    }

    return -1;
}

/****************************************************************************
 *
 * Thai functions.
 *
 ***************************************************************************/

/* Thai character types */

#define LDB_THAI_OOV        0x01    /* None-thai */
#define LDB_THAI_IDV        0x02    /* Independent vowels */
#define LDB_THAI_C          0x04    /* Consonants */
#define LDB_THAI_DV         0x08    /* Dependent vowels */
#define LDB_THAI_TM         0x10    /* Tone marks */
#define LDB_THAI_S          0x20    /* Signs */
#define LDB_THAI_JS         0x40    /* Joined Signs */

static unsigned char get_thai_char_type(
    IN LDB_CHAR             c)
{
    if (c >= 0x0E01 && c <= 0x0E2E)
        return LDB_THAI_C;

    if (c >= 0x0E48 && c <= 0x0E4C)
        return LDB_THAI_TM;

    if (c == 0x0E2F || c == 0x0E46)
        return LDB_THAI_S;

    if (c == 0x0E30 || c == 0x0E32 || (c >= 0x0E40 && c <= 0x0E45))
        return LDB_THAI_IDV;

    if (c == 0x0E31 || c == 0x0E47 || (c >= 0x0E34 && c <= 0x0E3A))
        return LDB_THAI_DV;

    if (c >= 0x0E4C && c <= 0x0E4E)
    {
        return LDB_THAI_JS;
    }

    return LDB_THAI_OOV;
}

int kmx_is_valid_thai_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    unsigned char t1;
    unsigned char t2;

    t1 = get_thai_char_type(c1);
    t2 = get_thai_char_type(c2);
    
    if (t2 == 0x0E33) 
    {
        if (t1 & (LDB_THAI_OOV | LDB_THAI_S | LDB_THAI_IDV | LDB_THAI_JS))
            return 0;
            
        return 1;
    }

    if (t1 & (LDB_THAI_OOV | LDB_THAI_IDV | LDB_THAI_TM | LDB_THAI_S | LDB_THAI_JS))
    {
        if (t2 & (LDB_THAI_DV | LDB_THAI_TM | LDB_THAI_JS))
            return 0;
    }
    else if (t1 == LDB_THAI_DV && t2 == LDB_THAI_DV)
    {
            return 0;
    }

    return 1;
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Hindi functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Hindi character types */

#define LDB_HINDI_OOV       0       /* Not a Hindi letter   */
#define LDB_HINDI_CON       1       /* Consonants           */
#define LDB_HINDI_INV       2       /* Independent vowels   */
#define LDB_HINDI_DEV       3       /* Dependent vowels     */
#define LDB_HINDI_VIR       4       /* Virama sign          */
#define LDB_HINDI_SIG       5       /* Other signs          */

#define H_OOV  LDB_HINDI_OOV
#define H_CON  LDB_HINDI_CON
#define H_INV  LDB_HINDI_INV
#define H_DEV  LDB_HINDI_DEV
#define H_VIR  LDB_HINDI_VIR
#define H_SIG  LDB_HINDI_SIG

static int get_hindi_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char HINDI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ H_OOV, H_SIG, H_SIG, H_SIG, H_INV, H_INV, H_INV, H_INV, H_INV, H_INV, H_INV, H_INV, H_INV, H_INV, H_INV, H_INV,
    /* 0x0910 - 0x091F */ H_INV, H_INV, H_INV, H_INV, H_INV, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON,
    /* 0x0920 - 0x092F */ H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON,
    /* 0x0930 - 0x093F */ H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_OOV, H_OOV, H_VIR, H_OOV, H_DEV, H_DEV,
    /* 0x0940 - 0x094F */ H_DEV, H_DEV, H_DEV, H_DEV, H_DEV, H_DEV, H_DEV, H_DEV, H_DEV, H_DEV, H_DEV, H_DEV, H_DEV, H_VIR, H_OOV, H_OOV,
    /* 0x0950 - 0x095F */ H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON, H_CON,
    /* 0x0960 - 0x096F */ H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV,
    /* 0x0970 - 0x097F */ H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV, H_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return HINDI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_HINDI_OOV;
}

int kmx_is_valid_hindi_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char HINDI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return HINDI_CHAR_START_TABLE[get_hindi_char_type(c)];
}

int kmx_is_valid_hindi_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char HINDI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return HINDI_CHAR_FOLLOW_TABLE[get_hindi_char_type(c1) * 6 + get_hindi_char_type(c2)];
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Nepali functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Nepali character types */

#define LDB_NEPALI_OOV       0       /* Not a Nepali letter   */
#define LDB_NEPALI_CON       1       /* Consonants           */
#define LDB_NEPALI_INV       2       /* Independent vowels   */
#define LDB_NEPALI_DEV       3       /* Dependent vowels     */
#define LDB_NEPALI_VIR       4       /* Virama sign          */
#define LDB_NEPALI_SIG       5       /* Other signs          */

#define N_OOV  LDB_NEPALI_OOV
#define N_CON  LDB_NEPALI_CON
#define N_INV  LDB_NEPALI_INV
#define N_DEV  LDB_NEPALI_DEV
#define N_VIR  LDB_NEPALI_VIR
#define N_SIG  LDB_NEPALI_SIG

static int get_nepali_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char NEPALI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return NEPALI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_NEPALI_OOV;
}

int kmx_is_valid_nepali_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char NEPALI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return NEPALI_CHAR_START_TABLE[get_nepali_char_type(c)];
}

int kmx_is_valid_nepali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char NEPALI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return NEPALI_CHAR_FOLLOW_TABLE[get_nepali_char_type(c1) * 6 + get_nepali_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Sinhala functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Sinhala character types */

#define LDB_SINHALA_OOV       0       /* Not a Sinhala letter   */
#define LDB_SINHALA_CON       1       /* Consonants           */
#define LDB_SINHALA_INV       2       /* Independent vowels   */
#define LDB_SINHALA_DEV       3       /* Dependent vowels     */
#define LDB_SINHALA_VIR       4       /* Virama sign          */
#define LDB_SINHALA_SIG       5       /* Other signs          */

#define S_OOV  LDB_SINHALA_OOV
#define S_CON  LDB_SINHALA_CON
#define S_INV  LDB_SINHALA_INV
#define S_DEV  LDB_SINHALA_DEV
#define S_VIR  LDB_SINHALA_VIR
#define S_SIG  LDB_SINHALA_SIG

static int get_sinhala_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char SINHALA_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ S_OOV, S_SIG, S_SIG, S_SIG, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV, S_INV,
    /* 0x0910 - 0x091F */ S_INV, S_INV, S_INV, S_INV, S_INV, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON,
    /* 0x0920 - 0x092F */ S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON,
    /* 0x0930 - 0x093F */ S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_OOV, S_OOV, S_VIR, S_OOV, S_DEV, S_DEV,
    /* 0x0940 - 0x094F */ S_DEV, S_DEV, S_DEV, S_DEV, S_DEV, S_DEV, S_DEV, S_DEV, S_DEV, S_DEV, S_DEV, S_DEV, S_DEV, S_VIR, S_OOV, S_OOV,
    /* 0x0950 - 0x095F */ S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON, S_CON,
    /* 0x0960 - 0x096F */ S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV,
    /* 0x0970 - 0x097F */ S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV, S_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return SINHALA_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_SINHALA_OOV;
}

int kmx_is_valid_sinhala_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char SINHALA_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SINHALA_CHAR_START_TABLE[get_sinhala_char_type(c)];
}

int kmx_is_valid_sinhala_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char SINHALA_CHAR_FOLLOW_TABLE[36] =
    {
        /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return SINHALA_CHAR_FOLLOW_TABLE[get_sinhala_char_type(c1) * 6 + get_sinhala_char_type(c2)];
}

//daodejing
/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Bodo functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Bodo character types */

#define LDB_BODO_OOV       0       /* Not a Bodo letter   */
#define LDB_BODO_CON       1       /* Consonants           */
#define LDB_BODO_INV       2       /* Independent vowels   */
#define LDB_BODO_DEV       3       /* Dependent vowels     */
#define LDB_BODO_VIR       4       /* Virama sign          */
#define LDB_BODO_SIG       5       /* Other signs          */

#define N_OOV  LDB_BODO_OOV
#define N_CON  LDB_BODO_CON
#define N_INV  LDB_BODO_INV
#define N_DEV  LDB_BODO_DEV
#define N_VIR  LDB_BODO_VIR
#define N_SIG  LDB_BODO_SIG

static int get_bodo_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char BODO_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return BODO_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_BODO_OOV;
}

int kmx_is_valid_bodo_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char BODO_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return BODO_CHAR_START_TABLE[get_bodo_char_type(c)];
}

int kmx_is_valid_bodo_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char BODO_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return BODO_CHAR_FOLLOW_TABLE[get_bodo_char_type(c1) * 6 + get_bodo_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Dogri functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Dogri character types */

#define LDB_DOGRI_OOV       0       /* Not a Dogri letter   */
#define LDB_DOGRI_CON       1       /* Consonants           */
#define LDB_DOGRI_INV       2       /* Independent vowels   */
#define LDB_DOGRI_DEV       3       /* Dependent vowels     */
#define LDB_DOGRI_VIR       4       /* Virama sign          */
#define LDB_DOGRI_SIG       5       /* Other signs          */

#define N_OOV  LDB_DOGRI_OOV
#define N_CON  LDB_DOGRI_CON
#define N_INV  LDB_DOGRI_INV
#define N_DEV  LDB_DOGRI_DEV
#define N_VIR  LDB_DOGRI_VIR
#define N_SIG  LDB_DOGRI_SIG

static int get_dogri_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char DOGRI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return DOGRI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_DOGRI_OOV;
}

int kmx_is_valid_dogri_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char DOGRI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return DOGRI_CHAR_START_TABLE[get_dogri_char_type(c)];
}

int kmx_is_valid_dogri_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char DOGRI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return DOGRI_CHAR_FOLLOW_TABLE[get_dogri_char_type(c1) * 6 + get_dogri_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Sanskrit functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Sanskrit character types */

#define LDB_SANSKRIT_OOV       0       /* Not a Sanskrit letter   */
#define LDB_SANSKRIT_CON       1       /* Consonants           */
#define LDB_SANSKRIT_INV       2       /* Independent vowels   */
#define LDB_SANSKRIT_DEV       3       /* Dependent vowels     */
#define LDB_SANSKRIT_VIR       4       /* Virama sign          */
#define LDB_SANSKRIT_SIG       5       /* Other signs          */

#define N_OOV  LDB_SANSKRIT_OOV
#define N_CON  LDB_SANSKRIT_CON
#define N_INV  LDB_SANSKRIT_INV
#define N_DEV  LDB_SANSKRIT_DEV
#define N_VIR  LDB_SANSKRIT_VIR
#define N_SIG  LDB_SANSKRIT_SIG

static int get_sanskrit_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char SANSKRIT_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return SANSKRIT_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_SANSKRIT_OOV;
}

int kmx_is_valid_sanskrit_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char SANSKRIT_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SANSKRIT_CHAR_START_TABLE[get_sanskrit_char_type(c)];
}

int kmx_is_valid_sanskrit_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char SANSKRIT_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return SANSKRIT_CHAR_FOLLOW_TABLE[get_sanskrit_char_type(c1) * 6 + get_sanskrit_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Kashmiri functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Kashmiri character types */

#define LDB_KASHMIRI_OOV       0       /* Not a Kashmiri letter   */
#define LDB_KASHMIRI_CON       1       /* Consonants           */
#define LDB_KASHMIRI_INV       2       /* Independent vowels   */
#define LDB_KASHMIRI_DEV       3       /* Dependent vowels     */
#define LDB_KASHMIRI_VIR       4       /* Virama sign          */
#define LDB_KASHMIRI_SIG       5       /* Other signs          */

#define N_OOV  LDB_KASHMIRI_OOV
#define N_CON  LDB_KASHMIRI_CON
#define N_INV  LDB_KASHMIRI_INV
#define N_DEV  LDB_KASHMIRI_DEV
#define N_VIR  LDB_KASHMIRI_VIR
#define N_SIG  LDB_KASHMIRI_SIG

static int get_kashmiri_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char KASHMIRI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return KASHMIRI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_KASHMIRI_OOV;
}

int kmx_is_valid_kashmiri_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char KASHMIRI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return KASHMIRI_CHAR_START_TABLE[get_kashmiri_char_type(c)];
}

int kmx_is_valid_kashmiri_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char KASHMIRI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return KASHMIRI_CHAR_FOLLOW_TABLE[get_kashmiri_char_type(c1) * 6 + get_kashmiri_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Konkani functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Konkani character types */

#define LDB_KONKANI_OOV       0       /* Not a Konkani letter   */
#define LDB_KONKANI_CON       1       /* Consonants           */
#define LDB_KONKANI_INV       2       /* Independent vowels   */
#define LDB_KONKANI_DEV       3       /* Dependent vowels     */
#define LDB_KONKANI_VIR       4       /* Virama sign          */
#define LDB_KONKANI_SIG       5       /* Other signs          */

#define N_OOV  LDB_KONKANI_OOV
#define N_CON  LDB_KONKANI_CON
#define N_INV  LDB_KONKANI_INV
#define N_DEV  LDB_KONKANI_DEV
#define N_VIR  LDB_KONKANI_VIR
#define N_SIG  LDB_KONKANI_SIG

static int get_konkani_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char KONKANI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return KONKANI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_KONKANI_OOV;
}

int kmx_is_valid_konkani_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char KONKANI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return KONKANI_CHAR_START_TABLE[get_konkani_char_type(c)];
}

int kmx_is_valid_konkani_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char KONKANI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return KONKANI_CHAR_FOLLOW_TABLE[get_konkani_char_type(c1) * 6 + get_konkani_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Maithili functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Maithili character types */

#define LDB_MAITHILI_OOV       0       /* Not a Maithili letter   */
#define LDB_MAITHILI_CON       1       /* Consonants           */
#define LDB_MAITHILI_INV       2       /* Independent vowels   */
#define LDB_MAITHILI_DEV       3       /* Dependent vowels     */
#define LDB_MAITHILI_VIR       4       /* Virama sign          */
#define LDB_MAITHILI_SIG       5       /* Other signs          */

#define N_OOV  LDB_MAITHILI_OOV
#define N_CON  LDB_MAITHILI_CON
#define N_INV  LDB_MAITHILI_INV
#define N_DEV  LDB_MAITHILI_DEV
#define N_VIR  LDB_MAITHILI_VIR
#define N_SIG  LDB_MAITHILI_SIG

static int get_maithili_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char MAITHILI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return MAITHILI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_MAITHILI_OOV;
}

int kmx_is_valid_maithili_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char MAITHILI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return MAITHILI_CHAR_START_TABLE[get_maithili_char_type(c)];
}

int kmx_is_valid_maithili_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char MAITHILI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return MAITHILI_CHAR_FOLLOW_TABLE[get_maithili_char_type(c1) * 6 + get_maithili_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Manipuri functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Manipuri character types */

#define LDB_MANIPURI_OOV       0       /* Not a Manipuri letter   */
#define LDB_MANIPURI_CON       1       /* Consonants           */
#define LDB_MANIPURI_INV       2       /* Independent vowels   */
#define LDB_MANIPURI_DEV       3       /* Dependent vowels     */
#define LDB_MANIPURI_VIR       4       /* Virama sign          */
#define LDB_MANIPURI_SIG       5       /* Other signs          */

#define N_OOV  LDB_MANIPURI_OOV
#define N_CON  LDB_MANIPURI_CON
#define N_INV  LDB_MANIPURI_INV
#define N_DEV  LDB_MANIPURI_DEV
#define N_VIR  LDB_MANIPURI_VIR
#define N_SIG  LDB_MANIPURI_SIG

static int get_manipuri_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char MANIPURI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return MANIPURI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_MANIPURI_OOV;
}

int kmx_is_valid_manipuri_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char MANIPURI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return MANIPURI_CHAR_START_TABLE[get_manipuri_char_type(c)];
}

int kmx_is_valid_manipuri_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char MANIPURI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return MANIPURI_CHAR_FOLLOW_TABLE[get_manipuri_char_type(c1) * 6 + get_manipuri_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Santali functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Santali character types */

#define LDB_SANTALI_OOV       0       /* Not a Santali letter   */
#define LDB_SANTALI_CON       1       /* Consonants           */
#define LDB_SANTALI_INV       2       /* Independent vowels   */
#define LDB_SANTALI_DEV       3       /* Dependent vowels     */
#define LDB_SANTALI_VIR       4       /* Virama sign          */
#define LDB_SANTALI_SIG       5       /* Other signs          */

#define N_OOV  LDB_SANTALI_OOV
#define N_CON  LDB_SANTALI_CON
#define N_INV  LDB_SANTALI_INV
#define N_DEV  LDB_SANTALI_DEV
#define N_VIR  LDB_SANTALI_VIR
#define N_SIG  LDB_SANTALI_SIG

static int get_santali_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char SANTALI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return SANTALI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_SANTALI_OOV;
}

int kmx_is_valid_santali_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char SANTALI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SANTALI_CHAR_START_TABLE[get_santali_char_type(c)];
}

int kmx_is_valid_santali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char SANTALI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return SANTALI_CHAR_FOLLOW_TABLE[get_santali_char_type(c1) * 6 + get_santali_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Somali functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Somali character types */

#define LDB_SOMALI_OOV       0       /* Not a Somali letter   */
#define LDB_SOMALI_CON       1       /* Consonants           */
#define LDB_SOMALI_INV       2       /* Independent vowels   */
#define LDB_SOMALI_DEV       3       /* Dependent vowels     */
#define LDB_SOMALI_VIR       4       /* Virama sign          */
#define LDB_SOMALI_SIG       5       /* Other signs          */

#define N_OOV  LDB_SOMALI_OOV
#define N_CON  LDB_SOMALI_CON
#define N_INV  LDB_SOMALI_INV
#define N_DEV  LDB_SOMALI_DEV
#define N_VIR  LDB_SOMALI_VIR
#define N_SIG  LDB_SOMALI_SIG

static int get_somali_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char SOMALI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return SOMALI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_SOMALI_OOV;
}

int kmx_is_valid_somali_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char SOMALI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SOMALI_CHAR_START_TABLE[get_somali_char_type(c)];
}

int kmx_is_valid_somali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char SOMALI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return SOMALI_CHAR_FOLLOW_TABLE[get_somali_char_type(c1) * 6 + get_somali_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Sindhi functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Sindhi character types */

#define LDB_SINDHI_OOV       0       /* Not a Sindhi letter   */
#define LDB_SINDHI_CON       1       /* Consonants           */
#define LDB_SINDHI_INV       2       /* Independent vowels   */
#define LDB_SINDHI_DEV       3       /* Dependent vowels     */
#define LDB_SINDHI_VIR       4       /* Virama sign          */
#define LDB_SINDHI_SIG       5       /* Other signs          */

#define N_OOV  LDB_SINDHI_OOV
#define N_CON  LDB_SINDHI_CON
#define N_INV  LDB_SINDHI_INV
#define N_DEV  LDB_SINDHI_DEV
#define N_VIR  LDB_SINDHI_VIR
#define N_SIG  LDB_SINDHI_SIG

static int get_sindhi_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char SINDHI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0900 - 0x090F */ N_OOV, N_SIG, N_SIG, N_SIG, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV, N_INV,
    /* 0x0910 - 0x091F */ N_INV, N_INV, N_INV, N_INV, N_INV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0920 - 0x092F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0930 - 0x093F */ N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_OOV, N_OOV, N_VIR, N_OOV, N_DEV, N_DEV,
    /* 0x0940 - 0x094F */ N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_DEV, N_VIR, N_OOV, N_OOV,
    /* 0x0950 - 0x095F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON, N_CON,
    /* 0x0960 - 0x096F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV,
    /* 0x0970 - 0x097F */ N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV, N_OOV
    };

    if (c >= 0x900 && c <= 0x97F)
        return SINDHI_CHAR_TYPE_TABLE[c - 0x900];
    else
        return LDB_SINDHI_OOV;
}

int kmx_is_valid_sindhi_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char SINDHI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SINDHI_CHAR_START_TABLE[get_sindhi_char_type(c)];
}

int kmx_is_valid_sindhi_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char SINDHI_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON INV DEV VIR SIG */
        /* OOV */ 1,  1,  1,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,
        /* DEV */ 1,  1,  1,  0,  0,  1,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,
    };

    return SINDHI_CHAR_FOLLOW_TABLE[get_sindhi_char_type(c1) * 6 + get_sindhi_char_type(c2)];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Bengali functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define B_OOV  0
#define B_CON  1
#define B_INV  2
#define B_DEV  3
#define B_VIR  4
#define B_SIG  5
#define B_NUK  6

static unsigned char get_bengali_char_type(
    IN LDB_CHAR             c)
{
    static const unsigned char BENGALI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0980 - 0x098F */ B_OOV ,B_SIG ,B_SIG ,B_SIG ,B_OOV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_OOV ,B_OOV ,B_OOV ,B_INV,
    /* 0x0990 - 0x099F */ B_INV ,B_OOV ,B_OOV ,B_INV ,B_INV ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON,
    /* 0x09A0 - 0x09AF */ B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_OOV ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON,
    /* 0x09B0 - 0x09BF */ B_CON ,B_OOV ,B_CON ,B_OOV ,B_OOV ,B_OOV ,B_CON ,B_CON ,B_CON ,B_CON ,B_OOV ,B_OOV ,B_NUK ,B_OOV ,B_DEV ,B_DEV,
    /* 0x09C0 - 0x09CF */ B_DEV ,B_DEV ,B_DEV ,B_DEV ,B_OOV ,B_OOV ,B_OOV ,B_DEV ,B_DEV ,B_OOV ,B_OOV ,B_DEV ,B_DEV ,B_VIR ,B_OOV ,B_OOV,
    /* 0x09D0 - 0x09DF */ B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_CON ,B_CON ,B_OOV ,B_CON,
    /* 0x09E0 - 0x09EF */ B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV,
    /* 0x09F0 - 0x09FF */ B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV
    };

    if (c >= 0x980 && c <= 0x9FF)
        return BENGALI_CHAR_TYPE_TABLE[c - 0x980];
    else
        return B_OOV;
}

int kmx_is_valid_bengali_word_start_char(
    IN LDB_CHAR             c)
{
    static const unsigned char BENGALI_CHAR_START_TABLE[7] =
    {
        /* OOV CON INV DEV VIR SIG NUK*/
            1,  1,  1,  0,  0,  0, 0
    };

    return BENGALI_CHAR_START_TABLE[get_bengali_char_type(c)];
}

int kmx_is_valid_bengali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    static const unsigned char BENGALI_CHAR_FOLLOW_TABLE[49] =
    {
              /* OOV CON INV DEV VIR SIG NUK*/
        /* OOV */ 1,  1,  1,  0,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,  1,
        /* INV */ 1,  1,  1,  0,  0,  1,  0,
        /* DEV */ 1,  1,  1,  0,  0,  1,  0,
        /* VIR */ 1,  1,  0,  0,  0,  0,  0,
        /* SIG */ 1,  1,  1,  0,  0,  0,  0,      /* INV can not follow SIG in Hindi, the Only difference. */
        /* NUK */ 1,  1,  1,  1,  1,  1,  0       

    };

    unsigned char t1;
    unsigned char t2;
    
    /* For special case 0x985 0x9cd 0x9af */
    if (c1 == 0x985 && c2 == 0x9CD)
        return 1;
    
    t1 = get_bengali_char_type(c1);
    t2 = get_bengali_char_type(c2);

    return BENGALI_CHAR_FOLLOW_TABLE[t1 * 7 + t2];
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Tibetan functions                                                       */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define T_OOV  KMX_LDB_TIBETAN_OOV
#define T_CON  KMX_LDB_TIBETAN_CON
#define T_JOC  KMX_LDB_TIBETAN_JOC
#define T_DEV  KMX_LDB_TIBETAN_DEV
#define T_VIR  KMX_LDB_TIBETAN_VIR
#define T_SIG  KMX_LDB_TIBETAN_SIG

unsigned char kmx_get_tibetan_char_type(
    const void*                 database,
    unsigned short              c)
{
#if defined(KMX_LDB_TIBETAN)
static const unsigned char TIBETAN_CHAR_TYPE_TABLE[0x100] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0F00 - 0x0F0F */ T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0F10 - 0x0F1F */ T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0F20 - 0x0F2F */ T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0F30 - 0x0F3F */ T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_SIG ,T_SIG ,T_SIG ,T_SIG ,T_SIG ,T_SIG ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0F40 - 0x0F4F */ T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,
    /* 0x0F50 - 0x0F5F */ T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,
    /* 0x0F60 - 0x0F6F */ T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_CON ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0F70 - 0x0F7F */ T_OOV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_DEV ,T_SIG ,T_SIG ,
    /* 0x0F80 - 0x0F8F */ T_DEV ,T_DEV ,T_SIG ,T_SIG ,T_VIR ,T_SIG ,T_SIG ,T_SIG ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0F90 - 0x0F9F */ T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,
    /* 0x0FA0 - 0x0FAF */ T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,
    /* 0x0FB0 - 0x0FBF */ T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_JOC ,T_DEV ,T_DEV ,T_DEV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0FC0 - 0x0FCF */ T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0FD0 - 0x0FDF */ T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0FE0 - 0x0FEF */ T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    /* 0x0FF0 - 0x0FFF */ T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,T_OOV ,
    };
    
    if (c >= 0xF00 && c <= 0xFFF)
        return TIBETAN_CHAR_TYPE_TABLE[c - 0xF00];
    else
        return KMX_LDB_TIBETAN_OOV;
#else
    return 0;
#endif
}    
    
int kmx_is_valid_tibetan_start_char(
    const void*                 database,
    unsigned short              c)
{
#if defined(KMX_LDB_TIBETAN)
    static const unsigned char TIBETAN_CHAR_START_TABLE[7] =
    {
        /* OOV CON JOC DEV VIR SIG */
            1,  1,  0,  0,  0,  0
    };

    return TIBETAN_CHAR_START_TABLE[kmx_get_tibetan_char_type(database, c)];
#else
    return 0;
#endif
}    
    
int kmx_is_valid_tibetan_follow_char(
    const void*                 database,
    unsigned short              c1,
    unsigned short              c2)
{
#if defined(KMX_LDB_TIBETAN)
    static const unsigned char TIBETAN_CHAR_FOLLOW_TABLE[36] =
    {
              /* OOV CON JOC DEV VIR SIG */
        /* OOV */ 1,  1,  0,  0,  0,  0,
        /* CON */ 1,  1,  1,  1,  1,  1,
        /* JOC */ 1,  1,  1,  1,  0,  0,
        /* DEV */ 1,  1,  0,  0,  0,  0,
        /* VIR */ 1,  1,  0,  0,  0,  0,
        /* SIG */ 1,  1,  0,  0,  0,  0,        

    };

    unsigned char t1;
    unsigned char t2;
    
    t1 = kmx_get_tibetan_char_type(database, c1);
    t2 = kmx_get_tibetan_char_type(database, c2);

    return TIBETAN_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif    
}
    
unsigned short kmx_tibetan_consonant_to_subjoined(
    const void*                 database,
    unsigned short              c)
{
    if (kmx_get_tibetan_char_type(database, c) == T_CON)
        c += 0x50;
    
    return c;

}

unsigned short kmx_tibetan_subjoined_to_consonant(
    const void*                 database,
    unsigned short              c)
{
    if (kmx_get_tibetan_char_type(database, c) == T_JOC)
        c -= 0x50;
    
    return c;    
}


/****************************************************************************
 *
 * User data functions.
 *
 ***************************************************************************/

static int udb_is_his_valid(
    IN const UDB*           udb)
{
    const LDB_BYTE*         b;      /* Buffer base address */
    const UDB_HIS*          p;      /* Current item */
    int                     s;      /* Position of current item */
    int                     c;      /* Size of previous item */

    b = (const LDB_BYTE*)udb + udb->hpos;

    c = 0;
    s = 0;
    
    p = (const UDB_HIS*)(b + s);
    while (p->next)
    {
        VALIDATE(p->prev == c);

        if (p->wid == udb->wid_sos)
        {
            VALIDATE(p->next == HSIZE);
            VALIDATE(p->len == 0);
        }
        else if (p->wid == udb->wid_oov)
        {
            VALIDATE(p->next == p->len * IIMEIZE + HSIZE);
            VALIDATE(p->len > 0);
        }
        else
        {
            VALIDATE(p->next == HSIZE);
            VALIDATE(p->len > 0);
        }
        VALIDATE(s + p->next + HSIZE <= udb->hsize);

        c = p->next;
        s += p->next;
        p = (const UDB_HIS*)(b + s);
    }

    VALIDATE(p->prev == c);
    VALIDATE( (s == udb->hlast) && (s + HSIZE <= udb->hsize) );
    VALIDATE(p->len == 0);
    VALIDATE(p->wid == 0);

    return KMX_TRUE;
}

static int udb_is_oov_valid(
    IN const UDB*           udb)
{
    const LDB_BYTE*         b;      /* Buffer base address */
    const UDB_OOV*          p;      /* Current item */
    int                     s;      /* Position of current item */
    int                     c;      /* Size of previous item */

    b = (const LDB_BYTE*)udb + udb->opos;

    c = 0;
    s = 0;
    
    p = (const UDB_OOV*)(b + s);
    while (p->next)
    {
        VALIDATE(p->prev == c);

        VALIDATE(is_aligned(p->next, 2));
        VALIDATE(p->next > OSIZE);
        VALIDATE(s + p->next + OSIZE <= udb->osize);

        c = p->next;
        s += p->next;
        p = (const UDB_OOV*)(b + s);
    }

    VALIDATE(p->prev == c);
    VALIDATE( (s == udb->olast) && (s + OSIZE <= udb->osize) );

    return KMX_TRUE;
}

static void udb_update(
    IN UDB*                 udb)
{
    KMX_ASSERT(udb);

    KMX_ASSERT(udb_is_his_valid(udb));
    KMX_ASSERT(udb_is_oov_valid(udb));

    udb->check = crc((const char*)udb + USIZE, udb->size - USIZE);
}

static int udb_is_valid(
    IN const ENGINE*        eng,
    IN const UDB*           udb,
    IN int                  size)
{
    int                     s;

    KMX_ASSERT(is_aligned(USIZE, 4));

    /* Validate cache object size */

    VALIDATE( (udb->size >= UDB_MIN) && (udb->size <= UDB_MAX) );
    VALIDATE(udb->size <= size);
    VALIDATE(is_aligned(udb->size, 4));

    /* Validate cache object checksum */

    VALIDATE(udb->check == crc((const char*)udb + USIZE, udb->size - USIZE));

    /* Validate version */

    VALIDATE(udb->did == eng->did);

    VALIDATE(udb->major_version == eng->major_version);
    VALIDATE(udb->minor_version == eng->minor_version);
    VALIDATE(udb->build_number == eng->build_number);

    VALIDATE(udb->wid_oov == eng->wid_oov);
    VALIDATE(udb->wid_sos == eng->wid_sos);

    /* Validate history object position, size and content */

    s = (udb->size - USIZE) / 2;
    KMX_ASSERT(is_aligned(s, 2));

    VALIDATE(udb->hpos == USIZE);
    VALIDATE(udb->hsize == s);
    VALIDATE(udb_is_his_valid(udb));

    /* Validate OOV object position, size and content */

    VALIDATE(udb->opos == udb->hpos + s);
    VALIDATE(udb->osize == s);
    VALIDATE(udb_is_oov_valid(udb));

    return KMX_TRUE;
}

static void udb_init(
    IN const ENGINE*        eng,
    IN UDB*                 udb,
    IN int                  size)
{
    int                     t;

    KMX_ASSERT(size >= UDB_MIN);

    t = size % 8;
    if (t > 0)
        size -= t;
    if (size > UDB_MAX)
        size = UDB_MAX;

    memset(udb, 0, size);

    udb->size = size;

    udb->did = eng->did;
    udb->major_version = eng->major_version;
    udb->minor_version = eng->minor_version;
    udb->build_number = eng->build_number;

    udb->wid_oov = eng->wid_oov;
    udb->wid_sos = eng->wid_sos;

    udb->hpos  = USIZE;
    udb->hsize = (udb->size - USIZE) / 2;

    udb->opos  = udb->hpos + udb->hsize;
    udb->osize = udb->hsize;

    udb_update(udb);
}

static void udb_open(
    IN ENGINE*              eng,
    IN void*                udb,
    IN int                  size)
{
    KMX_ASSERT(sizeof(int) == 4);
    KMX_ASSERT(sizeof(LDB_WORD) == 2);
    KMX_ASSERT(sizeof(LDB_CHAR) == 2);
    KMX_ASSERT(sizeof(LDB_WID) == 2);

    KMX_ASSERT(eng);
    KMX_ASSERT(udb);

    eng->udb = (UDB*)udb;

    if (!udb_is_valid(eng, eng->udb, size))
        udb_init(eng, eng->udb, size);
}

static void udb_add_oov(
    IN UDB*                 udb,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    UDB_OOV*                p;
    LDB_BYTE*               b;
    int                     n;
    int                     f;
    int                     t;

    b = (LDB_BYTE*)udb + udb->opos;

    /* Calc size of the new item */

    KMX_ASSERT(len > 0);
    n = OSIZE + IIMEIZE * len;

    if (n + OSIZE > udb->osize)
        return;             /* Item is larger than buffer */

    /* Remove the item if it's already exists */

    t = 0;
    while (t < udb->olast)
    {
        p = (UDB_OOV*)(b + t);
        if (p->next != n)
        {
            t += p->next;
            continue;       /* Length not match */
        }
        KMX_ASSERT(t + n <= udb->olast);
        if (memcmp(b + (t + OSIZE), str, IIMEIZE * len) != 0)
        {
            t += p->next;
            continue;       /* String not match */
        }
        f = p->prev;
        memmove(b + t, b + (t + n), udb->olast + OSIZE - t - n);
        memset(b + (udb->olast + OSIZE - n), 0, n);
        p->prev = (LDB_WORD)f;
        udb->olast -= n;
        break;
    }

    /* Ensure free space for the new item */

    f = udb->osize - udb->olast - OSIZE;
    if (n > f)
    {
        t = 0;
        while (n > f + t)
        {
            p = (UDB_OOV*)(b + t);
            t += p->next;
        }
        KMX_ASSERT( (t > 0) && (t <= udb->olast) );
        memmove(b, b + t, udb->olast + OSIZE - t);
        memset(b + (udb->olast + OSIZE - t), 0, t);
        p = (UDB_OOV*)b;
        p->prev = 0;
        udb->olast -= t;
    }

    /* Append new item at the end */

    KMX_ASSERT(n <= udb->osize - udb->olast - OSIZE);

    p = (UDB_OOV*)(b + udb->olast);
    p->next = (LDB_WORD)n;
    memcpy(b + (udb->olast + OSIZE), str, IIMEIZE * len);

    udb->olast += n;
    p = (UDB_OOV*)(b + udb->olast);
    p->prev = (LDB_WORD)n;
    p->next = 0;
}

static void udb_append(
    IN ENGINE*              eng,
    IN LDB_WID              wid,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    UDB*                    udb;
    LDB_BYTE*               b;
    UDB_HIS*                p;
    int                     n;
    int                     m;
    int                     f;
    int                     t;

    KMX_ASSERT(eng);
    udb = eng->udb;
    if (!udb)
        return;             /* User data not available */

    b = (LDB_BYTE*)udb + udb->hpos;

    /* Calc space for the new item */

    n = HSIZE;
    if (wid == udb->wid_sos)
    {
        KMX_ASSERT(str == 0);
        KMX_ASSERT(len == 0);
        if (udb->hlast > 0)
        {
            p = (UDB_HIS*)(b + udb->hlast);
            p = (UDB_HIS*)(b + (udb->hlast - p->prev));
            if (p->wid == udb->wid_sos)
                return;     /* Duplicated SOS item */
        }
    }
    else if (wid == udb->wid_oov)
    {
        KMX_ASSERT(str);
        KMX_ASSERT(len > 0);
        n += IIMEIZE * len;
    }
    else
    {
        KMX_ASSERT(str == 0);
        KMX_ASSERT(len > 0);
    }

    if (n + HSIZE > udb->hsize)
        return;             /* Item is larger than the whole buffer */

    /* Ensure free space for the new item */

    f = udb->hsize - udb->hlast - HSIZE;
    if (n > f)
    {
        m = align_adjust(udb->hsize / 4, 2);
        if (m > 1024)
            m = 1024;
        if (m < n)
            m = n;
        t = 0;
        while (m > f + t)
        {
            p = (UDB_HIS*)(b + t);
            if (p->wid == udb->wid_oov)
                udb_add_oov(udb, (const LDB_CHAR*)(b + (t + HSIZE)), p->len);
            t += p->next;
        }
        KMX_ASSERT( (t > 0) && (t <= udb->hlast) );
        memmove(b, b + t, udb->hlast + HSIZE - t);
        memset(b + udb->hlast + HSIZE - t, 0, t);
        p = (UDB_HIS*)b;
        p->prev = 0;
        udb->hlast -= t;
    }

    /* Append new item at the end */

    KMX_ASSERT(n <= udb->hsize - udb->hlast - HSIZE);

    p = (UDB_HIS*)(b + udb->hlast);
    p->wid = wid;
    p->len = (LDB_WORD)len;
    p->next = (LDB_WORD)n;
    if (wid == udb->wid_oov)
        memcpy(b + (udb->hlast + HSIZE), str, IIMEIZE * len);

    udb->hlast += n;
    p = (UDB_HIS*)(b + udb->hlast);
    p->wid = 0;
    p->len = 0;
    p->prev = (LDB_WORD)n;
    p->next = 0;

    udb_update(udb);
}

static UDB_HIT udb_his_first(
    IN const ENGINE*        eng)
{
    const UDB*              udb;
    const UDB_HIS*          p;

    KMX_ASSERT(eng);
    udb = eng->udb;
    if (!udb)
        return 0;           /* User data not available */

    if (udb->hlast)
    {
        p = (const UDB_HIS*)((const LDB_BYTE*)udb + (udb->hpos + udb->hlast));
        return udb->hlast - p->prev + 1;
    }
    else
    {
        return 0;
    }
}

static UDB_HIT udb_his_next(
    IN const ENGINE*        eng,
    IN UDB_HIT              it)
{
    const UDB*              udb;
    const UDB_HIS*          p;

    KMX_ASSERT(eng);
    udb = eng->udb;
    KMX_ASSERT(udb);
    
    it--;
    KMX_ASSERT( (it >= 0) && (it < udb->hlast) );
    p = (const UDB_HIS*)((const LDB_BYTE*)udb + (udb->hpos + it));

    if (p->prev)
    {
        KMX_ASSERT(it >= p->prev);
        return it - p->prev + 1;
    }
    else
    {
        KMX_ASSERT(it == 0);
        return 0;
    }
}

static LDB_WID udb_his_word(
    IN const ENGINE*        eng,
    IN UDB_HIT              it)
{
    const UDB*              udb;
    const UDB_HIS*          p;

    KMX_ASSERT(eng);
    udb = eng->udb;
    KMX_ASSERT(udb);
    
    it--;
    KMX_ASSERT( (it >= 0) && (it < udb->hlast) );
    p = (const UDB_HIS*)((const LDB_BYTE*)udb + (udb->hpos + it));

    return p->wid;
}

static int udb_his_len(
    IN const ENGINE*        eng,
    IN UDB_HIT              it)
{
    const UDB*              udb;
    const UDB_HIS*          p;

    KMX_ASSERT(eng);
    udb = eng->udb;
    KMX_ASSERT(udb);
    
    it--;
    KMX_ASSERT( (it >= 0) && (it < udb->hlast) );
    p = (const UDB_HIS*)((const LDB_BYTE*)udb + (udb->hpos + it));

    return p->len;
}

static const LDB_CHAR* udb_his_str(
    IN const ENGINE*        eng,
    IN UDB_HIT              it)
{
    const UDB*              udb;

    KMX_ASSERT(eng);
    udb = eng->udb;
    KMX_ASSERT(udb);
    
    it--;
    KMX_ASSERT( (it >= 0) && (it < udb->hlast) );

    return (const LDB_CHAR*)((const LDB_BYTE*)udb + (udb->hpos + it + HSIZE));
}

static UDB_OIT udb_oov_first(
    IN const ENGINE*        eng)
{
    const UDB*              udb;
    const UDB_OOV*          p;

    KMX_ASSERT(eng);
    udb = eng->udb;
    if (!udb)
        return 0;           /* User data not available */

    if (udb->olast)
    {
        p = (const UDB_OOV*)((const LDB_BYTE*)udb + (udb->opos + udb->olast));
        return udb->olast - p->prev + 1;
    }
    else
    {
        return 0;
    }
}

static UDB_OIT udb_oov_next(
    IN const ENGINE*        eng,
    IN UDB_OIT              it)
{
    const UDB*              udb;
    const UDB_OOV*          p;

    KMX_ASSERT(eng);
    udb = eng->udb;
    KMX_ASSERT(udb);

    it--;    
    KMX_ASSERT( (it >= 0) && (it < udb->olast) );
    p = (const UDB_OOV*)((const LDB_BYTE*)udb + (udb->opos + it));

    if (p->prev)
    {
        KMX_ASSERT(it >= p->prev);
        return it - p->prev + 1;
    }
    else
    {
        KMX_ASSERT(it == 0);
        return 0;
    }
}

static int udb_oov_len(
    IN const ENGINE*        eng,
    IN UDB_OIT              it)
{
    const UDB*              udb;
    const UDB_OOV*          p;

    KMX_ASSERT(eng);
    udb = eng->udb;
    KMX_ASSERT(udb);
    
    it--;    
    KMX_ASSERT( (it >= 0) && (it < udb->olast) );
    p = (const UDB_OOV*)((const LDB_BYTE*)udb + (udb->opos + it));

    KMX_ASSERT(p->next > OSIZE);
    return (p->next - OSIZE) / IIMEIZE;
}

static const LDB_CHAR* udb_oov_str(
    IN const ENGINE*        eng,
    IN UDB_OIT              it)
{
    const UDB*              udb;

    KMX_ASSERT(eng);
    udb = eng->udb;
    KMX_ASSERT(udb);
    
    it--;    
    KMX_ASSERT( (it >= 0) && (it < udb->olast) );

    return (const LDB_CHAR*)((const LDB_BYTE*)udb + (udb->opos + it + OSIZE));
}

/****************************************************************************
 *
 * Local functions.
 *
 ***************************************************************************/

static LDB_CHAR upper_case(
    IN const ENGINE*        eng,
    IN LDB_CHAR             c)
{
    int                     i;

    /* Lanuage specified letters */

    i = sorted_strchr(eng->lc2ucm, eng->lc2ucm_length, c);
    if (i >= 0)
        return eng->lc2ucm[eng->lc2ucm_length + i];

    /* Basic latin letters */

    if ( (c >= 0x61) && (c <= 0x7A) )
        return (LDB_CHAR) (c - 0x20);

    /* Latin-1 supplement letters */

    if ( (c >= 0xE0) && (c <= 0xF6) )
        return (LDB_CHAR) (c - 0x20);

    if ( (c >= 0xF8) && (c <= 0xFE) )
        return (LDB_CHAR) (c - 0x20);

    return c;
}

static LDB_CHAR lower_case(
    IN const ENGINE*        eng,
    IN LDB_CHAR             c)
{
    int                     i;

    /* Lanuage specified letters */

    i = sorted_strchr(eng->uc2lcm, eng->uc2lcm_length, c);
    if (i >= 0)
        return eng->uc2lcm[eng->uc2lcm_length + i];

    /* Basic latin letters */

    if ( (c >= 0x41) && (c <= 0x5A) )
        return (LDB_CHAR) (c + 0x20);

    /* Latin-1 supplement letters */

    if ( (c >= 0xC0) && (c <= 0xD6) )
        return (LDB_CHAR) (c + 0x20);

    if ( (c >= 0xD8) && (c <= 0xDE) )
        return (LDB_CHAR) (c + 0x20);

    return c;
}

static LDB_CHAR to_upper_case(
    IN const LDB*           ldb,
    IN LDB_CHAR             c)
{
    int                     i;

    /* Lanuage specified letters */

    i = sorted_strchr(ldb->lc2ucm, ldb->lc2ucm_length, c);
    if (i >= 0)
        return ldb->lc2ucm[ldb->lc2ucm_length + i];

    /* Basic latin letters */

    if ( (c >= 0x61) && (c <= 0x7A) )
        return (LDB_CHAR) (c - 0x20);

    /* Latin-1 supplement letters */

    if ( (c >= 0xE0) && (c <= 0xF6) )
        return (LDB_CHAR) (c - 0x20);

    if ( (c >= 0xF8) && (c <= 0xFE) )
        return (LDB_CHAR) (c - 0x20);

    return c;
}

static LDB_CHAR to_lower_case(
    IN const LDB*           ldb,
    IN LDB_CHAR             c)
{
    int                     i;

    /* Lanuage specified letters */

    i = sorted_strchr(ldb->uc2lcm, ldb->uc2lcm_length, c);
    if (i >= 0)
        return ldb->uc2lcm[ldb->uc2lcm_length + i];

    /* Basic latin letters */

    if ( (c >= 0x41) && (c <= 0x5A) )
        return (LDB_CHAR) (c + 0x20);

    /* Latin-1 supplement letters */

    if ( (c >= 0xC0) && (c <= 0xD6) )
        return (LDB_CHAR) (c + 0x20);

    if ( (c >= 0xD8) && (c <= 0xDE) )
        return (LDB_CHAR) (c + 0x20);

    return c;
}

static void string_convert_case(
    IN const ENGINE*        eng,
    IN OUT LDB_CHAR*        str,
    IN int                  len,
    IN LDB_WORD             flags)
{
    if (flags & KMX_LATIN_LOWER_CASE)
    {
        while (len > 0)
        {
            KMX_ASSERT(str);
            *str = lower_case(eng, *str);
            str++;
            len--;
        }
    }
    else if (flags & KMX_LATIN_UPPER_CASE)
    {
        while (len > 0)
        {
            KMX_ASSERT(str);
            *str = upper_case(eng, *str);
            str++;
            len--;
        }
    }
    else if (flags & KMX_LATIN_INITIAL_CASE)
    {
        if (len > 0)
        {
            KMX_ASSERT(str);
            *str = upper_case(eng, *str);
            str++;
            len--;
        }
        while (len > 0)
        {
            KMX_ASSERT(str);
            *str = lower_case(eng, *str);
            str++;
            len--;
        }
    }
}

static int is_valid_string(
    IN const ENGINE*        eng,
    IN const LDB_CHAR*      string,
    IN int                  length)
{
    LDB_CHAR prev_char = 0;
    int i;

    if (eng->language == KMX_LANG_HINDI)
    {
        for (i = 0; i < length; i++)
        {
            if (!kmx_is_valid_hindi_follow_char(prev_char, string[i]))
            {
                return KMX_FALSE;
            }
            prev_char = string[i];
        }               
    }
    else if (eng->language == KMX_LANG_BENGALI)
    {
        for (i = 0; i < length; i++)
        {
            if (!kmx_is_valid_bengali_follow_char(prev_char, string[i]))
            {
                return KMX_FALSE;
            }
            prev_char = string[i];
        }
    }
    else if (eng->language == KMX_LANG_THAI)
    {
        for (i = 0; i < length; i++)
        {
            if (!kmx_is_valid_thai_follow_char(prev_char, string[i]))
            {
                return KMX_FALSE;
            }
            prev_char = string[i];
        }
    }

    return KMX_TRUE;
}

/****************************************************************************
 *
 * Language database validation functions.
 *
 ***************************************************************************/

#ifdef KMX_DEBUG

static int ldb_is_alphabet_valid(
    IN const LDB*           ldb)
{
    int                     i;

    VALIDATE(ldb->alphabet);
    VALIDATE(ldb->alphabet_length > 0);

    for (i = 1; i < ldb->alphabet_length; i++)
    {
        VALIDATE(ldb->alphabet[i - 1] < ldb->alphabet[i]);
    }

    return KMX_TRUE;
}

static int ldb_is_lc2ucm_valid(
    IN const LDB*           ldb)
{
    int                     i;

    if (ldb->lc2ucm)
    {
        VALIDATE(ldb->lc2ucm_length > 0);

        for (i = 1; i < ldb->lc2ucm_length; i++)
        {
            VALIDATE(ldb->lc2ucm[i - 1] < ldb->lc2ucm[i]);
        }
    }
    else
    {
        VALIDATE(ldb->lc2ucm_length == 0);
    }

    return KMX_TRUE;
}

static int ldb_is_uc2lcm_valid(
    IN const LDB*           ldb)
{
    int                     i;

    if (ldb->uc2lcm)
    {
        VALIDATE(ldb->uc2lcm_length > 0);

        for (i = 1; i < ldb->uc2lcm_length; i++)
        {
            VALIDATE(ldb->uc2lcm[i - 1] < ldb->uc2lcm[i]);
        }
    }
    else
    {
        VALIDATE(ldb->uc2lcm_length == 0);
    }

    return KMX_TRUE;
}

static int ldb_is_sos_table_valid(
    IN const LDB*           ldb)
{
    int                     i;

    VALIDATE(ldb->sos_table);
    VALIDATE(ldb->sos_table_length > 0);

    for (i = 1; i < ldb->sos_table_length; i++)
    {
        VALIDATE(ldb->sos_table[i - 1] < ldb->sos_table[i]);
    }

    return KMX_TRUE;
}

static int ldb_is_sou_table_valid(
    IN const LDB*           ldb)
{
    int                     i;

    VALIDATE(ldb->sou_table);
    VALIDATE(ldb->sou_table_length > 0);

    for (i = 1; i < ldb->sou_table_length; i++)
    {
        VALIDATE(ldb->sou_table[i - 1] < ldb->sou_table[i]);
    }

    return KMX_TRUE;
}

static int ldb_is_word_count_valid(
    IN const LDB*           ldb)
{
    switch (ldb->word_id_bits)
    {
        case 4:
            VALIDATE( (ldb->word_count >= 6) && (ldb->word_count <= 13) );
            VALIDATE(ldb->wid_oov == 14);
            VALIDATE(ldb->wid_sos == 15);
            break;
        case 5:
            VALIDATE( (ldb->word_count >= 14) && (ldb->word_count <= 29) );
            VALIDATE(ldb->wid_oov == 30);
            VALIDATE(ldb->wid_sos == 31);
            break;
        case 6:
            VALIDATE( (ldb->word_count >= 30) && (ldb->word_count <= 61) );
            VALIDATE(ldb->wid_oov == 62);
            VALIDATE(ldb->wid_sos == 63);
            break;
        case 7:
            VALIDATE( (ldb->word_count >= 62) && (ldb->word_count <= 125) );
            VALIDATE(ldb->wid_oov == 126);
            VALIDATE(ldb->wid_sos == 127);
            break;
        case 8:
            VALIDATE( (ldb->word_count >= 126) && (ldb->word_count <= 253) );
            VALIDATE(ldb->wid_oov == 254);
            VALIDATE(ldb->wid_sos == 255);
            break;
        case 9:
            VALIDATE( (ldb->word_count >= 254) && (ldb->word_count <= 509) );
            VALIDATE(ldb->wid_oov == 510);
            VALIDATE(ldb->wid_sos == 511);
            break;
        case 10:
            VALIDATE( (ldb->word_count >= 510) && (ldb->word_count <= 1021) );
            VALIDATE(ldb->wid_oov == 1022);
            VALIDATE(ldb->wid_sos == 1023);
            break;
        case 11:
            VALIDATE( (ldb->word_count >= 1022) && (ldb->word_count <= 2045) );
            VALIDATE(ldb->wid_oov == 2046);
            VALIDATE(ldb->wid_sos == 2047);
            break;
        case 12:
            VALIDATE( (ldb->word_count >= 2046) && (ldb->word_count <= 4093) );
            VALIDATE(ldb->wid_oov == 4094);
            VALIDATE(ldb->wid_sos == 4095);
            break;
        case 13:
            VALIDATE( (ldb->word_count >= 4094) && (ldb->word_count <= 8189) );
            VALIDATE(ldb->wid_oov == 8190);
            VALIDATE(ldb->wid_sos == 8191);
            break;
        case 14:
            VALIDATE( (ldb->word_count >= 8190) && (ldb->word_count <= 16381) );
            VALIDATE(ldb->wid_oov == 16382);
            VALIDATE(ldb->wid_sos == 16383);
            break;
        case 15:
            VALIDATE( (ldb->word_count >= 16382) && (ldb->word_count <= 32765) );
            VALIDATE(ldb->wid_oov == 32766);
            VALIDATE(ldb->wid_sos == 32767);
            break;
        case 16:
            VALIDATE( (ldb->word_count >= 32766) && (ldb->word_count <= 65533) );
            VALIDATE(ldb->wid_oov == 65534);
            VALIDATE(ldb->wid_sos == 65535);
            break;
        default:
            return KMX_FALSE;   /* Invalid word id bits */
    }

    return KMX_TRUE;
}

static int ldb_is_word_table_valid(
    IN const LDB*           ldb)
{
    VALIDATE(ldb->word_table);
    VALIDATE( (ldb->word_level_bits >= 4) && (ldb->word_level_bits <= 8) );

    VALIDATE(ldb->string_table);

    switch (ldb->word_index_bits)
    {
        case 5:
            VALIDATE( (ldb->string_table_length >= 16) && (ldb->string_table_length <= 31) );
            break;
        case 6:
            VALIDATE( (ldb->string_table_length >= 32) && (ldb->string_table_length <= 63) );
            break;
        case 7:
            VALIDATE( (ldb->string_table_length >= 64) && (ldb->string_table_length <= 127) );
            break;
        case 8:
            VALIDATE( (ldb->string_table_length >= 128) && (ldb->string_table_length <= 255) );
            break;
        case 9:
            VALIDATE( (ldb->string_table_length >= 256) && (ldb->string_table_length <= 511) );
            break;
        case 10:
            VALIDATE( (ldb->string_table_length >= 512) && (ldb->string_table_length <= 1023) );
            break;
        case 11:
            VALIDATE( (ldb->string_table_length >= 1024) && (ldb->string_table_length <= 2047) );
            break;
        case 12:
            VALIDATE( (ldb->string_table_length >= 2048) && (ldb->string_table_length <= 4095) );
            break;
        case 13:
            VALIDATE( (ldb->string_table_length >= 4096) && (ldb->string_table_length <= 8191) );
            break;
        case 14:
            VALIDATE( (ldb->string_table_length >= 8192) && (ldb->string_table_length <= 16383) );
            break;
        case 15:
            VALIDATE( (ldb->string_table_length >= 16384) && (ldb->string_table_length <= 32767) );
            break;
        case 16:
            VALIDATE( (ldb->string_table_length >= 32768) && (ldb->string_table_length <= 65535) );
            break;
        case 17:
            VALIDATE( (ldb->string_table_length >= 65536) && (ldb->string_table_length <= 131071) );
            break;
		case 18:
            VALIDATE( (ldb->string_table_length >= 131072) && (ldb->string_table_length <= 262143) );
            break;
		case 19:
            VALIDATE( (ldb->string_table_length >= 262144) && (ldb->string_table_length <= 524287) );
            break;
        default:
            return KMX_FALSE;   /* Invalid word index bits */
    }

    return KMX_TRUE;
}

static int ldb_is_char_table_valid(
    IN const LDB*           ldb)
{
    int                     i;

    VALIDATE(ldb->char_table);
    VALIDATE(ldb->ckey_table);

    switch (ldb->char_code_bits)
    {
        case 5:
            VALIDATE( (ldb->char_code_count >= 17) && (ldb->char_code_count <= 32) );
            break;
        case 6:
            VALIDATE( (ldb->char_code_count >= 33) && (ldb->char_code_count <= 64) );
            break;
        case 7:
            VALIDATE( (ldb->char_code_count >= 65) && (ldb->char_code_count <= 128) );
            break;
        default:
            return KMX_FALSE;   /* Invalid char code bits */
    }

    for (i = 1; i < ldb->char_code_count; i++)
    {
        VALIDATE(ldb->char_table[i - 1] < ldb->char_table[i]);
    }

    if (IS_SURE_TYPE(ldb->did))
    {
        for (i = 0; i < ldb->char_code_count; i++)
        {
            VALIDATE(ldb->ckey_table[i] <= 15);
        }
    }
    else
    {
        for (i = 0; i < ldb->char_code_count; i++)
        {
            VALIDATE(ldb->ckey_table[i] <= 10);
        }
    }

    return KMX_TRUE;
}

static void read_bigram_index(
    IN const LDB*           ldb,
    IN int                  pos,
    OUT int*                wid,
    OUT int*                index)
{
    *wid = read_bits(ldb->bigram_index, pos * (ldb->word_id_bits + ldb->bigram_index_bits), ldb->word_id_bits);
    *index = read_bits(ldb->bigram_index, pos * (ldb->word_id_bits + ldb->bigram_index_bits) + ldb->word_id_bits, ldb->bigram_index_bits);
}

static int ldb_is_bigram_valid(
    IN const LDB*           ldb)
{
    int                     i;
    int                     j;
    int                     wid1;
    int                     prev_wid1;
    int                     wid2;
    int                     prev_wid2;
    int                     index;
    int                     prev_index;

    if (ldb->bigram)
    {
        /* Bigram available */

        VALIDATE(ldb->bigram_index);
        VALIDATE( (ldb->bigram_index_count > 0) && (ldb->bigram_index_count <= ldb->word_count) );

        switch (ldb->bigram_index_bits)
        {
            case 9:
                VALIDATE( (ldb->bigram_count >= 256) && (ldb->bigram_count <= 511) );
                break;
            case 10:
                VALIDATE( (ldb->bigram_count >= 512) && (ldb->bigram_count <= 1023) );
                break;
            case 11:
                VALIDATE( (ldb->bigram_count >= 1024) && (ldb->bigram_count <= 2047) );
                break;
            case 12:
                VALIDATE( (ldb->bigram_count >= 2048) && (ldb->bigram_count <= 4095) );
                break;
            case 13:
                VALIDATE( (ldb->bigram_count >= 4096) && (ldb->bigram_count <= 8191) );
                break;
            case 14:
                VALIDATE( (ldb->bigram_count >= 8192) && (ldb->bigram_count <= 16383) );
                break;
            case 15:
                VALIDATE( (ldb->bigram_count >= 16384) && (ldb->bigram_count <= 32767) );
                break;
            case 16:
                VALIDATE( (ldb->bigram_count >= 32768) && (ldb->bigram_count <= 65535) );
                break;
            default:
                return KMX_FALSE;   /* Invalid bigram index bits */
        }

        /* Validate bigram index and bigram list is ordered */

        read_bigram_index(ldb, 0, &prev_wid1, &prev_index);
        VALIDATE(prev_index == 0);
        for (i = 1; i < ldb->bigram_index_count; i++)
        {
            read_bigram_index(ldb, i, &wid1, &index);
            VALIDATE(wid1 > prev_wid1);
            VALIDATE( (index > prev_index) && (index < ldb->bigram_count) );
            prev_wid2 = read_bits(ldb->bigram, prev_index * ldb->word_id_bits, ldb->word_id_bits);
            for (j = prev_index + 1; j < index; j++)
            {
                wid2 = read_bits(ldb->bigram, j * ldb->word_id_bits, ldb->word_id_bits);
                VALIDATE(wid2 > prev_wid2);
                prev_wid2 = wid2;
            }
            prev_wid1 = wid1;
            prev_index = index;
        }
        read_bigram_index(ldb, ldb->bigram_index_count, &wid1, &index);
        VALIDATE(wid1 == 0);
        VALIDATE(index == ldb->bigram_count);
    }
    else
    {
        /* Bigram not available */

        VALIDATE(ldb->bigram_index == 0);
        VALIDATE(ldb->bigram_index_count == 0);
        VALIDATE(ldb->bigram_index_bits == 0);
        VALIDATE(ldb->bigram_count == 0);
    }

    return KMX_TRUE;
}

static void read_trigram(
    IN const LDB*           ldb,
    IN int                  index,
    OUT int*                wid1,
    OUT int*                wid2,
    OUT int*                wid3)
{
    *wid1 = read_bits(ldb->trigram, index * ldb->word_id_bits * 3, ldb->word_id_bits);
    *wid2 = read_bits(ldb->trigram, index * ldb->word_id_bits * 3 + ldb->word_id_bits, ldb->word_id_bits);
    *wid3 = read_bits(ldb->trigram, index * ldb->word_id_bits * 3 + ldb->word_id_bits * 2, ldb->word_id_bits);
}

static int ldb_is_trigram_valid(
    IN const LDB*           ldb)
{
    int                     i;
    int                     wid1;
    int                     prev_wid1;
    int                     wid2;
    int                     prev_wid2;
    int                     wid3;
    int                     prev_wid3;

    if (ldb->trigram)
    {
        /* Trigram available */

        VALIDATE( (ldb->trigram_count > 0) && (ldb->trigram_count <= 65535) );

        /* Validate trigram list is ordered */

        read_trigram(ldb, 0, &prev_wid1, &prev_wid2, &prev_wid3);
        for (i = 1; i < ldb->trigram_count; i++)
        {
            read_trigram(ldb, i, &wid1, &wid2, &wid3);
            if (wid1 == prev_wid1)
            {
                if (wid2 == prev_wid2)
                {
                    VALIDATE(wid3 > prev_wid3);
                }
                else
                {
                    VALIDATE(wid2 > prev_wid2);
                }
            }
            else
            {
                VALIDATE(wid1 > prev_wid1);
            }
            prev_wid1 = wid1;
            prev_wid2 = wid2;
            prev_wid3 = wid3;
        }
    }
    else
    {
        /* Trigram not available */

        VALIDATE(ldb->trigram_count == 0);
    }

    return KMX_TRUE;
}

static int read_exgram_wid(
    IN const LDB*           ldb,
    IN int                  pos)
{
    return read_bits(ldb->exgram_index, pos * (ldb->word_id_bits + ldb->exgram_index_bits), ldb->word_id_bits);
}

static int ldb_is_exgram_valid(
    IN const LDB*           ldb)
{
    int                     i;
    int                     wid;
    int                     prev_wid;

    if (ldb->exgram)
    {
        /* Exgram available */

        VALIDATE(ldb->exgram_index);
        VALIDATE(ldb->exgram_index_count > 0 && ldb->exgram_index_count <= ldb->word_count);
        VALIDATE(ldb->exgram_index_bits >= 9 && ldb->exgram_index_bits <= 16);

        /* Validate exgram index is ordered */

        prev_wid = read_exgram_wid(ldb, 0);
        for (i = 1; i < ldb->exgram_index_count; i++)
        {
            wid = read_exgram_wid(ldb, i);
            VALIDATE(wid > prev_wid);
            prev_wid = wid;
        }
    }
    else
    {
        /* Exgram not available */

        VALIDATE(ldb->exgram_index == 0);
        VALIDATE(ldb->exgram_index_count == 0);
        VALIDATE(ldb->exgram_index_bits == 0);
    }

    return KMX_TRUE;
}

#endif

static int is_valid_database(IN const LDB* ldb)
{
    VALIDATE(ldb);
    VALIDATE(ldb->did >= MIN_LATIN_DID && ldb->did <= MAX_LATIN_DID);

    #ifdef KMX_DEBUG
    /*
     * More validation in debug version.
     */
    {
        VALIDATE(ldb->copyright);
        VALIDATE(ldb->copyright_length > 0);

        VALIDATE(ldb->eudw_prompt);
        VALIDATE(ldb->eudw_prompt_length > 0);

        VALIDATE(ldb->eudw_title);
        VALIDATE(ldb->eudw_title_length > 0);

        VALIDATE(ldb_is_alphabet_valid(ldb));

        VALIDATE(ldb_is_lc2ucm_valid(ldb));
        VALIDATE(ldb_is_uc2lcm_valid(ldb));

        VALIDATE(ldb_is_sos_table_valid(ldb));
        VALIDATE(ldb_is_sou_table_valid(ldb));

        VALIDATE(ldb->mt_index);
        VALIDATE(ldb->mt_string);
        VALIDATE(ldb->mt_string_length > 0);

        VALIDATE(ldb->legram_index);
        VALIDATE(ldb->legram_string);
        VALIDATE(ldb->legram_string_length > 0);
                
        VALIDATE(ldb_is_word_count_valid(ldb));

        VALIDATE(ldb->word_index);
        VALIDATE( (ldb->max_word_length > 0) && (ldb->max_word_length <= MAX_WORD_LENGTH) );
        
        VALIDATE(ldb_is_word_table_valid(ldb));
        VALIDATE(ldb_is_char_table_valid(ldb));

        VALIDATE(ldb_is_bigram_valid(ldb));
        VALIDATE(ldb_is_trigram_valid(ldb));
        VALIDATE(ldb_is_exgram_valid(ldb));
    }
    #endif

    return KMX_TRUE;
}

/****************************************************************************
 *
 * Local functions.
 *
 ***************************************************************************/

static int is_sos_delimiter(
    IN const ENGINE*        eng,
    IN LDB_CHAR             c)
{
    if (sorted_strchr(eng->sos_table, eng->sos_table_length, c) >= 0)
        return KMX_TRUE;
    else
        return KMX_FALSE;
}

static int is_sou_delimiter(
    IN const ENGINE*        eng,
    IN LDB_CHAR             c)
{
    if (sorted_strchr(eng->sou_table, eng->sou_table_length, c) >= 0) 
        return KMX_TRUE;
    else
        return KMX_FALSE;
}

static int is_alphabet(
    IN const ENGINE*        eng,
    IN LDB_CHAR             c)
{
    if (sorted_strchr(eng->alphabet, eng->alphabet_length, c) >= 0)
        return KMX_TRUE;

    if (c >= 0x61 && c <= 0x7A)
        return KMX_TRUE;

    return KMX_FALSE;
}

static int has_alphabet(
    IN const ENGINE*        eng,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    while (len)
    {
        LDB_CHAR c;

        KMX_ASSERT(str);

        c = lower_case(eng, *str);
        if (is_alphabet(eng, c))
            return KMX_TRUE;

        str++;
        len--;
    }

    return KMX_FALSE;
}

static int search_exgram(
    IN ENGINE*              eng,
    IN LDB_WID              wid1,
    IN LDB_WID              wid2)
{
    if (eng->exgram)
    {
        int                     s;
        int                     e;
        int                     i;
        int                     k;
        LDB_WID                 w;

        if (wid1 == eng->ec_wid)
        {
            s = eng->ec_start;
            e = eng->ec_end;
        }
        else
        {
            k = -1;
            s = 0;
            e = eng->exgram_index_count - 1;

            while (s <= e)
            {
                i = (s + e) / 2;
                w = (LDB_WID) read_bits(eng->exgram_index, i * (eng->word_id_bits + eng->exgram_index_bits), eng->word_id_bits);

                if (wid1 < w)
                {
                    e = i - 1;
                }
                else if (wid1 > w)
                {
                    s = i + 1;
                }
                else /* wid1 == w */
                {
                    k = i;
                    break;
                }
            }

            eng->ec_wid = wid1;

            if (k == -1)
            {
                eng->ec_start = 0;
                eng->ec_end = -1;
                return KMX_FALSE;
            }

            eng->ec_start = s = read_bits(eng->exgram_index, k * (eng->word_id_bits + eng->exgram_index_bits) + eng->word_id_bits, eng->exgram_index_bits);
            eng->ec_end = e = read_bits(eng->exgram_index, (k + 1) * (eng->word_id_bits + eng->exgram_index_bits) + eng->word_id_bits, eng->exgram_index_bits) - 1;
        }

        while (s <= e)
        {
            i = (s + e) / 2;
            w = (LDB_WID) read_bits(eng->exgram, i * eng->word_id_bits, eng->word_id_bits);

            if (wid2 < w)
                e = i - 1;
            else if (wid2 > w)
                s = i + 1;
            else /* wid2 == w */
                return KMX_TRUE;
        }
    }

    return KMX_FALSE;
}

static int search_bigram(
    IN ENGINE*              eng,
    IN LDB_WID              wid1,
    IN LDB_WID              wid2)
{
    if (eng->bigram)
    {
        int                     s;
        int                     e;
        int                     i;
        int                     k;
        LDB_WID                 w;

        if (wid1 == eng->bc_wid)
        {
            s = eng->bc_start;
            e = eng->bc_end;
        }
        else
        {
            k = -1;
            s = 0;
            e = eng->bigram_index_count - 1;

            while (s <= e)
            {
                i = (s + e) / 2;
                w = (LDB_WID) read_bits(eng->bigram_index, i * (eng->word_id_bits + eng->bigram_index_bits), eng->word_id_bits);

                if (wid1 < w)
                {
                    e = i - 1;
                }
                else if (wid1 > w)
                {
                    s = i + 1;
                }
                else /* wid1 == w */
                {
                    k = i;
                    break;
                }
            }

            eng->bc_wid = wid1;

            if (k == -1)
            {
                eng->bc_start = 0;
                eng->bc_end = -1;
                return KMX_FALSE;
            }

            eng->bc_start = s = read_bits(eng->bigram_index, k * (eng->word_id_bits + eng->bigram_index_bits) + eng->word_id_bits, eng->bigram_index_bits);
            eng->bc_end = e = read_bits(eng->bigram_index, (k + 1) * (eng->word_id_bits + eng->bigram_index_bits) + eng->word_id_bits, eng->bigram_index_bits) - 1;
        }

        while (s <= e)
        {
            i = (s + e) / 2;
            w = (LDB_WID) read_bits(eng->bigram, i * eng->word_id_bits, eng->word_id_bits);

            if (wid2 < w)
                e = i - 1;
            else if (wid2 > w)
                s = i + 1;
            else /* wid2 == w */
                return KMX_TRUE;
        }
    }

    return KMX_FALSE;
}

static int compare_trigram_index(
    IN const ENGINE*        eng,
    IN int                  index,
    IN LDB_WID              wid1,
    IN LDB_WID              wid2)
{
    LDB_WID                 w;

    w = (LDB_WID) read_bits(eng->trigram, index * eng->word_id_bits * 3, eng->word_id_bits);

    if (w < wid1)
        return -1;
    if (w > wid1)
        return 1;

    w = (LDB_WID) read_bits(eng->trigram, index * eng->word_id_bits * 3 + eng->word_id_bits, eng->word_id_bits);

    if (w < wid2)
        return -1;
    if (w > wid2)
        return 1;

    return 0;
}

static int search_trigram(
    IN ENGINE*              eng,
    IN LDB_WID              wid1,
    IN LDB_WID              wid2,
    IN LDB_WID              wid3)
{
    if (eng->trigram)
    {
        int                     s;
        int                     e;
        int                     i;
        int                     k;
        LDB_WID                 w;
        int                     c;

        if ( (wid1 == eng->tc_wid1) && (wid2 == eng->tc_wid2) )
        {
            s = eng->tc_start;
            e = eng->tc_end;
        }
        else
        {
            k = -1;
            s = 0;
            e = eng->trigram_count - 1;

            while (s <= e)
            {
                i = (s + e) / 2;
                c = compare_trigram_index(eng, i, wid1, wid2);
                if (c < 0)
                {
                    s = i + 1;
                }
                else if (c > 0)
                {
                    e = i - 1;
                }
                else /* c == 0 */
                {
                    k = i;
                    break;
                }
            }

            eng->tc_wid1 = wid1;
            eng->tc_wid2 = wid2;

            if (k == -1)
            {
                eng->tc_start = 0;
                eng->tc_end = -1;
                return KMX_FALSE;
            }

            s = k;
            while ( (s - 1 >= 0) && (compare_trigram_index(eng, s - 1, wid1, wid2) == 0) )
                s--;

            e = k;
            while ( (e + 1 < eng->trigram_count) && (compare_trigram_index(eng, e + 1, wid1, wid2) == 0) )
                e++;

            eng->tc_start = s;
            eng->tc_end = e;
        }

        while (s <= e)
        {
            i = (s + e) / 2;
            w = (LDB_WID) read_bits(eng->trigram, i * eng->word_id_bits * 3 + eng->word_id_bits * 2, eng->word_id_bits);

            if (wid3 < w)
                e = i - 1;
            else if (wid3 > w)
                s = i + 1;
            else /* wid3 == w */
                return KMX_TRUE;
        }
    }

    return KMX_FALSE;
}

static int get_word_code_index(
    IN ENGINE*              eng,
    IN LDB_WID              wid)
{
    KMX_ASSERT(wid < eng->word_count);

    return read_bits(
        eng->word_table,
        wid * (eng->word_level_bits + eng->word_index_bits) + eng->word_level_bits,
        eng->word_index_bits);
}

static int get_word_level(
    IN ENGINE*              eng,
    IN LDB_WID              wid)
{
    KMX_ASSERT(wid < eng->word_count);

    return read_bits(
        eng->word_table,
        wid * (eng->word_level_bits + eng->word_index_bits),
        eng->word_level_bits);
}

static int get_word_code(
    IN ENGINE*              eng,
    IN int                  index)
{
    KMX_ASSERT(index >= 0);
    KMX_ASSERT(index < eng->string_table_length);

    return read_bits(
        eng->string_table,
        index * eng->char_code_bits,
        eng->char_code_bits);
}

static void get_word_string(
    IN ENGINE*              eng,
    IN LDB_WID              wid,
    OUT LDB_CHAR*           word_chr_string,
    OUT LDB_CHAR*           word_key_string,
    IN int                  length)
{
    int                     c;
    int                     bits;
    int                     bit_pos;

    bits = eng->char_code_bits;
    bit_pos = get_word_code_index(eng, wid) * bits;

    while (length > 0)
    {
        c = read_bits(eng->string_table, bit_pos, bits);
        if (word_chr_string)
            *word_chr_string++ = eng->char_table[c];
        if (word_key_string)
            *word_key_string++ = eng->ckey_table[c];
        bit_pos += bits;
        length--;
    }
}

static LDB_WID search_dictionary(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      string,
    IN int                  length)
{
    int                     i;
    int                     c;
    LDB_WID                 wid;
    LDB_WID                 end;
    LDB_BYTE                code[MAX_WORD_LENGTH];

    KMX_ASSERT(string);
    KMX_ASSERT(length > 0);

    if (length > eng->max_word_length)
        return eng->wid_oov;

    for (i = 0; i < length; i++)
    {
        c = sorted_strchr(eng->char_table, eng->char_code_count, lower_case(eng, string[i]));
        if (c >= 0)
            code[i] = (LDB_BYTE)c;
        else
            return eng->wid_oov;
    }

    if (length == 1)
    {
        c = eng->ckey_table[code[0]];
    }
    else
    {
        c = eng->key_count
            + (length - 2) * eng->key_count * eng->key_count
            + eng->ckey_table[code[0]] * eng->key_count
            + eng->ckey_table[code[1]];
    }


    wid = (LDB_WID) read_bits(eng->word_index, c * eng->word_id_bits, eng->word_id_bits);
    end = (LDB_WID) read_bits(eng->word_index, (c + 1) * eng->word_id_bits, eng->word_id_bits);

    while (wid < end)
    {
        c = get_word_code_index(eng, wid);
        for (i = 0; i < length; i++)
        {
            if (code[i] != get_word_code(eng, c + i))
                break;
        }
        if (i == length)
            return wid;
        wid++;
    }

    return eng->wid_oov;
}

static LDB_BYTE chr_to_key(
    IN ENGINE*              eng,
    IN LDB_CHAR             c)
{
    int                     i;

    if ( (c >= KMX_CHAR_DIGIT_0) && (c <= KMX_CHAR_DIGIT_9) )
    {
        return (LDB_BYTE) (c - KMX_CHAR_DIGIT_0);
    }
    else if ( (c >= KMX_CHAR_ARABIC_DIGIT_0) && (c <= KMX_CHAR_ARABIC_DIGIT_9) )
    {
        return (LDB_BYTE) (c - KMX_CHAR_ARABIC_DIGIT_0);
    }
    else if ( (c >= KMX_CHAR_PERSIAN_DIGIT_0) && (c <= KMX_CHAR_PERSIAN_DIGIT_9) )
    {
        return (LDB_BYTE) (c - KMX_CHAR_PERSIAN_DIGIT_0);
    }
    else if ( (c >= KMX_CHAR_HINDI_DIGIT_0) && (c <= KMX_CHAR_HINDI_DIGIT_9) )
    {
        return (LDB_BYTE) (c - KMX_CHAR_HINDI_DIGIT_0);
    }
    else if ( (c >= KMX_CHAR_THAI_DIGIT_0) && (c <= KMX_CHAR_THAI_DIGIT_9) )
    {
        return (LDB_BYTE) (c - KMX_CHAR_THAI_DIGIT_0);
    }
    else if ((eng->flags & KMX_LDB_ATTR_USE_KEY_STAR) && (c == '*'))
    {
        return KMX_LDB_KEY_STAR;
    }
    else if ((eng->flags & KMX_LDB_ATTR_USE_KEY_POUND) && (c == '#'))
    {
        return KMX_LDB_KEY_POUND;
    }    
    else
    {
        i = sorted_strchr(eng->char_table, eng->char_code_count, c);
        if (i >= 0)
            return eng->ckey_table[i];
        if (eng->is_sure_type)
        {
            if ( (c >= 0x61) && (c <= 0x7A) )
                return BASIC_LATIN_SURE_TYPE_MAP[c - 0x61];
            else
                return 15;
        }
        else
        {
            if ( (c >= 0x61) && (c <= 0x7A) )
                return BASIC_LATIN_9KEY_MAP[c - 0x61];
            else
                return 1;
        }
    }
}

static LDB_BYTE chr_to_group(
     IN ENGINE*              eng,
     IN LDB_CHAR             c)
{
    int i;
    i = sorted_strchr(eng->alphabet, eng->alphabet_length, c);

    if(i >= 0)
        return eng->group_id_table[i];

    return 0;
}

static void fill_group_keys(
                            IN ENGINE*              eng,
                            IN LDB_BYTE             group_id,
                            OUT LDB_BYTE*           group_keys)
{
    int i;
    KMX_ASSERT(group_keys);

    memset(group_keys, 0, sizeof(LDB_BYTE) * MAX_ASSO_KEY_COUNT);
    for (i = 0; i < eng->alphabet_length; i++)
    {
        if (eng->group_id_table[i] == group_id)
        {
            LDB_BYTE key;
            key = chr_to_key(eng, eng->alphabet[i]);
            KMX_ASSERT(key >= 0 && key < MAX_ASSO_KEY_COUNT);
            group_keys[key] |= 1;
        }
    }

}

static int str_group_cmp(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      str1,
    IN const LDB_CHAR*      str2,
    IN int                  length)
{
    int i;
    LDB_BYTE g1;
    LDB_BYTE g2;

    for (i = 0; i < length; i++)
    {
        g1 = chr_to_group(eng, str1[i]);
        g2 = chr_to_group(eng, str2[i]);
        if (g1 != g2)
            return g1 - g2;        
    }

    return 0;
}

static int match_string(
    IN const LDB_CHAR*      string1,
    IN int                  length1,
    IN const LDB_CHAR*      string2,
    IN int                  length2)
{
    int i;
    int n;

    if (length1 <= length2)
        n = length1;
    else
        n = length2;

    for (i = 0; i < n; i++)
    {
        if (string1[i] != string2[i])
            break;
    }

    return i;
}

static int cmp_match_str(
    IN const LDB_CHAR*      str1,
    IN int                  len1,
    IN const LDB_CHAR*      str2,
    IN int                  len2)
{
    int                     i;

    if (len1 < len2)
        return -1;
    else if (len1 > len2)
        return 1;

    for (i = 0; i < len1; i++)
    {
        if (str1[i] < str2[i])
            return -1;
        else if (str1[i] > str2[i])
            return 1;
    }

    return 0;
}

static int is_match_list_full(IN ENGINE* eng)
{
    KMX_ASSERT(eng->match_count <= eng->max_match);

    if (eng->match_count >= eng->max_match)
        return KMX_TRUE;
    else
        return KMX_FALSE;
}

static void add_match(
    IN ENGINE*              eng,
    IN int                  rank,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    int                     s;
    int                     e;
    int                     i;
    int                     c;
    MATCH*                  p;

    if (is_match_list_full(eng))
        return;
    
    #if !defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
    if (eng->language == KMX_LANG_HINDI && len == 1)
    {
        if (!kmx_is_valid_hindi_word_start_char(str[0]))
            return;
    }
    else if (eng->language == KMX_LANG_BENGALI && len == 1)
    {
        if (!kmx_is_valid_bengali_word_start_char(str[0]))
            return;
    }
    #endif

    if (eng->match_count > 0)
    {
        s = 0;
        e = eng->match_count - 1;
        while (s <= e)
        {
            i = (s + e) / 2;
            p = eng->match_list + i;
            c = cmp_match_str(str, len, p->string, p->length);
            if (c < 0)
            {
                e = i - 1;
            }
            else if (c > 0)
            {
                s = i + 1;
            }
            else /* c == 0 */
            {
                if (rank < p->rank)
                    p->rank = rank;
                return;
            }
        }
        KMX_ASSERT( (s >= 0) && (s <= eng->match_count) );
        if (s < eng->match_count)
        {
            memmove(
                eng->match_list + s + 1,
                eng->match_list + s,
                (eng->match_count - s) * sizeof(MATCH));
        }
        p = eng->match_list + s;
    }
    else /* eng->match_count == 0 */
    {
        p = eng->match_list;
    }

    p->rank = rank;
    p->length = len;
    memcpy(p->string, str, len * sizeof(LDB_CHAR));

    eng->match_count++;
}

static int cmp_match(
    IN const void*          e1,
    IN const void*          e2)
{
    const MATCH*            m1;
    const MATCH*            m2;

    m1 = (const MATCH*)e1;
    m2 = (const MATCH*)e2;

    if (m1->rank < m2->rank)
        return -1;
    else if (m1->rank > m2->rank)
        return 1;
    else
        return cmp_match_str(m1->string, m1->length, m2->string, m2->length);
}

static void add_template(
    IN ENGINE*              eng,
    IN int                  rank,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    if (len > 0)
    {
        if ( (len > eng->tmp_len) || (len == eng->tmp_len && rank < eng->tmp_rank) )
        {
            memcpy(eng->tmp_str, str, len * sizeof(LDB_CHAR));
            eng->tmp_len = len;
            eng->tmp_rank = rank;
        }
    }
}

//ZHMCH
static void match_number(IN ENGINE* eng)
{
    int i;
    LDB_CHAR* s;
    LDB_CHAR c;

    if (eng->is_sure_type)
    {
        if (eng->input.length == 1 && eng->input.string[0] == 15)
        {
            for (i = eng->mt_index[15]; i < eng->mt_index[16]; i++)
                add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
        }
        else
        {
            s = eng->buffer1;
            for (i = 0; i < eng->input.length; i++)
            {
                switch (eng->input.string[i])
                {
                    case 10:
                        c = '!';
                        break;
                    case 11:
                        c = '?';
                        break;
                    case 12:
                        c = '@';
                        break;
                    case 13:
                        c = '-';
                        break;
                    case 14:
                        c = '\'';
                        break;
                    case 15:
                        c = '.';
                        break;
                    default:
                        KMX_ASSERT(eng->input.string[i] >= 0 && eng->input.string[i] <= 9);
                        c = (LDB_CHAR) (KMX_CHAR_DIGIT_0 + eng->input.string[i]);
                        break;
                }
                s[i] = c;
            }
            if (eng->input.flags & KMX_LATIN_NUMBER_FIRST)
                add_match(eng, RANK_TOP, s, eng->input.length);
            else
                add_match(eng, RANK_BOTTOM, s, eng->input.length);
        }
    }
    else
    {
        if (eng->language == KMX_LANG_PERSIAN)
            c = KMX_CHAR_PERSIAN_DIGIT_0;
        else
            c = KMX_CHAR_DIGIT_0;
    #ifdef KMX_LDB_HINDI        
        if (eng->language == KMX_LANG_HINDI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_HINDI_DIGIT_0;	
        }        
    #endif

	#ifdef KMX_LDB_NEPALI
        if (eng->language == KMX_LANG_NEPALI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_NEPALI_DIGIT_0;
        }        
	#endif

	#ifdef KMX_LDB_SINHALA
        if (eng->language == KMX_LANG_SINHALA)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_SINHALA_DIGIT_0;
        }        
	#endif

	//daodejing  need  ldbºÅÐèÒªÐÞ¸Ä
	#ifdef KMX_LDB_BODO
        if (eng->language == KMX_LANG_BODO)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_BODO_DIGIT_0;
        }        
	#endif
	#ifdef KMX_LDB_DOGRI
        if (eng->language == KMX_LANG_DOGRI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_DOGRI_DIGIT_0;
        }        
	#endif
	#ifdef KMX_LDB_SANSKRIT
        if (eng->language == KMX_LANG_SANSKRIT)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_SANSKRIT_DIGIT_0;
        }        
	#endif
	#ifdef KMX_LDB_KASHMIRI
        if (eng->language == KMX_LANG_KASHMIRI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_KASHMIRI_DIGIT_0;
        }        
	#endif
	#ifdef KMX_LDB_KONKANI
        if (eng->language == KMX_LANG_KONKANI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_KONKANI_DIGIT_0;
        }        
	#endif
	#ifdef KMX_LDB_MAITHILI
        if (eng->language == KMX_LANG_MAITHILI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_MAITHILI_DIGIT_0;
        }        
	#endif
	#ifdef KMX_LDB_MANIPURI
        if (eng->language == KMX_LANG_MANIPURI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_MANIPURI_DIGIT_0;
        }        
	#endif
	#ifdef KMX_LDB_SANTALI
        if (eng->language == KMX_LANG_SANTALI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_SANTALI_DIGIT_0;
        }        
	#endif
	#ifdef KMX_LDB_SOMALI
        if (eng->language == KMX_LANG_SOMALI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_SOMALI_DIGIT_0;
        }        
	#endif
	#ifdef KMX_LDB_SINDHI
        if (eng->language == KMX_LANG_SINDHI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = KMX_CHAR_SINDHI_DIGIT_0;
        }        
	#endif
	
        #ifdef KMX_LDB_BENGALI
        if (eng->language == KMX_LANG_BENGALI)
        {
            if (eng->did == 924 || eng->did == 922 || eng->did == 929 || eng->did == 9200 || eng->did == 9201 || eng->did == 9202 || eng->did == 9203)
                c = KMX_CHAR_BENGALI_DIGIT_0;
            else
                c = KMX_CHAR_DIGIT_0;
        }
        #endif

        s = eng->buffer1;
        for (i = 0; i < eng->input.length; i++)
        {
            if (eng->input.string[i] == KMX_LDB_KEY_STAR)
                s[i] = '*';
            else if (eng->input.string[i] == KMX_LDB_KEY_POUND)
                s[i] = '#';            
            else
                s[i] = (LDB_CHAR) (c + eng->input.string[i]);
        }

        if (eng->input.flags & KMX_LATIN_NUMBER_FIRST)
            add_match(eng, RANK_TOP, s, eng->input.length);
        else
            add_match(eng, RANK_BOTTOM, s, eng->input.length);

        if (eng->input.length == 1 && eng->input.string[0] == 1)
        {
            for (i = eng->mt_index[1]; i < eng->mt_index[2]; i++)
                add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
        }

#ifdef KMX_LDB_HINDI 
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_HINDI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_NEPALI 
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_NEPALI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_SINHALA 
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_SINHALA && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

//daodejing need
#ifdef KMX_LDB_BODO
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_BODO && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_DOGRI
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_DOGRI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_SANSKRIT
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_SANSKRIT && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_KASHMIRI
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_KASHMIRI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_KONKANI
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_KONKANI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_MAITHILI
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_MAITHILI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_MANIPURI
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_MANIPURI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_SANTALI
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_SANTALI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_SOMALI
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_SOMALI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_SINDHI
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_SINDHI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }
#endif

#ifdef KMX_LDB_BENGALI
    if (eng->did == 923 || eng->did == 924 || eng->did == 922 || eng->did == 929 || eng->did == 9200 || eng->did == 9201 || eng->did == 9202 || eng->did == 9203)
    {
        /* Always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_BENGALI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
            {
                if (eng->mt_string[i] == 0x20)
                {
                    add_match(eng, RANK_TOP, eng->mt_string + i, 1);
                }
                else
                {
                    add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
                }
            }
        }
    }

   /* 9201 && 9202 && 9203 has symbol on key 10 */
    if ((eng->did == 9201 || eng->did == 9202 || eng->did == 9203) && (eng->input.length == 1) && (eng->input.string[0] == 10))
    {
        for (i = eng->mt_index[10]; i < eng->mt_index[11]; i++)
            add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
    }    

   /* 9200 has symbol on key 11 */
    if ((eng->did == 9200) && (eng->input.length == 1) && (eng->input.string[0] == 11))
    {
        for (i = eng->mt_index[11]; i < eng->mt_index[12]; i++)
            add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
    }    
#endif

#ifdef KMX_LDB_THAI
    #ifdef KMX_IME_THAI_KEY0_SPACE_MULTITAP
    if (eng->did == 360 || eng->did == 361 || eng->did == 365 || eng->did == 366 || eng->did == 367 || eng->did == 369)
    {
        /* Append space 0x20 candidate and always place space as the first candidate of the 0 */
        if (eng->language == KMX_LANG_THAI && eng->input.length == 1 && eng->input.string[0] == 0)
        {
            add_match(eng, RANK_TOP, L" ", 1);
        }
    }
    #endif
#endif 

    }
}

static void match_cache_word(IN ENGINE* eng)
{
    int                     i;
    int                     is_bigram_matched;
    int                     is_trigram_matched;
    int                     rank_level;
    int                     rank_value;
    UDB_HIT                 hit;
    LDB_WID                 wid;
    UDB_HIT                 hit1;
    LDB_WID                 hwid1;
    UDB_HIT                 hit2;
    LDB_WID                 hwid2;
    UDB_OIT                 oit;
    int                     match_length;
    LDB_CHAR*               word_chr_string;
    LDB_CHAR*               word_key_string;
    int                     word_length;

    word_chr_string = eng->buffer1;
    word_key_string = eng->buffer2;

    rank_value = 0;

    hit = udb_his_first(eng);
    while (hit)
    {
        wid = udb_his_word(eng, hit);
        word_length = udb_his_len(eng, hit);

        if ( (wid != eng->wid_sos) && (word_length >= eng->input.length) )
        {
            /* Check if previous two words match history */

            hwid1 = eng->wid_sos;
            hwid2 = eng->wid_sos;
            hit2 = udb_his_next(eng, hit);
            if (hit2)
            {
                hwid2 = udb_his_word(eng, hit2);
                if (hwid2 != eng->wid_sos)
                {
                    hit1 = udb_his_next(eng, hit2);
                    if (hit1)
                        hwid1 = udb_his_word(eng, hit1);
                }
            }
            is_trigram_matched = KMX_FALSE;
            is_bigram_matched = KMX_FALSE;
            if (hwid2 == eng->input.wid2)
            {
                if (hwid1 == eng->input.wid1)
                    is_trigram_matched = KMX_TRUE;
                else
                    is_bigram_matched = KMX_TRUE;
            }

            /* Match cache word against input string */

            if (wid == eng->wid_oov)
            {
                memcpy(word_chr_string, udb_his_str(eng, hit), sizeof(LDB_CHAR) * word_length);
                string_convert_case(eng, word_chr_string, word_length, KMX_LATIN_LOWER_CASE);
                for (i = 0; i < word_length; i++)
                    word_key_string[i] = chr_to_key(eng, word_chr_string[i]);
                match_length = match_string(
                    eng->input.string, eng->input.length,
                    word_key_string, word_length);
                if (match_length == eng->input.length)
                {
                    if (match_length == word_length)
                    {
                        if (is_trigram_matched)
                            rank_level = RANK_OOVT;
                        else if (is_bigram_matched)
                            rank_level = RANK_OOVB;
                        else
                            rank_level = RANK_OOVU;
                        string_convert_case(eng, word_chr_string, word_length, eng->input.flags);
                        add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                    }
                    else /* match_length < word_length */
                    {
                        if ( (eng->input.length >= FORWARD_MATCH) && (eng->language != KMX_LANG_VIETNAMESE) )
                        {
                            if (is_trigram_matched)
                                rank_level = RANK_FOOVT;
                            else if (is_bigram_matched)
                                rank_level = RANK_FOOVB;
                            else
                                rank_level = RANK_FOOVU;
                            string_convert_case(eng, word_chr_string, word_length, eng->input.flags);
                            add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                        }
                    }
                }
                else /* match_length < input_length */
                {
                    if (match_length == word_length)
                    {
                        if (is_trigram_matched)
                            rank_level = RANK_OOVT;
                        else if (is_bigram_matched)
                            rank_level = RANK_OOVB;
                        else
                            rank_level = RANK_OOVU;
                    }
                    else /* match_length < word_length */
                    {
                        if (is_trigram_matched)
                            rank_level = RANK_TOOVT;
                        else if (is_bigram_matched)
                            rank_level = RANK_TOOVB;
                        else
                            rank_level = RANK_TOOVU;
                    }
                    add_template(eng, rank_level + rank_value, word_chr_string, match_length);
                }
            }
            else
            {
                get_word_string(eng, wid, word_chr_string, word_key_string, word_length);
                match_length = match_string(
                    eng->input.string, eng->input.length,
                    word_key_string, word_length);
                if (match_length == eng->input.length)
                {
                    if (match_length == word_length)
                    {
                        if (is_trigram_matched)
                            rank_level = RANK_CACT;
                        else if (is_bigram_matched)
                            rank_level = RANK_CACB;
                        else
                            rank_level = RANK_CACU;
                        string_convert_case(eng, word_chr_string, word_length, eng->input.flags);
                        add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                    }
                    else /* match_length < word_length */
                    {
                        if ( (eng->input.length >= FORWARD_MATCH) && (eng->language != KMX_LANG_VIETNAMESE) )
                        {
                            if (is_trigram_matched)
                                rank_level = RANK_FCACT;
                            else if (is_bigram_matched)
                                rank_level = RANK_FCACB;
                            else
                                rank_level = RANK_FCACU;
                            string_convert_case(eng, word_chr_string, word_length, eng->input.flags);
                            add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                        }
                    }
                }
                else /* match_length < input_length */
                {
                    if (match_length == word_length)
                    {
                        if (is_trigram_matched)
                            rank_level = RANK_CACT;
                        else if (is_bigram_matched)
                            rank_level = RANK_CACB;
                        else
                            rank_level = RANK_CACU;
                    }
                    else /* match_length < word_length */
                    {
                        if (is_trigram_matched)
                            rank_level = RANK_TCACT;
                        else if (is_bigram_matched)
                            rank_level = RANK_TCACB;
                        else
                            rank_level = RANK_TCACU;
                    }
                    add_template(eng, rank_level + rank_value, word_chr_string, match_length);
                }
            }
        }

        hit = udb_his_next(eng, hit);
        rank_value++;
    }

    oit = udb_oov_first(eng);
    while (oit)
    {
        word_length = udb_oov_len(eng, oit);
        if (word_length >= eng->input.length)
        {
            /* Match cache word against input string */

            memcpy(word_chr_string, udb_oov_str(eng, oit), sizeof(LDB_CHAR) * word_length);
            string_convert_case(eng, word_chr_string, word_length, KMX_LATIN_LOWER_CASE);
            for (i = 0; i < word_length; i++)
                word_key_string[i] = chr_to_key(eng, word_chr_string[i]);
            match_length = match_string(
                eng->input.string, eng->input.length,
                word_key_string, word_length);
            if (match_length == eng->input.length)
            {
                if (match_length == word_length)
                {
                    rank_level = RANK_OOVU;
                    string_convert_case(eng, word_chr_string, word_length, eng->input.flags);
                    add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                }
                else /* match_length < word_length */
                {
                    if ( (eng->input.length >= FORWARD_MATCH) && (eng->language != KMX_LANG_VIETNAMESE) )
                    {
                        rank_level = RANK_FOOVU;
                        string_convert_case(eng, word_chr_string, word_length, eng->input.flags);
                        add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                    }
                }
            }
            else /* match_length < input_length */
            {
                if (match_length == word_length)
                    rank_level = RANK_OOVU;
                else /* match_length < word_length */
                    rank_level = RANK_TOOVU;
                add_template(eng, rank_level + rank_value, word_chr_string, match_length);
            }
        }

        oit = udb_oov_next(eng, oit);
        rank_value++;
    }
}

static void match_cache_asso(IN ENGINE* eng)
{
    int                     rank_value;
    UDB_HIT                 hit;
    LDB_WID                 wid;
    UDB_OIT                 oit;
    LDB_CHAR*               word_string;
    int                     word_length;

    word_string = eng->buffer1;

    rank_value = 0;

    hit = udb_his_first(eng);
    while (hit)
    {
        wid = udb_his_word(eng, hit);
        word_length = udb_his_len(eng, hit);

        if ( (wid != eng->wid_sos) && (word_length > eng->input.length) && (word_length <= eng->input.max_match_length) )
        {
            if (wid == eng->wid_oov)
            {
                memcpy(word_string, udb_his_str(eng, hit), sizeof(LDB_CHAR) * word_length);
                string_convert_case(eng, word_string, word_length, KMX_LATIN_LOWER_CASE);
                if (memcmp(word_string, eng->input.string, sizeof(LDB_CHAR) * eng->input.length) == 0)
                {
                    string_convert_case(eng, word_string, word_length, eng->input.flags);
                    add_match(eng, RANK_OOVU + rank_value, word_string, word_length);
                    if (is_match_list_full(eng))
                        return;
                }
            }
            else
            {
                get_word_string(eng, wid, word_string, 0, word_length);
                if (memcmp(word_string, eng->input.string, sizeof(LDB_CHAR) * eng->input.length) == 0)
                {
                    string_convert_case(eng, word_string, word_length, eng->input.flags);
                    add_match(eng, RANK_CACU + rank_value, word_string, word_length);
                    if (is_match_list_full(eng))
                        return;
                }
            }
        }

        hit = udb_his_next(eng, hit);
        rank_value++;
    }

    oit = udb_oov_first(eng);
    while (oit)
    {
        word_length = udb_oov_len(eng, oit);
        if (word_length > eng->input.length && word_length <= eng->input.max_match_length)
        {
            memcpy(word_string, udb_oov_str(eng, oit), sizeof(LDB_CHAR) * word_length);
            string_convert_case(eng, word_string, word_length, KMX_LATIN_LOWER_CASE);
            if (memcmp(word_string, eng->input.string, sizeof(LDB_CHAR) * eng->input.length) == 0)
            {
                string_convert_case(eng, word_string, word_length, eng->input.flags);
                add_match(eng, RANK_OOVU + rank_value, word_string, word_length);
                if (is_match_list_full(eng))
                    return;
            }
        }

        oit = udb_oov_next(eng, oit);
        rank_value++;
    }
}

static void match_cache_asso_ex(IN ENGINE* eng)
{
    int                     rank_value;
    UDB_HIT                 hit;
    LDB_WID                 wid;
    UDB_OIT                 oit;
    LDB_CHAR*               word_string;
    int                     word_length;

    word_string = eng->buffer1;

    rank_value = 0;

    hit = udb_his_first(eng);
    while (hit)
    {
        wid = udb_his_word(eng, hit);
        word_length = udb_his_len(eng, hit);

        if ( (wid != eng->wid_sos) && (word_length > eng->input.length) && (word_length <= eng->input.max_match_length) )
        {
            if (wid == eng->wid_oov)
            {
                memcpy(word_string, udb_his_str(eng, hit), sizeof(LDB_CHAR) * word_length);
                string_convert_case(eng, word_string, word_length, KMX_LATIN_LOWER_CASE);
                if (str_group_cmp(eng, word_string, eng->input.string, eng->input.length) == 0)
                {
                    string_convert_case(eng, word_string, word_length, eng->input.flags);
                    add_match(eng, RANK_OOVU + rank_value, word_string, word_length);
                    if (is_match_list_full(eng))
                        return;
                }
            }
            else
            {
                get_word_string(eng, wid, word_string, 0, word_length);
                if (str_group_cmp(eng, word_string, eng->input.string, eng->input.length) == 0)
                {
                    string_convert_case(eng, word_string, word_length, eng->input.flags);
                    add_match(eng, RANK_CACU + rank_value, word_string, word_length);
                    if (is_match_list_full(eng))
                        return;
                }
            }
        }

        hit = udb_his_next(eng, hit);
        rank_value++;
    }

    oit = udb_oov_first(eng);
    while (oit)
    {
        word_length = udb_oov_len(eng, oit);
        if (word_length > eng->input.length && word_length <= eng->input.max_match_length)
        {
            memcpy(word_string, udb_oov_str(eng, oit), sizeof(LDB_CHAR) * word_length);
            string_convert_case(eng, word_string, word_length, KMX_LATIN_LOWER_CASE);
            if (str_group_cmp(eng, word_string, eng->input.string, eng->input.length) == 0)
            {
                string_convert_case(eng, word_string, word_length, eng->input.flags);
                add_match(eng, RANK_OOVU + rank_value, word_string, word_length);
                if (is_match_list_full(eng))
                    return;
            }
        }

        oit = udb_oov_next(eng, oit);
        rank_value++;
    }
}


static int filter_greek_word(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      str,
    IN int                  len)
/*
 * Returns KMX_TRUE if given word should be filtered.
 */
{
    if (len == 1 && (eng->input.flags & KMX_LATIN_UPPER_CASE))
    {
        if (*str == 0x390 || *str == 0x3B0)
            return KMX_TRUE;
    }
    return KMX_FALSE;
}

static void match_dict_word(IN ENGINE* eng)
{
    int                     i;
    int                     k0;
    int                     k1;
    LDB_WID                 wid;
    LDB_WID                 swid;
    LDB_WID                 ewid;
    int                     match_length;
    int                     rank_level;
    int                     rank_value;
    LDB_CHAR*               word_chr_string;
    LDB_CHAR*               word_key_string;
    int                     word_length;

    word_chr_string = eng->buffer1;
    word_key_string = eng->buffer2;

    KMX_ASSERT(eng->input.length > 0);
    k0 = eng->input.string[0];

    if (eng->input.length == 1)
        k1 = 0;
    else
        k1 = eng->key_count + k0 * eng->key_count + eng->input.string[1];

    /* Search candidates with the same length */

    if (eng->input.length <= eng->max_word_length)
    {
        word_length = eng->input.length;

        if (word_length == 1)
            i = k0;
        else
            i = k1 + (word_length - 2) * eng->key_count * eng->key_count;
        

        swid = (LDB_WID) read_bits(eng->word_index, i * eng->word_id_bits, eng->word_id_bits);
        ewid = (LDB_WID) read_bits(eng->word_index, (i + 1) * eng->word_id_bits, eng->word_id_bits);
        for (wid = swid; wid < ewid; wid++)
        {
            if (search_exgram(eng, eng->input.wid2, wid))
            {
                eng->is_exgram_found = KMX_TRUE;
                continue;
            }
            get_word_string(eng, wid, word_chr_string, word_key_string, word_length);
            if (eng->language == KMX_LANG_GREEK && filter_greek_word(eng, word_chr_string, word_length))
                continue;
            match_length = match_string(
                eng->input.string, word_length,
                word_key_string, word_length);
            if (match_length == word_length)
            {
                if (search_trigram(eng, eng->input.wid1, eng->input.wid2, wid))
                    rank_level = RANK_DICT;
                else if (search_bigram(eng, eng->input.wid2, wid))
                    rank_level = RANK_DICB;
                else
                    rank_level = RANK_DICU;
                rank_value = get_word_level(eng, wid);
                string_convert_case(eng, word_chr_string, word_length, eng->input.flags);
                add_match(eng, rank_level + rank_value, word_chr_string, word_length);
            }
            else if (match_length >= eng->tmp_len)
            {
                if (search_trigram(eng, eng->input.wid1, eng->input.wid2, wid))
                    rank_level = RANK_TDICT;
                else if (search_bigram(eng, eng->input.wid2, wid))
                    rank_level = RANK_TDICB;
                else
                    rank_level = RANK_TDICU;
                rank_value = get_word_level(eng, wid);
                add_template(eng, rank_level + rank_value, word_chr_string, match_length);
            }
        }
    }

    /* Search candidates with the same key string prefix */

    if (eng->input.length > 1)
    {
        for (word_length = eng->max_word_length; word_length > eng->input.length; word_length--)
        {
            i = k1 + (word_length - 2) * eng->key_count * eng->key_count;

            swid = (LDB_WID) read_bits(eng->word_index, i * eng->word_id_bits, eng->word_id_bits);
            ewid = (LDB_WID) read_bits(eng->word_index, (i + 1) * eng->word_id_bits, eng->word_id_bits);
            for (wid = swid; wid < ewid; wid++)
            {
                if (search_exgram(eng, eng->input.wid2, wid))
                    continue;
                get_word_string(eng, wid, word_chr_string, word_key_string, word_length);
                if (eng->language == KMX_LANG_GREEK && filter_greek_word(eng, word_chr_string, word_length))
                    continue;
                match_length = match_string(
                    eng->input.string, eng->input.length,
                    word_key_string, word_length);
                if (match_length == eng->input.length)
                {
                    if (search_trigram(eng, eng->input.wid1, eng->input.wid2, wid))
                        rank_level = RANK_FDICT;
                    else if (search_bigram(eng, eng->input.wid2, wid))
                        rank_level = RANK_FDICB;
                    else
                        rank_level = RANK_FDICU;
                    rank_value = get_word_level(eng, wid);
                    if ( (eng->input.length >= FORWARD_MATCH) && (eng->language != KMX_LANG_VIETNAMESE) )
                    {
                        string_convert_case(eng, word_chr_string, word_length, eng->input.flags);
                        add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                    }
                    else if (eng->input.length >= 3)
                    {
                        string_convert_case(eng, word_chr_string, word_length, eng->input.flags);
                        add_match(eng, rank_level + rank_value, word_chr_string, match_length);
                    }
                }
                else if (match_length >= eng->tmp_len)
                {
                    if (search_trigram(eng, eng->input.wid1, eng->input.wid2, wid))
                        rank_level = RANK_TDICT;
                    else if (search_bigram(eng, eng->input.wid2, wid))
                        rank_level = RANK_TDICB;
                    else
                        rank_level = RANK_TDICU;
                    rank_value = get_word_level(eng, wid);
                    add_template(eng, rank_level + rank_value, word_chr_string, match_length);
                }
            }
        }
    }

    for (word_length = eng->input.length - 1; (word_length >= eng->tmp_len) && (word_length >= 2); word_length--)
    {
        if (word_length <= eng->max_word_length)
        {
            i = k1 + (word_length - 2) * eng->key_count * eng->key_count;

            swid = (LDB_WID) read_bits(eng->word_index, i * eng->word_id_bits, eng->word_id_bits);
            ewid = (LDB_WID) read_bits(eng->word_index, (i + 1) * eng->word_id_bits, eng->word_id_bits);
            for (wid = swid; wid < ewid; wid++)
            {
                if (search_exgram(eng, eng->input.wid2, wid))
                    continue;
                get_word_string(eng, wid, word_chr_string, word_key_string, word_length);
                if (eng->language == KMX_LANG_GREEK && filter_greek_word(eng, word_chr_string, word_length))
                    continue;
                match_length = match_string(
                    eng->input.string, eng->input.length,
                    word_key_string, word_length);
                if (match_length >= eng->tmp_len)
                {
                    if (match_length == word_length)
                    {
                        if (search_trigram(eng, eng->input.wid1, eng->input.wid2, wid))
                            rank_level = RANK_DICT;
                        else if (search_bigram(eng, eng->input.wid2, wid))
                            rank_level = RANK_DICB;
                        else
                            rank_level = RANK_DICU;
                        rank_value = get_word_level(eng, wid);
                        add_template(eng, rank_level + rank_value, word_chr_string, match_length);
                    }
                    else /* match_length <= word_length */
                    {
                        if (search_trigram(eng, eng->input.wid1, eng->input.wid2, wid))
                            rank_level = RANK_TDICT;
                        else if (search_bigram(eng, eng->input.wid2, wid))
                            rank_level = RANK_TDICB;
                        else
                            rank_level = RANK_TDICU;
                        rank_value = get_word_level(eng, wid);
                        add_template(eng, rank_level + rank_value, word_chr_string, match_length);
                    }
                }
            }
        }
    }
}

static void match_dict_asso_by_index(
    IN ENGINE*              eng,
    IN int                  index,
    IN int                  word_length)
{
    LDB_CHAR*               word_string;
    LDB_WID                 wid;
    LDB_WID                 swid;
    LDB_WID                 ewid;

    KMX_ASSERT(word_length > eng->input.length && word_length <= eng->input.max_match_length);

    word_string = eng->buffer1;

    swid = (LDB_WID) read_bits(eng->word_index, index       * eng->word_id_bits, eng->word_id_bits);
    ewid = (LDB_WID) read_bits(eng->word_index, (index + 1) * eng->word_id_bits, eng->word_id_bits);

    for (wid = swid; wid < ewid; wid++)
    {
        get_word_string(eng, wid, word_string, 0, word_length);
        if (eng->language == KMX_LANG_GREEK && filter_greek_word(eng, word_string, word_length))
            continue;

        if (memcmp(word_string, eng->input.string, sizeof(LDB_CHAR) * eng->input.length) == 0)
        {
            int rank_value;

            rank_value = get_word_level(eng, wid);
            string_convert_case(eng, word_string, word_length, eng->input.flags);
            add_match(eng, RANK_FDICU + rank_value, word_string, word_length);

            if (is_match_list_full(eng))
                return;
        }
    }
}

static void match_dict_asso_by_index_ex(
    IN ENGINE*              eng,
    IN int                  index,
    IN int                  word_length)
{
    LDB_CHAR*               word_string;
    LDB_WID                 wid;
    LDB_WID                 swid;
    LDB_WID                 ewid;

    KMX_ASSERT(word_length >= eng->input.length && word_length <= eng->input.max_match_length);

    word_string = eng->buffer1;

    swid = (LDB_WID) read_bits(eng->word_index, index       * eng->word_id_bits, eng->word_id_bits);
    ewid = (LDB_WID) read_bits(eng->word_index, (index + 1) * eng->word_id_bits, eng->word_id_bits);

    for (wid = swid; wid < ewid; wid++)
    {
        get_word_string(eng, wid, word_string, 0, word_length);
        if (eng->language == KMX_LANG_GREEK && filter_greek_word(eng, word_string, word_length))
            continue;

        if (str_group_cmp(eng, word_string, eng->input.string, eng->input.length) == 0)
        {
            int rank_value;

            rank_value = get_word_level(eng, wid);
            string_convert_case(eng, word_string, word_length, eng->input.flags);
            add_match(eng, RANK_FDICU + rank_value, word_string, word_length);

            if (is_match_list_full(eng))
                return;
        }
    }
}

static void match_dict_asso_by_index_ex_at_top_rank(
    IN ENGINE*              eng,
    IN int                  index,
    IN int                  word_length)
{
    LDB_CHAR*               word_string;
    LDB_WID                 wid;
    LDB_WID                 swid;
    LDB_WID                 ewid;

    KMX_ASSERT(word_length >= eng->input.length && word_length <= eng->input.max_match_length);

    word_string = eng->buffer1;

    swid = (LDB_WID) read_bits(eng->word_index, index       * eng->word_id_bits, eng->word_id_bits);
    ewid = (LDB_WID) read_bits(eng->word_index, (index + 1) * eng->word_id_bits, eng->word_id_bits);

    for (wid = swid; wid < ewid; wid++)
    {
        get_word_string(eng, wid, word_string, 0, word_length);
        if (eng->language == KMX_LANG_GREEK && filter_greek_word(eng, word_string, word_length))
            continue;

        if (str_group_cmp(eng, word_string, eng->input.string, eng->input.length) == 0)
        {
            int rank_value;

            rank_value = get_word_level(eng, wid);
            string_convert_case(eng, word_string, word_length, eng->input.flags);
            add_match(eng, RANK_TOP + rank_value, word_string, word_length);

            if (is_match_list_full(eng))
                return;
        }
    }
}

static void match_dict_asso(IN ENGINE* eng)
{
    int                     i;
    int                     k0;
    int                     k1;
    int                     word_length;

    KMX_ASSERT(eng->input.length > 0);

    k0 = chr_to_key(eng, eng->input.string[0]);

    if (eng->input.length == 1)
    {
        for (word_length = 2; word_length <= eng->input.max_match_length; word_length++)
        {
            for (k1 = 0; k1 < eng->key_count; k1++)
            {
                i = eng->key_count + (word_length - 2) * eng->key_count * eng->key_count + k0 * eng->key_count + k1;
                match_dict_asso_by_index(eng, i, word_length);
                if (is_match_list_full(eng))
                    return;
            }            
        }
    }
    else
    {
        k1 = chr_to_key(eng, eng->input.string[1]);
        for (word_length = eng->input.length + 1; word_length <= eng->input.max_match_length; word_length++)
        {
            i = eng->key_count + (word_length - 2) * eng->key_count * eng->key_count + k0 * eng->key_count + k1;
            match_dict_asso_by_index(eng, i, word_length);
            if (is_match_list_full(eng))
                return;
        }
    }
}

static void match_dict_asso_ex(IN ENGINE* eng)
{
    int                     i;
    int                     j;
    
    int                     k0;
    int                     k1;
    LDB_BYTE                g0;
    LDB_BYTE                g1;
    int                     word_length;

    LDB_BYTE                g0_keys[MAX_ASSO_KEY_COUNT];
    LDB_BYTE                g1_keys[MAX_ASSO_KEY_COUNT];
    

    KMX_ASSERT(eng->input.length > 0);
    
    g0 = chr_to_group(eng, eng->input.string[0]);
    fill_group_keys(eng, g0, g0_keys);

    if (eng->input.length > 1)
    {
        g1 = chr_to_group(eng, eng->input.string[1]);
        fill_group_keys(eng, g1, g1_keys);
    }        

    for (j = 0; j < MAX_ASSO_KEY_COUNT; j++)
    {
        if (!g0_keys[j])
            continue;

        k0 = j;
        if (eng->input.length == 1)
        {         
            for (word_length = 2; word_length <= eng->input.max_match_length; word_length++)
            {                     
                for (k1 = 0; k1 < eng->key_count; k1++)
                {
                    i = eng->key_count + (word_length - 2) * eng->key_count * eng->key_count + k0 * eng->key_count + k1;

                    match_dict_asso_by_index_ex(eng, i, word_length);
                    if (is_match_list_full(eng))
                        return;
                }            
            }
        }
        else
        {
            int l;

            for (l = 0; l < MAX_ASSO_KEY_COUNT; l++)
            {
                if (!g1_keys[l])
                    continue;
                
                k1 = l;                
                for (word_length = eng->input.length + 1; word_length <= eng->input.max_match_length; word_length++)
                {
                    i = eng->key_count + (word_length - 2) * eng->key_count * eng->key_count + k0 * eng->key_count + k1;

                    match_dict_asso_by_index_ex(eng, i, word_length);
                    if (is_match_list_full(eng))
                        return;
                }
            }            
        }
    }        
}

static void match_prefix(IN ENGINE* eng)
{
    int                     i;
    MATCH*                  p1;
    MATCH*                  p2;

    if (eng->language == KMX_LANG_VIETNAMESE)
        return;             /* Forward match is disabled for Vietnamese */

    if (eng->input.length < FORWARD_MATCH)
        return;

    p1 = 0;
    p2 = eng->match_list;
    for (i = 0; i < eng->match_count; i++, p2++)
    {
        KMX_ASSERT(p2->length >= eng->input.length);
        if (p2->rank == RANK_TOP || p2->rank == RANK_BOTTOM)
            continue;
        if (p2->length == eng->input.length)
            return;
        if (p1)
        {
            if (p2->rank < p1->rank)
                p1 = p2;
        }
        else
        {
            p1 = p2;
        }
    }

    if (p1)
    {
        LDB_CHAR* s;

        s = eng->buffer1;
        memcpy(s, p1->string, sizeof(LDB_CHAR) * eng->input.length);
        add_match(eng, RANK_PREFIX, s, eng->input.length);
    }
}

static void match_legram(IN ENGINE* eng)
{
    int                     i;
    int                     j;
    int                     legram_count;
    const LDB_CHAR*         legram_list;
    const LDB_CHAR*         k;

    /* Do not generate legram match if a normal extra match exist */    
    #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
    /* Skip this check for Context sensitive first char */
    if (!(eng->input.length == 1 && 
         (eng->language == KMX_LANG_HINDI 
         || eng->language == KMX_LANG_THAI 
         || eng->language == KMX_LANG_BENGALI)
         )
        )
    #endif
    for (i = 0; i < eng->match_count; i++)
    {
        if ( (eng->match_list[i].rank != RANK_TOP) &&
             (eng->match_list[i].rank != RANK_BOTTOM) )
            return;
    }

    /* Generate legram matchs from template */

    KMX_ASSERT(eng->tmp_len < eng->input.length);

    k = eng->input.string;

    i = eng->tmp_len;
    while (i + 1 < eng->input.length)
    {
        legram_list = eng->legram_string + eng->legram_index[k[i]];
        if (eng->language == KMX_LANG_THAI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(KMX_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && kmx_is_valid_thai_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && kmx_is_valid_thai_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
        else if (eng->language == KMX_LANG_HINDI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(KMX_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && kmx_is_valid_hindi_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && kmx_is_valid_hindi_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
        else if (eng->language == KMX_LANG_BENGALI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(KMX_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && kmx_is_valid_bengali_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && kmx_is_valid_bengali_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
        else
        {
            eng->tmp_str[i] = legram_list[0];
        }
        i++;
    }

    legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
    legram_list = eng->legram_string + eng->legram_index[k[i]];

    if (eng->language == KMX_LANG_GREEK && (eng->input.flags & KMX_LATIN_UPPER_CASE))
    {
        if (k[i] == 4 || k[i] == 8)
            legram_count--;
    }

    if (eng->language == KMX_LANG_THAI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                eng->input.length == 1 || 
                #endif
                kmx_is_valid_thai_follow_char(0, legram_list[j])
                )    
               )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
                found_match = 1;
            }
            else if (i > 0 && kmx_is_valid_thai_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(KMX_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
        }
    }
    else if (eng->language == KMX_LANG_HINDI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                eng->input.length == 1 || 
                #endif
                kmx_is_valid_hindi_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
                found_match = 1;
            }
            else if (i > 0 && kmx_is_valid_hindi_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(KMX_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
        }
    }
    else if (eng->language == KMX_LANG_BENGALI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                eng->input.length == 1 || 
                #endif
                kmx_is_valid_bengali_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
                found_match = 1;
            }
            else if (i > 0 && kmx_is_valid_bengali_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(KMX_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
        }
    }
    else
    {
        for (j = 0; j < legram_count; j++)
        {
            eng->tmp_str[i] = legram_list[j];
            string_convert_case(eng, eng->tmp_str, eng->input.length, eng->input.flags);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, eng->input.length);
        }
    }

    if (eng->language == KMX_LANG_ENGLISH && !eng->is_sure_type)
    {
        if ( (eng->tmp_len <= eng->input.length - 2) &&
             (k[eng->input.length - 2] == 1) &&
             (k[eng->input.length - 1] == 7) )
        {
            eng->tmp_str[eng->input.length - 2] = KMX_CHAR_APOSTROPHE;
            eng->tmp_str[eng->input.length - 1] = 0x73;
            string_convert_case(eng, eng->tmp_str, eng->input.length, eng->input.flags);
            add_match(eng, RANK_LEGRAM, eng->tmp_str, eng->input.length);
        }
    }
}

static void parse_history(
    IN ENGINE*                  eng,
    IN const KMX_INPUT_CONTEXT* context)
{
    LDB_WID                 w[2];
    LDB_WID                 wid;
    int                     count;
    const LDB_CHAR*         scan;
    const LDB_CHAR*         start;
    const LDB_CHAR*         word;
    int                     cc;
    UDB_HIT                 hit;

    w[0] = w[1] = eng->wid_sos;
    count = 0;

    /* Check input history first */

    if ( context && context->text_before && context->text_before_length > 0)
    {
        /* Scan history from end toward start */

        start = context->text_before;
        scan = start + context->text_before_length - 1;

        while (scan >= start)
        {
            /* Find a word delimited by space */

            while ( (scan >= start) && (*scan == KMX_CHAR_SPACE) )
                scan--;

            cc = 0;
            while ( (scan >= start) && (*scan != KMX_CHAR_SPACE) )
            {
                scan--;
                cc++;
            }
            word = scan + 1;
            
            /* Stop search at SOS delimiters */

            if ( (cc > 0) && is_sos_delimiter(eng, word[cc-1]) )
            {
                count = 2;
                break;
            }

            /* Search the word in dictionay */
            /* Words which exceed max length is ignored */

            if ( (cc > 0) && (cc <= MAX_WORD_LENGTH) )
            {
                w[count] = search_dictionary(eng, word, cc);
                count++;
                if (count == 2)
                    break;
            }
        }
    }

    /* If not enough words found in given history, then search cache */

    if (count < 2)
    {
        hit = udb_his_first(eng);
        if (hit)
        {
            wid = udb_his_word(eng, hit);
            if (wid != eng->wid_sos)
            {
                w[count] = wid;
                count++;
                if (count < 2)
                {
                    hit = udb_his_next(eng, hit);
                    if (hit)
                        w[count] = udb_his_word(eng, hit);
                }
            }
        }
    }

    eng->input.wid1 = w[1];
    eng->input.wid2 = w[0];
}

static int add_cache(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    int                     sos;
    int                     ret = 0;
    /* Strip SOS delimiters at the end of input string */

    sos = 0;
    while ( (len > 0) && is_sos_delimiter(eng, str[len-1]) )
    {
        sos = 1;
        len--;
    }

    if ( (len > 0) && (len <= MAX_WORD_LENGTH) )
    {
        LDB_WID wid;
        
        wid = search_dictionary(eng, str, len);
        if (wid == eng->wid_oov)
        {
            if (has_alphabet(eng, str, len))
            {
                udb_append(eng, eng->wid_oov, str, len);
                ret = 1;
            }
            else
                udb_append(eng, eng->wid_sos, 0, 0);
        }
        else
        {
            udb_append(eng, wid, 0, len);
            ret = 1;
        }
    }

    if (sos)
        udb_append(eng, eng->wid_sos, 0, 0);

    return ret;
}

static int init_word_match_input(
    IN ENGINE*                  eng,
    IN const KMX_INPUT_CONTEXT* context,
    IN const char*              string,
    IN LDB_WORD                 length,
    IN LDB_WORD                 flags)
{
    int i;

    if (!string)
        return KMX_FALSE;

    if (length == 0 || length > MAX_WORD_LENGTH)
        return KMX_FALSE;

    if (eng->is_sure_type)
    {
        for (i = 0; i < length; i++)
        {
            if (IS_VALID_SURE_TYPE_INPUT(string[i]))
                eng->input.string[i] = (LDB_CHAR) (string[i] - KMX_SURE_TYPE_FIRST_KEY);
            else
                return KMX_FALSE;
        }
    }
    else
    {
        for (i = 0; i < length; i++)
        {
            if (IS_VALID_9KEY_INPUT(string[i]))
                eng->input.string[i] = (LDB_CHAR) (string[i] - KMX_9KEY_FIRST_KEY);
            else if ((string[i] == KMX_9KEY_STAR_KEY) && (eng->flags & KMX_LDB_ATTR_USE_KEY_STAR))
                eng->input.string[i] = (LDB_CHAR) (KMX_LDB_KEY_STAR);
            else if ((string[i] == KMX_9KEY_POUND_KEY) && (eng->flags & KMX_LDB_ATTR_USE_KEY_POUND))
                eng->input.string[i] = (LDB_CHAR) (KMX_LDB_KEY_POUND);                
            else
                return KMX_FALSE;
        }
    }

    eng->input.length = length;
    eng->input.max_match_length = (LDB_WORD) eng->max_word_length;

    if (flags & KMX_LATIN_UPPER_CASE)
        eng->input.flags = KMX_LATIN_UPPER_CASE;
    else if (flags & KMX_LATIN_UPPER_CASE)
        eng->input.flags = KMX_LATIN_UPPER_CASE;
    else if (flags & KMX_LATIN_INITIAL_CASE)
        eng->input.flags = KMX_LATIN_INITIAL_CASE;
    else
        eng->input.flags = KMX_LATIN_LOWER_CASE;
    if (flags & KMX_LATIN_NUMBER_FIRST)
        eng->input.flags = (LDB_WORD)(eng->input.flags | KMX_LATIN_NUMBER_FIRST);

    parse_history(eng, context);

    eng->input.type = MATCH_TYPE_WORD;

    return KMX_TRUE;
}

static int init_asso_match_input(
    IN ENGINE*                  eng,
    IN const LDB_CHAR*          string,
    IN LDB_WORD                 length,
    IN LDB_WORD                 flags)
{
    int i;

    if (!string)
        return KMX_FALSE;

    if (length == 0 || length > MAX_WORD_LENGTH)
        return KMX_FALSE;

    for (i = 0; i < length; i++)
    {
        LDB_CHAR c;
        
        c = lower_case(eng, string[i]);
        if (is_alphabet(eng, c))
            eng->input.string[i] = c;
        else
            return KMX_FALSE;
    }

    eng->input.length = length;

    switch (length)
    {
        case 1:
            eng->input.max_match_length = 3;
            break;
        case 2:
            eng->input.max_match_length = 5;
            break;
        case 3:
            eng->input.max_match_length = 8;
            break;
        default:
            eng->input.max_match_length = (LDB_WORD) eng->max_word_length;
            break;
    }
    if (eng->input.max_match_length > eng->max_word_length)
        eng->input.max_match_length = (LDB_WORD) eng->max_word_length;

    if (flags & KMX_LATIN_UPPER_CASE)
        eng->input.flags = KMX_LATIN_UPPER_CASE;
    else if (flags & KMX_LATIN_UPPER_CASE)
        eng->input.flags = KMX_LATIN_UPPER_CASE;
    else if (flags & KMX_LATIN_INITIAL_CASE)
        eng->input.flags = KMX_LATIN_INITIAL_CASE;
    else
        eng->input.flags = KMX_LATIN_LOWER_CASE;

    eng->input.wid1 = eng->input.wid2 = eng->wid_sos;

    eng->input.type = MATCH_TYPE_ASSO;

    return KMX_TRUE;
}

static int is_identical_match_input(
    IN const MATCH_INPUT*   input1,
    IN const MATCH_INPUT*   input2)
{
    return input1->type == input2->type
        && input1->wid1 == input2->wid1
        && input1->wid2 == input2->wid2
        && input1->flags == input2->flags
        && input1->length == input2->length
        && memcmp(input1->string, input2->string, sizeof(LDB_CHAR) * input1->length) == 0;
}

static int is_cache_available(IN ENGINE* eng)
{
    return eng->cache_valid
        && is_identical_match_input(&eng->input, &eng->cache_input);
}

static void clear_cache(IN ENGINE* eng)
{
    eng->cache_valid = KMX_FALSE;
}

static void save_cache(IN ENGINE* eng)
{
    memcpy(&eng->cache_input, &eng->input, sizeof(MATCH_INPUT));
    eng->cache_valid = KMX_TRUE;
}

static void match_word(IN ENGINE* eng)
{
    /* Clear previous match result */

    eng->max_match = MAX_MATCH - 1;
    eng->match_count = 0;
    eng->tmp_len = 0;
    eng->is_exgram_found = KMX_FALSE;

    /* Always generate a number candidate */

    match_number(eng);

    /* Search candidates in cache */

    match_cache_word(eng);

    /* Search candidates in dictionary */

    match_dict_word(eng);

    /* Match prefix */

    eng->max_match = MAX_MATCH;

    match_prefix(eng);

    /* Match key string in legram */

    if (!eng->is_exgram_found)
        match_legram(eng);

    /* Sort match list by rank */

    qsort(eng->match_list, eng->match_count, sizeof(MATCH), cmp_match);
}

static void match_self_asso(
    IN ENGINE* eng,
    IN const LDB_CHAR* string,
    IN int length)
{
    LDB_CHAR* word_string;

    word_string = eng->buffer1;

    memcpy(word_string, string, sizeof(LDB_CHAR) * length);

    string_convert_case(eng, word_string, length, eng->input.flags);

    add_match(eng, RANK_TOP, word_string, length);
}

static void match_self_asso_ex(
    IN ENGINE* eng,
    IN const LDB_CHAR* string,
    IN int length)
{
    LDB_CHAR* word_string;
    
    word_string = eng->buffer1;
    
    memcpy(word_string, string, sizeof(LDB_CHAR) * length);

    if (is_valid_string(eng, word_string, length))
    {        
        string_convert_case(eng, word_string, length, eng->input.flags);

        add_match(eng, RANK_TOP, word_string, length);
    }
    
    if (length == 1)
    {
        int i;
        LDB_BYTE group_id;

        group_id = chr_to_group(eng, string[0]);

        for (i = 0; i < eng->alphabet_length; i++)
        {
            if (eng->group_id_table[i] == group_id)
            {                                
                if (string[0] == eng->alphabet[i])
                {
                    continue;
                }

                word_string[0] = eng->alphabet[i];
                
                if (is_valid_string(eng, word_string, 1))
                {
                    LDB_WID wid;
                    int rank_value;

                    wid = search_dictionary(eng, word_string, 1);
                    rank_value = get_word_level(eng, wid);

                    string_convert_case(eng, word_string, 1, eng->input.flags);

                    add_match(eng, RANK_TOP + rank_value, word_string, length);
                }
            }
        }
    }
    else
    {
        int i, j, k;
        LDB_BYTE                g0;
        LDB_BYTE                g1;
        LDB_BYTE                g0_keys[MAX_ASSO_KEY_COUNT];
        LDB_BYTE                g1_keys[MAX_ASSO_KEY_COUNT];

        g0 = chr_to_group(eng, string[0]);
        fill_group_keys(eng, g0, g0_keys);

        g1 = chr_to_group(eng, string[1]);
        fill_group_keys(eng, g1, g1_keys);

        for (j = 0; j < MAX_ASSO_KEY_COUNT; j++)
        {
            if (!g0_keys[j])
                continue;

            for (k = 0; k < MAX_ASSO_KEY_COUNT; k++)
            {
                if (!g1_keys[k])
                    continue;
                
                i = eng->key_count + (length - 2) * eng->key_count * eng->key_count + j * eng->key_count + k;
                match_dict_asso_by_index_ex_at_top_rank(eng, i, length);
                if (is_match_list_full(eng))
                    return;

            }                    
        }        
    }
}

static void match_asso(
    IN ENGINE* eng,
    IN const LDB_CHAR* string,
    IN int length)
{
    /* Clear previous match result */

    eng->max_match = MAX_MATCH;
    eng->match_count = 0;

    /* Add input string as first candidate */

    match_self_asso(eng, string, length);

    /* Search candidates in cache */

    match_cache_asso(eng);

    /* Search candidates in dictionary */

    match_dict_asso(eng);

    /* Sort match list by rank */

    qsort(eng->match_list, eng->match_count, sizeof(MATCH), cmp_match);
}

static void match_asso_ex(
                       IN ENGINE* eng,
                       IN const LDB_CHAR* string,
                       IN int length)
{
    /* Clear previous match result */

    eng->max_match = MAX_MATCH;
    eng->match_count = 0;

    /* Add input string as first candidate */

    match_self_asso_ex(eng, string, length);

    /* Search candidates in cache */

    match_cache_asso_ex(eng);

    /* Search candidates in dictionary */

    match_dict_asso_ex(eng);

    /* Sort match list by rank */

    qsort(eng->match_list, eng->match_count, sizeof(MATCH), cmp_match);
}

static int generate_match_result(
    IN ENGINE*                  eng,
    IN unsigned short           match_index,
    OUT KMX_LATIN_MATCH_RESULT* match_result)
{
    if (eng->match_count > 0 && match_index < eng->match_count)
    {
        if (match_result)
        {
            int i;

            if (eng->match_count - match_index <= KMX_MAX_WORD_MATCH)
            {
                match_result->count = (LDB_WORD) (eng->match_count - match_index);
                match_result->more_available = KMX_FALSE;
            }
            else
            {
                match_result->count = KMX_MAX_WORD_MATCH;
                match_result->more_available = KMX_TRUE;
            }

            for (i = 0; i < match_result->count; i++)
            {
                match_result->table[i].length = (LDB_WORD) (eng->match_list[match_index + i].length);
                memcpy(match_result->table[i].string,
                       eng->match_list[match_index + i].string,
                       sizeof(LDB_CHAR) * eng->match_list[match_index + i].length);
            }
        }
        return KMX_TRUE;
    }

    return KMX_FALSE;
}

static void bind_chr_to_group(   
    IN ENGINE*                  eng,
    IN int                      group_id,
    IN LDB_CHAR                 chr)
{
    int i;   

    i = sorted_strchr(eng->alphabet, eng->alphabet_length, chr);
    KMX_ASSERT(i >= 0);    
    
    eng->group_id_table[i] = (LDB_BYTE)group_id;
}

static int initialize_letter_group(
   IN ENGINE*                  eng,
   IN const unsigned short*    letter_group)
{
    const unsigned short* p = letter_group;
    int cur_group_id = 1;
        
	if (eng->alphabet_length > MAX_CHAR_CODE_COUNT)
    {
        return KMX_FALSE;
    }

    /* Bind char to group */    
    while (!((*p == 0) && (*(p + 1) == 0)))
    {        
        while (*p != 0)        
        {
            if (sorted_strchr(eng->alphabet, eng->alphabet_length, *p) < 0)
                return KMX_FALSE;

            bind_chr_to_group(eng, cur_group_id, *p);
            p++;
        }
                
        cur_group_id++;
        p++;
        if (*p == 0)
            break;
    }       
    
    cur_group_id--;

    if (cur_group_id <= 0)
        return KMX_FALSE;

    eng->group_count = cur_group_id;
    return KMX_TRUE;
}

static int is_ldb_use_key_star(IN const LDB* ldb)
{    
    int i, end;

    if (IS_SURE_TYPE(ldb->did))
        return 0;

    if(!ldb->ckey_table)
        return 0;

    end = ldb->char_code_count;

    for(i = 0; i < end; i++)
    {
        if (ldb->ckey_table[i] > 9)
            return 1;
    }

    return 0;
}

static int is_ldb_use_key_pound(IN const LDB* ldb)
{    
    #if defined(KMX_LDB_BENGALI)
    if (ldb->did == 9200)
        return 1;
    #endif
    
    return 0;
}

/****************************************************************************
 *
 * Implementation of global functions.
 *
 ***************************************************************************/
int kmx_latin_initialize_ex(
    IN KMX_IME_ENGINE*              engine,
    IN const KMX_IME_ENGINE_INIT*   init_info)
{
    ENGINE* eng;
    const LDB* ldb;

    #ifdef KMX_DEBUG
        printf("Latin engine size = %d bytes\n", sizeof(ENGINE));
    #endif

    if (!engine)
        return KMX_FALSE;

    if (!init_info)
        return KMX_FALSE;

    /* Check database */

    ldb = (const LDB*) (init_info->database);

    if (!is_valid_database(ldb))
        return KMX_FALSE;

    /* Initialize engine object */

    eng = (ENGINE*)engine->engine;

    memset(eng, 0, sizeof(ENGINE));

    eng->signature               = ENGINE_SIGNATURE;

    eng->did                     = ldb->did;
    eng->major_version           = ldb->major_version;
    eng->minor_version           = ldb->minor_version;
    eng->build_number            = ldb->build_number;

    eng->alphabet                = ldb->alphabet;
    eng->alphabet_length         = ldb->alphabet_length;

    eng->lc2ucm                  = ldb->lc2ucm;
    eng->lc2ucm_length           = ldb->lc2ucm_length;

    eng->uc2lcm                  = ldb->uc2lcm;
    eng->uc2lcm_length           = ldb->uc2lcm_length;

    eng->sos_table               = ldb->sos_table;
    eng->sos_table_length        = ldb->sos_table_length;

    eng->sou_table               = ldb->sou_table;
    eng->sou_table_length        = ldb->sou_table_length;

    eng->mt_index                = ldb->mt_index;
    eng->mt_string               = ldb->mt_string;
    eng->mt_string_length        = ldb->mt_string_length;

    eng->legram_index            = ldb->legram_index;
    eng->legram_string           = ldb->legram_string;
    eng->legram_string_length    = ldb->legram_string_length;

    eng->word_count              = ldb->word_count;
    eng->word_id_bits            = ldb->word_id_bits;
    eng->wid_oov                 = ldb->wid_oov;
    eng->wid_sos                 = ldb->wid_sos;

    eng->word_index              = ldb->word_index;
    eng->max_word_length         = ldb->max_word_length;

    eng->word_table              = ldb->word_table;
    eng->word_level_bits         = ldb->word_level_bits;
    eng->word_index_bits         = ldb->word_index_bits;

    eng->char_code_count         = ldb->char_code_count;
    eng->char_code_bits          = ldb->char_code_bits;

    eng->char_table              = ldb->char_table;
    eng->ckey_table              = ldb->ckey_table;

    eng->string_table            = ldb->string_table;
    eng->string_table_length     = ldb->string_table_length;

    eng->bigram_index            = ldb->bigram_index;
    eng->bigram_index_count      = ldb->bigram_index_count;
    eng->bigram_index_bits       = ldb->bigram_index_bits;
    eng->bigram                  = ldb->bigram;
    eng->bigram_count            = ldb->bigram_count;

    eng->trigram                 = ldb->trigram;
    eng->trigram_count           = ldb->trigram_count;

    eng->exgram_index            = ldb->exgram_index;
    eng->exgram_index_count      = ldb->exgram_index_count;
    eng->exgram_index_bits       = ldb->exgram_index_bits;
    eng->exgram                  = ldb->exgram;

    if (IS_SURE_TYPE(ldb->did))
    {
        eng->is_sure_type = 1;
        eng->language = ldb->did / 1000;
        eng->key_count = 16;
    }
    else
    {
        eng->is_sure_type = 0;
        if (IS_LATIN_EXTEND_DID(ldb->did))
            eng->language = LATIN_EXTEND_DID_TO_LANG(ldb->did);
        else
            eng->language = ldb->did / 10;
        eng->key_count = 10;
        
        if (is_ldb_use_key_star(ldb))
        {
            eng->key_count += 1;
            eng->flags |= KMX_LDB_ATTR_USE_KEY_STAR;
        }

        if (is_ldb_use_key_pound(ldb))
        {
            KMX_ASSERT(is_ldb_use_key_star(ldb));
            eng->key_count += 1;
            eng->flags |= KMX_LDB_ATTR_USE_KEY_POUND;
        }
        
    }

    eng->bc_wid   = -1;
    eng->bc_start = 0;
    eng->bc_end   = -1;

    eng->tc_wid1  = -1;
    eng->tc_wid2  = -1;
    eng->tc_start = 0;
    eng->tc_end   = -1;

    eng->ec_wid   = -1;
    eng->ec_start = 0;
    eng->ec_end   = -1;

    /* Initialize user data */

    if ( (init_info->user_data != 0) && (init_info->user_data_size >= UDB_MIN) )
        udb_open(eng, init_info->user_data, init_info->user_data_size);

    /* Initialize letter group, for latin query association ex */
    /* Not support suretype now */
    if ( (init_info->letter_group != 0) && !eng->is_sure_type)
    {
        if (!initialize_letter_group(eng, init_info->letter_group))
            return KMX_FALSE;
    }
	return KMX_TRUE;
}

int kmx_latin_initialize(
    IN KMX_IME_ENGINE*              engine,
    IN const KMX_IME_ENGINE_INIT*   init_info)
{
#ifdef SELFALLOC_PRODUCT
    IIME_MRESET(IIME_NULL);
#endif

    engine->engine = IIME_MALLOC(16 * 1024);

	return kmx_latin_initialize_ex(engine, init_info);
}

int kmx_latin_match(
    IN KMX_IME_ENGINE*          engine,
    IN const KMX_INPUT_CONTEXT* context,
    IN const char*              input_string,
    IN unsigned short           input_string_length,
    IN unsigned short           match_flags,
    IN unsigned short           match_index,
    OUT KMX_LATIN_MATCH_RESULT* match_result)
{
    ENGINE* eng;

    if (!engine)
        return KMX_FALSE;

    eng = (ENGINE*)engine->engine;
    if (eng->signature != ENGINE_SIGNATURE)
        return KMX_FALSE;

    if (!init_word_match_input(eng, context, input_string, input_string_length, match_flags))
        return KMX_FALSE;

    if (!is_cache_available(eng))
    {
        match_word(eng);
        save_cache(eng);
    }

    return generate_match_result(eng, match_index, match_result);
}

int kmx_latin_match_lenovo_qualcomm(
    IN KMX_IME_ENGINE*          engine,
    IN const KMX_INPUT_CONTEXT* context,
    IN const char*              input_string,
    IN unsigned short           input_string_length,
    IN unsigned short           match_flags,
    IN unsigned short           match_index,
    OUT KMX_LATIN_MATCH_RESULT* match_result)
{
    ENGINE* eng;

    if (!engine)
        return KMX_FALSE;

    eng = (ENGINE*)engine->engine;
    if (eng->signature != ENGINE_SIGNATURE)
        return KMX_FALSE;

    if (!init_word_match_input(eng, context, input_string, input_string_length, match_flags))
        return KMX_FALSE;

    if (!is_cache_available(eng))
    {
        int iTail = 0;
        match_word(eng);

        if (eng->match_count < MAX_MATCH)
        {
            iTail = eng->match_count;
            eng->match_count++;
        }
        else
            iTail = MAX_MATCH - 1;

        eng->match_list[iTail].length = 1;
        eng->match_list[iTail].rank = 0;
        eng->match_list[iTail].string[0] = 0xFFFF;
        eng->match_list[iTail].string[1] = 0;

        save_cache(eng);
    }

    return generate_match_result(eng, match_index, match_result);
}

int kmx_latin_query_association(
    KMX_IME_ENGINE*             engine,
    const unsigned short*       input_string,
    unsigned short              input_string_length,
    unsigned short              match_flags,
    unsigned short              match_index,
    KMX_LATIN_MATCH_RESULT*     match_result)
{
    ENGINE* eng;

    if (!engine)
        return KMX_FALSE;

    eng = (ENGINE*)engine->engine;
    if (eng->signature != ENGINE_SIGNATURE)
        return KMX_FALSE;

    if (!init_asso_match_input(eng, input_string, input_string_length, match_flags))
        return KMX_FALSE;

    if (!is_cache_available(eng))
    {
        match_asso(eng, input_string, input_string_length);
        save_cache(eng);
    }

    return generate_match_result(eng, match_index, match_result);
}

int kmx_latin_query_association_ex(
    KMX_IME_ENGINE*             engine,
    const unsigned short*       input_string,
    unsigned short              input_string_length,
    unsigned short              match_flags,
    unsigned short              match_index,
    KMX_LATIN_MATCH_RESULT*     match_result)
{
    ENGINE* eng;

    if (!engine)
        return KMX_FALSE;

    eng = (ENGINE*)engine->engine;
    if (eng->signature != ENGINE_SIGNATURE)
        return KMX_FALSE;

    if (eng->group_count == 0)
        return kmx_latin_query_association(engine, input_string, input_string_length, 
                                           match_flags, match_index, match_result);

    if (!init_asso_match_input(eng, input_string, input_string_length, match_flags))
        return KMX_FALSE;

    if (!is_cache_available(eng))
    {
        match_asso_ex(eng, input_string, input_string_length);
        save_cache(eng);
    }

    return generate_match_result(eng, match_index, match_result);
}

int kmx_latin_update_cache(
    IN KMX_IME_ENGINE*          engine,
    IN const unsigned short*    string,
    IN unsigned short           length)
{
    ENGINE* eng;
    int ret = KMX_TRUE;

    if (!engine)
        return KMX_FALSE;

    eng = (ENGINE*)engine->engine;
    if (eng->signature != ENGINE_SIGNATURE)
        return KMX_FALSE;

    if (!eng->udb)
        return KMX_FALSE;

    udb_append(eng, eng->wid_sos, 0, 0);

    if (string != 0 && length > 0)
    {
        const LDB_CHAR* scan;
        const LDB_CHAR* end;
        const LDB_CHAR* word;
        int cc;

        /* Scan history from beginning to the end */

        scan = string;
        end = scan + length;

        while (scan < end)
        {
            /* Find a word delimited by space */

            while ( (scan < end) && (*scan == KMX_CHAR_SPACE) )
                scan++;

            cc = 0;
            word = scan;
            while ( (scan < end) && (*scan != KMX_CHAR_SPACE) )
            {
                scan++;
                cc++;
            }

            /*  Append the word to cache */

            if(!add_cache(eng, word, cc))
                ret = KMX_FALSE;
        }
    }

    udb_append(eng, eng->wid_sos, 0, 0);

    clear_cache(eng);

    return ret;
}

static int kmx_is_indic_dummy_did(
    int                          did)
{
        int language;
        if (IS_LATIN_EXTEND_DID(did))
            language = LATIN_EXTEND_DID_TO_LANG(did);
        else
            language = did / 10;

        if (language == KMX_LANG_TELUGU || language == KMX_LANG_MARATHI || language == KMX_LANG_TAMIL
            || language == KMX_LANG_PUNJABI || language == KMX_LANG_GUJARATI || language == KMX_LANG_ORIYA
            || language == KMX_LANG_KANNADA || language == KMX_LANG_MALAYALAM || language == KMX_LANG_KHMER
            || language == KMX_LANG_LAO)
            return 1;

       return 0;
}

int kmx_latin_query_key_map(
    IN const void*              database,
    IN unsigned short           char_code,
    OUT unsigned short*         key_code)
{
    const LDB* ldb;
    int i;
    int is_sure_type;

    if (!key_code)
        return KMX_FALSE;

    /* Check database */

    ldb = (const LDB*) database;

    if (kmx_is_indic_dummy_did(ldb->did))
    {
        INDIC_DUMMY_DB* indic_ldb;
        int language;

        indic_ldb = (INDIC_DUMMY_DB*) database;
        
        language = ldb->did / 10;
        if (language == KMX_LANG_KHMER)
        {
            return KMX_FALSE;
        }
        else if (language == KMX_LANG_LAO)
        {
            return KMX_FALSE;
        }
        else
        {
            int i;
            for (i = 0; i < 10; i ++)
            {
                const LDB_CHAR* mt_entry = indic_ldb->multitap_string_2[i];
                while(*mt_entry)
                {
                    if (*mt_entry == char_code)
                    {
                        *key_code = (unsigned short)i;
                        return KMX_TRUE;
                    }
                    mt_entry++;
                }
            }
        }
        return KMX_FALSE;
    }
        
    if (!is_valid_database(ldb))
        return KMX_FALSE;

    is_sure_type = IS_SURE_TYPE(ldb->did);

    if (is_sure_type)
        *key_code = 15;
    else
        *key_code = 1;

    /* Check various numbers */
    if (char_code == '*' && is_ldb_use_key_star(ldb))
    {
        *key_code = KMX_LDB_KEY_STAR;
        return KMX_TRUE;
    }

    if (char_code == '#' && is_ldb_use_key_pound(ldb))
    {
        *key_code = KMX_LDB_KEY_POUND;
        return KMX_TRUE;
    }
        
    if (char_code >= KMX_CHAR_DIGIT_0 && char_code <= KMX_CHAR_DIGIT_9)
    {
        *key_code = (unsigned short) (char_code - KMX_CHAR_DIGIT_0);
        return KMX_TRUE;
    }

    if (char_code >= KMX_CHAR_ARABIC_DIGIT_0 && (char_code <= KMX_CHAR_ARABIC_DIGIT_9) )
    {
        *key_code = (unsigned short) (char_code - KMX_CHAR_ARABIC_DIGIT_0);
        return KMX_TRUE;
    }

    if (char_code >= KMX_CHAR_PERSIAN_DIGIT_0 && char_code <= KMX_CHAR_PERSIAN_DIGIT_9)
    {
        *key_code = (unsigned short) (char_code - KMX_CHAR_PERSIAN_DIGIT_0);
        return KMX_TRUE;
    }

    if (char_code >= KMX_CHAR_HINDI_DIGIT_0 && char_code <= KMX_CHAR_HINDI_DIGIT_9)
    {
        *key_code = (unsigned short) (char_code - KMX_CHAR_HINDI_DIGIT_0);
        return KMX_TRUE;
    }

	if (char_code >= KMX_CHAR_BENGALI_DIGIT_0 && char_code <= KMX_CHAR_BENGALI_DIGIT_9)
    {
        *key_code = (unsigned short) (char_code - KMX_CHAR_BENGALI_DIGIT_0);
        return KMX_TRUE;
    }

    if (char_code >= KMX_CHAR_THAI_DIGIT_0 && char_code <= KMX_CHAR_THAI_DIGIT_9)
    {
        *key_code = (unsigned short) (char_code - KMX_CHAR_THAI_DIGIT_0);
        return KMX_TRUE;
    }

    /* Convert character into lowercase */

    i = sorted_strchr(ldb->uc2lcm, ldb->uc2lcm_length, char_code);
    if (i >= 0)
    {
        char_code = (unsigned short) (ldb->uc2lcm[ldb->uc2lcm_length + i]);
    }
    else if ( (char_code >= 0x41 && char_code <= 0x5A) ||
              (char_code >= 0xC0 && char_code <= 0xD6) ||
              (char_code >= 0xD8 && char_code <= 0xDE) )
    {
        char_code = (unsigned short) (char_code + 0x20);
    }

    i = sorted_strchr(ldb->char_table, ldb->char_code_count, char_code);
    if (i >= 0)
    {
        *key_code = (unsigned short) (ldb->ckey_table[i]);
        return KMX_TRUE;
    }

    if (char_code >= 0x61 && char_code <= 0x7A)
    {
        if (is_sure_type)
            *key_code = (unsigned short) (BASIC_LATIN_SURE_TYPE_MAP[char_code - 0x61]);
        else
            *key_code = (unsigned short) (BASIC_LATIN_9KEY_MAP[char_code - 0x61]);
        return KMX_TRUE;
    }

    return KMX_TRUE;
}

int kmx_latin_query_multitap(
    const void*                     database,
    unsigned short                  key,
    unsigned short                  flags,
    const unsigned short**          string,
    unsigned short*                 length)
{
    const LDB* ldb;
    int n;
    int is_sure_type;
    unsigned short key_count;
    /* Check input parameters */

    if (!string)
        return KMX_FALSE;

    if (!length)
        return KMX_FALSE;

    ldb = (const LDB*) database;

    if (ldb->did == 9201)
    {
        INDIC_DUMMY_DB* indic_ldb;

        if (key > 11)
            return KMX_FALSE;

        indic_ldb = (INDIC_DUMMY_DB*) database;
        *string = indic_ldb->multitap_string_1[key];
        *length = (unsigned short)IIMEString_Length(*string);

        return KMX_TRUE;
    }

    if (kmx_is_indic_dummy_did(ldb->did))
    {
        INDIC_DUMMY_DB* indic_ldb;
        int language;

        /* Indic dummy ldb */
        if (key > 9)
            return KMX_FALSE;

        indic_ldb = (INDIC_DUMMY_DB*) database;
        
        language = ldb->did / 10;
        if (language == KMX_LANG_KHMER)
        {
            return KMX_FALSE;
        }
        else if (language == KMX_LANG_LAO)
        {
            return KMX_FALSE;
        }
        else
        {
            *string = indic_ldb->multitap_string_2[key];
            *length = (unsigned short)IIMEString_Length(*string);
        }
        return KMX_TRUE;
    }
    
    /* Normal ldb */
    if (!is_valid_database(ldb))
        return KMX_FALSE;

    is_sure_type = IS_SURE_TYPE(ldb->did);

    if (is_sure_type)
    {
        key_count = 16;
        if (key > 15)
            return KMX_FALSE;
    }
    else
    {
        key_count = 10;
        if (is_ldb_use_key_star(ldb))
        {
            key_count += 1;
        }

        if (is_ldb_use_key_pound(ldb))
        {
            key_count += 1;
        }
        
        if (key > (key_count - 1))
            return KMX_FALSE;
    }

    if (flags & KMX_LATIN_LOWER_CASE)
    {
    }
    else if (flags & KMX_LATIN_UPPER_CASE)
    {        
        key = (unsigned short) (key + key_count);        
    }
    else
    {
        return KMX_FALSE;
    }

    n = ldb->mt_index[key+1] - ldb->mt_index[key];
    if (n <= 0)
        return KMX_FALSE;

    *string = ldb->mt_string + ldb->mt_index[key];
    *length = (unsigned short)n;

    return KMX_TRUE;
}

/* justfy alphabeta and symbol */
int kmx_latin_is_chartable(
    IN KMX_IME_ENGINE*      engine,
    IN const LDB_CHAR		chr)
{
	ENGINE *eng;
	LDB_CHAR c;
	if (!engine)
		return 0;

    eng = (ENGINE*)engine->engine;

	c = lower_case(eng, chr);

	if (sorted_strchr(eng->char_table, eng->char_code_count, c) >= 0)
		return 1;
	else
		return 0;
}

/* justify alphabeat */
int kmx_latin_is_alphabeta(
    IN KMX_IME_ENGINE*      engine,
    IN const LDB_CHAR		chr)
{
	ENGINE *eng;
	LDB_CHAR c;

	if (!engine)
		return 0;

    eng = (ENGINE*)engine->engine;

	c = lower_case(eng, chr);

	if (sorted_strchr(eng->alphabet, eng->alphabet_length, c) >= 0)
		return 1;
	else
		return 0;
}

void kmx_latin_lower_case(
    IN KMX_IME_ENGINE*      engine,
    IN unsigned short*		str,
	IN unsigned short		len)
{
	ENGINE *eng;
	int i = 0;
	if (!engine)
		return;

    eng = (ENGINE*)engine->engine;
	for (i = 0; i < len; i++)
		str[i] = lower_case(eng, str[i]);
}

void kmx_latin_upper_case(
    IN KMX_IME_ENGINE*      engine,
    IN unsigned short*		str,
	IN unsigned short		len)
{
	ENGINE *eng;
	int i;
	if (!engine)
		return;

    eng = (ENGINE*)engine->engine;
	for (i = 0; i < len; i++)
		str[i] = upper_case(eng, str[i]);
}

void kmx_latin_initial_case(
    IN KMX_IME_ENGINE*      engine,
    IN unsigned short*		str,
	IN unsigned short		len)
{
	int i;
	ENGINE *eng;
	if (!engine)
		return;

    eng = (ENGINE*)engine->engine;
	for (i = 0; i < len; i++)
	{
		if (i == 0)
			str[i] = upper_case(eng, str[i]);
		else
			str[i] = lower_case(eng, str[i]);
	}
}

int kmx_latin_to_lower_case(
    IN const void*          database,
    IN unsigned short*		str,
	IN unsigned short		len)
{
    const LDB* ldb;
    int i;    

    if (!str)
        return KMX_FALSE;

    /* Check database */

    ldb = (const LDB*) database;
    if (!is_valid_database(ldb))
        return KMX_FALSE;
		
	for (i = 0; i < len; i++)
		str[i] = to_lower_case(ldb, str[i]);

    return KMX_TRUE;
}

int kmx_latin_to_upper_case(
    IN const void*          database,
    IN unsigned short*		str,
	IN unsigned short		len)
{
    const LDB* ldb;
    int i;    

    if (!str)
        return KMX_FALSE;

    /* Check database */

    ldb = (const LDB*) database;
    if (!is_valid_database(ldb))
        return KMX_FALSE;
		
	for (i = 0; i < len; i++)
		str[i] = to_upper_case(ldb, str[i]);

    return KMX_TRUE;	
}

int kmx_latin_to_initial_case(
    IN const void*          database,
    IN unsigned short*		str,
	IN unsigned short		len)
{
    const LDB* ldb;
    int i;    

    if (!str)
        return KMX_FALSE;

    /* Check database */

    ldb = (const LDB*) database;
    if (!is_valid_database(ldb))
        return KMX_FALSE;
		
	for (i = 0; i < len; i++)
    {
    	if (i == 0)
			str[i] = to_upper_case(ldb, str[i]);
		else
			str[i] = to_lower_case(ldb, str[i]);
	
    }

    return KMX_TRUE;
}

int kmx_is_sou_delimiter(
    IN KMX_IME_ENGINE*      engine,
    IN LDB_CHAR             c)
{
    ENGINE*                 eng;
    if (!engine)
        return 0;

    eng = (ENGINE*)engine->engine;

    return is_sou_delimiter(eng, c);
}


int kmx_get_db_info(
    IN const void*          database,
    OUT LDB_INFO*           info)
{
    const LDB*              ldb;

    if (!database)
        return 0;

    if (!info)
        return 0;

    ldb = (const LDB*) database;

    info->database_id = ldb->did;

    if (IS_SURE_TYPE(ldb->did))
    {
        info->is_sure_type = 1;
        info->language = ldb->did / 1000;
    }
    else
    {
        info->is_sure_type = 0;
        if (IS_LATIN_EXTEND_DID(ldb->did))
            info->language = LATIN_EXTEND_DID_TO_LANG(ldb->did);
        else
            info->language = ldb->did / 10;
    }

    info->major_version = ldb->major_version;
    info->minor_version = ldb->minor_version;
    info->build_number = ldb->build_number;

    info->copyright = ldb->copyright;
    info->copyright_length = ldb->copyright_length;

    info->engine_object_size = sizeof(ENGINE);

    info->max_input_length = MAX_WORD_LENGTH;

    info->eudw_prompt = ldb->eudw_prompt;
    info->eudw_prompt_length = ldb->eudw_prompt_length;

    info->eudw_title = ldb->eudw_title;
    info->eudw_title_length = ldb->eudw_title_length;

    return 1;
}

/****************************************************************************
 *
 * Some compile-time assertion to ensure our various assumptions are correct.
 *
 ***************************************************************************/

#define KMX_STATIC_ASSERT(exp)                  \
      do {                                      \
         enum { kmx_static_assert = 1 / (exp) };    \
      } while (0)

static void compile_time_assertions(void)
{
    KMX_STATIC_ASSERT(sizeof(char)  == 1);
    KMX_STATIC_ASSERT(sizeof(short) == 2);
    KMX_STATIC_ASSERT(sizeof(int)   == 4);
    KMX_STATIC_ASSERT(sizeof(long)  == 4);
    KMX_STATIC_ASSERT(sizeof(void*) == 4);

    KMX_STATIC_ASSERT(sizeof(LDB_BYTE) == 1);
    KMX_STATIC_ASSERT(sizeof(LDB_WORD) == 2);
    KMX_STATIC_ASSERT(sizeof(LDB_CHAR) == 2);

    KMX_STATIC_ASSERT(KMX_TRUE  == 1);
    KMX_STATIC_ASSERT(KMX_FALSE == 0);

    /*
     * Our engine object size must be less than or equal to the
     * global engine object size.
     */

/*    KMX_STATIC_ASSERT(sizeof(ENGINE) <= sizeof(KMX_IME_ENGINE));*/

    /*
     * "MAX_WORD_LENGTH" must be an even number
     * for proper alignment.
     */
    KMX_STATIC_ASSERT(((MAX_WORD_LENGTH) % 2) == 0);
}

/* Hangul multitap query interface */
#ifdef KMX_LDB_HANGUL

/* Syllable macros */

#define SBASE   0xAC00
#define SEND    0xD7A3
#define IS_SYLLABLE(c) ((c) >= SBASE && (c) <= SEND)

#define LBASE   0x1100
#define LEND    0x1112
#define IS_LC(c) ((c) >= LBASE && (c) <= LEND)

#define VBASE   0x1161
#define VEND    0x1175
#define IS_VO(c) ((c) >= VBASE && (c) <= VEND)

#define TBASE   0x11A7
#define TEND    0x11C2
#define IS_TC(c) ((c) >= TBASE && (c) <= TEND)

#define IS_HANGUL_CHAR(c) (IS_SYLLABLE(c) || IS_LC(c) || IS_VO(c) || IS_TC(c))

#define LCOUNT 19
#define VCOUNT 21
#define TCOUNT 28

#define NCOUNT (VCOUNT * TCOUNT)

#define S_HAS_T(s) (((s) - SBASE) % TCOUNT)
#define S_FROM_LVT(l,v,t)   (SBASE + (((l) - LBASE) * VCOUNT + ((v) - VBASE)) * TCOUNT + ((t) - TBASE))
#define S_FROM_LV(l,v)      (SBASE + (((l) - LBASE) * VCOUNT + ((v) - VBASE)) * TCOUNT)
#define L_FROM_S(s)     (LBASE + (((s) - SBASE) / NCOUNT))
#define V_FROM_S(s)     (VBASE + (((s) - SBASE) % NCOUNT) / TCOUNT)
#define T_FROM_S(s)     (TBASE + (((s) - SBASE) % TCOUNT))


#define MAX_HANGUL_ENTRY_LENGTH 4
static const unsigned short default_multitap_hangul_base[10][MAX_HANGUL_ENTRY_LENGTH] = 
{
/* key 0 */  {  0x0020, 0x0030, 0x00 },
/* key 1 */  {  0x1100, 0x110F, 0x00 },
/* key 2 */  {  0x1109, 0x110C, 0x110E, 0x00 },
/* key 3 */  {  0x1161, 0x1165, 0x00 },
/* key 4 */  {  0x1102, 0x1103, 0x1110, 0x00 },
/* key 5 */  {  0x110B, 0x1112, 0x00 },
/* key 6 */  {  0x1175, 0x1173, 0x1174, 0x00 },
/* key 7 */  {  0x1107, 0x1111, 0x00 },
/* key 8 */  {  0x1105, 0x1106, 0x11B1, 0x00 },
/* key 9 */  {  0x1169, 0x116E, 0x00 },
};

/* (c1, c2) -> c */
typedef struct  
{
    unsigned short c1;
    unsigned short c2;
    unsigned short c;
}MAP_ENTRY;

static const MAP_ENTRY lc_compose_table[] =
{
    { 0, 0, 0 }
};

static const MAP_ENTRY vo_compose_table[] =
{
    { 0x1169, 0x1161, 0x116A },
    { 0x116F, 0x1175, 0x1170 },
    { 0x1165, 0x1175, 0x1166 },
    { 0x1169, 0x1175, 0x116C },
    { 0x1161, 0x1175, 0x1162 },
    { 0x116E, 0x1175, 0x1171 },
    { 0x116A, 0x1175, 0x116B },
    { 0x116E, 0x1165, 0x116F },
    { 0, 0, 0 }
};

static const MAP_ENTRY tc_compose_table[] =
{    
    { 0x11AF, 0x1109, 0x11B3 },
    { 0x1103, 0x0000, 0x11AE },
    { 0x110E, 0x0000, 0x11BE },
    { 0x11AF, 0x1112, 0x11B6 },
    { 0x11AB, 0x110C, 0x11AC },
    { 0x110F, 0x0000, 0x11BF },
    { 0x11AF, 0x1100, 0x11B0 },
    { 0x1100, 0x0000, 0x11A8 },
    { 0x1106, 0x0000, 0x11B7 },
    { 0x1110, 0x0000, 0x11C0 },
    { 0x11AF, 0x1111, 0x11B5 },
    { 0x11AB, 0x1112, 0x11AD },
    { 0x110B, 0x0000, 0x11BC },
    { 0x11AF, 0x1107, 0x11B2 },
    { 0x110C, 0x0000, 0x11BD },
    { 0x1112, 0x0000, 0x11C2 },
    { 0x11B8, 0x1109, 0x11B9 },
    { 0x1105, 0x0000, 0x11AF },
    { 0x1102, 0x0000, 0x11AB },
    { 0x11AF, 0x1110, 0x11B4 },
    { 0x1111, 0x0000, 0x11C1 },
    { 0x1109, 0x0000, 0x11BA },
    { 0x11A8, 0x1109, 0x11AA },
    { 0x1107, 0x0000, 0x11B8 },
    { 0x11AF, 0x1106, 0x11B1 },
    { 0, 0, 0 }
};

static const MAP_ENTRY char_transform_table[] =
{
    { 0x1169, 0x002A, 0x116D },
    { 0x1162, 0x002A, 0x1164 },
    { 0x1166, 0x002A, 0x1168 },
    { 0x1107, 0x002A, 0x1108 },
    { 0x1161, 0x002A, 0x1163 },
    { 0x116E, 0x002A, 0x1172 },
    { 0x11A8, 0x002A, 0x11A9 },
    { 0x110C, 0x002A, 0x110D },
    { 0x1103, 0x002A, 0x1104 },
    { 0x1165, 0x002A, 0x1167 },
    { 0x1109, 0x002A, 0x110A },
    { 0x1100, 0x002A, 0x1101 },
    { 0x11BA, 0x002A, 0x11BB },
    { 0x1175, 0x002A, 0x1174 },
    /* Reverse convert */
    { 0x116D, 0x002A, 0x1169 },
    { 0x1164, 0x002A, 0x1162 },
    { 0x1168, 0x002A, 0x1166 },
    { 0x1108, 0x002A, 0x1107 },
    { 0x1163, 0x002A, 0x1161 },
    { 0x1172, 0x002A, 0x116E },
    { 0x11A9, 0x002A, 0x11A8 },
    { 0x110D, 0x002A, 0x110C },
    { 0x1104, 0x002A, 0x1103 },
    { 0x1167, 0x002A, 0x1165 },
    { 0x110A, 0x002A, 0x1109 },
    { 0x1101, 0x002A, 0x1100 },
    { 0x11BB, 0x002A, 0x11BA },
    { 0x1174, 0x002A, 0x1175 },
    /*{ 0x11AF, 0x002A, 0x11B1 }, show when multitap key 8 at T position. */
    { 0, 0, 0 }
};

static unsigned short find_mapping(const MAP_ENTRY * map_table, unsigned short c1, unsigned short c2)
{
    const MAP_ENTRY * e = map_table;

    while (e->c != 0)
    {
        if (e->c1 == c1 && e->c2 == c2)
        {
            break;
        }
        e++;    
    }

    return e->c;
}

static unsigned short decompose_tc(unsigned short c, unsigned short * lead, unsigned short * tail)
{
    const MAP_ENTRY * e = tc_compose_table;
    
    *lead = *tail = 0;

    /* special case for double simple con which can be map to lead con directly */
    if (c == 0x11A9)
    {
        *tail = 0x1101;
        return 1;
    }

    if (c == 0x11BB)
    {
        *tail = 0x110A;
        return 1;
    }

    while (e->c != 0)
    {
        if (e->c == c)
        {
            if (e->c2 == 0)
            {
                /* e->c1 is the leading con map of c*/
                *lead = e->c1;    
                return 1;
            }
            *lead = e->c1;
            *tail = e->c2;
            return 1;
        }
        e++;
    }    
    return 0;
}

#endif

int kmx_hangul_query_multitap(
    unsigned short                  prev_char,
    unsigned short                  key,    
    unsigned short*                 string_buffer,
    unsigned short*                 length,
    unsigned short*                 commit_char)
{    
#ifndef KMX_LDB_HANGUL
    return 0;
#else
    unsigned short hangul_mt_entry[MAX_HANGUL_ENTRY_LENGTH];
    const unsigned short* string;
    int i;
    int composed_char_count = 0;    

    if (!string_buffer || !length || !commit_char)
        return 0;
    
    if (key == 10)
    {
        unsigned short transform_char = 0;
        /* Handle transformation */
        if (IS_SYLLABLE(prev_char))
        {
            /* Syllable */
            unsigned short lc, vo, tc;
        
            lc = (unsigned short)L_FROM_S(prev_char);
            vo = (unsigned short)V_FROM_S(prev_char);

            if (!S_HAS_T(prev_char))
            {
                /* LV form */
                unsigned short new_vo;
                /* must be V transform */
                new_vo = find_mapping(char_transform_table, vo, 0x2A);
                if (new_vo)
                {
                    transform_char = (unsigned short)S_FROM_LV(lc, new_vo);
                }
            }
            else
            {
                /* LVT form */
                unsigned short new_tc;
                tc = (unsigned short)T_FROM_S(prev_char);
                new_tc = find_mapping(char_transform_table, tc, 0x2A);
                if (new_tc)
                {
                    transform_char = (unsigned short)S_FROM_LVT(lc, vo, new_tc);
                }
            }
        }
        else
        {
            transform_char = find_mapping(char_transform_table, prev_char, 0x2A);            
        }

        if (transform_char)
        {
            hangul_mt_entry[0] = transform_char;            
            *length = 1;
            memcpy(string_buffer, hangul_mt_entry, (*length) * sizeof(string_buffer[0]));
            *commit_char = 0;
            return 1;
        }
        else
        {            
            return 0;
        }
    }

    if (key > 9)
        return 0;
        
    *length = 0;
    *commit_char = 0;
    string = default_multitap_hangul_base[key];    

    while (string[*length])
    {
        *length = (unsigned short)(*length + 1);
    }

    memcpy(string_buffer, string, *length * sizeof(string_buffer[0]));

    /* Special Handle for 0x11B1, only show up at T position */
    if (key == 8)
    {
        *length = (unsigned short)(*length - 1);
    }

    if (prev_char == 0)
    {
        goto Finish;
    }
    
    if (key == 0 || !IS_HANGUL_CHAR(prev_char))
    {        
        *commit_char = prev_char;
        goto Finish;
    }
    
    if (IS_SYLLABLE(prev_char))
    {
        /* Syllable */
        unsigned short lc, vo, tc;
        
        lc = (unsigned short)L_FROM_S(prev_char);
        vo = (unsigned short)V_FROM_S(prev_char);

        if (!S_HAS_T(prev_char))
        {
            if (key == 8)
            {
                *length += 1;
            }
            /* LV form */
            for (i = 0; i < *length; i++)
            {
                unsigned short composed_char;
                unsigned short cur_char;                

                cur_char = string[i];
                composed_char = 0;

                if (IS_LC(cur_char))
                {                    
                    tc = find_mapping(tc_compose_table, cur_char, 0); /* convert to tc */  
                    if (tc)
                        composed_char = (unsigned short)S_FROM_LVT(lc, vo, tc);
                }
                else if (cur_char == 0x11B1)
                {                    
                    composed_char = (unsigned short)S_FROM_LVT(lc, vo, cur_char);
                }
                else
                {
                    unsigned short composed_vo;
                    composed_vo = find_mapping(vo_compose_table, vo, cur_char);
                    if (composed_vo)
                        composed_char = (unsigned short)S_FROM_LV(lc, composed_vo);
                }

                if (composed_char)
                {
                    hangul_mt_entry[composed_char_count++] = composed_char;
                }
            }
            if (composed_char_count)
            {                
                *length = (unsigned short)composed_char_count;
                memcpy(string_buffer, hangul_mt_entry, (*length) * sizeof(string_buffer[0]));
            }
            else
            {
                *commit_char = prev_char;
            }
        }
        else
        {
            /* LVT form */
            int decompose_syllable = 0;
            tc = (unsigned short)T_FROM_S(prev_char);
            for (i = 0; i < *length; i++)
            {
                unsigned short composed_char;
                unsigned short cur_char;                

                cur_char = string[i];
                composed_char = 0;

                if (IS_LC(cur_char))
                {
                    unsigned short composed_tc;
                    composed_tc = find_mapping(tc_compose_table, tc, cur_char);
                    if (composed_tc)
                        composed_char = (unsigned short)S_FROM_LVT(lc, vo, composed_tc);
                }
                else
                {
                    unsigned short ltc = 0, ttc = 0;
                    if (!decompose_syllable)
                    {
                        if (decompose_tc(tc, &ltc, &ttc))
                        {
                            if (ttc)
                            {                                
                                *commit_char = (unsigned short)S_FROM_LVT(lc, vo, ltc);
                                lc = ttc;
                            }
                            else
                            {
                                *commit_char = (unsigned short)S_FROM_LV(lc, vo);
                                lc = ltc;
                            }
                        }
                        else
                        {
                            /* Can not decompose, commit prev char and start new mt */
                            *commit_char = prev_char;
                            return 1;
                        }
                        decompose_syllable = 1;
                    }
                    
                    composed_char = (unsigned short)S_FROM_LV(lc, cur_char);
                }
                if (composed_char)
                {
                    hangul_mt_entry[composed_char_count++] = composed_char;
                }
            }
            if (composed_char_count)
            {                
                *length = (unsigned short)composed_char_count;
                memcpy(string_buffer, hangul_mt_entry, (*length) * sizeof(string_buffer[0]));
            }
            else
            {
                *commit_char = prev_char;
            }
        }
    }
    else
    {
        /* JAMO */

        /* TODO: consider transform key */
        for (i = 0; i < *length; i++)
        {
            unsigned short composed_char;
            unsigned short cur_char;

            cur_char = string[i];
            composed_char = 0;

            if (IS_LC(prev_char) && IS_VO(cur_char))
            {
                composed_char = (unsigned short)S_FROM_LV(prev_char, cur_char);
            }
            else
            {
                if (IS_LC(prev_char))
                    composed_char = find_mapping(lc_compose_table, prev_char, cur_char);
                else 
                    /* prev char cant be TC */
                    composed_char = find_mapping(vo_compose_table, prev_char, cur_char);

            }

            if (composed_char)
            {
                hangul_mt_entry[composed_char_count++] = composed_char;
            }
        }
        if (composed_char_count)
        {            
            *length = (unsigned short)composed_char_count;
            memcpy(string_buffer, hangul_mt_entry, (*length) * sizeof(string_buffer[0]));
        }
        else
        {
            *commit_char = prev_char;
        }
    }

Finish:
    return 1;
#endif
}

/*
 * 
 * De-serialize ldb object
 *
 */

typedef struct
{
    int header_size;
    int data_len;
    int crc_val;
    int version;    
} LDB_FOOTER;

#define LDB_RELOCATE_POINTER(p_member) if (p_member) p_member = (void*) (start + (unsigned int)(p_member))
#define LDB_READ_INT(p) (*((unsigned char*)(p))) | (*((unsigned char*)(p) + 1) << 8) | (*((unsigned char*)(p) + 2) << 16) | (*((unsigned char*)(p) + 3) << 24)

int kmx_latin_relocate_ldb(
    const void* raw_ldb_buffer, 
    int ldb_buffer_size,
    KMX_LDB_HEADER *ldb_header)
{
    LDB_FOOTER footer;
    LDB_BYTE* start;
    LDB_BYTE* p;
    int offset;
    LDB* ldb;
    
    VALIDATE(raw_ldb_buffer);
    VALIDATE(ldb_header);
    
    /* Alignment check - 4 bytes alignment of start address is necessary */
    VALIDATE((((unsigned int)raw_ldb_buffer) & 3) == 0);
    
    VALIDATE(ldb_buffer_size >= ( sizeof(LDB) + sizeof(LDB_FOOTER) ));
    
    start = (LDB_BYTE*)raw_ldb_buffer;
    
    /* Load footer */
    memset(&footer, 0, sizeof(footer));
    p =  start + ldb_buffer_size - sizeof(LDB_FOOTER);
    offset = 0;

    footer.header_size = LDB_READ_INT(p + offset);
    offset += sizeof(footer.header_size);

    footer.data_len = LDB_READ_INT(p + offset);
    offset += sizeof(footer.data_len);

    footer.crc_val = LDB_READ_INT(p + offset);
    offset += sizeof(footer.crc_val);

    footer.version = LDB_READ_INT(p + offset);
    offset += sizeof(footer.version);
    
    VALIDATE(footer.data_len == ldb_buffer_size);    
    VALIDATE(footer.header_size >= sizeof(LDB));
    VALIDATE(footer.version != 0);    
    VALIDATE( crc(start, ldb_buffer_size - sizeof(LDB_FOOTER)) == footer.crc_val );

    /* Seems everything goes fine here. Relocate all the pointers! */
    VALIDATE(footer.header_size <= sizeof(KMX_LDB_HEADER));
    memset(ldb_header, 0, sizeof(KMX_LDB_HEADER));
    memcpy(ldb_header, raw_ldb_buffer, footer.header_size);
    
    ldb = (LDB*) ldb_header;        
    
    LDB_RELOCATE_POINTER(ldb->copyright);
    LDB_RELOCATE_POINTER(ldb->eudw_prompt);
    LDB_RELOCATE_POINTER(ldb->eudw_title);
    LDB_RELOCATE_POINTER(ldb->alphabet);
    LDB_RELOCATE_POINTER(ldb->lc2ucm);
    LDB_RELOCATE_POINTER(ldb->uc2lcm);
    LDB_RELOCATE_POINTER(ldb->sos_table);
    LDB_RELOCATE_POINTER(ldb->sou_table);
    LDB_RELOCATE_POINTER(ldb->mt_index);
    LDB_RELOCATE_POINTER(ldb->mt_string);
    LDB_RELOCATE_POINTER(ldb->legram_index);
    LDB_RELOCATE_POINTER(ldb->legram_string);
    LDB_RELOCATE_POINTER(ldb->word_index);
    LDB_RELOCATE_POINTER(ldb->word_table);
    LDB_RELOCATE_POINTER(ldb->char_table);
    LDB_RELOCATE_POINTER(ldb->ckey_table);
    LDB_RELOCATE_POINTER(ldb->string_table);
    LDB_RELOCATE_POINTER(ldb->bigram_index);
    LDB_RELOCATE_POINTER(ldb->bigram);
    LDB_RELOCATE_POINTER(ldb->trigram);
    LDB_RELOCATE_POINTER(ldb->exgram_index);
    LDB_RELOCATE_POINTER(ldb->exgram);    
    
    return KMX_TRUE;
}
