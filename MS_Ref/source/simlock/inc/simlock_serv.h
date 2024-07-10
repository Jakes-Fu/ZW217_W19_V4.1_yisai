#ifndef _SIM_LOCK_SERV_H
#define _SIM_LOCK_SERV_H

#ifdef __cplusplus
    extern   "C"
    {
#endif
#include "mn_type.h"
#include "rsa.h"
#include "simlock_storage.h"


// AES information
// AES KEY LEN
#define AES_KEY_LEN                     (32)
#define SL_AES_IMEI_LEN                 (16)
#define SL_AES_USER_LEN                 (32)
#define SL_AES_SIGN_LEN                 (64)
#define SL_AES_SIGN_H_LEN               (SL_AES_SIGN_LEN >> 1)

// HUK(random number) definition
#define SL_HUK_LEN                      (8)
#define SL_HUK_H_LEN                    (SL_HUK_LEN >> 1)
#define SL_RAND_NUM_LEN                 (8)
#define SL_RAND_NUM_H_LEN               (SL_RAND_NUM_LEN >> 1)

// SHA information
#define SL_SHA256_LEN                   (32)
#define SL_SHA1_LEN                     (20)

// custimize data
typedef struct {
    uint32 simlock_support;
      uint32 oper_mode;
      uint32 ctrl_key_type;
      uint32 simlock_status;
      uint32 max_num_trials;
    NETWORK_LOCKS_T network_locks;
    NETWORK_SUBSET_LOCKS_T network_subset_locks;
    SP_LOCKS_T sp_locks;
    CORPORATE_LOCKS_T corporate_locks;
    USER_LOCKS_T user_locks;    
} simlock_cust_data_t;

// user data
typedef struct {
    uint32 simlock_status;
    uint32 nck_trials;
    uint32 nck_unlock_time;
    uint32 nsck_trials;
    uint32 nsck_unlock_time;
    uint32 spck_trials;
    uint32 spck_unlock_time;
    uint32 cck_trials;
    uint32 cck_unlock_time;
    uint32 pck_trials;
    uint32 pck_unlock_time;
} simlock_dynamic_data_t;

typedef struct {
    simlock_dynamic_data_t dynamic_data;
    uint32 dummy[4];
} simlock_user_data_t;

// data structure for PC tool or other clients
typedef struct {
    simlock_cust_data_t cust_data;
    simlock_encrypt_keys_t encrypt_keys;
    uint8 sign[SL_RSA_SIGN_LEN];
    uint8 enc_user_data[SL_RSA_N_H_LEN];
} simlock_data_t;

uint32 simlock_set_huk(char* data, uint32 len);

uint32 simlock_facauthpubkey(char* rsa,  uint32 rsa_len, char* sign, uint32 sign_len); 

uint32 simlock_authstart(char* data, uint32* len);

uint32 simlock_authend(char* data, uint32 len);

uint32 simlock_encrypt_keys(char* data, uint32 len, char* encrypt_data, uint32 en_len);

uint32 simlock_sign_keys(uint8* signed_keys, uint32 len);

uint32 simlock_data_process(char* data, uint32 len);

uint32 simlock_data_sha(uint8 *hash);

uint32 simlock_sign_data(uint8* signed_data, uint32 len);

uint32 simlock_get_rsa_pubkey(rsa_context* rsa_ctx_ptr);

//uint32 simlock_imei_process(char* data, uint32 len);

uint32 simlock_data_read(char* data, uint32 len);

#ifdef __cplusplus
    }
#endif

#endif  // End of _SIM_LOCK_H


