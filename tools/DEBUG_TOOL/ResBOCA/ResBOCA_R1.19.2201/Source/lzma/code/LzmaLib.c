/* LzmaLib.c -- LZMA library wrapper
2008-08-05
Igor Pavlov
Public domain */

#include "LzmaEnc.h"
#include "LzmaDec.h"
#include "Alloc.h"
#include "LzmaLib.h"
#include "Lzma86.h"
volatile int g_nHWCodec = 0;
static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

MY_STDAPI LzmaCompress(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  srcLen,
  unsigned char *outProps, size_t *outPropsSize,
  int level, /* 0 <= level <= 9, default = 5 */
  unsigned dictSize, /* use (1 << N) or (3 << N). 4 KB < dictSize <= 128 MB */
  int lc, /* 0 <= lc <= 8, default = 3  */
  int lp, /* 0 <= lp <= 4, default = 0  */
  int pb, /* 0 <= pb <= 4, default = 2  */
  int fb,  /* 5 <= fb <= 273, default = 32 */
  int numThreads /* 1 or 2, default = 2 */
)
{
  CLzmaEncProps props;
  LzmaEncProps_Init(&props);
  props.level = level;
  props.dictSize = dictSize;
  props.lc = lc;
  props.lp = lp;
  props.pb = pb;
  props.fb = fb;
  props.numThreads = numThreads;
  //@hongliang.xin 2011-5-6 modify to write end mark default
  return LzmaEncode(dest, destLen, src, srcLen, &props, outProps, outPropsSize, 0,
      NULL, &g_Alloc, &g_Alloc);
}

MY_STDAPI LzmaCompress2(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  srcLen,
  int level /* 0 <= level <= 9, default = 5 */,int bHWCodec
)
{
  SRes res;
  CLzmaEncProps props;
  unsigned char *outProps = dest;
  unsigned char *dest2 = dest + LZMA_PROPS_SIZE + 8;
  int i = 0;
  size_t propsSize = LZMA_PROPS_SIZE;
  g_nHWCodec = bHWCodec;
  LzmaEncProps_Init(&props);
  props.level = level;

  //////////////////////////////////////////////////////////////////////////
  ///using 5A mode
  if(bHWCodec)
  {
	  props.lc = 0;
	  props.pb = 2;
	  props.lp = 0;
  }
  //else using 5D mode

  //////////////////////////////////////////////////////////////////////////
  ///using 5D mode
//  props.lc = 0;
//  props.pb = 2;
//  props.lp = 0;
/*
// 2011-08-03 
#if 1
  props.lc = 4;
  props.lp = 1;
  props.pb = 2;
  props.fb = 32;
  props.dictSize = 256*1024;
  props.btMode = 0;
  props.algo = 0;
#else
  props.lc = 4;
  props.lp = 1;
  props.pb = 2;
  props.fb = 32;
  props.dictSize = 256*1024;
  props.btMode = 1;
  props.algo = 1;
#endif
*/ 

  for (i = 0; i < 4; i++)
      *(dest + LZMA_PROPS_SIZE+i) = (Byte)(srcLen >> (8 * i));

  *destLen = (*dest - LZMA_PROPS_SIZE - 8);

  res = LzmaEncode(dest2, destLen, src, srcLen, &props, outProps, &propsSize, 0,
      NULL, &g_Alloc, &g_Alloc);

  if(res == SZ_OK)
  {
	 *destLen += ( LZMA_PROPS_SIZE + 8);
  }

  g_nHWCodec = 0;

  return res;
}

MY_STDAPI LzmaCompress3(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  srcLen,
						int level /* 0 <= level <= 9, default = 5 */
						)
{
 	SRes res;

	UInt32 dictSize = 1<<23;

	res = Lzma86_Encode(dest,destLen, src, srcLen, level, dictSize, SZ_FILTER_YES);
	
	return res;
}

MY_STDAPI LzmaUncompress(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  *srcLen,
  const unsigned char *props, size_t propsSize)
{
  ELzmaStatus status;
  return LzmaDecode(dest, destLen, src, srcLen, props, (unsigned)propsSize, LZMA_FINISH_ANY, &status, &g_Alloc);
}

MY_STDAPI LzmaUncompress2(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  srcLen,int bHWCodec)
{
  SRes res;
  ELzmaStatus status;
  const unsigned char * props = src;
  size_t srcLen2 = srcLen-LZMA_PROPS_SIZE-8;
  const unsigned char * src2 = src+LZMA_PROPS_SIZE + 8;
  g_nHWCodec = bHWCodec;
  res = LzmaDecode(dest, destLen, src2, &srcLen2, props, (unsigned)LZMA_PROPS_SIZE, LZMA_FINISH_ANY, &status, &g_Alloc);
  g_nHWCodec = 0;
  return res;
}

MY_STDAPI LzmaUncompress3(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  srcLen)
{
	size_t  srcLen2 = srcLen;
	return Lzma86_Decode(dest,destLen,src,&srcLen2);	
}
