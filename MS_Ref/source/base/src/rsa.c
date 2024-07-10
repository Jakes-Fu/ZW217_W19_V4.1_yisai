/******************************************************************************
 ** File Name:    rsa.c                                                       *
 ** Author:       				                                              *
 ** DATE:         9/25/2009                                                   *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**----------------------------------------------------------------------------*/
#include "sci_types.h"
#include "rsa.h"
#include "string.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

extern int32 rsa_sub(unsigned int *pt,unsigned int *pm);
extern void  rsa_multidw(unsigned int *pt,unsigned int *ps,unsigned int m);

static void MontgomeryMulit(unsigned int *p, unsigned int *x, unsigned int *m, unsigned int _m)
{
	unsigned int k;
	unsigned int t[65];
	unsigned int *ut = t;
	unsigned int *ux = x;
	unsigned int *uy = p;
	int i=0;
	
	for(i=0; i<65;i++) t[i]=0;
	
	for(i=0; i < 32; i++)
	{
		rsa_multidw(ut,ux,*uy);
		uy++;
		k = *ut * _m;
		rsa_multidw(ut,m,k);
		ut++;
	}
    
	if ( 0 == rsa_sub(ut,m) ) ut -= 32;
	memcpy(p,ut,32*4);
	
	return;
}

static void MontgomeryReduce(unsigned int *p, unsigned int *m, unsigned int _m)
{
	unsigned int k;
	unsigned int t[65];
	unsigned int *ut = t;
	//unsigned int *uy = p;
	int i=0;
	
	for(i=32; i<65;i++) t[i]=0;
	for(i=0; i<32;i++) t[i]=p[i];
	
	for(i=0; i < 32; i ++)
	{
		k = *ut * _m;
		rsa_multidw(ut,m,k);
		ut++; 
	}
	
	if ( 0 == rsa_sub(ut,m) ) ut -= 32;
	memcpy(p,ut,32*4);
	
	return;
}

static unsigned GetM(unsigned int m) 
{
	unsigned r = 1;
	unsigned s = m;
	unsigned bit = 1;
	int i = 0;
	
	for(i = 0; i < 32; i ++)
	{
		if((s & 1) ==0)
		{
			s += m;
			r |= bit;
		}
		s >>= 1;
		bit <<= 1;
	}
	
	return r;
}

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:
//	Note:           p  = p exp (e) MOD m
//	Note:           r2 = 2 exp (k)
/*****************************************************************************/
void RSA_ModPower(unsigned int *p, unsigned int *m,
			  unsigned int *r2, unsigned int e)
{
	unsigned int x[32];
	unsigned int _m;
	unsigned int test = 0x80000000;
	int i,j;
	
	if ( (PNULL == p) ||
	     (PNULL == m) ||
	     (PNULL == r2) )
	{
		return;
	}
	
	_m = GetM(m[0]); //求模数低32位的负逆,供计算退位因子K使用
	MontgomeryMulit(p, r2, m, _m);
	
	for (i=0; i<32; i++) x[i] = p[i];
	
	for (j=0; j<32; j++)
	{
		if (e&test) break;
		test >>= 1;
	}
	test >>= 1;
	j++;
	
	for ( ; j<32; j++)
	{
		MontgomeryMulit(p, p, m, _m);
		if (e&test) MontgomeryMulit(p, x, m, _m);
		test >>= 1;
	}
	MontgomeryReduce(p,m,_m);
	
	return;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End
