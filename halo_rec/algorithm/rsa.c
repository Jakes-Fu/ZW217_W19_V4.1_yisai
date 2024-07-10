/*****************************************************************
RSA类源文件：	RSA.cpp
版本：			0.31 (2009.5.27)
说明：			适用于MFC，128-1024位RSA运算
作者：			Allen Yu
*****************************************************************/
//#include <os_api.h>
#include "system_hal.h"
#include "rsa_sprd.h"
//允许生成1120位（二进制）的中间结果
//#define		BIGINT_MAXLEN 35
//允许生成2112位（二进制）的中间结果
//#define		BIGINT_MAXLEN 66
//允许生成4224位（二进制）的中间结果
#define		BIGINT_MAXLEN 132
#define		DEC 10
#define		HEX 16

int padding_add_PKCS1_type_1(unsigned char *to, int tlen,
	     const unsigned char *from, int flen);
int padding_check_PKCS1_type_1(unsigned char *to, int tlen,
	     const unsigned char *from, int flen, int num);
int padding_add_PKCS1_type_2(unsigned char *to, int tlen,
	     const unsigned char *from, int flen);
int padding_check_PKCS1_type_2(unsigned char *to, int tlen,
	     const unsigned char *from, int flen, int num);
//	大整数结构
typedef struct
{
	//	大数在0x1 0000 0000进制(2^32)下的长度
	int	m_iLength32;

	//	用数组记录大数在0x1 0000 0000进制下每一位的值
	unsigned int	m_ulValue[BIGINT_MAXLEN];
} SBigInt;


static int myrand();

LOCAL SBigInt	RSA_C,RSA_M,RSA_N,RSA_D,RSA_E;

LOCAL SBigInt	RSA_P,RSA_Q;			//	调试方便放着，完成后放protected



//	构造RSA对象并初始化为零
void RSA_initialize()
{
	int i = 0;

    for(i=0;i<BIGINT_MAXLEN;i++)
	{
		RSA_C.m_ulValue[i]=0;
		RSA_D.m_ulValue[i]=0;
		RSA_E.m_ulValue[i]=0;
		RSA_M.m_ulValue[i]=0;
		RSA_N.m_ulValue[i]=0;
		RSA_P.m_ulValue[i]=0;
		RSA_Q.m_ulValue[i]=0;
	}

	RSA_C.m_iLength32=1;
	RSA_D.m_iLength32=1;
	RSA_E.m_iLength32=1;
	RSA_M.m_iLength32=1;
	RSA_N.m_iLength32=1;
	RSA_P.m_iLength32=1;
	RSA_Q.m_iLength32=1;
}

//	解构RSA对象
void RSA_deinitialize()
{
}

/****************************************************************************************
大数比较
调用方式：	两比较值
返回值：	若In<In2返回-1；若=返回0；若>返回1
****************************************************************************************/
int		Cmp(SBigInt *sIn, SBigInt *sIn2)
{
	int	i = 0;

    if(sIn->m_iLength32>sIn2->m_iLength32)
		return 1;
    if(sIn->m_iLength32<sIn2->m_iLength32)
		return -1;

    for(i=sIn->m_iLength32-1;i>=0;i--)
    {
        if(sIn->m_ulValue[i]>sIn2->m_ulValue[i])
			return 1;
        if(sIn->m_ulValue[i]<sIn2->m_ulValue[i])
			return -1;
    }

    return 0;
}

/****************************************************************************************
大数赋值
调用方式：	后一参数赋值给前一参数
返回值：	无
****************************************************************************************/
void	Mov_bigint(SBigInt *sOut, SBigInt *sIn)
{
	int	i =0;

//	if(*sOut==*sIn)
//		return;

    sOut->m_iLength32=sIn->m_iLength32;

    for(i=0;i<BIGINT_MAXLEN;i++)
		sOut->m_ulValue[i]=sIn->m_ulValue[i];

	return;
}

void	Mov_long(SBigInt *sOut, uint64_t A)
{
	int	i =0;

	if(A>0xffffffff)
    {
        sOut->m_iLength32=2;
        sOut->m_ulValue[1]=(uint64_t)(A>>32);
        sOut->m_ulValue[0]=(uint64_t)A;
    }
    else
    {
        sOut->m_iLength32=1;
        sOut->m_ulValue[0]=(uint64_t)A;
    }

	for(i=sOut->m_iLength32;i<BIGINT_MAXLEN;i++)
		sOut->m_ulValue[i]=0;
}

/****************************************************************************************
大数相加
调用形式：	后两参数想加赋值给前一参数，输出对象可以是第一个输入对象本身
返回值：	无
****************************************************************************************/
//	sOut可同SIn
void	Add_bigint(SBigInt *sOut, SBigInt *sIn, SBigInt *sIn2)
{
    unsigned			carry=0;
    uint64_t	sum=0;
	int					i =0;

    if(sIn->m_iLength32<sIn2->m_iLength32)
		sOut->m_iLength32=sIn2->m_iLength32;
	else
		sOut->m_iLength32=sIn->m_iLength32;

    for(i=0;i<sOut->m_iLength32;i++)
    {
		sum=sIn->m_ulValue[i];
		sum=sum+sIn2->m_ulValue[i]+carry;
        sOut->m_ulValue[i]=(uint64_t)sum;
        carry=(unsigned)(sum>>32);
    }

    sOut->m_ulValue[sOut->m_iLength32]=carry;
    sOut->m_iLength32+=carry;

	for(i=sOut->m_iLength32;i<BIGINT_MAXLEN;i++)
		sOut->m_ulValue[i]=0;

    return;
}

//	sOut可同SIn
void	Add_long(SBigInt *sOut, SBigInt *sIn, uint64_t ulIn)
{
    uint64_t	sum =0;
	int					i =0;

	Mov_bigint(sOut,sIn);

    sum=sIn->m_ulValue[0];
	sum+=ulIn;
    sOut->m_ulValue[0]=(uint64_t)sum;
    if(sum>0xffffffff)
    {
        i=1;
        while(sIn->m_ulValue[i]==0xffffffff)
		{
			sOut->m_ulValue[i]=0;
			i++;
		}
        sOut->m_ulValue[i]++;
        if(sOut->m_iLength32==i)
			sOut->m_iLength32++;
    }

    return;
}

/****************************************************************************************
大数相减
调用形式：	后两参数想减赋值给前一参数，输出对象可以是第一个输入对象本身
返回值：	无
****************************************************************************************/
//	sOut可同SIn
void	Sub_bigint(SBigInt *sOut, SBigInt *sIn, SBigInt *sIn2)
{
    unsigned			carry=0;
    uint64_t	num =0;
	int					i =0;

    if(Cmp(sIn,sIn2)<=0)
	{
		Mov_long(sOut,0);
		return;
	}
//	else
//		Mov(sOut,sIn);
	sOut->m_iLength32=sIn->m_iLength32;

    for(i=0;i<sIn->m_iLength32;i++)
    {
        if((sIn->m_ulValue[i]>sIn2->m_ulValue[i])||((sIn->m_ulValue[i]==sIn2->m_ulValue[i])&&(carry==0)))
        {
            sOut->m_ulValue[i]=sIn->m_ulValue[i]-carry-sIn2->m_ulValue[i];
            carry=0;
        }
        else
        {
            num=0x100000000+sIn->m_ulValue[i];
            sOut->m_ulValue[i]=(uint64_t)(num-carry-sIn2->m_ulValue[i]);
            carry=1;
        }
    }
    while(sOut->m_ulValue[sOut->m_iLength32-1]==0)
		sOut->m_iLength32--;

	for(i=sOut->m_iLength32;i<BIGINT_MAXLEN;i++)
		sOut->m_ulValue[i]=0;

    return;
}

//	sOut可同SIn
void	Sub_long(SBigInt *sOut, SBigInt *sIn, uint64_t ulIn)
{
	uint64_t num =0;
	int i =0;

	sOut->m_iLength32=sIn->m_iLength32;

    if(sIn->m_ulValue[0]>=ulIn)
	{
		sOut->m_ulValue[0]=sIn->m_ulValue[0]-ulIn;

		for(i=1;i<BIGINT_MAXLEN;i++)
			sOut->m_ulValue[i]=sIn->m_ulValue[i];

		return;
	}

    if(sIn->m_iLength32==1)
	{
		Mov_long(sOut,0);
		return;
	}

	num=0x100000000+sIn->m_ulValue[0];
    sOut->m_ulValue[0]=(uint64_t)(num-ulIn);
    i=1;
    while(sIn->m_ulValue[i]==0)
	{
		sOut->m_ulValue[i]=0xffffffff;
		i++;
	}
    sOut->m_ulValue[i]=sIn->m_ulValue[i]-1;
    if(sOut->m_ulValue[i]==0)
		sOut->m_iLength32--;

	for(i=sOut->m_iLength32;i<BIGINT_MAXLEN;i++)
		sOut->m_ulValue[i]=0;

    return;
}

//	sOut可同SIn
void Mul_long(SBigInt *sOut, SBigInt *sIn, uint64_t ulIn)
{
    uint64_t	mul =0;
    uint64_t		carry =0;
	int					i =0;

	carry=0;
	sOut->m_iLength32=sIn->m_iLength32;

    for(i=0;i<sOut->m_iLength32;i++)
    {
        mul=sIn->m_ulValue[i];
		mul=mul*ulIn+carry;
        sOut->m_ulValue[i]=(uint64_t)mul;
        carry=(uint64_t)(mul>>32);
    }

    if(carry)
	{
		sOut->m_ulValue[sOut->m_iLength32]=carry;
		sOut->m_iLength32++;
	}

	for(i=sOut->m_iLength32;i<BIGINT_MAXLEN;i++)
		sOut->m_ulValue[i]=0;

    return;
}

/****************************************************************************************
大数相乘
调用形式：	后两参数想乘赋值给前一参数，输出对象"不"!!!!可以是第一个输入对象
返回值：	无
****************************************************************************************/
//	sOut不可同SIn !!!!!!!!
void Mul_bigint(SBigInt *sOut, SBigInt *sIn, SBigInt *sIn2)
{
	uint64_t sum =0;
	uint64_t mul=0;
	uint64_t carry=0;
	int	i=0;
	int j=0;
	int k=0;

    if(sIn2->m_iLength32==1)
	{
		Mul_long(sOut,sIn,sIn2->m_ulValue[0]);
		return;
	}

	sOut->m_iLength32=sIn->m_iLength32+sIn2->m_iLength32-1;
    for(i=0;i<sOut->m_iLength32;i++)
	{
		sum=carry;
		carry=0;
		for(j=0;j<sIn2->m_iLength32;j++)
		{
			k=i-j;
            if((k>=0)&&(k<sIn->m_iLength32))
			{
				mul=sIn->m_ulValue[k];
				mul=mul*sIn2->m_ulValue[j];
			    carry+=mul>>32;
				mul=mul&0xffffffff;
				sum+=mul;
			}
        }
		carry+=sum>>32;
		sOut->m_ulValue[i]=(uint64_t)sum;
	}
	if(carry)
	{
		sOut->m_iLength32++;
		sOut->m_ulValue[sOut->m_iLength32-1]=(uint64_t)carry;
	}

	for(i=sOut->m_iLength32;i<BIGINT_MAXLEN;i++)
		sOut->m_ulValue[i]=0;

    return;
}


//	sOut可同SIn
void	Div_long(SBigInt *sOut, SBigInt *sIn, uint64_t ulIn)
{
    uint64_t div = 0;
	uint64_t mul = 0;
    uint64_t carry = 0;
	int	i = 0;

    sOut->m_iLength32=sIn->m_iLength32;

    if(sIn->m_iLength32==1)
	{
		sOut->m_ulValue[0]=sIn->m_ulValue[0]/ulIn;
		return;
	}

    for(i=sIn->m_iLength32-1;i>=0;i--)
    {
        div=carry;
        div=(div<<32)+sIn->m_ulValue[i];
        sOut->m_ulValue[i]=(uint64_t)(div/ulIn);
        mul=(div/ulIn)*ulIn;
        carry=(uint64_t)(div-mul);
    }
    if(sOut->m_ulValue[sOut->m_iLength32-1]==0)
		sOut->m_iLength32--;

	for(i=sOut->m_iLength32;i<BIGINT_MAXLEN;i++)
		sOut->m_ulValue[i]=0;

    return;
}

/****************************************************************************************
大数相除
调用形式：	后两参数想除赋值给前一参数，输出对象"不"!!!!可以是第一个输入对象
返回值：	无
****************************************************************************************/
//	sOut不可同SIn!!!!!!!!
void	Div_bigint(SBigInt *sOut, SBigInt *sIn, SBigInt *sIn2)
{
    unsigned int i = 0;
	unsigned int len = 0;
    uint64_t num = 0;
	uint64_t div = 0;
	SBigInt	sTemp1 = {0};
	SBigInt sTemp2 = {0};
	SBigInt sTemp3 = {0};

	if(sIn2->m_iLength32==1)
	{
		Div_long(sOut, sIn, sIn2->m_ulValue[0]);
		return;
	}

    Mov_bigint(&sTemp1,sIn);
	Mov_long(sOut,0);

    while(Cmp(&sTemp1,sIn2)>=0)
    {
		div=sTemp1.m_ulValue[sTemp1.m_iLength32-1];
		num=sIn2->m_ulValue[sIn2->m_iLength32-1];
		len=sTemp1.m_iLength32-sIn2->m_iLength32;
		if((div==num)&&(len==0))
		{
			Add_long(sOut,sOut,1);
			break;
		}
		if((div<=num)&&len)
		{
			len--;
			div=(div<<32)+sTemp1.m_ulValue[sTemp1.m_iLength32-2];
		}
		div=div/(num+1);
		Mov_long(&sTemp2,div);
		if(len)
		{
			sTemp2.m_iLength32+=len;
			for(i=sTemp2.m_iLength32-1;i>=len;i--)
				sTemp2.m_ulValue[i]=sTemp2.m_ulValue[i-len];
			for(i=0;i<len;i++)
				sTemp2.m_ulValue[i]=0;
		}
		Add_bigint(sOut,sOut,&sTemp2);
		Mul_bigint(&sTemp3,sIn2,&sTemp2);
		Sub_bigint(&sTemp1,&sTemp1,&sTemp3);
    }
    return;
}


/****************************************************************************************
大数求模
调用形式：	后两参数求余赋值给前一参数，输出对象可以是第一个输入对象本身
返回值：	无或者unsigned long
****************************************************************************************/
//	sOut可同SIn
void Mod_bigint(SBigInt *sOut, SBigInt *sIn, SBigInt *sIn2)
{
    SBigInt	sTemp1 = {0};
	SBigInt sTemp2 = {0};
	uint64_t div = 0;
	uint64_t num = 0;
    uint64_t carry = 0;
	unsigned int i = 0;
	unsigned int len = 0;

    Mov_bigint(sOut,sIn);
    while(Cmp(sOut,sIn2)>=0)
    {
		div=sOut->m_ulValue[sOut->m_iLength32-1];
		num=sIn2->m_ulValue[sIn2->m_iLength32-1];
		len=sOut->m_iLength32-sIn2->m_iLength32;
		if((div==num)&&(len==0))
		{
			Sub_bigint(sOut,sOut,sIn2);
			break;
		}
		if((div<=num)&&len)
		{
			len--;
			div=(div<<32)+sOut->m_ulValue[sOut->m_iLength32-2];
		}
		div=div/(num+1);
		Mov_long(&sTemp1,div);
		Mul_bigint(&sTemp2,sIn2,&sTemp1);
		if(len)
		{
			sTemp2.m_iLength32+=len;
			for(i=sTemp2.m_iLength32-1;i>=len;i--)
				sTemp2.m_ulValue[i]=sTemp2.m_ulValue[i-len];
			for(i=0;i<len;i++)
				sTemp2.m_ulValue[i]=0;
		}
		Sub_bigint(sOut,sOut,&sTemp2);
    }

    return;
}

//	sOut可同SIn
uint64_t	Mod_long(SBigInt *sIn, uint64_t ulIn)
{
    uint64_t div = 0;
    uint64_t carry = 0;
	int	i = 0;

    if(sIn->m_iLength32==1)
		return sIn->m_ulValue[0]%ulIn;

    for(i=sIn->m_iLength32-1;i>=0;i--)
    {
        div=sIn->m_ulValue[i]+carry*0x100000000;
        carry=(uint64_t)(div%ulIn);
    }

    return carry;
}

/****************************************************************************************
从字符串按10进制或16进制格式输入到大数
调用格式：	后字符串参数赋值给前一SBigInt参数，字符串格式只能为10或16
返回值：	无
****************************************************************************************/
/*void SetParameter_char(SBigInt *sOut, unsigned char * str, int system)*/
/*{*/
    /*int		i,strLen,k;*/
	/*SBigInt	sTemp;*/

	/*strLen=strlen((char *)str);//str.GetLength();*/

	/*for(i=0;i<BIGINT_MAXLEN;i++)*/
		/*sOut->m_ulValue[i]=0;*/

    /*for(i=0;i<strLen;i++)*/
    /*{*/
       /*Mul_long(&sTemp,sOut,system);*/

       /*if((str[i]>='0')&&(str[i]<='9'))*/
		   /*k=str[i]-48;*/
       /*else if((str[i]>='A')&&(str[i]<='F'))*/
		   /*k=str[i]-55;*/
       /*else if((str[i]>='a')&&(str[i]<='f'))*/
		   /*k=str[i]-87;*/
       /*else k=0;*/

	   /*Add_long(sOut,&sTemp,k);*/
    /*}*/

	/*return;*/
/*}*/

void SetParameter_int(SBigInt *sOut, unsigned char ulIn[], int iLength32)
{
    int	i = 0;

    HALO_Memcpy(sOut->m_ulValue,ulIn,iLength32*sizeof(unsigned int));
	/*for(i=0;i<iLength32;i++)*/
		/*sOut->m_ulValue[i]=ulIn[i];*/


	for(i=iLength32;i<BIGINT_MAXLEN;i++)
		sOut->m_ulValue[i]=0;

	sOut->m_iLength32=iLength32;

	return;
}
void SetParameter_char_openssl(SBigInt *sOut, unsigned char * str, int strLen)
{
       invert_char(str,strLen);
       SetParameter_int(sOut, str,strLen>>2);
	 invert_char(str,strLen);//recover the original input
	return;
}

/****************************************************************************************
将大数按10进制或16进制格式输出为字符串
调用格式：	把大数赋值给字符串，字符串格式只能为10或16
返回值：	无，参数str被赋值为N的sys进制字符串
****************************************************************************************/
void Put(unsigned char * str,int str_buflen, SBigInt *sOut, unsigned int system)
{
    unsigned char t[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    int a = 0;
    unsigned char ch = 0;
	SBigInt	sTemp = {0};
	SBigInt sTemp2 = {0};

	int index=str_buflen-1;

    if((sOut->m_iLength32==1)&&(sOut->m_ulValue[0]==0))
	{
		str[0]='0';
		str[1]='\0';
		return;
	}

	str[index]='\0';

    Mov_bigint(&sTemp,sOut);

    while(sTemp.m_ulValue[sTemp.m_iLength32-1]>0)
    {
		a=Mod_long(&sTemp,system);
        ch=t[a];
        //str.Insert(0,ch);
        //start insert to head
        index-=1;
	  str[index]=ch;
	 //end insert to head

		Div_long(&sTemp2,&sTemp,system);
		Mov_bigint(&sTemp,&sTemp2);
    }

    if (index>0)
    {
        int i=0;
	  while (index<str_buflen)
		str[i++]=str[index++];
    }
}

/****************************************************************************************
求M指数D对N的模
调用方式：	第一个为输出，不能和输入相同
返回值：	sOut=sIn^sE MOD sN
****************************************************************************************/
void	RsaTrans(SBigInt *sOut, SBigInt *sIn, SBigInt *sE, SBigInt *sN)
{
	int	i = 0;
	int j = 0;
	int k = 0;
	int	n = 0;
	uint64_t num = 0;

	SBigInt	sTemp1 = {0};
	SBigInt sTemp2 = {0};

	//	k为指数的bit数
	k=sE->m_iLength32*32-32;
	num=sE->m_ulValue[sE->m_iLength32-1];

	while(num)
	{
		num=num>>1;
		k++;
	}

	Mov_bigint(sOut,sIn);

	for(i=k-2;i>=0;i--)
	{
		Mul_long(&sTemp1, sOut, sOut->m_ulValue[sOut->m_iLength32-1]);
		Mod_bigint(&sTemp1,&sTemp1, sN);

        for(n=1;n<sOut->m_iLength32;n++)
		{

			for(j=sTemp1.m_iLength32;j>0;j--)
				sTemp1.m_ulValue[j]=sTemp1.m_ulValue[j-1];

			sTemp1.m_ulValue[0]=0;
			sTemp1.m_iLength32++;

			Mul_long(&sTemp2,sOut,sOut->m_ulValue[sOut->m_iLength32-n-1]);
			Add_bigint(&sTemp1,&sTemp1,&sTemp2);
			Mod_bigint(&sTemp1,&sTemp1,sN);
		}

		Mov_bigint(sOut,&sTemp1);

		if((sE->m_ulValue[i>>5]>>(i&31))&1)
		{
			Mul_long(&sTemp1,sIn,sOut->m_ulValue[sOut->m_iLength32-1]);
			Mod_bigint(&sTemp1,&sTemp1,sN);

            for(n=1;n<sOut->m_iLength32;n++)
			{
			    for(j=sTemp1.m_iLength32;j>0;j--)
					sTemp1.m_ulValue[j]=sTemp1.m_ulValue[j-1];

			    sTemp1.m_ulValue[0]=0;
			    sTemp1.m_iLength32++;

				Mul_long(&sTemp2,sIn,sOut->m_ulValue[sOut->m_iLength32-n-1]);
				Add_bigint(&sTemp1,&sTemp1,&sTemp2);
				Mod_bigint(&sTemp1,&sTemp1,sN);
			}
			Mov_bigint(sOut,&sTemp1);
		}
	}
    return;
}

/****************************************************************************************
加密
调用方式：	无
返回值：	C=M^E MOD N, 成功返回1，否则非1值
****************************************************************************************/
int	prv_Encrypt()
{
	if(Cmp(&RSA_M,&RSA_N)>=0)
	{
		return	-1;
	}

	RsaTrans(&RSA_C,& RSA_M,&RSA_E, &RSA_N);

    return 1;
}

/****************************************************************************************
解密
调用方式：	无
返回值：	M=C^D MOD N， 成功返回1，否则非1值
****************************************************************************************/
int	pub_Decrypt()
{
	if(Cmp(&RSA_C,&RSA_N)>=0)
	{
		return	-1;
	}

	RsaTrans(&RSA_M, &RSA_C, &RSA_D, &RSA_N);

	return 1;
}

/****************************************************************************************
加密
调用方式：	无
返回值：	C=M^E MOD N, 成功返回1，否则非1值
****************************************************************************************/
int	pub_Encrypt()
{
	if(Cmp(&RSA_M,&RSA_N)>=0)
	{
		return	-1;
	}

	RsaTrans(&RSA_C,& RSA_M,&RSA_D, &RSA_N);

    return 1;
}

/****************************************************************************************
解密
调用方式：	无
返回值：	M=C^D MOD N， 成功返回1，否则非1值
****************************************************************************************/
int	prv_Decrypt()
{
	if(Cmp(&RSA_C,&RSA_N)>=0)
	{
		return	-1;
	}

	RsaTrans(&RSA_M, &RSA_C, &RSA_E, &RSA_N);

	return 1;
}
boolean  RSA_PrvEnc(unsigned char* prv_E, unsigned char* mod_N,int  bitLen_N, unsigned char *from,int flen, unsigned char*to )
{
   int byteLen_N = 0;
   unsigned char *textpadded = NULL;
   byteLen_N=bitLen_N>>3;
  // unsigned char textpadded[byteLen_N];
  //@shawn begin
  	textpadded = HALO_Malloc(byteLen_N);
  	if(!textpadded)
		{
			return false;
		}
		//@shawn end
       RSA_initialize();
       if (prv_E!=NULL)
	     SetParameter_char_openssl(&RSA_E,prv_E,byteLen_N);
	if (mod_N!=NULL)
	     SetParameter_char_openssl(&RSA_N,mod_N,byteLen_N);

	 if((8 == byteLen_N)||(4 == byteLen_N))
	{
	        HALO_Memcpy(textpadded,from,byteLen_N);
	}
	else
	{
		if (padding_add_PKCS1_type_1(textpadded, byteLen_N,
		 from, flen)<=0) //@shawn begin
		 {
		 	HALO_Free(textpadded);
		 	textpadded = NULL;
		 	return false;
		}//@shawn end
	}
		
	 //RSA_invert_char(textpadded,byteLen_N);

	 SetParameter_char_openssl(&RSA_M,textpadded,byteLen_N);

	 if (prv_Encrypt()<0)//@shawn begin
	 	{
	 		HALO_Free(textpadded);
	    textpadded = NULL;
	 		return false;
		}//@shawn end
        /*unsigned int *cipher_int=to;*/

        HALO_Memcpy(to,(unsigned char *)RSA_C.m_ulValue,RSA_C.m_iLength32*sizeof(unsigned int));

        invert_char(to,byteLen_N);
        //@shawn begin
        HALO_Free(textpadded);
	     	textpadded = NULL;
	     	//@shawn end
	  return true;
}

boolean  RSA_PubDec(unsigned char* pub_D,unsigned char* mod_N,int  bitLen_N,unsigned char *from,unsigned char *to)
{
		
      int byteLen_N = 0;
      int ret = 0;
      unsigned char *text = NULL;
      byteLen_N=bitLen_N>>3;
      //unsigned char text[byteLen_N];
      //@shawn begin
    
  		text = HALO_Malloc(byteLen_N);
  		if(!text)
			{
					return false;
			}
		//@shawn end
			
	/*unsigned int * text_int=text;*/
       RSA_initialize();

       if (pub_D!=NULL)
	     		SetParameter_char_openssl(&RSA_D,pub_D,4);
			if (mod_N!=NULL)
	     SetParameter_char_openssl(&RSA_N,mod_N,byteLen_N);

        SetParameter_char_openssl(&RSA_C,from,byteLen_N);

        pub_Decrypt();

        HALO_Memcpy(text, (unsigned char *)RSA_M.m_ulValue, RSA_M.m_iLength32*sizeof(unsigned int));
        
        /*for (int i=0;i<RSA_M.m_iLength32;i++)*/
        /*{*/
            /*text_int[i]=RSA_M.m_ulValue[i];*/
        /*}*/

	  invert_char(text,byteLen_N);
		
	 if((8 == byteLen_N)||(4 == byteLen_N))
 	{
		HALO_Memcpy(to,text,byteLen_N);
		if(text != NULL)
		HALO_Free(text);
		return byteLen_N;
 	}
	else
	{

		ret = padding_check_PKCS1_type_1(to,byteLen_N,
		text+1, byteLen_N-1,byteLen_N);
		//@shawn begin
		HALO_Free(text);
		text = NULL;
		//@shawn end
		return ret;
	}	     
}

boolean  RSA_PubEnc(unsigned char* pub_D, unsigned char* mod_N,int  bitLen_N, unsigned char *from,int flen, unsigned char*to )
{
   int byteLen_N = 0;
   unsigned char *textpadded = NULL;
   byteLen_N=bitLen_N>>3;
   //unsigned char textpadded[byteLen_N];
    //@shawn begin
   	textpadded = HALO_Malloc(byteLen_N);
  	if(!textpadded)
		{
			return false;
		}
		//@shawn end
	RSA_initialize();
       if (pub_D!=NULL)
       {
	     SetParameter_char_openssl(&RSA_D,pub_D,4);
       }
	if (mod_N!=NULL)
	{
	     SetParameter_char_openssl(&RSA_N,mod_N,byteLen_N);
	}
        if (padding_add_PKCS1_type_2(textpadded, byteLen_N,
	     from, flen)<=0) //@shawn begin
	     {
	     	HALO_Free(textpadded);
	     	textpadded = NULL;
	     	return false;
	    }//@shawn end
	  
	 SetParameter_char_openssl(&RSA_M,textpadded,byteLen_N);

	 if (pub_Encrypt()<0) //@shawn begin
	     {
	     	HALO_Free(textpadded);
	     	textpadded = NULL;
	     	return false;
	    }//@shawn end
	 	
        HALO_Memcpy(to,(unsigned char *)RSA_C.m_ulValue,RSA_C.m_iLength32*sizeof(unsigned int));
        /*unsigned int *cipher_int=to;*/
        /*printf("cipher_int=%p\n",cipher_int);*/

        /*for (int i=0;i<RSA_C.m_iLength32;i++)*/
        /*{*/
        /*printf("cipher_int=%p\n",cipher_int);*/
            /*cipher_int[i]=RSA_C.m_ulValue[i];*/
        /*}*/

        invert_char(to,byteLen_N);
         //@shawn begin
        HALO_Free(textpadded);
	     	textpadded = NULL;
	     	//@shawn end
	  return true;
}
boolean RSA_PrvDec(unsigned char* prv_E,unsigned char* mod_N,int  bitLen_N, unsigned char *from,unsigned char *to)
{
      int byteLen_N = 0;
      int ret = 0;
      unsigned char *text = NULL;
  		byteLen_N=bitLen_N>>3;
      //unsigned char text[byteLen_N];
      //@shawn begin
      text = HALO_Malloc(byteLen_N);
  		if(!text)
			{
					return false;
			}
		//@shawn end
	//unsigned int * text_int=text;
	RSA_initialize();
       if (prv_E!=NULL)
       {
	     SetParameter_char_openssl(&RSA_E,prv_E,byteLen_N);
       }

	if (mod_N!=NULL)
	{
	     SetParameter_char_openssl(&RSA_N,mod_N,byteLen_N);
	}

        SetParameter_char_openssl(&RSA_C,from,byteLen_N);

        prv_Decrypt();

        HALO_Memcpy(text, (unsigned char *)RSA_M.m_ulValue, RSA_M.m_iLength32*sizeof(unsigned int));
        /*for (int i=0;i<RSA_M.m_iLength32;i++)*/
        /*{*/
            /*text_int[i]=RSA_M.m_ulValue[i];*/
        /*}*/

	  invert_char(text,byteLen_N);

         ret = padding_check_PKCS1_type_2(to,byteLen_N,
	     text+1, byteLen_N-1,byteLen_N);
	      //@shawn begin
        HALO_Free(text);
	     	text = NULL;
	     	//@shawn end
	     	return ret;
}


/****************************************************************************************
求随机数
调用方式：	无
返回值：	unsigned long值
****************************************************************************************/ 
#define RAND_MAX ((1U << 31) - 1)
static int g_seed = 0;
static int read_reg = 1;

static int myrand()
{
    if(read_reg)
    {
        g_seed = 504723;
        if(g_seed>0) read_reg = 0;
    }
return g_seed = (g_seed * 1103515245 + 12345) & RAND_MAX;
}

uint64_t  Rand()
{
    int ulNum = 0;
    ulNum=myrand()*0x10000+myrand();
    return ulNum;
}

