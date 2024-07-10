/*
 * qrencode - QR Code encoder
 *
 * Copyright (C) 2006-2012 Kentaro Fukuchi <kentaro@fukuchi.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#if HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "qrencode.h"
#include "qrspec.h"
#include "mqrspec.h"
#include "bitstream.h"
#include "qrinput.h"
#include "rscode.h"
#include "split.h"
#include "mask.h"
#include "mmask.h"
//#include "common.h"
#include "mmi_base_common.h"
//#include "msg.h"
//#include "AEEFile.h"
//#include "fs.h"

/******************************************************************************
 * Raw code
 *****************************************************************************/

typedef struct {
	int dataLength;
	unsigned char *data;
	int eccLength;
	unsigned char *ecc;
} RSblock;

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

//#define QT_OUT_FILE    "brew/ewm.bmp\0"  // "fs:/qrpay.bmp"
// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)
#define OUT_FILE_PIXEL_PRESCALER    4       
// Color of bmp pixels  
#define PIXEL_COLOR_R               0        
#define PIXEL_COLOR_G               0
#define PIXEL_COLOR_B               0 //0xff
#define BI_RGB                      0L

//#pragma pack(push, 2) //2字节对齐，不然会出问题
typedef struct
{
    uint16 bfType;
    uint16  bfSize;
    uint16  bfSize2; 
    uint16 bfReserved1;
    uint16 bfReserved2;
    uint16 bfOffBits;
    uint16 bfOffbits2;
} BMPFILEHEADER;

#if 0
    unsigned long biSize;
    signed long biWidth;
    signed long biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long biCompression;
    unsigned long biSizeImage;
    signed long biXPelsPerMeter;
    signed long biYPelsPerMeter;
    unsigned long biClrUsed;
    unsigned long biClrImportant;
#endif

typedef struct
{
  unsigned long  biSize;          /*Number of bytes required by the structure. */
  unsigned long  biWidth;         /*Width of the bitmap, in pixels. */
  int32  biHeight;        /*Height of the bitmap, in pixels. If biHeight is positive, the bitmap is a bottom-up DIB and its origin is the lower-left corner. If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.
                            If biHeight is negative, indicating a top-down DIB, */
  uint16   biPlanes;        /*number of planes for the target device. This value must be set to 1. */
  uint16   biBitCount;      /*bits-per-pixel. */
  unsigned long  biCompression;   /*Type of compression for a compressed bottom-up bitmap (top-down DIBs cannot be compressed)*/
  unsigned long  biSizeImage;     /*size, in bytes, of the image*/
  unsigned long  biXPelsPerMeter; /*horizontal resolution*/
  unsigned long  biYPelsPerMeter; /*Vertical Resolution*/
  unsigned long  biClrUsed;       /*Number of color indexes in the color table that are actually used by the bitmap*/
  unsigned long  biClrImportant;  /*Number of color indexes that are required for displaying the bitmap*/	
} BMPINFOHEADER;
//#pragma pack(pop)

static void RSblock_initBlock(RSblock *block, int dl, unsigned char *data, int el, unsigned char *ecc, RS *rs)
{
	block->dataLength = dl;
	block->data = data;
	block->eccLength = el;
	block->ecc = ecc;

	encode_rs_char(rs, data, ecc);
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
	for(i=0; i<QRspec_rsBlockNum1(spec); i++) {
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
	for(i=0; i<QRspec_rsBlockNum2(spec); i++) {
		RSblock_initBlock(block, dl, dp, el, ep, rs);
		dp += dl;
		ep += el;
		block++;
	}

	return 0;
}

static void QRraw_free(QRRawCode *raw);
static QRRawCode *QRraw_new(QRinput *input)
{
	QRRawCode *raw;
	int spec[5], ret;

	raw = (QRRawCode *)malloc(sizeof(QRRawCode));
	if(raw == NULL) return NULL;

	raw->datacode = QRinput_getByteStream(input);
	if(raw->datacode == NULL) {
		free(raw);
		return NULL;
	}

	QRspec_getEccSpec(input->version, input->level, spec);

	raw->version = input->version;
	raw->b1 = QRspec_rsBlockNum1(spec);
	raw->dataLength = QRspec_rsDataLength(spec);
	raw->eccLength = QRspec_rsEccLength(spec);
	raw->ecccode = (unsigned char *)malloc(raw->eccLength);
	if(raw->ecccode == NULL) {
		free(raw->datacode);
		free(raw);
		return NULL;
	}

	raw->blocks = QRspec_rsBlockNum(spec);
	raw->rsblock = (RSblock *)calloc(raw->blocks, sizeof(RSblock));
	if(raw->rsblock == NULL) {
		QRraw_free(raw);
		return NULL;
	}
	ret = RSblock_init(raw->rsblock, spec, raw->datacode, raw->ecccode);
	if(ret < 0) {
		QRraw_free(raw);
		return NULL;
	}

	raw->count = 0;

	return raw;
}

/**
 * Return a code (byte).
 * This function can be called iteratively.
 * @param raw raw code.
 * @return code
 */
static unsigned char QRraw_getCode(QRRawCode *raw)
{
	int col, row;
	unsigned char ret;

	if(raw->count < raw->dataLength) {
		row = raw->count % raw->blocks;
		col = raw->count / raw->blocks;
		if(col >= raw->rsblock[0].dataLength) {
			row += raw->b1;
		}
		ret = raw->rsblock[row].data[col];
	} else if(raw->count < raw->dataLength + raw->eccLength) {
		row = (raw->count - raw->dataLength) % raw->blocks;
		col = (raw->count - raw->dataLength) / raw->blocks;
		ret = raw->rsblock[row].ecc[col];
	} else {
		return 0;
	}
	raw->count++;
	return ret;
}

static void QRraw_free(QRRawCode *raw)
{
	if(raw != NULL) {
		free(raw->datacode);
		free(raw->ecccode);
		free(raw->rsblock);
		free(raw);
	}
}

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

static void MQRraw_free(MQRRawCode *raw);
static MQRRawCode *MQRraw_new(QRinput *input)
{
	MQRRawCode *raw;
	RS *rs;

	raw = (MQRRawCode *)malloc(sizeof(MQRRawCode));
	if(raw == NULL) return NULL;

	raw->version = input->version;
	raw->dataLength = MQRspec_getDataLength(input->version, input->level);
	raw->eccLength = MQRspec_getECCLength(input->version, input->level);
	raw->oddbits = raw->dataLength * 8 - MQRspec_getDataLengthBit(input->version, input->level);
	raw->datacode = QRinput_getByteStream(input);
	if(raw->datacode == NULL) {
		free(raw);
		return NULL;
	}
	raw->ecccode = (unsigned char *)malloc(raw->eccLength);
	if(raw->ecccode == NULL) {
		free(raw->datacode);
		free(raw);
		return NULL;
	}

	raw->rsblock = (RSblock *)calloc(1, sizeof(RSblock));
	if(raw->rsblock == NULL) {
		MQRraw_free(raw);
		return NULL;
	}

	rs = init_rs(8, 0x11d, 0, 1, raw->eccLength, 255 - raw->dataLength - raw->eccLength);
	if(rs == NULL) {
		MQRraw_free(raw);
		return NULL;
	}

	RSblock_initBlock(raw->rsblock, raw->dataLength, raw->datacode, raw->eccLength, raw->ecccode, rs);

	raw->count = 0;

	return raw;
}

/**
 * Return a code (byte).
 * This function can be called iteratively.
 * @param raw raw code.
 * @return code
 */
static unsigned char MQRraw_getCode(MQRRawCode *raw)
{
	unsigned char ret;

	if(raw->count < raw->dataLength) {
		ret = raw->datacode[raw->count];
	} else if(raw->count < raw->dataLength + raw->eccLength) {
		ret = raw->ecccode[raw->count - raw->dataLength];
	} else {
		return 0;
	}
	raw->count++;
	return ret;
}

static void MQRraw_free(MQRRawCode *raw)
{
	if(raw != NULL) {
		free(raw->datacode);
		free(raw->ecccode);
		free(raw->rsblock);
		free(raw);
	}
}


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

static FrameFiller *FrameFiller_new(int width, unsigned char *frame, int mqr)
{
	FrameFiller *filler;

	filler = (FrameFiller *)malloc(sizeof(FrameFiller));
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

static unsigned char *FrameFiller_next(FrameFiller *filler)
{
	unsigned char *p;
	int x, y, w;

	if(filler->bit == -1) {
		filler->bit = 0;
		return filler->frame + filler->y * filler->width + filler->x;
	}

	x = filler->x;
	y = filler->y;
	p = filler->frame;
	w = filler->width;

	if(filler->bit == 0) {
		x--;
		filler->bit++;
	} else {
		x++;
		y += filler->dir;
		filler->bit--;
	}

	if(filler->dir < 0) {
		if(y < 0) {
			y = 0;
			x -= 2;
			filler->dir = 1;
			if(!filler->mqr && x == 6) {
				x--;
				y = 9;
			}
		}
	} else {
		if(y == w) {
			y = w - 1;
			x -= 2;
			filler->dir = -1;
			if(!filler->mqr && x == 6) {
				x--;
				y -= 8;
			}
		}
	}
	if(x < 0 || y < 0) return NULL;

	filler->x = x;
	filler->y = y;

	if(p[y * w + x] & 0x80) {
		// This tail recursion could be optimized.
		return FrameFiller_next(filler);
	}
	return &p[y * w + x];
}

#ifdef WITH_TESTS
extern unsigned char *FrameFiller_test(int version)
{
	int width;
	unsigned char *frame, *p;
	FrameFiller *filler;
	int i, length;

	width = QRspec_getWidth(version);
	frame = QRspec_newFrame(version);
	if(frame == NULL) return NULL;
	filler = FrameFiller_new(width, frame, 0);
	if(filler == NULL) {
		free(frame);
		return NULL;
	}
	length = QRspec_getDataLength(version, QR_ECLEVEL_L) * 8
	       + QRspec_getECCLength(version, QR_ECLEVEL_L) * 8
		   + QRspec_getRemainder(version);
	for(i=0; i<length; i++) {
		p = FrameFiller_next(filler);
		if(p == NULL) {
			free(filler);
			free(frame);
			return NULL;
		}
		*p = (unsigned char)(i & 0x7f) | 0x80;
	}
	free(filler);
	return frame;
}

extern unsigned char *FrameFiller_testMQR(int version)
{
	int width;
	unsigned char *frame, *p;
	FrameFiller *filler;
	int i, length;

	width = MQRspec_getWidth(version);
	frame = MQRspec_newFrame(version);
	if(frame == NULL) return NULL;
	filler = FrameFiller_new(width, frame, 1);
	if(filler == NULL) {
		free(frame);
		return NULL;
	}
	length = MQRspec_getDataLengthBit(version, QR_ECLEVEL_L)
	       + MQRspec_getECCLength(version, QR_ECLEVEL_L) * 8;
	for(i=0; i<length; i++) {
		p = FrameFiller_next(filler);
		if(p == NULL) {
			fprintf(stderr, "Frame filler run over the frame!\n");
			free(filler);
			return frame;
		}
		*p = (unsigned char)(i & 0x7f) | 0x80;
	}
	free(filler);
	return frame;
}
#endif


/******************************************************************************
 * QR-code encoding
 *****************************************************************************/

static QRcode *QRcode_new(int version, int width, unsigned char *data)
{
	QRcode *qrcode;

	qrcode = (QRcode *)malloc(sizeof(QRcode));
	if(qrcode == NULL) return NULL;

	qrcode->version = version;
	qrcode->width = width;
	qrcode->data = data;

	return qrcode;
}

void QRcode_free(QRcode *qrcode)
{
	if(qrcode != NULL) {
		free(qrcode->data);
		free(qrcode);
	}
}

static QRcode *QRcode_encodeMask(QRinput *input, int mask)
{
	int width, version;
	QRRawCode *raw;
	unsigned char *frame, *masked, *p, code, bit;
	FrameFiller *filler;
	int i, j;
	QRcode *qrcode = NULL;

	if(input->mqr) {
//		errno = EINVAL;
		return NULL;
	}
	if(input->version < 0 || input->version > QRSPEC_VERSION_MAX) {
//		errno = EINVAL;
		return NULL;
	}
	if(input->level > QR_ECLEVEL_H) {
//		errno = EINVAL;
		return NULL;
	}

	raw = QRraw_new(input);
	if(raw == NULL) return NULL;

	version = raw->version;
	width = QRspec_getWidth(version);
	frame = QRspec_newFrame(version);
	if(frame == NULL) {
		QRraw_free(raw);
		return NULL;
	}
	filler = FrameFiller_new(width, frame, 0);
	if(filler == NULL) {
		QRraw_free(raw);
		free(frame);
		return NULL;
	}

	/* inteleaved data and ecc codes */
	for(i=0; i<raw->dataLength + raw->eccLength; i++) {
		code = QRraw_getCode(raw);
		bit = 0x80;
		for(j=0; j<8; j++) {
			p = FrameFiller_next(filler);
			if(p == NULL)  goto EXIT;
			*p = 0x02 | ((bit & code) != 0);
			bit = bit >> 1;
		}
	}
	QRraw_free(raw);
	raw = NULL;
	/* remainder bits */
	j = QRspec_getRemainder(version);
	for(i=0; i<j; i++) {
		p = FrameFiller_next(filler);
		if(p == NULL)  goto EXIT;
		*p = 0x02;
	}

	/* masking */
	if(mask == -2) { // just for debug purpose
		masked = (unsigned char *)malloc(width * width);
		memcpy(masked, frame, width * width);
	} else if(mask < 0) {
		masked = Mask_mask(width, frame, input->level);
	} else {
		masked = Mask_makeMask(width, frame, mask, input->level);
	}
	if(masked == NULL) {
		goto EXIT;
	}
	qrcode = QRcode_new(version, width, masked);
	if(qrcode == NULL) {
		free(masked);
	}

EXIT:
	QRraw_free(raw);
	free(filler);
	free(frame);
	return qrcode;
}

static QRcode *QRcode_encodeMaskMQR(QRinput *input, int mask)
{
	int width, version;
	MQRRawCode *raw;
	unsigned char *frame, *masked, *p, code, bit;
	FrameFiller *filler;
	int i, j;
	QRcode *qrcode = NULL;

	if(!input->mqr) {
//		errno = EINVAL;
		return NULL;
	}
	if(input->version <= 0 || input->version > MQRSPEC_VERSION_MAX) {
//		errno = EINVAL;
		return NULL;
	}
	if(input->level > QR_ECLEVEL_Q) {
//		errno = EINVAL;
		return NULL;
	}

	raw = MQRraw_new(input);
	if(raw == NULL) return NULL;

	version = raw->version;
	width = MQRspec_getWidth(version);
	frame = MQRspec_newFrame(version);
	if(frame == NULL) {
		MQRraw_free(raw);
		return NULL;
	}
	filler = FrameFiller_new(width, frame, 1);
	if(filler == NULL) {
		MQRraw_free(raw);
		free(frame);
		return NULL;
	}

	/* inteleaved data and ecc codes */
	for(i=0; i<raw->dataLength + raw->eccLength; i++) {
		code = MQRraw_getCode(raw);
		if(raw->oddbits && i == raw->dataLength - 1) {
			bit = 1 << (raw->oddbits - 1);
			for(j=0; j<raw->oddbits; j++) {
				p = FrameFiller_next(filler);
				if(p == NULL) goto EXIT;
				*p = 0x02 | ((bit & code) != 0);
				bit = bit >> 1;
			}
		} else {
			bit = 0x80;
			for(j=0; j<8; j++) {
				p = FrameFiller_next(filler);
				if(p == NULL) goto EXIT;
				*p = 0x02 | ((bit & code) != 0);
				bit = bit >> 1;
			}
		}
	}
	MQRraw_free(raw);
	raw = NULL;

	/* masking */
	if(mask < 0) {
		masked = MMask_mask(version, frame, input->level);
	} else {
		masked = MMask_makeMask(version, frame, mask, input->level);
	}
	if(masked == NULL) {
		goto EXIT;
	}

	qrcode = QRcode_new(version, width, masked);

EXIT:
	MQRraw_free(raw);
	free(filler);
	free(frame);
	return qrcode;
}

QRcode *QRcode_encodeInput(QRinput *input)
{
	if(input->mqr) {
		return QRcode_encodeMaskMQR(input, -1);
	} else {
		return QRcode_encodeMask(input, -1);
	}
}

static QRcode *QRcode_encodeStringReal(const char *string, int version, QRecLevel level, int mqr, QRencodeMode hint, int casesensitive)
{
	QRinput *input;
	QRcode *code;
	int ret;

	if(string == NULL) {
//		errno = EINVAL;
		//MSG_HIGH("string == NULL",0,0,0);
		return NULL;
	}
	if(hint != QR_MODE_8 && hint != QR_MODE_KANJI) {
//		errno = EINVAL;
		//MSG_HIGH("hint= %d",hint,0,0);
		return NULL;
	}

	if(mqr) {
		input = QRinput_newMQR(version, level);
	} else {
		input = QRinput_new2(version, level);
	}
	if(input == NULL) return NULL;

	ret = Split_splitStringToQRinput(string, input, hint, casesensitive);
		//MSG_HIGH("ret= %d",ret,0,0);
	if(ret < 0) {
		QRinput_free(input);
		return NULL;
	}
	code = QRcode_encodeInput(input);
	QRinput_free(input);
		//MSG_HIGH("QRcode_encodeStringReal",0,0,0);
	return code;
}

QRcode *QRcode_encodeString(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive)
{
	return QRcode_encodeStringReal(string, version, level, 0, hint, casesensitive);
}

QRcode *QRcode_encodeStringMQR(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive)
{
	return QRcode_encodeStringReal(string, version, level, 1, hint, casesensitive);
}

static QRcode *QRcode_encodeDataReal(const unsigned char *data, int length, int version, QRecLevel level, int mqr)
{
	QRinput *input;
	QRcode *code;
	int ret;

	if(data == NULL || length == 0) {
//		errno = EINVAL;
		return NULL;
	}

	if(mqr) {
		input = QRinput_newMQR(version, level);
	} else {
		input = QRinput_new2(version, level);
	}
	if(input == NULL) return NULL;

	ret = QRinput_append(input, QR_MODE_8, length, data);
	if(ret < 0) {
		QRinput_free(input);
		return NULL;
	}
	code = QRcode_encodeInput(input);
	QRinput_free(input);

	return code;
}

QRcode *QRcode_encodeData(int size, const unsigned char *data, int version, QRecLevel level)
{
	return QRcode_encodeDataReal(data, size, version, level, 0);
}

QRcode *QRcode_encodeString8bit(const char *string, int version, QRecLevel level)
{
	if(string == NULL) {
//		errno = EINVAL;
		return NULL;
	}
	return QRcode_encodeDataReal((unsigned char *)string, strlen(string), version, level, 0);
}

QRcode *QRcode_encodeDataMQR(int size, const unsigned char *data, int version, QRecLevel level)
{
	return QRcode_encodeDataReal(data, size, version, level, 1);
}

QRcode *QRcode_encodeString8bitMQR(const char *string, int version, QRecLevel level)
{
	if(string == NULL) {
//		errno = EINVAL;
		return NULL;
	}
	return QRcode_encodeDataReal((unsigned char *)string, strlen(string), version, level, 1);
}


/******************************************************************************
 * Structured QR-code encoding
 *****************************************************************************/

static QRcode_List *QRcode_List_newEntry(void)
{
	QRcode_List *entry;

	entry = (QRcode_List *)malloc(sizeof(QRcode_List));
	if(entry == NULL) return NULL;

	entry->next = NULL;
	entry->code = NULL;

	return entry;
}

static void QRcode_List_freeEntry(QRcode_List *entry)
{
	if(entry != NULL) {
		QRcode_free(entry->code);
		free(entry);
	}
}

void QRcode_List_free(QRcode_List *qrlist)
{
	QRcode_List *list = qrlist, *next;

	while(list != NULL) {
		next = list->next;
		QRcode_List_freeEntry(list);
		list = next;
	}
}

int QRcode_List_size(QRcode_List *qrlist)
{
	QRcode_List *list = qrlist;
	int size = 0;

	while(list != NULL) {
		size++;
		list = list->next;
	}

	return size;
}

#if 0
static unsigned char QRcode_parity(const char *str, int size)
{
	unsigned char parity = 0;
	int i;

	for(i=0; i<size; i++) {
		parity ^= str[i];
	}

	return parity;
}
#endif

QRcode_List *QRcode_encodeInputStructured(QRinput_Struct *s)
{
	QRcode_List *head = NULL;
	QRcode_List *tail = NULL;
	QRcode_List *entry;
	QRinput_InputList *list = s->head;

	while(list != NULL) {
		if(head == NULL) {
			entry = QRcode_List_newEntry();
			if(entry == NULL) goto ABORT;
			head = entry;
			tail = head;
		} else {
			entry = QRcode_List_newEntry();
			if(entry == NULL) goto ABORT;
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

static QRcode_List *QRcode_encodeInputToStructured(QRinput *input)
{
	QRinput_Struct *s;
	QRcode_List *codes;

	s = QRinput_splitQRinputToStruct(input);
	if(s == NULL) return NULL;

	codes = QRcode_encodeInputStructured(s);
	QRinput_Struct_free(s);

	return codes;
}

static QRcode_List *QRcode_encodeDataStructuredReal(
	int size, const unsigned char *data,
	int version, QRecLevel level,
	int eightbit, QRencodeMode hint, int casesensitive)
{
	QRinput *input;
	QRcode_List *codes;
	int ret;

	if(version <= 0) {
//		errno = EINVAL;
		return NULL;
	}
	if(!eightbit && (hint != QR_MODE_8 && hint != QR_MODE_KANJI)) {
//		errno = EINVAL;
		return NULL;
	}

	input = QRinput_new2(version, level);
	if(input == NULL) return NULL;

	if(eightbit) {
		ret = QRinput_append(input, QR_MODE_8, size, data);
	} else {
		ret = Split_splitStringToQRinput((char *)data, input, hint, casesensitive);
	}
	if(ret < 0) {
		QRinput_free(input);
		return NULL;
	}
	codes = QRcode_encodeInputToStructured(input);
	QRinput_free(input);

	return codes;
}

QRcode_List *QRcode_encodeDataStructured(int size, const unsigned char *data, int version, QRecLevel level) {
	return QRcode_encodeDataStructuredReal(size, data, version, level, 1, QR_MODE_NUL, 0);
}

QRcode_List *QRcode_encodeString8bitStructured(const char *string, int version, QRecLevel level) {
	if(string == NULL) {
//		errno = EINVAL;
		return NULL;
	}
	return QRcode_encodeDataStructured(strlen(string), (unsigned char *)string, version, level);
}

QRcode_List *QRcode_encodeStringStructured(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive)
{
	if(string == NULL) {
//		errno = EINVAL;
		return NULL;
	}
	return QRcode_encodeDataStructuredReal(strlen(string), (unsigned char *)string, version, level, 0, hint, casesensitive);
}

/******************************************************************************
 * System utilities
 *****************************************************************************/

#if 0
void QRcode_APIVersion(int *major_version, int *minor_version, int *micro_version)
{
	if(major_version != NULL) {
		*major_version = MAJOR_VERSION;
	}
	if(minor_version != NULL) {
		*minor_version = MINOR_VERSION;
	}
	if(micro_version != NULL) {
		*micro_version = MICRO_VERSION;
	}
}
#endif
#if 0
char *QRcode_APIVersionString(void)
{
	return VERSION;
}
#endif

void QRcode_clearCache(void)
{
	QRspec_clearCache();
	MQRspec_clearCache();
	free_rs_cache();
}
#define QT_OUT_FILE    "ewm.bmp"
int Save_QEImage(char* szSourceSring,uint8 ** ppImg, uint32 *pLen)
{
    unsigned int unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
    unsigned char* pRGBData, *pSourceData, *pDestData;
    QRcode* pQRC;
    uint8 * p_img = NULL;
//    FILE* f;
    BMPFILEHEADER kFileHeader;
    BMPINFOHEADER kInfoHeader;	
    //fs_rsp_msg_type rsp;
    uint32  bmp_buf_len = 0;
    uint32 head_len = 0;
    uint32 info_len = 0;
    uint32 handletmp;	
    // Compute QRCode
           // MSG_HIGH("Save_QEImage",0,0,0);
    if  (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_M, QR_MODE_8, 1))
    {
        //矩阵的维数
        head_len = sizeof(BMPFILEHEADER);
        info_len = sizeof(BMPINFOHEADER);
        unWidth = pQRC->width;
        unWidthAdjusted = unWidth * OUT_FILE_PIXEL_PRESCALER * 3;
           // MSG_HIGH("unWidth=%d unWidthAdjusted=%d",unWidth,unWidthAdjusted,0);
        //每一个维度占的像素的个数（8），每个像素3个字节
        if (unWidthAdjusted % 4)
        {
            unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
        }
        unDataBytes = unWidthAdjusted * unWidth * OUT_FILE_PIXEL_PRESCALER;
            //MSG_HIGH("unDataBytes=%d sizeof(BMPFILEHEADER) =%d  sizeof(BMPINFOHEADER)=%d",unDataBytes,sizeof(BMPFILEHEADER) ,sizeof(BMPINFOHEADER));
        // Allocate pixels buffer
		bmp_buf_len = head_len+info_len+unDataBytes;
        if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
        {
            //MSG_HIGH("Out of memory",0,0,0);
            return 1;
        }

        // Preset to white
        memset(pRGBData, 0xff, unDataBytes);

        // Prepare bmp headers
	 memset(&kFileHeader, 0, sizeof(BMPFILEHEADER));
        kFileHeader.bfType = 0x4d42;  // "BM"
	kFileHeader.bfSize = bmp_buf_len;
	kFileHeader.bfSize2 = bmp_buf_len>>16;
        kFileHeader.bfReserved1 = 0;
        kFileHeader.bfReserved2 = 0;
        kFileHeader.bfOffBits = (uint16)(sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER));
	kFileHeader.bfOffbits2 =0;

	memset(&kInfoHeader, 0, sizeof(BMPINFOHEADER));
        kInfoHeader.biSize = sizeof(BMPINFOHEADER);
        kInfoHeader.biWidth = unWidth * OUT_FILE_PIXEL_PRESCALER;
        kInfoHeader.biHeight = ((int)unWidth * OUT_FILE_PIXEL_PRESCALER);
        kInfoHeader.biPlanes = 1;
        kInfoHeader.biBitCount = 24;
        kInfoHeader.biCompression = BI_RGB;
        kInfoHeader.biSizeImage = 0;
        kInfoHeader.biXPelsPerMeter = 0;
        kInfoHeader.biYPelsPerMeter = 0;
        kInfoHeader.biClrUsed = 0;
        kInfoHeader.biClrImportant = 0;

#if 1
        // Convert QrCode bits to bmp pixels
        pSourceData = pQRC->data;
        for (y = 0; y < unWidth; y++)
        {
            pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;
            for (x = 0; x < unWidth; x++)
            {
                if (*pSourceData & 1)
                {
                    for (l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
                    {
                        for (n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
                        {
                            *(pDestData + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_B;
                            *(pDestData + 1 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_G;
                            *(pDestData + 2 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_R;
                        }
                    }
                }
                pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
                pSourceData++;
            }
        }
#endif
        // Output the bmp file
//        f = fs_open(QT_OUT_FILE,"wb");
	{
            #if 1
            p_img = SCI_ALLOC_APP(bmp_buf_len);
            if(p_img == NULL)
            {
                free(pRGBData);
                QRcode_free(pQRC);
                return 1;
            }
            SCI_MEMCPY(p_img,(uint8 *)&kFileHeader,head_len);
            SCI_MEMCPY(p_img+head_len,(uint8 *)&kInfoHeader,info_len);
            SCI_MEMCPY(p_img+head_len+info_len,(uint8 *)pRGBData,unDataBytes);
            * ppImg = p_img;
            *pLen = bmp_buf_len;
            #else
            char tmp_char[200] = {0};
            sprintf((char*)tmp_char,"ewm.bmp");
            UserFile_DeleteU("Others",QT_OUT_FILE);
            UserFile_WriteU("Others",QT_OUT_FILE,(uint8 *)&kFileHeader,sizeof(BMPFILEHEADER));
            UserFile_WriteExtU("Others",QT_OUT_FILE,FALSE,(uint8 *)&kInfoHeader,sizeof(BMPINFOHEADER));	
            UserFile_WriteExtU("Others",QT_OUT_FILE,FALSE,(uint8 *)pRGBData,sizeof(unsigned char)*unDataBytes);
            #endif
	}
#if 0
    	fs_test(QT_OUT_FILE, NULL, &rsp);
    	if (rsp.nametest.name_found == FALSE)
    	{
	 	fs_open(QT_OUT_FILE, FS_OA_CREATE, NULL, NULL, &rsp);
    	}
	else
	{
	 	fs_open(QT_OUT_FILE, FS_OA_READWRITE, NULL, NULL, &rsp);
	}
        //f = fopen(QT_OUT_FILE,"ab++");
        if (rsp.open.status != FS_OKAY_S)
        {
           // MSG_HIGH("Unable to open file\n",0,0,0);
            return 1;
        }
	 else 	 
        {
#if 0        
        	handletmp = rsp.open.handle; 
	        fs_write((fs_handle_type) handletmp, &kFileHeader,
	                    sizeof(BMPFILEHEADER), NULL, &rsp);

			
		fs_seek_write((fs_handle_type) handletmp, /* Handle of file to write to */
		                            FS_SEEK_SET, /* Seek origin */
		                            sizeof(BMPFILEHEADER), /* Position to seek to */
		                            &kInfoHeader, /* Buffer with bytes to write */
		                            sizeof(BMPINFOHEADER), /* Number of bytes to write */
		                            NULL,/* Callback function pointer */
		                            & rsp  /* Response message pointer */);
#if 1
		fs_seek_write((fs_handle_type) handletmp, /* Handle of file to write to */
		                            FS_SEEK_SET, /* Seek origin */
		                            sizeof(BMPFILEHEADER)+sizeof(BMPINFOHEADER), /* Position to seek to */
		                            pRGBData, /* Buffer with bytes to write */
		                            sizeof(unsigned char)*unDataBytes, /* Number of bytes to write */
		                            NULL,/* Callback function pointer */
		                            & rsp  /* Response message pointer */);		
#endif		
//	        fs_write((fs_handle_type) handletmp, &kInfoHeader,
//	                    sizeof(BITMAPINFOHEADER), NULL, &rsp);
//	        fs_write((fs_handle_type) handletmp, pRGBData,
//	                    sizeof(unsigned char)*unDataBytes, NULL, &rsp);			
        
//            	fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
//            	fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
//            	fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);

	        if (rsp.write.status != FS_OKAY_S)
	        {
	            fs_close((fs_handle_type) handletmp, NULL, &rsp);
	            //MSG_HIGH("YX_DB_WHITE_ListInit ERR DB Write",0,0,0);
	            return 1;
	        }
	        fs_close((fs_handle_type) handletmp, NULL, &rsp);
#endif			
		
        }
#endif	
        // Free data
        free(pRGBData);
        QRcode_free(pQRC);
    }
    else
    {
       // MSG_HIGH("NULL returned",0,0,0);
        return 1;
    }

    return 0;
}
