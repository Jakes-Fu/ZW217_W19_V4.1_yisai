/******************************************************************************
 ** File Name:      emc_phy_v5.c                                                                        *
 ** Author:             Yiyue.He                                                                        *
 ** DATE:               12/17/2008                                                                              *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.                                   *
 ** Description:    This file defines emc drv -- reg opreation layer,which is platform relative         *
 **             because emc in L chip is changed                                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------------------    *
 ** DATE           NAME             DESCRIPTION                                                             *
 ** 12/17/2008   Yiyue.He           Create                                                      *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "emc_reg_v5.h"
#include "emc_drvapi.h"
#include "dal_lcd.h"
#include "normcp_spec.h"
#include "mmu_drvapi.h"
#include "../../mmu/mmu_phy.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                                           *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

typedef struct
{
    uint8       cs_num;
    uint32      cs_cfg;
    uint32      cs_scfg0;
    uint32      cs_scfg1;
    uint32      cs_scfg2;
    uint32      cs_scfg3;
    SRAM_NORFLASH_CR_SET_FUNC   cs_cr_set_func;
    uint32      crI;
    uint32      crII;
} EMC_REG_CS_CFG_GROUP;


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL EMC_REGINFO_T *s_emc_reginfo_ptr = PNULL;
#define EMC_INIT_FLAG 0x5555AAAA

#pragma arm section zidata = "EMC_REG_BUF_IN_IRAM"

LOCAL uint32 s_emc_init_flag = 0;
LOCAL EMC_REG_CS_CFG_GROUP  emc_reg_cs_cfgs[EMC_CS_NUM_MAX_SUPPORT];
LOCAL uint32 s_arm_clk_set = 0;/*lint -esym(551,s_arm_clk_set)*/

#pragma arm section zidata
extern  uint8   Load$$IRAM_FIXED$$Base;/*lint !e27 !e19 */
extern  uint8   Image$$IRAM_FIXED$$Base;/*lint !e27 !e19 */
extern  uint8   Image$$IRAM_FIXED$$Length;/*lint !e27 !e19 */
#pragma arm section code = "EMC_FUNC_IN_IRAM"

PUBLIC void EMC_Reg_Set (register uint32 cfg0, register  uint32 cfg1, register uint32 scfg0, register  uint32 scfg1)
{
    uint32 i;
    EMC_WAIT_WRITEBUFFER_DONE

    REG32 (EXT_MEM_CFG0) = cfg0;
    REG32 (EXT_MEM_CFG1) = cfg1;

    {
        uint32 cs_num =0;

        for (cs_num=0; cs_num < EMC_CS_NUM_MAX_SUPPORT; ++cs_num)
        {
            if (emc_reg_cs_cfgs[cs_num].cs_num != EMC_CS_MAX)
            {
                if (emc_reg_cs_cfgs[cs_num].cs_cr_set_func != PNULL)
                {
                    if (SRAM_CR_SET_BY_EMC != emc_reg_cs_cfgs[cs_num].cs_cr_set_func)
                    {
                        emc_reg_cs_cfgs[cs_num].cs_cr_set_func (cs_num<<26, emc_reg_cs_cfgs[cs_num].crI, emc_reg_cs_cfgs[cs_num].crII);
                    }
                    else
                    {
                        CHIP_REG_AND ( (EXT_MEM_CFG0_CS0) + (cs_num <<2), ~ (BIT_0|BIT_1));
                        CHIP_REG_OR ( (EXT_MEM_SCFG0_CS0 + (cs_num <<4)), BIT_17);
                        REG16 ( (cs_num<<26) + ( (emc_reg_cs_cfgs[cs_num].crI) <<1)) = 0;
                        REG16 ( (cs_num<<26) + ( (emc_reg_cs_cfgs[cs_num].crII) <<1)) = 0;
                        CHIP_REG_AND ( (EXT_MEM_SCFG0_CS0 + (cs_num <<4)), ~ (BIT_17));

                    }

                    for (i=0; i<10; i++) {}

                    EMC_WAIT_WRITEBUFFER_DONE
                }

                CHIP_REG_SET ( (EXT_MEM_CFG0_CS0 + (cs_num <<2)), emc_reg_cs_cfgs[cs_num].cs_cfg);

                CHIP_REG_SET ( (EXT_MEM_SCFG0_CS0 + (cs_num <<4)), emc_reg_cs_cfgs[cs_num].cs_scfg0);
                CHIP_REG_SET ( (EXT_MEM_SCFG1_CS0 + (cs_num <<4)), emc_reg_cs_cfgs[cs_num].cs_scfg1);
                CHIP_REG_SET ( (EXT_MEM_SCFG2_CS0 + (cs_num <<4)), emc_reg_cs_cfgs[cs_num].cs_scfg2);
                CHIP_REG_SET ( (EXT_MEM_SCFG3_CS0 + (cs_num <<4)), emc_reg_cs_cfgs[cs_num].cs_scfg3);
            }
        }

        {

            REG32 (EXT_MEM_CFG0_CH5) = REG32 (EXT_MEM_CFG0_CS1);
            REG32 (EXT_MEM_CFG0_CH6) = REG32 (EXT_MEM_CFG0_CS2);
            REG32 (EXT_MEM_CFG0_CH7) = REG32 (EXT_MEM_CFG0_CS3);

        }

    }

    REG32 (EXT_MEM_SCFG0) = scfg0;
    REG32 (EXT_MEM_SCFG1) = scfg1;

    //Wait MCU_clk stable
    for (i=0; i<100; i++)
    {
        cfg0 = REG32 (0x04000000) ; //this is not a register address, it is sram address
    }

}

#define FLASH_START_ADDRESS 0x00000000
#define PSRAM_START_ADDRESS 0x04000000
PUBLIC void EMC_pSRAM_init(void)
{
		uint32 i = 0;
		uint32 tmp = 0;
		uint32 crI = 0;
		uint32 crII = 0;

		MMU_DisableIDCM();
		
		CHIP_REG_SET(0x20000000, 0x222211E0);
		CHIP_REG_SET(0x20000004, 0x00008080);

		for(i = 0; i < 10; i++){}
	
		// pSRAM burst config
		//crI = 0x8191F;     // BCR: variable latency
		crI = 0x8211F;
		crII = 0x10;      // RCR
		
		// external memory config 
		CHIP_REG_OR(0x20000024, BIT_17);   //CRE eb
		*(volatile uint16 *)(PSRAM_START_ADDRESS + (crI << 1)) = 0;
		for(i = 0; i < 10; i++){}
		*(volatile uint16 *)(PSRAM_START_ADDRESS + (crII<<1)) = 0;
		for(i = 0; i < 10; i++){}
		CHIP_REG_AND(0x20000024, ~BIT_17);
		
		for(i = 0; i < 10; i++){}
		
		// EMC config
		CHIP_REG_SET(0x20000024, 0x00AC1FFF);
		CHIP_REG_SET(0x20000028, 0x015116FF);
		CHIP_REG_SET(0x2000002c, 0x00501015);
		
		CHIP_REG_SET(0x20000000, 0x222210E0);
		CHIP_REG_SET(0x20000004, 0x00008080);
		
		for(i = 0; i < 100; i++)
		{
			tmp = CHIP_REG_GET(PSRAM_START_ADDRESS);
		}	

		MMU_EnableIDCM();
}

#pragma arm section code


LOCAL void EMC_Reg_SetToAsyn (uint32 arm_clk)
{
    uint32 cs_num =0;
    BOOLEAN is_adm = SCI_FALSE;

    is_adm = ( (REG32 (EXT_MEM_SCFG0_CS1) & BIT_23) != 0);

    //call before --main so here is hard code
    if (is_adm)
    {
        emc_reg_cs_cfgs[EMC_CS0].cs_scfg0 |= BIT_23;
        emc_reg_cs_cfgs[EMC_CS1].cs_scfg0 |= BIT_23;
        emc_reg_cs_cfgs[EMC_CS3].cs_scfg0 |= BIT_23;//victor add
    }


    switch (arm_clk)
    {
        case 200:
        case 192:

            for (cs_num=0; cs_num < EMC_CS_NUM_MAX_SUPPORT; ++cs_num)
            {
                emc_reg_cs_cfgs[cs_num].cs_scfg1 = 0x00007b7b;

                if (!is_adm)
                {
                    emc_reg_cs_cfgs[cs_num].cs_scfg2 = 0x00A07200;
                }
                else
                {
                    emc_reg_cs_cfgs[cs_num].cs_scfg2 = 0x0050722a;
                }

                emc_reg_cs_cfgs[cs_num].cs_scfg3 = EMC_REG_CS_SCFG3_BASE_VALUE;
            }

            break;

        case 26:

            for (cs_num=0; cs_num < EMC_CS_NUM_MAX_SUPPORT; ++cs_num)
            {
                emc_reg_cs_cfgs[cs_num].cs_scfg1 = 0x00007475;

                if (!is_adm)
                {
                    emc_reg_cs_cfgs[cs_num].cs_scfg2 = 0x00502100;
                }
                else
                {
                    emc_reg_cs_cfgs[cs_num].cs_scfg2 = 0x0050212a;
                }

                emc_reg_cs_cfgs[cs_num].cs_scfg3 = EMC_REG_CS_SCFG3_BASE_VALUE;
            }

            break;

        default:
            return;

    }

    EMC_Reg_Set (0x00000141, 0xC, 0x00FF0000, 0x00002222);   //default value

    for (cs_num=0; cs_num < EMC_CS_NUM_MAX_SUPPORT; ++cs_num)
    {
        emc_reg_cs_cfgs[cs_num].cs_num = EMC_CS_MAX;
    }


}


PUBLIC void EMC_MemSetToDef (void)
{

    EMC_MODE_CFG_PTR emc_cs_cfg_ptr = PNULL;
    uint32 cs_num;

    for (cs_num=0; cs_num < EMC_CS_NUM_MAX_SUPPORT; ++cs_num)
    {
        emc_reg_cs_cfgs[cs_num].cs_num = cs_num;
        emc_reg_cs_cfgs[cs_num].cs_scfg0 = EMC_REG_CS_SCFG0_BASE_VALUE;
        emc_reg_cs_cfgs[cs_num].cs_cr_set_func= PNULL;

        if (EMC_CS_REG_INIT_FLAG == s_emc_reginfo_ptr[cs_num].reg_flag)
        {
            emc_cs_cfg_ptr = s_emc_reginfo_ptr[cs_num].emc_cs_cfg_ptr;

            switch (emc_cs_cfg_ptr->emc_cs_mem_type)
            {
                case EMC_MEM_FLASH:
                    {
                        FLASH_CFG_FUNC_INPARAM_T        flash_cfg_input = {0};
                        FLASH_CFG_FUNC_OUTPARAM_T       flash_cfg_output = {0};
                        NORMCP_SPEC_PTR cs_mem_info_ptr = (NORMCP_SPEC_PTR) (s_emc_reginfo_ptr[cs_num].emc_cs_mem_info_ptr);
                        flash_cfg_input.access_mode = MEM_NORMAL_MODE;

                        if (PNULL != cs_mem_info_ptr->flash_cr_cfg_func)
                        {
                            cs_mem_info_ptr->flash_cr_cfg_func (&flash_cfg_input, &flash_cfg_output);
                            emc_reg_cs_cfgs[cs_num].cs_cr_set_func = flash_cfg_output.cs_cr_set_func;
                            emc_reg_cs_cfgs[cs_num].crI = flash_cfg_output.crI;
                            emc_reg_cs_cfgs[cs_num].crII = flash_cfg_output.crII;
                        }
                        else
                        {
                            emc_reg_cs_cfgs[cs_num].cs_cr_set_func = PNULL;
                        }

                    }
                    break;
                case EMC_MEM_SRAM:
                    {
                        SRAM_CFG_FUNC_INPARAM_T     sram_cfg_input = {0};
                        SRAM_CFG_FUNC_OUTPARAM_T        sram_cfg_output = {0};
                        NORMCP_SPEC_PTR cs_mem_info_ptr = (NORMCP_SPEC_PTR) (s_emc_reginfo_ptr[cs_num].emc_cs_mem_info_ptr);
                        sram_cfg_input.access_mode = MEM_NORMAL_MODE;

                        if (PNULL != cs_mem_info_ptr->flash_cr_cfg_func)
                        {
                            cs_mem_info_ptr->sram_cr_cfg_func (&sram_cfg_input, &sram_cfg_output);
                            emc_reg_cs_cfgs[cs_num].cs_cr_set_func = sram_cfg_output.cs_cr_set_func;
                            emc_reg_cs_cfgs[cs_num].crI = sram_cfg_output.crI;
                            emc_reg_cs_cfgs[cs_num].crII = sram_cfg_output.crII;
                            s_emc_init_flag = EMC_INIT_FLAG;

                        }
                        else
                        {
                            emc_reg_cs_cfgs[cs_num].cs_cr_set_func = PNULL;
                        }

                    }
                    break;
                default:
                    break;
            }
        }

    }
}

PUBLIC void EMC_Reg_SetToDef (void)
{
    EMC_Reg_SetToAsyn (EMC_CLK_MAX);
}


//  Only call the function before call __main() function
PUBLIC void EMC_SysStartUp (void)
{
    register uint32 func_load__addr     = (uint32) &Load$$IRAM_FIXED$$Base; /*lint !e27 */
    register uint32 func_run_addr       = (uint32) EMC_Reg_Set & (~0x1);
    register uint32 func_offset             = ( (uint32) EMC_Reg_Set& (~0x1)) - (uint32) &Image$$IRAM_FIXED$$Base;/*lint !e27 */
    register uint32 i = 0;

    func_load__addr += func_offset;

    //Copy the Function to Fixed Iram area
    for (i = 0; i <= 0x130; i +=2)
    {
        * (uint16 *) (func_run_addr + i) = * (uint16 *) (func_load__addr + i);
    }

    //Delay some cycle
    for (i = 0; i < 100; i++) {}

    //arm_clk=208, ahb_clk=104    
    {
        volatile uint32 j;
        
        j = REG32 (APB_CLK_CFG0);
        
        //set ARM clk and AHB clk ratio to be 2
        j |=  CLK_AHB_DIV2_EN;
        REG32 (APB_CLK_CFG0) = j;

        //set CLK_ARM to 208MHz
        j &= ~ (CLK_MCU_SEL_MSK);      
        j |= 0x1 << CLK_MCU_SEL_SHIFT;        
        
        REG32 (APB_CLK_CFG0) = j;

        for (j=0; j<100; j++) {}

        s_arm_clk_set = EMC_CLK_MAX;
    }

    if (s_emc_init_flag != EMC_INIT_FLAG)
    {
        uint32 cs_num;

        for (cs_num=0; cs_num < EMC_CS_NUM_MAX_SUPPORT; ++cs_num)
        {
            emc_reg_cs_cfgs[cs_num].cs_num = cs_num;
            emc_reg_cs_cfgs[cs_num].cs_cfg = 0;
            emc_reg_cs_cfgs[cs_num].cs_scfg0 = EMC_REG_CS_SCFG0_BASE_VALUE;
            emc_reg_cs_cfgs[cs_num].cs_cr_set_func= PNULL;
        }
    }
    else
    {
        emc_reg_cs_cfgs[EMC_CS0].cs_num= EMC_CS0;
        emc_reg_cs_cfgs[EMC_CS0].cs_cfg = 0;
        emc_reg_cs_cfgs[EMC_CS0].cs_scfg0 = EMC_REG_CS_SCFG0_BASE_VALUE;
        emc_reg_cs_cfgs[EMC_CS0].cs_cr_set_func= PNULL;
    }

    // Set EMC to Asyn
    EMC_Reg_SetToAsyn (EMC_CLK_MAX);

    //Delay some cycle
    for (i = 0; i < 100; i++) {}
}


/*********************************************************************/
//  Description:    This function is used to calculate and set EMC Reg value
//              Input : ahb_clk \ emc access mem mode \ mem info
//              Though usually this func is used in EMC_Timing_chng which only changes clk,
//              this func is expected to more freely use to get more func. So we export this
//              interface.
//  Author:             hyy
//  Note:
/*********************************************************************/
PUBLIC void EMC_Reg_cfg (
    uint32 emc_clk
)
{
    uint32  emc_reg_cfg0 = EMC_REG_CFG0_BASE_VALUE;
    uint32  emc_reg_cfg1 = EMC_REG_CFG1_BASE_VALUE;
    uint32  emc_reg_scfg1 = EMC_REG_SCFG1_BASE_VALUE;

    EMC_REG_CS_CFG   emc_reg_cs_cfg   = {0};
    EMC_REG_SCFG0    emc_reg_scfg0    = {0};
    EMC_REG_CS_SCFG0 emc_reg_cs_scfg0 = {0};
    EMC_REG_CS_SCFG1 emc_reg_cs_scfg1 = {0};
    EMC_REG_CS_SCFG2 emc_reg_cs_scfg2 = {0};
    EMC_REG_CS_SCFG3 emc_reg_cs_scfg3 = {0};

    SRAM_NORFLASH_CR_SET_FUNC cs_cr_set_func = PNULL;
    uint32 crI = 0;
    uint32 crII = 0;


    BOOLEAN is_need_half_clk = SCI_FALSE;

    uint8 cs_num = 0;
    EMC_MODE_CFG_PTR emc_cs_cfg_ptr = PNULL;

    //@product mapping layer to save description
    uint8   r_mode_bits_value[4] = {0,1,3,2};
    uint8   w_mode_bits_value[2] = {0,3};
    uint8   len_bits_value[6]   = {7,1,2,3,4,7};
    uint8   row_len_bits_value[5] = {0,1,2,3,4};
    uint8   mem_burst_value[6] =
    {
        BURST_LEN_1 ,
        BURST_LEN_4 ,
        BURST_LEN_8 ,
        BURST_LEN_16 ,
        BURST_LEN_32 ,
        BURST_LEN_CONTINUOUS
    };

    //@need reg value temp buf because if set cs reg one by one, the cs bcr/rcr config following will fail
    for (cs_num = EMC_CS0; cs_num < EMC_CS_NUM_MAX_SUPPORT; cs_num++)
    {
        emc_reg_cs_cfgs[cs_num].cs_num = EMC_CS_MAX;
    }

    //@check clk
    if (emc_clk > 1000000)
    {
        emc_clk = emc_clk/1000000;
    }

    emc_clk = emc_clk/2;

    //  EMC_Reg_SetToAsyn(s_arm_clk_set);

    //@calc the reg value and record the value in temp buf in iram ,at last set all the value to reg at one time
    for (cs_num = EMC_CS0; cs_num < EMC_CS_NUM_MAX_SUPPORT; cs_num++)
    {
        if (EMC_CS_REG_INIT_FLAG == s_emc_reginfo_ptr[cs_num].reg_flag)
        {
            emc_cs_cfg_ptr = s_emc_reginfo_ptr[cs_num].emc_cs_cfg_ptr;
            cs_cr_set_func = PNULL;

            switch (emc_cs_cfg_ptr->emc_cs_mem_type)
            {
                case EMC_MEM_FLASH:
                    {
                        FLASH_CFG_FUNC_INPARAM_T        flash_cfg_input = {0};
                        FLASH_CFG_FUNC_OUTPARAM_T       flash_cfg_output = {0};

                        NORMCP_SPEC_PTR cs_mem_info_ptr = (NORMCP_SPEC_PTR) (s_emc_reginfo_ptr[cs_num].emc_cs_mem_info_ptr);

                        emc_reg_cs_cfg.reg_value =   EMC_REG_CS_CFG_BASE_VALUE;
                        emc_reg_scfg0.reg_value = EMC_REG_SCFG0_BASE_VALUE;
                        emc_reg_cs_scfg0.reg_value = EMC_REG_CS_SCFG0_BASE_VALUE;
                        emc_reg_cs_scfg1.reg_value = EMC_REG_CS_SCFG1_BASE_VALUE;
                        emc_reg_cs_scfg2.reg_value = EMC_REG_CS_SCFG2_BASE_VALUE;
                        emc_reg_cs_scfg3.reg_value = EMC_REG_CS_SCFG3_BASE_VALUE;



                        //@mem cfg first
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {

                            flash_cfg_input.access_mode = MEM_BURST_MODE;

                            flash_cfg_input.burst_len = (NORMCP_BURST_LEN_E) mem_burst_value[emc_cs_cfg_ptr->emc_read_len];

                            if ( (emc_clk > cs_mem_info_ptr->tFlashSynTiming.max_sync_freq) || (emc_clk > cs_mem_info_ptr->tSramSynTiming.max_sync_freq))
                            {
                                flash_cfg_input.clk = emc_clk/2;
                                is_need_half_clk = SCI_TRUE;
                            }
                            else
                            {
                                flash_cfg_input.clk = emc_clk;
                                is_need_half_clk = SCI_FALSE;
                            }

                            if (BURST_LEN_CONTINUOUS == flash_cfg_input.burst_len)
                            {
                                flash_cfg_input.is_burst_wrap = SCI_FALSE;
                            }
                            else
                            {
                                flash_cfg_input.is_burst_wrap = SCI_TRUE;
                            }

                            flash_cfg_input.is_clk_rising_edge = SCI_TRUE;
                            flash_cfg_input.is_wait_one_cycle_delay = SCI_TRUE;
                            flash_cfg_input.wait_polarity = WAIT_ACTIVE_LOW;
                        }
                        else  if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_PAGE_MODE)
                        {
                            flash_cfg_input.access_mode = MEM_PAGE_MODE;
                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_NORMAL_MODE)
                        {
                            flash_cfg_input.access_mode = MEM_NORMAL_MODE;
                        }
                        else
                        {
                            SCI_ASSERT (SCI_FALSE);/*assert verified*/
                        }

                        flash_cfg_input.base_addr_offset = EMC_CS_ADDR_SIZE * cs_num;

                        if (PNULL != cs_mem_info_ptr->flash_cr_cfg_func)
                        {
                            cs_mem_info_ptr->flash_cr_cfg_func (&flash_cfg_input, &flash_cfg_output);
                            cs_cr_set_func = flash_cfg_output.cs_cr_set_func;
                            crI = flash_cfg_output.crI;
                            crII = flash_cfg_output.crII;
                        }
                        else
                        {
                            cs_cr_set_func = PNULL;
                        }


                        //@then get mem info and cfg output info set emc reg cs cfg0-cfg3
                        //cfg
                        emc_reg_cs_cfg.bits_map.dburst_rlength = len_bits_value[emc_cs_cfg_ptr->emc_read_len];

                        emc_reg_cs_cfg.bits_map.dburst_rmode = 1;

                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_PAGE_MODE)
                        {
                            emc_reg_cs_cfg.bits_map.dburst_rmode = 0;   //wrap
                        }

                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            emc_reg_cs_cfg.bits_map.hburst_ren = 1;

                            if (flash_cfg_input.is_burst_wrap)
                            {
                                emc_reg_cs_cfg.bits_map.dburst_rmode = 0;   //wrap
                            }
                            else
                            {
                                emc_reg_cs_cfg.bits_map.dburst_rmode = 1;
                            }
                        }

                        //@cfg 0
                        emc_reg_cs_scfg0.bits_map.admux_en = (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE) ?1:0;

                        emc_reg_cs_scfg0.bits_map.mode_r = r_mode_bits_value[emc_cs_cfg_ptr->emc_read_mode];

                        //flash only aysn single mode
                        emc_reg_cs_scfg0.bits_map.mode_w = 0;

                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {

                            emc_reg_cs_scfg0.bits_map.wait_en_r = (flash_cfg_output.is_wait_used) ?0x1:0x0;

                            emc_reg_cs_scfg0.bits_map.wait_pol = 0;

                            if (flash_cfg_output.is_wait_used)
                            {
                                emc_reg_cs_scfg0.bits_map.row_detect_en_r = 0;
                            }
                            else
                            {
                                emc_reg_cs_scfg0.bits_map.row_detect_en_r = 1;
                            }

                            if (is_need_half_clk)
                            {
                                emc_reg_cs_scfg0.bits_map.clk_mode = 1;
                            }
                            else
                            {
                                emc_reg_cs_scfg0.bits_map.clk_mode = 0;
                            }
                        }

                        //@cfg 1
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            if (is_need_half_clk)
                            {
                                emc_reg_cs_scfg1.bits_map.t_first_r = MIN (2* (flash_cfg_output.first_data_latency + 1) + 2,0xf);
                            }
                            else
                            {
                                emc_reg_cs_scfg1.bits_map.t_first_r = MIN ( (flash_cfg_output.first_data_latency +2 +1),0xf);
                            }

                            if (!is_need_half_clk)
                            {
                                emc_reg_cs_scfg1.bits_map.t_next_r = 1;
                            }
                            else
                            {
                                emc_reg_cs_scfg1.bits_map.t_next_r = 2;
                            }
                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_PAGE_MODE)
                        {
                            emc_reg_cs_scfg1.bits_map.t_first_r = MIN ( ( ( (cs_mem_info_ptr->tFlashAsynReadTiming.page_rd_access_time + EMC_DELAY_IN_CHIP) * emc_clk) /1000 + 1), 0xf);
                            emc_reg_cs_scfg1.bits_map.t_next_r = MIN ( ( ( (cs_mem_info_ptr->tFlashAsynReadTiming.page_intra_cycle_time + EMC_DELAY_IN_CHIP) * emc_clk) /1000 + 1), 0xf);

                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_NORMAL_MODE)
                        {
                            emc_reg_cs_scfg1.bits_map.t_first_r = MIN ( ( ( (cs_mem_info_ptr->tFlashAsynReadTiming.rd_cycle_time + EMC_DELAY_IN_CHIP) * emc_clk) /1000 + 1), 0xf);
                        }

                        emc_reg_cs_scfg1.bits_map.t_first_w =  MIN ( (cs_mem_info_ptr->tFlashAsynWriteTiming.wr_cycle_time*emc_clk/1000 + 2), 0xf);

                        //@cfg2
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            if (!is_need_half_clk)
                            {
                                emc_reg_cs_scfg2.bits_map.t_adv_wdt_r = 1;

                                if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                                {
                                    emc_reg_cs_scfg2.bits_map.t_adswt_r = 1;
                                    emc_reg_cs_scfg2.bits_map.t_oe_stp_r = 1;
                                }
                            }
                            else
                            {
                                emc_reg_cs_scfg2.bits_map.t_adv_wdt_r = 2;

                                if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                                {
                                    emc_reg_cs_scfg2.bits_map.t_adswt_r = 2;
                                    emc_reg_cs_scfg2.bits_map.t_oe_stp_r = 2;
                                }
                            }
                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_PAGE_MODE)
                        {
                            emc_reg_cs_scfg2.bits_map.t_adv_wdt_r = 1;
                            emc_reg_cs_scfg2.bits_map.t_adv_mode_r = 1;
                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_NORMAL_MODE)
                        {
                            emc_reg_cs_scfg2.bits_map.t_adv_wdt_r = MIN ( (cs_mem_info_ptr->tFlashAsynReadTiming.rd_adv_pulse_time * emc_clk/1000 + 1), 0x3);

                            if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                            {
                                emc_reg_cs_scfg2.bits_map.t_adswt_r = emc_reg_cs_scfg2.bits_map.t_adv_wdt_r + 1;
                            }

                        }

                        emc_reg_cs_scfg2.bits_map.t_adv_wdt_w = MIN ( (cs_mem_info_ptr->tFlashAsynWriteTiming.wr_adv_pulse_time * emc_clk/1000 + 1), 0x3);

                        if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                        {
                            emc_reg_cs_scfg2.bits_map.t_adswt_w = emc_reg_cs_scfg2.bits_map.t_adv_wdt_w + 1;
                        }

                        emc_reg_cs_scfg2.bits_map.t_we_stp_w = MIN ( (emc_reg_cs_scfg2.bits_map.t_adv_wdt_w), 0x7);/*lint !e650 confirmed by yong.li*/
                        emc_reg_cs_scfg2.bits_map.t_we_wdt_w = MIN ( (emc_reg_cs_scfg1.bits_map.t_first_w - emc_reg_cs_scfg2.bits_map.t_we_stp_w-1), 0xf);

                        //cfg3
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            if (!is_need_half_clk)
                            {
                                emc_reg_cs_scfg3.bits_map.t_sample_stp = 1;
                                emc_reg_cs_scfg3.bits_map.t_wait_dly_r = 1;
                            }
                            else
                            {
                                emc_reg_cs_scfg3.bits_map.t_valid_phase_r = 2;
                                emc_reg_cs_scfg3.bits_map.t_valid_phase_w = 2;
                                emc_reg_cs_scfg3.bits_map.t_sample_stp = 2;
                                emc_reg_cs_scfg3.bits_map.t_wait_dly_r = 2;
                                emc_reg_cs_scfg3.bits_map.t_wait_dly_w = 2;
                            }

                            emc_reg_cs_scfg3.bits_map.t_sample_dly_r = 1;
                            emc_reg_cs_scfg3.bits_map.t_sample_dly_w = 1;

                        }

                        //common cfg
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            emc_reg_scfg0.bits_map.clksmem1_out_en = 1;

                            if (is_need_half_clk)
                            {
                                emc_reg_scfg0.bits_map.clksmem1_out_mode = 1;
                                emc_reg_scfg0.bits_map.clksmem1_out_sel = 1;
                            }

                        }
                    }
                    break;

                case EMC_MEM_SRAM:
                    {
                        SRAM_CFG_FUNC_INPARAM_T     sram_cfg_input = {0};
                        SRAM_CFG_FUNC_OUTPARAM_T        sram_cfg_output = {0};

                        NORMCP_SPEC_PTR cs_mem_info_ptr = (NORMCP_SPEC_PTR) (s_emc_reginfo_ptr[cs_num].emc_cs_mem_info_ptr);

                        emc_reg_cs_cfg.reg_value =   EMC_REG_CS_CFG_BASE_VALUE;
                        emc_reg_cs_scfg0.reg_value = EMC_REG_CS_SCFG0_BASE_VALUE;
                        emc_reg_cs_scfg1.reg_value = EMC_REG_CS_SCFG1_BASE_VALUE;
                        emc_reg_cs_scfg2.reg_value = EMC_REG_CS_SCFG2_BASE_VALUE;
                        emc_reg_cs_scfg3.reg_value = EMC_REG_CS_SCFG3_BASE_VALUE;

                        //@mem cfg first
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            sram_cfg_input.access_mode = MEM_BURST_MODE;

                            sram_cfg_input.burst_len = (NORMCP_BURST_LEN_E) mem_burst_value[emc_cs_cfg_ptr->emc_read_len];

                            if ( (emc_clk > cs_mem_info_ptr->tSramSynTiming.max_sync_freq) || (emc_clk > cs_mem_info_ptr->tFlashSynTiming.max_sync_freq))
                            {
                                sram_cfg_input.clk  = emc_clk/2;
                                is_need_half_clk = SCI_TRUE;
                            }
                            else
                            {
                                sram_cfg_input.clk = emc_clk;
                                is_need_half_clk = SCI_FALSE;
                            }

                            if (BURST_LEN_CONTINUOUS == sram_cfg_input.burst_len)
                            {
                                sram_cfg_input.is_burst_wrap = SCI_FALSE;
                            }
                            else
                            {
                                sram_cfg_input.is_burst_wrap = SCI_TRUE;
                            }

                            sram_cfg_input.is_clk_rising_edge = SCI_TRUE;
                            sram_cfg_input.is_wait_one_cycle_delay = SCI_TRUE;
                            sram_cfg_input.wait_polarity = WAIT_ACTIVE_LOW;
                        }
                        else  if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_PAGE_MODE)
                        {
                            sram_cfg_input.access_mode = MEM_PAGE_MODE;
                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_NORMAL_MODE)
                        {
                            sram_cfg_input.access_mode = MEM_NORMAL_MODE;
                        }
                        else
                        {
                            SCI_ASSERT (SCI_FALSE);/*assert verified*/
                        }

                        sram_cfg_input.base_addr_offset = EMC_CS_ADDR_SIZE * cs_num;

                        if (PNULL != cs_mem_info_ptr->sram_cr_cfg_func)
                        {
                            cs_mem_info_ptr->sram_cr_cfg_func (&sram_cfg_input, &sram_cfg_output);
                            cs_cr_set_func = sram_cfg_output.cs_cr_set_func;
                            crI = sram_cfg_output.crI;
                            crII = sram_cfg_output.crII;
                        }
                        else
                        {
                            cs_cr_set_func = PNULL;
                        }

                        //@then get mem info and cfg output info set emc reg cs cfg0-cfg3
                        //cfg
                        emc_reg_cs_cfg.bits_map.dburst_rlength = len_bits_value[emc_cs_cfg_ptr->emc_read_len];

                        emc_reg_cs_cfg.bits_map.dburst_wlength = len_bits_value[emc_cs_cfg_ptr->emc_write_len];

                        emc_reg_cs_cfg.bits_map.dburst_rmode = 1;

                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            emc_reg_cs_cfg.bits_map.hburst_ren = 1;
                            //emc_reg_cs_cfg.bits_map.hburst_wen = 1;

                            if (sram_cfg_input.is_burst_wrap)
                            {
                                emc_reg_cs_cfg.bits_map.dburst_rmode = 0;   //wrap
                                emc_reg_cs_cfg.bits_map.dburst_wmode = 0;   //wrap
                            }
                            else
                            {
                                emc_reg_cs_cfg.bits_map.dburst_rmode = 1;
                                emc_reg_cs_cfg.bits_map.dburst_wmode = 1;
                            }
                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_PAGE_MODE)
                        {
                            emc_reg_cs_cfg.bits_map.dburst_rmode = 0;   //wrap
                        }

                        //cfg 0
                        emc_reg_cs_scfg0.bits_map.admux_en = (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE) ?1:0;

                        emc_reg_cs_scfg0.bits_map.mode_r = r_mode_bits_value[emc_cs_cfg_ptr->emc_read_mode];

                        emc_reg_cs_scfg0.bits_map.mode_w = w_mode_bits_value[emc_cs_cfg_ptr->emc_write_mode];

                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            if (sram_cfg_output.is_wait_used)
                            {
                                emc_reg_cs_scfg0.bits_map.wait_en_r = 1;
                                emc_reg_cs_scfg0.bits_map.wait_en_w = 1;
                                //@start for row cross not support sram
                                emc_reg_cs_scfg0.bits_map.row_detect_en_w = 1;
                                emc_reg_cs_scfg0.bits_map.row_detect_en_r = 1;
                                emc_reg_cs_scfg0.bits_map.row_length = row_len_bits_value[cs_mem_info_ptr->sram_row_boundary_len];
                                //@end
                            }
                            else
                            {
                                emc_reg_cs_scfg0.bits_map.wait_en_r = 0;
                                emc_reg_cs_scfg0.bits_map.wait_en_w = 0;
                                emc_reg_cs_scfg0.bits_map.row_detect_en_w = 1;
                                emc_reg_cs_scfg0.bits_map.row_detect_en_r = 1;
                                emc_reg_cs_scfg0.bits_map.row_length = row_len_bits_value[cs_mem_info_ptr->sram_row_boundary_len];
                            }

                            emc_reg_cs_scfg0.bits_map.wait_pol = 0;

                            if (sram_cfg_output.is_wait_used)
                            {
                                emc_reg_cs_scfg0.bits_map.row_detect_en_r = 0;
                                emc_reg_cs_scfg0.bits_map.row_detect_en_w = 0;
                            }
                            else
                            {
                                emc_reg_cs_scfg0.bits_map.row_detect_en_r = 1;
                                emc_reg_cs_scfg0.bits_map.row_detect_en_w = 1;
                            }

                            if (is_need_half_clk)
                            {
                                emc_reg_cs_scfg0.bits_map.clk_mode = 1;
                            }
                            else
                            {
                                emc_reg_cs_scfg0.bits_map.clk_mode = 0;
                            }
                        }

                        //@cfg 1
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            if (is_need_half_clk)
                            {
                                emc_reg_cs_scfg1.bits_map.t_first_r = MIN (2* (sram_cfg_output.first_data_latency + 1) + 2,0xf);
                            }
                            else
                            {
                                emc_reg_cs_scfg1.bits_map.t_first_r = MIN ( (sram_cfg_output.first_data_latency +2 +1),0xf);
                            }

                            if (!is_need_half_clk)
                            {
                                emc_reg_cs_scfg1.bits_map.t_next_r = 1;
                            }
                            else
                            {
                                emc_reg_cs_scfg1.bits_map.t_next_r = 2;
                            }

                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_PAGE_MODE)
                        {
                            emc_reg_cs_scfg1.bits_map.t_first_r = MIN ( ( ( (cs_mem_info_ptr->tSramAsynReadTiming.page_rd_access_time + EMC_DELAY_IN_CHIP) * emc_clk) /1000 + 1),0xf);
                            emc_reg_cs_scfg1.bits_map.t_next_r =  MIN ( ( ( (cs_mem_info_ptr->tSramAsynReadTiming.page_intra_cycle_time + EMC_DELAY_IN_CHIP) * emc_clk) /1000 + 1),0xf);
                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_NORMAL_MODE)
                        {
                            emc_reg_cs_scfg1.bits_map.t_first_r = MIN ( ( ( (cs_mem_info_ptr->tSramAsynReadTiming.rd_cycle_time + EMC_DELAY_IN_CHIP) * emc_clk) /1000 + 1),0xf);
                        }

                        if (emc_cs_cfg_ptr->emc_write_mode == EMC_WRITE_BURST_MODE)
                        {
                            if (is_need_half_clk)
                            {
                                emc_reg_cs_scfg1.bits_map.t_first_w = MIN (2* (sram_cfg_output.first_data_latency + 1) + 2,0xf);
                            }
                            else
                            {
                                emc_reg_cs_scfg1.bits_map.t_first_w = MIN ( (sram_cfg_output.first_data_latency +2 +1),0xf);
                            }

                            if (!is_need_half_clk)
                            {
                                emc_reg_cs_scfg1.bits_map.t_next_w = 1;
                            }
                            else
                            {
                                emc_reg_cs_scfg1.bits_map.t_next_w = 2;
                            }
                        }
                        else if (emc_cs_cfg_ptr->emc_write_mode == EMC_WRITE_NORMAL_MODE)
                        {
                            emc_reg_cs_scfg1.bits_map.t_first_w =  MIN ( (cs_mem_info_ptr->tSramAsynWriteTiming.wr_cycle_time*emc_clk/1000 + 1),0xf);
                        }

                        //@cfg2
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            if (!is_need_half_clk)
                            {
                                emc_reg_cs_scfg2.bits_map.t_adv_wdt_r = 1;

                                if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                                {
                                    emc_reg_cs_scfg2.bits_map.t_adswt_r = 1;
                                    emc_reg_cs_scfg2.bits_map.t_oe_stp_r = 1;
                                }
                                else
                                {
                                    emc_reg_cs_scfg2.bits_map.t_oe_stp_r = 0;
                                    emc_reg_cs_scfg2.bits_map.t_adswt_r = 0;
                                }
                            }
                            else
                            {
                                emc_reg_cs_scfg2.bits_map.t_adv_wdt_r = 2;

                                if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                                {
                                    emc_reg_cs_scfg2.bits_map.t_adswt_r = 2;
                                    emc_reg_cs_scfg2.bits_map.t_oe_stp_r = 2;
                                }
                                else
                                {
                                    emc_reg_cs_scfg2.bits_map.t_oe_stp_r = 0;
                                    emc_reg_cs_scfg2.bits_map.t_adswt_r = 0;
                                }
                            }

                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_PAGE_MODE)
                        {
                            emc_reg_cs_scfg2.bits_map.t_adv_wdt_r = 1;
                            emc_reg_cs_scfg2.bits_map.t_adv_mode_r = 1;
                        }
                        else if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_NORMAL_MODE)
                        {
                            emc_reg_cs_scfg2.bits_map.t_adv_wdt_r = MIN ( (cs_mem_info_ptr->tSramAsynReadTiming.rd_adv_pulse_time * emc_clk/1000 + 1),0x3);

                            if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                            {
                                emc_reg_cs_scfg2.bits_map.t_adswt_r = emc_reg_cs_scfg2.bits_map.t_adv_wdt_r + 1;
                            }
                        }

                        if (emc_cs_cfg_ptr->emc_write_mode == EMC_WRITE_BURST_MODE)
                        {
                            if (!is_need_half_clk)
                            {
                                emc_reg_cs_scfg2.bits_map.t_adv_wdt_w = 1;
                                emc_reg_cs_scfg2.bits_map.t_we_wdt_w = 1;

                                if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                                {
                                    emc_reg_cs_scfg2.bits_map.t_adswt_w = 1;
                                }
                                else
                                {
                                    emc_reg_cs_scfg2.bits_map.t_adswt_w = 0;
                                }
                            }
                            else
                            {
                                emc_reg_cs_scfg2.bits_map.t_adv_wdt_w = 2;
                                emc_reg_cs_scfg2.bits_map.t_we_wdt_w = 2;

                                if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                                {
                                    emc_reg_cs_scfg2.bits_map.t_adswt_w = 2;
                                }
                                else
                                {
                                    emc_reg_cs_scfg2.bits_map.t_adswt_w = 0;
                                }
                            }

                            emc_reg_cs_scfg2.bits_map.t_we_stp_w = 0;
                        }
                        else if (emc_cs_cfg_ptr->emc_write_mode == EMC_WRITE_NORMAL_MODE)
                        {
                            emc_reg_cs_scfg1.bits_map.t_first_w =  MIN ( (cs_mem_info_ptr->tSramAsynWriteTiming.wr_cycle_time*emc_clk/1000 + 1),0xf);
                            emc_reg_cs_scfg2.bits_map.t_adv_wdt_w = MIN ( (cs_mem_info_ptr->tSramAsynWriteTiming.wr_adv_pulse_time * emc_clk/1000 + 1),0x3);

                            if (cs_mem_info_ptr->ad_mode == NORMCP_ADM_MODE)
                            {
                                emc_reg_cs_scfg2.bits_map.t_adswt_w = emc_reg_cs_scfg2.bits_map.t_adv_wdt_w + 1;
                            }

                            if (cs_mem_info_ptr->tSramAsynWriteTiming.wr_ce_stp_time == 0)
                            {
                                emc_reg_cs_scfg2.bits_map.t_we_stp_w = 0;
                            }
                            else
                            {
                                emc_reg_cs_scfg2.bits_map.t_we_stp_w = MIN ( (cs_mem_info_ptr->tSramAsynWriteTiming.wr_ce_stp_time * emc_clk/1000 + 1),0x7);
                            }

                            emc_reg_cs_scfg2.bits_map.t_we_wdt_w = MIN ( (emc_reg_cs_scfg1.bits_map.t_first_w - emc_reg_cs_scfg2.bits_map.t_we_stp_w -1),0xf);
                        }

                        //cfg3
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            if (is_need_half_clk)
                            {
                                emc_reg_cs_scfg3.bits_map.t_valid_phase_r = 2;
                                emc_reg_cs_scfg3.bits_map.t_valid_phase_w = 2;
                                emc_reg_cs_scfg3.bits_map.t_sample_stp = 2;
                                emc_reg_cs_scfg3.bits_map.t_wait_dly_r = 2;
                                emc_reg_cs_scfg3.bits_map.t_wait_dly_w = 2;
                            }
                            else
                            {
                                emc_reg_cs_scfg3.bits_map.t_valid_phase_r = 0;
                                emc_reg_cs_scfg3.bits_map.t_valid_phase_w = 0;
                                emc_reg_cs_scfg3.bits_map.t_sample_stp = 1;
                                //emc_reg_cs_scfg3.bits_map.t_sample_stp = 2;
                                emc_reg_cs_scfg3.bits_map.t_wait_dly_r = 1;
                            }

                            emc_reg_cs_scfg3.bits_map.t_sample_dly_r = 1;
                            emc_reg_cs_scfg3.bits_map.t_sample_dly_w = 1;
                            emc_reg_cs_scfg3.bits_map.t_wait_ie_stp = 1;
                        }

                        //common cfg
                        if (emc_cs_cfg_ptr->emc_read_mode == EMC_READ_BURST_MODE)
                        {
                            emc_reg_scfg0.bits_map.clksmem1_out_en = 1;

                            if (is_need_half_clk)
                            {
                                emc_reg_scfg0.bits_map.clksmem1_out_mode = 1;
                                emc_reg_scfg0.bits_map.clksmem1_out_sel = 1;
                            }

                        }
                    }
                    break;

                default:
                    SCI_ASSERT (SCI_FALSE);/*assert verified*/
                    break;
            }

            emc_reg_cs_cfgs[cs_num].cs_num = cs_num;
            emc_reg_cs_cfgs[cs_num].cs_cfg = emc_reg_cs_cfg.reg_value;
            emc_reg_cs_cfgs[cs_num].cs_scfg0 = emc_reg_cs_scfg0.reg_value;
            emc_reg_cs_cfgs[cs_num].cs_scfg1 = emc_reg_cs_scfg1.reg_value;
            emc_reg_cs_cfgs[cs_num].cs_scfg2 = emc_reg_cs_scfg2.reg_value;
            emc_reg_cs_cfgs[cs_num].cs_scfg3 = emc_reg_cs_scfg3.reg_value;
            emc_reg_cs_cfgs[cs_num].cs_cr_set_func = cs_cr_set_func;
            emc_reg_cs_cfgs[cs_num].crI = crI;
            emc_reg_cs_cfgs[cs_num].crII    = crII;
        }
    }

    s_arm_clk_set = emc_clk*2;

    //set turn-around cycle to 2 at 100M burst mode, and set to 1 at 50M burst mode
    if (emc_clk > 66) // 1/15ns = 66.7M
    {
        emc_reg_scfg1 &= ~0x0000FFFF;
        emc_reg_scfg1 |= 0x00002222;
    }

    MMU_DisableIDCM();
    //@after all the cs regs have been calced ,now set.
    EMC_Reg_Set (emc_reg_cfg0,emc_reg_cfg1,emc_reg_scfg0.reg_value,emc_reg_scfg1);

    MMU_EnableIDCM();

    EMC_MemSetToDef();
}

PUBLIC void EMC_Reg_If_Init (EMC_REGINFO_T *emc_reginfo_ptr)
{

    s_emc_reginfo_ptr = emc_reginfo_ptr;
}

PUBLIC void EMC_SetARMAHBClock (uint32 clk)
{
    EMC_Timing_chng (clk);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

