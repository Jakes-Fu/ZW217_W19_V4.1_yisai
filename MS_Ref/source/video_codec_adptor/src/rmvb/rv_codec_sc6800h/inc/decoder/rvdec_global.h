
#ifndef _RVDEC_GLOBAL_H_
#define _RVDEC_GLOBAL_H_

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "rvdec_mode.h"
#include "rvdec_bitstrm.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif

extern RV_DECODER_T *g_rv_decoder_ptr;
extern DEC_FRM_BFR g_RvFrmYUVBfr[YUV_BUFFER_NUM+1];	//+1 for downsize
extern uint32 g_nFrame_rv;
#if 0
#if defined(BSLICE_SUPPORTED)
extern uint32 g_nBFrame;
extern uint32 g_Bframe_eb;
#endif
#endif

extern read_mbtype_PB_func g_RvDec_ReadMbtype_PB;
extern set_deblock_coeff_func g_RvDec_Set_Dbk_coeff;
extern get_slice_sc_func g_RvDec_Get_Slice_StartCode;
extern mc_func g_RvDec_Luma16x16_MC[16];
extern mc_func g_RvDec_Luma8x8_MC[16];
extern mc_func g_RvDec_Chroma8x8_MC[16];
extern mc_func g_RvDec_Chroma4x4_MC[16];
extern Intra4x4Pred_func g_RvIPred4x4[9];
extern Intra16x16Pred_func g_intraPredLuma16x16[4];
extern Intra16x16Pred_func g_intraPredChroma8x8[4];
extern Intra4x4Pred_func g_RvIPred4x4_d[9];
extern Intra16x16Pred_func g_intraPredLuma16x16_d[4];
extern Intra16x16Pred_func g_intraPredChroma8x8_d[4];
extern decMV_B_func g_DecMV_B;
extern weighted_average_func g_RvDec_Get_Weighted_Average;
extern uint8 g_int_bfr[21*MB_SIZE*2];

// bottom linebuffer
extern uint8	*g_bottom_line_bfr[3];

//store current mb's top and left refernce pixel; 
extern uint8 *luma_top/*[17+4]*/, luma_left[16];
extern uint8 *chroma_top[2]/*[9+4]*/, chroma_left[2][8];
extern uint8 top_left[3];

void foo();

extern const uint8 g_dec_aic_prob_rv8[900/*A*B*prob*/];
extern const int32 g_NewTAB_DQUANT_MQ_RV89[32][2];
extern const uint16 g_aic_table_index[20];
extern const uint8 g_mbtype_p_tab[12];
extern const uint8 g_mbtype_b_enc_tab[12];

extern RV_VLC_SET_T g_intra_decode_tables[MAX_INTRA_QP_REGIONS];
extern RV_VLC_SET_T g_inter_decode_tables[MAX_INTER_QP_REGIONS];

extern BYTE_VLC_SYMBOL_T g_aic_top_vlc;
extern BYTE_VLC_SYMBOL_T g_aic_mode1_vlc[AIC_MODE1_NUM];
extern SHORT_VLC_SYMBOL_T g_aic_mode2_vlc[AIC_MODE2_NUM];
extern BYTE_VLC_SYMBOL_T g_ptype_vlc[NUM_PTYPE_VLCS];
extern BYTE_VLC_SYMBOL_T g_btype_vlc[NUM_BTYPE_VLCS];
extern const uint8 g_quant_to_vlc_set[2][31];
extern const uint8 modulo_three_table[108][4];
extern const int32 g_BquantTable[32];
extern const int32 Above1VOnEdgeList[1];
extern const int32 Above1VList[5];
extern const int32 Above4VOnEdgeList[5];
extern const int32 Above4VList[9];
extern const int32 Left1VOnEdgeList[1];
extern const int32 Left1VList[5];
extern const int32 Left4VOnEdgeList[5];
extern const int32 Left4VList[9];

extern const int16 g_intra_cbppattern_tbl_0_0[1818][2];
extern const int16 g_intra_cbppattern_tbl_0_1[1804][2];
extern const int16 g_intra_cbppattern_tbl_1_0[1810][2];
extern const int16 g_intra_cbppattern_tbl_1_1[1786][2];
extern const int16 g_intra_cbppattern_tbl_2_0[1770][2];
extern const int16 g_intra_cbppattern_tbl_2_1[1806][2];
extern const int16 g_intra_cbppattern_tbl_3_0[1734][2];
extern const int16 g_intra_cbppattern_tbl_3_1[1782][2];
extern const int16 g_intra_cbppattern_tbl_4_0[1748][2];
extern const int16 g_intra_cbppattern_tbl_4_1[1790][2];
extern const int16 g_intra_second_pattern_tbl_0_0[522][2];
extern const int16 g_intra_second_pattern_tbl_0_1[554][2];
extern const int16 g_intra_second_pattern_tbl_1_0[526][2];
extern const int16 g_intra_second_pattern_tbl_1_1[590][2];
extern const int16 g_intra_second_pattern_tbl_2_0[546][2];
extern const int16 g_intra_second_pattern_tbl_2_1[630][2];
extern const int16 g_intra_second_pattern_tbl_3_0[566][2];
extern const int16 g_intra_second_pattern_tbl_3_1[684][2];
extern const int16 g_intra_second_pattern_tbl_4_0[632][2];
extern const int16 g_intra_second_pattern_tbl_4_1[676][2];
extern const int16 g_intra_third_pattern_tbl_0_0[536][2];
extern const int16 g_intra_third_pattern_tbl_0_1[570][2];
extern const int16 g_intra_third_pattern_tbl_1_0[550][2];
extern const int16 g_intra_third_pattern_tbl_1_1[636][2];
extern const int16 g_intra_third_pattern_tbl_2_0[572][2];
extern const int16 g_intra_third_pattern_tbl_2_1[670][2];
extern const int16 g_intra_third_pattern_tbl_3_0[640][2];
extern const int16 g_intra_third_pattern_tbl_3_1[656][2];
extern const int16 g_intra_third_pattern_tbl_4_0[700][2];
extern const int16 g_intra_third_pattern_tbl_4_1[646][2];
extern const uint8 g_intra_cbp_tbl_0_0_0[64][2];
extern const uint8 g_intra_cbp_tbl_0_0_1[32][2];
extern const uint8 g_intra_cbp_tbl_0_0_2[32][2];
extern const uint8 g_intra_cbp_tbl_0_0_3[64][2];
extern const uint8 g_intra_cbp_tbl_0_1_0[256][2];
extern const uint8 g_intra_cbp_tbl_0_1_1[64][2];
extern const uint8 g_intra_cbp_tbl_0_1_2[32][2];
extern const uint8 g_intra_cbp_tbl_0_1_3[32][2];
extern const uint8 g_intra_cbp_tbl_1_0_0[32][2];
extern const uint8 g_intra_cbp_tbl_1_0_1[32][2];
extern const uint8 g_intra_cbp_tbl_1_0_2[32][2];
extern const uint8 g_intra_cbp_tbl_1_0_3[64][2];
extern const uint8 g_intra_cbp_tbl_1_1_0[256][2];
extern const uint8 g_intra_cbp_tbl_1_1_1[64][2];
extern const uint8 g_intra_cbp_tbl_1_1_2[32][2];
extern const uint8 g_intra_cbp_tbl_1_1_3[32][2];
extern const uint8 g_intra_cbp_tbl_2_0_0[32][2];
extern const uint8 g_intra_cbp_tbl_2_0_1[32][2];
extern const uint8 g_intra_cbp_tbl_2_0_2[32][2];
extern const uint8 g_intra_cbp_tbl_2_0_3[64][2];
extern const uint8 g_intra_cbp_tbl_2_1_0[256][2];
extern const uint8 g_intra_cbp_tbl_2_1_1[64][2];
extern const uint8 g_intra_cbp_tbl_2_1_2[32][2];
extern const uint8 g_intra_cbp_tbl_2_1_3[64][2];
extern const uint8 g_intra_cbp_tbl_3_0_0[32][2];
extern const uint8 g_intra_cbp_tbl_3_0_1[32][2];
extern const uint8 g_intra_cbp_tbl_3_0_2[32][2];
extern const uint8 g_intra_cbp_tbl_3_0_3[32][2];
extern const uint8 g_intra_cbp_tbl_3_1_0[128][2];
extern const uint8 g_intra_cbp_tbl_3_1_1[64][2];
extern const uint8 g_intra_cbp_tbl_3_1_2[32][2];
extern const uint8 g_intra_cbp_tbl_3_1_3[32][2];
extern const uint8 g_intra_cbp_tbl_4_0_0[128][2];
extern const uint8 g_intra_cbp_tbl_4_0_1[32][2];
extern const uint8 g_intra_cbp_tbl_4_0_2[32][2];
extern const uint8 g_intra_cbp_tbl_4_0_3[32][2];
extern const uint8 g_intra_cbp_tbl_4_1_0[128][2];
extern const uint8 g_intra_cbp_tbl_4_1_1[64][2];
extern const uint8 g_intra_cbp_tbl_4_1_2[32][2];
extern const uint8 g_intra_cbp_tbl_4_1_3[32][2];
extern const int16 g_intra_first_pattern_tbl_0_0[1294][2];
extern const int16 g_intra_first_pattern_tbl_0_1[1270][2];
extern const int16 g_intra_first_pattern_tbl_0_2[1330][2];
extern const int16 g_intra_first_pattern_tbl_0_3[1394][2];
extern const int16 g_intra_first_pattern_tbl_1_0[1344][2];
extern const int16 g_intra_first_pattern_tbl_1_1[1278][2];
extern const int16 g_intra_first_pattern_tbl_1_2[1334][2];
extern const int16 g_intra_first_pattern_tbl_1_3[1320][2];
extern const int16 g_intra_first_pattern_tbl_2_0[1386][2];
extern const int16 g_intra_first_pattern_tbl_2_1[1328][2];
extern const int16 g_intra_first_pattern_tbl_2_2[1338][2];
extern const int16 g_intra_first_pattern_tbl_2_3[1408][2];
extern const int16 g_intra_first_pattern_tbl_3_0[1368][2];
extern const int16 g_intra_first_pattern_tbl_3_1[1330][2];
extern const int16 g_intra_first_pattern_tbl_3_2[1348][2];
extern const int16 g_intra_first_pattern_tbl_3_3[1382][2];
extern const int16 g_intra_first_pattern_tbl_4_0[1404][2];
extern const int16 g_intra_first_pattern_tbl_4_1[1412][2];
extern const int16 g_intra_first_pattern_tbl_4_2[1346][2];
extern const int16 g_intra_first_pattern_tbl_4_3[1400][2];
extern const int16 g_intra_coefficient_tbl_0[582][2];
extern const int16 g_intra_coefficient_tbl_1[646][2];
extern const int16 g_intra_coefficient_tbl_2[642][2];
extern const int16 g_intra_coefficient_tbl_3[642][2];
extern const int16 g_intra_coefficient_tbl_4[646][2];
extern const int16 g_inter_cbppattern_tbl_0[1778][2];
extern const int16 g_inter_cbppattern_tbl_1[1714][2];
extern const int16 g_inter_cbppattern_tbl_2[1760][2];
extern const int16 g_inter_cbppattern_tbl_3[1808][2];
extern const int16 g_inter_cbppattern_tbl_4[1758][2];
extern const int16 g_inter_cbppattern_tbl_5[1796][2];
extern const int16 g_inter_cbppattern_tbl_6[1822][2];
extern const uint8 g_inter_cbp_tbl_0_0[64][2];
extern const uint8 g_inter_cbp_tbl_0_1[64][2];
extern const uint8 g_inter_cbp_tbl_0_2[128][2];
extern const uint8 g_inter_cbp_tbl_0_3[128][2];
extern const uint8 g_inter_cbp_tbl_1_0[64][2];
extern const uint8 g_inter_cbp_tbl_1_1[64][2];
extern const uint8 g_inter_cbp_tbl_1_2[64][2];
extern const uint8 g_inter_cbp_tbl_1_3[128][2];
extern const uint8 g_inter_cbp_tbl_2_0[64][2];
extern const uint8 g_inter_cbp_tbl_2_1[32][2];
extern const uint8 g_inter_cbp_tbl_2_2[64][2];
extern const uint8 g_inter_cbp_tbl_2_3[128][2];
extern const uint8 g_inter_cbp_tbl_3_0[64][2];
extern const uint8 g_inter_cbp_tbl_3_1[32][2];
extern const uint8 g_inter_cbp_tbl_3_2[64][2];
extern const uint8 g_inter_cbp_tbl_3_3[128][2];
extern const uint8 g_inter_cbp_tbl_4_0[32][2];
extern const uint8 g_inter_cbp_tbl_4_1[32][2];
extern const uint8 g_inter_cbp_tbl_4_2[64][2];
extern const uint8 g_inter_cbp_tbl_4_3[128][2];
extern const uint8 g_inter_cbp_tbl_5_0[32][2];
extern const uint8 g_inter_cbp_tbl_5_1[32][2];
extern const uint8 g_inter_cbp_tbl_5_2[32][2];
extern const uint8 g_inter_cbp_tbl_5_3[64][2];
extern const uint8 g_inter_cbp_tbl_6_0[32][2];
extern const uint8 g_inter_cbp_tbl_6_1[32][2];
extern const uint8 g_inter_cbp_tbl_6_2[32][2];
extern const uint8 g_inter_cbp_tbl_6_3[32][2];
extern const int16 g_inter_first_pattern_tbl_0_0[1232][2];
extern const int16 g_inter_first_pattern_tbl_0_1[1390][2];
extern const int16 g_inter_first_pattern_tbl_1_0[1242][2];
extern const int16 g_inter_first_pattern_tbl_1_1[1364][2];
extern const int16 g_inter_first_pattern_tbl_2_0[1310][2];
extern const int16 g_inter_first_pattern_tbl_2_1[1356][2];
extern const int16 g_inter_first_pattern_tbl_3_0[1310][2];
extern const int16 g_inter_first_pattern_tbl_3_1[1358][2];
extern const int16 g_inter_first_pattern_tbl_4_0[1376][2];
extern const int16 g_inter_first_pattern_tbl_4_1[1388][2];
extern const int16 g_inter_first_pattern_tbl_5_0[1394][2];
extern const int16 g_inter_first_pattern_tbl_5_1[1348][2];
extern const int16 g_inter_first_pattern_tbl_6_0[1384][2];
extern const int16 g_inter_first_pattern_tbl_6_1[1442][2];
extern const int16 g_inter_second_pattern_tbl_0_0[512][2];
extern const int16 g_inter_second_pattern_tbl_0_1[564][2];
extern const int16 g_inter_second_pattern_tbl_1_0[518][2];
extern const int16 g_inter_second_pattern_tbl_1_1[576][2];
extern const int16 g_inter_second_pattern_tbl_2_0[528][2];
extern const int16 g_inter_second_pattern_tbl_2_1[608][2];
extern const int16 g_inter_second_pattern_tbl_3_0[562][2];
extern const int16 g_inter_second_pattern_tbl_3_1[696][2];
extern const int16 g_inter_second_pattern_tbl_4_0[582][2];
extern const int16 g_inter_second_pattern_tbl_4_1[694][2];
extern const int16 g_inter_second_pattern_tbl_5_0[688][2];
extern const int16 g_inter_second_pattern_tbl_5_1[674][2];
extern const int16 g_inter_second_pattern_tbl_6_0[668][2];
extern const int16 g_inter_second_pattern_tbl_6_1[672][2];
extern const int16 g_inter_third_pattern_tbl_0_0[522][2];
extern const int16 g_inter_third_pattern_tbl_0_1[584][2];
extern const int16 g_inter_third_pattern_tbl_1_0[536][2];
extern const int16 g_inter_third_pattern_tbl_1_1[610][2];
extern const int16 g_inter_third_pattern_tbl_2_0[570][2];
extern const int16 g_inter_third_pattern_tbl_2_1[692][2];
extern const int16 g_inter_third_pattern_tbl_3_0[584][2];
extern const int16 g_inter_third_pattern_tbl_3_1[664][2];
extern const int16 g_inter_third_pattern_tbl_4_0[692][2];
extern const int16 g_inter_third_pattern_tbl_4_1[650][2];
extern const int16 g_inter_third_pattern_tbl_5_0[674][2];
extern const int16 g_inter_third_pattern_tbl_5_1[644][2];
extern const int16 g_inter_third_pattern_tbl_6_0[650][2];
extern const int16 g_inter_third_pattern_tbl_6_1[640][2];
extern const int16 g_inter_coefficient_tbl_0[648][2];
extern const int16 g_inter_coefficient_tbl_1[646][2];
extern const int16 g_inter_coefficient_tbl_2[642][2];
extern const int16 g_inter_coefficient_tbl_3[642][2];
extern const int16 g_inter_coefficient_tbl_4[640][2];
extern const int16 g_inter_coefficient_tbl_5[642][2];
extern const int16 g_inter_coefficient_tbl_6[642][2];

extern const uint8 g_aic_top_vlc_tbl[256][2]; 
extern const int16 g_aic_mode2_vlc_tbl_0[614][2]; 
extern const int16 g_aic_mode2_vlc_tbl_1[608][2]; 
extern const int16 g_aic_mode2_vlc_tbl_2[572][2]; 
extern const int16 g_aic_mode2_vlc_tbl_3[616][2]; 
extern const int16 g_aic_mode2_vlc_tbl_4[604][2]; 
extern const int16 g_aic_mode2_vlc_tbl_5[572][2]; 
extern const int16 g_aic_mode2_vlc_tbl_6[616][2]; 
extern const int16 g_aic_mode2_vlc_tbl_7[590][2]; 
extern const int16 g_aic_mode2_vlc_tbl_8[590][2]; 
extern const int16 g_aic_mode2_vlc_tbl_9[584][2]; 
extern const int16 g_aic_mode2_vlc_tbl_10[576][2]; 
extern const int16 g_aic_mode2_vlc_tbl_11[596][2]; 
extern const int16 g_aic_mode2_vlc_tbl_12[578][2]; 
extern const int16 g_aic_mode2_vlc_tbl_13[576][2]; 
extern const int16 g_aic_mode2_vlc_tbl_14[572][2]; 
extern const int16 g_aic_mode2_vlc_tbl_15[580][2]; 
extern const int16 g_aic_mode2_vlc_tbl_16[586][2]; 
extern const int16 g_aic_mode2_vlc_tbl_17[612][2]; 
extern const int16 g_aic_mode2_vlc_tbl_18[570][2]; 
extern const int16 g_aic_mode2_vlc_tbl_19[602][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_0[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_1[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_2[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_3[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_4[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_5[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_6[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_7[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_8[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_10[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_11[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_12[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_13[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_14[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_15[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_16[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_17[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_18[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_20[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_21[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_22[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_23[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_24[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_25[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_26[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_27[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_28[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_30[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_31[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_32[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_33[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_34[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_35[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_36[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_37[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_38[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_40[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_41[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_42[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_43[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_44[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_45[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_46[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_47[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_48[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_50[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_51[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_52[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_53[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_54[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_55[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_56[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_57[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_58[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_60[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_61[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_62[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_63[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_64[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_65[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_66[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_67[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_68[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_70[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_71[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_72[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_73[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_74[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_75[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_76[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_77[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_78[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_80[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_81[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_82[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_83[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_84[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_85[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_86[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_87[128][2]; 
extern const uint8 g_aic_mode1_vlc_tbl_88[128][2]; 
extern const uint8 g_ptype_vlc_tbl_0[128][2]; 
extern const uint8 g_ptype_vlc_tbl_1[128][2]; 
extern const uint8 g_ptype_vlc_tbl_2[128][2]; 
extern const uint8 g_ptype_vlc_tbl_3[128][2]; 
extern const uint8 g_ptype_vlc_tbl_4[128][2]; 
extern const uint8 g_ptype_vlc_tbl_5[128][2]; 
extern const uint8 g_ptype_vlc_tbl_6[128][2]; 
extern const uint8 g_btype_vlc_tbl_0[64][2]; 
extern const uint8 g_btype_vlc_tbl_1[64][2]; 
extern const uint8 g_btype_vlc_tbl_2[64][2]; 
extern const uint8 g_btype_vlc_tbl_3[64][2]; 
extern const uint8 g_btype_vlc_tbl_4[64][2]; 
extern const uint8 g_btype_vlc_tbl_5[64][2]; 

extern const uint8 gs_VLCDecodeTable[256];
extern const uint8 g_interleaved_golomb_vlc[256][2];

extern uint8 *g_rgiRvClipTab;
#if defined(SC6800H_VIDEO_ROMCODE)
extern const uint32 *g_msk_rvdec_ptr;
extern const uint8 *g_clip_tbl_rvdec_ptr;
#else
extern uint32 g_msk_rvdec_ptr[33];
extern uint8 g_clip_tbl_rvdec_ptr[CLIP_TAB_SIZE];
#endif

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif  //_RVDEC_GLOBAL_H_
