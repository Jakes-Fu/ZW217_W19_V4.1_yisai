// BufPool.h: interface for the CBufPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFPOOL_H__ED88E4C4_7474_4790_A077_D9EE5CDD6F63__INCLUDED_)
#define AFX_BUFPOOL_H__ED88E4C4_7474_4790_A077_D9EE5CDD6F63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DspCommon.h"

class CBufPool  
{
public:
	CBufPool();
	virtual ~CBufPool();

public:
    void Append(BYTE* pData,int uSize);
    void Remove(int uSize);
    int GetSize();
    void Copy(BYTE* pDes,int uSize);

    void Reset()
    {
        m_pReadPt = m_pWritePt = m_pBuf;
    }

protected:
    void ReAlloc(int newsize);

protected:
    BYTE* m_pBuf;
    int m_nTotalSize;
    const int m_cUnitSize;

    BYTE* m_pReadPt;
    BYTE* m_pWritePt;
};

#endif // !defined(AFX_BUFPOOL_H__ED88E4C4_7474_4790_A077_D9EE5CDD6F63__INCLUDED_)
