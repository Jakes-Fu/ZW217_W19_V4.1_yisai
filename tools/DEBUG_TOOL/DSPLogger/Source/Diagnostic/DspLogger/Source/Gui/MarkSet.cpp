// MarkSet.cpp: implementation of the CMarkSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dsplogger.h"
#include "MarkSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define INVALID_MARK 0xFFFFFFFF
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMarkSet::CMarkSet()
{
    m_nCurMark = INVALID_MARK;
    m_nCurElement = INVALID_MARK;
}

CMarkSet::~CMarkSet()
{
    for(int i=0;i<m_aMarkIndexes.GetSize();i++)
    {
        delete m_aMarkIndexes[i];
    }
}

// Add a mark string
void CMarkSet::AddMark(CString& strMark)
{
    int nMarkIndex = FindMark(strMark);
    if(nMarkIndex != INVALID_MARK)
    {
        // Already have this mark
        return;
    }

    m_aMarkStrings.Add(strMark);

    // Add relative mark element array
    MarkArray* pArr = new MarkArray;
	if (pArr)
	{
		m_aMarkIndexes.Add(pArr);
	}
    
}

// Remove a mark string
void CMarkSet::RemoveMark(CString& strMark)
{
    int nMarkIndex = FindMark(strMark);
    if(nMarkIndex == INVALID_MARK)
    {
        // Do not have this mark
        return;
    }

    RemoveMark(nMarkIndex);
}

// Remove a mark string
void CMarkSet::RemoveMark(int nMarkIndex)
{
    // Check parameter first
    if(nMarkIndex < 0)
    {
        return;
    }

    int nCount = m_aMarkStrings.GetSize();
    if(nMarkIndex >= nCount)
    {
        return;
    }

    // Remove all element relative to this mark first
    delete m_aMarkIndexes[nMarkIndex];
    m_aMarkIndexes.RemoveAt(nMarkIndex);

    // Remove from mark array now
    m_aMarkStrings.RemoveAt(nMarkIndex);

    // Adjust current mark index
    if(m_nCurMark == m_aMarkStrings.GetSize())
    {
        m_nCurMark--;
    }
}

// Remove current mark
void CMarkSet::RemoveCurMark()
{
    if(m_nCurMark == INVALID_MARK)
    {
        return;
    }

    RemoveMark(m_nCurMark);    
}

// Add an element to current mark
void CMarkSet::AddElementToCurMark(ULONG nElement)
{
    // Check parameter first
    if(m_nCurMark == INVALID_MARK)
    {
        return;
    }

    AddElement(nElement,m_nCurMark);
}

// Add an element to specified mark
void CMarkSet::AddElement(ULONG nElement,CString& strMark)
{
    // Find mark first
    int nMarkIndex = FindMark(strMark);
    if(nMarkIndex == INVALID_MARK)
    {
        // Have not this mark now
        // add it to the array first
        AddMark(strMark);
        nMarkIndex = m_aMarkStrings.GetSize() - 1;
    }

    AddElement(nElement,nMarkIndex);
}

// Add an element to specified mark
void CMarkSet::AddElement(ULONG nElement,int nMarkIndex)
{
    // check parameter first
    if(nMarkIndex < 0)
    {
        return;
    }

    int nCount = m_aMarkStrings.GetSize();
    if(nMarkIndex >= nCount)
    {
        return;
    }

    int nEleIndex = FindElement(nElement,nMarkIndex);
    if(nEleIndex >= 0)
    {
        // already have this element
        return;
    }

    nEleIndex = -nEleIndex;
    nEleIndex--;

    m_aMarkIndexes[nMarkIndex]->InsertAt(nEleIndex,nElement);

    m_nCurMark = nMarkIndex;
    m_nCurElement = 0;
}

// Remove an element from current mark
void CMarkSet::RemoveElementFromCurMark(ULONG nElement)
{
    // Check parameter first
    if(m_nCurMark == INVALID_MARK)
    {
        return;
    }

    RemoveElement(nElement,m_nCurMark);
}

// Remove an element from specified mark
void CMarkSet::RemoveElement(ULONG nElement,CString& strMark)
{
    int nMarkIndex = FindMark(strMark);
    if(nMarkIndex == INVALID_MARK)
    {
        return;
    }

    RemoveElement(nElement,nMarkIndex);
}

// Remove an element from specified mark
void CMarkSet::RemoveElement(ULONG nElement,int nMarkIndex)
{
    // Check parameter first
    if(nMarkIndex < 0)
    {
        return;
    }

    int nCount = m_aMarkStrings.GetSize();
    if(nMarkIndex >= nCount)
    {
        return;
    }

    int nEleIndex = FindElement(nElement,nMarkIndex);
    if(nEleIndex < 0)
    {
        // Not found 
        return;
    }
	
	m_aMarkIndexes[nMarkIndex]->RemoveAt(nEleIndex);
	//
	if(0==m_aMarkIndexes[nMarkIndex]->GetSize())
	{
		RemoveMark(nMarkIndex);		
		m_nCurMark = INVALID_MARK;
		m_nCurElement = INVALID_MARK;
	}


}

// Remove all elements from current mark
void CMarkSet::RemoveAllElements()
{
    if(m_nCurMark == INVALID_MARK)
    {
        return;
    }

    RemoveAllElements(m_nCurMark);
}

// Remove all element in all marks
void CMarkSet::RemoveAllMarkElements()
{
    for(int i=0;i<m_aMarkStrings.GetSize();i++)
    {
        RemoveAllElements(i);
    }
	m_aMarkStrings.RemoveAll();
    m_nCurMark = INVALID_MARK;
    m_nCurElement = INVALID_MARK;
}


//Remove all elements from marks
void CMarkSet::RemoveAllElements(CString& strMark)
{
    int nMarkIndex = FindMark(strMark);
    if(nMarkIndex == INVALID_MARK)
    {
        return;
    }

    RemoveAllElements(nMarkIndex);
}

//Remove all elements from marks
void CMarkSet::RemoveAllElements(int nMarkIndex)
{
    // Check parameter first
    if(nMarkIndex < 0)
    {
        return;
    }

    int nCount = m_aMarkStrings.GetSize();
    if(nMarkIndex >= nCount)
    {
        return;
    }

    m_aMarkIndexes[nMarkIndex]->RemoveAll();
}

// Get specified element from specified mark
ULONG CMarkSet::GetElement(CString& strMark,int nEleIndex)
{
    int nMarkIndex = FindMark(strMark);
    if(nMarkIndex == INVALID_MARK)
    {
        // can't find
        return INVALID_MARK;
    }

    return GetElement(nMarkIndex,nEleIndex);
}

// Get specified element from specified mark
ULONG CMarkSet::GetElement(int nMarkIndex,int nEleIndex)
{
    // Check parameter first
    if(nMarkIndex < 0)
    {
        return INVALID_MARK;
    }

    int nCount = m_aMarkStrings.GetSize();
    if(nMarkIndex >= nCount)
    {
        return INVALID_MARK;
    }

    if(nEleIndex < 0)
    {
        return INVALID_MARK;
    }

    nCount = m_aMarkIndexes[nMarkIndex]->GetSize();
    if(nEleIndex >= nCount)
    {
        return INVALID_MARK;
    }

    return m_aMarkIndexes[nMarkIndex]->GetAt(nEleIndex);
}

// Get specified element in current mark
ULONG CMarkSet::GetElement(int nEleIndex)
{
    if(m_nCurMark == INVALID_MARK)
    {
        return INVALID_MARK;
    }

    return GetElement(m_nCurMark,nEleIndex);
}

// Get current element in current mark
ULONG CMarkSet::GetElement()
{
    return GetElement(m_nCurElement);
}

// Get element count 
int CMarkSet::GetElementCount(CString& strMark)
{
    int nMarkIndex = FindMark(strMark);
    if(nMarkIndex == INVALID_MARK)
    {
        // can't find
        return INVALID_MARK;
    }

    return GetElementCount(nMarkIndex);
}
int CMarkSet::GetElementCount(int nMarkIndex)
{
    // Check parameter first
    if(nMarkIndex < 0)
    {
        return INVALID_MARK;
    }

    int nCount = m_aMarkStrings.GetSize();
    if(nMarkIndex >= nCount)
    {
        return INVALID_MARK;
    }
    
    return m_aMarkIndexes[nMarkIndex]->GetSize();
}

// Get current element count
int CMarkSet::GetElementCount()
{
    if(m_nCurMark == INVALID_MARK)
    {
        return INVALID_MARK;
    }

    return GetElementCount(m_nCurMark);
}

int CMarkSet::GetAllElementCount()
{
	int nMarkCount = GetMarkCount();
	int nAllElementCount = 0;
	for(int i = 0; i < nMarkCount; i++)
	{
		nAllElementCount += m_aMarkIndexes[i]->GetSize();
	}

	return nAllElementCount;
}

// Get next element in current mark
ULONG CMarkSet::Next(BOOL bRewind)
{
    if(m_nCurMark == INVALID_MARK)
    {
        return INVALID_MARK;
    }
    
    m_nCurElement++;

    int nCount = m_aMarkIndexes[m_nCurMark]->GetSize();
    
    if(m_nCurElement >= nCount)
    {
        if(bRewind)
        {
            m_nCurElement = 0;
        }
        else
        {
            return INVALID_MARK;
        }
    }

    return m_aMarkIndexes[m_nCurMark]->GetAt(m_nCurElement);
}

// GetPrevious element in current mark
ULONG CMarkSet::Previous(BOOL bRewind)
{
    if(m_nCurMark == INVALID_MARK)
    {
        return INVALID_MARK;
    }

    m_nCurElement--;

    int nCount = m_aMarkIndexes[m_nCurMark]->GetSize();
    
    if(m_nCurElement == INVALID_MARK)
    {
        if(bRewind)
        {
            m_nCurElement = nCount - 1;
        }
        else
        {
            return INVALID_MARK;
        }
    }

    return m_aMarkIndexes[m_nCurMark]->GetAt(m_nCurElement);
}

// Retrieve mark string
BOOL CMarkSet::GetMarkString(CString& strMark)
{
    if(m_nCurMark == INVALID_MARK)
    {
        return FALSE;
    }

    return GetMarkString(m_nCurMark,strMark);
}

BOOL CMarkSet::GetMarkString(int nMarkIndex,CString& strMark)
{
    if(nMarkIndex == INVALID_MARK)
    {
        return FALSE;
    }

    int nCount = m_aMarkStrings.GetSize();
    if(nMarkIndex >= nCount)
    {
        return FALSE;
    }

    strMark = m_aMarkStrings[nMarkIndex];

    return TRUE;
}

BOOL CMarkSet::GetMarkStringEx(int nIndex, int& nElement, CString& strMark)
{
	int nMarkCount = GetMarkCount();
	int nAllElementCount = 0;
	int nLastCount = 0;
	for(int i = 0; i < nMarkCount; i++)
	{
		nAllElementCount += m_aMarkIndexes[i]->GetSize();
		if(nIndex < nAllElementCount)
		{
			strMark = m_aMarkStrings[i];
			nElement = m_aMarkIndexes[i]->GetAt(nIndex - nLastCount);
			break;
		}
		nLastCount = nAllElementCount;
	}

	return TRUE;
	
}

// Find element in the whole mark set
BOOL CMarkSet::FindElement(const ULONG nElement,int& nMarkIndex,int& nEleIndex,CString& strMark)
{
    for(nMarkIndex=0;nMarkIndex<m_aMarkStrings.GetSize();nMarkIndex++)
    {
        nEleIndex = FindElement(nElement,nMarkIndex);
        if(nEleIndex >= 0)
        {
            // Found
            GetMarkString(nMarkIndex,strMark);
            return TRUE;
        }
    }

    return FALSE;
}

// Change current mark
void CMarkSet::ChangeMark(CString& strMark)
{
    int nIndex = FindMark(strMark);
    if(nIndex == INVALID_MARK)
    {
        return;
    }

    ChangeMark(nIndex);
}

void CMarkSet::ChangeMark(int nIndex)
{
    // Check parameter first
    if(nIndex < 0)
    {
        return;
    }

    int nCount = m_aMarkStrings.GetSize();
    if(nIndex >= nCount)
    {
        return;
    }

    m_nCurMark = nIndex;
    m_nCurElement = 0;
}


// Find mark string
inline
int CMarkSet::FindMark(CString& strMark)
{
    for(int i=0;i<m_aMarkStrings.GetSize();i++)
    {
        if(!strMark.Compare(m_aMarkStrings[i]))
        {
            // Found
            return i;
        }
    }

    // Not found
    return INVALID_MARK;
}

// Find an element in specified mark
// If found,return its index
// else,because all array are sorted,
// so find the first element greater than given one
// then return negtive of the index plus one
inline
int CMarkSet::FindElement(ULONG nElement,int nMarkIndex)
{
    // Do not check parameter for efficiency
    MarkArray* pArr = m_aMarkIndexes[nMarkIndex];

    for(int i=0;i<pArr->GetSize();i++)
    {
        if(nElement == pArr->GetAt(i))
        {
            // Found
            return i;
        }

        if(nElement < pArr->GetAt(i))
        {
            break;
        }
    }

    // zero maybe ambiguity,so plus one
    i++;
    return (0 - i);
}

BOOL CMarkSet::ChangeCurMark(int nCurMark,int nCurElement)
{
   // Check parameter first
    int nCount = m_aMarkStrings.GetSize();
    if(nCurMark >= nCount && nCurMark < 0)
		return false;

    nCount = m_aMarkIndexes[nCurMark]->GetSize();
	if (nCurElement >= nCount && nCurElement < 0)
		return false;
    
	m_nCurMark = nCurMark;
	m_nCurElement=nCurElement;
	return true;
}