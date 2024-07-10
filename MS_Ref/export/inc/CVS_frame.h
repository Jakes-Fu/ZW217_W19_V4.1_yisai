/*******************************************************************
**  File Name:     CVSI_frame.h									   *
**  Author:        Tan LI										   *
**  Date:         04/09/2011                                       *
**  Copyright:    2011 Spreadtrum, Incorporated. All Rights Reserved.*
**  Description:   .											   *
********************************************************************
********************************************************************
**  Edit History                                                   *
**-----------------------------------------------------------------*
**  DATE            NAME            DESCRIPTION                    *
**  04/09/2011     Tan LI        Create.						   *
**  05/16/2012     Sheng Wu      Migrate to speech enhance.	       *
**  11/13/2012     Sheng Wu      Modified to float point.		   *
*******************************************************************/
#ifndef CVSF_FRAME_H
#define CVSF_FRAME_H


#include "CVS_typeDefs.h"
#include "CVS_fft.h"
#include "CVS_log2.h"

//#define SP_PROTECT_FRM 100
//#define SP_PROTECT_THR 100
//#define EC_PROTECT_THR 10

typedef struct
{	
	///parameters
	int32 sm[3];
	int32 alpha;
	int32 thr_cvs;
	///data
	int32 full_vad;	
	int32 Sc	[(BASE_N>>1)];
	int32 Ss	[(BASE_N>>1)];
	int32 G		[(BASE_N>>1)];
	int32 pSNR	[(BASE_N>>1)];
	int32 rateL	[MAX_SUBS];
	int32 vads	[MAX_SUBS];
	int32 Sf	[(BASE_N>>1)];
}CVSI_VAD;


typedef struct
{
	///data
	int32 *Ea;		//[BASE_N+1];	//premary   channel
	int32 *Eb;		//[BASE_N+1];	//secondary channel
	int32 amp1		[(BASE_N >> 1)]; //channel eaqualization premary   channel
	int32 amp2		[(BASE_N >> 1)]; //channel eaqualization secondary channel
	int32 chnDiff	[(BASE_N >> 1)];
	int32 eSum1;
	int32 eSum2;
	int32 sSum1;
	int32 sSum2;

	int32 msa;

	int32 NF_fix;
	int32 thr_fix;
	///parameters
	int32 Gain_limit;
	int32 NFloor;
	int32 NF_fade;
	int32 thr_cvs;
	int32 thr_cvs_sub;
	int32 VAD_Gain_limit;
	int32 G_SRise;
	int32 G_SFall;
	int32 G_NRise;
	int32 G_NFall;
	int32 alpha_S;
	int32 alpha_N;
	int32 sm_S[3];
	int32 sm_N[3];
	
	int32 vad_sm[3];
	int32 vad_alpha;
	int32 vad_thr;
	int32 pos_mode;
	int32 out_mode;
	int32 GAIN_COMP;
}CVSI_DUAL_POST;


typedef struct
{
	///data
	int32 *Ya;		//AEC filter out reference
	int32 *Yay2;
	int32 *Yb;		//echo in mic signal
	int32 *Yc;		//linear AEC output
	int32 *Sa;		//original pre channel

	int32 sm[3][MAX_SUBS];
	int32 alpha[MAX_SUBS];
	int32 NLP;		//NNP switch flag
	int32 NLPP;		//NNP flag per frame
	int32 NLPS[MAX_SUBS];

	int32 speech_flag[NFRAME];

	int32 NF_fix;
	int32 thr_fix;
	int32 msa;
	int32 msaY2;
	int32 sa_sum;
	int32 meanfl;
	///parameters
	int32 Gain_limit;
	int32 NFloor;
	int32 NF_fade;
	int32 thr_cvs;
	int32 thr_cvs_sub;

	int32 G_SRise;
	int32 G_SFall;
	int32 G_NRise;
	int32 G_NFall;
	int32 alpha_S;
	int32 alpha_N;
	int32 sm_S[3];
	int32 sm_N[3];
	int32 SPP_range;

	int32 tone_purity;
	int32 tone_cf1;
	int32 tone_cf2;
	int32 tone_cf3;
	int32 tone_fw1;
	int32 tone_fw2;
	int32 tone_fw3;
	int32 newNLP_mode1;   //a weak suppression [4,20)
	int32 newNLP_mode2;   //a strong suppressin [2,10)
	int32 newNLP_Rx_thr1; //weak suppression rx threshold [20,2000)
	int32 newNLP_Rx_thr2; //strong suppression rx threshold [20,2000)
}CVSI_MONO_POST;

typedef struct
{
	///data
	//int32 W	[(BASE_N >> 1) * 2];
	//int32 ew[(BASE_N >> 1) * 3];
	//int32 ma[(BASE_N >> 1) * 1];
	int32 *W;
	int32 *ew;
	int32 *ma;

	int32 *Ew;		//[(BASE_N+1)];		//T USED as CTC_ptr->Ew
	int32 *Ea;		//[(BASE_N>>1)+1];	//T USED as CTC_ptr->Ea
	int32 *En;		//phase of Ew

	int32 *XdB;
	int32 *XodB;
	int32 *nBin;
	int32 *ratios;

	int32 RxTxR;					//Rx to Tx ampli Ratio
	int32 RxTxR_CMP;				//compensation of Rx to Tx ampli Ratio
	int32 AEC_update;
	int32 AEC_LOSS;
	int32 AEC_EL_thr;			//echo loss ratio
	int32 AEC_ADFFL;			//echo loss ratio
	
	int32 cutoff;

	int32 s1_sum;
	int32 s2_sum;
	int32 yo_sum;
	int32 yr_sum;
}CVSI_AEC;

typedef struct
{	///data
	int32 offset;
	int32 depth;
	int32 rangeFreqS;
	int32 rangeFreqE;
	int32 frameThr;
	int32 echoGain;

	int32 IDX;
	int32 valid;
	int32 TDE;
	int32 MAX;
	int32 *XR;
	int32 *HL;
	// int32 XR[514 * (MAX_TDE - 1)];// [BASE_N  * (MAX_TDE - 1)];
	// int32 HL[320 * MAX_TDE];// [BASE_M  * MAX_TDE];

}CVSI_TDE;

//internal exchange buffer
typedef struct
{
	int32 Xa[(BASE_N >> 1) * 2];//[(BASE_N+1)];
	int32 XaY2[(BASE_N >> 1) * 2];//[(BASE_N+1)];

	int32 Xw[(BASE_N >> 1) * 4];//[(BASE_N+1)*2];
	int32 XwY2[(BASE_N >> 1) * 4];//[(BASE_N+1)*2];
	int32 Ea[(BASE_N >> 1) * 2];//[(BASE_N+1)];
	int32 EaY2[(BASE_N >> 1) * 2];//[(BASE_N+1)];
	int32 Ew[(BASE_N >> 1) * 4];//[(BASE_N+1)*2];
	int32 EwY2[(BASE_N >> 1) * 4];//[(BASE_N+1)*2];
	int32 dW  [(BASE_N >> 1) * 2];
	int32 dWY2[(BASE_N >> 1) * 2];
	int32 rW  [(BASE_N >> 1) * 2];

	int32 XdB[(BASE_N >> 1) * 2];//[(BASE_N+1)];
	int32 XodB[(BASE_N >> 1) * 2];
	int32 nBin[(BASE_N >> 1) * 2];//[(BASE_N+1)];
	int32 ratios[(BASE_N >> 1) * 2];//[(BASE_N+1)];

	int32 dy[(BASE_N >> 1) * 2];
	int32 dyY2[(BASE_N >> 1) * 2];

	int32 YaY2[(BASE_N >> 1) * 3];
	int32 Ya[(BASE_N >> 1) * 3];

	int32 *XR;//[(BASE_N >> 1) * 8];	//T USED *Sr1 *Sr2//rectangle fft reference spectrum 
	int32 *DR;//[(BASE_N >> 1) * 4];	//T
	int32 *DX;//[(BASE_N >> 1) * 2];	//T


	int32 CTCAECBUF[(BASE_N >> 1) * 36];
	int32 CTCAECBUFY2[(BASE_N >> 1) * 12];

#ifdef F_BAND
	int32 EX[(BASE_N >> 1) * 2];	//for fft1536 temp
#endif
}CVSI_RX;

//cross talk canceller for core dual mic algorithm
typedef struct
{
	//int32 ctc_W1	[(BASE_N >> 1) * 2];
	//int32 ctc_W2	[(BASE_N >> 1) * 2];
	//int32 ctc_ew1	[(BASE_N >> 1) * 2];
	//int32 ctc_ew2	[(BASE_N >> 1) * 2];
	//int32 ctc_ma1	[(BASE_N >> 1) * 1];
	//int32 ctc_ma2	[(BASE_N >> 1) * 1];
	int32 *ctc_W1;
	int32 *ctc_W2;
	int32 *ctc_ew1;
	int32 *ctc_ew2;
	int32 *ctc_ma1;
	int32 *ctc_ma2;

	int32 *Sw1;//CVSI_ptr->RX_ptr->Xw;				int32 Sw1[(BASE_N+1)*2];
	int32 *Sa1;//CVSI_ptr->RX_ptr->Xw+(BASE_N+1)*2;	int32 Sa1[(BASE_N+1)*1];
	int32 *Sw2;//CVSI_ptr->RX_ptr->Xa;				int32 Sw2[(BASE_N+1)*2];
	int32 *Sa2;//CVSI_ptr->RX_ptr->Xa+(BASE_N+1)*1;	int32 Sa2[(BASE_N+1)*1];	
	int32 *Sr1;//CVSI_ptr->RX_ptr->XR     [(BASE_N+1)*2];	//rectangle fft reference spectrum 1
	int32 *Sr2;//CVSI_ptr->RX_ptr->XR+2N+2[(BASE_N+1)*2];	//rectangle fft reference spectrum 2

	int32 Swl2[(BASE_N>>1)*2];
	
	int32 *Ew1;//int32 Ew1[(BASE_N+1)*2];//*AEC->Ew[(BASE_N+1)*2];
	int32 *Ew2;//int32 Ew2[(BASE_N+1)*2];//*AEC->Ew[(BASE_N+1)*2];
	int32 *Ea1;//int32 Ea1[(BASE_N+1)*1];//*AEC->Ea[(BASE_N+1)*1];
	int32 *Ea2;//int32 Ea2[(BASE_N+1)*1];//*AEC->Ea[(BASE_N+1)*1];
	//int32 *En1;//phase of Ew1[(BASE_N+1)*2];
	//int32 *En2;//phase of Ew2[(BASE_N+1)*2];

	int32 sum1Ori;
	int32 sum2Ori;
	int32 sum1Pos;
	int32 sum2Pos;
	int32 eGainCmp;
	int32 sGainCmp;
	//int32 CTC_LOSS;
	int32 CTC_RATE;
	//CTC parameters
	int32 ctc_vad;
	int32 ctc_aec;

	int32 CTC_LK1;
	int32 CTC_LK2;
	int32 CTC_FL;
	int32 CTC_VAD_FS;
	int32 CTC_VAD_FE;
	int32 CTC_VAD_COMP;

	int32 isPremary;
//	int32 W1_CRC_reset_cnt;
//	int32 W2_CRC_reset_cnt;

	int32 *G1;
	int32 *G2;

}CVSI_CTC;

typedef struct
{
	//DRC data
	int32 drc_para[9];

	int32 drc_p1_pos;
	int32 drc_p1_amp;
	int32 drc_p2_pos;
	int32 drc_p2_amp;
	int32 drc_s_pos;
	int32 drc_e_lim;
	int32 drc_sm;
	int32 drc_gain;
}CVSI_DRC;

typedef struct
{
	int32 AEC_P_EL_THR;
	int32 AEC_S_EL_THR;
	int32 NLP_UPDATE;
	int32 NLP_NLE_Gain;
	int32 NLP_NLF_Gain;
	int32 NLP_thr_L1;
	int32 NLP_thr_L2;
	int32 NLP_sub_L1;
	int32 NLP_sub_L2;
}CVSI_NES;

typedef struct
{
	//High pass para
	int32 cutoff;
	int32 cutbin;
	//int32 highmap[32];
}CVSI_HPF;

typedef struct
{
	//High pass para
	int32 EQBin[36];
	int32 EQMap[(BASE_N>>1)];
}CVSI_EQF;

typedef struct
{
	int32 CHN;//0 for bypass, 1 for mono enhance, 2 for dual tx enhance	
	int32 AEC;//AEC switch hand hold
	int32 POS;//mono pos switch hand hold
	int32 EQF;//EQ filter switch hand hold
	int32 NES;//nonlinear echo suppression switch hand hold
	int32 HPF;//high pass filter switch hand hold
	int32 HEF;//human enhance filter switch hand hold
	int32 SPP;//custom filter switch hand hold
	int32 DRC;//dynamic range control switch hand hold
}CVSI_MOD;

//20200305 WJI
typedef struct{
	int32 para1;
	int32 para2;
	int32 para3;
	int32 alpha;
	int32 eta0;
	int32 alphas;
	int32 Bmin_rec;
	int32 zeta0;
	int32 gamma1;
	int32 alphad;
	int32 alphad_long;
	int32 xi_min;
	int32 beta;
} NS_PARA_IN; 

typedef struct{
	int32 S[(BASE_N>>1)];
	int32 lambdad_hat[(BASE_N>>1)];
	int32 lambdad_bar[(BASE_N>>1)];
	int32 p_hat[(BASE_N>>1)];
	int32 q_hat[(BASE_N>>1)];
	int32 gamma[(BASE_N>>1)];
	int32 xi[(BASE_N>>1)];
	int32 nu[(BASE_N>>1)];
	int32 Sf[(BASE_N>>1)+NUM_B-1];
	int32 gamma_min_tilde[(BASE_N>>1)];
	int32 zeta_tilde[(BASE_N>>1)];
	int32 alphad_tilde[(BASE_N>>1)];
	int32 G[(BASE_N>>1)];
	int32 G_term[(BASE_N>>1)];
} NS_PARA_ALGO;

typedef struct  
{
	int32 gammas ;
	int32 gammal;
	int32 Ea[(BASE_N >> 1) * 2];
	int32 DTD_shortT;
	int32 DTD_longT;
	int32 DTD_Val;   //the result of the dtd value
	int32 DTD_thr1; //Decide whether to reduce noise (DTD_Val>DTD_thr1?)
	int32 DTD_thr2; //Decide whether to set zero the output (DTD_Val>DTD_thr2?)
	int32 DTD_cnt;
	int32 DTD_cnt1;
	int32 DTD_cnt2;
	int32 tmpNES;
	int32 tmpPOS;
//	int32 tmpNLP_UPDATE;
//	int32 tmpNLP_thr_L1;
//	int32 tmpNLP_thr_L2;
//	int32 tmpNLP_sub_L1;
//	int32 tmpNLP_sub_L2;
	int32 rx_level;
	int32 DTD_Rx_thr; //Decide whether to set zero according to the rx-level
	int32 DTD_Calc_Fre; //base on low frequency calculate the dtd(200Hz,1000Hz)
	int32 sub_ob_switch;
	int32 envir_init;
}CVSI_MONO_DTD;

typedef struct
{
	int32	mode_switch;

	int32   band_mode;// narrow band=0, wideband=1
	int32   forceNBloop;
	int32	sample_rate;
	int32	sample_pms;
	int32   stepLen;

	int32   isSpk;
	int32	volume;
	int32	spk_init;  
	int32   spk_mode;
	int32   spk_mode_cnt;

	CVSI_MOD *TX_MODUAL;
	CVSI_MOD *RX_MODUAL;

	int32	cnt_tx;
	int32	cnt_rx;

	//AEC data
	int32 AEC_ADF_FL;
	int32 NLP_rt_1;
	int32 NLP_rt_2;
	int32 NLP_gain_1;
	int32 NLP_gain_2;
	int32 NLP_gain_1_bk;
	int32 NLP_gain_2_bk;
	CVSI_NES *NES;
	CVSI_NES *NES_VOL[9];

	//AEC para
	int32 dl_AEC;
	int32 dl_CTC;
	int32 RxTxR_thr;
	int32 CNG_fade;
	int32 AEC_BUFL;
	int32 AEC_fade_mode;
	int32 AEC_fade_time;
	//CTC para
	int32 CTC_CL1_THR;
	int32 CTC_CL2_THR;
	int32 CHN_EQ_THR;

	int32 Speed_tx;	// 100
	int32 Speed_rx;	// 100
	//SPP para
	int32 SPP_frm_tx;	// 100
	int32 SPP_thr_tx;	// 100
	int32 SPP_frm_rx;	// 100
	int32 SPP_thr_rx;	// 100
	int32 ECP_thr;	// reused for ADF mode 

	//CNG para
	int32 TX_CNG_FL;
	int32 RX_CNG_FL;
	int32 TX_CNG_FD;
	int32 RX_CNG_FD;

	//NS+ para
	int32 TX_NSP_GAIN;
	int32 RX_NSP_GAIN;

	//SPP data
	int32 y1_sum;
	int32 y2_sum;
	int32 y1_LTM;
	int32 y2_LTM;

	int32 rf_sum;
	int32 nf_sum;
	int32 SPP_tx;
	int32 SPP_rx;
	int32 min_tx1;
	int32 min_tx2;
	int32 min_rx;
	int32 dc_tx1;
	int32 dc_tx2;
	int32 dc_rx;
	int32 dc_rf;

//	int32 TX_MIN_IDX;
//	int32 RX_MIN_IDX;
	int32 TX_nf_dc;
//	int32 nlp1_dc;
//	int32 nlp2_dc;
	int32 max_rx;

	FFTI_INFO   *FFT_ptr;

	FFTI_INFO   *NB_FFT_ptr;
	FFTI_INFO   *WB_FFT_ptr;
	FFTI_INFO   *SB_FFT_ptr;
	FFTI_INFO   *FB_FFT_ptr;

	CVSI_DRC	*DRC_tx;
	CVSI_DRC	*DRC_rx;
	CVSI_HPF	*HPF_tx;
	CVSI_HPF	*HPF_rx;
	CVSI_EQF	*EQF_tx;
	CVSI_EQF	*EQF_rx;

	CVSI_VAD    *VADm_ptr;
	CVSI_VAD	*VADr_ptr;

	CVSI_AEC    *AEC1_ptr;
	CVSI_AEC    *AEC2_ptr;
	CVSI_AEC    *AEC3_ptr;
	CVSI_AEC    *AEC4_ptr;

	CVSI_RX		*RX_ptr;
	CVSI_CTC    *CTC_ptr;
	CVSI_TDE    *TDE_ptr;
	CVSI_MONO_DTD *DTD_ptr;

	CVSI_DUAL_POST  *DUAL_POST_ptr;
	CVSI_MONO_POST  *TXMO_POST_ptr;
	CVSI_MONO_POST  *RXMO_POST_ptr;

	NS_PARA_ALGO	*ns_rx_algo_ptr;
	NS_PARA_ALGO	*ns_tx_algo_ptr;
	NS_PARA_IN		*ns_rx_para_input_ptr;
	NS_PARA_IN		*ns_tx_para_input_ptr;

	//buffers
	int32 y1_buf[(BASE_N>>1)*3];//mic  1 data buffer
	int32 y2_buf[(BASE_N>>1)*3];//mic  2 data buffer
	int32 rx_buf[(BASE_N>>1)*2];//raw rx data buffer

	int32 rx_ref[(8+1)*BASE_N];//reference for aec 64ms max delay +3 overlapbuf +1 nbframebuf
	int32 rx_nlm[(BASE_N >> 1) * 4];//[(8+1)*BASE_N];//nonlinear mapped reference

	int32 tx_syn[BASE_K];
	int32 *tx_amp;//tx_amp[((BASE_N>>1)+1)];//
	int32 *tx_amp_y2;
	int32 *tx_ang;//tx_ang[BASE_N+2];//
	int32 *tx_tmp;//tx_tmp[BASE_N+2];//

	int32 rx_syn[BASE_K];
	int32 *rx_amp;//rx_amp[((BASE_N>>1)+1)];//
	int32 *rx_ang;//rx_ang[BASE_N+2];//
	int32 *rx_tmp;//rx_tmp[BASE_N+2];//
	int16 tdns[(BASE_N >> 4) + (BASE_N >> 6)];
	//int32 Ya	[BASE_N+1];		//T
	//int32 Ea	[BASE_N+1];

	int32 Ss_tx 		[(BASE_N>>1)];	//reference noise
	int32 G_tx			[(BASE_N>>1)];	//enhance filter without smoothing	
	int32 Sc_tx 		[(BASE_N>>1)];	//old equalized filtered ref channel
	int32 pSNR_tx		[(BASE_N>>1)];
//	int32 Sc_tx_Y2 		[(BASE_N>>1)];
	int32 Ss_rx 		[(BASE_N>>1)];	//reference noise
	int32 G_rx			[(BASE_N>>1)];	//enhance filter without smoothing	
	int32 Sc_rx 		[(BASE_N>>1)];	//old equalized filtered ref channel
	int32 pSNR_rx		[(BASE_N>>1)];	
	int16 limitRX;
	int16 Agcmode;
	int32 AGC_ptr[10];
	int16 alphns;
	
	int16 mic1_ns_flag;
	int16 flag0;// smooth dataout
	int16 y2_flag;
	int32 gamma;
	int16 psd_idx;
	int16 COHthr;
	int16 EPD_thr[4];
	int16 SER_thr[3];
	int16 RF_thr;
	int16 maxhowls;
	int16 Binpre;
	int16 Binlimup;
	int16 NLS_flag1;
	int16 NLS_flag2;

	int32 Epre[160*BD];
	int32 Fpre[160*BD];
	int32 Fref[160*BD];
	int32 Epre1[8];
	int32 Epre2[8];
	int32 Epre3[8];

}CVSI_T;

#ifdef FFT_TEST
int32 FFT_open_ini		(FFTI_INFO **h_CVSI_ptr,int32 sample_rate);
int32 FFT_close_ini		(FFTI_INFO **h_FFT_ptr);
#endif
int32 FFT_open_map		(CVSI_T    *CVSI_ptr);
int32 FFT_close_map		(CVSI_T    *CVSI_ptr);

#define FFT_open FFT_open_map
#define FFT_close FFT_close_map
//#define FFT_open FFT_open_ini	
//#define FFT_close FFT_close_ini	

int32 CVSI_TX_frame(CVSI_T *CVSI_ptr,int16 *data_in,int16 *data_rf,int16 *data_rx,int16 *data_out,int32 volume);
int32 CVSI_RX_frame(CVSI_T *CVSI_ptr,int16 *data_in,int16 *data_out,int32 volume);

int32 CVSI_open (CVSI_T **h_CVSI_ptr,int32 *EX_PARA);
int32 CVSI_para_set(CVSI_T *CVSI_ptr,int32 *EX_PARA);

int32 CVSI_close		(CVSI_T **h_CVSI_ptr);
int32 CVS_Version();

//////CVS NV structure
typedef struct
{
	int16 CHN;//0 for bypass, 1 for mono enhance, 2 for dual tx enhance	
	int16 AEC;//AEC switch hand hold
	int16 POS;//mono pos switch hand hold
	int16 EQF;//EQ filter switch hand hold
	int16 NES;//nonlinear echo suppression switch hand hold
	int16 HPF;//high pass filter switch hand hold
	int16 HEF;//human enhance filter switch hand hold
	int16 SPP;//custom filter switch hand hold
	int16 DRC;//dynamic range control switch hand hold
	int16 RSV;
}CVSI_MOD_PARA;

typedef struct 
{
	int16 Gain_limit;
	int16 NFloor;
	int16 NF_fade;
	int16 thr_cvs;
	int16 thr_cvs_sub;
	
	int16 sm_S[3];
	int16 sm_N[3];
	int16 alpha_S;
	int16 alpha_N;
	int16 G_SRise;
	int16 G_SFall;
	int16 G_NRise;
	int16 G_NFall;

	int16 RSV;
}CVSI_POS_PARA;

typedef struct  
{
	int16 sm[3];
	int16 alpha;
	int16 thr_cvs;

	int16 RSV;
}CVSI_VAD_PARA;


typedef struct
{
	//DRC para	
	int16 drc_p1_pos;
	int16 drc_p1_amp;
	int16 drc_p2_pos;
	int16 drc_p2_amp;
	int16 drc_s_pos;
	int16 drc_e_lim;
	int16 drc_sm;
	int16 drc_gain;
	int16 RSV1;
	int16 RSV2;
}CVSI_DRC_PARA;

typedef struct
{
	int16 EQBin[28];
}CVSI_EQF_PARA;

typedef struct
{
	int16 AEC_P_EL_THR;
	int16 AEC_S_EL_THR;
	int16 NLP_UPDATE;
	int16 NLP_NLE_Gain;
	int16 NLP_NLF_Gain;
	int16 NLP_thr_L1;
	int16 NLP_thr_L2;
	int16 NLP_sub_L1;
	int16 NLP_sub_L2;
	int16 RSV;
}CVSI_NES_PARA;

typedef struct 
{
	int16 mode_switch;
	int16 forceNBloop;//=VAL->reserved[18];

	int16 dl_AEC;
	int16 RxTxR_thr;
	int16 CNG_fade;
	
	int16 SPP_frm;	// 100
	int16 SPP_thr;	// 100
	int16 ECP_thr;	// reused for ADF mode 

	int16 cutoff_tx;
	int16 cutoff_rx;

	int16 dl_CTC;
	int16 CTC_CL1_THR;
	int16 CTC_CL2_THR;
	int16 CHN_EQ_THR;

	int16 CTC_LK1		;	//=VAL->reserved[0];
	int16 CTC_LK2		;	//=VAL->reserved[1];
	int16 CTC_FL		;	//=VAL->reserved[2];
	int16 CTC_VAD_FS	;	//=VAL->reserved[3];
	int16 CTC_VAD_FE	;	//=VAL->reserved[4];
	int16 CTC_VAD_COMP	;	//=VAL->reserved[5];

	int16 AEC_fade_mode	;	//=VAL->reserved[11];
	int16 AEC_fade_time	;	//=VAL->reserved[12];
	int16 AEC_ADF_FL	;	//=VAL->reserved[13];//AEC_ADF_FL;
	int16 NLP_rt_1		;	//=VAL->reserved[ 6];//AEC_ADF_FL;
	int16 NLP_rt_2		;	//=VAL->reserved[19];//AEC_ADF_FL;
	int16 NLP_gain_1	;	//=VAL->reserved[14];//AEC_ADF_FL;
	int16 NLP_gain_2	;	//=VAL->reserved[15];//AEC_ADF_FL;
	int16 RSV;

	int16 TX_CNG_FL		;	//=VAL->reserved[7];
	int16 RX_CNG_FL		;	//=VAL->reserved[8];
	int16 TX_CNG_FD		;	//=VAL->reserved[9];
	int16 RX_CNG_FD		;	//=VAL->reserved[10];
	int16 TX_NSP_GAIN	;	//=VAL->reserved[16];
	int16 RX_NSP_GAIN	;	//=VAL->reserved[17];
					
	CVSI_MOD_PARA TX_MODUAL;//36
	CVSI_MOD_PARA RX_MODUAL;//36
	CVSI_NES_PARA NES_PARA[9];//32*9
	CVSI_POS_PARA Tx_Dual_Post;//68
	CVSI_POS_PARA Tx_Mono_Post;//68
	CVSI_POS_PARA Rx_Mono_Post;//68
	CVSI_VAD_PARA VAD_tx;//20
	CVSI_VAD_PARA VAD_rx;//20	
	CVSI_EQF_PARA EQF_tx;//112
	CVSI_EQF_PARA EQF_rx;//112
	CVSI_DRC_PARA DRC_tx;//24
	CVSI_DRC_PARA DRC_rx;//24

	int16 reserved[128];//512
}CVSI_CTRL_PARAM;

typedef struct  
{
	uint8 ucModeName[CVS_MAX_MODE_NAME_LEN];
	CVSI_CTRL_PARAM CVS_param_NV;
}CVSI_CTRL_PARAM_NV_T;


//////CVS NV structure
#endif