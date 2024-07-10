#include "JPG_Codec.h"


/************************************************************************/
/* Encode Initiate                                                      */
/************************************************************************/
uint32 InitMiscFields(uint16 width, uint16 height, uint16 sample_format)
{
	if ((width < 8)||(height<8))
	{
		return JPEG_FAILED;
	}
	
	g_JpegCodec.width = width;
	g_JpegCodec.height = height;
	
	g_JpegCodec.ratio[Y_ID].h_ratio = 2;
	g_JpegCodec.ratio[Y_ID].v_ratio = 2;
	g_JpegCodec.ratio[U_ID].h_ratio = 1;
	g_JpegCodec.ratio[U_ID].v_ratio = 1;
	g_JpegCodec.ratio[V_ID].h_ratio = 1;
	g_JpegCodec.ratio[V_ID].v_ratio = 1;
	
	/*get the mcu size*/
	g_JpegCodec.v_ratio_max = MAX3(g_JpegCodec.ratio[Y_ID].v_ratio, g_JpegCodec.ratio[U_ID].v_ratio, g_JpegCodec.ratio[V_ID].v_ratio);
	g_JpegCodec.h_ratio_max = MAX3(g_JpegCodec.ratio[Y_ID].h_ratio, g_JpegCodec.ratio[U_ID].h_ratio, g_JpegCodec.ratio[V_ID].h_ratio);
	g_JpegCodec.mcu_height = 8 * g_JpegCodec.v_ratio_max;
	g_JpegCodec.mcu_width = 8 * g_JpegCodec.h_ratio_max;
	
	g_JpegCodec.mcu_num_x = (width + g_JpegCodec.mcu_width -1)/g_JpegCodec.mcu_width;
	g_JpegCodec.mcu_num_y = (height + g_JpegCodec.mcu_height -1)/g_JpegCodec.mcu_height;
	//Adjusted image width and height
	g_JpegCodec.c_width = g_JpegCodec.mcu_num_x * g_JpegCodec.mcu_width;
	g_JpegCodec.c_height = g_JpegCodec.mcu_num_y * g_JpegCodec.mcu_height;
	
	return JPEG_SUCCESS;
	
}

uint32 JPEG_EncInit(
					uint16 width,
					uint16 height,
					uint16 compress_level, 
					uint8 *out_stream_ptr
					)
{
	memset(&g_JpegCodec, 0, sizeof(JPEG_CODEC_T));
	//	g_JpegCodec.jpg_trace_enc = fopen("jpg_trace_enc.txt", "w");
	//	g_JpegCodec.q_file = fopen("jpg_trace_quant.txt", "w");
	
	/*init sample ratio*/
	if (InitMiscFields(width, height, 1/*FORMAT_YUV420*/))
	{
		return JPEG_FAILED;
	}
	
	/*set huffman table*/
	InitHuffTblWithDefaultValue();
	
	/*set quantization table*/
	InitQuantTbl(compress_level);
	
	AdjustQuantTbl_Enc();
	
	/*alloc bocks buffer for 3 components*/
	AllocMCUBuf();
	/*open stream*/
	InitStreamForRW(out_stream_ptr, 0, WRITE_OP);
	
	g_JpegCodec.restart_interval = 0;
	g_JpegCodec.restart_to_go	= 0;
	g_JpegCodec.next_restart_num = 0;
	

	JPEG_VspInit();
	
	return JPEG_SUCCESS;
}

/************************************************************************/
/* Decode Initiate                                                      */
/************************************************************************/
void UpdateMiscFields( void)
{
	uint16 width, height;
	
	width = g_JpegCodec.width;
	height = g_JpegCodec.height;
	
	/*get the mcu size*/
	g_JpegCodec.v_ratio_max = MAX3(g_JpegCodec.ratio[Y_ID].v_ratio, g_JpegCodec.ratio[U_ID].v_ratio, g_JpegCodec.ratio[V_ID].v_ratio);
	g_JpegCodec.h_ratio_max = MAX3(g_JpegCodec.ratio[Y_ID].h_ratio, g_JpegCodec.ratio[U_ID].h_ratio, g_JpegCodec.ratio[V_ID].h_ratio);
	g_JpegCodec.mcu_height = 8 * g_JpegCodec.v_ratio_max;
	g_JpegCodec.mcu_width = 8 * g_JpegCodec.h_ratio_max;

	if(g_JpegCodec.mcu_height == 0 || g_JpegCodec.mcu_width == 0 )	
		return ;
	g_JpegCodec.mcu_num_x = (width + g_JpegCodec.mcu_width -1)/g_JpegCodec.mcu_width;
	g_JpegCodec.mcu_num_y = (height + g_JpegCodec.mcu_height -1)/g_JpegCodec.mcu_height;
	
	g_JpegCodec.c_width = g_JpegCodec.mcu_num_x * g_JpegCodec.mcu_width;
	g_JpegCodec.c_height = g_JpegCodec.mcu_num_y * g_JpegCodec.mcu_height;
	
	/*check if huffman table is null*/
	if (!g_JpegCodec.dc_huff_tbl[Y_ID].bits)
	{
		//		JPEG_ERROR(JPEG_EID_MISSHUFF,"using default table\n");
		InitHuffTblWithDefaultValue();
	}
	
	/*check if quant_tbl is null*/
	if (!g_JpegCodec.quant_tbl[Y_ID])
	{
		//		JPEG_ERROR(JPEG_EID_MISSQUANT, "using default table\n");
		InitQuantTbl(32/*COMPRESS_LEVEL_NORMAL*/);
	}
	
	
}

uint32 JPEG_DecInit(uint8 *input_stream_ptr ,uint32 len)
{
	memset(&g_JpegCodec, 0, sizeof(g_JpegCodec));
	
	//	g_JpegCodec.q_file = fopen("jpg_trace_quant.txt", "w");
	//	g_JpegCodec.huff_file = fopen("jpg_trace_huff.txt", "w");
	//	g_JpegCodec.stream_file = fopen("jpg_trace_stream.txt", "w");
	
	
	/*1, open stream file*/
	InitStreamForRW (input_stream_ptr, len, READ_OP);
	
	/*2, get image info from stream, the stream point stop at the end of scan header*/
	if (ReadJpegHead())
	{
		return JPEG_FAILED;
	}
	
//	g_JpegCodec.do_dequant = 1;
	
	/*3, update misc field in the g_JpegCodec structure*/
	UpdateMiscFields();
	
	AdjustQuantTbl_Dec();	
	
	/*4, init huffman table*/
	InitHuffTbl();
	
	/*5, set blocks*/
	AllocMCUBuf();
	
	JPEG_VspInit();

	Initialize_Clip();

	return JPEG_SUCCESS;
}


/************************************************************************/
/* JPEG VSP Initiate                                                    */
/************************************************************************/
uint8 JPEG_VspInit()
{
	int16 frame_width;
	int16 frame_height;
	frame_width = g_JpegCodec.width;
	frame_height = g_JpegCodec.height;
	g_vsp_pBase_bfr = (VSP_BASE_BFR_T *)(&vsp_pBase_bfr);
	g_pDct_bfr = (DCT_IO_BUFFER_T *)(&pDct_bfr);
#ifndef _VSP_	
	JPEG_InitBaseBuffer(frame_width,frame_height);
	JPEG_InitDCTIOBuffer();	
	
	JPEG_InitDCTEngine();
	g_block_num = 0;
#else
//	VSP_InitBaseBuffer(g_vsp_pBase_bfr);
	VSP_InitDCTIOBuffer(g_pDct_bfr);
	VSP_ResetDCT();
	VSP_InitDCTEngine();
	g_block_num = 0;
//	VSP_InitVSPEndian(VSP_BIG_ENDIAN);
#endif
	return JPEG_SUCCESS;
}

void JPEG_InitBaseBuffer(int16 frame_width, int16 frame_height)
{
	VSP_BASE_BFR_T *pBase_bfr;
	
	pBase_bfr = g_vsp_pBase_bfr;
	
	pBase_bfr->pDct_io_bfr_base		= (uint8 *)malloc(64*2*6);
	pBase_bfr->pMe_cmd_bfr_base		= (uint8 *)malloc(sizeof(MEA_CMD_T)*64);
	pBase_bfr->pMea_ref_bfr_base		= (uint8 *)malloc(frame_width * frame_height* 3 / 2);
	pBase_bfr->pMea_src_bfr_base		= (uint8 *)malloc(512);
	pBase_bfr->pMca_output_bfr_base	= (uint8 *)malloc(512);
	pBase_bfr->pMea_virtual_bfr_base     = (uint8 *)malloc(512); //64*2*6
	
	pBase_bfr->pMea_ctrl_reg_base 			= (uint8 *)malloc(sizeof(MEA_CTRL_REG_T));
	pBase_bfr->pDct_ctrl_reg_base		= (uint8 *)malloc(sizeof(DCT_CTRL_REG_T));
	pBase_bfr->pGlobal_reg_base 		= (uint8 *)malloc(sizeof(GLOBAL_CTRL_REG_T));	
}

void JPEG_InitDCTIOBuffer()
{
	DCT_IO_BUFFER_T *pDct_io_bfr;
	
	pDct_io_bfr = g_pDct_bfr;
	
	pDct_io_bfr->pBlk[0] = (int16 *)g_vsp_pBase_bfr->pDct_io_bfr_base;
	pDct_io_bfr->pBlk[1] = (int16 *)(pDct_io_bfr->pBlk[0] + 64);
	pDct_io_bfr->pBlk[2] = (int16 *)(pDct_io_bfr->pBlk[1] + 64);
	pDct_io_bfr->pBlk[3] = (int16 *)(pDct_io_bfr->pBlk[2] + 64);
	pDct_io_bfr->pBlk[4] = (int16 *)(pDct_io_bfr->pBlk[3] + 64);
	pDct_io_bfr->pBlk[5] = (int16 *)(pDct_io_bfr->pBlk[4] + 64);
}

void JPEG_InitDCTEngine()
{

}

void JPEG_InitVSPEndian()
{

}