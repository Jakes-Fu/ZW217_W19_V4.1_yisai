#include "system_hal.h"
#include "halo_factory_api.h"
#include "rsa_sprd.h"
#include "license_key.h"
#include "halo_factory.h"

LOCAL uint8 feature_list[F_LIST_LEN] = {0xE1};
LOCAL uint8 verify_signature_64bits(uint8 *uid);
LOCAL uint8 verify_signature_128256bits(uint8 *uid);
LOCAL uint8 Set_featurelist(uint8* list);
LOCAL uint8 Get_featurelist(uint8* list);
LOCAL BOOLEAN HALO_SignWrite(NV_EFUSE_SIGN_DATA_T sign);
LOCAL BOOLEAN HALO_SignRead( NV_EFUSE_SIGN_DATA_T* sign);
extern void halo_set_factory_module(HALO_FACTORY_MODULE_T* mod);
extern uint8 halo_get_shared_data(HALO_CLASS_DATA_T** data, uint32 id);

LOCAL const  HALO_FACTORY_MODULE_T Halo_Factory ={
	.Module_id = MODULE_ID,
	.name = "halo_facotory",
	.verify_License = Verify_Signature,
	.Get_featureList = Get_featurelist,
};

LOCAL struct FAC_CTRL
{
	HALO_CLASS_DATA_T * data_ptr;
}FAC_CTRL = {NULL}; 

PUBLIC uint8 factory_init(void)
{
	uint8 ret = 0;
	halo_set_factory_module(&Halo_Factory);
	return ret;
}


PUBLIC uint8 HALO_factory_CheckLicense(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{ 
	DIAG_READ_LICENSE_T *result = HALO_NULL;
	uint8 i = 0;
	uint8 test_system_info[56] = {0xAA};
	uint8      cnf_len  = 2+56;//56 bytes system info + 2bytes result
	MSG_HEAD_T  *head_ptr = HALO_NULL;
	uint8 errorcode = 0;
	uint8  model_len = 56;//56 bytes system info 
	uint8  signature_len = SIGNLEN;
	unsigned char ret = 1;
	

	ret = HALO_Verify_Signature();
	if(ret == 0x0)
	{	
		errorcode = 0x1;//flash and verify success
	}
    else if (ret == 0xe0)
	{
		errorcode = 0xe0;//read NV failed
	}
	else if (ret == 0xe1)
	{
		 errorcode = 0x0;//signature length =0,not flashed before
	}
	else if (ret == 0xe2)
	{
     
		errorcode = 0xf;//flash and verify failed
	}
	
	PRTF("HALO_Verify_Signature errcode =%d\n",errorcode);

	head_ptr = (MSG_HEAD_T*)HALO_MallocAPP(sizeof(DIAG_READ_LICENSE_T) + sizeof(MSG_HEAD_T)); 
	HALO_ASSERT(head_ptr != HALO_NULL);

	*dest_ptr       = (uint8 *)head_ptr;
	*dest_len_ptr   = sizeof(DIAG_READ_LICENSE_T) + sizeof(MSG_HEAD_T);

	HALO_Memcpy((void *)head_ptr, src_ptr, sizeof(MSG_HEAD_T));
	
	result = (void *)(head_ptr + 1);
	result->errorcode = errorcode;
	result->signature_len =signature_len;
	//HALO_Memcpy((void *)(result->system_info),(void *) test_system_info, sizeof(result->system_info));
	head_ptr->len = *dest_len_ptr;
    return ret;

}

PUBLIC uint8 HALO_factory_ReadUid(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
	DIAG_UID_T *result = HALO_NULL;
	uint8 errcode = 0;
	unsigned char ret = 1;
	uint16    cnf_len     = UIDLEN+1;
	// test_str_ptr[UIDLEN] = "45128986";
	uint8 uid_data[UIDLEN] = {0x0};
	//uint8      temp[4] ={0};
	char*     str_temp = HALO_NULL;
	MSG_HEAD_T  *head_ptr = HALO_NULL;;
	ret = halo_get_shared_data(&FAC_CTRL.data_ptr, Halo_Factory.Module_id);
	PRTF("halo_get_shared_data ret[0x%02X]  reset mode = 0x%02X\n", ret, FAC_CTRL.data_ptr->padding);
	
    ret = HALO_ReadUid(uid_data);
	PRTF_BLOCK("HALO_factory_WriteLicense HALO_ReadUid:",uid_data,sizeof(uid_data));
	if(ret != 0x0)
	{
		PRTF("HALO_ReadUid failed  =%d\n",ret);
		errcode =0xe0; 
	}
	else
	{
		errcode =0x0f; //check UID success
	}
	PRTF("uid read errcode =%d\n",errcode);
		   
	 
	head_ptr = (MSG_HEAD_T*)HALO_MallocAPP( sizeof(DIAG_UID_T) + sizeof(MSG_HEAD_T)); 
	HALO_ASSERT(HALO_NULL != head_ptr);

	*dest_ptr       = (uint8 *)head_ptr;
	*dest_len_ptr   = sizeof(DIAG_UID_T) + sizeof(MSG_HEAD_T);

	HALO_Memcpy((void *)head_ptr, src_ptr, sizeof(MSG_HEAD_T));

	result = (void *)(head_ptr + 1);
	HALO_Memset(result,0,sizeof(DIAG_UID_T));
	result->errorcode = errcode;
	HALO_Memcpy((void *)(result->uid_info),(void *)uid_data, UIDLEN);
	head_ptr->len = *dest_len_ptr;
	return ret;
}

PUBLIC uint8 HALO_factory_WriteLicense(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
	DIAG_WRITE_LICRET_T *result = HALO_NULL;
	//DIAG_WRITE_LICENSE_T input;
	NV_EFUSE_SIGN_DATA_T backup = {0};
	NV_EFUSE_SIGN_DATA_T input = {0};
	unsigned char ret = 1;
	uint16    SignLen =0;
	uint8      errcode = 0;
	uint8     sign_length = 0;
	uint16    cnf_len     = 1;
	uint32    sign1 = 0;
	uint32    sign2 = 0;
	uint8      sign[SIGNLEN]={0};
	uint32    cnf_ptr = 0;
	char*     str_ptr    = HALO_NULL;
	MSG_HEAD_T  *head_ptr = HALO_NULL;

	 SignLen = src_len-sizeof(MSG_HEAD_T)-1;//1 byte length
	 PRTF("Write Efuse Sign Len=%d\n",SignLen);
	 PRTF("Write Efuse printf Len=%d\n",SIGNLEN+sizeof(MSG_HEAD_T)+1);
     PRTF_BLOCK("HALO_factory_WriteLicense check memory",src_ptr,SIGNLEN+sizeof(MSG_HEAD_T)+1);
	 
	 if(SIGNLEN == SignLen)
	 {	
		head_ptr = src_ptr;
        HALO_Memcpy((void *)&sign_length, (void*)(head_ptr+1), sizeof(sign_length));	
		if(sign_length == 0x8)
		{
			input.length = 8;
		}
		else if(sign_length == 0x80)
		{
			input.length = 128;
		}
		else if(sign_length == 0xff)
		{
			input.length = 256;
		}
		else
		{
			errcode=0xe0; //length error
		}
		HALO_Memcpy((void *)&input.signature.data, (void *)((uint8*)(head_ptr+1)+sizeof(sign_length)), SIGNLEN);	
		PRTF_BLOCK("License memory data",input.signature.data,SIGNLEN);
		ret = HALO_SignRead(&backup);
		if(ret == 0)
			errcode = 0xe3;//read NV failed
		ret = HALO_SignWrite(input);
		if(ret == FALSE)
		{
			errcode=0xe1; // write NV faile
		}
		
			//sign1 = sign[3]|(sign[2]<<8&0xff00)|(sign[1]<<16&0xff0000)|(sign[0]<<24&0xff000000); Big endian
			// sign2 = sign[7]|(sign[6]<<8&0xff00)|(sign[5]<<16&0xff0000)|(sign[4]<<24&0xff000000); Big endian
			//sign1 = sign[0]|(sign[1]<<8&0xff00)|(sign[2]<<16&0xff0000)|(sign[3]<<24&0xff000000);
			//sign2 = sign[4]|(sign[5]<<8&0xff00)|(sign[6]<<16&0xff0000)|(sign[7]<<24&0xff000000);
			//PRTF("sign1=%x,sign2=%x\n",sign1,sign2);  
		errcode = 0x0;
	    
		
	 }
	else
	{
	     errcode=0xe2;// src length error
	
	}
	PRTF("sign write errcode =%d\n",errcode);
	
	ret = HALO_Verify_Signature();
	if(ret == 0x0)
	{	
		errcode = 0x0;//flash and verify success
	}
    else if (ret == 0xe0)
	{
		errcode = 0xe3;//read NV failed
	}
	else if (ret == 0xe1)
	{
		 errcode = 0xe4;//signature length =0,not flashed before (NV write internal error )
	}
	else if (ret == 0xe2)
	{
		errcode = 0xe5;//invalid license have been write down, will recover the old license!
		ret = HALO_SignWrite(backup);// recover the old license
		HALO_ASSERT(ret == TRUE);
	}
	
	PRTF("HALO_Verify_Signature errcode =0x%02X\n",errcode);
	
	head_ptr = (MSG_HEAD_T*)HALO_MallocAPP(sizeof(DIAG_WRITE_LICRET_T) + sizeof(MSG_HEAD_T)); 
	HALO_ASSERT(HALO_NULL != head_ptr);

	*dest_ptr       = (uint8 *)head_ptr;
	*dest_len_ptr   = sizeof(DIAG_WRITE_LICRET_T) + sizeof(MSG_HEAD_T);

	HALO_Memcpy((void *)head_ptr, src_ptr, sizeof(MSG_HEAD_T));
    result = (void *)(head_ptr + 1);
	result->errorcode = errcode;
	head_ptr->len = *dest_len_ptr;

}
 /*****************************************************************************/
//  Description :Write sign .
//  Global resource dependence : none
//  Parameter:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HALO_SignWrite(NV_EFUSE_SIGN_DATA_T sign)
{
    HALO_NVITEM_E result = HALO_NVERR_NONE;
    NV_EFUSE_SIGN_DATA_T sign_temp = {0};
   
    sign_temp = sign;
    result = HALO_NVITEM_Write(LICENSE_INDEX, sizeof(NV_EFUSE_SIGN_DATA_T),
                             (void *)&sign_temp);

    if(result == HALO_NVERR_NONE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description :Read  sign.
//  Global resource dependence : none
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HALO_SignRead( NV_EFUSE_SIGN_DATA_T* sign)
{
    HALO_NVITEM_E  result = HALO_NVERR_NONE;

    if(PNULL == sign)
        return FALSE;

    result = HALO_NVITEM_Read(LICENSE_INDEX,
                            sizeof(NV_EFUSE_SIGN_DATA_T),
                            (uint8 *)sign);

    if(result == HALO_NVERR_NONE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


LOCAL uint8 HALO_ReadUid(uint8 *uid)
{
 	return _HALO_ReadUid(uid);
}


LOCAL uint8 Set_featurelist(uint8* list)
{
	uint16 i =0;
	HALO_ASSERT(list != NULL);
	HALO_Memcpy(feature_list, list, F_LIST_LEN);
    feature_encode(feature_list,F_LIST_LEN,feature_list, feature_key,2);
	return 0;
}

LOCAL uint8 Get_featurelist(uint8* list)
{
	uint16 i = 0;
	uint8 ret =0;
	ret = feature_decode(feature_list,F_LIST_LEN,feature_list, feature_key,2);
	
		HALO_ASSERT(list != NULL);
#if(F_LIST_LEN == 1)
		HALO_Memcpy(list, feature_list, 1);
#elif(F_LIST_LEN == 84)
		HALO_Memcpy(list, feature_list, F_LIST_LEN-20);
#endif
	if (ret == true)
	{
		PRTF("Get_featurelist success!\n"); 
	}
	else if (ret == 0xe1 || ret == 0xe2)
	{
		PRTF("Get_featurelist internal error!\n"); 	
	}
	return ret;
}

 LOCAL uint8  Verify_Signature()
{
	uint8 uid[UIDLEN] = {0x0};
	uint8 ret =0;
	HALO_ReadUid(uid);
#if (SIGNLEN == 8)
	ret = verify_signature_64bits(uid);
#elif (SIGNLEN == 128 || SIGNLEN == 256)
    ret = verify_signature_128256bits(uid);
#endif
	return ret;	
}

LOCAL uint8 regulate_uid(uint32 uid0,uint32 uid1, uint8* uid)
{
	uid[0]= uid0>>24&0xff;
	uid[1]= uid0>>16&0xff;
	uid[2]= uid0>>8&0xff;
	uid[3]= uid0&0xff;
	uid[4]= uid1>>24&0xff;
	uid[5]= uid1>>16&0xff;
	uid[6]= uid1>>8&0xff;
	uid[7]= uid1&0xff;
	return 0;
}

LOCAL uint8 regulate_signature(NV_EFUSE_SIGN_DATA_T* sign_in, uint16* len, uint8* sign_out)
{
	//*len = sign_in->length;
	//uint16 Len;
	//uint8* des_ptr = sign_out;
	//uint8* src_ptr = sign_in->signature.data;
	*len = UIDLEN;	
	//Len = *len;
	HALO_ASSERT(*len <= SIGNMAXLEN);
	HALO_Memcpy(sign_out, sign_in->signature.data, *len);
	return 0;
}

LOCAL uint8 verify_signature_64bits(uint8 *uid)
{
	uint8 sign_result[SIGNMAXLEN] = {0x0};
	uint8 f_list[F_LIST_LEN] = {0x0};
	uint8 ret =0;
	uint16 sign_len = 0;
	uint16 lenbit = 0;
	NV_EFUSE_SIGN_DATA_T signature={0};
	
	
	ret = HALO_SignRead(&signature);

	if(!ret)
	{
	      PRTF("verify read nv failed!\n"); 
	      return FALSE;
	}
	regulate_signature(&signature, &sign_len, sign_result);
	lenbit = sign_len*8;
	ret = sprd_rsa_verify(pubExp,mod_N,uid,lenbit,sys_key,sign_result);
	if(ret)
	{
		PRTF("verify license success!\n"); 
		f_list[0] = 1;
		Set_featurelist(f_list);
		return TRUE;
	}
	else
	{
		PRTF("verify license failed!\n"); 
		f_list[0] = 0;
		Set_featurelist(f_list);	
		return FALSE;
	}
}

LOCAL uint8 verify_signature_128256bits(uint8 *uid)
{
	uint8 sign_result[SIGNLEN] = {0x0};
	uint8 f_list[F_LIST_LEN] = {0xE2};
	uint8 ret =0;
	uint16 sign_len = 0;
	uint16 lenbit = 0;
    uint8 j = 0;
	NV_EFUSE_SIGN_DATA_T signature={0};
	
	
	ret = HALO_SignRead(&signature);

	if(!ret)
	{
	      PRTF("verify read nv failed!\n"); 
	      return 0xe0;
	}
	//regulate_signature(&signature, &sign_len, sign_result);
	if(signature.length == 0)
	{
		PRTF("verify_signature_128256bits lenght = 0!\n");
		return 0xe1;
	}
	PRTF("verify_signature_128256bits signature.length  %d!\n", signature.length);
	HALO_Memcpy(sign_result, signature.signature.data, signature.length);
	lenbit = sign_len*8;
	
	//ret = sprd_rsa_verify(pubExp,mod_N,uid,lenbit,sys_key,sign_result);
	//pubExp,mod_N should integrate from server, so can't just reuse 64bits key
	HALO_ASSERT(sizeof(pubExp) == 4);
	HALO_ASSERT(sizeof(mod_128) == SIGNLEN);
	ret = sprd_rsa_verify128_256(pubExp,mod_128,uid,SIGNLEN*8,64,sys_key,sign_result, f_list);
	
	if(ret == TRUE)
	{
		PRTF("verify license success!\n"); 
		Set_featurelist(f_list);
		return 0x0;
	}
	else
	{
		if(feature_list)
		PRTF("verify license failed!\n"); 
		Set_featurelist(f_list);	
		return 0xe2;
	}
}


LOCAL uint8 verify_signature_pair(uint32 uid0,uint32 uid1,uint32* model)
{
	uint8 uid[UIDLEN] = {0x0};
	uint8 f_list[F_LIST_LEN] = {0x0};
	uint8 sign_result[SIGNMAXLEN] = {0x0};
	uint8 ret = 0;
	uint16 sign_len = 0;
	uint16 lenbit = 0;
	NV_EFUSE_SIGN_DATA_T signature={0};
	
	regulate_uid(uid0, uid1,  uid);
	
	ret = HALO_SignRead(&signature);

	if(!ret)
	{
	      PRTF("verify read nv failed!\n"); 
	      return FALSE;
	}
	regulate_signature(&signature, &sign_len, sign_result);
	lenbit = sign_len*8;
	ret = sprd_rsa_verify(pubExp,mod_N,uid,lenbit,sys_key,sign_result);
	if(ret)
	{
		PRTF("verify license success!\n"); 	
		return TRUE;
	}
	else
	{
		PRTF("verify license failed!\n");	
		return FALSE;
	}
}

