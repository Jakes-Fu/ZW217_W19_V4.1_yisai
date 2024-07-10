/******************************************************************************
 ** File Name:        sfc_reg_v5.h                                            *
 ** Author:           Yi.Qiu                                                  *
 ** DATE:             11/05/2010                                              *
 ** Copyright:        2010 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE            NAME            DESCRIPTION                               *
 ** 11/05/2010    Yi.Qiu            the first version                         *
 ******************************************************************************/
#ifndef _SFC_REG_V5_H_
#define _SFC_REG_V5_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------  */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **-------------------------------------------------------------------------- */
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**--------------------------------------------------------------------------- */
/* TPC control registers */
#define SFC_CMD_CFG                      (SFC_REG_BASE + 0x0000)
#define SFC_SOFT_REQ                     (SFC_REG_BASE + 0x0004)
#define SFC_TBUF_CLR                     (SFC_REG_BASE + 0x0008)
#define SFC_INT_CLR                      (SFC_REG_BASE + 0x000C)
#define SFC_STATUS                       (SFC_REG_BASE + 0x0010)
#define SFC_CS_TIMING_CFG                (SFC_REG_BASE + 0x0014)
#define SFC_RD_SAMPLE_CFG                (SFC_REG_BASE + 0x0018)
#define SFC_CLK_CFG                      (SFC_REG_BASE + 0x001C)
#define SFC_CS_CFG                       (SFC_REG_BASE + 0x0020)
#define SFC_ENDIAN_CFG                   (SFC_REG_BASE + 0x0024)
#define SFC_IO_DLY_CFG                   (SFC_REG_BASE + 0x0028)
#define SFC_WP_HLD_INIT                  (SFC_REG_BASE + 0x002C)
#define SFC_CMD_BUF0                     (SFC_REG_BASE + 0x0040)
#define SFC_CMD_BUF1                     (SFC_REG_BASE + 0x0044)
#define SFC_CMD_BUF2                     (SFC_REG_BASE + 0x0048)
#define SFC_CMD_BUF3                     (SFC_REG_BASE + 0x004C)
#define SFC_CMD_BUF4                     (SFC_REG_BASE + 0x0050)
#define SFC_CMD_BUF5                     (SFC_REG_BASE + 0x0054)
#define SFC_CMD_BUF6                     (SFC_REG_BASE + 0x0058)
#define SFC_CMD_BUF7                     (SFC_REG_BASE + 0x005C)
#define SFC_CMD_BUF8                     (SFC_REG_BASE + 0x0060)
#define SFC_CMD_BUF9                     (SFC_REG_BASE + 0x0064)
#define SFC_CMD_BUF10                    (SFC_REG_BASE + 0x0068)
#define SFC_CMD_BUF11                    (SFC_REG_BASE + 0x006C)
#define SFC_TYPE_BUF0                    (SFC_REG_BASE + 0x0070)
#define SFC_TYPE_BUF1                    (SFC_REG_BASE + 0x0074)
#define SFC_TYPE_BUF2                    (SFC_REG_BASE + 0x0078)

#define SFC_CS1_CMD_CFG                (SFC_REG_BASE + 0x0080)
#define SFC_CS1_TBUF_CLR               (SFC_REG_BASE + 0x0088)
#define SFC_CS1_CS_TIMING_CFG     (SFC_REG_BASE + 0x0094)
#define SFC_CS1_RD_SAMPLE_CFG    (SFC_REG_BASE + 0x0098)
#define SFC_CS1_CLK_CFG                 (SFC_REG_BASE + 0x009C)
#define SFC_CS1_ENDIAN_CFG          (SFC_REG_BASE + 0x00B4)
#define SFC_CS1_IO_DLY_CFG           (SFC_REG_BASE + 0x00B8)
#define SFC_CS1_WP_HLD_INIT         (SFC_REG_BASE + 0x00BC)
#define SFC_CS1_CMD_BUF0              (SFC_REG_BASE + 0x00C0)
#define SFC_CS1_CMD_BUF1              (SFC_REG_BASE + 0x00C4)
#define SFC_CS1_CMD_BUF2              (SFC_REG_BASE + 0x00C8)
#define SFC_CS1_CMD_BUF3              (SFC_REG_BASE + 0x00CC)
#define SFC_CS1_CMD_BUF4              (SFC_REG_BASE + 0x00D0)
#define SFC_CS1_CMD_BUF5              (SFC_REG_BASE + 0x00D4)
#define SFC_CS1_CMD_BUF6              (SFC_REG_BASE + 0x00D8)
#define SFC_CS1_CMD_BUF7              (SFC_REG_BASE + 0x00DC)
#define SFC_CS1_CMD_BUF8              (SFC_REG_BASE + 0x00E0)
#define SFC_CS1_CMD_BUF9              (SFC_REG_BASE + 0x00E4)
#define SFC_CS1_CMD_BUF10            (SFC_REG_BASE + 0x00E8)
#define SFC_CS1_CMD_BUF11            (SFC_REG_BASE + 0x00EC)
#define SFC_CS1_TYPE_BUF0             (SFC_REG_BASE + 0x00F0)
#define SFC_CS1_TYPE_BUF1             (SFC_REG_BASE + 0x00F4)
#define SFC_CS1_TYPE_BUF2             (SFC_REG_BASE + 0x00F8)

#define SFC_CS1_START_ADDR          (SFC_REG_BASE + 0x0200)
#define SFC_IEN                                 (SFC_REG_BASE + 0x0204)
#define SFC_INT_RAW                        (SFC_REG_BASE + 0x0208)
#define SFC_INT_STS                         (SFC_REG_BASE + 0x020C)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
/*SFC command config register */
typedef union sfc_cmd_cfg_tag
{
    struct sfc_cmd_cfg_map
    {
        volatile unsigned int cmd_set               :1;
        volatile unsigned int rdata_bit_mode        :2;       
        volatile unsigned int sts_ini_addr_sel      :2;
        volatile unsigned int reserved              :27;        
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_CFG_U;

typedef union sfc_soft_req_tag
{
    struct sfc_soft_req_map
    {
        volatile unsigned int soft_req              :1;
        volatile unsigned int reserved              :31;   
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_SOFT_REQ_U;

typedef union sfc_tbuf_clr_tag
{
    struct sfc_tbuf_clr_map
    {
        volatile unsigned int tbuf_clr              :1;
        volatile unsigned int reserved              :31;     
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_TBUF_CLR_U;

typedef union sfc_int_clr_tag
{
    struct sfc_int_clr_map
    {
        volatile unsigned int int_clr               :1;
        volatile unsigned int int_clr1               :1;
        volatile unsigned int reserved              :30;  
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_INT_CLR_U;

typedef union sfc_status_tag
{
    struct sfc_status_map
    {
        volatile unsigned int int_req               :1;
        volatile unsigned int idle                  :1;
        volatile unsigned int cur_state             :4;                
        volatile unsigned int reserved              :26;    
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_STATUS_U;

typedef union sfc_cs_timing_cfg_tag
{
    struct sfc_cs_timing_cfg_map
    {
        volatile unsigned int tslch_dly             :1;
        volatile unsigned int tshch_dly             :1;
        volatile unsigned int csh_clkout0_en        :1;        
        volatile unsigned int csh_clkout1_en        :1;
        volatile unsigned int tslsh_dly_num         :3;        
        volatile unsigned int cs_dly_sel            :2;  
        volatile unsigned int reserved              :22;        
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CS_TIMING_CFG_U;

typedef union sfc_rd_sample_cfg_tag
{
    struct sfc_rd_sample_cfg_map
    {
        volatile unsigned int t_sample_dly_r        :2;
        volatile unsigned int t_sample_dly_w        :2;
	 volatile unsigned int sample_rst            :1;        
	 volatile unsigned int bit_sample_vld_dlysel :3;        
	 volatile unsigned int byte_sample_vld_dlysel :3;         
        volatile unsigned int reserved              :21;      
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_RD_SAMPLE_CFG_U;

typedef union sfc_clk_cfg_tag
{
    struct sfc_clk_cfg_map
    {
        volatile unsigned int clk_div_mode          :2;
        volatile unsigned int clk_polarity          :1;
        volatile unsigned int clk_out_dly_inv       :1;
        volatile unsigned int clk_out_dly_sel       :4;
   	 volatile unsigned int clk_sample_dly_inv    :1; 
        volatile unsigned int clk_sample_dly_sel    :4;
        volatile unsigned int clk_out_en_dly_sel    :3;
        volatile unsigned int reserved              :16;        
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CLK_CFG_U;

typedef union sfc_cs_cfg_tag
{
    struct sfc_cs_cfg_map
    {
        volatile unsigned int soft_cs               :2;
        volatile unsigned int cs_position           :3;        
        volatile unsigned int reserved              :27;         
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CS_CFG_U;

typedef union sfc_endian_cfg_tag
{
    struct sfc_endian_cfg_map
    {
        volatile unsigned int ahb_endian            :2;
        volatile unsigned int spi_endian            :1;
        volatile unsigned int reserved              :29;     
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_ENDIAN_CFG_U;

typedef union sfc_io_dly_cfg_tag
{
    struct sfc_io_dly_cfg_map
    {
        volatile unsigned int data_oe_dly_sel       :2;
	 volatile unsigned int data_ie_dly_sel       :2;        
        volatile unsigned int data_ie_width_ctrl    :1;     
        volatile unsigned int do_dly_sel            :2;
        volatile unsigned int reserved              :25;                
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_IO_DLY_CFG_U;

typedef union sfc_wp_hld_tag
{
    struct sfc_wp_hld_map
    {
        volatile unsigned int wp_init               :1;
        volatile unsigned int hld_init              :1;        
        volatile unsigned int reserved              :30;   
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_WP_HLD_U;

typedef union sfc_cmd_buf0_tag
{
    struct sfc_cmd_buf0_map
    {      	
        volatile unsigned int cmd_buf0               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF0_U;

typedef union sfc_cmd_buf1_tag
{
    struct sfc_cmd_buf1_map
    {      	
        volatile unsigned int cmd_buf1               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF1_U;

typedef union sfc_cmd_buf2_tag
{
    struct sfc_cmd_buf2_map
    {      	
        volatile unsigned int cmd_buf2               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF2_U;

typedef union sfc_cmd_buf3_tag
{
    struct sfc_cmd_buf3_map
    { 
        volatile unsigned int cmd_buf3               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF3_U;

typedef union sfc_cmd_buf4_tag
{
    struct sfc_cmd_buf4_map
    {      	
        volatile unsigned int cmd_buf4               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF4_U;

typedef union sfc_cmd_buf5_tag
{
    struct sfc_cmd_buf5_map
    {      	
        volatile unsigned int cmd_buf5               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF5_U;

typedef union sfc_cmd_buf6_tag
{
    struct sfc_cmd_buf6_map
    {      	
        volatile unsigned int cmd_buf6               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF6_U;

typedef union sfc_cmd_buf7_tag
{
    struct sfc_cmd_buf7_map
    {      	
        volatile unsigned int cmd_buf7               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF7_U;

typedef union sfc_cmd_buf8_tag
{
    struct sfc_cmd_buf8_map
    {      	
        volatile unsigned int cmd_buf8               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF8_U;

typedef union sfc_cmd_buf9_tag
{
    struct sfc_cmd_buf9_map
    {      	
        volatile unsigned int cmd_buf9               :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF9_U;

typedef union sfc_cmd_buf10_tag
{
    struct sfc_cmd_buf10_map
    {      	
        volatile unsigned int cmd_buf10              :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF10_U;

typedef union sfc_cmd_buf11_tag
{
    struct sfc_cmd_buf11_map
    {      	
        volatile unsigned int cmd_buf11              :32;
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CMD_BUF11_U;

typedef union sfc_type_buf0_tag
{
    struct sfc_type_buf0_map
    {
        volatile unsigned int valid0                 :1;
        volatile unsigned int bit_mode0              :2;
        volatile unsigned int byte_num0              :2;
        volatile unsigned int operation_status0      :2;
        volatile unsigned int byte_send_mode0        :1;        
        volatile unsigned int valid1                 :1;
        volatile unsigned int bit_mode1              :2;
        volatile unsigned int byte_num1              :2;      
        volatile unsigned int operation_status1      :2;        
        volatile unsigned int byte_send_mode1        :1;
        volatile unsigned int valid2                 :1;        
        volatile unsigned int bit_mode2              :2;        
        volatile unsigned int byte_num2              :2;        
        volatile unsigned int operation_status2      :2;
        volatile unsigned int byte_send_mode2        :1;
        volatile unsigned int valid3                 :1;
        volatile unsigned int bit_mode3              :2;
        volatile unsigned int byte_num3              :2;
        volatile unsigned int operation_status3      :2;              
        volatile unsigned int byte_send_mode3        :1;     
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_TYPE_BUF0_U;

typedef union sfc_type_buf1_tag
{
    struct sfc_type_buf1_map
    {
        volatile unsigned int valid4                 :1;
        volatile unsigned int bit_mode4              :2;
        volatile unsigned int byte_num4              :2;
        volatile unsigned int operation_status4      :2;
        volatile unsigned int byte_send_mode4        :1;        
        volatile unsigned int valid5                 :1;
        volatile unsigned int bit_mode5              :2;
        volatile unsigned int byte_num5              :2;      
        volatile unsigned int operation_status5      :2;        
        volatile unsigned int byte_send_mode5        :1;
        volatile unsigned int valid6                 :1;        
        volatile unsigned int bit_mode6              :2;        
        volatile unsigned int byte_num6              :2;        
        volatile unsigned int operation_status6      :2;
        volatile unsigned int byte_send_mode6        :1;
        volatile unsigned int valid7                 :1;
        volatile unsigned int bit_mode7              :2;
        volatile unsigned int byte_num7              :2;
        volatile unsigned int operation_status7      :2;              
        volatile unsigned int byte_send_mode7        :1;     
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_TYPE_BUF1_U;

typedef union sfc_type_buf2_tag
{
    struct sfc_type_buf2_map
    {
        volatile unsigned int valid8                 :1;
        volatile unsigned int bit_mode8              :2;
        volatile unsigned int byte_num8              :2;
        volatile unsigned int operation_status8      :2;
        volatile unsigned int byte_send_mode8        :1;        
        volatile unsigned int valid9                 :1;
        volatile unsigned int bit_mode9              :2;
        volatile unsigned int byte_num9              :2;      
        volatile unsigned int operation_status9      :2;        
        volatile unsigned int byte_send_mode9        :1;
        volatile unsigned int valid10                :1;        
        volatile unsigned int bit_mode10             :2;        
        volatile unsigned int byte_num10             :2;        
        volatile unsigned int operation_status10     :2;
        volatile unsigned int byte_send_mode10       :1;
        volatile unsigned int valid11                :1;
        volatile unsigned int bit_mode11             :2;
        volatile unsigned int byte_num11             :2;
        volatile unsigned int operation_status11     :2;              
        volatile unsigned int byte_send_mode11       :1;    
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_TYPE_BUF2_U;

typedef union sfc_cs1_start_addr_tag
{
    struct sfc_cs1_start_addr_map
    {      	
        volatile unsigned int cs1_start_address    :8;
        volatile unsigned int reserved              :24; 
    } mBits ;
    volatile unsigned int dwValue ;
} SFC_CS1_START_ADDR_U;

/* SFC module control registers */
typedef struct sfc_reg_tag
{
    volatile SFC_CMD_CFG_U        cmd_cfg;
    volatile SFC_SOFT_REQ_U       soft_req;
    volatile SFC_TBUF_CLR_U       tbuf_clr;
    volatile SFC_INT_CLR_U        int_clr;
    volatile SFC_STATUS_U         status;
    volatile SFC_CS_TIMING_CFG_U  cs_timing_cfg;
    volatile SFC_RD_SAMPLE_CFG_U  rd_timing_cfg;
    volatile SFC_CLK_CFG_U        clk_cfg;
    volatile SFC_CS_CFG_U         cs_cfg;
    volatile SFC_ENDIAN_CFG_U     endian_cfg;
    volatile SFC_IO_DLY_CFG_U      io_dly_cfg;
    volatile SFC_WP_HLD_U         wp_hld_init;    
	volatile unsigned int		  reserved1;
	volatile unsigned int		  reserved2;
	volatile unsigned int		  reserved3;    
    volatile unsigned int		  reserved4;	
    volatile SFC_CMD_BUF0_U       cmd_buf0;
    volatile SFC_CMD_BUF1_U       cmd_buf1;
    volatile SFC_CMD_BUF2_U       cmd_buf2;
    volatile SFC_CMD_BUF3_U       cmd_buf3;
    volatile SFC_CMD_BUF4_U       cmd_buf4;
    volatile SFC_CMD_BUF5_U       cmd_buf5;
    volatile SFC_CMD_BUF6_U       cmd_buf6;
    volatile SFC_CMD_BUF7_U       cmd_buf7;
    volatile SFC_CMD_BUF8_U       cmd_buf8;
    volatile SFC_CMD_BUF9_U       cmd_buf9;
    volatile SFC_CMD_BUF10_U      cmd_buf10;
    volatile SFC_CMD_BUF11_U      cmd_buf11;
    volatile SFC_TYPE_BUF0_U      type_buf0;
    volatile SFC_TYPE_BUF1_U      type_buf1;
    volatile SFC_TYPE_BUF2_U      type_buf2;      
	volatile unsigned int		  reserved6;
    volatile SFC_CMD_CFG_U        cs1_cmd_cfg;	
	volatile unsigned int		  reserved7;	
    volatile SFC_TBUF_CLR_U       cs1_tbuf_clr;
	volatile unsigned int		  reserved8;
	volatile unsigned int		  reserved9;    
    volatile SFC_CS_TIMING_CFG_U  cs1_timing_cfg;
    volatile SFC_RD_SAMPLE_CFG_U  cs1_rd_timing_cfg;
    volatile SFC_CLK_CFG_U        cs1_clk_cfg;
	volatile unsigned int		  reserved10; 
	volatile unsigned int		  reserved11; 
	volatile unsigned int		  reserved12; 
	volatile unsigned int		  reserved13; 
	volatile unsigned int		  reserved14; 
    volatile SFC_ENDIAN_CFG_U     cs1_endian_cfg;
    volatile SFC_IO_DLY_CFG_U      cs1_io_dly_cfg;
    volatile SFC_WP_HLD_U         cs1_wp_hld_init;  	   
    volatile SFC_CMD_BUF0_U       cs1_cmd_buf0;
    volatile SFC_CMD_BUF1_U       cs1_cmd_buf1;
    volatile SFC_CMD_BUF2_U       cs1_cmd_buf2;
    volatile SFC_CMD_BUF3_U       cs1_cmd_buf3;
    volatile SFC_CMD_BUF4_U       cs1_cmd_buf4;
    volatile SFC_CMD_BUF5_U       cs1_cmd_buf5;
    volatile SFC_CMD_BUF6_U       cs1_cmd_buf6;
    volatile SFC_CMD_BUF7_U       cs1_cmd_buf7;
    volatile SFC_CMD_BUF8_U       cs1_cmd_buf8;
    volatile SFC_CMD_BUF9_U       cs1_cmd_buf9;
    volatile SFC_CMD_BUF10_U      cs1_cmd_buf10;
    volatile SFC_CMD_BUF11_U      cs1_cmd_buf11;
    volatile SFC_TYPE_BUF0_U      cs1_type_buf0;
    volatile SFC_TYPE_BUF1_U      cs1_type_buf1;
    volatile SFC_TYPE_BUF2_U      cs1_type_buf2; 	    
    volatile unsigned int          reserved16[65];
    volatile SFC_CS1_START_ADDR_U cs1_start_addr;
} SFC_REG_T;

#define SFC_REG_NUM                 (sizeof(SFC_REG_T)/sizeof(uint32))
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

