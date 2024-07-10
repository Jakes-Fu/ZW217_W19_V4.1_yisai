/******************************************************************************
 ** File Name:      ref_param.c                                               *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/03/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain some Ref parameter .                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/03/2003     Xueliang.Wang    Create.                                   *
 ** 11/03/2010     fei.zhang		    Add DSP USB Log function       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_base_trc.h"
#include "ref_param.h"
#include "os_api.h"
#include "nv_type.h"
#include "nvitem.h"
#include "sio.h"
#include "arm_reg.h"
#include "driver_export.h"
#include "layer1_engineering.h"
#include "chip.h"
#include "power_manager.h"
#include "os_param.h"
#include "prod_param.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
LOCAL REF_PARAM_T    s_ref_param;

/* the fifo count register for DEBUG port */
uint32	uart_fifo_cnt_ctl;	/*lint -esym(765,uart_fifo_cnt_ctl) -esym(552, uart_fifo_cnt_ctl)*/

uint8	sleep_need_uart_empty_fifo = 1;   /*lint -esym(765,sleep_need_uart_empty_fifo) -esym(552, sleep_need_uart_empty_fifo)*/
uint8   gprs_debug = 0;				/*lint -esym(765,gprs_debug) -esym(552, gprs_debug)*/

/*add for calibration setting
  in the case that we want use uart cali when com debug value is 0x4(usb mode)
*/
LOCAL uint8 s_com_debug_value = 0xff;  
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/


/* @jim.zhang CR:MS1763 2004-07-15 */

extern uint32 RM_GetUart0Mode(void);
extern void DSLP_SetEnableSleepFlag (void);
//extern BOOLEAN REF_WriteMarkerValue(uint32 value);
extern void CHIP_ChangeCoreVoltage(CORE_VOLTAGE_E core_voltage);
//extern uint32 REF_GetMarkerValue(void);
/* end CR:MS1763 */

/*****************************************************************************/
//  Description:    Set the fifo count register address of debug port.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/

LOCAL void REFPARAM_SetDebugUartFifoCnt(
	uint32 fifo_cnt_add					//the fifo count register address of debug uart
	)
{
	uart_fifo_cnt_ctl = fifo_cnt_add;
}

/*****************************************************************************/
//  Description:    Get init core voltage value from NV  .
//	Global resource dependence: 
//  Author:         Daniel.ding
//	Note:           
/*****************************************************************************/
LOCAL uint32 REFPARAM_GetCoreVoltage (void)
{
    return s_ref_param.core_voltage;
}
/*****************************************************************************/
//  Description:    The function initialize REF parameters which on NVItem.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void REFPARAM_InitRefParam(void)
{
    uint32  status;
    
    SCI_ASSERT(sizeof(s_ref_param) == 300);/*assert verified*/
    
    SCI_MEMSET( (uint8 *)(&s_ref_param), 0, sizeof(s_ref_param) );//lint !e746
    
    // set a invalid value. then the low-layer function will set a default value.
    s_ref_param.lcd_brightless = 0xFFFFFFFF;

    s_ref_param.uart_mode               = 0;            // COM_USER
    s_ref_param.uart_0_baud_rate        = 0x00000071;   // 115200
    s_ref_param.uart_1_baud_rate        = 0x00000071;   //  57600
    s_ref_param.auto_power_on           = 1;            // Don't auto PowerOn
    
    // @Xueliang.Wang add; CR9390; (2004-05-19)
    s_ref_param.enable_arm_sleep        = 0;        // 1:Enable     0:Disable
    s_ref_param.enable_arm_log          = 1;        // 1:Enable     0:Disable
    
    s_ref_param.alarm_time              = 0;

    s_ref_param.dsp_log_set = 1;  // Low 4-bit is DSP LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG;  
                                  // High 4-bit is DSP CARD LOG ENABLE: 0:Disable, 1:Enable

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    s_ref_param.bt_log_set = 1;  // Low 4-bit is BT LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG;  
                                  // High 4-bit is BT CARD LOG ENABLE: 0:Disable, 1:Enable	
#endif
    // End;    
    // Set reserved to 0.
    //SCI_MEMSET((uint8 *)(s_ref_param.reserved), 0, 282);    
    
    // Read REF parameter initial values from NVItem.
    status = EFS_NvitemRead(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param));    
#ifdef ZDT_PCBA_ZW201_SUPPORT
    if(s_ref_param.enable_arm_log != 0
        || s_ref_param.dsp_log_set != 0
        || s_ref_param.gps_log != 0
        || s_ref_param.com_debug != 0xFF
        || s_ref_param.com_data != 0xFF
        || s_ref_param.com_data2 != 0xFF
        || s_ref_param.uart0_as_data != 0 
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        || s_ref_param.bt_log_set != 0 
#endif
    )
    {
        s_ref_param.enable_arm_log          = 0;        // 1:Enable     0:Disable
        s_ref_param.dsp_log_set = 0;
        s_ref_param.gps_log = 0;
        
        s_ref_param.com_debug = 0xFF;
        s_ref_param.com_data = 0xFF;
        s_ref_param.com_data2 = 0xFF;
        
        s_ref_param.uart0_as_data = 0;
        
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        s_ref_param.bt_log_set = 0;  // Low 4-bit is BT LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG;  
                                      // High 4-bit is BT CARD LOG ENABLE: 0:Disable, 1:Enable	
#endif
        EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
    }
    else
#endif
    // If the initial values don't exist.
    if (NVERR_NOT_EXIST == status)
    {
#ifdef WIN32
        EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
#endif  // WIN32        
    }
    //set core voltage
    CHIP_ChangeCoreVoltage(REFPARAM_GetCoreVoltage());
	
   	/*@jim.zhang CR:MS8384 2004-04-02 */
#ifndef WIN32
#if defined (PLATFORM_SC8800G) || defined(PLATFORM_UWS6121E) 
    DSLP_SetEnableSleepFlag();
#endif
#endif


#ifndef WIN32
#if !defined (PLATFORM_UWS6121E)  
	if (COM_DATA ==RM_GetUart0Mode())
   	{
   		REFPARAM_SetDebugUartFifoCnt(getRMUartBaseAddr() + 0x0C);
   	}
   	else
   	{
   		REFPARAM_SetDebugUartFifoCnt(GetUART0BaseAddr() + 0x0C);
   	}
#else
	if (COM_DATA ==RM_GetUart0Mode())
   	{
   		REFPARAM_SetDebugUartFifoCnt(getRMUartBaseAddr() + 0x24);
   	}
   	else
   	{
   		REFPARAM_SetDebugUartFifoCnt(GetUART0BaseAddr() + 0x24);
   	}
#endif
#endif
   	
   	gprs_debug = s_ref_param.gprs_debug;	
   	sleep_need_uart_empty_fifo = s_ref_param.sleep_need_emtpy_uart_fifo; 
   	/* end CR:MS8384 */
}

/*****************************************************************************/
//  Description:    The function gets uart mode.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetUartMode(void)     // Return uart mode.
{
    if ( s_ref_param.uart0_as_data == 1 )
	{
	   	return COM_DATA;
	}
    else
    {
    	return COM_DEBUG;
    }
}

/*****************************************************************************/
//  Description:    The function sets uart mode.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetUartMode(
    uint32  mode                    // The mode value to be set
    )
{
    /*@CR:MS10775 2004-07-14 */
    if ( COM_USER == mode )
    {
    	s_ref_param.uart0_as_data = 1;
    }
    else if ( COM_DEBUG == mode )
    {
    	s_ref_param.uart0_as_data = 0;
    }
    else
    {}
    
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
} 

/*****************************************************************************/
//  Description:    The function get usb virtual com or uart using mode.
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           for function of usb virtual com
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetUSBVcomMode(void)
{
    if ( s_ref_param.uart_mode == 1 )
	{
	   	return TRUE;
	}
    else
    {
    	return FALSE;
    }
}

/*****************************************************************************/
//  Description:    The function set usb virtual com or uart using mode.
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           for function of usb virtual com
/*****************************************************************************/
PUBLIC uint32 REFPARAM_SetUSBVcomMode(
    uint32  mode                    // The usb virtual com mode value to be set
    )
{
    if ( 1 == mode )      // usb virtual com mode
    {
    	s_ref_param.uart_mode = 1;
    }
    else if ( 0 == mode ) // uart mode
    {
    	s_ref_param.uart_mode = 0;
    }
    else
    {
    }

    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
    
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    The function gets com_debug physical port No.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDebugPortPhyNo(void)     // Return uart0 baud rate.
{
    uint8 ret;

    if(0xff == s_com_debug_value)
    {
        ret = s_ref_param.com_debug;
    }
    else
    {
        ret = s_com_debug_value;
    }
    return ret;
}

/*****************************************************************************/
//  Description:    The function sets com_debug physical port No
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetDebugPortPhyNo(
    uint8  phy_port              // The baud rate value to be set
    )
{
/*the reason is that
  we want to support uart calibration and usb log default setting
  for usb service, when will do uninit
  it will reinit sio, and set the sio to the phy port value from nv.
  1. we don't want to change the nv setting
  2. the nv will be updated by other function, such as REFPARAM_SetAlarmTime in sys_time.c
  so we have to use another to keep this info
  
  */
    s_com_debug_value = phy_port;
#if 0 //delete by wwh	
    s_ref_param.com_debug = phy_port; 
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
#endif	
}  

/*****************************************************************************/
//  Description:    The function gets com_data physical port No.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDataPortPhyNo(void)     // Return uart0 baud rate.
{
    return (s_ref_param.com_data);
}

/*****************************************************************************/
//  Description:    The function sets com_data physical port No
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetDataPortPhyNo(
    uint8  phy_port              // The baud rate value to be set
    )
{
    s_ref_param.com_data = phy_port;

}  
/*****************************************************************************/
//  Description:    The function gets com_data physical port No.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetData2PortPhyNo(void)     
{
    return (s_ref_param.com_data2);
}

/*****************************************************************************/
//  Description:    The function gets uart0 baud rate.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetDebugPortBaudRate(void)     // Return uart0 baud rate.
{
    return (s_ref_param.uart_0_baud_rate);
}

/*****************************************************************************/
//  Description:    The function sets uart0 baud rate.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetDebugPortBaudRate(
    uint32  baud_rate              // The baud rate value to be set
    )
{
    s_ref_param.uart_0_baud_rate = baud_rate;

    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}  

/*****************************************************************************/
//  Description:    The function gets uart1 baud rate.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetDataPortBaudRate(void)     // Return uart1 baud rate.
{
    return (s_ref_param.uart_1_baud_rate);
}

/*****************************************************************************/
//  Description:    The function sets uart1 baud rate.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void REFPARAM_SetDataPortBaudRate(
    uint32  baud_rate              // The baud rate value to be set
    )
{
    s_ref_param.uart_1_baud_rate = baud_rate;

    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}

/*****************************************************************************/
//  Description:    The function gets auto PowerOn flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Auto PowerOn
//                  0: Don't auto PowerOn
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetAutoPowerOnFlag(void)   // Return the flag.
{

#ifndef FPGA_VERIFICATION
    return (s_ref_param.auto_power_on);
#else
    return SCI_TRUE; /* FPGA Verification, don't modify NV */
#endif

}

/*****************************************************************************/
//  Description:    The function sets auto PowerOn flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Auto PowerOn
//                  0: Don't auto PowerOn
/*****************************************************************************/
PUBLIC void REFPARAM_SetAutoPowerOnFlag(
    BOOLEAN b_auto_power_on         // The flag value to be set
    )
{
    s_ref_param.auto_power_on = b_auto_power_on;
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}

/*****************************************************************************/
//  Description:    The function gets enable arm sleep flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetEnableArmSleepFlag(void)   // Return the flag.
{
    return (s_ref_param.enable_arm_sleep);
}

/*****************************************************************************/
//  Description:    The function sets enable arm sleep flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC void REFPARAM_SetEnableArmSleepFlag(
    uint8 is_enable_arm_sleep     // The flag value to be set
    )
{
    s_ref_param.enable_arm_sleep = is_enable_arm_sleep;
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}

/*****************************************************************************/
//  Description:    The function gets enable arm log flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetEnableArmLogFlag(void)   // Return the flag.
{
    return (s_ref_param.enable_arm_log);
}

/*****************************************************************************/
//  Description:    The function sets enable arm log flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC void REFPARAM_SetEnableArmLogFlag(
    BOOLEAN is_enable_arm_log     // The flag value to be set
    )
{
    s_ref_param.enable_arm_log = is_enable_arm_log;
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}



/*****************************************************************************/
//  Description:    The function gets enable modem log flag.
//	Global resource dependence: 
//  Author:         Guangqiao.she
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetEnableModemLogFlag(void)   // Return the flag.
{
    return (s_ref_param.enable_modem_log);
}

/*****************************************************************************/
//  Description:    The function sets enable modem log flag.
//	Global resource dependence: 
//  Author:         Guangqiao.she
//	Note:           1: Enable
//                  0: Disable
/*****************************************************************************/
PUBLIC void REFPARAM_SetEnableModemLogFlag(
    BOOLEAN is_enable_modem_log     // The flag value to be set
    )
{
    s_ref_param.enable_modem_log = is_enable_modem_log;
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}


/*****************************************************************************/
// Description: The function gets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDspDebugPortPhyNo(void)	//Returns the dsp log setting
												    //Low 4-bit is DSP LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG
{
    uint8 dsp_log_mode = s_ref_param.dsp_log_set & 0xF;
    return dsp_log_mode;
}


/*****************************************************************************/
// Description: The function sets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetDspDebugPortPhyNo(
    								uint8 phy_port     //[IN]The  value to be set.  Low 4-bit is DSP LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG
   					 			)
{
    uint8 dsp_log_set = s_ref_param.dsp_log_set;
    dsp_log_set &= ~0xF;
    dsp_log_set |= phy_port;
	s_ref_param.dsp_log_set = dsp_log_set;
	EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}

/*****************************************************************************/
// Description: The function gets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDspCardLogFlag(void)	//Returns the dsp card log enable setting
												//High 4-bit is DSP CARD LOG ENABLE: 0:Disable, 1:Enable
{
    uint8 enable_dsp_card_log = (s_ref_param.dsp_log_set & 0xF0)>> 0x4;
    return enable_dsp_card_log;
}


/*****************************************************************************/
// Description: The function sets dsp log setting 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetDspCardLogFlag( 
                            BOOLEAN is_enable_dsp_card_log     // The flag value to be set. High 4-bit is DSP CARD LOG ENABLE: 0:Disable, 1:Enable  
                            )
{
    uint8 dsp_log_set = s_ref_param.dsp_log_set;
    dsp_log_set &= ~0xF0;
    dsp_log_set |= is_enable_dsp_card_log<<0x4;
	s_ref_param.dsp_log_set = dsp_log_set;
	EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);   
}

/*****************************************************************************/
// Description: The function gets dsp card log file number 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC uint16 REFPARAM_GetDspCardLogNum(void)	//Returns the dsp card log file number
{
    uint16 dsplognum = (s_ref_param.dsp_log_lowbyte) | (s_ref_param.dsp_log_highbyte<<0x8);
    return dsplognum;
}


/*****************************************************************************/
// Description: The function sets dsp card log file number 
// Global resource dependence: s_ref_param
// Author: fei.zhang        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetDspCardLogNum( 
                            uint16 dsp_card_log_num     // The value to be set
                            )
{
	s_ref_param.dsp_log_lowbyte = dsp_card_log_num & 0xFF;
    s_ref_param.dsp_log_highbyte = (dsp_card_log_num & 0xFF00)>>0x8;
	EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);   
}

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
/*****************************************************************************/
// Description: The function gets bt log setting 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetBtDebugPortPhyNo(void)	//Returns the bt log setting
												    //Low 4-bit is BT LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG
{
    uint8 bt_log_mode = s_ref_param.bt_log_set & 0xF;
    return bt_log_mode;
}


/*****************************************************************************/
// Description: The function sets bt log setting 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetBtDebugPortPhyNo(
    								uint8 phy_port     //[IN]The  value to be set.  Low 4-bit is BT LOG MODE: 0.Disable 1.COM1 2.ARM COM_DEBUG
   					 			)
{
    uint8 bt_log_set = s_ref_param.bt_log_set;
    bt_log_set &= ~0xF;
    bt_log_set |= phy_port;
	s_ref_param.bt_log_set = bt_log_set;
	EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}

/*****************************************************************************/
// Description: The function gets bt log setting 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetBtCardLogFlag(void)	//Returns the bt card log enable setting
												//High 4-bit is BT CARD LOG ENABLE: 0:Disable, 1:Enable
{
    uint8 enable_bt_card_log = (s_ref_param.bt_log_set & 0xF0)>> 0x4;
    return enable_bt_card_log;
}


/*****************************************************************************/
// Description: The function sets bt log setting 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetBtCardLogFlag( 
                            BOOLEAN is_enable_bt_card_log     // The flag value to be set. High 4-bit is BT CARD LOG ENABLE: 0:Disable, 1:Enable  
                            )
{
    uint8 bt_log_set = s_ref_param.bt_log_set;
    bt_log_set &= ~0xF0;
    bt_log_set |= is_enable_bt_card_log<<0x4;
	s_ref_param.bt_log_set = bt_log_set;
	EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);   
}

/*****************************************************************************/
// Description: The function gets bt card log file number 
// Global resource dependence: s_ref_param
// Author: tiger.han        
// Note:    		       
/*****************************************************************************/
PUBLIC uint16 REFPARAM_GetBtCardLogNum(void)	//Returns the dsp card log file number
{
    uint16 btlognum = (s_ref_param.bt_log_lowbyte) | (s_ref_param.bt_log_highbyte<<0x8);
    return btlognum;
}


/*****************************************************************************/
// Description: The function sets bt card log file number 
// Global resource dependence: s_ref_param
// Author: tiger.han       
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetBtCardLogNum( 
                            uint16 bt_card_log_num     // The value to be set
                            )
{
	s_ref_param.bt_log_lowbyte = bt_card_log_num & 0xFF;
    s_ref_param.bt_log_highbyte = (bt_card_log_num & 0xFF00)>>0x8;
	EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);   
}

#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
/*****************************************************************************/
// Description: The function get gps log setting
// Global resource dependence: s_ref_param
// Author: yinhui.huang      
// Note:    		       
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetGpsLogValue(void)   // Return the flag.
{
    return (s_ref_param.gps_log);
}

/*****************************************************************************/
// Description: The function set gps log setting
// Global resource dependence: s_ref_param
// Author: yinhui.huang      
// Note:    		       
/*****************************************************************************/
PUBLIC void REFPARAM_SetGpsLogValue(
    uint8 set_gps_log_value     // The flag value to be set
    )
{
    s_ref_param.gps_log = set_gps_log_value;
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}
#endif

#endif
PUBLIC CHR_PARAM_T * REFPARAM_GetChargeParam( void )
{
    return &(s_ref_param.chr_param);
}

/*****************************************************************************/
//  Description:    The function gets alarm time.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetAlarmTime(void)   // Return alarm time escaped from 2000-1-1 00:00:00.
{
    return s_ref_param.alarm_time;
}

/*****************************************************************************/
//  Description:    The function sets alarm time.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void REFPARAM_SetAlarmTime(
    uint32  alarm_time  // Alarm time escaped from 2000-1-1 00:00:00.
    )
{
    s_ref_param.alarm_time = alarm_time;
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}
    
/*****************************************************************************/
//  Description:    The function get lcd backlight brightless
//	Global resource dependence: 
//  Author:         Lin.liu
//	Note:           
/*****************************************************************************/    
PUBLIC uint32 REFPARAM_GetLCDBrightless( void )
{
    return s_ref_param.lcd_brightless;
}

/*****************************************************************************/
//  Description:    The function set lcd backlight brightless
//	Global resource dependence: 
//  Author:         Lin.liu
//	Note:           
/*****************************************************************************/    
PUBLIC void   REFPARAM_SetLCDBrightless( uint32 brightless )
{
    s_ref_param.lcd_brightless = brightless;
    
    // save to nv
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}

/*****************************************************************************/
//  Description:    The function gets RTC calibration information.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetRtcCalibrationInfo(
	RTC_CALIBRATION_INFO_T *rtc_info_ptr
	)
{
	*rtc_info_ptr = s_ref_param.rtc_cal_info;
	
	return SCI_SUCCESS;
}
	

/*****************************************************************************/
//  Description:    The function sets RTC calibration information.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void REFPARAM_SetRtcCalibrationInfo(
	RTC_CALIBRATION_INFO_T rtc_info
    )
{
	s_ref_param.rtc_cal_info = rtc_info;
    
    // save to nv
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);

}

 /*****************************************************************************/
//  Description:    The function gets dsp volume.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint8 REFPARAM_GetDspVolume(void)    // Return dsp volume.
{
      return s_ref_param.dsp_volume;
}
/*****************************************************************************/
//  Description:    The function sets dsp volume.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL void REFPARAM_SetDspVolume(
    uint8  dsp_volume              // The dsp volume to be set
    )
{
    s_ref_param.dsp_volume = dsp_volume;
    
    // save to nv
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}
/*****************************************************************************/
//  Description:    The function gets seine log flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetSeineLogFlag(void)  /*lint -esym(765,REFPARAM_GetSeineLogFlag)*/  // Return seine log flag.
{
    return s_ref_param.enable_seine_log;
}
/*****************************************************************************/
//  Description:    The function sets seine log flag.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL void REFPARAM_SetSeineLogFlag(
    BOOLEAN  is_enable_seine_log              // The log flag to be set
    )    
{
    s_ref_param.enable_seine_log = is_enable_seine_log;
    
    // save to nv
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}

/*****************************************************************************/
//  Description:    The function gets arm log uart id.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL  uint32 REFPARAM_GetArmLogUartId(void)    // Return arm log uart id.
{
    return s_ref_param.arm_log_uart_id;
}
/*****************************************************************************/
//  Description:    The function gets dsp log uart id.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
LOCAL uint32 REFPARAM_GetDspLogUartId(void)    // Return dsp log uart id.
{
       return s_ref_param.dsp_log_uart_id;
}

/*****************************************************************************/
//  Description:    The function gets RTC calibration information.
//	Global resource dependence: 
//  Author:        
//	Note:           
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetUartRecvMode( void )
{
	return s_ref_param.L1IT_Mode;
}

/*****************************************************************************/
//  Description:    The function sets RTC calibration information.
//	Global resource dependence: 
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC void REFPARAM_SeUartRecvmode(
	        uint8  mode      
    )
{
	s_ref_param.L1IT_Mode = mode;
    
    // save to nv
    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);

}

/*****************************************************************************/
//  Description:    The function sets uart mix mode or not.
//	Global resource dependence: 
//  Author:         
//	Note: use SCI_TRUE to set UART0 as mix mode, SCI_FALSE to not mix mode
/*****************************************************************************/
PUBLIC void REFPARAM_SetMixMode(BOOLEAN on)
{
    if(SCI_TRUE == on)
    {
        s_ref_param.gprs_debug = 1;
    }
    else
    {
        s_ref_param.gprs_debug = 0;
    }

    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}

/*****************************************************************************/
//  Description:    The function get uart mix mode setting.
//	Global resource dependence: 
//  Author:         
//	Note: if in mix mode return SCI_TRUE, or return SCI_FALSE.
/*****************************************************************************/
PUBLIC BOOLEAN REFPARAM_GetMixMode(void)
{
    if(1 == s_ref_param.gprs_debug)
    {
        return SCI_TRUE;    
    }
    else
    {
        return SCI_FALSE;    
    }
}

/*****************************************************************************/
//  Description:   set marker to verify whether mobile production is ok. 
//	Global resource dependence:
//  Author:         
//	Note:           adc_val : value to set   
//                  return value: SCI_TRUE: write success
/*****************************************************************************/  
PUBLIC BOOLEAN REFPARAM_SetCaliMarkerValue(uint32 adc_val)
{
#ifndef WIN32
    return REF_WriteMarkerValue(adc_val);
#else
    return SCI_FALSE;  
#endif
}

/*****************************************************************************/
//  Description:   get marker to verify whether mobile production is ok. 
//	Global resource dependence:
//  Author:         
//	Note:         return value: saved value in nv, (0xffffffff: return error)
/*****************************************************************************/  
PUBLIC uint32 REFPARAM_GetCaliMarkerValue(void)
{
#ifndef WIN32
    return REF_GetMarkerValue();
#else
    return 0xffffffff;
#endif
}


/*****************************************************************************/
//  Description:   get audio ref param struncture pointer on arm side . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/  
PUBLIC AUD_ARMREF_PARAM_T* REFPARAM_GetAudArmRefRaram(void)
{
    return &s_ref_param.audio_arm_param;
}
 /*****************************************************************************/
//  Description:   get marker to verify whether mobile production is ok. 
//	Global resource dependence:
//  Author:         
//	Note:         return value: saved value in nv, (0xffffffff: return error)
/*****************************************************************************/  
PUBLIC uint8 REFPARAM_GetUSBSpeedMode(void)
{
     return s_ref_param.usb_speed_mode;
}


/*****************************************************************************/
//  Description:   get audio ref param struncture pointer on arm side . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/  
PUBLIC void REFPARAM_SetUSBSpeedMode(uint8 SpeedMode )
{
    	s_ref_param.usb_speed_mode = SpeedMode;
	EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}   
/*****************************************************************************/
//  Description:   REFPARAM_GetRefConfigValue 
//	Global resource dependence: adc_ptr->reserved[6]
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC uint32 REFPARAM_GetRefConfigValue(void)
{
#ifndef WIN32
    ADC_T adc_t = {0};  
    
    REF_GetCalibrationPara_Adc((uint32*)&adc_t,sizeof(ADC_T)); 
    //SCI_TRACE_LOW:"[REF] REFPARAM_GetRefConfigValue() = 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,REF_PARAM_752_112_2_18_1_5_34_56,(uint8*)"d",adc_t.reserved[6]);
   
    return (adc_t.reserved[6]);
#else
    return 0xffffffff;
#endif
}
/*****************************************************************************/
//  Description:   REFPARAM_SetRefConfigValue
//	Global resource dependence: adc_ptr->reserved[6]
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC void REFPARAM_SetRefConfigValue(uint32 RefConfigValue)
{
#ifndef WIN32
    ADC_T adc_t = {0};  

    REF_GetCalibrationPara_Adc((uint32*)&adc_t,sizeof(ADC_T)); 
    adc_t.reserved[6] = RefConfigValue;
    REF_SetCalibrationPara_Adc((uint32*)&adc_t, sizeof(ADC_T)); 
#endif    

}
/*****************************************************************************/
//  Description:   get usb auto assert release mode flag . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetUsbAutoAssertRelModeFlag(void)
{
  uint8 mode;

  OSPARAM_GetAssertMode (&mode);
  //SCI_TRACE_LOW:"[REF] REFPARAM_GetUsbAutoAssertRelModeFlag() OSPARAM_GetAssertMode = 0x%X"
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,REF_PARAM_784_112_2_18_1_5_34_57,(uint8*)"d",mode);
  
  mode =(REFPARAM_GetUsbAutoModeFlag()) && (SCI_ASSERT_RELEASE_MODE == mode);

  return mode;
}
/*****************************************************************************/
//  Description:   REFPARAM_GetUsbAutoModeFlag. 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetUsbAutoModeFlag(void)
{

    uint8 UsbAutoModeFlag = 0;
    if(REFPARAM_GetRefConfigValue() & REF_CONFIG_USB_AUTO_MODE_BIT)
    {
        UsbAutoModeFlag = 1;
    }
    else
    {
        UsbAutoModeFlag = 0;     
    }
    //SCI_TRACE_LOW:"[REF] REFPARAM_GetUsbAutoModeFlag() UsbAutoModeFlag = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,REF_PARAM_807_112_2_18_1_5_34_58,(uint8*)"d",UsbAutoModeFlag);
    
    return (UsbAutoModeFlag);
}

/*****************************************************************************/
//  Description:   REFPARAM_SetUsbAutoModeFlag . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC void REFPARAM_SetUsbAutoModeFlag( 
                            uint8 usb_auto_up_mode     
                            )
{
    uint32 RefConfigValue = REFPARAM_GetRefConfigValue();
    uint8 UsbAutoModeFlag = 0;
    
    // if flag is what we want, just return, avoid writing NV again and again.
    if(RefConfigValue & REF_CONFIG_USB_AUTO_MODE_BIT)
    {
        UsbAutoModeFlag = 1;
    }
    else
    {
        UsbAutoModeFlag = 0;    
    }
    
    if( usb_auto_up_mode == UsbAutoModeFlag )
    {
        //SCI_TRACE_LOW:"[REF] REFPARAM_SetUsbAutoModeFlag() the same mode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,REF_PARAM_836_112_2_18_1_5_34_59,(uint8*)"d",UsbAutoModeFlag);
        return ; 
    }
    
    if(usb_auto_up_mode)
        RefConfigValue |=  REF_CONFIG_USB_AUTO_MODE_BIT;
    else
        RefConfigValue &= ~REF_CONFIG_USB_AUTO_MODE_BIT;

    //SCI_TRACE_LOW:"[REF] REFPARAM_SetUsbAutoModeFlag() RefConfigValue = 0x%X, mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,REF_PARAM_846_112_2_18_1_5_34_60,(uint8*)"dd",RefConfigValue, usb_auto_up_mode);
    // write fix nv item 
    REFPARAM_SetRefConfigValue(RefConfigValue);
}

/*****************************************************************************/
//  Description:   REFPARAM_GetDspDataOnFlag . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDspLogOnFlag(void)
{
	return  s_ref_param.dsp_data_get_mode&0x01;
}


/*****************************************************************************/
//  Description:   REFPARAM_SetDspDataOnFlag . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC void REFPARAM_SetDspLogOnFlag( 
                            BOOLEAN SetDsplogOn     
                            )
{
	if(SetDsplogOn)
	{
		s_ref_param.dsp_data_get_mode |= 0x01 ;
	}
	else
	{
		s_ref_param.dsp_data_get_mode &= (~0x01);
	}
	
	 EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}



/*****************************************************************************/
//  Description:   REFPARAM_GetDspIQOnFlag . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC uint8 REFPARAM_GetDspIQDataOnFlag(void)
{
	return  (s_ref_param.dsp_data_get_mode&0x10)>>4;
}



/*****************************************************************************/
//  Description:   REFPARAM_SetDspIQOnFlag . 
//	Global resource dependence:
//  Author:         
//	Note:        
/*****************************************************************************/
PUBLIC void REFPARAM_SetDspIQDataOnFlag( 
                            BOOLEAN SetDspIQDataOn     
                            )
{
	if(SetDspIQDataOn)
	{
		s_ref_param.dsp_data_get_mode |= 0x10 ;
	}
	else
	{
		s_ref_param.dsp_data_get_mode &= (~0x10);
	}
	
	 EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&s_ref_param), 1);
}

/*****************************************************************************/
//  Description:   REFPARAM_GetUsbServicesFlag
//	Global resource dependence:
//  Author:  tiger.han       
//	Note:        return value SCI_TRUE: SPRD USB
//                                      SCI_FALSE: MSFT WINUSB
/*****************************************************************************/  
PUBLIC uint8 REFPARAM_GetUsbServicesFlag(void)
{
#ifdef WINUSB_SUPPORT
    return SCI_FALSE;
#else
    return SCI_TRUE;
#endif
    //return (s_ref_param.reserved[1] ? SCI_TRUE:SCI_FALSE);
    // TODO -- MMI_COMPILE_E
}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
