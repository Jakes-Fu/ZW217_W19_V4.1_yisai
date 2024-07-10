/*============================================================================*
 *
 *                          Copyright (c) 2003 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      MarkSet.h
 *
 * Description:
 *      Manage all marks of the DspLogger test points
 *============================================================================*/

#if !defined(AFX_MARKSET_H__98ED7427_3388_4CAD_A3FC_FC1E4A4EAC41__INCLUDED_)
#define AFX_MARKSET_H__98ED7427_3388_4CAD_A3FC_FC1E4A4EAC41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#define DEFAULT_MARK_STRING "Default"
typedef CArray<ULONG,ULONG> MarkArray;

class CMarkSet
{
public:
	CMarkSet();
	virtual ~CMarkSet();

public:
    // Add a mark string
    void AddMark(CString& strMark);

    // Remove a mark string
    void RemoveMark(CString& strMark);
    void RemoveMark(int nIndex);

    // Remove current mark
    void RemoveCurMark();

    // Retrieve mark string
    BOOL GetMarkString(CString&);
    BOOL GetMarkString(int nMarkIndex,CString&);
	BOOL GetMarkStringEx(int nIndex, int& nElement, CString& strMark);

    // Add an element to current mark
    void AddElementToCurMark(ULONG nElement);

    // Add an element to specified mark
    void AddElement(ULONG nElement,CString& strMark);
    void AddElement(ULONG nElement,int nIndex);

    // Remove an element from current mark
    void RemoveElementFromCurMark(ULONG nElement);

    // Remove all elements from current mark
    void RemoveAllElements();

    // Remove all element in all marks
    void RemoveAllMarkElements();

    // Remove an element from specified mark
    void RemoveElement(ULONG nElement,CString& strMark);
    void RemoveElement(ULONG nElement,int nMarkIndex);

    //Remove all elements from marks
    void RemoveAllElements(CString& strMark);
    void RemoveAllElements(int nMarkIndex);

    // Get specified element from specified mark
    ULONG GetElement(CString& strMark,int nEleIndex);
    ULONG GetElement(int nMarkIndex,int nEleIndex);

    // Get specified element in current mark
    ULONG GetElement(int nEleIndex);

    // Get current element in current mark
    ULONG GetElement();

    // Get element count 
    int GetElementCount(CString& strMark);
    int GetElementCount(int nMarkIndex);

    // Get current element count
    int GetElementCount();
	
	int GetAllElementCount();

    // Get mark count
    int GetMarkCount()
    {
        return m_aMarkStrings.GetSize();
    }

    // Get next element in current mark
    ULONG Next(BOOL bRewind);

    // GetPrevious element in current mark
    ULONG Previous(BOOL bRewind);

    // Find element in the whole mark set
    BOOL FindElement(const ULONG nElement,int& nMarkIndex,int& nEleIndex,CString& strMark);

    // Change current mark
    void ChangeMark(CString& strMark);
    void ChangeMark(int nIndex);
	//@andybian 2006-8-30
	BOOL ChangeCurMark(int nCurMark,int nCurElement= 0);

protected:
    // Find mark string
    inline int FindMark(CString& strMark);

    // Find an element in specified mark
    inline int FindElement(ULONG nElement,int nMarkIndex);

protected:
    // Mark string array
    CStringArray m_aMarkStrings;

    // Mark index arrays
    // the dimension must equal to m_aMarkStrings
    CArray<MarkArray*,MarkArray*> m_aMarkIndexes;

    // Current mark index
    int m_nCurMark;

    // Current element index in current mark
    int m_nCurElement;
};

#endif // !defined(AFX_MARKSET_H__98ED7427_3388_4CAD_A3FC_FC1E4A4EAC41__INCLUDED_)
