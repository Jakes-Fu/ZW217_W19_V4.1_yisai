/*******************************************************************************
Copyright(c) 2005 - 2008 Hisense Mobile Commnunications Technology Co,Ltd
All Rights Reserved. By using this module you agree to the terms of the
Hisense Mobile Commnunications Technology Co,Ltd License Agreement for it.
*******************************************************************************/
/******************************************************************************
*Filename    : D:\T3901\t3901_0706\MS_MMI\source\mmi_app\app\pim\h\List.h
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

#ifndef PIM_LIST_H
#define PIM_LIST_H

#define HEAD	node.next
#define TAIL	node.previous


typedef struct node		/* Node of a linked list. */
{
    struct node *next;		/* Points at the next node in the list */
    struct node *previous;	/* Points at the previous node in the list */
} NodeType, *PNodeType;


typedef struct			/* Header for a linked list. */
{
    NodeType node;		/* Header list node */
    int count;			/* Number of nodes in list */
} ListType, *PListType; 


void	LstInit(PListType pList);
void	LstInsert(PListType pList, PNodeType pPrev, PNodeType pNode);
void	LstAdd(PListType pList, PNodeType pNode);
void	LstDelete(PListType pList, PNodeType pNode);
int	LstCount (PListType pList);
int	LstFind(PListType pList, PNodeType pNode);
void	LstFree(PListType pList);
PNodeType	LstGet (PListType pList);
PNodeType	LstFirst(PListType pList);
PNodeType	LstNth(PListType pList, int nodenum);
PNodeType LstNext(PNodeType pNode);

#endif
