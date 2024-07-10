/******************************************************************************
 ** File Name:      sleep_phy_nix8910.c                                       *
 ** Author:         bo.tang                                                   *
 ** DATE:           07/23/2018                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DSP and MCU for time compensate of the bug for DSP.       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/23/2018     bo.tang     		Create.                                   *

******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *

**---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "../sleep_phy.h"
#include "sleep_phy_sc6531efm.h"
#include "task_monitor.h"
#include "pin_sleep.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "fiq_drvapi.h"
#include "dal_audio.h"
#include "ldo_drvapi.h"
#include "../../tpc/r4p0/tpc_reg_r4p0.h"
#include "dmc400_asm_arm.h"
#include "cp_pwrctrl_asm_arm.h"
#include "cp_sysreg_asm_arm.h"
#include "sleep.h"
#include "../../../chip_module/chip/uix8910/cp_clkrst.h"
#include "../../sys_mail/uix8910/mailbox_reg_uix8910.h"
#include "sio.h"
/*lint -esym(551,test_sleep_count)*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *

**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*
**                            Macro Definitions                               *
**---------------------------------------------------------------------------*/
LOCAL uint32 reg_apb_eb0_val=0;
LOCAL uint32 reg_apb_peri_ctl0_ctl=0;
LOCAL uint32 reg_ahb_eb0_val = 0;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                                                                    *
 **---------------------------------------------------------------------------*/
PUBLIC uint32  RF_enable_flag				= 0;
//娣诲姞鏍囧織 
//鎸囩ず鏄疌P鐫＄湢鏃堕棿澶煭涓嶈繘鍏ユ繁鐫＄湢杩樻槸CP鐪熷疄鏈潯鐪犱笉杩涘叆娣辩潯娴佺▼
PUBLIC uint32  CP_sleep_flag				= 0;
// Status flag indicate whether MCU is in sleep or not
PUBLIC uint32  MCU_is_sleep					= 0x0;
PUBLIC uint32  MCU_is_wakeup				= 0x0;
PUBLIC uint32  MCU_is_waiting_tick			= 0x0;
PUBLIC uint32  s_dma_sleep_mode				= 0x0;
PUBLIC uint32  DDR_PARAM_SAVE_ADDR;  
PUBLIC uint32  LpsRef32kNum;
PUBLIC volatile uint32  LpsDomainSt    	    = 0x0;
PUBLIC uint32  cpuWfi32kNum                 = 0x0;
PUBLIC uint32  enterWfi32K;
PUBLIC BOOLEAN g_LdoEmmcPwrOffFlag 			= FALSE;
PUBLIC uint32  axiBusFreq                   = AXIBUSFREQ_400MHZ;

LOCAL BOOLEAN  is_deepsleep      			= FALSE;
LOCAL BOOLEAN  s_PERI_powerdomain_en		= FALSE;
LOCAL BOOLEAN  s_CM4_powerdomain_en			= FALSE;

//BT Sleep debug info. It will be deleted after bug fix.
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
uint8 s_bt_sleep_enter_time_1_index = 0;
uint32 s_bt_sleep_enter_time_1[10] = {0};
uint8 s_bt_sleep_enter_time_2_index = 0;
uint32 s_bt_sleep_enter_time_2[10] = {0};
uint8 s_bt_sleep_enter_time_3_index = 0;
uint32 s_bt_sleep_enter_time_3[10] = {0};
uint8 s_bt_sleep_enter_time_4_index = 0;
uint32 s_bt_sleep_enter_time_4[10] = {0};
uint32 s_bt_current_ap_32k=0;
uint8 s_bt_not_wakeup_error_times = 0;
#endif
//BT Sleep debug info. It will be deleted after bug fix.

#if 0 //def ARM_CORE_POWER_DOMAIN
#pragma arm section zidata = "ARMCORE_STACK"
#define ARMCORE_STACK_LIMIT  0x30
uint32 armcore_stack_space[ARMCORE_STACK_LIMIT];
#pragma arm section zidata
#endif

#define DDR_PARAM_ARRAY_LEN 120
//LOCAL const struct ddr_save_pattern ddr_param[DDR_PARAM_ARRAY_LEN];
//PUBLIC volatile struct ddr_save_pattern ddr_param[DDR_PARAM_ARRAY_LEN];
PUBLIC volatile LPS_SLP_INFO sHalSlpInfo;
PUBLIC void *sRamRo = NULL;
//PUBLIC void *sRamRw = NULL;

extern uint32 g_bt_sleep_time;
extern uint32 g_bt_sleep_time_ap32k;

#define SLEEP_DEBUG

#ifdef SLEEP_DEBUG
PUBLIC uint32 s_sleep_step = 0;
#endif
/**---------------------------------------------------------------------------*
 **                     Global Function Prototypes                            *
 **---------------------------------------------------------------------------*/
extern void     SCI_BeforeEnterDeepSleep (void);
extern void     SCI_BeforeEnterHalfDeepSleep (void);
extern void     SCI_AfterExitDeepSleep (void);

extern BOOLEAN RM_GetEnableArmSleepFlag (void);
extern int RM_SetEnableArmSleepFlag (int flg);

extern void 	UNI_WAKEUP_ENTRY(void);
extern void  __FPU_Enable(void);
extern void ipc_set_bt_rf_sm_addr(void);
//extern PUBLIC uint32 CHGMNG_GetChargeTimerInfo(void);
//extern uint32 DMA_HAL_Memcpy (void *des, void *src, uint32 count);

#define LPS_WKUP_PRIMARY_MAGIC			0xD8E5BEAF
#define LPS_WKUP_PRIMARY_MAGIC_ADDR		0x501001B8
#define LPS_WKUP_PRIMARY_JUMP_ADDR		0x501001BC
#define LPS_CP2AP_IPCCOMM_ADDR			0x501001F8

#define LPS_AP_RESUME_MAGIC             0xD8E1A000
#define LPS_AP_RESUME_MAGIC_ADDR        0x501001EC
#define LPS_AP_RESUME_JUMP_ADDR         0x501001F0

#define IDL_REF_32K  *(volatile unsigned long *)(0x50090088)
#define CFG_01_TEST  *(volatile unsigned long *)(0x501001D8)

/*----------------------------------------------------------------------------*
**                         Function Definitions                                                                                *
**--------------------------------------------------------------------------- */

PUBLIC void DSLP_SetEnableSleepFlag (void)
{
    if (RM_GetEnableArmSleepFlag())
    {
        RM_SetEnableArmSleepFlag (SCI_TRUE);
    }
    else
    {
        RM_SetEnableArmSleepFlag (SCI_FALSE);
    }
}

PUBLIC void SLEEP_PHY_PCM_EnableSleep (uint32 mode)
{
    if(ENABLE_DEEP_SLEEP == mode){
        sleep_active_modules 	&= ~SLEEP_DRV_PCM;
        sleep_apb_active_modues &= ~SLEEP_APB_IIS;
    }else{
        sleep_active_modules 	|= SLEEP_DRV_PCM;
        sleep_apb_active_modues |= SLEEP_APB_IIS;
    }
}

PUBLIC void SLEEP_LPS_EnableDeepSleep (void)
{
	uint32 cur32Num   = hwp_idle->IDL_32K_REF;
	uint32 WkUp32Knum = hwp_idle->IDL_M_TIMER;
	uint32 slpnum = 0;
#if 0
	sleep_active_modules |= SLEEP_DRV_LPSTIMER;
	if(hwp_idle->IDL_CTRL_TIMER == 1){
		slpnum = (uint32)(((uint64)WkUp32Knum + LPS_32KNUM_MASK - 
					        (uint64)cur32Num)%LPS_32KNUM_MASK);
		if(slpnum > 655){
			sleep_active_modules &= (~SLEEP_DRV_LPSTIMER);
		}
		
	}
#else
	if ((hwp_idle->IDL_CTRL_TIMER&0x1) == 1)
	{
		slpnum = (uint32)(((uint64)WkUp32Knum + LPS_32KNUM_MASK - 
					        (uint64)cur32Num)%LPS_32KNUM_MASK);
		if(slpnum < 655){
			sleep_active_modules |= SLEEP_DRV_CPCPU;
			CP_sleep_flag = 1;
		}
	}
#endif
	
}
PUBLIC void SLEEP_PHY_SIM_EnableSleep (uint32 mode)
{
    if (mode)
    {
        sleep_active_modules 	&= ~SLEEP_DRV_SIM;
        sleep_apb_active_modues &= ~SLEEP_APB_SIM;
    }
    else
    {
        sleep_active_modules 	|= SLEEP_DRV_SIM;
        sleep_apb_active_modues |= SLEEP_APB_SIM;
    }
}

PUBLIC void SLEEP_PHY_TPC_EnableSleep (uint32 mode)
{
    if (mode)
    {
        sleep_apb_active_modues &= ~SLEEP_APB_TPC;
    }
    else
    {
        sleep_apb_active_modues |= SLEEP_APB_TPC;
    }
}


PUBLIC BOOLEAN SLEEP_PERI_PowerDomain_Set(BOOLEAN is_enable)
{
    s_PERI_powerdomain_en = is_enable;
    return SCI_TRUE;
}

PUBLIC BOOLEAN SLEEP_PERI_PowerDomain_Get(void)
{
    return s_PERI_powerdomain_en;
}

PUBLIC BOOLEAN SLEEP_CM4_PowerDomain_Set(BOOLEAN is_enable)
{
    s_CM4_powerdomain_en = is_enable;
    return SCI_TRUE;
}

PUBLIC BOOLEAN SLEEP_CM4_PowerDomain_Get(void)
{
    return s_CM4_powerdomain_en;
}


PUBLIC void SLEEP_LpsGetCpUnlock32K(void)
{
	sHalSlpInfo.CpUnlock32K = HAL_GET_REF32K;
}

LOCAL void SLEEP_WCNOSC_EnableDeepSleep(void)
{
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
    uint32 curr_32k_tick = 0;

    if (BT_IsEnabled())
    {
        sleep_active_modules |= SLEEP_DRV_WCN_OSC;

        curr_32k_tick = hwp_idle->IDL_32K_REF;//当前的AP侧32K计数//这样避免在查询到(hwp_pwrctrl->WCN_LPS & CP_PWRCTRL_WCN2SYS_OSC_EN) == 0后再获取32K，有可能32K刚好比设定的睡眠时间长度       //g_bt_sleep_time大1个32K的极限场景。
        s_bt_current_ap_32k = curr_32k_tick;

        if ((0 != g_bt_sleep_time) 
    		&& (0 == (hwp_pwrctrl->WCN_LPS & CP_PWRCTRL_WCN2SYS_OSC_EN)) 
    		&& (0 == (sleep_active_modules & SLEEP_DRV_BT)))
        {
            curr_32k_tick = (uint32)(((uint64)curr_32k_tick + IDL_REF_32K_MASK - (uint64)g_bt_sleep_time_ap32k)%IDL_REF_32K_MASK);//求出已经过去的时间

            if (g_bt_sleep_time > curr_32k_tick)
            {
                if ((g_bt_sleep_time - curr_32k_tick) > 230)// 230 32K_ticks means about 6.8ms //检查是否小于6.8ms,小于6.9ms则AP不睡了
                {
                    sleep_active_modules &= (~SLEEP_DRV_WCN_OSC);
    
                    //BT Sleep debug info. It will be deleted after bug fix.
                    s_bt_sleep_enter_time_1[s_bt_sleep_enter_time_1_index] = s_bt_current_ap_32k;
                    s_bt_sleep_enter_time_1_index++;
                    if (s_bt_sleep_enter_time_1_index > 9)
                    {
                        s_bt_sleep_enter_time_1_index = 0;
                    }
                    //BT Sleep debug info. It will be deleted after bug fix.
                }
               else
               {
                    //BT Sleep debug info. It will be deleted after bug fix.
                    s_bt_sleep_enter_time_2[s_bt_sleep_enter_time_2_index] = s_bt_current_ap_32k;
                    s_bt_sleep_enter_time_2_index++;
                    if (s_bt_sleep_enter_time_2_index > 9)
                    {
                        s_bt_sleep_enter_time_2_index = 0;
                    }
                    //BT Sleep debug info. It will be deleted after bug fix.
                }
            }
            else
            {
                s_bt_sleep_enter_time_3[s_bt_sleep_enter_time_3_index] = s_bt_current_ap_32k;
                s_bt_sleep_enter_time_3_index++;
                if (s_bt_sleep_enter_time_3_index > 9)
                {
                    s_bt_sleep_enter_time_3_index = 0;
                }
            }
        }
        else//BT Sleep debug info. It will be deleted after bug fix.
        {
            s_bt_sleep_enter_time_4[s_bt_sleep_enter_time_4_index] = curr_32k_tick;
            s_bt_sleep_enter_time_4_index++;
            if (s_bt_sleep_enter_time_4_index > 9)
            {
                s_bt_sleep_enter_time_4_index = 0;
            }

            if ((0 != g_bt_sleep_time) && (0 == (sleep_active_modules & SLEEP_DRV_BT)))
            {
                curr_32k_tick = (uint32)(((uint64)curr_32k_tick+IDL_REF_32K_MASK-(uint64)g_bt_sleep_time_ap32k)%IDL_REF_32K_MASK);//求出已经过去的时间
                if ((g_bt_sleep_time+45) < curr_32k_tick)//g_bt_sleep_time must be less than 'curr_32k_tick'
                {
                    s_bt_not_wakeup_error_times++;
#if 0
                    SCI_ASSERT(0);
#endif
                }
            }

        }//debug info
    }
    else
    {
        sleep_active_modules &= (~SLEEP_DRV_WCN_OSC);//double check
    }
#endif
}

/*****************************************************************************
 *  Description:    this function is used to disable any module if they are  *
 *                  not used now                                             *
 *  Global Resource Dependence:                                              *
 *  Author: jiexia.yu                                                        *
 *  Note:   return SCI_TRUE when module was disabled, else return SCI_FALSE  *
******************************************************************************/
LOCAL BOOLEAN SLEEP_DisableAudioModule (void)
{
    BOOLEAN is_disabled = SCI_FALSE;
#if 0
    if (! (sleep_active_modules & SLEEP_DRV_AUDIO))
    {
        //turn off VB clock gate & cut off data path
        CHIP_REG_AND (APB_PERI_CTL0,
                      ~ (ARM_VB_MCLKON|ARM_VB_DA0ON|ARM_VB_DA1ON|ARM_VB_ADC0ON|ARM_VB_ADC1ON|
                         ARM_VB_ANAON));
        //ensure VB controlled by ARM
        CHIP_REG_OR (APB_PERI_CTL0, ARM_VB_ACC);

        is_disabled = SCI_TRUE;
    }
#endif
    return is_disabled;
}

#define LPS_CP2AP_IPCCOMM			*(volatile unsigned long *)(0x501001F8)

LOCAL void SLEEP_ReleaseCP(void)
{
	cpuWfi32kNum = hwp_idle->IDL_32K_REF;
	if(0x0 == hwp_sysCtrl->CPCPU_RST_CLR){
		LPS_MLBOX_POINT1 = hwp_idle->IDL_32K_REF;
		hwp_sysCtrl->CPCPU_RST_CLR = 0xF;
		while(LPS_CP2AP_IPCCOMM != 0xDA);
	}else{
		SCI_ASSERT (0);
	}
}
/*******************************************************************************
FUNCTION                                                              
  SLEEP_DisPllDisable                                             
DESCRIPTION                                                           
  close dsipll clock.
CALLED BY                                                            
  lowpower init.                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
LOCAL void SLEEP_DisPllDisable(void)
{
	hwp_idle->IDLE_CG_SW_SET  = CP_IDLE_DSIPLL;
	hwp_idle->IDLE_CG_SEL_SET = CP_IDLE_DSIPLL;
	hwp_idle->PD_PLL_SW_SET   = CP_IDLE_DSIPLL;
	hwp_idle->PD_PLL_SEL_SET  = CP_IDLE_DSIPLL;
	
}


/*******************************************************************************
FUNCTION                                                              
  SLEEP_OsTimerStop                                             
DESCRIPTION                                                           
  before enter deepsleep,stop OsTimer.
CALLED BY                                                            
  deepsleep process.                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
LOCAL void SLEEP_OsTimerCheck(void)
{
	uint32 CurVal = 0;
	
	if(OS_TimTickEnable()){
		while(!OS_TimTickIsEnabled());
		CurVal = OS_TimCurValue();    
		if((CurVal > 0xFFFFFF) || (CurVal < 328)){
			sleep_apb_active_modues |= SLEEP_APB_OSTIMER;
			return;
		}
	}
	sleep_apb_active_modues &= (~SLEEP_APB_OSTIMER);
}

/*******************************************************************************
FUNCTION                                                              
  SLEEP_OsTimerStart                                             
DESCRIPTION                                                           
  after deepsleep wkup,restore OsTimer.
CALLED BY                                                            
  deepsleep process.                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
LOCAL void SLEEP_OsTimerStart(void)
{
	uint32 osTicks;

	if(sHalSlpInfo.osTimerAdFlag){	
		sHalSlpInfo.osTimerWkUp = hwp_idle->IDL_32K_REF;
		osTicks = (uint32)(((uint64)sHalSlpInfo.osTimerWkUp + IDL_REF_32K_MASK 
			           - (uint64)sHalSlpInfo.osTimerSlp)%IDL_REF_32K_MASK);
		osTicks = (osTicks >> 1) + sHalSlpInfo.osTimerRun;
		SCI_AdjustTimer(osTicks);//osi_adjust_timer(osTicks);
		sHalSlpInfo.osTimerWkEn = *(volatile unsigned long *)(0x08809000);		
		SCI_TraceLow("LPS OsTimer Adjust Wk32K=0x%x,OsRun:0x%x,Reside:0x%x,LastTick:0x%x,Flag:0x%x,En:0x%x,osTicks:0x%x)", 
			sHalSlpInfo.osTimerWkUp, 
			sHalSlpInfo.osTimerRun,
			sHalSlpInfo.osTimerResidue,
			sHalSlpInfo.osTimerLastTick,
			sHalSlpInfo.osTimerAdFlag,
			sHalSlpInfo.osTimerWkEn,			
			osTicks);
		sHalSlpInfo.osTimerAdFlag = FALSE; 
		
	}
}
/*******************************************************************************
FUNCTION                                                              
  SLEEP_OsTimerStop                                             
DESCRIPTION                                                           
  before enter deepsleep,stop OsTimer.
CALLED BY                                                            
  deepsleep process.                                                  
NAME           TYPE/REMARK 

*******************************************************************************/
LOCAL void SLEEP_OsTimerStop(void)
{
	uint32 CurVal   = 0;
	uint32 LastTick = 0;
	
	if(OS_TimTickEnable()){
		while(!OS_TimTickIsEnabled());
		CurVal  = OS_TimCurValue();
		LastTick= SCI_GetLastTick();
		sHalSlpInfo.osTimerRun      = LastTick - CurVal;
		sHalSlpInfo.osTimerResidue  = CurVal;
		sHalSlpInfo.osTimerLastTick = LastTick;
		OS_TimTickStop();
		sHalSlpInfo.osTimerSlp    = hwp_idle->IDL_32K_REF;
		if(sHalSlpInfo.osTimerAdFlag == TRUE){
			SCI_ASSERT (0);
		}
		sHalSlpInfo.osTimerAdFlag = TRUE;
		
	}
}


/*******************************************************************************
FUNCTION                                                              
  SLEEP_LpsSramSave                                             
DESCRIPTION                                                           
  save sram ro code and rw data.
CALLED BY                                                            
  arm domain power off sleep process.                                                  
NAME           TYPE/REMARK 
*******************************************************************************/
LOCAL void SLEEP_LpsSramSave(void)
{
	//uint32 SramDur = 0;
	//DMA_HAL_Memcpy ((uint32 *)sRamRo, (uint32 *)LPS_APSRAM_BASE,  LPS_APSRAM_SIZE);
	memcpy((uint8 *)sRamRo, (uint8 *)LPS_APSRAM_BASE,  LPS_APSRAM_SIZE);
#if 0
	//SramDur = HAL_GET_REF32K_CNT;
	if((Type == RO_SRAM) && (gSramRoSave == 0xAABB)){
	    hal_ArmDmaSynCommonTran((u32)&Image$$SYSRAM_RO_REGION$$Base,
	                           (u32)sRamRo,(u32)&Image$$SYSRAM_RO_REGION$$Length);
		gSramRoSave = 0x0;
		//sHalSlpInfo.sRamRoSave = HAL_GET_REF32K_CNT - SramDur;
	}else if(Type == RW_SRAM){
		hal_ArmDmaSynCommonTran((u32)&Image$$SYSRAM_RW_REGION$$Base,(u32)sRamRw,
		    (u32)(SYSTEM_RAM_SIZE-((u32)&Image$$SYSRAM_RW_REGION$$Base-SYSTEM_RAM_BASE)));
		//sHalSlpInfo.sRamRwSave = HAL_GET_REF32K_CNT - SramDur;
	}
#endif
}

/*******************************************************************************
FUNCTION                                                              
  hal_LpsSramRestore                                             
DESCRIPTION                                                           
  .
CALLED BY                                                            
  .                                                  
NAME           TYPE/REMARK 
*******************************************************************************/
LOCAL void SLEEP_LpsSramRestore(void)
{
	//u32 SramDur = 0;

	//SramDur = HAL_GET_REF32K_CNT;
	//DMA_HAL_Memcpy ((uint32 *)LPS_APSRAM_BASE, (uint32 *)sRamRo, LPS_APSRAM_SIZE);
	memcpy((uint8 *)LPS_APSRAM_BASE,(uint8 *)sRamRo,LPS_APSRAM_SIZE);
#if 0
    hal_ArmDmaSynCommonTran((u32)sRamRo,
        (u32)&Image$$SYSRAM_RO_REGION$$Base,(u32)&Image$$SYSRAM_RO_REGION$$Length);
	hal_ArmDmaSynCommonTran((u32)sRamRw,(u32)&Image$$SYSRAM_RW_REGION$$Base,
		(u32)(SYSTEM_RAM_SIZE-((u32)&Image$$SYSRAM_RW_REGION$$Base-SYSTEM_RAM_BASE)));
	
	//sHalSlpInfo.sRamRestore = HAL_GET_REF32K_CNT - SramDur;
	//HW_POW_TRACE01("LPS Sram Restore(Dur32K:%d)\n",(HAL_GET_REF32K_CNT - SramDur));
#endif
}

LOCAL void SLEEP_WakeUpHandle(uint32 wksource)
{
	if(wksource & CP_IDLE_AWK2_AWK_STAT){
		hwp_idle->IDL_AWK_ST = CP_IDLE_AWK2_AWK_STAT;
		SCI_TraceLow("LPS KEY WakeUp(AwkSt:0x%x)", wksource);
	}

	if(wksource & CP_IDLE_AWK4_AWK_STAT){
		hwp_idle->IDL_AWK_ST = CP_IDLE_AWK4_AWK_STAT;
		SCI_TraceLow("LPS UART1 WakeUp(AwkSt:0x%x)", wksource);
	}

	if(wksource & CP_IDLE_AWK0_AWK_STAT){
		hwp_idle->IDL_AWK_ST = CP_IDLE_AWK0_AWK_STAT;
		SCI_TraceLow("LPS PMIC WakeUp(AwkSt:0x%x)", wksource);
	}

	if(wksource & CP_IDLE_AWK_OSW2_STAT){
		hwp_idle->IDL_AWK_ST = CP_IDLE_AWK_OSW2_STAT;
		SCI_TraceLow("LPS OSW2 WakeUp(AwkSt:0x%x,OswT(set:%dms,Dur:%d))", 
			wksource,
			sHalSlpInfo.Osw2ChgSet,
			(uint32)((float)(HAL_GET_REF32K - sHalSlpInfo.osTimerChg)/32.768));
	}
	
// prepare next sleep	
	PRIMCPU_WAKEUP_MAGIC_ADDR = CONFIG_PRIMARY_WAKEUP_JUMP_MAGIC;
	PRIMCPU_WAKEUP_JUMP_ADDR  = (uint32)UNI_WAKEUP_ENTRY;

// disable soft control auto switch 26M	
	hwp_idle->SLOW_SYS_CLK_SEL_HWEN = 1;

}


/*****************************************************************************
 *  Description:    this function is used to disable the clock to APB module *
 *                  if they are not used now                                 *
 *  Global Resource Dependence:                                              *
 *  Author: jiexia.yu                                                        *
 *  Note:   return SCI_TRUE when module was disabled, else return SCI_FALSE  *
******************************************************************************/
LOCAL BOOLEAN SLEEP_DisableAPBModule (void)
{
    BOOLEAN is_disabled = SCI_FALSE;

    if (!sleep_apb_active_modues){
        // Disable apb_force_on
        //REG32 (APB_SLP_CTL0) &= ~APB_FORCE_ON;

        // Clear apb device enable bits
        //REG32(APB_EB0_CLR) = APB_EB0_MASK;

        is_disabled = SCI_TRUE;
    }

    return is_disabled;
}




/*****************************************************************************
 *  Description:    this function is used to disable the clock to AHB module *
 *                  if they are not used now                                 *
 *  Global Resource Dependence:                                              *
 *  Author: Jeff.Li                                                          *
 *  Note:   return SCI_TRUE when module was disabled, else return SCI_FALSE  *
******************************************************************************/
LOCAL void SLEEP_DisableAHBModule (void)
{
    if (!sleep_ahb_active_modules){
        //clear ahb device enable bits
        //REG32(AHB_EB0_CLR) = AHB_EB0_MASK;  //AHB_EB0_CLR
    }
}

LOCAL void  SLEEP_HisrFunc (uint32 cnt, void *pData)
{
	
}

LOCAL void SLEEP_SwitchAxiBusFreq(uint32 AxiFreq)
{
	switch(AxiFreq)
	{
	case AXIBUSFREQ_200MHZ:
		hwp_sysCtrl->CFG_PLL_SYS_AXI_DIV  = 7;
		hwp_sysCtrl->CFG_PLL_SYS_AXI_DIV |= (1<<4);
		break;
		
	case AXIBUSFREQ_400MHZ:
		if(hwp_sysCtrl->CFG_PLL_SYS_AXI_DIV != 0xC){
			hwp_sysCtrl->CFG_PLL_SYS_AXI_DIV  = 0xC;
			hwp_sysCtrl->CFG_PLL_SYS_AXI_DIV |= (1<<4);
		}
		break;
		
	default:
		break;		
	}

}

LOCAL void SLEEP_IrqHandler (uint32 num)
{
	uint32 WkSt = hwp_idle->AP_INT_STA;

	if(WkSt & CP_IDLE_OSW2_IRQ){
		hal_Osw2TickStop();
		hal_Osw2IrqHandle();
	}
}

BOOLEAN SLEEP_BT_PowerDomain_Set(BOOLEAN is_on)
{
    BOOLEAN ret = SCI_TRUE;
// delete temply by zhigang.peng, during 8910 bringup
#if 0
    uint32 cur_tick, next_tick;
    uint32 timeout_tick;
    
    SCI_DisableIRQ();
    
    if (is_on)
    {
        REG32(APB_RST0_SET) |= ARM7_SOFT_RST_SET;
        REG32(APB_BT_PWR_CTL) &= (~CM4_POW_FORCE_PD);
        
        REG32(APB_MISC_CTL3) &= (~BT_DAC_PD_O_SW);
        REG32(APB_MISC_CTL3) &= (~RF_BT_ADDA_PD_SW);

        cur_tick = next_tick = SCI_GetTickCount();
        timeout_tick = (((REG32(APB_BT_PWR_CTL) & CM4_PWR_ON_DLY_MSK) >> CM4_PWR_ON_DLY_SHIFT)
                       + ((REG32(APB_BT_PWR_CTL) & CM4_PWR_ON_SEQ_DLY_MSK) >> CM4_PWR_ON_SEQ_DLY_SHIFT) + 2) << 1;

        for(;;)
        {
            if ((REG32(APB_PWR_STATE) & CM4_PD_STATUS_MSK) == CM4_PWR_STATE_PD_WAKEUP)
            {
                ret = SCI_TRUE;
                break;
            }

            if ( (next_tick - cur_tick) > timeout_tick)
            {
                TM_SendTestPointRequest (0xEAEA0001, (next_tick - cur_tick));
                SCI_PASSERT(0, ("cm4 wake Timeout!"));/*assert to do*/
            }
            
            next_tick = SCI_GetTickCount();
        }
        SLEEP_CM4_PowerDomain_Set(SCI_TRUE);        
    }
    else
    {
        REG32(APB_MISC_CTL3) |= BT_DAC_PD_O_SW;
        REG32(APB_MISC_CTL3) |= RF_BT_ADDA_PD_SW;
        
        REG32(APB_BT_PWR_CTL) |= CM4_POW_FORCE_PD;
//        REG32(APB_BT_PWR_CTL) |= CM4_SLP_POWOFF_AUTO_EN;

        cur_tick = next_tick = SCI_GetTickCount();
        timeout_tick = (((REG32(APB_BT_PWR_CTL) & CM4_ISO_ON_DLY_MSK) >> CM4_ISO_ON_DLY_SHIFT) + 2) << 1;
        
        for(;;)
        {
            if ((REG32(APB_PWR_STATE) & CM4_PD_STATUS_MSK)
                    //== (CM4_PWR_STATE_PD_STANDBY | CM4_PWR_STATE_ISO_ON | CM4_PWR_STATE_SHUTDOWN)
                    == ( CM4_PWR_STATE_SHUTDOWN)
                )
            {
                ret = SCI_TRUE;
                break;
            }

            if ( (next_tick - cur_tick) > timeout_tick)
            {
                TM_SendTestPointRequest (0xEAEA0002, (next_tick - cur_tick));
                SCI_PASSERT(0, ("cm4 powerdown Timeout!"));/*assert to do*/
            }
            
            next_tick = SCI_GetTickCount();
        }
        SLEEP_CM4_PowerDomain_Set(SCI_FALSE); 
    }

    SCI_RestoreIRQ();

    return ret;
#endif 
	return ret;
}


void SLEEP_PowerDomain_Config()
{
#if 0

#ifdef ARM_CORE_POWER_DOMAIN

    REG32(APB_ARMCORE_PWR_CTL) &= (~ARMCORE_ISO_ON_MSK);
    REG32(APB_ARMCORE_PWR_CTL) |= BIT_0;
    
    REG32(APB_ARMCORE_PWR_CTL) &= (~ARMCORE_PWR_ON_SEQ_MSK);
    REG32(APB_ARMCORE_PWR_CTL) |= BIT_8;
    
    REG32(APB_ARMCORE_PWR_CTL) &= (~ARMCORE_PWR_ON_DLY_MSK);
    REG32(APB_ARMCORE_PWR_CTL) |= BIT_16;

    REG32(APB_ARMCORE_PWR_CTL) |= ARMCORE_SLP_POWOFF_AUTO_EN;
#endif

    
    REG32(APB_PERI_PWR_CTL) &= (~PERI_PWR_ON_DLY_MSK);
//    REG32(APB_PERI_PWR_CTL) |= BIT_0;
    
    REG32(APB_PERI_PWR_CTL) &= (~PERI_PWR_ON_SEQ_DLY_MSK);
//    REG32(APB_PERI_PWR_CTL) |= BIT_8;
    
    REG32(APB_PERI_PWR_CTL) &= (~PERI_ISO_ON_DLY_MSK);
//    REG32(APB_PERI_PWR_CTL) |= BIT_16;

    if (SLEEP_PERI_PowerDomain_Get())
    {
        REG32(APB_PERI_PWR_CTL) |= PERI_SLP_POWOFF_AUTO_EN;
    }
    
    REG32(APB_MM_PWR_CTL) &= (~MM_PWR_ON_DLY_MSK);
//    REG32(APB_MM_PWR_CTL) |= BIT_0;
    
    REG32(APB_MM_PWR_CTL) &= (~MM_ISO_ON_REQ_MSK);
//    REG32(APB_MM_PWR_CTL) |= BIT_8;
    
    REG32(APB_MM_PWR_CTL) &= (~MM_ISO_ON_MSK);
//    REG32(APB_MM_PWR_CTL) |= BIT_16;
    
    REG32(APB_MM_PWR_CTL) |= MM_SLP_POWOFF_AUTO_EN;


        
    REG32(APB_BT_PWR_CTL) &= (~CM4_PWR_ON_DLY_MSK);
//    REG32(APB_BT_PWR_CTL) |= BIT_0;
    
    REG32(APB_BT_PWR_CTL) &= (~CM4_PWR_ON_SEQ_DLY_MSK);
//    REG32(APB_BT_PWR_CTL) |= BIT_8;
    
    REG32(APB_BT_PWR_CTL) &= (~CM4_ISO_ON_DLY_MSK);
//    REG32(APB_BT_PWR_CTL) |= BIT_16;
#endif
}

void SLEEP_PllAutoPd(void)
{
#if 0
    uint32 reg_val;

    reg_val = REG32(APB_PLL_CTL0);

    reg_val &= (~BIT_29);//apll auto power down disable
    reg_val &= (~BIT_30);//cpll auto power down disable

    REG32(APB_PLL_CTL0) = reg_val;
#endif	
}

void SLEEP_AnalogBBTOPSet(void)
{
    //CHIP_REG_OR(ANALOG_BB_TOP3, BB_CPLL_PD_AUTO_OFF | BB_BG_AUTO_OFF | BB_LDO_AUTO_OFF | BB_26M_AUTO_OFF);//CPLL_PD,BG_PD,BB_LDO_PD,CLK26M_PD 
}

void SLEEP_CLKENSet(BOOLEAN is_enable)
{
#if 0
    if(is_enable)
    {
         // Enable CLK_EN to make CPLL not idle   
         if((SLEEP_CM4_PowerDomain_Get() == SCI_TRUE) &&( (sleep_active_modules & SLEEP_DRV_FM) == SLEEP_DRV_FM))
         {
             REG32(CLK_EN) |= (CLK_DAP_EN |BB_CLK_BBPLL_EN);
         }
         else if((SLEEP_CM4_PowerDomain_Get() == SCI_TRUE) && ((sleep_active_modules & SLEEP_DRV_FM) != SLEEP_DRV_FM))
         {
             CHIP_REG_OR(CPLL_APLL_CONFIG, CPLL_CLKOUT83P2M_FMCAL_EN);
             CHIP_REG_OR(ANALOG_BB_TOP3, CPLL_CLKOUT64M_FMA_EN);
             REG32(CLK_EN) |= (CLK_DAP_EN |BB_CLK_BBPLL_EN|CLK_RF_FM_26M_EN |CLK_FM_RF_CTL_EN);
         }
         else if((SLEEP_CM4_PowerDomain_Get() != SCI_TRUE) && ((sleep_active_modules & SLEEP_DRV_FM) == SLEEP_DRV_FM))
         {
             CHIP_REG_OR(ANALOG_BB_TOP3, CPLL_CLKOUT64M_BTA_EN);
             REG32(CLK_EN) |= (CLK_DAP_EN |BB_CLK_BBPLL_EN|CLK_RF_BT_26M_EN |CLK_BT_RF_CTL_EN |CLK_BT_LPO_EN);
         }
         else
         {
             CHIP_REG_OR(CPLL_APLL_CONFIG, CPLL_CLKOUT83P2M_FMCAL_EN);
             CHIP_REG_OR(ANALOG_BB_TOP3, (CPLL_CLKOUT64M_BTA_EN | CPLL_CLKOUT64M_FMA_EN));
             REG32(CLK_EN) = 0x1DF;
         }
    }
    else
    {
         // Disable CLK_EN to make CPLL idle    
         if((SLEEP_CM4_PowerDomain_Get() == SCI_TRUE) && ((sleep_active_modules & SLEEP_DRV_FM) == SLEEP_DRV_FM))
         {
             REG32(CLK_EN) &= ~(CLK_DAP_EN |BB_CLK_BBPLL_EN);
         }
         else if((SLEEP_CM4_PowerDomain_Get() == SCI_TRUE) && ((sleep_active_modules & SLEEP_DRV_FM) != SLEEP_DRV_FM))
         {
             CHIP_REG_AND(CPLL_APLL_CONFIG, ~CPLL_CLKOUT83P2M_FMCAL_EN);
             CHIP_REG_AND(ANALOG_BB_TOP3, ~CPLL_CLKOUT64M_FMA_EN);
             REG32(CLK_EN) &= ~(CLK_DAP_EN |BB_CLK_BBPLL_EN|CLK_RF_FM_26M_EN |CLK_FM_RF_CTL_EN);
         }
         else if((SLEEP_CM4_PowerDomain_Get() != SCI_TRUE) && ((sleep_active_modules & SLEEP_DRV_FM) == SLEEP_DRV_FM))
         {
             CHIP_REG_AND(ANALOG_BB_TOP3, ~CPLL_CLKOUT64M_BTA_EN);
             REG32(CLK_EN) &= ~(CLK_DAP_EN |BB_CLK_BBPLL_EN|CLK_RF_BT_26M_EN |CLK_BT_RF_CTL_EN |CLK_BT_LPO_EN);
         }
         else
         {
             CHIP_REG_AND(CPLL_APLL_CONFIG, ~CPLL_CLKOUT83P2M_FMCAL_EN);
             CHIP_REG_AND(ANALOG_BB_TOP3, ~(CPLL_CLKOUT64M_BTA_EN | CPLL_CLKOUT64M_FMA_EN));
             REG32(CLK_EN) = 0x40;
         }
    }	
#endif
}
void SLEEP_MemoryEnable(void)
{
#if 0
    REG32(MEM_LP_CTRL0) = 0;  // disable memory low power mode
    REG32(MEM_SLEEP_SD_EN0) = 0; 
    REG32(MEM_SLEEP_SD_EN1) = 0; 
    REG32(MEM_SLEEP_SD_EN2) = 0; 
    REG32(MEM_SLEEP_SD_EN3) = 0;  
#endif
}

void SLEEP_UsbDisable(void)
{
#if defined(CHIP_VER_UIX8910MPW)
#else
	hwp_analogReg->usb_reg2 = 0;
	hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_USBC;
	hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_USB11;
	hwp_sysCtrl->CLK_OTHERS_DISABLE  = SYS_CTRL_DISABLE_OC_ID_USBPHY;
	hwp_sysCtrl->CLK_OTHERS_1_DISABLE= SYS_CTRL_DISABLE_OC_ID_USB_UTMI_48M;
	hwp_sysCtrl->CLK_OTHERS_1_DISABLE= SYS_CTRL_DISABLE_OC_ID_USB11_48M;
	//LDO_TurnOnLDO (LDO_LDO_VDD33);
#endif	
	
	
}



/*****************************************************************************
 *  Description: get current 32K number.        
 *                                                                           
 *  Global Resource Dependence:                                              
 *  Author: bo.tang                                                          
 *  Note:   	                                         
******************************************************************************/
PUBLIC uint32 SLEEP_GetCur32kNum(void)
{
	return hwp_idle->IDL_32K_REF;
}


/*****************************************************************************
 *  Description: set cp sleep lock.        
 *                                                                           
 *  Global Resource Dependence:                                              
 *  Author: bo.tang                                                          
 *  Note:   	                                         
******************************************************************************/
PUBLIC void SLEEP_SetCpSysSlpLock(uint32 SlpEn)
{
	if(SlpEn)
		LPS_CPSYSSLP_CTRL = 0xABABABAB;
	else
		LPS_CPSYSSLP_CTRL = 0x0;
}

/*****************************************************************************
 *  Description:    deepsleep init at system startup time                    *
 *  Global Resource Dependence:                                              *
 *  Author:                                                                  *
 *  Note:                                                                    *
******************************************************************************/
PUBLIC void SLEEP_InitSoft(void)
{
	uint32 status;

	status = ISR_RegHandler (TB_IDLE_LPS_INT, (TB_ISR) SLEEP_IrqHandler);

    if (status == TB_SUCCESS)
    {
    	CHIPDRV_EnableIRQINT (TB_IDLE_LPS_INT);
    }
}
/*****************************************************************************
 *  Description:    deepsleep init at system startup time                    *
 *  Global Resource Dependence:                                              *
 *  Author:                                                                  *
 *  Note:                                                                    *
******************************************************************************/
PUBLIC void SLEEP_Init (void)
{
	uint32 temp;
	
    // Get the sleep table entry    
    s_slp_ctl_tab = Slp_Get_Cfg();
	SCI_CPCPU_EnableDeepSleep(0);
	hal_Osw2IrqEnable();
	SLEEP_SetCpSysSlpLock(FALSE);
	
#if defined(CHIP_VER_UIX8910MPW)
#else
	SLEEP_DisPllDisable();   //close DSIPLL
	//bit0--pad_gpio_6_i
	//bit1--vad_int_h
	//bit2--keypad_irq_h
	//bit3--gpio_1_irq_h
	//bit4--irq_uart_1_h
	//bit5--~pad_uart_1_rxd_i,at wakeup
	//bit6--wcn2sys_awake
	//bit7--wcn_osc_en
	// enable key/uart1_irq wake up function
    hwp_idle->IDL_AWK_EN |= CP_IDLE_AWK0_AWK_STAT;	//enable pmic wakeup
    hwp_idle->IDL_AWK_EN |= CP_IDLE_AWK2_AWK_STAT;	//enable key wakeup
	hwp_idle->IDL_AWK_EN |= CP_IDLE_AWK3_AWK_STAT;	//enable gpio wakeup
    hwp_idle->IDL_AWK_EN |= CP_IDLE_AWK4_AWK_STAT;	//enable uart1_irq wakeup
    hwp_idle->IDL_AWK_EN |= CP_IDLE_AWK6_AWK_STAT;	//enable uart1_irq wakeup
    hwp_idle->IDL_AWK_EN |= CP_IDLE_AWK7_AWK_STAT;	//enable wcn_osc_en wakeup
    hwp_idle->IDL_AWK_EN |= CP_IDLE_AWK_OSW2_STAT;	//enable osw2 wakeup
    hwp_idle->IDL_AWK_EN |= CP_IDLE_AWK_OSW1_STAT;	//enable osw1 wakeup

	hwp_idle->IDL_EN = 0x3;
	hwp_idle->SLOW_SYS_CLK_SEL_HWEN = 1;
	hwp_idle->SLOW_CLK_SEL_HWEN     = 1;

	// care: pm1 deepsleep,must close AON_LP domain,avoid wakeup,bus error.
	hwp_pwrctrl->PWR_HWEN          |= 0x40;

	//enable ddr low power function
	hwp_clkrst->CLKEN_BB_SYSCTRL_SET= CP_CLKRST_CLKEN_SYSREG;
	hwp_sysreg->DDR_SLP_WAIT_NUMBER = 0xC8;
	hwp_sysreg->DDR_SLP_CTRL_ENABLE = 1;

	//
	hwp_sysreg->A5_STANDBYWFI_EN = 0x3;
	
	// enable lp mode 
	hwp_sysCtrl->Cfg_FORCE_LP_MODE_LP = 0x7F80;

	// enable zsp bus low power
	hwp_clkrst->ZSP_BUSLPMC_SET = 0xF; //enable zsp buslpmc
	
#if 0     /*将lowpower enable操作放到bootloader切换成快时钟之前*/
	// enable APLL low power mode
	hwp_analogReg->APLL_reg0 |= ANALOG_REG_APLL_LP_MODE_EN;

	// enable MEMPLL low power mode
	hwp_analogReg->MEMPLL_reg0 |= ANALOG_REG_MEMPLL_LP_MODE_EN;
#endif

	// sysctrl register write unlock
	hwp_sysCtrl->REG_DBG = 0xA50001;
	// clock autogate
	hwp_sysCtrl->CLK_SYS_AXI_MODE = 0x1F;
	hwp_sysCtrl->CLK_SYS_AHB_MODE = 0x1E;
	hwp_sysCtrl->CLK_AP_APB_MODE  = 0x782001;
	hwp_sysCtrl->CLK_AIF_APB_MODE = 0xF01;
	hwp_sysCtrl->CLK_AON_APB_MODE = 0xFE6C1;
	hwp_sysCtrl->CLK_OTHERS_MODE  = 0xF8;
	
	// disable modules
	// 1.disable lvds
	hwp_sysCtrl->CLK_AON_APB_DISABLE  = 0x8000000;

	// 2.disable vad
	hwp_sysCtrl->REG_DBG = 0xA50001;
	hwp_sysCtrl->CLK_AON_APB_DISABLE  = 0x80000;     //disable int clock
	hwp_sysCtrl->CLK_OTHERS_1_DISABLE = 0x4000000;   //disable func clock
	// 3.close camera
	hwp_sysCtrl->REG_DBG = 0xA50001;
	hwp_sysCtrl->CLK_AP_APB_DISABLE   = SYS_CTRL_DISABLE_AP_APB_CLK_ID_MOD_CAMERA;
	hwp_sysCtrl->CLK_AP_APB_DISABLE   = SYS_CTRL_DISABLE_AP_APB_CLK_ID_CAMERA;
	hwp_sysCtrl->CLK_OTHERS_DISABLE   = SYS_CTRL_DISABLE_OC_ID_PIX;
	hwp_sysCtrl->CLK_OTHERS_DISABLE   = SYS_CTRL_DISABLE_OC_ID_ISP;
	hwp_sysCtrl->CLK_OTHERS_DISABLE   = (SYS_CTRL_DISABLE_OC_ID_SPICAM |  \
										 SYS_CTRL_DISABLE_OC_ID_CAM);
	LDO_TurnOffLDO (LDO_LDO_CAMA);
	LDO_TurnOffLDO (LDO_LDO_CAMIO);
	// 4.close pagespy
	hwp_sysCtrl->REG_DBG = 0xA50001;
	hwp_sysCtrl->CLK_AP_APB_DISABLE   = SYS_CTRL_DISABLE_AP_APB_CLK_ID_PAGESPY;
	hwp_sysCtrl->CLK_OTHERS_DISABLE   = SYS_CTRL_DISABLE_OC_ID_PSRAM_PAGESPY;

	// 5.close bck
	hwp_sysCtrl->REG_DBG = 0xA50001;
	hwp_sysCtrl->CLK_OTHERS_DISABLE   = (SYS_CTRL_DISABLE_OC_ID_BCK1 |    \
										 SYS_CTRL_DISABLE_OC_ID_BCK2);
	hwp_sysCtrl->CLK_AIF_APB_DISABLE  = SYS_CTRL_DISABLE_AIF_APB_CLK_ID_AUD_2AD;
#if 0
	// 6.close coresight
	hwp_sysCtrl->CLK_OTHERS_1_DISABLE = SYS_CTRL_DISABLE_OC_ID_CORESIGHT;
#endif
    sRamRo = SCI_ALLOC_APP(LPS_APSRAM_SIZE);

#endif	
    // Config all sleep level common parameters
    //Slp_Reg_Cfg (0,SLP_CFG_COM);

	//enable ap cp standby wfi enable switch slow clock
	//hwp_sysreg->A5_STANDBYWFI_EN = (CP_SYSREG_AP_A5_STANDBY_EN | 
	//								CP_SYSREG_CP_A5_STANDBY_EN);

    // Deepsleep flag init
    is_deepsleep  = SCI_FALSE;
	LpsRef32kNum  = IDL_REF_32K;
	sHalSlpInfo.osTimerAdFlag = FALSE;
	sHalSlpInfo.wfiTotal = 0;



#if 0
    CHIP_REG_AND (APB_XTL_CTL0, (~XTL_WAIT_SEL));    
    CHIP_REG_AND(APB_XTL_CTL0, (~(0x140<<XTL_BUF_WAIT_SHIFT)));
    CHIP_REG_OR(APB_XTL_CTL0, (0x10<<XTL_BUF_WAIT_SHIFT));          // xtl buf wait time : 16 rtc cycle
    
    CHIP_REG_AND(APB_XTL_CTL0, (~(0x140<<XTL_WAIT_SHIFT)));
    CHIP_REG_OR(APB_XTL_CTL0, (0x20<<XTL_WAIT_SHIFT));              // xtl wait time : 32 rtc cycle

    CHIP_REG_AND (CPLL_APLL_SEL, (~CPLL_WAIT_CTRL_SEL));    
    CHIP_REG_AND(CPLL_APLL_PWR_CTRL, (~(0x140<<MCU_CPLL_WAIT_SHIFT)));
    CHIP_REG_OR(CPLL_APLL_PWR_CTRL, (0x28<<MCU_CPLL_WAIT_SHIFT));           // cpll wait time : 40 rtc cycle
  
    CHIP_REG_AND(CPLL_APLL_PWR_CTRL, (~(0x140<<MCU_APLL_WAIT_SHIFT)));
    CHIP_REG_OR(CPLL_APLL_PWR_CTRL, (0x28<<MCU_APLL_WAIT_SHIFT));           // apll wait time : 40 rtc cycle

    CHIP_REG_AND (MCU_PWR_WAIT, (~PWR_WAIT_CTRL_SEL));    
    CHIP_REG_AND(MCU_PWR_WAIT, (~(0x140<<MCU_PWR_WAIT_SHIFT)));
    CHIP_REG_OR(MCU_PWR_WAIT, (0x28<<MCU_PWR_WAIT_SHIFT));           // pll pwr wait time : 40 rtc cycle

    ANA_REG_MSK_OR(ANA_XTL_WAIT_CTRL, 0x32, ANA_XTL_WAIT_MSK);   // adie xtl wait time : 32 rtc cycle   

    //32K less for deepsleep
    ANA_REG_OR(ANA_CLK32KLESS_CTRL0, SLP_XO_LOW_CUR_EN); //Enable: DCXO low current mode enable in chip sleep mode

#ifndef ARM_CPLLCLK_DEEPSLEEP_SUPPORT
    //LDO CORE drop and rise by step, LDO CORE = 0.85V in deepsleep.
    ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL1, ~LDO_CORE_V_DS_MSK); 
    ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL1, 0xA << LDO_CORE_V_DS_SHIFT); //DS CORE:0.85V
    ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL1, ~LDO_CORE_V_SLP_STEP1_MSK); 
    ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL1, 0xF << LDO_CORE_V_SLP_STEP1_SHIFT); //step1:0.90V
    ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL2, ~LDO_CORE_V_SLP_STEP2_MSK); 
    ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL2, 0x13 << LDO_CORE_V_SLP_STEP2_SHIFT); //step2:0.94V
    ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL2, ~LDO_CORE_V_SLP_STEP3_MSK); 
    ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL2, 0x17 << LDO_CORE_V_SLP_STEP3_SHIFT); //step3:0.98V
    ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL3, ~LDO_CORE_V_SLP_STEP4_MSK); 
    ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL3, 0x1B << LDO_CORE_V_SLP_STEP4_SHIFT); //step4:1.02V
    ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL3, ~LDO_CORE_V_SLP_STEP5_MSK); 
    ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL3, 0x1F << LDO_CORE_V_SLP_STEP5_SHIFT); //step5:1.06V
    ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL0, SLP_LDOCORE_DROP_EN | LDO_CORE_SLP_OUT_STEP_EN | LDO_CORE_SLP_IN_STEP_EN);
#else
    if(SC6531EFM != CHIP_GetChipType())
    {
        //LDO CORE drop and rise by step, LDO CORE = 0.85V in deepsleep.
        ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL1, ~LDO_CORE_V_DS_MSK); 
        ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL1, 0xA << LDO_CORE_V_DS_SHIFT); //DS CORE:0.85V
        ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL1, ~LDO_CORE_V_SLP_STEP1_MSK); 
        ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL1, 0xF << LDO_CORE_V_SLP_STEP1_SHIFT); //step1:0.90V
        ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL2, ~LDO_CORE_V_SLP_STEP2_MSK); 
        ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL2, 0x13 << LDO_CORE_V_SLP_STEP2_SHIFT); //step2:0.94V
        ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL2, ~LDO_CORE_V_SLP_STEP3_MSK); 
        ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL2, 0x17 << LDO_CORE_V_SLP_STEP3_SHIFT); //step3:0.98V
        ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL3, ~LDO_CORE_V_SLP_STEP4_MSK); 
        ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL3, 0x1B << LDO_CORE_V_SLP_STEP4_SHIFT); //step4:1.02V
        ANA_REG_AND(ANA_LDO_CORE_SLP_CTRL3, ~LDO_CORE_V_SLP_STEP5_MSK); 
        ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL3, 0x1F << LDO_CORE_V_SLP_STEP5_SHIFT); //step5:1.06V
        ANA_REG_OR(ANA_LDO_CORE_SLP_CTRL0, SLP_LDOCORE_DROP_EN | LDO_CORE_SLP_OUT_STEP_EN | LDO_CORE_SLP_IN_STEP_EN);
    }
#endif
    ANA_REG_OR(ANA_SLP_CTRL, SLP_LDO_PD_EN |SLP_IO_EN); //Enable: ALL LDO and DCDC power down enable in deep sleep mode

    ANA_REG_OR(ANA_SLP_LDO_LP_CTRL0, 0x0000EF2F); //Set all LDO lowpower mode in deep sleep
    //ANA_REG_OR(ANA_SLP_LDO_PD_CTRL0, 0x0000DD2B); //Set all LDO PD mode in deep sleep, for debug

    //enable dma wake up mcu
    CHIP_REG_OR (AHB_SLP_CTL2, ~(0x0));

    SLEEP_AnalogBBTOPSet();
    SLEEP_PllAutoPd();
    SLEEP_PowerDomain_Config();
    SLEEP_MemoryEnable();
#endif
}

PUBLIC void SLEEP_SetChgOsw2Timer(uint32 Durms)
{
#if 0
   SCI_TRACE_LOW("SLEEP_SetChgOsw2Timer Durms=%dms, tickCount=%d, IDL_32K_REF=%d", 
   		Durms, 
		SCI_GetTickCount(),
		HAL_GET_REF32K);
#endif 
	if(hal_Osw2TickEnable()){
//        SCI_TRACE_LOW("call hal_Osw2TickStop ");
		hal_Osw2TickStop();
//		while(hal_Osw2TickEnable());
	}
	hal_Osw2TickStart(Durms*16384/1000);
	sHalSlpInfo.Osw2ChgSet = Durms;
	sHalSlpInfo.osTimerChg = HAL_GET_REF32K;
}
/*****************************************************************************
 *  Description:    this function is used to save poweroff domain reg        
 *                                                                           
 *  Global Resource Dependence:                                              
 *  Author: bo.tang                                                          
 *  Note:   	                                         
******************************************************************************/
PUBLIC void SLEEP_Suspend(uint32 slp_index)
{
	if((slp_index == AHB_CAN_SLP_APB_CAN_SLP) 
	&& (!sleep_active_modules)){
		PRIMCPU_WAKEUP_MAGIC_ADDR = CONFIG_PRIMARY_WAKEUP_JUMP_MAGIC;
		PRIMCPU_WAKEUP_JUMP_ADDR  = (uint32)UNI_WAKEUP_ENTRY;
		PRIMCPU_RESUME_MAGIC_ADDR = CONFIG_PRIMARY_RESUME_JUMP_MAGIC;
		PRIMCPU_RESUME_JUMP_ADDR  = (uint32)RECOVERY_ARM_REG;
		INT_PHY_GicDistSuspend();
		Slp_LpsSuspend();
		sHalSlpInfo.ApRunPoint2  = hwp_idle->IDL_32K_REF;
		SLEEP_LpsSramSave();
		sHalSlpInfo.ApRunPoint3  = hwp_idle->IDL_32K_REF;
		//MMU_CacheOff(1,1);
		MMU_DisableIDCM();
		sHalSlpInfo.ApRunPoint4  = hwp_idle->IDL_32K_REF;
		
		SLEEP_OsTimerStop();
		hwp_idle->SLOW_SYS_CLK_SEL_HWEN = 1;
		sHalSlpInfo.ApSlpEnry32K = hwp_idle->IDL_32K_REF;
		sHalSlpInfo.CpUnlock32K  = sHalSlpInfo.CpUnlock32K - sHalSlpInfo.ApSlpWk32K;
		sHalSlpInfo.ApRunPoint4 -= sHalSlpInfo.ApRunPoint3;
		sHalSlpInfo.ApRunPoint3 -= sHalSlpInfo.ApRunPoint2;
		sHalSlpInfo.ApRunPoint2 -= sHalSlpInfo.ApRunPoint1;
	}

}

/*****************************************************************************
 *  Description:    this function is used to restore poweroff domain reg        
 *                                                                           
 *  Global Resource Dependence:                                              
 *  Author: bo.tang                                                          
 *  Note:   	                                         
******************************************************************************/
PUBLIC void SLEEP_Resume(uint32 slp_index)
{
    uint32 wksource = 0;
    uint32 WfiRate  = 0;
    uint32 WfiTestS = 0;
    uint32 ActLock,AhbLock,ApbLock;
    uint32 CpWkDur,ApWkDur,SlptDur;
    uint32 WfiTotal;

    //SLEEP_SwitchAxiBusFreq(AXIBUSFREQ_400MHZ);
    hwp_idle->IDLE_RES9 |=(1<<4);
    ActLock = sleep_active_modules;
    AhbLock = sleep_ahb_active_modules;
    ApbLock = sleep_apb_active_modues;
	
// resume IDCM
	if((slp_index == AHB_CAN_SLP_APB_CAN_SLP) && (!ActLock)){
		sHalSlpInfo.DoffFlow1     = HAL_GET_REF32K;
		MMU_EnableIDCM();		//MMU_CacheOn(1,1);
	}
	
// get wake up source 
	wksource = hwp_idle->IDL_AWK_ST;

// set sd ldo state, set a flag for mcd_close
   if(0x8 == LpsDomainSt){
         g_LdoEmmcPwrOffFlag =  TRUE;
   }

// resume sram and module
	if(0x8 == LpsDomainSt){
		hwp_idle->IDLE_RES9 |=(1<<5);
		LPS_MLBOX_POINT0 = HAL_GET_REF32K;
		LPS_MLBOX_POINT2 = (sHalSlpInfo.ApSlpEnry32K - sHalSlpInfo.ApSlpWk32K);
		LPS_MLBOX_POINT3 = sHalSlpInfo.CpUnlock32K;
		sHalSlpInfo.ApSlpWk32K = HAL_GET_REF32K;
		sHalSlpInfo.DoffFlow2     = HAL_GET_REF32K;
		sHalSlpInfo.wfiTotal = 0;
		SLEEP_LpsSramRestore();
		sHalSlpInfo.DoffFlow3     = HAL_GET_REF32K;
		INT_PHY_GicDistResume();
		ipc_set_bt_rf_sm_addr();
		sHalSlpInfo.DoffFlow4     = HAL_GET_REF32K;
		Slp_LpsResume();
		sHalSlpInfo.DoffFlow5     = HAL_GET_REF32K;
		SLEEP_ReleaseCP();
		sHalSlpInfo.DoffFlow6     = HAL_GET_REF32K;
		SlptDur = (uint32)((float)(HAL_GET_REF32K - sHalSlpInfo.ApSlpEnry32K)/32.768);
		CpWkDur = (uint32)((float)sHalSlpInfo.CpUnlock32K/32.768);
		ApWkDur = (uint32)((float)LPS_MLBOX_POINT2/32.768);
		SCI_TraceLow("LPS WkUp Doff Resume:Mmu:%d,Sram:%d,Gic:%d,Module:%d,ApTotal:%d,CpTotal:%d)", 
			sHalSlpInfo.DoffFlow2 - sHalSlpInfo.DoffFlow1,
			sHalSlpInfo.DoffFlow3 - sHalSlpInfo.DoffFlow2,
			sHalSlpInfo.DoffFlow4 - sHalSlpInfo.DoffFlow3,
			sHalSlpInfo.DoffFlow5 - sHalSlpInfo.DoffFlow4,
			sHalSlpInfo.DoffFlow5 - sHalSlpInfo.DoffFlow1,
			sHalSlpInfo.DoffFlow6 - sHalSlpInfo.DoffFlow5);
		
		if(ApWkDur < 200){
			SCI_TraceLow("LPS WkUp Doff_OK wkSr:0x%x,MTimer:0x%x,Cur32K:0x%x,OsFlag:%d,SlpDur:%dms,WkDur(Cp:%dms,Ap:%dms)", 
				wksource,
				hwp_idle->IDL_M_TIMER, 
				hwp_idle->IDL_32K_REF,
				sHalSlpInfo.osTimerAdFlag,
				SlptDur,CpWkDur,ApWkDur);
		}else{
			SCI_TraceLow("LPS WkUp Doff_FL wkSr:0x%x,MTimer:0x%x,Cur32K:0x%x,OsFlag:%d,SlpDur:%dms,WkDur(Cp:%dms,Ap:%dms[%d,%d,%d])", 
				wksource,
				hwp_idle->IDL_M_TIMER, 
				hwp_idle->IDL_32K_REF,
				sHalSlpInfo.osTimerAdFlag,
				SlptDur, CpWkDur, ApWkDur,
				sHalSlpInfo.ApRunPoint2,
				sHalSlpInfo.ApRunPoint3,
				sHalSlpInfo.ApRunPoint4);
		}
		
	}else if(((hwp_idle->IDLE_RES9&0xF) == 0xC) && (LpsDomainSt != 0x8)){
		hwp_idle->IDLE_RES9 |=(1<<6);
		while(1);
		SCI_ASSERT (0);
	}else if(slp_index == AHB_CAN_SLP_APB_CAN_SLP){
		SCI_TraceLow("LPS Wkup Don(index:0x%x,step:0x%x,ActLock:0x%x,AHBLock:0x%x,APBLock:0x%x,WkSt:0x%x)", 
			slp_index, s_sleep_step,ActLock,AhbLock,ApbLock,hwp_idle->IDL_AWK_ST);
	}

	// Resume IDCM and ostimer.
	//if((slp_index == AHB_CAN_SLP_APB_CAN_SLP) && (!ActLock)){
	//	hwp_idle->IDLE_RES9 |=(1<<7);
	//	SLEEP_OsTimerStart();
	//}
	SLEEP_OsTimerStart();
	hwp_idle->IDLE_RES9 |=(1<<8);
	
	
    if((slp_index == AHB_CAN_SLP_APB_CAN_SLP) && (ActLock & SLEEP_DRV_AUDIO)){
		SCI_TraceLow("LPS AudioLock(index:0x%x,step:0x%x,ActLock:0x%x,AHBLock:0x%x,APBLock:0x%x)", 
			slp_index, s_sleep_step,ActLock,AhbLock,ApbLock);
	}
	
	// wfi point
	if(0x8 != LpsDomainSt){
		sHalSlpInfo.wfiTotal += (HAL_GET_REF32K - enterWfi32K);
	}
	WfiTestS = LPS_32KNUM_MASK_2S;
	//if((cpuWfi32kNum & WfiTestS) != (HAL_GET_REF32K & WfiTestS)){
	if((HAL_GET_REF32K - cpuWfi32kNum) > WfiTestS){
		WfiTotal = (HAL_GET_REF32K - cpuWfi32kNum);
		WfiRate  = sHalSlpInfo.wfiTotal*100/WfiTotal;		

		SCI_TraceLow("LPS wfi[index:0x%x,ActLock:0x%x,AHBLock:0x%x,APBLock:0x%x,32KNum(0x%x->0x%x),WfiEn:0x%x,WFI(Idle:%dms,Total:%dms,wfiRate:%d)]", 
			slp_index,ActLock,AhbLock,ApbLock,
			cpuWfi32kNum,HAL_GET_REF32K,
			hwp_sysreg->A5_STANDBYWFI_EN,
			(int32)((float)(sHalSlpInfo.wfiTotal)/32.768),
			(int32)((float)(WfiTotal)/32.768),
			WfiRate);
		cpuWfi32kNum = HAL_GET_REF32K;
		sHalSlpInfo.wfiTotal = 0;
	}
	sHalSlpInfo.wfiOut32K = HAL_GET_REF32K;
	LpsDomainSt = 0;
	SLEEP_WakeUpHandle(wksource);
	hwp_idle->IDLE_RES9 |=(1<<9);
}

LOCAL void SLEEP_RestoreReg (void)
{
    //REG_LOCAL_VALUE_DEF

    //RESTORE_GLOBAL_REG;

    if (is_deepsleep)
    {
        SCI_AfterExitDeepSleep();
    }

    //is_deepsleep = SCI_FALSE;
    //CHIP_REG_AND (AHB_SLP_CTL1, ~MCU_DMA_WAKE_UP_EN);

    TM_SendSleepRequest (TM_LEAVE_SLEEP_REQ);

    MCU_is_sleep = 0;
}

/*****************************************************************************/
//  Description:    This function is used to set system into sleep status.
//  Author:
//  Note:
//****************************************************************************/
PUBLIC void SCI_EnterDeepSleep (void)
{
    uint32 ahb_module_clear;
    uint32 clk_axi_freq;
    SLP_AHB_APB_TYPE_E  slp_index = AHB_NO_SLP_APB_NO_SLP;

    //SCI_TraceLow("enter SCI_EnterDeepSleep  0x%x", SCI_Getsleepmode());
#ifndef  JTAG_SUPPORT
    TX_INTERRUPT_SAVE_AREA
    TX_DISABLE_FIQ_IRQ

#ifdef SLEEP_DEBUG
    s_sleep_step = 0;
#endif
    sHalSlpInfo.ApRunPoint1 = hwp_idle->IDL_32K_REF;
    hwp_idle->IDLE_RES9 = 0x00;
    is_deepsleep = SCI_FALSE;
    // Send enter sleep request
    TM_SendSleepRequest (TM_ENTER_SLEEP_REQ);
    SLEEP_LPS_EnableDeepSleep();
    SLEEP_WCNOSC_EnableDeepSleep();
    SLEEP_OsTimerCheck();
    // Get slp status
    slp_index = Slp_Get_Slp_Status();
    // Save global register value
    //SAVE_GLOBAL_REG; 

    switch(SCI_Getsleepmode())
    {
	case SLEEP_DRV_NONE:
	    //lint -fallthrough
	//case SLEEP_DRV_LCD:
	//case SLEEP_DRV_LINEIN:
	//case SLEEP_DRV_LCD|SLEEP_DRV_LINEIN:
        s_sleep_step |= BIT_0;
        if (slp_index == AHB_CAN_SLP_APB_CAN_SLP){
            is_deepsleep = SCI_TRUE;   	// Enter deepsleep
            //SCI_BeforeEnterDeepSleep();	// Add customre sleep routine here
        }
        //SLEEP_DisableAudioModule();
#if 0
        if((slp_index == AHB_CAN_SLP_APB_CAN_SLP) ||(slp_index == AHB_NO_SLP_APB_CAN_SLP)){
            // Disable APB related moudles, in order to enter further
            // deep sleep state
            SLEEP_DisableAPBModule();
        }

        if((slp_index == AHB_CAN_SLP_APB_CAN_SLP) ||(slp_index == AHB_CAN_SLP_APB_NO_SLP)){
            // Disable AHB masters, to reduce the power consume by clock
            // tree
            SLEEP_DisableAHBModule();
        }
#endif
		
    break;

	case SLEEP_DRV_AUDIO|SLEEP_DRV_CPCPU:						//1.MP3;2.2G CALL
		//SLEEP_SwitchAxiBusFreq(AXIBUSFREQ_200MHZ);
		hwp_idle->SLOW_SYS_CLK_SEL_HWEN = 0;
		slp_index = AHB_NO_SLP_APB_NO_SLP;
		break;

// When playing MP3, MIDI etc, the system can enter half-deepsleep to save power
	case SLEEP_DRV_ENGINEER:
	case SLEEP_DRV_ENGINEER | SLEEP_DRV_CPCPU:
	case SLEEP_DRV_AUDIO:
	case SLEEP_DRV_CPCPU:										//AP idle+CP work
	case SLEEP_DRV_AUDIO|SLEEP_DRV_CPCPU|SLEEP_DRV_WCN_OSC:  	//FM+GSM online
		hwp_idle->SLOW_SYS_CLK_SEL_HWEN = 0;
		//ahb_module_clear = AHB_EB0_MASK;		
		//CHIP_REG_OR (AHB_SLP_CTL1, MCU_DMA_WAKE_UP_EN);
		if(SLEEP_AHB_DMA == sleep_ahb_active_modules){
			//ahb_module_clear &= ~DMA_EB_CLR;
			//REG32(AHB_EB0_CLR) = ahb_module_clear;
			//slp_index = AHB_CAN_SLP_APB_CAN_SLP;
		}
		slp_index = AHB_NO_SLP_APB_NO_SLP;
		break;
	case SLEEP_DRV_LCD:
	case SLEEP_DRV_MMI:
	case SLEEP_DRV_LCD|SLEEP_DRV_MMI:
	case SLEEP_DRV_LCD|SLEEP_DRV_MMI|SLEEP_DRV_CPCPU:
	case SLEEP_DRV_MMI|SLEEP_DRV_AUDIO|SLEEP_DRV_LCD|SLEEP_DRV_MP4|SLEEP_DRV_CPCPU:    //MP4 Play+CP work
	case SLEEP_DRV_MMI|SLEEP_DRV_AUDIO|SLEEP_DRV_LCD|SLEEP_DRV_MP4:    				   //MP4 Play+CP IDLE
		if((sleep_ahb_active_modules == 0) 
		&& (sleep_apb_active_modues == 0))
		{
			hwp_idle->SLOW_SYS_CLK_SEL_HWEN = 0;
//			SLEEP_DisableAPBModule();
//			SLEEP_DisableAHBModule();
		}
		else
		{
			
		}
		slp_index = AHB_NO_SLP_APB_NO_SLP;

		break;
		
	case SLEEP_DRV_MMI|SLEEP_DRV_AUDIO:
	case SLEEP_DRV_LCD|SLEEP_DRV_MP4:
	case SLEEP_DRV_LCD|SLEEP_DRV_AUDIO|SLEEP_DRV_MP4:
		//ahb_module_clear = AHB_EB0_MASK;		
		//CHIP_REG_OR (AHB_SLP_CTL1, MCU_DMA_WAKE_UP_EN);
		
		if(((SLEEP_AHB_DMA | SLEEP_AHB_ISP) == sleep_ahb_active_modules)
			|| (SLEEP_AHB_DMA == sleep_ahb_active_modules)){
			//ahb_module_clear &= ~DMA_EB_CLR;
			//REG32(AHB_EB0_CLR) = ahb_module_clear;

			//slp_index = AHB_CAN_SLP_APB_CAN_SLP;
		}else if(SLEEP_AHB_ISP == sleep_ahb_active_modules){
			//REG32(AHB_EB0_CLR) = ahb_module_clear;
			//slp_index = AHB_CAN_SLP_APB_CAN_SLP;
		}
        slp_index    = AHB_NO_SLP_APB_NO_SLP;
		break;
		
	case SLEEP_DRV_BTCLK|SLEEP_DRV_AUDIO:
	case SLEEP_DRV_BTCLK|SLEEP_DRV_MMI|SLEEP_DRV_AUDIO:
	case SLEEP_DRV_BTCLK:
	case SLEEP_DRV_FM|SLEEP_DRV_AUDIO:
#ifdef SLEEP_DEBUG
        s_sleep_step |= BIT_1;
#endif
        SCI_BeforeEnterHalfDeepSleep();

        if((slp_index == AHB_CAN_SLP_APB_CAN_SLP) ||(slp_index == AHB_CAN_SLP_APB_NO_SLP)){
            // Disable AHB masters, to reduce the power consume by clock tree
            SLEEP_DisableAHBModule();
        }
        slp_index    = AHB_NO_SLP_APB_NO_SLP;
	    break;

	    // Enter ARM core sleep
	default:
        slp_index    = AHB_NO_SLP_APB_NO_SLP;
		break;
		
	}

#ifdef SLEEP_DEBUG
    TM_SendTestPointRequest (0x112233,   1);
    s_sleep_step |= BIT_3;
    TM_SendTestPointRequest (slp_index,   SCI_Getsleepmode());
    TM_SendTestPointRequest (SLEEP_GetAHBModuleStatus(), SLEEP_GetAPBModuleStatus());
    //TM_SendTestPointRequest (REG32 (AHB_SLP_STS0),    REG32 (APB_SLP_STS0));
    //TM_SendTestPointRequest (DMA_PHY_DMASleepFlag(), 444);
    //TM_SendTestPointRequest (REG32(AHB_SLP_DBG), 555);
    //TM_SendTestPointRequest (REG32(APB_EB0),   REG32(AHB_EB0));
    TM_SendTestPointRequest (0x445566,   2);
    s_sleep_step |= BIT_4;
#endif

// Enter different sleep routine according to ahb and apb state
     //SLEEP_CLKENSet(SCI_FALSE);		// Disable CLK_EN to make CPLL idle

#if defined(CHIP_VER_UIS8910A) || defined(MEM_TYPE_DDR)
	slp_index = AHB_NO_SLP_APB_NO_SLP;
#endif	
	

	//slp_index = AHB_NO_SLP_APB_NO_SLP;
	enterWfi32K = hwp_idle->IDL_32K_REF;
	hwp_idle->IDLE_RES9 = 1;
	SLEEP_Suspend(slp_index);
	SCI_Enter_SleepRoutine(slp_index);
	TM_SendTestPointRequest(0x778899,   LpsDomainSt);  
	SLEEP_Resume(slp_index);

       
    //SLEEP_CLKENSet(SCI_TRUE);			// Enable CLK_EN to make CPLL not idle
    //REG32 (AHB_SLP_CTL1) &=  ~ (MCU_DEEP_SLP_EN | MCU_SYS_SLP_EN);

    TM_SendTestPointRequest (0xaabbcc,   4);
#if 0
    //to check share memory flag whether need to hold arm
    if (SCI_IsEnalbeArmHold())
    {
        DSP_HoldArmByFIQ(HOLD_ARM_TIME_BY_POLLING);
    }
#endif

    SLEEP_RestoreReg();
    //SLEEP_WakeUpHandle();
#ifdef CARD_LOG
    SIO_SetCardLogPauseFlag(SCI_FALSE);	
#endif
    TX_RESTORE
#endif
}

void SCI_RTC_TimerHandler (void)
{
    // Check if MCU is wakeup by other interrupt
}



/*****************************************************************************/
// Description :    This function is used to set the flag which indicate if xtl
//                  need open in deepsleep
// Global resource dependence :s_sleep_xtl_open_cnt
// Author :         yuhua.shi
// Note :
/*****************************************************************************/
PUBLIC void DPSLP_XTLOpenInSleep (
    BOOLEAN is_open             //indicated if xtl need open in deepsleep,
    //SCI_TRUE: need open xtl in deepsleep; SCI_FALSE: can close xtl in deepsleep
)
{
    //do nothing
}

/*****************************************************************************/
// Description :    This function is used to production test
// Author :         hao.liu
// Note :
/*****************************************************************************/
PUBLIC void SCI_Deep_Sleep_Test (void)
{
    int  loop;
	uint8 lpscp = 1;
	uint8 count = 0;
	do
	{
#ifndef  JTAG_SUPPORT
		TX_INTERRUPT_SAVE_AREA
		TX_DISABLE_FIQ_IRQ
#endif
		if ((hwp_idle->IDL_CTRL_SYS1&0x1) && (hwp_idle->IDL_CTRL_TIMER&0x1) && lpscp)
		{
			uint32 cur32Num   = hwp_idle->IDL_32K_REF;
			uint32 WkUp32Knum = hwp_idle->IDL_M_TIMER;
			uint32 slpnum = 0;
			slpnum = (uint32)(((uint64)WkUp32Knum + LPS_32KNUM_MASK - 
					        (uint64)cur32Num)%LPS_32KNUM_MASK);
			if(slpnum > 8192){
				//閲嶇疆鐫＄湢鏃堕棿
				hwp_idle->IDL_M_TIMER = hwp_idle->IDL_32K_REF-8192;
				hwp_idle->IDL_AWK_EN = 0;
				hwp_idle->IDL_OSW1_EN = 0;
				lpscp = 0;
			}
			else
			{
				lpscp = 1;
			}
		}
	
		SLEEP_OsTimerCheck();
		if (RF_enable_flag 
		|| (sleep_active_modules&(~SLEEP_DRV_CALIBRATION)) 
		||	sleep_ahb_active_modules 
		|| 	sleep_apb_active_modues 
		||  lpscp)
		{
#ifndef  JTAG_SUPPORT			
			TX_RESTORE
#endif
			if (count >= 1000/100)
			{
				mon_Event(__LINE__);
				mon_Event(RF_enable_flag);
				mon_Event(sleep_active_modules);
				mon_Event(sleep_apb_active_modues);
				mon_Event(sleep_ahb_active_modules);
				count = 0;
			}
			count++;
			SCI_Sleep(100);
		}
		else
		{
			break;
		}
	}while(1);
	
#if 1
	// Disable Interrupt controller
	INT_HAL_DisableINT();/*lint !e718 !e746 -esym(628, INT_HAL_DisableINT)*/

	// Disable the irq
	SCI_DisableIRQ();
#endif

	SCI_EnterAssertMode();
	
	SLEEP_OsTimerStop();
	sleep_apb_active_modues &= (~SLEEP_APB_OSTIMER);
	SCI_Calibration_EnableDeepSleep(1);
	
	hwp_idle->IDL_AWK_EN |= CP_IDLE_AWK_OSW2_STAT;

	SLEEP_SetChgOsw2Timer(5000);
	
	SCI_EnterDeepSleep();

    for (loop = 0; loop < 0x4000; loop++)
    {
    }

    // for we has been delay a little time after set the deep sleep bit,
    // so shouldn't reach here.
    // If reach here, we need find the reason.
}





#if 0
/* 
  * save ddr corresponding registers, pad registers is save in aon area,
  * so no need to save. 
  */
PUBLIC void rda_pm_lpddr2_regs_save(void)
{
	uint32 save_addr = (uint32)(&ddr_param[DDR_PARAM_ARRAY_LEN-1]);
	struct ddr_save_pattern *p_save = (struct ddr_save_pattern *)save_addr;
	int32 i = 0;
#ifdef TGT_AP_MEM_TYPE_PSRAM
	uint32 addr_base = REG_PSRAM_PHY_BASE;
#else
	uint32 addr_base = REG_LPDDR_PHY_BASE;
#endif
	uint32 save_reg_num = ARRAY_SIZE(phy_off_val) + ARRAY_SIZE(dmc_off_val);
	DDR_PARAM_SAVE_ADDR = save_addr;

	
	if (save_reg_num >= DDR_PARAM_ARRAY_LEN) {
		SCI_TRACE("The saved register number is more than max(%d)",
		       DDR_PARAM_ARRAY_LEN);
		return;
	}

	writel(DDR_DMC_PHY_SAVE_MAGIC, (void *)&p_save->offs);
	writel(addr_base, (void *)&p_save->val);
	p_save--;

	for (; i < ARRAY_SIZE(phy_off_val); i++) {
		writel(addr_base + phy_off_val[i].offs,
		       (void *)&p_save->offs);
		writel(phy_off_val[i].val, (void *)&p_save->val);
		p_save--;
	}

	for (i = 0; i < ARRAY_SIZE(dmc_off_val); i++) {
		writel(REG_DMC_CTRL_BASE + dmc_off_val[i].offs,
		       (void *)&p_save->offs);
		writel(dmc_off_val[i].val, (void *)&p_save->val);
		p_save--;
	}

	writel(REG_DMC_CTRL_BASE + REG_DMC_CTRL_MEMC_CMD, (void *)&p_save->offs);
	writel(DMC400_MEMC_CMD_GO, (void *)&p_save->val);
	p_save--;

	writel(REG_PWRCTRL_BASE + REG_PWRCTRL_DDR_HOLD_CTRL,
	       (void *)&p_save->offs);
	writel(0, (void *)&p_save->val);
	p_save--;

	writel(REG_SYSREG_BASE + REG_SYSREG_DDR_SLP_WAIT_NUMBER, (void *)&p_save->offs);
	writel(CP_SYSREG_N(0xc8), (void *)&p_save->val);
	p_save--;

	writel(REG_SYSREG_BASE + REG_SYSREG_DDR_SLP_CTRL_ENABLE, (void *)&p_save->offs);
	writel(CP_SYSREG_ENABLE, (void *)&p_save->val);
	p_save--;

	writel(DDR_DMC_PHY_SAVE_END, (void *)&p_save->offs);
}
#endif




/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *

**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif //End of file

