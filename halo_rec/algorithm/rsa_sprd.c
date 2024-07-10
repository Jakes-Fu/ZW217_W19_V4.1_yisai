#include <stdio.h>
#include "system_hal.h"
#include "rsa_sprd.h"
#include   "sha1_32.h"
#undef  PRTF_BLOCK(_EXP,PTR,LEN)
#define PRTF_BLOCK(_EXP,PTR,LEN) Printf_Buf(_EXP,PTR,LEN)

void Printf_Buf(uint8* str, uint8* address, int len)
{
	int i = 0;
	int j = 0;
	int loop = 0;
	int mod = 0;
	uint8 temp[45] ={0};
	
    PRTF_SILENCE("%s",str);
    PRTF_SILENCE("[Buf_table]");
	loop = len/8;
	mod = len%8;
	for(i = 0; i<loop; i++)
	{
		PRTF_SILENCE("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X ",\
		*(uint8*)(address+8*i),*(uint8*)(address+1+8*i),*(uint8*)(address+2+8*i),*(uint8*)(address+3+8*i),\
		*(uint8*)(address+4+8*i),*(uint8*)(address+5+8*i),*(uint8*)(address+6+8*i),*(uint8*)(address+7+8*i)\
		);
	}
	if(mod != 0)
	{
		
#if 1
		for(j = 0; j< mod; j++)
		{
			snprintf(temp+j*5,5,"0x%02X ",*(uint8*)(address+8*i+j));
		}
		snprintf(temp+j*5,1,"\0");
		PRTF_SILENCE("%s",temp);
#endif 
	}
    PRTF_SILENCE("[Buf_end]");
	
}

 uint32 ascii2hex(char* src, uint8* dst, int len)
{
    int i = 0;
	int j = 0;
	int tmp_len = 0;
    uint8 tmpData = 0;
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


uint8 shatter_bits(uint8 data)
{
	data=(data<<4)|(data>>4);

	data=((data<<2)&0xcc)|((data>>2)&0x33);

	data=((data<<1)&0xaa)|((data>>1)&0x55);

	//printf("  after shatter_bits data=%x \n",data);
	return data;
}

void deconvert_signvalue(uint8 *sign_result)
{
  uint8 tmp_data[4] = {0};
  uint8 tmp_v = 0;
  int16 i = 0;
  
  tmp_v = sign_result[0];

  for(i=0;i<3;i++)
  {
    sign_result[i] = sign_result[i+1];
	if(i%2 == 0)
    	sign_result[i] = shatter_bits(sign_result[i]);
  }

  sign_result[3] = tmp_v;
  sign_result[3] = shatter_bits(sign_result[3]);

  HALO_Memcpy(tmp_data,sign_result+2,2);
  HALO_Memcpy(tmp_data+2,sign_result,2);

   
  HALO_Memcpy(sign_result,tmp_data,4);


}

void convert_signvalue(uint8 *sign_result)
{
  uint8 tmp_data[4] = {0};
  uint8 tmp_v = 0;
  int16 i = 0;
  
  HALO_Memcpy(tmp_data,sign_result+2,2);
  HALO_Memcpy(tmp_data+2,sign_result,2);

  tmp_v = tmp_data[3];
  for(i=3;i>0;i--)
  { 
    tmp_data[i] = tmp_data[i-1];
	 if(i%2 == 1)
    	tmp_data[i] = shatter_bits(tmp_data[i]);
  }

  tmp_data[0] = tmp_v;
  tmp_data[0] = shatter_bits(tmp_data[0]);
  HALO_Memcpy(sign_result,tmp_data,4);

}
void shatter_signvalue(uint8 *sign_result,int16 len)
{
	int16 i = 0;
	for(i=0;i<len-3;i++)
	{
	   convert_signvalue(sign_result+i);
	
	}
	
}

void recover_signvalue(uint8 *sign_result,int16 len)
{
	int16 i = 0;
	for(i=len-4;i>=0;i--)
	{
		deconvert_signvalue(sign_result+i);	
	}
}	
	
	



void pri_ende(uint8 *src,int16 src_len,uint8 *des,uint8 *sys_key)
{
   int16  i = 0;
   uint8 in_key[] = {0x15,0x45,0x1a,0x8b,0xc8,0x92,0xf7,0x6b,0x12,0x99,0x5c,0x9a,0x98,0x08,0x11,0xa9};
   
   for(i=0;i<src_len;i++)
    {
    des[i] = src[i]^in_key[i%16];
    des[i] = des[i]^sys_key[i%32];
   }
}

uint8 compare_equal(uint8 *buf1, uint8 *buf2, uint16 len)
{
	uint16 i = 0;
	for(i=0;i<len;i++)
	{
		if(buf1[i]!=buf2[i])
			return FALSE;
	}
	return TRUE;
}

void feature_encode(uint8 *src,int16 src_len,uint8 *des, uint8** f_key,uint8 key_level)
{
	int16 i = 0;
	int16 j = 0;
	uint8 temp = 0;
	for(i=0;i<src_len;i++)
    {
		for(j=0;j<key_level;j++)
		{
			temp = f_key +2*i%8+j;
			des[i] = src[i]^temp;
			des[i] = shatter_bits(des[i]);
		}
    }
}

uint8 feature_decode(uint8 *src,int16 src_len,uint8 *des, uint8** f_key,uint8 key_level)
{
	int16 i = 0;
	int16 j = 0;
	uint8 ret = 0; 
	uint8 temp = 0;
	unsigned char feature_digest[20] = {0};
	for(i=0;i<src_len;i++)
    {
		for(j=key_level-1;j>=0;j--)
		{
			temp = f_key +2*i%8+j;
			des[i] = shatter_bits(src[i]);
			des[i] = des[i]^temp;
		}
    }
	ret = SHA1_ALGO((unsigned int *)des,64>>2,feature_digest);
	if(ret!=0)
        return 0xe1;
	ret = compare_equal(feature_digest,des+64,20);
	if (ret != TRUE)
		return 0xe2;
	return ret;
}


/****************************************************/
/* function name: sprd_rsa_sign(unsigned char *prvExp,unsigned char *mod_N,unsigned char *uid,int uid_bitlen,unsigned char *sys_key,unsigned char *sign_result)
/* in parameter:  
                 prvExp:  private exponent ,length should be bitLen_N>>3 bytes
	          mod_N:  modulus ,length should be bitLen_N>>3 bytes
		  uid: chip uid(55 bit)  
                  bit_len: the length of chip uid   
		  sys_key:encrypt key with aes algo
                   
/* out parameter: sign_result--sign value  8 bytes
/* return: true--success
           false---failed
/****************************************************/
boolean sprd_rsa_sign(unsigned char *prvExp,unsigned char *mod_N,unsigned char *uid,int uid_bitlen,unsigned char *sys_key,unsigned char *sign_result)
{
  
    int i = 0;
    int ret = 0;
    boolean rt_v = 0;

   unsigned char tmp_uid[0x08] = {0x0};
   unsigned char uid_digest[20] = {0x0};
  
   unsigned char uid_sign_result[8] = {0x0};
   int uid_bytelen = uid_bitlen/8 ;
     
   HALO_Memcpy(tmp_uid,uid,uid_bytelen);
   /*calculate uid digest*/ 
   ret = SHA1_ALGO((unsigned int *)tmp_uid,uid_bytelen>>2,uid_digest);
   if(ret!=0)
        return FALSE;
  

    uid_digest[0] = 0x0;
	PRTF_BLOCK("***sprd_rsa_sign encode uid_digest:",uid_digest,uid_bytelen);
 
   /*calculate signature with the fitst 4 byte of uid_digest*/
    rt_v = RSA_PrvEnc( prvExp,mod_N,uid_bitlen,uid_digest,uid_bytelen,uid_sign_result);  

	PRTF_BLOCK("***sprd_rsa_sign encode uid_sign_result:",uid_sign_result,uid_bytelen);
 
   
   if(rt_v == FALSE)
       return FALSE; 
   /*convert the uid signature*/
   convert_signvalue(uid_sign_result);
   convert_signvalue(uid_sign_result+4);//64bits conversion
    
   /*encrypt the converted signature with aes algo*/  
   
    pri_ende(uid_sign_result,uid_bytelen,sign_result,sys_key);

    
   return TRUE;

}


/****************************************************/
/* function name: sprd_rsa_verify(unsigned char *pubExp,unsigned char *mod_N,unsigned char *uid,int uid_bitlen,unsigned char *sys_key,unsigned char *sign_result)
/* in parameter:  
                  prvExp:  private exponent ,length should be bitLen_N>>3 bytes
	          mod_N:  modulus ,length should be bitLen_N>>3 bytes
		  uid: chip uid(55 bit)  
                  bit_len: the length of chip uid   
		  sys_key:encrypt key with aes algo
                   
/* out parameter: sign_result--sign value  8 bytes
/* return: true--success
           false---failed
/****************************************************/
boolean sprd_rsa_verify(unsigned char *pubExp,unsigned char *mod_N,unsigned char *uid,int uid_bitlen,unsigned char *sys_key,unsigned char *en_sign_result)
{
  
   int ret = 0;
   int ret_len = 0;
   int i = 0;
   unsigned char tmp_uid[0x08] = {0x0};
   unsigned char uid_digest[20] = {0x0};
   unsigned char vfy_digest[8] = {0x0};
   unsigned char uid_sign_result[8] = {0x0};
   int uid_bytelen = uid_bitlen/8 ;
    HALO_Memcpy(tmp_uid,uid,uid_bytelen);
   /*calculate uid digest*/  
   ret = SHA1_ALGO((unsigned int *)tmp_uid,uid_bytelen>>2,uid_digest);
    if(ret !=0)
       return FALSE;   
    uid_digest[0] = 0x0;
   PRTF_BLOCK("*** sprd_rsa_verify decode uid_digest:",uid_digest,uid_bytelen);

   /*decrypt en_sign_result*/
  
    pri_ende(en_sign_result,uid_bytelen,uid_sign_result,sys_key);

     /*convert uid sign result*/
    deconvert_signvalue(uid_sign_result);
    deconvert_signvalue(uid_sign_result+4);//64bits conversion

	PRTF_BLOCK("***sprd_rsa_verify decode uid_sign_result:",uid_sign_result,uid_bytelen);
   

   /*calculate verify signature*/
    ret_len = RSA_PubDec(pubExp,mod_N,uid_bitlen,uid_sign_result,vfy_digest);

    PRTF_BLOCK("***sprd_rsa_verify vfy_digest:",vfy_digest,ret_len);
	PRTF_BLOCK("***sprd_rsa_verify  uid_digest is:",uid_digest,8);
  
    ret = compare_equal(uid_digest,vfy_digest,ret_len);
    if(ret ==true)
      return TRUE;
    else
      return FALSE;
   

}


boolean sprd_rsa_sign128_256(unsigned char *prvExp,unsigned char *mod_N,unsigned char *uid,int uid_bitlen, int flen, unsigned char *sys_key,unsigned char *sign_result)
{
  
    int i = 0;
    int ret = 0;
    boolean rt_v = FALSE; 
   unsigned char* tmp_uid = NULL;
   unsigned char* uid_sign_result = NULL;
   unsigned char uid_digest[20] = {0};
   unsigned int text_len= uid_bitlen/8;
   tmp_uid = HALO_Malloc(flen+20);
	if(!tmp_uid)
	{
		return FALSE;
	}
	
	uid_sign_result = HALO_Malloc(text_len);
	if(!uid_sign_result)
	{
		return FALSE;
	}
   
    ret = SHA1_ALGO((unsigned int *)uid,flen>>2,uid_digest);
    if(ret !=0)
	{
		HALO_Free(tmp_uid);
		tmp_uid = NULL;
		HALO_Free(uid_sign_result);
		uid_sign_result = NULL;
       return FALSE; 
	}	   
    
   HALO_Memcpy(tmp_uid,uid,flen);
   HALO_Memcpy(tmp_uid+flen,uid_digest,20);
   
   rt_v = RSA_PrvEnc( prvExp,mod_N,uid_bitlen,tmp_uid,flen+20,uid_sign_result);  
   PRTF_BLOCK("***sprd_rsa_sign128_256 RSA_PrvEnc result:",uid_sign_result,text_len);
  
   if(rt_v == FALSE)
   {
	   HALO_Free(tmp_uid);
	   tmp_uid = NULL;
	   HALO_Free(uid_sign_result);
	   uid_sign_result = NULL;
       return FALSE; 
   }
   /*convert the uid signature*/
   shatter_signvalue(uid_sign_result,text_len);
   pri_ende(uid_sign_result,text_len,sign_result,sys_key);
	HALO_Free(tmp_uid);
	tmp_uid = NULL;
	HALO_Free(uid_sign_result);
	uid_sign_result = NULL;
   return TRUE;

}


/****************************************************/
/* function name: sprd_rsa_verify128_256(unsigned char *pubExp,unsigned char *mod_N,unsigned char *uid,int uid_bitlen,unsigned char *sys_key,unsigned char *sign_result)
/* in parameter:  
                  prvExp:  private exponent ,length should be bitLen_N>>3 bytes
	          mod_N:  modulus ,length should be bitLen_N>>3 bytes
		  uid: chip uid(64 bit)  
                  bit_len: the length of chip uid   
		  sys_key:encrypt key with aes algo
                   
/* out parameter: sign_result--sign value  8 bytes
/* return: true--success
           false---failed
/****************************************************/
boolean sprd_rsa_verify128_256(unsigned char *pubExp,unsigned char *mod_N,unsigned char *uid,int uid_bitlen,int flen, unsigned char *sys_key,unsigned char *en_sign_result, unsigned char *de_featurelist)
{
  
   int ret = 0;
   int ret_len = 0;
   int i = 0;
   unsigned char tmp_uid[8] = {0x0};
   unsigned char uid_digest[20];
   unsigned char* vfy_digest = NULL;
   unsigned char* uid_sign_result = NULL;
   unsigned int text_len= uid_bitlen/8;
   
    vfy_digest = HALO_Malloc(text_len);
	if(!vfy_digest)
	{
		return FALSE;
	}
	
	uid_sign_result = HALO_Malloc(text_len);
	if(!uid_sign_result)
	{
		return FALSE;
	}
   
   HALO_Memcpy(tmp_uid,uid,8);
   
   PRTF_BLOCK("***sprd_rsa_verify128_256  input signed values are:",en_sign_result,text_len);
 
   /*decrypt en_sign_result*/
  
    pri_ende(en_sign_result,text_len,uid_sign_result,sys_key);

     /*convert uid sign result*/
	recover_signvalue(uid_sign_result,text_len);

	PRTF_BLOCK("***sprd_rsa_sign128_256 after pri_ende uid_sign_result:",uid_sign_result,text_len);
	
   /*calculate verify signature*/
    ret_len = RSA_PubDec(pubExp,mod_N,uid_bitlen,uid_sign_result,vfy_digest);
	
	   PRTF("***sprd_rsa_sign128_256 after RSA_PubDec is ret_len: %d\n", ret_len);
	   PRTF_BLOCK("after RSA_PubDec vfy_digest :",vfy_digest,ret_len);
	  
       if(ret_len < 0)
	   {
			PRTF("***sprd_rsa_sign128_256 verify RSA_PubDec ret_len invalid [%d] ", ret_len);
			HALO_Free(vfy_digest);
			vfy_digest = NULL;
			HALO_Free(uid_sign_result);
			uid_sign_result = NULL;
			return FALSE;
	   }
	   
	    ret = SHA1_ALGO((unsigned int *)vfy_digest,flen>>2,uid_digest);
		if(ret !=0)
		{
			PRTF("***SHA1_ALGO uid_digest failed result [%d] ", ret);
			HALO_Free(vfy_digest);
			vfy_digest = NULL;
			HALO_Free(uid_sign_result);
			uid_sign_result = NULL;
		   return FALSE; 
		}	   
	  
	    ret = compare_equal(uid_digest,vfy_digest+flen,20);
		PRTF("***sprd_rsa_sign128_256 compare_equal hash result [%d] ", ret);
		if(ret == FALSE)
		{
			PRTF("\n***uid_digest is not equal to vfy_digest digest \n ");
			HALO_Free(vfy_digest);
			vfy_digest = NULL;
			HALO_Free(uid_sign_result);
			uid_sign_result = NULL;

			return FALSE;
		}
		
	    PRTF_BLOCK("sprd_rsa_sign128_256 verify local uid  :",tmp_uid,8);
		PRTF_BLOCK("sprd_rsa_sign128_256 verify  vfy_digest  :",vfy_digest,8);
	
		ret = compare_equal(tmp_uid,vfy_digest,8);
		PRTF("***sprd_rsa_sign128_256 compare_equal uid value[%d] ", ret);
		HALO_Memcpy(de_featurelist, vfy_digest, flen+20);
		HALO_Free(vfy_digest);
		vfy_digest = NULL;
		HALO_Free(uid_sign_result);
		uid_sign_result = NULL;
		if(ret ==TRUE)
		  return TRUE;
		else
		  return FALSE;
   

}