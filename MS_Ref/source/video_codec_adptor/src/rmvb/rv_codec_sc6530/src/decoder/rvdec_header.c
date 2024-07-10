
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_video_header.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

LOCAL void RvDec_ComputeEntropyQP(RV_DECODER_T *rv_decoder_ptr)
{
	int32 entropyQP;
	int32 QP = rv_decoder_ptr->uPQuant;
	int32 OSVQUANT = rv_decoder_ptr->OSVQUANT;

	/*compute entropy qp*/
	entropyQP = QP;
	
	switch(OSVQUANT)
	{
	case 0:
		break;
	case 1:
		if (QP <= 25)
		{
			entropyQP = mmin(30, QP+5);
		}
		break;
	case 2:
		if (QP <= 18)
		{
			entropyQP = mmin(30, QP+10);
		}else if (QP <= 25)
		{
			entropyQP = mmin(30, QP+5);
		}
		break;
	default:
	//	PRINTF ("illegal OSVQUANT VALUE!\n");
	        break;
	}

	rv_decoder_ptr->uEntropyQP = entropyQP;
}

LOCAL void RvDec_GetPicSize(RV_DECODER_T *rv_decoder_ptr, BitStream *stream, uint32 ptype)
{
	/* If we're decoding a P or B frame, get 1 bit that signals */
	/* difference in size from previous reference frame. */
	if (ptype == INTERPIC
#if defined(BSLICE_SUPPORTED)
		|| ptype == TRUEBPIC
#endif
		)
	{
		/* no diff in size */
		if (RvDec_Read1Bits(stream))
		{
			return;
		}
	}

{
	uint32 code;
	uint32 w, h;

	const uint32 code_width[8] = {160, 176, 240, 320, 352, 640, 704, 0};
	const uint32 code_height1[8] = {120, 132, 144, 240, 288, 480, 0, 0};
	const uint32 code_height2[4] = {180, 360, 576, 0};

	/* width */
	code = RvDec_ReadBits(stream, 3);
	w = code_width[code];
	if (w == 0)
	{
		do
		{
			code = RvDec_ReadBits(stream, 8);
			w += (code << 2);
		}
		while (code == 255);
	}

	/* height */
	code = RvDec_ReadBits(stream, 3);
	h = code_height1[code];
	if (h == 0)
	{
		code <<= 1;
		code |= RvDec_Read1Bits(stream);
		code &= 3;
		h = code_height2[code];
		if (h == 0)
		{
			do
			{
				code = RvDec_ReadBits(stream, 8);
				h += (code << 2);
			}
			while (code == 255);
		}
	}

	rv_decoder_ptr->uOrgFrameWidth  = w;
	rv_decoder_ptr->uOrgFrameHeight = h;
}
}

LOCAL void RvDec_GetMBASize(RV_DECODER_T *rv_decoder_ptr)
{
	/* initialize m_mbaSize */
	uint16 i, j;
	const uint16 MBA_NumMBs[]    = { 47,   98, 395, 1583,  6335, 9215 };
	const uint8 MBA_FieldWidth[] = {  6,    7,   9,   11,    13,   14 };
	const uint16 MBA_Widths =	sizeof( MBA_FieldWidth ) / sizeof( MBA_FieldWidth[0] );

	rv_decoder_ptr->uMbNumX = (rv_decoder_ptr->uOrgFrameWidth + 15)/MB_SIZE;
	rv_decoder_ptr->uMbNumY = (rv_decoder_ptr->uOrgFrameHeight + 15)/MB_SIZE;

    j = rv_decoder_ptr->uMbNumX * rv_decoder_ptr->uMbNumY - 1;
    for (i = 0; i < (MBA_Widths - 1) && MBA_NumMBs[i] <  j; i++)
        ;

    rv_decoder_ptr->uMbASize = MBA_FieldWidth[i];
}

PUBLIC MMDecRet RvDec_GetSliceHeader(RV_DECODER_T *rv_decoder_ptr, BitStream *stream, uint32 isPicHeader)
{
#if defined(RV8_SUPPORTED)
	if(rv_decoder_ptr->bIsRV8)
	{
		RvDec_FlushBits(stream, 3);
	}else
#endif	
	{
		/* ECC */
		if (RvDec_Read1Bits(stream) /*!= 0*/)
		{
		//	PRINTF("WARNING GetSliceHeader_RV9 -ECC packet\n");
		}
	}

	/* PTYPE */
	if(!isPicHeader)
	{
		RvDec_FlushBits(stream, 2);
    }else
    {	
    	rv_decoder_ptr->uPicCodeType = RvDec_ReadBits(stream, 2);
		if(rv_decoder_ptr->uPicCodeType == 2)
		{
			rv_decoder_ptr->uPicCodeType = INTERPIC;
		}else if(rv_decoder_ptr->uPicCodeType == 3)
		{
			rv_decoder_ptr->uPicCodeType = TRUEBPIC;
#if 0
		#if !defined(BSLICE_SUPPORTED)
		#else
			if (!g_Bframe_eb)
		#endif
				return MMDEC_OK;
#endif
		}else
		{
			rv_decoder_ptr->uPicCodeType = INTRAPIC;
		}
	}

#if defined(RV8_SUPPORTED)
	if(rv_decoder_ptr->bIsRV8)
	{
			/* ECC */
		RvDec_Read1Bits(stream);
	}
#endif
	/* PQUANT/SQUANT */
	rv_decoder_ptr->uPQuant = RvDec_ReadBits(stream, 5);
	rv_decoder_ptr->uEntropyQP = rv_decoder_ptr->uPQuant;
    rv_decoder_ptr->prevQuant  = rv_decoder_ptr->uPQuant;

#if defined(RV8_SUPPORTED)
	if(!rv_decoder_ptr->bIsRV8)
#endif		
	{
		/* Bitstream version */
		if (RvDec_Read1Bits(stream) /*!= 0*/)
		{
		//	PRINTF("ERROR GetSliceHeader_RV9 - Unrecognized RV bitstream version\n");
		}

		if(RvDec_Read1Bits(stream))
		{
		//	PRINTF("ERROR GetSliceHeader_RV9 - Interlaced bit set.\n");
		}

		rv_decoder_ptr->OSVQUANT = (uint8)RvDec_ReadBits(stream, 2);
		RvDec_ComputeEntropyQP (rv_decoder_ptr);
	}

	/* In-loop deblocking */
    rv_decoder_ptr->deblocking_filter_passthrough = RvDec_Read1Bits(stream);
	rv_decoder_ptr->uTR = RvDec_ReadBits(stream, 13);

#if defined(RV8_SUPPORTED)
	if(rv_decoder_ptr->bIsRV8)
	{
	//	RvDec_GetPicSizeRv8(rv_decoder_ptr, stream);
		if (rv_decoder_ptr->pctszSize > 0)
		{
			uint32 pctsz = RvDec_ReadBits(stream, rv_decoder_ptr->pctszSize);
			
			rv_decoder_ptr->uOrgFrameWidth = rv_decoder_ptr->Sizes[2*pctsz];
			rv_decoder_ptr->uOrgFrameHeight = rv_decoder_ptr->Sizes[2*pctsz+1];
		}
	}else
#endif	
	{
		RvDec_GetPicSize(rv_decoder_ptr, stream, rv_decoder_ptr->uPicCodeType);
	}

	if(!rv_decoder_ptr->bInitSuceess && isPicHeader)
	{
		RvDec_GetMBASize(rv_decoder_ptr);
	}
	/* MBA */
	RvDec_ReadBits(stream, rv_decoder_ptr->uMbASize);

#if defined(RV8_SUPPORTED)
	if(rv_decoder_ptr->bIsRV8)
	{
		/* RTYPE */
		RvDec_Read1Bits(stream);
	}
#endif

	return MMDEC_OK;
}

//#define FIELDLEN_SSC 24			/* Slice start code */
//#define FIELDLEN_SSC_RV8 17		/* Slice start code */
//const uint32 FIELDVAL_SSC		= (0x1d1c105e >> (32 - FIELDLEN_SSC));
//const uint32 FIELDVAL_SSC_RV8	= (0x00008000 >> (32 - FIELDLEN_SSC_RV8));
//#define MAX_SSC_LOOKAHEAD_NUMBER  7

#if defined(RV8_SUPPORTED)
PUBLIC BOOLEAN RvDec_GetSSC_RV8(RV_DECODER_T *rv_decoder_ptr, BitStream *stream)
{
	uint32 w, n;
	uint32 tmp;

	w = RvDec_ShowBits(stream, 24);

	for (n = 0; n < 8; n++)
	{
		tmp = (w >> 7) & 0x0001ffff;

		if (1 == tmp)
		{
			RvDec_FlushBits(stream, 17+n);   //flush the slice start code and the stuffing bits
			return TRUE;
		}else
		{
			if (1 == (w>>23))	break;
			else w <<= 1;
		}
	}
		
	return FALSE;
}
#endif

PUBLIC BOOLEAN RvDec_GetSSC_RV9(RV_DECODER_T *rv_decoder_ptr, BitStream *stream)
{
	if(rv_decoder_ptr->iSkipModesLeft > 1)
	{
		return FALSE;  
	}else
	{
		uint32 i;
		uint32 deltaofs;

		/* locate current slice, i - 1 */
		i = rv_decoder_ptr->uSilceNumber + 1;

		/* check if just beginning to decode the current slice */
		if (stream->bitcnt == (rv_decoder_ptr->slice_offset[i-1])*8)
			return TRUE;

		/* there is no more slice header in the bitstream */
		if (i == rv_decoder_ptr->total_slice_num)
			return FALSE;

		/* skip stuffing if any */
		deltaofs = (rv_decoder_ptr->slice_offset[i])*8 - stream->bitcnt;
		if (deltaofs < 8)
		{
			uint32 w;

			w = RvDec_ShowBits(stream, deltaofs);

			if (!w /*== 0*/)
			{
				RvDec_FlushBits(stream, deltaofs);
				return TRUE;
			}else
			{
				return FALSE;
			}
		}else
		{
			return FALSE;
		}
	}
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
