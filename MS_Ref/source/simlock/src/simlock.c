/******************************************************************************
 ** File Name:      simlock.c                     *
 ** Author:                                            *
 ** Date:           01/26/2016                                                *
 ** Copyright:          All Rights Reserved.   *                                    *
 ** Description:    simlock security                                                        *
 ** 
 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date                  01.26.2016                   *
 ** version              v2  
 ** Description         add reading interface of simlock data storage, 
 **                         provide the function for simlock data integration RSA verify 
 **                         descrypt the pass word which is designed by customer 
 **---------------------------------------------------------------------------*
 **
 ******************************************************************************/
#include "sci_types.h"
#include "os_api.h"
#include "ps_nvitem_id.h"
#include "nvitem.h"
#include "simlock.h"
#include "simlock_storage.h"
#include "sha1.h"
#include "rsa.h"
#include "flash_drvapi.h"
#include "simlock_serv.h"

#ifdef SIMLOCK_PROTECT_DATA
simlock_protect_data_t s_sl_protect_data = {0};
static uint32 s_protect_data_err_info = 0;
#endif

/*****************************************************************************/
//  Description :get RSA publick key
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE  Simlock_ReadPublicKey(PUBLIC_KEY* public_key,uint32 size)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    if(NULL == public_key||size>sizeof(PUBLIC_KEY) ){
        return SIMLOCK_ERROR;
    }
    result=simlock_read(SIMLOCK_PUBLICKEY,(void*)public_key,0,size);
    //DUMP_DATA((uint32*)public_key, size>>2, "dump public key");
    if(!result){
        ret= SIMLOCK_READ_PUBLICKEY_ERROR;
    }
    return ret;
}
SIMLOCK_ERROR_TYPE  Simlock_ReadPublicKey_BCD(PUBLIC_KEY* public_key,uint32 size)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    if(NULL == public_key||size>sizeof(PUBLIC_KEY) ){
        return SIMLOCK_ERROR;
    }
    result=simlock_read(SIMLOCK_PUBLICKEY,(void*)public_key,0,size);
    simlock_ascii2hex(public_key->n, public_key->n, 256);
    //DUMP_DATA((uint32*)public_key, size>>2, "dump public key");
    if(!result){
        ret= SIMLOCK_READ_PUBLICKEY_ERROR;
    }
    return ret;
}

/*****************************************************************************/
//  Description :write RSA publick key
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE  Simlock_WritePublicKey(PUBLIC_KEY* public_key,uint32 size)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    if(NULL == public_key||size>sizeof(PUBLIC_KEY) ){
        return SIMLOCK_ERROR;
    }
    result=simlock_write(SIMLOCK_PUBLICKEY,(void*)public_key,0,size);
    //DUMP_DATA((uint32*)public_key, size>>2, "dump public key");
    if(!result){
        ret= SIMLOCK_WRITE_PUBLICKEY_ERROR;
    }
    return ret;
}
/*****************************************************************************/
//  Description : Read simlock header (include the version info ) from simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_Readheader(SIMLOCK_HEADER* simlock_header)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    SCI_ASSERT(PNULL != simlock_header);
    simlock_read(HEADER,simlock_header,0,sizeof(SIMLOCK_HEADER));
    if(0 != strncmp(simlock_header->storage_magic,"SIMK",strlen("SIMK")))
    {        
        SCI_TRACE_LOW("[SIM_LOCK]magic num error\n");
        ret=SIMLOCK_READ_HEADER_ERROR;
    }
    return ret;
}
/*****************************************************************************/
//  Description : Read simlock data size from simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadDataSize(uint32* length)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != length);
    result=simlock_read(SIMLOCK_DATA_SIZE,length,0,STOR_SIZE(SIMLOCK_DATA_SIZE));
    if((!result) ||(*length>32*1024)||(*length==0)){
        SCI_TRACE_LOW("Simlock_ReadDataSize read the data len=%x,result=%d",*length,result);
         ret=SIMLOCK_READ_DATASIZE_ERROR;
     }
    return ret;    
}
/*****************************************************************************/
//  Description : Read the designed size of simlock dada from simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadData(uint8*buf,uint32 length)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != buf);
    result=simlock_storage_read(buf, STOR_OFFS(SIMLOCK_DATA), length, 0, length);    
    if(!result){
        ret=SIMLOCK_READ_DATA_ERROR;
    }
    return ret;
}

#ifdef SIMLOCK_PROTECT_DATA
#ifdef RF_NV_PROTECT_SUPPORT
SIMLOCK_ERROR_TYPE Simlock_InitRFProtectData(uint8 *rf_data)
{
    uint32 rf_size = 0;
    NVITEM_ERROR_E nv_ret = NVERR_NONE;
    SIMLOCK_ERROR_TYPE ret = SIMLOCK_SUCCESS;

    /* get RF NV */
    rf_size = EFS_GetItemLength(NV_MODEM_RF_NV);

    ret = EFS_NvitemRead(NV_MODEM_RF_NV, rf_size, rf_data);
    if (nv_ret != NVERR_NONE) {
        SCI_TRACE_LOW("Simlock_InitRFProtectData NV_MODEM_RF_NV failed!\n");
        ret = SIMLOCK_PRE_RF_REVERT_ERROR;
    }
    return ret;
}
#endif
/*****************************************************************************/
// Description :
//
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
simlock_protect_data_t* Simlock_InitProtectData()
{
    uint32 nv_size = 0;
#ifdef RF_NV_PROTECT_SUPPORT
    uint8 *rf_data;
#endif
    NVITEM_ERROR_E ret = NVERR_NONE;

    s_protect_data_err_info = 0;
#ifdef RF_NV_PROTECT_SUPPORT
    /* get RF CRC */
    nv_size = EFS_GetItemLength(NV_MODEM_RF_NV);
    rf_data = SCI_ALLOC_APPZ(SL_RESTORE_RF_LEN);
    if (!rf_data || (nv_size > SL_RESTORE_RF_LEN)) {
        SCI_TRACE_LOW("Simlock_InitProtectData RF malloc failed !\n");
        s_protect_data_err_info |= SIMLOCK_ERR_INIT_MALLOC_RF;
    }
    else {
        SCI_MEMSET(rf_data, 0, SL_RESTORE_RF_LEN);
        ret = EFS_NvitemRead(NV_MODEM_RF_NV, nv_size, rf_data);
        if (ret != NVERR_NONE) {
            SCI_TRACE_LOW("Simlock_InitProtectData NV_MODEM_RF_NV failed !\n");
            s_protect_data_err_info |= SIMLOCK_ERR_INIT_RF;
        }
        else {
            s_sl_protect_data.rf_crc = crc16(0, rf_data, SL_RESTORE_RF_LEN);
#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
            SCI_MEMCPY((uint8*)&s_sl_protect_data.rf_debug[0], (uint8*)rf_data, 8);
#endif
        }
        SCI_FREE(rf_data);
    }

#endif

#ifdef BT_NV_PROTECT_SUPPORT    
#if 1
    /* read BT MAC */
    nv_size = EFS_GetItemLength(NV_BT_CONFIG);
    ret = EFS_NvitemRead(NV_BT_CONFIG, nv_size, (uint8*)&(s_sl_protect_data.bt_addr));
    if (ret != NVERR_NONE) {
        SCI_TRACE_LOW("SIMLOCK: protect_data init BT failed !\n");
        s_protect_data_err_info |= SIMLOCK_ERR_INIT_BT;
    }
#endif

#endif

    /* read IMEI */
    ret = EFS_NvitemRead(NV_IMEI, 8, (uint8*)&s_sl_protect_data.IMEI1);
    if (ret != NVERR_NONE) {
        SCI_TRACE_LOW("Simlock_InitProtectData NV_IMEI failed !\n");
        s_protect_data_err_info |= SIMLOCK_ERR_INIT_IMEI;
    }
#ifdef MULTI_SIM_SYS_DUAL
    ret = EFS_NvitemRead(NV_IMEI1, 8, (uint8*)&s_sl_protect_data.IMEI2);
    if (ret != NVERR_NONE) {
        SCI_TRACE_LOW("Simlock_InitProtectData NV_IMEI2 failed !\n");
        s_protect_data_err_info |= SIMLOCK_ERR_INIT_IMEI2;
    }
#else
    SCI_MEMSET(s_sl_protect_data.IMEI2, 0, 8);
#endif

    if ((SIMLOCK_ERR_INIT_ERR & s_protect_data_err_info) != 0) {
        return PNULL;
    }
    return &s_sl_protect_data;
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
void Simlock_GetCurProtectData(simlock_protect_data_t *buf)
{
    SCI_MEMCPY(buf, &s_sl_protect_data, sizeof(simlock_protect_data_t));
}

/*****************************************************************************/
//  Description : Read the designed simlock protect dada from simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadProtectData(uint8 *buf)
{
    SIMLOCK_ERROR_TYPE ret = SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != buf);
    result = simlock_read(PROTECT_DATA, (void*)buf, 0, STOR_SIZE(PROTECT_DATA));   
    if (!result) {
        ret = SIMLOCK_READ_PROTECT_ERROR;
        s_protect_data_err_info |= SIMLOCK_ERR_READ_PRO;
    }
    return ret;
}

/*****************************************************************************/
//  Description : Append protect data into flash 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_WriteProtectData(uint8 *buf)
{
    SIMLOCK_ERROR_TYPE ret = SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != buf);
    result = simlock_write(PROTECT_DATA, (void*)buf, 0, STOR_SIZE(PROTECT_DATA));
    if (!result) {
        ret = SIMLOCK_WRITE_PROTECT_ERROR;
        s_protect_data_err_info |= SIMLOCK_ERR_WRITE_PRO;
    }
    return ret;
}

/*****************************************************************************/
//  Description : Read the designed simlock protect dada from simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
#ifdef RF_NV_PROTECT_SUPPORT
SIMLOCK_ERROR_TYPE Simlock_ReadRFRestoreData(uint8 *buf)
{
    SIMLOCK_ERROR_TYPE ret = SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != buf);
    result = simlock_read(RF_REVERT_DATA, (void*)buf, 0, STOR_SIZE(RF_REVERT_DATA));   
    if (!result) {
        ret = SIMLOCK_READ_RF_REVERT_ERROR;
        s_protect_data_err_info |= SIMLOCK_ERR_READ_REV;
    }
    return ret;
}

/*****************************************************************************/
//  Description : Append protect data into flash 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_WriteRFRestoreData(uint8 *buf)
{
    SIMLOCK_ERROR_TYPE ret = SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != buf);
    result = simlock_write(RF_REVERT_DATA, (void*)buf, 0, STOR_SIZE(RF_REVERT_DATA));
    if (!result) {
        ret = SIMLOCK_WRITE_RF_REVERT_ERROR;
        s_protect_data_err_info |= SIMLOCK_ERR_WRITE_REV;
    }
    return ret;
}
#endif
/*****************************************************************************/
// Description :   .
//
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
uint32 Simlock_GetProtectDataErrInfo(uint32 *buf)
{
    *buf = s_protect_data_err_info;
    return s_protect_data_err_info;
}

/*****************************************************************************/
// Description :   test UID.
//
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
void Simlock_TestUid(uint8 *uid)
{
    //simlock_get_orig_uid(uid);
    simlock_get_uid(uid);
}

/*****************************************************************************/
// Description : restore the protected NV data, if the NV data was modified.
//
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_RestoreProtectedNV()
{
	uint16 nv_len = 0;
    simlock_protect_data_t flash_data = {0};
    simlock_protect_data_t unwriten_data = {0xFF};
    simlock_protect_data_t *sl_protect = PNULL;
    SIMLOCK_ERROR_TYPE ret = SIMLOCK_SUCCESS;

    sl_protect = Simlock_InitProtectData();
    if (PNULL == sl_protect) {
        return SIMLOCK_READ_PROTECT_NV_ERROR;
    }
    ret = Simlock_ReadProtectData(&flash_data);
    if (SIMLOCK_SUCCESS != ret) {
        return ret;
    }
    SCI_MEMSET(&unwriten_data, 0xFF, sizeof(simlock_protect_data_t));   // Some data will be set as 0x00FF, but not 0xFFFF without this sentence
    if (SCI_MEMCMP(&unwriten_data, &flash_data, sizeof(simlock_protect_data_t)) == 0)
    {   // Simlock data is not writen, return fail
        s_protect_data_err_info |= (SIMLOCK_ERR_RESTORE_BT | SIMLOCK_ERR_RESTORE_SIM);
        return SIMLOCK_READ_PROTECT_ERROR;          
    }
#ifdef BT_NV_PROTECT_SUPPORT   
    /* BT */
    if (SCI_MEMCMP(&(sl_protect->bt_addr), &(flash_data.bt_addr), sizeof(flash_data.bt_addr)) != 0) {
	    nv_len = NVITEM_GetLength(NV_BT_CONFIG);
        if (NVERR_NONE != NVITEM_UpdateNvParam_Protect(NV_BT_CONFIG, nv_len, &(flash_data.bt_addr))) {
            s_protect_data_err_info |= SIMLOCK_ERR_RESTORE_BT;          
            ret = SIMLOCK_PROTECT_NV_ERROR;
        }
    }
 #endif
    /* IMEI */
    if (SCI_MEMCMP(sl_protect->IMEI1, flash_data.IMEI1, sizeof(flash_data.IMEI1)) != 0) {
        if (NVERR_NONE != NVITEM_UpdateNvParam_Protect(NV_IMEI, 8, flash_data.IMEI1)) {
            s_protect_data_err_info |= SIMLOCK_ERR_RESTORE_IMEI1;
            ret = SIMLOCK_PROTECT_NV_ERROR;
        }
    }
    if (SCI_MEMCMP(sl_protect->IMEI2, flash_data.IMEI2, sizeof(flash_data.IMEI2)) != 0) {
        if (NVERR_NONE != NVITEM_UpdateNvParam_Protect(NV_IMEI1, 8, flash_data.IMEI2)) {
            s_protect_data_err_info |= SIMLOCK_ERR_RESTORE_IMEI2;           
            ret = SIMLOCK_PROTECT_NV_ERROR;
        }
    }
#ifdef RF_NV_PROTECT_SUPPORT

    /* RF */
    if (sl_protect->rf_crc != flash_data.rf_crc) {
        uint8 *rf_restore = PNULL; //[SL_RESTORE_RF_LEN]
        rf_restore = SCI_ALLOCAZ(SL_RESTORE_RF_LEN);
        if (PNULL == rf_restore) {
            s_protect_data_err_info |= SIMLOCK_ERR_RESTORE_RF_ALOC;
            return SIMLOCK_PROTECT_NV_ERROR;
        }
        if (SIMLOCK_SUCCESS != Simlock_ReadRFRestoreData(rf_restore)) {
            SCI_FREE(rf_restore);
            return SIMLOCK_PROTECT_NV_ERROR;
        }
	    nv_len = NVITEM_GetLength(NV_MODEM_RF_NV);
        if (NVERR_NONE != EFS_NvitemWrite(NV_MODEM_RF_NV, nv_len, rf_restore, TRUE)) {
            s_protect_data_err_info |= SIMLOCK_ERR_RESTORE_RF;
            ret = SIMLOCK_PROTECT_NV_ERROR;
        }
        SCI_FREE(rf_restore);
    }
#endif

    return ret;
}
#endif

/*****************************************************************************/
//  Description : Write the designed size of simlock dada to simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_WriteDataAndSize(uint8*buf,uint32 size)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(size<32*1024);
    result=simlock_storage_write(buf, STOR_OFFS(SIMLOCK_DATA_SIZE), size, 0, size);    
    if(!result){
        ret=SIMLOCK_WRITE_DATA_ERROR;
    }
    return ret;
}
/*****************************************************************************/
//  Description : Write simlock encrypted dummy to simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_WriteDummy(uint8*dummy,uint32 length)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT((PNULL != dummy)&&(length<=32));
    result=simlock_write(ENCRYPTED_DUMMY,dummy,0,length);
    if(!result){
        ret=SIMLOCK_WRITE_DUMMY_ERROR;
     }
    return ret;    
}
/*****************************************************************************/
//  Description : Read simlock encrypted dummy from simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadDummy(uint8*dummy)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != dummy);
    result=simlock_read(ENCRYPTED_DUMMY,dummy,0,STOR_SIZE(ENCRYPTED_DUMMY));
    if(!result){
        ret=SIMLOCK_READ_DUMMY_ERROR;
     }
    return ret;    
}
/*****************************************************************************/
//  Description : Write simlock encrypted dummy to simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_WriteEncryptUserData(uint8*enc_data)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT((PNULL != enc_data));
    result=simlock_write(ENCRYPTED_USER_DATA,enc_data,0,32);
    if(!result){
        ret=SIMLOCK_WRITE_USERDATA_ERROR;
     }
    return ret;    
}
/*****************************************************************************/
//  Description : Read simlock encrypted dummy from simlock partition 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadEncryptUserData(uint8*enc_data)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != enc_data);
    result=simlock_read(ENCRYPTED_USER_DATA,enc_data,0,STOR_SIZE(ENCRYPTED_USER_DATA));
    if(!result){
         ret=SIMLOCK_READ_USERDATA_ERROR;
     }
    return ret;    
}

/*****************************************************************************/
//  Description : Read  User simlock data from simlock partition  
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadUserData(MN_SIM_LOCK_USER_DATA_T* simlock_user_data)
{
    uint32 data_length;
    simlock_data_t* data_buf=NULL;
    SIMLOCK_ERROR_TYPE ret = SIMLOCK_ERROR;
    
    SCI_ASSERT(PNULL != simlock_user_data);
    Simlock_ReadDataSize(&data_length);
    data_buf=(simlock_data_t*)SCI_ALLOC(data_length);
    if(!data_buf){
        return SIMLOCK_ERROR;
    }
    
    if(SIMLOCK_SUCCESS==Simlock_ReadData((uint8*)data_buf,data_length))
    {
        SCI_MEMCPY((void*)&simlock_user_data->user_locks,(void*)&data_buf->cust_data.user_locks,sizeof(simlock_user_data->user_locks));
        
        SCI_TraceLow("the simlock user crc0=%x,crc1=%x,crc2=%x,crc3=%x",simlock_user_data->encrypted_CRC[0],simlock_user_data->encrypted_CRC[1],
            simlock_user_data->encrypted_CRC[2],simlock_user_data->encrypted_CRC[3]);

        ret = SIMLOCK_SUCCESS;
    }
    else
    {
        ret = SIMLOCK_READ_DATA_ERROR;
    }
    
    if (NULL != data_buf)
    {
        SCI_FREE(data_buf);
    }
    
    return ret;
}
/*****************************************************************************/
//  Description : Read  control key data from simlock partition  
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadControlkeyData(MN_SIM_LOCK_CONTROL_KEY_T* simlock_controlkey_data)
{
    uint32 data_length;
    simlock_data_t* data_buf=NULL;
    SIMLOCK_ERROR_TYPE ret = SIMLOCK_ERROR;
    
    SCI_ASSERT(PNULL != simlock_controlkey_data);
    Simlock_ReadDataSize(&data_length);
    data_buf=(simlock_data_t*)SCI_ALLOC(data_length);
    if(!data_buf){
        return SIMLOCK_ERROR;
    }
    if(SIMLOCK_SUCCESS==Simlock_ReadData((uint8*)data_buf,data_length)){
    
        simlock_controlkey_data->simlock_support=data_buf->cust_data.simlock_support;
        simlock_controlkey_data->oper_mode=data_buf->cust_data.oper_mode;
        simlock_controlkey_data->control_key_type=data_buf->cust_data.ctrl_key_type;
        
        SCI_TraceLow("simlock_on_off=%x,opera_mode=%x,key_type=%x",simlock_controlkey_data->simlock_support,simlock_controlkey_data->oper_mode,
            simlock_controlkey_data->control_key_type);

        ret = SIMLOCK_SUCCESS;
    }
    else
    {
        ret = SIMLOCK_READ_DATA_ERROR;
    }


    if (NULL != data_buf)
    {
        SCI_FREE(data_buf);
    }    
    return ret;    
}
/*****************************************************************************/
//  Description : Read encrypted signature, signature of simlock data (SIM_LOCK_CUSTOMISE_DATA+ 
//SIM_LOCK_USER_DATA+SIM_LOCK_CONTROL_KEY )from simlock partition  
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadDataSignature(uint8* signature)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != signature);
    result=simlock_read(SIGNATURE,signature,0,STOR_SIZE(SIGNATURE));
    //DUMP_DATA(simlock_signature, 128, "dump signature");
    if(!result){
        ret=SIMLOCK_READ_SIGNATURE_ERROR;
    }
    return ret;
}
/*****************************************************************************/
//  Description : write encrypted signature,signature of simlock data (SIM_LOCK_CUSTOMISE_DATA+ 
//SIM_LOCK_USER_DATA+SIM_LOCK_CONTROL_KEY )from simlock partition  
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_WriteDataSignature(uint8* signature)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != signature);
    result=simlock_write(SIGNATURE,signature,0,STOR_SIZE(SIGNATURE));
    //DUMP_DATA(simlock_signature, 128, "dump signature");
    if(!result){
        ret=SIMLOCK_WRITE_SIGNATURE_ERROR;
    }
    return ret;
}
/*****************************************************************************/
//  Description : read encrypted keys (pin/puk simlock password)  
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadKeys(uint8* keys)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != keys);
    result=simlock_read(ENCRYPTED_KEYS,keys,0,STOR_SIZE(ENCRYPTED_KEYS));
    //DUMP_DATA(simlock_signature, 128, "dump signature");
    if(!result){
        ret=SIMLOCK_READ_KEYS_ERROR;
    }
    return ret;
}
/*****************************************************************************/
//  Description : write encrypted keys (pin/puk simlock password) 
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_WriteKeys(uint8*keys)
{
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;
    BOOLEAN result;
    SCI_ASSERT(PNULL != keys);
    result=simlock_write(ENCRYPTED_KEYS,keys,0,STOR_SIZE(ENCRYPTED_KEYS));
    //DUMP_DATA(simlock_signature, 128, "dump signature");
    if(!result){
        ret=SIMLOCK_WRITE_KEYS_ERROR;
    }
    return ret;
}
/*****************************************************************************/
//  Description : Read Customize simlock data  from simlock partition 
//  Global resource dependence : none
//the following 32 bytes used to store the encrypted password
/* uint32                     dummy1;        //the first byte is password length,encrypted password(16 bytes)
                                                       //begin from the second byte 
    uint32                     dummy2;        //
    uint32                     dummy3;        //
    uint32                     dummy4;        //
    uint32                     encrypted_CRC[4];
 /*/ 
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_ReadCustomiseData(MN_SIM_LOCK_CUSTOMIZE_DATA_T* simlock_customize_data)
{
    
    BOOLEAN result;
    uint8 password_len;
    uint8 sha1sum[20];
    uint8 decrypted_dummy[SIMLOCK_STORAGE_DUMMY_LEN];
    uint8 encrypted_dummy[SIMLOCK_STORAGE_DUMMY_LEN];
    uint8 aes_key[SL_SHA256_LEN];
    uint32 data_length;
    simlock_data_t* data_buf=NULL;
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_ERROR;
    
    SCI_ASSERT(PNULL != simlock_customize_data);
    
    if(SIMLOCK_SUCCESS==Simlock_ReadDataSize(&data_length))
    {    
        SCI_TraceLow("simlock data length=%4x",data_length);
        data_buf=(simlock_data_t*)SCI_ALLOC(data_length);
        if(!data_buf){
            return SIMLOCK_ERROR;
        }
        if(SIMLOCK_SUCCESS==Simlock_ReadData((uint8*)data_buf,data_length)){
        
            simlock_customize_data->SIM_lock_status=data_buf->cust_data.simlock_status;
            simlock_customize_data->max_num_trials=data_buf->cust_data.max_num_trials;
            SCI_MEMCPY((void*)&simlock_customize_data->network_locks,(void*)&data_buf->cust_data.network_locks,sizeof(simlock_customize_data->network_locks));
            SCI_MEMCPY((void*)&simlock_customize_data->network_subset_locks,(void*)&data_buf->cust_data.network_subset_locks,sizeof(simlock_customize_data->network_subset_locks));
            SCI_MEMCPY((void*)&simlock_customize_data->SP_locks,(void*)&data_buf->cust_data.sp_locks,sizeof(simlock_customize_data->SP_locks));
            SCI_MEMCPY((void*)&simlock_customize_data->corporate_locks,(void*)&data_buf->cust_data.corporate_locks,sizeof(simlock_customize_data->corporate_locks));
        }
        else
        {
            ret=SIMLOCK_READ_DATA_ERROR;
            goto SL_ReadCustomiseDataEnd; 
        }
        //read encrypted dummy or customise password
        if(SIMLOCK_SUCCESS==Simlock_ReadDummy(encrypted_dummy))
        {
            if(!simlock_get_uid(aes_key)){
                ret=SIMLOCK_READ_UID_ERROR;
                goto SL_ReadCustomiseDataEnd; 
            }
            result=simlock_aes_decrypt(encrypted_dummy, SIMLOCK_STORAGE_DUMMY_LEN, decrypted_dummy,aes_key,SL_SHA256_LEN);
            if( !result){    
                SCI_TRACE_LOW("[SIM_LOCK]simlock password decrypt failed\n");
                ret =SIMLOCK_DECRYPT_ERROR;    
                goto SL_ReadCustomiseDataEnd; 
            }    
            else {
                password_len=(uint8)*decrypted_dummy;
                SCI_MEMCPY((uint8*)&(simlock_customize_data->dummy1),decrypted_dummy,password_len+1);//size and data to customise_data_dummy1~3
                SCI_TRACE_LOW("simlock password decrypt ok\n");
                   ret=SIMLOCK_SUCCESS;
                goto SL_ReadCustomiseDataEnd; 
                
            }  
        }
        else
        {
            SCI_TRACE_LOW("simlock read dummy fail\n");
            ret=SIMLOCK_READ_DUMMY_ERROR;
            goto SL_ReadCustomiseDataEnd; 
            
        }
     }
    else
    {
        SCI_TraceLow("simlock data length read failed");
        ret=SIMLOCK_READ_DATASIZE_ERROR;
        goto SL_ReadCustomiseDataEnd; 
        
    }

SL_ReadCustomiseDataEnd:
    if(data_buf){
        SCI_FREE(data_buf);
    }

    return ret;
}

/*****************************************************************************/
//  Description : simlock data sha1 digest
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
SIMLOCK_ERROR_TYPE Simlock_SHA(uint8 hash_id,uint8 hashlen,uint8* shasum)
{
    uint32 data_length;
    uint8* data_buf=NULL;
    uint16 dalta_len=0;
    MULTIPS_CREATE_PARAM_T sim_cfg;
#ifdef SIMLOCK_PROTECT_DATA
    simlock_protect_data_t *sl_protect = PNULL;
#endif
    SIMLOCK_ERROR_TYPE ret=SIMLOCK_SUCCESS;

    if(SIMLOCK_SUCCESS==Simlock_ReadDataSize(&data_length))
    {
#ifdef SIMLOCK_PROTECT_DATA
        data_buf = SCI_ALLOC_APP(data_length + 8/*sizeof(IMEI)*/
            + sizeof(sim_cfg.sim_slot_map)/*sim_slot_cfg*/
            + STOR_SIZE(ENCRYPTED_DUMMY)
            + sizeof(simlock_encrypt_keys_t)
            + sizeof(simlock_protect_data_t));
#else
        data_buf = SCI_ALLOC_APP(data_length+8/*sizeof(IMEI)*/+sizeof(sim_cfg.sim_slot_map)/*sim_slot_cfg*/+STOR_SIZE(ENCRYPTED_DUMMY)+sizeof(simlock_encrypt_keys_t));
#endif

        SCI_ASSERT(data_buf);
            //read config data
        if(SIMLOCK_SUCCESS==Simlock_ReadData(data_buf+dalta_len, data_length)){
                    dalta_len+=data_length;
                    
            //read encrypted dummy
            ret=Simlock_ReadDummy(data_buf+dalta_len);
            if(ret!=SIMLOCK_SUCCESS)
            {
#ifdef SIMLOCK_PROTECT_DATA
                s_protect_data_err_info |= SIMLOCK_ERR_DUMM_GET;
#endif
                goto end;
            }
            dalta_len+=STOR_SIZE(ENCRYPTED_DUMMY);
            //read imei
            ret=EFS_NvitemRead(NV_IMEI, 8, (data_buf+dalta_len));
            if(ret!=NVERR_NONE)
            {
#ifdef SIMLOCK_PROTECT_DATA
                s_protect_data_err_info |= SIMLOCK_ERR_IMEI_GET;
#endif
                goto end;
            }
            dalta_len+=8/*sizeof(IMEI)*/;
            //read sim cfg
            if(NVERR_NONE != EFS_NvitemRead(NV_SIM_CFG,sizeof(MULTIPS_CREATE_PARAM_T), (uint8*)(&sim_cfg)))
            {
#ifdef SIMLOCK_PROTECT_DATA
                s_protect_data_err_info |= SIMLOCK_ERR_SIMC_GET;
#endif
                goto end;  
            }
               SCI_MEMCPY((void*)(data_buf+dalta_len),(void*)&sim_cfg.sim_slot_map,sizeof(sim_cfg.sim_slot_map));
               //SCI_TRACE_LOW("SIM CFG: SIM_SLOT_MAP=%x",*(uint32*)(data_buf+dalta_len));  
               dalta_len+=sizeof(sim_cfg.sim_slot_map);
            //read keys
            ret=Simlock_ReadKeys((uint8*)(data_buf+dalta_len));
            if(ret!=SIMLOCK_SUCCESS)
            {
#ifdef SIMLOCK_PROTECT_DATA
                s_protect_data_err_info |= SIMLOCK_ERR_KEY_GET;
#endif
                goto end;
            }
            dalta_len+=sizeof(simlock_encrypt_keys_t);

#ifdef SIMLOCK_PROTECT_DATA
            sl_protect = Simlock_InitProtectData();
            if (PNULL == sl_protect) {
                SCI_TRACE_LOW("SIMLOCK SHA: init protect data failed!");
                goto end;
            }
            SCI_MEMCPY((void*)(data_buf + dalta_len), (void*)sl_protect, sizeof(simlock_protect_data_t));
            dalta_len += sizeof(simlock_protect_data_t);
#endif

            switch(hash_id)
            {
                case RSA_SHA1:
                    sha1(data_buf, dalta_len, shasum);
                    break;
                case RSA_SHA256:
                    sha2(data_buf, dalta_len, shasum,0);
                    break;
                default:
                    goto end;
            }
    
        }
        else{
#ifdef SIMLOCK_PROTECT_DATA
            s_protect_data_err_info |= SIMLOCK_ERR_USER_GET;
#endif
             SCI_TRACE_LOW("[%s][%d]simlock data failed\n",__FUNCTION__,__LINE__);
                    ret=SIMLOCK_READ_DATA_ERROR;
        }
        //DUMP_DATA(sha1sum, 20, "dump sha1");  
    }
    else
    {
#ifdef SIMLOCK_PROTECT_DATA
        s_protect_data_err_info |= SIMLOCK_ERR_SIZE_GET;
#endif
        SCI_TRACE_LOW("[%s][%d]simlock data size failed\n",__FUNCTION__,__LINE__);
        ret=SIMLOCK_READ_DATASIZE_ERROR;
    }
    
    end:
    if(data_buf)
    {
        SCI_FREE(data_buf);
    }
    //SCI_TRACE_LOW("[%s][%d]simlock Simlock_SHA1 result=%d\n",__FUNCTION__,__LINE__,ret);
    return ret;
}

/*****************************************************************************/
//  Description : simlock rsa verify
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
BOOLEAN Simlock_Data_Verify(void)
{
    
    uint8 i,hash_id,hashlen;
    uint8 encrypt_user_data[SL_AES_USER_LEN];
    uint8 uid[SL_SHA256_LEN] = {0};
    uint8 rsa_decrypt_sign[SL_RSA_SIGN_LEN] = {0};
    uint8 sha256_data[SL_SHA256_LEN] = {0};
    uint8 user_aes[SL_AES_USER_LEN] = {0};
    uint8 shasum[SL_SHA256_LEN]={0};
    MN_SIM_LOCK_USER_DATA2_T user_data;
    rsa_context rsa_ctx;
    uint8* signature=NULL;
    BOOLEAN ret=FALSE;
    
#if defined(SL_RSA1024_C)
    hash_id=RSA_SHA1;
    hashlen=SL_SHA1_LEN;
#elif defined(SL_RSA2048_C)
    hash_id=RSA_SHA256;
    hashlen=SL_SHA256_LEN;
#endif
    
    signature=SCI_ALLOC_APP(SL_RSA_SIGN_LEN);
    
    if(signature==NULL)
    {
        SCI_TRACE_LOW("Simlock_Data_Verify(): MALLOC FAIL.");
         ret=FALSE;
        goto verify_end;
    }

    //read simlock data signature
    if(SIMLOCK_SUCCESS==Simlock_ReadDataSignature(signature))
    {
        //get the simlock data sha and encrypted user data
        if ( (SIMLOCK_SUCCESS==Simlock_SHA(hash_id,hashlen,shasum)))
       //   && (SIMLOCK_SUCCESS==Simlock_ReadEncryptUserData(encrypt_user_data)))
        {
#ifdef SIMLOCK_PROTECT_DATA_DEBUG_SCN
            SCI_MEMCPY((void*)s_sl_protect_data.debug, (void*)shasum, 20);    //only display first 20 byte for SHA1
#endif

            //DUMP_DATA(sha1sum, 20, "dump sha1");
            SCI_MEMSET(&rsa_ctx, 0, sizeof(rsa_context));
            if(!simlock_get_rsa_pubkey(&rsa_ctx)) {
                SCI_TRACE_LOW("[%s][%d]simlock data read public key failed\n",__FUNCTION__,__LINE__);
                ret=FALSE;
                goto verify_end;   
            }
            // get the uid
            if(!simlock_get_uid(uid)){
                SCI_TRACE_LOW("SIMLOCK: can't get uid!\n");
                ret=FALSE;
                goto verify_end; 
            }                   
#if 0
            // dynamic user data verify
            if(NVERR_NONE != EFS_NvitemRead(NV_SIM_LOCK_USER_DATA_ID,
                                            sizeof(MN_SIM_LOCK_USER_DATA2_T), &user_data))
            {
                SCI_TRACE_LOW("SIMLOCK: get user data from nv failed\n");
                ret=FALSE;
                goto verify_end;    
            }
            
            user_data.SIM_lock_status &=0xff;
            sha2(&(user_data), sizeof(simlock_dynamic_data_t), sha256_data,0);

            //encrypt user data with AES
            if(!simlock_aes_encrypt(sha256_data, SL_SHA256_LEN, user_aes, uid, SL_SHA256_LEN)){
                SCI_TRACE_LOW("SIMLOCK: user data encrypt failed!\n");
                ret=FALSE;
                goto verify_end;    
            }

            for(i = 0; i < SL_AES_USER_LEN; i ++){
                //SCI_TRACE_LOW("SIMLOCK: user_aes[%d]=%x, encrypt_user_data[%d]=%x\n",i,user_aes[i],i,encrypt_user_data[i]);
                if(user_aes[i] != encrypt_user_data[i]){
                    SCI_TRACE_LOW("SIMLOCK: user data verify failed!\n");
                    ret=FALSE;
                    goto verify_end;    
                    }
            }
#endif            
            // decrypt sign
            if(!simlock_aes_decrypt(signature, SL_RSA_SIGN_LEN, rsa_decrypt_sign, uid, SL_SHA256_LEN)){
#ifdef SIMLOCK_PROTECT_DATA
                s_protect_data_err_info |= SIMLOCK_ERR_SIGN_DES;
#endif
                SCI_TRACE_LOW("SIMLOCK: AES decrypt sign failed!\n");
                ret=FALSE;
                goto verify_end; 
            }
            
            //static customise data rsa verify
#if defined(SL_RSA1024_C) 
            if( rsa_pkcs1_verify(&rsa_ctx, RSA_PUBLIC, RSA_SHA1, SL_SHA1_LEN, shasum, rsa_decrypt_sign))
#elif defined(SL_RSA2048_C)
            if( rsa_pkcs1_verify(&rsa_ctx, RSA_PUBLIC, RSA_SHA256, SL_SHA256_LEN, shasum, rsa_decrypt_sign))
#endif 
            {
#ifdef SIMLOCK_PROTECT_DATA
                s_protect_data_err_info |= SIMLOCK_ERR_SIGN_VERI;
#endif
                SCI_TRACE_LOW("[SIM_LOCK]simlock data veryfied failed\n");
                ret=FALSE;
                goto verify_end;    
            }
            else
            {
                SCI_TRACE_LOW("simlock data verify ok\n");
                ret=TRUE;
                goto verify_end;
            }

        }
        else
        {
#ifdef SIMLOCK_PROTECT_DATA
            s_protect_data_err_info |= SIMLOCK_ERR_SHA_GET;
#endif
            SCI_TRACE_LOW("[%s][%d]simlock sha1 calculate fail\n",__FUNCTION__,__LINE__);
            ret=FALSE;
            goto verify_end;        
        }
            
     }
    else
    {
#ifdef SIMLOCK_PROTECT_DATA
        s_protect_data_err_info |= SIMLOCK_ERR_SIGN_GET;
#endif
        SCI_TRACE_LOW("[%s][%d]simlock read signature failed\n",__FUNCTION__,__LINE__);
        ret=FALSE;
        goto verify_end;
    }
    
    verify_end:
    if(signature){
        SCI_FREE(signature);
    }
    return ret;
}
/*****************************************************************************/
// Description :   This function is used to check if the last updated keys from UI or NV can 
//         match the encrypt keys, if match, the protocol stack can boot up 
//         or else, the protocol stack will boot up restricted.
//
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
BOOLEAN Simlock_Check(uint32 type,SIM_LOCK_KEY_T*input_key)
{

    simlock_encrypt_keys_t simlock_keys;
    uint8 encrypt_data[SIM_LOCK_ENCRYPTED_KEY_MAX_LEN] = {0};
    uint8 sha256_data[SIM_LOCK_ENCRYPTED_KEY_MAX_LEN] = {0};
    uint8 uid[SL_SHA256_LEN] = {0};
    uint8* encrypt_key_ptr = NULL;
    uint32 i = 0;
      uint8 key[SIM_LOCK_KEY_MAX_LEN]={0};
    // get the encrypt keys
    if(SIMLOCK_SUCCESS != Simlock_ReadKeys((uint8*)&simlock_keys)){
        SCI_TRACE_LOW("SIMLOCK: get sim key failed\n");
        return FALSE;
    }

    switch(type) {
        case MN_SIM_NETWORK_LOCK:
            encrypt_key_ptr = simlock_keys.pin_keys.nck;
            break;
        case MN_SIM_NETWORK_SUBSET_LOCK:
            encrypt_key_ptr = simlock_keys.pin_keys.nsck;
            break;
        case MN_SIM_SP_LOCK:
            encrypt_key_ptr = simlock_keys.pin_keys.spck;    
            break;
        case MN_SIM_CORPORATE_LOCK:
            encrypt_key_ptr = simlock_keys.pin_keys.cck;
            break;
        case MN_SIM_USER_LOCK:
            encrypt_key_ptr = simlock_keys.pin_keys.pck;
            break;
        default:
            SCI_TRACE_LOW("SIMLOCK: not support this simlock type\n");
            return FALSE;
    }

    // get the uid
    if(!simlock_get_uid(uid)){
        SCI_TRACE_LOW("SIMLOCK: can't get uid!\n");
        return FALSE;
    }
    SCI_MEMCPY(key,input_key->keys,input_key->key_len);
    if(!simlock_aes_encrypt(key, SIM_LOCK_KEY_MAX_LEN, sha256_data, uid, SL_SHA256_LEN)){
        SCI_TRACE_LOW("SIMLOCK: PIN/PUK encrypt failed!\n");
        return FALSE;
    }

    sha2(sha256_data, SIM_LOCK_ENCRYPTED_KEY_MAX_LEN, encrypt_data,0);
    // compare the two pwd
    for(i = 0; i < SIM_LOCK_ENCRYPTED_KEY_MAX_LEN; i ++){
       // SCI_TRACE_LOW("SIMLOCK: key[%d]=%x,sha256_data[%d]=%x,encrypt_data[%d]=%x,  encrypt_key_ptr[%d]=%x!\n",i,key[i],i,sha256_data[i],i,encrypt_data[i],i,encrypt_key_ptr[i]);
        if( encrypt_data[i]!= encrypt_key_ptr[i]){
            SCI_TRACE_LOW("SIMLOCK: unlock key error !\n");
            return FALSE;
        }
    }
    
    return TRUE;
}
/*****************************************************************************/
// Description :   This function is used to update the user data. 
//        (This interface is just for compatible with PS ATC)
//
// Global resource dependence :
// Author :         
// Note :
/*****************************************************************************/
BOOLEAN Simlock_EncryptData_Update(void/* user_data*/)// the last step to protect the simlock status
{
#if 0
    uint8 uid[SL_SHA256_LEN] = {0};
    uint8 sha256_data[SL_SHA256_LEN] = {0};
    uint8 encrypted_user_data[32];
    MN_SIM_LOCK_USER_DATA2_T nv_user_data;
    simlock_user_data_t user_data;

    if(NVERR_NONE != EFS_NvitemRead(NV_SIM_LOCK_USER_DATA_ID,
        sizeof(MN_SIM_LOCK_USER_DATA2_T), (uint8*)&nv_user_data)){
        SCI_TRACE_LOW("SIMLOCK: get user data from nv failed\n");
        return FALSE;
    }

    //get the uid
    if(!simlock_get_uid(uid)){
        SCI_TRACE_LOW("SIMLOCK: can't get uid!\n");
        return FALSE;
    }
    nv_user_data.SIM_lock_status &=0xff;

    
    sha2(&(nv_user_data), sizeof(simlock_dynamic_data_t), sha256_data,0);

    //encrypt user data with AES
    if(!simlock_aes_encrypt(sha256_data, SL_SHA256_LEN, encrypted_user_data, uid, SL_SHA256_LEN)){
        SCI_TRACE_LOW("SIMLOCK: user data encrypt failed!\n");
        return FALSE;
    }
    
    //SCI_TRACE_LOW("SIMLOCK: user data:");
    //simlock_dump(&nv_user_data, sizeof(MN_SIM_LOCK_USER_DATA2_T));

    // write back to nv
    if(SIMLOCK_SUCCESS != Simlock_WriteEncryptUserData(encrypted_user_data)){
        SCI_TRACE_LOW("SIMLOCK: simlock_sign can't be saved to simlock partition !\n");
        return FALSE;
    }
#endif
    return TRUE;
}

uint32 SIMLOCK_GetControlData(uint8 *data, uint16 size)
{
    return 0;
}

