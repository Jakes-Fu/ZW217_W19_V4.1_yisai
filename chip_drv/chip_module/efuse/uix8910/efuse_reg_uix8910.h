/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _EFUSE_CTRL_H_
#define _EFUSE_CTRL_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'efuse_ctrl'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// EFUSE_CTRL_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_EFUSE_CTRL_BASE        0x0510E000
#else
#define REG_EFUSE_CTRL_BASE        0x5010E000
#endif

#define REG32 volatile unsigned int
typedef volatile struct
{
    REG32 Reserved_00000000[2];                 //0x00000000
    REG32                          EFUSE_ALL0_INDEX;             //0x00000008
    REG32                          EFUSE_MODE_CTRL;              //0x0000000C
    REG32 Reserved_00000010;                    //0x00000010
    REG32                          EFUSE_IP_VER_reg;             //0x00000014
    REG32                          EFUSE_CFG0;                   //0x00000018
    REG32 Reserved_0000001C[9];                 //0x0000001C
    REG32                          EFUSE_SEC_EN;                 //0x00000040
    REG32                          EFUSE_SEC_ERR_FLAG;           //0x00000044
    REG32                          EFUSE_SEC_FLAG_CLR;           //0x00000048
    REG32                          EFUSE_SEC_MAGIC_NUMBER;       //0x0000004C
    REG32                          EFUSE_FW_CFG;                 //0x00000050
    REG32                          EFUSE_PW_SWT;                 //0x00000054
    REG32                          ap_ca5_dbgen_reg;             //0x00000058
    REG32                          ap_ca5_niden_reg;             //0x0000005C
    REG32                          ap_ca5_spien_reg;             //0x00000060
    REG32                          ap_ca5_spnien_reg;            //0x00000064
    REG32                          ap_ca5_dap_deviceen_reg;      //0x00000068
    REG32                          riscv_jtag_disable_reg;       //0x0000006C
    REG32                          zsp_jtag_disable_reg;         //0x00000070
    REG32                          debug_host_rx_disable_reg;    //0x00000074
    REG32                          uart_1_rx_disable_reg;        //0x00000078
    REG32                          uart_2_rx_disable_reg;        //0x0000007C
    REG32                          uart_3_rx_disable_reg;        //0x00000080
    REG32                          uart_cp_rx_disable_reg;       //0x00000084
    REG32                          mbist_disable_reg;            //0x00000088
    REG32                          scan_disable_reg;             //0x0000008C
    REG32                          efuse_bist_en_reg;            //0x00000090
    REG32                          cp_ca5_dbgen_reg;             //0x00000094
    REG32                          cp_ca5_niden_reg;             //0x00000098
    REG32                          cp_ca5_spien_reg;             //0x0000009C
    REG32                          cp_ca5_spnien_reg;            //0x000000A0
    REG32                          cp_ca5_dcp_deviceen_reg;      //0x000000A4
    REG32                          efuse_block0_rw_ctrl_reg;     //0x000000A8
    REG32                          efuse_block1_rw_ctrl_reg;     //0x000000AC
    REG32                          efuse_block2_rw_ctrl_reg;     //0x000000B0
    REG32                          efuse_block3_rw_ctrl_reg;     //0x000000B4
    REG32                          efuse_block4_rw_ctrl_reg;     //0x000000B8
    REG32                          efuse_block5_rw_ctrl_reg;     //0x000000BC
    REG32                          efuse_block6_rw_ctrl_reg;     //0x000000C0
    REG32                          efuse_block7_rw_ctrl_reg;     //0x000000C4
    REG32                          efuse_block8_rw_ctrl_reg;     //0x000000C8
    REG32                          efuse_block9_rw_ctrl_reg;     //0x000000CC
    REG32                          efuse_block10_rw_ctrl_reg;    //0x000000D0
    REG32                          efuse_block11_rw_ctrl_reg;    //0x000000D4
    REG32                          efuse_block12_rw_ctrl_reg;    //0x000000D8
    REG32                          efuse_block13_rw_ctrl_reg;    //0x000000DC
    REG32                          efuse_block14_rw_ctrl_reg;    //0x000000E0
    REG32                          efuse_block15_rw_ctrl_reg;    //0x000000E4
    REG32                          efuse_block16_rw_ctrl_reg;    //0x000000E8
    REG32                          efuse_block17_rw_ctrl_reg;    //0x000000EC
    REG32                          efuse_block18_rw_ctrl_reg;    //0x000000F0
    REG32                          efuse_block19_rw_ctrl_reg;    //0x000000F4
    REG32                          efuse_block20_rw_ctrl_reg;    //0x000000F8
    REG32                          efuse_block21_rw_ctrl_reg;    //0x000000FC
    REG32                          efuse_block22_rw_ctrl_reg;    //0x00000100
    REG32                          efuse_block23_rw_ctrl_reg;    //0x00000104
    REG32                          efuse_block24_rw_ctrl_reg;    //0x00000108
    REG32                          efuse_block25_rw_ctrl_reg;    //0x0000010C
    REG32                          efuse_block26_rw_ctrl_reg;    //0x00000110
    REG32                          efuse_block27_rw_ctrl_reg;    //0x00000114
    REG32                          efuse_block28_rw_ctrl_reg;    //0x00000118
    REG32                          efuse_block29_rw_ctrl_reg;    //0x0000011C
    REG32                          efuse_block30_rw_ctrl_reg;    //0x00000120
    REG32                          efuse_block31_rw_ctrl_reg;    //0x00000124
    REG32                          por_read_done_reg;            //0x00000128
    REG32                          efuse_cfg_reg;                //0x0000012C
    REG32                          efuse_block_en_reg;           //0x00000130
    REG32                          por_read_data0_reg;           //0x00000134
    REG32 Reserved_00000138[2];                 //0x00000138
    REG32                          wcn_jtag_disable_reg;         //0x00000140
    REG32                          wcn_uart_disable_reg;         //0x00000144
    REG32                          rf_uart_disable_reg;          //0x00000148
    REG32 Reserved_0000014C[429];               //0x0000014C
    REG32                          EFUSE_MEM;                    //0x00000800
} HWP_EFUSE_CTRL_T;

#define hwp_efuseCtrl              ((HWP_EFUSE_CTRL_T*) REG_ACCESS_ADDRESS(REG_EFUSE_CTRL_BASE))


//EFUSE_ALL0_INDEX
typedef union {
    REG32 v;
    struct {
        REG32 efuse_all0_end_index : 16; // [15:0]
        REG32 efuse_all0_start_index : 16; // [31:16]
    } b;
} REG_EFUSE_CTRL_EFUSE_ALL0_INDEX_T;

//EFUSE_MODE_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 efuse_all0_check_start : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_MODE_CTRL_T;

//EFUSE_IP_VER_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_ip_ver : 16; // [15:0], read only
        REG32 efuse_type : 2; // [17:16], read only
        REG32 __31_18 : 14;
    } b;
} REG_EFUSE_CTRL_EFUSE_IP_VER_REG_T;

//EFUSE_CFG0
typedef union {
    REG32 v;
    struct {
        REG32 tpgm_time_cnt : 9; // [8:0]
        REG32 __15_9 : 7;
        REG32 efuse_strobe_low_width : 8; // [23:16]
        REG32 clk_efs_div : 8; // [31:24]
    } b;
} REG_EFUSE_CTRL_EFUSE_CFG0_T;

//EFUSE_SEC_EN
typedef union {
    REG32 v;
    struct {
        REG32 sec_vdd_en : 1; // [0]
        REG32 sec_auto_check_enable : 1; // [1]
        REG32 double_bit_en_sec : 1; // [2]
        REG32 sec_margin_rd_enable : 1; // [3]
        REG32 sec_lock_bit_wr_en : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_EFUSE_CTRL_EFUSE_SEC_EN_T;

//EFUSE_SEC_ERR_FLAG
typedef union {
    REG32 v;
    struct {
        REG32 block_auto_check_failed : 1; // [0], read only
        REG32 shadow_block_auto_check_failed : 1; // [1], read only
        REG32 __3_2 : 2;
        REG32 sec_word0_prot_flag : 1; // [4], read only
        REG32 sec_word1_prot_flag : 1; // [5], read only
        REG32 __7_6 : 2;
        REG32 sec_pg_en_wr_flag : 1; // [8], read only
        REG32 sec_vdd_on_rd_flag : 1; // [9], read only
        REG32 sec_block0_rd_flag : 1; // [10], read only
        REG32 sec_magnum_wr_flag : 1; // [11], read only
        REG32 sec_enk_err_flag : 1; // [12], read only
        REG32 sec_all0_check_flag : 1; // [13], read only
        REG32 __31_14 : 18;
    } b;
} REG_EFUSE_CTRL_EFUSE_SEC_ERR_FLAG_T;

//EFUSE_SEC_FLAG_CLR
typedef union {
    REG32 v;
    struct {
        REG32 sec_word0_err_clr : 1; // [0]
        REG32 sec_word1_err_clr : 1; // [1]
        REG32 __3_2 : 2;
        REG32 sec_word0_prot_clr : 1; // [4]
        REG32 sec_word1_prot_clr : 1; // [5]
        REG32 __7_6 : 2;
        REG32 sec_pg_en_wr_clr : 1; // [8]
        REG32 sec_vdd_on_rd_clr : 1; // [9]
        REG32 sec_block0_rd_clr : 1; // [10]
        REG32 sec_magnum_wr_clr : 1; // [11]
        REG32 sec_enk_err_clr : 1; // [12]
        REG32 sec_all0_check_clr : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_EFUSE_CTRL_EFUSE_SEC_FLAG_CLR_T;

//EFUSE_SEC_MAGIC_NUMBER
typedef union {
    REG32 v;
    struct {
        REG32 sec_efuse_magic_number : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_EFUSE_CTRL_EFUSE_SEC_MAGIC_NUMBER_T;

//EFUSE_FW_CFG
typedef union {
    REG32 v;
    struct {
        REG32 conf_prot : 1; // [0]
        REG32 access_prot : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_EFUSE_CTRL_EFUSE_FW_CFG_T;

//EFUSE_PW_SWT
typedef union {
    REG32 v;
    struct {
        REG32 efs_enk1_on : 1; // [0]
        REG32 efs_enk2_on : 1; // [1]
        REG32 ns_s_pg_en : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_EFUSE_CTRL_EFUSE_PW_SWT_T;

//ap_ca5_dbgen_reg
typedef union {
    REG32 v;
    struct {
        REG32 ap_ca5_dbgen : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_AP_CA5_DBGEN_REG_T;

//ap_ca5_niden_reg
typedef union {
    REG32 v;
    struct {
        REG32 ap_ca5_niden : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_AP_CA5_NIDEN_REG_T;

//ap_ca5_spien_reg
typedef union {
    REG32 v;
    struct {
        REG32 ap_ca5_spien : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_AP_CA5_SPIEN_REG_T;

//ap_ca5_spnien_reg
typedef union {
    REG32 v;
    struct {
        REG32 ap_ca5_spnien : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_AP_CA5_SPNIEN_REG_T;

//ap_ca5_dap_deviceen_reg
typedef union {
    REG32 v;
    struct {
        REG32 ap_ca5_dap_deviceen : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_AP_CA5_DAP_DEVICEEN_REG_T;

//riscv_jtag_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 riscv_jtag_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_RISCV_JTAG_DISABLE_REG_T;

//zsp_jtag_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 zsp_jtag_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_ZSP_JTAG_DISABLE_REG_T;

//debug_host_rx_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 debug_host_rx_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_DEBUG_HOST_RX_DISABLE_REG_T;

//uart_1_rx_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 uart_1_rx_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_UART_1_RX_DISABLE_REG_T;

//uart_2_rx_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 uart_2_rx_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_UART_2_RX_DISABLE_REG_T;

//uart_3_rx_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 uart_3_rx_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_UART_3_RX_DISABLE_REG_T;

//uart_cp_rx_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 uart_cp_rx_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_UART_CP_RX_DISABLE_REG_T;

//mbist_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 mbist_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_MBIST_DISABLE_REG_T;

//scan_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 scan_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_SCAN_DISABLE_REG_T;

//efuse_bist_en_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_bist_en : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BIST_EN_REG_T;

//cp_ca5_dbgen_reg
typedef union {
    REG32 v;
    struct {
        REG32 cp_ca5_dbgen : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_CP_CA5_DBGEN_REG_T;

//cp_ca5_niden_reg
typedef union {
    REG32 v;
    struct {
        REG32 cp_ca5_niden : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_CP_CA5_NIDEN_REG_T;

//cp_ca5_spien_reg
typedef union {
    REG32 v;
    struct {
        REG32 cp_ca5_spien : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_CP_CA5_SPIEN_REG_T;

//cp_ca5_spnien_reg
typedef union {
    REG32 v;
    struct {
        REG32 cp_ca5_spnien : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_CP_CA5_SPNIEN_REG_T;

//cp_ca5_dcp_deviceen_reg
typedef union {
    REG32 v;
    struct {
        REG32 cp_ca5_dcp_deviceen : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_CP_CA5_DCP_DEVICEEN_REG_T;

//efuse_block0_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block0_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK0_RW_CTRL_REG_T;

//efuse_block1_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block1_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK1_RW_CTRL_REG_T;

//efuse_block2_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block2_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK2_RW_CTRL_REG_T;

//efuse_block3_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block3_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK3_RW_CTRL_REG_T;

//efuse_block4_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block4_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK4_RW_CTRL_REG_T;

//efuse_block5_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block5_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK5_RW_CTRL_REG_T;

//efuse_block6_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block6_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK6_RW_CTRL_REG_T;

//efuse_block7_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block7_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK7_RW_CTRL_REG_T;

//efuse_block8_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block8_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK8_RW_CTRL_REG_T;

//efuse_block9_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block9_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK9_RW_CTRL_REG_T;

//efuse_block10_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block10_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK10_RW_CTRL_REG_T;

//efuse_block11_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block11_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK11_RW_CTRL_REG_T;

//efuse_block12_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block12_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK12_RW_CTRL_REG_T;

//efuse_block13_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block13_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK13_RW_CTRL_REG_T;

//efuse_block14_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block14_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK14_RW_CTRL_REG_T;

//efuse_block15_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block15_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK15_RW_CTRL_REG_T;

//efuse_block16_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block16_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK16_RW_CTRL_REG_T;

//efuse_block17_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block17_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK17_RW_CTRL_REG_T;

//efuse_block18_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block18_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK18_RW_CTRL_REG_T;

//efuse_block19_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block19_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK19_RW_CTRL_REG_T;

//efuse_block20_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block20_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK20_RW_CTRL_REG_T;

//efuse_block21_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block21_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK21_RW_CTRL_REG_T;

//efuse_block22_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block22_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK22_RW_CTRL_REG_T;

//efuse_block23_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block23_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK23_RW_CTRL_REG_T;

//efuse_block24_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block24_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK24_RW_CTRL_REG_T;

//efuse_block25_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block25_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK25_RW_CTRL_REG_T;

//efuse_block26_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block26_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK26_RW_CTRL_REG_T;

//efuse_block27_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block27_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK27_RW_CTRL_REG_T;

//efuse_block28_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block28_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK28_RW_CTRL_REG_T;

//efuse_block29_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block29_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK29_RW_CTRL_REG_T;

//efuse_block30_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block30_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK30_RW_CTRL_REG_T;

//efuse_block31_rw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 efuse_block31_rw_ctrl : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_EFUSE_BLOCK31_RW_CTRL_REG_T;

//por_read_done_reg
typedef union {
    REG32 v;
    struct {
        REG32 por_read_done : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_POR_READ_DONE_REG_T;

//efuse_cfg_reg
typedef union {
    REG32 v;
    struct {
        REG32 cp_ca5_dap_deviceen_status : 1; // [0], read only
        REG32 cp_ca5_spniden_status : 1; // [1], read only
        REG32 cp_ca5_spiden_status : 1; // [2], read only
        REG32 cp_ca5_niden_status : 1; // [3], read only
        REG32 cp_ca5_dbgen_status : 1; // [4], read only
        REG32 efuse_bist_en_status : 1; // [5], read only
        REG32 scan_disable_status : 1; // [6], read only
        REG32 mbist_disable_status : 1; // [7], read only
        REG32 uart_cp_rx_disable_status : 1; // [8], read only
        REG32 uart_3_rx_disable_status : 1; // [9], read only
        REG32 uart_2_rx_disable_status : 1; // [10], read only
        REG32 uart_1_rx_disable_status : 1; // [11], read only
        REG32 debug_host_rx_disable_status : 1; // [12], read only
        REG32 zsp_jtag_disable_status : 1; // [13], read only
        REG32 riscv_jtag_disable_status : 1; // [14], read only
        REG32 ap_ca5_dap_deviceen_status : 1; // [15], read only
        REG32 ap_ca5_spniden_status : 1; // [16], read only
        REG32 ap_ca5_spiden_status : 1; // [17], read only
        REG32 ap_ca5_niden_status : 1; // [18], read only
        REG32 ap_ca5_dbgen_status : 1; // [19], read only
        REG32 __31_20 : 12;
    } b;
} REG_EFUSE_CTRL_EFUSE_CFG_REG_T;

//wcn_jtag_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 wcn_jtag_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_WCN_JTAG_DISABLE_REG_T;

//wcn_uart_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 wcn_uart_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_WCN_UART_DISABLE_REG_T;

//rf_uart_disable_reg
typedef union {
    REG32 v;
    struct {
        REG32 rf_uart_disable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_EFUSE_CTRL_RF_UART_DISABLE_REG_T;


//EFUSE_ALL0_INDEX
#define EFUSE_CTRL_EFUSE_ALL0_START_INDEX(n) (((n)&0xFFFF)<<16)
#define EFUSE_CTRL_EFUSE_ALL0_END_INDEX(n) (((n)&0xFFFF)<<0)

//EFUSE_MODE_CTRL
#define EFUSE_CTRL_RESERVED_0(n)   (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_ALL0_CHECK_START (1<<0)

//EFUSE_IP_VER_reg
#define EFUSE_CTRL_RESERVED_0_X1(n) (((n)&0x3FFF)<<18)
#define EFUSE_CTRL_EFUSE_TYPE(n)   (((n)&3)<<16)
#define EFUSE_CTRL_EFUSE_IP_VER(n) (((n)&0xFFFF)<<0)

//EFUSE_CFG0
#define EFUSE_CTRL_CLK_EFS_DIV(n)  (((n)&0xFF)<<24)
#define EFUSE_CTRL_EFUSE_STROBE_LOW_WIDTH(n) (((n)&0xFF)<<16)
#define EFUSE_CTRL_RESERVED_0_X2(n) (((n)&0x7F)<<9)
#define EFUSE_CTRL_TPGM_TIME_CNT(n) (((n)&0x1FF)<<0)

//EFUSE_SEC_EN
#define EFUSE_CTRL_RESERVED_0_X3(n) (((n)&0x7FFFFFF)<<5)
#define EFUSE_CTRL_SEC_LOCK_BIT_WR_EN (1<<4)
#define EFUSE_CTRL_SEC_MARGIN_RD_ENABLE (1<<3)
#define EFUSE_CTRL_DOUBLE_BIT_EN_SEC (1<<2)
#define EFUSE_CTRL_SEC_AUTO_CHECK_ENABLE (1<<1)
#define EFUSE_CTRL_SEC_VDD_EN      (1<<0)

//EFUSE_SEC_ERR_FLAG
#define EFUSE_CTRL_RESERVED_0_X4(n) (((n)&0x3FFFF)<<14)
#define EFUSE_CTRL_SEC_ALL0_CHECK_FLAG (1<<13)
#define EFUSE_CTRL_SEC_ENK_ERR_FLAG (1<<12)
#define EFUSE_CTRL_SEC_MAGNUM_WR_FLAG (1<<11)
#define EFUSE_CTRL_SEC_BLOCK0_RD_FLAG (1<<10)
#define EFUSE_CTRL_SEC_VDD_ON_RD_FLAG (1<<9)
#define EFUSE_CTRL_SEC_PG_EN_WR_FLAG (1<<8)
#define EFUSE_CTRL_RESERVED_1(n)   (((n)&3)<<6)
#define EFUSE_CTRL_SEC_WORD1_PROT_FLAG (1<<5)
#define EFUSE_CTRL_SEC_WORD0_PROT_FLAG (1<<4)
#define EFUSE_CTRL_RESERVED_2(n)   (((n)&3)<<2)
#define EFUSE_CTRL_SHADOW_BLOCK_AUTO_CHECK_FAILED (1<<1)
#define EFUSE_CTRL_BLOCK_AUTO_CHECK_FAILED (1<<0)

//EFUSE_SEC_FLAG_CLR
//#define EFUSE_CTRL_RESERVED_0_X4(n) (((n)&0x3FFFF)<<14)
#define EFUSE_CTRL_SEC_ALL0_CHECK_CLR (1<<13)
#define EFUSE_CTRL_SEC_ENK_ERR_CLR (1<<12)
#define EFUSE_CTRL_SEC_MAGNUM_WR_CLR (1<<11)
#define EFUSE_CTRL_SEC_BLOCK0_RD_CLR (1<<10)
#define EFUSE_CTRL_SEC_VDD_ON_RD_CLR (1<<9)
#define EFUSE_CTRL_SEC_PG_EN_WR_CLR (1<<8)
//#define EFUSE_CTRL_RESERVED_1(n) (((n)&3)<<6)
#define EFUSE_CTRL_SEC_WORD1_PROT_CLR (1<<5)
#define EFUSE_CTRL_SEC_WORD0_PROT_CLR (1<<4)
//#define EFUSE_CTRL_RESERVED_2(n) (((n)&3)<<2)
#define EFUSE_CTRL_SEC_WORD1_ERR_CLR (1<<1)
#define EFUSE_CTRL_SEC_WORD0_ERR_CLR (1<<0)

//EFUSE_SEC_MAGIC_NUMBER
#define EFUSE_CTRL_RESERVED_0_X5(n) (((n)&0xFFFF)<<16)
#define EFUSE_CTRL_SEC_EFUSE_MAGIC_NUMBER(n) (((n)&0xFFFF)<<0)

//EFUSE_FW_CFG
#define EFUSE_CTRL_RESERVED_0_X6(n) (((n)&0x3FFFFFFF)<<2)
#define EFUSE_CTRL_ACCESS_PROT     (1<<1)
#define EFUSE_CTRL_CONF_PROT       (1<<0)

//EFUSE_PW_SWT
#define EFUSE_CTRL_RESERVED_0_X7(n) (((n)&0x1FFFFFFF)<<3)
#define EFUSE_CTRL_NS_S_PG_EN      (1<<2)
#define EFUSE_CTRL_EFS_ENK2_ON     (1<<1)
#define EFUSE_CTRL_EFS_ENK1_ON     (1<<0)

//ap_ca5_dbgen_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_AP_CA5_DBGEN    (1<<0)

//ap_ca5_niden_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_AP_CA5_NIDEN    (1<<0)

//ap_ca5_spien_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_AP_CA5_SPIEN    (1<<0)

//ap_ca5_spnien_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_AP_CA5_SPNIEN   (1<<0)

//ap_ca5_dap_deviceen_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_AP_CA5_DAP_DEVICEEN (1<<0)

//riscv_jtag_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_RISCV_JTAG_DISABLE (1<<0)

//zsp_jtag_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_ZSP_JTAG_DISABLE (1<<0)

//debug_host_rx_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_DEBUG_HOST_RX_DISABLE (1<<0)

//uart_1_rx_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_UART_1_RX_DISABLE (1<<0)

//uart_2_rx_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_UART_2_RX_DISABLE (1<<0)

//uart_3_rx_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_UART_3_RX_DISABLE (1<<0)

//uart_cp_rx_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_UART_CP_RX_DISABLE (1<<0)

//mbist_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_MBIST_DISABLE   (1<<0)

//scan_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_SCAN_DISABLE    (1<<0)

//efuse_bist_en_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BIST_EN   (1<<0)

//cp_ca5_dbgen_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_CP_CA5_DBGEN    (1<<0)

//cp_ca5_niden_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_CP_CA5_NIDEN    (1<<0)

//cp_ca5_spien_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_CP_CA5_SPIEN    (1<<0)

//cp_ca5_spnien_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_CP_CA5_SPNIEN   (1<<0)

//cp_ca5_dcp_deviceen_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_CP_CA5_DCP_DEVICEEN (1<<0)

//efuse_block0_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK0_RW_CTRL (1<<0)

//efuse_block1_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK1_RW_CTRL (1<<0)

//efuse_block2_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK2_RW_CTRL (1<<0)

//efuse_block3_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK3_RW_CTRL (1<<0)

//efuse_block4_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK4_RW_CTRL (1<<0)

//efuse_block5_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK5_RW_CTRL (1<<0)

//efuse_block6_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK6_RW_CTRL (1<<0)

//efuse_block7_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK7_RW_CTRL (1<<0)

//efuse_block8_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK8_RW_CTRL (1<<0)

//efuse_block9_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK9_RW_CTRL (1<<0)

//efuse_block10_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK10_RW_CTRL (1<<0)

//efuse_block11_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK11_RW_CTRL (1<<0)

//efuse_block12_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK12_RW_CTRL (1<<0)

//efuse_block13_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK13_RW_CTRL (1<<0)

//efuse_block14_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK14_RW_CTRL (1<<0)

//efuse_block15_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK15_RW_CTRL (1<<0)

//efuse_block16_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK16_RW_CTRL (1<<0)

//efuse_block17_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK17_RW_CTRL (1<<0)

//efuse_block18_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK18_RW_CTRL (1<<0)

//efuse_block19_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK19_RW_CTRL (1<<0)

//efuse_block20_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK20_RW_CTRL (1<<0)

//efuse_block21_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK21_RW_CTRL (1<<0)

//efuse_block22_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK22_RW_CTRL (1<<0)

//efuse_block23_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK23_RW_CTRL (1<<0)

//efuse_block24_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK24_RW_CTRL (1<<0)

//efuse_block25_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK25_RW_CTRL (1<<0)

//efuse_block26_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK26_RW_CTRL (1<<0)

//efuse_block27_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK27_RW_CTRL (1<<0)

//efuse_block28_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK28_RW_CTRL (1<<0)

//efuse_block29_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK29_RW_CTRL (1<<0)

//efuse_block30_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK30_RW_CTRL (1<<0)

//efuse_block31_rw_ctrl_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_EFUSE_BLOCK31_RW_CTRL (1<<0)

//por_read_done_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_POR_READ_DONE   (1<<0)

//efuse_cfg_reg
#define EFUSE_CTRL_RESERVED_0_X8(n) (((n)&0xFFF)<<20)
#define EFUSE_CTRL_AP_CA5_DBGEN_STATUS (1<<19)
#define EFUSE_CTRL_AP_CA5_NIDEN_STATUS (1<<18)
#define EFUSE_CTRL_AP_CA5_SPIDEN_STATUS (1<<17)
#define EFUSE_CTRL_AP_CA5_SPNIDEN_STATUS (1<<16)
#define EFUSE_CTRL_AP_CA5_DAP_DEVICEEN_STATUS (1<<15)
#define EFUSE_CTRL_RISCV_JTAG_DISABLE_STATUS (1<<14)
#define EFUSE_CTRL_ZSP_JTAG_DISABLE_STATUS (1<<13)
#define EFUSE_CTRL_DEBUG_HOST_RX_DISABLE_STATUS (1<<12)
#define EFUSE_CTRL_UART_1_RX_DISABLE_STATUS (1<<11)
#define EFUSE_CTRL_UART_2_RX_DISABLE_STATUS (1<<10)
#define EFUSE_CTRL_UART_3_RX_DISABLE_STATUS (1<<9)
#define EFUSE_CTRL_UART_CP_RX_DISABLE_STATUS (1<<8)
#define EFUSE_CTRL_MBIST_DISABLE_STATUS (1<<7)
#define EFUSE_CTRL_SCAN_DISABLE_STATUS (1<<6)
#define EFUSE_CTRL_EFUSE_BIST_EN_STATUS (1<<5)
#define EFUSE_CTRL_CP_CA5_DBGEN_STATUS (1<<4)
#define EFUSE_CTRL_CP_CA5_NIDEN_STATUS (1<<3)
#define EFUSE_CTRL_CP_CA5_SPIDEN_STATUS (1<<2)
#define EFUSE_CTRL_CP_CA5_SPNIDEN_STATUS (1<<1)
#define EFUSE_CTRL_CP_CA5_DAP_DEVICEEN_STATUS (1<<0)

//efuse_block_en_reg
#define EFUSE_CTRL_EFUSE_BLOCK_EN(n) (((n)&0xFFFFFFFF)<<0)

//por_read_data0_reg
#define EFUSE_CTRL_POR_READ_DATA0(n) (((n)&0xFFFFFFFF)<<0)

//wcn_jtag_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_WCN_JTAG_DISABLE (1<<0)

//wcn_uart_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_WCN_UART_DISABLE (1<<0)

//rf_uart_disable_reg
//#define EFUSE_CTRL_RESERVED_0(n) (((n)&0x7FFFFFFF)<<1)
#define EFUSE_CTRL_RF_UART_DISABLE (1<<0)

//EFUSE_MEM
#define EFUSE_CTRL_EFUSE_DATA(n)   (((n)&0xFFFFFFFF)<<0)





#endif

