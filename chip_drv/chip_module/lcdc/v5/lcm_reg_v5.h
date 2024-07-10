/******************************************************************************
 ** File Name:    lcdc_lcm_reg_v5.h                                           *
 ** Author:       Changde.Li                                                  *
 ** DATE:         09/16/2011                                                  *
 ** Copyright:    2011 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 09/16/2011    changde         Modify for sc6530                           *
 ******************************************************************************/
#ifndef _LCDC_LCM_REG_V5_H_
#define _LCDC_LCM_REG_V5_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**----------------------------------------------------------------------------*/
/*
    LCM CS address
*/
#define  LCM_BASE_ADDR  LCM_SPACE_BASE

/*
  Control register for LCM operation
*/
//#define LCM_REG_BASE        0x20800000
#define LCM_CFG0            (LCM_REG_BASE + 0x00)
#define LCM_STS0            (LCM_REG_BASE + 0x08)
#define LCM_CS0_CFG0        (LCM_REG_BASE + 0x10)
#define LCM_CS0_CFG1        (LCM_REG_BASE + 0x14)
#define LCM_CS1_CFG0        (LCM_REG_BASE + 0x20)
#define LCM_CS1_CFG1        (LCM_REG_BASE + 0x24)
#define LCM_CS2_CFG0        (LCM_REG_BASE + 0x30)
#define LCM_CS2_CFG1        (LCM_REG_BASE + 0x34)
#define LCM_CS3_CFG0        (LCM_REG_BASE + 0x40)
#define LCM_CS3_CFG1        (LCM_REG_BASE + 0x44)
#define LCM_CS4_CFG0        (LCM_REG_BASE + 0x50)
#define LCM_CS4_CFG1        (LCM_REG_BASE + 0x54)

//define DMA req_id_addr for this module
#define LCM_REQ_ID          DMA_FMARK_LCM  
#if !defined(PLATFORM_UWS6121E)
#define LCM_REQ_ID_ADDR     (DMA_CTL_REG_BASE+0x2000+(LCM_REQ_ID-1)*0x04)
#else
#define LCM_REQ_ID_ADDR     (REG_DMA_BASE+0x40+(LCM_REQ_ID-1)*0x04)
#endif
/**---------------------------------------------------------------------------*
 **                        Protocol Define                                     *
 **----------------------------------------------------------------------------*/
//FOR SC6600L6 big-endian
typedef union _lcm_cfg0_tag                         //@0x20800000
{
    struct _lcm_cfg0_map
    {
        volatile  uint   rf_cs_position :2;         //[1:0]
        volatile  uint   reserved       :14;        //[15:2]
        volatile  uint   r2r_turnaround :4;         //[19:16]
        volatile  uint   r2w_turnaround :4;         //[23:20]
        volatile  uint   w2r_turnaround :4;         //[27:24]
        volatile  uint   w2w_turnaround :4;         //[31:28]
    } mBits;
    volatile  uint   dwValue;
} LCM_CFG0_U;

typedef union _lcm_sts0_tag                         //@0x20800008
{
    struct _lcm_sts0_map
    {
        volatile  uint   rfsts_smem_busy    :1;     //[0]    
        volatile  uint   rfsts_wbuf_wr_busy :1;     //[1]
        volatile  uint   reserved       :30;        //Reserved
    } mBits;
    volatile  uint   dwValue;
} LCM_STS0_U;

//CS0: @0x20800010 //@0x20800014
//CS1: @0x20800020 //@0x20800024
//CS2: @0x20800030 //@0x20800034
//CS3: @0x20800040 //@0x20800044
typedef union _lcm_cs_cfg0_tag
{
    struct _lcm_cs_cfg0_map
    {
        volatile  uint   data_width     :3;         //[2:0] split by 8bit/16bit/2bit/9bit/12bit
        volatile  uint   data_num       :2;         //[4:3] data spliting number
        volatile  uint   data_switch    :2;         //[6:5] data switch
        volatile  uint   reserved       :25;        //[31:7]        
    } mBits;
    volatile  uint   dwValue;
} LCM_CS_CFG0_U;
typedef union _lcm_cs_cfg1_tag
{
    struct _lcm_cs_cfg1_map
    {
        volatile  uint   rf_t_oe_stp_r      :3;     //[2:0]
        volatile  uint   rf_t_oe_hld_r      :1;     //[3]	
        volatile  uint   rf_t_we_stp_w      :3;     //[6:4]
        volatile  uint   rf_t_data_oe_hld_w :1;     //[7] 
        volatile  uint   rf_t_we_wdt_w      :4;     //[11:08] 
        volatile  uint   rf_t_ce_stp_r      :1;     //[12]
        volatile  uint   rf_t_ce_stp_w      :1;     //[13]  
        volatile  uint   rf_t_ce_hld_r      :1;     //[14]  
        volatile  uint   rf_t_ce_hld_w      :1;     //[15] 
        volatile  uint   rf_t_first_r       :5;     //[20:16] 
        volatile  uint   rf_t_first_w       :5;     //[25:21] 
        volatile  uint   reserved           :6;     //[31:26]        
    } mBits;
    volatile  uint   dwValue;
} LCM_CS_CFG1_U;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End




