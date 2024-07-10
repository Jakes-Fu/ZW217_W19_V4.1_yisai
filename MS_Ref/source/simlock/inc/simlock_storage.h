/*------------------------------------------------------------------------------
file name     simlock_storage.h
---------------------------------
VERSION    : 1     25-01-2016   
REASON     : Improved SIMLock
REFERENCE :  NEMOG SIMLock handling
DESCRIPTION: Initial version
-------------------------------------------------------------------------------

DESCRIPTION

    Functions for simlock data storage to PS/MN..

-------------------------------------------------------------------------------*/
#ifndef SIMLOCK_STORAGE_H
#define SIMLOCK_STORAGE_H

#include "sci_types.h"
#include "mn_type.h"
#ifdef SIMLOCK_PROTECT_DATA
#include "bt_abs.h"
#include "bt_cfg.h"
#endif

#define STOR_LEN_ANY 1

/* modify begin by wubin.zou, 2019-03-26 */
#define SIMLOCK_DATA_MAX_LEN        (5 * 1024)
/* modify end by wubin.zou, 2019-03-26 */

/* add begin by haibing.chen, 2018-11-30 */
#define SIMLOCK_STORAGE_DUMMY_LEN (32)
/* add end by haibing.chen, 2018-11-30 */

// RSA public key information
//#define SL_RSA1024_C
#define SL_RSA2048_C

#if defined(SL_RSA1024_C)
#define KEY_LEN     (128)
#elif defined(SL_RSA2048_C)
#define KEY_LEN     (256)
#endif

#define SL_RSA_N_LEN                (KEY_LEN<<1)
#define SL_RSA_E_LEN                (6)
#define SL_RSA_N_H_LEN              (SL_RSA_N_LEN >> 1)
#define SL_RSA_PK_LEN               (SL_RSA_N_LEN + SL_RSA_E_LEN)

#define SL_RSA_SIGN_LEN             (KEY_LEN)
#define SL_RSA_KEY_LEN              (KEY_LEN)

#ifdef SIMLOCK_PROTECT_DATA
#define SL_PROTECT_LEN              (160)
#define SL_RESTORE_RF_LEN           (24 * 1024) // equal to NVITEM_MAX_SIZE
#else
#define SL_PROTECT_LEN              (0)
#define SL_RESTORE_RF_LEN           (0)
#endif

typedef struct 
{
    uint8 n[SL_RSA_N_LEN];
    uint8 e[SL_RSA_E_LEN];
    uint8 reserved[2];
}PUBLIC_KEY;



typedef struct
{
    uint8 storage_magic[4];
    uint16 secure_version;
    uint16 config_version;
}SIMLOCK_HEADER;


// Simlock key information
typedef struct {
    uint8 nck[SIM_LOCK_KEY_MAX_LEN];
    uint8 nsck[SIM_LOCK_KEY_MAX_LEN];
    uint8 spck[SIM_LOCK_KEY_MAX_LEN];
    uint8 cck[SIM_LOCK_KEY_MAX_LEN];
    uint8 pck[SIM_LOCK_KEY_MAX_LEN];
} simlock_keyinfo_t;

typedef struct{
    uint8 nck[SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
    uint8 nsck[SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
    uint8 spck[SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
    uint8 cck[SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
    uint8 pck[SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];    
} simlock_encrypt_keyinfo_t;

typedef struct {
    simlock_keyinfo_t pin_keys;
    simlock_keyinfo_t puk_keys;
} simlock_keys_t;

typedef struct {
    simlock_encrypt_keyinfo_t pin_keys;
    simlock_encrypt_keyinfo_t puk_keys;
} simlock_encrypt_keys_t;

#ifdef SIMLOCK_PROTECT_DATA
typedef struct {
#ifdef BT_NV_PROTECT_SUPPORT
    BT_NV_PARAM bt_addr; // 8
#endif
#ifdef RF_NV_PROTECT_SUPPORT

    uint16 rf_crc;  // 2
#endif
    uint8 IMEI1[8];  // BCD 8
    uint8 IMEI2[8];  // BCD 8
  #ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
    uint8 rf_debug[8];  // 2
    uint8 debug[126];
  #else
    uint8 reseve[134];
  #endif
} simlock_protect_data_t;   // 160 byte
#endif

typedef struct 
{
    SIMLOCK_HEADER HEADER;
    PUBLIC_KEY SIMLOCK_PUBLICKEY;
    uint8  SIGNATURE[SL_RSA_SIGN_LEN];  /*aes encrypted RSA SIMLOCK signature*/
    simlock_encrypt_keys_t ENCRYPTED_KEYS;
    uint8  ENCRYPTED_DUMMY[SIMLOCK_STORAGE_DUMMY_LEN]; /*AES encrypted of customise "password"  with the structure: size+encrypted password*/
    uint8  ENCRYPTED_USER_DATA[32];//other encrypted customise config should follow the encrypted dummy when need to be added. 
    uint32 SIMLOCK_DATA_SIZE;
    uint8  SIMLOCK_DATA[SIMLOCK_DATA_MAX_LEN];
#ifdef SIMLOCK_PROTECT_DATA
    simlock_protect_data_t PROTECT_DATA;
#ifdef RF_NV_PROTECT_SUPPORT
    uint8 RF_REVERT_DATA[SL_RESTORE_RF_LEN];
#endif
#endif
} SIMLOCK_STORAGE_STR; 



#define STOR_OFFS(stor_id) \
    ((uint32)( (char*)&((SIMLOCK_STORAGE_STR*)0)->stor_id - (char*)0 ))
#define STOR_SIZE(stor_id) \
    sizeof(((SIMLOCK_STORAGE_STR*)0)->stor_id)
    
#define simlock_read(stor_id, buffer, offset, length) \
        simlock_storage_read(buffer, STOR_OFFS(stor_id), \
                          STOR_SIZE(stor_id), offset, length)
#define simlock_write(stor_id, buffer, offset, length) \
        simlock_storage_write(buffer, STOR_OFFS(stor_id), \
                          STOR_SIZE(stor_id), offset, length)
BOOLEAN simlock_storage_read(void* buffer,
                      uint32 stor_offset, uint32 stor_length,
                      uint32 offset, uint32 length);
BOOLEAN simlock_storage_write(void* buffer,
                      uint32 stor_offset, uint32 stor_length,
                      uint32 offset, uint32 length);
#endif
