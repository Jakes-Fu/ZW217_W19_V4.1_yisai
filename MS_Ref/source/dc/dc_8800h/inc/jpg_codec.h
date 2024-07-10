/******************************************************************************
 ** File Name:      jcodec.h                                                  *
 ** Author:         Zhemin.Lin                                                *
 ** Date:           2004/07/19                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    define the jpeg codec structure & the interface           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2004/07/19     Zhemin.Lin       Create.                                   *
 ******************************************************************************/
 
#ifndef JPEG_CODEC_H
#define JPEG_CODEC_H


#include <stdio.h>
#include <stdlib.h>
#include "string.h"
//#include "memory.h"

#include "jpg_jfif.h"
#include "vsp_drv.h"
#include "jpg_init.h"
#include "jpg_stream.h"
#include "jpg_huff.h"
#include "jpg_quant.h"
#include "jpg_dctxidct.h"
#include "dal_jpeg.h"
/************************************************************************/
/* MACRO Define                                                         */
/************************************************************************/
/*component id*/
#define LUM_ID		0
#define CHR_ID		1
#define Y_ID		0
#define U_ID		1
#define V_ID		2

//
#define INPUT_BUF_SIZE 4096


/*define return value*/
#define JPEG_SUCCESS	0
#define JPEG_FAILED		(-1)

/*define error id*/ 
#define JPEG_EID_NONE	0
#define JPEG_EID_MISSHUFF		1
#define JPEG_EID_MISSQUANT		2
#define JPEG_EID_IMGSIZE		3
#define JPEG_EID_SAMPLEFORMAT	4
#define JPEG_EID_COMPNUM		5
#define JPEG_EID_JFIFLEN		6
#define JPEG_EID_COLORSPACE		7
#define JPEG_EID_SAMPLEPRCISION	8
#define JPEG_EID_JFIFVERSION	9
#define JPEG_EID_NOTJFIF		10
#define JPEG_EID_MARKNOTSUPPORT	11
#define JPEG_EID_FILETYPE		12
#define JPEG_EID_ENCOUNTEOI		13
#define JPEG_EID_FILEOP			14
#define JPEG_EID_MEMOP			15

#define MAX_MCU_NUM	6

#define MAX3(a, b, c)	\
	MAX((a), MAX((b), (c)))

/************************************************************************/
/* Struct Definition	                                                    */
/************************************************************************/
//
typedef void(* JPEG_CallBack)(uint8 *y_coeff, uint8 *u_coeff, uint8 *v_coeff, uint16 x, uint16 y);
//
typedef struct{
	uint8	h_ratio;
	uint8	v_ratio;
}SAMPLE_RATIO_T;

typedef struct{
	uint8	dc_huff_tbl_id;
	uint8	ac_huff_tbl_id;
	uint8	quant_tbl_id;
}TBL_MAP_T;


typedef struct {
	/*image format*/
	uint16			width;				/*image real width*/
	uint16			height;				/*image real height*/
	uint8			sample_format;		/*sample format, FORMAT_YUV411 FORMAT_YUV422*/
	SAMPLE_RATIO_T	ratio[3];			/*sample ratio*/
	TBL_MAP_T		tbl_map[3];			/*table map for three component*/
	/*coeff size*/
	uint16			c_width;			/*width of coeff*/
	uint16			c_height;			/*height of coeff*/
	/*density*/
	uint16			xdensity;			/*X density*/
	uint16			ydensity;			/*Y density*/
	/*huffman information*/
	HUFF_TBL_T 		dc_huff_tbl[2];		/*dc huffman table*/
	HUFF_TBL_T 		ac_huff_tbl[2];		/*ac huffman table*/
	/*quantization table*/
	const uint8		*quant_tbl[2];		/*quantization table*/
	uint16			*quant_tbl_new[2];
	/*MCU info*/
	uint16 			mcu_num_x;			/*mcu number per row*/
	uint16 			mcu_num_y;			/*mcu number per col*/
	uint16			mcu_height;
	uint16			mcu_width;
	uint16			h_ratio_max;
	uint16			v_ratio_max;
	int16 			*blocks[6];		/*mcu block buffer*/
	//added by wangyi//
	uint8			*org_blocks[6];    /*mcu original block buffer*/
	uint8  			blocks_membership[MAX_MCU_NUM];/*which component the block blong to*/
	int16 pre_dc_value[3];				/*store pre block value for DPCM coding*/
	/*stream*/
	void *stream;						/*point to JPEG stream*/
	uint32			error_id;			/*last error id*/
	uint32			restart_interval;
	uint32			restart_to_go;
	uint32			next_restart_num;
	uint32			tmp_file_mode;		/*if 1, it means, it's means it's a tmp file mode*/
	uint32			do_dequant;
	FILE			*stream_file;
	uint32          i_words;
	uint32          i_bits;
	uint8			block_num;
}JPEG_CODEC_T;

extern JPEG_CODEC_T g_JpegCodec;

extern int16 g_mcu_buf[64*MAX_MCU_NUM];
extern uint8 g_mcu_org_buf[64*MAX_MCU_NUM];
/*zigzag table*/
extern const uint8 zigzag_order[64+16];
/**/
extern VSP_BASE_BFR_T *g_vsp_pBase_bfr;
extern DCT_IO_BUFFER_T *g_pDct_bfr;
extern VSP_BASE_BFR_T vsp_pBase_bfr;
extern DCT_IO_BUFFER_T pDct_bfr;

extern uint16 g_block_num;
extern uint16 g_oldblock_id;
extern uint16 jstream_op_mode;	
extern uint8  *jstream_buf;
extern uint8  inter_buf[INPUT_BUF_SIZE];
extern uint32 read_bytes;
extern uint32 bytes_in_buf;
extern uint32 jstream_len;
extern uint32 jstream_current_pos;
extern uint32 jstream_words;
extern uint16 jremain_bit_num;

void JPEG_EncodeMCU(uint8 *y_coeff, uint8 *u_coeff, uint8 *v_coeff, uint16 x, uint16 y);
uint32 JPEG_Encode(
				   uint32  y_coeff,
				   uint32  u_coeff,
				   uint32  v_coeff);

/*decoder operation interface*/

void JPEG_DecodeMCU(void);

uint32 JPEG_Decode(
				   uint32  y_coeff,
				   uint32  u_coeff,
				   uint32  v_coeff);
//
//for DCT
void JPEG_WritePixSrc_To_DCTIOBuffer(uint16 block_id);
void JPEG_ReadCoeff_From_DCTIOBuffer(uint16 block_id);
//for IDCT
void JPEG_WriteCoeff_To_DCTIOBuffer(uint16 block_id);
void JPEG_ReadPixSrc_From_DCTIOBuffer(uint16 block_id);

#endif
