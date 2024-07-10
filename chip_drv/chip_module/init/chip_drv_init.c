/******************************************************************************
 ** File Name:      chip_drv_init.c                                           *
 ** Author:         Jeff Li                                                   *
 ** DATE:           06/09/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces called during system and *
 **                 driver initialization, which export to init.c of refbase. *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/09/2010     Jeff.Li          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//lint -e766
#include "os_api.h"
#include "chip_plf_export.h"
#include "chg_drvapi.h"
#include "dma_drv_internal.h"
#include "clock_drvapi.h"
#include "watchdog.h"
#include "emc_drvapi.h"
#include "ldo_drvapi.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "rtc_drvapi.h"
#include "timer_drvapi.h"
#include "nlc_drvapi.h"
#include "gpio_drvapi.h"
#include "adc_drvapi.h"
#include "upm_api.h"
#include "analog_drvapi.h"
#include "mmu_drvapi.h"
#if !defined(PLATFORM_UWS6121E)
#include "udc_api.h"
#endif
#include "usbservice_api.h"
#include "eic_hal.h"
#include "normcp_spec.h"
#include "flash_drvapi.h"
#include "lzma_hal.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
PUBLIC void UartDSlp_Init (void);
PUBLIC void gea_init (void);

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    Phase1 of chip driver intialization. It initializes criti-
//                  cal resource of chip before the refbase starts to boot, it
//                  contains global clock init and enable, pinmap config init etc.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
#ifdef uart4_debug_demo
typedef unsigned int uint32_t;
// status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx_fifo_level : 8;        // [7:0], read only
        uint32_t tx_fifo_space : 5;        // [12:8], read only
        uint32_t at_match_flag : 1;        // [13], read only
        uint32_t tx_active : 1;            // [14], read only
        uint32_t rx_active : 1;            // [15], read only
        uint32_t rx_overflow_err : 1;      // [16], read only
        uint32_t tx_overflow_err : 1;      // [17], read only
        uint32_t rx_parity_err : 1;        // [18], read only
        uint32_t rx_framing_err : 1;       // [19], read only
        uint32_t rx_break_int : 1;         // [20], read only
        uint32_t character_miscompare : 1; // [21], read only
        uint32_t auto_baud_locked : 1;     // [22], read only
        uint32_t __23_23 : 1;              // [23]
        uint32_t dcts : 1;                 // [24], read only
        uint32_t cts : 1;                  // [25], read only
        uint32_t auto_ratio_flag : 1;      // [26], read only
        uint32_t mask_tx_enable_flag : 1;  // [27], read only
        uint32_t dtr : 1;                  // [28], read only
        uint32_t __30_29 : 2;              // [30:29]
        uint32_t clk_enabled : 1;          // [31], read only
    } b;
} DEBUG_REG_UART_STATUS_T;

typedef volatile struct
{
    uint32_t ctrl;        // 0x00000000
    uint32_t status;      // 0x00000004
    uint32_t rxtx_buffer; // 0x00000008
    uint32_t irq_mask;    // 0x0000000c
    uint32_t irq_cause;   // 0x00000010
    uint32_t triggers;    // 0x00000014
    uint32_t cmd_set;     // 0x00000018
    uint32_t cmd_clr;     // 0x0000001c
    uint32_t auto_ratio;  // 0x00000020
    uint32_t xon;         // 0x00000024
    uint32_t xoff;        // 0x00000028
} DEBUG_HWP_UART_T;

#define DEBUG_REG_UART4_BASE (0x04400000)
#define DEBUG_REG_UART0_BASE (0x51700000)

#define DEBUG_hwp_uart4 ((DEBUG_HWP_UART_T *)(DEBUG_REG_UART4_BASE))
#define DEBUG_hwp_uart0 ((DEBUG_HWP_UART_T *)(DEBUG_REG_UART0_BASE))

//extern void Debug_Uart4_puts(char *Str);

void Debug_Uart4_puts(char *Str)
{
	int i=0;
	DEBUG_REG_UART_STATUS_T status;

	while(Str[i]!=0)
	{
		status.v = DEBUG_hwp_uart4->status;
		do{
			status.v = DEBUG_hwp_uart4->status;
		}while(status.b.tx_fifo_space == 0);
		DEBUG_hwp_uart4->rxtx_buffer = Str[i];
		i++;
	}
	DEBUG_hwp_uart4->rxtx_buffer = '\r';
}

void Debug_Uart0_puts(char *Str)
{
	int i=0;
	DEBUG_REG_UART_STATUS_T status;

	while(Str[i]!=0)
	{
		status.v = DEBUG_hwp_uart0->status;
		do{
			status.v = DEBUG_hwp_uart0->status;
		}while(status.b.tx_fifo_space == 0);
		DEBUG_hwp_uart0->rxtx_buffer = Str[i];
		i++;
	}
	DEBUG_hwp_uart0->rxtx_buffer = '\r';
}


void Debug_Uart4_putsly1(char *Str,uint32 len)
{
	int i=0;
    int size=len;
	DEBUG_REG_UART_STATUS_T status;

	while(size)
	{
		status.v = DEBUG_hwp_uart4->status;
		do{
			status.v = DEBUG_hwp_uart4->status;
		}while(status.b.tx_fifo_space == 0);
		DEBUG_hwp_uart4->rxtx_buffer = Str[i];
        i++;
		size--;
	}
}

void Debug_Uart0_putsly(char *Str,uint32 len)
{
	int i=0;
	DEBUG_REG_UART_STATUS_T status;

	while(len)
	{
		status.v = DEBUG_hwp_uart0->status;
		do{
			status.v = DEBUG_hwp_uart0->status;
		}while(status.b.tx_fifo_space == 0);
		DEBUG_hwp_uart0->rxtx_buffer = Str[i];
        i++;
		len--;
	}
}

PUBLIC uint32 SCI_TraceLowly(const char * x_format, ...)
{
    va_list     args;
    int         nBuf;
    char        format_str[500] = {0};
    const  uint16  buf_len=500-1;

    SCI_DisableIRQ();

    va_start(args, x_format);

    nBuf = _vsnprintf(format_str, buf_len,x_format, args);
    va_end(args);

    SCI_RestoreIRQ();

    Debug_Uart0_puts(format_str);
    Debug_Uart0_puts('\n');
    Debug_Uart0_puts('\r');
    return nBuf;

}

#else
void Debug_Uart4_puts(char *Str)
{
}

PUBLIC uint32 SCI_TraceLowly(const char * x_format, ...)
{
    return 0;
}

#endif

char Debug_Test1 = 0;

PUBLIC void CHIPDRV_InitOnPhase1 (void)
{
	//Debug_Test1 = 2;
	//while(Debug_Test1 == 2);
    Debug_Uart4_puts("Enter CHIPDRV_InitOnPhase1");
    MMU_Init_Phase1();

    //INT_PHY_Init();

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#ifndef NOR_BOOTLOADER_SUPPORT
	//CHIP_LDOSFPDEnAfterRst();
#endif
#endif

#if defined(PLATFORM_UWS6121E)
    /*the sysmail register not default to 0 when power on, so set to 0 manually*/
	Debug_Uart4_puts("Enter CHIP_SYSMAIL_Reg_Init");
    CHIP_SYSMAIL_Reg_Init();
#endif

#ifdef FLASH_TYPE_SPI
	Debug_Uart4_puts("Enter FLASH_SystemInit");
    FLASH_SystemInit();
 #endif
    // Get current chip type
    Debug_Uart4_puts("Enter CHIP_InitHWChipID");
    CHIP_InitHWChipID();

    //Init chip
    Debug_Uart4_puts("Enter CHIP_Init");
    CHIP_Init ();

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    if (! (CHIP_DetectMemType()))
    {
        // Init flash device
        EMC_Init();
    }
#endif

    //LZMA_HAL_Init();
    Debug_Uart4_puts("End CHIPDRV_InitOnPhase1");
}

/*****************************************************************************/
// Description :    Phase2 of chip driver intialization. It initializes driver
//                  modules before refbase confirms powering on. it contains
//                  some necessary modules including RTC, timer, DMA, NLC etc.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase2 (void)
{
    TIMER_ID_E id;
     Debug_Uart4_puts("Enter CHIPDRV_InitOnPhase2\n");
    // Init interrupt controller
    ISR_Init ();

    LDO_Init();    /*ï¿½ï¿½ï¿½ï¿½ï¿½Î£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½*/
    PMU_Init();
    SLEEP_Init();

    //Init clock system
    CLOCK_System_Init();

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#if defined(LDO_CALIBRATION_SUPPORT)
	// ADC parameter init

    ADC_Init();
#endif

	//ldo calibration
	//LDO_Calibration();

	//vdddcxo rise to 1.95v
	//LDO_SetVoltValue(LDO_LDO_DCXO, 1950);
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
/* IB Calibration is used for LCD&KPD LED, LCD&KPD are not used in modem platform */
#if !defined(FPGA_VERIFICATION) && !defined(MODEM_PLATFORM)
    //IB calibration
    //IB_Calibration();
#endif
#endif

    // Init RTC.
   RTC_Init ();

    // Init Timer
#if defined(PLATFORM_UWS6121E)
    //for (id=TIMER_3; id<TIMER_4; id++)
    id = OS_TIMER;
#else
    for (id=TIMER_0; id<TIMER_MAX; id++)
#endif
    {
        TIMER_HAL_Open (id);
    }

#if defined(PLATFORM_UWS6121E)
    TIMER_HAL_IOctl (OS_TIMER, TIMER_IOCTL_CMD_START, NULL);
#else
    TIMER_HAL_IOctl (TIMER_1, TIMER_IOCTL_CMD_START, NULL);
#endif
    // Init DMA
    DMA_HAL_Init();

    SLEEP_InitSoft();
#ifdef  NANDBOOT_SUPPORT
    // Nandflash controller Init
    NANDCTL_Init (CHIP_GetAhbClk());
#else //Nor Platform

#ifndef FPGA_VERIFICATION
#ifdef PLATFORM_SC6600L
    EMC_SetARMAHBClock (80);
#elif defined(PLATFORM_SC6800H)
    if(SC6800H == CHIP_GetChipType())
    {
        EMC_SetARMAHBClock (200);
    }
    else
    {
        EMC_SetARMAHBClock (180);
    }
#endif
#endif
#endif //NANDBOOT_SUPPORT
#ifndef NANDBOOT_SUPPORT
    //Init MMU
    MMU_Init ();
#endif

#ifdef PLATFORM_SC6530
#ifndef FPGA_VERIFICATION
	EIC_HAL_Init();
#endif
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#ifndef FPGA_VERIFICATION
	//EIC_HAL_Init();
#endif
#endif

#if defined(PLATFORM_UWS6121E) && defined(SECURE_BOOT_SUPPORT)

    SecureMode_WCNuartEnable();
#endif
	Debug_Uart4_puts("END CHIPDRV_InitOnPhase2\n");
}

/*****************************************************************************/
// Description :    Phase3 of chip driver intialization. It initializes driver
//                  modules after refbase confirms powering on. it contains
//                  the modules including GPIO, watchdog, analog, charge etc.
// Author :         Jeff.Li
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase3 (void)
{
//#ifndef FPGA_VERIFICATION
    // Initialize GPIO driver.
    Debug_Uart4_puts("Enter CHIPDRV_InitOnPhase3_enter\n");
    GPIO_Init();
//#endif
#if defined(PLATFORM_UWS6121E)
{
#ifndef CHIP_ECO_SUPPORT
    uint32 temp = ADI_Analogdie_reg_read(0xc08);
    temp |= (1<<2)|(1<<10);
    ADI_Analogdie_reg_write (0xc08, temp);
    ADI_Analogdie_reg_write (0x900, 0x454e);
    ADI_Analogdie_reg_write (0x948, 0x1);      /*vgenÉÏÓÐÂ©µçµ¼ÖÂ32K error£¬ÁÙÊ±°æ±¾È¥µô*/
    ADI_Analogdie_reg_write (0xe6c, 0x2ff);  /*vmem do not reset*/
    ADI_Analogdie_reg_write (0xe68, 0x80);
#endif
}
#endif
    // Init and  start the watch dog.
    WatchDogTimerInit();
    // Init the GEA
    gea_init();
    // uart deepsleep wakeup func init
    UartDSlp_Init();
#if !defined(PLATFORM_SC6531EFM) && !defined(LDO_CALIBRATION_SUPPORT) && !defined(PLATFORM_UWS6121E)
	// ADC parameter init

    ADC_Init();
#endif
    //Init vbus thread
    USB_InitBottomHalf();
#if defined(PLATFORM_UWS6121E)
    drvUsbInit();
#else
    //Init usb device controller and ISR
    UDC_Init();
#endif
    //Init main thread
    USB_InitTopHalf();
    // Init chng freq
    CHNG_FREQ_Init();
#if !defined(FPGA_VERIFICATION) && !defined(MODEM_PLATFORM)
    // It depends on GPIO, so must be called after GPIO has been initilized
    CHGMNG_Init();   //ljj stub0806
#endif
    thermal_init();
#ifndef FPGA_VERIFICATION
    // Init Analog device
    ANA_Init();
#endif
    hal_PagespyInit();   //pagespy
    Debug_Uart4_puts("Enter CHIPDRV_InitOnPhase3_end\n");
}

/*****************************************************************************/
// Description :    Phase4 of chip driver intialization. It initializes MMU
//                  after refbase has copied the kernel image.
// Author :         Jeff.Li
// Note :           Before phase4, the MMU and virtual memory mapping is still
//                  not initialized, so the kernel image coping of refbase sho-
//                  uld make sure that the operating address is physical.
/*****************************************************************************/
PUBLIC void CHIPDRV_InitOnPhase4 (void)
{

#ifdef NANDBOOT_SUPPORT
    //Init MMU
    MMU_Init ();
#endif

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
