#ifndef		__SHA1_H__
#define		__SHA1_H__

#include "sci_types.h"

typedef 	unsigned long		CrULONG32;
typedef 	unsigned long		CrUINT32;
typedef 	int					CrINT32;
typedef 	unsigned char		CrUINT8;

#define ULONG	unsigned long
#define UNLOCK_KEY_MAX		32

#define SHA_CBLOCK	64
#define SHA_LBLOCK	16
#define SHA_BLOCK	16
#define SHA_LAST_BLOCK  56
#define SHA_LENGTH_BLOCK 8
#define SHA_DIGEST_LENGTH 20

#define K_00_19	0x5a827999L
#define K_20_39 0x6ed9eba1L
#define K_40_59 0x8f1bbcdcL
#define K_60_79 0xca62c1d6L

typedef struct SHAstate_st
{
	CrULONG32 h0,h1,h2,h3,h4;
	CrULONG32 Nl,Nh;
	CrULONG32 data[SHA_LBLOCK];
	CrINT32 num;
} SHA_CTX;

#define c2nl(c,l)	(l =(((CrULONG32)(*((c)++)))<<24), \
			 l|=(((CrULONG32)(*((c)++)))<<16), \
			 l|=(((CrULONG32)(*((c)++)))<< 8), \
			 l|=(((CrULONG32)(*((c)++)))    ))

#undef p_c2nl
#define p_c2nl(c,l,n)	{ \
			switch (n) { \
			case 0: l =((CrULONG32)(*((c)++)))<<24; \
			case 1: l|=((CrULONG32)(*((c)++)))<<16; \
			case 2: l|=((CrULONG32)(*((c)++)))<< 8; \
			case 3: l|=((CrULONG32)(*((c)++))); \
				} \
			}

#undef c2nl_p
/* NOTE the pointer is not incremented at the end of this */
#define c2nl_p(c,l,n)	{ \
			l=0; \
			(c)+=n; \
			switch (n) { \
			case 3: l =((CrULONG32)(*(--(c))))<< 8; \
			case 2: l|=((CrULONG32)(*(--(c))))<<16; \
			case 1: l|=((CrULONG32)(*(--(c))))<<24; \
				} \
			}

#undef p_c2nl_p
#define p_c2nl_p(c,l,sc,len) { \
			switch (sc) \
				{ \
			case 0: l =((CrULONG32)(*((c)++)))<<24; \
				if (--len == 0) break; \
			case 1: l|=((CrULONG32)(*((c)++)))<<16; \
				if (--len == 0) break; \
			case 2: l|=((CrULONG32)(*((c)++)))<< 8; \
				} \
			}

#undef nl2c
#define nl2c(l,c)	(*((c)++)=(CrUINT8)(((l)>>24)&0xff), \
			 *((c)++)=(CrUINT8)(((l)>>16)&0xff), \
			 *((c)++)=(CrUINT8)(((l)>> 8)&0xff), \
			 *((c)++)=(CrUINT8)(((l)    )&0xff))

#undef c2l
#define c2l(c,l)	(l =(((CrULONG32)(*((c)++)))    ), \
			 l|=(((CrULONG32)(*((c)++)))<< 8), \
			 l|=(((CrULONG32)(*((c)++)))<<16), \
			 l|=(((CrULONG32)(*((c)++)))<<24))

#undef p_c2l
#define p_c2l(c,l,n)	{ \
			switch (n) { \
			case 0: l =((CrULONG32)(*((c)++))); \
			case 1: l|=((CrULONG32)(*((c)++)))<< 8; \
			case 2: l|=((CrULONG32)(*((c)++)))<<16; \
			case 3: l|=((CrULONG32)(*((c)++)))<<24; \
				} \
			}

#undef c2l_p
/* NOTE the pointer is not incremented at the end of this */
#define c2l_p(c,l,n)	{ \
			l=0; \
			(c)+=n; \
			switch (n) { \
			case 3: l =((CrULONG32)(*(--(c))))<<16; \
			case 2: l|=((CrULONG32)(*(--(c))))<< 8; \
			case 1: l|=((CrULONG32)(*(--(c)))); \
				} \
			}

#undef p_c2l_p
#define p_c2l_p(c,l,sc,len) { \
			switch (sc) \
				{ \
			case 0: l =((CrULONG32)(*((c)++))); \
				if (--len == 0) break; \
			case 1: l|=((CrULONG32)(*((c)++)))<< 8; \
				if (--len == 0) break; \
			case 2: l|=((CrULONG32)(*((c)++)))<<16; \
				} \
			}

#undef l2c
#define l2c(l,c)	(*((c)++)=(CrUINT8)(((l)    )&0xff), \
			 *((c)++)=(CrUINT8)(((l)>> 8)&0xff), \
			 *((c)++)=(CrUINT8)(((l)>>16)&0xff), \
			 *((c)++)=(CrUINT8)(((l)>>24)&0xff))


#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

#if defined(WIN32)
/* 5 instructions with rotate instruction, else 9 */
#define Endian_Reverse32(a) \
	{ \
	CrULONG32 l=(a); \
	(a)=((ROTATE(l,8)&0x00FF00FF)|(ROTATE(l,24)&0xFF00FF00)); \
	}
#else
/* 6 instructions with rotate instruction, else 8 */
#define Endian_Reverse32(a) \
	{ \
	CrULONG32 l=(a); \
	l=(((l&0xFF00FF00)>>8L)|((l&0x00FF00FF)<<8L)); \
	(a)=ROTATE(l,16L); \
	}
#endif


#define	F_00_19(b,c,d)	((((c) ^ (d)) & (b)) ^ (d)) 
#define	F_20_39(b,c,d)	((b) ^ (c) ^ (d))
#define F_40_59(b,c,d)	(((b) & (c)) | (((b)|(c)) & (d))) 
#define	F_60_79(b,c,d)	F_20_39(b,c,d)

#undef Xupdate
#define Xupdate(a,i,ia,ib,ic,id) (a)=\
	(ia[(i)&0x0f]^ib[((i)+2)&0x0f]^ic[((i)+8)&0x0f]^id[((i)+13)&0x0f]);\
	X[(i)&0x0f]=(a)=ROTATE((a),1);

#define BODY_00_15(i,a,b,c,d,e,f,xa) \
	(f)=xa[i]+(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_16_19(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	Xupdate(f,i,xa,xb,xc,xd); \
	(f)+=(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_20_31(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	Xupdate(f,i,xa,xb,xc,xd); \
	(f)+=(e)+K_20_39+ROTATE((a),5)+F_20_39((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_32_39(i,a,b,c,d,e,f,xa) \
	Xupdate(f,i,xa,xa,xa,xa); \
	(f)+=(e)+K_20_39+ROTATE((a),5)+F_20_39((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_40_59(i,a,b,c,d,e,f,xa) \
	Xupdate(f,i,xa,xa,xa,xa); \
	(f)+=(e)+K_40_59+ROTATE((a),5)+F_40_59((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_60_79(i,a,b,c,d,e,f,xa) \
	Xupdate(f,i,xa,xa,xa,xa); \
	(f)=X[(i)&0x0f]+(e)+K_60_79+ROTATE((a),5)+F_60_79((b),(c),(d)); \
	(b)=ROTATE((b),30);

//BOOLEAN SysCrHashSHA1(char *szIn, char *szOut);
//BOOLEAN SysCrHashSHA1_20ByteIn(char *szIn, char *szOut);
//BOOLEAN SysCrHashSHA1_InfineonPTE(char *szIn, unsigned int size, char *szOut);

void		ATC_SHA1_Init(SHA_CTX *c);
void		ATC_SHA1_Update(SHA_CTX *c, CrUINT8 *data, CrULONG32 len);
void		ATC_SHA1_Final(CrUINT8 *md, SHA_CTX *c);
CrUINT8*	SHA1(CrUINT8 *d, CrULONG32 n,CrUINT8 *md);
void		SHA1_Transform(SHA_CTX *c, CrUINT8 *data);
void		sha1_block(SHA_CTX *c, register CrULONG32 *p, CrINT32 num);

BOOLEAN Atc_SysCrHashSHA1(uint8 *szIn, uint8 *szOut);
#endif		//__SHA1_H__
