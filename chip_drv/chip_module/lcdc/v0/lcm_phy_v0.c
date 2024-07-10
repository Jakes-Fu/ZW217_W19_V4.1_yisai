/***************************************************************************************
** File Name:      SC6800_lcm.c                                                          *
** DATE:           23/09/2005                                                           *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.                    *
** Description:    this file defines LCM interfaces,                                   *
**                   these interfaces is visible for LCM's user                          *
****************************************************************************************

****************************************************************************************
**                         Edit History                                                  *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                                                  *
** 23/09/2005                Create.                                                       *
****************************************************************************************/

/**------------------------------------------------------------------------------------*
 **                         Dependencies                                                  *
 **------------------------------------------------------------------------------------*/

#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "lcdc_lcm_reg_v0.h"
#include "dal_lcd.h"
#include "lcm_drvapi.h"
#include "dma_drv_internal.h"
#include "ref_outport.h"
#include "lcd_cfg.h"

#include "../../dma/dma_phy.h"
#include "../../dma/v0/dma_reg_v0.h"

/**------------------------------------------------------------------------------------*
 **                        Compiler Flag                                                   *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**------------------------------------------------------------------------------------*
 **                        Protocol Define                                             *
 **------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
 **                        Macro Define                                                    *
 **------------------------------------------------------------------------------------*/
#define SHIFT_NUM   18
#define BLOCK_NUM           (0x1 << SHIFT_NUM)
#define LCM_DMA_TIMEOUT     500 //ms

#define LCM_DMA_BURST_SIZE  32  //Refer to LCM FIFO DEPTH

PUBLIC LCMCallBackFunc s_lcm_callbackfunc = NULL;

LCM_ADDR_T  g_lcm_addr[6];

//- Define the Linklist of DMA
__align(32) DMA_CHN_CTL_REG_T dma_channelcfg_llist[MAX(LCD_WIDTH, LCD_HEIGHT)] = {0} ;
/**********************************************************************************************************/
// Description:    initialize LCM
// Global resource dependence
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/

PUBLIC void LCM_Init (void)
{

    //enable LCM
    AHB_CTL0_U *pAhb = (AHB_CTL0_U *) AHB_CTL0;

    pAhb->mBits.lcm_eb = SCI_TRUE;

    REG32 (LCM_PARAMETER0) = MAX_LCM_PARAM_VALUE;
    REG32 (LCM_PARAMETER1) = MAX_LCM_PARAM_VALUE;

    REG32 (LCM_RD_MODE) = 0x0;

    //DMA_RegCallBack(DMA_NLC, LCD_DMA_Callback);
}

/**********************************************************************************************************/
// Description:    LCM reset LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
//     reset sequence:1-0-delay-1
//       delay_ms - during reset,need to add wait time
/**********************************************************************************************************/
PUBLIC void LCM_ResetLcd (uint32 delay_ms,uint8 rst_level)
{
    LCM_RSTN_U    *pLcm_rst = (LCM_RSTN_U *) LCM_RSTN;

    pLcm_rst->dwValue = ~rst_level;

    OS_TickDelay (delay_ms);

    pLcm_rst->dwValue = rst_level;

    OS_TickDelay (delay_ms);

    pLcm_rst->dwValue = ~rst_level;

    OS_TickDelay (delay_ms);

    //SCI_TRACE_LOW("reset time = %dms,rst_level = %d",delay_ms,rst_level);

}

/**********************************************************************************************************/
// Description: register handle function
// Global resource dependence: s_lcm_callbackfunc
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC BOOLEAN LCM_RegisterCallBack (uint32 int_num, LCMCallBackFunc func)
{

    SCI_ASSERT (PNULL != func);/*assert verified*/

    s_lcm_callbackfunc = func;

    return SCI_TRUE;

}//end of LCM_RegisterInterrupt

/**********************************************************************************************************/
// Description:    send command and data to LCD
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: Jianping.wang
// Note:
//         input:
//             cmd - lcd command
//            data - lcd command data
//            lcd_cs - LCD cs
//         return:
//            ERR_LCD_NONE,if success
//            other,if fail
/**********************************************************************************************************/
PUBLIC ERR_LCD_E LCM_SendCmdData (uint32 cmd,uint32 data,uint32 lcd_id)
{
    uint32 cmd_addr;
    uint32 data_addr;

    cmd_addr = g_lcm_addr[lcd_id].cmd_addr;
    data_addr = g_lcm_addr[lcd_id].data_addr;

    //wait for if AHB FIFO full
    while (REG32 (LCM_STATUS) & BIT_1) {};

    CHIP_REG_SET (cmd_addr,cmd);

    CHIP_REG_SET (data_addr,data);

    return ERR_LCD_NONE;
}

/**********************************************************************************************************/
// Description:    send command to LCD
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: Jianping.wang
// Note:
//         input:
//             cmd - LCD command
//            lcd_cs - LCD cs
//         return:
//          ERR_LCD_NONE,if success
//            other,if fail
/**********************************************************************************************************/
PUBLIC ERR_LCD_E LCM_SendCmd (uint32 cmd,uint32 lcd_id)
{
    uint32 cmd_addr;

    cmd_addr = g_lcm_addr[lcd_id].cmd_addr;

    //wait for if AHB FIFO full
    while (REG32 (LCM_STATUS) & BIT_1) {};

    CHIP_REG_SET (cmd_addr,cmd);


    return ERR_LCD_NONE;
}

/**********************************************************************************************************/
// Description:    send data to LCD GRAM
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: Jianping.wang
// Note:
//         input:
//            data - display data
//            lcd_cs - LCD cs
//         return:
//            ERR_LCD_NONE,if success
//            other, if fail
/**********************************************************************************************************/
PUBLIC ERR_LCD_E LCM_SendData (uint32 data,uint32 lcd_id)
{
    uint32 data_addr;

    data_addr = g_lcm_addr[lcd_id].data_addr;

    //wait for if AHB FIFO full
    while (REG32 (LCM_STATUS) & BIT_1) {};

    CHIP_REG_SET (data_addr,data);

    return ERR_LCD_NONE;
}



/**********************************************************************************************************/
// Description:    NLC DMA Callback
// Global resource dependence:
// Author: younger
/**********************************************************************************************************/

LOCAL void _LCM_DMACallback (uint32 param)
{
    DMA_HAL_ChnEnable (DMA_NLC,SCI_FALSE);

    if (s_lcm_callbackfunc)
    {
        s_lcm_callbackfunc();
    }
}

/**********************************************************************************************************/
// Description:    send data to LCD GRAM use DMA
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: younger
// Note:
/**********************************************************************************************************/

uint32 LCM_DMA_Write (uint32 *pBuf,uint32 cs_addr,uint32 len,LCD_DATA_SIZE_E data_size,LCD_BRUSH_MODE_E brush_mode)
{
    uint32          pre_tick = 0;
    uint32          cur_tick = 0;
    uint32          trans_times;
    uint32          data_buf;
    DMA_CHNCFG_T    dma_ctl_lcm = {0};
    DMA_CAHCECONFIG_T cacheConfig[1];

    SCI_ASSERT ( (uint32) pBuf > 0);/*assert verified*/
    SCI_ASSERT (cs_addr      > 0);/*assert verified*/
    SCI_ASSERT (len          > 0);/*assert verified*/

    data_buf    = (uint32) pBuf;
    trans_times = (len >> (SHIFT_NUM)) + 1;

    while ( ( (trans_times--) > 0) && (len > 0))
    {
        SCI_MEMSET (&dma_ctl_lcm, 0, sizeof (DMA_CHNCFG_T));
        dma_ctl_lcm.type           = DMA_CHN_TYPE_HARD;
        dma_ctl_lcm.priority       = DMA_CHN_PRIORITY_0;
        dma_ctl_lcm.workmode       = DMA_CHN_WORKMODE_NORMAL;
        dma_ctl_lcm.src_address     = (uint32) data_buf;
        dma_ctl_lcm.dest_address   = cs_addr;
        dma_ctl_lcm.requestmode    = DMA_CHN_REQMODE_NORMAL;
        dma_ctl_lcm.total_length   = MIN (len, BLOCK_NUM);

        cacheConfig[0].bufferAddr = (uint32) data_buf;
        cacheConfig[0].bufferSize = dma_ctl_lcm.total_length;
        cacheConfig[0].dir = DMABUFFER_TO_DEVICE;

        SCI_ASSERT (dma_ctl_lcm.src_address  > 0);/*assert verified*/
        SCI_ASSERT (dma_ctl_lcm.dest_address  > 0);/*assert verified*/
        SCI_ASSERT (dma_ctl_lcm.total_length   > 0);/*assert verified*/

        len         -= MIN (len, BLOCK_NUM);
        data_buf    += BLOCK_NUM;

        if (data_size == DATA_SIZE_BYTE)
        {
            dma_ctl_lcm.block_length   = 0x10;

            if (0 == (dma_ctl_lcm.src_address % 4))
            {
                dma_ctl_lcm.src_datawidth  = DMA_DATAWIDTH_WORD;
                dma_ctl_lcm.dest_datawidth = DMA_DATAWIDTH_BYTE;
                dma_ctl_lcm.src_step       = 4;  // src increament
            }

            else if (0 == (dma_ctl_lcm.src_address % 2))
            {
                dma_ctl_lcm.src_datawidth  = DMA_DATAWIDTH_HALFWORD;
                dma_ctl_lcm.dest_datawidth = DMA_DATAWIDTH_BYTE;
                dma_ctl_lcm.src_step       = 2;  // src increament
            }
            else
            {
                dma_ctl_lcm.src_datawidth  = DMA_DATAWIDTH_BYTE;
                dma_ctl_lcm.dest_datawidth = DMA_DATAWIDTH_BYTE;
                dma_ctl_lcm.src_step       = 1;  // src increament
            }
        }
        else if (data_size == DATA_SIZE_HALFWORD)
        {
            dma_ctl_lcm.block_length = 0x20;

            if (0 == (dma_ctl_lcm.src_address % 4))
            {
                dma_ctl_lcm.src_datawidth  = DMA_DATAWIDTH_WORD;
                dma_ctl_lcm.dest_datawidth = DMA_DATAWIDTH_HALFWORD;
                dma_ctl_lcm.src_step       = 4;  // src increament
            }

            else if (0 == (dma_ctl_lcm.src_address % 2))
            {
                dma_ctl_lcm.src_datawidth  = DMA_DATAWIDTH_HALFWORD;
                dma_ctl_lcm.dest_datawidth = DMA_DATAWIDTH_HALFWORD;
                dma_ctl_lcm.src_step       = 2;  // src increament
            }
            else
            {
                SCI_ASSERT (0);/*assert to do*/
            }
        }
        else if (data_size == DATA_SIZE_WORD)
        {
            dma_ctl_lcm.block_length   = 0x20;
            dma_ctl_lcm.src_datawidth  = DMA_DATAWIDTH_WORD;
            dma_ctl_lcm.dest_datawidth = DMA_DATAWIDTH_WORD;
            dma_ctl_lcm.src_step       = 4;  // src increament
        }

        dma_ctl_lcm.dest_step = 0;  // dest nochange

        if (LCD_BRUSH_MODE_ASYNC == brush_mode)
        {
            DMA_HAL_ChnConfig (DMA_NLC, &dma_ctl_lcm);
            DMA_HAL_ChnIntConfig (DMA_NLC, DMA_CHN_INT_TYPE_TRANSDONE, _LCM_DMACallback);
            DMA_HAL_ChnEnable_Ex (DMA_NLC, SCI_TRUE, cacheConfig, 1);
            //           DMA_HAL_ChnEnable (DMA_NLC,SCI_TRUE);
        }
        else
        {
            DMA_HAL_ChnIntConfig (DMA_NLC, DMA_CHN_INT_TYPE_NONE, NULL);
            DMA_HAL_ChnEnable_Ex (DMA_NLC, SCI_TRUE, cacheConfig, 1);
            //            DMA_HAL_ChnEnable (DMA_NLC,SCI_TRUE);

            pre_tick = SCI_GetTickCount();

            while (!DMA_HAL_PollingTransDone (DMA_NLC))
            {
                cur_tick = SCI_GetTickCount();

                if (cur_tick < pre_tick)
                {
                    continue;
                }

                if (cur_tick - pre_tick >= LCM_DMA_TIMEOUT)
                {
                    DMA_HAL_ChnEnable (DMA_NLC,SCI_FALSE);
                    return SCI_TRUE;
                }
            }

            //disable DMA channel when transaction was done
            DMA_HAL_ChnEnable (DMA_NLC, SCI_FALSE);
        }

    }

    return SCI_TRUE;
}


/*********************************************************************/
//  Description: input lcd parameter struct to LCM Controller
//  Input:    lcd_spec_ptr        lcm spec struct pointer
//  Return: 0           successful
//          others      failed
//  Note:
//            None
/*********************************************************************/
PUBLIC ERR_LCD_E LCM_RegLCDInfo (LCD_SPEC_INFO_T_PTR lcd_spec_ptr)
{

    SCI_ASSERT (NULL != lcd_spec_ptr);/*assert verified*/

    LCM_SetMode (lcd_spec_ptr);

    return ERR_LCD_NONE;


}

/*********************************************************************/
//  Description: change bus priority for LCM controller
//  Input:    priority    bus priority
//  Return: 0           successful
//          others      failed
//  Note:
//      If WriteBusShare bit is SCI_TRUE, control module will share the bus
//     to manipulate the LCM during the busy phase in NAND both program and erase operation.
//     Otherwise, the control module will never active LCM during either program or erase operation.
//        31                            30                29    0
//        WriteBusShare     Reserved     Priority
//        1: share;
//        0: unshare
/*********************************************************************/
PUBLIC ERR_LCD_E LCM_SetBusRatio (uint32 ratio)
{

    SCI_ASSERT (ratio < 0x3fff);/*assert verified*/
    //Priority field configures the time ratio of LCM operation to NAND operation.
    CHIP_REG_OR (LCM_BUS_RATIO, (1UL << 31) | (ratio));

    return ERR_LCD_NONE;
}



PUBLIC void LCM_SetCSAddr (uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{

    uint32 reg_base = LCM_CS_BASE_ADDR;
    uint32 cd_offset = 0;


    //set cs address
    reg_base = LCM_CS_BASE_ADDR + lcd_cs * 0x100;
    cd_offset = 1 << (lcd_cd + 2);

    g_lcm_addr[lcd_id].cmd_addr  = reg_base;
    g_lcm_addr[lcd_id].data_addr = reg_base + cd_offset;

    return ;

}

uint32 LCM_GetDataAddr (uint32 lcd_id)
{
    return g_lcm_addr[lcd_id].data_addr;

}


/*********************************************************************/
//  Description: Set LCM parameter for LCM Device0 selected by lcm_cs[0].
//  Input:    None
//  Return: 0           successful
//          others      failed
//  Note:

//    Mode Selection Register:
//    31     6    5         4          3           2        1         0
//    Reserved    ModeSel5 ModeSel4 ModeSel3 ModeSel2 ModeSel1 ModeSel0
//                0: 8080     0: 8080  0: 8080  0: 8080  0: 8080     0: 8080
//                1: 6800     1: 6800  1: 6800  1: 6800    1: 6800     1: 6800
/*********************************************************************/
PUBLIC ERR_LCD_E LCM_SetMode (LCD_SPEC_INFO_T_PTR lcd_spec_ptr)
{

    SCI_ASSERT (NULL != lcd_spec_ptr);/*assert verified*/

    /* set LCM MODE Selection */
    if (lcd_spec_ptr->spec_ptr->bus_mode == BUS_MODE_6800) //modesel=1
    {
        CHIP_REG_OR (LCM_IFMODE, 1UL << lcd_spec_ptr->cs_pin);
    }
    else if (lcd_spec_ptr->spec_ptr->bus_mode == BUS_MODE_8080) //modesel=0
    {
        CHIP_REG_AND (LCM_IFMODE, ~ (1UL << lcd_spec_ptr->cs_pin));
    }
    else
    {
        SCI_PASSERT (SCI_FALSE, ("LCM controller don't support this mode"));/*assert to do*/
    }


    return ERR_LCD_NONE;
}


/**********************************************************************************************************/
// Description:change pulse width
// Global resource dependence:
// Author: Jianping.wang
// Note:
//         input:
//            ahb_clk - AHB bus clock
//         return:
//          ERR_LCD_NONE,if success
//            other,if fail
/**********************************************************************************************************/
PUBLIC ERR_LCD_E LCM_SetParam (LCD_SPEC_INFO_T_PTR lcd_spec_ptr, uint32 ahb_clk)
{
    LCM_PARAMTER0_U     *pLcm_para0     = (LCM_PARAMTER0_U *) LCM_PARAMETER0;
    LCM_PARAMTER1_U     *pLcm_para1     = (LCM_PARAMTER1_U *) LCM_PARAMETER1;
    LCM_PARAMTER0_U     Lcm_para_value;
    uint32              ns_per_clk      = 0;
    uint32              adjustment_factor= 0;

    //Caculate the number of ns for one clk
    adjustment_factor = (((1000000000 >> 20) % (ahb_clk >> 20)) == 0) ? 0:1;
    ns_per_clk        = (1000000000 >> 20) / (ahb_clk >> 20) - adjustment_factor; 

    SCI_ASSERT (NULL != ns_per_clk);/*assert verified*/
    adjustment_factor         = (((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rcss) % ns_per_clk) == 0) ? 0:1;
    Lcm_para_value.mBits.rcss = MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rcss) / ns_per_clk) + adjustment_factor), 0x3);
    adjustment_factor         = (((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rlpw) % ns_per_clk) == 0) ? 0:1;
    Lcm_para_value.mBits.rlpw = MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rlpw) / ns_per_clk) + adjustment_factor), 0xe);
    adjustment_factor         = (((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rhpw) % ns_per_clk) == 0) ? 0:1;
    Lcm_para_value.mBits.rhpw = MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rhpw) / ns_per_clk) + adjustment_factor), 0xe);
    adjustment_factor         = (((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.wcss) % ns_per_clk) == 0) ? 0:1;
    Lcm_para_value.mBits.wcss = MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.wcss) / ns_per_clk) + adjustment_factor), 0x3);
    adjustment_factor         = (((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.wlpw) % ns_per_clk) == 0) ? 0:1;
    Lcm_para_value.mBits.wlpw = MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.wlpw) / ns_per_clk) + adjustment_factor), 0xe);
    adjustment_factor         = (((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.whpw) % ns_per_clk) == 0) ? 0:1;
    Lcm_para_value.mBits.whpw = MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.whpw) / ns_per_clk) + adjustment_factor), 0xe);

    //wait for until AHB FIFO empty(1)[BIT_2]
    while (! (REG32 (LCM_STATUS)  & BIT_2)) {};

    if (0 == (lcd_spec_ptr->cs_pin))
    {
        pLcm_para0->dwValue = Lcm_para_value.dwValue;

    }
    else
    {
        pLcm_para1->dwValue = Lcm_para_value.dwValue;

    }

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:  set dest LCD's RGB mode to host
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC void LCM_SetRGBMode (uint32 cs_id,LCD_BUS_WIDTH_E rgb_mode)
{
    uint32 offset = cs_id<<1;

    //wait for until AHB FIFO empty(1)[BIT_2]
    while (! ( (REG32 (LCM_STATUS)) & BIT_2)) {};

    switch (rgb_mode)
    {
        case WIDTH_16:
            REG32 (LCM_RGBMODE) |= (0x0<<offset);
            break;
        case WIDTH_18:
            REG32 (LCM_RGBMODE) |= (0x1<<offset);
            break;
        case WIDTH_8:
            REG32 (LCM_RGBMODE) |= (0x2<<offset);
            break;
        case WIDTH_9:
            REG32 (LCM_RGBMODE) |= (0x3<<offset);
            break;
        default:
            break;
    }
}

/*****************************************************************************/
//  Description:  set the number of pixes in the next frame from OSD
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
LOCAL  uint32 s_data_number;
LOCAL  BOOLEAN s_run_flag = SCI_FALSE;
PUBLIC void LCM_SetDataNumber (LCD_SPEC_INFO_T_PTR lcd_spec_ptr, uint32 number)
{

    s_data_number = ( (0x7& (lcd_spec_ptr->cs_pin)) <<26) | ( (0x3f& (0x1<< (lcd_spec_ptr->cd_pin))) <<20) | (0xfffff&number);

    s_run_flag =SCI_TRUE;
}

PUBLIC void LCM_SetDataNumberConfirm (void)
{
    if (SCI_TRUE== s_run_flag)
    {
        REG32 (LCM_DATANUMBER) = s_data_number;
    }

    s_run_flag =SCI_FALSE;
}

/*********************************************************************/
//  Description:   Read LCM
//  Global resource dependence:
//  Author: Younger.Yang
//  Note: You may read LCD ID before lcd init,otherwise
//  the read data may be incorrect.
/*********************************************************************/

PUBLIC uint32 LCM_Read (uint32 lcd_id, uint32 cd_value)
{
    uint32 value;
    //Wait until AHB_fifo is empty
    LCM_WAIT_AHBFIFO_EMPTY

    REG32 (LCM_RD_MODE) |= BIT_0;

    if (cd_value == DATA_LEVEL)
    {
        LCM_SendData (BIT_18, lcd_id);
    }
    else if (cd_value == CMD_LEVEL)
    {
        LCM_SendData (BIT_18, lcd_id);
    }
    else
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    //Wait until AHB_fifo is empty
    LCM_WAIT_AHBFIFO_EMPTY

    //Need wait LCM Read Done
    do
    {
        volatile uint32 k = 0;

        for (k = 0; k < 15; k++) {};
    }
    while (0);

    value = REG32 (LCM_RDDATA);                //read data from register

    REG32 (LCM_RD_MODE) &= ~BIT_0;

    return value;
}

LOCAL void _LCM_WaitDMA_AllLinkDone (uint32 chn, uint32 time)
{
    uint32 pre_tick = 0;
    uint32 cur_tick = 0;

    if (SCI_InThreadContext())
    {
        SCI_Sleep (time);
    }

    //else
    {
        pre_tick = SCI_GetTickCount();

        while( !DMA_HAL_PollingListDone(chn) )
        {
            cur_tick = SCI_GetTickCount();

            if (cur_tick < pre_tick)
            {
                continue;
            }

            if (cur_tick - pre_tick >= LCM_DMA_TIMEOUT)
            {
                //SCI_ASSERT(0);
                DMA_HAL_ChnEnable (chn,SCI_FALSE);
                //SCI_TRACE_LOW:"_LCM_WaitDMA_AllLinkDone: time out !!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V0_631_112_2_17_23_5_6_1039,(uint8*)"");
                break;
            }
        }
    }
}

//---------------------------------------------------------------------//
LOCAL void LCM_DMALinkCallback_handler ( uint32 param )
{
	// Disable DMA Channel
    DMA_HAL_ChnEnable (DMA_NLC,SCI_FALSE);

    if (s_lcm_callbackfunc)
    {
        s_lcm_callbackfunc();
    }
    //SCI_TRACE_LOW("LCM_DmaWrite(): ISR out_timer=%d", SCI_GetTickCount());    
}

// ------------------------------------------------------------------------------------//
//  Description:   construct the dma linklist node.
//  Author:        lichd
//  Parameter:     
//  Note:          used for dma linklist mode
// ------------------------------------------------------------------------------------//
LOCAL void _DMA_LINKLIST_CONSTRUCT( 
                                    DMA_CHN_CTL_REG_T* linklist,uint32 node_num, int32 node_step, 
                                    uint32 dma_cfg0, uint32 dma_total_size, uint32 dma_src_addr, uint32 dma_des_addr,
                                    uint32 llptr,    uint32 dma_di,         uint32 dma_sbi,      uint32 dma_dbi
                                  )      
{                                                                                                        
    uint32 i=0;  
    
    for(i=0; i<node_num-1; i++)                                                                               
    {                                                                                                    
        linklist[i].cfg         =     dma_cfg0;                                                                 
        linklist[i].total_length=     dma_total_size;                                                           
        if(i==0)                                                                                         
            linklist[i].src_addr = dma_src_addr;                                                         
        else                                                                                             
        {                                                                                                
            dma_src_addr = (uint32)((int32)dma_src_addr + node_step);                                                                 
            linklist[i].src_addr = dma_src_addr;                                                         
        }                                                                                               
        linklist[i].dest_addr = dma_des_addr;                                                             
                                                                                                         
        linklist[i].sdep = dma_di;                                                               
        linklist[i].sbp  = dma_sbi;                                                              
        linklist[i].dbp  = dma_dbi;                                                              
        linklist[i].ll_ptr= (uint32) &linklist[i+1];                                              
    }                                                                                                    
    linklist[i].cfg         =     (DMA_CHx_LL_END_YES)|dma_cfg0;   //the last node of linklist                                                        
    linklist[i].total_length=     dma_total_size;  
    
    if(i==0)                                                                                             
        linklist[i].src_addr = dma_src_addr;                                                             
    else                                                                                                 
    {                                                                                                    
        dma_src_addr = (uint32)((int32)dma_src_addr + node_step);                                                                   
        linklist[i].src_addr = dma_src_addr;                                                            
    }   
    
    linklist[i].dest_addr = dma_des_addr;                                                                
                                                                                                         
    linklist[i].sdep = dma_di;                                                                   
    linklist[i].sbp  = dma_sbi;                                                                  
    linklist[i].dbp  = dma_dbi;                                                                  
    linklist[i].ll_ptr =  (uint32) &linklist[0];        

	MMU_InvalideDCACHE();
	
} 

//--------------------------------------------------------------------------------------------------------//
//  Description:   LCM_DmaWrite
//  Author:        lichd
//  Note:  
//  [in]--bus_width,     ////only support 8-bit£¬16bit
//  [in]--dir_type,      //clockwise rotate_:0,90',,180', 270';mirror support also.
//  [in]--src_addr,      //
//  [in]--des_addr,      //
//  [in]--data_width,    //if (320*320)picture_RGB565,src_width=320,src_height=240   
//  [in]--data_height,   //  
//  [in]--buf_width,     // 
//  [in]--brush_mode,    //async or sync 
//  [out]--BOOLEAN
//---------------------------------------------------------------------------------------------------------//
PUBLIC uint32 LCM_DmaWrite (
    LCD_ID_E lcd_id,                 
    LCD_BUS_WIDTH_E  bus_width,      
    uint32 src_addr,
    uint32 src_width,               
    uint32 src_height,
    uint32 buf_width,
    LCD_DIRECT_E    rotation_angle,
    LCD_BRUSH_MODE_E brush_mode    
)
{
    uint32 LCM_DATA_ADDR= g_lcm_addr[lcd_id].data_addr;  
    uint32 channel  = DMA_NLC;                        

    uint32 requestmode_sel=DMA_CHx_REQMODE_NORMAL;
			 
    //DMA_chanel_configuration:
    uint32 dma_cfg0 = 0;          
    uint32 dma_total_size = 0;         //total_size
    uint32 dma_src_addr = 0;
    uint32 dma_dst_addr = 0;

    uint32 dma_lptr = 0;
    uint32 dma_di   = 0;                //src and dst element_step
    uint32 dma_sbi  = 0;                //src block step
    uint32 dma_dbi  = 0;                //dst block step

    uint32 src_data_width=0;            //data bus width
    uint32 dst_data_width=0; 
    uint32 block_size    =0;

    uint32 src_elementstep =0;                            
    uint32 dst_elementstep =0;

    uint32 data_src_addr  = src_addr;  //picture_data parameter
    uint32 data_src_width = src_width;
    uint32 data_src_height= src_height;

    uint32 row_column_num = 0;         //for Linklist node
    int32 node_step = 0;

#if 0
    // Change the physical W/H to the W/H of data buffer
    if( (rotation_angle==LCD_DIRECT_ROT_90) || (rotation_angle==LCD_DIRECT_ROT_270) )
    {      
        uint32 temp;
        
        temp = src_width;
        src_width =src_height;
        src_height =temp;

		data_src_width = src_width;
        data_src_height= src_height;
    }
    //SCI_TRACE_LOW:"LCM_DmaWrite(): rot_angele=%d, brush_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V0_773_112_2_17_23_5_7_1040,(uint8*)"dd", rotation_angle,brush_mode);
    //SCI_TRACE_LOW:"LCM_DmaWrite(): src_addr=0x%x, src_w=%d,src_h=%d,buf_width=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V0_774_112_2_17_23_5_7_1041,(uint8*)"dddd", src_addr,src_width,src_height,buf_width);
    //SCI_TRACE_LOW:"LCM_DmaWrite(): in_timer=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V0_775_112_2_17_23_5_7_1042,(uint8*)"d", SCI_GetTickCount());
#endif
    
    switch (rotation_angle)                        
    {
        case LCD_DIRECT_NORMAL:
            {
                row_column_num = data_src_height;
                node_step      = (buf_width*2);

                //word aligned dma access
                if( ((src_addr&0x0003) == 0) && ((data_src_width&0x0001) == 0) )  
                {
                    src_elementstep= 4;  
                    dma_sbi = (DMA_CHx_BLOCKMODE_INCR8) | (DMA_CHx_BLOCKSTEP_POSITIVE) | (0);
                    dma_cfg0 = (uint32)( (DMA_CHx_SRC_DATAWIDTH_WORD) | 
	                                (DMA_CHx_DEST_DATAWIDTH_HALFWORD) |
	                                (requestmode_sel) );
                }
                else
                {
                    src_elementstep= 2;  
                    dma_sbi = (DMA_CHx_BLOCKSTEP_POSITIVE) | (0); 
                    dma_cfg0 = (uint32)( (DMA_CHx_SRC_DATAWIDTH_HALFWORD) | 
	                                    (DMA_CHx_DEST_DATAWIDTH_HALFWORD) |
	                                    (requestmode_sel) );                    
                }

                dma_src_addr =   src_addr; 
                dst_elementstep= 0;
                dma_di  = (DMA_CHx_SRC_STEP_POSITIVE) | (src_elementstep<<DMA_CHx_SRC_STEP_OFFSET) | 
                          (DMA_CHx_DEST_STEP_POSITIVE)| (dst_elementstep<<DMA_CHx_DEST_STEP_OFFSET);                                                  
                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;         
                block_size = LCM_DMA_BURST_SIZE;   
                dma_cfg0 = (uint32)( (dma_cfg0) | (block_size));
            
                dma_lptr =0;
                dma_total_size = data_src_width*2;
        	}
            break;
        case LCD_DIRECT_ROT_90: //clockwise rotate_90
            {
                row_column_num = data_src_width;
                node_step = 2;

                // address:@psrc+((height-1)DD*width+0)*2
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+0) *2; 
                src_elementstep= (buf_width*2);    
                dst_elementstep= 0;
                dma_di  = (DMA_CHx_SRC_STEP_NEGATIVE) | (src_elementstep<<DMA_CHx_SRC_STEP_OFFSET) | 
                          (DMA_CHx_DEST_STEP_POSITIVE)| (dst_elementstep<<DMA_CHx_DEST_STEP_OFFSET);  
                dma_sbi = 0;           
                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;          

                block_size = LCM_DMA_BURST_SIZE;    
                dma_cfg0 = (DMA_CHx_SRC_DATAWIDTH_HALFWORD)  |
                           (DMA_CHx_DEST_DATAWIDTH_HALFWORD) |
					       (requestmode_sel) |                           
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_src_height <<1);  
            }
            break;
        case LCD_DIRECT_ROT_180://clockwise rotate_180
            {
                row_column_num = data_src_height;
                node_step      = 0-(buf_width<<1);

                // address:@d=psrc+((height-1)DD*width+(width-1))*2
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+ (data_src_width-1)) *2; 
                src_elementstep= 2; 
                dst_elementstep=0;
                dma_di  = (DMA_CHx_SRC_STEP_NEGATIVE) | (src_elementstep<<DMA_CHx_SRC_STEP_OFFSET) | 
                          (DMA_CHx_DEST_STEP_POSITIVE)| (dst_elementstep<<DMA_CHx_DEST_STEP_OFFSET);                   
                dma_sbi = (DMA_CHx_BLOCKSTEP_POSITIVE) | (0);                                                             

                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;          

                block_size = LCM_DMA_BURST_SIZE;    
                dma_cfg0 = (DMA_CHx_SRC_DATAWIDTH_HALFWORD)  |
                           (DMA_CHx_DEST_DATAWIDTH_HALFWORD) |
					       (requestmode_sel) |  
                           (block_size);
                dma_lptr =0;
                dma_total_size= data_src_width<<1;  
            }
            break;
        case LCD_DIRECT_ROT_270://clockwise rotate_270

            {
                row_column_num = data_src_width;
                node_step      = -2;

                // address:@psrc+(0DD+(width-1))*2
                dma_src_addr = data_src_addr+ (0+ (data_src_width-1)) *2;                
                src_elementstep= buf_width*2;                                            
                dst_elementstep=0;
                dma_di  = (DMA_CHx_SRC_STEP_POSITIVE) | (src_elementstep<<DMA_CHx_SRC_STEP_OFFSET) | 
                          (DMA_CHx_DEST_STEP_POSITIVE)| (dst_elementstep<<DMA_CHx_DEST_STEP_OFFSET);    
                dma_sbi = (DMA_CHx_BLOCKSTEP_POSITIVE) | (0); //

                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;          

                block_size = LCM_DMA_BURST_SIZE;    
                
                dma_cfg0 = (DMA_CHx_SRC_DATAWIDTH_HALFWORD)  |
                           (DMA_CHx_DEST_DATAWIDTH_HALFWORD) |
					       (requestmode_sel) |                           
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_src_height <<1);    
            }            
            break;
        case LCD_DIRECT_MIR_H:  //mirror horizontal
            {
                row_column_num = data_src_height;
                node_step      = (buf_width*2);

                // address:a@b=psrc+(0DD+(width-1))*2
                dma_src_addr = data_src_addr+ (0+ (data_src_width-1)) *2;                
                src_elementstep= 2;                                                    
                dst_elementstep= 0;
                dma_di  = (DMA_CHx_SRC_STEP_NEGATIVE) | (src_elementstep<<DMA_CHx_SRC_STEP_OFFSET) | 
                          (DMA_CHx_DEST_STEP_POSITIVE)| (dst_elementstep<<DMA_CHx_DEST_STEP_OFFSET);    
                dma_sbi = (DMA_CHx_BLOCKSTEP_NEGATIVE) | (0);                                              
                
                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;          

                block_size = LCM_DMA_BURST_SIZE;    
                dma_cfg0 = (DMA_CHx_SRC_DATAWIDTH_HALFWORD)  |
                           (DMA_CHx_DEST_DATAWIDTH_HALFWORD) |
					       (requestmode_sel) |                           
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_src_width)*2;  
            }
            break;
        case LCD_DIRECT_MIR_V:  //mirror vertical
            {
                row_column_num = data_src_height;
                node_step      = 0-(buf_width<<1);

                // address:@c=psrc+((height-1)DD*width+0)*2
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+0) *2; 
                src_elementstep= 2;                                                   
                dst_elementstep=0;
                dma_di  = (DMA_CHx_SRC_STEP_POSITIVE) | (src_elementstep<<DMA_CHx_SRC_STEP_OFFSET) | 
                          (DMA_CHx_DEST_STEP_POSITIVE)| (dst_elementstep<<DMA_CHx_DEST_STEP_OFFSET); 
                dma_sbi = (DMA_CHx_BLOCKSTEP_POSITIVE) | (0);  

                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;          

                block_size = LCM_DMA_BURST_SIZE;    
                dma_cfg0 = (DMA_CHx_SRC_DATAWIDTH_HALFWORD)  |
                           (DMA_CHx_DEST_DATAWIDTH_HALFWORD) |
					       (requestmode_sel) |                           
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_src_width) *2;  
            }
            break;
        case LCD_DIRECT_MIR_HV:  //mirror horizontal&vertical
            {
                row_column_num = data_src_height;
                node_step      = 0-(buf_width<<1);

                // address:@d=psrc+((height-1)DD*width+(width-1))*2
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+ (data_src_width-1)) *2; 
                src_elementstep= 2;                                                                  
                dst_elementstep=0;
                dma_di  = (DMA_CHx_SRC_STEP_NEGATIVE) | (src_elementstep<<DMA_CHx_SRC_STEP_OFFSET) | 
                          (DMA_CHx_DEST_STEP_POSITIVE)| (dst_elementstep<<DMA_CHx_DEST_STEP_OFFSET);                   
                dma_sbi = (DMA_CHx_BLOCKSTEP_NEGATIVE) | (0);                                                             

                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;          

                block_size = LCM_DMA_BURST_SIZE;   
                dma_cfg0 = (DMA_CHx_SRC_DATAWIDTH_HALFWORD)  |
                           (DMA_CHx_DEST_DATAWIDTH_HALFWORD) |
					       (requestmode_sel) |                           
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_src_width) *2;  
            }
            break;
        default:
            //SCI_TRACE_LOW:"LCM: w = %d, h = %d, rotate=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V0_968_112_2_17_23_5_7_1043,(uint8*)"ddd", data_src_width, data_src_height, rotation_angle);
            break;
            
    }      
    
    // Construct the linklist of dma
    _DMA_LINKLIST_CONSTRUCT(dma_channelcfg_llist, row_column_num, node_step,
                            dma_cfg0,  dma_total_size,  dma_src_addr,  dma_dst_addr,
                            0,         dma_di,          dma_sbi,       dma_dbi);                  
   
    {
        DMA_CHNCFG_T    dma_ctl_lcm = {0};
		dma_ctl_lcm.type           = DMA_CHN_TYPE_HARD;
	    dma_ctl_lcm.priority       = DMA_CHN_PRIORITY_3;
	    dma_ctl_lcm.workmode       = DMA_CHN_WORKMODE_LINKLIST;	

		dma_ctl_lcm.ll_next = (uint32)(&dma_channelcfg_llist[0]);

		dma_ctl_lcm.src_datawidth  = src_data_width;
		dma_ctl_lcm.dest_datawidth = dst_data_width;
		dma_ctl_lcm.src_address  = dma_src_addr;
		dma_ctl_lcm.dest_address = dma_dst_addr;

		dma_ctl_lcm.requestmode  = DMA_CHN_REQMODE_NORMAL;		
		dma_ctl_lcm.block_length = block_size;           
		dma_ctl_lcm.total_length = dma_total_size;

		dma_ctl_lcm.src_step     = src_elementstep;				
		dma_ctl_lcm.dest_step    = dst_elementstep;

		dma_ctl_lcm.src_blockstep = dma_sbi;		
		dma_ctl_lcm.dest_blockstep = dma_dbi;
		
        DMA_HAL_ChnConfig (DMA_NLC, &dma_ctl_lcm);
	}

    //synchronize: polling mode
    if(LCD_BRUSH_MODE_SYNC == brush_mode)      
	{   
        // Diaable DMA INTERRUPT    
        DMA_HAL_ChnIntConfig (channel, DMA_CHN_INT_TYPE_NONE, SCI_NULL);
		            
	    DMA_HAL_ChnEnable (channel,SCI_TRUE);	
        
	    _LCM_WaitDMA_AllLinkDone (channel, 10);         
	                      
	    DMA_HAL_ChnEnable( channel, SCI_FALSE);
        //SCI_TRACE_LOW("LCM_DmaWrite(): out_timer=%d", SCI_GetTickCount());
    }
    //async: interrupt mode with callback
	else if(LCD_BRUSH_MODE_ASYNC == brush_mode)     
	{
        // Register DMA callback funtion and enable DMALlist mode.
        DMA_HAL_ChnIntConfig (channel, DMA_CHN_INT_TYPE_LISTDONE, LCM_DMALinkCallback_handler);       
		                    
        DMA_HAL_ChnEnable (channel,SCI_TRUE);                            
	}
    
    return SCI_SUCCESS;
}

#ifdef   __cplusplus
}
#endif
