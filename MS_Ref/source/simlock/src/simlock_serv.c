#include "rsa.h"
#include "aes.h"
#include "sha1.h"
#include "sha2.h"
#include "simlock.h"
#include "simlock_serv.h"
#include "simlock_storage.h"
#include "nvitem.h"
#include "ps_nvitem_id.h"
#include "efuse_drvapi.h"


#define SL_SSK_STRING    "Secure Storage Key"
#define SL_SSK_STR_LEN    18

//#define EN_SL_SAVE_EFUSE
#define SPK_EFUSE_BLKID 14
 
// {{ debug
static uint8 s_huk[SL_HUK_H_LEN] = {0};
// }}

static int s_identifypass = 0;
static uint32 s_rand = 0;
//static uint32 public_key_efuse=0;

extern uint16 crc16(uint16 crc, uint8 const *buffer, uint32 len);
static void simlock_dump(uint8* pbuf, uint32 len)
{
    uint32 i = 0, j = 0;
    char tmpbuf[128] = {0};
    
    for(i = 0; i < len; i++) 
    {
        sprintf(tmpbuf + j, "%02x ", *(pbuf+i));
        if(i%32 == 31) 
        { 
            SCI_TRACE_LOW("SIMLOCK: %s", tmpbuf);
            memset(tmpbuf, 0, strlen(tmpbuf));
            j = 0;
            continue;
        }

        j = strlen(tmpbuf);
    }

    if(len%32 != 0) {
        SCI_TRACE_LOW("SIMLOCK: %s", tmpbuf);
    }
    
    SCI_TRACE_LOW("\n");
}
static uint32 simlock_rand(void)
{
    uint32 rand_num = 0;

    rand_num = rand();
    
    return rand_num;
}

 uint32 simlock_ascii2hex(char* src, uint8* dst, int len)
{
    int i, j, tmp_len;
    uint8 tmpData;
    char *ascii_data = src;
    uint8 *hex_data = dst;

    for(i = 0; i < len; i++){
        if ((ascii_data[i] >= '0') && (ascii_data[i] <= '9')){
            tmpData = ascii_data[i] - '0';
        } else if ((ascii_data[i] >= 'A') && (ascii_data[i] <= 'F')){ //A....F  
            tmpData = ascii_data[i] - 'A' + 10;
        } else if((ascii_data[i] >= 'a') && (ascii_data[i] <= 'f')){//a....f  
            tmpData = ascii_data[i] - 'a' + 10; 
        } else {
            break;
        }
        ascii_data[i] = tmpData;
    }

    for(tmp_len = 0,j = 0; j < i; j+=2) {
        dst[tmp_len++] = (ascii_data[j]<<4) | ascii_data[j+1];
    }

    return tmp_len;
}

static uint32 simlock_hex2ascii(uint8* src, char*dst, int len)
{
    int i;
    char tmp[3];

    for(i = 0; i < len; i ++) {
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%02x", src[i]);
        strcat(dst, tmp);
    }

    return strlen(dst);
}
/*****************************************************************************/
// Description :  calculate  pubkey crc to efuse                            
//
/*****************************************************************************/
uint16 simlock_crc_pubkey(uint8 *buf, uint32 size)
{
    uint16 pubkey_crc;

    if(!buf || size<260) {
        SCI_TRACE_LOW("SIMLOCK: extract_signed_pubkey parameter error !\n");
        return -1;
    }

    pubkey_crc=crc16(0,buf,size);

    return pubkey_crc;
}
 uint32 simlock_get_rsa_pubkey(rsa_context* rsa_ctx_ptr)
{
    PUBLIC_KEY public_key= {0};
    char  rsa_N[SL_RSA_N_LEN + 1] = {0};
    char  rsa_E[SL_RSA_E_LEN + 1] = {0};
    uint8 rsa_sh256[SL_SHA256_LEN] = {0};
    uint8 aes_key[SL_AES_SIGN_H_LEN] = {0};
    char *tmp = NULL;
    uint8 uid[SL_SHA256_LEN] = {0};
    uint32 pubkey_crc = 0;
    uint32 pubkey_in_efuse = 0;
    uint32 aes_32_pk = 0;
    uint32 ret = 0;

    //get the rsa signature
    if(SIMLOCK_SUCCESS != Simlock_ReadPublicKey(&public_key, 
        sizeof(PUBLIC_KEY))){
        SCI_TRACE_LOW("SIMLOCK: get public key from simlock partition failed\n");
        return 0;
    }
   #if 0 //just for test
       pubkey_crc=simlock_crc_pubkey((uint8*)&public_key ,SL_RSA_PK_LEN);

        public_key_efuse &=0xFFFF;
    SCI_TRACE_LOW("SIMLOCK: public_key_efuse= 0x%x ,  pubkey_crc= 0x%x\n", public_key_efuse, pubkey_crc);
    if ((uint16)public_key_efuse !=  pubkey_crc) {
        SCI_TRACE_LOW("SIMLOCK: SPK has been changed!!!\n");
        //return 0;
    }
#endif    
#ifdef EN_SL_SAVE_EFUSE
    pubkey_crc=(uint32)simlock_crc_pubkey((uint8*)&public_key ,SL_RSA_PK_LEN);
    
    EFUSE_HAL_Open();
    ret = EFUSE_HAL_Read(SPK_EFUSE_BLKID, &pubkey_in_efuse);
    EFUSE_HAL_Close();
    
    if (EFUSE_RESULT_SUCCESS != ret) {
        SCI_TRACE_LOW("SIMLOCK: Efuse read failed!\n");
        return 0;
    }
    pubkey_in_efuse &=0xFFFF;
    pubkey_crc &=0xFFFF;
    //SCI_TRACE_LOW("SIMLOCK: pubkey_crc= 0x%x ,  pubkey_crc= 0x%x\n", pubkey_in_efuse, pubkey_crc);
    if (pubkey_in_efuse !=  pubkey_crc) {
        SCI_TRACE_LOW("SIMLOCK: SPK has been changed!!!\n");
        return 0;
    }
#endif    

    memcpy(rsa_N, public_key.n, SL_RSA_N_LEN);
    memcpy(rsa_E, public_key.e, SL_RSA_E_LEN);
    rsa_init(rsa_ctx_ptr, RSA_PKCS_V15, 0, NULL, NULL);
    rsa_ctx_ptr->len = SL_RSA_KEY_LEN;
    mpi_read_string(&rsa_ctx_ptr->N, 16, rsa_N);
    mpi_read_string(&rsa_ctx_ptr->E, 16, rsa_E);

    return 1;
}
/*****************************************************************************/
// Description :   This function is used to save the random number "HUK". This 
//         number will be used for tool authentication.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
uint32 simlock_set_huk(char* data, uint32 len)
{
    int ret = -1;
    uint8 huk[SL_HUK_H_LEN] = {0};
    uint32 *wtptr;
    uint32 rdval = 0;

    if(!data || len != SL_HUK_LEN) {
        SCI_TRACE_LOW("SIMLOCK: parameter error, len: %d !\n", len);
        return 0;
    }

    ret = simlock_ascii2hex(data, huk, len);

    if(ret != SL_HUK_H_LEN) {
        SCI_TRACE_LOW("SIMLOCK: orial data error, ret: %d !\n", ret);
        return 0;
    }
    memcpy(s_huk, huk, SL_HUK_H_LEN);

    return 1;
}

/*****************************************************************************/
// Description :   This function is used to check and save the RSA public key.
//
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
uint32 simlock_facauthpubkey(char* rsa_pubkey,  uint32 pubkey_len, char* sign, uint32 sign_len)
{
    int i = 0;
    int ret = -1;
    uint8 sign_hex[SL_AES_SIGN_H_LEN] = {0};
    uint8 key_data[SL_HUK_H_LEN + SL_SSK_STR_LEN] = {0};
    uint8 aes_key[SL_AES_SIGN_H_LEN] = {0};
    uint8 rsa_sh256[SL_SHA256_LEN] = {0};
    uint8 sign_now[SL_AES_SIGN_H_LEN] = {0};
    
    uint32 pubkey_crc = 0;
    uint32 pubkey_crc_efuse = 0;


    if(!rsa_pubkey ||pubkey_len != SL_RSA_PK_LEN ||!sign || sign_len != SL_AES_SIGN_LEN) {
        SCI_TRACE_LOW("SIMLOCK: parameter error, %x,%x !\n", pubkey_len, sign_len);
    }
    // get signature
    ret = simlock_ascii2hex(sign, sign_hex, sign_len);
    if(ret != SL_AES_SIGN_H_LEN) {
        SCI_TRACE_LOW("SIMLOCK: sign string error, ret: %d!\n", ret);
        return 0;
    }

    memcpy(key_data, s_huk, SL_HUK_H_LEN);

    memcpy(key_data + SL_HUK_H_LEN, SL_SSK_STRING, SL_SSK_STR_LEN);

    // general AES key
    sha2(key_data, SL_HUK_H_LEN + SL_SSK_STR_LEN, aes_key,0);

    // general data will be encrypted
    sha2(rsa_pubkey, SL_RSA_PK_LEN, rsa_sh256,0);

    // AES encrypt
    if(!simlock_aes_encrypt(rsa_sh256, SL_SHA256_LEN, sign_now, aes_key, SL_AES_SIGN_H_LEN))
    {
        SCI_TRACE_LOW("SIMLOCK: aes encrypt failed \n");
        return 0;
    }

    // compare the two signature
    for(i = 0; i < SL_AES_SIGN_H_LEN; i ++){
        if(sign_hex[i] != sign_now[i]){
            SCI_TRACE_LOW("SIMLOCK: sign error !\n");
            return 0;
        }
    }
#if 0    //just for test 
    public_key_efuse=simlock_crc_pubkey((uint8*)rsa_pubkey ,pubkey_len);
    SCI_TRACE_LOW("SIMLOCK:PUBKEY CRC =%0x!\n", public_key_efuse);
#endif
    
#ifdef EN_SL_SAVE_EFUSE
    pubkey_crc=(uint32)simlock_crc_pubkey((uint8*)rsa_pubkey ,pubkey_len);
    pubkey_crc &=0xFFFF;
    SCI_TRACE_LOW("SIMLOCK:PUBKEY CRC =%0x!\n", pubkey_crc);
    
    EFUSE_HAL_Open();
    ret = EFUSE_HAL_Write(SPK_EFUSE_BLKID, pubkey_crc|BIT_31);
    if(EFUSE_RESULT_SUCCESS != ret) {        
        //SCI_TRACE_LOW("SIMLOCK: EFUSE_LOCKED,  pubkey re-write req!\n");
        EFUSE_HAL_Read(SPK_EFUSE_BLKID, &pubkey_crc_efuse);
        SCI_TRACE_LOW("SIMLOCK: pubkey efuse write, err: %d,pubkey_crc_efuse=0x%0x \n", ret,pubkey_crc_efuse);
        
        pubkey_crc_efuse &=0xFFFF;
        EFUSE_HAL_Close();
        if(pubkey_crc != pubkey_crc_efuse) {
            if(EFUSE_LOCKED == ret){
                SCI_TRACE_LOW("SIMLOCK: efuse locked, puk has been changed \n");
                return 2;
            }else{
                SCI_TRACE_LOW("SIMLOCK: puk efuse written failed \n");
                return 0;
            }
        }
    }else{

        EFUSE_HAL_Close();
    }
   #endif 
    // write back to simlock partition
    if(SIMLOCK_SUCCESS != Simlock_WritePublicKey((PUBLIC_KEY*)rsa_pubkey,pubkey_len)) {
        SCI_TRACE_LOW("SIMLOCK: rsa pk can't be saved !\n");
        return 0;
    }
    return 1;
}

/*****************************************************************************/
// Description :   This function is used to start authentication.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
uint32 simlock_authstart(char* data, uint32* len)
{
    int ret = -1, i = 0;
    unsigned char rsa_cipher_data[SL_RSA_N_LEN >> 1] = {0};
    rsa_context rsa_ctx;

    // reset the auth flag,
    s_identifypass = 0;
    // general the rand number
    s_rand = simlock_rand();

    if(NULL==data){
        SCI_TRACE_LOW("SIMLOCK: parameter error ptr is null");
        return 0;
    }
    if(!simlock_get_rsa_pubkey(&rsa_ctx)) {
        SCI_TRACE_LOW("SIMLOCK: get rsa pubkey failed.\n");        
        return 0;
    }

    ret = rsa_pkcs1_encrypt(&rsa_ctx, RSA_PUBLIC, SL_RAND_NUM_H_LEN, (uint8*)&s_rand, rsa_cipher_data);
    if(ret){
        SCI_TRACE_LOW("SIMLOCK: rsa encrypt failed, ret: %d\n", ret);
        return 0;
    }

    if(*len <  (SL_RSA_N_LEN >> 1)) {
        SCI_TRACE_LOW("SIMLOCK: output buffer is not enough for data, len: %d\n", *len);
        return 0;
    }
    
    ret = simlock_hex2ascii(rsa_cipher_data, data, (SL_RSA_N_LEN >> 1));
    if(ret != SL_RSA_N_LEN){
        SCI_TRACE_LOW("SIMLOCK: rsa data convert to string failed, ret: %d\n", ret);
        return 0;
    }

    *len =  ret;

    return 1;
}

/*****************************************************************************/
// Description :   This function is used to end authentication.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
uint32 simlock_authend(char* data, uint32 len)
{
    int ret = -1;
    int cipher_len = 0;
    uint32 rand = 0;
    unsigned char rsa_cipher_data[SL_RSA_N_LEN >> 1] = {0};
    rsa_context rsa_ctx;

    if(!data || len != SL_RSA_N_LEN) {
        SCI_TRACE_LOW("SIMLOCK: parameter error, len:%d\n", len);
        return 0;
    }

    ret = simlock_ascii2hex(data, rsa_cipher_data, SL_RSA_N_LEN);
    if(ret != (SL_RSA_N_LEN >> 1)) {
        SCI_TRACE_LOW("SIMLOCK: cipher data error, ret: %d\n", ret);
        return 0;
    }

    if(!simlock_get_rsa_pubkey(&rsa_ctx)) {
        SCI_TRACE_LOW("SIMLOCK: get rsa pubkey failed.\n");        
        return 0;
    }

    ret = rsa_pkcs1_decrypt(&rsa_ctx, RSA_PUBLIC, &cipher_len, rsa_cipher_data, (unsigned char*)&rand);
    if(ret) {
        SCI_TRACE_LOW("SIMLOCK: rsa decrypt error , ret: %d!\n", ret);
        return 0;
    }
    
    // compare the two rand number
    if(rand == s_rand) {
        s_identifypass = 1;
    }else{
        SCI_TRACE_LOW("SIMLOCK:auth failed, rand: %x!\n", rand);
        return 0;
    }

    return 1;
}

/*****************************************************************************/
// Description :   This function is used to encrypt the original simlock 
//         keys(PIN/PUK). This cipher data will be returned to PC tools for keys signature.
//
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
uint32 simlock_encrypt_keys(char* data, uint32 len, char* encrypt_data, uint32 en_len)
{
    int i = 0;
    int j=0;
    int ret = 0;
    int data_offset = 0;
    int key_offset = 0;
    int enc_key_offset = 0;
    int dec_len = 0;
    uint8 uid[SL_SHA256_LEN] = {0};
    uint8*enc_data=NULL;
    simlock_keyinfo_t* keys_ptr=NULL;
    simlock_encrypt_keys_t *enc_key =NULL;
    simlock_encrypt_keys_t *sha256_key =NULL;
    rsa_context rsa_ctx;

    if(!s_identifypass) {
        SCI_TRACE_LOW("SIMLOCK:  oper is not authorization");
        return 0;
    }

    if(len != (SL_RSA_N_LEN << 1) ||!encrypt_data) {
        SCI_TRACE_LOW("SIMLOCK: para err,len:%d !\n", len);
        return 0;
    }
    // alloc then assigned to zero, don't change, since there is string like "strcat,strlen" used
    enc_data = SCI_ALLOC_APPZ(SL_RSA_N_H_LEN);
    keys_ptr=SCI_ALLOC_APPZ(sizeof(simlock_keyinfo_t));
    enc_key=SCI_ALLOC_APPZ(sizeof(simlock_encrypt_keys_t));
    sha256_key=SCI_ALLOC_APPZ(sizeof(simlock_encrypt_keys_t));
    
    if(enc_data == NULL||keys_ptr==NULL||enc_key==NULL||sha256_key==NULL)
    {
        SCI_TRACE_LOW("simlock_encrypt_keys(0: MALLOC FAIL.");
        goto failed_handle;
    }
    
    do {
        ret = simlock_ascii2hex(data + data_offset, enc_data, SL_RSA_N_LEN);
        if(ret != SL_RSA_N_H_LEN) {
            SCI_TRACE_LOW("SIMLOCK: data convert to hex failed !\n");
            goto  failed_handle;
        }
        
        ret = simlock_get_rsa_pubkey(&rsa_ctx);
        if(!ret) {
            SCI_TRACE_LOW("SIMLOCK: get rsa pub key failed !\n");
            goto failed_handle;
        }

        ret = rsa_pkcs1_decrypt(&rsa_ctx, RSA_PUBLIC, &dec_len, enc_data, (uint8*)keys_ptr);
        if(ret) {
            SCI_TRACE_LOW("SIMLOCK: decrypt failed!err:%x\n", ret);
            goto failed_handle;
        }
        if(dec_len != sizeof(simlock_keyinfo_t)) {
            SCI_TRACE_LOW("SIMLOCK: wrong keys, dec_len:%d!\n", dec_len);
            goto failed_handle;
        }

        if(!simlock_get_uid(uid)) {
            SCI_TRACE_LOW("SIMLOCK: can't get uid!\n");
            goto failed_handle;
        }

        for(i = 5; i >= 1; i --) {
            if(!simlock_aes_encrypt(keys_ptr->nck + key_offset, SIM_LOCK_KEY_MAX_LEN, 
                enc_key->pin_keys.nck + enc_key_offset, uid, SL_SHA256_LEN)) {
                SCI_TRACE_LOW("SIMLOCK: PIN/PUK encrypt failed!\n");
                goto failed_handle;
            }
            #if 0
            for(j=0;j<16;j++) 
                   {
                SCI_TRACE_LOW("keys_ptr->nck[%d] + %d=%x",j,key_offset,*(keys_ptr->nck + key_offset+j));
                   }
            #endif
            sha2(enc_key->pin_keys.nck + enc_key_offset, SIM_LOCK_ENCRYPTED_KEY_MAX_LEN, 
                sha256_key->pin_keys.nck + enc_key_offset,0);            
                   #if 0
                   for(j=0;j<32;j++) 
                   {
                        SCI_TRACE_LOW("enc_key->pin_keys.nck[%d] + %d=%x",j,enc_key_offset,*(enc_key->pin_keys.nck + enc_key_offset+j));
                        SCI_TRACE_LOW("sha256_key->pin_keys.nck[%d] + %d=%x",j,enc_key_offset,*(sha256_key->pin_keys.nck + enc_key_offset+j));
                   } 
                   #endif
            key_offset += SIM_LOCK_KEY_MAX_LEN;
            enc_key_offset += SIM_LOCK_ENCRYPTED_KEY_MAX_LEN;
        }

        key_offset = 0;
        data_offset += SL_RSA_N_LEN;    
    }while(len != data_offset);
    
    //SCI_TRACE_LOW("SIMLOCK: encrypt key:");
    //simlock_dump((uint8*)sha256_key, sizeof(simlock_encrypt_keys_t));
    
    if(SIMLOCK_SUCCESS!=Simlock_WriteKeys((uint8*)sha256_key)){
        SCI_TRACE_LOW("SIMLOCK: buffer is not enough for pin/puk encrypt data!\n");
        goto failed_handle;
    }

    ret = simlock_hex2ascii((uint8*)sha256_key, encrypt_data, sizeof(simlock_encrypt_keys_t));
    if(ret != (sizeof(simlock_encrypt_keys_t) << 1)){
        SCI_TRACE_LOW("SIMLOCK: encrypt key convert to hex format failed!,ret=%x,\n",ret);
        goto failed_handle;
    }
    end:
          if(enc_data)
            SCI_FREE(enc_data);
        if(keys_ptr)
            SCI_FREE(keys_ptr);
        if(enc_key)
            SCI_FREE(enc_key);
        if(sha256_key)
            SCI_FREE(sha256_key);
    return 1;
    
    failed_handle:
        if(enc_data)
            SCI_FREE(enc_data);
        if(keys_ptr)
            SCI_FREE(keys_ptr);
        if(enc_key)
            SCI_FREE(enc_key);
        if(sha256_key)
            SCI_FREE(sha256_key);
        return 0;
}

/*****************************************************************************/
// Description :   This function is used to update the simlock data (cust data 
//         + user data) and sign the data again with AES keys. Before updating,
//        it will check the integrity and correctness of the data.
//
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
uint32 simlock_data_process(char* data, uint32 len)
{
    rsa_context rsa_ctx;
    int i = 0, key_offset = 0;
    int dec_len = 0;
    int ret = 0;
#if defined(SL_RSA2048_C)
    uint8 sha_digest[SL_SHA256_LEN] = {0};
#elif defined(SL_RSA1024_C)
    uint8 sha_digest[SL_SHA1_LEN] = {0};
#endif
//    uint8 sha1_digest[20] = {0};
    
    uint8 uid[SL_SHA256_LEN] = {0};
    uint8 sha256_data[SL_SHA256_LEN] = {0};
    uint8 dummy_sign[16] = {0};
    uint8 user_data_sign[SL_SHA256_LEN]={0};
    uint8* data_buf=NULL;
    simlock_data_t* simlock_data=NULL;
    simlock_user_data_t sl_user_data;
    MN_SIM_LOCK_USER_DATA2_T *user_data=NULL;
#ifdef SIMLOCK_PROTECT_DATA
    simlock_protect_data_t *sl_protect = NULL;
#ifdef RF_NV_PROTECT_SUPPORT
    uint8 *rf_restore = NULL;
#endif
#endif

    if(!s_identifypass) {
        SCI_TRACE_LOW("SIMLOCK:  oper is not authorization");
        return 0;
    }

    //SCI_TRACE_LOW("SIMLOCK: len: %d, simlock_data_t: %d \n", len, (sizeof(simlock_data_t) << 1) );

    if(len != (sizeof(simlock_data_t) << 1)  || !data) {
        SCI_TRACE_LOW("SIMLOCK: para err,len:%d !\n", len);
        return 0;
    }

    data_buf = SCI_ALLOC_APPZ(sizeof(simlock_data_t)+4/*(data_size)*/);
    user_data=SCI_ALLOC_APPZ(sizeof(MN_SIM_LOCK_USER_DATA2_T));
    if((!data_buf )|| (!user_data)) {
        SCI_TRACE_LOW("SIMLOCK: simlock_data malloc failed !\n");
        goto failed_handle;
    }
#ifdef SIMLOCK_PROTECT_DATA
#ifdef RF_NV_PROTECT_SUPPORT
    rf_restore = SCI_ALLOC_APPZ(SL_RESTORE_RF_LEN);
    if (!rf_restore) {
        SCI_TRACE_LOW("SIMLOCK: rf_restore malloc failed !\n");
        goto failed_handle;
    }
    SCI_MEMSET(rf_restore, 0, SL_RESTORE_RF_LEN);
#endif
#endif
    ret = simlock_ascii2hex(data, (data_buf+/*data_size*/4), len);
    if(ret != sizeof(simlock_data_t)) {
        SCI_TRACE_LOW("SIMLOCK: data convert to hex failed !\n");
        goto failed_handle;
    }    
    *(uint32*)data_buf=ret;
    if(!simlock_get_rsa_pubkey(&rsa_ctx)) {
        SCI_TRACE_LOW("SIMLOCK: get rsa pubkey failed !\n");
        goto failed_handle;
    }
    simlock_data=(simlock_data_t*)((uint8*)data_buf+/*data_size*/4);

    ret = rsa_pkcs1_decrypt(&rsa_ctx, RSA_PUBLIC, &dec_len, simlock_data->enc_user_data, (uint8*)&sl_user_data);
    if(ret) {
        SCI_TRACE_LOW("SIMLOCK: decrypt failed!err:%d\n", ret);
        goto failed_handle;
    }

    if(dec_len != sizeof(simlock_user_data_t)) {
        SCI_TRACE_LOW("SIMLOCK: wrong user data!\n");
        goto failed_handle;
    }
    
#if defined(SL_RSA1024_C) //rsa 1024 verify
    sha1((uint8*)simlock_data, sizeof(simlock_cust_data_t) + sizeof(simlock_encrypt_keys_t), sha_digest);

    SCI_TRACE_LOW("SIMLOCK: digest data:");
    
    ret = rsa_pkcs1_verify(&rsa_ctx, RSA_PUBLIC, RSA_SHA1, SL_SHA1_LEN, sha_digest, simlock_data->sign);
#elif defined(SL_RSA2048_C)    
    sha2((uint8*)simlock_data, sizeof(simlock_cust_data_t) + sizeof(simlock_encrypt_keys_t), sha_digest,0);

    SCI_TRACE_LOW("SIMLOCK: digest data:");
    
    ret = rsa_pkcs1_verify(&rsa_ctx, RSA_PUBLIC, RSA_SHA256, SL_SHA256_LEN, sha_digest, simlock_data->sign);
#endif

    if(ret) {
        SCI_TRACE_LOW("SIMLOCK: verify failed! err:%d\n", ret);
        goto failed_handle;
    }
    #if 0//write data signature
    if(SIMLOCK_SUCCESS!=Simlock_WriteDataSignature(simlock_data->sign)){
           SCI_TRACE_LOW("SIMLOCK: write simlock sign failed! err:%d\n");
              goto failed_handle;       
    }
       #endif    
    //get the uid
    if(!simlock_get_uid(uid)) {
        SCI_TRACE_LOW("SIMLOCK: can't get uid!\n");
        goto failed_handle;
    }
    sl_user_data.dynamic_data.simlock_status=0xFF;
    sha2((uint8*)&sl_user_data.dynamic_data, sizeof(sl_user_data.dynamic_data), sha256_data,0);

    //encrypt user data with AES
    if(!simlock_aes_encrypt(sha256_data, SL_AES_USER_LEN, user_data_sign, uid, SL_SHA256_LEN)){
        SCI_TRACE_LOW("SIMLOCK: signature encrypt failed!\n");
        goto failed_handle;
    }
    if(!simlock_aes_encrypt((uint8*)sl_user_data.dummy, 16, dummy_sign, uid, SL_SHA256_LEN)){
        SCI_TRACE_LOW("SIMLOCK: signature encrypt failed!\n");
        goto failed_handle;
    }
    if(NVERR_NONE != EFS_NvitemRead(NV_SIM_LOCK_USER_DATA_ID,sizeof(MN_SIM_LOCK_USER_DATA2_T), (uint8*)user_data)){
        SCI_TRACE_LOW("SIMLOCK: get user data from nv failed\n");
        goto failed_handle;    
    }
    memcpy((void*)user_data,(void*)&sl_user_data.dynamic_data,sizeof(sl_user_data.dynamic_data));
    if (! NVITEM_UpdateNvParam_Protect(NV_SIM_LOCK_USER_DATA_ID,sizeof(MN_SIM_LOCK_USER_DATA2_T),(void *)user_data)){
        SCI_TRACE_LOW("SIMLOCK: write user data to nv failed\n");
        goto failed_handle;
    }
    if(SIMLOCK_SUCCESS!=Simlock_WriteEncryptUserData(user_data_sign)){
           SCI_TRACE_LOW("SIMLOCK: write encrypted dynamic user data failed!");
              goto failed_handle;       
    }
    //write dynamic password that is encrpted dummy
     if(SIMLOCK_SUCCESS!=Simlock_WriteDummy(dummy_sign,16)){
           SCI_TRACE_LOW("SIMLOCK: write encrypted dummy failed!");
              goto failed_handle;       
    }
#ifdef SIMLOCK_PROTECT_DATA
    // write protect data
    sl_protect = Simlock_InitProtectData();
    if (PNULL == sl_protect) {
        SCI_TRACE_LOW("SIMLOCK: init protect data failed!");
        goto failed_handle;
    }
    if (SIMLOCK_SUCCESS != Simlock_WriteProtectData((uint8*)sl_protect)) {
        SCI_TRACE_LOW("SIMLOCK: write protect data failed!");
        goto failed_handle;
    }
#ifdef RF_NV_PROTECT_SUPPORT
    if (SIMLOCK_SUCCESS != Simlock_InitRFProtectData(rf_restore)) {
        SCI_TRACE_LOW("SIMLOCK: init rf revert data failed!");
        goto failed_handle;
    }
    if (SIMLOCK_SUCCESS != Simlock_WriteRFRestoreData(rf_restore)) {
        SCI_TRACE_LOW("SIMLOCK: write rf data for revert failed!");
        goto failed_handle;
    }
#endif
#endif
    // write data
      if(SIMLOCK_SUCCESS!=Simlock_WriteDataAndSize((uint8*)data_buf, (sizeof(simlock_data_t)+/*data_size*/4))){
             SCI_TRACE_LOW("SIMLOCK: write data failed!");
              goto failed_handle;
      }   
    done:
    if(data_buf){
        SCI_FREE(data_buf);
    }
    if(user_data){
        SCI_FREE(user_data);
    }
#ifdef SIMLOCK_PROTECT_DATA
#ifdef RF_NV_PROTECT_SUPPORT
    if (rf_restore) {
        SCI_FREE(rf_restore);
    }
#endif
#endif
    return 1;    
    failed_handle:
    if(data_buf){
        SCI_FREE(data_buf);
    }
    if(user_data){
        SCI_FREE(user_data);
    }
#ifdef SIMLOCK_PROTECT_DATA
#ifdef RF_NV_PROTECT_SUPPORT
    if (rf_restore) {
        SCI_FREE(rf_restore);
    }
#endif
#endif
    return 0;
}

/*****************************************************************************/
// Description :   tools will get the hash of all simlock data for signature, device calculate the data hash
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
uint32 simlock_data_sha(uint8 *hash)
{
    int ret = 0;
    
    rsa_context rsa_ctx;
        //SCI_TRACE_LOW("SIMLOCK:  simlock_data_sha1 enter");

    if(!s_identifypass) {
        SCI_TRACE_LOW("SIMLOCK:  oper is not authorization");
        return 0;
    }
    
#if defined(SL_RSA1024_C)
    if(SIMLOCK_SUCCESS!=Simlock_SHA(RSA_SHA1,SL_SHA1_LEN,hash)){
        SCI_TRACE_LOW("SIMLOCK: get simlock data hash failed");
        return 0;
    }
#elif defined(SL_RSA2048_C)
    if(SIMLOCK_SUCCESS!=Simlock_SHA(RSA_SHA256,SL_SHA256_LEN,hash)){
        SCI_TRACE_LOW("SIMLOCK: get simlock data hash failed");
        return 0;
    }
#endif
    
    return 1;
}

/*****************************************************************************/
// Description tools write the rsa signature to device 
// the process include the rsa verify before updating the signature;  then update the encrypted signature
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
uint32 simlock_sign_data(uint8* signed_data, uint32 len)
{
    rsa_context rsa_ctx;
#if defined(SL_RSA1024_C)
    uint8 sha_digest[SL_SHA1_LEN]={0};
#elif defined(SL_RSA2048_C)
    uint8 sha_digest[SL_SHA256_LEN]={0};
#endif
//    uint8 sha1_digest[20];    
    uint8 uid[SL_SHA256_LEN] = {0};
    uint8 aes_sign[SL_RSA_SIGN_LEN] = {0};
    int ret=0;
#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
    simlock_protect_data_t sl_protect;
#endif
            
    if(!s_identifypass) {
        SCI_TRACE_LOW("SIMLOCK:  oper is not authorization");
        return 0;
    }

    if(len != (SL_RSA_N_H_LEN) ||!signed_data) {
        SCI_TRACE_LOW("SIMLOCK: para err,len:%d !\n", len);
        return 0;
    }
    if(!simlock_get_rsa_pubkey(&rsa_ctx)) {
        SCI_TRACE_LOW("SIMLOCK: get rsa pubkey failed !\n");
        return 0;
    }
    if(!simlock_data_sha(sha_digest)){
           SCI_TRACE_LOW("SIMLOCK: simlock sha1 calculate failed !\n");
        return 0; ;
    }
      SCI_TRACE_LOW("SIMLOCK: digest data:");
    //simlock_dump(sha1_digest, 20);

#if defined(SL_RSA1024_C)
    ret = rsa_pkcs1_verify(&rsa_ctx, RSA_PUBLIC, RSA_SHA1, SL_SHA1_LEN, sha_digest, signed_data);
#elif defined(SL_RSA2048_C)
    ret = rsa_pkcs1_verify(&rsa_ctx, RSA_PUBLIC, RSA_SHA256, SL_SHA256_LEN, sha_digest, signed_data);
#endif    
    if(ret) {
        SCI_TRACE_LOW("SIMLOCK: verify failed! err:%d\n", ret);
        return 0;
    }
    //get the uid
    if(!simlock_get_uid(uid)) {
        SCI_TRACE_LOW("SIMLOCK: can't get uid!\n");
        return 0;
    }
    //encrypt signature with AES
    if(!simlock_aes_encrypt(signed_data, SL_RSA_SIGN_LEN, aes_sign, uid, SL_SHA256_LEN)){
        SCI_TRACE_LOW("SIMLOCK: signature encrypt failed!\n");
        return 0;
    }
#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
    Simlock_ReadProtectData((uint8 *)&sl_protect);
    memcpy((void*)sl_protect.debug, (void*)sha_digest, 20);    //only display 20 byte for sha1
    Simlock_WriteProtectData((uint8 *)&sl_protect);
#endif
    if(SIMLOCK_SUCCESS!=Simlock_WriteDataSignature(aes_sign)){
        SCI_TRACE_LOW("SIMLOCK: write simlock data signature failed!\n");
        return 0;
    }
    s_identifypass=0;
    return 1;
    
}
