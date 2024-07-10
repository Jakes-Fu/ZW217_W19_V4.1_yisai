

#ifndef _HAL_ZSP_H_
#define _HAL_ZSP_H_

#define SPEECH_SUPPORT
#define MUSIC_SUPPORT

#if 1
#ifdef UINT8
#undef UINT8
#endif
#define UINT8 uint8

#ifdef UINT16
#undef UINT16
#endif
#define UINT16 uint16

#ifdef UINT32
#undef UINT32
#endif
#define UINT32 uint32

#ifdef INT8
#undef INT8
#endif
#define INT8 int8

#ifdef INT16
#undef INT16
#endif
#define INT16 int16

#ifdef INT32
#undef INT32
#endif
#define INT32 int32

#ifdef BOOL
#undef BOOL
#endif
#define BOOL int8

#ifdef VOID
#undef VOID
#endif
#define VOID void
#endif
// Music input buffer size.
#define AUDIO_INPUT_BUF_SIZE  (7*1024) // 7K Bytes
// Music output buffer size.
#define AUDIO_OUTPUT_BUF_SIZE (9*1024) // 9K Bytes
// Number of music post processing IIR bands.
#define EQ_MAX_BANDS 10
// Number of audio call IIR bands.
#define CALIB_AUDIO_CALL_IIR_BANDS (7)
// MSBC buf between wcn and zsp
#define MSBC_BUF_SIZE    30            // 30 Word
// MSBC ring buf in zsp 
#define MSBC_TO_PCM_BUF_SIZE    (120) // 120 Word
#define MSBC_TO_PCM_QUEUE_LEN    (8)
#define MSBC_TO_PCM_QUEUE_MASK    (MSBC_TO_PCM_QUEUE_LEN - 1)
// BT Work Mode
#define BT_WORK_MODE_NB    (0)
#define BT_WORK_MODE_WB    (1)

typedef enum
{
    NORMAL_VOICE_WB_REQ = 0x1111,
    NORMAL_VOICE_NB_REQ,
    
    BT_VOICE_WB_REQ,
    BT_VOICE_NB_REQ,
    
    PLAY_MP3_INIT_REQ,
    PLAY_AAC_INIT_REQ,
    PLAY_WMA_INIT_REQ,
    PLAY_AMR_INIT_REQ,

    PLAY_MP3_PRO_REQ,
    PLAY_AAC_PRO_REQ,
    PLAY_WMA_PRO_REQ,
    PLAY_AMR_PRO_REQ,

    MUSIC_POST_INIT_REQ,
    MUSIC_POST_PRO_REQ,

    RECORD_AAC_INIT_REQ,
    RECORD_AMR_INIT_REQ,
    RECORD_WAV_INIT_REQ,

    RECORD_AAC_PRO_REQ,
    RECORD_AMR_PRO_REQ,
    RECORD_WAV_PRO_REQ,

    PLAY_FM_INIT_REQ,

    CHANGE_VOICE_REQ,
    DTMF_DECODE_REQ,

    VOICE_RECORD_START_REQ,
    VOICE_RECORD_STOP_REQ,
    MUSIC_SBC_START_REQ,
    MUSIC_SBC_STOP_REQ,

    BT_MUSIC_CON_REQ,

    LOOPBACK_REQ,
    CLOSE_ALL_REQ,
    END_SHARECMD_REQ
    
} HAL_ZSP_WAKEUP_ID_T;

typedef enum
{
    NORMAL_VOICE_WB_IND = 0x2111,
    NORMAL_VOICE_NB_IND,
    
    BT_VOICE_WB_IND,
    BT_VOICE_NB_IND,
    
    PLAY_MP3_INIT_IND,
    PLAY_AAC_INIT_IND,
    PLAY_WMA_INIT_IND,
    PLAY_AMR_INIT_IND,

    PLAY_MP3_PRO_IND,
    PLAY_AAC_PRO_IND,
    PLAY_WMA_PRO_IND,
    PLAY_AMR_PRO_IND,

    MUSIC_POST_INIT_IND,
    MUSIC_POST_PRO_IND,

    RECORD_AAC_INIT_IND,
    RECORD_AMR_INIT_IND,
    RECORD_WAV_INIT_IND,

    RECORD_AAC_PRO_IND,
    RECORD_AMR_PRO_IND,
    RECORD_WAV_PRO_IND,

    PLAY_FM_INIT_IND,
    
    CHANGE_VOICE_IND,
    DTMF_DECODE_IND,

    VOICE_RECORD_START_IND,
    VOICE_RECORD_STOP_IND,
    MUSIC_SBC_START_IND,
    MUSIC_SBC_STOP_IND,

    BT_MUSIC_CON_IND,

    LOOPBACK_IND,
    CLOSE_ALL_IND,
    END_SHARECMD_IND
}HAL_ZSP2MCU_MB_ID_T;

typedef enum
{
    audIdle = 0xaabc,
    audWork = 0x1abc,
    audBusy = 0x2abc,
} AUD_STATUS_E;

typedef void (*HAL_ZSP_IRQ_HANDLER_T)(void);

//HAL_VOLTE.h
// =============================================================================
//  MACROS
// =============================================================================
/// FIFO depth.
#define HAL_VOLTE_FIFO_DEPTH                    (4)


// ============================================================================
// HAL_VOLTE_MODE_T
// -----------------------------------------------------------------------------
/// HAL VoLTE modes
// =============================================================================
typedef enum
{
    HAL_AMR66_DEC                              = 0x000000F0,
    HAL_AMR66_ENC                              = 0x0000000F,
    HAL_AMR885_DEC                             = 0x000000F1,
    HAL_AMR885_ENC                             = 0x0000001F,
    HAL_AMR1265_DEC                            = 0x000000F2,
    HAL_AMR1265_ENC                            = 0x0000002F,
    HAL_AMR1425_DEC                            = 0x000000F3,
    HAL_AMR1425_ENC                            = 0x0000003F,
    HAL_AMR1585_DEC                            = 0x000000F4,
    HAL_AMR1585_ENC                            = 0x0000004F,
    HAL_AMR1825_DEC                            = 0x000000F5,
    HAL_AMR1825_ENC                            = 0x0000005F,
    HAL_AMR1985_DEC                            = 0x000000F6,
    HAL_AMR1985_ENC                            = 0x0000006F,
    HAL_AMR2305_DEC                            = 0x000000F7,
    HAL_AMR2305_ENC                            = 0x0000007F,
    HAL_AMR2385_DEC                            = 0x000000F8,
    HAL_AMR2385_ENC                            = 0x0000008F
} HAL_VOLTE_MODE_T;


// ============================================================================
// HAL_VOLTE_FRAME_SIZE_T
// -----------------------------------------------------------------------------
/// Size of the frame in bytes, for the various supported codecs.
// =============================================================================
typedef enum
{
    HAL_VOLTE_FRAME_SIZE_AMR66                = 0x00000014, // 132
    HAL_VOLTE_FRAME_SIZE_AMR885               = 0x00000017, // 177
    HAL_VOLTE_FRAME_SIZE_AMR1265              = 0x00000010, // 253
    HAL_VOLTE_FRAME_SIZE_AMR1425              = 0x00000014, // 285
    HAL_VOLTE_FRAME_SIZE_AMR1585              = 0x00000014, // 317
    HAL_VOLTE_FRAME_SIZE_AMR1825              = 0x00000014, // 365
    HAL_VOLTE_FRAME_SIZE_AMR1985              = 0x0000001C, // 397
    HAL_VOLTE_FRAME_SIZE_AMR2305              = 0x00000020, // 461
    HAL_VOLTE_FRAME_SIZE_AMR2385              = 0x0000003C, // 477
    HAL_VOLTE_FRAME_SIZE_COD_BUF              = 0x00000048,
    HAL_VOLTE_FRAME_SIZE_PCM_BUF              = 0x00000280
} HAL_VOLTE_FRAME_SIZE_T;

#define HAL_VOLTE_FRAME_SIZE_MAX    (HAL_VOLTE_FRAME_SIZE_COD_BUF/2)   

// ============================================================================
// HAL_VOLTE_ENC_OUT_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef struct
{
    /// Mode of the voice coded output frame
    UINT16                         encRateMode;   //out
    UINT16                         txFrameType;   //out
    UINT16                         packFormat;    //in    
    UINT16                         dataLen;       //out    
    UINT16                         encOutBuf[36]; //0x00000008
} HAL_VOLTE_ENC_OUT_T; //Size : 0x2C

// ============================================================================
// HAL_VOLTE_DEC_IN_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef struct
{
    UINT16                         dtxOn;        //in                   
    UINT16                         rxFrameType;  //in
    UINT16                         decRateMode;  //in                  
    UINT16                         packFormat;   //in
    UINT16                         dataLen;      //in
    UINT16                         reserved;     //
    UINT16                         decInBuf[36]; //0x00000010
} HAL_VOLTE_DEC_IN_T; //Size : 0x34


// ============================================================================
// HAL_VOLTE_PCM_HALF_BUF_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef UINT16 HAL_VOLTE_PCM_HALF_BUF_T[HAL_VOLTE_FRAME_SIZE_PCM_BUF/2];

// ============================================================================
// HAL_VOLTE_PCM_BUF_T
// -----------------------------------------------------------------------------
/// Structure defining the VoLTE PCM swap buffers used in VoLTE encoding and
/// decoding
// =============================================================================
typedef struct
{
    HAL_VOLTE_PCM_HALF_BUF_T      pcmBuf[2];                    //0x00000000
} HAL_VOLTE_PCM_BUF_T; //Size : 0x500

// ============================================================================
// HAL_VOLTE_FIFO_ELM_T
// -----------------------------------------------------------------------------
/// This types describes a Fifo section. Rx and Tx are joined together because the
/// we always read and write one Rx and a Tx (or inversly) at the same time.
// =============================================================================
typedef struct
{
    /// Received encoded frame
    HAL_VOLTE_DEC_IN_T             rx;
    /// Encoded Frame to transmit
    HAL_VOLTE_ENC_OUT_T            tx;
} HAL_VOLTE_FIFO_ELM_T;

// ============================================================================
// HAL_SPEECH_FIFO_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef struct
{
    /// This types describes a Fifo section. Rx and Tx are joined together because
    /// the we always read and write one Rx and a Tx (or inversly) at the same time.
    HAL_VOLTE_FIFO_ELM_T          fifo[HAL_VOLTE_FIFO_DEPTH];  //0x00000000
    /// That boolean is set to TRUE when the Speech Fifo Rx has overflowed, which
    /// also corresponds to a Tx underflow. The reset is manual through coolwatcher.
    BOOL                           rxOverflowTxUnderflow;        //0x00000180
    /// That boolean is set to TRUE when the Speech Fifo Tx has overflowed, which
    /// also corresponds to a Rx underflow. This is normally expected in case of
    /// handover, for example. The reset is manual through coolwatcher.
    BOOL                           txOverflowRxUnderflow;        //0x00000181
} HAL_VOLTE_FIFO_T; //Size : 0x182


//HAL_SPEECH
// =============================================================================
//  MACROS
// =============================================================================
/// FIFO depth.
#define HAL_SPEECH_FIFO_DEPTH                    (4)

// ============================================================================
// HAL_SPEECH_MODE_T
// -----------------------------------------------------------------------------
/// HAL Speech modes
// =============================================================================
typedef enum
{
    HAL_AMR475_DEC                              = 0x000000F0,
    HAL_AMR475_ENC                              = 0x0000000F,
    HAL_AMR515_DEC                              = 0x000000F1,
    HAL_AMR515_ENC                              = 0x0000001F,
    HAL_AMR59_DEC                               = 0x000000F2,
    HAL_AMR59_ENC                               = 0x0000002F,
    HAL_AMR67_DEC                               = 0x000000F3,
    HAL_AMR67_ENC                               = 0x0000003F,
    HAL_AMR74_DEC                               = 0x000000F4,
    HAL_AMR74_ENC                               = 0x0000004F,
    HAL_AMR795_DEC                              = 0x000000F5,
    HAL_AMR795_ENC                              = 0x0000005F,
    HAL_AMR102_DEC                              = 0x000000F6,
    HAL_AMR102_ENC                              = 0x0000006F,
    HAL_AMR122_DEC                              = 0x000000F7,
    HAL_AMR122_ENC                              = 0x0000007F,
    HAL_EFR                                     = 0x00000177,
    HAL_FR                                      = 0x00001FFF,
    HAL_HR                                      = 0x00002FFF,
    HAL_INVALID                                 = 0x0000FFFF
} HAL_SPEECH_MODE_T;


// ============================================================================
// HAL_SPEECH_FRAME_SIZE_T
// -----------------------------------------------------------------------------
/// Size of the frame in bytes, for the various supported codecs.
// =============================================================================
typedef enum
{
    HAL_SPEECH_FRAME_SIZE_AMR475                = 0x0000000C,
    HAL_SPEECH_FRAME_SIZE_AMR515                = 0x00000010,
    HAL_SPEECH_FRAME_SIZE_AMR59                 = 0x00000010,
    HAL_SPEECH_FRAME_SIZE_AMR67                 = 0x00000014,
    HAL_SPEECH_FRAME_SIZE_AMR74                 = 0x00000014,
    HAL_SPEECH_FRAME_SIZE_AMR795                = 0x00000014,
    HAL_SPEECH_FRAME_SIZE_AMR102                = 0x0000001C,
    HAL_SPEECH_FRAME_SIZE_AMR122                = 0x00000020,
    HAL_SPEECH_FRAME_SIZE_AMR_RING              = 0x00000020,
    HAL_SPEECH_FRAME_SIZE_EFR                   = 0x00000020,
    HAL_SPEECH_FRAME_SIZE_FR                    = 0x00000024,
    HAL_SPEECH_FRAME_SIZE_HR                    = 0x00000010,
    HAL_SPEECH_FRAME_SIZE_COD_BUF               = 0x00000024,
    HAL_SPEECH_FRAME_SIZE_PCM_BUF               = 0x00000140
} HAL_SPEECH_FRAME_SIZE_T;


// ============================================================================
// HAL_RX_FRAME_TYPE_T
// -----------------------------------------------------------------------------
/// AMR enum
// =============================================================================
typedef enum
{
    HAL_RX_SPEECH_GOOD                          = 0x00000000,
    HAL_RX_SPEECH_DEGRADED                      = 0x00000001,
    HAL_RX_ONSET                                = 0x00000002,
    HAL_RX_SPEECH_BAD                           = 0x00000003,
    HAL_RX_SID_FIRST                            = 0x00000004,
    HAL_RX_SID_UPDATE                           = 0x00000005,
    HAL_RX_SID_BAD                              = 0x00000006,
    HAL_RX_NO_DATA                              = 0x00000007
} HAL_RX_FRAME_TYPE_T;


// ============================================================================
// HAL_TX_FRAME_TYPE_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
    HAL_TX_SPEECH                               = 0x00000000,
    HAL_TX_SID_FIRST                            = 0x00000001,
    HAL_TX_SID_UPDATE                           = 0x00000002,
    HAL_TX_NO_DATA                              = 0x00000003
} HAL_TX_FRAME_TYPE_T;


// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// HAL_SPEECH_ENC_OUT_T
// -----------------------------------------------------------------------------
/// Structure used to configure VOC frame encode
// =============================================================================
typedef struct
{
    /// Mode of the voice coded output frame
    UINT16                         encMode;                      //0x00000000
    /// Type of the coded output frame (AMR only)
    UINT16                         encFrameType;                 //0x00000002
    /// Flag for skipping the encoded frame, only valid when the ES is activated
    /// (replaces the unused VAD flag)
    UINT16                         echoSkipEncFrame;             //0x00000004
    /// Speech flag (all except AMR)
    UINT16                         sp;                           //0x00000006
    /// Encoder output buffer (coded frame in enc_mode)
    UINT16                         encOutBuf[HAL_SPEECH_FRAME_SIZE_COD_BUF/2]; //0x00000008
} HAL_SPEECH_ENC_OUT_T; //Size : 0x2C



// ============================================================================
// HAL_SPEECH_DEC_IN_T
// -----------------------------------------------------------------------------
/// Structure used to configure VOC frame decode
// =============================================================================
typedef struct
{
    /// DTX allowed by the network, active->G1
    UINT16                         dtxOn;                        //0x00000000
    /// mode of the voice coded input frame and of the next encoder speech frame
    /// output
    UINT16                         codecMode;                    //0x00000002
    /// Type of the coded input frame (AMR only)
    UINT16                         decFrameType;                 //0x00000004
    /// BFI flag (all vocoders except AMR)
    UINT16                         bfi;                          //0x00000006
    /// SID flag (all vocoders except AMR)
    UINT16                         sid;                          //0x00000008
    /// TAF flag (all vocoders except AMR)
    UINT16                         taf;                          //0x0000000A
    /// UFI flag (HR only)
    UINT16                         ufi;                          //0x0000000C
    /// Reserved
    UINT16                         reserved;                     //0x0000000E
    /// Decoder input buffer (coded frame in dec_mode)
    UINT16                         decInBuf[HAL_SPEECH_FRAME_SIZE_COD_BUF/2]; //0x00000010
} HAL_SPEECH_DEC_IN_T; //Size : 0x34

#define HAL_SPEECH_FRAME_SIZE_MAX (HAL_SPEECH_FRAME_SIZE_COD_BUF/2)

// ============================================================================
// HAL_SPEECH_PCM_HALF_BUF_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef UINT16 HAL_SPEECH_PCM_HALF_BUF_T[HAL_SPEECH_FRAME_SIZE_PCM_BUF/2];


// ============================================================================
// HAL_SPEECH_PCM_BUF_T
// -----------------------------------------------------------------------------
/// Structure defining the Speech PCM swap buffers used in Speech encoding and decoding
// =============================================================================
typedef struct
{
    HAL_SPEECH_PCM_HALF_BUF_T      pcmBuf[2];                    //0x00000000
} HAL_SPEECH_PCM_BUF_T; //Size : 0x280


// ============================================================================
// HAL_SPEECH_FIFO_ELM_T
// -----------------------------------------------------------------------------
/// This types describes a Fifo section. Rx and Tx are joined together because the
/// we always read and write one Rx and a Tx (or inversly) at the same time.
// =============================================================================
typedef struct
{
    /// Received encoded frame
    HAL_SPEECH_DEC_IN_T            rx;                           //0x00000000
    /// Encoded Frame to transmit
    HAL_SPEECH_ENC_OUT_T           tx;                           //0x00000034
} HAL_SPEECH_FIFO_ELM_T; //Size : 0x60



// ============================================================================
// HAL_SPEECH_FIFO_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef struct
{
    /// This types describes a Fifo section. Rx and Tx are joined together because
    /// the we always read and write one Rx and a Tx (or inversly) at the same time.
    HAL_SPEECH_FIFO_ELM_T          fifo[HAL_SPEECH_FIFO_DEPTH];  //0x00000000
    /// That boolean is set to TRUE when the Speech Fifo Rx has overflowed, which
    /// also corresponds to a Tx underflow. The reset is manual through coolwatcher.
    BOOL                           rxOverflowTxUnderflow;        //0x00000180
    /// That boolean is set to TRUE when the Speech Fifo Tx has overflowed, which
    /// also corresponds to a Rx underflow. This is normally expected in case of
    /// handover, for example. The reset is manual through coolwatcher.
    BOOL                           txOverflowRxUnderflow;        //0x00000181
} HAL_SPEECH_FIFO_T; //Size : 0x182


/// RDA VQE.
///
// ===================================================================
typedef struct{
    //for AF
    INT16 enable_af;
    INT16 Mu;
    INT16 MIN_LEAK;
    INT16 Delta_ctrl;
    INT16 delay;

    //for VAD
    INT16 amth;
    INT16 sil2spe;
    INT16 spe2sil;
    
    //for RES
    INT16 enable_res;
    INT16 res_gain;

    //for DTD
    INT16 enable_dtd;
    INT16 up_coeff;
    INT16 down_coeff;
    INT16 confident_coeff;
    INT16 lowest_coeff;
    INT16 gamma;
    INT16 DTD_protect_gain;
    INT16 dtd_hangover_out_max;
    INT16 dtd_hangover_out_max2;
    INT16 ERLE_small_thr; 
    INT32 FPH_sum_thr;    

    //for NS
    INT16 enable_ns;
    INT16 Gmin;
    INT16 Gmin_echo;
    INT16 Gmin_noise;
    INT16 alpha_ph;        //语音存在概率平滑的系数，越大噪声越容易残留
    INT16 alpha_S1;        //帧间平滑的系数，越大最小值估计的越大

    //for NLP
    INT16 enable_nlp;
    INT16 clip_process_flag;
    INT16 thd_process_flag;
    INT16 Gain_clip_value;
    INT16 clip_holdFrame;
    INT16 HL_thr;
    INT16 band1;
    
    // thd process
    INT16 GainThr;
    INT16 GainNbThr;
    INT16 thd_holdFrame;
    INT16 GainDetectRangeStart;
    INT16 GainDetectRangeEnd;
    INT16 HighBinsReduce;
    INT16 HighBinsStart;
    INT16 LowBinsReduce;
    INT16 LowBinsStart;
    INT16 HighBinsEnd;

    //for CNG
    INT16 enable_cng;
    INT16 cng_thr_amp; //-36dB 
    INT16 cng_thr_amp_up;  //-30dB 
}AUD_RDA_AEC_T;

typedef struct{

    //for NS
    INT16 enable_ns;
    INT16 Gmin;
    INT16 Gmin_echo;
    INT16 Gmin_noise;
    INT16 alpha_ph;        //语音存在概率平滑的系数，越大噪声越容易残留
    INT16 alpha_S1;        //帧间平滑的系数，越大最小值估计的越大
    //for CNG
    INT16 enable_cng;
    INT16 cng_thr_amp; //-36dB 
    INT16 cng_thr_amp_up;  //-30dB 
    INT16 reserved;
    
} AUD_RDA_RxNS_T;  //opened parameters


typedef struct
{
    INT16 enable;
    INT16 targetLevelDbfs;   // default 3 (-3 dBOv)
    INT16 compressionGaindB; // default 9 dB
    INT16 limiterEnable;     // default kAgcTrue (on)
    INT16 upperThr;
    INT16 lowerThr;
    INT16 decayValue;
    INT16 capacitorSlowFactor;
    INT16 capacitorFastFactor;
    INT16 noiseCompressFactor;
    INT16 noiseGateThr;
    INT16 noiseStdShortTermFactor;
} AUD_RDA_AGC_T;////


#define EQ_VOICE_MAX_BANDS 7

typedef struct
{
    INT16 num[3];
    INT16 den[3];

} AUD_EQ_FilterCoefs_T;

/*typedef enum
{
    EQ_SKIP = 0,
    EQ_LPF,
    EQ_HPF,
    EQ_peakingEQ,
    EQ_lowShelf,
    EQ_highShelf,
} EQ_filter_type;*/

typedef struct
{
    INT16 freq;
    INT16 type;
    INT16 gain;
    INT16 qual;
} AUD_EQ_iir_param_T;

typedef struct
{
    INT16 eqEnable;
    INT16 bands; 
    AUD_EQ_iir_param_T    param[EQ_VOICE_MAX_BANDS];
    AUD_EQ_FilterCoefs_T coeffs[EQ_VOICE_MAX_BANDS];
} AUD_RDA_EQ_T;

typedef struct
{
    INT16 in_enable;
    INT16 out_enable;
    INT16 scal_in;
    INT16 scal_out;
} AUD_RDA_DigGain_T;


typedef struct
{
    INT16   SpeechMode;
    INT16   aecEnable;
    INT16   rxNsEnable;
    INT16   reserved;

    AUD_RDA_DigGain_T DigtalGain;
    AUD_RDA_AEC_T AECParams;
    AUD_RDA_RxNS_T RxNSParams;
    AUD_RDA_AGC_T TxAGCParams;
    AUD_RDA_AGC_T RxAGCParams;
    AUD_RDA_EQ_T  TxEQParams;
    AUD_RDA_EQ_T  RxEQParams;
} AUD_RDA_VQE_PARAM_T;

typedef struct
{
    AUD_RDA_VQE_PARAM_T   rdaVqe;
}AUD_VQE_PARAM_T;


// ===================================================================
// AUD_MUSIC_DRC_PARAM_T
// -------------------------------------------------------------------
/// music drc.
///
// ===================================================================
typedef struct
{
    INT16 alc_enable;

    INT16 thres;
    INT16 width;
    INT16 R;
    INT16 R1;
    INT16 R2;
    INT16 limit;
    INT16 M_drc;
    INT16 alpha1;
    INT16 alpha2;
    INT16 noise_gate;
    INT16 alpha_max;
    INT16 Thr_dB;
    INT16 mm_gain;
    INT16 channel;
    INT16 reserved;
} AUD_MUSIC_DRC_T;

// ===================================================================
// AUD_MUSIC_IIR_EQ_BANDS_T
// -------------------------------------------------------------------
/// music iir eq.
///
// ===================================================================
/*typedef struct
{
    INT16 freq;
    INT16 qual;
    INT16 type;
    INT16 gain;
} AUD_MUSIC_IIR_EQ_BANDS_T;*/

// ===================================================================
// AUD_MUSIC_IIR_EQ_BANDS_T
// -------------------------------------------------------------------
/// music iir eq.
///
// ===================================================================
typedef struct
{
    INT16 eqEnable;
    INT16 bands;
    AUD_EQ_iir_param_T bandInfo[EQ_MAX_BANDS];
} AUD_MUSIC_EQ_PARAM_T;

typedef struct
{
    AUD_EQ_FilterCoefs_T coeffs[EQ_MAX_BANDS];
    AUD_MUSIC_EQ_PARAM_T param;
} AUD_MUSIC_EQ_T;

// ===================================================================
// AUD_ZSP_CFG_T
// -------------------------------------------------------------------
/// Structure defining
///
// ===================================================================

// ===================================================================
// AUDIO_INPUT_PARAM_T
// -------------------------------------------------------------------
/// Structure defining the Audio Encode/Decode input parameters
///
// ===================================================================
typedef struct
{
    UINT16 readOffset;
    UINT16 writeOffset;
    UINT16 fileEndFlag;
    UINT16 inLenth;      //Input stream buffer length, 7K byte
    UINT16 outLength;     //Output stream buffer length, (1152*4*2)byte
    UINT16 postProFlag;
    UINT16 sbcOutFlag;
    UINT16 musicVibFlag;

    // only for audio encode
    UINT16 recFormat;
    UINT16 channelNb;
    UINT32 sampleRate;
    UINT32 bitsPerSample;
} AUDIO_INPUT_PARAM_T;

// ===================================================================
// AUDIO_OUTPUT_PARAM_T
// -------------------------------------------------------------------
/// Structure defining the audio Encode/Decode output parameters
///
// ===================================================================
typedef struct
{
    //common paras
    UINT32 samplerate;
    UINT16 channelNb;    /* number of channels */
    UINT16 length;       /* number of samples per channel */
    UINT32 consumedLen;  /* size of the consumed data since last frame */
    UINT32 streamStatus; /* stream status */

    //other paras
    //UINT16 channelMode;
    //UINT16 layer;
    UINT16 readOffset;
    UINT16 writeOffset;
    UINT32 bitrate;
} AUDIO_OUTPUT_PARAM_T;

// ===================================================================
// AUDIO_MSBC_BUF_T
// -------------------------------------------------------------------
/// Structure defining the msbc buffer between wcn and zsp
///
// ===================================================================
typedef struct
{
    UINT32 wcn2zsp_mutex;
    UINT32 wcn2zsp_seq;
    UINT16 wcn2zsp_data[MSBC_BUF_SIZE];

    UINT32 zsp2wcn_mutex;
    UINT32 zsp2wcn_seq;
    UINT16 zsp2wcn_data[MSBC_BUF_SIZE];

    UINT16 reverd[512];

    UINT16 ul_ridx;
    UINT16 ul_widx;
    UINT32 ul_offset;
    UINT16 ul_pcm_queue[MSBC_TO_PCM_QUEUE_LEN][MSBC_TO_PCM_BUF_SIZE];

    UINT16 dl_ridx;
    UINT16 dl_widx;
    UINT32 dl_offset;
    UINT16 dl_pcm_queue[MSBC_TO_PCM_QUEUE_LEN][MSBC_TO_PCM_BUF_SIZE];
    
    UINT16 ul_pcm_8k[160];
    UINT16 dl_pcm_16k[320];
} AUDIO_MSBC_BUF_T;

typedef union 
{
    /* Speech Parameters */
    HAL_SPEECH_ENC_OUT_T    txAMRBuffer;
    HAL_VOLTE_ENC_OUT_T    txAMRVolte;
}UNION_AMR_TX_T;

typedef union 
{
    /* Speech Parameters */
    HAL_SPEECH_DEC_IN_T     rxAMRBuffer;
    HAL_VOLTE_DEC_IN_T     rxAMRVolte;
}UNION_AMR_RX_T;

typedef union
{
    HAL_SPEECH_PCM_BUF_T    txPcmBuffer;
    HAL_VOLTE_PCM_BUF_T     txPcmVolte;
}UNION_PCM_TX_T;

typedef union
{
    HAL_SPEECH_PCM_BUF_T    rxPcmBuffer;
    HAL_VOLTE_PCM_BUF_T     rxPcmVolte;
}UNION_PCM_RX_T;

typedef union 
{
    INT16 audOutput[AUDIO_OUTPUT_BUF_SIZE/2];//zsp return the decode/code to CPU
    AUDIO_MSBC_BUF_T msbc;
}UNION_AUDIO_OUTPUT_BUF_T;

typedef union
{
    INT16 audOutput[320];
    AUDIO_MSBC_BUF_T msbc;
}UNION_SPEECH_OUTPUT_BUF_T;


// ===================================================================
// AUD_SHAREMEM_T
// -------------------------------------------------------------------
/// Structure defining the audio share memory (CPARM <-> ZSP900m)
///
// ===================================================================
//#define SPEECH_SUPPORT
//#define MUSIC_SUPPORT
typedef struct
{
    /* ZSP Ctrl Info */
    UINT16 audCurrStatus;
    UINT16 updateParaInd;
    UINT16 loopBackType; 
    UINT16 voiceRecFormat; 
    UINT16 volteFlag; 
    UINT16 musicMode;
    UINT32 traceDataFlag;
    UINT32 BTWorkMode;

#ifdef SPEECH_SUPPORT
    UNION_AMR_TX_T txAMR;
    UNION_AMR_RX_T rxAMR;
#endif

    UNION_PCM_TX_T txPCM;
    UNION_PCM_RX_T rxPCM;

    /* VQE Parameters*/
    AUD_VQE_PARAM_T     vePara;

#ifdef MUSIC_SUPPORT
    /* Music Post Process Parameters */
    AUD_MUSIC_DRC_T drcPara;
    AUD_MUSIC_EQ_T eqPara;
    /* Audio(mp3/aac/wma/bt/amr) */
    AUDIO_INPUT_PARAM_T audInPara;
    AUDIO_OUTPUT_PARAM_T audOutPara;

    INT16 audInput[AUDIO_INPUT_BUF_SIZE/2]; //CPU send audio data to decoded/code
    UNION_AUDIO_OUTPUT_BUF_T Output;
#else //for speech record
    UNION_SPEECH_OUTPUT_BUF_T Output;
#endif
} AUD_ZSP_SHAREMEM_T;


//extern HAL_ERR_T hal_zspAudOpen(HAL_VOC_IRQ_HANDLER_T IrqHandler);
extern VOID hal_zspAudClose(VOID);
extern HAL_ERR_T hal_zspAudWakeup(HAL_ZSP_WAKEUP_ID_T wakeupId);
extern VOID hal_zspAudTraceData(UINT32 dataFlag);
extern HAL_ERR_T hal_zspMusicPlayStart(void);
extern HAL_ERR_T hal_zspMusicPlayStop(void);

extern AUD_ZSP_SHAREMEM_T* g_aud_sharemem;
//extern HAL_VOC_IRQ_HANDLER_T g_zspIrqHandler;

#endif

