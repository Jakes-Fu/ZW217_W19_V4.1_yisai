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


#ifndef _DMC400_ASM_ARM_H_
#define _DMC400_ASM_ARM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !



//==============================================================================
// dmc400
//------------------------------------------------------------------------------
///
//==============================================================================
#define REG_DMC_CTRL_BASE          0x08803000

#define REG_DMC_CTRL_MEMC_STATUS      0x00000000
#define REG_DMC_CTRL_MEMC_CONFIG      0x00000004
#define REG_DMC_CTRL_MEMC_CMD         0x00000008
#define REG_DMC_CTRL_ADDRESS_CONTROL    0x00000010
#define REG_DMC_CTRL_DECODE_CONTROL    0x00000014
#define REG_DMC_CTRL_FORMAT_CONTROL    0x00000018
#define REG_DMC_CTRL_LOW_POWER_CONTROL    0x00000020
#define REG_DMC_CTRL_TURNAROUND_PRIORITY    0x00000030
#define REG_DMC_CTRL_HIT_PRIORITY     0x00000034
#define REG_DMC_CTRL_QOS0_CONTROL     0x00000038
#define REG_DMC_CTRL_QOS1_CONTROL     0x0000003C
#define REG_DMC_CTRL_QOS2_CONTROL     0x00000040
#define REG_DMC_CTRL_QOS3_CONTROL     0x00000044
#define REG_DMC_CTRL_QOS4_CONTROL     0x00000048
#define REG_DMC_CTRL_QOS5_CONTROL     0x0000004C
#define REG_DMC_CTRL_QOS6_CONTROL     0x00000050
#define REG_DMC_CTRL_QOS7_CONTROL     0x00000054
#define REG_DMC_CTRL_QOS8_CONTROL     0x00000058
#define REG_DMC_CTRL_QOS9_CONTROL     0x0000005C
#define REG_DMC_CTRL_QOS10_CONTROL    0x00000060
#define REG_DMC_CTRL_QOS11_CONTROL    0x00000064
#define REG_DMC_CTRL_QOS12_CONTROL    0x00000068
#define REG_DMC_CTRL_QOS13_CONTROL    0x00000070
#define REG_DMC_CTRL_QOS14_CONTROL    0x00000074
#define REG_DMC_CTRL_QOS15_CONTROL    0x00000078
#define REG_DMC_CTRL_TIMEOUT_CONTROL    0x0000007C
#define REG_DMC_CTRL_QUEUE_CONTROL    0x00000080
#define REG_DMC_CTRL_WRITE_PRIORITY_CONTROL    0x00000088
#define REG_DMC_CTRL_WRITE_PRIORITY_CONTROL2    0x0000008C
#define REG_DMC_CTRL_READ_PRIORITY_CONTROL    0x00000090
#define REG_DMC_CTRL_READ_PRIORITY_CONTROL2    0x00000094
#define REG_DMC_CTRL_ACCESS_ADDRESS_MATCH    0x00000098
#define REG_DMC_CTRL_ACCESS_ADDRESS_MASK    0x000000A0
#define REG_DMC_CTRL_CHANNEL_STATUS    0x00000100
#define REG_DMC_CTRL_DIRECT_CMD       0x00000108
#define REG_DMC_CTRL_MR_DATA          0x00000110
#define REG_DMC_CTRL_REFRESH_CONTROL    0x00000120
#define REG_DMC_CTRL_T_REFI           0x00000200
#define REG_DMC_CTRL_T_RFC            0x00000204
#define REG_DMC_CTRL_T_MRR            0x00000208
#define REG_DMC_CTRL_T_MRW            0x0000020C
#define REG_DMC_CTRL_T_RCD            0x00000218
#define REG_DMC_CTRL_T_RAS            0x0000021C
#define REG_DMC_CTRL_T_RP             0x00000220
#define REG_DMC_CTRL_T_RPALL          0x00000224
#define REG_DMC_CTRL_T_RRD            0x00000228
#define REG_DMC_CTRL_T_FAW            0x0000022C
#define REG_DMC_CTRL_READ_LATENCY     0x00000230
#define REG_DMC_CTRL_T_RTR            0x00000234
#define REG_DMC_CTRL_T_RTW            0x00000238
#define REG_DMC_CTRL_T_RTP            0x0000023C
#define REG_DMC_CTRL_WRITE_LATENCY    0x00000240
#define REG_DMC_CTRL_T_WR             0x00000244
#define REG_DMC_CTRL_T_WTR            0x00000248
#define REG_DMC_CTRL_T_WTW            0x0000024C
#define REG_DMC_CTRL_T_ECKD           0x00000250
#define REG_DMC_CTRL_T_XCKD           0x00000254
#define REG_DMC_CTRL_T_EP             0x00000258
#define REG_DMC_CTRL_T_XP             0x0000025C
#define REG_DMC_CTRL_T_ESR            0x00000260
#define REG_DMC_CTRL_T_XSR            0x00000264
#define REG_DMC_CTRL_T_SRCKD          0x00000268
#define REG_DMC_CTRL_T_CKSRD          0x0000026C
#define REG_DMC_CTRL_T_RDDATA_EN      0x00000300
#define REG_DMC_CTRL_T_PHYWRLAT       0x00000304
#define REG_DMC_CTRL_RDLVL_CONTROL    0x00000308
#define REG_DMC_CTRL_RDLVL_MRS        0x0000030C
#define REG_DMC_CTRL_RDLVL_DIRECT     0x00000310
#define REG_DMC_CTRL_T_RDLVL_EN       0x00000318
#define REG_DMC_CTRL_T_RDLVL_RR       0x0000031C
#define REG_DMC_CTRL_WRLVL_CONTROL    0x00000328
#define REG_DMC_CTRL_WRLVL_MRS        0x0000032C
#define REG_DMC_CTRL_WRLVL_DIRECT     0x00000330
#define REG_DMC_CTRL_T_WRLVL_EN       0x00000338
#define REG_DMC_CTRL_T_WRLVL_WW       0x0000033C
#define REG_DMC_CTRL_PHY_POWER_CONTROL    0x00000348
#define REG_DMC_CTRL_PHY_UPDATE_CONTROL    0x00000350
#define REG_DMC_CTRL_USER_STATUS      0x00000400
#define REG_DMC_CTRL_USER_CONFIG0     0x00000404
#define REG_DMC_CTRL_USER_CONFIG1     0x00000408
#define REG_DMC_CTRL_INTEG_CFG        0x00000E00
#define REG_DMC_CTRL_INTEG_OUTPUTS    0x00000E08
#define REG_DMC_CTRL_PERIPH_ID_0      0x00000FE0
#define REG_DMC_CTRL_PERIPH_ID_1      0x00000FE4
#define REG_DMC_CTRL_PERIPH_ID_2      0x00000FE8
#define REG_DMC_CTRL_PERIPH_ID_3      0x00000FEC
#define REG_DMC_CTRL_COMPONENT_ID_0    0x00000FF0
#define REG_DMC_CTRL_COMPONENT_ID_1    0x00000FF4
#define REG_DMC_CTRL_COMPONENT_ID_2    0x00000FF8
#define REG_DMC_CTRL_COMPONENT_ID_3    0x00000FFC

//memc_status
#define DMC400_MEMC_STATUS_CONFIG  (0<<0)
#define DMC400_MEMC_STATUS_LOW_POWER (1<<0)
#define DMC400_MEMC_STATUS_PAUSED  (2<<0)
#define DMC400_MEMC_STATUS_READY   (3<<0)

//memc_config
#define DMC400_SYSTEM_INTERFACES_CFG_1_SYSTEM_INTERFACE (0<<0)
#define DMC400_SYSTEM_INTERFACES_CFG_2_SYSTEM_INTERFACE (1<<0)
#define DMC400_SYSTEM_INTERFACES_CFG_4_SYSTEM_INTERFACE (3<<0)
#define DMC400_MEMORY_INTERFACES_CFG_1_MEMORY_INTERFACE (0<<4)
#define DMC400_MEMORY_INTERFACES_CFG_2_MEMORY_INTERFACE (1<<4)
#define DMC400_MEMORY_DATA_WIDTH_CFG_32BIT_PHY_IF (1<<8)
#define DMC400_MEMORY_DATA_WIDTH_CFG_64BIT_PHY_IF (2<<8)
#define DMC400_MEMORY_DATA_WIDTH_CFG_128BIT_PHY_IF (3<<8)
#define DMC400_MEMORY_CHIP_SELECTS_CFG_1_CHIP_SEL (0<<12)
#define DMC400_MEMORY_CHIP_SELECTS_CFG_2_CHIP_SEL (1<<12)
#define DMC400_READ_QUEUE_DEPTH_CFG_16_ENTRY (0<<16)
#define DMC400_READ_QUEUE_DEPTH_CFG_32_ENTRY (1<<16)
#define DMC400_READ_QUEUE_DEPTH_CFG_64_ENTRY (3<<16)
#define DMC400_READ_QUEUE_DEPTH_CFG_128_ENTRY (7<<16)
#define DMC400_WRITE_QUEUE_DEPTH_CFG_16_ENTRY (0<<20)
#define DMC400_WRITE_QUEUE_DEPTH_CFG_32_ENTRY (1<<20)
#define DMC400_WRITE_QUEUE_DEPTH_CFG_64_ENTRY (3<<20)
#define DMC400_WRITE_QUEUE_DEPTH_CFG_128_ENTRY (7<<20)
#define DMC400_MAX_BURST_LENGTH_CFG_2_DMC_CYCLE (1<<24)
#define DMC400_MAX_BURST_LENGTH_CFG_4_DMC_CYCLE (2<<24)
#define DMC400_MAX_BURST_LENGTH_CFG_8_DMC_CYCLE (3<<24)
#define DMC400_MEMORY_ECC_CFG_FALSE (0<<28)
#define DMC400_MEMORY_ECC_CFG_TRUE (1<<28)

//memc_cmd
#define DMC400_MEMC_CMD_CONFIG     (0<<0)
#define DMC400_MEMC_CMD_SLEEP      (1<<0)
#define DMC400_MEMC_CMD_PAUSE      (2<<0)
#define DMC400_MEMC_CMD_GO         (3<<0)
#define DMC400_MEMC_CMD_INVALIDATE (4<<0)

//address_control
#define DMC400_COLUMN_BITS_8_COL_BITS (0<<0)
#define DMC400_COLUMN_BITS_9_COL_BITS (1<<0)
#define DMC400_COLUMN_BITS_10_COL_BITS (2<<0)
#define DMC400_COLUMN_BITS_11_COL_BITS (3<<0)
#define DMC400_COLUMN_BITS_12_COL_BITS (4<<0)
#define DMC400_ROW_BITS_13_ROW_BITS (2<<8)
#define DMC400_ROW_BITS_14_ROW_BITS (3<<8)
#define DMC400_ROW_BITS_15_ROW_BITS (4<<8)
#define DMC400_ROW_BITS_16_ROW_BITS (5<<8)
#define DMC400_BANK_BITS_2_BANK_BITS_4BK (2<<16)
#define DMC400_BANK_BITS_3_BANK_BITS_8BK (3<<16)
#define DMC400_CHIP_BITS_0_CHIP_BITS_1CS (0<<24)
#define DMC400_CHIP_BITS_1_CHIP_BITS_2CS (1<<24)
#define DMC400_CHANNEL_BITS_0_CHANNEL_BITS_1MEMIF (0<<28)
#define DMC400_CHANNEL_BITS_1_CHANNEL_BITS_2MEMIF (1<<28)

//decode_control
#define DMC400_ADDR_DECODE_CHANNEL_CHIP_ROW_BANK_COL (0<<0)
#define DMC400_ADDR_DECODE_ROW_CHANNEL_CHIP_BANK_COL (1<<0)
#define DMC400_ADDR_DECODE_CHIP_BANK_ROW_CHANNEL_COL (2<<0)
#define DMC400_ADDR_DECODE_ROW_CHIP_BANK_CHANNEL_COL (3<<0)
#define DMC400_STRIP_DECODE_PAGE_ADDR_13_12 (0<<4)
#define DMC400_STRIP_DECODE_PAGE_ADDR_12_11 (1<<4)
#define DMC400_STRIP_DECODE_PAGE_ADDR_11_10 (2<<4)
#define DMC400_STRIP_DECODE_PAGE_ADDR_10_9 (3<<4)
#define DMC400_STRIP_DECODE_PAGE_ADDR_9_8 (4<<4)
#define DMC400_STRIP_DECODE_PAGE_ADDR_8_7 (5<<4)
#define DMC400_STRIP_DECODE_PAGE_ADDR_7_6 (6<<4)
#define DMC400_STRIP_DECODE_PAGE_ADDR_6_5 (7<<4)

//format_control
#define DMC400_MEM_WIDTH_PHY_WIDTH_32_X16_DDR (1<<0)
#define DMC400_MEM_WIDTH_PHY_WIDTH_64_X32_DDR (2<<0)
#define DMC400_MEM_WIDTH_PHY_WIDTH_128_X64_DDR (3<<0)
#define DMC400_MEM_BURST_MEM_BURST_2_DDR_BL4 (1<<8)
#define DMC400_MEM_BURST_MEM_BURST_4_DDR_BL8 (2<<8)
#define DMC400_MEM_BURST_MEM_BURST_8_DDR_BL16 (3<<8)
#define DMC400_ACC_GRANU_ACC_GRANU_1_DDR_2N (0<<24)
#define DMC400_ACC_GRANU_ACC_GRANU_2_DDR_4N (1<<24)
#define DMC400_ACC_GRANU_ACC_GRANU_4_DDR_8N (2<<24)
#define DMC400_ACC_GRANU_ACC_GRANU_8_DDR_16N (3<<24)
#define DMC400_ALIGN_BOUNDARY_ALIGN_BOUNDARY_1_COL_1BIT (0<<28)
#define DMC400_ALIGN_BOUNDARY_ALIGN_BOUNDARY_2_COL_2BIT (1<<28)
#define DMC400_ALIGN_BOUNDARY_ALIGN_BOUNDARY_4_COL_3BIT (2<<28)
#define DMC400_ALIGN_BOUNDARY_ALIGN_BOUNDARY_8_COL_4BIT (3<<28)

//low_power_control
#define DMC400_STOP_MEM_CLOCK_IDLE_DISABLE (0<<0)
#define DMC400_STOP_MEM_CLOCK_IDLE_ENABLE (1<<0)
#define DMC400_STOP_MEM_CLOCK_SREF_DISABLE (0<<1)
#define DMC400_STOP_MEM_CLOCK_SREF_ENABLE (1<<1)
#define DMC400_AUTO_POWER_DOWN_DISABLE (0<<2)
#define DMC400_AUTO_POWER_DOWN_ENABLE (1<<2)
#define DMC400_AUTO_SELF_REFRESH_DISABLE (0<<3)
#define DMC400_AUTO_SELF_REFRESH_ENABLE (1<<3)
#define DMC400_ASR_PERIOD(n)       (((n)&15)<<4)

//turnaround_priority
#define DMC400_TURNAROUND_PRIORITY(n) (((n)&15)<<0)
#define DMC400_TURNAROUND_LIMIT(n) (((n)&15)<<4)

//hit_priority
#define DMC400_HIT_PRIORITY(n)     (((n)&15)<<0)
#define DMC400_HIT_LIMIT(n)        (((n)&15)<<4)

//qos0_control
#define DMC400_QOS0_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS0_TIMEOUT(n)     (((n)&15)<<8)

//qos1_control
#define DMC400_QOS1_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS1_TIMEOUT(n)     (((n)&15)<<8)

//qos2_control
#define DMC400_QOS2_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS2_TIMEOUT(n)     (((n)&15)<<8)

//qos3_control
#define DMC400_QOS3_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS3_TIMEOUT(n)     (((n)&15)<<8)

//qos4_control
#define DMC400_QOS4_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS4_TIMEOUT(n)     (((n)&15)<<8)

//qos5_control
#define DMC400_QOS5_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS5_TIMEOUT(n)     (((n)&15)<<8)

//qos6_control
#define DMC400_QOS6_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS6_TIMEOUT(n)     (((n)&15)<<8)

//qos7_control
#define DMC400_QOS7_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS7_TIMEOUT(n)     (((n)&15)<<8)

//qos8_control
#define DMC400_QOS8_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS8_TIMEOUT(n)     (((n)&15)<<8)

//qos9_control
#define DMC400_QOS9_PRIORITY(n)    (((n)&15)<<0)
#define DMC400_QOS9_TIMEOUT(n)     (((n)&15)<<8)

//qos10_control
#define DMC400_QOS10_PRIORITY(n)   (((n)&15)<<0)
#define DMC400_QOS10_TIMEOUT(n)    (((n)&15)<<8)

//qos11_control
#define DMC400_QOS11_PRIORITY(n)   (((n)&15)<<0)
#define DMC400_QOS11_TIMEOUT(n)    (((n)&15)<<8)

//qos12_control
#define DMC400_QOS12_PRIORITY(n)   (((n)&15)<<0)
#define DMC400_QOS12_TIMEOUT(n)    (((n)&15)<<8)

//qos13_control
#define DMC400_QOS13_PRIORITY(n)   (((n)&15)<<0)
#define DMC400_QOS13_TIMEOUT(n)    (((n)&15)<<8)

//qos14_control
#define DMC400_QOS14_PRIORITY(n)   (((n)&15)<<0)
#define DMC400_QOS14_TIMEOUT(n)    (((n)&15)<<8)

//qos15_control
#define DMC400_QOS15_PRIORITY(n)   (((n)&15)<<0)
#define DMC400_QOS15_TIMEOUT(n)    (((n)&15)<<8)

//timeout_control
#define DMC400_TIMEOUT_PRESCALAR_8_CLK (0<<0)
#define DMC400_TIMEOUT_PRESCALAR_16_CLK (1<<0)
#define DMC400_TIMEOUT_PRESCALAR_32_CLK (2<<0)
#define DMC400_TIMEOUT_PRESCALAR_64_CLK (3<<0)

//queue_control
#define DMC400_S0_RESERVE(n)       (((n)&15)<<0)

//write_priority_control
#define DMC400_WRITE_THRESHOLD_EN_DISABLE (0<<0)
#define DMC400_WRITE_THRESHOLD_EN_ENABLE (1<<0)
#define DMC400_WRITE_FILL_PRIORITY_1_16THS(n) (((n)&15)<<4)
#define DMC400_WRITE_FILL_PRIORITY_2_16THS(n) (((n)&15)<<8)
#define DMC400_WRITE_FILL_PRIORITY_3_16THS(n) (((n)&15)<<12)
#define DMC400_WRITE_FILL_PRIORITY_4_16THS(n) (((n)&15)<<16)
#define DMC400_WRITE_FILL_PRIORITY_5_16THS(n) (((n)&15)<<20)
#define DMC400_WRITE_FILL_PRIORITY_6_16THS(n) (((n)&15)<<24)
#define DMC400_WRITE_FILL_PRIORITY_7_16THS(n) (((n)&15)<<28)

//write_priority_control2
#define DMC400_WRITE_FILL_PRIORITY_8_16THS(n) (((n)&15)<<0)
#define DMC400_WRITE_FILL_PRIORITY_9_16THS(n) (((n)&15)<<4)
#define DMC400_WRITE_FILL_PRIORITY_10_16THS(n) (((n)&15)<<8)
#define DMC400_WRITE_FILL_PRIORITY_11_16THS(n) (((n)&15)<<12)
#define DMC400_WRITE_FILL_PRIORITY_12_16THS(n) (((n)&15)<<16)
#define DMC400_WRITE_FILL_PRIORITY_13_16THS(n) (((n)&15)<<20)
#define DMC400_WRITE_FILL_PRIORITY_14_16THS(n) (((n)&15)<<24)
#define DMC400_WRITE_FILL_PRIORITY_15_16THS(n) (((n)&15)<<28)

//read_priority_control
#define DMC400_READ_ESCALATION_DISABLE (0<<0)
#define DMC400_READ_ESCALATION_ENABLE (1<<0)
#define DMC400_READ_IN_BURST_PRIORITISATION_DISABLE (0<<1)
#define DMC400_READ_IN_BURST_PRIORITISATION_ENABLE (1<<1)
#define DMC400_READ_FILL_PRIORITY_1_16THS(n) (((n)&15)<<4)
#define DMC400_READ_FILL_PRIORITY_2_16THS(n) (((n)&15)<<8)
#define DMC400_READ_FILL_PRIORITY_3_16THS(n) (((n)&15)<<12)
#define DMC400_READ_FILL_PRIORITY_4_16THS(n) (((n)&15)<<16)
#define DMC400_READ_FILL_PRIORITY_5_16THS(n) (((n)&15)<<20)
#define DMC400_READ_FILL_PRIORITY_6_16THS(n) (((n)&15)<<24)
#define DMC400_READ_FILL_PRIORITY_7_16THS(n) (((n)&15)<<28)

//read_priority_control2
#define DMC400_READ_FILL_PRIORITY_8_16THS(n) (((n)&15)<<0)
#define DMC400_READ_FILL_PRIORITY_9_16THS(n) (((n)&15)<<4)
#define DMC400_READ_FILL_PRIORITY_10_16THS(n) (((n)&15)<<8)
#define DMC400_READ_FILL_PRIORITY_11_16THS(n) (((n)&15)<<12)
#define DMC400_READ_FILL_PRIORITY_12_16THS(n) (((n)&15)<<16)
#define DMC400_READ_FILL_PRIORITY_13_16THS(n) (((n)&15)<<20)
#define DMC400_READ_FILL_PRIORITY_14_16THS(n) (((n)&15)<<24)
#define DMC400_READ_FILL_PRIORITY_15_16THS(n) (((n)&15)<<28)

//access_address_match
#define DMC400_ACCESS_ADDRESS_MATCH(n) (((n)&0xFFFFF)<<12)

//access_address_mask
#define DMC400_ACCESS_ADDRESS_MASK(n) (((n)&0xFFFFF)<<12)

//channel_status
#define DMC400_M0_STATE_STANDBY    (0<<0)
#define DMC400_M0_STATE_DPD        (1<<0)
#define DMC400_M0_STATE_IDLE       (2<<0)
#define DMC400_M0_STATE_SELF_REFRESH (3<<0)
#define DMC400_M0_STATE_READING    (4<<0)
#define DMC400_M0_STATE_POWER_DOWN (5<<0)
#define DMC400_M0_STATE_WRITING    (6<<0)
#define DMC400_M1_STATE_STANDBY    (0<<4)
#define DMC400_M1_STATE_DPD        (1<<4)
#define DMC400_M1_STATE_IDLE       (2<<4)
#define DMC400_M1_STATE_SELF_REFRESH (3<<4)
#define DMC400_M1_STATE_READING    (4<<4)
#define DMC400_M1_STATE_POWER_DOWN (5<<4)
#define DMC400_M1_STATE_WRITING    (6<<4)

//direct_cmd
#define DMC400_DIRECT_ADDR(n)      (((n)&0xFFFF)<<0)
#define DMC400_DIRECT_BA(n)        (((n)&7)<<16)
#define DMC400_CHIP_ADDR_CHIP_0    (0<<20)
#define DMC400_CHIP_ADDR_CHIP_1    (1<<20)
#define DMC400_CHANNEL_ADDR_CHANNEL_0 (0<<24)
#define DMC400_CHANNEL_ADDR_CHANNEL_1 (1<<24)
#define DMC400_DIRECT_CMD_NOP      (0<<28)
#define DMC400_DIRECT_CMD_MRS      (1<<28)
#define DMC400_DIRECT_CMD_PRECHARGEALL (2<<28)
#define DMC400_DIRECT_CMD_AUTOREFRESH (3<<28)
#define DMC400_DIRECT_CMD_SELFREFRESH_ENTRY (4<<28)
#define DMC400_DIRECT_CMD_ZQC      (5<<28)
#define DMC400_DIRECT_CMD_MRR      (6<<28)
#define DMC400_DIRECT_CMD_POWERDOWN_ENTRY (7<<28)
#define DMC400_DIRECT_CMD_DEEP_POWERDOWN_ENTRY (8<<28)

//mr_data
#define DMC400_MR_DATA(n)          (((n)&0xFF)<<0)

//refresh_control
#define DMC400_PER_BANK_REFRESH_ALL_BANK_AUTOREFRESH (0<<0)
#define DMC400_PER_BANK_REFRESH_PRE_BANK_AUTOREFRESH (1<<0)

//t_refi
#define DMC400_T_REFI(n)           (((n)&0x7FF)<<0)

//t_rfc
#define DMC400_T_RFC(n)            (((n)&0x1FF)<<0)
#define DMC400_T_RFCAB(n)          (((n)&0x1FF)<<16)

//t_mrr
#define DMC400_T_MRR(n)            (((n)&7)<<0)

//t_mrw
#define DMC400_T_MRW(n)            (((n)&0x7F)<<0)

//t_rcd
#define DMC400_T_RCD(n)            (((n)&15)<<0)

//t_ras
#define DMC400_T_RAS(n)            (((n)&0x3F)<<0)

//t_rp
#define DMC400_T_RP(n)             (((n)&31)<<0)

//t_rpall
#define DMC400_T_RPALL(n)          (((n)&31)<<0)

//t_rrd
#define DMC400_T_RRD(n)            (((n)&15)<<0)

//t_faw
#define DMC400_T_FAW(n)            (((n)&0x3F)<<0)

//read_latency
#define DMC400_READ_LATENCY(n)     (((n)&15)<<0)

//t_rtr
#define DMC400_T_RTR(n)            (((n)&15)<<0)

//t_rtw
#define DMC400_T_RTW(n)            (((n)&31)<<0)

//t_rtp
#define DMC400_T_RTP(n)            (((n)&15)<<0)

//write_latency
#define DMC400_WRITE_LATENCY(n)    (((n)&15)<<0)

//t_wr
#define DMC400_T_WR(n)             (((n)&31)<<0)

//t_wtr
#define DMC400_T_WTR(n)            (((n)&31)<<0)
#define DMC400_T_WTR_CS(n)         (((n)&31)<<16)

//t_wtw
#define DMC400_T_WTW(n)            (((n)&0x3F)<<16)

//t_eckd
#define DMC400_T_ECKD(n)           (((n)&15)<<0)

//t_xckd
#define DMC400_T_XCKD(n)           (((n)&15)<<0)

//t_ep
#define DMC400_T_EP(n)             (((n)&15)<<0)

//t_xp
#define DMC400_T_XP(n)             (((n)&31)<<0)
#define DMC400_T_XPDLL(n)          (((n)&31)<<16)

//t_esr
#define DMC400_T_ESR(n)            (((n)&0x1FF)<<0)

//t_xsr
#define DMC400_T_XSR(n)            (((n)&0x3FF)<<0)
#define DMC400_T_XSRDLL(n)         (((n)&0x3FF)<<16)

//t_srckd
#define DMC400_T_SRCKD(n)          (((n)&15)<<0)

//t_cksrd
#define DMC400_T_CKSRD(n)          (((n)&15)<<0)

//t_rddata_en
#define DMC400_T_RDDATA_EN(n)      (((n)&15)<<0)

//t_phywrlat
#define DMC400_T_PHYWRLAT(n)       (((n)&15)<<0)
#define DMC400_T_PHYWRDATA         (1<<8)

//rdlvl_control
#define DMC400_RDLVL_MODE_NO_TRAINING (0<<0)
#define DMC400_RDLVL_MODE_PHY_INDEPENDENT_MODE (1<<0)
#define DMC400_RDLVL_MODE_PHY_EVALUATION_MODE (2<<0)
#define DMC400_RDLVL_SETUP_MRS_PRIOR_TRAIN (0<<4)
#define DMC400_RDLVL_SETUP_NOP_PRIOR_TRAIN (1<<4)
#define DMC400_RDLVL_CMD_READ_FOR_TRAIN (0<<8)
#define DMC400_RDLVL_CMD_MRR_FOR_TRAIN (1<<8)
#define DMC400_RDLVL_REFRESH_PRECHARGEALL_PRIOR_TRAIN (0<<12)
#define DMC400_RDLVL_REFRESH_PRECHARGEALL_AUTOREFRESH_PRIOR_TRAIN (1<<12)
#define DMC400_RDLVL_REG_SEL_MR32_FOR_TRAIN (0<<16)
#define DMC400_RDLVL_REG_SEL_MR40_FOR_TRAIN (1<<16)

//rdlvl_mrs
#define DMC400_RDLVL_MRS(n)        (((n)&7)<<0)

//rdlvl_direct
#define DMC400_RDLVL_REQ_READ_EYE_TRAIN (1<<0)
#define DMC400_RDLVL_REQ_READ_GATE_TRAIN (2<<0)
#define DMC400_RDLVL_CHIP_ADDR_CHIP_0 (0<<24)
#define DMC400_RDLVL_CHIP_ADDR_CHIP_1 (1<<24)
#define DMC400_RDLVL_CHANNEL_ADDR_CHANNEL_0 (0<<28)
#define DMC400_RDLVL_CHANNEL_ADDR_CHANNEL_1 (1<<28)

//t_rdlvl_en
#define DMC400_T_RDLVL_EN(n)       (((n)&0x3F)<<0)

//t_rdlvl_rr
#define DMC400_T_RDLVL_RR(n)       (((n)&0x3F)<<0)

//wrlvl_control
#define DMC400_WRLVL_MODE_NO_TRAINING (0<<0)
#define DMC400_WRLVL_MODE_PHY_INDEPENDENT_MODE (1<<0)
#define DMC400_WRLVL_MODE_PHY_EVALUATION_MODE (2<<0)
#define DMC400_WRLVL_REFRESH_PRECHARGEALL_PRIOR_TRAIN (0<<12)
#define DMC400_WRLVL_REFRESH_PRECHARGEALL_AUTOREFRESH_PRIOR_TRAIN (1<<12)

//wrlvl_mrs
#define DMC400_WRLVL_MRS(n)        (((n)&0x1FFF)<<0)

//wrlvl_direct
#define DMC400_WRLVL_REQ           (1<<0)
#define DMC400_WRLVL_CHIP_ADDR_CHIP_0 (0<<24)
#define DMC400_WRLVL_CHIP_ADDR_CHIP_1 (1<<24)
#define DMC400_WRLVL_CHANNEL_ADDR_CHANNEL_0 (0<<28)
#define DMC400_WRLVL_CHANNEL_ADDR_CHANNEL_1 (1<<28)

//t_wrlvl_en
#define DMC400_T_WRLVL_EN(n)       (((n)&0x3F)<<0)

//t_wrlvl_ww
#define DMC400_T_WRLVL_WW(n)       (((n)&0x3F)<<0)

//phy_power_control
#define DMC400_LP_WR_EN_DISABLE    (0<<0)
#define DMC400_LP_WR_EN_ENABLE     (1<<0)
#define DMC400_LP_RD_EN_DISABLE    (0<<1)
#define DMC400_LP_RD_EN_ENABLE     (1<<1)
#define DMC400_LP_IDLE_EN_DISABLE  (0<<2)
#define DMC400_LP_IDLE_EN_ENABLE   (1<<2)
#define DMC400_LP_PD_EN_DISABLE    (0<<3)
#define DMC400_LP_PD_EN_ENABLE     (1<<3)
#define DMC400_LP_SREF_EN_DISABLE  (0<<4)
#define DMC400_LP_SREF_EN_ENABLE   (1<<4)
#define DMC400_LP_DPD_EN_DISABLE   (0<<5)
#define DMC400_LP_DPD_EN_ENABLE    (1<<5)
#define DMC400_LP_WAKEUP_WR(n)     (((n)&15)<<8)
#define DMC400_LP_WAKEUP_RD(n)     (((n)&15)<<12)
#define DMC400_LP_WAKEUP_IDLE(n)   (((n)&15)<<16)
#define DMC400_LP_WAKEUP_PD(n)     (((n)&15)<<20)
#define DMC400_LP_WAKEUP_SREF(n)   (((n)&15)<<24)
#define DMC400_LP_WAKEUP_DPD(n)    (((n)&15)<<28)

//phy_update_control
#define DMC400_PHYUPD_TYPE_00_SREF (0<<0)
#define DMC400_PHYUPD_TYPE_00_STALL (1<<0)
#define DMC400_PHYUPD_TYPE_00_REFNSTALL (2<<0)
#define DMC400_PHYUPD_TYPE_00_DEFER (3<<0)
#define DMC400_PHYUPD_TYPE_01_SREF (0<<2)
#define DMC400_PHYUPD_TYPE_01_STALL (1<<2)
#define DMC400_PHYUPD_TYPE_01_REFNSTALL (2<<2)
#define DMC400_PHYUPD_TYPE_01_DEFER (3<<2)
#define DMC400_PHYUPD_TYPE_10_SREF (0<<4)
#define DMC400_PHYUPD_TYPE_10_STALL (1<<4)
#define DMC400_PHYUPD_TYPE_10_REFNSTALL (2<<4)
#define DMC400_PHYUPD_TYPE_10_DEFER (3<<4)
#define DMC400_PHYUPD_TYPE_11_SREF (0<<6)
#define DMC400_PHYUPD_TYPE_11_STALL (1<<6)
#define DMC400_PHYUPD_TYPE_11_REFNSTALL (2<<6)
#define DMC400_PHYUPD_TYPE_11_DEFER (3<<6)

//user_status
#define DMC400_USER_STATUS(n)      (((n)&0xFF)<<0)

//user_config0
#define DMC400_USER_CONFIG0(n)     (((n)&0xFF)<<0)

//user_config1
#define DMC400_USER_CONFIG1(n)     (((n)&0xFF)<<0)

//integ_cfg
#define DMC400_INTEG_TEST_EN       (1<<0)

//integ_outputs
#define DMC400_COMBINED_INTEG      (1<<0)
#define DMC400_ECC_SEC_INTEG       (1<<1)
#define DMC400_ECC_DED_INTEG       (1<<2)
#define DMC400_ECC_OVERFLOW_INTEG  (1<<3)

//periph_id_0
#define DMC400_PART_0(n)           (((n)&0xFF)<<0)

//periph_id_1
#define DMC400_PART_1(n)           (((n)&15)<<0)
#define DMC400_DES_0(n)            (((n)&15)<<4)

//periph_id_2
#define DMC400_DES_1(n)            (((n)&7)<<0)
#define DMC400_JEDEC               (1<<3)
#define DMC400_REVISION(n)         (((n)&15)<<4)

//periph_id_3
#define DMC400_CMOD(n)             (((n)&0xFF)<<0)

//component_id_0
#define DMC400_PRMBL_0(n)          (((n)&0xFF)<<0)

//component_id_1
#define DMC400_PRMBL_1(n)          (((n)&15)<<0)
#define DMC400_PCLASS(n)           (((n)&15)<<4)

//component_id_2
#define DMC400_PRMBL_2(n)          (((n)&0xFF)<<0)

//component_id_3
#define DMC400_PRMBL_3(n)          (((n)&0xFF)<<0)




#endif
