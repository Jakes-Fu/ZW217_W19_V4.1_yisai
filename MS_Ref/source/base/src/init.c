/******************************************************************************
 ** File Name:      init.c                                                    *
 ** Author:         Richard Yang                                              *
 ** DATE:           04/02/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Richard.Yang     Create.                                   *
 ** 06/03/2002     Xueliang.Wang    Modify according to new standard.         *
 ** 09/04/2003     Zhemin.Lin       Modify SCI_InitRef                        *
 ** 09/16/2003     Xueliang.Wang    Modify CR4013                             *
 ** 09/24/2003     Xueliang.Wang    Modify CR4916                             *
 ** 12/31/2003     Zhemin.Lin       Modify CR6659                             *
 ** 12/07/2004     Benjamin.Wang    Modify CR17118                            *
 ** 08/28/2010     Haifeng.yang     Modify                                    *
 ** 11/03/2010     fei.zhang		   Add DSP USB Log function       *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_base_trc.h"
#include "ref_tasks_def.h"
#include "init.h"
#include "version.h"
#include "power.h"
#include "efs.h"
#include "doidleonpage.h"
#include "tb_dal.h"
#include "ref_param.h"
#include "atc_sleep.h"
#include "os_param.h"
#include "sys_time_ext.h"
#include "boot_mode.h"
#include "bsd.h"
#include "xsr_partition.h"
#include "oscfg.h"
#include "layer1_engineering.h"
#include "mem_prod.h"
#include "power_manager.h"
#include "dal_power.h"
#include "iram_mgr.h"
#include "chip_init_drvapi.h"
#include "img_dec_interface.h"
#include "atc_port.h"
#ifndef WIN32
#include "deepsleep_drvapi.h"
#include "driver_export.h"
#include "run_mode.h"
#include "diag.h"
#include "task_monitor.h"
#include "sci_ftlmnt_Engin.h"
#include "sci_log.h"
#include "dal_dcamera.h"
#include "audio_config.h"
#include "threadx_assert.h"
#include "mux_driver.h"
#include "task_monitor.h"
#include "isp_service.h"
#include "prod_param.h"
#include "gpio_prod_api.h"
#include "chip_drv_init.h"
#include "char_dev.h"
#ifdef UMEM_SUPPORT
#include "flash.h"
#include "umem_bsd.h"
#endif
#endif /* #ifndef WIN32 */

#ifdef DSP_USB_LOG
#include "dsp_log.h"
#endif

#ifdef BT_USB_LOG
#include "bt_log.h"
#endif

#ifdef IMG_EDITER_SUPPORT
#include "img_proc.h"
#endif

#ifdef AUTOTEST_DEVICE_SUPPORT
#include "autotest_device.h"
#endif

#ifdef CHIP_VER_6531EFM
#include "arm9_shm_operation.h"
#endif

#ifdef FOTA_SUPPORT_REDSTONE
#include "rs_sdk_api.h"
#endif


#include "../../../../chip_drv/chip_module/pin/uws6121e/pinmap.h"

#ifdef SIMLOCK_PROTECT_DATA
#include "simlock.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
uint32 SCI_InitLogSaveTask(void);/*lint -esym(752, SCI_InitLogSaveTask)*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//lin.liu. CR12921.
//
// When it is non-zero, enable the nvitem cache feature, and can set the EFS priority
//     to lowest.
// If it is zero, disable the nvitem cache feature, and shouldn't set the EFS priority
// lower than MMI to avoid the EFS queue full( for we support asynchronous optr ).
//
//

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
//record the init status
LOCAL INIT_STATUS_E s_init_status = STATUS_START_INIT;

extern const char * SCI_GetRtosCompileDate(void);
extern const char * GetRefCompileDate(void);

#if defined(PRODUCT_DM)
    extern uint32 SCI_InitRtosBottomHalf( void );
#endif

#ifdef CMMB_SUPPORT
#ifndef WIN32
extern void DTLRFVT_ThreadInit(void);
extern void MTV_InitVLogFlag(void);
#endif
#endif

#ifdef DEMOD_HW_INNOFIDEI
#ifndef WIN32
extern int INNO_DoDemodLowPowerMode(void);
#endif
#endif

extern int  ipc_init(void);
extern int init_apcpverify(void);
extern void halo_init_test(void);
extern uint8 halo_ctrl_init(uint8* conf);

extern void NV_IPC_init(void);

extern void LOAD_modem(void);
extern void LOAD_modem_phase2(void);
extern void CHIP_MODEM_CP_Start(void);
extern void Modem_Init(void);
extern void UART_Clk_Enable(uint32 port);
extern void CHIP_DisablePowerKeyInt(void);
extern void ppp_read_init(void);


/*attention!!
  the api for td l1c is hard to be called, so define the MACOR directly
  this adress should be keep same with l1c's defination
  */
//#ifdef FPGA_VERIFICATION
//#define DMDSP_LOAD_ADDRESS        0x11C20040
//#else
#define DMDSP_LOAD_ADDRESS          0x20000
//#endif

#define PROD_INIT_AFTER_USER_IMAGE  0x01

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void ShowVersionInfo(void);

/*****************************************************************************/
// Description :    Create reference tasks.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
LOCAL int CreateRefTasks(uint32 mode);

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    get the init status
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC INIT_STATUS_E INIT_GetInitStatus(void)
{
	return s_init_status;
}

/*****************************************************************************/
// Description :    set the init status
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void INIT_SetInitStatus(INIT_STATUS_E status)
{
	s_init_status = status;
}

/******************************************************************************/
// Description:
// Dependence:
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 _INIT_CopyImage(void)
{
	BSD_HANDLE 		pHandle;
	BSD_DEVICE 		pDev;

//#ifndef FPGA_VERIFICATION /* for jtag debug */
    if(CHIP_DetectMemType())
    {
        char * bml_name;

        if (MEM_HasLoadImage())
        {
#if defined(VM_SUPPORT)
            bml_name = BML_KERNELOS2_NAME;
#else
            bml_name = BML_DEMANDONOS_NAME;
#endif
            // Load Demandon OS image !NOW!
            if ((SCI_NULL == MEM_GetUserImageAddress()) ||
                  (MEM_GetUserImageSize() == 0) ||
                  	(BSD_SUCCESS != BSD_Open (bml_name, BSD_GENERIC_READ, &pHandle)) ||
                  	(BSD_SUCCESS != BSD_GetDeviceByHandle (pHandle, &pDev)) ||
                  	(MEM_GetUserImageSize() > ((pDev.TotScts)*(pDev.SectorSize))) ||
                  	(BSD_SUCCESS != BSD_Read (pHandle,
          	                          0,
          	                          (MEM_GetUserImageSize()/pDev.SectorSize + 1),
          	                          (void *)MEM_GetUserImageAddress ())))
            {
                SCI_PASSERT (0, ("\r\nPlease download user image again"));/*assert verified*/
            }
        }

        //copy dsp code image
        if( BSD_SUCCESS == BSD_Open(BML_DSPCODE_PART_NAME, BSD_GENERIC_READ, &pHandle) )
        {
            if( BSD_SUCCESS == BSD_GetDeviceByHandle(pHandle, &pDev) )
            {
                if( BSD_SUCCESS != BSD_Read(pHandle, 0, pDev.TotScts, (void*)( DMDSP_LOAD_ADDRESS)) )
                {
                    //not recoverable
                    SCI_PASSERT (0, ("\r\nPlease download user image again"));/*assert verified*/
                }
            }
        }
	}
//#endif

    return SCI_SUCCESS;
}

/*****************************************************************************/
// Description :    Save HW restart condition.
//                  It is maybe by PowerButton, Charge, Alarm or none.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
LOCAL void SaveHWRestartCondition(void)
{
    uint32  condition;
    uint32  tmp;

    tmp = POWER_GetResetMode();

    if(tmp&MCU_MODE_USB)
    {
        /*mcft by usb */
        RM_SetDloaderMode(1);
        BOOT_SetUSBConnectStatus(0);     /*usbstatus no used, return value of power_getresetmode() only mode_used*/
    }

    // Check if it is PowerButton
    if (SCI_TRUE == CheckPowerButtonState ())
    {
        condition = RESTART_BY_POWER_BUTTON;
    }
    else
    {
        // Check if it is Alarm.
        if (CheckRTCState () & BIT_4)
        {
            condition = RESTART_BY_ALARM;
        }
        else
        {
            // Check if it is Charge.
            if( CHGMNG_IsChargeConnect() )
            {
                condition = RESTART_BY_CHARGE;
            }
            else

            {
                condition = RESTART_BY_NONE;
            }
        }
    }
#if 1 //bringup_temp tianyu.yang
#ifndef WIN32
    if((BIT_9 & ANA_REG_GET(ANA_POR_7S_CTRL)) != 0)
    {
        condition = RESTART_BY_SOFTWARE;
    }
#endif
#endif
    POWER_SetRestartCondition(condition);
}

/*****************************************************************************/
// Description :    Save sw restart condition.
//                  It is maybe by software, assert back, cft mode.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
LOCAL void SaveSWRestartCondition(void)
{
    uint32  condition;
    uint32  tmp;

    tmp = POWER_GetResetMode();

    tmp &= ~MCU_MODE_USB;

    /*get restartcondition first!*/
    condition = POWER_GetRestartCondition();

    if(ASSERT_BACK_MODE == tmp)
    {
    	condition = RESTART_BY_ASSERT;
    }
    else if(NORMAL_MODE != tmp)
    {
        condition = RESTART_BY_SOFTWARE;

		switch(tmp){
            /* In calibration mode.*/
			case TD_CALIBRATION_MODE:
				{
#ifndef WIN32
					BOOT_SetResetMode (CALIBRATION_MODE);
					RM_HandlePowerResetMode(TD_CALIBRATION_MODE);
#endif
				}
			case CALIBRATION_MODE:  	/*lint !e825*//*lint !e616*/
				{
					RM_SetCalibrationMode(SCI_TRUE);
				}
				break;
			case TD_CALIBRATION_POST_MODE:
				{
#ifndef WIN32
					BOOT_SetResetMode (CALIBRATION_POST_MODE);
					RM_HandlePowerResetMode(TD_CALIBRATION_POST_MODE);
#endif
				}
			case CALIBRATION_POST_MODE:    	/*lint !e825*//*lint !e616*/
				{
					RM_SetCalibrationPostMode(SCI_TRUE);
					SCI_Calibration_EnableDeepSleep(0);	//disable arm deep sleep.
				}
				break;
			case BOOT_RESET_MODE :
				{
				#ifndef WIN32
			        HAL_MCUBootReset();
				#endif
				}
				break;
			case PIN_TEST_MODE :
				{
					RM_SetPinTestFlag(SCI_TRUE);	/*in production-test mode.*/
				}
				break;
			case IQC_TEST_MODE:
				{
					RM_SetIQCTestFlag(SCI_TRUE);
					RM_SetPinTestFlag(SCI_TRUE);
				}
				break;
			case TD_CALIBRATION_POST_NO_LCM_MODE:
				{
#ifndef WIN32
					BOOT_SetResetMode (CALIBRATION_POST_NO_LCM_MODE);
					RM_HandlePowerResetMode(TD_CALIBRATION_POST_NO_LCM_MODE);
#endif
				}
			case CALIBRATION_POST_NO_LCM_MODE:   	/*lint !e825*//*lint !e616*/
				{
					RM_SetCalibrationPostNoLcmMode(SCI_TRUE);

					SCI_Calibration_EnableDeepSleep(0);	//disable arm deep sleep.
				}
				break;
			default:
			    break;
		}

    }

    POWER_SetRestartCondition(condition);
}

/*****************************************************************************/
// Description :    Initialize reference resource according to mode.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
#if !defined(PLATFORM_UWS6121E)
PUBLIC uint32 SCI_InitRef(uint32 mode)
{
    uint32 tmp;
#ifndef WIN32
    uint8  			debug_phy_port;
    uint8  			data_phy_port;
	SIO_CONTROL_S   m_dcb;
#endif
	uint32			iram_area_cnt = 0;
	IRAM_AREA * 	area_p = SCI_NULL;

	POWER_SetMsStatus(POWER_MS_STATUS_INIT);
#ifndef WIN32
	TM_SendInitRequest(); //kevin.wang@0719

    // Phase2 of chip driver intialization. It initializes driver modules
    // before refbase confirms powering on.
    CHIPDRV_InitOnPhase2 ();
#endif
#ifndef WIN32
    CharDev_Initialize();
#endif
	// Diag init;
	InitDiag();  //Added by Johnson.sun 2006/10/09

	// Init BSD interface
	BSD_Initialize ();

#ifdef NANDBOOT_SUPPORT
    if(CHIP_DetectMemType())
    {
      // Init XSR
#if !defined(WIN32)
	SCI_FTLMnt_Init ();
#endif
    }
#endif

	// Init EFS Module here, and EFS_Task not init it now.
	// So we can make sure that the EFS module has been init-ed when other tasks
	// call EFS API(include NVITEM API)	.
	EFS_Init();
#if defined(SIMLOCK_PROTECT_DATA)
	Simlock_RestoreProtectedNV();
#endif

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
	// Now we check uart tx fifo to set run mode
	RM_GetRunModeResponse ();
	SaveSWRestartCondition();
#endif

#ifndef WIN32
    // The parameter has been download to the module,
    // So it can read successfully, needn't write data to flash at here.
    // These two functions must be called after EFS_Init().
    OSPARAM_InitOsParam();
    REFPARAM_InitRefParam();
    PROD_InitProdParam();

#if defined(PLATFORM_SC6531EFM)
    if(RESTART_BY_ALARM != POWER_GetRestartCondition())
    {
        SaveHWRestartCondition();
    }

    if(RESTART_BY_POWER_BUTTON == POWER_GetRestartCondition())
    {
        if (REFPARAM_GetAutoPowerOnFlag())
        {
            CHIP_DisablePowerKeyInt();
            CHIP_Disable7sRstMode();
        }
    }
#endif
#endif

#if defined(PLATFORM_SC6531EFM)
	// Now we check uart tx fifo to set run mode
	RM_GetRunModeResponse ();
	SaveSWRestartCondition();
#endif

#ifdef ZDT_PCBA_ZW201_SUPPORT
    UART1ArbitrateSwitch (UART_CONTROL_BY_MCU);
#else
#ifndef TDPS_UEIT_ARM_L1SIM_HSDPA
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if( (L1API_DOWNLOADPARAM_GetDSPLogSwitch() == 1) && (REFPARAM_GetDspDebugPortPhyNo() == 1) )
    {
        UART1ArbitrateSwitch (UART_CONTROL_BY_DSP);
    }
    else if( (L1API_DOWNLOADPARAM_GetDSPLogSwitch() == 2)  && (REFPARAM_GetBtDebugPortPhyNo() == 1) )
    {
        UART1ArbitrateSwitch (UART_CONTROL_BY_CM4);
    }
    else
    {
        UART1ArbitrateSwitch (UART_CONTROL_BY_MCU);
    }
#else
    if( L1API_DOWNLOADPARAM_GetDSPLogSwitch() != FALSE)
    {
        if( POWER_IsCalibrationResetMode() )
        {
            DSPLogUARTSwitchOff ();
        }
        else
        {
            DSPLogUARTSwitchOn ();
        }
    }
    else
    {
        DSPLogUARTSwitchOff ();
    }
#endif
#endif
#endif

    if (RESTART_BY_ALARM == POWER_GetRestartCondition())
    {
        if (!TM_IsAlarmExpired())
        {
            //if it is a invalid alarm int, we need to reset the condition to RESTART_BY_NONE status,
            //and next it will enter into USB MCFT mode
            POWER_SetRestartCondition(RESTART_BY_NONE);
        }
    }

#if defined(PLATFORM_SC6531EFM) && (!defined(WIN32))
    if(RESTART_BY_NONE == POWER_GetRestartCondition() \
		|| (CHGMNG_ADP_USB == CHGMNG_IdentifyAdpType()))
#else
    if(RESTART_BY_NONE == POWER_GetRestartCondition())
#endif
    {
#ifndef FPGA_VERIFICATION
        /*enter into USB MCFT Mode*/
        RM_GetVcomRunMode();
#endif

        /*
        * In this period, we may enter into calibration mode
        * or pwr key,charge,alarm conditon is meet.
        * so should save restart condition again.
        */
        SaveSWRestartCondition();
    }

    mode = POWER_GetResetMode();

#ifndef WIN32
    if(mode == CALIBRATION_NV_ACCESS_MODE)
    {
        /*nv write access mode,write imei*/
        RM_Cali_NvAccess();
    }

    // Switch ARM log according to NV flag setting
    SCI_InitLogSwitch(REFPARAM_GetEnableArmLogFlag());

    // In test mode we close ARM log
    if ((RM_GetPinTestFlag())||  POWER_IsCalibrationResetMode( ))
    {
        SCI_InitLogSwitch(SCI_FALSE);
    }

    m_dcb.baud_rate     = SIO_GetBaudRate(COM_DEBUG);
    if( POWER_IsCalibrationResetMode() )
    {
        if(1 == RM_GetDloaderMode())
        {
            /*for USB MCFT*/
            debug_phy_port  = VIR_COM0;
        }
        else
        {
          #if defined(DPHONE_SUPPORT) || defined(CALI_MODE_PORT_UART0)
					   /*uart cft mode, default:uart0, 115200*/
            debug_phy_port  = COM0;
          #else
            /*uart cft mode, default:uart1, 115200*/
            debug_phy_port  = COM1;
          #endif
            REFPARAM_SetDebugPortPhyNo(debug_phy_port);
            m_dcb.baud_rate = 0x71;
        }
    }
    else
    {
        debug_phy_port      = REFPARAM_GetDebugPortPhyNo();
    }
#ifdef ZDT_PCBA_ZW201_SUPPORT
	debug_phy_port = 0xFF;
#endif

    m_dcb.flow_control   = 0;
#ifdef ZDT_PCBA_ZW201_SUPPORT
    debug_phy_port = 0xFF;
#endif
    /* creat COM_DEBUG */
    SIO_Create(COM_DEBUG, debug_phy_port, &m_dcb);

    /*init sio log buffer first*/
    SIO_ResetBuf(COM_DEBUG);

    /* creat COM_DATA */
    data_phy_port        = REFPARAM_GetDataPortPhyNo();
#ifdef ZDT_PCBA_ZW201_SUPPORT
    data_phy_port = 0xFF;
#endif
    m_dcb.baud_rate      = SIO_GetBaudRate(COM_DATA);
#ifndef MUX_SUPPORT
    SIO_Create(COM_DATA, data_phy_port, &m_dcb);
#else
    if( POWER_IsCalibrationResetMode() )
    {
        SIO_Create(COM_DATA, data_phy_port, &m_dcb);

    }
    else
    {
      #ifdef UART0_HWFLOW_SUPPORT
        m_dcb.flow_control = HW_FLOW_CONTROL;
      #endif
        MUX_Create(COM_DATA, data_phy_port, &m_dcb);
        #ifdef DUAL_MUX_SUPPORT
            MUX_Create(COM_DATA2, REFPARAM_GetData2PortPhyNo(), &m_dcb);
        #endif
        /*important, mux and sio share same port, so we should disable sio port when using mux func*/
        SIO_Create(COM_DATA, 0xff, &m_dcb);
    }
#endif
#if defined(PRODUCT_DM)
    SCI_InitRtosBottomHalf( );
#endif

    // Set log filters.
    SCI_InitLogFilter();

    // Init IRAM collision manager
    area_p = GetIRAMSet(&iram_area_cnt);
    if ((SCI_NULL != area_p) && (iram_area_cnt))
    {
        IRAM_Init (area_p, iram_area_cnt);
    }
    else
    {
        //SCI_TRACE_LOW:"\r\nSCI_InitRef: IRAM mgr fail to register"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,INIT_556_112_2_18_1_5_20_30,(uint8*)"");
    }

    // Check if need power on.
    tmp = POWER_GetRestartCondition();

    if (RESTART_BY_NONE == tmp)
    {
        if (REFPARAM_GetAutoPowerOnFlag())
        {
            // This function must be called after EFS_Init().
            POWER_SetRestartCondition(RESTART_BY_SOFTWARE);
        }
        else
        {
            HAL_TurnOffPower();
        }
    }
    else if ( RESTART_BY_POWER_BUTTON == tmp)
	{
		if ( IsPowerButtonReleased() )
		{
		    // we need check the charge status.
		    // when the charge is connect, If the user press the button before check
		    // the restart condition and release it before check the power button here.
		    // System will power down.
		    // But we want the system will restart by charge.
		    if( CHGMNG_IsChargeConnect() )
		    {
		        // The charge is in, need reset the restart condition.
		        // and power on
		        POWER_SetRestartCondition( RESTART_BY_CHARGE );
		    }
		    else
		    {
		        HAL_TurnOffPower();
		    }
		}
	}
	else if ( RESTART_BY_CHARGE == tmp)//for auto test
	{

		 if (REFPARAM_GetAutoPowerOnFlag())
		 {
		    POWER_SetAutoTestMode();/*lint !e746*/
		 }

	   if (POWER_IsAutoTestMode())

	   {
	      // This function must be called after EFS_Init().
	      POWER_SetRestartCondition(RESTART_BY_SOFTWARE);
	   }
	}

    // Phase3 of chip driver intialization. It initializes driver modules
    // after refbase confirms powering on.
	CHIPDRV_InitOnPhase3 ();

#ifndef MODEM_PLATFORM

#ifdef SPLOAD_ENABLE
    {
    	extern void LCM_Sensor_SperateBin_Init(void);
	    LCM_Sensor_SperateBin_Init();
	}
#endif

#ifndef NANDBOOT_SUPPORT
    #ifdef UMEM_SUPPORT
	    if(0xFFFFFFFF != (uint32)FLASH_GetUmemAddr())
        {
            if( ( CALIBRATION_POST_MODE != mode)&& \
                ( CALIBRATION_MODE != mode) &&  \
                ( CALIBRATION_POST_NO_LCM_MODE != mode))
            {
                Umem_RegBSD();
            }
	    }
	#endif
#endif

#ifdef IMG_EDITER_SUPPORT
	IMGPROC_Init(NULL);
#endif

	ISP_ServiceInit();

        LCD_Init();

	switch(mode)
	{
        case WATCHDOG_RESET_MODE:
        case EXTERNAL_RESET_MODE:
 			/* Set to RESTART_BY_ASSERT condition in order to MMI handle.*/
			POWER_SetRestartCondition(RESTART_BY_ASSERT);
			//no break;
        case ASSERT_BACK_MODE:	/*lint !e825*/
            if(!BOOT_GetBLStatus())  //bl is turn off
            {
			     LCD_Clear(MAIN_LCD_ID, 0x00);
            }
			break;
		default:
			break;
	}

    IMG_DEC_Init();
#endif

    // Export interface for external user setting
    // For the product parameter initialization
    // After that welcome logo will be showed
    PROD_Init();
#ifdef SECURITY_DATA_SUPPORT
	Security_Data_Init();
	Security_Data_WriteAndSelftest();
#endif
    //set the init status to STATUS_FINISH_PROD_INIT
    INIT_SetInitStatus(STATUS_FINISH_PROD_INIT);
#ifndef MODEM_PLATFORM
	_INIT_CopyImage();
#endif
    // Phase4 of chip driver intialization. It initializes MMU after
    // refbase has copied the kernel image.
    CHIPDRV_InitOnPhase4 ();

#ifdef DSP_USB_LOG
    SIO_DSP_LOG_Info_Notify();

    if (DSP_LOG_ARM_COM_DEBUG == REFPARAM_GetDspDebugPortPhyNo())
    {
		/*init DSP IRQ*/
	if (TB_SUCCESS == ISR_RegHandler(TB_DSP_INT1, (TB_ISR)SIO_DSP_ISR_Handler))
	{
		*(volatile uint32 *)(GR_ICLR) |= 0x04;  //Clear DSP IRQ1
		TB_DisableFIQ(TB_DSP_INT1);
		TB_EnableINT(TB_DSP_INT1);
	}

    	SIO_DspLogUSBEnableSet(TRUE);
	SIO_CreateDSPLogThread();
    }
    else
   {
    	SIO_DspLogUSBEnableSet(FALSE);
   }
#endif

#ifdef BT_USB_LOG
    SIO_BT_LOG_Info_Notify();

    if(BT_LOG_ARM_COM_DEBUG == REFPARAM_GetBtDebugPortPhyNo())
    {
        if (TB_SUCCESS == ISR_RegHandler(TB_CM4_TO_ARM9_INT2, (TB_ISR)SIO_BT_ISR_Handler))  //handler??????,handle??????clear irq
        {
             *(volatile uint32 *)(0X8B000160) |= BIT_18;  //Clear IRQ
             TB_DisableFIQ(TB_CM4_TO_ARM9_INT2);
             TB_EnableINT (TB_CM4_TO_ARM9_INT2);
        }
    	SIO_BTLOGUSBEnableSet(TRUE);
	SIO_CreateBTLOGThread();
    }
    else
   {
    	SIO_BTLOGUSBEnableSet(FALSE);
   }
#endif

#else
	IMG_DEC_Init();
#endif  /*WIN32*/

#ifdef PRJ_TDPS_UEIT_MMI
    SCI_InitRtosBottomHalf( );
#endif

#ifndef FPGA_VERIFICATION
#if defined(FM_SUPPORT) && defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#ifndef WIN32
    fm_calibration_main();
#endif
#endif
#endif

#ifdef MODEM_PLATFORM
#if defined(ATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#ifndef WIN32
    if(CHIP_TYPE_EX_AA_AB == CHIP_GetChipTypeEx())
    {
        // WA: chip defect Bug#684810
        // set VDDRF default to idle
        FM_Set_VDDRF_IdleMode();
    }
#endif
#endif
#endif

    PROD_InitByStep(PROD_INIT_AFTER_USER_IMAGE);

    // Initialize audio.
    AUD_Init();

 	//charge server & do idle on page init
 	DoIdle_Init( );    // this depend on nothing.

	// Create reference thread.
    CreateRefTasks(mode);

#ifdef FOTA_SUPPORT_REDSTONE
    if (RESTART_BY_CHARGE != POWER_GetRestartCondition())
    {
		rs_sdk_start();
    }
#endif

#ifdef FOTA_SUPPORT_ADUPS
    if (RESTART_BY_CHARGE != POWER_GetRestartCondition())
    {
		adups_task_create();
    }
#endif

#ifdef DEMOD_HW_INNOFIDEI
#ifndef WIN32
    INNO_DoDemodLowPowerMode();
#endif
#endif

#ifndef MODEM_PLATFORM
	SFS_Init(0);
#endif
	// Display version information by trace msg
    ShowVersionInfo();
	//CheckNVVersion();

#ifndef  WIN32
	REF_CheckCalibrationParamVersion(REF_GetCalibrationParamVersion());
	REF_CheckDownloadParamVersion(REF_GetDownloadParamVersion());
#endif

#ifdef  WIN32

    CHR_Init();
    // The parameter hasnot been download to the module,
    // it will failed reading from flash. And it need write data to flash at here.
    // So it should be called after EFS task created.

    // These two functions must be called after EFS_Init().
    OSPARAM_InitOsParam();
    REFPARAM_InitRefParam();
    // Set log filters.
    SCI_InitLogFilter();
#endif  // WIN32

#ifndef WIN32
	#ifdef AUTOTEST_DEVICE_SUPPORT
	    Register_DeviceAutoTest_CmdRoutine();
	#endif
#endif

    POWER_SetMsStatus(POWER_MS_STATUS_POWER_OFF);

#ifndef  WIN32
    #ifdef VT_SUPPORT
        $Super$$__cpp_initialise();  /*lint !e27 */
    #endif
#endif

#ifndef WIN32
    #ifdef CMMB_SUPPORT
        // Init CMMB test thread
        if (RM_GetCalibrationMode())
        {
            DTLRFVT_ThreadInit();
        }
        // Init V log To BB flag
        MTV_InitVLogFlag();
    #endif
#endif
#ifdef FPGA_TEST
       /* ADI audio auto gate */
       ADI_init_AUDIO();
#endif
       return SCI_SUCCESS;
}
#else
//uint8 g_debug_phy_port, g_data_phy_port;
extern void SIO_SetAtFromDebugPort(void);

#ifndef BLUETOOTH_SUPPORT
BOOLEAN BT_IsEnabled(void)
{
    return FALSE;
}
#endif

void SCI_DcxoSet(void)
{
	uint32 dcxo_trim, rg_ldo_rtc_cal;
	uint32 global_reg;
#ifndef  WIN32
	 EFUSE_HAL_GetDCXOTrim(&dcxo_trim);
	 EFUSE_HAL_GetRGLdoRtcCal(&rg_ldo_rtc_cal);

	 global_reg = (ANA_REG_GET(0xD24) & 0xf07f) | (dcxo_trim << 7);
	 ANA_REG_SET(0xD24, global_reg);

	 global_reg = (ANA_REG_GET(0xD4C) & 0xfe0f) | (rg_ldo_rtc_cal << 4);
	 ANA_REG_SET(0xD4C, global_reg);
#endif
}
uint8 g_diag_mode_flg = NORMAL_MODE;

PUBLIC uint32 SCI_InitRef(uint32 mode)
{
    uint32 tmp;
#ifndef WIN32
    uint8  			debug_phy_port = 0xff;
    uint8  			data_phy_port = 0xff;
	SIO_CONTROL_S   m_dcb = {0};

#endif
    uint32			iram_area_cnt = 0;
    IRAM_AREA * 	area_p = SCI_NULL;

#ifndef WIN32
    Debug_Uart4_puts("Enter SCI_InitRef\n");
#endif
	POWER_SetMsStatus(POWER_MS_STATUS_INIT);

		      // power down sdmmc ldo,because the default status is power on.
#ifndef WIN32
	#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
	 	ANA_REG_SET(REG_RDA2720M_GLOBAL_BASE+0xF4,1); // sd
	#else
		ANA_REG_SET(REG_RDA2720M_GLOBAL_BASE+0x100,1); // sd
		ANA_REG_SET(REG_RDA2720M_GLOBAL_BASE+0x10c,1);//sdio
	#endif


	TM_SendInitRequest(); //kevin.wang@0719

    // Phase2 of chip driver intialization. It initializes driver modules
    // before refbase confirms powering on.

    CHIPDRV_InitOnPhase2 ();

#endif
#ifndef WIN32

    CharDev_Initialize();
#endif

	// Diag init;
	InitDiag();  //Added by Johnson.sun 2006/10/09

	// Init BSD interface

	BSD_Initialize ();

#ifdef NANDBOOT_SUPPORT
    if(CHIP_DetectMemType())
    {
      // Init XSR
#if !defined(WIN32)

	SCI_FTLMnt_Init ();
#endif
    }
#endif

	// Init EFS Module here, and EFS_Task not init it now.
	// So we can make sure that the EFS module has been init-ed when other tasks
	// call EFS API(include NVITEM API)	.

	EFS_Init();

	SCI_DcxoSet();
#ifndef WIN32
    // The parameter has been download to the module,
    // So it can read successfully, needn't write data to flash at here.
    // These two functions must be called after EFS_Init().

    OSPARAM_InitOsParam();

    REFPARAM_InitRefParam();

    PROD_InitProdParam();
#if 1

    if(RESTART_BY_ALARM != POWER_GetRestartCondition())
    {
        SaveHWRestartCondition();
    }

    if(RESTART_BY_POWER_BUTTON == POWER_GetRestartCondition())
    {
        if (REFPARAM_GetAutoPowerOnFlag())
        {
            CHIP_DisablePowerKeyInt();
            CHIP_Disable7sRstMode();
        }
    }
#endif
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
       if (!g_diag_mode_flg)     /*not change to calib mode from diag cmd*/
       {
        	// Now we check uart tx fifo to set run mode
        	RM_GetRunModeResponse ();
       }

	SaveSWRestartCondition();
#endif

#ifdef MODEM_SYS_IN_AP_SIDE

#ifndef TDPS_UEIT_ARM_L1SIM_HSDPA

#if 0
    if( L1API_DOWNLOADPARAM_GetDSPLogSwitch() != FALSE)
    {
        if( POWER_IsCalibrationResetMode() )
        {
            DSPLogUARTSwitchOff ();
        }
        else
        {
            DSPLogUARTSwitchOn ();
        }
    }
    else
    {
        DSPLogUARTSwitchOff ();
    }
#endif
#endif
#endif

    if (RESTART_BY_ALARM == POWER_GetRestartCondition())
    {
        if (!TM_IsAlarmExpired())
        {
            //if it is a invalid alarm int, we need to reset the condition to RESTART_BY_NONE status,
            //and next it will enter into USB MCFT mode
            POWER_SetRestartCondition(RESTART_BY_NONE);
        }
    }

#if 1
#if (defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)) && (!defined(WIN32))
    if(RESTART_BY_NONE == POWER_GetRestartCondition()
	#if PMIC_CHARGER_DETECT_SUPPORT
		|| CHGMNG_ADP_USB == CHGMNG_IdentifyAdpType()
	#else
		|| CHGMNG_IsChargeConnect()
	#endif
	)
#else
    if(RESTART_BY_NONE == POWER_GetRestartCondition())
#endif
    {
#ifndef FPGA_VERIFICATION
       if (!g_diag_mode_flg)     /*not change to calib mode from diag cmd*/
        {
            /*enter into USB MCFT Mode*/
            RM_GetVcomRunMode();
        }
#endif

        /*
        * In this period, we may enter into calibration mode
        * or pwr key,charge,alarm conditon is meet.
        * so should save restart condition again.
        */
        SaveSWRestartCondition();
    }
#endif
    mode = POWER_GetResetMode();

#ifndef WIN32
#if 1
    if(mode == CALIBRATION_NV_ACCESS_MODE)
    {
        /*nv write access mode,write imei*/
        RM_Cali_NvAccess();
    }
#endif
    // Switch ARM log according to NV flag setting
    SCI_InitLogSwitch(REFPARAM_GetEnableArmLogFlag());
;
    if ((RM_GetPinTestFlag())||  POWER_IsCalibrationResetMode( ))
    {
        SCI_InitLogSwitch(SCI_FALSE);
    }

	//m_dcb.baud_rate = 921600;
    m_dcb.baud_rate     = SIO_GetBaudRate(COM_DEBUG);

    if( POWER_IsCalibrationResetMode() )
    {
        if(1 == RM_GetDloaderMode())
        {
            /*for USB MCFT*/
            debug_phy_port  = VIR_COM0;
        }
        else
        {
//#if defined(DPHONE_SUPPORT) || defined(CALI_MODE_PORT_UART0)
#if defined(CALI_MODE_PORT_UART0)
            /*uart cft mode, default:uart0, 115200*/
            debug_phy_port  = COM0;
#else
            /*uart cft mode, default:uart1, 115200*/
            debug_phy_port  = COM2;
#endif
            REFPARAM_SetDebugPortPhyNo(debug_phy_port);
#if !defined(PLATFORM_UWS6121E)
            m_dcb.baud_rate = 0x71;
            m_dcb.flow_control   = NO_FLOW_CONTROL;
#else
            m_dcb.baud_rate = BAUD_2000000;
            m_dcb.flow_control   = HW_FLOW_CONTROL;
#endif
        }
    }
    else
    {
       debug_phy_port      = REFPARAM_GetDebugPortPhyNo();
	m_dcb.flow_control   = NO_FLOW_CONTROL;
    }

#if defined(PLATFORM_UWS6121E)
#ifdef BT_LOG_MENU_SUPPORT
    MMIAPISET_BTLogInit();
    /*uart cft mode, default:uart1, 115200*/
    if (MMIAPISET_GetBTLogEnable())
    {
        debug_phy_port  = REFPARAM_GetDebugPortPhyNo();     /*8910m BT_test_use aptrace use uart1*/
        //SIO_SetAtFromDebugPort();
        m_dcb.flow_control   = NO_FLOW_CONTROL;
        ipc_set_bt_flow(0x11223344);
        REFPARAM_SetDebugPortPhyNo(debug_phy_port);
    }
#endif
#endif
#if defined(MOS_SUPPORT)    /*uart3(ap log) not be used,but need to set it NO_FLOW_CONTROL*/
    m_dcb.flow_control   = NO_FLOW_CONTROL;
#endif

    /* creat COM_DEBUG */
    debug_phy_port = REFPARAM_GetDebugPortPhyNo();//debug_phy_port=COM0;

    SIO_Create(COM_DEBUG, debug_phy_port, &m_dcb);
#if defined(MODEM_LOG)
    SIO_Create(COM_DEBUG2, debug_phy_port, &m_dcb);
#endif
    /*init sio log buffer first*/
    SIO_ResetBuf(COM_DEBUG);
    /* creat COM_DATA */
    data_phy_port        = REFPARAM_GetDataPortPhyNo();
#if defined(PLATFORM_UWS6121E)
    if( POWER_IsCalibrationResetMode() ) {
//#if defined(DPHONE_SUPPORT) || defined(CALI_MODE_PORT_UART0)
#if defined(CALI_MODE_PORT_UART0)
        /*uart cft mode, default:uart0, 115200*/
        data_phy_port  = COM2;
#else
        /*uart cft mode, default:uart1, 115200*/
        data_phy_port  = COM0;
#endif
    }
#ifdef BT_LOG_MENU_SUPPORT
    if (MMIAPISET_GetBTLogEnable())
    {
        data_phy_port        = REFPARAM_GetDataPortPhyNo();    /*8910m BT_test_use aptrace use uart1*/
    }
#endif
#endif
    m_dcb.flow_control   = NO_FLOW_CONTROL;
    m_dcb.baud_rate      = SIO_GetBaudRate(COM_DATA);
#ifndef MUX_SUPPORT
	data_phy_port=0xFF;
    SIO_Create(COM_DATA, data_phy_port, &m_dcb);
#else
    if( POWER_IsCalibrationResetMode() )
    {
        SIO_Create(COM_DATA, data_phy_port, &m_dcb);

    }
    else
    {
      #ifdef UART0_HWFLOW_SUPPORT
        m_dcb.flow_control = HW_FLOW_CONTROL;
      #endif
        MUX_Create(COM_DATA, data_phy_port, &m_dcb);
        #ifdef DUAL_MUX_SUPPORT
            MUX_Create(COM_DATA2, REFPARAM_GetData2PortPhyNo(), &m_dcb);
        #endif
        /*important, mux and sio share same port, so we should disable sio port when using mux func*/
        SIO_Create(COM_DATA, 0xff, &m_dcb);
    }
#endif
#ifdef PPP_USB
        ppp_read_init();
#endif
#if defined(PRODUCT_DM)
    SCI_InitRtosBottomHalf( );
#endif
    // Set log filters.
    SCI_InitLogFilter();

    // Init IRAM collision manager
    area_p = GetIRAMSet(&iram_area_cnt);
    if ((SCI_NULL != area_p) && (iram_area_cnt))
    {
        IRAM_Init (area_p, iram_area_cnt);
    }
    else
    {
        //SCI_TRACE_LOW:"\r\nSCI_InitRef: IRAM mgr fail to register"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,INIT_556_112_2_18_1_5_20_30,(uint8*)"");
    }
#if 1
    // Check if need power on.
	    tmp = POWER_GetRestartCondition();

	    if (RESTART_BY_NONE == tmp)
	    {
	        if (REFPARAM_GetAutoPowerOnFlag())
	        {
	            // This function must be called after EFS_Init().
	            POWER_SetRestartCondition(RESTART_BY_SOFTWARE);
	        }
	        else
	        {
	            HAL_TurnOffPower();
	        }
	    }
	    else if ( RESTART_BY_POWER_BUTTON == tmp)
		{
			if ( IsPowerButtonReleased() )
			{
			    // we need check the charge status.
			    // when the charge is connect, If the user press the button before check
			    // the restart condition and release it before check the power button here.
			    // System will power down.
			    // But we want the system will restart by charge.
			    if( CHGMNG_IsChargeConnect() )
			    {
			        // The charge is in, need reset the restart condition.
			        // and power on
			        POWER_SetRestartCondition( RESTART_BY_CHARGE );
			    }
			    else
			    {
			        HAL_TurnOffPower();
			    }
			}
		}
		else if ( RESTART_BY_CHARGE == tmp)//for auto test //only charge
		{
		    if (REFPARAM_GetAutoPowerOnFlag())
		    {
		        POWER_SetAutoTestMode();/*lint !e746*/
		    }

		    if (POWER_IsAutoTestMode())
		    {
		      // This function must be called after EFS_Init().
		        POWER_SetRestartCondition(RESTART_BY_SOFTWARE);
		    }
		}
        else if ( RESTART_BY_SOFTWARE == tmp)//power on
		{
			 if (REFPARAM_GetAutoPowerOnFlag() && CHGMNG_IsChargeConnect() )
			 {
			    POWER_SetAutoTestMode();/*lint !e746*/  //for auto test
			 }
		}
#endif
    // Phase3 of chip driver intialization. It initializes driver modules
    // after refbase confirms powering on.
    CHIPDRV_InitOnPhase3 ();

#ifndef MODEM_PLATFORM

#ifdef SPLOAD_ENABLE
    {
    	extern void LCM_Sensor_SperateBin_Init(void);
	    LCM_Sensor_SperateBin_Init();
	}
#endif

#ifndef NANDBOOT_SUPPORT
    #ifdef UMEM_SUPPORT
	    if(0xFFFFFFFF != (uint32)FLASH_GetUmemAddr())
        {
            if( ( CALIBRATION_POST_MODE != mode)&& \
                ( CALIBRATION_MODE != mode) &&  \
                ( CALIBRATION_POST_NO_LCM_MODE != mode))
            {
                Umem_RegBSD();
            }
	    }
	#endif
#endif
#ifdef IMG_EDITER_SUPPORT
	IMGPROC_Init(NULL);
#endif
#endif
#if !defined(FPGA_SUPPORT)
    //ISP_ServiceInit();
    LCD_Init();
    switch(mode)
    {
        case WATCHDOG_RESET_MODE:
        case EXTERNAL_RESET_MODE:
            /* Set to RESTART_BY_ASSERT condition in order to MMI handle.*/
#ifndef WIN32
			Debug_Uart4_puts("Enter RESTART_BY_ASSERT\n");
#endif
            POWER_SetRestartCondition(RESTART_BY_ASSERT);
            //no break;
        case ASSERT_BACK_MODE:	/*lint !e825*/
            if(!BOOT_GetBLStatus())  //bl is turn off
            {
                LCD_Clear(MAIN_LCD_ID, 0x00);
            }
            break;
        default:
            break;
    }
#ifndef WIN32
	Debug_Uart4_puts("Enter IMG_DEC_Init\n");
#endif
    IMG_DEC_Init();
#endif

    // Export interface for external user setting
    // For the product parameter initialization
    // After that welcome logo will be showed

    PROD_Init();
#if defined(RF_HW_SELFADAPTIVE_ENABLE) && defined(RF_BAND_SELFADAPTIVE)
	ipc_set_modem_rf_band(SCI_GET_RF_BAND_ID());
#endif

#if defined(SIMLOCK_PROTECT_DATA)
     if ( !POWER_IsCalibrationResetMode() )
    {
        if (!MNPHONE_SIMLockDataIntegrityCheck())
        {
           if(SIMLOCK_PROTECT_NV_ERROR == Simlock_RestoreProtectedNV())
           {
                POWER_Reset();
            }
        }
    }
#endif
#ifdef SECURITY_DATA_SUPPORT
	Security_Data_Init();
	Security_Data_WriteAndSelftest();
#endif
    //set the init status to STATUS_FINISH_PROD_INIT
#ifndef WIN32
    Debug_Uart4_puts("Enter INIT_SetInitStatus\n");
#endif
    INIT_SetInitStatus(STATUS_FINISH_PROD_INIT);
#ifndef MODEM_PLATFORM
	//_INIT_CopyImage();
#endif
    // Phase4 of chip driver intialization. It initializes MMU after
    // refbase has copied the kernel image.
#ifndef WIN32
    Debug_Uart4_puts("Enter CHIPDRV_InitOnPhase4\n");
#endif
    CHIPDRV_InitOnPhase4 ();
#ifndef WIN32
	Debug_Uart4_puts("END CHIPDRV_InitOnPhase4\n");
#endif
#ifdef DSP_USB_LOG
    SIO_DSP_LOG_Info_Notify();
    if (DSP_LOG_ARM_COM_DEBUG == REFPARAM_GetDspDebugPortPhyNo())
    {
        /*init DSP IRQ*/
        if (TB_SUCCESS == ISR_RegHandler(TB_DSP_INT1, (TB_ISR)SIO_DSP_ISR_Handler))
        {
            *(volatile uint32 *)(GR_ICLR) |= 0x04;  //Clear DSP IRQ1
            TB_DisableFIQ(TB_DSP_INT1);
            TB_EnableINT(TB_DSP_INT1);
        }
        SIO_DspLogUSBEnableSet(TRUE);
        SIO_CreateDSPLogThread();
    }
    else
   {
        SIO_DspLogUSBEnableSet(FALSE);
   }
#endif

#ifdef WIFI_SUPPORT_UNISOC_RX
   drvWifirxInit();
#endif
#ifdef BT_USB_LOG
    //SIO_BT_LOG_Info_Notify();
    SIO_CreateBTLOGThread();
#endif
#else    /*WIN32*/
    IMG_DEC_Init();
#endif  /*WIN32*/

#if 0
#ifdef PRJ_TDPS_UEIT_MMI
    SCI_InitRtosBottomHalf( );
#endif

#ifndef FPGA_VERIFICATION
#if defined(FM_SUPPORT) && defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#ifndef WIN32
    fm_calibration_main();
#endif
#endif
#endif

#ifdef MODEM_PLATFORM
#if defined(ATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#ifndef WIN32
    if(CHIP_TYPE_EX_AA_AB == CHIP_GetChipTypeEx())
    {
        // WA: chip defect Bug#684810
        // set VDDRF default to idle
        FM_Set_VDDRF_IdleMode();
    }
#endif
#endif
#endif

    PROD_InitByStep(PROD_INIT_AFTER_USER_IMAGE);

#endif
    // Initialize audio.
    AUD_Init();

    //charge server & do idle on page init
#ifndef WIN32
    Debug_Uart4_puts("Enter DoIdle_Init\n");
#endif
    DoIdle_Init( );    // this depend on nothing.
    // Create reference thread.
//zhouqin modify
    SCI_TraceLow("zhhouqin trace SCI_InitRef == mode =%d",mode);
#ifndef WIN32
	Debug_Uart4_puts("Enter CreateRefTasks\n");
#endif
    CreateRefTasks(mode);
   //load modem bin
#ifndef WIN32
#if defined(PLATFORM_UWS6121E)
    Debug_Uart4_puts("Enter LOAD_modem\n");
    LOAD_modem();
    Debug_Uart4_puts("END LOAD_modem\n");
    Debug_Uart4_puts("Enter ipc_init\n");
    ipc_init();

    Debug_Uart4_puts("Enter LOAD_modem_phase2\n");
    LOAD_modem_phase2();
    Debug_Uart4_puts("END LOAD_modem_phase2\n");

#endif
#endif

#if 1
#ifdef FOTA_SUPPORT_REDSTONE
    if (RESTART_BY_CHARGE != POWER_GetRestartCondition())
    {
        rs_sdk_start();
    }
#endif

#ifdef FOTA_SUPPORT_ADUPS
    if (RESTART_BY_CHARGE != POWER_GetRestartCondition())
    {
        adups_task_create();
    }
#endif

#ifdef DEMOD_HW_INNOFIDEI
#ifndef WIN32
    INNO_DoDemodLowPowerMode();
#endif
#endif
#endif

#ifndef MODEM_PLATFORM
    SFS_Init(0);
#endif
    // Display version information by trace msg
    ShowVersionInfo();
    //CheckNVVersion();

#ifndef  WIN32
	REF_CheckCalibrationParamVersion(REF_GetCalibrationParamVersion());
	REF_CheckDownloadParamVersion(REF_GetDownloadParamVersion());
#endif

#ifdef  WIN32
    CHR_Init();
    // The parameter hasnot been download to the module,
    // it will failed reading from flash. And it need write data to flash at here.
    // So it should be called after EFS task created.

    // These two functions must be called after EFS_Init().
    OSPARAM_InitOsParam();
    REFPARAM_InitRefParam();
    // Set log filters.
    SCI_InitLogFilter();
#endif  // WIN32

#ifndef WIN32
	#ifdef DIAG_EX_SUPPORT
        Register_Ex_CmdRoutine();
	#endif
	#ifdef AUTOTEST_DEVICE_SUPPORT
	    Register_DeviceAutoTest_CmdRoutine();
	#endif
#endif


    POWER_SetMsStatus(POWER_MS_STATUS_POWER_OFF);
#if 1
#ifndef  WIN32
    #ifdef VT_SUPPORT
            $Super$$__cpp_initialise();  /*lint !e27 */
    #endif
#endif

#ifndef WIN32
    #ifdef CMMB_SUPPORT
        // Init CMMB test thread
        if (RM_GetCalibrationMode())
        {
            DTLRFVT_ThreadInit();
        }

        // Init V log To BB flag
        MTV_InitVLogFlag();
    #endif
#endif
#ifdef FPGA_TEST
       /* ADI audio auto gate */
       ADI_init_AUDIO();
#endif
#endif
#ifndef WIN32
	Debug_Uart4_puts("Enter SCI_InitRef_END\n");
#endif
#ifndef WIN32
	#ifdef GPS_CHIP_VER_GREENEYE2
    	GPS_Init();
	#endif
#endif /* WIN32 */

#if 0
int initcout = 0;
	while(initcout == 0)
	{
		Debug_Uart4_puts("Enter CharDev_Initialize_1\n");
	}
#endif
       return SCI_SUCCESS;
}
#endif
/*****************************************************************************/
// Description :    Initialize Hardware.
//                  Include ISR, clock, timer, RTC, serial port.
// Global resource dependence :
// Author :         Richard.Yang
// Note :
/*****************************************************************************/
PUBLIC void SCI_InitHw(void)
{
}

/*****************************************************************************/
// Description :   Show Version information
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
LOCAL void ShowVersionInfo()
{
#ifndef WIN32
    const   char * info_ptr = NULL;
    uint32 t = 0;
    uint32 num = VERSION_GetInfoNum();

    //SCI_TRACE_LOW:"\r\n============Version Information=============\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,INIT_793_112_2_18_1_5_20_31,(uint8*)"");
    for(t = 0; t < num; t++)
    {
        info_ptr = VERSION_GetInfo(t);
        if(info_ptr)
        {
           SCI_TRACE_LOW(info_ptr);
        }
    }

	// Show RTOS build time!
    info_ptr = SCI_GetRtosCompileDate();
    //SCI_TRACE_LOW:"RTOS Compile Date : %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,INIT_803_112_2_18_1_5_20_32,(uint8*)"s", info_ptr);

	// Show Ref build time!
    info_ptr = GetRefCompileDate();
    //SCI_TRACE_LOW:"REF Compile Date : %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,INIT_807_112_2_18_1_5_20_33,(uint8*)"s", info_ptr);

    //SCI_TRACE_LOW:"\r\n=============================================\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,INIT_809_112_2_18_1_5_20_34,(uint8*)"");
#endif
}

/*****************************************************************************/
// Description :    Create reference tasks.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
LOCAL int CreateRefTasks(uint32 mode)
{
	switch(mode)
	{
		case CALIBRATION_MODE:
		case LAYER1_TEST_MODE:
		case DSP_CODE_DOWNLOAD_BACK_CALIBRATION:
			STATIC_TASK_CREATE(Cal);
			break;

		case CALIBRATION_POST_NO_LCM_MODE:
#ifndef WIN32
			STATIC_TASK_CREATE(CalNoApp);
#endif
			break;

		default:
			SCI_TraceLow("zhhouqin trace CreateRefTasks");
			STATIC_TASK_CREATE(Normal);
	}

	return SCI_SUCCESS;
}

/*****************************************************************************/
// Description:    The function is to define the user's initialization
//                  function. User implement this function.
// Global resource dependence:
// Author:         Xueliang.Wang
// Note:
/*****************************************************************************/
uint32 debug_var = 0;

PUBLIC void SCI_ApplicationDefine(void *first_unused_memory)
{
	uint16	reset_mode;
	uint32 i=0;

#ifndef WIN32
	//set the init status to STATUS_START_INIT
	INIT_SetInitStatus(STATUS_START_INIT);
#ifdef UART1_PIN_SEL_UART1_1 // sdio pin
	// SD's UART1 port should be EN_SDIO_LDO() in power_cfg.c before use
	LDO_Init();
	LDO_Prod_Init();
#endif

       reset_mode = POWER_GetResetMode();
       if ((NORMAL_MODE != (reset_mode & MODE_MAX_MASK)) || (reset_mode & 0x80))     /*first POWER_GetResetMode(),check mode from diag cmd or not*/
       {
              g_diag_mode_flg = 1;
       }
       else
       {
              g_diag_mode_flg = 0;
        	// Split the function (RM_GetRunMode) into two parts
        	RM_SendRunModeRequest();
       }

    	/* Save restart condition.*/
    	SaveHWRestartCondition();   //���δ�׮
#endif

	// while(debug_var==0);
	/* Initialize RTOS.     */
	SCI_InitRtos();

	SCI_TraceLow("zhhouqin trace SCI_ApplicationDefine");
	/* Initialize Reference.*/
	SCI_InitRef(0);

#if defined(PLATFORM_UWS6121E)
  	reset_mode = POWER_GetResetMode();

	 // init ipc, calib and normal mode all need
#ifndef WIN32


	 NV_IPC_init();

        if(CALIBRATION_MODE == reset_mode)
        {
		CHIP_Disable7sRstMode();      //disable 7s rst
		ipc_set_modem_boot_mode(2);
		CFW_RpcInit();
	 }
        else
        {
              // CHIP_PHY_PB1KeyHwRstSet();     /*enable 7s hwrest*/
		 ipc_set_modem_boot_mode(0);
        }
#endif
#else
#if defined(CHIP_VER_6531EFM) && defined(BLUETOOTH_SUPPORT)
	Shm_Data_Init();
#endif


#endif

#ifndef WIN32

     ///32K ageing thread init
     FLASH_RfParaStoreThreadInit();

#ifdef CARD_LOG
	SCI_InitLogSaveTask();
#endif

#if  !defined(PLATFORM_UWS6121E)
#ifdef DSP_IQ_DEBUG
	//select iq function
	(*((volatile uint32 *)(0x8b00034C))) &= (~0xF);
    (*((volatile uint32 *)(0x8b00034C))) |= 0x1;
    (*((volatile uint32 *)(0x8B00022C))) |= BIT_1;
    (*((volatile uint32 *)(0x8B00034C))) &= (~(0xFF<<4));

	//configure iq pin
	(*((volatile uint32 *)(0x8c000128))) = 0x102000;
	(*((volatile uint32 *)(0x8c000194))) = 0x82030;
	(*((volatile uint32 *)(0x8c00019c))) = 0x82030;
	(*((volatile uint32 *)(0x8c0001a0))) = 0x82030;
	(*((volatile uint32 *)(0x8c0001a4))) = 0x82030;
	(*((volatile uint32 *)(0x8A000108))) &= (~0xF); //set gpio to input mode
	(*((volatile uint32 *)(0x8A000128))) &= (~0xF); //disable gpio input

	//configure uart pin
	(*((volatile uint32 *)(0x8c000038))) = 0x82010; //BB_U1TXD
	(*((volatile uint32 *)(0x8c00003c))) = 0x82090; //BB_U1RXD
	(*((volatile uint32 *)(0x8b0001e0))) |= (0x2 << 6); //select UART1_1
#else
	if(SC6531EFM == CHIP_GetChipType())
	{
		(*((volatile uint32 *)(0x8c000194))) = 0x2074;
		(*((volatile uint32 *)(0x8c00019c))) = 0x2074;
		(*((volatile uint32 *)(0x8c0001a0))) = 0x20B4;
		(*((volatile uint32 *)(0x8c0001a4))) = 0x2034;
	}
#endif
#endif

  	//while(debug_var==1);

   //start modem bin
  #if defined(PLATFORM_UWS6121E)
  UART_Clk_Enable(UART_COM1); // UART2 Clk enable, for cp-log
  //CHIP_MODEM_CP_Start();
  Modem_Init();
  #else
  	reset_mode = POWER_GetResetMode();
  #endif

	switch(reset_mode)
	{
		case CALIBRATION_MODE:
		case DSP_CODE_DOWNLOAD_BACK_CALIBRATION:
			/* Start Calibration resource.*/
			SCI_InitAtc();
			#if !defined(PLATFORM_UWS6121E)
			L1API_SCI_StartCalibration();
			#else
     	// calibration TBD
    	#endif
			break;

		#if !defined(PLATFORM_UWS6121E)
		case LAYER1_TEST_MODE:
			/* Start Layer test resource.*/
   	   		SCI_StartLayer1Test();
			break;
   	#endif

		case ASSERT_BACK_MODE:
       		/* Power On automatically. */
       		POWER_PowerOn();
			break;

		case WATCHDOG_RESET_MODE:
	 		/* Power On automatically. */
         	POWER_PowerOn();
			break;

		default:
	        /* Power On automatically. */
	        POWER_PowerOn();
	}

    BOOT_InitResetMode ();
     // add test code here @@@fuxiang
	SCI_TraceLow("enter halo test");

	// halo_MainEntry();
	// halo_init();
	//halo_MainInit();
	//init_apcpverify(); //disable apcpverify checking
	//halo_init_test();
#ifdef HALO_SUPPORT
//#if (HALO_SUPPORT == TRUE)
#if (1)
	if(reset_mode == CALIBRATION_MODE ||reset_mode == CALIBRATION_POST_MODE || \
       reset_mode == CALIBRATION_NV_ACCESS_MODE || reset_mode == CALIBRATION_POST_NO_LCM_MODE )
	   {
		   SCI_TraceLow("@@@halo in calibration not init halo_ctrl");
	   }
	   halo_ctrl_init(&reset_mode);

#endif
#endif
	SCI_TraceLow("leave halo test");

#else   /* not define WIN32 */
    /* Power On automatically.*/
    POWER_PowerOn();
#endif  /* WIN32 */

}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //
