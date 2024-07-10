
#include "ms_ref_dc_trc.h"
#include "JPG_Codec.h"
#include "os_api.h"

#define COEFF_INDEX(i)  (zigzag_order[i])

static void	InPutRstMarker(void)
{
	JPEG_FLUSH(0xFFFFFFFF);
	JPEG_GETC();
	JPEG_GETC();
}

/************************************************************************/
/*     Support Different YUV Format:	YUV420	YUV444  YUV422			*/
/*     Added by wangyi                                                  */
/************************************************************************/
void CopyMCUToCoeff444(uint8 *y_coeff, uint8 *u_coeff, uint8 *v_coeff, uint16 x, uint16 y)
{
	uint16 i;
	uint32 width;
	uint8   * srcdata, * u_srcdata, * v_srcdata;
	uint8   * destdata, * u_destdata, * v_destdata;
	uint32  * pIntDst;
	uint32  * pIntSrc; 
	uint16 x_offset, y_offset;
	/*output one Y block 8x8*/
	{
		width = g_JpegCodec.c_width;
		x_offset = x * 8;
		y_offset = y * 8;
		destdata = y_coeff + width * y_offset + x_offset;
		srcdata = g_JpegCodec.org_blocks[0];
		for (i = 0; i < 8; i++)
		{
			pIntDst = (uint32 *) destdata;
			pIntSrc = (uint32 *) srcdata;
			pIntDst[0] = pIntSrc[0];
			pIntDst[1] = pIntSrc[1];
			destdata += width;
			srcdata += 8;
		}
	}
	
	/*output 4x4 U, V block*/
	{
		width = g_JpegCodec.c_width >> 1;
		x_offset = x * 4;
		y_offset = y * 4;
		u_destdata = u_coeff + width * y_offset + x_offset;
		v_destdata = v_coeff + width * y_offset + x_offset;
		u_srcdata = g_JpegCodec.org_blocks[1];
		v_srcdata = g_JpegCodec.org_blocks[2];
		for (i = 0; i < 4; i++)
		{
			u_destdata[0] = u_srcdata[0];			v_destdata[0] = v_srcdata[0];
			u_destdata[1] = u_srcdata[2];			v_destdata[1] = v_srcdata[2];
			u_destdata[2] = u_srcdata[4];			v_destdata[2] = v_srcdata[4];
			u_destdata[3] = u_srcdata[6];			v_destdata[3] = v_srcdata[6];
			u_destdata += width;					v_destdata += width;
			u_srcdata += 16;						v_srcdata += 16;
		}
	}

}

void CopyMCUToCoeff422(uint8 *y_coeff, uint8 *u_coeff, uint8 *v_coeff, uint16 x, uint16 y)
{
	uint16  block_id, i;
	uint32 width;
	uint8   * srcdata, * u_srcdata, * v_srcdata;
	uint8   * destdata, * u_destdata, * v_destdata;
	uint32  * pIntDst, * u_pIntDst, * v_pIntDst;
	uint32  * pIntSrc, * u_pIntSrc, * v_pIntSrc; 
	uint16 x_offset, y_offset;
	/*output two Y block 8x8*/
	for (block_id = 0; block_id < 2; block_id++)
	{
		width = g_JpegCodec.c_width;
		x_offset = x * 16 + block_id * 8;
		y_offset = y * 8;
		destdata = y_coeff + width * y_offset + x_offset;
		srcdata = g_JpegCodec.org_blocks[block_id];
		for (i = 0; i < 8; i++)
		{
			pIntSrc = (uint32 *) srcdata;
			pIntDst = (uint32 *) destdata;
			pIntDst[0] = pIntSrc[0];
			pIntDst[1] = pIntSrc[1];
			destdata += width;
			srcdata += 8;
		}
	}

	/*output 8x4 U block*/
	{
		width = g_JpegCodec.c_width >> 1;
		x_offset = x * 8;
		y_offset = y * 4;
		u_destdata = u_coeff + width * y_offset + x_offset;
		v_destdata = v_coeff + width * y_offset +x_offset;
		u_srcdata = g_JpegCodec.org_blocks[block_id++];
		v_srcdata = g_JpegCodec.org_blocks[block_id];
		for (i = 0; i < 4; i++)
		{
			u_pIntSrc = (uint32 *) u_srcdata;		v_pIntSrc = (uint32 *) v_srcdata;
			u_pIntDst = (uint32 *) u_destdata;		v_pIntDst = (uint32 *) v_destdata;
			u_pIntDst[0] = u_pIntSrc[0];			v_pIntDst[0] = v_pIntSrc[0];
			u_pIntDst[1] = u_pIntSrc[1];			v_pIntDst[1] = v_pIntSrc[1];
			u_destdata += width;					v_destdata += width;
			u_srcdata += 16;						v_srcdata += 16;
		}
	}
}

void CopyMCUToCoeff420(uint8 *y_coeff, uint8 *u_coeff, uint8 *v_coeff, uint16 x, uint16 y)
{
	uint16  block_id, i;
	uint32  width;
	uint8   * srcdata, * u_srcdata, * v_srcdata;
	uint8   * destdata, * u_destdata, * v_destdata;
	uint32  * pIntDst, * u_pIntDst, * v_pIntDst;
	uint32  * pIntSrc, * u_pIntSrc, * v_pIntSrc; 
	uint16 x_offset, y_offset;
	/*output four Y block 8x8*/
	for (block_id = 0; block_id < 4; block_id++)
	{
		width = g_JpegCodec.c_width;
		x_offset = x * 16 + (block_id % 2) * 8;
		y_offset = y * 16 + (block_id >> 1) * 8;
		destdata = y_coeff + width * y_offset + x_offset;
		srcdata = g_JpegCodec.org_blocks[block_id];
		for (i = 0; i < 8; i++)
		{
			pIntSrc = (uint32 *) srcdata;
			pIntDst = (uint32 *) destdata;
			pIntDst[0] = pIntSrc[0];
			pIntDst[1] = pIntSrc[1];
			destdata += width;
			srcdata += 8;
		}	
	}
	//output one 8*8 U block and one 8*8 V block
	{
		width = g_JpegCodec.c_width >> 1;
		x_offset = x * 8;
		y_offset = y * 8;
		u_destdata = u_coeff + width * y_offset + x_offset;
		v_destdata = v_coeff + width * y_offset +x_offset;
		u_srcdata = g_JpegCodec.org_blocks[block_id++];
		v_srcdata = g_JpegCodec.org_blocks[block_id];
		for (i = 0; i < 8; i++)
		{
			u_pIntSrc = (uint32 *) u_srcdata;		v_pIntSrc = (uint32 *) v_srcdata;
			u_pIntDst = (uint32 *) u_destdata;		v_pIntDst = (uint32 *) v_destdata;
			u_pIntDst[0] = u_pIntSrc[0];			v_pIntDst[0] = v_pIntSrc[0];
			u_pIntDst[1] = u_pIntSrc[1];			v_pIntDst[1] = v_pIntSrc[1];
			u_destdata += width;					v_destdata += width;
			u_srcdata += 8;							v_srcdata += 8;
		}
	}
}

void CopyMCUToCoeff411(uint8 *y_coeff, uint8 *u_coeff, uint8 *v_coeff, uint16 x, uint16 y)
{
	uint16  block_id, i;
	uint32 width;
	uint8 *srcdata;
	uint8  *destdata;
	uint32 *pIntDst;
	uint32 *pIntSrc;
	uint16 x_offset, y_offset;
	/*output four Y block 8x8*/
	for (block_id = 0; block_id < 4; block_id++)
	{
		width = g_JpegCodec.c_width;
		x_offset = x * 16 + block_id * 8;
		y_offset = y * 16;
		destdata = y_coeff + width * y_offset + x_offset;
		srcdata = g_JpegCodec.org_blocks[block_id];
		for (i = 0; i < 8; i++)
		{
			pIntSrc = (uint32 *) srcdata;
			pIntDst = (uint32 *) destdata;
			pIntDst[0] = pIntSrc[0];
			pIntDst[1] = pIntSrc[1];
			destdata += width;
			srcdata += 8;
		}	
	}
	//output one 16*4 U block
	{
		width = g_JpegCodec.c_width >> 1;
		x_offset = x * 16;
		y_offset = y * 4;
		destdata = u_coeff + width * y_offset + x_offset;
		srcdata = g_JpegCodec.org_blocks[block_id++];
		for (i = 0; i < 4; i++)
		{
			pIntSrc = (uint32 *) srcdata;
			pIntDst = (uint32 *) destdata;
			pIntDst[0] = pIntSrc[0];
			pIntDst[1] = pIntSrc[1];
			pIntDst[2] = pIntSrc[0];
			pIntDst[3] = pIntSrc[1];
			destdata += width;
			srcdata += 16;
		}
	}
	//output one 16*4 V block
	{
//		width = g_JpegCodec.c_width >> 1;
		x_offset = x * 16;
		y_offset = y * 4;
		destdata = v_coeff + width * y_offset + x_offset;
		srcdata = g_JpegCodec.org_blocks[block_id++];
		for (i = 0; i < 4; i++)
		{
			pIntSrc = (uint32 *) srcdata;
			pIntDst = (uint32 *) destdata;
			pIntDst[0] = pIntSrc[0];
			pIntDst[1] = pIntSrc[1];
			pIntDst[2] = pIntSrc[0];
			pIntDst[3] = pIntSrc[1];
			destdata += width;
			srcdata += 16;
		}
	}
}
/************************************************************************/
/* End of the COPY MCU TO Coeff                                         */
/************************************************************************/



uint32 JPEG_Decode(
	uint32  y_coeff,
	uint32  u_coeff,
	uint32  v_coeff
)
{
	uint16 x, y;
	uint8 format;
	JPEG_CallBack CopyMCUToCoeff;
	format= (g_JpegCodec.ratio[0].h_ratio << 4) | (g_JpegCodec.ratio[0].v_ratio);

	//SCI_TRACE_LOW:"JPEG_Decode,g_JpegCodec.ratio[0].h_ratio = %d, g_JpegCodec.ratio[0].v_ratio = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPG_DECODE_245_112_2_18_1_23_50_293,(uint8*)"dd",g_JpegCodec.ratio[0].h_ratio,g_JpegCodec.ratio[0].v_ratio);
	switch(format) 
	{
	case 0x11:
		g_JpegCodec.block_num = 3;
		CopyMCUToCoeff = &CopyMCUToCoeff444;
		break;
	case 0x21:
		g_JpegCodec.block_num = 4;
		CopyMCUToCoeff = &CopyMCUToCoeff422;
		break;
	case 0x41:
		g_JpegCodec.block_num = 6;
		CopyMCUToCoeff = &CopyMCUToCoeff411;
		break;
	case 0x22:
		g_JpegCodec.block_num = 6;
		CopyMCUToCoeff = &CopyMCUToCoeff420;
		break;
	default:
		//			JPEG_ERROR(JPEG_EID_SAMPLEFORMAT, "format = %d\n", sample_format);
		return JPEG_FAILED;
	}
	/*for every MCU do following*/
	for (y=0; y<g_JpegCodec.mcu_num_y; y++)
		for (x = 0; x<g_JpegCodec.mcu_num_x; x++)
	{

		/*restart interval*/
		if (g_JpegCodec.restart_interval)
		{
			if (g_JpegCodec.restart_to_go == 0)
			{
				InPutRstMarker();
				g_JpegCodec.next_restart_num += 1;
				g_JpegCodec.next_restart_num &= 0x07;
				g_JpegCodec.restart_to_go = g_JpegCodec.restart_interval;
				g_JpegCodec.pre_dc_value[0] = 0;
				g_JpegCodec.pre_dc_value[1] = 0;
				g_JpegCodec.pre_dc_value[2] = 0;
			}
			g_JpegCodec.restart_to_go--;
		}

		/*1, decode MCU*/
		JPEG_DecodeMCU();
		/*2, dequant MCU*/
		/*dequant is done in huffman decode*/
		//JPEG_DequantMCU();
		/*3, copy MCU data to coeff buffer*/
		/**Added by wangyi 2007/05/02**/
		CopyMCUToCoeff((uint8*)y_coeff, (uint8*)u_coeff, (uint8*)v_coeff, x, y);
		if (g_JpegCodec.error_id)
		{
			return JPEG_FAILED;
		}
	}
	JPEG_FLUSH(0);

	return JPEG_SUCCESS;
}

#ifdef _VSP_
void JPEG_HardIDCT(uint16 block_num)
{
	int32 cbp;
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)g_vsp_pBase_bfr->pDct_ctrl_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)g_vsp_pBase_bfr->pGlobal_reg_base;
	cbp = (1 << block_num) - 1;
	
	/*configure dct core register*/
	/*pDct_ctrl_reg->dct_config = (VSP_IDCT	<< DCT_WORK_MODE_BIT)		|		//DCT WORK MODE
	BYPASS_MODE		<< DCT_SCAN_MODE_BIT		|
	PIXEL_9BIT		<< DCT_PIXEL_8BIT_EN_BIT	|		//coefficient is 12 bits
	// 1 << DCT_PIXEL_8BIT_PACKET_BIT |
	1 << DCT_SIGN_EN_BIT |
	NOT_INVERSE << 6						|
	cbp				<< DCT_BLOCK_MASK_BIT		|
	// 0 << DCT_IN_BUF_ARRANGE_BIT |
	DCT_PRCT_DISABLE	<< DCT_PROTECT_EN_BIT		|		//PROTECT ENABLE
	DCT_LITTLE_ENDIAN	<< DCT_IN_ENDIAN_BIT		|
	DCT_BIG_ENDIAN	<< DCT_OUT_ENDIAN_BIT;*/
	VSP_ExternalData_IDCT_CFG(cbp,DCT_LITTLE_ENDIAN,DCT_BIG_ENDIAN);
	
	VSP_Disable_Quan();
	
	START_DCT_ENGINE
}
#endif

void JPEG_DecodeMCU(void)
{
	int16 *block;
	uint16 block_id, ci;
	HUFF_TBL_T *dc_tbl, *ac_tbl;
	uint16 *quant;
	int16 temp;
	uint8 *rgiDst;
	g_block_num = 0;
	for(block_id = 0; block_id < g_JpegCodec.block_num; block_id++)
	{
		block = g_JpegCodec.blocks[block_id];
		rgiDst = g_JpegCodec.org_blocks[block_id];
		/*check if at the end of blocks*/
#if 1
		{
			if(g_block_num == (/*g_JpegCodec.mcu_num_x * g_JpegCodec.mcu_num_y * */g_JpegCodec.block_num) - 1)
			{
				//First do IDCT
				JPEG_HardIDCT(1);
				//Meanwhile do the VLD
				ci = g_JpegCodec.blocks_membership[block_id];
				dc_tbl = &(g_JpegCodec.dc_huff_tbl[g_JpegCodec.tbl_map[ci].dc_huff_tbl_id]);
				ac_tbl = &(g_JpegCodec.ac_huff_tbl[g_JpegCodec.tbl_map[ci].ac_huff_tbl_id]);
				quant = g_JpegCodec.quant_tbl_new[g_JpegCodec.tbl_map[ci].quant_tbl_id];
				
				/*do DECODE one block*/
				Vld_DQuant_Block(block, dc_tbl, ac_tbl, quant);
				
				/*update pre_dc_val*/
				temp = block[0] + g_JpegCodec.pre_dc_value[ci];
				
				g_JpegCodec.pre_dc_value[ci] = temp;
				block[0] = quant[0] * temp + 1024;
				
				//judge if it is the proper time to read the IDCT coefficient from the IO BUFFER
				JPEG_ReadPixSrc_From_DCTIOBuffer(g_oldblock_id);
				
				//After read , then write the new VLD code into the IO BUFFER
				JPEG_WriteCoeff_To_DCTIOBuffer(block_id);
				
				JPEG_HardIDCT(1);
				
				JPEG_ReadPixSrc_From_DCTIOBuffer(block_id);
			}
			else if(g_block_num != 0)
			{
				//First do IDCT
				JPEG_HardIDCT(1);
				//Meanwhile do the VLD
				ci = g_JpegCodec.blocks_membership[block_id];
				dc_tbl = &(g_JpegCodec.dc_huff_tbl[g_JpegCodec.tbl_map[ci].dc_huff_tbl_id]);
				ac_tbl = &(g_JpegCodec.ac_huff_tbl[g_JpegCodec.tbl_map[ci].ac_huff_tbl_id]);
				quant = g_JpegCodec.quant_tbl_new[g_JpegCodec.tbl_map[ci].quant_tbl_id];
				
				/* DECODE one block*/
				Vld_DQuant_Block(block, dc_tbl, ac_tbl, quant);
				
				/*update pre_dc_val*/
				temp = block[0] + g_JpegCodec.pre_dc_value[ci];
				
				g_JpegCodec.pre_dc_value[ci] = temp;
				block[0] = quant[0] * temp + 1024;
				
				//judge if it is the proper time to read the IDCT coefficient from the IO BUFFER
				JPEG_ReadPixSrc_From_DCTIOBuffer(g_oldblock_id);
				
				g_oldblock_id = block_id;
									
			}
			else
			{
				ci = g_JpegCodec.blocks_membership[block_id];
				dc_tbl = &(g_JpegCodec.dc_huff_tbl[g_JpegCodec.tbl_map[ci].dc_huff_tbl_id]);
				ac_tbl = &(g_JpegCodec.ac_huff_tbl[g_JpegCodec.tbl_map[ci].ac_huff_tbl_id]);
				quant = g_JpegCodec.quant_tbl_new[g_JpegCodec.tbl_map[ci].quant_tbl_id];
				
				/* decode one block*/
				Vld_DQuant_Block(block, dc_tbl, ac_tbl, quant);
				
				/*update pre_dc_val*/
				temp = block[0] + g_JpegCodec.pre_dc_value[ci];
				
				g_JpegCodec.pre_dc_value[ci] = temp;
				block[0] = quant[0] * temp + 1024;
				
				g_oldblock_id = block_id;
			}
			//After read , then write the new VLD code into the IO BUFFER
			JPEG_WriteCoeff_To_DCTIOBuffer(block_id);
			g_block_num++;
		}
		
#else

		{	/*select huffman table*/
			ci = g_JpegCodec.blocks_membership[block_id];
			dc_tbl = &(g_JpegCodec.dc_huff_tbl[g_JpegCodec.tbl_map[ci].dc_huff_tbl_id]);
			ac_tbl = &(g_JpegCodec.ac_huff_tbl[g_JpegCodec.tbl_map[ci].ac_huff_tbl_id]);
			quant = g_JpegCodec.quant_tbl_new[g_JpegCodec.tbl_map[ci].quant_tbl_id];
			
			/*do encode one block*/
			Vld_DQuant_Block(block, dc_tbl, ac_tbl, quant);
			
			/*update pre_dc_val*/
			temp = block[0] + g_JpegCodec.pre_dc_value[ci];
			
			g_JpegCodec.pre_dc_value[ci] = temp;
			block[0] = quant[0] * temp + 1024;

			JPEG_WriteCoeff_To_DCTIOBuffer(block_id);

#ifdef _VSP_
			JPEG_HardIDCT(1);
#else
	//		rgiDst = g_pDct_bfr->pBlk[block_id];
			block = g_pDct_bfr->pBlk[block_id];
			JPEG_SoftIDCT(block, rgiDst);
#endif
			JPEG_ReadPixSrc_From_DCTIOBuffer(block_id);
		}
#endif
		
	}
}
