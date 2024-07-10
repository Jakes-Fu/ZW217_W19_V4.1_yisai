
/*

ldbapi4.h

*/

#ifndef LDBAPI4_H
#define LDBAPI4_H

#ifdef __cplusplus
extern "C"{
#endif 

#ifndef IN
    #define IN
#endif

#ifndef OUT
    #define OUT
#endif

/*

Key constants

The encoding of input key in LDBAPI is Unicode as the same as LDB_CHAR.
The data type of input key is LDB_CHAR.

The code points of input key is defined in Unicode Noncharacters Area
to avoid overlap with normal characher code points.

Definitions for "Noncharacters" in The Unicode Standard 4.0 :

    "Noncharacters are code points that are permanently reserved in the
    Unicode Standard for internal use. They are forbidden for use in open
    interchange of Unicode text data."

    "Applications are free to use any of these noncharacter code points
    internally but should never attempt to exchange them. If a noncharacter
    is received in open interchange, an application is not required to
    interpret it in any way. It is good practice, however, to recognize it
    as a noncharacter and to take appropriate action, such as removing it
    from the text. Note that Unicode conformance freely allows the removal
    of these characters."

    "In effect, noncharacters can be thought of as application-internal
    private-use code points."

*/

#define LDB_KEY_0           0xFDD0
#define LDB_KEY_1           0xFDD1
#define LDB_KEY_2           0xFDD2
#define LDB_KEY_3           0xFDD3
#define LDB_KEY_4           0xFDD4
#define LDB_KEY_5           0xFDD5
#define LDB_KEY_6           0xFDD6
#define LDB_KEY_7           0xFDD7
#define LDB_KEY_8           0xFDD8
#define LDB_KEY_9           0xFDD9

#define LDB_KEY_STAR        0xFDDA
#define LDB_KEY_POUND     0xFDDB

#define LDB_KEY_10          0xFDDA
#define LDB_KEY_11          0xFDDB
#define LDB_KEY_12          0xFDDC
#define LDB_KEY_13          0xFDDD
#define LDB_KEY_14          0xFDDE
#define LDB_KEY_15          0xFDDF

/* Flag constants */

#define LDB_LOWER_CASE      0x01
#define LDB_UPPER_CASE      0x02
#define LDB_INITIAL_CASE    0x04
#define LDB_EUDW            0x10

/* Engine flags */
#define LDB_USE_KEY_STAR    0x01
#define LDB_USE_KEY_POUND 0x02

/* Thai character types */

#define LDB_THAI_OOV        0x01    /* None-thai */
#define LDB_THAI_IDV        0x02    /* Independent vowels */
#define LDB_THAI_C          0x04    /* Consonants */
#define LDB_THAI_DV         0x08    /* Dependent vowels */
#define LDB_THAI_TM         0x10    /* Tone marks */
#define LDB_THAI_S          0x20    /* Signs */
#define LDB_THAI_JS         0x40    /* Joined Signs */

/* Hindi character types */

#define LDB_HINDI_OOV       0       /* Not a Hindi letter   */
#define LDB_HINDI_CON       1       /* Consonants           */
#define LDB_HINDI_INV       2       /* Independent vowels   */
#define LDB_HINDI_DEV       3       /* Dependent vowels     */
#define LDB_HINDI_VIR       4       /* Virama sign          */
#define LDB_HINDI_SIG       5       /* Other signs          */

/* Nepali character types */

#define LDB_NEPALI_OOV       0       /* Not a Nepali letter   */
#define LDB_NEPALI_CON       1       /* Consonants           */
#define LDB_NEPALI_INV       2       /* Independent vowels   */
#define LDB_NEPALI_DEV       3       /* Dependent vowels     */
#define LDB_NEPALI_VIR       4       /* Virama sign          */
#define LDB_NEPALI_SIG       5       /* Other signs          */

/* Sinhala character types */

#define LDB_SINHALA_OOV       0       /* Not a Sinhala letter   */
#define LDB_SINHALA_CON       1       /* Consonants           */
#define LDB_SINHALA_INV       2       /* Independent vowels   */
#define LDB_SINHALA_DEV       3       /* Dependent vowels     */
#define LDB_SINHALA_VIR       4       /* Virama sign          */
#define LDB_SINHALA_SIG       5       /* Other signs          */

//daodejing need
/* Bodo character types */
#define LDB_BODO_OOV       0       /* Not a Bodo letter   */
#define LDB_BODO_CON       1       /* Consonants           */
#define LDB_BODO_INV       2       /* Independent vowels   */
#define LDB_BODO_DEV       3       /* Dependent vowels     */
#define LDB_BODO_VIR       4       /* Virama sign          */
#define LDB_BODO_SIG       5       /* Other signs          */

/* Dogri character types */
#define LDB_DOGRI_OOV       0       /* Not a Dogri letter   */
#define LDB_DOGRI_CON       1       /* Consonants           */
#define LDB_DOGRI_INV       2       /* Independent vowels   */
#define LDB_DOGRI_DEV       3       /* Dependent vowels     */
#define LDB_DOGRI_VIR       4       /* Virama sign          */
#define LDB_DOGRI_SIG       5       /* Other signs          */

/* Sanskrit character types */
#define LDB_SANSKRIT_OOV       0       /* Not a Sanskrit letter   */
#define LDB_SANSKRIT_CON       1       /* Consonants           */
#define LDB_SANSKRIT_INV       2       /* Independent vowels   */
#define LDB_SANSKRIT_DEV       3       /* Dependent vowels     */
#define LDB_SANSKRIT_VIR       4       /* Virama sign          */
#define LDB_SANSKRIT_SIG       5       /* Other signs          */

/* Kashmiri character types */
#define LDB_KASHMIRI_OOV       0       /* Not a Kashmiri letter   */
#define LDB_KASHMIRI_CON       1       /* Consonants           */
#define LDB_KASHMIRI_INV       2       /* Independent vowels   */
#define LDB_KASHMIRI_DEV       3       /* Dependent vowels     */
#define LDB_KASHMIRI_VIR       4       /* Virama sign          */
#define LDB_KASHMIRI_SIG       5       /* Other signs          */

/* Konkani character types */
#define LDB_KONKANI_OOV       0       /* Not a Konkani letter   */
#define LDB_KONKANI_CON       1       /* Consonants           */
#define LDB_KONKANI_INV       2       /* Independent vowels   */
#define LDB_KONKANI_DEV       3       /* Dependent vowels     */
#define LDB_KONKANI_VIR       4       /* Virama sign          */
#define LDB_KONKANI_SIG       5       /* Other signs          */

/* Maithili character types */
#define LDB_MAITHILI_OOV       0       /* Not a Maithili letter   */
#define LDB_MAITHILI_CON       1       /* Consonants           */
#define LDB_MAITHILI_INV       2       /* Independent vowels   */
#define LDB_MAITHILI_DEV       3       /* Dependent vowels     */
#define LDB_MAITHILI_VIR       4       /* Virama sign          */
#define LDB_MAITHILI_SIG       5       /* Other signs          */

/* Manipuri character types */
#define LDB_MANIPURI_OOV       0       /* Not a Manipuri letter   */
#define LDB_MANIPURI_CON       1       /* Consonants           */
#define LDB_MANIPURI_INV       2       /* Independent vowels   */
#define LDB_MANIPURI_DEV       3       /* Dependent vowels     */
#define LDB_MANIPURI_VIR       4       /* Virama sign          */
#define LDB_MANIPURI_SIG       5       /* Other signs          */

/* Santali character types */
#define LDB_SANTALI_OOV       0       /* Not a Santali letter   */
#define LDB_SANTALI_CON       1       /* Consonants           */
#define LDB_SANTALI_INV       2       /* Independent vowels   */
#define LDB_SANTALI_DEV       3       /* Dependent vowels     */
#define LDB_SANTALI_VIR       4       /* Virama sign          */
#define LDB_SANTALI_SIG       5       /* Other signs          */

/* Somali character types */
#define LDB_SOMALI_OOV       0       /* Not a Somali letter   */
#define LDB_SOMALI_CON       1       /* Consonants           */
#define LDB_SOMALI_INV       2       /* Independent vowels   */
#define LDB_SOMALI_DEV       3       /* Dependent vowels     */
#define LDB_SOMALI_VIR       4       /* Virama sign          */
#define LDB_SOMALI_SIG       5       /* Other signs          */

/* Sindhi character types */
#define LDB_SINDHI_OOV       0       /* Not a Sindhi letter   */
#define LDB_SINDHI_CON       1       /* Consonants           */
#define LDB_SINDHI_INV       2       /* Independent vowels   */
#define LDB_SINDHI_DEV       3       /* Dependent vowels     */
#define LDB_SINDHI_VIR       4       /* Virama sign          */
#define LDB_SINDHI_SIG       5       /* Other signs          */

/* Bengali character types */
#define LDB_BENGALI_OOV       0       /* Not a Bengali letter */
#define LDB_BENGALI_CON       1       /* Consonants           */
#define LDB_BENGALI_INV       2       /* Independent vowels   */
#define LDB_BENGALI_DEV       3       /* Dependent vowels     */
#define LDB_BENGALI_VIR       4       /* Virama sign          */
#define LDB_BENGALI_SIG       5       /* Other signs          */
#define LDB_BENGALI_NUK       6       /* Nukta sign for extending consonant */

/* Myanmar character types */
#define LDB_MYANMAR_OOV       0       /* Not a Myanmar letter */
#define LDB_MYANMAR_CON       1       /* Consonants           */
#define LDB_MYANMAR_INV       2       /* Independent vowels   */
#define LDB_MYANMAR_DEV       3       /* Dependent vowels     */
#define LDB_MYANMAR_VIR       4       /* Virama sign          */
#define LDB_MYANMAR_SIG       5       /* Other signs          */
#define LDB_MYANMAR_ASAT      6       /* Asat sign            */
#define LDB_MYANMAR_SPS       7       /* Special signs        */
#define LDB_MYANMAR_DEC       8       /* Dependent consonants */

/* Tibetan character types */
#define LDB_TIBETAN_OOV       0       /* Not a Tibetan letter */
#define LDB_TIBETAN_CON       1       /* Consonants */
#define LDB_TIBETAN_JOC       2       /* Subjoined Consonants  */
#define LDB_TIBETAN_DEV       3       /* Dependent vowels */
#define LDB_TIBETAN_VIR       4       /* Virama sign */
#define LDB_TIBETAN_SIG       5       /* Special signs */


/* Types and structures */

typedef unsigned char       LDB_BYTE;
typedef unsigned short      LDB_WORD;
typedef unsigned short      LDB_CHAR;

#define IS_SURE_TYPE(did)   ( (did) >= 10000 ? 1 : 0 )
/* Start From Russian, Extend DID range [ilME_LANG * 100, (ilME_LANG) + 1 * 100) */
#define IS_LATIN_EXTEND_DID(did) ( ((did) >= 2000 && (did) < 10000) ? 1 : 0 )
#define LATIN_EXTEND_DID_TO_LANG(did) ( (did) / 100 )

typedef struct tagLDB_INFO
{
    int                     language;
    int                     database_id;
    int                     is_sure_type;
    int                     major_version;
    int                     minor_version;
    int                     build_number;
    const LDB_CHAR*         copyright;
    int                     copyright_length;
    int                     engine_object_size;
    int                     max_input_length;
    const LDB_CHAR*         eudw_prompt;        /* Prompt for eudw candidate */
    int                     eudw_prompt_length;
    const LDB_CHAR*         eudw_title;         /* Prompt for eudw input box */
    int                     eudw_title_length;
    int                     mt_engine_object_size;
} LDB_INFO;

typedef struct tagLDB_INIT
{
    const void*             database;
    void*                   user_data;
    int                     user_data_size;
} LDB_INIT;

/* Database functions */

int ldbapi4_get_db_info(
    IN const void*          database,
    OUT LDB_INFO*           info);

const LDB_CHAR* ldbapi4_get_db_multitype(
    IN const void*          database,
    IN LDB_CHAR             key,
    IN int                  flags,
    OUT int*                count);

int ldbapi4_ldb_is_sou_delimiter(
    IN const void*          database,
    IN LDB_CHAR             c);

int ldbapi4_is_key_star_used(IN const void* database);

int ldbapi4_is_key_pound_used(IN const void* database);

/* Engine functions */

int ldbapi4_initialize(
    IN void*                engobj,
    IN const LDB_INIT*      init);

void ldbapi4_cleanup(
    IN void*                engobj);

int ldbapi4_mt_initialize(
    IN void*                engobj,
    IN const void*          database);

void ldbapi4_mt_cleanup(
    IN void*                engobj);

void ldbapi4_get_info(
    IN void*                engobj,
    OUT LDB_INFO*           info);

const LDB_CHAR* ldbapi4_get_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN int                  flags,
    OUT int*                count);

LDB_CHAR ldbapi4_char_to_key(
    IN void*                engobj,
    IN LDB_CHAR             c);

int ldbapi4_match(
    IN void*                engobj,
    IN const LDB_CHAR*      context_before_string,
    IN int                  context_before_length,
    IN const LDB_CHAR*      context_after_string,
    IN int                  context_after_length,
    IN const LDB_CHAR*      input_string,
    IN int                  input_length,
    IN int                  flags,
    IN int                  max_match);

const LDB_CHAR* ldbapi4_get_match(
    IN void*                engobj,
    IN int                  match_index,
    OUT int*                length);

void ldbapi4_reset_user_data(
    IN void*                engobj);

void ldbapi4_update_user_data(
    IN void*                engobj,
    IN const LDB_CHAR*      history_string,
    IN int                  history_length,
    IN int                  flags);

int ldbapi4_is_sou_delimiter(
    IN void*                engobj,
    IN LDB_CHAR             c);

void ldbapi4_string_convert_case(
    IN void*                engobj,
    IN OUT LDB_CHAR*        string,
    IN int                  length,
    IN int                  flags);

int ldbapi4_check_case(
    IN void*                engobj,
    IN const LDB_CHAR*      string,
    IN int                  length);

/* Thai multitap lexcial rule handling APIs */

unsigned char ldbapi4_get_thai_char_type(
    IN LDB_CHAR             c);

int ldbapi4_is_valid_thai_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2);

int ldbapi4_is_valid_thai_word_start_char(
    IN LDB_CHAR             c);

const LDB_CHAR* ldbapi4_get_thai_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count);

/* Hindi multitap lexcial rule handling APIs */

unsigned char ldbapi4_get_hindi_char_type(
    IN LDB_CHAR             c);

int ldbapi4_is_valid_hindi_word_start_char(
    IN LDB_CHAR             c);

int ldbapi4_is_valid_hindi_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2);

const LDB_CHAR* ldbapi4_get_hindi_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count);

/* Bengali multitap lexcial rule handling APIs */
unsigned char ldbapi4_get_bengali_char_type(
    IN LDB_CHAR             c);

int ldbapi4_is_valid_bengali_word_start_char(
    IN LDB_CHAR             c);

int ldbapi4_is_valid_bengali_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2);

const LDB_CHAR* ldbapi4_get_bengali_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count);
    

/* Tibetan multitap lexcial rule handling APIs */
unsigned char ldbapi4_get_tibetan_char_type(
    IN LDB_CHAR             c);

int ldbapi4_is_valid_tibetan_word_start_char(
    IN LDB_CHAR             c);

int ldbapi4_is_valid_tibetan_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2);

const LDB_CHAR* ldbapi4_get_tibetan_multitype(
    IN void*                engobj,
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count);
    
LDB_CHAR ldbapi4_tibetan_consonant_to_subjoined(
    IN LDB_CHAR             c);
    
LDB_CHAR ldbapi4_tibetan_subjoined_to_consonant(
    IN LDB_CHAR             c);
    
/* Hangul multitap  */
const LDB_CHAR* ldbapi4_get_hangul_multitype(
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count,
    OUT int*                has_valid_comp);


int ldbapi4_get_hangul_multitype_ex(
        IN LDB_CHAR				key,
		IN LDB_CHAR             previous_char,
		OUT LDB_CHAR*           commit_char,
		OUT LDB_CHAR*           comp_char,
        OUT LDB_CHAR*           prev_comp_char);

/* Myanmar multitap */
int ldbapi4_is_valid_myanmar_follow_char(
    IN LDB_CHAR             c1,
    IN LDB_CHAR             c2);

const LDB_CHAR* ldbapi4_get_myanmar_multitype(
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
    OUT int*                count);

/* Indic dummy */
int ldbapi4_is_indic_dummy_did(
    IN int                          did);

#ifdef __cplusplus
}
#endif

#endif /* LDBAPI4_H */
