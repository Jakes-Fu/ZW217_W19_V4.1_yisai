/*******************************************************************************
 ** File Name:    efuse_reg_v7.h                                               *
 ** Author:       windy                                                        *
 ** DATE:         09/19/11                                                     *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.            *
 ** Description:                                                               *
 *******************************************************************************/

/*******************************************************************************
 **                   Edit    History                                          *
 **----------------------------------------------------------------------------*
 ** DATE          NAME                 DESCRIPTION                             *
 ** 09/19/11      windy                Create.                                 *
 ******************************************************************************/

#ifndef _EFUSE_REG_V7_H
#define _EFUSE_REG_V7_H

/*******************************************************************************
 **                      Compiler Flag                                         *
 *******************************************************************************/
#ifdef __cplusplus
extern   "C"
{
#endif

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
typedef enum
{
    EFUSE_PHY_MIN_ID        = 0,
    EFUSE_PHY_BLK_ID_0      = EFUSE_PHY_MIN_ID,
    EFUSE_PHY_BLK_ID_1      ,
    EFUSE_PHY_BLK_ID_2      ,
    EFUSE_PHY_BLK_ID_3      ,
    EFUSE_PHY_BLK_ID_4      ,
    EFUSE_PHY_BLK_ID_5      ,
    EFUSE_PHY_BLK_ID_6      ,
    EFUSE_PHY_BLK_ID_7      ,
    EFUSE_PHY_MAX_ID        = EFUSE_PHY_BLK_ID_7,
    EFUSE_PHY_BLK_SUM       = EFUSE_PHY_MAX_ID + 1
} EFUSE_BLOCK_ID_E;
#else
typedef enum
{
    EFUSE_PHY_MIN_ID        = 0,
    EFUSE_PHY_BLK_ID_0      = EFUSE_PHY_MIN_ID,
    EFUSE_PHY_BLK_ID_1      ,
    EFUSE_PHY_BLK_ID_2      ,
    EFUSE_PHY_BLK_ID_3      ,
    EFUSE_PHY_BLK_ID_4      ,
    EFUSE_PHY_BLK_ID_5      ,
    EFUSE_PHY_BLK_ID_6      ,
    EFUSE_PHY_BLK_ID_7      ,
    EFUSE_PHY_BLK_ID_8      ,
    EFUSE_PHY_BLK_ID_9      ,
    EFUSE_PHY_BLK_ID_10     ,
    EFUSE_PHY_BLK_ID_11     ,
    EFUSE_PHY_BLK_ID_12     ,
    EFUSE_PHY_BLK_ID_13     ,
    EFUSE_PHY_BLK_ID_14     ,
    EFUSE_PHY_BLK_ID_15     ,
    EFUSE_PHY_MAX_ID        = EFUSE_PHY_BLK_ID_15,
    EFUSE_PHY_BLK_SUM       = EFUSE_PHY_MAX_ID + 1
} EFUSE_BLOCK_ID_E;
#endif


#define    EFUSE_GLB_CTRL                (EFUSE_REG_BASE + 0x0000UL)
#define    EFUSE_DATA_RD                (EFUSE_REG_BASE + 0x0004UL)
#define    EFUSE_DATA_WR            (EFUSE_REG_BASE + 0x0008UL)
#define    EFUSE_ADDR_INDEX              (EFUSE_REG_BASE + 0x000CUL)
#define    EFUSE_MODE_CTRL               (EFUSE_REG_BASE + 0x0010UL)
#define    EFUSE_STATUS                 (EFUSE_REG_BASE + 0x0014UL)
#define    EFUSE_MAGIC_NUMBER              (EFUSE_REG_BASE + 0x0018UL)
#define    EFUSE_MARGIN_MAGIC_NUMBER          (EFUSE_REG_BASE + 0x001CUL)
#define    EFUSE_WR_TIMING_CTRL           (EFUSE_REG_BASE + 0x0020UL)
#define    EFUSE_RD_TIMING_CTRL           (EFUSE_REG_BASE + 0x0024UL)
#define    EFUSE_VERSION           (EFUSE_REG_BASE + 0x0028UL)
#define    EFUSE_POR_BLK00           (EFUSE_REG_BASE + 0x0040UL)
#define    EFUSE_POR_BLK01           (EFUSE_REG_BASE + 0x0044UL)
#define    EFUSE_POR_BLK02           (EFUSE_REG_BASE + 0x0048UL)
#define    EFUSE_POR_BLK03           (EFUSE_REG_BASE + 0x004CUL)
#define    EFUSE_POR_BLK04           (EFUSE_REG_BASE + 0x0050UL)
#define    EFUSE_POR_BLK05           (EFUSE_REG_BASE + 0x0054UL)
#define    EFUSE_POR_BLK06           (EFUSE_REG_BASE + 0x0058UL)
#define    EFUSE_POR_BLK07           (EFUSE_REG_BASE + 0x005CUL)
#define    EFUSE_POR_BLK08           (EFUSE_REG_BASE + 0x0060UL)
#define    EFUSE_POR_BLK09           (EFUSE_REG_BASE + 0x0064UL)
#define    EFUSE_POR_BLK10           (EFUSE_REG_BASE + 0x0068UL)
#define    EFUSE_POR_BLK11           (EFUSE_REG_BASE + 0x006CUL)
#define    EFUSE_POR_BLK12           (EFUSE_REG_BASE + 0x0070UL)
#define    EFUSE_POR_BLK13           (EFUSE_REG_BASE + 0x0074UL)
#define    EFUSE_POR_BLK14           (EFUSE_REG_BASE + 0x0078UL)
#define    EFUSE_POR_BLK15           (EFUSE_REG_BASE + 0x007CUL)
#define    EFUSE_POR_BLK16           (EFUSE_REG_BASE + 0x0080UL)
#define    EFUSE_POR_BLK17           (EFUSE_REG_BASE + 0x0084UL)
#define    EFUSE_POR_BLK18           (EFUSE_REG_BASE + 0x0088UL)
#define    EFUSE_POR_BLK19           (EFUSE_REG_BASE + 0x008CUL)
#define    EFUSE_POR_BLK20           (EFUSE_REG_BASE + 0x0090UL)
#define    EFUSE_POR_BLK21           (EFUSE_REG_BASE + 0x0094UL)
#define    EFUSE_POR_BLK22           (EFUSE_REG_BASE + 0x0098UL)
#define    EFUSE_POR_BLK23           (EFUSE_REG_BASE + 0x009CUL)
#define    EFUSE_POR_BLK24           (EFUSE_REG_BASE + 0x00A0UL)
#define    EFUSE_POR_BLK25           (EFUSE_REG_BASE + 0x00A4UL)
#define    EFUSE_POR_BLK26           (EFUSE_REG_BASE + 0x00A8UL)
#define    EFUSE_POR_BLK27           (EFUSE_REG_BASE + 0x00ACUL)
#define    EFUSE_POR_BLK28           (EFUSE_REG_BASE + 0x00B0UL)
#define    EFUSE_POR_BLK29           (EFUSE_REG_BASE + 0x00B4UL)
#define    EFUSE_POR_BLK30           (EFUSE_REG_BASE + 0x00B8UL)
#define    EFUSE_POR_BLK31           (EFUSE_REG_BASE + 0x00BCUL)
#define    EFUSE_POR_BLK32           (EFUSE_REG_BASE + 0x00C0UL)
#define    EFUSE_POR_BLK33           (EFUSE_REG_BASE + 0x00C4UL)
#define    EFUSE_POR_BLK34           (EFUSE_REG_BASE + 0x00C8UL)
#define    EFUSE_POR_BLK35           (EFUSE_REG_BASE + 0x00CCUL)
#define    EFUSE_POR_BLK36           (EFUSE_REG_BASE + 0x00D0UL)
#define    EFUSE_POR_BLK37           (EFUSE_REG_BASE + 0x00D4UL)
#define    EFUSE_POR_BLK38           (EFUSE_REG_BASE + 0x00D8UL)
#define    EFUSE_POR_BLK39           (EFUSE_REG_BASE + 0x00DCUL)
#define    EFUSE_POR_BLK40           (EFUSE_REG_BASE + 0x00E0UL)
#define    EFUSE_POR_BLK41           (EFUSE_REG_BASE + 0x00E4UL)
#define    EFUSE_POR_BLK42           (EFUSE_REG_BASE + 0x00E8UL)
#define    EFUSE_POR_BLK43           (EFUSE_REG_BASE + 0x00ECUL)
#define    EFUSE_POR_BLK44           (EFUSE_REG_BASE + 0x00F0UL)
#define    EFUSE_POR_BLK45           (EFUSE_REG_BASE + 0x00F4UL)
#define    EFUSE_POR_BLK46           (EFUSE_REG_BASE + 0x00F8UL)
#define    EFUSE_POR_BLK47           (EFUSE_REG_BASE + 0x00FCUL)
#define    EFUSE_POR_BLK48           (EFUSE_REG_BASE + 0x0100UL)
#define    EFUSE_POR_BLK49           (EFUSE_REG_BASE + 0x0104UL)
#define    EFUSE_POR_BLK50           (EFUSE_REG_BASE + 0x0108UL)
#define    EFUSE_POR_BLK51           (EFUSE_REG_BASE + 0x010CUL)
#define    EFUSE_POR_BLK52           (EFUSE_REG_BASE + 0x0110UL)
#define    EFUSE_POR_BLK53           (EFUSE_REG_BASE + 0x0114UL)
#define    EFUSE_POR_BLK54           (EFUSE_REG_BASE + 0x0118UL)
#define    EFUSE_POR_BLK55           (EFUSE_REG_BASE + 0x011CUL)
#define    EFUSE_POR_BLK56           (EFUSE_REG_BASE + 0x0120UL)
#define    EFUSE_POR_BLK57           (EFUSE_REG_BASE + 0x0124UL)
#define    EFUSE_POR_BLk58           (EFUSE_REG_BASE + 0x0128UL)
#define    EFUSE_POR_BLK59           (EFUSE_REG_BASE + 0x012CUL)
#define    EFUSE_POR_BLK60           (EFUSE_REG_BASE + 0x0130UL)
#define    EFUSE_POR_BLK61           (EFUSE_REG_BASE + 0x0134UL)
#define    EFUSE_POR_BLK62           (EFUSE_REG_BASE + 0x0138UL)
#define    EFUSE_POR_BLK63           (EFUSE_REG_BASE + 0x013CUL)

/******************************************************************************
 * Register Name       : EFUSE_DATA_RD
 * Register Offset     : 0x0000
 * Register Description: Data read from efuse memory
 * Register field description:
 *   EFUSE_DATA_RD           : [31:0 ] Efuse data, If SW use efuse controller
 *                              to send a read command to efuse memory, the
 *                              return value will store here.
 ******************************************************************************/
#define    EFUSE_DATA_RD_SHIFT                (0)
#define    EFUSE_DATA_RD_MSK                  (0xFFFFFFFFUL << EFUSE_DATA_RD_SHIFT)

/******************************************************************************
 * Register Name       : EFUSE_DATA_WR
 * Register Offset     : 0x0004
 * Register Description: Data to be write to efuse memory
 * Register field description:
 *   EFUSE_DATA_WR           : [31:0 ] Efuse data to be write. If SW want to
 *                              program the efuse memory, the data to be
 *                              programmed will write to this register before
 *                              SW issue a PGM command
 ************************** Fields Definition *********************************/
#define    EFUSE_DATA_WR_SHIFT                (0)
#define    EFUSE_DATA_WR_MSK                  (0xFFFFFFFFUL << EFUSE_DATA_WR_SHIFT)

/******************************************************************************
 * Register Name       : EFUSE_BLOCK_INDEX
 * Register Offset     : 0x0008
 * Register Description: block index for read, program or bist.
 * Register field description:
 *   READ_INDEX              : [2 :0 ] The efuse memory block index to be read,
 *                               when efuse controller read done next time ,the
 *                               read data is in regiser ¡°EFUSE_DATA_RD ¡°.
 *                              Attention : if Efuse is in PGM_MODE, since the
 *                              efuse controller will first read the efuse
 *                              memory, so in this case SW must makesure that
 *                              read_index equal to pgm_index.
 *   PGM_INDEX               : [18:16] The efuse memory block index to be
 *                              programmed, when issue a PGM command ,efuse
 *                              controller will write regiser value of ¡°
 *                              efuse_data_wr ¡± into efuse data block n.
 *   BIST_START_INDEX        : [28:26] In efuse memory bist test mode, this
 *                              field indicate the start word index
 *   BIST_SIZE               : [31:29] In efuse memory bist test mode,
 *                              this field indicate how many words will be
 *                              checked by bist logic from start index.
 ************************** Fields Definition *********************************/
#define    EFUSE_READ_INDEX_SHIFT                   (0)
#define    EFUSE_READ_INDEX_MSK                     (0x7 << EFUSE_READ_INDEX_SHIFT)
#define    EFUSE_PGM_INDEX_SHIFT                    (16)
#define    EFUSE_PGM_INDEX_MSK                      (0x7 << EFUSE_PGM_INDEX_SHIFT)
#define    EFUSE_BIST_START_INDEX_SHIFT             (26)
#define    EFUSE_BIST_START_INDEX_MSK               (0x7 << EFUSE_BIST_START_INDEX_SHIFT)
#define    EFUSE_BIST_SIZE_SHIFT                    (29)
#define    EFUSE_BIST_SIZE_MSK                      (0x7UL << EFUSE_BIST_SIZE_SHIFT)

/******************************************************************************
 * Register Name       : EFUSE_MODE_CTRL
 * Register Offset     : 0x000C
 * Register Description: Mode control of efuse memory
 * Register field description:
 *   EFUSE_PG_START          : [0    ] Write 1 to this bit start efuse program
 *                              process. Read this bit will always get 0
 *   EFUSE_RD_START          : [1    ] Write 1 to this bit start efuse read
 *                              process, Read this bit will always get 0
 *   EFUSE_STANDBY_START     : [2    ] Write 1 to this bit will make efuse went
 *                               to standby mode, Read this bit will always get
 *                               0
 *   BIST_START              : [31   ] When BIST_SW_EN=1, write 1 to this bit
 *                              will start efuse memory bist test process. this
 *                               work mode is designed for efuse memory
 *                              function test when mass production. DON¡¯T
 *                              active this process in normal working flow.
 ************************** Fields Definition *********************************/
#define    EFUSE_PG_START                     (BIT_0)
#define    EFUSE_RD_START                     (BIT_1)
#define    EFUSE_STANDBY_START                (BIT_2)
#define    EFUSE_BIST_START                   (BIT_31)

/******************************************************************************
 * Register Name       : EFUSE_PGM_PARA
 * Register Offset     : 0x0010
 * Register Description: Timing parameters in PGM and other control
 * Register field description:
 *   TPGM_TIME_CNT           : [8 :0 ] Clk_efuse Cycle counter, if set n, the
 *                              Tpgm time will last for (n+1) clk_efuse cycle,
 *                              only when PGM_EN=1 can write this field, in
 *                              general case, DON¡¯T change this field value.
 *   BLK0_AUTO_TEST_EN       : [16   ] If set this bit, when each time SW
 *                              program efuse memory block0, the controller
 *                              will automatically read its value and compare
 *                              with write value. If not match, the flag bit of
 *                               ¡°MEM_BLK0_ERR_FLAG¡± will set
 *   BLK1_AUTO_TEST_EN       : [17   ] If set this bit, when each time SW
 *                              program efuse memory block1, the controller
 *                              will automatically read its value and compare
 *                              with write value. If not match, the flag bit of
 *                               ¡°MEM_BLK1_ERR_FLAG¡± will set
 *   BLK2_AUTO_TEST_EN       : [18   ] If set this bit, when each time SW
 *                              program efuse memory block2, the controller
 *                              will automatically read its value and compare
 *                              with write value. If not match, the flag bit of
 *                               ¡°MEM_BLK2_ERR_FLAG¡± will set
 *   BLK3_AUTO_TEST_EN       : [19   ] If set this bit, when each time SW
 *                              program efuse memory block3, the controller
 *                              will automatically read its value and compare
 *                              with write value. If not match, the flag bit of
 *                               ¡°MEM_BLK3_ERR_FLAG¡± will set
 *   BLK4_AUTO_TEST_EN       : [20   ] If set this bit, when each time SW
 *                              program efuse memory block4, the controller
 *                              will automatically read its value and compare
 *                              with write value. If not match, the flag bit of
 *                               ¡°MEM_BLK4_ERR_FLAG¡± will set
 *   BLK5_AUTO_TEST_EN       : [21   ] If set this bit, when each time SW
 *                              program efuse memory block5, the controller
 *                              will automatically read its value and compare
 *                              with write value. If not match, the flag bit of
 *                               ¡°MEM_BLK5_ERR_FLAG¡± will set
 *   BLK6_AUTO_TEST_EN       : [22   ] If set this bit, when each time SW
 *                              program efuse memory block6, the controller
 *                              will automatically read its value and compare
 *                              with write value. If not match, the flag bit of
 *                               ¡°MEM_BLK6_ERR_FLAG¡± will set
 *   BLK7_AUTO_TEST_EN       : [23   ] If set this bit, when each time SW
 *                              program efuse memory block7, the controller
 *                              will automatically read its value and compare
 *                              with write value. If not match, the flag bit of
 *                               ¡°MEM_BLK7_ERR_FLAG¡± will set
 *   BIST_SW_EN              : [27   ] Write 1 to this bit will enable bist
 *                              test mode. This mode is for mass production
 *                              function test, DON¡¯T set this bit in normal
 *                              working mode.
 *   CLK_EFS_EN              : [28   ] Set to 1 will open clk_efs This bit must
 *                               set before efuse controller is in use
 *   EFUSE_VDD_ON            : [29   ] Set this bit will open 1.1v power supply
 *                               for efuse memory, before any operation towards
 *                               to efuse memory this bit have to set to 1.
 *   PCLK_DIV_EN             : [30   ] When set, the clk_efuse will equal to a
 *                              half of PCLK, otherwise will equal to PCLK.
 *                              This bit is for timing adjust and debug use, in
 *                               most case no need and don¡¯¡¯t change its
 *                              default value
 *   PGM_EN                  : [31   ] Only set this bit can SW write register
 *                              field of ¡°TPGM_TIME_CNT¡± and start PGM mode (
 *                              this bit used to protect software unexpectedly
 *                              programmed efuse memory)
 ************************** Fields Definition *********************************/
#define    EFUSE_TPGM_TIME_CNT_SHIFT          (0)
#define    EFUSE_TPGM_TIME_CNT_MSK            (0x1FFUL << EFUSE_TPGM_TIME_CNT_SHIFT)
#define    EFUSE_BLK0_AUTO_TEST_EN            (BIT_10)
#define    EFUSE_BLK1_AUTO_TEST_EN            (BIT_11)
#define    EFUSE_BLK2_AUTO_TEST_EN            (BIT_12)
#define    EFUSE_BLK3_AUTO_TEST_EN            (BIT_13)
#define    EFUSE_BLK4_AUTO_TEST_EN            (BIT_14)
#define    EFUSE_BLK5_AUTO_TEST_EN            (BIT_15)
#define    EFUSE_BLK6_AUTO_TEST_EN            (BIT_16)
#define    EFUSE_BLK7_AUTO_TEST_EN            (BIT_17)
#define    EFUSE_BLK8_AUTO_TEST_EN            (BIT_18)
#define    EFUSE_BLK9_AUTO_TEST_EN            (BIT_19)
#define    EFUSE_BLK10_AUTO_TEST_EN           (BIT_20)
#define    EFUSE_BLK11_AUTO_TEST_EN           (BIT_21)
#define    EFUSE_BLK12_AUTO_TEST_EN           (BIT_22)
#define    EFUSE_BLK13_AUTO_TEST_EN           (BIT_23)
#define    EFUSE_BLK14_AUTO_TEST_EN           (BIT_24)
#define    EFUSE_BLK15_AUTO_TEST_EN           (BIT_25)
#define    EFUSE_BIST_SW_EN                   (BIT_27)
#define    EFUSE_CLK_EFS_EN                   (BIT_28)
#define    EFUSE_VDD_ON                       (BIT_29)
#define    EFUSE_PCLK_DIV_EN                  (BIT_30)
#define    EFUSE_PGM_EN                       (BIT_31)

/******************************************************************************
 * Register Name       : EFUSE_STATUS
 * Register Offset     : 0x0014
 * Register Description: Efuse controller internal status
 * Register field description:
 *   PGM_BUSY                : [0    ] ¡°1¡± indicate efuse memory in
 *                              programming mode
 *   READ_BUSY               : [1    ] ¡°1¡± indicate efuse memory in read mode
 *   STANDBY_BUSY            : [2    ] ¡°1¡± indicate efuse memory in standby
 *                              mode [3]   R0  1¡¯h0   Reserved
 *   BIST_FAIL               : [4    ] ¡°1¡± will indicate that bist test faile
 *                              d
 *   BIST_BUSY               : [5    ] ¡°1¡± will indicate that bist test is in
 *                               process
 ************************** Fields Definition *********************************/
#define    EFUSE_PGM_BUSY                     (BIT_0)
#define    EFUSE_READ_BUSY                    (BIT_1)
#define    EFUSE_STANDBY_BUSY                 (BIT_2)
#define    EFUSE_BIST_FAIL                    (BIT_4)
#define    EFUSE_BIST_BUSY                    (BIT_5)

/******************************************************************************
 * Register Name       : EFUSE_BLK_FLAGS
 * Register Offset     : 0x0018
 * Register Description: Flags for each block
 * Register field description:
 *   BLK0_PROT_FLAG          : [0    ] If the SW send a PGM command to memory
 *                              block0, and the controller found this memory
 *                              block need to be protected(which means the
 *                              highest bit is 1), this bit will set to 1
 *   BLK1_PROT_FLAG          : [1    ] If the SW send a PGM command to memory
 *                              block1, and the controller found this memory
 *                              block need to be protected(which means the
 *                              highest bit is 1), this bit will set to 1
 *   BLK2_PROT_FLAG          : [2    ] If the SW send a PGM command to memory
 *                              block2, and the controller found this memory
 *                              block need to be protected(which means the
 *                              highest bit is 1), this bit will set to 1
 *   BLK3_PROT_FLAG          : [3    ] If the SW send a PGM command to memory
 *                              block3, and the controller found this memory
 *                              block need to be protected(which means the
 *                              highest bit is 1), this bit will set to 1
 *   BLK4_PROT_FLAG          : [4    ] If the SW send a PGM command to memory
 *                              block4, and the controller found this memory
 *                              block need to be protected(which means the
 *                              highest bit is 1), this bit will set to 1
 *   BLK5_PROT_FLAG          : [5    ] If the SW send a PGM command to memory
 *                              block5, and the controller found this memory
 *                              block need to be protected(which means the
 *                              highest bit is 1), this bit will set to 1
 *   BLK6_PROT_FLAG          : [6    ] If the SW send a PGM command to memory
 *                              block6, and the controller found this memory
 *                              block need to be protected(which means the
 *                              highest bit is 1), this bit will set to 1
 *   BLK7_PROT_FLAG          : [7    ] If the SW send a PGM command to memory
 *                              block7, and the controller found this memory
 *                              block need to be protected(which means the
 *                              highest bit is 1), this bit will set to 1
 *   BLK0_ERR_FLAG           : [8    ] If BLK0_AUTO_TEST_EN is set, and
 *                              controller compared the value read after PGM
 *                              from the same block, and found the two value
 *                              not match, this bit will set as an error flag.
 *                              But if this block is protected, the PGM command
 *                               in-fact not really send, so this bit will not
 *                              set.
 *   BLK1_ERR_FLAG           : [9    ] If BLK1_AUTO_TEST_EN is set, and
 *                              controller compared the value read after PGM
 *                              from the same block, and found the two value
 *                              not match, this bit will set as an error flag.
 *                              But if this block is protected, the PGM command
 *                               in-fact not really send, so this bit will not
 *                              set.
 *   BLK2_ERR_FLAG           : [10   ] If BLK2_AUTO_TEST_EN is set, and
 *                              controller compared the value read after PGM
 *                              from the same block, and found the two value
 *                              not match, this bit will set as an error flag.
 *                              But if this block is protected, the PGM command
 *                               in-fact not really send, so this bit will not
 *                              set.
 *   BLK3_ERR_FLAG           : [11   ] If BLK3_AUTO_TEST_EN is set, and
 *                              controller compared the value read after PGM
 *                              from the same block, and found the two value
 *                              not match, this bit will set as an error flag.
 *                              But if this block is protected, the PGM command
 *                               in-fact not really send, so this bit will not
 *                              set.
 *   BLK4_ERR_FLAG           : [12   ] If BLK4_AUTO_TEST_EN is set, and
 *                              controller compared the value read after PGM
 *                              from the same block, and found the two value
 *                              not match, this bit will set as an error flag.
 *                              But if this block is protected, the PGM command
 *                               in-fact not really send, so this bit will not
 *                              set.
 *   BLK5_ERR_FLAG           : [13   ] If BLK5_AUTO_TEST_EN is set, and
 *                              controller compared the value read after PGM
 *                              from the same block, and found the two value
 *                              not match, this bit will set as an error flag.
 *                              But if this block is protected, the PGM command
 *                               in-fact not really send, so this bit will not
 *                              set.
 *   BLK6_ERR_FLAG           : [14   ] If BLK6_AUTO_TEST_EN is set, and
 *                              controller compared the value read after PGM
 *                              from the same block, and found the two value
 *                              not match, this bit will set as an error flag.
 *                              But if this block is protected, the PGM command
 *                               in-fact not really send, so this bit will not
 *                              set.
 *   BLK7_ERR_FLAG           : [15   ] If BLK7_AUTO_TEST_EN is set, and
 *                              controller compared the value read after PGM
 *                              from the same block, and found the two value
 *                              not match, this bit will set as an error flag.
 *                              But if this block is protected, the PGM command
 *                               in-fact not really send, so this bit will not
 *                              set.
 ************************** Fields Definition *********************************/
#define    EFUSE_BLK0_PROT_FLAG               (BIT_0)
#define    EFUSE_BLK1_PROT_FLAG               (BIT_1)
#define    EFUSE_BLK2_PROT_FLAG               (BIT_2)
#define    EFUSE_BLK3_PROT_FLAG               (BIT_3)
#define    EFUSE_BLK4_PROT_FLAG               (BIT_4)
#define    EFUSE_BLK5_PROT_FLAG               (BIT_5)
#define    EFUSE_BLK6_PROT_FLAG               (BIT_6)
#define    EFUSE_BLK7_PROT_FLAG               (BIT_7)
#define    EFUSE_BLK8_PROT_FLAG               (BIT_8)
#define    EFUSE_BLK9_PROT_FLAG               (BIT_9)
#define    EFUSE_BLK10_PROT_FLAG              (BIT_10)
#define    EFUSE_BLK11_PROT_FLAG              (BIT_11)
#define    EFUSE_BLK12_PROT_FLAG              (BIT_12)
#define    EFUSE_BLK13_PROT_FLAG              (BIT_13)
#define    EFUSE_BLK14_PROT_FLAG              (BIT_14)
#define    EFUSE_BLK15_PROT_FLAG              (BIT_15)
#define    EFUSE_BLK0_ERR_FLAG                (BIT_16)
#define    EFUSE_BLK1_ERR_FLAG                (BIT_17)
#define    EFUSE_BLK2_ERR_FLAG                (BIT_18)
#define    EFUSE_BLK3_ERR_FLAG                (BIT_19)
#define    EFUSE_BLK4_ERR_FLAG                (BIT_20)
#define    EFUSE_BLK5_ERR_FLAG                (BIT_21)
#define    EFUSE_BLK6_ERR_FLAG                (BIT_22)
#define    EFUSE_BLK7_ERR_FLAG                (BIT_23)
#define    EFUSE_BLK8_ERR_FLAG                (BIT_24)
#define    EFUSE_BLK9_ERR_FLAG                (BIT_25)
#define    EFUSE_BLK10_ERR_FLAG               (BIT_26)
#define    EFUSE_BLK11_ERR_FLAG               (BIT_27)
#define    EFUSE_BLK12_ERR_FLAG               (BIT_28)
#define    EFUSE_BLK13_ERR_FLAG               (BIT_29)
#define    EFUSE_BLK14_ERR_FLAG               (BIT_30)
#define    EFUSE_BLK15_ERR_FLAG               (BIT_31)

/******************************************************************************
 * Register Name       : EFUSE_BLK_FLAGS_CLR
 * Register Offset     : 0x001C
 * Register Description: To clear EFUSE_BLK_FLAGS
 * Register field description:
 *   BLK0_PROT_FLAG_CLR      : [0    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK0_PROT_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK1_PROT_FLAG_CLR      : [1    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK1_PROT_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK2_PROT_FLAG_CLR      : [2    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK2_PROT_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK3_PROT_FLAG_CLR      : [3    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK3_PROT_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK4_PROT_FLAG_CLR      : [4    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK4_PROT_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK5_PROT_FLAG_CLR      : [5    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK5_PROT_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK6_PROT_FLAG_CLR      : [6    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK6_PROT_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK7_PROT_FLAG_CLR      : [7    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK7_PROT_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK0_ERR_FLAG_CLR       : [8    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK0_ERR_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK1_ERR_FLAG_CLR       : [9    ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK1_ERR_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK2_ERR_FLAG_CLR       : [10   ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK2_ERR_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK3_ERR_FLAG_CLR       : [11   ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK3_ERR_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK4_ERR_FLAG_CLR       : [12   ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK4_ERR_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK5_ERR_FLAG_CLR       : [13   ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK5_ERR_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK6_ERR_FLAG_CLR       : [14   ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK6_ERR_FLAG¡±, read this bit will always
 *                              get 0
 *   BLK7_ERR_FLAG_CLR       : [15   ] Write this bit ¡°1¡± will clear flag bit
 *                               ¡°BLK7_ERR_FLAG¡±, read this bit will always
 *                              get 0
 ************************** Fields Definition *********************************/
#define    EFUSE_BLK0_PROT_FLAG_CLR           (BIT_0)
#define    EFUSE_BLK1_PROT_FLAG_CLR           (BIT_1)
#define    EFUSE_BLK2_PROT_FLAG_CLR           (BIT_2)
#define    EFUSE_BLK3_PROT_FLAG_CLR           (BIT_3)
#define    EFUSE_BLK4_PROT_FLAG_CLR           (BIT_4)
#define    EFUSE_BLK5_PROT_FLAG_CLR           (BIT_5)
#define    EFUSE_BLK6_PROT_FLAG_CLR           (BIT_6)
#define    EFUSE_BLK7_PROT_FLAG_CLR           (BIT_7)
#define    EFUSE_BLK8_PROT_FLAG_CLR           (BIT_8)
#define    EFUSE_BLK9_PROT_FLAG_CLR           (BIT_9)
#define    EFUSE_BLK10_PROT_FLAG_CLR          (BIT_10)
#define    EFUSE_BLK11_PROT_FLAG_CLR          (BIT_11)
#define    EFUSE_BLK12_PROT_FLAG_CLR          (BIT_12)
#define    EFUSE_BLK13_PROT_FLAG_CLR          (BIT_13)
#define    EFUSE_BLK14_PROT_FLAG_CLR          (BIT_14)
#define    EFUSE_BLK15_PROT_FLAG_CLR          (BIT_15)
#define    EFUSE_BLK0_ERR_FLAG_CLR            (BIT_16)
#define    EFUSE_BLK1_ERR_FLAG_CLR            (BIT_17)
#define    EFUSE_BLK2_ERR_FLAG_CLR            (BIT_18)
#define    EFUSE_BLK3_ERR_FLAG_CLR            (BIT_19)
#define    EFUSE_BLK4_ERR_FLAG_CLR            (BIT_20)
#define    EFUSE_BLK5_ERR_FLAG_CLR            (BIT_21)
#define    EFUSE_BLK6_ERR_FLAG_CLR            (BIT_22)
#define    EFUSE_BLK7_ERR_FLAG_CLR            (BIT_23)
#define    EFUSE_BLK8_ERR_FLAG_CLR            (BIT_24)
#define    EFUSE_BLK9_ERR_FLAG_CLR            (BIT_25)
#define    EFUSE_BLK10_ERR_FLAG_CLR           (BIT_26)
#define    EFUSE_BLK11_ERR_FLAG_CLR           (BIT_27)
#define    EFUSE_BLK12_ERR_FLAG_CLR           (BIT_28)
#define    EFUSE_BLK13_ERR_FLAG_CLR           (BIT_29)
#define    EFUSE_BLK14_ERR_FLAG_CLR           (BIT_30)
#define    EFUSE_BLK15_ERR_FLAG_CLR           (BIT_31)

/******************************************************************************
 * Register Name       : EFUSE_MAGIC_NUMBER
 * Register Offset     : 0x0020
 * Register Description:  magic number to protect efuse from un-intentionally
 *                     programming
 * Register field description:
 *   EFUSE_MAGIC_NUMBER      : [15:0 ] Magic number, only when this field is
 *                              0x8810, the Efuse programming command can be
 *                              handle. So if SW want to program efuse memory,
 *                              except open clocks and power, 2 other
 *                              conditions must be met : (1)   PGM_EN =1; (2)
 *                              EFUSE_MAGIC_NUMBER = 0x8810
 ************************** Fields Definition *********************************/
#define    EFUSE_MAGIC_NUMBER_SHIFT           (0)
#define    EFUSE_MAGIC_NUMBER_MSK             (0xFFFFUL << EFUSE_MAGIC_NUMBER_SHIFT)


/*******************************************************************************
 **                      Compiler Flag                                         *
 *******************************************************************************/
#ifdef __cplusplus
}
#endif


#endif //_EFUSE_REG_V5_H

