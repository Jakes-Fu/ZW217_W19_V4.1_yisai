/******************************************************************************
 **  File Name:    efuse_reg_uix8910.c                                             *
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
#include "efuse_phy_uix8910_pmic.h"
//#include "ldo_drvapi.h"
#include "clock_drvapi.h"

//extern uint32 OS_TickDelay (uint32 ticks);



#define EFUSE_BLK_MAX_DATA 0x7FFFFFFF
#define EFUSE_LOCK_BIT       (BIT_31)

/* add begin by haibing.chen, 2018-10-12*/
LOCAL DEVICE_HANDLE s_pmic_efuse = SCI_NULL;
/* add end by haibing.chen, 2018-10-12 */

LOCAL uint32 s_efuse_blks_data[EFUSE_PHY_BLK_SUM] = {0};




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
    ANA_REG_MSK_OR (EFUSE_ADDR_INDEX , block_id << EFUSE_RW_INDEX_SHIFT, EFUSE_RW_INDEX_MSK);
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
    CHIP_REG_SET (EFUSE_DATA_WR, (data&0xFFFF));
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
        case EFUSE_NORMAL_RD_FLAG_CLR_MODE:
            reg_value = EFUSE_NORMAL_RD_FLAG_CLR;
            break;
//        case EFUSE_BIST_MODE:
//            reg_value = EFUSE_BIST_START;
//            break;
        default:
            reg_value = 0;
            break;
    }

    ANA_REG_SET (EFUSE_MODE_CTRL, reg_value);
}

LOCAL void EFUSE_PHY_EnablePgm (void)
{
    ANA_REG_SET (EFUSE_GLOBAL_CTRL, EFUSE_PGM_EN);
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
        CHIP_REG_SET (EFUSE_MAGIC_NUMBER, PMIC2720_MAGIC_NUM);
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
            phy_blk_id = EFUSE_PHY_BLK_ID_3;
            break;

        case EFUSE_ADC_CAL_HEADMIC_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_4;
            break;
            
        case EFUSE_ADC_CAL_BIG_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_18;
            break;
         case EFUSE_CHRG_CV_CAL_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_13;
            break;
            
        case EFUSE_ADC_CAL_SMALL_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_19;
            break;
            
        case EFUSE_LDOB_REF_CAL_ID:
        case EFUSE_LDODCXO_REF_CAL_ID:
        case EFUSE_LDOA_REF_CAL_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_5;
            break;
            
        case EFUSE_RC_CAL_ID:
        case EFUSE_AUD_CAL_ID:
        case EFUSE_LDOCORE_SLP_CAL_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_6;
            break;

        case EFUSE_RG_LDO_RTC_CAL_ID:
            phy_blk_id = EFUSE_PHY_BLK_ID_16;
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
LOCAL uint16 EFUSE_PHY_GetReadData (void)
{
    return ANA_REG_GET (EFUSE_DATA_RD);
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
    DEVICE_Reset(s_pmic_efuse);      
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
    if (is_enable)
    {
        /* Enable efuse device */    
        SCI_ASSERT (DEVICE_Find ("PMIC_EFUSE", &s_pmic_efuse) == CLOCK_SUCCESS); /*assert verified*/    
        DEVICE_Open(s_pmic_efuse);
    }
    else
    {
        DEVICE_Close(s_pmic_efuse);    
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
    uint32 reg_value = ANA_REG_GET (EFUSE_STATUS);
    BOOLEAN is_busy;

#if 1

    uint32 value=0;
    value = *((volatile unsigned long *)0x50300020);
    SCI_TRACE_LOW("EFUSE_PHY_IsModeBusy, reg_value=0x%x, value=0x%x, mode=%d",  reg_value, value, mode);
#endif

    switch (mode)
    {
        case EFUSE_PROG_MODE:
            reg_value = reg_value & EFUSE_PGM_BUSY;
            break;
        case EFUSE_READ_MODE:
            reg_value = reg_value & EFUSE_READ_BUSY;
            break;
//        case EFUSE_STANDBY_MODE:
//            reg_value = reg_value & EFUSE_STANDBY_BUSY;
//            break;
        case EFUSE_NORMAL_RD_FLAG_CLR_MODE:
            reg_value = reg_value & EFUSE_NORMAL_RD_DONE;
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

LOCAL BOOLEAN EFUSE_PHY_IsBusy (void)
{
    BOOLEAN is_busy = TRUE;
    uint32 status_value = ANA_REG_GET (EFUSE_STATUS);

    if ((status_value & EFUSE_STANDBY_BUSY) != 0)
    {
        is_busy = FALSE;
    }

//    SCI_TRACE_LOW("EFUSE_PHY_IsBusy, status_value=0x%x, is_busy=%d",  status_value, is_busy);

    return is_busy;
}

LOCAL BOOLEAN EFUSE_PHY_IsReadDone (void)
{
    BOOLEAN readDone = FALSE;
    uint32 status_value = ANA_REG_GET (EFUSE_STATUS);

    if ((status_value & EFUSE_NORMAL_RD_DONE) != 0)
    {
        readDone = TRUE;
    }

//    SCI_TRACE_LOW("EFUSE_PHY_IsReadDone, status_value=0x%x, readDone=%d",  status_value, readDone);

    return readDone;
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
    BOOLEAN         read_done = FALSE; 

    result = EFUSE_RESULT_SUCCESS;

//    is_busy_sts = EFUSE_PHY_IsModeBusy(EFUSE_STANDBY_MODE);
    is_busy_sts = EFUSE_PHY_IsBusy();    

    old_tick = SCI_GetTickCount();

    while (TRUE == is_busy_sts)
    {
//        is_busy_sts = EFUSE_PHY_IsModeBusy(EFUSE_STANDBY_MODE);
        is_busy_sts = EFUSE_PHY_IsBusy();    
        new_tick = SCI_GetTickCount();
        
        SCI_TRACE_LOW("EFUSE_PHY_Read, EFUSE IS BUSY");

        if ( (new_tick - old_tick) > EFUSE_READ_TIMEOUT)
        {
            break;
        }
    }

    if (TRUE == is_busy_sts)
    {
        return EFUSE_READ_FAIL;
    }

    /* set read addr */
    EFUSE_PHY_ConfigBlkId (block_id);

    /* start read */
    EFUSE_PHY_ConfigMode(EFUSE_READ_MODE);

    old_tick = SCI_GetTickCount();
    new_tick = old_tick;

    read_done = EFUSE_PHY_IsReadDone();
    while  ((FALSE == read_done)
        && ((new_tick - old_tick) < EFUSE_READ_TIMEOUT))
    {
        SCI_TRACE_LOW("EFUSE_PHY_Read, wait for read DONE");

        read_done = EFUSE_PHY_IsReadDone();
        new_tick = SCI_GetTickCount();
    }

    if (FALSE == read_done)
    {
        return EFUSE_READ_FAIL;
    }
    
    *des_data_ptr = EFUSE_PHY_GetReadData();// & EFUSE_BLK_MAX_DATA;

    /* clear rd done flag */
    EFUSE_PHY_ConfigMode (EFUSE_NORMAL_RD_FLAG_CLR_MODE);

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
return result;
    EFUSE_PHY_ConfigBlkId (block_id);

    EFUSE_PHY_ConfigWriteData(write_data);

    EFUSE_PHY_EnablePgm();

#if 0
    //EFUSE_PHY_ConfigWriteData (write_data|EFUSE_LOCK_BIT);
    EFUSE_PHY_EnableBlkAutoTest (block_id, TRUE);
#endif
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
                result = EFUSE_RESULT_SUCCESS;
            }
            else
            {
                result = EFUSE_WRITE_SOFT_COMPARE_FAIL;
            }
        }
    }
    SCI_TRACE_LOW("EFUSE_PHY_Write %d\r\n", result);
    return result;
}


