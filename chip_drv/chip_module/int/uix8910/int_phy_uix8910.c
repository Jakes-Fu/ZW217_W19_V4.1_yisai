/******************************************************************************
 ** File Name:      int_phy_v0.c                                                  *
 ** Author:         steve.zhan                                             *
 ** DATE:           10/14/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/14/2002     steve.zhan    Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "../int_phy.h"
#include "task_monitor.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Data                                              *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
LOCAL uint32 IrqUnitRegAddr[ISR_UNIT_NUM] = { (uint32)(&(hwp_gic400Reg->GICD_ISPENDRn[1])),(uint32)(&(hwp_gic400Reg->GICD_ISPENDRn[2])), (uint32)(&(hwp_gic400Reg->GICD_ISPENDRn[3]))};
PUBLIC uint32 IrqUnitRegValue[ISR_UNIT_NUM] = {0};

//define for fiq
LOCAL uint32 FiqUnitRegAddr[ISR_UNIT_NUM] = { (uint32)(&(hwp_gic400Reg->GICD_ISPENDRn[1])),(uint32)(&(hwp_gic400Reg->GICD_ISPENDRn[2])), (uint32)(&(hwp_gic400Reg->GICD_ISPENDRn[3]))};

PUBLIC uint32 FiqUnitRegValue[ISR_UNIT_NUM] = {0};

extern void *__vectors;

static void INT_PHY_IRQWholePrioritySet(void)
{
	uint32 real_priority_level;

	setIntPriority(SYS_IRQ_ID_TIMER_1_OS, 0xFF);
	real_priority_level = readIntPriority(SYS_IRQ_ID_TIMER_1_OS);
	real_priority_level &= 0xFF;
	setPriorityMask(real_priority_level);
	setIntPriority(SYS_IRQ_ID_TIMER_1_OS, 0);
}

static void INT_PHY_IRQIcfgrInitSet(void)
{
	//hwp_gic400Reg->GICD_ICFGR5 |= 

    //GICD_ICFGR5 |= (bit3 | bit13 | bit21 | bit23 | bit25);  /*id80--id95 RFSPI_CONFLICT(id81) LTE_CSIRS(id86)/LDTC1_CTRL(id90)/LDTC1_DATA(id91)/LDTC1_VIT(id92)*/     
}

static void INT_PHY_IRQIdSaveVarInit(void)
{
    //memset((uint8 *)g_irq_regs_data.SavedGICD_ENABLE, 0x0, (IRQ_ENABLE_NUM+IRQ_TARGET_NUM)*4);
}

static void INT_PHY_IRQSelectIntCpu(uint32 module_id, uint8 cpu_id)
{
	uint32 int_target_sr;
	uint8 int_target_reg, module_bit_atreg;
    uint32 int_target_regvalue;
	
	int_target_reg = module_id >> 2;
	module_bit_atreg = (module_id - (int_target_reg << 2)) << 3;
	
	int_target_sr = REG_GIC400_REG_BASE + 0x1800 + (int_target_reg << 2);
	int_target_regvalue = *(volatile uint32 *)int_target_sr;
	int_target_regvalue &= ~(0xFF << module_bit_atreg);   /* 清相应bit位为0 */
	int_target_regvalue |= (cpu_id << module_bit_atreg);
	*(volatile uint32 *)int_target_sr = int_target_regvalue;
}

static void INT_PHY_IRQIdSave(uint16 id)
{
#if 0
	uint8 reg_index, module_bit_atreg;

    /*保存CP相应IP的中断使能状态*/
	reg_index = id >> 5;
    module_bit_atreg = id % 32;
    g_irq_regs_data.SavedGICD_ENABLE[reg_index] |= (BIT_0 << module_bit_atreg);

    /*保存CP相应IP的target CP状态*/
	reg_index = id >> 2;
    module_bit_atreg = (id % 4) << 3;
    g_irq_regs_data.SavedGICD_TARGET[reg_index] &= ~(0xFF << module_bit_atreg);
    g_irq_regs_data.SavedGICD_TARGET[reg_index] |= (2 << module_bit_atreg);
#endif
}

static void INT_PHY_IRQIdNotSave(uint16 id)
{
#if 0
	uint8 reg_index, module_bit_atreg;

    /*撤销保存CP相应IP的中断使能状态*/
	reg_index = id >> 5;
    module_bit_atreg = id % 32;
    g_irq_regs_data.SavedGICD_ENABLE[reg_index] &= ~(BIT_0 << module_bit_atreg);
#endif
}

/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/, for
//       process default interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IrqHandleDefaultFunc (uint32 LogicBit)
{
    INT_PHY_IRQDisable (LogicBit);
    return (uint32) ISR_DONE;
}

/*****************************************************************************/
//  Description:    This function's source code in interrupt module/v0(x)/,
//  for process default  interrupt, this function is related chip type(platform).
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_PHY_FiqHandleDefaultFunc (uint32 LogicBit)
{
    INT_PHY_FIQDisable (LogicBit);
    return (uint32) ISR_DONE;
}
/*****************************************************************************/
//  Description:    This function is called when an IRQ happens. Is used to get
//                   interrupt registers status value.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
//  For example, the interrupt status register like this:
//
// intReg1:
//
//     bit00
//     bit01                       bit10
//     bit02                       bit11
//     bit03 ----> intReg2 bit12
//     bit04                       bit13
//
//     IrqUnitRegValue[0] = REG32(addr of intReg1);
//     IrqUnitRegValue[1] = (REG32(addr of intReg1) & (0b)01000) ? REG32(addr of intReg2) : 0;
//
/*****************************************************************************/

PUBLIC void INT_PHY_GetIrqIntStatus (void)
{
    int unitNum;

    for (unitNum = 0; unitNum < ISR_UNIT_NUM; ++unitNum)
    {
        IrqUnitRegValue[unitNum] = REG32 (IrqUnitRegAddr[unitNum]);
    }

    return;
}

PUBLIC void INT_PHY_GetFiqIntStatus (void)
{
    FiqUnitRegValue[0] = REG32 (FiqUnitRegAddr[0]);
}

void hal_IrqGlobleSet(uint8 value)
{
    if(value == 1)
    {
        enableGIC();
        enableGICProcessorInterface();   /* 使能处理器接口 */
    }
    else
    {
        disableGIC();
        disableGICProcessorInterface();   /* 不使能处理器接口 */
    }
}

void INT_PHY_VECTOR_Handle(void)
{
	uint32 temp;
	
	__asm
	{
		MRC p15, 0, temp, c1, c0, 0
	}
	temp &= ~(0x1005);
	__asm
	{
		MCR p15, 0, temp, c1, c0, 0
	}

#if 0
	//invalide all TLB entries
	__asm
	{
		MCR p15, 0, 0, c8, c7, 0
	}
#endif

	temp = (uint32)(&__vectors);    //0x80000000;   /*暂时定义*/
	__asm
	{
		MCR p15, 0, temp, c12, c0, 0
	}

#if 1	
	__asm
	{
		MRC p15, 0, temp, c1, c0, 0
	}
	temp |= (0x1005);
	__asm
	{
		MCR p15, 0, temp, c1, c0, 0
	}
#endif
}
void INT_PHY_SMP_Set(void)
{
	uint32 temp;
	__asm
	{
		MRC p15, 0, temp, c1, c0, 1
	}
	temp |= 0x40;
	__asm
	{
		MCR p15, 0, temp, c1, c0, 1
	}
}


/*****************************************************************************/
//  Description:    This function is called when system startup, only init. once.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void INT_PHY_Init (void)
{
	hal_IrqGlobleSet(0);
	INT_PHY_IRQWholePrioritySet();
	INT_PHY_IRQIcfgrInitSet();
	INT_PHY_IRQIdSaveVarInit();
	INT_PHY_IRQDisableAll();
	hal_IrqGlobleSet(1);
	INT_PHY_SMP_Set();
	INT_PHY_VECTOR_Handle();    
}

/*****************************************************************************/
//  Description:    using interrupt ligic number to clear the interrupt.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQClear (uint32 logicNum)
{

}

/*****************************************************************************/
//  Description:    Enable certain interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQEnable (uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) irq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));/*assert verified*/

    INT_DISABLE_ARM_IRQ_FIQ();
	
	enableIntID(logicNum);
    INT_PHY_IRQSelectIntCpu(logicNum + 32, 0x1);  /* int send to CPU1(ap)*/
    INT_PHY_IRQIdSave(logicNum);

    INT_ENABLE_ARM_IRQ_FIQ();

}

/*****************************************************************************/
//  Description:    Disable certain interrupt source. It is IRQ interrupt safe.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQDisable (uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) irq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));/*assert verified*/

    disableIntID(logicNum);
    INT_PHY_IRQIdNotSave(logicNum);

}

/*****************************************************************************/
//  Description:    Disable All interrupt source. It is IRQ interrupt safe.
//                       General use in system startup  and debug, and so on.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_IRQDisableAll (void)
{
    int i;
    for (i = 0; i < 0x60; ++i)
    {
		disableIntID(i);
    }

    return ;
}

/*****************************************************************************/
//  Description:    Get IRQ status, get uint1..group
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
uint32 INT_PHY_IRQGetUint1Status()
{
#if 0
    return REG32 (INT_IRQ_STS);
#else
    return 0;
#endif
}

/*****************************************************************************/
//  Description:    Get IRQ IE state
//
//  Dependency:
//  Author:         Yong.Li
//  Note:           ;
/*****************************************************************************/
uint32 INT_PHY_IRQGetIEState(void)
{
#if 0
    return REG32 (INT_IRQ_EN);
#else
    return 0;
#endif
}

/*****************************************************************************/
//  Description:    Disable the one interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_FIQEnable (uint32 logicNum)
{
    int i ;
	uint32 value;
	
    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) fiq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));/*assert verified*/

    INT_DISABLE_ARM_IRQ_FIQ();

	enableIntID(logicNum);
    INT_PHY_IRQSelectIntCpu(logicNum + 32, 0x1);  /* int send to CPU1(ap)*/
    INT_PHY_IRQIdSave(logicNum);
	value = hwp_gic400Reg->GICC_CTRL;
	value |= GIC400_REG_FIQEN;
	hwp_gic400Reg->GICC_CTRL = value;
    INT_ENABLE_ARM_IRQ_FIQ();

    return ;
}

/*****************************************************************************/
//  Description:    Disable the one interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_FIQDisable (uint32 logicNum)
{
    int i ;
	
    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) fiq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));/*assert verified*/

    disableIntID(logicNum);
    INT_PHY_IRQIdNotSave(logicNum);

    return ;
}

/*****************************************************************************/
//  Description:    Disable the one interrupt source. It is IRQ and FIQ interrupt safe
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_PHY_FIQDisableAll (void)
{
    int i;
    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
		disableIntID(irq_isr_array[i].intLogicNum);
    }

    return ;
}


PUBLIC void INT_PHYFiqDebug (void)
{
#if 0
    extern uint32 _tx_pc_val_before_fiq;
    TM_SendTestPointRequest (_tx_pc_val_before_fiq, REG32 (GR_VB_CTL));
#endif
}

PUBLIC uint32 INT_PHY_GetLisrId(void)
{
    return ((hwp_gic400Reg->GICC_IAR & 0x3FF) - 32);
}

PUBLIC void INT_PHY_CleanLisrInt(uint32 id)
{
    hwp_gic400Reg->GICC_EOIR = (id + 32);
}

PUBLIC uint8 INT_PHY_GetIrqIdStatus(uint32 int_num)
{
    return readIntPendStatus(int_num);
}

#define DIV_ROUND_UP(n,d) ((((n) + 32) + (d) - 1) / (d))
#define GIC_DIST_CTRL				0x000
#define GIC_DIST_CTR				0x004
#define GIC_DIST_IIDR				0x008
#define GIC_DIST_IGROUP				0x080
#define GIC_DIST_ENABLE_SET			0x100
#define GIC_DIST_ENABLE_CLEAR		0x180
#define GIC_DIST_PENDING_SET		0x200
#define GIC_DIST_PENDING_CLEAR		0x280
#define GIC_DIST_ACTIVE_SET			0x300
#define GIC_DIST_ACTIVE_CLEAR		0x380
#define GIC_DIST_PRI				0x400
#define GIC_DIST_TARGET				0x800
#define GIC_DIST_CONFIG				0xc00
#define GIC_DIST_SOFTINT			0xf00
#define GIC_DIST_SGI_PENDING_CLEAR	0xf10
#define GIC_DIST_SGI_PENDING_SET	0xf20

#define GICD_ENABLE			0x1
#define GICC_ENABLE			0x1
#define GICD_DISABLE			0x0
#define REG_READ(addr)			(*(volatile unsigned long *)(addr))
#define REG_WRITE(b,addr)	   {(*(volatile unsigned long *)(addr)) = b;}
#define GICD_INT_DEF_PRI		0xa0
#define GICD_INT_DEF_PRI_X4		((GICD_INT_DEF_PRI << 24) |\
					(GICD_INT_DEF_PRI << 16) |\
					(GICD_INT_DEF_PRI << 8) |\
					GICD_INT_DEF_PRI)
uint32 gicd_conf_cpid_mask[8] ={
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFF0F, //0-15 mask timer1 and timer1 os
    0xFFCFFFFF, //16-31 mask F8
    0x03F0FFFF, //32-47 mask SCI1 and SCI2,MAILBOX
    0x000003FC, //48-63 mask lte_fram1 and fram2
    0xC0000000, //64-79 idle
    0xF3FFFFFF, //80-95
};
uint32 gicd_target_cpid_mask[32] ={
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0x0000FFFF, //0-3   //timer1 and timer1 os
    0xFFFFFFFF, //4-7
    0xFFFFFFFF, //8-11
    0xFFFFFFFF, //9-15
    0xFFFFFFFF, //16-19
    0xFFFFFFFF, //20-23
    0xFF00FFFF, //24-27 //f8
    0xFFFFFFFF, //28-31
    0xFFFFFFFF, //32-35
    0xFFFFFFFF, //36-39
    0xFFFF0000, //40-43 //sci1 and sci2
    0x000000FF, //44-47 //mailbox and lte fram1 and lte fram2
    0xFFFFFF00, //48-51 //cp idle
    0x000000FF, //52-55 //gge
    0x00000000, //56-59 //gge
    0x00000000, //60-63 //gge
    0x00000000, //64-67 //gge
    0x00000000, //68-71 //gge
    0x00000000, //72-75 //gge
    0xFF000000, //76-79 //gge
    0xFFFFFFFF, //80-83
    0xFFFFFFFF, //84-87
    0xFFFFFFFF, //88-91
    0xFFFF00FF, //92-95 //axi dma
};
uint32 gicd_enable_cpid_mask[4]=
{
    0xFFFFFFFF,
    0xFBFFFFF3, //0-31
    0x001E1CFF, //32-63
    0xDFFF8000, //64-95
};
uint32 gicd_active_cpid_mask[4]=
{
    0xFFFFFFFF,
    0xFBFFFFF3, //0-31
    0x001E1CFF, //32-63
    0xDFFF8000, //64-95
};
	unsigned long saved_spi_enable[DIV_ROUND_UP(1020, 32)];
	unsigned long saved_spi_active[DIV_ROUND_UP(1020, 32)];
	unsigned long saved_spi_conf[DIV_ROUND_UP(1020, 16)];
	unsigned long saved_spi_target[DIV_ROUND_UP(1020, 4)];

PUBLIC TB_ISR INT_PHY_GetIrqFunc(uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) irq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    if (i == MAX_ISR_NUM)
    {
        return NULL;
    }

    return (irq_isr_array[i].Handlefunc);
    
}

PUBLIC uint32 INT_PHY_GetIrqIndex(uint32 logicNum)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) irq_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    return (i);
}

/*
 * Saves the GIC distributor registers during suspend or idle.  Must be called
 * with interrupts disabled but before powering down the GIC.  After calling
 * this function, no interrupts will be delivered by the GIC, and another
 * platform-specific wakeup source must be enabled.
 */
PUBLIC void INT_PHY_GicDistSuspend(void)
{
	unsigned long gic_irqs;
	unsigned long dist_base;
	int i;


	gic_irqs  = NB_SYS_IRQ;
	dist_base = 0x08201000;

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 16); i++)
		saved_spi_conf[i] =
			REG_READ(dist_base + GIC_DIST_CONFIG + i * 4);

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 4); i++)
		saved_spi_target[i] =
			REG_READ(dist_base + GIC_DIST_TARGET + i * 4);

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 32); i++)
		saved_spi_enable[i] =
			REG_READ(dist_base + GIC_DIST_ENABLE_SET + i * 4);

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 32); i++)
		saved_spi_active[i] =
			REG_READ(dist_base + GIC_DIST_ACTIVE_SET + i * 4);
}

/*
 * Restores the GIC distributor registers during resume or when coming out of
 * idle.  Must be called before enabling interrupts.  If a level interrupt
 * that occured while the GIC was suspended is still present, it will be
 * handled normally, but any edge interrupts that occured will not be seen by
 * the GIC and need to be handled by the platform-specific wakeup source.
 */


PUBLIC void INT_PHY_GicDistResume(void)
{
	unsigned int gic_irqs;
	unsigned int i;
	unsigned long dist_base;


	gic_irqs  = NB_SYS_IRQ;
	dist_base = 0x08201000;


	//writel_relaxed(GICD_DISABLE, dist_base + GIC_DIST_CTRL);

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 16); i++)
    {   
//        gic->saved_spi_conf[i] &=  gicd_conf_cpid_mask[i];   
		REG_WRITE(saved_spi_conf[i],
			dist_base + GIC_DIST_CONFIG + i * 4);
     }

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 4); i++)
    {   
		REG_WRITE(GICD_INT_DEF_PRI_X4,
			dist_base + GIC_DIST_PRI + i * 4);
    }

    
	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 4); i++)
    {
//        gic->saved_spi_target[i] &= gicd_target_cpid_mask[i];
		REG_WRITE(saved_spi_target[i],
			dist_base + GIC_DIST_TARGET + i * 4);
    }
    
	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 32); i++) {
        //saved_spi_enable[i] &= gicd_enable_cpid_mask[i];
		REG_WRITE(saved_spi_enable[i],
			dist_base + GIC_DIST_ENABLE_SET + i * 4);
	}

	for (i = 0; i < DIV_ROUND_UP(gic_irqs, 32); i++) {
        //saved_spi_active[i] &= gicd_active_cpid_mask[i];
		REG_WRITE(saved_spi_active[i],
			dist_base + GIC_DIST_ACTIVE_SET + i * 4);
	}
	REG_WRITE(0xFF, 0x08202004);
	REG_WRITE(GICD_ENABLE, dist_base + GIC_DIST_CTRL);
	REG_WRITE(GICC_ENABLE, 0x08202000);
}

#if 0
PUBLIC void INT_PHY_GicCpuSuspend(void)
{
	int i;
	u32 *ptr;
	unsigned long dist_base;
	unsigned long cpu_base;


	dist_base = 0x08201000;//gic_data_dist_base(gic);
	cpu_base  = 0x08202000;//gic_data_cpu_base(gic);



	ptr = raw_cpu_ptr(gic->saved_ppi_enable);
	for (i = 0; i < DIV_ROUND_UP(32, 32); i++)
		ptr[i] = REG_READ(dist_base + GIC_DIST_ENABLE_SET + i * 4);

	ptr = raw_cpu_ptr(gic->saved_ppi_active);
	for (i = 0; i < DIV_ROUND_UP(32, 32); i++)
		ptr[i] = REG_READ(dist_base + GIC_DIST_ACTIVE_SET + i * 4);

	ptr = raw_cpu_ptr(gic->saved_ppi_conf);
	for (i = 0; i < DIV_ROUND_UP(32, 16); i++)
		ptr[i] = REG_READ(dist_base + GIC_DIST_CONFIG + i * 4);

}

PUBLIC void INT_PHY_GicCpuResume(void)
{
	int i;
	u32 *ptr;
	unsigned long dist_base;
	unsigned long cpu_base;

	dist_base = 0x08200000;//gic_data_dist_base(gic);
	cpu_base  = 0x08202000;//gic_data_cpu_base(gic);

	ptr = raw_cpu_ptr(gic->saved_ppi_enable);
	for (i = 0; i < DIV_ROUND_UP(32, 32); i++) {
		REG_WRITE(GICD_INT_EN_CLR_X32,
			       dist_base + GIC_DIST_ENABLE_CLEAR + i * 4);
		REG_WRITE(ptr[i], dist_base + GIC_DIST_ENABLE_SET + i * 4);
	}

	ptr = raw_cpu_ptr(gic->saved_ppi_active);
	for (i = 0; i < DIV_ROUND_UP(32, 32); i++) {
		REG_WRITE(GICD_INT_EN_CLR_X32,
			       dist_base + GIC_DIST_ACTIVE_CLEAR + i * 4);
		REG_WRITE(ptr[i], dist_base + GIC_DIST_ACTIVE_SET + i * 4);
	}

	ptr = raw_cpu_ptr(gic->saved_ppi_conf);
	for (i = 0; i < DIV_ROUND_UP(32, 16); i++)
		REG_WRITE(ptr[i], dist_base + GIC_DIST_CONFIG + i * 4);

	for (i = 0; i < DIV_ROUND_UP(32, 4); i++)
		REG_WRITE(GICD_INT_DEF_PRI_X4,
					dist_base + GIC_DIST_PRI + i * 4);

	REG_WRITE(GICC_INT_PRI_THRESHOLD, cpu_base + GIC_CPU_PRIMASK);
	//gic_cpu_if_up(gic);
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
