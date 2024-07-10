#include "alloc-testing.h"
#include "framework.h"
#include "halo_factory.h"
#include "halo_factory_api.h"

DIAG_WRITE_LICENSE_T test_license_nv;

uint8 test_system_info[56] = "12345678901234567890123456789012345678901234567890123456";
int8 test_str_ptr[10]    = "45128986";
int8 test_license_ptr[10]    = "12345678";

static unsigned char test_cmpmem(void* src, void* dest, uint32 len)
{
	uint32 i;
	uint8 ret = 1;
	JUDGE(src != HALO_NULL,ret);
	JUDGE(dest != HALO_NULL,ret);
	JUDGE(len>0,ret);
	
	if(ret == 0)
	{
		PRTF("Invalid parameter can't compare \n");
		return ret;
	}
	
	for(i=0;i<len;i++)
	{
	 	if(*(uint8*)src!=*(uint8*)dest)
	 	{
			PRTF("test_cmpmem [%d] element not equal, src[%x],dest[%x]\n", i+1, *(uint8*)src, *(uint8*)dest);
			ret =0;
			break;
	 	}
		else if(i ==(len-1))
		{
			ret = 1;
		}
		((uint8*)src)++;
		((uint8*)dest)++;
	}
	return ret;
}

static unsigned char test_diagReadUid(void)
{   
	uint8 uid[UIDLEN];
	HALO_ReadUid(uid);
	PRTF("TEST ReadUid %02x %02x %02x %02x %02x %02x %02x %02x\n", uid[0], uid[1], uid[2], uid[3], uid[4], uid[5], uid[6], uid[7]);
	return 1;
}





static unsigned char test_CheckLicense(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{ 
	DIAG_READ_LICENSE_T *result;
	uint8      cnf_len  = 2+56;
	MSG_HEAD_T  *head_ptr;
	uint8 errorcode;
	uint8 *pmodel =HALO_NULL;
	uint8  model_len = 56;
	uint8  signature_len = SIGNLEN;
	unsigned char ret = 1;
	

#if 0
       model_len = src_len - sizeof(MSG_HEAD_T);

       pmodel =  SCI_ALLOC_APP(model_len); 
	SCI_ASSERT(SCI_NULL != pmodel);  
       PRTF("efuse check model_len =%d,src_len=%d,sizeof=%d\n",model_len,src_len,sizeof(MSG_HEAD_T));
       SCI_MEMCPY(pmodel, (src_ptr+sizeof(MSG_HEAD_T)), model_len);
	
	errcode = check_efuse_write(pmodel,model_len); 
#endif

	errorcode = HALO_Verify_Signature();
	PRTF("efuse check errcode =%d\n",errorcode);
	pmodel =  HALO_MallocAPP(model_len); 
	HALO_JUDGE_STR(TEST  test_CheckLicense_diag,HALO_NULL != pmodel,ret);

	head_ptr = (MSG_HEAD_T*)HALO_MallocAPP(sizeof(DIAG_READ_LICENSE_T) + sizeof(MSG_HEAD_T)); 
	HALO_JUDGE_STR(TEST  test_CheckLicense_diag,HALO_NULL != head_ptr,ret);

	*dest_ptr       = (uint8 *)head_ptr;
	*dest_len_ptr   = sizeof(DIAG_READ_LICENSE_T) + sizeof(MSG_HEAD_T);

	HALO_Memcpy((void *)head_ptr, src_ptr, sizeof(MSG_HEAD_T));
	
	result = (void *)(head_ptr + 1);
	result->errorcode =errorcode;
	result->signature_len =signature_len;
	//HALO_Memcpy((void *)(result->system_info),(void *) test_system_info, sizeof(result->system_info));
	head_ptr->len = *dest_len_ptr;
        HALO_Free(pmodel);
        return ret;

}

static unsigned char test_ReadUid(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
	DIAG_UID_T *result;
	
	uint8      errcode = 0;
	unsigned char ret = 1;
	uint16    cnf_len     = UIDLEN+1;
	//uint8      temp[4] ={0};
	char*     str_temp = HALO_NULL;
	MSG_HEAD_T  *head_ptr;

	errcode =0x0f; //check UID success
	PRTF("uid write errcode =%d\n",errcode);
		   
	 
	head_ptr = (MSG_HEAD_T*)HALO_MallocAPP( sizeof(DIAG_UID_T) + sizeof(MSG_HEAD_T)); 
	HALO_JUDGE_STR(TEST  test_CheckLicense_diag,HALO_NULL != head_ptr,ret);

	*dest_ptr       = (uint8 *)head_ptr;
	*dest_len_ptr   = sizeof(DIAG_UID_T) + sizeof(MSG_HEAD_T);

	HALO_Memcpy((void *)head_ptr, src_ptr, sizeof(MSG_HEAD_T));

	result = (void *)(head_ptr + 1);
	result->errorcode = errcode;
	HALO_Memcpy((void *)(result->uid_info),(void *) test_str_ptr, UIDLEN);
	
	head_ptr->len = *dest_len_ptr;
	return ret;
}

static unsigned char test_WriteLicense(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8.
{
	DIAG_WRITE_LICRET_T *result;
	DIAG_WRITE_LICENSE_T input;
	unsigned char ret = 1;
	uint16    SignLen =0;
	uint8      errcode = 0;
	uint16    cnf_len     = 1;
	uint32    sign1 = 0;
	uint32    sign2 = 0;
	uint8      sign[SIGNLEN]={0};
	uint32    cnf_ptr = 0;
	char*     str_ptr    = HALO_NULL;
	MSG_HEAD_T  *head_ptr;

	 SignLen = src_len-sizeof(MSG_HEAD_T);
	 PRTF("Write Efuse Sign Len=%d\n",SignLen);

	 if(SIGNLEN == SignLen)
	 {	

		HALO_Memcpy((void *)&input.len, (void*)(src_ptr+1), sizeof(input.len));	
		HALO_Memcpy((void *)&input.sign_info, src_ptr+sizeof(MSG_HEAD_T), SIGNLEN);	
		//sign1 = sign[3]|(sign[2]<<8&0xff00)|(sign[1]<<16&0xff0000)|(sign[0]<<24&0xff000000); Big endian
	       // sign2 = sign[7]|(sign[6]<<8&0xff00)|(sign[5]<<16&0xff0000)|(sign[4]<<24&0xff000000); Big endian
	        sign1 = sign[0]|(sign[1]<<8&0xff00)|(sign[2]<<16&0xff0000)|(sign[3]<<24&0xff000000);
	        sign2 = sign[4]|(sign[5]<<8&0xff00)|(sign[6]<<16&0xff0000)|(sign[7]<<24&0xff000000);
		PRTF("sign1=%x,sign2=%x\n",sign1,sign2);  
		errcode = 0x0;
	       PRTF("sign write errcode =%d\n",errcode);
		
	 }
	else
	{
	     errcode=0xff;
	
	}
	
	head_ptr = (MSG_HEAD_T*)HALO_MallocAPP(sizeof(DIAG_WRITE_LICRET_T) + sizeof(MSG_HEAD_T)); 
	HALO_JUDGE_STR(TEST  test_WriteLicense_diag,HALO_NULL != head_ptr,ret);

	*dest_ptr       = (uint8 *)head_ptr;
	*dest_len_ptr   = sizeof(DIAG_WRITE_LICRET_T) + sizeof(MSG_HEAD_T);

	HALO_Memcpy((void *)head_ptr, src_ptr, sizeof(MSG_HEAD_T));
        result = (void *)(head_ptr + 1);
	result->errorcode = errcode;
	head_ptr->len = *dest_len_ptr;
	return ret;
}

static unsigned char test_diag_interface(void)
{
    unsigned char ret = 1;
	unsigned char i =0;
	unsigned char head_len = sizeof(MSG_HEAD_T);
	uint8  errcode = 0;
	uint8  model_len = 0;
	uint8	 *output_msg_ptr = HALO_NULL;
	uint16 output_len      = 0;
	uint8	 *intput_msg_ptr = HALO_NULL;
	uint16  intput_len      = 0;
	DIAG_READ_LICENSE_T* test1_ret;
	DIAG_UID_T* test2_ret;
	DIAG_WRITE_LICRET_T* test3_ret;
	DIAG_WRITE_LICENSE_T* test_data;
       //test check license data transfer
	ret=  test_CheckLicense(&output_msg_ptr, &output_len, (uint8*)intput_msg_ptr,intput_len);
	HALO_JUDGE_STR(TEST test_CheckLicense_diag_interface,ret == 1,ret);
	test1_ret = (void*)(output_msg_ptr+sizeof(MSG_HEAD_T));
	errcode = test1_ret->errorcode;
	model_len = test1_ret->signature_len;
	PRTF("the return info is: ");
	PRTF("errorcode [%d] \n", errcode);
	PRTF("model_len [%d] \n", model_len);
	PRTF("system  info: ");
	//PRTF("%s ", test1_ret->system_info);
	PRTF("\n");
	HALO_Free(output_msg_ptr);
         //test read uid data transfer
	ret = test_ReadUid(&output_msg_ptr, &output_len, (uint8*)intput_msg_ptr,intput_len);
	test2_ret = (void*)(output_msg_ptr+sizeof(MSG_HEAD_T));
	PRTF("the return info is: ");
	PRTF("errorcode%d \n", test2_ret->errorcode);
	PRTF("UID info: ");
	PRTF("%s ", test2_ret->uid_info);
	HALO_Free(output_msg_ptr);
	
	 //test read write license transfer
	 model_len = sizeof(DIAG_WRITE_LICENSE_T) + sizeof(MSG_HEAD_T);
         intput_msg_ptr =(void*)HALO_MallocAPP(model_len); 
	 
	
	 test_data = (void*)((MSG_HEAD_T*)intput_msg_ptr+1);
	 test_data->len = SIGNLEN;
	 HALO_Memcpy((void*)(test_data->sign_info), (void*)test_license_ptr, sizeof(test_data->sign_info));
	 ret = test_WriteLicense(&output_msg_ptr, &output_len, (uint8*)intput_msg_ptr,intput_len);
	 test3_ret = (void*)(output_msg_ptr+sizeof(MSG_HEAD_T));
	 PRTF("the return info is: ");
	 PRTF("errorcode%d \n", test3_ret->errorcode);
	HALO_Free(output_msg_ptr);
	HALO_Free(intput_msg_ptr);
	

	return ret;
		
}


static unsigned char test_diagRead_WriteNV(void)
{   
	unsigned char sign_test[] = {0x0,0x01,0xb0,0x9,0xb,0x1c,0x0e,0x07};//"45128986";
	unsigned char sign_test_256[] = {0x97,0x55,0xBE,0xFC,0x79,0x52,0x13,0xE2,0x51,0x1F,0x08,0x27,0x05,0xD0,0x82,0x75,0x99,0xED,0x3D,0x52,0xD5,0x4A,0x8E,0x95,0x3C,0xB5,0x8C,0x6C,0x32,0xE2,0xB4,0xCF,0x23,0x27,0xA7,0x96,0x1E,0x66,0x0E,0x8C,0x30,0x8F,0x9F,0xCE,0xB8,0x87,0x8A,0xBB,0x15,0x0E,0xDC,0x3A,0x06,0xA7,0x1B,0xDD,0xF5,0x88,0x68,0x68,0x09,0x8E,0x47,0x41,0xAC,0x97,0xF3,0x05,0x71,0x0E,0x60,0x59,0x59,0x6C,0xB9,0x63,0x44,0x55,0xBA,0xE6,0x67,0x71,0x50,0x55,0x22,0x4F,0xCE,0xD8,0x0A,0x6A,0x1A,0x85,0x3E,0x6A,0x73,0x61,0xD0,0x35,0xC9,0x63,0x6D,0xEE,0xDA,0x64,0x0D,0xA8,0x1B,0x06,0x8A,0x53,0xC2,0xD7,0x87,0xB3,0x97,0x3E,0x4A,0xC0,0xF4,0x0B,0xF3,0xA8,0x62,0xE8,0xA0,0x6A,0xF0,0xF5,0xCE,0xC9,0x41,0x6A,0x35,0xB1,0xD1,0xAB,0xEF,0x55,0x83,0xBF,0xFA,0x86,0x6C,0x65,0xA6,0x21,0xB7,0x73,0x38,0x96,0xE0,0xCF,0x16,0x24,0xE4,0x3E,0x8E,0xCC,0x41,0xB6,0x60,0xCF,0x64,0x04,0x68,0xA6,0x5C,0x99,0x9B,0x0D,0x22,0xE0,0x10,0x2C,0xDE,0xB9,0x3C,0xD6,0x4D,0xBC,0xF5,0x7D,0xD8,0xC4,0x55,0xF4,0x1D,0x29,0x82,0x7F,0xB8,0xA0,0x43,0x94,0xD1,0x45,0x34,0x1A,0x5B,0x03,0x31,0xA8,0x4E,0x7B,0x26,0xB2,0x35,0x54,0xC9,0x09,0xBA,0xE3,0x91,0xFD,0x5C,0x9D,0x78,0x5B,0x17,0x18,0xD2,0x9C,0x1D,0x7F,0xDA,0x02,0x57,0x85,0xE3,0xD5,0x83,0x4E,0x38,0xC2,0x02,0x0C,0xA4,0xDB,0x9A,0x28,0x64,0x0B,0x90,0xA3,0x14,0x36,0xEB,0x14,0xAE,0xF2,0xB1,0x84,0x29,0x18,0x3C,0x39};
	unsigned char ret = 1;
	NV_EFUSE_SIGN_DATA_T NV_data;
	NV_EFUSE_SIGN_DATA_T NV_read;
	int i;
	NV_data.length = SIGNLEN;
	HALO_Memcpy(NV_data.signature.data,sign_test,SIGNLEN);
	HALO_SignWrite(NV_data);
	HALO_SignRead(&NV_read);
	HALO_JUDGE_STR(TEST 64bits NV write and read,NV_read.length == NV_data.length,ret);
	HALO_JUDGE_STR(TEST 64bits NV write and read,1==test_cmpmem(NV_read.signature.data, NV_data.signature.data,SIGNLEN),ret);
	
	NV_data.length = SIGNMAXLEN;
	HALO_Memcpy(NV_data.signature.data,sign_test_256,SIGNMAXLEN);
	HALO_SignWrite(NV_data);
	HALO_SignRead(&NV_read);
	HALO_JUDGE_STR(TEST 2048bits NV write and read,NV_read.length == NV_data.length,ret);
	HALO_JUDGE_STR(TEST 2048bits NV write and read,1==test_cmpmem(NV_read.signature.data, NV_data.signature.data,sizeof(NV_data.signature.data)),ret);
	
	return ret;
}

uint8 read_simulate_uid(uint8 *uid)
{
	unsigned char sign_test[] = {0x0,0x01,0xb0,0x9,0xb,0x1c,0x0e,0x07};//"45128986";
	HALO_Memcpy(uid,sign_test,SIGNLEN);
	return 0;
}

#undef HALO_ReadUid
#define HALO_ReadUid read_simulate_uid
static unsigned char test_diagVerifySignature(void)
{   
	unsigned char sign_test[] = {0x93,0xea,0x5b,0xed,0xb4,0xd0,0x01,0xe2};// encode of the "45128986";
	unsigned char ret = 1;
	NV_EFUSE_SIGN_DATA_T NV_data;
	NV_EFUSE_SIGN_DATA_T NV_read;
	int i;
	NV_data.length = SIGNLEN;
	HALO_Memcpy(NV_data.signature.data,sign_test,SIGNLEN);
	HALO_SignWrite(NV_data);
	HALO_SignRead(&NV_read);
	HALO_JUDGE_STR(TEST test_diagVerifySignature,1==test_cmpmem(NV_read.signature.data, NV_data.signature.data,SIGNLEN),ret);
	//ret = HALO_Verify_Signature();
	HALO_JUDGE_STR(TEST test_diagVerifySignature,HALO_Verify_Signature() == 1,ret);
	return ret;
}

static Unit_Test_T tests[] = {
	TEST(test_diagReadUid),
	TEST(test_diagRead_WriteNV),
	TEST(test_diagVerifySignature),
	TEST(test_diag_interface),
	{NULL,NULL}
};



int test_diagcmd(void)
{
	run_tests(tests);

	return 0;
}
