/******************************************************************************
 ** File Name:      ref_param.h                                               *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/03/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain some Ref parameter.                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/03/2003     Xueliang.Wang    Create.                                   *
 ** 11/03/2010    	fei.zhang		    Add DSP USB Log function       *
 ******************************************************************************/
#ifndef _REF_PARAM_H
#define _REF_PARAM_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h" 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Defines                                           *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 
// Charge Param defined by lin.liu
// 
#define CHR_TABLE_SIZE    16

#define LOUD_PARA_COUNT        5

#define AUDIO_SAMPLERATE_COUNT  9

// All the voltage unit is mV
typedef struct
{
    uint16       voltage_warning;     // when low than the voltage, send the warning msg to client.
    uint16       voltage_shutdown;    // when low than the voltage, send the shutdown msg to client.
    uint16       voltage_shutdown_tx; // when the tx voltage is low than the voltage, send the 
                                      // shutdown msg to client.
    uint16       adjust_backlight;    //
    uint16       adjust_call;         //
    
    uint16       float_range;         // when the voltage is low(high) than 
    
    // ADC to Voltage table, from 2.8V to 4.2V, and the last one is 0xFFFF;
    uint16       adc_voltage_table[ CHR_TABLE_SIZE ];
    
    // The table define as following:
    //  If the voltage >= the low WORD of table[n], the capacity is the high WORD of table[n].
    // The last one item of the table must be 0xFFFFFFFF
    uint16       voltage_capacity_table[ CHR_TABLE_SIZE ];
    
    // We read ADC result when a tx occur, and after reach the tx_count, then average and report
    // to chr_stm
    uint16       tx_count;
    uint16       battery_count;
    
    // When discharge, if the capacity is descending and reach the "count_1", 
    //     then change the capacity that report to client.
    //     if the capacity is ascending and reach the "count_2", then chanrge the capacity.
    // When charge, it is opposite.
    //     If the capacity is descending and reach the "count_2", then change the capacity.
    //     If the capacity is ascending and reach the "count_1", then change the capacity.
    uint16       capacity_change_count_1; 
    uint16       capacity_change_count_2;
    
    uint16       warning_count;  // when reach the warning count, will send the voltage warning to client.
    
    // send the one or more warning msg before send shutdown msg.
    uint16       warning_count_before_shutdown; 
    
    // we average the adc result in the recent average count
    uint16       average_count;
    
    uint16       reserved[ 11 ];
}CHR_PARAM_T;   // Size is 112 bytes
//
//
typedef struct RTC_CALIBRATION_INFO_tag
{
	uint32	last_cal_time;	
	uint32	last_cal_result;
	uint8	reserved1;
	int8	is_update_nv;
	int16	total_delta_seconds;
	int32	last_cal_numerator;	
} RTC_CALIBRATION_INFO_T;

typedef struct AUD_ARMREF_PARAM_tag
{
    int16 loud_param[AUDIO_SAMPLERATE_COUNT][LOUD_PARA_COUNT];
    uint8 lcf_on;
    uint8 loud_on;
    uint8 speaker_stereo;
    uint8 u8reserved[7];
    int16 s16reserved[4];
} AUD_ARMREF_PARAM_T;
 
// Size of this strucure should be 300 bytes !!!
#define REF_PARAM_T_SIZE             300
typedef struct REF_PARAM_tag
{
    uint32  uart_mode; 
    uint32  uart_0_baud_rate;
    uint32  uart_1_baud_rate;
    uint8   auto_power_on;      // 1: Auto PowerOn      0: Don't auto PowerOn
	/*@jim.zhang CR:MS8181 2004-03-29 */
    uint8	uart0_as_data;		// 1: uart0 used as DATA 0:used as DEBUG
   // uint8	uart1_as_debug;		// 1: uart1 used as debug 0:used as data
    /* end CR:MS8181 */
    /* @Jim.zhang CR:MS8384 2004-04-02 */
    uint8	gprs_debug;					//1:GPRS_DEBUG that mix mode(log + AT). 0: not mix mode
    uint8   sleep_need_emtpy_uart_fifo;	// 1: when deep sleep ,the uart tx fifo should be empty. 0: not need empty
    /*end CR:MS8384 */
    
    // @Xueliang.Wang add; CR9390; (2004-05-19)
    uint8   enable_arm_sleep;   // 1:Enable     0:Disable
    uint8   enable_arm_log;     // 1:Enable     0:Disable
    // End;    
    
    CHR_PARAM_T   chr_param;
    
    uint8   reserved1[1];
    uint8   enable_modem_log;     // 1:Enable     0:Disabl
    uint32  alarm_time;         // Number of seconds of alarm escaped from 2000-1-1 00:00:00
    
    uint32  lcd_brightless;     // the lcd backlight brightless. ( from 0 to 4, 5 level )
    
    RTC_CALIBRATION_INFO_T rtc_cal_info;
    uint32  core_voltage;       // the core voltage
    uint8   com_debug;          // phy port No for COM_DEBUG ,0xff: idle
    uint8   com_data;           // phy port No for COM_DATA  ,0xff: idle
    uint8   enable_seine_log;       // 1:Enable     0:Disable
    uint8   dsp_volume;         // max 255, 
    uint32  arm_log_uart_id;    // uart id
    uint32  dsp_log_uart_id;    // uart id
    AUD_ARMREF_PARAM_T audio_arm_param;

    uint8   L1IT_Mode;
    uint8   usb_speed_mode;     // 0:Full Speed;    1:High Speed;
    uint8   com_data2;          // phy port No for COM_DATA2  ,0xff: idle  
    uint8   dsp_log_set;	    // Low 4-bit is DSP LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG;  
                                // High 4-bit is DSP CARD LOG ENABLE: 0:Disable, 1:Enable
    uint8   dsp_log_highbyte;   // DSP CARD LOG file name index high byte
    uint8   dsp_log_lowbyte;    // DSP CARD LOG file name index low byte
    uint8   dsp_data_get_mode;  //bit0->0:  DSP data off ,->1,DSP date on,bit4->0:DSP log,bit4->1:DSP IQ 
    					      	//note: if dsp data on,set bit4 get IQ by logel in RAW date mode,amlog cann't get at same time,default get dsp log and armlog
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    uint8   bt_log_set;	    // Low 4-bit is BT LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG;  
                                // High 4-bit is BT CARD LOG ENABLE: 0:Disable, 1:Enable
    uint8   bt_log_highbyte;   // BT CARD LOG file name index high byte
    uint8   bt_log_lowbyte;    // BT CARD LOG file name index low byte
    uint8 gps_log;
    uint8 nv_ver_flag;
    uint8 pa_ver_flag;
    uint8   reserved[7];
#else
    uint8   reserved[13];
#endif
} REF_PARAM_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function initialize REF parameters which on NVItem.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void REFPARAM_InitRefParam(void);

/*****************************************************************************/
//  Description:    The function gets uart mode.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetUartMode(void);     // Return uart mode.

/*****************************************************************************/
//  Description:    The function sets uart mode.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetUartMode(
    uint32  mode                    // The mode value to be set
    );

/*****************************************************************************/
//  Description:    The function get usb virtual com or uart using mode.
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           for function of usb virtual com
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetUSBVcomMode(void);

/*****************************************************************************/
//  Description:    The function set usb virtual com or uart using mode.
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           for function of usb virtual com
/*****************************************************************************/
PUBLIC uint32 REFPARAM_SetUSBVcomMode(
    uint32  mode                    // The usb virtual com mode value to be set
    );

/*****************************************************************************/
//  Description:    The function gets com_debug physical port No.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDebugPortPhyNo(void);     // Return uart0 baud rate.

/*****************************************************************************/
//  Description:    The function sets com_debug physical port No
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetDebugPortPhyNo(
    uint8  phy_port              // The baud rate value to be set
    );

/*****************************************************************************/
//  Description:    The function gets com_data physical port No.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDataPortPhyNo(void);     // Return uart0 baud rate.

/*****************************************************************************/
//  Description:    The function sets com_data physical port No
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetDataPortPhyNo(
    uint8  phy_port              // The baud rate value to be set
    );
/*****************************************************************************/
//  Description:    The function gets com_data physical port No.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetData2PortPhyNo(void);     // Return uart0 baud rate.

/*****************************************************************************/
//  Description:    The function gets uart0 baud rate.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetDebugPortBaudRate(void);     // Return uart0 baud rate.

/*****************************************************************************/
//  Description:    The function sets uart0 baud rate.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetDebugPortBaudRate(
    uint32  baud_rate              // The baud rate value to be set
    );

/*****************************************************************************/
//  Description:    The function gets uart1 baud rate.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetDataPortBaudRate(void);    // Return uart1 baud rate.

/*****************************************************************************/
//  Description:    The function sets uart1 baud rate.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetDataPortBaudRate(
    uint32  baud_rate              // The baud rate value to be set
    );

/*****************************************************************************/
//  Description:    The function gets auto PowerOn flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Auto PowerOn
//                  0: Don't auto PowerOn
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetAutoPowerOnFlag(void);   // Return the flag.

/*****************************************************************************/
//  Description:    The function sets auto PowerOn flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Auto PowerOn
//                  0: Don't auto PowerOn
/*****************************************************************************/
PUBLIC void REFPARAM_SetAutoPowerOnFlag(
    BOOLEAN b_auto_power_on         // The flag value to be set
    );
    
/*****************************************************************************/
//  Description:    The function gets enable arm sleep flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetEnableArmSleepFlag(void);  // Return the flag.

/*****************************************************************************/
//  Description:    The function sets enable arm sleep flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC void REFPARAM_SetEnableArmSleepFlag(
    BOOLEAN is_enable_arm_sleep     // The flag value to be set
    );

/*****************************************************************************/
//  Description:    The function gets enable arm log flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetEnableArmLogFlag(void);  // Return the flag.

/*****************************************************************************/
//  Description:    The function sets enable arm log flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC void REFPARAM_SetEnableArmLogFlag(
    BOOLEAN is_enable_arm_log     // The flag value to be set
    );


/*****************************************************************************/
//  Description:    The function gets enable modem log flag.
//	Global resource dependence: 
//  Author:         Guangqiao.she
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetEnableModemLogFlag(void);  // Return the flag.

/*****************************************************************************/
//  Description:    The function sets enable arm log flag.
//	Global resource dependence: 
//  Author:         Guangqiao.she
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC void REFPARAM_SetEnableModemLogFlag(
    BOOLEAN is_enable_modem_log     // The flag value to be set
    );


/*****************************************************************************/
//  Description:    The function gets alarm time.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetAlarmTime(void);      // Return alarm time escaped from 2000-1-1 00:00:00.

/*****************************************************************************/
//  Description:    The function sets alarm time.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void REFPARAM_SetAlarmTime(
    uint32  alarm_time  // Alarm time escaped from 2000-1-1 00:00:00.
    );

/*****************************************************************************/
//  Description:    The function gets RTC calibration information.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetRtcCalibrationInfo(
	RTC_CALIBRATION_INFO_T *rtc_info_ptr
	);

/*****************************************************************************/
//  Description:    The function sets RTC calibration information.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void REFPARAM_SetRtcCalibrationInfo(
	RTC_CALIBRATION_INFO_T rtc_info
    );

/*****************************************************************************/
//  Description:    The function get uart mix mode setting.
//	Global resource dependence: 
//  Author:         
//	Note: if in mix mode return SCI_TRUE, or return SCI_FALSE.
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetMixMode(void);

/*****************************************************************************/
//  Description:    The function sets uart mix mode or not.
//	Global resource dependence: 
//  Author:         
//	Note: use SCI_TRUE to set UART0 as mix mode, SCI_FALSE to not mix mode
/*****************************************************************************/
PUBLIC void REFPARAM_SetMixMode(BOOLEAN on);
    
PUBLIC CHR_PARAM_T * REFPARAM_GetChargeParam( void );    

PUBLIC uint32 REFPARAM_GetLCDBrightless( void );
PUBLIC void   REFPARAM_SetLCDBrightless( uint32 brightless );

/*****************************************************************************/
//  Description:   set marker to verify whether mobile production is ok. 
//	Global resource dependence:
//  Author:         
//	Note:          adc_val : value to set   
//                 return value: SCI_TRUE: write success
/*****************************************************************************/  
PUBLIC BOOLEAN REFPARAM_SetCaliMarkerValue(uint32 adc_val);

/*****************************************************************************/
//  Description:   get marker to verify whether mobile production is ok. 
//	Global resource dependence:
//  Author:         
//	Note:          return value: saved value in nv,  (0xffffffff: return error)
/*****************************************************************************/  
PUBLIC uint32 REFPARAM_GetCaliMarkerValue(void);
/*****************************************************************************/
//  Description:   get audio ref param struncture pointer on arm side . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/  
PUBLIC AUD_ARMREF_PARAM_T* REFPARAM_GetAudArmRefRaram(void); 

/*****************************************************************************/
//  Description:    The function gets RTC calibration information.
//	Global resource dependence: 
//  Author:        
//	Note:           
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetUartRecvMode( void );

/*****************************************************************************/
//  Description:    The function sets RTC calibration information.
//	Global resource dependence: 
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC void REFPARAM_SeUartRecvmode( uint8  mode  );
 
/*****************************************************************************/
// Description: The function gets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDspDebugPortPhyNo(void);	//Returns the dsp log setting
												    //Low 4-bit is DSP LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG

/*****************************************************************************/
// Description: The function sets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetDspDebugPortPhyNo(
    								uint8 phy_port     //[IN]The  value to be set.  Low 4-bit is DSP LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG
   					 			);

/*****************************************************************************/
// Description: The function gets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDspCardLogFlag(void);	//Returns the dsp card log enable setting
												//High 4-bit is DSP CARD LOG ENABLE: 0:Disable, 1:Enable
/*****************************************************************************/
// Description: The function sets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetDspCardLogFlag( 
                            BOOLEAN is_enable_dsp_card_log     // The flag value to be set. High 4-bit is DSP CARD LOG ENABLE: 0:Disable, 1:Enable 
                            );

/*****************************************************************************/
// Description: The function gets dsp card log file number 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC uint16 REFPARAM_GetDspCardLogNum(void);	//Returns the dsp card log file number

/*****************************************************************************/
// Description: The function sets dsp card log file number 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetDspCardLogNum( 
                            uint16 dsp_card_log_num     // The value to be set
                            );

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description: The function gets bt log setting 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetBtDebugPortPhyNo(void);	//Returns the bt log setting
												    //Low 4-bit is BT LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG

/*****************************************************************************/
// Description: The function sets bt log setting 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetBtDebugPortPhyNo(
    								uint8 phy_port     //[IN]The  value to be set.  Low 4-bit is BT LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG
   					 			);

/*****************************************************************************/
// Description: The function gets bt log setting 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetBtCardLogFlag(void);	//Returns the bt card log enable setting
												//High 4-bit is BT CARD LOG ENABLE: 0:Disable, 1:Enable

/*****************************************************************************/
// Description: The function sets bt log setting 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetBtCardLogFlag( 
                            BOOLEAN is_enable_bt_card_log     // The flag value to be set. High 4-bit is BT CARD LOG ENABLE: 0:Disable, 1:Enable  
                            );

/*****************************************************************************/
// Description: The function gets bt card log file number 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC uint16 REFPARAM_GetBtCardLogNum(void);	//Returns the dsp card log file number

/*****************************************************************************/
// Description: The function sets bt card log file number 
// Global resource dependence: s_ref_param
// Author: tiger.han       
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetBtCardLogNum( 
                            uint16 bt_card_log_num     // The value to be set
                            );
#endif							
/*****************************************************************************/
// Description: The function sets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetUSBSpeedMode(uint8 SpeedMode );
/*****************************************************************************/
// Description: The function sets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetUSBSpeedMode(void); 

/*****************************************************************************/
//  Description:   REFPARAM_GetRefConfigValue 
//	Global resource dependence: adc_ptr->reserved[6]
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetRefConfigValue(void);
/*****************************************************************************/
//  Description:   REFPARAM_SetRefConfigValue
//	Global resource dependence: adc_ptr->reserved[6]
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC void REFPARAM_SetRefConfigValue(uint32 RefConfigValue);
/*****************************************************************************/
//  Description:   get usb auto assert release mode flag . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetUsbAutoAssertRelModeFlag(void);
/*****************************************************************************/
//  Description:   REFPARAM_GetUsbAutoModeFlag
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetUsbAutoModeFlag(void);
/*****************************************************************************/
//  Description:   REFPARAM_SetUsbAutoModeFlag
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC void REFPARAM_SetUsbAutoModeFlag( uint8 usb_auto_up_flag );   

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // _REF_PARAM_H
