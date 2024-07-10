
#ifndef _CONVnSAMPLE_H_
#define _CONVnSAMPLE_H_

#include "sci_types.h"

#define POS_Y1	0
#define POS_U	1
#define POS_Y2	2
#define POS_V	3

typedef enum
{
	IMGFMT_MCU2YUV = 1,		
	IMGFMT_MCU2YUYV,	//ok	1280*1024
	IMGFMT_MCU2YUYV_1600X1200,
	IMGFMT_YUV2YUV,		
	IMGFMT_YUV2YUYV,			
	IMGFMT_YUYV2YUV422,	
	IMGFMT_YUYV2YUV420, //ok
	IMGFMT_YUYV2YUYV,	
	IMGFMT_YUYV2MCU //ok
};

typedef struct MCU_SCALING_tag
{
	uint8*	inbuf;
	uint32	nBufSize;
	uint16	width;
	uint16	height;
	uint8	h_ratio;
	uint8	v_ratio;
	uint16	convert_type;
	uint8*	outbuf;

	uint32	current_line;
}MCU_SCALING_T;

typedef PUBLIC uint32  (*trace_log)(const char *x_format, ...);
typedef PUBLIC uint32 (*dma_transfer)(uint32 pSrc,uint32 pDest,uint32 nSize);

extern uint8 g_iram_buffer[];
extern trace_log g_trace_function;
extern dma_transfer p_dma_transfer;

extern uint8* SPRD_Scaling_init(trace_log pTrace,  dma_transfer dma_transfer);

extern int SPRD_convNsample(uint8* pInBuf, uint32 nInBufSize, 
					uint8* pOutBuf, uint32 nOutBufSize, 
					uint16 nWidth, uint16 nHeight,
					uint8 h, uint8 v, 
					uint16 convert_type);

extern void Scaling_Get_Data(uint8* return_ptr, uint16 num_of_line);

#endif