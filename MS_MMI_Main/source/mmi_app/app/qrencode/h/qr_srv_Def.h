
#ifndef _SRV_QRCODEDEF_H
#define _SRV_QRCODEDEF_H

/**
 * Maximum version (size) of QR-code symbol.
 */
#define MQRSPEC_VERSION_MAX 4

/**
 * Maximum version (size) of QR-code symbol.
 */
#define QRSPEC_VERSION_MAX 40

/**
 * Maximum number of symbols in a set of structured-appended symbols.
 */
#define MAX_STRUCTURED_SYMBOLS 16

/**
 * Length of a segment of structured-append header.
 */
#define STRUCTURE_HEADER_SIZE 20

/**
 * Length of a standard mode indicator in bits.
 */

#define MODE_INDICATOR_SIZE 4

#define mask_maskNum (8)
#define mmask_maskNum (4)
/**
 * Maximum width of a symbol
 */
#define QRSPEC_WIDTH_MAX 177

static char levelChar[4] = {'L', 'M', 'Q', 'H'};

#define QRspec_rsBlockNum(__spec__) (__spec__[0] + __spec__[3])
#define QRspec_rsBlockNum1(__spec__) (__spec__[0])
#define QRspec_rsDataCodes1(__spec__) (__spec__[1])
#define QRspec_rsEccCodes1(__spec__) (__spec__[2])
#define QRspec_rsBlockNum2(__spec__) (__spec__[3])
#define QRspec_rsDataCodes2(__spec__) (__spec__[4])
#define QRspec_rsEccCodes2(__spec__) (__spec__[2])
#define QRspec_rsBlockNum1(__spec__) (__spec__[0])

#define QRspec_rsDataLength(__spec__) \
	((QRspec_rsBlockNum1(__spec__) * QRspec_rsDataCodes1(__spec__)) + \
	 (QRspec_rsBlockNum2(__spec__) * QRspec_rsDataCodes2(__spec__)))
#define QRspec_rsEccLength(__spec__) \
	(QRspec_rsBlockNum(__spec__) * QRspec_rsEccCodes1(__spec__))
	
#define N1 (3)
#define N2 (3)
#define N3 (40)
#define N4 (10)

/**
 * Mode indicator. See Table 2 in Appendix 1 of JIS X0510:2004, pp.107.
 */
#define MQRSPEC_MODEID_NUM       0
#define MQRSPEC_MODEID_AN        1
#define MQRSPEC_MODEID_8         2
#define MQRSPEC_MODEID_KANJI     3

/**
 * Mode indicator. See Table 2 of JIS X0510:2004, pp.16.
 */
#define QRSPEC_MODEID_ECI        7
#define QRSPEC_MODEID_NUM        1
#define QRSPEC_MODEID_AN         2
#define QRSPEC_MODEID_8          4
#define QRSPEC_MODEID_KANJI      8
#define QRSPEC_MODEID_FNC1FIRST  5
#define QRSPEC_MODEID_FNC1SECOND 9
#define QRSPEC_MODEID_STRUCTURE  3
#define QRSPEC_MODEID_TERMINATOR 0

#define MM (rs->mm)
#define NN (rs->nn)
#define ALPHA_TO (rs->alpha_to) 
#define INDEX_OF (rs->index_of)
#define GENPOLY (rs->genpoly)
#define NROOTS (rs->nroots)
#define FCR (rs->fcr)
#define PRIM (rs->prim)
#define IPRIM (rs->iprim)
#define PAD (rs->pad)
#define A0 (NN)

#define MODNN(x) modnn(rs,x)

#define INT_MAX		2147483647
/* Stuff specific to the 8-bit symbol version of the general purpose RS codecs
 *
 */
typedef unsigned char data_t;

typedef int mask_MASKMAKER(int, const unsigned char *, unsigned char *);
typedef void mmask_MASKMAKER(int, const unsigned char *, unsigned char *);


#define BitStream_size(__bstream__) (__bstream__->length)

/**
 * Look up the alphabet-numeric convesion table (see JIS X0510:2004, pp.19).
 * @param __c__ character
 * @return value
 */
#define QRinput_lookAnTable(__c__) \
	((__c__ & 0x80)?(-1):QRinput_anTable[(int)__c__])
	
/**
 * QRcode class.
 * Symbol data is represented as an array contains width*width uchars.
 * Each uchar represents a module (dot). If the less significant bit of
 * the uchar is 1, the corresponding module is black. The other bits are
 * meaningless for usual applications, but here its specification is described.
 *
 * <pre>
 * MSB 76543210 LSB
 *     |||||||`- 1=black/0=white
 *     ||||||`-- data and ecc code area
 *     |||||`--- format information
 *     ||||`---- version information
 *     |||`----- timing pattern
 *     ||`------ alignment pattern
 *     |`------- finder pattern and separator
 *     `-------- non-data modules (format, timing, etc.)
 * </pre>
 */
typedef struct {
	int version;         ///< version of the symbol
	int width;           ///< width of the symbol
	unsigned char *data; ///< symbol data
} QRcode;

/******************************************************************************
 * Version and capacity
 *****************************************************************************/

typedef struct {
	int width; //< Edge length of the symbol
	int words;  //< Data capacity (bytes)
	int remainder; //< Remainder bit (bits)
	int ec[4];  //< Number of ECC code (bytes)
} QRspec_Capacity;

/**
 * Reed-Solomon codec control block
 */
struct _RS {
	int mm;              /* Bits per symbol */
	int nn;              /* Symbols per block (= (1<<mm)-1) */
	data_t *alpha_to;     /* log lookup table */
	data_t *index_of;     /* Antilog lookup table */
	data_t *genpoly;      /* Generator polynomial */
	int nroots;     /* Number of generator roots = number of parity symbols */
	int fcr;        /* First consecutive root, index form */
	int prim;       /* Primitive element, index form */
	int iprim;      /* prim-th root of 1, index form */
	int pad;        /* Padding bytes in shortened block */
	int gfpoly;
	struct _RS *next;
};


/**
 * Level of error correction.
 */
typedef enum {
	QR_ECLEVEL_L = 0, ///< lowest
	QR_ECLEVEL_M,
	QR_ECLEVEL_Q,
	QR_ECLEVEL_H      ///< highest
} QRecLevel;

/**
 * Encoding mode.
 */
typedef enum {
	QR_MODE_NUL = -1,  ///< Terminator (NUL character). Internal use only
	QR_MODE_NUM = 0,   ///< Numeric mode
	QR_MODE_AN,        ///< Alphabet-numeric mode
	QR_MODE_8,         ///< 8-bit data mode
	QR_MODE_KANJI,     ///< Kanji (shift-jis) mode
	QR_MODE_STRUCTURE, ///< Internal use only
	QR_MODE_ECI,       ///< ECI mode
	QR_MODE_FNC1FIRST,  ///< FNC1, first position
	QR_MODE_FNC1SECOND, ///< FNC1, second position
} QRencodeMode;

typedef struct {
	int length;
	unsigned char *data;
} BitStream;

/******************************************************************************
 * Entry of input data
 *****************************************************************************/
 typedef struct _QRinput_List QRinput_List;

struct _QRinput_List {
	QRencodeMode mode;
	int size;				///< Size of data chunk (byte).
	unsigned char *data;	///< Data chunk.
	BitStream *bstream;
	QRinput_List *next;
};


struct _QRinput {
	int version;
	QRecLevel level;
	QRinput_List *head;
	QRinput_List *tail;
	int mqr;
	int fnc1;
	unsigned char appid;
};


typedef struct _QRinput QRinput;

/******************************************************************************
 * Structured append input data
 *****************************************************************************/
 typedef struct _QRinput_InputList QRinput_InputList;

struct _QRinput_InputList {
	QRinput *input;
	QRinput_InputList *next;
};


struct _QRinput_Struct {
	int size;					///< number of structured symbols
	int parity;
	QRinput_InputList *head;
	QRinput_InputList *tail;
};

/**
 * Singly-linked list of QRcode. Used to represent a structured symbols.
 * A list is terminated with NULL.
 */
typedef struct _QRcode_List QRcode_List;

struct _QRcode_List {
	QRcode *code;
	QRcode_List *next;
};


/******************************************************************************
 * Raw code
 *****************************************************************************/

typedef struct {
	int dataLength;
	unsigned char *data;
	int eccLength;
	unsigned char *ecc;
} RSblock;


/******************************************************************************
 * Raw code for Micro QR Code
 *****************************************************************************/

typedef struct {
	int version;
	int dataLength;
	int eccLength;
	unsigned char *datacode;
	unsigned char *ecccode;
	RSblock *rsblock;
	int oddbits;
	int count;
} MQRRawCode;


/**
 * Set of QRinput for structured symbols.
 */
typedef struct _QRinput_Struct QRinput_Struct;

/**
 * Reed-Solomon codec control block
 */
 typedef struct _RS RS;



/*
 * General purpose RS codec, 8-bit symbols.
 */


typedef struct {
	int width; //< Edge length of the symbol
	int ec[4];  //< Number of ECC code (bytes)
} MQRspec_Capacity;

/******************************************************************************
 * Frame filling
 *****************************************************************************/

typedef struct {
	int width;
	unsigned char *frame;
	int x, y;
	int dir;
	int bit;
	int mqr;
} FrameFiller;

typedef struct {
	int version;
	int dataLength;
	int eccLength;
	unsigned char *datacode;
	unsigned char *ecccode;
	int b1;
	int blocks;
	RSblock *rsblock;
	int count;
} QRRawCode;

extern void view_simple(const unsigned char *str, int length);


#endif	/*_SRV_QRCODEDEF_H*/

