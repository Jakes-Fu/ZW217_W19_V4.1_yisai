// ArrayExt.cpp: implementation of the CArrayExt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "nveditor.h"
#include "ArrayExt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArrayExt::CArrayExt()
{
   m_pListHead      = NULL;
   m_pListTail      = NULL;

   m_dwTypeSize     = 0;
   m_dwArraySize    = 0;
   m_dwTotalSize    = 0;
}

CArrayExt::~CArrayExt()
{
    RemoveAll();
    
    if(NULL != m_pListHead )
    {
        delete m_pListHead;
    }
    
    m_pListHead      = NULL;
    m_pListTail      = NULL;
}

int CArrayExt::GetSize() const
{
    return m_dwArraySize;
}

void CArrayExt::RemoveAll()
{
    //删除客户端连接信息列表
    Array_item_Head_PTR  pListHeadNode=NULL;
    Array_item_Head_PTR  pListTailNode=NULL; 
    Array_item_Head_PTR  pDeleListNode=NULL; 
    
    if((m_pListHead != NULL) && (m_pListTail != NULL))
    {
        pListHeadNode = m_pListHead;
        pDeleListNode = pListTailNode = m_pListTail;       
        
        while(pListHeadNode != pListTailNode)
        {
            pListTailNode = pListTailNode->pPrevNode ;
            pListTailNode->pNextNode = pListHeadNode;
            pListHeadNode->pPrevNode = pListTailNode;
            
            delete pDeleListNode;
            
            pDeleListNode = pListTailNode;
        }    
    }

    m_pListTail = m_pListHead ;        
    m_dwArraySize = 0;

}

void *CArrayExt::GetAt(int nIndex) const
{
    Array_item_Head_PTR       pGetNode;
    BOOL                      bFound = FALSE;
    
    if((0 > nIndex) || (m_dwArraySize <= nIndex))
    {
        return NULL;
    }
    
    pGetNode = m_pListHead->pNextNode;
    while(pGetNode != m_pListHead)
    {
        if(pGetNode->dwIndex == nIndex)
        {
            bFound = TRUE;
            break;
        }
        
        pGetNode = pGetNode->pNextNode ;
    }
    
    if(FALSE == bFound)
    {
        return NULL;
    }

    return (void *)((char *)pGetNode + sizeof(Array_item_Head));
}

BOOL CArrayExt::SetAt(int nIndex, void *pnewElement)
{
    Array_item_Head_PTR       pGetNode;
    BOOL                      bFound = FALSE;
    
    if((0 > nIndex) || (m_dwArraySize <= nIndex))
    {
        return FALSE;
    }
    
    pGetNode = m_pListHead->pNextNode;
    while(pGetNode != m_pListHead)
    {
        if(pGetNode->dwIndex == nIndex)
        {
            bFound = TRUE;
            break;
        }
        
        pGetNode = pGetNode->pNextNode ;
    }
    
    if(FALSE == bFound)
    {
        return FALSE;
    }
    
    memcpy(((char *)pGetNode + sizeof(Array_item_Head)), pnewElement, m_dwTypeSize);

    return TRUE;
}

int CArrayExt::Add(void *pnewElement)
{
    Array_item_Head_PTR       pAddListNode = NULL;    

    pAddListNode = (Array_item_Head_PTR)new char[m_dwArrayItemSize];
    if(NULL == pAddListNode)
    {
        return -1;
    }
    ZeroMemory(pAddListNode, m_dwArrayItemSize);

    memcpy(((char *)pAddListNode + sizeof(Array_item_Head)), pnewElement, m_dwTypeSize);

    pAddListNode->dwIndex = m_dwArraySize++;

    //Add the new client to tailor
    pAddListNode ->pPrevNode = m_pListTail;
    pAddListNode ->pNextNode = m_pListHead;
    m_pListTail->pNextNode   = pAddListNode;
    m_pListHead->pPrevNode   = pAddListNode;
    m_pListTail = pAddListNode;
    
    return pAddListNode->dwIndex;
}

BOOL CArrayExt::SetTypeSize(DWORD nTypeSize)
{
   if(0 >= nTypeSize)
   {
       return FALSE;
   }

   m_dwTypeSize = nTypeSize;
   m_dwArrayItemSize = m_dwTypeSize + sizeof(Array_item_Head);

   //Initilize the list
   //add a empty node to list
   m_pListHead = (Array_item_Head_PTR)new char[m_dwArrayItemSize];
   if(NULL == m_pListHead )
   {
       return FALSE;
   }
   
   ZeroMemory(m_pListHead,m_dwArrayItemSize);

   m_pListHead->pPrevNode      = m_pListHead;
   m_pListHead->pNextNode      = m_pListHead;   
   m_pListTail                 = m_pListHead;   
   m_dwArraySize               = 0;

   return TRUE;
}

BOOL CArrayExt::RemoveAt(int nIndex, int nCount)
{
   Array_item_Head_PTR       pDeleteListNode;
   BOOL                      bFound = FALSE;
   
   if((0 >= nIndex) || (m_dwArraySize <= nIndex))
   {
       return FALSE;
   }

   pDeleteListNode = m_pListHead->pNextNode;
   while(pDeleteListNode != m_pListHead)
   {
       if(pDeleteListNode->dwIndex == nIndex)
       {
          bFound = TRUE;
          break;
       }

       pDeleteListNode = pDeleteListNode->pNextNode ;
   }
   
   if(FALSE == bFound)
   {
       return FALSE;
   }
   
   Array_item_Head_PTR   pNextNode;
   pNextNode = pDeleteListNode->pNextNode;   
   while(pNextNode != m_pListHead)
   {
      pNextNode->dwIndex = pNextNode->dwIndex - 1;
      pNextNode = pNextNode->pNextNode ;
   }

   pDeleteListNode->pPrevNode ->pNextNode = pDeleteListNode->pNextNode ;
   pDeleteListNode->pNextNode ->pPrevNode = pDeleteListNode->pPrevNode ;
   m_pListTail = m_pListHead->pPrevNode;
   delete pDeleteListNode;

   m_dwArraySize--;

   return TRUE;
   
}
