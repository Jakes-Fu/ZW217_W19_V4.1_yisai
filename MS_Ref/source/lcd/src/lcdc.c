/***************************************************************************************
** File Name:      SC8800H_lcdc.c                                                          *
** DATE:           19/09/2005                                                          *
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.                   *
** Description:    this file defines LCDC interfaces,                                  *
**                 these interfaces is visible for LCDC's user                         *
****************************************************************************************

****************************************************************************************
**                         Edit History                                                *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                                                 *
** 19/09/2005              Create.                                                     *
****************************************************************************************/


/**------------------------------------------------------------------------------------*
 **                         Dependencies                                               *
 **------------------------------------------------------------------------------------*/

#include "ms_ref_lcd_trc.h"
#include "lcdc_data_type.h"
#include "lcdc_common.h"
//#include "chip_plf_export.h"
#include "sc_reg.h"
#include "os_api.h"
#include "tb_comm.h"
#include "tb_drv.h"
#include "ref_outport.h"
#include "dal_lcd.h"
#include "power_manager.h"
#include "chip.h"
/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                                              *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define LCDC_INIT_CB_NUMS   3
#define LCDC_TV_CLOCK       27000000

#define REG32(x)      (*((volatile uint32 *)(x)))
extern uint32 OS_GetCurrentTime (void);
extern void LCD_GetLCDCSemaphore (void);
extern void LCD_PutLCDCSemaphore (void);
extern void GPIO_ResetLcd (BOOLEAN pulse_level, uint32  delay_ms);
/****************************************************************************************/
// Global Variable
/****************************************************************************************/
uint16                  g_tv_mode = 0;           //if LCDC tv mode,g_tv_mode = 1,else g_tv_mode = 0;
uint32                  g_vpll_reg_val;
LCDC_TYPE_T             g_lcdc_type;
volatile BLOCK_ACCESS_T g_lcd_blk[6];
volatile uint32         g_use_blk_num;   //block sum that is used
LCDC_MCU_ADDR_T         g_lcdc_mcu_addr[2];
lcdc_func_t             g_lcdc_int_list[LCDC_INIT_CB_NUMS];
LCDC_CONFIG_T           g_lcdc_cfg;
/****************************************************************************************/
uint32 g_lcdc_on = 0;

LOCAL PLL_DEVICE_REQ_T  s_PllDevice_Req_Lcdc;
LOCAL uint32            lcm_sel= 1;   //lcm1:1,lcm0:0,abort:other

/**********************************************************************************************************/
// Description: set LCDC access lcm0 or lcm1
// Global resource dependence: none
// Author:
// Note:lcm0:lcm_interface_select=LCD_CTRL_MODULE_LCDC_LCM0,other lcm1
/**********************************************************************************************************/
LOCAL void LCDC_LCM_Sel (uint32 lcm_interface_select)
{
    uint32 lcdc_lcd_mode;
    PWRMNG_LCDC_ClkSwtch (TRUE);   //lcdc_en
    lcm_sel = (LCD_CTRL_MODULE_LCDC_LCM0 == lcm_interface_select) ?0:1;
    lcdc_lcd_mode = TB_REG_GET (LCDC_LCD_MODE);

    if (0 == lcm_sel)
    {
        lcdc_lcd_mode &= ~BIT_11;
    }
    else
    {
        lcdc_lcd_mode |= BIT_11;
    }

    TB_REG_SET (LCDC_LCD_MODE, lcdc_lcd_mode);
}
/**********************************************************************************************************/
// Description: set LCDC work mode and lcd infomation
// Global resource dependence: g_lcdc_type
// Author: Jianping.wang
// Note:
//      p_lcdc - pointer to info structure
/**********************************************************************************************************/
PUBLIC void LCDC_SetLcdType (LCDC_TYPE_T *lcdc_ptr, uint32 lcd_cs)
{
    g_lcdc_type = *lcdc_ptr;
    LCDC_LCM_Sel (lcdc_ptr->controller);

    switch (lcdc_ptr->lcdc_mode)
    {
        case 0:
            break;
        case 1:

            if (0 == lcd_cs)
            {
                g_lcdc_mcu_addr[0].cmd_addr = LCDC_LCM_CS0_ADDR0;
                g_lcdc_mcu_addr[0].data_addr = LCDC_LCM_CS0_ADDR1;
                g_lcdc_mcu_addr[1].cmd_addr = LCDC_LCM_CS1_ADDR0;
                g_lcdc_mcu_addr[1].data_addr = LCDC_LCM_CS1_ADDR1;
            }
            else if (1 == lcd_cs)
            {
                g_lcdc_mcu_addr[0].cmd_addr = LCDC_LCM_CS1_ADDR0;
                g_lcdc_mcu_addr[0].data_addr = LCDC_LCM_CS1_ADDR1;
                g_lcdc_mcu_addr[1].cmd_addr = LCDC_LCM_CS0_ADDR0;
                g_lcdc_mcu_addr[1].data_addr = LCDC_LCM_CS0_ADDR1;
            }

            break;
        default:
            break;
    }

}//LCDC_SetLcdType
/**********************************************************************************************************/
// Description: initialize malloc block information
// Global resource dependence: g_lcd_blk
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
LOCAL void lcdc_init_blk (void)
{
    uint32 i = 0;
    uint32 mask = 0x11;

    for (i=0 ; i<LCD_BLOCK_NUM ; i++)
    {
        g_lcd_blk[i].blk_num  = i;
        g_lcd_blk[i].blk_used = 0;
        g_lcd_blk[i].blk_mask = mask;
        mask += 0x11;
    }

    g_use_blk_num = 0;
}//end lcdc_init_blk;
/**********************************************************************************************************/
// Description: malloc lcd display block
// Global resource dependence: g_lcd_blk,g_use_blk_num
// Author: Jianping.wang
// Note:
//      input:
//          none
//      output:
//          p_blknum-pointer to block number,if success
//          p_mask - pointer to access mask,if success
//      return:
//          LCD_OK,if success
//          LCD_ERROR_MALLOC_BLOCK, malloc fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_MallocLcdBlock (uint32 *blknum_ptr, uint32 *mask_ptr)
{
    int i = 0;

    if ( (blknum_ptr == PNULL) || (mask_ptr == PNULL))
    {
        return LCD_ERROR_POINTER_NULL;
    }

    if (g_use_blk_num >= LCD_BLOCK_NUM)
    {
        *blknum_ptr = LCD_BLOCK_NUM;
        return LCD_ERROR_MALLOC_BLOCK;
    }

    for (i=0 ; i<LCD_BLOCK_NUM ; i++)
    {
        if (!g_lcd_blk[i].blk_used)
        {
            g_lcd_blk[i].blk_used = 1;
            *blknum_ptr = i;
            *mask_ptr = g_lcd_blk[i].blk_mask;
            g_use_blk_num++;
            break;
        }
    }

    return LCD_OK;
}//end of LCDC_MallocLcdBlock
/**********************************************************************************************************/
// Description: free lcd display block
// Global resource dependence: g_lcd_blk,g_use_blk_num
// Author: Jianping.wang
// Note:
//      input:
//          p_mask - pointer to access mask
//          block_num - block index,0~5
//      return:
//          LCD_OK,if success
//          other, if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_FreeLcdBlock (uint32 block_num,uint32 *mask_ptr)
{
    LCDC_LCD_MODE_U    *lcdc_mode_ptr = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;
    uint32 reg_addr;

    if (mask_ptr == PNULL)
    {
        return LCD_ERROR_POINTER_NULL;
    }

    if (block_num > 5)
    {
        return LCD_ERROR_BLOCK_NUMBER;
    }

    if (g_lcd_blk[block_num].blk_mask != *mask_ptr)
    {
        return LCD_ERROR_ACCESS_MASK;
    }

    reg_addr = (uint32) LCDC_LCD_BLOCK0ADDR;
    reg_addr += block_num << 4;
    reg_addr += 0x4;

    //disable this block
    TB_REG_SET (reg_addr,0x0);

    g_use_blk_num -= 1;

    g_lcd_blk[block_num].blk_used = 0;
    g_lcd_blk[block_num].blk_mask += 0x66;

    if (g_lcd_blk[block_num].blk_mask > 0xffff)
    {
        g_lcd_blk[block_num].blk_mask = 0x11 * (block_num + 1);
    }

    lcdc_mode_ptr->mBits.shadow_update = 0x1;

    return LCD_OK;
}//end of LCDC_FreeLcdBlock
/**********************************************************************************************************/
// Description: set lcd display block information
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      operation successes,provided use correct access mask
//      input:
//          p_blk - pointer to display block information
//          p_mask - pointer to block access mask
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SetLcdBlock (LCD_BLOCK_T *blk_ptr,uint32 *mask_ptr)
{
    uint32 reg_addr;
    uint32 blk_width;
    uint32 num;

    SCI_ASSERT (blk_ptr != PNULL);
    num = blk_ptr->blk_num;

    SCI_ASSERT (num<=5);

    if (blk_ptr->blk_start.mBits.enable == 0)
    {
        reg_addr = (uint32) LCDC_LCD_BLOCK0ADDR;
        reg_addr += blk_ptr->blk_num << 4;
        reg_addr += 0x4;
        blk_ptr->blk_start.mBits.res = 0x0;
        TB_REG_SET (reg_addr,0);
        return LCD_OK;
    }

    blk_width = blk_ptr->blk_end.mBits.end_x - blk_ptr->blk_start.mBits.start_x + 1;


    //  SCI_ASSERT(blk_ptr->blk_start.mBits.start_x >= 0);
    //  SCI_ASSERT(blk_ptr->blk_start.mBits.start_y >= 0);
    //  SCI_ASSERT(blk_ptr->blk_end.mBits.end_x >= 0);
    //  SCI_ASSERT(blk_ptr->blk_end.mBits.end_y >= 0);
    SCI_ASSERT (blk_ptr->blk_config.mBits.resolution < 0x3);
    SCI_ASSERT ( (blk_ptr->blk_end.mBits.width<<3) >= blk_width);
    SCI_ASSERT (blk_ptr->blk_end.mBits.end_x > blk_ptr->blk_start.mBits.start_x);
    SCI_ASSERT (blk_ptr->blk_end.mBits.end_y >= blk_ptr->blk_start.mBits.start_y);


    reg_addr = (uint32) LCDC_LCD_BLOCK0ADDR;
    reg_addr += blk_ptr->blk_num << 4;

    SCI_ASSERT ( (blk_ptr->sdram_addr%8) == 0);
    TB_REG_SET (reg_addr,blk_ptr->sdram_addr);
    reg_addr += 0x4;

    blk_ptr->blk_start.mBits.res = 0x0;
    TB_REG_SET (reg_addr,blk_ptr->blk_start.value);
    reg_addr += 0x4;

    TB_REG_SET (reg_addr,blk_ptr->blk_end.value);
    reg_addr += 0x4;

    //default set it in drv now, compatible with SC6800
    blk_ptr->blk_config.mBits.alpha_sel = 1;

    blk_ptr->blk_config.mBits.res = 0x0;
    TB_REG_SET (reg_addr,blk_ptr->blk_config.value);

    return LCD_OK;
}//end of LCDC_SetLcdBlock
/**********************************************************************************************************/
// Description: LCDC module handle interrupt
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_irq (uint32 param)
{
    uint32 mode;
    LCDC_LCD_INT_STATUS_U  *int_stat_ptr     = (LCDC_LCD_INT_STATUS_U *) LCDC_LCD_INT_STATUS;
    LCDC_LCD_INT_CLEAR_U   *int_clear_ptr    = (LCDC_LCD_INT_CLEAR_U *) LCDC_LCD_INT_CLEAR;
    LCDC_LCM_INT_VECTOR_U  *int_vec_ptr      = (LCDC_LCM_INT_VECTOR_U *) LCDC_LCM_INT_VECTOR;
    LCDC_LCM_INT_STATUS_U  *int_mcu_stat_ptr = (LCDC_LCM_INT_STATUS_U *) LCDC_LCM_INT_STATUS;
    LCDC_LCD_MODE_U        *lcdc_mode_ptr    = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;
    LCM_INT_VECTOR_U       *lcm0_int_vec_ptr      = (LCM_INT_VECTOR_U *) LCDC_LCM_INT_VECTOR;
    LCM_INT_STATUS_U       *lcm0_int_mcu_stat_ptr = (LCM_INT_STATUS_U *) LCDC_LCM_INT_STATUS;

    mode = g_lcdc_type.lcdc_mode;

    if (lcdc_mode_ptr->mBits.tv_active != 1)
    {
        if (mode != 1)
        {
            int_stat_ptr->dwValue = TB_REG_GET (LCDC_LCD_INT_STATUS);

            if (1 == int_stat_ptr->mBits.underflow_int)
            {
                int_clear_ptr->mBits.underflow_int = 0x1;
            }

            if (1 == int_stat_ptr->mBits.end_of_frame)
            {
                if (g_lcdc_int_list[0])
                {
                    g_lcdc_int_list[0]();
                }

                int_clear_ptr->mBits.end_of_frame = 0x1;
            }

            if (1 == int_stat_ptr->mBits.start_of_frame)
            {
                if (g_lcdc_int_list[1])
                {
                    g_lcdc_int_list[1]();
                }

                int_clear_ptr->mBits.start_of_frame = 0x1;
            }
        }
        else
        {
            //int_vec_ptr->dwValue = TB_REG_GET(LCDC_LCM_INT_VECTOR);
            //compatible with lcm0 and lcm1
            if (lcm_sel)
            {
                int_vec_ptr->dwValue = TB_REG_GET (LCDC_LCM_INT_VECTOR);

                if (1 == int_vec_ptr->mBits.fifooverflow_vec)
                {
                    int_mcu_stat_ptr->mBits.fifo_overflow = 0x1;
                }

                if (1 == int_vec_ptr->mBits.osd_done_vec)
                {
                    if (g_lcdc_int_list[0])
                    {
                        g_lcdc_int_list[0]();
                    }

                    if (g_lcdc_int_list[2])
                    {
                        g_lcdc_int_list[2]();
                    }

                    int_mcu_stat_ptr->mBits.osd_done = 0x1;

                    //release semphore
                }
            }
            else
            {
                lcm0_int_vec_ptr->dwValue = TB_REG_GET (LCDC_LCM_INT_VECTOR);

                if (1 == lcm0_int_vec_ptr->mBits.fifooverflow_vec)
                {
                    lcm0_int_mcu_stat_ptr->mBits.fifo_overflow = 0x1;
                }

                if (1 == lcm0_int_vec_ptr->mBits.osd_done_vec)
                {
                if (g_lcdc_int_list[0])
                {
                    g_lcdc_int_list[0]();
                }

                if (g_lcdc_int_list[2])
                {
                    g_lcdc_int_list[2]();
                    }

                    lcm0_int_mcu_stat_ptr->mBits.osd_done = 0x1;

                    //release semphore
                }
            }

            int_stat_ptr->dwValue = TB_REG_GET (LCDC_LCD_INT_STATUS);

            if (1 == int_stat_ptr->mBits.underflow_int)
            {
                int_clear_ptr->mBits.underflow_int = 0x1;
                //SCI_TRACE_LOW:"---------LCDC MCU INT-------"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_406_112_2_18_1_45_28_130,(uint8*)"");
            }

        }
    }
    else
    {
        int_stat_ptr->dwValue = TB_REG_GET (LCDC_LCD_INT_STATUS);

        if (1 == int_stat_ptr->mBits.underflow_int)
        {
            int_clear_ptr->mBits.underflow_int = 0x1;
            //SCI_TRACE_LOW:"---------LCDC TV INT-------"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_418_112_2_18_1_45_28_131,(uint8*)"");
        }

        if (1 == int_stat_ptr->mBits.end_of_frame)
        {
            if (g_lcdc_int_list[0])
            {
                g_lcdc_int_list[0]();
            }

            int_clear_ptr->mBits.end_of_frame = 0x1;
        }

        if (1 == int_stat_ptr->mBits.start_of_frame)
        {
            if (g_lcdc_int_list[1])
            {
                g_lcdc_int_list[1]();
            }

            int_clear_ptr->mBits.start_of_frame = 0x1;
        }
    }

}//end of LCDC_irq
/**********************************************************************************************************/
// Description: initialize LCDC
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC int32 LCDC_Init (void)
{
    GR_GLB_GEN4_REG_U *gend4_ptr = (GR_GLB_GEN4_REG_U *) GR_GEN4;
    //CLK_PLL_SRC_SEL_T * pll_src_sel_mode = NULL;
    uint32 mode;
    uint32 i;
    uint32 pll_clk;
    uint32 clk_val;
    uint32 clk_div;
    PLL_TYPE_E pll_type;
    PWRMNG_ERROR_CODE_E error_code;

    for (i=0 ; i<LCDC_INIT_CB_NUMS; i++)
    {
        g_lcdc_int_list[i] = (lcdc_func_t) NULL;
    }

    if (LCD_MOD_LCM_SEL)
    {
        //Register irq to OS .
        DRV_RegHandler (28, LCDC_irq);

        //enable irq interface
        TB_REG_OR (INT_IRQ_EN, (1<<28));
    }
    else
    {
        //Register irq to OS .
        DRV_RegHandler (27, LCDC_irq);

        //enable irq interface
        TB_REG_OR (INT_IRQ_EN, (1<<27));
    }

    //   pll_src_sel_mode =  PWRMNG_GetPllSrcSelMode();

    // switch( pll_src_sel_mode->pllsrc_ctl.mBits.lcdpll_sel )
    //{
    //      case CLK_LCDC_SRC_MPLL:
    //         pll_type = PLLTYPE_M;
    //         pll_clk = CHIP_GetMPllClk();
    //        break;
    //case CLK_LCDC_SRC_VPLL:
    pll_type = PLLTYPE_V;
    pll_clk = CHIP_GetVPllClk();
    /*       break;
       case CLK_LCDC_SRC_TDPLL:
       case (CLK_LCDC_SRC_TDPLL+1):
           pll_type = PLLTYPE_TD;
           pll_clk = CHIP_GetTDPllClk();
           break;
       default:
           pll_type = PLLTYPE_V;
           pll_clk = CHIP_GetVPllClk();
           SCI_ASSERT(0);
           break;
        }
         */
    pll_clk /= 1000000;
    clk_val = 12;
    clk_div = pll_clk/clk_val - 1;


    //Init the pll device request info.
    s_PllDevice_Req_Lcdc.device_id = PLL_DEVICE_LCDC_ID;
    s_PllDevice_Req_Lcdc.device_name = "PLL_REQ_LCDC";
    s_PllDevice_Req_Lcdc.pll_type = pll_type;
    s_PllDevice_Req_Lcdc.b_pmt_chng_pll = TRUE;
    s_PllDevice_Req_Lcdc.pll_syncfreq_lmt = PLL_SYNC_NULL;

    if (g_lcdc_type.lcd_data_bits != 6)
    {
        //set LCDC clock
        gend4_ptr->mBits.clk_lcdc_div = clk_div;

    }
    else
    {
        //set LCDC clock
        clk_val = 15;
        clk_div = pll_clk/clk_val;

        SCI_ASSERT ( (clk_div >= 0x1) && (clk_div <= 0x7F));
        gend4_ptr->mBits.clk_lcdc_div = clk_div;
    }


    mode = g_lcdc_type.lcdc_mode;

#if 1
    //SCI_TRACE_LOW:" LCDC_Init() !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_537_112_2_18_1_45_29_132,(uint8*)"");

    if (s_PllDevice_Req_Lcdc.device_id != PLL_DEVICE_LCDC_ID)
    {
        SCI_PASSERT (0, (" The LCDC  pll req hasn't been init !"));
    }

    //register the lcdc request to the relative PLL.
    error_code = PWRMNG_RegPllDeviceReq (&s_PllDevice_Req_Lcdc);

    if (REQ_EXISTED == error_code)
    {
        //SCI_TraceLow:" the lcdc req has exist in the pll req lest already!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_549_112_2_18_1_45_29_133,(uint8*)"");
        /* deal with the error here */

        //SCI_PASSERT(0,(" the lcdc req has exist in the pll req lest already!"));
    }

    //enable the lcdc clk.
    PWRMNG_LCDC_ClkSwtch (TRUE);
#endif

    lcdc_init_blk();

    switch (mode)
    {
        case 0:
            lcdc_rgb_init (mode);
            break;
        case 1:
            lcdc_mcu_init (mode);
            break;
        default:
            break;
    }

    REG32 (LCDC_DAC_CONTROL_REG) |= BIT_0; //zhijun chen ,close tv_out circult,
    return LCD_OK;
}//end of LCDC_Init
/**********************************************************************************************************/
// Description: set LCDC Clock
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC int32 LCDC_SetClock (uint32 lcdc_clk)
{
    GR_GLB_GEN4_REG_U *gend4_ptr = (GR_GLB_GEN4_REG_U *) GR_GEN4;

    uint32 mode;
    uint32 i;
    uint32 pll_clk;
    uint32 clk_val;
    uint32 clk_div;
    PLL_TYPE_E pll_type;
    PWRMNG_ERROR_CODE_E error_code;

    if (g_tv_mode == 1)
    {
        return LCD_OK;
    }

    SCI_ASSERT (lcdc_clk); /*lint -e414*/



    pll_type = PLLTYPE_V;
    pll_clk = CHIP_GetVPllClk();



    clk_val = lcdc_clk;
    clk_div = pll_clk/clk_val - 1;/*lint -e414*/

    SCI_ASSERT (clk_div);

    SCI_ASSERT ( (clk_div >= 0x1) && (clk_div <= 0x7F));

    //Init the pll device request info.
    s_PllDevice_Req_Lcdc.device_id = PLL_DEVICE_LCDC_ID;
    s_PllDevice_Req_Lcdc.device_name = "PLL_REQ_LCDC";
    s_PllDevice_Req_Lcdc.pll_type = pll_type;
    s_PllDevice_Req_Lcdc.b_pmt_chng_pll = TRUE;
    s_PllDevice_Req_Lcdc.pll_syncfreq_lmt = PLL_SYNC_NULL;


    LCD_GetLCDCSemaphore();


    if (g_lcdc_type.lcd_data_bits != 6)
    {
        //set LCDC clock
        gend4_ptr->mBits.clk_lcdc_div = clk_div;

    }
    else
    {
        //set LCDC clock
        clk_val = 15;
        clk_div = pll_clk/clk_val;

        SCI_ASSERT ( (clk_div >= 0x1) && (clk_div <= 0x7F));
        gend4_ptr->mBits.clk_lcdc_div = clk_div;
    }


    LCD_PutLCDCSemaphore();

    return LCD_OK;
}//end of LCDC_SetClock
/**********************************************************************************************************/
// Description: get LCDC Clock
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC uint32 LCDC_GetClock (void)
{
    GR_GLB_GEN4_REG_U *gend4_ptr = (GR_GLB_GEN4_REG_U *) GR_GEN4;

    uint32 mode;
    uint32 i;
    uint32 pll_clk;
    uint32 clk_val;
    uint32 clk_div = 0;
    uint32 lcdc_clk = 0;
    PLL_TYPE_E pll_type;
    PWRMNG_ERROR_CODE_E error_code;

    if (g_tv_mode == 1)
    {
        lcdc_clk = LCDC_TV_CLOCK;
        return lcdc_clk;
    }


    pll_type = PLLTYPE_V;
    pll_clk = CHIP_GetVPllClk();


    clk_div = gend4_ptr->mBits.clk_lcdc_div + 1;

    //    if(clk_div)
    {
        lcdc_clk = pll_clk/clk_div;
    }
    //else
    // {
    //   SCI_ASSERT(0);
    //  }

    //SCI_TRACE_LOW:"Get LCDC Clock is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_684_112_2_18_1_45_29_134,(uint8*)"d",lcdc_clk);

    return lcdc_clk;
}
/**********************************************************************************************************/
// Description: restore LCDC Clock
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC int32 LCDC_RestoreClock (void)
{
    GR_GLB_GEN4_REG_U *gend4_ptr = (GR_GLB_GEN4_REG_U *) GR_GEN4;

    uint32 mode;
    uint32 i;
    uint32 pll_clk;
    uint32 clk_val;
    uint32 clk_div;
    uint32 lcdc_clk;
    PLL_TYPE_E pll_type;
    PWRMNG_ERROR_CODE_E error_code;

    if (g_tv_mode == 1)
    {
        return LCD_OK;
    }

    pll_type = PLLTYPE_V;
    pll_clk = CHIP_GetVPllClk();

    lcdc_clk = 12000000;
    clk_val = lcdc_clk;
    clk_div = pll_clk/clk_val - 1;

    SCI_ASSERT (clk_div);

    SCI_ASSERT ( (clk_div >= 0x1) && (clk_div <= 0x7F));

    //Init the pll device request info.
    s_PllDevice_Req_Lcdc.device_id = PLL_DEVICE_LCDC_ID;
    s_PllDevice_Req_Lcdc.device_name = "PLL_REQ_LCDC";
    s_PllDevice_Req_Lcdc.pll_type = pll_type;
    s_PllDevice_Req_Lcdc.b_pmt_chng_pll = TRUE;
    s_PllDevice_Req_Lcdc.pll_syncfreq_lmt = PLL_SYNC_NULL;


    LCD_GetLCDCSemaphore();

    if (g_lcdc_type.lcd_data_bits != 6)
    {
        //set LCDC clock
        gend4_ptr->mBits.clk_lcdc_div = clk_div;

    }
    else
    {
        //set LCDC clock
        clk_val = 15;
        clk_div = pll_clk/clk_val;

        SCI_ASSERT ( (clk_div >= 0x1) && (clk_div <= 0x7F));
        gend4_ptr->mBits.clk_lcdc_div = clk_div;
    }

    LCD_PutLCDCSemaphore();


    return LCD_OK;
}//end of LCDC_RestoreClock
/**********************************************************************************************************/
// Description: configure LCDC
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      input:
//          spec_ptr - pointer to LCD information struction
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_Configure (LCDC_SPEC_T *spec_ptr)
{
    uint32 mode;
    int32 error;

    if (spec_ptr == PNULL)
    {
        return LCD_ERROR_POINTER_NULL;
    }

    mode = g_lcdc_type.lcdc_mode;
    error = LCD_OK;

    switch (mode)
    {
        case 0:
            error = lcdc_rgb_configure (&spec_ptr->rgb_lcd_spec);
            break;
        case 1:
            error = lcdc_mcu_configure (&spec_ptr->mcu_lcd_spec);
            break;
        default:
            break;
    }

    return error;
}//end of LCDC_Configure
/**********************************************************************************************************/
// Description: get LCDC configure information that is related with LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      input:
//          none
//      output:
//          p_spec - pointer to information structure
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_GetInfo (LCDC_INFO_T *info_ptr)
{
    uint32 mode;
    int32 error;

    mode = g_lcdc_type.lcdc_mode;
    error = LCD_OK;

    if (info_ptr == PNULL)
    {
        return LCD_ERROR_POINTER_NULL;
    }

    switch (mode)
    {
        case 0:
            info_ptr->mcu_info.parameter0.value = 0x0;
            info_ptr->mcu_info.parameter1.value = 0x0;
            info_ptr->mcu_info.modesel.value    = 0x0;
            info_ptr->mcu_info.rgbmode.value    = 0x0;
            info_ptr->mcu_info.lcd_cd[0]        = 0x2;
            info_ptr->mcu_info.lcd_cd[1]        = 0x2;
            info_ptr->mcu_info.lcd_sum          = 0x0;
            error = lcdc_rgb_get_info (&info_ptr->rgb_info);
            break;
        case 1:
            info_ptr->rgb_info.timing0.value    = 0x0;
            info_ptr->rgb_info.timing1.value    = 0x0;
            info_ptr->rgb_info.lcd_ctrl.value   = 0x0;
            error = lcdc_mcu_get_info (&info_ptr->mcu_info);
            break;
        default:
            break;
    }

    return error;
}//end of LCDC_GetInfo
/**********************************************************************************************************/
// Description: send the command and data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      input:
//          cmd - lcd command
//          lcd_data - lcd command data
//          lcd_id - only is used for lcdc mcu mode
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SendCmdData (uint32 cmd,uint32 lcd_data,uint32 lcd_id)
{
    uint32 mode;

    mode = g_lcdc_type.lcdc_mode;

    switch (mode)
    {
        case 0:
            lcdc_rgb_send_cmd_data (cmd,lcd_data);
            break;
        case 1:
            lcdc_mcu_send_cmd_data (cmd,lcd_data,lcd_id);
            break;
        default:
            break;
    }

    return LCD_OK;
}//end of LCDC_SendCmdData
/**********************************************************************************************************/
// Description: send command to lcd
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      lcd_id parameter is used when lcdc mcu mode
//      input:
//          cmd - lcd command
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SendCmd (uint32 cmd,uint32 lcd_id)
{
    uint32 mode;

    mode = g_lcdc_type.lcdc_mode;

    switch (mode)
    {
        case 0:
            lcdc_rgb_send_cmd (cmd);
            break;
        case 1:
            lcdc_mcu_send_cmd (cmd,lcd_id);
            break;
        default:
            break;
    }

    return LCD_OK;
}//end of LCDC_SendCmd
/**********************************************************************************************************/
// Description: send display data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      this interface is used in lcdc mcu mode
//      input:
//          lcd_data - display data
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SendData (uint32 lcd_data,uint32 lcd_id)
{
    uint32 data_addr;

    data_addr = g_lcdc_mcu_addr[lcd_id].data_addr;

    TB_REG_SET (data_addr,lcd_data);

    return LCD_OK;
}//end of LCDC_SendData
/**********************************************************************************************************/
// Description: get LCD index register value
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      lcd_id parameter is used when lcdc mcu mode
//      input:
//          cmd - LCD command
//          lcd_id - LCD ID
//      output:
//          data_ptr - pointer to data that is LCD index register value
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_ReadLcdIndex (uint32 *data_ptr,uint32 cmd,uint32 lcd_id)
{
    uint32 mode;
    int32 error;

    mode = g_lcdc_type.lcdc_mode;
    error = LCD_OK;

    SCI_ASSERT (PNULL != data_ptr);

    switch (mode)
    {
        case 0:
            error = lcdc_rgb_read_lcdindex (data_ptr,cmd);
            break;
        case 1:
            error = lcdc_mcu_read_lcdindex (data_ptr,cmd,lcd_id);
            break;
        default:
            break;
    }

    return error;
}//end of LCDC_ReadLcdIndex
/**********************************************************************************************************/
// Description: start LCDC to write display data to LCD or TV
// Global resource dependence: g_lcdc_mode
// Author: Jianping
// Note:
//      lcd_id parameter is used when lcdc mcu mode
//      input:
//          lcd_width - lcd width
//          lcd_height - lcd height
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_TransCmd (uint16 lcd_width,uint16 lcd_height,uint32 lcd_cs)
{
    int32 error;
    uint32 mode;

    mode = g_lcdc_type.lcdc_mode;
    error = LCD_OK;

    switch (mode)
    {
        case 0:
            error = lcdc_rgb_transcmd (lcd_width,lcd_height);
            break;
        case 1:
            error = lcdc_mcu_transcmd (lcd_width,lcd_height,lcd_cs);
            break;
        default:
            break;
    }

    return error;
}//end of LCDC_TransCmd
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_GetConfigInfo (LCDC_CONFIG_T *cfg_ptr)
{
    LCDC_LCD_MODE_U          *lcdc_mode_ptr       = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;
    LCDC_LCD_SIZE_U          *lcdc_size_ptr       = (LCDC_LCD_SIZE_U *) LCDC_LCD_SIZE;
    LCDC_LCD_BACKGROUND_U    *lcdc_background_ptr = (LCDC_LCD_BACKGROUND_U *) LCDC_LCD_BACKGROUND;
    LCDC_COLOR_COEFF_A1_U    *lcdc_a1_ptr         = (LCDC_COLOR_COEFF_A1_U *) LCDC_COLOR_COEFF_A1;
    LCDC_COLOR_COEFF_A2_U    *lcdc_a2_ptr         = (LCDC_COLOR_COEFF_A2_U *) LCDC_COLOR_COEFF_A2;
    LCDC_COLOR_COEFF_A3_U    *lcdc_a3_ptr         = (LCDC_COLOR_COEFF_A3_U *) LCDC_COLOR_COEFF_A3;
    LCDC_COLOR_COEFF_B1_U    *lcdc_b1_ptr         = (LCDC_COLOR_COEFF_B1_U *) LCDC_COLOR_COEFF_B1;
    LCDC_COLOR_COEFF_B2_U    *lcdc_b2_ptr         = (LCDC_COLOR_COEFF_B2_U *) LCDC_COLOR_COEFF_B2;
    LCDC_COLOR_COEFF_B3_U    *lcdc_b3_ptr         = (LCDC_COLOR_COEFF_B3_U *) LCDC_COLOR_COEFF_B3;
    LCDC_COLOR_COEFF_C1_U    *lcdc_c1_ptr         = (LCDC_COLOR_COEFF_C1_U *) LCDC_COLOR_COEFF_C1;
    LCDC_COLOR_COEFF_C2_U    *lcdc_c2_ptr         = (LCDC_COLOR_COEFF_C2_U *) LCDC_COLOR_COEFF_C2;
    LCDC_COLOR_COEFF_C3_U    *lcdc_c3_ptr         = (LCDC_COLOR_COEFF_C3_U *) LCDC_COLOR_COEFF_C3;

    SCI_ASSERT (PNULL != cfg_ptr);

    cfg_ptr->mode         = lcdc_mode_ptr->mBits.lcdc_mode;
    cfg_ptr->background_b = lcdc_background_ptr->mBits.background_b;
    cfg_ptr->background_g = lcdc_background_ptr->mBits.background_g;
    cfg_ptr->backgrounf_r = lcdc_background_ptr->mBits.background_r;
    cfg_ptr->frame_mode   = lcdc_mode_ptr->mBits.refresh_mode;
    cfg_ptr->size_x       = lcdc_size_ptr->mBits.lcd_size_x;
    cfg_ptr->size_y       = lcdc_size_ptr->mBits.lcd_size_y;
    cfg_ptr->color_a1     = lcdc_a1_ptr->mBits.a1;
    cfg_ptr->color_a2     = lcdc_a2_ptr->mBits.a2;
    cfg_ptr->color_a3     = lcdc_a3_ptr->mBits.a3;
    cfg_ptr->color_b1     = lcdc_b1_ptr->mBits.b1;
    cfg_ptr->color_b2     = lcdc_b2_ptr->mBits.b2;
    cfg_ptr->color_b3     = lcdc_b3_ptr->mBits.b3;
    cfg_ptr->color_c1     = lcdc_c1_ptr->mBits.c1;
    cfg_ptr->color_c2     = lcdc_c2_ptr->mBits.c2;
    cfg_ptr->color_c3     = lcdc_c3_ptr->mBits.c3;

    cfg_ptr->timing0      = TB_REG_GET (LCDC_LCD_TIMING0);
    cfg_ptr->timing1      = TB_REG_GET (LCDC_LCD_TIMING1);
    cfg_ptr->lcd_ctrl     = TB_REG_GET (LCDC_LCD_CTRL_REG);
    cfg_ptr->para0        = TB_REG_GET (LCDC_LCMPARAMETER0);
    cfg_ptr->para1        = TB_REG_GET (LCDC_LCMPARAMETER1);
    cfg_ptr->modesel      = TB_REG_GET (LCDC_LCMMODESEL);
    cfg_ptr->rgb_mode     = TB_REG_GET (LCDC_LCM_RGB_MODE);
    cfg_ptr->selpin       = TB_REG_GET (LCDC_LCM_SELPIN);

}//end of LCDC_GetConfigInfo
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_SetConfig (LCDC_CONFIG_T *cfg_ptr)
{
    LCDC_LCD_MODE_U          *lcdc_mode_ptr       = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;
    LCDC_LCD_SIZE_U          *lcdc_size_ptr       = (LCDC_LCD_SIZE_U *) LCDC_LCD_SIZE;
    LCDC_COLOR_COEFF_A1_U    *lcdc_a1_ptr         = (LCDC_COLOR_COEFF_A1_U *) LCDC_COLOR_COEFF_A1;
    LCDC_COLOR_COEFF_A2_U    *lcdc_a2_ptr         = (LCDC_COLOR_COEFF_A2_U *) LCDC_COLOR_COEFF_A2;
    LCDC_COLOR_COEFF_A3_U    *lcdc_a3_ptr         = (LCDC_COLOR_COEFF_A3_U *) LCDC_COLOR_COEFF_A3;
    LCDC_COLOR_COEFF_B1_U    *lcdc_b1_ptr         = (LCDC_COLOR_COEFF_B1_U *) LCDC_COLOR_COEFF_B1;
    LCDC_COLOR_COEFF_B2_U    *lcdc_b2_ptr         = (LCDC_COLOR_COEFF_B2_U *) LCDC_COLOR_COEFF_B2;
    LCDC_COLOR_COEFF_B3_U    *lcdc_b3_ptr         = (LCDC_COLOR_COEFF_B3_U *) LCDC_COLOR_COEFF_B3;
    LCDC_COLOR_COEFF_C1_U    *lcdc_c1_ptr         = (LCDC_COLOR_COEFF_C1_U *) LCDC_COLOR_COEFF_C1;
    LCDC_COLOR_COEFF_C2_U    *lcdc_c2_ptr         = (LCDC_COLOR_COEFF_C2_U *) LCDC_COLOR_COEFF_C2;
    LCDC_COLOR_COEFF_C3_U    *lcdc_c3_ptr         = (LCDC_COLOR_COEFF_C3_U *) LCDC_COLOR_COEFF_C3;
    LCDC_LCD_BACKGROUND_U    *lcdc_background_ptr = (LCDC_LCD_BACKGROUND_U *) LCDC_LCD_BACKGROUND;


    SCI_ASSERT (PNULL != cfg_ptr);

    lcdc_mode_ptr->mBits.lcdc_mode          = cfg_ptr->mode;
    lcdc_mode_ptr->mBits.tv_active          = 0;
    lcdc_background_ptr->mBits.background_b = cfg_ptr->background_b;
    lcdc_background_ptr->mBits.background_g = cfg_ptr->background_g;
    lcdc_background_ptr->mBits.background_r = cfg_ptr->backgrounf_r;
    lcdc_mode_ptr->mBits.refresh_mode       = cfg_ptr->frame_mode;
    lcdc_size_ptr->mBits.lcd_size_x         = cfg_ptr->size_x;
    lcdc_size_ptr->mBits.lcd_size_y         = cfg_ptr->size_y;

    lcdc_a1_ptr->mBits.a1 = cfg_ptr->color_a1;
    lcdc_a2_ptr->mBits.a2 = cfg_ptr->color_a2;
    lcdc_a3_ptr->mBits.a3 = cfg_ptr->color_a3;
    lcdc_b1_ptr->mBits.b1 = cfg_ptr->color_b1;
    lcdc_b2_ptr->mBits.b2 = cfg_ptr->color_b2;
    lcdc_b3_ptr->mBits.b3 = cfg_ptr->color_b3;
    lcdc_c1_ptr->mBits.c1 = cfg_ptr->color_c1;
    lcdc_c2_ptr->mBits.c2 = cfg_ptr->color_c2;
    lcdc_c3_ptr->mBits.c3 = cfg_ptr->color_c3;

    TB_REG_SET (LCDC_LCD_TIMING0,  cfg_ptr->timing0);
    TB_REG_SET (LCDC_LCD_TIMING1,  cfg_ptr->timing1);
    TB_REG_SET (LCDC_LCD_CTRL_REG, cfg_ptr->lcd_ctrl);
    TB_REG_SET (LCDC_LCMPARAMETER0,cfg_ptr->para0);
    TB_REG_SET (LCDC_LCMPARAMETER1,cfg_ptr->para1);
    TB_REG_SET (LCDC_LCMMODESEL   ,cfg_ptr->modesel);
    TB_REG_SET (LCDC_LCM_RGB_MODE ,cfg_ptr->rgb_mode);
    TB_REG_SET (LCDC_LCM_SELPIN   ,cfg_ptr->selpin);
}//end of LCDC_SetConfig

/**********************************************************************************************************/
// Description: initialize LCDC TV mode
// Global resource dependence: g_lcd_background
// Author: Jianping.wang
// Note:
//      lcdc tv mode is prior to mcu mode and rgb mode
/**********************************************************************************************************/
PUBLIC int32 LCDC_StartTv (uint32 tv_mode)
{
    return 0;
}//end of LCDC_StartTv
/**********************************************************************************************************/
// Description: close LCDC TV mode,resume to LCDC rgb mode or mcu mode
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_CloseTv (void)
{

}//end of LCDC_CloseTV
/**********************************************************************************************************/
// Description: provide lcdc interrupt
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      input :
//          int_ptr - pointer to interrupt structure
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_SpecialMode (LCD_INT_ENA_T *int_ptr)
{
    uint32 mode;
    int32 error;

    mode = g_lcdc_type.lcdc_mode;
    error = LCD_OK;

    SCI_ASSERT (int_ptr != PNULL);

    switch (mode)
    {
        case 0:
            lcdc_rgb_int_enable (&int_ptr->rgb_int_enable);
            break;
        case 1:
            lcdc_mcu_int_enable (int_ptr->mcu_endofosd_en);
            break;
        default:
            break;
    }

    return error;
}//end of LCDC_IntEnable
/**********************************************************************************************************/
// Description: register handle function
// Global resource dependence: g_lcdc_int_list
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC BOOLEAN LCDC_RegisterInterrupt (uint32 int_num, lcdc_func_t func)
{

    //  SCI_ASSERT( PNULL != func );
    if (int_num >= LCDC_INIT_CB_NUMS)
    {
        return SCI_FALSE;
    }

    g_lcdc_int_list[int_num] = func;

    return SCI_TRUE;

}//end of LCDC_RegisterInterrupt
/**********************************************************************************************************/
// Description: reset LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      delay_ms parameter is used in lcdc mcu mode
//      input:
//          delay_ms - delay time(ms)
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCDC_ResetLcd (uint32 delay_ms)
{
    uint32 mode;

    mode = g_lcdc_type.lcdc_mode;

    switch (mode)
    {
        case 0:
            lcdc_rgb_reset();
            break;
        case 1:
            GPIO_ResetLcd (0,delay_ms);
            break;
        default:
            break;
    }

    return LCD_OK;
}//end of LCDC_ResetLcd
/**********************************************************************************************************/
// Description: select lcdc output data flow
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      this function is used in lcdc rgb mode
//      feed_back - 1:LCDC output data to ISP module
//                 0:LCDC don't output data to ISP
//      disable_lcdout - 1:disable lcdc output to lcd
//                       0:enable lcdc output to lcd
/**********************************************************************************************************/
PUBLIC void LCDC_ConfigDataFlow (uint32 feed_back,uint32 disable_lcdout)
{
    LCDC_LCD_CTRL_REG_U    *lcdc_lcd_ctrl_ptr = (LCDC_LCD_CTRL_REG_U *) LCDC_LCD_CTRL_REG;

    lcdc_lcd_ctrl_ptr->mBits.feedback_mode = feed_back;
    lcdc_lcd_ctrl_ptr->mBits.disable_lcd_out = disable_lcdout;

}//end of LCDC_ConfigDataFlow
/**********************************************************************************************************/
// Description: adjust transfer timing
// Global resource dependence:
// Author: Jianping.wang
// Note:
//
/**********************************************************************************************************/
PUBLIC int32 LCDC_ChangePulseWidth (LCD_MODE_E mode, uint32 cs_id, LCD_TIMING_U timing_para)
{
    LCDC_RGB_LCD_TIMING0_U    *rgb_timing0_ptr    = (LCDC_RGB_LCD_TIMING0_U *) LCDC_LCD_TIMING0;
    LCDC_RGB_LCD_TIMING1_U    *rgb_timing1_ptr    = (LCDC_RGB_LCD_TIMING1_U *) LCDC_LCD_TIMING1;
    LCDC_MCU_LCD_PARAMETER0_U *mcu_parameter0_ptr = (LCDC_MCU_LCD_PARAMETER0_U *) LCDC_LCMPARAMETER0;
    LCDC_MCU_LCD_PARAMETER1_U *mcu_parameter1_ptr = (LCDC_MCU_LCD_PARAMETER1_U *) LCDC_LCMPARAMETER1;

    if (mode == LCD_RGB)
    {
        rgb_timing0_ptr->mBits.res = 0;
        rgb_timing0_ptr->mBits.vsw = timing_para.rgb_mode_timing.vsw;
        rgb_timing0_ptr->mBits.efw = timing_para.rgb_mode_timing.efw;
        rgb_timing0_ptr->mBits.bfw = timing_para.rgb_mode_timing.bfw;
        rgb_timing1_ptr->mBits.res = 0;
        rgb_timing1_ptr->mBits.hsw = timing_para.rgb_mode_timing.hsw;
        rgb_timing1_ptr->mBits.elw = timing_para.rgb_mode_timing.elw;
        rgb_timing1_ptr->mBits.blw = timing_para.rgb_mode_timing.blw;
    }
    else if (cs_id == 0)
    {
        mcu_parameter0_ptr->mBits.res  = 0;
        mcu_parameter0_ptr->mBits.rcss = timing_para.mcu_mode_timing.rcss;
        mcu_parameter0_ptr->mBits.rlpw = timing_para.mcu_mode_timing.rlpw;
        mcu_parameter0_ptr->mBits.rhpw = timing_para.mcu_mode_timing.rhpw;
        mcu_parameter0_ptr->mBits.wcss = timing_para.mcu_mode_timing.wcss;
        mcu_parameter0_ptr->mBits.wlpw = timing_para.mcu_mode_timing.wlpw;
        mcu_parameter0_ptr->mBits.whpw = timing_para.mcu_mode_timing.whpw;
    }
    else
    {
        mcu_parameter1_ptr->mBits.res  = 0;
        mcu_parameter1_ptr->mBits.rcss = timing_para.mcu_mode_timing.rcss;
        mcu_parameter1_ptr->mBits.rlpw = timing_para.mcu_mode_timing.rlpw;
        mcu_parameter1_ptr->mBits.rhpw = timing_para.mcu_mode_timing.rhpw;
        mcu_parameter1_ptr->mBits.wcss = timing_para.mcu_mode_timing.wcss;
        mcu_parameter1_ptr->mBits.wlpw = timing_para.mcu_mode_timing.wlpw;
        mcu_parameter1_ptr->mBits.whpw = timing_para.mcu_mode_timing.whpw;
    }

    return LCD_OK;
}//end of LCD_IsrHandle
/**********************************************************************************************************/
// Description: close LCDC
// Global resource dependence:
// Author: jianping.wang
// Note:
//      flag - 1:disable lcdc;0:enable lcdc
//---------------------------------------------------------------------------------------------------------
PUBLIC void LCDC_Close (uint32 flag)
{
    LCDC_LCD_MODE_U        *lcdc_mode_ptr    = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;
    GR_GLB_GEN4_REG_U      *gen4_ptr         = (GR_GLB_GEN4_REG_U *) GR_GEN4;

    PWRMNG_ERROR_CODE_E error_code;
    return;

#if 0

    if (flag == 1)
    {
        if (SCI_FALSE == LCD_IsPermitLcdcClose())
        {
            //SCI_TRACE_LOW:"----Don't permint close lcdc----"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_1270_112_2_18_1_45_30_135,(uint8*)"");
            return;
        }
    }
    else
    {
        if (SCI_FALSE == LCD_IsPermitLcdcOpen())
        {
            //SCI_TRACE_LOW:"----Don't permint open lcdc again----"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_1278_112_2_18_1_45_30_136,(uint8*)"");
            return;
        }
    }

    if (flag == 1)
    {

        lcdc_mode_ptr->mBits.lcd_enable = 0x0;
        //     ahb_ctl0_ptr->mBits.lcdc_eb     = 0x0;
        //     ahb_ctl0_ptr->mBits.tve_eb      = 0x0;
#if 1
        //SCI_TRACE_LOW:" LCDC_Close(), close lcdc !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_1290_112_2_18_1_45_30_137,(uint8*)"");

        if (s_PllDevice_Req_Lcdc.device_id != PLL_DEVICE_LCDC_ID)
        {
            SCI_PASSERT (0, (" The LCDC req hasn't been init !"));
        }

        //disable the lcdc clk.
        PWRMNG_LCDC_ClkSwtch (FALSE);

        //unregister the lcdc request from the relative PLL.
        error_code = PWRMNG_UnRegPllDeviceReq (&s_PllDevice_Req_Lcdc);

        if (error_code == REQ_NOT_EXIST)
        {
            //SCI_TraceLow:" the lcdc req not in the pll req list !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_1305_112_2_18_1_45_30_138,(uint8*)"");
            /* deal with the error here */

            //SCI_PASSERT(0,(" the lcdc req not in the pll req list !"));
        }

#endif
    }
    else
    {

        //  ahb_ctl0_ptr->mBits.lcdc_eb     = 0x1;
        //  ahb_ctl0_ptr->mBits.tve_eb      = 0x1;
#if 1
        //SCI_TRACE_LOW:" LCDC_Close(), open lcdc !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_1319_112_2_18_1_45_30_139,(uint8*)"");

        if (s_PllDevice_Req_Lcdc.device_id != PLL_DEVICE_LCDC_ID)
        {
            SCI_PASSERT (0, (" The LCDC req hasn't been init !"));
        }

        //register the lcdc request to the relative PLL.
        error_code = PWRMNG_RegPllDeviceReq (&s_PllDevice_Req_Lcdc);

        if (REQ_EXISTED == error_code)
        {
            //SCI_TraceLow:" the lcdc req has exist in the pll req lest already!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_1331_112_2_18_1_45_30_140,(uint8*)"");
            /* deal with the error here */

            //SCI_PASSERT(0,(" the lcdc req has exist in the pll req lest already!"));
        }

        //enable the lcdc clk.
        PWRMNG_LCDC_ClkSwtch (TRUE);
#endif
        lcdc_mode_ptr->mBits.lcd_enable = 0x1;

        if (lcdc_mode_ptr->mBits.tv_active == 1)
        {
            //         gen1_ptr->mBits.v_pllmn_we = 1;               //video pll write enable
            //         TB_REG_AND(GLB_VPLL_MN_REG_ADDR,~(0xfff<<16));
            //         TB_REG_OR(GLB_VPLL_MN_REG_ADDR,(0x51<<16));   //Vpll=81MHz

            //register the lcdc request to the relative PLL.
            error_code = PWRMNG_RegPllDeviceReq (&s_PllDevice_Req_Lcdc);

            if (REQ_EXISTED == error_code)
            {
                //SCI_TraceLow:" the lcdc req has exist in the pll req lest already!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_1353_112_2_18_1_45_30_141,(uint8*)"");
                /* deal with the error here */
            }

            //register the tv out request to the relative PLL.


            gen4_ptr->mBits.clk_lcdc_div = 5;             //lcdc clock is 27MHz

            if (lcdc_mode_ptr->mBits.tvread_mode == 0)
            {
                lcdc_mode_ptr->dwValue = 0x2e;
            }
            else
            {
                lcdc_mode_ptr->dwValue = 0x4e;
            }

            //set DAC register
            TB_REG_AND (GLB_LDO_CTL3_REG_ADDR,~3);  //open dac ldo
            TB_REG_OR (GLB_LDO_CTL3_REG_ADDR,2);
        }
        else if (lcdc_mode_ptr->mBits.lcdc_mode != 1)
        {
            lcdc_mode_ptr->mBits.start_refresh = 0x1;
        }

        OS_TickDelay (1);
    }

#endif

}//end of LCDC_Close
/**********************************************************************************************************/
// Description: set color correction matrix in rgb mode and mcu mode;
//              or set rgb to yuv conversion matrix in tv mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      matrix_ptr - pointer to the matrix that is color correct matrix,or rgb to yuv conversion matrix
/**********************************************************************************************************/
PUBLIC void LCDC_SetColorMatrix (COLORMATRIX_T *matrix_ptr)
{
    LCDC_COLOR_COEFF_A1_U   *lcdc_a1_ptr = (LCDC_COLOR_COEFF_A1_U *) LCDC_COLOR_COEFF_A1;
    LCDC_COLOR_COEFF_A2_U   *lcdc_a2_ptr = (LCDC_COLOR_COEFF_A2_U *) LCDC_COLOR_COEFF_A2;
    LCDC_COLOR_COEFF_A3_U   *lcdc_a3_ptr = (LCDC_COLOR_COEFF_A3_U *) LCDC_COLOR_COEFF_A3;
    LCDC_COLOR_COEFF_B1_U   *lcdc_b1_ptr = (LCDC_COLOR_COEFF_B1_U *) LCDC_COLOR_COEFF_B1;
    LCDC_COLOR_COEFF_B2_U   *lcdc_b2_ptr = (LCDC_COLOR_COEFF_B2_U *) LCDC_COLOR_COEFF_B2;
    LCDC_COLOR_COEFF_B3_U   *lcdc_b3_ptr = (LCDC_COLOR_COEFF_B3_U *) LCDC_COLOR_COEFF_B3;
    LCDC_COLOR_COEFF_C1_U   *lcdc_c1_ptr = (LCDC_COLOR_COEFF_C1_U *) LCDC_COLOR_COEFF_C1;
    LCDC_COLOR_COEFF_C2_U   *lcdc_c2_ptr = (LCDC_COLOR_COEFF_C2_U *) LCDC_COLOR_COEFF_C2;
    LCDC_COLOR_COEFF_C3_U   *lcdc_c3_ptr = (LCDC_COLOR_COEFF_C3_U *) LCDC_COLOR_COEFF_C3;

    SCI_ASSERT (matrix_ptr != PNULL);

    lcdc_a1_ptr->dwValue = matrix_ptr->coeffA1.value;
    lcdc_a2_ptr->dwValue = matrix_ptr->coeffA2.value;
    lcdc_a3_ptr->dwValue = matrix_ptr->coeffA3.value;
    lcdc_b1_ptr->dwValue = matrix_ptr->coeffB1.value;
    lcdc_b2_ptr->dwValue = matrix_ptr->coeffB2.value;
    lcdc_b3_ptr->dwValue = matrix_ptr->coeffB3.value;
    lcdc_c1_ptr->dwValue = matrix_ptr->coeffC1.value;
    lcdc_c2_ptr->dwValue = matrix_ptr->coeffC2.value;
    lcdc_c3_ptr->dwValue = matrix_ptr->coeffC3.value;

}//end of LCDC_SetColorMatrix
/**********************************************************************************************************/
// Description: updata tv display
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/



/**********************************************************************************************************/
// Description: start LCDC,use background refresh LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_Start (void)
{
    LCDC_LCD_MODE_U    *lcdc_mode_ptr = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;

    lcdc_mode_ptr->mBits.lcd_enable    = 0x1;
    lcdc_mode_ptr->mBits.shadow_update = 0x1;
    lcdc_mode_ptr->mBits.start_refresh = 0x1;

}//end LCDC_Start
/**********************************************************************************************************/
// Description: Update LCDC Register setting
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_UpdateRegisters (void)
{
    LCDC_LCD_MODE_U    *lcdc_mode_ptr = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;

    lcdc_mode_ptr->mBits.shadow_update = 0x1;

}//end LCDC_Start
/**********************************************************************************************************/
// Description: set LCD background color
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_SetBackground (uint32 color)
{
    LCDC_LCD_BACKGROUND_U    *background_ptr = (LCDC_LCD_BACKGROUND_U *) LCDC_LCD_BACKGROUND;
    LCDC_LCD_MODE_U          *lcdc_mode_ptr  = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;

    background_ptr->dwValue = color;
    lcdc_mode_ptr->mBits.shadow_update = 0x1;

}//end of LCDC_SetBackground
/**********************************************************************************************************/
// Description: disable use display block
// Global resource dependence:  g_lcd_blk
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_DisableLcdBlock (void)
{
    uint32 i;
    uint32 reg_addr;

    for (i=0 ; i<LCD_BLOCK_NUM ; i++)
    {
        if (g_lcd_blk[i].blk_used==1)
        {
            reg_addr = (uint32) LCDC_LCD_BLOCK0ADDR;
            reg_addr += i << 4;
            reg_addr += 0x4;
            TB_REG_AND (reg_addr,0x7fffffff);
        }
    }
}//end of LCDC_DisableLcdBlock
/**********************************************************************************************************/
// Description: software reset LCDC and TV Encoder
// Global resource dependence:
// Author: Jianping.wang
// Note:wirte 1-0
/**********************************************************************************************************/
PUBLIC void LCDC_Reset (void)
{
    AHB_CTL0_U             *ahb_ptr     = (AHB_CTL0_U *) AHB_CTL0;
    AHB_SOFT_RST_U    *ahb_rst_ptr    = (AHB_SOFT_RST_U *) AHB_SOFT_RST;
    //enable LCDC
    ahb_ptr->mBits.lcdc_eb = 0x1;


    ahb_rst_ptr->mBits.lcdc_soft_rst = 0x1;
    ahb_rst_ptr->mBits.tve_soft_rst  = 0x1;
    SCI_Sleep (1);
    ahb_rst_ptr->mBits.lcdc_soft_rst = 0x0;
    ahb_rst_ptr->mBits.tve_soft_rst  = 0x0;

    if (0 == lcm_sel)
    {
        TB_REG_AND (LCDC_LCD_MODE, ~BIT_11);
    }
    else
    {
        TB_REG_OR (LCDC_LCD_MODE, BIT_11);
    }


}//end of LCDC_Reset
/**********************************************************************************************************/
// Description: Check if LCDC finish one brush LCD operation or not
// Global resource dependence:
// Author: Jianping.wang
// Note:
//     input  - none
//     output - none
//     return - 1:finish brush operation
//              0:don't finish brush operation
/**********************************************************************************************************/
PUBLIC BOOLEAN LCDC_CheckBrushLcdStatue (void)
{
    uint32                   mode;
    uint32                   reg_val;
    LCDC_LCM_INT_STATUS_U       *mcu_reg_ptr    = (LCDC_LCM_INT_STATUS_U *) LCDC_LCM_INT_STATUS;
    LCM_INT_STATUS_U            *lcm0_reg_ptr   = (LCM_INT_STATUS_U *) LCDC_LCM_INT_STATUS;
    LCDC_LCD_INT_STATUS_U       *rgb_reg_ptr    = (LCDC_LCD_INT_STATUS_U *) LCDC_LCD_INT_STATUS;

    mode = g_lcdc_type.lcdc_mode;

    if (mode == 1)
    {
        if (lcm_sel)
        {
            reg_val = mcu_reg_ptr->mBits.osd_done;
        }
        else
        {
            reg_val = lcm0_reg_ptr->mBits.osd_done;
        }
    }
    else
    {
        reg_val = rgb_reg_ptr->mBits.end_of_frame;
    }

    return reg_val;

}//end of LCDC_BrushLcdStatue
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_ClearStatus (void)
{
    LCDC_LCM_INT_STATUS_U       *mcu_reg_ptr = (LCDC_LCM_INT_STATUS_U *) LCDC_LCM_INT_STATUS;
    LCM_INT_STATUS_U            *lcm0_reg_ptr   = (LCM_INT_STATUS_U *) LCDC_LCM_INT_STATUS;

    if (lcm_sel)
    {
        mcu_reg_ptr->mBits.osd_done = 1;
    }
    else
    {
        lcm0_reg_ptr->mBits.osd_done = 1;
    }

}//end of LCDC_ClearStatus
#if 0
/**********************************************************************************************************/
// Description: start ISP
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
LOCAL void ISP_Config (void)
{
    uint16 *buf_ptr;

    //SCI_TRACE_LOW:"*********Enter into ISP_config()*********"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_1575_112_2_18_1_45_31_142,(uint8*)"");

    //  buf_ptr = (uint16 *)LCD_GetSubLCDBuffer();

    SCI_ASSERT ( ( (uint32) buf_ptr%8) == 0);

    * (volatile uint32 *) 0x20900200 |= 0x0000000e; //enable isp ccir and lcd
    * (volatile uint32 *) 0x8b000008 |= 0x00300000; //bit20-23 change from 2 to 3

    //reset isp
    * (volatile uint32 *) 0x20900210 |= (0x1<<1);
    OS_TickDelay (10);
    * (volatile uint32 *) 0x20900210 &= ~0x2;

    //*(volatile uint32 *)0x20900200 |= 0x2;
    * (volatile uint32 *) 0x20200000 |= (0x3<<8); //set 1 for bit8 and bit9
    * (volatile uint32 *) 0x20200008 &= ~0xf;
    * (volatile uint32 *) 0x20200008 |= 0x2;   //set MPEG Captrue mode

    //Produce a posedge
    * (volatile uint32 *) 0x20200000 &= ~0x1;
    * (volatile uint32 *) 0x20200000 |= 0x1;

    //Set SC2-PortA connection
    * (volatile uint32 *) 0x20200008 |= (0x1<<8);
    //Bypass SC2
    * (volatile uint32 *) 0x20200010 |= (0x1<<10);
    //Set no-color mode of PortA
    * (volatile uint32 *) 0x20200010 |= (0x1<<12);
    //Set SC2 trimming
    * (volatile uint32 *) 0x20200308 = 0x00dc0160;
    * (volatile uint32 *) 0x2020030c = 0x0;

    //Set PortA trimming
    * (volatile uint32 *) 0x20200318 = 0x00dc0160;
    * (volatile uint32 *) 0x2020031c = 0x00;

    //Produce a posedge
    * (volatile uint32 *) 0x20200000 &= ~0x1;
    * (volatile uint32 *) 0x20200000 |= 0x1;

    //Set PortA base-address
    * (volatile uint32 *) 0x20200358 = (uint32) buf_ptr;

    // Enable Port A
    * (volatile uint32 *) 0x20200344 |= (0x1<<3);

}//end of ISP_Config
#endif
/**********************************************************************************************************/
// Description: wait ISP finish this task
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void Wait_isp (void)
{
    uint32 count = 0;

    count = OS_GetCurrentTime();

    do
    {
        SCI_ASSERT ( (OS_GetCurrentTime() - count) < 50);
    }
    while ( (REG32 (0x2020002c) & 0x8) != 8);
}//end of Wait_isp
/**********************************************************************************************************/
// Description: start LCDC MCU MODE
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
//PUBLIC void Start_LCDC()
//{
//  uint16 left;
//  uint16 right;
//  uint16 top;
//  uint16 bottom;
//
//  SCI_TRACE_LOW("********Enter into Start_LCDC()*********");
//  left = 0x0;
//  right = 176*2-1;
//  top = 0;
//  bottom = 219;
//  ISP_Config();
//  lcdc_mcu_init(1);
//
//  LCDC_TransCmd((right-left+1),(bottom-top+1),0);
//
//  Wait_isp();
//}//end of Start_LCDC
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
//PUBLIC void Open_lcdc_path(BOOLEAN is_open)
//{
//  LCDC_LCD_MODE_U   *lcdc_mode_ptr = (LCDC_LCD_MODE_U *)LCDC_LCD_MODE;
//
//  SCI_TRACE_LOW("*********Enter into Open_lcdc_path()************");
//  if(is_open)
//  {
//      g_lcdc_on = 1;
//      isp_lcdc_init();
//  }
//  else
//  {
//      g_lcdc_on = 0;
//      lcdc_mode_ptr->dwValue = 0x0;
//  }
//}//end of Open_lcdc_path
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
//PUBLIC uint32 LCDC_GetPath(void)
//{
//  return g_lcdc_on;
//}//end of LCDC_GetPath
/**********************************************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_SelectRstFuc (BOOLEAN is_open_rst)
{

}//end of LCDC_SelectRstFuc
/**********************************************************************************************************/
// Description: close LCDC TV DAC
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/


/**********************************************************************************************************/
// Description: close LCDC TV DAC
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void LCDC_SetDisplayWindows (uint16 left,
                                    uint16 top,
                                    uint16 right,
                                    uint16 bottom)
{

    LCDC_DISP_WIN_START_ADDR_U  *lcdc_start_addr_ptr = (LCDC_DISP_WIN_START_ADDR_U *) LCDC_DISP_WIN_START_ADDR;
    LCDC_DISP_WIN_END_ADDR_U    *lcdc_end_addr_ptr   = (LCDC_DISP_WIN_END_ADDR_U *) LCDC_DISP_WIN_END_ADDR;
    lcdc_start_addr_ptr->mBits.start_x    =   left;
    lcdc_start_addr_ptr->mBits.start_y    =   top;
    lcdc_end_addr_ptr  ->mBits.end_x      =   right;
    lcdc_end_addr_ptr  ->mBits.end_y      =   bottom;

}
PUBLIC ERR_LCD_E LCDC_SetBlockBuffer (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    uint32 *buf_ptr
)
{


    return ERR_LCD_NONE;
}
/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                       *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
