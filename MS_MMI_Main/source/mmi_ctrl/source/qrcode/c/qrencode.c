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

#include "qrencode.h"
#include "qrspec.h"
#include "mask.h"
#include "bitstream.h"
#include "qrinput.h"
#include "rscode.h"

#include "mqrspec.h"
#include "mmask.h"

#include "os_api.h"
#include "mmask.h"

#include "os_api.h"

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
	SCI_TRACE_LOW("QRcode_encodeMask RSblock_init enter");
	dl = QRspec_rsDataCodes1(spec);
	el = QRspec_rsEccCodes1(spec);
	SCI_TRACE_LOW("QRcode_encodeMask init_rs before1 dl =%d,el=%d",dl,el);
	rs = init_rs(8, 0x11d, 0, 1, el, 255 - dl - el);
	if(rs == NULL) 
	  {
	  	SCI_TRACE_LOW("QRcode_encodeMask init_rs return null");
	  	return -1;
	  }
	block = blocks;
	dp = data;
	ep = ecc;
	SCI_TRACE_LOW("QRcode_encodeMask RSblock_initBlock before1");
	for(i=0; i<QRspec_rsBlockNum1(spec); i++) {
		RSblock_initBlock(block, dl, dp, el, ep, rs);
		dp += dl;
		ep += el;
		block++;
	}

	if(QRspec_rsBlockNum2(spec) == 0) return 0;

	dl = QRspec_rsDataCodes2(spec);
	el = QRspec_rsEccCodes2(spec);
	SCI_TRACE_LOW("QRcode_encodeMask init_rs before2");
	rs = init_rs(8, 0x11d, 0, 1, el, 255 - dl - el);
	if(rs == NULL) return -1;
	SCI_TRACE_LOW("QRcode_encodeMask RSblock_initBlock before2");
	for(i=0; i<QRspec_rsBlockNum2(spec); i++) {
		RSblock_initBlock(block, dl, dp, el, ep, rs);
		dp += dl;
		ep += el;
		block++;
	}
	SCI_TRACE_LOW("QRcode_encodeMask RSblock_init end");
	return 0;
}

static void QRraw_free(QRRawCode *raw);

static QRRawCode *QRraw_new(QRinput *input)
{
    QRRawCode *raw;
    int spec[5], ret;
    SCI_TRACE_LOW("QRcode_encodeMask QRraw_new");
    raw = (QRRawCode *)SCI_ALLOC_APP(sizeof(QRRawCode));
    if(raw == NULL) return NULL;
    SCI_TRACE_LOW("QRcode_encodeMask kal_adm_alloc QRRawCode");
    raw->datacode = QRinput_getByteStream(input);
    if(raw->datacode == NULL)
    {
        SCI_FREE(raw);
    	return NULL;
    }

    QRspec_getEccSpec(input->version, input->level, spec);

    raw->version = input->version;
    raw->b1 = QRspec_rsBlockNum1(spec);
    raw->dataLength = QRspec_rsDataLength(spec);
    raw->eccLength = QRspec_rsEccLength(spec);
    raw->ecccode = (unsigned char *)SCI_ALLOC_APP(raw->eccLength);
    SCI_TRACE_LOW("QRcode_encodeMask kal_adm_alloc raw->ecccode");
    if(raw->ecccode == NULL)
    {
        SCI_FREE(raw->datacode);
        SCI_FREE(raw);
        return NULL;
    }

    raw->blocks = QRspec_rsBlockNum(spec);
    raw->rsblock = (RSblock *)SCI_ALLOC_APP(sizeof(RSblock)*raw->blocks);
    SCI_TRACE_LOW("QRcode_encodeMask kal_adm_alloc raw->rsblock");
    if(raw->rsblock == NULL) 
    {
    	SCI_TRACE_LOW("QRcode_encodeMask kal_adm_alloc raw->rsblock is null");
    	QRraw_free(raw);
    	return NULL;
    }
    SCI_TRACE_LOW("QRcode_encodeMask RSblock_init before");
    ret = RSblock_init(raw->rsblock, spec, raw->datacode, raw->ecccode);
    SCI_TRACE_LOW("QRcode_encodeMask RSblock_init");
    if(ret < 0) 
    {
    	QRraw_free(raw);
    	return NULL;
    }

    raw->count = 0;
    SCI_TRACE_LOW("QRcode_encodeMask end");
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
    if(raw != NULL)
    {
        SCI_FREE( raw->datacode);
        SCI_FREE( raw->ecccode);
        SCI_FREE( raw->rsblock);
        SCI_FREE( raw);
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

	raw = (MQRRawCode *)SCI_ALLOC_APP(sizeof(MQRRawCode));
	if(raw == NULL) return NULL;

	raw->version = input->version;
	raw->dataLength = MQRspec_getDataLength(input->version, input->level);
	raw->eccLength = MQRspec_getECCLength(input->version, input->level);
	raw->oddbits = raw->dataLength * 8 - MQRspec_getDataLengthBit(input->version, input->level);
	raw->datacode = QRinput_getByteStream(input);
	if(raw->datacode == NULL) {
		SCI_FREE(raw);
		return NULL;
	}
	raw->ecccode = (unsigned char *)SCI_ALLOC_APP(raw->eccLength);
	if(raw->ecccode == NULL) {
		SCI_FREE(raw->datacode);
		SCI_FREE(raw);
		return NULL;
	}

	raw->rsblock = (RSblock *)SCI_ALLOC_APP(sizeof(RSblock));
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
        SCI_FREE(raw->datacode);
        SCI_FREE(raw->ecccode);
        SCI_FREE(raw->rsblock);
        SCI_FREE(raw);
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

    filler = (FrameFiller *)SCI_ALLOC_APP(sizeof(FrameFiller));
    if(filler == NULL)
    {
        SCI_TRACE_LOW(" FrameFiller_new filler == NULL ");
        return NULL;
    } 
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
        SCI_TRACE_LOW(" FrameFiller_next ");
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
			SCI_FREE(filler);
			SCI_FREE(frame);
			return NULL;
		}
		*p = (unsigned char)(i & 0x7f) | 0x80;
	}
	SCI_FREE(filler);
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
		SCI_FREE(frame);
		return NULL;
	}
	length = MQRspec_getDataLengthBit(version, QR_ECLEVEL_L)
	       + MQRspec_getECCLength(version, QR_ECLEVEL_L) * 8;
	for(i=0; i<length; i++) {
		p = FrameFiller_next(filler);
		if(p == NULL) {
			SCI_FREE(filler);
			return frame;
		}
		*p = (unsigned char)(i & 0x7f) | 0x80;
	}
	SCI_FREE(filler);
	return frame;
}
#endif


/******************************************************************************
 * QR-code encoding
 *****************************************************************************/

static QRcode *QRcode_new(int version, int width, unsigned char *data)
{
    QRcode *qrcode;

    qrcode = (QRcode *)SCI_ALLOC_APP(sizeof(QRcode));
    if(qrcode == NULL) 
    {
        SCI_TRACE_LOW(" QRcode_new qrcode == NULL ");
        return NULL;
    }

    qrcode->version = version;
    qrcode->width = width;
    qrcode->data = data;

    return qrcode;
}

void QRcode_free(QRcode *qrcode)
{
    if(qrcode != NULL) 
    {
        SCI_FREE(qrcode->data);
        SCI_FREE(qrcode);
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
    SCI_TRACE_LOW(" QRcode_encodeMask start ");
    if(input->mqr)
    {
        SCI_TRACE_LOW(" QRcode_encodeMask start input->mqr ");
        return NULL;
    }
    if(input->version < 0 || input->version > QRSPEC_VERSION_MAX)
    {
        SCI_TRACE_LOW(" QRcode_encodeMask start version error ");
        return NULL;
    }
    if(input->level > QR_ECLEVEL_H) 
    {
        SCI_TRACE_LOW(" QRcode_encodeMask start level error ");
        return NULL;
    }
    SCI_TRACE_LOW(" QRcode_encodeMask QRraw_new befor");
    raw = QRraw_new(input);
    if(raw == NULL) 
    {
        SCI_TRACE_LOW(" QRcode_encodeMask raw == NULL");
        return NULL;
    }
    SCI_TRACE_LOW(" QRcode_encodeMask QRraw_new after version = %d", raw->version);
    version = raw->version;
    width = QRspec_getWidth(version);
    frame = QRspec_newFrame(version);
    SCI_TRACE_LOW(" QRcode_encodeMask newFrame ");

    if(frame == NULL)
    {
        SCI_TRACE_LOW(" QRcode_encodeMask frame == NULL ");
        QRraw_free(raw);
        return NULL;
    }
    filler = FrameFiller_new(width, frame, 0);
    SCI_TRACE_LOW(" QRcode_encodeMask FrameFiller_new ");
    if(filler == NULL) 
    {
        SCI_TRACE_LOW(" QRcode_encodeMask filler == NULL ");
        QRraw_free(raw);
        SCI_FREE( frame);
        return NULL;
    }

    SCI_TRACE_LOW(" QRcode_encodeMask QRraw_getCode before ");
    /* inteleaved data and ecc codes */
    for(i=0; i<raw->dataLength + raw->eccLength; i++)
    {
        code = QRraw_getCode(raw);
        bit = 0x80;
        for(j=0; j<8; j++) 
        {
            p = FrameFiller_next(filler);
            if(p == NULL)
            {
                SCI_TRACE_LOW(" QRcode_encodeMask FrameFiller_next p=NULL ");
                goto EXIT;
            }
            *p = 0x02 | ((bit & code) != 0);
            bit = bit >> 1;
        }
    }
    SCI_TRACE_LOW(" QRcode_encodeMask QRraw_getCode after ");

    /* remainder bits */
    j = QRspec_getRemainder(version);
    SCI_TRACE_LOW(" QRcode_encodeMask get getRemainder =%d,version=%d ", j, version );
    for(i=0; i<j; i++) 
    {
        p = FrameFiller_next(filler);
        if(p == NULL)
        {
            SCI_TRACE_LOW(" QRcode_encodeMask p == NULL ");
            goto EXIT;
        }
        *p = 0x02;
    }

    SCI_TRACE_LOW(" QRcode_encodeMask remainder bits mask %d", mask );
    /* masking */
    if(mask == -2) // just for debug purpose
    { 
        masked = (unsigned char *)SCI_ALLOC_APP(width * width);
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
    SCI_TRACE_LOW(" QRcode_encodeMask get masked %d ", masked );
    if(masked == NULL) 
    {
        SCI_TRACE_LOW(" QRcode_encodeMask masked == NULL ");
        goto EXIT;
    }
    qrcode = QRcode_new(version, width, masked);
    SCI_TRACE_LOW(" QRcode_encodeMask QRcode_new ");
    if(qrcode == NULL) 
    {
        SCI_TRACE_LOW(" QRcode_encodeMask qrcode == NULL ");
        SCI_FREE(masked);
    }
    SCI_TRACE_LOW(" QRcode_encodeMask masking");
EXIT:

    SCI_FREE(raw);
    SCI_FREE(filler);
    SCI_FREE(frame);
    SCI_TRACE_LOW(" QRcode_encodeMask end");
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
	SCI_TRACE_LOW("QRcode_encodeMaskMQR start");
	if(!input->mqr) {
		return NULL;
	}
	if(input->version <= 0 || input->version > MQRSPEC_VERSION_MAX) {
		return NULL;
	}
	if(input->level > QR_ECLEVEL_Q) {
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
		SCI_FREE(frame);
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
	SCI_FREE(filler);
	SCI_FREE(frame);
	SCI_TRACE_LOW("QRcode_encodeMaskMQR end");
	return qrcode;
}

QRcode *QRcode_encodeInput(QRinput *input)
{
	SCI_TRACE_LOW(" QRcode_encodeInput start ");
	if(input->mqr) {
		return QRcode_encodeMaskMQR(input, -1);
	} else {
		return QRcode_encodeMask(input, -1);
	}
	SCI_TRACE_LOW("  QRcode_encodeInput end ");
}

static QRcode *QRcode_encodeStringReal(const char *string, int version, QRecLevel level, int mqr, QRencodeMode hint, int casesensitive)
{
	QRinput *input;
	QRcode *code;
	int ret;

	if(string == NULL) {
		return NULL;
	}
	if(hint != QR_MODE_8 && hint != QR_MODE_KANJI) {
		return NULL;
	}

	if(mqr) {
		input = QRinput_newMQR(version, level);
	} else {
		input = QRinput_new2(version, level);
	}
	if(input == NULL) return NULL;

	ret = Split_splitStringToQRinput(string, input, hint, casesensitive);
	if(ret < 0) {
		QRinput_free(input);
		return NULL;
	}
	code = QRcode_encodeInput(input);
	QRinput_free(input);

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

	entry = (QRcode_List *)SCI_ALLOC_APP(sizeof(QRcode_List));
	if(entry == NULL) return NULL;

	entry->next = NULL;
	entry->code = NULL;

	return entry;
}

static void QRcode_List_freeEntry(QRcode_List *entry)
{
	if(entry != NULL) {
		QRcode_free(entry->code);
		SCI_FREE(entry);
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

static unsigned char QRcode_parity(const char *str, int size)
{
	unsigned char parity = 0;
	int i;

	for(i=0; i<size; i++) {
		parity ^= str[i];
	}

	return parity;
}

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
		return NULL;
	}
	if(!eightbit && (hint != QR_MODE_8 && hint != QR_MODE_KANJI)) {
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
		return NULL;
	}
	return QRcode_encodeDataStructured(strlen(string), (unsigned char *)string, version, level);
}

QRcode_List *QRcode_encodeStringStructured(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive)
{
	if(string == NULL) {
		return NULL;
	}
	return QRcode_encodeDataStructuredReal(strlen(string), (unsigned char *)string, version, level, 0, hint, casesensitive);
}

/******************************************************************************
 * System utilities
 *****************************************************************************/

void QRcode_APIVersion(int *major_version, int *minor_version, int *micro_version)
{
/*
	if(major_version != NULL) {
		*major_version = MAJOR_VERSION;
	}
	if(minor_version != NULL) {
		*minor_version = MINOR_VERSION;
	}
	if(micro_version != NULL) {
		*micro_version = MICRO_VERSION;
	}
*/	
}

char *QRcode_APIVersionString(void)
{
	return  0;//VERSION;
}

void QRcode_clearCache(void)
{
	QRspec_clearCache();
	MQRspec_clearCache();
	free_rs_cache();
}

