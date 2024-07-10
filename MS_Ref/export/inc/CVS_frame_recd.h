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
#ifndef CVSF_FRAME_H_recd
#define CVSF_FRAME_H_recd


#include "CVS_typeDefs_recd.h"
#include "CVS_fft_recd.h"
#include "CVS_log2_recd.h"


typedef struct
{	
	///parameters
	int32 sm[3];
	int32 alpha;
//	int32 thr_cvs;
	///data
	int32 full_vad;	
	int32 Sc	[(BASE_N_recd>>1)];
	int32 Ss	[(BASE_N_recd>>1)];
	int32 G		[(BASE_N_recd>>1)];
	int32 pSNR	[(BASE_N_recd>>1)];
	int32 rateL	[MAX_SUBS_recd];
	int32 vads	[MAX_SUBS_recd];
	int32 Sf	[(BASE_N_recd>>1)];
}CVSI_VAD_recd;



typedef struct
{
	///data
	int32 *Ya;		//AEC filter out reference
	int32 *Yb;		//echo in mic signal
	int32 *Yc;		//linear AEC output
	int32 *Sa;		//original pre channel

	int32 sm[3][MAX_SUBS_recd];
	int32 alpha[MAX_SUBS_recd];

	int32 NF_fix;
	int32 thr_fix;
	int32 msa;// 128点幅度谱平均值
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
}CVSI_MONO_POST_recd;


//internal exchange buffer
typedef struct
{//int32 *XS;//	
	int32 Xa[(BASE_N_recd >> 1) * 2];//[(BASE_N_recd+1)];
	int32 Xw[(BASE_N_recd >> 1) * 4];//[(BASE_N_recd+1)*2];
	int32 Ea[(BASE_N_recd >> 1) * 2];//[(BASE_N_recd+1)];
	int32 Ew[(BASE_N_recd >> 1) * 4];//[(BASE_N_recd+1)*2];
	int32 dW[(BASE_N_recd >> 1) * 2];
	int32 dy[(BASE_N_recd >> 1) * 2];//rx_tmp tx_tmp
	int32 Ya[(BASE_N_recd >> 1) * 3];

	int32 *XR;//[(BASE_N_recd >> 1) * 8];	//T USED *Sr1 *Sr2//rectangle fft reference spectrum 
	int32 *DR;//[(BASE_N_recd >> 1) * 4];	//T
	int32 *DX;//[(BASE_N_recd >> 1) * 2];	//T

	int32 CTCAECBUF[(BASE_N_recd >> 1) * 36];
#ifdef F_BAND_recd
	int32 EX[(BASE_N_recd >> 1) * 2];	//for fft1536_recd temp
#endif
}CVSI_RX_recd;


typedef struct
{ 
	int32 drc_para[9];

	int32 drc_p1_pos;
	int32 drc_p1_amp;
	int32 drc_p2_pos;
	int32 drc_p2_amp;
	int32 drc_s_pos;
	int32 drc_e_lim;
	int32 drc_sm;
	int32 drc_gain;
}CVSI_DRC_recd;



typedef struct
{
	//High pass para
	int32 cutoff;
	int32 cutbin;

}CVSI_HPF_recd;

typedef struct
{
	//High pass para
	int32 EQBin[36];
	int32 EQMap[(BASE_N_recd>>1)];
}CVSI_EQF_recd;

typedef struct
{
	int32 CHN;//0 for bypass, 1 for mono enhance, 2 for dual tx enhance	
	int32 POS;//mono pos switch hand hold
	int32 EQF;//EQ filter switch hand hold
	int32 HPF;//high pass filter switch hand hold
	int32 DRC;//dynamic range control switch hand hold
	int32 RSV;
}CVSI_MOD_recd;

typedef struct
{
	int32	mode_switch;

	int32   band_mode;// narrow band=0, wideband=1
	int32	sample_rate;
	int32	sample_pms;
	int32   stepLen;
	int32	volume;

	CVSI_MOD_recd *TX_MODUAL;
	CVSI_MOD_recd *RX_MODUAL;

	int32	cnt_tx;
	int32	cnt_rx;

	//NS+ para
	int32 TX_NSP_GAIN;
	int32 RX_NSP_GAIN;

	int32 dc_rx;

	FFTI_INFO_recd   *FFT_ptr;

	FFTI_INFO_recd   *NB_FFT_ptr;
	FFTI_INFO_recd   *WB_FFT_ptr;
	FFTI_INFO_recd   *SB_FFT_ptr;
	FFTI_INFO_recd   *FB_FFT_ptr;

	CVSI_DRC_recd	*DRC_tx;
	CVSI_DRC_recd	*DRC_rx;
	CVSI_HPF_recd	*HPF_tx;
	CVSI_HPF_recd	*HPF_rx;
	CVSI_EQF_recd	*EQF_tx;
	CVSI_EQF_recd	*EQF_rx;

	CVSI_VAD_recd    *VADm_ptr;
	CVSI_VAD_recd	 *VADr_ptr;
	CVSI_RX_recd		*RX_ptr;

	CVSI_MONO_POST_recd  *TXMO_POST_ptr;
	CVSI_MONO_POST_recd  *RXMO_POST_ptr;

	//buffers
	int32 y1_buf[(BASE_N_recd>>1)*3];//mic  1 data buffer
	int32 y2_buf[(BASE_N_recd>>1)*3];//mic  2 data buffer
	int32 rx_buf[(BASE_N_recd>>1)*2];//raw rx data buffer

	int32 rx_ref[(8+1)*BASE_N_recd];//reference for aec 64ms max delay +3 overlapbuf +1 nbframebuf
	int32 rx_nlm[(BASE_N_recd >> 1) * 4];//[(8+1)*BASE_N_recd];//NonLinear_recd mapped reference

	int32 tx_syn[BASE_K_recd];
	int32 *tx_amp;//tx_amp[((BASE_N_recd>>1)+1)];//
	int32 *tx_ang;//tx_ang[BASE_N_recd+2];//
	int32 *tx_tmp;//tx_tmp[BASE_N_recd+2];//

	int32 rx_syn[BASE_K_recd];
	int32 *rx_amp;//rx_amp[((BASE_N_recd>>1)+1)];//
	int32 *rx_ang;//rx_ang[BASE_N_recd+2];//
	int32 *rx_tmp;//rx_tmp[BASE_N_recd+2];//

	int32 Ss_tx 		[(BASE_N_recd>>1)];	//reference noise
	int32 G_tx			[(BASE_N_recd>>1)];	//enhance filter without smoothing	
	int32 Sc_tx 		[(BASE_N_recd>>1)];	//old equalized filtered ref channel
	int32 pSNR_tx		[(BASE_N_recd>>1)];

	int32 Ss_rx 		[(BASE_N_recd>>1)];	//reference noise
	int32 G_rx			[(BASE_N_recd>>1)];	//enhance filter without smoothing	
	int32 Sc_rx 		[(BASE_N_recd>>1)];	//old equalized filtered ref channel
	int32 pSNR_rx		[(BASE_N_recd>>1)];	

}CVSI_T_recd;


int32 FFT_open_map_recd		(CVSI_T_recd    *CVSI_ptr);
int32 FFT_close_map_recd		(CVSI_T_recd    *CVSI_ptr);

#define FFT_open_recd FFT_open_map_recd
#define FFT_close_recd FFT_close_map_recd

int32 CVSI_RX_frame_recd(CVSI_T_recd *CVSI_ptr,int16 *data_in,int16 *data_out,int32 volume);
int32 CVSI_open_recd (CVSI_T_recd **h_CVSI_ptr  ,int32 *EX_PARA);
int32 CVSI_para_set_recd(CVSI_T_recd *CVSI_ptr  ,int32 *EX_PARA);
int32 CVSI_close_recd		(CVSI_T_recd **h_CVSI_ptr);
int32 CVS_Version_recd();

//////CVS NV structure
typedef struct
{
	int16 CHN;//0 for bypass, 1 for mono enhance, 2 for dual tx enhance	
	int16 POS;//mono pos switch hand hold
	int16 EQF;//EQ filter switch hand hold
	int16 HPF;//high pass filter switch hand hold
	int16 DRC;//dynamic range control switch hand hold
	int16 RSV;
}CVSI_MOD_PARA_recd;

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
}CVSI_POS_PARA_recd;

typedef struct  
{
	int16 sm[3];
	int16 alpha;
//	int16 thr_cvs;

	int16 RSV;// SPP
}CVSI_VAD_PARA_recd;


typedef struct
{	
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
}CVSI_DRC_PARA_recd;

typedef struct
{
	int16 EQBin[36];
}CVSI_EQF_PARA_recd;



typedef struct 
{
	int16 mode_switch;
	int16 cutoff_tx;
	int16 cutoff_rx;
	int16 TX_NSP_GAIN	;	
	int16 RX_NSP_GAIN	;	
					
	CVSI_MOD_PARA_recd TX_MODUAL;//36
	CVSI_MOD_PARA_recd RX_MODUAL;//36
	CVSI_POS_PARA_recd Tx_Mono_Post;//68
	CVSI_POS_PARA_recd Rx_Mono_Post;//68
	CVSI_VAD_PARA_recd VAD_tx;//20
	CVSI_VAD_PARA_recd VAD_rx;//20	
	CVSI_EQF_PARA_recd EQF_tx;//112
	CVSI_EQF_PARA_recd EQF_rx;//112
	CVSI_DRC_PARA_recd DRC_tx;//24
	CVSI_DRC_PARA_recd DRC_rx;//24

	int16 reserved[129];//512
}CVSI_CTRL_PARAM_recd;

typedef struct  
{
	uint8 ucModeName[CVS_MAX_MODE_NAME_LEN_recd];
	CVSI_CTRL_PARAM_recd CVS_param_NV;
}CVSI_CTRL_PARAM_NV_T_recd;


//////CVS NV structure
#endif