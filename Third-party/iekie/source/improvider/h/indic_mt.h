/****************************************************************************
 *
 * indic_mt.h
 *
 */

#ifndef __INDIC_MT_H__
#define __INDIC_MT_H__

#define MAX_MULTITAPS 10

typedef struct tagINDIC_DUMMY_DB
{
    int did;
    const LDB_CHAR (*multitap_string_1)[32];
    const LDB_CHAR (*multitap_string_2)[32];
    const LDB_CHAR (*multitap_string_3)[32];
    const LDB_CHAR (*multitap_string_4)[32];
    const LDB_CHAR (*multitap_string_5)[32];
}INDIC_DUMMY_DB;

typedef enum
{
	  /* DEVANAGARI SIGNS */
    TELUGU_DEVANAGARI_S_DANDA		= 0x0964,
	TELUGU_DEVANAGARI_S_DDANDA		= 0x0965,

    /* VARIOUS SIGNS */

    TELUGU_S_CHANDRA_BINDU = 0xc01,
    TELUGU_S_ANUSVARA = 0xc02,
    TELUGU_S_VISARGA = 0xc03,

    /* INDEPENDENT VOWEL */

    TELUGU_IV_A = 0xc05,
    TELUGU_IV_AA = 0xc06,
    TELUGU_IV_I = 0xc07,
    TELUGU_IV_II = 0xc08,
    TELUGU_IV_U = 0xc09,
    TELUGU_IV_UU = 0xc0a,
    TELUGU_IV_VR = 0xc0b,
    TELUGU_IV_VL = 0xc0c,
    TELUGU_IV_E = 0xc0e,
    TELUGU_IV_EE = 0xc0f,
    TELUGU_IV_AI = 0xc10,
    TELUGU_IV_O = 0xc12,
    TELUGU_IV_OO = 0xc13,
    TELUGU_IV_AU = 0xc14,

    /*  CONSONANTS  */

    TELUGU_C_KA = 0xc15,
    TELUGU_C_KHA = 0xc16,
    TELUGU_C_GA = 0xc17,
    TELUGU_C_GHA = 0xc18,
    TELUGU_C_NGA = 0xc19,
    TELUGU_C_CA = 0xc1a,
    TELUGU_C_CHA = 0xc1b,
    TELUGU_C_JA = 0xc1c,
    TELUGU_C_JHA = 0xc1d,
    TELUGU_C_NYA = 0xc1e,
    TELUGU_C_TTA = 0xc1f,
    TELUGU_C_TTHA = 0xc20,
    TELUGU_C_DDA = 0xc21,
    TELUGU_C_DDHA = 0xc22,
    TELUGU_C_NNA = 0xc23,
    TELUGU_C_TA = 0xc24,
    TELUGU_C_THA = 0xc25,
    TELUGU_C_DA = 0xc26,
    TELUGU_C_DHA = 0xc27,
    TELUGU_C_NA = 0xc28,
    TELUGU_C_PA = 0xc2a,
    TELUGU_C_PHA = 0xc2b,
    TELUGU_C_BA = 0xc2c,
    TELUGU_C_BHA = 0xc2d,
    TELUGU_C_MA = 0xc2e,
    TELUGU_C_YA = 0xc2f,
    TELUGU_C_RA = 0xc30,
    TELUGU_C_RRA = 0xc31,
    TELUGU_C_LA = 0xc32,
    TELUGU_C_LLA = 0xc33,
    TELUGU_C_VA = 0xc35,
    TELUGU_C_SHA = 0xc36,
    TELUGU_C_SSA = 0xc37,
    TELUGU_C_SA = 0xc38,
    TELUGU_C_HA = 0xc39,

    /*  DEPENDENT VOWELS  */

    TELUGU_DV_AA = 0xc3e,
    TELUGU_DV_I = 0xc3f,
    TELUGU_DV_II = 0xc40,
    TELUGU_DV_U = 0xc41,
    TELUGU_DV_UU = 0xc42,
    TELUGU_DV_VR = 0xc43,
    TELUGU_DV_VRR = 0xc44,
    TELUGU_DV_E = 0xc46,
    TELUGU_DV_EE = 0xc47,
    TELUGU_DV_AI = 0xc48,
    TELUGU_DV_O = 0xc4a,
    TELUGU_DV_OO = 0xc4b,
    TELUGU_DV_AU = 0xc4c,
    TELUGU_S_HALANT = 0xc4d,
    TELUGU_ADV_EE = 0xc55,
    TELUGU_ADV_AI = 0xc56,
    TELUGU_IV_VRR = 0xc60,
    TELUGU_IV_VLL = 0xc61,

    TELUGU_S_DANDA = 0xc64,
    TELUGU_S_DDANDA = 0xc65,
    /*  DIGITS  */

    TELUGU_D_ZERO = 0xc66,
    TELUGU_D_ONE = 0xc67,
    TELUGU_D_TWO = 0xc68,
    TELUGU_D_THREE = 0xc69,
    TELUGU_D_FOUR = 0xc6a,
    TELUGU_D_FIVE = 0xc6b,
    TELUGU_D_SIX = 0xc6c,
    TELUGU_D_SEVEN = 0xc6d,
    TELUGU_D_EIGHT = 0xc6e,
    TELUGU_D_NINE = 0xc6f

} telugu_characters_enum;

typedef enum
{   /* VARIOUS SIGNS + INDEPENDENT VOWEL */
    MARATHI_S_CHANDRA_BINDU		= 0x0901,
    MARATHI_S_BINDU				= 0x0902,
    MARATHI_S_VISARGA			= 0x0903,
    MARATHI_NIV_A				= 0x0904,
    MARATHI_IV_A				= 0x0905,
    MARATHI_IV_AA				= 0x0906,
    MARATHI_IV_I				= 0x0907,
    MARATHI_IV_II				= 0x0908,
    MARATHI_IV_U				= 0x0909,
    MARATHI_IV_UU				= 0x090a,
    MARATHI_IV_R				= 0x090b,
    MARATHI_NIV_L				= 0x090c,
    MARATHI_NIV_CE				= 0x090d,
    MARATHI_NIV_SE				= 0x090e,
    MARATHI_IV_E				= 0x090f,
    MARATHI_IV_AI				= 0x0910,
    MARATHI_NIV_CO				= 0x0911,
    MARATHI_NIV_SO				= 0x0912,
    MARATHI_IV_O				= 0x0913,
    MARATHI_IV_AU				= 0x0914,

    /* CONSONANTS */
    MARATHI_C_KA				= 0x0915,
    MARATHI_C_KHA				= 0x0916,
    MARATHI_C_GA				= 0x0917,
    MARATHI_C_GHA				= 0x0918,
    MARATHI_C_NGA				= 0x0919,
    MARATHI_C_CHA				= 0x091a,
    MARATHI_C_CHHA				= 0x091b,
    MARATHI_C_JA				= 0x091c,
    MARATHI_C_JHA				= 0x091d,
    MARATHI_C_NYA				= 0x091e,
    MARATHI_C_TTA				= 0x091f,
    MARATHI_C_TTHA				= 0x0920,
    MARATHI_C_DDA				= 0x0921,
    MARATHI_C_DDHA				= 0x0922,
    MARATHI_C_NNA				= 0x0923,
    MARATHI_C_TA				= 0x0924,
    MARATHI_C_THA				= 0x0925,
    MARATHI_C_DA				= 0x0926,
    MARATHI_C_DHA				= 0x0927,
    MARATHI_C_NA				= 0x0928,
    MARATHI_C_NNNA				= 0x0929,
    MARATHI_C_PA				= 0x092a,
    MARATHI_C_PHA				= 0x092b,
    MARATHI_C_BA				= 0x092c,
    MARATHI_C_BHA				= 0x092d,
    MARATHI_C_MA				= 0x092e,
    MARATHI_C_YA				= 0x092f,
    MARATHI_C_RA				= 0x0930,
    MARATHI_C_RRA				= 0x0931,
    MARATHI_C_LA				= 0x0932,
    MARATHI_C_LLA				= 0x0933,
    MARATHI_C_LLLA				= 0x0934,
    MARATHI_C_VA				= 0x0935,
    MARATHI_C_SHA				= 0x0936,
    MARATHI_C_SSA				= 0x0937,
    MARATHI_C_SA				= 0x0938,
    MARATHI_C_HA				= 0x0939,
    MARATHI_N_BLANK1			= 0x093a,
    MARATHI_N_BLANK2			= 0x093b,

    /* SIGNS */
    MARATHI_S_NUKTA				= 0x093c,
    MARATHI_NS_AVAGRAHA			= 0x093d,

    /* DEPENDENT VOWELS */
    MARATHI_DV_AA				= 0x093e,
    MARATHI_DV_I				= 0x093f,
    MARATHI_DV_II				= 0x0940,
    MARATHI_DV_U				= 0x0941,
    MARATHI_DV_UU				= 0x0942,
    MARATHI_DV_R				= 0x0943,
    MARATHI_NDV_RR				= 0x0944,
    MARATHI_DV_CE				= 0x0945,
    MARATHI_NDV_E				= 0x0946,
    MARATHI_DV_E				= 0x0947,
    MARATHI_DV_AI				= 0x0948,
    MARATHI_NDV_CO				= 0x0949,
    MARATHI_NDV_O				= 0x094a,
    MARATHI_DV_O				= 0x094b,
    MARATHI_DV_AU				= 0x094c,

    /* SIGNS */
    MARATHI_S_HALANT			= 0x094d,
    MARATHI_NS_RESERVED1		= 0x094e,
    MARATHI_NS_RESERVED2		= 0x094f,
    MARATHI_S_OM				= 0x0950,
    MARATHI_NS_UDATTA			= 0x0951,
    MARATHI_NS_ANUDATTA			= 0x0952,
    MARATHI_NS_GACCENT			= 0x0953,
    MARATHI_NS_AACCENT			= 0x0954,
    MARATHI_N_BLANK5			= 0x0955,
    MARATHI_N_BLANK6			= 0x0956,
    MARATHI_N_BLANK7			= 0x0957,

    /* ADDITIONAL CONSONANT */
    MARATHI_AC_QA				= 0x0958,
    MARATHI_AC_KHHA				= 0x0959,
    MARATHI_AC_GHHA				= 0x095a,
    MARATHI_AC_ZA				= 0x095b,
    MARATHI_AC_DDDHA			= 0x095c,
    MARATHI_AC_RHA				= 0x095d,
    MARATHI_AC_FA				= 0x095e,
    MARATHI_AC_YYA				= 0x095f,

    /* GENERIC ADDITIONS */
    MARATHI_NG_RR				= 0x0960,
    MARATHI_NG_LL				= 0x0961,
    MARATHI_NG_L				= 0x0962,
    MARATHI_NG_VLL				= 0x0963,
    MARATHI_NG_D				= 0x0964,
    MARATHI_NG_DD				= 0x0965,

    /* DIGITS */
    MARATHI_D_ZERO				= 0x0966,
    MARATHI_D_ONE				= 0x0967,
    MARATHI_D_TWO				= 0x0968,
    MARATHI_D_THREE				= 0x0969,
    MARATHI_D_FOUR				= 0x096a,
    MARATHI_D_FIVE				= 0x096b,
    MARATHI_D_SIX				= 0x096c,
    MARATHI_D_SEVEN				= 0x096d,
    MARATHI_D_EIGHT				= 0x096e,
    MARATHI_D_NINE				= 0x096f,
    /* DEVANAGARI SPECIFIC ADDITOIN */
    MARATHI_S_ABBR				= 0x0970
}marathi_characters_enum;

typedef enum
{
	/* DEVANAGARI SIGNS */
	TAMIL_DEVANAGARI_S_DANDA		= 0x0964,
	TAMIL_DEVANAGARI_S_DDANDA		= 0x0965,
	/* VARIOUS SIGNS + INDEPENDENT VOWEL */
    TAMIL_S_ANUSVARA = 0xb82,
    TAMIL_S_VISARGA = 0xb83,
    TAMIL_IV_A = 0xb85,
    TAMIL_IV_AA = 0xb86,
    TAMIL_IV_I = 0xb87,
    TAMIL_IV_II = 0xb88,
    TAMIL_IV_U = 0xb89,
    TAMIL_IV_UU = 0xb8a,

    TAMIL_IV_E = 0xb8e,
    TAMIL_IV_EE = 0xb8f,
    TAMIL_IV_AI = 0xb90,
    TAMIL_IV_O = 0xb92,
    TAMIL_IV_OO = 0xb93,
    TAMIL_IV_AU = 0xb94,

    /* CONSONANTS */
    TAMIL_C_KA = 0xb95,
    TAMIL_C_NGA = 0xb99,
    TAMIL_C_CHA = 0xb9a,
    TAMIL_C_JA = 0xb9c,
    TAMIL_C_NYA = 0xb9e,
    TAMIL_C_TTA = 0xb9f,
    TAMIL_C_NNA = 0xba3,
    TAMIL_C_TA = 0xba4,
    TAMIL_C_NA = 0xba8,
    TAMIL_C_NNNA = 0xba9,
    TAMIL_C_PA = 0xbaa,
    TAMIL_C_MA = 0xbae,
    TAMIL_C_YA = 0xbaf,
    TAMIL_C_RA = 0xbb0,
    TAMIL_C_RRA = 0xbb1,
    TAMIL_C_LA = 0xbb2,
    TAMIL_C_LLA = 0xbb3,
    TAMIL_C_LLLA = 0xbb4,
    TAMIL_C_VA = 0xbb5,
    TAMIL_C_SSA = 0xbb7,
    TAMIL_C_SA = 0xbb8,
    TAMIL_C_HA = 0xbb9,

    /* DEPENDENT VOWELS */
    TAMIL_DV_AA = 0xbbe,
    TAMIL_DV_I = 0xbbf,
    TAMIL_DV_II = 0xbc0,
    TAMIL_DV_U = 0xbc1,
    TAMIL_DV_UU = 0xbc2,
    TAMIL_DV_E = 0xbc6,
    TAMIL_DV_EE = 0xbc7,
    TAMIL_DV_AI = 0xbc8,
    TAMIL_DV_O = 0xbca,
    TAMIL_DV_OO = 0xbcb,
    TAMIL_DV_AU = 0xbcc,

    /* SIGNS */
    TAMIL_S_HALANT = 0xbcd,

    /* APPEND DEPENDENT VOWEL */
    TAMIL_AD_AU = 0xbd7,

    TAMIL_S_DANDA = 0xbe4,
    TAMIL_S_DDANDA = 0xbe5,
    /* DIGITS */
    TAMIL_D_ZERO = 0xbe6,
    TAMIL_D_ONE = 0xbe7,
    TAMIL_D_TWO = 0xbe8,
    TAMIL_D_THREE = 0xbe9,
    TAMIL_D_FOUR = 0xbea,
    TAMIL_D_FIVE = 0xbeb,
    TAMIL_D_SIX = 0xbec,
    TAMIL_D_SEVEN = 0xbed,
    TAMIL_D_EIGHT = 0xbee,
    TAMIL_D_NINE = 0xbef,

    /* TAMIL NUMERIiIME */
    TAMIL_D_TEN = 0xbf0,
    TAMIL_D_HUNDRED = 0xbf1,
    TAMIL_D_THOUSAND = 0xbf2,

    /* TAMIL SYMBOLS */
    TAMIL_OS_DAY = 0xbf3,
    TAMIL_OS_MONTH = 0xbf4,
    TAMIL_OS_YEAR = 0xbf5,
    TAMIL_OS_DEBIT = 0xbf6,
    TAMIL_OS_CREDIT = 0xbf7,
    TAMIL_OS_ABOVE = 0xbf8,

    /* CURRENCY SYMBOL */
    TAMIL_OS_RUPEE = 0xbf9,

    /* TAMIL SYMBOLS */
    TAMIL_OS_NUMERNAL = 0xbfa
} tamil_characters_enum;

typedef enum
{
    /* DEVANAGARI SIGNS */
	PUNJABI_DEVANAGARI_S_DANDA			= 0x0964,
	PUNJABI_DEVANAGARI_S_DOUBLE_DANDA		= 0x0965,

	/* VARIOUS SIGNS + INDEPENDENT VOWEL */
    PUNJABI_S_ADAK_BINDI = 0x0A01,
    PUNJABI_S_BINDI = 0x0A02,
    PUNJABI_S_VISARGA = 0x0A03,
    PUNJABI_IV_A = 0x0A05,
    PUNJABI_IV_AA = 0x0A06,
    PUNJABI_IV_I = 0x0A07,
    PUNJABI_IV_II = 0x0A08,
    PUNJABI_IV_U = 0x0A09,
    PUNJABI_IV_UU = 0x0A0a,
    PUNJABI_IV_EE = 0x0A0f,
    PUNJABI_IV_AI = 0x0A10,
    PUNJABI_IV_OO = 0x0A13,
    PUNJABI_IV_AU = 0x0A14,

    /* CONSONANTS */
    PUNJABI_C_KA = 0x0A15,
    PUNJABI_C_KHA = 0x0A16,
    PUNJABI_C_GA = 0x0A17,
    PUNJABI_C_GHA = 0x0A18,
    PUNJABI_C_NGA = 0x0A19,
    PUNJABI_C_CA = 0x0A1a,
    PUNJABI_C_CHA = 0x0A1b,
    PUNJABI_C_JA = 0x0A1c,
    PUNJABI_C_JHA = 0x0A1d,
    PUNJABI_C_NYA = 0x0A1e,
    PUNJABI_C_TTA = 0x0A1f,
    PUNJABI_C_TTHA = 0x0A20,
    PUNJABI_C_DDA = 0x0A21,
    PUNJABI_C_DDHA = 0x0A22,
    PUNJABI_C_NNA = 0x0A23,
    PUNJABI_C_TA = 0x0A24,
    PUNJABI_C_THA = 0x0A25,
    PUNJABI_C_DA = 0x0A26,
    PUNJABI_C_DHA = 0x0A27,
    PUNJABI_C_NA = 0x0A28,
    PUNJABI_C_PA = 0x0A2a,
    PUNJABI_C_PHA = 0x0A2b,
    PUNJABI_C_BA = 0x0A2c,
    PUNJABI_C_BHA = 0x0A2d,
    PUNJABI_C_MA = 0x0A2e,
    PUNJABI_C_YA = 0x0A2f,
    PUNJABI_C_RA = 0x0A30,
    PUNJABI_C_LA = 0x0A32,
    PUNJABI_C_LLA = 0x0A33,
    PUNJABI_C_VA = 0x0A35,
    PUNJABI_C_SHA = 0x0A36,
    PUNJABI_C_SA = 0x0A38,
    PUNJABI_C_HA = 0x0A39,

    /* SIGNS */
    PUNJABI_S_NUKTA = 0x0A3c,

    /* DEPENDENT VOWELS */
    PUNJABI_DV_AA = 0x0A3e,
    PUNJABI_DV_I = 0x0A3f,
    PUNJABI_DV_II = 0x0A40,
    PUNJABI_DV_U = 0x0A41,
    PUNJABI_DV_UU = 0x0A42,
    PUNJABI_DV_EE = 0x0A47,
    PUNJABI_DV_AI = 0x0A48,
    PUNJABI_DV_OO = 0x0A4b,
    PUNJABI_DV_AU = 0x0A4c,

    /* SIGNS */
    PUNJABI_S_HALANT = 0x0A4d,
    PUNJABI_S_UDAAT = 0x0A51,

    /* ADDITIONAL CONSONANT */
    PUNJABI_AC_KHHA = 0x0A59,
    PUNJABI_AC_GHHA = 0x0A5a,
    PUNJABI_AC_ZA = 0x0A5b,
    PUNJABI_AC_RRA = 0x0A5c,
    PUNJABI_AC_FA = 0x0A5e,
    PUNJABI_S_DANDA = 0x0A64,
	PUNJABI_S_DOUBLE_DANDA = 0x0A65,
    
    /* DIGITS */
    PUNJABI_D_ZERO = 0x0A66,
    PUNJABI_D_ONE = 0x0A67,
    PUNJABI_D_TWO = 0x0A68,
    PUNJABI_D_THREE = 0x0A69,
    PUNJABI_D_FOUR = 0x0A6a,
    PUNJABI_D_FIVE = 0x0A6b,
    PUNJABI_D_SIX = 0x0A6c,
    PUNJABI_D_SEVEN = 0x0A6d,
    PUNJABI_D_EIGHT = 0x0A6e,
    PUNJABI_D_NINE = 0x0A6f,

    /* PUNJABI SPECIFIC ADDITOIN */
    PUNJABI_AC_TIPPI = 0x0A70,
    PUNJABI_AC_ADDAK = 0x0A71,
    PUNJABI_AC_IRI = 0x0A72,
    PUNJABI_AC_URA = 0x0A73,
    PUNJABI_AC_ONKAR = 0x0A74,
    
    /* SIGN */
    PUNJABI_S_YAKASH = 0x0A75,
    
    /* END OF ENUM */
	PUNJABI_DUMMY_END
} punjabi_characters_enum;

typedef enum
{
	  /* DEVANAGARI SIGNS */
		GUJARATI_DEVANAGARI_S_DANDA		= 0x0964,
		GUJARATI_DEVANAGARI_S_DDANDA		= 0x0965,
		
	/* VARIOUS SIGNS + INDEPENDENT VOWEL */

    GUJARATI_S_CHANDRA_BINDU = 0xa81,
    GUJARATI_S_BINDU         = 0xa82,
    GUJARATI_S_VISARGA       = 0xa83,
	GUJARATI_IV_A		     =	0xa85,
	GUJARATI_IV_AA		     =	0xa86,
	GUJARATI_IV_I		     =	0xa87,
	GUJARATI_IV_II		     =	0xa88,
	GUJARATI_IV_U		     =	0xa89,
	GUJARATI_IV_UU		     =	0xa8a,
	GUJARATI_IV_VR		     =	0xa8b,
	GUJARATI_IV_VL		     =	0xa8c,
	GUJARATI_IV_CE		     =	0xa8d,
	GUJARATI_IV_E		     =	0xa8f,
	GUJARATI_IV_AI		     =	0xa90,
	GUJARATI_IV_CO		     =	0xa91,
	GUJARATI_IV_O		     =	0xa93,
	GUJARATI_IV_AU		     =	0xa94,

	/*  CONSONANTS  */

	GUJARATI_C_KA		     =	0xa95,
	GUJARATI_C_KHA		     =	0xa96,
	GUJARATI_C_GA		     =	0xa97,
	GUJARATI_C_GHA		     =	0xa98,
	GUJARATI_C_NGA		     =	0xa99,
	GUJARATI_C_CA		     =	0xa9a,
	GUJARATI_C_CHA		     =	0xa9b,
	GUJARATI_C_JA		     =	0xa9c,
	GUJARATI_C_JHA		     =	0xa9d,
	GUJARATI_C_NYA		     =	0xa9e,
	GUJARATI_C_TTA		     =	0xa9f,
	GUJARATI_C_TTHA		     =	0xaa0,
	GUJARATI_C_DDA		     =	0xaa1,
	GUJARATI_C_DDHA		     =	0xaa2,
	GUJARATI_C_NNA		     =	0xaa3,
	GUJARATI_C_TA		     =	0xaa4,
	GUJARATI_C_THA		     =	0xaa5,
	GUJARATI_C_DA		     =	0xaa6,
	GUJARATI_C_DHA		     =	0xaa7,
	GUJARATI_C_NA		     =	0xaa8,
	GUJARATI_C_PA		     =	0xaaa,
	GUJARATI_C_PHA		     =	0xaab,
	GUJARATI_C_BA		     =	0xaac,
	GUJARATI_C_BHA		     =	0xaad,
	GUJARATI_C_MA		     =	0xaae,
	GUJARATI_C_YA		     =	0xaaf,
	GUJARATI_C_RA		     =	0xab0,
	GUJARATI_C_LA		     =	0xab2,
	GUJARATI_C_LLA		     =	0xab3,
	GUJARATI_C_VA		     =	0xab5,
	GUJARATI_C_SHA		     =	0xab6,
	GUJARATI_C_SSA		     =	0xab7,
	GUJARATI_C_SA		     =	0xab8,
	GUJARATI_C_HA		     =	0xab9,

	GUJARATI_S_NUKTA		     =	0xabc,
	GUJARATI_S_AVAGRAHA		     =	0xabd,

	/*  DEPENDENT VOWELS  */

	GUJARATI_DV_AA		     =	0xabe,
	GUJARATI_DV_I		     =	0xabf,
	GUJARATI_DV_II		     =	0xac0,
	GUJARATI_DV_U		     =	0xac1,
	GUJARATI_DV_UU		     =	0xac2,
	GUJARATI_DV_VR		     =	0xac3,
	GUJARATI_DV_VRR		     =	0xac4,
	GUJARATI_DV_CE		     =	0xac5,
	GUJARATI_DV_E		     =	0xac7,
	GUJARATI_DV_AI		     =	0xac8,
	GUJARATI_DV_CO		     =	0xac9,
	GUJARATI_DV_O		     =	0xacb,
	GUJARATI_DV_AU		     =	0xacc,
	GUJARATI_S_HALANT		     =	0xacd,
	GUJARATI_S_OM		     =	0xad0,
	GUJARATI_IV_VRR		     =	0xae0,
	GUJARATI_IV_VLL		     =	0xae1,
	GUJARATI_S_VL		     =	0xae2,
	GUJARATI_S_VLL		     =	0xae3,
	GUJARATI_DEVANAGARI_D		     =	0xae4,
	GUJARATI_DEVANAGARI_DD		     =	0xae5,

	/*  DIGITS  */

	GUJARATI_D_ZERO		     =	0xae6,
	GUJARATI_D_ONE		     =	0xae7,
	GUJARATI_D_TWO		     =	0xae8,
	GUJARATI_D_THREE		     =	0xae9,
	GUJARATI_D_FOUR		     =	0xaea,
	GUJARATI_D_FIVE		     =	0xaeb,
	GUJARATI_D_SIX		     =	0xaec,
	GUJARATI_D_SEVEN		     =	0xaed,
	GUJARATI_D_EIGHT		     =	0xaee,
	GUJARATI_D_NINE		     =	0xaef,
	GUJARATI_D_iIME		     =	0xaf1

}gujarati_characters_enum;

typedef enum
{   
	/* ORIYA SIGNS */
    ORIYA_DEVANAGARI_S_DANDA = 0x0964,
    ORIYA_DEVANAGARI_S_DOUBLE_DANDA = 0x0965,
	
	/* VARIOUS SIGNS + INDEPENDENT VOWEL */
    ORIYA_S_CHANDRA_BINDU = 0x0B01,
    ORIYA_S_ANUSVARA = 0x0B02,
    ORIYA_S_VISARGA = 0x0B03,

    ORIYA_IV_A = 0x0B05,
    ORIYA_IV_AA = 0x0B06,
    ORIYA_IV_I = 0x0B07,
    ORIYA_IV_II = 0x0B08,
    ORIYA_IV_U = 0x0B09,
    ORIYA_IV_UU = 0x0B0A,
    ORIYA_IV_R = 0x0B0B,
    ORIYA_IV_L = 0x0B0C,
    ORIYA_IV_E = 0x0B0F,
    ORIYA_IV_AI = 0x0B10,
    ORIYA_IV_O = 0x0B13,
    ORIYA_IV_AU = 0x0B14,

    /* CONSONANTS */
    ORIYA_C_KA = 0x0B15,
    ORIYA_C_KHA = 0x0B16,
    ORIYA_C_GA = 0x0B17,
    ORIYA_C_GHA = 0x0B18,
    ORIYA_C_NGA = 0x0B19,
    ORIYA_C_CHA = 0x0B1A,
    ORIYA_C_CHHA = 0x0B1B,
    ORIYA_C_JA = 0x0B1C,
    ORIYA_C_JHA = 0x0B1D,
    ORIYA_C_NYA = 0x0B1E,
    ORIYA_C_TTA = 0x0B1F,
    ORIYA_C_TTHA = 0x0B20,
    ORIYA_C_DDA = 0x0B21,
    ORIYA_C_DDHA = 0x0B22,
    ORIYA_C_NNA = 0x0B23,
    ORIYA_C_TA = 0x0B24,
    ORIYA_C_THA = 0x0B25,
    ORIYA_C_DA = 0x0B26,
    ORIYA_C_DHA = 0x0B27,
    ORIYA_C_NA = 0x0B28,

    ORIYA_C_PA = 0x0B2A,
    ORIYA_C_PHA = 0x0B2B,
    ORIYA_C_BA = 0x0B2C,
    ORIYA_C_BHA = 0x0B2D,
    ORIYA_C_MA = 0x0B2E,
    ORIYA_C_YA = 0x0B2F,
    ORIYA_C_RA = 0x0B30,

    ORIYA_C_LA = 0x0B32,
    ORIYA_C_LLA = 0x0B33,

    ORIYA_C_VA = 0x0B35,
    ORIYA_C_SHA = 0x0B36,
    ORIYA_C_SSA = 0x0B37,
    ORIYA_C_SA = 0x0B38,
    ORIYA_C_HA = 0x0B39,

    /* SIGNS */
    ORIYA_S_NUKTA = 0x0B3C,
    ORIYA_NS_AVAGRAHA = 0x0B3D,

    /* DEPENDENT VOWELS */
    ORIYA_DV_AA = 0x0B3E,
    ORIYA_DV_I = 0x0B3F,
    ORIYA_DV_II = 0x0B40,
    ORIYA_DV_U = 0x0B41,
    ORIYA_DV_UU = 0x0B42,
    ORIYA_DV_R = 0x0B43,
    ORIYA_DV_RR = 0x0B44,

    ORIYA_DV_E = 0x0B47,
    ORIYA_DV_AI = 0x0B48,

		ORIYA_SAN_AU = 0x0B4A, /* extra: to handle 0x0B4C becouse its pre,above and post base dependent vowle*/
    ORIYA_DV_O = 0x0B4B,
    ORIYA_DV_AU = 0x0B4C,

    /* SIGNS */
    ORIYA_S_HALANT = 0x0B4D,

    ORIYA_S_AI = 0x0B56,
    ORIYA_S_AU = 0x0B57,

    /* ADDITIONAL CONSONANT */
    ORIYA_AC_RRA = 0x0B5C,
    ORIYA_AC_RHA = 0x0B5D,
    ORIYA_AC_YYA = 0x0B5F,

    /* GENERIC ADDITIONS */
    ORIYA_NG_RR = 0x0B60,
    ORIYA_NG_LL = 0x0B61,
    ORIYA_NG_L = 0x0B62,
    ORIYA_NG_VLL = 0x0B63,
    ORIYA_NG_D = 0x0B64,
    ORIYA_NG_DD = 0x0B65,

    /* DIGITS */
    ORIYA_D_ZERO = 0x0B66,
    ORIYA_D_ONE = 0x0B67,
    ORIYA_D_TWO = 0x0B68,
    ORIYA_D_THREE = 0x0B69,
    ORIYA_D_FOUR = 0x0B6A,
    ORIYA_D_FIVE = 0x0B6B,
    ORIYA_D_SIX = 0x0B6C,
    ORIYA_D_SEVEN = 0x0B6D,
    ORIYA_D_EIGHT = 0x0B6E,
    ORIYA_D_NINE = 0x0B6F,
    /* ORIYA SPECIFIC ADDITOIN */
    ORIYA_S_ISSHAR = 0x0B70,
    ORIYA_S_WA = 0x0B71
} ORIYA_characters_enum;

typedef enum
{   
	/* DEVANAGARI SIGNS */
	KANNADA_DEVANAGARI_S_DANDA			= 0x0964,
	KANNADA_DEVANAGARI_S_DOUBLE_DANDA		= 0x0965,

	/* VARIOUS SIGNS */
    KANNADA_S_ANUSVARA			= 0x0C82,
	KANNADA_S_VISARGA			= 0x0C83,

	/* BLANK */
	/*N_BLANK1                  = 0x0C84,*/

	/* INDEPENDENT VOWELS */
	KANNADA_IV_A				= 0x0C85,
	KANNADA_IV_AA				= 0x0C86,
	KANNADA_IV_I				= 0x0C87,
	KANNADA_IV_II				= 0x0C88,
	KANNADA_IV_U				= 0x0C89,
	KANNADA_IV_UU				= 0x0C8A,
	KANNADA_IV_VR				= 0x0C8B,
	KANNADA_IV_VL				= 0x0C8C,
	/*NIV_RESERVED1             = 0x0C8D,*//* RESERVED */
	KANNADA_IV_E				= 0x0C8E,
	KANNADA_IV_EE				= 0x0C8F,
	KANNADA_IV_AI				= 0x0C90,
	/*NIV_RESERVED2             = 0x0C91,*//* RESERVED */
	KANNADA_IV_O				= 0x0C92,
	KANNADA_IV_OO				= 0x0C93,
	KANNADA_IV_AU				= 0x0C94,

    /* CONSONANTS */
    KANNADA_C_KA				= 0x0C95,
    KANNADA_C_KHA				= 0x0C96,
    KANNADA_C_GA				= 0x0C97,
    KANNADA_C_GHA				= 0x0C98,
    KANNADA_C_NGA				= 0x0C99,
    KANNADA_C_CA				= 0x0C9A,
    KANNADA_C_CHA				= 0x0C9B,
    KANNADA_C_JA				= 0x0C9C,
    KANNADA_C_JHA				= 0x0C9D,
    KANNADA_C_NYA				= 0x0C9E,
    KANNADA_C_TTA				= 0x0C9F,
    KANNADA_C_TTHA				= 0x0CA0,
    KANNADA_C_DDA				= 0x0CA1,
    KANNADA_C_DDHA				= 0x0CA2,
    KANNADA_C_NNA				= 0x0CA3,
    KANNADA_C_TA				= 0x0CA4,
    KANNADA_C_THA				= 0x0CA5,
    KANNADA_C_DA				= 0x0CA6,
    KANNADA_C_DHA				= 0x0CA7,
    KANNADA_C_NA				= 0x0CA8,
	/*NC_RESERVED1              = 0x0CA9,*//* RESERVED */
    KANNADA_C_PA				= 0x0CAA,
    KANNADA_C_PHA				= 0x0CAB,
    KANNADA_C_BA				= 0x0CAC,
    KANNADA_C_BHA				= 0x0CAD,
    KANNADA_C_MA				= 0x0CAE,
    KANNADA_C_YA				= 0x0CAF,
    KANNADA_C_RA				= 0x0CB0,
    KANNADA_C_RRA				= 0x0CB1,
    KANNADA_C_LA				= 0x0CB2,
    KANNADA_C_LLA				= 0x0CB3,
	/*NC_RESERVED2              = 0x0CB4,*//* RESERVED */
    KANNADA_C_VA				= 0x0CB5,
    KANNADA_C_SHA				= 0x0CB6,
    KANNADA_C_SSA				= 0x0CB7,
    KANNADA_C_SA				= 0x0CB8,
    KANNADA_C_HA				= 0x0CB9,
    
    /* BLANK */
	/*N_BLANK2                  = 0x0CBA,*/
	/*N_BLANK3                  = 0x0CBB,*/
	
	/* VARIOUS SIGNS */
    KANNADA_S_NUKTA				= 0x0CBC,
    KANNADA_S_AVAGRAHA			= 0x0CBD,

    /* DEPENDENT VOWELS */
    KANNADA_DV_AA				= 0x0CBE,
    KANNADA_DV_I				= 0x0CBF,
    KANNADA_DV_II				= 0x0CC0,
    KANNADA_DV_U				= 0x0CC1,
    KANNADA_DV_UU				= 0x0CC2,
    KANNADA_DV_VR				= 0x0CC3,
    KANNADA_DV_VRR				= 0x0CC4,
	/*NDV_RESERVED1             = 0x0CC5,*//* RESERVED */
    KANNADA_DV_E				= 0x0CC6,
    KANNADA_DV_EE				= 0x0CC7,
    KANNADA_DV_AI				= 0x0CC8,
	/*NDV_RESERVED2             = 0x0CC9,*//* RESERVED */
    KANNADA_DV_O				= 0x0CCA,
	KANNADA_DV_OO				= 0x0CCB,
    KANNADA_DV_AU				= 0x0CCC,

	/* SIGNS */
    KANNADA_S_HALANT			= 0x0CCD,

	/* BLANK */
    /*N_BLANK4                  = 0x0CCE,*/
	/*N_BLANK5                  = 0x0CCF,*/
	/*N_BLANK6                  = 0x0CD0,*/
	/*N_BLANK7                  = 0x0CD1,*/
	/*N_BLANK8                  = 0x0CD2,*/
	/*N_BLANK9                  = 0x0CD3,*/
	/*N_BLANK10                 = 0x0CD4,*/
	
	/* VARIOUS SIGNS */
	KANNADA_S_LENGTH_MARK		= 0x0CD5,
	KANNADA_S_AI_LENGTH_MARK	= 0x0CD6,

	/* BLANK */
   /* N_BLANK11                 = 0x0CD7,*/
	/*N_BLANK12                 = 0x0CD8,*/
	/*N_BLANK13                 = 0x0CD9,*/
	/*N_BLANK14                 = 0x0CDA,*/
	/*N_BLANK15                 = 0x0CDB,*/
	/*N_BLANK16                 = 0x0CDC,*/
	/*N_BLANK17                 = 0x0CDD,*/

	/* ADDITIONAL CONSONANT */
    KANNADA_AC_FA				= 0x0CDE,/*obslete historic letter */

	/* BLANK */
    /*N_BLANK18                 = 0x0CDF,*/
   

    /* GENERIC ADDITION */
    KANNADA_IV_VRR				= 0x0CE0, 
    KANNADA_IV_VLL				= 0x0CE1, 
	KANNADA_IV_VVL				= 0x0CE2, 
	KANNADA_IV_VVLL				= 0x0CE3, 

	/* BLANK */
    KANNADA_S_DANDA				= 0x0CE4,
	KANNADA_S_DOUBLE_DANDA		= 0x0CE5,
    
    /* DIGITS */
    KANNADA_D_ZERO				= 0x0CE6,
    KANNADA_D_ONE				= 0x0CE7,
    KANNADA_D_TWO				= 0x0CE8,
    KANNADA_D_THREE				= 0x0CE9,
    KANNADA_D_FOUR				= 0x0CEA,
    KANNADA_D_FIVE				= 0x0CEB,
    KANNADA_D_SIX				= 0x0CEC,
    KANNADA_D_SEVEN				= 0x0CED,
    KANNADA_D_EIGHT				= 0x0CEE,
    KANNADA_D_NINE				= 0x0CEF,
    
	/* BLANK */
    /*N_BLANK21                 = 0x0CF0,*/

	/* VARIOUS SIGNS */
	KANNADA_S_JIHVAMULIYA		= 0x0CF1,
	KANNADA_S_UPADHMANIYA		= 0x0CF2,

	/* END OF ENUM */
    KANNADA_DUMMY_END
} kannada_characters_enum;

typedef enum
{
	/* MALAYALAM SIGNS */
    MALAYALAM_DEVANAGARI_S_DANDA = 0x0964,
    MALAYALAM_DEVANAGARI_S_DOUBLE_DANDA = 0x0965,
	
	/* VARIOUS SIGNS */
	MALAYALAM_S_ANUSVARA	     =	0x0D02,
	MALAYALAM_S_VISARGA		     =	0x0D03,
	
	/* INDEPENDENT VOWEL */
	MALAYALAM_IV_A		         =	0x0D05,
	MALAYALAM_IV_AA		         =	0x0D06,
	MALAYALAM_IV_I		         =	0x0D07,
	MALAYALAM_IV_II		         =	0x0D08,
	MALAYALAM_IV_U		         =	0x0D09,
	MALAYALAM_IV_UU		         =	0x0D0A,
	MALAYALAM_IV_VR		         =	0x0D0B,
	MALAYALAM_IV_VL		         =	0x0D0C,
	MALAYALAM_IV_E		         =	0x0D0E,
	MALAYALAM_IV_EE		         =	0x0D0F,
	MALAYALAM_IV_AI		         =	0x0D10,
	MALAYALAM_IV_O		         =	0x0D12,
	MALAYALAM_IV_OO		         =	0x0D13,
	MALAYALAM_IV_AU		         =	0x0D14,
	
	/* CONSONANTS */
	MALAYALAM_C_KA		         =	0x0D15,
	MALAYALAM_C_KHA		         =	0x0D16,
	MALAYALAM_C_GA		         =	0x0D17,
	MALAYALAM_C_GHA		         =	0x0D18,
	MALAYALAM_C_NGA		         =	0x0D19,
	MALAYALAM_C_CA		         =	0x0D1A,
	MALAYALAM_C_CHA		         =	0x0D1B,
	MALAYALAM_C_JA		         =	0x0D1C,
	MALAYALAM_C_JHA		         =	0x0D1D,
	MALAYALAM_C_NYA		         =	0x0D1E,
	MALAYALAM_C_TTA		         =	0x0D1F,
	MALAYALAM_C_TTHA		     =	0x0D20,
	MALAYALAM_C_DDA		         =	0x0D21,
	MALAYALAM_C_DDHA		     =	0x0D22,
	MALAYALAM_C_NNA		         =	0x0D23,
	MALAYALAM_C_TA		         =	0x0D24,
	MALAYALAM_C_THA		         =	0x0D25,
	MALAYALAM_C_DA		         =	0x0D26,
	MALAYALAM_C_DHA		         =	0x0D27,
	MALAYALAM_C_NA		         =	0x0D28,
	MALAYALAM_C_PA		         =	0x0D2A,
	MALAYALAM_C_PHA		         =	0x0D2B,
	MALAYALAM_C_BA		         =	0x0D2C,
	MALAYALAM_C_BHA		         =	0x0D2D,
	MALAYALAM_C_MA		         =	0x0D2E,
	MALAYALAM_C_YA		         =	0x0D2F,
	MALAYALAM_C_RA		         =	0x0D30,
	MALAYALAM_C_RRA		         =	0x0D31,
	MALAYALAM_C_LA		         =	0x0D32,
	MALAYALAM_C_LLA		         =	0x0D33,
	MALAYALAM_C_LLLA		     =	0x0D34,
	MALAYALAM_C_VA		         =	0x0D35,
	MALAYALAM_C_SHA		         =	0x0D36,
	MALAYALAM_C_SSA		         =	0x0D37,
	MALAYALAM_C_SA		         =	0x0D38,
	MALAYALAM_C_HA		         =	0x0D39,
	
	/* VARIOUS SIGNS */ 
	MALAYALAM_S_AVAGRAHA         =	0x0D3D,
	
	/* DEPENDENT VOWELS */
	MALAYALAM_DV_AA		         =	0x0D3E,
	MALAYALAM_DV_I		         =	0x0D3F,
	MALAYALAM_DV_II		         =	0x0D40,
	MALAYALAM_DV_U		         =	0x0D41,
	MALAYALAM_DV_UU		         =	0x0D42,
	MALAYALAM_DV_VR		         =	0x0D43,
	MALAYALAM_DV_VRR	         =	0x0D44,
	MALAYALAM_DV_E		         =	0x0D46,
	MALAYALAM_DV_EE		         =	0x0D47,
	MALAYALAM_DV_AI		         =	0x0D48,
	MALAYALAM_DV_O		         =	0x0D4A,
	MALAYALAM_DV_OO		         =	0x0D4B,
	MALAYALAM_DV_AU		         =	0x0D4C,
	
	/* VARIOUS SIGNS */ 
	MALAYALAM_S_HALANT		     =	0x0D4D,
	MALAYALAM_S_AU_LENGTH_MARK	 =	0x0D57,
	
	/* INDEPENDENT VOWELS */
	MALAYALAM_IV_VRR		     =	0x0D60,
	MALAYALAM_IV_VLL		     =	0x0D61,
	
	/* DEPENDENT VOWELS */
	MALAYALAM_DV_VL			     =	0x0D62,
	MALAYALAM_DV_VLL		     =	0x0D63,

	/* MALAYALAM SIGNS */
    MALAYALAM_S_DANDA 			 = 0x0D64,
    MALAYALAM_S_DOUBLE_DANDA 	 = 0x0D65,

	/*  DIGITS  */
	MALAYALAM_D_ZERO		     =	0x0D66,
	MALAYALAM_D_ONE		         =	0x0D67,
	MALAYALAM_D_TWO		         =	0x0D68,
	MALAYALAM_D_THREE		     =	0x0D69,
	MALAYALAM_D_FOUR		     =	0x0D6A,
	MALAYALAM_D_FIVE		     =	0x0D6B,
	MALAYALAM_D_SIX		         =	0x0D6C,
	MALAYALAM_D_SEVEN		     =	0x0D6D,
	MALAYALAM_D_EIGHT		     =	0x0D6E,
	MALAYALAM_D_NINE		     =	0x0D6F,
	MALAYALAM_D_TEN			     =	0x0D70,
	MALAYALAM_D_HUNDERED	     =	0x0D71,
	MALAYALAM_D_THOUSAND	     =	0x0D72,
	
	/* FRACTION */
	MALAYALAM_F_QUARTER		     =	0x0D73,
	MALAYALAM_F_HALF		     =	0x0D74,
	MALAYALAM_F_THREE_QUARTER    =	0x0D75,
	
	/* DATE MARK */
	MALAYALAM_S_DATE_MARK		 =  0x0D79,
	
	/* CHILLU LETTERS */
	MALAYALAM_C_CHILLU_NN		 =  0x0D7A,
	MALAYALAM_C_CHILLU_N		 =  0x0D7B,
	MALAYALAM_C_CHILLU_RR		 =  0x0D7C,
	MALAYALAM_C_CHILLU_L		 =  0x0D7D,
	MALAYALAM_C_CHILLU_LL		 =  0x0D7E,
	MALAYALAM_C_CHILLU_K		 =  0x0D7F,
	
	/* END OF ENUM */
	MALAYALAM_DUMMY_END
}malayalam_characters_enum;

int GetMultitype_Telugu(
        IN const void*          pdbMisc,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Marathi(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Tamil(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Punjabi(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Gujarati(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Oriya(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Kannada(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Malayalam(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Khmer(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Lao(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

int GetMultitype_Bengali(
        IN const void*          pdb,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

#endif /* __INDIC_MT_H__ */
