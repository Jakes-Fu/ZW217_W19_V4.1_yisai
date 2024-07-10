/******************************************************************************
 ** File Name:      jquant.c                                                  *
 ** Author:         Zhemin.Lin                                                *
 ** Date:           2004/07/19                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    implementation DCT quantization interface                 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2004/07/19     Zhemin.Lin       Create.                                   *
 ******************************************************************************/

#include "JPG_Codec.h"

#define COEFF_INDEX(i) i

static uint16 s_level = 0;


void InitQuantTbl(uint16 level)
{
	uint8  i;
	uint16 tmp;

	if (level >= 32)
	{
		s_level = 32;
		g_JpegCodec.quant_tbl[LUM_ID] = lum_quant_tbl_default;
		g_JpegCodec.quant_tbl[CHR_ID] = chr_quant_tbl_default;
	}
	else if (level > 0)
	{
		s_level = level;
		g_JpegCodec.quant_tbl[LUM_ID] = &quant_tbl[LUM_ID][0];
		g_JpegCodec.quant_tbl[CHR_ID] = &quant_tbl[CHR_ID][0];
		for (i=0; i<64; i++)
		{
			tmp = (lum_quant_tbl_default[i]*s_level + 16)/32;
			quant_tbl[0][i] = (uint8)tmp;
		}
		for (i=0; i<64; i++)
		{
			tmp = (chr_quant_tbl_default[i]*s_level + 16)/32;
			quant_tbl[1][i] = (uint8)tmp; 
		}
		
	}
	else
	{
		s_level = 0;
		g_JpegCodec.quant_tbl[LUM_ID] = &quant_tbl[LUM_ID][0];
		g_JpegCodec.quant_tbl[CHR_ID] = &quant_tbl[CHR_ID][0];
	}
	
	g_JpegCodec.tbl_map[Y_ID].quant_tbl_id = LUM_ID;
	g_JpegCodec.tbl_map[U_ID].quant_tbl_id = CHR_ID;
	g_JpegCodec.tbl_map[V_ID].quant_tbl_id = CHR_ID;
}


void AdjustQuantTbl_Enc(void)
{
	uint8 tbl_id, i;

	g_JpegCodec.quant_tbl_new[LUM_ID] = &quant_tbl_new[LUM_ID][0];
	g_JpegCodec.quant_tbl_new[CHR_ID] = &quant_tbl_new[CHR_ID][0];
	
//	PrintQuantTblToFile_Enc("\\\\Luminance quantization table", g_JpegCodec.quant_tbl[LUM_ID] );
//	PrintQuantTblToFile_Enc("\\\\Chrominance quantization table", g_JpegCodec.quant_tbl[CHR_ID]);
	
	if (s_level)
	{
		for (tbl_id = 0; tbl_id < 2; tbl_id++)
		{
			for (i=0; i<64; i++)
			{
				quant_tbl_new[tbl_id][i] = (1<<15) / g_JpegCodec.quant_tbl[tbl_id][i];
			}
		}
	}
	else
	{
		/*if level == 0, not quantization, the input has been quantized*/
		for (tbl_id = 0; tbl_id < 2; tbl_id++)
		{
			for (i=0; i<64; i++)
			{
				quant_tbl_new[tbl_id][i] = (1<<15);
			}
		}
	}
}

void AdjustQuantTbl_Dec()
{
	uint8 tbl_id, i;

//	PrintQuantTblToFile_Dec("\\\\Luminance quantization table", g_JpegCodec.quant_tbl[LUM_ID] );
//	PrintQuantTblToFile_Dec("\\\\Chrominance quantization table", g_JpegCodec.quant_tbl[CHR_ID]);


	g_JpegCodec.quant_tbl_new[LUM_ID] = &quant_tbl_new[LUM_ID][0];
	g_JpegCodec.quant_tbl_new[CHR_ID] = &quant_tbl_new[CHR_ID][0];
	
//	if (g_JpegCodec.do_dequant)
	{
		for (tbl_id = 0; tbl_id < 2; tbl_id++)
		{
			for (i=0; i<64; i++)
			{
				quant_tbl_new[tbl_id][i] = quant_tbl[tbl_id][i];
			}
		}
	}
}