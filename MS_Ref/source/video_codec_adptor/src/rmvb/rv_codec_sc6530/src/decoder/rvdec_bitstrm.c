
#include "rvdec_video_header.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

PUBLIC void RvDec_InitBitstream (BitStream *stream, void *pBitstream, int32 length)
{
	uint32 offset = (uint32)pBitstream & 0x3;

	length = ((length >> 2) << 2) + 8;
	
	stream->p_stream = (uint8 *)((uint32)pBitstream - offset);   //word align address
	stream->stream_len = length;
	stream->stream_len_left = length;

	stream->bitcnt = 0;
	stream->bitsLeft = 32 - offset*8;	
	
#if defined(RV_BIG_ENDIAN)
	stream->rdptr = stream->rdbfr + 1; 
#else
	stream->rdptr = stream->rdbfr;
#endif
	
	if (BITSTREAM_BFR_SIZE * (int32)(sizeof (uint32)) >= length)
	{
		memcpy (stream->rdptr, stream->p_stream, length);
		stream->stream_len_left = 0;
	}
	else
	{
		memcpy (stream->rdptr, stream->p_stream, BITSTREAM_BFR_SIZE * sizeof(uint32));
		stream->p_stream += BITSTREAM_BFR_SIZE * sizeof(uint32);
		stream->stream_len_left -= BITSTREAM_BFR_SIZE * (int)sizeof(uint32);
	}
	
#if !defined(RV_BIG_ENDIAN)
	{
		uint8 * pCharTmp;
		
		pCharTmp = (uint8 *)stream->rdptr;
		
		
		((uint8 *)(&stream->bufa)) [0] = pCharTmp[3];
		((uint8 *)(&stream->bufa)) [1] = pCharTmp[2];
		((uint8 *)(&stream->bufa)) [2] = pCharTmp[1];
		((uint8 *)(&stream->bufa)) [3] = pCharTmp[0];
		
		((uint8 *)(&stream->bufb)) [0] = pCharTmp[7];
		((uint8 *)(&stream->bufb)) [1] = pCharTmp[6];
		((uint8 *)(&stream->bufb)) [2] = pCharTmp[5];
		((uint8 *)(&stream->bufb)) [3] = pCharTmp[4];
		
		stream->rdptr += 2;
	}
#endif
}

PUBLIC uint32 RvDec_ReadBits(BitStream *stream, uint32 nBits)
{
	uint32 iVar;

	iVar = RvDec_ShowBits(stream, nBits);
	RvDec_FlushBits(stream, nBits);

	return iVar;
}

void fill_bitstream_bfr (BitStream * stream)
{
	uint32 *ptr = NULL;			
				
#if defined(RV_BIG_ENDIAN)
	stream->rdbfr[0] = *stream->rdptr;
	ptr = stream->rdbfr + 1;
#else
	ptr = stream->rdbfr;		
#endif
	
	if (BITSTREAM_BFR_SIZE * (int32)(sizeof(int32)) > stream->stream_len_left)
	{
		if (stream->stream_len_left)
		{
			memcpy (ptr, stream->p_stream, stream->stream_len_left);
			stream->stream_len_left = 0;
		}
	}else
	{
		memcpy (ptr, stream->p_stream, BITSTREAM_BFR_SIZE * sizeof(int32));
		stream->p_stream += BITSTREAM_BFR_SIZE * sizeof (int32);
		stream->stream_len_left -= (int32) (BITSTREAM_BFR_SIZE * sizeof(int32));
	}		
	
	stream->rdptr = stream->rdbfr;  				
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
