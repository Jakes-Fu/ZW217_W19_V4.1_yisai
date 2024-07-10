

#ifndef _RVDEC_BITSTRM_H_
#define _RVDEC_BITSTRM_H_

#include "rvdec_basic.h"

#define BITSTREAM_BFR_SIZE		128

typedef struct bitstream
{ 
#if !defined(RV_BIG_ENDIAN)
    uint32 bufa;
    uint32 bufb;
#endif
	uint32 bitcnt;
    uint32 bitsLeft; // left bits in the word pointed by rdptr
    uint32 *rdptr;
	uint32 rdbfr[BITSTREAM_BFR_SIZE + 1];	// bitstream data

	/*a nalu information*/
	int32 stream_len;		//length of the nalu
	int32 stream_len_left;	//left length not decoded of the nalu
	uint8 *p_stream;		//point to the nalu(current decoded position)
}BitStream;

PUBLIC void RvDec_InitBitstream(BitStream *stream, void *pBitstream, int32 length);
PUBLIC uint32 RvDec_ReadBits(BitStream *stream, uint32 nbits);
PUBLIC void fill_bitstream_bfr(BitStream *stream);

#if defined(RV_BIG_ENDIAN)	//big endian
#define RvDec_ShowBits(stream, nBits) \
((nBits) <= stream->bitsLeft) ? ((*stream->rdptr) >> (stream->bitsLeft - (nBits))) & g_msk_rvdec_ptr [nBits] : \
	(((*stream->rdptr) & g_msk_rvdec_ptr [stream->bitsLeft]) << ((nBits) - stream->bitsLeft)) | ((*(stream->rdptr+1)) >> (32 - ((nBits) - stream->bitsLeft))) 

#define RvDec_FlushBits(stream, nbits) \
{ \
stream->bitcnt += (nbits); \
if ((nbits) < stream->bitsLeft) \
{		\
	stream->bitsLeft -= (nbits);	\
} \
else \
{\
	stream->bitsLeft += 32 - (nbits);\
	stream->rdptr++;\
  if (stream->rdptr >=  stream->rdbfr + BITSTREAM_BFR_SIZE)\
  fill_bitstream_bfr (stream); \
} \
}

__inline uint32 RvDec_Read1Bits(BitStream *stream)
{
	uint32 val;
	
	val = ((*stream->rdptr) >> (stream->bitsLeft -1))&0x1;
	stream->bitcnt++;
	stream->bitsLeft--;

	if (!stream->bitsLeft)
	{
		stream->rdptr++;
  		if (stream->rdptr >=  stream->rdbfr + BITSTREAM_BFR_SIZE)
  		{
		 	fill_bitstream_bfr (stream); 
  		}
  		stream->bitsLeft = 32;
	}
	
	return val;
}
#else	//little endian

#define RvDec_ShowBits(stream, nBits) \
	((nBits) <= stream->bitsLeft) ? ( (stream->bufa >> (stream->bitsLeft - (nBits)) ) & g_msk_rvdec_ptr [nBits] ) \
	: ((stream->bufa << ((nBits) - stream->bitsLeft)) | (stream->bufb >> (32 - (nBits) + stream->bitsLeft))) & g_msk_rvdec_ptr [nBits]

#define RvDec_FlushBits(stream, nbits) \
{ \
	stream->bitcnt += (nbits); \
	if ((nbits) < stream->bitsLeft) \
		stream->bitsLeft -= (nbits);	\
	else \
{\
	uint8 *pCharTmp; \
		stream->bitsLeft += 32 - (nbits); \
		stream->bufa = stream->bufb; \
		if (stream->rdptr >=  stream->rdbfr + BITSTREAM_BFR_SIZE) \
		fill_bitstream_bfr (stream); \
		\
		pCharTmp = (uint8 *)stream->rdptr; \
		((uint8 *)&stream->bufb)[0] = pCharTmp[3]; \
		((uint8 *)&stream->bufb)[1] = pCharTmp[2]; \
		((uint8 *)&stream->bufb)[2] = pCharTmp[1]; \
		((uint8 *)&stream->bufb)[3] = pCharTmp[0]; \
		stream->rdptr++; \
}\
}		

__inline uint32 RvDec_Read1Bits(BitStream *stream)
{
	uint32 val;
	
	val = (stream->bufa >> (stream->bitsLeft -1))&0x1;
	stream->bitcnt++;
	stream->bitsLeft--;

	if (!stream->bitsLeft)
	{
		uint8 *pCharTmp; 
		
		stream->bufa = stream->bufb;
		if (stream->rdptr >=  (stream->rdbfr + BITSTREAM_BFR_SIZE))
		{
			fill_bitstream_bfr (stream); 
		}
		
		pCharTmp = (uint8 *)stream->rdptr; 
		((uint8 *)&stream->bufb)[0] = pCharTmp[3]; 
		((uint8 *)&stream->bufb)[1] = pCharTmp[2];
		((uint8 *)&stream->bufb)[2] = pCharTmp[1]; 
		((uint8 *)&stream->bufb)[3] = pCharTmp[0]; 
		stream->rdptr++; 
		stream->bitsLeft = 32;
	}

	return val;
}
#endif

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_BITSTRM_H_
