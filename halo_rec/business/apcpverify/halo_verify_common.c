#include "sha1_32.h"
#include "rsa_sprd.h"


/*
typedef struct _ap_pkt
{
	uint8 head[4];
	uint8 timestamp[4];	
	uint8 code[144];
}ap_pkt_t;
*/




LOCAL uint g_seed = 0;
LOCAL int32 read_reg = 1;
LOCAL int32 s_modem_type = -1;




LOCAL void __wtob(uint8 *buf,uint val ,uint *len)
{
     int32 i;
     i = *len;
     buf[i] = (val>>24);
     i++;
     buf[i] = ((val&0xff0000)>>16);
     i++;
     buf[i] = ((val&0xff00)>>8);
     i++;
     buf[i] = (val&0xff);
     *len = i;
}

LOCAL int32 myrand()
{
    if(read_reg)
    {
        g_seed = HALO_GetTickCount();
        if(g_seed>0) read_reg = 0;
    }
return g_seed = (g_seed * 1103515245 + 12345) & RAND_MAX;
}

LOCAL uint get_rand()
{
	uint val;
	int32 timeout = 10;
	while(timeout--)
	{
		val = myrand();
		if(val != MAGIC_VAL )
		{
			return val;
		}
	}
	PRTF("get rand fail\n");
	return 0xdeadbeaf;
}


int32 gen_rand_code(uint8 *c)
{
	int32 i;
	uint len = 0;
	
	if(c == NULL)
	{
		PRTF("gen rand code error\n");
		return -1;
	}

	for(i=0;i<4;i++)//gen 16byte random code
	{
		__wtob(c,get_rand(),&len);
		len++;
		//PRTF("len %d",len);
	}
	return 0;
}

int32 calc_sha1(uint8 *data,uint data_len,uint8 *out)
{
	int32 i;
	SHA1Context_32	sha;

	if(data == NULL || out == NULL)
	{
		PRTF("gen calc sha1 error\n");
		return -1;
	}
	
	SHA1Reset_32(&sha);
	SHA1Input_32(&sha, (uint*)data, data_len>>2);
  	SHA1Result_32(&sha, out);

	return 0;
}


int32 comb_ts(uint8 *in)
{
	uint len = 0;
	int32 ct = -1;
	if( in == NULL)
	{
		PRTF("comb ts error\n");
		return -1;
	}
	ct = HALO_GetTickCount();
	if( ct != 0 )
	{
		__wtob(in,ct,&len);
		PRTF(" comb_ts ts = %d ms,  len = %d ",ct,len);	
	}else{
			return -1;
	}
	return 0;
}

int32 gen_cert(uint8 *in,uint8 *out,uint len)
{
	int32 i=0;
	if(in == NULL || out == NULL)
	{
		PRTF("gen cert error\n");
		return -1;
	}
	RSA_PrvEnc(ENCODE_PRIK,ENCODE_N,1024,in,len,out);
	return 0;
}


int32 comb_rd(uint8 *rd,uint rd_len,
		uint8 *cert,uint cert_len,
		uint8 *out)
{
	if(rd == NULL || cert == NULL)
	{
		PRTF("comb rd error\n");
		return -1;
	}
	HALO_Memcpy(out,rd,rd_len);
	HALO_Memcpy(out+rd_len,cert,cert_len);
	return 0;
}

int32 set_pld(uint8 *in)
{
	int32 ret = 0;
	uint8 rd[16];
	uint8 hash[24];
	uint8 vcode[128];
	ret = gen_rand_code(rd);
	if(ret!=0) goto out;
	ret = calc_sha1(rd,16,hash);
	if(ret!=0) goto out;
	ret = comb_ts((unsigned char *)&hash[20]);
	if(ret!=0) goto out;
	ret = gen_cert(hash,vcode,24);
	if(ret!=0) goto out;
	ret = comb_rd(rd,16,vcode,128,in);
	if(ret!=0) goto out;
	return 0;
out:
	PRTF("set payload error");
	return -1;
}


int32 set_pkt(ap_pkt_t *pkt)
{
	int32 ret = 0;
	uint len = 0;
	uint ts = 0;
	
	if( pkt == NULL)
	{
		PRTF("halo pkt failed with val: NULL");
		return -1;
	}
	pkt->head[0] = CMD >> 8;
	pkt->head[1] = CMD & 0xff;
	pkt->head[2] = 0xff; //reserve
	pkt->head[3] = 144; //payload length
	
	ret = set_pld((unsigned char*)&(pkt->code[0]));
	
	ts = HALO_GetTickCount();
	__wtob(pkt->timestamp,ts,&len);
	PRTF(" before RPC mark the  ts = %d ms,  len = %d ", ts, len);
	return ret;
}


int32 verify_cert_real(uint8 *in)
{
	uint8 puke[4] = {0x0,0x1,0x0,0x1};
	uint8 rd[16];
	uint8 vcode[128];
	uint8 hash[20];
	int32 rlen = 0;
	int32 i;
	uint gtm = 0;
	
	if(in == NULL)
	{
			PRTF("in = null\n");
			return -1;	
	}
	HALO_Memset(rd,0,sizeof(rd));
	HALO_Memset(vcode,0,sizeof(vcode));
	HALO_Memset(hash,0,sizeof(hash));
	rlen = RSA_PubDec(puke,DECODE_N,1024,in+16,vcode);
	if(rlen != 24)
	{
		PRTF("len error\n");
		PRTF("@@@@auth len%d", rlen);
		return -1;
	}
#if 1 //@shawn test
	gtm = __BTOW(vcode[20],vcode[21],vcode[22],vcode[23]);
	PRTF("@@@@auth gtm = %d ms, s_modem_type is %x", gtm, s_modem_type);
	calc_sha1(in,16,hash);
	for(i=0;i<20;i++)
	{
		if(hash[i] != vcode[i])
		{
			PRTF("check hash fail\n");
			return -1;
		}
	}
#endif
	PRTF("halo verify_cert CP ceriticate success");
	return 0;
}
int32 get_pkt(ap_pkt_t *in)
{
	int32 ret = 0;
	if( in == NULL)
	{
		PRTF("halo_get_pkt with NULL package");
		return -1;
	}
	if(	in->head[0] == (R_CMD >>8)
		&& in->head[1] == (R_CMD & 0xff))
	{
		if(in->head[3] != 144)
		{
			PRTF("halo pkt length error");
			return -1;
		}
		PRTF("halo enter verify cert");
		//ret = verify_cert_fake((unsigned char*)&(in->code[0])); for RPC tunnel test only
		ret = verify_cert_real((unsigned char*)&(in->code[0]));
	}else{
		PRTF("halo wrong pkt 0x%x 0x%x",in->head[0],in->head[1]);
		return -1;
	}
	return ret;
}	