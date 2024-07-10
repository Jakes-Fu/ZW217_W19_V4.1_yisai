#ifndef RECORD_PROCESS_H
#define RECORD_PROCESS_H

#include "sci_types.h"
#include "audio_api.h"
#include "asm.h"
#include "nv_dev_adp.h"
#include "CVS_frame_recd.h"

/*
 * RCD_DROP_SUPPORT
 * Bug 1321202 (SPCSS00684391)
 * Drop final RCD_FIFO_DEPTH*20ms data before stop/pasue
 */
#include "cvs_fifo_recd.h"

/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
 /*
NB
	CVSI_ptr->sample_rate	= 8000;
	CVSI_ptr->stepLen		= 160;
	CVSI_ptr->sample_pms	= 8;
	N = 256;
WB
	CVSI_ptr->sample_rate	= 16000;
	CVSI_ptr->stepLen		= 320;
	CVSI_ptr->sample_pms	= 16;
	N = 512;
SB
	CVSI_ptr->sample_rate	= 32000;
	CVSI_ptr->stepLen		= 640;
	CVSI_ptr->sample_pms	= 32;
	N = 1024;
FB
	CVSI_ptr->sample_rate	= 48000;
	CVSI_ptr->stepLen		= 960;
	CVSI_ptr->sample_pms	= 48;
	N = 1536;
*/
typedef enum {
	RCD_NB_MODE,
	RCD_WB_MODE,
	RCD_SWB_MODE,
	RCD_UNKOWN_MODE,
	RCD_MAX_MODE,
	RCD_CUR_MODE
} RCD_BAND_MODE_E;

typedef struct
{
	uint32 point;
	int16 *buffer;
}CVSRCD_FRAME_T;

typedef struct
{
	char* mode_name;
	uint32 band_mode;

	CVSRCD_FRAME_T frame_in;
	CVSRCD_FRAME_T frame_cvs;

	CVSRCD_FRAME_T frame_out;

	uint32 frame_size;

	uint16 is_enable;
	uint16 is_config;
	CVSI_T_recd* CVSRCD_ptr;
	uint32 frame_cnt;

	fifo_t *fifo_p;
	fifo_t *fifo_free_p;
}CVSRCD_CTR_T;



/******************************************************************************/
#define	CVSRCD_MODIFY_DATE			"2020-02-25"
#define CVSRCD_PRINT_LENGTH			64
#define CVSRCD_TEMP_BUFFRT_LEN		160

// incress RCD_FIFO_DEPTH to avoid pop(key tone) when stop
#define RCD_FIFO_DEPTH 1
#define RCD_FIFO_DEPTH2 2


/* CVS debug */
#define CVSRCD_DEBUG_ENABLE			0
#define CVSRCD_DUMP_ENABLE			0

#ifndef WIN32
#if CVSRCD_DEBUG_ENABLE
	#define CVSRCD_DEBUG(A,B...)	SCI_TRACE_LOW("[CVSRCD][%s][%d] "A,\
													__FUNCTION__,\
													SCI_GetTickCount(),\
													##B)
	#define CVSRCD_ERROR(A,B...)	SCI_TRACE_LOW("[CVSRCD][%s] "A" !!!",\
													__FUNCTION__,\
													##B)
#else
	#define CVSRCD_DEBUG(A,B...)	(A,##B)
	#define CVSRCD_ERROR(A,B...)	SCI_TRACE_LOW("[CVSRCD][%s] "A" !!!",\
													__FUNCTION__,\
													##B)
#endif
	#define CVSRCD_TRACE(A,B...)	SCI_TRACE_LOW("[CVSRCD][%s][%d] "A,\
													__FUNCTION__,\
													SCI_GetTickCount(),\
													##B)
#else
	#define CVSRCD_DEBUG(A,B)
	#define CVSRCD_ERROR(A,B)
	#define CVSRCD_TRACE(A,B)
#endif

/**---------------------------------------------------------------------------**
 **                           Functions                                       **
 **---------------------------------------------------------------------------**/
PUBLIC int32 CVSRCD_DeInit(void);

PUBLIC int32 CVSRCD_Init(
	char *mode_name,
	uint32 sample_rate);

PUBLIC int32 CVSRCD_Rx_Process(
	uint32 sample_length,
	int16 *psData);

#endif
