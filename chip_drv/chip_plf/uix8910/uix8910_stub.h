/******************************************************************************
 ** File Name:      sc6531efm_spi_cfg.h                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           12/20/2010                                                  *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of spi device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/20/2010     liuhao     Create.                                   *
 ** 05/30/2010     wenjun.shi     Add platform specific information, include:
 **                                threshold ratio info
 **                                interrupt enable bit info
 **                                cs bit info.                                   *
 ******************************************************************************/

#ifndef _UIX8910_STUB_
#define _UIX8910_STUB_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "sci_types.h"
#include "audio_config.h"
#include "ps_eng.h"
#include "mn_type.h"
#include "layer1_engineering.h"
#include "layer1_audio.h"
#include "gsm_gprs.h"
#include "state_machine.h"
#include "layer1_audio_codec.h"
#include "sim_type.h"
#include "sim_to_simat.h"
#include "layer1_sbc.h"
#include "calibration.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
//#define DEBUG_SPI
#ifdef DEBUG_SPI
#define SPI_PRINT   SCI_TRACE_LOW
#else
#define SPI_PRINT(_x_)
#endif

#ifndef CHIP_SC6600C	
 #define NUM_RAMP_POINTS     8 * 2       // Raising edge and falling edge
#else
 #define NUM_RAMP_POINTS     20          //points of ramp up and down totally
#endif

#define NUM_TX_LEVEL        16          // 2db per step
#define NUM_RAMP_RANGES     16        //constant parameter numbers, 16 level

#define MAX_COMPENSATE_POINTS  75
#define RX_CMP_PARAM_NUM_GSM900   35
#define RX_CMP_PARAM_NUM_DCS1800   75
#define RX_CMP_PARAM_NUM_PCS1900   60
#define RX_CMP_PARAM_NUM_GSM850   25

#define NUM_BAND            4           // GSM, DCS, PCS,GSM850
#define NUM_ARFCN_DUAL_BAND 548         //
#define NUM_ARFCN_PCS       374
#define NUM_AGC_BANDS       4           // For most cases 2 bands, LNA and IF amplifier.
#define NUM_TEMP_BANDS      5
#define NUM_GSM_ARFCN_BANDS 6
#define NUM_DCS_ARFCN_BANDS 8
#define NUM_PCS_ARFCN_BANDS 7
#define NUM_GSM850_ARFCN_BANDS 6
#define NUM_FREQ_LEVEL      5           // For frequency compensation
#define NUM_SLOPE_PARA      17        

/**---------------------------------------------------------------------------*
**                               Variable Prototype                        **
**---------------------------------------------------------------------------*/
extern const uint16  s_calibrationparam_version;
extern const uint16  s_downloadparam_version;

extern BLOCK_ID        g_P_CC_ID[MULTI_SYS_NUM];
extern BLOCK_ID       g_P_SMR_ID[MULTI_SYS_NUM];
extern BLOCK_ID       g_P_SIM_ID[MULTI_SYS_NUM];
extern BLOCK_ID        g_P_SS_ID[MULTI_SYS_NUM]; /*lint -esym(552,g_P_SS_ID) */
extern uint32   g_sim_simat_task_id[MULTI_SYS_NUM];



/**---------------------------------------------------------------------------*
**                               Macro Define                        **
**---------------------------------------------------------------------------*/

typedef void (* L1_RFRESET_REG_CALLBACK) (void);

typedef void (*AAC_LC_FRAME_IND_CALLBACK_PFUNC)(
  uint32    *pui_out_pcm_data,
  uint32    pui_pcm_size,
  uint16    us_result
  );

typedef void (*AAC_LC_HEADER_IND_CALLBACK_PFUNC)(
  uint32          ui_sample_rate,
  uint16          us_channel_num,
  uint16          us_result
  );


typedef struct CELL_BASIC_INFO_T_s {
    BOOLEAN  cell_present;
    uint16  arfcn;
    uint8  bsic;
    uint16  rssi;
    BOOLEAN  identity_present;
    uint16  mcc;
    uint16  mnc;
	uint16  mnc_digit_num;
    uint16  lac;
    uint16  cell_id;
} CELL_BASIC_INFO_T;

/****************** Parameters below are RF related. Need to be calibrated for ********/
/**************     handset, the new parameters will be downloaded to Flash by ********/
/**************     a special utility without recompile the software ******************/
#ifdef CHIP_SC6600C


#ifdef SC6600M

typedef struct
{
   // uint16 RF_ID;
   // uint16 RF_Project_ID;	
    uint16 slope_length ;
    uint16 cdac ;
    uint16 cafc ;
    uint16 slope_struct[3][17] ;
    
}RF_aero2_AFC_T ;

typedef struct
{
    /************* AFC  ********************/
    uint16 is_using_si4134;        // TRUE, the si4134 from Silicon lab is used. Then the way to calibrate
                                   // the 13Mhz is different. For si4134, Total 64 steps( 1.0 ppm per step)
                                   // can be set to set coarse 13Mhz.
    uint16 si4134_coarse_13m;      // For si4134, it is 6bits, and for setting the CDAC register of si4134
    
    uint16 afc_center;
    uint16 afc_slope_per_6bits;
    
}RF_aero1_AFC_T ; 
typedef struct
{
    #ifndef _CALIBRATION_VER_0009_
	    /************* AFC  ********************/
	    uint16 rx_ctrl_word_num    ;
	    uint16 tx_ctrl_word_num    ;
	    uint16 crystal_algo_sel    ;
	    uint16 tx_send_random_data ;
	    uint16 afc_dac_bits        ;
	    uint16 gain_index_step     ;
	    uint16 agc_ctrl_step_num   ;
	#endif
    
    RF_aero1_AFC_T rf_aero1_afc_struct;
     // Oscillator temperature calibration table.
    int16  osci_temp_comp_in_dac[ NUM_TEMP_BANDS ];     // In Hz
                                                        // Temp -30 ~ -11 
                                                        // Temp -10 ~ 9 
                                                        // Temp  10 ~ 29 
                                                        // Temp  30 ~ 49 
                                                        // Temp  50 ~ 69 
    RF_aero2_AFC_T rf_aero2_afc_struct;                                                        
} RF_param_common_DSP_use_T;


#else // #ifdef SC6600M

typedef struct
{
    uint16 RF_ID;
    uint16 RF_Project_ID;	
    uint16 slope_length ;
    uint16 cdac ;
    uint16 cafc ;
    uint16 slope_struct[3][17] ;
    
}RF_AFC_T ;

typedef struct
{
    /************* AFC  ********************/
    uint16 is_using_si4134;        // TRUE, the si4134 from Silicon lab is used. Then the way to calibrate
                                   // the 13Mhz is different. For si4134, Total 64 steps( 1.0 ppm per step)
                                   // can be set to set coarse 13Mhz.
    uint16 si4134_coarse_13m;      // For si4134, it is 6bits, and for setting the CDAC register of si4134
    
    uint16 afc_center;
    uint16 afc_slope_per_6bits;
    
     // Oscillator temperature calibration table.
    int16  osci_temp_comp_in_dac[ NUM_TEMP_BANDS ];     // In Hz
                                                        // Temp -30 ~ -11 
                                                        // Temp -10 ~ 9 
                                                        // Temp  10 ~ 29 
                                                        // Temp  30 ~ 49 
                                                        // Temp  50 ~ 69 
    RF_AFC_T rf_afc_struct ;                                                        
} RF_param_common_DSP_use_T;

#endif // #ifdef SC6600M

#else

typedef struct
{
    /************* AFC  ********************/
    uint16 is_using_si4134;        // TRUE, the si4134 from Silicon lab is used. Then the way to calibrate
                                   // the 13Mhz is different. For si4134, Total 64 steps( 1.0 ppm per step)
                                   // can be set to set coarse 13Mhz.
    uint16 si4134_coarse_13m;      // For si4134, it is 6bits, and for setting the CDAC register of si4134
    uint16 afc_center;             // DAC value for Nominal 13Mhz.
    uint16 afc_slope_hz_per_6bits; // the frequency offset value once DAC value shifts 6bit.
    // Oscillator temperature calibration table.
    int16  osci_temp_comp_in_dac[ NUM_TEMP_BANDS ];     // In Hz
                                                        // Temp -30 ~ -11 
                                                        // Temp -10 ~ 9 
                                                        // Temp  10 ~ 29 
                                                        // Temp  30 ~ 49 
                                                        // Temp  50 ~ 69 
} RF_param_common_DSP_use_T;


#endif

typedef struct 
{
	int16 stage0[NUM_RAMP_POINTS];
	int16 stage1[NUM_RAMP_POINTS];
	int16 stage2[NUM_RAMP_POINTS];
	int16 stage3[NUM_RAMP_POINTS];
	int16 stage4[NUM_RAMP_POINTS];
	int16 stage5[NUM_RAMP_POINTS];
	int16 stage6[NUM_RAMP_POINTS];
	int16 stage7[NUM_RAMP_POINTS];
	int16 stage8[NUM_RAMP_POINTS];
	int16 stage9[NUM_RAMP_POINTS];
	int16 stage10[NUM_RAMP_POINTS];
	int16 stage11[NUM_RAMP_POINTS];
	int16 stage12[NUM_RAMP_POINTS];
	int16 stage13[NUM_RAMP_POINTS];
	int16 stage14[NUM_RAMP_POINTS];
	int16 stage15[NUM_RAMP_POINTS];

}RF_ramp_table_T;

typedef struct
{

#ifndef CHIP_SC6600C	
	int16 RF_ramp_up_table_stage0[NUM_RAMP_POINTS]; //64 words totally
    int16 RF_ramp_up_table_stage1[NUM_RAMP_POINTS];
    int16 RF_ramp_down_table_stage0[NUM_RAMP_POINTS];
    int16 RF_ramp_down_table_stage1[NUM_RAMP_POINTS];
#else
    int16 rf_ramp_param_constant_up[NUM_RAMP_RANGES];//const part of the ramp tale
    int16 rf_ramp_param_constant_down[NUM_RAMP_RANGES];//const part of the ramp tale
    int16 rf_ramp_up_param_num;//the number of ramp up parameters in the ramp parameters table for one level
    int16 rf_ramp_down_param_num;//the number of ramp down
/*#ifndef _EDGE_DEV_ 
    int16 reserved[64 - 2*NUM_RAMP_RANGES - 1 - 1];//keep 64words totally to keep consistency with dsp code
#else
    int16 reserved[14];
    int16 RF_ramp_delta_timing_8PSK[NUM_TX_LEVEL];
#endif*/
 //byhwt-2  change for edge
    int16 temperature_gsm_tx_compensate_value[9];
    int16 temperature_edge_tx_compensate_value[9];
    int16 volatge_gsm_txpower_compensate_value[6];
    int16 volatge_edge_txpower_compensate_value[6];


#endif
    int16 TX_compensate_boundary_arfcn_index[4];
    int16 RF_ramppwr_step_factor[5][NUM_TX_LEVEL];
    int16 RF_ramp_delta_timing[NUM_TX_LEVEL];
    int16 RF_ramp_PA_power_on_duration[NUM_TX_LEVEL];
#ifndef CHIP_SC6600C	
    int16 RX_compensate_boundary_arfcn_index[4];
    int16 RX_compensate_value[5];
#else
#ifndef _EDGE_DEV_ 
    int16 reserved2[9];
#else
    int16 RF_8PSK_step_factor;
    int16 rf_tx_gain_8PSK[8];
#endif
#endif
    int16 temperature_RX_compensate_boundary_ADC[4];
    //int16 temperature_RX_compensate_value[5];
     int16 temperature_rx_level_compensate_value[5];
    int16 temperature_TX_compensate_boundary_ADC[4];
    int16 temperature_TX_rampwr_step_factor[5];
    //int16  ramp_switch_level;
    //uint16 afc_alg_bias;
    int16 rf_edge_delta_timing;
    int16 rf_edge_power_on_duration;

    uint16 agc_ctrl_word[91];   // The index is the expected received signal strangth in db in 2 db steps,
                                    // the content is the control word set the RF receiver chain agc.
    
    int16  max_rf_gain_index;
#ifndef _EDGE_DEV_
    int16  reserve_for_future[20];
#else
    int16  RF_ramp_PA_power_on_duration_8PSK[NUM_TX_LEVEL];
    int16  reserve_for_future[4];
#endif
#ifdef CHIP_SC6600C	    
    int16 RX_compensate_value[MAX_COMPENSATE_POINTS];
    RF_ramp_table_T RF_ramp_table;
#endif
    
} RF_param_band_DSP_use_T;

typedef struct
{
	RF_param_common_DSP_use_T   rf_common_param_dsp_use;
	RF_param_band_DSP_use_T     rf_gsm_param_dsp_use;
	RF_param_band_DSP_use_T     rf_dcs_param_dsp_use;
	RF_param_band_DSP_use_T     rf_pcs_param_dsp_use;
	RF_param_band_DSP_use_T     rf_gsm850_param_dsp_use;	
}RF_param_DSP_use_T;

    //**********************************************************************************************
    // ********* parameters below need to be calibrated if the types of RF components have ********
    // ********* been changed. Do not need to calibrate for each phone *****************************
    //**********************************************************************************************
    // Due to unideal performance of the SAW filter at front and unflat frequency response,
    // the rx level measure may be uneven on different frequency channel.
typedef struct
{
    int8    rx_whole_band_comp_gsm[NUM_GSM_ARFCN_BANDS];
            // channel 1-25 //
            // channel 26-50 //
            // channel 51-75 //
            // channel 76-100 //
            // channel 101-124 //
            // channel 975-1024 //
    int8    rx_whole_band_comp_dcs[NUM_DCS_ARFCN_BANDS];
            // Channel 512-550 //
            // Channel 551-600 //
            // Channel 601-650 //
            // Channel 651-700 //
            // Channel 701-750 //
            // Channel 751-800 //
            // Channel 801-850 //
            // Channel 851-886 //
    int8    rx_whole_band_comp_pcs[NUM_PCS_ARFCN_BANDS];
            // Channel 512-550 //
            // Channel 551-600 //
            // Channel 601-650 //
            // Channel 651-700 //
            // Channel 701-750 //
            // Channel 751-800 //
            // Channel 801-810 //
     int8    rx_whole_band_comp_gsm850[NUM_GSM850_ARFCN_BANDS];        
    // PA temperature compensation.
    int8    pa_GSM_temp_comp[ NUM_TEMP_BANDS];
            // Temp -30 ~ -11 //
            // Temp -10 ~ 9 //
            // Temp  10 ~ 29 //
            // Temp  30 ~ 49 //
            // Temp  50 ~ 69 //

    int8    pa_DCS_temp_comp[ NUM_TEMP_BANDS];
            // Temp -30 ~ -11 //
            // Temp -10 ~ 9 //
            // Temp  10 ~ 29 //
            // Temp  30 ~ 49 //
            // Temp  50 ~ 69 //

    int8    pa_PCS_temp_comp[ NUM_TEMP_BANDS];
            // Temp -30 ~ -11 //
            // Temp -10 ~ 9 //
            // Temp  10 ~ 29 //
            // Temp  30 ~ 49 //
            // Temp  50 ~ 69 //
	int8    pa_GSM850_temp_comp[ NUM_TEMP_BANDS];

    /******************** RX level calculation *********************/
    // The received signal level(RSSI) value reported to the network may need to be adjusted based on the received signal
    // level.
    int8    rxlev_GSM_temp_comp[ NUM_TEMP_BANDS];
            // Temp -30 ~ -11 //
            // Temp -10 ~ 9 //
            // Temp  10 ~ 29 //
            // Temp  30 ~ 49 //
            // Temp  50 ~ 69 //

    int8    rxlev_DCS_temp_comp[ NUM_TEMP_BANDS];
            // Temp -30 ~ -11 //
            // Temp -10 ~ 9 //
            // Temp  10 ~ 29 //
            // Temp  30 ~ 49 //
            // Temp  50 ~ 69 //

    int8    rxlev_PCS_temp_comp[ NUM_TEMP_BANDS];
            // Temp -30 ~ -11 //
            // Temp -10 ~ 9 //
            // Temp  10 ~ 29 //
            // Temp  30 ~ 49 //
            // Temp  50 ~ 69 //
	int8    rxlev_GSM850_temp_comp[ NUM_TEMP_BANDS];
	
    /****** PA ramp compensation for battery voltage variation. *******/
    int8    pa_GSM_vcc_comp_threshold;      // If the voltage is below the threshold, pa compensation is needed. Battery measure ADC value.
    int8    pa_GSM_vcc_comp_step;           // The PA ramp level need to be changed per threshold.
    int8    pa_DCS_vcc_comp_threshold;      // If the voltage is below the threshold, pa compensation is needed.
    int8    pa_DCS_vcc_comp_step;           // The PA ramp level need to be changed per threshold.
    int8    pa_PCS_vcc_comp_threshold;      // If the voltage is below the threshold, pa compensation is needed.
    int8    pa_PCS_vcc_comp_step;           // The PA ramp level need to be changed per threshold.
    int8    pa_GSM850_vcc_comp_threshold;      // If the voltage is below the threshold, pa compensation is needed. Battery measure ADC value.
    int8    pa_GSM850_vcc_comp_step;           // The PA ramp level need to be changed per threshold.

} RF_param_MCU_use_T;


/******************Parameters below are for battery and 13M clock ect. ******/
typedef struct
{
    /**************** Battery monitor *********************/
    // Battery measurement calibration.
    // Due to the unprecise of the divider resistor value for measure the battery voltage, the voltage
    // value measured may be not accurate enough for battery charging.
    uint32 batt_mV_per_4bits;   // The actual voltage for 4bits ADC output.

    /******************* 32K/13M calibration. ***************/
    /**** the ratio of 32k and 13Mhz needs to be calibrated running ***/
    /***  for deep sleep mode ****/

    uint16  clock_32k_cal_interval; // How often the calibration shall be invoked.
    uint16  clock_32k_cal_duration; // How long the calibration shall be done.
}Misc_cal_MCU_use_T;

//
// This structure defined by Lin.liu(2003-0829) for ADC calibration. CR: MS4474
//
typedef struct
{
    uint32    adc[2];           // calibration of ADC, two test point
    uint32    battery[2];       // calibraton of battery(include resistance), two test point
    uint32    reserved[8];      // reserved for feature use.
}ADC_T;
// End Added by Lin.liu

typedef struct
{
    uint16              calibration_param_version;
    RF_param_DSP_use_T  rf_param_dsp_use;
    RF_param_MCU_use_T  rf_param_mcu_use;
    Misc_cal_MCU_use_T  misc_cal_mcu_use;
    int16                   mic_gain;        // In DB
    int16                   auxi_mic_gain;   // In DB
    uint16                  PM_version;      // the version of phone module

    // Following Added by Lin.liu. (2003-08-29), CR: MS4474
    uint16                  software_version;   // Software version of Tools
    uint16                  station_num;
    uint8                   operate_code[ 16 ];
    uint32                  date;

    ADC_T                   adc;             // Added By Lin.liu, for ADC calibration
    
    // End Lin.liu Added.
}calibration_param_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

PUBLIC void L1API_RegisterRFResetFunc(L1_RFRESET_REG_CALLBACK func);
PUBLIC void REF_GetAdcCalibrationPara( 
							   uint32 battery_calibration[2],
							   uint32 program_calibration[2],
							   uint32 invalid_calibration,
							   uint32 chr_pm_ver_support
							   );

PUBLIC uint32 REF_GetMarkerValue(void);
PUBLIC void LAYER1_CfgAmrParam( AMR_MODE amr_mode, uint8 channels, uint8 sample_rate );
PUBLIC uint32 LAYER1_EnableVoiceLoopback(
   BOOLEAN is_enable,             //0: disable  1:enable
    uint16  usLoopbackType,      // 
    uint16  usVoiceFormat,
    uint16  usReserved            // millisecones
    );

PUBLIC uint32 LAYER1_GetAudioEngineeringParameter( // If succeed, return SCI_SUCCESS,
                                                   // else return SCI_ERROR
    AUDIO_DEVICE_MODE_TYPE_E  parameter_type,     
    AUDIO_TEST_DATA_T   *param_data				 
    );

PUBLIC uint32 LAYER1_SetAudioEngineeringParameter( // If succeed, return SCI_SUCCESS,
                                                   // else return SCI_ERROR
    AUDIO_DEVICE_MODE_TYPE_E  parameter_type,     
    AUDIO_TEST_DATA_T   param_data				 
    );

PUBLIC uint32 LAYER1_SwitchTestMode(
	BOOLEAN is_test_mode
	)  ;

PUBLIC void REF_GetCalibrationPara_Adc(uint32  *adc_ptr,uint32 len);

PUBLIC BOOLEAN  REF_SetCalibrationPara_Adc(uint32  *adc_ptr, uint32 len);

PUBLIC BOOLEAN REF_WriteMarkerValue(uint32 value);

PUBLIC uint32 * REF_SetMemCalibrationPara_Adc(uint32  *adc_ptr, uint32 len, uint32 * cali_len_ptr);

PUBLIC void L1API_DSPCloseAllDevice() ;

PUBLIC void L1API_DSPMaxPowerTX( BOOLEAN is_enable );

PUBLIC void L1API_DSPMinPowerRX(BOOLEAN is_enable, uint32 ms_band);

PUBLIC void GetCellInfoFromPs(
    DIAG_CELL_INFO_LOG_T *cell_info // Output: cell information sent to Monitoring tooL
);

PUBLIC void PS_CancelForceCampon(void);

PUBLIC void PS_ForceCampon(
    uint16 sim_no,
    uint16 arfcn_count,  ///1 -- 6
    uint16 *arfcn_list
);

PUBLIC BOOLEAN MM_SetdisablePowerSweep(BOOLEAN is_disable);

PUBLIC void RRC_GetCellBasicInfo(
                          uint32 dual_sys,
                          DIAG_CELL_INFO_T *scell_info,
                          DIAG_CELL_INFO_T *ncell_info
                          );

PUBLIC void RRC_GetCellInfoPtr( 
                               uint32 dual_sys,
                               CELL_INFO_T ** cell_arr_ptr, //output cell information array address 
                               int16 ** scell_index_ptr     //output serving cell index of cell array
                               );

PUBLIC int32 RRC_GetTimesOfNoCellAvailable(uint32 dual_sys);

PUBLIC int32 RRC_GetTimesOfPowerSweep(uint32 dual_sys);

PUBLIC void RRPROC_Set_disablePwr(BOOLEAN disable_Pwr);


PUBLIC void RRPROC_Set_identifyBSMode(BOOLEAN identify_mode);

PUBLIC void SCI_SaveBaseMsg(
            xSignalHeaderRec *sig_ptr,
            uint32           comm_ref
            );


PUBLIC BOOLEAN L1API_GetBlockErrorRatio(uint32 task_id);

PUBLIC void LAYER1_GetLayer1MonItemPtr(
    LAYER1_MONITOR_ITEM_T **item_ptr //[OUT] monitor data ptr
    );

PUBLIC void RRA_GetRraMonItem(
        RRA_MONITOR_ITEM_T *item_ptr
        );

PUBLIC uint32 LAYER1_RxADCOnOff(   // If succeed, return SCI_SUCCESS,
                                        // else return SCI_ERROR
    BOOLEAN     		 flag            // If uplink this tone.(SCI_TRUE: Uplink)
    );

PUBLIC uint32 LAYER1_PreSetAudioEnable(   // If succeed, return SCI_SUCCESS,
               BOOLEAN  is_enable      // else return SCI_ERROR       
               );

PUBLIC uint32 LAYER1_SetAudioEnable(   // If succeed, return SCI_SUCCESS,
               BOOLEAN  is_enable      // else return SCI_ERROR       
               );

PUBLIC void LAYER1_SetDevVolume(uint16 volume_level);

PUBLIC void LAYER1_SetDlnkMute(BOOLEAN is_enable,uint16 dlnk_mute_type);

PUBLIC  void LAYER1_SetMagicVoiceParam( uint16  type,
                                uint16  uplink_value, 
                                uint16  downlink_value);

PUBLIC void LAYER1_StartMagicVoiceFunc(MAG_CALLBACK       callback_fun,
                                uint16              type,     //used to indicate which direction is valid.
                                uint16	            uplink_value,
                                uint16              downlink_value);
PUBLIC void LAYER1_StopMagicVoiceFunc() ;

PUBLIC void LAYER1_WriteVoiceData (uint16 *source_buf_ptr,uint16 data_length);

PUBLIC  void LAYER1_ReadVoiceData( uint16 *destination_buf_ptr,uint16 data_length );

PUBLIC void LAYER1_SetDevModeParam(
	    uint16  *param_buffer_ptr,//buffer where stores the parameter of one mode
	    uint32 length           //byte uint
	    );

PUBLIC void GMM_GetGU( 
                               uint32                dual_sys,
                               GMM_STATE_UPDATED     **gu_ptr
                               );
PUBLIC void GMM_GetTLLICur( 
                               uint32     dual_sys,
                               TLLI_T     **tlli_cur_ptr
                               );
PUBLIC BOOLEAN L1API_GetPagingWakeUpReport(void);

PUBLIC BOOLEAN L1API_GetTBFSlotMask(
    uint8* ul_slot_ptr, //[OUT] uplink slot mask pointer
    uint8* dl_slot_ptr  //[OUT] downlink slot mask pointer
    );

PUBLIC void L1API_SetPagingWakeUpReport(BOOLEAN flag);


PUBLIC BOOLEAN LLCENG_IsDataCiphered(void);

PUBLIC BOOLEAN PS_GetPagingWakeUpReport(void);

PUBLIC void PS_SetPagingWakeUpReport(BOOLEAN flag);

PUBLIC void RRC_GetPowerSweepInfoPtr( 
                               uint32 dual_sys,
                               ARFCN_POWERLEVEL_T ** power_sweep_result,
                               int16 ** arfcn_num
                               );

PUBLIC void RRC_GetRrResPtr(uint32 dual_sys, RES_PARAM_T **rr_res_ptr);

PUBLIC RR_STATUS_INFO_T* RR_GetStatusInfo(uint32 dual_sys);

PUBLIC void SM_GetNsapiNumPtr( 
                               uint32       dual_sys,
                               int16        *nsapi_num_ptr
                               );

//PUBLIC SIGNAL_ACTION_E DiscardSignal(
//    xSignalHeaderRec  *sig_ptr,     // input signal address
//    void              *argv_ptr     // input sth else,not used now
//);


PUBLIC ENG_GMM_STATUS_INFO_T* ENG_GMM_GetStatusInfo(uint32 dual_sys);


PUBLIC BOOLEAN L3FUNC_FindPlmnInAvailablePlmnlist(
    PLMN_T      plmn,     // the special plmn to be found
    AVAILABLE_PLMN_LA_LIST_T *list_ptr // pointer the plmn list
);

PUBLIC BOOLEAN L3FUNC_FindPlmnInFplmslist(
					PLMN_T            plmn,     // the special PLMN to be found
					FPLMN_LIST_T      *list_ptr // pointer to the fplmn list
					);

PUBLIC BOOLEAN L3FUNC_FindPlmnInLongFplmnlist(
    const PLMN_T            plmn,     // the special PLMN to be found
    const FPLMN_LONG_LIST_T *list_ptr // pointer to the long fplmn list
);


PUBLIC BOOLEAN L3FUNC_UpdateFplmnList( // true means update ok,false means no operate
    BOOLEAN         del_flag, // false means add, true means delete
    PLMN_T          plmn,     // specified item be added or be deleted
    FPLMN_LIST_T    *list_ptr // forbidden plmn list
);


PUBLIC BOOLEAN L3FUNC_UpdateLongFplmnList( // true means update ok,false means no operate
    BOOLEAN           del_flag, // false means add, true means delete
    PLMN_T            plmn,     // specified item be added or be deleted
    FPLMN_LONG_LIST_T *list_ptr // long forbidden plmn list
);


PUBLIC uint32 PS_TransStrToBCD(
                         char  *bcd_str_ptr,    // in: bcd string
                         uint8 *bcd_data_ptr,   // in/out: buffer for bcd data
                         int16 *bcd_data_len    // in: max bytes of the buffer for bcd data
                                                // out: bytes indicate the length of bcd data
                         );

//PUBLIC SIGNAL_ACTION_E SaveSignal(
//    xSignalHeaderRec  *sig_ptr,     // input signal address
//    void              *argv_ptr     // input sth else,not used now
//);


PUBLIC BOOLEAN REF_GetIsCalibrationedFlag(void);


PUBLIC BOOLEAN SIM_IsCallControlSupport(uint8 cur_sim_task);

PUBLIC BOOLEAN REF_WriteCaliCDACValue(uint32 value);


PUBLIC int32 SM_CompareQosValid(                           //RETURN:
                         TOTAL_QOS_T    *qos1_ptr,  //IN:
                         TOTAL_QOS_T    *qos2_ptr   //IN:
                         );

PUBLIC void SM_SetSubscribeQosValue(
                             TOTAL_QOS_T* qos_ptr
                             );


PUBLIC BOOLEAN RLC_IsGPRSActive(void);


PUBLIC void RRPROC_SetTbfPaging(
    uint16  dual_sys,      //in -- system index, 0 or 1
    BOOLEAN tbf_paging_on  //in -- 
);


PUBLIC BOOLEAN SM_SetQos(
                  BOOLEAN         is_set_qmin,    //IN: true, is set min qos
                  TOTAL_QOS_T     *qos_ptr        //IN/OUT:
                  );

PUBLIC void LAYER1_AUDIO_CODEC_CopyData(
    DSP_CODEC_DATA_T *data_ptr
    );

PUBLIC void LAYER1_AUDIO_CODEC_CopyLRData(
    DSP_CODEC_DATA_LR_T *data_ptr
    );

PUBLIC void LAYER1_AUDIO_CODEC_GetAddressBase(
    AUDIO_CODEC_ARM_DSP_ADDRESS_MAP_T * const address_base_ptr
    );

PUBLIC BOOLEAN L1API_GetGsmRXPwrResult(
                        uint16 arfcn,
                        uint8  band,
                        int16* result);

PUBLIC void L1API_GsmRXCWPwrTest( BOOLEAN is_debug_on, 
                        uint16 arfcn,
                        uint8 band,
                        uint16 gain);

PUBLIC void L1API_GsmTxCWPwrTest( BOOLEAN is_debug_on, 
                         uint16 arfcn,
                         uint8 band,
                         uint16 gainorpower
                        );

PUBLIC void L1API_ReadWriteRfRegister(uint16 p0, uint16 p1, uint16 p2, uint16 p3);

PUBLIC L1_RW_RF_REG_RESULT_E L1API_ReadWriteRfRegisterResult(uint16 p0,uint16 *reg_val);

PUBLIC uint32 LAYER1_SetVolume( // If succeed, return SCI_SUCCESS,
                                // else return SCI_ERROR
    uint16  speaker_vol // Volume value of speaker(0 - DSP_VOL_SPEAKER_MAX)
    );

PUBLIC void PS_GetRRCellsInfoBsicAndEx(
	uint16 dual_sys, 
	RR_CELLS_INFO_T *rr_cells_info_bsic_ptr, 
	RR_CELLS_INFO_TRAFFIC_T *rr_cells_info_bsic_traffic_ptr, 
	RR_CELLS_INFO_EX_T *rr_cells_info_ex_ptr,
	BOOLEAN *is_traffic_ptr);

PUBLIC void LAYER1_StartPlay( AUD_REC_FORMAT_E voice_format,
					           PLAY_CALLBACK callback_fun,
				               BOOLEAN is_to_speaker,
				               BOOLEAN is_to_net
	                          );

PUBLIC void LAYER1_StopPlay( void );


PUBLIC void SIMSIGNAL_SIMDphoneInitReq(uint32 cur_sim_task, BOOLEAN is_entire_service);

PUBLIC void LAYER1_StartRecord( AUD_REC_FORMAT_E voice_format,
					    RECORD_CALLBACK callback_fun,
		                BOOLEAN is_from_mic,
		                BOOLEAN is_from_net
			   			);

PUBLIC void LAYER1_StopRecord( void );

PUBLIC uint32 LAYER1_DownlinkMuteEnable (
    	BOOLEAN mute_enable,
    	uint16  mute_type
);

PUBLIC uint32 LAYER1_MuteSwitch(    // If succeed, return SCI_SUCCESS,
                                    // else return SCI_ERROR
    BOOLEAN b_mute,                 // SCI_TRUE: Mute ON
    uint16  mute_type               // add for bluetooth mode 
    );

PUBLIC SIM_FILE_NAME_E SIMFS_GetFileName(
    uint32 cur_sim_task,
    uint16 file_id,  //the file id
    uint8  path_id_len,
    uint8  *path_id);

PUBLIC SIM_FILE_TYPE_E SIMFS_GetFileType(  //return value:the file's file type
    SIM_FILE_NAME_E file_name   //the file name
);


PUBLIC int8 SIMFS_FileRouting( //reutrn value:the number of file in the routing to the destionation file
    SIM_FILE_NAME_E file_name,  //the desitination file
    SIM_FILE_NAME_E cur_file,   //the current selected direction file
    SIM_FILE_NAME_E *file_route //the file route to the destionation file
);

PUBLIC uint16  SIMFS_GetFileID(    //return value:the file ID
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name     //the file name
);

PUBLIC  void LAYER1_SendVoiceDataToDA(uint16 *data_ptr);

PUBLIC  void LAYER1_SendVoiceDataToNet(uint16 *data_ptr);

PUBLIC void LAYER1_StartRecord_Ad(RECORD_CALLBACK2 callback);

PUBLIC void LAYER1_StartRecord_Downlink(RECORD_CALLBACK2 callback);

PUBLIC  void LAYER1_StopRecord_Ad(void);

PUBLIC void LAYER1_StopRecord_Downlink(void);

PUBLIC void L1_AAC_DecodeFrame(//return none
    uint8* uc_frame_ptr, //pointer of input frame data
    uint16 us_frame_len, //length of input frame data
    AAC_LC_FRAME_IND_CALLBACK_PFUNC callback_func_ptr//pointer of callback which
                //is used to inform codec the message that decode frame is over
    );

PUBLIC void L1_AAC_DecodeHeader(  //return none
    uint8* uc_header_ptr,   //pointer of input header data
    uint16 us_header_len,   //length of input header data
    AAC_LC_HEADER_IND_CALLBACK_PFUNC callback_func_ptr//pointer of callback which 
                  //is used to inform codec the message that decode header is over
    );

PUBLIC void L1_AAC_PcmDataCopy(
    uint16 *us_dest_l_ptr, //pointer of dest left pcm data
    uint16 *us_dest_r_ptr, //pointer of dest right pcm data
    uint32 ui_len
    );


PUBLIC BOOLEAN L1_SBC_Config_Encoder  (uint8  channel_mode, 
                             uint8  alloc_method,
                             uint16 sample_freq, 
                             uint8  nrof_blocks, 
                             uint8  nrof_subbands, 
                             uint8  bitpool,
                             SBC_CONFIG_ENCODER_IND_CALLBACK_PFUNC callback_func_ptr);

PUBLIC BOOLEAN L1_SBC_Encode_Frame(int16* ps_left_pcm, int16* ps_right_pcm, uint16 us_pcm_len, uint8* puc_frame, uint16 us_frame_len, uint8 bitpool, uint16 us_frame_size, SBC_ENCODE_FRAME_IND_CALLBACK_PFUNC callback_func_ptr);

PUBLIC void LAYER1_TRANPCM(uint16 srcAddr, uint16 scrSampleRate, uint16 sampleNum, void * callback);

PUBLIC void LAYER1_TRANPCM_Config(uint16 samplerate);


PUBLIC void Layer1_Audio_Register(
	LAYER1_AUDIO_CALLBACK callback_fun
);

PUBLIC void Layer1_Cmd_Audio(Audio_CmdQ_T *cmd_msg);


PUBLIC void LAYER1_AUDIO_CODEC_Data(//return none
    DSP_CODEC_SRC_DATA_T * data_info
    );

PUBLIC uint16 CAL_GetCalibrationParamSize(CALIBRATION_PARAM_E  param_type);

PUBLIC void RRPROC_Set_identifyBS(BOOLEAN start_identify);

PUBLIC void LAYER1_GetLayer1MonItem(
    LAYER1_MONITOR_ITEM_T* item_ptr //[OUT] monitor data
    );


PUBLIC uint32 PS_TransBCDToStr(
                         uint8  *bcd_data_ptr,  // in: bcd data
                         int16  bcd_data_len,   // in: length of bcd data
                         char   *bcd_str_ptr,   // in/out: buffer for bcd string
                         int16  max_bcd_str_len // in: max bytes of the buffer for bcd string
                         );
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
