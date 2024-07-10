/*******************************************************************************
Copyright(c) 2005 - 2008 Hisense Mobile Commnunications Technology Co,Ltd
All Rights Reserved. By using this module you agree to the terms of the
Hisense Mobile Commnunications Technology Co,Ltd License Agreement for it.
*******************************************************************************/
/******************************************************************************
*Filename    : List.c
*
*Description: 
*
*Notes        :N/A
*
*---------------------------------------------------------------------------
*Change History: 
*----------------------------------------------------------------------------
*
*2009-7-7 ,wangxiaolin,create originally.
*
 *******************************************************************************/


#ifdef WIN32
#include "std_header.h"
#endif
#include "PIMlibmem.h"
#include "../h/list.h"

void LstInit(PListType pList)
{/*lint !e18 !e516*/
	pList->HEAD = NULL;
	pList->TAIL = NULL;
	pList->count=0;
}

void LstInsert(PListType pList, /*pointer to list descriptor*/
			   PNodeType pPrev, /*pointer to node after which to insert*/
			   PNodeType pNode  /*pointer to node to be inserted*/
			   ) 
{
	PNodeType pNext;

	if (pPrev==NULL)
	{
		pNext=pList->HEAD;  /*new node is to be first in list */
		pList->HEAD=pNode;
	}
	else
	{			/*make prev node point fwd to new */
		pNext=pPrev->next;
		pPrev->next=pNode;
	}
	
	if (pNext==NULL)
		pList->TAIL=pNode; /* new node is to be last in list*/
	else
		pNext->previous=pNode; /*make next node point back to new*/
	
	/*set pointers in new node, and update node count */
	pNode->next = pNext;
	pNode->previous = pPrev;

	pList->count++;

}


void LstAdd(PListType pList, /*pointer to list descriptor */
    	    PNodeType pNode /*pointer to node to be added */)
{/*lint !e18 !e516*/
	LstInsert(pList,pList->TAIL, pNode);
}


PNodeType LstGet (PListType pList /*ptr to list from which to get node*/)
{/*lint !e18 !e516*/
	PNodeType pNode=pList->HEAD;
	
	if (pNode!=NULL) /*is list empty?*/
	{
		pList->HEAD=pNode->next; /*make next node be 1st*/
		if (pNode->next==NULL) /*is there any next node ?*/
			pList->TAIL=NULL; /*no, list is empty*/
		else
			pNode->next->previous=NULL;/* yes, make it 1st node */
		pList->count--;  /*update node count*/
	}

	return(pNode);
}


void LstDelete(PListType pList, PNodeType pNode)
{
	if (pNode->previous==NULL)
 		pList->HEAD=pNode->next;
	else
		pNode->previous->next =pNode->next;

	if (pNode->next==NULL)
		pList->TAIL=pNode->previous;
	else
		pNode->next->previous=pNode->previous;

	pList->count--;
}

int  LstCount (PListType pList)
{		
	return (pList->count);
}

PNodeType LstNth(PListType pList, int nodenum)
{
	PNodeType pNode;

	if ((nodenum<1) || (nodenum>pList->count))
		return (NULL);
	
	if (nodenum<(pList->count>>1))
	{
		pNode=pList->HEAD;
		while (--nodenum>0)
			pNode=pNode->next;
	}
	else
	{
		nodenum-=pList->count;
		pNode=pList->TAIL;
		while (nodenum++<0)
			pNode=pNode->previous;
	}
	return (pNode);
}


PNodeType LstFirst(PListType pList)
{/*lint !e18 !e516*/
	return (pList->HEAD);
}

PNodeType LstNext(PNodeType pNode)
{/*lint !e18 !e516*/
	return (pNode->next);
}

int LstFind(PListType pList, PNodeType pNode)
{
	PNodeType	pNextNode;
	int index=1;

	pNextNode = LstFirst(pList);

	while ((pNextNode != NULL) && (pNextNode != pNode))
	{
		index++;
		pNextNode = LstNext(pNextNode);
	}

	if (pNextNode == NULL)
		return (-1);
	else
		return (index);
}

void LstFree(PListType pList)
{
	PNodeType pNode1;
	PNodeType pNode2;

	if (pList->count > 0)
	{
		pNode1 = pList->HEAD;
		while (pNode1 != NULL)
		{
			pNode2 = pNode1->next;
			xppFree(pNode1);
			pNode1 = pNode2;
		}

		pList->count = 0;
		pList->HEAD = pList->TAIL = NULL;
	}
}


