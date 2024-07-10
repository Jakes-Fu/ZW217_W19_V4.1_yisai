/*******************************************************************************
 ** File Name:    efuse_reg_umc40_64X8_r1p0.h                                  *
 ** Author:                                                                    *
 ** DATE:         07/13/16                                                     *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                               *
 *******************************************************************************/

/*******************************************************************************
 **                   Edit    History                                          *
 **----------------------------------------------------------------------------*
 ** DATE          NAME                 DESCRIPTION                             *
 ** 09/19/11      windy                Create.                                 *
 ******************************************************************************/

#ifndef _EFUSE_PHY_UIX8901_H
#define _EFUSE_PHY_UIX8901_H


#include "uix8910_reg_base.h"

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
    EFUSE_PHY_BLK_ID_0      = EFUSE_PHY_MIN_ID,     /* DCDC CLK_3M */
    EFUSE_PHY_BLK_ID_1      ,                       /* DCDC CLK_3M */
    EFUSE_PHY_BLK_ID_2      ,                       /* ADI option */
    EFUSE_PHY_BLK_ID_3      ,                       /* Fuel gage 的单点校准值 */
    EFUSE_PHY_BLK_ID_4      ,
    EFUSE_PHY_BLK_ID_5      ,                       /* LDOs参考电压校准 */
    EFUSE_PHY_BLK_ID_6      ,
    EFUSE_PHY_BLK_ID_7      ,
    EFUSE_PHY_BLK_ID_8      ,
    EFUSE_PHY_BLK_ID_9      ,
    EFUSE_PHY_BLK_ID_10     ,
    EFUSE_PHY_BLK_ID_11     ,
    EFUSE_PHY_BLK_ID_12     ,                       /* RGB Current driver内部电阻 */
    EFUSE_PHY_BLK_ID_13     ,                       /* CHG CC-CV的校准值6bit CHG_CC-CV<5:0>  */
    EFUSE_PHY_BLK_ID_14     ,                       /* Effuse THM校准 */
    EFUSE_PHY_BLK_ID_15     ,                       /* DCDC PFM threshold, ZX offset threshold */
    EFUSE_PHY_BLK_ID_16     ,                       /* RTC_LDO校准值 */
    EFUSE_PHY_BLK_ID_17     ,                       /* AUXADC offset calibration*/
    EFUSE_PHY_BLK_ID_18     ,                       /* AuxADC 的校准 大量程校准电压点为4.2V和3.6V */
    EFUSE_PHY_BLK_ID_19     ,                       /* AuxADC 的校准 小量程 电压为0.1V和1.0V */
    EFUSE_PHY_BLK_ID_20     ,                       /* 小量程的校准电压Headmic */
    EFUSE_PHY_BLK_ID_21     ,                       /* Audio Ibias/PA OVP/LDO VB calibration */
    EFUSE_PHY_BLK_ID_22     ,                       /* Headmicbias calibration */
    EFUSE_PHY_BLK_ID_23     ,
    EFUSE_PHY_BLK_ID_24     ,
    EFUSE_PHY_BLK_ID_25     ,
    EFUSE_PHY_BLK_ID_26     ,                       /* sleep DCDC CORE */
    EFUSE_PHY_BLK_ID_27     ,                       /* normal DCDC CORE */
    EFUSE_PHY_BLK_ID_28     ,
    EFUSE_PHY_BLK_ID_29     ,                       /* DCDC CORE REFERENCE */
    EFUSE_PHY_BLK_ID_30     ,                       /* DCDC GEN */
    EFUSE_PHY_BLK_ID_31     ,                       /* DCDC WPA */
    EFUSE_PHY_MAX_ID        = EFUSE_PHY_BLK_ID_31,
    EFUSE_PHY_BLK_SUM       = EFUSE_PHY_MAX_ID + 1
} EFUSE_BLOCK_ID_E;
#endif



#define    EFUSE_GLOBAL_CTRL            (EFUSE_REG_BASE + 0x0000UL)
#define    EFUSE_DATA_RD                (EFUSE_REG_BASE + 0x0004UL)
#define    EFUSE_DATA_WR                (EFUSE_REG_BASE + 0x0008UL)
#define    EFUSE_ADDR_INDEX             (EFUSE_REG_BASE + 0x000CUL)
#define    EFUSE_MODE_CTRL              (EFUSE_REG_BASE + 0x0010UL)
#define    EFUSE_STATUS                 (EFUSE_REG_BASE + 0x0014UL)
#define    EFUSE_MAGIC_NUMBER           (EFUSE_REG_BASE + 0x0018UL)
#define    EFUSE_DEB_MAGIC_NUMBER       (EFUSE_REG_BASE + 0x001CUL)
#define    EFUSE_WR_TIMING_CTRL         (EFUSE_REG_BASE + 0x0020UL)
#define    EFUSE_RD_TIMING_CTRL         (EFUSE_REG_BASE + 0x0024UL)


#define    EFUSE_PGM_EN_SHIFT            (0)
#define    EFUSE_PGM_EN                  (0x1UL << EFUSE_PGM_EN_SHIFT)

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
 * Register Name       : EFUSE_ADDR_INDEX
 * Register Offset     : 0x0008
 * Register Description: block index for read, program or bist.
 * Register field description:
 ************************** Fields Definition *********************************/
#define    EFUSE_RW_INDEX_SHIFT                   (0)
#define    EFUSE_RW_INDEX_MSK                     (0x1F << EFUSE_RW_INDEX_SHIFT)


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
 ************************** Fields Definition *********************************/
#define    EFUSE_PG_START                     (BIT_0)
#define    EFUSE_RD_START                     (BIT_1)
#define    EFUSE_NORMAL_RD_FLAG_CLR           (BIT_2)


/******************************************************************************
 * Register Name       : EFUSE_STATUS
 * Register Offset     : 0x0014
 * Register Description: Efuse controller internal status
 * Register field description:
 *   PGM_BUSY                : [0    ] “1” indicate efuse memory in programming mode
 *   READ_BUSY               : [1    ] “1” indicate efuse memory in read mode
 *   STANDBY_BUSY            : [2    ] “1” indicate efuse memory in standby
 *   EFUSE_GLOBAL_PROT       : [3    ] “1” 
 *   EFUSE_NORMAL_RD_DONE    : [4    ] “1” indicate EFUSE normal read has been done
 *                               process
 ************************** Fields Definition *********************************/
#define    EFUSE_PGM_BUSY                     (BIT_0)
#define    EFUSE_READ_BUSY                    (BIT_1)
#define    EFUSE_STANDBY_BUSY                 (BIT_2)
#define    EFUSE_GLOBAL_PROT                  (BIT_3)
#define    EFUSE_NORMAL_RD_DONE               (BIT_4)



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


#endif //_EFUSE_PHY_UIX8901_H

