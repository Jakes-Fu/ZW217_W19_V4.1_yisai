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

void JPEG_PutC(uint8 ch);
void JPEG_PutW(uint16 w);
void JPEG_PutBits(uint16 bits, uint16 size);
void JPEG_Flush(uint32 value);
uint16 JPEG_GetBit();
uint16 JPEG_GetBits(uint8 size);
uint8 JPEG_GetC();
uint16 JPEG_GetW();
uint16 JPEG_GetValSize(int16 val);


#ifndef MIPS_STREAM_OP

#define JPEG_PUTC(ch)				JPEG_PutC(ch)
#define JPEG_PUTW(w)				JPEG_PutW(w)
#define JPEG_PUTBITS(bits, size)	JPEG_PutBits(bits, size)
#define JPEG_FLUSH(value)			JPEG_Flush(value)
#define JPEG_GETBIT()				JPEG_GetBit()
#define JPEG_GETBITS(size)			JPEG_GetBits(size)
#define JPEG_GETC()					JPEG_GetC()
#define JPEG_GETW()					JPEG_GetW()
//#define JPEG_GETVALSIZE(val)		JPEG_GetValSize(val)
#define JPEG_GETVALSIZE(val) \
	{\
		if (val <0) \
		{\
			val = -val;\
		}\
		nbits = 0;\
		while(val)\
		{\
			nbits++;\
			val >>=1 ;\
		}\
	}

#else

#define JPEG_GETVALSIZE(val)		16
#define JPEG_PUTC(ch)				
#define JPEG_PUTW(w)				
#define JPEG_PUTBITS(bits, size)	
#define JPEG_FLUSH(value)			
#define JPEG_GETBIT()				JPEG_GetBit()
#define JPEG_GETBITS(size)			JPEG_GetBits(size)
#define JPEG_GETC()					JPEG_GetC()
#define JPEG_GETW()					JPEG_GetW()

#endif


uint32 JPEG_OpenStreamForRead(const char *filename);

uint32 JPEG_OpenStreamForWrite(const char *filename);

uint32 JPEG_CloseStream(void);

uint32 InitStreamForRW (uint8 * buf, uint32 len, uint8 ReadOrWrite);
#endif //JPEG_STREAM_H


