// ArrayExt.h: interface for the CArrayExt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARRAYEXT_H__A2A38E49_7CBD_4F98_B68C_AC9F9098FF37__INCLUDED_)
#define AFX_ARRAYEXT_H__A2A38E49_7CBD_4F98_B68C_AC9F9098FF37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_ARRAY_ITEM_NUM        (0x40)
#define DEFAULT_ARRAY_ITEM_GROW_NUM   (0x10)

typedef struct ARRAY_ITEM_HEAD_STRU
{
    struct ARRAY_ITEM_HEAD_STRU *pPrevNode;
    struct ARRAY_ITEM_HEAD_STRU *pNextNode;
    DWORD  dwIndex;
    //char arItemContent[nTypeSize];
}Array_item_Head, *Array_item_Head_PTR;


class CArrayExt  
{
public:
	BOOL RemoveAt(int nIndex, int nCount = 0);
	BOOL SetTypeSize(DWORD nTypeSize);
	int Add(void *pnewElement);
	BOOL SetAt(int nIndex, void *pnewElement);
	void *GetAt(int nIndex) const;
	void RemoveAll();	
	int GetSize() const;
	CArrayExt();
	virtual ~CArrayExt();
private:
    DWORD m_dwTypeSize;
    DWORD m_dwArrayItemSize;
    DWORD m_dwArraySize;    
    DWORD m_dwTotalSize;
    DWORD m_dwDeleteCount;

    Array_item_Head_PTR  m_pListTail;
    Array_item_Head_PTR  m_pListHead;
};


#endif // !defined(AFX_ARRAYEXT_H__A2A38E49_7CBD_4F98_B68C_AC9F9098FF37__INCLUDED_)
