/******************************************************************************
 **  File Name:    efuse_reg_umc40_64X8_r1p0.c                                             *
 **  Author:       Jenny Deng                                                 *
 **  Date:         20/10/2009                                                 *
 **  Copyright:    2009 Spreadtrum, Incorporated. All Rights Reserved.        *
 **  Description:  This file defines the basic operation interfaces of        *
 **                EFuse initilize and operation. It provides read and        *
 **                writer interfaces of 0~5 efuse. Efuse 0 for Sn block.      *
 **                Efuse 1 to 4 for Hash blocks. Efuse 5 for control block.   *
 ******************************************************************************
 ******************************************************************************
 **  Edit History                                                             *
 **-----------------------------------------------------------------------*---*
 **  DATE               Author              Operation                         *
 **  20/10/2009         Jenny.Deng          Create.                           *
 **   26/06/2012        windy.wang          Update                            *
 ******************************************************************************/

#include "sci_types.h"
#include "arm_reg.h"
#include "chip_plf_export.h"
#include "efuse_drvapi.h"
#include "../efuse_phy.h"
#include "efuse_reg_umc40_64X8_r1p0.h"
#include "ldo_drvapi.h"

extern uint32 OS_TickDelay (uint32 ticks);

#define EFUSE_BLK_MAX_DATA 0x7FFFFFFF
#define EFUSE_LOCK_BIT       (BIT_31)
LOCAL uint32 s_efuse_blks_data[EFUSE_PHY_BLK_SUM] = {0};
LOCAL LDO_VOLT_LEVEL_E s_vcama_org;
LOCAL uint32 s_cama_volt;
#define EFUSE_VOLT_VAL 2500  //mv

/******************************************************************************
 * [Description] :                                                            *
 *      clear efuse block protect flag                                        *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected efuse block                      *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL void EFUSE_PHY_ClrBlkProtFlag (
    uint32 block_id
)
{
    uint32 reg_value;

    switch (block_id)
    {
        case EFUSE_PHY_BLK_ID_0:
            reg_value = EFUSE_BLK0_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_1:
            reg_value = EFUSE_BLK1_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_2:
            reg_value = EFUSE_BLK2_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_3:
            reg_value = EFUSE_BLK3_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_4:
            reg_value = EFUSE_BLK4_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_5:
            reg_value = EFUSE_BLK5_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_6:
            reg_value = EFUSE_BLK6_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_7:
            reg_value = EFUSE_BLK7_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_8:
            reg_value = EFUSE_BLK8_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_9:
            reg_value = EFUSE_BLK9_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_10:
            reg_value = EFUSE_BLK10_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_11:
            reg_value = EFUSE_BLK11_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_12:
            reg_value = EFUSE_BLK12_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_13:
            reg_value = EFUSE_BLK13_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_14:
            reg_value = EFUSE_BLK14_PROT_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_15:
            reg_value = EFUSE_BLK15_PROT_FLAG_CLR;    
            break;
        default:
            reg_value = 0;
            break;
    }

    CHIP_REG_SET (EFUSE_BLK_FLAGS_CLR,reg_value);
}
/******************************************************************************
 * [Description] :                                                            *
 *      clear efuse block error flag                                          *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected efuse block                      *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL void EFUSE_PHY_ClrBlkErrFlag (
    uint32 block_id
)
{
    uint32 reg_value;

    switch (block_id)
    {
        case EFUSE_PHY_BLK_ID_0:
            reg_value = EFUSE_BLK0_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_1:
            reg_value = EFUSE_BLK1_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_2:
            reg_value = EFUSE_BLK2_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_3:
            reg_value = EFUSE_BLK3_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_4:
            reg_value = EFUSE_BLK4_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_5:
            reg_value = EFUSE_BLK5_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_6:
            reg_value = EFUSE_BLK6_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_7:
            reg_value = EFUSE_BLK7_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_8:
            reg_value = EFUSE_BLK8_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_9:
            reg_value = EFUSE_BLK9_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_10:
            reg_value = EFUSE_BLK10_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_11:
            reg_value = EFUSE_BLK11_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_12:
            reg_value = EFUSE_BLK12_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_13:
            reg_value = EFUSE_BLK13_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_14:
            reg_value = EFUSE_BLK14_ERR_FLAG_CLR;
            break;
        case EFUSE_PHY_BLK_ID_15:
            reg_value = EFUSE_BLK15_ERR_FLAG_CLR;
            break;    
        default:
            reg_value = 0;
            break;
    }

    CHIP_REG_SET (EFUSE_BLK_FLAGS_CLR, reg_value);
}

/******************************************************************************
 * [Description] :                                                            *
 *      set block id for "reading" / "writing" efuse block                    *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL void EFUSE_PHY_ConfigBlkId (
    uint32 block_id
)
{
    uint32 reg_value;
    reg_value = CHIP_REG_GET (EFUSE_BLOCK_INDEX);
    reg_value &= ~ (EFUSE_READ_INDEX_MSK | EFUSE_PGM_INDEX_MSK);
    reg_value |= (block_id << EFUSE_READ_INDEX_SHIFT) | (block_id << EFUSE_PGM_INDEX_SHIFT);
    CHIP_REG_SET (EFUSE_BLOCK_INDEX , reg_value);
}

/******************************************************************************
 * [Description] :                                                            *
 *      set block id for bist efuse test                                      *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *    1.If size eq 0, efuse block "start_id" would enter bist test mode       *
 *    2.If size eq 1, efuse block "start_id" ~ "start_id + 1" would enter     *
 *      bist test mode                                                        *
 *    3.If size eq n, efuse block "start_id" ~ "start_id + n" would enter     *
 *      bist test mode                                                        *
 ******************************************************************************/
LOCAL void EFUSE_PHY_ConfigBistBlkId (
    uint32 start_id,
    uint32 size
)
{
    uint32 reg_value;
    reg_value = CHIP_REG_GET (EFUSE_BLOCK_INDEX);
    reg_value &= ~ (EFUSE_BIST_START_INDEX_MSK | EFUSE_BIST_SIZE_MSK);
    reg_value |= (start_id << EFUSE_BIST_START_INDEX_SHIFT) & EFUSE_BIST_START_INDEX_MSK;
    reg_value |= (size << EFUSE_BIST_SIZE_SHIFT) & EFUSE_BIST_SIZE_MSK;

    CHIP_REG_SET (EFUSE_BLOCK_INDEX , reg_value);
}

/******************************************************************************
 * [Description] :                                                            *
 *      set data that is going to be write to an efuse block                  *
 * [Global resource dependence]:                                              *
 *      data: data that is to be written to the respective EFuse block        *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL void EFUSE_PHY_ConfigWriteData (
    uint32 data
)
{
    CHIP_REG_SET (EFUSE_DATA_WR, data);
}

/******************************************************************************
 * [Description] :                                                            *
 *      send program/read/standy/bist start commmand                          *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *   EFUSE_PROG_MODE : efuse program start command                            *
 *   EFUSE_READ_MODE : efuse read start command                               *
 *   EFUSE_STANDBY_MODE : efuse standy by start command                       *
 *   EFUSE_BIST_MODE : efuse bist start command                               *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL void EFUSE_PHY_ConfigMode (
    EFUSE_MODE_CTL_E mode
)
{
    uint32 reg_value;

    switch (mode)
    {
        case EFUSE_PROG_MODE:
            reg_value = EFUSE_PG_START;
            break;
        case EFUSE_READ_MODE:
            reg_value = EFUSE_RD_START;
            break;
        case EFUSE_STANDBY_MODE:
            reg_value = EFUSE_STANDBY_START;
            break;
        case EFUSE_BIST_MODE:
            reg_value = EFUSE_BIST_START;
            break;
        default:
            reg_value = 0;
            break;
    }

    CHIP_REG_SET (EFUSE_MODE_CTRL, reg_value);
}

/******************************************************************************
 * [Description] :                                                            *
 *      EFuse bist test of efuse register, before use                         *
 *      efuse must call this function to init efuse                           *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      is_enable: enable/disable bist test                                   *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      bist test is different from reading and writing operation             *
 ******************************************************************************/
LOCAL void EFUSE_PHY_EnableBistTest (
    BOOLEAN is_enable
)
{
    uint32 reg_value = EFUSE_BIST_SW_EN;

    if (is_enable)
    {
        CHIP_REG_OR (EFUSE_PGM_PARA, reg_value);
    }
    else
    {
        CHIP_REG_AND (EFUSE_PGM_PARA, ~ reg_value);
    }
}

/******************************************************************************
 * [Description] :                                                            *
 *      enable/disable block auto test                                        *
 * [Global resource dependence]:                                              *
 *      block_id : id number of the selected efuse block                      *
 *      is_enable: enable/disable                                             *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL void EFUSE_PHY_EnableBlkAutoTest (
    uint32 block_id,
    BOOLEAN is_enable
)
{
    uint32 reg_value = CHIP_REG_GET (EFUSE_PGM_PARA);

    if (is_enable)
    {
        switch (block_id)
        {
            case EFUSE_PHY_BLK_ID_0:
                reg_value |= EFUSE_BLK0_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_1:
                reg_value |= EFUSE_BLK1_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_2:
                reg_value |= EFUSE_BLK2_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_3:
                reg_value |= EFUSE_BLK3_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_4:
                reg_value |= EFUSE_BLK4_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_5:
                reg_value |= EFUSE_BLK5_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_6:
                reg_value |= EFUSE_BLK6_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_7:
                reg_value |= EFUSE_BLK7_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_8:
                reg_value |= EFUSE_BLK8_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_9:
                reg_value |= EFUSE_BLK9_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_10:
                reg_value |= EFUSE_BLK10_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_11:
                reg_value |= EFUSE_BLK11_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_12:
                reg_value |= EFUSE_BLK12_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_13:
                reg_value |= EFUSE_BLK13_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_14:
                reg_value |= EFUSE_BLK14_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_15:
                reg_value |= EFUSE_BLK15_AUTO_TEST_EN;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (block_id)
        {
            case EFUSE_PHY_BLK_ID_0:
                reg_value &= ~ (uint32) EFUSE_BLK0_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_1:
                reg_value &= ~ EFUSE_BLK1_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_2:
                reg_value &= ~ EFUSE_BLK2_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_3:
                reg_value &= ~ EFUSE_BLK3_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_4:
                reg_value &= ~ EFUSE_BLK4_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_5:
                reg_value &= ~ EFUSE_BLK5_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_6:
                reg_value &= ~ EFUSE_BLK6_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_7:
                reg_value &= ~ EFUSE_BLK7_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_8:
                reg_value |= EFUSE_BLK8_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_9:
                reg_value |= EFUSE_BLK9_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_10:
                reg_value |= EFUSE_BLK10_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_11:
                reg_value |= EFUSE_BLK11_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_12:
                reg_value |= EFUSE_BLK12_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_13:
                reg_value |= EFUSE_BLK13_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_14:
                reg_value |= EFUSE_BLK14_AUTO_TEST_EN;
                break;
            case EFUSE_PHY_BLK_ID_15:
                reg_value |= EFUSE_BLK15_AUTO_TEST_EN;
                break;    
            default:
                break;
        }
    }

    CHIP_REG_SET (EFUSE_PGM_PARA, reg_value);
}

/******************************************************************************
 * [Description] :                                                            *
 *      enable/disable magic number prot register                             *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      is_enable: enable/disable magic protect                               *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL void EFUSE_PHY_EnableMagicNumProt (
    BOOLEAN is_enable
)
{
    if (is_enable)
    {
        CHIP_REG_SET (EFUSE_MAGIC_NUMBER, 0);
    }
    else
    {
        CHIP_REG_SET (EFUSE_MAGIC_NUMBER, MAGIC_NUM);
    }
}

/******************************************************************************
 * [Description] :                                                            *
 *      Get max bits of the selected efuse blocks, and these blocks are used  *
 *      to store hash/imei                                                    *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *      max bits of HASH/IMEI blocks                                          *
 * [Note]:                                                                    *
 *    1.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 *    2.Each block has 31-bit data.                                           *
 ******************************************************************************/
PUBLIC uint32 EFUSE_PHY_GetAppBitLen (
    EFUSE_BLK_FUN_INDEX blk_hal
)
{
    uint32 min_id = 0;
    uint32 max_id = 0;

    switch (blk_hal)
    {
        case EFUSE_IMEI_START_ID:
            min_id = EFUSE_PHY_BLK_ID_10;
            max_id = EFUSE_PHY_BLK_ID_13;
            break;
        case EFUSE_UID_START_ID:
            min_id = EFUSE_PHY_BLK_ID_0;
            max_id = EFUSE_PHY_BLK_ID_1;
            break;
        case EFUSE_HASH_START_ID:
            min_id = EFUSE_PHY_BLK_ID_9;
            max_id = EFUSE_PHY_BLK_ID_13;
            break;
        case EFUSE_MAX_ID:
            min_id = EFUSE_PHY_MIN_ID;
            max_id = EFUSE_PHY_MAX_ID;
            break;
        default:
            break;
    }

    return (uint32) ( (max_id - min_id + 1) *31);
}

/******************************************************************************
 * [Description] :                                                            *
 *      Get max num of the selected efuse blocks, and these blocks are used   *
 *      to store hash/imei                                                    *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *      max number of HASH/IMEI blocks                                        *
 * [Note]:                                                                    *
 *    1.Hash block is several efuse block, and there index of block are       *
 *      consecutive.                                                          *
 *    2.Each block has 31-bit data.                                           *
 ******************************************************************************/
PUBLIC uint32 EFUSE_PHY_GetAppBlkLen (
    EFUSE_BLK_FUN_INDEX blk_hal
)
{
    uint32 min_id = 0;
    uint32 max_id = 0;

    switch (blk_hal)
    {
        case EFUSE_IMEI_START_ID:
            min_id = EFUSE_PHY_BLK_ID_10;
            max_id = EFUSE_PHY_BLK_ID_13;
            break;
        case EFUSE_UID_START_ID:
            min_id = EFUSE_PHY_BLK_ID_0;
            max_id = EFUSE_PHY_BLK_ID_1;
            break;
        case EFUSE_HASH_START_ID:
            min_id = EFUSE_PHY_BLK_ID_9;
            max_id = EFUSE_PHY_BLK_ID_13;
            break;          
        case EFUSE_MAX_ID:
            min_id = EFUSE_PHY_MIN_ID;
            max_id = EFUSE_PHY_MAX_ID;
            break;

        default:
            break;
    }

    return (max_id - min_id + 1);
}

/******************************************************************************
 * [Description] :                                                            *
 *      parameter is efuse block index, and return efuse block id number      *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:return efuse phy id according to hash id                            *
 *        if fun_index is invalid, the return id is invalid block id.         *
 ******************************************************************************/
PUBLIC uint32 EFUSE_PHY_GetBlkId (
    EFUSE_BLK_FUN_INDEX fun_index
)
{
    uint32 phy_blk_id = EFUSE_PHY_BLK_SUM;//initial a invalid data

    switch (fun_index)
    {
         /* 
            block 0~1:      uid for first 2 block, 
            block 2~8:      calibration for 7 block,
            blcok 9~13:     hash value for 5 block,
            blcok 14~15:    reserved.
         */
        case EFUSE_MIN_ID:      
            phy_blk_id = EFUSE_PHY_MIN_ID;
            break;
        case EFUSE_UID_START_ID:      
            phy_blk_id = EFUSE_PHY_BLK_ID_0;
            break;
        case EFUSE_UID_END_ID: 
            phy_blk_id = EFUSE_PHY_BLK_ID_1;
            break;
        case EFUSE_HASH_START_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_9;
            break;
        case EFUSE_IMEI_START_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_10;
            break;
        case EFUSE_HASH_END_ID:
        case EFUSE_IMEI_END_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_13;
            break;
		case EFUSE_CPLL_LDO_CAL_ID:
		case EFUSE_BT_CAL_ID:
		case EFUSE_IB_CAL_ID:
		case EFUSE_CHRG_CV_CAL_ID:
			phy_blk_id = EFUSE_PHY_BLK_ID_3;
			break;
		case EFUSE_ADC_CAL_HEADMIC_ID:
		case EFUSE_LDODCXO_REF_CAL_ID:
		case EFUSE_LDOA_REF_CAL_ID:
			phy_blk_id = EFUSE_PHY_BLK_ID_4;
			break;
		case EFUSE_ADC_CAL_BIG_ID:
		case EFUSE_ADC_CAL_SMALL_START_ID:
		case EFUSE_LDOB_REF_CAL_ID:
			phy_blk_id = EFUSE_PHY_BLK_ID_5;
			break;
		case EFUSE_ADC_CAL_SMALL_END_ID:
		case EFUSE_RC_CAL_ID:
		case EFUSE_AUD_CAL_ID:
		case EFUSE_LDOCORE_SLP_CAL_ID:
			phy_blk_id = EFUSE_PHY_BLK_ID_6;
			break;
        case EFUSE_MAX_ID:
            phy_blk_id = EFUSE_PHY_MAX_ID;
            break;
        default:
            break;
    }

    return phy_blk_id;
}

/******************************************************************************
 * [Description] :                                                            *
 *      Get max blocks of the bits, and 31 bit is stored in a block           *
 * [Global resource dependence]:                                              *
 *      length : number of bits                                               *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *      max blocks used to stored som bits                                    *
 * [Note]:                                                                    *
 *    1.Each block has 31-bit data.                                           *
 ******************************************************************************/
PUBLIC uint32 EFUSE_PHY_GetBlkNumFromBit (
    uint32 length
)
{
    uint32 blk_num;

    if (length %31)
    {
        blk_num = (length/31) + 1;
    }
    else
    {
        blk_num = length/31;
    }

    return (blk_num);
}

/******************************************************************************
 * [Description] :                                                            *
 *      Get mask data of the bits                                             *
 *      eg:                                                                   *
 *            input         output                                            *
 *              0            0x0                                              *
 *              1            0x01                                             *
 *              2            0x03                                             *
 *              3            0x07                                             *
 *             ...           ...                                              *
 *             ...           ...                                              *
 *             ...           ...                                              *
 *             31        0x7FFF_FFFF                                          *
 *             32        0xFFFF_FFFF                                          *
 * [Global resource dependence]:                                              *
 *      length : number of bits                                               *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *      max blocks used to stored som bits                                    *
 * [Note]:                                                                    *
 *    1.Each block has 31-bit data.                                           *
 ******************************************************************************/
LOCAL uint32 EFUSE_PHY_GetBitMaskData (
    uint32 bit_num
)
{
    uint32 sum = 0;

    if (bit_num >= 32)
    {
        sum = 0xFFFFFFFF;
    }
    else
    {
        while (bit_num)
        {
            sum = sum << 1;
            sum += 0x1;
            bit_num --;
        }
    }

    return sum;
}

/******************************************************************************
 * [Description] :                                                            *
 *      read data directly from an EFuse block                                *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      read efuse data including lock bit                                    *
 ******************************************************************************/
LOCAL uint32 EFUSE_PHY_GetReadData (void)
{
    return CHIP_REG_GET (EFUSE_DATA_RD);
}

/******************************************************************************
 * [Description] :                                                            *
 *      Efuse applys an array. This function returns its address              *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *      address of an uint32 array, and its lenght is equal to num of blocks  *
 * [Note]:                                                                    *
 *   1. If efuse has 8 blocks, and each block has 32-bit length. So length of *
 *      the efuse array is 8.                                                 *
 ******************************************************************************/
PUBLIC uint32 *EFUSE_PHY_GetEfusePtr (void)
{
    return s_efuse_blks_data;
}

/******************************************************************************
 * [Description] :                                                            *
 *      EFuse module reset operation                                          *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      is_enable: enable/disable initialize/uninitialize                     *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 ******************************************************************************/
PUBLIC void EFUSE_PHY_InitReset (void)
{
    uint32 i;

    CHIP_REG_SET (APB_RST0_SET, EFUSE_SOFT_RST);

    for (i = 0; i < 100; i++) {};

    CHIP_REG_SET (APB_RST0_CLR, EFUSE_SOFT_RST);
}

/******************************************************************************
 * [Description] :                                                            *
 *      EFuse open/close operation                                            *
 * [Global resource dependence]:                                              *
 *      s_vcama_org                                                           *
 * [Parameters]:                                                              *
 *      is_enable: enable/disable initialize/uninitialize                     *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 *      power on effuse; open efuse clk; Enable program                       *
 ******************************************************************************/
PUBLIC void EFUSE_PHY_Init (
    BOOLEAN is_enable
)
{
    uint32 reg_value = EFUSE_VDD_ON | EFUSE_CLK_EFS_EN | EFUSE_PGM_EN;
    
    if (is_enable)
    {
        /* a-die control is power on */
        LDO_TurnOnLDO (LDO_LDO_CAMA);

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
        s_vcama_org = LDO_GetVoltLevel (LDO_LDO_CAMA);

        if (LDO_VOLT_LEVEL1 != s_vcama_org)
        {
            LDO_SetVoltLevel (LDO_LDO_CAMA,LDO_VOLT_LEVEL1);
        }
#else
        s_cama_volt = LDO_GetVoltValue (LDO_LDO_CAMA);

        if (EFUSE_VOLT_VAL != s_cama_volt)   //define EFUSE_VOLT_VALUE  2500      //mv
        {
            LDO_SetVoltValue (LDO_LDO_CAMA,EFUSE_VOLT_VAL);
        }
#endif

        //efuse 2.5 power on    LDO_CH_CTRL[14]    0x0
        ANA_REG_AND(ANA_LDO_CH_CTRL, ~(EFS_PROG_SUP_PD));

        /* efuse module clock enable */
        CHIP_REG_SET (APB_EB0_SET, EFUSE_EB);

        /* efuse module is reset */
        EFUSE_PHY_InitReset ();

        /* efuse module config parameters */
        CHIP_REG_OR (EFUSE_PGM_PARA, reg_value);

        OS_TickDelay (2); //delay 2ms
    }
    else
    {
        CHIP_REG_AND (EFUSE_PGM_PARA, ~reg_value);

		//efuse module clock disable
		CHIP_REG_SET (APB_EB0_CLR, EFUSE_EB);

		//Power off
        LDO_TurnOffLDO (LDO_LDO_CAMA);
        
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
        if (LDO_VOLT_LEVEL1 != s_vcama_org)
        {
            LDO_SetVoltLevel (LDO_LDO_CAMA,s_vcama_org);
        }
#else
        if (EFUSE_VOLT_VAL != s_cama_volt)
        {
            LDO_SetVoltValue (LDO_LDO_CAMA,s_cama_volt);
        }
#endif

        //efuse 2.5 power down  LDO_CH_CTRL[14]    0x1
        ANA_REG_OR(ANA_LDO_CH_CTRL, EFS_PROG_SUP_PD);
        
    }

}

/******************************************************************************
 * [Description] :                                                            *
 *      check efuse block id                                                  *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected efuse block                      *
 * [Return]:                                                                  *
 *      TRUE : id is correct.                                                 *
 *      FALSE: id is wrong                                                    *
 * [Note]:                                                                    *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_PHY_IsBlkIdValid (
    uint32 block_id
)
{
    BOOLEAN is_valid = TRUE ;

    if (block_id >= EFUSE_PHY_BLK_SUM)
    {
        is_valid = FALSE;
    }

    return is_valid;
}

/******************************************************************************
 * [Description] :                                                            *
 *      program/read/standy/bist start busy status                            *
 *      if efuse is busy, return TRUE.                                        *
 *      if efuse is idle , return FALSE.                                      *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *   EFUSE_PROG_MODE    : efuse program busy status                           *
 *   EFUSE_READ_MODE    : efuse read start command                            *
 *   EFUSE_STANDBY_MODE : efuse standy by start command                       *
 *   EFUSE_BIST_MODE    : efuse bist start command                            *
 * [Return]:                                                                  *
 *   TRUE  : busy                                                             *
 *   FALSE : idle                                                             *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL BOOLEAN EFUSE_PHY_IsModeBusy (
    EFUSE_MODE_CTL_E mode
)
{
    uint32 reg_value = CHIP_REG_GET (EFUSE_STATUS);
    BOOLEAN is_busy;

    switch (mode)
    {
        case EFUSE_PROG_MODE:
            reg_value = reg_value & EFUSE_PGM_BUSY;
            break;
        case EFUSE_READ_MODE:
            reg_value = reg_value & EFUSE_READ_BUSY;
            break;
        case EFUSE_STANDBY_MODE:
            reg_value = reg_value & EFUSE_STANDBY_BUSY;
            break;
        case EFUSE_BIST_MODE:
            reg_value = reg_value & EFUSE_BIST_BUSY;
            break;
        default:
            reg_value = 0;
            break;
    }

    if (reg_value)
    {
        is_busy = TRUE;
    }
    else
    {
        is_busy = FALSE;
    }

    return is_busy;
}
/******************************************************************************
 * [Description] :                                                            *
 *      bist test fail status                                                 *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 *   TRUE  : bist test fail                                                   *
 *   FALSE : bist test successful                                             *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL EFUSE_RETURN_E EFUSE_PHY_IsBistFail (void)
{
    uint32 reg_value = CHIP_REG_GET (EFUSE_STATUS);
    EFUSE_RETURN_E result;

    reg_value = reg_value & EFUSE_BIST_FAIL;

    if (reg_value)
    {
        result = EFUSE_BIST_TEST_FAIL;
    }
    else
    {
        result = EFUSE_RESULT_SUCCESS;
    }

    return result;
}

/******************************************************************************
 * [Description] :                                                            *
 *      check efuse block protect control bit                                 *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected efuse block                      *
 * [Return]:                                                                  *
 *      TRUE : efuse data is protected                                        *
 *      FALSE: efuse data is not protected                                    *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL uint32 EFUSE_PHY_IsBlkProt (
    uint32 block_id
)
{
    uint32 reg_value = CHIP_REG_GET (EFUSE_BLK_FLAGS);
    BOOLEAN is_prot;

    switch (block_id)
    {
        case EFUSE_PHY_BLK_ID_0:
            reg_value &= EFUSE_BLK0_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_1:
            reg_value &= EFUSE_BLK1_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_2:
            reg_value &= EFUSE_BLK2_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_3:
            reg_value &= EFUSE_BLK3_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_4:
            reg_value &= EFUSE_BLK4_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_5:
            reg_value &= EFUSE_BLK5_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_6:
            reg_value &= EFUSE_BLK6_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_7:
            reg_value &= EFUSE_BLK7_PROT_FLAG;
            break;
		case EFUSE_PHY_BLK_ID_8:
            reg_value &= EFUSE_BLK8_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_9:
            reg_value &= EFUSE_BLK9_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_10:
            reg_value &= EFUSE_BLK10_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_11:
            reg_value &= EFUSE_BLK11_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_12:
            reg_value &= EFUSE_BLK12_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_13:
            reg_value &= EFUSE_BLK13_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_14:
            reg_value &= EFUSE_BLK14_PROT_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_15:
            reg_value &= EFUSE_BLK15_PROT_FLAG;
            break;
        default:
            reg_value = 0;
            break;
    }

    if (reg_value)
    {
        is_prot = TRUE;
    }
    else
    {
        is_prot = FALSE;
    }

    return is_prot;
}


/******************************************************************************
 * [Description] :                                                            *
 *      check efuse block error flag                                          *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected efuse block                      *
 * [Return]:                                                                  *
 *      TRUE : efuse error falg is valid                                      *
 *      FALSE: efuse error flag is invalid                                    *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL uint32 EFUSE_PHY_IsBlkErr (
    uint32 block_id
)
{
    uint32 reg_value = CHIP_REG_GET (EFUSE_BLK_FLAGS);
    BOOLEAN is_error;

    switch (block_id)
    {
        case EFUSE_PHY_BLK_ID_0:
            reg_value &= EFUSE_BLK0_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_1:
            reg_value &= EFUSE_BLK1_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_2:
            reg_value &= EFUSE_BLK2_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_3:
            reg_value &= EFUSE_BLK3_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_4:
            reg_value &= EFUSE_BLK4_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_5:
            reg_value &= EFUSE_BLK5_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_6:
            reg_value &= EFUSE_BLK6_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_7:
            reg_value &= EFUSE_BLK7_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_8:
            reg_value &= EFUSE_BLK8_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_9:
            reg_value &= EFUSE_BLK9_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_10:
            reg_value &= EFUSE_BLK10_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_11:
            reg_value &= EFUSE_BLK11_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_12:
            reg_value &= EFUSE_BLK12_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_13:
            reg_value &= EFUSE_BLK13_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_14:
            reg_value &= EFUSE_BLK14_ERR_FLAG;
            break;
        case EFUSE_PHY_BLK_ID_15:
            reg_value &= EFUSE_BLK15_ERR_FLAG;
            break;    
        default:
            reg_value = 0;
            break;
    }

    if (reg_value)
    {
        is_error = TRUE;
    }
    else
    {
        is_error = FALSE;
    }

    return is_error;
}

/******************************************************************************
 * [Description] :                                                            *
 *      If an efuse block has not been written before, it is not locked.      *
 *      But if it has been written before, it is locke.                       *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected block                            *
 * [Return]:                                                                  *
 *      EFUSE_LOCKED    : block is protected                                  *
 *      EFUSE_NOT_LOCK  : block is not protected                              *
 * [Note]:                                                                    *
 *    1.If an efuse block is written, it would be protected at once.          *
 *    2.If an efuse block is protected, it would not be written any more.     *
 *    3.Each efuse block has only one writting operation.                     *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_PHY_IsLock (
    uint32 block_id
)
{
    uint32 is_busy_sts = 0;
    uint32 old_tick = 0;
    uint32 new_tick = 0;
    EFUSE_RETURN_E result;

    result = EFUSE_RESULT_SUCCESS;

    EFUSE_PHY_ConfigBlkId (block_id);
    EFUSE_PHY_ConfigMode (EFUSE_READ_MODE);

    is_busy_sts = EFUSE_PHY_IsModeBusy (EFUSE_READ_MODE);

    old_tick = SCI_GetTickCount();

    while (TRUE == is_busy_sts)
    {
        is_busy_sts = EFUSE_PHY_IsModeBusy (EFUSE_READ_MODE);
        new_tick = SCI_GetTickCount();

        if ( (new_tick - old_tick) > EFUSE_READ_TIMEOUT)
        {
            break;
        }
    }

    if (TRUE == is_busy_sts)
    {
        result =  EFUSE_LOCKED;
    }
    else
    {
        if (NULL == (EFUSE_LOCK_BIT & EFUSE_PHY_GetReadData ()))
        {
            result = EFUSE_NOT_LOCK;
        }
        else
        {
            result = EFUSE_LOCKED;
        }
    }

    return result;
}

/******************************************************************************
 * [Description] :                                                            *
 *      read data from the selected efuse block                               *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected bllock                           *
 *      des_data_ptr : the pointer of data reading from the selected block    *
 * [Return]:                                                                  *
 *      EFUSE_READ_FAIL        : fail to read data                            *
 *      EFUSE_RESULT_SUCCESS   : successful to read data                      *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Data of the selected efuse block is stored in the 2rd parameter.      *
 *    3.Each block has 31-bit data.                                           *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_PHY_Read (
    uint32 block_id,
    uint32 *des_data_ptr
)
{
    uint32 is_busy_sts = 0;
    uint32 old_tick = 0;
    uint32 new_tick = 0;
    EFUSE_RETURN_E result;

    result = EFUSE_RESULT_SUCCESS;

    EFUSE_PHY_ConfigBlkId (block_id);
    EFUSE_PHY_ConfigMode (EFUSE_READ_MODE);

    is_busy_sts = EFUSE_PHY_IsModeBusy (EFUSE_READ_MODE);

    old_tick = SCI_GetTickCount();

    while (TRUE == is_busy_sts)
    {
        is_busy_sts = EFUSE_PHY_IsModeBusy (EFUSE_READ_MODE);
        new_tick = SCI_GetTickCount();

        if ( (new_tick - old_tick) > EFUSE_READ_TIMEOUT)
        {
            break;
        }
    }

    if (TRUE == is_busy_sts)
    {
        result =  EFUSE_READ_FAIL;
    }
    else
    {
        *des_data_ptr = EFUSE_PHY_GetReadData ();// & EFUSE_BLK_MAX_DATA;
    }

    return result;
}

/******************************************************************************
 * [Description] :                                                            *
 *      read data from the selected efuse block                               *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected bllock                           *
 *      des_data_ptr : the pointer of data reading from the selected block    *
 * [Return]:                                                                  *
 *      EFUSE_READ_FAIL        : fail to read data                            *
 *      EFUSE_RESULT_SUCCESS   : successful to read data                      *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Data of the selected efuse block is stored in the 2rd parameter.      *
 *    3.Each block has 31-bit data.                                           *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_PHY_Read_Ex (
    uint32 block_id,
    uint32 *des_data_ptr
)
{
    uint32 is_busy_sts = 0;
    uint32 old_tick = 0;
    uint32 new_tick = 0;
    EFUSE_RETURN_E result;

    result = EFUSE_RESULT_SUCCESS;

    EFUSE_PHY_ConfigBlkId (block_id);
    EFUSE_PHY_ConfigMode (EFUSE_READ_MODE);

    is_busy_sts = EFUSE_PHY_IsModeBusy (EFUSE_READ_MODE);

    old_tick = SCI_GetTickCount();

    while (TRUE == is_busy_sts)
    {
        is_busy_sts = EFUSE_PHY_IsModeBusy (EFUSE_READ_MODE);
        new_tick = SCI_GetTickCount();

        if ( (new_tick - old_tick) > EFUSE_READ_TIMEOUT)
        {
            break;
        }
    }

    if (TRUE == is_busy_sts)
    {
        result =  EFUSE_READ_FAIL;
    }
    else
    {
        *des_data_ptr = EFUSE_PHY_GetReadData () & EFUSE_BLK_MAX_DATA;
    }

    return result;
}
/******************************************************************************
 * [Description] :                                                            *
 *      Copy num of bit to an array                                           *
 * [Global resource dependence]:                                              *
 *      src_ptr: the pointer of source                                        *
 *      des_ptr: the pointer of destination                                   *
 *      bit_num: number of bits                                               *
 * [Parameters]:                                                              *
 * [Return]:                                                                  *
 * [Note]:                                                                    *
 ******************************************************************************/
LOCAL void EFUSE_PHY_SplitDataCopy (
    uint8  *src_ptr,
    uint8  *des_ptr,
    uint32 bit_num
)
{
    uint32 len,remainder;
    uint32 i;
    uint32 divider;

    divider = 8;//len of uint8
    len = (bit_num + (divider - 1)) /divider;
    remainder = bit_num % divider;

    if (src_ptr != des_ptr)
    {
        for (i = 0; i < len; i++)
        {
            if ( (i == (len - 1)) && (0 != remainder))
            {
                remainder = EFUSE_PHY_GetBitMaskData (remainder);
                * (des_ptr + i) = * (src_ptr + i)  & remainder;
            }
            else
            {
                * (des_ptr + i) = * (src_ptr + i);
            }
        }
    }
    else
    {
        if (0 != remainder)
        {
            remainder = EFUSE_PHY_GetBitMaskData (remainder);
            * (des_ptr + len - 1) = * (src_ptr + len - 1) & remainder;
        }
    }
}

/******************************************************************************
 * [Description] :                                                            *
 *      Reading data is 32-bit length. But each block is 31-bit length.       *
 *      Each block is only one writting operation. For using all 31-bit       *
 *      data, transform 32-bit to 31-bit, and each 31-bit data is to be       *
 *      written for an block.                                                 *
 *  eg: Reading data is a pointer, and its lenght is 4.                       *
 *      Its valid bit are 31*4. Bit 124~128 would be neglected.               *
 *       original       result                                                *
 *      [31 : 0 ]      [30 : 0 ]                                              *
 *      [63 : 32]      [61 :31 ]                                              *
 *      [95 : 64]      [92 :62 ]                                              *
 *      [127: 96]      [123:93 ]                                              *
 *                                                                            *
 * [Global resource dependence]:                                              *
 *      s_efuse_blks_data: reading or writting data of efuse blocks           *
 * [Parameters]:                                                              *
 *      src_dat_ptr: the pointer of original data                             *
 *      des_dat_ptr: the pointer of result data                               *
 *      len_id     : max number of blocks                                     *
 *      blk_num    : max length of bits                                       *
 *      is_split   : direction of splitting data                              *
 *                   TRUE : transforom original data to 31-bit block data     *
 *                   FALSE: transform block data to original data             *
 * [Return]:                                                                  *
 *      If direction is from orginal to result, pointer of 31-bit result is   *
 *      returned. And if it is from result to orignal data, pointer of orignal*
 *      data is returned.                                                     *
 * [Note]:                                                                    *
 *    1.If length of reading data is larger than limit, NULL is returned.     *
 *    2.Reading data would not be changed. And result is stored in return.    *
 ******************************************************************************/
PUBLIC BOOLEAN EFUSE_PHY_SplitData (
    uint8 *src_dat_ptr,
    uint8 *des_dat_ptr,
    uint32 blk_num,
    uint32 bit_num,
    BOOLEAN is_split
)
{
    uint32 i;
    uint32 len;
    uint32 *src_ptr ;
    uint32 *des_ptr ;
    uint32 divider;

    if (blk_num > EFUSE_PHY_BLK_SUM) //max block number
    {
        return FALSE;
    }

    src_ptr = (uint32 *) src_dat_ptr;
    des_ptr = (uint32 *) des_dat_ptr;
    divider = 31;
    len = (bit_num + divider - 1) /divider;

    if (is_split)
    {
        /* TRUE branch: 32-bit to 31-bit data */

        EFUSE_PHY_SplitDataCopy (src_dat_ptr, des_dat_ptr,bit_num);

        for (i = len-1; i < len; i--)
        {
            if (i == 0)
            {
                * (des_ptr) = * (des_ptr) & EFUSE_BLK_MAX_DATA;
            }
            else
            {
                * (des_ptr + i) = ( (* (des_ptr + i) & (EFUSE_BLK_MAX_DATA >> i)) << i)
                                  + (* (des_ptr + i - 1) >> (divider - i + 1));
            }
        }
    }
    else
    {
        /* FALSE branch: 31-bit to 32-bit data */

        for (i = 0; i < len; i++)
        {
            if (len -1 == i)
            {
                * (src_ptr + i) = * (src_ptr + i) >> i;
            }
            else
            {
                * (src_ptr + i) = (* (src_ptr + i) >> i)
                                  + (* (src_ptr + i + 1) << (divider - i));
            }
        }

        EFUSE_PHY_SplitDataCopy (src_dat_ptr, des_dat_ptr,bit_num);
    }

    return TRUE;
}
/******************************************************************************
 * [Description] :                                                            *
 *      write data to the selected efuse block                                *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      block_id : id number of the selected block                            *
 *      write_data : the pointer of data that is to be written                *
 * [Return]:                                                                  *
 *      EFUSE_RESULT_SUCCESS          : successful to write data, and data of *
 *                                      the block is equal to writting data   *
 *      EFUSE_WRITE_SOFT_COMPARE_FAIL : successful to write data, but data of *
 *                                      the block is not equal to writting    *
 *                                      data                                  *
 *      EFUSE_WRITE_HARD_COMPARE_FAIL : fail to write data                    *
 *      EFUSE_WRITE_SOFT_HARD_COMPARE_: fail to write data, and data of the   *
 *                                      block is not equal to writting data   *
 *      EFUSE_WRITE_VERIFY_FAIL       : fail to read after write              *
 * [Note]:                                                                    *
 *    1.Return is the status of reading operation, such as successful or fail.*
 *    2.Each block is only one writing operation.                             *
 *    3.Each block has 31-bit data.                                           *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_PHY_Write (
    uint32 block_id,
    uint32 write_data
)
{
    uint32 sts = 0;
    uint32 read_data;
    uint32 old_tick = 0;
    uint32 new_tick = 0;
    EFUSE_RETURN_E result;

    result = EFUSE_RESULT_SUCCESS;

    EFUSE_PHY_ConfigBlkId (block_id);
	//match the WriteImei tool
	EFUSE_PHY_ConfigWriteData(write_data);
    //EFUSE_PHY_ConfigWriteData (write_data|EFUSE_LOCK_BIT);
    EFUSE_PHY_EnableBlkAutoTest (block_id, TRUE);
    EFUSE_PHY_EnableMagicNumProt (FALSE);
    EFUSE_PHY_ConfigMode (EFUSE_PROG_MODE);

    sts = EFUSE_PHY_IsModeBusy (EFUSE_PROG_MODE);

    old_tick = SCI_GetTickCount();

    while (TRUE == sts)
    {
        sts = EFUSE_PHY_IsModeBusy (EFUSE_PROG_MODE);
        new_tick = SCI_GetTickCount();

        if ( (new_tick - old_tick) > EFUSE_WRITE_TIMEOUT)
        {
            break;
        }
    }

    if (TRUE == sts)
    {
    	SCI_TRACE_LOW("EFUSE_PHY_Write WRITE BUSY");
        return EFUSE_WRITE_FAIL;
    }
    else
    {
        /* get the hardware compare result */
        sts = EFUSE_PHY_IsBlkErr (block_id);

        /* read data and compare */
        result = EFUSE_PHY_Read (block_id, &read_data);

        if (EFUSE_RESULT_SUCCESS != result)
        {
            result = EFUSE_WRITE_VERIFY_FAIL;
        }
        else
        {
            /* compare write and read data */
            if (write_data == read_data)
            {
                if (0 != sts)
                {
                    result = EFUSE_WRITE_HARD_COMPARE_FAIL;
                }
                else
                {
                    result = EFUSE_RESULT_SUCCESS;
                }
            }
            else
            {
                if (0 != sts)
                {
                    result = EFUSE_WRITE_SOFT_HARD_COMPARE_FAIL;
                }
                else
                {
                    result = EFUSE_WRITE_SOFT_COMPARE_FAIL;
                }
            }
        }
    }
	SCI_TRACE_LOW("EFUSE_PHY_Write %d\r\n", result);
    return result;
}

/******************************************************************************
 * [Description] :                                                            *
 *      If all the EFuse block is 0 between start_id and end_id               *
 *      return EFuse_RESULT_SUCCESS,                                          *
 *      else if not all the block data is 0 return EFuse_BIST_FAIL            *
 * [Global resource dependence]:                                              *
 * [Parameters]:                                                              *
 *      start_id : the smalleset selected efuse block id                      *
 *      end_id   : the largest selected efuse block id                        *
 *      end_id should be larger than start_id.                                *
 * [Return]:                                                                  *
 *      EFuse_BIST_FAIL            block is written                           *
 *      EFuse_RESULT_SUCCESS       block is not written                       *
 * [Note]:                                                                    *
 *   1. This mode is for mass production function test,                       *
 *      DON'T set this bit in normal working mode.                            *
 *   2. The function return data is the types of bist test operation, such as *
 *      successful or fail.If the EFuse block is locked the *b_is_lock_ptr is *
 *      TRUE, else is FALSE                                                   *
 ******************************************************************************/
PUBLIC EFUSE_RETURN_E EFUSE_PHY_Bist (
    uint32 start_id,
    uint32 end_id
)
{
    uint32 is_busy = 0;
    uint32 old_tick = 0;
    uint32 new_tick = 0;
    EFUSE_RETURN_E result;

    EFUSE_PHY_ConfigBlkId (start_id);
    EFUSE_PHY_ConfigBistBlkId (start_id, end_id - start_id);
    EFUSE_PHY_EnableBistTest (TRUE);
    EFUSE_PHY_ConfigMode (EFUSE_BIST_MODE);

    is_busy = EFUSE_PHY_IsModeBusy (EFUSE_BIST_MODE);

    old_tick = SCI_GetTickCount();

    while (TRUE == is_busy)
    {
        is_busy = EFUSE_PHY_IsModeBusy (EFUSE_BIST_MODE);
        new_tick = SCI_GetTickCount();

        if ( (new_tick - old_tick) > EFUSE_BIST_TIMEOUT)
        {
            break;
        }
    }

    result =  EFUSE_PHY_IsBistFail ();

    return result;
}
