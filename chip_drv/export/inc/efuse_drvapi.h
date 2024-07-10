/*****************************************************************************
 **  File Name:    efuse_drvapi.h                                            *
 **  Author:       windy.wang                                                *
 **  Date:         20/10/2009                                                *
 **  Copyright:    2009 Spreadtrum, Incorporated. All Rights Reserved.       *
 **  Description:  This file defines the basic operation interfaces of       *
 **                EFuse initilize and operation. It provides read and       *
 **                writer interfaces of 0~5 EFUSE_. Efuse 0 for Sn block.    *
 **                Efuse 1 to 4 for Hash blocks. Efuse 5 for control block.  *
 *****************************************************************************
 *****************************************************************************
 **  Edit History                                                            *
 **--------------------------------------------------------------------------*
 **  DATE               Author              Operation                        *
 **  25/06/2012         windy.wang          Create.                          *
 *****************************************************************************/

#ifndef _EFUSE_DRVAPI_H
#define _EFUSE_DRVAPI_H

//copy from bootloader  "hal_efuse_def.h"
#define HAL_EFUSE_SINGLE_BLOCK_COUNT (192)
#define HAL_EFUSE_DOUBLE_BLOCK_COUNT (HAL_EFUSE_SINGLE_BLOCK_COUNT / 2)

#define HAL_EFUSE_SINGLE_BLOCK_LOCK_0 (0)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_1 (1)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_2 (2)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_3 (3)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_4 (4)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_5 (5)
#define HAL_EFUSE_DOUBLE_BLOCK_RMA (3)
#define HAL_EFUSE_DOUBLE_BLOCK_MEDKEY_0 (4)   // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_MEDKEY_3 (7)   // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_HUK_0 (8)      // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_HUK_7 (15)     // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_PRIKEY1_0 (16) // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_PRIKEY1_7 (23) // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_PRIKEY2_0 (24) // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_PRIKEY2_7 (31) // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_SK_0 (32)      // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_SK_7 (39)      // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_0 (40)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_1 (41)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_2 (42)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_3 (43)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_4 (44)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_5 (45)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_6 (46)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_7 (47)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_0 (48)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_1 (49)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_2 (50)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_3 (51)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_4 (52)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_5 (53)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_6 (54)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_7 (55)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_0 (56)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_1 (57)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_2 (58)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_3 (59)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_4 (60)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_5 (61)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_6 (62)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_7 (63)
#define HAL_EFUSE_DOUBLE_BLOCK_NONSECURE_COUNTER_0 (64)
#define HAL_EFUSE_DOUBLE_BLOCK_NONSECURE_COUNTER_7 (71)
#define HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_0 (72)
#define HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_1 (73)
#define HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER2_0 (74)
#define HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER2_1 (75)
#define HAL_EFUSE_DOUBLE_BLOCK_SIMLOCK_COUNTER_0 (76)
#define HAL_EFUSE_DOUBLE_BLOCK_SIMLOCK_COUNTER_1 (77)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_BONDING (80)
#define HAL_EFUSE_DOUBLE_BLOCK_BONDING (89)
#define HAL_EFUSE_DOUBLE_BLOCK_USB (90)
#define HAL_EFUSE_SINGLE_BLOCK_PROD_TYPE_SECTION (189)
#define HAL_EFUSE_SINGLE_BLOCK_UID_0 (190)
#define HAL_EFUSE_SINGLE_BLOCK_UID_1 (191)

#define RDA_EFUSE_UNIQUE_ID_LOW_INDEX HAL_EFUSE_SINGLE_BLOCK_UID_0
#define RDA_EFUSE_UNIQUE_ID_HIGH_INDEX HAL_EFUSE_SINGLE_BLOCK_UID_1

/* the function return value */
typedef enum
{
    EFUSE_RESULT_SUCCESS                 = 0x00,//success to read & write
    EFUSE_READ_FAIL                      = 0x01,//fail to read
    EFUSE_WRITE_FAIL                     = 0x02,//fail to write
    EFUSE_WRITE_SOFT_COMPARE_FAIL        = 0x03,//fail to write
    EFUSE_WRITE_VERIFY_FAIL              = 0x04,
    EFUSE_PARAMETER_ERROR                = 0x05,
    EFUSE_ID_ERROR                       = 0x06,//block id is invalid
    EFUSE_HAS_WRITEED_ERROR              = 0x07,
    EFUSE_PTR_NULL_ERROR                 = 0x08,//pointer is NULL
    EFUSE_PTR_NUM_ERROR                  = 0x09,//bit length error
    EFUSE_WRITE_HARD_COMPARE_FAIL        = 0x0A,//fail to write data
    EFUSE_WRITE_SOFT_HARD_COMPARE_FAIL   = 0x0B,//fail to write
    EFUSE_BIST_TEST_FAIL                 = 0x0C,//efuse data is not zero
    EFUSE_BIST_TIMEOUT                   = 0x0D,
    EFUSE_NOT_LOCK                       = 0x0E,//could be written again
    EFUSE_LOCKED                         = 0x0F //forbidden to be written again
} EFUSE_RETURN_E;

PUBLIC void EFUSE_HAL_Open (void);
PUBLIC void EFUSE_HAL_Close (void);
PUBLIC uint32 EFUSE_HAL_GetAllBlkNum (void);
PUBLIC uint32 EFUSE_HAL_GetHashBlkLen (void);
PUBLIC uint32 EFUSE_HAL_GetIMEABitLen (void);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_IsLock (uint32 block_id);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_IsLockImei (uint32 bit_num);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_IsLockHash (void);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_Read (uint32 block_id,uint32 *des_data_ptr);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_Read_Ex (uint32 block_id,uint32 *des_data_ptr);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadImei (uint8 *des_data_ptr, uint32 bit_num);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadHash (uint32 *des_data_ptr);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_Write (uint32 block_id,uint32 write_data);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_WriteImei (uint8 *des_data_ptr, uint32 bit_num);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_WriteHash (uint32 *des_data_ptr);
PUBLIC BOOLEAN EFUSE_HAL_CalibrationGet (uint32 *p_cal_data);
PUBLIC BOOLEAN EFUSE_HAL_CCCVPointGet (uint32 *p_cal_cccv);
PUBLIC void EFUSE_HAL_Test (void);
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadUid(uint8 *des_data_ptr);
PUBLIC void EFUSE1_HAL_Open (void);
PUBLIC void EFUSE1_HAL_Close (void);
PUBLIC void EFUSE1_HAL_Write (int page_index, uint32 val);
PUBLIC void EFUSE1_HAL_Write_Ex (int page_index, uint32 val, uint8 doubles);
PUBLIC uint32 EFUSE1_HAL_Read (int page_index);
PUBLIC uint32 EFUSE1_HAL_Read_EX (int page_index, uint8 doubles);
PUBLIC void EFUSE1_HAL_enWCNuart (void);
#if defined PLATFORM_UWS6121E
PUBLIC EFUSE_RETURN_E EFUSE_HAL_ReadPTS(uint32 *pts);
#endif
#endif //_EFUSE_DRVAPI_H


