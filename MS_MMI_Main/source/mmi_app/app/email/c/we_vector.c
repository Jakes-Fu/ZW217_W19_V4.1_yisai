#include "we_vector.h"

#define WE_SUCCESS 0x00
#define WE_NOMEMORY 0x10
#define WE_BADPARM 0x21
#define WE_ERROR 0xFF

static void wevector_Dtor(IWEVector *me);
static void wevector_ExpandItems(IWEVector *me, uint32 nIndex, BOOLEAN bExpand);
static int wevector_MemGrowEx(IWEVector *p, void **ppMem, int *pcb, int cbUsed, int cbMore, int cbAllocExtra);

// local protos
int wevector_FreeItem(IWEVector *me, uint32 nIndex) 
{
   return wevector_ReplaceAt(me, nIndex, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// IWEVector

void wevector_Delete(IWEVector *me)
{
   //IWEMem *pIMemMgr = me->pIMemMgr;
   if(me)
   {
	  wevector_Dtor(me);
	   //IWESHELL_Release(me->pIWEShell);
	   //IWEMEM_Free(pIMemMgr, me);
	  free(me);
   }
   //IWEMEM_Release(pIMemMgr);
}


static void wevector_Dtor(IWEVector *me)
{
	if (me)
	{
		wevector_DeleteAll(me);
		//IWEMEM_Free(me->pIMemMgr, me->ppItems);
		free(me->ppItems);
	}
}


int wevector_GetAt(IWEVector *me, uint32 nIndex, void **ppoItem)
{
   if (nIndex >= me->nItems) 
   {
      *ppoItem = NULL;
      return WE_BADPARM;
   }
   
   *ppoItem = me->ppItems[nIndex];
   return WE_SUCCESS;
}


int wevector_ReplaceAt(IWEVector *me, uint32 nIndex, void *pvNew)
{
   void *pvItem = NULL;
   int nErr = wevector_GetAt(me, nIndex, &pvItem);
   
   if (!nErr) 
   {
      if (pvItem && me->pfnFree) 
      {
         me->pfnFree(pvItem);
         //free(pvItem);
      }
      me->ppItems[nIndex] = pvNew;
   }
   
   return nErr;
}


static void wevector_ExpandItems(IWEVector *me, uint32 nIndex, BOOLEAN bExpand)
{
   int nDir = bExpand ? 1 : -1;
   
   if (nIndex < me->nItems) 
   {
      uint32 nIndexSrc = nIndex;
      uint32 nIndexDest = nIndex;
      
      if (bExpand) 
      {
         nIndexDest++;
      } 
      else 
      {
         nIndexSrc++;
      }
      
      memmove(me->ppItems+nIndexDest, me->ppItems+nIndexSrc, (me->nItems - nIndexSrc) * sizeof(void *));
   }
   
   me->nItems += nDir;
   me->cbUsed = me->nItems * sizeof(void *);
}

static int wevector_MemGrowEx(IWEVector *p, void **ppMem, int *pcb, int cbUsed, int cbMore, int cbAllocExtra)
{
   void *pMem  = *ppMem;
   int  cb     = *pcb;
   int  cbFree = cb - cbUsed;
   
   if (cbFree < cbMore) 
   {
      cb += cbMore - cbFree + cbAllocExtra;
      
      pMem = realloc(pMem, cb);
      if (!pMem) 
      {
         return WE_NOMEMORY;
      }
      
      *ppMem = pMem;
      *pcb   = cb;
   }
   
   return WE_SUCCESS;
}

int wevector_InsertAt(IWEVector *me, uint32 nIndex, void *pvItem)
{
   int nErr;
   
   if (nIndex > me->nItems)
   {
      nIndex = me->nItems;
   }
   
   nErr = wevector_MemGrowEx(me, (void *)&me->ppItems, &me->cbSize, me->cbUsed, 
      sizeof(void *), (me->nGrowBy * sizeof(void *)));
   
   if (!nErr) 
   {
      wevector_ExpandItems(me, nIndex, 1);
      me->ppItems[nIndex] = pvItem;
   }
   
   return nErr;
}

int wevector_Insert(IWEVector *me, void *pvItem)
{
   int nErr;
   uint32 nIndex;   
   
   nIndex = me->nItems;
   
   nErr = wevector_MemGrowEx(me, (void *)&me->ppItems, &me->cbSize, me->cbUsed, 
      sizeof(void *), (me->nGrowBy * sizeof(void *)));
   
   if (!nErr) 
   {
      wevector_ExpandItems(me, nIndex, 1);
      me->ppItems[nIndex] = pvItem;
   }
   
   return nErr;
}

int wevector_DeleteAt(IWEVector *me, uint32 nIndex)
{
   int nErr = wevector_FreeItem(me, nIndex);
   if (!nErr) 
   {
      wevector_ExpandItems(me, nIndex, 0);
   }
   return nErr;
}


void wevector_DeleteAll(IWEVector *me)
{
   uint32 i;
   for (i=0; i < me->nItems; i++) 
   {
      wevector_FreeItem(me, i);
   }
   me->nItems = 0;
   me->cbUsed = 0;
}

/*
static WEPFNNOTIFY wevector_SetPfnFree(IWEVector *me, WEPFNNOTIFY pfnFree) 
{
WEPFNNOTIFY pfnOld = me->pfnFree;
me->pfnFree = pfnFree;
return pfnOld; // return old pfn
}
*/

int wevector_Size(IWEVector *me) 
{
   return me->nItems;
}

int wevector_New(IWEVector **ppo)
{
   IWEVector *me = NULL;
   int    iRet = WE_SUCCESS;
         
   do 
   {
      me = malloc(sizeof(IWEVector));
      if(NULL == me)
      {
         iRet = WE_ERROR;
         break;
      }
      memset(me, 0x00, sizeof(IWEVector));     

      me->nGrowBy = 10;
      *ppo = me;
      
   } while (0);   
   
   return iRet;   
}

WEPFNNOTIFY wevector_SetPfnFree(IWEVector *me, WEPFNNOTIFY pfnFree)
{
	WEPFNNOTIFY pfnOld = me->pfnFree;
	me->pfnFree = pfnFree;
	return pfnOld; // return old pfn
}

// WE_INT wevector_EnsureCapacity(IWEVector *me, WE_UINT32 nRequired, WE_UINT32 nGrowBy)
// {
//    WE_INT nErr = WE_SUCCESS;
// 
//    WE_UINT32 nAlloc  = me->cbSize * sizeof(WE_VOID*);
// 
//    me->nGrowBy = nGrowBy;
// 
//    if (nAlloc < nRequired) {
//       WE_INT cbAlloc = (nRequired - nAlloc) * sizeof(WE_VOID*);
//       nErr = MemGrow((WE_VOID*)&me->ppItems, &me->cbSize, me->cbSize, cbAlloc);
//    }
//    return nErr;
// }

/*
static WE_UINT32 wevector_AddRef(IWEVector *me)
{
return ++me->uiRef;
}

  static WE_UINT32 wevector_Release(IWEVector *me)
  {
  if(--me->uiRef > 0)
  return me->uiRef;
  
    wevector_Delete(me);
    return 0;
    }
    
      
        WE_INT wevector_loader(WE_VOID **ppObj, IWEShell *pIWEShell)
        {
        IWEMem    *pIMemMgr = NULL;
        IWEVector *me = NULL;
        WE_INT    iRet = WE_SUCCESS;
        
          if(NULL == pIWEShell)
          {
          return WE_BADPARM;
          }
          
            IWESHELL_CreateInstance(pIWEShell, WECLSID_MEMORY, (WE_VOID**)&pIMemMgr);
            if(NULL == pIMemMgr)
            {
            return WE_NOMEMORY;
            }
            
              IWEMEM_SetContral(pIMemMgr, TRUE);
              
                do 
                {
                me = IWEMEM_Malloc(pIMemMgr, sizeof(IWEVector));
                WE_BREAKNULL(me, iRet, WE_NOMEMORY, "no memory");
                WE_MEMSET(me, 0x00, sizeof(IWEVector));
                
                  me->uiRef = 1;
                  me->pvt = &me->svt;
                  me->nGrowBy = 10;
                  me->pIMemMgr = pIMemMgr;
                  IWEMEM_AddRef(pIMemMgr);
                  me->pIWEShell = pIWEShell;
                  IWESHELL_AddRef(pIWEShell);
                  
                    me->svt.AddRef    = wevector_AddRef;
                    me->svt.Release   = wevector_Release;
                    me->svt.Size      = wevector_Size;
                    me->svt.GetAt     = wevector_GetAt;
                    me->svt.ReplaceAt = wevector_ReplaceAt;
                    me->svt.InsertAt  = wevector_InsertAt;
                    me->svt.DeleteAt  = wevector_DeleteAt;
                    me->svt.DeleteAll = wevector_DeleteAll;
                    me->svt.SetPfnFree = wevector_SetPfnFree;
                    
                      *ppObj = me;
                      
                        } while (0);
                        
                          IWEMEM_Release(pIMemMgr);
                          
                            return iRet;
                            }
                            */
