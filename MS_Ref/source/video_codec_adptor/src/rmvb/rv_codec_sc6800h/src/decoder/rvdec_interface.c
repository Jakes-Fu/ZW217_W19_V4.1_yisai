
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

PUBLIC MMDecRet RvDecInit(MMCodecBuffer *buffer_ptr, MMDecVideoFormat *pVideoFormat)
{
	RV_DECODER_T *rv_decoder_ptr;

	RvDec_InitInterMem (buffer_ptr);

	SCI_ASSERT(NULL != (g_rv_decoder_ptr = (RV_DECODER_T *)RvDec_InterMemAlloc(sizeof(RV_DECODER_T))));/*assert verified*/
	rv_decoder_ptr = g_rv_decoder_ptr;
	rv_decoder_ptr->bIsRV8 = (pVideoFormat->video_std == RV_8);	
	rv_decoder_ptr->frame_rate = pVideoFormat->frame_rate;
	
	rv_decoder_ptr->stream = (BitStream *)RvDec_InterMemAlloc(sizeof(BitStream));	
	SCI_ASSERT(NULL != rv_decoder_ptr->stream);/*assert verified*/

	RvDec_InitDecoderPara(rv_decoder_ptr);
#if defined(SC6800H_VIDEO_ROMCODE)
	RvDec_init_ROM_TABLE();
#endif

#if defined(RV8_SUPPORTED)
	if(rv_decoder_ptr->bIsRV8)
	{
		rv_decoder_ptr->uMcFactor = 3;
		rv_decoder_ptr->DecIPredMode4x4 = RvDec_DecIPMode4x4_RV8;
		g_RvDec_ReadMbtype_PB = RvDec_ReadMbtype_PB_RV8;
		g_RvDec_Get_Slice_StartCode = RvDec_GetSSC_RV8;
#if defined(BSLICE_SUPPORTED)
		g_DecMV_B = RvDec_DecMV_B_RV8;
#endif
#if defined(DBK_SUPPORTED)
		g_RvDec_Set_Dbk_coeff = set_deblock_coeff_r8;
#endif
	}else
#endif	
	{
		rv_decoder_ptr->uMcFactor_d = 8;
		rv_decoder_ptr->uMcFactor = 4;

		rv_decoder_ptr->DecIPredMode4x4 = RvDec_DecIPMode4x4_RV9;
		g_RvDec_ReadMbtype_PB = RvDec_ReadMbtype_PB_RV9;
		g_RvDec_Get_Slice_StartCode = RvDec_GetSSC_RV9;
#if defined(BSLICE_SUPPORTED)
		g_DecMV_B = RvDec_DecMV_B_RV9;
#endif
#if defined(DBK_SUPPORTED)
 		g_RvDec_Set_Dbk_coeff = set_deblock_coeff_r9;
#endif
	}

	RvDec_SetClipTab();
	RvDec_InitVldTable();
	RvDec_InitMCFun(rv_decoder_ptr->bIsRV8);
	RvDec_InitIPredFun();

	if (rv_decoder_ptr->error_flag)
	{
		return MMDEC_ERROR;
	}
	
	return MMDEC_OK;
}

PUBLIC MMDecRet RvDecDecode(MMDecInput *dec_input_ptr, MMDecOutput *dec_output_ptr)
{
 	MMDecRet ret;
	RV_DECODER_T *rv_decoder_ptr = g_rv_decoder_ptr;
	BitStream *stream = rv_decoder_ptr->stream;
	
	rv_decoder_ptr->need_downsize  = dec_input_ptr->need_downsize;
	rv_decoder_ptr->total_slice_num = dec_input_ptr->total_slice_num;

	/* in the case where segment information is not available, assume each */
	/* segment given to the is properly formatted (with header and decodable */
	/* data, or 4 bytes of 0's which will be caught by the decoder later) */
	if (!rv_decoder_ptr->total_slice_num || (rv_decoder_ptr->total_slice_num > MAX_SLICE_NUM))
	{
		return MMDEC_ERROR;
	}
	memcpy(rv_decoder_ptr->slice_offset, dec_input_ptr->slice_offset, rv_decoder_ptr->total_slice_num*sizeof(int32));
	memcpy(rv_decoder_ptr->slice_valid, dec_input_ptr->slice_valid, rv_decoder_ptr->total_slice_num*sizeof(uint8));

	RvDec_InitBitstream(stream, dec_input_ptr->pStream, dec_input_ptr->dataLen);

	ret = RvDec_GetSliceHeader(rv_decoder_ptr, stream, 1);

	if(dec_input_ptr->expected_IVOP && (rv_decoder_ptr->uPicCodeType != INTRAPIC))
	{
        if (g_nFrame_rv)
	    {
			g_nFrame_rv++;
	    }
		return MMDEC_FRAME_SEEK_IVOP;
	}

	dec_output_ptr->frameEffective = FALSE;
	dec_output_ptr->pOutFrameY = PNULL;
	dec_output_ptr->pOutFrameU = PNULL;
	dec_output_ptr->pOutFrameV = PNULL;

#if defined(BSLICE_SUPPORTED)
#if 0
    if (TRUEBPIC != rv_decoder_ptr->uPicCodeType)
	{
		g_nBFrame = 0;
	}else
	{
		g_nBFrame++;

		if ((g_nBFrame & 0x1) || !g_Bframe_eb)
		{
 		//	PRINTF("\n");
			return MMDEC_OK;
		}
	}
#endif	
#else
	if(TRUEBPIC == rv_decoder_ptr->uPicCodeType)
    {
	//	PRINTF("\n");
        return MMDEC_OK;
    }
#endif

	if (!rv_decoder_ptr->bInitSuceess)
	{
		if( MMDEC_OK != RvDec_InitSessionDecode(rv_decoder_ptr) )
        {
            return MMDEC_MEMORY_ERROR;
        }
		rv_decoder_ptr->bInitSuceess = 1;
	}

	if(!RvDec_GetCurRecFrameBfr(rv_decoder_ptr))
	{
		return MMDEC_OUTPUT_BUFFER_OVERFLOW;
	}		

	RvDec_InitPicture(rv_decoder_ptr);

	if(INTRAPIC == rv_decoder_ptr->uPicCodeType)
	{
	//	PRINTF("\tI Frame.\n");
		ret = RvDec_DecoderFrameI(rv_decoder_ptr);
	}
	else if(INTERPIC == rv_decoder_ptr->uPicCodeType)
	{		
	//	PRINTF("\tP Frame.\n");
		if (rv_decoder_ptr->need_downsize)
		{
			ret = RvDec_DecoderFrameP_d(rv_decoder_ptr);
		}else
		{
			ret = RvDec_DecoderFrameP(rv_decoder_ptr);
		}
	}else 
	{
	//	PRINTF("\tB Frame.\n");
#if defined(BSLICE_SUPPORTED)
 	 	ret = RvDec_DecoderFrameB (rv_decoder_ptr);	
#else
		return MMDEC_OK;
#endif
	}	
	
	if(MMDEC_OK == ret)
	{	
		STORABLE_PICTURE_T *pic = PNULL;

		/*output frame for display*/
		if(rv_decoder_ptr->uPicCodeType != TRUEBPIC)
		{
			if(g_nFrame_rv > 0)
			{
				/*send backward reference (the lastest reference frame) frame's YUV to display*/
				pic = rv_decoder_ptr->pBckRefFrame;
			}		
		}else
		{
			/*send current B frame's YUV to display*/
			pic = rv_decoder_ptr->pCurRecFrame;
		}

		if(pic != PNULL)
		{
			dec_output_ptr->pOutFrameY = pic->pDecFrame->imgYUV[0];
			dec_output_ptr->pOutFrameU = pic->pDecFrame->imgYUV[1];
			dec_output_ptr->pOutFrameV = pic->pDecFrame->imgYUV[2];
			if (pic->is_downsized)
			{
				dec_output_ptr->frame_width = rv_decoder_ptr->uFrameWidth_d;
				dec_output_ptr->frame_height = rv_decoder_ptr->uFrameHeight_d;
			}else
			{
				dec_output_ptr->frame_width = rv_decoder_ptr->uFrameWidth;
				dec_output_ptr->frame_height = rv_decoder_ptr->uFrameHeight;
			}
			dec_output_ptr->is_downsized = pic->is_downsized;
			dec_output_ptr->frameEffective = 1;
			pic->pDecFrame->bDisp = TRUE; 
		}else
		{
			dec_output_ptr->frameEffective = 0;
		}
		
		RvDec_exit_picture(rv_decoder_ptr);
	}

	g_nFrame_rv++;
	
	if (!rv_decoder_ptr->error_flag)
	{
		return MMDEC_OK;
	}else
	{
		return MMDEC_ERROR;
	}
}

PUBLIC MMDecRet RvDecRelease(void)
{
	RvDec_FreeMem();

	return MMDEC_OK;
}

PUBLIC MMDecRet RvDecGetOneDispFrm(MMDecOutput *pOutput, int req_transposed, int is_last_frame)
{
	return MMDEC_OK;
}

PUBLIC MMDecRet RvDecReleaseDispBfr(uint8 *pFrameY)
{
	return MMDEC_OK;
}

PUBLIC int32 RvDec_GetPictureType(uint8 first_byte)
{
	uint32 shift = 5;
	const int32 type_tbl[4] = {0, 0, 1, 2};

#if defined(RV8_SUPPORTED)
	if(g_rv_decoder_ptr->bIsRV8)
	{
		shift = 3;
	}
#endif	
		
	/* PTYPE */
	return type_tbl[(first_byte >> shift) & 0x3];
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
