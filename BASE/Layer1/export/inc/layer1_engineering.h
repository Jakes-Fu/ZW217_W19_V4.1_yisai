/******************************************************************************
 ** File Name:      layer1_engineering.h                                      *
 ** Author:         Leo Feng                                                  *
 ** DATE:           11/03/2004                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/03/2002     Leo.Feng         Create.                                   *
 ******************************************************************************/

#ifndef _LAYER1_ENGINEERING_H
#define _LAYER1_ENGINEERING_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef uint32 (*L1PAGETIMER_CALLBACK_FUNC)(uint32 param);

//Added for suuprt engineering mode edited by @leo.feng
#define	AFC_CONTROL	0xBD31
#define	AFC_MASK	0x03FF

#define HW_EVT_CTRL 0xBD80
#define CTRL_WORD_LOW(_STARTING_SLOT)  (HW_EVT_CTRL + 2 * (_STARTING_SLOT))
#define CTRL_WORD_HIGH(_STARTING_SLOT) (HW_EVT_CTRL + 2 * (_STARTING_SLOT) + 1)

#define DGAIN_MASK  0xFC00
#define AGAIN_MASK  0x000E
#define LNA_MASK    0x0030
#define RESTORE_GAIN(_dgain,_again,_lna) ( ((_dgain)>>10) + ((_again)<<6) + ((_lna)<<6) + ((_lna)<<4) )
#define CAL_GAIN(_word_l,_word_h) RESTORE_GAIN( (_word_l)&DGAIN_MASK, (_word_h)&AGAIN_MASK, (_word_h)&LNA_MASK)
#define INVALID_TX_PWR_LEV 0xFF
typedef struct{
	uint32 high_level;
	uint32 low_level;
}GPS_CIPHER_REPORT_T;

//GPS call back function
typedef void (*GPS_CALLBACK)(GPS_CIPHER_REPORT_T GPS_cipher);

typedef void (*PWRTRANSMIT_CALLBACK)(BOOLEAN flag);
//end

//it's refered by nv_productionparam_type.h.
typedef struct{
	uint16	PA_type;
	uint16	Transceiver_type;
	uint16	Crystal_type;
	uint16  RF_PARA_RESERVED[3];
}RF_SOLUTION_T;
//end

typedef struct {
    SIGNAL_VARS
    uint32  cur_afc_value;
} APP_L1_GET_AFC_CNF_T;
//typedef yPDef_L1_MN_GET_AFC_CNF  *yPDP_L1_MN_GET_AFC_CNF;
typedef struct {
    SIGNAL_VARS
    uint32  cur_pa_value;
} APP_L1_GET_PA_CNF_T;
//typedef yPDef_L1_MN_GET_PA_CNF  *yPDP_L1_MN_GET_PA_CNF;
typedef struct {
    SIGNAL_VARS
    uint32  cur_agc_value;
} APP_L1_GET_AGC_CNF_T;
//typedef yPDef_L1_MN_GET_AGC_CNF  *yPDP_L1_MN_GET_AGC_CNF;
typedef struct {
    SIGNAL_VARS
    uint32  total_block_count;
    //SDL_Integer  Param2;
    uint32  err_block_count;
} APP_L1_GET_BLOCK_ERROR_RATIO_CNF_T;
//typedef yPDef_L1_MN_GET_BLOCK_ERROR_RATIO_CNF  *yPDP_L1_MN_GET_BLOCK_ERROR_RATIO_CNF;
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 typedef struct
{
	uint16 vX;
	uint16 vY;
	uint16 vZ;
	uint16 vCustomerCode;
	uint16 vBase;
	uint16 vBuild;
	uint16 vSubBuild;
	uint16 vChip;
} DSP_VERSION_T;

// Layer1记录的相关数据
typedef struct
{
    // bsic search number
    uint32 bsic_search_total;       // 请求BSIC Search的总次数
    uint32 bsic_search_succ;        // BSIC Search成功的次数
    uint32 bsic_search_fail;        // BSIC Search Fail Number
    // bsic confirm number
    uint32 bsic_confirm_total;      // 请求BSIC confirm的总次数
    uint32 bsic_confirm_succ;       // BSIC confirm成功的次数
    uint32 bsic_confirm_fail;       // BSIC confirm Fail Number
    // bcch decode number
    uint32 scell_bcchdec_continue;  // 请求连续Decode服务小区BCCH的次数
    uint32 scell_bcchdec_single;    // 请求Decode服务小区单个BCCH块的次数
    uint32 scell_bcch_total;        // 进行对服务小区BCCH Decode的总的Block数
    uint32 scell_bad_bcch;          // 对服务小区BCCH Decode的Bad BCCH的Block数
    uint32 ncell_bcchdec_continue;  // 请求连续Decode临近小区BCCH的次数
    uint32 ncell_bcchdec_single;    // 请求Decode临近小区单个BCCH块的次数
    uint32 ncell_bcch_total;        // 进行对临近小区BCCH Decode的总的Block数
    uint32 ncell_bad_bcch;          // 对临近小区BCCH Decode的Bad BCCH的Block数
    // Rx_Lev number
    uint32 rxlev_total;             // Layer1进行测量的总的次数
    uint32 rxlev_bad;               // Layer1进行重新测量的次数
    // PCH Block Number
    uint32 pch_block_total;         // 收到的PCH总数
    uint32 bad_pch_num;             // Bad的PCH总数
    //dummy pch number
    uint32 dummy_pch_cnt;          //dummy pch总数
    uint32 detected_dummy_pch_cnt;  //dsp探到的dummy pch 数
    uint32 burst_pch_cnt1;	        //接收1个burst的pch
    uint32 burst_pch_cnt2;          //接收2个burst的pch
    uint32 burst_pch_cnt3;          //接收3个burst的pch
    uint32 burst_pch_cnt4;	        //接收4个burst的pch

    // RLA Sort Number
    uint32 rla_sort_total;          // Layer1进行六强排序的总数
    // RACH
    uint32 send_rach_num;           // 发送RACH的次数
    uint32 rx_agch_num;             // 正确接受的AGCH数目
    // PRACH
    uint32 send_prach_num;          // 发送PRACH的次数
    uint32 rx_pagch_num;            // 正确接收的PRACH数目
    
    // BCCH Camp Number
    uint32 bcch_camp_num;           // Layer1进行CampOn的总次数
    // PowerSweep Number
    uint32 pwrswp_num;              // Layer1进行PowerSweep的总次数
    // Handover number
    uint32 handover_total;          // Layer1进行Handover的总次数
    uint32 handover_fail;           // Layer1进行Handover失败的总次数
	uint32 gprs_rcvd_block;
	uint32 gprs_discarded_block;

    
}LAYER1_MONITOR_ITEM_T;

typedef struct
{
    uint32 powersweep_req_0;
    uint32 powersweep_req_1;
    uint32 powersweep_to_layer1_0;
    uint32 powersweep_to_layer1_1;
    uint32 fab_powersweep_cnf_num; 
    //if layer1 doesn't give mph_data_ind when vice scell become main scell, it will be added 1.
    uint32 rach_exp_num;          
    //if rach is delayed 4 seconds to send, it will be added 1.
    uint32 rach_delay_exp_num;   
    uint32 fab_scell_meas_num; 
}RRA_MONITOR_ITEM_T;

typedef enum
{
    L1_RW_RF_REG_OK,
    L1_RW_RF_REG_FAILED,
    L1_RW_RF_REG_PENDING
}L1_RW_RF_REG_RESULT_E;

//extern uint32  g_task_id_for_engineering_mode;
extern uint32  g_total_pch_block_count;
extern uint32  g_error_pch_block_count;
//extern BOOLEAN g_is_engineering_mode;
extern uint32  s_task_id_for_read_dsp_memory;
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get the layer1 monitor item
//	Global resource dependence : g_layer1_monitor_item
//	Note: call by mmi
/*****************************************************************************/
extern void LAYER1_GetLayer1MonItem(
    LAYER1_MONITOR_ITEM_T* item_ptr //[OUT] monitor data
    );
 
 /*--------------------------------------------------
	FUNCTION NAME:	L1API_GetAFCReq
	DESCRIPTION:	request to report current AFC value
	NOTES:          @leo.feng			
---------------------------------------------------*/
extern BOOLEAN L1API_GetAFC(
		uint32 task_id
		);            

/*--------------------------------------------------
	FUNCTION NAME:	L1API_SetAFCReq
	DESCRIPTION:	request to change current AFC value to the given value
	NOTES:          @leo.feng			
---------------------------------------------------*/
extern BOOLEAN L1API_SetAFC(
		uint32 afc_value
		);            

/*--------------------------------------------------
	FUNCTION NAME:	L1API_GetPAReq
	DESCRIPTION:	request to report current PA value
	NOTES:          @leo.feng			
---------------------------------------------------*/
extern BOOLEAN L1API_GetPA(
		uint32 task_id
		);            

/*--------------------------------------------------
	FUNCTION NAME:	L1API_SetPAReq
	DESCRIPTION:	request to change current PA value to the given value
	NOTES:          @leo.feng			
---------------------------------------------------*/
extern BOOLEAN L1API_SetPA(
		uint32 pa_value
		);            

/*--------------------------------------------------
	FUNCTION NAME:	L1API_GetAGCReq
	DESCRIPTION:	request to report current AGC value
	NOTES:          @leo.feng			
---------------------------------------------------*/
extern BOOLEAN L1API_GetAGC(
        uint32 task_id
		);            

/*--------------------------------------------------
	FUNCTION NAME:	L1API_GetBlockErrorRatioReq
	DESCRIPTION:	request to report block error ratio
	NOTES:          @leo.feng			
---------------------------------------------------*/
extern BOOLEAN L1API_GetBlockErrorRatio(
        uint32 task_id
		); 

/*--------------------------------------------------
	FUNCTION NAME:	L1API_ApplyMaxPower
	DESCRIPTION:	request to use maximum power to transmit
	NOTES:          @leo.feng			
---------------------------------------------------*/
BOOLEAN L1API_ApplyMaxPower(void);
		
extern void L1API_DSPCloseAllDevice(void) ;
	


extern void L1API_WakeUpSleepARM(uint32 nearest_timer,uint32 mcu_timer);  

/*-------------------------------------------------
	DESCRIPTION:API to read dsp side memory
--------------------------------------------------*/
extern void L1API_ReadDspMemory(
		int16 ram_flag,					// 1:PRAM, 0: DRAM	
		int16 source_address,			// DSP address to read 		
		int16 length,					// The length of data block. 0 means a single word.
		int32 task_id
		);

/*-------------------------------------------------
	DESCRIPTION:API to write dsp side memory
--------------------------------------------------*/
extern void L1API_WriteDspMemory(
		int16  ram_flag,				// 1:PRAM, 0: DRAM
		int16  destine_address,			// Destine address - DSP address to be wrote to.	
		int16  length,					// Length - The length of data block. 0 means a single word.
		int16  *value_or_source_address	// If the length is 0,the value will be put into the destine address directly.
										// Otherwise a memcpy is performed.	
		);       


/*--------------------------------------------------
	FUNCTION NAME:	L1API_DSPMaxPowerTX
	DESCRIPTION:	Diag 调用此函数来让DSP开启或关闭以最大功率发射的功能
                    Band 设置为GSM900，默认值。
	NOTES:          @leo.feng			
---------------------------------------------------*/
extern void L1API_DSPMaxPowerTX( BOOLEAN is_enable );

/*--------------------------------------------------
	FUNCTION NAME:	L1API_DSPMinPowerRX
	DESCRIPTION:	Diag 调用此函数来让DSP开启或关闭以最小功率接收的功能
	NOTES:          @leo.feng			
---------------------------------------------------*/
extern void L1API_DSPMinPowerRX(BOOLEAN is_enable, uint32 ms_band);

/*****************************************************************************/
// 	Description : get the layer1 monitor item ptr
//	Global resource dependence : g_layer1_monitor_item
//  Author: William Qian
//	Note: call by mn
/*****************************************************************************/
extern void LAYER1_GetLayer1MonItemPtr(
    LAYER1_MONITOR_ITEM_T **item_ptr //[OUT] monitor data ptr
    );


/*****************************************************************************/
//  Description:    check calibration version
//	Global resource dependence:
//  Author:         sunsome.ju
//	Note: 2005 1 12
/*****************************************************************************/
void REF_CheckCalibrationParamVersion(uint16 calib_param_version);     // check calibration param version

/*****************************************************************************/
//  Description:    check downloadparam version
//	Global resource dependence:
//  Author:         sunsome.ju
//	Note: 2005 1 12
/*****************************************************************************/
void REF_CheckDownloadParamVersion(uint16 download_param_version);     // check calibration param version

/*******************************************************************/
//	Function name:   REF_GetDownloadParamVersion
// 	Discription: Return downloadparam nv version.
//  Author: fancier.fan
//	Note:           
/*******************************************************************/
uint16 REF_GetDownloadParamVersion(void);

/*******************************************************************/
//	Function name:   REF_GetCalibrationParamVersion
// 	Discription: Return calibration nv version.
//  Author: fancier.fan
//	Note:           
/*******************************************************************/
extern uint16 REF_GetCalibrationParamVersion(void);

/*----------------------------------------------------------------
	FUNCATION NAME:	REF_SwitchL1WorkMode
	Description: ref use it to switch layer1 work mode between normal and calibration.
-----------------------------------------------------------------*/
void	REF_SwitchL1WorkMode(uint16 mode_to_enter);

/*----------------------------------------------------------------
	FUNCATION NAME:	SCI_StartCalibration
	Description:	create tasks of fake_rr and calibration
	Notes:	
-----------------------------------------------------------------*/
extern void L1API_SCI_StartCalibration(void);

/*****************************************************************************/
// 	Description : get the gprs time slot ptr
//	Global resource dependence : g_layer1_monitor_item
//	Note: call by mn
/*****************************************************************************/
extern BOOLEAN L1API_GetTBFSlotMask(
    uint8* ul_slot_ptr, //[OUT] uplink slot mask pointer
    uint8* dl_slot_ptr  //[OUT] downlink slot mask pointer
    );

/*-----------------------------------------------------------------
 Function Name: DOWNLOADPARAM_GetDSPLogSwitch
 Description: This function returns on/off value for DSP log.

 Notes:
-------------------------------------------------------------------*/
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
extern uint8 L1API_DOWNLOADPARAM_GetDSPLogSwitch( void );
#else
extern BOOLEAN L1API_DOWNLOADPARAM_GetDSPLogSwitch( void );
#endif
/*******************************************************************/
//	Function name:   REF_ShowDspVersion
// 	Discription: Get the adc calibration from the nvitem
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/

extern void REF_GetAdcCalibrationPara( 
							   uint32 battery_calibration[2],
							   uint32 program_calibration[2],
							   uint32 invalid_calibration,
							   uint32 chr_pm_ver_support
							   );
		
/*******************************************************************/
//	Function name:   REF_CheckLayer1State
// 	Discription: Return a flag to indication if g_layer1_state ==
//	             L1_STATE_NULL.
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/
extern uint8 REF_CheckLayer1State(void);
extern void L1API_Audio_Loop_Enable(BOOLEAN bEnableLoop, BOOLEAN bAudioAux);

extern void L1API_CmdQ_SetAudioEnable(BOOLEAN  input_switch, BOOLEAN output_switch);


/*----------------------------------------------------------------
	Description:	In Layer1 test mode, creat layer1 task and dummy PS task.
-----------------------------------------------------------------*/
extern uint32 SCI_StartLayer1Test(void);

/*----------------------------------------------------------------
	Description:	Layer 1 Initialize function, creat layer1 task.    
-----------------------------------------------------------------*/
extern uint32 SCI_InitLayer1(void);


/*****************************************************************************/
//  Description:    This function start gps validation
//	Global resource dependence:
//	Note:     CR91528
/*****************************************************************************/
void L1API_StartGPSValidation(uint32 random_clock,		//CLOCK PRODUCE BY GPS ENGINE
						   GPS_CALLBACK callback_fun		//function which get cipher result
						   );

extern	void L1API_GetRFSolutionParamFromNv(RF_SOLUTION_T *rf_solution);

extern BOOLEAN REF_WriteMarkerValue(uint32 value);

extern uint32 REF_GetMarkerValue(void);

extern uint32 REF_GetCaliDateValue(void);

extern BOOLEAN REF_WriteCaliDateValue(uint32 value);
extern void REF_GetCalibrationPara_Adc(uint32  *adc_ptr,uint32 len);
extern uint32 REF_GetCalibrationPara_PM_Version(void);


extern BOOLEAN REF_SetCalibrationPara_Adc(uint32  *adc_ptr, uint32 len);
extern uint32 * REF_SetMemCalibrationPara_Adc(uint32 * adc_ptr, uint32 adc_len, uint32 * cali_len_ptr);

extern BOOLEAN REF_WriteCaliCDACValue(uint32 value);
extern BOOLEAN REF_GetIsCalibrationedFlag(void);

/*****************************************************************************/
//  Description:   Get the paging-engine-timer's expiring inteval value, that's in ms unit.
//                      i.e. 2500 meas 2.5 seconds.
//                     
//  Global resource dependence:
//  return value : 
//				expire timeout value in ms unit.			
//  Author: 	@arthur
//  Note: 
//
/*****************************************************************************/ 
uint16 L1PagingTimer_GetInteval( void );
/*****************************************************************************/
//  Description:   Create a timer by Registering  callback. (the timer is base on paging cycle engine.)
//                     
//  Global resource dependence:
//  return value : 
// 				TRUE, registered successfuly.
//				FALSE,fail to register.
//  Author:   @arthur
//  Note:
//
/*****************************************************************************/ 
BOOLEAN L1PagingTimer_RegisterCallback(L1PAGETIMER_CALLBACK_FUNC func);
/*****************************************************************************/
//  Description:   delete a timer by unregistering  callback. (the timer is base on paging cycle engine.)
//                     
//  Global resource dependence:
//  return value : 
// 				TRUE, unregistered successfuly.
//				FALSE,the timer hasn't been found.
//  Author:   @arthur
//  Note:
//
/*****************************************************************************/ 
BOOLEAN L1PagingTimer_UnRegisterCallback(L1PAGETIMER_CALLBACK_FUNC func);



/*****************************************************************************/
// 	Description: L1API_GetGsmMaxOutputPower
//	Global resource dependence : none
//  Author: liang.zhu
//  Note:
/*****************************************************************************/
uint8 L1API_GetGsmTestOutputPowerLev(uint32 band, uint8 pwr_db);

/*****************************************************************************/
// 	Description: L1API_GsmTxPwrTest
//	Global resource dependence : none
//  Author: liang.zhu
//  Note:
/*****************************************************************************/
extern void L1API_GsmTxPwrTest( BOOLEAN is_debug_on, 
                         uint16 arfcn,
                         uint8 band,
                         uint16 gainorpower
                        );

/*****************************************************************************/
// 	Description: L1API_GsmRXPwrTest
//	Global resource dependence : none
//  Author: liang.zhu
//  Note:
/*****************************************************************************/
extern void L1API_GsmRXPwrTest( BOOLEAN is_debug_on, 
                        uint16 arfcn,
                        uint8 band);

/*****************************************************************************/
// 	Description: L1API_GetGsmGetRXPwrResult
//	Global resource dependence : none
//  Author: liang.zhu
//  Note:
/*****************************************************************************/
extern BOOLEAN L1API_GetGsmRXPwrResult(
                        uint16 arfcn,
                        uint8  band,
                        int16* result);
/*----------------------------------------------------------------------
	FUNTION NAME:	L1API_GetOTP()
	DESCRIPTION:	return current uplink transmitter power value
	NOTES:		    if this function called not at traffic state,
	                it will return 0xFF as exception value
	                add by janson.liu	
------------------------------------------------------------------------*/
extern uint8 L1API_GetOTP();
						

extern void L1API_SetMaxTxPwrTch(BOOLEAN flag, uint16 pwr_lev);

extern uint16 L1API_GetMaxTxPwrTchFromAtCmd(void);

extern BOOLEAN L1API_GetDspVersion(uint16* chip_id,uint16*  version_id,uint16*patch_id);

extern void L1API_SetGsmMaxTXPwr_Custom(BOOLEAN enable, uint8 pwr_egsm,  uint8 pwr_dcs,uint8 pwr_850, uint8 pwr_pcs);
extern void L1API_SetGsmMaxTXPwr_Custom_Rach(BOOLEAN enable, uint8 pwr_egsm,  uint8 pwr_dcs,uint8 pwr_850, uint8 pwr_pcs);

extern BOOLEAN  L1API_GetGsmMaxPowerTransmitFlag(void);

extern void L1API_SetGsmMaxPowerTransmitFlag(BOOLEAN flag);

extern void  L1API_RegistTXPwr(PWRTRANSMIT_CALLBACK pwr_callback);

extern BOOLEAN L1API_GetTaPwr(uint8* ta,uint8* pwr);

/*****************************************************************************/
//  Description:    This function is used to read/write RF registers
//	Global resource dependence:
//                      P0	P1	            P2	            P3
//  Reg read	        0	RF reg address	X               X
//  Reg write           1   RF reg address	RF reg value    X
//	Note:     
/*****************************************************************************/
extern void L1API_ReadWriteRfRegister(uint16 p0, uint16 p1, uint16 p2, uint16 p3);

/*****************************************************************************/
//  Description:    This function is used to get the read/write RF registers result
//	Global resource dependence:
//                      P0	reg_val	     return value                       
//  Reg read	        0	RF reg val 	 0 ok,1 fail,2 waiting dsp response
//  Reg write           1	X            0 ok,1 fail,2 waiting dsp response
//	Note:     
/*****************************************************************************/
extern L1_RW_RF_REG_RESULT_E L1API_ReadWriteRfRegisterResult(uint16 p0,uint16 *reg_val);


extern void L1API_Rf26MSwitchOnOff(BOOLEAN  on,uint16 reg_val);
extern void L1API_ReadRf26MSwitch(void);

extern void L1API_SetPagingWakeUpReport(BOOLEAN flag);

extern BOOLEAN L1API_GetPagingWakeUpReport(void);

#ifdef __cplusplus
    }  
#endif

#endif  // _LAYER1_AUDIO_H
