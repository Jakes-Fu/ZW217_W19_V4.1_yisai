/******************************************************************************
 ** File Name:      I2C_phy_r7p0.c                                            *
 ** Author:                                                                   *
 ** DATE:           07/05/2016                                                *
 ** Copyright:      2016 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of I2C device.        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/05/2016                      Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "timer_drvapi.h"
#include "i2c_reg_uws6121e.h"
#include "pinmap_drvapi.h"

#include "ap_apb.h"
#include "aon_clk.h"
#include "sys_ctrl.h"
#include "aon_clk_gen.h"


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#define CONFIG_I2C_CLOCK 26000000

#define I2C_HIGH_OPT   1
#define I2C_DVD_OPT    0
#define I2C_TRIM_OPT   1
#define I2C_CMDBUF_SUPPORT 0
#define I2C_CMDBUF_SIZE 8
static volatile uint32 cmd_buffer_nr = 0;

#define I2C_TIMEOUT_FACTOR  500000  //the critical value is 10000



#define I2C_WAIT_INT                                                       \
    {                                                                      \
        timetick = OS_GetTickCount();                            \
        while(!(ptr->STATUS & I2C_MASTER_IRQ_STATUS))                                    \
        {                                                                  \
            uint32 wait_counter=0x00;                                      \
            for(wait_counter=0x00; wait_counter<0xff;wait_counter++)       \
            {                                                              \
                wait_counter++;                                            \
            }                                                              \
            if((OS_GetTickCount() - timetick) >= g_i2c_timeout) \
            {                                                              \
                if(ERR_I2C_NONE == ret_value)                              \
                {                                                          \
                    ret_value = ERR_I2C_INT_TIMEOUT;                       \
                }                                                          \
                break;                                                     \
            }                                                              \
            \
        }                                                                  \
        \
    }

#define I2C_WAIT_BUSY                                                       \
    {                                                                       \
        timetick = OS_GetTickCount();                             \
        while(ptr->STATUS & I2C_MASTER_BUSY)                                        \
        {                                                                   \
            if ((OS_GetTickCount() - timetick) >= g_i2c_timeout) \
            {                                                               \
                if(ERR_I2C_NONE == ret_value)                               \
                {                                                           \
                    ret_value = ERR_I2C_BUSY_TIMEOUT;                       \
                }                                                           \
                break;                                                      \
            }                                                               \
        }                                                                   \
                                                                            \
    }
#define I2C_WAIT_ACK                                                        \
    {                                                                       \
        timetick = OS_GetTickCount();                             \
        while(ptr->STATUS & I2C_MASTER_RXACK)                                        \
        {                                                                   \
            if ((OS_GetTickCount() - timetick) >= g_i2c_timeout) \
            {                                                               \
                if(ERR_I2C_NONE == ret_value)                               \
                {                                                           \
                    ret_value = ERR_I2C_ACK_TIMEOUT;                        \
                }                                                           \
                break;                                                      \
            }                                                               \
        }                                                                   \
    }

#define I2C_CLEAR_INT                                                      \
    {                                                                      \
        ptr->IRQ_CLR = I2C_MASTER_IRQ_CLR;                                  \
    }


#define I2C_WAIT_TIP                                                      \
    {                                                                      \
        timetick = OS_GetTickCount();                            \
        while(ptr->STATUS & I2C_MASTER_TIP)                                    \
        {                                                                  \
            uint32 wait_counter=0x00;                                      \
            for(wait_counter=0x00; wait_counter<0xff;wait_counter++)       \
            {                                                              \
                wait_counter++;                                            \
            }                                                              \
            if((OS_GetTickCount() - timetick) >= g_i2c_timeout) \
            {                                                              \
                if(ERR_I2C_NONE == ret_value)                              \
                {                                                          \
                    ret_value = ERR_I2C_INT_TIMEOUT;                       \
                }                                                          \
                break;                                                     \
            }                                                              \
            \
        }                                                                  \
        \
    }

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/
PUBLIC volatile uint32     g_i2c_timeout = 2; //unit is ms
extern const I2C_BASE_INFO __i2c_base_info[I2C_BUS_MAX];

/*********************************************************************************************************
** Function name:__I2C_PHY_GetBase
** Descriptions:this fun is used to get base address
*********************************************************************************************************/
LOCAL uint32  __I2C_PHY_GetBase (uint32 phy_id)
{
    int32 i;
    uint32 ret = 0;

    for (i = 0; i < I2C_BUS_MAX; i++)
    {
        if (phy_id == (uint32) __i2c_base_info[i].phy_id)
        {
            ret = (uint32) __i2c_base_info[i].base_addr;
            break;
        }
    }

    SCI_PASSERT ( (0 != ret), ("get I2C controller base address fail!"));/*assert verified*/
    return ret;
}
/*********************************************************************************************************
** Function name:__I2C_PHY_SetSCL
** Descriptions:this fun is used to set freq
*********************************************************************************************************/
LOCAL ERR_I2C_E __I2C_PHY_SetSCL (uint32 phy_id, uint32 freq)
{
	uint32 ctrlReg, pre_scale;
	volatile HWP_I2C_MASTER_T *ptr = (volatile HWP_I2C_MASTER_T *) __I2C_PHY_GetBase (phy_id);
	//APB_clk= CHIP_GetAPBClk();

	ctrlReg = ptr->CTRL & ~(I2C_MASTER_CLOCK_PRESCALE_MASK);

	pre_scale = CONFIG_I2C_CLOCK / (5 * freq) - 1;

	ctrlReg |= I2C_MASTER_CLOCK_PRESCALE(pre_scale);
	// Set the new clock scal.
	ptr->CTRL = ctrlReg;
	g_i2c_timeout = I2C_TIMEOUT_FACTOR / (freq);

	if (g_i2c_timeout < 2)
	{
		g_i2c_timeout = 2;
	}

	return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:__I2C_PHY_SetPort
** Descriptions:this fun is used to set pad group
*********************************************************************************************************/
LOCAL ERR_I2C_E __I2C_PHY_SetPort (uint32 phy_id, uint32 port)
{
    IIC_PRINT (("[IIC DRV:]__I2C_PHY_SelPort:  phy_id=%x, port=%x", phy_id, port));

    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_ControlInit_V0
** Descriptions:this fun is used to init controler
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_ControlInit_V0 (uint32 phy_id, uint32 freq, uint32 port)
{
	uint32 second_time;
	REG_AP_APB_CLK_AP_EN1_T APB_CLK_AP_EN1;
	REG_AP_APB_CLK_AP_EN0_T APB_CLK_AP_EN0;
	REG_AON_CLK_CGM_I2C3_SEL_CFG_T AON_CLK_CGM_I2C3_SEL_CFG;
	REG_AON_CLK_GEN_GATE_EN_SEL0_CFG_T AON_CLK_GEN_GATE_EN_SEL0_CFG;
	REG_AON_CLK_GEN_GATE_EN_SW_CTL0_CFG_T AON_CLK_GEN_GATE_EN_SW_CTL0_CFG;

	volatile HWP_I2C_MASTER_T *ptr = (volatile HWP_I2C_MASTER_T *) __I2C_PHY_GetBase (phy_id);

	switch(phy_id)
	{
		case 0:

			APB_CLK_AP_EN0.v = hwp_apApb->clk_ap_en0;
			APB_CLK_AP_EN0.b.clk_en_i2c1_fr = 1;
			hwp_apApb->clk_ap_en0 = APB_CLK_AP_EN0.v;

			APB_CLK_AP_EN1.v = hwp_apApb->clk_ap_en1;
			APB_CLK_AP_EN1.b.clk_en_i2c1 = 1;
			hwp_apApb->clk_ap_en1 = APB_CLK_AP_EN1.v;
		break;
		case 1:

			APB_CLK_AP_EN0.v = hwp_apApb->clk_ap_en0;
			APB_CLK_AP_EN0.b.clk_en_i2c2_fr = 1;
			hwp_apApb->clk_ap_en0 = APB_CLK_AP_EN0.v;

			APB_CLK_AP_EN1.v = hwp_apApb->clk_ap_en1;
			APB_CLK_AP_EN1.b.clk_en_i2c2 = 1;
			hwp_apApb->clk_ap_en1 = APB_CLK_AP_EN1.v;
		break;
		case 2:

			#if 0
			AON_CLK_GEN_GATE_EN_SEL0_CFG.v = hwp_aonClkGen->gate_en_sel0_cfg;
			AON_CLK_GEN_GATE_EN_SEL0_CFG.b.cgm_xtal_26m_aon_auto_gate_sel = 0; //soft ctrl
			//AON_CLK_GEN_GATE_EN_SEL0_CFG.b.cgm_xtal_26m_ap_auto_gate_sel = 0;
			hwp_aonClkGen->gate_en_sel0_cfg = AON_CLK_GEN_GATE_EN_SEL0_CFG.v;
			SCI_Sleep(10);

			AON_CLK_GEN_GATE_EN_SW_CTL0_CFG.v = hwp_aonClkGen->gate_en_sw_ctl0_cfg;
			AON_CLK_GEN_GATE_EN_SW_CTL0_CFG.b.cgm_xtal_26m_aon_force_en = 1;
			//AON_CLK_GEN_GATE_EN_SW_CTL0_CFG.b.cgm_xtal_26m_ap_force_en = 1;
			//AON_CLK_GEN_GATE_EN_SW_CTL0_CFG.cgm_xtal_lp_26m_aon_force_en = 1;
			//AON_CLK_GEN_GATE_EN_SW_CTL0_CFG.cgm_xtal_26m_pub_force_en = 1;
			hwp_aonClkGen->gate_en_sw_ctl0_cfg = AON_CLK_GEN_GATE_EN_SW_CTL0_CFG.v;
			SCI_Sleep(10);

			AON_CLK_CGM_I2C3_SEL_CFG.v = hwp_aonClk->cgm_i2c3_sel_cfg;
			AON_CLK_CGM_I2C3_SEL_CFG.b.cgm_i2c3_sel = 1;  //xtal_26m
			hwp_aonClk->cgm_i2c3_sel_cfg = AON_CLK_CGM_I2C3_SEL_CFG.v; //need debug
			SCI_Sleep(10);

			hwp_sysCtrl->aon_clock_auto_sel1_clr = (uint32)((1<<12)|(1<<10)|(1<<9)); //cfg to soft
			hwp_sysCtrl->aon_clock_force_en1_set = (uint32)((1<<12)|(1<<10)|(1<<9)); //enable clk
			#endif

			AON_CLK_CGM_I2C3_SEL_CFG.v = hwp_aonClk->cgm_i2c3_sel_cfg;
			AON_CLK_CGM_I2C3_SEL_CFG.b.cgm_i2c3_sel = 1;  //1:xtal_26m   4: apll_200m
			hwp_aonClk->cgm_i2c3_sel_cfg = AON_CLK_CGM_I2C3_SEL_CFG.v;

			hwp_sysCtrl->aon_clock_auto_sel1_clr = (uint32)((1<<11)|(1<<9)|(1<<8)); //cfg to soft
			hwp_sysCtrl->aon_clock_force_en1_set = (uint32)((1<<11)|(1<<9)|(1<<8)); //enable clk

		break;

		default:
		//assert
		break;
	}

	ptr->CTRL &= ~I2C_MASTER_EN; //you must first disable i2c module then change clock

	__I2C_PHY_SetSCL (phy_id, freq);

	if (I2C_PORT_NUM < port)
	{
	    SCI_ASSERT (0);/*assert to do*/
	}

	__I2C_PHY_SetPort (phy_id, port);

	// Enable the I2c
	ptr->CTRL |= I2C_MASTER_EN;
	ptr->CMD  = I2C_MASTER_STO;
    
    TIMER_DELAY_US(10); // add by bao 解决音频待机电流10ma的问题
        
    IIC_PRINT (("[IIC DRV:]I2C_PHY_ControlInit_V0: freq=%d, port=%d", freq, port));
    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_StartBus_V0
** Descriptions:this fun is used to send start cmd
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_StartBus_V0 (uint32 phy_id, uint8 addr, BOOLEAN rw, BOOLEAN ack_en)
{
    uint32 timetick = 0;
    volatile HWP_I2C_MASTER_T *ptr = (volatile HWP_I2C_MASTER_T *) __I2C_PHY_GetBase (phy_id);
    uint32   ret_value = ERR_I2C_NONE;

    if (rw)
    {
        /*read cmd*/
        ptr->TXRX_BUFFER =  (uint32) (addr |0x1);
    }
    else
    {
        /*write cmd*/
        ptr->TXRX_BUFFER = (uint32) addr;
    }

    ptr->CMD = I2C_MASTER_STA | I2C_MASTER_RW;

    I2C_WAIT_INT
    I2C_WAIT_TIP
    I2C_CLEAR_INT

    //check ACK
    if (ack_en)
    {
        I2C_WAIT_ACK
    }

    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_WriteBytes_V0
** Descriptions:this fun is used to send write cmd
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_WriteBytes_V0 (uint32 phy_id, uint8 *pCmd, uint32 len, BOOLEAN ack_en, BOOLEAN no_stop)
{
	uint32 timetick = 0;
	uint32 i = 0;
	uint32 cmd;
	volatile HWP_I2C_MASTER_T *ptr = (volatile HWP_I2C_MASTER_T *) __I2C_PHY_GetBase (phy_id);
	uint32   ret_value = ERR_I2C_NONE;

	for (i=0; i<len; i++)
	{
		cmd = I2C_MASTER_RW ;
		ptr->TXRX_BUFFER = ( (uint32) pCmd[i]);
		if ( (i== (len-1)) && (!no_stop))
		{
			cmd |= I2C_MASTER_STO;
		}
		ptr->CMD = cmd;

		I2C_WAIT_INT
        I2C_WAIT_TIP
		I2C_CLEAR_INT

		//check ACK
		if (ack_en)
		{
			I2C_WAIT_ACK
		}

	}
	return ret_value;
}


/*********************************************************************************************************
** Function name:I2C_PHY_ReadBytes_V0
** Descriptions:this fun is used to send read cmd
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_ReadBytes_V0 (uint32 phy_id, uint8 *pCmd, uint32 len, BOOLEAN ack_en)
{
	uint32 timetick = 0;
	uint32 i = 0;
	uint32 cmd = 0;

	volatile HWP_I2C_MASTER_T *ptr = (volatile HWP_I2C_MASTER_T *) __I2C_PHY_GetBase (phy_id);
	uint32 ret_value = ERR_I2C_NONE;

	for (i = 0;  i < len; i++)
	{
		cmd = I2C_MASTER_RD;

		if (i == (len-1))
		{
			cmd |= I2C_MASTER_ACK | I2C_MASTER_STO;
		}

		ptr->CMD = cmd;

	    I2C_WAIT_INT
        I2C_WAIT_TIP
		I2C_CLEAR_INT

		pCmd[i] = (uint8)(ptr->TXRX_BUFFER);
	}

	return ret_value;
}

/*********************************************************************************************************
** Function name:I2C_PHY_StopBus_V0
** Descriptions:this fun is used to send stop cmd
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_StopBus_V0 (uint32 phy_id)
{
    uint32 timetick = 0;
    volatile HWP_I2C_MASTER_T *ptr = (volatile HWP_I2C_MASTER_T *) __I2C_PHY_GetBase (phy_id);
    uint32   ret_value = ERR_I2C_NONE;
    ptr->CMD = I2C_MASTER_STO;
    I2C_WAIT_INT
    I2C_CLEAR_INT
    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_SendACK_V0
** Descriptions:this fun is used to send ack
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_SendACK_V0 (uint32 phy_id)
{
    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_GetACK_V0
** Descriptions:this fun is used to get ack
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_GetACK_V0 (uint32 phy_id)
{
    return ERR_I2C_NONE;
}

PUBLIC I2C_PHY_FUN phy_fun_v0 =
{
    I2C_PHY_ControlInit_V0,
    I2C_PHY_StartBus_V0,
    I2C_PHY_WriteBytes_V0,
    I2C_PHY_ReadBytes_V0,
    I2C_PHY_StopBus_V0,
    I2C_PHY_SendACK_V0,
    I2C_PHY_GetACK_V0
};
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/


#ifdef   __cplusplus
}
#endif

/*  End Of File */
