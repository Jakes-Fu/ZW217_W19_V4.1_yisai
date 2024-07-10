
/******************************************************************************
 ** File Name:      simlock.h                                                 *
 ** Author:                                                         *
 ** DATE:           01/26/2016                                                *
 ** Copyright:       All Rights Reserved.         *
 ** Description:    

 /******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 01/02/2015     gloria.he      Create.                                   *
 **                                   *
 ******************************************************************************/
#ifndef _SIM_LOCK_H
#define _SIM_LOCK_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mn_type.h"
#include "simlock_storage.h"
 
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         MOCAL    define                                      *
 **---------------------------------------------------------------------------*/
#ifdef SIMLOCK_PROTECT_DATA
#define SIMLOCK_ERR_INIT_MALLOC_RF  (0x00000001)
#define SIMLOCK_ERR_INIT_RF         (0x00000002)
#define SIMLOCK_ERR_INIT_BT         (0x00000004)
#define SIMLOCK_ERR_INIT_IMEI       (0x00000008)
#define SIMLOCK_ERR_INIT_IMEI2      (0x00000010)
#define SIMLOCK_ERR_READ_PRO        (0x00000020)
#define SIMLOCK_ERR_WRITE_PRO       (0x00000040)
#define SIMLOCK_ERR_SHA_GET         (0x00000080)
#define SIMLOCK_ERR_SIGN_DES        (0x00000100)
#define SIMLOCK_ERR_SIGN_GET        (0x00000200)
#define SIMLOCK_ERR_SIGN_VERI       (0x00000400)
#define SIMLOCK_ERR_DUMM_GET        (0x00000800)
#define SIMLOCK_ERR_IMEI_GET        (0x00001000)
#define SIMLOCK_ERR_SIMC_GET        (0x00002000)
#define SIMLOCK_ERR_KEY_GET         (0x00004000)
#define SIMLOCK_ERR_USER_GET        (0x00008000)
#define SIMLOCK_ERR_SIZE_GET        (0x00010000)
#define SIMLOCK_ERR_READ_REV        (0x00020000)
#define SIMLOCK_ERR_WRITE_REV       (0x00040000)
#define SIMLOCK_ERR_RESTORE_RF_ALOC         (0x00080000)    // restore fail
#define SIMLOCK_ERR_RESTORE_BT              (0x00100000)
#define SIMLOCK_ERR_RESTORE_IMEI1           (0x00200000)
#define SIMLOCK_ERR_RESTORE_IMEI2           (0x00400000)
#define SIMLOCK_ERR_RESTORE_RF              (0x00800000)

#define SIMLOCK_ERR_INIT_ERR    (SIMLOCK_ERR_INIT_MALLOC_RF | SIMLOCK_ERR_INIT_RF | \
                                 SIMLOCK_ERR_INIT_BT | SIMLOCK_ERR_INIT_IMEI | SIMLOCK_ERR_INIT_IMEI2)
#define SIMLOCK_ERR_RESTORE_SIM     (SIMLOCK_ERR_RESTORE_IMEI1 | SIMLOCK_ERR_RESTORE_IMEI2 | SIMLOCK_ERR_RESTORE_RF | SIMLOCK_ERR_RESTORE_RF_ALOC)
#endif


/**---------------------------------------------------------------------------*
 **                         Declare type Variable                                  *
 **---------------------------------------------------------------------------*/
typedef enum _SIMLOCK_ERROR_TYPE
{
	SIMLOCK_SUCCESS=0,
	SIMLOCK_ERROR=1,
	SIMLOCK_READ_DATA_ERROR=2,
	SIMLOCK_WRITE_DATA_ERROR=3,
	SIMLOCK_READ_DATASIZE_ERROR=4,
	SIMLOCK_WRITE_DUMMY_ERROR=5,
	SIMLOCK_READ_DUMMY_ERROR=6,
	SIMLOCK_WRITE_USERDATA_ERROR,
	SIMLOCK_READ_USERDATA_ERROR,
	SIMLOCK_READ_HEADER_ERROR,
	SIMLOCK_READ_PUBLICKEY_ERROR,
	SIMLOCK_WRITE_PUBLICKEY_ERROR,
	SIMLOCK_READ_SIGNATURE_ERROR,
	SIMLOCK_WRITE_SIGNATURE_ERROR,
	SIMLOCK_READ_KEYS_ERROR,
	SIMLOCK_WRITE_KEYS_ERROR,
	SIMLOCK_READ_UID_ERROR,
	SIMLOCK_SHA1_ERROR,
	SIMLOCK_DATA_VERIFY_ERROR,
#ifdef SIMLOCK_PROTECT_DATA
	SIMLOCK_READ_PROTECT_ERROR,
	SIMLOCK_WRITE_PROTECT_ERROR,
	SIMLOCK_READ_RF_REVERT_ERROR,
	SIMLOCK_PRE_RF_REVERT_ERROR,    // prepare rf data for write
	SIMLOCK_WRITE_RF_REVERT_ERROR,
	SIMLOCK_READ_PROTECT_NV_ERROR,
	SIMLOCK_PROTECT_NV_ERROR,
#endif
	SIMLOCK_DECRYPT_ERROR	
}SIMLOCK_ERROR_TYPE;


// Simlock types definition
typedef enum {
	SL_INV,		// invalid type
	SL_NCK, 	// network  control key  type 
	SL_NSCK,	// network subset  control key type
	SL_SPCK,	// service provider control key type
	SL_CCK,		// corporate control key type
	SL_PCK		// personalisation control key type
} simlock_type_e;

// Simlock key types
typedef enum {
	SL_KEY_INV,	// invalid type
	SL_KEY_PIN,	// PIN type
	SL_KEY_PUK	// PUK type
} simlock_key_type_e;
/**---------------------------------------------------------------------------*
 **                         Declare function                                  *
 **---------------------------------------------------------------------------*/
SIMLOCK_ERROR_TYPE Simlock_Readheader(SIMLOCK_HEADER* simlock_header);
SIMLOCK_ERROR_TYPE Simlock_ReadEncryptDummy(uint8* encrypted_signature);

SIMLOCK_ERROR_TYPE  Simlock_ReadPublicKey(PUBLIC_KEY* public_key,uint32 size);
SIMLOCK_ERROR_TYPE  Simlock_ReadPublicKey_BCD(PUBLIC_KEY* public_key,uint32 size);
SIMLOCK_ERROR_TYPE  Simlock_WritePublicKey(PUBLIC_KEY* public_key,uint32 size);
SIMLOCK_ERROR_TYPE Simlock_ReadIMEI(uint8*data,uint8 index);
SIMLOCK_ERROR_TYPE Simlock_WriteIMEI(uint8*data_bcd,uint8 index);
SIMLOCK_ERROR_TYPE Simlock_ReadData(uint8*buf,uint32 length);
SIMLOCK_ERROR_TYPE Simlock_WriteDataAndSize(uint8*buf,uint32 size);
SIMLOCK_ERROR_TYPE Simlock_ReadDataSignature(uint8* signature);
SIMLOCK_ERROR_TYPE Simlock_WriteDataSignature(uint8* signature);
SIMLOCK_ERROR_TYPE Simlock_ReadKeys(uint8* keys);
SIMLOCK_ERROR_TYPE Simlock_WriteKeys(uint8*keys);
SIMLOCK_ERROR_TYPE Simlock_ReadKeysSignature(uint8* keys);
SIMLOCK_ERROR_TYPE Simlock_WriteKeysSignature(uint8*keys_signature);

SIMLOCK_ERROR_TYPE Simlock_ReadCustomiseData(MN_SIM_LOCK_CUSTOMIZE_DATA_T * simlock_customize_data);
SIMLOCK_ERROR_TYPE Simlock_ReadUserData(MN_SIM_LOCK_USER_DATA_T *simlock_user_data);
SIMLOCK_ERROR_TYPE Simlock_ReadControlkeyData(MN_SIM_LOCK_CONTROL_KEY_T* simlock_control_key);
BOOLEAN Simlock_Data_Verify(void);

BOOLEAN Simlock_EncryptData_Update(void/* user_data*/);// the last step to protect the simlock status
BOOLEAN Simlock_Check(uint32 type,SIM_LOCK_KEY_T*key);

#ifdef SIMLOCK_PROTECT_DATA
SIMLOCK_ERROR_TYPE Simlock_ReadProtectData(uint8 *buf);
SIMLOCK_ERROR_TYPE Simlock_WriteProtectData(uint8 *buf);
#ifdef RF_NV_PROTECT_SUPPORT
SIMLOCK_ERROR_TYPE Simlock_ReadRFRestoreData(uint8 *buf);
SIMLOCK_ERROR_TYPE Simlock_WriteRFRestoreData(uint8 *buf);
#endif
simlock_protect_data_t* Simlock_InitProtectData();
#ifdef RF_NV_PROTECT_SUPPORT
SIMLOCK_ERROR_TYPE Simlock_InitRFProtectData(uint8 *rf_data);
#endif
void Simlock_GetCurProtectData(simlock_protect_data_t *buf);
uint32 Simlock_GetProtectDataErrInfo(uint32 *buf);
//void simlock_set_protect_data_debug_info(uint8 *shasum);
void Simlock_TestUid(uint8 *uid);
SIMLOCK_ERROR_TYPE Simlock_RestoreProtectedNV();
#endif

#ifdef __cplusplus
    }
#endif

#endif  // End of _SIM_LOCK_H

