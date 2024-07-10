/******************************************************************************
 ** File Name:      jstream.h                                                 *
 ** Author:         Zhemin.Lin                                                *
 ** Date:           2004/07/19                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    stream operation interface                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2004/07/19     Zhemin.Lin       Create.                                   *
 ******************************************************************************/
#ifndef JPEG_STREAM_H
#define JPEG_STREAM_H

#include "sci_types.h"

#define READ_OP		0
#define WRITE_OP	1


//__inline uint16 JPEG_GetValSize(int16 val);
//extern uint32 jstream_len;

void JPEG_Fill_Bit_Buffer(void);
void JPEG_Fill_Buffer(void);
void JPEG_Input_Bytes_to_IRAM_from_SDRAM(void);
void JPEG_Output_Bytes_to_SDRAM_from_IRAM(uint32 data_len);

void JPEG_PutC(uint8 ch);
void JPEG_PutW(uint16 w);
void JPEG_PutBits(uint32 bits, uint32 size);
void JPEG_Flush(uint32 value);
uint16 JPEG_GetBit(void);
uint16 JPEG_GetW();
uint8 JPEG_GetC();
uint16 JPEG_GetBits(uint8 size);


#define JPEG_PUTC(ch)				JPEG_PutC(ch)
#define JPEG_PUTW(w)				JPEG_PutW(w)
#define JPEG_PUTBITS(bits, size)	JPEG_PutBits(bits, size)
#define JPEG_FLUSH(value)			JPEG_Flush(value)
#define JPEG_GETC()					JPEG_GetC()
#define JPEG_GETW()					JPEG_GetW()

//#define JPEG_GETVALSIZE(val)		JPEG_GetValSize(val)
#define JPEG_GETVALSIZE(val) \
	{\
		nbits = 0;\
		while(val)\
		{\
			nbits++;\
			val >>=1 ;\
		}\
	}


#define CHECK_BIT_BUFFER(nbits) \
	{ if (jremain_bit_num < (nbits)) {  \
	JPEG_Fill_Bit_Buffer();}\
 }

#define JPEG_GETBITS(nbits) \
	(((int) (jstream_words >> (jremain_bit_num -= (nbits)))) & ((1<<(nbits))-1))

#define PEEK_BITS(nbits) \
	(((int) (jstream_words >> (jremain_bit_num -  (nbits)))) & ((1<<(nbits))-1))

#define DROP_BITS(nbits) \
	(jremain_bit_num -= (nbits))			




uint32 JPEG_OpenStreamForRead(const char *filename);

uint32 JPEG_OpenStreamForWrite(const char *filename);

uint32 JPEG_CloseStream(void);

uint32 InitStreamForRW (uint8 * buf, uint32 len, uint8 ReadOrWrite);


//extern uint32 jstream_len;

#endif //JPEG_STREAM_H


