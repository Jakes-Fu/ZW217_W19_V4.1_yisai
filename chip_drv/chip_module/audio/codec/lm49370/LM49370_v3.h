/******************************************************************************
 ** File Name:      lm49370_v3.h                                              *
 ** Author:                                                            *
 ** DATE:                                                           *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    .                                         *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/02/2009               Create.                                   *
 *****************************************************************************/

#ifndef _LM49370_V3_H_
#define _LM49370_V3_H_


#define LM49370_SUCCESS                             0
#define LM49370_ERR_INVALID_READBACK                1
#define LM49370_ERR_INVALID_PARA                    2

/**---------------------------------------------------------------------------*
 **                         Registers                                         *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                         Shifts                                            *
 **---------------------------------------------------------------------------*/
#define PLL_ENALBE                 (1 << 2)

#define PLL_IN_MCLK                (0x0)
#define PLL_IN_IIS_CLK_IN          (0x1)
#define PLL_IN_PCM_CLK_IN          (0x2)

//#define PLL_OUTPUT                   (0x1)

typedef enum
{
    INTERFACE_PCM = 0,
    INTERFACE_IIS = 1

} INTERFACE;

/**---------------------------------------------------------------------------*
 **                         IIS Configuration                                 *
 **---------------------------------------------------------------------------*/

typedef enum
{
    IIS_CLK_SRC_DAC = 0,
    IIS_CLK_SRC_ADC
} IIS_CLK_SRC;

typedef enum
{
    BITS_PER_WORD_16 = 0,
    BITS_PER_WORD_25,
    BITS_PER_WORD_32
} IIS_WS_MODE;

typedef enum
{
    IIS_GROUPING_RL = 0,
    IIS_GROUPING_LR
} IIS_GROUPING;

typedef enum
{
    IIS_MODE_IIS_COMPATIBLE = 0,
    IIS_MODE_LEFT_JUSTIFIED
} IIS_MODE;

typedef enum
{
    IIS_BIT_8 = 0,
    IIS_BIT_16,
    IIS_BIT_32
} IIS_BIT_PER_CHANNAL;

typedef struct
{
    // IIS Output
    BOOLEAN IisOutputEnable;
    BOOLEAN IisInputEnable;

    // IIS Input
    BOOLEAN IisClockMaster;
    BOOLEAN IisWSMaster;

    // Internal Oscillator
    BOOLEAN IntOSCEnable;

    // IIS Stereo Reverse
    BOOLEAN IisStereoReverse;

    // IIS WS Mode
    IIS_WS_MODE IisWSMode;

    // IIS clock source, default is DAC
    //IIS_CLK_SRC IisClkSrc;

    //IIS_BIT_PER_CHANNAL IisBitPerChannel;

    // IIS Grouping
    IIS_GROUPING IisGrouping;

    // IIS Mode
    IIS_MODE IisMode;
} IIS_CONFIG, *PIIS_CONFIG;

/**---------------------------------------------------------------------------*
 **                         PCM Configuration                                 *
 **---------------------------------------------------------------------------*/

typedef enum
{
    PCM_CLK_SRC_DAC = 0,
    PCM_CLK_SRC_ADC
} PCM_CLK_SRC;

typedef enum
{
    PCM_SYNC_WIDTH_1BIT = 0,
    PCM_SYNC_WIDTH_4BIT,
    PCM_SYNC_WIDTH_8BIT,
    PCM_SYNC_WIDTH_15BIT
} PCM_SYNC_WIDTH;

typedef enum
{
    PCM_MODE_uLAW = 0,
    PCM_MODE_ALAW
} PCM_MODE;

typedef enum
{
    PCM_BIT_8 = 0,
    PCM_BIT_16,
    PCM_BIT_32,
    PCM_BIT_64,
    PCM_BIT_128
} PCM_BIT_PER_CHANNEL;

typedef struct
{
    // PCM Output
    BOOLEAN PcmOutputEnable;
    BOOLEAN PcmInputEnable;

    // PCM Input
    BOOLEAN PcmClockMaster;
    BOOLEAN PcmSyncMaster;

    // PCM Compand
    BOOLEAN PcmCompandOn;

    // PCM SDO LSB HZ
    BOOLEAN PcmSdoLSBHZEnable;

    // PCM Clock source
    //PCM_CLK_SRC PcmClkSrc;

    // PCM Sync Width
    PCM_SYNC_WIDTH PcmSyncWidth;

    PCM_BIT_PER_CHANNEL PcmBitPerChannel;

    // PCM Mode
    PCM_MODE PcmMode;
} PCM_CONFIG, *PPCM_CONFIG;

typedef enum
{
    SAM_6000    = 6000,
    SAM_8000    = 8000,
    SAM_16000  = 16000,
    SAM_11025  = 11025,
    SAM_12000  = 12000,
    SAM_24000  = 24000,
    SAM_22050  = 22050,
    SAM_32000  = 32000,
    SAM_44100  = 44100,
    SAM_48000  = 48000


} SAMPLE_RATE;

/**---------------------------------------------------------------------------*
 **                         IIS-PCM Bridge Configuration                      *
 **---------------------------------------------------------------------------*/

typedef enum
{
    PCM_TX_SEL_ADC = 0,
    PCM_TX_SEL_MONOSUM_CIRCUIT
} PCM_TX_SEL;

typedef enum
{
    IIS_TX_SEL_ADC = 0,
    IIS_TX_SEL_PCM_RECEIVER,
    IIS_TX_SEL_DAC,
    IIS_TX_SEL_DISABLE
} IIS_TX_SEL;

typedef enum
{
    DAC_INPUT_SEL_IIS_RECEIVER = 0,
    DAC_INPUT_SEL_PCM_RECEIVER,
    DAC_INPUT_SEL_ADC,
    DAC_INPUT_SEL_DISABLE
} DAC_INPUT_SEL;

typedef enum
{
    MONO_SUM_SEL_DAC = 0,
    MONO_SUM_SEL_IIS_RECEIVER
} MONO_SUM_SEL;

typedef enum
{
    MONO_SUM_MODE_LR = 0,
    MONO_SUM_MODE_L,
    MONO_SUM_MODE_R
} MONO_SUM_MODE;

typedef struct
{
    BOOLEAN AdcSrcEnable;       // Default : disable
    BOOLEAN DacSrcEnalbe;       // Default : disable

    PCM_TX_SEL PcmTxSel;

    IIS_TX_SEL IisTxSel;

    DAC_INPUT_SEL DacInputSel;

    MONO_SUM_SEL MonoSumSel;

    MONO_SUM_MODE MonoSumMode;
} IIS_PCM_BRIDGE, *PIIS_PCM_BRIDGE;

/**---------------------------------------------------------------------------*
 **                         Clock                         *
 **---------------------------------------------------------------------------*/

typedef enum
{
    DAC_OVERSAM_125 = 0,
    DAC_OVERSAM_128,
    DAC_OVERSAM_64,
    DAC_OVERSAM_32
} DAC_OVERSAMPLING;

typedef enum
{
    ADC_OVERSAM_125 = 0,
    ADC_OVERSAM_128
} ADC_OVERSAMPLING;

typedef enum
{
    MCLK = 0,
    PLL_OUTPUT,
    IIS_CLK_IN,
    PCM_CLK_IN
} CLK_SRC;

/**---------------------------------------------------------------------------*
 **                         Basic Configuration                       *
 **---------------------------------------------------------------------------*/
typedef enum
{
    POWER_DOWN = 0,
    STAND_BY,
    ACTIVE_WTO_HP_DET,
    ACTIVE_WT_HP_DET
} CHIP_MODE;

typedef enum
{
    BYPASS_CAP_SIZE_01UF = 0,
    BYPASS_CAP_SIZE_1UF,
    BYPASS_CAP_SIZE_22UF,
    BYPASS_CAP_SIZE_47UF
} BYPASS_CAP_SIZE;

typedef enum
{
    LEFT_SRC_DAC = 0,
    LEFT_SRC_AUX
} LEFT_SRC;

typedef enum
{
    RIGHT_SRC_DAC = 0,
    RIGHT_SRC_AUX
} RIGHT_SRC;

typedef struct
{
    BOOLEAN SideToneSel;        // 0x15, bit0
    BOOLEAN CPISel;             // 0x15, bit1
    BOOLEAN RightSel;           // 0x15, bit2
    BOOLEAN LeftSel;                // 0x15, bit3
    BOOLEAN HpMute;         // 0x15, bit4
} HP_OUTPUT;

typedef struct
{
    BOOLEAN CPISel;             // 0x14, bit0
    BOOLEAN RightSel;           // 0x14, bit1
    BOOLEAN LeftSel;                // 0x14, bit2
    BOOLEAN LsMute;             // 0x14, bit3
} LS_OUTPUT;

typedef struct
{
    BOOLEAN MICSel;             // 0x06, bit0
    BOOLEAN CPISel;             // 0x06, bit1
    BOOLEAN LeftSel;                // 0x06, bit2
    BOOLEAN RightSel;           // 0x06, bit3
    BOOLEAN ADCMute;            // 0x07, bit1
} ADC_INPUT;

typedef enum
{
    VOLUME_LEVEL_MIN = 0,
    VOLUME_LEVEL_01,
    VOLUME_LEVEL_02,
    VOLUME_LEVEL_03,
    VOLUME_LEVEL_04,
    VOLUME_LEVEL_05,
    VOLUME_LEVEL_06,
    VOLUME_LEVEL_07,
    VOLUME_LEVEL_08,
    VOLUME_LEVEL_09,
    VOLUME_LEVEL_10,
    VOLUME_LEVEL_11,
    VOLUME_LEVEL_12,
    VOLUME_LEVEL_13,
    VOLUME_LEVEL_14,
    VOLUME_LEVEL_15,
    VOLUME_LEVEL_16,
    VOLUME_LEVEL_17,
    VOLUME_LEVEL_18,
    VOLUME_LEVEL_19,
    VOLUME_LEVEL_20,
    VOLUME_LEVEL_21,
    VOLUME_LEVEL_22,
    VOLUME_LEVEL_23,
    VOLUME_LEVEL_24,
    VOLUME_LEVEL_25,
    VOLUME_LEVEL_26,
    VOLUME_LEVEL_27,
    VOLUME_LEVEL_28,
    VOLUME_LEVEL_29,
    VOLUME_LEVEL_30,
    VOLUME_LEVEL_MAX
} VOLUME_CONTROL;

typedef struct
{
    CHIP_MODE ChipMode;             // 0x00, bit1:0
    BYPASS_CAP_SIZE BypassCapSize;      // 0x00, bit5:4

    BOOLEAN StereoModeEnable;           // 0x15, bit5

    LEFT_SRC LeftSrc;                   // 0x0F, bit7
    RIGHT_SRC RightSrc;                 // 0x10, bit7

    HP_OUTPUT HpOutput;
    LS_OUTPUT LsOutput;
    ADC_INPUT AdcInput;

    VOLUME_CONTROL AUXLeft;         // 0x0F, bit4:0
    VOLUME_CONTROL AUXRight;            // 0x10, bit4:0
    VOLUME_CONTROL DACLevel;            // 0x11, bit4:0

    BOOLEAN ADCEnable;
    BOOLEAN EnableLS;
    BOOLEAN EnableHP;

    //BOOLEAN AdcMute;                  // 0x07, bit1
} BASIC_CONFIG;

/**---------------------------------------------------------------------------*
 **                         Headset/MIC                       *
 **---------------------------------------------------------------------------*/
typedef enum
{
    OCL_VCM_VOLTAGE_12 = 0,
    OCL_VCM_VOLTAGE_15
} OCL_VCM_VOLTAGE;

typedef enum
{
    MIC_BIAS_VOLTAGE_20 = 0,
    MIC_BIAS_VOLTAGE_25,
    MIC_BIAS_VOLTAGE_28,
    MIC_BIAS_VOLTAGE_33
} MIC_BIAS_VOLTAGE;

typedef enum
{
    MIC_SEL_INT = 0,
    MIC_SEL_EXT
} MIC_SELECT;

typedef enum
{
    INT_MIC_DIFF = 0,
    INT_MIC_SE
} INTERNAL_MIC;

typedef struct
{
    BOOLEAN HPOCLMode;                  // 0x15, bit6
    OCL_VCM_VOLTAGE OCLVCMVoltage;  // 0x0C, bit0

    MIC_BIAS_VOLTAGE MicBiasVoltage;    // 0x0C, bit2:1
    MIC_SELECT MICSelct;                    // 0x0B, bit6

    INTERNAL_MIC InternalMic;           // 0x0B, bit5
} HEADSET_MIC;

/**---------------------------------------------------------------------------*
**                         Interface Property                                *
**---------------------------------------------------------------------------*/

typedef struct
{
    //INTERFACE Interface;      // PCM, IIS : useless

    PPCM_CONFIG pPcmConfig;     // Pointer to PCM configuration
    PIIS_CONFIG pIisConfig;     // Pointer to IIS configuration

    SAMPLE_RATE SampleRate;     // Sample rate

    BASIC_CONFIG BasicConfig;

    HEADSET_MIC HeadsetMic;

    IIS_PCM_BRIDGE IisPcmBridge;
} INTERFACE_PROPERTY, *PINTERFACE_PROPERTY;

extern uint8 LM49370_SetMode (PINTERFACE_PROPERTY pInterface);

#endif  // _LM49370_V3_H_
