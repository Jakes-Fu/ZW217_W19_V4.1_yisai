#ifndef _RDA_PLAT_SLEEP_H_
#define _RDA_PLAT_SLEEP_H_

//#include "cp_idle_asm_arm.h"
//#include "sys_ctrl_asm_arm.h"
#include "../../chip_plf/uix8910/cp_idle.h"


#define CONFIG_WAKEUP_JUMP_ADDR					0x0010c100
#define CONFIG_PRIMARY_RESUME_JUMP_MAGIC		0xD8E1A000

#define CONFIG_PRIMARY_WAKEUP_JUMP_MAGIC		0xD8E5BEAF 
#define CONFIG_SMP_WAKEUP_JUMP_MAGIC        	0xC8DACAFE

#define CONFIG_WAKEUP_CODE_ADDR         		0x0010c120
#define CONFIG_WAKEUP_AON_SRAM_OFF 				0xb800

#define DDR_DMC_PHY_SAVE_MAGIC 					0x81357951
#define DDR_DMC_PHY_SAVE_END 					0x70753159
#define AP_AON_SRAM_MAX 						0xc000
#define AP_AON_SRAM_SAVE_REGISTER_NUM 			180
#define CPU_POWER_OFF_FLAG 						0x49444c45

#define AP_KERNEL_WAKEUP_CONTINUE 				0xd3
#define AP_KERNEL_WAKEUP_SUSPEND 				0xce

#define RDA_CP_SRAM_SIZE 						0x30000
#define RDA_CP_SRAM_START 						0x10000

#define IDL_32K_REF_20MS 						640
#define IDL_REF_32K_MASK                        0xFFFFFFFF
#define LPS_APSRAM_SIZE							0x4000
#define LPS_APSRAM_BASE							0x800000

#define LPS_32KNUM_MASK                         0xFFFFFFFF
//#define LPS_32KNUM_MASK_2S					    0xFFFF0000    // 2s
#define LPS_32KNUM_MASK_2S					    0x10000    // 2s
#define LPS_32KNUM_MASK_8S					    0x40000//0xFFFC0000    // 8s

#define CP_IDLE_DSIPLL           				(1<<6)
/* the registers used must be same with file "rda_suspend_resume_cfg.h",
 * or system will be currupted!
 */
#ifdef CHIP_VER_UIX8910MPW
#define PRIMCPU_WAKEUP_MAGIC_ADDR 				*(volatile unsigned long *)(0x501001B8)
#define PRIMCPU_WAKEUP_JUMP_ADDR 				*(volatile unsigned long *)(0x501001BC)
#define PRIMCPU_RESUME_MAGIC_ADDR 				*(volatile unsigned long *)(0x501001EC)
#define PRIMCPU_RESUME_JUMP_ADDR 				*(volatile unsigned long *)(0x501001F0)
#define HAL_GET_REF32K                          (hwp_idle->IDL_32K_REF)


#else
#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E) 
#define SPI_FLASH_CONFIG                        (*(volatile uint32 *)(0x880014))
#define SPI_FLASH2_CONFIG                        (*(volatile uint32 *)(0x881014))
#define PRIMCPU_WAKEUP_MAGIC_ADDR 				(hwp_idle->IDLE_RES0)
#define PRIMCPU_WAKEUP_JUMP_ADDR 				(hwp_idle->IDLE_RES1)
#define LPS_CP2AP_IPCCOMM_ADDR 					(0x501001F8)
#define PRIMCPU_RESUME_MAGIC_ADDR 				(hwp_idle->IDLE_RES10)
#define PRIMCPU_RESUME_JUMP_ADDR 				(hwp_idle->IDLE_RES11)
#define HAL_GET_REF32K                          (hwp_idle->IDL_32K_REF)
#define LPS_CPSYSSLP_CTRL             			*(volatile unsigned long *)(0x50100214)  //CFG16

#else


//#error "error machine macro!\n"
#endif
#endif

#define AXIBUSFREQ_200MHZ                       (200000000)
#define AXIBUSFREQ_400MHZ                       (400000000)

//  lowpower mailbox 66~86
#define LPS_MLBOX_POINT0                        (hwp_mailbox->SYSMAIL66)
#define LPS_MLBOX_POINT1                        (hwp_mailbox->SYSMAIL67)
#define LPS_MLBOX_POINT2                        (hwp_mailbox->SYSMAIL68)
#define LPS_MLBOX_POINT3                        (hwp_mailbox->SYSMAIL69)
#define LPS_MLBOX_POINT4                        (hwp_mailbox->SYSMAIL70)
#define LPS_MLBOX_POINT5                        (hwp_mailbox->SYSMAIL71)
#define LPS_MLBOX_POINT6                        (hwp_mailbox->SYSMAIL72)
#define LPS_MLBOX_POINT7                        (hwp_mailbox->SYSMAIL73)
#define LPS_MLBOX_POINT8                        (hwp_mailbox->SYSMAIL74)
#define LPS_MLBOX_POINT9                        (hwp_mailbox->SYSMAIL75)
#define LPS_MLBOX_POINT10                       (hwp_mailbox->SYSMAIL76)
#define LPS_MLBOX_POINT11                       (hwp_mailbox->SYSMAIL77)

typedef struct{
	uint32 osTimerRun;
	uint32 osTimerLastTick;
	uint32 osTimerResidue;
	uint32 osTimerSlp;
	uint8  osTimerAdFlag;
	uint32 osTimerWkUp;
	uint32 osTimerWkEn;
	uint32 ApSlpEnry32K;
	uint32 ApSlpWk32K;
	uint32 CpUnlock32K;
	uint32 Osw2ChgSet;
	uint32 osTimerChg;
	uint32 wfiOut32K;
	uint32 wfiTotal;
	uint32 wfiFlag;
	uint32 ApRunPoint1;
	uint32 ApRunPoint2;
	uint32 ApRunPoint3;
	uint32 ApRunPoint4;
	uint32 DoffFlow1;
	uint32 DoffFlow2;
	uint32 DoffFlow3;
	uint32 DoffFlow4;
	uint32 DoffFlow5;
	uint32 DoffFlow6;
	
}LPS_SLP_INFO;

#endif /* _RDA_PLAT_SLEEP_H_ */
