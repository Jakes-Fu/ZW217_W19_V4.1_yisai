#include "JPG_Codec.h"

/************************************************************************/
/* Data In Internal RAM                                                 */
/************************************************************************/
/*codec structure*/
JPEG_CODEC_T g_JpegCodec;

/*MCU buffer*/
int16 g_mcu_buf[MAX_MCU_NUM*64];

/*Block buffer*/
uint8 g_mcu_org_buf[MAX_MCU_NUM*64];

/**for hardare accelerator**/
VSP_BASE_BFR_T *g_vsp_pBase_bfr;
DCT_IO_BUFFER_T *g_pDct_bfr;

VSP_BASE_BFR_T vsp_pBase_bfr;
DCT_IO_BUFFER_T pDct_bfr;

//
uint8	iclip[1024];
uint8	*pClip_table;

//
uint16   g_block_num;
uint16    g_oldblock_id;
/************************************************************************/
/* Data In External RAM                                                 */
/************************************************************************/