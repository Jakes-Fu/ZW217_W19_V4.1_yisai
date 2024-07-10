#if !defined(AFX_TYPEDEF_H__EFAB2D72_8EFE_4987_BEAD_04D7AC4F1F84__INCLUDED_)
#define AFX_TYPEDEF_H__EFAB2D72_8EFE_4987_BEAD_04D7AC4F1F84__INCLUDED_

//??
//#define JUNIPER_NXP

enum
{
    sidetone_off,
    sidetone_digital,
    sidetone_pga,
    sidetone_external
};

enum
{
    aec_off,
    aec_digital,
    aec_pga,
    aec_external
};

enum
{
    volume_off,
    volume_digital,
    volume_pga,
    volume_external
};

enum
{
    sample_rate_8k,
    sample_rate_16k,
	sample_rate_24k,
	sample_rate_32k,
	sample_rate_48k,
};

enum
{
    agc_switch_off,
    agc_switch_on,
};

#define SAMPLE_RATE_8000    8000
#define SAMPLE_RATE_16000   16000

#define DL_FIR_EQ_COUNT 17
#define UL_FIR_EQ_COUNT 33
#define ST_FIR_EQ_COUNT 33

#define EXTEND_DATA_SIZE 59

#define EXTEND_VERSION_SIZE 460
#define ROCKY_VERSION_SIZE 500
#define AUDIO_VERSION_3_SIZE 740

#define DSP_VOLUME_SIZE 10
#define ARM_VOLUME_SIZE 10

#define VOLUME_LEVEL 9

typedef struct AudioStruct
{
    WORD dl_device_internal;
    WORD dl_device_external;
    WORD ul_device_internal;
    WORD ul_device_external;
    WORD sidetone_switch;
    WORD aec_switch;
    WORD volume_mode;
    WORD dl_pga_gain_l;
    WORD dl_pga_gain_h;
    WORD ul_pga_gain_l;
    WORD ul_pga_gain_h;
    WORD sample_rate;

    WORD dl_dp_gain;
    WORD dl_dp_attenu;

    WORD dl_eq_bass_alpha;
    WORD dl_eq_bass_beta;
    WORD dl_eq_bass_gama;
    WORD dl_eq_bass_gain;
    WORD dl_eq_mid_alpha;
    WORD dl_eq_mid_beta;
    WORD dl_eq_mid_gama;
    WORD dl_eq_mid_gain;
    WORD dl_eq_treble_alpha;
    WORD dl_eq_treble_beta;
    WORD dl_eq_treble_gama;
    WORD dl_eq_treble_gain;
    
    WORD digital_sidetone_gain;

    WORD ul_dp_gain;
    WORD ul_dp_attenu;

    WORD ul_eq_bass_alpha;
    WORD ul_eq_bass_beta;
    WORD ul_eq_bass_gama;
    WORD ul_eq_bass_gain;
    WORD ul_eq_treble_alpha;
    WORD ul_eq_treble_beta;
    WORD ul_eq_treble_gama;
    WORD ul_eq_treble_gain;

    WORD dl_pop_switch;
    WORD dl_agc_switch;

    //WORD reserved[7];
    WORD dl_eq_agc_switch;
    WORD dp_switch;
    WORD dl_eq_switch;
    WORD d1_agc_rsv0;
    WORD d1_agc_rsv1;
    WORD ul_eq_switch;
    WORD dl_da_pop_switch;

    WORD ul_fir_hpf_enable;
    WORD aec_enable;
    WORD pdelay;
    WORD dl_ref_hpf_enable;
    
    WORD decor_filter_enable;
    WORD fir_taps;
    WORD aec_frozen;
    WORD coeff_frozen;
    WORD dt_dect_threshold;
    WORD dt_noise_floor;
    WORD step_size;
    WORD coeff_norm_shift;
    WORD sa_ctrl;
        
    WORD send_attenu_in_dt;
    WORD send_attenu_in_rv;
    WORD send_threshold;
    WORD r2dt_threshold;
    WORD s2dt_threshold;
    WORD recv_threshold;
    WORD bn40;
    WORD sa_ar;

    WORD ng_select;
    WORD alpha_distor;
    WORD beta_distor;
    WORD ul_ng_plk_wpyy_a;
    WORD ul_ng_plk_wpyy_n;
    WORD ul_ng_plk_holdc;     
    WORD ul_ng_plk_att;
    WORD ul_ng_clk_wpyy_a;
    WORD ul_ng_clk_wpyy_n;
    WORD ul_ng_clk_holdc;
    WORD ul_ng_clk_att;   
    WORD dl_ng_plk_wpyy_a;
    WORD dl_ng_plk_wpyy_n;    
    WORD dl_ng_plk_holdc;
    WORD dl_ng_plk_att;   
    WORD dl_ng_clk_wpyy_a;
    WORD dl_ng_clk_wpyy_n;    
    WORD dl_ng_clk_holdc;     
    WORD dl_ng_clk_att;       
        
    WORD da_limit;

    WORD reserved1;

    WORD dl_fir_eq_coeff[DL_FIR_EQ_COUNT];
    WORD ul_fir_eq_coeff[UL_FIR_EQ_COUNT];
    WORD st_fir_eq_coeff[ST_FIR_EQ_COUNT];
    
    //WORD extend[EXTEND_DATA_SIZE];
    WORD dl_clarity_switch : 8;
    WORD ul_clarity_switch : 8;

    WORD reserved2 : 4;
    WORD aec_sa_in_select : 4;
    WORD aec_sa_off : 4;
    WORD aec_coeff_frozen : 4;

	WORD audio_three_switch;

	WORD st_voice_threshold;
	WORD st_noise_threshold;
	WORD st_holdc;
	WORD st_att;
	WORD st_zcr_threshold;
	WORD st_zcr_ceil;

	WORD st_dgain[VOLUME_LEVEL];
	WORD dl_PGA0[VOLUME_LEVEL];
	WORD dl_d_gain[VOLUME_LEVEL];
	WORD ref_dgain[VOLUME_LEVEL];
	WORD ul_PGA0[VOLUME_LEVEL];

	WORD reserved4[5];

	short arm_volume[ARM_VOLUME_SIZE];
    WORD dsp_volume[DSP_VOLUME_SIZE];

    WORD dl_vad_switch : 4;
    WORD dl_complex_switch : 4;
    WORD dl_noise_est_switch : 4;
    WORD dl_nr_switch : 4;

    WORD dl_dis_switch : 4;
    WORD dl_amb_noise_est_switch : 4;
    WORD dl_amb_eq_switch : 4;
    WORD dl_acoustic_eq_switch : 4;

    WORD dl_drc_switch : 4;
    WORD dl_dac_limit_switch : 4;
    WORD dl_hpf_set : 4;
    WORD dl_lpf_set : 4;

    WORD ul_vad_switch : 4;
    WORD ul_complex_switch : 4;
    WORD ul_noise_est_switch : 4;
    WORD ul_nr_switch : 4;
    
    WORD ul_dis_switch : 4;
    WORD ul_amb_noise_est_switch : 4;
    WORD ul_amb_eq_switch : 4;
    WORD reserved3 : 4;
    
    WORD ul_drc_switch : 4;
    WORD ul_echo_residual_switch : 4;
    WORD ul_hpf_set : 4;
    WORD ul_lpf_set : 4;

	WORD ul_no_vad_cnt_thd;
    WORD ul_min_psne;
    WORD ul_max_temp_uamn;
    WORD ul_vad_thd;
    WORD ul_active_thd;
    WORD ul_noise_thd;
    WORD ul_max_psne;
    WORD ul_voise_burst;
    WORD ul_noise_tail;
    WORD ul_rfilter_delay;
    WORD ul_rfilter_tail;
    WORD ul_rfilter;
    WORD ul_dgain;
    WORD ul_sim_M;
    WORD ul_sim_fac;
	WORD ul_dac_limit;
    WORD ul_ns_factor;
    WORD ul_ns_limit;
	WORD ul_ns_up_factor;
    WORD ul_dis_snr_thd;
    WORD ul_dis_band_1k;
    WORD ul_dis_limit;
    WORD ul_drc_thd;
    WORD ul_drc_ratio;
    WORD ul_drc_dstep;
    WORD ul_drc_ustep;
    WORD ul_drc_cnt;

	WORD ul_amb_release;
	WORD ul_amb_attack;
	WORD ul_amb_ndefault;
	WORD ul_echo_ns_limit;

    WORD dl_no_vad_cnt_thd;
    WORD dl_min_psne;
    WORD dl_max_temp_uamn;
    WORD dl_vad_thd;
    WORD dl_active_thd;
    WORD dl_noise_thd;
    WORD dl_max_psne;
    WORD dl_voise_burst;
    WORD dl_noise_tail;
    WORD dl_rfilter_delay;
    WORD dl_rfilter_tail;
    WORD dl_rfilter;
    WORD dl_dgain;
    WORD dl_sim_M;
    WORD dl_sim_fac;
    WORD dl_dac_limt;
    WORD dl_ns_factor;
    WORD dl_ns_limit;
	WORD dl_ns_up_factor;
    WORD dl_dis_snr_thd;
    WORD dl_dis_band_1k;
    WORD dl_dis_limit;
    WORD dl_drc_thd;
    WORD dl_drc_ratio;
    WORD dl_drc_dstep;
    WORD dl_drc_ustep;
    WORD dl_drc_cnt;
    
	WORD dl_reserved[4];

	WORD ul_dgain1[VOLUME_LEVEL];
	WORD dl_PGA1[VOLUME_LEVEL];
	WORD dl_drc_dgain[VOLUME_LEVEL];

	WORD audio_max_sa_limit;

	WORD ul_vaq_vol_buf_1;
	WORD ul_vaq_a_bn_set[2];
	WORD ul_vaq_a_tn_set[2];
	WORD ul_vaq_vol_buf_0;

	WORD reserved5[18];   // extend2[102]

}AUDIO_STRUCT;

#define MODE_NAME_LEN 16
typedef struct _audio_mode
{
    BYTE mode_name[MODE_NAME_LEN];
    AUDIO_STRUCT audio_data;
}AUDIO_MODE;

typedef struct _audio_data
{
    int mode_count;
    AUDIO_MODE* lpAudioMode;
}AUDIO_DATA;

//teana hu 2010.06.02
#define  TOTAL_EQ_MODE_NUM  6
#define NAME_MAX_LEN (16)
#define EQ_EXTEND_ARRAY_DIM 41
typedef struct 
{
    short f1_g0;  
    short f1_g1;  
    short f1_fp;  
  
    short f2_g0;  
    short f2_g1;    
    short f2_fp;  
}LCF_FILTER_NV_PARAM_T; 

typedef struct 
{
    short bass_f0;          
    short bass_q;          
    short bass_boostdB;            
    short bass_gaindB;   
}EQ_BASS_NV_PARAM_T;  

typedef struct 
{ 
    short treble_f0;          
    short treble_q;          
    short treble_boostdB;            
    short treble_gaindB; 
}EQ_TREBLE_NV_PARAM_T;  
 
typedef struct 
{
    LCF_FILTER_NV_PARAM_T lcf_filter;	
    short   agc_in_gain;
    EQ_BASS_NV_PARAM_T eq_bass;
    EQ_TREBLE_NV_PARAM_T eq_treble;
}EQ_MODE_NV_PARAM_T;  

typedef struct //PACKED 
{
    BYTE ucParaSetName[NAME_MAX_LEN];	// EQ PARA_SET name
    short eq_mode_control;
    EQ_MODE_NV_PARAM_T eq_modes[TOTAL_EQ_MODE_NUM];
	short   extend_array[EQ_EXTEND_ARRAY_DIM];
}AUDIO_EQ_STRUCT_T;//totally 91words

// This is only used for mp3 4.0
// eq_platform_6800_only is used for the first version for mp3 4.0 implement in 6800
enum
{
	eq_platform_6800_only,
	eq_platform_all,
};

typedef struct _eq_data
{
	int eq_count;
	int eq_platform;   // eq_platform_6800_only or eq_platform_all
	AUDIO_EQ_STRUCT_T* lpEQMode;
}EQ_DATA;

enum
{
	sound_eq_ver_3,
	sound_eq_ver_4,
	sound_eq_ver_none = 0xFF,  // Does not use eq mode
};

#define GET_SOUND_EQ_VER(eq_control) ( ( eq_control & 0x1000 ) >> 12 )
typedef struct  
{
	short fp_l;
	short lcf_gain_l;
	short fp_r;
	short lcf_gain_r;
}BUTTERWORTH_LCF_T;

enum
{
	sound_eq_mode_f1f1,
	sound_eq_mode_butterworth,
};

//

//teana hu 2011.02.16
#define DEV_SET_NUM 3
#define AUD_PROC_EXP_NUM 3
#define AUDIO_NV_ARM_PARA_RESERVE 64
#define AUDIO_DEVICE_MODE_PARAM_MAX 8
#define AUDIO_AGC_INPUG_GAIN_MAX 8
#define AUDIO_ARM_VOLUME_LEVEL 31
#define AUDIO_NV_ARM_APP_PARA_RESERVE 9
#define AUDIO_ARM_APP_TYPE_MAX 16
#define AUDIO_ARM_APP_TYPE_MIN 8
#define AUDIO_ARM_APP_TYPE_ONE 1
//teana hu 2012.01.20
#define AUDIO_ARM_APP_TYPE_SIX 2
//

typedef struct Audio_Nv_Arm_Device_Path_Struct
{
	short valid_dev_set_count;
	//AUDMOD_DEVICECTL_T bit0-bit7:
	//ucADChannel&0x1, 
	//ucDAChannel&0x1, 
	//ucDA0Reverse&0x1, 
	//ucDA1Reverse&0x1, 
	//ucStereoEn&0x1, 
	//ucSpkAmpEn&0x1, 
	//ucSpkAmpEn&0x1,
	//ucMixerInput&0x1 
	short reserve;//shujing add for align
	short dev_set[AUDIO_DEVICE_MODE_PARAM_MAX];
}AUDIO_NV_ARM_DEVICE_PATH_T;

typedef struct Audio_Nv_Arm_App_Config_Info_Struct
{
	short eq_switch;
	short agc_input_gain[AUDIO_AGC_INPUG_GAIN_MAX];
	short valid_volume_level_count;
	short arm_volume[AUDIO_ARM_VOLUME_LEVEL];
	//teana hu 2012.01.30
	LONG arm_volume_l[AUDIO_ARM_VOLUME_LEVEL];
	//
	short reserve[AUDIO_NV_ARM_APP_PARA_RESERVE];
}AUDIO_NV_ARM_APP_CONFIG_INFO_T;//include eq_switch/agc_input_gain/arm_volume_table

typedef struct Audio_Nv_Arm_App_Config_Info_Min_Struct
{
	short eq_switch;
	short agc_input_gain[AUDIO_AGC_INPUG_GAIN_MAX];
	short valid_volume_level_count;
	short arm_volume[AUDIO_ARM_VOLUME_LEVEL];
	short reserve[AUDIO_NV_ARM_APP_PARA_RESERVE];
}AUDIO_NV_ARM_APP_CONFIG_INFO_MIN_T;//include eq_switch/agc_input_gain/arm_volume_table

typedef struct Audio_Nv_Arm_App_Set_Struct
{
	short valid_app_set_count;
	short valid_agc_input_gain_count;
	short aud_proc_exp_control[2];//switch of agc/lcf/eq_select--original dsp audio nv:extend2[110]	
	AUDIO_NV_ARM_APP_CONFIG_INFO_T app_config_info[AUDIO_ARM_APP_TYPE_MAX];	
}AUDIO_NV_ARM_APP_SET_T;

typedef struct Audio_Nv_Arm_App_Set_Min_Struct
{
	short valid_app_set_count;
	short valid_agc_input_gain_count;
	short aud_proc_exp_control[2];//switch of agc/lcf/eq_select--original dsp audio nv:extend2[110]	
	AUDIO_NV_ARM_APP_CONFIG_INFO_MIN_T app_config_info[AUDIO_ARM_APP_TYPE_MIN];	
}AUDIO_NV_ARM_APP_SET_MIN_T;

typedef struct Audio_Nv_Arm_App_Set_One_Struct
{
	short valid_app_set_count;
	short valid_agc_input_gain_count;
	short aud_proc_exp_control[2];//switch of agc/lcf/eq_select--original dsp audio nv:extend2[110]	
	AUDIO_NV_ARM_APP_CONFIG_INFO_MIN_T app_config_info[AUDIO_ARM_APP_TYPE_ONE];	
}AUDIO_NV_ARM_APP_SET_ONE_T;

//teana hu 2012.01.20
typedef struct Audio_Nv_Arm_App_Config_Info_Max_Struct
{
	short eq_switch;
	short agc_input_gain[AUDIO_AGC_INPUG_GAIN_MAX];
	short valid_volume_level_count;
	LONG arm_volume[AUDIO_ARM_VOLUME_LEVEL];
	short reserve[AUDIO_NV_ARM_APP_PARA_RESERVE];
}AUDIO_NV_ARM_APP_CONFIG_INFO_MAX_T;//include eq_switch/agc_input_gain/arm_volume_table

typedef struct Audio_Nv_Arm_App_Set_Six_Struct
{
	short valid_app_set_count;
	short valid_agc_input_gain_count;
	short aud_proc_exp_control[2];//switch of agc/lcf/eq_select--original dsp audio nv:extend2[110]	
	AUDIO_NV_ARM_APP_CONFIG_INFO_MAX_T app_config_info[AUDIO_ARM_APP_TYPE_SIX];	
}AUDIO_NV_ARM_APP_SET_SIX_T;

typedef struct VolumeStructSix
{
    AUDIO_NV_ARM_DEVICE_PATH_T dev_path_set;
	AUDIO_NV_ARM_APP_SET_SIX_T app_config_info_set;
    short midi_opt;
    short aud_dev;     
    short reserve[AUDIO_NV_ARM_PARA_RESERVE];
}VOLUME_STRUCT_SIX;

typedef struct Audio_Nv_Arm_App_Config_Info_Sixteen_Struct
{
	short eq_switch;
	short agc_input_gain[AUDIO_AGC_INPUG_GAIN_MAX];
	short valid_volume_level_count;
	short arm_volume[AUDIO_ARM_VOLUME_LEVEL];
	short reserve[AUDIO_NV_ARM_APP_PARA_RESERVE];
}AUDIO_NV_ARM_APP_CONFIG_INFO_SIXTEEN_T;//include eq_switch/agc_input_gain/arm_volume_table

typedef struct Audio_Nv_Arm_App_Set_Sixteen_Struct
{
	short valid_app_set_count;
	short valid_agc_input_gain_count;
	short aud_proc_exp_control[2];//switch of agc/lcf/eq_select--original dsp audio nv:extend2[110]	
	AUDIO_NV_ARM_APP_CONFIG_INFO_MIN_T app_config_info[AUDIO_ARM_APP_TYPE_MAX];	
}AUDIO_NV_ARM_APP_SET_SIXTEEN_T;

typedef struct VolumeStructSixteen
{
    AUDIO_NV_ARM_DEVICE_PATH_T dev_path_set;
	AUDIO_NV_ARM_APP_SET_SIXTEEN_T app_config_info_set;
    short midi_opt;
    short aud_dev;     
    short reserve[AUDIO_NV_ARM_PARA_RESERVE];
}VOLUME_STRUCT_SIXTEEN;
//

typedef struct VolumeStructMin
{
    AUDIO_NV_ARM_DEVICE_PATH_T dev_path_set;
	AUDIO_NV_ARM_APP_SET_MIN_T app_config_info_set;
    short midi_opt;
    short aud_dev;     
    short reserve[AUDIO_NV_ARM_PARA_RESERVE];
}VOLUME_STRUCT_MIN;

typedef struct VolumeStruct
{
    AUDIO_NV_ARM_DEVICE_PATH_T dev_path_set;
	AUDIO_NV_ARM_APP_SET_T app_config_info_set;
    short midi_opt;
    short aud_dev;     
    short reserve[AUDIO_NV_ARM_PARA_RESERVE];
}VOLUME_STRUCT;

typedef struct VolumeStructOne
{
    AUDIO_NV_ARM_DEVICE_PATH_T dev_path_set;
	AUDIO_NV_ARM_APP_SET_ONE_T app_config_info_set;
    short midi_opt;
    short aud_dev;     
    short reserve[AUDIO_NV_ARM_PARA_RESERVE];
}VOLUME_STRUCT_ONE;

typedef struct _volume_mode
{
    BYTE mode_name[MODE_NAME_LEN];
    VOLUME_STRUCT volume_data;
}VOLUME_MODE;

typedef struct _volume_data
{
    int mode_count;
    VOLUME_MODE* lpVolumeMode;
}VOLUME_DATA;
//

//teana hu 2011.03.21
#define EQ_BAND_G_NUM 8
#define EQ_MODE_G_NUM 6
#define EQ_EXTEND_G 59
#define BAND_F0_NUM 8
#define Q_NUM 8
#define BOOSTDB_DEFAULT_BAND_NUM 8
#define BOOSTDB_CURRENT_BAND_NUM 8
#define EQ_EXTEND_GX 54

typedef struct  
{
	short f0;
	short q;
	short boostdB;
	short gaindB;
}EQ_BAND_G;

typedef struct  
{
	short agc_in_gain;
	short band_control;
	EQ_BAND_G eq_band[EQ_BAND_G_NUM];
}EQ_MODE_G;

typedef struct  
{
	short agc_in_gain;
	short band_control;
	short boostdB_default_band[BOOSTDB_DEFAULT_BAND_NUM];
	short boostdB_current_band[BOOSTDB_CURRENT_BAND_NUM];
}EQ_MODE_GX;

typedef struct 
{
	BYTE eq_set_name[NAME_MAX_LEN];
	short eq_control;
	short f0_band[BAND_F0_NUM];
	short q[Q_NUM];
	short level_n;
	EQ_MODE_GX eq_mode[EQ_MODE_G_NUM];
	short extend[EQ_EXTEND_GX];	
}TUN_STRUCT_G_T;

typedef struct 
{
	BYTE eq_set_name[NAME_MAX_LEN];
	short eq_control;
	EQ_MODE_G eq_mode[EQ_MODE_G_NUM];
	short extend[EQ_EXTEND_G];
}EQ_STRUCT_G_T;

typedef struct _eq_data_g
{
	int eq_count;
	EQ_STRUCT_G_T* lpEQMode;
}EQ_DATA_G;

typedef struct _tun_data_g
{
	int tun_count;
	TUN_STRUCT_G_T* lpTunMode;
}TUN_DATA_G;
//

//teana hu 2011.04.14
#define LVVE_EQ_COEF_SIZE      (32) 
#define LVVE_MAX_MODE_NAME_LEN (16)
#define LVM_MAXENUM             2147483647
#define LVVE_CONFIG_RESERVE_LEN (64)

typedef struct
{
    /* General parameters */
    LONG                 OperatingMode;          /* Operating mode */
	
    /* Parameters for FENS */
	
    SHORT                   FENS_limit_NS;          /* FENS Noise Suppression Limit */
	
	unsigned short                  Mode;
} LVFENS_ControlParams_st;

typedef struct
{
    /* General */
    short                     NLPP_Limit;
    unsigned short                    NLPP_HPF_CornerFreq;
	
} LVNLPP_ControlParams_st;

typedef struct
{
    /* General parameters */
    LONG                 OperatingMode;          /* Operating mode */
	
    /* Parameters for VCX */
    short                   mode;
    short                   Gain_Lin_Max;
    short                   Noise_Sensitivity;
    short                   AVL_Target_level_lin;
    short                   AVL_MinGainLin;
    short                   AVL_MaxGainLin;
    short                   AVL_NominalGain;
    short                   AVL_Attack;
    short                   AVL_Release;
    short                   AVL_ReleaseMax;
    short                   AVL_Limit_MaxOutputLin;
    short                   SpDetect_Threshold;
	
} LVVC_ControlParams_st;

typedef struct
{
    /* General */
    unsigned short                    EQ_Length;
    short                     *pEQ_Coefs;
	
} LVEQ_ControlParams_st;

typedef struct
{
    /* General parameters */
    LONG                 OperatingMode;          /* Operating mode */
	
    /***********************************************************************************************/
    /**** Add the module public control parameters here. Include a heading for each group of    ****/
    /**** parameters used by the module. Group the parameters logically, by use or fuction.     ****/
    /**** Delete this comment in the real module                                                ****/
    /***********************************************************************************************/
    /* LVHF - Internal mode parameters */
    unsigned short                  Mode;                    // [0, 128]   default  0xBF = FULL ON, 64=mute, 128=NLMS_preset
    unsigned short                  TuningMode;
	
    /* LVHF - Scaling parameters */    
    short                   InputGain;               // [0, 32767] default = 8192 = 1.0
    short                   OutputGain;              // [0, 32767] default = 2048 = 1.0
	
    /* LVHF - PRE-NLMS parameters */    
    short                   NLMS_limit;              // [-24, 0]dB default = 0
	
    /* LVHF - NLMS parameters */    
    unsigned short                  NLMS_LB_taps;            // [0, 200]   default     64
    short                   NLMS_LB_two_alpha;       // [0, 32767] default   1000 = 0.25
    short                   NLMS_LB_erl;             // [0, 32767] default    128 = less than 0 dB
    unsigned short                  NLMS_HB_taps;            // [0, 200]   default     64
    short                   NLMS_HB_two_alpha;       // [0, 32767] default   1000 = 0.25
    short                   NLMS_HB_erl;             // [0, 32767] default    128 = less than 0 dB
    short                   NLMS_preset_coefs;       // [0, 2]     default      1 = yes, nonzero
    short                   NLMS_offset;             // [0, 32767] default    300 = see above and try to change it!
	
    /* LVHF - DENS parameters */
    short                   DENS_tail_alpha;         // [0, 32767] default  25395 = 0.775
    short                   DENS_tail_portion;       // [0, 32767] default  29491 = 0.9
    short                   DENS_gamma_e_high;       // [0, 32767] default   2047 = 1.999
    short                   DENS_gamma_e_dt;         // [0, 32767] default   1024 = 1.0
    short                   DENS_gamma_e_low;        // [0, 32767] default   1024 = 1.0
    short                   DENS_gamma_e_alpha;      // [0, 32767] default  24000 = 0.7325
    short                   DENS_gamma_n;
    short                   DENS_limit_ns;           // [0, 32767] default   9830 = 0.3
    short                   DENS_CNI_Gain;           // [0, 32767] default  32686 = 1.995
    short                   DENS_NL_atten;           // [0, 2048] default   2048 = 1.0
    short                   DENS_spdet_near;
    short                   DENS_spdet_act;
    short                   DENS_NFE_blocksize;
	
    /* LVHF - Speech Detector parameters */
    short                   SPDET_far;
    short                   SPDET_mic;
    short                   SPDET_x_clip;
	
    /* LVHF - Path Change Detector parameters */
    short                   PCD_threshold;
    unsigned short                  PCD_taps;
    short                   PCD_erl;
    short                   PCD_minimum_erl;
    short                   PCD_erl_step;
    short                   PCD_gamma_e_rescue; 
	
} LVHF_ControlParams_st;

typedef struct
{
    /* General parameters */
    LONG                 OperatingMode;          /* Operating mode */
	
    /* Parameters for LVWM */
    short                   mode;
    short                   AVL_Target_level_lin;
    short                   AVL_MinGainLin;
    short                   AVL_MaxGainLin;
    short                   AVL_Attack;
    short                   AVL_Release;
    short                   AVL_Limit_MaxOutputLin;
    short                   SpDetect_Threshold;
	
} LVWM_ControlParams_st;

// typedef struct
// {
//     /* General parameters */
//     LONG                     OperatingMode;          /* Operating mode */
// 	
//     /* Parameters for VE_Rx */
//     LONG                         Mute;
// 	
//     /* VOL parameters */
//     LONG                         VOL_OperatingMode;
//     SHORT                           VOL_Gain;
// 	
// #ifndef JUNIPER_NXP
//     /* FENS parameters */
//     LVFENS_ControlParams_st             FENS_ControlParams;
// #endif
// 	
//     /* NLPP parameters */
//     LONG                         NLPP_OperatingMode;
//     LVNLPP_ControlParams_st             NLPP_ControlParams;
// 	
// #ifndef JUNIPER_NXP
//     /* Voice Clarity parameters */
//     LVVC_ControlParams_st               VC_ControlParams;
// #endif
// 
//     /* Equalizer parameters */
//     LONG                         EQ_OperatingMode;
//     LVEQ_ControlParams_st               EQ_ControlParams;
// 	
// } LVVE_Rx_ControlParams_st;

//teana hu 2012.01.06
#define LVMDRC_MAX_KNEES                     5      /* Maximum number of knee points */

typedef struct
{
    LONG                 OperatingMode;  // /* Operating mode for DRC */
    /* Parameters for compressor */
    unsigned short                  NumKnees; //Max is 5, minimum is 1 for top level [input level 0dBFs]
    SHORT                   CompressorCurveInputLevels[LVMDRC_MAX_KNEES]; //Specified in dBFS [range is -96dB till 0dBFs]
    SHORT                   CompressorCurveOutputLevels[LVMDRC_MAX_KNEES]; //Specified in dBFS
    SHORT                   AttackTime; //specified in  ms
    SHORT                   ReleaseTime; //specified in ms
    LONG                 LimiterOperatingMode;          /* Operating mode for limiter*/
    SHORT                   LimitLevel; //Expressed in dBFS
} LVDRC_ControlParams_st;

typedef struct
{
    SHORT                  CNG_Volume;     /* CNG Output Volume in dB (-90 to 0 dB)*/
} CNGControl_Parameters_st;

typedef struct
{
    LONG                 OperatingMode;  // /* Operating mode for DRC */
    /* Parameters for compressor */
    unsigned short                  NumKnees; //Max is 5, minimum is 1 for top level [input level 0dBFs]
    SHORT                   CompressorCurveInputLevels[LVMDRC_MAX_KNEES]; //Specified in dBFS [range is -96dB till 0dBFs]
    SHORT                   CompressorCurveOutputLevels[LVMDRC_MAX_KNEES]; //Specified in dBFS
    SHORT                   AttackTime; //specified in  ms
    SHORT                   ReleaseTime; //specified in ms
    LONG                 LimiterOperatingMode;          /* Operating mode for limiter*/
    SHORT                   LimitLevel; //Expressed in dBFS
} LVNG_ControlParams_st;

// typedef struct
// {
//     /* General parameters */
//     LONG                     OperatingMode;          /* Operating mode */
// 	
//     /* Parameters for VE_Rx */
//     LONG                         Mute;
// 	
//     /* VOL parameters */
//     LONG                         VOL_OperatingMode;
//     SHORT                           VOL_Gain;
// 	
//     /* FENS parameters */
//     LVFENS_ControlParams_st             FENS_ControlParams;
// 	
//     /* NLPP parameters */
//     LONG                         NLPP_OperatingMode;
//     LVNLPP_ControlParams_st             NLPP_ControlParams;
// 	
//     /* Voice Clarity parameters */
//     LVVC_ControlParams_st               VC_ControlParams;
// 	
//     /* Equalizer parameters */
//     LONG                         EQ_OperatingMode;
//     LVEQ_ControlParams_st               EQ_ControlParams;
// 
// 	/* DRC parameters */
//     LVDRC_ControlParams_st              DRC_ControlParams;
// 	
//     /* Microphone High Pass Filer */
//     LONG                         HPF_OperatingMode;
//     unsigned short                          HPF_CornerFreq;
// 	
//     /* CNG Parameters*/
//     LONG                         CNG_OperatingMode;        /* Operating mode for CNG */
//     CNGControl_Parameters_st             CNG_ControlParams;
// 	
//     /* Whisper Mode parameters */
//     LVWM_ControlParams_st               WM_ControlParams;
// 	
//     /* Noise Gate parameters */
//     LVNG_ControlParams_st               NG_ControlParams;
// 	
// } LVVE_Dual_mic_Rx_ControlParams_st;

typedef struct
{
    /* General parameters */
    LONG                 OperatingMode;          /* Operating mode */
	
    /* Parameters for NV */
    unsigned short                  Mode                    ;
    unsigned short                  Mode2                   ;
    unsigned short                  Tuning_mode             ;
    SHORT                   Input_Gain_Mic0         ;
    SHORT                   Input_Gain_Mic1         ;
    SHORT                   Output_Gain             ;
	
    unsigned short                  NLMS0_LB_taps           ;
    SHORT                   NLMS0_LB_twoalpha       ;
    SHORT                   NLMS0_LB_erl            ;
    unsigned short                  NLMS0_HB_taps           ;
    SHORT                   NLMS0_HB_twoalpha       ;
    SHORT                   NLMS0_HB_erl            ;
    SHORT                   NLMS0_preset_coefs      ;
    SHORT                   NLMS0_offset            ;
    unsigned short                  NLMS1_LB_taps           ;
    SHORT                   NLMS1_LB_twoalpha       ;
    SHORT                   NLMS1_LB_erl            ;
    unsigned short                  NLMS1_HB_taps           ;
    SHORT                   NLMS1_HB_twoalpha       ;
    SHORT                   NLMS1_HB_erl            ;
    SHORT                   NLMS1_preset_coefs      ;
    SHORT                   NLMS1_offset            ;
	
    SHORT                   CAL_micPowFloorMin      ;
    SHORT                   WgThreshold             ;
    SHORT                   MpThreshold             ;	
    SHORT                   FSB_init_table0[8]      ;
    SHORT                   FSB_init_table1[8]      ;
    unsigned short                  FSB_taps                ;
    SHORT                   FSB_twoalpha            ;
    SHORT                   FSB_ref_gain            ;
	
    unsigned short                  GSC_taps                ;
    SHORT                   GSC_twoalpha            ;
    SHORT                   GSC_erl                 ;
    SHORT                   GSC_offset              ;
	
    SHORT                   DNNS_EchoGammaHi        ;
    SHORT                   DNNS_EchoGammaLo        ;
    SHORT                   DNNS_EchoAlphaRev       ;
    SHORT                   DNNS_EchoTailPortion    ;
    SHORT                   DNNS_NlAtten            ;
    SHORT                   DNNS_NoiseGammaS        ;
    SHORT                   DNNS_NoiseGammaN        ;
    SHORT                   DNNS_NoiseGainMinS      ;
    SHORT                   DNNS_NoiseGainMinN      ;
    SHORT                   DNNS_NoiseBiasComp      ;
    SHORT                   DNNS_GainEta            ;
    SHORT                   DNNS_AcThreshold        ;
    SHORT                   DNNS_WbThreshold        ;
    SHORT                   DNNS_LostBeamThreshold  ;
	
    SHORT                   PCD_beta                ;
    SHORT                   PCD_Threshold           ;
	
} LVNV_ControlParams_st;

// typedef struct
// {
//     /* General parameters */
//     LONG               OperatingMode;          /* Operating mode */
// 	
//     /* Parameters for VE_Rx */
//     LONG                   Mute;
// 	
// 	
//     /* Delay Parameter */
//     LONG                   BD_OperatingMode;
//     unsigned short                    BulkDelay;
// 	
//     /* VOL parameters */
//     LONG                   VOL_OperatingMode;
//     short                     VOL_Gain;
// 	
//     /* Microphone High Pass Filer */
//     LONG                   HPF_OperatingMode;
//     short                    MIC_HPF_CornerFreq;
// 	
//     /* Hands Free parameters */
//     LVHF_ControlParams_st         HF_ControlParams;
// 
// 	/* Noise Void parameters */
//     LVNV_ControlParams_st         NV_ControlParams;
// 	
//     /* Whisper Mode parameters */
//     LVWM_ControlParams_st         WM_ControlParams;
// 	
//     /* Equalizer parameters */
//     LONG                   EQ_OperatingMode;
//     LVEQ_ControlParams_st         EQ_ControlParams;
// 
// 	/* DRC parameters */
//     LVDRC_ControlParams_st        DRC_ControlParams;
// 	
// } LVVE_Dual_mic_Tx_ControlParams_st;


//

// typedef struct
// {
//     /* General parameters */
//     LONG               OperatingMode;          /* Operating mode */
// 	
//     /* Parameters for VE_Rx */
//     LONG                   Mute;
// 	
// 	
//     /* Delay Parameter */
//     LONG                   BD_OperatingMode;
//     unsigned short                    BulkDelay;
// 	
//     /* VOL parameters */
//     LONG                   VOL_OperatingMode;
//     short                     VOL_Gain;
// 	
//     /* Microphone High Pass Filer */
//     LONG                   HPF_OperatingMode;
//     short                    MIC_HPF_CornerFreq;
// 	
//     /* Hands Free parameters */
//     LVHF_ControlParams_st         HF_ControlParams;
// 	
// #ifndef JUNIPER_NXP
//     /* Whisper Mode parameters */
//     LVWM_ControlParams_st         WM_ControlParams;
// #endif
// 
//     /* Equalizer parameters */
//     LONG                   EQ_OperatingMode;
//     LVEQ_ControlParams_st         EQ_ControlParams;
// 	
// } LVVE_Tx_ControlParams_st;

//

//teana hu 2012.01.06
typedef struct
{
    /* General parameters */
    LONG               OperatingMode;          /* Operating mode */
	
    /* Parameters for VE_Rx */
    LONG                   Mute;
	
	
    /* Delay Parameter */
    LONG                   BD_OperatingMode;
    unsigned short                    BulkDelay;
	
    /* VOL parameters */
    LONG                   VOL_OperatingMode;
    short                     VOL_Gain;
	
    /* Microphone High Pass Filer */
    LONG                   HPF_OperatingMode;
    short                    MIC_HPF_CornerFreq;
	
    /* Hands Free parameters */
    LVHF_ControlParams_st         HF_ControlParams;
	
    /* Whisper Mode parameters */
    LVWM_ControlParams_st         WM_ControlParams;
	
    /* Equalizer parameters */
    LONG                   EQ_OperatingMode;
    LVEQ_ControlParams_st         EQ_ControlParams;
	
} LVVE_Tx_ControlParams_st_1;

typedef struct
{
    /* General parameters */
    LONG               OperatingMode;          /* Operating mode */
	
    /* Parameters for VE_Rx */
    LONG                   Mute;
	
	
    /* Delay Parameter */
    LONG                   BD_OperatingMode;
    unsigned short                    BulkDelay;
	
    /* VOL parameters */
    LONG                   VOL_OperatingMode;
    short                     VOL_Gain;
	
    /* Microphone High Pass Filer */
    LONG                   HPF_OperatingMode;
    short                    MIC_HPF_CornerFreq;
	
    /* Hands Free parameters */
    LVHF_ControlParams_st         HF_ControlParams;
	
    /* Equalizer parameters */
    LONG                   EQ_OperatingMode;
    LVEQ_ControlParams_st         EQ_ControlParams;
	
} LVVE_Tx_ControlParams_st_2;

typedef struct
{
    /* General parameters */
    LONG               OperatingMode;          /* Operating mode */
	
    /* Parameters for VE_Rx */
    LONG                   Mute;
	
	
    /* Delay Parameter */
    LONG                   BD_OperatingMode;
    unsigned short                    BulkDelay;
	
    /* VOL parameters */
    LONG                   VOL_OperatingMode;
    SHORT                     VOL_Gain;
	
    /* Microphone High Pass Filer */
    LONG                   HPF_OperatingMode;
    unsigned short                    MIC_HPF_CornerFreq;
	
    /* Hands Free parameters */
    LVHF_ControlParams_st         HF_ControlParams;
	
	
    /* Whisper Mode parameters */
    LVWM_ControlParams_st         WM_ControlParams;
	
    /* Equalizer parameters */
    LONG                   EQ_OperatingMode;
    LVEQ_ControlParams_st         EQ_ControlParams;
	
	
    /* DRC parameters */
    LVDRC_ControlParams_st        DRC_ControlParams;
} LVVE_Tx_ControlParams_st_3;

typedef struct
{
    /* General parameters */
    LONG               OperatingMode;          /* Operating mode */
	
    /* Parameters for VE_Rx */
    LONG                   Mute;
	
	
    /* Delay Parameter */
    LONG                   BD_OperatingMode;
    unsigned short                    BulkDelay;
	
    /* VOL parameters */
    LONG                   VOL_OperatingMode;
    short                     VOL_Gain;
	
    /* Microphone High Pass Filer */
    LONG                   HPF_OperatingMode;
    short                    MIC_HPF_CornerFreq;
	
    /* Hands Free parameters */
    LVHF_ControlParams_st         HF_ControlParams;
	
	/* Noise Void parameters */
    LVNV_ControlParams_st         NV_ControlParams;
	
    /* Whisper Mode parameters */
    LVWM_ControlParams_st         WM_ControlParams;
	
    /* Equalizer parameters */
    LONG                   EQ_OperatingMode;
    LVEQ_ControlParams_st         EQ_ControlParams;
	
	/* DRC parameters */
    LVDRC_ControlParams_st        DRC_ControlParams;
	
} LVVE_Tx_ControlParams_st;


typedef struct
{
    /* General parameters */
    LONG                     OperatingMode;          /* Operating mode */
	
    /* Parameters for VE_Rx */
    LONG                         Mute;
	
    /* VOL parameters */
    LONG                         VOL_OperatingMode;
    SHORT                           VOL_Gain;
	
    /* FENS parameters */
    LVFENS_ControlParams_st             FENS_ControlParams;
	
    /* NLPP parameters */
    LONG                         NLPP_OperatingMode;
    LVNLPP_ControlParams_st             NLPP_ControlParams;
	
    /* Voice Clarity parameters */
    LVVC_ControlParams_st               VC_ControlParams;
	
    /* Equalizer parameters */
    LONG                         EQ_OperatingMode;
    LVEQ_ControlParams_st               EQ_ControlParams;
	
} LVVE_Rx_ControlParams_st_1;

typedef struct
{
    /* General parameters */
    LONG                     OperatingMode;          /* Operating mode */
	
    /* Parameters for VE_Rx */
    LONG                         Mute;
	
    /* VOL parameters */
    LONG                         VOL_OperatingMode;
    SHORT                           VOL_Gain;
	
    /* NLPP parameters */
    LONG                         NLPP_OperatingMode;
    LVNLPP_ControlParams_st             NLPP_ControlParams;
	
    /* Equalizer parameters */
    LONG                         EQ_OperatingMode;
    LVEQ_ControlParams_st               EQ_ControlParams;
	
} LVVE_Rx_ControlParams_st_2;

typedef struct
{
    /* General parameters */
    LONG                     OperatingMode;          /* Operating mode */
	
    /* Parameters for VE_Rx */
    LONG                         Mute;
	
    /* VOL parameters */
    LONG                         VOL_OperatingMode;
    SHORT                           VOL_Gain;
	
    /* FENS parameters */
    LVFENS_ControlParams_st             FENS_ControlParams;
	
	
    /* NLPP parameters */
    LONG                         NLPP_OperatingMode;
    LVNLPP_ControlParams_st             NLPP_ControlParams;
	
    /* Voice Clarity parameters */
    LVVC_ControlParams_st               VC_ControlParams;
	
    /* Equalizer parameters */
    LONG                         EQ_OperatingMode;
    LVEQ_ControlParams_st               EQ_ControlParams;
	
	
    /* Microphone High Pass Filer */
    LONG                         HPF_OperatingMode;
    unsigned short                          HPF_CornerFreq;
	
    /* CNG Parameters*/
    LONG                         CNG_OperatingMode;        /* Operating mode for CNG */
    CNGControl_Parameters_st             CNG_ControlParams;
	
    /* Whisper Mode parameters */
    LVWM_ControlParams_st               WM_ControlParams;
	
	
} LVVE_Rx_ControlParams_st_3;

typedef struct
{
    /* General parameters */
    LONG                     OperatingMode;          /* Operating mode */
	
    /* Parameters for VE_Rx */
    LONG                         Mute;
	
    /* VOL parameters */
    LONG                         VOL_OperatingMode;
    SHORT                           VOL_Gain;
	
    /* FENS parameters */
    LVFENS_ControlParams_st             FENS_ControlParams;
	
    /* NLPP parameters */
    LONG                         NLPP_OperatingMode;
    LVNLPP_ControlParams_st             NLPP_ControlParams;
	
    /* Voice Clarity parameters */
    LVVC_ControlParams_st               VC_ControlParams;
	
    /* Equalizer parameters */
    LONG                         EQ_OperatingMode;
    LVEQ_ControlParams_st               EQ_ControlParams;
	
	/* DRC parameters */
    LVDRC_ControlParams_st              DRC_ControlParams;
	
    /* Microphone High Pass Filer */
    LONG                         HPF_OperatingMode;
    unsigned short                          HPF_CornerFreq;
	
    /* CNG Parameters*/
    LONG                         CNG_OperatingMode;        /* Operating mode for CNG */
    CNGControl_Parameters_st             CNG_ControlParams;
	
    /* Whisper Mode parameters */
    LVWM_ControlParams_st               WM_ControlParams;
	
    /* Noise Gate parameters */
    LVNG_ControlParams_st               NG_ControlParams;
	
} LVVE_Rx_ControlParams_st;


typedef struct LVVE_CTRL_PARAM_1_
{
    LVVE_Rx_ControlParams_st_1    ControlParams_Rx;
    short  RxEqCoefs[LVVE_EQ_COEF_SIZE];
    LVVE_Tx_ControlParams_st_1    ControlParams_Tx;
    short  TxEqCoefs[LVVE_EQ_COEF_SIZE];
}LVVE_CTRL_PARAM_1_T;

typedef struct LVVE_CTRL_PARAM_NV_1_
{
    BYTE ucModeName[LVVE_MAX_MODE_NAME_LEN];
    LVVE_CTRL_PARAM_1_T lvve_ctrl_param_t;
	short lvve_reserve[LVVE_CONFIG_RESERVE_LEN];
}LVVE_CTRL_PARAM_NV_1_T;

typedef struct LVVE_CTRL_PARAM_2_
{
    LVVE_Rx_ControlParams_st_2    ControlParams_Rx;
    short  RxEqCoefs[LVVE_EQ_COEF_SIZE];
    LVVE_Tx_ControlParams_st_2    ControlParams_Tx;
    short  TxEqCoefs[LVVE_EQ_COEF_SIZE];
}LVVE_CTRL_PARAM_2_T;

typedef struct LVVE_CTRL_PARAM_NV_2_
{
    BYTE ucModeName[LVVE_MAX_MODE_NAME_LEN];
    LVVE_CTRL_PARAM_2_T lvve_ctrl_param_t;
	short lvve_reserve[LVVE_CONFIG_RESERVE_LEN];
}LVVE_CTRL_PARAM_NV_2_T;

typedef struct LVVE_CTRL_PARAM_3_
{
    LVVE_Rx_ControlParams_st_3    ControlParams_Rx;
    short  RxEqCoefs[LVVE_EQ_COEF_SIZE];
    LVVE_Tx_ControlParams_st_3    ControlParams_Tx;
    short  TxEqCoefs[LVVE_EQ_COEF_SIZE];
}LVVE_CTRL_PARAM_3_T;

typedef struct LVVE_CTRL_PARAM_NV_3_
{
    BYTE ucModeName[LVVE_MAX_MODE_NAME_LEN];
    LVVE_CTRL_PARAM_3_T lvve_ctrl_param_t;
	short lvve_reserve[LVVE_CONFIG_RESERVE_LEN];
}LVVE_CTRL_PARAM_NV_3_T;

typedef struct LVVE_CTRL_PARAM_
{
    LVVE_Rx_ControlParams_st    ControlParams_Rx;
    short  RxEqCoefs[LVVE_EQ_COEF_SIZE];
    LVVE_Tx_ControlParams_st    ControlParams_Tx;
    short  TxEqCoefs[LVVE_EQ_COEF_SIZE];
}LVVE_CTRL_PARAM_T;


typedef struct LVVE_CTRL_PARAM_NV_
{
    BYTE ucModeName[LVVE_MAX_MODE_NAME_LEN];
    LVVE_CTRL_PARAM_T lvve_ctrl_param_t;
	short lvve_reserve[LVVE_CONFIG_RESERVE_LEN];
}LVVE_CTRL_PARAM_NV_T;


typedef struct LVVE_CTRL_PARAM_NV_MIN_
{
    BYTE ucModeName[LVVE_MAX_MODE_NAME_LEN];
    LVVE_CTRL_PARAM_1_T lvve_ctrl_param_t;
}LVVE_CTRL_PARAM_NV_MIN_T;


typedef struct LVVE_CTRL_PARAM_NV_LIST_
{
	int mode_count;
    LVVE_CTRL_PARAM_NV_T*  lp_lvve_ctrl_nv_params;
}LVVE_CTRL_PARAM_NV_LIST_T;

//

//teana hu 2012.01.19
#define AUDIO_NV_DSP_MODE_NAME_MAX_LEN (16)

#define AUDIO_NV_DSP_MODE_STRUCT_RESERVE (24)
#define AUDIO_NV_DSP_VOL_CFG_NUM (32)

typedef struct Audio_Nv_Dsp_Mode_Struct
{
    unsigned short dac_set;
    unsigned short adc_set;
    unsigned short path_set;
    unsigned short vol_cfg0[AUDIO_NV_DSP_VOL_CFG_NUM];
    unsigned short vol_cfg1[AUDIO_NV_DSP_VOL_CFG_NUM];
    unsigned short vol_cfg2[AUDIO_NV_DSP_VOL_CFG_NUM];
    unsigned short reserve[AUDIO_NV_DSP_MODE_STRUCT_RESERVE];    
}AUDIO_NV_DSP_MODE_STRUCT_T;

typedef struct Audio_Nv_Dsp_Mode_Info_struct
{
    BYTE ucModeName[AUDIO_NV_DSP_MODE_NAME_MAX_LEN];	
    AUDIO_NV_DSP_MODE_STRUCT_T tAudioNvDspModeStruct;   
}AUDIO_NV_DSP_MODE_INFO_T;

typedef struct Audio_Nv_dsp_Mode_List_
{
	int nModeCount;
	AUDIO_NV_DSP_MODE_INFO_T* lp_audio_nv_dsp_mode_info;
}AUDIO_NV_DSP_MODE_LIST_T;

//

#endif