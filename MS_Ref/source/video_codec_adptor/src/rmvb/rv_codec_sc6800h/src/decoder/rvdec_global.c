
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

RV_DECODER_T *g_rv_decoder_ptr;
DEC_FRM_BFR g_RvFrmYUVBfr[YUV_BUFFER_NUM+1];
uint32 g_nFrame_rv;
#if 0
#if defined(BSLICE_SUPPORTED)
uint32 g_nBFrame;
uint32 g_Bframe_eb;
#endif
#endif

#if !defined(SC6800H_VIDEO_ROMCODE)
uint8 g_ClipTab_rv89[CLIP_TAB_SIZE];
#endif
uint8 *g_rgiRvClipTab;

RV_VLC_SET_T g_intra_decode_tables[MAX_INTRA_QP_REGIONS];
RV_VLC_SET_T g_inter_decode_tables[MAX_INTER_QP_REGIONS];

BYTE_VLC_SYMBOL_T g_aic_top_vlc;
BYTE_VLC_SYMBOL_T g_aic_mode1_vlc[AIC_MODE1_NUM];
SHORT_VLC_SYMBOL_T g_aic_mode2_vlc[AIC_MODE2_NUM];
BYTE_VLC_SYMBOL_T g_ptype_vlc[NUM_PTYPE_VLCS];
BYTE_VLC_SYMBOL_T g_btype_vlc[NUM_BTYPE_VLCS];

read_mbtype_PB_func g_RvDec_ReadMbtype_PB;
set_deblock_coeff_func g_RvDec_Set_Dbk_coeff;
get_slice_sc_func g_RvDec_Get_Slice_StartCode;
mc_func g_RvDec_Luma16x16_MC[16];
mc_func g_RvDec_Luma8x8_MC[16];
mc_func g_RvDec_Chroma8x8_MC[16];
mc_func g_RvDec_Chroma4x4_MC[16];
Intra4x4Pred_func g_RvIPred4x4[9];
Intra16x16Pred_func g_intraPredLuma16x16[4];
Intra16x16Pred_func g_intraPredChroma8x8[4];
Intra4x4Pred_func g_RvIPred4x4_d[9];
Intra16x16Pred_func g_intraPredLuma16x16_d[4];
Intra16x16Pred_func g_intraPredChroma8x8_d[4];

decMV_B_func g_DecMV_B;
weighted_average_func g_RvDec_Get_Weighted_Average;
uint8 g_int_bfr[21*MB_SIZE*2];

// bottom linebuffer
uint8	*g_bottom_line_bfr[3];

//store current mb's top and left refernce pixel; 
uint8 *luma_top/*[17+4]*/, luma_left[16];
uint8 *chroma_top[2]/*[9+4]*/, chroma_left[2][8];
uint8 top_left[3];

#if defined(SC6800H_VIDEO_ROMCODE)
const uint32 *g_msk_rvdec_ptr;
const uint8 *g_clip_tbl_rvdec_ptr;
#else
uint32 g_msk_rvdec_ptr[33];
uint8 g_clip_tbl_rvdec_ptr[CLIP_TAB_SIZE];
#endif
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
