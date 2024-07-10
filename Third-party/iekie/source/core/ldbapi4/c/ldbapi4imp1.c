
/*

Supported languages:

    10x English
    20x Russian
    30x German
    31x Vietnamese
    32x Indonesian
    33x Turkish
    34x Malay
    35x Greek
    36x Thai
    37x Danish
    38x Dutch
    39x Polish
    40x French
    41x Hungarian
    42x Hebrew          
    43x Bulgarian
    44x Czech
    45x Finnish         
    46x Persian
    47x Slovak          
    48x Norwegian       
    49x Swedish         
    50x Spanish
    51x Croatian        Not ready
    52x Romanian        
    53x Slovenian       
	54x Ukrainian
	55x Swahili
	56x Lithuanian
	57x Armenian
    60x Portuguese
    70x Italian
    80x Arabic
    90x Hindi
	91x Hinglish
	92x Bengali
    94x Uyghur

*/

#ifdef _MSC_VER
    /* Disable Microsoft Visial C++ warning: unreferenced formal parameter */
    #pragma warning(disable: 4100)
    /* Disable Microsoft Visial C++ warning: named type definition in parentheses */
    #pragma warning(disable: 4115)
    /* Disable Microsoft Visial C++ warning: nonstandard extension used: nameless struct/union */
    #pragma warning(disable: 4201)
    /* Disable Microsoft Visial C++ warning: nonstandard extension used : bit field types other than int */
    #pragma warning(disable: 4214)
    /* Disable Microsoft Visial C++ warning: unreferenced local function has been removed */
    #pragma warning(disable: 4505)
#endif

#include "IIMEstd.h"
#include "ldbapi4.h"

#include <string.h>
#include <stdlib.h>
#define NDEBUG
#ifdef NDEBUG
    #ifdef LDB_DEBUG
        #undef LDB_DEBUG
    #endif
    #define LDB_ASSERT(e)   ((void)0)
#else
    #ifndef LDB_DEBUG
        #define LDB_DEBUG
    #endif
    #include <assert.h>
    #define LDB_ASSERT      assert
    #ifdef _MSC_VER
        #pragma message("    Debug is on")
    #endif
#endif

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Database definitions                                                    */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Must be an even number for proper alignment */
#define MAX_WORD_LENGTH     24

#define LDB_WID             LDB_WORD

typedef struct tagLDB
{
    int                     did;
    int                     major_version;
    int                     minor_version;
    int                     build_number;

    const LDB_CHAR*         copyright;              /* NULL-terminated string */
    int                     copyright_length;

    const LDB_CHAR*         eudw_prompt;            /* NULL-terminated string */
    int                     eudw_prompt_length;

    const LDB_CHAR*         eudw_title;             /* NULL-terminated string */
    int                     eudw_title_length;

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

    const LDB_WORD*         mt_index;               /* 1 extra item at the end */
    const LDB_CHAR*         mt_string;
    int                     mt_string_length;

    const LDB_WORD*         legram_index;           /* 1 extra item at the end */
    const LDB_CHAR*         legram_string;
    int                     legram_string_length;

    int                     word_count;
    int                     word_id_bits;
    LDB_WID                 wid_oov;
    LDB_WID                 wid_sos;

    const LDB_BYTE*         word_index;             /* 1 extra item at the end */
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

    const LDB_BYTE*         bigram_index;           /* 1 extra item at the end */
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
} LDB;

typedef const LDB* LDB_CPTR;

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* User data definitions                                                   */
/*                                                                         */
/*-------------------------------------------------------------------------*/

typedef struct tagUDB
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

typedef struct tagUDB_HIS
{
    LDB_WORD                prev;   /* Size of previous item */
    LDB_WORD                next;   /* Size of current item */
    LDB_WID                 wid;
    LDB_WORD                len;
} UDB_HIS;

typedef struct tagUDB_OOV
{
    LDB_WORD                prev;   /* Size of previous item */
    LDB_WORD                next;   /* Size of current item */
} UDB_OOV;

/* Minimum byte size of cache database */
#define UDB_MIN             1024

/* Maximum byte size of cache database */
#define UDB_MAX             16384

/* Iterator for history items */
#define UDB_HIT             int

/* Iterator for OOV items */
#define UDB_OIT             int

#define USIZE               ((int)sizeof(UDB))
#define HSIZE               ((int)sizeof(UDB_HIS))
#define OSIZE               ((int)sizeof(UDB_OOV))
#define IIMEIZE               ((int)sizeof(LDB_CHAR))

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Match engine definitions                                                */
/*                                                                         */
/*-------------------------------------------------------------------------*/

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

typedef struct tagMATCH
{
    int                     rank;
    int                     length;
    LDB_CHAR                string[MAX_WORD_LENGTH + 2];
} MATCH;

#ifdef SPREADTRUM_PRODUCT
#define MAX_MATCH           48
#else
#define MAX_MATCH           32
#endif

#define FORWARD_MATCH       5

#define LDB_SMART_ENGINE_SIGNATURE      0x4D4D4D4D
#define LDB_MULTITAP_ENGINE_SIGNATURE   0x5D5D5D5D

#define DECLARE_BASE_ENGINE_MEMBERS() \
    int                     signature;\
    int                     did;\
    int                     major_version;\
    int                     minor_version;\
    int                     build_number;\
\
    const LDB_CHAR*         copyright;\
    int                     copyright_length;\
\
    const LDB_CHAR*         eudw_prompt;\
    int                     eudw_prompt_length;\
\
    const LDB_CHAR*         eudw_title;\
    int                     eudw_title_length;\
\
    const LDB_CHAR*         alphabet;\
    int                     alphabet_length;\
\
    const LDB_CHAR*         lc2ucm;\
    int                     lc2ucm_length;\
\
    const LDB_CHAR*         uc2lcm;\
    int                     uc2lcm_length;\
\
    const LDB_CHAR*         sos_table;\
    int                     sos_table_length;\
\
    const LDB_CHAR*         sou_table;\
    int                     sou_table_length;\
\
    const LDB_WORD*         mt_index;\
    const LDB_CHAR*         mt_string;\
    int                     mt_string_length;\
\
    int                     char_code_count;\
    int                     char_code_bits;\
\
    const LDB_CHAR*         char_table;\
    const LDB_BYTE*         ckey_table;\
\
    int                     language;\
    int                     is_sure_type;\
\
    int                     key_count;\
    int                     flags;

typedef struct
{
   DECLARE_BASE_ENGINE_MEMBERS()

} BASE_ENGINE;

typedef struct tagENGINE
{
    DECLARE_BASE_ENGINE_MEMBERS()

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
} ENGINE;


#define LDB_MT_ENTRY_MAX_CHARS      48
#define LDB_MT_ENTRY_MAX_NUM        12
#define LDB_MT_TABLE_MAX_NUM        6  

typedef struct 
{
    unsigned short      count;
    LDB_CHAR            chars[LDB_MT_ENTRY_MAX_CHARS];
}LDB_MT_ENTRY;

typedef struct 
{
    unsigned short      count;
    LDB_MT_ENTRY        entries[LDB_MT_ENTRY_MAX_NUM];
}LDB_MT_TABLE;

typedef struct tagMT_ENGINE
{
   DECLARE_BASE_ENGINE_MEMBERS()

    /* Dynamic mt table */
    int                     table_count;
    LDB_MT_TABLE            tables[LDB_MT_TABLE_MAX_NUM];
} MT_ENGINE;


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

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* General definitions                                                     */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define LDB_TRUE            1
#define LDB_FALSE           0

#define VALIDATE(exp)       if (!(exp)) return LDB_FALSE

#define LDB_9KEY_FIRST_KEY          LDB_KEY_0
#define LDB_9KEY_LAST_KEY           LDB_KEY_POUND
#define IS_VALID_9KEY_INPUT(c)      ( (c) >= LDB_9KEY_FIRST_KEY && (c) <= LDB_9KEY_LAST_KEY )

#define LDB_SURE_TYPE_FIRST_KEY     LDB_KEY_0
#define LDB_SURE_TYPE_LAST_KEY      LDB_KEY_15
#define IS_VALID_SURE_TYPE_INPUT(c) ( (c) >= LDB_SURE_TYPE_FIRST_KEY && (c) <= LDB_SURE_TYPE_LAST_KEY )

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Character constants                                                     */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define LDB_CHAR_SPACE      0x20
#define LDB_CHAR_APOSTROPHE 0x27
#define LDB_CHAR_COMMA      0x2C
#define LDB_CHAR_FULL_STOP  0x2E

#define LDB_CHAR_DIGIT_0            0x30
#define LDB_CHAR_DIGIT_9            0x39

#define LDB_CHAR_ARABIC_DIGIT_0     0x0660
#define LDB_CHAR_ARABIC_DIGIT_9     0x0669

#define LDB_CHAR_PERSIAN_DIGIT_0    0x06F0
#define LDB_CHAR_PERSIAN_DIGIT_9    0x06F9

#define LDB_CHAR_HINDI_DIGIT_0      0x0966
#define LDB_CHAR_HINDI_DIGIT_9      0x096F

#define LDB_CHAR_NEPALI_DIGIT_0      0x0966
#define LDB_CHAR_NEPALI_DIGIT_9      0x096F

#define LDB_CHAR_SINHALA_DIGIT_0      0x0966
#define LDB_CHAR_SINHALA_DIGIT_9      0x096F

//daodejing need
#define LDB_CHAR_BODO_DIGIT_0      0x0966
#define LDB_CHAR_BODO_DIGIT_9      0x096F

#define LDB_CHAR_DOGRI_DIGIT_0      0x0966
#define LDB_CHAR_DOGRI_DIGIT_9      0x096F

#define LDB_CHAR_SANSKRIT_DIGIT_0      0x0966
#define LDB_CHAR_SANSKRIT_DIGIT_9      0x096F

#define LDB_CHAR_KASHMIRI_DIGIT_0      0x0966
#define LDB_CHAR_KASHMIRI_DIGIT_9      0x096F

#define LDB_CHAR_KONKANI_DIGIT_0      0x0966
#define LDB_CHAR_KONKANI_DIGIT_9      0x096F

#define LDB_CHAR_MAITHILI_DIGIT_0      0x0966
#define LDB_CHAR_MAITHILI_DIGIT_9      0x096F

#define LDB_CHAR_MANIPURI_DIGIT_0      0x0966
#define LDB_CHAR_MANIPURI_DIGIT_9      0x096F

#define LDB_CHAR_SANTALI_DIGIT_0      0x0966
#define LDB_CHAR_SANTALI_DIGIT_9      0x096F

#define LDB_CHAR_SOMALI_DIGIT_0      0x0966
#define LDB_CHAR_SOMALI_DIGIT_9      0x096F

#define LDB_CHAR_SINDHI_DIGIT_0      0x0966
#define LDB_CHAR_SINDHI_DIGIT_9      0x096F

#define LDB_CHAR_BENGALI_DIGIT_0    0x09E6
#define LDB_CHAR_BENGALI_DIGIT_9    0x09EF

#define LDB_CHAR_THAI_DIGIT_0       0x0E50
#define LDB_CHAR_THAI_DIGIT_9       0x0E59


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Helper functions                                                        */
/*                                                                         */
/*-------------------------------------------------------------------------*/

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
    LDB_ASSERT(value >= 0);
    LDB_ASSERT(unit > 0);

    return ((value % unit) == 0) ? LDB_TRUE : LDB_FALSE;
}

static int align_adjust(
    IN int                  value,
    IN int                  unit)
{
    int                     test;

    LDB_ASSERT(value >= 0);
    LDB_ASSERT(unit > 0);

    test = value % unit;

    return (test > 0) ? (value + (unit - test)) : value;
}

//读取几个bit位的数据并返回
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

    LDB_ASSERT(data);
    LDB_ASSERT(offset >= 0);
    LDB_ASSERT( (bits > 0) && (bits <= 19) );
	//offset:这个偏移，单位是bit
	//得到偏移之后的坐标
    data += offset / 8;//偏移字节

    if (bits == 1)
		//如果只是读取一位:	
        return (data[0] >> (offset % 8)) & 1;//(data[0] >> (offset % 8)): 操作的目标地址	位与--只能得到该地址的最后一位
    else if (bits <= 9)
        return (((data[1] << 8) | data[0]) >> (offset % 8)) & BITS_MASK[bits];
			//((data[1] << 8) | data[0]):	接近目标的地址数据拼装
				//>> (offset % 8)	:	得到目标地址数据
				//0x000001FF:	这个是，刚好九位有效
    else
        return (((data[2] << 16) | (data[1] << 8) | data[0]) >> (offset % 8)) & BITS_MASK[bits];
			//同理!
}

//ZHMCH	返回给定字符所在str_table的index
static int sorted_strchr(
    IN const LDB_CHAR*      str,
    IN int                  len,
    IN LDB_CHAR             c)
{
    int                     s;
    int                     e;
    int                     i;

    if ( (str!= 0) && (len > 0) )
    {
        LDB_ASSERT(str);

        s = 0;
        e = len - 1;

        while (s <= e)
        {
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

static int has_key(
    IN const ENGINE*        eng,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    if (eng->is_sure_type)
    {
    while (len > 0)
        {
            /* loosen the key cap from LDB_KEY_9 to LDB_KEY_STAR */
            if (IS_VALID_SURE_TYPE_INPUT(*str))
                return LDB_TRUE;
            str++;
            len--;
        }
    }
    else
    {
        while (len > 0)
        {
            /* loosen the key cap from LDB_KEY_9 to LDB_KEY_STAR */
            if (IS_VALID_9KEY_INPUT(*str))
            return LDB_TRUE;
        str++;
        len--;
        }
    }
    return LDB_FALSE;
}

typedef int (*IS_VALID_FOLLOW_CHAR_FUNC)(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2);

static int init_mt_tables_impl(
    IN MT_ENGINE*            eng,
    IN LDB_CHAR*             prev_chars,
    IN unsigned short        prev_char_count,
    IN IS_VALID_FOLLOW_CHAR_FUNC is_valid_follow_char)
{
    int table_count;
    int i, j, k;
    LDB_MT_TABLE* table;
    
    table_count = prev_char_count;
    if (eng->key_count > LDB_MT_ENTRY_MAX_NUM)
        return 0;

    LDB_ASSERT(table_count <= LDB_MT_TABLE_MAX_NUM);

    for (i = 0; i < table_count; i++)
    {
        LDB_CHAR prev_char = prev_chars[i];

        table = &eng->tables[i];
        table->count = (unsigned short)eng->key_count;
        
        for (j = 0; j < eng->key_count; j++)
        {
            int char_count;
            const LDB_CHAR* chars;
            LDB_MT_ENTRY* e;

            e = &table->entries[j];
            e->count = 0;
            chars = ldbapi4_get_multitype(eng, (LDB_CHAR)(LDB_KEY_0 + j), 0, &char_count);
            if (!chars || !char_count || char_count > LDB_MT_ENTRY_MAX_CHARS - 1)
                return 0;

            for (k = 0; k < char_count; k++)
            {
                if (is_valid_follow_char(prev_char, chars[k]))
                {
                    e->chars[e->count] = chars[k];
                    e->count++;
                }
            }
            e->chars[e->count] = 0;
        }
    }
    eng->table_count = table_count;
    
    return 1;
}
/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Thai functions                                                          */
/*                                                                         */
/*-------------------------------------------------------------------------*/

unsigned char ldbapi4_get_thai_char_type(
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

int ldbapi4_is_valid_thai_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_thai_char_type(c1);
    t2 = ldbapi4_get_thai_char_type(c2);
    
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

int ldbapi4_is_valid_thai_word_start_char(
    IN LDB_CHAR             c)
{
    unsigned char t = ldbapi4_get_thai_char_type(c);

    if(t & (LDB_THAI_DV | LDB_THAI_TM | LDB_THAI_JS))
        return 0;

    return 1;
}

static int thai_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0E01, /* C */
                              0x0E31, /* DV */
                              0x0000, /* OOV */};
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_thai_follow_char);    
}

const LDB_CHAR* ldbapi4_get_thai_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#ifdef KMX_LDB_THAI
    const LDB_MT_ENTRY*     e;
    const LDB_MT_ENTRY*     thai_mt_table_c;
    const LDB_MT_ENTRY*     thai_mt_table_dv;
    const LDB_MT_ENTRY*     thai_mt_table_oov_idv_tm_s;
    MT_ENGINE*              eng;
    int                     i;
    unsigned char           t;

    LDB_ASSERT(count);
    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_POUND) )
        return 0;           /* Invalid key */

    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 3)
        return 0;            

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;

    thai_mt_table_c            = eng->tables[0].entries;
    thai_mt_table_dv           = eng->tables[1].entries;
    thai_mt_table_oov_idv_tm_s = eng->tables[2].entries;
    
    t = ldbapi4_get_thai_char_type(prev_char);
    if (t & (LDB_THAI_OOV | LDB_THAI_IDV | LDB_THAI_TM | LDB_THAI_S))
        e = &(thai_mt_table_oov_idv_tm_s[i]);
    else if (t == LDB_THAI_DV)
        e = &(thai_mt_table_dv[i]);
    else
        e = &(thai_mt_table_c[i]);

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Hindi functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define H_OOV  LDB_HINDI_OOV
#define H_CON  LDB_HINDI_CON
#define H_INV  LDB_HINDI_INV
#define H_DEV  LDB_HINDI_DEV
#define H_VIR  LDB_HINDI_VIR
#define H_SIG  LDB_HINDI_SIG

unsigned char ldbapi4_get_hindi_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_HINDI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_HINDI_OOV;
#endif
}

int ldbapi4_is_valid_hindi_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_HINDI)
    static const unsigned char HINDI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return HINDI_CHAR_START_TABLE[ldbapi4_get_hindi_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_hindi_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_HINDI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_hindi_char_type(c1);
    t2 = ldbapi4_get_hindi_char_type(c2);

    return HINDI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int hindi_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_hindi_follow_char);
}

const LDB_CHAR* ldbapi4_get_hindi_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_HINDI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* hindi_multitap_table_0;
    const LDB_MT_ENTRY* hindi_multitap_table_1;
    const LDB_MT_ENTRY* hindi_multitap_table_2;
    const LDB_MT_ENTRY* hindi_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    hindi_multitap_table_0 = eng->tables[0].entries;
    hindi_multitap_table_1 = eng->tables[1].entries;
    hindi_multitap_table_2 = eng->tables[2].entries;
    hindi_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_hindi_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(hindi_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(hindi_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(hindi_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(hindi_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Nepali functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_NEPALI_OOV
#define N_CON  LDB_NEPALI_CON
#define N_INV  LDB_NEPALI_INV
#define N_DEV  LDB_NEPALI_DEV
#define N_VIR  LDB_NEPALI_VIR
#define N_SIG  LDB_NEPALI_SIG

unsigned char ldbapi4_get_nepali_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_NEPALI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_NEPALI_OOV;
#endif
}

int ldbapi4_is_valid_nepali_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_NEPALI)
    static const unsigned char NEPALI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return NEPALI_CHAR_START_TABLE[ldbapi4_get_nepali_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_nepali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_NEPALI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_nepali_char_type(c1);
    t2 = ldbapi4_get_nepali_char_type(c2);

    return NEPALI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int nepali_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_nepali_follow_char);
}

const LDB_CHAR* ldbapi4_get_nepali_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_NEPALI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* nepali_multitap_table_0;
    const LDB_MT_ENTRY* nepali_multitap_table_1;
    const LDB_MT_ENTRY* nepali_multitap_table_2;
    const LDB_MT_ENTRY* nepali_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    nepali_multitap_table_0 = eng->tables[0].entries;
    nepali_multitap_table_1 = eng->tables[1].entries;
    nepali_multitap_table_2 = eng->tables[2].entries;
    nepali_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_nepali_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(nepali_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(nepali_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(nepali_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(nepali_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}



/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Sinhala functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define S_OOV  LDB_SINHALA_OOV
#define S_CON  LDB_SINHALA_CON
#define S_INV  LDB_SINHALA_INV
#define S_DEV  LDB_SINHALA_DEV
#define S_VIR  LDB_SINHALA_VIR
#define S_SIG  LDB_SINHALA_SIG

unsigned char ldbapi4_get_sinhala_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SINHALA)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_SINHALA_OOV;
#endif
}

int ldbapi4_is_valid_sinhala_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SINHALA)
    static const unsigned char SINHALA_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SINHALA_CHAR_START_TABLE[ldbapi4_get_sinhala_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_sinhala_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_SINHALA)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_sinhala_char_type(c1);
    t2 = ldbapi4_get_sinhala_char_type(c2);

    return SINHALA_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int sinhala_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_sinhala_follow_char);
}

const LDB_CHAR* ldbapi4_get_sinhala_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_SINHALA)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* sinhala_multitap_table_0;
    const LDB_MT_ENTRY* sinhala_multitap_table_1;
    const LDB_MT_ENTRY* sinhala_multitap_table_2;
    const LDB_MT_ENTRY* sinhala_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    sinhala_multitap_table_0 = eng->tables[0].entries;
    sinhala_multitap_table_1 = eng->tables[1].entries;
    sinhala_multitap_table_2 = eng->tables[2].entries;
    sinhala_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_sinhala_char_type(prev_char))
    {
        default:
        case S_OOV:
            e = &(sinhala_multitap_table_0[i]);
            break;

        case S_CON:
            e = &(sinhala_multitap_table_1[i]);
            break;

        case S_INV:
        case S_DEV:
            e = &(sinhala_multitap_table_2[i]);
            break;

        case S_VIR:
        case S_SIG:
            e = &(sinhala_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

//daodejing  need
/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Bodo functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_BODO_OOV
#define N_CON  LDB_BODO_CON
#define N_INV  LDB_BODO_INV
#define N_DEV  LDB_BODO_DEV
#define N_VIR  LDB_BODO_VIR
#define N_SIG  LDB_BODO_SIG

unsigned char ldbapi4_get_bodo_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_BODO)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
        return BODO_CHAR_TYPE_TABLE[c - 0x900];//daodejing need
    else
        return LDB_BODO_OOV;
#else
        return LDB_BODO_OOV;
#endif
}

int ldbapi4_is_valid_bodo_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_BODO)
    static const unsigned char BODO_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return BODO_CHAR_START_TABLE[ldbapi4_get_bodo_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_bodo_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_BODO)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_bodo_char_type(c1);
    t2 = ldbapi4_get_bodo_char_type(c2);

    return BODO_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int bodo_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_bodo_follow_char);
}

const LDB_CHAR* ldbapi4_get_bodo_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_BODO)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* bodo_multitap_table_0;
    const LDB_MT_ENTRY* bodo_multitap_table_1;
    const LDB_MT_ENTRY* bodo_multitap_table_2;
    const LDB_MT_ENTRY* bodo_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    bodo_multitap_table_0 = eng->tables[0].entries;
    bodo_multitap_table_1 = eng->tables[1].entries;
    bodo_multitap_table_2 = eng->tables[2].entries;
    bodo_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_bodo_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(bodo_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(bodo_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(bodo_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(bodo_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Dogri functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_DOGRI_OOV
#define N_CON  LDB_DOGRI_CON
#define N_INV  LDB_DOGRI_INV
#define N_DEV  LDB_DOGRI_DEV
#define N_VIR  LDB_DOGRI_VIR
#define N_SIG  LDB_DOGRI_SIG

unsigned char ldbapi4_get_dogri_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_DOGRI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_DOGRI_OOV;
#endif
}

int ldbapi4_is_valid_dogri_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_DOGRI)
    static const unsigned char DOGRI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return DOGRI_CHAR_START_TABLE[ldbapi4_get_dogri_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_dogri_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_DOGRI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_dogri_char_type(c1);
    t2 = ldbapi4_get_dogri_char_type(c2);

    return DOGRI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int dogri_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_dogri_follow_char);
}

const LDB_CHAR* ldbapi4_get_dogri_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_DOGRI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* dogri_multitap_table_0;
    const LDB_MT_ENTRY* dogri_multitap_table_1;
    const LDB_MT_ENTRY* dogri_multitap_table_2;
    const LDB_MT_ENTRY* dogri_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    dogri_multitap_table_0 = eng->tables[0].entries;
    dogri_multitap_table_1 = eng->tables[1].entries;
    dogri_multitap_table_2 = eng->tables[2].entries;
    dogri_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_dogri_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(dogri_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(dogri_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(dogri_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(dogri_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Sanskrit functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_SANSKRIT_OOV
#define N_CON  LDB_SANSKRIT_CON
#define N_INV  LDB_SANSKRIT_INV
#define N_DEV  LDB_SANSKRIT_DEV
#define N_VIR  LDB_SANSKRIT_VIR
#define N_SIG  LDB_SANSKRIT_SIG

unsigned char ldbapi4_get_sanskrit_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SANSKRIT)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_SANSKRIT_OOV;
#endif
}

int ldbapi4_is_valid_sanskrit_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SANSKRIT)
    static const unsigned char SANSKRIT_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SANSKRIT_CHAR_START_TABLE[ldbapi4_get_sanskrit_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_sanskrit_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_SANSKRIT)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_sanskrit_char_type(c1);
    t2 = ldbapi4_get_sanskrit_char_type(c2);

    return SANSKRIT_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int sanskrit_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_sanskrit_follow_char);
}

const LDB_CHAR* ldbapi4_get_sanskrit_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_SANSKRIT)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* sanskrit_multitap_table_0;
    const LDB_MT_ENTRY* sanskrit_multitap_table_1;
    const LDB_MT_ENTRY* sanskrit_multitap_table_2;
    const LDB_MT_ENTRY* sanskrit_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    sanskrit_multitap_table_0 = eng->tables[0].entries;
    sanskrit_multitap_table_1 = eng->tables[1].entries;
    sanskrit_multitap_table_2 = eng->tables[2].entries;
    sanskrit_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_sanskrit_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(sanskrit_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(sanskrit_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(sanskrit_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(sanskrit_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Kashmiri functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_KASHMIRI_OOV
#define N_CON  LDB_KASHMIRI_CON
#define N_INV  LDB_KASHMIRI_INV
#define N_DEV  LDB_KASHMIRI_DEV
#define N_VIR  LDB_KASHMIRI_VIR
#define N_SIG  LDB_KASHMIRI_SIG

unsigned char ldbapi4_get_kashmiri_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_KASHMIRI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_KASHMIRI_OOV;
#endif
}

int ldbapi4_is_valid_kashmiri_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_KASHMIRI)
    static const unsigned char KASHMIRI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return KASHMIRI_CHAR_START_TABLE[ldbapi4_get_kashmiri_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_kashmiri_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_KASHMIRI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_kashmiri_char_type(c1);
    t2 = ldbapi4_get_kashmiri_char_type(c2);

    return KASHMIRI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int kashmiri_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_kashmiri_follow_char);
}

const LDB_CHAR* ldbapi4_get_kashmiri_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_KASHMIRI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* kashmiri_multitap_table_0;
    const LDB_MT_ENTRY* kashmiri_multitap_table_1;
    const LDB_MT_ENTRY* kashmiri_multitap_table_2;
    const LDB_MT_ENTRY* kashmiri_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    kashmiri_multitap_table_0 = eng->tables[0].entries;
    kashmiri_multitap_table_1 = eng->tables[1].entries;
    kashmiri_multitap_table_2 = eng->tables[2].entries;
    kashmiri_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_kashmiri_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(kashmiri_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(kashmiri_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(kashmiri_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(kashmiri_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Konkani functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_KONKANI_OOV
#define N_CON  LDB_KONKANI_CON
#define N_INV  LDB_KONKANI_INV
#define N_DEV  LDB_KONKANI_DEV
#define N_VIR  LDB_KONKANI_VIR
#define N_SIG  LDB_KONKANI_SIG

unsigned char ldbapi4_get_konkani_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_KONKANI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_KONKANI_OOV;
#endif
}

int ldbapi4_is_valid_konkani_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_KONKANI)
    static const unsigned char KONKANI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return KONKANI_CHAR_START_TABLE[ldbapi4_get_konkani_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_konkani_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_KONKANI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_konkani_char_type(c1);
    t2 = ldbapi4_get_konkani_char_type(c2);

    return KONKANI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int konkani_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_konkani_follow_char);
}

const LDB_CHAR* ldbapi4_get_konkani_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_KONKANI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* konkani_multitap_table_0;
    const LDB_MT_ENTRY* konkani_multitap_table_1;
    const LDB_MT_ENTRY* konkani_multitap_table_2;
    const LDB_MT_ENTRY* konkani_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    konkani_multitap_table_0 = eng->tables[0].entries;
    konkani_multitap_table_1 = eng->tables[1].entries;
    konkani_multitap_table_2 = eng->tables[2].entries;
    konkani_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_konkani_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(konkani_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(konkani_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(konkani_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(konkani_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Maithili functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_MAITHILI_OOV
#define N_CON  LDB_MAITHILI_CON
#define N_INV  LDB_MAITHILI_INV
#define N_DEV  LDB_MAITHILI_DEV
#define N_VIR  LDB_MAITHILI_VIR
#define N_SIG  LDB_MAITHILI_SIG

unsigned char ldbapi4_get_maithili_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_MAITHILI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_MAITHILI_OOV;
#endif
}

int ldbapi4_is_valid_maithili_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_MAITHILI)
    static const unsigned char MAITHILI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return MAITHILI_CHAR_START_TABLE[ldbapi4_get_maithili_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_maithili_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_MAITHILI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_maithili_char_type(c1);
    t2 = ldbapi4_get_maithili_char_type(c2);

    return MAITHILI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int maithili_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_maithili_follow_char);
}

const LDB_CHAR* ldbapi4_get_maithili_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_MAITHILI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* maithili_multitap_table_0;
    const LDB_MT_ENTRY* maithili_multitap_table_1;
    const LDB_MT_ENTRY* maithili_multitap_table_2;
    const LDB_MT_ENTRY* maithili_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    maithili_multitap_table_0 = eng->tables[0].entries;
    maithili_multitap_table_1 = eng->tables[1].entries;
    maithili_multitap_table_2 = eng->tables[2].entries;
    maithili_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_maithili_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(maithili_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(maithili_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(maithili_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(maithili_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Manipuri functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_MANIPURI_OOV
#define N_CON  LDB_MANIPURI_CON
#define N_INV  LDB_MANIPURI_INV
#define N_DEV  LDB_MANIPURI_DEV
#define N_VIR  LDB_MANIPURI_VIR
#define N_SIG  LDB_MANIPURI_SIG

unsigned char ldbapi4_get_manipuri_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_MANIPURI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_MANIPURI_OOV;
#endif
}

int ldbapi4_is_valid_manipuri_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_MANIPURI)
    static const unsigned char MANIPURI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return MANIPURI_CHAR_START_TABLE[ldbapi4_get_manipuri_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_manipuri_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_MANIPURI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_manipuri_char_type(c1);
    t2 = ldbapi4_get_manipuri_char_type(c2);

    return MANIPURI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int manipuri_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_manipuri_follow_char);
}

const LDB_CHAR* ldbapi4_get_manipuri_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_MANIPURI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* manipuri_multitap_table_0;
    const LDB_MT_ENTRY* manipuri_multitap_table_1;
    const LDB_MT_ENTRY* manipuri_multitap_table_2;
    const LDB_MT_ENTRY* manipuri_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    manipuri_multitap_table_0 = eng->tables[0].entries;
    manipuri_multitap_table_1 = eng->tables[1].entries;
    manipuri_multitap_table_2 = eng->tables[2].entries;
    manipuri_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_manipuri_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(manipuri_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(manipuri_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(manipuri_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(manipuri_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Santali functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_SANTALI_OOV
#define N_CON  LDB_SANTALI_CON
#define N_INV  LDB_SANTALI_INV
#define N_DEV  LDB_SANTALI_DEV
#define N_VIR  LDB_SANTALI_VIR
#define N_SIG  LDB_SANTALI_SIG

unsigned char ldbapi4_get_santali_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SANTALI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_SANTALI_OOV;
#endif
}

int ldbapi4_is_valid_santali_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SANTALI)
    static const unsigned char SANTALI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SANTALI_CHAR_START_TABLE[ldbapi4_get_santali_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_santali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_SANTALI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_santali_char_type(c1);
    t2 = ldbapi4_get_santali_char_type(c2);

    return SANTALI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int santali_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_santali_follow_char);
}

const LDB_CHAR* ldbapi4_get_santali_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_SANTALI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* santali_multitap_table_0;
    const LDB_MT_ENTRY* santali_multitap_table_1;
    const LDB_MT_ENTRY* santali_multitap_table_2;
    const LDB_MT_ENTRY* santali_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    santali_multitap_table_0 = eng->tables[0].entries;
    santali_multitap_table_1 = eng->tables[1].entries;
    santali_multitap_table_2 = eng->tables[2].entries;
    santali_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_santali_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(santali_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(santali_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(santali_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(santali_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Somali functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_SOMALI_OOV
#define N_CON  LDB_SOMALI_CON
#define N_INV  LDB_SOMALI_INV
#define N_DEV  LDB_SOMALI_DEV
#define N_VIR  LDB_SOMALI_VIR
#define N_SIG  LDB_SOMALI_SIG

unsigned char ldbapi4_get_somali_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SOMALI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_SOMALI_OOV;
#endif
}

int ldbapi4_is_valid_somali_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SOMALI)
    static const unsigned char SOMALI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SOMALI_CHAR_START_TABLE[ldbapi4_get_somali_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_somali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_SOMALI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_somali_char_type(c1);
    t2 = ldbapi4_get_somali_char_type(c2);

    return SOMALI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int somali_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_somali_follow_char);
}

const LDB_CHAR* ldbapi4_get_somali_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_SOMALI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* somali_multitap_table_0;
    const LDB_MT_ENTRY* somali_multitap_table_1;
    const LDB_MT_ENTRY* somali_multitap_table_2;
    const LDB_MT_ENTRY* somali_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    somali_multitap_table_0 = eng->tables[0].entries;
    somali_multitap_table_1 = eng->tables[1].entries;
    somali_multitap_table_2 = eng->tables[2].entries;
    somali_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_somali_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(somali_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(somali_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(somali_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(somali_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Sindhi functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define N_OOV  LDB_SINDHI_OOV
#define N_CON  LDB_SINDHI_CON
#define N_INV  LDB_SINDHI_INV
#define N_DEV  LDB_SINDHI_DEV
#define N_VIR  LDB_SINDHI_VIR
#define N_SIG  LDB_SINDHI_SIG

unsigned char ldbapi4_get_sindhi_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SINDHI)
    /*  093C is a Sign, but it can only follow CON, so we set it as VIR */
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
#else
        return LDB_SINDHI_OOV;
#endif
}

int ldbapi4_is_valid_sindhi_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_SINDHI)
    static const unsigned char SINDHI_CHAR_START_TABLE[6] =
    {
        /* OOV CON INV DEV VIR SIG */
            1,  1,  1,  0,  0,  0
    };

    return SINDHI_CHAR_START_TABLE[ldbapi4_get_sindhi_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_sindhi_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_SINDHI)
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

    unsigned char t1;
    unsigned char t2;

    t1 = ldbapi4_get_sindhi_char_type(c1);
    t2 = ldbapi4_get_sindhi_char_type(c2);

    return SINDHI_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

static int sindhi_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0000, /* OOV */
                              0x0920, /* CON */
                              0x0910, /* INV */
                              0x0901, /* SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_sindhi_follow_char);
}

const LDB_CHAR* ldbapi4_get_sindhi_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_SINDHI)
    
    const LDB_MT_ENTRY* e;
    const LDB_MT_ENTRY* sindhi_multitap_table_0;
    const LDB_MT_ENTRY* sindhi_multitap_table_1;
    const LDB_MT_ENTRY* sindhi_multitap_table_2;
    const LDB_MT_ENTRY* sindhi_multitap_table_3;
    MT_ENGINE*                  eng;
    int                         i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    
    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 4)
        return 0;                

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;
    
    sindhi_multitap_table_0 = eng->tables[0].entries;
    sindhi_multitap_table_1 = eng->tables[1].entries;
    sindhi_multitap_table_2 = eng->tables[2].entries;
    sindhi_multitap_table_3 = eng->tables[3].entries;

    
    switch (ldbapi4_get_sindhi_char_type(prev_char))
    {
        default:
        case H_OOV:
            e = &(sindhi_multitap_table_0[i]);
            break;

        case H_CON:
            e = &(sindhi_multitap_table_1[i]);
            break;

        case H_INV:
        case H_DEV:
            e = &(sindhi_multitap_table_2[i]);
            break;

        case H_VIR:
        case H_SIG:
            e = &(sindhi_multitap_table_3[i]);
            break;
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Bengali functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define B_OOV  LDB_BENGALI_OOV
#define B_CON  LDB_BENGALI_CON
#define B_INV  LDB_BENGALI_INV
#define B_DEV  LDB_BENGALI_DEV
#define B_VIR  LDB_BENGALI_VIR
#define B_SIG  LDB_BENGALI_SIG
#define B_NUK  LDB_BENGALI_NUK

unsigned char ldbapi4_get_bengali_char_type(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_BENGALI)
    static const unsigned char BENGALI_CHAR_TYPE_TABLE[0x80] =
    {
                          /* 0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F */
    /* 0x0980 - 0x098F */ B_OOV ,B_SIG ,B_SIG ,B_SIG ,B_OOV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_INV ,B_OOV ,B_OOV ,B_INV,
    /* 0x0990 - 0x099F */ B_INV ,B_OOV ,B_OOV ,B_INV ,B_INV ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON,
    /* 0x09A0 - 0x09AF */ B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_OOV ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON ,B_CON,
    /* 0x09B0 - 0x09BF */ B_CON ,B_OOV ,B_CON ,B_OOV ,B_OOV ,B_OOV ,B_CON ,B_CON ,B_CON ,B_CON ,B_OOV ,B_OOV ,B_NUK ,B_SIG ,B_DEV ,B_DEV,
    /* 0x09C0 - 0x09CF */ B_DEV ,B_DEV ,B_DEV ,B_DEV ,B_OOV ,B_OOV ,B_OOV ,B_DEV ,B_DEV ,B_OOV ,B_OOV ,B_DEV ,B_DEV ,B_VIR ,B_OOV ,B_OOV,
    /* 0x09D0 - 0x09DF */ B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_CON ,B_CON ,B_OOV ,B_CON,
    /* 0x09E0 - 0x09EF */ B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV,
    /* 0x09F0 - 0x09FF */ B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV ,B_OOV
    };

    if (c >= 0x980 && c <= 0x9FF)
        return BENGALI_CHAR_TYPE_TABLE[c - 0x980];
    else
        return LDB_BENGALI_OOV;
#else
    return 0;
#endif
}

int ldbapi4_is_valid_bengali_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_BENGALI)
    static const unsigned char BENGALI_CHAR_START_TABLE[7] =
    {
        /* OOV CON INV DEV VIR SIG NUK*/
            1,  1,  1,  0,  0,  0, 0
    };

    return BENGALI_CHAR_START_TABLE[ldbapi4_get_bengali_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_bengali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
#if defined(KMX_LDB_BENGALI)
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
    
    t1 = ldbapi4_get_bengali_char_type(c1);
    t2 = ldbapi4_get_bengali_char_type(c2);

    return BENGALI_CHAR_FOLLOW_TABLE[t1 * 7 + t2];
#else
    return 0;
#endif
}

static int bengali_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x09A0, /* CON */
                              0x0000, /* OOV */
                              0x0990, /* INV */
                              0x09CD, /* VIR */
                              0x0985, /* 0985 */
                              0x09BC, /* NUKTA */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_bengali_follow_char);
}

const LDB_CHAR* ldbapi4_get_bengali_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_BENGALI)

    const LDB_MT_ENTRY*     e;
    const LDB_MT_ENTRY*     bengali_multitap_table_0;
    const LDB_MT_ENTRY*     bengali_multitap_table_1;
    const LDB_MT_ENTRY*     bengali_multitap_table_2;
    const LDB_MT_ENTRY*     bengali_multitap_table_3;
    const LDB_MT_ENTRY*     bengali_multitap_table_4;
    const LDB_MT_ENTRY*     bengali_multitap_table_5;
    MT_ENGINE*              eng;
    int                     i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_POUND) )
        return 0;           /* Invalid key */

    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 6)
        return 0;            

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;

    bengali_multitap_table_0 = eng->tables[0].entries;
    bengali_multitap_table_1 = eng->tables[1].entries;
    bengali_multitap_table_2 = eng->tables[2].entries;
    bengali_multitap_table_3 = eng->tables[3].entries;
    bengali_multitap_table_4 = eng->tables[4].entries;
    bengali_multitap_table_5 = eng->tables[5].entries;

    if (prev_char == 0x985)
    {
        e = &(bengali_multitap_table_4[i]);
    }
    else
    {
        switch (ldbapi4_get_bengali_char_type(prev_char))
        {            
            case B_CON:
                e = &(bengali_multitap_table_0[i]);
                break;

            default:
            case B_OOV:
            case B_SIG:
                e = &(bengali_multitap_table_1[i]);
                break;

            case B_INV:
            case B_DEV:
                e = &(bengali_multitap_table_2[i]);
                break;

            case B_VIR:        
                e = &(bengali_multitap_table_3[i]);
                break;

            case B_NUK:
                e = &(bengali_multitap_table_5[i]);
                break;
        }
    }

    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Myanmar functions                                                       */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Dev with lower rank must precede higher rank in the same syllable. */
static unsigned char get_myanmar_dev_rank(
    IN LDB_CHAR             c)
{
    /* Right */
    if ( c == 0x1031 )
        return 0;
    
    /* Up */
    if ( (c >= 0x102D && c <= 0x102E) || (c >= 0x1032 && c <= 0x1035) )
        return 1;
    
    /* Down */
    if ( c >= 0x102F && c <= 0x1030 )
        return 2;
    
    /* Left */
    if ( c >= 0x102B && c <= 0x102C )
        return 3;
    
    /* Actually, this is not a Myanmar vowel when arrive here. */
    return 4;
}

unsigned char ldbapi4_get_myanmar_char_type(
    IN LDB_CHAR             c)
{
    if ( (c >= 0x1000 && c <= 0x1021) ||  c == 0x103F )
    {
        return LDB_MYANMAR_CON;
    }

    if ( c >= 0x1022 && c <= 0x102A )
    {
        return LDB_MYANMAR_INV;
    }
    
    if ( c >= 0x102B && c <= 0x1035 )
    {
        return LDB_MYANMAR_DEV;
    }

    if ( c >= 0x1036 && c <= 0x1038 )
    {
        return LDB_MYANMAR_SIG;
    }

    if ( c == 0x1039 )
    {
        return LDB_MYANMAR_VIR;
    }

    if ( c == 0x103A )
    {
        return LDB_MYANMAR_ASAT;
    }
    
    if ( c >= 0x103B && c <= 0x103E)
    {
        return LDB_MYANMAR_DEC;
    }

    if ( c >= 0x104C && c <= 0x104F)
    {
        return LDB_MYANMAR_SPS;
    }
    
    return LDB_MYANMAR_OOV;
}

int ldbapi4_is_valid_myanmar_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
{
    unsigned char t1, t2;

    t1 = ldbapi4_get_myanmar_char_type(c1);
    t2 = ldbapi4_get_myanmar_char_type(c2);

    if (t1 == LDB_MYANMAR_CON || t1 == LDB_MYANMAR_INV || t1 == LDB_MYANMAR_SPS || t1 == LDB_MYANMAR_ASAT)
    {
        return 1;
    }
    
    /* We must make sure other letters can always be inserted. */
    if (t2 == LDB_MYANMAR_OOV)
    {
        return 1;
    }

    if (t1 == LDB_MYANMAR_OOV)
    {
        if (t2 == LDB_MYANMAR_CON || t2 == LDB_MYANMAR_INV || t2 == LDB_MYANMAR_SPS)
        {
            return 1;
        }
        return 0;
    }
    
    if (t1 == LDB_MYANMAR_DEV)
    {
        if (t2 == LDB_MYANMAR_CON || t2 == LDB_MYANMAR_INV || t2 == LDB_MYANMAR_SPS 
            || t2 == LDB_MYANMAR_ASAT || t2 == LDB_MYANMAR_SIG)
        {
            return 1;
        }
        if (t2 == LDB_MYANMAR_DEV)
        {
            return get_myanmar_dev_rank(c1) < get_myanmar_dev_rank(c2);
        }
        return 0;
    }

    if (t1 == LDB_MYANMAR_VIR)
    {
        return t2 == LDB_MYANMAR_CON;
    }
    
    if (t1 == LDB_MYANMAR_SIG)
    {
        if (t2 == LDB_MYANMAR_CON || t2 == LDB_MYANMAR_INV || t2 == LDB_MYANMAR_SPS)
        {
            return 1;
        }
        if (c1 == 0x1036 && t2 == LDB_MYANMAR_ASAT)
        {
            return 1;
        }
        if (t2 == LDB_MYANMAR_SIG)
        {
            return c1 < c2;
        }
        return 0;
    }
    
    if (t1 == LDB_MYANMAR_DEC)
    {
        if (t2 == LDB_MYANMAR_VIR)
        {
            return 0;
        }
        if (t2 == LDB_MYANMAR_DEC)
        {
            return c1 < c2;
        }
        return 1;
    }

    return 0;
}

const LDB_CHAR* ldbapi4_get_myanmar_multitype(
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#ifdef KMX_LDB_MYANMAR
    typedef struct
    {
        unsigned short      count;
        LDB_CHAR            chars[13];
    } MYANMAR_MULTITAP_ENTRY;
    
    static MYANMAR_MULTITAP_ENTRY myanmar_multitap_entry;
#ifdef SPREADTRUM_GFIVE_MYANMAR
    static const MYANMAR_MULTITAP_ENTRY MYANMAR_MULTITAP_TABLE[10] =
    {
        { 10, {
                ' ', '.', 0x104A, 0x104B, 0x104C, 0x104D, 0x104E, 0x104F, 0x1040, '0', '\0'
              } },  /* key 0 */
        {  7, {
                0x1000, 0x1001, 0x1002, 0x1003, 0x1004, 0x1041, '1', '\0'
              } },        /* key 1 */
        {  8, {
                0x1005, 0x1006, 0x1007, 0x1008, 0x1009, 0x100A, 0x1042, '2', '\0'
              } },        /* key 2 */
        {  7, {
                0x100B, 0x100C, 0x100D, 0x100E, 0x100F, 0x1043, '3', '\0'
              } },  /* key 3 */
        {  7, {
                0x1010, 0x1011, 0x1012, 0x1013, 0x1014, 0x1044, '4', '\0'
              } },        /* key 4 */
        {  7, {
                0x1015, 0x1016, 0x1017, 0x1018, 0x1019, 0x1045, '5', '\0'
              } },        /* key 5 */
        {  7, {
                0x101A, 0x101B, 0x101C, 0x101D, 0x101E, 0x1046,  '6', '\0'
              } },       /* key 6 */
        {  5, {
                0x101F, 0x1020, 0x103F, 0x1047, '7', '\0'
              } }, /* key 7 */
        { 12, {
                0x1021, 0x1022, 0x1023, 0x1024, 0x1025, 0x1026, 0x1027, 0x1028, 0x1029, 0x102A, 0x1048, '8',  '\0'
              } },        /* key 8 */
        {  2, {
                0x1049, '9', '\0'
              } }                       /* key 9 */
    };           
    const MYANMAR_MULTITAP_ENTRY* e;
    int i;
    
    if (!count)
        return 0;

    *count = 0;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    else
    {
        i = key - LDB_KEY_0;
        
        e = &MYANMAR_MULTITAP_TABLE[i];
    }
#else
    static const MYANMAR_MULTITAP_ENTRY MYANMAR_MULTITAP_TABLE[11] =
    {
        {  5, {
                0x0020, 0x1031, 0x103C, 0x1039, 0x1040, 0x0000
              } },
        { 12, {
                0x103A, 0x102D, 0x102E, 0x1032, 0x1036, 0x1041, 0x104A, 0x104B, 0x002E, 0x0028, 0x0029, 0x003F, 0x0000
              } },
        {  6, {
                0x102F, 0x1030, 0x103D, 0x103E, 0x1037, 0x1042, 0x0000
              } },
        {  5, {
                0x102B, 0x102C, 0x103B, 0x1038, 0x1043, 0x0000
              } },
        {  6, {
                0x1000, 0x1001, 0x1002, 0x1003, 0x1004, 0x1044, 0x0000
              } },
        {  7, {
                0x1005, 0x1006, 0x1007, 0x1008, 0x100A, 0x1009, 0x1045, 0x0000
              } },
        {  6, {
                0x100B, 0x100C, 0x100D, 0x100E, 0x100F, 0x1046, 0x0000
              } },
        {  6, {
                0x1010, 0x1011, 0x1012, 0x1013, 0x1014, 0x1047, 0x0000
              } },
        {  6, {
                0x1015, 0x1016, 0x1017, 0x1018, 0x1019, 0x1048, 0x0000
              } },
        {  9, {
                0x1021, 0x101A, 0x101B, 0x101C, 0x101D, 0x101E, 0x101F, 0x1020, 0x1049, 0x0000
              } },
        {  12, {
                0x1023, 0x1024, 0x1025, 0x1026, 0x1027, 0x1029, 0x102A, 0x103F, 0x104C, 0x104D, 0x104E, 0x104F, 0x0000
              } },
    };           
    const MYANMAR_MULTITAP_ENTRY* e;
    int i;
    
    if (!count)
        return 0;

    *count = 0;

    if (key == LDB_KEY_STAR)
        e = &MYANMAR_MULTITAP_TABLE[10];
    else if ( (key < LDB_KEY_0) || (key > LDB_KEY_9) )
        return 0;           /* Invalid key */
    else
    {
        i = key - LDB_KEY_0;
        
        e = &MYANMAR_MULTITAP_TABLE[i];
    }
#endif

    myanmar_multitap_entry.count = 0;
    for (i = 0; i < e->count; i++)
    {
        if (ldbapi4_is_valid_myanmar_follow_char(prev_char, e->chars[i]))
        {
            myanmar_multitap_entry.chars[myanmar_multitap_entry.count] = e->chars[i];
            myanmar_multitap_entry.count++;
        }
    }

    if (myanmar_multitap_entry.count > 0)
    {
        *count = myanmar_multitap_entry.count;
        return myanmar_multitap_entry.chars;
    }    
#endif
    return NULL;
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Tibetan functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define T_OOV  LDB_TIBETAN_OOV
#define T_CON  LDB_TIBETAN_CON
#define T_JOC  LDB_TIBETAN_JOC
#define T_DEV  LDB_TIBETAN_DEV
#define T_VIR  LDB_TIBETAN_VIR
#define T_SIG  LDB_TIBETAN_SIG

unsigned char ldbapi4_get_tibetan_char_type(
    IN LDB_CHAR             c)
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
        return LDB_TIBETAN_OOV;
#else
    return 0;
#endif
}

int ldbapi4_is_valid_tibetan_word_start_char(
    IN LDB_CHAR             c)
{
#if defined(KMX_LDB_TIBETAN)
    static const unsigned char TIBETAN_CHAR_START_TABLE[7] =
    {
        /* OOV CON JOC DEV VIR SIG */
            1,  1,  0,  0,  0,  0
    };

    return TIBETAN_CHAR_START_TABLE[ldbapi4_get_tibetan_char_type(c)];
#else
    return 0;
#endif
}

int ldbapi4_is_valid_tibetan_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2)
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
    
    t1 = ldbapi4_get_tibetan_char_type(c1);
    t2 = ldbapi4_get_tibetan_char_type(c2);

    return TIBETAN_CHAR_FOLLOW_TABLE[t1 * 6 + t2];
#else
    return 0;
#endif
}

LDB_CHAR ldbapi4_tibetan_consonant_to_subjoined(
    IN LDB_CHAR             c)
{
    if (ldbapi4_get_tibetan_char_type(c) == T_CON)
        c += 0x50;
    
    return c;
}    
    
LDB_CHAR ldbapi4_tibetan_subjoined_to_consonant(
    IN LDB_CHAR             c)
{
    if (ldbapi4_get_tibetan_char_type(c) == T_JOC)
        c -= 0x50;
    
    return c;    
}
    
static int tibetan_init_mt_tables(
    IN MT_ENGINE*            eng)
{
    LDB_CHAR prev_chars[] = { 0x0F40, /* CON */
                              0x0F90, /* JOC */
                              0x0F71, /* DEV, OOV, VIR, SIG */
    };
    
    return init_mt_tables_impl(eng, 
                               prev_chars, 
                               sizeof(prev_chars) / sizeof(prev_chars[0]), 
                               ldbapi4_is_valid_tibetan_follow_char);
}

const LDB_CHAR* ldbapi4_get_tibetan_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count)
{
#if defined(KMX_LDB_TIBETAN)

    const LDB_MT_ENTRY*     e;
    const LDB_MT_ENTRY*     tibetan_multitap_table_0;
    const LDB_MT_ENTRY*     tibetan_multitap_table_1;
    const LDB_MT_ENTRY*     tibetan_multitap_table_2;            
    MT_ENGINE*              eng;
    int                     i;

    if (!count)
        return 0;

    *count = 0;
    eng = (MT_ENGINE*) engobj;

    if ( (key < LDB_KEY_0) || (key > LDB_KEY_POUND) )
        return 0;           /* Invalid key */

    if (!eng)
        return 0;

    if (eng->signature != LDB_MULTITAP_ENGINE_SIGNATURE)
        return 0;

    if (eng->table_count != 3)
        return 0;            

    i = key - LDB_KEY_0;

    if (i >= eng->key_count)
        return 0;

    tibetan_multitap_table_0 = eng->tables[0].entries;
    tibetan_multitap_table_1 = eng->tables[1].entries;
    tibetan_multitap_table_2 = eng->tables[2].entries;

    switch (ldbapi4_get_tibetan_char_type(prev_char))
    {            
        case T_CON:
            e = &(tibetan_multitap_table_0[i]);
            break;
        
        case T_JOC:
            e = &(tibetan_multitap_table_1[i]);
            break;
        
        default:
        case T_OOV:
        case T_SIG:
        case T_DEV:
        case T_VIR:
            e = &(tibetan_multitap_table_2[i]);
            break;
    }


    if (e->count > 0)
    {
        *count = e->count;
        return e->chars;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* User data functions                                                     */
/*                                                                         */
/*-------------------------------------------------------------------------*/

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

    return LDB_TRUE;
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

    return LDB_TRUE;
}

static void udb_update(
    IN UDB*                 udb)
{
    LDB_ASSERT(udb);

    LDB_ASSERT(udb_is_his_valid(udb));
    LDB_ASSERT(udb_is_oov_valid(udb));

    udb->check = crc((const char*)udb + USIZE, udb->size - USIZE);
}

static int udb_is_valid(
    IN const ENGINE*        eng,
    IN const UDB*           udb,
    IN int                  size)
{
    int                     s;

    LDB_ASSERT(is_aligned(USIZE, 4));

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
    LDB_ASSERT(is_aligned(s, 2));

    VALIDATE(udb->hpos == USIZE);
    VALIDATE(udb->hsize == s);
    VALIDATE(udb_is_his_valid(udb));

    /* Validate OOV object position, size and content */

    VALIDATE(udb->opos == udb->hpos + s);
    VALIDATE(udb->osize == s);
    VALIDATE(udb_is_oov_valid(udb));

    return LDB_TRUE;
}

static void udb_init(
    IN const ENGINE*        eng,
    IN UDB*                 udb,
    IN int                  size)
{
    int                     t;

    LDB_ASSERT(size >= UDB_MIN);

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
    LDB_ASSERT(sizeof(int) == 4);
    LDB_ASSERT(sizeof(LDB_WORD) == 2);
    LDB_ASSERT(sizeof(LDB_CHAR) == 2);
    LDB_ASSERT(sizeof(LDB_WID) == 2);

    LDB_ASSERT(eng);
    LDB_ASSERT(udb);

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

    LDB_ASSERT(len > 0);
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
        LDB_ASSERT(t + n <= udb->olast);
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
        LDB_ASSERT( (t > 0) && (t <= udb->olast) );
        memmove(b, b + t, udb->olast + OSIZE - t);
        memset(b + (udb->olast + OSIZE - t), 0, t);
        p = (UDB_OOV*)b;
        p->prev = 0;
        udb->olast -= t;
    }

    /* Append new item at the end */

    LDB_ASSERT(n <= udb->osize - udb->olast - OSIZE);

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

    LDB_ASSERT(eng);
    udb = eng->udb;
    if (!udb)
        return;             /* User data not available */

    b = (LDB_BYTE*)udb + udb->hpos;

    /* Calc space for the new item */

    n = HSIZE;
    if (wid == udb->wid_sos)
    {
        LDB_ASSERT(str == 0);
        LDB_ASSERT(len == 0);
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
        LDB_ASSERT(str);
        LDB_ASSERT(len > 0);
        n += IIMEIZE * len;
    }
    else
    {
        LDB_ASSERT(str == 0);
        LDB_ASSERT(len > 0);
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
        LDB_ASSERT( (t > 0) && (t <= udb->hlast) );
        memmove(b, b + t, udb->hlast + HSIZE - t);
        memset(b + udb->hlast + HSIZE - t, 0, t);
        p = (UDB_HIS*)b;
        p->prev = 0;
        udb->hlast -= t;
    }

    /* Append new item at the end */

    LDB_ASSERT(n <= udb->hsize - udb->hlast - HSIZE);

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

    LDB_ASSERT(eng);
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

    LDB_ASSERT(eng);
    udb = eng->udb;
    LDB_ASSERT(udb);
    
    it--;
    LDB_ASSERT( (it >= 0) && (it < udb->hlast) );
    p = (const UDB_HIS*)((const LDB_BYTE*)udb + (udb->hpos + it));

    if (p->prev)
    {
        LDB_ASSERT(it >= p->prev);
        return it - p->prev + 1;
    }
    else
    {
        LDB_ASSERT(it == 0);
        return 0;
    }
}

static LDB_WID udb_his_word(
    IN const ENGINE*        eng,
    IN UDB_HIT              it)
{
    const UDB*              udb;
    const UDB_HIS*          p;

    LDB_ASSERT(eng);
    udb = eng->udb;
    LDB_ASSERT(udb);
    
    it--;
    LDB_ASSERT( (it >= 0) && (it < udb->hlast) );
    p = (const UDB_HIS*)((const LDB_BYTE*)udb + (udb->hpos + it));

    return p->wid;
}

static int udb_his_len(
    IN const ENGINE*        eng,
    IN UDB_HIT              it)
{
    const UDB*              udb;
    const UDB_HIS*          p;

    LDB_ASSERT(eng);
    udb = eng->udb;
    LDB_ASSERT(udb);
    
    it--;
    LDB_ASSERT( (it >= 0) && (it < udb->hlast) );
    p = (const UDB_HIS*)((const LDB_BYTE*)udb + (udb->hpos + it));

    return p->len;
}

static const LDB_CHAR* udb_his_str(
    IN const ENGINE*        eng,
    IN UDB_HIT              it)
{
    const UDB*              udb;

    LDB_ASSERT(eng);
    udb = eng->udb;
    LDB_ASSERT(udb);
    
    it--;
    LDB_ASSERT( (it >= 0) && (it < udb->hlast) );

    return (const LDB_CHAR*)((const LDB_BYTE*)udb + (udb->hpos + it + HSIZE));
}

static UDB_OIT udb_oov_first(
    IN const ENGINE*        eng)
{
    const UDB*              udb;
    const UDB_OOV*          p;

    LDB_ASSERT(eng);
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

    LDB_ASSERT(eng);
    udb = eng->udb;
    LDB_ASSERT(udb);

    it--;    
    LDB_ASSERT( (it >= 0) && (it < udb->olast) );
    p = (const UDB_OOV*)((const LDB_BYTE*)udb + (udb->opos + it));

    if (p->prev)
    {
        LDB_ASSERT(it >= p->prev);
        return it - p->prev + 1;
    }
    else
    {
        LDB_ASSERT(it == 0);
        return 0;
    }
}

static int udb_oov_len(
    IN const ENGINE*        eng,
    IN UDB_OIT              it)
{
    const UDB*              udb;
    const UDB_OOV*          p;

    LDB_ASSERT(eng);
    udb = eng->udb;
    LDB_ASSERT(udb);
    
    it--;    
    LDB_ASSERT( (it >= 0) && (it < udb->olast) );
    p = (const UDB_OOV*)((const LDB_BYTE*)udb + (udb->opos + it));

    LDB_ASSERT(p->next > OSIZE);
    return (p->next - OSIZE) / IIMEIZE;
}

static const LDB_CHAR* udb_oov_str(
    IN const ENGINE*        eng,
    IN UDB_OIT              it)
{
    const UDB*              udb;

    LDB_ASSERT(eng);
    udb = eng->udb;
    LDB_ASSERT(udb);
    
    it--;    
    LDB_ASSERT( (it >= 0) && (it < udb->olast) );

    return (const LDB_CHAR*)((const LDB_BYTE*)udb + (udb->opos + it + OSIZE));
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Local functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static LDB_CHAR upper_case(
    IN const BASE_ENGINE*   eng,
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
    IN const BASE_ENGINE*   eng,
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

static void string_convert_case(
    IN const BASE_ENGINE*   eng,
    IN OUT LDB_CHAR*        string,
    IN int                  length,
    IN int                  flags)
{
    flags &= LDB_LOWER_CASE | LDB_UPPER_CASE | LDB_INITIAL_CASE;

    if (flags == LDB_LOWER_CASE)
    {
        while (length > 0)
        {
            LDB_ASSERT(string);
            *string = lower_case(eng, *string);
            string++;
            length--;
        }
    }
    else if (flags == LDB_UPPER_CASE)
    {
        while (length > 0)
        {
            LDB_ASSERT(string);
            *string = upper_case(eng, *string);
            string++;
            length--;
        }
    }
    else if (flags == LDB_INITIAL_CASE)
    {
        if (length > 0)
        {
            LDB_ASSERT(string);
            *string = upper_case(eng, *string);
            string++;
            length--;
        }
        while (length > 0)
        {
            LDB_ASSERT(string);
            *string = lower_case(eng, *string);
            string++;
            length--;
        }
    }
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Database validation functions                                           */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#ifdef LDB_DEBUG

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

    return LDB_TRUE;
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

    return LDB_TRUE;
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

    return LDB_TRUE;
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

    return LDB_TRUE;
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

    return LDB_TRUE;
}

static int ldb_is_word_count_valid(
    IN const LDB*           ldb)
{
    switch (ldb->word_id_bits)
    {
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
            return LDB_FALSE;   /* Invalid word id bits */
    }

    return LDB_TRUE;
}

static int ldb_is_word_table_valid(
    IN const LDB*           ldb)
{
    VALIDATE(ldb->word_table);
    VALIDATE( (ldb->word_level_bits >= 4) && (ldb->word_level_bits <= 8) );

    VALIDATE(ldb->string_table);

    switch (ldb->word_index_bits)
    {
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
            return LDB_FALSE;   /* Invalid word index bits */
    }

    return LDB_TRUE;
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
            return LDB_FALSE;   /* Invalid char code bits */
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

    return LDB_TRUE;
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
                return LDB_FALSE;   /* Invalid bigram index bits */
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

    return LDB_TRUE;
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

    return LDB_TRUE;
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

    return LDB_TRUE;
}

static int ldb_is_valid(
    IN const LDB*           ldb)
{
    VALIDATE(ldb);

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

    return LDB_TRUE;
}
#endif

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Local functions                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static int is_sos_delimiter(
    IN const BASE_ENGINE*   eng,
    IN LDB_CHAR             c)
{
    return (sorted_strchr(eng->sos_table, eng->sos_table_length, c) >= 0) ? LDB_TRUE : LDB_FALSE;
}

static int is_sou_delimiter(
    IN const BASE_ENGINE*   eng,
    IN LDB_CHAR             c)
{
    return (sorted_strchr(eng->sou_table, eng->sou_table_length, c) >= 0) ? LDB_TRUE : LDB_FALSE;
}

static int search_exgram(
    IN ENGINE*              eng,
    IN LDB_WID              wid1,
    IN LDB_WID              wid2)
{
    int                     s;
    int                     e;
    int                     i;
    int                     k;
    LDB_WID                 w;

    if (eng->exgram)
    {
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
                return LDB_FALSE;
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
                return LDB_TRUE;
        }
    }

    return LDB_FALSE;
}

static int search_bigram(
    IN ENGINE*              eng,
    IN LDB_WID              wid1,
    IN LDB_WID              wid2)
{
    int                     s;
    int                     e;
    int                     i;
    int                     k;
    LDB_WID                 w;

    if (eng->bigram)
    {
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
                return LDB_FALSE;
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
                return LDB_TRUE;
        }
    }

    return LDB_FALSE;
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
    int                     s;
    int                     e;
    int                     i;
    int                     k;
    LDB_WID                 w;
    int                     c;

    if (eng->trigram)
    {
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
                return LDB_FALSE;
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
                return LDB_TRUE;
        }
    }

    return LDB_FALSE;
}

/*
需要理清楚以下关系:
word_table

以下五个数组
word_index
word_table

char_table
ckey_table

string_table

按键--->word_index---->得到偏移量
	---->word_table: word code index --->得到word code?	exe:get_word_code_index
	----->string_table:word code	exe:get_word_code

*/



//ZHMCH	根据偏移量wid获取一个word code
//eng->word_table	:	word数组
//(eng->word_level_bits + eng->word_index_bits):	word数组的一个元素的长度
//wid * (eng->word_level_bits + eng->word_index_bits) + eng->word_level_bits:	wid个偏移量之后，
	//除去word_level_bits(此English输入法为5)位数之后的一个word元素
static int get_word_code_index(
    IN ENGINE*              eng,
    IN LDB_WID              wid)
{
    LDB_ASSERT(wid < eng->word_count);

    return read_bits(
        eng->word_table,
        wid * (eng->word_level_bits + eng->word_index_bits) + eng->word_level_bits,
        eng->word_index_bits);
}

//从word_table数组中获取一个wrod的word_level_bits:这个数值表示的是?
static int get_word_level(
    IN ENGINE*              eng,
    IN LDB_WID              wid)
{
    LDB_ASSERT(wid < eng->word_count);

    return read_bits(
        eng->word_table,
        wid * (eng->word_level_bits + eng->word_index_bits),
        eng->word_level_bits);
}

//根据word_code_index，从string_table数组获取word code
//那么，其实，string_table才是真正存储最终使用的单词?
static int get_word_code(
    IN ENGINE*              eng,
    IN int                  index)
{
    LDB_ASSERT(index >= 0);
    LDB_ASSERT(index < eng->string_table_length);

    return read_bits(
        eng->string_table,
        index * eng->char_code_bits,
        eng->char_code_bits);
}

//每一个word code index，都只是
static void get_word_string(
    IN ENGINE*              eng,
    IN LDB_WID              wid,
    OUT LDB_CHAR*           word_chr_string,
    OUT LDB_BYTE*           word_key_string,
    IN int                  length)
{
    int                     c;
    int                     bits;
    int                     bit_pos;

    bits = eng->char_code_bits;//每一个char code的bits
    bit_pos = get_word_code_index(eng, wid) * bits;//由此可见，string_table里面保存的是所有的string
    				//然后，word_table数组，只是用来表示每一个string的索引
    				//

	//连续的从string_table数组中读取长度为length的字母编码，
	//并从char_table中获取到对应的字母
	//然后组装成一个string
    while (length > 0)
    {
    	//string_table:每一个string，存放的时候，都只是存放对应每一个字母的编码
    	//word_table数组中，保存的只是代表的string存放在string_table数组中的索引
    	//length:传进来的这个参数---是依附在哪个数组之上，怎么得来?
        c = read_bits(eng->string_table, bit_pos, bits);
		//以下两行:	
        *word_chr_string++ = eng->char_table[c];//char_table:	单字母，根据系列号返回一个表示字母编码
        *word_key_string++ = eng->ckey_table[c];//意义?
        //继续偏移，获取下一个字母
        bit_pos += bits;
        length--;
    }
}

//寻找词典:wid偏移量--这个函数的作用是?
//string--->char arr--->(char_table)index arr--->
//功能: 字符串---每一个字符所在table的inde数组----逆操作得到显示此字符串需要的按键对应的偏移量
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

    LDB_ASSERT(string);
    LDB_ASSERT(length > 0);

	//eng->wid_oov:	表示长度越界--只保存长度不大于eng->max_word_length的strign
    if (length > eng->max_word_length)
        return eng->wid_oov;

	//由string每一个字母在str_table表中的index组成的code数组
    for (i = 0; i < length; i++)
    {	//获得一个字母在str_table中的index
        c = sorted_strchr(eng->char_table, eng->char_code_count, lower_case((BASE_ENGINE*)eng, string[i]));
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
    {	//c此处是偏移量:---编码布局--跟word_index一致!
        c = eng->key_count
          + (length - 2) * eng->key_count * eng->key_count
          + eng->ckey_table[code[0]] * eng->key_count
          + eng->ckey_table[code[1]];
		//可知，eng->ckey_table[code[0]]----是键值
		//逆操作得到显示此字符串需要的按键对应的偏移量
		//可知:ckey_table的作用:-----char-key(按键)对应表:功能:---根据char所在str_table的index
			//得到对应字符串的按键系列--对应的偏移量
    }
	//获得该词对应的编码期间(word_index的偏移量)---why?---只有一个匹配该按键系列的词?
    wid = (LDB_WID) read_bits(eng->word_index, c * eng->word_id_bits, eng->word_id_bits);
    end = (LDB_WID) read_bits(eng->word_index, (c + 1) * eng->word_id_bits, eng->word_id_bits);

/*
总结:
string
string的每一个字符在char_table中的数组index组成的index数组
计算偏移量:
		c = eng->key_count
		  + (length - 2) * eng->key_count * eng->key_count
		  + eng->ckey_table[code[0]] * eng->key_count
		  + eng->ckey_table[code[1]];
		其中，ckey_table是字符串转换为按键的表
	计算偏移量所需参数:
		按键的数量
		按键的次数(或者逆推的，字符串的长度)
		前两次按键的键值

获得按键系列对应的一组词的坐标期间的流程:
	计算按键系列对应的一组词的偏移坐标(word_index中保存连续的一系列的词组偏移量)
	以word_index的一个元素(做了以上偏移之后)的值作为word_table的偏移量，word_table的下一个元素作为结束地址
	从string_table获得具体的一个词的起始和结束地址
	word_table:	一个词的坐标期间(起始偏移地址和结束偏移地址)


逆向创建ldb的过程推导:
	键盘配置:ckey_table	按键和key对应关系
	按键系列与单词的键值对原始数据
	保存一个按键组合:
		逐个对每一个词的每一个字母进行编码并保存到string_table数组中去!(每一个字母都需要进行编码，十六进制?,看应用进行逆推)
		开始保存第一个词时，记下该偏移量(string_table)，每保存一个词，都把偏移量保存到word_table(保存的是词的起始和结束偏移量)
		本系列按键对应的词保存完成之后，把string_table的下一个地址到word_index中，作为下一系列的起始地址
	重复以上的操作，保存所有的键值对

	word_index表是---按键系列和词组的关系映射表
		需要遵守预设的映射树以满足以下的对应关系:
	        c = eng->key_count
	          + (length - 2) * eng->key_count * eng->key_count
	          + eng->ckey_table[code[0]] * eng->key_count
	          + eng->ckey_table[code[1]];

*/



	//wid,end:说明；
    while (wid < end)
    {
    	//根据偏移量---从word_table中得到词在string_table中的起始地址
        c = get_word_code_index(eng, wid);//操作word_table
        for (i = 0; i < length; i++)
        {
        	//逐个单词比较该词长度个元素，判断string_table是否存在
            if (code[i] != get_word_code(eng, c + i))//string_table
                break;
        }
		//如果吻合，就是存在!
        if (i == length)
            return wid;
        wid++;
    }

    return eng->wid_oov;
}

//
static void input_string_to_key_string(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      str,
    IN int                  len,
    OUT LDB_CHAR*           key)
{
    int                     i;
    LDB_CHAR                c;

    while (len > 0)
    {
        if (eng->is_sure_type && IS_VALID_SURE_TYPE_INPUT(*str))
        {
            *key = *str;
        }
        else if (!eng->is_sure_type && IS_VALID_9KEY_INPUT(*str))
        {
            *key = *str;
        }
        else if ( (*str >= LDB_CHAR_DIGIT_0) && (*str <= LDB_CHAR_DIGIT_9) )
        {
            *key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_DIGIT_0));
        }
        else if ( (*str >= LDB_CHAR_ARABIC_DIGIT_0) && (*str <= LDB_CHAR_ARABIC_DIGIT_9) )
        {
            *key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_ARABIC_DIGIT_0));
        }
        else if ( (*str >= LDB_CHAR_PERSIAN_DIGIT_0) && (*str <= LDB_CHAR_PERSIAN_DIGIT_9) )
        {
            *key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_PERSIAN_DIGIT_0));
        }
        else if ( (*str >= LDB_CHAR_HINDI_DIGIT_0) && (*str <= LDB_CHAR_HINDI_DIGIT_9) )
        {
            *key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_HINDI_DIGIT_0));
        }
    	else if ( (*str >= LDB_CHAR_NEPALI_DIGIT_0) && (*str <= LDB_CHAR_NEPALI_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_NEPALI_DIGIT_0));
    	}
    	else if ( (*str >= LDB_CHAR_SINHALA_DIGIT_0) && (*str <= LDB_CHAR_SINHALA_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_SINHALA_DIGIT_0));
    	}
		//daodejing
    	else if ( (*str >= LDB_CHAR_BODO_DIGIT_0) && (*str <= LDB_CHAR_BODO_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_BODO_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_DOGRI_DIGIT_0) && (*str <= LDB_CHAR_DOGRI_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_DOGRI_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_SANSKRIT_DIGIT_0) && (*str <= LDB_CHAR_SANSKRIT_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_SANSKRIT_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_KASHMIRI_DIGIT_0) && (*str <= LDB_CHAR_KASHMIRI_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_KASHMIRI_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_KONKANI_DIGIT_0) && (*str <= LDB_CHAR_KONKANI_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_KONKANI_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_MAITHILI_DIGIT_0) && (*str <= LDB_CHAR_MAITHILI_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_MAITHILI_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_MANIPURI_DIGIT_0) && (*str <= LDB_CHAR_MANIPURI_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_MANIPURI_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_SANTALI_DIGIT_0) && (*str <= LDB_CHAR_SANTALI_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_SANTALI_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_SOMALI_DIGIT_0) && (*str <= LDB_CHAR_SOMALI_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_SOMALI_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_SINDHI_DIGIT_0) && (*str <= LDB_CHAR_SINDHI_DIGIT_9) )
    	{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_SINDHI_DIGIT_0));
    	}
		else if ( (*str >= LDB_CHAR_BENGALI_DIGIT_0) && (*str <= LDB_CHAR_BENGALI_DIGIT_9) )
		{
			*key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_BENGALI_DIGIT_0));
		}
        else if ( (*str >= LDB_CHAR_THAI_DIGIT_0) && (*str <= LDB_CHAR_THAI_DIGIT_9) )
        {
            *key = (LDB_CHAR) (LDB_KEY_0 + (*str - LDB_CHAR_THAI_DIGIT_0));
        }
        else
        {
            c = lower_case((BASE_ENGINE*)eng, *str);
            i = sorted_strchr(eng->char_table, eng->char_code_count, *str);
            if (i >= 0)
            {
                *key = (LDB_CHAR) (LDB_KEY_0 + eng->ckey_table[i]);
            }
            else if ( (c >= 0x61) && (c <= 0x7A) )
            {
                if (eng->is_sure_type)
                    *key = (LDB_CHAR) (LDB_KEY_0 + BASIC_LATIN_SURE_TYPE_MAP[c - 0x61]);
                else
                    *key = (LDB_CHAR) (LDB_KEY_0 + BASIC_LATIN_9KEY_MAP[c - 0x61]);
            }
            else
            {
                *key = c;
            }
        }
        str++;
        key++;
        len--;
    }
}

static LDB_BYTE chr_to_key(
    IN ENGINE*              eng,
    IN LDB_CHAR             c)
{
    int                     i;

    if (eng->is_sure_type && IS_VALID_SURE_TYPE_INPUT(c))
    {
        return (LDB_BYTE) (c - LDB_KEY_0);
    }
    else if (!eng->is_sure_type && IS_VALID_9KEY_INPUT(c))
    {
        return (LDB_BYTE) (c - LDB_KEY_0);
    }
    else if ( (c >= LDB_CHAR_DIGIT_0) && (c <= LDB_CHAR_DIGIT_9) )
    {
        return (LDB_BYTE) (c - LDB_CHAR_DIGIT_0);
    }
    else if ( (c >= LDB_CHAR_ARABIC_DIGIT_0) && (c <= LDB_CHAR_ARABIC_DIGIT_9) )
    {
        return (LDB_BYTE) (c - LDB_CHAR_ARABIC_DIGIT_0);
    }
    else if ( (c >= LDB_CHAR_PERSIAN_DIGIT_0) && (c <= LDB_CHAR_PERSIAN_DIGIT_9) )
    {
        return (LDB_BYTE) (c - LDB_CHAR_PERSIAN_DIGIT_0);
    }
    else if ( (c >= LDB_CHAR_HINDI_DIGIT_0) && (c <= LDB_CHAR_HINDI_DIGIT_9) )
    {
        return (LDB_BYTE) (c - LDB_CHAR_HINDI_DIGIT_0);
    }
	else if ( (c >= LDB_CHAR_NEPALI_DIGIT_0) && (c <= LDB_CHAR_NEPALI_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_NEPALI_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_SINHALA_DIGIT_0) && (c <= LDB_CHAR_SINHALA_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_SINHALA_DIGIT_0);
   	}
	//daodejing
	else if ( (c >= LDB_CHAR_BODO_DIGIT_0) && (c <= LDB_CHAR_BODO_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_BODO_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_DOGRI_DIGIT_0) && (c <= LDB_CHAR_DOGRI_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_DOGRI_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_SANSKRIT_DIGIT_0) && (c <= LDB_CHAR_SANSKRIT_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_SANSKRIT_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_KASHMIRI_DIGIT_0) && (c <= LDB_CHAR_KASHMIRI_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_KASHMIRI_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_KONKANI_DIGIT_0) && (c <= LDB_CHAR_KONKANI_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_KONKANI_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_MAITHILI_DIGIT_0) && (c <= LDB_CHAR_MAITHILI_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_MAITHILI_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_MANIPURI_DIGIT_0) && (c <= LDB_CHAR_MANIPURI_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_MANIPURI_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_SANTALI_DIGIT_0) && (c <= LDB_CHAR_SANTALI_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_SANTALI_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_SOMALI_DIGIT_0) && (c <= LDB_CHAR_SOMALI_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_SOMALI_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_SINDHI_DIGIT_0) && (c <= LDB_CHAR_SINDHI_DIGIT_9) )
	{
       	return (LDB_BYTE) (c - LDB_CHAR_SINDHI_DIGIT_0);
   	}
	else if ( (c >= LDB_CHAR_BENGALI_DIGIT_0) && (c <= LDB_CHAR_BENGALI_DIGIT_9) )
    {
        return (LDB_BYTE) (c - LDB_CHAR_BENGALI_DIGIT_0);
    }
    else if ( (c >= LDB_CHAR_THAI_DIGIT_0) && (c <= LDB_CHAR_THAI_DIGIT_9) )
    {
        return (LDB_BYTE) (c - LDB_CHAR_THAI_DIGIT_0);
    }
    else if ((eng->flags & LDB_USE_KEY_STAR) && c == '*')
    {
        return LDB_KEY_STAR - LDB_KEY_0;
    }
    else if ((eng->flags & LDB_USE_KEY_POUND) && c == '#')
    {
        return LDB_KEY_POUND - LDB_KEY_0;
    }    
    else
    {
        i = sorted_strchr(eng->char_table, eng->char_code_count, c);
        if (i >= 0)
            return eng->ckey_table[i];

        if (eng->is_sure_type)
        {
            if (c >= 0x61 && c <= 0x7A)
                return BASIC_LATIN_SURE_TYPE_MAP[c - 0x61];
            return 15;
        }
        else
        {
            if (c >= 0x61 && c <= 0x7A)
                return BASIC_LATIN_9KEY_MAP[c - 0x61];
                
            if ( c == 0x20 )
                return 0;
                
            if ((eng->flags & LDB_USE_KEY_STAR))
            {
                LDB_WORD i;
                for (i = eng->mt_index[10]; i < eng->mt_index[11]; i++)
                {
                    if (eng->mt_string[i] == c)
                        return LDB_KEY_STAR - LDB_KEY_0;
                }                            
            }

            if ((eng->flags & LDB_USE_KEY_POUND))
            {
                LDB_WORD i;
                for (i = eng->mt_index[11]; i < eng->mt_index[12]; i++)
                {
                    if (eng->mt_string[i] == c)
                        return LDB_KEY_POUND - LDB_KEY_0;
                }                            
            }
                
            return 1;
        }
    }
}

static int match_string(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      input_string,
    IN int                  input_length,
    IN const LDB_CHAR*      word_chr_string,
    IN const LDB_BYTE*      word_key_string,
    IN int                  word_length)
{
    int                     c;

    c = 0;
    while ( (input_length > 0) && (word_length > 0) )
    {
        if (eng->is_sure_type && IS_VALID_SURE_TYPE_INPUT(*input_string))
        {
            if (*input_string - LDB_KEY_0 != *word_key_string)
                break;
        }
        else if (!eng->is_sure_type && IS_VALID_9KEY_INPUT(*input_string))
        {
            if (*input_string - LDB_KEY_0 != *word_key_string)
                break;
        }
        else
        {
            if (*input_string != *word_chr_string)
                break;
        }
        input_string++;
        input_length--;
        word_chr_string++;
        word_key_string++;
        word_length--;
        c++;
    }

    return c;
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

    LDB_ASSERT(eng->match_count <= eng->max_match);
    
    #if !defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
    if (eng->language == IIME_LANG_HINDI && len == 1)
    {
        if (!ldbapi4_is_valid_hindi_word_start_char(str[0]))
            return;
    }
    if (eng->language == IIME_LANG_NEPALI && len == 1)
    {
        if (!ldbapi4_is_valid_nepali_word_start_char(str[0]))
            return;
    }
    if (eng->language == IIME_LANG_SINHALA && len == 1)
    {
        if (!ldbapi4_is_valid_sinhala_word_start_char(str[0]))
            return;
    }
	//daodejing
    if (eng->language == IIME_LANG_BODO && len == 1)
    {
        if (!ldbapi4_is_valid_bodo_word_start_char(str[0]))
            return;
    }
	if (eng->language == IIME_LANG_DOGRI && len == 1)
    {
        if (!ldbapi4_is_valid_dogri_word_start_char(str[0]))
            return;
    }
	if (eng->language == IIME_LANG_SANSKRIT && len == 1)
    {
        if (!ldbapi4_is_valid_sanskrit_word_start_char(str[0]))
            return;
    }
	if (eng->language == IIME_LANG_KASHMIRI && len == 1)
    {
        if (!ldbapi4_is_valid_kashmiri_word_start_char(str[0]))
            return;
    }
	if (eng->language == IIME_LANG_KONKANI && len == 1)
    {
        if (!ldbapi4_is_valid_konkani_word_start_char(str[0]))
            return;
    }
	if (eng->language == IIME_LANG_MAITHILI && len == 1)
    {
        if (!ldbapi4_is_valid_maithili_word_start_char(str[0]))
            return;
    }
	if (eng->language == IIME_LANG_MANIPURI && len == 1)
    {
        if (!ldbapi4_is_valid_manipuri_word_start_char(str[0]))
            return;
    }
	if (eng->language == IIME_LANG_SANTALI && len == 1)
    {
        if (!ldbapi4_is_valid_santali_word_start_char(str[0]))
            return;
    }
	if (eng->language == IIME_LANG_SOMALI && len == 1)
    {
        if (!ldbapi4_is_valid_somali_word_start_char(str[0]))
            return;
    }
	if (eng->language == IIME_LANG_SINDHI && len == 1)
    {
        if (!ldbapi4_is_valid_sindhi_word_start_char(str[0]))
            return;
    }
    if (eng->language == IIME_LANG_THAI && len == 1)
    {
        if (!ldbapi4_is_valid_thai_word_start_char(str[0]))
            return;
    }

    if (eng->language == IIME_LANG_BENGALI && len == 1)
    {
        if (!ldbapi4_is_valid_bengali_word_start_char(str[0]))
            return;
    }
    #endif
    
    if (eng->match_count == eng->max_match)
        return;             /* Match list is full */

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
        LDB_ASSERT( (s >= 0) && (s <= eng->match_count) );
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
    p->string[len] = 0;

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

static void match_orginal(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      str,
    IN int                  len,
    IN int                  flags)
{
    LDB_CHAR                s[MAX_WORD_LENGTH];

    memcpy(s, str, sizeof(LDB_CHAR) * len);
    string_convert_case((BASE_ENGINE*)eng, s, len, flags);
    add_match(eng, RANK_TOP, s, len);
}

static void match_number(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      str,
    IN int                  len,
    IN int                  flags)
{
    LDB_CHAR                c;
    int                     i;
    LDB_CHAR                s[MAX_WORD_LENGTH];

    if (eng->is_sure_type)
    {
        if (len == 1 && chr_to_key(eng, str[0]) == 15)
        {
            for (i = eng->mt_index[15]; i < eng->mt_index[16]; i++)
                add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
        }
        else
        {
            for (i = 0; i < len; i++)
            {
                c = chr_to_key(eng, str[i]);
                switch (c)
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
                        LDB_ASSERT(c >= 0 && c <= 9);
                        c = (LDB_CHAR) (LDB_CHAR_DIGIT_0 + c);
                        break;
                }
                s[i] = c;
            }
            add_match(eng, RANK_BOTTOM, s, len);
        }
    }
    else
    {
    if (eng->language == IIME_LANG_PERSIAN)
        c = LDB_CHAR_PERSIAN_DIGIT_0;
    else
        c = LDB_CHAR_DIGIT_0;
        
    #ifdef KMX_LDB_HINDI        
        if (eng->language == IIME_LANG_HINDI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_HINDI_DIGIT_0;	
        }        
    #endif

    #ifdef KMX_LDB_NEPALI
        if (eng->language == IIME_LANG_NEPALI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_NEPALI_DIGIT_0;	
        }        
    #endif

	#ifdef KMX_LDB_SINHALA
        if (eng->language == IIME_LANG_SINHALA)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_SINHALA_DIGIT_0;	
        }        
	#endif

	//daodejing need
	#ifdef KMX_LDB_BODO
        if (eng->language == IIME_LANG_BODO)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_BODO_DIGIT_0;	
        }        
	#endif

	#ifdef KMX_LDB_DOGRI
        if (eng->language == IIME_LANG_DOGRI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_DOGRI_DIGIT_0;	
        }        
	#endif

	#ifdef KMX_LDB_SANSKRIT
        if (eng->language == IIME_LANG_SANSKRIT)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_SANSKRIT_DIGIT_0;	
        }        
	#endif

	#ifdef KMX_LDB_KASHMIRI
        if (eng->language == IIME_LANG_KASHMIRI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_KASHMIRI_DIGIT_0;	
        }        
	#endif

	#ifdef KMX_LDB_KONKANI
        if (eng->language == IIME_LANG_KONKANI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_KONKANI_DIGIT_0;	
        }        
	#endif

	#ifdef KMX_LDB_MAITHILI
        if (eng->language == IIME_LANG_MAITHILI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_MAITHILI_DIGIT_0;	
        }        
	#endif

	#ifdef KMX_LDB_MANIPURI
        if (eng->language == IIME_LANG_MANIPURI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_MANIPURI_DIGIT_0;	
        }        
	#endif

	#ifdef KMX_LDB_SANTALI
        if (eng->language == IIME_LANG_SANTALI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_SANTALI_DIGIT_0;	
        }        
	#endif

	#ifdef KMX_LDB_SOMALI
        if (eng->language == IIME_LANG_SOMALI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_SOMALI_DIGIT_0;	
        }        
	#endif

	#ifdef KMX_LDB_SINDHI
        if (eng->language == IIME_LANG_SINDHI)
        {
            if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 904 || eng->did == 9000 || eng->did == 9010 || eng->did == 9015 || eng->did == 9011 || eng->did == 9016 || eng->did == 9013 || eng->did == 9018)
                c = LDB_CHAR_SINDHI_DIGIT_0;	
        }        
	#endif

    #ifdef KMX_LDB_BENGALI
    if (eng->language == IIME_LANG_BENGALI)
        if (eng->did == 924 || eng->did == 922 || eng->did == 929 || eng->did == 9200 || eng->did == 9201 || eng->did == 9202 || eng->did == 9203)
            c = LDB_CHAR_BENGALI_DIGIT_0;	
    #endif

    for (i = 0; i < len; i++)
        {
            if (str[i] == LDB_KEY_STAR)
                s[i] = '*';
            else if (str[i] == LDB_KEY_POUND)
                s[i] = '#';
            else
                s[i] = (LDB_CHAR) (c + chr_to_key(eng, str[i]));
        }

        add_match(eng, RANK_BOTTOM, s, len);

        if ( (len == 1) && (chr_to_key(eng, str[0]) == 1) )
        {
            for (i = eng->mt_index[1]; i < eng->mt_index[2]; i++)
                add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
        }
    
        /* For Thai language, Key 0 may aslo contain some symbol */
        if ( (eng->language == IIME_LANG_THAI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
        {
            for (i = eng->mt_index[0]; i < eng->mt_index[1]; i++)
                add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
        }
        
        /* 366, 369 has symbol on key 10 */
        if ( ((eng->did == 366) || (eng->did == 369)) && (len == 1) && (chr_to_key(eng, str[0]) == 10))
        {
            for (i = eng->mt_index[10]; i < eng->mt_index[11]; i++)
                add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
        }

#ifdef KMX_LDB_HINDI    
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {    
    /* Always place space as the first candidate of the 0 */
    if ( (eng->language == IIME_LANG_HINDI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_NEPALI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_SINHALA) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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

//daodejing
#ifdef KMX_LDB_BODO
    if (eng->did == 902 || eng->did == 909 || eng->did == 903 || eng->did == 9010 || eng->did == 9015 || eng->did == 9013 || eng->did == 9018)
    {    
    /* Always place space as the first candidate of the 0 */
    if ( (eng->language == IIME_LANG_BODO) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_DOGRI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_SANSKRIT) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_KASHMIRI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_KONKANI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_MAITHILI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_MANIPURI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_SANTALI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_SOMALI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_SINDHI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ( (eng->language == IIME_LANG_BENGALI) && (len == 1) && (chr_to_key(eng, str[0]) == 0))
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
    if ((eng->did == 9201 || eng->did == 9202 || eng->did == 9203) && (len == 1) && (chr_to_key(eng, str[0]) == 10))
    {
        for (i = eng->mt_index[10]; i < eng->mt_index[11]; i++)
            add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
    }

    /* 9200 has symbol on key 11 */
    if ((eng->did == 9200) && (len == 1) && (chr_to_key(eng, str[0]) == 11))
    {
        for (i = eng->mt_index[11]; i < eng->mt_index[12]; i++)
            add_match(eng, RANK_LEGRAM + i, eng->mt_string + i, 1);
    }


    
#endif

#ifdef KMX_LDB_THAI
    #ifdef KMX_IME_THAI_KEY0_SPACE_MULTITAP
    if (eng->did == 360 || eng->did == 361 || eng->did == 365 || eng->did == 367)
    {
        /* Append space 0x20 candidate and always place space as the first candidate of the 0 */
        if (eng->language == IIME_LANG_THAI && len == 1 && (chr_to_key(eng, str[0]) == 0))
        {
            add_match(eng, RANK_TOP, L" ", 1);
        }
    }
    #endif
#endif 

    }
}

static void match_cache(
    IN ENGINE*              eng,
    IN LDB_WID              wid1,
    IN LDB_WID              wid2,
    IN const LDB_CHAR*      input_string,
    IN int                  input_length,
    IN int                  flags)
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
    LDB_CHAR                word_chr_string[MAX_WORD_LENGTH];
    LDB_BYTE                word_key_string[MAX_WORD_LENGTH];
    int                     word_length;

    rank_value = 0;

    hit = udb_his_first(eng);
    while (hit)
    {
        wid = udb_his_word(eng, hit);
        word_length = udb_his_len(eng, hit);

        if ( (wid != eng->wid_sos) && (word_length >= input_length) )
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
            is_trigram_matched = LDB_FALSE;
            is_bigram_matched = LDB_FALSE;
            if (hwid2 == wid2)
            {
                if (hwid1 == wid1)
                    is_trigram_matched = LDB_TRUE;
                else
                    is_bigram_matched = LDB_TRUE;
            }

            /* Match cache word against input string */

            if (wid == eng->wid_oov)
            {
                memcpy(word_chr_string, udb_his_str(eng, hit), sizeof(LDB_CHAR) * word_length);
                string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, LDB_LOWER_CASE);
                for (i = 0; i < word_length; i++)
                    word_key_string[i] = chr_to_key(eng, word_chr_string[i]);
                match_length = match_string(
                    eng, input_string, input_length,
                    word_chr_string, word_key_string, word_length);
                if (match_length == input_length)
                {
                    if (match_length == word_length)
                    {
                        if (is_trigram_matched)
                            rank_level = RANK_OOVT;
                        else if (is_bigram_matched)
                            rank_level = RANK_OOVB;
                        else
                            rank_level = RANK_OOVU;
                        string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, flags);
                        add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                    }
                    else /* match_length < word_length */
                    {
                        if ( (input_length >= FORWARD_MATCH) && (eng->language != IIME_LANG_VIETNAMESE) )
                        {
                            if (is_trigram_matched)
                                rank_level = RANK_FOOVT;
                            else if (is_bigram_matched)
                                rank_level = RANK_FOOVB;
                            else
                                rank_level = RANK_FOOVU;
                            string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, flags);
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
                    eng, input_string, input_length,
                    word_chr_string, word_key_string, word_length);
                if (match_length == input_length)
                {
                    if (match_length == word_length)
                    {
                        if (is_trigram_matched)
                            rank_level = RANK_CACT;
                        else if (is_bigram_matched)
                            rank_level = RANK_CACB;
                        else
                            rank_level = RANK_CACU;
                        string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, flags);
                        add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                    }
                    else /* match_length < word_length */
                    {
                        if ( (input_length >= FORWARD_MATCH) && (eng->language != IIME_LANG_VIETNAMESE) )
                        {
                            if (is_trigram_matched)
                                rank_level = RANK_FCACT;
                            else if (is_bigram_matched)
                                rank_level = RANK_FCACB;
                            else
                                rank_level = RANK_FCACU;
                            string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, flags);
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
        if (word_length >= input_length)
        {
            /* Match cache word against input string */

            memcpy(word_chr_string, udb_oov_str(eng, oit), sizeof(LDB_CHAR) * word_length);
            string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, LDB_LOWER_CASE);
            for (i = 0; i < word_length; i++)
                word_key_string[i] = chr_to_key(eng, word_chr_string[i]);
            match_length = match_string(
                eng, input_string, input_length,
                word_chr_string, word_key_string, word_length);
            if (match_length == input_length)
            {
                if (match_length == word_length)
                {
                    rank_level = RANK_OOVU;
                    string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, flags);
                    add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                }
                else /* match_length < word_length */
                {
                    if ( (input_length >= FORWARD_MATCH) && (eng->language != IIME_LANG_VIETNAMESE) )
                    {
                        rank_level = RANK_FOOVU;
                        string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, flags);
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

static int filter_greek_word(
    IN const LDB_CHAR*      string,
    IN int                  length,
    IN int                  flags)
/*
 * Returns LDB_TRUE if given word should be filtered.
 */
{
    if (length == 1 && (flags & LDB_UPPER_CASE))
    {
        if (*string == 0x390 || *string == 0x3B0)
            return LDB_TRUE;
    }
    return LDB_FALSE;
}

/*
编码思路:
明晰按键组合与候选word之间的关系---建立code映射
把所有的word，组合成连续的数据流，导入一个数组当中
	导入的动作:	需要对应每一个字母的编码---需要替换?
	导入数据流时，记录下每一个单词在数组中的位置
		并据此，填充string_table(需要再仔细查看该数组)	
*/
static void match_dictionary(
    IN ENGINE*              eng,
    IN LDB_WID              wid1,
    IN LDB_WID              wid2,
    IN const LDB_CHAR*      input_string,
    IN int                  input_length,
    IN int                  flags)
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
    LDB_CHAR                word_chr_string[MAX_WORD_LENGTH];//typedef unsigned short      LDB_CHAR;
    LDB_BYTE                word_key_string[MAX_WORD_LENGTH];//typedef unsigned char       LDB_BYTE;
    int                     word_length;

    LDB_ASSERT(input_length > 0);
    k0 = chr_to_key(eng, input_string[0]);//ZHMCH:此时得到的是按键的数值:按下"9"--得到0x9
    if (input_length == 1)
        k1 = 0;//如果是首次输入按键---就是--此时传过来的按键次数是1，设置k1为0
        	//那么，k1的意义:已知长度(就是总共输入的按键次数-1)造成的固化的偏移量
        	//未知: 需要在已经固化长度下---计算出可以使用的偏移量--候选者的地址
    else
		//偏移的计算:
			//eng->key_count
			//k0 * eng->key_count:指的是按键的次数?统计?--还是ldb的属性?
			//key_count需要搞清楚意思:
				//根据#define LDB_MT_ENTRY_MAX_NUM        12
				//if (eng->key_count > LDB_MT_ENTRY_MAX_NUM)
				//得到:eng->key_count 就是按键次数的统计!
    	{
    	int k1_arg0=0;
		int k1_arg1=0;
		int k1_arg2=0;
		k1_arg0 = eng->key_count;
		k1_arg1 = k0 * eng->key_count;
		k1_arg2 = chr_to_key(eng, input_string[1]);

        k1 = eng->key_count + k0 * eng->key_count + chr_to_key(eng, input_string[1]);
    	}
		//k1 =
			//eng->key_count	按键次数-----按键组合的偏移?---vc模拟器数值不明!!!
					//eng->key_count这个数值的意义: 跟ldb的index是对应的?
			//+ k0 * eng->key_count 	-------键值* 按键次数-----意义:按键组合的偏移
					//推断:	每增加一个键值，偏移量都是一样的
						//说明:	每一个组合的长度是一样的
			//+ chr_to_key(eng, input_string[1]);-------前一个按键数值---意义?

			//eng->key_count	关键问题是要理解这个变量!!!!!




				
    /* Search candidates with the same length */

    if (input_length <= eng->max_word_length)
    {
        word_length = input_length;
        if (word_length == 1)
            i = k0;
			//如果只有一个按键----偏移量就是按键数值
        else
            i = k1 + (word_length - 2) * eng->key_count * eng->key_count;
			//k1 + (word_length - 2) * eng->key_count * eng->key_count;
				//eng->key_count * eng->key_count
/*
结合
			static LDB_WID search_dictionary(
				IN ENGINE*				eng,
				IN const LDB_CHAR*		string,
				IN int					length)
函数理解:


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
			
	eng->key_count
		---这个应该是---低位的偏移!!!!----赋值之后，应该会作为一种基点来使用
			#define LDB_MT_ENTRY_MAX_NUM        12
				存在的位置?:	bits:4	即用低八位就可以保存起来!!!
	(length - 2) * eng->key_count * eng->key_count
		---是不是一种偏移?
				
			*/





		//ZHMCH
        swid = (LDB_WID) read_bits(eng->word_index, i * eng->word_id_bits, eng->word_id_bits);
        ewid = (LDB_WID) read_bits(eng->word_index, (i + 1) * eng->word_id_bits, eng->word_id_bits);
		//表明:	word_table:两个编码元素之间，一系列的code
		//一系列编码:	每一个编码，都是对应string_table元素，---所以的元素组合起来，就是一个word!
		//所以word_table和string_table之间的关系，自此就明朗了
		//word---> word code range--->string_table	&&  char_table
			//---> word arr!
        for (wid = swid; wid < ewid; wid++)
        {
            if (search_exgram(eng, wid2, wid))
            {
                eng->is_exgram_found = LDB_TRUE;
                continue;
            }
			//(wid = swid; wid < ewid; wid++)
            get_word_string(eng, wid, word_chr_string, word_key_string, word_length);
            if (eng->language == IIME_LANG_GREEK && filter_greek_word(word_chr_string, word_length, flags))
                continue;
            match_length = match_string(
                eng, input_string, input_length,
                word_chr_string, word_key_string, word_length);
            if (match_length == word_length)
            {
                if (search_trigram(eng, wid1, wid2, wid))
                    rank_level = RANK_DICT;
                else if (search_bigram(eng, wid2, wid))
                    rank_level = RANK_DICB;
                else
                    rank_level = RANK_DICU;
                rank_value = get_word_level(eng, wid);
                string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, flags);
                add_match(eng, rank_level + rank_value, word_chr_string, word_length);
            }
            else if (match_length >= eng->tmp_len)
            {
                if (search_trigram(eng, wid1, wid2, wid))
                    rank_level = RANK_TDICT;
                else if (search_bigram(eng, wid2, wid))
                    rank_level = RANK_TDICB;
                else
                    rank_level = RANK_TDICU;
                rank_value = get_word_level(eng, wid);
                add_template(eng, rank_level + rank_value, word_chr_string, match_length);
            }
        }
    }

    /* Search candidates with the same key string prefix */

    if (input_length > 1)
    {
        for (word_length = eng->max_word_length; word_length > input_length; word_length--)
        {
            i = k1 + (word_length - 2) * eng->key_count * eng->key_count;
            swid = (LDB_WID) read_bits(eng->word_index, i * eng->word_id_bits, eng->word_id_bits);
            ewid = (LDB_WID) read_bits(eng->word_index, (i + 1) * eng->word_id_bits, eng->word_id_bits);
            for (wid = swid; wid < ewid; wid++)
            {
                if (search_exgram(eng, wid2, wid))
                    continue;
                get_word_string(eng, wid, word_chr_string, word_key_string, word_length);
                if (eng->language == IIME_LANG_GREEK && filter_greek_word(word_chr_string, word_length, flags))
                    continue;
                match_length = match_string(
                    eng, input_string, input_length,
                    word_chr_string, word_key_string, word_length);
                if (match_length == input_length)
                {
                    if (search_trigram(eng, wid1, wid2, wid))
                        rank_level = RANK_FDICT;
                    else if (search_bigram(eng, wid2, wid))
                        rank_level = RANK_FDICB;
                    else
                        rank_level = RANK_FDICU;
                    rank_value = get_word_level(eng, wid);
                    if ( (input_length >= FORWARD_MATCH) && (eng->language != IIME_LANG_VIETNAMESE) )
                    {
                        string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, flags);
                        add_match(eng, rank_level + rank_value, word_chr_string, word_length);
                    }
                    else if (input_length >= 3)
                    {
                        string_convert_case((BASE_ENGINE*)eng, word_chr_string, word_length, flags);
                        add_match(eng, rank_level + rank_value, word_chr_string, match_length);
                    }
                }
                else if (match_length >= eng->tmp_len)
                {
                    if (search_trigram(eng, wid1, wid2, wid))
                        rank_level = RANK_TDICT;
                    else if (search_bigram(eng, wid2, wid))
                        rank_level = RANK_TDICB;
                    else
                        rank_level = RANK_TDICU;
                    rank_value = get_word_level(eng, wid);
                    add_template(eng, rank_level + rank_value, word_chr_string, match_length);
                }
            }
        }
    }

    for (word_length = input_length - 1; (word_length >= eng->tmp_len) && (word_length >= 2); word_length--)
    {
        if (word_length <= eng->max_word_length)
        {
            i = k1 + (word_length - 2) * eng->key_count * eng->key_count;
            swid = (LDB_WID) read_bits(eng->word_index, i * eng->word_id_bits, eng->word_id_bits);
            ewid = (LDB_WID) read_bits(eng->word_index, (i + 1) * eng->word_id_bits, eng->word_id_bits);
            for (wid = swid; wid < ewid; wid++)
            {
                if (search_exgram(eng, wid2, wid))
                    continue;
                get_word_string(eng, wid, word_chr_string, word_key_string, word_length);
                if (eng->language == IIME_LANG_GREEK && filter_greek_word(word_chr_string, word_length, flags))
                    continue;
                match_length = match_string(
                    eng, input_string, input_length,
                    word_chr_string, word_key_string, word_length);
                if (match_length >= eng->tmp_len)
                {
                    if (match_length == word_length)
                    {
                        if (search_trigram(eng, wid1, wid2, wid))
                            rank_level = RANK_DICT;
                        else if (search_bigram(eng, wid2, wid))
                            rank_level = RANK_DICB;
                        else
                            rank_level = RANK_DICU;
                        rank_value = get_word_level(eng, wid);
                        add_template(eng, rank_level + rank_value, word_chr_string, match_length);
                    }
                    else /* match_length <= word_length */
                    {
                        if (search_trigram(eng, wid1, wid2, wid))
                            rank_level = RANK_TDICT;
                        else if (search_bigram(eng, wid2, wid))
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

static void match_prefix(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      input_string,
    IN int                  input_length,
    IN int                  flags)
{
    int                     i;
    MATCH*                  p1;
    MATCH*                  p2;
    LDB_CHAR                s[MAX_WORD_LENGTH];

    if (eng->language == IIME_LANG_VIETNAMESE)
        return;             /* Forward match is disabled for Vietnamese */

    if (input_length < FORWARD_MATCH)
        return;

    p1 = 0;
    p2 = eng->match_list;
    for (i = 0; i < eng->match_count; i++, p2++)
    {
        LDB_ASSERT(p2->length >= input_length);
        if (p2->rank == RANK_BOTTOM)
            continue;
        if (p2->length == input_length)
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
        memcpy(s, p1->string, sizeof(LDB_CHAR) * input_length);
        add_match(eng, RANK_PREFIX, s, input_length);
    }
}

static void match_legram(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      input_string,
    IN int                  input_length,
    IN int                  flags)
{
    int                     i;
    int                     j;
    int                     legram_count;
    const LDB_CHAR*         legram_list;
    LDB_BYTE                k[MAX_WORD_LENGTH];

    /* Do not generate legram match if a normal extra match exist */
    #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
    /* Skip this check for Context sensitive first char */
    if (!(input_length == 1 && 
         (eng->language == IIME_LANG_HINDI 
         || eng->language == IIME_LANG_NEPALI
         || eng->language == IIME_LANG_SINHALA
         || eng->language == IIME_LANG_BODO
         || eng->language == IIME_LANG_DOGRI
         || eng->language == IIME_LANG_SANSKRIT
         || eng->language == IIME_LANG_KASHMIRI
         || eng->language == IIME_LANG_KONKANI
         || eng->language == IIME_LANG_MAITHILI
         || eng->language == IIME_LANG_MANIPURI
         || eng->language == IIME_LANG_SANTALI
         || eng->language == IIME_LANG_SOMALI
         || eng->language == IIME_LANG_SINDHI
         || eng->language == IIME_LANG_THAI 
         || eng->language == IIME_LANG_BENGALI)//daodejing
         )
        )//IM_NEPALI_SUPPORT IM_SINHALA_SUPPORT
    #endif
    for (i = 0; i < eng->match_count; i++)
    {
        if ( (eng->match_list[i].rank != RANK_TOP) &&
             (eng->match_list[i].rank != RANK_BOTTOM) )
            return;
    }

    /* Generate legram matchs from template */

    LDB_ASSERT(eng->tmp_len < input_length);

    for (i = eng->tmp_len; i < input_length; i++)
        k[i] = chr_to_key(eng, input_string[i]);

    i = eng->tmp_len;
    while (i + 1 < input_length)
    {
        legram_list = eng->legram_string + eng->legram_index[k[i]];
        if (eng->language == IIME_LANG_THAI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_thai_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_thai_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
        else if (eng->language == IIME_LANG_HINDI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_hindi_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_hindi_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
        else if (eng->language == IIME_LANG_NEPALI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_nepali_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_nepali_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
        else if (eng->language == IIME_LANG_SINHALA)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_sinhala_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_sinhala_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		//daodejing
        else if (eng->language == IIME_LANG_BODO)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_bodo_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_bodo_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		else if (eng->language == IIME_LANG_DOGRI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_dogri_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_dogri_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		else if (eng->language == IIME_LANG_SANSKRIT)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_sanskrit_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_sanskrit_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		else if (eng->language == IIME_LANG_KASHMIRI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_kashmiri_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_kashmiri_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		else if (eng->language == IIME_LANG_KONKANI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_konkani_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_konkani_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		else if (eng->language == IIME_LANG_MAITHILI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_maithili_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_maithili_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		else if (eng->language == IIME_LANG_MANIPURI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_manipuri_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_manipuri_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		else if (eng->language == IIME_LANG_SANTALI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_santali_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_santali_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		else if (eng->language == IIME_LANG_SOMALI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_somali_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_somali_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
		else if (eng->language == IIME_LANG_SINDHI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_sindhi_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_sindhi_follow_char(eng->tmp_str[i-1], legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
            }
        }
        else if (eng->language == IIME_LANG_BENGALI)
        {
            legram_count = eng->legram_index[k[i]+1] - eng->legram_index[k[i]];
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);  /* Use digit if no valid char pair found */
            for (j = 0; j < legram_count; j++)
            {
                if (i == 0 && ldbapi4_is_valid_bengali_follow_char(0, legram_list[j]))
                {
                    eng->tmp_str[i] = legram_list[j];
                    break;
                }
                else if (i > 0 && ldbapi4_is_valid_bengali_follow_char(eng->tmp_str[i-1], legram_list[j]))
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

    if (eng->language == IIME_LANG_GREEK && (flags & LDB_UPPER_CASE))
    {
        if (k[i] == 4 || k[i] == 8)
            legram_count--;
    }

    if (eng->language == IIME_LANG_THAI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_thai_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_thai_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
    else if (eng->language == IIME_LANG_HINDI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_hindi_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_hindi_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
    else if (eng->language == IIME_LANG_NEPALI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_nepali_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_nepali_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }	
    else if (eng->language == IIME_LANG_SINHALA)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
			#if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
			#endif
                ldbapi4_is_valid_sinhala_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_sinhala_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }	
	//daodejing
    else if (eng->language == IIME_LANG_BODO)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_bodo_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_bodo_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_DOGRI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_dogri_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_dogri_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_SANSKRIT)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_sanskrit_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_sanskrit_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_KASHMIRI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_kashmiri_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_kashmiri_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_KONKANI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_konkani_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_konkani_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_MAITHILI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_maithili_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_maithili_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_MANIPURI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_manipuri_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_manipuri_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_SANTALI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_santali_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_santali_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_SOMALI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_somali_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_somali_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_SINDHI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_sindhi_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_sindhi_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
	else if (eng->language == IIME_LANG_BODO)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_bodo_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_bodo_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
    else if (eng->language == IIME_LANG_BENGALI)
    {
        int found_match = 0;
        for (j = 0; j < legram_count; j++)
        {
            if (i == 0 && 
                (
                #if defined(KMX_CFG_DISABLE_START_CHAR_CHECK)
                input_length == 1 || 
                #endif
                ldbapi4_is_valid_bengali_follow_char(0, legram_list[j])
                )
                )
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
            else if (i > 0 && ldbapi4_is_valid_bengali_follow_char(eng->tmp_str[i-1], legram_list[j]))
            {
                eng->tmp_str[i] = legram_list[j];
                add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
                found_match = 1;
            }
        }
        if (!found_match)
        {
            eng->tmp_str[i] = (LDB_CHAR)(LDB_CHAR_DIGIT_0 + k[i]);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }
    else
    {
        for (j = 0; j < legram_count; j++)
        {
            eng->tmp_str[i] = legram_list[j];
            string_convert_case((BASE_ENGINE*)eng, eng->tmp_str, input_length, flags);
            add_match(eng, RANK_LEGRAM + j + 10, eng->tmp_str, input_length);
        }
    }

    if (eng->language == IIME_LANG_ENGLISH && !eng->is_sure_type)
    {
        if ( (eng->tmp_len <= input_length - 2) &&
             (k[input_length - 2] == 1) &&
             (k[input_length - 1] == 7) )
        {
            eng->tmp_str[input_length - 2] = LDB_CHAR_APOSTROPHE;
            eng->tmp_str[input_length - 1] = 0x73;
            string_convert_case((BASE_ENGINE*)eng, eng->tmp_str, input_length, flags);
            add_match(eng, RANK_LEGRAM, eng->tmp_str, input_length);
        }
    }
}

static void parse_history(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      history_string,
    IN int                  history_length,
    OUT LDB_WID*            w1,
    OUT LDB_WID*            w2)
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

    if ( (history_string != 0) && (history_length > 0) )
    {
        /* Scan history from end toward start */

        start = history_string;
        scan = start + history_length - 1;

        while (scan >= start)
        {
            /* Find a word delimited by space */

            while ( (scan >= start) && (*scan == LDB_CHAR_SPACE) )
                scan--;

            cc = 0;
            while ( (scan >= start) && (*scan != LDB_CHAR_SPACE) )
            {
                scan--;
                cc++;
            }
            word = scan + 1;
            
            /* Stop search at SOS delimiters */

            if ( (cc > 0) && is_sos_delimiter((BASE_ENGINE*)eng, word[cc-1]) )
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

    *w1 = w[1];
    *w2 = w[0];
}

static int has_alphabet(
    IN const ENGINE*        eng,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    LDB_CHAR                c;

    LDB_ASSERT(len >= 0);

    while (len)
    {
        LDB_ASSERT(str);

        c = lower_case((BASE_ENGINE*)eng, *str);
        if (sorted_strchr(eng->alphabet, eng->alphabet_length, c) >= 0)
            return LDB_TRUE;
        else if ( (c >= 0x61) && (c <= 0x7A) )
            return LDB_TRUE;

        str++;
        len--;
    }

    return LDB_FALSE;
}

static int is_number(
    IN const ENGINE*        eng,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    LDB_ASSERT(len >= 0);

    while (len)
    {
        LDB_ASSERT(str);
        if ( (*str == LDB_CHAR_FULL_STOP) || ((*str >= LDB_CHAR_DIGIT_0) && (*str <= LDB_CHAR_DIGIT_9)) )
        {
        }
        else if (*str >= LDB_CHAR_ARABIC_DIGIT_0 && *str <= LDB_CHAR_ARABIC_DIGIT_9)
        {
        }
        else if (*str >= LDB_CHAR_PERSIAN_DIGIT_0 && *str <= LDB_CHAR_PERSIAN_DIGIT_9)
        {
        }
        else if (*str >= LDB_CHAR_HINDI_DIGIT_0 && *str <= LDB_CHAR_HINDI_DIGIT_9)
        {
        }
        else if (*str >= LDB_CHAR_NEPALI_DIGIT_0 && *str <= LDB_CHAR_NEPALI_DIGIT_9)
        {
        }
        else if (*str >= LDB_CHAR_SINHALA_DIGIT_0 && *str <= LDB_CHAR_SINHALA_DIGIT_9)
        {
        }
		//daodejing
        else if (*str >= LDB_CHAR_BODO_DIGIT_0 && *str <= LDB_CHAR_BODO_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_DOGRI_DIGIT_0 && *str <= LDB_CHAR_DOGRI_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_SANSKRIT_DIGIT_0 && *str <= LDB_CHAR_SANSKRIT_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_KASHMIRI_DIGIT_0 && *str <= LDB_CHAR_KASHMIRI_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_KONKANI_DIGIT_0 && *str <= LDB_CHAR_KONKANI_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_MAITHILI_DIGIT_0 && *str <= LDB_CHAR_MAITHILI_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_MANIPURI_DIGIT_0 && *str <= LDB_CHAR_MANIPURI_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_SANTALI_DIGIT_0 && *str <= LDB_CHAR_SANTALI_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_SOMALI_DIGIT_0 && *str <= LDB_CHAR_SOMALI_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_SINDHI_DIGIT_0 && *str <= LDB_CHAR_SINDHI_DIGIT_9)
        {
        }
		else if (*str >= LDB_CHAR_BENGALI_DIGIT_0 && *str <= LDB_CHAR_BENGALI_DIGIT_9)
        {
        }
        else if (*str >= LDB_CHAR_THAI_DIGIT_0 && *str <= LDB_CHAR_THAI_DIGIT_9)
        {
        }
        else
        {
            return LDB_FALSE;
        }
        str++;
        len--;
    }

    return LDB_TRUE;
}

static void add_cache(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    LDB_WID                 wid;
    int                     sos;

    /* Strip SOS delimiters at the end of input string */

    sos = 0;
    while ( (len > 0) && is_sos_delimiter((BASE_ENGINE*)eng, str[len-1]) )
    {
        sos = 1;
        len--;
    }

    if ( (len > 0) && (len <= MAX_WORD_LENGTH) )
    {
        wid = search_dictionary(eng, str, len);
        if (wid == eng->wid_oov)
        {
            if (has_alphabet(eng, str, len))
                udb_append(eng, eng->wid_oov, str, len);
            else
                udb_append(eng, eng->wid_sos, 0, 0);
        }
        else
        {
            udb_append(eng, wid, 0, len);
        }
    }

    if (sos)
        udb_append(eng, eng->wid_sos, 0, 0);
}

static void add_cache_eudw(
    IN ENGINE*              eng,
    IN const LDB_CHAR*      str,
    IN int                  len)
{
    if ( (len > 1) && (len <= MAX_WORD_LENGTH) )
    {
        if (search_dictionary(eng, str, len) == eng->wid_oov)
        {
            if (!is_number(eng, str, len))
            {
                udb_append(eng, eng->wid_sos, 0, 0);
                udb_append(eng, eng->wid_oov, str, len);
                udb_append(eng, eng->wid_sos, 0, 0);
            }
        }
    }
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Implementation of database functions                                    */
/*                                                                         */
/*-------------------------------------------------------------------------*/

int ldbapi4_get_db_info(
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
    info->mt_engine_object_size = sizeof(MT_ENGINE);

    info->max_input_length = MAX_WORD_LENGTH;

    info->eudw_prompt = ldb->eudw_prompt;
    info->eudw_prompt_length = ldb->eudw_prompt_length;

    info->eudw_title = ldb->eudw_title;
    info->eudw_title_length = ldb->eudw_title_length;

    return 1;
}

const LDB_CHAR* ldbapi4_get_db_multitype(
    IN const void*          database,
    IN LDB_CHAR             key,
    IN int                  flags,
    OUT int*                count)
{
    int                     i;
    int                     c;
    int                     key_count;
    const LDB*              ldb;

    if (!database)
        return 0;

    if (!count)
        return 0;

    ldb = (const LDB*) database;

    *count = 0;

    if (IS_SURE_TYPE(ldb->did))
    {
        key_count = 16;
        if (!IS_VALID_SURE_TYPE_INPUT(key))
            return 0;
    }
    else
    {
        key_count = 10;
        if (ldbapi4_is_key_star_used(database))
        {
            key_count += 1;
        }
        else
        {
            if (key == LDB_KEY_STAR)
                return 0;
        }
        
        if (ldbapi4_is_key_pound_used(database))
        {
            key_count += 1;
        }
        else
        {
            if (key == LDB_KEY_POUND)
                return 0;        
        }
        
        if ( (key < LDB_KEY_0) || (key > LDB_KEY_POUND) )
                return 0;
    }

    flags &= (LDB_LOWER_CASE | LDB_UPPER_CASE);
    if (flags == 0)
        flags = LDB_LOWER_CASE;

    i = key - LDB_KEY_0;
    if (flags == LDB_UPPER_CASE)
        i += key_count;
    else if (flags == (LDB_LOWER_CASE | LDB_UPPER_CASE))
        i += 2* key_count;

    c = ldb->mt_index[i+1] - ldb->mt_index[i];
    if (c <= 0)
        return 0;

    *count = c;
    return ldb->mt_string + ldb->mt_index[i];
}

int ldbapi4_ldb_is_sou_delimiter(
    IN const void*          database,
    IN LDB_CHAR             c)
{
    const LDB*              ldb;

    if (!database)
        return 0;

    ldb = (const LDB*) database;
    return (sorted_strchr(ldb->sou_table, ldb->sou_table_length, c) >= 0) ? LDB_TRUE : LDB_FALSE;
}

int ldbapi4_is_key_star_used(IN const void* database)
{
    const LDB*              ldb;

    if(!database)
        return 0;
    
    ldb = (const LDB*) database;
	
    if (IS_SURE_TYPE(ldb->did))
        return 0;
	
	/* Thai use key star */	
    #if defined(KMX_LDB_THAI)
    if (ldb->did == 362 || ldb->did == 366 || ldb->did == 369 || ldb->did == 3603 ||ldb->did == 3607 || ldb->did == 3611 ||ldb->did == 3616 || ldb->did == 3632 || ldb->did == 3604)
        return 1;
    #endif

    #if defined(KMX_LDB_BENGALI)
    if (ldb->did == 924 || ldb->did == 921 || ldb->did == 928 || ldb->did == 922 || ldb->did == 929 || ldb->did == 9200 || ldb->did == 9201 || ldb->did == 9202 || ldb->did == 9203)
        return 1;
    #endif
        
    return 0;
}

int ldbapi4_is_key_pound_used(IN const void* database)
{
    const LDB*              ldb;

    if(!database)
        return 0;
    
    ldb = (const LDB*) database;
	
    if (IS_SURE_TYPE(ldb->did))
        return 0;

    #if defined(KMX_LDB_BENGALI)
    if (ldb->did == 9200)
        return 1;
#if defined(WATERWORLD_BENGALI_SPECIAL) || defined(SPRD_BENGALI_SPECIAL) || defined(BENGALI_ARABNUM_AND_POUND_INPUT)
    if (ldb->did == 9201)
        return 1;
#endif
    #endif
        
    return 0;
}


/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Implementation of engine functions                                      */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static void initialize_base_engine(
    IN BASE_ENGINE*         eng,
    IN const LDB*           ldb)
{
    eng->did                     = ldb->did;
    eng->major_version           = ldb->major_version;
    eng->minor_version           = ldb->minor_version;
    eng->build_number            = ldb->build_number;

    eng->copyright               = ldb->copyright;
    eng->copyright_length        = ldb->copyright_length;

    eng->eudw_prompt             = ldb->eudw_prompt;
    eng->eudw_prompt_length      = ldb->eudw_prompt_length;

    eng->eudw_title              = ldb->eudw_title;
    eng->eudw_title_length       = ldb->eudw_title_length;

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

    eng->char_code_count         = ldb->char_code_count;
    eng->char_code_bits          = ldb->char_code_bits;

    eng->char_table              = ldb->char_table;
    eng->ckey_table              = ldb->ckey_table;

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
        if (ldbapi4_is_key_star_used(ldb))
        {
            eng->key_count += 1;
            eng->flags |= LDB_USE_KEY_STAR;
        }
        if (ldbapi4_is_key_pound_used(ldb))
        {
            /* pound key must be used together with star key!!! */
            LDB_ASSERT(ldbapi4_is_key_star_used(ldb));
            eng->key_count += 1;
            eng->flags |= LDB_USE_KEY_POUND;
        }        
    }
}

int ldbapi4_initialize(
    IN void*                engobj,
    IN const LDB_INIT*      init)
{
    ENGINE*                 eng;
    const LDB*              ldb;

    if (!engobj)
        return LDB_FALSE;

    if (!init)
        return LDB_FALSE;

    eng = (ENGINE*)engobj;

    /* Find database */

    ldb = (const LDB*) init->database;
    if (!ldb)
        return LDB_FALSE;

    LDB_ASSERT(ldb_is_valid(ldb));//zhmch

    /* Initialize engine object */

    memset(eng, 0, sizeof(ENGINE));

    eng->signature               = LDB_SMART_ENGINE_SIGNATURE;

    initialize_base_engine((BASE_ENGINE*)eng, ldb);

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

    if ( (init->user_data != 0) && (init->user_data_size >= UDB_MIN) )
        udb_open(eng, init->user_data, init->user_data_size);

    return LDB_TRUE;
}

void ldbapi4_cleanup(
    IN void*                engobj)
{
    LDB_ASSERT(engobj);

    /* Do nothing by design */
}

static int ldbapi4_mt_initialize_mt_tables(IN MT_ENGINE* eng)
{
    #ifdef KMX_LDB_THAI
    if (eng->language == IIME_LANG_THAI)
        return thai_init_mt_tables(eng);
    #endif

    #ifdef KMX_LDB_HINDI
    if (eng->language == IIME_LANG_HINDI)
        return hindi_init_mt_tables(eng);
    #endif

	#ifdef KMX_LDB_NEPALI
    if (eng->language == IIME_LANG_NEPALI)
        return nepali_init_mt_tables(eng);
	#endif

 	#ifdef KMX_LDB_SINHALA
    if (eng->language == IIME_LANG_SINHALA)
        return sinhala_init_mt_tables(eng);
	#endif

	//daodejing
 	#ifdef KMX_LDB_BODO
    if (eng->language == IIME_LANG_BODO)
        return bodo_init_mt_tables(eng);
	#endif

	#ifdef KMX_LDB_DOGRI
    if (eng->language == IIME_LANG_DOGRI)
        return dogri_init_mt_tables(eng);
	#endif

	#ifdef KMX_LDB_SANSKRIT
    if (eng->language == IIME_LANG_SANSKRIT)
        return sanskrit_init_mt_tables(eng);
	#endif

	#ifdef KMX_LDB_KASHMIRI
    if (eng->language == IIME_LANG_KASHMIRI)
        return kashmiri_init_mt_tables(eng);
	#endif

	#ifdef KMX_LDB_KONKANI
    if (eng->language == IIME_LANG_KONKANI)
        return konkani_init_mt_tables(eng);
	#endif

	#ifdef KMX_LDB_MAITHILI
    if (eng->language == IIME_LANG_MAITHILI)
        return maithili_init_mt_tables(eng);
	#endif

	#ifdef KMX_LDB_MANIPURI
    if (eng->language == IIME_LANG_MANIPURI)
        return manipuri_init_mt_tables(eng);
	#endif

	#ifdef KMX_LDB_SANTALI
    if (eng->language == IIME_LANG_SANTALI)
        return santali_init_mt_tables(eng);
	#endif

	#ifdef KMX_LDB_SOMALI
    if (eng->language == IIME_LANG_SOMALI)
        return somali_init_mt_tables(eng);
	#endif

	#ifdef KMX_LDB_SINDHI
    if (eng->language == IIME_LANG_SINDHI)
        return sindhi_init_mt_tables(eng);
	#endif

    #ifdef KMX_LDB_BENGALI
    if (eng->language == IIME_LANG_BENGALI)
        return bengali_init_mt_tables(eng);
    #endif

    #ifdef KMX_LDB_TIBETAN
    if (eng->language == IIME_LANG_TIBETAN)
        return tibetan_init_mt_tables(eng);
    #endif

    return LDB_TRUE;        
}

int ldbapi4_mt_initialize(
    IN void*                engobj,
    IN const void*          database)
{
    MT_ENGINE*              eng;
    const LDB*              ldb;

    if (!engobj)
        return LDB_FALSE;

    if (!database)
        return LDB_FALSE;

    eng = (MT_ENGINE*)engobj;
    
    /* Find database */

    ldb = (const LDB*) database;
    
    LDB_ASSERT(ldb_is_valid(ldb));

    /* Initialize engine object */

    memset(eng, 0, sizeof(MT_ENGINE));

    eng->signature               = LDB_MULTITAP_ENGINE_SIGNATURE;
    
    initialize_base_engine((BASE_ENGINE*)eng, ldb);

    if (!ldbapi4_mt_initialize_mt_tables(eng))
        return LDB_FALSE;

    return LDB_TRUE;
}

void ldbapi4_mt_cleanup(
    IN void*                engobj)
{
    LDB_ASSERT(engobj);

    /* Do nothing by design */
}

void ldbapi4_get_info(
    IN void*                engobj,
    OUT LDB_INFO*           info)
{
    BASE_ENGINE*            eng;

    LDB_ASSERT(engobj);
    eng = (BASE_ENGINE*)engobj;

    LDB_ASSERT(info);

    info->database_id = eng->did;
    info->language = eng->language;
    info->is_sure_type = eng->is_sure_type;

    info->major_version = eng->major_version;
    info->minor_version = eng->minor_version;
    info->build_number = eng->build_number;

    info->copyright = eng->copyright;
    info->copyright_length = eng->copyright_length;

    
    info->engine_object_size = sizeof(ENGINE);
    info->mt_engine_object_size = sizeof(MT_ENGINE);

    info->max_input_length = MAX_WORD_LENGTH;

    info->eudw_prompt = eng->eudw_prompt;
    info->eudw_prompt_length = eng->eudw_prompt_length;

    info->eudw_title = eng->eudw_title;
    info->eudw_title_length = eng->eudw_title_length;
}

const LDB_CHAR* ldbapi4_get_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN int                  flags,
    OUT int*                count)
{
    BASE_ENGINE*            eng;
    int                     i;
    int                     c;

    LDB_ASSERT(engobj);
    eng = (BASE_ENGINE*)engobj;

    LDB_ASSERT(count);
    *count = 0;

    if (eng->is_sure_type)
    {
        if (!IS_VALID_SURE_TYPE_INPUT(key))
            return 0;
    }
    else
    {    
        if (!(eng->flags & LDB_USE_KEY_STAR))
        {
            if (key == LDB_KEY_STAR)
                return 0;           /* Invalid key */
        }

        if (!(eng->flags & LDB_USE_KEY_POUND))
        {
            if (key == LDB_KEY_POUND)
                return 0;           /* Invalid key */
        }
        
        if ( (key < LDB_KEY_0) || (key > LDB_KEY_POUND) )
            return 0;           /* Invalid key */

    }

    flags &= (LDB_LOWER_CASE | LDB_UPPER_CASE);
    if (flags == 0)
        flags = LDB_LOWER_CASE;

    i = key - LDB_KEY_0;
    if (flags == LDB_UPPER_CASE)
        i += eng->key_count;
    else if (flags == (LDB_LOWER_CASE | LDB_UPPER_CASE))
        i += 2 * eng->key_count;

    c = eng->mt_index[i+1] - eng->mt_index[i];
    if (c <= 0)
        return 0;

    *count = c;
    return eng->mt_string + eng->mt_index[i];
}

LDB_CHAR ldbapi4_char_to_key(
    IN void*                engobj,
    IN LDB_CHAR             c)
{
    BASE_ENGINE*            eng;
    int                     i;

    LDB_ASSERT(engobj);
    eng = (BASE_ENGINE*)engobj;

    if (eng->is_sure_type)
    {
        if (IS_VALID_SURE_TYPE_INPUT(c))
            return c;
    }
    else
    {
        if ((eng->flags & LDB_USE_KEY_STAR) && c == LDB_KEY_STAR)
            return c;
        if ((eng->flags & LDB_USE_KEY_POUND) && c == LDB_KEY_POUND)
            return c;
        
        if (c >= LDB_KEY_0 && c <= LDB_KEY_9)
            return c;
    }

    if ( (c >= LDB_CHAR_DIGIT_0) && (c <= LDB_CHAR_DIGIT_9) )
        return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_DIGIT_0));

    if ( (c >= LDB_CHAR_ARABIC_DIGIT_0) && (c <= LDB_CHAR_ARABIC_DIGIT_9) )
        return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_ARABIC_DIGIT_0));

    if ( (c >= LDB_CHAR_PERSIAN_DIGIT_0) && (c <= LDB_CHAR_PERSIAN_DIGIT_9) )
        return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_PERSIAN_DIGIT_0));

    if ( (c >= LDB_CHAR_HINDI_DIGIT_0) && (c <= LDB_CHAR_HINDI_DIGIT_9) )
        return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_HINDI_DIGIT_0));
	if ( (c >= LDB_CHAR_NEPALI_DIGIT_0) && (c <= LDB_CHAR_NEPALI_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_NEPALI_DIGIT_0));	
	if ( (c >= LDB_CHAR_SINHALA_DIGIT_0) && (c <= LDB_CHAR_SINHALA_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_SINHALA_DIGIT_0));	
	//daodejing
	if ( (c >= LDB_CHAR_BODO_DIGIT_0) && (c <= LDB_CHAR_BODO_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_BODO_DIGIT_0));
	if ( (c >= LDB_CHAR_DOGRI_DIGIT_0) && (c <= LDB_CHAR_DOGRI_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_DOGRI_DIGIT_0));
	if ( (c >= LDB_CHAR_SANSKRIT_DIGIT_0) && (c <= LDB_CHAR_SANSKRIT_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_SANSKRIT_DIGIT_0));
	if ( (c >= LDB_CHAR_KASHMIRI_DIGIT_0) && (c <= LDB_CHAR_KASHMIRI_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_KASHMIRI_DIGIT_0));
	if ( (c >= LDB_CHAR_KONKANI_DIGIT_0) && (c <= LDB_CHAR_KONKANI_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_KONKANI_DIGIT_0));
	if ( (c >= LDB_CHAR_MAITHILI_DIGIT_0) && (c <= LDB_CHAR_MAITHILI_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_MAITHILI_DIGIT_0));
	if ( (c >= LDB_CHAR_MANIPURI_DIGIT_0) && (c <= LDB_CHAR_MANIPURI_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_MANIPURI_DIGIT_0));
	if ( (c >= LDB_CHAR_SANTALI_DIGIT_0) && (c <= LDB_CHAR_SANTALI_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_SANTALI_DIGIT_0));
	if ( (c >= LDB_CHAR_SOMALI_DIGIT_0) && (c <= LDB_CHAR_SOMALI_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_SOMALI_DIGIT_0));
	if ( (c >= LDB_CHAR_SINDHI_DIGIT_0) && (c <= LDB_CHAR_SINDHI_DIGIT_9) )
        	return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_SINDHI_DIGIT_0));
	if ( (c >= LDB_CHAR_BENGALI_DIGIT_0) && (c <= LDB_CHAR_BENGALI_DIGIT_9) )
        return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_BENGALI_DIGIT_0));

    if ( (c >= LDB_CHAR_THAI_DIGIT_0) && (c <= LDB_CHAR_THAI_DIGIT_9) )
        return (LDB_CHAR)(LDB_KEY_0 + (c - LDB_CHAR_THAI_DIGIT_0));

    c = lower_case(eng, c);
    i = sorted_strchr(eng->char_table, eng->char_code_count, c);
    if (i >= 0)
        return (LDB_CHAR) (LDB_KEY_0 + eng->ckey_table[i]);
    
    if ((eng->flags & LDB_USE_KEY_STAR) && c == '*')
    {
        return LDB_KEY_STAR;
    }

    if ((eng->flags & LDB_USE_KEY_POUND) && c == '#')
    {
        return LDB_KEY_POUND;
    }
    
    if (eng->is_sure_type)
    {
        if ( (c >= 0x61) && (c <= 0x7A) )
            return (LDB_CHAR) (LDB_KEY_0 + BASIC_LATIN_SURE_TYPE_MAP[c - 0x61]);
        return LDB_KEY_15;
    }
    else
    {
        if ( (c >= 0x61) && (c <= 0x7A) )
            return (LDB_CHAR) (LDB_KEY_0 + BASIC_LATIN_9KEY_MAP[c - 0x61]);
        
        if ( c == 0x20 )
            return LDB_KEY_0;
            
        if ((eng->flags & LDB_USE_KEY_STAR))
        {
            LDB_WORD i;
            for (i = eng->mt_index[10]; i < eng->mt_index[11]; i++)
            {
                if (eng->mt_string[i] == c)
                    return LDB_KEY_STAR;
            }                            
        }

        if ((eng->flags & LDB_USE_KEY_POUND))
        {
            LDB_WORD i;
            for (i = eng->mt_index[11]; i < eng->mt_index[12]; i++)
            {
                if (eng->mt_string[i] == c)
                    return LDB_KEY_POUND;
            }                            
        }
                        
        return LDB_KEY_1;
    }
}

#if 1//IEKIE_CAND_LINK
typedef unsigned short			uint16;
#ifndef wchar
typedef uint16 wchar;
#endif
#ifndef uint8
#define uint8  unsigned char
#endif
extern wchar* MMI_STRNTOWSTR(
                             wchar* dst,
                             size_t dst_max_count,
                             const uint8* src,
                             size_t src_max_count,
                             size_t count
                             );

const char * const iekie_cand_link_str = "iekie";
int iekie_cand_link_index;
#endif//IEKIE_CAND_LINK

int ldbapi4_match(
    IN void*                engobj,
    IN const LDB_CHAR*      context_before_string,
    IN int                  context_before_length,
    IN const LDB_CHAR*      context_after_string,
    IN int                  context_after_length,
    IN const LDB_CHAR*      input_string,
    IN int                  input_length,
    IN int                  flags,
    IN int                  max_match)
{
    ENGINE*                 eng;
    LDB_WID                 wid1;
    LDB_WID                 wid2;
    LDB_CHAR                buf[MAX_WORD_LENGTH];

	#if 0//IEKIE_CAND_LINK
	wchar iekie_link_cand_str[100];
	uint16 iekie_link_cand_str_len = 0;
	#endif//IEKIE_CAND_LINK
	
    LDB_ASSERT(engobj);
    eng = (ENGINE*)engobj;

    if (eng->signature != LDB_SMART_ENGINE_SIGNATURE)
        return 0;


    LDB_ASSERT(input_string);

    if (input_length <= 0 || input_length > MAX_WORD_LENGTH)
        return 0;

    LDB_ASSERT( (input_length > 0) && (input_length <= MAX_WORD_LENGTH) );

    /* Clear previous match result */

    eng->max_match = MAX_MATCH - 1;
    eng->match_count = 0;
    eng->tmp_len = 0;
    eng->is_exgram_found = LDB_FALSE;

    /* Find last two words in history */

    parse_history(eng, context_before_string, context_before_length, &wid1, &wid2);

    /* If input string is a normal string, add it into match list */

    if (has_key(eng, input_string, input_length))
    {
        memcpy(buf, input_string, sizeof(LDB_CHAR) * input_length);
        string_convert_case((BASE_ENGINE*)eng, buf, input_length, LDB_LOWER_CASE);
    }
    else
    {
        match_orginal(eng, input_string, input_length, flags);
        input_string_to_key_string(eng, input_string, input_length, buf);
    }

    input_string = buf;

    /* Always generate a number candidate */

    match_number(eng, input_string, input_length, flags);

    /* Search candidates in cache */

    match_cache(eng, wid1, wid2, input_string, input_length, flags);

    /* Search candidates in dictionary */

    match_dictionary(eng, wid1, wid2, input_string, input_length, flags);

    /* Match prefix */

    eng->max_match = MAX_MATCH;

    match_prefix(eng, input_string, input_length, flags);

    /* Match key string in legram */

    if (!eng->is_exgram_found)
        match_legram(eng, input_string, input_length, flags);

    /* Sort match list by rank */

    qsort(eng->match_list, eng->match_count, sizeof(MATCH), cmp_match);
	//zhmch iekie
	#if 0//IEKIE_CAND_LINK
	if (eng->match_count < MAX_MATCH)
	{
	  iekie_link_cand_str_len = strlen((char*)iekie_cand_link_str);
	  MMI_STRNTOWSTR(iekie_link_cand_str,iekie_link_cand_str_len,(const uint8 *)iekie_cand_link_str,
			  iekie_link_cand_str_len,
			  iekie_link_cand_str_len
			  );
	  MMI_WSTRNCPY(eng->match_list[eng->match_count].string,
	  			iekie_link_cand_str_len,
				iekie_link_cand_str,iekie_link_cand_str_len,iekie_link_cand_str_len);
	  
	  eng->match_list[eng->match_count].rank = 1;
	  eng->match_list[eng->match_count].length = iekie_link_cand_str_len;
	  iekie_cand_link_index = eng->match_count;
	  eng->match_count++;
	}
	#endif//IEKIE_CAND_LINK

    return eng->match_count;
}

const LDB_CHAR* ldbapi4_get_match(
    IN void*                engobj,
    IN int                  match_index,
    OUT int*                length)
{
    const ENGINE*           eng;
    const MATCH*            m;

    LDB_ASSERT(engobj);
    eng = (const ENGINE*)engobj;

    if (eng->signature != LDB_SMART_ENGINE_SIGNATURE)
        return NULL;

    LDB_ASSERT( (match_index >= 0) && (match_index < eng->match_count) );
    m = eng->match_list + match_index;

    LDB_ASSERT(length);
    *length = m->length;

    return m->string;
}

void ldbapi4_reset_user_data(
    IN void*                engobj)
{
    ENGINE*                 eng;

    LDB_ASSERT(engobj);
    eng = (ENGINE*)engobj;

    if (eng->signature != LDB_SMART_ENGINE_SIGNATURE)
        return;

    if (eng->udb)
        udb_init(eng, eng->udb, eng->udb->size);
}

void ldbapi4_update_user_data(
    IN void*                engobj,
    IN const LDB_CHAR*      history_string,
    IN int                  history_length,
    IN int                  flags)
{
    ENGINE*                 eng;
    const LDB_CHAR*         scan;
    const LDB_CHAR*         end;
    const LDB_CHAR*         word;
    int                     cc;

    LDB_ASSERT(engobj);
    eng = (ENGINE*)engobj;
    
    if (eng->signature != LDB_SMART_ENGINE_SIGNATURE)
        return;

    if (!eng->udb)
        return;

    if ( (history_string == 0) || (history_length <= 0) )
    {
        udb_append(eng, eng->wid_sos, 0, 0);
    }
    else
    {
        /* Scan history from beginning to the end */

        scan = history_string;
        end = scan + history_length;

        while (scan < end)
        {
            /* Find a word delimited by space */

            while ( (scan < end) && (*scan == LDB_CHAR_SPACE) )
                scan++;

            cc = 0;
            word = scan;
            while ( (scan < end) && (*scan != LDB_CHAR_SPACE) )
            {
                scan++;
                cc++;
            }

            /*  Append the word to cache */

            if (flags & LDB_EUDW)
                add_cache_eudw(eng, word, cc);
            else
                add_cache(eng, word, cc);
        }
    }
}

int ldbapi4_is_sou_delimiter(
    IN void*                engobj,
    IN LDB_CHAR             c)
{
    BASE_ENGINE*            eng;

    LDB_ASSERT(engobj);
    eng = (BASE_ENGINE*)engobj;

    return is_sou_delimiter(eng, c);
}

void ldbapi4_string_convert_case(
    IN void*                engobj,
    IN OUT LDB_CHAR*        string,
    IN int                  length,
    IN int                  flags)
{
    BASE_ENGINE*            eng;

    LDB_ASSERT(engobj);
    eng = (BASE_ENGINE*)engobj;

    string_convert_case((BASE_ENGINE*)eng, string, length, flags);
}

int ldbapi4_check_case(
    IN void*                engobj,
    IN const LDB_CHAR*      string,
    IN int                  length)
{
    BASE_ENGINE*            eng;
    int                     i;
    int                     initial_upper_case;
    int                     initial_lower_case;
    int                     remain_upper_case_found;
    int                     remain_lower_case_found;

    LDB_ASSERT(engobj);
    eng = (BASE_ENGINE*)engobj;

    /* Check case of initial char */

    LDB_ASSERT(string);
    LDB_ASSERT(length > 0);

    initial_upper_case = 0;
    initial_lower_case = 0;

    i = sorted_strchr(eng->uc2lcm, eng->uc2lcm_length, *string);
    if (i >= 0)
    {
        initial_upper_case = 1;
    }
    else
    {
        i = sorted_strchr(eng->lc2ucm, eng->lc2ucm_length, *string);
        if (i >= 0)
            initial_lower_case = 1;
    }

    string++;
    length--;

    /* Check case of remain chars */

    remain_upper_case_found = 0;
    remain_lower_case_found = 0;

    while (length > 0)
    {
        if (!remain_upper_case_found)
        {
            i = sorted_strchr(eng->uc2lcm, eng->uc2lcm_length, *string);
            if (i >= 0)
                remain_upper_case_found = 1;
        }
        if (!remain_lower_case_found)
        {
            i = sorted_strchr(eng->lc2ucm, eng->lc2ucm_length, *string);
            if (i >= 0)
                remain_lower_case_found = 1;
        }
        if (remain_upper_case_found && remain_lower_case_found)
            break;
        string++;
        length--;
    }

    if (initial_upper_case)
    {
        if (remain_upper_case_found)
            return remain_lower_case_found ? LDB_INITIAL_CASE : LDB_UPPER_CASE;
        else
            return LDB_INITIAL_CASE;
    }
    else if (initial_lower_case)
    {
        return LDB_LOWER_CASE;
    }
    else
    {
        if (remain_upper_case_found)
            return remain_lower_case_found ? LDB_LOWER_CASE : LDB_UPPER_CASE;
        else
            return LDB_LOWER_CASE;
    }
}

/*****************************************************************************************************
 * 
 *  Hangul Input 
 * 
 *****************************************************************************************************/

static const unsigned short default_multitap_hangul_base[10][4] = 
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

/* if the return value equal to prev_char, indicates compose failed, else the return value is the composed char */

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

static int get_string_length(const LDB_CHAR* str)
{
    int count = 0;
    while (*str)
    {
        count++;
        str++;
    }
    return count;
}

static unsigned short compose_syllable(unsigned short lc, unsigned short vo, unsigned short tc)
{
    /*TODO: Add range check*/
    if (tc == 0)
    {
        tc = TBASE;
    }
    return (unsigned short)(((lc - LBASE) * VCOUNT + (vo - VBASE)) * TCOUNT + (tc - TBASE) + SBASE);
}

static void decompose_syllable(unsigned short syllable, unsigned short * lc, unsigned short * vo, unsigned short * tc)
{
    unsigned short sindex = (unsigned short)(syllable - SBASE);
    *lc = (unsigned short)(LBASE + sindex / NCOUNT);
    *vo = (unsigned short)(VBASE + (sindex % NCOUNT) / TCOUNT);
    *tc = (unsigned short)(TBASE + sindex % TCOUNT);

    if (*tc == TBASE)
    {
        *tc = 0;
    }
}

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
    { 0x11Af, 0x11B7, 0x11B1 },
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

static unsigned short compose_lc(unsigned short c1, unsigned short c2)
{
    return find_mapping(lc_compose_table, c1, c2);
}

static unsigned short compose_vo(unsigned short c1, unsigned short c2)
{
    return find_mapping(vo_compose_table, c1, c2);
}

static unsigned short compose_tc(unsigned short c1, unsigned short c2)
{    
    if (c1 == 0)
    {
        c1 = c2;
        c2 = 0;
    }

    return find_mapping(tc_compose_table, c1, c2);
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
                *tail = e->c1;    
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

static unsigned short compose_char(unsigned short prev_char, unsigned short cur_char)
{
    if (IS_SYLLABLE(prev_char))
    {
        unsigned short lc, vo, tc;
        decompose_syllable(prev_char, &lc, &vo, &tc);

        if (IS_VO(cur_char))
        {
            unsigned short comp_vo;

            if (tc)
            {
                /* can not compose vowel if T presents */
                return prev_char;
            }

            comp_vo = compose_vo(vo, cur_char);
            /* can not compose vowel */
            if (comp_vo == vo || comp_vo == 0)
            {
                return prev_char;
            }

            return compose_syllable(lc, comp_vo, tc);
        }

        if (IS_LC(cur_char))
        {
            unsigned short comp_tc;
            
            comp_tc = compose_tc(tc, cur_char);

            if (comp_tc == tc || comp_tc == 0)
            {
                return prev_char;
            }

            return compose_syllable(lc, vo, comp_tc);
        }
        else if (IS_TC(cur_char))
        {
            if (tc == 0)
                return compose_syllable(lc, vo, cur_char);
        }

        
    }

    if (IS_LC(prev_char))
    {
        if (IS_LC(cur_char))
        {
            return compose_lc(prev_char, cur_char);
        }

        if (IS_VO(cur_char))
        {
            return compose_syllable(prev_char, cur_char, 0);
        }        
    }

    /* fallback vowel compose */
    if (IS_VO(prev_char))
    {
        if (IS_VO(cur_char))
        {
            return compose_vo(prev_char, cur_char);
        }
    }
    
    return prev_char;
        
}

static void remove_previous_char(void)
{
    /* TODO: */
}

static unsigned short * compose_components(unsigned short prev_char, unsigned short * components)
{
    static unsigned short char_buf[20];    
    unsigned int flag = 0;
    int i = 0;
    unsigned short * p = components;

    while (*p)
    {
        unsigned short c;
        c = compose_char(prev_char, *p);
        if (c != prev_char && c != 0)
        {
            flag = 1;
            char_buf[i] = c;
            i++;
        }        
        p++;
    }
    char_buf[i] = 0;

    if (flag)
    {
        /* remove previous char, this should be lift */
        remove_previous_char();
        return char_buf;
    }
    else
        return components;    
}


static unsigned short map_transform_char(unsigned short prev_char)
{
    return find_mapping(char_transform_table, prev_char, 0x2A);
}

static unsigned short * transform_char(unsigned short prev_char)
{    
    static unsigned short char_buf[2];
    unsigned short nc;
    unsigned short lc, vo, tc;
    memset(char_buf, 0, sizeof(char_buf));
    

    if (IS_SYLLABLE(prev_char))
    {        
        decompose_syllable(prev_char, &lc, &vo, &tc);
        
        if (tc == 0)
        {
            /* must be a vo comp */
            nc = map_transform_char(vo);
            if (nc == vo || nc == 0)
                return char_buf;
            
            vo = nc;
            goto compose_return;
        }

        nc = map_transform_char(tc);
        if (nc == tc || nc == 0)
        {
            return char_buf;
        }
        tc = nc;
        goto compose_return;
    }

    nc = map_transform_char(prev_char);
    if (nc == prev_char || nc == 0)
    {
        return char_buf;
    }
    remove_previous_char();
    char_buf[0] = nc;
    return char_buf;

compose_return:
    remove_previous_char();
    char_buf[0] = compose_syllable(lc, vo, tc);
    return char_buf;
}

const LDB_CHAR* ldbapi4_get_hangul_multitype(
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count,
    OUT int*                has_valid_comp)

{  
    /* key star */
    LDB_CHAR* entry;

    if (key != LDB_KEY_STAR)
    {
        int i;

        if (key > LDB_KEY_9 || key < LDB_KEY_0)
        {
            return NULL;
        }

        i = key - LDB_KEY_0;

        if (!prev_char)
        {
            entry = (LDB_CHAR*)default_multitap_hangul_base[i];
            *count = get_string_length(entry);
            if (i == 8) /* 0x11B1 only shown at T position. */
                *count = *count - 1;

            if (i)            
                *has_valid_comp = LDB_TRUE;            
            else
                *has_valid_comp = LDB_FALSE;

            return entry;
        }
        
        if(!IS_SYLLABLE(prev_char) && !IS_LC(prev_char) && !IS_VO(prev_char) && !IS_TC(prev_char))
        {
            entry = (LDB_CHAR*)default_multitap_hangul_base[i];
            *count = get_string_length(entry);
            if (i == 8) /* 0x11B1 only shown at T position. */
                *count = *count - 1;

             if (i)            
                *has_valid_comp = LDB_TRUE;            
            else
                *has_valid_comp = LDB_FALSE;
            return entry;
        }
        
        entry = compose_components(prev_char, (unsigned short*)default_multitap_hangul_base[i]);
        if (entry == default_multitap_hangul_base[i])        
            *has_valid_comp = LDB_FALSE;        
        else
            *has_valid_comp = LDB_TRUE;
        
        *count = get_string_length(entry);
        if (i == 8 && !(*has_valid_comp))
        {
            *count = *count - 1;
        }
        return entry;
    }
    else
    {           
        entry = transform_char(prev_char);
        if (!entry || entry[0] == 0 || entry[0] == prev_char)        
            *has_valid_comp = LDB_FALSE;        
        else
            *has_valid_comp = LDB_TRUE;

        *count = get_string_length(entry);
        return entry;
    }

}


int ldbapi4_get_hangul_multitype_ex(
        IN LDB_CHAR				key,
		IN LDB_CHAR             previous_char,
		OUT LDB_CHAR*           commit_char,
		OUT LDB_CHAR*           comp_char,
        OUT LDB_CHAR*           prev_comp_char)
{
    LDB_CHAR t = 0, t_prev = 0, t_last = 0;
    const LDB_CHAR* entry;
    int has_valid_comp = 0;
    int count;

    if (!IS_SYLLABLE(previous_char) || !S_HAS_T(previous_char))
        return 0;

    t = (unsigned short)(T_FROM_S(previous_char));
    decompose_tc(t, &t_prev, &t_last);

    entry = ldbapi4_get_hangul_multitype(key, t_last, &count, &has_valid_comp);

    if (!has_valid_comp)
        return 0;
    
    if (commit_char)
    {
        LDB_CHAR l, v;
        l = (unsigned short)L_FROM_S(previous_char);
        v =(unsigned short)V_FROM_S(previous_char);
        if (t_prev)
            *commit_char = (unsigned short)S_FROM_LVT(l, v, t_prev);
        else
            *commit_char = (unsigned short)S_FROM_LV(l, v);
        
    }

    if (comp_char && entry)
    {
        *comp_char = entry[0];
    }

    if (prev_comp_char)
    {
        if (t_last)
        {
            *prev_comp_char = t_last;
        }        
    }

    return 1;
}

int ldbapi4_is_indic_dummy_did(
    IN int                          did)
{
        int language;
        if (IS_LATIN_EXTEND_DID(did))
            language = LATIN_EXTEND_DID_TO_LANG(did);
        else
            language = did / 10;

        if (language == IIME_LANG_TELUGU || language == IIME_LANG_MARATHI || language == IIME_LANG_TAMIL
            || language == IIME_LANG_PUNJABI || language == IIME_LANG_GUJARATI || language == IIME_LANG_ORIYA
            || language == IIME_LANG_KANNADA || language == IIME_LANG_MALAYALAM || language == IIME_LANG_KHMER
            || language == IIME_LANG_LAO || language == IIME_LANG_BODO || language == IIME_LANG_DOGRI || language == IIME_LANG_SANSKRIT 
            || language == IIME_LANG_KASHMIRI || language == IIME_LANG_KONKANI || language == IIME_LANG_MAITHILI || language == IIME_LANG_MANIPURI 
            || language == IIME_LANG_SANTALI || language == IIME_LANG_SOMALI || language == IIME_LANG_SINDHI )//daodejing  need
            return 1;

       return 0;
}


