/******************************************************************************
 ** File Name:      jencoder.c                                                *
 ** Author:         Zhemin.Lin                                                *
 ** Date:           2004/07/19                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    implementate jpeg encoder operation interface             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2004/07/19     Zhemin.Lin       Create.                                   *
 ******************************************************************************/
#include "os_api.h"
#include "JPG_Codec.h"

//for zigzag has been done in matlab code
//#define COEFF_INDEX(i)  i 


#define COEFF_INDEX(i)  (zigzag_order[i])

/*static void OutPutRstMarker(void)
{
	JPEG_FLUSH(0xFFFFFFFF);
	
	PutMarker(M_RST0+g_JpegCodec.next_restart_num);
	
}
*/
__inline void	MBCopyFromImageToDCTIOBuf(uint8* p_src, uint16 strider, uint8* p_dst)
{
	uint32	i = 0;

	for(i = 0; i < 8; i++)
	{
		*(uint32*)p_dst = *(uint32*)p_src;
		*(uint32*)(p_dst + 4) = *(uint32*)(p_src + 4);		
		 p_src += strider;
		 p_dst += 8;
	}
}

#if 0
void CopyCoeffToMCU(uint8 *y_coeff, uint8 *u_coeff, uint8 *v_coeff, uint16 x, uint16 y)
{

	uint16	i;
	uint8	*srcdata;
	uint8	*destdata,*destdata2;
	uint32	*pIntDst;
	uint32	*pIntSrc; 
	uint16	x_offset, y_offset;
	
	/*output four Y block 8x8*/

	x_offset = (x << 4);
	y_offset = (y << 4);
	destdata = g_JpegCodec.org_blocks[0];
	destdata2 = g_JpegCodec.org_blocks[1];
	srcdata = y_coeff + g_JpegCodec.c_width * y_offset + x_offset;
	for (i = 0; i < 8; i++)
	{
		pIntSrc = (uint32 *) srcdata;
		pIntDst = (uint32 *) destdata;
		pIntDst[0] = pIntSrc[0];
		pIntDst[1] = pIntSrc[1];
		pIntDst = (uint32 *) destdata2;
		pIntDst[0] = pIntSrc[2];
		pIntDst[1] = pIntSrc[3];
		
		srcdata += g_JpegCodec.c_width;
		destdata += 8;
		destdata2 += 8;
	}	

	y_offset = (y << 4) +8;
	destdata = g_JpegCodec.org_blocks[2];
	destdata2 = g_JpegCodec.org_blocks[3];
	srcdata = y_coeff + g_JpegCodec.c_width * y_offset + x_offset;
	for (i = 0; i < 8; i++)
	{
		pIntSrc = (uint32 *) srcdata;
		pIntDst = (uint32 *) destdata;
		pIntDst[0] = pIntSrc[0];
		pIntDst[1] = pIntSrc[1];
		pIntDst = (uint32 *) destdata2;
		pIntDst[0] = pIntSrc[2];
		pIntDst[1] = pIntSrc[3];
		
		srcdata += g_JpegCodec.c_width;
		destdata += 8;
		destdata2 += 8;
	}



	x_offset = x << 3;
	y_offset = y << 3;

	srcdata = u_coeff + (g_JpegCodec.c_width >> 1) * y_offset + x_offset;
	destdata = g_JpegCodec.org_blocks[4];
	for (i = 0; i < 8; i++)
	{
		pIntSrc = (uint32 *) srcdata;			
		pIntDst = (uint32 *) destdata;		
		pIntDst[0] = pIntSrc[0];			
		pIntDst[1] = pIntSrc[1];			
		srcdata += g_JpegCodec.c_width >> 1;						
		destdata += 8;						
	}


	
	srcdata = v_coeff + (g_JpegCodec.c_width >> 1) * y_offset + x_offset;
	destdata = g_JpegCodec.org_blocks[5];
	for (i = 0; i < 8; i++)
	{
		pIntSrc = (uint32 *) srcdata;
		pIntDst = (uint32 *) destdata;
		pIntDst[0] = pIntSrc[0];
		pIntDst[1] = pIntSrc[1];
		srcdata += g_JpegCodec.c_width >> 1;
		destdata += 8;
	}







	
}

#endif

uint32 JPEG_Encode(
	uint32  y_coeff,
	uint32  u_coeff,
	uint32  v_coeff
)
{
	
	uint16 x, y;
	if (g_JpegCodec.error_id)
	{
//		JPEG_TRACE("JPEG: current codec state is error\n");
		return JPEG_FAILED;
	}

	WriteJpegHead();

	/*for every MCU do following*/
	for (y=0; y<g_JpegCodec.mcu_num_y; y++)
		for (x = 0; x<g_JpegCodec.mcu_num_x; x++)
	{
		/*1, copy coeff data to correspondig blocks*/
	//	CopyCoeffToMCU((uint8 *)y_coeff, (uint8 *)u_coeff, (uint8 *)v_coeff, x, y);
		/*2, call QuantMCU*/
		/*quantilization is done in huffman encode*/
		//JPEG_QuantMCU();
		/*3, call EncodeMCU*/
		JPEG_EncodeMCU((uint8 *)y_coeff, (uint8 *)u_coeff, (uint8 *)v_coeff, x, y);
		
		if (g_JpegCodec.error_id)
		{
			return JPEG_FAILED;
		}
	}


		
	JPEG_FLUSH(0);
	
	{
		/*4, write EOI*/
		WriteJpegTail();
	}
	return JPEG_SUCCESS;
}
#ifdef _VSP_
void JPEG_HardDCT(uint8 block_num)
{
	uint8 cbp;
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
	VSP_SrcPix_DCT_CFG(DCT_LITTLE_ENDIAN,DCT_BIG_ENDIAN);
	
	VSP_Disable_Quan();
	
	START_DCT_ENGINE
	
}
#endif

void JPEG_EncodeMCU(uint8 *y_coeff, uint8 *u_coeff, uint8 *v_coeff, uint16 x, uint16 y)
{
	int16 *rgiDst;
	uint16 block_id, ci;
	HUFF_TBL_T *dc_tbl, *ac_tbl;
	const uint16 *quant;
	int32 tmp;
	uint8 *block;
	uint16	x_offset, y_offset;
	uint8	*srcdata;
	uint16     width_strider = g_JpegCodec.c_width;
	
	for(block_id=0; block_id < 6; block_id++)
	{
		if(block_id < 4)
		{
			x_offset = (x << 4) + ((block_id % 2) ? 8 : 0);
			if(block_id < 2)
				y_offset = (y << 4);
			else
				y_offset = (y << 4) +8;
			width_strider = g_JpegCodec.c_width;
			srcdata = (uint8*)(y_coeff + width_strider * y_offset + x_offset);
		}
		else
		{
			x_offset = x << 3;
			y_offset = y << 3;
			srcdata = block_id == 4 ? u_coeff : v_coeff;
			width_strider = g_JpegCodec.c_width >> 1;
			srcdata += width_strider * y_offset + x_offset;
		}
		
#ifndef _VSP_		
		block = g_JpegCodec.org_blocks[block_id];
		rgiDst = g_pDct_bfr->pBlk[block_id];
		/*check if at the end of blocks*/
		if (!block)
		{
			break;
		}	
		//DCT Transform
	//	JPEG_WritePixSrc_To_DCTIOBuffer(block_id);
		MBCopyFromImageToDCTIOBuf(srcdata, width_strider, (uint8*)g_pDct_bfr->pBlk[block_id]);
		JPEG_SoftDCT(block,rgiDst);
#else
		VSP_Write_SrcPix_DCTIOBUF_CFG(DCT_INPUT_BUF_BLOCK, DCT_BIG_ENDIAN, DCT_BIG_ENDIAN);	   
		MBCopyFromImageToDCTIOBuf(srcdata, width_strider, (uint8*)g_pDct_bfr->pBlk[0]);
		JPEG_HardDCT(1);
#endif

#if 1
		if(g_block_num == (g_JpegCodec.mcu_num_x * g_JpegCodec.mcu_num_y * 6)-1)
		{
			rgiDst = g_JpegCodec.blocks[g_oldblock_id];
			/*select huffman table*/
			ci = g_JpegCodec.blocks_membership[g_oldblock_id];
			dc_tbl = &(g_JpegCodec.dc_huff_tbl[g_JpegCodec.tbl_map[ci].dc_huff_tbl_id]);
			ac_tbl = &(g_JpegCodec.ac_huff_tbl[g_JpegCodec.tbl_map[ci].ac_huff_tbl_id]);
			quant = g_JpegCodec.quant_tbl_new[g_JpegCodec.tbl_map[ci].quant_tbl_id];
			/*set pre_dc_val*/
			/*09/18/2004*/
			rgiDst[0] -= 1024;
			tmp = ((rgiDst[0]) * quant[0]+ (1<<14))>>15;
			if (g_JpegCodec.tmp_file_mode == 1)
			{
				rgiDst[0] = ((int16)tmp);
			}
			else
			{
				rgiDst[0] = ((int16)tmp) - g_JpegCodec.pre_dc_value[ci];
			}

			g_JpegCodec.pre_dc_value[ci] = (int16)tmp;

			/*do VLC and Quant one block*/
			Vlc_Quant_Block(rgiDst, dc_tbl, ac_tbl, quant);

			
			JPEG_ReadCoeff_From_DCTIOBuffer(block_id);
			
			//The Last block;
			rgiDst = g_JpegCodec.blocks[block_id];
			/*select huffman table*/
			ci = g_JpegCodec.blocks_membership[block_id];
			dc_tbl = &(g_JpegCodec.dc_huff_tbl[g_JpegCodec.tbl_map[ci].dc_huff_tbl_id]);
			ac_tbl = &(g_JpegCodec.ac_huff_tbl[g_JpegCodec.tbl_map[ci].ac_huff_tbl_id]);
			quant = g_JpegCodec.quant_tbl_new[g_JpegCodec.tbl_map[ci].quant_tbl_id];
			/*set pre_dc_val*/
			/*09/18/2004*/
			rgiDst[0] -= 1024;
			tmp = ((rgiDst[0]) * quant[0]+ (1<<14))>>15;
			if (g_JpegCodec.tmp_file_mode == 1)
			{
				rgiDst[0] = ((int16)tmp);
			}
			else
			{
				rgiDst[0] = ((int16)tmp) - g_JpegCodec.pre_dc_value[ci];
			}

			g_JpegCodec.pre_dc_value[ci] = (int16)tmp;

			/*do VLC and Quant one block*/
			Vlc_Quant_Block(rgiDst, dc_tbl, ac_tbl, quant);
		}
		else if(g_block_num != 0)
		{
			rgiDst = g_JpegCodec.blocks[g_oldblock_id];
			/*select huffman table*/
			ci = g_JpegCodec.blocks_membership[g_oldblock_id];
			dc_tbl = &(g_JpegCodec.dc_huff_tbl[g_JpegCodec.tbl_map[ci].dc_huff_tbl_id]);
			ac_tbl = &(g_JpegCodec.ac_huff_tbl[g_JpegCodec.tbl_map[ci].ac_huff_tbl_id]);
			quant = g_JpegCodec.quant_tbl_new[g_JpegCodec.tbl_map[ci].quant_tbl_id];
			/*set pre_dc_val*/
			/*09/18/2004*/
			rgiDst[0] -= 1024;
			tmp = ((rgiDst[0]) * quant[0]+ (1<<14))>>15;
			if (g_JpegCodec.tmp_file_mode == 1)
			{
				rgiDst[0] = ((int16)tmp);
			}
			else
			{
				rgiDst[0] = ((int16)tmp) - g_JpegCodec.pre_dc_value[ci];
			}

			g_JpegCodec.pre_dc_value[ci] = (int16)tmp;

			/*do VLC and Quant one block*/
			Vlc_Quant_Block(rgiDst, dc_tbl, ac_tbl, quant);
			
			JPEG_ReadCoeff_From_DCTIOBuffer(block_id);
			g_oldblock_id = block_id;
		}
		else
		{
			JPEG_ReadCoeff_From_DCTIOBuffer(block_id);
			g_oldblock_id = block_id;
		}
		g_block_num++;
#else
		{
			JPEG_ReadCoeff_From_DCTIOBuffer(block_id);
						
			//The Last block;
			rgiDst = g_JpegCodec.blocks[block_id];
			/*select huffman table*/
			ci = g_JpegCodec.blocks_membership[block_id];
			dc_tbl = &(g_JpegCodec.dc_huff_tbl[g_JpegCodec.tbl_map[ci].dc_huff_tbl_id]);
			ac_tbl = &(g_JpegCodec.ac_huff_tbl[g_JpegCodec.tbl_map[ci].ac_huff_tbl_id]);
			quant = g_JpegCodec.quant_tbl_new[g_JpegCodec.tbl_map[ci].quant_tbl_id];
			/*set pre_dc_val*/
			/*09/18/2004*/
			rgiDst[0] -= 1024;
			tmp = ((rgiDst[0]) * quant[0]+ (1<<14))>>15;
			if (g_JpegCodec.tmp_file_mode == 1)
			{
				rgiDst[0] = ((int16)tmp);
			}
			else
			{
				rgiDst[0] = ((int16)tmp) - g_JpegCodec.pre_dc_value[ci];
			}

			g_JpegCodec.pre_dc_value[ci] = (int16)tmp;

			/*do VLC and Quant one block*/
			Vlc_Quant_Block(rgiDst, dc_tbl, ac_tbl, quant);
		}
#endif
	}
}

