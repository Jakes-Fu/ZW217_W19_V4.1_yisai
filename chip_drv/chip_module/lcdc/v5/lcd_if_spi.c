/***************************************************************************************
** File Name:      lcd_if_spi.c                                                          *
** DATE:           01/19/2011                                                           *
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.                    *
** Description:    this file defines LCM interfaces,                                   *
**                 these interfaces is visible for LCM's user                          *
****************************************************************************************

****************************************************************************************
**                         Edit History                                                  *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                                                  *
** 01/19/2010                Create.                                                       *
****************************************************************************************/

/**------------------------------------------------------------------------------------*
 **                         Dependencies                                                  *
 **------------------------------------------------------------------------------------*/

#include "os_api.h"
#include "sci_types.h"
#include "sc_reg.h"
#include "ref_outport.h"
#include "ref_param.h"
#include "fiq_drv.h"

#include "dal_lcd.h"
#include "lcm_drvapi.h"
#include "lcd_cfg.h"
#include "lcd_hal_internal.h"

#include "spi_drv.h"
#include "spi_drvapi.h"
#include "deep_sleep.h"

#include "dma_drv_internal.h"
#include "../../dma/dma_phy.h"

#if 0 //!defined(CHIP_VER_6531) && !defined(CHIP_VER_6531EFM) && !defined(CHIP_VER_UIX8910) && !defined(CHIP_VER_UIX8910MPW) && !defined(CHIP_VER_UIS8910C) && !defined(CHIP_VER_UIS8910A)
#include "..\..\dma\v5\dma_reg_v5.h"
#endif

/**********************************************************************************************************/
// **                        Compiler Flag                                                   **
/**********************************************************************************************************/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**********************************************************************************************************/
// **                               Macro Define                                              **
/**********************************************************************************************************/
#define SPIDMA_BURST_SIZE  32

#define _Delayms( ticks) \
    SCI_Sleep( ticks )

#define LCM_DMA_DELAY_DEF       0x10 // H clock
#define LCM_RESET_PULSE_WIDHT   50   // Ms

#define SPI0_PORT_ID  0
#define SPI1_PORT_ID  1

/**********************************************************************************************************/
// **                               Data Prototypt                              **
/**********************************************************************************************************/

// SPI control register filed definitions
typedef struct
{
    VOLATILE uint32 data;               // Transmit word or Receive word
    VOLATILE uint32 clkd;               // clock dividor register
    VOLATILE uint32 ctl0;               // control register
    VOLATILE uint32 ctl1;               // Receive Data full threshold/Receive Data full threshold
    VOLATILE uint32 ctl2;               // 2-wire mode reigster
    VOLATILE uint32 ctl3;               // transmit data interval
    VOLATILE uint32 ctl4;               // transmit data interval
    VOLATILE uint32 ctl5;               // transmit data interval
    VOLATILE uint32 ien;                // interrutp enable register
    VOLATILE uint32 iclr;               // interrupt clear register
    VOLATILE uint32 iraw;               // interrupt clear register
    VOLATILE uint32 ists;               // interrupt clear register
    VOLATILE uint32 sts1;               // fifo cnt register, bit[5:0] for RX and [13:8] for TX
    VOLATILE uint32 sts2;               // masked interrupt status register
    VOLATILE uint32 dsp_wait;           // Used for DSP control
    VOLATILE uint32 sts3;               // tx_empty_threshold and tx_full_threshold
    VOLATILE uint32 ctl6;
    VOLATILE uint32 sts4;
    VOLATILE uint32 fifo_rst;
    VOLATILE uint32 ctl7;               // SPI_RX_HLD_EN : SPI_TX_HLD_EN : SPI_MODE
    VOLATILE uint32 sts5;               // CSN_IN_ERR_SYNC2
    VOLATILE uint32 ctl8;               // SPI_CD_BIT : SPI_TX_DUMY_LEN : SPI_TX_DATA_LEN_H
    VOLATILE uint32 ctl9;               // SPI_TX_DATA_LEN_L
    VOLATILE uint32 ctl10;              // SPI_RX_DATA_LEN_H : SPI_RX_DUMY_LEN
    VOLATILE uint32 ctl11;              // SPI_RX_DATA_LEN_L
    VOLATILE uint32 ctl12;              // SW_TX_REQ : SW_RX_REQ
} SPI_CTL_REG_T;

//- the struct of DMA channel configuration
typedef struct _dma_channel_config
{
    int32 cfg0;
    uint32 cfg1;
    uint32 src_addr;
    uint32 dst_addr;

    uint32 lptr;          //note: must be aligned at 32byte boundary
    int32 element_step;
    int32 src_blockstep;
    int32 dst_blockstep;
} DMA_CHANNEL_CONFIG_T;

__align (32) DMA_CHANNEL_CONFIG_T dma_channelcfg_llist[MAX (LCD_WIDTH, LCD_HEIGHT) ] = {0} ; /*lint !e506*/


// **********************************************************************************************************/
// **                               Function  Define                            **
// **********************************************************************************************************/
PUBLIC void SPI_SetCsLow (uint32 spi_sel_csx , BOOLEAN is_low);
PUBLIC void  SPI_SetDatawidth (uint32 datawidth);
PUBLIC void SPI_SetCd (uint32 cd);
PUBLIC void SPI_WriteData (uint32 data, uint32 data_len, uint32 dummy_bitlen);
#if 1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
PUBLIC void  SPI_SelectData2Pin(void);
PUBLIC void  SPI_SelectData2RGB565(void);
PUBLIC void  SPI_SelectDataRGB565(void);
#endif //CHIP_VER_6531EFM
/**********************************************************************************************************/
// Description: Set #C/D signal as DATA_LEVEL or CMD_LEVEL
// Global resource dependence:
// Author: lichd Dec.10.
// Note:   0:CMD_LEVEL   1:DATA_LEVEL
/**********************************************************************************************************/
#if 1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
#define SPI_USED_BASE                       0x69000000
#else //CHIP_VER_6531EFM
#define SPI_USED_BASE                       0x68000000
#endif //CHIP_VER_6531EFM

#define SPI_SEL_CS_SHIFT 8
#define SPI_SEL_CS_MASK (0x0F<<SPI_SEL_CS_SHIFT)

LOCAL void _lcm_setcd (uint32 _spi_logicid, uint32 cd_level)
#if 0
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *) (SPI_USED_BASE);

    //0-command;1-data
    if (cd_level == 0)
    {
        spi_ctr_ptr->ctl0 &= ~ (uint32) ( (0x2) <<SPI_SEL_CS_SHIFT);
        //spi_ctr_ptr->ctl8 &= ~ (SPI_CD_MASK);
    }
    else
    {
        //spi_ctr_ptr->ctl8 |= (SPI_CD_MASK);
        spi_ctr_ptr->ctl0 |= (uint32) ( (0x2) <<SPI_SEL_CS_SHIFT);
    }
}
#else
{
    // set cd in spi group
    uint32 spi_arg=cd_level;

    LCD_SPEC_INFO_T_PTR _lcm_spec_info_ptr = LCM_GetCurrentSpecinfo (0);
    _spi_logicid = _lcm_spec_info_ptr->cd_pin;

    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_CDLEVEL, &spi_arg);
}
#endif

LOCAL void _lcm_setcs (uint32 _spi_logicid, SPI_CSHL_E cs_level)
{
    uint32 spi_arg=0;
    //set spi cs low/high level.
    spi_arg = cs_level;
    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_CS_NO_MUTEX, &spi_arg);
}

//#define SPI_DMA_SUPPORT
#ifdef SPI_DMA_SUPPORT
PUBLIC void DMA_ChannelLinkEn (uint32 chn, uint32 enable)
{
    if (enable == SCI_FALSE)
    {
        * (volatile uint32 *) (DMA_LINK_EN) &= ~ (1<<chn);
    }
    else
    {
        * (volatile uint32 *) (DMA_LINK_EN) |= (1<<chn);
    }
}
/**********************************************************************************************************/
// Description:    DMA Callback
// Global resource dependence:
// Author: lichd
/**********************************************************************************************************/
LOCAL void _lcm_spi_harddma_callback (uint32 channel_id)
{
    uint32 chn = channel_id&0xffff;
    uint32 _spi_logicid;
    uint32 spi_arg;
    LCD_SPEC_INFO_T_PTR  _lcm_spec_info_ptr = LCM_GetCurrentSpecinfo (LCM_GetCurrentLcdid());

    _spi_logicid = _lcm_spec_info_ptr->cs_pin;
    // Restore DMA block delay
    DMA_SetBlockDelay (LCM_DMA_DELAY_DEF);

    // Disable DMA Transfer interrupt
    DMA_ALLTRNSF_INT_DIS (chn);
    // disable dma channel
    spi_arg = SCI_FALSE;
    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_DMA_EN, &spi_arg);
    // disable dma linklist
    DMA_ChannelLinkEn (chn, SCI_FALSE);
    // Disable DMA Channel
    DMA_ChannelEnable (chn, SCI_FALSE);

    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_G_BUSIDLE_STS, &spi_arg);

    if (s_lcm_callbackfunc)
    {
        s_lcm_callbackfunc();
    }

    //set spi cs HIGH.
    _lcm_setcs (_spi_logicid, SPI_CS_HIGH);
    SCI_SPI1_EnableDeepSleep (ENABLE_DEEP_SLEEP);
}
#endif

LOCAL void _lcm_spic_init (uint32 lcd_id)
{
    // Adding your code here
}

/**********************************************************************************************************/
// Description:    LCM reset LCD
// Global resource dependence:
// Author: Fei.zhang
// Note:
//     reset sequence:1-0-delay-1
//       delay_ms - during reset,need to add wait time
/**********************************************************************************************************/
LOCAL void _lcm_spi_resetlcd (uint32 delay_ms,uint8 rst_level)
{
    if (rst_level == 1)
    {
        //do Panel reset
        * (volatile uint32 *) APB_MISC_CTL1 &= ~LCM_RSTN;
        SCI_Sleep (delay_ms);

        * (volatile uint32 *) APB_MISC_CTL1 |= LCM_RSTN;
        SCI_Sleep (delay_ms);

        * (volatile uint32 *) APB_MISC_CTL1 &= ~LCM_RSTN;
        SCI_Sleep (delay_ms);
    }
    else
    {
        //do Panel reset
        * (volatile uint32 *) APB_MISC_CTL1 |= LCM_RSTN;
        SCI_Sleep (delay_ms);

        * (volatile uint32 *) APB_MISC_CTL1 &= ~LCM_RSTN;
        SCI_Sleep (delay_ms);

        * (volatile uint32 *) APB_MISC_CTL1 |= LCM_RSTN;
        SCI_Sleep (delay_ms);
    }
}

/**********************************************************************************************************/
// Description:configure the controller according the LCM timing spec
// Global resource dependence:
// Author: lichd 11.15
// Note:
//         input:
//            ahb_clk - AHB bus clock
//         return:
//            ERR_LCD_NONE,if success
//            other,if fail
//  Note: Not support this function, return  ERR_LCD_NONE derectly
/**********************************************************************************************************/
LOCAL ERR_LCD_E _lcm_spic_setparam (LCD_SPEC_INFO_T_PTR lcd_spec_ptr, uint32 ahb_clk,uint16 lcd_id)
{
    //You must open the logic spi port
    //LCD_SPEC_INFO_T    lcd_spec_info;
    uint32 _spi_logicid;
    //uint32 _spi_logic_cd;
    uint32 spi_arg;
    SPI_DEV spi_dev = {0};
    LCD_SPEC_INFO_T _lcm_spec_info_t = *lcd_spec_ptr;
    _spi_logicid  = _lcm_spec_info_t.cs_pin;

    spi_dev.id = _spi_logicid;
    spi_dev.mode = (_lcm_spec_info_t.spec_ptr->timing->spi_mode_timing.cpol<<1) |
                   (_lcm_spec_info_t.spec_ptr->timing->spi_mode_timing.cpha) ;
    spi_dev.freq          = _lcm_spec_info_t.spec_ptr->timing->spi_mode_timing.freq;
    spi_dev.tx_bit_length = _lcm_spec_info_t.spec_ptr->timing->spi_mode_timing.tx_bitlen;
    //spi_dev.data_mode = _lcm_spec_info_t.spec_ptr->timing->spi_mode_timing.data_mode;     // 2-data lane

    if (SCI_FALSE == SPI_HAL_Open (&spi_dev))
    {
        SCI_TRACE_LOW ("SPI_HAL_Open(): SUCCESS!");
    }

#if 1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
    #if defined(LCD_SPI_3WIRE_9BIT_1DATA)
        spi_arg = SPI_3WIRE_9BIT_SDA;
    #elif defined(LCD_SPI_3WIRE_9BIT_2DATA)
        spi_arg = SPI_3WIRE_9BIT_SDIO;
    #elif defined(LCD_SPI_4WIRE_8BIT_1DATA)
        spi_arg = SPI_4WIRE_8BIT_SDA;
    #elif defined(LCD_SPI_4WIRE_8BIT_2DATA)
        spi_arg = SPI_4WIRE_8BIT_SDIO;
    #endif
#else //CHIP_VER_6531EFM
    spi_arg = SPI_4WIRE_8BIT_SDA;
#endif //CHIP_VER_6531EFM
    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_LCDWORKMODE, &spi_arg);

    /* "0x0" : idle mode
      "0x1" : receive mode
      "0x2" : transmit mode
      "0x3" : transmit and receive
    */
    spi_arg = SPI_RX_AND_TX;
    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_TRANSMODE, &spi_arg);
    //set spi cs high.
    _lcm_setcs (_spi_logicid, SPI_CS_HIGH);

    spi_arg = spi_dev.tx_bit_length;
    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);

    SCI_ASSERT (_lcm_spec_info_t.cs_pin != _lcm_spec_info_t.cd_pin);/*assert to do*/
    //avoid spi drv assert()
    spi_dev.id = _lcm_spec_info_t.cd_pin;

    if (SCI_FALSE == SPI_HAL_Open (&spi_dev))
    {
        SCI_TRACE_LOW ("SPI_HAL_Open(): SUCCESS!");
    }

    return ERR_LCD_NONE;
}

/**********************************************************************************************************/
//  Description: Use SPI to control lcm_panel, TO certify the lcm_cmd_addr and lcm_data_addr
//  Author: lichd 09.03
//  Input:
//          lcd_cs:  0--SPI0CS0-->BASE:0x8E000000,
//                   1--SPI1CS0-->BASE:0x8E003000,
//          lcd_cd:  0--SPI0CS1
//                   1--SPI1CS1
//  Return: 0           successful
//          others      failed
//  Note:
/**********************************************************************************************************/
LOCAL void _lcm_spic_setaddr (uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 _spi_logicid = lcd_cs;
    uint32 spi_rtx_addr;

    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_G_DATAPORT, &spi_rtx_addr);
    g_lcm_addr[lcd_id].cmd_addr  = spi_rtx_addr;
    g_lcm_addr[lcd_id].data_addr = spi_rtx_addr;
}

/**********************************************************************************************************/
// Description:    send command to LCD
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: lichd 11.13
// Note:
//         input:
//            cmd - LCD command
//            lcd_id - LCD cs
//         return:
//            ERR_LCD_NONE,if success
//            other,if fail
/**********************************************************************************************************/

LOCAL ERR_LCD_E _lcm_spic_sendcmd (uint32 cmd,uint32 lcd_id)
{
    uint8  u8_temp=0;
    uint8  tx_bitlen=0;
    //uint8  cd_sel=0;
    uint16 u16_temp=0;

    uint32 _spi_logicid = 0;
    uint32 spi_arg;
    LCD_SPEC_INFO_T_PTR _lcm_spec_info_ptr = LCM_GetCurrentSpecinfo (lcd_id);

    _spi_logicid = _lcm_spec_info_ptr->cs_pin;
    tx_bitlen    = _lcm_spec_info_ptr->spec_ptr->timing->spi_mode_timing.tx_bitlen;

    //set spi cs low.
    _lcm_setcs (_spi_logicid, SPI_CS_LOW);

    if (tx_bitlen == 16)
    {
        spi_arg = 16;
        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);

        _lcm_setcd (_spi_logicid, CMD_LEVEL);
        u16_temp = (uint16) (cmd&0xffff);
        SPI_HAL_LCDWrite (_spi_logicid, (uint32 *) (&u16_temp), 1, 0);
        //return to data_operation: CD=1
        _lcm_setcd (_spi_logicid, DATA_LEVEL);
    }
    else
    {
        spi_arg = 8;
        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);

        _lcm_setcd (_spi_logicid, CMD_LEVEL);
        u8_temp = (uint8) (cmd&0xff);
        SPI_HAL_LCDWrite (_spi_logicid, (uint32 *) (&u8_temp), 1, 0);
        //return to data_operation: CD=1
        _lcm_setcd (_spi_logicid, DATA_LEVEL);
    }

    //set spi cs HIGH.
    _lcm_setcs (_spi_logicid, SPI_CS_HIGH);
    return ERR_LCD_NONE;
}

/**********************************************************************************************************/
// Description:    send data to LCD GRAM
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: lichd 11.13
// Note:
//         input:
//            data - display data
//            lcd_id - LCD cs
//         return:
//            ERR_LCD_NONE,if success
//            other, if fail
/**********************************************************************************************************/
LOCAL ERR_LCD_E _lcm_spic_senddata (uint32 data,uint32 lcd_id)
{
    uint8  u8_temp = 0;
    uint8  tx_bitlen = 0;
    //uint8  cd_sel=0;
    uint16 u16_temp = 0;
    uint32 spi_arg;
    uint32 _spi_logicid = 0;
    LCD_SPEC_INFO_T_PTR _lcm_spec_info_ptr = LCM_GetCurrentSpecinfo (lcd_id);

    _spi_logicid = _lcm_spec_info_ptr->cs_pin;
    tx_bitlen    = _lcm_spec_info_ptr->spec_ptr->timing->spi_mode_timing.tx_bitlen;

    //set spi cs low.
    _lcm_setcs (_spi_logicid, SPI_CS_LOW);

    if (tx_bitlen == 16)
    {
        spi_arg = 16;
        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);

        u16_temp = (uint16) (data&0xffff);
        SPI_HAL_LCDWrite (_spi_logicid, (uint32 *) (&u16_temp), 1, 0);
        //SPI_WriteData (data, 1, 0);
    }
    else
    {
        spi_arg = 8;
        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);

        u8_temp = (uint8) (data&0xff);
        SPI_HAL_LCDWrite (_spi_logicid, (uint32 *) (&u8_temp), 1, 0);
    }

    //set spi cs HIGH.
    _lcm_setcs (_spi_logicid, SPI_CS_HIGH);
    return ERR_LCD_NONE;
}

/**********************************************************************************************************/
//  Description:   construct the dma linklist node.
//  Author:        lichd
//  Note:          used for dma linklist mode
/**********************************************************************************************************/
LOCAL void _DMA_LINKLIST_CONSTRUCT (
    DMA_CHANNEL_CONFIG_T *linklist,uint32 num,int32 node_step,
    int32 dma_cfg0, uint32 dma_total_size, uint32 dma_src_addr, uint32 dma_des_addr,
    uint32 llptr,     int32 dma_di,          int32 dma_sbi,       int32 dma_dbi
)
{
    uint32 i=0;

    for (i=0; i<num-1; i++)
    {
        linklist[i].cfg0 =     dma_cfg0;
        linklist[i].cfg1 =     dma_total_size;

        if (i==0)
        {
            linklist[i].src_addr = dma_src_addr;
        }
        else
        {
            dma_src_addr = (uint32) ( (int32) dma_src_addr + node_step);
            linklist[i].src_addr = dma_src_addr;
        }

        linklist[i].dst_addr = dma_des_addr;

        linklist[i].element_step = dma_di;
        linklist[i].src_blockstep= dma_sbi;
        linklist[i].dst_blockstep= dma_dbi;
        linklist[i].lptr         = (uint32) &linklist[i+1];
    }

    linklist[i].cfg0 = (int32)(BIT_31) |dma_cfg0;
    linklist[i].cfg1 =     dma_total_size;

    if (i==0)
    {
        linklist[i].src_addr = dma_src_addr;
    }
    else
    {
        dma_src_addr = (uint32) ( (int32) dma_src_addr + node_step);
        linklist[i].src_addr = dma_src_addr;
    }

    linklist[i].dst_addr = dma_des_addr;

    linklist[i].element_step = dma_di;
    linklist[i].src_blockstep= dma_sbi;
    linklist[i].dst_blockstep= dma_dbi;
    linklist[i].lptr         = (uint32) &linklist[0];
}

/**********************************************************************************************************/
//  Description:   _lcm_spi_dmawrite
//  Author:        lichd
//  Note: Dependence DMA_hal.h for SC6610
//  [in]--bus_width,     //only support WIDTH-8 lcm pannel with spi or parallel interface.
//  [in]--dir_type,      //clockwise rotate_:0,90',,180', 270';mirror support also.
//  [in]--src_addr,      //
//  [in]--des_addr,      //
//  [in]--data_width,    //
//  [in]--data_height,   //
//  [in]--buf_width,     //
//  [in]--brush_mode,    //async or sync
//  [out]--BOOLEAN
/**********************************************************************************************************/
LOCAL uint32 _lcm_spi_dmawrite (
    LCD_BUS_WIDTH_E bus_width,
    uint32 src_addr,
    uint32 des_addr,
    uint32 data_width,
    uint32 data_height,
    uint32 buf_width,
    LCD_DIRECT_E dir_type,
    LCD_BRUSH_MODE_E brush_mode
)
{
    //Need to be done
#ifdef SPI_DMA_SUPPORT
    uint32 LCM_DATA_ADDR= des_addr;
    uint32 channel  = 0;
    uint32 spi_port_used=0;

    LCD_DIRECT_E    rotation_angle = dir_type;

    //DMA_chanel_configuration:
    int32 dma_cfg0 = 0;
    uint32 dma_total_size = 0;           //total_size
    uint32 dma_src_addr = 0;
    uint32 dma_dst_addr = 0;

    uint32 dma_lptr = 0;
    int32 dma_di   = 0;                //src and dst element_step
    int32 dma_sbi  = 0;                //src block step
    int32 dma_dbi  = 0;                //dst block step

    uint32 src_data_width=0;            //data bus width
    uint32 dst_data_width=0;
    uint32 block_size    =0;

    uint32 src_elementstep =0;
    uint32 dst_elementstep =0;

    uint32 data_src_addr  = src_addr;   //picture_data parameter
    uint32 data_src_width = data_width;
    uint32 data_src_height= data_height;

    uint32 row_column_num = 0;          //for Linklist node
    int32 node_step = 0;

    uint32 spi_arg;
    uint32 _spi_logicid = 0;
    SPI_RTX_DMA_CH_T spi_dma_chn;

    LCD_SPEC_INFO_T_PTR _lcm_spec_info_ptr = LCM_GetCurrentSpecinfo (LCM_GetCurrentLcdid());

    _spi_logicid = _lcm_spec_info_ptr->cs_pin;
    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_G_DMA_CHNID, &spi_dma_chn);
    channel = spi_dma_chn.spi_dma_tx_ch;

    //set spi cs low.
    _lcm_setcs (_spi_logicid, SPI_CS_LOW);

    //set txt mode
    spi_arg = SPI_TX_ONLY;
    SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_TRANSMODE, &spi_arg);

    //only set blockdelay with burstdelay=8clock
    DMA_SetBlockDelay (0x10);

    if ( (rotation_angle == LCD_DIRECT_ROT_90)
            || (rotation_angle == LCD_DIRECT_ROT_270))
    {
        uint32 temp;

        temp = data_width;
        data_width =data_height;
        data_height =temp;
    }

    switch (rotation_angle)
    {
        case LCD_DIRECT_NORMAL:
            {
                if ( (buf_width == data_width) && (LCD_BRUSH_MODE_SYNC == brush_mode))
                {
                    // If the picture is identical with the panel, use spi_hal driver to show
                    SPI_HAL_Write (_spi_logicid,
                                   (uint8 *) src_addr,  data_width*data_height*2);
                    return SCI_TRUE;
                }
                else
                {
                    // halfword aligned dma access
                    if ( (data_width & 0x0001) || (src_addr & 0x0003))
                    {
                        row_column_num = data_height;
                        node_step      = (buf_width*2);
                        spi_arg = 16;
                        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);

                        src_data_width = DMA_DATAWIDTH_HALFWORD;
                        dst_data_width = DMA_DATAWIDTH_HALFWORD;

                        dma_src_addr =   src_addr;
                        src_elementstep= 2;
                        dst_elementstep= 0;
                        dma_di  = (0<<31) | (src_elementstep<<16) | (0<<15) |dst_elementstep;
                        dma_sbi = (0<<31) | (2);
                        dma_dst_addr = des_addr;
                        dma_dbi = 0;
                        block_size = SPIDMA_BURST_SIZE;
                        dma_cfg0 = (uint32) ( (src_data_width<<26) | (dst_data_width<<24) | (block_size));

                        dma_lptr =0;
                        dma_total_size = data_width*2;
                    }
                    // word aligned dma access, set dma_cfg0.[19] to improve efficiency
                    else
                    {
                        row_column_num = data_height;
                        node_step      = (buf_width*2);
                        spi_arg = 32;
                        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);
                        src_data_width = DMA_DATAWIDTH_WORD;
                        dst_data_width = DMA_DATAWIDTH_WORD;

                        dma_src_addr =   src_addr;
                        src_elementstep= 4;
                        dst_elementstep= 0;
                        dma_di  = (0<<31) | (src_elementstep<<16) | (0<<15) |dst_elementstep;
                        dma_sbi = (0<<31) | (4);
                        dma_dst_addr = des_addr;
                        dma_dbi = 0;
                        block_size = SPIDMA_BURST_SIZE;
                        dma_cfg0 = (uint32) ( (1<<19) | (src_data_width<<26) | (dst_data_width<<24) | (block_size));

                        dma_lptr =0;
                        dma_total_size = data_width*2;
                    }
                }

                break;
            }
        case LCD_DIRECT_ROT_90: //clockwise rotate_90
            {
                row_column_num = data_width;
                node_step = 2;

                spi_arg = 16;
                SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);

                src_data_width = DMA_DATAWIDTH_HALFWORD;
                dst_data_width = DMA_DATAWIDTH_HALFWORD;

                // address:@psrc+((height-1)DD*width+0)*2
                dma_src_addr = data_src_addr+ ( (data_height-1) *buf_width+0) *2;
                src_elementstep= (buf_width*2-1);
                dst_elementstep= 0;
                dma_di  = (1<<31) | (src_elementstep<<16) | (0<<15) |dst_elementstep;
                dma_sbi = (1<<31) | (buf_width*2-1);
                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;

                block_size = SPIDMA_BURST_SIZE;
                dma_cfg0 = (src_data_width<<26)  |
                           (dst_data_width<<24)  |
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_height <<1);
            }
            break;
        case LCD_DIRECT_ROT_180://clockwise rotate_180
            {
                row_column_num = data_height;
                node_step      = - (buf_width<<1);

                spi_arg = 16;
                SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);
                src_data_width = DMA_DATAWIDTH_HALFWORD;
                dst_data_width = DMA_DATAWIDTH_HALFWORD;

                // address:@d=psrc+((height-1)DD*width+(width-1))*2
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+ (data_src_width-1)) *2;
                src_elementstep= 2-1;
                dst_elementstep=0;
                dma_di  = (1<<31) | (src_elementstep<<16) | (0<<15) |dst_elementstep;
                dma_sbi = (1<<31) | (2-1);

                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;

                block_size = SPIDMA_BURST_SIZE;
                dma_cfg0 = (src_data_width<<26)  |
                           (dst_data_width<<24)  |
                           (block_size);
                dma_lptr =0;
                dma_total_size= data_src_width<<1;
            }
            break;
        case LCD_DIRECT_ROT_270://clockwise rotate_270

            {
                row_column_num = data_width;
                node_step      = -2;

                spi_arg = 16;
                SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);

                src_data_width = DMA_DATAWIDTH_HALFWORD;
                dst_data_width = DMA_DATAWIDTH_HALFWORD;

                // address:@psrc+(0DD+(width-1))*2
                dma_src_addr = data_src_addr+ (0+ (data_src_width-1)) *2;
                src_elementstep= buf_width*2;
                dst_elementstep=0;
                dma_di  = (0<<31) | (src_elementstep<<16) | (0<<15) |dst_elementstep;
                dma_sbi = (0<<31) | (buf_width*2);  //

                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;

                block_size = SPIDMA_BURST_SIZE;

                dma_cfg0 = (src_data_width<<26)  |
                           (dst_data_width<<24)  |
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_height <<1);
            }
            break;
        case LCD_DIRECT_MIR_H:  //mirror horizontal
            {
                row_column_num = data_height;
                node_step      = (buf_width*2);

                spi_arg = 16;
                SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);

                src_data_width = DMA_DATAWIDTH_HALFWORD;
                dst_data_width = DMA_DATAWIDTH_HALFWORD;

                // address:a@b=psrc+(0DD+(width-1))*2
                dma_src_addr = data_src_addr+ (0+ (data_src_width-1)) *2;
                src_elementstep= 2-1;
                dst_elementstep= 0;
                dma_di  = (1<<31) | (src_elementstep<<16) | (0<<15) |dst_elementstep;
                dma_sbi = (1<<31) | (2-1);

                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;

                block_size = SPIDMA_BURST_SIZE;
                dma_cfg0 = (src_data_width<<26)  |
                           (dst_data_width<<24)  |
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_width) *2;
            }
            break;
        case LCD_DIRECT_MIR_V:  //mirror vertical
            {
                row_column_num = data_height;
                node_step      = - (buf_width<<1);

                spi_arg = 16;
                SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);
                src_data_width = DMA_DATAWIDTH_HALFWORD;
                dst_data_width = DMA_DATAWIDTH_HALFWORD;

                // address:@c=psrc+((height-1)DD*width+0)*2
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+0) *2;
                src_elementstep= 2;
                dst_elementstep=0;
                dma_di  = (0<<31) | (src_elementstep<<16) | (0<<15) |dst_elementstep;
                dma_sbi = (0<<25) | (2);

                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;

                block_size = SPIDMA_BURST_SIZE;
                dma_cfg0 = (src_data_width<<26)  |
                           (dst_data_width<<24)  |
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_width) *2;
            }
            break;
        case LCD_DIRECT_MIR_HV:  //mirror horizontal&vertical
            {
                row_column_num = data_height;
                node_step      = - (buf_width<<1);

                spi_arg = 16;
                SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_BITLEN, &spi_arg);
                src_data_width = DMA_DATAWIDTH_HALFWORD;
                dst_data_width = DMA_DATAWIDTH_HALFWORD;

                // address:@d=psrc+((height-1)DD*width+(width-1))*2
                dma_src_addr = data_src_addr+ ( (data_src_height-1) *buf_width+ (data_src_width-1)) *2;
                src_elementstep= 2-1;
                dst_elementstep=0;
                dma_di  = (1<<31) | (src_elementstep<<16) | (0<<15) |dst_elementstep;
                dma_sbi = (1<<31) | (2-1);

                dma_dst_addr = LCM_DATA_ADDR;
                dma_dbi = 0;

                block_size = SPIDMA_BURST_SIZE;
                dma_cfg0 = (src_data_width<<26)  |
                           (dst_data_width<<24)  |
                           (block_size);
                dma_lptr =0;
                dma_total_size= (data_width) *2;
            }
            break;
        default:
            SCI_TRACE_LOW ("LCM: w = %d, h = %d, rotate=%d", data_src_width, data_src_height, rotation_angle);
            break;

    }

    // Construct the linklist of dma
    _DMA_LINKLIST_CONSTRUCT (dma_channelcfg_llist, row_column_num, node_step,
                             dma_cfg0,  dma_total_size,  dma_src_addr,  dma_dst_addr,
                             0,         dma_di,          dma_sbi,       dma_dbi);

    // validate DMA Configiguration
    DMA_CHN_QUICKCFG (
        channel,
        dma_cfg0,
        dma_total_size,
        dma_src_addr,
        dma_dst_addr,
        (uint32) &dma_channelcfg_llist[0],
        dma_di,
        dma_sbi,
        dma_dbi);


    if (LCD_BRUSH_MODE_SYNC == brush_mode)                         // Sync polling model
    {
        // Diaable DMA INTERRUPT
        DMA_ALLTRNSF_INT_DIS (channel);
        // Enable DMA
        spi_arg = SCI_TRUE;
        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_DMA_EN, &spi_arg);
        DMA_ChannelLinkEn (channel, SCI_TRUE);                     // dma link enable
        DMA_ChannelEnable (channel,SCI_TRUE);

        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_G_BUSIDLE_STS, &spi_arg);

        _WaitDmaDone (channel, LCM_DMA_BLOCK_TIMEOUT);             // LCM_DMA_BLOCK_TIMEOUT=5ms

        //disable dma channel
        spi_arg = SCI_FALSE;
        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_DMA_EN, &spi_arg);
        DMA_ChannelLinkEn (channel, SCI_FALSE);                    // dma link disable
        DMA_ChannelEnable (channel,SCI_FALSE);

        //set spi cs HIGH.
        _lcm_setcs (_spi_logicid, SPI_CS_HIGH);
    }
    else if (LCD_BRUSH_MODE_ASYNC == brush_mode)                   // Async interrupt model
    {
        SCI_SPI1_EnableDeepSleep (DISABLE_DEEP_SLEEP);

        // Register DMA callback funtion
        DMA_RegCallBack (channel, _lcm_spi_harddma_callback);

        // enable DMA AllTransfer_done interrupt
        DMA_ALLTRNSF_INT_EN (channel);

        spi_arg = SCI_TRUE;
        SPI_HAL_Ioctl (_spi_logicid, SPI_CTL_S_DMA_EN, &spi_arg);
        DMA_ChannelLinkEn (channel, SCI_TRUE);                      // dma link enable
        DMA_ChannelEnable (channel,SCI_TRUE);                       // start dma transfer
    }

#endif

    return SCI_SUCCESS;
}

/**********************************************************************************************************/
//  Description:   Read LCM ID
//  Global resource dependence:
//  Author: lichd
//  Note: Only suport reading id after lcm init success
/**********************************************************************************************************/
LOCAL uint32 _lcm_spic_readdata (uint32 lcd_id, uint32 cd_level)
{
    uint32 data_result=0;
    /*
        uint32 data_len=1;

        uint32 _spi_logicid = 0;
        uint32 spi_arg;
        LCD_SPEC_INFO_T_PTR _lcm_spec_info_ptr = LCM_GetCurrentSpecinfo( lcd_id);

        _spi_logicid = _lcm_spec_info_ptr->cs_pin;

        SCI_ASSERT(data_len <= 1);

        //set spi cs low. DATA_LEVEL
        _lcm_setcs(_spi_logicid, SPI_CS_LOW);
        _lcm_setcd(_spi_logicid, cd_level);

        SPI_HAL_LCDRead(_spi_logicid, &data_result, data_len, 0);

        //set spi cs height.
        _lcm_setcs(_spi_logicid, SPI_CS_HIGH);
    */
    return data_result;
}

PUBLIC void SPI_SetMode(uint32 spi_mode)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
    uint32 temp = spi_ctr_ptr->ctl7;

    temp &= ~ (0x7 << 3);;
    temp |= (spi_mode<<3);

    spi_ctr_ptr->ctl7 = temp;
}

PUBLIC void SPI_SetClkDiv(uint32 div)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

    spi_ctr_ptr->clkd = div;
}

PUBLIC void  SPI_SetReadS(uint32 datawidth)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

    uint32 temp = spi_ctr_ptr->ctl2;
  
    temp = temp&(~0x1f);
    temp = temp | (datawidth&0x1f);
		
    spi_ctr_ptr->ctl2 = temp;
}

PUBLIC void SPI_SetRWMode(void)
{
      uint32 ctl1;
      volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
	  
      ctl1 = spi_ctr_ptr->ctl1;	  
      spi_ctr_ptr->ctl1  = ctl1 | BIT_12 | BIT_13;     // set RT mode
 
}

// Transmit data bit number:spi_ctl0[6:2]
PUBLIC void  SPI_SetDatawidth (uint32 datawidth)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *) (SPI_USED_BASE);
    uint32 temp = spi_ctr_ptr->ctl0;

    if (32 == datawidth)
    {
        spi_ctr_ptr->ctl0 &= ~0x7C;  //  [6:2]
        return;
    }

    temp &= ~0x0000007C;  //mask
    temp |= (datawidth<<2);

    spi_ctr_ptr->ctl0 = temp;
}

PUBLIC void SPI_SetCsLow (uint32 spi_sel_csx , BOOLEAN is_low)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *) (SPI_USED_BASE);

    if (is_low)
    {
        //spi_ctl0[11:8]:cs3<->cs0 chip select, 0-selected;1-none
        spi_ctr_ptr->ctl0 &= ~ (uint32) (SPI_SEL_CS_MASK);
        spi_ctr_ptr->ctl0 &= ~ (uint32) ( (1<<spi_sel_csx) <<SPI_SEL_CS_SHIFT);
    }
    else
    {
        //spi_ctl0[11:8]:cs3<->cs0 chip select, 0-selected;1-none
        spi_ctr_ptr->ctl0 |= ( (1<<spi_sel_csx) <<SPI_SEL_CS_SHIFT);
    }
}

#define SPI_CD_MASK  BIT_15
PUBLIC void SPI_SetCd (uint32 cd_level)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *) (SPI_USED_BASE);

    //0-command;1-data
    if (cd_level == 0)
    {
        spi_ctr_ptr->ctl8 &= ~ (SPI_CD_MASK);
    }
    else
    {
        spi_ctr_ptr->ctl8 |= (SPI_CD_MASK);
    }
}

#define TX_MAX_LEN_MASK     0xFFFFF
#define TX_DUMY_LEN_MASK    0x3F    //[09:04]
#define TX_DATA_LEN_H_MASK  0x0F    //[03:00]
#define TX_DATA_LEN_L_MASK  0xFFFF  //[15:00]
/*****************************************************************************/
//  Description:  Set rxt data length with dummy_len
//  Author     :  lichd
//  Note       :  the unit is identical with datawidth you set
/*****************************************************************************/
PUBLIC void SPI_SetTxLen (uint32 data_len, uint32 dummy_bitlen)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *) (SPI_USED_BASE);
    uint32 ctl8 = spi_ctr_ptr->ctl8;
    uint32 ctl9 = spi_ctr_ptr->ctl9;

    data_len &= TX_MAX_LEN_MASK;

    ctl8 &= ~ ( (TX_DUMY_LEN_MASK<<4) | TX_DATA_LEN_H_MASK);
    ctl9 &= ~ (TX_DATA_LEN_L_MASK);

    // set dummy_bitlen in bit[9:4] and data_len[19:16] in bit[3:0]
    spi_ctr_ptr->ctl8 = (ctl8 | (dummy_bitlen<<4) | (data_len>>16));
    // set data_len[15:00]
    spi_ctr_ptr->ctl9 = (ctl9 | (data_len&0xFFFF));
}
#define RX_MAX_LEN_MASK     0xFFFFFF
#define RX_DUMY_LEN_MASK    0x3F    //[09:04]
#define RX_DATA_LEN_H_MASK  0x0F    //[03:00]
#define RX_DATA_LEN_L_MASK  0xFFFF  //[15:00]
PUBLIC void SPI_SetRxLen(uint32 data_len, uint32 dummy_bitlen)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
    uint32 ctl10 = spi_ctr_ptr->ctl10;
    uint32 ctl11 = spi_ctr_ptr->ctl11;

    data_len &= RX_MAX_LEN_MASK;

    ctl10 &= ~((RX_DUMY_LEN_MASK<<4) | RX_DATA_LEN_H_MASK);
    ctl11 &= ~( RX_DATA_LEN_L_MASK );

    // set dummy_bitlen in bit[9:4] and data_len[19:16] in bit[3:0]
    spi_ctr_ptr->ctl10 = (ctl10 | (dummy_bitlen<<4) | (data_len>>16));
    // set data_len[15:00]
    spi_ctr_ptr->ctl11 = (ctl11 | (data_len&0xFFFF));
}

// Request txt trans before send data
#define SW_RX_REQ_MASK BIT_0
#define SW_TX_REQ_MASK BIT_1
PUBLIC void SPI_TxReq (void)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *) (SPI_USED_BASE);

    spi_ctr_ptr->ctl12 |= SW_TX_REQ_MASK;
}

PUBLIC void SPI_RxTxReq( void )
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

    spi_ctr_ptr->ctl12 |= (SW_RX_REQ_MASK|SW_TX_REQ_MASK);
}

PUBLIC void SPI_WaitTxFinish()
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *) (SPI_USED_BASE);

    while (! ( (spi_ctr_ptr->iraw) &BIT_8)) // TX_END_IRQ,1-end
    {
    }

    spi_ctr_ptr->iclr |= BIT_8;

    // Wait for spi bus idle
    while ( (spi_ctr_ptr->sts2) &BIT_8)
    {
    }

    // Wait for tx real empty
    while (! ( (spi_ctr_ptr->sts2) & BIT_7))
    {
    }
}

PUBLIC uint32 SPI_WriteReadData(uint32 data, uint32 data_len, uint32 dummy_bitlen)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
    // The unit of data_len is identical with buswidth
    SPI_SetTxLen(data_len, dummy_bitlen);
    SPI_SetRxLen(data_len, dummy_bitlen);	
    SPI_RxTxReq( );
 	
    spi_ctr_ptr->data = data;
    SPI_WaitTxFinish();
    // Wait for RX data
    while((spi_ctr_ptr->sts2)&BIT_5) 
    {
    }	
    return (spi_ctr_ptr->data);
}

PUBLIC void SPI_WriteData (uint32 data, uint32 data_len, uint32 dummy_bitlen)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *) (SPI_USED_BASE);

    // The unit of data_len is identical with buswidth
    SPI_SetTxLen (data_len, dummy_bitlen);
    SPI_TxReq();

    spi_ctr_ptr->data = data;

    SPI_WaitTxFinish();
}

#if 1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
PUBLIC void  SPI_SelectData2Pin(void)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);
    uint32 temp = spi_ctr_ptr->ctl8;

#ifdef SPI_DATA2_SEL_CD
    spi_ctr_ptr->ctl8 = temp|(0x1<<13); 
#else // SPI_DATA2_SEL_DI
    spi_ctr_ptr->ctl8 = temp&(~(0x1<<13));
#endif

}

PUBLIC void  SPI_SelectData2RGB565(void)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

    uint32 temp = spi_ctr_ptr->ctl7;

    spi_ctr_ptr->ctl7 = temp|(0xC000);  //  [15:14]

}

PUBLIC void  SPI_SelectDataRGB565(void)
{
    volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *)(SPI_USED_BASE);

    uint32 temp = spi_ctr_ptr->ctl7;

    spi_ctr_ptr->ctl7 = temp|(0x4000);  //  [15:14]

}
#endif //CHIP_VER_6531EFM

/*****************************************************************************/
// Description: Valid the data_num with data_width
// Global resource dependence:
// Author: lichd Dec.10.
// Note:
/*****************************************************************************/
PUBLIC uint32 SPI_ValidRect (uint32 data_num, uint32 lcd_id)
{
    uint8  tx_bitlen=0;
    uint32 _spi_logicid = 0;

    LCD_SPEC_INFO_T_PTR _lcm_spec_info_ptr = LCM_GetCurrentSpecinfo (lcd_id);

    {
        volatile SPI_CTL_REG_T *spi_ctr_ptr = (volatile SPI_CTL_REG_T *) (SPI_USED_BASE);
        uint32   temp = spi_ctr_ptr->ctl7;
        spi_ctr_ptr->ctl7 = temp | BIT_7; //Set TX_HL_EN
    }

    _spi_logicid = _lcm_spec_info_ptr->cs_pin;
    tx_bitlen    = _lcm_spec_info_ptr->spec_ptr->timing->spi_mode_timing.tx_bitlen;

    _lcm_setcs (_spi_logicid, SPI_CS_LOW);
    _lcm_setcd (_spi_logicid, DATA_LEVEL);

#if 1 //defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
    #if defined(LCD_SPI_3WIRE_9BIT_1DATA)
        SPI_SetDatawidth(SPI_3W9B1D_BW);
        SPI_SelectDataRGB565();
    #elif defined(LCD_SPI_3WIRE_9BIT_2DATA)
        SPI_SetDatawidth(SPI_3W9B2D_BW);
        SPI_SelectData2Pin();
        SPI_SelectData2RGB565();
    #elif defined(LCD_SPI_4WIRE_8BIT_1DATA)
        SPI_SetDatawidth(SPI_4W8B1D_BW);	
    #elif defined(LCD_SPI_4WIRE_8BIT_2DATA)
        SPI_SetDatawidth(SPI_4W8B2D_BW);
        SPI_SelectData2Pin(); // not verify
    #endif
#else
    SPI_SetDatawidth(16);
#endif //CHIP_VER_6531EFM
    SPI_SetTxLen (data_num, 0);

    SPI_TxReq();

    return SCI_TRUE;
}

PUBLIC uint32 SPI_ValidRect_End (uint32 lcdid)
{
	uint32 _spi_logicid = 0;
    //uint32 _spi_cdpin=0;
    //uint32 spi_arg;
    LCD_SPEC_INFO_T_PTR _lcm_spec_info_ptr = LCM_GetCurrentSpecinfo (lcdid);
	_spi_logicid = _lcm_spec_info_ptr->cs_pin;
    _lcm_setcs (_spi_logicid, SPI_CS_HIGH);

	return SCI_TRUE;
}

// Configure the object of s_if_spi
const PUBLIC LCM_CONTROLLER_T  s_if_spi =
{
    _lcm_spic_init,
    _lcm_spi_resetlcd,

    _lcm_spic_setparam,
    _lcm_spic_setaddr,

    _lcm_spic_senddata,
    _lcm_spic_sendcmd,
    _lcm_spi_dmawrite,
    _lcm_spic_readdata,
};

#ifdef   __cplusplus
}
#endif

