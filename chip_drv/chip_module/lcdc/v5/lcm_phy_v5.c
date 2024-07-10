/******************************************************************************
** File Name:      lcm_phy_v5.c                                                *
** DATE:           23/09/2005                                                  *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.           *
** Description:    this file defines LCM interfaces,                           *
**                   these interfaces is visible for LCM's user                *
*******************************************************************************

*******************************************************************************
**                         Edit History                                        *
**-----------------------------------------------------------------------------*
** DATE                    DESCRIPTION                                         *
** 23/09/2005                Create.                                           *
*******************************************************************************/

/**----------------------------------------------------------------------------*
 **                         Dependencies                                       *
 **----------------------------------------------------------------------------*/

#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "ref_outport.h"

#include "dal_lcd.h"
#include "lcm_drvapi.h"
#include "lcd_cfg.h"
#include "lcd_hal_internal.h"

#include "dma_drv_internal.h"
#include "../../dma/dma_phy.h"

#if 0//!defined(CHIP_VER_6531) && !defined(CHIP_VER_6531EFM) && !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIX8910MPW) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A)
#include "..\..\dma\v5\dma_reg_v5.h"
#endif

#include "lcm_reg_v5.h"

/**----------------------------------------------------------------------------*
 **                        Compiler Flag                                       *
 **----------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**-----------------------------------------------------------------------------*
 **                        Macro Define                                         *
 **-----------------------------------------------------------------------------*/
#define LCM_DMA_TIMEOUT     500 //ms

#define _Delayms( ticks) \
                         SCI_Sleep( ticks )

#define LCM_DMA_BURST_SIZE  32  //Refer to LCM FIFO DEPTH

#define LCM_DMA_CONNECT(chn) CHIP_REG_SET(LCM_REQ_ID_ADDR, chn)

/**-----------------------------------------------------------------------------*
 **                        Local Define                                         *
 **-----------------------------------------------------------------------------*/
LOCAL DEVICE_HANDLE s_dev_lcm = SCI_NULL; 
static uint32 channel;

/*******************************************************************************/
// Description: set lcm datawidth
// Global resource dependence:
// Author: lichd 
// Note:      
/*******************************************************************************/
#define  BUS_WIDTH_2X8BIT  0  /*Send 16bit data as two 8bit in LCMD0~LCMD7*/
#define  BUS_WIDTH_16BIT   1  /*Send 16bit data in LCMD0~LCMD15 */

PUBLIC void _lcm_set_buswidth ( uint32 bus_width, uint32 lcd_id)
{    
    LCM_CS_CFG0_U *plcm_cs0_cfg0     = (LCM_CS_CFG0_U *) LCM_CS0_CFG0;  //cs0
    LCM_CS_CFG0_U *plcm_cs1_cfg0     = (LCM_CS_CFG0_U *) LCM_CS1_CFG0;  //cs1
    LCM_CS_CFG0_U *plcm_cs2_cfg0     = (LCM_CS_CFG0_U *) LCM_CS2_CFG0;  //cs2
    LCM_CS_CFG0_U *plcm_cs3_cfg0     = (LCM_CS_CFG0_U *) LCM_CS3_CFG0;  //cs3

    LCM_CS_CFG0_U     lcm_cs_cfg0;

    LCD_SPEC_INFO_T_PTR _lcm_spec_info_ptr = LCM_GetCurrentSpecinfo( lcd_id );
    
    SCI_ASSERT (bus_width <= BUS_WIDTH_16BIT); /*assert to do*/

    if( bus_width == BUS_WIDTH_2X8BIT )
        lcm_cs_cfg0.dwValue = 0x28;    
    else 
        lcm_cs_cfg0.dwValue = 0x01;    

    // wait for if LCM Idle
    while ( REG32(LCM_STS0) & BIT_1 ){}
    
    /* change data with only */
    switch (_lcm_spec_info_ptr->cs_pin)
    {
        case 0:
            plcm_cs0_cfg0->dwValue = lcm_cs_cfg0.dwValue;
            break;
        case 1:
            plcm_cs1_cfg0->dwValue = lcm_cs_cfg0.dwValue;
            break;
        case 2:
            plcm_cs2_cfg0->dwValue = lcm_cs_cfg0.dwValue;
            break;
        case 3:
            plcm_cs3_cfg0->dwValue = lcm_cs_cfg0.dwValue;
            break;
        default:
            //SCI_TRACE_LOW:"_lcm_set_buswidth: the cs_pin value of lcd_spec_ptr should not large than 4.."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V5_98_112_2_17_23_5_13_1052,(uint8*)"");
            SCI_ASSERT (0); /*assert to do*/
            break;
    }
}

/*******************************************************************************/
// Description:    initialize LCM
// Global resource dependence
// Author: Jianping.wang
// Note:
/*******************************************************************************/
#define RF_T_FIRST_R        (0x5)    //temp
#define RF_T_FIRST_W        (0x5)
#define RF_T_WE_STP_W       (0x0)
#define RF_T_WE_WIDTH_W     (0x1)
PUBLIC void _lcm_init (uint32 lcd_id)
{
    // enable LCM
    SCI_ASSERT (DEVICE_Find ("LCM", &s_dev_lcm) == CLOCK_SUCCESS); /*assert verified*/    
    DEVICE_Open(s_dev_lcm);
    
    *(volatile uint32 *)LCM_CFG0 = 0x0;//0x55550000;
     
    *(volatile uint32 *)LCM_CS0_CFG0 = 0x00000001;  //suppose 16bit LCD 

    *(volatile uint32 *)LCM_CS0_CFG1 = (RF_T_WE_WIDTH_W<<8) |//Need to check
                                       (RF_T_WE_STP_W<<4)   |
                                       (RF_T_FIRST_W<<21)   |
                                       (RF_T_FIRST_R<<16);
 
}

/*******************************************************************************/
// Description:    LCM reset LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
//     reset sequence:1-0-delay-1
//       delay_ms - during reset,need to add wait time
/*******************************************************************************/
PUBLIC void _lcm_lcm_resetlcd (uint32 delay_ms,uint8 rst_level)
{
    if(rst_level == 1)  
    {
        //do Panel reset
        *(volatile uint32 *)APB_MISC_CTL1 &= ~LCM_RSTN;     
        SCI_Sleep(delay_ms);

        *(volatile uint32 *)APB_MISC_CTL1 |= LCM_RSTN;  
        SCI_Sleep(delay_ms);

        *(volatile uint32 *)APB_MISC_CTL1 &= ~LCM_RSTN;     
        SCI_Sleep(delay_ms);
    }
    else
    {
        //do Panel reset
        *(volatile uint32 *)APB_MISC_CTL1 |= LCM_RSTN;      
        SCI_Sleep(delay_ms);

        *(volatile uint32 *)APB_MISC_CTL1 &= ~LCM_RSTN; 
        SCI_Sleep(delay_ms);

        *(volatile uint32 *)APB_MISC_CTL1 |= LCM_RSTN;      
        SCI_Sleep(delay_ms);
    }
}

/*******************************************************************************/
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
/*******************************************************************************/
PUBLIC ERR_LCD_E _lcm_lcmc_sendcmd (uint32 cmd,uint32 lcd_id)
{
    _lcm_set_buswidth(BUS_WIDTH_16BIT, lcd_id);
    // wait for if LCM Idle
    while ( REG32(LCM_STS0) & BIT_1 ){}
    
    * (volatile uint16 *) (g_lcm_addr[lcd_id].cmd_addr)  = cmd;

    return ERR_LCD_NONE;    
}

/*******************************************************************************/
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
/*******************************************************************************/
PUBLIC ERR_LCD_E _lcm_lcmc_senddata (uint32 data,uint32 lcd_id)
{
    _lcm_set_buswidth(BUS_WIDTH_16BIT, lcd_id);
    // wait for if LCM Idle
    while ( REG32(LCM_STS0) & BIT_1 ){}
    * (volatile uint16 *) (g_lcm_addr[lcd_id].data_addr)  = data;
    
    return ERR_LCD_NONE;    
}

/*
PUBLIC ERR_LCD_E LCM_SendCmdData (uint32 cmd,uint32 data,uint32 lcd_id)
{
    LCM_SendData(cmd,  lcd_id);
    LCM_SendData(data, lcd_id);

    return ERR_LCD_NONE;
}


PUBLIC BOOLEAN LCM_RegisterCallBack (uint32 int_num, LCMCallBackFunc func)
{
    SCI_ASSERT (PNULL != func);

    s_lcm_callbackfunc = func;

    return SCI_TRUE;

}//end of LCM_RegisterInterrupt
*/
/*******************************************************************************/
// Description:    NLC DMA Callback
// Global resource dependence:
// Author: younger
/*******************************************************************************/

LOCAL void _LCM_DMACallback (uint32 param)
{
    if (s_lcm_callbackfunc)
    {
        s_lcm_callbackfunc();
    }
    LCM_DMA_CONNECT(0);
    DMA_HAL_FreeChn(channel);
}

LOCAL void _LCM_WaitDMADone (uint32 chn, uint32 time)
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

        //while (!DMA_CHNDONE_STS (chn))
        while (!DMA_PHY_PollingTransDone (chn))
        {
            cur_tick = SCI_GetTickCount();

            if (cur_tick < pre_tick)
            {
                continue;
            }

            if (cur_tick - pre_tick >= LCM_DMA_TIMEOUT)
            {
                //SCI_ASSERT(0);
                DMA_PHY_ChnEnable (chn,SCI_FALSE);
                //SCI_TRACE_LOW:"_LCM_WaitDMADone: time out !!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V5_262_112_2_17_23_5_13_1053,(uint8*)"");
                break;
            }
        }
    }
}

PUBLIC void _lcm_lcmc_setaddr (uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 reg_base = LCM_BASE_ADDR;
    uint32 cd_offset = 0;
    uint32 cs_offset = 0;
    SCI_ASSERT ( (lcd_cs<=3) && (lcd_cd<=1)); /*assert to do*/

    // Note:CFG0@0X20800000[1:0] use default value. so: CS[2:0] mapping to {HADDR[30],HADDR[27:26]}
    cs_offset = lcd_cs * (1<<26);
    // set cd address . only use LCMA0,bound to A17
    cd_offset = 0x00020000;  

    g_lcm_addr[lcd_id].cmd_addr  = reg_base + cs_offset;
    g_lcm_addr[lcd_id].data_addr = reg_base + cs_offset + cd_offset;
}

/*******************************************************************************/
// Description:change pulse width
// Global resource dependence:
// Author: Jianping.wang
// Note:
//         input:
//            ahb_clk - AHB bus clock
//         return:
//          ERR_LCD_NONE,if success
//            other,if fail
/*******************************************************************************/
PUBLIC ERR_LCD_E _lcm_lcmc_setparam (LCD_SPEC_INFO_T_PTR lcd_spec_ptr, uint32 ahb_clk, uint16 lcd_id)
{
    LCM_CS_CFG0_U *plcm_cs0_cfg0     = (LCM_CS_CFG0_U *) LCM_CS0_CFG0;  //cs0
    LCM_CS_CFG1_U *plcm_cs0_cfg1     = (LCM_CS_CFG1_U *) LCM_CS0_CFG1;
    LCM_CS_CFG0_U *plcm_cs1_cfg0     = (LCM_CS_CFG0_U *) LCM_CS1_CFG0;  //cs1
    LCM_CS_CFG1_U *plcm_cs1_cfg1     = (LCM_CS_CFG1_U *) LCM_CS1_CFG1;
    LCM_CS_CFG0_U *plcm_cs2_cfg0     = (LCM_CS_CFG0_U *) LCM_CS2_CFG0;  //cs2
    LCM_CS_CFG1_U *plcm_cs2_cfg1     = (LCM_CS_CFG1_U *) LCM_CS2_CFG1;
    LCM_CS_CFG0_U *plcm_cs3_cfg0     = (LCM_CS_CFG0_U *) LCM_CS3_CFG0;  //cs3
    LCM_CS_CFG1_U *plcm_cs3_cfg1     = (LCM_CS_CFG1_U *) LCM_CS3_CFG1;

    LCM_CS_CFG0_U     lcm_cs_cfg0 = {0};
    LCM_CS_CFG1_U     lcm_cs_cfg1 = {0};
    
    uint32 we_stp = 0;
    uint32 we_l = 0;
    uint32 we_h = 0;

    /* multiple 8/10, so lcm_cycle = 1.25*min_cycle(spec). */
    //temp_value = (100000000 >> 17) / (ahb_clk >> 20) ;

    if (ahb_clk > 1000000)
    {
        ahb_clk = ahb_clk/1000000;
    }

    /* Set the ww/rr/wr/rw turn around to improve the effiency */
    REG32( LCM_CFG0 )= 0x11110000;

    /* Note:init lcm with datawidth=16, then set different buswidth in _dmawrite() */
    if(lcd_spec_ptr->spec_ptr->bus_width == WIDTH_8) 
        lcm_cs_cfg0.dwValue = 0x28;     
    else if(lcd_spec_ptr->spec_ptr->bus_width == WIDTH_16)
        lcm_cs_cfg0.dwValue = 0x01; 
    else 
        SCI_ASSERT(0); /*assert to do*/
    
    lcm_cs_cfg1.mBits.rf_t_oe_stp_r   =MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rcss * ahb_clk -1) /1000) + 1), 0x6);
    lcm_cs_cfg1.mBits.rf_t_first_r    =MIN(
                                       MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rcss * ahb_clk -1) /1000) + 1), 0x6) +
                                       MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rhpw * ahb_clk -1) /1000) + 1), 0xe) +
                                       MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rlpw * ahb_clk -1) /1000) + 1), 0xe) ,
                                       0x1E);

    lcm_cs_cfg1.mBits.rf_t_data_oe_hld_w   = 1;
    we_stp = MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.wcss * ahb_clk  -1)/1000) + 1), 0x6);
    we_l = MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.wlpw * ahb_clk -1 )/ 1000) + 1), 0xe);
    we_h = MIN ( ( ( (lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.whpw * ahb_clk -1 )/1000) + 1), 0xe);
    lcm_cs_cfg1.mBits.rf_t_we_stp_w   = we_stp;
    lcm_cs_cfg1.mBits.rf_t_we_wdt_w   = we_l;
    if ( we_h - 1 > we_stp)
    {
        lcm_cs_cfg1.mBits.rf_t_first_w = MIN (we_l + we_h - 1, 0x1E);
    }
    else
    {
        lcm_cs_cfg1.mBits.rf_t_first_w = MIN (we_l + we_stp + 1, 0x1E);
    }
    
    // wait for until AHB FIFO empty(1)[BIT_2],0-idle, 1-accessing
    while ( REG32(LCM_STS0) & BIT_1 ){}  

    switch (lcd_spec_ptr->cs_pin)
    {
        case 0:
            plcm_cs0_cfg0->dwValue = lcm_cs_cfg0.dwValue;
            plcm_cs0_cfg1->dwValue = lcm_cs_cfg1.dwValue;
            break;
        case 1:
            plcm_cs1_cfg0->dwValue = lcm_cs_cfg0.dwValue;
            plcm_cs1_cfg1->dwValue = lcm_cs_cfg1.dwValue;
            break;
        case 2:
            plcm_cs2_cfg0->dwValue = lcm_cs_cfg0.dwValue;
            plcm_cs2_cfg1->dwValue = lcm_cs_cfg1.dwValue;
            break;
        case 3:
            plcm_cs3_cfg0->dwValue = lcm_cs_cfg0.dwValue;
            plcm_cs3_cfg1->dwValue = lcm_cs_cfg1.dwValue;
            break;
        default:
            //SCI_TRACE_LOW:"LCM_SetParam: the cs_pin value of lcd_spec_ptr should not large than 4.."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V5_375_112_2_17_23_5_13_1054,(uint8*)"");
            SCI_ASSERT (0); /*assert to do*/
            break;
    }

    return ERR_LCD_NONE;    
}

/*******************************************************************************/
//  Description:   Read LCM
//  Global resource dependence:
//  Author: Younger.Yang
//  Note: You may read LCD ID before lcd init,otherwise
//  the read data may be incorrect.
/*******************************************************************************/

PUBLIC uint32 _lcm_lcmc_readid (uint32 lcd_id, uint32 cd_level)
{
    uint32 read_data;

    _lcm_set_buswidth(BUS_WIDTH_16BIT, lcd_id);
    // wait for if LCM Idle
    while ( REG32(LCM_STS0) & BIT_1 ){}
    
    if (DATA_LEVEL == cd_level)
    {
        read_data = * (volatile uint16 *) (g_lcm_addr[lcd_id].data_addr) ;
    }
    else
    {
        read_data = * (volatile uint16 *) (g_lcm_addr[lcd_id].cmd_addr) ;
    }
    
    return read_data; 
}

/*******************************************************************************/
//  Description:   _lcm_parallel_dmawrite
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
/*******************************************************************************/
PUBLIC uint32 _lcm_parallel_dmawrite (              
                                        LCD_BUS_WIDTH_E  bus_width,      
                                        uint32 src_addr,
                                        uint32 dst_addr,    
                                        uint32 data_width,               
                                        uint32 data_height,
                                        uint32 buf_width,
                                        LCD_DIRECT_E    dir_type,
                                        LCD_BRUSH_MODE_E brush_mode    
)
{   
    DMA_CHNCFG_T chn_cfg={0}; 
    
    uint32 LCM_DATA_ADDR = dst_addr;
    uint32 FIFO_SIZE_LIMIT =64;    
    
    uint32 dma_src_addr = 0;
    uint32 dma_dst_addr = 0;
    uint32 src_data_width=0;
    uint32 dst_data_width=0;
    
    int32 dma_src_trsfstep   = 0;
    int32 dma_dst_trsfstep   = 0;    
    int32 dma_src_fragstep   = 0;
    int32 dma_dst_fragstep   = 0; 
    int32 dma_src_blockstep  = 0;       
    int32 dma_dst_blockstep  = 0;       

    uint32 frag_length  = 4;
    uint32 block_length = 16;    
    uint32 total_length = 64;

    uint32 data_src_addr  = src_addr;   //picture_data parameter
    uint32 data_src_width = data_width;
    uint32 data_src_height= data_height;
    
    if (WIDTH_8==bus_width)                      //Define DMA_DEST_DATABUS_WIDTH according to lcd bus_width
    {
        *(volatile uint32 *)(0x20800010)  = 0x28;//set buswidth-8bits, num=1
    
        src_data_width = DMA_DATAWIDTH_HALFWORD; //set DMA SRC_DATA_WIDTH and DST_DATA_WIDTH
        dst_data_width = DMA_DATAWIDTH_HALFWORD;
    }
    else if (WIDTH_16==bus_width)                 //main_use type.
    {
        *(volatile uint32 *)(0x20800010)  = 0x01;  //set buswidth-16bits   
        src_data_width = DMA_DATAWIDTH_HALFWORD;
        dst_data_width = DMA_DATAWIDTH_HALFWORD;
    }
    else
    {
        return 0;
    }

    //SCI_TRACE_LOW:"LCM:w = %d,h = %d,bufw=%d rotate=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V5_471_112_2_17_23_5_13_1055,(uint8*)"dddd", data_src_width, data_src_height,buf_width, dir_type);
    
    switch ( dir_type )                        //PROCESS:picture_rotation by DMA
    {
        case LCD_DIRECT_NORMAL:
            {
                dma_src_addr = data_src_addr;      //@psrc+(0DD)*2
                dma_dst_addr = LCM_DATA_ADDR;
                
                dma_src_trsfstep  = 2;                // 2
                dma_src_fragstep  = 0;                // 2
                dma_src_blockstep = (buf_width - data_src_width) *2;  
                
                dma_dst_trsfstep = 0;
                dma_dst_trsfstep = 0;
                dma_dst_blockstep= 0;

                frag_length  = FIFO_SIZE_LIMIT; 
                block_length = data_src_width<<1;//block_size: width*2
                total_length = (data_src_height*data_src_width) *2;  //total_size : height*width*2
                
            }
            break;
        case LCD_DIRECT_ROT_90: //clockwise rotate_90
            {   //c@psrc+((height-1)DD*width+0)*2
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+0) *2; 
                dma_dst_addr = LCM_DATA_ADDR;
                
                dma_src_trsfstep  = 0-buf_width*2;    //(-width)*2
                dma_src_fragstep  = 0;    //(-width)*2
                dma_src_blockstep = ((data_src_height)*buf_width+1) *2;
                //dma_sbi = (MIN(176,data_src_height)-1)*buf_width+1)*2 );   //src block_step: for sc6610
                
                dma_dst_trsfstep = 0;
                dma_dst_trsfstep = 0;
                dma_dst_blockstep= 0;

                frag_length  = FIFO_SIZE_LIMIT; 
                block_length = data_src_height<<1;//block_size: height*2
                total_length = (data_src_height*data_src_width) *2;  //total_size : height*width*2              
            }
            break;
        case LCD_DIRECT_ROT_180://clockwise rotate_180
            {
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+ (data_src_width-1)) *2; //@d=psrc+((height-1)DD*width+(width-1))*2
                dma_dst_addr = LCM_DATA_ADDR;
                
                dma_src_trsfstep  = -2;
                dma_src_fragstep  = 0;
                dma_src_blockstep = 0-(buf_width - data_src_width) *2;//
                
                dma_dst_trsfstep = 0;
                dma_dst_trsfstep = 0;
                dma_dst_blockstep= 0;

                frag_length  = FIFO_SIZE_LIMIT; 
                block_length = data_src_width<<1;    //block_size: width*2
                total_length = (data_src_height*data_src_width) *2;  //total_size : height*width*2                

            }
            break;
        case LCD_DIRECT_ROT_270://clockwise rotate_270
            {
                dma_src_addr = data_src_addr+ (0+ (data_src_width-1)) *2; //@b=psrc+(0DD+(width-1))*2
                dma_dst_addr = LCM_DATA_ADDR;
                
                dma_src_trsfstep  = buf_width*2;    //(width)*2
                dma_src_fragstep  = 0;    //(width)*2
                dma_src_blockstep = 0-(data_src_height*buf_width+1)*2;

                dma_dst_trsfstep = 0;
                dma_dst_trsfstep = 0;
                dma_dst_blockstep= 0;

                frag_length  = FIFO_SIZE_LIMIT; 
                block_length = data_src_height<<1;    //block_size: height*2
                total_length = (data_src_height*data_src_width) *2;  //total_size : height*width*2                   
            }
            break;
        case LCD_DIRECT_MIR_H:  //mirror horizontal
            {
                dma_src_addr = data_src_addr+ (0+ (data_src_width-1)) *2; //@b=psrc+(0DD+(width-1))*2
                dma_dst_addr = LCM_DATA_ADDR;
                
                dma_src_trsfstep  = -2;    
                dma_src_fragstep  = 0;    
                dma_src_blockstep = (buf_width+data_src_width)*2; 

                dma_dst_trsfstep = 0;
                dma_dst_trsfstep = 0;
                dma_dst_blockstep= 0;

                frag_length  = FIFO_SIZE_LIMIT; 
                block_length = data_src_width<<1;    //block_size: width*2
                total_length = (data_src_height*data_src_width) *2;  //total_size : height*width*2                 
            }
            break;
        case LCD_DIRECT_MIR_V:  //mirror vertical
            {
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+0) *2; //@c=psrc+((height-1)DD*width+0)*2
                dma_dst_addr = LCM_DATA_ADDR;
                
                dma_src_trsfstep  = 2;    
                dma_src_fragstep  = 0;    
                dma_src_blockstep = 0-(data_src_width+buf_width)*2; //src block_step: (data_width+buf_width)*(-2)

                dma_dst_trsfstep = 0;
                dma_dst_trsfstep = 0;
                dma_dst_blockstep= 0;

                frag_length  = FIFO_SIZE_LIMIT; 
                block_length = data_src_width<<1;    //block_size: width*2
                total_length = (data_src_height*data_src_width) *2;  //total_size : height*width*2                 
            }
            break;
        case LCD_DIRECT_MIR_HV:  //mirror horizontal&vertical
            {
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+ (data_src_width-1)) *2; //@d=psrc+((height-1)DD*width+(width-1))*2
                dma_dst_addr = LCM_DATA_ADDR;
                
                dma_src_trsfstep  = -2;    
                dma_src_fragstep  = 0;    
                dma_src_blockstep = 0-(buf_width-data_src_width)*2;//src block_step: (-)(buf_width-data_width)*2

                dma_dst_trsfstep = 0;
                dma_dst_trsfstep = 0;
                dma_dst_blockstep= 0;

                frag_length  = FIFO_SIZE_LIMIT; 
                block_length = data_src_width<<1;    //block_size: width*2
                total_length = (data_src_height*data_src_width) *2;  //total_size : height*width*2                  
            }
            break;
        default:
            //SCI_TRACE_LOW("LCM: w = %d, h = %d, rotate=%d", data_src_width, data_src_height, rotation_angle);
            break;
            //
    }

    // DMA channel Config
    chn_cfg.src_address = dma_src_addr;
    chn_cfg.dest_address= dma_dst_addr;
    
    chn_cfg.src_step  = dma_src_trsfstep;
    chn_cfg.src_fragstep  = dma_src_fragstep;
    chn_cfg.src_blockstep = dma_src_blockstep;
    chn_cfg.src_datawidth = src_data_width;

    chn_cfg.dest_step     = dma_dst_trsfstep;
    chn_cfg.dest_fragstep = dma_dst_fragstep;
    chn_cfg.dest_blockstep= dma_dst_blockstep;
    chn_cfg.dest_datawidth= dst_data_width;

    chn_cfg.frag_length   = frag_length;
    chn_cfg.block_length  = block_length;
    chn_cfg.total_length  = total_length;

#ifdef DMA_FMARK_ENABLE    
    chn_cfg.requestmode = DMA_CHN_REQMODE_BLOCK;
#else
    chn_cfg.requestmode = DMA_CHN_REQMODE_TRASACTION;
#endif

    //Alloc a idle dma channel with your uid     
    channel = DMA_HAL_AllocChn(DMA_CHN_TRSF_TYPE_FULL);
    DMA_HAL_ChnConfig (channel, &chn_cfg);
    LCM_DMA_CONNECT(channel);   

    DMA_HAL_ChnEnable(channel, SCI_TRUE);    
    //SCI_TRACE_LOW:"LCM:DMA_channel=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V5_640_112_2_17_23_5_14_1056,(uint8*)"d", channel);
    
    if( 0 == brush_mode )   //Sync model
    {
        // Disable DMA INTERRUPT    
        DMA_HAL_ChnIntConfig (channel, DMA_CHN_INT_TYPE_NONE, SCI_NULL);    
        // Disable DMA Transfer interrupt
        //DMA_PHY_DisAllInt (channel);
                
        DMA_HAL_ChnEnable(channel, SCI_TRUE);
#ifndef DMA_FMARK_ENABLE         
        DMA_PHY_ChnRequest( channel );
#endif        
        _LCM_WaitDMADone (channel, 30);        //LCM_DMA_BLOCK_TIMEOUT=5ms

        //disable dma channel
        DMA_HAL_ChnEnable(channel, SCI_FALSE); 
        LCM_DMA_CONNECT(0);
        DMA_HAL_FreeChn(channel);
        //SCI_TRACE_LOW:"LCM_DmaWrite(): out_timer=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCM_PHY_V5_659_112_2_17_23_5_14_1057,(uint8*)"d", SCI_GetTickCount());
    }

    if (1 == brush_mode)   //Async model
    {
        // Register DMA callback funtion and enable transaction mode.
        DMA_HAL_ChnIntConfig (channel, DMA_CHN_INT_TYPE_TRANSDONE, _LCM_DMACallback);

        // Enable DMA
        DMA_PHY_ChnEnable (channel,SCI_TRUE);
        DMA_PHY_ChnRequest( channel );
    }   

    return SCI_TRUE;  //avoid pclint error
}

// Configure the object of s_if_lcm
const PUBLIC LCM_CONTROLLER_T  s_if_lcm =
{
    _lcm_init,
    _lcm_lcm_resetlcd,
        
    _lcm_lcmc_setparam,
    _lcm_lcmc_setaddr,
                        
    _lcm_lcmc_senddata,
    _lcm_lcmc_sendcmd,
    _lcm_parallel_dmawrite,
    _lcm_lcmc_readid,
};
    
#ifdef   __cplusplus
}
#endif
