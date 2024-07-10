/************************************************************************/
/* Common                                                               */
/************************************************************************/
#include"JPG_Codec.h"
////
void AllocMCUBuf()
{
	uint16 block_id, block_num;
	uint8 component_id, i;
	
	block_id = 0;
	for (component_id = 0; component_id < 3; component_id++)
	{
		block_num = g_JpegCodec.ratio[component_id].h_ratio * g_JpegCodec.ratio[component_id].v_ratio;
		for (i=0; i<block_num; i++)
		{
			g_JpegCodec.blocks[block_id] = g_mcu_buf + block_id * 64;
			g_JpegCodec.org_blocks[block_id] = g_mcu_org_buf +block_id * 64;
			g_JpegCodec.blocks_membership[block_id] = component_id;
			block_id++;
		}
	}
}

/************************************************************************/
/* For DCT                                                              */
/************************************************************************/
void JPEG_WritePixSrc_To_DCTIOBuffer(uint16 block_id)
{
	int16 i;
	uint8 *pSrcCoeff;
	int16 *pDst;
//	int32 *pIntSrc, *pIntDst;
//	DCT_IO_BUFFER_T *pDct_io_bfr = g_pDct_bfr;
//	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)g_vsp_pBase_bfr->pDct_ctrl_reg_base;
#ifdef _VSP_
	VSP_Write_SrcPix_DCTIOBUF_CFG(DCT_INPUT_BUF_BLOCK, DCT_BIG_ENDIAN, DCT_BIG_ENDIAN);
#endif
		pSrcCoeff = g_JpegCodec.org_blocks[block_id];
#ifdef _VSP_
		pDst = g_pDct_bfr->pBlk[0];

		memcpy((void*)pDst,(void*)pSrcCoeff,64);
/*
		pIntSrc = (int32 *)pSrcCoeff;
		pIntDst = (int32 *)pDst;
		for (i = 0; i < 2; i++)
		{
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
		}
*/		
#else
		pDst = g_pDct_bfr->pBlk[block_id];
		for(i = 0; i < 8; i++)
		{
			*pDst++ = *pSrcCoeff++;
			*pDst++ = *pSrcCoeff++;
			*pDst++ = *pSrcCoeff++;
			*pDst++ = *pSrcCoeff++;
			
			*pDst++ = *pSrcCoeff++;
			*pDst++ = *pSrcCoeff++;
			*pDst++ = *pSrcCoeff++;
			*pDst++ = *pSrcCoeff++;
		}
#endif
}
void JPEG_ReadCoeff_From_DCTIOBuffer(uint16 block_id)
{
	int16 i;
	int16 *pDctCoeff, *pSrc;
//	int32 *pIntSrc, *pIntDst;
//	DCT_IO_BUFFER_T *pDct_io_bfr = g_pDct_bfr;
//	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)g_vsp_pBase_bfr->pDct_ctrl_reg_base;
#ifdef _VSP_
	VSP_WAIT_DCT_DONE
	VSP_Read_DCTIOBUF_DCT_CFG(DCT_BYPASS_MODE);
	START_DCT_ENGINE_OUT
	VSP_WAIT_DCT_OUTBUF_READY
#endif
//	for(block_id = 0; block_id < block_num; block_id++)
	{
		pDctCoeff = g_JpegCodec.blocks[block_id];
#ifdef _VSP_
		pSrc = g_pDct_bfr->pBlk[0];
#else
		pSrc = g_pDct_bfr->pBlk[block_id];
#endif
		memcpy((void*)pDctCoeff,(void*)pSrc,128);
/*
		pIntSrc = (int32 *)pSrc;
		pIntDst = (int32 *)pDctCoeff;
		for (i = 0; i < 4; i++)
		{
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
			*pIntDst++ = *pIntSrc++;
		}		
*/		
	}
}
/************************************************************************/
/* For IDCT                                                             */
/************************************************************************/
void JPEG_WriteCoeff_To_DCTIOBuffer(uint16 block_id)
{
	int16 i;
	int16 *pIDCTCoeff = g_JpegCodec.blocks[block_id];
	DCT_IO_BUFFER_T *pDct_io_bfr = g_pDct_bfr;
#ifdef _VSP_
	int16 * pDst = pDct_io_bfr->pBlk[0];
#else 
	int16 * pDst = pDct_io_bfr->pBlk[block_id];
#endif	
//	int32 * pIntSrc = (int32 *)pIDCTCoeff;
//	int32 * pIntDst = (int32 *)pDst;
	
//	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)g_vsp_pBase_bfr->pDct_ctrl_reg_base;
#ifdef _VSP_
	VSP_Write_COF_DCTIOBUF_CFG(DCT_BYPASS_MODE, DCT_INPUT_BUF_BLOCK, DCT_LITTLE_ENDIAN, DCT_BIG_ENDIAN);
#endif

	memcpy((void*)pDst,(void*)pIDCTCoeff,128);
/*
	for (i = 0; i < 4; i++)
	{
		*pIntDst++ = *pIntSrc++;
		*pIntDst++ = *pIntSrc++;
		*pIntDst++ = *pIntSrc++;
		*pIntDst++ = *pIntSrc++;
		
		*pIntDst++ = *pIntSrc++;
		*pIntDst++ = *pIntSrc++;
		*pIntDst++ = *pIntSrc++;
		*pIntDst++ = *pIntSrc++;
	}		
	*/
}

void JPEG_ReadPixSrc_From_DCTIOBuffer(uint16 block_id)
{
	int16 i;
	uint8 * pDst;
	int32 * pSrc , twoPix;
	DCT_IO_BUFFER_T *pDct_io_bfr = g_pDct_bfr;	
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)g_vsp_pBase_bfr->pDct_ctrl_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)g_vsp_pBase_bfr->pGlobal_reg_base;
#ifdef _VSP_	
	/*while(pDct_ctrl_reg->dct_int_raw != 1);
	pDct_ctrl_reg->dct_int_clr = 1;*/
	VSP_WAIT_DCT_DONE
	VSP_Read_DCTIOBUF_IDCT_CFG();
	START_DCT_ENGINE_OUT
	VSP_WAIT_DCT_OUTBUF_READY
#endif
//	for(block_id = 0 ; block_id < block_num; block_id++)
	{
		pDst = g_JpegCodec.org_blocks[block_id];
#ifdef _VSP_
		pSrc = (int32*)(g_pDct_bfr->pBlk[0]);
#else
		pSrc = (int32*)(g_pDct_bfr->pBlk[block_id]);
#endif
		for(i = 0; i < 8; i++)
		{

#ifdef	BIG_ENDIAN 
			twoPix = *pSrc++;
			pDst[0] = pClip_table[(twoPix >> 16)];
			pDst[1] = pClip_table[((twoPix<<16)>>16)];
			
			twoPix = *pSrc++;
			pDst[2] = pClip_table[(twoPix >> 16)];
			pDst[3] = pClip_table[((twoPix<<16)>>16)];
			
			twoPix = *pSrc++;
			pDst[4] = pClip_table[(twoPix >> 16)];
			pDst[5] = pClip_table[((twoPix<<16)>>16)];

			twoPix = *pSrc++;
			pDst[6] = pClip_table[(twoPix >> 16)];
			pDst[7] = pClip_table[((twoPix<<16)>>16)];
#else
			twoPix = *pSrc++;
			pDst[0] = pClip_table[((twoPix<<16)>>16)];
			pDst[1] = pClip_table[(twoPix >> 16)];

			twoPix = *pSrc++;
			pDst[2] = pClip_table[((twoPix<<16)>>16)];
			pDst[3] = pClip_table[(twoPix >> 16)];

			twoPix = *pSrc++;
			pDst[4] = pClip_table[((twoPix<<16)>>16)];
			pDst[5] = pClip_table[(twoPix >> 16)];

			twoPix = *pSrc++;
			pDst[6] = pClip_table[((twoPix<<16)>>16)];
			pDst[7] = pClip_table[(twoPix >> 16)];
#endif
			pDst += 8;
		}
	}
}
