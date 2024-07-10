/*************************************************************************
IIMEfile.c

Description: 

**************************************************************************/

#include "IIMEstd.h"
#include "IIMEmemory.h"

#ifdef SELFALLOC_PRODUCT

#if defined(SPREATRUM_MINI_PRODUCT) || defined(LONGCHEER_MINI_PRODUCT)
#define MAX_USED_BUFFER (8 * 1024)
#else
#define MAX_USED_BUFFER (22 * 1024)
#endif

IIME_UINT g_pBlocks[MAX_USED_BUFFER / sizeof(IIME_UINT)];

IIME_BYTE *g_pBase = (IIME_BYTE *)g_pBlocks;
IIME_BYTE *g_pAvalible = IIME_NULL;

void IIMEMemory_SelfReset(void* pFrom)
{
    if (pFrom == IIME_NULL){
		g_pAvalible = (IIME_BYTE *)g_pBlocks;
        g_pBase = (IIME_BYTE *)g_pBlocks;
    }
	else
		g_pAvalible = pFrom;
}

void *IIMEMemory_SelfAlloc(unsigned int size)
{
	void *p;
	if (g_pAvalible == IIME_NULL || (MAX_USED_BUFFER - (g_pAvalible - g_pBase)) < (int)size)
		return 0;

	p = g_pAvalible;
	g_pAvalible += size;

	return p;
}

void IIMEMemory_SelfFree(void* pBlock)
{
	return;	/* do nothing*/
}


#endif /* SELFALLOC_PRODUCT*/
