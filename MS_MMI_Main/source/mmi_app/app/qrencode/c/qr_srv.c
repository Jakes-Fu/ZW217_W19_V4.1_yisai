

#ifndef _SRV_QRCODE_C
#define _SRV_QRCODE_C





#include "qr_srv_Def.h"

#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "mmicc_export.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmi_common.h"

#include "mmicom_panel.h"
#include "mmienvset_export.h"

//#include "zdt_app.h"
#include "mmiidle_export.h"
#include "mmi_position.h"
#include "ldo_drvapi.h"
#include "mmisrvrecord_export.h"
#include "mmisrvfmm_export.h"

#include "mmifmm_pic.h"

#include "img_dec_interface.h"
#include "graphics_draw.h"


//#include "clib_list.h"
static int casesensitive = 0;	//2是否只允许大写字母
static int eightbit = 1;
static int version = 1;//  1
static int size = 1;//  4 ,
static int margin = 1;//  4
static int structured = 0;
static int micro = 0;
static QRecLevel level = QR_ECLEVEL_L;
static QRencodeMode hint = QR_MODE_8;
static char **textv;
static int textc;
static RS *rslist = NULL;
static int QR_X0=0;
static int QR_Y0=0;
static int QR_Weigth=0;
static int QR_Height=0;
static U8 Version_NUM =21;
/**
 * Table of the capacity of symbols
 * See Table 1 (pp.106) and Table 8 (pp.113) of Appendix 1, JIS X0510:2004.
 */
static const MQRspec_Capacity mqrspecCapacity[MQRSPEC_VERSION_MAX + 1] = {
	{  0, {0,  0,  0, 0}},
	{ 11, {2,  0,  0, 0}},
	{ 13, {5,  6,  0, 0}},
	{ 15, {6,  8,  0, 0}},
	{ 17, {8, 10, 14, 0}}
};
/* See Table 10 of Appendix 1. (pp.115) */
static const int mqrspec_typeTable[MQRSPEC_VERSION_MAX + 1][3] = {
	{-1, -1, -1},
	{ 0, -1, -1},
	{ 1,  2, -1},
	{ 3,  4, -1},
	{ 5,  6,  7}
};

/******************************************************************************
 * Length indicator
 *****************************************************************************/

static const int qrspec_lengthTableBits[4][3] = {
	{10, 12, 14},
	{ 9, 11, 13},
	{ 8, 16, 16},
	{ 8, 10, 12}
};

static const int mqrspec_lengthTableBits[4][4] = {
	{ 3, 4, 5, 6},
	{ 0, 3, 4, 5},
	{ 0, 0, 4, 5},
	{ 0, 0, 3, 4}
};

/**
 * Table of the capacity of symbols
 * See Table 1 (pp.13) and Table 12-16 (pp.30-36), JIS X0510:2004.
 */
static const QRspec_Capacity qrspecCapacity[QRSPEC_VERSION_MAX + 1] = {
	{  0,    0, 0, {   0,    0,    0,    0}},
	{ 21,   26, 0, {   7,   10,   13,   17}}, // 1
	{ 25,   44, 7, {  10,   16,   22,   28}},
	{ 29,   70, 7, {  15,   26,   36,   44}},
	{ 33,  100, 7, {  20,   36,   52,   64}},
	{ 37,  134, 7, {  26,   48,   72,   88}}, // 5
	{ 41,  172, 7, {  36,   64,   96,  112}},
	{ 45,  196, 0, {  40,   72,  108,  130}},
	{ 49,  242, 0, {  48,   88,  132,  156}},
	{ 53,  292, 0, {  60,  110,  160,  192}},
	{ 57,  346, 0, {  72,  130,  192,  224}}, //10
	{ 61,  404, 0, {  80,  150,  224,  264}},
	{ 65,  466, 0, {  96,  176,  260,  308}},
	{ 69,  532, 0, { 104,  198,  288,  352}},
	{ 73,  581, 3, { 120,  216,  320,  384}},
	{ 77,  655, 3, { 132,  240,  360,  432}}, //15
	{ 81,  733, 3, { 144,  280,  408,  480}},
	{ 85,  815, 3, { 168,  308,  448,  532}},
	{ 89,  901, 3, { 180,  338,  504,  588}},
	{ 93,  991, 3, { 196,  364,  546,  650}},
	{ 97, 1085, 3, { 224,  416,  600,  700}}, //20
	{101, 1156, 4, { 224,  442,  644,  750}},
	{105, 1258, 4, { 252,  476,  690,  816}},
	{109, 1364, 4, { 270,  504,  750,  900}},
	{113, 1474, 4, { 300,  560,  810,  960}},
	{117, 1588, 4, { 312,  588,  870, 1050}}, //25
	{121, 1706, 4, { 336,  644,  952, 1110}},
	{125, 1828, 4, { 360,  700, 1020, 1200}},
	{129, 1921, 3, { 390,  728, 1050, 1260}},
	{133, 2051, 3, { 420,  784, 1140, 1350}},
	{137, 2185, 3, { 450,  812, 1200, 1440}}, //30
	{141, 2323, 3, { 480,  868, 1290, 1530}},
	{145, 2465, 3, { 510,  924, 1350, 1620}},
	{149, 2611, 3, { 540,  980, 1440, 1710}},
	{153, 2761, 3, { 570, 1036, 1530, 1800}},
	{157, 2876, 0, { 570, 1064, 1590, 1890}}, //35
	{161, 3034, 0, { 600, 1120, 1680, 1980}},
	{165, 3196, 0, { 630, 1204, 1770, 2100}},
	{169, 3362, 0, { 660, 1260, 1860, 2220}},
	{173, 3532, 0, { 720, 1316, 1950, 2310}},
	{177, 3706, 0, { 750, 1372, 2040, 2430}} //40
};


/**
 * Table of the error correction code (Reed-Solomon block)
 * See Table 12-16 (pp.30-36), JIS X0510:2004.
 */
static const int eccTable[QRSPEC_VERSION_MAX+1][4][2] = {
	{{ 0,  0}, { 0,  0}, { 0,  0}, { 0,  0}},
	{{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}}, // 1
	{{ 1,  0}, { 1,  0}, { 1,  0}, { 1,  0}},
	{{ 1,  0}, { 1,  0}, { 2,  0}, { 2,  0}},
	{{ 1,  0}, { 2,  0}, { 2,  0}, { 4,  0}},
	{{ 1,  0}, { 2,  0}, { 2,  2}, { 2,  2}}, // 5
	{{ 2,  0}, { 4,  0}, { 4,  0}, { 4,  0}},
	{{ 2,  0}, { 4,  0}, { 2,  4}, { 4,  1}},
	{{ 2,  0}, { 2,  2}, { 4,  2}, { 4,  2}},
	{{ 2,  0}, { 3,  2}, { 4,  4}, { 4,  4}},
	{{ 2,  2}, { 4,  1}, { 6,  2}, { 6,  2}}, //10
	{{ 4,  0}, { 1,  4}, { 4,  4}, { 3,  8}},
	{{ 2,  2}, { 6,  2}, { 4,  6}, { 7,  4}},
	{{ 4,  0}, { 8,  1}, { 8,  4}, {12,  4}},
	{{ 3,  1}, { 4,  5}, {11,  5}, {11,  5}},
	{{ 5,  1}, { 5,  5}, { 5,  7}, {11,  7}}, //15
	{{ 5,  1}, { 7,  3}, {15,  2}, { 3, 13}},
	{{ 1,  5}, {10,  1}, { 1, 15}, { 2, 17}},
	{{ 5,  1}, { 9,  4}, {17,  1}, { 2, 19}},
	{{ 3,  4}, { 3, 11}, {17,  4}, { 9, 16}},
	{{ 3,  5}, { 3, 13}, {15,  5}, {15, 10}}, //20
	{{ 4,  4}, {17,  0}, {17,  6}, {19,  6}},
	{{ 2,  7}, {17,  0}, { 7, 16}, {34,  0}},
	{{ 4,  5}, { 4, 14}, {11, 14}, {16, 14}},
	{{ 6,  4}, { 6, 14}, {11, 16}, {30,  2}},
	{{ 8,  4}, { 8, 13}, { 7, 22}, {22, 13}}, //25
	{{10,  2}, {19,  4}, {28,  6}, {33,  4}},
	{{ 8,  4}, {22,  3}, { 8, 26}, {12, 28}},
	{{ 3, 10}, { 3, 23}, { 4, 31}, {11, 31}},
	{{ 7,  7}, {21,  7}, { 1, 37}, {19, 26}},
	{{ 5, 10}, {19, 10}, {15, 25}, {23, 25}}, //30
	{{13,  3}, { 2, 29}, {42,  1}, {23, 28}},
	{{17,  0}, {10, 23}, {10, 35}, {19, 35}},
	{{17,  1}, {14, 21}, {29, 19}, {11, 46}},
	{{13,  6}, {14, 23}, {44,  7}, {59,  1}},
	{{12,  7}, {12, 26}, {39, 14}, {22, 41}}, //35
	{{ 6, 14}, { 6, 34}, {46, 10}, { 2, 64}},
	{{17,  4}, {29, 14}, {49, 10}, {24, 46}},
	{{ 4, 18}, {13, 32}, {48, 14}, {42, 32}},
	{{20,  4}, {40,  7}, {43, 22}, {10, 67}},
	{{19,  6}, {18, 31}, {34, 34}, {20, 61}},//40
};

/******************************************************************************
 * Alignment pattern
 *****************************************************************************/

/**
 * Positions of alignment patterns.
 * This array includes only the second and the third position of the alignment
 * patterns. Rest of them can be calculated from the distance between them.
 *
 * See Table 1 in Appendix E (pp.71) of JIS X0510:2004.
 */
static const int alignmentPattern[QRSPEC_VERSION_MAX+1][2] = {
	{ 0,  0},
	{ 0,  0}, {18,  0}, {22,  0}, {26,  0}, {30,  0}, // 1- 5
	{34,  0}, {22, 38}, {24, 42}, {26, 46}, {28, 50}, // 6-10
	{30, 54}, {32, 58}, {34, 62}, {26, 46}, {26, 48}, //11-15
	{26, 50}, {30, 54}, {30, 56}, {30, 58}, {34, 62}, //16-20
	{28, 50}, {26, 50}, {30, 54}, {28, 54}, {32, 58}, //21-25
	{30, 58}, {34, 62}, {26, 50}, {30, 54}, {26, 52}, //26-30
	{30, 56}, {34, 60}, {30, 58}, {34, 62}, {30, 54}, //31-35
	{24, 50}, {28, 54}, {32, 58}, {26, 54}, {30, 58}, //35-40
};

const signed char QRinput_anTable[128] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	36, -1, -1, -1, 37, 38, -1, -1, -1, -1, 39, 40, -1, 41, 42, 43,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 44, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

/******************************************************************************
 * Format information
 *****************************************************************************/

/* See calcFormatInfo in tests/test_qrspec.c */
static const unsigned int qrspec_formatInfo[4][8] = {
	{0x77c4, 0x72f3, 0x7daa, 0x789d, 0x662f, 0x6318, 0x6c41, 0x6976},
	{0x5412, 0x5125, 0x5e7c, 0x5b4b, 0x45f9, 0x40ce, 0x4f97, 0x4aa0},
	{0x355f, 0x3068, 0x3f31, 0x3a06, 0x24b4, 0x2183, 0x2eda, 0x2bed},
	{0x1689, 0x13be, 0x1ce7, 0x19d0, 0x0762, 0x0255, 0x0d0c, 0x083b}
};

/* See calcFormatInfo in tests/test_mqrspec.c */
static const unsigned int formatInfo[4][8] = {
	{0x4445, 0x55ae, 0x6793, 0x7678, 0x06de, 0x1735, 0x2508, 0x34e3},
	{0x4172, 0x5099, 0x62a4, 0x734f, 0x03e9, 0x1202, 0x203f, 0x31d4},
	{0x4e2b, 0x5fc0, 0x6dfd, 0x7c16, 0x0cb0, 0x1d5b, 0x2f66, 0x3e8d},
	{0x4b1c, 0x5af7, 0x68ca, 0x7921, 0x0987, 0x186c, 0x2a51, 0x3bba}
};

/******************************************************************************
 * Version information pattern
 *****************************************************************************/

/**
 * Version information pattern (BCH coded).
 * See Table 1 in Appendix D (pp.68) of JIS X0510:2004.
 */
static const unsigned int versionPattern[QRSPEC_VERSION_MAX - 6] = {
	0x07c94, 0x085bc, 0x09a99, 0x0a4d3, 0x0bbf6, 0x0c762, 0x0d847, 0x0e60d,
	0x0f928, 0x10b78, 0x1145d, 0x12a17, 0x13532, 0x149a6, 0x15683, 0x168c9,
	0x177ec, 0x18ec4, 0x191e1, 0x1afab, 0x1b08e, 0x1cc1a, 0x1d33f, 0x1ed75,
	0x1f250, 0x209d5, 0x216f0, 0x228ba, 0x2379f, 0x24b0b, 0x2542e, 0x26a64,
	0x27541, 0x28c69
};

/**
 * Cache of initial frames.
 */
/* C99 says that static storage shall be initialized to a null pointer
 * by compiler. */
static unsigned char *frames[QRSPEC_VERSION_MAX + 1];
/**
 * Cache of initial mqr_frames.
 */
/* C99 says that static storage shall be initialized to a null pointer
 * by compiler. */
static unsigned char *mqr_frames[MQRSPEC_VERSION_MAX + 1];


#if 1 //def WIN32
static void* qr_srv_malloc(uint32 size)
{
	char* mem_ptr = NULL;
	
	mem_ptr = (char*)malloc(size);
	return (void*)mem_ptr;
}

static void* qr_srv_calloc(uint32 nmemb, uint32 size)
{
    uint32 total = nmemb * size;
	char* mem_ptr = NULL;
	
	mem_ptr = (char*)malloc(total);
	return (void*)mem_ptr;
}

static void qr_srv_free(void* ptr)
{
	
	free(ptr);
}


#else
static void* qr_srv_malloc(uint32 size)
{
	char* mem_ptr = NULL;
	
	//mem_ptr = (char*)malloc(size);
	mem_ptr = (char*)malloc(size);
	return (void*)mem_ptr;
}

static void* qr_srv_calloc(uint32 nmemb, uint32 size)
{
    uint32 total = nmemb * size;
	char* mem_ptr = NULL;
	
	mem_ptr = (char*)malloc(total);
	return (void*)mem_ptr;
}


static void qr_srv_free(void* ptr)
{
    
	char* mem_ptr = (char*)ptr;
	
	//free(&mem_ptr);
	free(&mem_ptr);
}

#endif

int MQRspec_getECCLength(int version, QRecLevel level)
{
	return mqrspecCapacity[version].ec[level];
}

QRinput* QRinput_new2(int version, QRecLevel level)
{
	QRinput *input;

	if(version < 0 || version > QRSPEC_VERSION_MAX || level > QR_ECLEVEL_H) 
	{
		//errno = EINVAL;
		return NULL;
	}

	input = (QRinput *)qr_srv_malloc(sizeof(QRinput));
	if(input == NULL)
	{
		return NULL;
	}

	input->head = NULL;
	input->tail = NULL;
	input->version = version;
	input->level = level;
	input->mqr = 0;
	input->fnc1 = 0;

	return input;
}

QRinput *QRinput_newMQR(int version, QRecLevel level)
{
	QRinput *input;

	if(version <= 0 || version > MQRSPEC_VERSION_MAX)
	{
		goto INVALID;
	}
	
	if((MQRspec_getECCLength(version, level) == 0))
	{
		goto INVALID;
	}

	input = QRinput_new2(version, level);
	if(input == NULL)
	{
		return NULL;
	}

	input->mqr = 1;

	return input;

INVALID:
	//errno = EINVAL;
	return NULL;
}



/******************************************************************************
 * Numeric data
 *****************************************************************************/

/**
 * Check the input data.
 * @param size
 * @param data
 * @return result
 */
static int QRinput_checkModeNum(int size, const char *data)
{
	int i;

	for(i=0; i<size; i++) 
	{
		if(data[i] < '0' || data[i] > '9')
			return -1;
	}

	return 0;
}

/**
 * Check the input data.
 * @param size
 * @param data
 * @return result
 */
static int QRinput_checkModeAn(int size, const char *data)
{
	int i;

	for(i=0; i<size; i++) 
	{
		if(QRinput_lookAnTable(data[i]) < 0)
			return -1;
	}

	return 0;
}

/**
 * Check the input data.
 * @param size
 * @param data
 * @return result
 */
static int QRinput_checkModeKanji(int size, const unsigned char *data)
{
	int i;
	unsigned int val;

	if(size & 1)
		return -1;

	for(i=0; i<size; i+=2) 
	{
		val = ((unsigned int)data[i] << 8) | data[i+1];
		if(val < 0x8140 || (val > 0x9ffc && val < 0xe040) || val > 0xebbf) 
		{
			return -1;
		}
	}

	return 0;
}

static int QRinput_checkModeFNC1Second(int size, const unsigned char *data)
{
	if(size != 1)
		return -1;

	return 0;
}


/******************************************************************************
 * Validation
 *****************************************************************************/

int QRinput_check(QRencodeMode mode, int size, const unsigned char *data)
{
	if((mode == QR_MODE_FNC1FIRST && size < 0) || size <= 0)
	{
		return -1;
	}

	switch(mode)
	{
		case QR_MODE_NUM:
			return QRinput_checkModeNum(size, (const char *)data);
		case QR_MODE_AN:
			return QRinput_checkModeAn(size, (const char *)data);
		case QR_MODE_KANJI:
			return QRinput_checkModeKanji(size, data);
		case QR_MODE_8:
			return 0;
		case QR_MODE_STRUCTURE:
			return 0;
		case QR_MODE_ECI:
			return 0;
		case QR_MODE_FNC1FIRST:
			return 0;
		case QR_MODE_FNC1SECOND:
			return QRinput_checkModeFNC1Second(size, data);
		case QR_MODE_NUL:
			break;
	}

	return -1;
}


/******************************************************************************
 * Entry of input data
 *****************************************************************************/

static QRinput_List *QRinput_List_newEntry(QRencodeMode mode, int size, const unsigned char *data)
{
	QRinput_List *entry;

	if(QRinput_check(mode, size, data)) 
	{
		//errno = EINVAL;
		return NULL;
	}

	entry = (QRinput_List *)qr_srv_malloc(sizeof(QRinput_List));
	if(entry == NULL)
	{
		return NULL;
	}

	entry->mode = mode;
	entry->size = size;
	if(size > 0) 
	{
		entry->data = (unsigned char *)qr_srv_malloc(size);
		if(entry->data == NULL) 
		{
			qr_srv_free(entry);
			return NULL;
		}
		memcpy(entry->data, data, size);
	}
	entry->bstream = NULL;
	entry->next = NULL;

	return entry;
}

static void QRinput_appendEntry(QRinput *input, QRinput_List *entry)
{
	if(input->tail == NULL) 
	{
		input->head = entry;
		input->tail = entry;
	} 
	else 
	{
		input->tail->next = entry;
		input->tail = entry;
	}
	entry->next = NULL;
}


int QRinput_append(QRinput *input, QRencodeMode mode, int size, const unsigned char *data)
{
	QRinput_List *entry;

	entry = QRinput_List_newEntry(mode, size, data);//保存模式，尺寸，数据，通过 QRinput_List *entry;
	if(entry == NULL) 
	{
		return -1;
	}

	QRinput_appendEntry(input, entry); //将entry地址 赋给input；即保存在entry数据赋给input

	return 0;
}

static QRencodeMode Split_identifyMode(const char *string, QRencodeMode hint)
{
	unsigned char c, d;
	unsigned int word;

	c = string[0];

	if(c == '\0')
	{
		return QR_MODE_NUL;
	}
	
	if(isdigit(c)) 
	{
		return QR_MODE_NUM;
	} 
	else if(isalnum(c)) 
	{
		return QR_MODE_AN;
	} 
	else if(hint == QR_MODE_KANJI) 
	{
		d = string[1];
		if(d != '\0') 
		{
			word = ((unsigned int)c << 8) | d;
			if((word >= 0x8140 && word <= 0x9ffc) || (word >= 0xe040 && word <= 0xebbf)) 
			{
				return QR_MODE_KANJI;
			}
		}
	}

	return QR_MODE_8;
}

static char *dupAndToUpper(const char *str, QRencodeMode hint)
{
	char *newstr, *p;
	QRencodeMode mode;
	int str_len = 0;

	str_len = strlen(str);
	if(str_len == 0)
	{
		return NULL;
	}
	newstr = (char *)qr_srv_malloc(str_len + 1);
	//newstr = strdup(str);
	if(newstr == NULL)
	{
		return NULL;
	}
	strcpy(newstr, str);

	p = newstr;
	while(*p != '\0')
	{
		mode = Split_identifyMode(p, hint);
		if(mode == QR_MODE_KANJI) 
		{
			p += 2;
		}
		else
		{
			if (*p >= 'a' && *p <= 'z') 
			{
				*p = (char)((int)*p - 32);
			}
			p++;
		}
	}

	return newstr;
}


/******************************************************************************
 * Utilities
 *****************************************************************************/
int QRinput_isSplittableMode(QRencodeMode mode)
{
	return (mode >= QR_MODE_NUM && mode <= QR_MODE_KANJI);
}

int QRspec_lengthIndicator(QRencodeMode mode, int version)
{
	int l;

	if(!QRinput_isSplittableMode(mode))
	{
		return 0;
	}
	
	if(version <= 9) 
	{
		l = 0;
	} 
	else if(version <= 26) 
	{
		l = 1;
	} 
	else 
	{
		l = 2;
	}

	return qrspec_lengthTableBits[mode][l];
}

/**
 * Estimates the length of the encoded bit stream of numeric data.
 * @param size
 * @return number of bits
 */
int QRinput_estimateBitsModeNum(int size)
{
	int w;
	int bits;

	w = size / 3;
	bits = w * 10;
	switch(size - w * 3) 
	{
		case 1:
			bits += 4;
			break;
		case 2:
			bits += 7;
			break;
		default:
			break;
	}

	return bits;
}

/******************************************************************************
 * 8 bit data
 *****************************************************************************/

/**
 * Estimates the length of the encoded bit stream of 8 bit data.
 * @param size
 * @return number of bits
 */
int QRinput_estimateBitsMode8(int size)
{
	return size * 8;
}

/******************************************************************************
 * Kanji data
 *****************************************************************************/

/**
 * Estimates the length of the encoded bit stream of kanji data.
 * @param size
 * @return number of bits
 */
int QRinput_estimateBitsModeKanji(int size)
{
	return (size / 2) * 13;
}
/******************************************************************************
 * ECI header
 *****************************************************************************/
static unsigned int QRinput_decodeECIfromByteArray(unsigned char *data)
{
	int i;
	unsigned int ecinum;

	ecinum = 0;
	for(i=0; i<4; i++)
	{
		ecinum = ecinum << 8;
		ecinum |= data[3-i];
	}

	return ecinum;
}

int QRinput_estimateBitsModeECI(unsigned char *data)
{
	unsigned int ecinum;

	ecinum = QRinput_decodeECIfromByteArray(data);;

	/* See Table 4 of JISX 0510:2004 pp.17. */
	if(ecinum < 128) 
	{
		return MODE_INDICATOR_SIZE + 8;
	} 
	else if(ecinum < 16384) 
	{
		return MODE_INDICATOR_SIZE + 16;
	} 
	else 
	{
		return MODE_INDICATOR_SIZE + 24;
	}
}


/**
 * Estimates the length of the encoded bit stream of alphabet-numeric data.
 * @param size
 * @return number of bits
 */
int QRinput_estimateBitsModeAn(int size)
{
	int w;
	int bits;

	w = size / 2;
	bits = w * 11;
	if(size & 1)
	{
		bits += 6;
	}

	return bits;
}

static int Split_eat8(const char *string, QRinput *input, QRencodeMode hint)
{
	const char *p, *q;
	QRencodeMode mode;
	int ret;
	int run;
	int dif;
	int la, ln;

	la = QRspec_lengthIndicator(QR_MODE_AN, input->version);
	ln = QRspec_lengthIndicator(QR_MODE_NUM, input->version);

	p = string + 1;
	while(*p != '\0') 
	{
		mode = Split_identifyMode(p, hint);
		if(mode == QR_MODE_KANJI) 
		{
			break;
		}
		if(mode == QR_MODE_NUM) 
		{
			q = p;
			while(isdigit(*q)) 
			{
				q++;
			}
			dif = QRinput_estimateBitsMode8(p - string) /* + 4 + l8 */
				+ QRinput_estimateBitsModeNum(q - p) + 4 + ln
				- QRinput_estimateBitsMode8(q - string) /* - 4 - l8 */;
			if(dif < 0) 
			{
				break;
			} 
			else 
			{
				p = q;
			}
		} 
		else if(mode == QR_MODE_AN) 
		{
			q = p;
			while(isalnum(*q)) 
			{
				q++;
			}
			dif = QRinput_estimateBitsMode8(p - string) /* + 4 + l8 */
				+ QRinput_estimateBitsModeAn(q - p) + 4 + la
				- QRinput_estimateBitsMode8(q - string) /* - 4 - l8 */;
			if(dif < 0) 
			{
				break;
			} 
			else 
			{
				p = q;
			}
		} 
		else 
		{
			p++;
		}
	}

	run = p - string;
	ret = QRinput_append(input, QR_MODE_8, run, (unsigned char *)string);
	if(ret < 0)
	{
		return -1;
	}

	return run;
}


static int Split_eatNum(const char *string, QRinput *input,QRencodeMode hint)
{
	const char *p;
	int ret;
	int run;
	int dif;
	int ln;
	QRencodeMode mode;

	ln = QRspec_lengthIndicator(QR_MODE_NUM, input->version);

	p = string;
	while(isdigit(*p))
	{
		p++;
	}
	run = p - string;
	mode = Split_identifyMode(p, hint);
	if(mode == QR_MODE_8)
	{
		dif = QRinput_estimateBitsModeNum(run) + 4 + ln
			+ QRinput_estimateBitsMode8(1) /* + 4 + l8 */
			- QRinput_estimateBitsMode8(run + 1) /* - 4 - l8 */;
		if(dif > 0)
		{
			return Split_eat8(string, input, hint);
		}
	}
	if(mode == QR_MODE_AN) 
	{
		dif = QRinput_estimateBitsModeNum(run) + 4 + ln
			+ QRinput_estimateBitsModeAn(1) /* + 4 + la */
			- QRinput_estimateBitsModeAn(run + 1) /* - 4 - la */;
		if(dif > 0)
		{
			return Split_eatAn(string, input, hint);
		}
	}

	ret = QRinput_append(input, QR_MODE_NUM, run, (unsigned char *)string);
	if(ret < 0) return -1;

	return run;
}

static int Split_eatAn(const char *string, QRinput *input, QRencodeMode hint)
{
	const char *p, *q;
	int ret;
	int run;
	int dif;
	int la, ln;

	la = QRspec_lengthIndicator(QR_MODE_AN, input->version);
	ln = QRspec_lengthIndicator(QR_MODE_NUM, input->version);

	p = string;
	while(isalnum(*p)) 
	{
		if(isdigit(*p)) 
		{
			q = p;
			while(isdigit(*q)) 
			{
				q++;
			}
			dif = QRinput_estimateBitsModeAn(p - string) /* + 4 + la */
				+ QRinput_estimateBitsModeNum(q - p) + 4 + ln
				- QRinput_estimateBitsModeAn(q - string) /* - 4 - la */;
			if(dif < 0) 
			{
				break;
			} 
			else 
			{
				p = q;
			}
		} 
		else 
		{
			p++;
		}
	}

	run = p - string;

	if(*p && !isalnum(*p)) 
	{
		dif = QRinput_estimateBitsModeAn(run) + 4 + la
			+ QRinput_estimateBitsMode8(1) /* + 4 + l8 */
			- QRinput_estimateBitsMode8(run + 1) /* - 4 - l8 */;
		if(dif > 0) 
		{
			return Split_eat8(string, input, hint);
		}
	}

	ret = QRinput_append(input, QR_MODE_AN, run, (unsigned char *)string);
	if(ret < 0)
	{
		return -1;
	}

	return run;
}

static int Split_eatKanji(const char *string, QRinput *input, QRencodeMode hint)
{
	const char *p;
	int ret;
	int run;

	p = string;
	while(Split_identifyMode(p, hint) == QR_MODE_KANJI) 
	{
		p += 2;
	}
	run = p - string;
	ret = QRinput_append(input, QR_MODE_KANJI, run, (unsigned char *)string);
	if(ret < 0)
	{
		return -1;
	}

	return run;
}

static int Split_splitString(const char *string, QRinput *input,
		QRencodeMode hint)
{
	int length;
	QRencodeMode mode;

	if(*string == '\0') return 0;

	mode = Split_identifyMode(string, hint);
	if(mode == QR_MODE_NUM) 
	{
		length = Split_eatNum(string, input, hint);
	} 
	else if(mode == QR_MODE_AN)
	{
		length = Split_eatAn(string, input, hint);
	}
	else if(mode == QR_MODE_KANJI && hint == QR_MODE_KANJI)
	{
		length = Split_eatKanji(string, input, hint);
	}
	else 
	{
		length = Split_eat8(string, input, hint);
	}
	if(length == 0) return 0;
	if(length < 0) return -1;
	return Split_splitString(&string[length], input, hint);
}

int Split_splitStringToQRinput(const char *string, QRinput *input,
		QRencodeMode hint, int casesensitive)
{
	char *newstr;
	int ret;

	if(string == NULL || *string == '\0') 
	{
		//errno = EINVAL;
		return -1;
	}
	
	if(!casesensitive) 
	{
		newstr = dupAndToUpper(string, hint);
		if(newstr == NULL) return -1;
		ret = Split_splitString(newstr, input, hint);
		qr_srv_free(newstr);
	} 
	else
	{
		ret = Split_splitString(string, input, hint);
	}

	return ret;
}

void BitStream_free(BitStream *bstream)
{
	if(bstream != NULL) 
	{
		qr_srv_free(bstream->data);
		qr_srv_free(bstream);
	}
}

static void QRinput_List_freeEntry(QRinput_List *entry)
{
	if(entry != NULL)
	{
		qr_srv_free(entry->data);
		BitStream_free(entry->bstream);
		qr_srv_free(entry);
	}
}

void QRinput_free(QRinput *input)
{
	QRinput_List *list, *next;

	if(input != NULL)
	{
		list = input->head;
		while(list != NULL) 
		{
			next = list->next;
			QRinput_List_freeEntry(list);
			list = next;
		}
		qr_srv_free(input);
	}
}

static void QRinput_InputList_freeEntry(QRinput_InputList *entry)
{
	if(entry != NULL)
	{
		QRinput_free(entry->input);
		qr_srv_free(entry);
	}
}

/******************************************************************************
 * Structured input data
 *****************************************************************************/

static QRinput_InputList *QRinput_InputList_newEntry(QRinput *input)
{
	QRinput_InputList *entry;

	entry = (QRinput_InputList *)qr_srv_malloc(sizeof(QRinput_InputList));
	if(entry == NULL) 
	{
		return NULL;
	}

	entry->input = input;
	entry->next = NULL;

	return entry;
}

QRinput_Struct *QRinput_Struct_new(void)
{
	QRinput_Struct *s;

	s = (QRinput_Struct *)qr_srv_malloc(sizeof(QRinput_Struct));
	
	if(s == NULL) 
	{
		return NULL;
	}

	s->size = 0;
	s->parity = -1;
	s->head = NULL;
	s->tail = NULL;

	return s;
}

void QRinput_Struct_free(QRinput_Struct *s)
{
	QRinput_InputList *list, *next;
	
	if(s != NULL)
	{
		list = s->head;
		while(list != NULL)
		{
			next = list->next;
			QRinput_InputList_freeEntry(list);
			list = next;
		}
		qr_srv_free(s);
	}
}

int QRinput_Struct_appendInput(QRinput_Struct *s, QRinput *input)
{
	QRinput_InputList *e;

	if(input->mqr)
	{
		//errno = EINVAL;
		return -1;
	}

	e = QRinput_InputList_newEntry(input);
	if(e == NULL) return -1;

	s->size++;
	if(s->tail == NULL) 
	{
		s->head = e;
		s->tail = e;
	} 
	else 
	{
		s->tail->next = e;
		s->tail = e;
	}

	return s->size;
}

static unsigned char QRinput_calcParity(QRinput *input)
{
	unsigned char parity = 0;
	QRinput_List *list;
	int i;

	list = input->head;
	while(list != NULL) 
	{
		if(list->mode != QR_MODE_STRUCTURE) 
		{
			for(i=list->size-1; i>=0; i--) 
			{
				parity ^= list->data[i];
			}
		}
		list = list->next;
	}

	return parity;
}

void QRinput_Struct_setParity(QRinput_Struct *s, unsigned char parity)
{
	s->parity = (int)parity;
}

static unsigned char QRinput_Struct_calcParity(QRinput_Struct *s)
{
	QRinput_InputList *list;
	unsigned char parity = 0;

	list = s->head;
	while(list != NULL)
	{
		parity ^= QRinput_calcParity(list->input);
		list = list->next;
	}

	QRinput_Struct_setParity(s, parity);

	return parity;
}

/**
 * Insert a structured-append header to the head of the input data.
 * @param input input data.
 * @param size number of structured symbols.
 * @param index index number of the symbol. (1 <= index <= size)
 * @param parity parity among input data. (NOTE: each symbol of a set of structured symbols has the same parity data)
 * @retval 0 success.
 * @retval -1 error occurred and errno is set to indeicate the error. See Execptions for the details.
 * @throw EINVAL invalid parameter.
 * @throw ENOMEM unable to allocate memory.
 */
 /*__STATIC*/
static int QRinput_insertStructuredAppendHeader(QRinput *input, int size, int index, unsigned char parity)
{
	QRinput_List *entry;
	unsigned char buf[3];

	if(size > MAX_STRUCTURED_SYMBOLS)
	{
		//errno = EINVAL;
		return -1;
	}
	if(index <= 0 || index > MAX_STRUCTURED_SYMBOLS)
	{
		//errno = EINVAL;
		return -1;
	}

	buf[0] = (unsigned char)size;
	buf[1] = (unsigned char)index;
	buf[2] = parity;
	entry = QRinput_List_newEntry(QR_MODE_STRUCTURE, 3, buf);
	if(entry == NULL)
	{
		return -1;
	}

	entry->next = input->head;
	input->head = entry;

	return 0;
}


int QRinput_Struct_insertStructuredAppendHeaders(QRinput_Struct *s)
{
	int num, i;
	QRinput_InputList *list;

	if(s->parity < 0)
	{
		QRinput_Struct_calcParity(s);
	}
	num = 0;
	list = s->head;
	while(list != NULL)
	{
		num++;
		list = list->next;
	}
	i = 1;
	list = s->head;
	while(list != NULL)
	{
		if(QRinput_insertStructuredAppendHeader(list->input, num, i, s->parity))
			return -1;
		i++;
		list = list->next;
	}

	return 0;
}

/******************************************************************************
 * Structured QR-code encoding
 *****************************************************************************/

static QRcode_List *QRcode_List_newEntry(void)
{
	QRcode_List *entry;

	entry = (QRcode_List *)qr_srv_malloc(sizeof(QRcode_List));
	if(entry == NULL) return NULL;

	entry->next = NULL;
	entry->code = NULL;

	return entry;
}
int MQRspec_getDataLengthBit(int version, QRecLevel level)
{
	int w;
	int ecc;

	w = mqrspecCapacity[version].width - 1;
	ecc = mqrspecCapacity[version].ec[level];
	if(ecc == 0) return 0;
	return w * w - 64 - ecc * 8;
}
int MQRspec_getDataLength(int version, QRecLevel level)
{
	return (MQRspec_getDataLengthBit(version, level) + 4) / 8;
}

int QRspec_maximumWords(QRencodeMode mode, int version)
{
	int l;
	int bits;
	int words;

	if(!QRinput_isSplittableMode(mode))
	{
		return 0;
	}
	if(version <= 9)
	{
		l = 0;
	} 
	else if(version <= 26)
	{
		l = 1;
	}
	else
	{
		l = 2;
	}

	bits = qrspec_lengthTableBits[mode][l];
	words = (1 << bits) - 1;
	if(mode == QR_MODE_KANJI)
	{
		words *= 2; // the number of bytes is required
	}

	return words;
}

BitStream *BitStream_new(void)
{
	BitStream *bstream;

	bstream = (BitStream *)qr_srv_malloc(sizeof(BitStream));
	if(bstream == NULL) return NULL;

	bstream->length = 0;
	bstream->data = NULL;

	return bstream;
}

static int BitStream_allocate(BitStream *bstream, int length)
{
	unsigned char *data;

	if(bstream == NULL)
	{
		return -1;
	}

	data = (unsigned char *)qr_srv_malloc(length);
	if(data == NULL)
	{
		return -1;
	}

	if(bstream->data)
	{
		qr_srv_free(bstream->data);
	}
	bstream->length = length;
	bstream->data = data;

	return 0;
}
int BitStream_append(BitStream *bstream, BitStream *arg)
{
	unsigned char *data;

	if(arg == NULL)
	{
		return -1;
	}
	if(arg->length == 0) 
	{
		return 0;
	}
	if(bstream->length == 0)
	{
		if(BitStream_allocate(bstream, arg->length))
		{
			return -1;
		}
		memcpy(bstream->data, arg->data, arg->length);
		return 0;
	}

	data = (unsigned char *)qr_srv_malloc(bstream->length + arg->length);
	if(data == NULL)
	{
		return -1;
	}
	memcpy(data, bstream->data, bstream->length);
	memcpy(data + bstream->length, arg->data, arg->length);

	qr_srv_free(bstream->data);
	bstream->length += arg->length;
	bstream->data = data;

	return 0;
}

static BitStream *BitStream_newFromNum(int bits, unsigned int num)
{
	unsigned int mask;
	int i;
	unsigned char *p;
	BitStream *bstream;

	bstream = BitStream_new();
	if(bstream == NULL) return NULL;

	if(BitStream_allocate(bstream, bits))
	{
		BitStream_free(bstream);
		return NULL;
	}

	p = bstream->data;
	mask = 1 << (bits - 1);
	for(i=0; i<bits; i++)
	{
		if(num & mask)
		{
			*p = 1;
		}
		else
		{
			*p = 0;
		}
		p++;
		mask = mask >> 1;
	}

	return bstream;
}
int BitStream_appendNum(BitStream *bstream, int bits, unsigned int num)
{
	BitStream *b;
	int ret;

	if(bits == 0) return 0;

	b = BitStream_newFromNum(bits, num);
	if(b == NULL) return -1;

	ret = BitStream_append(bstream, b);
	BitStream_free(b);

	return ret;
}

int MQRspec_lengthIndicator(QRencodeMode mode, int version)
{
	return mqrspec_lengthTableBits[mode][version - 1];
}

/**
 * Convert the number data to a bit stream.
 * @param entry
 * @param mqr
 * @retval 0 success
 * @retval -1 an error occurred and errno is set to indeicate the error.
 *            See Execptions for the details.
 * @throw ENOMEM unable to allocate memory.
 */
static int QRinput_encodeModeNum(QRinput_List *entry, int version, int mqr)
{
	int words, i, ret;
	unsigned int val;

	entry->bstream = BitStream_new();
	if(entry->bstream == NULL) return -1;

	if(mqr)
	{
		if(version > 1)
		{
			ret = BitStream_appendNum(entry->bstream, version - 1, MQRSPEC_MODEID_NUM);
			if(ret < 0)
			{
				goto ABORT;
			}
		}
		ret = BitStream_appendNum(entry->bstream, MQRspec_lengthIndicator(QR_MODE_NUM, version), entry->size);
			if(ret < 0)
			{
				goto ABORT;
			}
	} 
	else 
	{
		ret = BitStream_appendNum(entry->bstream, 4, QRSPEC_MODEID_NUM);
			if(ret < 0)
			{
					goto ABORT;
			}
	
		ret = BitStream_appendNum(entry->bstream, QRspec_lengthIndicator(QR_MODE_NUM, version), entry->size);
			if(ret < 0)
			{
					goto ABORT;
			}
	}

	words = entry->size / 3;
	for(i=0; i<words; i++) 
	{
		val  = (entry->data[i*3  ] - '0') * 100;
		val += (entry->data[i*3+1] - '0') * 10;
		val += (entry->data[i*3+2] - '0');

		ret = BitStream_appendNum(entry->bstream, 10, val);
		if(ret < 0) goto ABORT;
	}

	if(entry->size - words * 3 == 1)
	{
		val = entry->data[words*3] - '0';
		ret = BitStream_appendNum(entry->bstream, 4, val);
		if(ret < 0) goto ABORT;
	}
	else if(entry->size - words * 3 == 2)
	{
		val  = (entry->data[words*3  ] - '0') * 10;
		val += (entry->data[words*3+1] - '0');
		BitStream_appendNum(entry->bstream, 7, val);
		if(ret < 0) goto ABORT;
	}

	return 0;
ABORT:
	BitStream_free(entry->bstream);
	entry->bstream = NULL;
	return -1;
}


/**
 * Convert the alphabet-numeric data to a bit stream.
 * @param entry
 * @param mqr
 * @retval 0 success
 * @retval -1 an error occurred and errno is set to indeicate the error.
 *            See Execptions for the details.
 * @throw ENOMEM unable to allocate memory.
 * @throw EINVAL invalid version.
 */
static int QRinput_encodeModeAn(QRinput_List *entry, int version, int mqr)
{
	int words, i, ret;
	unsigned int val;

	entry->bstream = BitStream_new();
	if(entry->bstream == NULL) return -1;

	if(mqr)
	{
		if(version < 2)
		{
//			errno = EINVAL;
			goto ABORT;
		}
		ret = BitStream_appendNum(entry->bstream, version - 1, MQRSPEC_MODEID_AN);
		if(ret < 0) goto ABORT;
		ret = BitStream_appendNum(entry->bstream, MQRspec_lengthIndicator(QR_MODE_AN, version), entry->size);
		if(ret < 0) goto ABORT;
	}
	else
	{
		ret = BitStream_appendNum(entry->bstream, 4, QRSPEC_MODEID_AN);
		if(ret < 0) goto ABORT;
		ret = BitStream_appendNum(entry->bstream, QRspec_lengthIndicator(QR_MODE_AN, version), entry->size);
		if(ret < 0) goto ABORT;
	}

	words = entry->size / 2;
	for(i=0; i<words; i++)
	{
		val  = (unsigned int)QRinput_lookAnTable(entry->data[i*2  ]) * 45;
		val += (unsigned int)QRinput_lookAnTable(entry->data[i*2+1]);

		ret = BitStream_appendNum(entry->bstream, 11, val);
		if(ret < 0) goto ABORT;
	}

	if(entry->size & 1)
	{
		val = (unsigned int)QRinput_lookAnTable(entry->data[words * 2]);

		ret = BitStream_appendNum(entry->bstream, 6, val);
		if(ret < 0) goto ABORT;
	}

	return 0;
ABORT:
	BitStream_free(entry->bstream);
	entry->bstream = NULL;
	return -1;
}



/**
 * Convert the 8bits data to a bit stream.
 * @param entry
 * @param mqr
 * @retval 0 success
 * @retval -1 an error occurred and errno is set to indeicate the error.
 *            See Execptions for the details.
 * @throw ENOMEM unable to allocate memory.
 */
static int QRinput_encodeMode8(QRinput_List *entry, int version, int mqr)
{
	int ret;

	entry->bstream = BitStream_new();
	if(entry->bstream == NULL) return -1;

	if(mqr)
	{
		if(version < 3)
		{
//			errno = EINVAL;
			goto ABORT;
		}
		ret = BitStream_appendNum(entry->bstream, version - 1, MQRSPEC_MODEID_8);
		if(ret < 0) goto ABORT;
		ret = BitStream_appendNum(entry->bstream, MQRspec_lengthIndicator(QR_MODE_8, version), entry->size);
		if(ret < 0) goto ABORT;
	} 
	else 
	{
		ret = BitStream_appendNum(entry->bstream, 4, QRSPEC_MODEID_8);
		if(ret < 0) goto ABORT;
		ret = BitStream_appendNum(entry->bstream, QRspec_lengthIndicator(QR_MODE_8, version), entry->size);
		if(ret < 0) goto ABORT;
	}

	ret = BitStream_appendBytes(entry->bstream, entry->size, entry->data);
	if(ret < 0) goto ABORT;

	return 0;
ABORT:
	BitStream_free(entry->bstream);
	entry->bstream = NULL;
	return -1;
}


/**
 * Convert the kanji data to a bit stream.
 * @param entry
 * @param mqr
 * @retval 0 success
 * @retval -1 an error occurred and errno is set to indeicate the error.
 *            See Execptions for the details.
 * @throw ENOMEM unable to allocate memory.
 * @throw EINVAL invalid version.
 */
static int QRinput_encodeModeKanji(QRinput_List *entry, int version, int mqr)
{
	int ret, i;
	unsigned int val, h;

	entry->bstream = BitStream_new();
	if(entry->bstream == NULL) return -1;

	if(mqr)
	{
		if(version < 2)
		{
//			errno = EINVAL;
			goto ABORT;
		}
		ret = BitStream_appendNum(entry->bstream, version - 1, MQRSPEC_MODEID_KANJI);
		if(ret < 0) goto ABORT;
		ret = BitStream_appendNum(entry->bstream, MQRspec_lengthIndicator(QR_MODE_KANJI, version), entry->size/2);
		if(ret < 0) goto ABORT;
	} 
	else 
	{
		ret = BitStream_appendNum(entry->bstream, 4, QRSPEC_MODEID_KANJI);
		if(ret < 0) goto ABORT;
		ret = BitStream_appendNum(entry->bstream, QRspec_lengthIndicator(QR_MODE_KANJI, version), entry->size/2);
		if(ret < 0) goto ABORT;
	}

	for(i=0; i<entry->size; i+=2)
	{
		val = ((unsigned int)entry->data[i] << 8) | entry->data[i+1];
		if(val <= 0x9ffc)
		{
			val -= 0x8140;
		} else 
		{
			val -= 0xc140;
		}
		h = (val >> 8) * 0xc0;
		val = (val & 0xff) + h;

		ret = BitStream_appendNum(entry->bstream, 13, val);
		if(ret < 0) goto ABORT;
	}

	return 0;
ABORT:
	BitStream_free(entry->bstream);
	entry->bstream = NULL;
	return -1;
}


/******************************************************************************
 * Structured Symbol
 *****************************************************************************/

/**
 * Convert a structure symbol code to a bit stream.
 * @param entry
 * @param mqr
 * @retval 0 success
 * @retval -1 an error occurred and errno is set to indeicate the error.
 *            See Execptions for the details.
 * @throw ENOMEM unable to allocate memory.
 * @throw EINVAL invalid entry.
 */
static int QRinput_encodeModeStructure(QRinput_List *entry, int mqr)
{
	int ret;

	if(mqr) 
	{
//		errno = EINVAL;
		return -1;
	}
	entry->bstream = BitStream_new();
	if(entry->bstream == NULL) return -1;

	ret = BitStream_appendNum(entry->bstream, 4, QRSPEC_MODEID_STRUCTURE);
	if(ret < 0) goto ABORT;
	ret = BitStream_appendNum(entry->bstream, 4, entry->data[1] - 1);
	if(ret < 0) goto ABORT;
	ret = BitStream_appendNum(entry->bstream, 4, entry->data[0] - 1);
	if(ret < 0) goto ABORT;
	ret = BitStream_appendNum(entry->bstream, 8, entry->data[2]);
	if(ret < 0) goto ABORT;

	return 0;
ABORT:
	BitStream_free(entry->bstream);
	entry->bstream = NULL;
	return -1;
}

static int QRinput_encodeModeECI(QRinput_List *entry, int version)
{
	int ret, words;
	unsigned int ecinum, code;

	entry->bstream = BitStream_new();
	if(entry->bstream == NULL) return -1;

	ecinum = QRinput_decodeECIfromByteArray(entry->data);

	/* See Table 4 of JISX 0510:2004 pp.17. */
	if(ecinum < 128) 
	{
		words = 1;
		code = ecinum;
	} else if(ecinum < 16384)
	{
		words = 2;
		code = 0x8000 + ecinum;
	} 
	else 
	{
		words = 3;
		code = 0xc0000 + ecinum;
	}

	ret = BitStream_appendNum(entry->bstream, 4, QRSPEC_MODEID_ECI);
	if(ret < 0) goto ABORT;
	
	ret = BitStream_appendNum(entry->bstream, words * 8, code);
	if(ret < 0) goto ABORT;

	return 0;
ABORT:
	BitStream_free(entry->bstream);
	entry->bstream = NULL;
	return -1;
}


static int QRinput_encodeModeFNC1Second(QRinput_List *entry, int version)
{
	int ret;

	entry->bstream = BitStream_new();
	if(entry->bstream == NULL) return -1;

	ret = BitStream_appendNum(entry->bstream, 4, QRSPEC_MODEID_FNC1SECOND);
	if(ret < 0) goto ABORT;
	
	ret = BitStream_appendBytes(entry->bstream, 1, entry->data);
	if(ret < 0) goto ABORT;

	return 0;
ABORT:
	BitStream_free(entry->bstream);
	entry->bstream = NULL;
	return -1;
}

/**
 * Convert the input data in the data chunk to a bit stream.
 * @param entry
 * @return number of bits (>0) or -1 for failure.
 */
static int QRinput_encodeBitStream(QRinput_List *entry, int version, int mqr)
{
	int words, ret;
	QRinput_List *st1 = NULL, *st2 = NULL;

	if(entry->bstream != NULL) 
	{
		BitStream_free(entry->bstream);
		entry->bstream = NULL;
	}

	words = QRspec_maximumWords(entry->mode, version);
	if(words != 0 && entry->size > words) 
	{
		st1 = QRinput_List_newEntry(entry->mode, words, entry->data);
		if(st1 == NULL)
		{
			goto ABORT;
		}
		st2 = QRinput_List_newEntry(entry->mode, entry->size - words, &entry->data[words]);
		if(st2 == NULL) 
		{
			goto ABORT;
		}

		ret = QRinput_encodeBitStream(st1, version, mqr);
		if(ret < 0)
		{
			goto ABORT;
		}
		ret = QRinput_encodeBitStream(st2, version, mqr);
		if(ret < 0) 
		{
			goto ABORT;
		}
		entry->bstream = BitStream_new();
		if(entry->bstream == NULL) 
		{
			goto ABORT;
		}
		ret = BitStream_append(entry->bstream, st1->bstream);
		if(ret < 0) 
		{
			goto ABORT;
		}
		ret = BitStream_append(entry->bstream, st2->bstream);
		if(ret < 0) 
		{
			goto ABORT;
		}
		QRinput_List_freeEntry(st1);
		QRinput_List_freeEntry(st2);
	} 
	else 
	{
		ret = 0;
		switch(entry->mode) 
		{
			case QR_MODE_NUM:
				ret = QRinput_encodeModeNum(entry, version, mqr);
				break;
			case QR_MODE_AN:
				ret = QRinput_encodeModeAn(entry, version, mqr);
				break;
			case QR_MODE_8:
				ret = QRinput_encodeMode8(entry, version, mqr);
				break;
			case QR_MODE_KANJI:
				ret = QRinput_encodeModeKanji(entry, version, mqr);
				break;
			case QR_MODE_STRUCTURE:
				ret = QRinput_encodeModeStructure(entry, mqr);
				break;
			case QR_MODE_ECI:
				ret = QRinput_encodeModeECI(entry, version);
				break;
			case QR_MODE_FNC1SECOND:
				ret = QRinput_encodeModeFNC1Second(entry, version);
			default:
				break;
		}
		if(ret < 0) return -1;
	}

	return BitStream_size(entry->bstream);
ABORT:
	QRinput_List_freeEntry(st1);
	QRinput_List_freeEntry(st2);
	return -1;
}

/**
 * Convert the input data to a bit stream.
 * @param input input data.
 * @retval 0 success
 * @retval -1 an error occurred and errno is set to indeicate the error.
 *            See Execptions for the details.
 * @throw ENOMEM unable to allocate memory.
 */
static int QRinput_createBitStream(QRinput *input)
{
	QRinput_List *list;
	int bits, total = 0;

	list = input->head;
	while(list != NULL)
	{
		bits = QRinput_encodeBitStream(list, input->version, input->mqr);
		if(bits < 0) return -1;
		total += bits;
		list = list->next;
	}

	return total;
}

static int QRinput_insertFNC1Header(QRinput *input)
{
	QRinput_List *entry = NULL;

	if(input->fnc1 == 1)
	{
		entry = QRinput_List_newEntry(QR_MODE_FNC1FIRST, 0, NULL);
	}
	else if(input->fnc1 == 2) 
	{
		entry = QRinput_List_newEntry(QR_MODE_FNC1SECOND, 1, &(input->appid));
	}
	if(entry == NULL)
	{
		return -1;
	}

	if(input->head->mode != QR_MODE_STRUCTURE || input->head->mode != QR_MODE_ECI)
	{
		entry->next = input->head;
		input->head = entry;
	}
	else 
	{
		entry->next = input->head->next;
		input->head->next = entry;
	}

	return 0;
}


/******************************************************************************
 * Estimation of the bit length
 *****************************************************************************/

/**
 * Estimates the length of the encoded bit stream on the current version.
 * @param entry
 * @param version version of the symbol
 * @param mqr
 * @return number of bits
 */
static int QRinput_estimateBitStreamSizeOfEntry(QRinput_List *entry, int version, int mqr)
{
	int bits = 0;
	int l, m;
	int num;

	if(version == 0) version = 1;

	switch(entry->mode)
	{
		case QR_MODE_NUM:
			bits = QRinput_estimateBitsModeNum(entry->size);
			break;
		case QR_MODE_AN:
			bits = QRinput_estimateBitsModeAn(entry->size);
			break;
		case QR_MODE_8:
			bits = QRinput_estimateBitsMode8(entry->size);
			break;
		case QR_MODE_KANJI:
			bits = QRinput_estimateBitsModeKanji(entry->size);
			break;
		case QR_MODE_STRUCTURE:
			return STRUCTURE_HEADER_SIZE;
		case QR_MODE_ECI:
			bits = QRinput_estimateBitsModeECI(entry->data);
			break;
		case QR_MODE_FNC1FIRST:
			return MODE_INDICATOR_SIZE;
			break;
		case QR_MODE_FNC1SECOND:
			return MODE_INDICATOR_SIZE + 8;
		default:
			return 0;
	}

	if(mqr)
	{
		l = QRspec_lengthIndicator(entry->mode, version);
		m = version - 1;
		bits += l + m;
	} 
	else 
	{
		l = QRspec_lengthIndicator(entry->mode, version);
		m = 1 << l;
		num = (entry->size + m - 1) / m;

		bits += num * (MODE_INDICATOR_SIZE + l);
	}

	return bits;
}

/**
 * Estimates the length of the encoded bit stream of the data.
 * @param input input data
 * @param version version of the symbol
 * @return number of bits
 __STATIC
 */
static int QRinput_estimateBitStreamSize(QRinput *input, int version)
{
	QRinput_List *list;
	int bits = 0;

	list = input->head;
	while(list != NULL)
	{
		bits += QRinput_estimateBitStreamSizeOfEntry(list, version, input->mqr);
		list = list->next;
	}

	return bits;
}

/**
 * Estimates the required version number of the symbol.
 * @param input input data
 * @return required version number
 */
static int QRinput_estimateVersion(QRinput *input)
{
	int bits;
	int version, prev;

	version = 0;
	do {
		prev = version;
		bits = QRinput_estimateBitStreamSize(input, prev);
		version = QRspec_getMinimumVersion((bits + 7) / 8, input->level);
		if (version < 0)
		{
			return -1;
		}
	} while (version > prev);

	return version;
}

int QRinput_getVersion(QRinput *input)
{
	return input->version;
}
int QRspec_getMinimumVersion(int size, QRecLevel level)
{
	int i;
	int words;

	for(i=1; i<= QRSPEC_VERSION_MAX; i++)
	{
		words  = qrspecCapacity[i].words - qrspecCapacity[i].ec[level];
		if(words >= size) return i;
	}

	return -1;
}

/**
 * Convert the input data to a bit stream.
 * When the version number is given and that is not sufficient, it is increased
 * automatically.
 * @param input input data.
 * @retval 0 success
 * @retval -1 an error occurred and errno is set to indeicate the error.
 *            See Execptions for the details.
 * @throw ENOMEM unable to allocate memory.
 * @throw ERANGE input is too large.
 */
static int QRinput_convertData(QRinput *input)
{
	int bits;
	int ver;

	ver = QRinput_estimateVersion(input);
	if(ver > QRinput_getVersion(input))
	{
		QRinput_setVersion(input, ver);
	}

	for(;;) 
	{
		bits = QRinput_createBitStream(input);
		if(bits < 0) return -1;
		ver = QRspec_getMinimumVersion((bits + 7) / 8, input->level);
		if(ver < 0)
		{
//			errno = ERANGE;
			return -1;
		} 
		else if(ver > QRinput_getVersion(input))
		{
			QRinput_setVersion(input, ver);
		} 
		else
		{
			break;
		}
	}

	return 0;
}

/**
 * Merge all bit streams in the input data.
 * @param input input data.
 * @return merged bit stream
 */
/*__STATIC*/
static BitStream *QRinput_mergeBitStream(QRinput *input)
{
	BitStream *bstream;
	QRinput_List *list;
	int ret;

	if(input->mqr)
	{
		if(QRinput_createBitStream(input) < 0)
		{
			return NULL;
		}
	} 
	else
	{
		if(input->fnc1)
		{
			if(QRinput_insertFNC1Header(input) < 0)
			{
				return NULL;
			}
		}
		if(QRinput_convertData(input) < 0) 
		{
			return NULL;
		}
	}

	bstream = BitStream_new();
	if(bstream == NULL) return NULL;

	list = input->head;
	while(list != NULL)
	{
		ret = BitStream_append(bstream, list->bstream);
		if(ret < 0)
		{
			BitStream_free(bstream);
			return NULL;
		}
		list = list->next;
	}

	return bstream;
}

static BitStream *BitStream_newFromBytes(int size, unsigned char *data)
{
	unsigned char mask;
	int i, j;
	unsigned char *p;
	BitStream *bstream;

	bstream = BitStream_new();
	if(bstream == NULL) return NULL;

	if(BitStream_allocate(bstream, size * 8))
	{
		BitStream_free(bstream);
		return NULL;
	}

	p = bstream->data;
	for(i=0; i<size; i++)
	{
		mask = 0x80;
		for(j=0; j<8; j++)
		{
			if(data[i] & mask)
			{
				*p = 1;
			} 
			else 
			{
				*p = 0;
			}
			p++;
			mask = mask >> 1;
		}
	}

	return bstream;
}
int BitStream_appendBytes(BitStream *bstream, int size, unsigned char *data)
{
	BitStream *b;
	int ret;

	if(size == 0) return 0;

	b = BitStream_newFromBytes(size, data);
	if(b == NULL) return -1;

	ret = BitStream_append(bstream, b);
	BitStream_free(b);

	return ret;
}
/**
 * Append padding bits for the input data - Micro QR Code version.
 * @param bstream Bitstream to be appended.
 * @param input input data.
 * @retval 0 success
 * @retval -1 an error occurred and errno is set to indeicate the error.
 *            See Execptions for the details.
 * @throw ERANGE input data is too large.
 * @throw ENOMEM unable to allocate memory.
 */
static int QRinput_appendPaddingBitMQR(BitStream *bstream, QRinput *input)
{
	int bits, maxbits, words, maxwords, i, ret, termbits;
	BitStream *padding = NULL;
	unsigned char *padbuf;
	int padlen;

	bits = BitStream_size(bstream);
	maxbits = MQRspec_getDataLengthBit(input->version, input->level);
	maxwords = maxbits / 8;

	if(maxbits < bits)
	{
//		errno = ERANGE;
		return -1;
	}
	if(maxbits == bits)
	{
		return 0;
	}

	termbits = input->version * 2 + 1;

	if(maxbits - bits <= termbits) 
	{
		ret = BitStream_appendNum(bstream, maxbits - bits, 0);
		goto DONE;
	}

	bits += termbits;

	words = (bits + 7) / 8;
	if(maxbits - words * 8 > 0) 
	{
		termbits += words * 8 - bits;
		if(words == maxwords) termbits += maxbits - words * 8;
	}
	else 
	{
		termbits += words * 8 - bits;
	}
	padding = BitStream_new();
	if(padding == NULL) return -1;
	ret = BitStream_appendNum(padding, termbits, 0);
	if(ret < 0) goto DONE;

	padlen = maxwords - words;
	if(padlen > 0)
	{
		padbuf = (unsigned char *)qr_srv_malloc(padlen);
		if(padbuf == NULL)
		{
			ret = -1;
			goto DONE;
		}
		for(i=0; i<padlen; i++)
		{
			padbuf[i] = (i&1)?0x11:0xec;
		}
		ret = BitStream_appendBytes(padding, padlen, padbuf);
		qr_srv_free(padbuf);
		if(ret < 0)
		{
			goto DONE;
		}
		termbits = maxbits - maxwords * 8;
		if(termbits > 0)
		{
			ret = BitStream_appendNum(padding, termbits, 0);
			if(ret < 0) goto DONE;
		}
	}

	ret = BitStream_append(bstream, padding);

DONE:
	BitStream_free(padding);
	return ret;
}

int QRspec_getDataLength(int version, QRecLevel level)
{
	return qrspecCapacity[version].words - qrspecCapacity[version].ec[level];
}

/**
 * Append padding bits for the input data.
 * @param bstream Bitstream to be appended.
 * @param input input data.
 * @retval 0 success
 * @retval -1 an error occurred and errno is set to indeicate the error.
 *            See Execptions for the details.
 * @throw ERANGE input data is too large.
 * @throw ENOMEM unable to allocate memory.
 */
static int QRinput_appendPaddingBit(BitStream *bstream, QRinput *input)
{
	int bits, maxbits, words, maxwords, i, ret;
	BitStream *padding = NULL;
	unsigned char *padbuf;
	int padlen;

	bits = BitStream_size(bstream);
	maxwords = QRspec_getDataLength(input->version, input->level);
	maxbits = maxwords * 8;

	if(maxbits < bits)
	{
//		errno = ERANGE;
		return -1;
	}
	if(maxbits == bits)
	{
		return 0;
	}

	if(maxbits - bits <= 4)
	{
		ret = BitStream_appendNum(bstream, maxbits - bits, 0);
		goto DONE;
	}

	words = (bits + 4 + 7) / 8;

	padding = BitStream_new();
	if(padding == NULL) return -1;
	ret = BitStream_appendNum(padding, words * 8 - bits, 0);
	if(ret < 0) goto DONE;

	padlen = maxwords - words;
	if(padlen > 0)
	{
		padbuf = (unsigned char *)qr_srv_malloc(padlen);
		if(padbuf == NULL) 
		{
			ret = -1;
			goto DONE;
		}
		for(i=0; i<padlen; i++)
		{
			padbuf[i] = (i&1)?0x11:0xec;
		}
		ret = BitStream_appendBytes(padding, padlen, padbuf);
		qr_srv_free(padbuf);
		if(ret < 0)
		{
			goto DONE;
		}
	}

	ret = BitStream_append(bstream, padding);

DONE:
	BitStream_free(padding);
	return ret;
}


/**
 * Merge all bit streams in the input data and append padding bits
 * @param input input data.
 * @return padded merged bit stream
 */
/*__STATIC*/
static BitStream *QRinput_getBitStream(QRinput *input)
{
	BitStream *bstream;
	int ret;

	bstream = QRinput_mergeBitStream(input);
	if(bstream == NULL) 
	{
		return NULL;
	}
	
	if(input->mqr) 
	{
		ret = QRinput_appendPaddingBitMQR(bstream, input);
	} 
	else 
	{
		ret = QRinput_appendPaddingBit(bstream, input);
	}
	
	if(ret < 0) 
	{
		BitStream_free(bstream);
		return NULL;
	}

	return bstream;
}

unsigned char *BitStream_toByte(BitStream *bstream)
{
	int i, j, size, bytes;
	unsigned char *data, v;
	unsigned char *p;

	size = BitStream_size(bstream);
	if(size == 0)
	{
		return NULL;
	}
	data = (unsigned char *)qr_srv_malloc((size + 7) / 8);
	if(data == NULL)
	{
		return NULL;
	}

	bytes = size  / 8;

	p = bstream->data;
	for(i=0; i<bytes; i++)
	{
		v = 0;
		for(j=0; j<8; j++)
		{
			v = v << 1;
			v |= *p;
			p++;
		}
		data[i] = v;
	}
	if(size & 7)
	{
		v = 0;
		for(j=0; j<(size & 7); j++)
		{
			v = v << 1;
			v |= *p;
			p++;
		}
		data[bytes] = v;
	}

	return data;
}

/**
 * Pack all bit streams padding bits into a byte array.
 * @param input input data.
 * @return padded merged byte stream
 */

unsigned char *QRinput_getByteStream(QRinput *input)
{
	BitStream *bstream;
	unsigned char *array;

	bstream = QRinput_getBitStream(input);
	if(bstream == NULL)
	{
		return NULL;
	}
	array = BitStream_toByte(bstream);
	BitStream_free(bstream);

	return array;
}
/*__STATIC*/
static void MQRraw_free(MQRRawCode *raw)
{
	if(raw != NULL) 
	{
		qr_srv_free(raw->datacode);
		qr_srv_free(raw->ecccode);
		qr_srv_free(raw->rsblock);
		qr_srv_free(raw);
	}
}

static /*inline */int modnn(RS *rs, int x)
{
	while (x >= rs->nn)
	{
		x -= rs->nn;
		x = (x >> rs->mm) + (x & rs->nn);
	}
	return x;
}

/* Initialize a Reed-Solomon codec
 * symsize = symbol size, bits
 * gfpoly = Field generator polynomial coefficients
 * fcr = first root of RS code generator polynomial, index form
 * prim = primitive element to generate polynomial roots
 * nroots = RS code generator polynomial degree (number of roots)
 * pad = padding bytes at front of shortened block
 */
static RS *init_rs_char(int symsize, int gfpoly, int fcr, int prim, int nroots, int pad)
{
  RS *rs;


/* Common code for intializing a Reed-Solomon control block (char or int symbols)
 * Copyright 2004 Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */
//#undef NULL
//#define NULL ((void *)0)

  int i, j, sr,root,iprim;

  rs = NULL;
  /* Check parameter ranges */
  if(symsize < 0 || symsize > (int)(8*sizeof(data_t)))
  {
    goto done;
  }

  if(fcr < 0 || fcr >= (1<<symsize))
    goto done;
  if(prim <= 0 || prim >= (1<<symsize))
    goto done;
  if(nroots < 0 || nroots >= (1<<symsize))
    goto done; /* Can't have more roots than symbol values! */
  if(pad < 0 || pad >= ((1<<symsize) -1 - nroots))
    goto done; /* Too much padding */

  rs = (RS *)qr_srv_calloc(1,sizeof(RS));
  if(rs == NULL)
    goto done;

  rs->mm = symsize;
  rs->nn = (1<<symsize)-1;
  rs->pad = pad;

  rs->alpha_to = (data_t *)qr_srv_malloc(sizeof(data_t)*(rs->nn+1));
  if(rs->alpha_to == NULL)
  {
    qr_srv_free(rs);
    rs = NULL;
    goto done;
  }
  rs->index_of = (data_t *)qr_srv_malloc(sizeof(data_t)*(rs->nn+1));
  if(rs->index_of == NULL)
  {
    qr_srv_free(rs->alpha_to);
    qr_srv_free(rs);
    rs = NULL;
    goto done;
  }

  /* Generate Galois field lookup tables */
  rs->index_of[0] = A0; /* log(zero) = -inf */
  rs->alpha_to[A0] = 0; /* alpha**-inf = 0 */
  sr = 1;
  for(i=0;i<rs->nn;i++)
  {
    rs->index_of[sr] = i;
    rs->alpha_to[i] = sr;
    sr <<= 1;
    if(sr & (1<<symsize))
      sr ^= gfpoly;
    sr &= rs->nn;
  }
  if(sr != 1)
  {
    /* field generator polynomial is not primitive! */
    qr_srv_free(rs->alpha_to);
    qr_srv_free(rs->index_of);
    qr_srv_free(rs);
    rs = NULL;
    goto done;
  }

  /* Form RS code generator polynomial from its roots */
  rs->genpoly = (data_t *)qr_srv_malloc(sizeof(data_t)*(nroots+1));
  if(rs->genpoly == NULL)
  {
    qr_srv_free(rs->alpha_to);
    qr_srv_free(rs->index_of);
    qr_srv_free(rs);
    rs = NULL;
    goto done;
  }
  rs->fcr = fcr;
  rs->prim = prim;
  rs->nroots = nroots;
  rs->gfpoly = gfpoly;

  /* Find prim-th root of 1, used in decoding */
  for(iprim=1;(iprim % prim) != 0;iprim += rs->nn)
    ;
  rs->iprim = iprim / prim;

  rs->genpoly[0] = 1;
  for (i = 0,root=fcr*prim; i < nroots; i++,root += prim) 
  {
    rs->genpoly[i+1] = 1;

    /* Multiply rs->genpoly[] by  @**(root + x) */
    for (j = i; j > 0; j--)
	{
      if (rs->genpoly[j] != 0)
	rs->genpoly[j] = rs->genpoly[j-1] ^ rs->alpha_to[modnn(rs,rs->index_of[rs->genpoly[j]] + root)];
      else
	rs->genpoly[j] = rs->genpoly[j-1];
    }
    /* rs->genpoly[0] can never be zero */
    rs->genpoly[0] = rs->alpha_to[modnn(rs,rs->index_of[rs->genpoly[0]] + root)];
  }
  /* convert rs->genpoly[] to index form for quicker encoding */
  for (i = 0; i <= nroots; i++)
    rs->genpoly[i] = rs->index_of[rs->genpoly[i]];
 done:;

  return rs;
}

RS *init_rs(int symsize, int gfpoly, int fcr, int prim, int nroots, int pad)
{
	RS *rs;

#ifdef HAVE_LIBPTHREAD
	pthread_mutex_lock(&rslist_mutex);
#endif
	for(rs = rslist; rs != NULL; rs = rs->next) 
	{
		if(rs->pad != pad) continue;
		if(rs->nroots != nroots) continue;
		if(rs->mm != symsize) continue;
		if(rs->gfpoly != gfpoly) continue;
		if(rs->fcr != fcr) continue;
		if(rs->prim != prim) continue;

		goto DONE;
	}

	rs = init_rs_char(symsize, gfpoly, fcr, prim, nroots, pad);
	if(rs == NULL) goto DONE;
	rs->next = rslist;
	rslist = rs;

DONE:
#ifdef HAVE_LIBPTHREAD
	pthread_mutex_unlock(&rslist_mutex);
#endif
	return rs;
}

/* The guts of the Reed-Solomon encoder, meant to be #included
 * into a function body with the following typedefs, macros and variables supplied
 * according to the code parameters:

 * data_t - a typedef for the data symbol
 * data_t data[] - array of NN-NROOTS-PAD and type data_t to be encoded
 * data_t parity[] - an array of NROOTS and type data_t to be written with parity symbols
 * NROOTS - the number of roots in the RS code generator polynomial,
 *          which is the same as the number of parity symbols in a block.
            Integer variable or literal.
	    * 
 * NN - the total number of symbols in a RS block. Integer variable or literal.
 * PAD - the number of pad symbols in a block. Integer variable or literal.
 * ALPHA_TO - The address of an array of NN elements to convert Galois field
 *            elements in index (log) form to polynomial form. Read only.
 * INDEX_OF - The address of an array of NN elements to convert Galois field
 *            elements in polynomial form to index (log) form. Read only.
 * MODNN - a function to reduce its argument modulo NN. May be inline or a macro.
 * GENPOLY - an array of NROOTS+1 elements containing the generator polynomial in index form

 * The memset() and memmove() functions are used. The appropriate header
 * file declaring these functions (usually <string.h>) must be included by the calling
 * program.

 * Copyright 2004, Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */

void encode_rs_char(RS *rs, const data_t *data, data_t *parity)
{
  int i, j;
  data_t feedback;

  memset(parity,0,NROOTS*sizeof(data_t));

  for(i=0;i<NN-NROOTS-PAD;i++)
  {
    feedback = INDEX_OF[data[i] ^ parity[0]];
    if(feedback != A0)
	{      /* feedback term is non-zero */
#ifdef UNNORMALIZED
      /* This line is unnecessary when GENPOLY[NROOTS] is unity, as it must
       * always be for the polynomials constructed by init_rs()
       */
      feedback = MODNN(NN - GENPOLY[NROOTS] + feedback);
#endif
      for(j=1;j<NROOTS;j++)
	parity[j] ^= ALPHA_TO[MODNN(feedback + GENPOLY[NROOTS-j])];
    }
    /* Shift */
    memmove(&parity[0],&parity[1],sizeof(data_t)*(NROOTS-1));
    if(feedback != A0)
      parity[NROOTS-1] = ALPHA_TO[MODNN(feedback + GENPOLY[0])];
    else
      parity[NROOTS-1] = 0;
  }
}


static void RSblock_initBlock(RSblock *block, int dl, unsigned char *data, int el, unsigned char *ecc, RS *rs)
{
	block->dataLength = dl;
	block->data = data;
	block->eccLength = el;
	block->ecc = ecc;

	encode_rs_char(rs, data, ecc);
}


/*__STATIC*/
static MQRRawCode *MQRraw_new(QRinput *input)
{
	MQRRawCode *raw;
	RS *rs;

	raw = (MQRRawCode *)qr_srv_malloc(sizeof(MQRRawCode));
	if(raw == NULL)
	{
		return NULL;
	}

	raw->version = input->version;
	raw->dataLength = MQRspec_getDataLength(input->version, input->level);
	raw->eccLength = MQRspec_getECCLength(input->version, input->level);
	raw->oddbits = raw->dataLength * 8 - MQRspec_getDataLengthBit(input->version, input->level);
	raw->datacode = QRinput_getByteStream(input);
	if(raw->datacode == NULL) 
	{
		qr_srv_free(raw);
		return NULL;
	}
	raw->ecccode = (unsigned char *)qr_srv_malloc(raw->eccLength);
	if(raw->ecccode == NULL) 
	{
		qr_srv_free(raw->datacode);
		qr_srv_free(raw);
		return NULL;
	}

	raw->rsblock = (RSblock *)qr_srv_calloc(sizeof(RSblock), 1);
	if(raw->rsblock == NULL)
	{
		MQRraw_free(raw);
		return NULL;
	}

	rs = init_rs(8, 0x11d, 0, 1, raw->eccLength, 255 - raw->dataLength - raw->eccLength);
	if(rs == NULL)
	{
		MQRraw_free(raw);
		return NULL;
	}

	RSblock_initBlock(raw->rsblock, raw->dataLength, raw->datacode, raw->eccLength, raw->ecccode, rs);

	raw->count = 0;

	return raw;
}

int MQRspec_getWidth(int version)
{
	return mqrspecCapacity[version].width;
}

static void mqrspec_putFinderPattern(unsigned char *frame, int width, int ox, int oy)
{
	static const unsigned char finder[] = {
		0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
		0xc1, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1,
		0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
	};
	int x, y;
	const unsigned char *s;

	frame += oy * width + ox;
	s = finder;
	for(y=0; y<7; y++) {
		for(x=0; x<7; x++) {
			frame[x] = s[x];
		}
		frame += width;
		s += 7;
	}
}
static unsigned char *MQRspec_createFrame(int version)
{
	unsigned char *frame, *p, *q;
	int width;
	int x, y;

	width = mqrspecCapacity[version].width;
	frame = (unsigned char *)qr_srv_malloc(width * width);
	if(frame == NULL) return NULL;

	memset(frame, 0, width * width);
	/* Finder pattern */
	mqrspec_putFinderPattern(frame, width, 0, 0);
	/* Separator */
	p = frame;
	for(y=0; y<7; y++)
	{
		p[7] = 0xc0;
		p += width;
	}
	memset(frame + width * 7, 0xc0, 8);
	/* Mask format information area */
	memset(frame + width * 8 + 1, 0x84, 8);
	p = frame + width + 8;
	for(y=0; y<7; y++)
	{
		*p = 0x84;
		p += width;
	}
	/* Timing pattern */
	p = frame + 8;
	q = frame + width * 8;
	for(x=1; x<width-7; x++)
	{
		*p =  0x90 | (x & 1);
		*q =  0x90 | (x & 1);
		p++;
		q += width;
	}

	return frame;
}
unsigned char *MQRspec_newFrame(int version)
{
	unsigned char *frame;
	int width;

	if(version < 1 || version > MQRSPEC_VERSION_MAX) return NULL;

#ifdef HAVE_LIBPTHREAD
	pthread_mutex_lock(&frames_mutex);
#endif
	if(mqr_frames[version] == NULL)
	{
		mqr_frames[version] = MQRspec_createFrame(version);
	}
#ifdef HAVE_LIBPTHREAD
	pthread_mutex_unlock(&frames_mutex);
#endif
	if(mqr_frames[version] == NULL) 
	{
		return NULL;
	}
	width = mqrspecCapacity[version].width;
	frame = (unsigned char *)qr_srv_malloc(width * width);
	if(frame == NULL)
	{
		return NULL;
	}
	memcpy(frame, mqr_frames[version], width * width);

	return frame;
}

static FrameFiller *qrencode_FrameFiller_new(int width, unsigned char *frame, int mqr)
{
	FrameFiller *filler;

	filler = (FrameFiller *)qr_srv_malloc(sizeof(FrameFiller));
	if(filler == NULL) return NULL;
	filler->width = width;
	filler->frame = frame;
	filler->x = width - 1;
	filler->y = width - 1;
	filler->dir = -1;
	filler->bit = -1;
	filler->mqr = mqr;

	return filler;
}

/**
 * Return a code (byte).
 * This function can be called iteratively.
 * @param raw raw code.
 * @return code
 */
 /*__STATIC*/
static unsigned char MQRraw_getCode(MQRRawCode *raw)
{
	unsigned char ret;

	if(raw->count < raw->dataLength) 
	{
		ret = raw->datacode[raw->count];
	}
	else if(raw->count < raw->dataLength + raw->eccLength) 
	{
		ret = raw->ecccode[raw->count - raw->dataLength];
	} 
	else 
	{
		return 0;
	}
	raw->count++;
	return ret;
}


static unsigned char *qrencode_FrameFiller_next(FrameFiller *filler)
{
	unsigned char *p;
	int x, y, w;

	if(filler->bit == -1)
	{
		filler->bit = 0;
		return filler->frame + filler->y * filler->width + filler->x;
	}

	x = filler->x;
	y = filler->y;
	p = filler->frame;
	w = filler->width;

	if(filler->bit == 0) 
	{
		x--;
		filler->bit++;
	} 
	else 
	{
		x++;
		y += filler->dir;
		filler->bit--;
	}

	if(filler->dir < 0)
	{
		if(y < 0) 
		{
			y = 0;
			x -= 2;
			filler->dir = 1;
			if(!filler->mqr && x == 6)
			{
				x--;
				y = 9;
			}
		}
	} 
	else
	{
		if(y == w)
		{
			y = w - 1;
			x -= 2;
			filler->dir = -1;
			if(!filler->mqr && x == 6)
			{
				x--;
				y -= 8;
			}
		}
	}
	if(x < 0 || y < 0) return NULL;

	filler->x = x;
	filler->y = y;

	if(p[y * w + x] & 0x80)
	{
		// This tail recursion could be optimized.
		return qrencode_FrameFiller_next(filler);
	}
	return &p[y * w + x];
}

#define mask_MASKMAKER(__exp__) \
	int x, y;\
	int b = 0;\
\
	for(y=0; y<width; y++) {\
		for(x=0; x<width; x++) {\
			if(*s & 0x80) {\
				*d = *s;\
			} else {\
				*d = *s ^ ((__exp__) == 0);\
			}\
			b += (int)(*d & 1);\
			s++; d++;\
		}\
	}\
	return b;

static int Mask_mask0(int width, const unsigned char *s, unsigned char *d)
{
	mask_MASKMAKER((x+y)&1)
}

static int Mask_mask1(int width, const unsigned char *s, unsigned char *d)
{
	mask_MASKMAKER(y&1)
}

static int Mask_mask2(int width, const unsigned char *s, unsigned char *d)
{
	mask_MASKMAKER(x%3)
}

static int Mask_mask3(int width, const unsigned char *s, unsigned char *d)
{
	mask_MASKMAKER((x+y)%3)
}

static int Mask_mask4(int width, const unsigned char *s, unsigned char *d)
{
	mask_MASKMAKER(((y/2)+(x/3))&1)
}

static int Mask_mask5(int width, const unsigned char *s, unsigned char *d)
{
	mask_MASKMAKER(((x*y)&1)+(x*y)%3)
}

static int Mask_mask6(int width, const unsigned char *s, unsigned char *d)
{
	mask_MASKMAKER((((x*y)&1)+(x*y)%3)&1)
}

static int Mask_mask7(int width, const unsigned char *s, unsigned char *d)
{
	mask_MASKMAKER((((x*y)%3)+((x+y)&1))&1)
}

static mask_MASKMAKER *mask_maskMakers[mask_maskNum] = {
	Mask_mask0, Mask_mask1, Mask_mask2, Mask_mask3,
	Mask_mask4, Mask_mask5, Mask_mask6, Mask_mask7
};

unsigned int QRspec_getFormatInfo(int mask, QRecLevel level)
{
	if(mask < 0 || mask > 7) return 0;

	return qrspec_formatInfo[level][mask];
}

/*__STATIC */
static int Mask_writeFormatInformation(int width, unsigned char *frame, int mask, QRecLevel level)
{
	unsigned int format;
	unsigned char v;
	int i;
	int blacks = 0;

	format = QRspec_getFormatInfo(mask, level);

	for(i=0; i<8; i++) 
	{
		if(format & 1)
		{
			blacks += 2;
			v = 0x85;
		} 
		else
		{
			v = 0x84;
		}
		frame[width * 8 + width - 1 - i] = v;
		if(i < 6)
		{
			frame[width * i + 8] = v;
		} 
		else 
		{
			frame[width * (i + 1) + 8] = v;
		}
		format= format >> 1;
	}
	for(i=0; i<7; i++) 
	{
		if(format & 1)
		{
			blacks += 2;
			v = 0x85;
		}
		else
		{
			v = 0x84;
		}
		frame[width * (width - 7 + i) + 8] = v;
		if(i == 0) 
		{
			frame[width * 8 + 7] = v;
		}
		else
		{
			frame[width * 8 + 6 - i] = v;
		}
		format= format >> 1;
	}

	return blacks;
}
/*__STATIC */
static int Mask_calcN2(int width, unsigned char *frame)
{
	int x, y;
	unsigned char *p;
	unsigned char b22, w22;
	int demerit = 0;

	p = frame + width + 1;
	for(y=1; y<width; y++)
	{
		for(x=1; x<width; x++)
		{
			b22 = p[0] & p[-1] & p[-width] & p [-width-1];
			w22 = p[0] | p[-1] | p[-width] | p [-width-1];
			if((b22 | (w22 ^ 1))&1) 
			{
				demerit += N2;
			}
			p++;
		}
		p++;
	}

	return demerit;
}
/*__STATIC */
static int Mask_calcRunLength(int width, unsigned char *frame, int dir, int *runLength)
{
	int head;
	int i;
	unsigned char *p;
	int pitch;

	pitch = (dir==0)?1:width;
	if(frame[0] & 1)
	{
		runLength[0] = -1;
		head = 1;
	} 
	else 
	{
		head = 0;
	}
	runLength[head] = 1;
	p = frame + pitch;

	for(i=1; i<width; i++) 
	{
		if((p[0] ^ p[-pitch]) & 1) 
		{
			head++;
			runLength[head] = 1;
		}
		else 
		{
			runLength[head]++;
		}
		p += pitch;
	}

	return head + 1;
}
/*__STATIC */
static int Mask_calcN1N3(int length, int *runLength)
{
	int i;
	int demerit = 0;
	int fact;

	for(i=0; i<length; i++)
	{
		if(runLength[i] >= 5)
		{
			demerit += N1 + (runLength[i] - 5);
			//n1 += N1 + (runLength[i] - 5);
		}
		if((i & 1)) 
		{
			if(i >= 3 && i < length-2 && (runLength[i] % 3) == 0) 
			{
				fact = runLength[i] / 3;
				if(runLength[i-2] == fact &&
				   runLength[i-1] == fact &&
				   runLength[i+1] == fact &&
				   runLength[i+2] == fact)
				{
					if(i == 3 || runLength[i-3] >= 4 * fact)
					{
						demerit += N3;
						//n3 += N3;
					} 
					else if(i+4 >= length || runLength[i+3] >= 4 * fact)
					{
						demerit += N3;
						//n3 += N3;
					}
				}
			}
		}
	}

	return demerit;
}


/*__STATIC */
static int Mask_evaluateSymbol(int width, unsigned char *frame)
{
	int x, y;
	int demerit = 0;
	int runLength[QRSPEC_WIDTH_MAX + 1];
	int length;

	demerit += Mask_calcN2(width, frame);

	for(y=0; y<width; y++)
	{
		length = Mask_calcRunLength(width, frame + y * width, 0, runLength);
		demerit += Mask_calcN1N3(length, runLength);
	}

	for(x=0; x<width; x++) 
	{
		length = Mask_calcRunLength(width, frame + x, 1, runLength);
		demerit += Mask_calcN1N3(length, runLength);
	}

	return demerit;
}


unsigned char *Mask_mask(int width, unsigned char *frame, QRecLevel level)
{
	int i;
	unsigned char *mask, *bestMask;
	int minDemerit = INT_MAX;//不确定值， 后期修改
	int blacks;
	int bratio;
	int demerit;
	int w2 = width * width;

	mask = (unsigned char *)qr_srv_malloc(w2);
	if(mask == NULL) return NULL;
	bestMask = NULL;

	for(i=0; i<mask_maskNum; i++) 
	{
//		n1 = n2 = n3 = n4 = 0;
		demerit = 0;
		blacks = mask_maskMakers[i](width, frame, mask);
		blacks += Mask_writeFormatInformation(width, mask, i, level);
		bratio = (200 * blacks + w2) / w2 / 2; /* (int)(100*blacks/w2+0.5) */
		demerit = (abs(bratio - 50) / 5) * N4;
//		n4 = demerit;
		demerit += Mask_evaluateSymbol(width, mask);
//		printf("(%d,%d,%d,%d)=%d\n", n1, n2, n3 ,n4, demerit);
		if(demerit < minDemerit)
		{
			minDemerit = demerit;
			qr_srv_free(bestMask);
			bestMask = mask;
			mask = (unsigned char *)qr_srv_malloc(w2);
			if(mask == NULL) break;
		}
	}
	qr_srv_free(mask);
	return bestMask;
}

#define mmask_MASKMAKER(__exp__) \
	int x, y;\
\
	for(y=0; y<width; y++) {\
		for(x=0; x<width; x++) {\
			if(*s & 0x80) {\
				*d = *s;\
			} else {\
				*d = *s ^ ((__exp__) == 0);\
			}\
			s++; d++;\
		}\
	}

static void MMask_mask0(int width, const unsigned char *s, unsigned char *d)
{
	mmask_MASKMAKER(y&1)
}

static void MMask_mask1(int width, const unsigned char *s, unsigned char *d)
{
	mmask_MASKMAKER(((y/2)+(x/3))&1)
}

static void MMask_mask2(int width, const unsigned char *s, unsigned char *d)
{
	mmask_MASKMAKER((((x*y)&1)+(x*y)%3)&1)
}

static void MMask_mask3(int width, const unsigned char *s, unsigned char *d)
{
	mmask_MASKMAKER((((x+y)&1)+((x*y)%3))&1)
}

static mmask_MASKMAKER *mmask_maskMakers[mmask_maskNum] = {
	MMask_mask0, MMask_mask1, MMask_mask2, MMask_mask3
};


unsigned int MQRspec_getFormatInfo(int mask, int version, QRecLevel level)
{
	int type;

	if(mask < 0 || mask > 3) return 0;
	if(version <= 0 || version > MQRSPEC_VERSION_MAX) return 0;
	if(level == QR_ECLEVEL_H) return 0;
	type = mqrspec_typeTable[version][level];
	if(type < 0) return 0;

	return formatInfo[mask][type];
}

/*__STATIC*/
static void MMask_writeFormatInformation(int version, int width, unsigned char *frame, int mask, QRecLevel level)
{
	unsigned int format;
	unsigned char v;
	int i;

	format = MQRspec_getFormatInfo(mask, version, level);

	for(i=0; i<8; i++)
	{
		v = 0x84 | (format & 1);
		frame[width * (i + 1) + 8] = v;
		format = format >> 1;
	}
	for(i=0; i<7; i++) 
	{
		v = 0x84 | (format & 1);
		frame[width * 8 + 7 - i] = v;
		format = format >> 1;
	}
}

/*__STATIC*/
static int MMask_evaluateSymbol(int width, unsigned char *frame)
{
	int x, y;
	unsigned char *p;
	int sum1 = 0, sum2 = 0;

	p = frame + width * (width - 1);
	for(x=1; x<width; x++) 
	{
		sum1 += (p[x] & 1);
	}

	p = frame + width * 2 - 1;
	for(y=1; y<width; y++) 
	{
		sum2 += (*p & 1);
		p += width;
	}

	return (sum1 <= sum2)?(sum1 * 16 + sum2):(sum2 * 16 + sum1);
}


unsigned char *MMask_mask(int version, unsigned char *frame, QRecLevel level)
{
	int i;
	unsigned char *mask, *bestMask;
	int maxScore = 0;
	int score;
	int width;

	width = MQRspec_getWidth(version);

	mask = (unsigned char *)qr_srv_malloc(width * width);
	if(mask == NULL) return NULL;
	bestMask = NULL;

	for(i=0; i<mmask_maskNum; i++)
	{
		score = 0;
		mmask_maskMakers[i](width, frame, mask);
		MMask_writeFormatInformation(version, width, mask, i, level);
		score = MMask_evaluateSymbol(width, mask);
		if(score > maxScore)
		{
			maxScore = score;
			qr_srv_free(bestMask);
			bestMask = mask;
			mask = (unsigned char *)qr_srv_malloc(width * width);
			if(mask == NULL) break;
		}
	}
	qr_srv_free(mask);
	return bestMask;
}


unsigned char *MMask_makeMask(int version, unsigned char *frame, int mask, QRecLevel level)
{
	unsigned char *masked;
	int width;

	if(mask < 0 || mask >= mmask_maskNum)
	{
//		errno = EINVAL;
		return NULL;
	}

	width = MQRspec_getWidth(version);
	masked = (unsigned char *)qr_srv_malloc(width * width);
	if(masked == NULL) return NULL;

	mmask_maskMakers[mask](width, frame, masked);
	MMask_writeFormatInformation(version, width, masked, mask, level);

	return masked;
}



/******************************************************************************
 * QR-code encoding
 *****************************************************************************/
/*__STATIC*/
static QRcode *QRcode_new(int version, int width, unsigned char *data)
{
	QRcode *qrcode;

	qrcode = (QRcode *)qr_srv_malloc(sizeof(QRcode));
	if(qrcode == NULL) return NULL;

	qrcode->version = version;
	qrcode->width = width;
	qrcode->data = data;

	return qrcode;
}

int QRspec_getECCLength(int version, QRecLevel level)
{
	return qrspecCapacity[version].ec[level];
}
void QRspec_getEccSpec(int version, QRecLevel level, int spec[5])
{
	int b1, b2;
	int data, ecc;

	b1 = eccTable[version][level][0];
	b2 = eccTable[version][level][1];
	data = QRspec_getDataLength(version, level);
	ecc  = QRspec_getECCLength(version, level);

	if(b2 == 0)
	{
		spec[0] = b1;
		spec[1] = data / b1;
		spec[2] = ecc / b1;
		spec[3] = spec[4] = 0;
	} 
	else 
	{
		spec[0] = b1;
		spec[1] = data / (b1 + b2);
		spec[2] = ecc  / (b1 + b2);
		spec[3] = b2;
		spec[4] = spec[1] + 1;
	}
}

/*__STATIC*/
static void QRraw_free(QRRawCode *raw)
{
	if(raw != NULL) 
	{
		qr_srv_free(raw->datacode);
		qr_srv_free(raw->ecccode);
		qr_srv_free(raw->rsblock);
		qr_srv_free(raw);
	}
}

static int RSblock_init(RSblock *blocks, int spec[5], unsigned char *data, unsigned char *ecc)
{
	int i;
	RSblock *block;
	unsigned char *dp, *ep;
	RS *rs;
	int el, dl;

	dl = QRspec_rsDataCodes1(spec);
	el = QRspec_rsEccCodes1(spec);
	rs = init_rs(8, 0x11d, 0, 1, el, 255 - dl - el);
	if(rs == NULL) return -1;

	block = blocks;
	dp = data;
	ep = ecc;
	for(i=0; i<QRspec_rsBlockNum1(spec); i++)
	{
		RSblock_initBlock(block, dl, dp, el, ep, rs);
		dp += dl;
		ep += el;
		block++;
	}

	if(QRspec_rsBlockNum2(spec) == 0) return 0;

	dl = QRspec_rsDataCodes2(spec);
	el = QRspec_rsEccCodes2(spec);
	rs = init_rs(8, 0x11d, 0, 1, el, 255 - dl - el);
	if(rs == NULL) return -1;
	for(i=0; i<QRspec_rsBlockNum2(spec); i++) 
	{
		RSblock_initBlock(block, dl, dp, el, ep, rs);
		dp += dl;
		ep += el;
		block++;
	}

	return 0;
}


/*__STATIC*/
static QRRawCode *QRraw_new(QRinput *input)
{
	QRRawCode *raw;
	int spec[5], ret;

	raw = (QRRawCode *)qr_srv_malloc(sizeof(QRRawCode));
	if(raw == NULL) return NULL;

	raw->datacode = QRinput_getByteStream(input);
	if(raw->datacode == NULL)
	{
		qr_srv_free(raw);
		return NULL;
	}

	QRspec_getEccSpec(input->version, input->level, spec);

	raw->version = input->version;
	raw->b1 = QRspec_rsBlockNum1(spec);
	raw->dataLength = QRspec_rsDataLength(spec);
	raw->eccLength = QRspec_rsEccLength(spec);
	raw->ecccode = (unsigned char *)qr_srv_malloc(raw->eccLength);
	if(raw->ecccode == NULL)
	{
		qr_srv_free(raw->datacode);
		qr_srv_free(raw);
		return NULL;
	}

	raw->blocks = QRspec_rsBlockNum(spec);
	raw->rsblock = (RSblock *)qr_srv_calloc(sizeof(RSblock), raw->blocks);
	if(raw->rsblock == NULL) 
	{
		QRraw_free(raw);
		return NULL;
	}
	ret = RSblock_init(raw->rsblock, spec, raw->datacode, raw->ecccode);
	if(ret < 0)
	{
		QRraw_free(raw);
		return NULL;
	}

	raw->count = 0;

	return raw;
}

int QRspec_getWidth(int version)
{
	return qrspecCapacity[version].width;
}



/**
 * Put a finder pattern.
 * @param frame
 * @param width
 * @param ox,oy upper-left coordinate of the pattern
 */
static void putFinderPattern(unsigned char *frame, int width, int ox, int oy)
{
	static const unsigned char finder[] = {
		0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
		0xc1, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1,
		0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
	};
	int x, y;
	const unsigned char *s;

	frame += oy * width + ox;
	s = finder;
	for(y=0; y<7; y++)
	{
		for(x=0; x<7; x++)
		{
			frame[x] = s[x];
		}
		frame += width;
		s += 7;
	}
}


/**
 * Put an alignment marker.
 * @param frame
 * @param width
 * @param ox,oy center coordinate of the pattern
 */
static void QRspec_putAlignmentMarker(unsigned char *frame, int width, int ox, int oy)
{
	static const unsigned char finder[] = {
		0xa1, 0xa1, 0xa1, 0xa1, 0xa1,
		0xa1, 0xa0, 0xa0, 0xa0, 0xa1,
		0xa1, 0xa0, 0xa1, 0xa0, 0xa1,
		0xa1, 0xa0, 0xa0, 0xa0, 0xa1,
		0xa1, 0xa1, 0xa1, 0xa1, 0xa1,
	};
	int x, y;
	const unsigned char *s;

	frame += (oy - 2) * width + ox - 2;
	s = finder;
	for(y=0; y<5; y++) 
	{
		for(x=0; x<5; x++)
		{
			frame[x] = s[x];
		}
		frame += width;
		s += 5;
	}
}

static void QRspec_putAlignmentPattern(int version, unsigned char *frame, int width)
{
	int d, w, x, y, cx, cy;

	if(version < 2) return;

	d = alignmentPattern[version][1] - alignmentPattern[version][0];
	if(d < 0)
	{
		w = 2;
	}
	else 
	{
		w = (width - alignmentPattern[version][0]) / d + 2;
	}

	if(w * w - 3 == 1) 
	{
		x = alignmentPattern[version][0];
		y = alignmentPattern[version][0];
		QRspec_putAlignmentMarker(frame, width, x, y);
		return;
	}

	cx = alignmentPattern[version][0];
	for(x=1; x<w - 1; x++) 
	{
		QRspec_putAlignmentMarker(frame, width,  6, cx);
		QRspec_putAlignmentMarker(frame, width, cx,  6);
		cx += d;
	}

	cy = alignmentPattern[version][0];
	for(y=0; y<w-1; y++)
	{
		cx = alignmentPattern[version][0];
		for(x=0; x<w-1; x++)
		{
			QRspec_putAlignmentMarker(frame, width, cx, cy);
			cx += d;
		}
		cy += d;
	}
}

unsigned int QRspec_getVersionPattern(int version)
{
	if(version < 7 || version > QRSPEC_VERSION_MAX) return 0;

	return versionPattern[version - 7];
}

static unsigned char *QRspec_createFrame(int version)
{
	unsigned char *frame, *p, *q;
	int width;
	int x, y;
	unsigned int verinfo, v;

	width = qrspecCapacity[version].width;
	frame = (unsigned char *)qr_srv_malloc(width * width);
	if(frame == NULL) return NULL;

	memset(frame, 0, width * width);
	/* Finder pattern */
	putFinderPattern(frame, width, 0, 0);
	putFinderPattern(frame, width, width - 7, 0);
	putFinderPattern(frame, width, 0, width - 7);
	/* Separator */
	p = frame;
	q = frame + width * (width - 7);
	for(y=0; y<7; y++)
	{
		p[7] = 0xc0;
		p[width - 8] = 0xc0;
		q[7] = 0xc0;
		p += width;
		q += width;
	}
	memset(frame + width * 7, 0xc0, 8);
	memset(frame + width * 8 - 8, 0xc0, 8);
	memset(frame + width * (width - 8), 0xc0, 8);
	/* Mask format information area */
	memset(frame + width * 8, 0x84, 9);
	memset(frame + width * 9 - 8, 0x84, 8);
	p = frame + 8;
	for(y=0; y<8; y++)
	{
		*p = 0x84;
		p += width;
	}
	p = frame + width * (width - 7) + 8;
	for(y=0; y<7; y++) 
	{
		*p = 0x84;
		p += width;
	}
	/* Timing pattern */
	p = frame + width * 6 + 8;
	q = frame + width * 8 + 6;
	for(x=1; x<width-15; x++)
	{
		*p =  0x90 | (x & 1);
		*q =  0x90 | (x & 1);
		p++;
		q += width;
	}
	/* Alignment pattern */
	QRspec_putAlignmentPattern(version, frame, width);

	/* Version information */
	if(version >= 7)
	{
		verinfo = QRspec_getVersionPattern(version);

		p = frame + width * (width - 11);
		v = verinfo;
		for(x=0; x<6; x++)
		{
			for(y=0; y<3; y++)
			{
				p[width * y + x] = 0x88 | (v & 1);
				v = v >> 1;
			}
		}

		p = frame + width - 11;
		v = verinfo;
		for(y=0; y<6; y++)
		{
			for(x=0; x<3; x++)
			{
				p[x] = 0x88 | (v & 1);
				v = v >> 1;
			}
			p += width;
		}
	}
	/* and a little bit... */
	frame[width * (width - 8) + 8] = 0x81;

	return frame;
}


unsigned char *QRspec_newFrame(int version)
{
	unsigned char *frame;
	int width;

	if(version < 1 || version > QRSPEC_VERSION_MAX) return NULL;

#ifdef HAVE_LIBPTHREAD
	pthread_mutex_lock(&frames_mutex);
#endif
	if(frames[version] == NULL) {
		frames[version] = QRspec_createFrame(version);
	}
#ifdef HAVE_LIBPTHREAD
	pthread_mutex_unlock(&frames_mutex);
#endif
	if(frames[version] == NULL) return NULL;

	width = qrspecCapacity[version].width;
	frame = (unsigned char *)qr_srv_malloc(width * width);
	if(frame == NULL) return NULL;
	memcpy(frame, frames[version], width * width);

	return frame;
}


/**
 * Return a code (byte).
 * This function can be called iteratively.
 * @param raw raw code.
 * @return code
 */
 /*__STATIC*/
static unsigned char QRraw_getCode(QRRawCode *raw)
{
	int col, row;
	unsigned char ret;

	if(raw->count < raw->dataLength)
	{
		row = raw->count % raw->blocks;
		col = raw->count / raw->blocks;
		if(col >= raw->rsblock[0].dataLength)
		{
			row += raw->b1;
		}
		ret = raw->rsblock[row].data[col];
	} 
	else if(raw->count < raw->dataLength + raw->eccLength)
	{
		row = (raw->count - raw->dataLength) % raw->blocks;
		col = (raw->count - raw->dataLength) / raw->blocks;
		ret = raw->rsblock[row].ecc[col];
	} 
	else
	{
		return 0;
	}
	raw->count++;
	return ret;
}

int QRspec_getRemainder(int version)
{
	return qrspecCapacity[version].remainder;
}

unsigned char *Mask_makeMask(int width, unsigned char *frame, int mask, QRecLevel level)
{
	unsigned char *masked;

	if(mask < 0 || mask >= mask_maskNum)
	{
//		errno = EINVAL;
		return NULL;
	}

	masked = (unsigned char *)qr_srv_malloc(width * width);
	if(masked == NULL) return NULL;

	mask_maskMakers[mask](width, frame, masked);
	Mask_writeFormatInformation(width, masked, mask, level);

	return masked;
}

/*__STATIC*/
static QRcode *QRcode_encodeMask(QRinput *input, int mask)
{
	int width, version;
	QRRawCode *raw;
	unsigned char *frame, *masked, *p, code, bit;
	FrameFiller *filler;
	int i, j;
	QRcode *qrcode = NULL;

	if(input->mqr)
	{
//		errno = EINVAL;
		return NULL;
	}
	if(input->version < 0 || input->version > QRSPEC_VERSION_MAX)
	{
//		errno = EINVAL;
		return NULL;
	}
	if(input->level > QR_ECLEVEL_H) 
	{
//		errno = EINVAL;
		return NULL;
	}

	raw = QRraw_new(input);
	if(raw == NULL) return NULL;

	version = raw->version;
	width = QRspec_getWidth(version);
	frame = QRspec_newFrame(version);
	if(frame == NULL)
	{
		QRraw_free(raw);
		return NULL;
	}
	filler = qrencode_FrameFiller_new(width, frame, 0);
	if(filler == NULL)
	{
		QRraw_free(raw);
		qr_srv_free(frame);
		return NULL;
	}

	/* inteleaved data and ecc codes */
	for(i=0; i<raw->dataLength + raw->eccLength; i++)
	{
		code = QRraw_getCode(raw);
		bit = 0x80;
		for(j=0; j<8; j++) 
		{
			p = qrencode_FrameFiller_next(filler);
			if(p == NULL)  goto EXIT;
			*p = 0x02 | ((bit & code) != 0);
			bit = bit >> 1;
		}
	}
	QRraw_free(raw);
	raw = NULL;
	/* remainder bits */
	j = QRspec_getRemainder(version);
	for(i=0; i<j; i++) 
	{
		p = qrencode_FrameFiller_next(filler);
		if(p == NULL)  goto EXIT;
		*p = 0x02;
	}

	/* masking */
	if(mask == -2)	// just for debug purpose
	{ 							
		masked = (unsigned char *)qr_srv_malloc(width * width);
		memcpy(masked, frame, width * width);
	} 
	else if(mask < 0) 
	{
		masked = Mask_mask(width, frame, input->level);
	} 
	else
	{
		masked = Mask_makeMask(width, frame, mask, input->level);
	}
	if(masked == NULL)
	{
		goto EXIT;
	}
	qrcode = QRcode_new(version, width, masked);

EXIT:
	QRraw_free(raw);
	qr_srv_free(filler);
	qr_srv_free(frame);
	return qrcode;
}


/*__STATIC*/
static QRcode *QRcode_encodeMaskMQR(QRinput *input, int mask)
{
	int width, version;
	MQRRawCode *raw;
	unsigned char *frame, *masked, *p, code, bit;
	FrameFiller *filler;
	int i, j;
	QRcode *qrcode = NULL;

	if(!input->mqr)
	{
		//errno = EINVAL;
		return NULL;
	}
	if(input->version <= 0 || input->version > MQRSPEC_VERSION_MAX) 
	{
		//errno = EINVAL;
		return NULL;
	}
	if(input->level > QR_ECLEVEL_Q) 
	{
		//errno = EINVAL;
		return NULL;
	}

	raw = MQRraw_new(input);
	if(raw == NULL)
	{
		return NULL;
	}

	version = raw->version;
	width = MQRspec_getWidth(version);
	frame = MQRspec_newFrame(version);
	if(frame == NULL) 
	{
		MQRraw_free(raw);
		return NULL;
	}
	filler = qrencode_FrameFiller_new(width, frame, 1);
	if(filler == NULL)
	{
		MQRraw_free(raw);
		qr_srv_free(frame);
		return NULL;
	}

	/* inteleaved data and ecc codes */
	for(i=0; i<raw->dataLength + raw->eccLength; i++)
	{
		code = MQRraw_getCode(raw);
		if(raw->oddbits && i == raw->dataLength - 1) 
		{
			bit = 1 << raw->oddbits;
			for(j=0; j<raw->oddbits; j++) 
			{
				p = qrencode_FrameFiller_next(filler);
				if(p == NULL) goto EXIT;
				*p = 0x02 | ((bit & code) != 0);
				bit = bit >> 1;
			}
		}
		else 
		{
			bit = 0x80;
			for(j=0; j<8; j++)
			{
				p = qrencode_FrameFiller_next(filler);
				if(p == NULL) goto EXIT;
				*p = 0x02 | ((bit & code) != 0);
				bit = bit >> 1;
			}
		}
	}
	MQRraw_free(raw);
	raw = NULL;

	/* masking */
	if(mask < 0) 
	{
		masked = MMask_mask(version, frame, input->level);
	} 
	else
	{
		masked = MMask_makeMask(version, frame, mask, input->level);
	}
	if(masked == NULL)
	{
		goto EXIT;
	}

	qrcode = QRcode_new(version, width, masked);

EXIT:
	MQRraw_free(raw);
	qr_srv_free(filler);
	qr_srv_free(frame);
	return qrcode;
}

void QRcode_free(QRcode *qrcode)
{
	if(qrcode != NULL) 
	{
		qr_srv_free(qrcode->data);
		qr_srv_free(qrcode);
	}
}
QRcode *QRcode_encodeInput(QRinput *input)
{
	if(input->mqr) 
	{
		return QRcode_encodeMaskMQR(input, -1);
	} 
	else 
	{
		return QRcode_encodeMask(input, -1);
	}
}

static void QRcode_List_freeEntry(QRcode_List *entry)
{
	if(entry != NULL) 
	{
		QRcode_free(entry->code);
		qr_srv_free(entry);
	}
}
void QRcode_List_free(QRcode_List *qrlist)
{
	QRcode_List *list = qrlist, *next;

	while(list != NULL)
	{
		next = list->next;
		QRcode_List_freeEntry(list);
		list = next;
	}
}

QRcode_List *QRcode_encodeInputStructured(QRinput_Struct *s)
{
	QRcode_List *head = NULL;
	QRcode_List *tail = NULL;
	QRcode_List *entry;
	QRinput_InputList *list = s->head;

	while(list != NULL) 
	{
		if(head == NULL) 
		{
			entry = QRcode_List_newEntry();
			if(entry == NULL)
			{
				goto ABORT;
			}
			head = entry;
			tail = head;
		} 
		else 
		{
			entry = QRcode_List_newEntry();
			if(entry == NULL)
			{
				goto ABORT;
			}
			tail->next = entry;
			tail = tail->next;
		}
		tail->code = QRcode_encodeInput(list->input);
		if(tail->code == NULL) {
			goto ABORT;
		}
		list = list->next;
	}

	return head;
ABORT:
	QRcode_List_free(head);
	return NULL;
}

int QRcode_List_size(QRcode_List *qrlist)
{
	QRcode_List *list = qrlist;
	int size = 0;

	while(list != NULL)
	{
		size++;
		list = list->next;
	}

	return size;
}

#define GDI_COLOR_WHITE MMI_WHITE_COLOR//MMI_BLACK_COLOR
#define GDI_COLOR_BLACK MMI_BLACK_COLOR//MMI_WHITE_COLOR


void gdi_draw_solid_rect(int x1, int y1, int x2, int y2,  GUI_COLOR_T color)
{
    const GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0};
    rect.left = x1;
    rect.top = y1;
    rect.right = x2;
    rect.bottom = y2;

    GUI_FillRect( &lcd_dev_info, rect, color);
}


static void draw_QRcode(QRcode *qrcode, int ox, int oy) 	//在(x,y)处画单位0或1
{
	int x, y, width;
	unsigned char *p;
//	SDL_Rect rect;
	int x1, y1, x2, y2;
	ox += (QR_Weigth- Version_NUM*size )/2;
	oy += (QR_Height- Version_NUM*size  )/2;	
	width = qrcode->width;
	p = qrcode->data;	
	for(y=0; y<width; y++) 
	{
		for(x=0; x<width; x++) 
		{
//			rect.x = ox + x * size;
//			rect.y = oy + y * size;
//			rect.w = size;
//			rect.h = size;
//			SDL_FillRect(screen, &rect, (*p&1)?0:0xffffff);
			x1 = ox + x * size;
			y1 = oy + y * size;
			x2 = x1 + size - 1;
			y2 = y1 + size - 1;
			if(*p&1)
			{
				gdi_draw_solid_rect(x1, y1,x2, y2,  GDI_COLOR_BLACK);
			}
			else
			{
				gdi_draw_solid_rect(x1, y1,x2, y2,  GDI_COLOR_WHITE);
			}
			p++;
		}
	}
}

void draw_structuredQRcode(QRinput_Struct *s)	//通过size,parity,head,tail 画二维码结构  
{
	int i, w, h, n, x, y;
	int swidth;
	QRcode_List *qrcodes, *p;

	qrcodes = QRcode_encodeInputStructured(s);//返回 head地址
	if(qrcodes == NULL) return;

	swidth = (qrcodes->code->width + margin * 2) * size;
	n = QRcode_List_size(qrcodes);  		 //返回size
	w = (n < 4)?n:4;
	h = (n - 1) / 4 + 1;

//	screen = SDL_SetVideoMode(swidth * w, swidth * h, 32, 0);
//	SDL_FillRect(screen, NULL, 0xffffff);

	p = qrcodes;
	for(i=0; i<n; i++)
	{
		x = (i % 4) * swidth;
		y = (i / 4) * swidth;
		draw_QRcode(p->code, x, y);  //在(x,y)处根据code画二维码
		p = p->next;
	}
//	SDL_Flip(screen);
	QRcode_List_free(qrcodes);
}

void draw_structuredQRcodeFromText(int argc, char **argv) 
{									//*argv[]:字符串数组,用来存放指向你的字符串参数的指针数组
	QRinput_Struct *s;
	QRinput *input;
	int i, ret;

	s = QRinput_Struct_new();	//动态分配内存的首地址，QRinput_Struct * s;
	if(s == NULL) 
	{
		//fprintf(stderr, "Failed to allocate memory.\n");
		//exit(EXIT_FAILURE);
   		//kal_prompt_trace(MOD_MMI,"[QR]Failed to allocate memory ");//HERO_DEBUG
   		return;
	}
	for(i=0; i<argc; i++) 
	{		//argc:整数类型统计参数个数
		input = QRinput_new2(version, level);	//输入版本和纠错级别 ； 动态分配一个内存的首地址,QRinput *input；来保存
		if(input == NULL) 
		{
			//fprintf(stderr, "Failed to allocate memory.\n");
			//exit(EXIT_FAILURE);
	   		//kal_prompt_trace(MOD_MMI,"[QR]Failed to allocate memory ");//HERO_DEBUG
			QRinput_Struct_free(s);
	   		return;
		}
		if(eightbit) 
		{	 						//ret<0;说明无数据，为NULL;
			ret = QRinput_append(input, QR_MODE_8, strlen(argv[i]), (unsigned char *)argv[i]);
		} 
		else 
		{
			ret = Split_splitStringToQRinput(argv[i], input, hint, casesensitive);//一个字符串分割成字符串数组
		}
		if(ret < 0) 
		{
			//perror("Encoding the input string");
			//exit(EXIT_FAILURE);
	   		//kal_prompt_trace(MOD_MMI,"[QR]Encoding the input string");//HERO_DEBUG
	   		QRinput_free(input);
			QRinput_Struct_free(s);
	   		return;
		}
		ret = QRinput_Struct_appendInput(s, input);
		if(ret < 0) 
		{
			//perror("Encoding the input string");
			//exit(EXIT_FAILURE);
	   		//kal_prompt_trace(MOD_MMI,"[QR]Encoding the input string");//HERO_DEBUG
			QRinput_Struct_free(s);
	   		return;
		}
	}
	ret = QRinput_Struct_insertStructuredAppendHeaders(s);
	if(ret < 0) 
	{
		//fprintf(stderr, "Too many inputs.\n");
   		//kal_prompt_trace(MOD_MMI,"[QR]Too many inputs");//HERO_DEBUG
	}

	draw_structuredQRcode(s);
	QRinput_Struct_free(s);
}

int QRinput_setVersion(QRinput *input, int version)
{
	if(input->mqr || version < 0 || version > QRSPEC_VERSION_MAX) {
//		errno = EINVAL;
		return -1;
	}

	input->version = version;

	return 0;
}

int QRinput_setErrorCorrectionLevel(QRinput *input, QRecLevel level)
{
	if(input->mqr || level > QR_ECLEVEL_H)
	{
//		errno = EINVAL;
		return -1;
	}

	input->level = level;

	return 0;
}


/**
 * Returns required length in bytes for specified mode, version and bits.
 * @param mode
 * @param version
 * @param bits
 * @return required length of code words in bytes.
 */
 /*__STATIC*/
static int QRinput_lengthOfCode(QRencodeMode mode, int version, int bits)
{
	int payload, size, chunks, remain, maxsize;

	payload = bits - 4 - QRspec_lengthIndicator(mode, version);
	switch(mode) {
		case QR_MODE_NUM:
			chunks = payload / 10;
			remain = payload - chunks * 10;
			size = chunks * 3;
			if(remain >= 7) {
				size += 2;
			} else if(remain >= 4) {
				size += 1;
			}
			break;
		case QR_MODE_AN:
			chunks = payload / 11;
			remain = payload - chunks * 11;
			size = chunks * 2;
			if(remain >= 6) size++;
			break;
		case QR_MODE_8:
			size = payload / 8;
			break;
		case QR_MODE_KANJI:
			size = (payload / 13) * 2;
			break;
		case QR_MODE_STRUCTURE:
			size = payload / 8;
			break;
		default:
			size = 0;
			break;
	}
	maxsize = QRspec_maximumWords(mode, version);
	if(size < 0) size = 0;
	if(maxsize > 0 && size > maxsize) size = maxsize;

	return size;
}


static QRinput_List *QRinput_List_dup(QRinput_List *entry)
{
	QRinput_List *n;

	n = (QRinput_List *)qr_srv_malloc(sizeof(QRinput_List));
	if(n == NULL) return NULL;

	n->mode = entry->mode;
	n->size = entry->size;
	n->data = (unsigned char *)qr_srv_malloc(n->size);
	if(n->data == NULL) {
		qr_srv_free(n);
		return NULL;
	}
	memcpy(n->data, entry->data, entry->size);
	n->bstream = NULL;
	n->next = NULL;

	return n;
}

QRinput *QRinput_dup(QRinput *input)
{
	QRinput *n;
	QRinput_List *list, *e;

	if(input->mqr) {
		n = QRinput_newMQR(input->version, input->level);
	} else {
		n = QRinput_new2(input->version, input->level);
	}
	if(n == NULL) return NULL;

	list = input->head;
	while(list != NULL) {
		e = QRinput_List_dup(list);
		if(e == NULL) {
			QRinput_free(n);
			return NULL;
		}
		QRinput_appendEntry(n, e);
		list = list->next;
	}

	return n;
}


static int QRinput_List_shrinkEntry(QRinput_List *entry, int bytes)
{
	unsigned char *data;

	data = (unsigned char *)qr_srv_malloc(bytes);
	if(data == NULL) return -1;

	memcpy(data, entry->data, bytes);
	qr_srv_free(entry->data);
	entry->data = data;
	entry->size = bytes;

	return 0;
}


 /*__STATIC*/
static int QRinput_splitEntry(QRinput_List *entry, int bytes)
{
	QRinput_List *e;
	int ret;

	e = QRinput_List_newEntry(entry->mode, entry->size - bytes, entry->data + bytes);
	if(e == NULL) {
		return -1;
	}

	ret = QRinput_List_shrinkEntry(entry, bytes);
	if(ret < 0) {
		QRinput_List_freeEntry(e);
		return -1;
	}

	e->next = entry->next;
	entry->next = e;

	return 0;
}


QRinput_Struct *QRinput_splitQRinputToStruct(QRinput *input)
{
	QRinput *p;
	QRinput_Struct *s;
	int bits, maxbits, nextbits, bytes, ret;
	QRinput_List *list, *next, *prev;

	if(input->mqr) {
//		errno = EINVAL;
		return NULL;
	}

	s = QRinput_Struct_new();
	if(s == NULL) return NULL;

	input = QRinput_dup(input);
	if(input == NULL) {
		QRinput_Struct_free(s);
		return NULL;
	}

	QRinput_Struct_setParity(s, QRinput_calcParity(input));
	maxbits = QRspec_getDataLength(input->version, input->level) * 8 - STRUCTURE_HEADER_SIZE;

	if(maxbits <= 0) {
		QRinput_Struct_free(s);
		QRinput_free(input);
		return NULL;
	}

	bits = 0;
	list = input->head;
	prev = NULL;
	while(list != NULL) {
		nextbits = QRinput_estimateBitStreamSizeOfEntry(list, input->version, input->mqr);
		if(bits + nextbits <= maxbits) {
			ret = QRinput_encodeBitStream(list, input->version, input->mqr);
			if(ret < 0) goto ABORT;
			bits += ret;
			prev = list;
			list = list->next;
		} else {
			bytes = QRinput_lengthOfCode(list->mode, input->version, maxbits - bits);
			if(bytes > 0) {
				/* Splits this entry into 2 entries. */
				ret = QRinput_splitEntry(list, bytes);
				if(ret < 0) goto ABORT;
				/* First half is the tail of the current input. */
				next = list->next;
				list->next = NULL;
				/* Second half is the head of the next input, p.*/
				p = QRinput_new2(input->version, input->level);
				if(p == NULL) goto ABORT;
				p->head = next;
				/* Renew QRinput.tail. */
				p->tail = input->tail;
				input->tail = list;
				/* Point to the next entry. */
				prev = list;
				list = next;
			} else {
				/* Current entry will go to the next input. */
				prev->next = NULL;
				p = QRinput_new2(input->version, input->level);
				if(p == NULL) goto ABORT;
				p->head = list;
				p->tail = input->tail;
				input->tail = prev;
			}
			ret = QRinput_Struct_appendInput(s, input);
			if(ret < 0) goto ABORT;
			input = p;
			bits = 0;
		}
	}
	QRinput_Struct_appendInput(s, input);
	if(s->size > MAX_STRUCTURED_SYMBOLS) {
		QRinput_Struct_free(s);
//		errno = ERANGE;
		return NULL;
	}
	ret = QRinput_Struct_insertStructuredAppendHeaders(s);
	if(ret < 0) {
		QRinput_Struct_free(s);
		return NULL;
	}

	return s;

ABORT:
	QRinput_free(input);
	QRinput_Struct_free(s);
	return NULL;
}

void draw_structuredQRcodeFromQRinput(QRinput *stream)
{
	QRinput_Struct *s;

	QRinput_setVersion(stream, version);
	QRinput_setErrorCorrectionLevel(stream, level);
	s = QRinput_splitQRinputToStruct(stream);
	if(s != NULL)
	{
		draw_structuredQRcode(s);
		QRinput_Struct_free(s);
	} 
	else
	{
		fprintf(stderr, "Input data is too large for this setting.\n");
	}
}


int QRinput_setVersionAndErrorCorrectionLevel(QRinput *input, int version, QRecLevel level)
{
	if(input->mqr)
	{
		if(version <= 0 || version > MQRSPEC_VERSION_MAX) goto INVALID;
		if((MQRspec_getECCLength(version, level) == 0)) goto INVALID;
	} 
	else 
	{
		if(version < 0 || version > QRSPEC_VERSION_MAX) goto INVALID;
		if(level > QR_ECLEVEL_H) goto INVALID;
	}

	input->version = version;
	input->level = level;

	return 0;

INVALID:
//	errno = EINVAL;
	return -1;
}

void draw_singleQRcode(QRinput *stream, int mask)
{
	QRcode *qrcode;
	int width;

	QRinput_setVersionAndErrorCorrectionLevel(stream, version, level);//设置版本 和纠错级别
	if(micro)
	{
		qrcode = QRcode_encodeMaskMQR(stream, mask);
	} else
	{
		qrcode = QRcode_encodeMask(stream, mask);
	}
	if(qrcode == NULL)
	{
		width = (11 + margin * 2) * size;
		fprintf(stderr, "Input data does not fit to this setting.\n");
	} 
	else 
	{
		version = qrcode->version;
		width = (qrcode->width + margin * 2) * size;
	}

//	screen = SDL_SetVideoMode(width, width, 32, 0);
//	SDL_FillRect(screen, NULL, 0xffffff);
	//gdi_draw_solid_rect(0, 0, width, width, GDI_COLOR_WHITE);
	if(qrcode)
	{
		draw_QRcode(qrcode, QR_X0, QR_Y0);
	}
//	SDL_Flip(screen);
	QRcode_free(qrcode);
}

void view(int mode, QRinput *input)
{
	int flag = 1;
	int mask = -1;
	//SDL_Event event;
	int loop;

	//while(flag) 
	{
		if(mode) 
		{
			draw_structuredQRcodeFromText(textc, textv);
		} 
		else 
		{
			if(structured) 
			{
				draw_structuredQRcodeFromQRinput(input);
			} else 
			{
				draw_singleQRcode(input, mask);
			}
		}
		#if 0
		if(mode || structured)
		{
			printf("Version %d, Level %c.\n", version, levelChar[level]);
		} else 
		{
			printf("Version %d, Level %c, Mask %d.\n", version, levelChar[level], mask);
		}
		loop = 1;
		while(loop)
		{
			//usleep(10000);
			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_KEYDOWN) 
				{
					switch(event.key.keysym.sym) 
					{
					case SDLK_RIGHT:
						version++;
						if(version > QRSPEC_VERSION_MAX)
							version = QRSPEC_VERSION_MAX;
						loop = 0;
						break;
					case SDLK_LEFT:
						version--;
						if(version < 1)
							version = 1;
						loop = 0;
						break;
					case SDLK_UP:
						size++;
						loop = 0;
						break;
					case SDLK_DOWN:
						size--;
						if(size < 1) size = 1;
						loop = 0;
						break;
					case SDLK_0:
					case SDLK_1:
					case SDLK_2:
					case SDLK_3:
					case SDLK_4:
					case SDLK_5:
					case SDLK_6:
					case SDLK_7:
						if(!mode && !structured)
						{
							mask = (event.key.keysym.sym - SDLK_0);
							loop = 0;
						}
						break;
					case SDLK_8:
						if(!mode && !structured)
						{
							mask = -1;
							loop = 0;
						}
						break;
					case SDLK_l:
						level = QR_ECLEVEL_L;
						loop = 0;
						break;
					case SDLK_m:
						level = QR_ECLEVEL_M;
						loop = 0;
						break;
					case SDLK_h:
						level = QR_ECLEVEL_H;
						loop = 0;
						break;
					case SDLK_q:
						level = QR_ECLEVEL_Q;
						loop = 0;
						break;
					case SDLK_ESCAPE:
						loop = 0;
						flag = 0;
						break;
					default:
						break;
					}
				}
				if(event.type == SDL_QUIT)
				{
					loop = 0;
					flag = 0;
				}
			}
		}
		#endif
	}
}

void view_simple(const unsigned char *str, int length)
{
	QRinput *input;
	int ret;

	if(micro) 
	{
		input = QRinput_newMQR(version, level);
	} 
	else 
	{
		input = QRinput_new2(version, level);
	}
	
	if(input == NULL) 
	{
		//fprintf(stderr, "Memory allocation error.\n");
		//exit(EXIT_FAILURE);
   		//kal_prompt_trace(MOD_MMI,"[QR]Memory allocation error.");//HERO_DEBUG
   		return;
	}
	
	if(eightbit) 
	{
		ret = QRinput_append(input, QR_MODE_8, length, str);
	} 
	else 
	{
		ret = Split_splitStringToQRinput((char *)str, input, hint, casesensitive);
	}
	
	if(ret < 0) 
	{
		//perror("Encoding the input string");
		//exit(EXIT_FAILURE);
   		//kal_prompt_trace(MOD_MMI,"[QR]Encoding the input string");//HERO_DEBUG
   		return;
	}

	view(0, input);

	QRinput_free(input);
}
void GT_Qcode_show(const unsigned char *QR_TEST_STR,int x0,int y0,int w0,int h0)
{
	U8 QR_TEST_STR_UTF8[300] = {0};
	U8 Temp_length;
	U8 Temp_Version_NUM;
	int Temp_version;
	//mmi_chset_ucs2_to_utf8_string((uint8 *)QR_TEST_STR_UTF8, 300, (uint8 *)QR_TEST_STR);
	SCI_STRCPY(QR_TEST_STR_UTF8, QR_TEST_STR);//yangyu add
	
	Temp_length = strlen(QR_TEST_STR_UTF8);
#if	1
	if(Temp_length<= 17)
	{
		Temp_version = 1;
		Temp_Version_NUM = 21;
		
	}
	else if((Temp_length>17)&&(Temp_length<= 32))
	{
		Temp_version = 2;
		Temp_Version_NUM = 25;
	}
	else if((Temp_length>32)&&(Temp_length<= 52))
	{
		Temp_version = 3;
		Temp_Version_NUM = 29;
	}
	else if((Temp_length>52)&&(Temp_length<= 77))
	{
		Temp_version = 4;
		Temp_Version_NUM = 33;
	}	
	else if((Temp_length>77)&&(Temp_length<= 105))
	{
		Temp_version = 5;
		Temp_Version_NUM = 37;
	}
	else if((Temp_length>105)&&(Temp_length<= 133))
	{
		Temp_version = 6;
		Temp_Version_NUM = 41;
	}
	else if((Temp_length>133)&&(Temp_length<= 153))
	{
		Temp_version = 7;
		Temp_Version_NUM = 45;
	}
	else if(Temp_length>153)
	{
		Temp_version = 8;
		Temp_Version_NUM = 49;
	}	
#endif	
	if(w0<=h0)
	{	
			size = w0/(Temp_Version_NUM + margin * 2);	
	}
	else
	{	
			size = h0/(Temp_Version_NUM +  margin * 2);
	}
	QR_X0 =	x0;
	QR_Y0 =	y0;
	QR_Weigth = w0;
	QR_Height = h0;
	Version_NUM = Temp_Version_NUM;
	level = QR_ECLEVEL_L;
	version = Temp_version;
	gdi_draw_solid_rect(x0, y0, x0 + w0 - 1, y0 + h0 - 1, GDI_COLOR_WHITE);
	view_simple((const unsigned char *)QR_TEST_STR_UTF8,strlen(QR_TEST_STR_UTF8));
}
#endif  /*_SRV_QRCODE_C*/

